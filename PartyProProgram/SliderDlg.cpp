// SliderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PartyProProgram.h"
#include "SliderDlg.h"
#include "afxdialogex.h"


// CSliderDlg 对话框

IMPLEMENT_DYNAMIC(CSliderDlg, CDialogEx)

CSliderDlg::CSliderDlg(SCENE* &Data,CWnd* pParent /*=NULL*/)
	: CDialogEx(CSliderDlg::IDD, pParent)
{
	m_curScene = Data;
	bUseUpdate = true;
}

CSliderDlg::~CSliderDlg()
{

}

void CSliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_REMARK1, m_EditRemark[0]);
	DDX_Control(pDX, IDC_EDIT_REMARK2, m_EditRemark[1]);
	DDX_Control(pDX, IDC_EDIT_REMARK3, m_EditRemark[2]);
	DDX_Control(pDX, IDC_EDIT_REMARK4, m_EditRemark[3]);
	DDX_Control(pDX, IDC_EDIT_REMARK5, m_EditRemark[4]);
	DDX_Control(pDX, IDC_EDIT_REMARK6, m_EditRemark[5]);
	DDX_Control(pDX, IDC_EDIT_REMARK7, m_EditRemark[6]);
	DDX_Control(pDX, IDC_EDIT_REMARK8, m_EditRemark[7]);
	DDX_Control(pDX, IDC_EDIT_REMARK9, m_EditRemark[8]);
	DDX_Control(pDX, IDC_EDIT_REMARK10, m_EditRemark[9]);
	DDX_Control(pDX, IDC_EDIT_REMARK11, m_EditRemark[10]);
	DDX_Control(pDX, IDC_EDIT_REMARK12, m_EditRemark[11]);
	DDX_Control(pDX, IDC_EDIT_REMARK13, m_EditRemark[12]);
	DDX_Control(pDX, IDC_EDIT_REMARK14, m_EditRemark[13]);
	DDX_Control(pDX, IDC_EDIT_REMARK15, m_EditRemark[14]);
	DDX_Control(pDX, IDC_EDIT_REMARK16, m_EditRemark[15]);
	DDX_Control(pDX, IDC_EDIT_REMARK17, m_EditRemark[16]);
	DDX_Control(pDX, IDC_EDIT_REMARK18, m_EditRemark[17]);
	DDX_Control(pDX, IDC_EDIT_REMARK19, m_EditRemark[18]);
	DDX_Control(pDX, IDC_EDIT_REMARK20, m_EditRemark[19]);
	DDX_Control(pDX, IDC_EDIT_REMARK21, m_EditRemark[20]);
	DDX_Control(pDX, IDC_EDIT_REMARK22, m_EditRemark[21]);
	DDX_Control(pDX, IDC_EDIT_REMARK23, m_EditRemark[22]);
	DDX_Control(pDX, IDC_EDIT_REMARK24, m_EditRemark[23]);
	DDX_Control(pDX, IDC_EDIT_REMARK25, m_EditRemark[24]);
	DDX_Control(pDX, IDC_EDIT_REMARK26, m_EditRemark[25]);
	DDX_Control(pDX, IDC_EDIT_REMARK27, m_EditRemark[26]);
	DDX_Control(pDX, IDC_EDIT_REMARK28, m_EditRemark[27]);
	DDX_Control(pDX, IDC_EDIT_REMARK29, m_EditRemark[28]);
	DDX_Control(pDX, IDC_EDIT_REMARK30, m_EditRemark[29]);
	DDX_Control(pDX, IDC_EDIT_REMARK31, m_EditRemark[30]);
	DDX_Control(pDX, IDC_EDIT_REMARK32, m_EditRemark[31]);

	DDX_Control(pDX, IDC_SLIDER1, m_SliderCtrl[0]);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderCtrl[1]);
	DDX_Control(pDX, IDC_SLIDER3, m_SliderCtrl[2]);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderCtrl[3]);
	DDX_Control(pDX, IDC_SLIDER5, m_SliderCtrl[4]);
	DDX_Control(pDX, IDC_SLIDER6, m_SliderCtrl[5]);
	DDX_Control(pDX, IDC_SLIDER7, m_SliderCtrl[6]);
	DDX_Control(pDX, IDC_SLIDER8, m_SliderCtrl[7]);
	DDX_Control(pDX, IDC_SLIDER9, m_SliderCtrl[8]);
	DDX_Control(pDX, IDC_SLIDER10, m_SliderCtrl[9]);
	DDX_Control(pDX, IDC_SLIDER11, m_SliderCtrl[10]);
	DDX_Control(pDX, IDC_SLIDER12, m_SliderCtrl[11]);
	DDX_Control(pDX, IDC_SLIDER13, m_SliderCtrl[12]);
	DDX_Control(pDX, IDC_SLIDER14, m_SliderCtrl[13]);
	DDX_Control(pDX, IDC_SLIDER15, m_SliderCtrl[14]);
	DDX_Control(pDX, IDC_SLIDER16, m_SliderCtrl[15]);
	DDX_Control(pDX, IDC_SLIDER17, m_SliderCtrl[16]);
	DDX_Control(pDX, IDC_SLIDER18, m_SliderCtrl[17]);
	DDX_Control(pDX, IDC_SLIDER19, m_SliderCtrl[18]);
	DDX_Control(pDX, IDC_SLIDER20, m_SliderCtrl[19]);
	DDX_Control(pDX, IDC_SLIDER21, m_SliderCtrl[20]);
	DDX_Control(pDX, IDC_SLIDER22, m_SliderCtrl[21]);
	DDX_Control(pDX, IDC_SLIDER23, m_SliderCtrl[22]);
	DDX_Control(pDX, IDC_SLIDER24, m_SliderCtrl[23]);
	DDX_Control(pDX, IDC_SLIDER25, m_SliderCtrl[24]);
	DDX_Control(pDX, IDC_SLIDER26, m_SliderCtrl[25]);
	DDX_Control(pDX, IDC_SLIDER27, m_SliderCtrl[26]);
	DDX_Control(pDX, IDC_SLIDER28, m_SliderCtrl[27]);
	DDX_Control(pDX, IDC_SLIDER29, m_SliderCtrl[28]);
	DDX_Control(pDX, IDC_SLIDER30, m_SliderCtrl[29]);
	DDX_Control(pDX, IDC_SLIDER31, m_SliderCtrl[30]);
	DDX_Control(pDX, IDC_SLIDER32, m_SliderCtrl[31]);

	DDX_Control(pDX, IDC_EDIT_VAR1, m_EditVar[0]);
	DDX_Control(pDX, IDC_EDIT_VAR2, m_EditVar[1]);
	DDX_Control(pDX, IDC_EDIT_VAR3, m_EditVar[2]);
	DDX_Control(pDX, IDC_EDIT_VAR4, m_EditVar[3]);
	DDX_Control(pDX, IDC_EDIT_VAR5, m_EditVar[4]);
	DDX_Control(pDX, IDC_EDIT_VAR6, m_EditVar[5]);
	DDX_Control(pDX, IDC_EDIT_VAR7, m_EditVar[6]);
	DDX_Control(pDX, IDC_EDIT_VAR8, m_EditVar[7]);
	DDX_Control(pDX, IDC_EDIT_VAR9, m_EditVar[8]);
	DDX_Control(pDX, IDC_EDIT_VAR10, m_EditVar[9]);
	DDX_Control(pDX, IDC_EDIT_VAR11, m_EditVar[10]);
	DDX_Control(pDX, IDC_EDIT_VAR12, m_EditVar[11]);
	DDX_Control(pDX, IDC_EDIT_VAR13, m_EditVar[12]);
	DDX_Control(pDX, IDC_EDIT_VAR14, m_EditVar[13]);
	DDX_Control(pDX, IDC_EDIT_VAR15, m_EditVar[14]);
	DDX_Control(pDX, IDC_EDIT_VAR16, m_EditVar[15]);
	DDX_Control(pDX, IDC_EDIT_VAR17, m_EditVar[16]);
	DDX_Control(pDX, IDC_EDIT_VAR18, m_EditVar[17]);
	DDX_Control(pDX, IDC_EDIT_VAR19, m_EditVar[18]);
	DDX_Control(pDX, IDC_EDIT_VAR20, m_EditVar[19]);
	DDX_Control(pDX, IDC_EDIT_VAR21, m_EditVar[20]);
	DDX_Control(pDX, IDC_EDIT_VAR22, m_EditVar[21]);
	DDX_Control(pDX, IDC_EDIT_VAR23, m_EditVar[22]);
	DDX_Control(pDX, IDC_EDIT_VAR24, m_EditVar[23]);
	DDX_Control(pDX, IDC_EDIT_VAR25, m_EditVar[24]);
	DDX_Control(pDX, IDC_EDIT_VAR26, m_EditVar[25]);
	DDX_Control(pDX, IDC_EDIT_VAR27, m_EditVar[26]);
	DDX_Control(pDX, IDC_EDIT_VAR28, m_EditVar[27]);
	DDX_Control(pDX, IDC_EDIT_VAR29, m_EditVar[28]);
	DDX_Control(pDX, IDC_EDIT_VAR30, m_EditVar[29]);
	DDX_Control(pDX, IDC_EDIT_VAR31, m_EditVar[30]);
	DDX_Control(pDX, IDC_EDIT_VAR32, m_EditVar[31]);

	DDX_Control(pDX, IDC_EDIT_CH1, m_EditChannel[0]);
	DDX_Control(pDX, IDC_EDIT_CH2, m_EditChannel[1]);
	DDX_Control(pDX, IDC_EDIT_CH3, m_EditChannel[2]);
	DDX_Control(pDX, IDC_EDIT_CH4, m_EditChannel[3]);
	DDX_Control(pDX, IDC_EDIT_CH5, m_EditChannel[4]);
	DDX_Control(pDX, IDC_EDIT_CH6, m_EditChannel[5]);
	DDX_Control(pDX, IDC_EDIT_CH7, m_EditChannel[6]);
	DDX_Control(pDX, IDC_EDIT_CH8, m_EditChannel[7]);
	DDX_Control(pDX, IDC_EDIT_CH9, m_EditChannel[8]);
	DDX_Control(pDX, IDC_EDIT_CH10, m_EditChannel[9]);
	DDX_Control(pDX, IDC_EDIT_CH11, m_EditChannel[10]);
	DDX_Control(pDX, IDC_EDIT_CH12, m_EditChannel[11]);
	DDX_Control(pDX, IDC_EDIT_CH13, m_EditChannel[12]);
	DDX_Control(pDX, IDC_EDIT_CH14, m_EditChannel[13]);
	DDX_Control(pDX, IDC_EDIT_CH15, m_EditChannel[14]);
	DDX_Control(pDX, IDC_EDIT_CH16, m_EditChannel[15]);
	DDX_Control(pDX, IDC_EDIT_CH17, m_EditChannel[16]);
	DDX_Control(pDX, IDC_EDIT_CH18, m_EditChannel[17]);
	DDX_Control(pDX, IDC_EDIT_CH19, m_EditChannel[18]);
	DDX_Control(pDX, IDC_EDIT_CH20, m_EditChannel[19]);
	DDX_Control(pDX, IDC_EDIT_CH21, m_EditChannel[20]);
	DDX_Control(pDX, IDC_EDIT_CH22, m_EditChannel[21]);
	DDX_Control(pDX, IDC_EDIT_CH23, m_EditChannel[22]);
	DDX_Control(pDX, IDC_EDIT_CH24, m_EditChannel[23]);
	DDX_Control(pDX, IDC_EDIT_CH25, m_EditChannel[24]);
	DDX_Control(pDX, IDC_EDIT_CH26, m_EditChannel[25]);
	DDX_Control(pDX, IDC_EDIT_CH27, m_EditChannel[26]);
	DDX_Control(pDX, IDC_EDIT_CH28, m_EditChannel[27]);
	DDX_Control(pDX, IDC_EDIT_CH29, m_EditChannel[28]);
	DDX_Control(pDX, IDC_EDIT_CH30, m_EditChannel[29]);
	DDX_Control(pDX, IDC_EDIT_CH31, m_EditChannel[30]);
	DDX_Control(pDX, IDC_EDIT_CH32, m_EditChannel[31]);
	DDX_Control(pDX, IDC_CHECK1, m_VoiceCheck[0]);
	DDX_Control(pDX, IDC_CHECK2, m_VoiceCheck[1]);
	DDX_Control(pDX, IDC_CHECK3, m_VoiceCheck[2]);
	DDX_Control(pDX, IDC_CHECK4, m_VoiceCheck[3]);
	DDX_Control(pDX, IDC_CHECK5, m_VoiceCheck[4]);
	DDX_Control(pDX, IDC_CHECK6, m_VoiceCheck[5]);
	DDX_Control(pDX, IDC_CHECK7, m_VoiceCheck[6]);
	DDX_Control(pDX, IDC_CHECK8, m_VoiceCheck[7]);
	DDX_Control(pDX, IDC_CHECK9, m_VoiceCheck[8]);
	DDX_Control(pDX, IDC_CHECK10, m_VoiceCheck[9]);
	DDX_Control(pDX, IDC_CHECK11, m_VoiceCheck[10]);
	DDX_Control(pDX, IDC_CHECK12, m_VoiceCheck[11]);
	DDX_Control(pDX, IDC_CHECK13, m_VoiceCheck[12]);
	DDX_Control(pDX, IDC_CHECK14, m_VoiceCheck[13]);
	DDX_Control(pDX, IDC_CHECK15, m_VoiceCheck[14]);
	DDX_Control(pDX, IDC_CHECK16, m_VoiceCheck[15]);
	DDX_Control(pDX, IDC_CHECK17, m_VoiceCheck[16]);
	DDX_Control(pDX, IDC_CHECK18, m_VoiceCheck[17]);
	DDX_Control(pDX, IDC_CHECK19, m_VoiceCheck[18]);
	DDX_Control(pDX, IDC_CHECK20, m_VoiceCheck[19]);
	DDX_Control(pDX, IDC_CHECK21, m_VoiceCheck[20]);
	DDX_Control(pDX, IDC_CHECK22, m_VoiceCheck[21]);
	DDX_Control(pDX, IDC_CHECK23, m_VoiceCheck[22]);
	DDX_Control(pDX, IDC_CHECK24, m_VoiceCheck[23]);
	DDX_Control(pDX, IDC_CHECK25, m_VoiceCheck[24]);
	DDX_Control(pDX, IDC_CHECK26, m_VoiceCheck[25]);
	DDX_Control(pDX, IDC_CHECK27, m_VoiceCheck[26]);
	DDX_Control(pDX, IDC_CHECK28, m_VoiceCheck[27]);
	DDX_Control(pDX, IDC_CHECK29, m_VoiceCheck[28]);
	DDX_Control(pDX, IDC_CHECK30, m_VoiceCheck[29]);
	DDX_Control(pDX, IDC_CHECK31, m_VoiceCheck[30]);
	DDX_Control(pDX, IDC_CHECK32, m_VoiceCheck[31]);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scrollBar);

	DDX_Control(pDX, IDC_EDIT_TOTAL, m_edit_total);
	DDX_Control(pDX, IDC_SLIDER_TOTAL, m_slider_total);
	DDX_Check(pDX, IDC_CHECK_TOTAL, g_Config.g_bTotalEnable);
	
}


