#include "ASCBitmap.h"
#include "ASCUtils.h"
#include "ASCGraphics.h"

CASCBitmap::CASCBitmap() : m_pBmpBits(0), m_nWidth(0), m_nHeight(0), m_nLineBytes(0), m_bValid(false), m_pBmpInfo(0)
{

}

CASCBitmap::~CASCBitmap()
{
	if (m_pBmpBits)
	{
		free(m_pBmpBits);
		m_pBmpBits = 0;
	}

	if (m_pBmpInfo)
	{
		free(m_pBmpInfo);
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
		pfnProcLineScale ProcLineConv = GetLineScaleProc(m_PixelFormat, fmt);
		if (!ProcLineConv)
		{
			return;
		}
		
		ASCInt nLineBytes = BytesPerLine(m_nWidth, cASCPixelFormatBits[fmt], 32);
		ASCByte* pNewBits = (ASCByte*)malloc(nLineBytes * m_nHeight);
		ASCByte* pDestBits = pNewBits;
		ASCByte* pOldBits = m_pBmpBits;

		for (ASCInt i = 0; i < m_nHeight; i++)
		{
			ProcLineConv(pNewBits, pOldBits, m_nWidth);
			pNewBits += nLineBytes;
			pOldBits += m_nLineBytes;
		}

		SetBmpInfo(m_nWidth, m_pBmpInfo->bmiHeader.biHeight, fmt);
		memcpy(m_pBmpBits, pDestBits, nLineBytes * m_nHeight);
		free(pDestBits);
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

void CASCBitmap::SetBmpInfo(ASCInt nWidth, ASCInt nHeight, CASCPixelFormat fmt)
{
	m_bValid = true;

	if (m_pBmpBits)
	{
		free(m_pBmpBits);
		m_pBmpBits = 0;
	}

	m_PixelFormat = fmt;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nLineBytes = BytesPerLine(nWidth, cASCPixelFormatBits[m_PixelFormat], 32);
	m_pBmpBits = (ASCByte*)malloc(GetBitsBytes());

	if (m_pBmpInfo)
	{
		free(m_pBmpInfo);
		m_pBmpInfo = 0;
	}

	ASCInt nInfoSize = sizeof(BITMAPINFOHEADER);
	if ((m_PixelFormat == apfA4R4G4B4) || (m_PixelFormat == apfR5G6B5))
	{
		nInfoSize += sizeof(RGBQUAD) * 3;
	}

	m_pBmpInfo = (BITMAPINFO*)malloc(nInfoSize);
	memset(m_pBmpInfo, 0, nInfoSize);
	m_pBmpInfo->bmiHeader.biSize = nInfoSize;
	m_pBmpInfo->bmiHeader.biWidth = nWidth;
	m_pBmpInfo->bmiHeader.biHeight = nHeight;
	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = cASCPixelFormatBits[m_PixelFormat];
	m_pBmpInfo->bmiHeader.biSizeImage = GetBitsBytes();

	if (m_PixelFormat == apfA4R4G4B4)
	{
		m_pBmpInfo->bmiHeader.biCompression = BI_BITFIELDS;
		memcpy(m_pBmpInfo->bmiColors, BitField12Bit(), 3 * sizeof(RGBQUAD));
	}
	else if (m_PixelFormat == apfR5G6B5)
	{
		m_pBmpInfo->bmiHeader.biCompression = BI_BITFIELDS;
		memcpy(m_pBmpInfo->bmiColors, BitField16Bit(), 3 * sizeof(RGBQUAD));
	}
	else
	{
		m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	}
}

ASCUInt32* CASCBitmap::BitField16Bit()
{
	static ASCUInt32 uBitField[3] = {0xF800, 0x07E0, 0x001F};
	return uBitField;
}

ASCUInt32* CASCBitmap::BitField12Bit()
{
	static ASCUInt32 uBitField[3] = {0x0F00, 0x00F0, 0x000F};
	return uBitField;
}

ASCBoolean CASCBitmap::LoadFormFile(const wstring& sFileName)
{
	CASCFileStream fs;
	if (fs.Open(sFileName))
		return LoadFromStream(&fs);
	return false;
}

ASCBoolean CASCBitmap::LoadFromStream(CASCStream* pStream)
{
	m_bValid = false;
	BITMAPFILEHEADER bfh;
	if ((!pStream->ReadBuffer(&bfh, sizeof(bfh))) || (bfh.bfType != 0x4D42))
	{
		// _ASSERTE(!"DIB format error!");
		return false;
	}

	BITMAPINFOHEADER bih;
	if (!pStream->ReadBuffer(&bih, sizeof(bih)) || bih.biSize < sizeof(BITMAPINFOHEADER) ||
		(bih.biCompression != BI_RGB && bih.biCompression != BI_BITFIELDS) || 
		(bih.biBitCount != 16 && bih.biBitCount != 24 && bih.biBitCount != 32))
	{
		// _ASSERTE(!"DIB format are not support or error!");
		return false;
	}

	CASCPixelFormat format;
	if (bih.biBitCount == 24)
		format = apfR8G8B8;
	else if (bih.biBitCount == 32)
		format = apfA8R8G8B8;
	else if (bih.biBitCount == 16 && bih.biCompression == BI_RGB)
		format = apfA1R5G5B5;
	else if (bih.biBitCount == 16 && bih.biCompression == BI_BITFIELDS)
	{
		ASCUInt32 uBitFields[3];
		if (!pStream->ReadBuffer(uBitFields, 3 * sizeof(ASCUInt32)))
		{
			// _ASSERTE(!"DIB format error!");
			return false;
		}
		if (memcmp(uBitFields, BitField16Bit(), 3 * sizeof(ASCUInt32)) == 0)
			format = apfR5G6B5;
		else if (memcmp(uBitFields, BitField12Bit(), 3 * sizeof(ASCUInt32)) == 0)
			format = apfA4R4G4B4;
		else
		{
			// _ASSERTE(!"DIB format are not support or error!");
			return false;
		}
	}

	SetBmpInfo(bih.biWidth, bih.biHeight, format);

	if (pStream->Seek(bfh.bfOffBits) != bfh.bfOffBits)
	{
		// _ASSERTE(!"DIB format error!");
		return false;
	}

	if (!pStream->ReadBuffer(m_pBmpBits, GetBitsBytes()))
	{
		// _ASSERTE(!"DIB format error!");
		return false;
	}

	m_bValid = true;
	return true;
}

ASCBoolean CASCBitmap::SaveToFile(const wstring& sFileName)
{
	CASCFileStream fstm;
	if (fstm.Create(sFileName))
		return SaveToStream(&fstm);
	return false;
}

ASCBoolean CASCBitmap::SaveToStream(CASCStream* pStream)
{
	if (!GetValid())
		return false;

	int nImgSize = GetBitsBytes();
	int nInfoSize = sizeof(BITMAPINFOHEADER);
	if (m_PixelFormat == apfA4R4G4B4 || m_PixelFormat == apfR5G6B5)
		nInfoSize += 3 * sizeof(ASCUInt32);

	BITMAPFILEHEADER bif;
	bif.bfType = 0x4D42;
	bif.bfSize = sizeof(bif) + nInfoSize + nImgSize; 
	bif.bfReserved1 = 0;
	bif.bfReserved2 = 0;
	bif.bfOffBits = sizeof(bif) + nInfoSize;
	if (!pStream->WriteBuffer(&bif, sizeof(bif)))
	{
		// _ASSERTE(!"Writer DIB error");
		return false;
	}

	if (!pStream->WriteBuffer(m_pBmpInfo, nInfoSize))
	{
		// _ASSERTE(!"Writer DIB error");
		return false;
	}

	if (!pStream->WriteBuffer(m_pBmpBits, nImgSize))
	{
		// _ASSERTE(!"Writer DIB error");
		return false;
	}

	return true;
}

void CASCBitmap::Draw(HDC hDC, ASCInt nX, ASCInt nY)
{
	::SetDIBitsToDevice(hDC, nX, nY, m_nWidth, m_nHeight, 0, 0, 0, m_nHeight, m_pBmpBits, m_pBmpInfo, DIB_RGB_COLORS);
}