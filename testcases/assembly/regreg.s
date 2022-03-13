main:
    li a1, -1
    li a2, -3
    add a0, a1, a2
    sub a0, a1, a2
    slt a0, a1, a2
    sltu a0, a1, a2
    xor a0, a1, a2
    li a3, 1
    srl a0, a2, a3
    sra a0, a2, a3
    or a0, a1, a2
    and a0, a1, a2
    jr ra