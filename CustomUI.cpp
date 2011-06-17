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
#include <xaudio2.h>
#include <math.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include "common.h"
#include "cloth.h"
#include "vmath.h"

//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 

//#define DBUG_EDIT_BOX

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,
                                  void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext );
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void CALLBACK OnLostDevice( void* pUserContext );
void CALLBACK OnDestroyDevice( void* pUserContext );

void InitApp();
void RenderText();
void UpdateStats(double totalTime);

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*                  g_pFont = NULL;         // Font for drawing text
ID3DXSprite*                g_pTextSprite = NULL;   // Sprite for batching draw text calls
ID3DXEffect*                g_pEffect = NULL;       // D3DX effect interface
CDXUTXFileMesh              g_Mesh;                 // Background mesh
D3DXMATRIXA16               g_mView;
CModelViewerCamera          g_Camera;               // A model viewing camera
CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg             g_SettingsDlg;          // Device settings dialog
CDXUTDialog                 g_HUD;                  // dialog for standard controls
CDXUTDialog                 g_SampleUI;             // dialog for sample specific controls


//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN			(100)
#define IDC_TOGGLEREF					(101)
#define IDC_CHANGEDEVICE				(102)
#define IDC_AUDIODEMO					(103)
#define IDC_CLOTHDEMO					(104)
#define IDC_RESET_STATS					(105)
#define IDC_TOOGLE_LOOP_LIBS			(106)

#define IDC_OUTPUT_CALC_STATS			(200)
#define IDC_OUTPUT_CALC_REPS			(201)

#define IDC_OUTPUT_BAYARD_TITLE			(250)

#define IDC_OUTPUT_AVG_STATS_VMATH		(300)
#define IDC_OUTPUT_AVG_STATS_XNAMATH	(301)
#define IDC_OUTPUT_AVG_STATS_VCLASS		(302)
#define IDC_OUTPUT_AVG_STATS_FPU		(303)

#define IDC_EDITBOX_DBUG				(400)

#define IDC_RADIO_VMATH					(500)
#define IDC_RADIO_XNAMATH				(501)
#define IDC_RADIO_VCLASS				(502)
#define IDC_RADIO_FPU					(503)

#define IDC_SLIDER_CH1_LOW				(600)
#define IDC_SLIDER_CH1_MID				(601)
#define IDC_SLIDER_CH1_HIGH				(602)
#define IDC_SLIDER_CH2_LOW				(603)
#define IDC_SLIDER_CH2_MID				(604)
#define IDC_SLIDER_CH2_HIGH				(605)
#define IDC_SLIDER_CH3_LOW				(606)
#define IDC_SLIDER_CH3_MID				(607)
#define IDC_SLIDER_CH3_HIGH				(608)
#define IDC_SLIDER_CH4_LOW				(609)
#define IDC_SLIDER_CH4_MID				(610)
#define IDC_SLIDER_CH4_HIGH				(611)
#define IDC_STATIC_CH_MIN				(612)
#define IDC_STATIC_CH_MAX				(IDC_STATIC_CH_MIN + g_audioSliderIdSize - 1)

#define IDC_PLAY_BTN					(700)
#define IDC_PAUSE_BTN					(701)

#define IDC_SLIDER_MATH_REPS			(800)

#define IDC_SLIDER_CLOTH_ROT			(900)
#define IDC_SLIDER_CLOTH_TRANS			(901)
#define IDC_SLIDER_CLOTH_GRAVITY		(902)
#define IDC_STATIC_CLOTH_MIN			(903)
#define IDC_STATIC_CLOTH_MAX			(IDC_STATIC_CLOTH_MIN + g_clothSliderIdSize - 1)



//--------------------------------------------------------------------------------------
// Global hacks
//--------------------------------------------------------------------------------------

LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices

const unsigned int g_audioSliderId[] =
{
	IDC_SLIDER_CH1_LOW,
	IDC_SLIDER_CH1_MID,
	IDC_SLIDER_CH1_HIGH,

	IDC_SLIDER_CH2_LOW,
	IDC_SLIDER_CH2_MID,
	IDC_SLIDER_CH2_HIGH,

	IDC_SLIDER_CH3_LOW,
	IDC_SLIDER_CH3_MID,
	IDC_SLIDER_CH3_HIGH,

	IDC_SLIDER_CH4_LOW,
	IDC_SLIDER_CH4_MID,
	IDC_SLIDER_CH4_HIGH,
};

const unsigned int g_audioSliderIdSize = sizeof(g_audioSliderId)/sizeof(const unsigned int);

const unsigned int g_clothSliderId[] =
{
	IDC_SLIDER_CLOTH_ROT,
	IDC_SLIDER_CLOTH_TRANS,
	IDC_SLIDER_CLOTH_GRAVITY,
};

const unsigned int g_clothSliderIdSize = sizeof(g_clothSliderId)/sizeof(const unsigned int);

const int cSliderX0		= 10;
const int cSliderY0		= 50;
const int cSliderWidth	= 300;
const int cSliderHeight	= 16;

