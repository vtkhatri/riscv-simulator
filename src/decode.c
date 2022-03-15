#include "decode.h"

#include "instmasks.h"
#include "regimm.h"
#include "regreg.h"
#include "jump.h"
#include "branch.h"
#include "mem.h"
#include "gprf.h"
#include "ecall.h"

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

        fprintf(logfile, "[REGIMM] %08x : rd %d, rs1 %d(%08x), funct3 %x, funct7 %x, imm %x\n", instruction, rd, rs1, gprread(rs1), funct3, funct7, imm);
        retval = registerimmediate(rd, rs1, funct3, funct7, imm);

        gprnextpc();

    } else if (check(instruction, registerregistermask)) {
        unsigned int
            rd = getrd(instruction),
            rs1 = getrs1(instruction),
            rs2 = getrs2(instruction),
            funct3 = getfunct3(instruction),
            funct7 = getfunct7(instruction);

        fprintf(logfile, "[REGREG] %08x : rd %d, rs1 %d(%08x), rs2 %d(%08x), funct3 %x, funct7 %x\n",
                instruction, rd, rs1, gprread(rs1), rs2, gprread(rs2), funct3, funct7);
        retval = registerregister(rd, rs1, rs2, funct3, funct7);

        gprnextpc();

    } else if (check(instruction, jalmask)) {
        unsigned int
            rd = getrd(instruction),
            imm = getjalimm(instruction);

        fprintf(logfile, "[JAL] %08x : rd %d, imm %x\n", instruction, rd, imm);
        retval = jumpandlink(rd, imm);

    } else if (check(instruction, jalrmask)){
        unsigned int
            rs1 = getrs1(instruction),
            rd = getrd(instruction),
            imm = getjalrimm(instruction);

        fprintf(logfile, "[JALR] %08x : rd %d, rs1 %d(%08x), imm %x\n", instruction, rd, rs1, gprread(rs1), imm);
        retval = jumpandlinkregister(rs1, rd, imm);

    } else if (check(instruction, branchmask)) {
        unsigned int
            brimm = getbranchimm(instruction),
            rs1 = getrs1(instruction),
            rs2 = getrs2(instruction),
            funct3 = getfunct3(instruction);
        
        fprintf(logfile, "[BRANCH] %08x : rs1 %d(%08x), rs2 %d(%08x), funct3 %x, brimm %x\n",
                instruction, rs1, gprread(rs1), rs2, gprread(rs2), funct3, brimm);
        retval = branch(rs1, rs2, funct3, brimm);

    } else if (check(instruction, loadmask)) {
        unsigned int
            rd = getrd(instruction),
            rs1 = getrs1(instruction),
            funct3 = getfunct3(instruction),
            loadimm = getloadimm(instruction);
        
        fprintf(logfile, "[LOAD] %08x : rd %d, rs1 %d(%08x), funct3 %x, imm %x\n", instruction, rd, rs1, gprread(rs1), funct3, loadimm);
        retval = load(rd, rs1, funct3, loadimm);

        gprnextpc();

    } else if (check(instruction, storemask)) {
        unsigned int
            rs1 = getrs1(instruction),
            rs2 = getrs2(instruction),
            funct3 = getfunct3(instruction),
            storeimm = getstoreimm(instruction);
        
        fprintf(logfile, "[STORE] %08x : rs1 %d(%08x), rs2 %d(%08x), funct3 %x, imm %x\n", instruction, rs1, gprread(rs1), rs2, gprread(rs2), funct3, storeimm);
        retval = store(rs1, rs2, funct3, storeimm);

        gprnextpc();

    } else if (check(instruction, luimask)) {
        unsigned int
            rd = getrd(instruction),
            uimm = getuimm(instruction);

        fprintf(logfile, "[LUI] %08x : rd %d, upperimm %x\n", instruction, rd, uimm);
        retval = lui(rd, uimm);

        gprnextpc();

    } else if (check(instruction, auipcmask)) {
        unsigned int
            rd = getrd(instruction),
            uimm = getuimm(instruction);

        fprintf(logfile, "[AUIPC] %08x : rd %d, upperimm %x\n", instruction, rd, uimm);
        retval = auipc(rd, uimm);

        gprnextpc();

    } else if (check(instruction, ecallmask)) {
        fprintf(logfile, "[ECALL] %08x : a7 %0d, a0 %0d, a1 %0d, a2 %0d\n", instruction, gprread(a7), gprread(a0), gprread(a1), gprread(a2));
        retval = ecall();
        gprnextpc();

    } else if (instruction == 0) {
        fprintf(logfile, "[TRAP] %08x : exiting with all zero instruction\n", instruction);
        printgprf(stdout);
        exit(0);

    } else {
        return EINVAL;
    }

    if (retval) {
        if (retval == ENOEXEC) {  // NOEXEC means nothing left to execute
            return ENOEXEC;       // this happens when jr ra is called with ra = 0
        }
        printf("[ERROR] %s got retval = %x\n", __func__, retval);
        return(retval);
    }

    return errno;
}
