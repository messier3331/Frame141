#include "stdafx.h"
#include "Terrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"

CTerrain::CTerrain()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTerrain::~CTerrain()
{
	Release(); 
}

void CTerrain::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Stage/Terrain/Zelcrodancer/Grass/Grass%d.png", TEX_MULTI, L"Terrain", L"Tile", 70)))
	{
		AfxMessageBox(L"Tile Img Insert Failed");
		return;
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE*		pTile = new TILE;

			float	fX = float((TILECX) * j);
			float	fY = float((TILECY) * i);

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { TILECX, TILECY, 0.f };
			pTile->byOption = 0;
			pTile->byDrawID = 0;

			m_vecTile.push_back(pTile);
		}
	}
}

void CTerrain::Update()
{
}

void CTerrain::Render()
{
	RECT	rc{};
	
	D3DXMATRIX		matWorld, matScale, matRotZ, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex(0);

	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, 
			iter->vPos.x - m_pMainView->GetScrollPos(0),	// 0인 경우 x 스크롤 값
			iter->vPos.y - m_pMainView->GetScrollPos(1),	// 1인 경우 y 스크롤 값
			iter->vPos.z);

		matWorld = matScale * matTrans;

		GetClientRect(m_pMainView->m_hWnd, &rc);

		float		fX = WINCX / float(rc.right - rc.left);
		float		fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX, fY);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(0.f, 0.f, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	for (auto& iter : m_vecWall)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x - m_pMainView->GetScrollPos(0),	// 0인 경우 x 스크롤 값
			iter->vPos.y - m_pMainView->GetScrollPos(1),	// 1인 경우 y 스크롤 값
			iter->vPos.z);

		matWorld = matScale * matTrans;

		GetClientRect(m_pMainView->m_hWnd, &rc);

		float		fX = WINCX / float(rc.right - rc.left);
		float		fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX, fY);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(0.f, 0.f, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}



}

void CTerrain::Mini_Render()
{
	D3DXMATRIX		matWorld, matScale,matTrans;

	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x - m_pMainView->GetScrollPos(0),	// 0인 경우 x 스크롤 값
			iter->vPos.y - m_pMainView->GetScrollPos(1),	// 1인 경우 y 스크롤 값
			iter->vPos.z);

		matWorld = matScale * matTrans;		

		Set_Ratio(&matWorld, 0.3f, 0.3f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(0.f, 0.f, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));	
	}

	for (auto& iter : m_vecWall)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x - m_pMainView->GetScrollPos(0),	// 0인 경우 x 스크롤 값
			iter->vPos.y - m_pMainView->GetScrollPos(1),	// 1인 경우 y 스크롤 값
			iter->vPos.z);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, 0.3f, 0.3f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(0.f, 0.f, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<TILE*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	for_each(m_vecWall.begin(), m_vecWall.end(), Safe_Delete<TILE*>);
	m_vecWall.clear();
	m_vecWall.shrink_to_fit();

}

void CTerrain::Tile_Change(const D3DXVECTOR3 & vPos, const int & iDrawID)
{
	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = (BYTE)iDrawID;
	m_vecTile[iIndex]->byOption = 1;
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t index = 0; index < m_vecTile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
			return index;
	}

	return -1;
}

bool CTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	// y = ax + b;

	float	fGradient[4]
	{
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f),
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f)
	};

	// 12 -> 3 -> 6 -> 9
	D3DXVECTOR3		vPoint[4]
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f},
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f },

	};

	//y = ax + b
	// -b = ax - y -> b =  y - ax

	float	fB[4]
	{
		vPoint[0].y - fGradient[0] * vPoint[0].x,
		vPoint[1].y - fGradient[1] * vPoint[1].x,
		vPoint[2].y - fGradient[2] * vPoint[2].x,
		vPoint[3].y - fGradient[3] * vPoint[3].x
	};

	 // 0 == ax + b - y	// 직선 상에 있다.
	 // 0	> ax + b - y	// 직선 보다 위쪽에 있다
	 // 0	< ax + b - y	// 직선 보다 아래쪽에 있다

	bool	bCheck[4]{ false };

	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;

	if (0 >= fGradient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;

	if (0 >= fGradient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;

	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CTerrain::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
{
	// 12 -> 3 -> 6 -> 9
	D3DXVECTOR3		vPoint[4]
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f },

	};

	D3DXVECTOR3	vDir[4]
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3	vNormal[4]
	{
		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }

	};

	D3DXVECTOR3	vMouseDir[4]
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CTerrain::Picking_Grass(const D3DXVECTOR3& vPos, const int& Draw)
{
	int X(0), Y(0);
	int iIndex(0);

	X = vPos.x / TILECX;
	Y = vPos.y / TILECY;
	iIndex = (Y * TILEX) + X;

	m_vecTile[iIndex]->byDrawID = Draw;
}

void CTerrain::Add_Wall(const D3DXVECTOR3& vPos, const int& Draw)
{
	int X(0), Y(0);
	X = ((int)vPos.x / TILECX) * TILECX;
	Y = ((int)vPos.y / TILECY) * TILECY;

	for (auto& iter : m_vecWall)
	{
		if ((iter->vPos.x == X) && (iter->vPos.y == Y))
		{
			iter->byDrawID = Draw;
			return;
		}

	}

	TILE* pTile = new TILE;

	pTile->vPos = { (float)X, (float)Y, 0.f };
	pTile->vSize = { TILECX, TILECY, 0.f };
	pTile->byOption = 1;
	pTile->byDrawID = Draw;
	m_vecWall.push_back(pTile);
}

void CTerrain::Delete_Wall(const D3DXVECTOR3& vPos)
{
	int X(0), Y(0);
	X = ((int)vPos.x / TILECX) * TILECX;
	Y = ((int)vPos.y / TILECY) * TILECY;

	for (auto iter = m_vecWall.begin(); iter != m_vecWall.end(); ++iter)
	{
		if ((*iter)->vPos.x == X && (*iter)->vPos.y == Y)
		{
			delete* iter;
			iter = m_vecWall.erase(iter);
			return;
		}
	}
}

void CTerrain::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
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
