/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCDX9SwapChains.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 multiple swap chains implementation
 *  Date   : 2013/07/25
 *  Desc   : ASC Direct3D9 multiple swap chains implementation
 *
 *******************************************************************************************/

#pragma once

#include "../ASCConfig.h"

#include <d3d9.h>
#include <vector>
using std::vector;
#include "../ASCTypes.h"
#include "../ASCSwapChains.h"

class CASCDX9SwapChain
{
public:
	CASCDX9SwapChain();
	~CASCDX9SwapChain();

	ASCBoolean Initialize(const PASCSwapChainDesc pUserDesc);
	void Finalize();

	ASCBoolean BeginRender();
	void EndRender();
	ASCBoolean Present();

	ASCBoolean IsInitialized();

	IDirect3DSwapChain9* GetD3DSwapChain9();
	D3DPRESENT_PARAMETERS GetPresentParams();
	IDirect3DSurface9* GetDepthStencil();
private:
	ASCBoolean				m_bInitialized;
	IDirect3DSwapChain9*	m_pD3DSwapChain;
	D3DPRESENT_PARAMETERS	m_PresentParams;
	IDirect3DSurface9*		m_pDepthStencil;
	D3DFORMAT				m_DepthStencilFormat;
	IDirect3DSurface9*		m_pSavedBackBuffer;
	IDirect3DSurface9*		m_pSavedDepthStencil;

	ASCBoolean MakePresentParams(const PASCSwapChainDesc pUserDesc);
	ASCBoolean CreateSwapChain(const PASCSwapChainDesc pUserDesc);
	void DestroySwapChain();
	ASCBoolean CreateDepthStencil(const PASCSwapChainDesc pUserDesc);
	void DestroyDepthStencil();
	ASCBoolean SaveRenderBuffers();
	void RestoreRenderBuffers();
	ASCBoolean SetRenderBuffers();
};

class CASCDX9SwapChains
{
public:
	CASCDX9SwapChains();
	~CASCDX9SwapChains();

	ASCInt Add(const PASCSwapChainDesc pUserDesc);
	ASCBoolean CreateAll(CASCSwapChains* pUserChains);
	void Clear();

	ASCInt GetCount();
	CASCDX9SwapChain* GetItem(ASCInt nIndex);
private:
	vector<CASCDX9SwapChain*>	m_Datas;
};