/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCSurfaces.h
 *  Content: ASC(Asphyre Sphinx for C++) pixel format image processing classes
 *  Date   : 2013/08/19
 *  Desc   : 32-bit and custom pixel format image processing classes
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCTypes.h"

class CASCSystemSurface
{
public:
	CASCSystemSurface();
	~CASCSystemSurface();

	void SetSize(ASCInt nWidth, ASCInt nHeight);
	void CopyFrom(CASCSystemSurface* pSource);
	void Clear(ASCColor uColor);
	void ResetAlpha();
	ASCBoolean HasAlphaChannel();

	ASCPointer GetBits();
	ASCInt GetPitch();
	ASCInt GetWidth();
	ASCInt GetHeight();
	ASCUInt32 GetPixel(ASCInt nX, ASCInt nY);
	void SetPixel(ASCInt nX, ASCInt nY, const ASCUInt32 uValue);
	ASCPointer GetScanline(ASCInt nIndex);
private:
	ASCPointer	m_pBits;
	ASCInt		m_nPitch;
	ASCInt		m_nWidth;
	ASCInt		m_nHeight;
	ASCInt		m_nSearchIndex;
};