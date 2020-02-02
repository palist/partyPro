
// PartProRecordDlg.cpp : ʵ���ļ�
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
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // ɾ���ļ�����ֻ���·���ִ�
	CString szPath = szFilePath;
	return szPath;
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CPartProRecordDlg �Ի���




CPartProRecordDlg::CPartProRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPartProRecordDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_Record_Report = new CSuperGridCtrl();
	//ͨѶ����
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


// CPartProRecordDlg ��Ϣ�������

BOOL CPartProRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitXTPRecordReport();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPartProRecordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPartProRecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


///��ʼ���б�
BOOL CPartProRecordDlg::InitXTPRecordReport(void)
{
	
	m_Record_Report->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 100, 100), this, 0x1003);
	m_Record_Report->SetExtendedStyle(LVS_EX_GRIDLINES/*| LVS_EX_CHECKBOXES*/);


	int nCount = m_Record_Report->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_Record_Report->DeleteColumn(0);
	}
	m_Record_Report->InsertColumn(0, _T("���"), LVCFMT_LEFT, 80);
	m_Record_Report->InsertColumn(1, _T("��Ŀ����"), LVCFMT_LEFT, 100);
	m_Record_Report->InsertColumn(2, _T("�����Ŀ"), LVCFMT_LEFT, 100);
	m_Record_Report->InsertColumn(3, _T("����������Ŀ"), LVCFMT_LEFT, 100);

	CRect grid_rc1;
	GetDlgItem(IDC_STATIC_LIST)->GetWindowRect(grid_rc1);
	ScreenToClient(grid_rc1);
	grid_rc1.top += 5;
	grid_rc1.bottom -= 5;
	m_Record_Report->MoveWindow(&grid_rc1);

	m_Record_Report->Invalidate();

	////��ʾ
	UpdateReocrdReport();///����10����ʾ

	UpdateTVNameList();
	
	return TRUE;
}



///��ʾ10����ʾ
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

		info->AddSubItemText("��ѡ��");

		info->AddSubItemText("��ѡ��");

		m_Record_Report->InsertRootItem(info);
	}

	m_Record_Report->Invalidate();
	
}


//˫��ʱ�����޸�����
LRESULT CPartProRecordDlg::OnNMDblclkList(WPARAM wParam, LPARAM lParam)
{
	m_iSubItem = (int)wParam;
	m_iItem = (int)lParam;

	if (m_iItem == -1)
		return 0;
	//��ȡ�б��ϵ��ı�����ʾ��Edit�ؼ���
	CString itemTxt;;

	//����Edit�Ŀ��ߣ����ƶ�
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

		//�޸�
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



////�����Ŀ������
void CPartProRecordDlg::UpdateTVNameList(void)
{
	 ////�����ǲ���ͬ��
	CString szPath = get_current_path();
	CFileFind finder;
	CString strPath,strFileName,strTVName,strNum;
	int iCount=0;
	strPath.Format(_T("%s//Program file//*.ppl"),szPath);////���ļ���·��
	BOOL bWorking = finder.FindFile(strPath);////��������Program file
	if(!bWorking)
	{
		//AfxMessageBox(StringArray[43]);
		return;
	}
	m_combo_normal.Clear();
	m_combo_serior.Clear();
	///��ʼѭ������Program file
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strFileName = finder.GetFileName();
		int iPos = strFileName.Find(_T("."));
		if (iPos == -1)continue;
		//////�������н�Ŀ���ƣ������ϱ��
		//strNum.Format(_T("%d"), iCount++);
		m_combo_normal.AddString(strFileName.Left(iPos));
		m_combo_serior.AddString(strFileName.Left(iPos));
	}
}


