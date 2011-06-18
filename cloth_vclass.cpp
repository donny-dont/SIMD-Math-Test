//--------------------------------------------------------------------------------------
//	File: cloth.cpp
//
//	Original algorithm extracted from the white paper:
//	Advanced Character Physics by Thomas Jakobsen
//	http://www.teknikus.dk/tj/gdc2001.htm
///////////////////////////////////////////////////////////////////////////////

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
#include <xmmintrin.h>
#include <emmintrin.h>
#include <math.h>
#include "common.h"
#include "vclass.h"
#include "vclass_typedef.h"
#include "vclass_simdtype.h"
#include "cloth.h"

///////////////////////////////////////////////////////////////////////////////
//								Consts & Defines
///////////////////////////////////////////////////////////////////////////////

#define	VCLASS_PI				(3.1415926535897932384626433832795f)
#define	VCLASS_DTOR(angle)		((VCLASS_PI/180.0f)*((float)angle))
#define	VCLASS_RTOD(angle)		((180.0f/VCLASS_PI)*((float)angle))

namespace CLOTH_VCLASS
{
	using namespace VCLASS;

	#include "cloth_vclass.inl"
}

namespace CLOTH_VCLASS_TYPEDEF
{
	using namespace VCLASS_TYPEDEF;

	#include "cloth_vclass.inl"
}

namespace CLOTH_VCLASS_SIMDTYPE
{
	using namespace VCLASS_SIMDTYPE;

	#include "cloth_vclass.inl"
}
