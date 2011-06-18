?VSin@VCLASS_TYPEDEF@@YA?AV?$vector4@MT__m128@@@1@ABV21@@Z PROC ; VCLASS_TYPEDEF::VSin, COMDAT
	push	ebp
	mov	ebp, esp
	and	esp, -16				; fffffff0H
	sub	esp, 16					; 00000010H
	mov	eax, DWORD PTR _x$[ebp]
	movaps	xmm0, XMMWORD PTR [eax]
	movaps	xmm1, XMMWORD PTR __xmm@0
	movaps	xmm2, XMMWORD PTR __xmm@1
	movaps	xmm3, XMMWORD PTR __xmm@2
	movaps	xmm4, XMMWORD PTR __xmm@3
	movaps	xmm5, XMMWORD PTR __xmm@4
	movaps	xmm6, XMMWORD PTR __xmm@5
	movaps	xmm7, XMMWORD PTR __xmm@6
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	mulps	xmm1, xmm0
	mulps	xmm1, xmm0
	mulps	xmm2, xmm0
	mulps	xmm1, xmm0
	mulps	xmm2, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm2, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm2, xmm0
	mulps	xmm4, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm2, xmm0
	mulps	xmm4, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm5, xmm0
	mulps	xmm2, xmm0
	mulps	xmm4, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm5, xmm0
	mulps	xmm2, xmm0
	mulps	xmm4, xmm0
	mulps	xmm6, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm5, xmm0
	movaps	XMMWORD PTR tv802[esp+16], xmm0
	mulps	xmm2, xmm0
	mulps	xmm4, xmm0
	mulps	xmm6, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm5, xmm0
	mulps	xmm7, xmm0
	mulps	xmm7, XMMWORD PTR tv802[esp+16]
	mulps	xmm7, XMMWORD PTR tv802[esp+16]
	addps	xmm7, XMMWORD PTR tv802[esp+16]
	mulps	xmm2, xmm0
	mulps	xmm4, xmm0
	mulps	xmm6, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm5, xmm0
	mulps	xmm2, xmm0
	mulps	xmm4, xmm0
	mulps	xmm6, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm5, xmm0
	mulps	xmm2, xmm0
	mulps	xmm4, xmm0
	mulps	xmm6, xmm0
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	mulps	xmm5, xmm0
	addps	xmm6, xmm7
	mulps	xmm2, xmm0
	mulps	xmm4, xmm0
	addps	xmm5, xmm6
	mulps	xmm1, xmm0
	mulps	xmm3, xmm0
	addps	xmm4, xmm5
	mulps	xmm2, xmm0
	addps	xmm3, xmm4
	mulps	xmm1, xmm0
	addps	xmm2, xmm3
	addps	xmm1, xmm2
	movaps	XMMWORD PTR [eax], xmm1
	mov	esp, ebp
	pop	ebp
	ret	0
?VSin@VCLASS_TYPEDEF@@YA?AV?$vector4@MT__m128@@@1@ABV21@@Z ENDP ; VCLASS_TYPEDEF::VSin
