/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCDevice.h
 *  Content: ASC(Asphyre Sphinx for C++) Device Abstract declaration
 *  Date   : 2013/05/31
 *  Desc   : Hardware device specification that handles creation of back buffers, swap
 *			 chains and other administrative tasks. This device plays primordial part
 *           in communication between the application and hardware-specific
 *           implementation of each particular provider
 *
 *******************************************************************************************/

#pragma once

#include "ASCTypes.h"
#include "ASCIntVector2D.h"
#include "ASCSwapChains.h"
#include "ASCTextures.h"

// The current state of the device
enum CASCDeviceState {
	/*
	 * The device has not yet been initialized
	*/
	adsNotActive,
	/*
	 * The device has been initialized and is working properly
	*/
	adsActive,
	/*
	 * Initialization was attempted for this device but failed. Before attempting
	 * another initialization, ResetInitFailed must be called first
	*/
	adsInitFailed,
	/*
	 * Run-time failure occurred while working with the device. The device is no
	 * longer in stable state and should be finalized as soon as possible to
	 * prevent major issues from occurring
	*/
	adsRunTimeFault,
	/*
	 * The device is currently being initialized. This state can be detected
	 * inside device's creation events and it means that the device has not
	 * finished initialization phase yet. The application must be very cautious
	 * with any device-related calls while the device is in this state
	*/
	adsCreating
};

// Type of technology that is being used in ASC device
enum CASCDeviceTechnology {
	adtUnknown,  // The technology has not yet been established
	adtDirectX,  // Microsoft DirectX technology is being used
	adtOpenGL,   // OpenGL by Khronos Group is being used
	adtOpenGLES  // OpenGL ES by Khronos Group is being used
	// adtPropietary  // Private propietary technology is being used
};

/*
 * Hardware device wrapper that handles communication between application and
 * the video card. The device must be created from ASC factory and is one
 * of the first objects that needs to be initialized before working with any
 * other components
*/
class CASCDevice
{
public:
	CASCDevice()
	{
		m_DevTech			= adtUnknown;
		m_nTechVersion		= 0;

		m_pSwapChains		= new CASCSwapChains(this);
		m_DevState			= adsNotActive;
		m_uFillStencilValue	= 0;
		m_fFillDepthValue	= 1.0;
	}

	~CASCDevice()
	{
		Finalize();
		delete m_pSwapChains;
		m_pSwapChains = 0;
	}

	/*
	 * Initializes the device using the swap chain information provided in SwapChains and 
	 * prepares it for rendering. If the call succeeds, True is returned and False otherwise
	*/
	ASCBoolean Initialize()
	{
		ASCBoolean bResult = ((m_DevState == adsNotActive) && (m_pSwapChains->GetCount() > 0));
		if (!bResult)
		{
			return false;
		}

		bResult = InitializeDevice();
		if (!bResult)
		{
			return false;
		}

		m_DevState = adsActive;
	}

	/*
	 * Finalizes the device releasing all its resources and handles. User-created
	 * content that is not handled automatically by ASC should be released
	 * before calling this method
	*/
	void Finalize()
	{

	}

	/*
	 * The current state of the device. If the device is not in working state,
	 * any rendering calls may fail either silently or returning @False
	*/
	CASCDeviceState GetDeviceState()
	{
		return m_DevState;
	}

	// Indicates the type of technology that is currently being used
	CASCDeviceTechnology GetDeviceTechnology()
	{
		return m_DevTech;
	}

	/*
	 * The value that should be used for setting depth buffer either on the
	 * screen or the currently used rendering target
	*/
	ASCSingle GetFillDepthValue()
	{
		return m_fFillDepthValue;
	}
	void SetFillDepthValue(const ASCSingle fValue)
	{
		m_fFillDepthValue = fValue;
	}

	/*
	 * The value that should be used for setting stencil buffer either on the
	 * screen or the currently used rendering target
	*/
	ASCUInt GetFillStencilValue()
	{
		return m_uFillStencilValue;
	}
	void SetFillStencilValue(const ASCUInt uValue)
	{
		m_uFillStencilValue = uValue;
	}

	/*
	 * Begins rendering scene to the first swap chain described in SwapChains, 
	 * clears the back-buffer with the given background color and values 
	 * stored in FillDepthValue/FillStencilValue, and calls the provided event handler, 
	 * where the actual rendering should be made
	*/
	void Render(CASCNotifyEvent Handler, ASCColor uBkColor)
	{
		if ((m_DevState == adsActive) && (CanRender(0)))
		{
			RenderWith(0, Handler, uBkColor);
		} 
		else
		{
			// Timing.Sleep(5);
		}
	}

	/*
	 * Begins rendering scene to the swap chain identified by its index described
	 * in SwapChains, clears the back-buffer with the given background color and values 
	 * stored in FillDepthValue/FillStencilValue, and calls the provided event handler, 
	 * where the actual rendering should be made. The first swap chain has index of zero. 
	 * If the provided index is outside of valid range, this method does nothing
	*/
	void Render(ASCInt nSwapChainIndex, CASCNotifyEvent Handler, ASCUInt uBkColor)
	{
		if ((m_DevState == adsActive) && (CanRender(nSwapChainIndex)))
		{
			RenderWith(nSwapChainIndex, Handler, uBkColor);
		} 
		else
		{
			// Timing.Sleep(5);
		}
	}

