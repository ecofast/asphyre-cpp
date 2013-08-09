/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCDX9Device.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 texture implementation
 *  Date   : 2013/08/07
 *  Desc   : General definition of ASC Direct3D9 texture management
 *
 *******************************************************************************************/

#pragma once

#include <d3d9.h>

#include "../ASCTypes.h"
#include "../ASCTextures.h"
#include "ASCDX9Common.h"

class CASCDX9LockableTexture : public CASCLockableTexture
{
public:
	CASCDX9LockableTexture() : CASCLockableTexture()
	{
		m_pSysTexture = 0;
		m_pVideoTexture = 0;

		ResetParams();
	}

	virtual void Bind(ASCInt nStage)
	{
		if (G_pD3D9Device && m_pVideoTexture)
		{
			G_pD3D9Device->SetTexture(nStage, m_pVideoTexture);
		}
	}

	virtual void HandleDeviceReset()
	{
		if ((G_D3D9Mode != admDirect3D9Ex) && (!m_pVideoTexture) && (m_PixelFormat != apfUnknown) && (m_VideoPool == D3DPOOL_DEFAULT ))
		{
			CreateVideoTexture();
		}
	}

	virtual void HandleDeviceLost()
	{
		if ((G_D3D9Mode != admDirect3D9Ex) && (m_VideoPool == D3DPOOL_DEFAULT))
		{
			DestroyVideoTexture();
		}
	}

	virtual void UpdateMipmaps()
	{
		if (m_pVideoTexture)
		{
			m_pVideoTexture->GenerateMipSubLevels();
		}
	}

	virtual void Lock(RECT rc, ASCPointer pBits, ASCInt* pPitch)
	{
		pBits = 0;
		*pPitch = 0;

		/*
		 * If the rectangle specified in Rect is the entire texture, 
		 * then provide null pointer instead
		*/
		RECT* pRc;
		if ((rc.left == 0) && (rc.top == 0) && (rc.right == GetWidth()) && (rc.bottom == GetHeight()))
		{
			pRc = 0;
		} 
		else
		{
			pRc = &rc;
		}

		ASCUInt32 uUsage;
		if (IsDynamicTexture())
		{
			uUsage = D3DLOCK_DISCARD;
			// Only the entire texture can be locked at a time when dealing with dynamic textures
			if (pRc)
			{
				return;
			}
		}
		else
		{
			uUsage = 0;
		}

		D3DLOCKED_RECT rcLocked;
		if (G_D3D9Mode == admDirect3D9Ex)
		{
			if (m_pSysTexture && (SUCCEEDED(m_pSysTexture->LockRect(0, &rcLocked, pRc, uUsage))))
			{
				pBits = rcLocked.pBits;
				*pPitch = rcLocked.Pitch;
			}
		} 
		else
		{
			if (m_pVideoTexture && SUCCEEDED(m_pVideoTexture->LockRect(0, &rcLocked, pRc, uUsage)))
			{
				pBits = rcLocked.pBits;
				*pPitch = rcLocked.Pitch;
			}
		}
	}

	virtual void Unlock()
	{
		if (G_D3D9Mode == admDirect3D9Ex)
		{
			if (m_pSysTexture)
			{
				m_pSysTexture->UnlockRect(0);
			}
			CopySysTextureToVideo();
		} 
		else
		{
			if (m_pVideoTexture)
			{
				m_pVideoTexture->UnlockRect(0);
			}
		}
	}
protected:
	virtual void UpdateSize()
	{
		DestroyVideoTexture();
		CreateVideoTexture();
	}

	virtual ASCBoolean CreateTexture()
	{
		if (!ComputeParams())
		{
			return false;
		}

		if (G_D3D9Mode == admDirect3D9Ex)
		{
			if (!CreateSystemTexture())
			{
				return false;
			}
		}

		return CreateVideoTexture();
	}

	virtual void DestroyTexture()
	{
		DestroyVideoTexture();
		DestroySystemTexture();
		ResetParams();
	}
private:
	IDirect3DTexture9*	m_pSysTexture;
	IDirect3DTexture9*	m_pVideoTexture;
	D3DPOOL				m_VideoPool;
	ASCUInt32			m_uSysUsage;
	ASCUInt32			m_uVideoUsage;

