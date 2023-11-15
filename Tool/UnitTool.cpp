#include "stdafx.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "UnitTool.h"
#include "FileInfo.h"


IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strObjName(_T("효선"))
	, m_iObjHp(1)
	, m_iObjAttack(0)
	, m_iObjSpeed(0)
	, m_tObjPos({ 0.f, 0.f, 0.f })
	, gen(random_device()())
	, m_strPresetFindName(_T(""))
	, m_fAxisX(0)
	, m_fAxisY(0)
{
}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fAxisY);
	DDX_Text(pDX, IDC_EDIT2, m_strObjName);
	DDX_Text(pDX, IDC_EDIT3, m_fAxisX);
	DDX_Text(pDX, IDC_EDIT4, m_iObjHp);
	DDX_Text(pDX, IDC_EDIT5, m_iObjSpeed);
	DDX_Text(pDX, IDC_EDIT7, m_iObjAttack);
	DDX_Text(pDX, IDC_EDIT8, m_strPresetFindName);
	DDX_Control(pDX, IDC_LIST1, m_PresetListBox);
	//DDX_Control(pDX, IDC_LIST3, m_PresetListCtrl);
	DDX_Control(pDX, IDC_PICTURE1, m_Picture);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnRandomReset)
	ON_BN_CLICKED(IDC_BUTTON10, &CUnitTool::OnSavePreset)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnErasePreset)
	ON_EN_CHANGE(IDC_EDIT8, &CUnitTool::OnSearchPresetData)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnPresetListBox)
	ON_BN_CLICKED(IDC_BUTTON8, &CUnitTool::OnSavePresetData)
	ON_BN_CLICKED(IDC_BUTTON9, &CUnitTool::OnLoadPresetData)
	//ON_NOTIFY(LVN_INSERTITEM, IDC_LIST3, &CUnitTool::OnAddPreset)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CUnitTool::OnPresetListBox)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기


void CUnitTool::OnSavePreset()
{
	UpdateData(TRUE);

	UNITDATA* pUnit = nullptr;

	auto& iter = m_mapUnitData.find(m_strObjName);

	if (iter != m_mapUnitData.end())
	{
		iter->second->iHp = m_iObjHp;
		iter->second->iAttack = m_iObjAttack;
		iter->second->iSpeed = m_iObjSpeed;
		iter->second->tPos.x = m_fAxisX;
		iter->second->tPos.y = m_fAxisY;
	}
	else
	{
		pUnit = new UNITDATA;
		pUnit->strName = m_strObjName;
		pUnit->iHp = m_iObjHp;
		pUnit->iAttack = m_iObjAttack;
		pUnit->iSpeed = m_iObjSpeed;
		pUnit->tPos.x = m_fAxisX;
		pUnit->tPos.y = m_fAxisY;

		m_PresetListBox.AddString(m_strObjName);
		//m_PresetListCtrl.InsertItem(LVIF_TEXT, m_PresetListCtrl.GetItemCount(), pUnit->strName, 0, 0, 0, 0);
		m_mapUnitData.insert({ pUnit->strName, pUnit });

	}

	UpdateData(FALSE);
}


void CUnitTool::OnRandomReset()
{
	UpdateData(TRUE);

	CString strTempName = L"";
	switch (GetRandomNumber(1, 7))
	{
	case 1:
		strTempName += L"발칙한";
		break;
	case 2:
		strTempName += L"낡은";
		break;
	case 3:
		strTempName += L"졸린";
		break;
	case 4:
		strTempName += L"생생한";
		break;
	case 5:
		strTempName += L"영롱한";
		break;
	case 6:
		strTempName += L"화난";
		break;
	default:
		strTempName += L"평범한";

		break;
	}
	strTempName += L"효선";
	strTempName.AppendFormat(L"%d", GetRandomNumber(1, 30));
	m_strObjName = strTempName;
	m_iObjHp = GetRandomNumber(1, 5);
	m_iObjAttack = GetRandomNumber(1, 3);
	m_iObjSpeed = GetRandomNumber(1, 2);
	m_tObjPos = { 0.f, 0.f, 0.f };

	UpdateData(FALSE);
}


