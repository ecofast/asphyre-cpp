/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / LinZhenqun / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCBitmapsPNG.h
 *  Content: ASC(Asphyre Sphinx for C++) PNG image format connection
 *  Date   : 2013/11/27
 *  Desc   : PNG image format connection for ASC
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCBitmaps.h"

class CASCBitmapPNG : public CASCCustomBitmap
{
public:
	CASCBitmapPNG();

	virtual ASCBoolean LoadFromStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pDest);
	virtual ASCBoolean SaveToStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pSource);
};

extern inline CASCBitmapPNG* ASCBitmapPNG();