
// ColorPickerDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"
#include "./ctrl/MouseEvent.h"
#include "../PartyProProgram/PartyProProgramDlg.h"

// CColorPickerDlg �Ի���
class CColorPickerDlg : public CDialog
{
// ����
public:
	CColorPickerDlg(CWnd* pParent = NULL);	// ��׼���캯��

public:
	static CColorPickerDlg* getContext(CWnd* pParent);
	static CColorPickerDlg* m_context;
	static void destroyMe();

// �Ի�������
	enum { IDD = IDD_COLORPICKERDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	//-------------------------------  zzw
	CMouseEvent m_event;
	CPartyProProgramDlg* m_parent;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
