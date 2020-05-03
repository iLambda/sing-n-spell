#ifndef __SING_N_SPELL_MEM_STORAGE
#define __SING_N_SPELL_MEM_STORAGE

#include <mbed.h>
#include <rtos.h>

#include "mem/error.h"

#define STORAGE_NUM_SONGS       2
#define STORAGE_NUM_INSTR       2

namespace mem {

    typedef uint16_t memaddr_t;
    typedef uint8_t  memdata_t;
    
    class Storage {            

        private:
            /* Create some storage */
            Storage() {}

        public:
            /* Setup the storage */
            static void setup();

        public:
            /* Save a song */
            // static mem_error_t save(const song_t& song, uint8_t slotid = 0);
            // /* Save an instrument */
            // static mem_error_t save(const instrument_t& instr, uint8_t slotid);
            
            // /* Load a song */
            // static mem_error_t load(song_t& song, uint8_t slotid);
            // /* Load an instrument */
            // static mem_error_t load(instrument_t& instr, uint8_t slotid);      

        private:
            /* Write some data at given addr */
            static void write(memaddr_t address, memdata_t data);
            /* Write a whole page to the address */
            static void writePage(memaddr_t address, memdata_t* data, size_t len);
            /* Write a whole page to the address */
            template <size_t N>
            static void writePage(memaddr_t address, memdata_t data[N]) {
                writePage(address, data, N);
            }

            /* Write some data at given addr, only if different */
            static void safeWrite(memaddr_t address, memdata_t data);
            /* Write a whole page to the address */
            static void safeWritePage(memaddr_t address, memdata_t* data, size_t len);
            /* Write a whole page to the address */
            template <size_t N>
            static void safeWritePage(memaddr_t address, memdata_t data[N]) {
                safeWritePage(address, data, N);
            }

            /* Read some data at given addr */
            static memdata_t read(memaddr_t address);
            /* Read some data at given addr */
            static void readPage(memaddr_t address, memdata_t* data, size_t len);
            /* Read some data at given addr */
            static void read(memaddr_t address, memdata_t &data) {
                data = read(address);
            }


    };

};
#endif