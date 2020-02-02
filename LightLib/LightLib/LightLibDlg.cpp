
// LightLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LightLib.h"
#include "LightLibDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CLightLibDlg 对话框




CLightLibDlg::CLightLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightLibDlg::IDD, pParent)
	, m_light_type(FALSE)
	, m_light_name(_T(""))
	, m_ch_count(0)
	, m_light_factory(_T(""))
	, m_red_ch(0)
	, m_green_ch(0)
	, m_blue_ch(0)
	, m_x_ch(0)
	, m_y_ch(0)
	, m_x_ch_l(0)
	, m_y_ch_l(0)
	, m_import_file_path(_T(""))
	, m_white_ch(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLightLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ICON, m_icon_list);
	DDX_Radio(pDX, IDC_RADIO1, m_light_type);
	DDX_Text(pDX, IDC_EDIT_LIGHT_NAME, m_light_name);
	DDX_Text(pDX, IDC_EDIT_CHANNEL_COUNT, m_ch_count);
	DDX_Text(pDX, IDC_EDIT_LIGHT_FACTORY, m_light_factory);
	DDX_Text(pDX, IDC_EDIT_RED, m_red_ch);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_green_ch);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_blue_ch);
	DDX_Text(pDX, IDC_EDIT_X, m_x_ch);
	DDX_Text(pDX, IDC_EDIT_Y, m_y_ch);
	DDX_Text(pDX, IDC_EDIT_X_L, m_x_ch_l);
	DDX_Text(pDX, IDC_EDIT_Y_L, m_y_ch_l);
	DDX_Control(pDX, IDC_LIST_CHANNEL, m_channel_list);
	DDX_Text(pDX, IDC_EDIT_WHITE, m_white_ch);
}

BEGIN_MESSAGE_MAP(CLightLibDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_NEW, &CLightLibDlg::OnBnClickedBtnNew)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CLightLibDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CLightLibDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS, &CLightLibDlg::OnBnClickedBtnSaveAs)

	ON_EN_CHANGE(IDC_EDIT_CHANNEL_COUNT, &CLightLibDlg::OnEnChangeEditChannelCount)
	ON_BN_CLICKED(IDC_BTN_SET_CH_NAME, &CLightLibDlg::OnBnClickedBtnSetChName)
	//ON_EN_CHANGE(IDC_EDIT_RED, &CLightLibDlg::OnEnChangeEditRed)
	//ON_EN_CHANGE(IDC_EDIT_GREEN, &CLightLibDlg::OnEnChangeEditGreen)
	//ON_EN_CHANGE(IDC_EDIT_BLUE, &CLightLibDlg::OnEnChangeEditBlue)
	//ON_EN_CHANGE(IDC_EDIT_X, &CLightLibDlg::OnEnChangeEditX)
	//ON_EN_CHANGE(IDC_EDIT_Y, &CLightLibDlg::OnEnChangeEditY)
	//ON_EN_CHANGE(IDC_EDIT_X_L, &CLightLibDlg::OnEnChangeEditXL)
	//ON_EN_CHANGE(IDC_EDIT_Y_L, &CLightLibDlg::OnEnChangeEditYL)
	//ON_EN_CHANGE(IDC_EDIT_WHITE, &CLightLibDlg::OnEnChangeEditWhite)
	ON_BN_CLICKED(IDC_BTN_WRITER, &CLightLibDlg::OnBnClickedBtnWriter)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ICON, &CLightLibDlg::OnNMDblclkListIcon)
END_MESSAGE_MAP()


// CLightLibDlg 消息处理程序

void CLightLibDlg::insert_bmp_to_list(int IDB_BMP)
{
	CBitmap* pBmp = new CBitmap();
	pBmp->LoadBitmap(IDB_BMP);
	m_image_list.Add(pBmp, RGB(255, 255, 255));
	delete pBmp;
}

