#include "stdafx.h"
#include "PartyProProgram.h"
#include "zigbee_device.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "PartyProProgramDlg.h"

class CDMX512_TWOApp;
using namespace boost;

zigbee_device g_zigbee_device;
zigbee_device::zigbee_device():serial_port_(io_service_)
{
	m_uFlag=0;
	m_bSerialOpen = FALSE;//串口初始为关状态
	m_bConnectSussess =FALSE;
}
BOOL zigbee_device::ThreadReciveData()
{
	boost::thread thd(boost::bind(&zigbee_device::ReadData,this));
	return TRUE;
}
BOOL zigbee_device::ConnetSerial(const char* com_name, unsigned int rate)
{
	try{	
            serial_port_.open(com_name);	   
	         serial_port_.set_option(boost::asio::serial_port::baud_rate(rate));
	         serial_port_.set_option(boost::asio::serial_port::flow_control());
	         serial_port_.set_option(boost::asio::serial_port::parity());
	         serial_port_.set_option(boost::asio::serial_port::stop_bits());
	         serial_port_.set_option(boost::asio::serial_port::character_size(8));
	}
	catch (const std::exception& e)
	{
		m_bSerialOpen = FALSE;///出错就返回假，打开串口标识为假
		return FALSE;
	}
	unsigned char *headData =new unsigned char[4];
	headData[0] =0xAB;
	headData[1] =0x05;
	headData[2] =0x01;
	headData[3] =0xBA;
	WriteDataUnPrt(headData,4);
	delete headData;
	headData =NULL;
	m_bSerialOpen = TRUE;///没有出错就打开串口，标识为真
///使用boost库线程类 创建一个线程， 用于接收串口收到的数据
	boost::thread thd(boost::bind(&zigbee_device::ReadData,this));
	return TRUE;
}


zigbee_device::~zigbee_device()
{
}