LPCWSTR	g_audioSliderText[] =
{
	L"Bass Low",
	L"Bass Mid",
	L"Bass High",

	L"Drums Low",
	L"Drums Mid",
	L"Drums High",

	L"Guitar Low",
	L"Guitar Mid",
	L"Guitar High",

	L"Trumpet Low",
	L"Trumpet Mid",
	L"Trumpet High",
};

LPCWSTR	g_clothSliderText[] =
{
	L"Rotation",
	L"Translation",
	L"Gravity",
};

//demo flags
int g_demoType = DEMO_TYPE_CLOTH;
//int g_demoType = DEMO_TYPE_AUDIO;

int g_libType = MATHLIB_TYPE_VMATH;
int g_libTypeLast = g_libType;
bool g_loopLibs = false;

//demo reps factors
float g_audioReps	= 1;
float g_clothReps	= 1.f/2.8f;

const int cAvgSize = 16;
double g_avgVMath[cAvgSize] = {0};
double g_avgVMathMin = 999999.f;
double g_avgXNAMath[cAvgSize] = {0};
double g_avgXNAMathMin = 999999.f;
double g_avgVClass[cAvgSize] = {0};
double g_avgVClassMin = 999999.f;
double g_avgFPU[cAvgSize] = {0};
double g_avgFPUMin = 999999.f;


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	extern void TestDot(void);
	extern void TestSine(void);

	TestDot();
	TestSine();

	//more tests
	{
		using namespace VMATH;

		Vec4 va = VLoad(1.f);
		Vec4 vb = VLoad(2.f);
		Vec4 vr;

		VAddSlow(vr, va, vb);

		printf("");
	}


    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device (either D3D9 or D3D10) 
    // that is available on the system depending on which D3D callbacks are set below

    // Set DXUT callbacks
    DXUTSetCallbackD3D9DeviceAcceptable( IsDeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnCreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnFrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnLostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( KeyboardProc );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

    DXUTSetCursorSettings( true, true );
    InitApp();
	InitXAudio2();
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );
    DXUTCreateWindow( L"VMath Demo" );
    DXUTCreateDevice( true, 640, 480 );
    DXUTMainLoop();

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Helper Functions
//--------------------------------------------------------------------------------------
int GetCurrReps()
{
	int reps;
	float *currReps = NULL;

	if( g_demoType == DEMO_TYPE_CLOTH)
	{
		currReps = &g_clothReps;
	}
	else if ( g_demoType == DEMO_TYPE_AUDIO)
	{
		currReps = &g_audioReps;
	}
	else
	{
		assert(false);
	}

	int sliderValue = g_SampleUI.GetSlider( IDC_SLIDER_MATH_REPS )->GetValue();
	reps =  (int)((float)sliderValue * (*currReps));

	if (reps == 0)
	{
		//do at least one rep
		reps = 1;
	}

	return(reps);
}


//--------------------------------------------------------------------------------------
// Helper Functions
//--------------------------------------------------------------------------------------
inline char* ConvertToASCII(LPCWSTR str)
{
	static char	ansi[1024];

	int	ii=0;
	while(*str != 0)
	{
		ansi[ii++] = (char)*str;
		str++;
	}

	ansi[ii] = 0x00;

	return(ansi);
}

//--------------------------------------------------------------------------------------
// Helper Functions
//--------------------------------------------------------------------------------------
void ParseText(LPCWSTR str)
{
	char*	string1 = ConvertToASCII(str);

	char seps[]   = " ,\t\n";
	char *token1,
		 *next_token1;

	token1 = strtok_s( string1, seps, &next_token1);

	int ii=0;
	while (token1 != NULL)
    {
        // Get next token:
        if (token1 != NULL)
        {
			g_intValues[ii] = (int)atoi( token1 );
            g_floatValues[ii++] = (float)atof( token1 );
			
            token1 = strtok_s( NULL, seps, &next_token1);
			
        }
	}
}

//--------------------------------------------------------------------------------------
// Turn on/off visiblity of demo controls
//--------------------------------------------------------------------------------------
void UpdateDemoUI()
{
	bool audioVisible = false;
	bool clothVisible = false;

	if (g_demoType == DEMO_TYPE_AUDIO)
	{
		clothVisible = false;
		audioVisible = true;
	}
	else if (g_demoType == DEMO_TYPE_CLOTH)
	{
		StopChannels();
		clothVisible = true;
		audioVisible = false;
	}

	//audio controls
	for (int ii = 0; ii<g_audioSliderIdSize; ii++)
	{
		g_SampleUI.GetSlider( g_audioSliderId[ii] )->SetVisible(audioVisible);

		int ID = IDC_STATIC_CH_MIN+ii;
		g_SampleUI.GetStatic( ID )->SetVisible(audioVisible);

	}

	g_SampleUI.GetButton( IDC_PLAY_BTN )->SetVisible(audioVisible);
	g_SampleUI.GetButton( IDC_PAUSE_BTN )->SetVisible(audioVisible);

	//cloth controls
	for (int ii = 0; ii<g_clothSliderIdSize; ii++)
	{
		g_SampleUI.GetSlider( g_clothSliderId[ii] )->SetVisible(clothVisible);

		int ID = IDC_STATIC_CLOTH_MIN+ii;
		g_SampleUI.GetStatic( ID )->SetVisible(clothVisible);

	}
}

