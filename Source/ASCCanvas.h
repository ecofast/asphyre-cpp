/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCCanvas.h
 *  Content: ASC(Asphyre Sphinx for C++) 2D rendering canvas and functions
 *  Date   : 2013/07/24
 *  Desc   : 2D rendering canvas specification and functions for drawing lines, filled
 *           shapes and images using gradient colors and alpha transparency
 *
 *******************************************************************************************/

#pragma once

#include "ASCTypes.h"

/*
 * The blending effect that should be applied when drawing 2D primitives
*/
enum CASCBlendingEffect
{
	/*
	 * Undefined blending effect. This effect type is used internally and should
	 * not be used elsewhere
	*/
	abeUnknown,
	/*
	 * Normal blending effect. If drawing primitive has alpha-channel supplied,
	 * it will be alpha-blended to the destination depending on alpha transparency
	*/
	abeNormal,
	/*
	 * Shadow drawing effect. The screen(or render target) will be multiplied by
	 * alpha-channel of the source primitive; thus, the rendered image will look like a shadow
	*/
	abeShadow,
	/*
	 * Addition blending effect. The source primitive will be multiplied by its
	 * alpha-channel and then added to the destination using saturation.
	*/
	abeAdd,
	/*
	 * Multiplication blending effect. The screen(or render target) will be
	 * multiplied by the source primitive
	*/
	abeMultiply,
	/*
	 * Inverse multiplication effect. The screen(or render target) will be
	 * multiplied by an inverse of the source primitive
	*/
	abeInvMultiply,
	/*
	 * Source color blending effect. Instead of using alpha-channel, the
	 * grayscale value of source primitive's pixels will be used as an alpha
	 * value for blending on destination
	*/
	abeSrcColor,
	/*
	 * Source color addition effect. Instead of using alpha-channel, the
	 * grayscale value of source primitive's pixels will be used as an alpha
	 * value for multiplying source pixels, which will then be added to
	 * destination using saturation
	*/
	abeSrcColorAdd
};

/*
 * 2D canvas specification for rendering points, lines, filled shapes
 * and images using gradient color fills, different blending effects,
 * and alpha transparency
*/
class CASCCanvas
{
public:
	CASCCanvas();
	~CASCCanvas();

protected:
	virtual ASCBoolean	HandleDeviceCreate();
	virtual void		HandleDeviceDestroy();
	virtual ASCBoolean	HandleDeviceReset();
	virtual void		HandleDeviceLost();
	virtual void		HandleBeginScene()	= 0;
	virtual void		HandleEndScene()	= 0;
private:
	ASCInt				m_nCacheStall;

	void				OnDeviceCreate(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled);
	void				OnDeviceDestroy(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled);
	void				OnDeviceReset(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled);
	void				OnDeviceLost(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled);
	void				OnBeginScene(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled);
	void				OnEndScene(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled);
};