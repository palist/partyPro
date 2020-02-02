// AddDeleteLight.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PartyProProgram.h"
#include "AddDeleteLight.h"
#include "afxdialogex.h"
#include "AddLightParam.h"
//#include "../PartProRecord/PartProRecord/PartProRecordDlg.cpp"


// CAddDeleteLight �Ի���

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


// CAddDeleteLight ��Ϣ�������

CString CAddDeleteLight::get_current_path()
{
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // ɾ���ļ�����ֻ���·���ִ�
	CString szPath = szFilePath;
	return szPath;
}

void CAddDeleteLight::flushLibList()
{
	m_lib_list.DeleteAllItems();
	int row = 0;
	////�����ǲ���ͬ��
	CString szPath = get_current_path();
	CFileFind finder;
	CString strPath, strFileName, strTVName, strNum;
	int iCount = 0;
	strPath.Format(_T("%slight lib//*.light"), szPath);////���ļ���·��
	BOOL bWorking = finder.FindFile(strPath);////��������Program file
	///��ʼѭ������Program file
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


	CRect rect;

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_lib_list.GetClientRect(&rect);


	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_lib_list.SetExtendedStyle(m_lib_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVIS_SELECTED);

	// Ϊ�б���ͼ�ؼ��������   
	m_lib_list.InsertColumn(0, _T("������"), LVCFMT_CENTER, rect.Width() , 0);

	// ���б���ͼ�ؼ��в����б���������б������ı� 
	flushLibList();



	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_add_list.GetClientRect(&rect);


	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_add_list.SetExtendedStyle(m_add_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVIS_SELECTED);

	// Ϊ�б���ͼ�ؼ��������   
	m_add_list.InsertColumn(0, _T("DMX��ַ"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_add_list.InsertColumn(1, _T("�ƾ�"), LVCFMT_CENTER, rect.Width() * 2 / 3, 1);

	//// ���б���ͼ�ؼ��в����б���������б������ı� 

	insertData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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


///��д���л�����
void CAddDeleteLight::SerializeToLight(CArchive& ar)
{
	if (ar.IsStoring())///��� �Ǵ�������
	{
	}
	else///����Ǽ�������
	{
		LIGHT_UNIT light_unit;
		long long sizeCount = 0;
		ar >> sizeCount;
		//if (sizeCount != sizeof(LIGHT_UNIT))
		//{
		//	AfxMessageBox("��ȡʧ�ܣ��������ļ�Ϊ��������ɣ�");
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
	////�����ǲ���ͬ��
	CString szPath = get_current_path();
	CFileFind finder;
	CString strPath, strFileName, strTVName, strNum;
	int iCount = 0;
	strPath.Format(_T("%slight lib//%s.light"), szPath, file_name);////���ļ���·��
	BOOL bWorking = finder.FindFile(strPath);////��������Program file
	///��ʼѭ������Program file
	if (bWorking)
	{
		bWorking = finder.FindNextFile();
		strFileName = finder.GetFilePath();
		CFile file;
		int iRet = file.Open(strFileName, CFile::modeRead);///�����ļ�
		///�����ļ�ʧ�ܾ��˳�
		if (iRet == 0)
		{
			AfxMessageBox("�ļ���ʧ�ܣ�");
			return;
		}
		CArchive archive(&file, CArchive::load);//���л�д���ļ�
		SerializeToLight(archive);
		archive.Close();
		file.Close();
	}
}

void CAddDeleteLight::OnBnClickedBtnFlush()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flushLibList();
}


void CAddDeleteLight::OnBnClickedBtnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CDialogEx::OnOK();
}
