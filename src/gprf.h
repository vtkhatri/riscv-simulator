enum {
    // list all 32 register names
}

int gprread(unsigned int index, unsigned int *value);
int gprwrite(unsigned int index, unsigned int *value);

int gprlui(unsigned int index, unsigned int *value);
int auipc(unsigned int *value);
