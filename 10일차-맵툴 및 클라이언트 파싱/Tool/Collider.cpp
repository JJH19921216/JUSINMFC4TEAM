#include "stdafx.h"
#include "Collider.h"
#include "MainFrm.h"
#include "ToolView.h"

CCollider::CCollider()
{
}

CCollider::~CCollider()
{
}

void CCollider::Initialize()
{
	// AfxGetApp : 메인 쓰레드를 갖고 있는 현재 메인 app을 반환
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	//GetPane(행, 열) : 해당하는 창의 정보를 얻어오는 함수 
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	m_pObj = pMainView->m_pObj;
}

void CCollider::Update()
{
}

void CCollider::Render()
{

}

void CCollider::Mini_Render()
{
}

void CCollider::Release()
{
}

void CCollider::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
}

void CCollider::Add_Collider(const D3DXVECTOR3& vPos, const BYTE& byDrawID)
{
	if (m_ColliderInfoList.find(byDrawID) != m_ColliderInfoList.end())
	{
		m_pObj->m_vecObj.back()->vecCollide = m_ColliderInfoList[byDrawID];
	}
	else
		m_pObj->m_vecObj.back()->vecCollide.clear();
}

void CCollider::Update_ObjCollider()
{
	for (auto& iter : m_pObj->m_vecObj)
	{
		if (m_ColliderInfoList.find(iter->byDrawID) != m_ColliderInfoList.end())
		{
			iter->vecCollide = m_ColliderInfoList[iter->byDrawID];
		}
		else
			iter->vecCollide.clear();
	}
}


