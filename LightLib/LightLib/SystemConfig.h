#pragma once
#include <vector>


#define LIGHT_MAX_COUNT 100



//µÆµ¥Ôª
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
	int white_ch;
	int x_ch;
	int y_ch;
	int x_ch_l;
	int y_ch_l;
	int region_index;
	int start_channel;
};



class CSystemConfig
{
public:
	CSystemConfig(void);
	~CSystemConfig(void);

public:
	LIGHT_UNIT g_light_unit;
public:
	void read(CString path= ".//param.light");
	void save(CString path = ".//param.light");
	void Serialize(CArchive& ar);
};

extern CSystemConfig g_Config; 