//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    // Initialize dialogs
    g_SettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );

    g_HUD.SetCallback( OnGUIEvent ); int iY = 10;
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
    g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
    g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

	g_HUD.AddButton( IDC_AUDIODEMO, L"Audio Demo", 35, iY += 48, 125, 22);
	g_HUD.AddButton( IDC_CLOTHDEMO, L"Cloth Demo", 35, iY += 24, 125, 22);
	g_HUD.AddButton( IDC_TOOGLE_LOOP_LIBS, L"Toogle Auto Test", 35, iY += 24, 125, 22);
	g_HUD.AddButton( IDC_RESET_STATS, L"Reset Min", 35, iY += 24, 125, 22);


    g_SampleUI.SetCallback( OnGUIEvent );

    g_SampleUI.SetFont( 1, L"Courier", 20, FW_NORMAL );
    g_SampleUI.SetFont( 2, L"Courier", 12, FW_NORMAL );

	//audio controls
	for (int ii = 0; ii<g_audioSliderIdSize; ii++)
	{
		int ID = IDC_STATIC_CH_MIN+ii;

		g_SampleUI.AddStatic(	ID, g_audioSliderText[ii],
								cSliderX0, cSliderY0 + ii*(cSliderHeight),
								cSliderWidth,
								cSliderHeight);

		g_SampleUI.GetStatic( ID )->SetTextColor( D3DCOLOR_ARGB( 255, 0, 255, 0 ) ); // Change color to green
		g_SampleUI.GetControl( ID )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP;
	}

	//cloth controls
	for (int ii = 0; ii<g_clothSliderIdSize; ii++)
	{
		int ID = IDC_STATIC_CLOTH_MIN+ii;

		g_SampleUI.AddStatic(	ID, g_clothSliderText[ii],
								cSliderX0, cSliderY0 + ii*(cSliderHeight),
								cSliderWidth,
								cSliderHeight);

		g_SampleUI.GetStatic( ID )->SetTextColor( D3DCOLOR_ARGB( 255, 0, 255, 0 ) ); // Change color to green
		g_SampleUI.GetControl( ID )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP;
	}



	g_SampleUI.AddSlider(	IDC_SLIDER_MATH_REPS,
							cSliderX0+5, 480-60,
							cSliderWidth,
							cSliderHeight,
							0, 100,
							0, false );

	WCHAR wszOutput[1024];
	int reps = GetCurrReps();
	swprintf_s( wszOutput, 1024, L"Calculation Repetitions: %d",  reps);

    g_SampleUI.AddStatic( IDC_OUTPUT_CALC_REPS, wszOutput, cSliderX0, cSliderY0, 620, 40 );
    g_SampleUI.GetStatic( IDC_OUTPUT_CALC_REPS )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 255, 60 ) ); // Change color to yellow
    g_SampleUI.GetControl( IDC_OUTPUT_CALC_REPS )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
    g_SampleUI.GetControl( IDC_OUTPUT_CALC_REPS )->GetElement( 0 )->iFont = 2;


    // Static
	g_SampleUI.AddStatic( IDC_OUTPUT_CALC_STATS, L"Time: ", cSliderX0+100, 480-120, 400, 30 );
	g_SampleUI.GetStatic( IDC_OUTPUT_CALC_STATS )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 32, 32 ) ); // Change color to red
	g_SampleUI.GetControl( IDC_OUTPUT_CALC_STATS )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
	g_SampleUI.GetControl( IDC_OUTPUT_CALC_STATS )->GetElement( 0 )->iFont = 2;


	//avg controls
	LPCWSTR	avgTextArr[] =
	{
		L"VMath   Avg: ",
		L"XNAMath Avg: ",
		L"VClass  Avg: ",
		L"FPU     Avg: ",
	};

	int avgTextIdArr[] =
	{
		IDC_OUTPUT_AVG_STATS_VMATH,
		IDC_OUTPUT_AVG_STATS_XNAMATH,
		IDC_OUTPUT_AVG_STATS_VCLASS,
		IDC_OUTPUT_AVG_STATS_FPU,
	};
	
	for(int ii=0; ii<4; ii++)
	{
		g_SampleUI.AddStatic( avgTextIdArr[ii], avgTextArr[ii], 0, 0, 400, 30 );
		g_SampleUI.GetStatic( avgTextIdArr[ii] )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255) );
		g_SampleUI.GetControl( avgTextIdArr[ii] )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
		g_SampleUI.GetControl( avgTextIdArr[ii] )->GetElement( 0 )->iFont = 2;
	}

	g_SampleUI.AddStatic( IDC_OUTPUT_BAYARD_TITLE, L"", 0, 0, 400, 30 );
	g_SampleUI.GetStatic( IDC_OUTPUT_BAYARD_TITLE )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255) );
	g_SampleUI.GetControl( IDC_OUTPUT_BAYARD_TITLE )->GetElement( 0 )->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
	g_SampleUI.GetControl( IDC_OUTPUT_BAYARD_TITLE )->GetElement( 0 )->iFont = 2;


	g_SampleUI.AddButton( IDC_PLAY_BTN, L"Play", cSliderX0, 250, 80, 20, L'P' );
	g_SampleUI.AddButton( IDC_PAUSE_BTN, L"Pause", cSliderX0+100, 250, 80, 20, L'S' );
	

