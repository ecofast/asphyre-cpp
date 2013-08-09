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

#include <windows.h>
#include "ASCTypes.h"
#include "ASCEvents.h"
#include "ASCFloatVector2D.h"

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

	// Draws line between the two specified 2D floating-point vectors using gradient of two colors
	virtual void RenderLine(const CASCFloatVector2D Src, const CASCFloatVector2D Dest, ASCColor uColor1, ASCColor uColor2) = 0;
	// Draws line between the two specified 2D floating-point vectors using solid color
	void RenderLine(const CASCFloatVector2D Src, const CASCFloatVector2D Dest, ASCColor uColor);
	// Draws line between the specified coordinates using solid color
	void RenderLine(ASCSingle fX1, ASCSingle fY1, ASCSingle fX2, ASCSingle fY2, ASCColor uColor);

	/*
	 * Flushes the canvas cache and presents the pending primitives on the screen 
	 * or render target. This can be useful to make sure that nothing
	 * remains in canvas cache before starting to draw, for instance, a 3D scene
	*/
	virtual void Flush() = 0;

	/*
     * Resets all the states necessary for canvas operation. 
	 * This can be useful when custom state changes have been made(for instance, in a 3D scene)
	 * so to restore the canvas to its working condition this method should be called
	*/
	virtual void ResetStates();

	/*
	 * The clipping rectangle in which the rendering will be made. 
	 * This can be useful for restricting the rendering to a certain part of screen
	*/
	RECT GetClipRect();
	void SetClipRect(const RECT rc);

	/*
	 * Number of times the rendering cache was reseted during last rendering frame. 
	 * Each cache reset is typically a time-consuming operation so high
	 * number of such events could be detrimental to the application's rendering
	 * performance. If this parameter happens to be considerably high(above 20)
	 * in the rendered scene, the rendering code should be revised for better
	 * grouping of images, shapes and blending types
	*/
	ASCInt GetCacheStall();

protected:
	virtual ASCBoolean HandleDeviceCreate();
	virtual void HandleDeviceDestroy();
	virtual ASCBoolean HandleDeviceReset();
	virtual void HandleDeviceLost();
	virtual void HandleBeginScene()	= 0;
	virtual void HandleEndScene()	= 0;

	virtual void GetViewport(ASCInt* pX, ASCInt* pY, ASCInt* pWidth, ASCInt* pHeight) = 0;
	virtual void SetViewport(ASCInt nX, ASCInt nY, ASCInt nWidth, ASCInt nHeight) = 0;

	/*
	 * Determines whether antialiasing should be used when stretching images and textures.
	 * If this parameter is set to False, no antialiasing will be made and
	 * stretched images will appear pixelated. There is little to none
	 * performance gain from not using antialiasing, so this parameter should
	 * typically be set to True
	*/
	virtual ASCBoolean GetAntialias() = 0;
	virtual void SetAntialias(ASCBoolean bValue) = 0;

	/*
	 * Determines whether mipmapping should be used when rendering images and
	 * textures. Mipmapping can improve visual quality when extreme shrinking of
	 * original images is made at the expense of performance
	*/
	virtual ASCBoolean GetMipMapping() = 0;
	virtual void SetMipMapping(ASCBoolean bValue) = 0;

	void NextRenderCall();
private:
	ASCInt m_nCacheStall;

	void OnDeviceCreate(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnDeviceDestroy(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnDeviceReset(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnDeviceLost(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnBeginScene(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnEndScene(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
};