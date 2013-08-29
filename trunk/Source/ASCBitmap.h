/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCBitmap.h
 *  Content: ASC(Asphyre Sphinx for C++) Bitmap wrapper
 *  Date   : 2013/08/29
 *  Desc   : Windows Bitmap wrapper for ASC
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <string>
using std::wstring;
#include "ASCTypes.h"
#include "ASCStreams.h"

class CASCBitmap
{
public:
	CASCBitmap();
	~CASCBitmap();

	ASCInt GetWidth();
	ASCInt GetHeight();
	CASCPixelFormat GetPixelFormat();
	void SetPixelFormat(CASCPixelFormat fmt);
	ASCBoolean GetValid();
	ASCInt GetLineBytes();
	ASCInt GetBitsBytes();

	ASCByte* GetLineBits(ASCInt nLineNo);
	ASCByte* GetBits();

	void SetBmpInfo(ASCInt nWidth, ASCInt nHeight, CASCPixelFormat fmt);

	ASCBoolean LoadFormFile(const wstring& sFileName);
	ASCBoolean LoadFromStream(CASCStream* pStream);
	ASCBoolean SaveToFile(const wstring& sFileName);
	ASCBoolean SaveToStream(CASCStream* pStream);

	// ���� DIB λͼ
	void Draw(HDC hDC, ASCInt nX, ASCInt nY);
protected:
private:
	// λͼ����
	ASCByte*		m_pBmpBits;
	ASCInt			m_nWidth;
	ASCInt			m_nHeight;
	// ÿ�г���
	ASCInt			m_nLineBytes;
	CASCPixelFormat	m_PixelFormat;
	ASCBoolean		m_bValid;
	BITMAPINFO*		m_pBmpInfo; 
};