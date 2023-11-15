#pragma once
#include "afxdialogex.h"
#include "Incude.h"

// CUnitTool 대화 상자

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUnitTool();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	//Value
	CString			m_strObjName;
	int				m_iObjHp;
	int				m_iObjAttack;
	int				m_iObjSpeed;
	D3DXVECTOR3		m_tObjPos;
	mt19937			gen;


	//Control
	afx_msg void	OnSavePreset();
	afx_msg void	OnRandomReset();
	afx_msg void	OnAddPreset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void	OnPresetListBox(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void	OnErasePreset();
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnSearchPresetData();
	afx_msg void	OnSavePresetData();
	afx_msg void	OnLoadPresetData();
	afx_msg void	OnDropFiles(HDROP hDropInfo);


	CListBox	m_PresetListBox;
	CListCtrl	m_PresetListCtrl;
	map<CString, UNITDATA*>	m_mapUnitData;
	map<CString, CImage*>		m_mapPngImg;


private:
	int	GetRandomNumber(int min, int max)
	{
		uniform_int_distribution<int> distrib(min, max);
		int fNum = distrib(gen);
		return fNum;
	}
	virtual BOOL OnInitDialog();
	int		FindItemByName(CListCtrl& listCtrl, const CString& strName);

public:
	
	CString m_strPresetFindName;
	CStatic m_Picture;
	float m_fAxisX;
	float m_fAxisY;
	afx_msg void OnPresetListBox();
};
