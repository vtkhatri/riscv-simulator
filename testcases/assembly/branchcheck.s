main:
    addi a0, zero, 1
    addi a1, zero, -1
    blt  a0, a1, signedless
    bltu a0, a1, less
signedless:
    jal less
less:
    bgtu a0, a1, greater
    bgt  a0, a1, signedgreater
greater:
    jal skip
signedgreater:
    jal skip
skip:
    addi ra, zero, 0
    jr ra
