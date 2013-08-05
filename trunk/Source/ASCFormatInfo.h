/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCFormatInfo.h
 *  Content: ASC(Asphyre Sphinx for C++) detailed specification of different pixel formats
 *  Date   : 2013/07/26
 *  Desc   : Detailed specification of different pixel formats for ASC
 *
 *******************************************************************************************/

#pragma once

#include "ASCTypes.h"
#include "ASCUtils.h"

enum CASCChannelType
{
	actR, actG, actB, actA, actUnknown
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
const CASCFormatBitInfo C_ASCFormatBitInfos[ASC_PIXELFORMAT_ELEMNUM] = 
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

const CASCPixelFormatCategory C_PixelFormatCategories[ASC_PIXELFORMAT_ELEMNUM] = 
{
	apfcMisc, apfcRGB, apfcRGB, apfcRGB, apfcRGB, apfcRGB, apfcRGB, apfcRGB, apfcRGB
};

const ASCInt	ASC_CHANNELTYPE_ELEMNUM							= 5;
// const ASCChar	C_ChannelTextLetters[ASC_CHANNELTYPE_ELEMNUM]	= {"R", "G", "B", "A", "-"};

void ResetFormatInfo(const PASCPixelFormatInfo pInfo)
{
	pInfo->nNumChannels = 0;
	for (ASCInt i = 0; i < GetArrayLen(pInfo->Channels); i++)
	{
		pInfo->Channels[i].ChannelType		= actUnknown;
		pInfo->Channels[i].ChannelStorage	= acsUnknown;
		pInfo->Channels[i].nBits			= 0;
		pInfo->Channels[i].nPos				= -1;
	}
}

ASCInt AddChannel(const PASCPixelFormatInfo pInfo, CASCChannelType ChannelType, 
				  CASCChannelStorage ChannelStorage, ASCInt nBits, ASCInt nPos)
{
	if (pInfo->nNumChannels < 4)
	{
		ASCInt nResult = pInfo->nNumChannels;
		pInfo->Channels[nResult].ChannelType	= ChannelType;
		pInfo->Channels[nResult].ChannelStorage	= ChannelStorage;
		pInfo->Channels[nResult].nBits			= nBits;
		pInfo->Channels[nBits].nPos				= nPos;
		pInfo->nNumChannels++;

		return nResult;
	}
	else
	{
		return -1;
	}
}

ASCBoolean SameChannel(const PASCChannelInfo pChannel1, const PASCChannelInfo pChanne2)
{
	return ((pChannel1->ChannelType == pChanne2->ChannelType) && (pChannel1->ChannelStorage == pChanne2->ChannelStorage) 
		&& (pChannel1->nBits == pChanne2.nBits) && (pChannel1->nPos == pChanne2->nPos));
}

ASCBoolean SameFormat(const PASCPixelFormatInfo pInfo1, const PASCPixelFormatInfo pInfo2)
{
	ASCBoolean bMatch;
	for (ASCInt i = 0; i < pInfo1->nNumChannels; i++)
	{
		bMatch = false;
		for (ASCInt j = 0; j < pInfo2->nNumChannels; j++)
		{
			if (SameChannel(pInfo1->Channels[i], pInfo2->Channels[j]))
			{
				bMatch = true;
				break;
			}
		}

		if (!bMatch)
		{
			return false;
		}
	}
	return true;
}

CASCPixelFormatInfo GetPixelFormatInfo(CASCPixelFormat fmt)
{
	CASCPixelFormatInfo info;
	ResetFormatInfo(&info);

	switch (fmt)
	{
	case apfR8G8B8:
		AddChannel(&info, actR, acsUnsigned, 8, 16);
		AddChannel(&info, actG, acsUnsigned, 8, 8);
		AddChannel(&info, actB, acsUnsigned, 8, 0);
		break;
	case apfA8R8G8B8:
		AddChannel(&info, actA, acsUnsigned, 8, 24);
		AddChannel(&info, actR, acsUnsigned, 8, 16);
		AddChannel(&info, actG, acsUnsigned, 8, 8);
		AddChannel(&info, actB, acsUnsigned, 8, 0);
		break;
	case apfX8R8G8B8:
		AddChannel(&info, actX, acsUnsigned, 8, 24);
		AddChannel(&info, actR, acsUnsigned, 8, 16);
		AddChannel(&info, actG, acsUnsigned, 8, 8);
		AddChannel(&info, actB, acsUnsigned, 8, 0);
		break;
	case apfR5G6B5:
		AddChannel(&info, actR, acsUnsigned, 5, 11);
		AddChannel(&info, actG, acsUnsigned, 6, 5);
		AddChannel(&info, actB, acsUnsigned, 5, 0);
		break;
	case apfA1R5G5B5:
		AddChannel(&info, actA, acsUnsigned, 1, 15);
		AddChannel(&info, actR, acsUnsigned, 5, 10);
		AddChannel(&info, actG, acsUnsigned, 5, 5);
		AddChannel(&info, actB, acsUnsigned, 5, 0);
		break;
	case apfX1R5G5B5:
		AddChannel(&info, actX, acsUnsigned, 1, 15);
		AddChannel(&info, actR, acsUnsigned, 5, 10);
		AddChannel(&info, actG, acsUnsigned, 5, 5);
		AddChannel(&info, actB, acsUnsigned, 5, 0);
		break;
	case apfA4R4G4B4:
		AddChannel(&info, actA, acsUnsigned, 4, 12);
		AddChannel(&info, actR, acsUnsigned, 4, 8);
		AddChannel(&info, actG, acsUnsigned, 4, 4);
		AddChannel(&info, actB, acsUnsigned, 4, 0);
		break;
	default:
		break;
	}

	return info;
}

CASCPixelFormat PixelFormatInfoToPixelFormat(const PASCPixelFormatInfo pInfo)
{
	CASCPixelFormatInfo FmtInfo;
	for (CASCPixelFormat fmt = apfUnknown + 1; fmt <= apfA4R4G4B4; fmt++)
	{
		FmtInfo = GetPixelFormatInfo(fmt);
		if (SameFormat(FmtInfo, pInfo))
		{
			return fmt;
		}
	}
	return apfUnknown;
}

ASCBoolean IsUsefulChannel(CASCChannelType ChannelType)
{
	return ((ChannelType >= actR) && (ChannelType <= actA));
}

ASCInt GetChannelCount(const PASCPixelFormatInfo pInfo)
{
	ASCInt nResult = 0;
	for (ASCInt i = 0; i < pInfo->nNumChannels; i++)
	{
		if (IsUsefulChannel(pInfo->Channels[i].ChannelType))
		{
			nResult++;
		}
	}
	return nResult;
}

ASCInt FindChannelPos(CASCChannelType ChannelType, const PASCPixelFormatInfo pInfo)
{
	for (ASCInt i = 0; i < pInfo->nNumChannels; i++)
	{
		if (pInfo->Channels[i].ChannelType == ChannelType)
		{
			return i;
		}
	}
	return -1;
}