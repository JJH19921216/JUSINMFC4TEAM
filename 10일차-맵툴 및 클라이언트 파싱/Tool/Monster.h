#pragma once
#include "Include.h"

class CToolView;
class CMonster
{
public:
	CMonster();
	~CMonster();
public:
	void		Initialize();
	void		Update();
	void		Render();
	void		Mini_Render();
	void		Release();
public:
	vector<Monster*>& Get_VecMonster() { return m_vecMonster; }

public:
	void		Set_MainView(CToolView* pView) { m_pMainView = pView; }
	void		Set_SubView(CToolView* pView) { m_pSubView = pView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

public:
	void		Add_Object(const D3DXVECTOR3& vPos, const BYTE& byMDrawID);
	int			Get_TileIdx(const D3DXVECTOR3& vPos);
public:
	vector<Monster*>		m_vecMonster;
	CToolView* m_pMainView = nullptr;
	CToolView* m_pSubView = nullptr;
	Monster					m_PreviewMonster;
public:
	void	   Preview_Render();
	void	   SetPreview(const D3DXVECTOR3& vPos, const BYTE& byMDrawID) { m_PreviewMonster.vPos = vPos; m_PreviewMonster.byMDrawID = byMDrawID; }

	void	   CLPreview_Render();
	void	   SetCLPreview(const BYTE& byMDrawID) { m_PreviewMonster.byMDrawID = byMDrawID; }

};

