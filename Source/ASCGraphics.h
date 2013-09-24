/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCGraphics.h
 *  Content: ASC(Asphyre Sphinx for C++) Graphic utils
 *  Date   : 2013/08/29
 *  Desc   : Graphic utils for ASC
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCTypes.h"

typedef ASCUInt32	ASCColor32;
typedef ASCUInt16	ASCColor16;
typedef ASCUInt8	ASCColor8;

#pragma pack(push, 1)
struct ASCColor24 
{
	ASCByte	b;
	ASCByte	g;
	ASCByte	r;
};
#pragma pack(pop)

/*
 * 常用颜色
*/
const ASCColor32 cASCColor32Black		= 0xFF000000;
const ASCColor32 cASCColor32Maroom		= 0xFF800000;
const ASCColor32 cASCColor32Green		= 0xFF008000;
const ASCColor32 cASCColor32Olive		= 0xFF808000;
const ASCColor32 cASCColor32Navy		= 0xFF000080;
const ASCColor32 cASCColor32Purple		= 0xFF800080;
const ASCColor32 cASCColor32Teal		= 0xFF008080;
const ASCColor32 cASCColor32Gray		= 0xFF808080;
const ASCColor32 cASCColor32Silver		= 0xFFC0C0C0;
const ASCColor32 cASCColor32Red			= 0xFFFF0000;
const ASCColor32 cASCColor32Lime		= 0xFF00FF00;
const ASCColor32 cASCColor32Yellow		= 0xFFFFFF00;
const ASCColor32 cASCColor32Blue		= 0xFF0000FF;
const ASCColor32 cASCColor32Fuchsia		= 0xFFFF00FF;
const ASCColor32 cASCColor32Aqua		= 0xFF00FFFF;
const ASCColor32 cASCColor32Moneygreen	= 0xFFC0DCC0;
const ASCColor32 cASCColor32Skyblue		= 0xFFA6CAF0;
const ASCColor32 cASCColor32Cream		= 0xFFFFFBF0;
const ASCColor32 cASCColor32Medgray		= 0xFFA0A0A4;
const ASCColor32 cASCColor32White		= 0xFFFFFFFF;

// 合成 32bit 颜色
#define Color32FromARGB(a, r, g, b)		((ASCColor32)((((a)&0xFF)<<24) | (((r)&0xFF)<<16) | (((g)&0xFF)<<8) | ((b)&0xFF)))
#define Color32FromXRGB(r, g, b)		Color32FromARGB(0xFF, r, g, b)

// 取 32bit 颜色的各通道值
#define AFromColor32(color)				((ASCByte)((color)>>24))
#define RFromColor32(color)				((ASCByte)(((color)&0x00FF0000)>>16))
#define GFromColor32(color)				((ASCByte)(((color)&0x0000FF00)>>8))
#define BFromColor32(color)				((ASCByte)((color)&0x000000FF))

// 合成颜色 4444, argb 是 0~15 的值
#define Color4444FromARGB(a, r, g, b)	((ASCColor16)((((a)&0xF)<<12) | (((r)&0xF)<<8) | (((g)&0xF)<<4) | (((b)&0xF)<<0)))
#define Color4444FromXRGB(r, g, b)		Color4444FromARGB(0xF, r, g, b)

// 取 4444 颜色的各通道值, argb 是 0~15 的值
#define AFromColor4444(color)			((ASCByte)(((color)&0xF000)>>12))
#define RFromColor4444(color)			((ASCByte)(((color)&0x0F00)>>8))
#define GFromColor4444(color)			((ASCByte)(((color)&0x00F0)>>4))
#define BFromColor4444(color)			((ASCByte)(((color)&0x000F)>>0))

// 合成颜色 565, rgb 的值是 0~31, 0~63, 0~31
#define Color565FromRGB(r, g, b)		((ASCColor16)((((r)&0x1F)<<11) | (((g)&0x3F)<<5) | (((b)&0x1F)<<0)))

// 取 565 颜色的各通道值, rgb 的值是 0~31, 0~63, 0~31
#define RFromColor565(color)			((ASCByte)(((color)&0xF800)>>11))
#define GFromColor565(color)			((ASCByte)(((color)&0x07E0)>>5))
#define BFromColor565(color)			((ASCByte)(((color)&0x001F)>>0))

