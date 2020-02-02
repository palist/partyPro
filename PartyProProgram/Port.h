#pragma once
#include "afxwin.h"

#define WriteCount 2048
#define ReadCount 512


#define MSG_TOP_COUNT  5   ///��Ϣͷ���ַ�����
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

#define ORDER_CONNECT_SUCEED _T("0xAB 0x05 0x02 0xBA")


#define WM_USER_RECE_DATA		WM_USER + 101

////������ʾ������
typedef enum{
	CONNECT_COM =1000,
	DISCONNECT_COM,
	UPDATE_F3,
	UPDATE_F8
};


class CPort 
{

public:
	CPort();   // ��׼���캯��
	virtual ~CPort();

public:
	HANDLE m_hCom; //ȫ�ֱ��������ھ�� 
	bool m_isOpen;
	CString m_com_port;
	int m_region;	//��
	bool m_bExit;	//�˳��߳�
	int m_uExcFlag;///ִ�б�ʶ
	bool m_bIsReturnComplete;

public:	
	CString sendPort(CString orderStr);
	BOOL  sendPortOnly(CString orderStr);
	CString readPort();
	short int checkCode(CString checkStr);
	bool ConnectPort();
	bool DisconnectPort();	

};
