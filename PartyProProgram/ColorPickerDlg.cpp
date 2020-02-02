
// ColorPickerDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ColorPickerDlg.h"


// CColorPickerDlg �Ի���
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
	// ��ʼ�� һ old_color���ǵ�ǰѡ�е���ɫ���ⲿ����ȫ�ֱ�����¼����
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

	//�����¼�  --- start
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE() 
	ON_WM_PAINT() 

	//�������������¼�
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_R, &CColorPickerDlg::OnNMCustomdrawSliderR)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_G, &CColorPickerDlg::OnNMCustomdrawSliderG)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_B, &CColorPickerDlg::OnNMCustomdrawSliderB)
	//�����¼�  --- end

	ON_BN_CLICKED(IDOK, &CColorPickerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CColorPickerDlg ��Ϣ�������

BOOL CColorPickerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ʼ����ʼ -- 
	m_event.StartInit(IDC_STATIC_HSB_RECT,  //��ɫ����Static_ID
		IDC_STATIC_OLD,						//����ɫ��Static_ID
		IDC_STATIC_NEW,						//��ѡ�����ɫ��Static_ID
		IDB_BITMAP_HSB);					//BMPͼƬID

	//RGBֵ��ʾ�� -- ��ӦIDC_Edit�ؼ�
	m_event.InitRGBEdit(IDC_EDIT_R_VAL, IDC_EDIT_G_VAL, IDC_EDIT_B_VAL);


	//���ÿؼ���ֵ -- ��������������ؼ�ID
	m_event.InitSpinCtrl(IDC_SLIDER_R, IDC_SLIDER_G, IDC_SLIDER_B);


	//��ʼ������
	m_event.EndInit();


	Invalidate();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

//��ɫ��ק�¼�
void CColorPickerDlg::OnNMCustomdrawSliderR(NMHDR *pNMHDR, LRESULT *pResult)
{
	int r = ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_R))->GetPos();

	//����Rֵ
	m_event.UpdateRColor(r);  
	//UpdateData(true);
	//m_parent->GroupDetal_color(m_red, m_green, m_blue);
}

//��ɫ��ק�¼�
void CColorPickerDlg::OnNMCustomdrawSliderG(NMHDR *pNMHDR, LRESULT *pResult)
{
	int g = ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_G))->GetPos();

	//����Gֵ
	m_event.UpdateGColor(g);
	//UpdateData(true);
	//m_parent->GroupDetal_color(m_red, m_green, m_blue);
}


//��ɫ��ק�¼�
void CColorPickerDlg::OnNMCustomdrawSliderB(NMHDR *pNMHDR, LRESULT *pResult)
{
	int b = ((CSliderCtrl *)GetDlgItem(IDC_SLIDER_B))->GetPos();

	//����Bֵ
	m_event.UpdateBColor(b); 
	//UpdateData(true);
	//m_parent->GroupDetal_color(m_red, m_green, m_blue);
}

// ---- ����¼� start
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
// ---- ����¼� end

//�ػ��¼�
void CColorPickerDlg::OnPaint() 
{
	CPaintDC dc(this); 

	m_event.onPaint(&dc);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CColorPickerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ȡ���
void CColorPickerDlg::OnBnClickedOk()
{
	UpdateData(true);
	CDialog::OnOK();
}


