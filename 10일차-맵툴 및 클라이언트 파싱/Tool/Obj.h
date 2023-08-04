#pragma once

#include "Include.h"

class CToolView;
class CCollideView;
class CObj
{
public:
	CObj();
	~CObj();

public:
	void		Initialize();
	void		Update();
	void		Render();
	void		Mini_Render();
	void		Release();

public:
	vector<OBJ*>& Get_VecObj() { return m_vecObj; }

public:
	void		Set_MainView(CToolView* pView) { m_pMainView = pView; }
	void		Set_SubView(CCollideView* pView) { m_pSubView = pView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

public:
	void		Add_Object(const D3DXVECTOR3& vPos, const BYTE& byDrawID);
	int			Get_TileIdx(const D3DXVECTOR3& vPos);
public:
	vector<OBJ*>		m_vecObj;
	CToolView*			m_pMainView = nullptr;
	CCollideView*		m_pSubView = nullptr;
	OBJ					m_PreviewObj;
public:
	void	   Preview_Render();
	void	   SetPreview(const D3DXVECTOR3& vPos, const BYTE& byDrawID) { m_PreviewObj.vPos = vPos; m_PreviewObj.byDrawID = byDrawID; }
	
	void	   CLPreview_Render();
	void	   SetCLPreview(const BYTE& byDrawID) {m_PreviewObj.byDrawID = byDrawID; }
	void	   ColliderRender();

	vector<OBJ*>	m_vecRedolist;
	vector<OBJ*>	m_vecRender;
	void		ObjRedo();
	void		ObjUndo();
};

