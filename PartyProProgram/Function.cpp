#include "stdafx.h"
#include "Function.h"
#include "DlgPort.h"
#include "SystemConfig.h"
#include "PartyProProgramDlg.h"

CCriticalSection g_CriticalSection;//�����ٽ���

CString PlayCurrentScene(SCENE* pRunData)
{
	CString sendStr="";
	for (int i = 0; i < MSG_TOP_COUNT; i++)
	{
		sendStr.AppendFormat("%02X ", MSG_TOP_STR_VALUE);
	}
	sendStr.AppendFormat("%02X ", 0);;

	g_CriticalSection.Lock();
	for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
	{
		sendStr.AppendFormat("%02X ", pRunData->channelValue[i]);
	}
	g_CriticalSection.Unlock();

	for (int i = 0; i < 10; i++)
	{
		//sendStr.AppendFormat("%02d",g_ZgbSetDoc.m_puVoiceTen[i]);
		sendStr.AppendFormat("%02X ", 0);
	}
	return sendStr;
}


CString PlayRunScene(SCENE* pRunData)
{
	CString sendStr = "";
	for (int i = 0; i < MSG_TOP_COUNT; i++)
	{
		sendStr.AppendFormat("%02X ", MSG_TOP_STR_VALUE);
	}
	sendStr.AppendFormat("%02X ", 0);;

	g_CriticalSection.Lock();
	for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
	{
		sendStr.AppendFormat("%02X ", pRunData->channelValue[i]);
	}
	g_CriticalSection.Unlock();

	for (int i = 0; i < 10; i++)
	{
		//sendStr.AppendFormat("%02d",g_ZgbSetDoc.m_puVoiceTen[i]);
		sendStr.AppendFormat("%02X ", 0);
	}
	return sendStr;
}

////�̺߳�������������
UINT ThreadDebugScene( LPVOID pParam )
{
	CPartyProProgramDlg* pThis = (CPartyProProgramDlg*)pParam;
	CString sendStr;
	BOOL flag = FALSE;
	while(!pThis->bThreadExit)
	{
		sendStr = "";
		if (CDlgPort::getContext()->m_curData == nullptr) {
			AfxMessageBox("δѡ�е�ǰͨѶ����Ͷ˿ڣ�");
			break;
		}

		if (!pThis->m_check_loop) {
			sendStr = PlayCurrentScene(pThis->m_curData);
			flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
			if (flag<0)break;
		}
		else {
			std::vector<SCENE*>::iterator scene_vec_iter;
			SCENE* pData = nullptr;
			SCENE* pNextData=nullptr;
			SCENE* pRunData = new SCENE;
			long start = 0,end = 0,sentTimes=1;
			int inter_value_temp=0,max_inter_value=1;
			
			std::vector<SCENE*>g_scene_vec_temp;
			g_CriticalSection.Lock();
			g_scene_vec_temp = g_Config.g_rigion[g_Config.g_rigion_num].scene_vec;
			g_CriticalSection.Unlock();

			for (scene_vec_iter = g_scene_vec_temp.begin(); scene_vec_iter < g_scene_vec_temp.end(); )
			{
				if (pThis->bThreadExit|| !pThis->m_check_loop)break;
				pData = *scene_vec_iter;
				if (scene_vec_iter == g_scene_vec_temp.end()-1)
					scene_vec_iter = g_scene_vec_temp.begin();
				else
					scene_vec_iter++;
				start = GetTickCount();
				sendStr = PlayRunScene(pData);
				CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
				end = GetTickCount();
				sentTimes = pData->shapeTime * 1000 / (end - start);
				TRACE("����ʱ�䣺%.3f,���������%d\n", (end - start) / 1000.0f, sentTimes);

				//��ȡ��һ������
				if (scene_vec_iter < g_scene_vec_temp.end())
					pNextData = *scene_vec_iter;
				else {
					pNextData = *(g_scene_vec_temp.begin());
				}

				for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
				{
					inter_value_temp = abs(pNextData->channelValue[i] - pData->channelValue[i]);
					if (max_inter_value < inter_value_temp)max_inter_value = inter_value_temp;
				}
				sentTimes = sentTimes < max_inter_value ? sentTimes : max_inter_value;

				if (sentTimes <= 1)sentTimes = 1;
				//����������������С�ָ�
				for (int k = 0; k < sentTimes;k++) {
					if (pThis->bThreadExit)break;
					start = GetTickCount();
					for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
					{
						inter_value_temp = pNextData->channelValue[i] - pData->channelValue[i];
						pRunData->channelValue[i] = pData->channelValue[i]+inter_value_temp*k / sentTimes;
					}
					sendStr = PlayRunScene(pRunData);
					flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
					if (flag < 0)break;
					end = GetTickCount();			
					//sentTimes = pData->shapeTime*1000 / (end-start);
					if(pData->shapeTime * 1000 / sentTimes>(end - start))
					Sleep(pData->shapeTime * 1000 / sentTimes- (end - start));
					
					TRACE("����ʱ�䣺%.3f,���������%d,������ֵ��%d\n", (end - start) / 1000.0f, sentTimes, pRunData->channelValue[96]);
				}

				//�ȴ�ʱ��
				Sleep(pData->waitTime*1000);

				
			}
			delete pRunData;
			pRunData = nullptr;
		}

		//CDlgPort::getContext()->m_curData->sendPortOnly(sendStr); 
		
		////ÿ�η�������ʱ��ͣ��40����
		Sleep(35); 
	}
	pThis->bThreadExit = false;

	return 0;
}



