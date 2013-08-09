#include "ASCPixelFormats.h"

ASCInt GetChannelDistance(CASCPixelFormat SrcFmt, CASCPixelFormat DestFmt)
{
	CASCPixelFormatInfo	SrcInfo		= GetPixelFormatInfo(SrcFmt);
	CASCPixelFormatInfo	DestInfo	= GetPixelFormatInfo(DestFmt);
	ASCInt				nResult		= 0;
	CASCChannelType		ChannelType;
	ASCInt				nDestNo;
	for (ASCInt nSrcNo = 0; nSrcNo <= SrcInfo.nNumChannels - 1; nSrcNo++)
	{
		ChannelType = SrcInfo.Channels[nSrcNo].ChannelType;
		if (!IsUsefulChannel(ChannelType))
		{
			continue;
		}
		nDestNo = FindChannelPos(ChannelType, &DestInfo);
		if (nDestNo == -1)
		{
			continue;
		}
		nResult += ASCRound((pow(SrcInfo.Channels[nSrcNo].nBits - DestInfo.Channels[nDestNo].nBits, 2)));
	}
	return nResult;
}

ASCInt GetChannelExtraBits(CASCPixelFormat SampleFmt, CASCPixelFormat ReqFmt)
{
	CASCPixelFormatInfo	SampleInfo	= GetPixelFormatInfo(SampleFmt);
	CASCPixelFormatInfo	ReqInfo		= GetPixelFormatInfo(ReqFmt);
	CASCChannelType		ChannelType;
	ASCInt				nResult		= 0;
	for (ASCInt i = 0; i < SampleInfo.nNumChannels; i++)
	{
		ChannelType = SampleInfo.Channels[i].ChannelType;
		if ((!IsUsefulChannel(ChannelType)) || (FindChannelPos(ChannelType, &ReqInfo) == -1))
		{
			nResult += (SampleInfo.Channels[i].nBits);
		}
	}
	return nResult;
}

ASCInt GetChannelPosDistance(CASCPixelFormat SrcFmt, CASCPixelFormat DestFmt)
{
	CASCPixelFormatInfo	SrcInfo		= GetPixelFormatInfo(SrcFmt);
	CASCPixelFormatInfo	DestInfo	= GetPixelFormatInfo(DestFmt);
	ASCSingle			fSum		= 0.0;
	CASCChannelType		ChannelType;
	ASCInt				nDestNo, nSrcPos, nDestPos;
	for (ASCInt nSrcNo = 0; nSrcNo <= SrcInfo.nNumChannels - 1; nSrcNo++)
	{
		ChannelType = SrcInfo.Channels[nSrcNo].ChannelType;
		if (!IsUsefulChannel(ChannelType))
		{
			continue;
		}
		nDestNo = FindChannelPos(ChannelType, &DestInfo);
		if (nDestNo == -1)
		{
			continue;
		}
		nSrcPos = (SrcInfo.nNumChannels - 1) - nSrcNo;
		nDestPos = (DestInfo.nNumChannels - 1) - nDestNo;
		fSum += (pow(nSrcPos - nDestPos, 2));
	}
	return ASCRound(sqrt(fSum));
}

ASCBoolean AcceptFormat(CASCPixelFormat SampleFmt, CASCPixelFormat ReqFmt)
{
	CASCPixelFormatInfo	SampleInfo	= GetPixelFormatInfo(SampleFmt);
	CASCPixelFormatInfo	ReqInfo		= GetPixelFormatInfo(ReqFmt);
	ASCBoolean			bMatched	= false;
	CASCChannelType		ChannelType;
	ASCInt				nDestNo;
	for (ASCInt i = 0; i < ReqInfo.nNumChannels; i++)
	{
		ChannelType = ReqInfo.Channels[i].ChannelType;
		if (!IsUsefulChannel(ChannelType))
		{
			continue;
		}

		/*
		 * Sample format needs to contain at least those channels present in
		 * the requested format. In addition, they must have the same storage type
		*/
		nDestNo = FindChannelPos(ChannelType, &SampleInfo);
		if ((nDestNo == -1) || (SampleInfo.Channels[nDestNo].ChannelStorage != ReqInfo.Channels[i].ChannelStorage))
		{
			return false;
		}

		bMatched = true;
	}
	return (bMatched || ((SampleFmt == ReqFmt) && (SampleFmt != apfUnknown)));
}

CASCPixelFormat FindClosestFormatGeneric(CASCPixelFormat fmt, CASCPixelFormatList* pExistingFmtLst)
{
	CASCPixelFormatList FmtLst;
	for (ASCInt i = 0; i < pExistingFmtLst->GetCount(); i++)
	{
		if (AcceptFormat(pExistingFmtLst->GetFormat(i), fmt))
		{
			FmtLst.Insert(pExistingFmtLst->GetFormat(i));
		}
	}
	FmtLst.SortBestMatchByGivenFormat(fmt);
	if (FmtLst.GetCount() > 0)
	{
		return FmtLst.GetFormat(0);
	}
	else
	{
		return apfUnknown;
	}
}

