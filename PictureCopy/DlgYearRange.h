#pragma once
/*
*	��ݷ�Χ���öԻ���
*	���ߣ�coyer@126.com
*/

// CDlgYearRange �Ի���

class CDlgYearRange : public CDialog
{
	DECLARE_DYNAMIC(CDlgYearRange)

public:
	CDlgYearRange(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgYearRange();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DATE_RANGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_yearMin;
	int m_yearMax;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
