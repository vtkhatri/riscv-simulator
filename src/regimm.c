#include "rsim.h"
#include "instmasks.h"
#include "gprf.h"

#include "regimm.h"

int registerimmediate(unsigned int rd, unsigned int rs1,
                      unsigned int funct3, unsigned int funct7,
                      unsigned int imm) {
    switch(funct3) {
        case funct3add:
            gprwrite(rd, gprread(rs1)+imm);
            break;
        case funct3shiftleftl:
            gprwrite(rd, gprread(rs1) << imm);
            break;
        case funct3setlessthan:
            if ((int)gprread(rs1) < (int)imm) {
                gprwrite(rd, 1);
            } else {
                gprwrite(rd, 0);
            }
            break;
        case funct3setlessthanu:
            if (gprread(rs1) < imm) {
                gprwrite(rd, 1);
            } else {
                gprwrite(rd, 0);
            }
            break;
        case funct3xor:
            gprwrite(rd, gprread(rs1) ^ imm);
            break;
        case funct3shiftright:
            if (funct7 == funct7shiftrightl) {
                gprwrite(rd, (int) gprread(rs1) >> imm);
            } else if (funct7 == funct7shiftrighta) {
                gprwrite(rd, gprread(rs1) >> imm);
            } else {
                fprintf(logfile, "[ERROR] %s - funct3 for shift right has illegal funct7 field (%x)\n", __func__, funct7);
                return EINVAL;
            }
            break;
        case funct3or:
            gprwrite(rd, gprread(rs1) | imm);
            break;
        case funct3and:
            gprwrite(rd, gprread(rs1) | imm);
            break;
        default:
            fprintf(logfile, "[ERROR] %s - funct3 (%x) does not match anything\n", __func__, funct3);
            return EINVAL;
            break;
    }
    return 0;
}
