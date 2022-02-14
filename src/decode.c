#include "decode.h"
#include "gprf.h"

int jumpandlinkregister(unsigned int instruction) {
    int rd = getrd(instruction);

    if (rd == ra && 0 == gprread(rd)) {
        return ENOEXEC;
    }
    // todo : actually jump and link
    
    return 0;
}

int decodeandcall(unsigned int instruction) {
    int retval = 0;

    if (check(instruction, registerimmediatemask)) {
        fprintf(logfile, "%08x - register immediate\n", instruction);
    } else if (check(instruction, registerregistermask)) {
        fprintf(logfile, "%08x - register register\n", instruction);
    } else if (check(instruction, jalmask)) {
        fprintf(logfile, "%08x - jal\n", instruction);
    } else if (check(instruction, jalrmask)){
        retval = jumpandlinkregister(instruction);
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
    unsigned int pc = gprfgetpc();
    fprintf(logfile, "old pc = %08x\n", pc);
    pc = pc + 4;
    gprfputpc(pc);
    fprintf(logfile, "new pc = %08x\n", pc);

    return 0;
}
