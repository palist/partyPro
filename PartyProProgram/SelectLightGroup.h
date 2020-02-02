#pragma once
#include <vector>
#include "SystemConfig.h"

// CSelectLightGroup �Ի���

class CSelectLightGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectLightGroup)

public:
	CSelectLightGroup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectLightGroup();

// �Ի�������
	enum { IDD = IDD_DLG_SELECT_LIGHT_GROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void insertData();
	afx_msg void OnBnClickedOk();
	CListCtrl m_add_list;
	int m_region_index;
	int m_light_count;
	afx_msg void OnBnClickedBtnNone();
	afx_msg void OnBnClickedBtnAll();
	afx_msg void OnBnClickedBtnSingle();
	afx_msg void OnBnClickedBtnDouble();
	afx_msg void OnBnClickedBtnComment();
	bool m_single_flag;
	bool m_double_flag;
};
