// AddDeleteLight.cpp : 实现文件
//

#include "stdafx.h"
#include "PartyProProgram.h"
#include "afxdialogex.h"
#include "SelectLightGroup.h"
#include "AddLightParam.h"
//#include "../PartProRecord/PartProRecord/PartProRecordDlg.cpp"


// CSelectLightGroup 对话框

IMPLEMENT_DYNAMIC(CSelectLightGroup, CDialogEx)

CSelectLightGroup::CSelectLightGroup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectLightGroup::IDD, pParent)
{
	m_light_count = 0;
	m_single_flag = false;
	m_double_flag = false;
}

CSelectLightGroup::~CSelectLightGroup()
{
}

void CSelectLightGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADD, m_add_list);
}


BEGIN_MESSAGE_MAP(CSelectLightGroup, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectLightGroup::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_NONE, &CSelectLightGroup::OnBnClickedBtnNone)
	ON_BN_CLICKED(IDC_BTN_ALL, &CSelectLightGroup::OnBnClickedBtnAll)
	ON_BN_CLICKED(IDC_BTN_SINGLE, &CSelectLightGroup::OnBnClickedBtnSingle)
	ON_BN_CLICKED(IDC_BTN_DOUBLE, &CSelectLightGroup::OnBnClickedBtnDouble)
	ON_BN_CLICKED(IDC_BTN_COMMENT, &CSelectLightGroup::OnBnClickedBtnComment)
END_MESSAGE_MAP()


// CSelectLightGroup 消息处理程序



BOOL CSelectLightGroup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	CRect rect;


	// 获取编程语言列表视图控件的位置和大小   
	m_add_list.GetClientRect(&rect);


	// 为列表视图控件添加全行选中和栅格风格   
	m_add_list.SetExtendedStyle(m_add_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVIS_SELECTED | LVS_EX_CHECKBOXES);

	// 为列表视图控件添加三列   
	m_add_list.InsertColumn(0, _T("DMX地址"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_add_list.InsertColumn(1, _T("灯具"), LVCFMT_CENTER, rect.Width() * 2 / 3, 1);

	//// 在列表视图控件中插入列表项，并设置列表子项文本 

	insertData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSelectLightGroup::insertData()
{
	m_add_list.DeleteAllItems();
	int row = 0;
	CString str;
	std::vector<LIGHT_UNIT>::iterator light_iter;
	for (light_iter = g_Config.g_light_vec.begin(); light_iter != g_Config.g_light_vec.end(); light_iter++)
	{
		int start_pos = (*light_iter).start_channel;
		int region_index = (*light_iter).region_index;
		str.Format("%d:%d~%d", region_index+1, start_pos + 1, start_pos + (*light_iter).channel_count);
		m_add_list.InsertItem(row, str, row);
		str.Format("%d_%s", row + 1, (*light_iter).light_name);
		m_add_list.SetItemText(row, 1, str);
		m_add_list.SetCheck(row,(*light_iter).bGroup);
		row++;
	}
	UpdateData(false);
}




void CSelectLightGroup::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int row = 0;
	std::vector<LIGHT_UNIT>::iterator light_iter;
	for (light_iter = g_Config.g_light_vec.begin(); light_iter != g_Config.g_light_vec.end(); light_iter++)
	{
		(*light_iter).bGroup = m_add_list.GetCheck(row);
		row++;
	}

	CDialogEx::OnOK();
}


void CSelectLightGroup::OnBnClickedBtnNone()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i< m_add_list.GetItemCount();i++)
	{
		m_add_list.SetCheck(i, false);
	}
}


void CSelectLightGroup::OnBnClickedBtnAll()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_add_list.GetItemCount(); i++)
	{
		m_add_list.SetCheck(i, true);
	}
}


void CSelectLightGroup::OnBnClickedBtnSingle()
{
	// TODO: 在此添加控件通知处理程序代码
	int ind = 0;
	m_single_flag = !m_single_flag;
	for (int i = 0; i < m_add_list.GetItemCount(); i++)
	{
		if (ind++ % 2 == 0) {		
			m_add_list.SetCheck(i, m_single_flag);
		}	
	}
}


void CSelectLightGroup::OnBnClickedBtnDouble()
{
	// TODO: 在此添加控件通知处理程序代码
	int ind = 0;			
	m_double_flag = !m_double_flag;
	for (int i = 0; i < m_add_list.GetItemCount(); i++)
	{
		if (ind++ % 2 == 1) {

			m_add_list.SetCheck(i, m_double_flag);
		}		
	}
}


void CSelectLightGroup::OnBnClickedBtnComment()
{
	// TODO: 在此添加控件通知处理程序代码
}
