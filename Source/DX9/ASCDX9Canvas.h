/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCDX9Canvas.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 canvas implementation
 *  Date   : 2013/07/25
 *  Desc   : General definition of ASC Direct3D9 canvas
 *
 *******************************************************************************************/

#pragma once

//#include <algorithm>
using std::min;

#include <d3d9.h>

#include "../ASCTypes.h"
#include "../ASCCanvas.h"
#include "../ASCTextures.h"
#include "ASCDX9Common.h"

#define ASC_DX9_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

typedef struct CASCDX9VertexRec
{
	D3DVECTOR	Vertex;
	ASCSingle	fRHW;
	ASCUInt32	uColor;
	ASCSingle	fU, fV;
} *PASCDX9VertexRec;

const ASCInt ASC_DX9_VERTEXREC_SIZE = sizeof(CASCDX9VertexRec);

/*
 * The following option controls the behavior of antialiased lines.
 * Enable the option for compatibility with DirectX 7 wrapper.
 * Also note that antialiased lines don't work on Intel GMA X3100 and have
 * disastrous consequences on Intel HD 3000 video cards
*/
#define NoAntialiasedLines

/*
 * The following parameters roughly affect the rendering performance. The
 * higher values means that more primitives will fit in cache, but it will
 * also occupy more bandwidth, even when few primitives are rendered.
 * These parameters can be fine-tuned in a finished product to improve the
 * overall performance
*/
const ASCInt MAX_CACHED_PRIMITIVES = 3072;
const ASCInt MAX_CACHED_INDICES    = 4096;
const ASCInt MAX_CACHED_VERTICES   = 4096;

enum CASCDX9CanvasTopology
{
	actUnknown, actPoints, actLines, actTriangles
};

class CASCDX9Canvas : public CASCCanvas
{
public:
	CASCDX9Canvas()
	{
		m_pVertexArray = 0;
		m_pIndexArray = 0;
		m_pVertexBuffer = 0;
		m_pIndexBuffer = 0;
	}

	~CASCDX9Canvas()
	{
		DestroyVideoBuffers();
		DestroySystemBuffers();
	}

	void Flush()
	{
		if ((m_nVertexCount > 0) && (m_nPrimitives > 0) && (UploadVertexBuffer()) && (UploadIndexBuffer()))
		{
			RenderBuffers();
		}

		m_nVertexCount = 0;
		m_nIndexCount = 0;
		m_nPrimitives = 0;
		m_CanvasTopology = actUnknown;
		m_CachedEffect = abeUnknown;

		if (!G_pD3D9Device)
		{
			G_pD3D9Device->SetTexture(0, 0);
		}

		m_pCachedTexture = 0;
		m_pActiveTexture = 0;
	}

	virtual void ResetStates()
	{
		m_nVertexCount = 0;
		m_nIndexCount = 0;
		m_nPrimitives = 0;
		m_CanvasTopology = actUnknown;
		m_CachedEffect = abeUnknown;
		m_pCachedTexture = 0;
		m_pActiveTexture = 0;

		if (!G_pD3D9Device)
		{
			return;
		}
		else
		{
			// Disable pixel shader
			G_pD3D9Device->SetPixelShader(0);

			// Disable unnecessary device states
			G_pD3D9Device->SetRenderState(D3DRS_LIGHTING, 0);
			G_pD3D9Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			G_pD3D9Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			G_pD3D9Device->SetRenderState(D3DRS_FOGENABLE, 0);

		#ifdef NoAntialiasedLines
			G_pD3D9Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, 0);
		#else
			G_pD3D9Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, 1);
		#endif

			// Enable Alpha-testing
			G_pD3D9Device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
			G_pD3D9Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			G_pD3D9Device->SetRenderState(D3DRS_ALPHAREF, $00000001);

			// Default alpha-blending behavior
			G_pD3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

			// Alpha-blending stages for COLOR component
			G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

			G_pD3D9Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			// Alpha-blending stages for ALPHA component
			G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

			G_pD3D9Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

			// Texture filtering flags
			G_pD3D9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			G_pD3D9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			G_pD3D9Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

			// Triangle fill mode
			G_pD3D9Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}
