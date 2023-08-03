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

public:

	void		Render(CDC* pDC);

private:
	CPoint m_startPoint;
	CPoint m_endPoint;
	CClientDC*		m_dc;
};

