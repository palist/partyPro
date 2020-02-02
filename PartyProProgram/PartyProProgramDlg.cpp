
// PartyProProgramDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PartyProProgram.h"
#include "PartyProProgramDlg.h"
#include "afxdialogex.h"
#include "DlgPort.h"
#include "drawDlg.h"
#include "colorDlg/DIB.h"
#include "Function.h"
#include "AddDeleteLight.h"
#include "SelectLightGroup.h"
#include "ColorPickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern CCriticalSection g_CriticalSection;//定义临界区

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


// CPartyProProgramDlg 对话框


CPartyProProgramDlg::CPartyProProgramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPartyProProgramDlg::IDD, pParent)
	, m_check_loop(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//通讯窗口
	CDlgPort::getContext()->Create(IDD_DLG_MUL_PORT,this);
	CDlgPort::getContext()->ShowWindow(SW_HIDE);
	m_pTreeGrid = new CSuperGridCtrl();
	if (g_Config.g_rigion[g_Config.g_rigion_num].scene_vec.size() > 0)
		m_curData = *g_Config.g_rigion[g_Config.g_rigion_num].scene_vec.begin();
	else
		m_curData = nullptr;

	m_sliderDlg = new CSliderDlg(m_curData);

	m_cutRigion = &g_Config.g_rigion[g_Config.g_rigion_num];
	
	m_iItem = 0;
	bThreadExit = false;

	m_channel_type = m_cutRigion->channel_type;
	m_control_select = m_cutRigion->control_select;

	CdrawDlg::getContext(this)->Create(IDD_DRAW_DIALOG, this);
	CdrawDlg::getContext(this)->ShowWindow(SW_HIDE);
	CColorPickerDlg::getContext(this)->Create(IDD_COLORPICKERDLG_DIALOG, this);
	CColorPickerDlg::getContext(this)->ShowWindow(SW_HIDE);
}



CPartyProProgramDlg::~CPartyProProgramDlg()
{
	delete m_pTreeGrid;
	m_pTreeGrid = nullptr;
	m_curData = nullptr;
	m_cutRigion = nullptr;
	delete m_sliderDlg;
	m_sliderDlg = nullptr;
}



void CPartyProProgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SPLITER_SHOW, m_static_sliterDlg_zone);
	DDX_Radio(pDX, IDC_RADIO1, m_redio_group);
	DDX_Control(pDX, IDC_COMBO_REGION, m_combo_region);
	DDX_CBIndex(pDX, IDC_COMBO_REGION, g_Config.g_rigion_num);
	DDX_Control(pDX, IDC_STATIC_SPLITE, m_static_scene_list_zone);
	DDX_Control(pDX, IDC_EDIT_VAR, m_edit_var);
	DDX_Check(pDX, IDC_CHECK_CHANGE_CHANNEL, g_Config.g_bChangeScene);
	DDX_Control(pDX, IDC_COMBO2, m_combo_WaitTime);
	DDX_Control(pDX, IDC_COMBO3, m_combo_SharpTime);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_check_loop);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_TYPE, m_channel_type);
	DDX_Check(pDX, IDC_CHECK_CONTROL_SELECT, m_control_select);
}