///���͵�����
UINT ThreadDebugRecord( LPVOID pParam)
{


	CPartyProProgramDlg* pThis = (CPartyProProgramDlg*)pParam;
	CString sendStr;
	BOOL flag = FALSE;
	while (!pThis->bThreadExit)
	{
		sendStr = "";
		if (CDlgPort::getContext()->m_curData == nullptr) {
			AfxMessageBox("δѡ�е�ǰͨѶ����Ͷ˿ڣ�");
			break;
		}

		std::vector<SCENE*>::iterator scene_vec_iter;
		SCENE* pData = nullptr;
		long start = 0, end = 0, sentTimes = 1;
		int inter_value_temp = 0, max_inter_value = 0;

		int iSendFirstData = 0, scaneIndex=0;
		std::vector<SCENE*>g_scene_vec_temp;
		g_CriticalSection.Lock();
		g_scene_vec_temp = g_Config.g_rigion[g_Config.g_rigion_num].scene_vec;
		g_CriticalSection.Unlock();

		for (scene_vec_iter = g_scene_vec_temp.begin(); scene_vec_iter < g_scene_vec_temp.end(); scene_vec_iter++)
		{

			pData = *scene_vec_iter;
			scaneIndex++;
			//sendStr = PlayRunScene(pData);
			flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
			if (flag < 0)return -1;
			

					///��ʽ���͵���Ϣͷ6���ֽ�
					for (int i = 0; i < MSG_TOP_COUNT; i++)
					{
						sendStr.AppendFormat("%02X ", MSG_READY_RECORD);
					}
					sendStr.AppendFormat("%02X ", 0);
					flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
					if (flag < 0)return -1;

					int iIndex = 0;
					if (iSendFirstData == 0) {	//��һ֡���� 
						//����ǵ�һ֡������һ֡��Ϊ���� ����״̬ �� ��������
						for (int i = 0; i < 240; i++)
						{
							sendStr.AppendFormat("%02X ", 0);
						}
						sendStr.AppendFormat("%s ", 0xEF);///������ʶ ΪEF�����н�Ŀ���������ݷ������
						sendStr.AppendFormat("%02X ", 0);
						sendStr.AppendFormat("%02X ", 0);
						sendStr.AppendFormat("%02X ", g_scene_vec_temp.size());
						sendStr.AppendFormat("%02X ", 0);
						for (int i = 0; i < 11; i++)
						{
							sendStr.AppendFormat("%02X ", 0);
						}
						flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
						if (flag < 0)return -1;

						//���¿�ʼ�ܵ�һ֡
						iSendFirstData = 1;
						continue;
					}
					else {
						g_CriticalSection.Lock();
						for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
						{
							sendStr.AppendFormat("%02X ", pData->channelValue[i]);
						}
						g_CriticalSection.Unlock();
						flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
						if (flag < 0)return -1;
					}

					if (scene_vec_iter == (g_scene_vec_temp.end() - 1)) ///�������Ϊ���һ�������ͼ��Ͻ�����ʶ
					{
							sendStr.AppendFormat("%s ", 0xCC);///������ʶ ΪCC�����н�Ŀ���������ݷ������
							sendStr.AppendFormat("%02X ", pData->shapeTime);///����Yֵ
							sendStr.AppendFormat("%02X ", pData->shapeTime);
							sendStr.AppendFormat("%02X ", 0);///��Ŀ���
							sendStr.AppendFormat("%02X ", scaneIndex);
							for (int i = 0; i < 11; i++)
							{
								sendStr.AppendFormat("%02X ", 0);
							}
							flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
							if (flag < 0)return -1;
							return 0;
						//}
						//else
						//{
						//	sendStr.AppendFormat("%s ", 0xBB);///������ʶ ΪCC�����н�Ŀ���������ݷ������
						//	sendStr.AppendFormat("%02X ", pData->shapeTime);///����Yֵ
						//	sendStr.AppendFormat("%02X ", pData->shapeTime);
						//	sendStr.AppendFormat("%02X ", 0);///��Ŀ���
						//	sendStr.AppendFormat("%02X ", scaneIndex);
						//	for (int i = 0; i < 11; i++)
						//	{
						//		sendStr.AppendFormat("%s ", 0);
						//	}
						//	flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
						//	if (flag < 0)return -1;
						//}
					}
					else///�������һ�룬
					{
						sendStr.AppendFormat("%s ", 0xAA);///������ʶ ΪCC�����н�Ŀ���������ݷ������
						sendStr.AppendFormat("%02X ", pData->shapeTime);///����Yֵ
						sendStr.AppendFormat("%02X ", pData->shapeTime);
						sendStr.AppendFormat("%02X ", 0);///��Ŀ���
						sendStr.AppendFormat("%02X ", scaneIndex);
						for (int i = 0; i < 11; i++)
						{
							sendStr.AppendFormat("%02X ", 0);
						}
						flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
						if (flag < 0)return -1;
					}

		}

		////ÿ�η�������ʱ��ͣ��40����
		Sleep(35);
	}
	pThis->bThreadExit = false;

	return 0;



	//Sleep(1000);
	//////�����ǲ���ͬ��
	//TCHAR szPath[MAX_PATH]={0};
	//if(!GetCurrentDirectory(MAX_PATH,szPath))///��ȡ��ǰִ�г����·����������������
	//{
	//	if(g_bIsProcessStart)///�����ļ���ʱ����
	//	{
	//		m_Proc.PostMessage(WM_CLOSE,0,0);///�رս�����
	//	} 
	//	//AfxMessageBox(StringArray[20]);
	//	return;
	//}

	//CString strPath;
	//CString strTenToSixteen;
	//int iTVSize =0;
	//std::map<UINT8,CString>::iterator itMap;////ӳ�伯���࣬��һ��Ԫ��Ϊ��ֵ���ڶ���Ԫ��Ϊ�ַ���
	//UINT8 cNum =0;///��Ŀ���
	//UINT8 *pText =NULL;
	//for(itMap =g_ZgbSetDoc.m_mapTVName.begin();itMap != g_ZgbSetDoc.m_mapTVName.end();itMap++,iTVSize++)
	//{
	//	memset(m_pSendText,0,256);
	//	pText =m_pSendText;
	//	cNum =itMap->first+1;///��ȡ��Ŀ���
	//	strPath.Format(_T("%s//Program file//%s.ppl"),szPath,(*itMap).second);///�õ���Program file��·��
	//	CFile file;
	//	int iRet = file.Open(strPath,CFile::modeRead);///�򿪽�Ŀ �ļ�
	//	if(iRet ==FALSE)
	//	{
	//		Sleep(200);
	//		if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//		{
	//			g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//		}
	//		if(g_bIsProcessStart)///�����ļ���ʱ����
	//		{
	//			m_Proc.PostMessage(WM_CLOSE,0,0);///�رս�����
	//		} 
	//		return;
	//	}
	//	///���л���ȡ����
	//	CArchive ar(&file,CArchive::load);///��ȡ����
	//	Serialize(ar);
	//	ar.Close();
	//	file.Close();

	//	int iCfgQuence =g_ZgbSetDoc.m_arryCfgFile.GetCount();///��ǰ��Ŀ�ж��ٸ�����
	//	BOOL iSendFirstData=0;
	//	for(int iCount =0;iCount <iCfgQuence;iCount++)///ѭ����Ŀ�����г���
	//	{
	//		if(m_bIsFinishOneData)////�Ƿ����һ�����ݷ��͵ı�ʶ��һ��ʼ�ͷ�������Ϣͷ��Ҫ�з��ز�������������
	//		{
	//			if(IsRecordBreakOff())
	//			{
	//				TRACE("634,cNum=%d,iCount=%d\n",cNum,iCount);
	//				return;////�����ж��Ƿ��ͻȻ��ֹ¼�ƣ�����ǣ��ͷ�����ֹ����
	//			}

	//			TRACE("694,cNum=%d,iCount=%d\n",cNum,iCount);
	//			///��ʽ���͵���Ϣͷ6���ֽ�
	//			for(int i=0;i<MSG_TOP_COUNT;i++)
	//			{
	//				if(i==MSG_TOP_COUNT-1)
	//				{
	//					msg_Top[i] =0;   ///��6���ֽ�Ϊ00
	//					break;
	//				}
	//				msg_Top[i]=MSG_READY_RECORD; ///5�ֽ�5B +00
	//			} 
	//			g_uExcFlag =EXC_RECORD_MODE;///��ʶ��Ϊ��������
	//			g_zigbee_device.WriteDataUnPrt(msg_Top,MSG_TOP_COUNT-1);
	//			Sleep(1);
	//			int iIndex =0;
	//			if(iSendFirstData == 0){	//��һ֡���� 
	//				//����ǵ�һ֡������һ֡��Ϊ���� ����״̬ �� ��������
	//				for(int i=0;i<LIGHT_COUNT;i++)
	//				{
	//					for(int j=0;j<16;j++)
	//					{
	//						if(g_ZgbSetDoc.m_iVoiceCtrl[i][j]==1){
	//							pText[iIndex] = 0xFF;	
	//						}
	//						else{
	//							pText[iIndex] = 0x00;
	//						}
	//						iIndex++;
	//					}
	//				}
	//				pText =pText+GARREYCOUNT;
	//				*pText=0xEF;///������ʶ ΪEF�����н�Ŀ���������ݷ������
	//				pText++;
	//				*pText =0x00;///����Yֵ
	//				pText++;
	//				*pText =0x00;///����Nֵ
	//				pText++;
	//				*pText = (UINT8)iCfgQuence;///������
	//				pText++;
	//				*pText =0x00;
	//				pText++;
	//				*pText =(*g_ZgbSetDoc.m_voiceStyleTemp)*16+(*g_ZgbSetDoc.m_voiceTypeTemp);
	//				//TRACE("*g_ZgbSetDoc.m_voiceTypeTemp=%X\n",(*g_ZgbSetDoc.m_voiceStyleTemp)*16+(*g_ZgbSetDoc.m_voiceTypeTemp));
	//				pText++;
	//				for(int i=0;i<8;i++)
	//				{
	//					*pText =0x00;
	//					pText++;
	//				}
	//				*pText =0x00;
	//				pText++;
	//				*pText =0x00;
	//				g_zigbee_device.WriteDataUnPrt(m_pSendText,256);
	//				pText =m_pSendText;
	//				//���¿�ʼ�ܵ�һ֡
	//				iSendFirstData=1;
	//				iCount = -1;
	//				m_bIsFinishOneData =FALSE;
	//				continue;
	//			}
	//			else{	   
	//				for(int i=0;i<LIGHT_COUNT;i++)
	//				{
	//					for(int j=0;j<16;j++)
	//					{
	//						pText[iIndex] =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).m_arryGallery[i][j];
	//						iIndex++;
	//					}
	//				}
	//				pText =pText+GARREYCOUNT;
	//			} 

	//			//iIndex=0;
	//			TRACE("719,cNum=%d,iCount=%d\n",cNum,iCount);
	//			if(iCount==iCfgQuence -1) ///�������Ϊ���һ�룬�ͼ��Ͻ�����ʶ
	//			{
	//				TRACE("720\n");
	//				///��������һ����Ŀ�������⴦��������ʶΪ�ã�
	//				if(iTVSize ==g_ZgbSetDoc.m_mapTVName.size()-1)
	//				{
	//					TRACE("724\n");
	//					*pText=0xCC;///������ʶ ΪCC�����н�Ŀ���������ݷ������
	//					pText++;
	//					*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_K;///����Yֵ
	//					pText++;
	//					*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_N;///����Nֵ
	//					pText++;
	//					*pText =cNum;///��Ŀ���
	//					pText++;
	//					*pText =iCount;
	//					pText++;
	//					*pText =(*g_ZgbSetDoc.m_voiceStyleTemp);
	//					pText++;
	//					for(int i=0;i<8;i++)
	//					{
	//						*pText =g_ZgbSetDoc.m_puVoiceTen[i];
	//						pText++;
	//					}
	//					*pText =(*g_ZgbSetDoc.m_voiceStyleTemp);
	//					pText++;
	//					*pText =(*g_ZgbSetDoc.m_voiceStyleTemp);
	//					g_uExcFlag =EXC_RECORD_MODE_ALL;
	//					g_zigbee_device.WriteDataUnPrt(m_pSendText,256);
	//					pText =m_pSendText;
	//					TRACE("748\n");
	//					return;
	//				}
	//				else
	//				{
	//					TRACE("754\n");
	//					*pText =0xBB;///������ʶ Ϊ��һ����Ŀ���������ݷ������
	//					pText++;
	//					*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_K;///����Yֵ
	//					pText++;
	//					*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_N;///����Nֵ
	//					pText++;
	//					*pText =cNum;///��Ŀ���
	//					pText++;
	//					*pText =iCount;
	//					pText++;
	//					*pText =(*g_ZgbSetDoc.m_voiceStyleTemp);
	//					pText++;
	//					for(int i=0;i<8;i++)
	//					{
	//						*pText =g_ZgbSetDoc.m_puVoiceTen[i];
	//						pText++;
	//					}
	//					*pText =(*g_ZgbSetDoc.m_voiceStyleTemp);
	//					pText++;
	//					*pText =(*g_ZgbSetDoc.m_voiceStyleTemp);
	//					g_zigbee_device.WriteDataUnPrt(m_pSendText,256);
	//					pText =m_pSendText;
	//				}
	//			}
	//			else///�������һ�룬
	//			{
	//				TRACE("780,cNum=%d,iCount=%d\n",cNum,iCount);
	//				*pText =0xAA;///������ʶ Ϊ��һ�������������
	//				pText++;
	//				*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_K;///����Yֵ
	//				pText++;
	//				*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_N;///����Nֵ
	//				pText++;
	//				*pText =cNum;///��Ŀ���
	//				pText++;
	//				*pText =iCount;
	//				pText++;
	//				*pText =(*g_ZgbSetDoc.m_voiceStyleTemp);
	//				pText++;
	//				for(int i=0;i<8;i++)
	//				{
	//					*pText =g_ZgbSetDoc.m_puVoiceTen[i];
	//					pText++;
	//				}
	//				*pText =(*g_ZgbSetDoc.m_voiceStyleTemp);
	//				pText++;
	//				*pText =(*g_ZgbSetDoc.m_voiceStyleTemp);
	//				TRACE("802\n");
	//				g_zigbee_device.WriteDataUnPrt(m_pSendText,256);
	//				pText =m_pSendText;
	//				TRACE("805\n");
	//			}
	//			m_bIsFinishOneData =FALSE;
	//			//	   mu.unlock();
	//		}
	//		else
	//		{
	//			for(int j=0;j<25;j++)
	//			{
	//				if(IsRecordBreakOff()) return;//�ж��Ƿ�ͻȻ��ֹ
	//				///�ж��Ƿ�տ�ʼ¼�Ƶ���Ϣͷ���
	//				if(m_iIsFirstSend)
	//				{	
	//					for(int k=0;k<1000;k++)
	//					{
	//						if(IsRecordBreakOff())
	//						{
	//							return;//�ж��Ƿ�ͻȻ��ֹ
	//						}
	//						Sleep(40);
	//						if(!m_iIsFirstSend) break;
	//						if(j==2)
	//						{ 
	//							if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//							{
	//								g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//							}
	//							if(g_bIsProcessStart)///�����ļ���ʱ����
	//							{
	//								m_Proc.PostMessage(WM_CLOSE,0,0);///�رս�����

	//							}  
	//							for(int i=0;i<MSG_TOP_COUNT-1;i++)
	//							{
	//								msg_Top[i]=MSG_RECORD_FAILED; ////�����;�˳����ͷ�ʧ��ID��5E��
	//							}
	//							g_zigbee_device.WriteDataUnPrt(msg_Top,MSG_TOP_COUNT-1);///����5���ֽڵ�5E
	//							return;
	//						}
	//						continue;
	//					}
	//				}
	//				////ÿ������һ��ı�ʶ�����س�ʱ����
	//				if(!m_bIsFinishOneData)
	//				{ 
	//					boost::mutex mu;////��һ�ѻ�����
	//					mu.lock();
	//					Sleep(50);
	//					if(j==24)
	//					{
	//						if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//						{
	//							g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//						}
	//						if(g_bIsProcessStart)///�����ļ���ʱ����
	//						{
	//							m_Proc.PostMessage(WM_CLOSE,0,0);///�رս�����

	//						}  
	//						for(int i=0;i<MSG_TOP_COUNT-1;i++)
	//						{
	//							msg_Top[i]=MSG_RECORD_FAILED; ////�����;�˳����ͷ�ʧ��ID��5E��
	//						}
	//						g_zigbee_device.WriteDataUnPrt(msg_Top,MSG_TOP_COUNT-1);///����5���ֽڵ�5E
	//						return;
	//					}
	//					mu.unlock();
	//				}
	//				else
	//				{
	//					iCount--;///��Ŀ˳���һ
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}

	//return 0;
}




