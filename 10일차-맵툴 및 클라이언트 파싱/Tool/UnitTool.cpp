// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "ToolView.h"
#include "MainFrm.h"

// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strTemp(_T(""))
	, m_strSrc(_T(""))
	, m_strName(_T(""))
	, m_iHp(0)
	, m_iAttack(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), CDeleteMap());
	m_mapUnitData.clear();
	for_each(m_mapPngImg.begin(), m_mapPngImg.end(),
		[](auto& MyPair) {

			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});

	m_mapPngImg.clear();

}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTemp);
	DDX_Text(pDX, IDC_EDIT2, m_strSrc);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iHp);
	DDX_Text(pDX, IDC_EDIT5, m_iAttack);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);

	DDX_Control(pDX, IDC_BUTTON3, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
	DDX_Control(pDX, IDC_PICTURE2, m_Picture2);
	DDX_Control(pDX, IDC_LIST2, m_ListBox2);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)

	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnAddLine)

	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnCreateUnit)
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnSearchData)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnDeleteData)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON7, &CUnitTool::OnLoadData)
	ON_BN_CLICKED(IDC_BUTTON20, &CUnitTool::OnDeleteLine)
	ON_BN_CLICKED(IDC_BUTTON12, &CUnitTool::OnSaveLine)
	ON_BN_CLICKED(IDC_BUTTON11, &CUnitTool::OnLoadLine)
	ON_LBN_SELCHANGE(IDC_LIST2, &CUnitTool::OnCharcterListBox)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON13, &CUnitTool::OnMonsterButton)
 
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.





void CUnitTool::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString		strFindName;

	//GetCurSel : 리스트 박스에서 선택한 목록의 인덱스를 반환

	int		iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : 해당 인덱스의 문자열을 얻어오는 함수
	m_ListBox.GetText(iIndex, strFindName);

	auto		iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iHp	  = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);
	
	UpdateData(FALSE);

}


void CUnitTool::OnCreateUnit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	UNITDATA*	pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iHp = m_iHp;
	pUnit->iAttack = m_iAttack;

	for (int i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[2].GetCheck())
		pUnit->byItem |= SAPPHIRE;



	// AddString : 리스트 박스에 문자열을 추가
	m_ListBox.AddString(pUnit->strName);

	m_mapUnitData.insert({ pUnit->strName, pUnit });

	UpdateData(FALSE);
}



BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP		bBitmap = (HBITMAP)LoadImage(nullptr, 
												L"../Texture/JusinLogo1.bmp",
												IMAGE_BITMAP, 
												100, 
												50,
												LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_Bitmap.SetBitmap(bBitmap);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUnitTool::OnSearchData()
{
	UpdateData(TRUE);

	auto& iter = m_mapUnitData.find(m_strFindName);

	if (iter == m_mapUnitData.end())
		return;

	int		iIndex = m_ListBox.FindString(0, m_strFindName);

	if (LB_ERR == iIndex)
		return;

	m_ListBox.SetCurSel(iIndex);

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);

	UpdateData(FALSE);
}


void CUnitTool::OnDeleteData()
{
	UpdateData(TRUE);

	CString		strFindName = L"";

	int		iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : 해당 인덱스의 문자열을 얻어오는 함수
	m_ListBox.GetText(iIndex, strFindName);

	auto		iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);

	m_ListBox.DeleteString(iIndex);

	UpdateData(FALSE);
}


