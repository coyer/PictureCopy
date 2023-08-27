// FilerCopyer.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureCopy.h"
#include "CopyWorker.h"
#include "FileDealer.h"

// CFilerCopyer

CCopyWorker::CCopyWorker()
{
	m_bRunning = FALSE;
	m_pConfig = NULL;
	m_pWnd = NULL;
}

CCopyWorker::~CCopyWorker()
{
}

BOOL CCopyWorker::init(CConfiger* pConfig, CWnd* pWnd) {
	if (isRunning()) return FALSE;

	m_pWnd = pWnd;
	m_pConfig = pConfig;

	return TRUE;
}

void CCopyWorker::sendMsgInfo(CString& str) {
	if (!m_pWnd) return;
	void* p = malloc((str.GetLength() + 1) * sizeof(TCHAR));
	memset(p, 0, (str.GetLength() + 1) * sizeof(TCHAR));
	memcpy(p, str.GetBuffer(), str.GetLength() * sizeof(TCHAR));
	m_pWnd->PostMessage(WM_SYNC_MSG, SYNCT_MSG_NEEDFREE, (LPARAM)p);
}

void CCopyWorker::sendMsgInfo(LPCTSTR str) {
	if (!m_pWnd) return;
	m_pWnd->PostMessage(WM_SYNC_MSG, SYNCT_MSG, (LPARAM)str);
}

void CCopyWorker::sendMsg(int type, LPARAM param) {
	if (!m_pWnd) return;
	m_pWnd->PostMessage(WM_SYNC_MSG, type, param);
}

BOOL CCopyWorker::isRunning() {
	return m_bRunning;
}

