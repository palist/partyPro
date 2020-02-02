// AddRamMaterial.cpp : 实现文件
//

#include "stdafx.h"
#include "..//Profiles.h"
#include "AddRamMaterial.h"
#include "afxdialogex.h"
#include "supergridctrl.h"
#include "../systemConfig.h"

// CAddRamMaterial 对话框

IMPLEMENT_DYNAMIC(CAddRamMaterial, CDialogEx)

CAddRamMaterial::CAddRamMaterial(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_RAW_MATERIAL, pParent)
{
	m_pTreeGrid = new CSuperGridCtrl();
	m_pTreeGridDoorCore = new CSuperGridCtrl();
}

CAddRamMaterial::~CAddRamMaterial()
{
	delete m_pTreeGrid;
	m_pTreeGrid = nullptr;
	delete m_pTreeGridDoorCore;
	m_pTreeGridDoorCore = nullptr;
}

void CAddRamMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SUBITEM, m_strItemTextEdit);
	DDX_Control(pDX, IDC_EDIT_SUBITEM, m_SubItemEdit);
	DDX_Control(pDX, IDC_COMBO_ISROTATE, m_isRotate);
	DDX_Control(pDX, IDC_STATIC_PROFILE_ZONE, m_profile_zone);
	DDX_Control(pDX, IDC_STATIC_DOORCORE_ZONE, m_doorcore_zone);
}


BEGIN_MESSAGE_MAP(CAddRamMaterial, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ADD, &CAddRamMaterial::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CAddRamMaterial::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALL, &CAddRamMaterial::OnBnClickedBtnDeleteAll)
	ON_BN_CLICKED(IDC_BTN_ADD2, &CAddRamMaterial::OnBnClickedBtnAdd2)
	ON_BN_CLICKED(IDC_BTN_DELETE2, &CAddRamMaterial::OnBnClickedBtnDelete2)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALL2, &CAddRamMaterial::OnBnClickedBtnDeleteAll2)
	//ON_MESSAGE(WM_USER_DBUTTON_DOWN, OnUserSeletedItem)
	ON_MESSAGE(WM_USER_SELECTED_GRID_ITEM, OnUserSeletedItem)
	ON_MESSAGE(WM_USER_DBLCLK, &CAddRamMaterial::OnNMDblclkList)
	ON_EN_KILLFOCUS(IDC_EDIT_SUBITEM, &CAddRamMaterial::OnEnKillfocusGreeitemEdit)
	ON_BN_CLICKED(IDOK, &CAddRamMaterial::OnBnClickedOk)
	ON_CBN_KILLFOCUS(IDC_COMBO_ISROTATE, &CAddRamMaterial::OnCbnKillfocusComboKnife)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CAddRamMaterial::OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CAddRamMaterial::OnBnClickedBtnExport)
	ON_BN_CLICKED(IDC_BTN_IMPORT2, &CAddRamMaterial::OnBnClickedBtnImport2)
	ON_BN_CLICKED(IDC_BTN_EXPORT2, &CAddRamMaterial::OnBnClickedBtnExport2)
END_MESSAGE_MAP()


// CAddRamMaterial 消息处理程序


