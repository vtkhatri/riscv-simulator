#include "rsim.h"
#include "instmasks.h"
#include "gprf.h"

#include "regimm.h"

int registerregister(unsigned int rd, unsigned int rs1,
                     unsigned int rs2, unsigned int funct3,
                     unsigned int funct7) {
    errno=0;

    switch(funct3) {
        case funct3add:
            gprwrite(rd, gprread(rs1)+gprread(rs2));
            break;
        case funct3shiftleftl:
            gprwrite(rd, gprread(rs1) << (gprread(rs2) & 127)); // lower 5-bits decide the shamt
            break;
        case funct3setlessthan:
            if ((int)gprread(rs1) < (int)gprread(rs2)) {
                gprwrite(rd, 1);
            } else {
                gprwrite(rd, 0);
            }
            break;
        case funct3setlessthanu:
            if (gprread(rs1) < gprread(rs2)) {
                gprwrite(rd, 1);
            } else {
                gprwrite(rd, 0);
            }
            break;
        case funct3xor:
            gprwrite(rd, gprread(rs1) ^ gprread(rs2));
            break;
        case funct3shiftright:
            if (funct7 == funct7shiftrightl) {
                gprwrite(rd, (int) gprread(rs1) >> (gprread(rs2) & 127));
            } else if (funct7 == funct7shiftrighta) {
                gprwrite(rd, gprread(rs1) >> (gprread(rs2) & 127));
            } else {
                fprintf(logfile, "[ERROR] %s - funct3 for shift right has illegal funct7 field (%x)\n", __func__, funct7);
                return EINVAL;
            }
            break;
        case funct3or:
            gprwrite(rd, gprread(rs1) | gprread(rs2));
            break;
        case funct3and:
            gprwrite(rd, gprread(rs1) & gprread(rs2));
            break;
        default:
            fprintf(logfile, "[ERROR] %s - funct3 (%x) does not match anything\n", __func__, funct3);
            return EINVAL;
            break;
    }

    return errno;
}