protected:
	virtual ASCBoolean HandleDeviceCreate()
	{
		InitCacheParams();
		CreateSystemBuffers();

		if (G_D3D9Mode  == admDirect3D9Ex)
		{
			return (CreateVideoBuffers());
		} 
		else
		{
			return true;
		}
	}

	virtual void HandleDeviceDestroy()
	{
		if (G_D3D9Mode == admDirect3D9Ex)
		{
			DestroySystemBuffers();
		} 
		
		DestroySystemBuffers();
	}

	virtual ASCBoolean HandleDeviceReset()
	{
		if (G_D3D9Mode != admDirect3D9Ex)
		{
			return (CreateVideoBuffers());
		} 
		else
		{
			return true;
		}
	}

	virtual void HandleDeviceLost()
	{
		if (G_D3D9Mode != admDirect3D9Ex)
		{
			DestroyVideoBuffers();
		}
	}

	virtual void HandleBeginScene()
	{
		ResetStates();
	}

	virtual void HandleEndScene()
	{
		Flush();
	}
private:
	IDirect3DVertexBuffer9*		m_pVertexBuffer;
	IDirect3DIndexBuffer9*		m_pIndexBuffer;
	ASCPointer					m_pVertexArray;
	ASCPointer					m_pIndexArray;

	ASCInt						m_nVertexCache;
	ASCInt						m_nIndexCache;
	ASCInt						m_nVertexCount;
	ASCInt						m_nIndexCount;
	ASCInt						m_nPrimitives;
	ASCInt						m_nMaxPrimitives;

	CASCTexture*				m_pActiveTexture;
	CASCTexture*				m_pCachedTexture;
	CASCBlendingEffect			m_CachedEffect;

	CASCPoint4					m_QuadMapping;

	CASCDX9CanvasTopology		m_CanvasTopology;


	ASCBoolean CreateVideoBuffers()
	{
		if (!G_pD3D9Device)
		{
			return false;
		}

		// Dynamic Vertex Buffer
		if (FAILED(G_pD3D9Device->CreateVertexBuffer(m_nVertexCache * ASC_DX9_VERTEXREC_SIZE, D3DUSAGE_WRITEONLY or D3DUSAGE_DYNAMIC, 
			ASC_DX9_VERTEX_FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0)))
		{
			return false;
		}

		// Dynamic Index Buffer
		if (FAILED(G_pD3D9Device->CreateIndexBuffer(m_nIndexCache * sizeof(WORD), D3DUSAGE_WRITEONLY or D3DUSAGE_DYNAMIC, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIndexBuffer, 0)))
		{
			return false;
		}

		return true;
	}

	void DestroyVideoBuffers()
	{
		if (m_pIndexBuffer)
		{
			m_pIndexBuffer = 0;
		}

		if (m_pVertexBuffer)
		{
			m_pVertexBuffer = 0;
		}
	}

	void InitCacheParams()
	{
		m_nMaxPrimitives = min(G_D3DCaps9.MaxPrimitiveCount, MAX_CACHED_PRIMITIVES);
		m_nVertexCache = min(G_D3DCaps9.MaxVertexIndex, MAX_CACHED_VERTICES);
		m_nIndexCache = min(G_D3DCaps9.MaxVertexIndex, MAX_CACHED_INDICES);
	}

	void PrepareVertexArray()
	{
		PASCDX9VertexRec pEntry = m_pVertexArray;
		for (ASCInt i = 0; i < MAX_CACHED_VERTICES; i++)
		{
			memset(pEntry, 0, ASC_DX9_VERTEXREC_SIZE);
			pEntry->Vertex.z = 0.0;
			pEntry->fRHW = 1.0;
			pEntry++;
		}
	}

	void CreateSystemBuffers()
	{
		realloc(m_pVertexArray, m_nVertexCache * ASC_DX9_VERTEXREC_SIZE);
		memset(m_pVertexArray, 0, m_nVertexCache * ASC_DX9_VERTEXREC_SIZE);

		realloc(m_pIndexArray, m_nIndexCache * sizeof(WORD));
		memset(m_pIndexArray, m_nIndexCache * sizeof(WORD));

		PrepareVertexArray();
	}

	void DestroySystemBuffers()
	{
		if (m_pIndexArray)
		{
			free(m_pIndexArray);
			m_pIndexArray = 0;
		}

		if (m_pVertexArray)
		{
			free(m_pVertexArray);
			m_pVertexArray = 0;
		}
	}

	ASCBoolean UploadVertexBuffer()
	{
		if (!m_pVertexBuffer)
		{
			return false;
		}

		ASCInt nBufSize = m_nVertexCount * ASC_DX9_VERTEXREC_SIZE;
		ASCPointer pMemAddr;
		if (FAILED(m_pVertexBuffer->Lock(0, nBufSize, &pMemAddr, D3DLOCK_DISCARD)))
		{
			return false;
		}
		
		memmove(pMemAddr, m_pVertexArray, nBufSize);
		m_pVertexBuffer->Unlock();

		return true;
	}

	ASCBoolean UploadIndexBuffer()
	{
		if (!m_pIndexBuffer)
		{
			return false;
		}

		ASCInt nBufSize = m_IndexCount * sizeof(WORD);
		ASCPointer pMemAddr;
		if (FAILED(m_pIndexBuffer->Lock(0, nBufSize, &pMemAddr, D3DLOCK_DISCARD)))
		{
			return false;
		}

		memmove(pMemAddr, m_pVertexArray, nBufSize);
		m_pIndexBuffer->Unlock();

		return true;
	}

	void RenderBuffers()
	{
		if (!G_pD3D9Device)
		{
			return;
		}

		G_pD3D9Device->SetStreamSource(0, m_pVertexBuffer, 0, ASC_DX9_VERTEXREC_SIZE);
		G_pD3D9Device->SetIndices(m_pIndexBuffer);
		G_pD3D9Device->SetVertexShader(0);
		G_pD3D9Device->SetFVF(ASC_DX9_VERTEX_FVF);
		switch (m_CanvasTopology)
		{
		case actPoints:
			G_pD3D9Device->DrawPrimitive(D3DPT_POINTLIST, 0, m_nPrimitives);
			break;
		case actLines:
			G_pD3D9Device->DrawPrimitive(D3DPT_LINELIST, 0, m_nPrimitives);
			break;
		case actTriangles:
			G_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nVertexCount, 0, m_nPrimitives);
			break;
		default:
			break;
		}

		NextRenderCall();
	}

	ASCPointer NextVertexEntry()
	{
		return (ASCPointer)((ASCInt)m_pVertexArray + (m_nVertexCount * ASC_DX9_VERTEXREC_SIZE));
	}

	void AddIndexEntry(ASCInt nIndex)
	{
		WORD* pEntry = (ASCPointer)((ASCInt)m_pIndexArray + (m_nIndexCount * sizeof(WORD)));
		*pEntry = nIndex;
		m_nIndexCount++;
	}

	ASCBoolean RequestCache(CASCDX9CanvasTopology Topology, ASCInt nVertices, ASCInt nIndices, 
		CASCBlendingEffect Effect, const CASCTexture* pTexture)
	{
		if (!((nVertices <= MAX_CACHED_VERTICES) && (nIndices <= MAX_CACHED_INDICES)))
		{
			return false;
		}

		ASCBoolean bNeedReset = (m_nVertexCount + nVertices > m_nVertexCache);
		bNeedReset = (bNeedReset || (m_nIndexCount + nIndices > m_nIndexCache));
		bNeedReset = (bNeedReset || (m_CanvasTopology == actUnknown) || (m_CanvasTopology != Topology));
		bNeedReset = (bNeedReset || (m_CachedEffect == abeUnknown) || (m_CachedEffect != Effect));
		bNeedReset = (bNeedReset || (m_pCachedTexture != pTexture));

		if (bNeedReset)
		{
			Flush();

			if ((m_CachedEffect == abeUnknown) || (m_CachedEffect != Effect))
			{
				SetEffectStates(Effect);
			}

			if (G_pD3D9Device && ((m_CachedEffect == abeUnknown) || (m_pCachedTexture != pTexture)))
			{
				if (pTexture)
				{
					pTexture->Bind(0);
				}
				else
				{
					G_pD3D9Device->SetTexture(0, 0);
				}
			}

			m_CanvasTopology = Topology;
			m_CachedEffect = Effect;
			m_pCachedTexture = pTexture;
		}
	}

	void SetEffectStates(CASCBlendingEffect Effect)
	{
		if (!G_pD3D9Device)
		{
			return;
		}

		switch (Effect)
		{
		case abeNormal:
			G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			G_pD3D9Device->SetRenderState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			G_pD3D9Device->SetRenderState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
		case abeShadow:
			G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			G_pD3D9Device->SetRenderState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			G_pD3D9Device->SetRenderState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
		case abeAdd:
			G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			G_pD3D9Device->SetRenderState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			G_pD3D9Device->SetRenderState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
		case abeMultiply:
			G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
			G_pD3D9Device->SetRenderState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			G_pD3D9Device->SetRenderState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
		case abeSrcColor:
			G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			G_pD3D9Device->SetRenderState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			G_pD3D9Device->SetRenderState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
		case abeSrcColorAdd:
			G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			G_pD3D9Device->SetRenderState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			G_pD3D9Device->SetRenderState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
		case abeInvMultiply:
			G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			G_pD3D9Device->SetRenderState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			G_pD3D9Device->SetRenderState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			break;
		default:
			break;
		}
	}
};