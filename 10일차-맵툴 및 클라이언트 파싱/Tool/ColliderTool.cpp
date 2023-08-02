// ColliderTool.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "ColliderTool.h"
#include "afxdialogex.h"
#include "CollideView.h"


// CColliderTool 대화 상자

IMPLEMENT_DYNAMIC(CColliderTool, CDialog)

CColliderTool::CColliderTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CColliderTool, pParent)
{

}

CColliderTool::~CColliderTool()
{
}

void CColliderTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CColliderTool, CDialog)
END_MESSAGE_MAP()


// CColliderTool 메시지 처리기


BOOL CColliderTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//CCreateContext pContext;
	//CWnd* pFrameWnd = this;

	////pContext.m_pCurrentDoc = new CDocument;
	//pContext.m_pNewViewClass = RUNTIME_CLASS(CCollideView);
	//CCollideView* pView =
	//	(CCollideView*)((CFrameWnd*)pFrameWnd)->CreateView(&pContext);
	//ASSERT(pView);
	//pView->ShowWindow(SW_NORMAL);
	///**
	//* After a view is created, resize that to
	//* have the same size as the dialog.
	//*/
	//CRect rectWindow;
	//rectWindow = CRect(10, 10, 100, 100);
	///**
	//* Leave a little space for border and title...
	//*/
	//pView->MoveWindow(rectWindow);

	//CCreateContext context;
	//context.m_pCurrentDoc = NULL; // 또는 사용할 문서를 설정
	//context.m_pNewViewClass = RUNTIME_CLASS(CCollideView);
	//CRect dialogRect;
	//GetClientRect(dialogRect); // 다이얼로그의 크기를 얻어옴
	//dialogRect.right -= 100.f;
	//dialogRect.bottom -= 100.f;

	//CCollideView* pView = (CCollideView*)((CFrameWnd*)this)->CreateView(&context);
	//if (pView)
	//{
	//	// 다이얼로그의 크기에 맞게 뷰의 위치와 크기 조정
	//	ASSERT(pView);
	//	pView->OnInitialUpdate();
	//	pView->MoveWindow(dialogRect);
	//	pView->ShowWindow(SW_NORMAL);
	//}

	CRect dialogRect;
	GetClientRect(dialogRect); // 다이얼로그의 크기를 얻어옴
	dialogRect.right -= 100.f;
	dialogRect.bottom -= 100.f;

	CRuntimeClass* pClass = RUNTIME_CLASS(CFrameWnd);
	CFrameWnd* m_pFrameWnd = (CFrameWnd*)pClass->CreateObject();
	m_pFrameWnd->Create(NULL, NULL, WS_CHILDWINDOW,
		dialogRect, this);
	m_pFrameWnd->ShowWindow(SW_SHOW);

	// Create a CScrollView
	pClass = RUNTIME_CLASS(CCollideView);
	m_CollideView = (CCollideView*)pClass->CreateObject();

	m_CollideView->SetScrollSizes(MM_TEXT, dialogRect.Size());
	m_CollideView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		dialogRect, m_pFrameWnd, AFX_IDW_PANE_FIRST + 1, NULL);
	m_CollideView->ShowWindow(SW_SHOW);
	m_CollideView->SendMessage(WM_INITIALUPDATE);

	m_pFrameWnd->SetWindowPos(NULL, 0, 0, dialogRect.Width(), dialogRect.Height(), 0);
	dialogRect.OffsetRect(-2, -2);
	m_CollideView->SetWindowPos(NULL, -1, -1, dialogRect.Width(), dialogRect.Height(), 0);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
