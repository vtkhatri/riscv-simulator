#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "rsim.h"

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
    printf("Usage -\n  ./rsim <path to memory image file> <starting address> <stack address>\n");
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
        programcounter = (int) strtol(argv[2], NULL, 16);
        if (programcounter == 0) {
            // printf("[INFO] program counter = 0,"); // TODO : should we print this? def value is 0.
        }
    } else {
        memfile = fopen(argv[1], "r");
        if (memfile == NULL) {
            printf("[ERROR] could not open file %s, quitting.\n", argv[1]);
            return(1);
        }
        programcounter = (int) strtol(argv[2], NULL, 16);
        if (programcounter == 0) {
            // printf("[INFO] program counter = 0,"); // TODO : should we print this? def value is 0.
        }
        stackaddress = atoi(argv[3]);
        if (stackaddress == 0) {
            printf("[ERROR] invalid stack address 0, quitting.\n");
            return(1);
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

    fprintf(logfile, "memory file = %s\nprogram counter = %d\nstack address = %d\n", memfilename, programcounter, stackaddress);

    return(0);
}
