
// DlgProxy.cpp : 实现文件
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
	
	// 为使应用程序在自动化对象处于活动状态时一直保持 
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。  设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
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
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	//	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CPictureCopyDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CPictureCopyDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPictureCopyDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IPictureCopy 的支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {64E78357-8EBA-434B-9679-A23060651AEA}
static const IID IID_IPictureCopy =
{ 0x64E78357, 0x8EBA, 0x434B, { 0x96, 0x79, 0xA2, 0x30, 0x60, 0x65, 0x1A, 0xEA } };

BEGIN_INTERFACE_MAP(CPictureCopyDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CPictureCopyDlgAutoProxy, IID_IPictureCopy, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏在此项目的 StdAfx.h 中定义
// {1728C39B-124F-4426-9257-F47BB6DA5EC8}
IMPLEMENT_OLECREATE2(CPictureCopyDlgAutoProxy, "PictureCopy.Application", 0x1728c39b, 0x124f, 0x4426, 0x92, 0x57, 0xf4, 0x7b, 0xb6, 0xda, 0x5e, 0xc8)


// CPictureCopyDlgAutoProxy 消息处理程序
