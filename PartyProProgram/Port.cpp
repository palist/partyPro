
// MCUPort.cpp : 实现文件
//
#include "stdafx.h"
#include "Port.h"
#include "SystemConfig.h"
#include "math.h"
#include "DlgPort.h"

//ab 05 01 ba  设置为灯光节目录入   AB 05 02 BA 
//ab 05 03 ba   设置为灯控台录入   AB 05 02 BA 
//ab 06 01 ba  xx 设置第几个节目录入 01-0A  总共五个字节  返回ab 06 01 ba
//ab 08 01 ba  学习灯光节目暂停键   返回AB 06 01 BA +学习帧数量（场景数量）
//ab 07 01 ba  xx  设置第几个节目播放 01-0A  总共五个字节   AB 06 01 BA 
//ab 09 01 ba   播放暂停   AB 06 01 BA 
//ab 0A 01 ba xx xx设置学习第几个节目声光联动   设置学习第几帧   学习成功ab 06 01 ba 
//ab 0B 01  ba xx xx  设置播放节目地址   设置播放帧   AB 06 01 BA 
//ab 09 01 ba   播放暂停   AB 06 01 BA 
//ab 0c 01 ba  xx 配置第几个声光联动配置 AB 06 01 BA 
//先传2个字节  1场景数量 2节目类型（单步双步）3场景速度  传512个字节
//AB 06 01 BA
//ab 0E 01 ba xx xx xx设置导出第几个节目   设置导出第几帧  高 低位（0为读取节目数）   学习成功ab 06 01 ba 如果是ab 0E 01 BA XX XX 00为导出节目数命令 
//ab 0F 01 ba xx xx xx设置导入第几个节目   设置导入第几帧   高 低位（0为导入节目数）   学习成功ab 06 01 ba   如果是ab 0F 01 BA XX XX 00为导入节目数命令 
//ab 1E 01 ba xx xx设置导出第几个节目声光联动节目   设置导出第几帧   成功ab 06 01 ba
//ab 1F 01 ba xx xx设置导入第几个节目声光联动节目   设置导出第几帧   成功ab 06 01 ba        接收到确认码后 直接发512个数据

const CString CMD_CONNECT_FLAG = "AB 05 01 BA ";
const CString CMD_CONNECT_COMPLETE_FLAG = "AB 05 02 BA ";
const CString CMD_REBORT = "EF 05 01 FE ";



// CPort

UINT threadReadData( LPVOID p );


CPort::CPort()
{
	m_isOpen = false;
	m_hCom = nullptr; //全局变量，串口句柄 
	m_com_port = _T("");
	m_region =0;	//域
	m_bExit = false;
	m_bIsReturnComplete = false;
	m_uExcFlag = 0;
}



CPort::~CPort()
{
	//destroyMe();
}


