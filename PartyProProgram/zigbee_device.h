#ifndef ZIGBEE_MANAGER_HPP
#define ZIGBEE_MANAGER_HPP
#include "Task.h"
//#include "PartyProProgramDlg.h"


#define MSG_TOP_COUNT  6   ///消息头的字符数量
#define MSG_TOP_STR_VALUE 90 ///消息头的每个字节的值（调试消息头）
#define MSG_TOP_SPLICE_VALUE 90//消息头的每个字节的值（拼接消息头）
#define MSG_DEBLOCKING 90///解锁头
#define MSG_READY_RECORD 91////返回的开始录制ID
#define MSG_TOP_RECORD_VALUE 92/////消息头的每个字节的值（记录消息头）
#define MSG_RECORD_ALL_SUCCESS 93////返回全部录制成功ID
#define MSG_RECORD_FAILED  94//// 停止发送ID
#define SLEEP_TIME   40///发送停顿时间

#define EXC_NULL  0
#define EXC_RECORD_MODE 100
#define EXC_RECORD_FINISH_ONE 101
#define EXC_DEBLOCKING 102
#define EXC_DEBLOCKING_IN 103
#define EXC_RECORD_MODE_ALL 104
#define EXC_CONNECT_SERIAL 201
#define EXC_RETURN_RESULT_4 202
#define EXC_RETURN_RESULT_2 203
#define EXC_RETURN_CANCLE 204
#define EXC_RECIVE_DATA 205
#define EXC_RETURN_RESULT_10 206
#define EXC_RETURN_RESULT_9 207

////更新提示框命令
typedef enum{
	CONNECT_COM =1000,
	DISCONNECT_COM,
	UPDATE_F3,
	UPDATE_F8
};


class zigbee_device:public CTask
{
public:
	zigbee_device();
	virtual ~zigbee_device();
	BOOL ConnetSerial(const char* com_name, unsigned int rate);///连接串口
	BOOL ThreadReciveData();
	///接收数据
    void ReadData();///读取串口                   
	///发送不同类型的数据
	void WriteDataPPrt(UINT8 (*temp)[16],size_t nSize);////发送串口数据
	void WriteDataPrt(const char *temp);///发送串口数据
	void WriteDataUnPrt(unsigned  char *temp,size_t nSize);///发送串口数据
	void WriteDataInt(const int temp);///发送串口数据
	void WriteData(const char temp);///发送串口数据
    ///关闭串口
	void CloseSerial();///关闭串口
	BOOL IsSerialOpen();///判断串口是否打开

	boost::asio::serial_port& GetPort()///得到串口通道变量
	{
		return serial_port_;
	}
public:///成员变量
	boost::asio::serial_port serial_port_;///串口通道变量
    UINT m_uFlag;///标识接收到的命令
	bool m_bSerialOpen;//串口的开关标识
	bool m_bConnectSussess;
	CString m_com_port;
	int m_region;	//域
};

//extern zigbee_device g_zigbee_device;///全局变量
#endif // ZIGBEE_MANAGER_HPP