#pragma once


// CAddLightParam �Ի���

class CAddLightParam : public CDialogEx
{
	DECLARE_DYNAMIC(CAddLightParam)

public:
	CAddLightParam(int start_addr_index,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddLightParam();

// �Ի�������
	enum { IDD = IDD_DLG_ADD_LIGHT_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_add_light_count;
	int m_region_num;
	UINT m_start_addr;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
