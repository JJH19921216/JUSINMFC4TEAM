#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"ReadImgPath failed");
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
	
	CScrollMgr::Get_Instance()->Set_Scroll(WINCX * 0.5f, WINCY * 0.5f);

	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
	ScrollScene();
}

void CStage::Render_Scene()
{
	CObjMgr::Get_Instance()->Render();
}

void CStage::Release_Scene()
{
}

void CStage::ScrollScene()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);	// 마우스 위치 값을 얻어오는 함수
	ScreenToClient(g_hWnd, &ptMouse); // 스크린 상의 좌표를 우리가 생성한 창 좌표로 변환

	if ((ptMouse.x <= WINCX && ptMouse.y <= WINCY && ptMouse.x >= 0 && ptMouse.y >= 0) && !(ptMouse.x > 50.f && ptMouse.x < WINCX - 50.f && ptMouse.y > 50.f && ptMouse.y < WINCY - 50.f))
	{
		D3DXVECTOR3 vMouseDir = { ptMouse.x - WINCX * 0.5f, ptMouse.y - WINCY * 0.5f, 0.f };
		D3DXVec3Normalize(&vMouseDir, &vMouseDir);
		int	iScrollX = int(CScrollMgr::Get_Instance()->Get_ScollX());
		int	iScrollY = int(CScrollMgr::Get_Instance()->Get_ScollY());

		CScrollMgr::Get_Instance()->Set_ScrollX(vMouseDir.x * 500.f * CTimeMgr::Get_Instance()->Get_TimeDelta());

		CScrollMgr::Get_Instance()->Set_ScrollY(vMouseDir.y * 500.f * CTimeMgr::Get_Instance()->Get_TimeDelta());

	}
}
