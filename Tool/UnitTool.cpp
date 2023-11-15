// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strTest(_T(""))
	, m_strCopy(_T(""))
	, m_strName(_T(""))
	, m_iHp(0)
	, m_iAttack(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTest);
	DDX_Text(pDX, IDC_EDIT2, m_strCopy);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iHp);
	DDX_Text(pDX, IDC_EDIT5, m_iAttack);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnAddCharactor)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnSearchData)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnDeleteData)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnLoadData)
END_MESSAGE_MAP()

void CUnitTool::OnBnClickedButton1()
{
	// TRUE : 다이얼로그 박스로부터 입력된 값들을 얻어옴	
	// FALSE : 변수에 저장된 값들을 다이얼로그 박스에 반영

	UpdateData(TRUE);

	m_strCopy = m_strTest;

	UpdateData(FALSE);
}

void CUnitTool::OnAddCharactor()
{
	UpdateData(TRUE);

	UNITDATA*		pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iHp = m_iHp;
	pUnit->iAttack = m_iAttack;

	for (int i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[2].GetCheck())
		pUnit->byItem |= SAPPHIRE;

	/*if (dynamic_cast<CButton*>(GetDlgItem(IDC_CHECK1))->GetCheck())
		pUnit->byItem |= RUBY;*/


	// AddString : 리스트 박스에 매개변수에 해당하는 문자열을 추가함
	m_ListBox.AddString(pUnit->strName);

	m_mapUnitData.insert({ pUnit->strName, pUnit });

	UpdateData(FALSE);
}


void CUnitTool::OnListBox()
{
	UpdateData(TRUE);

	CString		strSelectName;

	// GetCurSel : 커서가 놓인 셀의 인덱스 값을 반환(0번부터 인덱스가 존재)
	int		iIndex = m_ListBox.GetCurSel();

	// GetText : 해당 인덱스에 있는 리스트 박스 문자열을 얻어오는 함수
	m_ListBox.GetText(iIndex, strSelectName);

	auto	iter = m_mapUnitData.find(strSelectName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);
	
	UpdateData(FALSE);
}


void CUnitTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), CDeleteMap());
	m_mapUnitData.clear();
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP		hBitmap = (HBITMAP)LoadImage(nullptr, 
		L"../Texture/JusinLogo1.bmp", 
		IMAGE_BITMAP, 
		100, 50, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_Bitmap.SetBitmap(hBitmap);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUnitTool::OnSearchData()
{
	UpdateData(TRUE);

	auto&	iter = m_mapUnitData.find(m_strFindName);

	if (iter == m_mapUnitData.end())
		return;

	//FindString : 첫 번째 매개 변수 인덱스부터 두 번째 매개 변수에 해당하는 문자열 탐색
	int		iIndex = m_ListBox.FindString(0, m_strFindName);

	if (LB_ERR == iIndex)
		return;
	
	// 인덱스 셀을 선택함
	m_ListBox.SetCurSel(iIndex);

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);


	UpdateData(FALSE);
}


void CUnitTool::OnDeleteData()
{
	UpdateData(TRUE);

	CString	strFindName = L"";
	
	int	iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	m_ListBox.GetText(iIndex, strFindName);

	auto&	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);

	m_ListBox.DeleteString(iIndex);


	UpdateData(FALSE);
}


void CUnitTool::OnSaveData()
{
	CFileDialog		Dlg(FALSE,	// TRUE(열기, 불러오기), FALSE(다른 이름으로 저장하기) 모드지정
						L"dat",	// 기본 확장자명
						L"*.dat", // 대화 상자에 표시될 최초 파일명
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, //OFN_HIDEREADONLY(읽기 전용 체크박스 숨김) | OFN_OVERWRITEPROMPT(중복 파일 저장 시 경고메세지 띄움)
						L"Data Files(*.dat) | *.dat||", // 대화 상자에 표시될 파일 형식("콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||"
						this);	// 부모 윈도우 주소

	TCHAR	szPath[MAX_STR] = L"";

	// 현재 프로젝트 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_STR, szPath);
	// D:\유준환\141기\Frame141\Tool

	// 전체 경로에서 파일 이름만 잘라주는 함수, 만약 경로상 파일 이름이 없다면 제일 마지막 폴더이름을 잘라냄
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	// D:\유준환\141기\Frame141\Data
	Dlg.m_ofn.lpstrInitialDir = szPath;
	
	// DoModal : 대화 상자를 실행, IDOK 또는 IDCANCEL 반환
	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : 현재 다이얼로그가 선택괸 경로를 반환
		CString		str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwByte(0);
		DWORD		dwStrByte(0);

		for (auto& MyPair : m_mapUnitData)
		{
			dwStrByte = sizeof(TCHAR)* (MyPair.first.GetLength() + 1);

			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, MyPair.first.GetString(), dwStrByte, &dwByte, nullptr);

			WriteFile(hFile, &(MyPair.second->byItem), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iAttack), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iHp), sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnLoadData()
{
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE,	// TRUE(열기, 불러오기), FALSE(다른 이름으로 저장하기) 모드지정
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

	// DoModal : 대화 상자를 실행, IDOK 또는 IDCANCEL 반환
	if (IDOK == Dlg.DoModal())
	{

		for (auto& MyPair : m_mapUnitData)
			delete MyPair.second;

		m_mapUnitData.clear();

		// 기존의 리스트 박스의 목록을 초기화
		m_ListBox.ResetContent();

		CString		str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwByte(0);
		DWORD		dwStrByte(0);
		UNITDATA	tData{};

		while (true)
		{
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*	pName = new TCHAR[dwStrByte];
			
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			ReadFile(hFile, &(tData.byItem), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				break;
			}

			UNITDATA*		pUnit = new UNITDATA;
			pUnit->strName = pName;

			delete[]pName;
			pName = nullptr;

			pUnit->byItem = tData.byItem;
			pUnit->byJobIndex = tData.byJobIndex;
			pUnit->iAttack = tData.iAttack;
			pUnit->iHp = tData.iHp;

			m_mapUnitData.insert({ pUnit->strName, pUnit });

			m_ListBox.AddString(pUnit->strName);
			
		}
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}
