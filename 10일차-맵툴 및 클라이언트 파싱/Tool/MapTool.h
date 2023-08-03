#pragma once
#include "afxwin.h"
#include "FileInfo.h"

#include <atlimage.h>

// CMapTool 대화 상자입니다.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
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
	virtual BOOL OnInitDialog();

	void GetResource(CString _Path, int _count);

	CButton m_TileButton;
	CButton m_ObjButton;
	afx_msg void OnTileButton();
	afx_msg void OnObjButton();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void		ResizeDialog(CImage* _img);
	CRect		OriginDialogRect;
	afx_msg void OnUndoButton();
	afx_msg void OnRedoButton();
};
