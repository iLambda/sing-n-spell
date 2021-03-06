#ifndef __SING_N_SPELL_SYNTH_ENGINE
#define __SING_N_SPELL_SYNTH_ENGINE

#include <mbed.h>
#include <rtos.h>

#include "io/controller.h"
#include "io/midi.h"
#include "synth/keymap.h"
#include "utils/data/dllist.h"
#include "utils/data/preserved.h"
#include "utils/debugging.h"

#define ENGINE_NOTE_DEFAULT             48      /* Default note is C3 */
#define ENGINE_WORKBENCH_WORD_SIZE      128

namespace synth {
    
    class Engine {

        private:
            /* The current keymap */
            static keymap_t m_keymap;
            /* The current key */
            static uint8_t m_key;
            
            /* The workbench word */
            static word_t* m_workbench;
            /* The workbench iterator */
            static worditerator_t* m_workbenchIterator;
            /* Has the workbench been dirtied ? */
            static bool m_workbenchDirty;

            /* Is the controller playing ? */
            static bool m_controllerPlaying;
            /* The state of the edit mode button */
            static utils::preserved_t<bool> m_editMode;

            /* The workbench mutex */
            static Mutex m_workbenchMutex;
            
        private:
            /* Create some storage */
            Engine() { }

        public:
            /* Run the engine */
            static void run();
            /* Tidy workbench (write the current workbench to the keymap) */
            static void tidy();
            /* Fetches current word in memory and write it in the wordbench */
            static void fetch();
            /* Select a key, given a note */
            static void select(uint8_t midinote);
            /* Converts a midi note to a frequency */
            __STATIC_FORCEINLINE float MIDItoFrequency(uint8_t note) {
                return io::midi_note_to_freq(note);
            }

        private:
            /* Tidy workbench then fetch */
            static void tidyFetch(uint8_t newid);

        public:
            /* Returns the keymap */
            __STATIC_FORCEINLINE const keymap_t& keymap() {
                return Engine::m_keymap;
            }
            /* Returns the current note */
            __STATIC_FORCEINLINE uint8_t note() {
                return Engine::noteOfKey(Engine::m_key);
            }
            /* Returns the pitch of the current note */
            __STATIC_FORCEINLINE uint8_t pitch() {
                return Engine::pitchOf(Engine::m_key);
            }
            /* Returns the frequency of the current note */
            __STATIC_FORCEINLINE float frequency() {
                return Engine::MIDItoFrequency(pitch());
            }

            /* Returns the current key */
            __STATIC_FORCEINLINE keyentry_t& key() {
                return Engine::m_keymap.keys[Engine::m_key];
            }
            /* Returns the word of the current key */
            __STATIC_FORCEINLINE word_t*& word() {
                /* Check if current word is in global or local mode */
                return wordOf(Engine::m_key);
            }
            /* Returns an iterator over the current word */
            __STATIC_FORCEINLINE worditerator_t& workbenchIterator() {
                /* Returns the word iterator */
                return *Engine::m_workbenchIterator;
            }
            /* Is workbench dirty ? */
            __STATIC_FORCEINLINE bool dirty() {
                return Engine::m_workbenchDirty;
            }
            /* Checks if we're in edit mode */
            __STATIC_FORCEINLINE bool editMode() {
                /* We're in edit mode iff the toggle is in edit mode, 
                   and the controller is not playing */
                return (io::Controller::input().edit && !Engine::m_controllerPlaying);
            }

        private:
            /* On midi received */
            static void midiReceived(const io::midimsg_t& midi);
            /* On input received */
            static void inputReceived(const io::inputstate_t& input);
            
        private:
            __STATIC_FORCEINLINE uint8_t& lastPosition() { return lastPositionOf(Engine::m_key); }
            /* Word at */
            __STATIC_FORCEINLINE uint8_t& lastPositionOf(uint8_t idx) {
                /* Check if current word is in global or local mode */
                return Engine::m_keymap.keys[idx].mode == keymode_t::KEY_MODE_LOCAL
                    ? Engine::m_keymap.keys[idx].lastIdx
                    : Engine::m_keymap.lastGlobalIdx;
            }
            /* Word at */
            __STATIC_FORCEINLINE word_t*& wordOf(uint8_t idx) {
                /* Check if current word is in global or local mode */
                return Engine::m_keymap.keys[idx].mode == keymode_t::KEY_MODE_LOCAL
                    ? Engine::m_keymap.keys[idx].word
                    : Engine::m_keymap.global;
            }
            /* Pitch at */
            __STATIC_FORCEINLINE uint8_t pitchOf(uint8_t idx) {
                /* Check if current word is in global or local mode */
                return Engine::m_keymap.keys[idx].mode == keymode_t::KEY_MODE_LOCAL
                    ? Engine::m_keymap.keys[idx].pitch
                    : noteOfKey(idx);
            }
            /* Note to Key ID */
            __STATIC_FORCEINLINE uint8_t keyOfNote(uint8_t midinote) {
                return 
                    midinote <= KEYMAP_NOTE_FIRST ? 0 :
                    midinote  > KEYMAP_NOTE_LAST  ? KEYMAP_LENGTH - 1
                        : (midinote - KEYMAP_NOTE_FIRST);
            }
            /* Note to Key ID */
            __STATIC_FORCEINLINE uint8_t noteOfKey(uint8_t keyidx) {
                return keyidx + KEYMAP_NOTE_FIRST;
            }
            /* Out of memory */
            __STATIC_FORCEINLINE void outOfMemory() {
                /* Notify */
                /* TODO : change */
                dbg::printf("Out of memory !!!\n");
            }


    };

};
#endif