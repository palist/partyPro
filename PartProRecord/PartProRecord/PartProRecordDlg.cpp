
// PartProRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PartProRecord.h"
#include "PartProRecordDlg.h"
#include "afxdialogex.h"
#include "AddPasserword.h"
#include "../../PartyProProgram/SystemConfig.h"
#include "../../PartyProProgram/Port.h"
#include "../../PartyProProgram/DlgPort.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT threadSendData(LPVOID p);

CString get_current_path()
{
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
	CString szPath = szFilePath;
	return szPath;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPartProRecordDlg 对话框




CPartProRecordDlg::CPartProRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPartProRecordDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_Record_Report = new CSuperGridCtrl();
	//通讯窗口
	CDlgPort::getContext()->Create(IDD_DLG_MUL_PORT, this);
	CDlgPort::getContext()->ShowWindow(SW_HIDE);

}

CPartProRecordDlg::~CPartProRecordDlg()
{
	delete m_Record_Report;
	m_Record_Report = nullptr;
}


void CPartProRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NORMAL, m_combo_normal);
	DDX_Control(pDX, IDC_COMBO_SERIOR, m_combo_serior);

}

BEGIN_MESSAGE_MAP(CPartProRecordDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RECORD_ONE_KEY, &CPartProRecordDlg::OnBnClickedBtnRecordOneKey)
	ON_BN_CLICKED(IDBTN_CLEAN_FESTIVAL, &CPartProRecordDlg::OnBnClickedCleanFestival)
	ON_BN_CLICKED(IDBTN_CLEAN_SELECT, &CPartProRecordDlg::OnBnClickedCleanSelect)
	ON_BN_CLICKED(IDBTN_LOAD_IN, &CPartProRecordDlg::OnBnClickedLoadIn)
	ON_MESSAGE(WM_USER_DBLCLK, &CPartProRecordDlg::OnNMDblclkList)
	ON_CBN_KILLFOCUS(IDC_COMBO_NORMAL, &CPartProRecordDlg::OnCbnKillfocusComboKnife)
	ON_CBN_KILLFOCUS(IDC_COMBO_SERIOR, &CPartProRecordDlg::OnCbnKillfocusComboKnife)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CPartProRecordDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPartProRecordDlg 消息处理程序

BOOL CPartProRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	//InitXTPRecordReport();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitXTPRecordReport();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPartProRecordDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPartProRecordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPartProRecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


///初始化列表
BOOL CPartProRecordDlg::InitXTPRecordReport(void)
{
	
	m_Record_Report->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 100, 100), this, 0x1003);
	m_Record_Report->SetExtendedStyle(LVS_EX_GRIDLINES/*| LVS_EX_CHECKBOXES*/);


	int nCount = m_Record_Report->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_Record_Report->DeleteColumn(0);
	}
	m_Record_Report->InsertColumn(0, _T("序号"), LVCFMT_LEFT, 80);
	m_Record_Report->InsertColumn(1, _T("节目类型"), LVCFMT_LEFT, 100);
	m_Record_Report->InsertColumn(2, _T("常规节目"), LVCFMT_LEFT, 100);
	m_Record_Report->InsertColumn(3, _T("声光联动节目"), LVCFMT_LEFT, 100);

	CRect grid_rc1;
	GetDlgItem(IDC_STATIC_LIST)->GetWindowRect(grid_rc1);
	ScreenToClient(grid_rc1);
	grid_rc1.top += 5;
	grid_rc1.bottom -= 5;
	m_Record_Report->MoveWindow(&grid_rc1);

	m_Record_Report->Invalidate();

	////显示
	UpdateReocrdReport();///插入10行显示

	UpdateTVNameList();
	
	return TRUE;
}



