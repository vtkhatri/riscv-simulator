#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#include "instmasks.h"

#define check(x,y) ((x) & 127) == (y)

#define getrd(x) ((x) & (31 << 7)) >> 7
#define getrs1(x) ((x) & (31 << 15)) >> 15
#define getrs2(x) ((x) & (31 << 20)) >> 20

#define getfunct3(x) ((x) & (7 << 12)) >> 12
#define getfunct7(x) ((x) & (127 << 25)) >> 25

extern FILE *logfile;

extern int gprfputpc(unsigned int);
extern unsigned int gprfgetpc(); 


