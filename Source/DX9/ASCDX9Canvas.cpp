#include "ASCDX9Canvas.h"

const ASCDWord ASC_DX9_VERTEX_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

typedef struct CASCDX9VertexRec
{
	D3DVECTOR	Vertex;
	ASCSingle	fRHW;
	ASCUInt32	uColor;
	ASCSingle	fU, fV;
} *PASCDX9VertexRec;

const ASCInt ASC_DX9_VERTEXREC_SIZE = sizeof(CASCDX9VertexRec);

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

CASCDX9Canvas::CASCDX9Canvas() : CASCCanvas()
{
	m_pVertexArray = 0;
	m_pIndexArray = 0;
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
}

CASCDX9Canvas::~CASCDX9Canvas()
{
	DestroyVideoBuffers();
	DestroySystemBuffers();
}

void CASCDX9Canvas::Flush()
{
	if ((m_nVertexCount > 0) && (m_nPrimitives > 0) && (UploadVertexBuffer()) && (UploadIndexBuffer()))
	{
		RenderBuffers();
	}

	m_nVertexCount = 0;
	m_nIndexCount = 0;
	m_nPrimitives = 0;
	m_CanvasTopology = actdx9Unknown;
	m_CachedEffect = abeUnknown;

	if (G_pD3D9Device)
	{
		G_pD3D9Device->SetTexture(0, 0);
	}

	m_pCachedTexture = 0;
	m_pActiveTexture = 0;
}

void CASCDX9Canvas::ResetStates()
{
	m_nVertexCount = 0;
	m_nIndexCount = 0;
	m_nPrimitives = 0;
	m_CanvasTopology = actdx9Unknown;
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

#ifdef NOANTIALIASEDLINES
		G_pD3D9Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, 0);
#else
		G_pD3D9Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, 1);
#endif

		// Enable Alpha-testing
		G_pD3D9Device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
		G_pD3D9Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		G_pD3D9Device->SetRenderState(D3DRS_ALPHAREF, 0x00000001);

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

ASCBoolean CASCDX9Canvas::HandleDeviceCreate()
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

void CASCDX9Canvas::HandleDeviceDestroy()
{
	if (G_D3D9Mode == admDirect3D9Ex)
	{
		DestroyVideoBuffers();
	} 

	DestroySystemBuffers();
}

ASCBoolean CASCDX9Canvas::HandleDeviceReset()
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

void CASCDX9Canvas::HandleDeviceLost()
{
	if (G_D3D9Mode != admDirect3D9Ex)
	{
		DestroyVideoBuffers();
	}
}

void CASCDX9Canvas::HandleBeginScene()
{
	ResetStates();
}

void CASCDX9Canvas::HandleEndScene()
{
	Flush();
}

void CASCDX9Canvas::GetViewport(ASCInt* pX, ASCInt* pY, ASCInt* pWidth, ASCInt* pHeight)
{
	if (!G_pD3D9Device)
	{
		*pX = 0;
		*pY = 0;
		*pWidth = 0;
		*pHeight = 0;
		return;
	}

	D3DVIEWPORT9 vp9;
	memset(&vp9, 0, sizeof(vp9));
	G_pD3D9Device->GetViewport(&vp9);
	*pX = vp9.X;
	*pY = vp9.Y;
	*pWidth = vp9.Width;
	*pHeight = vp9.Height;
}

void CASCDX9Canvas::SetViewport(ASCInt nX, ASCInt nY, ASCInt nWidth, ASCInt nHeight)
{
	if (!G_pD3D9Device)
	{
		return;
	}

	Flush();
	D3DVIEWPORT9 vp9;
	vp9.X = nX;
	vp9.Y = nY;
	vp9.Width = nWidth;
	vp9.Height = nHeight;
	vp9.MinZ = 0.0;
	vp9.MaxZ = 1.0;
	G_pD3D9Device->SetViewport(&vp9);
}

