#include <stdlib.h>
#include <stdio.h>

#include "instmasks.h"
#include "rsim.h"

#define check(x,y) ((x) & 127) == (y)

#define getfunct3(x) ((x) & (7 << 12)) >> 12
#define getfunct7(x) ((x) & (127 << 25)) >> 25

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
    } else if (check(instruction, luimask)) {
        fprintf(logfile, "%x - lui\n", instruction);
    } else if (check(instruction, auipcmask)) {
        fprintf(logfile, "%x - auipc / call\n", instruction);
    } else {
        printf("[ERROR] could not classify instruction %x\n", instruction);
        return(1);
    }
    return (0);
}
