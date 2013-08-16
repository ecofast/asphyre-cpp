/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCDX9Canvas.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 canvas implementation
 *  Date   : 2013/07/25
 *  Desc   : General definition of ASC Direct3D9 canvas
 *
 *******************************************************************************************/

#pragma once

#include "../ASCConfig.h"

#include <d3d9.h>
#include "../ASCTypes.h"
#include "../ASCCanvas.h"
#include "../ASCTextures.h"
#include "ASCDX9Common.h"
using std::min;

/*
 * The following option controls the behavior of antialiased lines.
 * Enable the option for compatibility with DirectX 7 wrapper.
 * Also note that antialiased lines don't work on Intel GMA X3100 and have
 * disastrous consequences on Intel HD 3000 video cards
*/
#define NOANTIALIASEDLINES

enum CASCDX9CanvasTopology
{
	actdx9Unknown, actdx9Points, actdx9Lines, actdx9Triangles
};

class CASCDX9Canvas : public CASCCanvas
{
public:
	CASCDX9Canvas();
	~CASCDX9Canvas();

	void Flush();
	virtual void ResetStates();

	virtual void RenderPixel(const CASCFloatVector2D Pt, ASCColor uColor);
	virtual void RenderLine(const CASCFloatVector2D Src, const CASCFloatVector2D Dest, ASCColor uColor1, ASCColor uColor2);
	virtual void RenderIndexedTriangles(PASCFloatVector2D pVertices, ASCUInt* pColors, ASCInt* pIndices,
		ASCInt nNumVertices, ASCInt nNumTriangles, CASCBlendingEffect Effect = abeNormal);
	virtual void RenderTexture(CASCTexture* pTexture, CASCPoint4 Points, CASCPoint4 Mappings, 
		CASCColor4 Colors, CASCBlendingEffect Effect = abeNormal);
protected:
	virtual ASCBoolean HandleDeviceCreate();
	virtual void HandleDeviceDestroy();
	virtual ASCBoolean HandleDeviceReset();
	virtual void HandleDeviceLost();
	virtual void HandleBeginScene();
	virtual void HandleEndScene();

	virtual void GetViewport(ASCInt* pX, ASCInt* pY, ASCInt* pWidth, ASCInt* pHeight);
	virtual void SetViewport(ASCInt nX, ASCInt nY, ASCInt nWidth, ASCInt nHeight);

	virtual ASCBoolean GetAntialias();
	virtual void SetAntialias(ASCBoolean bValue);

	virtual ASCBoolean GetMipMapping();
	virtual void SetMipMapping(ASCBoolean bValue);
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


	ASCBoolean CreateVideoBuffers();
	void DestroyVideoBuffers();

	void InitCacheParams();
	void PrepareVertexArray();

	void CreateSystemBuffers();
	void DestroySystemBuffers();

	ASCBoolean UploadVertexBuffer();
	ASCBoolean UploadIndexBuffer();

	void RenderBuffers();

	ASCPointer NextVertexEntry();
	void AddIndexEntry(ASCInt nIndex);

	ASCBoolean RequestCache(CASCDX9CanvasTopology Topology, ASCInt nVertices, ASCInt nIndices, 
		CASCBlendingEffect Effect, CASCTexture* pTexture);

	void SetEffectStates(CASCBlendingEffect Effect);
};