BEGIN_MESSAGE_MAP(CPartyProProgramDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_XY_CONTROL, &CPartyProProgramDlg::OnBnClickedBtnXyControl)
	ON_BN_CLICKED(IDC_BTN_COLOR_CONTROL, &CPartyProProgramDlg::OnBnClickedBtnColorControl)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO16, OnButtonClick)
	ON_CBN_SELCHANGE(IDC_COMBO_REGION, &CPartyProProgramDlg::OnCbnSelchangeComboRegion)
	ON_BN_CLICKED(IDC_BTN_INTER_SCENE, &CPartyProProgramDlg::OnBnClickedBtnInterScene)
	ON_MESSAGE(WM_USER_SELECTED_GRID_ITEM, &CPartyProProgramDlg::OnUserSeletedItem)
	ON_MESSAGE(WM_USER_DBLCLK, &CPartyProProgramDlg::OnNMDblclkList)
	ON_EN_KILLFOCUS(IDC_EDIT_VAR, &CPartyProProgramDlg::OnEnKillfocusGreeitemEdit)
	ON_CBN_KILLFOCUS(IDC_COMBO2, &CPartyProProgramDlg::OnCbnKillfocusComboKnife)
	ON_CBN_KILLFOCUS(IDC_COMBO3, &CPartyProProgramDlg::OnCbnKillfocusComboKnife)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_DELETE_SCENE, &CPartyProProgramDlg::OnBnClickedBtnDeleteScene)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_FLUSH_SCROLL, &CPartyProProgramDlg::OnFlushSceneList)
	//ON_CBN_SELCHANGE(IDC_COMBO2, &CPartyProProgramDlg::OnCbnSelchangeCombo2)
	//ON_CBN_SELCHANGE(IDC_COMBO3, &CPartyProProgramDlg::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &CPartyProProgramDlg::OnBnClickedBtnOpenFile)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CPartyProProgramDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &CPartyProProgramDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_CHECK_LOOP, &CPartyProProgramDlg::OnBnClickedCheckLoop)
	ON_BN_CLICKED(IDC_BTN_RECORD, &CPartyProProgramDlg::OnBnClickedBtnRecord)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CPartyProProgramDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_CHECK_CHANGE_CHANNEL, &CPartyProProgramDlg::OnBnClickedChangeChannelValue)
	ON_BN_CLICKED(IDC_CHECK_CONTROL_SELECT, &CPartyProProgramDlg::OnBnClickedCheckControlSelect)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL_TYPE, &CPartyProProgramDlg::OnCbnSelchangeComboChannelType)
	ON_BN_CLICKED(IDC_BTN_SAVE_FILE, &CPartyProProgramDlg::OnBnClickedBtnSaveFile)
	ON_COMMAND(ID_ADD_DELETE_LIGHT, &CPartyProProgramDlg::OnAddDeleteLight)
	ON_COMMAND(ID_OPEN_LIGHT_LIB, &CPartyProProgramDlg::OnOpenLightLib)
	ON_BN_CLICKED(IDC_BTN_SELECT_LIGHT, &CPartyProProgramDlg::OnBnClickedBtnSelectLight)
	ON_COMMAND(ID_RECORD, &CPartyProProgramDlg::OnRecord)
END_MESSAGE_MAP()


// CPartyProProgramDlg 消息处理程序

int CPartyProProgramDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  在此添加您专用的创建代码
	m_pTreeGrid->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 100, 100), this, 0x1003);
	m_pTreeGrid->SetExtendedStyle(LVS_EX_GRIDLINES/*| LVS_EX_CHECKBOXES*/|!LVS_EX_HEADERDRAGDROP);

	int nCount = m_pTreeGrid->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_pTreeGrid->DeleteColumn(0);
	}
	m_pTreeGrid->InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(1, _T("等待时间"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(2, _T("渐变时间"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(3, _T("时间点"), LVCFMT_LEFT, 60);

	CString str;

	for (int i=0;i<CHANNEL_COUNT;i++)
	{
		str.Format("%d",i+1);
		m_pTreeGrid->InsertColumn(4+i, str, LVCFMT_LEFT, 40);
	}
	//m_pTreeGrid->SetScrollRange(SB_HORZ,0, CHANNEL_COUNT+4);

	m_pTreeGrid->Invalidate();
	UpdateWindow();
	return 0;
}


BOOL CPartyProProgramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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
	CMenu menu;//定义一个菜单类变量
	menu.LoadMenu(IDR_MENU1);//装载IDR_MENU1菜单资源
	SetMenu(&menu);//和当前窗口关联起来
	menu.Detach();//分离

	CRect grid_rc1;
	m_static_sliterDlg_zone.GetWindowRect(grid_rc1);
	ScreenToClient(grid_rc1);
	grid_rc1.top += 5;
	grid_rc1.bottom -= 5;

	m_sliderDlg->Create(CSliderDlg::IDD,this);
	m_sliderDlg->ShowWindow(SW_SHOW);
	m_sliderDlg->MoveWindow(&grid_rc1);
		
	m_redio_group = g_Config.g_redioIniValue;

	CString rigionStr;
	for (int i=0;i<RIGION_COUNT;i++)
	{
		rigionStr.Format("域%d",i+1);
		m_combo_region.AddString(rigionStr);
	}
	m_combo_region.SetCurSel(0);

	CString str;
	m_combo_WaitTime.AddString("0");
	m_combo_WaitTime.AddString("0.03");
	m_combo_WaitTime.AddString("0.05");
	m_combo_WaitTime.AddString("0.07");
	m_combo_WaitTime.AddString("0.09");
	m_combo_WaitTime.AddString("0.1");
	m_combo_WaitTime.AddString("0.3");
	m_combo_WaitTime.AddString("0.5");
	m_combo_WaitTime.AddString("0.7");
	m_combo_WaitTime.AddString("0.9");
	m_combo_WaitTime.AddString("1");
	m_combo_WaitTime.AddString("1.5");
	m_combo_WaitTime.AddString("2");
	m_combo_WaitTime.AddString("2.5");
	m_combo_WaitTime.AddString("3");
	m_combo_WaitTime.AddString("3.5");
	m_combo_WaitTime.AddString("4");
	m_combo_WaitTime.AddString("4.5");
	m_combo_WaitTime.AddString("5");
	m_combo_WaitTime.AddString("6");
	m_combo_WaitTime.AddString("7");
	m_combo_WaitTime.AddString("8");
	m_combo_WaitTime.AddString("9");
	m_combo_WaitTime.AddString("10");
	m_combo_WaitTime.AddString("20");
	m_combo_WaitTime.AddString("30");
	m_combo_WaitTime.AddString("40");
	m_combo_WaitTime.AddString("50");
	m_combo_WaitTime.AddString("60");
	m_combo_WaitTime.AddString("70");
	m_combo_WaitTime.AddString("80");
	m_combo_WaitTime.AddString("90");
	m_combo_WaitTime.AddString("100");
	m_combo_WaitTime.AddString("110");
	m_combo_WaitTime.AddString("200");
	m_combo_WaitTime.AddString("300");

	str.Format("%g",g_Config.g_waitTimeSetValue);
	m_combo_WaitTime.SetWindowTextA(str);

	m_combo_SharpTime.AddString("0");
	m_combo_SharpTime.AddString("0.1");
	m_combo_SharpTime.AddString("0.3");
	m_combo_SharpTime.AddString("0.5");
	m_combo_SharpTime.AddString("0.7");
	m_combo_SharpTime.AddString("0.9");
	m_combo_SharpTime.AddString("1");
	m_combo_SharpTime.AddString("1.5");
	m_combo_SharpTime.AddString("2");
	m_combo_SharpTime.AddString("2.5");
	m_combo_SharpTime.AddString("3");
	m_combo_SharpTime.AddString("3.5");
	m_combo_SharpTime.AddString("4");
	m_combo_SharpTime.AddString("4.5");
	m_combo_SharpTime.AddString("5");
	m_combo_SharpTime.AddString("6");
	m_combo_SharpTime.AddString("7");
	m_combo_SharpTime.AddString("8");
	m_combo_SharpTime.AddString("9");
	m_combo_SharpTime.AddString("10");
	m_combo_SharpTime.AddString("20");
	m_combo_SharpTime.AddString("30");
	m_combo_SharpTime.AddString("40");
	m_combo_SharpTime.AddString("50");
	m_combo_SharpTime.AddString("60");
	m_combo_SharpTime.AddString("70");
	m_combo_SharpTime.AddString("80");
	m_combo_SharpTime.AddString("90");
	m_combo_SharpTime.AddString("100");
	m_combo_SharpTime.AddString("110");
	m_combo_SharpTime.AddString("200");
	m_combo_SharpTime.AddString("300");

	str.Format("%g",g_Config.g_shapeTimeSetValue);
	m_combo_SharpTime.SetWindowTextA(str);




	flashGridctrl();

	((CButton*)GetDlgItem(IDC_BTN_STOP))->EnableWindow(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPartyProProgramDlg::flashGridctrl()
{
	int index=0;
	CString str;
	m_pTreeGrid->DeleteAll();
	std::vector<SCENE*>::iterator scene_vec_iter;
	for(scene_vec_iter = m_cutRigion ->scene_vec.begin();scene_vec_iter< m_cutRigion->scene_vec.end();scene_vec_iter++)
	{

		SCENE* pData = *scene_vec_iter;
		//m_curData = pData;
		CItemInfo *info = new CItemInfo;
		info->SetImage(0);
		info->SetData(pData);

		str.Format(_T("%d"),++index);
		info->SetItemText(str);

		str.Format(_T("%g"), pData->waitTime);
		info->AddSubItemText(str);

		str.Format(_T("%g"), pData->shapeTime);
		info->AddSubItemText(str);

		info->AddSubItemText(pData->time.Format("%M:%S"));

		for (int i=0;i<CHANNEL_COUNT;i++)
		{
			str.Format(_T("%d"), pData->channelValue[i]);
			info->AddSubItemText(str);
		}


		m_pTreeGrid->InsertRootItem(info);
	}
	CRect scene_rc;
	m_static_scene_list_zone.GetWindowRect(scene_rc);
	ScreenToClient(scene_rc);
	scene_rc.top += 60;
	scene_rc.bottom -= 10;
	scene_rc.left += 10;
	scene_rc.right -= 20;
	m_pTreeGrid->MoveWindow(&scene_rc);
	//m_pTreeGrid->SetScrollRange(SB_HORZ, 0, CHANNEL_COUNT + 4);
	m_pTreeGrid->Invalidate();
	
}

void CPartyProProgramDlg::rushGridctrlData(CItemInfo *info)
{
	int index=0;
	CString str;
	std::vector<SCENE*>::iterator scene_vec_iter;
	for(scene_vec_iter = m_cutRigion->scene_vec.begin();scene_vec_iter<m_cutRigion->scene_vec.end();scene_vec_iter++)
	{

		SCENE* pData = *scene_vec_iter;
		if (pData!=info->GetData())continue;
		str.Format(_T("%d"),++index);
		info->SetItemText(str);

		str.Format(_T("%g"), pData->waitTime);
		info->AddSubItemText(str);

		str.Format(_T("%g"), pData->shapeTime);
		info->AddSubItemText(str);

		info->AddSubItemText(pData->time.Format("%M:%S"));

		for (int i=0;i<CHANNEL_COUNT;i++)
		{
			str.Format(_T("%d"), pData->channelValue[i]);
			info->AddSubItemText(str);
		}
	}
	//m_pTreeGrid->SetScrollRange(SB_HORZ, 0, CHANNEL_COUNT + 4);
}


void CPartyProProgramDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPartyProProgramDlg::OnPaint()
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
HCURSOR CPartyProProgramDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPartyProProgramDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);


	if (m_pTreeGrid && m_pTreeGrid->GetSafeHwnd()&& m_static_scene_list_zone)
	{
		CRect scene_rc;
		m_static_scene_list_zone.GetWindowRect(scene_rc);
		ScreenToClient(scene_rc);
		scene_rc.top += 60;
		scene_rc.bottom -= 10;
		scene_rc.left += 10;
		scene_rc.right -= 20;
		m_pTreeGrid->MoveWindow(&scene_rc);
		m_pTreeGrid->Invalidate();
	}

}

void CPartyProProgramDlg::GroupDetal_xy(int x_pos,int y_pos)
{
	if (!m_curData)return;
	int n = 0;
	int curIndex = 0;
	std::vector<LIGHT_UNIT>::iterator light_iter;
	for (light_iter = g_Config.g_light_vec.begin(); light_iter != g_Config.g_light_vec.end(); light_iter++)
	{
		if (!(*light_iter).bGroup)continue;
		int start = (*light_iter).start_channel;
		int count = (*light_iter).channel_count;
		for (int i = start; i < start + count; i++) {
			if ((*light_iter).channel_name[i - start].Compare("X轴") == 0) {
				m_curData->channelValue[i] = x_pos;
			}
			if ((*light_iter).channel_name[i - start].Compare("Y轴") == 0)m_curData->channelValue[i] = y_pos;
		}
	}

	//for (int i=0;i<CHANNEL_COUNT;i++)
	//{
	//	if (m_cutRigion->RemarkName[i].Compare("X轴") == 0)m_curData->channelValue[i] = x_pos;
	//	if (m_cutRigion->RemarkName[i].Compare("Y轴") == 0)m_curData->channelValue[i] = y_pos;
	//}
	m_sliderDlg->m_curScene = m_curData;
	m_sliderDlg->M_UpdateData(false);
	if (g_Config.g_bChangeScene) {
		flashGridctrl();
	}
}


void CPartyProProgramDlg::GroupDetal_color(int red, int green,int blue)
{
	if (!m_curData)return;
	int n = 0;
	int curIndex = 0;
	std::vector<LIGHT_UNIT>::iterator light_iter;
	for (light_iter = g_Config.g_light_vec.begin(); light_iter != g_Config.g_light_vec.end(); light_iter++)
	{
		if (!(*light_iter).bGroup)continue;
		int start = (*light_iter).start_channel;
		int count = (*light_iter).channel_count;
		for (int i = start; i < start + count; i++) {
			if ((*light_iter).channel_name[i - start].Compare("红色") == 0)m_curData->channelValue[i] = red;
			if ((*light_iter).channel_name[i - start].Compare("绿色") == 0)m_curData->channelValue[i] = green;
			if ((*light_iter).channel_name[i - start].Compare("蓝色") == 0)m_curData->channelValue[i] = blue;
		}
	}

	//for (int i = 0; i < CHANNEL_COUNT; i++)
	//{
	//	if (m_cutRigion->RemarkName[i].Compare("红色") == 0)m_curData->channelValue[i] = red;
	//	if (m_cutRigion->RemarkName[i].Compare("绿色") == 0)m_curData->channelValue[i] = green;
	//	if (m_cutRigion->RemarkName[i].Compare("蓝色") == 0)m_curData->channelValue[i] = blue;
	//}
	m_sliderDlg->m_curScene = m_curData;
	m_sliderDlg->M_UpdateData(false);
	if (g_Config.g_bChangeScene) {
		flashGridctrl();
	}
}

void CPartyProProgramDlg::OnBnClickedBtnXyControl()
{
	// TODO: 在此添加控件通知处理程序代码
	CdrawDlg::getContext(this)->ShowWindow(SW_SHOW);
	//CdrawDlg dlg(this);
	//dlg.DoModal();
	//GroupDetal_xy(dlg.m_x_pos,dlg.m_y_pos);
}


void CPartyProProgramDlg::OnBnClickedBtnColorControl()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorPickerDlg::getContext(this)->ShowWindow(SW_SHOW);
	//CColorPickerDlg dlg(this);
	//dlg.DoModal();
	//GroupDetal_color(dlg.m_red,dlg.m_green, dlg.m_blue);
}


void CPartyProProgramDlg::OnButtonClick(UINT nID)
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = nID - IDC_RADIO1;
	// TODO: 在此添加控件通知处理程序代码
	g_Config.g_startPos = nIndex*32;
	m_sliderDlg->M_UpdateData(false);
}


