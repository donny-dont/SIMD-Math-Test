#include "DXUT.h"
#include <xmmintrin.h>
#include <emmintrin.h>
#include <math.h>

extern void DBugVec(WCHAR *str, float* p);
#define XNAMATH

#ifdef XNAMATH
#include "_xnamath_.h"
XMVECTOR TestDotXNA(XMVECTOR n, XMVECTOR i)
{
	XMVECTOR r;

	r = XMVector4Dot(XMVector4Reflect(i, n), XMVector4Reflect(n, i));
	r = XMVector4Dot(r, XMVector4Dot(n, i));

	return(r);
}

void TestDot(void)
{
	XMVECTOR n = {1.f, 2.f, 1.f, 3.f};
	XMVECTOR i = {0.1f, -3.f, 2.f, -2.f};
	XMVECTOR r;

	r = TestDotXNA(n, i);
	float*	f = (float*)&r;
	DBugVec(L"XNAMath", f);
}
#else
#include "vmath.h"
using namespace VMATH;
Vec4 TestDotVMath(Vec4 n, Vec4 i)
{
	Vec4 r;

	r = Dot(Reflect(i, n), Reflect(n, i));
	r = Dot(r, Dot(n, i));

	return(r);
}

void TestDot(void)
{
	Vec4 n = {1.f, 2.f, 1.f, 3.f};
	Vec4 i = {0.1f, -3.f, 2.f, -2.f};
	Vec4 r;

	r = TestDotVMath(n, i);
	float*	f = (float*)&r;
	DBugVec(L"VMath", f);
}
#endif