#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#include "mem.h"

static unsigned int *ram;

int initmemory(unsigned int size, char *memfilename) {
    ram = (unsigned int *) malloc (size); // assuming size is passed as bytes.
    if (ram == NULL) {
        printf("[ERROR] could not initialize memory of size %d", size);
        return (1);
    }

    FILE *memfile = fopen(memfilename, "r");
    if (memfile == NULL) {
        printf("[ERROR] could not open memfile");
        return (2);
    }

    char *address = (char *) malloc (5); // 64k address space, should be represented by 16-bits = 4 hex characters + 1 colon
    char *value = (char *) malloc (8); // 32-bit value, 8 hex characters.

    while (fscanf(memfile, "%s %[^\n]s", address, value) != EOF) {
        char *end = address + strlen(address) - 1;
        if (*end == ':') *end = '\0';
        else {
            printf("[ERROR] address scanned (%s) does not have a colon, misformatted mem file %s", address, memfilename);
            return (3);
        }

        unsigned int addressint = (unsigned int) strtoul (address, NULL, 16);
        if (addressint == 0) {
            if (errno == EINVAL) {
                printf("[ERROR] invalid input %s", address);
                return (4);
            }
            if (errno == ERANGE) {
                printf("[ERROR] address too big %s", address);
                return (4);
            }
        }

        unsigned int valueint = (unsigned int) strtoul (value, NULL, 16);
        if (valueint == 0) {
            if (errno == EINVAL) {
                printf("[ERROR] invalid input %s", address);
                return (4);
            }
            if (errno == ERANGE) {
                printf("[ERROR] value too big %s", value);
                return (4);
            }
        }

        printf("%s=%d %s=%d\n", address, addressint, value, valueint);
        ram[addressint] = valueint;
    }

    fclose(memfile);
    return (0);
}