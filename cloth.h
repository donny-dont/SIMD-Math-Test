//--------------------------------------------------------------------------------------
// File: cloth.h
//--------------------------------------------------------------------------------------

#ifndef __CLOTH__
#define __CLOTH__


#define	CLOTH_NUM_ITERATIONS		(8)

///////////////////////////////////////////////////////////////////////////////
//								Externs
///////////////////////////////////////////////////////////////////////////////

namespace CLOTH_VCLASS
{
	extern HRESULT ClothInit(void);
	extern void ClothShutDown(void);
	extern HRESULT ClothCopyVertices(bool addColor);
	extern HRESULT ClothAnimateAndRender(IDirect3DDevice9* pd3dDevice, D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView, D3DXMATRIXA16* mProj, float fTimeStep, int reps, double *totalTimeOut);
	extern void ClothSetGlobalParam(float rot, float trans, float gravity);
	extern void ClothUIHack(void);
}

namespace CLOTH_VMATH
{
	extern HRESULT ClothInit(void);
	extern void ClothShutDown(void);
	extern HRESULT ClothCopyVertices(bool addColor);
	extern HRESULT ClothAnimateAndRender(IDirect3DDevice9* pd3dDevice, D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView, D3DXMATRIXA16* mProj, float fTimeStep, int reps, double *totalTimeOut);
	extern void ClothSetGlobalParam(float rot, float trans, float gravity);
	extern void ClothUIHack(void);
}

namespace CLOTH_XNAMATH
{
	extern HRESULT ClothInit(void);
	extern void ClothShutDown(void);
	extern HRESULT ClothCopyVertices(bool addColor);
	extern HRESULT ClothAnimateAndRender(IDirect3DDevice9* pd3dDevice, D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView, D3DXMATRIXA16* mProj, float fTimeStep, int reps, double *totalTimeOut);
	extern void ClothSetGlobalParam(float rot, float trans, float gravity);
	extern void ClothUIHack(void);
}


#endif // #ifndef __CLOTH__