//--------------------------------------------------------------------------------------
// File: eq_xna_exec.cpp
//--------------------------------------------------------------------------------------

#include "DXUT.h"
#include <xaudio2.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include "_xnamath_.h"
#include <math.h>
#include "common.h"

#include <windows.h>
#include <xaudio2.h>
#include <strsafe.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>
#include "SDKwavefile.h"
#include <xmmintrin.h>
#include <emmintrin.h>

#include "eq_xna.h"

extern __declspec(align(128))		AudioSampleStruct	g_AudioSample;
extern __declspec(align(128))		EQ_XNAMATH::EQSTATE g_eqXNAMath;

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
double ProcessAudioXNAMath(int audioFrames)
{
	PerformanceCounterStart();

	//use 1st channel as master window (all channels have the same # of samples)
    XAUDIO2_VOICE_STATE state;
	g_AudioSample.pSourceVoice[0]->GetState( &state );

	int					samples			= g_AudioSample.wavSize[0] / 2;
	unsigned __int64	samplesPlayed	= state.SamplesPlayed;
	int					currSample		= int (samplesPlayed % (unsigned __int64)samples);

	int beg = currSample-audioFrames;
	int end = currSample+audioFrames;

	if (end > samples - 1)
	{
		end = samples - 1;
	}

	if (beg < 0)
	{
		beg = 0;
	}

	XMVECTOR *pSrc = (XMVECTOR*)g_AudioSample.pSIMDWavDataSrc;
	XMVECTOR *pDest = (XMVECTOR*)g_AudioSample.pSIMDWavDataDest;
	XMVECTOR base = XMVectorReplicate(32768.f);

	for(int ii=beg; ii<=end; ii++)
	{
		//loads
		XMVECTOR sampleIn = pSrc[ii];

		//normalize
		sampleIn = sampleIn / base;

		XMVECTOR sampleOut = EQ_XNAMATH::do_3band(&g_eqXNAMath, sampleIn);

		//denormalize
		sampleOut = sampleOut * base;

		//stores
		pDest[ii] = sampleOut;
	}

	//unshuffle and send it to the data channels
	short *pAudioDest0 = (short*)g_AudioSample.pWavDataDest[0];
	short *pAudioDest1 = (short*)g_AudioSample.pWavDataDest[1];
	short *pAudioDest2 = (short*)g_AudioSample.pWavDataDest[2];
	short *pAudioDest3 = (short*)g_AudioSample.pWavDataDest[3];

	for(int ii=beg; ii<=end; ii++)
	{
		__declspec(align(128))	float s[4];

		_mm_store_ps(s, pDest[ii]);

		pAudioDest0[ii] = (short)s[0];
		pAudioDest1[ii] = (short)s[1];
		pAudioDest2[ii] = (short)s[2];
		pAudioDest3[ii] = (short)s[3];
	}

	return(PerformanceCounterEnd());
}
