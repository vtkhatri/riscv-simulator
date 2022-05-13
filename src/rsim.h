#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

extern FILE *logfile, *memlogfile, *gprflogfile;
extern char *logfilename;

void stripextension(char *filename); // might be usefule somewhere else
int signextend(int value, int immlength);
