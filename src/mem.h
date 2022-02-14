#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#define word     32
#define halfword 16
#define byte     8

#define wordalignment     sizeof(int)
#define halfwordalignment sizeof(int)/2
#define bytealignment     sizeof(char)

#define halfwordlowmask  65535      // 0x0000ffff
#define halfwordhighmask 4294901760 // 0xffff0000

#define byte1mask        255        // 0x000000ff
#define byte1maskinv     4294967040 // 0xffffff00
#define byte2mask        65280      // 0x0000ff00
#define byte2maskinv     4294902015 // 0xffff00ff
#define byte3mask        16711680   // 0x00ff0000
#define byte3maskinv     4278255615 // 0xff00ffff
#define byte4mask        4278190080 // 0xff000000
#define byte4maskinv     16777215   // 0x00ffffff

#define ramaddress(x) (x)/wordalignment

extern int decodeandcall(unsigned int instruction);
extern unsigned int gprfgetpc();

extern FILE *logfile;

int memwrite32u(unsigned int address, unsigned int value);
int memwrite16u(unsigned int address, unsigned int value);
int memwrite8u(unsigned int address, unsigned int value);

unsigned int memread32u(unsigned int address);
unsigned int memread16u(unsigned int address);
unsigned int memread8u(unsigned int address);

int initmemory(unsigned int size, char *memfilename);
