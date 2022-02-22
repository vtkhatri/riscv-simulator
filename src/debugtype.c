#include "debugtype.h"

#include "rsim.h"
#include "gprf.h"
#include "mem.h"

int loopcheck() {
    char cmd;
    char *address = (char*) malloc (9*sizeof(char));

    int readchars;

ttarget:
    fprintf(stdout, "> ");
    readchars = scanf("%c %s", &cmd, address);

    if (readchars == 0) {
        fprintf(stdout, "usage - [n|c|p] <location to print> <address for mem>");
    }

    if (cmd == 'n') {
        return 0; // go to next loop
    } else if (cmd == 'c') {
        return EPERM; // eperm means don't need to check again.
    } else if (cmd == 'p') {
        if (readchars == 1) {
            printgprf();
        } else {
            if (readchars == 2) {
                unsigned int addressnum = (unsigned int) strtoul (address, NULL, 16);
                fprintf(stdout, "ram[%08x] = %08x", addressnum, memread32u(addressnum));
            }
            goto ttarget;
        }
    }

    return 0;
}

int handledebugtype(debugtypet debugtype) {
    switch(debugtype) {
    case gdb:
        logfile = stdout;
        return loopcheck();
        break;
    case tracemem:
        memlogfile = logfile;
        break;
    case tracegprf:
        gprflogfile = logfile;
        break;
    case traceall:
        memlogfile = logfile;
        gprflogfile = logfile;
        break;
    default:
        fprintf(stderr, "[ERROR] debug type? %d", debugtype);
        return EINVAL;
    }
}
