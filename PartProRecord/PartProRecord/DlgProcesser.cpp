// DlgProcesser.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "Record.h"
#include "DlgProcesser.h"


// CDlgProcesser �Ի���

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


// CDlgProcesser ��Ϣ�������

BOOL CDlgProcesser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Proc.SetStep(1);   ///���ò���
	m_Proc.SetRange(0,100);///���ý�������Χ
	SetTimer(110,50,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgProcesser::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
