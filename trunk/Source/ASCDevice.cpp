#include "ASCDevice.h"
#include "ASCStrUtils.h"

CASCDevice::CASCDevice()
{
	m_DevTech = adtUnknown;
	m_nTechVersion = 0;
	m_nTechFeatureVersion = 0;
	m_fDeviceScale = 1.0;

	m_pSwapChains = new CASCSwapChains(this);
	m_DevState = adsNotActive;

	m_fFillDepthValue = 1.0;
	m_uFillStencilValue	= 0;
}

CASCDevice::~CASCDevice()
{
	Finalize();
	delete m_pSwapChains;
	m_pSwapChains = 0;
}

ASCBoolean CASCDevice::Initialize()
{
	// 1) Check initial conditions
	if (m_DevState == adsActive)
	{
		return true;
	}

	if (m_DevState == adsInitFailed)
	{
		return false;
	}

	ASCBoolean bResult = true;

	// 2) Initialize device parameters
	ASCDeviceInitEvent()->Notify(this, &bResult);
	if (!bResult)
	{
		return false;
	}

	m_DevState = adsCreating;

	// 3) Create and initialize the particular device
	bResult = InitializeDevice();
	if (!bResult)
	{
		m_DevState = adsInitFailed;
		ASCTimerResetEvent()->Notify(this);
		return false;
	}

	m_DevState = adsActive;

	// 4) Notify others that the device has been created
	ASCDeviceCreateEvent()->Notify(this, &bResult);
	if (!bResult)
	{
		FinalizeDevice();
		m_DevState = adsInitFailed;
		ASCTimerResetEvent()->Notify(this);
		return false;
	}

	// 5) Notify others that the device is now in ready state
	ASCDeviceResetEvent()->Notify(this, &bResult);
	if (!bResult)
	{
		ASCDeviceDestroyEvent()->Notify(this);
		FinalizeDevice();
		m_DevState = adsInitFailed;
		ASCTimerResetEvent()->Notify(this);
		return false;
	}

	// 6) Notify the timer that a lenghty operation took place
	ASCTimerResetEvent()->Notify(this);
	return true;
}

void CASCDevice::Finalize()
{
	if (!((m_DevState == adsActive) || (m_DevState == adsRunTimeFault)))
	{
		return;
	}

	ClearStates();

	ASCDeviceLostEvent()->Notify(this);
	ASCDeviceDestroyEvent()->Notify(this);

	FinalizeDevice();

	m_DevState = adsNotActive;
}

CASCDeviceState CASCDevice::GetDeviceState()
{
	return m_DevState;
}

ASCSingle CASCDevice::GetFillDepthValue()
{
	return m_fFillDepthValue;
}

void CASCDevice::SetFillDepthValue(const ASCSingle fValue)
{
	m_fFillDepthValue = fValue;
}

ASCUInt32 CASCDevice::GetFillStencilValue()
{
	return m_uFillStencilValue;
}

void CASCDevice::SetFillStencilValue(const ASCUInt32 uValue)
{
	m_uFillStencilValue = uValue;
}

void CASCDevice::Render(CASCNotifyEvent Handler, ASCColor uBkColor)
{
	if ((m_DevState == adsActive) && (CanRender(0)))
	{
		RenderWith(0, Handler, uBkColor);
	} 
	else
	{
		ASCTiming()->Sleep(5);
	}
}

void CASCDevice::Render(ASCInt nSwapChainIndex, CASCNotifyEvent Handler, ASCColor uBkColor)
{
	if ((m_DevState == adsActive) && (CanRender(nSwapChainIndex)))
	{
		RenderWith(nSwapChainIndex, Handler, uBkColor);
	} 
	else
	{
		ASCTiming()->Sleep(5);
	}
}

void CASCDevice::RenderTo(CASCNotifyEvent Handler, ASCColor uBkColor, ASCBoolean bFillBk, CASCRenderTargetTexture* pTexture)
{
	if ((m_DevState != adsActive) || (!CanRender(-1)) || (!pTexture))
	{
		return;
	}

	if (!(pTexture->BeginRenderTo()))
	{
		return;
	}
	else
	{
		RenderToTarget(Handler, uBkColor, bFillBk);
		pTexture->EndRenderTo();
	}
}

