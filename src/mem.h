#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#define word     32
#define halfword 16
#define byte     8

#define wordalignment     sizeof(int)
#define halfwordalignment sizeof(short)
#define bytealignment     sizeof(char)

#define halfwordlowmask  65535      // 0x0000ffff
#define halfwordhighmask 4294901760 // 0xffff0000

#define bytemask        255        // 0x000000ff

#define ramaddress(x) (x)/wordalignment

extern int decodeandcall(unsigned int instruction);
extern unsigned int gprgetpc();

extern FILE *logfile, *memlogfile;

int memwrite32u(unsigned int address, unsigned int value);
int memwrite16u(unsigned int address, unsigned int value);
int memwrite8u(unsigned int address, unsigned int value);

unsigned int memread32u(unsigned int address);
unsigned int memread16u(unsigned int address);
unsigned int memread8u(unsigned int address);

int initmemory(unsigned int size, char *memfilename);

int load(unsigned int rd, unsigned int rs1, unsigned int funct3, unsigned int ldimm);

int store(unsigned int rs1, unsigned int rs2, unsigned int funct3, unsigned int stimm);
