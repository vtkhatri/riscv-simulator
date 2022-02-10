main:
	addi	t0, zero, 0
	addi	t0, t0, 92
	bnez	t0, .L1
	addi	t0, t0, -2
	j	L2
L1:
	addi	t0, t0, -90
L2:
	addi	t0, t0, -2
	j	ra
