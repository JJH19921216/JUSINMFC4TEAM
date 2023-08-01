#pragma once
#include "afxwin.h"
#include "FileInfo.h"

#include <atlimage.h>

// CObject 대화 상자

class CObject : public CDialog
{
	DECLARE_DYNAMIC(CObject)

public:
	CObject(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CObject();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CObject };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnListBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);

private:
	void	Horizontal_Scroll();

public:
	CListBox m_ListBox;
	CStatic m_Picture;

	map<CString, CImage*>	m_mapPngImg;
	int		m_iDrawID = 0;

	afx_msg void OnSaveData();
};
