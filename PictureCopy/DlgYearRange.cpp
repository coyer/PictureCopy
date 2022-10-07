// DlgYearRange.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PictureCopy.h"
#include "DlgYearRange.h"
#include "afxdialogex.h"


// CDlgYearRange �Ի���

IMPLEMENT_DYNAMIC(CDlgYearRange, CDialog)

CDlgYearRange::CDlgYearRange(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_DATE_RANGE, pParent)
	, m_yearMin(0)
	, m_yearMax(0)
{

}

CDlgYearRange::~CDlgYearRange()
{
}

void CDlgYearRange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_YEAR_MIN, m_yearMin);
	DDX_Text(pDX, IDC_EDIT_YEAR_MAX, m_yearMax);
	DDV_MinMaxInt(pDX, m_yearMin, 1970, 2065);
	DDV_MinMaxInt(pDX, m_yearMax, 1990, 2100);
}


BEGIN_MESSAGE_MAP(CDlgYearRange, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgYearRange::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgYearRange ��Ϣ�������


void CDlgYearRange::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialog::OnOK();
}


BOOL CDlgYearRange::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