BEGIN_MESSAGE_MAP(CSliderDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BTN_ALL_CLEAR, &CSliderDlg::OnBnClickedBtnAllClear)
	ON_BN_CLICKED(IDC_BTN_ALL_ON, &CSliderDlg::OnBnClickedBtnAllOn)
	ON_BN_CLICKED(IDC_CHECK_TOTAL, &CSliderDlg::OnBnClickedCheckTotal)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK1, IDC_CHECK32, OnButtonClicked)
	ON_CONTROL_RANGE(EN_UPDATE, IDC_EDIT_REMARK1, IDC_EDIT_REMARK32, OnRemarkEdit)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_VAR1, IDC_EDIT_VAR32, OnChannelEdit)
END_MESSAGE_MAP()


// CSliderDlg 消息处理程序

BOOL CSliderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	CString ShowChannelStr,remarkName;
	int startCx = 0,startCy = 0;
	CRect grid_rc1;
	this->GetWindowRect(grid_rc1);
	ScreenToClient(grid_rc1);
	int unitWidth = (int)(grid_rc1.Width()/*-150*/)/32.0f;
	startCy = grid_rc1.Height()/12;

	for (int i=0;i<SHOW_COUNT;i++)
	{
		remarkName = g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + i];
		m_EditRemark[i].SetWindowText(remarkName);
		m_EditRemark[i].MoveWindow(startCx,0,unitWidth, startCy,0);
		
		if(m_curScene)
			ShowChannelStr.Format("%d", m_curScene->channelValue[g_Config.g_startPos+i]);
		else
			ShowChannelStr.Format("%d", 0);
		m_EditVar[i].SetWindowText(ShowChannelStr);
		m_EditVar[i].MoveWindow(startCx, startCy,unitWidth, startCy,0);

		m_SliderCtrl[i].SetRangeMin(0,TRUE); ///滑块最小值为0
		m_SliderCtrl[i].SetRangeMax(255,TRUE);///滑块最大值为255
		m_SliderCtrl[i].SetTicFreq(1); ////刻度为每32刻一度
		m_SliderCtrl[i].SetPageSize(1/*LIGHT_COUNT*/);///按一下键盘上下方向键的跳值
		m_SliderCtrl[i].SetLineSize(1);///方向键的跳值 
		//m_SliderCtrl[i].SetPos(255);      ///滑块的值显示为0处
		if (m_curScene)
			m_SliderCtrl[i].SetPos(255- m_curScene->channelValue[g_Config.g_startPos+i]);
		else
			m_SliderCtrl[i].SetPos(255);
		m_SliderCtrl[i].MoveWindow(startCx, 2*startCy,unitWidth,6 * startCy,0);

		ShowChannelStr.Format("%d",g_Config.g_startPos+i+1);
		m_EditChannel[i].SetWindowText(ShowChannelStr);
		m_EditChannel[i].MoveWindow(startCx,8* startCy,unitWidth, startCy,0);
		m_EditChannel[i].SetReadOnly(1);

		if (m_curScene)m_VoiceCheck[i].SetCheck(m_curScene->voiceCheck[g_Config.g_startPos + i]);
		m_VoiceCheck[i].MoveWindow(startCx+5, 9*startCy, unitWidth, startCy, 0);

		startCx += unitWidth;

	}
	// 设置水平滚动条的滚动范围为1到100
	m_scrollBar.SetScrollRange(0, CHANNEL_COUNT-32);
	// 设置水平滚动条的初始位置为20
	m_scrollBar.SetScrollPos(g_Config.g_startPos);
	// 在编辑框中显示20
	//SetDlgItemInt(IDC_HSCROLL_EDIT, 20);
	m_scrollBar.MoveWindow(0,grid_rc1.Height()-57,grid_rc1.Width()-32,25,0);


	//总使能
	ShowChannelStr.Format("%d",g_Config.g_totalChannelValue);
	m_edit_total.SetWindowText(ShowChannelStr);
	m_edit_total.MoveWindow(startCx+40,25,unitWidth,25,0);

	m_slider_total.SetRangeMin(0,TRUE); ///滑块最小值为0
	m_slider_total.SetRangeMax(255,TRUE);///滑块最大值为255
	m_slider_total.SetTicFreq(1); ////刻度为每32刻一度
	m_slider_total.SetPageSize(1/*LIGHT_COUNT*/);///按一下键盘上下方向键的跳值
	m_slider_total.SetLineSize(1);///方向键的跳值 
	m_slider_total.SetPos(255-g_Config.g_totalChannelValue);
	m_slider_total.MoveWindow(startCx+40,50,150,176,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}





void CSliderDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int pos = pScrollBar->GetScrollPos();
	bUseUpdate = true;
	switch (nSBCode)
	{
		// 如果向左滚动一列，则pos减1
	case SB_LINELEFT:
		pos -= 1;
		break;
		// 如果向右滚动一列，则pos加1
	case SB_LINERIGHT:
		pos += 1;
		break;
	//	// 如果向左滚动一页，则pos减10
	//case SB_PAGELEFT:
	//	pos -= 10;
	//	break;
	//	// 如果向右滚动一页，则pos加10
	//case SB_PAGERIGHT:
	//	pos += 10;
	//	break;
		// 如果滚动到最左端，则pos为1
	case SB_LEFT:
		pos = 1;
		break;
		// 如果滚动到最右端，则pos为100
	case SB_RIGHT:
		pos = 512;
		break;
		// 如果拖动滚动块到指定位置，则pos赋值为nPos的值
	case SB_THUMBPOSITION:
		pos = nPos;
		break;
		// 下面的m_horiScrollbar.SetScrollPos(pos);执行时
		// 会第二次进入此函数，最终确定滚动块位置，并且会
		// 直接到default分支，所以在此处设置编辑框中显示数值
	case SB_THUMBTRACK:
		pos = nPos;
		break;
	default:
		//SetDlgItemInt(IDC_HSCROLL_EDIT, pos);
		bUseUpdate = false;
		return;
	}
	g_Config.g_startPos = pos;
	CString ShowChannelStr, remarkName;
	for (int i=0;i<SHOW_COUNT;i++)
	{
		remarkName = g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + i];
		m_EditRemark[i].SetWindowText(remarkName);

		if (m_curScene)
			m_SliderCtrl[i].SetPos(255-m_curScene->channelValue[g_Config.g_startPos+i]);
		else
			m_SliderCtrl[i].SetPos(255);

		ShowChannelStr.Format("%d", 255 - m_SliderCtrl[i].GetPos());
		m_EditVar[i].SetWindowText(ShowChannelStr);

		ShowChannelStr.Format("%d",g_Config.g_startPos+i+1);
		m_EditChannel[i].SetWindowText(ShowChannelStr);

		if (m_curScene)m_VoiceCheck[i].SetCheck(m_curScene->voiceCheck[g_Config.g_startPos + i]);

	}
	
	// 设置滚动块位置
	pScrollBar->SetScrollPos(pos);
	::PostMessageA(theApp.GetMainWnd()->m_hWnd,WM_FLUSH_SCROLL,1,NULL);
	bUseUpdate = false;	
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

}


void CSliderDlg::GroupDetal(int index)
{
	if (!m_curScene)return;
	int ch[CHANNEL_COUNT] = {0};
	int n = 0;
	int curIndex = 0;
	std::vector<LIGHT_UNIT>::iterator light_iter;
	for (light_iter = g_Config.g_light_vec.begin(); light_iter != g_Config.g_light_vec.end(); light_iter++)
	{		
		if ((*light_iter).bGroup && index >= (*light_iter).start_channel&& index  < (*light_iter).start_channel + (*light_iter).channel_count)
		{
			curIndex = index - (*light_iter).start_channel;
			for (int i = (*light_iter).start_channel; i < (*light_iter).start_channel + (*light_iter).channel_count; i++) {
				ch[n++] = m_curScene->channelValue[i];
			}
			break;
		}
	}
	for (light_iter = g_Config.g_light_vec.begin(); light_iter != g_Config.g_light_vec.end(); light_iter++)
	{
		if ((*light_iter).bGroup) {
			for (int i = 0; i < n;i++) {
				if (curIndex == i&&i< (*light_iter).channel_count) {
					m_curScene->channelValue[(*light_iter).start_channel + i] = ch[i];
					M_UpdateData(false);
				}					
			}
		}
	}
}

void CSliderDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int pos = pScrollBar->GetScrollPos();
	bUseUpdate = true;
	switch (nSBCode)
	{
		// 如果向左滚动一列，则pos减1
	case SB_LINELEFT:
		pos -= 1;
		break;
		// 如果向右滚动一列，则pos加1
	//case SB_LINERIGHT:
	//	pos += 1;
	//	break;
	//	// 如果向左滚动一页，则pos减10
	//case SB_PAGELEFT:
	//	pos -= 10;
	//	break;
	//	// 如果向右滚动一页，则pos加10
	case SB_PAGERIGHT:
		pos += 10;
		break;
		// 如果滚动到最左端，则pos为1
	case SB_LEFT:
		pos = 0;
		break;
		// 如果滚动到最右端，则pos为100
	case SB_RIGHT:
		pos = 255;
		break;
		// 如果拖动滚动块到指定位置，则pos赋值为nPos的值
	case SB_THUMBPOSITION:
		pos = nPos;
		break;
		// 下面的m_horiScrollbar.SetScrollPos(pos);执行时
		// 会第二次进入此函数，最终确定滚动块位置，并且会
		// 直接到default分支，所以在此处设置编辑框中显示数值
	case SB_THUMBTRACK:
		pos = nPos;
		break;
	default:
		//SetDlgItemInt(IDC_HSCROLL_EDIT, pos);
		bUseUpdate = false;
		return;
	}
	if (pos>255)pos = 255;
	if (pos < 0)pos = 0;
	CString ShowChannelStr;
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER1:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[0].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+0] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 0);
		break;
	case IDC_SLIDER2:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[1].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+1] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 1);
		break;
	case IDC_SLIDER3:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[2].SetWindowText(ShowChannelStr);
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+2] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 2);
		break;
	case IDC_SLIDER4:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[3].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+3] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 3);
		break;
	case IDC_SLIDER5:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[4].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+4] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 4);
		break;
	case IDC_SLIDER6:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[5].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+5] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 5);
		break;
	case IDC_SLIDER7:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[6].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+6] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 6);
		break;
	case IDC_SLIDER8:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[7].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+7] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 7);
		break;
	case IDC_SLIDER9:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[8].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+8] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 8);
		break;
	case IDC_SLIDER10:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[9].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+9] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 9);
		break;
	case IDC_SLIDER11:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[10].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+10] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 10);
		break;
	case IDC_SLIDER12:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[11].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+11] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 11);
		break;
	case IDC_SLIDER13:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[12].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+12] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 12);
		break;
	case IDC_SLIDER14:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[13].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+13] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 13);
		break;
	case IDC_SLIDER15:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[14].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+14] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 14);
		break;
	case IDC_SLIDER16:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[15].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+15] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 15);
		break;
	case IDC_SLIDER17:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[16].SetWindowText(ShowChannelStr);		
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+16] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 16);
		break;
	case IDC_SLIDER18:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[17].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+17] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 17);
		break;
	case IDC_SLIDER19:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[18].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+18] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 18);
		break;
	case IDC_SLIDER20:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[19].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+19] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 19);
		break;
	case IDC_SLIDER21:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[20].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+20] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 20);
		break;
	case IDC_SLIDER22:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[21].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+21] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 21);
		break;
	case IDC_SLIDER23:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[22].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+22] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 22);
		break;
	case IDC_SLIDER24:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[23].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+23] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 23);
		break;
	case IDC_SLIDER25:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[24].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+24] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 24);
		break;
	case IDC_SLIDER26:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[25].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+25] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 25);
		break;
	case IDC_SLIDER27:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[26].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+26] = 255 - pos; 
		GroupDetal(g_Config.g_startPos + 26);
		break;
	case IDC_SLIDER28:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[27].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+27] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 27);
		break;
	case IDC_SLIDER29:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[28].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+28] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 28);
		break;
	case IDC_SLIDER30:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[29].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+29] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 29);
		break;
	case IDC_SLIDER31:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[30].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+30] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 30);
		break;
	case IDC_SLIDER32:
		ShowChannelStr.Format("%d",255-pos);
		m_EditVar[31].SetWindowText(ShowChannelStr);	
		if (m_curScene)m_curScene->channelValue[g_Config.g_startPos+31] = 255 - pos;
		GroupDetal(g_Config.g_startPos + 31);
		break;
	case IDC_SLIDER_TOTAL:
		ShowChannelStr.Format("%d",255-pos);
		m_edit_total.SetWindowText(ShowChannelStr);	
		g_Config.g_totalChannelValue = 255 - pos;
		break;

	}


	// 设置滚动块位置
	pScrollBar->SetScrollPos(pos);
	::PostMessageA(theApp.GetMainWnd()->m_hWnd,WM_FLUSH_SCROLL,0,NULL);
	bUseUpdate = false;
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	
}


