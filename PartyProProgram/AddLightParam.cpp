// AddLightParam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PartyProProgram.h"
#include "AddLightParam.h"
#include "afxdialogex.h"


// CAddLightParam �Ի���

IMPLEMENT_DYNAMIC(CAddLightParam, CDialogEx)

CAddLightParam::CAddLightParam(int start_addr_index,CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddLightParam::IDD, pParent)
	, m_add_light_count(0)
	, m_region_num(0)
	, m_start_addr(start_addr_index)
{

}

CAddLightParam::~CAddLightParam()
{
}

void CAddLightParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_CBIndex(pDX, IDC_COMBO1, m_add_light_count);
	//DDX_CBIndex(pDX, IDC_COMBO4, m_region_num);
	DDX_Text(pDX, IDC_EDIT1, m_start_addr);
}


BEGIN_MESSAGE_MAP(CAddLightParam, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddLightParam::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddLightParam ��Ϣ�������
BOOL CAddLightParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_COMBO1)->SetWindowTextA("1");
	GetDlgItem(IDC_COMBO4)->SetWindowTextA("1");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CAddLightParam::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(str);
	m_add_light_count = atoi(str);
	GetDlgItem(IDC_COMBO4)->GetWindowTextA(str);
	m_region_num = atoi(str);
	CDialogEx::OnOK();
}