///һ��¼�ư�ť
void CPartProRecordDlg::OnBnClickedBtnRecordOneKey()
{
	record_vec.clear();
	for (UINT cNum = 0; cNum < m_Record_Report->GetCount(); cNum++)
	{
		CString Record_name = m_Record_Report->GetItemText(cNum, 2);
		if (Record_name.Compare("��ѡ��") == 0)break;
		record_vec.push_back(Record_name);
	}
	for (UINT cNum = 0; cNum < m_Record_Report->GetCount(); cNum++)
	{
		CString Record_name = m_Record_Report->GetItemText(cNum, 3);
		if (Record_name.Compare("��ѡ��") == 0)break;
		record_vec.push_back(Record_name);
	}

	m_bExit = FALSE;
	if (CDlgPort::getContext()->m_curData->m_isOpen==false)
	{
		AfxMessageBox("����δ���ӣ�");
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



///��д���л�����
void CPartProRecordDlg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())///��� �Ǵ�������
	{
	}
	else///����Ǽ�������
	{
		long long sizeCount = 0;
		ar >> sizeCount;
		if (sizeCount != sizeof(CSystemConfig))
		{
			AfxMessageBox("�������ļ�Ϊ��������ɣ���ɾ��ԭ���ò��������������ļ���");
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
					ar >> pData->channelValue[j];	//ͨ��ֵ
					ar >> pData->voiceCheck[j];		//��������
				}

				ar >> pData->waitTime;	//�ȴ�ʱ��
				ar >> pData->shapeTime;	//����ʱ��
				ar >> pData->time;		//ʱ���

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


/////���͵�����
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
//		AfxMessageBox("����ֵ����");
//		return -1;
//	}
//
//
//	BOOL iSendFirstData = 0;
//	int flag = 0;
//
//	for (int cNum =0; !m_bExit&&cNum < record_vec.size(); cNum++)
//	{
//		strPath.Format(_T("%sProgram file//%s.ppl"), curPath, record_vec[cNum]);///�õ���Program file��·��
//		CFile file;
//		int iRet = file.Open(strPath, CFile::modeRead);///�򿪽�Ŀ �ļ�
//		if (iRet == FALSE)
//		{
//			AfxMessageBox("�ļ���ȡʧ�ܣ�");
//			return -2;
//		}
//		///���л���ȡ����
//		CArchive ar(&file, CArchive::load);///��ȡ����
//		Serialize(ar);
//		ar.Close();
//		file.Close();
//		
//
//
//		int iCfgQuence = g_Config.g_rigion[0].scene_vec.size();
//
//		for (int iCount = 0; !m_bExit && iCount < iCfgQuence; iCount++)///ѭ����Ŀ�����г���
//		{
//			SCENE* temp_vec = g_Config.g_rigion[0].scene_vec[iCount];
//			
//			//if (iCount == 0) {
//			//	//����ǵ�һ֡������һ֡��Ϊ���� ����״̬ �� ��������
//			//	sendStr = "";
//			//	for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
//			//	{
//			//		sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select == true ? 0xFF : 0x00);
//			//	}
//			//	sendStr.AppendFormat("%02X ", 0xEF);///������ʶ ΪEF�����н�Ŀ���������ݷ������
//			//	sendStr.AppendFormat("%02X ", 0x00);///����Yֵ
//			//	sendStr.AppendFormat("%02X ", 0x00);///����Nֵ
//			//	sendStr.AppendFormat("%02X ", iCfgQuence);///������
//			//	sendStr.AppendFormat("%02X ", 0x00);
//			//	sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].channel_type);
//			//	for (int i = 0; i < 10; i++)
//			//	{
//			//		sendStr.AppendFormat("%02X ", 0);
//			//	}
//			//	retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
//			//	if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
//			//		AfxMessageBox("����ֵ����");
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
//			//������������
//			for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
//			{
//				sendStr.AppendFormat("%02X ", temp_vec->channelValue[i]);
//			}
//	
//			if (iCount == iCfgQuence - 1) ///�������Ϊ���һ�룬�ͼ��Ͻ�����ʶ
//			{
//				TRACE("720\n");
//				///��������һ����Ŀ�������⴦��������ʶΪ�ã�
//				if (cNum == record_vec.size()-1)
//				{
//					TRACE("724\n");
//					sendStr.AppendFormat("%02X ", 0xCC);///������ʶ ΪCC�����н�Ŀ���������ݷ������
//					sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///����Yֵ
//					sendStr.AppendFormat("%02X ", temp_vec->waitTime);///����Nֵ
//					sendStr.AppendFormat("%02X ", cNum);///��Ŀ���
//					sendStr.AppendFormat("%02X ", iCount);
//					sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
//					for (int i = 0; i < 10; i++)
//					{
//						sendStr.AppendFormat("%02X ", 0);
//					}
//					retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
//					if (retValue.Compare("5D 5D 5D 5D 5D ") != 0) {
//						AfxMessageBox("����ֵ����");
//					}
//					AfxMessageBox("һ��¼�Ƴɹ���");
//					return 0;
//				}
//				else
//				{
//					sendStr.AppendFormat("%02X ", 0xBB);///������ʶ ΪCC�����н�Ŀ���������ݷ������
//					sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///����Yֵ
//					sendStr.AppendFormat("%02X ", temp_vec->waitTime);///����Nֵ
//					sendStr.AppendFormat("%02X ", cNum);///��Ŀ���
//					sendStr.AppendFormat("%02X ", iCount);
//					sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
//					for (int i = 0; i < 10; i++)
//					{
//						sendStr.AppendFormat("%02X ", 0);
//					}
//					retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
//					if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
//						AfxMessageBox("����ֵ����");
//						return -1;
//					}
//				}
//			}
//			else///�������һ�룬
//			{
//				sendStr.AppendFormat("%02X ", 0xAA);///������ʶ ΪCC�����н�Ŀ���������ݷ������
//				sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///����Yֵ
//				sendStr.AppendFormat("%02X ", temp_vec->waitTime);///����Nֵ
//				sendStr.AppendFormat("%02X ", cNum);///��Ŀ���
//				sendStr.AppendFormat("%02X ", iCount);
//				sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select);
//				for (int i = 0; i < 10; i++)
//				{
//					sendStr.AppendFormat("%02X ", 0);
//				}			
//				retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
//				if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
//					AfxMessageBox("����ֵ����");
//					return -1;
//				}
//			}			
//		}
//	}
//	return TRUE;
//}



///���͵�����
void CPartProRecordDlg::SendRecordData(void)
{
	Sleep(1000);
	////�����ǲ���ͬ��
	TCHAR szPath[MAX_PATH] = { 0 };
	if (!GetCurrentDirectory(MAX_PATH, szPath))///��ȡ��ǰִ�г����·����������������
	{
		if (m_bIsProcessStart)///�����ļ���ʱ����
		{
			m_Proc.PostMessage(WM_CLOSE, 0, 0);///�رս�����
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
		AfxMessageBox("����ֵ����");
		
		return ;
	}
	
	CString curPath = get_current_path();
	CString strPath;
	CString strTenToSixteen;
	int iTVSize = 0;
	int cNum = 0;///��Ŀ���
	int flag;

	for (UINT cNum = 0; !m_bExit && cNum < record_vec.size(); cNum++, iTVSize++)
	{
		strPath.Format(_T("%sProgram file//%s.ppl"), curPath, record_vec[cNum]);///�õ���Program file��·��
		CFile file;
		int iRet = file.Open(strPath, CFile::modeRead);///�򿪽�Ŀ �ļ�
		if (iRet == FALSE)
		{
			Sleep(200);
			if (m_bIsProcessStart)///�����ļ���ʱ����
			{
				m_Proc.PostMessage(WM_CLOSE, 0, 0);///�رս�����
			}
			AfxMessageBox("�ļ���ȡʧ�ܣ�");
			return;
		}
		///���л���ȡ����
		CArchive ar(&file, CArchive::load);///��ȡ����
		Serialize(ar);
		ar.Close();
		file.Close();


		int iCfgQuence = g_Config.g_rigion[0].scene_vec.size();///��ǰ��Ŀ�ж��ٸ�����
		BOOL iSendFirstData = 0;
		for (int iCount = 0; !m_bExit && iCount < iCfgQuence; iCount++)///ѭ����Ŀ�����г���
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
			if (m_bIsFinishOneData)////�Ƿ����һ�����ݷ��͵ı�ʶ��һ��ʼ�ͷ�������Ϣͷ��Ҫ�з��ز�������������
			{			
				int iIndex = 0;			
				if (iSendFirstData == 0) {	//��һ֡���� 
					//����ǵ�һ֡������һ֡��Ϊ���� ����״̬ �� ��������
					for (int i = 0; i < 240/*CHANNEL_COUNT*/; i++)
					{
						sendStr.AppendFormat("%02X ", temp_vec->voiceCheck[i] == true ? 0xFF : 0x00);
					}
					sendStr.AppendFormat("%02X ", 0xEF);///������ʶ ΪEF�����н�Ŀ���������ݷ������
					sendStr.AppendFormat("%02X ", 0x00);///����Yֵ
					sendStr.AppendFormat("%02X ", 0x00);///����Nֵ
					sendStr.AppendFormat("%02X ", iCfgQuence);///������
					sendStr.AppendFormat("%02X ", 0x00);
					sendStr.AppendFormat("%02X ", g_Config.g_rigion[0].control_select*16+g_Config.g_rigion[0].channel_type);
					for (int i = 0; i < 10; i++)
					{
						sendStr.AppendFormat("%02X ", 0);
					}
					retValue = CDlgPort::getContext()->m_curData->sendPort(sendStr);
					if (retValue.Compare("5B 5B 5B 5B 5B ") != 0) {
						AfxMessageBox("����ֵ����");
					}
					//���¿�ʼ�ܵ�һ֡
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

				if (iCount == iCfgQuence - 1) ///�������Ϊ���һ�룬�ͼ��Ͻ�����ʶ
				{
					TRACE("720\n");
					///��������һ����Ŀ�������⴦��������ʶΪ�ã�
					if (cNum == record_vec.size()-1)
					{
						TRACE("724\n");
						sendStr.AppendFormat("%02X ", 0xCC);///������ʶ ΪCC�����н�Ŀ���������ݷ������
						sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///����Yֵ
						sendStr.AppendFormat("%02X ", temp_vec->waitTime);///����Nֵ
						sendStr.AppendFormat("%02X ", cNum);///��Ŀ���
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
							AfxMessageBox("����ֵ����");
						}
						AfxMessageBox("һ��¼�Ƴɹ���");
						return ;
					}
					else
					{
						sendStr.AppendFormat("%02X ", 0xBB);///������ʶ Ϊ��һ����Ŀ���������ݷ������
						sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///����Yֵ
						sendStr.AppendFormat("%02X ", temp_vec->waitTime);///����Nֵ
						sendStr.AppendFormat("%02X ", cNum);///��Ŀ���
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
							AfxMessageBox("����ֵ����");
							return ;
						}
					}
				}
				else///�������һ�룬
				{
					sendStr.AppendFormat("%02X ", 0xAA);///������ʶ Ϊ��һ�������������
					sendStr.AppendFormat("%02X ", temp_vec->shapeTime);///����Yֵ
					sendStr.AppendFormat("%02X ", temp_vec->waitTime);///����Nֵ
					sendStr.AppendFormat("%02X ", cNum);///��Ŀ���
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
						AfxMessageBox("����ֵ����");
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
					///�ж��Ƿ�տ�ʼ¼�Ƶ���Ϣͷ���
					if (m_iIsFirstSend)
					{
						for (int k = 0; !m_bExit && k < 1000; k++)
						{
							Sleep(40);
							if (!m_iIsFirstSend) break;
							if (j == 2)
							{
								if (m_bIsProcessStart)///�����ļ���ʱ����
								{
									m_Proc.PostMessage(WM_CLOSE, 0, 0);///�رս�����
								}
								sendStr = "";
								for (int i = 0; i < MSG_TOP_COUNT; i++)
								{
									sendStr.AppendFormat("%02X ", MSG_RECORD_FAILED);
								}
								CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
								AfxMessageBox("¼��ʧ�ܣ�");
								return;
							}
							continue;
						}
					}
					////ÿ������һ��ı�ʶ�����س�ʱ����
					if (!m_bIsFinishOneData)
					{
						//boost::mutex mu;////��һ�ѻ�����
						//mu.lock();
						Sleep(50);
						if (j == 24)
						{
							if (m_bIsProcessStart)///�����ļ���ʱ����
							{
								m_Proc.PostMessage(WM_CLOSE, 0, 0);///�رս�����
							}
							sendStr = "";
							for (int i = 0; i < MSG_TOP_COUNT; i++)
							{
								sendStr.AppendFormat("%02X ", MSG_RECORD_FAILED);
							}
							CDlgPort::getContext()->m_curData->sendPortOnly(sendStr);
							AfxMessageBox("¼��ʧ�ܣ�");
							return;
						}
						//mu.unlock();
					}
					else
					{
						iCount--;///��Ŀ˳���һ
						break;
					}
				}
			}
		}
	}
}

///�������н�Ŀ¼�Ƴɹ�����
void CPartProRecordDlg::RecordAllSuccess(void)
{
	//if(g_bIsProcessStart)///�����ļ���ʱ����
	//{
	//	if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//	{
	//		g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//	}
	//	m_bCfgStat =TRUE;
	//	m_Proc.PostMessage(WM_CLOSE,0,0);///�رս�����
	//}
}
///ͻȻ��ֹ����
BOOL CPartProRecordDlg::IsRecordBreakOff(void)
{
	//if(m_bRecordStop)
	//{
	//	TRACE("905,IsRecordBreakOff\n");
	//	boost::mutex mu;////��һ�ѻ�����
	//	mu.lock();
	//	for(int i=0;i<MSG_TOP_COUNT-1;i++)
	//	{
	//		msg_Top[i]=MSG_RECORD_FAILED; ////�����;�˳����ͷ�ʧ��ID��5E��
	//	}
	//	g_zigbee_device.WriteDataUnPrt(msg_Top,MSG_TOP_COUNT-1);///����5���ֽڵ�5E
	//	mu.unlock();
	//	g_uExcFlag =EXC_NULL;///��ʶΪ��
	//	if(g_ZgbSetDoc.m_arryCfgFile.GetCount()>0)
	//	{
	//		g_ZgbSetDoc.m_arryCfgFile.RemoveAll();
	//	}
	//	TRACE("919,IsRecordBreakOff\n");
	//	return TRUE;
	//}
	return FALSE;
}

//��ս�Ŀ�ļ�
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
			if (tempFind.IsDirectory()) //�ļ���
			{ 
			} 
			else //�ļ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"INI�����ļ�t(*.ini)|*.ini||",AfxGetMainWnd());
	file.m_ofn.lpstrInitialDir=_T(".\\");

	if(file.DoModal()==IDOK)
	{
		CString str=file.GetPathName();
		///�Ƚ���
		 CAddPasserword decode;
		 if(FALSE ==decode.dcfile(str))
		 {
			 AfxMessageBox("�ļ�û�м��ܣ����ȼ��ܣ�");
			 ////��ʾ
			UpdateReocrdReport();///����10����ʾ
			m_Record_Report->Invalidate();
			 return;
		 }
		// g_bIsProcessStart =TRUE; ///��������ʶΪ��
		/////����������
		// if(IDCANCEL ==m_Proc.Create("sldk"))////�˳������������⴦��
		//{
		//	g_bIsProcessStart =FALSE;///��������ʶΪ�٣���������
		//}

		CString strPathIni,strItem ;
		char chData[2048];
		TCHAR strValueName[256];

		strPathIni= str;
		strItem = _T("ModeCfg");

		_stprintf_s(strValueName, sizeof(strValueName) / sizeof(strValueName[0]), _T("DMX512"));
		GetPrivateProfileString(strItem,strValueName,_T(""),chData,100,strPathIni);
		CString SceneTypeTotal = chData;

		//(=DMX512,���8�׽�Ŀ)
		//DMX512=1:K��,2:����,3:����,4:��ҡ,5:���,6:����,7:����,8:����,
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
		////��ʾ
		UpdateReocrdReport();///����10����ʾ
		m_Record_Report->Invalidate();
		//m_Proc.PostMessage(WM_CLOSE,0,0);///�رս�����
		decode.ecfile(str);
	}
}



void CPartProRecordDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	g_Config.save("./param.ppl");
	CDialogEx::OnClose();
}


void CPartProRecordDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgPort::getContext()->ShowWindow(SW_SHOW);
}


UINT threadSendData(LPVOID p)
{
	CPartProRecordDlg* pRecord = (CPartProRecordDlg*)p;
	pRecord->SendRecordData();
	if (pRecord->m_bIsProcessStart)///�����ļ���ʱ����
	{
		pRecord->m_Proc.PostMessage(WM_CLOSE, 0, 0);///�رս�����
	}
	return 0;
}



////����������
void CPartProRecordDlg::CreateProDlg()
{
	m_bIsProcessStart = TRUE; ///��������ʶΪ��
	if (IDCANCEL == m_Proc.DoModal())////�˳������������⴦��
	{
		m_bIsProcessStart = FALSE;///��������ʶΪ�٣���������
		m_bExit = TRUE; ///��ֹ¼�Ʊ�ʶΪ��
	}
}

