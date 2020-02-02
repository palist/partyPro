// DlgPort.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PartyProProgram.h"
#include "DlgPort.h"
#include "afxdialogex.h"
#include "Port.h"


CDlgPort *CDlgPort::m_context = nullptr;
// CDlgPort �Ի���

IMPLEMENT_DYNAMIC(CDlgPort, CDialogEx)


CDlgPort::CDlgPort(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPort::IDD, pParent)
	, m_EditSendStr(_T(""))
	, m_EditRecvStr(_T(""))
{
	m_pTreeGrid = new CSuperGridCtrl();
	m_curData = nullptr;
	port_vec.clear();
}

CDlgPort::~CDlgPort()
{
	delete m_pTreeGrid;
	m_pTreeGrid = nullptr;
	port_vec.swap(std::vector<CPort*>());
	m_curData = nullptr;
}

CDlgPort *CDlgPort::getContext()
{
	if (m_context == nullptr)
		m_context = new CDlgPort;

	return m_context;
}

void CDlgPort::destroyMe()
{
	//
	delete m_context;
	m_context = nullptr;
}


void CDlgPort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RECV, m_EditRecvStr);
	DDX_Text(pDX, IDC_EDIT_SEND, m_EditSendStr);
	DDX_Control(pDX, IDC_STATIC_LIST, m_static_port_zone);
}


BEGIN_MESSAGE_MAP(CDlgPort, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CDlgPort::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDOK, &CDlgPort::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgPort::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_CLEAR2, &CDlgPort::OnBnClickedBtnClear2)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CDlgPort::OnBnClickedBtnConnect)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER_SELECTED_GRID_ITEM, OnUserSeletedItem)
	ON_MESSAGE(WM_USER_CHECK, &CDlgPort::OnUserClickedSeleted)
	ON_MESSAGE(WM_USER_RECE_DATA, &CDlgPort::msgProg)
	ON_BN_CLICKED(IDC_BTN_RESH, &CDlgPort::OnBnClickedBtnResh)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CDlgPort::OnBnClickedBtnOpen)
END_MESSAGE_MAP()


// CDlgPort ��Ϣ�������

int CDlgPort::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pTreeGrid->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, 0x1003);
	m_pTreeGrid->SetExtendedStyle(LVS_EX_GRIDLINES/*| LVS_EX_CHECKBOXES*/);
	int nCount = m_pTreeGrid->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_pTreeGrid->DeleteColumn(0);
	}
	m_pTreeGrid->InsertColumn(0, _T("���"), LVCFMT_LEFT, 80);
	m_pTreeGrid->InsertColumn(1, _T("����"), LVCFMT_LEFT, 80);
	m_pTreeGrid->InsertColumn(2, _T("���ں�"), LVCFMT_LEFT, 100);
	m_pTreeGrid->InsertColumn(3, _T("����״̬"), LVCFMT_LEFT, 80);

	return 0;
}

void CDlgPort::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString sendStr;
	((CEdit*)GetDlgItem(IDC_EDIT_SEND_TEST))->GetWindowTextA(sendStr);
	m_EditSendStr += sendStr+ _T("\r\n");
	((CEdit*)GetDlgItem(IDC_EDIT_SEND))->SetWindowTextA(m_EditSendStr);
	((CEdit*)GetDlgItem(IDC_EDIT_SEND))->LineScroll(((CEdit*)GetDlgItem(IDC_EDIT_SEND))->GetLineCount()-1,0);
	m_curData->sendPort(sendStr);//��������
	
}


