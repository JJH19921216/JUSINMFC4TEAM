#pragma once
#include "Include.h"

class CToolView;
class CCollideView;
class CObj;

class CCollider
{
public:
	CCollider();
	~CCollider();

public:
	void		Initialize();
	void		Update();
	void		Render();
	void		Mini_Render();
	void		Release();

public:
	void		Set_MainView(CToolView* pView) { m_pMainView = pView; }
	void		Set_SubView(CCollideView* pView) { m_pSubView = pView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

public:
	void		Add_Collider(const D3DXVECTOR3& vPos, const BYTE& byDrawID);
	void		Update_ObjCollider();
public:
	CToolView* m_pMainView = nullptr;
	CCollideView* m_pSubView = nullptr;
	CObj* m_pObj = nullptr;

public:
	map<BYTE, vector<D3DXVECTOR3>> m_ColliderInfoList;
};

