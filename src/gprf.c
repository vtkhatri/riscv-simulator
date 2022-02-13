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
    return(0);
}

int gprread(gprfindex index, unsigned int *value) {
    *value = x[index];
    return (0);
}

int gprfwrite(gprfindex index, unsigned int value) {
    if (index == zero) {
        printf("[ERROR] writing to zero register\n");
        return (1);
    }

    x[index] = value;
    return (0);
}

int printgprf() {
    fprintf(logfile, "pc - %d\n", pc);
    for (int i=zero; i<=t6; i++) {
        int gprval;
        gprread(i, &gprval);
        fprintf(logfile, "%s - %x\n", gprfnames[i], gprval);
    }
    return (0);
}
