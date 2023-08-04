#include "stdafx.h"
#include "Monster.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
CMonster::CMonster()
{
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_PreviewMonster.vPos = { 0.f,0.f,0.f };
	m_PreviewMonster.byMDrawID = 0;
}

void CMonster::Update()
{
}

void CMonster::Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	for (auto& iter : m_vecMonster)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Stage", L"Monster", iter->byMDrawID);

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0),
			iter->vPos.y - m_pMainView->GetScrollPos(1),
			iter->vPos.z);

		matWorld = matScale * matTrans;

		RECT		rc{};

		// GetClientRect : ���� Ŭ���̾�Ʈ ������ ��Ʈ ������ ����
		GetClientRect(m_pMainView->m_hWnd, &rc);

		float fX = WINCX / float(rc.right - rc.left);
		float fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

		float		fCenterX = (pTexInfo->tImgInfo.Width) / 2.f;
		float		fCenterY = (pTexInfo->tImgInfo.Height) / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// �ؽ�ó ��ü �ּ�
			nullptr, // ����� �̹��� ������ ���� ��Ʈ ����ü �ּ�, null�ΰ�� �̹����� 0, 0�������� ���
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // ����� �̹����� �߽� �࿡ ���� vector3 ����ü ������, nullptr�� ��� 0, 0�� �߽� ��ǥ
			nullptr, // ��ġ ��ǥ�� ���� vector3 ����ü ���ξ�
			D3DCOLOR_ARGB(255, 255, 255, 255)); 	// ����� ���� �̹����� ���� ���� ��, ��� �� ���� ���� �ݿ�, 0xffffffff�� �Ѱ��ָ� ���� ���� ����

		swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			nullptr,	// ����� ��Ʈ�� �ּ�
			0,			// ���� �ɼ�
			D3DCOLOR_ARGB(255, 255, 255, 255));

		++iIndex;
	}
}

void CMonster::Mini_Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	for (auto& iter : m_vecMonster)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Stage", L"Monster", iter->byMDrawID);

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x,
			iter->vPos.y,
			iter->vPos.z);

		matWorld = matScale * matTrans;

		float		fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float		fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		Set_Ratio(&matWorld, 0.3f, 0.3f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// �ؽ�ó ��ü �ּ�
			nullptr, // ����� �̹��� ������ ���� ��Ʈ ����ü �ּ�, null�ΰ�� �̹����� 0, 0�������� ���
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // ����� �̹����� �߽� �࿡ ���� vector3 ����ü ������, nullptr�� ��� 0, 0�� �߽� ��ǥ
			nullptr, // ��ġ ��ǥ�� ���� vector3 ����ü ���ξ�
			D3DCOLOR_ARGB(255, 255, 255, 255));	// ����� ���� �̹����� ���� ���� ��, ��� �� ���� ���� �ݿ�, 0xffffffff�� �Ѱ��ָ� ���� ���� ����
	}
}

void CMonster::Release()
{
	for_each(m_vecMonster.begin(), m_vecMonster.end(), CDeleteObj());
	m_vecMonster.clear();
	m_vecMonster.shrink_to_fit();
}

void CMonster::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}

void CMonster::Add_Object(const D3DXVECTOR3& vPos, const BYTE& byMDrawID)
{
	Monster* pMonster = new Monster;

	pMonster->vPos = vPos;
	pMonster->vSize = { (float)TILECX, (float)TILECY, 0.f };
	pMonster->byMOption = 0;
	pMonster->byMDrawID = byMDrawID;

	m_vecMonster.push_back(pMonster);
}

int CMonster::Get_TileIdx(const D3DXVECTOR3& vPos)
{
	for (size_t index = 0; index < m_vecMonster.size(); ++index)
	{
		//if (Picking_Dot(vPos, index))
		//	return index;
	}

	return -1;
}

void CMonster::Preview_Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"Object", m_PreviewMonster.byMDrawID);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, m_PreviewMonster.vPos.x - m_pMainView->GetScrollPos(0),
		m_PreviewMonster.vPos.y - m_pMainView->GetScrollPos(1),
		m_PreviewMonster.vPos.z);

	matWorld = matScale * matTrans;

	RECT		rc{};

	// GetClientRect : ���� Ŭ���̾�Ʈ ������ ��Ʈ ������ ����
	GetClientRect(m_pMainView->m_hWnd, &rc);

	float fX = WINCX / float(rc.right - rc.left);
	float fY = WINCY / float(rc.bottom - rc.top);

	Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

	float		fCenterX = (pTexInfo->tImgInfo.Width) / 2.f;
	float		fCenterY = (pTexInfo->tImgInfo.Height) / 2.f;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// �ؽ�ó ��ü �ּ�
		nullptr, // ����� �̹��� ������ ���� ��Ʈ ����ü �ּ�, null�ΰ�� �̹����� 0, 0�������� ���
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // ����� �̹����� �߽� �࿡ ���� vector3 ����ü ������, nullptr�� ��� 0, 0�� �߽� ��ǥ
		nullptr, // ��ġ ��ǥ�� ���� vector3 ����ü ���ξ�
		D3DCOLOR_ARGB(100, 255, 255, 255)); 	// ����� ���� �̹����� ���� ���� ��, ��� �� ���� ���� �ݿ�, 0xffffffff�� �Ѱ��ָ� ���� ���� ����
}

void CMonster::CLPreview_Render()
{
// 	D3DXMATRIX	matWorld, matScale, matTrans;
// 
// 	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Stage", L"Monster", m_PreviewMonster.byDrawID);
// 
// 	m_pSubView->SetScrollSizes(MM_TEXT, CSize(pTexInfo->tImgInfo.Width, pTexInfo->tImgInfo.Height));
// 
// 	float		fCenterX = (pTexInfo->tImgInfo.Width) / 2.f;
// 	float		fCenterY = (pTexInfo->tImgInfo.Height) / 2.f;
// 
// 	D3DXMatrixIdentity(&matWorld);
// 	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
// 	D3DXMatrixTranslation(&matTrans, fCenterX - m_pSubView->GetScrollPos(0),
// 		fCenterY - m_pSubView->GetScrollPos(1),
// 		0.f);
// 
// 	matWorld = matScale * matTrans;
// 
// 	RECT		rc{};
// 
// 	// GetClientRect : ���� Ŭ���̾�Ʈ ������ ��Ʈ ������ ����
// 	GetClientRect(m_pSubView->m_hWnd, &rc);
// 
// 	float fX = WINCX / float(rc.right - rc.left);
// 	float fY = WINCY / float(rc.bottom - rc.top);
// 
// 	Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);
// 
// 
// 	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
// 
// 	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// �ؽ�ó ��ü �ּ�
// 		nullptr, // ����� �̹��� ������ ���� ��Ʈ ����ü �ּ�, null�ΰ�� �̹����� 0, 0�������� ���
// 		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // ����� �̹����� �߽� �࿡ ���� vector3 ����ü ������, nullptr�� ��� 0, 0�� �߽� ��ǥ
// 		nullptr, // ��ġ ��ǥ�� ���� vector3 ����ü ���ξ�
// 		D3DCOLOR_ARGB(255, 255, 255, 255)); 	// ����� ���� �̹����� ���� ���� ��, ��� �� ���� ���� �ݿ�, 0xffffffff�� �Ѱ��ָ� ���� ���� ����

}
