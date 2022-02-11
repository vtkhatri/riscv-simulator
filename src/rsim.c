#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#include "rsim.h"
#include "mem.c"

void stripextension(char *filename) {
    char *end = filename + strlen(filename);

    while (end > filename && *end != '.') {
        --end;
    }

    if (end > filename) {
        *end = '\0';
    }
}

void printUsage() {
    printf("Usage -\n  ./rsim <path to memory image file> <starting address in hex> <stack address>\n");
    return;
}

int main(int argc, char** argv) {

    if (argc > 4) {
        printf("too many arguments.\n");
        printUsage();
        return(1);
    }

    // initialize whatever is required
    FILE *memfile = fopen("program.mem", "r");
    unsigned int programcounter = 0;
    unsigned int stackaddress = 65535;

    char *memfilename = (char *) malloc (strlen(argv[1]) + 1);
    memfilename = argv[1];

    if (argv[2] == NULL) {
        memfile = fopen(argv[1], "r");
        if (memfile == NULL) {
            printf("[ERROR] could not open file %s, quitting.\n", argv[1]);
            return(1);
        }
    } else if (argv[3] == NULL) {
        memfile = fopen(argv[1], "r");
        if (memfile == NULL) {
            printf("[ERROR] could not open file %s, quitting.\n", argv[1]);
            return(1);
        }
        programcounter = (unsigned int) strtoul(argv[2], NULL, 16); // in hex because we get it from dumping .o files
        if (programcounter == 0) {
            if (errno == EINVAL) {
                printf("[ERROR] invalid input %s", argv[2]);
                return (1);
            }
            if (errno == ERANGE) {
                printf("[ERROR] program counter too big %s", argv[2]);
                return (1);
            }
        }
    } else {
        memfile = fopen(argv[1], "r");
        if (memfile == NULL) {
            printf("[ERROR] could not open file %s, quitting.\n", argv[1]);
            return(1);
        }
        programcounter = (unsigned int) strtoul (argv[2], NULL, 16); // in hex because we get it from dumping .o files
        if (programcounter == 0) {
            if (errno == EINVAL) {
                printf("[ERROR] invalid input %s", argv[2]);
                return (1);
            }
            if (errno == ERANGE) {
                printf("[ERROR] value too big %s", argv[2]);
                return (1);
            }
        }
        stackaddress = (unsigned int) strtoul (argv[3], NULL, 10); // in hex because we get it from dumping .o files
        if (stackaddress == 0) {
            printf("[ERROR] invalid stack address 0, quitting.");
            return (1);
        }
    }

    // initializing logfile
    char *logfilename = (char *) malloc (strlen(memfilename) +1);
    strcpy(logfilename, memfilename);
    stripextension(logfilename);
    strcat(logfilename, ".log");

    logfile = fopen(logfilename, "w'");
    if (logfile == NULL) {
        printf("[ERROR] couldn't initialize logfile %s, quitting.\n", logfilename);
        return(1);
    }

    // distinct prints to logfile to check if everything is working.
    fprintf(logfile, "memory file = %s\nprogram counter = %d\nstack address = %d\n", memfilename, programcounter, stackaddress);

    // initializing memory
    int retval = initmemory(stackaddress, memfilename);
    if (retval) return(retval);


    fclose(logfile);
    return(0);
}