	ASCBoolean ComputeParams()
	{
		m_uSysUsage = 0;
		m_uVideoUsage = 0;
		if (GetMipMapping())
		{
			m_uVideoUsage = m_uSysUsage | D3DUSAGE_AUTOGENMIPMAP;
		}

		if (G_D3D9Mode == admDirect3D9Ex)
		{
			m_VideoPool = D3DPOOL_DEFAULT;

			if (IsDynamicTexture())
			{
				m_uSysUsage = m_uSysUsage | D3DUSAGE_DYNAMIC;
				m_uVideoUsage = m_uVideoUsage | D3DUSAGE_DYNAMIC;
			}

			m_PixelFormat = DX9FindTextureFormatEx(m_PixelFormat, m_uSysUsage, m_uVideoUsage);
		} 
		else
		{
			m_uVideoUsage = D3DPOOL_MANAGED;

			if (IsDynamicTexture())
			{
				m_uVideoUsage = m_uVideoUsage | D3DUSAGE_DYNAMIC;
				m_VideoPool = D3DPOOL_DEFAULT;
			}

			m_PixelFormat = DX9FindTextureFormat(m_PixelFormat, m_uVideoUsage);
		}

		return (m_PixelFormat != apfUnknown);
	}

	void ResetParams()
	{
		m_uSysUsage = 0;
		m_uVideoUsage = 0;
		m_VideoPool = D3DPOOL_SCRATCH;
	}

	ASCBoolean CreateSystemTexture()
	{
		D3DFORMAT fmt = ASCPixelFormatToDX9(m_PixelFormat);
		if (!(G_pD3D9Device && (fmt != D3DFMT_UNKNOWN)))
		{
			return false;
		}
		return (SUCCEEDED(G_pD3D9Device->CreateTexture(GetWidth(), GetHeight(), 1, m_uSysUsage, fmt, D3DPOOL_SYSTEMMEM, &m_pSysTexture, 0)));
	}

	void DestroySystemTexture()
	{
		if (m_pSysTexture)
		{
			m_pSysTexture = 0;
		}
	}

	ASCBoolean CreateVideoTexture()
	{
		ASCInt nLevels;
		if (GetMipMapping())
		{
			nLevels = 0;
		}
		else
		{
			nLevels = 1;
		}

		D3DFORMAT fmt = ASCPixelFormatToDX9(m_PixelFormat);
		if (!(G_pD3D9Device && (fmt != D3DFMT_UNKNOWN)))
		{
			return false;
		}

		return (SUCCEEDED(G_pD3D9Device->CreateTexture(GetWidth(), GetHeight(), nLevels, m_uVideoUsage, fmt, m_VideoPool, &m_pVideoTexture, 0)));
	}

	void DestroyVideoTexture()
	{
		if (m_pVideoTexture)
		{
			m_pVideoTexture = 0;
		}
	}

	ASCBoolean CopySysTextureToVideo()
	{
		if (!(G_pD3D9Device && m_pSysTexture && m_pVideoTexture))
		{
			return false;
		}
		return (SUCCEEDED(G_pD3D9Device->UpdateTexture(m_pSysTexture, m_pVideoTexture)));
	}
};

const ASCInt ASC_TEXTURE_DEPTHSTENCILLEVEL_DX9 = 2;

class CASCDX9RenderTargetTexture : public CASCRenderTargetTexture
{
public:
	CASCDX9RenderTargetTexture() : CASCRenderTargetTexture()
	{
		m_pTexture = 0;
		m_pDepthBuffer = 0;
		m_pSavedBackBuffer = 0;
		m_pSavedDepthBuffer = 0;
		m_DepthStencilFormat = D3DFMT_UNKNOWN;
	}

	virtual void Bind(ASCInt nStage)
	{
		if (G_pD3D9Device && m_pTexture)
		{
			G_pD3D9Device->SetTexture(nStage, m_pTexture);
		}
	}

	virtual void HandleDeviceReset()
	{
		if (G_D3D9Mode != admDirect3D9Ex)
		{
			CreateTextureInstance();
		}
	}

	virtual void HandleDeviceLost()
	{
		if (G_D3D9Mode != admDirect3D9Ex)
		{
			DestroyTextureInstance();
		}
	}

	virtual void UpdateMipmaps()
	{
		if (m_pTexture)
		{
			m_pTexture->GenerateMipSubLevels();
		}
	}

	virtual ASCBoolean BeginRenderTo()
	{
		// 1) Verify initial conditions
		if (!(G_pD3D9Device && m_pTexture))
		{
			return false;
		}

		// 2) Save the currently set buffers
		if (!SaveRenderBuffers())
		{
			return false;
		}

		// 3) Set new render target and depth-stencil
		if (!SetRenderBuffers())
		{
			RestoreRenderBuffers();
			return false;
		}

		// 4) Update viewport to reflect the new render target size
		SetDefaultViewport();

		// 5) Define the current depth-stencil level for Clear() method
		if (GetDepthStencil())
		{
			G_nDX9ActiveDepthStencilLevel = ASC_TEXTURE_DEPTHSTENCILLEVEL_DX9;
		}
		else
		{
			G_nDX9ActiveDepthStencilLevel = 0;
		}

		return true;
	}

