#include "rsim.h"
#include "gprf.h"
#include "mem.h"

int read() {
    int charstoread = gprread(a2);
    int addresstowriteto = gprread(a1);
    char *instring = (char *) malloc (sizeof(char)*charstoread);
    fprintf(stdout, "enter %0d characters\n> ", charstoread);
    char *dummy = fgets(instring, sizeof(instring)+1, stdin);
    unsigned int i;
    for (i=0; i<charstoread; i++) {
        memwrite8u(addresstowriteto+i, instring[i]);
    }

    gprwrite(a0, i);

    return 0;
}

int write() {
    FILE *tempfile = (gprread(a1) == 1) ? stdout : stderr;
    int charstoread = gprread(a2);
    int addresstoreadfrom = gprread(a1);
    fprintf(tempfile, "printing %0d characters from memory location %08x (%0d)\n> ", charstoread, addresstoreadfrom, addresstoreadfrom);

    unsigned int i;
    for (i=0; i<charstoread; i++) {
        fprintf(tempfile, "%c", memread8u(addresstoreadfrom+i));
    }
    fprintf(tempfile, "\n");

    gprwrite(a0, i);

    return 0;
}

int ecall() {
    switch (gprread(a7)) {
        case 63:
            if (gprread(a0) != 0) {
                fprintf(logfile, "[ERROR] ecall trying to read from something other than stdin, returning EINVAL\n");
                return EINVAL;
            }
            errno = read();
            break;
        case 64:
            if (gprread(a1) != 1 && gprread(a2) != 2) {
                fprintf(logfile, "[ERROR] ecall trying to write to something other than stdout or stderr, returning EINVAL\n");
                return EINVAL;
            }
            errno = write();
            break;
        case 94:
            fprintf(logfile, "[ECALL] exiting with code - %0d\n", gprread(a0));
            exit(gprread(a0));
            break;
        default:
            fprintf(logfile, "[ERROR] ecall called with invalid value in a7(%0d)\n", gprread(a7));
            errno = EINVAL;
    }

    return errno;
}
