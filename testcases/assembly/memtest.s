main:
    addi  sp, sp, -12
    addi  t0, zero, 1
    sb    t0, 0(sp)
    addi  t0, t0, 1
    sb    t0, 1(sp)
    addi  t0, t0, 1
    sb    t0, 2(sp)
    addi  t0, t0, 1
    sb    t0, 3(sp)
    addi  t0, t0, 1
    sh    t0, 4(sp)
    addi  t0, t0, 1
    sh    t0, 6(sp)
    addi  t0, t0, 1
    sw    t0, 8(sp)
    lb    t1, 0(sp)
    lb    t1, 1(sp)
    lb    t1, 2(sp)
    lb    t1, 3(sp)
    lh    t1, 4(sp)
    lh    t1, 6(sp)
    lw    t1, 8(sp)
    jr    ra