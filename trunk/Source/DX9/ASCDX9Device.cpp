#include <d3d9.h>
#include <d3dx9.h>
#include <objbase.h>
#include "ASCDX9Device.h"
#include "../ASCEvents.h"
#include "ASCDX9Common.h"

// DIRECT3DCREATE9EXFUNCTION
typedef HRESULT (WINAPI *Direct3DCreate9ExFunction)(ASCUInt uSDKVersion, IDirect3D9Ex**);

CASCDX9Device::CASCDX9Device() : CASCDevice()
{
	m_DevTech = adtDirectX;
	m_nTechVersion = 0x900;

	m_pSwapChains = new CASCDX9SwapChains();

	m_bManagedD3D = false;
	m_bManagedDevice = false;
}

CASCDX9Device::~CASCDX9Device()
{
	delete m_pSwapChains;
	m_pSwapChains = 0;
}

CASCDX9SwapChains* CASCDX9Device::GetDX9SwapChains()
{
	return m_pSwapChains;
}

ASCBoolean CASCDX9Device::InitializeDevice()
{
	if (!CreateDirect3D())
	{
		return false;
	}

	if (!GetDisplayMode())
	{
		DestroyDirect3D();
		return false;
	}

	if (!MakePresentParams())
	{
		ClearD3D9PresentParams();
		ClearD3D9DisplayMode();
		DestroyDirect3D();
		return false;
	}

	if (!CreateDevice())
	{
		ClearD3D9PresentParams();
		ClearD3D9DisplayMode();
		DestroyDirect3D();
		return false;
	}

	if (!(m_pSwapChains->CreateAll((CASCSwapChains *)GetSwapChains())))
	{
		DestroyDevice();
		ClearD3D9PresentParams();
		ClearD3D9DisplayMode();
		DestroyDirect3D();
		return false;
	}

	m_bIsInLostState = false;
	return true;
}

void CASCDX9Device::FinalizeDevice()
{
	m_pSwapChains->Clear();
	DestroyDevice();
	ClearD3D9PresentParams();
	ClearD3D9DisplayMode();
	DestroyDirect3D();
}

void CASCDX9Device::ResetDevice()
{
	MoveIntoLostState();
	AttemptRecoverState();
}

ASCBoolean CASCDX9Device::CanRender(ASCInt nSwapChainIndex)
{
	return CheckDeviceCondition(nSwapChainIndex);
}

void CASCDX9Device::RenderWith(ASCInt nSwapChainIndex, CASCNotifyEvent Handler, ASCUInt uBkColor)
{
	PASCSwapChainDesc pDesc = GetSwapChains()->GetItem(nSwapChainIndex);
	if ((!pDesc) || (!G_pD3D9Device))
	{
		return;
	}

	CASCDX9SwapChain* pChain = 0;
	if (nSwapChainIndex > 0)
	{
		pChain = m_pSwapChains->GetItem(nSwapChainIndex - 1);
		if (!pChain)
		{
			return;
		}
	}

	if (pChain && (!(pChain->BeginRender())))
	{
		return;
	}

	SetDefaultViewport(pDesc);
	G_nDX9ActiveDepthStencilLevel = (ASCInt)(pDesc->DepthStencil);
	Clear(uBkColor);

	if (SUCCEEDED(G_pD3D9Device->BeginScene()))
	{
		ASCBeginSceneEvent()->Notify(this);
		Handler();  // Handler(this);
		ASCEndSceneEvent()->Notify(this);
		G_pD3D9Device->EndScene();
	}

	if (pChain)
	{
		pChain->EndRender();
		pChain->Present();
	}
	else
	{
		if (G_D3D9Mode == admDirect3D9Ex)
		{
			((IDirect3DDevice9Ex*)G_pD3D9Device)->PresentEx(0, 0, 0, 0, 0);
		} 
		else
		{
			G_pD3D9Device->Present(0, 0, 0, 0);
		}
	}
}

void CASCDX9Device::RenderToTarget(CASCNotifyEvent Handler, ASCUInt uBkColor, ASCBoolean bFillBk)
{
	if (!G_pD3D9Device)
	{
		return;
	}

	if (bFillBk)
	{
		Clear(uBkColor);
	}

	if (SUCCEEDED(G_pD3D9Device->BeginScene()))
	{
		ASCBeginSceneEvent()->Notify(this);
		Handler();  // Handler(this);
		ASCEndSceneEvent()->Notify(this);
		G_pD3D9Device->EndScene();
	}
}

