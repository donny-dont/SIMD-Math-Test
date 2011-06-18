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
	static Vec4 vsa (vsaf);
	static Vec4 M_PI(cPi);


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

	  es->lg = Vec4(1.0f);
	  es->mg = Vec4(1.0f);
	  es->hg = Vec4(1.0f);

	  // Calculate filter cutoff frequencies
	  float	lf = 2 * sinf(cPi * ((float)lowfreq / (float)mixfreq)); 
	  float	hf = 2 * sinf(cPi * ((float)highfreq  / (float)mixfreq)); 

	  es->lf = Vec4(lf);
	  es->hf = Vec4(hf);
	}

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
#ifdef EQ_VCLASS_NO_OVERLOADED_OPERATORS
		// Locals

		Vec4  l,m,h;      // Low / Mid / High - Sample Values

		// Filter #1 (lowpass)
		Vec4 tmp0, tmp1, tmp2;

		//es.f1p0  += (es.lf * (sample   - es.f1p0)) + vsa;
		tmp0 = Vec4::VSub(sample, es->f1p0);
		tmp1 = Vec4::VMul(es->lf, tmp0);
		tmp2 = Vec4::VAdd(tmp1, vsa);
		es->f1p0 = Vec4::VAdd(es->f1p0, tmp2);

		//es->f1p1  += (es->lf * (es->f1p0 - es->f1p1));
		tmp0 = Vec4::VSub(es->f1p0, es->f1p1);
		tmp1 = Vec4::VMul(es->lf, tmp0);
		es->f1p1 = Vec4::VAdd(es->f1p1, tmp1);

		//es->f1p2  += (es->lf * (es->f1p1 - es->f1p2));
		tmp0 = Vec4::VSub(es->f1p1, es->f1p2);
		tmp1 = Vec4::VMul(es->lf, tmp0);
		es->f1p2 = Vec4::VAdd(es->f1p2, tmp1);

		//es->f1p3  += (es->lf * (es->f1p2 - es->f1p3));
		tmp0 = Vec4::VSub(es->f1p2, es->f1p3);
		tmp1 = Vec4::VMul(es->lf, tmp0);
		es->f1p3 = Vec4::VAdd(es->f1p3, tmp1);

		l          = es->f1p3;

		// Filter #2 (highpass)

		//es->f2p0  += (es->hf * (sample   - es->f2p0)) + vsa;
		tmp0 = Vec4::VSub(sample, es->f2p0);
		tmp1 = Vec4::VMul(es->hf, tmp0);
		tmp2 = Vec4::VAdd(tmp1, vsa);
		es->f2p0 = Vec4::VAdd(es->f2p0, tmp2);

		//es->f2p1  += (es->hf * (es->f2p0 - es->f2p1));
		tmp0 = Vec4::VSub(es->f2p0, es->f2p1);
		tmp1 = Vec4::VMul(es->hf, tmp0);
		es->f2p1 = Vec4::VAdd(es->f2p1, tmp1);

		//es->f2p2  += (es->hf * (es->f2p1 - es->f2p2));
		tmp0 = Vec4::VSub(es->f2p1, es->f2p2);
		tmp1 = Vec4::VMul(es->hf, tmp0);
		es->f2p2 = Vec4::VAdd(es->f2p2, tmp1);

		//es->f2p3  += (es->hf * (es->f2p2 - es->f2p3));
		tmp0 = Vec4::VSub(es->f2p2, es->f2p3);
		tmp1 = Vec4::VMul(es->hf, tmp0);
		es->f2p3 = Vec4::VAdd(es->f2p3, tmp1);

		//h          = es->sdm3 - es->f2p3;
		h = Vec4::VSub(es->sdm3, es->f2p3);

		// Calculate midrange (signal - (low + high))

		//m          = es->sdm3 - (h + l);
		tmp0 = Vec4::VAdd(h, l);
		m = Vec4::VSub(es->sdm3, tmp0);

		// Scale, Combine and store

		//l         *= es->lg;
		//m         *= es->mg;
		//h         *= es->hg;

		l = Vec4::VMul(l, es->lg);
		m = Vec4::VMul(m, es->mg);
		h = Vec4::VMul(h, es->hg);

		// Shuffle history buffer 

		es->sdm3   = es->sdm2;
		es->sdm2   = es->sdm1;
		es->sdm1   = sample;                

		// Return result

		tmp0 = Vec4::VAdd(m, h);
		tmp1 = Vec4::VAdd(l, tmp0);

		//return(l + m + h);
		return(tmp1);
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