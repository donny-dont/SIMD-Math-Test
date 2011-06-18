?do_3band@EQ_VMATH@@YA?AT__m128@@PAUEQSTATE@1@AAT2@@Z PROC ; EQ_VMATH::do_3band, COMDAT
	mov	eax, DWORD PTR _es$[esp-4]
	mov	ecx, DWORD PTR _sample$[esp-4]
	movaps	xmm0, XMMWORD PTR [ecx]
	subps	xmm0, XMMWORD PTR [eax+16]
	movaps	xmm1, XMMWORD PTR [eax]
	mulps	xmm0, xmm1
	addps	xmm0, XMMWORD PTR _vsa
	addps	xmm0, XMMWORD PTR [eax+16]
	movaps	XMMWORD PTR [eax+16], xmm0
	subps	xmm0, XMMWORD PTR [eax+32]
	mulps	xmm0, xmm1
	addps	xmm0, XMMWORD PTR [eax+32]
	movaps	xmm2, XMMWORD PTR [eax+64]
	movaps	XMMWORD PTR [eax+32], xmm0
	subps	xmm0, XMMWORD PTR [eax+48]
	mulps	xmm0, xmm1
	addps	xmm0, XMMWORD PTR [eax+48]
	movaps	XMMWORD PTR [eax+48], xmm0
	subps	xmm0, xmm2
	mulps	xmm0, xmm1
	movaps	xmm1, XMMWORD PTR [eax+80]
	addps	xmm2, xmm0
	movaps	xmm3, XMMWORD PTR [eax+128]
	movaps	XMMWORD PTR [eax+64], xmm2
	movaps	xmm0, XMMWORD PTR [ecx]
	subps	xmm0, XMMWORD PTR [eax+96]
	mulps	xmm0, xmm1
	addps	xmm0, XMMWORD PTR _vsa
	addps	xmm0, XMMWORD PTR [eax+96]
	movaps	XMMWORD PTR [eax+96], xmm0
	subps	xmm0, XMMWORD PTR [eax+112]
	mulps	xmm0, xmm1
	addps	xmm0, XMMWORD PTR [eax+112]
	movaps	XMMWORD PTR [eax+112], xmm0
	subps	xmm0, xmm3
	mulps	xmm0, xmm1
	addps	xmm0, xmm3
	movaps	xmm3, XMMWORD PTR [eax+144]
	movaps	XMMWORD PTR [eax+128], xmm0
	subps	xmm0, xmm3
	mulps	xmm0, xmm1
	addps	xmm0, xmm3
	movaps	xmm3, XMMWORD PTR [eax+192]
	movaps	XMMWORD PTR [eax+144], xmm0
	movaps	xmm1, xmm3
	subps	xmm1, xmm0
	movaps	xmm0, XMMWORD PTR [eax+176]
	movaps	XMMWORD PTR [eax+192], xmm0
	movaps	xmm0, XMMWORD PTR [eax+160]
	movaps	XMMWORD PTR [eax+176], xmm0
	movaps	xmm0, XMMWORD PTR [ecx]
	movaps	XMMWORD PTR [eax+160], xmm0
	movaps	xmm4, xmm1
	movaps	xmm0, xmm3
	movaps	xmm3, XMMWORD PTR [eax+240]
	mulps	xmm3, xmm1
	movaps	xmm1, XMMWORD PTR [eax+208]
	addps	xmm4, xmm2
	subps	xmm0, xmm4
	mulps	xmm0, XMMWORD PTR [eax+224]
	addps	xmm0, xmm3
	mulps	xmm1, xmm2
	addps	xmm0, xmm1
	ret	0
?do_3band@EQ_VMATH@@YA?AT__m128@@PAUEQSTATE@1@AAT2@@Z ENDP ; EQ_VMATH::do_3band