ASCBoolean CASCDX9Device::ResizeSwapChain(ASCInt nSwapChainIndex, PASCSwapChainDesc pNewUserDesc)
{
	ASCBoolean bResult = false;

	PASCSwapChainDesc pDesc = GetSwapChains()->GetItem(nSwapChainIndex);
	if ((!pDesc) || (!G_pD3D9Device))
	{
		return bResult;
	}

	CASCDX9SwapChain* pChain = 0;
	if (nSwapChainIndex > 0)
	{
		pChain = m_pSwapChains->GetItem(nSwapChainIndex - 1);
		if (!pChain)
		{
			return bResult;
		}
	}

	pDesc->nWidth = pNewUserDesc->nWidth;
	pDesc->nHeight = pNewUserDesc->nHeight;

	if (!pChain)
	{
		if (G_D3D9Mode == admDirect3D9Ex)
		{
			G_D3D9PresentParams.BackBufferWidth = pDesc->nWidth;
			G_D3D9PresentParams.BackBufferHeight = pDesc->nHeight;
			bResult = SUCCEEDED(((IDirect3DDevice9Ex*)G_pD3D9Device)->ResetEx(&G_D3D9PresentParams, 0));
		} 
		else
		{
			MoveIntoLostState();
			G_D3D9PresentParams.BackBufferWidth = pDesc->nWidth;
			G_D3D9PresentParams.BackBufferHeight = pDesc->nHeight;
			bResult = AttemptRecoverState();
		}
	}
	else
	{
		pChain->Finalize();
		bResult = pChain->Initialize(pDesc);
	}

	return bResult;
}

ASCBoolean CASCDX9Device::CreateDirect3D()
{
	m_bManagedD3D = false;
	if (G_pD3D9)
	{
		G_D3D9Mode = admDirect3D9;
		m_nTechFeatureVersion = 0x900;

		IDirect3D9Ex* pD3D9Ex = 0;
		if (SUCCEEDED(G_pD3D9->QueryInterface(__uuidof(IDirect3D9Ex), (void**)&pD3D9Ex)))
		{
			G_D3D9Mode = admDirect3D9Ex;
			m_nTechFeatureVersion = 0x901;
			pD3D9Ex = 0;
		}

		return true;
	}

	if (G_D3D9Mode != admDirect3D9)
	{
		HMODULE hD3D = LoadLibrary(L"d3d9.dll");
		IDirect3D9Ex* pD3D9Ex;
		Direct3DCreate9ExFunction pfnCreate9Ex = (Direct3DCreate9ExFunction)GetProcAddress(hD3D, "Direct3DCreate9Ex");
		if (pfnCreate9Ex)
		{
			(*pfnCreate9Ex)(D3D_SDK_VERSION, &pD3D9Ex);
			pD3D9Ex->QueryInterface(__uuidof(IDirect3D9), reinterpret_cast<void **>(&G_pD3D9));
			G_D3D9Mode = admDirect3D9Ex;
			m_nTechFeatureVersion = 0x901;
			pD3D9Ex = 0;
		}

		if (hD3D)
		{
			FreeLibrary(hD3D);
		}
	}

	if (!G_pD3D9)
	{
		G_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if (G_pD3D9)
		{
			G_D3D9Mode = admDirect3D9;
			m_nTechFeatureVersion = 0x900;
		}
	}

	if (G_pD3D9)
	{
		m_bManagedD3D = true;
		return true;
	}
	else
	{
		return false;
	}
}

void CASCDX9Device::DestroyDirect3D()
{
	if (m_bManagedD3D)
	{
		if (G_pD3D9)
		{
			G_pD3D9 = 0;
		}
		m_bManagedD3D = false;
	}

	G_D3D9Mode = admUnknown;
	m_nTechVersion = 0;
}

