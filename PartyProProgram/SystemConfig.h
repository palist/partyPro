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
	int channelValue[CHANNEL_COUNT];	//通道值
	int voiceCheck[CHANNEL_COUNT];		//声光联动

	double waitTime;	//等待时间
	double shapeTime;	//渐变时间
	CTime time;		//时间点
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
	CString RemarkName[CHANNEL_COUNT];		//别名
	bool control_select;		//声控选择
	BOOL channel_type;			//通道融合类型（单步，双步，四步，八步，16步，32步）
};


//灯单元
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
	int g_startPos;		//滚动条现在所处的位置
	CRigion g_rigion[RIGION_COUNT];
	int g_redioIniValue;		//REDIO方式选择起始通道
	int g_rigion_num;			//当前第几个域
	BOOL g_bChangeScene;	//推杆是否改变场景值
	double g_waitTimeSetValue;	//等待时间
	double g_shapeTimeSetValue;		//渐变时间
	int g_totalChannelValue;		//总使能值 
	BOOL g_bTotalEnable;			//总使能开关
	CString g_record_tpye[RECORD_COUNT];	//节目类型

	std::vector<LIGHT_UNIT> g_light_vec;	//组合操作等列表

public:
	void read(CString path= ".//param.ppl");
	void save(CString path = ".//param.ppl");
	void Serialize(CArchive& ar);
};

extern CSystemConfig g_Config; 