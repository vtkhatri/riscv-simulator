    addi s10, zero, 10
    jr ra
factorial:
    addi sp, sp, -8 # save regs
    sw a0, 4( sp )
    sw ra, 0( sp )
    addi t0, zero, 1 # temporary = 1
    bgt a0, t0, else # if n>1, go to else
    addi a0, zero, 1 # otherwise, return 1
    addi sp , sp , 8 # restore sp
    jr ra # return
else:
    addi a0, a0, -1 # n = n − 1
    jal factorial # recursive call
    lw t1, 4( sp ) # restore n into t1
    lw ra, 0( sp ) # restore ra
    addi sp, sp, 8 # restore sp
    mul a0, t1, a0 # a0=n*factorial(n−1)
    jr ra # return
main:
    addi sp, sp, -4
    sw ra, 0(sp)     # storing return address
    addi a0, zero, 6 # !6 = 720
    jal factorial
    li s11, 0        # simple test for jalr
    jalr s11, 0      # can it go to code and return properly
    lw ra, 0(sp)     # restoring return address
    addi sp, sp, 4
    jr ra
