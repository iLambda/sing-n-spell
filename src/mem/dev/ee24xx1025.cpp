#include "ee24xx1025.h"

#define EE24XX1025_CONTROLCODE    0xA0

/* New driver for a 24xx1025 */
mem::dev::EE24xx1025::EE24xx1025(I2C* bus, uint8_t a01) {
    /* Save the I2C bus */
    this->m_bus = bus;
    /* The A01 pins state */
    this->m_i2caddress = EE24XX1025_CONTROLCODE | ((a01 & 0x03) << 1);
}

/* Read a value at a given address */
bool mem::dev::EE24xx1025::read(const uint32_t& address, uint8_t& out) {
    return read(address, &out, 1);
}

/* Read a value at a given address */
bool mem::dev::EE24xx1025::read(const uint32_t& address, uint8_t* buffer, size_t n) {
    /* Compute the target i2c address */
    uint8_t i2caddress = this->m_i2caddress | (((address & 0x10000) >> 0x0D) & 0x08);
    /* Prepare */
    char values[] = { ((address >> 8) & 0xFF), (address & 0xFF) };
    /* Read */
    if (!this->m_bus->write(i2caddress, values, 2)) {
        if (!this->m_bus->read(i2caddress, (char*)buffer, n)) {
            /* Done */
            return true;
        }
    }
    /* Failure */
    return false;
}

/* Write a value to a given address */
bool mem::dev::EE24xx1025::write(const uint32_t& address, const uint8_t& value) {
    /* Compute the target i2c address */
    uint8_t i2caddress = this->m_i2caddress | (((address & 0x10000) >> 0x0D) & 0x08);
    /* Prepare */
    char values[] = { ((address >> 8) & 0xFF), (address & 0xFF), value };
    /* Read */
    if (this->m_bus->write(i2caddress, values, 3)) {
        /* Failure */
        return false;
    }
    /* Wait for ack */
    while (this->m_bus->write(i2caddress, 0, 0)) {
        // Wait for ack.
        ThisThread::sleep_for(1);
    }
    /* Failure */
    return false;
}

/* Write a value to a given address */
bool mem::dev::EE24xx1025::write(const uint32_t& address, uint8_t* buffer, size_t n) {

}