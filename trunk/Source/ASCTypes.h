/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCTypes.h
 *  Content: ASC(Asphyre Sphinx for C++) types and definitions
 *  Date   : 2013/05/31
 *  Desc   : Essential types, constants and functions that work with colors, pixels and
 *           rectangles that are used throughout the entire framework
 *
 *******************************************************************************************/

#pragma once

#include "ASCIntVector2D.h"
#include "ASCFloatVector2D.h"

typedef	int					ASCInt;
typedef	unsigned int		ASCUInt;

typedef ASCInt				ASCInt32;
typedef ASCUInt				ASCUInt32;

typedef	ASCUInt32			ASCColor;

typedef	float				ASCSingle;
typedef double				ASCDouble;

typedef bool				ASCBoolean;

typedef void*				ASCPointer;

typedef void (*CASCNotifyEvent)(void);

// TObject in Delphi
typedef void*				CASCObject;

/*
 * Defines how individual pixels and their colors are encoded in the images and
 * textures. The order of letters in the constants defines the order of the
 * encoded components; R stands for Red, G for Green, B for Blue, A for Alpha,
 * L for Luminance and X for Not Used (or discarded). Letters such as V and U
 * define displacement for bump-mapped textures and the rest are miscellaneous
*/
enum CASCPixelFormat {
	/*
	 * Unknown pixel format. It is usually returned when no valid pixel format is
	 * available. In some cases, it can be specified to indicate that the format
	 * should be selected by default or automatically
	*/
	apfUnknown,
	/*
	 * 24-bit RGB pixel format. This format can be used for storage and it is
	 * unsuitable for rendering both on DirectX and OpenGL
	 */
	apfR8G8B8, 
	/*
	 * 32-bit RGBA pixel format. The most commonly used pixel format for storing
	 * and loading textures and images
	*/
	apfA8R8G8B8, 
	/*
	 * 32-bit RGB pixel format that has no alpha-channel. Should be used for
	 * images and textures that have no transparency information in them
	*/
	apfX8R8G8B8, 
	/*
	 * 16-bit RGB pixel format. This format can be used as an alternative to
	 * A8R8G8B8 in cases where memory footprint is important at the expense
	 * of visual quality
	*/
	apfR5G6B5, 
	/*
	 * 16-bit RGBA pixel format with one bit dedicated for alpha-channel. This
	 * format can be used for images where a transparency mask is used; that is,
	 * the pixel is either transparent or not, typical for those images where
	 * a single color is picked to be transparent
	*/
	apfA1R5G5B5, 
	/*
	 * 16-bit RGB pixel format with only 15 bits used for actual storage. This
	 * format was common on older hardware many years ago but today it is rarely
	 * used or even supported
	*/
	apfX1R5G5B5,
	/*
	 * 16-bit RGBA pixel format with 4 bits for each channel. This format can be
	 * used as a replacement for A8R8G8B8 format in cases where memory
	 * footprint is important at the expense of visual quality
	*/
	apfA4R4G4B4, 
	/*
	 * 16-bit RGB pixel format with 4 bits unused. It is basically A4R4G4B4 with 
	 * alpha-channel discarded. This format is widely supported, but in typical applications 
	 * it is more convenient to use R5G6B5 instead
	*/
	apfX4R4G4B4
};

typedef CASCPixelFormat* PASCPixelFormat;

/*
 * This constant has values defined for every possible combination of CASCPixelFormat 
 * and indicates the total number of bits used for each particular pixel format
*/
const ASCInt CASCPixelFormatBits[sizeof(CASCPixelFormat)] = {0, 24, 32, 32, 16, 16, 16, 16, 16};

/*
 * A combination of two colors, primarily used for displaying text with the
 * first color being on top and the second being on bottom. The format for
 * specifying colors is defined as A8R8G8B8
*/
typedef ASCUInt32 CASCColor2[2];
typedef CASCColor2* PASCColor2;

/*
 * A combination of four colors, primarily used for displaying images and
 * rectangles with the colors corresponding to each of the vertices. The
 * colors are specified on clockwise order: top-left, top-right, bottom-right
 * and bottom-left. The format for specifying colors is defined as A8R8G8B8
*/
typedef ASCUInt32 CASCColor4[4];
typedef CASCColor4* PASCColor4;

/*
 * A combination of four 2D floating-point vectors that define a rectangle,
 * mainly used for drawing rectangular primitives and images. The vertices are
 * specified on clockwise order: top-left, top-right, bottom-right and bottom-left
*/
typedef CASCFloatVector2D CASCPoint4[4];
typedef CASCPoint4* PASCPoint4;