//--------------------------------------------------------------------------------------
// File: testsine.h
//--------------------------------------------------------------------------------------

#include "DXUT.h"
#include <xmmintrin.h>
#include <emmintrin.h>
#include <math.h>
#include "vmath.h"
#include "vclass.h"

extern void DBugVec(WCHAR *str, float* p);

#define	VMATH_PI				(3.1415926535897932384626433832795f)
#define	VMATH_DTOR(angle)		((VMATH_PI/180.0f)*((float)angle))
#define	VMATH_RTOD(angle)		((180.0f/VMATH_PI)*((float)angle))

////////////////////////////////////////////////////////////////////////////////
//sin(x) =	- x^15/1307674368000 + x^13/6227020800 - x^11/39916800 + x^9/362880
//			- x^7/5040 + x^5/120 - x^3/6 + x
////////////////////////////////////////////////////////////////////////////////
namespace VMATH
{
	Vec4 VSin(const Vec4& x)
	{
		Vec4 c1 = VReplicate(-1.f/6.f);
		Vec4 c2 = VReplicate(1.f/120.f);
		Vec4 c3 = VReplicate(-1.f/5040.f);
		Vec4 c4 = VReplicate(1.f/362880);
		Vec4 c5 = VReplicate(-1.f/39916800);
		Vec4 c6 = VReplicate(1.f/6227020800);
		Vec4 c7 = VReplicate(-1.f/1307674368000);

		Vec4 res =	x + 
					c1*x*x*x + 
					c2*x*x*x*x*x + 
					c3*x*x*x*x*x*x*x + 
					c4*x*x*x*x*x*x*x*x*x + 
					c5*x*x*x*x*x*x*x*x*x*x*x + 
					c6*x*x*x*x*x*x*x*x*x*x*x*x*x + 
					c7*x*x*x*x*x*x*x*x*x*x*x*x*x*x*x;

		return (res);
	}

Vec4 VSin2(const Vec4& x)
{
	Vec4 c1 = VReplicate(-1.f/6.f);
	Vec4 c2 = VReplicate(1.f/120.f);
	Vec4 c3 = VReplicate(-1.f/5040.f);
	Vec4 c4 = VReplicate(1.f/362880);
	Vec4 c5 = VReplicate(-1.f/39916800);
	Vec4 c6 = VReplicate(1.f/6227020800);
	Vec4 c7 = VReplicate(-1.f/1307674368000);

	Vec4 tmp0 = x;
	Vec4 x3 = x*x*x;
	Vec4 tmp1 = c1*x3;
	Vec4 res = tmp0 + tmp1;
	
	Vec4 x5 = x3*x*x;
	tmp0 = c2*x5;
	res	= res + tmp0;

	Vec4 x7 = x5*x*x;
	tmp0 = c3*x7;
	res	= res + tmp0;

	Vec4 x9 = x7*x*x;
	tmp0 = c4*x9;
	res	= res + tmp0;

	Vec4 x11 = x9*x*x;
	tmp0 = c5*x11;
	res	= res + tmp0;

	Vec4 x13 = x11*x*x;
	tmp0 = c6*x13;
	res	= res + tmp0;

	Vec4 x15 = x13*x*x;
	tmp0 = c7*x15;
	res	= res + tmp0;

	return (res);
}


#if 0
	Vec4 VSinProcedural(const Vec4& x)
	{
		Vec4 c1 = VReplicate(-1.f/6.f);
		Vec4 c2 = VReplicate(1.f/120.f);
		Vec4 c3 = VReplicate(-1.f/5040.f);
		Vec4 c4 = VReplicate(1.f/362880);
		Vec4 c5 = VReplicate(-1.f/39916800);
		Vec4 c6 = VReplicate(1.f/6227020800);
		Vec4 c7 = VReplicate(-1.f/1307674368000);

		Vec4 tmp0 = x;
		Vec4 x3 = VMul(VMul(x, x), x);
		Vec4 tmp1 = VMul(c1, x3);
		Vec4 res = VAdd(tmp0, tmp1);
		
		Vec4 x5 = VMul(VMul(x3, x), x);
		tmp0 = VMul(c2, x5);
		res = VAdd(res, tmp0);

		Vec4 x7 = VMul(VMul(x5, x), x);
		tmp0 = VMul(c3, x7);
		res	= VAdd(res, tmp0);

		Vec4 x9 = VMul(VMul(x7, x), x);
		tmp0 = VMul(c4, x9);
		res	= VAdd(res, tmp0);

		Vec4 x11 = VMul(VMul(x9, x), x);
		tmp0 = VMul(c5, x11);
		res	= VAdd(res, tmp0);

		Vec4 x13 = VMul(VMul(x11, x), x);
		tmp0 = VMul(c6, x13);
		res	= VAdd(res, tmp0);

		Vec4 x15 = VMul(VMul(x13, x), x);
		tmp0 = VMul(c7, x15);
		res	= VAdd(res, tmp0);

		return (res);
	}
#endif

Vec4 DotPlatformSpecific(Vec4 va, Vec4 vb)
{
	return (Dot(va, vb));
}

}

namespace VCLASS
{
	Vec4 VSin(const Vec4& x)
	{
		Vec4 c1 = Vec4(-1.f/6.f);
		Vec4 c2 = Vec4(1.f/120.f);
		Vec4 c3 = Vec4(-1.f/5040.f);
		Vec4 c4 = Vec4(1.f/362880);
		Vec4 c5 = Vec4(-1.f/39916800);
		Vec4 c6 = Vec4(1.f/6227020800);
		Vec4 c7 = Vec4(-1.f/1307674368000);

		Vec4 res =	x + 
					c1*x*x*x + 
					c2*x*x*x*x*x + 
					c3*x*x*x*x*x*x*x + 
					c4*x*x*x*x*x*x*x*x*x + 
					c5*x*x*x*x*x*x*x*x*x*x*x + 
					c6*x*x*x*x*x*x*x*x*x*x*x*x*x + 
					c7*x*x*x*x*x*x*x*x*x*x*x*x*x*x*x;

		return (res);
	}
}

void TestSine(void)
{
	for(float a=-180.f; a<=180.f; a+=30.f)
	{
		{
			using namespace VMATH;

			Vec4 sina = VSin(VReplicate(VMATH_DTOR(a)));
			DBugVec(L"Sin=", (float*)&sina);

			sina  = VSin2(VReplicate(VMATH_DTOR(a)));
			DBugVec(L"SinOL=", (float*)&sina);

			//sina  = VSinProcedural(VReplicate(VMATH_DTOR(a)));
			//DBugVec(L"SinPR=", (float*)&sina);

			printf("");

		}

		{
			using namespace VCLASS;

			Vec4 sina = VSin(Vec4(VMATH_DTOR(a)));

			DBugVec(L"Sin=", (float*)&sina);
		}

	}

}