#ifdef DBUG_EDIT_BOX
    // Edit box
    g_SampleUI.AddEditBox(	IDC_EDITBOX_DBUG, L"", cSliderX0, 440, 600, 32 );
#endif

    // audio sliders
	for (int ii = 0; ii<g_audioSliderIdSize; ii++)
	{
		g_SampleUI.AddSlider(	g_audioSliderId[ii],
								cSliderX0, cSliderY0 + ii*(cSliderHeight),
								cSliderWidth,
								cSliderHeight,
								0, 100, 100, false );
	}

    // cloth sliders
	g_SampleUI.AddSlider(	IDC_SLIDER_CLOTH_ROT, cSliderX0, cSliderY0 + 0*(cSliderHeight),
							cSliderWidth, cSliderHeight,
							0, 100, 0, false );

	g_SampleUI.AddSlider(	IDC_SLIDER_CLOTH_TRANS, cSliderX0, cSliderY0 + 1*(cSliderHeight),
							cSliderWidth, cSliderHeight,
							0, 100, 100, false );

	g_SampleUI.AddSlider(	IDC_SLIDER_CLOTH_GRAVITY, cSliderX0, cSliderY0 + 2*(cSliderHeight),
							cSliderWidth, cSliderHeight,
							0, 100, 100, false );


    // Radio buttons
    g_SampleUI.AddRadioButton( IDC_RADIO_VMATH, 1, L"VMath", 0, 50, 220, 24, true, L'1' );
    g_SampleUI.AddRadioButton( IDC_RADIO_XNAMATH, 1, L"XNAMath", 0, 50, 220, 24, false, L'2' );
    g_SampleUI.AddRadioButton( IDC_RADIO_VCLASS, 1, L"VClass", 0, 50, 220, 24, false, L'3' );
	g_SampleUI.AddRadioButton( IDC_RADIO_FPU, 1, L"FPU", 0, 50, 220, 24, false, L'4' );


	UpdateDemoUI();
	
}


//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    // Must support pixel shader 2.0
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    assert( DXUT_D3D9_DEVICE == pDeviceSettings->ver );

    HRESULT hr;
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    D3DCAPS9 caps;

    V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal,
                            pDeviceSettings->d3d9.DeviceType,
                            &caps ) );

    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
        caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
    {
        pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
        pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext )
{
    HRESULT hr;

    CDXUTIMEEditBox::Initialize( DXUTGetHWND() );

    V_RETURN( g_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnD3D9CreateDevice( pd3dDevice ) );
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                              L"Verdana", &g_pFont ) );

    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
#if defined( DEBUG ) || defined( _DEBUG )
        dwShaderFlags |= D3DXSHADER_DEBUG;
    #endif
#ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
#ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    // Read the D3DX effect file
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"CustomUI.fx" ) );
    V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
                                        NULL, &g_pEffect, NULL ) );

    g_Mesh.Create( pd3dDevice, L"misc\\cell.x" );

    // Setup the camera's view parameters
    D3DXVECTOR3 vecEye( 0.0f, 1.5f, -7.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 0.2f, 0.0f );
    D3DXVECTOR3 vecUp ( 0.0f, 1.0f, 0.0f );
    g_Camera.SetViewParams( &vecEye, &vecAt );
    D3DXMatrixLookAtLH( &g_mView, &vecEye, &vecAt, &vecUp );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice,
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );
    V_RETURN( g_SettingsDlg.OnD3D9ResetDevice() );

    if( g_pFont )
        V_RETURN( g_pFont->OnResetDevice() );
    if( g_pEffect )
        V_RETURN( g_pEffect->OnResetDevice() );

    // Create a sprite to help batch calls when drawing many lines of text
    V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pTextSprite ) );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
    g_HUD.SetSize( 170, 170 );
    g_SampleUI.SetLocation( 0, 0 );
    g_SampleUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

    g_SampleUI.GetControl( IDC_OUTPUT_CALC_REPS )->SetSize( pBackBufferSurfaceDesc->Width - 40, 30 );
	g_SampleUI.GetControl( IDC_OUTPUT_CALC_REPS )->SetLocation( cSliderX0, pBackBufferSurfaceDesc->Height - 80 );

    g_SampleUI.GetControl( IDC_OUTPUT_CALC_STATS )->SetSize( pBackBufferSurfaceDesc->Width - 40, 30 );
	g_SampleUI.GetControl( IDC_OUTPUT_CALC_STATS )->SetLocation( cSliderX0, pBackBufferSurfaceDesc->Height - 120);


	int avgTextIdArr[] =
	{
		IDC_OUTPUT_AVG_STATS_VMATH,
		IDC_OUTPUT_AVG_STATS_XNAMATH,
		IDC_OUTPUT_AVG_STATS_VCLASS,
		IDC_OUTPUT_AVG_STATS_FPU,
	};

	for (int ii=0; ii<4; ii++)
	{
		g_SampleUI.GetControl( avgTextIdArr[ii] )->SetSize( pBackBufferSurfaceDesc->Width - 40, 30 );
		g_SampleUI.GetControl( avgTextIdArr[ii] )->SetLocation( cSliderX0, pBackBufferSurfaceDesc->Height - (200-ii*15));
	}

	g_SampleUI.GetControl( IDC_OUTPUT_BAYARD_TITLE )->SetSize( pBackBufferSurfaceDesc->Width - 40, 30 );
	g_SampleUI.GetControl( IDC_OUTPUT_BAYARD_TITLE )->SetLocation( cSliderX0, pBackBufferSurfaceDesc->Height - 40);



