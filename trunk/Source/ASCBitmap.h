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

	// 设位图信息(nHeight 可以为负). 原来的位图数据将丢失
	void SetBmpInfo(ASCInt nWidth, ASCInt nHeight, CASCPixelFormat fmt);

	ASCBoolean LoadFormFile(const wstring& sFileName);
	ASCBoolean LoadFromStream(CASCStream* pStream);
	ASCBoolean SaveToFile(const wstring& sFileName);
	ASCBoolean SaveToStream(CASCStream* pStream);

	// 绘制 DIB 位图
	void Draw(HDC hDC, ASCInt nX, ASCInt nY);
protected:
	ASCUInt32* BitField16Bit();
	ASCUInt32* BitField12Bit();
private:
	// 位图数据
	ASCByte*		m_pBmpBits;
	ASCInt			m_nWidth;
	ASCInt			m_nHeight;
	// 每行长度
	ASCInt			m_nLineBytes;
	CASCPixelFormat	m_PixelFormat;
	ASCBoolean		m_bValid;
	BITMAPINFO*		m_pBmpInfo; 
};