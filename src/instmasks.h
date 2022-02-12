/* Integer register-immediate instructions*/
#define luimask   55 // 0110111
#define auipcmask 23 // 0010111

#define registerimmediatemask 19 // 0010011

#define funct3add          0 // 000
#define funct3shiftleftl   1 // 001
#define funct3setlessthan  2 // 010
#define funct3setlessthanu 3 // 011
#define funct3xor          4 // 100
#define funct3shiftrightl  5 // 101
#define funct3or           6 // 110
#define funct3and          7 // 111

#define funct7shiftrightl  0  // 0000000
#define funct7shiftrighta  32 // 0100000

// nop is encoded as addi x0, x0, 0 - 000000000000 00000 000 00000 0010011 = 19
#define nopmask 19

/* Integer register register operations*/
#define registerregistermask 51 // 0110011

// all funct3 masks and funct7 masks same as register immediate

/*Control transfer instructions*/

/*Uncondtional jumps*/
#define jalmask  111 // 1101111
#define jalrmask 103 // 1100111

/*Conditional branches*/
#define branchmask 99 // 1100011

#define funct3equal         0 // 000
#define funct3notequal      1 // 001
#define funct3lessthan      4 // 100
#define funct3greaterequal  5 // 101
#define funct3lessthanu     6 // 110
#define funct3greaterequalu 7 // 111

/*Load and store instructions*/

#define loadmask  3  // 0000011
#define storemask 35 // 0100011

#define funct3byte      0 // 000
#define funct3halfword  1 // 001
#define funct3word      2 // 010
#define funct3byteu     4 // 100
#define funct3halfwordu 5 // 101

/*Environment call and break point*/

#define ecallmask  115     // 0................ 1110011
#define ebreakmask 1048691 // 1 00000 000 00000 1110011


