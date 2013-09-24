/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCFormatInfo.h
 *  Content: ASC(Asphyre Sphinx for C++) detailed specification of different pixel formats
 *  Date   : 2013/07/26
 *  Desc   : Detailed specification of different pixel formats for ASC
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCTypes.h"
#include "ASCUtils.h"

enum CASCChannelType
{
	actR, actG, actB, actA, actX, actUnknown
};

enum CASCChannelStorage
{
	acsUnsigned, acsSigned, acsFloat, acsUnknown
};

typedef struct CASCChannelInfo
{
	CASCChannelType		ChannelType;
	CASCChannelStorage	ChannelStorage;
	ASCInt				nBits;
	ASCInt				nPos;
} *PASCChannelInfo;

typedef struct CASCPixelFormatInfo
{
	ASCInt			nNumChannels;
	CASCChannelInfo	Channels[4];
} *PASCPixelFormatInfo;

typedef struct CASCFormatBitInfo
{
	ASCInt nRPos;
	ASCInt nRNum;
	ASCInt nGPos;
	ASCInt nGNum;
	ASCInt nBPos;
	ASCInt nBNum;
	ASCInt nAPos;
	ASCInt nANum;
} *PASCFormatBitInfo;

enum CASCPixelFormatCategory
{
	apfcRGB,/* apfcLuminance, apfcRGBf, */apfcMisc
};

// R-G-B-A
const CASCFormatBitInfo cFormatBitInfos[cASCPixelFormatElemNum] = 
{
	(-1,  0, -1,  0, -1,  0, -1,  0),  // apfUnknown
	(16,  8,  8,  8,  0,  8, -1,  0),  // apfR8G8B8
	(16,  8,  8,  8,  0,  8, 24,  8),  // apfA8R8G8B8
	(16,  8,  8,  8,  0,  8, -1,  0),  // apfX8R8G8B8
	(11,  5,  5,  6,  0,  5, -1,  0),  // apfR5G6B5
	(10,  5,  5,  5,  0,  5, 15,  1),  // apfA1R5G5B5
	(10,  5,  5,  5,  0,  5, -1,  0),  // apfX1R5G5B5
	( 8,  4,  4,  4,  0,  4, 12,  4),  // apfA4R4G4B4
	( 8,  4,  4,  4,  0,  4, -1,  0)   // apfX4R4G4B4
};

const CASCPixelFormatCategory cPixelFormatCategories[cASCPixelFormatElemNum] = 
{
	apfcMisc, apfcRGB, apfcRGB, apfcRGB, apfcRGB, apfcRGB, apfcRGB, apfcRGB, apfcRGB
};

const ASCInt	cChannelTypeElemNum							= 5;
// const ASCChar	cChannelTextLetters[cChannelTypeElemNum]	= {"R", "G", "B", "A", "-"};

void ResetFormatInfo(const PASCPixelFormatInfo pInfo);

ASCInt AddChannel(const PASCPixelFormatInfo pInfo, CASCChannelType ChannelType, 
				  CASCChannelStorage ChannelStorage, ASCInt nBits, ASCInt nPos);

ASCBoolean SameChannel(const PASCChannelInfo pChannel1, const PASCChannelInfo pChanne2);

ASCBoolean SameFormat(const PASCPixelFormatInfo pInfo1, const PASCPixelFormatInfo pInfo2);

CASCPixelFormatInfo GetPixelFormatInfo(CASCPixelFormat fmt);

CASCPixelFormat PixelFormatInfoToPixelFormat(const PASCPixelFormatInfo pInfo);

ASCBoolean IsUsefulChannel(CASCChannelType ChannelType);

ASCInt GetChannelCount(const PASCPixelFormatInfo pInfo);

ASCInt FindChannelPos(CASCChannelType ChannelType, const PASCPixelFormatInfo pInfo);