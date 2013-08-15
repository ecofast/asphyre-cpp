#include "ASCTextures.h"
#include "ASCPixelFormatConv.h"

CASCTexture::CASCTexture()
{
	m_nWidth = 256;
	m_nHeight = 256;
	m_bActive = false;
	m_PixelFormat = apfUnknown;
	m_bMipMapping = false;
}

CASCTexture::~CASCTexture()
{
	if (m_bActive)
	{
		Finalize();
	}
}

ASCBoolean CASCTexture::Initialize()
{
	ASCBoolean bResult = !m_bActive;
	if (!bResult)
	{
		return false;
	}
	else
	{
		bResult = CreateTexture();
		m_bActive = bResult;
		return bResult;
	}
}

void CASCTexture::Finalize()
{
	if (m_bActive)
	{
		DestroyTexture();
	}
	m_bActive = false;
}

ASCPointer CASCTexture::GetResourceView()
{
	return 0;
}

void CASCTexture::Bind(ASCInt nStage)
{

}

void CASCTexture::HandleDeviceReset()
{

}

void CASCTexture::HandleDeviceLost()
{

}

CASCFloatVector2D CASCTexture::PixelToLogical(const CASCIntVector2D& pos)
{
	CASCFloatVector2D result;

	if (m_nWidth > 0)
	{
		result.X = (ASCSingle)pos.X / m_nWidth;
	} 
	else
	{
		result.X = 0.0;
	}

	if (m_nHeight > 0)
	{
		result.Y = (ASCSingle)pos.Y / m_nHeight;
	} 
	else
	{
		result.Y = 0.0;
	}

	return result;
}

CASCFloatVector2D CASCTexture::PixelToLogical(const CASCFloatVector2D& pos)
{
	CASCFloatVector2D result;

	if (m_nWidth > 0)
	{
		result.X = pos.X / m_nWidth;
	} 
	else
	{
		result.X = 0.0;
	}

	if (m_nHeight > 0)
	{
		result.Y = pos.Y / m_nHeight;
	} 
	else
	{
		result.Y = 0.0;
	}

	return result;
}

CASCIntVector2D CASCTexture::LogicalToPixel(const CASCFloatVector2D& pos)
{
	CASCIntVector2D	result;

	result.X = ASCRound(pos.X * m_nWidth);
	result.Y = ASCRound(pos.Y * m_nHeight);
	return result;
}

void CASCTexture::UpdateMipmaps()
{

}

CASCPixelFormat CASCTexture::GetPixelFormat()
{
	return m_PixelFormat;
}

void CASCTexture::SetPixelFormat(const CASCPixelFormat fmt)
{
	if (!m_bActive)
	{
		m_PixelFormat = fmt;
	}
}

ASCInt CASCTexture::GetWidth()
{
	return m_nWidth;
}

void CASCTexture::SetWidth(const ASCInt nW)
{
	m_nWidth = nW;
	if (m_bActive)
	{
		UpdateSize();
	}
}

ASCInt CASCTexture::GetHeight()
{
	return m_nHeight;
}

void CASCTexture::SetHeight(const ASCInt nH)
{
	m_nHeight = nH;
	if (m_bActive)
	{
		UpdateSize();
	}
}

ASCBoolean CASCTexture::GetActive()
{
	return m_bActive;
}

ASCInt CASCTexture::BytesPerPixel()
{
	return (GetBytesPerPixel());
}

ASCBoolean CASCTexture::GetMipMapping()
{
	return m_bMipMapping;
}

void CASCTexture::SetMipMapping(const ASCBoolean bMM)
{
	if (!m_bActive)
	{
		m_bMipMapping = bMM;
	}
}

ASCInt CASCTexture::GetBytesPerPixel()
{
	return (C_ASCPixelFormatBits[m_PixelFormat] / 8);
}

void CASCTexture::UpdateSize()
{

}

ASCBoolean CASCTexture::CreateTexture()
{
	return true;
}

void CASCTexture::DestroyTexture()
{

}

CASCLockableTexture::CASCLockableTexture()
{
	CASCTexture::CASCTexture();
	m_bDynamicTexture = false;
}

ASCUInt CASCLockableTexture::GetPixel(const ASCInt nX, const ASCInt nY)
{
	if ((nX < 0) | (nY < 0) | (nX >= GetWidth()) | (nY >= GetHeight()))
	{
		return 0;
	}

	ASCUInt uResult = 0;
	ASCPointer pBits = 0;
	ASCInt nPitch = 0;
	Lock(ASCBounds(0, nY, GetWidth(), 1), pBits, nPitch);
	if (!pBits)
	{
		return 0;
	}
	else
	{
		uResult = ASCPixelFormatXTo32((ASCPointer)((ASCInt)pBits + nX * GetBytesPerPixel()), GetPixelFormat());
		Unlock();
		return uResult;
	}
}

void CASCLockableTexture::SetPixel(const ASCInt nX, const ASCInt nY, const ASCUInt uVal)
{
	if ((nX < 0) | (nY < 0) | (nX >= GetWidth()) | (nY >= GetHeight()))
	{
		return;
	}

	ASCPointer pBits;
	ASCInt nPitch;
	Lock(ASCBounds(0, nY, GetWidth(), 1), pBits, nPitch);
	if (!pBits)
	{
		return;
	}
	else
	{
		Pixel32ToASCPixelFormatX(uVal, (ASCPointer)((ASCInt)pBits + nX * GetBytesPerPixel()), GetPixelFormat());
		Unlock();
	}
}

ASCBoolean CASCLockableTexture::IsDynamicTexture()
{
	return m_bDynamicTexture;
}

void CASCLockableTexture::SetIsDynamicTexture(const ASCBoolean bVal)
{
	if (!GetActive())
	{
		m_bDynamicTexture = bVal;
	}
}

CASCRenderTargetTexture::CASCRenderTargetTexture()
{
	CASCTexture::CASCTexture();
	m_bDepthStencil = false;
	m_nMultisamples = 0;
}

ASCBoolean CASCRenderTargetTexture::GetDepthStencil()
{
	return m_bDepthStencil;
}

void CASCRenderTargetTexture::SetDepthStencil(const ASCBoolean bVal)
{
	if (!GetActive())
	{
		m_bDepthStencil = bVal;
	}
}

ASCInt CASCRenderTargetTexture::GetMultisamples()
{
	return m_nMultisamples;
}

void CASCRenderTargetTexture::SetMultisamples(const ASCInt nVal)
{
	if (!GetActive())
	{
		m_nMultisamples = nVal;
	}
}