	virtual void EndRenderTo()
	{
		RestoreRenderBuffers();
	}
protected:
	virtual void UpdateSize()
	{
		DestroyTextureInstance();
		CreateTextureInstance();
	}

	virtual ASCBoolean CreateTexture()
	{
		return CreateTextureInstance();
	}

	virtual void DestroyTexture()
	{
		DestroyTextureInstance();
	}
private:
	IDirect3DTexture9*	m_pTexture;
	IDirect3DSurface9*	m_pDepthBuffer;
	IDirect3DSurface9*	m_pSavedBackBuffer;
	IDirect3DSurface9*	m_pSavedDepthBuffer;
	D3DFORMAT			m_DepthStencilFormat;

	ASCBoolean CreateTextureInstance()
	{
		if (!G_pD3D9Device)
		{
			return false;
		}

		ASCInt nLevels;
		ASCUInt32 uUsage = D3DUSAGE_RENDERTARGET;
		if (GetMipMapping())
		{
			nLevels = 0;
			uUsage = uUsage | D3DUSAGE_AUTOGENMIPMAP;
		} 
		else
		{
			nLevels = 1;
		}

		m_PixelFormat = DX9FindTextureFormat(m_PixelFormat, uUsage);
		if (m_PixelFormat == apfUnknown)
		{
			return false;
		}

		if (GetDepthStencil())
		{
			m_DepthStencilFormat = DX9FindDepthStencilFormat(G_nDX9ActiveDepthStencilLevel);
			if (m_DepthStencilFormat == D3DFMT_UNKNOWN)
			{
				return false;
			}
		}

		D3DFORMAT fmt =ASCPixelFormatToDX9(m_PixelFormat);
		if (!SUCCEEDED(G_pD3D9Device->CreateTexture(GetWidth(), GetHeight(), nLevels, uUsage, fmt, D3DPOOL_DEFAULT, &m_pTexture, 0)))
		{
			return false;
		}

		if (GetDepthStencil())
		{
			if (!SUCCEEDED(G_pD3D9Device->CreateDepthStencilSurface(GetWidth(), GetHeight(), m_DepthStencilFormat, 
				D3DMULTISAMPLE_NONE, 0, true, &m_pDepthBuffer, 0)))
			{
				m_pTexture = 0;
				return false;
			}
		}

		return true;
	}

	void DestroyTextureInstance()
	{
		if (m_pDepthBuffer)
		{
			m_pDepthBuffer = 0;
		}

		if (m_pTexture)
		{
			m_pTexture = 0;
		}

		m_DepthStencilFormat = D3DFMT_UNKNOWN;
	}

	ASCBoolean SaveRenderBuffers()
	{
		if (!G_pD3D9Device)
		{
			return false;
		}

		if (!SUCCEEDED(G_pD3D9Device->GetRenderTarget(0, &m_pSavedBackBuffer)))
		{
			return false;
		}

		if (G_D3D9PresentParams.EnableAutoDepthStencil)
		{
			if (!SUCCEEDED(G_pD3D9Device->GetDepthStencilSurface(&m_pSavedDepthBuffer)))
			{
				m_pSavedBackBuffer = 0;
				return false;
			}
		}

		return true;
	}

	void RestoreRenderBuffers()
	{
		if (G_pD3D9Device)
		{
			if (G_D3D9PresentParams.EnableAutoDepthStencil)
			{
				G_pD3D9Device->SetDepthStencilSurface(m_pSavedDepthBuffer);
			}
			G_pD3D9Device->SetRenderTarget(0, m_pSavedBackBuffer);
		}

		if (m_pSavedDepthBuffer)
		{
			m_pSavedDepthBuffer = 0;
		}

		if (m_pSavedBackBuffer)
		{
			m_pSavedBackBuffer = 0;
		}
	}

	ASCBoolean SetRenderBuffers()
	{
		if (!(G_pD3D9Device && m_pTexture))
		{
			return false;
		}

		IDirect3DSurface9* pSurf;
		if (!SUCCEEDED(m_pTexture->GetSurfaceLevel(0, &pSurf)))
		{
			return false;
		}

		if (!SUCCEEDED(G_pD3D9Device->SetRenderTarget(0, pSurf)))
		{
			return false;
		}
		else
		{
			return (SUCCEEDED(G_pD3D9Device->SetDepthStencilSurface(m_pDepthBuffer)));
		}
	}

	void SetDefaultViewport()
	{
		if (!G_pD3D9Device)  // if (G_pD3D9Device) ??
		{
			return;
		}

		D3DVIEWPORT9 vp9;
		vp9.X = 0;
		vp9.Y = 0;
		vp9.Width = GetWidth();
		vp9.Height = GetHeight();
		vp9.MinZ = 0.0;
		vp9.MaxZ = 1.0;

		G_pD3D9Device->SetViewport(&vp9);
	}
};