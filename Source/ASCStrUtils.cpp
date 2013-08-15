#include <algorithm>
using std::transform;
#include "ASCStrUtils.h"

wstring UpperCase(const wstring& ws)
{
	wstring sDest = ws;
	transform(sDest.begin(), sDest.end(), sDest.begin(),  toupper);
	return sDest;
}

string UpperCaseA(const string& s)
{
	string sDest = s;
	transform(sDest.begin(), sDest.end(), sDest.begin(),  toupper);
	return sDest;
}

wstring LowerCase(const wstring& ws)
{
	wstring sDest = ws;
	transform(sDest.begin(), sDest.end(), sDest.begin(),  tolower);
	return sDest;
}

string LowerCaseA(const string& s)
{
	string sDest = s;
	transform(sDest.begin(), sDest.end(), sDest.begin(),  tolower);
	return sDest;
}

ASCBoolean Compare(const wstring& ws1, const wstring& ws2, ASCBoolean bSensitive /*= true*/)
{
	if (bSensitive)
		return wcscmp(ws1.c_str(), ws2.c_str()) == 0;
	else
		return _wcsicmp(ws1.c_str(), ws2.c_str()) == 0;
}

ASCBoolean CompareA(const string& s1, const string& s2, ASCBoolean bSensitive /*= true*/)
{
	if (bSensitive)
		return _mbscmp((byte*)s1.c_str(), (byte*)s2.c_str()) == 0;
	else
		return _mbsicmp((byte*)s1.c_str(), (byte*)s2.c_str()) == 0;
}

wstring Trim(const wstring& ws, ASCWChar wc /*= ' '*/)
{
	wstring sDest;

	wstring::size_type r = ws.size() - 1;
	wstring::size_type l = 0;
	while ((l <= r) && (ws[l] == wc))
	{
		++l;
	}
	if (l > r)
	{
		return sDest;
	}
	while (ws[r] == wc)
	{
		--r;
	}

	sDest.assign(ws, l, r - l + 1);
	return sDest;
}

wstring TrimLeft(const wstring& ws, ASCWChar wc /*= ' '*/)
{
	wstring sDest;

	ASCInt r = ws.size() - 1;
	ASCInt l = 0;
	while ((l <= r) && (ws[l] == wc))
	{
		++l;
	}

	sDest.assign(ws, l, r - l + 1);
	return sDest;
}

wstring TrimRight(const wstring& ws, ASCWChar wc /*= ' '*/)
{
	wstring sDest;

	ASCInt r = ws.size() - 1;
	while (ws[r] == wc)
	{
		--r;
	}

	sDest.assign(ws, 0, r + 1);
	return sDest;
}

ASCInt StrToInt(const wstring& ws)
{
	wchar_t* pWCh;
	return wcstol(ws.c_str(), &pWCh, 0);
}

ASCInt StrToIntA(const string& s)
{
	char* pCh;
	return strtol(s.c_str(), &pCh, 0);
}

ASCBoolean TryStrToInt(const wstring& ws, ASCInt& nRet)
{
	errno = 0;
	ASCWChar* pWCh;
	ASCInt nValue = wcstol(ws.c_str(), &pWCh, 0);

	// 存在非法字符
	if (*pWCh)
		return false;

	// 溢出
	if (errno == ERANGE)
		return false;

	nRet = nValue;
	return true;
}

ASCBoolean TryStrToIntA(const string& s, ASCInt& nRet)
{
	errno = 0;
	char* pCh;
	ASCInt nValue = strtol(s.c_str(), &pCh, 0);

	if (*pCh)
		return false;

	if (errno == ERANGE)
		return false;

	nRet = nValue;
	return true;
}

ASCInt StrToIntDef(const wstring& ws, ASCInt nDef /*= 0*/)
{
	ASCInt nValue;
	if (!TryStrToInt(ws, nValue))
		nValue = nDef;
	return nValue;
}

ASCInt StrToIntDefA(const string& s, ASCInt nDef /*= 0*/)
{
	ASCInt nValue;
	if (!TryStrToIntA(s, nValue))
		nValue = nDef;
	return nValue;
}

ASCUInt StrToUInt(const wstring& ws)
{
	ASCWChar* pWCh;
	return wcstoul(ws.c_str(), &pWCh, 0);
}

ASCUInt StrToUIntA(const string& s)
{
	ASCChar* pCh;
	return strtoul(s.c_str(), &pCh, 0);
}

ASCBoolean TryStrToUInt(const wstring& ws, ASCUInt& uRet)
{
	errno = 0;
	ASCWChar* pWCh;
	ASCInt nValue = wcstoul(ws.c_str(), &pWCh, 0);

	if (*pWCh)
		return false;

	if (errno == ERANGE)
		return false;

	uRet = nValue;
	return true;
}

ASCBoolean TryStrToUIntA(const string& s, ASCUInt& uRet)
{
	errno = 0;
	ASCChar* pCh;
	ASCInt nValue = strtoul(s.c_str(), &pCh, 0);

	if (*pCh)
		return false;

	if (errno == ERANGE)
		return false;

	uRet = nValue;
	return true;
}

