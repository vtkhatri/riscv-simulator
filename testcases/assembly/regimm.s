main:
    li a0, 1
    sll a0, a0, 33
    li a0, -1
    addi a0, a0, -1
    addi a0, a0, 1
    slti a1, a0, 0
    sltiu a1, a0, 0
    li a0, -16
    xori a0, a0, -1
    li a0, -16
    srai a0, a0, 1
    srli a0, a0, 1
    srai a0, a0, 1
    li a0, 1
    ori a0, a0, 4
    andi a0, a0, 1
    lui a0, 0xfffff
    auipc a0, 0xfffff
    jr ra
