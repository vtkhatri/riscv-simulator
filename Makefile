.PHONY: all clean test build

SRCDIRS := src/
CCARGS := -g

ifdef debug # allows us to dynamically define a pre-processor directive from make command-line
	CCARGS += -D debug=$(debug)
endif

TARGETEXEC := rsim # Risc-v SIMulator -> rsim

SRCFILES := $(shell find $(SRCDIRS) -name '*.c')

all: build test

clean: rm $(TARGETEXEC)

build:
	@echo $(CC) $(CCARGS) $(SRCFILES) -o $(TARGETEXEC)

test:
	@echo "not implemented yet"
	@echo "should look for files in 'testcases/' and exectute them one by one, storing the outputs in the same/different file."