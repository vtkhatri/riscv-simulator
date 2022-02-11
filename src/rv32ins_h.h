/* Integer register-immediate instructions*/

ADDI  /*Add immediate*/
SLTIU /*Set less than immediate unsigned*/
SLTI  /* Set less than immediate*/
ANDI /*Bitwise AND immediate*/
ORI /*Bitwise OR immediate*/
XORI /*Bitwise XOR immediate*/
SLLI /*logical left shift immediate*/
SRLI /*Logical right shift immediate*/
SRAI /*Artihmetic right shift*/
LUI /*Load Upper Immediate*/
AUIPC /*Add upper immediate to program counter*/

/* Integer register register operations*/

ADD /*Addition reg-reg*/
SLT /*set less than*/
SLTU /*set less than unsigned*/
AND /*bitwise and*/
OR /*bitwise or*/
XOR /*bitwise xor*/
SLL /*logical left shift*/
SRL /*logical right shift*/
SRA /*arithmetic right shift*/
SUB /*Subtraction*/

NOP /*encoded as ADDI x0,x0,0*/

/*Control transfer instructions*/

/*Uncondtional jumps*/

JAL /*Jump and Link*/
JALR /*Jump and link register*/

/*Conditional branches*/

BEQ /*branch if equal*/
BNE /*branch if not equal*/
BLTU /*branch if less than unsigned*/
BLT /*branch if less than signed*/
BGEU /* branch if greater than or equal to unsigned*/
BGE /* branch if greater than or equal to signed*/
BGT /*branch if greater than signed*/
BGTU /*branch if greater than unsigned*/
BLE /*branch if less than or equal to signed*/
BLEU /*branch if less than or equal to unsigned*/

/*Load and store instructions*/

LW /*load 32 bit value from memory to register*/
LH /*loads 16 bit value from memory, sign extends to 32 bits and store to register*/
LHU /*loads 16 bit value from memory, zero extends to 32 bits and store to register*/
LB /*loads 8 bit value from memory, sign extend to 32 bits and store to register*/
LBU /*loads 8 bit value from memory, zero extends to 32 bits and store to register*/
SW /*store 32 bit from register to memory*/
SH /*store low 16 bit value from register to memory*/
SB /*store low 8 bit value from register to memory*/

/*Environment call and break point*/

ECALL/*used to make service request to execution environment*/
EBREAK/*used to return control to debugging environment*/


