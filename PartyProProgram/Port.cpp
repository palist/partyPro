
// MCUPort.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "Port.h"
#include "SystemConfig.h"
#include "math.h"
#include "DlgPort.h"

//ab 05 01 ba  ����Ϊ�ƹ��Ŀ¼��   AB 05 02 BA 
//ab 05 03 ba   ����Ϊ�ƿ�̨¼��   AB 05 02 BA 
//ab 06 01 ba  xx ���õڼ�����Ŀ¼�� 01-0A  �ܹ�����ֽ�  ����ab 06 01 ba
//ab 08 01 ba  ѧϰ�ƹ��Ŀ��ͣ��   ����AB 06 01 BA +ѧϰ֡����������������
//ab 07 01 ba  xx  ���õڼ�����Ŀ���� 01-0A  �ܹ�����ֽ�   AB 06 01 BA 
//ab 09 01 ba   ������ͣ   AB 06 01 BA 
//ab 0A 01 ba xx xx����ѧϰ�ڼ�����Ŀ��������   ����ѧϰ�ڼ�֡   ѧϰ�ɹ�ab 06 01 ba 
//ab 0B 01  ba xx xx  ���ò��Ž�Ŀ��ַ   ���ò���֡   AB 06 01 BA 
//ab 09 01 ba   ������ͣ   AB 06 01 BA 
//ab 0c 01 ba  xx ���õڼ��������������� AB 06 01 BA 
//�ȴ�2���ֽ�  1�������� 2��Ŀ���ͣ�����˫����3�����ٶ�  ��512���ֽ�
//AB 06 01 BA
//ab 0E 01 ba xx xx xx���õ����ڼ�����Ŀ   ���õ����ڼ�֡  �� ��λ��0Ϊ��ȡ��Ŀ����   ѧϰ�ɹ�ab 06 01 ba �����ab 0E 01 BA XX XX 00Ϊ������Ŀ������ 
//ab 0F 01 ba xx xx xx���õ���ڼ�����Ŀ   ���õ���ڼ�֡   �� ��λ��0Ϊ�����Ŀ����   ѧϰ�ɹ�ab 06 01 ba   �����ab 0F 01 BA XX XX 00Ϊ�����Ŀ������ 
//ab 1E 01 ba xx xx���õ����ڼ�����Ŀ����������Ŀ   ���õ����ڼ�֡   �ɹ�ab 06 01 ba
//ab 1F 01 ba xx xx���õ���ڼ�����Ŀ����������Ŀ   ���õ����ڼ�֡   �ɹ�ab 06 01 ba        ���յ�ȷ����� ֱ�ӷ�512������

const CString CMD_CONNECT_FLAG = "AB 05 01 BA ";
const CString CMD_CONNECT_COMPLETE_FLAG = "AB 05 02 BA ";
const CString CMD_REBORT = "EF 05 01 FE ";



// CPort

UINT threadReadData( LPVOID p );


CPort::CPort()
{
	m_isOpen = false;
	m_hCom = nullptr; //ȫ�ֱ��������ھ�� 
	m_com_port = _T("");
	m_region =0;	//��
	m_bExit = false;
	m_bIsReturnComplete = false;
	m_uExcFlag = 0;
}



CPort::~CPort()
{
	//destroyMe();
}


// CPort ��Ϣ�������
//������ת�ַ���
CString Dec2Hex(unsigned int intDec)
{
	CString strHex;
	char charHex[255];
	sprintf(charHex,"%02X",intDec&0xFF);
	strHex=charHex;
	if(strHex.GetLength()==1)
		strHex=_T("0")+strHex;
	return strHex;
}

unsigned int Char2Dec(char ch)
{
	if((ch>='0')&&(ch<='9'))
		return ch-'0';
	else if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	else if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
	else
		return 0x10;
}
//�ַ���תʮ������
unsigned int Hex2Dec(CString str)
{
	unsigned int intValue;
	char ch1,ch2;
	str = str.Right(2);
	if (str.GetLength()==2)
	{
		ch1 = (char)str.GetAt(0);
		ch2 = (char)str.GetAt(1);
	}
	else if (str.GetLength()==1)
	{
		ch1 = '0';
		ch2 = (char)str.GetAt(0);
	}

	intValue = Char2Dec(ch1)*16+Char2Dec(ch2);
	return intValue;
}


