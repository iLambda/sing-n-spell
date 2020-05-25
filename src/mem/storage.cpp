#include "storage.h"

void mem::Storage::run() {

}

/* The basic routines for write, read */
void mem::Storage::write(memaddr_t address, memdata_t data) {
    /* TODO */
}

/* Write a whole page to the address */
void mem::Storage::writePage(memaddr_t address, memdata_t* data, size_t len) {
    while (len--) {
        write(address++, *(data++));
    }
}

/* Write some data at given addr, only if different */
void mem::Storage::safeWrite(memaddr_t address, memdata_t data) {
    /* Read first */
    if (read(address) != data) {
        /* Write iff different */
        write(address, data);
    }
}
/* Write a whole page to the address */
void mem::Storage::safeWritePage(memaddr_t address, memdata_t* data, size_t len) {
    /* Get it */
    while (len--) {
        /* Safewrite here */
        safeWrite(address++, *(data++));
    }
}

/* Read some data at given addr */
mem::memdata_t mem::Storage::read(memaddr_t address) {
    /* TODO */
    return 0;
}
/* Read some data at given addr */
void mem::Storage::readPage(memaddr_t address, memdata_t* data, size_t len) {
    while (len--) {
        *(data++) = read(address++);
    }
}