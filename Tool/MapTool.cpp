// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveData)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnListBox()
{
	UpdateData(TRUE);

	CString		strSelectName;

	int		iIndex = m_ListBox.GetCurSel();

	m_ListBox.GetText(iIndex, strSelectName);

	auto	iter = m_mapPngImg.find(strSelectName);


	int iSkip = strSelectName.Find(_T("Grass"));
	if (iSkip != -1) 
	{
		iSkip += _tcslen(_T("Grass"));
	}

	CString strName = strSelectName.Mid(iSkip);
	int iDrawID = _ttoi(strName);



	// AfxGetApp : 메인 쓰레드를 갖고 있는 현재 메인 app을 반환
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	// GetPane(행, 열) : 행과 열에 해당하는 클래스의 정보를 얻어오는 함수 
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	pMainView->m_iDrawID = iDrawID;

	if (iter == m_mapPngImg.end())
		return;

	m_Picture.SetBitmap(*(iter->second));


	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	
	CDialog::OnDropFiles(hDropInfo);
	
	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MAX_STR] = L"";

	// DragQueryFile : 드롭된 파일 중 0번 인덱스에 해당하는 파일의 전체 경로를 얻어옴
	// 2인자 :  0xffffffff(-1)로 지정되면 드롭된 파일의 개수를 반환

	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString	strRelative = CFileInfo::ConvertRelativePath(szFilePath);
		CString	strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());
		
		// 확장자 명을 제거
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage*	pPngImg = new CImage;
			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_ListBox.AddString(strFileName);
		}		
	}

	Horizontal_Scroll();

	UpdateData(FALSE);
}

void CMapTool::Horizontal_Scroll()
{
	CString			strName;
	CSize			size;

	int		iWidth(0);

	CDC*	pDC = m_ListBox.GetDC();

	// GetCount : 리스트 박스 목록의 개수를 얻어옴
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		//	GetText: 인덱스에 해당하는 목록의 문자열을 얻어옴
		m_ListBox.GetText(i, strName);

		// 현재 문자열의 길이를 픽셀 단위로 변환
		size = pDC->GetTextExtent(strName);

		if (size.cx > iWidth)
			iWidth = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트 박스가 가로 스크롤 바를 생성할 수 있는 최대 범위를 얻어오는 함수
	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);

}



void CMapTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapPngImg.begin(), m_mapPngImg.end(), [](auto& MyPair)
	{
		MyPair.second->Destroy();
		Safe_Delete(MyPair.second);
	});
	m_mapPngImg.clear();

	
}


void CMapTool::OnSaveData()
{
	CFileDialog		Dlg(FALSE,	// TRUE(열기, 불러오기), FALSE(다른 이름으로 저장하기) 모드지정
		L"dat",	// 기본 확장자명
		L"*.dat", // 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, //OFN_HIDEREADONLY(읽기 전용 체크박스 숨김) | OFN_OVERWRITEPROMPT(중복 파일 저장 시 경고메세지 띄움)
		L"Data Files(*.dat) | *.dat||", // 대화 상자에 표시될 파일 형식("콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||"
		this);	// 부모 윈도우 주소

	TCHAR	szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString		str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwByte(0);
		
		CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView*		pMainView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
		
		vector<TILE*>&	vecTile = pMainView->m_pTerrain->Get_VecTile();

		if (vecTile.empty())
			return;
		
		char cTile = 'T';

		for (auto& iter : vecTile)
		{			
			WriteFile(hFile, &cTile, sizeof(char), &dwByte, nullptr);
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);
		}

		vector<TILE*>& vecWall = pMainView->m_pTerrain->Get_VecWall();
		if (vecWall.empty())
			return;

		cTile = 'W';

		for (auto& iter : vecWall)
		{
			WriteFile(hFile, &cTile, sizeof(char), &dwByte, nullptr);
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);
		}
		

		CloseHandle(hFile);
	}
}
