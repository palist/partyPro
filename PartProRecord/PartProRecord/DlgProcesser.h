#pragma once
#include "afxcmn.h"
#include "resource.h"


// CDlgProcesser 对话框

class CDlgProcesser : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProcesser)

public:
	CDlgProcesser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProcesser();

	// 对话框数据
	enum { IDD = IDD_DLG_PROCESSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Proc;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void CreateProDlg(int flag);
};
