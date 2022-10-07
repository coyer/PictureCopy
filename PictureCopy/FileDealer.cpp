#include "stdafx.h"
#include "FileDealer.h"
#include "Configer.h"
#include "exif.h"
#include "FileStyleJudge.h"
#include "exif.h"
#include <time.h>

CFileDealer::CFileDealer()
{
	m_fileSize = 0;
	m_styleType = _T("others");
	memset(&m_createTime, 0, sizeof(FileDateInfo));
	memset(&m_dateInfo, 0, sizeof(FileDateInfo));
	m_updatetime = 0;
}

CFileDealer::~CFileDealer()
{
}

//
CFileDealer::CFileDealer(CString& basePath, CString& path, CString filename) {
	m_srcPath = basePath;
	m_path = path;
	m_filename = filename;
	m_fullPathname = basePath + path + filename;

	int pos = m_filename.ReverseFind(_T('.'));
	if (pos >= 0) {
		m_extName = m_filename.Mid(pos + 1).MakeLower();
		m_pureName = m_filename.Left(pos);
	}
	else {
		m_extName = _T("");
		m_pureName = m_filename;
	}
	
}

void CFileDealer::SetCreateTime(FILETIME createTime)
{
	m_updatetime = ((__int64)createTime.dwHighDateTime << 32) + createTime.dwLowDateTime;

	SYSTEMTIME stUTC, timeLocal;
	FileTimeToSystemTime(&createTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &timeLocal);
	m_createTime.year = timeLocal.wYear;
	m_createTime.month = timeLocal.wMonth;
	m_createTime.day = timeLocal.wDay;
	m_dateInfo.dayOfWeek = timeLocal.wDayOfWeek;
}

void CFileDealer::SetFilesize(__int64 size)
{
	m_fileSize = size;
}

void CFileDealer::SetStatusInfo(FILETIME createTime, __int64 size)
{
	SetCreateTime(createTime);
	SetFilesize(size);
}

BOOL CFileDealer::SetMaybeTime(int year, int month, int day)
{
	m_dateInfo.year = year;
	m_dateInfo.month = month;
	m_dateInfo.day = day;
	return TRUE;
}

BOOL CFileDealer::SetMaybeTime(FileDateInfo& dateInfo)
{
	m_dateInfo = dateInfo;
	return TRUE;
}

BOOL CFileDealer::ParseFileExtType()
{
	CString strType = CFileStyleJudge::GetInstance()->ParseType(m_extName);
	m_styleType = strType;
	if (strType.GetLength() > 0) {
		return TRUE;
	}
	else {
		m_styleType = _T("others");
		return FALSE;
	}
}

int SplitNumberFromString(const char* strval, int* arrData, int maxcount) {
	if (!strval) return 0;
	int n = strlen(strval);
	int i = 0;
	const char* p = strval;
	while (p < strval + n)
	{
		if (*p >= '0' && *p <= '9') {
			char* endptr = 0;
			int v = strtol(p, &endptr, 10);
			arrData[i++] = v;
			p = endptr;
			if (i == maxcount) break;
		}
		p++;
	}
	return i;
}

int SplitNumberFromStringT(const TCHAR* strval, __int64* arrData, int maxcount) {
	if (!strval) return 0;
	int n = _tcslen(strval);
	int i = 0;
	const TCHAR* p = strval;
	while (p < strval + n)
	{
		if (*p >= '0' && *p <= '9') {
			int prev = 0, next = 0;
			TCHAR* endptr = 0;
			__int64 v = _tcstoi64(p, &endptr, 10);
			if (endptr) next = *endptr;
			if (p - strval > 1) prev = *(p - 1);

			if ((next >= _T('a') && next <= _T('z')) || next >= _T('A') && next <= _T('Z')) {
				//下一个字符可能是base64之类的，不能算是日期
			} else if ((prev >= _T('a') && prev <= _T('z')) || prev >= _T('A') && prev <= _T('Z')) {
				//前一个字符可能是base64之类的，也不能算是日期
			}
			else {
				arrData[i++] = v;
				if (i == maxcount) break;
			}
			
			p = endptr;
		}
		p++;
	}
	return i;
}

BOOL JudgeMonthDay(int y, int m, int d) {
	if (d < 1 || d > 31 || m > 12 || m < 1) return FALSE;
	if (m == 2 && d > 29) return FALSE;
	if (d == 31 && (m == 4 || m == 6 || m == 9 || m == 11)) return FALSE;
	int yearmax = CConfiger::GetInstance()->yearRangeMax;
	int yearmin = CConfiger::GetInstance()->yearRangeMin;
	if (y < yearmin || y > yearmax) return FALSE;
	return TRUE;
}

