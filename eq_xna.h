//--------------------------------------------------------------------------------------
// File: eq.h
//--------------------------------------------------------------------------------------

#ifndef __EQ3BANDXNA__
#define __EQ3BANDXNA__

namespace EQ_XNAMATH
{
	// ------------
	//| Structures |
	// ------------

	typedef struct
	{
	  // Filter #1 (Low band)

	  XMVECTOR  lf;       // Frequency
	  XMVECTOR  f1p0;     // Poles ...
	  XMVECTOR  f1p1;     
	  XMVECTOR  f1p2;
	  XMVECTOR  f1p3;

	  // Filter #2 (High band)

	  XMVECTOR  hf;       // Frequency
	  XMVECTOR  f2p0;     // Poles ...
	  XMVECTOR  f2p1;
	  XMVECTOR  f2p2;
	  XMVECTOR  f2p3;

	  // Sample history buffer

	  XMVECTOR  sdm1;     // Sample data minus 1
	  XMVECTOR  sdm2;     //                   2
	  XMVECTOR  sdm3;     //                   3

	  // Gain Controls

	  XMVECTOR  lg;       // low  gain
	  XMVECTOR  mg;       // mid  gain
	  XMVECTOR  hg;       // high gain
	  
	} EQSTATE;  


	// ---------
	//| Exports |
	// ---------

	extern void	init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq);
	extern XMVECTOR	do_3band(EQSTATE* es, XMVECTOR& sample);
}

#endif // #ifndef __EQ3BANDXNA__
