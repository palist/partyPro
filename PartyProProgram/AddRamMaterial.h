#pragma once
#include "afxwin.h"

class CSuperGridCtrl;


// CAddRamMaterial 对话框

class CAddRamMaterial : public CDialogEx
{
	DECLARE_DYNAMIC(CAddRamMaterial)

public:
	CAddRamMaterial(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddRamMaterial();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RAW_MATERIAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg LRESULT OnUserSeletedItem(WPARAM wParam, LPARAM lParam);
	LRESULT OnNMDblclkList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnKillfocusGreeitemEdit();
	afx_msg void OnCbnKillfocusComboKnife();

	DECLARE_MESSAGE_MAP()

public:
	CSuperGridCtrl * m_pTreeGrid;
	CMaterialData* m_curData;

	CSuperGridCtrl *m_pCurGrid;

	CSuperGridCtrl * m_pTreeGridDoorCore;
	CDoorCoreMaterial* m_curDataDoorCore;
	int m_iSubItem;
	int m_iItem;
	CString m_strItemTextEdit;
	CEdit m_SubItemEdit;

public:
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnDeleteAll();
	afx_msg void OnBnClickedBtnAdd2();
	afx_msg void OnBnClickedBtnDelete2();
	afx_msg void OnBnClickedBtnDeleteAll2();
	afx_msg void OnBnClickedOk();
	CComboBox m_isRotate;
	CStatic m_profile_zone;
	CStatic m_doorcore_zone;
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedBtnImport2();
	afx_msg void OnBnClickedBtnExport2();
};
