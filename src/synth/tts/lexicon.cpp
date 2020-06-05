#include "lexicon.h"

using namespace synth;

#define BIND_ERROR(x)      {if (!(x)) { return false; }}

/* A word */
struct synth::word_t {
    /* The buffer containing the phonemes */
    uint8_t* buffer;
    /* The bin it came from */
    Lexicon::Bin* bin;
};

/* Get word */
word_t* worditerator_t::word() const {
    return this->m_word;
}

/* Get the position */
size_t worditerator_t::position() const {
    return this->m_position;
}
/* Get the length */
size_t worditerator_t::length() const {
    return Lexicon::length(this->m_word);
}

            /* Get a n-m context for the element */
void worditerator_t::context(size_t n, size_t m, uint8_t* const& before, uint8_t* const& after, size_t& beforeN, size_t& afterM) const {
    /* If word is null, return */
    if (m_word == nullptr) { return; }
    /* Get the number of elements */
    afterM = min(m_word->bin->blockSize - 1 - this->m_position, m);
    beforeN = min(this->m_position, n);
    /* Copy them */
    if (afterM > 0) {
        memcpy(after, &m_word->buffer[m_position + 1], afterM);
    }
    memcpy(before, &m_word->buffer[m_position - beforeN], beforeN);
}

/* Select first element */
worditerator_t& worditerator_t::first() { 
    /* Set position */
    this->m_position = 0;
    /* Return self */
    return *this;
}

/* Select last element */
worditerator_t& worditerator_t::last() {
    /* If null, return */
    if (m_word == nullptr) { return *this; }
    /* Go at end */
    this->m_position = this->m_word->bin->blockSize - 1;
    /* Return self */
    return *this;
}

/* Select a value */
bool worditerator_t::select(size_t i) {
    /* If null, return */
    if (m_word == nullptr) { return false; }
    /* If invalid position, return false */
    if (i >= this->m_word->bin->blockSize) {
        return false;
    }
    /* Set */
    this->m_position = i;
    /* OK */
    return true;
}


/* Select end of sequence */
worditerator_t& worditerator_t::end() {
    /* If null, return */
    if (m_word == nullptr) { return *this; }
    /* Go at end */
    size_t len;
    if ((len = Lexicon::length(this->m_word)) != WORD_INVALID_LENGTH) {
        this->m_position = len;
    }
    /* Return self */
    return *this;
}

/* Next */
bool worditerator_t::next() {
    /* If word is null, return */
    if (m_word == nullptr) { return false; }
    /* If at end of buffer, can't go forward */
    if (this->m_position >= (this->m_word->bin->blockSize - 1)) { return false; }
    /* Else, advance */
    this->m_position++;
    /* OK */
    return true;
}

/* Peek the next element */
bool worditerator_t::peek(uint8_t& out) const {
    /* If word is null, return */
    if (m_word == nullptr) { return false; }
    /* If out of bounds, return  */
    if (this->m_position >= (this->m_word->bin->blockSize - 1)) { return false; }
    /* Return next element */
    out = this->m_word->buffer[this->m_position + 1];
    /* OK */
    return true;
}

/* Next */
bool worditerator_t::hasNext() {
    /* If word is null, return */
    if (m_word == nullptr) { return false; }
    /* Return */
    return this->m_position < (this->m_word->bin->blockSize - 1);
}

/* Previous */
bool worditerator_t::previous() {
    /* If word is null, return */
    if (m_word == nullptr) { return false; }
    /* If position = 0, can't go back */
    if (this->m_position == 0) { return false; }
    /* Else, go back */
    this->m_position--;
    /* OK */
    return true;
}

/* Get value at */
uint8_t& worditerator_t::at() {
    /* If word is null, error */
    MBED_ASSERT(m_word != nullptr);
    /* Return */
    return this->m_word->buffer[this->m_position];
}

/* Get value at */
const uint8_t& worditerator_t::get() const {
    /* If word is null, error */
    MBED_ASSERT(m_word != nullptr);
    /* Return */
    return this->m_word->buffer[this->m_position];
}