///显示10行显示
void CPartProRecordDlg::UpdateReocrdReport(void)
{
	int index = 0;
	CString str;
	m_Record_Report->DeleteAll();

	for (int i=0;i<10;i++)
	{
		//SCENE* pData = *scene_vec_iter;
		//m_curData = pData;
		CItemInfo* info = new CItemInfo;
		info->SetImage(2);
		//info->SetData(pData);

		str.Format(_T("%d"), ++index);
		info->SetItemText(str);

		info->AddSubItemText(g_Config.g_record_tpye[i]);

		info->AddSubItemText("请选择");

		info->AddSubItemText("请选择");

		m_Record_Report->InsertRootItem(info);
	}

	m_Record_Report->Invalidate();
	
}


//双击时，可修改数据
LRESULT CPartProRecordDlg::OnNMDblclkList(WPARAM wParam, LPARAM lParam)
{
	m_iSubItem = (int)wParam;
	m_iItem = (int)lParam;

	if (m_iItem == -1)
		return 0;
	//获取列表上的文本，显示到Edit控件里
	CString itemTxt;;

	//调整Edit的宽、高，并移动
	CRect rect, dlgRect;
	int width;

	itemTxt = m_Record_Report->GetItemText(m_iItem, m_iSubItem);
	width = m_Record_Report->GetColumnWidth(m_iSubItem);
	m_Record_Report->GetWindowRect(&dlgRect);
	m_Record_Report->GetSubItemRect(m_iItem, m_iSubItem, LVIR_BOUNDS, rect);


	ScreenToClient(&dlgRect);
	int height = rect.Height();
	rect.top += (dlgRect.top);
	rect.left += (dlgRect.left);
	rect.bottom = rect.top + height;
	rect.right = rect.left + width;

	if (m_iSubItem == 2)
	{
		m_combo_normal.SetWindowText(itemTxt);
		::SetWindowPos(m_combo_normal.GetSafeHwnd(), HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height() + 100, SWP_SHOWWINDOW);
		m_combo_normal.ShowWindow(SW_SHOW);
		m_combo_normal.SetWindowTextA(itemTxt);
	}
	else if (m_iSubItem == 3)
	{
		m_combo_serior.SetWindowText(itemTxt);
		::SetWindowPos(m_combo_serior.GetSafeHwnd(), HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height() + 100, SWP_SHOWWINDOW);
		m_combo_serior.ShowWindow(SW_SHOW);
		m_combo_serior.SetWindowTextA(itemTxt);
	}
	return 0;
}



void CPartProRecordDlg::OnCbnKillfocusComboKnife()
{
	UpdateData(TRUE);

	int nSel = m_Record_Report->GetSelectedItem();
	if (nSel >= 0)
	{
		CSuperGridCtrl::CTreeItem* hCurItem = m_Record_Report->GetTreeItem(nSel);
		CItemInfo* pInfo = (CItemInfo*)m_Record_Report->GetData(hCurItem);
		//SCENE* pData = (SCENE*)pInfo->GetData();

		//修改
		CString str;
		if (m_iSubItem == 2) {
			m_combo_normal.GetWindowTextA(str);
			m_Record_Report->SetItemText(m_iItem, m_iSubItem, str);
			//pData->waitTime = atof(str);
			m_combo_normal.ShowWindow(SW_HIDE);
		}
		else if (m_iSubItem == 3) {
			m_combo_serior.GetWindowTextA(str);
			m_Record_Report->SetItemText(m_iItem, m_iSubItem, str);
			//pData->shapeTime = atof(str);
			m_combo_serior.ShowWindow(SW_HIDE);
		}
	
		m_Record_Report->Invalidate();
		m_Record_Report->SetFocus();
	}
}



