#ifndef __SING_N_SPELL_SYNTH_LEXICON
#define __SING_N_SPELL_SYNTH_LEXICON

#include <mbed.h>
#include <rtos.h>

#define LEXICON_MAX_MEMORY      1024     

#define LEXICON_POOL_BINS_COUNT     5
#define LEXICON_POOL_BINS           {8, 128}, {16, 128}, {32, 64}, {64, 64}, {128, 32} /* MUST BE ORDERED !!! */

#define WORD_INVALID_LENGTH        (~((size_t)0))

namespace synth {

    /* A word ; structure is opaque */
    struct word_t;
    /* A word iterator */
    class worditerator_t;

    /* The pool of words */
    class Lexicon {

        public:
            /* A bin */
            struct Bin {
              /* The pool */
              osMemoryPoolId_t pool;
              /* The block size */
              size_t blockSize;  
              /* The number of blocks */
              size_t blockCount;  
            };

        private:  
            /* The bins */
            static Bin m_bins[LEXICON_POOL_BINS_COUNT];

        private:
            /* This class is static */
            Lexicon() {}

        public:
            /* Setup the lexicon */
            static void setup();

            /* Allocate a word */
            static bool alloc(word_t*& word, size_t size);
            /* Reallocate a word */
            static bool resize(word_t*& word, size_t size, bool copy = true);
            /* Reallocate a word, iff the new size is bigger */
            static bool enlarge(word_t*& word, size_t size);
            /* Shrink a word */
            static bool shrink(word_t*& word);
            /* Clone the original word into another, but shrinked */
            static bool shrinkCopy(word_t* const& source, word_t*& target, bool collapseZeroLength = false);
            /* Copy the original word into another */
            static void copy(word_t* const& source, word_t*& target);
            /* Deallocate a word */
            static void release(word_t*& word);

            /* Buffer size of a word */
            static size_t size(word_t* const& word);
            /* Length of a word */
            static size_t length(word_t* const& word);

            /* Gets the underlying buffer : can't be modified */
            static const uint8_t* buffer(word_t* const& word);
            /* Get an iterator */
            static worditerator_t iterator(word_t*& word);

        private: 
            /* First fit bin for size */
            static bool firstFitBin(Lexicon::Bin*& bin, size_t size);
            /* First fit bin for size, if we removed one block in some bin */
            static bool firstFitBinRemoved(Lexicon::Bin*& bin, size_t size, Lexicon::Bin* const& removedBin);
            /* Find optimal bin for size. Does not guarantee the bin is free */
            static bool optimalBin(Lexicon::Bin*& bin, size_t size);

            /* Available memory */
            //__STATIC_FORCEINLINE size_t available() { return LEXICON_MAX_MEMORY - Lexicon::m_usedMemory; }
            /* Used memory */
            //__STATIC_FORCEINLINE size_t used() { return Lexicon::m_usedMemory; }
            /* Does the pool fit a buffer this big ? */
            //__STATIC_FORCEINLINE bool fits(size_t size) { return (LEXICON_MAX_MEMORY - Lexicon::m_usedMemory) >= size; }
        
        
    };

    /* A word iterator */
    class worditerator_t {
        /* Iterator making function is a friend */
        friend worditerator_t Lexicon::iterator(word_t*&);

        /* The underlying word */
        word_t* m_word = nullptr;
        /* The position */
        mutable size_t m_position = 0;

        /* Make an iterator */
        worditerator_t(word_t* word, size_t position = 0) 
            : m_word(word), m_position(position) { }
 
        public:
            /* Copy an iterator */
            worditerator_t(const worditerator_t& word) 
                : m_word(word.m_word), m_position(word.m_position) {}

        public:
            /* Equality operator */
            inline bool operator==(const worditerator_t& other) const {
                return this->m_word == other.m_word 
                    && this->m_position == other.m_position;
            }
            /* Difference operator */
            inline bool operator!=(const worditerator_t& other) const {
                return !(*this == other);
            }

            /* The null iterator */
            static worditerator_t null() { return worditerator_t(nullptr, 0);  }

            /* Get word */
            word_t* word() const;
            /* Get a n-m context for the element */
            void context(size_t n, size_t m, uint8_t* const& before, uint8_t* const& after, size_t& beforeN, size_t& afterM) const;
            
            /* Get the position */
            size_t position() const;
            /* Get the length */
            size_t length() const;

            /* Select first element */
            void first() const;
            /* Select last element */
            void last() const;
            /* Select end of sequence */
            void end() const;
            /* Next */
            bool next() const;
            /* Previous */
            bool previous() const;
            /* Select a value at index i */
            bool select(size_t i) const;
            /* Get value at */
            uint8_t& at();
            /* Observe value at */
            const uint8_t& get() const;
    };

};

#endif