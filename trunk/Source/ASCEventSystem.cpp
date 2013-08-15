#include <algorithm>
using std::swap;
#include "ASCEventSystem.h"

static ASCInt G_nGlobalEventID = 0;

/*
 * List of all existing ASC event providers implementing observer pattern.
 * This class is an instance of CASCEventProviders which is ready to use
 * in the application
*/
CASCEventProviders* ASCEventProviders()
{
	static CASCEventProviders gASCEventProviders;
	return &gASCEventProviders;
}

CASCEventProvider::CASCEventProvider(CASCEventProviders* pOwner)
{
	m_pOwner = pOwner;
	if (m_pOwner)
	{
		m_pOwner->Include(this);
	}
	m_bEventListDirty = false;

	m_EventValidator = 0;
}

CASCEventProvider::~CASCEventProvider()
{
	RemoveAll();
	if (m_pOwner)
	{
		m_pOwner->Exclude(this);
	}
}

ASCInt CASCEventProvider::Subscribe(const wchar_t* sClassName, CASCEventCallback CallBack, const ASCInt nPriority /* = -1 */)
{
	ASCInt nIndex = m_Datas.size();
	m_Datas.resize(nIndex + 1);
	m_Datas[nIndex].m_nEventID = NextEventID();
	m_Datas[nIndex].m_Callback = CallBack;
	m_Datas[nIndex].m_sClassName = sClassName;
	m_Datas[nIndex].m_nPriority1 = nPriority;
	m_Datas[nIndex].m_nPriority2 = -1;

	MarkEventListDirty();
	return (m_Datas[nIndex].m_nEventID);
}

void CASCEventProvider::UnSubscribe(const ASCInt nEventID)
{
	ASCInt nIndex = IndexOfEventID(nEventID);
	if (nIndex != -1)
	{
		Remove(nIndex);
	}
}

void CASCEventProvider::UnSubscribeClass(const wchar_t* sClassName)
{
	for (ASCUInt i = 0; i < m_Datas.size(); i++)
	{
		if (m_Datas[i].m_sClassName == sClassName)
		{
			Remove(i);
			return;
		}
	}
}

ASCBoolean CASCEventProvider::SetClassPriority(const wstring& sClassName, const ASCInt nPriority)
{
	for (ASCUInt i = 0; i < m_Datas.size(); i++)
	{
		if (m_Datas[i].m_sClassName == sClassName)
		{
			m_Datas[i].m_nPriority2	= nPriority;
			m_bEventListDirty		= true;
			return true;
		}
	}
	return false;
}

void CASCEventProvider::MarkEventListDirty()
{
	m_bEventListDirty = true;
	if (m_pOwner)
	{
		m_pOwner->MarkEventListsDirty();
	}
}

ASCBoolean CASCEventProvider::Notify(void* pSender, ASCPointer pParam /* = 0 */)
{
	if (m_pOwner)
	{
		m_pOwner->CheckEventLists();
	}
	if (m_bEventListDirty)
	{
		UpdateEventList();
	}

	ASCBoolean bResult	= false;
	ASCBoolean bAllowed;
	for (ASCUInt i = 0; i < m_Datas.size(); i++)
	{
		if (m_EventValidator)
		{
			bAllowed = true;
			m_EventValidator(this, pSender, pParam, m_Datas[i].m_sClassName, &bAllowed);
			if (!bAllowed)
			{
				continue;
			}
		}

		m_Datas[i].m_Callback(pSender, pParam, &bResult);
		if (bResult)
		{
			return true;
		}
	}
	return false;
}

CASCEventProviders* CASCEventProvider::GetOwner()
{
	return m_pOwner;
}

CASCEventValidator CASCEventProvider::GetEventValidator()
{
	return m_EventValidator;
}

void CASCEventProvider::SetEventValidator(const CASCEventValidator Value)
{
	m_EventValidator = Value;
}

ASCInt CASCEventProvider::NextEventID()
{
	return(G_nGlobalEventID++);
}

ASCInt CASCEventProvider::IndexOfEventID(const ASCInt nID)
{
	for (ASCUInt i = 0; i < m_Datas.size(); i++)
	{
		if (m_Datas[i].m_nEventID == nID)
		{
			return i;
		}
	}
	return -1;
}

void CASCEventProvider::RemoveAll()
{
	m_Datas.clear();
	MarkEventListDirty();
}

void CASCEventProvider::Remove(ASCInt nIndex)
{
	if ((nIndex > 0) && ((size_t)nIndex < m_Datas.size()))
	{
		m_Datas.erase(m_Datas.begin() + nIndex);
		MarkEventListDirty();
	}
}

void CASCEventProvider::EventListSwap(ASCInt nIndex1, ASCInt nIndex2)
{
	swap(m_Datas[nIndex1], m_Datas[nIndex2]);
}