ASCUInt StrToUIntDef(const wstring& ws, ASCUInt uDef)
{
	ASCUInt uValue;
	if (!TryStrToUInt(ws, uValue))
		uValue = uDef;
	return uValue;
}

ASCUInt StrToUIntDefA(const string& s, ASCUInt uDef)
{
	ASCUInt uValue;
	if (!TryStrToUIntA(s, uValue))
		uValue = uDef;
	return uValue;
}

ASCDouble StrToFloat(const wstring& ws)
{
	ASCWChar* pWCh;
	return wcstod(ws.c_str(), &pWCh);
}

ASCDouble StrToFloatA(const string& s)
{
	ASCChar* pCh;
	return strtod(s.c_str(), &pCh);
}

ASCBoolean TryStrToFloat(const wstring& ws, ASCDouble& fRet)
{
	errno = 0;
	ASCWChar* pWCh;
	double fValue = wcstod(ws.c_str(), &pWCh);

	if (*pWCh)
		return false;

	if (errno == ERANGE)
		return false;

	fRet = fValue;
	return true;
}

ASCBoolean TryStrToFloatA(const string& s, ASCDouble& fRet)
{
	errno = 0;
	ASCChar* pCh;
	double fValue = strtod(s.c_str(), &pCh);

	if (*pCh)
		return false;

	if (errno == ERANGE)
		return false;

	fRet = fValue;
	return true;
}

ASCDouble StrToFloatDef(const wstring& ws, ASCDouble fDef)
{
	ASCDouble fValue;
	if (!TryStrToFloat(ws, fValue))
		fValue = fDef;
	return fValue;
}

ASCDouble StrToFloatDefA(const string& s, ASCDouble fDef)
{
	ASCDouble fValue;
	if (!TryStrToFloatA(s, fValue))
		fValue = fDef;
	return fValue;
}

wstring IntToStr(ASCInt nValue)
{
	return Format(L"%d", nValue);
}

string IntToStrA(ASCInt nValue)
{
	return FormatA("%d", nValue);
}

wstring UIntToStr(ASCUInt uValue)
{
	return Format(L"%u", uValue);
}

string UIntToStrA(ASCUInt uValue)
{
	return FormatA("%u", uValue);
}

wstring FloatToStr(ASCDouble fValue)
{
	return Format(L"%f", fValue);
}

string FloatToStrA(ASCDouble fValue)
{
	return FormatA("%f", fValue);
}

const wstring wsBoolTrue = L"True";
const wstring wsBoolFalse = L"False";

ASCBoolean StrToBool(const wstring& ws)
{
	if (ws == wsBoolTrue)
		return true;
	else
		return false;
}

wstring BoolToStr(ASCBoolean bValue)
{
	if (bValue)
		return wsBoolTrue;
	else
		return wsBoolFalse;
}

wstring FetchString(wstring &ws, ASCChar wc)
{
	wstring sSub;
	wstring::size_type pos = ws.find(wc);
	if (pos == wstring::npos)
	{
		sSub = ws;
		ws.erase(0, pos);
	}
	else
	{
		sSub = ws.substr(0, pos);
		ws.erase(0, pos + 1);
	}
	return sSub;
}

wstring FormatV(const ASCWChar* pwcFormat, va_list argList)
{
	wstring ws;
	ASCInt nLen = _vscwprintf(pwcFormat, argList);
	if (nLen > 0)
	{
		ws.resize(nLen);
		vswprintf_s(&ws[0], nLen + 1, pwcFormat, argList);
	}
	return ws;
}

wstring __cdecl Format(const ASCWChar* pwcFormat, ...)
{
	va_list argList;
	va_start(argList, pwcFormat);
	wstring ws = FormatV(pwcFormat, argList);    
	va_end(argList);
	return ws;
}

string FormatVA(const ASCChar* pcFormat, va_list argList)
{
	string s;
	ASCInt nLen = _vscprintf(pcFormat, argList);
	if (nLen > 0)
	{
		s.resize(nLen);
		vsprintf_s(&s[0], nLen + 1, pcFormat, argList);
	}
	return s;
}

string __cdecl FormatA(const ASCChar* pcFormat, ...)
{
	va_list argList;
	va_start(argList, pcFormat);
	string s = FormatVA(pcFormat, argList);
	va_end(argList);
	return s;
}

wstring StrToWStr(const string& s, ASCUInt uCP /*= CP_ACP*/)
{
	wstring sDest;
	int nLen = ::MultiByteToWideChar(uCP, 0, s.c_str(), s.size(), NULL, 0);
	if (nLen == 0) 
		return sDest;
	sDest.resize(nLen);
	::MultiByteToWideChar(uCP, 0, s.c_str(), s.size(), &sDest[0], nLen);
	return sDest;
}

string WStrToStr(const wstring& ws, ASCUInt uCP /*= CP_ACP*/)
{
	string sDest;
	int nLen = ::WideCharToMultiByte(uCP, 0, ws.c_str(), ws.size(), NULL, 0, NULL, NULL);
	if (nLen == 0)
		return sDest;
	sDest.resize(nLen);
	::WideCharToMultiByte(uCP, 0, ws.c_str(), ws.size(), &sDest[0], nLen, NULL, NULL);
	return sDest;
}