/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCBitmaps.h
 *  Content: ASC(Asphyre Sphinx for C++) bitmap handler
 *  Date   : 2013/08/28
 *  Desc   : Handler for loading different bitmap types
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <string>
using std::wstring;
#include <vector>
using std::vector;
#include "ASCTypes.h"
#include "ASCStreams.h"
#include "ASCSurfaces.h"

class CASCCustomBitmap
{
public:
	CASCCustomBitmap();

	virtual ASCBoolean LoadFromStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pDest) = 0;
	virtual ASCBoolean SaveToStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pSource) = 0;

	wstring GetDesc();
protected:
	wstring	m_sDesc;
};

struct CASCBitmapAssociation
{
	wstring				m_sExtension;
	CASCCustomBitmap*	m_pHandler;
};

class CASCBitmapManager
{
public:
	CASCBitmapManager();
	~CASCBitmapManager();

	ASCBoolean RegisterExt(const wstring& sExtension, CASCCustomBitmap* pHandler);
	void UnregisterExt(const wstring& sExtension);

	CASCCustomBitmap* AssociatedHandler(const wstring& sExtension);

	ASCBoolean LoadFromStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pDest);
	ASCBoolean SaveToStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pSource);

	ASCBoolean LoadFromFile(const wstring& sFileName, CASCSystemSurface* pDest);
	ASCBoolean SaveToFile(const wstring& sFileName, CASCSystemSurface* pSource);
private:
	vector<CASCBitmapAssociation>	m_Associations;

	ASCInt FindExtension(const wstring& sExtension);
	void RemoveAssociation(ASCInt nIndex);
};

extern inline CASCBitmapManager* ASCBitmapManager();