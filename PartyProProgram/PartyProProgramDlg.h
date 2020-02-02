
// PartyProProgramDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "SliderDlg.h"
#include "supergridctrl.h"
#include "afxcmn.h"


// CPartyProProgramDlg 对话框
class CPartyProProgramDlg : public CDialogEx
{
// 构造
public:
	CPartyProProgramDlg(CWnd* pParent = NULL);	// 标准构造函数
	CPartyProProgramDlg::~CPartyProProgramDlg();
// 对话框数据
	enum { IDD = IDD_PARTYPROPROGRAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_redio_group;
	CComboBox m_combo_region;
	CComboBox m_combo_WaitTime;
	CComboBox m_combo_SharpTime;
	CSuperGridCtrl * m_pTreeGrid;
	int m_iSubItem;
	int m_iItem;
	CEdit m_edit_var;	
	SCENE *m_curData;
	CRigion* m_cutRigion;
	CWinThread* m_pThread;     // 线程对象指针
	bool bThreadExit;	//线程退出

	CWinThread* m_pRecordThread;     // 线程对象指针
	bool bRecordThreadExit;	//线程退出


public:
	afx_msg void GroupDetal_color(int red, int green, int blue);
	void OnBnClickedBtnXyControl();
	afx_msg void OnBnClickedBtnColorControl();
	CStatic m_static_sliterDlg_zone;
	CSliderDlg *m_sliderDlg;
	afx_msg void OnButtonClick(UINT nID);
	afx_msg void OnCbnSelchangeComboRegion();
	CStatic m_static_scene_list_zone;
	afx_msg void OnBnClickedBtnInterScene();
	afx_msg void OnBnClickedBtnDeleteScene();
	afx_msg LRESULT OnUserSeletedItem(WPARAM wParam, LPARAM lParam);
	LRESULT OnNMDblclkList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnKillfocusGreeitemEdit();
	afx_msg void OnCbnKillfocusComboKnife();	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void GroupDetal_xy(int x_pos, int y_pos);
	void flashGridctrl();
	void rushGridctrlData(CItemInfo *info);
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnFlushSceneList(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedBtnOpenFile();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnStop();
	BOOL m_check_loop;
	afx_msg void OnBnClickedCheckLoop();
	afx_msg void OnBnClickedBtnRecord();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedChangeChannelValue();
	BOOL m_channel_type;
	BOOL m_control_select;
	afx_msg void OnBnClickedCheckControlSelect();
	afx_msg void OnCbnSelchangeComboChannelType();
	afx_msg void OnBnClickedBtnSaveFile();
	afx_msg void OnAddDeleteLight();
	afx_msg void OnOpenLightLib();
	afx_msg void OnBnClickedBtnSelectLight();
	afx_msg void OnRecord();
};