BOOL CLightLibDlg::OnInitDialog()
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
	m_channel_image_list.Create(16, 16, ILC_COLOR, 26, 5);
	m_channel_list.SetImageList(&m_channel_image_list, LVSIL_SMALL);

	CRect rect;

	// 获取编程语言列表视图控件的位置和大小   
	m_channel_list.GetClientRect(&rect);
	

	// 为列表视图控件添加全行选中和栅格风格   
	m_channel_list.SetExtendedStyle(m_channel_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVIS_SELECTED);

	// 为列表视图控件添加三列   
	m_channel_list.InsertColumn(0, _T("通道号"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_channel_list.InsertColumn(1, _T("通道描述"), LVCFMT_CENTER, rect.Width() *2 / 3, 1);

	// 在列表视图控件中插入列表项，并设置列表子项文本 
	int row = 0;
	CString str;
	for (int i=0;i<m_ch_count;i++)
	{
		str.Format("%d", i + 1);
		m_channel_list.InsertItem(row, str,row);
		if (m_channel_name[i].Compare("")==0) {
			str.Format("通道%d", i + 1);
		}
		else {
			str = m_channel_name[i];
		}
		m_channel_list.SetItemText(row, 1, str);
		row++;
	}


	/////////////////////////////////////////////////////////

	// 获取编程语言列表视图控件的位置和大小   
	m_icon_list.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格   
	m_icon_list.SetExtendedStyle(m_icon_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVIS_SELECTED );

	// 为列表视图控件添加三列   
	m_icon_list.InsertColumn(0, _T("图标/名称"), LVCFMT_CENTER, rect.Width());

	// 在列表视图控件中插入列表项，并设置列表子项文本 
	m_image_list.Create(16, 16, ILC_COLOR, 26, 5);

	insert_bmp_to_list(IDB_BITMAP1);
	insert_bmp_to_list(IDB_BITMAP2);
	insert_bmp_to_list(IDB_BITMAP3);
	insert_bmp_to_list(IDB_BITMAP4);
	insert_bmp_to_list(IDB_BITMAP5);
	insert_bmp_to_list(IDB_BITMAP6);
	insert_bmp_to_list(IDB_BITMAP7);
	insert_bmp_to_list(IDB_BITMAP8);
	insert_bmp_to_list(IDB_BITMAP9);
	insert_bmp_to_list(IDB_BITMAP10);
	insert_bmp_to_list(IDB_BITMAP11);
	insert_bmp_to_list(IDB_BITMAP12);
	insert_bmp_to_list(IDB_BITMAP13);
	insert_bmp_to_list(IDB_BITMAP14);
	insert_bmp_to_list(IDB_BITMAP15);
	insert_bmp_to_list(IDB_BITMAP16);
	insert_bmp_to_list(IDB_BITMAP17);
	insert_bmp_to_list(IDB_BITMAP18);
	insert_bmp_to_list(IDB_BITMAP19);
	insert_bmp_to_list(IDB_BITMAP20);
	insert_bmp_to_list(IDB_BITMAP21);
	insert_bmp_to_list(IDB_BITMAP22);
	insert_bmp_to_list(IDB_BITMAP23);
	insert_bmp_to_list(IDB_BITMAP24);
	insert_bmp_to_list(IDB_BITMAP25);
	insert_bmp_to_list(IDB_BITMAP26);

	m_icon_list.SetImageList(&m_image_list, LVSIL_SMALL);

	m_icon_list.InsertItem(row++, "推杆", row - 1);
	m_icon_list.InsertItem(row++, "红色", row - 1);
	m_icon_list.InsertItem(row++, "绿色", row - 1);
	m_icon_list.InsertItem(row++, "蓝色", row - 1);
	m_icon_list.InsertItem(row++, "白色", row - 1);
	m_icon_list.InsertItem(row++, "颜色板1", row - 1);
	m_icon_list.InsertItem(row++, "颜色板2", row - 1);
	m_icon_list.InsertItem(row++, "图案板1", row - 1);
	m_icon_list.InsertItem(row++, "图案板2", row - 1);
	m_icon_list.InsertItem(row++, "彩色图案", row - 1);
	m_icon_list.InsertItem(row++, "图案旋转1", row - 1);
	m_icon_list.InsertItem(row++, "图案旋转2", row - 1);
	m_icon_list.InsertItem(row++, "X轴", row - 1);
	m_icon_list.InsertItem(row++, "Y轴", row - 1);
	m_icon_list.InsertItem(row++, "X轴微调", row - 1);
	m_icon_list.InsertItem(row++, "Y轴微调", row - 1);
	m_icon_list.InsertItem(row++, "棱镜", row - 1);
	m_icon_list.InsertItem(row++, "棱镜旋转", row - 1);
	m_icon_list.InsertItem(row++, "频闪", row - 1);
	m_icon_list.InsertItem(row++, "光圈", row - 1);
	m_icon_list.InsertItem(row++, "聚焦", row - 1);
	m_icon_list.InsertItem(row++, "色温", row - 1);
	m_icon_list.InsertItem(row++, "调光", row - 1);
	m_icon_list.InsertItem(row++, "速度", row - 1);
	m_icon_list.InsertItem(row++, "烟雾", row - 1);
	m_icon_list.InsertItem(row++, "混色", row - 1);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLightLibDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLightLibDlg::OnPaint()
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
HCURSOR CLightLibDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLightLibDlg::OnBnClickedBtnNew()
{
	// TODO: 在此添加控件通知处理程序代码
	m_light_type =0;
	m_light_name = "";
	m_ch_count=0;
	m_light_factory="";
	m_red_ch = 0;
	m_green_ch = 0;
	m_blue_ch = 0;
	m_x_ch = 0;
	m_y_ch = 0;
	m_x_ch_l = 0;
	m_y_ch_l = 0;
	m_white_ch = 0;

	for (int i=0;i< LIGHT_MAX_COUNT;i++)
	{
		m_channel_name[i] = "";
		m_channeal_image_index[i] = 0;
	}
	//m_channel_image_list.DeleteImageList();
	UpdateData(false);
	flushData();
}


void CLightLibDlg::OnBnClickedBtnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "打开灯库文件t(*.light)|*.light||", AfxGetMainWnd());
	file.m_ofn.lpstrInitialDir = _T(".\\");
	if (file.DoModal() == IDOK)
	{
		CString str = file.GetPathName();
		m_import_file_path = str;
		//theApp.NewParam.FilePath.RunningPath = str;
		if (str.Right(6).Compare(_T(".light")) != 0) {
			return;
		}
		g_Config.read(str);

		m_light_type = g_Config.g_light_unit.light_type;
		m_light_name = g_Config.g_light_unit.light_name;
		m_ch_count = g_Config.g_light_unit.channel_count;
		m_light_factory = g_Config.g_light_unit.light_factory;
		m_red_ch = g_Config.g_light_unit.red_ch;
		m_green_ch = g_Config.g_light_unit.green_ch;
		m_blue_ch = g_Config.g_light_unit.blue_ch;
		m_white_ch = g_Config.g_light_unit.white_ch;
		m_x_ch = g_Config.g_light_unit.x_ch;
		m_y_ch = g_Config.g_light_unit.y_ch;
		m_x_ch_l = g_Config.g_light_unit.x_ch_l;
		m_y_ch_l = g_Config.g_light_unit.y_ch_l;
		for (int i = 0; i < m_ch_count; i++)
		{
			m_channel_name[i] = g_Config.g_light_unit.channel_name[i];
			IMAGEINFO imgInfo;
			m_image_list.GetImageInfo(findImageIndex(m_channel_name[i]), &imgInfo);
			m_channel_image_list.Replace(i, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		}
		UpdateData(FALSE);
		flushData();

	}
}


void CLightLibDlg::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_Config.g_light_unit.light_type = m_light_type;
	g_Config.g_light_unit.light_name = m_light_name;
	g_Config.g_light_unit.channel_count = m_ch_count;
	g_Config.g_light_unit.light_factory = m_light_factory;
	g_Config.g_light_unit.red_ch = m_red_ch;
	g_Config.g_light_unit.green_ch = m_green_ch;
	g_Config.g_light_unit.blue_ch = m_blue_ch;
	g_Config.g_light_unit.x_ch = m_x_ch;
	g_Config.g_light_unit.y_ch = m_y_ch;
	g_Config.g_light_unit.x_ch_l = m_x_ch_l;
	g_Config.g_light_unit.y_ch_l = m_y_ch_l;
	g_Config.g_light_unit.white_ch = m_white_ch;
	for (int i = 0; i < m_ch_count; i++)
	{
		g_Config.g_light_unit.channel_name[i] = m_channel_name[i];
	}

	if (m_import_file_path.Right(6).Compare(".light") == 0) {
		g_Config.save(m_import_file_path);
		AfxMessageBox("保存成功！");
	}
		
}


