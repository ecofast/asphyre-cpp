/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCPixelFormats.h
 *  Content: ASC(Asphyre Sphinx for C++) supported pixel formats utilities
 *  Date   : 2013/07/30
 *  Desc   : Supported pixel formats utilities
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <math.h>
#include <algorithm>
using std::swap;
#include <vector>
using std::vector;
#include "ASCTypes.h"
#include "ASCUtils.h"
#include "ASCFormatInfo.h"

ASCInt GetChannelDistance(CASCPixelFormat SrcFmt, CASCPixelFormat DestFmt);
ASCInt GetChannelExtraBits(CASCPixelFormat SampleFmt, CASCPixelFormat ReqFmt);
ASCInt GetChannelPosDistance(CASCPixelFormat SrcFmt, CASCPixelFormat DestFmt);
ASCBoolean AcceptFormat(CASCPixelFormat SampleFmt, CASCPixelFormat ReqFmt);

/*
 * Defines a list of supported ASC pixel formats
*/
class CASCPixelFormatList
{
public:
	CASCPixelFormatList();

	// Inserts all supported ASC formats to the list
	void InsertAll();

	// Inserts the specified format to the end of list
	ASCInt Insert(CASCPixelFormat fmt);

	// Returns index of the specified format in the list
	ASCInt IndexOf(CASCPixelFormat fmt);

	// If the specified format does not exist in the list, adds it to the end
	ASCInt Include(CASCPixelFormat fmt);

	// Removes element specified by the given index
	void Remove(ASCInt nIndex);

	// Removes all elements from the list
	void Clear();

	// Sorts formats in the list from best to worst matching the given format
	void SortBestMatchByGivenFormat(CASCPixelFormat fmt);

	// Number of elements in the list
	ASCInt GetCount();

	// Provides access to each individual element in the list
	CASCPixelFormat GetFormat(ASCInt nIndex);
private:
	vector<CASCPixelFormat>	m_Items;
	CASCPixelFormat			m_ReqSortFormat;

	void ItemSortSwap(ASCInt nIndex1, ASCInt nIndex2);
	ASCInt ItemSortCompare(CASCPixelFormat fmt1, CASCPixelFormat fmt2);
	ASCInt ItemSortSplit(ASCInt nStart, ASCInt nStop);
	void ItemSortExec(ASCInt nStart, ASCInt nStop);
};

CASCPixelFormat FindClosestFormatGeneric(CASCPixelFormat fmt, CASCPixelFormatList* pExistingFmtLst);

/*
 * Takes a list of existing pixel formats and tries to find in it a format
 * that closely resembles the provided format sample. The heuristics used by
 * this function tries not to add new channels and will never return a format
 * that has less channels than the sample; it also tries to avoid converting
 * between different format types like integer and floating-point formats
*/
CASCPixelFormat FindClosestFormat(CASCPixelFormat fmt, CASCPixelFormatList* pExistingFmtLst);