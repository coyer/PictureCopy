
// PictureCopyDlg.h : ͷ�ļ�
/*
*	���Ի���
*	���ߣ�coyer@126.com
*/

#pragma once
#include "CopyWorker.h"

#include "afxwin.h"

class CPictureCopyDlgAutoProxy;


// CPictureCopyDlg �Ի���
class CPictureCopyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPictureCopyDlg);
	friend class CPictureCopyDlgAutoProxy;

// ����
public:
	CPictureCopyDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CPictureCopyDlg();

	CCopyWorker	m_worker;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICTURECOPY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CPictureCopyDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL	CanExit();
	void	setButtonStatus();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT  OnSyncMsg(UINT msgType, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

	void	UpdateItemPos(int ID, int cx);
public:
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedBtnStop();
	void Log(int level, const char* msg);
	void Log(int level, const wchar_t* msg);
	void Log(int level, CString& msg);
	void ClearLog();
	CListBox m_lstInfo;
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnDblclkListInfo();
};