void CLightLibDlg::OnBnClickedBtnSaveAs()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_Config.g_light_unit.light_type = m_light_type;
	g_Config.g_light_unit.light_name = m_light_name;
	g_Config.g_light_unit.channel_count = m_ch_count;
	g_Config.g_light_unit.light_factory = m_light_factory;
	g_Config.g_light_unit.red_ch = m_red_ch;
	g_Config.g_light_unit.green_ch = m_green_ch;
	g_Config.g_light_unit.blue_ch = m_blue_ch;
	g_Config.g_light_unit.white_ch = m_white_ch;
	g_Config.g_light_unit.x_ch = m_x_ch;
	g_Config.g_light_unit.y_ch = m_y_ch;
	g_Config.g_light_unit.x_ch_l = m_x_ch_l;
	g_Config.g_light_unit.y_ch_l = m_y_ch_l;
	for (int i = 0; i < m_ch_count; i++)
	{
		g_Config.g_light_unit.channel_name[i] = m_channel_name[i];
	}

	CFileDialog file(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "灯库文件t(*.light)|*.light||", AfxGetMainWnd());
	file.m_ofn.lpstrInitialDir = _T(".\\");
	if (file.DoModal() == IDOK)
	{
		CString str = file.GetPathName();
		str += _T(".light");
		g_Config.save(str);
	}
}


