main:
	li a0, 513
	lui a1, 0x80000
	mulh a2, a1, a0
	mul a3, a1, a0
	jr ra
