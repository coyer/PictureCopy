#pragma once
/*
*	�������ֶԻ���
*	���ߣ�coyer@126.com
*/

// CDlgInputName �Ի���

class CDlgInputName : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputName)

public:
	CDlgInputName(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInputName();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