void CDlgPort::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->ShowWindow(SW_HIDE);
	//CDialogEx::OnOK();
}
afx_msg LRESULT CDlgPort::msgProg(WPARAM wParam,LPARAM lParam)
{
	CString *str= (CString*)(LPCTSTR)lParam;

	if(wParam==TRUE){
		m_EditRecvStr = *str+ _T("\r\n");
		((CEdit*)GetDlgItem(IDC_EDIT_RECV))->SetWindowTextA(m_EditRecvStr);
		((CEdit*)GetDlgItem(IDC_EDIT_RECV))->LineScroll(((CEdit*)GetDlgItem(IDC_EDIT_RECV))->GetLineCount()-1,0);
		//if(((CEdit*)GetDlgItem(IDC_EDIT_RECV))->GetLineCount()>100)m_EditRecvStr="";
	}
	else{
		m_EditSendStr = *str+ _T("\r\n");
		((CEdit*)GetDlgItem(IDC_EDIT_SEND))->SetWindowTextA(m_EditSendStr);
		((CEdit*)GetDlgItem(IDC_EDIT_SEND))->LineScroll(((CEdit*)GetDlgItem(IDC_EDIT_SEND))->GetLineCount()-1,0);
		//if(((CEdit*)GetDlgItem(IDC_EDIT_SEND))->GetLineCount()>100)m_EditSendStr="";
	}
	
	UpdateData(FALSE);
	return 0;
}

///�������Щ���ڴ���
void CDlgPort::CheckComIsExist(void)
{
	CString strCom;
	HANDLE hCom = NULL;
	for(int i=1;i<99;i++)////���256������ͨ��
	{
		if(i>9)
		{
			strCom.Format(_T("\\\\.\\COM%d"),i);///����9���ϵĴ��ڣ��ڴ���ʱ��Ҫ�ر��ʽ
		}
		else
		{
			strCom.Format(_T("COM%d"),i);//����9�Ĵ��ڣ��ڴ���ʱ��һ���ʽ
		}
		hCom =CreateFile(strCom,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);//�򿪴���
		if(hCom ==INVALID_HANDLE_VALUE)///������ش��󣬱�ʾ���ڲ�����
		{
			hCom =NULL;continue;
		}
		if(strCom.GetLength() >5)///�ַ���������5������ʾ������9����
		{
			strCom = strCom.Mid(4,strCom.GetLength());///ȡ����ͨ��������
		}
		CloseHandle(hCom);hCom =NULL;///�����Ϊ�رմ���
		
									 
		//�����б�
		CPort* pData = new CPort;
		pData->m_region = port_vec.size() + 1;
		pData->m_com_port = strCom;
		pData->m_isOpen = FALSE;
		port_vec.push_back(pData);

	}
}
void CDlgPort::OnBnClickedBtnClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_EditSendStr = "";
	UpdateData(FALSE);
}


void CDlgPort::OnBnClickedBtnClear2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_EditRecvStr = "";
	UpdateData(FALSE);
}



void CDlgPort::OnBnClickedBtnConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(!m_curData)return;
	if(m_curData->m_isOpen)
	{
		m_curData->DisconnectPort();
		m_curData->m_isOpen = false;
		((CButton*)GetDlgItem(IDC_BTN_CONNECT))->SetWindowTextA(_T("����"));
	}
	else{
		bool flag = m_curData->ConnectPort();
		if(!flag)return;
		m_curData->m_isOpen = true;
		((CButton*)GetDlgItem(IDC_BTN_CONNECT))->SetWindowTextA(_T("�Ͽ�����"));
	}
	OnInitDialog();
}



//
BOOL CDlgPort::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	int index = 0;
	m_pTreeGrid->DeleteAll();
	for (vec_iter = port_vec.begin(); vec_iter < port_vec.end(); )
	{
		if ((*vec_iter)->m_isOpen == false)
		{
			port_vec.erase(vec_iter);
			vec_iter = port_vec.begin();
			continue;
		}
		vec_iter++;
	}
	CheckComIsExist();
	for(vec_iter = port_vec.begin();vec_iter<port_vec.end();vec_iter++)
	{
		CPort* pData = *vec_iter;
		//m_curData = pData;
		CItemInfo *info = new CItemInfo;
		info->SetImage(2);
		info->SetData(pData);

		str.Format(_T("%d"), ++index);
		info->SetItemText(str);

		str.Format(_T("��%d"), pData->m_region);
		info->AddSubItemText(str);

		info->AddSubItemText(pData->m_com_port);

		info->AddSubItemText(pData->m_isOpen==0?_T("δ����"):_T("������"));

		m_pTreeGrid->InsertRootItem(info);
	}
	CRect grid_rc1;
	m_static_port_zone.GetWindowRect(grid_rc1);
	ScreenToClient(grid_rc1);
	grid_rc1.top += 5;
	grid_rc1.bottom -= 5;
	m_pTreeGrid->MoveWindow(&grid_rc1);

	((CEdit*)GetDlgItem(IDC_EDIT_SEND_TEST))->SetWindowTextA("AB 05 01 BA");

	return TRUE;
}

