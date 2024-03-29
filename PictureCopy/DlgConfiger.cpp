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
	, m_bCheckFileContent(FALSE)
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
	DDX_Check(pDX, IDC_CHECK_FILE_CONTENT, m_bCheckFileContent);
}


BEGIN_MESSAGE_MAP(CDlgConfiger, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgConfiger::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DSTROOT_SELECT, &CDlgConfiger::OnBnClickedButtonDstrootSelect)
	ON_BN_CLICKED(IDC_BUTTON_SRC_ADD, &CDlgConfiger::OnBnClickedButtonSrcAll)
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
	ON_BN_CLICKED(IDC_BTN_SOURCE_UP, &CDlgConfiger::OnBnClickedBtnSourceUp)
	ON_BN_CLICKED(IDC_BTN_SOURCE_DOWN, &CDlgConfiger::OnBnClickedBtnSourceDown)
END_MESSAGE_MAP()


// CDlgConfiger 消息处理程序

BOOL CDlgConfiger::UpdateUIData()
{
	if (!UpdateData()) return FALSE;

	m_pConfig->sourceFileNotDelete = m_sourceMode;
	m_pConfig->overWriteMode = (OVERWRITEMODE)m_existMode;

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
	//m_pConfig->filter = m_strFilter;
	m_pConfig->timeFromExif = m_bTimeFromExif;
	m_pConfig->timeFromFolder = m_bTimeFromFolder;
	m_pConfig->timeFromFilename = m_bTimeFromFilename;
	m_pConfig->checkFileContent = m_bCheckFileContent;

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
	EnableToolTips(TRUE);

	m_tooltip.Create(this);
	m_tooltip.SetMaxTipWidth(200);
	//设定文字的颜色
	m_tooltip.SetTipTextColor(RGB(0, 0, 255));

	//设定提示文字在控件上停留的时间
	m_tooltip.SetDelayTime(150);
	m_tooltip.Activate(TRUE);

	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_DEST), _T("您选择的目标文件目录"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_DSTROOT_SELECT), _T("点击后，弹出文件夹选择框作为目标目录"));
	m_tooltip.AddTool(GetDlgItem(IDC_LIST_SOURCES), _T("您选择的源文件目录"));
	
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_SRC_ADD), _T("增加一个源文件目录"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_SRC_DEL), _T("删除选择的源文件目录"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_CLEAR), _T("清除所有的源文件目录"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_SRC_MODE), _T("使用移动模式，拷贝完成后源文件不存在"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_SRC_MODE_REMAIN), _T("使用复制模式，源文件不被删除"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_TIME_FROM_EXIF), _T("JPG文件头中一般含有EXIF信息，从EXIF中读取日期和作者等信息"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_TIME_FROM_FILENAME), _T("尝试读取文件名称中的日期信息来作为文件日期，有可能把普通数字解析成日期"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_TIME_FROM_FOLDER), _T("尝试读取文件夹称中的日期信息来作为文件日期，有可能把普通数字解析成日期"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_DATE_RANGE), _T("限制文件的日期范围，有助于防止普通数字字符串被当成日期"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_FOLDER_RULE), _T("按照原来的子目录层级，在新文件夹中保持相同的目录"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_FOLDER_CUSTOM), _T("按照自定义规则来进行子文件夹的设置"));
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_DEST_FOLDER), _T("可用的定义有：<TYPE>，<EXT>，<DEV>，<AUTHOR>，<YYYY>，<YY>，<MM>，<DD>，<DAY>，<~>"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_FILENAME_RULE), _T("保留原来的文件名，不变更文件名"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_FILENAME_CUSTOM), _T("按照自定义规则来进行文件名称的改变，软件会自动保持后缀不变"));
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_DEST_FILENAME), _T("可用的定义有：<TYPE>，<EXT>，<DEV>，<AUTHOR>，<YYYY>，<YY>，<MM>，<DD>，<DAY>，<~>"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_EXIST_MODE), _T("如果同名文件已经存在，则跳过"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_RENAME), _T("自动在同名文件名后面添加_1，以示区分"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_OVERWRITE), _T("目标目录中，同名文件如果存在的话，会被覆盖，请慎重选择"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_SAVE_NEWER), _T("比较同名文件的日期，源文件日期新则覆盖目标文件"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_SAVE_OLDER), _T("比较同名文件的日期，源文件日期旧则覆盖目标文件"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_SAVE_BIGGER), _T("比较同名文件的文件大小，源文件大则覆盖目标文件"));
	m_tooltip.AddTool(GetDlgItem(IDC_RADIO_SAVE_SMALLER), _T("比较同名文件的文件大小，源文件小则覆盖目标文件"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_SAVEINI), _T("保存当前的配置到 config.ini 中去"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_TYPE_DEFINE), _T("进入文件类型设置"));
	m_tooltip.AddTool(GetDlgItem(IDOK), _T("更新所有的配置信息"));
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_FILTER), _T("只有在文件类型设置中的后缀文件才会被备份，请点击按钮进行设置"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_FILE_CONTENT), _T("如果要复制的文件名已经存在，对文件内容进行字节比对，确保完全相同的文件不重复备份。"));

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

	m_strFilter = m_pConfig->getFileTypeString();
	m_bTimeFromExif = m_pConfig->timeFromExif;
	m_bTimeFromFolder = m_pConfig->timeFromFolder;
	m_bTimeFromFilename = m_pConfig->timeFromFilename;
	m_bCheckFileContent = m_pConfig->checkFileContent;

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
	m_strFilter = m_pConfig->getFileTypeString();
	UpdateData(FALSE);
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


BOOL CDlgConfiger::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_tooltip.m_hWnd != NULL)
		m_tooltip.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgConfiger::OnBnClickedBtnSourceUp()
{
	int sel = m_lstSource.GetCurSel();
	if (sel < 1) return;
	CString strText;
	m_lstSource.GetText(sel, strText);
	m_lstSource.DeleteString(sel);
	m_lstSource.InsertString(sel - 1, strText);
	m_lstSource.SetCurSel(sel - 1);
}


void CDlgConfiger::OnBnClickedBtnSourceDown()
{
	int sel = m_lstSource.GetCurSel();
	if (sel < 0 || sel == m_lstSource.GetCount() - 1) return;
	CString strText;
	m_lstSource.GetText(sel, strText);
	m_lstSource.DeleteString(sel);
	m_lstSource.InsertString(sel + 1, strText);
	m_lstSource.SetCurSel(sel + 1);
}
