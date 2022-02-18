#include "mem.h"
#include "gprf.h"
#include "instmasks.h"

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

int signextendmemimm(int value, int fieldlength) {
    int mask;
    mask = 1U << (fieldlength - 1);
    value = value & ((1U << fieldlength) - 1);
    return (value ^ mask) - mask;
}

int load(unsigned int rd, unsigned int rs1, unsigned int funct3, unsigned int ldimm) {
    errno = 0;

    unsigned int effectiveaddress = gprread(rs1) + signextendmemimm(ldimm, 12);
    int readvalue;

    switch(funct3) {
        case funct3byte:
            readvalue = memread8u(effectiveaddress);
            gprwrite(rd, signextendmemimm(readvalue, 8));
            break;
        case funct3halfword:
            readvalue = memread16u(effectiveaddress);
            gprwrite(rd, signextendmemimm(readvalue, 16));
            break;
        case funct3word:
            readvalue = memread32u(effectiveaddress);
            gprwrite(rd, readvalue);
            break;
        case funct3byteu:
            readvalue = memread8u(effectiveaddress);
            gprwrite(rd, readvalue);
            break;
        case funct3halfwordu:
            readvalue = memread16u(effectiveaddress);
            gprwrite(rd, readvalue);
            break;
        default:
            fprintf(logfile, "[ERROR] load given invalid funct3 (%d)", funct3);
            return EINVAL;
            break;
    }

    return errno;
}

int store(unsigned int rs1, unsigned int rs2, unsigned int funct3, unsigned int stimm) {
    errno = 0;

    unsigned int effectiveaddress = gprread(rs1) + signextendmemimm(stimm, 12);
    unsigned int writevalue = gprread(rs2);

    switch(funct3) {
        case funct3byte:
            errno = memwrite8u(effectiveaddress, signextendmemimm(writevalue,8));
            break;
        case funct3halfword:
            errno = memwrite16u(effectiveaddress, signextendmemimm(writevalue,16));
            break;
        case funct3word:
            errno = memwrite32u(effectiveaddress, writevalue);
            break;
        case funct3byteu:
            errno = memwrite8u(effectiveaddress, writevalue);
            break;
        case funct3halfwordu:
            errno = memwrite16u(effectiveaddress, writevalue);
            break;
        default:
            fprintf(logfile, "[ERROR] store given invalid funct3 (%d)", funct3);
            return EINVAL;
            break;
    }

    return errno;
}
