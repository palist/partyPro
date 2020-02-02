// DlgExportExcelSet.cpp : 实现文件
//

#include "stdafx.h"
#include "SvjSawProject.h"
#include "DlgExportExcelSet.h"
#include "afxdialogex.h"


// CDlgExportExcelSet 对话框

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


// CDlgExportExcelSet 消息处理程序
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
// 填充数据
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
	m_pTreeGrid->InsertColumn(0, _T("属性"), LVCFMT_LEFT, 200);
	m_pTreeGrid->InsertColumn(1, _T("输出名称"), LVCFMT_LEFT, 200);

	return 0;
}

//
//修改导出名称
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
// 选择某一行
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
// 勾选
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
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


//
// 置顶
//
void CDlgExportExcelSet::OnBnClickedBtnTop()
{
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel > 0)
	{
		
		m_nSelIndex = nSel;
		//当前节点
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
// 置底
//
void CDlgExportExcelSet::OnBnClickedBtnBottom()
{
	int nSel = m_pTreeGrid->GetSelectedItem();
	int count = m_pTreeGrid->GetItemCount();

	if (nSel >= 0  && (nSel <= count-2))
	{
		m_nSelIndex = nSel;
		//当前节点
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
// 向上移动一行
//
void CDlgExportExcelSet::OnBnClickedBtnUp()
{
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel > 0)
	{
		m_nSelIndex = nSel;
		//当前节点
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
// 向下移动一行
//
void CDlgExportExcelSet::OnBnClickedBtnDown()
{
	int count = m_pTreeGrid->GetItemCount();
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel >= 0 && (nSel < count - 1))
	{
		m_nSelIndex = nSel;
		//当前节点
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
//置顶、置底节点
void CDlgExportExcelSet::move_node_top_bottom(LB_List &resultList, void *node, BOOL isTop)
{
	//获取代码链表
	LB_Node *curNode3 = resultList.find_node(node);

	//置顶
	if (isTop)
	{
		//直接取头部
		LB_Node *head = resultList.head();

		//先把上一个节点的next指向curNode的next
		LB_Node *curPreNode = curNode3->previous();
		LB_Node *curNextNode = curNode3->next();
		curPreNode->next(curNextNode);
			
		//并且如果不是尾节点的话，需要指向上一个节点
		if (curNextNode)
			curNextNode->previous(curPreNode);

		//为空，说明是尾部指针，那么需要重新指定尾部
		else
			resultList.tail(curPreNode);

		//再来操作头部节点
		head->previous(curNode3);

		//最后操作自身
		curNode3->next(head);
		curNode3->previous(NULL);

		//设置头部
		resultList.head(curNode3);
	}

	//置底
	else
	{
		//取尾部
		LB_Node *tail_node = resultList.tail();

		//当前节点的pre， 并且把当前节点的next也取出来
		LB_Node *pre_node = curNode3->previous();
		LB_Node *next_node = curNode3->next();
		if (pre_node)
			pre_node->next(next_node);

		//如果为空，说明是头部指针，需要重新指定头指针
		else
			resultList.head(next_node);
		
		next_node->previous(pre_node);

		//操作尾部指针
		tail_node->next(curNode3);

		//操作当前指针
		curNode3->previous(tail_node);
		curNode3->next(NULL);

		//设置尾部
		resultList.tail(curNode3);
	}
}


//
//上下移动节点
void CDlgExportExcelSet::move_node_up_down(LB_List &resultList, void *node, BOOL isUp)
{
	//获取代码链表
	LB_Node *curNode3 = resultList.find_node(node);

	//上移
	if (isUp)
	{
		//对于1 2 3 4 --> 1 3 2 4
		// 1->next = 3,
		// 2->pre = 3;
		// 2->next = 4;
		// 3->pre = 1;
		// 3->next = 2;
		// 4->pre = 2;
		//获取前一个B节点
		LB_Node *preNode2 = curNode3->previous();
		if (preNode2)
		{
			//再获取B节点的前一个节点，即A节点
			LB_Node *preNode1 = preNode2->previous();

			//当前节点的下一个节点
			LB_Node *nextNode4 = curNode3->next();

			//需要处理
			if (preNode1)
				preNode1->next(curNode3);
			else
			{
				resultList.head(curNode3); //如果前面没有数据，那么该节点需要变成头节点
				curNode3->previous(NULL);
			}

			curNode3->previous(preNode1);
			curNode3->next(preNode2);
			preNode2->previous(curNode3);

			preNode2->next(nextNode4);

			//如果不是最后一个节点，那么还得指向前一个
			if (nextNode4)
				nextNode4->previous(preNode2);
			else
			{
				resultList.tail(preNode2); //如果后面没有数据，那么前一个节点需要变成尾节点
				preNode2->next(NULL);
			}
		}
	}

	//下移
	else
	{
		//对于1 2 3 4 5 --> 1 2 4 3 5
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
				resultList.head(node4); //如果前面没有数据，那么该节点需要变成头节点
				node4->previous(NULL);
			}

			node4->next(curNode3);
			curNode3->previous(node4);
			curNode3->next(node5);

			if (node5)
				node5->previous(curNode3);
			else
			{
				resultList.tail(curNode3); //如果后面没有数据，那么前一个节点需要变成尾节点
				curNode3->next(NULL);
			}
		}
	}
}

