//--------------------------------------------------------------------------------------
// File: vclass.h
//--------------------------------------------------------------------------------------

#ifndef __VCLASS__
#define __VCLASS__

///////////////////////////////////////////////////////////////////////////////
//	This example class shows a sketch of how many SIMD game math libraries are
//	commonly designed, i.e., encapsulating data and functions inside a class.
//	Also it encapsulates overloaded operators to make the design clean and
//	friendly to the programmers.
//
//	Nonethelss this approach causes the **INEVITABLE CODE BLOAT** when
//	performing more complex math expressions.
///////////////////////////////////////////////////////////////////////////////

namespace VCLASS
{
	class Vec4
	{
		public:
			inline Vec4() {};

			inline Vec4(float *pVec)
			{
				xyzw = _mm_load_ps(pVec);
			};

			inline Vec4(float f)
			{
				xyzw = _mm_set_ps1(f);
			};

			inline Vec4(__m128 qword)
			{
				xyzw = qword;
			};

			inline Vec4(float x, float y, float z, float w)
			{
				xyzw = _mm_set_ps(x, y, z, w);
			}

			inline Vec4& operator+=(const Vec4 &rhs)
			{
				xyzw = _mm_add_ps(xyzw, rhs.xyzw);
				return(*this);
			};

			inline Vec4& operator-=(const Vec4 &rhs)
			{
				xyzw = _mm_sub_ps(xyzw, rhs.xyzw);
				return(*this);
			};


			inline Vec4& operator*=(const Vec4 &rhs)
			{
				xyzw = _mm_mul_ps(xyzw, rhs.xyzw);
				return(*this);
			};

			inline const Vec4 operator+(const Vec4 &rhs) const
			{
				Vec4 res(*this);
				res.xyzw = _mm_add_ps(res.xyzw, rhs.xyzw);
				return(res);
			};

			inline const Vec4 operator*(const Vec4 &rhs) const
			{
				Vec4 res(*this);
				res.xyzw = _mm_mul_ps(res.xyzw, rhs.xyzw);
				return(res);
			};

			inline const Vec4 operator-(const Vec4 &rhs) const
			{
				Vec4 res(*this);
				res.xyzw = _mm_sub_ps(res.xyzw, rhs.xyzw);
				return(res);
			};

			inline const Vec4 operator/(const Vec4 &rhs) const
			{
				Vec4 res(*this);
				res.xyzw = _mm_div_ps(res.xyzw, rhs.xyzw);
				return(res);
			};


			inline void Store(float *pVec)
			{
				_mm_store_ps(pVec, xyzw);
			};

			inline void Bc()
			{
				xyzw = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(3,3,3,3));
			}

			static inline Vec4 Dot(Vec4& va, Vec4& vb)
			{
				__m128 t0 = _mm_mul_ps(va.xyzw, vb.xyzw);
				__m128 t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1,0,3,2));
				__m128 t2 = _mm_add_ps(t0, t1);
				__m128 t3 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(2,3,0,1));
				__m128 dot = _mm_add_ps(t3, t2);
				Vec4	res(dot);
				return(res);
			}

			static inline Vec4 Sqrt(Vec4& va)
			{
				__m128 t0 = _mm_sqrt_ps(va.xyzw);
				Vec4 vsqrt(t0);
				return(vsqrt);
			}

#if 0
			static inline Vec4 VAdd(Vec4 va, Vec4 vb)
			{
				return(_mm_add_ps(va.xyzw, vb.xyzw));
			};

			static inline Vec4 VSub(Vec4 va, Vec4 vb)
			{
				return(_mm_sub_ps(va.xyzw, vb.xyzw));
			};

			static inline Vec4 VMul(Vec4 va, Vec4 vb)
			{
				return(_mm_mul_ps(va.xyzw, vb.xyzw));
			};
#endif

			static inline void GetX(float *p, Vec4 v)
			{
				_mm_store_ss(p, v.xyzw);
			}


		private:
			__m128	xyzw;
	};
}

#endif // #ifndef __VCLASS__