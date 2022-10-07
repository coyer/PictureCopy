// DlgInputName.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureCopy.h"
#include "DlgInputName.h"
#include "afxdialogex.h"


// CDlgInputName 对话框

IMPLEMENT_DYNAMIC(CDlgInputName, CDialog)

CDlgInputName::CDlgInputName(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_INPUT, pParent)
	, m_strName(_T(""))
{

}

CDlgInputName::~CDlgInputName()
{
}

void CDlgInputName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(CDlgInputName, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInputName::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputName 消息处理程序


void CDlgInputName::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialog::OnOK();
}


BOOL CDlgInputName::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