void CSliderDlg::M_UpdateData(bool status)
{
	bUseUpdate = true;
	if (!status)
	{
		if (!m_curScene)return;
		CString ShowChannelStr;
		for (int i=0;i<SHOW_COUNT;i++)
		{
			ShowChannelStr = g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + i];
			m_EditRemark[i].SetWindowText(ShowChannelStr);	

			ShowChannelStr.Format("%d",m_curScene->channelValue[g_Config.g_startPos+i]);
			m_EditVar[i].SetWindowText(ShowChannelStr);

			m_SliderCtrl[i].SetPos(255-m_curScene->channelValue[g_Config.g_startPos+i]);

			ShowChannelStr.Format("%d",g_Config.g_startPos+i+1);
			m_EditChannel[i].SetWindowText(ShowChannelStr);

		}
		UpdateData(false);
	}
	else{
		UpdateData(true);
	}
	bUseUpdate = false;
}


void CSliderDlg::OnBnClickedBtnAllClear()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<CHANNEL_COUNT;i++)
	{
		m_curScene->channelValue[i] = 0;		
	}
	M_UpdateData(false);
}


void CSliderDlg::OnBnClickedBtnAllOn()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<CHANNEL_COUNT;i++)
	{
		m_curScene->channelValue[i] = 255;		
	}
	M_UpdateData(false);
}


