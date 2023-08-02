#include "stdafx.h"
#include "Obj.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"

CObj::CObj()
{
}

CObj::~CObj()
{
	Release();
}

void CObj::Initialize()
{
	m_PreviewObj.vPos = { 0.f,0.f,0.f };
	m_PreviewObj.byDrawID = 0;
}

void CObj::Update()
{
}

void CObj::Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	for (auto& iter : m_vecObj)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"Object", iter->byDrawID);

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0),
			iter->vPos.y - m_pMainView->GetScrollPos(1),
			iter->vPos.z);

		matWorld = matScale * matTrans;

		RECT		rc{};

		// GetClientRect : 현재 클라이언트 영역의 렉트 정보를 얻어옴
		GetClientRect(m_pMainView->m_hWnd, &rc);

		float fX = WINCX / float(rc.right - rc.left);
		float fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

		float		fCenterX = (pTexInfo->tImgInfo.Width) / 2.f;
		float		fCenterY = (pTexInfo->tImgInfo.Height) / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// 텍스처 객체 주소
			nullptr, // 출력할 이미지 영역에 대한 렉트 구조체 주소, null인경우 이미지의 0, 0기준으로 출력
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 축에 대한 vector3 구조체 포인터, nullptr인 경우 0, 0이 중심 좌표
			nullptr, // 위치 좌표에 따른 vector3 구조체 포인어
			D3DCOLOR_ARGB(255, 255, 255, 255)); 	// 출력할 원본 이미지와 섞을 색상 값, 출력 시 섞은 색이 반영, 0xffffffff를 넘겨주면 원본 색상 유지

		swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			nullptr,	// 출력할 렉트의 주소
			0,			// 정렬 옵션
			D3DCOLOR_ARGB(255, 255, 255, 255));

		++iIndex;
	}
}

void CObj::Mini_Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	for (auto& iter : m_vecObj)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"Object", iter->byDrawID);

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

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// 텍스처 객체 주소
			nullptr, // 출력할 이미지 영역에 대한 렉트 구조체 주소, null인경우 이미지의 0, 0기준으로 출력
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 축에 대한 vector3 구조체 포인터, nullptr인 경우 0, 0이 중심 좌표
			nullptr, // 위치 좌표에 따른 vector3 구조체 포인어
			D3DCOLOR_ARGB(255, 255, 255, 255));	// 출력할 원본 이미지와 섞을 색상 값, 출력 시 섞은 색이 반영, 0xffffffff를 넘겨주면 원본 색상 유지
	}
}

void CObj::Release()
{
	for_each(m_vecObj.begin(), m_vecObj.end(), CDeleteObj());
	m_vecObj.clear();
	m_vecObj.shrink_to_fit();
}

void CObj::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
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

void CObj::Add_Object(const D3DXVECTOR3& vPos, const BYTE& byDrawID)
{
	OBJ* pObj = new OBJ;
	
	pObj->vPos = vPos;
	pObj->vSize = { (float)TILECX, (float)TILECY, 0.f };
	pObj->byOption = 0;
	pObj->byDrawID = byDrawID;

	m_vecObj.push_back(pObj);
}

int CObj::Get_TileIdx(const D3DXVECTOR3& vPos)
{
	for (size_t index = 0; index < m_vecObj.size(); ++index)
	{
		//if (Picking_Dot(vPos, index))
		//	return index;
	}

	return -1;
}

void CObj::Preview_Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;
		
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"Object", m_PreviewObj.byDrawID);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, m_PreviewObj.vPos.x - m_pMainView->GetScrollPos(0),
		m_PreviewObj.vPos.y - m_pMainView->GetScrollPos(1),
		m_PreviewObj.vPos.z);

	matWorld = matScale * matTrans;

	RECT		rc{};

	// GetClientRect : 현재 클라이언트 영역의 렉트 정보를 얻어옴
	GetClientRect(m_pMainView->m_hWnd, &rc);

	float fX = WINCX / float(rc.right - rc.left);
	float fY = WINCY / float(rc.bottom - rc.top);

	Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

	float		fCenterX = (pTexInfo->tImgInfo.Width) / 2.f;
	float		fCenterY = (pTexInfo->tImgInfo.Height) / 2.f;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// 텍스처 객체 주소
		nullptr, // 출력할 이미지 영역에 대한 렉트 구조체 주소, null인경우 이미지의 0, 0기준으로 출력
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 축에 대한 vector3 구조체 포인터, nullptr인 경우 0, 0이 중심 좌표
		nullptr, // 위치 좌표에 따른 vector3 구조체 포인어
		D3DCOLOR_ARGB(100, 255, 255, 255)); 	// 출력할 원본 이미지와 섞을 색상 값, 출력 시 섞은 색이 반영, 0xffffffff를 넘겨주면 원본 색상 유지

}
