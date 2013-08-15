/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCPixelFormatConv.h
 *  Content: ASC(Asphyre Sphinx for C++) Pixel Format conversion
 *  Date   : 2013/07/30
 *  Desc   : Pixel format conversion utilities
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCTypes.h"

/*
 * Converts a single pixel from an arbitrary format back to 32-bit RGBA format(apfA8R8G8B8)
 * If the specified format is not supported, this function returns zero
 * Params:
 *   pSrc: Pointer to a valid block of memory where the source pixel resides
 *   SrcFmt: Pixel format that is used to describe the source pixel
 * Returns: Resulting pixel in 32-bit RGBA format(apfA8R8G8B8)
*/
ASCUInt ASCPixelFormatXTo32(ASCPointer pSrc, CASCPixelFormat SrcFmt);

/*
 * Converts a single pixel from 32-bit RGBA format(apfA8R8G8B8)) to anarbitrary format
 * If the specified format is not supported, this function does nothing
 * Params:
 *   uSrc: Source pixel specified in 32-bit RGBA format(apfA8R8G8B8)
 *   pDest: Pointer to the memory block where the resulting pixel should be written to. This memory should be previously allocated
 *   DestFmt: Pixel format that is used to describe the destination pixel
*/
void Pixel32ToASCPixelFormatX(ASCUInt uSrc, ASCPointer pDest, CASCPixelFormat DestFmt);

/*
 * Converts an array of pixels from an arbitrary format back to 32-bit RGBA format(apfA8R8G8B8)
 * If the specified format is not supported, this function does nothing
 * Params:
 *   pSrc: Source Pointer to a valid memory block that holds the source pixels
 *   pDest: Dest Pointer to a valid memory block where destination pixels will be written to
 *   SrcFmt: Pixel format that is used to describe the source pixels
 *   nElemNum: The number of pixels to convert
*/
void ASCPixelFormatXArrayTo32(ASCPointer pSrc, ASCPointer pDest, CASCPixelFormat SrcFmt, ASCInt nElemNum);

/*
 * Converts an array of pixels from 32-bit RGBA format(apfA8R8G8B8) to an arbitrary format
 * If the specified format is not supported, this function does nothing
 * Params:
 *   pSrc: Source Pointer to a valid memory block that holds the source pixels
 *   pDest: Dest Pointer to a valid memory block where destination pixels will be written to
 *   DestFmt: Pixel format that is used to describe the destination pixels
 *   nElemNum: The number of pixels to convert
*/
void Pixel32ToASCPixelFormatXArray(ASCPointer pSrc, ASCPointer pDest, CASCPixelFormat DestFmt, ASCInt nElemNum);