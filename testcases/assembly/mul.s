main:
	li a0, 513
	lui a1, 0x80000
	mulh a2, a1, a0
	mul a3, a1, a0
	li a6, -1
	li a7, 2
	mulhu s2, a6, a7
	mulhsu s3, a6, a7
	li s8, -11
	li s9, 3
	div s10, s8, s9
	divu s11, s8, s9
	rem t3, s8, s9
	remu t4, s8, s9
	jr ra
