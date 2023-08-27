#pragma once
#include "afxwin.h"
/*
*	�ļ���׺�����ж϶Ի���
*	���ߣ�coyer@126.com
*/

// CDlgFileStyleDefine �Ի���

class CDlgFileStyleDefine : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileStyleDefine)

public:
	CDlgFileStyleDefine(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFileStyleDefine();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STYLE_DEFINE };
#endif
	BOOL	UpdateExtText(BOOL savemode);

	void	RenameCursel();
	void	UpdateUI();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lstStyles;
	int		m_lastCursel;
	afx_msg void OnBnClickedButtonAddType();
	afx_msg void OnBnClickedButtonDelType();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListStylename();
//	afx_msg void OnSelcancelListStylename();
//	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnKillfocusEditExtendDesc();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDblclkListStylename();
	afx_msg void OnBnClickedBtnRename();
	afx_msg void OnBnClickedBtnAddDefault();
};
