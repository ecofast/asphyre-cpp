#include "ASCDX9SwapChains.h"
#include "ASCDX9Common.h"

CASCDX9SwapChain::CASCDX9SwapChain()
{
	m_bInitialized = false;
	m_pD3DSwapChain = 0;
	m_DepthStencilFormat = D3DFMT_UNKNOWN;
	memset(&m_PresentParams, 0, sizeof(m_PresentParams));
}

CASCDX9SwapChain::~CASCDX9SwapChain()
{
	if (m_bInitialized)
	{
		Finalize();
	}
}

ASCBoolean CASCDX9SwapChain::Initialize(const PASCSwapChainDesc pUserDesc)
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
	return true;
}

void CASCDX9SwapChain::Finalize()
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

ASCBoolean CASCDX9SwapChain::BeginRender()
{
	ASCBoolean bResult = (m_pD3DSwapChain && G_pD3D9Device);
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

void CASCDX9SwapChain::EndRender()
{
	RestoreRenderBuffers();
}

ASCBoolean CASCDX9SwapChain::Present()
{
	if (!m_pD3DSwapChain)
	{
		return false;
	}
	else
	{
		return (SUCCEEDED(m_pD3DSwapChain->Present(0, 0, 0, 0, 0)));
	}
}

ASCBoolean CASCDX9SwapChain::IsInitialized()
{
	return m_bInitialized;
}

IDirect3DSwapChain9* CASCDX9SwapChain::GetD3DSwapChain9()
{
	return m_pD3DSwapChain;
}

D3DPRESENT_PARAMETERS CASCDX9SwapChain::GetPresentParams()
{
	return m_PresentParams;
}

IDirect3DSurface9* CASCDX9SwapChain::GetDepthStencil()
{
	return m_pDepthStencil;
}

ASCBoolean CASCDX9SwapChain::MakePresentParams(const PASCSwapChainDesc pUserDesc)
{
	if (!pUserDesc)
	{
		return false;
	}

	m_PresentParams.BackBufferWidth = pUserDesc->nWidth;
	m_PresentParams.BackBufferHeight = pUserDesc->nHeight;
	m_PresentParams.Windowed = true;
	m_PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_PresentParams.hDeviceWindow = (HWND)pUserDesc->hWndHandle;
	if (pUserDesc->bVSync)
	{
		m_PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}
	else
	{
		m_PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	m_PresentParams.BackBufferFormat = DX9FindBackBufferFormat(pUserDesc->Format);

	m_DepthStencilFormat = DX9FindDepthStencilFormat((ASCInt)(pUserDesc->DepthStencil));

	DX9FindBestMultisampleType(m_PresentParams.BackBufferFormat, m_DepthStencilFormat, pUserDesc->nMultisamples, 
		&(m_PresentParams.MultiSampleType), &(m_PresentParams.MultiSampleQuality));

	return true;
}

ASCBoolean CASCDX9SwapChain::CreateSwapChain(const PASCSwapChainDesc pUserDesc)
{
	// 1) Verify initial conditions
	ASCBoolean bResult = (pUserDesc && G_pD3D9Device && (m_PresentParams.BackBufferWidth > 0) && (m_PresentParams.BackBufferHeight > 0));
	if (!bResult)
	{
		return false;
	}

	// 2) Create additional swap chain
	bResult = SUCCEEDED(G_pD3D9Device->CreateAdditionalSwapChain(&m_PresentParams, &m_pD3DSwapChain));

	// 3) Update the description of the first swap chain
	if (bResult)
	{
		pUserDesc->Format = DX9PixelFormatToASC(m_PresentParams.BackBufferFormat);
		pUserDesc->nMultisamples = (ASCInt)m_PresentParams.MultiSampleType;
	}

	return bResult;
}

void CASCDX9SwapChain::DestroySwapChain()
{
	m_DepthStencilFormat = D3DFMT_UNKNOWN;
	memset(&m_PresentParams, 0, sizeof(m_PresentParams));
	if (m_pD3DSwapChain)
	{
		m_pD3DSwapChain = 0;
	}
}

ASCBoolean CASCDX9SwapChain::CreateDepthStencil(const PASCSwapChainDesc pUserDesc)
{
	// 1) If no depth-stencil is required, return success
	if (pUserDesc->DepthStencil == adstNone)
	{
		return true;
	}

	// 2) Verify initial conditions
	if (!(pUserDesc && G_pD3D9Device && (m_DepthStencilFormat != D3DFMT_UNKNOWN)))
	{
		return false;
	}

	// 3) Create depth-stencil surface
	return (SUCCEEDED(G_pD3D9Device->CreateDepthStencilSurface(pUserDesc->nWidth, pUserDesc->nHeight,
		m_DepthStencilFormat, m_PresentParams.MultiSampleType, m_PresentParams.MultiSampleQuality, true, &m_pDepthStencil, 0)));
}

void CASCDX9SwapChain::DestroyDepthStencil()
{
	if (m_pDepthStencil)
	{
		m_pDepthStencil = 0;
	}
}

ASCBoolean CASCDX9SwapChain::SaveRenderBuffers()
{
	if (!G_pD3D9Device)
	{
		return false;
	}

	if (FAILED(G_pD3D9Device->GetRenderTarget(0, &m_pSavedBackBuffer)))
	{
		return false;
	}

	if (m_PresentParams.EnableAutoDepthStencil)
	{
		if (SUCCEEDED(G_pD3D9Device->GetDepthStencilSurface(&m_pSavedDepthStencil)))
		{
			return true;
		} 
		else
		{
			m_pSavedBackBuffer = 0;
			return false;
		}
	} 
	else
	{
		return false;
	}
}

void CASCDX9SwapChain::RestoreRenderBuffers()
{
	if (G_pD3D9Device)
	{
		if (m_PresentParams.EnableAutoDepthStencil)
		{
			G_pD3D9Device->SetDepthStencilSurface(m_pSavedDepthStencil);
		}
		G_pD3D9Device->SetRenderTarget(0, m_pSavedBackBuffer);
	}

	if (m_pSavedDepthStencil)
	{
		m_pSavedDepthStencil = 0;
	}

	if (m_pSavedBackBuffer)
	{
		m_pSavedBackBuffer = 0;
	}
}

ASCBoolean CASCDX9SwapChain::SetRenderBuffers()
{
	if (!(G_pD3D9Device && m_pD3DSwapChain))
	{
		return false;
	}

	if (FAILED(m_pD3DSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pSavedBackBuffer)))
	{
		return false;
	}

	if (SUCCEEDED(G_pD3D9Device->SetRenderTarget(0, m_pSavedBackBuffer)))
	{
		return (SUCCEEDED(G_pD3D9Device->SetDepthStencilSurface(m_pDepthStencil)));
	} 
	else
	{
		return false;
	}
}

CASCDX9SwapChains::CASCDX9SwapChains()
{

}

CASCDX9SwapChains::~CASCDX9SwapChains()
{
	Clear();
}

ASCInt CASCDX9SwapChains::Add(const PASCSwapChainDesc pUserDesc)
{
	CASCDX9SwapChain* pItem = new CASCDX9SwapChain();
	if (!(pItem->Initialize(pUserDesc)))
	{
		delete pItem;
		return -1;
	}

	ASCUInt uResult = m_Datas.size();
	m_Datas.resize(uResult + 1);
	m_Datas[uResult] = pItem;
	return (ASCInt)uResult;
}

void CASCDX9SwapChains::Clear()
{
	for (ASCInt i = m_Datas.size() - 1; i >= 0; i--)
	{
		if (m_Datas[i])
		{
			delete m_Datas[i];
			m_Datas[i] = 0;
		}
	}
	m_Datas.clear();
}

ASCBoolean CASCDX9SwapChains::CreateAll(CASCSwapChains* pUserChains)
{
	/*
	 * Typically, the first swap chain is ignored as it is the primary swap chain
	 * created by the device. 
	*/
	const ASCInt CHAIN_INIT_INDEX = 1;

	if (!pUserChains)
	{
		return false;
	}

	if (m_Datas.size() > 0)
	{
		Clear();
	}

	ASCBoolean bResult = true;
	PASCSwapChainDesc pDesc;
	ASCInt nIndex;
	for (ASCInt i = CHAIN_INIT_INDEX; i < pUserChains->GetCount(); i++)
	{
		pDesc = pUserChains->GetItem(i);
		if (!pDesc)
		{
			bResult = false;
			break;
		}

		nIndex = Add(pDesc);
		bResult = (nIndex != -1);
		if (!bResult)
		{
			break;
		}
	}

	if ((!bResult) && (m_Datas.size() > 0))
	{
		Clear();
	}

	return bResult;
}

ASCInt CASCDX9SwapChains::GetCount()
{
	return (m_Datas.size());
}

CASCDX9SwapChain* CASCDX9SwapChains::GetItem(ASCInt nIndex)
{
	if ((nIndex >= 0) && ((size_t)nIndex < m_Datas.size()))
	{
		return (m_Datas[nIndex]);
	} 
	else
	{
		return 0;
	}
}