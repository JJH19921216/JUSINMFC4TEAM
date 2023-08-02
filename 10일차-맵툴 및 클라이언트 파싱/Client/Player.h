#pragma once
#include "Obj.h"
class CPlayer :
    public CObj
{

public:
	virtual HRESULT		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(void);
	virtual void		Release(void);

private:
	//map<>		m_vecTile;
	D3DXVECTOR3 m_vDest;
	bool		m_bMove;
};

