.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
    li $v0,4
    la $a0,_prompt
    syscall
    li $v0,5
    syscall
    jr $ra

print:
    li $v0,1
    syscall
    li $v0,4
    la $a0,_ret
    syscall
    move $v0,$0
    jr $ra
fibo:
	li $t1,0
	move $t2,$t1
	move $t1,$t3
	li $t4,1
	move $v0,$t4
	jr $ra
label1:
main:
	li $t4,2
	add $t5,$t6,$t4
	sub $t4,$t5,$t5
	move $t3,$t4
label5:
label8:
	ble $t6,$t3,label7
	j label6
label7:
	move $t0,$a0
	move $a0,$t5
	move $t0,$a0
	move $a0,$t6
	addi $sp,$sp,-24
	sw $t0,0($sp)
	sw $ra,4($sp)
	sw $t1,8($sp)
	sw $t2,12($sp)
	sw $t3,16($sp)
	sw $t4,20($sp)
	jal fibo
	lw $a0,0($sp)
	lw $ra,4($sp)
	lw $t1,8($sp)
	lw $t2,12($sp)
	lw $t3,16($sp)
	lw $t4,20($sp)
	addi $sp,$sp,24
	move $t4 $v0
	move $t5,$t4
	li $t4,1
	add $t7,$t6,$t4
	move $t6,$t7
	j label8
label6:
	li $t4,1
	move $v0,$t4
	jr $ra
label3:
