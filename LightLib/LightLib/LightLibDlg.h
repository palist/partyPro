
// LightLibDlg.h : ͷ�ļ�
//

#pragma once

#include "SystemConfig.h"


// CLightLibDlg �Ի���
class CLightLibDlg : public CDialogEx
{
// ����
public:
	CLightLibDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LIGHTLIB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void insert_bmp_to_list(int IDB_BMP);
	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_channel_list;
	CListCtrl m_icon_list;
	BOOL m_light_type;
	CString m_light_name;
	int m_ch_count;
	CString m_light_factory;
	int m_red_ch;
	int m_green_ch;
	int m_blue_ch;
	int m_white_ch;
	int m_x_ch;
	int m_y_ch;
	int m_x_ch_l;
	int m_y_ch_l;
	CImageList m_image_list;	//��׼ͼ����
	CImageList m_channel_image_list;	//�����õ�ͼ����
	CString m_channel_name[LIGHT_MAX_COUNT];
	int	m_channeal_image_index[LIGHT_MAX_COUNT];

	afx_msg void OnBnClickedBtnNew();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnSaveAs();	
	afx_msg void OnEnChangeEditChannelCount();

	int findImageIndex(CString name);
	void flushData();
	CString m_import_file_path;
	afx_msg void OnBnClickedBtnSetChName();
	afx_msg void OnEnChangeEditRed();
	afx_msg void OnEnChangeEditGreen();
	afx_msg void OnEnChangeEditBlue();
	afx_msg void OnEnChangeEditX();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnEnChangeEditXL();
	afx_msg void OnEnChangeEditYL();
	afx_msg void OnEnChangeEditWhite();
	afx_msg void OnBnClickedBtnWriter();

	afx_msg void OnNMDblclkListIcon(NMHDR* pNMHDR, LRESULT* pResult);
};
