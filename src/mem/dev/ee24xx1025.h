#ifndef __SING_N_SPELL_MEM_DEVICE_24XX1025
#define __SING_N_SPELL_MEM_DEVICE_24XX1025

#include <mbed.h>
#include <rtos.h>

namespace mem {
    
    namespace dev {

        class EE24xx1025 {

            private:
                /* The I2C bus */
                I2C* m_bus;
                /* The state of the A0-A1 pins */
                uint8_t m_i2caddress;

            public:
                /* New driver for a 24xx1025 */
                EE24xx1025(I2C* bus, uint8_t a01);

            public:
                /* Read a value at a given address */
                bool read(const uint32_t& address, uint8_t* buffer, size_t n);
                /* Read a value at a given address */
                bool read(const uint32_t& address, uint8_t& out);
                /* Write a value to a given address */
                bool write(const uint32_t& address, const uint8_t& value);
                /* Write a value to a given address */
                bool write(const uint32_t& address, uint8_t* buffer, size_t n);

        };

    }

}

#endif
