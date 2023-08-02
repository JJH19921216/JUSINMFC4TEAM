#pragma once
#include "ColliderTool.h"


// CCollideView 보기

class CCollideView : public CScrollView
{
	friend class CColliderTool;

	DECLARE_DYNCREATE(CCollideView)

protected:
	CCollideView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CCollideView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

public:/*
		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName
			, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd
			, UINT nID, CCreateContext * pContext = NULL);*/



	//virtual CDocument* GetDocument() const;
	DECLARE_MESSAGE_MAP()
};


