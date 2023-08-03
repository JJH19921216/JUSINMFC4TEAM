// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"


// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
	for_each(m_mapPngImg.begin(), m_mapPngImg.end(), 
		[](auto& MyPair) {

		MyPair.second->Destroy();
		Safe_Delete(MyPair.second);
	});

	m_mapPngImg.clear();

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_RADIO1, m_TileButton);
	DDX_Control(pDX, IDC_RADIO2, m_ObjButton);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveData)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTool::OnTileButton)
	ON_BN_CLICKED(IDC_RADIO2, &CMapTool::OnObjButton)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnUndoButton)
	ON_BN_CLICKED(IDC_BUTTON10, &CMapTool::OnRedoButton)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnListBox()
{
	UpdateData(TRUE);

	CString		strFindName;

	//GetCurSel : 리스트 박스에서 선택한 목록의 인덱스를 반환

	int		iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : 해당 인덱스의 문자열을 얻어오는 함수
	m_ListBox.GetText(iIndex, strFindName);

	auto		iter = m_mapPngImg.find(strFindName);

	if (iter == m_mapPngImg.end())
		return;
	ResizeDialog((iter->second));
	m_Picture.SetBitmap(*(iter->second));

	int i = 0;

	for (; i < strFindName.GetLength(); ++i)
	{
		// isdigit : 매개 변수로 넘겨받은 단일 문자가 숫자 형태의 글자인지 문자 형태의 글자인지 판별하는 함수
		// 숫자 형태의 문자라 판별될 경우 0이 아닌 값을 반환

		if (0 != isdigit(strFindName[i]))
			break;
	}
	// Delete(index, count) : 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수

	strFindName.Delete(0, i);

	m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);

}

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	
	TCHAR	szFilePath[MAX_PATH] = L"";
	TCHAR	szFileName[MAX_STR] = L"";
	
	// DragQueryFile : 드롭된 파일의 정보를 얻어오는 함수
	// 0xffffffff(-1)을 두 번째 매개 변수로 전달할 경우 드롭된 파일의 개수를 반환

	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString	strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);

		// PathFindFileName : 경로 중 파일 이름만 남기는 함수
		CString	strFileName = PathFindFileName(strRelativePath);

		lstrcpy(szFileName, strFileName.GetString());
		
		//PathRemoveExtension : 확장자를 잘라내는 함수
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto	iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage* pPndImg = new CImage;
			//pPndImg = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"Tile", iter->byDrawID)->pTexture;
			pPndImg->Load(strRelativePath);

			m_mapPngImg.insert({ strFileName, pPndImg });
			m_ListBox.AddString(strFileName);
		}
	}

	Horizontal_Scroll();

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(FALSE);
}

void CMapTool::Horizontal_Scroll()
{
	CString	strName;
	CSize	Size;

	int	iWidth = 0;

	CDC*	pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// GetTextExtent : 현재 문자열의 길이를 픽셀 단위로 반환
		Size = pDC->GetTextExtent(strName);

		if (Size.cx > iWidth)
			iWidth = Size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트 박스가 가로로 스크롤 할 수 있는 최대 범위를 얻어오는 함수
	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);

}


