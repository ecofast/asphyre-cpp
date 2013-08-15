/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCDX9Device.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 device management
 *  Date   : 2013/07/25
 *  Desc   : General definition of ASC Direct3D9 device management
 *
 *******************************************************************************************/

#pragma once

#include "../ASCConfig.h"

#include "../ASCDevice.h"
#include "ASCDX9SwapChains.h"

// Preserve FPU state or not
#define PRESERVEFPU

// Enable multi-threading mode or not
// #define ENABLEMULTITHREAD

class CASCDX9Device : public CASCDevice
{
public:
	CASCDX9Device();
	~CASCDX9Device();

	CASCDX9SwapChains* GetDX9SwapChains();
protected:
	virtual ASCBoolean InitializeDevice();
	virtual void FinalizeDevice();
	virtual void ResetDevice();
	virtual ASCBoolean CanRender(ASCInt nSwapChainIndex);
	virtual void RenderWith(ASCInt nSwapChainIndex, CASCNotifyEvent Handler, ASCUInt uBkColor);
	virtual void RenderToTarget(CASCNotifyEvent Handler, ASCUInt uBkColor, ASCBoolean bFillBk);
	virtual ASCBoolean ResizeSwapChain(ASCInt nSwapChainIndex, PASCSwapChainDesc pNewUserDesc);
private:
	CASCDX9SwapChains*	m_pSwapChains;
	ASCBoolean			m_bManagedD3D;
	ASCBoolean			m_bManagedDevice;
	ASCBoolean			m_bUsingDepthBuf;
	ASCBoolean			m_bUsingStencil;
	ASCBoolean			m_bIsInLostState;

	ASCBoolean CreateDirect3D();
	void DestroyDirect3D();
	ASCBoolean GetDisplayMode();
	ASCBoolean MakePresentParams();
	ASCBoolean CreateDevice();
	void DestroyDevice();
	void MoveIntoLostState();
	ASCBoolean AttemptRecoverState();
	ASCBoolean CheckDeviceCondition(ASCInt nSwapChainIndex);
	void SetDefaultViewport(const PASCSwapChainDesc pDesc);
	void Clear(ASCColor uColor);
};