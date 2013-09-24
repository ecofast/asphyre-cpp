#include "ASCBitmaps.h"
#include "ASCUtils.h"

CASCCustomBitmap::CASCCustomBitmap()
{
	m_sDesc = L"Unknown Bitmap";
}

wstring CASCCustomBitmap::GetDesc()
{
	return m_sDesc;
}

CASCBitmapManager* ASCBitmapManager()
{
	static CASCBitmapManager gASCBitmapManager;
	return &gASCBitmapManager;
}

CASCBitmapManager::CASCBitmapManager()
{

}

CASCBitmapManager::~CASCBitmapManager()
{

}

ASCInt CASCBitmapManager::FindExtension(const wstring& sExtension)
{
	for (ASCInt i = 0; i < (ASCInt)m_Associations.size(); i++)
	{
		if (sExtension == m_Associations[i].m_sExtension)
		{
			return i;
		}
	}
	return -1;
}

void CASCBitmapManager::RemoveAssociation(ASCInt nIndex)
{
	m_Associations.erase(m_Associations.begin() + nIndex);
}

ASCBoolean CASCBitmapManager::RegisterExt(const wstring& sExtension, CASCCustomBitmap* pHandler)
{
	ASCInt nIndex = FindExtension(sExtension);
	if (nIndex != -1)
	{
		return false;
	}

	nIndex = m_Associations.size();
	m_Associations.resize(nIndex + 1);
	m_Associations[nIndex].m_pHandler = pHandler;
	m_Associations[nIndex].m_sExtension = sExtension;
	return true;
}

void CASCBitmapManager::UnregisterExt(const wstring& sExtension)
{
	ASCInt nIndex = FindExtension(sExtension);
	if (nIndex != -1)
	{
		RemoveAssociation(nIndex);
	}
}

CASCCustomBitmap* CASCBitmapManager::AssociatedHandler(const wstring& sExtension)
{
	ASCInt nIndex = FindExtension(sExtension);
	if (nIndex == -1)
	{
		return 0;
	} 
	else
	{
		return m_Associations[nIndex].m_pHandler;
	}
}

ASCBoolean CASCBitmapManager::LoadFromStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pDest)
{
	CASCCustomBitmap* pHandler = AssociatedHandler(sExtension);
	if (pHandler)
	{
		return (pHandler->LoadFromStream(sExtension, pStream, pDest));
	} 
	else
	{
		return false;
	}
}

ASCBoolean CASCBitmapManager::SaveToStream(const wstring& sExtension, CASCStream* pStream, CASCSystemSurface* pSource)
{
	CASCCustomBitmap* pHandler = AssociatedHandler(sExtension);
	if (pHandler)
	{
		return (pHandler->SaveToStream(sExtension, pStream, pSource));
	} 
	else
	{
		return false;
	}
}

ASCBoolean CASCBitmapManager::LoadFromFile(const wstring& sFileName, CASCSystemSurface* pDest)
{
	CASCCustomBitmap* pHandler = AssociatedHandler(ExtractFileExt(sFileName));
	if (!pHandler)
	{
		return false;
	};

	CASCFileStream* pFileStream = new CASCFileStream();
	if (!(pFileStream->Open(sFileName)))
	{
		delete pFileStream;
		pFileStream = 0;

		return false;
	}

	ASCBoolean bResult = pHandler->LoadFromStream(ExtractFileExt(sFileName), pFileStream, pDest);
	delete pFileStream;
	pFileStream = 0;

	return bResult;
}

ASCBoolean CASCBitmapManager::SaveToFile(const wstring& sFileName, CASCSystemSurface* pSource)
{
	CASCCustomBitmap* pHandler = AssociatedHandler(ExtractFileExt(sFileName));
	if (!pHandler)
	{
		return false;
	};

	CASCFileStream* pFileStream = new CASCFileStream();
	if (!(pFileStream->Create(sFileName)))
	{
		delete pFileStream;
		pFileStream = 0;

		return false;
	}

	ASCBoolean bResult = pHandler->SaveToStream(ExtractFileExt(sFileName), pFileStream, pSource);

	delete pFileStream;
	pFileStream = 0;
	return bResult;
}