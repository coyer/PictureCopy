#pragma once
/*
*	输入名字对话框
*	作者：coyer@126.com
*/

// CDlgInputName 对话框

class CDlgInputName : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputName)

public:
	CDlgInputName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInputName();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