void CMapTool::OnSaveData()
{
	CFileDialog	Dlg(FALSE, // TRUE(열기), FALSE(다른 이름으로 저장) 모드 지정
		L"dat", // default 확장자명
		L"*.dat",  // 대화 상자에 표시할 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기 전용 체크박스 숨김), OFN_OVERWRITEPROMPT(중복 저장 시 경고 메세지 띄움)
		L"Data File(*.dat) | *.dat||", // "콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열"
		this);	// 부모 윈도우 주소

	TCHAR		szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : 선택된 경로를 반환
		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;

		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView*		pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
		CTerrain*		pTerrain = pMainView->m_pTerrain;

		vector< TILE*>&		vecTile = pTerrain->Get_VecTile();

		if (vecTile.empty())
			return;

		for (auto& iter : vecTile)
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);


		CloseHandle(hFile);

	}
}


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTool::GetResource(CString _Path, int _count)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	TCHAR	szFilePath[MAX_PATH] = L"";
	TCHAR	szFileName[MAX_STR] = L"";
	
	//ListBox, m_mapPngImg 초기화 
	m_ListBox.ResetContent();
	for_each(m_mapPngImg.begin(), m_mapPngImg.end(),
		[](auto& MyPair) {

			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});
	m_mapPngImg.clear();

	for (int i = 0; i < _count; ++i)
	{
		swprintf_s(szFilePath, _Path, i);
		CString	strRelativePath = szFilePath;
		
		// PathFindFileName : 경로 중 파일 이름만 남기는 함수
		CString	strFileName = PathFindFileName(strRelativePath);

		lstrcpy(szFileName, strFileName.GetString());

		//PathRemoveExtension : 확장자를 잘라내는 함수
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto	iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage* pPndImg = new CImage;
			pPndImg->Load(strRelativePath);

			m_mapPngImg.insert({ strFileName, pPndImg });
			m_ListBox.AddString(strFileName);
		}
	}

	Horizontal_Scroll();


	GetWindowRect(OriginDialogRect);

	UpdateData(FALSE);
}


void CMapTool::OnTileButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetResource(L"../Texture/Stage/Map/Tile/%d.png", 215);
	g_TileEdit = TRUE;
	g_ObjEdit = FALSE;
}


void CMapTool::OnObjButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetResource(L"../Texture/Stage/Map/Object/%d.png", 61);
	g_TileEdit = FALSE;
	g_ObjEdit = TRUE;
}


void CMapTool::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnClose();
	g_TileEdit = FALSE;
	g_ObjEdit = FALSE;
}



//Dialog 열때마다 한번씩 실행
void CMapTool::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	GetResource(L"../Texture/Stage/Map/Tile/%d.png", 215);
	m_TileButton.SetCheck(TRUE);
	g_TileEdit = TRUE;
	g_ObjEdit = FALSE;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMapTool::ResizeDialog(CImage* _img)
{
	CRect dialogRect; 
	GetWindowRect(dialogRect);
		
	SetWindowPos(NULL, dialogRect.left, dialogRect.top, OriginDialogRect.Width()+ _img->GetWidth() * 1, OriginDialogRect.Height() + _img->GetHeight() * 0.5f, 0);
}


void CMapTool::OnUndoButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// AfxGetApp : 메인 쓰레드를 갖고 있는 현재 메인 app을 반환
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	//GetPane(행, 열) : 해당하는 창의 정보를 얻어오는 함수 
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	if (g_TileEdit)
	{
		CTerrain* pTerrain = pMainView->m_pTerrain;
		pTerrain->TileUndo(); 
		
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
		pMainView->Invalidate(FALSE);
		pMiniView->Invalidate(FALSE);
		
	}
	else if (g_ObjEdit)
	{
		CObj* pObj = pMainView->m_pObj;
		pObj->ObjUndo();
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
		pMainView->Invalidate(FALSE);
		pMiniView->Invalidate(FALSE);

	}

}


void CMapTool::OnRedoButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// AfxGetApp : 메인 쓰레드를 갖고 있는 현재 메인 app을 반환
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	//GetPane(행, 열) : 해당하는 창의 정보를 얻어오는 함수 
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	if (g_TileEdit)
	{
		CTerrain* pTerrain = pMainView->m_pTerrain;
		pTerrain->TileRedo();
		
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
		pMainView->Invalidate(FALSE);
		pMiniView->Invalidate(FALSE);

	}
	else if (g_ObjEdit)
	{
		CObj* pObj = pMainView->m_pObj;
		pObj->ObjRedo();
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
		pMainView->Invalidate(FALSE);
		pMiniView->Invalidate(FALSE);

	}

}
