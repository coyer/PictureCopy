
// DlgProxy.h: ͷ�ļ�
//

#pragma once

class CPictureCopyDlg;


// CPictureCopyDlgAutoProxy ����Ŀ��

class CPictureCopyDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CPictureCopyDlgAutoProxy)

	CPictureCopyDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	CPictureCopyDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~CPictureCopyDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CPictureCopyDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

