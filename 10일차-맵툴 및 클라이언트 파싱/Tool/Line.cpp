#include "stdafx.h"
#include "Line.h"

CLine::CLine()
{
}


CLine::~CLine()
{
}

void CLine::Render(CDC* /*pDC*/)
{
	m_dc->MoveTo(m_startPoint.x, m_startPoint.y);
	m_dc->LineTo(m_endPoint.x, m_endPoint.y);
		
	//ReleaseDC(g_hWnd); // Device Context ¹ÝÈ¯

	//MoveToEx(hDC, int(m_tInfo.tLpoint.fX ), (int)m_tInfo.tLpoint.fY, nullptr);
	//LineTo(hDC, int(m_tInfo.tRpoint.fX ), (int)m_tInfo.tRpoint.fY);
}

