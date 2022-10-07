#include "stdafx.h"
#include "CStringUtils.h"
#include <cstringt.h>

//
// CStringAתCStringW
//
CStringW CStrA2CStrW(const CStringA &cstrSrcA)
{
	CStringW cstrDestW;
	int len = MultiByteToWideChar(CP_ACP, 0, cstrSrcA, cstrSrcA.GetLength(), NULL, 0);

	MultiByteToWideChar(CP_ACP, 0, cstrSrcA, cstrSrcA.GetLength(), cstrDestW.GetBuffer(len), len);
	cstrDestW.ReleaseBuffer();
	return cstrDestW;
}

//
// CStringWתCStringA
//
CStringA CStrW2CStrA(const CStringW &cstrSrcW)
{
	CStringA cstrDestA;
	int len = WideCharToMultiByte(CP_ACP, 0, cstrSrcW, cstrSrcW.GetLength(), NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cstrSrcW), cstrSrcW.GetLength(), cstrDestA.GetBuffer(len), len, NULL, NULL);
	cstrDestA.ReleaseBuffer();
	return cstrDestA;
}

CStringW Utf8ToCStrW(const CStringA& utf8str)
{
	CStringW cstrDestW;
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8str, utf8str.GetLength(), NULL, 0);
	
	MultiByteToWideChar(CP_UTF8, 0, utf8str, utf8str.GetLength(), cstrDestW.GetBuffer(len), len);
	cstrDestW.ReleaseBuffer();
	return cstrDestW;
}

CStringA Utf8ToCStrA(const CStringA& utf8str)
{
	CStringW strW = Utf8ToCStrW(utf8str);
	return CStrW2CStrA(strW);
}

CStringA CStrW2Utf8(const CStringW& cstrSrcW)
{
	CStringA cstrDestA;
	int len = WideCharToMultiByte(CP_UTF8, 0, cstrSrcW, cstrSrcW.GetLength(), NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_UTF8, 0, LPCWSTR(cstrSrcW), cstrSrcW.GetLength(), cstrDestA.GetBuffer(len), len, NULL, NULL);
	cstrDestA.ReleaseBuffer();
	return cstrDestA;
}

CStringA CStrA2Utf8(const CStringA& cstrSrcA)
{
	CStringW strW = CStrA2CStrW(cstrSrcA);
	return CStrW2Utf8(strW);
}