#include "mem.h"

static unsigned int *ram;

int initmemory(unsigned int size, char *memfilename) {

    errno = 0;
    ram = (unsigned int *) calloc (ramaddress(size), sizeof(unsigned int));
    if (ram == NULL) {
        return ENOMEM;
    }

    FILE *memfile = fopen(memfilename, "r");
    if (memfile == NULL) {
        return ENOENT;
    }

    char *address = (char *) malloc (5); // 64k address space, should be represented by 16-bits = 4 hex characters + 1 colon
    char *value = (char *) malloc (8); // 32-bit value, 8 hex characters.

    while (fscanf(memfile, "%s %[^\n]s", address, value) != EOF) {
        char *end = address + strlen(address) - 1;
        if (*end == ':') *end = '\0'; // stripping away the colon from address field
        else {
            return EINVAL;
        }

        unsigned int addressint = (unsigned int) strtoul (address, NULL, 16);
        if (addressint == 0 && errno != 0) {
            printf("[ERROR] %s - can't convert address (%s) to int\n", __func__, address);
            return errno;
        }

        unsigned int valueint = (unsigned int) strtoul (value, NULL, 16);
        if (valueint == 0 && errno != 0) {
            printf("[ERROR] %s - can't convert value (%s) to int\n", __func__, value);
            return errno;
        }

        ram[ramaddress(addressint)] = valueint; // actually putting the value in ram
    }

    fclose(memfile);
    return 0;
}

int memwrite32u(unsigned int address, unsigned int value) {
    if (address % wordalignment) {
        return EINVAL;
    }

    // simple overwrite of element
    ram[ramaddress(address)] = value;
    return 0;
}

int memwrite16u(unsigned int address, unsigned int value) {
    if (address % halfwordalignment) {
        printf("[ERROR] mis-aligned address, called %s with address %x", __func__, address);
        return EINVAL;
    }

    if (value != halfwordlowmask & value) {
        return ERANGE;
    }

    // clearing 16-bits
    ram[ramaddress(address)] = ram[ramaddress(address)] & (halfwordlowmask << halfword*(address % wordalignment));
    // setting 16-bits according to value
    ram[ramaddress(address)] = ram[ramaddress(address)] | (value << halfword*(address % wordalignment));

    return 0;
}

int memwrite8u(unsigned int address, unsigned int value) {
    if (value != value & byte1mask) {
        return ERANGE;
    }

    // clearing 8-bits
    ram[ramaddress(address)] = ram[ramaddress(address)] & (byte1maskinv << byte*(address % wordalignment));
    // setting 8-bits according to value
    ram[ramaddress(address)] = ram[ramaddress(address)] | (value << byte*(address % wordalignment));

    return 0;
}

unsigned int memread32u(unsigned int address) {
    if (address % wordalignment) {
        errno = EINVAL;
        return 0;
    }

    // simple word dump
    return ram[ramaddress(address)];
}

unsigned int memread16u(unsigned int address) {
    if (address % halfwordalignment) {
        errno = EINVAL;
        return 0;
    }

    if (address % wordalignment) {
        return (ram[ramaddress(address)] & halfwordhighmask) >> halfword;
    } else {
        return ram[ramaddress(address)] & halfwordlowmask;
    }
}

unsigned int memread8u(unsigned int address) {
    return (ram[ramaddress(address)] & (byte1mask << (byte * (address % wordalignment)))) // get 8-bits
              >> (byte * (address % wordalignment)); // shift to correct it's value
}
