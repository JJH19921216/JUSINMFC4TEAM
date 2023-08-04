
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;
bool	g_TileEdit = false;
bool	g_ObjEdit = false;
bool	g_LineEdit = false;
bool	g_MonsterEdit = false;
vector<LINE>  m_vecLineData;
// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() : m_pTerrain(nullptr), m_pObj(nullptr),m_pMonster(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

#pragma region 생략

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

#pragma endregion 생략

void CToolView::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));

	// AfxGetMainWnd : 현재 메인 윈도우를 반환하는 전역 함수

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();
	
	RECT	rcWnd{};

	// GetWindowRect : 현재 윈도우의 렉트 정보를 얻어와 기록하는 함수
	pMainFrm->GetWindowRect(&rcWnd);
	
	// SetRect : 지정한 매개 변수대로 렉트 정보를 기록하는 함수
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	// GetClientRect : 현재 view 창의 렉트 정보를 얻어오는 함수
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos : 매개 변수 대로 새롭게 윈도우의 위치와 크기를 조정하는 함수
	// SetWindowPos(배치할 윈도우의 z순서, x좌표, y좌표, 가로 크기, 세로 크릭, 배치할 윈도우의 크기 및 위지 지정(현재 순서를 유지하겠다는 옵션))

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);
	
	CScrollView::OnInitialUpdate();

	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Initialize()))
		AfxMessageBox(L"CDevice Initialize FAILED");


	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
		AfxMessageBox(L"CUBE IMG FAILED");


	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->Set_MainView(this);

	m_pObj = new CObj;
	m_pObj->Initialize();
	m_pObj->Set_MainView(this);

	m_pMonster = new CMonster;
	m_pMonster->Initialize();
	m_pMonster->Set_MainView(this);
	m_pMonster->Set_SubView(this);

	m_pLine = new CLine;
	m_pLine->SetMainView(this);
	m_dc = new CClientDC(this);

	SetTimer(0, 1, 0);
}

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
// 	CDevice::Get_Instance()->Get_Device()->Clear(0,
// 		NULL,
// 		D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
// 		D3DCOLOR_ARGB(255, 0, 0, 255),	// 백버퍼 색상
// 		1.f,		// z버퍼의 초기화 값
// 		0);			// 스텐실 버퍼의 초기화 값

	CDevice::Get_Instance()->Render_Begin();
	m_pTerrain->Render();
	m_pObj->Render();
	m_pMonster->Render();
	if (g_ObjEdit)
		m_pObj->Preview_Render();

	CDevice::Get_Instance()->Render_End();
	m_pLine->Render(m_vecLineData);

	CDevice::Get_Instance()->Get_Device()->Present(NULL, NULL, NULL, NULL);
}

void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete(m_pTerrain);
	Safe_Delete(m_pObj);
	Safe_Delete(m_pMonster);
	Safe_Delete(m_pLine);

	CTextureMgr::Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	CMapTool*		pMapTool = &(pMyForm->m_MapTool);
	CUnitTool*		pUnitTool = &(pMyForm->m_UnitTool);
	if (g_TileEdit)
		m_pTerrain->Tile_Change(D3DXVECTOR3((float)point.x + GetScrollPos(0)* g_Ratio,
											(float)point.y + GetScrollPos(1)* g_Ratio, 0.f), pMapTool->m_iDrawID);

	if (g_ObjEdit)
		m_pObj->Add_Object(D3DXVECTOR3(((float)point.x + GetScrollPos(0))/ g_Ratio,
									   ((float)point.y + GetScrollPos(1))/ g_Ratio, 0.f), pMapTool->m_iDrawID);
	if (g_MonsterEdit)
		m_pMonster->Add_Object(D3DXVECTOR3(((float)point.x + GetScrollPos(0)) / g_Ratio,
									   ((float)point.y + GetScrollPos(1)) / g_Ratio, 0.f), pUnitTool->m_iDrawID);

	// Invalidate : 호출 시, 윈도우의 WM_PAINT와 WM_ERASEBKGND 메세지를 발생시킴
	// FALSE : WM_PAINT 메세지만 발생
	// TRUE : WM_PAINT, WM_ERASEBKGND 메세지를 둘 다 발생

	Invalidate(FALSE);
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));    // 빨간색 펜 생성
	CPen* oldPen = m_dc->SelectObject(&pen);

	if (!g_LineEdit)
	{
	
		m_dc->MoveTo(point.x, point.y);
		m_pLine->SetStartPoint(point);
		
		m_pLine->SetEndPoint(point);
		// 		m_pLine->SetStartPoint(CPoint(((float)point.x + GetScrollPos(0)) / fRatio, ((float)point.y + GetScrollPos(1)) / fRatio));
		// 		m_pLine->SetEndPoint(CPoint(((float)point.x + GetScrollPos(0)) / fRatio, ((float)point.y + GetScrollPos(1)) / fRatio));
		Invalidate(FALSE);
	}
	else if (g_LineEdit)
	{
		cacheLine.tLpoint.x = point.x + GetScrollPos(0);
		cacheLine.tLpoint.y = point.y + GetScrollPos(1);
	}
	m_dc->SelectObject(oldPen);
	
	
	
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);
	
	if (g_TileEdit || g_ObjEdit || g_LineEdit || g_MonsterEdit)
	{
		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
		CMapTool* pMapTool = &(pMyForm->m_MapTool);
		CUnitTool* pUnitTool = &(pMyForm->m_UnitTool);
		
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			if (g_TileEdit)
				m_pTerrain->Tile_Change(D3DXVECTOR3((float)point.x + GetScrollPos(0) / g_Ratio,
					(float)point.y + GetScrollPos(1) / g_Ratio, 0.f), pMapTool->m_iDrawID);

			if (g_MonsterEdit)
				m_pMonster->SetPreview(D3DXVECTOR3((float)point.x + GetScrollPos(0) / g_Ratio,
					(float)point.y + GetScrollPos(1) / g_Ratio, 0.f), pUnitTool->m_iDrawID);

			if (g_ObjEdit)
			{
				m_pObj->SetPreview(D3DXVECTOR3(((float)point.x + GetScrollPos(0)) / g_Ratio,
					((float)point.y + GetScrollPos(1)) / g_Ratio, 0.f), pMapTool->m_iDrawID);
			}
			
			


			mouseEndPoint = point;

			CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
			Invalidate(FALSE);

			pMiniView->Invalidate(FALSE);
		}

	}
}


void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX * g_Ratio, TILEY * TILECY * g_Ratio / 2.f));

	switch (nChar) {
	case VK_UP:
		g_Ratio += 0.1f;
		break;
	case VK_DOWN:
		if(g_Ratio > 0.1f)
			g_Ratio -= 0.1f;
		break;
	}
	Invalidate(FALSE);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(g_LineEdit)
	{
		cacheLine.tRpoint.x = point.x + GetScrollPos(0);
		cacheLine.tRpoint.y = point.y + GetScrollPos(1);
		m_vecLineData.push_back(cacheLine);
	}
	CScrollView::OnLButtonUp(nFlags, point);

	Invalidate(FALSE);
}


