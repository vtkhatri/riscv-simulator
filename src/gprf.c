#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#include "gprf.h"

extern FILE *logfile;

static unsigned int x[32];
static unsigned int pc;

int initgprf(unsigned int programcounter, unsigned int stackaddress) {
    pc = programcounter;
    x[sp] = stackaddress;
    x[zero] = 0;

    return 0;
}

unsigned int gprread(gprfindex index) {
    if (index > 31) {
        errno = ERANGE;
        return 0;
    }
    return x[index];
}

int gprfwrite(gprfindex index, unsigned int value) {
    if (index == zero) {
        return EINVAL;
    }
    if (index > 31) {
        return ERANGE;
    }

    x[index] = value;
    return 0;
}

int printgprf() {
    fprintf(logfile, "pc - %d\n", pc);
    for (int i=zero; i<=t6; i++) {
        fprintf(logfile, "%s - %x\n", gprfnames[i], gprread(i));
        if (errno != 0) {
            fprintf(logfile, "[ERROR] apocalypitic error occured, out of range with using enum - %s", __func__);
            return EINVAL;
        }
    }
    return 0;
}
