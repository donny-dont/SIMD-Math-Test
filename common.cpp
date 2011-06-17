//--------------------------------------------------------------------------------------
// File: common.cpp
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
#include <math.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include "common.h"


//--------------------------------------------------------------------------------------
//									shared globals
//--------------------------------------------------------------------------------------

//performance counters
LARGE_INTEGER g_qwTimeBefore = { 0 };
LARGE_INTEGER g_qwTimeAfter = { 0 };
LARGE_INTEGER g_ticksPerSecond = { 0 };

//tunning hacks
float	g_floatValues[16] = { 0 };
int		g_intValues[16]  = { 0 };

//--------------------------------------------------------------------------------------
//	Debug
//--------------------------------------------------------------------------------------
void DBugVec(WCHAR *str, float* p)
{
	WCHAR wszOutput[1024];
	swprintf_s(wszOutput, 1024, L"\n%s: (%5.4f, %5.4f, %5.4f, %5.4f)", str, p[0], p[1], p[2], p[3]);
	OutputDebugString(wszOutput);
}
