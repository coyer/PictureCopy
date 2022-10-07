#pragma once
/*
*	年份范围配置对话框
*	作者：coyer@126.com
*/

// CDlgYearRange 对话框

class CDlgYearRange : public CDialog
{
	DECLARE_DYNAMIC(CDlgYearRange)

public:
	CDlgYearRange(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgYearRange();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DATE_RANGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_yearMin;
	int m_yearMax;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
