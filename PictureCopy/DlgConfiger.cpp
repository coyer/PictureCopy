// DlgConfiger.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureCopy.h"
#include "DlgConfiger.h"
#include "afxdialogex.h"
#include "DlgFileStyleDefine.h"
#include "DlgYearRange.h"

// CDlgConfiger 对话框

IMPLEMENT_DYNAMIC(CDlgConfiger, CDialogEx)

CDlgConfiger::CDlgConfiger(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONFIG, pParent)
	, m_strFilter(_T(""))
	, m_sourceMode(0)
	, m_existMode(0)
	, m_dstFolderRule(_T(""))
	, m_dstFileNameRule(_T(""))
	, m_dstFolderMode(FALSE)
	, m_dstFilenameMode(FALSE)
	, m_destFolderRoot(_T(""))
	, m_bTimeFromExif(FALSE)
	, m_bTimeFromFolder(FALSE)
	, m_bTimeFromFilename(FALSE)
{

}

CDlgConfiger::~CDlgConfiger()
{
}

void CDlgConfiger::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SOURCES, m_lstSource);
	DDX_Text(pDX, IDC_EDIT_FILTER, m_strFilter);
	DDX_Radio(pDX, IDC_RADIO_SRC_MODE, m_sourceMode);
	DDX_Radio(pDX, IDC_RADIO_EXIST_MODE, m_existMode);
	DDX_Text(pDX, IDC_EDIT_DEST_FOLDER, m_dstFolderRule);
	DDX_Text(pDX, IDC_EDIT_DEST_FILENAME, m_dstFileNameRule);
	DDX_Radio(pDX, IDC_RADIO_FOLDER_RULE, m_dstFolderMode);
	DDX_Radio(pDX, IDC_RADIO_FILENAME_RULE, m_dstFilenameMode);
	DDX_Text(pDX, IDC_EDIT_DEST, m_destFolderRoot);
	DDX_Check(pDX, IDC_CHECK_TIME_FROM_EXIF, m_bTimeFromExif);
	DDX_Check(pDX, IDC_CHECK_TIME_FROM_FOLDER, m_bTimeFromFolder);
	DDX_Check(pDX, IDC_CHECK_TIME_FROM_FILENAME, m_bTimeFromFilename);
}


BEGIN_MESSAGE_MAP(CDlgConfiger, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgConfiger::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DSTROOT_SELECT, &CDlgConfiger::OnBnClickedButtonDstrootSelect)
	ON_BN_CLICKED(IDC_BUTTON_SRC_ALL, &CDlgConfiger::OnBnClickedButtonSrcAll)
	ON_LBN_SELCHANGE(IDC_LIST_SOURCES, &CDlgConfiger::OnSelchangeListSources)
	ON_BN_CLICKED(IDC_BUTTON_SRC_DEL, &CDlgConfiger::OnBnClickedButtonSrcDel)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlgConfiger::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CDlgConfiger::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgConfiger::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_RADIO_FOLDER_RULE, &CDlgConfiger::OnClickedRadioFolderRule)
	ON_BN_CLICKED(IDC_RADIO_FOLDER_CUSTOM, &CDlgConfiger::OnBnClickedRadioFolderCustom)
	ON_BN_CLICKED(IDC_RADIO_FILENAME_RULE, &CDlgConfiger::OnBnClickedRadioFilenameRule)
	ON_BN_CLICKED(IDC_RADIO_FILENAME_CUSTOM, &CDlgConfiger::OnBnClickedRadioFilenameCustom)
	ON_BN_CLICKED(IDC_BUTTON_SAVEINI, &CDlgConfiger::OnBnClickedButtonSaveini)
	ON_BN_CLICKED(IDC_BUTTON_TYPE_DEFINE, &CDlgConfiger::OnBnClickedButtonTypeDefine)
	ON_BN_CLICKED(IDC_BUTTON_DATE_RANGE, &CDlgConfiger::OnBnClickedButtonDateRange)