void HexM2OleVariant(CString strHexM,char *sendCh)
{
	int count  = strHexM.GetLength()/3;
	BYTE *bt = new BYTE[count];

	short int length=0;
	short int intDec;
	int ind = 0;
	while(strHexM.GetLength()>0)
	{        
		ind = strHexM.Find(' ');
		if (ind>0)
		{
			intDec=Hex2Dec(strHexM.Mid(0,ind));
			bt[length++]=char(intDec);
			strHexM.Delete(0,ind+1);
		}
		else{
			intDec=Hex2Dec(strHexM);
			bt[length++]=char(intDec);
			strHexM = "";
		}	
	}
	for(int i=0;i<length;i++)
		sendCh[i]=bt[i];
	delete bt;
	bt = nullptr;
}





void diliveCode(CString Str,CStringArray &ar,CString divChar)
{
	//FA 2A 00 03 F0 02 03 1C F5
	//int len=Str.GetLength();
	int ind = 0;
	int divCount = divChar.GetLength();
	while(Str.GetLength()>0)
	{   
		ind = Str.Find(divChar);
		if (ind>=0)
		{
			ar.Add(Str.Mid(0,ind+divCount-1));
			Str.Delete(0,ind+divCount);
		}
		else{
			ar.Add(Str);
			Str  = _T("") ;			
		}	
	}
}




bool CPort::ConnectPort()
{  
	if (m_com_port.Compare(_T(""))==0)
	{
		AfxMessageBox(_T("���ӵĴ��ں�Ϊ�գ�"));
		return FALSE;
	}
	CString tempCom = m_com_port;
	if (m_com_port.GetLength()>4)
	{
		tempCom.Format("\\\\.\\%s",m_com_port);
	}
	m_hCom=CreateFile(tempCom,//COM1�� ; ע�⴮�ں��������COM9Ӧ����ǰ�����\\.\������COM10��ʾΪ"\\\\.\\COM10"
		GENERIC_READ|GENERIC_WRITE, //�������д  
		0, //��ռ��ʽ  
		NULL,  
		OPEN_EXISTING, //�򿪶����Ǵ���  
		FILE_ATTRIBUTE_NORMAL, //0ͬ����ʽ ; FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED�첽��ʽ
		NULL); 
	if(m_hCom==INVALID_HANDLE_VALUE) {  
		AfxMessageBox(_T("��COMʧ��!"));  
		m_isOpen = false;
		return FALSE;   
	} 
	else
	{
		m_isOpen = true;
	}
	SetupComm(m_hCom,WriteCount,ReadCount); //���뻺����������������Ĵ�С����1024
	COMMTIMEOUTS TimeOuts; 
	TimeOuts.ReadIntervalTimeout=2; //�趨����ʱ 
	TimeOuts.ReadTotalTimeoutMultiplier=2; 
	TimeOuts.ReadTotalTimeoutConstant=2; 
	TimeOuts.WriteTotalTimeoutMultiplier=2; //�趨д��ʱ 
	TimeOuts.WriteTotalTimeoutConstant=2; 
	SetCommTimeouts(m_hCom,&TimeOuts); //���ó�ʱ 
	DCB dcb; 
	GetCommState(m_hCom,&dcb); 
	dcb.BaudRate= 115200; //������Ϊ9600 
	dcb.ByteSize=8; //ÿ���ֽ���8λ 
	dcb.Parity=NOPARITY; //����żУ��λ 
	dcb.StopBits=ONESTOPBIT; //����ֹͣλ 
	SetCommState(m_hCom,&dcb); 
	PurgeComm(m_hCom,PURGE_TXCLEAR | PURGE_RXCLEAR); 
	//AfxBeginThread(threadReadData,this);

	CString retStr;
	int max_times = 3;
	do 
	{
		if (CMD_CONNECT_COMPLETE_FLAG == sendPort(CMD_CONNECT_FLAG))
		{
			m_bIsReturnComplete = true;
			return TRUE;
		}
		else {
			sendPort(CMD_REBORT);
		}
	} while (max_times-->0);
	//if (retStr.Compare(CMD_CONNECT_COMPLETE_FLAG)==0)
	//{
	//	m_bIsReturnComplete = true;
	//	//DCB dcb; 
	//	//GetCommState(m_hCom,&dcb); 
	//	//dcb.BaudRate=115200; //������Ϊ9600 
	//	//dcb.ByteSize=8; //ÿ���ֽ���8λ 
	//	//dcb.Parity=NOPARITY; //����żУ��λ 
	//	//dcb.StopBits=ONESTOPBIT; //����ֹͣλ 
	//	//SetCommState(m_hCom,&dcb); 
	//	//PurgeComm(m_hCom,PURGE_TXCLEAR | PURGE_RXCLEAR); 


	//	//sendPort(CMD_CONNECT_COMPLETE_FLAG);	//�Ƿ�ƿ�̨¼��ģʽ
	//	return TRUE;
	//}
	//else{
	//	m_bIsReturnComplete = false;
	//	return FALSE;
	//}
	AfxMessageBox(_T("Ӳ������Ӧ�������²���豸!"));
	m_bIsReturnComplete = false;
	return FALSE;
}