BOOL CCopyWorker::checkFileExist(LPCTSTR filename)
{
	DWORD dwAttrib = GetFileAttributes(filename);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

DWORD WINAPI gThreadFunc(void* p) {
	CCopyWorker *pThread = (CCopyWorker*)p;
	pThread->running();
	return 0;
}

BOOL CCopyWorker::start() {
	if (m_bRunning) {
		return FALSE;
	}
	m_bRunning = TRUE;
	if (!prepare()) {
		m_bRunning = FALSE;
		return FALSE;
	}
	sendMsgInfo(_T("开始启动工作线程..."));
	m_thdHandle = ::CreateThread(NULL, NULL, gThreadFunc, this, 0, 0);
	sendMsg(SYNCT_STARTED);
	return TRUE;
}

void CCopyWorker::stop(BOOL sm) {
	m_bRunning = FALSE;

	if (m_thdHandle) 
	{
		::WaitForSingleObject(m_thdHandle, 0);
		CloseHandle(m_thdHandle);
		m_thdHandle = NULL;
	}
	if (sm) {
		sendMsg(SYNCT_STOPED, 0);
	}
}

BOOL CCopyWorker::prepare()
{
	sendMsgInfo(_T("开始检查同步配置参数..."));
	if (m_pConfig->destPathRoot.GetLength() == 0) {
		sendMsgInfo(_T("目标根目录不能为空！"));
		return FALSE;
	}
	if (m_pConfig->sourcePaths.GetCount() == 0) {
		sendMsgInfo(_T("没有需要拷贝的目录！"));
		return FALSE;
	}

	int n = m_pConfig->sourcePaths.GetCount();
	for (int i = 0; i < n; i++) {
		CString source = m_pConfig->sourcePaths.GetAt(i);
		if (m_pConfig->destPathRoot == source) {
			sendMsgInfo(_T("目录错误，源目录和目标相同了！"));
			return FALSE;
		}
		if (m_pConfig->destPathRoot.Find(source) == 0) {
			sendMsgInfo(_T("源目录包含目标根目录，目标目录设置错误。"));
			return FALSE;
		}
	}
	m_pConfig->BeforePrepareRun();

	sendMsgInfo(_T("配置参数检查完毕，开始下一步..."));
	return TRUE;
}
#define PATH_LENGTH  2048

void CCopyWorker::running() {
	memset(&m_cpInfo, 0, sizeof(m_cpInfo));
	m_cpInfo.dwStart = ::GetTickCount();

	sendMsgInfo(_T("开始遍历源文件目录："));
	int n = m_pConfig->sourcePaths.GetCount();
	for (int i = 0; i < n; i++) {
		CString source = m_pConfig->sourcePaths.GetAt(i);
		{
			CString log;
			log.Format(_T("%d：同步目录： %s"), i+ 1, source);
			sendMsgInfo(log);
		}
		listFiles(source, _T(""));
	}

	sendMsgInfo(_T("所有目录同步完成了"));
	CString log;
	DWORD tc = GetTickCount() - m_cpInfo.dwStart;
	short seconds = tc / 1000 % 60;
	short minutes = tc / 1000 / 60 % 60;
	short hours = tc / 1000 / 60 / 60 % 24;

	sendMsgInfo(_T("-------------------  同步的文件信息  -------------------"));
	sendMsgInfo(_T("\nCopy info:"));
	log.Format(_T("文件夹数量: %d"), m_cpInfo.folderCount);
	sendMsgInfo(log);
	log.Format(_T("文件数量: %d"), m_cpInfo.fileCount);
	sendMsgInfo(log);
	log.Format(_T("拷贝数量: %d"), m_cpInfo.copyCount);
	sendMsgInfo(log);
	log.Format(_T("覆盖文件数: %d"), m_cpInfo.overCount);
	sendMsgInfo(log);
	log.Format(_T("相同文件数: %d"), m_cpInfo.existFile);
	sendMsgInfo(log);
	log.Format(_T("错误数量: %d"), m_cpInfo.errorCount);
	sendMsgInfo(log);

	log.Format(_T("消耗时间: %d:%02d:%02d"), hours, minutes, seconds);
	sendMsgInfo(log);

	sendMsg(SYNCT_NEEDSTOP);
}

CString CCopyWorker::getStatInfoStr()
{
	CString log;
	DWORD tc = GetTickCount() - m_cpInfo.dwStart;
	short seconds = tc / 1000 % 60;
	short minutes = tc / 1000 / 60 % 60;
	short hours = tc / 1000 / 60 / 60 % 24;

	log.Format(_T("文件夹数量: %d\n文件数量: %d\n拷贝数量: %d\n覆盖文件数: %d\n相同文件数: %d\n错误数量: %d\n消耗时间: %d:%02d:%02d"),
		m_cpInfo.folderCount, m_cpInfo.fileCount, m_cpInfo.copyCount, m_cpInfo.overCount, m_cpInfo.existFile, m_cpInfo.errorCount, hours, minutes, seconds);

	return log;
}

void CCopyWorker::listFiles(CString& baseSrc, CString folder)
{
	HANDLE hFind;
	WIN32_FIND_DATA findData;

	CString szSrc = baseSrc;
	if (folder.GetLength() > 0) {
		szSrc += folder;
	}

	szSrc += m_pConfig->filter;
	hFind = FindFirstFile(szSrc, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		CString log;
		log.Format(_T("源文件目录不存在： %s"), szSrc);
		sendMsgInfo(log);
		return;
	}

	m_cpInfo.folderCount++;

	do
	{
		// 忽略"."和".."两个结果 
		if (_tcscmp(findData.cFileName, _T(".")) == 0 || _tcscmp(findData.cFileName, _T("..")) == 0) {
			continue;
		}

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)    // 是否是目录 
		{
			CString tmpFolder = folder + findData.cFileName + _T("\\");
			listFiles(baseSrc, tmpFolder);
		}
		else {
			LARGE_INTEGER size;
			size.LowPart = findData.nFileSizeLow;
			size.HighPart = findData.nFileSizeHigh;

			CFileDealer filed(baseSrc, folder, findData.cFileName);
			if (!filed.ParseFileExtType()) {
				continue;
			}
			
			filed.SetFilesize(size.QuadPart);
			filed.SetUpdateTime(findData.ftLastWriteTime);

			if (m_pConfig->isFileNeedMoreTime()) {
				filed.ParseMoreTimeInfo(m_pConfig->timeFromExif, m_pConfig->timeFromFilename, m_pConfig->timeFromFolder);
			}
			doCopy(&filed);
		}
	} while (FindNextFile(hFind, &findData) && m_bRunning);

	::FindClose(hFind);
}

