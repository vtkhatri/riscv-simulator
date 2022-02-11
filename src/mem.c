#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#include "mem.h"

#define word     32
#define halfword 16
#define byte     8

#define wordalignment     4
#define halfwordalignment 2

#define halfwordlowmask  65535
#define halfwordhighmask 4294901760
#define byte1mask        255
#define byte2mask        65280
#define byte3mask        16711680
#define byte4mask        4278190080

static unsigned int *ram;

int initmemory(unsigned int size, char *memfilename) {
    ram = (unsigned int *) calloc (size/wordalignment, sizeof(unsigned int)); // assuming size is passed as bytes.
    if (ram == NULL) {
        printf("[ERROR] could not initialize memory of size %d", size);
        return (1);
    }

    FILE *memfile = fopen(memfilename, "r");
    if (memfile == NULL) {
        printf("[ERROR] could not open memfile");
        return (2);
    }

    char *address = (char *) malloc (5); // 64k address space, should be represented by 16-bits = 4 hex characters + 1 colon
    char *value = (char *) malloc (8); // 32-bit value, 8 hex characters.

    while (fscanf(memfile, "%s %[^\n]s", address, value) != EOF) {
        char *end = address + strlen(address) - 1;
        if (*end == ':') *end = '\0'; // stripping away the colon from address field
        else {
            printf("[ERROR] address scanned (%s) does not have a colon, misformatted mem file %s", address, memfilename);
            return (3);
        }

        unsigned int addressint = (unsigned int) strtoul (address, NULL, 16);
        if (addressint == 0) {
            if (errno == EINVAL) {
                printf("[ERROR] invalid input %s", address);
                return (4);
            }
            if (errno == ERANGE) {
                printf("[ERROR] address too big %s", address);
                return (4);
            }
        }

        unsigned int valueint = (unsigned int) strtoul (value, NULL, 16);
        if (valueint == 0) {
            if (errno == EINVAL) {
                printf("[ERROR] invalid input %s", address);
                return (4);
            }
            if (errno == ERANGE) {
                printf("[ERROR] value too big %s", value);
                return (4);
            }
        }

        ram[addressint/wordalignment] = valueint; // actually putting the value in ram
    }

    fclose(memfile);
    return (0);
}

int memwrite32u(unsigned int address, unsigned int value) {
    if (address % wordalignment) {
        printf("[ERROR] mis-aligned address, called %s with address %x", __func__, address);
        return (1);
    }
    ram[address] = value;
    return (0);
}

int memwrite32(unsigned int address, int value) {
    if (address % wordalignment) {
        printf("[ERROR] mis-aligned address, called %s with address %x", __func__, address);
        return (1);
    }
    ram[address] = (unsigned int) value;
    return (0);
}

int memwrite16u(unsigned int address, unsigned int value) {
    if (address % halfwordalignment) {
        printf("[ERROR] mis-aligned address, called %s with address %x", __func__, address);
        return (1);
    }

    if (address % wordalignment) {      // if address is word aligned, i.e. goes on lower half of the word
        if (value & halfwordhighmask) { // and there are bits in the upper half of word "value"
            printf("[ERROR] value is more than lower half of address can handle, (%s, %x, %x)", __func__, value, address);
            return(2);
        }

        ram[address] = value;

    } else {                            // if address is not word aligned, i.e. goes on upper half of the word
        if (value & halfwordlowmask) {  // and there are bits in the lower half of word "value"
            printf("[ERROR] value is more than higher half of address can handle, (%s, %x, %x)", __func__, value, address);
            return(2);
        }

        ram[address] = value << halfword;
    }
    return 0;
}

int memwrite16(unsigned int address, int value) {
    if (address % halfwordalignment) {
        printf("[ERROR] mis-aligned address, called %s with address %x", __func__, address);
        return (1);
    }

    if (address % wordalignment) {      // if address is word aligned, i.e. goes on lower half of the word
        if (value & halfwordhighmask) { // and there are bits in the upper half of word "value"
            printf("[ERROR] value is more than lower half of address can handle, (%s, %x, %x)", __func__, value, address);
            return(2);
        }

        ram[address] = (unsigned int) value;

    } else {                            // if address is not word aligned, i.e. goes on upper half of the word
        if (value & halfwordlowmask) {  // and there are bits in the lower half of word "value"
            printf("[ERROR] value is more than higher half of address can handle, (%s, %x, %x)", __func__, value, address);
            return(2);
        }

        ram[address] = (unsigned int)value << halfword;
    }
    return 0;
}
