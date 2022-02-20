#include "rsim.h"
#include "instmasks.h"
#include "gprf.h"

#include "branch.h"

#define immfieldlength 13
int signextendbranchimm(int value) {
    int mask;
    mask = 1U << (immfieldlength - 1);
    value = value & ((1U << immfieldlength) - 1);
    return (value ^ mask) - mask;
}

int branch(unsigned int rs1, unsigned int rs2,
           unsigned int funct3, unsigned int brimm) {
    errno=0;

    if (brimm % 4) {
        fprintf(logfile, "[ERROR] mis-aligned branch offset from current pc (%08x)", brimm);
        return EFAULT;
    }

    unsigned int newpc = gprgetpc() + (int) signextendbranchimm(brimm);

    switch(funct3) {
        case funct3equal:
            if (gprread(rs1) == gprread(rs2)) {
                gprputpc(newpc);
            } else {
                gprnextpc();
            }
            break;
        case funct3notequal:
            if (gprread(rs1) != gprread(rs2)) {
                gprputpc(newpc);
            } else {
                gprnextpc();
            }
            break;
        case funct3lessthan:
            if ((int) gprread(rs1) < (int) gprread(rs2)) {
                gprputpc(newpc);
            } else {
                gprnextpc();
            }
            break;
        case funct3greaterequal:
            if ((int) gprread(rs1) >= (int) gprread(rs2)) {
                gprputpc(newpc);
            } else {
                gprnextpc();
            }
            break;
        case funct3lessthanu:
            if (gprread(rs1) < gprread(rs2)) {
                gprputpc(newpc);
            } else {
                gprnextpc();
            }
            break;
        case funct3greaterequalu:
            if (gprread(rs1) >= gprread(rs2)) {
                gprputpc(newpc);
            } else {
                gprnextpc();
            }
            break;
        default:
            fprintf(logfile, "[ERROR] branch funct3 (%d) not recognized", funct3);
            return EINVAL;
    }

    return errno;
}
