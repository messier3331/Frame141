// PathFind.cpp: 구현 파일
//

//#include "pch.h"
#include "stdafx.h"
#include "Tool.h"
#include "PathFinder.h"
#include "afxdialogex.h"
#include "FileInfo.h"

// CPathFind 대화 상자

IMPLEMENT_DYNAMIC(CPathFinder, CDialog)

CPathFinder::CPathFinder(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PATHFINDER, pParent)
{

}

CPathFinder::~CPathFinder()
{
}

void CPathFinder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_MFCBUTTON1, m_SaveBtn);
	DDX_Control(pDX, IDC_MFCBUTTON2, m_LoadBtn);
}


BEGIN_MESSAGE_MAP(CPathFinder, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFinder::OnListBox)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CPathFinder::OnSaveData)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CPathFinder::OnLoadData)
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPathFind 메시지 처리기


HBRUSH CPathFinder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	static CBrush brBkgnd(RGB(57, 57, 57));
	pDC->SetTextColor(RGB(255, 255, 255));

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetBkColor(RGB(57, 57, 57));
		break;
	case CTLCOLOR_EDIT:
		pDC->SetBkColor(RGB(40, 40, 40));
		break;
	case CTLCOLOR_LISTBOX:
		pDC->SetBkColor(RGB(57, 57, 57));
		//pDC->SetTextColor(RGB(10, 10, 10));
		break;
	default:
		break;
	}

	hbr = brBkgnd;
	return hbr;
}


void CPathFinder::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for_each(m_PathList.begin(), m_PathList.end(), CDeleteObj());
	m_PathList.clear();
}


void CPathFinder::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CPathFinder::OnSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wofstream		fout;

	fout.open(L"../Data/ImgPath.txt", ios::out);

	if (!fout.fail())	// 파일 개방 성공
	{
		for (auto& iter : m_PathList)
		{
			fout << iter->wstrObjKey << L"|" << iter->wstrStateKey << L"|" << iter->iCount << L"|" << iter->wstrPath << endl;
		}

		fout.close();
	}

	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathFinder::OnLoadData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wifstream		fin;

	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";

		TCHAR	szPath[MAX_PATH] = L"";

		wstring	wstrCombined = L"";

		m_ListBox.ResetContent();

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;

			m_ListBox.AddString(wstrCombined.c_str());
		}

		fin.close();
	}

	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);

}


void CPathFinder::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);
	TCHAR	szFilePath[MAX_PATH] = L"";
	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	
	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFilePath, m_PathList);
	}


	m_ListBox.ResetContent();


	wstring		wstrCombined = L"";
	TCHAR		szBuf[MAX_STR] = L"";

	for (auto& iter : m_PathList)
	{
		//정수-> wstring 변환, 3번째 매개변수: 10진수로 변환
		_itow_s(iter->iCount, szBuf, 10);

		wstrCombined = iter->wstrObjKey + L"|" + iter->wstrStateKey + L"|" + szBuf + L"|" + iter->wstrPath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	UpdateData(FALSE);
}

void CPathFinder::SetMFCButton(CMFCButton& button)
{
	button.SetFaceColor(RGB(100, 100, 100));
	button.SetTextColor(RGB(255, 255, 255));
	button.SetFont(&m_Font);
}


BOOL CPathFinder::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Font.CreatePointFont(96, L"Pretendard Variable Bold");


	SetMFCButton(m_SaveBtn);
	SetMFCButton(m_LoadBtn);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CPathFinder::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(57, 57, 57));

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