void CSliderDlg::OnBnClickedCheckTotal()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void CSliderDlg::OnButtonClicked(UINT uId)
{
	int index = uId - IDC_CHECK1;
	bool bFlag = ((CButton*)GetDlgItem(uId))->GetCheck();
	if (m_curScene)m_curScene->voiceCheck[g_Config.g_startPos + index] = bFlag;
}

void CSliderDlg::OnRemarkEdit(UINT uId)
{
	if (bUseUpdate)return;
	int index = uId - IDC_EDIT_REMARK1;
	CString str;
	((CButton*)GetDlgItem(uId))->GetWindowTextA(str);
	switch (uId)
	{
	case IDC_EDIT_REMARK1:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 0] = str;
		break;
	case IDC_EDIT_REMARK2:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 1] = str;
		break;
	case IDC_EDIT_REMARK3:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 2] = str;
		break;
	case IDC_EDIT_REMARK4:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 3] = str;
		break;
	case IDC_EDIT_REMARK5:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 4] = str;
		break;
	case IDC_EDIT_REMARK6:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 5] = str;
		break;
	case IDC_EDIT_REMARK7:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 6] = str;
		break;
	case IDC_EDIT_REMARK8:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 7] = str;
		break;
	case IDC_EDIT_REMARK9:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 8] = str;
		break;
	case IDC_EDIT_REMARK10:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 9] = str;
		break;
	case IDC_EDIT_REMARK11:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 10] = str;
		break;
	case IDC_EDIT_REMARK12:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 11] = str;
		break;
	case IDC_EDIT_REMARK13:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 12] = str;
		break;
	case IDC_EDIT_REMARK14:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 13] = str;
		break;
	case IDC_EDIT_REMARK15:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 14] = str;
		break;
	case IDC_EDIT_REMARK16:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 15] = str;
		break;
	case IDC_EDIT_REMARK17:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 16] = str;
		break;
	case IDC_EDIT_REMARK18:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 17] = str;
	case IDC_EDIT_REMARK19:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 18] = str;
		break;
	case IDC_EDIT_REMARK20:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 19] = str;
		break;
	case IDC_EDIT_REMARK21:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 20] = str;
		break;
	case IDC_EDIT_REMARK22:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 21] = str;
		break;
	case IDC_EDIT_REMARK23:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 22] = str;
		break;
	case IDC_EDIT_REMARK24:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 23] = str;
		break;
	case IDC_EDIT_REMARK25:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 24] = str;
		break;
	case IDC_EDIT_REMARK26:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 25] = str;
		break;
	case IDC_EDIT_REMARK27:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 26] = str;
		break;
	case IDC_EDIT_REMARK28:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 27] = str;
		break;
	case IDC_EDIT_REMARK29:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 28] = str;
		break;
	case IDC_EDIT_REMARK30:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 29] = str;
		break;
	case IDC_EDIT_REMARK31:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 30] = str;
		break;
	case IDC_EDIT_REMARK32:
		g_Config.g_rigion[g_Config.g_rigion_num].RemarkName[g_Config.g_startPos + 31] = str;
		break;
	default:
		break;
	}
}


