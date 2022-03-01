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
            if (funct7 == funct7sub) {
                gprwrite(rd, gprread(rs1)-gprread(rs2));
            } else if (funct7 == funct7m) {
                long long temp = (int) gprread(rs1) * (int) gprread(rs2);
                gprwrite(rd, temp & lower32mask);
            } else if (funct7 == funct7normal) {
                gprwrite(rd, gprread(rs1)+gprread(rs2));
            } else {
                fprintf(logfile, "[ERROR] %s - funct7 (%x) is illegal\n", __func__, funct7);
                return EINVAL;
            }
            break;
        case funct3shiftleftl:
            if (funct7 == funct7m) {
                long long temp = (int) gprread(rs1) * (int) gprread(rs2);
                gprwrite(rd, (temp & upper32mask) >> 32);
            } else if (funct7 == funct7normal) {
                gprwrite(rd, gprread(rs1) << (gprread(rs2) & 127)); // lower 5-bits decide the shamt
            } else {
                fprintf(logfile, "[ERROR] %s - funct7 (%x) is illegal\n", __func__, funct7);
                return EINVAL;
            }
            break;
        case funct3setlessthan:
            if (funct7 == funct7m) {
                long long temp = (int) gprread(rs1) * (unsigned int) gprread(rs2);
                gprwrite(rd, (temp & upper32mask) >> 32);
            } else if (funct7 == funct7normal) {
                if ((int)gprread(rs1) < (int)gprread(rs2)) {
                    gprwrite(rd, 1);
                } else {
                    gprwrite(rd, 0);
                }
            } else {
                fprintf(logfile, "[ERROR] %s - funct7 (%x) is illegal\n", __func__, funct7);
                return EINVAL;
            }

            break;
        case funct3setlessthanu:
            if (funct7 == funct7m) {
                long long temp = (unsigned int) gprread(rs1) * (unsigned int) gprread(rs2);
                gprwrite(rd, (temp & upper32mask) >> 32);
            } else if (funct7 == funct7normal) {
                if (gprread(rs1) < gprread(rs2)) {
                    gprwrite(rd, 1);
                } else {
                    gprwrite(rd, 0);
                }
            } else {
                fprintf(logfile, "[ERROR] %s - funct7 (%x) is illegal\n", __func__, funct7);
                return EINVAL;
            }

            break;
        case funct3xor:
            if (funct7 == funct7m) {
                gprwrite(rd, (int) gprread(rs1) / (int) gprread(rs2));
            } else if (funct7 == funct7normal) {
                gprwrite(rd, gprread(rs1) ^ gprread(rs2));
            } else {
                fprintf(logfile, "[ERROR] %s - funct7 (%x) is illegal\n", __func__, funct7);
                return EINVAL;
            }
            break;
        case funct3shiftright:
            if (funct7 == funct7m) {
            } else if (funct7 == funct7shiftrightl) {
                gprwrite(rd, (int) gprread(rs1) >> (gprread(rs2) & 127));
            } else if (funct7 == funct7shiftrighta) {
                gprwrite(rd, gprread(rs1) >> (gprread(rs2) & 127));
            } else {
                fprintf(logfile, "[ERROR] %s - funct7 (%x) is illegal\n", __func__, funct7);
                return EINVAL;
            }
            break;
        case funct3or:
            if (funct7 == funct7m) {
                gprwrite(rd, (int) gprread(rs1) % (int) gprread(rs2)); // TODO : copying sign of div to rem
            } else if (funct7 == funct7normal) {
                gprwrite(rd, gprread(rs1) | gprread(rs2));
            } else {
                fprintf(logfile, "[ERROR] %s - funct7 (%x) is illegal\n", __func__, funct7);
                return EINVAL;
            }

            break;
        case funct3and:
            if (funct7 == funct7m) {
                gprwrite(rd, (unsigned int) gprread(rs1) % (unsigned int) gprread(rs2));
            } else if (funct7 == funct7normal) {
                gprwrite(rd, gprread(rs1) & gprread(rs2));
            } else {
                fprintf(logfile, "[ERROR] %s - funct7 (%x) is illegal\n", __func__, funct7);
                return EINVAL;
            }

            break;
        default:
            fprintf(logfile, "[ERROR] %s - funct3 (%x) does not match anything\n", __func__, funct3);
            return EINVAL;
            break;
    }

    return errno;
}
