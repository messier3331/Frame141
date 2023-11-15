#pragma once

#include "Incude.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	void		Initialize(void);
	void		Update();
	void		Render();
	void		Mini_Render();
	void		Release();

public:

	vector<TILE*>&			Get_VecTile() { return m_vecTile; }
	vector<TILE*>&			Get_VecWall() { return m_vecWall; }

	void		Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	int			Get_TileIndex(const D3DXVECTOR3& vPos);
	void		Picking_Grass(const D3DXVECTOR3& vPos, const int& Draw);
	void		Add_Wall(const D3DXVECTOR3& vPos, const int& Draw);
	void		Delete_Wall(const D3DXVECTOR3& vPos);

	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }

private:
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);


private:
	vector<TILE*>		m_vecTile;
	vector<TILE*>		m_vecWall;

	CToolView*			m_pMainView;
};

