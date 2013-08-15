#include "ASCProvider.h"

CASCFactory* ASCFactory()
{
	static CASCFactory gASCFactory;
	return &gASCFactory;
}

ASCUInt CASCProvider::GetProviderID()
{
	return m_uProviderID;
}

CASCLockableTexture* CASCFactory::CreateLockableTexture()
{
	if (m_pProvider)
	{
		return (m_pProvider->CreateLockableTexture());
	} 
	else
	{
		return 0;
	}
}

CASCFactory::CASCFactory()
{
	m_pProvider = 0;
}

CASCFactory::~CASCFactory()
{
	Clear();
}

CASCDevice* CASCFactory::CreateDevice()
{
	if (m_pProvider)
	{
		return (m_pProvider->CreateDevice());
	}
	else
	{
		return 0;
	}
}

CASCCanvas* CASCFactory::CreateCanvas()
{
	if (m_pProvider)
	{
		return (m_pProvider->CreateCanvas());
	} 
	else
	{
		return 0;
	}
}

CASCRenderTargetTexture* CASCFactory::CreateRenderTargetTexture()
{
	if (m_pProvider)
	{
		return (m_pProvider->CreateRenderTargetTexture());
	} 
	else
	{
		return 0;
	}
}

void CASCFactory::Subscribe(CASCProvider* pProvider)
{
	ASCInt nIndex = IndexOf(pProvider);
	if (nIndex == -1)
	{
		Insert(pProvider);
	}
}

void CASCFactory::UnSubscribe(const CASCProvider* pProvider, ASCBoolean bNoFree /*= false*/)
{
	Remove(IndexOf(pProvider), bNoFree);
}

void CASCFactory::UseProvider(ASCUInt uProviderID)
{
	m_pProvider = FindProvider(uProviderID);
}

ASCInt CASCFactory::Insert(CASCProvider* pProvider)
{
	ASCInt nIndex = m_Providers.size();
	m_Providers.resize(nIndex + 1);
	m_Providers[nIndex] = pProvider;
	return nIndex;
}

ASCInt CASCFactory::IndexOf(const CASCProvider* pProvider)
{
	for (ASCUInt i = 0; i < m_Providers.size(); i++)
	{
		if (m_Providers[i] == pProvider)
		{
			return (ASCInt)i;
		}
	}
	return -1;
}

void CASCFactory::Remove(ASCUInt nIndex, ASCBoolean bNoFree)
{
	if ((nIndex >= 0) && (nIndex < m_Providers.size()))
	{
		if (m_Providers[nIndex])
		{
			if (!bNoFree)
			{
				delete m_Providers[nIndex];
				m_Providers[nIndex] = 0;
			}
			m_Providers.erase(m_Providers.begin() + nIndex);
		}
	}
}

void CASCFactory::Clear()
{
	for (ASCUInt i = 0; i < m_Providers.size(); i++)
	{
		if (m_Providers[i])
		{
			delete m_Providers[i];
			m_Providers[i] = 0;
		}
	}
	m_Providers.clear();
}

CASCProvider* CASCFactory::FindProvider(ASCUInt uProviderID)
{
	ASCInt nIndex = -1;
	for (ASCUInt i = 0; i < m_Providers.size(); i++)
	{
		if (m_Providers[i]->GetProviderID() == uProviderID)
		{
			nIndex = i;
			break;
		}
	}

	if ((nIndex == -1) && (m_Providers.size() > 0))
	{
		nIndex = 0;
	}

	if (nIndex != -1)
	{
		return m_Providers[nIndex];
	} 
	else
	{
		return 0;
	}
}
