// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnUnitTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnMapTool)
	ON_BN_CLICKED(IDC_BUTTON11, &CMyForm::OnPathFinder)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// CreatePointFont(폰트 크기, 글씨체)
	m_Font.CreatePointFont(80, L"Pretendard Variable");

	// GetDlgItem : 다이얼로그에 배치된 리소스를 id값에 따라 얻어오는 함수
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON11)->SetFont(&m_Font);



#pragma region 탭 쓰지마
	//if (nullptr == m_MapTool.GetSafeHwnd())
	//	m_MapTool.Create(IDD_MAPTOOL, GetDlgItem(IDC_TAB1));

	//if (nullptr == m_UnitTool.GetSafeHwnd())
	//	m_UnitTool.Create(IDD_UNITTOOL, GetDlgItem(IDC_TAB1));


	//m_TabCtrl.InsertItem(0, L"Tile");
	//m_TabCtrl.InsertItem(1, L"Unit");

	//CRect tabRect, unitRect, mapRect;

	//m_TabCtrl.GetClientRect(&tabRect);

	//m_TabCtrl.GetItemRect(0, &mapRect);
	//m_TabCtrl.GetItemRect(1, &unitRect);

	//int iX = unitRect.left;
	//int iY = unitRect.bottom;
	//int iWidth = tabRect.right - unitRect.left;
	//int iHeight = tabRect.bottom - unitRect.top;


	//m_MapTool.SetWindowPos(NULL, 0, 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	//m_UnitTool.SetWindowPos(NULL, 0, 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	//m_UnitTool.MoveWindow(iX, iY, iWidth, iHeight, TRUE);
	//m_MapTool.MoveWindow(iX, iY, iWidth, iHeight, TRUE);



	//m_MapTool.ShowWindow(SW_SHOW);
	//m_UnitTool.ShowWindow(SW_HIDE);

#pragma endregion
}


void CMyForm::OnUnitTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// id에 해당하는 다이얼로그 박스를 생성
	// GetSafeHwnd : 현재 다이얼로그 윈도우 핸들을 반환

	if (nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);


	m_UnitTool.SetWindowPos(NULL, 1600, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	// 창 모양으로 출력
	m_UnitTool.ShowWindow(SW_SHOW);


}


void CMyForm::OnMapTool()
{
	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL);

	// 창 모양으로 출력
	m_MapTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnPathFinder()
{
	if (nullptr == m_PathFinder.GetSafeHwnd())
		m_PathFinder.Create(IDD_PATHFINDER);

	m_PathFinder.SetWindowPos(NULL, 0, 1200, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_PathFinder.ShowWindow(SW_SHOW);

}


BOOL CMyForm::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(57, 57, 57));
	return TRUE;
}
