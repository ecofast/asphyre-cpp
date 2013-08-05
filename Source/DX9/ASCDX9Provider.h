/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCDX9Provider.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 provider definition
 *  Date   : 2013/07/25
 *  Desc   : General definition of ASC Direct3D9 provider
 *
 *******************************************************************************************/

#pragma once

#include "../ASCTypes.h"
#include "../ASCProvider.h"
#include "../ASCFactory.h"
#include "../ASCDevice.h"
#include "../ASCCanvas.h"
#include "../ASCTextures.h"
#include "ASCDX9Device.h"

const ASCUInt ASC_PROVIDERID_DX9 = 0x10000900;

class CASCDX9Provider : public CASCProvider
{
public:
	CASCDX9Provider() : CASCProvider()
	{
		m_uProviderID = ASC_PROVIDERID_DX9;
		ASCFactory()->Subscribe(this);
	}

	~CASCDX9Provider()
	{
		ASCFactory()->UnSubscribe(this, true);
	}

	virtual CASCDevice* CreateDevice()
	{
		return (new CASCDX9Device());
	}

	virtual CASCCanvas* CreateCanvas()
	{
		// return (new CASCDX9);
	}
};

extern inline CASCDX9Provider* ASCDX9Provider();