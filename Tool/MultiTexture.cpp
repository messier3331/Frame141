#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture()
{
}

CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO * CMultiTexture::Get_Texture(const TCHAR * pStateKey, const int & iCount)
{
	auto iter = find_if(m_MapMultiTex.begin(), m_MapMultiTex.end(),
		[&](auto& MyPair)->bool
	{
		if (pStateKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_MapMultiTex.end())
		return false;
	
	return iter->second[iCount];
}

HRESULT CMultiTexture::Insert_Texture(const TCHAR * pFilePath,
									const TCHAR * pStateKey, 
									const int & iCount)
{
	// 경로의 길이 절대 520 BYTE를 넘어선 안된다.
	TCHAR	szFullPath[MAX_PATH] = L"";
	
	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			Safe_Delete(pTexInfo);

			AfxMessageBox(szFullPath);
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
												szFullPath,
												pTexInfo->tImgInfo.Width,	    
												pTexInfo->tImgInfo.Height,    
												pTexInfo->tImgInfo.MipLevels, 
												0,	
												pTexInfo->tImgInfo.Format,
												D3DPOOL_MANAGED, 
												D3DX_DEFAULT,	
												D3DX_DEFAULT,	
												0,	
												nullptr,
												nullptr,
												&(pTexInfo->pTexture))))
		{
			Safe_Delete(pTexInfo);
			AfxMessageBox(L"MULTI Texture Load Failed");
			return E_FAIL;
		}

		m_MapMultiTex[pStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release()
{
	for_each(m_MapMultiTex.begin(), m_MapMultiTex.end(), [](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), [](auto& Temp)
		{
			if (Temp)
			{
				delete Temp;
				Temp = nullptr;
			}
		});
		MyPair.second.clear();

	});
	m_MapMultiTex.clear();

}
