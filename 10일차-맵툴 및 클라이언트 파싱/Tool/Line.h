#pragma once

#include "Include.h"
class CToolView;
class CLine
{
public:
	CLine();
	~CLine();

public:
	void	SetStartPoint(const CPoint& pt) { m_startPoint = pt; }
	void	SetEndPoint(const CPoint& pt) { m_endPoint = pt; }

	const CPoint& GetStartPoint() const { return m_startPoint; }
	const CPoint& GetEndPoint() const { return m_endPoint; }
	void		SetMainView(CToolView* pView) { m_pMainView = pView; }
public:
	void		Init();
	void		Render(vector<LINE>& InVector);
	void		Release();
	void		CreateLine(D3DXVECTOR2& vPos);
	void		CreateLineStart(D3DXVECTOR2& vPos);
public:
	vector<LINE*>					m_vecLine;
	size_t							m_iIndex;
private:
	CPoint m_startPoint;
	CPoint m_endPoint;
	CClientDC* m_dc;
	LPD3DXLINE				m_pLine;

	CToolView*			m_pMainView = nullptr;
};

