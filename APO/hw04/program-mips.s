
program code


build/program-mips:     file format elf32-tradbigmips


Disassembly of section my_text:

00404f10 <subroutine_fnc>:
  404f10:	27bdffd8 	addiu	sp,sp,-40 #sp = sp + (-40) -> sp += -40
  404f14:	afbe0024 	sw	s8,36(sp) # Mem[36 + sp] = s8 -> *sp + 36 = s8
  404f18:	03a0f025 	move	s8,sp # s8 = sp
  404f1c:	afc40028 	sw	a0,40(s8) # Mem[40 + s8] = a0 -> *s8 + 40 = a0
  404f20:	afc5002c 	sw	a1,44(s8) # Mem[44 + s8] = a1 -> *s8 + 44 = a1
  404f24:	afc0000c 	sw	zero,12(s8) # Mem[12 + s8] = zero -> *s8 + 12 = 0
  404f28:	1000001a 	b	404f94 <subroutine_fnc+0x84> # go to 404f94
  404f2c:	00000000 	nop
  404f30:	8fc4002c 	lw	a0,44(s8) # a0 = Mem[s8 + 44] -> a0 = *s8 + 44
  404f34:	27c2001c 	addiu	v0,s8,28 # v0 = s8 + 28
  404f38:	00402825 	move	a1,v0 # a1 = v0
  404f3c:	24060001 	li	a2,1 # a2 = 1
  404f40:	24020fa4 	li	v0,4004 # v0 = write
  404f44:	0000000c 	syscall # call write
  404f48:	afc70010 	sw	a3,16(s8) # Mem[s8 + 16] = a3 -> *s8 + 16 = a3
  404f4c:	afc20018 	sw	v0,24(s8) # Mem[s8 + 24] = v0 -> *s8 + 24 = v0
  404f50:	8fc20010 	lw	v0,16(s8) # v0 = Mem[s8 + 16] -> v0 = *s8 + 16
  404f54:	00000000 	nop
  404f58:	14400004 	bnez	v0,404f6c <subroutine_fnc+0x5c> # v0 != 0 ? 404f6c : next inst.
  404f5c:	00000000 	nop
  404f60:	8fc20018 	lw	v0,24(s8) #v0 = Mem[s8 + 24] -> v0 = *s8 + 24
  404f64:	10000002 	b	404f70 <subroutine_fnc+0x60> # go to 404f70
  404f68:	00000000 	nop
  404f6c:	2402ffff 	li	v0,-1 # v0 = -1
  404f70:	afc20008 	sw	v0,8(s8) # Mem[s8 + 8] = v0 -> *s8 + 8 = v0
  404f74:	8fc20008 	lw	v0,8(s8) # v0 = Mem[s8 + 8] -> v0 = *s8 + 8
  404f78:	00000000 	nop
  404f7c:	0440001c 	bltz	v0,404ff0 <subroutine_fnc+0xe0> # v0 < 0 ? 404ff0 : next inst.
  404f80:	00000000 	nop
  404f84:	8fc2000c 	lw	v0,12(s8) # v0 = Mem[s8 + 12] -> v0 = *s8 + 12
  404f88:	00000000 	nop
  404f8c:	24420001 	addiu	v0,v0,1 # v0 = v0 + 1 -> v0 += 1
  404f90:	afc2000c 	sw	v0,12(s8) # Mem[s8 + 12] = v0 -> *s8 + 12 = v0
  404f94:	8fc40028 	lw	a0,40(s8) # a0 = Mem[s8 + 40] -> a0 = *s8 + 40
  404f98:	27c2001c 	addiu	v0,s8,28 # v0 = s8 + 28
  404f9c:	00402825 	move	a1,v0 # a1 = v0
  404fa0:	24060001 	li	a2,1 # a2 = 1
  404fa4:	24020fa3 	li	v0,4003 # v0 = read
  404fa8:	0000000c 	syscall # call read
  404fac:	afc70010 	sw	a3,16(s8) # Mem[s8 + 16] = a3 -> *s8 + 16 = a3
  404fb0:	afc20014 	sw	v0,20(s8) # Mem[s8 + 20] = v0 -> *s8 + 20 = v0
  404fb4:	8fc20010 	lw	v0,16(s8) # v0 = Mem[s8 + 16] -> v0 = *s8 + 16
  404fb8:	00000000 	nop
  404fbc:	14400004 	bnez	v0,404fd0 <subroutine_fnc+0xc0> # v0 != 0 ? 404fd0 : next inst.
  404fc0:	00000000 	nop
  404fc4:	8fc20014 	lw	v0,20(s8) # v0 = Mem[s8 + 20] -> v0 = *s8 + 20
  404fc8:	10000002 	b	404fd4 <subroutine_fnc+0xc4> # go to 404fd4
  404fcc:	00000000 	nop
  404fd0:	2402ffff 	li	v0,-1 # v0 = -1
  404fd4:	afc20008 	sw	v0,8(s8) # Mem[s8 + 8] = v0 -> *s8 + 8 = v0
  404fd8:	8fc30008 	lw	v1,8(s8) # v1 = Mem[s8 + 8] -> v1 = *s8 + 8
  404fdc:	24020001 	li	v0,1 # v0 = 1
  404fe0:	1062ffd3 	beq	v1,v0,404f30 <subroutine_fnc+0x20> # v1 != v0 ? 404f30 : next inst.
  404fe4:	00000000 	nop
  404fe8:	10000002 	b	404ff4 <subroutine_fnc+0xe4> # go to 404fd4
  404fec:	00000000 	nop
  404ff0:	00000000 	nop
  404ff4:	8fc20008 	lw	v0,8(s8) # v0 = Mem[s8 + 8] -> v0 = *s8 + 8
  404ff8:	00000000 	nop
  404ffc:	04410004 	bgez	v0,405010 <subroutine_fnc+0x100> # v0 >= 0 ? 405010 : next inst.
  405000:	00000000 	nop
  405004:	2402ffff 	li	v0,-1 #v0 = -1
  405008:	10000002 	b	405014 <subroutine_fnc+0x104> # go to 405014
  40500c:	00000000 	nop
  405010:	8fc2000c 	lw	v0,12(s8) # v0 = Mem[s8 + 12] -> v0 = *s8 + 12
  405014:	03c0e825 	move	sp,s8 # sp = s8
  405018:	8fbe0024 	lw	s8,36(sp) # s8 = Mem[sp + 36] -> s8 = *sp + 36
  40501c:	27bd0028 	addiu	sp,sp,40 # 
  405020:	03e00008 	jr	ra
  405024:	00000000 	nop