END_MESSAGE_MAP()


// CDlgConfiger 消息处理程序

BOOL CDlgConfiger::UpdateUIData()
{
	if (!UpdateData()) return FALSE;

	m_pConfig->sourceFileNotDelete = m_sourceMode;
	m_pConfig->overWriteMode = m_existMode;

	if (!m_pConfig->setDestFolderStr(m_destFolderRoot)) {
		AfxMessageBox(_T("存储根目录不对，请重新选择"));
	}
	m_pConfig->sourcePaths.RemoveAll();
	int n = m_lstSource.GetCount();
	for (int i = 0; i < n; i++) {
		CString strTmp;
		m_lstSource.GetText(i, strTmp);
		m_pConfig->addSourceFolder(strTmp);
	}
	m_pConfig->filter = m_strFilter;
	m_pConfig->timeFromExif = m_bTimeFromExif;
	m_pConfig->timeFromFolder = m_bTimeFromFolder;
	m_pConfig->timeFromFilename = m_bTimeFromFilename;

	m_pConfig->destFolderRuleMode = m_dstFolderMode;
	if (!m_pConfig->setDestFolderRule(m_dstFolderRule)) {
		AfxMessageBox(_T("自定义文件夹中含有非法字符"));
	}

	m_pConfig->destFilenameRuleMode = m_dstFilenameMode;
	if (!m_pConfig->setDestFilenameRule(m_dstFileNameRule)) {
		AfxMessageBox(_T("自定义文件名中含有非法字符"));
	}

	return TRUE;
}
void CDlgConfiger::OnBnClickedOk()
{
	if (!UpdateUIData()) {
		AfxMessageBox(_T("参数错误，请检查"));
		return;
	}

	CDialogEx::OnOK();
}


BOOL CDlgConfiger::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDOK)->EnableWindow(!m_isRunning);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(!m_isRunning);
	if (m_isRunning) {
		SetWindowText(_T("软件运行中，不能修改配置文件！！！"));
	}

	for (int i = 0; i < m_pConfig->sourcePaths.GetCount(); i++) {
		m_lstSource.AddString(m_pConfig->sourcePaths.GetAt(i));
	}

	m_destFolderRoot = m_pConfig->destPathRoot;
	m_sourceMode = m_pConfig->sourceFileNotDelete;
	m_existMode = m_pConfig->overWriteMode;

	m_dstFolderRule = m_pConfig->getDestFolderRuleString();
	m_dstFileNameRule = m_pConfig->getDestFilenameRuleString();
	
	m_dstFolderMode = m_pConfig->destFolderRuleMode;
	m_dstFilenameMode = m_pConfig->destFilenameRuleMode;

	m_strFilter = m_pConfig->filter;
	m_bTimeFromExif = m_pConfig->timeFromExif;
	m_bTimeFromFolder = m_pConfig->timeFromFolder;
	m_bTimeFromFilename = m_pConfig->timeFromFilename;

	UpdateData(FALSE);

	GetDlgItem(IDC_EDIT_DEST_FOLDER)->EnableWindow(m_dstFolderMode);
	GetDlgItem(IDC_EDIT_DEST_FILENAME)->EnableWindow(m_dstFilenameMode);

	OnSelchangeListSources();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgConfiger::OnBnClickedButtonDstrootSelect()
{
	BROWSEINFO bi;
	TCHAR szPath[MAX_PATH];

	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath; //此参数如为NULL则不能显示对话框
	bi.lpszTitle = _T("请选择存储根目录");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	bi.lpfn = NULL;
	bi.iImage = 0;   //初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if (pIDList && SHGetPathFromIDList(pIDList, szPath))
	{
		SetDlgItemText(IDC_EDIT_DEST, szPath);
	}
}


