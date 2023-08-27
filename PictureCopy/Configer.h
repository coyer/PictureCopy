#pragma once
/*
*	配置文件
*	作者：coyer@126.com
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
	_T("<~>"),		//原始路径或者原始文件名
	_T("<TYPE>"),	//类型，audio，vidio，
	_T("<EXT>"),	//扩展名
	_T("<YYYY>"),   //年，4位
	_T("<YY>"),		//年，2位
	_T("<MM>"),		//月
	_T("<DD>"),		//日
	_T("<DEV>")		// exif 中的设备名，可能没有
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
	int		sourceFileNotDelete;//源文件拷贝后是否删除
	OVERWRITEMODE		overWriteMode;		//重名覆盖策略
	int		destFolderRuleMode;		//文件夹命名规则，0表示按原来的不变。
	int		destFilenameRuleMode;		//文件名命名规则，0表示按原来的不变。
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

