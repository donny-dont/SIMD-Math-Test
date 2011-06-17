//--------------------------------------------------------------------------------------
// File: vmath.h
//--------------------------------------------------------------------------------------


#ifndef __VMATH__
#define __VMATH__

///////////////////////////////////////////////////////////////////////////////
//	This example show the fastest SIMD design for vector calculations
//	1 - Not structured data, instead use atomic datatypes
//	2 - All inline
//	3 - No overloaded operators
//	4 - Data passed and returned by value
//	5 - For single 32-bit results (such as dot product), repeated results
//	stored in all 4 slots. Allowing math expression optimizations
///////////////////////////////////////////////////////////////////////////////

namespace VMATH
{

	typedef __m128	Vec4;

	///////////////////////////////////////////
	//BEGIN:hacked from XNAMath to show extra load
	///////////////////////////////////////////
	typedef __declspec(align(16)) struct Vec4F32 {
		union {
			float f[4];
			Vec4 v;
		};

	#if defined(__cplusplus)
		inline operator Vec4() const { return v; }
	#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
		inline operator __m128i() const { return reinterpret_cast<const __m128i *>(&v)[0]; }
		inline operator __m128d() const { return reinterpret_cast<const __m128d *>(&v)[0]; }
	#endif
	#endif // __cplusplus
	} Vec4F32;

	const Vec4F32 g_XMOne = { 1.0f, 1.0f, 1.0f, 1.0f};

	inline Vec4 VReciprocal(Vec4 va)
	{
		return _mm_div_ps(g_XMOne,va);
	}
	///////////////////////////////////////////
	//END:hacked from XNAMath to show extra load
	///////////////////////////////////////////

	inline Vec4	VLoad(float *pVec)
	{
		return(_mm_load_ps(pVec));
	};

	inline Vec4 VLoad(float f)
	{
		return(_mm_set_ps(f, f, f, f));
	};

	inline Vec4 VReplicate(float f)
	{
		return _mm_set_ps1(f);
	}

	inline Vec4 VLoad(float x, float y, float z, float w)
	{
		return(_mm_set_ps(x, y, z, w));
	}

	inline Vec4 VAdd(Vec4 va, Vec4 vb)
	{
		return(_mm_add_ps(va, vb));
	};

	inline void VAddSlow(Vec4& vr, Vec4 va, Vec4 vb)
	{
		vr = _mm_add_ps(va, vb);
	};

	inline Vec4 VSub(Vec4 va, Vec4 vb)
	{
		return(_mm_sub_ps(va, vb));
	};

	inline Vec4 VMul(Vec4 va, Vec4 vb)
	{
		return(_mm_mul_ps(va, vb));
	};

	inline Vec4 VDiv(Vec4 va, Vec4 vb)
	{
		return(_mm_div_ps(va, vb));
	};

	//slower div from XNAMath using extra load
	inline Vec4 VDivSlow(Vec4 va, Vec4 vb)
	{
		Vec4 InvV = VReciprocal(vb);
	    return VMul(va, InvV);
	}

	inline void VStore(float *pVec, Vec4 v)
	{
		_mm_store_ps(pVec, v);
	};

	inline Vec4 VBc(Vec4 v)
	{
		return(_mm_shuffle_ps(v, v, _MM_SHUFFLE(3,3,3,3)));
	}

	//////////////////////////////////////////////////////////////////////////////
	// Fast SSE2 4-dimensional dot product by Gustavo M. Oliveira.
	// Feel free to use for commercial or non-commercial applications,
	// as long as this header is mantained.
	//
	// For more information on how to write and use high-performance
	// SIMD Libraties, visit:
	// http://www.guitarv.com/ComputerScience.aspx?page=articles
	//
	// The author assumes NO RESPONSIBILITY and/or LIABILITY  for any problems
	// caused by the use of this software. Use it at your own risk.
	//////////////////////////////////////////////////////////////////////////////
	inline Vec4 Dot(Vec4 va, Vec4 vb)
	{
		Vec4 t0 = _mm_mul_ps(va, vb);
		Vec4 t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1,0,3,2));
		Vec4 t2 = _mm_add_ps(t0, t1);
		Vec4 t3 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(2,3,0,1));
		Vec4 dot = _mm_add_ps(t3, t2);
		return (dot);
	}

	inline Vec4 Sqrt(Vec4 v)
	{
		return(_mm_sqrt_ps(v));
	}

	inline void GetX(float *p, Vec4 v)
	{
		_mm_store_ss(p, v);
	}

	inline Vec4 Reflect(Vec4 Incident, Vec4 Normal)
	{
		// Result = Incident - (2 * dot(Incident, Normal)) * Normal
		Vec4 Result = Dot(Incident,Normal);
		Result = _mm_add_ps(Result,Result);
		Result = _mm_mul_ps(Result,Normal);
		Result = _mm_sub_ps(Incident,Result);
		return Result;
	}

	////////////////////////////////////////////////////////////////////////////////
	//	Overloaded operators, left here just as a reference.
	//	WARNING: This bloats the code as expressions grow
	////////////////////////////////////////////////////////////////////////////////
	inline Vec4 operator+(Vec4 va, Vec4 vb)
	{
		return(VAdd(va, vb));
	}

	inline Vec4& operator+= (Vec4& va, Vec4 vb)
	{
		va = VAdd(va, vb);
		return (va);
	}

	inline Vec4 operator-(Vec4 va, Vec4 vb)
	{
		return(VSub(va, vb));
	}

	inline Vec4& operator-= (Vec4& va, Vec4 vb)
	{
		va = VSub(va, vb);
		return (va);
	}

	inline Vec4 operator*(Vec4 va, Vec4 vb)
	{
		return(VMul(va, vb));
	}

	inline Vec4& operator*= (Vec4& va, Vec4 vb)
	{
		va = VMul(va, vb);
		return (va);
	}

	inline Vec4 operator/(Vec4 va, Vec4 vb)
	{
		return(VDiv(va, vb));
	}

	inline Vec4& operator/= (Vec4& va, Vec4 vb)
	{
		va = VDiv(va, vb);
		return (va);
	}
}

#endif // #ifndef __VMATH__