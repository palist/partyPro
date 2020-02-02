
// PartProRecordDlg.h : 头文件
//

#pragma once

#include "supergridctrl.h"
#include "DlgProcesser.h"
#include <vector>

// CPartProRecordDlg 对话框
class CPartProRecordDlg : public CDialogEx
{
// 构造
public:
	CPartProRecordDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPartProRecordDlg();
// 对话框数据
	enum { IDD = IDD_PARTPRORECORD_DIALOG };

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

	virtual void Serialize(CArchive& ar);		///序列化函数
	afx_msg void OnBnClickedBtnRecordOneKey();	///一键录制处理函数

	BOOL InitXTPRecordReport(void);	///初始化列表
	BOOL IsRecordBreakOff(void);	///突然中止录制函数

	void UpdateReocrdReport(void);	////初始化32个位置行，现在固定有32个节目位置
	void UpdateTVNameList(void);	///更新节目名称到下拉列表
	void SendRecordData(void);		///发送录制的数据
	void RecordAllSuccess(void);	///录制成功
	void CreateProDlg();	///创建进度条
	afx_msg void OnBnClickedCleanFestival();
	afx_msg void OnBnClickedCleanSelect();
	afx_msg void OnBnClickedLoadIn();
	// /常规关联列表
	CSuperGridCtrl *m_Record_Report;		// /常规关联列表	
	LRESULT OnNMDblclkList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnKillfocusComboKnife();
	int m_iSubItem;
	int m_iItem;
	CComboBox m_combo_normal;
	CComboBox m_combo_serior;
	std::vector<CString >record_vec;
	BOOL m_bExit;////是否录制停止 
	BOOL m_bIsProcessStart;///进度条存在标识
	BOOL m_bIsFinishOneData;///是否完成第一个消息头的发送
	int m_iIsFirstSend;///是否
	CDlgProcesser m_Proc;///进度条

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
};
