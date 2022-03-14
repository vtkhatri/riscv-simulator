#include "debugtype.h"

#include "rsim.h"
#include "gprf.h"
#include "mem.h"

static char prevcmd;

int loopcheck() {
    char cmd;
    char *address;
    char *input = (char *) malloc (sizeof(cmd)+sizeof(address)+sizeof(char));
    char *dummy;


ttarget:
    errno = 0;
    fprintf(stdout, "> ");
    dummy = fgets(input, sizeof(input)+1, stdin);

    cmd = strtok(input, " ")[0];
    if (cmd == '\n') {
        cmd = prevcmd;
    }
    prevcmd = cmd;
    address = strtok(NULL, " \n");

    if (cmd == 'n') {
        return 0; // go to next loop
    } else if (cmd == 'c') {
        return EPERM; // eperm means don't need to check anything again, just complete execution.
    } else if (cmd == 'p') {
        if (address == NULL || *address == 'g') {
            printgprf(stdout);
            goto ttarget;
        } else {
            unsigned int addressnum = strtoul(address, NULL, 16);
            fprintf(stdout, "ram[%08x] -> %08x\n", addressnum, memread32u(addressnum));
            goto ttarget;
        }
    } else {
        fprintf(stdout, "invalid input %c %s.\n", cmd, address);
        goto ttarget;
    }

    return 0;
}
