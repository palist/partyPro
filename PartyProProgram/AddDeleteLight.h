#pragma once
#include <vector>
#include "SystemConfig.h"

// CAddDeleteLight 对话框

class CAddDeleteLight : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDeleteLight)

public:
	CAddDeleteLight(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDeleteLight();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_DELETE_LIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void insertData();
	afx_msg void OnBnClickedBtnFlush();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedOk();
	CListCtrl m_lib_list;
	CListCtrl m_add_list;
	CString get_current_path();
	void flushLibList();
	void SerializeToLight(CArchive& ar);
	
	void get_light_info(CString file_name);
	int m_region_index;
	int m_light_count;
};