bool CPort::DisconnectPort()
{
	if(m_hCom)
	{
		CloseHandle(m_hCom);
		m_hCom = nullptr;
		m_isOpen = false;
		m_bIsReturnComplete = false;
		return true;
	}
	return false;
}




CString CPort::sendPort(CString orderStr)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���͵��ַ����ϱ���Ϊʮ�����Ƹ�ʽ
	if(!m_hCom||!m_isOpen)return "";

	int len = orderStr.GetLength();
	char *lpOutBuffer = new char[len+1]; 
	memset(lpOutBuffer,0,len+1);	
	BOOL bWriteStat;
	HexM2OleVariant(orderStr,lpOutBuffer);
	DWORD dwBytesWrite=len/3; 	
	//OVERLAPPED m_osWrite; 
	//memset(&m_osWrite,0,sizeof(OVERLAPPED));
	//m_osWrite.hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
	PurgeComm(m_hCom, PURGE_TXABORT| PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR); 
	bWriteStat=WriteFile(m_hCom,lpOutBuffer,dwBytesWrite,& dwBytesWrite,NULL); 
	if(!bWriteStat) { 
		AfxMessageBox(_T("д����ʧ��!")); 
		return "";
	} 
	SendMessage(CDlgPort::getContext()->m_hWnd,WM_USER_RECE_DATA,false,(LPARAM)&orderStr);
	delete lpOutBuffer;
	lpOutBuffer = nullptr;

	return readPort();
}



BOOL CPort::sendPortOnly(CString orderStr)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���͵��ַ����ϱ���Ϊʮ�����Ƹ�ʽ
	if(!m_hCom||!m_isOpen)return -1;

	int len = orderStr.GetLength();
	char *lpOutBuffer = new char[len+1]; 
	memset(lpOutBuffer,0,len+1);	
	BOOL bWriteStat;
	HexM2OleVariant(orderStr,lpOutBuffer);
	DWORD dwBytesWrite=len/3; 	
	PurgeComm(m_hCom, PURGE_TXABORT| PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR); 
	bWriteStat=WriteFile(m_hCom,lpOutBuffer,dwBytesWrite,& dwBytesWrite,NULL); 
	if(!bWriteStat) { 
		AfxMessageBox(_T("д����ʧ��!")); 
		return -1;
	} 

	SendMessage(CDlgPort::getContext()->m_hWnd,WM_USER_RECE_DATA,false,(LPARAM)&orderStr);
	delete lpOutBuffer;
	lpOutBuffer = nullptr;
	return 0;
}

