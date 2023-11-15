#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture()
{
}

CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::Insert_Texture(const TCHAR * pFilePath,
									const TCHAR * pStateKey, 
									const int & iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어와 구조체에 기록하는 함수

	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);

		MSG_BOX(pFilePath);
		return E_FAIL;
	}

	// D3DXCreateTextureFromFileEx : 텍스처 컴객체를 생성하는 함수

	// D3DPOOL_DEFAULT : 가장 적합한 메모리, 동작 속도가 빠르지만, 장치가 손실되었을 때 복원하기가 까다롭다.
	// D3DPOOL_MANAGED : dx가 자원 관리, 그래픽 메모리를 사용하지만 메인 메모리에 백업, D3DPOOL_DEFAULT보다 느리지만 안정적이다.
	// D3DPOOL_SYSTEMMEM : 메인 메모리를 사용하여 저장
	// D3DPOOL_SCRATCH : 메인 메모리를 사용하지만 dx 장치가 접근 할 수 없음
		
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(), 
											pFilePath,	// 파일의 이름 및 경로
											m_pTexInfo->tImgInfo.Width,	    // 텍스처 가로 사이즈
											m_pTexInfo->tImgInfo.Height,    // 텍스처 세로 사이즈
											m_pTexInfo->tImgInfo.MipLevels, // 텍스처 밉 레벨 설정
											0,	// 특수한 경우가 아닌 이상 0(일반 텍스처 옵션) 
											m_pTexInfo->tImgInfo.Format, // 텍스처 픽셀 포맷 정보
											D3DPOOL_MANAGED, // dx 자원들을 어떤 메모리에 보관할 것인가
											D3DX_DEFAULT,	// 이미지 필터링 방식 설정
											D3DX_DEFAULT,	// 밉맵을 이용한 확대 축소 시 필터링 설정
											0,	// 제거할 색상, 0으로 설정할 경우 컬러키 무효화
											nullptr,  // 이미지 정보 기록(위에서 구조체 정보 받아와서 설정 안함)
											nullptr, // 팔레트 정보(안쓴다)
											&(m_pTexInfo->pTexture)))) // 컴 객체
	{
		Safe_Delete(m_pTexInfo);
		MSG_BOX(L"Single Texture Load Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release()
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
