#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

FILE *logfile, *memlogfile, *gprflogfile; // global logfile so that everything can dump to that file as required
char *logfilename;

void stripextension(char *filename); // might be usefule somewhere else
