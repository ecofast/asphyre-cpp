/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCTextures.h
 *  Content: ASC(Asphyre Sphinx for C++) Custom Texture implementation
 *  Date   : 2013/07/19
 *  Desc   : Texture specification and general implementation common to all providers
 *           in ASC framework
 *
 *******************************************************************************************/

#pragma once

#include "ASCTypes.h"
#include "ASCUtils.h"
#include "ASCIntVector2D.h"
#include "ASCFloatVector2D.h"

/*
RECT ASCBounds(ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight)
{
	RECT rcResult;
	rcResult.left	= nLeft;
	rcResult.top	= nTop;
	rcResult.right	= nLeft + nWidth;
	rcResult.bottom	= nTop + nHeight;
	return rcResult;
}
*/

/*
 * General texture specification, which specifies common parameters and
 * provides basic utilities
*/
class CASCTexture
{
public:
	CASCTexture()
	{
		m_nWidth		= 256;
		m_nHeight		= 256;
		m_bActive		= false;
		m_PixelFormat	= apfUnknown;
		m_bMipMapping	= false;
	}

	~CASCTexture()
	{
		if (m_bActive)
		{
			Finalize();
		}
	}

	/*
	 * Initializes the texture creating all provider specific resources. If this
	 * method succeeds, the texture can be used for rendering and the returned
	 * value is True. If the returned value is False, it means that the texture
	 * initialization failed and configuration parameters need to be revised
	*/
	ASCBoolean Initialize()
	{
		ASCBoolean bResult = !m_bActive;
		if (!bResult)
		{
			return false;
		}
		else
		{
			bResult = CreateTexture();
			m_bActive = bResult;
			return bResult;
		}
	}

	// Finalizes the texture releasing all provider specific resources
	void Finalize()
	{
		if (m_bActive)
		{
			DestroyTexture();
		}
		m_bActive = false;
	}

	// Returns the pointer to shader resource view when used in latest DX10+ providers
	virtual	ASCPointer GetResourceView()
	{
		return 0;
	}

	// Binds the texture to the given stage index in DX9- and OGL providers
	virtual	void Bind(ASCInt nStage)
	{

	}

	virtual	void HandleDeviceReset()
	{

	}

	virtual	void HandleDeviceLost()
	{

	}

	/*
	 * Converts 2D integer pixel coordinates to their logical representation
	 * provided in range of [0..1]
	*/
	CASCFloatVector2D PixelToLogical(const CASCIntVector2D& pos)
	{
		CASCFloatVector2D result;
	
		if (m_nWidth > 0)
		{
			result.X = (ASCSingle)pos.X / m_nWidth;
		} 
		else
		{
			result.X = 0.0;
		}

		if (m_nHeight > 0)
		{
			result.Y = (ASCSingle)pos.Y / m_nHeight;
		} 
		else
		{
			result.Y = 0.0;
		}

		return result;
	}

	/*
	 * Converts 2D floating-point pixel coordinates to their logical
	 * representation provided in range of [0..1]
	*/
	CASCFloatVector2D PixelToLogical(const CASCFloatVector2D& pos)
	{
		CASCFloatVector2D result;

		if (m_nWidth > 0)
		{
			result.X = pos.X / m_nWidth;
		} 
		else
		{
			result.X = 0.0;
		}

		if (m_nHeight > 0)
		{
			result.Y = pos.Y / m_nHeight;
		} 
		else
		{
			result.Y = 0.0;
		}

		return result;
	}

	/*
	 * Converts 2D logic texture coordinates in range of [0..1] to pixel
	 * coordinates
	*/
	CASCIntVector2D LogicalToPixel(const CASCFloatVector2D& pos)
	{
		CASCIntVector2D	result;

		result.X = ASCRound(pos.X * m_nWidth);
		result.Y = ASCRound(pos.Y * m_nHeight);
		return result;
	}

	/*
	 * Updates all mipmap images contained in the texture. This should only be
	 * used when m_bMipmapping is set to True
	*/
	virtual	void UpdateMipmaps()
	{

	}

	// The pixel format of the texture's surface and its mipmap levels
	CASCPixelFormat GetPixelFormat()
	{
		return m_PixelFormat;
	}

	void SetPixelFormat(const CASCPixelFormat fmt)
	{
		if (!m_bActive)
		{
			m_PixelFormat = fmt;
		}
	}

	// The width of texture's surface or first mipmap level
	ASCInt GetWidth()
	{
		return m_nWidth;
	}

	void SetWidth(const ASCInt nW)
	{
		m_nWidth = nW;
		if (m_bActive)
		{
			UpdateSize();
		}
	}

	// The height of texture's surface or first mipmap level
	ASCInt GetHeight()
	{
		return m_nHeight;
	}

	void SetHeight(const ASCInt nH)
	{
		m_nHeight = nH;
		if (m_bActive)
		{
			UpdateSize();
		}
	}

	// Indicates whether the texture has been created and initialized properly
	ASCBoolean GetActive()
	{
		return m_bActive;
	}

	// Indicates how many bytes each pixel in texture occupies
	ASCInt BytesPerPixel()
	{
		return (GetBytesPerPixel());
	}

