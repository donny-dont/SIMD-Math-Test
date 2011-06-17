//--------------------------------------------------------------------------------------
//	File: cloth.cpp
//
//	Original algorithm extracted from the white paper:
//	Advanced Character Physics by Thomas Jakobsen
//	http://www.teknikus.dk/tj/gdc2001.htm
///////////////////////////////////////////////////////////////////////////////


#include "DXUT.h"
#include <xaudio2.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include "_xnamath_.h"
#include <math.h>
#include "common.h"
#include "cloth.h"

///////////////////////////////////////////////////////////////////////////////
//								Consts & Defines
///////////////////////////////////////////////////////////////////////////////

#define	XNAMATH_PI				(3.1415926535897932384626433832795f)
#define	XNAMATH_DTOR(angle)		((XNAMATH_PI/180.0f)*((float)angle))
#define	XNAMATH_RTOD(angle)		((180.0f/XNAMATH_PI)*((float)angle))

namespace CLOTH_XNAMATH
{

	const int	cClothMaxConstraints	= 16;

	#ifdef NDEBUG
		const int	cClothWidth				= 65;
		const int	cClothHeight			= 65;
	#else
		const int	cClothWidth				= 20;
		const int	cClothHeight			= 20;
	#endif

	const int	cClothSize				= cClothWidth*cClothHeight;
	const float	cClothRestLength		= (1.f/(float)(cClothWidth))*5.f;
	const int	cIndicesArrSize			= 32768;


	///////////////////////////////////////////////////////////////////////////////
	//								Structs
	///////////////////////////////////////////////////////////////////////////////

	typedef struct ClothConstraints
	{
		int				cIndex[cClothMaxConstraints];
		int				cIndexCount;

	}	ClothConstraints, *PClothConstraints;

	typedef struct Cloth
	{
		XMVECTOR					m_x[cClothSize];
		XMVECTOR					m_oldx[cClothSize];
		XMVECTOR					m_a[cClothSize];
		XMVECTOR					m_vGravity;
		XMVECTOR					fTimeStep;
		XMVECTOR					restlength;
		XMVECTOR					hook[2];
		XMVECTOR					worldTrans;

		bool						clothInit;
		float						rot;
		float						dist;

		ClothConstraints			cnstr[cClothSize];
		int							NUM_ITERATIONS;
		int							NUM_PARTICLES;

		LPDIRECT3DVERTEXBUFFER9		pVB;
		LPDIRECT3DINDEXBUFFER9		pVI;
		int							vertCount;
		int							primCount;

		void AccumulateForces();
		void Verlet();
		void SatisfyConstraints();
		void TimeStep();

	}	Cloth, *PCloth;


	///////////////////////////////////////////////////////////////////////////////
	//								Globals
	///////////////////////////////////////////////////////////////////////////////

	__declspec(align(128))	Cloth	g_cloth;


	///////////////////////////////////////////////////////////////////////////////
	//								Functions
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	// Get cloth index
	///////////////////////////////////////////////////////////////////////////////
	inline int GetI(int x, int y)
	{
		return((y*cClothWidth) + x);
	}