void CDlgConfiger::OnBnClickedButtonSrcAll()
{
	BROWSEINFO bi;
	TCHAR szPath[MAX_PATH];

	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath; //此参数如为NULL则不能显示对话框
	bi.lpszTitle = _T("选择源文件目录");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	bi.lpfn = NULL;
	bi.iImage = 0;   //初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if (pIDList && SHGetPathFromIDList(pIDList, szPath))
	{
		//SetDlgItemText(IDC_EDIT_SOURCE, szPath);
		if (_tcslen(szPath) > 0) {
			m_lstSource.AddString(szPath);
		}
	}
}


void CDlgConfiger::OnSelchangeListSources()
{
	int sel = m_lstSource.GetCurSel();
	GetDlgItem(IDC_BUTTON_SRC_DEL)->EnableWindow(sel >= 0);
}


void CDlgConfiger::OnBnClickedButtonSrcDel()
{
	int sel = m_lstSource.GetCurSel();
	if (sel >= 0) m_lstSource.DeleteString(sel);
}


void CDlgConfiger::OnBnClickedButtonClear()
{
	m_lstSource.ResetContent();
}

void CDlgConfiger::OnBnClickedButtonOpen()
{
	CString strFilter = _T("配置文件 (*.ini)|*.ini| 文本文件 (*.txt)|*.txt| 所有文件 (*.txt)|*.txt||");
	CFileDialog dlg(TRUE, _T("*.ini"), _T("config.ini"), OFN_HIDEREADONLY, strFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		m_pConfig->loadFromFile(dlg.GetPathName());
	}
}

void CDlgConfiger::OnBnClickedButtonSave()
{
	if (!UpdateUIData()) {
		AfxMessageBox(_T("参数错误，请检查参数配置后再保存"));
		return;
	}
	CString strFilter = _T("配置文件 (*.ini)|*.ini| 文本文件 (*.txt)|*.txt| 所有文件 (*.txt)|*.txt||");
	CFileDialog dlg(FALSE, _T("*.ini"), _T("config.ini"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		m_pConfig->saveToFile(dlg.GetPathName());
	}
}


void CDlgConfiger::OnClickedRadioFolderRule()
{
	GetDlgItem(IDC_EDIT_DEST_FOLDER)->EnableWindow(FALSE);
}


void CDlgConfiger::OnBnClickedRadioFolderCustom()
{
	GetDlgItem(IDC_EDIT_DEST_FOLDER)->EnableWindow(TRUE);
}


void CDlgConfiger::OnBnClickedRadioFilenameRule()
{
	GetDlgItem(IDC_EDIT_DEST_FILENAME)->EnableWindow(FALSE);
}


void CDlgConfiger::OnBnClickedRadioFilenameCustom()
{
	GetDlgItem(IDC_EDIT_DEST_FILENAME)->EnableWindow(TRUE);
}


void CDlgConfiger::OnBnClickedButtonSaveini()
{
	if (!UpdateUIData()) {
		AfxMessageBox(_T("参数错误，请检查参数配置后再保存"));
		return;
	}

	CString configFile;
	GetModuleFileName(NULL, configFile.GetBuffer(MAX_PATH), MAX_PATH);
	configFile.ReleaseBuffer();

	int pos = configFile.ReverseFind(_T('\\'));
	configFile = configFile.Left(pos);
	configFile += _T("\\config.ini");

	m_pConfig->saveToFile(configFile);
}


void CDlgConfiger::OnBnClickedButtonTypeDefine()
{
	CDlgFileStyleDefine dlg;
	dlg.DoModal();
}


void CDlgConfiger::OnBnClickedButtonDateRange()
{
	CDlgYearRange dlg;
	dlg.m_yearMin = m_pConfig->yearRangeMin;
	dlg.m_yearMax = m_pConfig->yearRangeMax;
	if (dlg.DoModal() == IDOK) {
		m_pConfig->yearRangeMin = dlg.m_yearMin;
		m_pConfig->yearRangeMax = dlg.m_yearMax;
	}
}
