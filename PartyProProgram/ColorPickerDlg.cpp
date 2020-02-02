
// ColorPickerDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ColorPickerDlg.h"


// CColorPickerDlg 对话框
CColorPickerDlg* CColorPickerDlg::m_context = nullptr;

CColorPickerDlg* CColorPickerDlg::getContext(CWnd* pParent)
{
	if (m_context == nullptr)
		m_context = new CColorPickerDlg(pParent);
	return m_context;
}

void CColorPickerDlg::destroyMe()
{
	//
	delete m_context;
	m_context = nullptr;
}

CColorPickerDlg::CColorPickerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorPickerDlg::IDD, pParent)
	, m_red(0)
	, m_green(0)
	, m_blue(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_parent = (CPartyProProgramDlg*)pParent;
	// 初始化 一 old_color，是当前选中的颜色，外部可用全局变量记录起来
	int old_color = RGB(0,0,0);
	m_event.init(this, old_color);
	
}

void CColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_R_VAL, m_red);
	DDX_Text(pDX, IDC_EDIT_G_VAL, m_green);
	DDX_Text(pDX, IDC_EDIT_B_VAL, m_blue);
}

BEGIN_MESSAGE_MAP(CColorPickerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()

	//必须事件  --- start
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE() 
	ON_WM_PAINT() 

	//三个拖拉条的事件
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_R, &CColorPickerDlg::OnNMCustomdrawSliderR)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_G, &CColorPickerDlg::OnNMCustomdrawSliderG)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_B, &CColorPickerDlg::OnNMCustomdrawSliderB)
	//必须事件  --- end

	ON_BN_CLICKED(IDOK, &CColorPickerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CColorPickerDlg 消息处理程序

BOOL CColorPickerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化开始 -- 
	m_event.StartInit(IDC_STATIC_HSB_RECT,  //彩色带的Static_ID
		IDC_STATIC_OLD,						//旧颜色的Static_ID
		IDC_STATIC_NEW,						//新选择的颜色的Static_ID
		IDB_BITMAP_HSB);					//BMP图片ID

	//RGB值显示框 -- 对应IDC_Edit控件
	m_event.InitRGBEdit(IDC_EDIT_R_VAL, IDC_EDIT_G_VAL, IDC_EDIT_B_VAL);


	//设置控件的值 -- 传入的三条拖拉控件ID
	m_event.InitSpinCtrl(IDC_SLIDER_R, IDC_SLIDER_G, IDC_SLIDER_B);


	//初始化结束
	m_event.EndInit();


	Invalidate();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CColorPickerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//红色拖拽事件
void CColorPickerDlg::OnNMCustomdrawSliderR(NMHDR *pNMHDR, LRESULT *pResult)
{
	int r = ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_R))->GetPos();

	//更新R值
	m_event.UpdateRColor(r);  
	//UpdateData(true);
	//m_parent->GroupDetal_color(m_red, m_green, m_blue);
}

//绿色拖拽事件
void CColorPickerDlg::OnNMCustomdrawSliderG(NMHDR *pNMHDR, LRESULT *pResult)
{
	int g = ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_G))->GetPos();

	//更新G值
	m_event.UpdateGColor(g);
	//UpdateData(true);
	//m_parent->GroupDetal_color(m_red, m_green, m_blue);
}


//蓝色拖拽事件
void CColorPickerDlg::OnNMCustomdrawSliderB(NMHDR *pNMHDR, LRESULT *pResult)
{
	int b = ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_B))->GetPos();

	//更新B值
	m_event.UpdateBColor(b); 
	//UpdateData(true);
	//m_parent->GroupDetal_color(m_red, m_green, m_blue);
}

// ---- 鼠标事件 start
void CColorPickerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_event.mouse_down(nFlags, point);
	CDialog::OnLButtonDown(nFlags, point);
	UpdateData(true);	
	m_parent->GroupDetal_color(m_red, m_green, m_blue);
}
void CColorPickerDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_event.mouse_up(nFlags, point);
	CDialog::OnLButtonUp(nFlags, point);
	UpdateData(true);
	m_parent->GroupDetal_color(m_red, m_green, m_blue);
}
void CColorPickerDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_event.mouse_move(nFlags, point);
	CDialog::OnMouseMove(nFlags, point);
	//UpdateData(true);
	//m_parent->GroupDetal_color(m_red, m_green, m_blue);
}
// ---- 鼠标事件 end

//重绘事件
void CColorPickerDlg::OnPaint() 
{
	CPaintDC dc(this); 

	m_event.onPaint(&dc);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CColorPickerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//获取结果
void CColorPickerDlg::OnBnClickedOk()
{
	UpdateData(true);
	CDialog::OnOK();
}


