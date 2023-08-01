// CObject.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CObject.h"
#include "afxdialogex.h"
#include "ToolView.h"


// CObject 대화 상자

IMPLEMENT_DYNAMIC(CObject, CDialog)

CObject::CObject(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CObject, pParent)
{

}

CObject::~CObject()
{
}

void CObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObject, CDialog)
END_MESSAGE_MAP()


// CObject 메시지 처리기
