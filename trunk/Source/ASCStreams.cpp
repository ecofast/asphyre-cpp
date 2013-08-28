#include "ASCStreams.h"

ASCBoolean CASCStream::ReadBuffer(void* pBuffer, ASCInt nCount)
{
	return ((nCount != 0) && (Read(pBuffer, nCount) == nCount));
}

ASCBoolean CASCStream::WriteBuffer(const void* pBuffer, ASCInt nCount)
{
	return ((nCount != 0) && (Write(pBuffer, nCount) == nCount));
}

ASCInt CASCStream::CopyFrom(CASCStream* pSource, ASCInt nCount)
{
	const ASCInt cMaxBufSize = 0xF000;

	if (nCount == 0)
	{
		pSource->SetPosition(0);
		nCount = pSource->GetSize();
	}

	ASCInt nResult = nCount;
	ASCInt nBufSize, nNum;
	if (nCount > cMaxBufSize)
	{
		nBufSize = cMaxBufSize;
	} 
	else
	{
		nBufSize = nCount;
	}

	ASCChar* pBuffer = new ASCChar[nBufSize];
	while (nCount != 0)
	{
		if (nCount > nBufSize)
		{
			nNum = nBufSize;
		}
		else
		{
			nNum = nCount;
		}
		pSource->ReadBuffer(pBuffer, nNum);
		WriteBuffer(pBuffer, nNum);
		nCount -= nNum;
	}
	delete [] pBuffer;

	return nResult;
}

ASCInt CASCStream::GetPosition()
{
	return Seek(0, asoCurrent);
}

void CASCStream::SetPosition(const ASCInt nPos)
{
	Seek(nPos, asoBeginning);
}

ASCInt CASCStream::GetSize()
{
	ASCInt nPos = Seek(0, asoCurrent);
	ASCInt nResult = Seek(0, asoEnd);
	Seek(nPos, asoBeginning);

	return nResult;	
}

CASCFileStream::CASCFileStream() : CASCStream(), m_nHandle(0)
{

}

CASCFileStream::CASCFileStream(const CASCFileStream& Stream)
{

}

CASCFileStream::~CASCFileStream()
{
	if (m_nHandle > 0)
	{
		CloseHandle((HANDLE)m_nHandle);
	}
}