void CPartyProProgramDlg::OnCbnSelchangeComboRegion()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_Config.g_rigion_num = m_combo_region.GetCurSel();
	m_cutRigion = &g_Config.g_rigion[g_Config.g_rigion_num];
	m_sliderDlg->M_UpdateData(false);
}


void CPartyProProgramDlg::OnBnClickedBtnInterScene()
{
	// TODO: 在此添加控件通知处理程序代码
	SCENE* pData = new SCENE;
	memset(pData,NULL,sizeof(SCENE));
	g_CriticalSection.Lock();
	if(!m_curData){
		if(!m_cutRigion->scene_vec.empty()){
			pData->copy(m_cutRigion->scene_vec.back());
		}
		m_cutRigion->scene_vec.push_back(pData);
	}
	else{
		std::vector<SCENE*>::iterator scene_vec_iter;

		for(scene_vec_iter = m_cutRigion->scene_vec.begin();scene_vec_iter<m_cutRigion->scene_vec.end();scene_vec_iter++)
		{
			SCENE* pSouchData = *scene_vec_iter;
			if (pSouchData==m_curData)
			{
				pData->copy(pSouchData);
				m_cutRigion->scene_vec.insert(scene_vec_iter,pData);
				break;
			}
		}
	}
	m_curData = pData;
	flashGridctrl();
	g_CriticalSection.Unlock();
}


void CPartyProProgramDlg::OnBnClickedBtnDeleteScene()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_curData)return;

	std::vector<SCENE*>::iterator scene_vec_iter;
	for(scene_vec_iter = m_cutRigion->scene_vec.begin();scene_vec_iter<m_cutRigion->scene_vec.end();scene_vec_iter++)
	{
		SCENE* pSouchData = *scene_vec_iter;
		if (pSouchData==m_curData)
		{
			m_cutRigion->scene_vec.erase(scene_vec_iter);
			break;
		}
	}

	flashGridctrl();
}



LRESULT CPartyProProgramDlg::OnUserSeletedItem(WPARAM wParam, LPARAM lParam)
{
	m_pTreeGrid = (CSuperGridCtrl *)lParam;

	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel >= 0)
	{
		CSuperGridCtrl::CTreeItem *hItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hItem);
		m_curData = (SCENE *)pInfo->GetData();
		if (m_curData) {
			m_sliderDlg->m_curScene = m_curData;
			m_sliderDlg->M_UpdateData(false);
		}

		CString str;
		str.Format("%g",m_curData->waitTime);
		m_combo_WaitTime.SetWindowTextA(str);
		str.Format("%g",m_curData->shapeTime);
		m_combo_SharpTime.SetWindowTextA(str);
		m_iItem = nSel;
	}
	return 0;
}



