//--------------------------------------------------------------------------------------
// File: common.h
//--------------------------------------------------------------------------------------

#ifndef __COMMON__
#define __COMMON__

//--------------------------------------------------------------------------------------
// forward declarations hacks
//--------------------------------------------------------------------------------------
extern HRESULT InitXAudio2(void);
extern HRESULT PlayPCM( IXAudio2* pXaudio2, LPCWSTR szFilename );
extern HRESULT PlayChannels(void);
extern HRESULT StopChannels(void);
extern double ProcessAudioFPU(int audioFrames);
extern double ProcessAudioVClass(int audioFrames);
extern double ProcessAudioVMath(int audioFrames);
extern double ProcessAudioXNAMath(int audioFrames);

//globals
extern IXAudio2* g_pXAudio2;
extern LARGE_INTEGER g_qwTimeBefore;
extern LARGE_INTEGER g_qwTimeAfter;
extern LARGE_INTEGER g_ticksPerSecond;
extern float g_floatValues[16];
extern int g_intValues[16];
extern void DBugVec(WCHAR *str, float* p);

//--------------------------------------------------------------------------------------
//									inline time functions
//--------------------------------------------------------------------------------------
inline void PerformanceCounterStart(void)
{
	QueryPerformanceFrequency(&g_ticksPerSecond);
	QueryPerformanceCounter( &g_qwTimeBefore );
}

inline double PerformanceCounterEnd(void)
{
	QueryPerformanceCounter( &g_qwTimeAfter );
	double	diff = (double)(g_qwTimeAfter.QuadPart - g_qwTimeBefore.QuadPart);
	double time = diff/(double)g_ticksPerSecond.QuadPart;
	return(time);
}

//--------------------------------------------------------------------------------------
//									defines & consts
//--------------------------------------------------------------------------------------

#define	MATHLIB_TYPE_FPU		(0)
#define	MATHLIB_TYPE_VMATH		(1)
#define	MATHLIB_TYPE_VCLASS		(2)
#define	MATHLIB_TYPE_XNAMATH	(3)
#define	MATHLIB_TYPE_MAX		(MATHLIB_TYPE_XNAMATH)
#define	MATHLIB_TYPE_MIN		(MATHLIB_TYPE_FPU)

#define DEMO_TYPE_AUDIO			(0)
#define DEMO_TYPE_CLOTH			(1)

//--------------------------------------------------------------------------------------
// more hacks
//--------------------------------------------------------------------------------------
// A structure for our custom vertex type
typedef struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color

} CUSTOMVERTEX, *PCUSTOMVERTEX;

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)


//--------------------------------------------------------------------------------------
// Aux Structs
//--------------------------------------------------------------------------------------

typedef struct AudioSampleStruct
{
	__m128*					pSIMDWavDataSrc;		//source copy for processing SIMD friendly
	__m128*					pSIMDWavDataDest;		//audio data stored as:
													//b0, g0, d0, t0, b1, g1, d1, t1, b2, g2, d2, t2, etc...

	BYTE*					pWavDataSrc[4];		//source copy for processing FPU
	BYTE*					pWavDataDest[4];


	int						wavSize[4];
	IXAudio2SourceVoice*	pSourceVoice[4];

}	AudioChannelStruct;



#endif // #ifndef __COMMON__
