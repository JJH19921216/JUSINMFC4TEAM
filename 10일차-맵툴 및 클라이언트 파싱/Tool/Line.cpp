#include "stdafx.h"
#include "Line.h"
#include "Device.h"
#include "ToolView.h"

CLine::CLine()
{
}


CLine::~CLine()
{
}

void CLine::Init()
{
}

void CLine::Render(vector<LINE>& InVector)
{
	for (LINE line : InVector)
	{
		LPD3DXLINE pLine;
		D3DXCreateLine(CDevice::Get_Instance()->Get_Device(), &pLine);
	
		D3DXVECTOR2			vPoint[2] = {
	
		D3DXVECTOR2(line.tLpoint.x - m_pMainView->GetScrollPos(0), line.tLpoint.y - m_pMainView->GetScrollPos(1)),
		D3DXVECTOR2(line.tRpoint.x - m_pMainView->GetScrollPos(0), line.tRpoint.y - m_pMainView->GetScrollPos(1)),
		};
	
		pLine->Draw(vPoint, 2, D3DCOLOR_ARGB(255, 0, 0, 255));
	}
}
void CLine::CreateLine(D3DXVECTOR2& vPos)
{
	m_vecLine[m_iIndex]->tRpoint = vPos;
}

void CLine::CreateLineStart(D3DXVECTOR2& vPos)
{
	LINE* pLine = new LINE;

	pLine->tLpoint = vPos;

	m_vecLine.emplace_back(pLine);

	m_iIndex = m_vecLine.size() - 1;
}
void CLine::Release(void)
{
	for (auto& vec : m_vecLine)
	{
		Safe_Delete(vec);
	}
	m_vecLine.clear();
}