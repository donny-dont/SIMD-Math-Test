//--------------------------------------------------------------------------------------
// File: CustomUI.cpp
//
// Sample to show usage of DXUT's GUI system
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTguiIME.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmesh.h"
#include "SDKmisc.h"
#include "resource.h"

//--------------------------------------------------------------------------------------
// File: XAudio2BasicSound.cpp
//
// XNA Developer Connection
// (C) Copyright Microsoft Corp.  All rights reserved.
//--------------------------------------------------------------------------------------
#define _WIN32_DCOM
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <xaudio2.h>
#include <strsafe.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>
#include "SDKwavefile.h"

#include <xmmintrin.h>
#include <emmintrin.h>
#include "vclass.h"
#include "vmath.h"
#include "common.h"
#include "eq.h"
#include "cloth.h"
#include "_xnamath_.h"
#include "eq_xna.h"



//--------------------------------------------------------------------------------------
// Helper macros
//--------------------------------------------------------------------------------------
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif


//--------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------
HRESULT PlayPCM( IXAudio2* pXaudio2, LPCWSTR szFilename );
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );

HRESULT LoadPCM( IXAudio2* pXaudio2);
HRESULT PlayChannels(void);

//--------------------------------------------------------------------------------------
// Hacks & nasty globals
//--------------------------------------------------------------------------------------
IXAudio2*					g_pXAudio2 = NULL;
IXAudio2MasteringVoice*		g_pMasteringVoice = NULL;

__declspec(align(128))		EQ_XNAMATH::EQSTATE g_eqXNAMath;
__declspec(align(128))		EQ_VMATH::EQSTATE g_eqVMath;
__declspec(align(128))		EQ_VCLASS::EQSTATE g_eqVClass;
__declspec(align(128))		EQ_FPU::EQSTATE	 g_eqFPU[4];			//unecessary but looks cool

__declspec(align(128))		AudioSampleStruct	g_AudioSample = {0};

//--------------------------------------------------------------------------------------
// Init XAudio2
//--------------------------------------------------------------------------------------
HRESULT InitXAudio2(void)
{
    HRESULT hr;

    //
    // Initialize XAudio2
    //
    CoInitializeEx( NULL, COINIT_MULTITHREADED );
   

    UINT32 flags = 0;
#ifdef _DEBUG
    flags |= XAUDIO2_DEBUG_ENGINE;
#endif

    if( FAILED( hr = XAudio2Create( &g_pXAudio2, flags ) ) )
    {
        wprintf( L"Failed to init XAudio2 engine: %#X\n", hr );
        CoUninitialize();
        return 0;
    }

    //
    // Create a mastering voice
    //
	if( FAILED( hr = g_pXAudio2->CreateMasteringVoice( &g_pMasteringVoice ) ) )
    {
        wprintf( L"Failed creating mastering voice: %#X\n", hr );
        SAFE_RELEASE( g_pXAudio2 );
        CoUninitialize();
        return 0;
    }

	LoadPCM(g_pXAudio2);

	return (hr);
}