ASCBoolean CASCDX9Canvas::GetAntialias()
{
	if (!G_pD3D9Device)
	{
		return false;
	}

	ASCDWord uVal1, uVal2;
	G_pD3D9Device->GetSamplerState(0, D3DSAMP_MAGFILTER, &uVal1);
	G_pD3D9Device->GetSamplerState(0, D3DSAMP_MINFILTER, &uVal2);
	if ((uVal1 == D3DTEXF_POINT) || (uVal2 == D3DTEXF_POINT))
	{
		return false;
	} 
	else
	{
		return true;
	}
}

void CASCDX9Canvas::SetAntialias(ASCBoolean bValue)
{
	if (!G_pD3D9Device)
	{
		return;
	}

	Flush();
	if (bValue)
	{
		G_pD3D9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		G_pD3D9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	} 
	else
	{
		G_pD3D9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		G_pD3D9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}
}

ASCBoolean CASCDX9Canvas::GetMipMapping()
{
	if (!G_pD3D9Device)
	{
		return false;
	}

	ASCDWord uVal;
	G_pD3D9Device->GetSamplerState(0, D3DSAMP_MIPFILTER, &uVal);
	if ((uVal == D3DTEXF_NONE) || (uVal == D3DTEXF_POINT))
	{
		return false;
	} 
	else
	{
		return true;
	}
}

void CASCDX9Canvas::SetMipMapping(ASCBoolean bValue)
{
	if (!G_pD3D9Device)
	{
		return;
	}

	Flush();
	if (bValue)
	{
		G_pD3D9Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	} 
	else
	{
		G_pD3D9Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	}
}

ASCBoolean CASCDX9Canvas::CreateVideoBuffers()
{
	if (!G_pD3D9Device)
	{
		return false;
	}

	// Dynamic Vertex Buffer
	if (FAILED(G_pD3D9Device->CreateVertexBuffer(m_nVertexCache * ASC_DX9_VERTEXREC_SIZE, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
		ASC_DX9_VERTEX_FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0)))
	{
		return false;
	}

	// Dynamic Index Buffer
	if (FAILED(G_pD3D9Device->CreateIndexBuffer(m_nIndexCache * sizeof(WORD), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_INDEX16,
		D3DPOOL_DEFAULT, &m_pIndexBuffer, 0)))
	{
		return false;
	}

	return true;
}

void CASCDX9Canvas::DestroyVideoBuffers()
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

void CASCDX9Canvas::InitCacheParams()
{
	m_nMaxPrimitives = min(G_D3DCaps9.MaxPrimitiveCount, MAX_CACHED_PRIMITIVES);
	m_nVertexCache = min(G_D3DCaps9.MaxVertexIndex, MAX_CACHED_VERTICES);
	m_nIndexCache = min(G_D3DCaps9.MaxVertexIndex, MAX_CACHED_INDICES);
}

void CASCDX9Canvas::PrepareVertexArray()
{
	PASCDX9VertexRec pEntry = (PASCDX9VertexRec)m_pVertexArray;
	for (ASCInt i = 0; i < MAX_CACHED_VERTICES; i++)
	{
		memset(pEntry, 0, ASC_DX9_VERTEXREC_SIZE);
		pEntry->Vertex.z = 0.0;
		pEntry->fRHW = 1.0;
		pEntry++;
	}
}

void CASCDX9Canvas::CreateSystemBuffers()
{
	m_pVertexArray = realloc(m_pVertexArray, m_nVertexCache * ASC_DX9_VERTEXREC_SIZE);
	memset(m_pVertexArray, 0, m_nVertexCache * ASC_DX9_VERTEXREC_SIZE);

	m_pIndexArray = realloc(m_pIndexArray, m_nIndexCache * sizeof(WORD));
	memset(m_pIndexArray, 0, m_nIndexCache * sizeof(WORD));

	PrepareVertexArray();
}

void CASCDX9Canvas::DestroySystemBuffers()
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

ASCBoolean CASCDX9Canvas::UploadVertexBuffer()
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

