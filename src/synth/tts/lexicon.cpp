#include "lexicon.h"

using namespace synth;

/* A word */
struct synth::word_t {
    /* The buffer containing the phonemes */
    uint8_t* buffer;
    /* The size of the buffer */
    size_t size;
};

/* Get word */
word_t* worditerator_t::word() const {
    return this->m_word;
}

/* Next */
bool worditerator_t::next() const {
    /* If at end of buffer, can't go forward */
    if (this->m_position >= (this->m_word->size - 1)) { return false; }
    /* Else, advance */
    this->m_position++;
    /* OK */
    return true;
}
/* Previous */
bool worditerator_t::previous() const {
    /* If position = 0, can't go back */
    if (this->m_position == 0) { return false; }
    /* Else, go back */
    this->m_position--;
    /* OK */
    return true;
}

/* Get value at */
uint8_t& worditerator_t::at() {
    /* Return */
    return this->m_word->buffer[this->m_position];
}

/* Get value at */
const uint8_t& worditerator_t::get() const {
    /* Return */
    return this->m_word->buffer[this->m_position];
}


/* The amount of memory used */
size_t Lexicon::m_usedMemory = 0;

/* Gets the underlying buffer : can't be modified */
const uint8_t* Lexicon::buffer(const word_t*& word) {
    return word->buffer;
}
/* Get an iterator */
worditerator_t Lexicon::iterator(word_t*& word) {
    return worditerator_t(word);
}

/* Buffer size of a word */
size_t Lexicon::size(const word_t*& word) {
    return word != nullptr ? word->size : 0;
}

/* Length of a word */
size_t Lexicon::length(const word_t*& word) {
    /* If null, length is 0 */
    if (word == nullptr) { return WORD_INVALID_LENGTH; }
    /* Go through buffer */
    size_t len = 0;
    while (len < word->size) {
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
word_t* Lexicon::alloc(size_t size) {
    /* If size is zero, return null */
    if (size == 0) { return nullptr; }
    /* Not enough memory ? If so, return null.
       /!\ Special care taken to avoid overflows. */
    if (!Lexicon::fits(size)) {
        return nullptr;
    }
    /* Lock */
    CriticalSectionLock lock;
    /* Increment used memory */
    Lexicon::m_usedMemory += size;
    /* Allocate buffer */
    uint8_t* buffer = (uint8_t*)malloc(size * sizeof(uint8_t));
    /* If calloc failed, return null */
    if (buffer == nullptr) {
        return nullptr;    
    }
    /* Initialize buffer with full 0xFF */
    memset(buffer, ~((int)0), size);
    /* Return word */
    return new word_t { buffer, size };
}

/* Reallocate a word */
bool Lexicon::resize(word_t*& word, size_t size) {
    /* If size is zero, just free the word */
    if (size == 0) {  
        Lexicon::release(word);
        word = nullptr;
        return false;
    }
    /* If size equal, we did it */
    if (size == word->size) { return true; }
    /* If size is bigger, but not enough memory, we fail */
    if (size > word->size && !Lexicon::fits(size - word->size)) {
        return false;
    }
    /* Lock */
    CriticalSectionLock lock;
    /* Everything is ok. Update used memory counter */
    Lexicon::m_usedMemory -= word->size;
    Lexicon::m_usedMemory += size;
    /* Update size in word & reallocate buffer */
    word->size = size;
    word->buffer = (uint8_t*)realloc(word->buffer, size);
    /* If realloc failed, destroy word and return false */
    if (word->buffer == nullptr) {
        /* Release memory, then fail */
        Lexicon::release(word);
        word = nullptr;
        return false;
    } 
    /* All good ! */
    return true;
}

/* Reallocate a word, iff the new size is bigger */
bool Lexicon::enlarge(word_t*& word, size_t size) {
    /* Check if size is larger */
    if (size > word->size) {
        return Lexicon::resize(word, size);
    }
    /* If size is equal, say we did it. Else, say it failed */
    return word->size == size;
}

/* Deallocate a word */
void Lexicon::release(word_t*& word) {
    /* If null, ignore */
    if (word == nullptr) {
        return;
    }
    /* Lock */
    CriticalSectionLock lock;
    /* Update mem counter */
    Lexicon::m_usedMemory -= word->size;
    /* Release internal buffer */
    if (word->buffer != nullptr) {
        free(word->buffer);
    }
    /* Release word */
    delete word;
    /* Reinitialize word */
    word = nullptr;
}