///由于是静态成员，里面不能调用类成员变量
void zigbee_device::ReadData()
{
	int /*iReocrdSuccess =0,*/iDataNum=0,iDataIndex=0;
     boost::system::error_code err;
	while(1)
	{
		unsigned char chRecv1[32]={0};///接收数据缓冲区
		//memset(g_DlgDmx->m_pMM_Resule,0,520);
		size_t ret =serial_port_.read_some(boost::asio::buffer(&chRecv1, 32), err); ///接收数据
		CString Cst;
		TRACE("ReadData=%d\n",ret);
		for(int i=0;i<(int)ret;i++){
			Cst.AppendFormat(_T("%X "),chRecv1[i]);	
		}
		//TRACE(Cst);
		TRACE("\n");

		//TRACE("g_uExcFlag =%d,iRecord =%d,chRecv1 = %X,chRecv2 = %X,chRecv3 = %X\n",g_uExcFlag,iRecord,chRecv1[0],chRecv1[1],chRecv1[2]);
		///串口关闭时 会接收错误信息
		if (err)  ///串口发生错误处理
		{   
			//AfxMessageBox("串口发生错误!");
			return;
		}  
		////只有录制页面才接收数据
		///接收5A 5A 5A 5A 5A就
		//串口标识
		else if(ret>0 && g_uExcFlag ==EXC_CONNECT_SERIAL)
		{
			if(chRecv1[0] ==0xAB&&
				chRecv1[1] ==0x05&&
				chRecv1[2] ==0x02&&
				chRecv1[3] ==0xBA){
				//成功了一半,更改波特率为112500
				serial_port_.set_option(boost::asio::serial_port::baud_rate(115200));
				m_bSerialOpen = TRUE;///没有出错就打开串口，标识为真
				m_bConnectSussess =TRUE;
				g_uExcFlag =EXC_NULL;
			}
			continue;
		}
		else if(ret>0 && g_uExcFlag ==EXC_RETURN_CANCLE)
		{
			if(g_bIsReturnComplete==false){
				for(int i=0;i<(int)ret;i++,iDataIndex++){
					if(iDataIndex>520)break;
					g_DlgDmx->m_pMM_Resule[iDataIndex] =chRecv1[i];
				}
				iDataIndex=0;
				if(chRecv1[0] ==0xAB&&
					chRecv1[1] ==0x05&&
					chRecv1[2] ==0x02&&
					chRecv1[3] ==0xBA){
					g_uExcFlag =EXC_NULL;
					g_bIsReturnComplete =TRUE;
					//更改波特率为112500
					//serial_port_.set_option(boost::asio::serial_port::baud_rate(115200));
					
				}
				continue;
			}
		}
		else if(ret>0 && g_uExcFlag ==EXC_DEBLOCKING_IN)
		{
			if(g_bIsReturnComplete==false&&chRecv1[0] ==0xAB){
				for(int i=0;i<(int)ret;i++,iDataIndex++){
					if(iDataIndex>520)break;
					g_DlgDmx->m_pMM_Resule[iDataIndex] =chRecv1[i];
				}
				iDataIndex=0;
				if(g_DlgDmx->m_pMM_Resule[0] ==g_DlgDmx->m_pMM[0] &&
						g_DlgDmx->m_pMM_Resule[1] ==g_DlgDmx->m_pMM[1] &&
						g_DlgDmx->m_pMM_Resule[2] ==g_DlgDmx->m_pMM[2] &&
						g_DlgDmx->m_pMM_Resule[3] ==g_DlgDmx->m_pMM[3] &&
						g_DlgDmx->m_pMM_Resule[4] ==g_DlgDmx->m_pMM[4] )
				{
					g_bIsDeblocking =TRUE;
					g_uExcFlag =EXC_NULL;
					g_DlgDmx->GetDlgItem(IDBTN_DEBLOCKING)->SetWindowText(StringArray[93]);
				}

				continue;
			}
		}
		else if(ret>0 && chRecv1[0] ==MSG_DEBLOCKING && g_uExcFlag ==EXC_DEBLOCKING)
		{	
			g_uExcFlag =EXC_DEBLOCKING_IN;	
			continue;
		}
		else if(ret>0 && chRecv1[0] ==MSG_READY_RECORD && g_uExcFlag ==EXC_RECORD_MODE)
		{	
			g_DlgDmx->m_dlgRecord.m_bIsFinishOneData=TRUE;///完成第一次数据发送标识为真
			g_DlgDmx->m_dlgRecord.m_iIsFirstSend =FALSE;////是否第一次发送标识变为假（之前为真了）
			CString strText;
			strText.Format(_T("%d--  %d,%d,%d,%d,%d\r\n"),g_iRecordCount++,chRecv1,chRecv1,chRecv1,chRecv1,chRecv1);
			//TRACE(strText);
			g_strRecordSetup +=strText;
			//g_uExcFlag =EXC_NULL;

			 //iReocrdSuccess=0;
			continue;
		}
		//所有数据发送完毕的返回命令5D 5D 5D 5D 5D
		else if(ret>0 && chRecv1[0] ==MSG_RECORD_ALL_SUCCESS && g_uExcFlag ==EXC_RECORD_MODE_ALL)
		{
			// iReocrdSuccess++;
			//if(iReocrdSuccess==5)
			//{
				g_DlgDmx->m_dlgRecord.RecordAllSuccess();
				//iReocrdSuccess=0;
				CString strText;
				strText.Format(_T("%d--  %d,%d,%d,%d,%d\r\n"),g_iRecordCount++,chRecv1,chRecv1,chRecv1,chRecv1,chRecv1);
				g_strRecordSetup +=strText;
			//	g_uExcFlag =EXC_NULL;
			//}
			continue;
		}
		else if(ret>0 && g_uExcFlag ==EXC_RETURN_RESULT_4)
		{
			if(g_bIsReturnComplete ==false){
				for(int i=0;i<(int)ret;i++,iDataIndex++){
					if(iDataIndex>520)break;
					g_DlgDmx->m_pMM_Resule[iDataIndex] =chRecv1[i];
				}
				iDataIndex=0;
				g_bIsReturnComplete =TRUE;
				g_uExcFlag =EXC_NULL;
				continue;
			}	
		}
		else if(ret>0 && g_uExcFlag ==EXC_RETURN_RESULT_2)
		{		
			if(g_bIsReturnComplete ==false){
				for(int i=0;i<(int)ret;i++,iDataIndex++){
					if(iDataIndex>520)break;
					g_DlgDmx->m_pMM_Resule[iDataIndex] =chRecv1[i];
				}
				iDataIndex=0;
				g_bIsReturnComplete =TRUE;
				g_uExcFlag =EXC_NULL;	
				continue;
			}
		}
		else if(ret>0 && g_uExcFlag ==EXC_RECIVE_DATA)
		{
			if(g_bIsReturnComplete ==false){
				iDataNum++;
				for(int i=0;i<(int)ret;i++,iDataIndex++){
					if(iDataIndex>520)break;
					g_DlgDmx->m_pMM_Resule[iDataIndex] =chRecv1[i];
				}
				if(iDataNum==18){
					//TRACE("EXC_RECIVE_DATA\n");
					//for(int i=0;i<520;i++){
					//	TRACE("%X-" ,g_DlgDmx->m_pMM_Resule[i]);
					//}
					//TRACE("\n");
					g_bIsReturnComplete =TRUE;
					g_uExcFlag =EXC_NULL;
					iDataNum = 0;
					iDataIndex = 0;
					continue;
				}
			}
		}
		else if(ret>0 && g_uExcFlag ==EXC_RETURN_RESULT_10)
		{
			if(g_bIsReturnComplete ==false){
				for(int i=0;i<(int)ret;i++,iDataIndex++){
					if(iDataIndex>520)break;
					g_DlgDmx->m_pMM_Resule[iDataIndex] =chRecv1[i];
				}
				iDataIndex=0;
				g_bIsReturnComplete =TRUE;
				g_uExcFlag =EXC_NULL;
				continue;
			}
		}
	    //iReocrdSuccess=0;
       ///回复等待录制成功处理
	}
}

///多个发送函数，处理不同的数据发送
///1. 发送字符串
void zigbee_device::WriteDataPrt(const char *temp)
{
	boost::asio::write(GetPort(), boost::asio::buffer(temp,strlen(temp)+1));
}
///2. 发送unsigned char 数据 即16进制数据
void zigbee_device::WriteDataUnPrt(unsigned char *temp,size_t nSize)
{
	boost::asio::write(GetPort(), boost::asio::buffer(temp,nSize));
}
///3. 发送 整数型类
void zigbee_device::WriteDataInt(const int temp)
{
     boost::asio::write(GetPort(), boost::asio::buffer(&temp,4));
}
//4. 发送单个字符
void zigbee_device::WriteData(const char temp)
{
	boost::asio::write(GetPort(), boost::asio::buffer(&temp,1));
}
void zigbee_device::WriteDataPPrt(UINT8 temp[][16],size_t nSize)
{
	 boost::asio::write(GetPort(), boost::asio::buffer(temp,nSize));
}
///关闭串口
void zigbee_device::CloseSerial()
{
	   m_bSerialOpen = FALSE;
       serial_port_.close();   
	   CloseService();
}
BOOL zigbee_device::IsSerialOpen()
{
	return m_bSerialOpen;
}
