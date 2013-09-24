/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCUtils.h
 *  Content: ASC(Asphyre Sphinx for C++) useful function definitions
 *  Date   : 2013/05/31
 *  Desc   : Useful functions used throughout the entire framework
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <string>
using std::wstring;
#include <windows.h>
#include <Shlwapi.h>
#include "ASCTypes.h"

#pragma comment(lib, "Shlwapi.lib")

// 通过四舍五入的方式取整数
ASCInt ASCRound(ASCDouble f);

RECT ASCBounds(ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight);

void ASCPoint4FromRect(const RECT Rc, CASCPoint4& Rtn);

// Creates 4-point rectangle with the specified top left corner and the given dimensions
void ASCPoint4FromLTWH(ASCSingle fLeft, ASCSingle fTop, ASCSingle fWidth, ASCSingle fHeight, CASCPoint4& Rtn);

// Creates 4-color gradient where all colors are specified by the same source color
void ASCColor4FromColor(ASCColor uColor, CASCColor4& Color4);

// Creates 4-color gradient where each color is specified individually
void ASCColor4From4Color(ASCColor uColor1, ASCColor uColor2, ASCColor uColor3, ASCColor uColor4, CASCColor4& Rtn);

template <typename T>
inline ASCInt GetArrayLen(const T& arr)
{
	return (sizeof(arr) / sizeof(arr[0]));
}

template <typename T>
inline void SwapValue(T& v1, T& v2)
{
	T v = v1;
	v1 = v2;
	v2 = v;
}

// 返回不小于两数相除的商的最小整数
ASCInt CeilDiv(ASCInt nDividend, ASCInt nDivisor);

class CASCResourse
{
public:
	CASCResourse(const ASCPointer pHandle, const ASCUInt uSize);
	~CASCResourse();

	ASCPointer GetHandle();
	ASCUInt GetSize();
private:
	ASCPointer	m_pHandle;
	ASCUInt		m_uSize;
};

CASCResourse* LoadResource(const wstring& sFileName, ASCUInt& uSize);

wstring ExtractFileExt(const wstring& sFileName);

/*
 * 取一行的字节数
 * nPixelsPerLine: 一行多少像素
 * nBitsPerPixel: 为一像素多少位
 * nAlign: 多少位对齐
*/
ASCInt BytesPerLine(ASCInt nPixelsPerLine, ASCInt nBitsPerPixel, ASCInt nAlign);