CASCPixelFormat FindClosestFormat(CASCPixelFormat fmt, CASCPixelFormatList* pExistingFmtLst)
{
	return FindClosestFormatGeneric(fmt, pExistingFmtLst);
}

CASCPixelFormatList::CASCPixelFormatList()
{

}

void CASCPixelFormatList::InsertAll()
{
	Clear();
	
	ASCInt i = (ASCInt)apfUnknown;
	while (i <= (ASCInt)apfX4R4G4B4)
	{
		Insert((CASCPixelFormat)i);
		i++;
	}
}

ASCInt CASCPixelFormatList::Insert(CASCPixelFormat fmt)
{
	ASCInt nResult = m_Items.size();
	m_Items.resize(nResult + 1);
	m_Items[nResult] = fmt;
	return nResult;
}

ASCInt CASCPixelFormatList::IndexOf(CASCPixelFormat fmt)
{
	for (ASCUInt i = 0; i < m_Items.size(); i++)
	{
		if (m_Items[i] == fmt)
		{
			return i;
		}
	}
	return -1;
}

ASCInt CASCPixelFormatList::Include(CASCPixelFormat fmt)
{
	ASCInt nResult = IndexOf(fmt);
	if (nResult == -1)
	{
		nResult = Insert(fmt);
	}

	return nResult;
}

void CASCPixelFormatList::Remove(ASCInt nIndex)
{
	if ((nIndex < 0) || (nIndex >= m_Items.size()))
	{
		return;
	}

	m_Items.erase(m_Items.begin() + nIndex);
}

void CASCPixelFormatList::Clear()
{
	m_Items.clear();
}

void CASCPixelFormatList::SortBestMatchByGivenFormat(CASCPixelFormat fmt)
{
	m_ReqSortFormat = fmt;
	if (m_Items.size() > 0)
	{
		ItemSortExec(0, m_Items.size() - 1);
	}
}

ASCInt CASCPixelFormatList::GetCount()
{
	return (m_Items.size());
}

CASCPixelFormat CASCPixelFormatList::GetFormat(ASCInt nIndex)
{
	if ((nIndex >= 0) && (nIndex < m_Items.size()))
	{
		return (m_Items[nIndex]);
	} 
	else
	{
		return apfUnknown;
	}
}

void CASCPixelFormatList::ItemSortSwap(ASCInt nIndex1, ASCInt nIndex2)
{
	swap(m_Items[nIndex1], m_Items[nIndex2]);
}

ASCInt CASCPixelFormatList::ItemSortCompare(CASCPixelFormat fmt1, CASCPixelFormat fmt2)
{
	ASCInt	nDelta1 = GetChannelDistance(fmt1, m_ReqSortFormat);
	ASCInt	nDelta2 = GetChannelDistance(fmt2, m_ReqSortFormat);
	if (nDelta1 == nDelta2)
	{
		nDelta1 = GetChannelExtraBits(fmt1, m_ReqSortFormat);
		nDelta2 = GetChannelExtraBits(fmt2, m_ReqSortFormat);
	}
	if (nDelta1 == nDelta2)
	{
		nDelta1 = GetChannelPosDistance(fmt1, m_ReqSortFormat);
		nDelta2 = GetChannelPosDistance(fmt2, m_ReqSortFormat);
	}
	if (nDelta1 == nDelta2)
	{
		nDelta1 = GetChannelCount(&GetPixelFormatInfo(fmt1));
		nDelta2 = GetChannelCount(&GetPixelFormatInfo(fmt2));
	}
	if (nDelta1 == nDelta2)
	{
		nDelta1 = abs(C_ASCPixelFormatBits[fmt1] - C_ASCPixelFormatBits[m_ReqSortFormat]);
		nDelta2 = abs(C_ASCPixelFormatBits[fmt2] - C_ASCPixelFormatBits[m_ReqSortFormat]);
	}
	if (nDelta1 > nDelta2)
	{
		return 1;
	}
	else if (nDelta1 == nDelta2)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

ASCInt CASCPixelFormatList::ItemSortSplit(ASCInt nStart, ASCInt nStop)
{
	ASCInt			nLeft	= nStart + 1;
	ASCInt			nRight	= nStop;
	CASCPixelFormat	Pivot	= m_Items[nStart];
	while (nLeft < nRight)
	{
		while ((nLeft < nStop) && (ItemSortCompare(m_Items[nLeft], Pivot) < 0))
		{
			nLeft++;
		}
		while ((nRight > nStart) && (ItemSortCompare(m_Items[nRight], Pivot) >= 0))
		{
			nRight--;
		}
		if (nLeft < nRight)
		{
			ItemSortSwap(nLeft, nRight);
		}
	}
	ItemSortSwap(nStart, nRight);
	return nRight;
}

void CASCPixelFormatList::ItemSortExec(ASCInt nStart, ASCInt nStop)
{
	ASCInt nSplitPos;
	if (nStart < nStop)
	{
		nSplitPos = ItemSortSplit(nStart, nStop);
		ItemSortExec(nStart, nSplitPos - 1);
		ItemSortExec(nSplitPos + 1, nStop);
	}
}
