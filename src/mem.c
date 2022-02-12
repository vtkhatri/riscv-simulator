#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#include "mem.h"

#define word     32
#define halfword 16
#define byte     8

#define wordalignment     sizeof(int)
#define halfwordalignment sizeof(int)/2
#define bytealignment     sizeof(char)

#define halfwordlowmask  65535      // 0x0000ffff
#define halfwordhighmask 4294901760 // 0xffff0000

#define byte1mask        255        // 0x000000ff
#define byte1maskinv     4294967040 // 0xffffff00
#define byte2mask        65280      // 0x0000ff00
#define byte2maskinv     4294902015 // 0xffff00ff
#define byte3mask        16711680   // 0x00ff0000
#define byte3maskinv     4278255615 // 0xff00ffff
#define byte4mask        4278190080 // 0xff000000
#define byte4maskinv     16777215   // 0x00ffffff

#define ramaddress(x) (x)/wordalignment

static unsigned int *ram;
extern int decodeandcall(unsigned int instruction);

int initmemory(unsigned int size, char *memfilename) {
    ram = (unsigned int *) calloc (ramaddress(size), sizeof(unsigned int));
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

        ram[ramaddress(addressint)] = valueint; // actually putting the value in ram
    }

    // temporary testing
    int retval;
    for (int i=0; i<ramaddress(size) && retval == 0; i=i+4) {
        if (ram[ramaddress(i)]) {
            retval = decodeandcall(ram[ramaddress(i)]);
        }
    }

    fclose(memfile);
    return (0);
}

int memwrite32u(unsigned int address, unsigned int value) {
    if (address % wordalignment) {
        printf("[ERROR] mis-aligned address, called %s with address %x", __func__, address);
        return (1);
    }

    // simple overwrite of element
    ram[ramaddress(address)] = value;
    return (0);
}

int memwrite16u(unsigned int address, unsigned int value) {
    if (address % halfwordalignment) {
        printf("[ERROR] mis-aligned address, called %s with address %x", __func__, address);
        return (1);
    }

    if (value != halfwordlowmask & value) {
        printf("[ERROR] value is more than 2 bytes, (%s, %x)", __func__, value);
        return(2);
    }

    // clearing 16-bits
    ram[ramaddress(address)] = ram[ramaddress(address)] & (halfwordlowmask << halfword*(address % wordalignment));
    // setting 16-bits according to value
    ram[ramaddress(address)] = ram[ramaddress(address)] | (value << halfword*(address % wordalignment));

    return 0;
}

int memwrite8u(unsigned int address, unsigned int value) {
    if (value != value & byte1mask) {
        printf("[ERROR] value cannot be stored in 1 byte, (%s, %x)", __func__, value);
        return(2);
    }

    // clearing 8-bits
    ram[ramaddress(address)] = ram[ramaddress(address)] & (byte1maskinv << byte*(address % wordalignment));
    // setting 8-bits according to value
    ram[ramaddress(address)] = ram[ramaddress(address)] | (value << byte*(address % wordalignment));

    return (0);
}

int memread32u(unsigned int address, unsigned int *value) {
    if (address % wordalignment) {
        printf("[ERROR] misaligned word access by %s at address %x", __func__, address);
        return (1);
    }

    // simple word dump
    *value = ram[ramaddress(address)];
    return (0);
}

int memread16u(unsigned int address, unsigned int *value) {
    if (address % halfwordalignment) {
        printf("[ERROR] misaligned half-word access by %s at address %x", __func__, address);
        return (1);
    }

    if (address % wordalignment) {
        *value = (ram[ramaddress(address)] & halfwordhighmask) >> halfword;
    } else {
        *value = ram[ramaddress(address)] & halfwordlowmask;
    }
    return (1);
}

int memread8u(unsigned int address, unsigned int *value) {
    *value = (ram[ramaddress(address)] & (byte1mask << (byte * (address % wordalignment)))) // get 8-bits
              >> (byte * (address % wordalignment)); // shift to correct it's value
    return (1);
}
