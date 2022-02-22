#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

typedef enum {
    gdb,
    tracemem,
    tracegprf,
    traceall
} debugtypet;

int handledebugtype(debugtypet);
