/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCDX9Textures.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 texture implementation
 *  Date   : 2013/08/07
 *  Desc   : General definition of ASC Direct3D9 texture management
 *
 *******************************************************************************************/

#pragma once

#include "../ASCConfig.h"

#include <d3d9.h>
#include "../ASCTypes.h"
#include "../ASCTextures.h"

class CASCDX9LockableTexture : public CASCLockableTexture
{
public:
	CASCDX9LockableTexture();

	virtual void Bind(ASCInt nStage);
	virtual void HandleDeviceReset();
	virtual void HandleDeviceLost();
	virtual void UpdateMipmaps();
	virtual void Lock(RECT rc, ASCPointer& pBits, ASCInt& nPitch);
	virtual void Unlock();
protected:
	virtual void UpdateSize();
	virtual ASCBoolean CreateTexture();
	virtual void DestroyTexture();
private:
	IDirect3DTexture9*	m_pSysTexture;
	IDirect3DTexture9*	m_pVideoTexture;
	D3DPOOL				m_VideoPool;
	ASCUInt32			m_uSysUsage;
	ASCUInt32			m_uVideoUsage;

	ASCBoolean ComputeParams();
	void ResetParams();
	ASCBoolean CreateSystemTexture();
	void DestroySystemTexture();
	ASCBoolean CreateVideoTexture();
	void DestroyVideoTexture();
	ASCBoolean CopySysTextureToVideo();
};

class CASCDX9RenderTargetTexture : public CASCRenderTargetTexture
{
public:
	CASCDX9RenderTargetTexture();

	virtual void Bind(ASCInt nStage);
	virtual void HandleDeviceReset();
	virtual void HandleDeviceLost();
	virtual void UpdateMipmaps();
	virtual ASCBoolean BeginRenderTo();
	virtual void EndRenderTo();
protected:
	virtual void UpdateSize();
	virtual ASCBoolean CreateTexture();
	virtual void DestroyTexture();
private:
	IDirect3DTexture9*	m_pTexture;
	IDirect3DSurface9*	m_pDepthBuffer;
	IDirect3DSurface9*	m_pSavedBackBuffer;
	IDirect3DSurface9*	m_pSavedDepthBuffer;
	D3DFORMAT			m_DepthStencilFormat;

	ASCBoolean CreateTextureInstance();
	void DestroyTextureInstance();
	ASCBoolean SaveRenderBuffers();
	void RestoreRenderBuffers();
	ASCBoolean SetRenderBuffers();
	void SetDefaultViewport();
};