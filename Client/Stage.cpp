#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Obj.h"
CStage::CStage()
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 36)))
	{
		ERR_MSG(L"Stage Tile Image Insert failed");
		return E_FAIL;
	}	

	CObj*	pObj = new CMyTerrain;
	
	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();	
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TERRAIN, pObj);

	// 플레이어
	/*pObj = new CPlayer;
	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::PLAYER, pObj);*/
	
	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);	// 마우스 위치 값을 얻어오는 함수

	ScreenToClient(g_hWnd, &ptMouse);
	int	iOffSetMinX = 100;
	int	iOffSetMaxX = 700;

	int	iOffSetMinY = 100;
	int	iOffSetMaxY = 500;
	float m_fSpeed = 3.f;

	if (iOffSetMinX >= ptMouse.x)
		CObj::m_vScroll.x+=m_fSpeed;

	if (iOffSetMaxX < ptMouse.x)
		CObj::m_vScroll.x -= m_fSpeed;

	if (iOffSetMinY >= ptMouse.y)
		CObj::m_vScroll.y += m_fSpeed;

	if (iOffSetMaxY < ptMouse.y)
		CObj::m_vScroll.y -= m_fSpeed;
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render_Scene()
{
	CObjMgr::Get_Instance()->Render();
}

void CStage::Release_Scene()
{
}