//双击时，可修改数据
LRESULT CPartyProProgramDlg::OnNMDblclkList(WPARAM wParam, LPARAM lParam)
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

	itemTxt = m_pTreeGrid->GetItemText(m_iItem, m_iSubItem);
	width = m_pTreeGrid->GetColumnWidth(m_iSubItem);
	m_pTreeGrid->GetWindowRect(&dlgRect);
	m_pTreeGrid->GetSubItemRect(m_iItem, m_iSubItem, LVIR_BOUNDS, rect);


	ScreenToClient(&dlgRect);
	int height = rect.Height();
	rect.top += (dlgRect.top );
	rect.left += (dlgRect.left);
	rect.bottom = rect.top + height;
	rect.right = rect.left + width;

	if (m_iSubItem == 1)
	{
		m_combo_WaitTime.SetWindowText(itemTxt);
		::SetWindowPos(m_combo_WaitTime.GetSafeHwnd(), HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height() + 100, SWP_SHOWWINDOW);
		m_combo_WaitTime.ShowWindow(SW_SHOW);

		m_combo_WaitTime.SetWindowTextA(itemTxt);

	}
	else if (m_iSubItem == 2)
	{
		m_combo_SharpTime.SetWindowText(itemTxt);
		::SetWindowPos(m_combo_SharpTime.GetSafeHwnd(), HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height() + 100, SWP_SHOWWINDOW);
		m_combo_SharpTime.ShowWindow(SW_SHOW);

		m_combo_SharpTime.SetWindowTextA(itemTxt);

	}
	else
	{
		m_edit_var.SetWindowText(itemTxt);
		::SetWindowPos(m_edit_var.GetSafeHwnd(), HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		m_edit_var.ShowWindow(SW_SHOW);
		m_edit_var.SetFocus();
	}

	return 0;
}

//
//编辑完后修改相应数据
void CPartyProProgramDlg::OnEnKillfocusGreeitemEdit()
{
	UpdateData(TRUE);
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel >= 0)
	{
		CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);
		SCENE *pData = (SCENE *)pInfo->GetData();
		
		CString editStr;
		m_edit_var.GetWindowTextA(editStr);
		//修改
		m_pTreeGrid->SetItemText(m_iItem, m_iSubItem, editStr);

		//if (m_iSubItem == 1){
		//	if(editStr)
		//		pData->waitTime = atof(editStr);
		//}
		//else if (m_iSubItem == 2) {
		//	if(editStr)
		//		pData->shapeTime = atof(editStr);
		//}
/*		else*/ if (m_iSubItem == 3) {
			if(editStr)
				pData->time.Format("%M:%S");
		}
		else if (m_iSubItem >= 4&&m_iSubItem<=516) {
			if(editStr)
				pData->channelValue[m_iSubItem-4] = atoi(editStr);

		}

		m_pTreeGrid->Invalidate();
	}

	//
	m_edit_var.ShowWindow(SW_HIDE);

}


void CPartyProProgramDlg::OnCbnKillfocusComboKnife()
{
	UpdateData(TRUE);

	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel >= 0)
	{
		CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);
		SCENE*pData = (SCENE*)pInfo->GetData();

		//修改
		CString str;
		if (m_iSubItem == 1) {
			m_combo_WaitTime.GetWindowTextA(str);
			m_pTreeGrid->SetItemText(m_iItem, m_iSubItem, str);
			pData->waitTime = atof(str);
			m_combo_WaitTime.ShowWindow(SW_HIDE);
		}	
		else if (m_iSubItem == 2) {
			m_combo_SharpTime.GetWindowTextA(str);
			m_pTreeGrid->SetItemText(m_iItem, m_iSubItem, str);
			pData->shapeTime = atof(str);
			m_combo_SharpTime.ShowWindow(SW_HIDE);
		}
			
		m_pTreeGrid->SetFocus();
		m_pTreeGrid->Invalidate();
	}
}





void CPartyProProgramDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_Config.save();
	CDialogEx::OnClose();
}




void CPartyProProgramDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString str;
	int nIndex = m_combo_WaitTime.GetCurSel();
	m_combo_WaitTime.GetLBText(nIndex,str);

	if(str)
		g_Config.g_waitTimeSetValue = atof(str);

	//int nSel = m_pTreeGrid->GetSelectedItem();
	//if (nSel >= 0)
	//{
	//	CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
	//	CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);
	//	SCENE *pData = (SCENE *)pInfo->GetData();

	//	//修改
	//	m_pTreeGrid->SetItemText(m_iItem, m_iSubItem, str);
	//	if(str)
	//		pData->waitTime = atof(str);
	//	//m_pTreeGrid->Invalidate();
	//	flashGridctrl();
	//}
	if(m_curData&&str){
		m_curData->waitTime = atof(str);
		flashGridctrl();
	}
}


void CPartyProProgramDlg::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString str;
	int nIndex = m_combo_SharpTime.GetCurSel();
	m_combo_SharpTime.GetLBText(nIndex,str);

	if(str)
		g_Config.g_shapeTimeSetValue = atof(str);

	//int nSel = m_pTreeGrid->GetSelectedItem();
	//if (nSel >= 0)
	//{
	//	CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
	//	CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);
	//	SCENE *pData = (SCENE *)pInfo->GetData();

	//	//修改
	//	m_pTreeGrid->SetItemText(m_iItem, m_iSubItem, str);
	//	if(str)
	//		pData->shapeTime = atof(str);
	//	//m_pTreeGrid->Invalidate();
	//	flashGridctrl();
	//}
	if(m_curData&&str){
		m_curData->shapeTime = atof(str);
		flashGridctrl();
	}

}


BOOL CPartyProProgramDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
		//else if (pMsg->wParam == VK_UP|| pMsg->wParam == VK_DOWN)
		//{
		//	OnUserSelectedGridItem(0,0);
		//	//return TRUE;
		//}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


