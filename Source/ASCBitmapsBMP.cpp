#include "ASCBitmapsBMP.h"

CASCBitmapBMP* ASCBitmapBMP()
{
	static CASCBitmapBMP gASCBitmapBMP;
	return &gASCBitmapBMP;
}

CASCBitmapBMP::CASCBitmapBMP()
{
	m_sDesc = L"Windows Bitmap";
}

ASCBoolean CASCBitmapBMP::LoadFromStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pDest)
{
	
	return true;
}

ASCBoolean CASCBitmapBMP::SaveToStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pSource)
{
	return true;
}