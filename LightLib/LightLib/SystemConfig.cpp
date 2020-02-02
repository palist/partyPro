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
	int iRet =file.Open(path,CFile::modeRead);///创建文件
	///创建文件失败就退出
	if(iRet ==0)
	{
		AfxMessageBox("文件打开失败！");
		return;
	}
	CArchive archive(&file, CArchive::load);//序列化写入文件
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
	int iRet =file.Open(path,CFile::modeCreate | CFile::modeWrite);///创建文件
	///创建文件失败就退出
	if(iRet ==0)
	{
		AfxMessageBox("文件打开失败！");
		return;
	}
	CArchive archive(&file, CArchive::store);//序列化写入文件
	Serialize(archive);
	archive.Close();
	file.Close();
	
}




///重写序列化函数
void CSystemConfig::Serialize(CArchive& ar)
{
	if (ar.IsStoring())///如果 是储存数据
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
	else///如果是加载数据
	{	
		long long sizeCount = 0;
		ar >> sizeCount;
		if(sizeCount!=sizeof(LIGHT_UNIT))
		{
			AfxMessageBox("此配置文件为旧软件生成，请删除原配置并重新生成配置文件！");
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