	/*
	 * Determines whether mipmapping should be used for this texture. If this
	 * parameter is set to True, a full set of mipmap levels will be used in
	 * the texture and handled by its specific provider implementation.
	 * Mipmapping is used when the texture is drawn in significantly smaller sizes
	 * to improve visual quality of the displayed image
	*/
	ASCBoolean GetMipMapping()
	{
		return m_bMipMapping;
	}

	void SetMipMapping(const ASCBoolean bMM)
	{
		if (!m_bActive)
		{
			m_bMipMapping = bMM;
		}
	}
protected:
	CASCPixelFormat	m_PixelFormat;

	virtual	ASCInt GetBytesPerPixel()
	{
		return (C_ASCPixelFormatBits[m_PixelFormat] / 8);
	}

	virtual	void UpdateSize()
	{

	}

	virtual	ASCBoolean CreateTexture()
	{
		return true;
	}

	virtual	void DestroyTexture()
	{

	}
private:
	ASCInt		m_nWidth;
	ASCInt		m_nHeight;
	ASCBoolean	m_bActive;
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
	CASCLockableTexture()
	{
		CASCTexture::CASCTexture();
		m_bDynamicTexture = false;
	}

	/*
	 * Provides access to the raw texture's pixel data. If mipmapping is enabled,
	 * this gives access to the top-level mipmap(other mipmaps will be updated
	 * automatically). After accessing the texture's pixel data it is necessary
	 * to call Unlock.
	 *
	 * Params:
	 * Rect: The rectangle inside the texture that will be updated. For dynamic textures
	 *       this rectangle should cover the entire texture in some providers.
	 * Bits: In this parameter the pointer to the top-left pixel is provided 
	 *       within the specified rectangle. If the method fails, NULL is returned.
	 * Pitch: The number of bytes that each scanline occupies in the texture. 
	 *        This value can be used for accessing individual rows when
	 *        accessing pixel data. If the method fails, zero is returned.
	*/
	virtual	void Lock(const RECT rc, ASCPointer pBits, ASCInt* pPitch) = 0;

	/*
	 * Finishes accessing texture's pixel data. If mipmapping is enabled, 
	 * other mipmap levels are updated automatically
	*/
	virtual	void Unlock() = 0;

	/*
	 * The pixel values are handled in 32-bit RGBA pixel format(apf_A8R8G8B8). 
	 * If the actual pixel format used in the texture is different, 
	 * the conversion is done automatically. 
	 * In some providers using the routines may induce significant
	 * performance hit, especially if mipmapping is enabled. 
	 * For performance-critical applications it is better to get access to all
	 * texture pixels at once using Lock and Unlock methods instead.
	*/
	ASCUInt GetPixel(const ASCInt nX, const ASCInt nY)
	{
		if ((nX < 0) | (nY < 0) | (nX >= GetWidth()) | (nY >= GetHeight()))
		{
			return 0;
		}

		ASCUInt uResult = 0;
		ASCPointer		pBits;
		ASCInt			nPitch;
		Lock(ASCBounds(0, nY, GetWidth(), 1), pBits, &nPitch);
		if (!pBits)
		{
			return 0;
		}
		else
		{
			// uResult = PixelXto32(Pointer(PtrInt(Bits) + PtrInt(x) * BytesPerPixel), FFormat);
			Unlock();
		}
		
	}

	void SetPixel(const ASCInt nX, const ASCInt nY, const ASCUInt uVal)
	{
		if ((nX < 0) | (nY < 0) | (nX >= GetWidth()) | (nY >= GetHeight()))
		{
			return;
		}

		ASCPointer		pBits;
		ASCInt			nPitch;
		Lock(ASCBounds(0, nY, GetWidth(), 1), pBits, &nPitch);
		if (!pBits)
		{
			return;
		}
		else
		{
			// Pixel32toX(uVal, Pointer(PtrInt(Bits) + PtrInt(x) * BytesPerPixel), FFormat);
			Unlock();
		}
	}

	/*
	 * Determines whether the texture will be used for frequent access and
	 * updates. This is useful for textures that are modified at least once
	 * per rendering frame
	*/
	ASCBoolean IsDynamicTexture()
	{
		return m_bDynamicTexture;
	}

	void SetIsDynamicTexture(const ASCBoolean bVal)
	{
		if (!GetActive())
		{
			m_bDynamicTexture = bVal;
		}
		
	}
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
	CASCRenderTargetTexture()
	{
		CASCTexture::CASCTexture();
		m_bDepthStencil = false;
		m_nMultisamples = 0;
	}

	virtual	ASCBoolean BeginRenderTo()	= 0;
	virtual	void EndRenderTo()	= 0;

	/*
	 * Determines whether depth-stencil buffer should be created and used with
	 * this texture
	*/
	ASCBoolean GetDepthStencil()
	{
		return m_bDepthStencil;
	}

	void SetDepthStencil(const ASCBoolean bVal)
	{
		if (!GetActive())
		{
			m_bDepthStencil = bVal;
		}
		
	}

	/*
	 * Determines the number of samples used for antialiasing. This parameter is
	 * supported only on the newest DX10+ providers
	*/
	ASCInt GetMultisamples()
	{
		return m_nMultisamples;
	}

	void SetMultisamples(const ASCInt nVal)
	{
		if (!GetActive())
		{
			m_nMultisamples = nVal;
		}
		
	}
protected:
	ASCInt		m_nMultisamples;
private:
	ASCBoolean	m_bDepthStencil;
};