ASCBoolean CASCDX9Canvas::UploadIndexBuffer()
{
	if (!m_pIndexBuffer)
	{
		return false;
	}

	ASCInt nBufSize = m_nIndexCount * sizeof(WORD);
	ASCPointer pMemAddr;
	if (FAILED(m_pIndexBuffer->Lock(0, nBufSize, &pMemAddr, D3DLOCK_DISCARD)))
	{
		return false;
	}

	memmove(pMemAddr, m_pIndexArray, nBufSize);
	m_pIndexBuffer->Unlock();

	return true;
}

void CASCDX9Canvas::RenderBuffers()
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
	case actdx9Points:
		G_pD3D9Device->DrawPrimitive(D3DPT_POINTLIST, 0, m_nPrimitives);
		break;
	case actdx9Lines:
		G_pD3D9Device->DrawPrimitive(D3DPT_LINELIST, 0, m_nPrimitives);
		break;
	case actdx9Triangles:
		G_pD3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nVertexCount, 0, m_nPrimitives);
		break;
	default:
		break;
	}

	NextRenderCall();
}

ASCPointer CASCDX9Canvas::NextVertexEntry()
{
	return (ASCPointer)((ASCInt)m_pVertexArray + (m_nVertexCount * ASC_DX9_VERTEXREC_SIZE));
}

void CASCDX9Canvas::AddIndexEntry( ASCInt nIndex )
{
	WORD* pEntry = (WORD*)((ASCInt)m_pIndexArray + (m_nIndexCount * sizeof(WORD)));
	*pEntry = nIndex;
	m_nIndexCount++;
}

ASCBoolean CASCDX9Canvas::RequestCache(CASCDX9CanvasTopology Topology, ASCInt nVertices, ASCInt nIndices, CASCBlendingEffect Effect, CASCTexture* pTexture)
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

	return true;
}

void CASCDX9Canvas::SetEffectStates(CASCBlendingEffect Effect)
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
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		break;
	case abeShadow:
		G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		break;
	case abeAdd:
		G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		break;
	case abeMultiply:
		G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		break;
	case abeSrcColor:
		G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		break;
	case abeSrcColorAdd:
		G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		break;
	case abeInvMultiply:
		G_pD3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		G_pD3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		G_pD3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		break;
	default:
		break;
	}
}

void CASCDX9Canvas::RenderPixel(const CASCFloatVector2D Pt, ASCColor uColor)
{
	if (!RequestCache(actdx9Points, 1, 0, abeNormal, 0))
	{
		return;
	}

	PASCDX9VertexRec pEntry = (PASCDX9VertexRec)NextVertexEntry();
	pEntry->Vertex.x = Pt.X * m_fInternalScale;
	pEntry->Vertex.y = Pt.Y * m_fInternalScale;
	pEntry->uColor = uColor;
	m_nVertexCount++;

	m_nPrimitives++;
}

void CASCDX9Canvas::RenderLine(const CASCFloatVector2D Src, const CASCFloatVector2D Dest, ASCColor uColor1, ASCColor uColor2)
{
	if (!RequestCache(actdx9Lines, 2, 0, abeNormal, 0))
	{
		return;
	}

	PASCDX9VertexRec pEntry = (PASCDX9VertexRec)NextVertexEntry();
	pEntry->Vertex.x = Src.X * m_fInternalScale;
	pEntry->Vertex.y = Src.Y * m_fInternalScale;
	pEntry->uColor = uColor1;
	m_nVertexCount++;

	pEntry = (PASCDX9VertexRec)NextVertexEntry();
	pEntry->Vertex.x = Dest.X * m_fInternalScale;
	pEntry->Vertex.y = Dest.Y * m_fInternalScale;
	pEntry->uColor = uColor2;
	m_nVertexCount++;

	m_nPrimitives++;
}

