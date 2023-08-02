#pragma once
#include <Include.h>

// CColliderTool 대화 상자

class CCollideView;

class CColliderTool : public CDialog
{
	DECLARE_DYNAMIC(CColliderTool)

public:
	CColliderTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColliderTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CColliderTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CCollideView*		m_CollideView;
	virtual BOOL OnInitDialog();
};