#ifdef DBUG_EDIT_BOX
    g_SampleUI.GetControl( IDC_EDITBOX_DBUG )->SetLocation( cSliderX0, pBackBufferSurfaceDesc->Height -40 );
    g_SampleUI.GetControl( IDC_EDITBOX_DBUG )->SetSize( pBackBufferSurfaceDesc->Width - 40, 32 );
#endif

	g_SampleUI.GetControl( IDC_PLAY_BTN )->SetLocation( cSliderX0, 250 );
	g_SampleUI.GetControl( IDC_PAUSE_BTN )->SetLocation( cSliderX0+100, 250 );

	//g_intValues[0]

	g_SampleUI.GetControl( IDC_SLIDER_MATH_REPS )->SetLocation( cSliderX0+ 5, pBackBufferSurfaceDesc->Height - 60 );

	for (int ii = 0; ii<g_audioSliderIdSize; ii++)
	{
		g_SampleUI.GetControl( g_audioSliderId[ii] )->SetLocation
		(
			cSliderX0, cSliderY0 + ii*(cSliderHeight)
		);
	}


	for (int ii = 0; ii<g_clothSliderIdSize; ii++)
	{
		g_SampleUI.GetControl( g_clothSliderId[ii] )->SetLocation
		(
			cSliderX0, cSliderY0 + ii*(cSliderHeight)
		);
	}
    g_SampleUI.GetControl( IDC_RADIO_VMATH )->SetLocation( pBackBufferSurfaceDesc->Width - 130, 220);
    g_SampleUI.GetControl( IDC_RADIO_XNAMATH )->SetLocation( pBackBufferSurfaceDesc->Width - 130, 220+24 );
    g_SampleUI.GetControl( IDC_RADIO_VCLASS )->SetLocation( pBackBufferSurfaceDesc->Width - 130, 220+48 );
	g_SampleUI.GetControl( IDC_RADIO_FPU )->SetLocation( pBackBufferSurfaceDesc->Width - 130, 220+72 );

    g_Mesh.RestoreDeviceObjects( pd3dDevice );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    D3DXMATRIXA16 m;
    D3DXMatrixRotationY( &m, D3DX_PI * fElapsedTime / 40.0f );
    D3DXMatrixMultiply( &g_mView, &m, &g_mView );
}

