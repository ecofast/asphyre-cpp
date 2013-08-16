/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCConsts.h
 *  Content: ASC(Asphyre Sphinx for C++) Constant variable definitions
 *  Date   : 2013/05/31
 *  Desc   : Constant variables and functions used throughout the entire framework
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCTypes.h"

const ASCSingle ASCPi = 3.1415926f;

/*
 * Unknown Color individual constant. It can be used in some cases to specify
 * that no color is present or required, or to clear the rendering buffer
*/
const ASCUInt ASCColorUnknown = 0x00000000;

// White Color individual constant. In some cases it can be used when no color is to be specified
const ASCUInt ASCColorWhite = 0xFFFFFFFF;

// Black Color individual constant. It can be used in certain circumstances, for instance, to draw a shadow of the image
const ASCUInt ASCColorBlack = 0xFF000000;

/*
 * Opaque Color individual constant. This one can be used in certain cases
 * where the color of the image is to preserved but the result should be
 * completely transparent
*/
const ASCUInt ASCColorOpaque = 0x00FFFFFF;

/*
 * Creates 32-bit RGBA color using the specified individual components for
 * red, green, blue and alpha channel
 */
ASCUInt ASCColorFromRGB(ASCInt nR, ASCInt nG, ASCInt nB, ASCInt nA = 255)
{
	return (ASCUInt(nB) | (ASCUInt(nG) << 8) | (ASCUInt(nR) << 16) | (ASCUInt(nA) << 24));
}

/*
 * Creates 32-bit RGBA color using the specified grayscale value with
 * alpha-channel set to 255
*/
ASCUInt ASCColorFromGrayScale(ASCInt nGray)
{
	return ((ASCUInt(nGray) & 0xFF) | ((ASCUInt(nGray) & 0xFF) << 8) | ((ASCUInt(nGray) & 0xFF) << 16) | 0xFF000000);
}

/*
 * Creates 32-bit RGBA color with the specified alpha-channel and each of red,
 * green and blue components set to 255
*/
ASCUInt ASCColorWithSpecAlpha(ASCInt nAlpha)
{
	return (0x00FFFFFF | ((ASCUInt(nAlpha) & 0xFF) << 24));
}