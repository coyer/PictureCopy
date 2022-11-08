#include "stdafx.h"
#include "FileStyleJudge.h"

CSingleType::CSingleType(CString& name)
{
	styleName = name;
}

CSingleType::~CSingleType() {
	extNames.RemoveAll();
}

void CSingleType::SetName(CString& name)
{
	styleName = name;
}

int CSingleType::AddExt(CString& ext)
{
	if (ext.GetLength() == 0) return -1;
	int i = FindExt(ext);
	if (i < 0) {
		i = extNames.Add(ext);
	}
	return i;
}

int CSingleType::FindExt(CString& ext)
{
	int n = extNames.GetCount();
	for (int i = 0; i < n; i++) {
		if (ext == extNames.GetAt(i)) {
			return i;
		}
	}
	return -1;
}

BOOL  CSingleType::DeleteExt(CString& ext)
{
	int i = FindExt(ext);
	if (i >= 0) {
		extNames.RemoveAt(i);
		return TRUE;
	}
	
	return FALSE;
}

BOOL CSingleType::SetExtsFromString(CString& str) {
	extNames.RemoveAll();
	CString strValue; // an individual name, value pair
	int i = 0; // substring index to extract
	while (AfxExtractSubString(strValue, str, i++, _T(',')))
	{
		strValue.Trim();
		if (strValue.GetLength() > 0) {
			extNames.Add(strValue);
		}
	}
	return TRUE;
}

CString CSingleType::ExtsToString() {
	CString str;
	int n = extNames.GetCount();
	for (int i = 0; i < n; i++) {
		str += extNames.GetAt(i) + _T(",");
	}
	return str;
}


///////////////////////////////////////////////////////////////////////////////////
CFileStyleJudge::CFileStyleJudge()
{
	AddStyleAndExts(_T("照片"), _T("jpg,jpeg,png,heic,tiff,bmp,webp,gif,bmp,psd,raw,svg,hdr,tif,ico"));
	AddStyleAndExts(_T("音乐"), _T("mp3,wav,ape,cda,aif,aiff,mid,midi,wma,ra,m4a,flac,aac,adt,adts"));
	AddStyleAndExts(_T("视频"), _T("mpg,mpeg,mp4,avi,rm,rmvb,mov,wmv,asf,asx,mkv,flv,vob,3gp,navi"));
	AddStyleAndExts(_T("文档"), _T("txt,doc,xml,ppt,pps,docs,xls,xlsx,xltx,docm,dot,potx,ppsm,pptx,rtf,vsd,vsdm,wps,"));
	AddStyleAndExts(_T("种子"), _T("torrent,"));
}

CFileStyleJudge::~CFileStyleJudge()
{
	Clear();
}

CFileStyleJudge* CFileStyleJudge::GetInstance() {
	static CFileStyleJudge global;
	return &global;
}

CString	CFileStyleJudge::ParseType(CString& extName)
{
	int n = m_typeData.GetCount();
	for (int i = 0; i < n; i++) {
		CSingleType* p = (CSingleType*)m_typeData.GetAt(i);
		int t = p->FindExt(extName);
		if (t >= 0) return p->styleName;
	}
	return _T("");
}

void	CFileStyleJudge::Clear() {
	int n = m_typeData.GetCount();
	for (int i = 0; i < n; i++) {
		CSingleType* p = (CSingleType*)m_typeData.GetAt(i);
		delete p;
	}
	m_typeData.RemoveAll();
}

CSingleType* CFileStyleJudge::AddStyle(CString& styleName) {
	CSingleType* pType = GetStyle(styleName);
	if (!pType) {
		pType = new CSingleType(styleName);
		m_typeData.Add(pType);
	}
	return pType;
}

CSingleType* CFileStyleJudge::GetStyle(CString& styleName)
{
	int n = m_typeData.GetCount();
	for (int i = 0; i < n; i++) {
		CSingleType* p = (CSingleType*)m_typeData.GetAt(i);
		if (p->styleName == styleName) return p;
	}

	return NULL;
}

CStringArray* CFileStyleJudge::GetAllExt(CString& styleName)
{
	CSingleType* p = GetStyle(styleName);
	if (!p) return NULL;
	return &(p->extNames);
}


CSingleType* CFileStyleJudge::GetAt(int index)
{
	CSingleType* p = (CSingleType*)m_typeData.GetAt(index);
	return p;
}

int	 CFileStyleJudge::GetCount()
{
	return m_typeData.GetCount();
}

void CFileStyleJudge::DeleteStyle(CString& styleName)
{
	int i = GetIndex(styleName);
	DeleteStyle(i);
}

void CFileStyleJudge::DeleteStyle(int index)
{
	if (index < 0 || index >= m_typeData.GetCount()) return;
	m_typeData.RemoveAt(index);
}

void CFileStyleJudge::DeleteExt(CString& styleName, CString& extName)
{
	CSingleType* p = GetStyle(styleName);
	if (!p) return;
	p->DeleteExt(extName);
}

int	 CFileStyleJudge::GetIndex(CString& styleName)
{
	int n = m_typeData.GetCount();
	for (int i = 0; i < n; i++) {
		CSingleType* p = (CSingleType*)m_typeData.GetAt(i);
		if (p->styleName == styleName) return i;
	}

	return -1;
}

BOOL CFileStyleJudge::UpdateStyleName(int index, CString& newName)
{
	if (index < 0 || index >= m_typeData.GetCount()) {
		return FALSE;
	}

	CSingleType* p = (CSingleType*)m_typeData.GetAt(index);
	p->SetName(newName);

	return TRUE;
}

BOOL CFileStyleJudge::UpdateStyleName(CString& oldName, CString& newName)
{
	if (newName.GetLength() == 0) return FALSE;

	int index = GetIndex(oldName);
	return UpdateStyleName(index, newName);
}

BOOL CFileStyleJudge::UpdateExtText(int index, CString& text)
{
	if (index < 0 || index >= m_typeData.GetCount()) {
		return FALSE;
	}
	CSingleType* p = (CSingleType*)m_typeData.GetAt(index);
	return p->SetExtsFromString(text);
}

CString	CFileStyleJudge::GetExtString(int index) {
	if (index < 0 || index >= m_typeData.GetCount()) {
		return _T("");
	}
	CSingleType* p = (CSingleType*)m_typeData.GetAt(index);
	return p->ExtsToString();
}

BOOL CFileStyleJudge::AddStyleAndExts(LPCTSTR styleName, LPCTSTR extsarr)
{
	if (!styleName) return FALSE;
	CString style = styleName;
	CString exts = extsarr;
	return AddStyleAndExts(style, exts);
}
BOOL CFileStyleJudge::AddStyleAndExts(CString& styleName, CString& extsarr)
{
	CSingleType* p = AddStyle(styleName);
	if (!p) return FALSE;

	p->SetExtsFromString(extsarr);
	return TRUE;
}