#pragma once
#include "afxcmn.h"
#include "resource.h"


// CDlgProcesser �Ի���

class CDlgProcesser : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProcesser)

public:
	CDlgProcesser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProcesser();

	// �Ի�������
	enum { IDD = IDD_DLG_PROCESSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Proc;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void CreateProDlg(int flag);
};
