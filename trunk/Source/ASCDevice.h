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

#include "ASCConfig.h"

#include <string>
using std::wstring;
#include "ASCTypes.h"
#include "ASCIntVector2D.h"
#include "ASCSwapChains.h"
#include "ASCTextures.h"
#include "ASCEvents.h"
#include "ASCTiming.h"

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
	/*
	 * The technology has not yet been established
	*/
	adtUnknown,
	/*
	 * Microsoft DirectX technology is being used
	*/
	adtDirectX,
	/*
	 * OpenGL by Khronos Group is being used
	*/
	adtOpenGL,
	/*
	 * OpenGL ES by Khronos Group is being used
	*/
	adtOpenGLES,
	/*
	 * Private propietary technology is being used
	*/
	adtPropietary
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
	CASCDevice();
	~CASCDevice();

	/*
	 * Initializes the device using the swap chain information provided in SwapChains and 
	 * prepares it for rendering. If the call succeeds, True is returned and False otherwise
	*/
	ASCBoolean Initialize();

	/*
	 * Finalizes the device releasing all its resources and handles.
	 * User-created content that is not handled automatically by ASC
	 * should be released before calling this method
	*/
	void Finalize();

	CASCSwapChains* GetSwapChains();
	CASCDeviceState GetDeviceState();

	CASCDeviceTechnology GetDeviceTechnology();
	ASCInt GetTechVersion();
	ASCInt GetTechFeatureVersion();

	ASCSingle GetDeviceScale();
	void SetDeviceScale(ASCSingle fValue);

	ASCSingle GetFillDepthValue();
	void SetFillDepthValue(const ASCSingle fValue);

	ASCUInt32 GetFillStencilValue();
	void SetFillStencilValue(const ASCUInt32 uValue);

	/*
	 * Begins rendering scene to the first swap chain described in m_pSwapChains, 
	 * clears the back-buffer with the given background color and values stored in
	 * m_fFillDepthValue/m_uFillStencilValue, and calls the provided event handler, 
	 * where the actual rendering should be made
	*/
	void Render(CASCNotifyEvent Handler, ASCColor uBkColor);

	/*
	 * Begins rendering scene to the swap chain identified by its index described
	 * in SwapChains, clears the back-buffer with the given background color and values 
	 * stored in m_fFillDepthValue/m_uFillStencilValue, and calls the provided event handler, 
	 * where the actual rendering should be made. The first swap chain has index of zero. 
	 * If the provided index is outside of valid range, this method does nothing
	*/
	void Render(ASCInt nSwapChainIndex, CASCNotifyEvent Handler, ASCColor uBkColor);

	/*
	 * Begins rendering scene on the specified render target texture. If bFillBk
	 * is set to True, the render target is cleared using the given background
	 * color and values stored in m_fFillDepthValue/m_uFillStencilValue. This method 
	 * calls the provided event handler, where the actual rendering should be made. 
	 * The render target texture must be property created and initialized before 
	 * calling this method. If there is a problem starting the rendering to the 
	 * given render target, this method will silently fail and the given event handler 
	 * will not be called
	*/
	void RenderTo(CASCNotifyEvent Handler, ASCColor uBkColor, ASCBoolean bFillBk, CASCRenderTargetTexture* pTexture);

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
	ASCBoolean Resize(ASCInt nSwapChainIndex, const CASCIntVector2D NewSize);

	/*
	 * Returns True if the device either failed to initialize or is in run-time
	 * fault state. If the device is working properly or has not yet been
	 * initialized, False is returned
	*/
	ASCBoolean IsAtFault();
	
	/*
	 * Clears all textures, shaders and states currently bound to the device.
	 * This method works only on some modern providers
	*/
	void ClearStates();

	/*
	 * Resets the failed state of the device, which is usually set when
	 * the initialization has failed. This must be done explicitly to
	 * acknowledge that the application is aware of the situation
	*/
	void ResetInitFailed();
protected:
	/*
	 * Indicates the type of technology that is currently being used
	*/
	CASCDeviceTechnology	m_DevTech;

	/*
	 * Indicates the version of current technology that is currently being used.
	 * The values are specified in hexadecimal format. That is, a value of 0x100
	 * indicates version 1.0, while a value of 0x247 would indicate version
	 * 2.4.7. This value is used in combination with m_DevTech, so if m_DevTech
	 * is set to adtDirectX and this value is set to 0xA10, it means that
	 * DirectX 10.1 is being used
	*/
	ASCInt					m_nTechVersion;

	/*
	 * Indicates the feature level version of current technology that is
	 * currently being used. The difference between this parameter and
	 * m_nTechVersion is that the second parameter indicates type of
	 * technology being used(for example, DirectX 10), while this one
	 * indicates the level of features available(for example, DirectX 9.0c).
	 * The values here are specified in hexadecimal format. That is, a value of
	 * 0x213 would indicate version 2.1.3
	*/
	ASCInt					m_nTechFeatureVersion;

	/*
	 * The current state of the device. If the device is not in working state,
	 * any rendering calls may fail either silently or returning False
	*/
	CASCDeviceState			m_DevState;

	/*
	 * Indicates the current scale of device display. This is typically used on
	 * Retina displays to provide mapping between logical and pixel units. For
	 * example, if m_fDisplayScale is 2, then the screen has twice pixel density
	 * for each logical unit
	*/
	ASCSingle				m_fDeviceScale;

	virtual	ASCBoolean InitializeDevice() = 0;
	virtual	void FinalizeDevice() = 0;
	virtual	void ResetDevice();

	virtual	ASCBoolean CanRender(ASCInt nSwapChainIndex);
	virtual void RenderWith(ASCInt nSwapChainIndex, CASCNotifyEvent Handler, ASCUInt uBkColor) = 0;
	virtual void RenderToTarget(CASCNotifyEvent Handler, ASCUInt uBkColor, ASCBoolean bFillBk) = 0;
	virtual ASCBoolean ResizeSwapChain(ASCInt nSwapChainIndex, PASCSwapChainDesc pNewUserDesc);
	virtual void ClearDevStates();
private:
	/*
	 * The list of swap chains that will be used for rendering into. In a typical
	 * scenario at least one swap chain must be added to this list for device
	 * initialization to succeed.
	*/
	CASCSwapChains*	m_pSwapChains;

	/*
	 * The value that should be used for setting depth buffer either on the
	 * screen or the currently used rendering target
	*/
	ASCSingle		m_fFillDepthValue;

	/*
	 * The value that should be used for setting stencil buffer either on the
	 * screen or the currently used rendering target
	*/
	ASCUInt32		m_uFillStencilValue;
};

/*
 * Returns a readable text string with the name of the specified device technology
*/
wstring ASCDeviceTechnologyToString(CASCDeviceTechnology DevTech);

/*
 * Converts device version value originally specified in hexadecimal format
 * (e.g. 0x324) into a readable text string describing that version(e.g. "3.2.4").
 * If bCompact parameter is set to true,  the version text is reduced for
 * trailing zeros, so a text like "3.0" becomes just "3"
*/
wstring ASCDeviceVersionToString(ASCInt nVersion, ASCBoolean bCompact = false);

/*
 * Returns a readable text string that describes the current device's
 * technology, technology version and feature level version.
 * This information can be used for informative purposes
*/
wstring ASCDeviceFullTechnologyToString(CASCDevice* pDevice);