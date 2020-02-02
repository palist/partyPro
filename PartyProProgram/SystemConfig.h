#pragma once
#include <vector>

#define RIGION_COUNT 20
#define CHANNEL_COUNT 512
#define SHOW_COUNT 32
#define RECORD_COUNT 32
#define LIGHT_MAX_COUNT 100


#define WM_FLUSH_UI WM_USER+1
#define WM_FLUSH_SCROLL  WM_USER + 102






struct SCENE{
	int channelValue[CHANNEL_COUNT];	//ͨ��ֵ
	int voiceCheck[CHANNEL_COUNT];		//��������

	double waitTime;	//�ȴ�ʱ��
	double shapeTime;	//����ʱ��
	CTime time;		//ʱ���
	void copy(SCENE *temp){

		for (int j=0;j<CHANNEL_COUNT;j++)
		{
			channelValue[j] = temp->channelValue[j];
			voiceCheck[j] = temp->voiceCheck[j];
		}

		waitTime = temp->waitTime;
		shapeTime = temp->shapeTime;
		time = temp->time;
	}
};

class CRigion
{
public:
	CRigion(void);
	~CRigion(void);

public:
	int rigion_num;
	std::vector<SCENE*>scene_vec;
	//SCENE* m_curScene;
	CString RemarkName[CHANNEL_COUNT];		//����
	bool control_select;		//����ѡ��
	BOOL channel_type;			//ͨ���ں����ͣ�������˫�����Ĳ����˲���16����32����
};


//�Ƶ�Ԫ
struct LIGHT_UNIT
{
	int light_type;
	CString light_name;
	int channel_count;
	CString channel_name[LIGHT_MAX_COUNT];
	CString light_factory;
	int red_ch;
	int green_ch;
	int blue_ch;
	int x_ch;
	int y_ch;
	int x_ch_l;
	int y_ch_l;
	int region_index;
	int start_channel;
	bool bGroup;
};

class CSystemConfig
{
public:
	CSystemConfig(void);
	~CSystemConfig(void);

public:
	int g_startPos;		//����������������λ��
	CRigion g_rigion[RIGION_COUNT];
	int g_redioIniValue;		//REDIO��ʽѡ����ʼͨ��
	int g_rigion_num;			//��ǰ�ڼ�����
	BOOL g_bChangeScene;	//�Ƹ��Ƿ�ı䳡��ֵ
	double g_waitTimeSetValue;	//�ȴ�ʱ��
	double g_shapeTimeSetValue;		//����ʱ��
	int g_totalChannelValue;		//��ʹ��ֵ 
	BOOL g_bTotalEnable;			//��ʹ�ܿ���
	CString g_record_tpye[RECORD_COUNT];	//��Ŀ����

	std::vector<LIGHT_UNIT> g_light_vec;	//��ϲ������б�

public:
	void read(CString path= ".//param.ppl");
	void save(CString path = ".//param.ppl");
	void Serialize(CArchive& ar);
};

extern CSystemConfig g_Config; 