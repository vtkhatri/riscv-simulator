#include <stdlib.h>
#include <stdio.h>

#include "instmasks.h"
#include "rsim.h"

#define funct3mask 7 << 12 // 111 00000 0000000
                           //  f3   rd   opcode
#define funct7mask 127 << 25 // fe 00 00 00 00

#define check(x,y) ((x) & (y)) == (y)

unsigned int getfunct3(unsigned int instruction) {
    return ((instruction & funct3mask)>> 12);
}

unsigned int getfunct7(unsigned int instruction) {
    return ((instruction & funct7mask)>> 25);
}

int decodeandcall(unsigned int instruction) {
    if (check(instruction, registerimmediatemask)) {
        fprintf(logfile, "%x - register immediate\n", instruction);
    } else if (check(instruction, registerregistermask)) {
        fprintf(logfile, "%x - register immediate\n", instruction);
    } else if (check(instruction, jalmask)) {
        fprintf(logfile, "%x - jal\n", instruction);
    } else if (check(instruction, jalrmask)){
        fprintf(logfile, "%x - jal regsiter\n", instruction);
    } else if (check(instruction, branchmask)) {
        fprintf(logfile, "%x - branch inst\n", instruction);
    } else if (check(instruction, loadmask)) {
        fprintf(logfile, "%x - load\n", instruction);
    } else if (check(instruction, storemask)) {
        fprintf(logfile, "%x - store\n", instruction);
    }
    return (0);
}
