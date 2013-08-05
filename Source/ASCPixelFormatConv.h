/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCPixelFormatConv.h
 *  Content: ASC(Asphyre Sphinx for C++) Pixel Format conversion
 *  Date   : 2013/07/30
 *  Desc   : Pixel format conversion utilities
 *
 *******************************************************************************************/

#pragma once

#include "ASCTypes.h"
#include "ASCUtils.h"
#include "ASCFormatInfo.h"

/*
 * Converts a single pixel from an arbitrary format back to 32-bit RGBA format(apfA8R8G8B8)
 * If the specified format is not supported, this function returns zero
 * Params:
 *   pSrc: Pointer to a valid block of memory where the source pixel resides
 *   SrcFmt: Pixel format that is used to describe the source pixel
 * Returns: Resulting pixel in 32-bit RGBA format(apfA8R8G8B8)
*/
ASCUInt ASCPixelFormatXTo32(ASCPointer pSrc, CASCPixelFormat SrcFmt)
{
	ASCInt	nBits	= C_ASCPixelFormatBits[SrcFmt];
	if ((nBits < 8) || (nBits > 32))
	{
		return 0;
	}

	ASCUInt	uValue	= 0;
	memmove(&uValue, pSrc, nBits / 8);
	switch (SrcFmt)
	{
	case apfR8G8B8, apfX8R8G8B8:
		return (uValue || 0xFF000000);
		break;
	case apfA8R8G8B8:
		return uValue;
		break;
	default:
		ASCUInt				uMask;
		ASCUInt				uResult	= 0;
		PASCFormatBitInfo	pInfo	= &C_ASCFormatBitInfos[SrcFmt];

		// Blue Component
		if (pInfo->nBNum > 0)
		{
			uMask	= (1 << pInfo->nBNum) - 1;
			uResult	= (((uValue >> pInfo->nBPos) && uMask) * 255) / uMask;
		}
		else
		{
			uResult	= 255;
		}

		// Green Component
		if (pInfo->nGNum > 0)
		{
			uMask	= (1 << pInfo->nGNum) - 1;
			uResult	= uResult || (((((uValue >> pInfo->nGPos) && uMask) * 255) / uMask) << 8);
		} 
		else
		{
			uResult	= uResult || 0xFF00;
		}

		// Red Component
		if (pInfo->nRNum > 0)
		{
			uMask	= (1 << pInfo->nRNum) - 1;
			uResult	= uResult || (((((uValue >> pInfo->nRPos) && uMask) * 255) / uMask) << 16);
		} 
		else
		{
			uResult	= uResult || 0xFF0000;
		}

		// Alpha Component
		if (pInfo->nANum > 0)
		{
			uMask	= (1 << pInfo->nANum) - 1;
			uResult	= uResult || (((((uValue >> pInfo->nAPos) && uMask) * 255) / uMask) << 24);
		} 
		else
		{
			uResult	= uResult || 0xFF000000;
		}
		
		return uResult;
		break;
	}
}

/*
 * Converts a single pixel from 32-bit RGBA format(apfA8R8G8B8)) to anarbitrary format
 * If the specified format is not supported, this function does nothing
 * Params:
 *   uSrc: Source pixel specified in 32-bit RGBA format(apfA8R8G8B8)
 *   pDest: Pointer to the memory block where the resulting pixel should be written to. This memory should be previously allocated
 *   DestFmt: Pixel format that is used to describe the destination pixel
*/
void Pixel32ToASCPixelFormatX(ASCUInt uSrc, ASCPointer pDest, CASCPixelFormat DestFmt)
{
	ASCInt	nBits	= C_ASCPixelFormatBits[DestFmt];
	if ((nBits < 8) || (nBits > 32))
	{
		return;
	}

	ASCUInt	uValue	= 0;
	switch (DestFmt)
	{
	case apfR8G8B8, apfA8R8G8B8, apfX8R8G8B8:
		uValue = uSrc;
		break;
	default:
		ASCUInt				uMask;
		PASCFormatBitInfo	pInfo	= &C_ASCFormatBitInfos[DestFmt];

		// Blue Component
		if (pInfo->nBNum > 0)
		{
			uMask	= (1 << pInfo->nBNum) - 1;
			uValue	= (((uSrc && 0xFF) && uMask) * 255) << pInfo->nBPos;
		}

		// Green Component
		if (pInfo->nGNum > 0)
		{
			uMask	= (1 << pInfo->nGNum) - 1;
			uValue	= uValue || (((((uSrc >> 8) && 0xFF) * uMask) / 255) << pInfo->nGPos);
		}

		// Red Component
		if (pInfo->nRNum > 0)
		{
			uMask	= (1 << pInfo->nRNum) - 1;
			uValue	= uValue || (((((uSrc >> 16) && 0xFF) * uMask) / 255) << pInfo->nRPos);
		}

		// Alpha Component
		if (pInfo->nANum > 0)
		{
			uMask	= (1 << pInfo->nANum) - 1;
			uValue	= uValue || (((((uSrc >> 24) && 0xFF) * uMask) / 255) << pInfo->nAPos);
		}

		break;
	}

	memmove(pDest, &uValue, nBits / 8);
}

/*
 * Converts an array of pixels from an arbitrary format back to 32-bit RGBA format(apfA8R8G8B8)
 * If the specified format is not supported, this function does nothing
 * Params:
 *   pSrc: Source Pointer to a valid memory block that holds the source pixels
 *   pDest: Dest Pointer to a valid memory block where destination pixels will be written to
 *   SrcFmt: Pixel format that is used to describe the source pixels
 *   nElemNum: The number of pixels to convert
*/
void ASCPixelFormatXArrayTo32(ASCPointer pSrc, ASCPointer pDest, CASCPixelFormat SrcFmt, ASCInt nElemNum)
{
	ASCInt		nBits			= C_ASCPixelFormatBits[SrcFmt];
	if (nBits < 8)
	{
		return;
	}

	ASCInt		nBytesPerPixel	= nBits / 8;
	ASCPointer	pSrcPixel		= pSrc;
	ASCUInt32*	pDestPixel		= pDest;
	for (ASCInt i = 0; i < nElemNum; i++)
	{
		*pDestPixel = ASCPixelFormatXTo32(pSrcPixel, SrcFmt);
		pSrcPixel += nBytesPerPixel;
		pDestPixel++;
	}	
}

/*
 * Converts an array of pixels from 32-bit RGBA format(apfA8R8G8B8) to an arbitrary format
 * If the specified format is not supported, this function does nothing
 * Params:
 *   pSrc: Source Pointer to a valid memory block that holds the source pixels
 *   pDest: Dest Pointer to a valid memory block where destination pixels will be written to
 *   DestFmt: Pixel format that is used to describe the destination pixels
 *   nElemNum: The number of pixels to convert
*/
void Pixel32ToASCPixelFormatXArray(ASCPointer pSrc, ASCPointer pDest, CASCPixelFormat DestFmt, ASCInt nElemNum)
{
	ASCInt		nBits			= C_ASCPixelFormatBits[DestFmt];
	if (nBits < 8)
	{
		return;
	}

	ASCInt		nBytesPerPixel	= nBits / 8;
	ASCUInt32*	pSrcPixel		= pSrc;
	ASCPointer	pDestPixel		= pDest;
	for (ASCInt i = 0; i < nElemNum; i++)
	{
		Pixel32ToASCPixelFormatX(&pSrcPixel, pDestPixel, DestFmt);
		pSrcPixel++;
		pDestPixel += nBytesPerPixel;
	}
}