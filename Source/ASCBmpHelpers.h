/*******************************************************************************************
 *
 *  Copyright (C) LinZhenqun / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCBmpHelpers.h
 *  Content: ASC(Asphyre Sphinx for C++) bitmap import helpers
 *  Date   : 2013/11/26
 *  Desc   : Helpers for bitmap import
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <string>
using std::wstring;

#include "ASCBitmap.h"
#include "ASCPng.h"
#include "ASCTypes.h"

/*
 * �� PNG �ļ�����λͼ: BmpLoadFromPNG
*/
ASCBoolean BmpLoadFromPNG(const wstring& sFileName, CASCBitmap* pBmp);


/*
 * �� TGA �ļ�����λͼ: BmpLoadFromTGA
*/

// TGA ͼ������
#define TGA_IMGTYPE_TRUECOLOR       2
#define TGA_IMGTYPE_RLETRUECOLOR    10

// TGA �ļ�ͷ��Ϣ
#pragma pack(push, 1)
struct CASCTGAHeader 
{
	ASCUInt8	m_nIDLen;
	ASCUInt8	m_nColorMapType;       
	ASCUInt8	m_nImgType;          
	ASCUInt8	m_nColorMapSpec[5];
	ASCUInt8	m_nOrgX;
	ASCUInt8	m_nOrgY;
	ASCUInt8	m_nWidth;
	ASCUInt8	m_nHeight;
	ASCUInt8	m_nBPP;
	ASCUInt8	m_nImgDesc;
};
#pragma pack(pop)

ASCBoolean BmpLoadFromTGA(const wstring& sFileName, CASCBitmap* pBmp);