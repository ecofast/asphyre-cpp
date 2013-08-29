#include "ASCBitmap.h"

CASCBitmap::CASCBitmap() : m_pBmpBits(0), m_nWidth(0), m_nHeight(0), m_nLineBytes(0), m_bValid(false), m_pBmpInfo(0)
{

}

CASCBitmap::~CASCBitmap()
{
	if (m_pBmpBits)
	{
		delete m_pBmpBits;
		m_pBmpBits = 0;
	}

	if (m_pBmpInfo)
	{
		delete m_pBmpInfo;
		m_pBmpInfo = 0;
	}
}

ASCInt CASCBitmap::GetWidth()
{
	return m_nWidth;
}

ASCInt CASCBitmap::GetHeight()
{
	return m_nHeight;
}

CASCPixelFormat CASCBitmap::GetPixelFormat()
{
	return m_PixelFormat;
}

void CASCBitmap::SetPixelFormat(CASCPixelFormat fmt)
{
	if (!m_bValid)
	{
		return;
	}

	if (fmt != m_PixelFormat)
	{

	}
}

ASCBoolean CASCBitmap::GetValid()
{
	return m_bValid;
}

ASCInt CASCBitmap::GetLineBytes()
{
	return m_nLineBytes;
}

ASCInt CASCBitmap::GetBitsBytes()
{
	return (m_nLineBytes * m_nHeight);
}

ASCByte* CASCBitmap::GetLineBits(ASCInt nLineNo)
{
	if (m_pBmpInfo->bmiHeader.biHeight > 0)
		nLineNo = m_pBmpInfo->bmiHeader.biHeight - nLineNo - 1;
	return m_pBmpBits + m_nLineBytes * nLineNo;
}

ASCByte* CASCBitmap::GetBits()
{
	return m_pBmpBits;
}
