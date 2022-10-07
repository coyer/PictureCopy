
// PictureCopyDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CPictureCopyDlg �Ի���


IMPLEMENT_DYNAMIC(CPictureCopyDlg, CDialogEx);

CPictureCopyDlg::CPictureCopyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PICTURECOPY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CPictureCopyDlg::~CPictureCopyDlg()
{
	// ����öԻ������Զ���������
	//  ���˴���ָ��öԻ���ĺ���ָ������Ϊ NULL���Ա�
	//  �˴���֪���öԻ����ѱ�ɾ����
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


// CPictureCopyDlg ��Ϣ�������

BOOL CPictureCopyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPictureCopyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPictureCopyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���û��ر� UI ʱ������������Ա���������ĳ��
//  �������Զ�����������Ӧ�˳���  ��Щ
//  ��Ϣ�������ȷ����������: �����������ʹ�ã�
//  ������ UI�������ڹرնԻ���ʱ��
//  �Ի�����Ȼ�ᱣ�������

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
	// �����������Ա�����������Զ���
	//  �������Իᱣ�ִ�Ӧ�ó���
	//  ʹ�Ի���������������� UI ����������
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
	Log(0, _T("�������ý���"));
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
		Log(0, "���������ú�ͬ�����������");
		OnBnClickedButtonConfig();
		return;
	}
	Log(0, "��ʼ�ļ��鵵��������");
	BOOL ret = m_worker.start();
	setButtonStatus();
	
	if (ret) {
		Log(0, _T("��������ɹ����̴߳�����..."));
	}
	else {
		Log(0, _T("��������ʧ�ܣ�"));
	}
}


void CPictureCopyDlg::OnBnClickedBtnStop()
{
	Log(0, _T("�鵵ֹͣ�����Ѿ�����"));
	m_worker.stop(TRUE);
	setButtonStatus();
	Log(0, _T("�鵵����ֹͣ���"));
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
		Log(0, _T("��Ϊ����ԭ���ֹͣ��ͬ������"));
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
//	// TODO: �ڴ˴������Ϣ����������
//}


void CPictureCopyDlg::OnDblclkListInfo()
{
	CString str;
	int sel = m_lstInfo.GetCurSel();
	m_lstInfo.GetText(sel, str);

	//����һ��HGLOBAL�����������ָ�������ڴ��
	if (OpenClipboard())
	{
		HGLOBAL hClip;
		EmptyClipboard(); //���������������
		hClip = GlobalAlloc(GMEM_MOVEABLE, str.GetLength() + 1);
		//�ڶ��Ϸ�����ƶ����ڴ�飬���򷵻�һ���ڴ���
		TCHAR * buff; //����ָ���ַ��͵�ָ�����
		buff = (TCHAR*)GlobalLock(hClip);
		//�Է�����ڴ����м��������ڴ����ת����һ��ָ��,������Ӧ�����ü�������1
		_tcscpy(buff, str);
		//���û���������ݸ��Ƶ�ָ������У�ʵ���Ͼ��Ǹ��Ƶ�������ڴ����
		GlobalUnlock(hClip);
		//����д����ϣ����н����������������ü��������ּ�1
		SetClipboardData(CF_TEXT, hClip);
		//����������ݵ��ڴ�������������Դ������
		CloseClipboard();
	}
}
