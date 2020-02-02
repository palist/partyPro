// DlgExportExcelSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SvjSawProject.h"
#include "DlgExportExcelSet.h"
#include "afxdialogex.h"


// CDlgExportExcelSet �Ի���

IMPLEMENT_DYNAMIC(CDlgExportExcelSet, CDialogEx)

CDlgExportExcelSet::CDlgExportExcelSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_EXCEL_EXPORTSET, pParent)
{
	m_pTreeGrid = new CSuperGridCtrl;
	m_strExportName = _T("");
}

CDlgExportExcelSet::~CDlgExportExcelSet()
{
}

void CDlgExportExcelSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTUR_FRAME, m_ImageFrame);
	DDX_Text(pDX, IDC_EXPORT_NAME, m_strExportName);
}


BEGIN_MESSAGE_MAP(CDlgExportExcelSet, CDialogEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EXPORT_NAME, OnChangeExportName)
	ON_MESSAGE(WM_USER_SELECTED_GRID_ITEM, OnUserSelectedGridItem)
	ON_MESSAGE(WM_USER_CHECK, OnUserCheck)

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgExportExcelSet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_TOP, &CDlgExportExcelSet::OnBnClickedBtnTop)
	ON_BN_CLICKED(IDC_BTN_BOTTOM, &CDlgExportExcelSet::OnBnClickedBtnBottom)
	ON_BN_CLICKED(IDC_BTN_UP, &CDlgExportExcelSet::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CDlgExportExcelSet::OnBnClickedBtnDown)
END_MESSAGE_MAP()


// CDlgExportExcelSet ��Ϣ�������
BOOL CDlgExportExcelSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect grid_rc;
	m_ImageFrame.GetWindowRect(grid_rc);
	ScreenToClient(grid_rc);
	CRect new_grid_rc(grid_rc.left + 5, grid_rc.top + 5, grid_rc.right - 5, grid_rc.bottom - 5);
	m_pTreeGrid->MoveWindow(&new_grid_rc);
	FillData();
	return TRUE;
}

//
// �������
//
void CDlgExportExcelSet::FillData()
{
	m_pTreeGrid->DeleteAllItems();
	m_pTreeGrid->DeleteAll();

	LB_Node *node = ::g_export_excel_setting.export_excel_list().head();
	while (node)
	{
		CExportExcel *export_excel = (CExportExcel *)node->entity(); 
		CItemInfo* lp = new CItemInfo();
		lp->SetImage(0);
		lp->SetData(export_excel);
		lp->SetItemText(export_excel->m_strName);
		lp->AddSubItemText(export_excel->m_strExportName); 
		m_pTreeGrid->InsertRootItem(lp);
		 
		node = node->next();
	}

	int nCount = m_pTreeGrid->GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CSuperGridCtrl::CTreeItem *hItem = m_pTreeGrid->GetTreeItem(i);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hItem);

		CExportExcel *pExcel = (CExportExcel *)pInfo->GetData();
		 
		pInfo->SetCheck(pExcel->m_is_export);
		m_pTreeGrid->SetCheck(i, pExcel->m_is_export);
	}
}

int CDlgExportExcelSet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	m_pTreeGrid->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, 0x1007);
	m_pTreeGrid->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	int nCount = m_pTreeGrid->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nCount; i++)
		m_pTreeGrid->DeleteColumn(0);
	m_pTreeGrid->InsertColumn(0, _T("����"), LVCFMT_LEFT, 200);
	m_pTreeGrid->InsertColumn(1, _T("�������"), LVCFMT_LEFT, 200);

	return 0;
}

//
//�޸ĵ�������
void CDlgExportExcelSet::OnChangeExportName()
{
	UpdateData(TRUE);
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel>0)
	{
		CSuperGridCtrl::CTreeItem *hItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hItem);
		CExportExcel *export_excel = (CExportExcel *)pInfo->GetData();
		if (export_excel)
		{
			export_excel->m_strExportName = m_strExportName;
			pInfo->SetSubItemText(0, m_strExportName);
			m_pTreeGrid->SetItemText(nSel, 1, m_strExportName);
		}
	}
}

//
// ѡ��ĳһ��
//
long CDlgExportExcelSet::OnUserSelectedGridItem(WPARAM wParam, LPARAM lParam)
{
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel >= 0)
	{
		CSuperGridCtrl::CTreeItem *hItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hItem);

			CExportExcel *export_excel = (CExportExcel *)pInfo->GetData();
			m_strExportName = export_excel->m_strExportName;
			UpdateData(FALSE);
	
		Invalidate();
	}
	return 0;
}


