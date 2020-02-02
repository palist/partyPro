#ifndef ZIGBEE_MANAGER_HPP
#define ZIGBEE_MANAGER_HPP
#include "Task.h"
//#include "PartyProProgramDlg.h"


#define MSG_TOP_COUNT  6   ///��Ϣͷ���ַ�����
#define MSG_TOP_STR_VALUE 90 ///��Ϣͷ��ÿ���ֽڵ�ֵ��������Ϣͷ��
#define MSG_TOP_SPLICE_VALUE 90//��Ϣͷ��ÿ���ֽڵ�ֵ��ƴ����Ϣͷ��
#define MSG_DEBLOCKING 90///����ͷ
#define MSG_READY_RECORD 91////���صĿ�ʼ¼��ID
#define MSG_TOP_RECORD_VALUE 92/////��Ϣͷ��ÿ���ֽڵ�ֵ����¼��Ϣͷ��
#define MSG_RECORD_ALL_SUCCESS 93////����ȫ��¼�Ƴɹ�ID
#define MSG_RECORD_FAILED  94//// ֹͣ����ID
#define SLEEP_TIME   40///����ͣ��ʱ��

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

////������ʾ������
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
	BOOL ConnetSerial(const char* com_name, unsigned int rate);///���Ӵ���
	BOOL ThreadReciveData();
	///��������
    void ReadData();///��ȡ����                   
	///���Ͳ�ͬ���͵�����
	void WriteDataPPrt(UINT8 (*temp)[16],size_t nSize);////���ʹ�������
	void WriteDataPrt(const char *temp);///���ʹ�������
	void WriteDataUnPrt(unsigned  char *temp,size_t nSize);///���ʹ�������
	void WriteDataInt(const int temp);///���ʹ�������
	void WriteData(const char temp);///���ʹ�������
    ///�رմ���
	void CloseSerial();///�رմ���
	BOOL IsSerialOpen();///�жϴ����Ƿ��

	boost::asio::serial_port& GetPort()///�õ�����ͨ������
	{
		return serial_port_;
	}
public:///��Ա����
	boost::asio::serial_port serial_port_;///����ͨ������
    UINT m_uFlag;///��ʶ���յ�������
	bool m_bSerialOpen;//���ڵĿ��ر�ʶ
	bool m_bConnectSussess;
	CString m_com_port;
	int m_region;	//��
};

//extern zigbee_device g_zigbee_device;///ȫ�ֱ���
#endif // ZIGBEE_MANAGER_HPP