# riscv-simulator
simulating rv32i instruction set

### make modifiers

to extend make's search range -
* for assembly files - `ASSSDIRS=<path>`
* for testcases (.c+.s) - `TESTDIRS=<path>`
* for mem files - `MEMDIRS=<path>`
to change debugtype -
* for memory tracefiles - `mem=1`
* for gprf tracefiles - `gprf=1`
* for gdb-like interface - `gdb=1`
* for all trace dump - `all=1`