void CASCDX9Canvas::RenderIndexedTriangles(PASCFloatVector2D pVertices, ASCUInt* pColors, ASCInt* pIndices, 
										   ASCInt nNumVertices, ASCInt nNumTriangles, CASCBlendingEffect Effect /*= abeNormal*/)
{
	if (!RequestCache(actdx9Triangles, nNumVertices, nNumTriangles * 3, Effect, 0))
	{
		return;
	}

	ASCInt* pIndex = pIndices;
	for (ASCInt i = 0; i < nNumTriangles * 3; i++)
	{
		AddIndexEntry(m_nVertexCount + *pIndex);
		pIndex++;
	}

	PASCFloatVector2D pVertex = pVertices;
	ASCUInt* pColor = pColors;
	for (ASCInt i = 0; i < nNumVertices; i++)
	{
		PASCDX9VertexRec pEntry = (PASCDX9VertexRec)NextVertexEntry();
		pEntry->Vertex.x = (ASCSingle)(pVertex->X * m_fInternalScale - 0.5);
		pEntry->Vertex.y = (ASCSingle)(pVertex->Y * m_fInternalScale - 0.5);
		pEntry->uColor = *pColor;
		m_nVertexCount++;

		pVertex++;
		pColor++;
	}

	m_nPrimitives += nNumTriangles;
}

void CASCDX9Canvas::RenderTexture(CASCTexture* pTexture, CASCPoint4 Points, CASCPoint4 Mappings, 
								  CASCColor4 Colors, CASCBlendingEffect Effect /*= abeNormal*/)
{
	m_pActiveTexture = pTexture;
	m_QuadMapping[0] = Mappings[0];
	m_QuadMapping[1] = Mappings[1];
	m_QuadMapping[2] = Mappings[2];
	m_QuadMapping[3] = Mappings[3];

	RequestCache(actdx9Triangles, 4, 6, Effect, m_pActiveTexture);
	
	AddIndexEntry(m_nVertexCount + 2);
	AddIndexEntry(m_nVertexCount);
	AddIndexEntry(m_nVertexCount + 1);

	AddIndexEntry(m_nVertexCount + 3);
	AddIndexEntry(m_nVertexCount + 2);
	AddIndexEntry(m_nVertexCount + 1);

	PASCDX9VertexRec pEntry = (PASCDX9VertexRec)NextVertexEntry();
	pEntry->Vertex.x = (ASCSingle)(Points[0].X * m_fInternalScale - 0.5);
	pEntry->Vertex.y = (ASCSingle)(Points[0].Y * m_fInternalScale - 0.5);
	pEntry->uColor = Colors[0];
	pEntry->fU = m_QuadMapping[0].X;
	pEntry->fV = m_QuadMapping[0].Y;
	m_nVertexCount++;

	pEntry = (PASCDX9VertexRec)NextVertexEntry();
	pEntry->Vertex.x = (ASCSingle)(Points[1].X * m_fInternalScale - 0.5);
	pEntry->Vertex.y = (ASCSingle)(Points[1].Y * m_fInternalScale - 0.5);
	pEntry->uColor = Colors[1];
	pEntry->fU = m_QuadMapping[1].X;
	pEntry->fV = m_QuadMapping[1].Y;
	m_nVertexCount++;

	pEntry = (PASCDX9VertexRec)NextVertexEntry();
	pEntry->Vertex.x = (ASCSingle)(Points[3].X * m_fInternalScale - 0.5);
	pEntry->Vertex.y = (ASCSingle)(Points[3].Y * m_fInternalScale - 0.5);
	pEntry->uColor = Colors[3];
	pEntry->fU = m_QuadMapping[3].X;
	pEntry->fV = m_QuadMapping[3].Y;
	m_nVertexCount++;

	pEntry = (PASCDX9VertexRec)NextVertexEntry();
	pEntry->Vertex.x = (ASCSingle)(Points[2].X * m_fInternalScale - 0.5);
	pEntry->Vertex.y = (ASCSingle)(Points[2].Y * m_fInternalScale - 0.5);
	pEntry->uColor = Colors[2];
	pEntry->fU = m_QuadMapping[2].X;
	pEntry->fV = m_QuadMapping[2].Y;
	m_nVertexCount++;

	m_nPrimitives += 2;
}