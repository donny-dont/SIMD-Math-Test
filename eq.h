//--------------------------------------------------------------------------------------
// File: eq.h
//--------------------------------------------------------------------------------------

#ifndef __EQ3BAND__
#define __EQ3BAND__

namespace EQ_FPU
{
	// ------------
	//| Structures |
	// ------------

	typedef struct
	{
	  // Filter #1 (Low band)

	  float  lf;       // Frequency
	  float  f1p0;     // Poles ...
	  float  f1p1;     
	  float  f1p2;
	  float  f1p3;

	  // Filter #2 (High band)

	  float  hf;       // Frequency
	  float  f2p0;     // Poles ...
	  float  f2p1;
	  float  f2p2;
	  float  f2p3;

	  // Sample history buffer

	  float  sdm1;     // Sample data minus 1
	  float  sdm2;     //                   2
	  float  sdm3;     //                   3

	  // Gain Controls

	  float  lg;       // low  gain
	  float  mg;       // mid  gain
	  float  hg;       // high gain
	  
	} EQSTATE;  


	// ---------
	//| Exports |
	// ---------

	extern void   init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq);
	extern float do_3band(EQSTATE* es, float sample);
}

namespace EQ_VCLASS
{
	using namespace VCLASS;

	// ------------
	//| Structures |
	// ------------

	typedef struct
	{
	  // Filter #1 (Low band)

	  Vec4  lf;       // Frequency
	  Vec4  f1p0;     // Poles ...
	  Vec4  f1p1;     
	  Vec4  f1p2;
	  Vec4  f1p3;

	  // Filter #2 (High band)

	  Vec4  hf;       // Frequency
	  Vec4  f2p0;     // Poles ...
	  Vec4  f2p1;
	  Vec4  f2p2;
	  Vec4  f2p3;

	  // Sample history buffer

	  Vec4  sdm1;     // Sample data minus 1
	  Vec4  sdm2;     //                   2
	  Vec4  sdm3;     //                   3

	  // Gain Controls

	  Vec4  lg;       // low  gain
	  Vec4  mg;       // mid  gain
	  Vec4  hg;       // high gain
	  
	} EQSTATE;  


	// ---------
	//| Exports |
	// ---------

	extern void	init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq);
	extern Vec4	do_3band(EQSTATE* es, Vec4& sample);
}


namespace EQ_VMATH
{
	using namespace VMATH;

	// ------------
	//| Structures |
	// ------------

	typedef struct
	{
	  // Filter #1 (Low band)

	  Vec4  lf;       // Frequency
	  Vec4  f1p0;     // Poles ...
	  Vec4  f1p1;     
	  Vec4  f1p2;
	  Vec4  f1p3;

	  // Filter #2 (High band)

	  Vec4  hf;       // Frequency
	  Vec4  f2p0;     // Poles ...
	  Vec4  f2p1;
	  Vec4  f2p2;
	  Vec4  f2p3;

	  // Sample history buffer

	  Vec4  sdm1;     // Sample data minus 1
	  Vec4  sdm2;     //                   2
	  Vec4  sdm3;     //                   3

	  // Gain Controls

	  Vec4  lg;       // low  gain
	  Vec4  mg;       // mid  gain
	  Vec4  hg;       // high gain
	  
	} EQSTATE;  


	// ---------
	//| Exports |
	// ---------

	extern void	init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq);
	extern Vec4	do_3band(EQSTATE* es, Vec4& sample);
}

#endif // #ifndef __EQ3BAND__