////插入节目下拉表
void CPartProRecordDlg::UpdateTVNameList(void)
{
	 ////查找是不是同名
	CString szPath = get_current_path();
	CFileFind finder;
	CString strPath,strFileName,strTVName,strNum;
	int iCount=0;
	strPath.Format(_T("%s//Program file//*.ppl"),szPath);////打开文件的路径
	BOOL bWorking = finder.FindFile(strPath);////搜索所有Program file
	if(!bWorking)
	{
		//AfxMessageBox(StringArray[43]);
		return;
	}
	m_combo_normal.Clear();
	m_combo_serior.Clear();
	///开始循环所有Program file
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strFileName = finder.GetFileName();
		int iPos = strFileName.Find(_T("."));
		if (iPos == -1)continue;
		//////保存所有节目名称，并编上编号
		//strNum.Format(_T("%d"), iCount++);
		m_combo_normal.AddString(strFileName.Left(iPos));
		m_combo_serior.AddString(strFileName.Left(iPos));
	}
}


///一键录制按钮
void CPartProRecordDlg::OnBnClickedBtnRecordOneKey()
{
	record_vec.clear();
	for (UINT cNum = 0; cNum < m_Record_Report->GetCount(); cNum++)
	{
		CString Record_name = m_Record_Report->GetItemText(cNum, 2);
		if (Record_name.Compare("请选择") == 0)break;
		record_vec.push_back(Record_name);
	}
	for (UINT cNum = 0; cNum < m_Record_Report->GetCount(); cNum++)
	{
		CString Record_name = m_Record_Report->GetItemText(cNum, 3);
		if (Record_name.Compare("请选择") == 0)break;
		record_vec.push_back(Record_name);
	}

	m_bExit = FALSE;
	if (CDlgPort::getContext()->m_curData->m_isOpen==false)
	{
		AfxMessageBox("串口未连接！");
		return;
	}
	//std::vector<CPort*>::iterator vec_iter;
	//for (vec_iter = CDlgPort::getContext()->port_vec.begin(); vec_iter < CDlgPort::getContext()->port_vec.end(); vec_iter++)
	//{
	//	if ((*vec_iter)->m_isOpen == true)
	//	{
			AfxBeginThread(threadSendData, this);
	//	}
	//}
	
	CreateProDlg();
}



///重写序列化函数
void CPartProRecordDlg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())///如果 是储存数据
	{
	}
	else///如果是加载数据
	{
		long long sizeCount = 0;
		ar >> sizeCount;
		if (sizeCount != sizeof(CSystemConfig))
		{
			AfxMessageBox("此配置文件为旧软件生成，请删除原配置并重新生成配置文件！");
			DeleteFile(".//param.ppl");
			return;
		}

		int rigionCount = 0;
		ar >> g_Config.g_startPos;
		ar >> g_Config.g_redioIniValue;
		ar >> g_Config.g_rigion_num;

		ar >> rigionCount;

		for (int r = 0; r < rigionCount; r++)
		{
			ar >> g_Config.g_rigion[r].rigion_num;
			std::vector<SCENE*>vec_temp;
			g_Config.g_rigion[r].scene_vec.swap(vec_temp);
			int vec_count;
			ar >> vec_count;
			for (int i = 0; i < vec_count; i++)
			{
				SCENE* pData = new SCENE;

				for (int j = 0; j < CHANNEL_COUNT; j++)
				{
					ar >> pData->channelValue[j];	//通道值
					ar >> pData->voiceCheck[j];		//声光联动
				}

				ar >> pData->waitTime;	//等待时间
				ar >> pData->shapeTime;	//渐变时间
				ar >> pData->time;		//时间点

				g_Config.g_rigion[r].scene_vec.push_back(pData);
			}

			for (int k = 0; k < CHANNEL_COUNT; k++) {
				ar >> g_Config.g_rigion[r].RemarkName[k];
			}
			ar >> g_Config.g_rigion[r].control_select;
			ar >> g_Config.g_rigion[r].channel_type;

		}
		ar >> g_Config.g_bChangeScene;
		ar >> g_Config.g_waitTimeSetValue;
		ar >> g_Config.g_shapeTimeSetValue;
		ar >> g_Config.g_totalChannelValue;
		ar >> g_Config.g_bTotalEnable;

		////PORT
		//int port_count = 0;
		//ar >> port_count;
		//for (int i = 0; i < port_count; i++)
		//{
		//	CPort* pData = new CPort;
		//	ar >> pData->m_com_port;
		//	ar >> pData->m_region;

		//	CDlgPort::getContext()->port_vec.push_back(pData);
		//}

		for (int r = 0; r < RECORD_COUNT; r++)
		{
			ar >> g_Config.g_record_tpye[r];
		}
	}
}


