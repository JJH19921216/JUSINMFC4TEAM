#include "stdafx.h"
#include "Obj.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
#include "CollideView.h"

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

	//y��ǥ ���� ���� ���� ���� (�������� �ڷ� ��ġ)
	m_vecRender = m_vecObj;
	sort(m_vecRender.begin(), m_vecRender.end(), [](OBJ* first, OBJ* second)
		{
			return first->vPos.y < second->vPos.y;
		});

	for (auto& iter : m_vecRender)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"Object", iter->byDrawID);

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0) / g_Ratio,
			iter->vPos.y - m_pMainView->GetScrollPos(1) / g_Ratio,
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


		
	}

	for (auto& iter : m_vecObj)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0) / g_Ratio,
			iter->vPos.y - m_pMainView->GetScrollPos(1) / g_Ratio,
			iter->vPos.z);

		matWorld = matScale * matTrans;

		RECT		rc{};

		// GetClientRect : ���� Ŭ���̾�Ʈ ������ ��Ʈ ������ ����
		GetClientRect(m_pMainView->m_hWnd, &rc);

		float fX = WINCX / float(rc.right - rc.left);
		float fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
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

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// �ؽ�ó ��ü �ּ�
			nullptr, // ����� �̹��� ������ ���� ��Ʈ ����ü �ּ�, null�ΰ�� �̹����� 0, 0�������� ���
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // ����� �̹����� �߽� �࿡ ���� vector3 ����ü ������, nullptr�� ��� 0, 0�� �߽� ��ǥ
			nullptr, // ��ġ ��ǥ�� ���� vector3 ����ü ���ξ�
			D3DCOLOR_ARGB(255, 255, 255, 255));	// ����� ���� �̹����� ���� ���� ��, ��� �� ���� ���� �ݿ�, 0xffffffff�� �Ѱ��ָ� ���� ���� ����
	}
}

void CObj::Release()
{
	for_each(m_vecObj.begin(), m_vecObj.end(), CDeleteObj());
	m_vecObj.clear();
	m_vecObj.shrink_to_fit();

	for_each(m_vecRedolist.begin(), m_vecRedolist.end(), CDeleteObj());
	m_vecRedolist.clear();
	m_vecRedolist.shrink_to_fit();
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

	for_each(m_vecRedolist.begin(), m_vecRedolist.end(), CDeleteObj());
	m_vecRedolist.clear();
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
	D3DXMatrixTranslation(&matTrans, m_PreviewObj.vPos.x - m_pMainView->GetScrollPos(0) / g_Ratio,
		m_PreviewObj.vPos.y - m_pMainView->GetScrollPos(1) / g_Ratio,
		m_PreviewObj.vPos.z);

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

void CObj::CLPreview_Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"Object", m_PreviewObj.byDrawID);

	m_pSubView->SetScrollSizes(MM_TEXT, CSize(pTexInfo->tImgInfo.Width, pTexInfo->tImgInfo.Height));

	float		fCenterX = (pTexInfo->tImgInfo.Width) / 2.f;
	float		fCenterY = (pTexInfo->tImgInfo.Height) / 2.f;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, fCenterX - m_pSubView->GetScrollPos(0),
		fCenterY - m_pSubView->GetScrollPos(1),
		0.f);

	matWorld = matScale * matTrans;

	RECT		rc{};

	// GetClientRect : ���� Ŭ���̾�Ʈ ������ ��Ʈ ������ ����
	GetClientRect(m_pSubView->m_hWnd, &rc);

	float fX = WINCX / float(rc.right - rc.left);
	float fY = WINCY / float(rc.bottom - rc.top);

	Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);


	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,	// �ؽ�ó ��ü �ּ�
		nullptr, // ����� �̹��� ������ ���� ��Ʈ ����ü �ּ�, null�ΰ�� �̹����� 0, 0�������� ���
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // ����� �̹����� �߽� �࿡ ���� vector3 ����ü ������, nullptr�� ��� 0, 0�� �߽� ��ǥ
		nullptr, // ��ġ ��ǥ�� ���� vector3 ����ü ���ξ�
		D3DCOLOR_ARGB(255, 255, 255, 255)); 	// ����� ���� �̹����� ���� ���� ��, ��� �� ���� ���� �ݿ�, 0xffffffff�� �Ѱ��ָ� ���� ���� ����

}

void CObj::ObjUndo()
{
	if (!m_vecObj.empty())
	{
		m_vecRedolist.push_back(m_vecObj.back());
		m_vecObj.pop_back();
	}
}

void CObj::ObjRedo()
{
	if (!m_vecRedolist.empty())
	{
		m_vecObj.push_back(m_vecRedolist.back());
		m_vecRedolist.pop_back();
	}
}

