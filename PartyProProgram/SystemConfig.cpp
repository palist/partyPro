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
		path = ".//param.ppl";
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
					ar << pSouchData->channelValue[j];	//ͨ��ֵ
					ar << pSouchData->voiceCheck[j];	//��������
				}
				ar<<pSouchData->waitTime;	//�ȴ�ʱ��
				ar<<pSouchData->shapeTime;	//����ʱ��
				ar<<pSouchData->time;		//ʱ���			
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
	else///����Ǽ�������
	{	
		long long sizeCount = 0;
		ar>>sizeCount;
		if(sizeCount!=sizeof(CSystemConfig))
		{
			AfxMessageBox("�������ļ�Ϊ��������ɣ���ɾ��ԭ���ò��������������ļ���");
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
					ar >> pData->channelValue[j];	//ͨ��ֵ
					ar >> pData->voiceCheck[j];		//��������
				}

				ar >> pData->waitTime;	//�ȴ�ʱ��
				ar >> pData->shapeTime;	//����ʱ��
				ar >> pData->time;		//ʱ���

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