void CSliderDlg::OnChannelEdit(UINT uId)
{
	if (bUseUpdate)return;
	int index = 0;
	switch (uId)
	{
	case IDC_EDIT_VAR1:
		index = 0;
		break;
	case IDC_EDIT_VAR2:
		index = 1;
		break;
	case IDC_EDIT_VAR3:
		index = 2;
		break;
	case IDC_EDIT_VAR4:
		index = 3;
		break;
	case IDC_EDIT_VAR5:
		index = 4;
		break;
	case IDC_EDIT_VAR6:
		index = 5;
		break;
	case IDC_EDIT_VAR7:
		index = 6;
		break;
	case IDC_EDIT_VAR8:
		index = 7;
		break;
	case IDC_EDIT_VAR9:
		index = 8;
		break;
	case IDC_EDIT_VAR10:
		index = 9;
		break;
	case IDC_EDIT_VAR11:
		index = 10;
		break;
	case IDC_EDIT_VAR12:
		index = 11;
		break;
	case IDC_EDIT_VAR13:
		index = 12;
		break;
	case IDC_EDIT_VAR14:
		index = 13;
		break;
	case IDC_EDIT_VAR15:
		index = 14;
		break;
	case IDC_EDIT_VAR16:
		index = 15;
		break;
	case IDC_EDIT_VAR17:
		index = 16;
		break;
	case IDC_EDIT_VAR18:
		index = 17;
		break;
	case IDC_EDIT_VAR19:
		index = 18;
		break;
	case IDC_EDIT_VAR20:
		index = 19;
		break;
	case IDC_EDIT_VAR21:
		index = 20;
		break;
	case IDC_EDIT_VAR22:
		index = 21;
		break;
	case IDC_EDIT_VAR23:
		index = 22;
		break;
	case IDC_EDIT_VAR24:
		index = 23;
		break;
	case IDC_EDIT_VAR25:
		index = 24;
		break;
	case IDC_EDIT_VAR26:
		index = 25;
		break;
	case IDC_EDIT_VAR27:
		index = 26;
		break;
	case IDC_EDIT_VAR28:
		index = 27;
		break;
	case IDC_EDIT_VAR29:
		index = 28;
		break;
	case IDC_EDIT_VAR30:
		index = 29;
		break;
	case IDC_EDIT_VAR31:
		index = 30;
		break;
	case IDC_EDIT_VAR32:
		index = 31;
		break;
	}
	CString str;
	((CButton*)GetDlgItem(uId))->GetWindowTextA(str);
	int pos = atoi(str);
	if (m_curScene)m_curScene->channelValue[g_Config.g_startPos + index] = pos;
	m_SliderCtrl[index].SetPos(255 - pos);

	::PostMessageA(theApp.GetMainWnd()->m_hWnd, WM_FLUSH_SCROLL, 0, NULL);
}

BOOL CSliderDlg::PreTranslateMessage(MSG* pMsg)
{
	//屏蔽ESC关闭窗体/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	//屏蔽回车关闭窗体,但会导致回车在窗体上失效.
	//if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN && pMsg->wParam) return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void CSliderDlg::OnOK()
{
	//CDialogEx::OnOK();
}