void CUnitTool::OnSaveData()
{
	// CFileDialog : 파일 열기 혹은 저장작업 필요한 대화상자를 생성하는 객체
	
	CFileDialog	Dlg(FALSE, // TRUE(열기), FALSE(다른 이름으로 저장) 모드 지정
					L"dat", // default 확장자명
					L"*.dat",  // 대화 상자에 표시할 최초 파일명
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기 전용 체크박스 숨김), OFN_OVERWRITEPROMPT(중복 저장 시 경고 메세지 띄움)
					L"Data File(*.dat) | *.dat||", // "콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열"
					this);	// 부모 윈도우 주소

	TCHAR		szPath[MAX_PATH] = L"";

	// GetCurrentDirectory: 현재 프로젝트가 있는 디렉토리 경로를 얻어옴(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);
	//szPath = 0x00afeb90 L"D:\\유준환\\139기\\Frame139\\Tool"
	
	// PathRemoveFileSpec : 전체 경로에서 파일 이름만 잘라내는 함수, 경로 상에 파일명이 없다면 제일 말단 폴더 이름을 잘라낸다.
	PathRemoveFileSpec(szPath);
	// szPath = 0x00afeb90 L"D:\\유준환\\139기\\Frame139"
	
	/* /, //, \, \\, | (token, 구분자) : 경로에서 폴더명 단위로 잘라내어 쓰거나 읽고자 할 때 구분하기 위한 기호 */

	lstrcat(szPath, L"\\Data");
	// szPath = 0x00afeb90 L"D:\\유준환\\139기\\Frame139\\Data"
	
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : 선택된 경로를 반환
		CString	str  = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;
		
		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;

		for (auto& MyPair : m_mapUnitData)
		{
			// key 값 저장
			dwStrByte = sizeof(TCHAR) * (MyPair.first.GetLength() + 1);
			
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, MyPair.first.GetString(), dwStrByte, &dwByte, nullptr);

			// value 값 저장

			WriteFile(hFile, &(MyPair.second->iAttack),		sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iHp),			sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->byJobIndex),	sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->byItem),		sizeof(BYTE), &dwByte, nullptr);

		}

		CloseHandle(hFile);

	}
}

/*
string strName;
char	szName[32];

strcpy_s(szName, strName.c_str());*/


void CUnitTool::OnLoadData()
{
	UpdateData(TRUE);

	CFileDialog	Dlg(TRUE, // TRUE(열기), FALSE(다른 이름으로 저장) 모드 지정
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

		for (auto& MyPair : m_mapUnitData)
			delete MyPair.second;

		m_mapUnitData.clear();

		// 리스트 박스 목록을 초기화하는 함수
		m_ListBox.ResetContent();
		
		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;
		UNITDATA	tData{};

	
		while(true)
		{
			// key 값 저장
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*	pName = new TCHAR[dwStrByte];

			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// value 값 저장

			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byItem), sizeof(BYTE), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				break;
			}

			UNITDATA*	pUnit = new UNITDATA;
			pUnit->strName = pName;
			delete[]pName;

			pUnit->iAttack = tData.iAttack;
			pUnit->iHp = tData.iHp;
			pUnit->byJobIndex = tData.byJobIndex;
			pUnit->byItem = tData.byItem;

			m_mapUnitData.insert({pUnit->strName, pUnit});

			m_ListBox.AddString(pUnit->strName);

		}

		CloseHandle(hFile);

	}

	UpdateData(FALSE);
}


void CUnitTool::OnDeleteLine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	g_LineEdit = false;
	g_MonsterEdit = false;
	if (m_vecLineData.size() > 0)
		m_vecLineData.pop_back();

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->Invalidate();

}


void CUnitTool::OnSaveLine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	g_LineEdit = false;
	g_MonsterEdit = false;

	CFileDialog	Dlg(FALSE, // TRUE(열기), FALSE(다른 이름으로 저장) 모드 지정
		L"dat", // default 확장자명
		L"*.dat",  // 대화 상자에 표시할 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기 전용 체크박스 숨김), OFN_OVERWRITEPROMPT(중복 저장 시 경고 메세지 띄움)
		L"Data File(*.dat) | *.dat||", // "콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열"
		this);	// 부모 윈도우 주소

	TCHAR		szPath[MAX_PATH] = L"";

	// GetCurrentDirectory: 현재 프로젝트가 있는 디렉토리 경로를 얻어옴(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);
	//szPath = 0x00afeb90 L"D:\\유준환\\139기\\Frame139\\Tool"

	// PathRemoveFileSpec : 전체 경로에서 파일 이름만 잘라내는 함수, 경로 상에 파일명이 없다면 제일 말단 폴더 이름을 잘라낸다.
	PathRemoveFileSpec(szPath);
	// szPath = 0x00afeb90 L"D:\\유준환\\139기\\Frame139"

	/* /, //, \, \\, | (token, 구분자) : 경로에서 폴더명 단위로 잘라내어 쓰거나 읽고자 할 때 구분하기 위한 기호 */

	lstrcat(szPath, L"\\Data");
	// szPath = 0x00afeb90 L"D:\\유준환\\139기\\Frame139\\Data"

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
		DWORD	dwStrByte = 0;

		for (auto& Myvec : m_vecLineData)
		{
			// value 값 저장
			WriteFile(hFile, &Myvec, sizeof(LINE), &dwByte, nullptr);

			//WriteFile(hFile, &(MyPair.second->Lpoint), sizeof(int), &dwByte, nullptr);
			//WriteFile(hFile, &(MyPair.second->iHp), sizeof(int), &dwByte, nullptr);
			
		}

		CloseHandle(hFile);

	}
	

}


