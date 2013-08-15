/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCDX9Provider.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 provider definition
 *  Date   : 2013/07/25
 *  Desc   : General definition of ASC Direct3D9 provider
 *
 *******************************************************************************************/

#pragma once

#include "../ASCConfig.h"

#include "../ASCProvider.h"
#include "ASCDX9Device.h"
#include "ASCDX9Canvas.h"
#include "ASCDX9Textures.h"

const ASCUInt ASC_PROVIDERID_DX9 = 0x10000900;

class CASCDX9Provider : public CASCProvider
{
public:
	CASCDX9Provider();
	~CASCDX9Provider();

	virtual CASCDevice* CreateDevice();
	virtual CASCCanvas* CreateCanvas();
	virtual CASCLockableTexture* CreateLockableTexture();
	virtual CASCRenderTargetTexture* CreateRenderTargetTexture();
};

extern inline CASCDX9Provider* ASCDX9Provider();