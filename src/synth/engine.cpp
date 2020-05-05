#include "synth/tts/lexicon.h"

#include "engine.h"

#define BIND_OOM(x)   { if(!(x)) { Engine::outOfMemory(); return; } }

using namespace synth;

/* The current keymap */
keymap_t Engine::m_keymap = {0};
/* The current key */
uint8_t Engine::m_key = ENGINE_KEY_GLOBAL;
/* The workbench word */
word_t* Engine::m_workbench = nullptr;
/* The workbench iterator */
worditerator_t* Engine::m_workbenchIterator = nullptr;
/* Has the workbench been dirtied ? */
bool Engine::m_workbenchDirty = false;


/* Run the engine */
void Engine::run() {
    /* Default pitch in keymap */
    for (uint8_t i = 0 ; i < KEYMAP_LENGTH ; ++i) {
        /* Default pitch for local mode is engine note default */
        Engine::m_keymap.keys[i].pitch = ENGINE_NOTE_DEFAULT;
    }
    /* Allocate the workbench word, and assert it worked */
    MBED_ASSERT(Lexicon::alloc(Engine::m_workbench, ENGINE_WORKBENCH_WORD_SIZE));
    /* Make iterator. It is never regenerated since the workbench buffer never moves */
    Engine::m_workbenchIterator = new worditerator_t(Lexicon::iterator(Engine::m_workbench));
    /* Select default note */
    Engine::select(ENGINE_NOTE_DEFAULT);
}

/* Write workbench to key */
void Engine::tidy() {
    /* If not dirty, it's all good */
    if (!Engine::m_workbenchDirty) { return; }
    /* Shrink-clone word into place (and COLLAPSE ZERO LENGTH) */
    BIND_OOM(Lexicon::shrinkCopy(Engine::m_workbench, Engine::word(), true));
    /* Undirty */
    Engine::m_workbenchDirty = false;
}

/* Select a key, given a note */
void Engine::select(uint8_t midinote) {
    /* Convert note to key ID */
    uint8_t nextKey = Engine::keyOfNote(midinote);
    uint8_t oldKey = Engine::m_key;

    /* Tidy the workbench */
    Engine::tidy();    
    /* Change the current id */
    Engine::m_key = nextKey;
    /* If old key and next key point to the same word, no need to reload */
    if (Engine::wordOf(nextKey) == Engine::wordOf(oldKey)) { return; }

    /* Reset the iterator to first frame */
    Engine::wordIterator().first();
    /* Copy the keymap data into the workbench 
       (copy cannot fail, workbench is always biggest) */
    Lexicon::copy(Engine::word(), Engine::m_workbench);
}