// CPort 消息处理程序
//二进制转字符串
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
//字符串转十六进制
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
		AfxMessageBox(_T("连接的串口号为空！"));
		return FALSE;
	}
	CString tempCom = m_com_port;
	if (m_com_port.GetLength()>4)
	{
		tempCom.Format("\\\\.\\%s",m_com_port);
	}
	m_hCom=CreateFile(tempCom,//COM1口 ; 注意串口号如果大于COM9应该在前面加上\\.\，比如COM10表示为"\\\\.\\COM10"
		GENERIC_READ|GENERIC_WRITE, //允许读和写  
		0, //独占方式  
		NULL,  
		OPEN_EXISTING, //打开而不是创建  
		FILE_ATTRIBUTE_NORMAL, //0同步方式 ; FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED异步方式
		NULL); 
	if(m_hCom==INVALID_HANDLE_VALUE) {  
		AfxMessageBox(_T("打开COM失败!"));  
		m_isOpen = false;
		return FALSE;   
	} 
	else
	{
		m_isOpen = true;
	}
	SetupComm(m_hCom,WriteCount,ReadCount); //输入缓冲区和输出缓冲区的大小都是1024
	COMMTIMEOUTS TimeOuts; 
	TimeOuts.ReadIntervalTimeout=2; //设定读超时 
	TimeOuts.ReadTotalTimeoutMultiplier=2; 
	TimeOuts.ReadTotalTimeoutConstant=2; 
	TimeOuts.WriteTotalTimeoutMultiplier=2; //设定写超时 
	TimeOuts.WriteTotalTimeoutConstant=2; 
	SetCommTimeouts(m_hCom,&TimeOuts); //设置超时 
	DCB dcb; 
	GetCommState(m_hCom,&dcb); 
	dcb.BaudRate= 115200; //波特率为9600 
	dcb.ByteSize=8; //每个字节有8位 
	dcb.Parity=NOPARITY; //无奇偶校验位 
	dcb.StopBits=ONESTOPBIT; //两个停止位 
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
	//	//dcb.BaudRate=115200; //波特率为9600 
	//	//dcb.ByteSize=8; //每个字节有8位 
	//	//dcb.Parity=NOPARITY; //无奇偶校验位 
	//	//dcb.StopBits=ONESTOPBIT; //两个停止位 
	//	//SetCommState(m_hCom,&dcb); 
	//	//PurgeComm(m_hCom,PURGE_TXCLEAR | PURGE_RXCLEAR); 


	//	//sendPort(CMD_CONNECT_COMPLETE_FLAG);	//是否灯控台录入模式
	//	return TRUE;
	//}
	//else{
	//	m_bIsReturnComplete = false;
	//	return FALSE;
	//}
	AfxMessageBox(_T("硬件无响应，请重新插拔设备!"));
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
	// TODO: 在此添加控件通知处理程序代码
	//发送的字符串上表面为十六进制格式
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
		AfxMessageBox(_T("写串口失败!")); 
		return "";
	} 
	SendMessage(CDlgPort::getContext()->m_hWnd,WM_USER_RECE_DATA,false,(LPARAM)&orderStr);
	delete lpOutBuffer;
	lpOutBuffer = nullptr;

	return readPort();
}



BOOL CPort::sendPortOnly(CString orderStr)
{
	// TODO: 在此添加控件通知处理程序代码
	//发送的字符串上表面为十六进制格式
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
		AfxMessageBox(_T("写串口失败!")); 
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
	DWORD wCount;//读取的字节数 
	BOOL bReadStat; 
	DWORD dwErrorFlags; 
	COMSTAT ComStat; 
	ClearCommError(m_hCom,&dwErrorFlags,&ComStat); 
	bReadStat=ReadFile(m_hCom,str,ReadCount,&wCount,NULL); 
	if(!bReadStat) { 
		AfxMessageBox(_T("读串口失败!")); 
	} 
	str[wCount] = '\0';

	CString strtemp="";
	for(int k=0;k<strlen(str);k++)
	{
		strtemp.AppendFormat("%s ",Dec2Hex((UINT)str[k]));
	}

	//发送到界面窗口
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





///由于是静态成员，里面不能调用类成员变量
UINT threadReadData( LPVOID p )
{
	CPort *pPort = (CPort *)p;
	int iDataNum=0,iDataIndex=0;
	while(!pPort->m_bExit)
	{
		CString RecvStr = pPort->readPort();///接收数据缓冲区

		int ret = RecvStr.GetLength();
		////只有录制页面才接收数据
		///接收5A 5A 5A 5A 5A就
		//串口标识
		if(ret>0 && pPort->m_uExcFlag ==EXC_CONNECT_SERIAL)
		{
			if(RecvStr.Compare(ORDER_CONNECT_SUCEED) == 0){
					pPort->m_isOpen = TRUE;///没有出错就打开串口，标识为真
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
						//更改波特率为112500
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
			//g_DlgDmx->m_dlgRecord.m_bIsFinishOneData=TRUE;///完成第一次数据发送标识为真
			//g_DlgDmx->m_dlgRecord.m_iIsFirstSend =FALSE;////是否第一次发送标识变为假（之前为真了）
			//CString strText;
			//strText.Format(_T("%d--  %d,%d,%d,%d,%d\r\n"),g_iRecordCount++,chRecv1,chRecv1,chRecv1,chRecv1,chRecv1);
			////TRACE(strText);
			//g_strRecordSetup +=strText;
			////m_uExcFlag =EXC_NULL;

			////iReocrdSuccess=0;
			continue;
		}
		//所有数据发送完毕的返回命令5D 5D 5D 5D 5D
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
