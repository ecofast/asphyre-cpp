#include "ASCImages.h"
#include "ASCProvider.h"
#include "ASCSurfaces.h"
#include "ASCBitmaps.h"
#include "ASCPixelFormatConv.h"

CASCImage::CASCImage()
{
	m_nPatternCount = -1;
	m_PatternSize = CASCIntVector2D(0, 0);
	m_bMipMapping = false;
	m_PixelFormat = apfUnknown;
	m_bDynamicImage = false;
}

CASCImage::~CASCImage()
{
	RemoveAllTextures();
}

void CASCImage::HandleDeviceReset()
{
	for (ASCInt i = 0; i < (ASCInt)(m_Textures.size()); i++)
	{
		if (m_Textures[i])
		{
			m_Textures[i]->HandleDeviceReset();
		}
	}
}

void CASCImage::HandleDeviceLost()
{
	for (ASCInt i = 0; i < (ASCInt)(m_Textures.size()); i++)
	{
		if (m_Textures[i])
		{
			m_Textures[i]->HandleDeviceLost();
		}
	}
}

wstring CASCImage::GetName()
{
	return m_sName;
}

void CASCImage::SetName(const wstring& sName)
{
	m_sName = sName;
}

ASCInt CASCImage::GetPatternCount()
{
	return m_nPatternCount;
}

void CASCImage::SetPatternCount(ASCInt nPatternCount)
{
	m_nPatternCount = nPatternCount;
}

CASCIntVector2D CASCImage::GetPatternSize()
{
	return m_PatternSize;
}

void CASCImage::SetPatternSize(CASCIntVector2D vec)
{
	m_PatternSize = vec;
}

CASCIntVector2D CASCImage::GetVisibleSize()
{
	return m_VisibleSize;
}

void CASCImage::SetVisibleSize(CASCIntVector2D vec)
{
	m_VisibleSize = vec;
}

ASCBoolean CASCImage::GetMipMapping()
{
	return m_bMipMapping;
}

void CASCImage::SetMipMapping(ASCBoolean bMipMapping)
{
	m_bMipMapping = bMipMapping;
}

CASCPixelFormat CASCImage::GetPixelFormat()
{
	return m_PixelFormat;
}

void CASCImage::SetPixelFormat(CASCPixelFormat fmt)
{
	m_PixelFormat = fmt;
}

ASCBoolean CASCImage::GetDynamicImage()
{
	return m_bDynamicImage;
}

void CASCImage::SetDynamicImage(ASCBoolean bDynamicImage)
{
	m_bDynamicImage = bDynamicImage;
}

ASCInt CASCImage::GetTextureCount()
{
	return m_Textures.size();
}

CASCLockableTexture* CASCImage::GetTexture(ASCInt nIndex)
{
	if ((nIndex >= 0) && (nIndex < (ASCInt)(m_Textures.size())))
	{
		return m_Textures[nIndex];
	} 
	else
	{
		return 0;
	}
}

ASCInt CASCImage::InsertTexture()
{
	CASCLockableTexture* pTexture = ASCFactory()->CreateLockableTexture();
	if (!pTexture)
	{
		return -1;
	}

	ASCInt nResult = m_Textures.size();
	m_Textures.resize(nResult + 1);
	m_Textures[nResult] = pTexture;

	return nResult;
}

CASCLockableTexture* CASCImage::InsertTexture(ASCInt nWidth, ASCInt nHeight)
{
	CASCLockableTexture* pResult = ASCFactory()->CreateLockableTexture();
	if (!pResult)
	{
		return 0;
	}

	pResult->SetWidth(nWidth);
	pResult->SetHeight(nHeight);
	pResult->SetPixelFormat(m_PixelFormat);
	pResult->SetMipMapping(m_bMipMapping);
	pResult->SetIsDynamicTexture(m_bDynamicImage);
	if (!(pResult->Initialize()))
	{
		delete pResult;
		pResult = 0;
		return 0;
	}

	ASCInt nIndex = m_Textures.size();
	m_Textures.resize(nIndex + 1);
	m_Textures[nIndex] = pResult;

	return pResult;
}

void CASCImage::RemoveTexture(ASCInt nIndex)
{
	if ((nIndex >= 0) && (nIndex < (ASCInt)(m_Textures.size())))
	{
		if (m_Textures[nIndex])
		{
			delete m_Textures[nIndex];
			m_Textures[nIndex] = 0;
		}
		m_Textures.erase(m_Textures.begin() + nIndex);
	}
}

ASCInt CASCImage::IndexOfTexture(CASCLockableTexture* pSample)
{
	for (ASCInt i = 0; i < (ASCInt)(m_Textures.size()); i++)
	{
		if (m_Textures[i] == pSample)
		{
			return i;
		}
	}
	return -1;
}

void CASCImage::RemoveAllTextures()
{
	for (ASCInt i = 0; i < (ASCInt)(m_Textures.size()); i++)
	{
		if (m_Textures[i])
		{
			delete m_Textures[i];
			m_Textures[i] = 0;
		}
	}
	m_Textures.clear();
}

ASCBoolean CASCImage::LoadFromFile(const wstring& sFileName)
{
	CASCSystemSurface* pSystemSurface = new CASCSystemSurface();
	if (!(ASCBitmapManager()->LoadFromFile(sFileName, pSystemSurface)))
	{
		delete pSystemSurface;
		pSystemSurface = 0;
		return false;
	}

	RemoveAllTextures();

	m_PixelFormat = apfA8R8G8B8;
	CASCLockableTexture* pNewTexture = InsertTexture(pSystemSurface->GetWidth(), pSystemSurface->GetHeight());
	if (!pNewTexture)
	{
		delete pSystemSurface;
		pSystemSurface = 0;
		return false;
	}

	ASCPointer pBits;
	ASCInt nPitch;
	pNewTexture->Lock(ASCBounds(0, 0, pNewTexture->GetWidth(), pNewTexture->GetHeight()), pBits, nPitch);
	if ((!pBits) || (nPitch < 1))
	{
		RemoveAllTextures();
		delete pSystemSurface;
		pSystemSurface = 0;
		return false;
	}

	ASCPointer pWritePixel = pBits;
	for (ASCInt nIndex = 0; nIndex < pSystemSurface->GetHeight(); nIndex++)
	{
		Pixel32ToASCPixelFormatXArray(pSystemSurface->GetScanline(nIndex), pWritePixel, pNewTexture->GetPixelFormat(), pSystemSurface->GetWidth());
		pWritePixel = (ASCPointer)((ASCInt)pWritePixel + nPitch);
	}

	pNewTexture->Unlock();
	delete pSystemSurface;
	pSystemSurface = 0;

	if (pNewTexture->GetMipMapping())
	{
		pNewTexture->UpdateMipmaps();
	}

	return true;
}