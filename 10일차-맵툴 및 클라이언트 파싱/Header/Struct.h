#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vSize;

	D3DXMATRIX		matWorld;
}INFO;

typedef	struct tagFrame
{
	float		fFrame; // �������� ���� ���� ����
	float		fMax;	// �ִ� �̹����� ������ ��

}FRAME;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;   // �� ��ü

	D3DXIMAGE_INFO			tImgInfo;	// ����ü

}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3	vPos;			// Ÿ���� ���� ��ǥ
	D3DXVECTOR3 vSize;			// Ÿ���� ����, ���� ������

	BYTE		byOption;		// 0, 1��(��ֹ�)
	BYTE		byDrawID;		// �� �� Ÿ�� �̹���

}TILE;


typedef struct tagObj
{
	D3DXVECTOR3	vPos;			// Ÿ���� ���� ��ǥ
	D3DXVECTOR3 vSize;			// ������Ʈ�� ����, ���� ������

	BYTE		byOption;		// 0, 1��(��ֹ�)
	BYTE		byDrawID;		// �� �� Ÿ�� �̹���
	
	vector<D3DXVECTOR3> vecCollide; //�ݶ��̴� ���� ����Ʈ
}OBJ;

typedef struct tagMonster
{
	D3DXVECTOR3	vPos;			// Ÿ���� ���� ��ǥ
	D3DXVECTOR3 vSize;			// ������Ʈ�� ����, ���� ������

	BYTE		byOption;		// 0, 1��(��ֹ�)
	BYTE		byDrawID;		// �� �� Ÿ�� �̹���

	vector<D3DXVECTOR3> vecLineMove;
}Monster;

typedef	struct tagUnitData
{
#ifdef _AFX
	CString	strName;
#else
	wstring	strName;
#endif

	int		iAttack;
	int		iHp;
	BYTE	byJobIndex;
	BYTE	byItem;

}UNITDATA;

typedef struct tagTexturePath
{
	wstring		wstrObjKey		= L"";
	wstring		wstrStateKey	= L"";
	wstring		wstrPath = L"";
	int			iCount = 0;

}IMGPATH;
typedef	struct tagLinePoint
{
	tagLinePoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float _fX, float _fY) : fX(_fX), fY(_fY) {}

	float	fX;
	float	fY;

}LINEPOINT;


// typedef struct tagLine
// {
// 	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
// 	tagLine(LINEPOINT& _tLpoint, LINEPOINT& _tRpoint)
// 		: tLpoint(_tLpoint), tRpoint(_tRpoint) { }
// 
// 	LINEPOINT	tLpoint;
// 	LINEPOINT	tRpoint;
// 
// }LINE;

typedef struct tagLine
{
	D3DXVECTOR2 tLpoint;
	D3DXVECTOR2 tRpoint;
}LINE;