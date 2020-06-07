#include "store.h"

#define BIND_MEM_ERROR(e) { mem::mem_error_t ___err = e; if (___err) { return ___err; } }

I2C* mem::Store::m_bus = nullptr;
mem::dev::EE24xx1025* mem::Store::m_eeproms[] = { 0 };

void mem::Store::run() {
    /* Create the I2C bus */
    Store::m_bus = new I2C(NC, NC);
    Store::m_bus->frequency(400000);
    /* Initialize EEPROMs */
    Store::m_eeproms[0] = new dev::EE24xx1025(Store::m_bus, 0x00);
    Store::m_eeproms[1] = new dev::EE24xx1025(Store::m_bus, 0x01);
}

/* Reset memory to factory settings */
void mem::Store::reset() {
    
}



/* The basic routines for write, read */
mem::mem_error_t mem::Store::write(memaddr_t address, const memdata_t& data) {
    /* Write to memory */
    Store::m_eeproms[!!(address & 0x400)]->write(address, data);
    /* Ok */
    return FAIL_MEM_NONE;
}

/* Write a whole page to the address */
mem::mem_error_t mem::Store::writePage(memaddr_t address, memdata_t* data, size_t len) {
    while (len--) {
        write(address++, *(data++));
    }
    /* Ok */
    return FAIL_MEM_NONE;
}

/* Write some data at given addr, only if different */
mem::mem_error_t mem::Store::safeWrite(memaddr_t address, const memdata_t& data) {
    /* Read first */
    memdata_t tmp;
    BIND_MEM_ERROR(read(address, tmp));
    /* COmpare */
    if (tmp != data) {
        /* Write iff different */
        write(address, data);
    }
    /* Ok */
    return FAIL_MEM_NONE;
}
/* Write a whole page to the address */
mem::mem_error_t mem::Store::safeWritePage(memaddr_t address, memdata_t* data, size_t len) {
    /* Get it */
    while (len--) {
        /* Safewrite here */
        safeWrite(address++, *(data++));
    }
    /* Ok */
    return FAIL_MEM_NONE;
}

/* Read some data at given addr */
mem::mem_error_t mem::Store::read(memaddr_t address, memdata_t &data) {
    /* Read from memory */
    Store::m_eeproms[!!(address & 0x400)]->read(address, data);
    /* Ok */
    return FAIL_MEM_NONE;
}
/* Read some data at given addr */
mem::mem_error_t mem::Store::readPage(memaddr_t address, memdata_t* data, size_t len) {
    memdata_t tmp;
    while (len--) {
        BIND_MEM_ERROR(read(address++, tmp));
        *(data++) = tmp;
    }
    /* Ok */
    return FAIL_MEM_NONE;
}