BOOL JudgeNumberDate(int a, int b, int c, FileDateInfo* pDateInfo) {
	if (JudgeMonthDay(a, b, c)) {
		pDateInfo->year = a;
		pDateInfo->month = b;
		pDateInfo->day = c;
		return TRUE;
	}
	if (JudgeMonthDay(c, b, a)) {
		pDateInfo->year = c;
		pDateInfo->month = b;
		pDateInfo->day = a;
		return TRUE;
	}
	if (JudgeMonthDay(c, a, b)) {
		pDateInfo->year = c;
		pDateInfo->month = a;
		pDateInfo->day = b;
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDealer::ParseExifInfo()
{
	easyexif::EXIFInfo exifinfo;
	if (exifinfo.parseFromFile(CStrT2CStrA(m_fullPathname)) != PARSE_EXIF_SUCCESS) {
		return FALSE;
	}
	CStringA strA = exifinfo.Model.c_str();
	m_exifDevice = CStrA2CStrT(strA);
	strA = exifinfo.Software.c_str();
	m_exifAuthor = CStrA2CStrT(strA);

	strA = exifinfo.DateTime.c_str();
	CString strDateTime = CStrA2CStrT(strA);
	return ParseTimeFromString(strDateTime, &m_dateInfo);
}

BOOL CFileDealer::ParseTimeFromString(CString& pureName, FileDateInfo* pDateInfo)
{
	if (pureName.GetLength() < 7) return FALSE;
	__int64 intarr[10] = { 0 };
	int n = SplitNumberFromStringT(pureName, intarr, 10);
	if (n == 0) return FALSE;

	int yearmax = CConfiger::GetInstance()->yearRangeMax;
	int yearmin = CConfiger::GetInstance()->yearRangeMin;

	if (n > 2) {
		for (int i = 0; i < n - 2; i++) {
			int a = (int)intarr[0];
			int b = (int)intarr[1];
			int c = (int)intarr[2];
			if (JudgeNumberDate(a, b, c, pDateInfo)) {
				return TRUE;
			}
		}
	}

	for (int i = 0; i < n; i++) {
		__int64 t = intarr[i];
		while (t > 99999999) t = t / 10;
		if (t >= yearmin* 10000 && t <= yearmax * 10000) {
			int s = t % 10000;
			int m = s / 100;
			int d = s % 100;
			int y = (int)(t / 10000);
			if (JudgeMonthDay(y, m, d)) {
				pDateInfo->year = (int)(t / 10000);
				pDateInfo->month = m;
				pDateInfo->day = d;
				return TRUE;
			}
		}
		t = t = intarr[i];
		if (t > 21000000 && t < 4000000000) {
			CTime tm = t;
			int y = tm.GetYear();
			if (y >= yearmin && y <= yearmax) {
				pDateInfo->year = tm.GetYear();
				pDateInfo->month = tm.GetMonth();
				pDateInfo->day = tm.GetDay();
				return TRUE;
			}
		}
	}
	
	

	return FALSE;
}

void SplitePath(CString& path, CStringArray& ret) {
	int len = path.GetLength();
	if (len == 0) return;

	int lastPos = 0;
	for (int i = 0; i < len; i++) {
		TCHAR ch = path.GetAt(i);
		if (ch == _T('/') || ch == _T('\\')) {
			if (i == lastPos) {
				continue;
			}
			ret.Add(path.Mid(lastPos, i - lastPos));
			lastPos = i + 1;
		}
	}
	if (lastPos < len) {
		ret.Add(path.Mid(lastPos + 1, len - lastPos));
	}
}

BOOL CFileDealer::ParseMoreTimeInfo(BOOL fromExif, BOOL fromFilename, BOOL fromFolder)
{
	FileDateInfo fdi = { 0 };
	if (fromExif && ParseExifInfo()) {
		return TRUE;
	}
	if (fromFilename) {
		if (ParseTimeFromString(m_pureName, &fdi)) {
			SetMaybeTime(fdi);
			return TRUE;
		}
	}
	if (fromFolder) {
		CStringArray arr;
		SplitePath(m_path, arr);
		int n = arr.GetCount();
		for (int i = 0; i < n; i ++ ) {
			if (ParseTimeFromString(arr[i], &fdi))
				SetMaybeTime(fdi);
				return TRUE;
		}
	}

	return FALSE;
}

// 判断文件是否存在
BOOL CFileDealer::IsFileExist(LPCTSTR csFile)
{
	DWORD dwAttrib = GetFileAttributes(csFile);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

// 判断文件夹是否存在
BOOL CFileDealer::IsDirExist(LPCTSTR csDir)
{
	DWORD dwAttrib = GetFileAttributes(csDir);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

// 判断文件或文件夹是否存在
BOOL CFileDealer::IsPathExist(LPCTSTR csPath)
{
	DWORD dwAttrib = GetFileAttributes(csPath);
	return INVALID_FILE_ATTRIBUTES != dwAttrib;
}

// 变变变变变种(听说会更快一点)，见备注1
BOOL CFileDealer::IsPathExistEx(LPCTSTR csPath)
{
	WIN32_FILE_ATTRIBUTE_DATA attrs = { 0 };
	return 0 != GetFileAttributesEx(csPath, GetFileExInfoStandard, &attrs);
}

BOOL CFileDealer::ReplaceStringVariant(CString& strVar, BOOL isPathMode) {
	strVar.Replace(_T("<TYPE>"), m_styleType);
	strVar.Replace(_T("<EXT>"), m_extName);
	strVar.Replace(_T("<DEV>"), m_exifDevice);
	strVar.Replace(_T("<AUTHOR>"), m_exifAuthor);
	
	CString strYear4, strYear2, strMonth, strDay, strWeek;
	strYear4.Format(_T("%d"), m_createTime.year);
	strYear2.Format(_T("%02d"), m_createTime.year % 100);
	strMonth.Format(_T("%02d"), m_createTime.month);
	strDay.Format(_T("%02d"), m_createTime.day);
	strWeek.Format(_T("d"), m_createTime.dayOfWeek);

	strVar.Replace(_T("<YYYY>"), strYear4);
	strVar.Replace(_T("<YY>"), strYear2);
	strVar.Replace(_T("<MM>"), strMonth);
	strVar.Replace(_T("<DD>"), strDay);
	strVar.Replace(_T("<WEEKDAY>"), strWeek);
	if (isPathMode) {
		strVar.Replace(_T("<~>"), m_path);
	}
	else {
		strVar.Replace(_T("<~>"), m_pureName);
	}
	return TRUE;
}
