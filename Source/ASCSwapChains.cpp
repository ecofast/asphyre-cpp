#include "ASCSwapChains.h"
#include "ASCDevice.h"

CASCSwapChains::CASCSwapChains(void* pDevice)
{
	m_pDevice = pDevice;
}

CASCSwapChains::~CASCSwapChains()
{

}

ASCInt CASCSwapChains::Insert()
{
	if (!IsDeviceInactive())
	{
		return -1;
	}

	ASCInt nResult = m_Datas.size();
	m_Datas.resize(nResult + 1);
	memset(&(m_Datas[nResult]), 0, sizeof(CASCSwapChainDesc));
	return nResult;
}

ASCInt CASCSwapChains::Add(const CASCSwapChainDesc desc)
{
	ASCInt nResult = Insert();
	if (nResult == -1)
	{
		return -1;
	}

	m_Datas[nResult] = desc;
	return nResult;
}

ASCInt CASCSwapChains::Add(ASCUInt uWndHandle, CASCIntVector2D vtSize, 
						   ASCInt nMultisamples /*= 0*/, ASCBoolean bVSync /*= false*/, CASCPixelFormat Format /*= apfUnknown*/, 
						   CASCDepthStencilType DepthStencil /*= dstNone*/ )
{
	ASCInt nResult = Insert();
	if (nResult == -1)
	{
		return -1;
	} 
	else
	{
		m_Datas[nResult].hWndHandle = uWndHandle;
		m_Datas[nResult].nWidth = vtSize.X;
		m_Datas[nResult].nHeight = vtSize.Y;
		m_Datas[nResult].Format = Format;
		m_Datas[nResult].bVSync = bVSync;
		m_Datas[nResult].nMultisamples = nMultisamples;
		m_Datas[nResult].DepthStencil = DepthStencil;
		return nResult;
	}
}

void CASCSwapChains::Remove(ASCInt nIndex)
{
	if ((!IsDeviceInactive()) || (nIndex < 0) || (nIndex >= (ASCInt)(m_Datas.size())))
	{
		return;
	}

	vector<CASCSwapChainDesc>::iterator it = m_Datas.begin();
	m_Datas.erase(it + nIndex);
}

void CASCSwapChains::Clear()
{
	if (!IsDeviceInactive())
	{
		return;
	}

	m_Datas.clear();
}

void* CASCSwapChains::GetDevice()
{
	return m_pDevice;
}

ASCInt CASCSwapChains::GetCount()
{
	return m_Datas.size();
}

PASCSwapChainDesc CASCSwapChains::GetItem(ASCInt nIndex )
{
	if (nIndex >= 0 && nIndex < (ASCInt)m_Datas.size())
	{
		return &(m_Datas[nIndex]);
	} 
	else
	{
		return 0;
	}
}

ASCBoolean CASCSwapChains::IsDeviceInactive()
{
	/*
	if (Assigned(FDevice)) and (FDevice is TAsphyreDevice) then
	  Result := TAsphyreDevice(FDevice).State = adsNotActive
	else
	  Result := True;
	*/

	if (m_pDevice)
	{
		return (((CASCDevice* )m_pDevice)->GetDeviceState() == adsNotActive);
	} 
	else
	{
		return true;
	}
}