00405028 <toplevel_fnc>:
  405028:	27bdffd0 	addiu	sp,sp,-48
  40502c:	afbf002c 	sw	ra,44(sp)
  405030:	afbe0028 	sw	s8,40(sp)
  405034:	03a0f025 	move	s8,sp
  405038:	3c020041 	lui	v0,0x41
  40503c:	24426150 	addiu	v0,v0,24912
  405040:	00402025 	move	a0,v0
  405044:	24050302 	li	a1,770
  405048:	24060180 	li	a2,384
  40504c:	24020fa5 	li	v0,4005
  405050:	0000000c 	syscall
  405054:	afc70018 	sw	a3,24(s8)
  405058:	afc2001c 	sw	v0,28(s8)
  40505c:	8fc20018 	lw	v0,24(s8)
  405060:	00000000 	nop
  405064:	14400004 	bnez	v0,405078 <toplevel_fnc+0x50>
  405068:	00000000 	nop
  40506c:	8fc2001c 	lw	v0,28(s8)
  405070:	10000002 	b	40507c <toplevel_fnc+0x54>
  405074:	00000000 	nop
  405078:	2402ffff 	li	v0,-1
  40507c:	afc20020 	sw	v0,32(s8)
  405080:	8fc20020 	lw	v0,32(s8)
  405084:	00000000 	nop
  405088:	04410004 	bgez	v0,40509c <toplevel_fnc+0x74>
  40508c:	00000000 	nop
  405090:	8fc20020 	lw	v0,32(s8)
  405094:	10000007 	b	4050b4 <toplevel_fnc+0x8c>
  405098:	00000000 	nop
  40509c:	8fc50020 	lw	a1,32(s8)
  4050a0:	00002025 	move	a0,zero
  4050a4:	0c1013c4 	jal	404f10 <subroutine_fnc>
  4050a8:	00000000 	nop
  4050ac:	afc20024 	sw	v0,36(s8)
  4050b0:	8fc20024 	lw	v0,36(s8)
  4050b4:	03c0e825 	move	sp,s8
  4050b8:	8fbf002c 	lw	ra,44(sp)
  4050bc:	8fbe0028 	lw	s8,40(sp)
  4050c0:	27bd0030 	addiu	sp,sp,48
  4050c4:	03e00008 	jr	ra
  4050c8:	00000000 	nop

program data


build/program-mips:     file format elf32-tradbigmips

Contents of section my_data:
 416150 64617461 00000000                    data....        
