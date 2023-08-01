#pragma once

#include "Include.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	const wstring&	Get_ObjKey()const { return m_wstrObjKey; }

	const INFO&		Get_Info(void)const { return m_tInfo; }
	
	void			Set_Pos(const D3DXVECTOR3& vPos) { m_tInfo.vPos = vPos; }
	
	void			Set_Dir(const D3DXVECTOR3& vDir) 
	{ 
		m_tInfo.vDir = vDir;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	}
	void			Set_Scroll(D3DXVECTOR3 _vScroll) { m_vScroll = _vScroll; }
	const D3DXVECTOR3 Get_Scroll() { return m_vScroll; }
public:
	virtual HRESULT		Initialize(void)PURE;
	virtual int			Update(void)PURE;
	virtual void		Late_Update(void)PURE;
	virtual void		Render(void)PURE;
	virtual void		Release(void)PURE;

protected:
	void		Move_Frame(void);

protected:
	

	INFO					m_tInfo;
	FRAME					m_tFrame;

	wstring					m_wstrObjKey = L"";
	wstring					m_wstrStateKey = L"";
public:
	static D3DXVECTOR3			m_vScroll;
};
