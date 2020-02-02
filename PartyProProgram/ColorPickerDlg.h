
// ColorPickerDlgDlg.h : 头文件
//

#pragma once
#include "resource.h"
#include "./ctrl/MouseEvent.h"
#include "../PartyProProgram/PartyProProgramDlg.h"

// CColorPickerDlg 对话框
class CColorPickerDlg : public CDialog
{
// 构造
public:
	CColorPickerDlg(CWnd* pParent = NULL);	// 标准构造函数

public:
	static CColorPickerDlg* getContext(CWnd* pParent);
	static CColorPickerDlg* m_context;
	static void destroyMe();

// 对话框数据
	enum { IDD = IDD_COLORPICKERDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	//-------------------------------  zzw
	CMouseEvent m_event;
	CPartyProProgramDlg* m_parent;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnNMCustomdrawSliderR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderB(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); 
	//////////////////////////////////////////////////////////////////////////


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_red;
	int m_green;
	int m_blue;
};