afx_msg LRESULT CPartyProProgramDlg::OnFlushSceneList(WPARAM wParam,LPARAM lParam)
{
	//CString *str= (CString*)(LPCTSTR)lParam;
	int flag = (int)wParam;
	if (flag==0)
	{
		UpdateData(TRUE);
		if(g_Config.g_bChangeScene&&m_curData){

			m_curData = m_sliderDlg->m_curScene;
			//rushGridctrlData();
			flashGridctrl();
		}

	}
	else if (flag==1){
		////设置水平位置
		//int rangeMin, rangeMax;
		//m_pTreeGrid->GetScrollRange(SB_HORZ, &rangeMin, &rangeMax);

		//m_pTreeGrid->SetScrollPos(SB_HORZ, (g_Config.g_startPos+4)* rangeMax / (CHANNEL_COUNT+4));
		//m_pTreeGrid->Scroll();

		//int nSel = g_Config.g_startPos + 4; //第五十条item
		//int nItem = m_pTreeGrid->get();
		//CRect rc;
		//m_pTreeGrid->GetItemRect(nItem, rc, LVIR_BOUNDS);
		//CSize sz((nSel - nItem) * rc.Width(), 0);
		//m_pTreeGrid->Scroll(sz);
		//m_pTreeGrid->SetItemState(nSel, LVIS_SELECTED, LVIS_SELECTED);
	}
	return 0;
}



void CPartyProProgramDlg::OnBnClickedBtnOpenFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"INI配置文件t(*.ppl)|*.ppl||",AfxGetMainWnd());
	file.m_ofn.lpstrInitialDir=_T(".\\");
	if(file.DoModal()==IDOK)
	{
		CString str=file.GetPathName(); 
		//theApp.NewParam.FilePath.RunningPath = str;
		if(str.Right(4).Compare(_T(".ppl"))!=0){
			return;
		}
		g_Config.read(str);
		m_curData = nullptr;
	}
	flashGridctrl();
}


void CPartyProProgramDlg::OnBnClickedBtnPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_BTN_PLAY))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BTN_STOP))->EnableWindow(TRUE);
	bThreadExit = false;

	m_pThread = AfxBeginThread(ThreadDebugScene,this);

	if(NULL == m_pThread)
	{
		TRACE("创建新的线程出错！\n");
		return;
	}
}



void CPartyProProgramDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码(
	((CButton*)GetDlgItem(IDC_BTN_PLAY))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BTN_STOP))->EnableWindow(FALSE);
	
	bThreadExit = true;
}


void CPartyProProgramDlg::OnBnClickedCheckLoop()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
}


void CPartyProProgramDlg::OnBnClickedBtnRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	if (bThreadExit)return;

	//m_pRecordThread = AfxBeginThread(ThreadDebugRecord, this);

	//if (NULL == m_pRecordThread)
	//{
	//	TRACE("创建新的线程出错！\n");
	//	return;
	//}
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
	CString str_url = szFilePath;
	str_url += "/PartProRecord.exe";
	ShellExecute(NULL, "open", str_url, NULL, NULL, SW_SHOWNORMAL);
}


void CPartyProProgramDlg::OnBnClickedBtnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgPort::getContext()->ShowWindow(SW_SHOW);
}


void CPartyProProgramDlg::OnBnClickedChangeChannelValue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
}


void CPartyProProgramDlg::OnBnClickedCheckControlSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_cutRigion->control_select = m_control_select;
}


void CPartyProProgramDlg::OnCbnSelchangeComboChannelType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_cutRigion->channel_type = m_channel_type;
}


void CPartyProProgramDlg::OnBnClickedBtnSaveFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "配置文件t(*.ppl)|*.ppl||", AfxGetMainWnd());
	file.m_ofn.lpstrInitialDir = _T(".\\");
	if (file.DoModal() == IDOK)
	{
		CString str = file.GetPathName();
		//theApp.NewParam.FilePath.RunningPath = str;
		if(str.Right(4).Compare(".ppl")!=0)str += _T(".ppl");
		g_Config.save(str);
	}
}


void CPartyProProgramDlg::OnAddDeleteLight()
{
	// TODO: 在此添加命令处理程序代码
	CAddDeleteLight dlg;
	dlg.DoModal();
	m_sliderDlg->M_UpdateData(false);
}


void CPartyProProgramDlg::OnOpenLightLib()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
	CString str_url = szFilePath;
	str_url += "/LightLib.exe";
	ShellExecute(NULL, "open", str_url, NULL, NULL, SW_SHOWNORMAL);
}


void CPartyProProgramDlg::OnBnClickedBtnSelectLight()
{
	// TODO: 在此添加控件通知处理程序代码
	CSelectLightGroup dlg;
	dlg.DoModal();
}


void CPartyProProgramDlg::OnRecord()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedBtnRecord();
}
