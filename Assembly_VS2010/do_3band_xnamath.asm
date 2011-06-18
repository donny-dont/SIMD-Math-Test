?do_3band@EQ_XNAMATH@@YA?AT__m128@@PAUEQSTATE@1@AAT2@@Z PROC ; EQ_XNAMATH::do_3band, COMDAT
	mov	eax, DWORD PTR _es$[esp-4]
	mov	ecx, DWORD PTR _sample$[esp-4]
	movaps	xmm1, XMMWORD PTR [ecx]
	subps	xmm1, XMMWORD PTR [eax+16]
	mulps	xmm1, XMMWORD PTR [eax]
	addps	xmm1, XMMWORD PTR _vsa
	addps	xmm1, XMMWORD PTR [eax+16]
	movaps	XMMWORD PTR [eax+16], xmm1
	movaps	xmm2, XMMWORD PTR [eax+16]
	subps	xmm2, XMMWORD PTR [eax+32]
	mulps	xmm2, XMMWORD PTR [eax]
	addps	xmm2, XMMWORD PTR [eax+32]
	movaps	XMMWORD PTR [eax+32], xmm2
	movaps	xmm1, XMMWORD PTR [eax+32]
	subps	xmm1, XMMWORD PTR [eax+48]
	mulps	xmm1, XMMWORD PTR [eax]
	addps	xmm1, XMMWORD PTR [eax+48]
	movaps	XMMWORD PTR [eax+48], xmm1
	movaps	xmm1, XMMWORD PTR [eax+48]
	subps	xmm1, XMMWORD PTR [eax+64]
	mulps	xmm1, XMMWORD PTR [eax]
	addps	xmm1, XMMWORD PTR [eax+64]
	movaps	XMMWORD PTR [eax+64], xmm1
	movaps	xmm3, XMMWORD PTR [ecx]
	subps	xmm3, XMMWORD PTR [eax+96]
	mulps	xmm3, XMMWORD PTR [eax+80]
	movaps	xmm2, XMMWORD PTR [eax+64]
	addps	xmm3, XMMWORD PTR _vsa
	addps	xmm3, XMMWORD PTR [eax+96]
	movaps	XMMWORD PTR [eax+96], xmm3
	movaps	xmm0, XMMWORD PTR [eax+96]
	subps	xmm0, XMMWORD PTR [eax+112]
	mulps	xmm0, XMMWORD PTR [eax+80]
	addps	xmm0, XMMWORD PTR [eax+112]
	movaps	XMMWORD PTR [eax+112], xmm0
	movaps	xmm0, XMMWORD PTR [eax+112]
	subps	xmm0, XMMWORD PTR [eax+128]
	mulps	xmm0, XMMWORD PTR [eax+80]
	addps	xmm0, XMMWORD PTR [eax+128]
	movaps	XMMWORD PTR [eax+128], xmm0
	movaps	xmm0, XMMWORD PTR [eax+128]
	subps	xmm0, XMMWORD PTR [eax+144]
	mulps	xmm0, XMMWORD PTR [eax+80]
	addps	xmm0, XMMWORD PTR [eax+144]
	movaps	XMMWORD PTR [eax+144], xmm0
	movaps	xmm0, XMMWORD PTR [eax+192]
	movaps	xmm1, xmm0
	subps	xmm1, XMMWORD PTR [eax+144]
	movaps	xmm3, xmm1
	addps	xmm3, xmm2
	subps	xmm0, xmm3
	movaps	xmm3, xmm0
	movaps	xmm0, XMMWORD PTR [eax+176]
	movaps	XMMWORD PTR [eax+192], xmm0
	movaps	xmm0, XMMWORD PTR [eax+160]
	movaps	XMMWORD PTR [eax+176], xmm0
	movaps	xmm0, XMMWORD PTR [ecx]
	movaps	XMMWORD PTR [eax+160], xmm0
	movaps	xmm0, XMMWORD PTR [eax+224]
	mulps	xmm0, xmm3
	movaps	xmm3, XMMWORD PTR [eax+208]
	mulps	xmm3, xmm2
	movaps	xmm2, XMMWORD PTR [eax+240]
	addps	xmm0, xmm3
	mulps	xmm2, xmm1
	addps	xmm0, xmm2
	ret	0
?do_3band@EQ_XNAMATH@@YA?AT__m128@@PAUEQSTATE@1@AAT2@@Z ENDP ; EQ_XNAMATH::do_3band
