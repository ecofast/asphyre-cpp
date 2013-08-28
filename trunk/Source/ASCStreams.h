/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCStreams.h
 *  Content: ASC(Asphyre Sphinx for C++) stream classes
 *  Date   : 2013/08/28
 *  Desc   : Stream classes from Delphi(2007) RTL
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <string>
using std::wstring;
#include <Windows.h>
#include "ASCTypes.h"

/*
 * Stream seek origins
*/
enum CASCSeekOrigin
{
	asoBeginning, asoCurrent, asoEnd
};

/*
 * 最大支持 2G 存储的流基类
*/
class CASCStream
{
public:
	virtual ASCInt Read(void* pBuffer, ASCInt nCount) = 0;
	virtual ASCInt Write(const void* pBuffer, ASCInt nCount) = 0;
	virtual ASCInt Seek(ASCInt nOffset, CASCSeekOrigin Origin = asoBeginning) = 0;
	ASCBoolean ReadBuffer(void* pBuffer, ASCInt nCount);
	ASCBoolean WriteBuffer(const void* pBuffer, ASCInt nCount);
	// 从其他流拷贝数据, 若 nCount 为 0 则拷贝整个流
	ASCInt CopyFrom(CASCStream* pSource, ASCInt nCount);
	ASCInt GetPosition();
	void SetPosition(const ASCInt nPos);
	virtual ASCInt GetSize();
	virtual void SetSize(const ASCInt nNewSize) = 0;
};

/*
 * 文件流
*/
class CASCFileStream : public CASCStream
{
public:
	CASCFileStream();
	virtual ~CASCFileStream();

	// 创建文件
	ASCBoolean Create(const wstring& sFileName, ASCUInt uDesiredAccess = GENERIC_READ | GENERIC_WRITE, ASCUInt uShareMode = FILE_SHARE_READ);
	// 创建或打开文件
	ASCBoolean Create(const wstring& sFileName, ASCUInt uDesiredAccess, ASCUInt uShareMode, ASCUInt uDisposition, ASCUInt uFlagsAndAttrs);
	// 打开文件
	ASCBoolean Open(const wstring& sFileName, ASCUInt uDesiredAccess = GENERIC_READ | GENERIC_WRITE, ASCUInt uShareMode = FILE_SHARE_READ);

	virtual ASCInt Read(void* pBuffer, ASCInt nCount);
	virtual ASCInt Write(const void* pBuffer, ASCInt nCount);
	virtual ASCInt Seek(ASCInt nOffset, CASCSeekOrigin Origin = asoBeginning);
	virtual void SetSize(const ASCInt nNewSize);

	wstring GetFileName();
	ASCInt GetHandle();
protected:
	CASCFileStream(const CASCFileStream& Stream);
	void operator = (const CASCFileStream& Stream);

	ASCInt	m_nHandle;
private:
	wstring	m_sFileName;
};

/*
 * 内存流
*/
class CASCMemoryStream : public CASCStream
{
public:
	CASCMemoryStream();
	virtual ~CASCMemoryStream();

	virtual ASCInt Read(void* pBuffer, ASCInt nCount);
	virtual ASCInt Write(const void* pBuffer, ASCInt nCount);
	virtual ASCInt Seek(ASCInt nOffset, CASCSeekOrigin Origin = asoBeginning);
	virtual void SetSize(const ASCInt nNewSize);

	void Clear();
	ASCBoolean SaveToStream(CASCStream* pStream);
	ASCBoolean LoadFromStream(CASCStream* pStream);
	ASCBoolean SaveToFile(const wstring& sFileName);
	ASCBoolean LoadFromFile(const wstring& sFileName);

	ASCByte* GetMemoryPointer();
protected:
	CASCMemoryStream(const CASCMemoryStream& Stream);
	void operator = (const CASCMemoryStream& Stream);

	virtual void* Realloc(ASCInt& nNewCapacity);
	void SetCapacity(ASCInt nNewCapacity);
	void SetPointer(void* pPointer, ASCInt nSize);
private:
	ASCByte*	m_pMemory;
	ASCInt		m_nSize;
	ASCInt		m_nPosition;
	ASCInt		m_nCapacity;
};