void CUnitTool::OnAddPreset(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CUnitTool::OnPresetListBox(NMHDR* pNMHDR, LRESULT* pResult)
{
	int iIndex = m_PresetListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (iIndex == -1)
		return;

	/*LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);*/
	UpdateData(TRUE);

	CString strName = m_PresetListCtrl.GetItemText(iIndex, 0);

	auto iter = m_mapUnitData.find(strName);
	if (iter == m_mapUnitData.end())
		return;

	m_strObjName = iter->second->strName;
	m_iObjHp = iter->second->iHp;
	m_iObjAttack = iter->second->iAttack;
	m_iObjSpeed = iter->second->iSpeed;
	m_tObjPos = iter->second->tPos;

	auto	iter2 = m_mapPngImg.find(strName);

	if (iter2 != m_mapPngImg.end())
		m_Picture.SetBitmap(*(iter2->second));

	UpdateData(FALSE);

	//*pResult = 0;
}


void CUnitTool::OnErasePreset()
{
#pragma region 리스트컨트롤 관련
	//int iIndex = m_PresetListCtrl.GetNextItem(-1, LVNI_SELECTED);
	//if (iIndex == -1)
	//	return;

	//UpdateData(TRUE);

	//CString strName = L"";
	//strName = m_PresetListCtrl.GetItemText(iIndex, 0);

	//auto& iter = m_mapUnitData.find(strName);
	//if (iter == m_mapUnitData.end())
	//	return;

	//Safe_Delete(iter->second);
	//m_mapUnitData.erase(strName);
	//m_PresetListCtrl.DeleteItem(iIndex);

	//UpdateData(FALSE);

#pragma endregion

	UpdateData(TRUE);

	CString	strFindName = L"";

	int	iIndex = m_PresetListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	m_PresetListBox.GetText(iIndex, strFindName);

	auto& iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);

	m_PresetListBox.DeleteString(iIndex);


	UpdateData(FALSE);
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	//m_PresetListCtrl.SetIconSpacing(CSize(90, 40));

	return TRUE;
}

int CUnitTool::FindItemByName(CListCtrl& listCtrl, const CString& strName)
{
	int iIndex = listCtrl.GetItemCount();
	for (int i = 0; i < iIndex; ++i)
	{
		CString strItemText = listCtrl.GetItemText(i, 0);
		if (strItemText.CompareNoCase(strName) == 0)
		{
			return i;
		}
	}
	return -1;
}


BOOL CUnitTool::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(120, 120, 150));

	return TRUE;
}


HBRUSH CUnitTool::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 모든 텍스트 컨트롤의 색상을 흰색으로 설정
	if (nCtlColor == CTLCOLOR_STATIC ||
		nCtlColor == CTLCOLOR_EDIT ||
		nCtlColor == CTLCOLOR_BTN ||
		nCtlColor == CTLCOLOR_LISTBOX)
	{
		static CBrush brBkgnd(RGB(120, 120, 150));
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(120, 120, 150));
		hbr = brBkgnd;
	}

	return hbr;
}


void CUnitTool::OnSearchPresetData()
{
	UpdateData(TRUE);

	auto& iter = m_mapUnitData.find(m_strPresetFindName);
	if (iter == m_mapUnitData.end())
		return;

	//int iIndex = FindItemByName(m_PresetListCtrl, m_strPresetFindName);
	//m_PresetListCtrl.SetItemState(-1, 0, LVIS_SELECTED);
	//m_PresetListCtrl.SetItemState(iIndex, LVIS_SELECTED, LVIS_SELECTED);
	//m_PresetListCtrl.EnsureVisible(iIndex, FALSE);


	int		iIndex = m_PresetListBox.FindString(0, m_strPresetFindName);
	if (LB_ERR == iIndex)
		return;

	m_PresetListBox.SetCurSel(iIndex);

	m_strObjName = iter->second->strName;
	m_iObjHp = iter->second->iHp;
	m_iObjAttack = iter->second->iAttack;
	m_iObjSpeed = iter->second->iSpeed;
	m_fAxisX = iter->second->tPos.x;
	m_fAxisY = iter->second->tPos.y;

	UpdateData(FALSE);
}


