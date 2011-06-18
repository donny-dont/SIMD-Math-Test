#ifndef __CLASS_SIMDTYPE__
#define __CLASS_SIMDTYPE__

namespace VCLASS_SIMDTYPE
{
	///////////////////////////////////////////
	// SIMD CLASS (Same as VCLASS)
	///////////////////////////////////////////

	class simd_type
	{
		public:
			inline simd_type() {}

			inline simd_type(float *pVec)
				: xyzw(_mm_load_ps(pVec))
			{ }

			inline simd_type(float f)
				: xyzw(_mm_set_ps1(f))
			{ }

			inline simd_type(const __m128 qword)
				: xyzw(qword)
			{ }

			inline simd_type(float x, float y, float z, float w)
				: xyzw(_mm_set_ps(x, y, z, w))
			{ }

			inline simd_type(const simd_type& copy)
				: xyzw(copy.xyzw)
			{ }

			inline simd_type& operator= (const simd_type& copy)
			{
				xyzw = copy.xyzw;

				return *this;
			}

			inline simd_type& operator+=(const simd_type &rhs)
			{
				xyzw = _mm_add_ps(xyzw, rhs.xyzw);
				return *this;
			}

			inline simd_type& operator-=(const simd_type &rhs)
			{
				xyzw = _mm_sub_ps(xyzw, rhs.xyzw);
				return *this;
			}

			inline simd_type& operator*=(const simd_type &rhs)
			{
				xyzw = _mm_mul_ps(xyzw, rhs.xyzw);
				return *this;
			}

			inline simd_type operator+(const simd_type &rhs) const
			{
				return simd_type(_mm_add_ps(xyzw, rhs.xyzw));
			}

			inline simd_type operator*(const simd_type &rhs) const
			{
				return simd_type(_mm_mul_ps(xyzw, rhs.xyzw));
			}

			inline simd_type operator-(const simd_type &rhs) const
			{
				return simd_type(_mm_sub_ps(xyzw, rhs.xyzw));
			}

			inline simd_type operator/(const simd_type &rhs) const
			{
				return simd_type(_mm_div_ps(xyzw, rhs.xyzw));
			}

			inline void Store(float *pVec) const
			{
				_mm_store_ps(pVec, xyzw);
			}

			inline void Bc()
			{
				xyzw = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(3,3,3,3));
			}

			static inline simd_type Dot(const simd_type& va, const simd_type& vb)
			{
				__m128 t0 = _mm_mul_ps(va.xyzw, vb.xyzw);
				__m128 t1 = _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(1,0,3,2));
				__m128 t2 = _mm_add_ps(t0, t1);
				__m128 t3 = _mm_shuffle_ps(t2, t2, _MM_SHUFFLE(2,3,0,1));
				__m128 dot = _mm_add_ps(t3, t2);

				return simd_type(dot);
			}

			static inline simd_type Sqrt(const simd_type& va)
			{
				return simd_type(_mm_sqrt_ps(va.xyzw));
			}

			static inline simd_type VAdd(const simd_type& va, const simd_type& vb)
			{
				return simd_type(_mm_add_ps(va.xyzw, vb.xyzw));
			}

			static inline simd_type VSub(const simd_type& va, const simd_type& vb)
			{
				return simd_type(_mm_sub_ps(va.xyzw, vb.xyzw));
			}

			static inline simd_type VMul(const simd_type& va, const simd_type& vb)
			{
				return simd_type(_mm_mul_ps(va.xyzw, vb.xyzw));
			}

			static inline void GetX(float *p, const simd_type& v)
			{
				_mm_store_ss(p, v.xyzw);
			}

		private:
			__m128	xyzw;
	};

	///////////////////////////////////////////
	// Vec4
	///////////////////////////////////////////

	template <typename Real, typename Rep>
	class vector4
	{
		public:
			inline vector4() { }

			inline vector4(Real *pVec)
				: _rep(pVec)
			{ }

			inline vector4(Real f)
				: _rep(f)
			{ }

			inline vector4(Real x, Real y, Real z, Real w)
				: _rep(x, y, z, w)
			{ }

			inline vector4(const simd_type& rep)
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
				_rep += rhs._rep;

				return *this;
			}

			inline vector4& operator-= (const vector4& rhs)
			{
				_rep -= rhs._rep;

				return *this;
			}

			inline vector4& operator*= (const vector4& rhs)
			{
				_rep *= rhs._rep;

				return *this;
			}

			inline vector4 operator+ (const vector4& rhs) const
			{
				return vector4(_rep + rhs._rep);
			}

			inline vector4 operator- (const vector4& rhs) const
			{
				return vector4(_rep - rhs._rep);
			}

			inline vector4 operator* (const vector4& rhs) const
			{
				return vector4(_rep * rhs._rep);
			}

			inline vector4 operator/ (const vector4& rhs) const
			{
				return vector4(_rep / rhs._rep);
			}

			inline void Store(Real *pVec) const
			{
				_rep.Store(pVec);
			}

			inline void Bc()
			{
				_rep.Bc();
			}

			static inline vector4 Dot(const vector4& va, const vector4& vb)
			{
				return vector4(simd_type::Dot(va._rep, vb._rep));
			}

			static inline vector4 Sqrt(const vector4& va)
			{
				return vector4(simd_type::Sqrt(va._rep));
			}

			static inline vector4 VAdd(const vector4& va, const vector4& vb)
			{
				return vector4(simd_type::VAdd(va._rep, vb._rep));
			}

			static inline vector4 VSub(const vector4& va, const vector4& vb)
			{
				return vector4(simd_type::VSub(va._rep, vb._rep));
			}

			static inline vector4 VMul(const vector4& va, const vector4& vb)
			{
				return vector4(simd_type::VMul(va._rep, vb._rep));
			}

			static inline void GetX(Real *p, const vector4& v)
			{
				simd_type::GetX(p, v._rep);
			}

		private:
			Rep _rep;
	} ;

	typedef vector4<float, simd_type> Vec4;
}

#endif
