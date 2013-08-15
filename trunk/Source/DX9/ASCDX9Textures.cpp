#include "ASCDX9Textures.h"
#include "ASCDX9Common.h"

CASCDX9LockableTexture::CASCDX9LockableTexture() : CASCLockableTexture()
{
	m_pSysTexture = 0;
	m_pVideoTexture = 0;

	ResetParams();
}

void CASCDX9LockableTexture::Bind(ASCInt nStage)
{
	if (G_pD3D9Device && m_pVideoTexture)
	{
		G_pD3D9Device->SetTexture(nStage, m_pVideoTexture);
	}
}

void CASCDX9LockableTexture::HandleDeviceReset()
{
	if ((G_D3D9Mode != admDirect3D9Ex) && (!m_pVideoTexture) && (m_PixelFormat != apfUnknown) && (m_VideoPool == D3DPOOL_DEFAULT ))
	{
		CreateVideoTexture();
	}
}

void CASCDX9LockableTexture::HandleDeviceLost()
{
	if ((G_D3D9Mode != admDirect3D9Ex) && (m_VideoPool == D3DPOOL_DEFAULT))
	{
		DestroyVideoTexture();
	}
}

void CASCDX9LockableTexture::UpdateMipmaps()
{
	if (m_pVideoTexture)
	{
		m_pVideoTexture->GenerateMipSubLevels();
	}
}

void CASCDX9LockableTexture::Lock(RECT rc, ASCPointer& pBits, ASCInt& nPitch)
{
	pBits = 0;
	nPitch = 0;

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

	ASCUInt uUsage;
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
			nPitch = rcLocked.Pitch;
		}
	} 
	else
	{
		if (m_pVideoTexture && SUCCEEDED(m_pVideoTexture->LockRect(0, &rcLocked, pRc, uUsage)))
		{
			pBits = rcLocked.pBits;
			nPitch = rcLocked.Pitch;
		}
	}
}

void CASCDX9LockableTexture::Unlock()
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

void CASCDX9LockableTexture::UpdateSize()
{
	DestroyVideoTexture();
	CreateVideoTexture();
}

ASCBoolean CASCDX9LockableTexture::CreateTexture()
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

void CASCDX9LockableTexture::DestroyTexture()
{
	DestroyVideoTexture();
	DestroySystemTexture();
	ResetParams();
}

ASCBoolean CASCDX9LockableTexture::ComputeParams()
{
	m_uSysUsage = 0;
	if (GetMipMapping())
	{
		m_uVideoUsage = D3DUSAGE_AUTOGENMIPMAP;
	}
	else
	{
		m_uVideoUsage = 0;
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

void CASCDX9LockableTexture::ResetParams()
{
	m_uSysUsage = 0;
	m_uVideoUsage = 0;
	m_VideoPool = D3DPOOL_SCRATCH;
}

ASCBoolean CASCDX9LockableTexture::CreateSystemTexture()
{
	D3DFORMAT fmt = ASCPixelFormatToDX9(m_PixelFormat);
	if (!(G_pD3D9Device && (fmt != D3DFMT_UNKNOWN)))
	{
		return false;
	}
	return (SUCCEEDED(G_pD3D9Device->CreateTexture(GetWidth(), GetHeight(), 1, m_uSysUsage, fmt, D3DPOOL_SYSTEMMEM, &m_pSysTexture, 0)));
}

void CASCDX9LockableTexture::DestroySystemTexture()
{
	if (m_pSysTexture)
	{
		m_pSysTexture = 0;
	}
}

ASCBoolean CASCDX9LockableTexture::CreateVideoTexture()
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

void CASCDX9LockableTexture::DestroyVideoTexture()
{
	if (m_pVideoTexture)
	{
		m_pVideoTexture = 0;
	}
}

ASCBoolean CASCDX9LockableTexture::CopySysTextureToVideo()
{
	if (!(G_pD3D9Device && m_pSysTexture && m_pVideoTexture))
	{
		return false;
	}
	return (SUCCEEDED(G_pD3D9Device->UpdateTexture(m_pSysTexture, m_pVideoTexture)));
}

const ASCInt cASCTextureDepthStencilLvDX9 = 2;

CASCDX9RenderTargetTexture::CASCDX9RenderTargetTexture() : CASCRenderTargetTexture()
{
	m_pTexture = 0;
	m_pDepthBuffer = 0;
	m_pSavedBackBuffer = 0;
	m_pSavedDepthBuffer = 0;
	m_DepthStencilFormat = D3DFMT_UNKNOWN;
}

void CASCDX9RenderTargetTexture::Bind(ASCInt nStage)
{
	if (G_pD3D9Device && m_pTexture)
	{
		G_pD3D9Device->SetTexture(nStage, m_pTexture);
	}
}

void CASCDX9RenderTargetTexture::HandleDeviceReset()
{
	if (G_D3D9Mode != admDirect3D9Ex)
	{
		CreateTextureInstance();
	}
}

void CASCDX9RenderTargetTexture::HandleDeviceLost()
{
	if (G_D3D9Mode != admDirect3D9Ex)
	{
		DestroyTextureInstance();
	}
}

void CASCDX9RenderTargetTexture::UpdateMipmaps()
{
	if (m_pTexture)
	{
		m_pTexture->GenerateMipSubLevels();
	}
}

ASCBoolean CASCDX9RenderTargetTexture::BeginRenderTo()
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
		G_nDX9ActiveDepthStencilLevel = cASCTextureDepthStencilLvDX9;
	}
	else
	{
		G_nDX9ActiveDepthStencilLevel = 0;
	}

	return true;
}

void CASCDX9RenderTargetTexture::EndRenderTo()
{
	RestoreRenderBuffers();
}

void CASCDX9RenderTargetTexture::UpdateSize()
{
	DestroyTextureInstance();
	CreateTextureInstance();
}

ASCBoolean CASCDX9RenderTargetTexture::CreateTexture()
{
	return CreateTextureInstance();
}

void CASCDX9RenderTargetTexture::DestroyTexture()
{
	DestroyTextureInstance();
}

ASCBoolean CASCDX9RenderTargetTexture::CreateTextureInstance()
{
	if (!G_pD3D9Device)
	{
		return false;
	}

	ASCInt nLevels = 1;
	ASCUInt uUsage = D3DUSAGE_RENDERTARGET;
	if (GetMipMapping())
	{
		nLevels = 0;
		uUsage = uUsage | D3DUSAGE_AUTOGENMIPMAP;
	};

	m_PixelFormat = DX9FindTextureFormat(m_PixelFormat, uUsage);
	if (m_PixelFormat == apfUnknown)
	{
		return false;
	}

	if (GetDepthStencil())
	{
		m_DepthStencilFormat = DX9FindDepthStencilFormat(cASCTextureDepthStencilLvDX9);
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

void CASCDX9RenderTargetTexture::DestroyTextureInstance()
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

ASCBoolean CASCDX9RenderTargetTexture::SaveRenderBuffers()
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

void CASCDX9RenderTargetTexture::RestoreRenderBuffers()
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

ASCBoolean CASCDX9RenderTargetTexture::SetRenderBuffers()
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

void CASCDX9RenderTargetTexture::SetDefaultViewport()
{
	if (!G_pD3D9Device)
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