int CAddRamMaterial::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pTreeGrid->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, 0x1003);
	m_pTreeGrid->SetExtendedStyle(LVS_EX_GRIDLINES);
	int nCount = m_pTreeGrid->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_pTreeGrid->DeleteColumn(0);
	}
	m_pTreeGrid->InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(1, _T("物料编码"), LVCFMT_LEFT, 80);
	m_pTreeGrid->InsertColumn(2, _T("基材颜色"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(3, _T("截面"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(4, _T("物料名称"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(5, _T("物料材质"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(6, _T("长度(mm)"), LVCFMT_LEFT, 70);
	m_pTreeGrid->InsertColumn(7, _T("宽度(mm)"), LVCFMT_LEFT, 70);
	m_pTreeGrid->InsertColumn(8, _T("厚度(mm)"), LVCFMT_LEFT, 70);
	m_pTreeGrid->InsertColumn(9, _T("数量"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(10, _T("单价"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(11, _T("是否可旋转"), LVCFMT_LEFT, 70);
	m_pTreeGrid->InsertColumn(12, _T("切割刀具"), LVCFMT_LEFT, 60);
	m_pTreeGrid->InsertColumn(13, _T("切角长度"), LVCFMT_LEFT, 60);
	

	//////////////////////////////第二门芯原材料////////////////////////////////////////////

	m_pTreeGridDoorCore->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, 0x1003);
	m_pTreeGridDoorCore->SetExtendedStyle(LVS_EX_GRIDLINES);
	int nDoorCoreCount = m_pTreeGridDoorCore->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nDoorCoreCount; i++)
	{
		m_pTreeGridDoorCore->DeleteColumn(0);
	}
	m_pTreeGridDoorCore->InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60);
	m_pTreeGridDoorCore->InsertColumn(1, _T("物料编码"), LVCFMT_LEFT, 80);
	m_pTreeGridDoorCore->InsertColumn(2, _T("基材颜色"), LVCFMT_LEFT, 60);
	m_pTreeGridDoorCore->InsertColumn(3, _T("截面"), LVCFMT_LEFT, 60);
	m_pTreeGridDoorCore->InsertColumn(4, _T("物料名称"), LVCFMT_LEFT, 60);
	m_pTreeGridDoorCore->InsertColumn(5, _T("物料材质"), LVCFMT_LEFT, 60);
	m_pTreeGridDoorCore->InsertColumn(6, _T("长度(mm)"), LVCFMT_LEFT, 70);
	m_pTreeGridDoorCore->InsertColumn(7, _T("宽度(mm)"), LVCFMT_LEFT, 70);
	m_pTreeGridDoorCore->InsertColumn(8, _T("厚度(mm)"), LVCFMT_LEFT, 70);
	m_pTreeGridDoorCore->InsertColumn(9, _T("数量"), LVCFMT_LEFT, 60);
	m_pTreeGridDoorCore->InsertColumn(10, _T("单价"), LVCFMT_LEFT, 60);
	m_pTreeGridDoorCore->InsertColumn(11, _T("是否可旋转"), LVCFMT_LEFT, 70);
	m_pTreeGridDoorCore->InsertColumn(12, _T("切割刀具"), LVCFMT_LEFT, 60);
	m_pTreeGridDoorCore->InsertColumn(13, _T("切角长度"), LVCFMT_LEFT, 60);


	return 0;
}

//
BOOL CAddRamMaterial::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	int index = 0;
	m_pTreeGrid->DeleteAll();

	LB_Node* node = CSystemConfig::getContext()->g_material_list.head();
	while (node)
	{
		CMaterialData* pData = (CMaterialData*)node->entity();
		m_curData = pData;
		CItemInfo *info = new CItemInfo;
		info->SetImage(0);
		info->SetData(pData);

		str.Format("%d", ++index);
		info->SetItemText(str);

		info->AddSubItemText(pData->get_material());

		info->AddSubItemText(pData->get_color());

		info->AddSubItemText(pData->get_partNumber());

		info->AddSubItemText(pData->get_name());

		info->AddSubItemText(pData->get_texture());

		//长度
		str.Format("%.1f", pData->get_length());
		info->AddSubItemText(str);

		str.Format("%.1f", pData->get_width());
		info->AddSubItemText(str);

		str.Format("%.1f", pData->get_thickness());
		info->AddSubItemText(str);


		//数量，旋转
		str.Format("%d", pData->get_count());
		info->AddSubItemText(str);

		//价格
		str.Format("%.1f", pData->get_money());
		info->AddSubItemText(str);

		//旋转
		info->AddSubItemText(pData->get_isRotate()?_T("是"):_T("否"));

		info->AddSubItemText(pData->get_knifeName());

		str.Format("%.1f", pData->get_cutLen());
		info->AddSubItemText(str);


		m_pTreeGrid->InsertRootItem(info);

		node = node->next();
	}
	CRect grid_rc1;
	m_profile_zone.GetWindowRect(grid_rc1);
	ScreenToClient(grid_rc1);
	grid_rc1.top += 20;
	grid_rc1.bottom -= 40;
	m_pTreeGrid->MoveWindow(&grid_rc1);

	//////////////////////////////////////////////////////////////////////////

	m_pTreeGridDoorCore->DeleteAll();
	index = 0;
	LB_Node* doorCoreNode = CSystemConfig::getContext()->g_door_core_material_list.head();
	while (doorCoreNode)
	{
		CDoorCoreMaterial* pData = (CDoorCoreMaterial*)doorCoreNode->entity();
		m_curDataDoorCore = pData;
		CItemInfo *info = new CItemInfo;
		info->SetImage(0);
		info->SetData(pData);

		str.Format("%d", ++index);
		info->SetItemText(str);

		info->AddSubItemText(pData->get_material());

		info->AddSubItemText(pData->get_color());

		info->AddSubItemText(pData->get_partNumber());

		info->AddSubItemText(pData->get_name());

		info->AddSubItemText(pData->get_texture());

		//长度
		str.Format("%.1f", pData->get_length());
		info->AddSubItemText(str);

		str.Format("%.1f", pData->get_width());
		info->AddSubItemText(str);

		str.Format("%.1f", pData->get_thickness());
		info->AddSubItemText(str);


		//数量，旋转
		str.Format("%d", pData->get_count());
		info->AddSubItemText(str);

		//价格
		str.Format("%.1f", pData->get_money());
		info->AddSubItemText(str);

		//旋转
		info->AddSubItemText(pData->get_isRotate() ? _T("是") : _T("否"));

		info->AddSubItemText(pData->get_knifeName());

		str.Format("%.1f", pData->get_cutLen());
		info->AddSubItemText(str);


		m_pTreeGridDoorCore->InsertRootItem(info);

		doorCoreNode = doorCoreNode->next();
	}


	CRect grid_rc2;
	m_doorcore_zone.GetWindowRect(grid_rc2);
	ScreenToClient(grid_rc2);
	grid_rc2.top += 20;
	grid_rc2.bottom -= 40;
	m_pTreeGridDoorCore->MoveWindow(&grid_rc2);

	m_isRotate.ResetContent();
	m_isRotate.AddString(_T("是"));
	m_isRotate.AddString(_T("否"));

	return TRUE;
}

void CAddRamMaterial::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	
	if (m_pTreeGrid && m_pTreeGrid->GetSafeHwnd()&& m_profile_zone)
	{
		CRect grid_rc1;
		m_profile_zone.GetWindowRect(grid_rc1);
		ScreenToClient(grid_rc1);
		grid_rc1.top += 20;
		grid_rc1.bottom -= 40;
		m_pTreeGrid->MoveWindow(&grid_rc1);
	}
	if (m_pTreeGridDoorCore && m_pTreeGridDoorCore->GetSafeHwnd()&& m_doorcore_zone)
	{
		CRect grid_rc2;
		m_doorcore_zone.GetWindowRect(grid_rc2);
		ScreenToClient(grid_rc2);
		grid_rc2.top += 20;
		grid_rc2.bottom -= 40;
		m_pTreeGridDoorCore->MoveWindow(&grid_rc2);
	}

}

BOOL CAddRamMaterial::PreTranslateMessage(MSG* pMsg)
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


void CAddRamMaterial::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CMaterialData* pData = new CMaterialData;
	pData->set_material(_T("material"));
	pData->set_color(_T("白枫林"));
	pData->set_partNumber(_T("aerbeisix"));
	pData->set_name(_T("cut_1"));
	pData->set_texture(_T("铝合金"));
	pData->set_length(6000);
	pData->set_width(100);
	pData->set_thickness(100);
	pData->set_count(1000);
	pData->set_money(1);
	pData->set_isRotate(false);
	pData->set_knifeName(_T("knife1"));
	pData->set_cutLen(5.0);

	CSystemConfig::getContext()->g_material_list.add(pData);

	OnInitDialog();
}


void CAddRamMaterial::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_pTreeGrid->GetSelectedItem();
	if (nSel >= 0)
	{
		CSuperGridCtrl::CTreeItem *hItem = m_pTreeGrid->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hItem);
		CMaterialData *pData = (CMaterialData *)pInfo->GetData();
		CSystemConfig::getContext()->g_material_list.removeValue(pData);
		
		delete pData;
		pData = nullptr;
	}
	OnInitDialog();
}


void CAddRamMaterial::OnBnClickedBtnDeleteAll()
{
	// TODO: 在此添加控件通知处理程序代码
	delete_void_list(CSystemConfig::getContext()->g_material_list, CMaterialData);
	OnInitDialog();
}


void CAddRamMaterial::OnBnClickedBtnAdd2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDoorCoreMaterial* pData = new CDoorCoreMaterial;
	pData->set_material(_T("material"));
	pData->set_color(_T("白枫林"));
	pData->set_partNumber(_T("aerbeisix"));
	pData->set_name(_T("cut_1"));
	pData->set_texture(_T("铝合金"));
	pData->set_length(6000);
	pData->set_width(100);
	pData->set_thickness(100);
	pData->set_count(1000);
	pData->set_money(1);
	pData->set_isRotate(false);
	pData->set_knifeName(_T("knife1"));
	pData->set_cutLen(5.0);

	CSystemConfig::getContext()->g_door_core_material_list.add(pData);

	OnInitDialog();
}


void CAddRamMaterial::OnBnClickedBtnDelete2()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_pTreeGridDoorCore->GetSelectedItem();
	if (nSel >= 0)
	{
		CSuperGridCtrl::CTreeItem *hItem = m_pTreeGridDoorCore->GetTreeItem(nSel);
		CItemInfo *pInfo = (CItemInfo *)m_pTreeGridDoorCore->GetData(hItem);
		CDoorCoreMaterial *pData = (CDoorCoreMaterial *)pInfo->GetData();
		CSystemConfig::getContext()->g_door_core_material_list.removeValue(pData);

		delete pData;
		pData = nullptr;
	}
	OnInitDialog();
}


void CAddRamMaterial::OnBnClickedBtnDeleteAll2()
{
	// TODO: 在此添加控件通知处理程序代码
	delete_void_list(CSystemConfig::getContext()->g_door_core_material_list, CDoorCoreMaterial);
	OnInitDialog();
}


LRESULT CAddRamMaterial::OnUserSeletedItem(WPARAM wParam, LPARAM lParam)
{
	m_pCurGrid = (CSuperGridCtrl *)lParam;

	if (m_pCurGrid == m_pTreeGrid)
	{
		int nSel = m_pTreeGrid->GetSelectedItem();
		if (nSel >= 0)
		{
			CSuperGridCtrl::CTreeItem *hItem = m_pTreeGrid->GetTreeItem(nSel);
			CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hItem);
			m_curData = (CMaterialData *)pInfo->GetData();
		}
	}
	else if (m_pCurGrid == m_pTreeGridDoorCore)
	{
		int nDoorCoreSel = m_pTreeGridDoorCore->GetSelectedItem();
		if (nDoorCoreSel >= 0)
		{
			CSuperGridCtrl::CTreeItem *hItem = m_pTreeGridDoorCore->GetTreeItem(nDoorCoreSel);
			CItemInfo *pInfo = (CItemInfo *)m_pTreeGridDoorCore->GetData(hItem);
			m_curDataDoorCore = (CDoorCoreMaterial *)pInfo->GetData();
		}
	}
	
	return 0;
}


