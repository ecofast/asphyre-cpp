/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCDX9Device.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 device management
 *  Date   : 2013/07/25
 *  Desc   : General definition of ASC Direct3D9 device management
 *
 *******************************************************************************************/

#pragma once

#include <d3d9.h>

#include "../ASCTypes.h"
#include "../ASCDevice.h"
#include "../ASCTextures.h"
#include "../ASCSwapChains.h"

class CASCDX9Device : public CASCDevice
{
public:
	CASCDX9Device()
	{

	}

	~CASCDX9Device()
	{

	}

	virtual	ASCBoolean		InitializeDevice()
	{

	}

	virtual	ASCBoolean		FinalizeDevice()
	{

	}

	virtual	ASCBoolean		ResetDevice()
	{
		
	}

	virtual	ASCBoolean		CanRender(ASCInt nSwapChainIndex)
	{
		
	}

	virtual void			RenderWith(ASCInt nSwapChainIndex, CASCNotifyEvent Handler, ASCUInt uBkColor)
	{

	}

	virtual void			RenderToTarget(CASCNotifyEvent Handler, ASCUInt uBkColor, ASCBoolean bFillBk)
	{

	}

	virtual ASCBoolean		ResizeSwapChain(ASCInt nSwapChainIndex, PSwapChainDesc pNewUserDesc)
	{
		return true;
	}

protected:
private:
};