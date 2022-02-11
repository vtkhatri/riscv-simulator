.PHONY: all clean test testfiles memfiles build

SRCDIRS := src
SRCS := $(shell find $(SRCDIRS) -name '*.c')
TESTDIRS := testcases
TESTS := $(shell find $(TESTDIRS) -name '*.c')
CASSS := $(TESTS:c=s)
ASSSS := $(shell find $(TESTDIRS) -name '*.s')
ASSS := $(CASSS) $(ASSSS)
OBJS := $(ASSS:s=o)
MEMS := $(ASSS:s=mem)
LOGS := $(ASSS:s=log)
PCS := $(ASSS:s=pc)

PROGRAMCOUNTER := 0
STACKADDRESS := 65535

ifdef MEMDIRS
	MEMS += $(shell find $(MEMDIRS) -name '*.mem') # ability to provide mem files
endif

ifdef ASSSDIRS
	ASSS += $(shell find $(ASSSDIRS) -name '*.s') # ability to provide assembly files
endif

RVOBJDUMP := /pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain/bin/riscv64-unknown-linux-gnu-objdump
RVGCC := /pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain/bin/riscv64-unknown-linux-gnu-gcc
RVAS := /pkgs/riscv-gnu-toolchain/riscv-gnu-toolchain/bin/riscv64-unknown-linux-gnu-as

GREPCLEAN := grep -o '^[[:blank:]]*[[:xdigit:]]*:[[:blank:]][[:xdigit:]]*'
GETPC := awk '/<main>:/ {print $$1}'

EXEC := ./rsim # Risc-v SIMulator -> rsim
CCARGS := -g

ifdef debug # allows us to dynamically define a pre-processor directive from make command-line
	CCARGS += -D debug=$(debug)
endif

all: build testfiles test

clean:
	rm -f $(EXEC) $(OBJS) $(MEMS) $(CASSS) $(LOGS) $(PCS)

build:
	$(CC) $(CCARGS) $(SRCS) -o $(EXEC)

test: $(LOGS)

%.log: %.mem %.pc
	$(EXEC) $< $$(cat $(filter-out $<,$^)) $(STACKADDRESS)

testfiles: $(ASSS) $(OBJS) $(MEMS) $(PCS)

%.s: %.c
	$(RVGCC) -S -fpic -march=rv32i -mabi=ilp32 $< -o $@

%.o: %.s
	$(RVAS) -ahld $< -o $@

%.mem: %.o
	$(RVOBJDUMP) -d $< | $(GREPCLEAN) > $@

%.pc: %.o
	$(RVOBJDUMP) -d $< | $(GETPC) > $@
