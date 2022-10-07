
// PictureCopyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureCopy.h"
#include "PictureCopyDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "DlgConfiger.h"
#include "Configer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPictureCopyDlg 对话框


IMPLEMENT_DYNAMIC(CPictureCopyDlg, CDialogEx);

CPictureCopyDlg::CPictureCopyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PICTURECOPY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CPictureCopyDlg::~CPictureCopyDlg()
{
	// 如果该对话框有自动化代理，则
	//  将此代理指向该对话框的后向指针设置为 NULL，以便
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CPictureCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_lstInfo);
}

BEGIN_MESSAGE_MAP(CPictureCopyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SYNC_MSG, &CPictureCopyDlg::OnSyncMsg)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CPictureCopyDlg::OnBnClickedButtonConfig)
	ON_BN_CLICKED(ID_START, &CPictureCopyDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CPictureCopyDlg::OnBnClickedBtnStop)
	ON_WM_SIZE()
//	ON_WM_SIZING()
	ON_LBN_DBLCLK(IDC_LIST_INFO, &CPictureCopyDlg::OnDblclkListInfo)
END_MESSAGE_MAP()


// CPictureCopyDlg 消息处理程序

BOOL CPictureCopyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CString configFile;
	GetModuleFileName(NULL, configFile.GetBuffer(MAX_PATH), MAX_PATH);
	configFile.ReleaseBuffer();

	int pos = configFile.ReverseFind(_T('\\'));
	configFile = configFile.Left(pos);
	configFile += _T("\\config.ini");

	if (!CCopyWorker::checkFileExist(configFile)) {
		configFile = theApp.GetProfileString(_T("PictureCopy"), _T("config"), _T(""));
	}

	CConfiger* pConfig = CConfiger::GetInstance();
	pConfig->loadFromFile(configFile);

	m_worker.init(pConfig, this);
	setButtonStatus();

	if (!pConfig->isReady()) {
		OnBnClickedButtonConfig();
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPictureCopyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPictureCopyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPictureCopyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。  这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。

void CPictureCopyDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CPictureCopyDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CPictureCopyDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CPictureCopyDlg::CanExit()
{
	if (m_worker.isRunning()) return FALSE;
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


void CPictureCopyDlg::OnBnClickedButtonConfig()
{
	//if (m_worker.isRunning()) {
	//	return;
	//}
	Log(0, _T("打开了配置界面"));
	CDlgConfiger dlg;
	dlg.m_pConfig = CConfiger::GetInstance();
	dlg.m_isRunning = m_worker.isRunning();
	dlg.DoModal();
}


void CPictureCopyDlg::OnBnClickedStart()
{
	CConfiger* pConfig = CConfiger::GetInstance();
	ClearLog();
	if (!pConfig->isReady()) {
		Log(0, "请您先配置好同步任务的需求！");
		OnBnClickedButtonConfig();
		return;
	}
	Log(0, "开始文件归档整理任务");
	BOOL ret = m_worker.start();
	setButtonStatus();
	
	if (ret) {
		Log(0, _T("启动任务成功，线程处理中..."));
	}
	else {
		Log(0, _T("启动任务失败！"));
	}
}


void CPictureCopyDlg::OnBnClickedBtnStop()
{
	Log(0, _T("归档停止命令已经发送"));
	m_worker.stop(TRUE);
	setButtonStatus();
	Log(0, _T("归档任务停止完成"));
}

void CPictureCopyDlg::setButtonStatus()
{
	BOOL running = m_worker.isRunning();
	GetDlgItem(ID_START)->EnableWindow(!running);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(running);
}

void CPictureCopyDlg::ClearLog()
{
	m_lstInfo.ResetContent();
}

void CPictureCopyDlg::Log(int level, const char* msg)
{
	CString str(msg);
	m_lstInfo.AddString(str);
}

void CPictureCopyDlg::Log(int level, const wchar_t* msg) {
	m_lstInfo.AddString(msg);
}

void CPictureCopyDlg::Log(int level, CString& msg) {
	m_lstInfo.AddString(msg);
}

LRESULT CPictureCopyDlg::OnSyncMsg(UINT msgType, LPARAM lParam) {
	switch (msgType)
	{
	case SYNCT_STARTED:
		setButtonStatus();
		break;
	case SYNCT_NEEDSTOP:
		m_worker.stop(TRUE);
		break;
	case SYNCT_MSG:
		Log(1, (LPCTSTR)lParam);
		break;
	case SYNCT_MSG_NEEDFREE:
		Log(1, (LPCTSTR)lParam);
		free((void*)lParam);
		break;
	case SYNCT_ERROR:
		m_worker.stop(0);
		Log(0, _T("因为错误原因而停止了同步任务"));
		break;
	case SYNCT_STOPED:
		setButtonStatus();
		break;
	default:
		break;
	}
	return 0;
}

void CPictureCopyDlg::UpdateItemPos(int ID, int cx) {
	CRect rc;
	CWnd* pBtn = GetDlgItem(ID);
	pBtn->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.MoveToX(cx - 15 - rc.Width());
	pBtn->SetWindowPos(0, rc.left, rc.top, 0, 0, SWP_NOSIZE);
}

void CPictureCopyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc;
	CWnd* pStartBtn = GetDlgItem(ID_START);
	if (!pStartBtn) return;
	pStartBtn->GetWindowRect(&rc);

	UpdateItemPos(ID_START, cx);
	UpdateItemPos(IDC_BTN_STOP, cx);
	UpdateItemPos(IDC_BUTTON_CONFIG, cx);

	m_lstInfo.SetWindowPos(NULL, 0, 0, cx - rc.Width() - 45, cy - 30, SWP_NOMOVE);
}


//void CPictureCopyDlg::OnSizing(UINT fwSide, LPRECT pRect)
//{
//	CDialogEx::OnSizing(fwSide, pRect);
//
//	// TODO: 在此处添加消息处理程序代码
//}


void CPictureCopyDlg::OnDblclkListInfo()
{
	CString str;
	int sel = m_lstInfo.GetCurSel();
	m_lstInfo.GetText(sel, str);

	//定义一个HGLOBAL句柄变量用来指向分配的内存块
	if (OpenClipboard())
	{
		HGLOBAL hClip;
		EmptyClipboard(); //将剪贴板内容清空
		hClip = GlobalAlloc(GMEM_MOVEABLE, str.GetLength() + 1);
		//在堆上分配可移动的内存块，程序返回一个内存句柄
		TCHAR * buff; //定义指向字符型的指针变量
		buff = (TCHAR*)GlobalLock(hClip);
		//对分配的内存块进行加锁，将内存块句柄转化成一个指针,并将相应的引用计数器加1
		_tcscpy(buff, str);
		//将用户输入的数据复制到指针变量中，实际上就是复制到分配的内存块中
		GlobalUnlock(hClip);
		//数据写入完毕，进行解锁操作，并将引用计数器数字减1
		SetClipboardData(CF_TEXT, hClip);
		//将存放有数据的内存块放入剪贴板的资源管理中
		CloseClipboard();
	}
}
