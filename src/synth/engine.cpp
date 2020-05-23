#include "synth/tts/lexicon.h"

#include "engine.h"

#define BIND_OOM(x)   { if(!(x)) { Engine::outOfMemory(); return; } }

using namespace synth;

/* The current keymap */
keymap_t Engine::m_keymap = {0};
/* The current key */
uint8_t Engine::m_key = ENGINE_NOTE_DEFAULT;
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
    /* Initialize the lexicon */
    synth::Lexicon::setup();
    /* Allocate the workbench word, and assert it worked */
    bool success = Lexicon::alloc(Engine::m_workbench, ENGINE_WORKBENCH_WORD_SIZE);
    MBED_ASSERT(success);
    /* Make iterator. It is never regenerated since the workbench buffer never moves */
    Engine::m_workbenchIterator = new worditerator_t(Lexicon::iterator(Engine::m_workbench));
    
    /* Select default note */
    Engine::select(ENGINE_NOTE_DEFAULT);

    /* Subscribe to midi event */
    io::Controller::midiReceive() += callback(&Engine::midiReceived);
}

/* On midi received */
void Engine::midiReceived(const io::midimsg_t& midi) {
    /* Check midi msg type */
    /* TODO : SEPARATE THREAD WITH EVENTQUEUE
       SO WE DON'T MISS ANY MIDI MESSAGES */
    switch (io::midi_message_type(midi)) {
        /* CLassic messages */
        case io::MIDI_TYPE_NOTEON:
            if (Engine::editMode()) {
                Engine::select(midi.key);
            }
            return;

        case io::MIDI_TYPE_NOTEOFF:
        case io::MIDI_TYPE_START:
        case io::MIDI_TYPE_STOP: {
            return;
        }
        /* Controller change */
        case io::MIDI_TYPE_CC: {
            /* If we got a cc, check its value */
            switch (midi.controller) {
                /* All notes / sound off */
                case io::MIDI_CC_ALLSOUNDOFF:
                case io::MIDI_CC_NOTEOFF_ALL: {
                    return;
                }
                /* Other CC msgs */
                default: {
                    if (io::midi_is_special_cc(midi.controller)) { return; }
                    return;
                }
            }
        }
        /* Other messages : ignore */
        default:
            return;
    }
}

/* Write workbench to key */
void Engine::tidy() {
    /* Save the last selected idx */
    Engine::lastPosition() = Engine::workbenchIterator().position();
    /* Shrink-clone word into place (and COLLAPSE ZERO LENGTH) */
    BIND_OOM(Lexicon::shrinkCopy(Engine::m_workbench, Engine::word(), true));
}


/* Fetches current word in memory and write it in the wordbench */
void Engine::fetch() {
    /* Copy the keymap data into the workbench 
       (copy cannot fail, workbench is always biggest) */
    Lexicon::copy(Engine::word(), Engine::m_workbench);
    /* Reset the iterator to last saved position */
    Engine::workbenchIterator().select(Engine::lastPosition());
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
    /* Fetch the contents of the memory into the wordbench */
    Engine::fetch();
}