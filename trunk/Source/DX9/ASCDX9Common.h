/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCDX9Common.h
 *  Content: ASC(Asphyre Sphinx for C++) Direct3D9 general type definitions and utilities
 *  Date   : 2013/07/26
 *  Desc   : General type definitions and utilities of Direct3D9 for ASC
 *
 *******************************************************************************************/

#pragma once

#include <d3d9.h>
#include <d3d9types.h>
#include <d3d9caps.h>
#include "../ASCTypes.h"

enum CASCDirect3D9Mode
{
	admUnknown, admDirect3D9, admDirect3D9Ex
};

const ASCInt ASC_BACKBUFFERFORMAT_NUM = 5;
const CASCPixelFormat C_ASCBackBufferFormats[ASC_BACKBUFFERFORMAT_NUM] =
{
	apfA8R8G8B8, apfX8R8G8B8, apfA1R5G5B5, apfX1R5G5B5, apfR5G6B5
};

const ASCInt ASC_DEPTHSTENCILFORMAT_NUM = 6;
const D3DFORMAT C_ASCDepthStencilFormats[ASC_DEPTHSTENCILFORMAT_NUM] =
{
	D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D15S1, D3DFMT_D32, D3DFMT_D24X8, D3DFMT_D16
};

/*
 * The following variables are declared as globals. 
 * The global declaration, although generally avoided, 
 * in this case decreases the coupling of DX9 provider classes
*/
extern CASCDirect3D9Mode		G_D3D9Mode;
extern D3DDISPLAYMODEEX			G_D3D9DisplayMode;
extern D3DPRESENT_PARAMETERS	G_D3D9PresentParams;
extern IDirect3D9*				G_pD3D9;
extern IDirect3DDevice9*		G_pD3D9Device;
extern D3DCAPS9					G_D3DCaps9;
extern ASCInt					G_nDX9ActiveDepthStencilLevel;

CASCPixelFormat DX9PixelFormatToASC(D3DFORMAT fmt)
{
	switch (fmt)
	{
	case D3DFMT_R8G8B8:
		return apfR8G8B8;
	case D3DFMT_A8R8G8B8:
		return apfA8R8G8B8;
	case D3DFMT_X8R8G8B8:
		return apfX8R8G8B8;
	case D3DFMT_R5G6B5:
		return apfR5G6B5;
	case D3DFMT_A1R5G5B5:
		return apfA1R5G5B5;
	case D3DFMT_X1R5G5B5:
		return apfX1R5G5B5;
	case D3DFMT_A4R4G4B4:
		return apfA4R4G4B4;
	case D3DFMT_X4R4G4B4:
		return apfX4R4G4B4;
	default:
		return apfUnknown;
	}
}

D3DFORMAT ASCPixelFormatToDX9(CASCPixelFormat fmt)
{
	switch (fmt)
	{
	case apfR8G8B8:
		return D3DFMT_R8G8B8;
	case apfA8R8G8B8:
		return D3DFMT_A8R8G8B8;
	case apfX8R8G8B8:
		return D3DFMT_X8R8G8B8;
	case apfR5G6B5:
		return D3DFMT_R5G6B5;
	case apfA1R5G5B5:
		return D3DFMT_A1R5G5B5;
	case apfX1R5G5B5:
		return D3DFMT_X1R5G5B5;
	case apfA4R4G4B4:
		return D3DFMT_A4R4G4B4;
	case apfX4R4G4B4:
		return D3DFMT_X4R4G4B4;
	default:
		return D3DFMT_UNKNOWN;
	}
}

D3DFORMAT DX9FindBackBufferFormat(CASCPixelFormat fmt)
{
	if (!G_pD3D9)
	{
		return D3DFMT_UNKNOWN;
	}

	if (fmt == apfUnknown)
	{
		fmt = apfA8R8G8B8;
	}

	D3DFORMAT ModeFmt = G_D3D9DisplayMode.Format;
	if (ModeFmt == D3DFMT_UNKNOWN)
	{
		ModeFmt = D3DFMT_X8R8G8B8;
	}

	CASCPixelFormatList*	pFmtLst	= new CASCPixelFormatList();
	CASCPixelFormat			Sample;
	D3DFORMAT				TestFmt;
	for (ASCInt i = 0; i < ASC_BACKBUFFERFORMAT_NUM; i++)
	{
		Sample	= C_ASCBackBufferFormats[i];
		TestFmt	= ASCPixelFormatToDX9(Sample);
		if (TestFmt ==D3DFMT_UNKNOWN)
		{
			continue;
		}
		if (!SUCCEEDED(G_pD3D9->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, ModeFmt, TestFmt, true)))
		{
			continue;
		}
		pFmtLst.Insert(Sample);
	}

	D3DFORMAT Result = ASCPixelFormatToDX9(FindClosestFormat(fmt, pFmtLst));
	delete pFmtLst;
	return Result;
}

