#pragma once

#include "supergridctrl.h"

// CDlgExportExcelSet 对话框

class CDlgExportExcelSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgExportExcelSet)

public:
	CDlgExportExcelSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgExportExcelSet();




// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EXCEL_EXPORTSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnChangeExportName();
	long OnUserSelectedGridItem(WPARAM wParam, LPARAM lParam);
	long OnUserCheck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();


	CString			m_strExportName;
	CSuperGridCtrl *m_pTreeGrid;
	CStatic			m_ImageFrame;
	int				m_nSelIndex{ -1 }; //移动的时候会用到该索引 

private:
	void move_node_up_down(LB_List &resultList, void *node, BOOL isUp);
	void move_node_top_bottom(LB_List &resultList, void *node, BOOL isTop);

public:
	void FillData();
public:
	afx_msg void OnBnClickedBtnTop();
	afx_msg void OnBnClickedBtnBottom();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
};