ASCBoolean CASCDevice::Resize(ASCInt nSwapChainIndex, const CASCIntVector2D NewSize)
{
	if (IsAtFault())
	{
		return false;
	}

	PASCSwapChainDesc pUserDesc = m_pSwapChains->GetItem(nSwapChainIndex);
	if (!pUserDesc)
	{
		return false;
	}

	if ((pUserDesc->nWidth == NewSize.X) && (pUserDesc->nHeight == NewSize.Y))
	{
		return true;
	}

	ClearStates();
	if (m_DevState != adsNotActive)
	{
		CASCIntVector2D PrevSize;
		PrevSize.X = pUserDesc->nWidth;
		PrevSize.Y = pUserDesc->nHeight;
		pUserDesc->nWidth = NewSize.X;
		pUserDesc->nHeight = NewSize.Y;

		if (ResizeSwapChain(nSwapChainIndex, pUserDesc))
		{
			return true;
		} 
		else
		{
			pUserDesc->nWidth	= PrevSize.X;
			pUserDesc->nHeight	= PrevSize.Y;
			return false;
		}
	} 
	else
	{
		return true;
	}
}

ASCBoolean CASCDevice::IsAtFault()
{
	return ((m_DevState == adsInitFailed) || (m_DevState == adsRunTimeFault));
}

void CASCDevice::ClearStates()
{
	if (m_DevState == adsActive)
	{
		ClearDevStates();
	}
}

void CASCDevice::ResetInitFailed()
{
	if (m_DevState == adsInitFailed)
	{
		m_DevState = adsNotActive;
	}
}

CASCSwapChains* CASCDevice::GetSwapChains()
{
	return m_pSwapChains;
}

void CASCDevice::ResetDevice()
{
	// place holder
}

ASCBoolean CASCDevice::CanRender(ASCInt nSwapChainIndex)
{
	return true;
}

ASCBoolean CASCDevice::ResizeSwapChain(ASCInt nSwapChainIndex, PASCSwapChainDesc pNewUserDesc)
{
	return true;
}

void CASCDevice::ClearDevStates()
{
	// place holder
}

CASCDeviceTechnology CASCDevice::GetDeviceTechnology()
{
	return m_DevTech;
}

void CASCDevice::SetDeviceScale(ASCSingle fValue)
{
	m_fDeviceScale = fValue;
}

ASCSingle CASCDevice::GetDeviceScale()
{
	return m_fDeviceScale;
}

ASCInt CASCDevice::GetTechVersion()
{
	return m_nTechVersion;
}

ASCInt CASCDevice::GetTechFeatureVersion()
{
	return m_nTechFeatureVersion;
}

wstring ASCDeviceTechnologyToString(CASCDeviceTechnology DevTech)
{
	switch (DevTech)
	{
	case adtDirectX:
		return L"DirectX";
		break;
	case adtOpenGL:
		return L"OpenGL";
		break;
	case adtOpenGLES:
		return L"OpenGL ES";
		break;
	case adtPropietary:
		return L"Propietary";
		break;
	default:
		return L"Unknown";
		break;
	}
}

wstring ASCDeviceVersionToString(ASCInt nVersion, ASCBoolean bCompact /*= false*/)
{
	if (nVersion <= 0)
	{
		return L"0.0";
	}

	wstring sResult = L"";

	if ((nVersion & 0x00F) > 0)
	{
		sResult = L"." + IntToStr(nVersion & 0x00F);
	}

	if ((!bCompact) || ((nVersion & 0x0F0) > 0))
	{
		sResult = L"." + IntToStr((nVersion & 0x0F0) >> 4) + sResult;
	}

	sResult = IntToStr(nVersion >> 8) + sResult;
	return sResult;
}

wstring ASCDeviceFullTechnologyToString(CASCDevice* pDevice)
{
	if ((!pDevice) || (pDevice->GetDeviceTechnology() == adtUnknown))
	{
		return L"Unidentified device technology.";
	}

	wstring sResult = ASCDeviceTechnologyToString(pDevice->GetDeviceTechnology());

	if (pDevice->GetTechVersion() > 0)
	{
		sResult = sResult + (ASCWChar)32 + ASCDeviceVersionToString(pDevice->GetTechVersion(), true);
	}

	if ((pDevice->GetDeviceTechnology() == adtDirectX) && (pDevice->GetTechVersion() == 0x900))
	{
		// DirectX 9 specific
		if (pDevice->GetTechFeatureVersion() == 0x901)
		{
			sResult = sResult + L" Ex (Vista)";
		}
		else
		{
			sResult = sResult + L" (XP compatibility)";
		}
	}
	else
	{
		// General feature levels
		if (pDevice->GetTechFeatureVersion() > 0)
		{
			sResult = sResult + L" (feature level " + ASCDeviceVersionToString(pDevice->GetTechFeatureVersion()) + L")";
		}
	}

	return sResult;
}