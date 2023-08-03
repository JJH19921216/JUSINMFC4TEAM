// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "ToolView.h"


// CUnitTool ��ȭ �����Դϴ�.

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
	ON_BN_CLICKED(IDC_BUTTON10, &CUnitTool::OnDeleteLine)
	ON_BN_CLICKED(IDC_BUTTON12, &CUnitTool::OnSaveLine)
	ON_BN_CLICKED(IDC_BUTTON11, &CUnitTool::OnLoadLine)
	ON_LBN_SELCHANGE(IDC_LIST2, &CUnitTool::OnCharcterListBox)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON13, &CUnitTool::OnMonsterButton)
 
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.





void CUnitTool::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CString		strFindName;

	//GetCurSel : ����Ʈ �ڽ����� ������ ����� �ε����� ��ȯ

	int		iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : �ش� �ε����� ���ڿ��� ������ �Լ�
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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



	// AddString : ����Ʈ �ڽ��� ���ڿ��� �߰�
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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

	// GetText : �ش� �ε����� ���ڿ��� ������ �Լ�
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
	// CFileDialog : ���� ���� Ȥ�� �����۾� �ʿ��� ��ȭ���ڸ� �����ϴ� ��ü
	
	CFileDialog	Dlg(FALSE, // TRUE(����), FALSE(�ٸ� �̸����� ����) ��� ����
					L"dat", // default Ȯ���ڸ�
					L"*.dat",  // ��ȭ ���ڿ� ǥ���� ���� ���ϸ�
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(�б� ���� üũ�ڽ� ����), OFN_OVERWRITEPROMPT(�ߺ� ���� �� ��� �޼��� ���)
					L"Data File(*.dat) | *.dat||", // "�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ�"
					this);	// �θ� ������ �ּ�

	TCHAR		szPath[MAX_PATH] = L"";

	// GetCurrentDirectory: ���� ������Ʈ�� �ִ� ���丮 ��θ� ����(���� ���)
	GetCurrentDirectory(MAX_PATH, szPath);
	//szPath = 0x00afeb90 L"D:\\����ȯ\\139��\\Frame139\\Tool"
	
	// PathRemoveFileSpec : ��ü ��ο��� ���� �̸��� �߶󳻴� �Լ�, ��� �� ���ϸ��� ���ٸ� ���� ���� ���� �̸��� �߶󳽴�.
	PathRemoveFileSpec(szPath);
	// szPath = 0x00afeb90 L"D:\\����ȯ\\139��\\Frame139"
	
	/* /, //, \, \\, | (token, ������) : ��ο��� ������ ������ �߶󳻾� ���ų� �а��� �� �� �����ϱ� ���� ��ȣ */

	lstrcat(szPath, L"\\Data");
	// szPath = 0x00afeb90 L"D:\\����ȯ\\139��\\Frame139\\Data"
	
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : ���õ� ��θ� ��ȯ
		CString	str  = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;
		
		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;

		for (auto& MyPair : m_mapUnitData)
		{
			// key �� ����
			dwStrByte = sizeof(TCHAR) * (MyPair.first.GetLength() + 1);
			
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, MyPair.first.GetString(), dwStrByte, &dwByte, nullptr);

			// value �� ����

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

	CFileDialog	Dlg(TRUE, // TRUE(����), FALSE(�ٸ� �̸����� ����) ��� ����
		L"dat", // default Ȯ���ڸ�
		L"*.dat",  // ��ȭ ���ڿ� ǥ���� ���� ���ϸ�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(�б� ���� üũ�ڽ� ����), OFN_OVERWRITEPROMPT(�ߺ� ���� �� ��� �޼��� ���)
		L"Data File(*.dat) | *.dat||", // "�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ�"
		this);	// �θ� ������ �ּ�

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

		// ����Ʈ �ڽ� ����� �ʱ�ȭ�ϴ� �Լ�
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
			// key �� ����
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*	pName = new TCHAR[dwStrByte];

			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// value �� ����

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	g_LineEdit = false;

	if(m_vecLineData.size() > 0)
		m_vecLineData.pop_back();
	UpdateData(FALSE);
}


void CUnitTool::OnSaveLine()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	g_LineEdit = false;

	

}


void CUnitTool::OnLoadLine()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	g_LineEdit = false;

	

}
void CUnitTool::OnAddLine()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	
	g_LineEdit = true;


	

}

void CUnitTool::OnCharcterListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CString		strFindName;

	//GetCurSel : ����Ʈ �ڽ����� ������ ����� �ε����� ��ȯ

	int		iIndex = m_ListBox2.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : �ش� �ε����� ���ڿ��� ������ �Լ�
	m_ListBox2.GetText(iIndex, strFindName);

	auto		iter = m_mapPngImg.find(strFindName);

	if (iter == m_mapPngImg.end())
		return;

	m_Picture2.SetBitmap(*(iter->second));

	int i = 0;

	for (; i < strFindName.GetLength(); ++i)
	{
		// isdigit : �Ű� ������ �Ѱܹ��� ���� ���ڰ� ���� ������ �������� ���� ������ �������� �Ǻ��ϴ� �Լ�
		// ���� ������ ���ڶ� �Ǻ��� ��� 0�� �ƴ� ���� ��ȯ

		if (0 != isdigit(strFindName[i]))
			break;
	}
	// Delete(index, count) : �ε��� ��ġ�κ��� ī��Ʈ��ŭ ���ڸ� �����ϴ� �Լ�

	strFindName.Delete(0, i);

	m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);
}

void CUnitTool::GetResource(CString _Path, int _count)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);

	TCHAR	szFilePath[MAX_PATH] = L"";
	TCHAR	szFileName[MAX_STR] = L"";

	//ListBox, m_mapPngImg �ʱ�ȭ 
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

		// PathFindFileName : ��� �� ���� �̸��� ����� �Լ�
		CString	strFileName = PathFindFileName(strRelativePath);

		lstrcpy(szFileName, strFileName.GetString());

		//PathRemoveExtension : Ȯ���ڸ� �߶󳻴� �Լ�
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

		// GetTextExtent : ���� ���ڿ��� ���̸� �ȼ� ������ ��ȯ
		Size = pDC->GetTextExtent(strName);

		if (Size.cx > iWidth)
			iWidth = Size.cx;
	}

	m_ListBox2.ReleaseDC(pDC);

	// GetHorizontalExtent : ����Ʈ �ڽ��� ���η� ��ũ�� �� �� �ִ� �ִ� ������ ������ �Լ�
	if (iWidth > m_ListBox2.GetHorizontalExtent())
		m_ListBox2.SetHorizontalExtent(iWidth);

}


void CUnitTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	TCHAR	szFilePath[MAX_PATH] = L"";
	TCHAR	szFileName[MAX_STR] = L"";

	// DragQueryFile : ��ӵ� ������ ������ ������ �Լ�
	// 0xffffffff(-1)�� �� ��° �Ű� ������ ������ ��� ��ӵ� ������ ������ ��ȯ

	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString	strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);

		// PathFindFileName : ��� �� ���� �̸��� ����� �Լ�
		CString	strFileName = PathFindFileName(strRelativePath);

		lstrcpy(szFileName, strFileName.GetString());

		//PathRemoveExtension : Ȯ���ڸ� �߶󳻴� �Լ�
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetResource(L"../Texture/Stage/Monster/%d.png", 3);
	g_LineEdit = FALSE;
	g_MonsterEdit = TRUE;
}