ASCBoolean CASCDX9Device::GetDisplayMode()
{
	if (!G_pD3D9)
	{
		return false;
	}

	ClearD3D9DisplayMode();

	if (G_D3D9Mode == admDirect3D9Ex)  // Vista enhanced mode
	{
		return (SUCCEEDED(((IDirect3D9Ex*)G_pD3D9)->GetAdapterDisplayModeEx(D3DADAPTER_DEFAULT, &G_D3D9DisplayMode, 0)));
	}
	else  // XP compatibility mode
	{
		D3DDISPLAYMODE mode;
		if (SUCCEEDED(G_pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
		{
			G_D3D9DisplayMode.Width = mode.Width;
			G_D3D9DisplayMode.Height = mode.Height;
			G_D3D9DisplayMode.RefreshRate = mode.RefreshRate;
			G_D3D9DisplayMode.Format = mode.Format;
			return true;
		}
		else
		{
			return false;
		}
	}
}

ASCBoolean CASCDX9Device::MakePresentParams()
{
	PASCSwapChainDesc pSwapChain = GetSwapChains()->GetItem(0);
	if (!pSwapChain)
	{
		return false;
	}

	G_D3D9PresentParams.BackBufferWidth = pSwapChain->nWidth;
	G_D3D9PresentParams.BackBufferHeight = pSwapChain->nHeight;
	G_D3D9PresentParams.Windowed = true;
	G_D3D9PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	G_D3D9PresentParams.hDeviceWindow = (HWND)(pSwapChain->hWndHandle);
	if (pSwapChain->bVSync)
	{
		G_D3D9PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	} 
	else
	{
		G_D3D9PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	G_D3D9PresentParams.BackBufferFormat = DX9FindBackBufferFormat(pSwapChain->Format);
	if (pSwapChain->DepthStencil != adstNone)
	{
		G_D3D9PresentParams.EnableAutoDepthStencil = true;
		G_D3D9PresentParams.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		G_D3D9PresentParams.AutoDepthStencilFormat = DX9FindDepthStencilFormat((ASCInt)pSwapChain->DepthStencil);
	}

	DX9FindBestMultisampleType(G_D3D9PresentParams.BackBufferFormat, G_D3D9PresentParams.AutoDepthStencilFormat, 
		pSwapChain->nMultisamples, &G_D3D9PresentParams.MultiSampleType, &(G_D3D9PresentParams.MultiSampleQuality));

	m_bUsingDepthBuf = pSwapChain->DepthStencil > adstNone;
	m_bUsingStencil = pSwapChain->DepthStencil > adstDepthOnly;

	return true;
}

ASCBoolean CASCDX9Device::CreateDevice()
{
	m_bManagedDevice = false;

	// 1) Verify whether the device has already been created
	if (G_pD3D9Device)
	{
		if (G_D3D9Mode == admUnknown)
		{
			G_D3D9Mode = admDirect3D9;
			IDirect3D9Ex* pD3D9Ex = 0;
			if (SUCCEEDED(G_pD3D9->QueryInterface(__uuidof(IDirect3D9Ex), (void**)&pD3D9Ex)))
			{
				G_D3D9Mode = admDirect3D9Ex;
				pD3D9Ex = 0;
			}
			return (SUCCEEDED(G_pD3D9Device->GetDeviceCaps(&G_D3DCaps9)));
		}
	}

	// 2) Check starting conditions
	if (!G_pD3D9)
	{
		return false;
	}

	PASCSwapChainDesc pDesc = GetSwapChains()->GetItem(0);
	if (!pDesc)
	{
		return false;
	}

	// 3) Prepare the device flags
	ASCUInt32 uFlags = D3DCREATE_NOWINDOWCHANGES;
#ifdef PRESERVEFPU
	uFlags = uFlags | D3DCREATE_FPU_PRESERVE;
#endif
#ifdef ENABLEMULTITHREAD
	uFlags = uFlags | D3DCREATE_MULTITHREADED;
#endif

	// 4) Create Direct3D9 device
	ASCBoolean bResult;
	if (G_D3D9Mode == admDirect3D9Ex)  // Vista enhanced mode
	{
		IDirect3DDevice9Ex *pD3DDev9Ex;
		bResult = SUCCEEDED(((IDirect3D9Ex*)G_pD3D9)->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			(HWND)(pDesc->hWndHandle), uFlags | D3DCREATE_HARDWARE_VERTEXPROCESSING, &G_D3D9PresentParams, 0, &pD3DDev9Ex));

		if (!bResult)
		{
			bResult = SUCCEEDED(((IDirect3D9Ex*)G_pD3D9)->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
				(HWND)(pDesc->hWndHandle), uFlags | D3DCREATE_SOFTWARE_VERTEXPROCESSING, &G_D3D9PresentParams, 0, &pD3DDev9Ex));
		}

		if (bResult)
		{
			G_pD3D9Device = pD3DDev9Ex;
			pD3DDev9Ex = 0;
		}
	} 
	else  // XP compatibility mode
	{
		bResult = SUCCEEDED(G_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			(HWND)(pDesc->hWndHandle), uFlags | D3DCREATE_HARDWARE_VERTEXPROCESSING, &G_D3D9PresentParams, &G_pD3D9Device));

		if (!bResult)
		{
			bResult = SUCCEEDED(G_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
					(HWND)(pDesc->hWndHandle), uFlags | D3DCREATE_SOFTWARE_VERTEXPROCESSING, &G_D3D9PresentParams, &G_pD3D9Device));
		}
	}

	// 5) Retrieve the capabilities of the device
	if (bResult)
	{
		bResult = SUCCEEDED(G_pD3D9Device->GetDeviceCaps(&G_D3DCaps9));
		if (!bResult)
		{
			G_pD3D9Device = 0;
		}
	}

	// 6) Update the description of the first swap chain
	if (bResult)
	{
		pDesc->Format = DX9PixelFormatToASC(G_D3D9PresentParams.BackBufferFormat);
		pDesc->nMultisamples = (ASCInt)G_D3D9PresentParams.MultiSampleType;
	}

	m_bManagedDevice = bResult;
	return bResult;
}

