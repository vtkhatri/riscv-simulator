#include "decode.h"

#include "instmasks.h"
#include "regimm.h"
// #include "regreg.h"
#include "jump.h"
// #include "branch.h"
#include "mem.h"
#include "gprf.h"

int decodeandcall(unsigned int instruction) {
    int retval = 0;
    errno = 0;

    if (check(instruction, registerimmediatemask)) {
        unsigned int
            rd = getrd(instruction),
            rs1 = getrs1(instruction),
            funct3 = getfunct3(instruction),
            funct7 = getfunct7(instruction),
            imm = getregimmimm(instruction);

        fprintf(logfile, "[REGIMM] %08x : rd %d, rs1 %d, funct3 %d, funct7 %d, imm %d\n", instruction, rd, rs1, funct3, funct7, imm);
        retval = registerimmediate(rd, rs1, funct3, funct7, imm);

        gprnextpc();

    } else if (check(instruction, registerregistermask)) {
        unsigned int
            rd = getrd(instruction),
            rs1 = getrs1(instruction),
            rs2 = getrs2(instruction),
            funct3 = getfunct3(instruction),
            funct7 = getfunct7(instruction);

        fprintf(logfile, "[REGREG] %08x : rd %d, rs1 %d, rs2 %d, funct3 %d, funct7 %d\n", instruction, rd, rs1, rs2, funct3, funct7);
        // retval = registerregister(rd, rs1, rs2, funct3, funct7);

        gprnextpc();

    } else if (check(instruction, jalmask)) {
        unsigned int
            rd = getrd(instruction),
            imm = getjalimm(instruction);

        fprintf(logfile, "[JAL] %08x : rd %d, imm %d\n", instruction, rd, imm);
        retval = jumpandlink(rd, imm);

    } else if (check(instruction, jalrmask)){
        unsigned int
            rs1 = getrs1(instruction),
            rd = getrd(instruction),
            imm = getjalrimm(instruction);

        fprintf(logfile, "[JALR] %08x : rd %d, rs1 %d, imm %d", instruction, rd, rs1, imm);
        retval = jumpandlinkregister(rs1, rd, imm);

    } else if (check(instruction, branchmask)) {
        fprintf(logfile, "%08x - branch inst\n", instruction);

        gprnextpc();

    } else if (check(instruction, loadmask)) {
        fprintf(logfile, "%08x - load\n", instruction);

        gprnextpc();

    } else if (check(instruction, storemask)) {
        fprintf(logfile, "%08x - store\n", instruction);

        gprnextpc();

    } else if (check(instruction, luimask)) {
        unsigned int
            rd = getrd(instruction),
            uimm = getuimm(instruction);

        fprintf(logfile, "[LUI] %08x : rd %d, upperimm %d\n", instruction, rd, uimm);
        retval = lui(rd, uimm);

        gprnextpc();

    } else if (check(instruction, auipcmask)) {
        unsigned int
            rd = getrd(instruction),
            uimm = getuimm(instruction);

        fprintf(logfile, "[AUIPC] %08x : rd %d, upperimm %d\n", instruction, rd, uimm);
        retval = auipc(rd, uimm);

        gprnextpc();

    } else {
        return EINVAL;
    }

    if (retval) {
        if (retval == ENOEXEC) {  // NOEXEC means nothing left to execute
            return ENOEXEC;       // this happens when jr ra is called with ra = 0
        }
        printf("[ERROR] %s got retval = %d\n", __func__, retval);
        return(retval);
    }

    return errno;
}
