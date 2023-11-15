#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice() : m_pSDK(nullptr), m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{
}

CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::Init_Device()
{
	// 1. 모체 객체(장치를 조사할 객체 : LPDIRECT3D9) 생성
	
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	
	// 2. 장치의 지원 수준 조사

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps      : 장치에 대한 정보를 얻어오는 함수
	// D3DADAPTER_DEFAULT : 정보를 얻으려는 기본 그래픽 카드를 의미

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps");
		return E_FAIL;
	}

	DWORD	vp(0);	// vertex processing

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	
	// 3. 장치를 제어할 객체(LPDIRECT3DDEVICE9) 생성

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	// CreateDevice : 그래픽 장치를 제어할 디바이스 객체를 생성

	/*CreateDevice(어떤 그래픽 카드를 제어할 것인가, 
					어떤 정보로 장치에 접근할 것인가,
					장치를 사용할 윈도우,
					동작 방식
					사용 환경
					결과 값으로 저장할 포인터의 주소)*/

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									g_hWnd,
									vp,
									&d3dpp,
									&m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}

	// 스프라이트 생성
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// 폰트 생성

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"Pretendard Variable");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Render_Begin()
{
	m_pDevice->Clear(0,			// 렉트 배열의 원소 개수
					nullptr,	// 렉트 배열의 이름(null인 경우 화면 전체 영역을 지정)
					D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // 스텐실, 깊이버퍼, 후면 버퍼 모두 지우겠다는 의미
					D3DCOLOR_ARGB(255, 0, 0, 255),	// 후면 버퍼를 채울 색상
					1.f,		// 깊이 버퍼 초기화 값
					0);			// 스텐실 버퍼 초기화 값

	m_pDevice->BeginScene();
	// 2D 이미지를 그릴 수 있도록 그리기 장치 객체를 준비, 현재 옵션은 알파테스트가 유효한 상태에서 알파블랜딩을 사용하겠다는 옵션
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();

	m_pDevice->EndScene();

	// 후면 버퍼가 전면 버퍼로 전환되어 화면에 출력한다.
	// 1~2. 스왑 체인이 D3DSWAPEFFECT_COPY로 작성되지 않은 한 NULL
	// 3. 출력 대상 윈도우 핸들, NULL인 경우 D3DPRESENT_PARAMETERS 구조체에 정의한 핸들을 사용 
	// 4. 스왑 체인이 D3DSWAPEFFECT_COPY로 작성되지 않은 한 NULL
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDevice::Release()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS & d3dpp)
{
	d3dpp.BackBufferWidth  = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount  = 1;
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;	

	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 버퍼 하나로 뒤집으면서 사용하는 방식
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 복사 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	d3dpp.hDeviceWindow = g_hWnd;

	d3dpp.Windowed = TRUE;		// 창 모드로 실행할지 전체화면 모드로 실행할지 결정(TRUE 인 경우, 창 모드)

	// DX가 자동적이로 깊이, 스텐실 버퍼를 만들고 관리할 것인가
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 전체화면 시에만 사용되는 멤버 변수들
	
	// 전체 화면 시 현재 모니터 재생률에 대한 관리 여부
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// 모니터 재생률과 실제 시연의 간격 설정

	// D3DPRESENT_INTERVAL_IMMEDIATE : 즉시 시연
	// D3DPRESENT_INTERVAL_DEFAULT : 모니터 재생률에 맞출 것

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
