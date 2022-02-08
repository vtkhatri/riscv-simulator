.PHONY: all clean test testfiles memfiles build

SRCDIRS := src
SRCS := $(shell find $(SRCDIRS) -name '*.c')
TESTDIRS := testcases
TESTS := $(shell find $(TESTDIRS) -name '*.c')
ASSS := $(TESTS:c=s)
OBJS := $(TESTS:c=o)
MEMS := $(TESTS:c=mem)

# MEMS += $(shell find $(MEMDIRS) -name '*.mem') # ability to provide mem files

RVOBJDUMP := /pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain/bin/riscv64-unknown-linux-gnu-objdump
RVGCC := /pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain/bin/riscv64-unknown-linux-gnu-gcc
RVAS := /pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain/bin/riscv64-unknown-linux-gnu-as

GREPCLEAN := grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*'

EXEC := rsim # Risc-v SIMulator -> rsim
CCARGS := -g

ifdef debug # allows us to dynamically define a pre-processor directive from make command-line
	CCARGS += -D debug=$(debug)
endif

all: build testfiles test

clean:
	rm -f $(EXEC) $(OBJS) $(MEMS) $(ASSS)

build:
	$(CC) $(CCARGS) $(SRCS) -o $(EXEC)

test:
	@echo "not implemented yet"
	@echo "should look for files in 'memfiles/' and pass them one by one to our executable, storing output as required."

testfiles: $(ASSS) $(OBJS) $(MEMS)

$(TESTDIRS)/%.s: $(TESTDIRS)/%.c
	$(RVGCC) -S -fpic -march=rv32i -mabi=ilp32 $< -o $@

$(TESTDIRS)/%.o: $(TESTDIRS)/%.s
	$(RVAS) -ahld $< -o $@

$(TESTDIRS)/%.mem: $(TESTDIRS)/%.o
	$(RVOBJDUMP) -d $< | $(GREPCLEAN) > $@