void CASCDX9Device::DestroyDevice()
{
	if (m_bManagedDevice)
	{
		ClearD3D9Caps();
		if (G_pD3D9Device)
		{
			G_pD3D9Device = 0;
		}
		m_bManagedDevice = false;
	}
}

void CASCDX9Device::MoveIntoLostState()
{
	if (!m_bIsInLostState)
	{
		ASCDeviceLostEvent()->Notify(this);
		m_pSwapChains->Clear();
		m_bIsInLostState = true;
	}
}

ASCBoolean CASCDX9Device::AttemptRecoverState()
{
	if (!G_pD3D9Device)
	{
		return false;
	}

	ASCBoolean bResult = false;
	if (m_bIsInLostState)
	{
		if (G_D3D9Mode == admDirect3D9Ex)
		{
			bResult = SUCCEEDED(((IDirect3DDevice9Ex*)G_pD3D9Device)->ResetEx(&G_D3D9PresentParams, 0));
		} 
		else
		{
			bResult = SUCCEEDED(G_pD3D9Device->Reset(&G_D3D9PresentParams));
		}

		if (bResult)
		{
			bResult = m_pSwapChains->CreateAll(GetSwapChains());
			if (!bResult)
			{
				m_DevState = adsRunTimeFault;
				return false;
			}
			m_bIsInLostState = false;
			ASCDeviceResetEvent()->Notify(this);
		}
	}

	return bResult;
}

ASCBoolean CASCDX9Device::CheckDeviceCondition(ASCInt nSwapChainIndex)
{
	if (!G_pD3D9Device)
	{
		return false;
	}

	ASCBoolean bResult;
	if (G_D3D9Mode == admDirect3D9Ex)
	{
		bResult = true;
		if (nSwapChainIndex == -1)
		{
			return bResult;
		}

		PASCSwapChainDesc pDesc = GetSwapChains()->GetItem(nSwapChainIndex);
		if (!pDesc)
		{
			return bResult;
		}

		bResult = SUCCEEDED(((IDirect3DDevice9Ex*)G_pD3D9Device)->CheckDeviceState((HWND)(pDesc->hWndHandle)));
		if (!bResult)
		{
			MoveIntoLostState();
			bResult = AttemptRecoverState();
		}
	} 
	else
	{
		HRESULT res = G_pD3D9Device->TestCooperativeLevel();
		switch (res)
		{
		case D3DERR_DEVICELOST:
			MoveIntoLostState();
			bResult = false;
			break;
		case D3DERR_DEVICENOTRESET:
			if (!m_bIsInLostState)
			{
				MoveIntoLostState();
			}
			bResult = AttemptRecoverState();
			break;
		case D3DERR_DRIVERINTERNALERROR:
			MoveIntoLostState();
			bResult = AttemptRecoverState();
			break;
		case D3D_OK:
			bResult = true;
			break;
		default:
			bResult = false;
			break;
		}
	}

	return bResult;
}

void CASCDX9Device::SetDefaultViewport(const PASCSwapChainDesc pDesc)
{
	if ((!G_pD3D9Device) || (!pDesc))
	{
		return;
	}

	D3DVIEWPORT9 vp9;
	vp9.X = 0;
	vp9.Y = 0;
	vp9.Width = pDesc->nWidth;
	vp9.Height = pDesc->nHeight;
	vp9.MinZ = 0.0;
	vp9.MaxZ = 1.0;

	G_pD3D9Device->SetViewport(&vp9);
}

void CASCDX9Device::Clear(ASCColor uColor)
{
	if (!G_pD3D9Device)
	{
		return;
	}

	ASCUInt uFlags = D3DCLEAR_TARGET;
	if (G_nDX9ActiveDepthStencilLevel > 0)
	{
		uFlags = uFlags | D3DCLEAR_ZBUFFER;
	}
	if (G_nDX9ActiveDepthStencilLevel > 1)
	{
		uFlags = uFlags | D3DCLEAR_STENCIL;
	}

	G_pD3D9Device->Clear(0, 0, uFlags, uColor, GetFillDepthValue(), GetFillStencilValue());
}