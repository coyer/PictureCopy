#pragma once
/*
*	单个文件处理类
*	作者：coyer@126.com
*/

typedef struct {
	int year;
	int month;
	int day;
	int dayOfWeek;
}FileDateInfo;

class CFileDealer
{
private:
	CFileDealer();
public:
	/*
	* basePath: 要拷贝的原始目录，
	* path：相对basePath，目前的子目录
	* filename：文件名，包含了扩展名
	*/
	CFileDealer(CString& basePath, CString& path, CString filename);
	~CFileDealer();

	void		SetFilesize(__int64 size);
	void		SetCreateTime(FILETIME createTime);
	void		SetUpdateTime(FILETIME updateTime);

	BOOL		SetMaybeTime(int year, int month, int day, int dayOfWeek);
	BOOL		SetMaybeTime(FileDateInfo& dateInfo);

	BOOL	ParseMoreTimeInfo(BOOL fromExif, BOOL fromFilename, BOOL fromFolder);
	BOOL	ParseTimeFromString(CString& pureName, FileDateInfo* pDateInfo);
	BOOL	ParseExifInfo();

	//BOOL	ParseStringTime(CString strNumber, FileDateInfo* pDateInfo);
	BOOL	ParseFileExtType();

	BOOL	ReplaceStringVariant(CString& strVar, BOOL isPathMode);
	// 判断文件是否存在
	static BOOL IsFileExist(LPCTSTR csFile);
	static BOOL IsDirExist(LPCTSTR csDir);
	static BOOL IsPathExist(LPCTSTR csPath);
	static BOOL IsPathExistEx(LPCTSTR csPath);

	CString&	GetFileExt() { return m_extName; }
	CString&	GetFileExtType() { return m_styleType; }
	CString&	GetFileDevice() { return m_exifDevice; }
	CString&	GetFileAuthor() { return m_exifAuthor; }
	CString&	GetFullPathname() { return m_fullPathname; }
	CString&	GetPath() { return m_path; }
	CString&	GetFilename() { return m_filename; }
	CString&	GetPureFilename() { return m_pureName; }
	CString&	GetSourceFolder() { return m_srcPath; }
	__int64		GetFilesize() { return m_fileSize; }
	__int64		GetUpdateTime() { return m_updatetime; }
private:
	CString  m_fullPathname;
	CString  m_srcPath;
	CString  m_path;
	CString  m_filename;
	CString  m_extName;
	CString  m_pureName;
	FileDateInfo m_createTime;
	FileDateInfo m_dateInfo;
	__int64	m_updatetime;

	CString	m_exifDevice;
	CString  m_exifAuthor;
	__int64 m_fileSize;
	CString m_styleType;

	
};