void CUnitTool::OnSavePresetData()
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
		CString str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte(0);
		DWORD dwStrByte(0);

		for (auto& myPair : m_mapUnitData)
		{
			dwStrByte = sizeof(TCHAR) * (myPair.first.GetLength() + 1);

			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, myPair.first.GetString(), dwStrByte, &dwByte, nullptr);

			WriteFile(hFile, &(myPair.second->tPos), sizeof(D3DXVECTOR3), &dwByte, nullptr);
			WriteFile(hFile, &(myPair.second->iHp), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(myPair.second->iAttack), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(myPair.second->iSpeed), sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnLoadPresetData()
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
		m_PresetListCtrl.DeleteAllItems();

		CString strName = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strName.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwByte(0);
		DWORD		dwStrByte(0);
		UNITDATA	tData{};

		while (true)
		{
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR* pName = new TCHAR[dwStrByte];

			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			ReadFile(hFile, &(tData.tPos), sizeof(D3DXVECTOR3), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iSpeed), sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				break;
			}

			UNITDATA* pUnit = new UNITDATA;
			pUnit->strName = pName;

			delete[]pName;
			pName = nullptr;

			pUnit->iHp = tData.iHp;
			pUnit->iAttack = tData.iAttack;
			pUnit->iSpeed = tData.iSpeed;
			pUnit->tPos = tData.tPos;

			m_mapUnitData.insert({ pUnit->strName, pUnit });
			m_PresetListCtrl.InsertItem(LVIF_TEXT, m_PresetListCtrl.GetItemCount(), pUnit->strName, 0, 0, 0, 0);
		}
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


void CUnitTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MAX_STR] = L"";

	int a = 0;
	DragQueryFile(hDropInfo, 0, szFilePath, MAX_PATH);

	CString	strRelative = CFileInfo::ConvertRelativePath(szFilePath);
	CString	strFileName = PathFindFileName(strRelative);

	lstrcpy(szFileName, strFileName.GetString());

	PathRemoveExtension(szFileName);

	strFileName = szFileName;


	CString strName;
	int iIndex = m_PresetListBox.GetCurSel();
	m_PresetListBox.GetText(iIndex, strName);

	//int iIndex = m_PresetListCtrl.GetNextItem(-1, LVNI_SELECTED);
	//if (iIndex == -1)
	//{
	//	UpdateData(FALSE);
	//	return;
	//}
	//CString strName = m_PresetListCtrl.GetItemText(iIndex, 0);

	auto iter = m_mapPngImg.find(strName);

	if (iter == m_mapPngImg.end())
	{
		CImage* pPngImg = new CImage;
		pPngImg->Load(strRelative);

		m_mapPngImg.insert({ strName, pPngImg });
		m_Picture.SetBitmap(*pPngImg);
	}
	else
	{
		CImage* pPngImg = new CImage;
		//TODO: 원래 해당 key에 저장된 이미지를 safe delete?

		pPngImg->Load(strRelative);

		iter->second = pPngImg;
		m_Picture.SetBitmap(*(iter->second));
	}

	UpdateData(FALSE);
}


void CUnitTool::OnPresetListBox()
{
	UpdateData(TRUE);

	CString		strSelectedName;

	int iIndex = m_PresetListBox.GetCurSel();
	m_PresetListBox.GetText(iIndex, strSelectedName);

	auto iter = m_mapUnitData.find(strSelectedName);

	if (iter == m_mapUnitData.end())
		return;

	m_strObjName = iter->second->strName;
	m_iObjHp = iter->second->iHp;
	m_iObjAttack = iter->second->iAttack;
	m_iObjSpeed = iter->second->iSpeed;
	m_fAxisX = iter->second->tPos.x;
	m_fAxisY = iter->second->tPos.y;

	auto iter2 = m_mapPngImg.find(m_strObjName);

	if (iter2 != m_mapPngImg.end())
	{
		m_Picture.SetBitmap(*(iter2->second));
	}
	else
	{
		//m_Picture.res
	}
	UpdateData(FALSE);
}
