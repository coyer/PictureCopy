
// DlgProxy.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PictureCopy.h"
#include "DlgProxy.h"
#include "PictureCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPictureCopyDlgAutoProxy

IMPLEMENT_DYNCREATE(CPictureCopyDlgAutoProxy, CCmdTarget)

CPictureCopyDlgAutoProxy::CPictureCopyDlgAutoProxy()
{
	EnableAutomation();
	
	// ΪʹӦ�ó������Զ��������ڻ״̬ʱһֱ���� 
	//	���У����캯������ AfxOleLockApp��
	AfxOleLockApp();

	// ͨ��Ӧ�ó����������ָ��
	//  �����ʶԻ���  ���ô�����ڲ�ָ��
	//  ָ��Ի��򣬲����öԻ���ĺ���ָ��ָ��
	//  �ô���
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CPictureCopyDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CPictureCopyDlg)))
		{
			m_pDialog = reinterpret_cast<CPictureCopyDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CPictureCopyDlgAutoProxy::~CPictureCopyDlgAutoProxy()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	//	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CPictureCopyDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CPictureCopyDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPictureCopyDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_IPictureCopy ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {64E78357-8EBA-434B-9679-A23060651AEA}
static const IID IID_IPictureCopy =
{ 0x64E78357, 0x8EBA, 0x434B, { 0x96, 0x79, 0xA2, 0x30, 0x60, 0x65, 0x1A, 0xEA } };

BEGIN_INTERFACE_MAP(CPictureCopyDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CPictureCopyDlgAutoProxy, IID_IPictureCopy, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {1728C39B-124F-4426-9257-F47BB6DA5EC8}
IMPLEMENT_OLECREATE2(CPictureCopyDlgAutoProxy, "PictureCopy.Application", 0x1728c39b, 0x124f, 0x4426, 0x92, 0x57, 0xf4, 0x7b, 0xb6, 0xda, 0x5e, 0xc8)


// CPictureCopyDlgAutoProxy ��Ϣ�������
