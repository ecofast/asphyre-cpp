/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCProvider.h
 *  Content: ASC(Asphyre Sphinx for C++) provider definition
 *  Date   : 2013/05/31
 *  Desc   : General definition of ASC provider, which should create all hardware
 *           specific classes
 *
 *******************************************************************************************/

#pragma once

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
	virtual CASCDevice*					CreateDevice()				= 0;
	/*
	 * This function creates hardware-specific ASC canvas
	*/
	virtual CASCCanvas*					CreateCanvas()				= 0;
	/*
	 * This function creates hardware-specific lockable texture
	*/
	virtual CASCLockableTexture*		CreateLockableTexture()		= 0;
	/*
	 * This function creates hardware-specific render target texture.
	 * If render targets are not supported in this provider, NULL is returned
	*/
	virtual CASCRenderTargetTexture*	CreateRenderTargetTexture()	= 0;
protected:
	/*
	 * The numerical identifier of the provider that must be unique among all
	 * providers supported in ASC
	*/
	ASCUInt	m_uProviderID;
};