#include "ASCSwapChains.h"

CASCSwapChains::CASCSwapChains( void* pDevice )
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

	ASCInt result = m_Datas.size();
	m_Datas.resize(result + 1);
	memset(&m_Datas[result], 0, sizeof(CSwapChainDesc));
	return result;
}

ASCInt CASCSwapChains::Add( const CSwapChainDesc desc )
{
	ASCInt result = Insert();
	if (result == -1)
	{
		return -1;
	}

	m_Datas[result] = desc;
	return (result);
}

ASCInt CASCSwapChains::Add( ASCUInt uWndHandle, ASCIntVector2D vtSize, 
						   ASCInt nMultisamples /*= 0*/, ASCBoolean bVSync /*= false*/, ASCPixelFormat Format /*= apfUnknown*/, 
						   CDepthStencilType DepthStencil /*= dstNone*/ )
{
	ASCInt result = Insert();
	if (result == -1)
	{
		return (-1);
	} 
	else
	{
		m_Datas[result].WndHandle = uWndHandle;
		m_Datas[result].Width = ptSize.X;
		m_Datas[result].Height = ptSize.Y;
		m_Datas[result].Format = Format;
		m_Datas[result].VSync = bVSync;
		m_Datas[result].Multisamples = nMultisamples;
		m_Datas[result].DepthStencil = DepthStencil;
		return result;
	}
}

void CASCSwapChains::Remove( ASCInt nIndex )
{
	if ((!IsDeviceInactive()) || (nIndex < 0) || (nIndex >= ASCInt(m_Datas.size())))
	{
		return;
	}

	vector<SwapChainDesc>::iterator it = m_Datas.begin();
	m_Datas.erase(it + nIndex);
	/*
	for (AGEInt i = 0; i < nIndex; i++)
	{
		it++;
	}
	m_Datas.erase(it);
	*/
}

void CASCSwapChains::RemoveAll()
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

ASCInt CASCSwapChains::GetCount() const
{
	return m_Datas.size();
}

PSwapChainDesc CASCSwapChains::GetItem( ASCInt nIndex )
{
	if (nIndex >= 0 && nIndex < AGEInt(m_Datas.size()))
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
	if (m_pDevice)  // (Assigned(FDevice))and(FDevice is TAsphyreDevice)
	{
		return (true);  // Result:= TAsphyreDevice(FDevice).State = adsNotActive;
	} 
	else
	{
		return (true);
	}
}