//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Creates the scene geometry
//-----------------------------------------------------------------------------
HRESULT InitGeometry(IDirect3DDevice9* pd3dDevice)
{
	if (g_pVB)
	{
		return S_OK;
	}

    // Initialize three vertices for rendering a triangle
    CUSTOMVERTEX g_Vertices[] =
    {
        { -1.0f,-1.0f, 0.0f, 0xffff0000, },
        {  1.0f,-1.0f, 0.0f, 0xff0000ff, },
        {  0.0f, 1.0f, 0.0f, 0xffffffff, },
    };

    // Create the vertex buffer.
    if( FAILED( pd3dDevice->CreateVertexBuffer( 3 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
    {
        return E_FAIL;
    }

    // Fill the vertex buffer.
    VOID* pVertices;
    if( FAILED( g_pVB->Lock( 0, sizeof( g_Vertices ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, g_Vertices, sizeof( g_Vertices ) );
    g_pVB->Unlock();

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Render Hack
//--------------------------------------------------------------------------------------
void RenderHack(IDirect3DDevice9* pd3dDevice, D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView, D3DXMATRIXA16* mProj)
{
	InitGeometry(pd3dDevice);

	if (!g_pVB)
	{
		return;
	}


	// Turn off culling, so we see the front and back of the triangle
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// Turn off D3D lighting, since we are providing our own vertex colors
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	pd3dDevice->SetTransform( D3DTS_WORLD, mWorld );
	pd3dDevice->SetTransform( D3DTS_VIEW, mView );
	pd3dDevice->SetTransform( D3DTS_PROJECTION, mProj );

	// Render the vertex buffer contents
	pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof( CUSTOMVERTEX ) );
	pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );
	//pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, 0, 2 );

	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CleanUpHacks()
{
	CLOTH_VMATH::ClothShutDown();
	CLOTH_XNAMATH::ClothShutDown();
	CLOTH_VCLASS::ClothShutDown();

    if( g_pVB != NULL )
	    g_pVB->Release();

	g_pVB = NULL;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.OnRender( fElapsedTime );
        return;
    }

    HRESULT hr;
    D3DXMATRIXA16 mWorld;
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;
    D3DXMATRIXA16 mWorldViewProjection;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
        // Get the projection & view matrix from the camera class
        mWorld = *g_Camera.GetWorldMatrix();
        mProj = *g_Camera.GetProjMatrix();
        mView = g_mView;

        mWorldViewProjection = mWorld * mView * mProj;

        // Update the effect's variables.  Instead of using strings, it would 
        // be more efficient to cache a handle to the parameter by calling 
        // ID3DXEffect::GetParameterByName
        V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
        V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
        V( g_pEffect->SetFloat( "g_fTime", ( float )fTime ) );

        g_pEffect->SetTechnique( "RenderScene" );
        UINT cPasses;
        g_pEffect->Begin( &cPasses, 0 );
        ID3DXMesh* pMesh = g_Mesh.GetMesh();
        for( UINT p = 0; p < cPasses; ++p )
        {
            g_pEffect->BeginPass( p );
            for( UINT m = 0; m < g_Mesh.m_dwNumMaterials; ++m )
            {
                g_pEffect->SetTexture( "g_txScene", g_Mesh.m_pTextures[m] );
                g_pEffect->CommitChanges();
                pMesh->DrawSubset( m );
            }
            g_pEffect->EndPass();
        }
        g_pEffect->End();

		int reps = GetCurrReps();
		float fps = DXUTGetFPS();
	
		double totalTime = 0.f;
		double timeAvg = 0.f;

		if (g_demoType == DEMO_TYPE_AUDIO)
		{
			if (g_libType == MATHLIB_TYPE_VMATH)
			{
				for(int ii=0; ii<reps; ii++)
				{
					double time = ProcessAudioVMath(2048);
					totalTime += time;
				}
			}
			else if (g_libType == MATHLIB_TYPE_VCLASS)
			{
				for(int ii=0; ii<reps; ii++)
				{
					double time = ProcessAudioVClass(2048);
					totalTime += time;
				}
			}
			else if (g_libType == MATHLIB_TYPE_XNAMATH)
			{
				for(int ii=0; ii<reps; ii++)
				{
					double time = ProcessAudioXNAMath(2048);
					totalTime += time;
				}
			}
			else if (g_libType == MATHLIB_TYPE_FPU)
			{
				for(int ii=0; ii<reps; ii++)
				{
					double time = ProcessAudioFPU(2048);
					totalTime += time;
				}
			}
			else
			{
				assert(false);
			}
		}


		if (g_demoType == DEMO_TYPE_CLOTH)
		{
			float timeStep = 1.f/60.f;
			if (g_libType == MATHLIB_TYPE_VMATH)
			{
				CLOTH_VMATH::ClothAnimateAndRender(pd3dDevice, &mWorld, &mView, &mProj, timeStep, reps, &totalTime);
			}
			else if (g_libType == MATHLIB_TYPE_VCLASS)
			{
				CLOTH_VCLASS::ClothAnimateAndRender(pd3dDevice, &mWorld, &mView, &mProj, timeStep, reps, &totalTime);
			}
			else if (g_libType == MATHLIB_TYPE_XNAMATH)
			{
				CLOTH_XNAMATH::ClothAnimateAndRender(pd3dDevice, &mWorld, &mView, &mProj, timeStep, reps, &totalTime);
			}
			else if (g_libType == MATHLIB_TYPE_FPU)
			{
				
			}
			else
			{
				assert(false);
			}
		}

		timeAvg = totalTime/(double)reps;

		WCHAR wszOutput[1024];
		swprintf_s(wszOutput, 1024,
			L"Total  Calc. Time (ms): %5.6f\nSingle Calc. Time (ms): %5.6f",
			totalTime*1000, timeAvg*1000);
		g_SampleUI.GetStatic( IDC_OUTPUT_CALC_STATS )->SetText(wszOutput);
		UpdateStats(totalTime);

        RenderText();


        V( g_HUD.OnRender( fElapsedTime ) );
        V( g_SampleUI.OnRender( fElapsedTime ) );
        V( pd3dDevice->EndScene() );
    }
}

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void UpdateStats(double totalTime)
{
	WCHAR wszOutput[1024]	 = {0};
	LPCWSTR	avgTextArr[] =
	{
		L"VMath   Avg: %5.6f, Min: %5.6f",
		L"XNAMath Avg: %5.6f, Min: %5.6f",
		L"VClass  Avg: %5.6f, Min: %5.6f",
		L"FPU     Avg: %5.6f, Min: %5.6f",
	};

	int avgTextIdArr[] =
	{
		IDC_OUTPUT_AVG_STATS_VMATH,
		IDC_OUTPUT_AVG_STATS_XNAMATH,
		IDC_OUTPUT_AVG_STATS_VCLASS,
		IDC_OUTPUT_AVG_STATS_FPU,
	};

	static int avgIndexVMath = 0;
	static int avgIndexXNAMath = 0;
	static int avgIndexVClassMath = 0;
	static int avgIndexFPUMath = 0;

	double *pCurrArr = 0;
	int	*pCurr = 0;
	int id = 0;
	double *pMin = 0;

	if (g_libType == MATHLIB_TYPE_VMATH)
	{
		pCurr = &avgIndexVMath;
		pCurrArr = &g_avgVMath[0];
		pMin = &g_avgVMathMin;
		id = 0;
	}
	else if (g_libType == MATHLIB_TYPE_XNAMATH)
	{
		pCurr = &avgIndexXNAMath;
		pCurrArr = &g_avgXNAMath[0];
		pMin = &g_avgXNAMathMin;
		id = 1;
	}
	else if (g_libType == MATHLIB_TYPE_VCLASS)
	{
		pCurr = &avgIndexVClassMath;
		pCurrArr = &g_avgVClass[0];
		pMin = &g_avgVClassMin;
		id = 2;
	}
	else if (g_libType == MATHLIB_TYPE_FPU)
	{
		pCurr = &avgIndexFPUMath;
		pCurrArr = &g_avgFPU[0];
		pMin = &g_avgFPUMin;
		id = 3;
	}


	pCurrArr[*pCurr] = totalTime;

	(*pCurr)++;
	if ((*pCurr) == cAvgSize)
	{
		double avgTmp = 0;
		for(int ii=0; ii<(*pCurr); ii++)
		{
			avgTmp += pCurrArr[ii];
		}

		avgTmp /= (double)cAvgSize;

		if (avgTmp < *pMin)
		{
			*pMin = avgTmp; 
		}

		if ((g_libType == MATHLIB_TYPE_FPU) && (g_demoType == DEMO_TYPE_CLOTH))
		{
			swprintf_s(wszOutput, 1024, L"Not Available");
		}
		else
		{
			swprintf_s(wszOutput, 1024, avgTextArr[id], avgTmp*1000., (*pMin)*1000.);
		}

		g_SampleUI.GetStatic( avgTextIdArr[id] )->SetText(wszOutput);

		(*pCurr) = 0;

		if (g_loopLibs)
		{
			g_libType++;
			if (g_libType > MATHLIB_TYPE_MAX)
			{
				g_libType = MATHLIB_TYPE_MIN;
			}
		}
	}
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 5, 5 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    //txtHelper.DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
	txtHelper.DrawTextLine( DXUTGetFrameStats( true ) );
    txtHelper.DrawTextLine( DXUTGetDeviceStats() );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    txtHelper.DrawTextLine( L"Press ESC to quit" );
    txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    *pbNoFurtherProcessing = CDXUTIMEEditBox::StaticMsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void FromSlidersToEQ()
{
	float pGainArr[g_audioSliderIdSize];

	for (int ii = 0; ii<g_audioSliderIdSize; ii++)
	{
		float value = (float)g_SampleUI.GetSlider( g_audioSliderId[ii] )->GetValue();

		pGainArr[ii] = value / 100.f;
	}

	//that's really nasty!
	extern void UpdateEQ( float *pGainArr);

	UpdateEQ(pGainArr);
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void FromSlidersToCloth()
{
	float rot = (float)g_SampleUI.GetSlider( IDC_SLIDER_CLOTH_ROT )->GetValue();
	rot = (rot/100.f)*180.f;

	float s, k;

	s = (float)g_SampleUI.GetSlider( IDC_SLIDER_CLOTH_TRANS )->GetValue();
	float trans = (s/100.f) * 2.f;

	s = (float)g_SampleUI.GetSlider( IDC_SLIDER_CLOTH_GRAVITY )->GetValue();
	k = -1.5f;
	float gravity = ((2*s*k)/100.f) - k;
	

	if (g_libType == MATHLIB_TYPE_VMATH)
	{
		CLOTH_VMATH::ClothSetGlobalParam(rot, trans, gravity);
	}
	else if (g_libType == MATHLIB_TYPE_VCLASS)
	{
		CLOTH_VCLASS::ClothSetGlobalParam(rot, trans, gravity);
	}
	else if (g_libType == MATHLIB_TYPE_XNAMATH)
	{
		CLOTH_XNAMATH::ClothSetGlobalParam(rot, trans, gravity);
	}
	else
	{
		assert(false);
	}

}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
void ResetStats(void)
{
	g_avgVMathMin	= 
	g_avgXNAMathMin	=
	g_avgVClassMin	=
	g_avgFPUMin		= 999999.f;
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    WCHAR wszOutput[1024];

    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN:
            DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:
            DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:
            g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
		case IDC_AUDIODEMO:
		{
			g_SampleUI.GetStatic( IDC_OUTPUT_BAYARD_TITLE )->SetText(L"You are listening to \"Bayard\" From the Album \"FuZion BraSil II\"");

			g_demoType = DEMO_TYPE_AUDIO;
			ResetStats();
			UpdateStats(0.);
			UpdateDemoUI();
		} break;
		case IDC_CLOTHDEMO:
		{
			g_SampleUI.GetStatic( IDC_OUTPUT_BAYARD_TITLE )->SetText(L"");

			g_demoType = DEMO_TYPE_CLOTH;
			ResetStats();
			UpdateStats(0.);
			UpdateDemoUI();
		} break;

		case IDC_RESET_STATS:
		{
			ResetStats();
		} break;

		case IDC_TOOGLE_LOOP_LIBS:
		{
			g_loopLibs = !g_loopLibs;

			if (g_loopLibs)
			{
				g_libTypeLast = g_libType;
				g_SampleUI.GetRadioButton(IDC_RADIO_VMATH)->SetVisible(false);
				g_SampleUI.GetRadioButton(IDC_RADIO_XNAMATH)->SetVisible(false);
				g_SampleUI.GetRadioButton(IDC_RADIO_VCLASS)->SetVisible(false);
				g_SampleUI.GetRadioButton(IDC_RADIO_FPU)->SetVisible(false);
			}
			else
			{
				g_libType = g_libTypeLast;
				g_SampleUI.GetRadioButton(IDC_RADIO_VMATH)->SetVisible(true);
				g_SampleUI.GetRadioButton(IDC_RADIO_XNAMATH)->SetVisible(true);
				g_SampleUI.GetRadioButton(IDC_RADIO_VCLASS)->SetVisible(true);
				g_SampleUI.GetRadioButton(IDC_RADIO_FPU)->SetVisible(true);
			}

		} break;

		case IDC_PLAY_BTN:
			PlayChannels();
			break;
		case IDC_PAUSE_BTN:
			StopChannels();
			break;
#ifdef DBUG_EDIT_BOX
        case IDC_EDITBOX_DBUG:
		{
			switch( nEvent )
			{
				case EVENT_EDITBOX_STRING:
				{
					LPCWSTR pStr = (( CDXUTEditBox* )pControl )->GetText();
					ParseText(pStr);

				} break;
			}
		} break;
#endif
        case IDC_SLIDER_MATH_REPS:
		{
			//reset stats
			g_avgVMathMin	= 
			g_avgXNAMathMin	=
			g_avgVClassMin	=
			g_avgFPUMin		= 999999.f;

			int reps = GetCurrReps();
			swprintf_s( wszOutput, 1024, L"Calculation Repetitions: %d",  reps);
			g_SampleUI.GetStatic( IDC_OUTPUT_CALC_REPS )->SetText( wszOutput );
		} break;
		case IDC_SLIDER_CH1_LOW:
		case IDC_SLIDER_CH1_MID:
		case IDC_SLIDER_CH1_HIGH:
		case IDC_SLIDER_CH2_LOW:
		case IDC_SLIDER_CH2_MID:
		case IDC_SLIDER_CH2_HIGH:
		case IDC_SLIDER_CH3_LOW:
		case IDC_SLIDER_CH3_MID:
		case IDC_SLIDER_CH3_HIGH:
		case IDC_SLIDER_CH4_LOW:
		case IDC_SLIDER_CH4_MID:
		case IDC_SLIDER_CH4_HIGH:
		{
			FromSlidersToEQ();
		} break;

		case IDC_SLIDER_CLOTH_ROT:
		case IDC_SLIDER_CLOTH_TRANS:
		case IDC_SLIDER_CLOTH_GRAVITY:
		{
			FromSlidersToCloth();
		} break;

        case IDC_RADIO_VMATH:
		{
			g_libType = MATHLIB_TYPE_VMATH;
		}
		break;
        case IDC_RADIO_XNAMATH:
		{
			g_libType = MATHLIB_TYPE_XNAMATH;
		}
		break;
        case IDC_RADIO_VCLASS:
		{
			g_libType = MATHLIB_TYPE_VCLASS;
		}
		break;
        case IDC_RADIO_FPU:
		{
			g_libType = MATHLIB_TYPE_FPU;
		}
		break;

   }
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D9LostDevice();
    g_SettingsDlg.OnD3D9LostDevice();
    g_Mesh.InvalidateDeviceObjects();

    if( g_pFont )
        g_pFont->OnLostDevice();
    if( g_pEffect )
        g_pEffect->OnLostDevice();
    SAFE_RELEASE( g_pTextSprite );
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
    CDXUTIMEEditBox::Uninitialize( );

    g_DialogResourceManager.OnD3D9DestroyDevice();
    g_SettingsDlg.OnD3D9DestroyDevice();
    g_Mesh.Destroy();

	//hacks
	CleanUpHacks();

    SAFE_RELEASE( g_pEffect );
    SAFE_RELEASE( g_pFont );
}



