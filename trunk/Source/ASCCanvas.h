/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCCanvas.h
 *  Content: ASC(Asphyre Sphinx for C++) 2D rendering canvas and functions
 *  Date   : 2013/07/24
 *  Desc   : 2D rendering canvas specification and functions for drawing lines, filled
 *           shapes and images using gradient colors and alpha transparency
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <windows.h>
#include "ASCTypes.h"
#include "ASCFloatVector2D.h"
#include "ASCTextures.h"
#include "ASCGraphics.h"

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

	ASCInt GetCacheStall();

	ASCSingle GetDeviceScale();
	void SetDeviceScale(const ASCSingle fValue);

	ASCSingle GetExternalScale();
	void SetExternalScale(const ASCSingle fValue);

	// Draws a single pixel on the screen or render target using the specified 2D floating-point vector
	virtual void RenderPixel(const CASCFloatVector2D Pt, ASCColor uColor) = 0;
	// Draws a single pixel on the screen or render target using the specified coordinates
	void RenderPixel(ASCSingle fX, ASCSingle fY, ASCColor uColor);

	// Draws line between the two specified 2D floating-point vectors using gradient of two colors
	virtual void RenderLine(const CASCFloatVector2D Src, const CASCFloatVector2D Dest, ASCColor uColor1, ASCColor uColor2) = 0;
	// Draws line between the two specified 2D floating-point vectors using solid color
	void RenderLine(const CASCFloatVector2D Src, const CASCFloatVector2D Dest, ASCColor uColor);
	// Draws line between the specified coordinates using solid color
	void RenderLine(ASCSingle fX1, ASCSingle fY1, ASCSingle fX2, ASCSingle fY2, ASCColor uColor);

	/*
	 * Draws multiple filled triangles using the specified vertices, vertex colors and index buffers. 
	 * This is a low-level routine and can be used for drawing complex shapes quickly and efficiently
	*/
	virtual void RenderIndexedTriangles(PASCFloatVector2D pVertices, ASCUInt* pColors, ASCInt* pIndices,
		ASCInt nNumVertices, ASCInt nNumTriangles, CASCBlendingEffect Effect = abeNormal) = 0;

	// Draws filled triangle between the specified vertices and vertex colors
	void FillTriangle(const CASCFloatVector2D p1, const CASCFloatVector2D p2, const CASCFloatVector2D p3,
		ASCColor uColor1, ASCColor uColor2, ASCColor uColor3, CASCBlendingEffect Effect = abeNormal);

	// Draws filled quad between the specified vertices and vertex colors
	void FillQuad(const CASCPoint4 Pts, const CASCColor4 Colors, CASCBlendingEffect Effect = abeNormal);

	// Draws rectangle filled with the specified 4-color gradient
	void FillRect(const RECT Rc, const CASCColor4 Colors, CASCBlendingEffect Effect = abeNormal);

	// Draws rectangle filled with solid color
	void FillRect(const RECT Rc, ASCColor uColor, CASCBlendingEffect Effect = abeNormal);

	// Draws rectangle at the given coordinates filled with solid color
	void FillRect(ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight, ASCColor uColor, CASCBlendingEffect Effect = abeNormal);

	/*
	 * Draws lines between four corners of the given rectangle where the lines
	 * are filled using 4-color gradient. This method uses filled shapes instead
	 * of line primitives for pixel-perfect mapping but assumes that the four
	 * vertex points are aligned to form rectangle
	*/
	void FrameRect(const CASCPoint4 Pts, const CASCColor4 Colors, CASCBlendingEffect Effect = abeNormal);

	/*
	 * Draws lines that form the specified rectangle using colors from the given
	 * 4-color gradient. This primitive uses filled shapes and not actual lines
	 * for pixel-perfect mapping
	*/
	void FrameRect(const RECT Rc, const CASCColor4 Colors, CASCBlendingEffect Effect = abeNormal);
	void FrameRect(ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight, ASCColor uColor, CASCBlendingEffect Effect = abeNormal);

	/*
	 * Draws horizontal line using the specified coordinates and filled with
	 * two color gradient. This primitive uses a filled shape and not line
	 * primitive for pixel-perfect mapping
	*/
	void RenderHorizLine(ASCSingle fLeft, ASCSingle fTop, ASCSingle fWidth, ASCColor uColor1, ASCColor uColor2, CASCBlendingEffect Effect = abeNormal);
	void RenderHorizLine(ASCSingle fLeft, ASCSingle fTop, ASCSingle fWidth, ASCColor uColor, CASCBlendingEffect Effect = abeNormal);

	/*
	 * Draws vertical line using the specified coordinates and filled with
	 * two color gradient. This primitive uses a filled shape and not line
	 * primitive for pixel-perfect mapping
	*/
	void RenderVertLine(ASCSingle fLeft, ASCSingle fTop, ASCSingle fHeight, ASCColor uColor1, ASCColor uColor2, CASCBlendingEffect Effect = abeNormal);
	void RenderVertLine(ASCSingle fLeft, ASCSingle fTop, ASCSingle fHeight, ASCColor uColor, CASCBlendingEffect Effect = abeNormal);

	/*
	 * Draws a filled rectangle at the given position and size with a hole(in
	 * form of ellipse) inside at the given center and radius. The quality of the
	 * hole is defined by the value of nSteps in number of subdivisions.
	 * This entire shape is filled with gradient starting from outer color at the
	 * edges of rectangle and inner color ending at the edge of hole. This shape
	 * can be particularly useful for highlighting items on the screen by
	 * darkening the entire area except the one inside the hole
	*/
	void RenderQuadHole(const CASCFloatVector2D Pos, const CASCFloatVector2D Size, const CASCFloatVector2D Center, const CASCFloatVector2D Radius,
		ASCColor uOutColor, ASCColor uInColor, ASCInt nSteps, CASCBlendingEffect Effect = abeNormal);

	/*
	 * Draws textured rectangle at the given vertices and multiplied by the 
	 * specified 4-color gradient. All pixels of the rendered texture are multiplied
	 * by the gradient color before applying alpha-blending. If the texture has  
	 * no alpha-channel present, alpha value of the gradient will be used instead
	*/
	virtual void RenderTexture(CASCTexture* pTexture, CASCPoint4 Points, CASCPoint4 Mappings, 
		CASCColor4 Colors, CASCBlendingEffect Effect = abeNormal) = 0;

	void RenderTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, 
		CASCBlendingEffect Effect = abeNormal);
	void RenderTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCColor Color, CASCBlendingEffect Effect = abeNormal);
	void RenderTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, CASCBlendingEffect Effect = abeNormal);

	void RenderScaledTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCSingle fScale, 
		ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect = abeNormal);
	void RenderScaledTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCSingle fScale, 
		ASCColor Color = cASCColor32White, CASCBlendingEffect Effect = abeNormal);

	void RenderStretchTexture(ASCSingle fLeft, ASCSingle fTop, ASCSingle fRight, ASCSingle fBottom, CASCTexture* pTexture, 
		ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect = abeNormal);
	void RenderStretchTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCSingle fWidth, ASCSingle fHeight, ASCSingle fScaleX, ASCSingle fScaleY, 
		ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect = abeNormal);
	
	void RenderPartTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCInt nSrcX1, ASCInt nSrcY1, ASCInt nSrcX2, ASCInt nSrcY2, 
		ASCSingle fScaleX, ASCSingle fScaleY, ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect = abeNormal);

	void RenderRotateTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCSingle fAngle, ASCSingle fScaleX, ASCSingle fScaleY, 
		ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect = abeNormal);

	void RenderTextureWaveX(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCInt nWidth, ASCInt nHeight, ASCInt nAmp, ASCInt nLen, ASCInt nPhase, 
		ASCColor Color = cASCColor32White, CASCBlendingEffect Effect = abeNormal);
	void RenderTextureWaveY(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCInt nWidth, ASCInt nHeight, ASCInt nAmp, ASCInt nLen, ASCInt nPhase, 
		ASCColor Color = cASCColor32White, CASCBlendingEffect Effect = abeNormal);
protected:
	ASCSingle	m_fInternalScale;

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
	/*
	 * Number of times the rendering cache was reseted during last rendering frame. 
	 * Each cache reset is typically a time-consuming operation so high
	 * number of such events could be detrimental to the application's rendering
	 * performance. If this parameter happens to be considerably high(above 20)
	 * in the rendered scene, the rendering code should be revised for better
	 * grouping of images, shapes and blending types
	*/
	ASCInt		m_nCacheStall;

	/*
	 * Determines the current scale of device to be rendered on. This value should
	 * be typically taken from CASCDevice when rendering on the screen, or set to 1.0
	 * when rendering on render target. Additionally, this value can be set to
	 * other values to compensate for screen's DPI
	*/
	ASCSingle	m_fDeviceScale;

	/*
	 * Determines the scale that user(or application) uses for rendering
	 * on this canvas. If this scale matches m_fDeviceScale, then
	 * pixel to pixel mapping is achieved
	*/
	ASCSingle	m_fExternalScale;

	void UpdateInternalScale();

	void OnDeviceCreate(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnDeviceDestroy(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnDeviceReset(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnDeviceLost(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnBeginScene(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
	void OnEndScene(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled);
};