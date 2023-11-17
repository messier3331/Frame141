#pragma once
//#include "afxdialogex.h"
#include "Incude.h"

// CPathFind 대화 상자

class CPathFinder : public CDialog
{
	DECLARE_DYNAMIC(CPathFinder)

public:
	CPathFinder(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPathFinder();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFINDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnListBox();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	virtual BOOL OnInitDialog();

private:
	void	SetMFCButton(CMFCButton& button);

	CListBox m_ListBox;
	list<IMGPATH*>	m_PathList;

	CMFCButton	m_SaveBtn;
	CMFCButton	m_LoadBtn;
	CFont		m_Font;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
