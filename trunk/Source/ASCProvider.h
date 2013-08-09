/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCProvider.h
 *  Content: ASC(Asphyre Sphinx for C++) provider definition / Component Factory class
 *  Date   : 2013/05/31
 *  Desc   : General definition of ASC provider, which should create all hardware
 *           specific classes
 *
 *******************************************************************************************/

#pragma once

#include <vector>
using std::vector;
#include "ASCTypes.h"
#include "ASCDevice.h"
#include "ASCCanvas.h"
#include "ASCTextures.h"

class CASCProvider
{
public:
	ASCUInt	GetProviderID()
	{
		return m_uProviderID;
	}

	/*
	 * This function creates hardware-specific ASC device
	*/
	virtual CASCDevice* CreateDevice() = 0;
	/*
	 * This function creates hardware-specific ASC canvas
	*/
	virtual CASCCanvas* CreateCanvas() = 0;
	/*
	 * This function creates hardware-specific lockable texture
	*/
	virtual CASCLockableTexture* CreateLockableTexture() = 0;
	/*
	 * This function creates hardware-specific render target texture.
	 * If render targets are not supported in this provider, NULL is returned
	*/
	virtual CASCRenderTargetTexture* CreateRenderTargetTexture() = 0;
protected:
	/*
	 * The numerical identifier of the provider that must be unique among all
	 * providers supported in ASC
	*/
	ASCUInt	m_uProviderID;
};

/*
 * General factory implementation that creates all provider-dependent 
 * classes such as device, canvas and textures
*/
class CASCFactory
{
public:
	CASCFactory()
	{
		m_pProvider = 0;
	}

	~CASCFactory()
	{
		Clear();
	}

	/*
	 * Creates ASC device that is specific to the currently selected provider
	*/
	CASCDevice* CreateDevice()
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

	/*
	 * Creates ASC canvas that is specific to the currently selected provider
	*/
	CASCCanvas* CreateCanvas()
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

	/*
	 * Creates lockable ASC texture that is specific to the currently selected provider
	*/
	CASCLockableTexture* CreateLockableTexture()
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

	/*
	 * Creates render target ASC texture that is specific to the currently selected provider
	*/
	CASCRenderTargetTexture* CreateRenderTargetTexture()
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

	/*
	 * Subscribes a new provider to the list of available providers that can be
	 * used by the application. This function is usually called automatically
	 * by each of the providers
	*/
	void Subscribe(CASCProvider* pProvider)
	{
		ASCInt nIndex = IndexOf(pProvider);
		if (nIndex == -1)
		{
			Insert(pProvider);
		}
	}

	/*
	 * Unsubscribes the specified provider from the list of available providers
	 * that can be used by the application
	*/
	void UnSubscribe(const CASCProvider* pProvider, ASCBoolean bNoFree = false)
	{
		Remove(IndexOf(pProvider), bNoFree);
	}

	/*
	 * Activates the provider with the given numerical identifier to be used by
	 * the factory's creation functions
	*/
	void UseProvider(ASCUInt uProviderID)
	{
		m_pProvider = FindProvider(uProviderID);
	}
private:
	vector<CASCProvider*>	m_Providers;
	CASCProvider*			m_pProvider;

	ASCInt Insert(CASCProvider* pProvider)
	{
		ASCInt nIndex = m_Providers.size();
		m_Providers.resize(nIndex + 1);
		m_Providers[nIndex] = pProvider;
		return nIndex;
	}

	ASCInt IndexOf(const CASCProvider* pProvider)
	{
		for (ASCUInt i = 0; i < m_Providers.size(); i++)
		{
			if (m_Providers[i] == pProvider)
			{
				return i;
			}
		}
		return -1;
	}

	void Remove(ASCUInt nIndex, ASCBoolean bNoFree)
	{
		if ((nIndex >= 0) && (nIndex < m_Providers.size()))
		{
			if ((m_Providers[nIndex]) && (!bNoFree))
			{
				delete m_Providers[nIndex];
				m_Providers.erase(m_Providers.begin() + nIndex);
			}
		}
	}

	void Clear()
	{
		for (ASCUInt i = 0; i < m_Providers.size(); i++)
		{
			if (m_Providers[i])
			{
				delete m_Providers[i];
			}
		}
		m_Providers.clear();
	}

	CASCProvider* FindProvider(ASCUInt uProviderID)
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
};

/*
 * Instance of CASCFactory that is ready to use in applications
 * without having to create that class explicitly. This factory object is used
 * by the entire framework
*/
extern inline CASCFactory* ASCFactory();