// 合成颜色 1555, argb 分别是 0~1, 0~31, 0~31, 0~31
#define Color1555FromARGB(a, r, g, b)	((ASCColor16)((((a)&0x1)<<15) | (((r)&0x1F)<<10) | (((g)&0x1F)<<5) | (((b)&0x1F)<<0)))
#define Color1555FromXRGB(r, g, b)		Color1555FromARGB(1, r, g, b)

// 取 1555 颜色的各通道值, argb 分别是 0~1，0~31，0~31，0~31
#define AFromColor1555(color)			((ASCByte)((color)>>15))
#define RFromColor1555(color)			((ASCByte)(((color)&0x7C00)>>10))
#define GFromColor1555(color)			((ASCByte)(((color)&0x03E0)>>5))
#define BFromColor1555(color)			((ASCByte)(((color)&0x001F)>>0))

/*
 * 以下为高精度颜色格式转换函数
*/

inline ASCByte Scale8To4(ASCByte c)
{
	static ASCByte sTable[256] = {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,
		11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,
		12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,
		14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15};
	return sTable[c];
}

inline ASCByte Scale8To5(ASCByte c)
{
	static ASCByte sTable[256] = {0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,
		3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,7,7,7,7,7,7,
		7,7,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,11,11,11,
		11,11,11,11,11,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,14,14,
		14,14,14,14,14,14,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,17,17,
		17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,20,
		20,20,20,20,20,20,20,21,21,21,21,21,21,21,21,22,22,22,22,22,22,22,22,22,
		23,23,23,23,23,23,23,23,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,
		26,26,26,26,26,26,26,26,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,
		28,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,31,31,31,31,31};
	return sTable[c];
}

inline ASCByte Scale8To6(ASCByte c)
{
	static ASCByte sTable[256] = {0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,
		6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12,
		13,13,13,13,14,14,14,14,15,15,15,15,16,16,16,16,17,17,17,17,18,18,18,
		18,19,19,19,19,20,20,20,20,21,21,21,21,21,22,22,22,22,23,23,23,23,24,
		24,24,24,25,25,25,25,26,26,26,26,27,27,27,27,28,28,28,28,29,29,29,29,
		30,30,30,30,31,31,31,31,32,32,32,32,33,33,33,33,34,34,34,34,35,35,35,
		35,36,36,36,36,37,37,37,37,38,38,38,38,39,39,39,39,40,40,40,40,41,41,
		41,41,42,42,42,42,42,43,43,43,43,44,44,44,44,45,45,45,45,46,46,46,46,
		47,47,47,47,48,48,48,48,49,49,49,49,50,50,50,50,51,51,51,51,52,52,52,
		52,53,53,53,53,54,54,54,54,55,55,55,55,56,56,56,56,57,57,57,57,58,58,
		58,58,59,59,59,59,60,60,60,60,61,61,61,61,62,62,62,62,63,63,63};
	return sTable[c];
}

inline ASCByte Scale4To5(ASCByte c)
{
	static ASCByte sTable[16] = {0,2,4,6,8,10,12,14,17,19,21,23,25,27,29,31};
	return sTable[c];
}

inline ASCByte Scale4To6(ASCByte c)
{
	static ASCByte sTable[16] = {0,4,8,13,17,21,25,29,34,38,42,46,50,55,59,63};
	return sTable[c];
}

inline ASCByte Scale4To8(ASCByte c)
{
	static ASCByte sTable[16] = {0,17,34,51,68,85,102,119,136,153,170,187,204,221,238,255};
	return sTable[c];
}

inline ASCByte Scale5To4(ASCByte c)
{
	static ASCByte sTable[32] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,
		11,11,12,12,13,13,14,14,15,15};
	return sTable[c];
}

inline ASCByte Scale5To6(ASCByte c)
{
	static ASCByte sTable[32] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,33,35,
		37,39,41,43,45,47,49,51,53,55,57,59,61,63};
	return sTable[c];
}

inline ASCByte Scale5To8(ASCByte c)
{
	static ASCByte sTable[32] = {0,8,16,25,33,41,49,58,66,74,82,90,99,107,115,123,132,140,
		148,156,165,173,181,189,197,206,214,222,230,239,247,255};
	return sTable[c];
}

