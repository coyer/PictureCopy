#include "stdafx.h"
#include "Configer.h"
#include "CopyWorker.h"
#include "CJson.h"


CConfiger::CConfiger()
{
	filter = _T("*.*");
	sourceFileNotDelete = 0;
	overWriteMode = SKIP;
	destFolderRuleMode = 0;
	destFilenameRuleMode = 0;
	destFolderRuleString = _T("<YYYY>-<MM>");
	destFilenameRuleString = SubFolderNameMode[0];

	timeFromExif = 0;
	timeFromFolder = 0;
	timeFromFilename = 0;

	m_bNeedMoreTime = FALSE;
	m_bNeedExtType = FALSE;

	yearRangeMin = 2000;
	yearRangeMax = 2022;
}

CConfiger::~CConfiger()
{
}

CConfiger* CConfiger::GetInstance() {
	static CConfiger gcfg;
	return &gcfg;
}

BOOL CConfiger::isReady()
{
	if (destPathRoot.IsEmpty()) return FALSE;
	if (sourcePaths.GetCount() == 0) return FALSE;

	if (destFolderRuleMode && destFolderRuleString.GetLength() == 0) return FALSE;
	if (destFilenameRuleMode && destFilenameRuleString.GetLength() == 0) return FALSE;

	return TRUE;
}

//BOOL MyWritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int value, LPCTSTR lpFileName)
//{
//	CString strValue;
//	strValue.Format(_T("%d"), value);
//	return ::WritePrivateProfileString(lpAppName, lpKeyName, strValue, lpFileName);
//}

CString int2CStr(int val) {
	CString str;
	str.Format(_T("%d"), val);
	return str;
}

CStringA CConfiger::combineKeyValue(const char* keyname, CString keyvalue) {
	CStringA strTmp;
	strTmp.Format("%s=%s\n", keyname, CStrT2Utf8(keyvalue));
	return strTmp;
}

CStringA CConfiger::combineKeyValue(const char* keyname, int keyvalue) {
	CStringA strTmp;
	strTmp.Format("%s=%d\n", keyname, keyvalue);
	return strTmp;
}

BOOL CConfiger::saveToFile(CString& filename)
{
	CStringA content;

	CFile file;
	if (!file.Open(filename, CFile::modeCreate | CFile::modeWrite)) {
		return FALSE;
	}
	char header[3] = { (char)0xEF, (char)0xBB, (char)0xBF };
	file.Write(header, 3);

	content += combineKeyValue("SourcePathes", getSourceFolderStr());
	content += combineKeyValue("DestPathRoot", destPathRoot);
	content += combineKeyValue("FileFilter", filter);
	content += combineKeyValue("SourceFileNotDelete", sourceFileNotDelete);
	content += combineKeyValue("OverWriteMode", (int)overWriteMode);
	content += combineKeyValue("DestFolderRuleMode", destFolderRuleMode);
	content += combineKeyValue("DestFilenameRuleMode", destFilenameRuleMode);
	content += combineKeyValue("DestFolderRuleStr", getDestFolderRuleString());
	content += combineKeyValue("DestFilenameRuleStr", getDestFilenameRuleString());
	content += combineKeyValue("TimeFromExif", timeFromExif);
	content += combineKeyValue("TimeFromFolder", timeFromFolder);
	content += combineKeyValue("TimeFromFilename", timeFromFilename);
	content += combineKeyValue("YearRangeMin", yearRangeMin);
	content += combineKeyValue("YearRangeMax", yearRangeMax);

	file.Write(content, content.GetLength());
	file.Close();

	//WritePrivateProfileString(NULL, _T("SourcePathes"), strSource, filename);
	//WritePrivateProfileString(NULL, _T("DestPathRoot"), destPathRoot, filename);
	//WritePrivateProfileString(NULL, _T("FileFilter"), filter, filename);

	//MyWritePrivateProfileInt(NULL, _T("SourceFileNotDelete"), sourceFileNotDelete, filename);
	//MyWritePrivateProfileInt(NULL, _T("OverWriteMode"), overWriteMode, filename);
	//MyWritePrivateProfileInt(NULL, _T("DestFolderRuleMode"), destFolderRuleMode, filename);
	//MyWritePrivateProfileInt(NULL, _T("DestFilenameRuleMode"), destFilenameRuleMode, filename);

	//WritePrivateProfileString(NULL, _T("DestFolderRuleStr"), getDestFolderRuleString(), filename);
	//WritePrivateProfileString(NULL, _T("DestFilenameRuleStr"), getDestFilenameRuleString(), filename);
	return TRUE;
}

