#ifndef __VCLASS_TYPEDEF__
#define __VCLASS_TYPEDEF__

namespace VCLASS_TYPEDEF
{
	///////////////////////////////////////////
	// SIMD TYPEDEF
	///////////////////////////////////////////

	typedef __m128 simd_type;
	typedef const __m128& simd_param;

	inline simd_type VLoad(float *pVec)
	{
		return _mm_load_ps(pVec);
	}

	inline simd_type VLoad(float f)
	{
		return _mm_set_ps1(f);
	}

	inline simd_type VLoad(float x, float y, float z, float w)
	{
		return _mm_set_ps(x, y, z, w);
	}

	inline simd_type VBAdd(simd_param va, simd_param vb)
	{
		return _mm_add_ps(va, vb);
	}

	inline simd_type VBSub(simd_param va, simd_param vb)
	{
		return _mm_sub_ps(va, vb);
	}

	inline simd_type VBMul(simd_param va, simd_param vb)
	{
		return _mm_mul_ps(va, vb);
	}

	inline simd_type VBDiv(simd_param va, simd_param vb)
	{
		return _mm_div_ps(va, vb);
	}

	inline void VBStore(float *pVec, simd_param v)
	{
		return _mm_store_ps(pVec, v);
	}

	inline simd_type VBc(simd_param v)
	{
		return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3,3,3,3));
	}

	inline simd_type VBDot(simd_param va, simd_param vb)
	{
		simd_type t0 = _mm_mul_ps(va, vb);
		simd_type t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1,0,3,2));
		simd_type t2 = _mm_add_ps(t0, t1);
		simd_type t3 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(2,3,0,1));

		return _mm_add_ps(t3, t2);
	}

	inline simd_type VBSqrt(simd_param v)
	{
		return _mm_sqrt_ps(v);
	}

	inline void VBGetX(float *p, simd_param v)
	{
		_mm_store_ss(p, v);
	}

	inline simd_type VBReflect(simd_param Incident, simd_param Normal)
	{
		// Result = Incident - (2 * dot(Incident, Normal)) * Normal
		simd_type Result = VBDot(Incident,Normal);
		Result = _mm_add_ps(Result,Result);
		Result = _mm_mul_ps(Result,Normal);
		return _mm_sub_ps(Incident,Result);
	}

	///////////////////////////////////////////
	// Vec4
	///////////////////////////////////////////

	template <typename Real, typename Rep>
	class vector4
	{
		public:
			inline vector4() { }

			inline vector4(Real *pVec)
				: _rep(VLoad(pVec))
			{ }

			inline vector4(Real f)
				: _rep(VLoad(f))
			{ }

			inline vector4(Real x, Real y, Real z, Real w)
				: _rep(VLoad(x, y, z, w))
			{ }

			inline vector4(simd_type rep)
				: _rep(rep)
			{ }

			inline vector4(const vector4& copy)
				: _rep(copy._rep)
			{ }

			inline vector4& operator= (const vector4& copy)
			{
				_rep = copy._rep;

				return *this;
			}

			inline vector4& operator+= (const vector4& rhs)
			{
				_rep = VBAdd(_rep, rhs._rep);

				return *this;
			}

			inline vector4& operator-= (const vector4& rhs)
			{
				_rep = VBSub(_rep, rhs._rep);

				return *this;
			}

			inline vector4& operator*= (const vector4& rhs)
			{
				_rep = VBMul(_rep, rhs._rep);

				return *this;
			}

			inline vector4 operator+ (const vector4& rhs) const
			{
				return vector4(VBAdd(_rep, rhs._rep));
			}

			inline vector4 operator- (const vector4& rhs) const
			{
				return vector4(VBSub(_rep, rhs._rep));
			}

			inline vector4 operator* (const vector4& rhs) const
			{
				return vector4(VBMul(_rep, rhs._rep));
			}

			inline vector4 operator/ (const vector4& rhs) const
			{
				return vector4(VBDiv(_rep, rhs._rep));
			}

			inline void Store(Real *pVec) const
			{
				VBStore(pVec, _rep);
			}

			inline void Bc()
			{
				VBc(_rep);
			}

			static inline vector4 Dot(const vector4& va, const vector4& vb)
			{
				return vector4(VBDot(va._rep, vb._rep));
			}

			static inline vector4 Sqrt(const vector4& va)
			{
				return vector4(VBSqrt(va._rep));
			}

			static inline vector4 VAdd(const vector4& va, const vector4& vb)
			{
				return vector4(VBAdd(va._rep, vb._rep));
			}

			static inline vector4 VSub(const vector4& va, const vector4& vb)
			{
				return vector4(VBSub(va._rep, vb._rep));
			}

			static inline vector4 VMul(const vector4& va, const vector4& vb)
			{
				return vector4(VBMul(va._rep, vb._rep));
			}

			static inline void GetX(Real *p, const vector4& v)
			{
				VBGetX(p, v._rep);
			}

		private:
			Rep _rep;
	} ;

	typedef vector4<float, simd_type> Vec4;
}

#endif
