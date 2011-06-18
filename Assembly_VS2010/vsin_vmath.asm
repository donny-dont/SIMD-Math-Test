?VSin@VMATH@@YA?AT__m128@@ABT2@@Z PROC			; VMATH::VSin, COMDAT
	push	ebp
	mov	ebp, esp
	and	esp, -16				; fffffff0H
	sub	esp, 16					; 00000010H
	mov	eax, DWORD PTR _x$[ebp]
	movaps	xmm1, XMMWORD PTR [eax]
	movaps	xmm0, XMMWORD PTR __xmm@0
	movaps	xmm2, XMMWORD PTR __xmm@1
	movaps	xmm3, XMMWORD PTR __xmm@2
	movaps	xmm4, XMMWORD PTR __xmm@3
	movaps	xmm5, XMMWORD PTR __xmm@4
	movaps	xmm6, XMMWORD PTR __xmm@5
	movaps	xmm7, XMMWORD PTR __xmm@6
	mulps	xmm0, xmm1
	mulps	xmm0, xmm1
	mulps	xmm2, xmm1
	mulps	xmm0, xmm1
	mulps	xmm2, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm2, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm2, xmm1
	mulps	xmm4, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm2, xmm1
	mulps	xmm4, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm5, xmm1
	mulps	xmm2, xmm1
	mulps	xmm4, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm5, xmm1
	mulps	xmm2, xmm1
	mulps	xmm4, xmm1
	mulps	xmm6, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm5, xmm1
	movaps	XMMWORD PTR tv770[esp+16], xmm1
	mulps	xmm2, xmm1
	mulps	xmm4, xmm1
	mulps	xmm6, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm5, xmm1
	mulps	xmm7, xmm1
	mulps	xmm7, XMMWORD PTR tv770[esp+16]
	mulps	xmm7, XMMWORD PTR tv770[esp+16]
	addps	xmm7, XMMWORD PTR tv770[esp+16]
	mulps	xmm2, xmm1
	mulps	xmm4, xmm1
	mulps	xmm6, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm5, xmm1
	mulps	xmm2, xmm1
	mulps	xmm4, xmm1
	mulps	xmm6, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm5, xmm1
	mulps	xmm2, xmm1
	mulps	xmm4, xmm1
	mulps	xmm6, xmm1
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	mulps	xmm5, xmm1
	addps	xmm6, xmm7
	mulps	xmm2, xmm1
	mulps	xmm4, xmm1
	addps	xmm5, xmm6
	mulps	xmm0, xmm1
	mulps	xmm3, xmm1
	addps	xmm4, xmm5
	mulps	xmm2, xmm1
	addps	xmm3, xmm4
	mulps	xmm0, xmm1
	addps	xmm2, xmm3
	addps	xmm0, xmm2
	mov	esp, ebp
	pop	ebp
	ret	0
?VSin@VMATH@@YA?AT__m128@@ABT2@@Z ENDP			; VMATH::VSin
