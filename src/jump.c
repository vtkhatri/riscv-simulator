#include "mem.h"
#include "gprf.h"
#include "rsim.h"
#include "instmasks.h"

#include "jump.h"

int jumpandlinkregister(unsigned int rs1, unsigned int rd, unsigned int imm) {
    errno = 0;

    unsigned int pc = gprgetpc()+4;
    unsigned int newpc = gprread(rs1) + signextend(imm, 12);

    // exit condition - jr ra when ra has all zeros
    if (rs1 == ra && 0 == gprread(rs1)) {
        return ENOEXEC;
    }

    // deciding if to save current PC
    if (rd == zero) {
        #if(debug == all)
        fprintf(logfile, "[JALR] no linking, just jump\n");
        #endif
    } else {
        gprwrite(rd, pc);

        #if(debug == all)
        fprintf(logfile, "[JALR] saved pc %08x to %d register\n", pc, rd);
        #endif
    }

    if (gprread(rs1) % 4) {
        fprintf(logfile, "[ERROR] mis-aligned jump address (%08x)", gprread(rs1));
        return EFAULT;
    }

    // update pc
    gprputpc(newpc);

    return errno;
}

int jumpandlink(unsigned int rd, unsigned int jalimm) {
    errno = 0;

    // deciding if to save current PC
    if (rd == zero) {
        #if(debug == all)
        fprintf(logfile, "[JAL] no linking, just jump\n");
        #endif
    } else {
        gprwrite(rd, gprgetpc()+4);

        #if(debug == all)
        fprintf(logfile, "[JAL] saved pc %08x to %d register\n", gprgetpc()+4, rd);
        #endif
    }

    if (jalimm % 4) {
        fprintf(logfile, "[ERROR] mis-aligned addition to pc (%08x)", jalimm);
        return EFAULT;
    }

    // update pc
    gprputpc(gprgetpc()+signextend(jalimm,21));

    return errno;
}