//双击时，可修改数据
LRESULT CAddRamMaterial::OnNMDblclkList(WPARAM wParam, LPARAM lParam)
{
	m_iSubItem = (int)wParam;
	m_iItem = (int)lParam;

	if (m_iItem == -1)
		return 0;
	//获取列表上的文本，显示到Edit控件里
	CString itemTxt;;

	//调整Edit的宽、高，并移动
	CRect rect, dlgRect;
	int width;

	if (m_pCurGrid == m_pTreeGrid)
	{
		itemTxt = m_pTreeGrid->GetItemText(m_iItem, m_iSubItem);
		width = m_pTreeGrid->GetColumnWidth(m_iSubItem);
		m_pTreeGrid->GetWindowRect(&dlgRect);
		m_pTreeGrid->GetSubItemRect(m_iItem, m_iSubItem, LVIR_BOUNDS, rect);
	}
	else {
		itemTxt = m_pTreeGridDoorCore->GetItemText(m_iItem, m_iSubItem);
		width = m_pTreeGridDoorCore->GetColumnWidth(m_iSubItem);
		m_pTreeGridDoorCore->GetWindowRect(&dlgRect);
		m_pTreeGridDoorCore->GetSubItemRect(m_iItem, m_iSubItem, LVIR_BOUNDS, rect);

	}


	
	ScreenToClient(&dlgRect);
	int height = rect.Height();
	rect.top += (dlgRect.top );
	rect.left += (dlgRect.left);
	rect.bottom = rect.top + height;
	rect.right = rect.left + width;

	if (m_iSubItem == 11)
	{
		m_isRotate.SetWindowText(itemTxt);
		::SetWindowPos(m_isRotate.GetSafeHwnd(), HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height() + 100, SWP_SHOWWINDOW);
		m_isRotate.ShowWindow(SW_SHOW);

		if (itemTxt == "是")
			m_isRotate.SetCurSel(0);
		else {
			m_isRotate.SetCurSel(1);
		}

	}
	else
	{
		m_SubItemEdit.SetWindowText(itemTxt);
		::SetWindowPos(m_SubItemEdit.GetSafeHwnd(), HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		m_SubItemEdit.ShowWindow(SW_SHOW);
		m_SubItemEdit.SetFocus();
	}

	return 0;
}

//
//编辑完后修改相应数据
void CAddRamMaterial::OnEnKillfocusGreeitemEdit()
{
	UpdateData(TRUE);
	if (m_pCurGrid == m_pTreeGrid)
	{
		int nSel = m_pTreeGrid->GetSelectedItem();
		if (nSel >= 0)
		{
			CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
			CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);
			CMaterialData *pData = (CMaterialData *)pInfo->GetData();

			//修改
			m_pTreeGrid->SetItemText(m_iItem, m_iSubItem, m_strItemTextEdit);

			if (m_iSubItem == 1)
				pData->set_material(m_strItemTextEdit);
			else if (m_iSubItem == 2) {
				pData->set_color(m_strItemTextEdit);
			}
			else if (m_iSubItem == 3) {
				pData->set_partNumber(m_strItemTextEdit);
			}
			else if (m_iSubItem == 4) {
				pData->set_name(m_strItemTextEdit);
			}
			else if (m_iSubItem == 5) {
				pData->set_texture(m_strItemTextEdit);
			}
			else if (m_iSubItem == 6) {
				int val = atof(m_strItemTextEdit);
				pData->set_length(val);
			}
			else if (m_iSubItem == 7) {
				int val = atof(m_strItemTextEdit);
				pData->set_width(val);
			}
			else if (m_iSubItem == 8) {
				int val = atof(m_strItemTextEdit);
				pData->set_thickness(val);
			}
			else if (m_iSubItem == 9) {
				int val = atoi(m_strItemTextEdit);
				pData->set_count(val);
			}
			else if (m_iSubItem == 10) {
				double val = atof(m_strItemTextEdit);
				pData->set_money(val);
			}
			else if (m_iSubItem == 11) {
				bool val = atoi(m_strItemTextEdit);
				pData->set_isRotate(val);
			}
			else if (m_iSubItem == 12) {
				pData->set_knifeName(m_strItemTextEdit);
			}
			else if (m_iSubItem == 13) {
				int val = atof(m_strItemTextEdit);
				pData->set_cutLen(val);
			}
			m_pTreeGrid->Invalidate();
		}

	}

	//////////////////////////////////////////////////////////////////////////
	else
	{
		int nDoorCoreSel = m_pTreeGridDoorCore->GetSelectedItem();
		if (nDoorCoreSel >= 0)
		{
			CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGridDoorCore->GetTreeItem(nDoorCoreSel);
			CItemInfo *pInfo = (CItemInfo *)m_pTreeGridDoorCore->GetData(hCurItem);
			CDoorCoreMaterial *pData = (CDoorCoreMaterial *)pInfo->GetData();

			//修改
			m_pTreeGridDoorCore->SetItemText(m_iItem, m_iSubItem, m_strItemTextEdit);

			if (m_iSubItem == 1)
				pData->set_material(m_strItemTextEdit);
			else if (m_iSubItem == 2) {
				pData->set_color(m_strItemTextEdit);
			}
			else if (m_iSubItem == 3) {
				pData->set_partNumber(m_strItemTextEdit);
			}
			else if (m_iSubItem == 4) {
				pData->set_name(m_strItemTextEdit);
			}
			else if (m_iSubItem == 5) {
				pData->set_texture(m_strItemTextEdit);
			}
			else if (m_iSubItem == 6) {
				int val = atof(m_strItemTextEdit);
				pData->set_length(val);
			}
			else if (m_iSubItem == 7) {
				int val = atof(m_strItemTextEdit);
				pData->set_width(val);
			}
			else if (m_iSubItem == 8) {
				int val = atof(m_strItemTextEdit);
				pData->set_thickness(val);
			}
			else if (m_iSubItem == 9) {
				int val = atoi(m_strItemTextEdit);
				pData->set_count(val);
			}
			else if (m_iSubItem == 10) {
				double val = atof(m_strItemTextEdit);
				pData->set_money(val);
			}
			else if (m_iSubItem == 11) {
				bool val = atoi(m_strItemTextEdit);
				pData->set_isRotate(val);
			}
			else if (m_iSubItem == 12) {
				pData->set_knifeName(m_strItemTextEdit);
			}
			else if (m_iSubItem == 13) {
				int val = atof(m_strItemTextEdit);
				pData->set_cutLen(val);
			}
			m_pTreeGridDoorCore->Invalidate();
		}
	}

	//
	m_SubItemEdit.ShowWindow(SW_HIDE);
	
}