/* Insert */
worditerator_t& worditerator_t::insert() {
    /* Shift the memory */
    memmove(
        &this->m_word->buffer[this->m_position + 1], 
        &this->m_word->buffer[this->m_position], this->m_word->bin->blockSize - this->m_position - 1);
    /* Erase the block that's been created */
    this->m_word->buffer[this->m_position] = 0xFF;

    return *this;
}
/* Delete */
worditerator_t& worditerator_t::erase() {
    /* Shift the memory */
    if (this->m_position < this->m_word->bin->blockSize - 1) {
        memmove(
            &this->m_word->buffer[this->m_position], 
            &this->m_word->buffer[this->m_position + 1], this->m_word->bin->blockSize - this->m_position);
    }
    /* Erase the block that's been left */
    this->m_word->buffer[this->m_word->bin->blockSize - 1] = 0xFF;
    return *this;
}

/* The bins */
Lexicon::Bin Lexicon::m_bins[LEXICON_POOL_BINS_COUNT] = { 0 };
/* Setup the bin data */
constexpr size_t bindata[LEXICON_POOL_BINS_COUNT][2] = {LEXICON_POOL_BINS};
/* The pool memory */
uint8_t pool0[MBED_RTOS_STORAGE_MEM_POOL_MEM_SIZE(bindata[0][1], bindata[0][0])] = {0};
uint8_t pool1[MBED_RTOS_STORAGE_MEM_POOL_MEM_SIZE(bindata[1][1], bindata[1][0])] = {0};
uint8_t pool2[MBED_RTOS_STORAGE_MEM_POOL_MEM_SIZE(bindata[2][1], bindata[2][0])] = {0};
uint8_t pool3[MBED_RTOS_STORAGE_MEM_POOL_MEM_SIZE(bindata[3][1], bindata[3][0])] = {0};
uint8_t pool4[MBED_RTOS_STORAGE_MEM_POOL_MEM_SIZE(bindata[4][1], bindata[4][0])] = {0};
uint8_t* poolMemory[LEXICON_POOL_BINS_COUNT] = { pool0, pool1, pool2, pool3, pool4 };
mbed_rtos_storage_mem_pool_t poolObjMem[LEXICON_POOL_BINS_COUNT] = {0};

/* Setup the lexicon */
void Lexicon::setup() {
    /* Make an attribute */
    osMemoryPoolAttr_t attr = { 0 };
    /* Setup the bins */
    for (uint8_t i = 0; i < LEXICON_POOL_BINS_COUNT; i++) {
        /* Fill the sizes */
        Lexicon::m_bins[i].blockSize = bindata[i][0];
        Lexicon::m_bins[i].blockCount = bindata[i][1];
        /* Fill the attribute */
        attr.mp_mem = poolMemory[i];
        attr.mp_size = MBED_RTOS_STORAGE_MEM_POOL_MEM_SIZE(
            Lexicon::m_bins[i].blockCount, 
            Lexicon::m_bins[i].blockSize);
        attr.cb_mem = &poolObjMem[i];
        attr.cb_size = sizeof(poolObjMem[i]);
        /* Allocate pool */
        Lexicon::m_bins[i].pool = osMemoryPoolNew(m_bins[i].blockCount, m_bins[i].blockSize, &attr);
        /* Assert pool has been created */
        MBED_ASSERT(Lexicon::m_bins[i].pool != NULL);
    }
}

/* First fit bin for size */
bool Lexicon::firstFitBin(Lexicon::Bin*& bin, size_t size) {
    /* Check each bin */
    for (uint8_t i = 0; i < LEXICON_POOL_BINS_COUNT; i++) {
        /* Is the block size enough ? */
        if (Lexicon::m_bins[i].blockSize < size) { continue; }
        /* Is there enough space left in this bin ? */
        if (osMemoryPoolGetSpace(Lexicon::m_bins[i].pool) == 0) { continue; }
        /* We found a large enough bin, with blocks left. This is ok */
        bin = &Lexicon::m_bins[i];
        return true; 
    }
    /* No space */
    return false;
}