ASCInt CASCEventProvider::EventListCompare(ASCInt nIndex1, ASCInt nIndex2)
{
	ASCInt nResult = 0;

	if (m_Datas[nIndex1].m_nPriority1 > m_Datas[nIndex2].m_nPriority1)
	{
		nResult = 1;
	}
	if (m_Datas[nIndex1].m_nPriority1 < m_Datas[nIndex2].m_nPriority1)
	{
		nResult = -1;
	}

	if (nResult == 0)
	{
		if (m_Datas[nIndex1].m_nPriority2 > m_Datas[nIndex2].m_nPriority2)
		{
			nResult = 1;
		}
		if (m_Datas[nIndex1].m_nPriority2 < m_Datas[nIndex2].m_nPriority2)
		{
			nResult = -1;
		}
	}

	if (nResult == 0)
	{
		nResult = (m_Datas[nIndex1].m_sClassName.compare(m_Datas[nIndex2].m_sClassName));
	}

	if (nResult == 0)
	{
		if (m_Datas[nIndex1].m_nEventID > m_Datas[nIndex2].m_nEventID)
		{
			nResult = 1;
		}
		if (m_Datas[nIndex1].m_nEventID < m_Datas[nIndex2].m_nEventID)
		{
			nResult = -1;
		}
	}

	return nResult;
}

ASCInt CASCEventProvider::EventListSplit(ASCInt nStart, ASCInt nStop)
{
	ASCInt nLeft, nRight, nPivot;
	nLeft	= nStart + 1;
	nRight	= nStop;
	nPivot	= nStart;
	while (nLeft < nRight)
	{
		while ((nLeft < nStop) && (EventListCompare(nLeft, nPivot) < 0))
		{
			nLeft++;
		}
		while ((nRight > nStart) && (EventListCompare(nRight, nPivot) >= 0))
		{
			nRight--;
		}
		if (nLeft < nRight)
		{
			EventListSwap(nLeft, nRight);
		}
	}

	EventListSwap(nStart, nRight);
	return nRight;
}

void CASCEventProvider::EventListSort(ASCInt nStart, ASCInt nStop)
{
	if (nStart < nStop)
	{
		ASCInt nSplitPos = EventListSplit(nStart, nStop);
		EventListSort(nStart, nSplitPos - 1);
		EventListSort(nSplitPos + 1, nStop);
	}
}

void CASCEventProvider::UpdateEventList()
{
	if (m_Datas.size() > 1)
	{
		EventListSort(0, m_Datas.size() - 1);
	}

	m_bEventListDirty = false;
}

CASCEventProviders::CASCEventProviders()
{
	m_bEventListsDirty = false;
}

CASCEventProviders::~CASCEventProviders()
{
	Clear();
}

CASCEventProvider* CASCEventProviders::Add()
{
	return (GetItem(Insert()));
}

ASCInt CASCEventProviders::Insert()
{
	if (m_bListSemaphore)
	{
		return -1;
	}

	m_bListSemaphore = true;

	ASCInt nResult = m_Datas.size();
	m_Datas.resize(nResult + 1);
	m_Datas[nResult] = new CASCEventProvider(this);
	m_bListSemaphore = false;
	m_bEventListsDirty	= true;

	return nResult;
}

void CASCEventProviders::Remove(ASCInt nIndex)
{
	if ((nIndex < 0) || ((size_t)nIndex >= m_Datas.size()) || m_bListSemaphore)
	{
		return;
	}

	m_bListSemaphore = true;
	if (m_Datas[nIndex])
	{
		delete m_Datas[nIndex];
	}
	m_Datas.erase(m_Datas.begin() + nIndex);

	m_bListSemaphore = false;
	m_bEventListsDirty = true;
}

void CASCEventProviders::Clear()
{
	if (m_bListSemaphore)
	{
		return;
	}
	m_bListSemaphore = true;

	for (ASCInt i = m_Datas.size() - 1; i >= 0; i--)
	{
		if (m_Datas[i])
		{
			delete m_Datas[i];
		}
	}
	m_Datas.clear();

	m_bListSemaphore = false;
	m_bEventListsDirty = true;
}

ASCInt CASCEventProviders::IndexOf(const CASCEventProvider* Provider)
{
	for (ASCUInt i = 0; i < m_Datas.size(); i++)
	{
		if (m_Datas[i] == Provider)
		{
			return i;
		}
	}
	return -1;
}

void CASCEventProviders::MarkEventListsDirty()
{
	m_bEventListsDirty = true;
}

void CASCEventProviders::UnSubscribe(const wchar_t* sClassName)
{
	for (ASCInt i = m_Datas.size() - 1; i >= 0; i--)
	{
		m_Datas[i]->UnSubscribeClass(sClassName);
	}
}

ASCInt CASCEventProviders::GetItemCount()
{
	return (m_Datas.size());
}

CASCEventProvider* CASCEventProviders::GetItem(ASCInt nIndex)
{
	if ((nIndex >= 0) && ((size_t)nIndex < m_Datas.size()))
	{
		return (m_Datas[nIndex]);
	} 
	else
	{
		return 0;
	}
}

ASCInt CASCEventProviders::Include(CASCEventProvider* pProvider)
{
	if (m_bListSemaphore)
	{
		return -1;
	}

	ASCInt nResult = IndexOf(pProvider);
	if (nResult == -1)
	{
		nResult = m_Datas.size();
		m_Datas.resize(nResult + 1);
		m_Datas[nResult] = pProvider;

		m_bEventListsDirty = true;
	}
	return nResult;
}

void CASCEventProviders::Exclude(CASCEventProvider* pProvider)
{
	if (m_bListSemaphore)
	{
		return;
	}

	ASCInt nIndex = IndexOf(pProvider);
	if (nIndex == -1)
	{
		return;
	}

	m_Datas.erase(m_Datas.begin() + nIndex);
	m_bEventListsDirty = true;
}

void CASCEventProviders::CheckEventLists()
{

}