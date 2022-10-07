#pragma once
/*
*	�ַ�������ת��
*	���ߣ�coyer@126.com
*/
#include <cstringt.h>

//
// CStringתCStringA
//
#ifdef _UNICODE
#define CStrT2CStrA(cstr) CStrW2CStrA((cstr))
#else
#define CStrT2CStrA(cstr) (cstr)
#endif


//
// CStringתCStringW
//
#ifdef _UNICODE
#define CStrT2CStrW(cstr) (cstr)
#else
#define CStrT2CStrW(cstr) CStrA2CStrW((cstr))
#endif


//
// CStringAתCString
//

#ifdef _UNICODE
#define CStrA2CStrT(cstr) CStrA2CStrW((cstr))
#else
#define CStrA2CStrT(cstr) (cstr)
#endif

//
// CStringWתCString
//

#ifdef _UNICODE
#define CStrW2CStrT(cstr) (cstr)
#else
#define CStrW2CStrT(cstr) CStrW2CStrA((cstr))
#endif


#ifdef _UNICODE
#define Utf8ToCStrT(cstr)	Utf8ToCStrW((cstr))
#else
#define Utf8ToCStrT(cstr)	Utf8ToCStrA((cstr))
#endif

#ifdef _UNICODE
#define CStrT2Utf8(cstr)	CStrW2Utf8((cstr))
#else
#define CStrT2Utf8(cstr)	CStrA2Utf8((cstr))
#endif

//
// CStringAתCStringW
//
CStringW CStrA2CStrW(const CStringA &cstrSrcA);

//
// CStringWתCStringA
//
CStringA CStrW2CStrA(const CStringW &cstrSrcW);

//
// CString��UTF8��ת
//
CStringW Utf8ToCStrW(const CStringA& utf8str);
CStringA Utf8ToCStrA(const CStringA& utf8str);

CStringA CStrW2Utf8(const CStringW& cstrSrcW);
CStringA CStrA2Utf8(const CStringA& cstrSrcA);