	/*
	 * Begins rendering scene on the specified render target texture. If FillBk
	 * is set to True, the render target is cleared using the given background
	 * color and values stored in FillDepthValue/FillStencilValue. This method 
	 * calls the provided event handler, where the actual rendering should be made. 
	 * The render target texture must be property created and initialized before 
	 * calling this method. If there is a problem starting the rendering to the 
	 * given render target, this method will silently fail and the given event handler will not be called
	*/
	void RenderTo(CASCNotifyEvent Handler, ASCUInt uBkColor, ASCBoolean bFillBk, CASCRenderTargetTexture* pTexture)
	{
		if ((m_DevState != adsActive) || (!CanRender(-1)) || (!pTexture))
		{
			return;
		}

		if (!(pTexture->BeginRenderTo()))
		{
			return;
		}
		else
		{
			RenderToTarget(Handler, uBkColor, bFillBk);
			pTexture->EndRenderTo();
		}
	}

	/*
	 * Changes size of the back-buffer tied to swap chain identified by the
	 * given index. The first swap chain has index of zero. If the index is
	 * outside of valid range or the swap chain cannot be resized, the returned
	 * value is False and the size of swap chain remains unchanged. If this
	 * method succeeds, the swap chain will have its size updated and True will
	 * be returned. In some providers this may cause device to be reset and some
	 * resources to be recreated, so any resources that are not handled by ASC
	 * should be released before calling this; the best way to handle this
	 * scenario is to subscribe to EventDeviceReset and EventDeviceLost events 
	 * provided in ASCEvents.cpp
	*/
	ASCBoolean Resize(ASCInt nSwapChainIndex, const CASCIntVector2D NewSize)
	{
		PASCSwapChainDesc pUserDesc;
		if ((!pUserDesc) || (IsAtFault()))
		{
			return false;
		}

		if ((pUserDesc->nWidth == NewSize.X) && (pUserDesc->nHeight == NewSize.Y))
		{
			return true;
		}

		ClearStates();

		CASCIntVector2D		PrevSize;
		PrevSize.X			= pUserDesc->nWidth;
		PrevSize.Y			= pUserDesc->nHeight;
		pUserDesc->nWidth	= NewSize.X;
		pUserDesc->nHeight	= NewSize.Y;
		if (m_DevState != adsNotActive)
		{
			if (ResizeSwapChain(nSwapChainIndex, pUserDesc))
			{
				return true;
			} 
			else
			{
				pUserDesc->nWidth	= PrevSize.X;
				pUserDesc->nHeight	= PrevSize.Y;
				return false;
			}
		} 
		else
		{
			return true;
		}
	}

	/*
	 * Returns True if the device either failed to initialize or is in run-time
	 * fault state. If the device is working properly or has not yet been
	 * initialized, False is returned
	*/
	ASCBoolean IsAtFault()
	{
		return ((m_DevState == adsInitFailed) || (m_DevState == adsRunTimeFault));
	}

	/*
	 * Clears all textures, shaders and states currently bound to the device.
	 * This method works only on some modern providers
	*/
	void ClearStates()
	{
		if (m_DevState == adsActive)
		{
			ClearDevStates();
		}
	}

	/*
	 * Resets the failed state of the device, which is usually set when
	 * the initialization has failed. This must be done explicitly to
	 * acknowledge that the application is aware of the situation
	*/
	void ResetInitFailed()
	{
		if (m_DevState == adsInitFailed)
		{
			m_DevState = adsNotActive;
		}
	}
	
	/*
	 * The list of swap chains that will be used for rendering into. In a typical
	 * scenario at least one swap chain must be added to this list for device
	 * initialization to succeed.
	*/
	CASCSwapChains* GetSwapChains()
	{
		return m_pSwapChains;
	}
protected:
	CASCDeviceTechnology	m_DevTech;
	ASCInt					m_nTechVersion;
	CASCDeviceState			m_DevState;

	virtual	ASCBoolean InitializeDevice() = 0;
	virtual	void FinalizeDevice() = 0;
	virtual	void ResetDevice()
	{
		//
	}

	virtual	ASCBoolean CanRender(ASCInt nSwapChainIndex)
	{
		return true;
	}

	virtual void RenderWith(ASCInt nSwapChainIndex, CASCNotifyEvent Handler, ASCUInt uBkColor) = 0;

	virtual void RenderToTarget(CASCNotifyEvent Handler, ASCUInt uBkColor, ASCBoolean bFillBk) = 0;

	virtual ASCBoolean ResizeSwapChain(ASCInt nSwapChainIndex, PASCSwapChainDesc pNewUserDesc)
	{
		return true;
	}

	virtual void ClearDevStates()
	{

	}
private:
	CASCSwapChains*	m_pSwapChains;
	ASCSingle		m_fFillDepthValue;
	ASCUInt			m_uFillStencilValue;
};