/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCBitmapsBMP.h
 *  Content: ASC(Asphyre Sphinx for C++) Windows Bitmap image format connection
 *  Date   : 2013/08/29
 *  Desc   : Windows Bitmap image format connection for ASC
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCBitmaps.h"

class CASCBitmapBMP : public CASCCustomBitmap
{
public:
	CASCBitmapBMP();

	virtual ASCBoolean LoadFromStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pDest);
	virtual ASCBoolean SaveToStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pSource);
};

extern inline CASCBitmapBMP* ASCBitmapBMP();