void CDlgPort::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);


	if (m_pTreeGrid && m_pTreeGrid->GetSafeHwnd()&& m_static_port_zone)
	{
		CRect grid_rc1;
		m_static_port_zone.GetWindowRect(grid_rc1);
		ScreenToClient(grid_rc1);
		//grid_rc1.top += 5;
		//grid_rc1.bottom -= 5;
		m_pTreeGrid->MoveWindow(&grid_rc1);
	}

}

BOOL CDlgPort::PreTranslateMessage(MSG* pMsg)
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



LRESULT CDlgPort::OnUserSeletedItem(WPARAM wParam, LPARAM lParam)
{
	m_pTreeGrid = (CSuperGridCtrl *)lParam;

	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel >= 0)
	{
		CSuperGridCtrl::CTreeItem *hItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hItem);
		m_curData = (CPort *)pInfo->GetData();
		if(!m_curData->m_isOpen)
		{
			((CButton*)GetDlgItem(IDC_BTN_CONNECT))->SetWindowTextA(_T("����"));

		}
		else{
			((CButton*)GetDlgItem(IDC_BTN_CONNECT))->SetWindowTextA(_T("�Ͽ�����"));
		}
		

	}
	return 0;
}


LRESULT CDlgPort::OnUserClickedSeleted(WPARAM wParam, LPARAM lParam)
{
	m_pTreeGrid = (CSuperGridCtrl *)lParam;
	
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel >= 0)
	{
		CSuperGridCtrl::CTreeItem *hItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hItem);
		m_curData = (CPort *)pInfo->GetData();
		bool checkStatus = (bool)m_pTreeGrid->GetCheck(nSel);
		if (checkStatus&&!m_curData->m_isOpen)
		{				
			m_curData->ConnectPort();
			m_pTreeGrid->SetItemText(nSel,3,_T("������"));
		}
		else if (!checkStatus&&m_curData->m_isOpen){
			m_curData->DisconnectPort();
			m_pTreeGrid->SetItemText(nSel,3,_T("δ����"));
		}
	}
	return 0;
}


void CDlgPort::OnBnClickedBtnResh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	OnInitDialog();
}


void CDlgPort::OnBnClickedBtnOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	static CRect rectLarge;//���治�ı�ĶԻ����λ��
	static CRect rectSmall;//����ı��˵ĶԻ����λ��
	if (rectLarge.IsRectNull())
	{
		CRect rectSeparator;
		GetWindowRect(&rectLarge);
		GetDlgItem(IDC_STATIC_LIST)->GetWindowRect(&rectSeparator);
		rectSmall.left = rectLarge.left;
		rectSmall.top = rectLarge.top;
		rectSmall.right = rectLarge.right;
		rectSmall.bottom = rectSeparator.bottom+15;
	}
	CString str;
	GetDlgItem(IDC_BTN_OPEN)->GetWindowTextA(str);
	if (str == "����<<")
	{
		SetWindowPos(NULL, 0, 0, rectSmall.Width(), rectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
		GetDlgItem(IDC_BTN_OPEN)->SetWindowTextA("չ��>>");
	}
	else
	{
		SetWindowPos(NULL, 0, 0, rectLarge.Width(), rectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);//���ô���λ��
		GetDlgItem(IDC_BTN_OPEN)->SetWindowTextA("����<<");
	}

}