CString CPort::readPort()
{
	if(!m_hCom)return "";
	char str[ReadCount]; 
	memset(str,0,ReadCount);
	DWORD wCount;//��ȡ���ֽ��� 
	BOOL bReadStat; 
	DWORD dwErrorFlags; 
	COMSTAT ComStat; 
	ClearCommError(m_hCom,&dwErrorFlags,&ComStat); 
	bReadStat=ReadFile(m_hCom,str,ReadCount,&wCount,NULL); 
	if(!bReadStat) { 
		AfxMessageBox(_T("������ʧ��!")); 
	} 
	str[wCount] = '\0';

	CString strtemp="";
	for(int k=0;k<strlen(str);k++)
	{
		strtemp.AppendFormat("%s ",Dec2Hex((UINT)str[k]));
	}

	//���͵����洰��
	SendMessage(CDlgPort::getContext()->m_hWnd,WM_USER_RECE_DATA,TRUE,(LPARAM)&strtemp);
	return strtemp;
}



short int CPort::checkCode(CString checkStr)
{
	short int value=0;
	short int intDec;
	int ind = 0;
	while(checkStr.GetLength()>0)
	{        
		ind = checkStr.Find(' ');
		if (ind>0)
		{
			intDec=Hex2Dec(checkStr.Mid(0,ind));
			checkStr.Delete(0,ind+1);
		}
		else{
			intDec=Hex2Dec(checkStr);
			checkStr = "";
		}	
		value+=intDec;
	}

	return value&0xff;
}