/* First fit bin for size */
bool Lexicon::firstFitBinRemoved(Lexicon::Bin*& bin, size_t size, Lexicon::Bin* const& removedBin) {
    /* Check each bin */
    for (uint8_t i = 0; i < LEXICON_POOL_BINS_COUNT; i++) {
        /* Is the block size enough ? */
        if (Lexicon::m_bins[i].blockSize < size) { continue; }
        /* Is there enough space left in this bin ? */
        if (Lexicon::m_bins[i].pool != removedBin->pool && osMemoryPoolGetSpace(Lexicon::m_bins[i].pool) == 0) { continue; }
        /* We found a large enough bin, with blocks left. This is ok */
        bin = &Lexicon::m_bins[i];
        return true; 
    }
    /* No space */
    bin = nullptr;
    return false;
}

/* First optimal bin for size */
bool Lexicon::optimalBin(Lexicon::Bin*& bin, size_t size) {
    /* Check each bin */
    for (uint8_t i = 0; i < LEXICON_POOL_BINS_COUNT; i++) {
        /* Is the block size enough ? */
        if (Lexicon::m_bins[i].blockSize < size) { continue; }
        /* We found the smallest bin */
        bin = &Lexicon::m_bins[i];
        return true; 
    }
    /* No space */
    return false;
}

/* Gets the underlying buffer : can't be modified */
const uint8_t* Lexicon::buffer(word_t* const& word) {
    return word->buffer;
}
/* Get an iterator */
worditerator_t Lexicon::iterator(word_t*& word) {
    return worditerator_t(word);
}

/* Buffer size of a word */
size_t Lexicon::size(word_t* const& word) {
    return word != nullptr ? word->bin->blockSize : 0;
}

/* Length of a word */
size_t Lexicon::length(word_t* const& word) {
    /* If null, length is 0 */
    if (word == nullptr) { return WORD_INVALID_LENGTH; }
    /* Go through buffer */
    size_t len = 0;
    while (len < word->bin->blockSize) {
        /* Check and increment length */
        if (word->buffer[len] == 0xFF) {
            return len;
        }
        /* Increment */
        ++len;
    }
    /* Not null terminated. Return error */
    return WORD_INVALID_LENGTH;
}

/* Allocate a word */
bool Lexicon::alloc(word_t*& word, size_t requested_size) {
    /* Ensure word is null and size != 0 */
    MBED_ASSERT(word == nullptr);
    MBED_ASSERT(requested_size > 0);
    /* Find smallest bin. */
    Lexicon::Bin* bin = nullptr;
    if (!Lexicon::firstFitBin(bin, requested_size)) {
        return false;
    }
    /* Allocate buffer */
    uint8_t* buffer = (uint8_t*)osMemoryPoolAlloc(bin->pool, 0);
    /* If failed, no more memory */
    if (buffer == nullptr) {
        return false;    
    }
    /* Initialize buffer with full 0xFF */
    memset(buffer, ~((int)0), bin->blockSize);
    /* Return word */
    word = new word_t { buffer, bin };
    /* Ok */
    return true;
}

