#include "stdafx.h"
#include "MyTerrain.h"
#include "Device.h"
#include "TextureMgr.h"


CMyTerrain::CMyTerrain()
{
}


CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	if (FAILED(Load_Tile(L"../Data/Tile.dat")))
	{
		MSG_BOX(L"Tile Data Load Failed");
		return E_FAIL;
	}

	return S_OK;
}

int CMyTerrain::Update(void)
{
	return 0;
}

void CMyTerrain::Late_Update(void)
{
}

void CMyTerrain::Render(void)
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex(0);

	for (auto& iter : m_vecTile)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x,	// 0인 경우 x 스크롤 값
			iter->vPos.y,	// 1인 경우 y 스크롤 값
			iter->vPos.z);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

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
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x,	// 0인 경우 x 스크롤 값
			iter->vPos.y,	// 1인 경우 y 스크롤 값
			iter->vPos.z);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

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

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();

	for_each(m_vecWall.begin(), m_vecWall.end(), CDeleteObj());
	m_vecWall.clear();


}

HRESULT CMyTerrain::Load_Tile(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	Release();

	DWORD		dwByte(0);
	TILE*		pTile = nullptr;
	char		cTile;
	
	while (true)
	{
		ReadFile(hFile, &cTile, sizeof(char), &dwByte, nullptr);
		pTile = new TILE;
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		if (cTile == 'T')
		{
			m_vecTile.push_back(pTile);
		}
		else if (cTile == 'W')
		{
			m_vecWall.push_back(pTile);
		}
		

	}

	CloseHandle(hFile);

	return S_OK;
}


