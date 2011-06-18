?do_3band@EQ_VCLASS_SIMDTYPE@@YA?AV?$vector4@MVsimd_type@VCLASS_SIMDTYPE@@@VCLASS_SIMDTYPE@@PAUEQSTATE@1@AAV23@@Z PROC ; EQ_VCLASS_SIMDTYPE::do_3band, COMDAT
	mov	eax, DWORD PTR _es$[esp-4]
	mov	ecx, DWORD PTR _sample$[esp-4]
	movaps	xmm0, XMMWORD PTR [ecx]
	subps	xmm0, XMMWORD PTR [eax+16]
	mulps	xmm0, XMMWORD PTR [eax]
	addps	xmm0, XMMWORD PTR _vsa
	addps	xmm0, XMMWORD PTR [eax+16]
	movaps	XMMWORD PTR [eax+16], xmm0
	movaps	xmm1, XMMWORD PTR [eax+16]
	subps	xmm1, XMMWORD PTR [eax+32]
	mulps	xmm1, XMMWORD PTR [eax]
	addps	xmm1, XMMWORD PTR [eax+32]
	movaps	XMMWORD PTR [eax+32], xmm1
	movaps	xmm0, XMMWORD PTR [eax+32]
	subps	xmm0, XMMWORD PTR [eax+48]
	mulps	xmm0, XMMWORD PTR [eax]
	addps	xmm0, XMMWORD PTR [eax+48]
	movaps	XMMWORD PTR [eax+48], xmm0
	movaps	xmm0, XMMWORD PTR [eax+48]
	subps	xmm0, XMMWORD PTR [eax+64]
	mulps	xmm0, XMMWORD PTR [eax]
	addps	xmm0, XMMWORD PTR [eax+64]
	movaps	XMMWORD PTR [eax+64], xmm0
	movaps	xmm1, XMMWORD PTR [ecx]
	subps	xmm1, XMMWORD PTR [eax+96]
	mulps	xmm1, XMMWORD PTR [eax+80]
	addps	xmm1, XMMWORD PTR _vsa
	addps	xmm1, XMMWORD PTR [eax+96]
	movaps	xmm4, XMMWORD PTR [eax+64]
	movaps	XMMWORD PTR [eax+96], xmm1
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
	movaps	xmm3, XMMWORD PTR [eax+208]
	movaps	xmm1, xmm0
	subps	xmm1, XMMWORD PTR [eax+144]
	movaps	xmm2, xmm1
	addps	xmm2, xmm4
	subps	xmm0, xmm2
	movaps	xmm2, XMMWORD PTR [eax+240]
	mulps	xmm0, XMMWORD PTR [eax+224]
	mulps	xmm2, xmm1
	movaps	xmm1, XMMWORD PTR [eax+176]
	movaps	XMMWORD PTR [eax+192], xmm1
	movaps	xmm1, XMMWORD PTR [eax+160]
	movaps	XMMWORD PTR [eax+176], xmm1
	movaps	xmm1, XMMWORD PTR [ecx]
	mulps	xmm3, xmm4
	movaps	XMMWORD PTR [eax+160], xmm1
	mov	eax, DWORD PTR ___$ReturnUdt$[esp-4]
	addps	xmm0, xmm3
	addps	xmm0, xmm2
	movaps	XMMWORD PTR [eax], xmm0
	ret	0
?do_3band@EQ_VCLASS_SIMDTYPE@@YA?AV?$vector4@MVsimd_type@VCLASS_SIMDTYPE@@@VCLASS_SIMDTYPE@@PAUEQSTATE@1@AAV23@@Z ENDP ; EQ_VCLASS_SIMDTYPE::do_3band