/////发送的数据
//BOOL CPartProRecordDlg::SendRecordData(void)
//{
//	CString curPath = get_current_path();
//
//	CString strPath;
//	CString strTenToSixteen;
//	int iTVSize = 0;
//	CString sendStr = "";
//	for (int i = 0; i < MSG_TOP_COUNT; i++)
//	{
//		sendStr.AppendFormat("%02X ", MSG_TOP_RECORD_VALUE);
//	}
//	CString retValue;
//	retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
//	if (retValue.Compare("5B 5B 5B 5B 5B ")!=0) {
//		AfxMessageBox("返回值错误！");
//		return -1;
//	}
//
//
//	BOOL iSendFirstData = 0;
//	int flag = 0;
//
//	for (int cNum =0; !m_bExit&&cNum < record_vec.size(); cNum++)
//	{
//		strPath.Format(_T("%sProgram file//%s.ppl"), curPath, record_vec[cNum]);///得到打开Program file的路径
//		CFile file;
//		int iRet = file.Open(strPath, CFile::modeRead);///打开节目 文件
//		if (iRet == FALSE)
//		{
//			AfxMessageBox("文件读取失败！");
//			return -2;
//		}
//		///序列化提取内容
//		CArchive ar(&file, CArchive::load);///提取内容
//		Serialize(ar);
//		ar.Close();
//		file.Close();
//		
//
//
//		int iCfgQuence = g_Config.g_rigion[0].scene_vec.size();
//
//		for (int iCount = 0; !m_bExit && iCount < iCfgQuence; iCount++)///循环节目中所有场景
//		{
//			SCENE* temp_vec = g_Config.g_rigion[0].scene_vec[iCount];
//			
//			//if (iCount == 0) {
//			//	//如果是第一帧，则发送一帧作为传递 声控状态 和 声控类型
//			//	sendStr = "";
//			//	for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
//			//	{
//			//		sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select == true ? 0xFF : 0x00);
//			//	}
//			//	sendStr.AppendFormat("%02X ", 0xEF);///结束标识 为EF，所有节目的所有数据发送完毕
//			//	sendStr.AppendFormat("%02X ", 0x00);///发送Y值
//			//	sendStr.AppendFormat("%02X ", 0x00);///发送N值
//			//	sendStr.AppendFormat("%02X ", iCfgQuence);///场景数
//			//	sendStr.AppendFormat("%02X ", 0x00);
//			//	sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].channel_type);
//			//	for (int i = 0; i < 10; i++)
//			//	{
//			//		sendStr.AppendFormat("%02X ", 0);
//			//	}
//			//	retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
//			//	if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
//			//		AfxMessageBox("返回值错误！");
//			//	}
//			//}
//
//			sendStr = "";
//			for (int i = 0; i < MSG_TOP_COUNT; i++)
//			{
//				sendStr.AppendFormat("%02X ", MSG_READY_RECORD);
//			}
//			flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
//			if (flag < 0)break;
//			int iIndex = 0;
//			sendStr = "";
//			//发送数据主体
//			for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
//			{
//				sendStr.AppendFormat("%02X ", temp_vec->channelValue[i]);
//			}
//	
//			if (iCount == iCfgQuence - 1) ///如果发送为最后一针，就加上结束标识
//			{
//				TRACE("720\n");
//				///如果是最后一个节目，就另外处理　结束标识为ＣＣ
//				if (cNum == record_vec.size()-1)
//				{
//					TRACE("724\n");
//					sendStr.AppendFormat("%02X ", 0xCC);///结束标识 为CC，所有节目的所有数据发送完毕
//					sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///发送Y值
//					sendStr.AppendFormat("%02X ", temp_vec->waitTime);///发送N值
//					sendStr.AppendFormat("%02X ", cNum);///节目编号
//					sendStr.AppendFormat("%02X ", iCount);
//					sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
//					for (int i = 0; i < 10; i++)
//					{
//						sendStr.AppendFormat("%02X ", 0);
//					}
//					retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
//					if (retValue.Compare("5D 5D 5D 5D 5D ") != 0) {
//						AfxMessageBox("返回值错误！");
//					}
//					AfxMessageBox("一键录制成功！");
//					return 0;
//				}
//				else
//				{
//					sendStr.AppendFormat("%02X ", 0xBB);///结束标识 为CC，所有节目的所有数据发送完毕
//					sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///发送Y值
//					sendStr.AppendFormat("%02X ", temp_vec->waitTime);///发送N值
//					sendStr.AppendFormat("%02X ", cNum);///节目编号
//					sendStr.AppendFormat("%02X ", iCount);
//					sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
//					for (int i = 0; i < 10; i++)
//					{
//						sendStr.AppendFormat("%02X ", 0);
//					}
//					retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
//					if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
//						AfxMessageBox("返回值错误！");
//						return -1;
//					}
//				}
//			}
//			else///不是最后一针，
//			{
//				sendStr.AppendFormat("%02X ", 0xAA);///结束标识 为CC，所有节目的所有数据发送完毕
//				sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///发送Y值
//				sendStr.AppendFormat("%02X ", temp_vec->waitTime);///发送N值
//				sendStr.AppendFormat("%02X ", cNum);///节目编号
//				sendStr.AppendFormat("%02X ", iCount);
//				sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
//				for (int i = 0; i < 10; i++)
//				{
//					sendStr.AppendFormat("%02X ", 0);
//				}			
//				retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
//				if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
//					AfxMessageBox("返回值错误！");
//					return -1;
//				}
//			}			
//		}
//	}
//	return TRUE;
//}