void CUnitTool::OnLoadLine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	g_LineEdit = false;
	g_MonsterEdit = false;

	UpdateData(TRUE);

	CFileDialog	Dlg(TRUE, // TRUE(열기), FALSE(다른 이름으로 저장) 모드 지정
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

		for (auto& Myvec : m_vecLineData)
			delete &Myvec;

		m_vecLineData.clear();

		// 리스트 박스 목록을 초기화하는 함수
		m_ListBox.ResetContent();

		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;
		LINE	tLine {};


		while (true)
		{
			// key 값 저장
// 			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
// 
// 			TCHAR* pName = new TCHAR[dwStrByte];
// 
// 			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// value 값 저장

			ReadFile(hFile, &tLine, sizeof(LINE), &dwByte, nullptr);
			
			if (0 == dwByte)
			{

				break;
			}

			LINE* pLine = new LINE;
			pLine->tLpoint = tLine.tLpoint;
			pLine->tRpoint = tLine.tRpoint;
			m_vecLineData.push_back(*pLine);

		}

		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
		pMainView->Invalidate();

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
	

}
void CUnitTool::OnAddLine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	
	g_LineEdit = true;
	g_MonsterEdit = false;


	

}

void CUnitTool::OnCharcterListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString		strFindName;

	//GetCurSel : 리스트 박스에서 선택한 목록의 인덱스를 반환

	int		iIndex = m_ListBox2.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : 해당 인덱스의 문자열을 얻어오는 함수
	m_ListBox2.GetText(iIndex, strFindName);

	auto		iter = m_mapPngImg.find(strFindName);

	if (iter == m_mapPngImg.end())
		return;

	m_Picture2.SetBitmap(*(iter->second));

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

void CUnitTool::GetResource(CString _Path, int _count)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	TCHAR	szFilePath[MAX_PATH] = L"";
	TCHAR	szFileName[MAX_STR] = L"";

	//ListBox, m_mapPngImg 초기화 
	m_ListBox2.ResetContent();
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
			m_ListBox2.AddString(strFileName);
		}
	}

	Horizontal_Scroll();

	UpdateData(FALSE);
}

void CUnitTool::Horizontal_Scroll()
{
	CString	strName;
	CSize	Size;

	int	iWidth = 0;

	CDC* pDC = m_ListBox2.GetDC();

	for (int i = 0; i < m_ListBox2.GetCount(); ++i)
	{
		m_ListBox2.GetText(i, strName);

		// GetTextExtent : 현재 문자열의 길이를 픽셀 단위로 반환
		Size = pDC->GetTextExtent(strName);

		if (Size.cx > iWidth)
			iWidth = Size.cx;
	}

	m_ListBox2.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트 박스가 가로로 스크롤 할 수 있는 최대 범위를 얻어오는 함수
	if (iWidth > m_ListBox2.GetHorizontalExtent())
		m_ListBox2.SetHorizontalExtent(iWidth);

}


void CUnitTool::OnDropFiles(HDROP hDropInfo)
{
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
			m_ListBox2.AddString(strFileName);
		}
	}

	Horizontal_Scroll();

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(FALSE);
}


void CUnitTool::OnMonsterButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetResource(L"../Texture/Stage/Monster/Monster%d.png", 3);
	g_LineEdit = FALSE;
	g_MonsterEdit = TRUE;
}



void CUnitTool::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnClose();
	g_LineEdit = FALSE;
	g_MonsterEdit = FALSE;

}