//
// ��ѡ
//
long CDlgExportExcelSet::OnUserCheck(WPARAM wParam, LPARAM lParam)
{
	CSuperGridCtrl::CTreeItem *hParentItem = (CSuperGridCtrl::CTreeItem *)wParam;
	CItemInfo *parent_info = m_pTreeGrid->GetData(hParentItem);
	BOOL is_check = parent_info->GetCheck();
	POSITION childPOS = m_pTreeGrid->GetHeadPosition(hParentItem);
	if (!childPOS)
	{
		CExportExcel *export_excel = (CExportExcel *)parent_info->GetData();
		parent_info->GetCheck() ? export_excel->m_is_export = 1 : export_excel->m_is_export = 0;
	}
	else
	{
		while (childPOS)
		{
			CSuperGridCtrl::CTreeItem *hItem = m_pTreeGrid->GetNextChild(hParentItem, childPOS);
			CItemInfo *info = m_pTreeGrid->GetData(hItem);
			info->SetCheck(is_check);
			CExportExcel *export_excel = (CExportExcel *)info->GetData();
			is_check ? export_excel->m_is_export = 1 : export_excel->m_is_export = 0;
			if (!m_pTreeGrid->IsCollapsed(hParentItem))
			{
				int nRow = m_pTreeGrid->GetCurIndex(hItem);
				m_pTreeGrid->SetCheck(nRow, is_check);
			}
		}
	}
	return 0;
}


void CDlgExportExcelSet::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pTreeGrid)
	{
		m_pTreeGrid->DestroyWindow();
		delete m_pTreeGrid;
		m_pTreeGrid = NULL;
	}
}

void CDlgExportExcelSet::OnOK()
{
	// TODO: Add extra validation here
	::g_export_excel_setting.SaveSetting();
	CDialogEx::OnOK();
}

void CDlgExportExcelSet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}


//
// �ö�
//
void CDlgExportExcelSet::OnBnClickedBtnTop()
{
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel > 0)
	{
		
		m_nSelIndex = nSel;
		//��ǰ�ڵ�
		CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);

		CExportExcel *pExcel = (CExportExcel *)pInfo->GetData();
		move_node_top_bottom(g_export_excel_setting.export_excel_list(), pExcel, TRUE);
		FillData();

		m_nSelIndex = 0;
		m_pTreeGrid->EnsureVisible(m_nSelIndex, TRUE);
		m_pTreeGrid->SelectedItem(m_nSelIndex);
	}
}

//
// �õ�
//
void CDlgExportExcelSet::OnBnClickedBtnBottom()
{
	int nSel = m_pTreeGrid->GetSelectedItem();
	int count = m_pTreeGrid->GetItemCount();

	if (nSel >= 0  && (nSel <= count-2))
	{
		m_nSelIndex = nSel;
		//��ǰ�ڵ�
		CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);

		CExportExcel *pExcel = (CExportExcel *)pInfo->GetData();
		move_node_top_bottom(g_export_excel_setting.export_excel_list(), pExcel, FALSE);
		FillData();

		
		m_nSelIndex = count - 1;
		m_pTreeGrid->EnsureVisible(m_nSelIndex, TRUE);
		m_pTreeGrid->SelectedItem(m_nSelIndex);
	}
}

//
// �����ƶ�һ��
//
void CDlgExportExcelSet::OnBnClickedBtnUp()
{
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel > 0)
	{
		m_nSelIndex = nSel;
		//��ǰ�ڵ�
		CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);
 
		CExportExcel *pExcel = (CExportExcel *)pInfo->GetData();
		move_node_up_down(g_export_excel_setting.export_excel_list(), pExcel, TRUE);
		FillData();
 
		m_nSelIndex--;
		m_pTreeGrid->EnsureVisible(m_nSelIndex, TRUE);
		m_pTreeGrid->SelectedItem(m_nSelIndex);
	}
}


//
// �����ƶ�һ��
//
void CDlgExportExcelSet::OnBnClickedBtnDown()
{
	int count = m_pTreeGrid->GetItemCount();
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel >= 0 && (nSel < count - 1))
	{
		m_nSelIndex = nSel;
		//��ǰ�ڵ�
		CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);

		CExportExcel *pExcel = (CExportExcel *)pInfo->GetData();
		move_node_up_down(g_export_excel_setting.export_excel_list(), pExcel, FALSE);
		FillData();

		m_nSelIndex++;
		m_pTreeGrid->EnsureVisible(m_nSelIndex, TRUE);
		m_pTreeGrid->SelectedItem(m_nSelIndex);
	}
}


