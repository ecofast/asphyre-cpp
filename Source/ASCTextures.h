/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCTextures.h
 *  Content: ASC(Asphyre Sphinx for C++) Custom Texture implementation
 *  Date   : 2013/07/19
 *  Desc   : Texture specification and general implementation common to all providers
 *           in ASC framework
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCTypes.h"
#include "ASCUtils.h"
#include "ASCIntVector2D.h"
#include "ASCFloatVector2D.h"

/*
 * General texture specification, which specifies common parameters and
 * provides basic utilities
*/
class CASCTexture
{
public:
	CASCTexture();
	~CASCTexture();
	
	/*
	 * Initializes the texture creating all provider specific resources. If this
	 * method succeeds, the texture can be used for rendering and the returned
	 * value is True. If the returned value is False, it means that the texture
	 * initialization failed and configuration parameters need to be revised
	*/
	ASCBoolean Initialize();

	// Finalizes the texture releasing all provider specific resources
	void Finalize();

	// Returns the pointer to shader resource view when used in latest DX10+ providers
	virtual	ASCPointer GetResourceView();

	// Binds the texture to the given stage index in DX9- and OGL providers
	virtual	void Bind(ASCInt nStage);

	virtual	void HandleDeviceReset();
	virtual	void HandleDeviceLost();

	/*
	 * Converts 2D integer pixel coordinates to their logical representation
	 * provided in range of [0..1]
	*/
	CASCFloatVector2D PixelToLogical(const CASCIntVector2D& pos);

	/*
	 * Converts 2D floating-point pixel coordinates to their logical
	 * representation provided in range of [0..1]
	*/
	CASCFloatVector2D PixelToLogical(const CASCFloatVector2D& pos);

	/*
	 * Converts 2D logic texture coordinates in range of [0..1] to pixel
	 * coordinates
	*/
	CASCIntVector2D LogicalToPixel(const CASCFloatVector2D& pos);
	
	/*
	 * Updates all mipmap images contained in the texture. This should only be
	 * used when m_bMipmapping is set to True
	*/
	virtual	void UpdateMipmaps();

	CASCPixelFormat GetPixelFormat();
	void SetPixelFormat(const CASCPixelFormat fmt);

	ASCInt GetWidth();
	void SetWidth(const ASCInt nW);

	ASCInt GetHeight();
	void SetHeight(const ASCInt nH);

	ASCBoolean GetActive();

	// Indicates how many bytes each pixel in texture occupies
	ASCInt BytesPerPixel();

	ASCBoolean GetMipMapping();
	void SetMipMapping(const ASCBoolean bMM);
protected:
	// The pixel format of the texture's surface and its mipmap levels
	CASCPixelFormat	m_PixelFormat;

	virtual	ASCInt GetBytesPerPixel();
	virtual	void UpdateSize();
	virtual	ASCBoolean CreateTexture();
	virtual	void DestroyTexture();
private:
	// The width of texture's surface or first mipmap level
	ASCInt		m_nWidth;

	// The height of texture's surface or first mipmap level
	ASCInt		m_nHeight;

	// Indicates whether the texture has been created and initialized properly
	ASCBoolean	m_bActive;

	/*
	 * Determines whether mipmapping should be used for this texture. If this
	 * parameter is set to True, a full set of mipmap levels will be used in
	 * the texture and handled by its specific provider implementation.
	 * Mipmapping is used when the texture is drawn in significantly smaller sizes
	 * to improve visual quality of the displayed image
	*/
	ASCBoolean	m_bMipMapping;
};

/*
 * Lockable texture specification where full support is provided for direct
 * access to texture pixel data. This texture can typically be loaded from
 * disk and can have its pixels modified while the application is running.
 * This is the most common type of textures used in ASC framework
*/
class CASCLockableTexture : public CASCTexture
{
public:
	CASCLockableTexture();

	/*
	 * Provides access to the raw texture's pixel data. If mipmapping is enabled,
	 * this gives access to the top-level mipmap(other mipmaps will be updated
	 * automatically). After accessing the texture's pixel data it is necessary
	 * to call Unlock.
	 *
	 * Params:
	 * rc: The rectangle inside the texture that will be updated. For dynamic textures
	 *     this rectangle should cover the entire texture in some providers.
	 * pBits: In this parameter the pointer to the top-left pixel is provided 
	 *        within the specified rectangle. If the method fails, NULL is returned.
	 * nPitch: The number of bytes that each scanline occupies in the texture. 
	 *         This value can be used for accessing individual rows when
	 *         accessing pixel data. If the method fails, zero is returned.
	*/
	virtual	void Lock(RECT rc, ASCPointer& pBits, ASCInt& nPitch) = 0;

	/*
	 * Finishes accessing texture's pixel data. If mipmapping is enabled, 
	 * other mipmap levels are updated automatically
	*/
	virtual	void Unlock() = 0;

	/*
	 * This provides direct access to texture's pixels.
	 * The pixel values are handled in 32-bit RGBA pixel format(apf_A8R8G8B8). 
	 * If the actual pixel format used in the texture is different, 
	 * the conversion is done automatically. 
	 * In some providers using the routines may induce significant
	 * performance hit, especially if mipmapping is enabled. 
	 * For performance-critical applications it is better to get access to all
	 * texture pixels at once using Lock and Unlock methods instead.
	*/
	ASCUInt GetPixel(const ASCInt nX, const ASCInt nY);
	void SetPixel(const ASCInt nX, const ASCInt nY, const ASCUInt uVal);

	/*
	 * Determines whether the texture will be used for frequent access and
	 * updates. This is useful for textures that are modified at least once
	 * per rendering frame
	*/
	ASCBoolean IsDynamicTexture();
	void SetIsDynamicTexture(const ASCBoolean bVal);
private:
	ASCBoolean	m_bDynamicTexture;
};

/*
 * Render target texture specification which supports drawing the entire
 * scene directly on the texture. This type of texture may not be supported on
 * some providers. It can be used for advanced multi-pass rendering effects
*/
class CASCRenderTargetTexture : public CASCTexture
{
public:
	CASCRenderTargetTexture();

	virtual	ASCBoolean BeginRenderTo() = 0;
	virtual	void EndRenderTo() = 0;

	ASCBoolean GetDepthStencil();
	void SetDepthStencil(const ASCBoolean bVal);

	ASCInt GetMultisamples();
	void SetMultisamples(const ASCInt nVal);
protected:
	/*
	 * Determines the number of samples used for antialiasing. This parameter is
	 * supported only on the newest DX10+ providers
	*/
	ASCInt		m_nMultisamples;
private:
	/*
	 * Determines whether depth-stencil buffer should be created and used with
	 * this texture
	*/
	ASCBoolean	m_bDepthStencil;
};