///发送的数据
void CPartProRecordDlg::SendRecordData(void)
{
	Sleep(1000);
	////查找是不是同名
	TCHAR szPath[MAX_PATH] = { 0 };
	if (!GetCurrentDirectory(MAX_PATH, szPath))///获取当前执行程序的路径，不包括程序名
	{
		if (m_bIsProcessStart)///加载文件超时处理
		{
			m_Proc.PostMessage(WM_CLOSE, 0, 0);///关闭进度条
		}
		//AfxMessageBox(StringArray[20]);
		return;
	}
		
	CString sendStr = "";
	for (int i = 0; i < MSG_TOP_COUNT; i++)
	{
		sendStr.AppendFormat("%02X ", MSG_TOP_RECORD_VALUE);
	}
	CString retValue;
	retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
	if (retValue.Compare("5B 5B 5B 5B 5B ")!=0) {
		AfxMessageBox("返回值错误！");
		
		return ;
	}
	
	CString curPath = get_current_path();
	CString strPath;
	CString strTenToSixteen;
	int iTVSize = 0;
	int cNum = 0;///节目编号
	int flag;

	for (UINT cNum = 0; !m_bExit && cNum < record_vec.size(); cNum++, iTVSize++)
	{
		strPath.Format(_T("%sProgram file//%s.ppl"), curPath, record_vec[cNum]);///得到打开Program file的路径
		CFile file;
		int iRet = file.Open(strPath, CFile::modeRead);///打开节目 文件
		if (iRet == FALSE)
		{
			Sleep(200);
			if (m_bIsProcessStart)///加载文件超时处理
			{
				m_Proc.PostMessage(WM_CLOSE, 0, 0);///关闭进度条
			}
			AfxMessageBox("文件读取失败！");
			return;
		}
		///序列化提取内容
		CArchive ar(&file, CArchive::load);///提取内容
		Serialize(ar);
		ar.Close();
		file.Close();


		int iCfgQuence = g_Config.g_rigion[0].scene_vec.size();///当前节目有多少个场景
		BOOL iSendFirstData = 0;
		for (int iCount = 0; !m_bExit && iCount < iCfgQuence; iCount++)///循环节目中所有场景
		{
			SCENE* temp_vec = g_Config.g_rigion[0].scene_vec[iCount];
			sendStr = "";
			for (int i = 0; i < MSG_TOP_COUNT; i++)
			{
				sendStr.AppendFormat("%02X ", MSG_READY_RECORD);
			}
			flag = CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
			if (flag < 0)break;
			else
				m_bIsFinishOneData = TRUE;
			sendStr = "";
			if (m_bIsFinishOneData)////是否完成一次数据发送的标识　一开始就发送了消息头，要有返回才真正发送数据
			{			
				int iIndex = 0;			
				if (iSendFirstData == 0) {	//第一帧发送 
					//如果是第一帧，则发送一帧作为传递 声控状态 和 声控类型
					for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
					{
						sendStr.AppendFormat("%02X ", temp_vec->voiceCheck[i] == true ? 0xFF : 0x00);
					}
					sendStr.AppendFormat("%02X ", 0xEF);///结束标识 为EF，所有节目的所有数据发送完毕
					sendStr.AppendFormat("%02X ", 0x00);///发送Y值
					sendStr.AppendFormat("%02X ", 0x00);///发送N值
					sendStr.AppendFormat("%02X ", iCfgQuence);///场景数
					sendStr.AppendFormat("%02X ", 0x00);
					sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select*16+g_Config.g_rigion[0].channel_type);
					for (int i = 0; i < 10; i++)
					{
						sendStr.AppendFormat("%02X ", 0);
					}
					retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
					if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
						AfxMessageBox("返回值错误！");
					}
					//重新开始跑第一帧
					iSendFirstData = 1;
					iCount = -1;
					m_bIsFinishOneData = FALSE;
					continue;
				}
				else {
					for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
					{
						sendStr.AppendFormat("%02X ", temp_vec->channelValue[i]);
					}
				}

				if (iCount == iCfgQuence - 1) ///如果发送为最后一针，就加上结束标识
				{
					TRACE("720\n");
					///如果是最后一个节目，就另外处理　结束标识为ＣＣ
					if (cNum == record_vec.size()-1)
					{
						TRACE("724\n");
						sendStr.AppendFormat("%02X ", 0xCC);///结束标识 为CC，所有节目的所有数据发送完毕
						sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///发送Y值
						sendStr.AppendFormat("%02X ", temp_vec->waitTime);///发送N值
						sendStr.AppendFormat("%02X ", cNum);///节目编号
						sendStr.AppendFormat("%02X ", iCount);
						sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
						for (int i = 0; i < 8; i++)
						{
							sendStr.AppendFormat("%02X ", temp_vec->voiceCheck[i]);
						}
						sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
						sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
						retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
						if (retValue.Compare("5D 5D 5D 5D 5D ") != 0) {
							AfxMessageBox("返回值错误！");
						}
						AfxMessageBox("一键录制成功！");
						return ;
					}
					else
					{
						sendStr.AppendFormat("%02X ", 0xBB);///结束标识 为，一个节目的所有数据发送完毕
						sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///发送Y值
						sendStr.AppendFormat("%02X ", temp_vec->waitTime);///发送N值
						sendStr.AppendFormat("%02X ", cNum);///节目编号
						sendStr.AppendFormat("%02X ", iCount);
						sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
						for (int i = 0; i < 8; i++)
						{
							sendStr.AppendFormat("%02X ", temp_vec->voiceCheck[i]);
						}
						sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
						sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
						retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
						if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
							AfxMessageBox("返回值错误！");
							return ;
						}
					}
				}
				else///不是最后一针，
				{
					sendStr.AppendFormat("%02X ", 0xAA);///结束标识 为，一个场景发送完毕
					sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///发送Y值
					sendStr.AppendFormat("%02X ", temp_vec->waitTime);///发送N值
					sendStr.AppendFormat("%02X ", cNum);///节目编号
					sendStr.AppendFormat("%02X ", iCount);
					sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
					for (int i = 0; i < 8; i++)
					{
						sendStr.AppendFormat("%02X ", temp_vec->voiceCheck[i]);
					}
					sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
					sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
					retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
					if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
						AfxMessageBox("返回值错误！");
						return ;
					}
				}
				m_bIsFinishOneData = FALSE;
				//mu.unlock();
			}
			else
			{
				for (int j = 0; !m_bExit && j < 25; j++)
				{
					///判断是否刚开始录制的消息头命令，
					if (m_iIsFirstSend)
					{
						for (int k = 0; !m_bExit && k < 1000; k++)
						{
							Sleep(40);
							if (!m_iIsFirstSend) break;
							if (j == 2)
							{
								if (m_bIsProcessStart)///加载文件超时处理
								{
									m_Proc.PostMessage(WM_CLOSE, 0, 0);///关闭进度条
								}
								sendStr = "";
								for (int i = 0; i < MSG_TOP_COUNT; i++)
								{
									sendStr.AppendFormat("%02X ", MSG_RECORD_FAILED);
								}
								CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
								AfxMessageBox("录制失败！");
								return;
							}
							continue;
						}
					}
					////每发送完一针的标识，返回超时处理
					if (!m_bIsFinishOneData)
					{
						//boost::mutex mu;////加一把互拆锁
						//mu.lock();
						Sleep(50);
						if (j == 24)
						{
							if (m_bIsProcessStart)///加载文件超时处理
							{
								m_Proc.PostMessage(WM_CLOSE, 0, 0);///关闭进度条
							}
							sendStr = "";
							for (int i = 0; i < MSG_TOP_COUNT; i++)
							{
								sendStr.AppendFormat("%02X ", MSG_RECORD_FAILED);
							}
							CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
							AfxMessageBox("录制失败！");
							return;
						}
						//mu.unlock();
					}
					else
					{
						iCount--;///节目顺序减一
						break;
					}
				}
			}
		}
	}
}

