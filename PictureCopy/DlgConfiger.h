#pragma once
/*
*	配置文件UI
*	作者：coyer@126.com
*/

// CDlgConfiger 对话框
#include "Configer.h"
#include "afxwin.h"

class CDlgConfiger : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConfiger)

public:
	CDlgConfiger(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConfiger();

	CConfiger* m_pConfig;
	BOOL		m_isRunning;
	CToolTipCtrl m_tooltip;

	BOOL	UpdateUIData();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lstSource;
	CString m_strFilter;
	int m_sourceMode;
	int m_existMode;
	CString m_dstFolderRule;
	CString m_dstFileNameRule;
	BOOL m_dstFolderMode;
	BOOL m_dstFilenameMode;

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDstrootSelect();

	afx_msg void OnBnClickedButtonSrcAll();
	afx_msg void OnSelchangeListSources();
	afx_msg void OnBnClickedButtonSrcDel();
	afx_msg void OnBnClickedButtonClear();

	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonSave();

	CString m_destFolderRoot;
	afx_msg void OnClickedRadioFolderRule();
	afx_msg void OnBnClickedRadioFolderCustom();
	afx_msg void OnBnClickedRadioFilenameRule();
	afx_msg void OnBnClickedRadioFilenameCustom();
	afx_msg void OnBnClickedButtonSaveini();
	afx_msg void OnBnClickedButtonTypeDefine();
	BOOL m_bTimeFromExif;
	BOOL m_bTimeFromFolder;
	BOOL m_bTimeFromFilename;
	afx_msg void OnBnClickedButtonDateRange();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
