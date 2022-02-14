#include "mem.h"
#include "gprf.h"
#include "instmasks.h"

#include "jalr.h"

int jumpandlinkregister(unsigned int rs1, unsigned int rd, unsigned int imm) {
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
        gprwrite(rd, gprgetpc());

        #if(debug == all)
        fprintf(logfile, "[JALR] saved pc to %d register\n", rd);
        #endif
    }

    // update pc
    gprputpc(gprread(rs1) + imm);

    return 0;
}
