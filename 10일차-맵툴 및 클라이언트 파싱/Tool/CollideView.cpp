// CollideView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CollideView.h"


// CCollideView

IMPLEMENT_DYNCREATE(CCollideView, CScrollView)

CCollideView::CCollideView()
{

}

CCollideView::~CCollideView()
{
}


BEGIN_MESSAGE_MAP(CCollideView, CScrollView)
END_MESSAGE_MAP()


// CCollideView 그리기

void CCollideView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}
//
//BOOL CCollideView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//{
//	return CWnd::Create(lpszClassName, lpszWindowName,
//		dwStyle, rect, pParentWnd, nID, pContext);
//}
//
//CDocument* CCollideView::GetDocument() const
//{
//	return (CDocument*)m_pDocument;
//}

void CCollideView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CCollideView 진단

#ifdef _DEBUG
void CCollideView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CCollideView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCollideView 메시지 처리기
