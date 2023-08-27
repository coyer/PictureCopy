#pragma once
/*
*	工作线程，真正干活的类
*	作者：coyer@126.com
*/

#define SYNCT_STARTED 1
#define SYNCT_NEEDSTOP 2
#define SYNCT_MSG 3
#define SYNCT_MSG_NEEDFREE 4
#define SYNCT_ERROR 5
#define SYNCT_STOPED 6


// CCopyWorker
#include "Configer.h"
class CFileDealer;

class CCopyWorker
{
public:
	CCopyWorker();           // 动态创建所使用的受保护的构造函数
	virtual ~CCopyWorker();

	BOOL	init(CConfiger* pConfig, CWnd* pWnd);


	void sendMsgInfo(CString& str);
	void sendMsgInfo(LPCTSTR str);
	void sendMsg(int type, LPARAM param = NULL);
	void running();

	BOOL isRunning();
	BOOL start();
	void stop(BOOL sendMsg);
	MYCPINFO* getStatInfo() { return &m_cpInfo; }
	CString getStatInfoStr();

	static BOOL checkFileExist(LPCTSTR filename);

protected:
	BOOL prepare();
	void listFiles(CString& baseSrc, CString folder);
	BOOL doMakeDir(CString foldername);
	BOOL copyFile(LPCTSTR src, LPCTSTR dst);
	BOOL checkFileExist(LPCTSTR filename, __int64* fileSize, __int64* updateTime);
	//BOOL getNewFileName(LPCTSTR szFilename, TCHAR* szNewFilename);
	CString getNewFilename(CString& filename);

	CString  getTaskDestFolder(CFileDealer* mfi);
	CString  getTaskDestFilename(CFileDealer* mfi);

	int doCopy(CFileDealer* fd);

protected:
	BOOL	m_bRunning;
	CWnd*	m_pWnd;
	CConfiger	* m_pConfig;

	HANDLE		m_thdHandle;
	MYCPINFO   m_cpInfo;
};


