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

#include <windows.h>
#include "ASCTypes.h"

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

/*
template<typename T>
const T& max3(const T& a, const T& b, const T& c)
{
	using std::max;
	return max(max(a, b), c);
}
*/