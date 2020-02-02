// AddLightParam.cpp : 实现文件
//

#include "stdafx.h"
#include "PartyProProgram.h"
#include "AddLightParam.h"
#include "afxdialogex.h"


// CAddLightParam 对话框

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


// CAddLightParam 消息处理程序
BOOL CAddLightParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_COMBO1)->SetWindowTextA("1");
	GetDlgItem(IDC_COMBO4)->SetWindowTextA("1");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAddLightParam::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(str);
	m_add_light_count = atoi(str);
	GetDlgItem(IDC_COMBO4)->GetWindowTextA(str);
	m_region_num = atoi(str);
	CDialogEx::OnOK();
}


