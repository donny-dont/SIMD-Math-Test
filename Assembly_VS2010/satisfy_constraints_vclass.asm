?SatisfyConstraints@Cloth@CLOTH_VCLASS@@QAEXXZ PROC	; CLOTH_VCLASS::Cloth::SatisfyConstraints, COMDAT
; _this$ = ecx
	sub	esp, 8
	cmp	DWORD PTR [ecx+490208], 0
	movaps	xmm4, XMMWORD PTR __xmm@2
	mov	DWORD PTR _j$114065[esp+8], 0
	jle	$LN7@SatisfyCon
	push	ebx
	push	esi
	push	edi
$LL65@SatisfyCon:
	movaps	xmm0, XMMWORD PTR [ecx+202848]
	movaps	XMMWORD PTR [ecx], xmm0
	movaps	xmm0, XMMWORD PTR [ecx+202864]
	movaps	XMMWORD PTR [ecx+1024], xmm0
	mov	eax, DWORD PTR [ecx+490212]
	dec	eax
	mov	DWORD PTR _i$114069[esp+20], 0
	test	eax, eax
	jle	$LN8@SatisfyCon
	mov	edx, ecx
	lea	ebx, DWORD PTR [ecx+202972]
	npad	7
$LL64@SatisfyCon:
	xor	esi, esi
	cmp	DWORD PTR [ebx], esi
	jle	SHORT $LN5@SatisfyCon
	lea	edi, DWORD PTR [ebx-64]
	npad	7
$LL3@SatisfyCon:
	mov	eax, DWORD PTR [edi]
	movaps	xmm3, XMMWORD PTR [edx]
	add	eax, eax
	movaps	xmm0, XMMWORD PTR [ecx+eax*8]
	subps	xmm0, xmm3
	movaps	xmm2, xmm0
	mulps	xmm2, xmm0
	movaps	xmm1, xmm2
	shufps	xmm1, xmm2, 78				; 0000004eH
	addps	xmm1, xmm2
	movaps	xmm2, xmm1
	shufps	xmm2, xmm1, 177				; 000000b1H
	addps	xmm2, xmm1
	sqrtps	xmm2, xmm2
	movaps	xmm1, xmm2
	subps	xmm1, XMMWORD PTR [ecx+202832]
	divps	xmm1, xmm2
	mulps	xmm0, xmm4
	mulps	xmm0, xmm1
	addps	xmm3, xmm0
	movaps	XMMWORD PTR [edx], xmm3
	movaps	xmm1, XMMWORD PTR [ecx+eax*8]
	subps	xmm1, xmm0
	inc	esi
	movaps	XMMWORD PTR [ecx+eax*8], xmm1
	add	edi, 4
	cmp	esi, DWORD PTR [ebx]
	jl	SHORT $LL3@SatisfyCon
$LN5@SatisfyCon:
	mov	eax, DWORD PTR _i$114069[esp+20]
	mov	esi, DWORD PTR [ecx+490212]
	inc	eax
	dec	esi
	add	edx, 16					; 00000010H
	add	ebx, 68					; 00000044H
	mov	DWORD PTR _i$114069[esp+20], eax
	cmp	eax, esi
	jl	$LL64@SatisfyCon
$LN8@SatisfyCon:
	mov	eax, DWORD PTR _j$114065[esp+20]
	inc	eax
	mov	DWORD PTR _j$114065[esp+20], eax
	cmp	eax, DWORD PTR [ecx+490208]
	jl	$LL65@SatisfyCon
	pop	edi
	pop	esi
	pop	ebx
$LN7@SatisfyCon:
	add	esp, 8
	ret	0
?SatisfyConstraints@Cloth@CLOTH_VCLASS@@QAEXXZ ENDP	; CLOTH_VCLASS::Cloth::SatisfyConstraints
