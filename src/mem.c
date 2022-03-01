#include "mem.h"
#include "rsim.h"
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

    unsigned int before = ram[ramaddress(address)];
    ram[ramaddress(address)] = value;
    fprintf(memlogfile, "ram[%08x]  = %08x(32) ; %08x -> %08x \n", address, value, before, ram[ramaddress(address)]);
    return 0;
}

int memwrite16u(unsigned int address, unsigned int value) {
    if (address % halfwordalignment) {
        printf("[ERROR] mis-aligned address, called %s with address %x\n", __func__, address);
        return EINVAL;
    }

    if (value > halfwordlowmask) {
        printf("[ERROR] value passed to %s is more than 16-bits %08x (%08x)\n", __func__, value, (halfwordlowmask & value));
        return ERANGE;
    }

    unsigned int before = ram[ramaddress(address)];
    // clearing 16-bits
    ram[ramaddress(address)] = ram[ramaddress(address)] & ~(halfwordlowmask << byte*(address % wordalignment));
    // setting 16-bits according to value
    ram[ramaddress(address)] = ram[ramaddress(address)] | (value << byte*(address % wordalignment));

    fprintf(memlogfile, "ram[%08x]  = %08x(16) ; %08x -> %08x \n", address, value, before, ram[ramaddress(address)]);

    return 0;
}

int memwrite8u(unsigned int address, unsigned int value) {
    if (value > bytemask) {
        printf("[ERROR] value passed to %s is more than 8-bits %08x (%08x)\n", __func__, value, value);
        return ERANGE;
    }


    unsigned int before = ram[ramaddress(address)];
    // clearing 8-bits
    ram[ramaddress(address)] = ram[ramaddress(address)] & ~(bytemask << byte*(address % wordalignment));
    // setting 8-bits according to value
    ram[ramaddress(address)] = ram[ramaddress(address)] | (value << byte*(address % wordalignment));

    fprintf(memlogfile, "ram[%08x]  = %08x(08) ; %08x -> %08x \n", address, value, before, ram[ramaddress(address)]);

    return 0;
}

unsigned int memread32u(unsigned int address) {
    if (address % wordalignment) {
        errno = EINVAL;
        return 0;
    }

    fprintf(memlogfile, "ram[%08x] -> %08x\n", address, ram[ramaddress(address)]);

    // simple word dump
    return ram[ramaddress(address)];
}

unsigned int memread16u(unsigned int address) {
    if (address % halfwordalignment) {
        errno = EINVAL;
        return 0;
    }

    unsigned int readval;
    if (address % wordalignment) {
        readval = (ram[ramaddress(address)] & halfwordhighmask) >> halfword;
    } else {
        readval = ram[ramaddress(address)] & halfwordlowmask;
    }

    fprintf(memlogfile, "ram[%08x] -> %08x(16) ; %08x\n", address, readval, ram[ramaddress(address)]);

    return readval;
}

unsigned int memread8u(unsigned int address) {

    unsigned int readval = (ram[ramaddress(address)] & (bytemask << (byte * (address % wordalignment)))) // get 8-bits
                            >> (byte * (address % wordalignment)); // shift to correct it's value

    fprintf(memlogfile, "ram[%08x] -> %08x(8) ; %08x\n", address, readval, ram[ramaddress(address)]);

    return readval;
}

int load(unsigned int rd, unsigned int rs1, unsigned int funct3, unsigned int ldimm) {
    errno = 0;

    unsigned int effectiveaddress = gprread(rs1) + signextend(ldimm, 12);
    int readvalue;

    switch(funct3) {
        case funct3byte:
            readvalue = memread8u(effectiveaddress);
            gprwrite(rd, signextend(readvalue, 8));
            break;
        case funct3halfword:
            readvalue = memread16u(effectiveaddress);
            gprwrite(rd, signextend(readvalue, 16));
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
            fprintf(memlogfile, "[ERROR] load given invalid funct3 (%d)\n", funct3);
            return EINVAL;
            break;
    }

    return errno;
}

int store(unsigned int rs1, unsigned int rs2, unsigned int funct3, unsigned int stimm) {
    errno = 0;

    unsigned int effectiveaddress = gprread(rs1) + signextend(stimm, 12);
    unsigned int writevalue = gprread(rs2);

    switch(funct3) {
        case funct3byte:
            errno = memwrite8u(effectiveaddress, signextend(writevalue,8));
            break;
        case funct3halfword:
            errno = memwrite16u(effectiveaddress, signextend(writevalue,16));
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
            fprintf(memlogfile, "[ERROR] store given invalid funct3 (%d)\n", funct3);
            return EINVAL;
            break;
    }

    return errno;
}
