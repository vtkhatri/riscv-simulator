#include "gprf.h"

extern FILE *logfile;

static unsigned int x[32];
static unsigned int pc;

const char *gprfnames[] = {
    "zero",
    "ra",
    "sp",
    "gp",
    "tp",
    "t0",
    "t1",
    "t2",
    "s0fp",
    "s1",
    "a0",
    "a1",
    "a2",
    "a3",
    "a4",
    "a5",
    "a6",
    "a7",
    "s2",
    "s3",
    "s4",
    "s6",
    "s7",
    "s8",
    "s9",
    "s10",
    "s11",
    "t3",
    "t4",
    "t5",
    "t6"
};

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

int gprwrite(gprfindex index, unsigned int value) {
    if (index == zero) {
        return EINVAL;
    }
    if (index > 31) {
        return ERANGE;
    }

    x[index] = value;
    return 0;
}

int gprfprint() {
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

unsigned int gprgetpc() {
    return pc;
}

int gprputpc(unsigned int progcount) {
    pc = progcount;
    return 0;
}
