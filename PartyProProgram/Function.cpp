#include "stdafx.h"
#include "Function.h"
#include "DlgPort.h"
#include "SystemConfig.h"
#include "PartyProProgramDlg.h"

CCriticalSection g_CriticalSection;//定义临界区

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

////线程函数处理发送数据
UINT ThreadDebugScene( LPVOID pParam )
{
	CPartyProProgramDlg* pThis = (CPartyProProgramDlg*)pParam;
	CString sendStr;
	BOOL flag = FALSE;
	while(!pThis->bThreadExit)
	{
		sendStr = "";
		if (CDlgPort::getContext()->m_curData == nullptr) {
			AfxMessageBox("未选中当前通讯的域和端口！");
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
				TRACE("发送时间：%.3f,渐变次数：%d\n", (end - start) / 1000.0f, sentTimes);

				//获取下一个场景
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
				//找最大间隔，并求出最小分隔
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
					
					TRACE("发送时间：%.3f,渐变次数：%d,渐变数值：%d\n", (end - start) / 1000.0f, sentTimes, pRunData->channelValue[96]);
				}

				//等待时间
				Sleep(pData->waitTime*1000);

				
			}
			delete pRunData;
			pRunData = nullptr;
		}

		//CDlgPort::getContext()->m_curData->sendPortOnly(sendStr); 
		
		////每次发送数据时都停顿40毫秒
		Sleep(35); 
	}
	pThis->bThreadExit = false;

	return 0;
}



