/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCDX9SwapChains.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 multiple swap chains implementation
 *  Date   : 2013/07/25
 *  Desc   : ASC Direct3D9 multiple swap chains implementation
 *
 *******************************************************************************************/

#pragma once

#include <d3d9.h>

#include "../ASCTypes.h"
#include "../ASCSwapChains.h"

class CASCDX9SwapChain
{
public:
	CASCDX9SwapChain()
	{
		m_bInitialized			= false;
		m_pD3DSwapChain			= 0;
		m_DepthStencilFormat	= D3DFMT_UNKNOWN;
		memset(&m_PresentParams, 0, sizeof(m_PresentParams));
	}

	~CASCDX9SwapChain()
	{
		if (m_bInitialized)
		{
			Finalize();
		}
	}

	ASCBoolean Initialize(const PASCSwapChainDesc pUserDesc)
	{
		ASCBoolean bResult = ((!m_bInitialized) && (pUserDesc));
		if (!bResult)
		{
			return false;
		}

		bResult = MakePresentParams(pUserDesc);
		if (!bResult)
		{
			return false;
		}

		bResult = CreateSwapChain(pUserDesc);
		if (!bResult)
		{
			return false;
		}

		bResult = CreateDepthStencil(pUserDesc);
		if (!bResult)
		{
			DestroySwapChain();
			return false;
		}

		m_bInitialized = true;
	}

	void Finalize()
	{
		if (!m_bInitialized)
		{
			return;
		}

		if (m_pSavedBackBuffer)
		{
			m_pSavedBackBuffer = 0;
		}

		if (m_pSavedDepthStencil)
		{
			m_pSavedDepthStencil = 0;
		}

		DestroyDepthStencil();
		DestroySwapChain();

		m_bInitialized = false;
	}

	ASCBoolean BeginRender()
	{
		ASCBoolean bResult ;//= ((m_pD3DSwapChain) && (G_pD3D9Device));
		if (!bResult)
		{
			return false;
		}

		bResult = SaveRenderBuffers();
		if (!bResult)
		{
			return false;
		}

		bResult = SetRenderBuffers();
		if (!bResult)
		{
			RestoreRenderBuffers();
			return false;
		}

		return true;
	}

	void EndRender()
	{
		RestoreRenderBuffers();
	}

	ASCBoolean Present()
	{
		if (!m_pD3DSwapChain)
		{
			return false;
		}
		else
		{
			return SUCCEEDED(m_pD3DSwapChain->Present(0, 0, 0, 0, 0));
		}
	}

	ASCBoolean IsInitialized()
	{
		return m_bInitialized;
	}

	IDirect3DSwapChain9* GetD3DSwapChain9()
	{
		return m_pD3DSwapChain9;
	}

	D3DPRESENT_PARAMETERS GetPresentParams()
	{
		return m_PresentParams;
	}

	IDirect3DSurface9* GetDepthStencil()
	{
		return m_pDepthStencil;
	}
private:
	ASCBoolean				m_bInitialized;
	IDirect3DSwapChain9*	m_pD3DSwapChain;
	D3DPRESENT_PARAMETERS	m_PresentParams;
	IDirect3DSurface9*		m_pDepthStencil;
	D3DFORMAT				m_DepthStencilFormat;
	IDirect3DSurface9*		m_pSavedBackBuffer;
	IDirect3DSurface9*		m_pSavedDepthStencil;

	ASCBoolean MakePresentParams(const PASCSwapChainDesc pUserDesc)
	{

	}

	ASCBoolean CreateSwapChain(const PASCSwapChainDesc pUserDesc)
	{
		// 1) Verify initial conditions
		ASCBoolean bResult = (pUserDesc && G_pD3D9Device && (m_PresentParams.BackBufferWidth > 0) && (m_PresentParams.BackBufferHeight > 0));
		if (!bResult)
		{
			return false;
		}

		// 2) Create additional swap chain
		// bResult = SUCCEEDED(G_pD3D9Device.CreateAdditionalSwapChain(m_PresentParams, m_pD3DSwapChain));

		// 3) Update the description of the first swap chain
		if (bResult)
		{
			// pUserDesc->Format = DX9FormatToAsphyre(FPresentParams.BackBufferFormat);
			pUserDesc->Multisamples = (ASCInt)m_PresentParams.MultiSampleType;
		}
	}

	void DestroySwapChain()
	{

	}

	ASCBoolean CreateDepthStencil(const PASCSwapChainDesc pUserDesc)
	{

	}

	void DestroyDepthStencil()
	{

	}

	ASCBoolean SaveRenderBuffers()
	{

	}

	void RestoreRenderBuffers()
	{

	}

	ASCBoolean SetRenderBuffers()
	{

	}
};