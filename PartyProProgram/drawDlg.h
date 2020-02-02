
// drawDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"

#include "DrawGrid.h"
#include "../PartyProProgram/PartyProProgramDlg.h"

// CdrawDlg �Ի���
class CdrawDlg : public CDialog
{
// ����
public:
	CdrawDlg(CWnd* pParent = NULL);	// ��׼���캯��

public:
	static CdrawDlg* getContext(CWnd* pParent);
	static CdrawDlg* m_context;
	static void destroyMe();

// �Ի�������
	enum { IDD = IDD_DRAW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	CDrawGrid m_draw;

	CPartyProProgramDlg* m_parent;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