//
//�ö����õ׽ڵ�
void CDlgExportExcelSet::move_node_top_bottom(LB_List &resultList, void *node, BOOL isTop)
{
	//��ȡ��������
	LB_Node *curNode3 = resultList.find_node(node);

	//�ö�
	if (isTop)
	{
		//ֱ��ȡͷ��
		LB_Node *head = resultList.head();

		//�Ȱ���һ���ڵ��nextָ��curNode��next
		LB_Node *curPreNode = curNode3->previous();
		LB_Node *curNextNode = curNode3->next();
		curPreNode->next(curNextNode);
			
		//�����������β�ڵ�Ļ�����Ҫָ����һ���ڵ�
		if (curNextNode)
			curNextNode->previous(curPreNode);

		//Ϊ�գ�˵����β��ָ�룬��ô��Ҫ����ָ��β��
		else
			resultList.tail(curPreNode);

		//��������ͷ���ڵ�
		head->previous(curNode3);

		//����������
		curNode3->next(head);
		curNode3->previous(NULL);

		//����ͷ��
		resultList.head(curNode3);
	}

	//�õ�
	else
	{
		//ȡβ��
		LB_Node *tail_node = resultList.tail();

		//��ǰ�ڵ��pre�� ���Ұѵ�ǰ�ڵ��nextҲȡ����
		LB_Node *pre_node = curNode3->previous();
		LB_Node *next_node = curNode3->next();
		if (pre_node)
			pre_node->next(next_node);

		//���Ϊ�գ�˵����ͷ��ָ�룬��Ҫ����ָ��ͷָ��
		else
			resultList.head(next_node);
		
		next_node->previous(pre_node);

		//����β��ָ��
		tail_node->next(curNode3);

		//������ǰָ��
		curNode3->previous(tail_node);
		curNode3->next(NULL);

		//����β��
		resultList.tail(curNode3);
	}
}


//
//�����ƶ��ڵ�
void CDlgExportExcelSet::move_node_up_down(LB_List &resultList, void *node, BOOL isUp)
{
	//��ȡ��������
	LB_Node *curNode3 = resultList.find_node(node);

	//����
	if (isUp)
	{
		//����1 2 3 4 --> 1 3 2 4
		// 1->next = 3,
		// 2->pre = 3;
		// 2->next = 4;
		// 3->pre = 1;
		// 3->next = 2;
		// 4->pre = 2;
		//��ȡǰһ��B�ڵ�
		LB_Node *preNode2 = curNode3->previous();
		if (preNode2)
		{
			//�ٻ�ȡB�ڵ��ǰһ���ڵ㣬��A�ڵ�
			LB_Node *preNode1 = preNode2->previous();

			//��ǰ�ڵ����һ���ڵ�
			LB_Node *nextNode4 = curNode3->next();

			//��Ҫ����
			if (preNode1)
				preNode1->next(curNode3);
			else
			{
				resultList.head(curNode3); //���ǰ��û�����ݣ���ô�ýڵ���Ҫ���ͷ�ڵ�
				curNode3->previous(NULL);
			}

			curNode3->previous(preNode1);
			curNode3->next(preNode2);
			preNode2->previous(curNode3);

			preNode2->next(nextNode4);

			//����������һ���ڵ㣬��ô����ָ��ǰһ��
			if (nextNode4)
				nextNode4->previous(preNode2);
			else
			{
				resultList.tail(preNode2); //�������û�����ݣ���ôǰһ���ڵ���Ҫ���β�ڵ�
				preNode2->next(NULL);
			}
		}
	}

	//����
	else
	{
		//����1 2 3 4 5 --> 1 2 4 3 5
		// 2->next = 4,
		// 4->pre = 2;
		// 3->pre = 4;
		// 3->next = 5;
		// 4->next = 3;
		// 5->pre = 3;
		LB_Node *node4 = curNode3->next();
		if (node4)
		{
			LB_Node *node2 = curNode3->previous();
			LB_Node *node5 = node4->next();

			if (node2)
			{
				node2->next(node4);
				node4->previous(node2);
			}
			else
			{
				resultList.head(node4); //���ǰ��û�����ݣ���ô�ýڵ���Ҫ���ͷ�ڵ�
				node4->previous(NULL);
			}

			node4->next(curNode3);
			curNode3->previous(node4);
			curNode3->next(node5);

			if (node5)
				node5->previous(curNode3);
			else
			{
				resultList.tail(curNode3); //�������û�����ݣ���ôǰһ���ڵ���Ҫ���β�ڵ�
				curNode3->next(NULL);
			}
		}
	}
}

