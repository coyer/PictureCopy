// FileStyleDefine.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureCopy.h"
#include "DlgFileStyleDefine.h"
#include "afxdialogex.h"
#include "DlgInputName.h"
#include "FileStyleJudge.h"

// CDlgFileStyleDefine 对话框

IMPLEMENT_DYNAMIC(CDlgFileStyleDefine, CDialogEx)

CDlgFileStyleDefine::CDlgFileStyleDefine(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_STYLE_DEFINE, pParent)
{
	m_lastCursel = -1;
}

CDlgFileStyleDefine::~CDlgFileStyleDefine()
{
}

void CDlgFileStyleDefine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STYLENAME, m_lstStyles);
}


BEGIN_MESSAGE_MAP(CDlgFileStyleDefine, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TYPE, &CDlgFileStyleDefine::OnBnClickedButtonAddType)
	ON_BN_CLICKED(IDC_BUTTON_DEL_TYPE, &CDlgFileStyleDefine::OnBnClickedButtonDelType)
	ON_LBN_SELCHANGE(IDC_LIST_STYLENAME, &CDlgFileStyleDefine::OnSelchangeListStylename)
//	ON_LBN_SELCANCEL(IDC_LIST_STYLENAME, &CDlgFileStyleDefine::OnSelcancelListStylename)
//	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgFileStyleDefine::OnBnClickedButtonUpdate)
ON_EN_KILLFOCUS(IDC_EDIT_EXTEND_DESC, &CDlgFileStyleDefine::OnKillfocusEditExtendDesc)
ON_BN_CLICKED(IDOK, &CDlgFileStyleDefine::OnBnClickedOk)
ON_LBN_DBLCLK(IDC_LIST_STYLENAME, &CDlgFileStyleDefine::OnDblclkListStylename)
END_MESSAGE_MAP()


// CDlgFileStyleDefine 消息处理程序


void CDlgFileStyleDefine::OnBnClickedButtonAddType()
{
	CDlgInputName dlg;
	if (dlg.DoModal() != IDOK) return;
	CSingleType* p = CFileStyleJudge::GetInstance()->GetStyle(dlg.m_strName);
	if (p) {
		return;
	}
	m_lstStyles.AddString(dlg.m_strName);
	CFileStyleJudge::GetInstance()->AddStyle(dlg.m_strName);
}

void CDlgFileStyleDefine::OnBnClickedButtonDelType()
{
	int index = m_lstStyles.GetCurSel();
	m_lstStyles.DeleteString(index);
	CFileStyleJudge::GetInstance()->DeleteStyle(index);
}


BOOL CDlgFileStyleDefine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFileStyleJudge* pJudge = CFileStyleJudge::GetInstance();
	int n = pJudge->GetCount();
	for (int i = 0; i < n; i++) {
		m_lstStyles.AddString(pJudge->GetAt(i)->styleName);
	}
	
	m_lstStyles.SetCurSel(0);
	m_lastCursel = 0;
	UpdateExtText(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgFileStyleDefine::UpdateExtText(BOOL savemode)
{
	BOOL ret = FALSE;
	CString strText;
	if (!savemode) {
		if (m_lastCursel >= 0)
		{
			CFileStyleJudge* p = CFileStyleJudge::GetInstance();
			strText = p->GetExtString(m_lastCursel);
			ret = TRUE;
		}
		SetDlgItemText(IDC_EDIT_EXTEND_DESC, strText);
	}
	else {
		if (m_lastCursel >= 0) {
			GetDlgItemText(IDC_EDIT_EXTEND_DESC, strText);
			CFileStyleJudge* p = CFileStyleJudge::GetInstance();
			p->UpdateExtText(m_lastCursel, strText);
			ret = TRUE;
		}
	}
	return ret;
}

void CDlgFileStyleDefine::OnSelchangeListStylename()
{
	int cursel = m_lstStyles.GetCurSel();
	if (cursel == m_lastCursel) return;

	UpdateExtText(TRUE);
	m_lastCursel = cursel;
	UpdateExtText(FALSE);
}


//void CDlgFileStyleDefine::OnSelcancelListStylename()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}



void CDlgFileStyleDefine::OnKillfocusEditExtendDesc()
{
	UpdateExtText(TRUE);
}


void CDlgFileStyleDefine::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgFileStyleDefine::OnDblclkListStylename()
{
	int index = m_lstStyles.GetCurSel();
	CString oldName;
	m_lstStyles.GetText(index, oldName);

	CDlgInputName dlg;
	dlg.m_strName = oldName;
	if (dlg.DoModal() != IDOK) return;
	if (dlg.m_strName == oldName) return;

	CSingleType* p = CFileStyleJudge::GetInstance()->GetStyle(dlg.m_strName);
	if (p) {
		AfxMessageBox(_T("已经存在同名的类型，修改失败"));
		return;
	}

	if (!CFileStyleJudge::GetInstance()->UpdateStyleName(oldName, dlg.m_strName)) {
		AfxMessageBox(_T("修改失败，请检查输入的名称"));
		return;
	}
	m_lstStyles.DeleteString(index);
	m_lstStyles.InsertString(index, dlg.m_strName);
	m_lstStyles.SetCurSel(index);
}
