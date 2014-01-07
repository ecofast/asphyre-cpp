#include "ASCBitmapsPNG.h"
#include "ASCBmpHelpers.h"

CASCBitmapPNG* ASCBitmapPNG()
{
	static CASCBitmapPNG gASCBitmapPNG;
	return &gASCBitmapPNG;
}

CASCBitmapPNG::CASCBitmapPNG() : CASCCustomBitmap()
{
	m_sDesc = L"PNG";
}

ASCBoolean CASCBitmapPNG::LoadFromStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pDest)
{
	ASCBoolean bResult = true;
	CASCBitmap* pBmp = new CASCBitmap();
	bResult = BmpLoadFromPNG(((CASCFileStream*)pStream)->GetFileName(), pBmp);
	if (bResult)
	{
		ASCBoolean bUnmaskAlpha = pBmp->GetPixelFormat() != apfA8R8G8B8;
		pBmp->SetPixelFormat(apfA8R8G8B8);
		pDest->SetSize(pBmp->GetWidth(), pBmp->GetHeight());
		for (ASCInt i = 0; i < pBmp->GetHeight(); i++)
		{
			memcpy(pDest->GetScanline(i), pBmp->GetLineBits(i), pBmp->GetWidth() * 4);
		}
		if (bUnmaskAlpha)
		{
			pDest->ResetAlpha();
		}
	}
	delete pBmp;
	return bResult;
}

ASCBoolean CASCBitmapPNG::SaveToStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pSource)
{
	ASCBoolean bResult = true;
	CASCBitmap* pBmp = new CASCBitmap();
	pBmp->SetBmpInfo(pSource->GetWidth(), pSource->GetHeight(), apfA8R8G8B8);
	for (ASCInt i = 0; i < pSource->GetHeight(); i++)
	{
		memcpy(pBmp->GetLineBits(i), pSource->GetScanline(i), pSource->GetWidth() * 4);
	}
	try
	{
		pBmp->SaveToStream(pStream);
	}
	catch (...)
	{
		bResult = false;
	}
	delete pBmp;
	return bResult;
}