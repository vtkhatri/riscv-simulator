#include "rsim.h"

#include "debugtype.h"
#include "mem.h"
#include "decode.h"
#include "gprf.h"

static debugtypet debugtype; 

void stripextension(char *filename) {
    char *end = filename + strlen(filename);

    while (end > filename && *end != '.') {
        --end;
    }

    if (end > filename) {
        *end = '\0';
    }
}

int handledebugtype(debugtypet debugtype) {
    switch(debugtype) {
    case gdb:
        logfile = stdout;
        return loopcheck();
        break;
    case tracemem:
        fclose(logfile);
        memlogfile = fopen(logfilename, "a");
        logfile = stdout;
        return EPERM;
    case tracegprf:
        fclose(logfile);
        gprflogfile = fopen(logfilename, "a");
        logfile = stdout;
        return EPERM;
    case traceall:
        fclose(logfile);
        memlogfile = fopen(logfilename, "a");
        gprflogfile = memlogfile;
        return EPERM;
    default:
        fprintf(stderr, "[ERROR] debug type? %d", debugtype);
        return EINVAL;
    }
}

void printUsage() {
    printf("Usage -\n  ./rsim <path to memory image file> <starting address in hex> <stack address> <debugtype>\n");
    return;
}

int mainloop(debugtypet debugtype) {
    errno = 0;
    unsigned int pc, retval, instruction;
    unsigned int tocheck = 0;

    do {
        retval = 0;
        if (tocheck != EPERM) tocheck = handledebugtype(debugtype);
        pc = gprgetpc();
        instruction = memread32u(pc);
        fprintf(logfile, "[INST] inst fetched from %08x -> %08x\n", pc, instruction);
        retval = decodeandcall(instruction);
        if (retval == ENOEXEC) {
            fprintf(logfile, "[FIN] jr ra called with ra containing 0; pc = %08x\n", pc);
            return 0;
        }
    } while (retval == 0);

    return retval;
}

int main(int argc, char** argv) {

    if (argc > 5) {
        printf("too many arguments.\n");
        printUsage();
        return(1);
    }

    // initialize whatever is required
    FILE *memfile = fopen("testcases/assembly/first.mem", "r");
    unsigned int programcounter = 0;
    unsigned int stackaddress = 65536;
    debugtype = tracemem;

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
    } else if (argv[4] == NULL) {
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
        stackaddress = (unsigned int) strtoul (argv[3], NULL, 16); // in hex because we get it from dumping .o files
        if (stackaddress == 0 || stackaddress % 4) {
            printf("[ERROR] invalid stack address %08x(%0d), quitting.\n", stackaddress, stackaddress);
            return (1);
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
        stackaddress = (unsigned int) strtoul (argv[3], NULL, 16); // in hex because we get it from dumping .o files
        if (stackaddress == 0 || stackaddress % 4) {
            printf("[ERROR] invalid stack address %08x(%0d), quitting.\n", stackaddress, stackaddress);
            return (1);
        }
        if (strstr(argv[4], "gdb")) debugtype = gdb;
        else if (strstr(argv[4], "tracegprf")) debugtype = tracegprf;
        else if (strstr(argv[4], "tracemem")) debugtype = tracemem;
        else if (strstr(argv[4], "traceall")) debugtype = traceall;
        else fprintf(stderr, "undefined output type, assuming tracefile for mem output.\n");
    }

    // initializing logfile
    logfilename = (char *) malloc (strlen(memfilename) +1);
    strcpy(logfilename, memfilename);
    stripextension(logfilename);
    strcat(logfilename, ".log");

    memlogfile = fopen("/dev/null", "w");
    gprflogfile = fopen("/dev/null", "w");
    logfile = fopen(logfilename, "w");
    if (logfile == NULL) {
        printf("[ERROR] couldn't initialize logfile %s, quitting.\n", logfilename);
        return(1);
    }

    switch (debugtype) {
        case gdb:
            logfile = stdout;
            break;
        case tracegprf:
            gprflogfile = logfile;
            break;
        case tracemem:
            memlogfile = logfile;
            break;
        case traceall:
            memlogfile = logfile;
            gprflogfile = logfile;
            break;
        default:
            memlogfile = logfile;
            break;
    }

    // distinct prints to logfile to check if everything is working.
    fprintf(logfile, "memory file = %s\nprogram counter = %08x(%d)\nstack address = %d\ndebugtype = %d\n",
                     memfilename, programcounter, programcounter, stackaddress, debugtype);

    // initializing memory
    int retval = initmemory(stackaddress, memfilename);
    if (retval) return(retval);

    // initializing pc and gprf
    retval = initgprf(programcounter, stackaddress);
    if (retval) return(retval);

    retval = mainloop(debugtype);
    if (retval != 0) {
        fprintf(stderr, "[ERROR] mainloop returns non-zero retval.\n");
    }

    fclose(logfile);
    return(retval);
}

