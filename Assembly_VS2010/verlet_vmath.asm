?Verlet@Cloth@CLOTH_VMATH@@QAEXXZ PROC			; CLOTH_VMATH::Cloth::Verlet, COMDAT
; _this$ = ecx
	movaps	xmm2, XMMWORD PTR __xmm@0
	movaps	xmm3, XMMWORD PTR __xmm@1
	xor	edx, edx
	cmp	DWORD PTR [ecx+490212], edx
	jle	SHORT $LN1@Verlet
	mov	eax, ecx
	npad	6
$LL3@Verlet:
	movaps	xmm0, XMMWORD PTR [eax]
	movaps	xmm4, XMMWORD PTR [eax+67600]
	movaps	xmm1, XMMWORD PTR [ecx+202816]
	movaps	xmm5, xmm0
	mulps	xmm5, xmm2
	mulps	xmm4, xmm3
	subps	xmm5, xmm4
	mulps	xmm1, xmm1
	mulps	xmm1, XMMWORD PTR [eax+135200]
	addps	xmm5, xmm1
	addps	xmm5, xmm0
	movaps	XMMWORD PTR [eax], xmm5
	movaps	XMMWORD PTR [eax+67600], xmm0
	inc	edx
	add	eax, 16					; 00000010H
	cmp	edx, DWORD PTR [ecx+490212]
	jl	SHORT $LL3@Verlet
$LN1@Verlet:
	ret	0
?Verlet@Cloth@CLOTH_VMATH@@QAEXXZ ENDP			; CLOTH_VMATH::Cloth::Verlet