void CLightLibDlg::OnEnChangeEditChannelCount()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	flushData();
}

int CLightLibDlg::findImageIndex(CString name)
{
	for (int i=0;i<26;i++)
	{
		CString text = m_icon_list.GetItemText(i, 0);
		if (name.Compare(text)==0) {
			return i;
		}
	}
	return 0;
}

void CLightLibDlg::flushData()
{
	//刷新列表
	m_channel_list.DeleteAllItems();
	int row = 0;
	CString str;
	for (int i = 0; i < m_ch_count; i++)
	{
		str.Format("%d", i + 1);
		m_channel_list.InsertItem(row, str,row);
		if (m_channel_name[i].Compare("") == 0) {
			str.Format("通道%d", i + 1);
		}
		else {
			str = m_channel_name[i];			
		}
		m_channel_list.SetItemText(row, 1, str);
		row++;
	}
}


void CLightLibDlg::OnBnClickedBtnSetChName()
{
	// TODO: 在此添加控件通知处理程序代码
	int icon_index = m_icon_list.GetSelectionMark();
	int channel_index = m_channel_list.GetSelectionMark();
	if (icon_index>=0&&channel_index>=0)
	{
		CString text = m_icon_list.GetItemText(icon_index,0);
		m_channeal_image_index[channel_index] = icon_index;
		m_channel_name[channel_index] = text;
		IMAGEINFO imgInfo;
		m_image_list.GetImageInfo(icon_index, &imgInfo);
		m_channel_image_list.Replace(channel_index, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		flushData();

		switch (icon_index)
		{
		case 1:
			m_red_ch = channel_index+1;
			break;
		case 2:
			m_green_ch = channel_index + 1;
			break;
		case 3:
			m_blue_ch = channel_index + 1;
			break;
		case 4:
			m_white_ch = channel_index + 1;
			break;
		case 12:
			m_x_ch = channel_index + 1;
			break;
		case 13:
			m_y_ch = channel_index + 1;
			break;
		case 14:
			m_x_ch_l = channel_index + 1;
			break;
		case 15:
			m_y_ch_l = channel_index + 1;
			break;
		default:
			break;
		}
		UpdateData(FALSE);
	}
}


void CLightLibDlg::OnEnChangeEditRed()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_red_ch > 0 && m_red_ch <= m_ch_count)
	{
		CString text = "红色";
		m_channel_name[m_red_ch-1] = text;
		m_channeal_image_index[m_red_ch - 1] = 2;
		IMAGEINFO imgInfo;
		m_image_list.GetImageInfo(2, &imgInfo);
		m_channel_image_list.Replace(m_red_ch - 1, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		flushData();
	}
}


