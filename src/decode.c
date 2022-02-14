#include "decode.h"

#include "instmasks.h"
#include "regimm.h"
// #include "regreg.h"
// #include "jal.h"
#include "jalr.h"
// #include "branch.h"
#include "mem.h"
#include "gprf.h"

int decodeandcall(unsigned int instruction) {
    int retval = 0;

    if (check(instruction, registerimmediatemask)) {
        unsigned int
            rd = getrs1(instruction),
            rs1 = getrd(instruction),
            funct3 = getfunct3(instruction),
            funct7 = getfunct7(instruction),
            imm = getregimmimm(instruction);

        retval = registerimmediate(rd, rs1, funct3, funct7, imm);

    } else if (check(instruction, registerregistermask)) {
        fprintf(logfile, "%08x - register register\n", instruction);

    } else if (check(instruction, jalmask)) {
        fprintf(logfile, "%08x - jal\n", instruction);

    } else if (check(instruction, jalrmask)){
        unsigned int
            rs1 = getrs1(instruction),
            rd = getrd(instruction),
            imm = getjalrimm(instruction);

        retval = jumpandlinkregister(rs1, rd, imm);

    } else if (check(instruction, branchmask)) {
        fprintf(logfile, "%08x - branch inst\n", instruction);

    } else if (check(instruction, loadmask)) {
        fprintf(logfile, "%08x - load\n", instruction);

    } else if (check(instruction, storemask)) {
        fprintf(logfile, "%08x - store\n", instruction);

    } else if (check(instruction, luimask)) {
        fprintf(logfile, "%08x - lui\n", instruction);

    } else if (check(instruction, auipcmask)) {
        fprintf(logfile, "%08x - auipc / call\n", instruction);

    } else {
        return EINVAL;
    }

    if (retval) {
        if (retval == ENOEXEC) {
            return ENOEXEC;
        }
        printf("[ERROR] %s got retval = %d\n", __func__, retval);
        return(retval);
    }
    // temp
    unsigned int pc = gprgetpc();
    fprintf(logfile, "old pc = %08x\n", pc);
    pc = pc + 4;
    gprputpc(pc);
    fprintf(logfile, "new pc = %08x\n", pc);

    return 0;
}
