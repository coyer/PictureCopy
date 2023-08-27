#pragma once
/*
*	�����ļ�
*	���ߣ�coyer@126.com
*/

typedef struct {
	DWORD dwStart = 0;
	int fileCount = 0;
	int copyCount = 0;
	int overCount = 0;
	int folderCount = 0;
	int existFile = 0;
	int errorCount = 0;
}MYCPINFO;

const CString SubFolderNameMode[] = {
	_T("<~>"),		//ԭʼ·������ԭʼ�ļ���
	_T("<TYPE>"),	//���ͣ�audio��vidio��
	_T("<EXT>"),	//��չ��
	_T("<YYYY>"),   //�꣬4λ
	_T("<YY>"),		//�꣬2λ
	_T("<MM>"),		//��
	_T("<DD>"),		//��
	_T("<DEV>")		// exif �е��豸��������û��
};

typedef enum {
	SKIP = 0,
	RENAME,
	OVERWRITE,
	SAVENEW,
	SAVEOLD,
	SAVEBIG,
	SAVESMALL
}OVERWRITEMODE;

class CConfiger
{
public:
	CConfiger();
	~CConfiger();
	static CConfiger* GetInstance();

	BOOL saveToFile(CString& filename);
	BOOL loadFromFile(CString& filename);

	BOOL isReady();

	CString getDestFolderRuleString();
	CString getDestFilenameRuleString();
	BOOL	setDestFolderRule(CString& ruleStr);
	BOOL	setDestFilenameRule(CString& ruleStr);
	BOOL	addSourceFolder(CString& folder);
	CString	getSourceFolderStr();
	BOOL	setSourceFolderStr(CString& folders);
	BOOL	setDestFolderStr(CString& folder);


	BOOL	BeforePrepareRun();
	BOOL	isFileNeedMoreTime();
	//BOOL	isFileNeedExtType();

	CString getFileTypeString();

	CString destPathRoot;
	CStringArray sourcePaths;
	int		sourceFileNotDelete;//Դ�ļ��������Ƿ�ɾ��
	OVERWRITEMODE		overWriteMode;		//�������ǲ���
	int		destFolderRuleMode;		//�ļ�����������0��ʾ��ԭ���Ĳ��䡣
	int		destFilenameRuleMode;		//�ļ�����������0��ʾ��ԭ���Ĳ��䡣
	CString destFolderRuleString;
	CString destFilenameRuleString;
	CString filter;
	int		timeFromExif;
	int		timeFromFolder;
	int		timeFromFilename;
	int		yearRangeMin;
	int		yearRangeMax;
	BOOL	checkFileContent;

protected:
	void	SetConfig(CString& keyname, CString& value);
	CStringA combineKeyValue(const char* keyname, CString keyvalue);
	CStringA combineKeyValue(const char* keyname, int keyvalue);

	BOOL	m_bNeedMoreTime;
	//BOOL	m_bNeedExtType;
};