void CLightLibDlg::OnEnChangeEditGreen()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_green_ch > 0 && m_green_ch <= m_ch_count)
	{
		CString text = "绿色";
		m_channel_name[m_green_ch - 1] = text;
		m_channeal_image_index[m_green_ch - 1] = 3;
		IMAGEINFO imgInfo;
		m_image_list.GetImageInfo(3, &imgInfo);
		m_channel_image_list.Replace(m_green_ch - 1, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		flushData();
	}
}


void CLightLibDlg::OnEnChangeEditBlue()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_blue_ch > 0 && m_blue_ch <= m_ch_count)
	{
		CString text = "蓝色";
		m_channel_name[m_blue_ch - 1] = text;
		m_channeal_image_index[m_blue_ch - 1] = 4;
		IMAGEINFO imgInfo;
		m_image_list.GetImageInfo(4, &imgInfo);
		m_channel_image_list.Replace(m_blue_ch - 1, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		flushData();
	}
}


void CLightLibDlg::OnEnChangeEditX()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_x_ch > 0 && m_x_ch <= m_ch_count)
	{
		CString text = "X轴";
		m_channel_name[m_x_ch - 1] = text;
		m_channeal_image_index[m_x_ch - 1] = 13;
		IMAGEINFO imgInfo;
		m_image_list.GetImageInfo(13, &imgInfo);
		m_channel_image_list.Replace(m_x_ch - 1, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		flushData();
	}
}


void CLightLibDlg::OnEnChangeEditY()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_y_ch > 0 && m_y_ch <= m_ch_count)
	{
		CString text = "Y轴";
		m_channel_name[m_y_ch - 1] = text;
		m_channeal_image_index[m_y_ch - 1] = 14;
		IMAGEINFO imgInfo;
		m_image_list.GetImageInfo(14, &imgInfo);
		m_channel_image_list.Replace(m_y_ch - 1, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		flushData();
	}
}


void CLightLibDlg::OnEnChangeEditXL()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_x_ch_l > 0 && m_x_ch_l <= m_ch_count)
	{
		CString text = "X轴微调";
		m_channel_name[m_x_ch_l - 1] = text;
		m_channeal_image_index[m_x_ch_l - 1] = 15;
		IMAGEINFO imgInfo;
		m_image_list.GetImageInfo(15, &imgInfo);
		m_channel_image_list.Replace(m_x_ch_l - 1, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		flushData();
	}
}


void CLightLibDlg::OnEnChangeEditYL()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_y_ch_l > 0 && m_y_ch_l <= m_ch_count)
	{
		CString text = "Y轴微调";
		m_channel_name[m_y_ch_l - 1] = text;
		m_channeal_image_index[m_y_ch_l - 1] = 16;
		IMAGEINFO imgInfo;
		m_image_list.GetImageInfo(16, &imgInfo);
		m_channel_image_list.Replace(m_y_ch_l - 1, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		flushData();
	}
}

void CLightLibDlg::OnEnChangeEditWhite()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_white_ch > 0 && m_white_ch <= m_ch_count)
	{
		CString text = "白色";
		m_channel_name[m_white_ch - 1] = text;
		m_channeal_image_index[m_white_ch - 1] = 5;
		IMAGEINFO imgInfo;
		m_image_list.GetImageInfo(16, &imgInfo);
		m_channel_image_list.Replace(m_white_ch - 1, CBitmap::FromHandle(imgInfo.hbmImage), NULL);
		flushData();
	}
}



void CLightLibDlg::OnBnClickedBtnWriter()
{
	// TODO: 在此添加控件通知处理程序代码
	OnEnChangeEditRed();
	OnEnChangeEditGreen();
	OnEnChangeEditBlue();
	OnEnChangeEditX();
	OnEnChangeEditY();
	OnEnChangeEditXL();
	OnEnChangeEditYL();
	OnEnChangeEditWhite();
}


void CLightLibDlg::OnNMDblclkListIcon(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedBtnSetChName();
	*pResult = 0;
}