//--------------------------------------------------------------------------------------
// Shutdown XAudio2
//--------------------------------------------------------------------------------------
HRESULT ShutdownXAudio2(void)
{
	HRESULT hr = S_OK;

    // All XAudio2 interfaces are released when the engine is destroyed, but being tidy
    g_pMasteringVoice->DestroyVoice();

    SAFE_RELEASE( g_pXAudio2 );

	for(int ii=0; ii<4; ii++)
	{
		free( g_AudioSample.pWavDataSrc[ii] );
		free( g_AudioSample.pWavDataDest[ii] );
	}

	free( g_AudioSample.pSIMDWavDataSrc );
	free( g_AudioSample.pSIMDWavDataDest );


    CoUninitialize();

	return (hr);
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT PlayChannels(void)
{
	HRESULT hr = S_OK;

	//start all channels as soon as possible
	for(int ii=0; ii<4; ii++)
	{
		g_AudioSample.pSourceVoice[ii]->Start( 0 );
	}

	return (hr);
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT StopChannels(void)
{
	HRESULT hr = S_OK;

	for(int ii=0; ii<4; ii++)
	{
		if (g_AudioSample.pSourceVoice[ii])
		{
			g_AudioSample.pSourceVoice[ii]->Stop();
		}
	}

	return (hr);
}


//--------------------------------------------------------------------------------------
// Name: LoadPCM
// Desc: Load wav data into audio voices
//--------------------------------------------------------------------------------------
HRESULT LoadPCM( IXAudio2* pXaudio2)
{
	HRESULT hr = S_OK;

	LPCWSTR	aTrackFileName[] =
	{
		L"Media\\Wavs\\article_bass_16b.wav",
		L"Media\\Wavs\\article_drums_16b.wav",
		L"Media\\Wavs\\article_gtr_16b.wav",
		L"Media\\Wavs\\article_trumpet_16b.wav",
	};

    //
    // Locate the wave file
    //
	for(int ii=0; ii<4; ii++)
	{
		LPCWSTR szFilename = aTrackFileName[ii];

		WCHAR strFilePath[MAX_PATH];
		if( FAILED( hr = FindMediaFileCch( strFilePath, MAX_PATH, szFilename ) ) )
		{
			wprintf( L"Failed to find media file: %s\n", szFilename );
			return hr;
		}

		//
		// Read in the wave file
		//
		CWaveFile wav;
		if( FAILED( hr = wav.Open( strFilePath, NULL, WAVEFILE_READ ) ) )
		{
			wprintf( L"Failed reading WAV file: %#X (%s)\n", hr, strFilePath );
			return hr;
		}

		// Get format of wave file
		WAVEFORMATEX* pwfx = wav.GetFormat();

		// Calculate how many bytes and samples are in the wave
		DWORD cbWaveSize = wav.GetSize();

		// Read the sample data into memory
		BYTE* pbWaveData = new BYTE[ cbWaveSize ];
	
		if( FAILED( hr = wav.Read( pbWaveData, cbWaveSize, &cbWaveSize ) ) )
		{
			wprintf( L"Failed to read WAV data: %#X\n", hr );
			SAFE_DELETE_ARRAY( pbWaveData );
			return hr;
		}

		// Create the source voice
		if( FAILED( hr = pXaudio2->CreateSourceVoice( &g_AudioSample.pSourceVoice[ii], pwfx ) ) )
		{
			wprintf( L"Error %#X creating source voice\n", hr );
			SAFE_DELETE_ARRAY( pbWaveData );
			return hr;
		}

		//save src & dest voice for audio processing
		g_AudioSample.pWavDataSrc[ii] = pbWaveData;
		g_AudioSample.pWavDataDest[ii] = new BYTE[ cbWaveSize ];
		memcpy(g_AudioSample.pWavDataDest[ii], g_AudioSample.pWavDataSrc[ii], cbWaveSize);

		g_AudioSample.wavSize[ii] = cbWaveSize;

		// Submit the wave sample data using an XAUDIO2_BUFFER structure
		XAUDIO2_BUFFER buffer = {0};
		buffer.pAudioData = g_AudioSample.pWavDataDest[ii];
		buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
		buffer.AudioBytes = cbWaveSize;
		//buffer.LoopLength = buffer.AudioBytes;
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

		if( FAILED( hr = g_AudioSample.pSourceVoice[ii]->SubmitSourceBuffer( &buffer ) ) )
		{
			wprintf( L"Error %#X submitting source buffer\n", hr );
			g_AudioSample.pSourceVoice[ii]->DestroyVoice();
			SAFE_DELETE_ARRAY( g_AudioSample.pWavDataSrc[ii] );
			SAFE_DELETE_ARRAY( g_AudioSample.pWavDataDest[ii] );
			return hr;
		}
	}

	//init eq's
	for(int ii=0; ii<4; ii++)
	{
		EQ_FPU::init_3band_state(&g_eqFPU[ii],880,5000,44100);

		g_eqFPU[ii].lg = 1.0f; // Boost bass by 50%
		g_eqFPU[ii].mg = 1.0f; // Cut mid by 25%
		g_eqFPU[ii].hg = 1.0f; // Leave high band alone 
	}

	
	{
		using namespace	VCLASS;
		Vec4 lg = Vec4(1.f);
		Vec4 mg	= Vec4(1.f);
		Vec4 hg = Vec4(1.f);

		EQ_VCLASS::init_3band_state(&g_eqVClass,880,5000,44100);
	}

	
	{
		using namespace	VMATH;
		Vec4 lg = VLoad(1.f);
		Vec4 mg	= VLoad(1.f);
		Vec4 hg = VLoad(1.f);

		EQ_VMATH::init_3band_state(&g_eqVMath,880,5000,44100);
	}

	{
		XMVECTOR lg = XMVectorReplicate(1.f);
		XMVECTOR mg	= XMVectorReplicate(1.f);
		XMVECTOR hg = XMVectorReplicate(1.f);

		EQ_XNAMATH::init_3band_state(&g_eqXNAMath,880,5000,44100);
	}


	//store source data SIMD friendly (assuming all channels have the same data size)
	for(int ii=0; ii<3; ii++)
	{
		assert (g_AudioSample.wavSize[ii] == g_AudioSample.wavSize[ii+1]);
	}

	int samples = g_AudioSample.wavSize[0] / 2; 

	//original samples stored as 16-bit so it takes twice as much in 32-bit floats
	g_AudioSample.pSIMDWavDataSrc = new __m128[ samples ];	
	g_AudioSample.pSIMDWavDataDest = new __m128[ samples ];	

	//shuffle the data to be SIMD friendly
	short*	bass	= (short*)g_AudioSample.pWavDataSrc[0];
	short*	guitar	= (short*)g_AudioSample.pWavDataSrc[1];
	short*	drums	= (short*)g_AudioSample.pWavDataSrc[2];
	short*	trumpet	= (short*)g_AudioSample.pWavDataSrc[3];

	__m128	*pSIMD = (__m128*)g_AudioSample.pSIMDWavDataSrc;

	for(int jj=0; jj<samples; jj++)
	{
		float b = (float)bass[jj];
		float g = (float)guitar[jj];
		float d = (float)drums[jj];
		float t = (float)trumpet[jj];

		pSIMD[jj] = _mm_set_ps(b, g, d, t);
	}

	memcpy(g_AudioSample.pSIMDWavDataDest, g_AudioSample.pSIMDWavDataSrc, samples*16);

	return (hr);
}

//--------------------------------------------------------------------------------------
// Name: From the UI update the EQs
//--------------------------------------------------------------------------------------
void UpdateEQ( float *pGainArr)
{
	int	gi = 0;

	for(int ch=0; ch<4; ch++)
	{
		g_eqFPU[ch].lg = pGainArr[gi++];
		g_eqFPU[ch].mg = pGainArr[gi++];
		g_eqFPU[ch].hg = pGainArr[gi++];
	}

	{
		using namespace	VCLASS;

		int ch = 0;
		g_eqVClass.lg = Vec4(pGainArr[ch], pGainArr[ch+3], pGainArr[ch+6], pGainArr[ch+9]);
		ch++;
		g_eqVClass.mg = Vec4(pGainArr[ch], pGainArr[ch+3], pGainArr[ch+6], pGainArr[ch+9]);
		ch++;
		g_eqVClass.hg = Vec4(pGainArr[ch], pGainArr[ch+3], pGainArr[ch+6], pGainArr[ch+9]);
	}

	{
		using namespace	VMATH;

		int ch = 0;
		g_eqVMath.lg = VLoad(pGainArr[ch], pGainArr[ch+3], pGainArr[ch+6], pGainArr[ch+9]);
		ch++;
		g_eqVMath.mg = VLoad(pGainArr[ch], pGainArr[ch+3], pGainArr[ch+6], pGainArr[ch+9]);
		ch++;
		g_eqVMath.hg = VLoad(pGainArr[ch], pGainArr[ch+3], pGainArr[ch+6], pGainArr[ch+9]);
	}

	{
		int ch = 0;
		g_eqXNAMath.lg = _mm_set_ps(pGainArr[ch], pGainArr[ch+3], pGainArr[ch+6], pGainArr[ch+9]);
		ch++;
		g_eqXNAMath.mg = _mm_set_ps(pGainArr[ch], pGainArr[ch+3], pGainArr[ch+6], pGainArr[ch+9]);
		ch++;
		g_eqXNAMath.hg = _mm_set_ps(pGainArr[ch], pGainArr[ch+3], pGainArr[ch+6], pGainArr[ch+9]);
	}
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
double ProcessAudioVClass(int audioFrames)
{
	using namespace VCLASS;

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

	Vec4 *pSrc = (Vec4*)g_AudioSample.pSIMDWavDataSrc;
	Vec4 *pDest = (Vec4*)g_AudioSample.pSIMDWavDataDest;
	Vec4 base(32768.f);

	for(int ii=beg; ii<=end; ii++)
	{
		//loads
		Vec4 sampleIn = pSrc[ii];
		sampleIn = sampleIn / base;

		Vec4 sampleOut = EQ_VCLASS::do_3band(&g_eqVClass, sampleIn);

		//denormalize
		sampleOut = sampleOut*base;

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

		pDest[ii].Store(s);

		pAudioDest0[ii] = (short)s[0];
		pAudioDest1[ii] = (short)s[1];
		pAudioDest2[ii] = (short)s[2];
		pAudioDest3[ii] = (short)s[3];
	}

	return(PerformanceCounterEnd());
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
double ProcessAudioVMath(int audioFrames)
{
	using namespace VMATH;

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

	Vec4 *pSrc = (Vec4*)g_AudioSample.pSIMDWavDataSrc;
	Vec4 *pDest = (Vec4*)g_AudioSample.pSIMDWavDataDest;
	Vec4 base = VLoad(32768.f);

	for(int ii=beg; ii<=end; ii++)
	{
		//loads
		Vec4 sampleIn = pSrc[ii];

		//normalize
		sampleIn = VDiv(sampleIn, base);

		Vec4 sampleOut = EQ_VMATH::do_3band(&g_eqVMath, sampleIn);

		//denormalize
		sampleOut = VMul(sampleOut, base);

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

		VStore(s, pDest[ii]);

		pAudioDest0[ii] = (short)s[0];
		pAudioDest1[ii] = (short)s[1];
		pAudioDest2[ii] = (short)s[2];
		pAudioDest3[ii] = (short)s[3];
	}

	return(PerformanceCounterEnd());
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
double ProcessAudioFPU(int audioFrames)
{
	PerformanceCounterStart();

	for(int ch=0; ch<4; ch++)
	{
		int samples = g_AudioSample.wavSize[ch] / 2;
		short *pAudioSrc = (short*)g_AudioSample.pWavDataSrc[ch];
		short *pAudioDest = (short*)g_AudioSample.pWavDataDest[ch];

        XAUDIO2_VOICE_STATE state;
		g_AudioSample.pSourceVoice[ch]->GetState( &state );

		unsigned __int64	samplesPlayed	= state.SamplesPlayed;
		int					currSample		= int (samplesPlayed % (unsigned __int64)samples);

		//process window around the current sample
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

		for(int ii=beg; ii<=end; ii++)
		{
			//normalize
			float	sampleIn = ((float)pAudioSrc[ii])/32768.f;
			float	sampleOut = EQ_FPU::do_3band(&g_eqFPU[ch], sampleIn);

			//denormalize
			short s = (short)(sampleOut*32768.f);
			pAudioDest[ii] = s;
		}

	}
	
	return(PerformanceCounterEnd());
}

//--------------------------------------------------------------------------------------
// Helper function to try to find the location of a media file
//--------------------------------------------------------------------------------------
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename )
{
    bool bFound = false;

    if( NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10 )
        return E_INVALIDARG;

    // Get the exe name, and exe path
    WCHAR strExePath[MAX_PATH] = {0};
    WCHAR strExeName[MAX_PATH] = {0};
    WCHAR* strLastSlash = NULL;
    GetModuleFileName( NULL, strExePath, MAX_PATH );
    strExePath[MAX_PATH - 1] = 0;
    strLastSlash = wcsrchr( strExePath, TEXT( '\\' ) );
    if( strLastSlash )
    {
        wcscpy_s( strExeName, MAX_PATH, &strLastSlash[1] );

        // Chop the exe name from the exe path
        *strLastSlash = 0;

        // Chop the .exe from the exe name
        strLastSlash = wcsrchr( strExeName, TEXT( '.' ) );
        if( strLastSlash )
            *strLastSlash = 0;
    }

    wcscpy_s( strDestPath, cchDest, strFilename );
    if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
        return S_OK;

    // Search all parent directories starting at .\ and using strFilename as the leaf name
    WCHAR strLeafName[MAX_PATH] = {0};
    wcscpy_s( strLeafName, MAX_PATH, strFilename );

    WCHAR strFullPath[MAX_PATH] = {0};
    WCHAR strFullFileName[MAX_PATH] = {0};
    WCHAR strSearch[MAX_PATH] = {0};
    WCHAR* strFilePart = NULL;

    GetFullPathName( L".", MAX_PATH, strFullPath, &strFilePart );
    if( strFilePart == NULL )
        return E_FAIL;

    while( strFilePart != NULL && *strFilePart != '\0' )
    {
        swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            wcscpy_s( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            wcscpy_s( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        swprintf_s( strSearch, MAX_PATH, L"%s\\..", strFullPath );
        GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
    }
    if( bFound )
        return S_OK;

    // On failure, return the file as the path but also return an error code
    wcscpy_s( strDestPath, cchDest, strFilename );

    return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
}
