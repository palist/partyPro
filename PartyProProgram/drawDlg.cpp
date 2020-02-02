
// drawDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "drawDlg.h"
#include "PartyProProgram.h"
#include <afxwin.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CdrawDlg �Ի���

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




// CdrawDlg ��Ϣ�������

BOOL CdrawDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//�����������
	m_draw.init(this, IDC_STATIC_RECT);

	//��ʼ��������Χ�ؼ����л�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CdrawDlg::OnEnChangeEditX()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_parent->GroupDetal_xy(m_x_pos, m_y_pos);
}


void CdrawDlg::OnEnChangeEditY()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_parent->GroupDetal_xy(m_x_pos, m_y_pos);
}




void CdrawDlg::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CDialog::OnOK();
}
