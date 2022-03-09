main:
    addi a0, zero, 1
    addi a1, zero, -1
    blt  a0, a1, signedless
    bltu a0, a1, less
signedless:
    jr ra
less:
    bgeu  a0, a1, signedgreater
    bge a0, a1, greater
signedgreater:
    jr ra
greater:
    addi a0, zero, -1
    addi a1, zero, -2
    blt  a0, a1, signedless
    bltu a0, a1, negless
negless:
    addi a0, zero, 2
    addi a1, zero, 2
    blt a0, a1, signedless
    bltu a0, a1, signedless
    bge a0, a1, equal
equal:
    bge a0, a1, endref
endref:
    addi ra, zero, 0
    jr ra
