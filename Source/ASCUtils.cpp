#include "ASCUtils.h"

ASCInt ASCRound(ASCDouble f)
{
	return (f >= 0 ? (ASCInt)(f + 0.5f) : (ASCInt)(f - 0.5f));  // return (ASCInt)(f + 0.5);
}

RECT ASCBounds( ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight )
{
	RECT rcResult;
	rcResult.left = nLeft;
	rcResult.top = nTop;
	rcResult.right = nLeft + nWidth;
	rcResult.bottom = nTop + nHeight;
	return rcResult;
}

void ASCPoint4FromRect(const RECT Rc, CASCPoint4& Rtn)
{
	Rtn[0].X = (ASCSingle)Rc.left;
	Rtn[0].Y = (ASCSingle)Rc.top;
	Rtn[1].X = (ASCSingle)Rc.right;
	Rtn[1].Y = (ASCSingle)Rc.top;
	Rtn[2].X = (ASCSingle)Rc.right;
	Rtn[2].Y = (ASCSingle)Rc.bottom;
	Rtn[3].X = (ASCSingle)Rc.left;
	Rtn[3].Y = (ASCSingle)Rc.bottom;
}

void ASCColor4FromColor(ASCColor uColor, CASCColor4& Rtn)
{
	Rtn[0] = uColor;
	Rtn[1] = uColor;
	Rtn[2] = uColor;
	Rtn[3] = uColor;
}

void ASCPoint4FromLTWH(ASCSingle fLeft, ASCSingle fTop, ASCSingle fWidth, ASCSingle fHeight, CASCPoint4& Rtn)
{
	Rtn[0].X = fLeft;
	Rtn[0].Y = fTop;
	Rtn[1].X = fLeft + fWidth;
	Rtn[1].Y = fTop;
	Rtn[2].X = fLeft + fWidth;
	Rtn[2].Y = fTop + fHeight;
	Rtn[3].X = fLeft;
	Rtn[3].Y = fTop + fHeight;
}

void ASCColor4From4Color(ASCColor uColor1, ASCColor uColor2, ASCColor uColor3, ASCColor uColor4, CASCColor4& Rtn)
{
	Rtn[0] = uColor1;
	Rtn[1] = uColor2;
	Rtn[2] = uColor3;
	Rtn[3] = uColor4;
}

CASCResourse::CASCResourse(const ASCPointer pHandle, const ASCUInt uSize)
{
	m_pHandle = pHandle;
	m_uSize = uSize;
}

CASCResourse::~CASCResourse()
{
	free(m_pHandle);
}

ASCPointer CASCResourse::GetHandle()
{
	return m_pHandle;
}

ASCUInt CASCResourse::GetSize()
{
	return m_uSize;
}

CASCResourse* LoadResource(const wstring& sFileName, ASCUInt& uSize)
{
	if (sFileName == L"")
	{
		return 0;
	}

	// load from file
	HANDLE h = CreateFile(sFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
	if (INVALID_HANDLE_VALUE == h)
	{
		return 0;
	}

	ASCUInt uFileSize = GetFileSize(h, 0);
	ASCPointer pData = malloc(uFileSize);
	if (!pData)
	{
		CloseHandle(h);
		return 0;
	}

	ASCDWord uBytesRead;
	if (!ReadFile(h, pData, uFileSize, &uBytesRead, 0))
	{
		CloseHandle(h);
		free(pData);
		return 0;
	}

	CloseHandle(h);
	uSize = uBytesRead;
	return (new CASCResourse(pData, uSize));
}

wstring ExtractFileExt(const wstring& sFileName)
{
	return (PathFindExtensionW(sFileName.c_str()));
}

ASCInt BytesPerLine(ASCInt nPixelsPerLine, ASCInt nBitsPerPixel, ASCInt nAlign)
{
	return ((nPixelsPerLine * nBitsPerPixel + nAlign - 1) &~ (nAlign - 1)) >> 3;
}

ASCInt CeilDiv(ASCInt nDividend, ASCInt nDivisor)
{
	ASCInt nResult = nDividend / nDivisor;
	if (nDividend % nDivisor > 0) 
		++nResult;
	return nResult;
}