// DlgProcesser.cpp : 实现文件
//

#include "stdafx.h"
//#include "Record.h"
#include "DlgProcesser.h"


// CDlgProcesser 对话框

IMPLEMENT_DYNAMIC(CDlgProcesser, CDialogEx)

	CDlgProcesser::CDlgProcesser(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProcesser::IDD, pParent)
{

}

CDlgProcesser::~CDlgProcesser()
{
}

void CDlgProcesser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Proc);
}


BEGIN_MESSAGE_MAP(CDlgProcesser, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgProcesser 消息处理程序

BOOL CDlgProcesser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Proc.SetStep(1);   ///设置步长
	m_Proc.SetRange(0,100);///设置进度条范围
	SetTimer(110,50,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgProcesser::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Proc.StepIt();
	if(m_Proc.GetPos() ==100)
	{
		m_Proc.SetPos(0);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgProcesser::CreateProDlg(int flag)
{
}
