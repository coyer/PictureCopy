#pragma once
/*
*	�����ж�
*	���ߣ�coyer@126.com
*/
//�������Ƚ�С��ֱ�����������ˡ���Ҫ�ǲ�����std������ֱ࣬����mfc�ˡ�
//����������Ƚϴ���Ҫ����hash map
class CSingleType {
public:
	CSingleType(CString& name);
	~CSingleType();
	
	CString styleName;
	CStringArray extNames;

	void  SetName(CString& name);
	int   AddExt(CString& ext);
	BOOL  DeleteExt(CString& ext);
	int	  FindExt(CString& ext);
	BOOL  SetExtsFromString(CString& str);
	CString ExtsToString();
};

class CFileStyleJudge
{
public:
	CFileStyleJudge();
	~CFileStyleJudge();
	void		AddDefaultTypes();
	static CFileStyleJudge*	 GetInstance();

	CString		GetSaveString();
	BOOL		LoadSaveString(CString& str);

	CString		ParseType(CString& extname);
	BOOL		UpdateExtText(int index, CString& text);
	CString		GetExtString(int index);

	BOOL		AddStyleAndExts(CString& styleName, CString& extsarr);
	BOOL		AddStyleAndExts(LPCTSTR styleName, LPCTSTR extsarr);

	void		Clear();
	CSingleType*	AddStyle(CString& styleName);
	CSingleType*	GetStyle(CString& styleName);
	
	CStringArray*	GetAllExt(CString& styleName);

	CSingleType*		GetAt(int index);
	int					GetCount();
	void				DeleteStyle(CString& styleName);
	void				DeleteStyle(int index);
	void				DeleteExt(CString& styleName, CString& extName);
	int					GetIndex(CString& styleName);
	BOOL				UpdateStyleName(int index, CString& newName);
	BOOL				UpdateStyleName(CString& oldName, CString& newName);
private:
	CPtrArray		m_typeData;
};

