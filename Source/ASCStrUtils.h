/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCStrUtils.h
 *  Content: ASC(Asphyre Sphinx for C++) string helper utilities
 *  Date   : 2013/08/14
 *  Desc   : Useful string functions used by ASC(Thanks to LinZhenqun)
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <Windows.h>
#include <mbstring.h>
#include <string>
using std::string;
using std::wstring;
#include "ASCTypes.h"

wstring FormatV(const ASCWChar* pwcFormat, va_list argList);
wstring __cdecl Format(const ASCWChar* pwcFormat, ...);
string FormatVA(const ASCChar* pcFormat, va_list argList);
string __cdecl FormatA(const ASCChar* pcFormat, ...);

wstring UpperCase(const wstring& ws);
string UpperCaseA(const string& s);

wstring LowerCase(const wstring& ws);
string LowerCaseA(const string& s);

ASCBoolean Compare(const wstring& ws1, const wstring& ws2, ASCBoolean bSensitive = true);
ASCBoolean CompareA(const string& s1, const string& s2, ASCBoolean bSensitive = true);

wstring Trim(const wstring& ws, ASCWChar ch = ' ');
wstring TrimLeft(const wstring& ws, ASCWChar ch = ' ');
wstring TrimRight(const wstring& ws, ASCWChar ch = ' ');

wstring IntToStr(ASCInt nValue);
string IntToStrA(ASCInt nValue);

wstring UIntToStr(ASCUInt uValue);
string UIntToStrA(ASCUInt uValue);

wstring FloatToStr(ASCDouble fValue);
string FloatToStrA(ASCDouble fValue);

ASCInt StrToInt(const wstring& ws);
ASCInt StrToIntA(const string& s);

ASCBoolean TryStrToInt(const wstring& ws, ASCInt& nRet);
ASCBoolean TryStrToIntA(const string& s, ASCInt& nRet);

ASCInt StrToIntDef(const wstring& ws, ASCInt nDef = 0);
ASCInt StrToIntDefA(const string& s, ASCInt nDef = 0);

ASCUInt StrToUInt(const wstring& ws);
ASCUInt StrToUIntA(const string& s);

ASCBoolean TryStrToUInt(const wstring& ws, ASCUInt& uRet);
ASCBoolean TryStrToUIntA(const string& s, ASCUInt& uRet);

ASCUInt StrToUIntDef(const wstring& ws, ASCUInt uDef);
ASCUInt StrToUIntDefA(const string& s, ASCUInt uDef);

ASCDouble StrToFloat(const wstring& ws);
ASCDouble StrToFloatA(const string& s);

ASCBoolean TryStrToFloat(const wstring& ws, ASCDouble& fRet);
ASCBoolean TryStrToFloatA(const string& s, ASCDouble& fRet);

ASCDouble StrToFloatDef(const wstring& ws, ASCDouble fDef);
ASCDouble StrToFloatDefA(const string& s, ASCDouble fDef);

ASCBoolean StrToBool(const wstring& ws);
wstring BoolToStr(ASCBoolean bValue);

wstring StrToWStr(const string& s, ASCUInt uCP = CP_ACP);
string WStrToStr(const wstring& ws, ASCUInt uCP = CP_ACP);

/*
 * 提取字符串, ws 提取后会删除被提取的部分, wc 是分割符.
 * 返回提取的字符串
*/
wstring FetchString(wstring &ws, ASCChar wc);