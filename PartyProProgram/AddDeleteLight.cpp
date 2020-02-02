// AddDeleteLight.cpp : 实现文件
//

#include "stdafx.h"
#include "PartyProProgram.h"
#include "AddDeleteLight.h"
#include "afxdialogex.h"
#include "AddLightParam.h"
//#include "../PartProRecord/PartProRecord/PartProRecordDlg.cpp"


// CAddDeleteLight 对话框

IMPLEMENT_DYNAMIC(CAddDeleteLight, CDialogEx)

CAddDeleteLight::CAddDeleteLight(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddDeleteLight::IDD, pParent)
{
	m_light_count = 0;
	std::vector<LIGHT_UNIT>::iterator light_iter;
	for (light_iter = g_Config.g_light_vec.begin(); light_iter != g_Config.g_light_vec.end(); light_iter++)
	{
		m_light_count += (*light_iter).channel_count;
	}
	
}

CAddDeleteLight::~CAddDeleteLight()
{
}

void CAddDeleteLight::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LIB, m_lib_list);
	DDX_Control(pDX, IDC_LIST_ADD, m_add_list);
}


BEGIN_MESSAGE_MAP(CAddDeleteLight, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_FLUSH, &CAddDeleteLight::OnBnClickedBtnFlush)
	ON_BN_CLICKED(IDC_BTN_ADD, &CAddDeleteLight::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CAddDeleteLight::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDOK, &CAddDeleteLight::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddDeleteLight 消息处理程序

CString CAddDeleteLight::get_current_path()
{
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
	CString szPath = szFilePath;
	return szPath;
}

void CAddDeleteLight::flushLibList()
{
	m_lib_list.DeleteAllItems();
	int row = 0;
	////查找是不是同名
	CString szPath = get_current_path();
	CFileFind finder;
	CString strPath, strFileName, strTVName, strNum;
	int iCount = 0;
	strPath.Format(_T("%slight lib//*.light"), szPath);////打开文件的路径
	BOOL bWorking = finder.FindFile(strPath);////搜索所有Program file
	///开始循环所有Program file
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strFileName = finder.GetFileName();
		int iPos = strFileName.Find(_T("."));
		if (iPos == -1)continue;
		m_lib_list.InsertItem(row, strFileName.Left(iPos), row);
		row++;
	}

}

BOOL CAddDeleteLight::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	CRect rect;

	// 获取编程语言列表视图控件的位置和大小   
	m_lib_list.GetClientRect(&rect);


	// 为列表视图控件添加全行选中和栅格风格   
	m_lib_list.SetExtendedStyle(m_lib_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVIS_SELECTED);

	// 为列表视图控件添加三列   
	m_lib_list.InsertColumn(0, _T("灯类型"), LVCFMT_CENTER, rect.Width() , 0);

	// 在列表视图控件中插入列表项，并设置列表子项文本 
	flushLibList();



	// 获取编程语言列表视图控件的位置和大小   
	m_add_list.GetClientRect(&rect);


	// 为列表视图控件添加全行选中和栅格风格   
	m_add_list.SetExtendedStyle(m_add_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVIS_SELECTED);

	// 为列表视图控件添加三列   
	m_add_list.InsertColumn(0, _T("DMX地址"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_add_list.InsertColumn(1, _T("灯具"), LVCFMT_CENTER, rect.Width() * 2 / 3, 1);

	//// 在列表视图控件中插入列表项，并设置列表子项文本 

	insertData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAddDeleteLight::insertData()
{
	m_add_list.DeleteAllItems();
	int row = 0;
	CString str;
	std::vector<LIGHT_UNIT>::iterator light_iter;
	for (light_iter = g_Config.g_light_vec.begin(); light_iter != g_Config.g_light_vec.end(); light_iter++)
	{
		int start_pos = (*light_iter).start_channel;
		int region_index = (*light_iter).region_index;
		str.Format("%d:%d~%d", region_index+1, start_pos+1, start_pos + (*light_iter).channel_count);
		m_add_list.InsertItem(row, str, row);
		str.Format("%d_%s", row + 1, (*light_iter).light_name);
		m_add_list.SetItemText(row, 1, str);

		for (int n = 0; n < (*light_iter).channel_count; n++) {
			g_Config.g_rigion[region_index].RemarkName[n + start_pos] = (*light_iter).channel_name[n];
		}

		row++;
	}
	UpdateData(false);
}


///重写序列化函数
void CAddDeleteLight::SerializeToLight(CArchive& ar)
{
	if (ar.IsStoring())///如果 是储存数据
	{
	}
	else///如果是加载数据
	{
		LIGHT_UNIT light_unit;
		long long sizeCount = 0;
		ar >> sizeCount;
		//if (sizeCount != sizeof(LIGHT_UNIT))
		//{
		//	AfxMessageBox("读取失败，此配置文件为旧软件生成！");
		//	return ;
		//}
		ar >> light_unit.light_type;
		ar >> light_unit.light_name;
		ar >> light_unit.channel_count;
		for (int i = 0; i < light_unit.channel_count; i++) {
			ar >> light_unit.channel_name[i];
		}
		ar >> light_unit.light_factory;

		light_unit.region_index = m_region_index;
		light_unit.start_channel = m_light_count;
		light_unit.bGroup = false;

		g_Config.g_light_vec.push_back(light_unit);
		m_light_count += light_unit.channel_count;
	}
	
}


void CAddDeleteLight::get_light_info(CString file_name)
{
	////查找是不是同名
	CString szPath = get_current_path();
	CFileFind finder;
	CString strPath, strFileName, strTVName, strNum;
	int iCount = 0;
	strPath.Format(_T("%slight lib//%s.light"), szPath, file_name);////打开文件的路径
	BOOL bWorking = finder.FindFile(strPath);////搜索所有Program file
	///开始循环所有Program file
	if (bWorking)
	{
		bWorking = finder.FindNextFile();
		strFileName = finder.GetFilePath();
		CFile file;
		int iRet = file.Open(strFileName, CFile::modeRead);///创建文件
		///创建文件失败就退出
		if (iRet == 0)
		{
			AfxMessageBox("文件打开失败！");
			return;
		}
		CArchive archive(&file, CArchive::load);//序列化写入文件
		SerializeToLight(archive);
		archive.Close();
		file.Close();
	}
}

void CAddDeleteLight::OnBnClickedBtnFlush()
{
	// TODO: 在此添加控件通知处理程序代码
	flushLibList();
}


void CAddDeleteLight::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int icon_index = m_lib_list.GetSelectionMark();
	CString light_name = m_lib_list.GetItemText(icon_index,0);
	

	CAddLightParam dlg(m_light_count+1);
	dlg.DoModal();	
	m_region_index = dlg.m_region_num-1;
	m_light_count = dlg.m_start_addr-1;
	for (int i = 0;i< dlg.m_add_light_count; i++) {
		get_light_info(light_name);
	}
	
	insertData();

}


void CAddDeleteLight::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int icon_index = m_add_list.GetSelectionMark();
	if (icon_index >= 0 && icon_index < g_Config.g_light_vec.size())
	{
		int i = 0;
		std::vector<LIGHT_UNIT>::iterator light_iter;
		for (light_iter = g_Config.g_light_vec.begin(); light_iter != g_Config.g_light_vec.end(); light_iter++)
		{
			if (icon_index == i++) {
				g_Config.g_light_vec.erase(light_iter);
				break;
			}
		}
	}
	insertData();
}


void CAddDeleteLight::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码


	CDialogEx::OnOK();
}
