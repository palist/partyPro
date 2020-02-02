#pragma once
#include "resource.h"
#include "afxwin.h"
#include "supergridctrl.h"
#include "Port.h"
#include <vector>

// CDlgPort 对话框

class CDlgPort : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPort)

public:
	CDlgPort(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPort();
public:
	static CDlgPort *getContext();
	static CDlgPort *m_context;
	static void destroyMe();
// 对话框数据
	enum { IDD = IDD_DLG_MUL_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_EditSendStr;
	CString m_EditRecvStr;
	CString m_strDataRXTemp ;
	//CComboBox m_ComboSerial;
	CStatic m_static_port_zone;
	CSuperGridCtrl * m_pTreeGrid;
	int m_iSubItem;
	int m_iItem;
	CPort* m_curData;
	std::vector<CPort*>port_vec;
	std::vector<CPort*>::iterator vec_iter;
public:
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnClear2();
	afx_msg void OnBnClickedBtnConnect();
	void CheckComIsExist(void);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnUserSeletedItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserClickedSeleted(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT msgProg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedBtnResh();
	afx_msg void OnBnClickedBtnOpen();
};