void CAddRamMaterial::OnCbnKillfocusComboKnife()
{
	if (m_pCurGrid == NULL)
		return;

	UpdateData(TRUE);
	if (m_pCurGrid == m_pTreeGrid)
	{
		int nSel = m_pTreeGrid->GetSelectedItem();
		if (nSel >= 0)
		{
			CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGrid->GetTreeItem(nSel);
			CItemInfo *pInfo = (CItemInfo *)m_pTreeGrid->GetData(hCurItem);
			CMaterialData *pData = (CMaterialData *)pInfo->GetData();

			//修改
			CString str;
			m_isRotate.GetWindowTextA(str);
			m_pTreeGrid->SetItemText(m_iItem, m_iSubItem, str);
			if (m_iSubItem == 11)
				pData->set_isRotate(str.Compare(_T("是")) == 0 ? true : false);
			m_pTreeGrid->SetFocus();
			m_pTreeGrid->Invalidate();
		}

	}
	//////////////////////////////////////////////////////////////////////////
	else
	{
		int nDoorCoreSel = m_pTreeGridDoorCore->GetSelectedItem();
		if (nDoorCoreSel >= 0)
		{
			CSuperGridCtrl::CTreeItem *hCurItem = m_pTreeGridDoorCore->GetTreeItem(nDoorCoreSel);
			CItemInfo *pInfo = (CItemInfo *)m_pTreeGridDoorCore->GetData(hCurItem);
			CDoorCoreMaterial *pData = (CDoorCoreMaterial *)pInfo->GetData();

			//修改
			CString str;
			m_isRotate.GetWindowTextA(str);
			m_pTreeGridDoorCore->SetItemText(m_iItem, m_iSubItem, str);
			if (m_iSubItem == 11)
				pData->set_isRotate(str.Compare(_T("是")) == 0 ? true : false);
			m_pTreeGridDoorCore->SetFocus();
			m_pTreeGridDoorCore->Invalidate();

		}
	}

	m_isRotate.ShowWindow(SW_HIDE);
	
}


void CAddRamMaterial::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CSystemConfig::getContext()->save();
	CDialogEx::OnOK();
}


void CAddRamMaterial::OnBnClickedBtnImport()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog   dlg(TRUE,
		"xls",
		"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("xls文件(*.xls)|*.xls;*.xlsx||"), NULL);
	dlg.m_ofn.lpstrInitialDir = ".";

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		api_read_material_excel_file(strPath);
		OnInitDialog();
	}
}


void CAddRamMaterial::OnBnClickedBtnExport()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CAddRamMaterial::OnBnClickedBtnImport2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CAddRamMaterial::OnBnClickedBtnExport2()
{
	// TODO: 在此添加控件通知处理程序代码
}
