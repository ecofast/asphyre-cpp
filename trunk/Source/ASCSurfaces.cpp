#include <stdlib.h>
#include <string.h>
#include "ASCSurfaces.h"

CASCSystemSurface::CASCSystemSurface()
{
	m_pBits = 0;
	m_nPitch = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nSearchIndex = -1;
}

CASCSystemSurface::~CASCSystemSurface()
{
	if (m_pBits)
	{
		delete m_pBits;
		m_pBits = 0;
	}
}

void CASCSystemSurface::SetSize(ASCInt nWidth, ASCInt nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nPitch = m_nWidth * 4;
	m_pBits = realloc(m_pBits, nWidth * nHeight * 4);

	Clear(0);
}

void CASCSystemSurface::Clear(ASCColor uColor)
{
	ASCUInt32* pPixel = (ASCUInt32*)m_pBits;
	for (ASCInt i = 0; i < m_nWidth * m_nHeight; i++)
	{
		*pPixel = uColor;
		pPixel++;
	}
}

void CASCSystemSurface::CopyFrom(CASCSystemSurface* pSource)
{
	if ((m_nWidth != pSource->GetWidth()) || (m_nHeight != pSource->GetHeight()))
	{
		SetSize(pSource->GetWidth(), pSource->GetHeight());
	}
	memmove(m_pBits, pSource->GetBits(), m_nWidth * m_nHeight * 4);
}

ASCPointer CASCSystemSurface::GetBits()
{
	return m_pBits;
}

ASCInt CASCSystemSurface::GetPitch()
{
	return m_nPitch;
}

ASCInt CASCSystemSurface::GetWidth()
{
	return m_nWidth;
}

ASCInt CASCSystemSurface::GetHeight()
{
	return m_nHeight;
}

ASCUInt32 CASCSystemSurface::GetPixel(ASCInt nX, ASCInt nY)
{
	if ((nX < 0) || (nY < 0) || (nX >= m_nWidth) || (nY >= m_nHeight))
	{
		return 0;
	} 
	else
	{
		return (*((ASCUInt32*)((ASCInt)m_pBits + m_nPitch * nY + nX * 4)));
	}
}

void CASCSystemSurface::SetPixel(ASCInt nX, ASCInt nY, const ASCUInt32 uValue)
{
	if ((nX < 0) || (nY < 0) || (nX >= m_nWidth) || (nY >= m_nHeight))
	{
		return;
	}
	else
	{
		*((ASCUInt32*)((ASCInt)m_pBits + m_nPitch * nY + nX * 4)) = uValue;
	}
}

ASCPointer CASCSystemSurface::GetScanline(ASCInt nIndex)
{
	if ((nIndex >= 0) && (nIndex < m_nHeight))
	{
		return ((ASCPointer)((ASCInt)m_pBits + m_nPitch * nIndex));
	}
	else
	{
		return 0;
	}
}

void CASCSystemSurface::ResetAlpha()
{
	ASCUInt32* pPixel = (ASCUInt32*)m_pBits;
	for (ASCInt i = 0; i < m_nWidth * m_nHeight; i++)
	{
		*pPixel = *pPixel | 0xFF000000;
		pPixel++;
	}
}

ASCBoolean CASCSystemSurface::HasAlphaChannel()
{
	for (ASCInt j = 0; j < m_nHeight; j++)
	{
		ASCUInt32* pPixel = (ASCUInt32*)(GetScanline(j));
		for (ASCInt i = 0; i < m_nWidth; i++)
		{
			if (((*pPixel) & 0xFF000000) > 0)
			{
				return true;
			}
			pPixel++;
		}
	}
	return false;
}