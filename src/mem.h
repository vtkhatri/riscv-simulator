#include "rsim.h"

int memwrite32u(unsigned int address, unsigned int value);
int memwrite16u(unsigned int address, unsigned int value);
int memwrite8u(unsigned int address, unsigned int value);

unsigned int memread32u(unsigned int address);
unsigned int memread16u(unsigned int address);
unsigned int memread8u(unsigned int address);

int initmemory(unsigned int size, char *memfilename);