inline ASCByte Scale6To4(ASCByte c)
{
	static ASCByte sTable[64] = {0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,5,6,6,6,6,
		7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13,14,
		14,14,14,15,15,15};
	return sTable[c];
}

inline ASCByte Scale6To5(ASCByte c)
{
	static ASCByte sTable[64] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,
		13,13,14,14,15,15,16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,25,25,26,
		26,27,27,28,28,29,29,30,30,31,31};
	return sTable[c];
}

inline ASCByte Scale6To8(ASCByte c)
{
	static ASCByte sTable[64] = {0,4,8,12,16,20,24,28,32,36,40,45,49,53,57,61,65,69,73,77,
		81,85,89,93,97,101,105,109,113,117,121,125,130,134,138,142,146,150,154,158,162,
		166,170,174,178,182,186,190,194,198,202,206,210,215,219,223,227,231,235,239,243,
		247,251,255};
	return sTable[c];
}

inline void LineScale4444To4444(void* pDest, void* pSrc, ASCInt nCount)
{
	memcpy(pDest, pSrc, nCount * 2);
}

inline ASCColor16 ColorScale4444To1555(ASCColor16 color)
{
	ASCByte a = (color & 0xF000) > 0x8000 ? 1 : 0;
	ASCByte r = Scale4To5(RFromColor4444(color));
	ASCByte g = Scale4To5(GFromColor4444(color));
	ASCByte b = Scale4To5(BFromColor4444(color));
	return Color1555FromARGB(a, r, g, b);
}

inline void LineScale4444To1555(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale4444To1555(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}
}

inline ASCColor16 ColorScale4444To565(ASCColor16 color)
{
	ASCByte r = Scale4To5(RFromColor4444(color));
	ASCByte g = Scale4To6(GFromColor4444(color));
	ASCByte b = Scale4To5(BFromColor4444(color));
	return Color565FromRGB(r, g, b);
}

inline void LineScale4444To565(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale4444To565(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}
}

inline ASCColor24 ColorScale4444To888(ASCColor16 color)
{
	ASCColor24 c24;
	c24.r = Scale4To8(RFromColor4444(color));
	c24.g = Scale4To8(GFromColor4444(color));
	c24.b = Scale4To8(BFromColor4444(color));
	return c24;
}

