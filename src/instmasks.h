/* opcode extraction and checking */
#define check(x,y) ((x) & 127) == (y)

#define getrd(x) ((x) & (31 << 7)) >> 7
#define getrs1(x) ((x) & (31 << 15)) >> 15
#define getrs2(x) ((x) & (31 << 20)) >> 20

#define getfunct3(x) ((x) & (7 << 12)) >> 12
#define getfunct7(x) ((x) & (127 << 25)) >> 25

/* register-immediate */
#define luimask   55 // 0110111
#define auipcmask 23 // 0010111

#define getuimm(x) ((x) & (1048575 << 12)) // upper 20-bits from 32-bit inst, not shifte

#define registerimmediatemask 19 // 0010011
#define getregimmimm(x) ((x) & (4095 << 20)) >> 20 // inst[31:20] = imm[11:0]
#define getshamtfromimm(x) (x) & 31 // imm[4:0] = shift amount

#define funct3add          0 // 000
#define funct3shiftleftl   1 // 001
#define funct3setlessthan  2 // 010
#define funct3setlessthanu 3 // 011
#define funct3xor          4 // 100
#define funct3shiftright   5 // 101
#define funct3or           6 // 110
#define funct3and          7 // 111

#define funct7shiftrightl  0  // 0000000
#define funct7shiftrighta  32 // 0100000


// nop is encoded as addi x0, x0, 0 - 000000000000 00000 000 00000 0010011 = 19
#define nopmask 19

/* register register */
#define registerregistermask 51 // 0110011

// all funct3 masks same as register immediate
#define funct7add 0  // 0000000
#define funct7sub 32 // 0100000

/* control instructions */
/* uncondtional jumps */
#define jalmask  111 // 1101111
#define getjalimm(x) (((x) & (1 << 31)) >> 11) \
                   + (((x) & (1023 << 30)) >> 20) \
                   + (((x) & (1 << 20)) >> 9) \
                   + (((x) & (127 << 12))) // inst[31:12] = imm[20|10:1|11|19:12]

#define jalrmask 103 // 1100111
#define getjalrimm(x) ((x) & (4095 << 20)) >> 20 // inst[31:20] = imm[11:0]

/* conditional branches */
#define branchmask 99 // 1100011

#define funct3equal         0 // 000
#define funct3notequal      1 // 001
#define funct3lessthan      4 // 100
#define funct3greaterequal  5 // 101
#define funct3lessthanu     6 // 110
#define funct3greaterequalu 7 // 111

/* memory operations */

#define loadmask  3  // 0000011
#define storemask 35 // 0100011

#define funct3byte      0 // 000
#define funct3halfword  1 // 001
#define funct3word      2 // 010
#define funct3byteu     4 // 100
#define funct3halfwordu 5 // 101

/* env call and env break */

#define ecallmask  115     // 0................ 1110011
#define ebreakmask 1048691 // 1 00000 000 00000 1110011


