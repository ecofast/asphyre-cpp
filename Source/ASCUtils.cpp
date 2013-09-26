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

void ASCZeroPoint4(CASCPoint4& Rtn)
{
	Rtn[0].X = 0;
	Rtn[0].Y = 0;
	Rtn[1].X = 0;
	Rtn[1].Y = 0;
	Rtn[2].X = 0;
	Rtn[2].Y = 0;
	Rtn[3].X = 0;
	Rtn[3].Y = 0;
}

void ASCPoint4From8Values(ASCSingle fX1, ASCSingle fY1, ASCSingle fX2, ASCSingle fY2, ASCSingle fX3, ASCSingle fY3, ASCSingle fX4, ASCSingle fY4, CASCPoint4& Rtn)
{
	Rtn[0].X = fX1;
	Rtn[0].Y = fY1;
	Rtn[1].X = fX2;
	Rtn[1].Y = fY2;
	Rtn[2].X = fX3;
	Rtn[2].Y = fY3;
	Rtn[3].X = fX4;
	Rtn[3].Y = fY4;
}

void ASCPoint4FromLTWHScaled(ASCSingle fLeft, ASCSingle fTop, ASCSingle fWidth, ASCSingle fHeight, ASCSingle fScale, CASCPoint4& Rtn)
{
	ASCPoint4FromLTWH(fLeft, fTop, (ASCSingle)ASCRound(fWidth * fScale), (ASCSingle)ASCRound(fHeight * fScale), Rtn);
}

void ASCPoint4FromLTWHScaledXY(ASCSingle fLeft, ASCSingle fTop, ASCSingle fWidth, ASCSingle fHeight, ASCSingle fScaleX, ASCSingle fScaleY, CASCPoint4& Rtn)
{
	ASCPoint4FromLTWH(fLeft, fTop, (ASCSingle)ASCRound(fWidth * fScaleX), (ASCSingle)ASCRound(fHeight * fScaleY), Rtn);
}

ASCUInt ValueOfRGBA(ASCUInt uR, ASCUInt uG, ASCUInt uB, ASCUInt uA /*= 0xFF*/)
{
	return (uR | (uG << 8) | (uB << 16) | (uA << 24));
}

void ASCColor4FromRGBA(ASCUInt uR, ASCUInt uG, ASCUInt uB, ASCUInt uA, CASCColor4& Rtn)
{
	ASCColor4FromColor(ValueOfRGBA(uR, uG, uB, uA), Rtn);
}

void ASCPoint4Rotated(const CASCFloatVector2D& Origin, const CASCFloatVector2D& Size, const CASCFloatVector2D& Middle, ASCSingle fAngle, 
					  ASCSingle fScaleX, ASCSingle fScaleY, CASCPoint4& Rtn)
{
	ASCSingle fCosPhi = cos(fAngle);
	ASCSingle fSinPhi = sin(fAngle);
	// create 4 points centered at (0, 0)
	ASCPoint4FromLTWH(-Middle.X, -Middle.Y, Size.X, Size.Y, Rtn);
	CASCFloatVector2D pt2;
	// process the created points
	for (ASCInt i = 0; i < 4; i++)
	{
		// scale the point
		Rtn[i].X = Rtn[i].X * fScaleX;
		Rtn[i].Y = Rtn[i].Y * fScaleY;
		// rotate the point around Phi
		pt2.X = (Rtn[i].X * fCosPhi) - (Rtn[i].Y * fSinPhi);
		pt2.Y = (Rtn[i].Y * fCosPhi) - (Rtn[i].X * fSinPhi);
		// translate the point to (Origin)
		Rtn[i].X = pt2.X + Origin.X;
		Rtn[i].Y = pt2.Y + Origin.Y;
	}
}

void ASCPoint4RotatedCentered(const CASCFloatVector2D& Origin, const CASCFloatVector2D& Size, ASCSingle fAngle, ASCSingle fScaleX, ASCSingle fScaleY, CASCPoint4& Rtn)
{
	ASCPoint4Rotated(Origin, Size, CASCFloatVector2D((ASCSingle)(Size.X * 0.5), (ASCSingle)(Size.Y * 0.5)), fAngle, fScaleX, fScaleY, Rtn);
}

void ASCPoint4RotatedTransFormed(ASCSingle fX, ASCSingle fY, ASCSingle fX1, ASCSingle fY1, ASCSingle fX2, ASCSingle fY2, ASCSingle fX3, ASCSingle fY3, 
								 ASCSingle fX4, ASCSingle fY4, ASCSingle fCenterX, ASCSingle fCenterY, ASCSingle fAngle, ASCSingle 
								 fScaleX, ASCSingle fScaleY, CASCPoint4& Rtn)
{
	ASCSingle fCosPhi = cos(fAngle);
	ASCSingle fSinPhi = sin(fAngle);
	// create 4 points centered at (0, 0)
	Rtn[0].X = fX1 - fCenterX;
	Rtn[0].Y = fY1 - fCenterY;
	Rtn[1].X = fX2 - fCenterX;
	Rtn[1].Y = fY2 - fCenterY;
	Rtn[2].X = fX3 - fCenterX;
	Rtn[2].Y = fY3 - fCenterY;
	Rtn[3].X = fX4 - fCenterX;
	Rtn[3].Y = fY4 - fCenterY;
	CASCFloatVector2D pt2;
	// process the created points
	for (ASCInt i = 0; i < 4; i++)
	{
		// scale the point
		Rtn[i].X = Rtn[i].X * fScaleX;
		Rtn[i].Y = Rtn[i].Y * fScaleY;
		// rotate the point around Phi
		pt2.X = (Rtn[i].X * fCosPhi) - (Rtn[i].Y * fSinPhi);
		pt2.Y = (Rtn[i].Y * fCosPhi) - (Rtn[i].X * fSinPhi);
		// translate the point to (Origin)
		Rtn[i].X = pt2.X + fX;
		Rtn[i].Y = pt2.Y + fY;
	}
}

ASCInt ASCTrunc(ASCDouble f)
{
	return (ASCInt)f;
}

wstring ExtractCurrPath()
{
	wchar_t wc[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, wc);
	return wc;
}

wstring ExtractCurrModulePath()
{
	wchar_t wc[MAX_PATH];
	GetModuleFileName(0, wc, MAX_PATH);
	*wcsrchr(wc, L'\\') = L'\0';
	return wc;
}