inline void LineScale4444To888(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor24* pDestColor = (ASCColor24*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale4444To888(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}    
}

inline ASCColor32 ColorScale4444To8888(ASCColor16 color)
{
	ASCByte a = Scale4To8(AFromColor4444(color));
	ASCByte r = Scale4To8(RFromColor4444(color));
	ASCByte g = Scale4To8(GFromColor4444(color));
	ASCByte b = Scale4To8(BFromColor4444(color));
	return Color32FromARGB(a, r, g, b);
}

inline void LineScale4444To8888(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor32* pDestColor = (ASCColor32*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale4444To8888(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor16 ColorScale1555To4444(ASCColor16 color)
{
	ASCByte a = (color & 0x8000) ? 0xF : 0;
	ASCByte r = Scale5To4(RFromColor1555(color));
	ASCByte g = Scale5To4(GFromColor1555(color));
	ASCByte b = Scale5To4(BFromColor1555(color));
	return Color4444FromARGB(a, r, g, b);
}

inline void LineScale1555To4444(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale1555To4444(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline void LineScale1555To1555(void* pDest, void* pSrc, ASCInt nCount)
{
	memcpy(pDest, pSrc, nCount * 2);    
}

inline ASCColor16 ColorScale1555To565(ASCColor16 color)
{
	ASCByte r = RFromColor1555(color);
	ASCByte g = Scale5To6(GFromColor1555(color));
	ASCByte b = BFromColor1555(color);
	return Color565FromRGB(r, g, b);
}

inline void LineScale1555To565(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale1555To565(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}
}

inline ASCColor24 ColorScale1555To888(ASCColor16 color)
{
	ASCColor24 c24;
	c24.r = Scale5To8(RFromColor1555(color));
	c24.g = Scale5To8(GFromColor1555(color));
	c24.b = Scale5To8(BFromColor1555(color));
	return c24;
}

inline void LineScale1555To888(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor24* pDestColor = (ASCColor24*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale1555To888(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor32 ColorScale1555To8888(ASCColor16 color)
{
	ASCByte a = (color & 0x8000) ? 0xFF : 0;
	ASCByte r = Scale5To8(RFromColor1555(color));
	ASCByte g = Scale5To8(GFromColor1555(color));
	ASCByte b = Scale5To8(BFromColor1555(color));
	return Color32FromARGB(a, r, g, b);
}

inline void LineScale1555To8888(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor32* pDestColor = (ASCColor32*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale1555To8888(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor16 ColorScale565To4444(ASCColor16 color)
{
	ASCByte a = 0xF;
	ASCByte r = Scale5To4(RFromColor565(color));
	ASCByte g = Scale6To4(GFromColor565(color));
	ASCByte b = Scale5To4(BFromColor565(color));
	return Color4444FromARGB(a, r, g, b);
}

inline void LineScale565To4444(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale565To4444(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}
}

inline ASCColor16 ColorScale565To1555(ASCColor16 color)
{
	ASCByte a = 0x1;
	ASCByte r = RFromColor565(color);
	ASCByte g = Scale6To5(GFromColor565(color));
	ASCByte b = BFromColor565(color);
	return Color1555FromARGB(a, r, g, b);
}

inline void LineScale565To1555(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale565To1555(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}
}

inline void LineScale565To565(void* pDest, void* pSrc, ASCInt nCount)
{
	memcpy(pDest, pSrc, nCount * 2);
}

inline ASCColor24 ColorScale565To888(ASCColor16 color)
{
	ASCColor24 c24;
	c24.r = Scale5To8(RFromColor565(color));
	c24.g = Scale6To8(GFromColor565(color));
	c24.b = Scale5To8(BFromColor565(color));
	return c24;
}

inline void LineScale565To888(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor24* pDestColor = (ASCColor24*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale565To888(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor32 ColorScale565To8888(ASCColor16 color)
{
	ASCByte a = 0xFF;
	ASCByte r = Scale5To8(RFromColor565(color));
	ASCByte g = Scale6To8(GFromColor565(color));
	ASCByte b = Scale5To8(BFromColor565(color));
	return Color32FromARGB(a, r, g, b);
}

inline void LineScale565To8888(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor16* pSrcColor = (ASCColor16*)pSrc;
	ASCColor32* pDestColor = (ASCColor32*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale565To8888(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	} 
}

inline ASCColor16 ColorScale888To4444(ASCColor24 color)
{
	ASCByte a = 0xF;
	ASCByte r = Scale8To4(color.r);
	ASCByte g = Scale8To4(color.g);
	ASCByte b = Scale8To4(color.b);
	return Color4444FromARGB(a, r, g, b);
}

inline void LineScale888To4444(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor24* pSrcColor = (ASCColor24*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale888To4444(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor16 ColorScale888To1555(ASCColor24 color)
{
	ASCByte a = 0x1;
	ASCByte r = Scale8To5(color.r);
	ASCByte g = Scale8To5(color.g);
	ASCByte b = Scale8To5(color.b);
	return Color1555FromARGB(a, r, g, b);
}

inline void LineScale888To1555(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor24* pSrcColor = (ASCColor24*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale888To1555(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor16 ColorScale888To565(ASCColor24 color)
{
	ASCByte r = Scale8To5(color.r);
	ASCByte g = Scale8To6(color.g);
	ASCByte b = Scale8To5(color.b);
	return Color565FromRGB(r, g, b);
}

inline void LineScale888To565(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor24* pSrcColor = (ASCColor24*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale888To565(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline void LineScale888To888(void* pDest, void* pSrc, ASCInt nCount)
{
	memcpy(pDest, pSrc, nCount * 3);
}

inline ASCColor32 ColorScale888To8888(ASCColor24 color)
{
	return Color32FromARGB(0xFF, color.r, color.g, color.b);
}

inline void LineScale888To8888(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor24* pSrcColor = (ASCColor24*)pSrc;
	ASCColor32* pDestColor = (ASCColor32*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale888To8888(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor16 ColorScale8888To4444(ASCColor32 color)
{
	ASCByte a = Scale8To4(AFromColor32(color));
	ASCByte r = Scale8To4(RFromColor32(color));
	ASCByte g = Scale8To4(GFromColor32(color));
	ASCByte b = Scale8To4(BFromColor32(color));
	return Color4444FromARGB(a, r, g, b);
}

inline void LineScale8888To4444(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor32* pSrcColor = (ASCColor32*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale8888To4444(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor16 ColorScale8888To1555(ASCColor32 color)
{
	ASCByte a = (color & 0xff000000) > 0x80000000 ? 1 : 0;
	ASCByte r = Scale8To5(RFromColor32(color));
	ASCByte g = Scale8To5(GFromColor32(color));
	ASCByte b = Scale8To5(BFromColor32(color));
	return Color1555FromARGB(a, r, g, b);
}

inline void LineScale8888To1555(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor32* pSrcColor = (ASCColor32*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale8888To1555(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor16 ColorScale8888To565(ASCColor32 color)
{
	ASCByte r = Scale8To5(RFromColor32(color));
	ASCByte g = Scale8To6(GFromColor32(color));
	ASCByte b = Scale8To5(BFromColor32(color));
	return Color565FromRGB(r, g, b);
}

inline void LineScale8888To565(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor32* pSrcColor = (ASCColor32*)pSrc;
	ASCColor16* pDestColor = (ASCColor16*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale8888To565(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline ASCColor24 ColorScale8888To888(ASCColor32 color)
{
	ASCColor24 c24;
	c24.r = RFromColor32(color);
	c24.g = GFromColor32(color);
	c24.b = BFromColor32(color);
	return c24;
}

inline void LineScale8888To888(void* pDest, void* pSrc, ASCInt nCount)
{
	ASCColor32* pSrcColor = (ASCColor32*)pSrc;
	ASCColor24* pDestColor = (ASCColor24*)pDest;
	while (nCount--)
	{
		*pDestColor = ColorScale8888To888(*pSrcColor);
		++pDestColor;
		++pSrcColor;
	}  
}

inline void LineScale8888To8888(void* pDest, void* pSrc, ASCInt nCount)
{
	memcpy(pDest, pSrc, nCount * 4);
}

// 颜色行转换回调函数
typedef void (* pfnProcLineScale)(void* pDest, void* pSrc, ASCInt nSrcCount);

// 取颜色行转换的回调函数
inline pfnProcLineScale GetLineScaleProc(CASCPixelFormat SrcFmt, CASCPixelFormat DestFmt)
{
	// 8 * 8 --> 888, 8888, X8888, 565, 1555, X1555, 4444, X4444
	static pfnProcLineScale LineScaleProcs[cASCPixelFormatElemNum - 1][cASCPixelFormatElemNum - 1] = 
	{
		{LineScale888To888, LineScale888To8888, LineScale888To888, LineScale888To565, LineScale888To1555, LineScale888To1555, LineScale888To4444, LineScale888To4444},
		{LineScale8888To888, LineScale8888To8888, LineScale8888To888, LineScale8888To565, LineScale8888To1555, LineScale8888To1555, LineScale8888To4444, LineScale8888To4444},
		{LineScale888To888, LineScale888To8888, LineScale888To888, LineScale888To565, LineScale888To1555, LineScale888To1555, LineScale888To4444, LineScale888To4444},
		{LineScale565To888, LineScale565To8888, LineScale565To888, LineScale565To565, LineScale565To1555, LineScale565To1555, LineScale565To4444, LineScale565To4444},
		{LineScale1555To888, LineScale1555To8888, LineScale1555To888, LineScale1555To565, LineScale1555To1555, LineScale1555To1555, LineScale1555To4444, LineScale1555To4444},
		{LineScale1555To888, LineScale1555To8888, LineScale1555To888, LineScale1555To565, LineScale1555To1555, LineScale1555To1555, LineScale1555To4444, LineScale1555To4444},
		{LineScale4444To888, LineScale4444To8888, LineScale4444To888, LineScale4444To565, LineScale4444To1555, LineScale4444To1555, LineScale4444To4444, LineScale4444To4444},
		{LineScale4444To888, LineScale4444To8888, LineScale4444To888, LineScale4444To565, LineScale4444To1555, LineScale4444To1555, LineScale4444To4444, LineScale4444To4444}
	};
	return LineScaleProcs[SrcFmt - 1][DestFmt - 1];
}