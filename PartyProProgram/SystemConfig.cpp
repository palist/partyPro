#include "stdafx.h"
#include "SystemConfig.h"
#include "DlgPort.h"

CSystemConfig g_Config;
CString SYSTEM_PARAM_PATH  = _T(".//systemConfig.ini");


CRigion::CRigion(void)
{
	//m_curScene = new SCENE;
	///memset(m_curScene,0,sizeof(SCENE));
	//m_curScene = nullptr;
	rigion_num = 0;
}

CRigion::~CRigion(void)
{	
	std::vector<SCENE*>vec_temp;
	scene_vec.swap(vec_temp);
	scene_vec.clear();
	//delete m_curScene;
	//m_curScene = nullptr;
}


CSystemConfig::CSystemConfig(void)
{
	g_startPos = 0;
	g_redioIniValue = 0;	
	g_bChangeScene = 0;
	g_waitTimeSetValue = 0;
	g_shapeTimeSetValue = 0;	

}


CSystemConfig::~CSystemConfig(void)
{

}


void CSystemConfig::read(CString path)
{
	if (path.Compare("")==0)
	{
		path = ".//param.ppl";
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
		path = ".//param.ppl";
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
		ar<<(long long)sizeof(CSystemConfig);

		ar<<g_startPos;
		ar<<g_redioIniValue;
		ar << g_rigion_num;

		ar<<RIGION_COUNT;
		for (int i=0;i<RIGION_COUNT;i++)
		{
			ar<< g_rigion[i].rigion_num;
			ar<< g_rigion[i].scene_vec.capacity();
			std::vector<SCENE*>::iterator scene_vec_iter;
			for(scene_vec_iter = g_rigion[i].scene_vec.begin();scene_vec_iter< g_rigion[i].scene_vec.end();scene_vec_iter++)
			{
				SCENE* pSouchData = *scene_vec_iter;
				for (int j=0;j<CHANNEL_COUNT;j++)
				{
					ar << pSouchData->channelValue[j];	//通道值
					ar << pSouchData->voiceCheck[j];	//声光联动
				}
				ar<<pSouchData->waitTime;	//等待时间
				ar<<pSouchData->shapeTime;	//渐变时间
				ar<<pSouchData->time;		//时间点			
			}
			for (int k = 0; k < CHANNEL_COUNT;k++) {
				ar << g_rigion[i].RemarkName[k];
			}
			ar << g_rigion[i].control_select;
			ar << g_rigion[i].channel_type;
		}
		ar<<g_bChangeScene;
		ar<<g_waitTimeSetValue;
		ar<<g_shapeTimeSetValue;
		ar<<g_totalChannelValue;
		ar<<g_bTotalEnable;

		for (int r=0;r<RECORD_COUNT;r++)
		{
			ar << g_record_tpye[r];
		}

	}
	else///如果是加载数据
	{	
		long long sizeCount = 0;
		ar>>sizeCount;
		if(sizeCount!=sizeof(CSystemConfig))
		{
			AfxMessageBox("此配置文件为旧软件生成，请删除原配置并重新生成配置文件！");
			DeleteFile(".//param.ppl");
			return;
		}

		int rigionCount=0;
		ar>>g_startPos;
		ar>>g_redioIniValue;
		ar >> g_rigion_num;

		ar >> rigionCount;

		for (int r = 0; r < rigionCount; r++)
		{
			ar >> g_rigion[r].rigion_num;
			std::vector<SCENE*>vec_temp;
			g_rigion[r].scene_vec.swap(vec_temp);
			int vec_count;
			ar >> vec_count;
			for (int i = 0; i < vec_count; i++)
			{
				SCENE* pData = new SCENE;

				for (int j = 0; j < CHANNEL_COUNT; j++)
				{
					ar >> pData->channelValue[j];	//通道值
					ar >> pData->voiceCheck[j];		//声光联动
				}

				ar >> pData->waitTime;	//等待时间
				ar >> pData->shapeTime;	//渐变时间
				ar >> pData->time;		//时间点

				g_rigion[r].scene_vec.push_back(pData);
			}

			for (int k = 0; k < CHANNEL_COUNT; k++) {
				ar >> g_rigion[r].RemarkName[k];
			}
			ar >> g_rigion[r].control_select;
			ar >> g_rigion[r].channel_type;

		}
		ar>>g_bChangeScene;
		ar>>g_waitTimeSetValue;
		ar>>g_shapeTimeSetValue;
		ar>>g_totalChannelValue;
		ar>>g_bTotalEnable;


		for (int r = 0; r < RECORD_COUNT; r++)
		{
			ar >> g_record_tpye[r];
		}
	}
}