BOOL recuMakeDir(CString& folder) {
	if (::PathIsDirectory(folder)) return TRUE;

	int pos = folder.ReverseFind(_T('\\'));
	if (pos < 0) return FALSE;
	CString parent = folder.Left(pos);
	if (parent.GetLength() == 0) return FALSE;
	if (!recuMakeDir(parent)) return FALSE;
	return ::CreateDirectory(folder, 0);
}

BOOL CCopyWorker::doMakeDir(CString foldername) {
	if (foldername.GetLength() == 0) return FALSE;
	
	if (foldername.Right(1) == _T("\\")) {
		foldername = foldername.Left(foldername.GetLength() - 1);
	}

	return recuMakeDir(foldername);
}

BOOL CCopyWorker::checkFileExist(LPCTSTR filename, __int64* fileSize, __int64* updateTime) {
	BOOL ret = FALSE;
	HANDLE hFile = ::CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		if (fileSize) {
			LARGE_INTEGER size;
			::GetFileSizeEx(hFile, &size);
			*fileSize = size.QuadPart;
		}

		if (updateTime) {
			FILETIME ftCreate, ftAccess, ftModify;
			if (::GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify))
			{
				__int64 t = ((__int64)ftModify.dwHighDateTime << 32) + ftModify.dwLowDateTime;
				*updateTime = t;
			}
		}

		::CloseHandle(hFile);
		ret = TRUE;
	}

	return ret;
}

//BOOL CCopyWorker::getNewFileName(LPCTSTR szFilename, TCHAR* szNewFilename) {
//	TCHAR* ext = NULL;
//	TCHAR pureName[PATH_LENGTH] = { 0 };
//	TCHAR* p = _tcsrchr((TCHAR*)szFilename, _T('.'));
//
//	if (p) {
//		ext = p;
//		int len = p - szFilename;
//		memcpy(pureName, szFilename, len * sizeof(TCHAR));
//		//_tcscpy_s(pureName, len, szFilename);
//	}
//	else {
//		_tcscpy_s(pureName, szFilename);
//	}
//	int i = 1;
//	while (i && m_bRunning) {
//		TCHAR tmpFilename[PATH_LENGTH] = { 0 };
//		wsprintf(tmpFilename, _T("%s_%d.%s"), pureName, i++, ext);
//		if (!checkFileExist(tmpFilename)) {
//			//_tcscpy_s(szNewFilename, tmpFilename); //编译不过，换下面的方式
//			memcpy(szNewFilename, tmpFilename, sizeof(TCHAR) * (_tcslen(tmpFilename) + 1));
//			break;
//		}
//	}
//	return TRUE;
//}

CString CCopyWorker::getNewFilename(CString& filename)
{
	CString fileExt, pureName;
	int pos = filename.ReverseFind(_T('.'));
	if (pos >= 0) {
		pureName = filename.Left(pos);
		fileExt = filename.Mid(pos + 1);
	}
	int i = 1;
	while (i && m_bRunning) {
		CString tmpFilename;
		tmpFilename.Format(_T("%s_%d.%s"), pureName, i++, fileExt);
		if (!checkFileExist(tmpFilename, NULL, NULL)) return tmpFilename;
	}
	return filename;
}

CString  CCopyWorker::getTaskDestFolder(CFileDealer* mfi)
{
	if (m_pConfig->destFolderRuleMode == 0) return m_pConfig->destPathRoot + mfi->GetPath();
	CString strPath = m_pConfig->destFolderRuleString;
	mfi->ReplaceStringVariant(strPath, TRUE);
	return m_pConfig->destPathRoot + strPath;
}

