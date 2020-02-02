
// drawDlg.h : 头文件
//

#pragma once
#include "resource.h"

#include "DrawGrid.h"
#include "../PartyProProgram/PartyProProgramDlg.h"

// CdrawDlg 对话框
class CdrawDlg : public CDialog
{
// 构造
public:
	CdrawDlg(CWnd* pParent = NULL);	// 标准构造函数

public:
	static CdrawDlg* getContext(CWnd* pParent);
	static CdrawDlg* m_context;
	static void destroyMe();

// 对话框数据
	enum { IDD = IDD_DRAW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	CDrawGrid m_draw;

	CPartyProProgramDlg* m_parent;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_x_pos;
	int m_y_pos;
	afx_msg void OnEnChangeEditX();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnBnClickedBtnOk();
};
