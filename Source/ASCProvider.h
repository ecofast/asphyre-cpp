/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCProvider.h
 *  Content: ASC(Asphyre Sphinx for C++) provider definition / Component Factory class
 *  Date   : 2013/05/31
 *  Desc   : General definition of ASC provider, which should create all hardware
 *           specific classes
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <vector>
using std::vector;
#include "ASCTypes.h"
#include "ASCDevice.h"
#include "ASCCanvas.h"
#include "ASCTextures.h"

class CASCProvider
{
public:
	ASCUInt	GetProviderID();

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
	CASCFactory();
	~CASCFactory();

	/*
	 * Creates ASC device that is specific to the currently selected provider
	*/
	CASCDevice* CreateDevice();

	/*
	 * Creates ASC canvas that is specific to the currently selected provider
	*/
	CASCCanvas* CreateCanvas();

	/*
	 * Creates lockable ASC texture that is specific to the currently selected provider
	*/
	CASCLockableTexture* CreateLockableTexture();

	/*
	 * Creates render target ASC texture that is specific to the currently selected provider
	*/
	CASCRenderTargetTexture* CreateRenderTargetTexture();

	/*
	 * Subscribes a new provider to the list of available providers that can be
	 * used by the application. This function is usually called automatically
	 * by each of the providers
	*/
	void Subscribe(CASCProvider* pProvider);

	/*
	 * Unsubscribes the specified provider from the list of available providers
	 * that can be used by the application
	*/
	void UnSubscribe(const CASCProvider* pProvider, ASCBoolean bNoFree = false);

	/*
	 * Activates the provider with the given numerical identifier to be used by
	 * the factory's creation functions
	*/
	void UseProvider(ASCUInt uProviderID);
private:
	vector<CASCProvider*>	m_Providers;
	CASCProvider*			m_pProvider;

	ASCInt Insert(CASCProvider* pProvider);
	ASCInt IndexOf(const CASCProvider* pProvider);
	void Remove(ASCUInt nIndex, ASCBoolean bNoFree);
	void Clear();
	CASCProvider* FindProvider(ASCUInt uProviderID);
};

/*
 * Instance of CASCFactory that is ready to use in applications
 * without having to create that class explicitly. This factory object is used
 * by the entire framework
*/
extern inline CASCFactory* ASCFactory();