//CString MyGetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR defValue, LPCTSTR lpFileName)
//{
//	CString strTmp;
//	::GetPrivateProfileString(lpAppName, lpKeyName, defValue, strTmp.GetBuffer(2048), 2046, lpFileName);
// strTmp.ReleaseBuffer();
//	return strTmp;
//}

BOOL CConfiger::loadFromFile(CString& filename)
{
	CFile file;
	if (!file.Open(filename, CFile::modeRead)) {
		return FALSE;
	}
	ULONGLONG filesize = file.GetLength();

	BYTE head[3];
	file.Read(head, 3);
	if (!(head[0] == 0xEF && head[1] == 0xBB && head[2] == 0xBF))
	{
		file.SeekToBegin();
	}
	
	//only read 1M string;
	if (filesize > 1024 * 1024) filesize = 1024 * 1024;
	CStringA content;
	file.Read(content.GetBuffer(filesize), filesize);
	file.Close();

	content.ReleaseBuffer();
	CString jsonContent = Utf8ToCStrT(content);

	CString strNameValue; // an individual name, value pair

	int i = 0; // substring index to extract
	while (AfxExtractSubString(strNameValue, jsonContent, i++))
	{
		strNameValue.TrimLeft();
		if (strNameValue.Find(_T("//")) == 0) {
			continue;
		}
		CString strName, strValue; // individual name and value elements

		// Attempt to extract the name element from the pair
		if (!AfxExtractSubString(strName, strNameValue, 0, _T('=')))
		{
			continue;
		}

		// Attempt to extract the value element from the pair
		if (!AfxExtractSubString(strValue, strNameValue, 1, _T('=')))
		{
			continue;
		}

		// Pass the name, value pair to the debugger for display
		SetConfig(strName, strValue);
	}

	/*sourceFileNotDelete = ::GetPrivateProfileInt(NULL, _T("SourceFileNotDelete"), 0, filename);
	overWriteMode = ::GetPrivateProfileInt(NULL, _T("OverWriteMode"), 0, filename);
	destFolderRuleMode = ::GetPrivateProfileInt(NULL, _T("DestFolderRuleMode"), 0, filename);
	destFilenameRuleMode = ::GetPrivateProfileInt(NULL, _T("DestFilenameRuleMode"), 0, filename);

	destPathRoot = MyGetPrivateProfileString(NULL, _T("DestPathRoot"), _T(""), filename);
	filter = MyGetPrivateProfileString(NULL, _T("FileFilter"), _T("*.*"), filename);

	CString souceTotal = MyGetPrivateProfileString(NULL, _T("SourcePathes"), _T(""), filename);

	i = 0;
	CString strPath;
	srcPaths.RemoveAll();
	while (AfxExtractSubString(strPath, souceTotal, i++, _T('|'))) {
		srcPaths.Add(strPath);
	}

	CString folderRules = MyGetPrivateProfileString(NULL, _T("DestFolderRuleStr"), _T(""), filename);
	setDestFolderRule(folderRules);
	CString fileRules = MyGetPrivateProfileString(NULL, _T("DestFilenameRuleStr"), _T(""), filename);
	setDestFilenameRule(fileRules);*/

	return TRUE;
}

void CConfiger::SetConfig(CString& keyname, CString& value)
{
	if (keyname == _T("SourceFileNotDelete")) {
		sourceFileNotDelete = _wcstol_l(value, 0, 10, 0);
		return;
	}
	if (keyname == _T("OverWriteMode")) {
		overWriteMode = (OVERWRITEMODE)_wcstol_l(value, 0, 10, 0);
		return;
	}
	if (keyname == _T("DestFolderRuleMode")) {
		destFolderRuleMode = _wcstol_l(value, 0, 10, 0);
		return;
	}
	if (keyname == _T("DestFilenameRuleMode")) {
		destFilenameRuleMode = _wcstol_l(value, 0, 10, 0);
		return;
	}

	if (keyname == _T("DestPathRoot")) {
		setDestFolderStr(value);
		return;
	}

	if (keyname == _T("FileFilter")) {
		filter = value;
		return;
	}

	if (keyname == _T("SourcePathes")) {
		setSourceFolderStr(value);
		return;
	}

	if (keyname == _T("DestFolderRuleStr")) {
		setDestFolderRule(value);
		return;
	}

	if (keyname == _T("DestFilenameRuleStr")) {
		setDestFilenameRule(value);
		return;
	}

	if (keyname == _T("TimeFromExif")) {
		timeFromExif = _wcstol_l(value, 0, 10, 0);
		return;
	}
	if (keyname == _T("TimeFromFolder")) {
		timeFromFolder = _wcstol_l(value, 0, 10, 0);
		return;
	}
	if (keyname == _T("TimeFromFilename")) {
		timeFromFilename = _wcstol_l(value, 0, 10, 0);
		return;
	}
	if (keyname == _T("YearRangeMin")) {
		yearRangeMin = _wcstol_l(value, 0, 10, 0);
		return;
	}
	if (keyname == _T("YearRangeMax")) {
		yearRangeMax = _wcstol_l(value, 0, 10, 0);
		return;
	}
}

