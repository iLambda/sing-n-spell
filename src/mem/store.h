#ifndef __SING_N_SPELL_MEM_STORE
#define __SING_N_SPELL_MEM_STORE

#include <mbed.h>
#include <rtos.h>

#include "mem/fail.h"
#include "mem/dev/ee24xx1025.h"

namespace mem {

    typedef uint16_t memaddr_t;
    typedef uint8_t  memdata_t;
    
    class Store {            

        private:
            /* The I2C port */
            static I2C* m_bus;
            /* The two eeproms */
            static dev::EE24xx1025* m_eeproms[2];

        private:
            Store() {}

        public:
            /* Setup the storage controller */
            static void run();

        public:
            /* Reset memory to factory settings */
            void reset();   

        private:
            /* Write some data at given addr */
            static mem_error_t write(memaddr_t address, const memdata_t& data);
            /* Write a whole page to the address */
            static mem_error_t writePage(memaddr_t address, memdata_t* data, size_t len);
            /* Write a whole page to the address */
            template <size_t N>
            static mem_error_t writePage(memaddr_t address, memdata_t data[N]) {
                writePage(address, data, N);
            }

            /* Write some data at given addr, only if different */
            static mem_error_t safeWrite(memaddr_t address, const memdata_t& data);
            /* Write a whole page to the address */
            static mem_error_t safeWritePage(memaddr_t address, memdata_t* data, size_t len);
            /* Write a whole page to the address */
            template <size_t N>
            static mem_error_t safeWritePage(memaddr_t address, memdata_t data[N]) {
                safeWritePage(address, data, N);
            }

            /* Read some data at given addr */
            static mem_error_t readPage(memaddr_t address, memdata_t* data, size_t len);
            /* Read some data at given addr */
            static mem_error_t read(memaddr_t address, memdata_t &data);


    };

};
#endif