D3DFORMAT DX9FindDepthStencilFormat(ASCInt nStencilLevel)
{
	const ASCInt C_FormatIndexes[2, 6] =
	{
		(3, 0, 1, 4, 5, 2),
		(0, 1, 2, 3, 4, 5)
	};

	if ((!G_pD3D9) || (nStencilLevel < 1) || (nStencilLevel > 2))
	{
		return D3DFMT_UNKNOWN;
	}

	D3DFORMAT DisplayFmt =G_D3D9DisplayMode.Format;
	if (DisplayFmt = D3DFMT_UNKNOWN)
	{
		DisplayFmt =D3DFMT_X8R8G8B8;
	}

	D3DFORMAT fmt;
	for (ASCInt i = 0; i <= 5; i++)
	{
		fmt = C_ASCDepthStencilFormats[C_FormatIndexes[nStencilLevel - 1][i]];
		if (SUCCEEDED(G_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayFmt, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, fmt)))
		{
			return fmt;
		}
	}

	return D3DFMT_UNKNOWN;
}

void DX9FindBestMultisampleType(D3DFORMAT BackBufferFmt, D3DFORMAT DepthFmt, ASCInt nMultisamples, 
								D3DMULTISAMPLE_TYPE* pSampleType, ASCUInt32* pQualityLv)
{
	*pSampleType = D3DMULTISAMPLE_NONE;
	pQualityLv = 0;
	if ((!G_pD3D9) || (nMultisamples < 2))
	{
		return;
	}

	D3DMULTISAMPLE_TYPE	TestSample;
	ASCUInt32			uQuaLevels;
	ASCBoolean			bSuccess;
	for (ASCInt i = nMultisamples; i >= 2; i--)
	{
		TestSample = D3DMULTISAMPLE_TYPE(i);

		bSuccess = SUCCEEDED(G_pD3D9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, BackBufferFmt, true, TestSample, &uQuaLevels));
		if (bSuccess && (DepthFmt != D3DFMT_UNKNOWN))
		{
			bSuccess = SUCCEEDED(G_pD3D9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DepthFmt, true, TestSample, 0));
		}

		if (bSuccess)
		{
			*pSampleType = TestSample;
			*pQualityLv = uQuaLevels - 1;
			break;
		}
	}
}

CASCPixelFormat DX9FindTextureFormat(CASCPixelFormat fmt, ASCUInt32 uUsage)
{
	if (!G_pD3D9)
	{
		return apfUnknown;
	}

	D3DFORMAT DisplayFmt = G_D3D9DisplayMode.Format;
	if (DisplayFmt = D3DFMT_UNKNOWN)
	{
		DisplayFmt = D3DFMT_X8R8G8B8;
	}

	CASCPixelFormatList *pFmtLst = new CASCPixelFormatList();
	D3DFORMAT			TestFormat;
	for (CASCPixelFormat Sample = apfUnknown + 1; Sample <= apfX4R4G4B4; Sample++)
	{
		TestFormat =ASCPixelFormatToDX9(Sample);
		if (TestFormat = D3DFMT_UNKNOWN)
		{
			continue;
		}

		if (SUCCEEDED(G_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayFmt, uUsage, D3DRTYPE_TEXTURE, TestFormat)))
		{
			pFmtLst->Insert(Sample);
		}
	}

	CASCPixelFormat Result = FindClosestFormat(fmt, pFmtLst);
	delete pFmtLst;
	return Result;
}

CASCPixelFormat DX9FindTextureFormatEx(CASCPixelFormat fmt, ASCUInt32 uUsage, ASCUInt32 uUsage2)
{
	if (!G_pD3D9)
	{
		return apfUnknown;
	}

	D3DFORMAT DisplayFmt = G_D3D9DisplayMode.Format;
	if (DisplayFmt = D3DFMT_UNKNOWN)
	{
		DisplayFmt = D3DFMT_X8R8G8B8;
	}

	CASCPixelFormatList *pFmtLst = new CASCPixelFormatList();
	D3DFORMAT			TestFormat;
	for (CASCPixelFormat Sample = apfUnknown + 1; Sample <= apfX4R4G4B4; Sample++)
	{
		TestFormat =ASCPixelFormatToDX9(Sample);
		if (TestFormat = D3DFMT_UNKNOWN)
		{
			continue;
		}

		if (FAILED(G_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayFmt, uUsage, D3DRTYPE_TEXTURE, TestFormat)))
		{
			continue;
		}

		if (FAILED(G_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayFmt, uUsage2, D3DRTYPE_TEXTURE, TestFormat)))
		{
			continue;
		}

		pFmtLst->Insert(Sample);
	}

	CASCPixelFormat Result = FindClosestFormat(fmt, pFmtLst);
	delete pFmtLst;
	return Result;
}