///返回所有节目录制成功处理
void CPartProRecordDlg::RecordAllSuccess(void)
{
	//if(g_bIsProcessStart)///加载文件超时处理
	//{
	//	if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//	{
	//		g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//	}
	//	m_bCfgStat =TRUE;
	//	m_Proc.PostMessage(WM_CLOSE,0,0);///关闭进度条
	//}
}
///突然中止函数
BOOL CPartProRecordDlg::IsRecordBreakOff(void)
{
	//if(m_bRecordStop)
	//{
	//	TRACE("905,IsRecordBreakOff\n");
	//	boost::mutex mu;////加一把互拆锁
	//	mu.lock();
	//	for(int i=0;i<MSG_TOP_COUNT-1;i++)
	//	{
	//		msg_Top[i]=MSG_RECORD_FAILED; ////如果中途退出，就发失败ID（5E）
	//	}
	//	g_zigbee_device.WriteDataUnPrt(msg_Top,MSG_TOP_COUNT-1);///发送5个字节的5E
	//	mu.unlock();
	//	g_uExcFlag =EXC_NULL;///标识为空
	//	if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//	{
	//		g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//	}
	//	TRACE("919,IsRecordBreakOff\n");
	//	return TRUE;
	//}
	return FALSE;
}

//清空节目文件
void CPartProRecordDlg::OnBnClickedCleanFestival()
{
	CString szPath,strFileFind,strFilePath;
	strFilePath.Format(_T("%s//Program file//*.*"),szPath);
	CFileFind tempFind; 
	BOOL IsFinded = tempFind.FindFile(strFilePath); 
	BOOL bRet =FALSE;
	while (IsFinded) 
	{ 
		IsFinded = tempFind.FindNextFile(); 
		CString strFileName;
		
		if(!tempFind.IsDots()) 
		{
			strFileName.Format(_T("%s//Program file//%s"),szPath,tempFind.GetFileName().GetBuffer());
			if (tempFind.IsDirectory()) //文件夹
			{ 
			} 
			else //文件
			{   
				 bRet =TRUE;
				DeleteFile(strFileName);
			} 
		} 
	}
	 tempFind.Close(); 
	
}