CString CCopyWorker::getTaskDestFilename(CFileDealer* mfi)
{
	if (m_pConfig->destFilenameRuleMode == 0) return mfi->GetFilename();
	CString strFilename = m_pConfig->destFolderRuleString;
	mfi->ReplaceStringVariant(strFilename, FALSE);
	return strFilename + _T(".") + mfi->GetFileExt();
}

int CCopyWorker::doCopy(CFileDealer* fd) {
	CString src = fd->GetFullPathname();
	CString destFolder = getTaskDestFolder(fd);
	CString destFilename = getTaskDestFilename(fd);
	CString dest = destFolder + destFilename;
	CString log;

	BOOL destExist = FALSE;
	
	//1创建文件夹
	doMakeDir(destFolder);

	m_cpInfo.fileCount++;

	__int64 updateTime = 0;
	__int64 dstFileSize = 0;

	destExist = checkFileExist(dest, &dstFileSize, &updateTime);

	if (destExist) {
		log = src + _T(" ==>> ") + dest;
		m_cpInfo.existFile++;
		
		if (m_pConfig->checkFileContent)
		{
			BOOL isFileSame = FALSE;
			CFileDealer::CheckFileIsSame(src, dest, &isFileSame);
			if (isFileSame) {
				log = _T("存在同名文件，且内容完全相同，直接跳过: ") + log;
				sendMsgInfo(log);
				return 0;
			}
		}

		switch (m_pConfig->overWriteMode)
		{
		case 0: //skip
			log = _T("存在同名文件，跳过: ") + log;
			sendMsgInfo(log);
			return 0;
		case 1: //改名
			dest = getNewFilename(dest);
			log = _T("改名后拷贝: ") + dest;
			sendMsgInfo(log);
			destExist = FALSE;
			break;
		case 2: //直接覆盖
			log = _T("直接覆盖: ") + dest;
			sendMsgInfo(log);
			destExist = FALSE;
			break;
		case 3:	//保留新的
			if (fd->GetUpdateTime() <= updateTime) {
				log = _T("保留最近的，跳过: ") + log;
				sendMsgInfo(log);
				return 0;
			}
			break;
		case 4:	//保留旧的
			if (fd->GetUpdateTime() >= updateTime) {
				log = _T("保留旧文件，跳过: ") + log;
				sendMsgInfo(log);
				return 0;
			}
			break;
		case 5: //保留大的
			if (fd->GetFilesize() <= dstFileSize) {
				log = _T("保留大文件，跳过: ") + log;
				sendMsgInfo(log);
				return 0;
			}
			break;
		case 6: //保留小的
			if (fd->GetFilesize() >= dstFileSize) {
				log = _T("保留小文件，跳过: ") + log;
				sendMsgInfo(log);
				return 0;
			}
			break;
		default:
			log = _T("位置模式，跳过: ") + log;
			sendMsgInfo(log);
			return 0;
		}
	}

	if (!m_bRunning) {
		sendMsgInfo(_T("您取消了同步任务"));
		return -1;
	}
	BOOL copyok = copyFile(src, dest);
	CString copystr;
	if (copyok) {
		copystr = _T("成功：");
	}
	else {
		copystr = _T("失败：");
	}
	if (destExist && m_pConfig->overWriteMode != OVERWRITE) {
		m_cpInfo.overCount++;
		log = _T("覆盖文件") + copystr + src + _T(" ==>> ") + dest;
		sendMsgInfo(log);
	}
	else {
		m_cpInfo.copyCount++;
		log = _T("拷贝文件") + copystr + src + _T(" => ") + dest;
		sendMsgInfo(log);
	}
	return copyok;
}

BOOL CCopyWorker::copyFile(LPCTSTR src, LPCTSTR dst) {
	BOOL ret;
	if (m_pConfig->sourceFileNotDelete) {
		ret = ::CopyFile(src, dst, FALSE);
	}
	else {
		ret = ::MoveFile(src, dst);
	}

	return ret;
}
