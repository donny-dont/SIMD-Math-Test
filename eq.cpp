//--------------------------------------------------------------------------------------
// File: eq.cpp
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTguiIME.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmesh.h"
#include "SDKmisc.h"
#include "resource.h"
#include <xaudio2.h>
//#include <xnamath.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <math.h>
#include "vmath.h"
#include "vclass.h"
#include "vclass_typedef.h"
#include "vclass_simdtype.h"
#include "eq.h"

//#define EQ_VCLASS_NO_OVERLOADED_OPERATORS

namespace EQ_VMATH
{
	using namespace VMATH;

	//---------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//
	//                                3 Band EQ :)
	//
	// EQ.C - Main Source file for 3 band EQ
	//
	// (c) Neil C / Etanza Systems / 2K6
	//
	// Shouts / Loves / Moans = etanza at lycos dot co dot uk 
	//
	// This work is hereby placed in the public domain for all purposes, including
	// use in commercial applications.
	//
	// The author assumes NO RESPONSIBILITY for any problems caused by the use of
	// this software.
	//
	//----------------------------------------------------------------------------

	// NOTES :
	//
	// - Original filter code by Paul Kellet (musicdsp.pdf)
	//
	// - Uses 4 first order filters in series, should give 24dB per octave
	//
	// - Now with P4 Denormal fix :)


	//----------------------------------------------------------------------------

	// -----------
	//| Constants |
	// -----------

	static const float cPi = 3.1415926535897932384626433832795f;
	static float vsaf = (1.0f / 4294967295.0f);   // Very small amount (Denormal Fix)
	static Vec4 vsa = VLoad(vsaf);
	static Vec4 M_PI = VLoad(cPi);


	// ---------------
	//| Initialise EQ |
	// ---------------

	// Recommended frequencies are ...
	//
	//  lowfreq  = 880  Hz
	//  highfreq = 5000 Hz
	//
	// Set mixfreq to whatever rate your system is using (eg 48Khz)
	void init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq)
	{
	  // Clear state 

	  memset(es,0,sizeof(EQSTATE));

	  // Set Low/Mid/High gains to unity

	  es->lg = VLoad(1.0f);
	  es->mg = VLoad(1.0f);
	  es->hg = VLoad(1.0f);

	  // Calculate filter cutoff frequencies
	  float	lf = 2 * sinf(cPi * ((float)lowfreq / (float)mixfreq)); 
	  float	hf = 2 * sinf(cPi * ((float)highfreq  / (float)mixfreq)); 

	  es->lf = VLoad(lf);
	  es->hf = VLoad(hf);
	}

	// ---------------
	//| EQ one sample |
	// ---------------

	// - sample can be any range you like :)
	//
	// Note that the output will depend on the gain settings for each band 
	// (especially the bass) so may require clipping before output, but you 
	// knew that anyway :)


	// ---------------
	//| EQ one sample |
	// ---------------

	// - sample can be any range you like :)
	//
	// Note that the output will depend on the gain settings for each band 
	// (especially the bass) so may require clipping before output, but you 
	// knew that anyway :)

	Vec4 do_3band(EQSTATE* es, Vec4& sample)
	{
#ifndef __INTEL_COMPILER	//for intel compiler using overloaded operators usually generate better code
		// Locals

		Vec4  l,m,h;      // Low / Mid / High - Sample Values

		// Filter #1 (lowpass)

		//es.f1p0  += (es.lf * (sample   - es.f1p0)) + vsa;
		es->f1p0 = VAdd(es->f1p0, VAdd(VMul(es->lf, VSub(sample, es->f1p0)), vsa));

		//es->f1p1  += (es->lf * (es->f1p0 - es->f1p1));
		es->f1p1 = VAdd(es->f1p1, VMul(es->lf, VSub(es->f1p0, es->f1p1)));

		//es->f1p2  += (es->lf * (es->f1p1 - es->f1p2));
		es->f1p2 = VAdd(es->f1p2, VMul(es->lf, VSub(es->f1p1, es->f1p2)));

		//es->f1p3  += (es->lf * (es->f1p2 - es->f1p3));
		es->f1p3 = VAdd(es->f1p3, VMul(es->lf, VSub(es->f1p2, es->f1p3)));

		l          = es->f1p3;

		// Filter #2 (highpass)

		//es->f2p0  += (es->hf * (sample   - es->f2p0)) + vsa;
		es->f2p0 = VAdd(es->f2p0, VAdd(VMul(es->hf, VSub(sample, es->f2p0)), vsa));

		//es->f2p1  += (es->hf * (es->f2p0 - es->f2p1));
		es->f2p1 = VAdd(es->f2p1, VMul(es->hf, VSub(es->f2p0, es->f2p1)));

		//es->f2p2  += (es->hf * (es->f2p1 - es->f2p2));
		es->f2p2 = VAdd(es->f2p2, VMul(es->hf, VSub(es->f2p1, es->f2p2)));

		//es->f2p3  += (es->hf * (es->f2p2 - es->f2p3));
		es->f2p3 = VAdd(es->f2p3, VMul(es->hf, VSub(es->f2p2, es->f2p3)));

		//h          = es->sdm3 - es->f2p3;
		h = VSub(es->sdm3, es->f2p3);

		// Calculate midrange (signal - (low + high))

		//m          = es->sdm3 - (h + l);
		m = VSub(es->sdm3, VAdd(h, l));

		// Scale, Combine and store

		//l         *= es->lg;
		//m         *= es->mg;
		//h         *= es->hg;

		l = VMul(l, es->lg);
		m = VMul(m, es->mg);
		h = VMul(h, es->hg);

		// Shuffle history buffer 

		es->sdm3   = es->sdm2;
		es->sdm2   = es->sdm1;
		es->sdm1   = sample;                

		// Return result

		//return(l + m + h);
		return(VAdd(l, VAdd(m, h)));
#else
	  // Locals

	  Vec4  l,m,h;      // Low / Mid / High - Sample Values

	  // Filter #1 (lowpass)

	  es->f1p0  += (es->lf * (sample   - es->f1p0)) + vsa;
	  es->f1p1  += (es->lf * (es->f1p0 - es->f1p1));
	  es->f1p2  += (es->lf * (es->f1p1 - es->f1p2));
	  es->f1p3  += (es->lf * (es->f1p2 - es->f1p3));

	  l          = es->f1p3;

	  // Filter #2 (highpass)
	  
	  es->f2p0  += (es->hf * (sample   - es->f2p0)) + vsa;
	  es->f2p1  += (es->hf * (es->f2p0 - es->f2p1));
	  es->f2p2  += (es->hf * (es->f2p1 - es->f2p2));
	  es->f2p3  += (es->hf * (es->f2p2 - es->f2p3));

	  h          = es->sdm3 - es->f2p3;

	  // Calculate midrange (signal - (low + high))

	  m          = es->sdm3 - (h + l);

	  // Scale, Combine and store

	  l         *= es->lg;
	  m         *= es->mg;
	  h         *= es->hg;

	  // Shuffle history buffer 

	  es->sdm3   = es->sdm2;
	  es->sdm2   = es->sdm1;
	  es->sdm1   = sample;                

	  // Return result

	  return(l + m + h);
#endif
	}
}

namespace EQ_VCLASS
{
	using namespace VCLASS;

	#include "eq_vclass.inl"
}

namespace EQ_VCLASS_TYPEDEF
{
	using namespace VCLASS_TYPEDEF;

	#include "eq_vclass.inl"
}

namespace EQ_VCLASS_SIMDTYPE
{
	using namespace VCLASS_SIMDTYPE;

	#include "eq_vclass.inl"
}
