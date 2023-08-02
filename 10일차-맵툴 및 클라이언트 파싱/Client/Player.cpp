#include "stdafx.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "Device.h"

HRESULT CPlayer::Initialize(void)
{
	m_tFrame.fFrame = 0;
	m_tFrame.fMax = 12;
	m_tInfo.vPos = { WINCX * 0.5f, WINCY * 0.5f, 0.f };
	m_bMove = false;
	m_vDest = { WINCX * 0.5f, WINCY * 0.5f, 0.f };
	m_tInfo.vLook.x = 1.f;
    return E_NOTIMPL;
}

int CPlayer::Update(void)
{
	if (GetAsyncKeyState(MK_LBUTTON))
	{
		m_vDest = { ::Get_Mouse().x + CScrollMgr::Get_Instance()->Get_ScollX(),::Get_Mouse().y + CScrollMgr::Get_Instance()->Get_ScollY(),0.f };
		m_bMove = true;
		D3DXVec3Normalize(&m_tInfo.vDir, &(m_vDest - m_tInfo.vPos));
		if (m_tInfo.vDir.x < 0.f)
			m_tInfo.vLook.x = -1.f;
		else
			m_tInfo.vLook.x = 1.f;
	}

	if (m_bMove)
	{		
		m_tInfo.vPos += m_tInfo.vDir * 1.f;
		if (D3DXVec3Length(&(m_tInfo.vPos - m_vDest)) < 5.f)
			m_bMove = false;
		
	}

	CObj::Move_Frame();
    return 0;
}

void CPlayer::Late_Update(void)
{
}

void CPlayer::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR			szBuf[MIN_STR] = L"";
	int				iIndex = 0;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, m_tInfo.vLook.x, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x - CScrollMgr::Get_Instance()->Get_ScollX(),	// x축으로 스크롤 값
			m_tInfo.vPos.y - CScrollMgr::Get_Instance()->Get_ScollY(),	// y축으로 스크롤 값
			m_tInfo.vPos.z);

		matWorld = matScale * matTrans;

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Walk", m_tFrame.fFrame);

		float	fX = (pTexInfo->tImgInfo.Width) / 2.f;
		float	fY = (pTexInfo->tImgInfo.Height) / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, // 출력할 이미지 컴객체
			nullptr, // 출력할 이미지 영역에 대한 RECT구조체 포인터, NULL인 경우 이미지의 0,0 기준으로 출력
			&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 중심 축에 대한 VEC3 구조체 포인터, NULL인 경우 0, 0이 중심 좌표
			nullptr, // 위치 좌표에 대한 VEC3구조체 포인터, NULL인 경우 스크린 상 0, 0 좌표 출력
			D3DCOLOR_ARGB(255, 255, 255, 255));// 출력할 원본 이미지와 섞을 색상 값, 출력 시 섞은 색상이 반영(0xffffffff를 넘겨주면 원본 색상 유지)

}

void CPlayer::Release(void)
{
}
