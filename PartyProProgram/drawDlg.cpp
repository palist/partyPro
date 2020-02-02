
// drawDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "drawDlg.h"
#include "PartyProProgram.h"
#include <afxwin.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CdrawDlg 对话框

CdrawDlg* CdrawDlg::m_context = nullptr;

CdrawDlg* CdrawDlg::getContext(CWnd* pParent)
{
	if (m_context == nullptr)
		m_context = new CdrawDlg(pParent);

	return m_context;
}

void CdrawDlg::destroyMe()
{
	//
	delete m_context;
	m_context = nullptr;
}


CdrawDlg::CdrawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CdrawDlg::IDD, pParent)
	, m_x_pos(128)
	, m_y_pos(128)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_parent = (CPartyProProgramDlg*)pParent;

}

void CdrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_x_pos);
	DDX_Text(pDX, IDC_EDIT_Y, m_y_pos);
}

BEGIN_MESSAGE_MAP(CdrawDlg, CDialog) 
	ON_WM_PAINT() 
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CdrawDlg::OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_EDIT_X, &CdrawDlg::OnEnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, &CdrawDlg::OnEnChangeEditY)
	ON_BN_CLICKED(IDC_BTN_OK, &CdrawDlg::OnBnClickedBtnOk)
END_MESSAGE_MAP()




// CdrawDlg 消息处理程序

BOOL CdrawDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//网格绘制区域
	m_draw.init(this, IDC_STATIC_RECT);

	//开始、结束范围控件、切换
	m_draw.set_ctrl(IDC_EDIT_X, IDC_EDIT_Y);

	return TRUE; 
}
 

void CdrawDlg::OnPaint()
{
	CPaintDC dc(this); 
	m_draw.on_paint(&dc);
}

void CdrawDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_draw.mouse_up(point);
	CDialog::OnLButtonUp(nFlags, point);
}

void CdrawDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_draw.mouse_down(point);
	CDialog::OnLButtonDown(nFlags, point);
}

void CdrawDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	m_draw.mouse_move(point);
	CDialog::OnMouseMove(nFlags, point);
}


void CdrawDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CdrawDlg::OnEnChangeEditX()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	m_parent->GroupDetal_xy(m_x_pos, m_y_pos);
}


void CdrawDlg::OnEnChangeEditY()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	m_parent->GroupDetal_xy(m_x_pos, m_y_pos);
}




void CdrawDlg::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CDialog::OnOK();
}