	///////////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////////
	HRESULT ClothInit(void)
	{
		HRESULT hr = S_OK;

		int w = cClothWidth;
		int h = cClothHeight;

		memset(&g_cloth, 0x00, sizeof(g_cloth));

		//g_cloth.worldTrans = XMVectorSet(1.f, 2.f, 0.f, 0.f);
		g_cloth.worldTrans = XMVectorSet(0.f, 0.f, 2.f, 1.f);

		//build patch constraints
		for(int yy=0; yy<=h-2; yy++)
		{
			for(int xx=0; xx<=w-2; xx++)
			{
				/////////////////////
				// (xx,yy)----(xx+1,yy)
				//  !
				//  !
				// (xx,yy+1)
				int ii, jj;
				jj = 0; ii = GetI(xx, yy);
				g_cloth.cnstr[ii].cIndex[jj++] = GetI(xx+1, yy);
				g_cloth.cnstr[ii].cIndex[jj++] = GetI(xx, yy+1);
				g_cloth.cnstr[ii].cIndexCount = jj;
			}
		}

		//bottom gaps
		for(int xx=0; xx<=w-2; xx++)
		{
			/////////////////////
			// (xx,h-1)----(xx+1,h-1)
			int ii, jj;
			ii = GetI(xx, h-1);
			jj = g_cloth.cnstr[ii].cIndexCount;
			assert(jj == 0);
			g_cloth.cnstr[ii].cIndex[jj++] = GetI(xx+1, h-1);
			g_cloth.cnstr[ii].cIndexCount = jj;
		}

		//right gaps
		for(int yy=0; yy<=h-2; yy++)
		{
			/////////////////////
			// (w-1,yy)
			//  !
			//  !
			// (w-1,yy+1)
			int ii, jj;
			ii = GetI(w-1, yy);
			jj = g_cloth.cnstr[ii].cIndexCount;
			assert(jj == 0);
			g_cloth.cnstr[ii].cIndex[jj++] = GetI(w-1, yy+1);
			g_cloth.cnstr[ii].cIndexCount = jj;
		}


		//setup intial rest points
		for(int yy=0; yy<=h-1; yy++)
		{
			for(int xx=0; xx<=w-1; xx++)
			{
				int	 ii = GetI(xx, yy);

				float	x = ((float)xx)*cClothRestLength;
				float	y = ((float)yy)*cClothRestLength;

				//XMVECTOR	localPos = XMVectorSet(x, y, 0.f, 0.f);
				XMVECTOR	localPos = XMVectorSet(0.f, 0.f, y, x);

				g_cloth.m_x[ii] = 
				g_cloth.m_oldx[ii] = localPos + g_cloth.worldTrans;
			}
		}

		g_cloth.hook[1] = g_cloth.m_x[0];
		g_cloth.hook[0] = g_cloth.m_x[cClothWidth-1];

		//test
		XMVECTOR	diff = g_cloth.hook[1] - g_cloth.hook[0];
		XMVECTOR	d = XMVectorSqrt(XMVector4Dot(diff,diff))*XMVectorReplicate(0.5f);
		g_cloth.dist = XMVectorGetX(d);


		//other inital values
		//g_cloth.m_vGravity = XMVectorSet(0.f, -1.5f, 0.f, 0.f);
		g_cloth.m_vGravity = XMVectorSet(0.f, 0.f, -1.5f, 0.f);
		g_cloth.NUM_PARTICLES = cClothSize;
		g_cloth.NUM_ITERATIONS = CLOTH_NUM_ITERATIONS;
		g_cloth.restlength = XMVectorReplicate(cClothRestLength);


		//setup index buffers
		WORD indicesArr[cIndicesArrSize];		//nasty, create tmp array on stack
		memset(indicesArr, 0x00, sizeof(indicesArr));
		IDirect3DDevice9* pd3dDevice = DXUTGetD3D9Device();

		if( FAILED( pd3dDevice->CreateIndexBuffer( cIndicesArrSize*sizeof(WORD),		//2-bytes per index
			D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, 
			&g_cloth.pVI, NULL ) ) )
		{

			return (E_FAIL);
		}

		//p0------p1
		//!       !
		//!       !
		//!       !
		//p2------p3
		int ii=0;
		for(int yy=0; yy<=h-2; yy++)
		{
			for(int xx=0; xx<=w-2; xx++)
			{
				//p0-p1
				indicesArr[ii++] = GetI(xx, yy);
				indicesArr[ii++] = GetI(xx+1, yy);
				g_cloth.primCount++;

				//p1-p3
				indicesArr[ii++] = GetI(xx+1, yy);
				indicesArr[ii++] = GetI(xx+1, yy+1);
				g_cloth.primCount++;

				//p3-p2
				indicesArr[ii++] = GetI(xx+1, yy+1);
				indicesArr[ii++] = GetI(xx, yy+1);
				g_cloth.primCount++;

				//p2-p0
				indicesArr[ii++] = GetI(xx, yy+1);
				indicesArr[ii++] = GetI(xx, yy);
				g_cloth.primCount++;
			}
		}

		VOID* pIndices = 0;

		hr = g_cloth.pVI->Lock(0, sizeof(indicesArr), (VOID**)&pIndices, 0 );
		if (FAILED(hr))
		{
			SAFE_RELEASE(g_cloth.pVI);
			return (E_FAIL);
		}

		int size = sizeof(indicesArr);
		memcpy( pIndices, indicesArr, size );
		g_cloth.pVI->Unlock();

		// Create the vertex buffer.
		g_cloth.vertCount = ii;
		if( FAILED( pd3dDevice->CreateVertexBuffer
			(
				g_cloth.vertCount * sizeof( CUSTOMVERTEX ),
				0, D3DFVF_CUSTOMVERTEX,
				D3DPOOL_DEFAULT, &g_cloth.pVB, NULL
			) ) )
		{
			return (E_FAIL);
		}

		ClothCopyVertices(true);
		return(hr);
	}

