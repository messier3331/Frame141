
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() : m_pTerrain(nullptr)
{


}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// SetScrollSizes : 스크롤 바 사이즈를 지정하는 scrollview 클래스의 멤버 함수
	// MM_TEXT : 픽셀 단위로 스크롤 바의 크기를 조정
	// 스크롤 가로, 세로 사이즈
	// CSize : 사이즈를 담당하는 클래스

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));




	// AfxGetMainWnd : 현재의 메인 윈도우 정보를 반환하는 전역 함수
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : 현재 윈도우의 RECT 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 지정한 매개 변수대로 rect를 구성하는 함수
	// 현재 0,0 기준으로 다시 렉트 정보를 재조정하고 있음

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	//SetWindowPos : 매개 변수대로 새롭게 윈도우 위치와 크기를 조정하는 함수
	//SetWindowPos(배치할 윈도우의 Z순서에 대한 포인터, X좌표, Y좌표, 가로 크기,세로 크기, 배치할 윈도우의 크기 및 위치 지정 옵션)
	
	//마지막 매개 변수가 NULL인 경우 : 순서 변경을 하지 않음
	//마지막 매개 변수가 SWP_NOZORDER인 경우 : 현재 순서를 유지하겠음

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm) *2, int(WINCY + fColFrm) * 2, SWP_NOZORDER);


	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
		return;

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"Cube Img Insert Failed");
		return;
	}

	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();

	m_pTerrain->Set_MainView(this);

	m_iDrawID = 0;

}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	m_pTerrain->Picking_Grass(D3DXVECTOR3((float)point.x + GetScrollPos(0),
										(float)point.y + GetScrollPos(1),
										0.f), m_iDrawID);

	CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView*	pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	pMiniView->Invalidate(FALSE);

	// Invalidate : 호출 시 윈도우에 WM_PAINT와  WM_ERASEBKGND 메세지를 발생 시킴
	// WM_ERASEBKGND : 배경을 지우라는 메세지

	// 매개 변수가 FALSE인 경우 : WM_PAINT 메세지만 발생
	// 매개 변수가 TRUE인 경우 : WM_PAINT와 WM_ERASEBKGND 메세지 둘 다 발생

	Invalidate(FALSE);
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnRButtonDown(nFlags, point);


	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pTerrain->Delete_Wall(D3DXVECTOR3((float)point.x + GetScrollPos(0),
			(float)point.y + GetScrollPos(1),
			0.f));
	}
	else
	{
		m_pTerrain->Add_Wall(D3DXVECTOR3((float)point.x + GetScrollPos(0),
			(float)point.y + GetScrollPos(1),
			0.f), m_iDrawID);
	}

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
	pMiniView->Invalidate(FALSE);


	Invalidate(FALSE);
}



void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_pTerrain->Picking_Grass(D3DXVECTOR3((float)point.x + GetScrollPos(0),
											(float)point.y + GetScrollPos(1),
											0.f), 
											m_iDrawID);
		Invalidate(FALSE);

		CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMiniView*	pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

		pMiniView->Invalidate(FALSE);
	}

}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	

#pragma region 복습
	// Draw : 스크린 공간에 2D 스프라이트 이미지를 출력
	/*CDevice::Get_Instance()->Get_Sprite()->Draw(m_pSingle->Get_Texture()->pTexture, // 텍스처 컴객체
	nullptr, // 출력할 이미지 영역에 대한 RECT 구조체 포인터, NULL인 경우 이미지의 0, 0기준으로 출력
	nullptr, // 출력할 이미지의 중심점에 VECTOR3 구조체 포인터, NULL인 경우 0, 0이 중심 좌표
	nullptr, // 이미지를 출력할 위치, VECTOR3 구조체 포인터, NULL인 경우 스크린 상 0, 0 좌표에 출력
	D3DCOLOR_ARGB(255, 255, 255, 255));	// 출력할 원본 이미지와 합성할 색상 값, 0xffffffff를 넘겨줄 경우 원본 색상을 유지*/

	//float	fCenterX = m_pSingle->Get_Texture()->tImgInfo.Width / 2.f;
	//float	fCenterY = m_pSingle->Get_Texture()->tImgInfo.Height / 2.f;
#pragma endregion 복습	

	CDevice::Get_Instance()->Render_Begin();
	
	m_pTerrain->Render();

	CDevice::Get_Instance()->Render_End();
}

#pragma region 안써

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


#pragma endregion 안써


void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	Safe_Delete(m_pTerrain);

	CTextureMgr::Destroy_Instance();
	CDevice::Destroy_Instance();

}



void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.





	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}
