#include "rsim.h"

int memwrite32u(unsigned int address, unsigned int value);
int memwrite16u(unsigned int address, unsigned int value);
int memwrite8u(unsigned int address, unsigned int value);

int memread32u(unsigned int address, unsigned int *value);
int memread16u(unsigned int address, unsigned int *value);
int memread8u(unsigned int address, unsigned int *value);

int initmemory(unsigned int size, char *memfilename);
