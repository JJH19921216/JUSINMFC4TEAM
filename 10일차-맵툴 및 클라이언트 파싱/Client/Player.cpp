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
			m_tInfo.vPos.x - CScrollMgr::Get_Instance()->Get_ScollX(),	// x������ ��ũ�� ��
			m_tInfo.vPos.y - CScrollMgr::Get_Instance()->Get_ScollY(),	// y������ ��ũ�� ��
			m_tInfo.vPos.z);

		matWorld = matScale * matTrans;

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Walk", m_tFrame.fFrame);

		float	fX = (pTexInfo->tImgInfo.Width) / 2.f;
		float	fY = (pTexInfo->tImgInfo.Height) / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, // ����� �̹��� �İ�ü
			nullptr, // ����� �̹��� ������ ���� RECT����ü ������, NULL�� ��� �̹����� 0,0 �������� ���
			&D3DXVECTOR3(fX, fY, 0.f), // ����� �̹����� �߽� �࿡ ���� VEC3 ����ü ������, NULL�� ��� 0, 0�� �߽� ��ǥ
			nullptr, // ��ġ ��ǥ�� ���� VEC3����ü ������, NULL�� ��� ��ũ�� �� 0, 0 ��ǥ ���
			D3DCOLOR_ARGB(255, 255, 255, 255));// ����� ���� �̹����� ���� ���� ��, ��� �� ���� ������ �ݿ�(0xffffffff�� �Ѱ��ָ� ���� ���� ����)

}

void CPlayer::Release(void)
{
}