ASCBoolean CASCFileStream::Create(const wstring& sFileName, ASCUInt uDesiredAccess /*= GENERIC_READ | GENERIC_WRITE*/, ASCUInt uShareMode /*= FILE_SHARE_READ*/)
{
	m_nHandle = (ASCInt)CreateFileW(sFileName.c_str(), uDesiredAccess, uShareMode, 
		0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	return (m_nHandle > 0);
}

ASCBoolean CASCFileStream::Create(const wstring& sFileName, ASCUInt uDesiredAccess, ASCUInt uShareMode, ASCUInt uDisposition, ASCUInt uFlagsAndAttrs)
{
	m_nHandle = (ASCInt)CreateFileW(sFileName.c_str(), uDesiredAccess, uShareMode, 0,  
		uDisposition, uFlagsAndAttrs, 0);
	return (m_nHandle > 0);
}

ASCBoolean CASCFileStream::Open(const wstring& sFileName, ASCUInt uDesiredAccess /*= GENERIC_READ | GENERIC_WRITE*/, ASCUInt uShareMode /*= FILE_SHARE_READ*/)
{
	m_nHandle = (ASCInt)CreateFileW(sFileName.c_str(), uDesiredAccess, uShareMode, 0, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	return (m_nHandle > 0);
}

ASCInt CASCFileStream::Read(void* pBuffer, ASCInt nCount)
{
	ASCDWord uResult;
	if (!ReadFile((HANDLE)m_nHandle, pBuffer, nCount, &uResult, 0))
		return 0;
	else
		return (ASCInt)uResult;
}

ASCInt CASCFileStream::Write(const void* pBuffer, ASCInt nCount)
{
	ASCDWord uResult;
	if (!WriteFile((HANDLE)m_nHandle, pBuffer, nCount, &uResult, 0))
		return 0;
	else
		return (ASCInt)uResult;
}

ASCInt CASCFileStream::Seek(ASCInt nOffset, CASCSeekOrigin Origin /*= asoBeginning*/)
{
	return SetFilePointer((HANDLE)m_nHandle, nOffset, 0, Origin);
}

void CASCFileStream::SetSize(const ASCInt nNewSize)
{
	Seek(nNewSize, asoBeginning);
	SetEndOfFile((HANDLE)m_nHandle);
}

wstring CASCFileStream::GetFileName()
{
	return m_sFileName;
}

ASCInt CASCFileStream::GetHandle()
{
	return m_nHandle;
}

void CASCFileStream::operator=(const CASCFileStream& Stream)
{

}

CASCMemoryStream::CASCMemoryStream() : CASCStream(), m_pMemory(0), m_nSize(0), m_nPosition(0), m_nCapacity(0)
{

}

CASCMemoryStream::CASCMemoryStream(const CASCMemoryStream& Stream)
{

}

CASCMemoryStream::~CASCMemoryStream()
{
	Clear();
}

ASCInt CASCMemoryStream::Read(void* pBuffer, ASCInt nCount)
{
	if ((m_nPosition >= 0) && (nCount >= 0))
	{
		ASCInt nResult = m_nSize - m_nPosition;
		if (nResult > 0)
		{
			if (nResult > nCount)
				nResult = nCount;
			memcpy(pBuffer, m_pMemory + m_nPosition, nResult);
			m_nPosition += nResult;
			return nResult;
		}
	}
	return 0;
}

ASCInt CASCMemoryStream::Write(const void* pBuffer, ASCInt nCount)
{
	if ((m_nPosition >= 0) && (nCount >= 0))
	{
		ASCInt nPos = m_nPosition + nCount;
		if (nPos > 0)
		{
			if (nPos > m_nSize)
			{
				if (nPos > m_nCapacity)
					SetCapacity(nPos);
				m_nSize = nPos;
			}
			memcpy(m_pMemory + m_nPosition, pBuffer, nCount);
			m_nPosition = nPos;
			return nCount;
		}
	}
	return 0;
}

ASCInt CASCMemoryStream::Seek(ASCInt nOffset, CASCSeekOrigin Origin /*= asoBeginning*/)
{
	switch(Origin)
	{
	case asoBeginning:
		m_nPosition = nOffset;
		break;
	case asoCurrent:
		m_nPosition += nOffset;
		break;
	case asoEnd:
		m_nPosition = m_nSize + nOffset;
		break;
	}
	return m_nPosition;
}

void CASCMemoryStream::SetSize(const ASCInt nNewSize)
{
	ASCInt nOldPos = m_nPosition;
	SetCapacity(nNewSize);
	m_nSize = nNewSize;
	if (nOldPos > nNewSize)
		Seek(0, asoEnd);
}

void CASCMemoryStream::Clear()
{
	SetCapacity(0);
	m_nSize = 0;
	m_nPosition = 0;
}

ASCBoolean CASCMemoryStream::SaveToStream(CASCStream* pStream)
{
	return (pStream->WriteBuffer(m_pMemory, m_nSize));
}

ASCBoolean CASCMemoryStream::LoadFromStream(CASCStream* pStream)
{
	pStream->SetPosition(0);
	ASCInt nCount = pStream->GetSize();
	SetSize(nCount);
	if (nCount > 0)
		return (pStream->ReadBuffer(m_pMemory, nCount));
	return false;
}

ASCBoolean CASCMemoryStream::SaveToFile(const wstring& sFileName)
{
	CASCFileStream FileStream;
	if (FileStream.Create(sFileName))
		return SaveToStream(&FileStream);
	return false;
}

ASCBoolean CASCMemoryStream::LoadFromFile(const wstring& sFileName)
{
	CASCFileStream FileStream;
	if (FileStream.Open(sFileName, GENERIC_READ, FILE_SHARE_READ))
		return LoadFromStream(&FileStream);
	return false;
}

void CASCMemoryStream::operator=(const CASCMemoryStream& Stream)
{

}

void* CASCMemoryStream::Realloc(ASCInt& nNewCapacity)
{
	const ASCUInt cMemoryDelta = 0x2000;  // Must be a power of 2 

	if ((nNewCapacity > 0) && (nNewCapacity != m_nSize))
		nNewCapacity = (nNewCapacity + (cMemoryDelta - 1)) & (~(cMemoryDelta - 1));

	void* pResult = m_pMemory;
	if (nNewCapacity != m_nCapacity)
	{
		if (nNewCapacity == 0)
		{
			free(pResult);
			pResult = 0;
		}
		else 
		{
			if (m_nCapacity == 0)
				pResult = malloc(nNewCapacity);
			else
				pResult = realloc(pResult, nNewCapacity);
			// if Result = nil then raise EStreamError.CreateRes(@SMemoryStreamError);
		}
	}
	return pResult;
}

void CASCMemoryStream::SetCapacity(ASCInt nNewCapacity)
{
	SetPointer(Realloc(nNewCapacity), m_nSize);
	m_nCapacity = nNewCapacity;
}

void CASCMemoryStream::SetPointer(void* pPointer, ASCInt nSize)
{
	m_pMemory = (ASCByte*)pPointer;
	m_nSize = nSize;
}

ASCByte* CASCMemoryStream::GetMemoryPointer()
{
	return m_pMemory;
}