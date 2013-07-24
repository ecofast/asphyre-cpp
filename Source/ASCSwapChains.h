/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCSwapChains.h
 *  Content: ASC(Asphyre Sphinx for C++) generic swap chains declaration
 *  Date   : 2013/05/31
 *  Desc   : Specification and implementation of rendering swap chains used in ASC providers
 *
 *******************************************************************************************/

#pragma once

#include <vector>
using std::vector;

#include "ASCTypes.h"
#include "ASCIntVector2D.h"

// The type of depth and stencil buffers to be created in the swap chain
enum CDepthStencilType {dstNone,         // No depth-stencil buffer is needed 
                        dstDepthOnly,    // Only depth buffer is required, stencil buffer is not needed
                        dstDepthStencil  // Both depth and stencil buffers are required
                       };

// General description of rendering swap chain
struct CSwapChainDesc
{
	// The width of rendering surface
	ASCInt				Width;
	// The height of rendering surface
	ASCInt				Height;
	// determine whether to wait for vertical retrace to provide flicker-free animations
	ASCBoolean			VSync;
	/*
	 * The desired pixel format to be used in the rendering target. This is a
	 * suggestion and different format may be choosen by the provider depending
	 * on hardware support. If this parameter is set to apfUnknown(by default),
	 * the best possible pixel format will be used
	*/
	ASCPixelFormat		Format;
	/*
	 * The handle to the application's main window or a control where the
	 * rendering should be made(can be another window or even a panel)
	*/
	ASCUInt				WndHandle;
	/*
	 * Number of samples to use for antialiasing. This is a suggestion and
	 * different value may actually be used by the provider depending on
	 * hardware support; values of zero and one are treated as no multisampling
	*/
	ASCInt				Multisamples;
	// The type of depth-stencil buffer to be used with the swap chain
	CDepthStencilType	DepthStencil;
};

// Pointer to SwapChainDesc structure typically used to pass it by reference
typedef CSwapChainDesc* PSwapChainDesc;

/*
 * List of all rendering swap chains that are to be used with ASC device.
 * This class describes all swap chains that should be created and used with
 * the device; if the device is already initialized, modifying swap chains is
 * not allowed
*/
class CASCSwapChains
{
public:
	CASCSwapChains(void* pDevice);
	~CASCSwapChains();

	// Inserts a new swap chain to the end of list and returns its index
	ASCInt					Insert();
	/*
	 * Adds a new rendering swap chain specified in the given structure to the
	 * end of list and returns its index
	*/
	ASCInt					Add(const CSwapChainDesc desc);
	/*
	 * Adds a new rendering swap chain with the specified parameters to the end
	 * of list and returns its index
	*/
	ASCInt					Add(ASCUInt uWndHandle, ASCIntVector2D vtSize, 
								ASCInt nMultisamples = 0, ASCBoolean bVSync = false, ASCPixelFormat Format = apfUnknown,
								CDepthStencilType DepthStencil = dstNone);
	/*
	 * Removes the swap at the specified index from the list, shifting all
	 * elements by one. The index should be in range of [0..(Count - 1)] range;
	 * if it is outside of valid range, this function does nothing
	*/
	void					Remove(ASCInt nIndex);
	// Removes all rendering swap chains from the list
	void					RemoveAll();
	/*
	 * The pointer to a valid ASC device which owns this list of rendering
	 * swap chains
	*/
	void*					GetDevice();
	// Number of swap chains in the list
	ASCInt					GetCount();
	/*
	 * Provides access to each of the rendering swap chains in the list by index,
	 * which should be in range of [0..(Count - 1)] range. If the index is
	 * outside of valid range, @nil is returned
	*/
	PSwapChainDesc			GetItem(ASCInt nIndex);
private:
	vector<CSwapChainDesc>	m_Datas;
	void*					m_pDevice;
	ASCBoolean				IsDeviceInactive();
};