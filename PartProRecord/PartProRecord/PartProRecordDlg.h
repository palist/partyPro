
// PartProRecordDlg.h : ͷ�ļ�
//

#pragma once

#include "supergridctrl.h"
#include "DlgProcesser.h"
#include <vector>

// CPartProRecordDlg �Ի���
class CPartProRecordDlg : public CDialogEx
{
// ����
public:
	CPartProRecordDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CPartProRecordDlg();
// �Ի�������
	enum { IDD = IDD_PARTPRORECORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


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

	virtual void Serialize(CArchive& ar);		///���л�����
	afx_msg void OnBnClickedBtnRecordOneKey();	///һ��¼�ƴ�����

	BOOL InitXTPRecordReport(void);	///��ʼ���б�
	BOOL IsRecordBreakOff(void);	///ͻȻ��ֹ¼�ƺ���

	void UpdateReocrdReport(void);	////��ʼ��32��λ���У����ڹ̶���32����Ŀλ��
	void UpdateTVNameList(void);	///���½�Ŀ���Ƶ������б�
	void SendRecordData(void);		///����¼�Ƶ�����
	void RecordAllSuccess(void);	///¼�Ƴɹ�
	void CreateProDlg();	///����������
	afx_msg void OnBnClickedCleanFestival();
	afx_msg void OnBnClickedCleanSelect();
	afx_msg void OnBnClickedLoadIn();
	// /��������б�
	CSuperGridCtrl *m_Record_Report;		// /��������б�	
	LRESULT OnNMDblclkList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnKillfocusComboKnife();
	int m_iSubItem;
	int m_iItem;
	CComboBox m_combo_normal;
	CComboBox m_combo_serior;
	std::vector<CString >record_vec;
	BOOL m_bExit;////�Ƿ�¼��ֹͣ 
	BOOL m_bIsProcessStart;///���������ڱ�ʶ
	BOOL m_bIsFinishOneData;///�Ƿ���ɵ�һ����Ϣͷ�ķ���
	int m_iIsFirstSend;///�Ƿ�
	CDlgProcesser m_Proc;///������

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
};
