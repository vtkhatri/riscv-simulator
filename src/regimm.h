#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

int registerimmediate(unsigned int rd,
                      unsigned int rs1,
                      unsigned int funct3,
                      unsigned int funct7,
                      unsigned int imm);

int lui(unsigned int rd,
        unsigned int uimm);

int auipc(unsigned int rd,
          unsigned int uimm);
