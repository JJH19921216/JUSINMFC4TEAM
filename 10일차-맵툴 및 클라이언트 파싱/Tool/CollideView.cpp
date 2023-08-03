// CollideView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CollideView.h"
#include "Device.h"


// CCollideView

IMPLEMENT_DYNCREATE(CCollideView, CScrollView)

CCollideView::CCollideView()
{

}

CCollideView::~CCollideView()
{
}


BEGIN_MESSAGE_MAP(CCollideView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CCollideView 그리기

void CCollideView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_pObj = new CObj;
	m_pObj->Initialize();
	m_pObj->Set_SubView(this);
	m_pObj->SetCLPreview(20);
	m_bDrawStart = false;
	m_bDrawEnd = false;;
	m_DC = new CClientDC(this);
}

void CCollideView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.


	CDevice::Get_Instance()->Render_Begin();
	m_pObj->CLPreview_Render();
	CDevice::Get_Instance()->Render_End(m_hWnd);
	
	ColliderRender();
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




void CCollideView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);
	CPen pen; 
	pen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));   
	CPen* oldPen = m_DC->SelectObject(&pen);

	if (!m_bDrawStart)
	{
		m_bDrawStart = true;
		StartPoint.x = point.x+ GetScrollPos(0);
		StartPoint.y = point.y+ GetScrollPos(1);
		EndPoint.x = point.x + GetScrollPos(0);
		EndPoint.y = point.y + GetScrollPos(1);

		if (m_CollideList.find(m_pObj->m_PreviewObj.byDrawID) != m_CollideList.end()) {
			m_CollideList[m_pObj->m_PreviewObj.byDrawID].clear();
			m_CollideList[m_pObj->m_PreviewObj.byDrawID].push_back({ (float)StartPoint.x,(float)StartPoint.y,0.f });
		}
		else
		{
			vector<D3DXVECTOR3> Temp;
			Temp.push_back({ (float)StartPoint.x,(float)StartPoint.y,0.f });
			m_CollideList.insert({ m_pObj->m_PreviewObj.byDrawID,Temp });
		}

	}
	else if (m_bDrawStart)
	{
		StartPoint.x = point.x + GetScrollPos(0);
		StartPoint.y = point.y + GetScrollPos(1);
		//EndPoint.x = point.x + GetScrollPos(0);
		//EndPoint.y = point.y + GetScrollPos(1);

		m_CollideList[m_pObj->m_PreviewObj.byDrawID].push_back({ (float)StartPoint.x,(float)StartPoint.y,0.f });
		
	}

	m_DC->SelectObject(oldPen);
}


void CCollideView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonUp(nFlags, point);
	
}


void CCollideView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);
	
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));   
	CPen* oldPen = m_DC->SelectObject(&pen);
	
	if (m_bDrawStart)
	{
		m_DC->SetROP2(R2_XORPEN);
		m_DC->MoveTo(CPoint{ (int)StartPoint.x - GetScrollPos(0), (int)StartPoint.y - GetScrollPos(1) });
		m_DC->LineTo(CPoint{(int)EndPoint.x - GetScrollPos(0), (int)EndPoint.y - GetScrollPos(1) });
		EndPoint.x = point.x + GetScrollPos(0);
		EndPoint.y = point.y + GetScrollPos(1);
		m_DC->MoveTo(CPoint{ (int)StartPoint.x - GetScrollPos(0), (int)StartPoint.y - GetScrollPos(1) });
		m_DC->LineTo(CPoint{ (int)EndPoint.x - GetScrollPos(0), (int)EndPoint.y - GetScrollPos(1) });

		if(point.x > 320.f)
			ScrollToPosition({ GetScrollPosition().x + 2,GetScrollPosition().y});
	}
	

	m_DC->SelectObject(oldPen);
}


void CCollideView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDblClk(nFlags, point);
	if (m_bDrawStart)
	{
		m_bDrawStart = false;		
		EndPoint.x = point.x + GetScrollPos(0);;
		EndPoint.y = point.y + GetScrollPos(1);;
		m_CollideList[m_pObj->m_PreviewObj.byDrawID].push_back({ (float)EndPoint.x,(float)EndPoint.y,0.f });

		Invalidate();
	}
}

void CCollideView::ColliderRender()
{
	if (m_CollideList.find(m_pObj->m_PreviewObj.byDrawID) != m_CollideList.end()) {
		
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		CPen* oldPen = m_DC->SelectObject(&pen);

		m_DC->SetROP2(R2_COPYPEN);
		m_DC->MoveTo(m_CollideList[m_pObj->m_PreviewObj.byDrawID][0].x - GetScrollPos(0), m_CollideList[m_pObj->m_PreviewObj.byDrawID][0].y - GetScrollPos(1));
		for (int i = 0; i < m_CollideList[m_pObj->m_PreviewObj.byDrawID].size() - 1; ++i)
		{
 			m_DC->LineTo(m_CollideList[m_pObj->m_PreviewObj.byDrawID][i + 1].x - GetScrollPos(0), m_CollideList[m_pObj->m_PreviewObj.byDrawID][i + 1].y - GetScrollPos(1));
		}
		m_DC->LineTo(m_CollideList[m_pObj->m_PreviewObj.byDrawID][0].x - GetScrollPos(0), m_CollideList[m_pObj->m_PreviewObj.byDrawID][0].y - GetScrollPos(1));

		m_DC->SelectObject(oldPen);
	}

}
