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
	m_bSerialOpen = FALSE;//���ڳ�ʼΪ��״̬
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
		m_bSerialOpen = FALSE;///����ͷ��ؼ٣��򿪴��ڱ�ʶΪ��
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
	m_bSerialOpen = TRUE;///û�г���ʹ򿪴��ڣ���ʶΪ��
///ʹ��boost���߳��� ����һ���̣߳� ���ڽ��մ����յ�������
	boost::thread thd(boost::bind(&zigbee_device::ReadData,this));
	return TRUE;
}


zigbee_device::~zigbee_device()
{
}


///�����Ǿ�̬��Ա�����治�ܵ������Ա����
void zigbee_device::ReadData()
{
	int /*iReocrdSuccess =0,*/iDataNum=0,iDataIndex=0;
     boost::system::error_code err;
	while(1)
	{
		unsigned char chRecv1[32]={0};///�������ݻ�����
		//memset(g_DlgDmx->m_pMM_Resule,0,520);
		size_t ret =serial_port_.read_some(boost::asio::buffer(&chRecv1, 32), err); ///��������
		CString Cst;
		TRACE("ReadData=%d\n",ret);
		for(int i=0;i<(int)ret;i++){
			Cst.AppendFormat(_T("%X "),chRecv1[i]);	
		}
		//TRACE(Cst);
		TRACE("\n");

		//TRACE("g_uExcFlag =%d,iRecord =%d,chRecv1 = %X,chRecv2 = %X,chRecv3 = %X\n",g_uExcFlag,iRecord,chRecv1[0],chRecv1[1],chRecv1[2]);
		///���ڹر�ʱ ����մ�����Ϣ
		if (err)  ///���ڷ���������
		{   
			//AfxMessageBox("���ڷ�������!");
			return;
		}  
		////ֻ��¼��ҳ��Ž�������
		///����5A 5A 5A 5A 5A��
		//���ڱ�ʶ
		else if(ret>0 && g_uExcFlag ==EXC_CONNECT_SERIAL)
		{
			if(chRecv1[0] ==0xAB&&
				chRecv1[1] ==0x05&&
				chRecv1[2] ==0x02&&
				chRecv1[3] ==0xBA){
				//�ɹ���һ��,���Ĳ�����Ϊ112500
				serial_port_.set_option(boost::asio::serial_port::baud_rate(115200));
				m_bSerialOpen = TRUE;///û�г���ʹ򿪴��ڣ���ʶΪ��
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
					//���Ĳ�����Ϊ112500
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
			g_DlgDmx->m_dlgRecord.m_bIsFinishOneData=TRUE;///��ɵ�һ�����ݷ��ͱ�ʶΪ��
			g_DlgDmx->m_dlgRecord.m_iIsFirstSend =FALSE;////�Ƿ��һ�η��ͱ�ʶ��Ϊ�٣�֮ǰΪ���ˣ�
			CString strText;
			strText.Format(_T("%d--  %d,%d,%d,%d,%d\r\n"),g_iRecordCount++,chRecv1,chRecv1,chRecv1,chRecv1,chRecv1);
			//TRACE(strText);
			g_strRecordSetup +=strText;
			//g_uExcFlag =EXC_NULL;

			 //iReocrdSuccess=0;
			continue;
		}
		//�������ݷ�����ϵķ�������5D 5D 5D 5D 5D
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
       ///�ظ��ȴ�¼�Ƴɹ�����
	}
}

///������ͺ���������ͬ�����ݷ���
///1. �����ַ���
void zigbee_device::WriteDataPrt(const char *temp)
{
	boost::asio::write(GetPort(), boost::asio::buffer(temp,strlen(temp)+1));
}
///2. ����unsigned char ���� ��16��������
void zigbee_device::WriteDataUnPrt(unsigned char *temp,size_t nSize)
{
	boost::asio::write(GetPort(), boost::asio::buffer(temp,nSize));
}
///3. ���� ��������
void zigbee_device::WriteDataInt(const int temp)
{
     boost::asio::write(GetPort(), boost::asio::buffer(&temp,4));
}
//4. ���͵����ַ�
void zigbee_device::WriteData(const char temp)
{
	boost::asio::write(GetPort(), boost::asio::buffer(&temp,1));
}
void zigbee_device::WriteDataPPrt(UINT8 temp[][16],size_t nSize)
{
	 boost::asio::write(GetPort(), boost::asio::buffer(temp,nSize));
}
///�رմ���
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