void CPartProRecordDlg::OnBnClickedCleanSelect()
{
	for (int i = 0; i < RECORD_COUNT; i++)
	{
		g_Config.g_record_tpye[i] = "";
	}
	UpdateReocrdReport();
}


void CPartProRecordDlg::OnBnClickedLoadIn()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"INI配置文件t(*.ini)|*.ini||",AfxGetMainWnd());
	file.m_ofn.lpstrInitialDir=_T(".\\");

	if(file.DoModal()==IDOK)
	{
		CString str=file.GetPathName();
		///先解密
		 CAddPasserword decode;
		 if(FALSE ==decode.dcfile(str))
		 {
			 AfxMessageBox("文件没有加密，请先加密！");
			 ////显示
			UpdateReocrdReport();///插入10行显示
			m_Record_Report->Invalidate();
			 return;
		 }
		// g_bIsProcessStart =TRUE; ///进度条标识为真
		/////创建进度条
		// if(IDCANCEL ==m_Proc.Create("sldk"))////退出进度条就另外处理
		//{
		//	g_bIsProcessStart =FALSE;///进度条标识为假，即不存在
		//}

		CString strPathIni,strItem ;
		char chData[2048];
		TCHAR strValueName[256];

		strPathIni= str;
		strItem = _T("ModeCfg");

		_stprintf_s(strValueName, sizeof(strValueName) / sizeof(strValueName[0]), _T("DMX512"));
		GetPrivateProfileString(strItem,strValueName,_T(""),chData,100,strPathIni);
		CString SceneTypeTotal = chData;

		//(=DMX512,最多8套节目)
		//DMX512=1:K歌,2:动感,3:浪漫,4:慢摇,5:柔和,6:商务,7:抒情,8:明亮,
		int x,y;
		CString xStr,yStr;
		for(int i=0;i<8;i++){
			xStr.Format(_T("%d:"),i+1);
			yStr.Format(_T("%d:"),i+2);
			x = SceneTypeTotal.Find(xStr);	
			y = SceneTypeTotal.Find(yStr);
			g_Config.g_record_tpye[i] = SceneTypeTotal.Mid(x+2,y-x-3);
			if(i==7){
				y = SceneTypeTotal.GetLength();
				g_Config.g_record_tpye[i] = SceneTypeTotal.Mid(x+2,y-x-3);
			}
		}
		////显示
		UpdateReocrdReport();///插入10行显示
		m_Record_Report->Invalidate();
		//m_Proc.PostMessage(WM_CLOSE,0,0);///关闭进度条
		decode.ecfile(str);
	}
}



void CPartProRecordDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_Config.save("./param.ppl");
	CDialogEx::OnClose();
}


void CPartProRecordDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgPort::getContext()->ShowWindow(SW_SHOW);
}


UINT threadSendData(LPVOID p)
{
	CPartProRecordDlg* pRecord = (CPartProRecordDlg*)p;
	pRecord->SendRecordData();
	if (pRecord->m_bIsProcessStart)///加载文件超时处理
	{
		pRecord->m_Proc.PostMessage(WM_CLOSE, 0, 0);///关闭进度条
	}
	return 0;
}



////创建进度条
void CPartProRecordDlg::CreateProDlg()
{
	m_bIsProcessStart = TRUE; ///进度条标识为真
	if (IDCANCEL == m_Proc.DoModal())////退出进度条就另外处理
	{
		m_bIsProcessStart = FALSE;///进度条标识为假，即不存在
		m_bExit = TRUE; ///中止录制标识为真
	}
}

