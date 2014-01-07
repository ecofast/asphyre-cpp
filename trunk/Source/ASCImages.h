/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCImages.h
 *  Content: ASC(Asphyre Sphinx for C++) hardware-accelerated multi-purpose image class
 *  Date   : 2013/08/28
 *  Desc   : Classes and utilities for handling images that contain multiple patterns
 *           and textures suitable for 2D and 3D rendering. The images can be loaded
 *           directly from disk, or even created manually
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <string>
using std::wstring;
#include <vector>
using std::vector;
#include "ASCTypes.h"
#include "ASCTextures.h"
#include "ASCIntVector2D.h"

/*
 * 2-dimensional image implementation that may contain one or more patterns
 * and/or textures which can be rendered on ASC's canvas. The image data can
 * be loaded quickly from external file on disk. The image supports different 
 * pixel formats with or without alpha-channel, mipmapping and dynamic access
*/
class CASCImage
{
public:
	CASCImage();
	~CASCImage();

	virtual void HandleDeviceReset();
	virtual void HandleDeviceLost();

	wstring GetName();
	void SetName(const wstring& sName);

	ASCInt GetPatternCount();
	void SetPatternCount(ASCInt nPatternCount);

	CASCIntVector2D GetPatternSize();
	void SetPatternSize(CASCIntVector2D vec);

	CASCIntVector2D GetVisibleSize();
	void SetVisibleSize(CASCIntVector2D vec);

	ASCBoolean GetMipMapping();
	void SetMipMapping(ASCBoolean bMipMapping);

	CASCPixelFormat GetPixelFormat();
	void SetPixelFormat(CASCPixelFormat fmt);

	ASCBoolean GetDynamicImage();
	void SetDynamicImage(ASCBoolean bDynamicImage);

	ASCInt GetTextureCount();

	CASCLockableTexture* GetTexture(ASCInt nIndex);

	/*
	 * Inserts new texture to the end of the list. The texture is added to the
	 * image's list of textures without initializing. The index of newly added
	 * texture is returned
	*/
	ASCInt InsertTexture();

	/*
	 * Inserts new texture to the end of the list. The texture is initialized
	 * first with the specified size. The reference to newly added texture is
	 * returned and the texture is added to image's list of textures only
	 * if initialization succeeds. If initialization fails, NULL is returned and
	 * no texture is added to the list
	*/
	CASCLockableTexture* InsertTexture(ASCInt nWidth, ASCInt nHeight);

	/*
	 * Removes the texture specified by the given index from the list.
	 * If the index is outside of valid range, this method does nothing
	*/
	void RemoveTexture(ASCInt nIndex);

	/*
	 * Inserts the specified texture sample to the list of image's textures and
	 * returns its new index
	*/
	ASCInt IndexOfTexture(CASCLockableTexture* pSample);

	// Removes and releases all image's textures
	void RemoveAllTextures();

	/*
	 * Loads image consisting of a single texture from external file on disk.
	 * This function returns true on success and false otherwise
	*/
	ASCBoolean LoadFromFile(const wstring& sFileName);
private:
	// The unique name of image that identifies it in the owner's list
	wstring							m_sName;
	vector<CASCLockableTexture*>	m_Textures;
	// The total number of patterns contained within the image
	ASCInt							m_nPatternCount;
	/*
	 * The size of individual pattern inside the image. If no patterns are
	 * stored, this size should match the texture's size
	*/
	CASCIntVector2D					m_PatternSize;
	/*
	 * The visible area inside of each image's pattern that will be used in
	 * rendering. This size must be smaller or equal to m_PatternSize
	*/
	CASCIntVector2D					m_VisibleSize;
	/*
	 * Determines whether the image should contain mipmap data. This can increase
	 * memory consumption and slow down the loading of images, but produces better
	 * visual results when the image is shrunken to smaller sizes
	*/
	ASCBoolean						m_bMipMapping;
	/*
	 * The pixel format that will be used in all image's textures. This parameter
	 * is actually a suggestion and different format may be used in the textures,
	 * depending on hardware support; if this format is not supported, usually
	 * the closest format will be chosen in textures. When loading pixel data from
	 * external files, the conversion will be done automatically, if the texture format
	 * does not match the stored pixel format
	*/
	CASCPixelFormat					m_PixelFormat;
	/*
	 * Determines whether this image requires frequent access to its pixel data.
	 * If this parameter is set to true, dynamic textures will be used, where
	 * pixel data can be updated frequently. This should be used when image pixels
	 * need to be updated at least once per frame
	*/
	ASCBoolean						m_bDynamicImage;
};