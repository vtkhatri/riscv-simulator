#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

typedef enum {
    zero,
    ra,
    sp,
    gp,
    tp,
    t0,
    t1,
    t2,
    s0fp,
    s1,
    a0,
    a1,
    a2,
    a3,
    a4,
    a5,
    a6,
    a7,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    t3,
    t4,
    t5,
    t6
} gprfindex;

unsigned int gprread(gprfindex);
int gprwrite(gprfindex, unsigned int);
int printgprf();

int initgprf(unsigned int pc, unsigned int sp);
unsigned int gprgetpc();
int gprputpc(unsigned int pc);