/* Reallocate a word */
bool Lexicon::resize(word_t*& word, size_t required_new_size, bool copy) {
    /* If size is zero, just free the word */
    if (required_new_size == 0) {  
        Lexicon::release(word);
        word = nullptr;
        return true;
    }
    /* If word is null, error */
    if (word == nullptr) { return false; }

    /* If we freed the current buffer, what is the smallest bin
       that could fit the memory we ask for ? ? */
    Bin* idealBin = nullptr;
    if (!firstFitBinRemoved(idealBin, required_new_size, word->bin)) {
        /* It couldn't fit anyway. Fail */
        return false;
    }
    /* If bin size is equal, no need to resize or anything : we did it */
    if (idealBin->blockSize == word->bin->blockSize) {
        return true;
    }   
    /* Allocate new buffer. 
            The pool ALWAYS has space : if we're there, 
            target and src bin are of different size. Since firstFitRemoved 
            returned true, idealBin can fit the word, or it is of the same size
            than current word. Since second condition is false, it must be that
            idealBin can fit the word. */
    uint8_t* new_buffer = (uint8_t*)osMemoryPoolAlloc(idealBin->pool, 0);
    /* Check if buffer couldn't be allocated (can still happen for RTOS reasons) */
    if (new_buffer == nullptr) {
        return false;
    }
    /* Set to 0xFF */
    memset(new_buffer, ~((int)0), idealBin->blockSize);
    /* Copy old data if needed */
    if (copy) {
        memcpy(new_buffer, word->buffer, min(idealBin->blockSize, word->bin->blockSize));
    }
    /* Critical section ! Atomic operation only */
    {
        /* Lock */
        CriticalSectionLock lock;
        /* Free the old data */
        osMemoryPoolFree(word->bin->pool, word->buffer);
        /* Replace the old buffer by the new one */
        word->bin = idealBin;
        word->buffer = new_buffer;
    }
    /* All good ! */
    return true;
}

/* Reallocate a word, iff the new size is bigger */
bool Lexicon::enlarge(word_t*& word, size_t requested_size) {
    /* Check if size is larger */
    if (requested_size > word->bin->blockSize) {
        return Lexicon::resize(word, requested_size);
    }
    /* Else, it's all good */
    return true;
}

/* Shrink a word */
bool Lexicon::shrink(word_t*& word) {
    /* Compute length */
    size_t length = Lexicon::length(word);
    /* Reallocate, but with buffer size length + 1 */
    return Lexicon::resize(word, length+1);
}

/* Clone the original word into another, but shrinked */
bool Lexicon::shrinkCopy(word_t* const& source, word_t*& target, bool collapseZeroLength) {
    /* Compute len */
    size_t length = Lexicon::length(source);
    /* Assert size is valid */
    MBED_ASSERT(length != WORD_INVALID_LENGTH);
    
    /* Check if zero length must be collapsed */
    if (length == 0 && collapseZeroLength) {
        /* Release target */
        Lexicon::release(target);
        /* OK */
        return true;
    }    
    /* Compute target size */
    size_t size = length + 1;
    /* Prepare the target : check its status */
    if (target == nullptr) {
        /* Null ; we need to allocate such a buffer */
        BIND_ERROR(Lexicon::alloc(target, size));
    } else {
        /* Buffer already exists. Just reallocate but no copy */
        BIND_ERROR(Lexicon::resize(target, size, false));
    }
    /* Copy src buffer into target buffer */
    memcpy(target->buffer, source->buffer, size);
    /* We're good to go */
    return true;   
}

/* Copy the original word into another */
void Lexicon::copy(word_t* const& source, word_t*& target) {
    /* Compute sizes */
    auto target_size = Lexicon::size(target);
    auto source_size = Lexicon::size(source);
    /* Ensure sizes are ok */
    MBED_ASSERT(target_size >= source_size);
    
    /* Check if source isn't null */
    if (source_size > 0) {
        /* Copy live data */
        memcpy(target->buffer, source->buffer, source_size);
    }   
    /* If target is strictly bigger than source, pad 
        (guarantees that target isn't a null ptr, since if 
         target_size = 0 (<=> a null ptr), the condition is always false) */
    if (target_size > source_size) {
        memset(target->buffer + source_size, ~((int)0), target_size - source_size);
    }
}

/* Deallocate a word */
void Lexicon::release(word_t*& word) {
    /* If null, ignore */
    if (word == nullptr) {
        return;
    }
    /* Store old word */
    word_t* ptr = word;
    /* Enter critical */
    {
        /* Lock */
        CriticalSectionLock lock;
        /* Release internal buffer */
        if (word->buffer != nullptr) {
            osMemoryPoolFree(word->bin->pool, word->buffer);
        }
        /* Reinitialize word */
        word = nullptr;
    }
    /* Release word */
    delete ptr;
}
