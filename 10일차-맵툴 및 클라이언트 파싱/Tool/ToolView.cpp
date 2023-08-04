
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView() : m_pTerrain(nullptr), m_pObj(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
}

#pragma region ����

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����

#pragma endregion ����

void CToolView::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));

	// AfxGetMainWnd : ���� ���� �����츦 ��ȯ�ϴ� ���� �Լ�

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();
	
	RECT	rcWnd{};

	// GetWindowRect : ���� �������� ��Ʈ ������ ���� ����ϴ� �Լ�
	pMainFrm->GetWindowRect(&rcWnd);
	
	// SetRect : ������ �Ű� ������� ��Ʈ ������ ����ϴ� �Լ�
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	// GetClientRect : ���� view â�� ��Ʈ ������ ������ �Լ�
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos : �Ű� ���� ��� ���Ӱ� �������� ��ġ�� ũ�⸦ �����ϴ� �Լ�
	// SetWindowPos(��ġ�� �������� z����, x��ǥ, y��ǥ, ���� ũ��, ���� ũ��, ��ġ�� �������� ũ�� �� ���� ����(���� ������ �����ϰڴٴ� �ɼ�))

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

	m_pCollider = new CCollider;
	m_pCollider->Initialize();
	m_pCollider->Set_MainView(this);


	m_pLine = new CLine;
	m_dc = new CClientDC(this);
}

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CDevice::Get_Instance()->Render_Begin();

	m_pTerrain->Render();
	m_pObj->Render();
	//m_pLine->Render(pDC);

	if (g_ObjEdit)
		m_pObj->Preview_Render();

	CDevice::Get_Instance()->Render_End();

	m_pObj->ColliderRender();
}

void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete(m_pTerrain);
	Safe_Delete(m_pObj);

	Safe_Delete(m_pLine);

	CTextureMgr::Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	CMapTool*		pMapTool = &(pMyForm->m_MapTool);
	
	if (g_TileEdit)
		m_pTerrain->Tile_Change(D3DXVECTOR3((float)point.x + GetScrollPos(0)* g_Ratio,
											(float)point.y + GetScrollPos(1)* g_Ratio, 0.f), pMapTool->m_iDrawID);

	else if (g_ObjEdit)
	{
		m_pObj->Add_Object(D3DXVECTOR3(((float)point.x + GetScrollPos(0)) / g_Ratio,
			((float)point.y + GetScrollPos(1)) / g_Ratio, 0.f), pMapTool->m_iDrawID);
		m_pCollider->Add_Collider(D3DXVECTOR3(((float)point.x + GetScrollPos(0)) / g_Ratio,
			((float)point.y + GetScrollPos(1)) / g_Ratio, 0.f), pMapTool->m_iDrawID);
	}
		
	// Invalidate : ȣ�� ��, �������� WM_PAINT�� WM_ERASEBKGND �޼����� �߻���Ŵ
	// FALSE : WM_PAINT �޼����� �߻�
	// TRUE : WM_PAINT, WM_ERASEBKGND �޼����� �� �� �߻�

	Invalidate(FALSE);
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));    // ������ �� ����
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
		m_pLine->SetStartPoint(point);
	}
	m_dc->SelectObject(oldPen);
	
	
	
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);
	
	if (g_TileEdit || g_ObjEdit)
	{
		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
		CMapTool* pMapTool = &(pMyForm->m_MapTool);

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (g_TileEdit)
				m_pTerrain->Tile_Change(D3DXVECTOR3((float)point.x + GetScrollPos(0) / g_Ratio,
					(float)point.y + GetScrollPos(1) / g_Ratio, 0.f), pMapTool->m_iDrawID);

			

			CPen pen;
			pen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));    // ������ �� ����
			CPen* oldPen = m_dc->SelectObject(&pen);
			
			if (g_LineEdit)
			{
				m_dc->SetROP2(R2_XORPEN);
				m_dc->MoveTo(m_pLine->GetStartPoint().x, m_pLine->GetStartPoint().y);
				m_dc->LineTo(m_pLine->GetEndPoint().x, m_pLine->GetEndPoint().y);
				m_pLine->SetEndPoint(point);
				m_dc->MoveTo(m_pLine->GetStartPoint().x, m_pLine->GetStartPoint().y);
				m_dc->LineTo(m_pLine->GetEndPoint().x, m_pLine->GetEndPoint().y);
				Invalidate(FALSE);
			}
			m_dc->SelectObject(oldPen);
		
		}
		else if (g_ObjEdit)
		{
			m_pObj->SetPreview(D3DXVECTOR3(((float)point.x + GetScrollPos(0)) / g_Ratio,
				((float)point.y + GetScrollPos(1)) / g_Ratio, 0.f), pMapTool->m_iDrawID);
		}
		
			

			

		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
		
		Invalidate(FALSE);
		pMiniView->Invalidate(FALSE);
	}
}


void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
