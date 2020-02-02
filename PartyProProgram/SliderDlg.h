#pragma once

#include "SystemConfig.h"
#include "afxwin.h"
#include "afxcmn.h"

// CSliderDlg 对话框

class CSliderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSliderDlg)

public:
	CSliderDlg(SCENE* &Data,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSliderDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SPLITER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	SCENE* m_curScene;

	virtual BOOL OnInitDialog();
	

public:
	CSliderCtrl m_SliderCtrl[SHOW_COUNT];
	CEdit m_EditRemark[SHOW_COUNT];
	CEdit m_EditVar[SHOW_COUNT];
	CEdit m_EditChannel[SHOW_COUNT];
	CButton m_VoiceCheck[SHOW_COUNT];

	CScrollBar m_scrollBar;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg
		void GroupDetal(int index);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void M_UpdateData(bool status);
	CEdit m_edit_total;
	CSliderCtrl m_slider_total;
	afx_msg void OnBnClickedBtnAllClear();
	afx_msg void OnBnClickedBtnAllOn();
	afx_msg void OnBnClickedCheckTotal();
	void OnButtonClicked(UINT uId);
	void OnRemarkEdit(UINT uId);
	void OnChannelEdit(UINT uId);
	bool bUseUpdate;
	virtual BOOL PreTranslateMessage(MSG* pMsg);  // PreTranslateMessage是消息在送给TranslateMessage函数之前被调用的
	virtual void OnOK();
};
