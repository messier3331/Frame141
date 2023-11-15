#pragma once
#include "afxwin.h"
#include "Incude.h"

// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnAddCharactor();
	afx_msg void OnListBox();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSearchData();

public:	
	// value
	CString m_strTest;
	CString m_strCopy;
	CString m_strName;
	int m_iHp;
	int m_iAttack;	
	
	CString m_strFindName;


public: // control
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];
	CButton m_Bitmap;


	map<CString, UNITDATA*>		m_mapUnitData;
	afx_msg void OnDeleteData();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
};

// 탐색(edit control 사용할 것)
// 삭제(button을 사용할 것)
// 저장, 불러오기 만들 것(button을 사용할 것)