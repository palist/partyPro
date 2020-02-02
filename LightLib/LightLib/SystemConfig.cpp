#include "StdAfx.h"
#include "SystemConfig.h"


CSystemConfig g_Config;
//CString SYSTEM_PARAM_PATH  = _T(".//param.light");



CSystemConfig::CSystemConfig(void)
{

}


CSystemConfig::~CSystemConfig(void)
{

}


void CSystemConfig::read(CString path)
{
	if (path.Compare("")==0)
	{
		path = ".//param.light";
	}
	CFile file;
	int iRet =file.Open(path,CFile::modeRead);///�����ļ�
	///�����ļ�ʧ�ܾ��˳�
	if(iRet ==0)
	{
		AfxMessageBox("�ļ���ʧ�ܣ�");
		return;
	}
	CArchive archive(&file, CArchive::load);//���л�д���ļ�
	Serialize(archive);
	archive.Close();
	file.Close();

}


void CSystemConfig::save(CString path){
	if (path.Compare("") == 0)
	{
		path = ".//param.light";
	}
	CFile file;
	int iRet =file.Open(path,CFile::modeCreate | CFile::modeWrite);///�����ļ�
	///�����ļ�ʧ�ܾ��˳�
	if(iRet ==0)
	{
		AfxMessageBox("�ļ���ʧ�ܣ�");
		return;
	}
	CArchive archive(&file, CArchive::store);//���л�д���ļ�
	Serialize(archive);
	archive.Close();
	file.Close();
	
}




///��д���л�����
void CSystemConfig::Serialize(CArchive& ar)
{
	if (ar.IsStoring())///��� �Ǵ�������
	{	
		ar << (long long)sizeof(LIGHT_UNIT);
		ar << g_light_unit.light_type;
		ar << g_light_unit.light_name;
		ar << g_light_unit.channel_count;
		for (int i = 0; i < g_light_unit.channel_count;i++) {
			ar << g_light_unit.channel_name[i];
		}
		ar << g_light_unit.light_factory;
		ar << g_light_unit.red_ch;
		ar << g_light_unit.green_ch;
		ar << g_light_unit.blue_ch;
		ar << g_light_unit.x_ch;
		ar << g_light_unit.y_ch;
		ar << g_light_unit.x_ch_l;
		ar << g_light_unit.y_ch_l;
		ar << g_light_unit.white_ch;

	}
	else///����Ǽ�������
	{	
		long long sizeCount = 0;
		ar >> sizeCount;
		if(sizeCount!=sizeof(LIGHT_UNIT))
		{
			AfxMessageBox("�������ļ�Ϊ��������ɣ���ɾ��ԭ���ò��������������ļ���");
			DeleteFile(".//param.light");
			return;
		}
		ar >> g_light_unit.light_type;
		ar >> g_light_unit.light_name;
		ar >> g_light_unit.channel_count;
		for (int i = 0; i < g_light_unit.channel_count; i++) {
			ar >> g_light_unit.channel_name[i];
		}
		ar >> g_light_unit.light_factory;
		ar >> g_light_unit.red_ch;
		ar >> g_light_unit.green_ch;
		ar >> g_light_unit.blue_ch;
		ar >> g_light_unit.x_ch;
		ar >> g_light_unit.y_ch;
		ar >> g_light_unit.x_ch_l;
		ar >> g_light_unit.y_ch_l;
		ar >> g_light_unit.white_ch;
	}
}

