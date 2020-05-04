#ifndef __SING_N_SPELL_SYNTH_LEXICON
#define __SING_N_SPELL_SYNTH_LEXICON

#include <mbed.h>
#include <rtos.h>

#define LEXICON_MAX_MEMORY      1024     

#define WORD_INVALID_LENGTH        (~((size_t)0))

namespace synth {

    /* A word ; structure is opaque */
    struct word_t;
    /* A word iterator */
    class worditerator_t;

    /* The pool of words */
    class Lexicon {

        private:
            /* The amount of memory used */
            static size_t m_usedMemory;        

        private:
            /* This class is static */
            Lexicon() {}

        public:
            /* Allocate a word */
            static word_t* alloc(size_t size);
            /* Reallocate a word */
            static bool resize(word_t*& word, size_t size);
            /* Reallocate a word, iff the new size is bigger */
            static bool enlarge(word_t*& word, size_t size);
            /* Shrink a word */
            static void shrink(word_t*& word);
            /* Deallocate a word */
            static void release(word_t*& word);

            /* Buffer size of a word */
            static size_t size(const word_t*& word);
            /* Length of a word */
            static size_t length(const word_t*& word);

            /* Gets the underlying buffer : can't be modified */
            static const uint8_t* buffer(const word_t*& word);
            /* Get an iterator */
            static worditerator_t iterator(word_t*& word);

            /* Available memory */
            __STATIC_FORCEINLINE size_t available() { return LEXICON_MAX_MEMORY - Lexicon::m_usedMemory; }
            /* Used memory */
            __STATIC_FORCEINLINE size_t used() { return Lexicon::m_usedMemory; }
            /* Does the pool fit a buffer this big ? */
            __STATIC_FORCEINLINE bool fits(size_t size) { return (LEXICON_MAX_MEMORY - Lexicon::m_usedMemory) >= size; }
        
        
    };

    /* A word iterator */
    class worditerator_t {
        /* Iterator making function is a friend */
        friend worditerator_t Lexicon::iterator(word_t*&);

        /* The underlying word */
        word_t* const m_word = nullptr;
        /* The position */
        mutable size_t m_position = 0;

        /* Make an iterator */
        worditerator_t(word_t* word, size_t position = 0) 
            : m_word(word), m_position(position) { }

        public:
            /* Get word */
            word_t* word() const;
            /* Next */
            bool next() const;
            /* Previous */
            bool previous() const;
            /* Get value at */
            uint8_t& at();
            /* Observe value at */
            const uint8_t& get() const;
    };

};

#endif