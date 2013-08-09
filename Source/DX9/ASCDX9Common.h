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
#include "../ASCPixelFormats.h"

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

void ClearD3D9DisplayMode();
void ClearD3D9PresentParams();
void ClearD3D9Caps();

CASCPixelFormat DX9PixelFormatToASC(D3DFORMAT fmt);

D3DFORMAT ASCPixelFormatToDX9(CASCPixelFormat fmt);

D3DFORMAT DX9FindBackBufferFormat(CASCPixelFormat fmt);

D3DFORMAT DX9FindDepthStencilFormat(ASCInt nStencilLevel);

void DX9FindBestMultisampleType(D3DFORMAT BackBufferFmt, D3DFORMAT DepthFmt, ASCInt nMultisamples, 
								D3DMULTISAMPLE_TYPE* pSampleType, ASCUInt32* pQualityLv);

CASCPixelFormat DX9FindTextureFormat(CASCPixelFormat fmt, ASCUInt32 uUsage);
CASCPixelFormat DX9FindTextureFormatEx(CASCPixelFormat fmt, ASCUInt32 uUsage, ASCUInt32 uUsage2);