	///////////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////////
	void ClothShutDown(void)
	{
		SAFE_RELEASE(g_cloth.pVB);
		SAFE_RELEASE(g_cloth.pVI);
		memset(&g_cloth, 0x00, sizeof(g_cloth));
	}


	///////////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////////
	HRESULT ClothCopyVertices(bool addColor)
	{
		HRESULT hr = S_OK;

		int w = cClothWidth;
		int h = cClothHeight;

		// Fill the vertex buffer.
		VOID* pVertices = 0;

		if( FAILED(
			g_cloth.pVB->Lock ( 0, g_cloth.vertCount * sizeof( CUSTOMVERTEX ), ( void** )&pVertices, 0 )
			) )
		{
			return E_FAIL;
		}

		//FLOAT x, y, z;      // The untransformed, 3D position for the vertex
		//DWORD color;        // The vertex color

		//memcpy( pVertices, g_Vertices, sizeof( g_Vertices ) );
		CUSTOMVERTEX*	pV = (CUSTOMVERTEX*)pVertices;

		int kk=0;
		if (addColor)
		{
			for(int yy=0; yy<=h-1; yy++)
			{
				for(int xx=0; xx<=w-1; xx++)
				{
					int ii = GetI(xx, yy);
					__declspec(align(128)) float	f[4];

					//VStore(f, g_cloth.m_x[ii]);
					//g_cloth.m_x[ii].Store(f);
					XMStoreFloat4A((XMFLOAT4A*)f, g_cloth.m_x[ii]);

					pV[kk].x = f[3];
					pV[kk].y = f[2];
					pV[kk].z = f[1];

					//add some cool colors since I won't lit the patches
					float	xF = (1.f-((float)xx * ((float)1.f/(w-1))))*255.f;
					float	yF = (1.f-((float)yy * ((float)1.f/(h-1))))*255.f;

					
					pV[kk].color = D3DCOLOR_ARGB(0xff, (short)xF, (short)yF, 0xff);
					kk++;
				}
			}
		}
		else
		{
			for(int yy=0; yy<=h-1; yy++)
			{
				for(int xx=0; xx<=w-1; xx++)
				{
					int ii = GetI(xx, yy);
					__declspec(align(128)) float	f[4];

					//VStore(f, g_cloth.m_x[ii]);
					//g_cloth.m_x[ii].Store(f);
					XMStoreFloat4A((XMFLOAT4A*)f, g_cloth.m_x[ii]);

					pV[kk].x = f[3];
					pV[kk].y = f[2];
					pV[kk].z = f[1];
					kk++;
				}
			}
		}


		g_cloth.pVB->Unlock();

		return(hr);
	}

	///////////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////////
	HRESULT ClothAnimateAndRender(IDirect3DDevice9* pd3dDevice, D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView, D3DXMATRIXA16* mProj, float fTimeStep, int reps, double *totalTimeOut)
	{
		HRESULT hr = S_OK;

		if (!g_cloth.clothInit)
		{
			ClothInit();
			g_cloth.clothInit = !g_cloth.clothInit;
		}

		ClothUIHack();
		g_cloth.fTimeStep = XMVectorReplicate(fTimeStep);

		*totalTimeOut = 0.;

		for(int ii=0; ii<reps; ii++)
		{
			double time;

			PerformanceCounterStart();

			g_cloth.TimeStep();

			time = PerformanceCounterEnd();

			*totalTimeOut += time;
		}

		ClothCopyVertices(false);

		// Turn off culling, so we see the front and back of the triangle
		pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

		// Turn off D3D lighting, since we are providing our own vertex colors
		pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

		pd3dDevice->SetTransform( D3DTS_WORLD, mWorld );
		pd3dDevice->SetTransform( D3DTS_VIEW, mView );
		pd3dDevice->SetTransform( D3DTS_PROJECTION, mProj );

		// Render the vertex buffer contents
		pd3dDevice->SetStreamSource( 0, g_cloth.pVB, 0, sizeof( CUSTOMVERTEX ) );
		pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		pd3dDevice->SetIndices( g_cloth.pVI);
		pd3dDevice->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, g_cloth.vertCount, 0,  g_cloth.primCount);

		pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

		return(hr);
	}

	void ClothUIHack(void)
	{
		float	s = sinf(g_cloth.rot);
		float	c = cosf(g_cloth.rot);

		float	x0 = -g_cloth.dist;
		float	x0r = x0*c;
		float	z0r = x0*s;

		float	x1 = +g_cloth.dist;
		float	x1r = x1*c;
		float	z1r = x1*s;

		//XMVECTOR	pa = XMVectorSet(x0r, 0.f, z0r, 0.f) + g_cloth.worldTrans;
		XMVECTOR	pa = XMVectorSet(0.f, z0r, 0.f, x0r) + g_cloth.worldTrans;
		//XMVECTOR	pb = XMVectorSet(x1r, 0.f, z1r, 0.f) + g_cloth.worldTrans;
		XMVECTOR	pb = XMVectorSet(0.f, z1r, 0.f, x1r) + g_cloth.worldTrans;

		g_cloth.hook[0] = pa;
		g_cloth.hook[1] = pb;
	}

	void ClothSetGlobalParam(float rot, float trans, float gravity)
	{
		g_cloth.rot = XNAMATH_DTOR(rot);
		//g_cloth.worldTrans = XMVectorSet(1.f, trans, 0.f, 0.f);
		g_cloth.worldTrans = XMVectorSet(0.f, 0.f, trans, 1.f);
		//g_cloth.m_vGravity = XMVectorSet(0.f, gravity, 0.f, 0.f);
		g_cloth.m_vGravity = XMVectorSet(0.f, 0.f, gravity, 0.f);
	}


	///////////////////////////////////////////////////////////////////////////////
	//							Simulation Code
	///////////////////////////////////////////////////////////////////////////////

	// Verlet integration step
void Cloth::Verlet()
{
	XMVECTOR	d1 = XMVectorReplicate(0.99903f);		
	XMVECTOR	d2 = XMVectorReplicate(0.99899f); //had to change the constant so the optimizer does not unify the code

	for(int i=0; i<NUM_PARTICLES; i++)
	{
		XMVECTOR& x = m_x[i];
		XMVECTOR temp = x;
		XMVECTOR& oldx = m_oldx[i];
		XMVECTOR& a = m_a[i];
		x += (d1*x)-(d2*oldx)+a*fTimeStep*fTimeStep;
		oldx = temp;
	}
}

	// This function should accumulate forces for each particle
	void Cloth::AccumulateForces()
	{    
		// All particles are influenced by gravity
		for(int i=0; i<NUM_PARTICLES; i++)  m_a[i] = m_vGravity;
	}


// Here constraints should be satisfied
void Cloth::SatisfyConstraints()
{
	XMVECTOR	half = XMVectorReplicate(0.5f);

	// Implements simulation of a stick in a box
	for(int j=0; j<NUM_ITERATIONS; j++)
	{
		// First satisfy (C1)
		m_x[0] = hook[0];
		m_x[cClothWidth-1] = hook[1];

		// For all particles
		for(int i=0; i<NUM_PARTICLES-1; i++)
		{
			// Then satisfy (C2)
			XMVECTOR& x1 = m_x[i];

			for(int cc=0; cc<cnstr[i].cIndexCount; cc++)
			{
				//get 2nd particle index
				int i2 = cnstr[i].cIndex[cc];

				XMVECTOR& x2 = m_x[i2];
				XMVECTOR delta = x2-x1;
				XMVECTOR deltalength = XMVectorSqrt(XMVector4Dot(delta,delta));
				XMVECTOR diff = (deltalength-restlength)/deltalength;
				x1 += delta*half*diff;
				x2 -= delta*half*diff;
			}
		}
	}
}

	void Cloth::TimeStep()
	{
		AccumulateForces();
		Verlet();
		SatisfyConstraints();
	}
}