///发送的数据
UINT ThreadDebugRecord( LPVOID pParam)
{


	CPartyProProgramDlg* pThis = (CPartyProProgramDlg*)pParam;
	CString sendStr;
	BOOL flag = FALSE;
	while (!pThis->bThreadExit)
	{
		sendStr = "";
		if (CDlgPort::getContext()->m_curData == nullptr) {
			AfxMessageBox("未选中当前通讯的域和端口！");
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
			

					///正式发送的消息头6个字节
					for (int i = 0; i < MSG_TOP_COUNT; i++)
					{
						sendStr.AppendFormat("%02X ", MSG_READY_RECORD);
					}
					sendStr.AppendFormat("%02X ", 0);
					flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
					if (flag < 0)return -1;

					int iIndex = 0;
					if (iSendFirstData == 0) {	//第一帧发送 
						//如果是第一帧，则发送一帧作为传递 声控状态 和 声控类型
						for (int i = 0; i < 240; i++)
						{
							sendStr.AppendFormat("%02X ", 0);
						}
						sendStr.AppendFormat("%s ", 0xEF);///结束标识 为EF，所有节目的所有数据发送完毕
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

						//重新开始跑第一帧
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

					if (scene_vec_iter == (g_scene_vec_temp.end() - 1)) ///如果发送为最后一场景，就加上结束标识
					{
							sendStr.AppendFormat("%s ", 0xCC);///结束标识 为CC，所有节目的所有数据发送完毕
							sendStr.AppendFormat("%02X ", pData->shapeTime);///发送Y值
							sendStr.AppendFormat("%02X ", pData->shapeTime);
							sendStr.AppendFormat("%02X ", 0);///节目编号
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
						//	sendStr.AppendFormat("%s ", 0xBB);///结束标识 为CC，所有节目的所有数据发送完毕
						//	sendStr.AppendFormat("%02X ", pData->shapeTime);///发送Y值
						//	sendStr.AppendFormat("%02X ", pData->shapeTime);
						//	sendStr.AppendFormat("%02X ", 0);///节目编号
						//	sendStr.AppendFormat("%02X ", scaneIndex);
						//	for (int i = 0; i < 11; i++)
						//	{
						//		sendStr.AppendFormat("%s ", 0);
						//	}
						//	flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
						//	if (flag < 0)return -1;
						//}
					}
					else///不是最后一针，
					{
						sendStr.AppendFormat("%s ", 0xAA);///结束标识 为CC，所有节目的所有数据发送完毕
						sendStr.AppendFormat("%02X ", pData->shapeTime);///发送Y值
						sendStr.AppendFormat("%02X ", pData->shapeTime);
						sendStr.AppendFormat("%02X ", 0);///节目编号
						sendStr.AppendFormat("%02X ", scaneIndex);
						for (int i = 0; i < 11; i++)
						{
							sendStr.AppendFormat("%02X ", 0);
						}
						flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
						if (flag < 0)return -1;
					}

		}

		////每次发送数据时都停顿40毫秒
		Sleep(35);
	}
	pThis->bThreadExit = false;

	return 0;



	//Sleep(1000);
	//////查找是不是同名
	//TCHAR szPath[MAX_PATH]={0};
	//if(!GetCurrentDirectory(MAX_PATH,szPath))///获取当前执行程序的路径，不包括程序名
	//{
	//	if(g_bIsProcessStart)///加载文件超时处理
	//	{
	//		m_Proc.PostMessage(WM_CLOSE,0,0);///关闭进度条
	//	} 
	//	//AfxMessageBox(StringArray[20]);
	//	return;
	//}

	//CString strPath;
	//CString strTenToSixteen;
	//int iTVSize =0;
	//std::map<UINT8,CString>::iterator itMap;////映射集合类，第一个元素为数值，第二个元素为字符串
	//UINT8 cNum =0;///节目编号
	//UINT8 *pText =NULL;
	//for(itMap =g_ZgbSetDoc.m_mapTVName.begin();itMap != g_ZgbSetDoc.m_mapTVName.end();itMap++,iTVSize++)
	//{
	//	memset(m_pSendText,0,256);
	//	pText =m_pSendText;
	//	cNum =itMap->first+1;///获取节目编号
	//	strPath.Format(_T("%s//Program file//%s.ppl"),szPath,(*itMap).second);///得到打开Program file的路径
	//	CFile file;
	//	int iRet = file.Open(strPath,CFile::modeRead);///打开节目 文件
	//	if(iRet ==FALSE)
	//	{
	//		Sleep(200);
	//		if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//		{
	//			g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//		}
	//		if(g_bIsProcessStart)///加载文件超时处理
	//		{
	//			m_Proc.PostMessage(WM_CLOSE,0,0);///关闭进度条
	//		} 
	//		return;
	//	}
	//	///序列化提取内容
	//	CArchive ar(&file,CArchive::load);///提取内容
	//	Serialize(ar);
	//	ar.Close();
	//	file.Close();

	//	int iCfgQuence =g_ZgbSetDoc.m_arryCfgFile.GetCount();///当前节目有多少个场景
	//	BOOL iSendFirstData=0;
	//	for(int iCount =0;iCount <iCfgQuence;iCount++)///循环节目中所有场景
	//	{
	//		if(m_bIsFinishOneData)////是否完成一次数据发送的标识　一开始就发送了消息头，要有返回才真正发送数据
	//		{
	//			if(IsRecordBreakOff())
	//			{
	//				TRACE("634,cNum=%d,iCount=%d\n",cNum,iCount);
	//				return;////首先判断是否会突然中止录制，如果是，就发送中止命令
	//			}

	//			TRACE("694,cNum=%d,iCount=%d\n",cNum,iCount);
	//			///正式发送的消息头6个字节
	//			for(int i=0;i<MSG_TOP_COUNT;i++)
	//			{
	//				if(i==MSG_TOP_COUNT-1)
	//				{
	//					msg_Top[i] =0;   ///第6个字节为00
	//					break;
	//				}
	//				msg_Top[i]=MSG_READY_RECORD; ///5字节5B +00
	//			} 
	//			g_uExcFlag =EXC_RECORD_MODE;///标识设为导入配置
	//			g_zigbee_device.WriteDataUnPrt(msg_Top,MSG_TOP_COUNT-1);
	//			Sleep(1);
	//			int iIndex =0;
	//			if(iSendFirstData == 0){	//第一帧发送 
	//				//如果是第一帧，则发送一帧作为传递 声控状态 和 声控类型
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
	//				*pText=0xEF;///结束标识 为EF，所有节目的所有数据发送完毕
	//				pText++;
	//				*pText =0x00;///发送Y值
	//				pText++;
	//				*pText =0x00;///发送N值
	//				pText++;
	//				*pText = (UINT8)iCfgQuence;///场景数
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
	//				//重新开始跑第一帧
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
	//			if(iCount==iCfgQuence -1) ///如果发送为最后一针，就加上结束标识
	//			{
	//				TRACE("720\n");
	//				///如果是最后一个节目，就另外处理　结束标识为ＣＣ
	//				if(iTVSize ==g_ZgbSetDoc.m_mapTVName.size()-1)
	//				{
	//					TRACE("724\n");
	//					*pText=0xCC;///结束标识 为CC，所有节目的所有数据发送完毕
	//					pText++;
	//					*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_K;///发送Y值
	//					pText++;
	//					*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_N;///发送N值
	//					pText++;
	//					*pText =cNum;///节目编号
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
	//					*pText =0xBB;///结束标识 为，一个节目的所有数据发送完毕
	//					pText++;
	//					*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_K;///发送Y值
	//					pText++;
	//					*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_N;///发送N值
	//					pText++;
	//					*pText =cNum;///节目编号
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
	//			else///不是最后一针，
	//			{
	//				TRACE("780,cNum=%d,iCount=%d\n",cNum,iCount);
	//				*pText =0xAA;///结束标识 为，一个场景发送完毕
	//				pText++;
	//				*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_K;///发送Y值
	//				pText++;
	//				*pText =g_ZgbSetDoc.m_arryCfgFile.GetAt(iCount).Value_N;///发送N值
	//				pText++;
	//				*pText =cNum;///节目编号
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
	//				if(IsRecordBreakOff()) return;//判断是否突然中止
	//				///判断是否刚开始录制的消息头命令，
	//				if(m_iIsFirstSend)
	//				{	
	//					for(int k=0;k<1000;k++)
	//					{
	//						if(IsRecordBreakOff())
	//						{
	//							return;//判断是否突然中止
	//						}
	//						Sleep(40);
	//						if(!m_iIsFirstSend) break;
	//						if(j==2)
	//						{ 
	//							if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//							{
	//								g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//							}
	//							if(g_bIsProcessStart)///加载文件超时处理
	//							{
	//								m_Proc.PostMessage(WM_CLOSE,0,0);///关闭进度条

	//							}  
	//							for(int i=0;i<MSG_TOP_COUNT-1;i++)
	//							{
	//								msg_Top[i]=MSG_RECORD_FAILED; ////如果中途退出，就发失败ID（5E）
	//							}
	//							g_zigbee_device.WriteDataUnPrt(msg_Top,MSG_TOP_COUNT-1);///发送5个字节的5E
	//							return;
	//						}
	//						continue;
	//					}
	//				}
	//				////每发送完一针的标识，返回超时处理
	//				if(!m_bIsFinishOneData)
	//				{ 
	//					boost::mutex mu;////加一把互拆锁
	//					mu.lock();
	//					Sleep(50);
	//					if(j==24)
	//					{
	//						if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//						{
	//							g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//						}
	//						if(g_bIsProcessStart)///加载文件超时处理
	//						{
	//							m_Proc.PostMessage(WM_CLOSE,0,0);///关闭进度条

	//						}  
	//						for(int i=0;i<MSG_TOP_COUNT-1;i++)
	//						{
	//							msg_Top[i]=MSG_RECORD_FAILED; ////如果中途退出，就发失败ID（5E）
	//						}
	//						g_zigbee_device.WriteDataUnPrt(msg_Top,MSG_TOP_COUNT-1);///发送5个字节的5E
	//						return;
	//					}
	//					mu.unlock();
	//				}
	//				else
	//				{
	//					iCount--;///节目顺序减一
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}

	//return 0;
}