///�����Ǿ�̬��Ա�����治�ܵ������Ա����
UINT threadReadData( LPVOID p )
{
	CPort *pPort = (CPort *)p;
	int iDataNum=0,iDataIndex=0;
	while(!pPort->m_bExit)
	{
		CString RecvStr = pPort->readPort();///�������ݻ�����

		int ret = RecvStr.GetLength();
		////ֻ��¼��ҳ��Ž�������
		///����5A 5A 5A 5A 5A��
		//���ڱ�ʶ
		if(ret>0 && pPort->m_uExcFlag ==EXC_CONNECT_SERIAL)
		{
			if(RecvStr.Compare(ORDER_CONNECT_SUCEED) == 0){
					pPort->m_isOpen = TRUE;///û�г���ʹ򿪴��ڣ���ʶΪ��
					pPort->m_uExcFlag =EXC_NULL;
			}
			continue;
		}
		else if(ret>0 && pPort->m_uExcFlag ==EXC_RETURN_CANCLE)
		{
			if(pPort->m_bIsReturnComplete==false){
				//for(int i=0;i<(int)ret;i++,iDataIndex++){
				//	if(iDataIndex>520)break;
				//	g_DlgDmx->m_pMM_Resule[iDataIndex] =chRecv1[i];
				//}
				//iDataIndex=0;
				if(RecvStr.Compare(ORDER_CONNECT_SUCEED) == 0){
						pPort->m_uExcFlag =EXC_NULL;
						pPort->m_bIsReturnComplete =TRUE;
						//���Ĳ�����Ϊ112500
						//serial_port_.set_option(boost::asio::serial_port::baud_rate(115200));

				}
				continue;
			}
		}
		else if(ret>0 && pPort->m_uExcFlag ==EXC_DEBLOCKING_IN)
		{
			if(pPort->m_bIsReturnComplete==false&&RecvStr.Left(4).Compare(_T("0xAB")) == 0){
				iDataIndex=0;
				//if(g_DlgDmx->m_pMM_Resule[0] ==g_DlgDmx->m_pMM[0] &&
				//	g_DlgDmx->m_pMM_Resule[1] ==g_DlgDmx->m_pMM[1] &&
				//	g_DlgDmx->m_pMM_Resule[2] ==g_DlgDmx->m_pMM[2] &&
				//	g_DlgDmx->m_pMM_Resule[3] ==g_DlgDmx->m_pMM[3] &&
				//	g_DlgDmx->m_pMM_Resule[4] ==g_DlgDmx->m_pMM[4] )
				//{
				//	g_bIsDeblocking =TRUE;
				//	pPort->m_uExcFlag =EXC_NULL;
				//	g_DlgDmx->GetDlgItem(IDBTN_DEBLOCKING)->SetWindowText(StringArray[93]);
				//}

				continue;
			}
		}
		else if(ret>0 && _ttoi(RecvStr.Left(4)) == MSG_DEBLOCKING && pPort->m_uExcFlag ==EXC_DEBLOCKING)
		{	
			pPort->m_uExcFlag =EXC_DEBLOCKING_IN;	
			continue;
		}
		else if(ret>0 && _ttoi(RecvStr.Left(4)) ==MSG_READY_RECORD &&pPort->m_uExcFlag ==EXC_RECORD_MODE)
		{	
			//g_DlgDmx->m_dlgRecord.m_bIsFinishOneData=TRUE;///��ɵ�һ�����ݷ��ͱ�ʶΪ��
			//g_DlgDmx->m_dlgRecord.m_iIsFirstSend =FALSE;////�Ƿ��һ�η��ͱ�ʶ��Ϊ�٣�֮ǰΪ���ˣ�
			//CString strText;
			//strText.Format(_T("%d--  %d,%d,%d,%d,%d\r\n"),g_iRecordCount++,chRecv1,chRecv1,chRecv1,chRecv1,chRecv1);
			////TRACE(strText);
			//g_strRecordSetup +=strText;
			////m_uExcFlag =EXC_NULL;

			////iReocrdSuccess=0;
			continue;
		}
		//�������ݷ�����ϵķ�������5D 5D 5D 5D 5D
		else if(ret>0 && _ttoi(RecvStr.Left(4)) ==MSG_RECORD_ALL_SUCCESS && pPort->m_uExcFlag ==EXC_RECORD_MODE_ALL)
		{
			//// iReocrdSuccess++;
			////if(iReocrdSuccess==5)
			////{
			//g_DlgDmx->m_dlgRecord.RecordAllSuccess();
			////iReocrdSuccess=0;
			//CString strText;
			//strText.Format(_T("%d--  %d,%d,%d,%d,%d\r\n"),g_iRecordCount++,chRecv1,chRecv1,chRecv1,chRecv1,chRecv1);
			//g_strRecordSetup +=strText;
			////	m_uExcFlag =EXC_NULL;
			////}
			continue;
		}
		else if(ret>0 &&pPort->m_uExcFlag ==EXC_RETURN_RESULT_4)
		{
			if(pPort->m_bIsReturnComplete ==false){
				iDataIndex=0;
				pPort->m_bIsReturnComplete =TRUE;
				pPort->m_uExcFlag =EXC_NULL;
				continue;
			}	
		}
		else if(ret>0 && pPort->m_uExcFlag ==EXC_RETURN_RESULT_2)
		{		
			if(pPort->m_bIsReturnComplete ==false){
				iDataIndex=0;
				pPort->m_bIsReturnComplete =TRUE;
				pPort->m_uExcFlag =EXC_NULL;	
				continue;
			}
		}
		else if(ret>0 && pPort->m_uExcFlag ==EXC_RECIVE_DATA)
		{
			if(pPort->m_bIsReturnComplete ==false){
				iDataNum++;
				//for(int i=0;i<(int)ret;i++,iDataIndex++){
				//	if(iDataIndex>520)break;
				//	g_DlgDmx->m_pMM_Resule[iDataIndex] =chRecv1[i];
				//}
				if(iDataNum==18){
					pPort->m_bIsReturnComplete =TRUE;
					pPort->m_uExcFlag =EXC_NULL;
					iDataNum = 0;
					iDataIndex = 0;
					continue;
				}
			}
		}
		else if(ret>0 &&pPort->m_uExcFlag ==EXC_RETURN_RESULT_10)
		{
			if(pPort->m_bIsReturnComplete ==false){
				pPort->m_bIsReturnComplete =TRUE;
				pPort->m_uExcFlag =EXC_NULL;
				continue;
			}
		}

		Sleep(50);
	}
	return 0;
}
