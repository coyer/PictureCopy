// DlgInputName.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PictureCopy.h"
#include "DlgInputName.h"
#include "afxdialogex.h"


// CDlgInputName �Ի���

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


// CDlgInputName ��Ϣ�������


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
				  // �쳣: OCX ����ҳӦ���� FALSE
}