CString CConfiger::getDestFolderRuleString()
{
	return destFolderRuleString;
}

CString CConfiger::getDestFilenameRuleString()
{
	return destFilenameRuleString;
}

BOOL CConfiger::setDestFolderRule(CString& ruleStr)
{
	TCHAR* szBanFolder = _T("?*:|\"");
	
	if (ruleStr.FindOneOf(szBanFolder) >= 0) return FALSE;
	destFolderRuleString = ruleStr;
	destFolderRuleString.Replace(_T('/'), _T('\\'));
	return TRUE;
}

BOOL CConfiger::setDestFilenameRule(CString& ruleStr)
{
	TCHAR* szBan = _T("?*:|\"/\\");
	destFilenameRuleString = ruleStr;
	if (ruleStr.FindOneOf(szBan) >= 0) return FALSE;

	return TRUE;
}

BOOL CConfiger::addSourceFolder(CString& folder)
{
	if (folder.GetLength() == 0) return FALSE;
	TCHAR ch = folder.GetAt(folder.GetLength() - 1);
	if (ch != _T('/') && ch != _T('\\')) {
		folder += _T("\\");
	}

	for (int i = 0; i < sourcePaths.GetCount(); i++) {
		if (sourcePaths.GetAt(i) == folder) return FALSE;
	}
	sourcePaths.Add(folder);
	return TRUE;
}

CString	CConfiger::getSourceFolderStr() {
	CString strSource;
	for (int i = 0; i < sourcePaths.GetCount(); i++) {
		strSource += sourcePaths.GetAt(i) + _T("|");
	}
	return strSource;
}

BOOL CConfiger::setSourceFolderStr(CString& folders)
{
	int i = 0;
	CString strPath;
	sourcePaths.RemoveAll();
	while (AfxExtractSubString(strPath, folders, i++, _T('|'))) {
		addSourceFolder(strPath);
	}
	return TRUE;
}

BOOL CConfiger::setDestFolderStr(CString& folder)
{
	destPathRoot = folder;
	if (destPathRoot.GetLength() == 0) return FALSE;
	TCHAR ch = destPathRoot.GetAt(destPathRoot.GetLength() - 1);
	if (ch != _T('/') && ch != _T('\\')) destPathRoot += _T("\\");
	return TRUE;
}


BOOL JudgeFileHaveTime(CString& str) {
	if (str.Find(_T("<YYYY>")) >= 0) return TRUE;
	if (str.Find(_T("<YY>")) >= 0) return TRUE;
	if (str.Find(_T("<MM>")) >= 0) return TRUE;
	if (str.Find(_T("<DD>")) >= 0) return TRUE;
	return FALSE;
}

BOOL CConfiger::isFileNeedMoreTime()
{
	return m_bNeedMoreTime;
}

BOOL CConfiger::isFileNeedExtType()
{
	return m_bNeedExtType;
}

BOOL CConfiger::BeforePrepareRun()
{
	m_bNeedExtType = FALSE;
	m_bNeedMoreTime = FALSE;
	if (overWriteMode == SAVENEW || overWriteMode == SAVEOLD) {
		m_bNeedMoreTime = TRUE;
	}
	if (destFolderRuleMode && JudgeFileHaveTime(destFolderRuleString)) {
		m_bNeedMoreTime = TRUE;
	}
	if (destFilenameRuleMode && JudgeFileHaveTime(destFilenameRuleString)) {
		m_bNeedMoreTime = TRUE;
	}

	if (destFolderRuleMode && destFolderRuleString.Find(_T("<TYPE>")) >= 0) {
		m_bNeedExtType = TRUE;
	}
	if (destFilenameRuleMode && destFilenameRuleString.Find(_T("<TYPE>")) >= 0) {
		m_bNeedExtType = TRUE;
	}
	return TRUE;
}