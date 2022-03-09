main:
    li a7, 63   # read
    li a2, 2    # 2 characters
    li a0, 0    # from stdin
    li a1, 100  # to location 100
    ecall
    li a7, 64   # write
    li a2, 2    # 2 characters
    li a1, 100  # from location 100
    li a0, 1    # to stdout
    ecall
    li a7, 94   # exit
    li a0, 666  # with code 666
    ecall
    jr ra
