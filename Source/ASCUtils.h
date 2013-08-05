/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCUtils.h
 *  Content: ASC(Asphyre Sphinx for C++) useful function definitions
 *  Date   : 2013/05/31
 *  Desc   : Useful functions used throughout the entire framework
 *
 *******************************************************************************************/

#pragma once

#include "ASCTypes.h"

ASCInt Round(ASCSingle f)
{
	return ASCInt(f + 0.5);
}

RECT Bounds(ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight)
{
	RECT rcResult;
	rcResult.left	= nLeft;
	rcResult.top	= nTop;
	rcResult.right	= nLeft + nWidth;
	rcResult.bottom	= nTop + nHeight;
}

template <typename T>
ASCInt GetArrayLen(const T& arr)
{
	return (sizeof(arr) / sizeof(arr[0]));
}

template<typename T>
const T& max3(const T& a, const T& b, const T& c)
{
	using std::max;
	return max(max(a, b), c);
}