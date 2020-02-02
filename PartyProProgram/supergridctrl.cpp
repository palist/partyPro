// supergridctrl.cpp : implementation file
//

#include "stdafx.h"
#include "supergridctrl.h"
#include <afxtempl.h>
#include "resource.h"

/*
    SuperGrid, YALC (yet another listview control) 

	Written by me(Allan Nielsen), that is .. if the code works :)
		mailto:allan@object-oriented.com

	Copyright (c) 1999.

	If you find bugs, please notify me and I'll contact the author who wrote the darn thing :)
	
    You may use it, abuse it, redistribute it, in any way you desire, but
	if you choose to use this control in any commercial or non commercial application then please 
	send me an email letting me know, makes me :) . 
	

	HISTORY 

	what's new since last update: 

	4 dec. 1998.
	
				- MeasureItem now uses current font	
				- Combobox support in CItemInfo				
				  see the ShowList in MySuperGrid.h/cpp, PreTranslateMessage functions
				  and the file ComboInListView.cpp/h
                - function to indicate the precens of a combobox it draws a down arrow ..see DrawComboBox
				  the idea came from Robert Bouwens. :)
				- in file TestTreeDlg.cpp:  due to the MeasureItem message-reflection	
				  the listctrl is now created in the CDialog::OnCreate function and later on resized
				- LVIS_DROPHILITED support when dragging items
				- Support for LVS_EX_CHECKBOXES	  
				- Added some examples on how to print preview selected item(or at least to get some information from of the grid)
				  the sample is in the CMySuperGrid.h/cpp which is derived from CSuperGridCtrl it shows you
				  how to initalize the grid, sort items, search for items and subitems, select and delete thems
				- added virtual GetIcon function to set your default icon handler for a listview index.
				- added virtual GetCellRGB() to set the color for current selected cell
				- renamed CTreeNode to CTreeItem, sorry ;-(
				- the nested class CTreeItem has been stripped down to nothing but simple (struct)data members
				  only servering as a linked list of lists now :-)
				- well.. moved all the importent stuff from CTreeItem to CSuperGridCtrl e.g
				  all operations on a CTreeItem now resides in the CSuperGridCtrl, much more OO-like ;-)
				- added Quicksort function
				- got rid of TheOpenCloseThing function.
				- added virtual function OnUpdateListViewItem..called when ever an Item is about to be updated
				- added virtual function OnControlLButtonDown...called when ever LButtondown in a cell
				- added virtual function CreateDragImageEx...

	11 jan 1999:
				- added SetFocus in OnDblclk and in OnLButtonDown, big thank you to Dieter Gollwitzer 
				- fixed a minor hittest error in the HitTestOnSign(), again thanks to Dieter Gollwitzer 


	16 jan 1999: 
				- added virtual OnItemExpanding(....
				- added virtual OnItemExpanded(...
				- added virtual OnCollapsing(....
				- added virtual OnItemCollapsed(...
				- added virtual OnDeleteItem(....
				- added virtual OnVKMultiply(...
				- added virtual OnVkSubTract(..
				- added virtual OnVKAdd(....
				- added SetChildrenFlag();

	1 feb 1999:
				- CItemInfo now supports individual cell-color
				- fixed some bugs in regards to listdata associated with in each cell
				- added virtual BOOL OnVkReturn(........
				- added virtual BOOL OnItemLButtonDown(.....

	4 mai 1999  - moved to Switzerland to work as a systemdeveloper for 2-3 years :).
				- new email :)
				- added new class for encapsulating drawing + - buttons.

   22.june 1999 - Added GetCount()
				- removed support for VC++ 5.0.
				- added GetTreeItem(.....) 
				- GetSelectedItem(.....)
				- Howto disable Drag/drop in CMySuperGrid
				- level 4 compiled
				- Clean up some redudant code crap

	30.june 99  - fixed minor error in DrawItem, a drawing error occurred when 1.st column was hidden, 
				  it was previous fixed, but some how it was introduced again :).
	
	
	9. juli 99  - CRAIG SCHMIDT "the beachboy":)

				  Craig did the implementation of the Multiple Root request
				  his email are "craig_a_schmidt@yahoo.com" if you have any Questions
				  in regards to multiple roots implementation, although I think I can help too :).

				  he added the following functions :

				  - CTreeItem*  InsertRootItem(.....
				  - void DeleteRootItem(....
				  - BOOL IsRoot(...
				  - CTreeItem* GetRootItem(..
				  - int GetRootIndex(..
				  - int GetRootCount(...
				  - and a member variable, CPtrList m_RootItems which of course are a collection of ..ta da RootItems a.k.a CTreeItem.
				  see the file CMySuperGrid.cpp for usage of the InsertRootItem.

   	10.juli 99    added simple wrappers for the rootitems collection

				  -	Added GetRootHeadPosition(...)
				  -	Added GetRootTailPosition();
				  -	Added CTreeItem* GetNextRoot(...
				  -	Added CTreeItem* GetPrevRoot(...


	12. juli 99  - fixed minor error in CreateDragImageEx, an error occurred when 1st column was hidden and a drag operation was initiated

	13. juli 99	 - Added void DeleteAll()...deletes all items in the grid, uhh
				 - Removed GetNextSiblingItem(...), GetPrevSiblingItem(...) due to support for multiple roots. 
				 - Added CTreeItem *GetNext(...) and CTreeItem* GetPrev(..) due to support for multiple roots.

	14. juli 99	 - Fixed a memory leak in CMySuperGrid::HowToInsertItemsAfterTheGridHasBeenInitialized....


	
	WHAT'S UP :
				- Vacation
				- ATL VERSION COMMING UP.
				- Better documentation :)
				- Peace on earth.
				- service pack 4 for VC++ 6.0 :=|
				- Windows 3000 :)
*/

/////////////////////////////////////////////////////////////////////////////
// CSuperGridCtrl
CSuperGridCtrl::CSuperGridCtrl()
{
	m_cxImage = m_cyImage= 0;
 	m_psTreeLine.CreatePen(PS_SOLID, 1, RGB(192,192,192));
	m_psRectangle.CreatePen(PS_SOLID, 1, RGB(198,198,198));
	m_psPlusMinus.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	m_brushErase.CreateSolidBrush(RGB(255,255,255));
	m_himl = NULL;
}


CSuperGridCtrl::~CSuperGridCtrl()
{
	m_psPlusMinus.DeleteObject();
	m_psRectangle.DeleteObject();
	m_psTreeLine.DeleteObject();
	m_brushErase.DeleteObject();

	while(m_RootItems.GetCount())
	{
		CTreeItem * root = (CTreeItem*)m_RootItems.RemoveHead();
		if(root!=NULL && GetData(root) != NULL)
			delete GetData(root);
		delete root;
	}
	m_RootItems.RemoveAll();
}



BEGIN_MESSAGE_MAP(CSuperGridCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSuperGridCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CSuperGridCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperGridCtrl message handlers

BOOL CSuperGridCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_REPORT | LVS_SINGLESEL | LVS_SHAREIMAGELISTS | LVS_OWNERDRAWFIXED | LVS_SHOWSELALWAYS;	
	return CListCtrl::PreCreateWindow(cs);
}



int CSuperGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(!m_ImageList.Create(IDB_BITMAP1,16,1,RGB(0, 0x80, 0x80)))
		return -1;
	SetImageList(&m_ImageList, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;

	return 0;
}




void CSuperGridCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (lpDrawItemStruct->CtlType != ODT_LISTVIEW)
        return;

	if(lpDrawItemStruct->itemAction == ODA_DRAWENTIRE)
	{
		if(m_himl==NULL)
		{
			m_himl = (HIMAGELIST)::SendMessage(m_hWnd, LVM_GETIMAGELIST, (WPARAM)(int)(LVSIL_SMALL), 0L);
			if(m_himl==NULL)
				return;
		}

		LV_ITEM lvi;
		static _TCHAR szBuff[MAX_PATH];
		LPCTSTR pszText;
	
		int nItem = lpDrawItemStruct->itemID;
		CRect rcItem(lpDrawItemStruct->rcItem);

		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_INDENT | LVIF_PARAM; 
		lvi.iItem = nItem;
		lvi.iSubItem =0;
		lvi.pszText = szBuff;
		lvi.cchTextMax = sizeof(szBuff);
		lvi.stateMask = 0xFFFF;		
		GetItem(&lvi);

		CTreeItem *pSelItem = (CTreeItem*)lpDrawItemStruct->itemData;

		CRect rcLabel;
		GetItemRect(nItem, rcLabel, LVIR_LABEL);
		
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		ASSERT(pDC);

		CRect rcClipBox;
		pDC->GetClipBox(rcClipBox);

		COLORREF crBackground, crText;
		
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			// Set the text background and foreground colors
			crBackground = GetSysColor (COLOR_HIGHLIGHT);
			crText = GetSysColor (COLOR_HIGHLIGHTTEXT);
		}
		else
		{
			// Set the text background and foreground colors to the standard window
			// colors
			crBackground = GetSysColor (COLOR_WINDOW);
			crText = GetSysColor (COLOR_WINDOWTEXT);
		}

		//Get current Icon, you have overridden this I hope :-)
		int iImage = GetIcon(pSelItem);
		if(iImage!=-1)
		{
			if(lvi.iImage!=iImage)
			{
				LV_ITEM lvItem;				
				lvItem.mask =  LVIF_IMAGE;
				lvItem.iImage = iImage;
				lvItem.iItem = nItem;
				lvItem.iSubItem = 0;
				SetItem(&lvItem);
				lvi.iImage = iImage; 
			}
		}

		CRect rc;
		GetItemRect(nItem, rc, LVIR_BOUNDS);

		CRect rcIcon;
		GetItemRect(nItem, rcIcon, LVIR_ICON);
		//Draw Current image
		int nOffset = (rcItem.Height() - m_cyImage)/2;
		int nY = rcItem.bottom - m_cyImage - nOffset;
		int nWidth = m_cxImage;//def icon size
		//do not draw icon out side 1.st column.
		int iTryIndent = GetIndent(pSelItem) * m_cxImage + m_cxImage;
		if(rc.left + iTryIndent  > GetColumnWidth(0)-2/*looks better -2*/)
			nWidth = (rc.left + iTryIndent) - GetColumnWidth(0);

		UINT uiFlags = ILD_TRANSPARENT;

		if( GetItemState(nItem, LVIF_STATE) & LVIS_DROPHILITED)//if dragging show a SelectDropTarget alike effect :)
			uiFlags |= ILD_BLEND50;
		
		if((nWidth=m_cxImage-nWidth) >-1)//calc width of icon
		{
			ImageList_DrawEx(m_himl, lvi.iImage, pDC->m_hDC,  
							 rc.left + (GetIndent(pSelItem) * m_cxImage), 
							 nY, 
							 nWidth,	
							 m_cyImage,
							 CLR_DEFAULT, 
							 CLR_DEFAULT, 
							 uiFlags);
			DrawTreeItem(pDC, pSelItem, nItem, rc);
		}

		//Draw selection bar (erase old selection too)
		pDC->SetBkColor(crBackground);

		CRect rcClip = lpDrawItemStruct->rcItem;
		rcClip.left += GetIndent(pSelItem) * m_cxImage + m_cxImage + 2;
		if(rcClip.left > GetColumnWidth(0))
			rcClip.left = GetColumnWidth(0);
		//fill background color
		ExtTextOut(pDC->GetSafeHdc(), 0, 0, ETO_OPAQUE, rcClip, NULL, 0, NULL);
		
		//draw color in first col if any
		rcClip.right = rcLabel.right;
		CItemInfo *pItemInfo = GetData(pSelItem);
		COLORREF clf = pItemInfo->GetItemClr();
		if(clf!=-1)
		{	
			CBrush br(clf);
			pDC->FillRect(rcClip, &br);
		}
		//draw selection rect in 1.st col if selected
		if ((lpDrawItemStruct->itemState & ODS_SELECTED) && (m_CurSubItem==0))
		{
			CBrush br(GetCellRGB());
			pDC->FillRect(rcClip,&br);
			pDC->DrawFocusRect(rcClip);
		}

		//if checkbox style
		UINT nStateImageMask = lvi.state & LVIS_STATEIMAGEMASK;
		if (nStateImageMask)
		{
			int nImage = (nStateImageMask>>12) - 1;
			CImageList *pImageList = GetImageList(LVSIL_STATE);
			if (pImageList)
			{
				int cxIcon,cyIcon=0;
				ImageList_GetIconSize(pImageList->m_hImageList, &cxIcon, &cyIcon);
				if(rc.left + (GetIndent(pSelItem) * m_cxImage) + m_cxImage + cxIcon < GetColumnWidth(0))
					pImageList->Draw(pDC, nImage,CPoint(rc.left + (GetIndent(pSelItem) * m_cxImage) + cxIcon, nY), ILD_TRANSPARENT);
			}
		}


		//draw 1. item	
		GetItemRect(nItem, rcItem, LVIR_LABEL);
		pszText = MakeShortString(pDC, szBuff, rcItem.right - rcItem.left, 2*OFFSET_FIRST);
		rcLabel = rcItem;
		rcLabel.left+=OFFSET_FIRST;
		rcLabel.right-=OFFSET_FIRST;
		
		pDC->SetBkColor (crBackground);
		crText=pItemInfo->GetTextColor();
		pDC->SetTextColor (crText);
		pDC->DrawText(pszText,-1, rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);

		//draw subitems..
		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH;
		for(int nColumn=1; GetColumn(nColumn, &lvc); nColumn++)
		{
			rcItem.left=rcItem.right;
			rcItem.right+=lvc.cx;
			if (rcItem.left < rcClipBox.right && rcItem.right > rcClipBox.left && rcItem.right > rcItem.left)
			{
				//support for colors in each cell
				COLORREF clf = pItemInfo->GetBkColor(nColumn-1);
				if(clf!=-1)
				{	
					CBrush br(clf);
					pDC->FillRect(rcItem, &br);
				}
				
				int nRetLen = GetItemText(nItem, nColumn, szBuff, sizeof(szBuff));
				if(nRetLen==0)
					pszText=NULL;
				else
					pszText=MakeShortString(pDC,szBuff,rcItem.right-rcItem.left,2*OFFSET_OTHER);

				UINT nJustify=DT_LEFT;

				if(pszText==szBuff)
				{
					switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
					{
					case LVCFMT_RIGHT:
						nJustify=DT_RIGHT;
						break;
					case LVCFMT_CENTER:
						nJustify=DT_CENTER;
						break;
					default:
						break;
					}
				}
				rcLabel=rcItem;
				rcLabel.left+=OFFSET_OTHER;
				rcLabel.right-=OFFSET_OTHER;
				if (lpDrawItemStruct->itemState & ODS_SELECTED)
					DrawFocusCell(pDC, lpDrawItemStruct->itemID, nColumn);
				if(pszText!=NULL)
					pDC->DrawText(pszText,-1,rcLabel, nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);
			}//if

			//draw down arrow if combobox regardless of rcClipBox rgn
			//old code modify by hgf
	//		if (lpDrawItemStruct->itemState & ODS_SELECTED)
	//			DrawComboBox(pDC, pSelItem, nItem, nColumn);
		}//for
	}//ODA_DRAWENTIRE
}



void CSuperGridCtrl::DrawComboBox(CDC* pDC, CTreeItem *pSelItem, int nItem, int nColumn)
{
	CItemInfo* pInfo = GetData(pSelItem);
	CItemInfo::CONTROLTYPE ctrlType;
	if(pInfo->GetControlType(nColumn-1, ctrlType))
	{
		if(ctrlType == pInfo->CONTROLTYPE::combobox) 
		{
			CRect rect;
			GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, rect);
			rect.left=rect.right - GetSystemMetrics(SM_CYVSCROLL);
			pDC->DrawFrameControl(rect, DFC_SCROLL, DFCS_SCROLLDOWN);
		}
	}
}


//this piece of code is borrowed from the wndproc.c file in the odlistvw.exe example from MSDN and was converted to mfc-style
void CSuperGridCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    if (lpMeasureItemStruct->CtlType != ODT_LISTVIEW)
        return;

	TEXTMETRIC tm;
	CClientDC dc(this);	
	CFont* pFont = GetFont();
	CFont* pOldFont = dc.SelectObject(pFont);	
	dc.GetTextMetrics(&tm);
	int nItemHeight = tm.tmHeight + tm.tmExternalLeading;
	lpMeasureItemStruct->itemHeight = nItemHeight + 4; //or should I go for max(nItemheight+4, m_cxImage+2);
	dc.SelectObject(pOldFont);
}



//the basic rutine making the ... thing snatched it from some tedious code example some where in MSDN call odlistvw.exe
LPCTSTR CSuperGridCtrl::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
{
	static const _TCHAR szThreeDots[]=_T("...");

	int nStringLen=lstrlen(lpszLong);

	if(nStringLen==0 || pDC->GetTextExtent(lpszLong,nStringLen).cx + nOffset < nColumnLen)
		return(lpszLong);

	static _TCHAR szShort[MAX_PATH];

	lstrcpy(szShort,lpszLong);
	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	for(int i=nStringLen-1; i > 0; i--)
	{
		szShort[i]=0;
		if(pDC->GetTextExtent(szShort,i).cx + nOffset + nAddLen < nColumnLen)
			break;
	}
	lstrcat(szShort,szThreeDots);
	return(szShort);
}



void CSuperGridCtrl::InitializeOrderGrid()
{
    int nCount = GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < nCount;i++)
	{
		DeleteColumn(0);
	}
	InsertColumn(0, _T("柜体名称"), LVCFMT_LEFT, 130);
	InsertColumn(1, _T("客户名称"), LVCFMT_LEFT, 100);
	InsertColumn(2, _T("板件名称"), LVCFMT_LEFT, 100);
	InsertColumn(3, _T("板件ID"), LVCFMT_LEFT, 100);
	InsertColumn(4, _T("数量"), LVCFMT_LEFT, 35);
	InsertColumn(5, _T("材质颜色"), LVCFMT_LEFT, 80);
	InsertColumn(6, _T("长度"), LVCFMT_LEFT, 70);
	InsertColumn(7, _T("宽度"), LVCFMT_LEFT, 70);
	InsertColumn(8, _T("厚度"), LVCFMT_LEFT, 60);
	InsertColumn(9, _T("旋转"), LVCFMT_LEFT, 60);
	//InsertColumn(10, _T("纹路"), LVCFMT_LEFT, 80);
	//InsertColumn(11, StringID(ID_DAOJU_STRING), LVCFMT_LEFT, 50);
}

void CSuperGridCtrl::InitializeHoleGrid()
{
	int nCount = GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < nCount;i++)
	{
		DeleteColumn(0);
	}
	InsertColumn(0,_T("编号"),LVCFMT_LEFT,60);
	InsertColumn(1,_T("钻头号"),LVCFMT_LEFT,70);
	InsertColumn(2,_T("孔类型"),LVCFMT_LEFT,80);
	InsertColumn(3,_T("直径"),LVCFMT_LEFT,60);
	InsertColumn(4,_T("孔深"),LVCFMT_LEFT,60);
	InsertColumn(5,_T("位置X"),LVCFMT_LEFT,60);
	InsertColumn(6,_T("位置Y"),LVCFMT_LEFT,60);
	InsertColumn(7,_T("位置Z"),LVCFMT_LEFT,60);

}

void CSuperGridCtrl::InitializeGCodeGrid()
{
	int nCount = GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < nCount;i++)
	{
		DeleteColumn(0);
	}
	InsertColumn(0,_T("行号"),LVCFMT_LEFT,80);
	InsertColumn(1,_T("代码"),LVCFMT_LEFT,300);
}

void CSuperGridCtrl::FillGrid(CStringArray &strArray)
{
	int nSize=strArray.GetSize();
	for(int i=0;i<nSize;i++)
	{
        CString strIndex;
		strIndex.Format(_T("%d"),i+1);
		CItemInfo* lp = new CItemInfo();
		lp->SetImage(7);
		lp->SetData(NULL);
		lp->SetItemText(strIndex);
		lp->AddSubItemText(strArray[i]);
		InsertRootItem(lp);
	}
}

void CSuperGridCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	switch(pLVKeyDow->wVKey)
	{
		case VK_SPACE: 
			{
				if(GetExtendedStyle() & LVS_EX_CHECKBOXES)
				{	
					int nIndex = GetSelectedItem();
					if(nIndex !=-1)
					{
						CTreeItem* pItem = GetTreeItem(nIndex);
						if(pItem!=NULL)
						{
							CItemInfo *pInfo = GetData(pItem);
							pInfo->SetCheck(!pInfo->GetCheck());
						}
					}
				}
			}break;
		case VK_MULTIPLY:
			{  
				int nIndex = GetSelectedItem();
				if(nIndex != -1)
				{
					CWaitCursor wait;
					SetRedraw(0);
					CTreeItem *pParent = GetTreeItem(nIndex);
					int nScroll=0;
					if(OnVKMultiply(pParent, nIndex))
					{	
						ExpandAll(pParent, nScroll);
					}		
					SetRedraw(1);
					RedrawItems(nIndex, nScroll);
					EnsureVisible(nScroll, TRUE);
				 }
			 }break;

		case VK_ADD:
			{
					int nIndex = GetSelectedItem();
					if(nIndex!=-1)
					{
						CWaitCursor wait;
						CTreeItem *pSelItem = GetTreeItem(nIndex);
						int nScrollIndex = 0;
						if(OnVKAdd(pSelItem, nIndex))
						{
							 nScrollIndex = Expand(pSelItem, nIndex);
						}
						CRect rc;
						GetItemRect(nIndex, rc, LVIR_BOUNDS);
						InvalidateRect(rc);
						UpdateWindow();
						EnsureVisible(nScrollIndex, 1);
					}

			}break;


		case VK_SUBTRACT:
			{
				int nIndex = GetSelectedItem();
				if(nIndex!=-1)
				{
					CWaitCursor wait;
					CTreeItem *pSelItem = GetTreeItem(nIndex);
					if(OnVkSubTract(pSelItem, nIndex))
					{
						Collapse(pSelItem);
					}
					CRect rc;
					GetItemRect(nIndex, rc, LVIR_BOUNDS);
					InvalidateRect(rc);
					UpdateWindow();
				}
			}break;
		default :break;
	}
	*pResult = 0;
}


BOOL CSuperGridCtrl::HitCheck(CTreeItem *hHitItem)
{
    return TRUE;
}

void CSuperGridCtrl::OnUpdateCheckState()
{

}

BOOL CSuperGridCtrl::HitTestOnSign(CPoint point, LVHITTESTINFO& ht)
{
	ht.pt = point;
	// Test which subitem was clicked.
	SubItemHitTest(&ht);
	if(ht.iItem!=-1)
	{
		//first hittest on checkbox
		BOOL bHit = FALSE;
		if(GetExtendedStyle() & LVS_EX_CHECKBOXES)
		{
			if (ht.flags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))//isn't this allways ownerdrawfixed :-)
			{
				CRect rcIcon,rcLabel;
				GetItemRect(ht.iItem, rcIcon, LVIR_ICON);//has to be between these two ....right :)
				GetItemRect(ht.iItem, rcLabel, LVIR_LABEL);
				// check if hit was on a state icon 
				if (point.x > rcIcon.left && point.x < rcLabel.left)
					bHit = TRUE;
			}
			else if (ht.flags & LVHT_ONITEMSTATEICON)
				bHit = TRUE;
		}

		CTreeItem* pItem = GetTreeItem(ht.iItem);
		if(pItem!=NULL)
		{
			if(bHit)//if checkbox
			{
				//yes I know..have to maintain to sets of checkstates here...
				//one for listview statemask and one for CTreeItem..but its located here so no harm done
				CItemInfo *pInfo = GetData(pItem);
				if(HitCheck(pItem))
				{
			//		SetCheck(ht.iItem,!GetCheck(ht.iItem));
					
					pInfo->SetCheck(!pInfo->GetCheck());
					SetCheck(ht.iItem,pInfo->GetCheck());
					GetParent()->SendMessage(WM_USER_CHECK,(WPARAM)pItem,(LPARAM)this);
				}
			}
			//if haschildren and clicked on + or - then expand/collapse
			if(ItemHasChildren(pItem))
			{
				//hittest on the plus/sign "button" 
				CRect rcBounds;
				GetItemRect(ht.iItem, rcBounds, LVIR_BOUNDS);
				CRectangle rect(this, NULL, GetIndent(pItem), rcBounds);
				BOOL bRedraw=0;//if OnItemExpanding or OnCollapsing returns false, dont redraw
				if(rect.HitTest(point))
				{
					SetRedraw(0);
					int nScrollIndex=0;
					if(IsCollapsed(pItem))
					{	
						if(OnItemExpanding(pItem, ht.iItem))
						{
							nScrollIndex = Expand(pItem, ht.iItem);
							OnItemExpanded(pItem, ht.iItem);
							bRedraw=1;
						}
					}	
					else {
					   if(OnCollapsing(pItem))
					   {
							Collapse(pItem);
							OnItemCollapsed(pItem);
							bRedraw=1;
					   }
					}
					SetRedraw(1);
					if(bRedraw)
					{
						CRect rc;
						GetItemRect(ht.iItem, rc, LVIR_BOUNDS);
						InvalidateRect(rc);
						UpdateWindow();
						EnsureVisible(nScrollIndex, 1);
						return 0;
					}	
				}
			}//has kids
		}//pItem!=NULL
	}
	OnUpdateCheckState();
	return 1;
}



void CSuperGridCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;  
	if( GetFocus() != this) 
		SetFocus();
	if(GetParent())
		GetParent()->SendMessage(WM_USER_DBLCLK,pNMListCtrl->iSubItem,pNMListCtrl->iItem);
		//GetParent()->SendMessage(WM_USER_DBLCLK,0,0); //2016-11-13为了实现筛选对话框中可编辑材质
	*pResult = 0;
}

void CSuperGridCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
    if(GetParent())
	    GetParent()->SendMessage(WM_USER_MOUSE_MOVE,0,0);
	CListCtrl::OnMouseMove(nFlags, point);
}

void CSuperGridCtrl::OnTimer(UINT nIDEvent) 
{
	CListCtrl::OnTimer(nIDEvent);
	if(nIDEvent==1)
	{
		if(CWnd::GetCapture()!=this)
		{
			CImageList::DragLeave(this);
			CImageList::EndDrag();
			ReleaseCapture();
			InvalidateRect(NULL);
			UpdateWindow();
			KillTimer(1);
			return;
		}

		SetTimer(1,300,NULL);//reset timer
		DWORD dwPos = ::GetMessagePos();
		CPoint ptList(LOWORD(dwPos),HIWORD(dwPos));
		ScreenToClient(&ptList);

		CRect rect;
		GetClientRect(rect);
		int cy = rect.Height();
		//
		// perform autoscroll if the cursor is near the top or bottom.
		//
		if (ptList.y >= 0 && ptList.y <= m_cyImage) 
		{
			int n = GetTopIndex(); 
			CImageList::DragShowNolock(0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
			CImageList::DragShowNolock(1);
			if (GetTopIndex()== n)
				KillTimer (1);
			else {
				CImageList::DragShowNolock(0);
				CImageList::DragMove(ptList);
				CImageList::DragShowNolock(1);
				return;
			}
		}
		else if (ptList.y >= cy - m_cyImage && ptList.y <= cy) 
		{
			int n = GetTopIndex(); 
			CImageList::DragShowNolock(0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
			CImageList::DragShowNolock(1);
			if (GetTopIndex()== n)
				KillTimer (1);
			else {
				CImageList::DragShowNolock(0);
				CImageList::DragMove(ptList);
				CImageList::DragShowNolock(1);
				return;
			}
		}
		
		KillTimer(1);
	}
}



void CSuperGridCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if(GetParent())
		GetParent()->SendMessage(WM_USER_LBUTTON_UP,0,0);
	CListCtrl::OnLButtonUp(nFlags, point);
}



//used with the drag/drop operation
void CSuperGridCtrl::CopyChildren(CTreeItem* pDest, CTreeItem* pSrc)
{
	if (ItemHasChildren(pSrc))
	{
		POSITION pos = pSrc->m_listChild.GetHeadPosition();
		while (pos != NULL)
		{
			CTreeItem* pItem = (CTreeItem *)pSrc->m_listChild.GetNext(pos);
			CItemInfo* lp = CopyData(GetData(pItem));
			CTreeItem* pNewItem = InsertItem(pDest, lp);
			CopyChildren(pNewItem, pItem);
		}
	}
}



void CSuperGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//its not meself
	if( GetFocus() != this) 
		SetFocus();
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSuperGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( GetFocus() != this) 
		SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}




BOOL CSuperGridCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(GetFocus()==this)
		{
			switch( pMsg->wParam )
			{
				case VK_LEFT:
					{
						// Decrement the order number.
						m_CurSubItem--;
						if(m_CurSubItem < 0) 
							m_CurSubItem = 0;
						else{
							
							CHeaderCtrl* pHeader = GetHeaderCtrl();
							// Make the column visible.
							// We have to take into account that the header may be reordered.
							MakeColumnVisible( Header_OrderToIndex( pHeader->m_hWnd, m_CurSubItem));
							// Invalidate the item.
							int iItem = GetSelectedItem();
							if( iItem != -1 )
							{
								CRect rcBounds;
								GetItemRect(iItem, rcBounds, LVIR_BOUNDS);
								InvalidateRect(&rcBounds);
								UpdateWindow();
							}
						}
					}
					return TRUE;

				case VK_RIGHT:
					{
						// Increment the order number.
						m_CurSubItem++;
						CHeaderCtrl* pHeader = GetHeaderCtrl();
						int nColumnCount = pHeader->GetItemCount();
						// Don't go beyond the last column.
						if( m_CurSubItem > nColumnCount -1 ) 
							m_CurSubItem = nColumnCount-1;
						else
						{
							MakeColumnVisible(Header_OrderToIndex( pHeader->m_hWnd, m_CurSubItem));
							 
							int iItem = GetSelectedItem();
							// Invalidate the item.
							if( iItem != -1 )
							{
								CRect rcBounds;
								GetItemRect(iItem, rcBounds, LVIR_BOUNDS);
								InvalidateRect(&rcBounds);
								UpdateWindow();
							}
						}
					}
					return TRUE;

				case VK_RETURN://edit itemdata

					break;
				default:
					break;
			}
		}
	}
	return CListCtrl::PreTranslateMessage(pMsg);
}




void CSuperGridCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO*)pNMHDR;
 	LV_ITEM *plvItem = &plvDispInfo->item;
	if (plvItem->pszText != NULL)//valid text
	{
		if(plvItem->iItem != -1) //valid item
		{
			CTreeItem*pSelItem = GetTreeItem(plvItem->iItem);
			if(pSelItem != NULL)
			{
				OnUpdateListViewItem(pSelItem, plvItem);
			}
		}
	}
	*pResult = 0;
}



int CSuperGridCtrl::GetNumCol()
{
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	return pHeader ? pHeader->GetItemCount() : 0;
}



//Think Rex Myer is spooking here
void CSuperGridCtrl::MakeColumnVisible(int nCol)
{
	if(nCol < 0)
		return;
	// Get the order array to total the column offset.
	CHeaderCtrl* pHeader = GetHeaderCtrl();

	int nColCount = pHeader->GetItemCount();
	ASSERT( nCol < nColCount);
	int *pOrderarray = new int[nColCount];
	Header_GetOrderArray(pHeader->m_hWnd, nColCount, pOrderarray);
	// Get the column offset
	int offset = 0;
	for(int i = 0; pOrderarray[i] != nCol; i++)
		offset += GetColumnWidth(pOrderarray[i]);

	int colwidth = GetColumnWidth(nCol);
	delete[] pOrderarray;

	CRect rect;
	GetItemRect(0, &rect, LVIR_BOUNDS);
	// Now scroll if we need to show the column
	CRect rcClient;
	GetClientRect(&rcClient);
	if(offset + rect.left < 0 || offset + colwidth + rect.left > rcClient.right)
	{
		CSize size(offset + rect.left,0);
		Scroll(size);
		InvalidateRect(NULL);
		UpdateWindow();
	}
}



//Think Rex Myer is spooking here
int CSuperGridCtrl::IndexToOrder( int iIndex )
{
	// This translates a column index value to a column order value.
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColCount = pHeader->GetItemCount();
	int *pOrderarray = new int[nColCount];
	Header_GetOrderArray(pHeader->m_hWnd, nColCount, pOrderarray);
	for(int i=0; i<nColCount; i++)
	{
		if(pOrderarray[i] == iIndex )
		{
			delete[] pOrderarray;
			return i;
		}
	}
	delete[] pOrderarray;
	return -1;
}



void CSuperGridCtrl::DrawFocusCell(CDC *pDC, int nItem, int iSubItem)
{
	if(iSubItem==m_CurSubItem)
	{
		CRect rect;
		GetSubItemRect(nItem, iSubItem, LVIR_BOUNDS, rect);
		CBrush br(GetCellRGB());
		if(iSubItem==0)
			GetItemRect(iSubItem, rect, LVIR_LABEL);
		pDC->FillRect(rect, &br);
		pDC->DrawFocusRect(rect);
	}
}

//insert item and return new parent pointer.
CSuperGridCtrl::CTreeItem* CSuperGridCtrl::InsertItem(CTreeItem *pParent, CItemInfo* lpInfo,  BOOL bUpdate)
{
	if(pParent==NULL)
		return NULL;

	CTreeItem *pItem = NULL;
	pItem =  new CTreeItem();

	if(lpInfo==NULL)
		lpInfo = new CItemInfo;

	UpdateData(pItem, lpInfo);
	SetIndent(pItem, GetIndent(pParent)+1);
	SetParentItem(pItem, pParent);
	//add as the last child 
	pParent->m_listChild.AddTail(pItem);

	if(!bUpdate)
		 Hide(pParent, TRUE);	
	else
	{
		//calc listview index for the new node
		int nIndex = NodeToIndex(pItem);			
		CString str = GetData(pItem)->GetItemText();
		LV_ITEM     lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
		lvItem.pszText = str.GetBuffer(1); 
		//insert item
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		lvItem.lParam = (LPARAM)pItem;
		lvItem.iIndent = GetIndent(pItem);
		CListCtrl::InsertItem(&lvItem);
		if(lpInfo->GetCheck())
			SetCheck(nIndex);
		//Get subitems
		int nSize = GetData(pItem)->GetItemCount();
		for(int i=0; i < nSize;i++)
		{
		   CString str = GetData(pItem)->GetSubItem(i);
		   lvItem.mask = LVIF_TEXT;
		   lvItem.iSubItem = i+1;
		   lvItem.pszText = str.GetBuffer(1);
		   SetItem(&lvItem);
		}
		InternaleUpdateTree();//better do this
	}
	return pItem;
}	



void CSuperGridCtrl::InternaleUpdateTree()
{
	int nItems = GetItemCount();
	for(int nItem=0; nItem < nItems; nItem++)
	{
		CTreeItem* pItem = GetTreeItem(nItem);
		SetCurIndex(pItem, nItem);
	}
}


int CSuperGridCtrl::NodeToIndex(CTreeItem *pNode)
{
	int nStartIndex=0;
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CTreeItem * root = (CTreeItem*)m_RootItems.GetNext(pos);
		int ret = _NodeToIndex(root, pNode, nStartIndex);
		if(ret != -1)
			return ret;
	}
	return -1;
}




CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetRootItem(int nIndex)
{
	POSITION pos = m_RootItems.FindIndex(nIndex);
	if(pos==NULL)
		return NULL;
	return (CTreeItem*)m_RootItems.GetAt(pos);
}



int CSuperGridCtrl::GetRootIndex(CTreeItem * root)
{
	int nIndex = 0;
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos != NULL)
	{
		CTreeItem * pItem = (CTreeItem*)m_RootItems.GetNext(pos);
		if(pItem== root)
			return nIndex;
		nIndex++;
	}
	return -1;
}



BOOL CSuperGridCtrl::IsRoot(CTreeItem * lpItem)
{
	return m_RootItems.Find(lpItem) != NULL;
}


void CSuperGridCtrl::DeleteRootItem(CTreeItem * root)
{
	POSITION pos = m_RootItems.Find(root);
	if(pos!=NULL)
	{
		CTreeItem* pRoot=(CTreeItem*)m_RootItems.GetAt(pos);
		if(pRoot->m_lpNodeInfo!=NULL)
				delete pRoot->m_lpNodeInfo;
		delete pRoot;
		m_RootItems.RemoveAt(pos);
	}
}



CSuperGridCtrl::CTreeItem*  CSuperGridCtrl::InsertRootItem(CItemInfo * lpInfo)
{
	if(lpInfo==NULL)
		lpInfo = new CItemInfo;

	CTreeItem* pRoot = NULL;
	
	pRoot =  new CTreeItem();

	CleanMe(pRoot);
	UpdateData(pRoot, lpInfo);
	SetIndent(pRoot, 1);
	SetCurIndex(pRoot, GetItemCount());
	SetParentItem(pRoot, NULL);

	CItemInfo* lp = GetData(pRoot);
	LV_ITEM lvItem;		
	lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
	CString strItem = lp->GetItemText();
	lvItem.pszText = strItem.GetBuffer(1); 
	lvItem.iItem = GetItemCount();
	lvItem.lParam = (LPARAM)pRoot;
	lvItem.iIndent = 1;
	lvItem.iSubItem = 0;
	CListCtrl::InsertItem(&lvItem);
	int nSize = lp->GetItemCount();
	for(int i=0; i < nSize;i++)
	{
	   CString str = lp->GetSubItem(i);
	   lvItem.mask = LVIF_TEXT;
	   lvItem.iSubItem = i+1;
	   lvItem.pszText = str.GetBuffer(1);
	   SetItem(&lvItem);
	}
	m_RootItems.AddTail(pRoot);
	return pRoot;
}



void CSuperGridCtrl::DrawTreeItem(CDC* pDC, CTreeItem* pSelItem, int nListItem, const CRect& rcBounds)
{
	int nColWidth = GetColumnWidth(0);
	int yDown = rcBounds.top;
    CPen* pPenTreeLine = pDC->SelectObject(&m_psTreeLine);
	int iIndent = GetIndent(pSelItem);
	int nHalfImage = (m_cxImage >> 1);
	int nBottomDown = yDown + nHalfImage + ((rcBounds.Height() - m_cyImage) >> 1);
	//
	BOOL bChild = ItemHasChildren(pSelItem);
	BOOL bCollapsed = IsCollapsed(pSelItem);
	//draw outline	
	while(1)
	{
		CTreeItem* pParent = GetParentItem(pSelItem);
		if(pParent==NULL)//no more parents, stop
			break;

		POSITION pos = pParent->m_listChild.GetTailPosition();
		while(pos!=NULL)
		{
			CTreeItem *pLastChild = (CTreeItem*)pParent->m_listChild.GetPrev(pos);
			int nIndex = GetCurIndex(pLastChild);
			int nCurIndent = GetIndent(pLastChild);
			if(nListItem > nIndex && iIndent > nCurIndent)//no need to go further in this loop
				break;

			//no drawing outside the 1st columns right
			int xLine =  rcBounds.left + nCurIndent * m_cxImage - nHalfImage;
			if(nIndex == nListItem && nCurIndent==iIndent)
			{
				//draw '-
				int x;
				pDC->MoveTo(xLine, yDown);
				pDC->LineTo(xLine, nBottomDown);
				// -
				xLine + nHalfImage > nColWidth ? x = nColWidth: x = xLine + nHalfImage;
				
				pDC->MoveTo(xLine, nBottomDown);
				pDC->LineTo(x, nBottomDown);
				break;
			}
			else
			if(nIndex > nListItem && nCurIndent==iIndent)
			{
				//draw |-
				int x;
				xLine + nHalfImage > nColWidth ? x = nColWidth : x = xLine + nHalfImage;
				pDC->MoveTo(xLine, nBottomDown);
				pDC->LineTo(x, nBottomDown);
				//-
				pDC->MoveTo(xLine, yDown);
				pDC->LineTo(xLine, rcBounds.bottom);
				break;
			}
			else
			if(nIndex > nListItem && nCurIndent < iIndent)
			{
				//draw |
				pDC->MoveTo(xLine, yDown);
				pDC->LineTo(xLine, rcBounds.bottom);
				break;
			}
		}			
		pSelItem = pParent;//next
	}

	//draw plus/minus sign
	if(bChild)
	{
		CRectangle rect(this, pDC, iIndent, rcBounds);

		rect.DrawRectangle(this);

		CPen* pPenPlusMinus = pDC->SelectObject(&m_psPlusMinus);
		if(bCollapsed)
			rect.DrawPlus();
		else {
			rect.DrawMinus();
			//draw line up to parent folder
			CPen* pLine = pDC->SelectObject(&m_psTreeLine);
			int nOffset = (rcBounds.Height() - m_cyImage)/2;
			pDC->MoveTo(rect.GetLeft()+ m_cxImage, rcBounds.top + m_cyImage+nOffset);
			pDC->LineTo(rect.GetLeft() + m_cxImage, rcBounds.bottom);
			pDC->SelectObject(pLine);		
		}
		pDC->SelectObject(pPenPlusMinus);		
	}
	pDC->SelectObject(pPenTreeLine);
}



//walk all over the place setting the hide/show flag of the nodes.
//it also deletes items from the listviewctrl.
void CSuperGridCtrl::HideChildren(CTreeItem *pItem, BOOL bHide,int nItem)
{
	if(!IsCollapsed(pItem))
	if(ItemHasChildren(pItem))
	{
		Hide(pItem, bHide);
		POSITION pos = pItem->m_listChild.GetHeadPosition();
		while (pos != NULL)
		{
			HideChildren((CTreeItem *)pItem->m_listChild.GetNext(pos),bHide,nItem+1);
			DeleteItem(nItem);

		}
	}
}




void CSuperGridCtrl::Collapse(CTreeItem *pItem)
{
	if(pItem != NULL && ItemHasChildren(pItem))
	{
		SetRedraw(0);
		int nIndex = NodeToIndex(pItem);			
		HideChildren(pItem, TRUE, nIndex+1);
		InternaleUpdateTree();
		SetRedraw(1);
	}
}


void CSuperGridCtrl::ExpandAll(CTreeItem *pItem, int& nScroll)
{
	const int nChildren = pItem->m_listChild.GetCount();
	if (nChildren > 0)
	{
		int nIndex = NodeToIndex(pItem);
		nScroll = Expand(pItem, nIndex);
	}

	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos)
	{
		CTreeItem *pChild = (CTreeItem*)pItem->m_listChild.GetNext(pos);
		ExpandAll(pChild, nScroll);
	}
	
}



int CSuperGridCtrl::Expand(CTreeItem* pSelItem, int nIndex)
{
	if(ItemHasChildren(pSelItem) && IsCollapsed(pSelItem))
	{
		LV_ITEM lvItem;
		lvItem.mask = LVIF_INDENT;
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		lvItem.lParam=(LPARAM)pSelItem;
		lvItem.iIndent = GetIndent(pSelItem);
		SetItem(&lvItem);

		Hide(pSelItem, FALSE);
		//expand children
		POSITION pos = pSelItem->m_listChild.GetHeadPosition();
		while(pos != NULL)
		{
			CTreeItem* pNextNode = (CTreeItem*)pSelItem->m_listChild.GetNext(pos);
			CString str = GetData(pNextNode)->GetItemText();
			LV_ITEM lvItem;
			lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
			lvItem.pszText =str.GetBuffer(1); 
			lvItem.iItem = nIndex + 1;
			lvItem.iSubItem = 0;
			lvItem.lParam=(LPARAM)pNextNode;
			lvItem.iIndent = GetIndent(pSelItem)+1;
			CListCtrl::InsertItem(&lvItem);
			if(GetData(pNextNode)->GetCheck())
				SetCheck(nIndex + 1);
			//get subitems
			int nSize = GetData(pNextNode)->GetItemCount();
			for(int i=0; i< nSize;i++)
			{
			   CString str=GetData(pNextNode)->GetSubItem(i);
			   lvItem.mask = LVIF_TEXT;
			   lvItem.iSubItem = i+1;
			   lvItem.pszText=str.GetBuffer(1);
			   SetItem(&lvItem);
			}
			nIndex++;
		}
	}
	InternaleUpdateTree();
	return nIndex;
}




int CSuperGridCtrl::SelectNode(CTreeItem *pLocateNode)
{
	if(IsRoot(pLocateNode))
	{
		UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
		SetItemState(0, uflag, uflag);
		return 0;
	}
	int nSelectedItem=-1;
	CTreeItem* pNode = pLocateNode;
	CTreeItem* pTopLevelParent=NULL;
	//Get top parent
	while(1)
	{
		CTreeItem *pParent = GetParentItem(pLocateNode);
		if(IsRoot(pParent))
			break;
		pLocateNode = pParent;
	}
	pTopLevelParent = pLocateNode;//on top of all
	//Expand the folder
	if(pTopLevelParent != NULL)
	{
		SetRedraw(0);
		CWaitCursor wait;
		CTreeItem *pRoot = GetParentItem(pTopLevelParent);

		if(IsCollapsed(pRoot))
			Expand(pRoot,0);

		ExpandUntil(pTopLevelParent, pNode);

		UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
		nSelectedItem = NodeToIndex(pNode);

		SetItemState(nSelectedItem, uflag, uflag);

		SetRedraw(1);
		EnsureVisible(nSelectedItem, TRUE);
	}
	return nSelectedItem;
}




void CSuperGridCtrl::ExpandUntil(CTreeItem *pItem, CTreeItem* pStopAt)
{
	const int nChildren = pItem->m_listChild.GetCount();
	if (nChildren > 0)
	{
		POSITION pos = pItem->m_listChild.GetHeadPosition();
		while (pos)
		{
			CTreeItem *pChild = (CTreeItem*)pItem->m_listChild.GetNext(pos);
			if(pChild == pStopAt)	
			{
				int nSize = GetIndent(pChild);
				CTreeItem** ppParentArray = new CTreeItem*[nSize];
				int i=0;
				while(1)
				{
					CTreeItem *pParent = GetParentItem(pChild);
					
					if(IsRoot(pParent))
						break;
					pChild = pParent;
					ppParentArray[i] = pChild;
					i++;
				}

				for(int x=i; x > 0; x--)
				{
					CTreeItem *pParent = ppParentArray[x-1];
					Expand(pParent, NodeToIndex(pParent));
				}
				delete [] ppParentArray;
				return;
			}
		}
	}

	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos)
	{
		CTreeItem *pChild = (CTreeItem*)pItem->m_listChild.GetNext(pos);
		ExpandUntil(pChild, pStopAt);
	}
	
}



void CSuperGridCtrl::DeleteItemEx(CTreeItem *pSelItem, int nItem)
{
	SetRedraw(0);
	DeleteItem(nItem);//delete cur item in listview
	//delete/hide all children in pSelItem
	HideChildren(pSelItem, TRUE, nItem);
	//delete all internal nodes
	// If root, must delete from m_rootData
	if(GetParentItem(pSelItem) == NULL )
	{
		DeleteRootItem(pSelItem);
	}
	else
		Delete(pSelItem);

	InternaleUpdateTree();
	if(nItem-1<0)//no more items in list
	{
		/*SetRedraw(1); 
		InvalidateRect(NULL);
		UpdateWindow();*/
		return;
	}

	UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
	CRect rcTestIfItemIsValidToSelectOtherWiseSubtrackOneFromItem;//just to get the documention right :)
	GetItemRect(nItem, rcTestIfItemIsValidToSelectOtherWiseSubtrackOneFromItem ,LVIR_LABEL) ? SetItemState(nItem, uflag, uflag) : SetItemState(nItem-1, uflag, uflag);
	
	/*SetRedraw(1);
	InvalidateRect(NULL);
	UpdateWindow();*/
}



void CSuperGridCtrl::CleanMe(CTreeItem *pItem)
{
	// delete child nodes
	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		CTreeItem* pItemData = (CTreeItem*)pItem->m_listChild.GetNext(pos);
		if(pItemData!=NULL)
		{
			if(pItemData->m_lpNodeInfo!=NULL)
				delete pItemData->m_lpNodeInfo;

			pItemData->m_listChild.RemoveAll();
			delete pItemData;
		}
	}
	pItem->m_listChild.RemoveAll();
}




CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetNext(CTreeItem* pStartAt, CTreeItem* pNode, BOOL bInit, BOOL bDontIncludeHidden)
{
	static BOOL bFound;
	if (bInit)
		bFound = FALSE;
		
	if (pNode == pStartAt)
		bFound = TRUE;

	if(bDontIncludeHidden)
	{
		if (!IsCollapsed(pStartAt))
		{
			POSITION pos = pStartAt->m_listChild.GetHeadPosition();
			while (pos != NULL)
			{
				CTreeItem* pChild = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
				if (bFound)
					return pChild;
				pChild = GetNext(pChild, pNode, FALSE, TRUE);
				if (pChild != NULL)
					return pChild;
			}
		}
	}
	else {
			POSITION pos = pStartAt->m_listChild.GetHeadPosition();
			while (pos != NULL)
			{
				CTreeItem* pChild = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
				if (bFound)
					return pChild;
				pChild = GetNext(pChild, pNode, FALSE,FALSE);
				if (pChild != NULL)
					return pChild;
			}
	}
	// if reached top and last level return original
	if (bInit)
		return pNode;
	else
		return NULL;
}



CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetPrev(CTreeItem* pStartAt, CTreeItem* pNode, BOOL bInit, BOOL bDontIncludeHidden)
{
	static CTreeItem* pPrev;
	if (bInit)
		pPrev = pStartAt;

	if (pNode == pStartAt)
		return pPrev;

	pPrev = pStartAt;

	if(bDontIncludeHidden)
	{
		if (!IsCollapsed(pStartAt))
		{
			POSITION pos = pStartAt->m_listChild.GetHeadPosition();
			while (pos != NULL)
			{
				CTreeItem* pCur = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
				CTreeItem* pChild = GetPrev(pCur,pNode, FALSE,TRUE);
				if (pChild != NULL)
					return pChild;
			}
		}
	}
	else {
		POSITION pos = pStartAt->m_listChild.GetHeadPosition();
		while (pos != NULL)
		{
			CTreeItem* pCur = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
			CTreeItem* pChild = GetPrev(pCur,pNode, FALSE,FALSE);
			if (pChild != NULL)
				return pChild;
		}
	}

	if (bInit)
		return pPrev;
	else
		return NULL;
}


int CSuperGridCtrl::_NodeToIndex(CTreeItem *pStartpos, CTreeItem* pNode, int& nIndex, BOOL binit)
{
	static BOOL bFound;	
	// Account for other root nodes
	if(GetParentItem(pStartpos) == NULL && GetRootIndex(pStartpos) !=0)
		nIndex++;

	if(binit)
		bFound=FALSE;

	if(pStartpos==pNode)
		bFound=TRUE;

	if(!IsCollapsed(pStartpos))
	{
		POSITION pos = GetHeadPosition(pStartpos);
		while (pos)
		{
			CTreeItem *pChild = GetNextChild(pStartpos, pos);
			if(bFound)
				return nIndex;

//	Craig Schmidt: Cannot set nIndex as return value.  Worked find with single root but
//				   the calling function get confused since the return value may indicate
//				   that the next root needs to be searched.  Didn'd spend much time on
//				   this so there is probably a better way of doing this.
//			nIndex = _NodeToIndex(pChild, pNode, nIndex, binit);
			_NodeToIndex(pChild, pNode, nIndex, binit);
			nIndex++;
		}
	}
	if(binit && bFound)
		return nIndex;
	else
		return -1;
}


BOOL CSuperGridCtrl::Delete(CTreeItem* pNode, BOOL bClean)
{
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CTreeItem * pRoot = (CTreeItem*)m_RootItems.GetNext(pos);
		if(_Delete(pRoot, pNode, bClean))
			return TRUE;
	}
	return FALSE;
}



BOOL CSuperGridCtrl::_Delete(CTreeItem* pStartAt, CTreeItem* pNode, BOOL bClean)
{
	POSITION pos = pStartAt->m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION posPrev = pos;
		CTreeItem *pChild = (CTreeItem*)pStartAt->m_listChild.GetNext(pos);
		if (pChild == pNode)
		{
			pStartAt->m_listChild.RemoveAt(posPrev);
			if(bClean)
			{
				if(GetData(pNode)!=NULL)
					delete GetData(pNode);
				delete pNode;
			}
			return TRUE;
		}
		if (_Delete(pChild, pNode) == TRUE)
			return TRUE;
	}
	return FALSE;
}




BOOL CSuperGridCtrl::IsChildOf(const CTreeItem* pParent, const CTreeItem* pChild) const
{
	if (pChild == pParent)
		return TRUE;
	POSITION pos = pParent->m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		CTreeItem* pNode = (CTreeItem*)pParent->m_listChild.GetNext(pos);
		if (IsChildOf(pNode, pChild))
			return TRUE;
	}
	return FALSE;
}

void CSuperGridCtrl::SelectedItem(int nItem)
{
	SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED
					,LVIS_SELECTED|LVIS_FOCUSED);
	CRect rc;
	GetItemRect(nItem, rc, LVIR_BOUNDS);
	InvalidateRect(rc);
	UpdateWindow();
}

void CSuperGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( GetFocus() != this) 
		SetFocus();

	LVHITTESTINFO ht;
	ht.pt = point;
	SubItemHitTest(&ht);
	if(OnItemLButtonDown(ht))
	{
		BOOL bSelect=1;
		bSelect = HitTestOnSign(point, ht);
		if(bSelect && ht.iItem!=-1)
		{
			m_CurSubItem = IndexToOrder(ht.iSubItem);
			CHeaderCtrl* pHeader = GetHeaderCtrl();
			// Make the column fully visible.
			MakeColumnVisible(Header_OrderToIndex(pHeader->m_hWnd, m_CurSubItem));
			CListCtrl::OnLButtonDown(nFlags, point);
//			OnControlLButtonDown(nFlags, point, ht);//old code modify by hgf
			//update row anyway for selection bar
			CRect rc;
			GetItemRect(ht.iItem, rc, LVIR_BOUNDS);
			InvalidateRect(rc);
			UpdateWindow();
		}
	}
	if(GetParent())
	{
		GetParent()->SendMessage(WM_USER_SELECTED_GRID_ITEM,0,(LPARAM)this);
		GetParent()->SendMessage(WM_USER_LBUTTON_DOWN,0,0);
	}
}

void CSuperGridCtrl::OnLButtonDblClk( UINT nFlags, CPoint point)
{
    if( GetFocus() != this) 
		SetFocus();

	LVHITTESTINFO ht;
	ht.pt = point;
	SubItemHitTest(&ht);
	if(OnItemLButtonDown(ht))
	{
		BOOL bSelect=1;
		bSelect = HitTestOnSign(point, ht);
		if(bSelect && ht.iItem!=-1)
		{
			m_CurSubItem = IndexToOrder(ht.iSubItem);
			CHeaderCtrl* pHeader = GetHeaderCtrl();
			// Make the column fully visible.
			MakeColumnVisible(Header_OrderToIndex(pHeader->m_hWnd, m_CurSubItem));
			CListCtrl::OnLButtonDown(nFlags, point);
//			OnControlLButtonDown(nFlags, point, ht);//old code modify by hgf
			//update row anyway for selection bar
			CRect rc;
			GetItemRect(ht.iItem, rc, LVIR_BOUNDS);
			InvalidateRect(rc);
			UpdateWindow();
		}
	}
	if(GetParent())
	{
		GetParent()->SendMessage(WM_USER_SELECTED_GRID_ITEM,0,(LPARAM)this);
		GetParent()->SendMessage(WM_USER_DBUTTON_DOWN,0,(LPARAM)this);
	}
}

void CSuperGridCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    CListCtrl::OnRButtonDown(nFlags, point);
	if(GetParent())
	{
        GetParent()->SendMessage(WM_USER_RBUTTON_DOWN,0,(LPARAM)this); 
	}
}

void CSuperGridCtrl::OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem)
{
	//default implementation you would go for this 9 out of 10 times
	CItemInfo *lp = GetData(lpItem);
	CString str = (CString)plvItem->pszText;
	if(lp!=NULL)
	{
		if(plvItem->iSubItem==0)
			lp->SetItemText(str);
		else //subitem data 
			lp->SetSubItemText(plvItem->iSubItem-1, str);
	   UpdateData(lpItem, lp);
	}
	SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
}



void CSuperGridCtrl::DeleteAll()
{
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CTreeItem * pRoot = (CTreeItem*)m_RootItems.GetNext(pos);
		if(pRoot!=NULL)
			DeleteItemEx(pRoot, 0);
	}

	SetRedraw(1);
	InvalidateRect(NULL);
	UpdateWindow();
}


POSITION CSuperGridCtrl::GetRootHeadPosition() const
{
	return m_RootItems.GetHeadPosition();
}


POSITION CSuperGridCtrl::GetRootTailPosition() const
{
	return m_RootItems.GetTailPosition();
}


CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetNextRoot(POSITION& pos) const
{
	return (CTreeItem*)m_RootItems.GetNext(pos);
}


CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetPrevRoot(POSITION& pos) const
{
	return (CTreeItem*)m_RootItems.GetNext(pos);
}


POSITION CSuperGridCtrl::GetHeadPosition(CTreeItem* pItem) const
{
	return pItem->m_listChild.GetHeadPosition();
}



CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetNextChild(CTreeItem *pItem, POSITION& pos) const
{
	return (CTreeItem*)pItem->m_listChild.GetNext(pos);
}



CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetPrevChild(CTreeItem *pItem, POSITION& pos) const
{
	return (CTreeItem*)pItem->m_listChild.GetPrev(pos);
}



POSITION CSuperGridCtrl::GetTailPosition(CTreeItem *pItem) const
{
	return pItem->m_listChild.GetTailPosition();
}



void CSuperGridCtrl::AddTail(CTreeItem *pParent, CTreeItem *pChild)
{
	pParent->m_listChild.AddTail(pChild);
}


inline int StrComp(const CString* pElement1, const CString* pElement2)
{
	return pElement1->Compare(*pElement2);
}



int CSuperGridCtrl::CompareChildren(const void* p1, const void* p2)
{
	CTreeItem * pChild1 = *(CTreeItem**)p1;
	CTreeItem * pChild2 = *((CTreeItem**)p2);
	CItemInfo *pItem1=(*pChild1).m_lpNodeInfo;
	CItemInfo *pItem2=(*pChild2).m_lpNodeInfo;
	return StrComp(&(pItem1->GetItemText()), &(pItem2->GetItemText()));
}


void CSuperGridCtrl::Sort(CTreeItem* pParent, BOOL bSortChildren)
{
	const int nChildren = NumChildren(pParent);
	int i = 0;
	if (nChildren > 1)
	{
		CTreeItem** ppSortArray = new CTreeItem*[nChildren];
		// Fill in array with pointers to our children.
		POSITION pos = pParent->m_listChild.GetHeadPosition();
		for (i=0; pos; i++)
		{
			ASSERT(i < nChildren);
			ppSortArray[i] = (CTreeItem*)pParent->m_listChild.GetAt(pos);
			pParent->m_listChild.GetNext(pos);
		}

		qsort(ppSortArray, nChildren, sizeof(CTreeItem*), CompareChildren);
		// reorg children with new sorted list
		pos = pParent->m_listChild.GetHeadPosition();
		for (i=0; pos; i++)
		{
			ASSERT(i < nChildren);
			pParent->m_listChild.SetAt(pos, ppSortArray[i]);
			pParent->m_listChild.GetNext(pos);
		}

		delete [] ppSortArray;
	}

	if(bSortChildren)
	{
		POSITION pos = pParent->m_listChild.GetHeadPosition();
		while (pos)
		{
			CTreeItem *pChild = (CTreeItem*)pParent->m_listChild.GetNext(pos);
			Sort(pChild, TRUE);
		}
	}
}



int CSuperGridCtrl::NumChildren(const CTreeItem *pItem) const
{
	return pItem->m_listChild.GetCount();
}



BOOL CSuperGridCtrl::ItemHasChildren(const CTreeItem* pItem) const
{ 
	BOOL bChildren = pItem->m_listChild.GetCount() != 0;
	//see if we have a flag
	int nFlag = pItem->m_bSetChildFlag;
	if(nFlag!=-1)
		return 1;
	else
		return bChildren;
}


void CSuperGridCtrl::SetChildrenFlag(CTreeItem *pItem, int nFlag) const
{
	pItem->m_bSetChildFlag = nFlag;
}


BOOL CSuperGridCtrl::IsCollapsed(const CTreeItem* pItem) const
{
	return pItem->m_bHideChildren;//e.g not visible
}


void CSuperGridCtrl::Hide(CTreeItem* pItem, BOOL bFlag)
{
	pItem->m_bHideChildren=bFlag;
}


int CSuperGridCtrl::GetIndent(const CTreeItem* pItem) const
{
	return pItem->m_nIndent;
}


void CSuperGridCtrl::SetIndent(CTreeItem *pItem, int iIndent)
{
	pItem->m_nIndent = iIndent;
}



int CSuperGridCtrl::GetCurIndex(const CTreeItem *pItem) const
{
	return pItem->m_nIndex;
}


void CSuperGridCtrl::SetCurIndex(CTreeItem* pItem, int nIndex) 
{
	pItem->m_nIndex = nIndex;
}


void CSuperGridCtrl::SetParentItem(CTreeItem*pItem, CTreeItem* pParent)
{
	pItem->m_pParent=pParent;

}


CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetParentItem(const CTreeItem* pItem) 
{
	return pItem->m_pParent;
};



CItemInfo* CSuperGridCtrl::GetData(const CTreeItem* pItem) 
{
	return pItem->m_lpNodeInfo;
}

void *CSuperGridCtrl::GetData(int nItem)
{
    CTreeItem *hTreeItem=(CTreeItem *)GetItemData(nItem);
	if(hTreeItem)
	{
	    CItemInfo *pInfo=hTreeItem->m_lpNodeInfo;
		if(pInfo)
			return pInfo->GetData();
	}
	return NULL;
}


int CSuperGridCtrl::IsVisual(int nRow)
{
	//判断结点是否收缩
    CTreeItem *hTreeItem=(CTreeItem *)GetItemData(nRow);
	CTreeItem *hParentTreeItem=GetParentItem(hTreeItem);
    while(hParentTreeItem)
	{
		if(IsCollapsed(hParentTreeItem))//收缩
			return 0;
		hParentTreeItem=GetParentItem(hParentTreeItem);
	}
	//判断矩形框位置
	CRect rc,rcHead,rcAllItems;
	GetClientRect(rc);
	GetHeaderCtrl()->GetWindowRect(rcHead);
    ScreenToClient(rcHead);
	rcAllItems=CRect(rc.left,rcHead.bottom,rc.right,rc.bottom);
	int nItemCount=GetItemCount();
	if(nRow<nItemCount)
	{
        CRect rcItem;
		GetItemRect(nRow,rcItem,LVIR_BOUNDS);
		if(rcItem.top>=rcAllItems.top&&rcItem.top<=rcAllItems.bottom)
			return 1;
	}
	return 0;
}

void CSuperGridCtrl::UpdateData(CTreeItem* pItem, CItemInfo* lpInfo)
{
	pItem->m_lpNodeInfo = lpInfo;
}


//overrides
CItemInfo* CSuperGridCtrl::CopyData(CItemInfo* lpSrc)
{
	ASSERT(FALSE);//debug
	return NULL;  //release
}

//default implementation for setting icons
int CSuperGridCtrl::GetIcon(const CTreeItem* pItem)
{
	if(pItem!=NULL)
	{
		int n = GetData(pItem)->GetImage();
		if(n!=-1)
			return n;
		
		int iImage = 0;
		if(ItemHasChildren(pItem))
		{
			IsCollapsed(pItem) ? iImage = 1/*close icon*/:iImage = 0;/*open icon*/
		}
		else
			iImage = 2;//doc icon
		return iImage;
	}
	return 0;//just take the first item in CImageList ..what ever that is
}



COLORREF CSuperGridCtrl::GetCellRGB()
{
	return RGB(192,0,0);
}

int CSuperGridCtrl::get_layer(CTreeItem *hTreeItem)
{
    int nLayer=0;
	CTreeItem *hParentItem=GetParentItem(hTreeItem);
	while(hParentItem)
	{
		nLayer++;
		hParentItem=GetParentItem(hParentItem);
	}
	return nLayer;
}

int CSuperGridCtrl::cal_column_font_count(int nColumn)
{
	LVCOLUMN columnData;
	CString columnName;
	columnData.mask = LVCF_TEXT;
	columnData.cchTextMax =100;
	columnData.pszText=columnName.GetBuffer(100);
	
	GetColumn(nColumn,&columnData);
	CString strColumnTitle=columnData.pszText;
    int nFontCount=strColumnTitle.GetLength();
    //遍历所有行对应的列
    POSITION POS=GetRootHeadPosition();
	while(POS)
	{
		CTreeItem *hTreeItem=(CTreeItem*)GetNextRoot(POS);
        cal_column_font_count(hTreeItem,nColumn,nFontCount);
	}

	int nTitleLenth=strColumnTitle.GetLength();
	
	if(nFontCount>nTitleLenth)
	{
		nFontCount=nTitleLenth;
		if(nFontCount>=8)
            return (int)nFontCount*0.8;
	}
	else if(nTitleLenth>=8)
	{
		if(nTitleLenth>=12)
			return nTitleLenth*0.6;
		else if(nTitleLenth>=10)
			return nTitleLenth*0.7;
		else
		    return nTitleLenth*0.8;
	}
	return nFontCount;
}

void CSuperGridCtrl::cal_column_font_count(CTreeItem *hTreeItem,int nColumn,int &nFontCount)
{
	CItemInfo* lp = GetData(hTreeItem);
	int nSubCount=lp->GetItemCount();
	CString strText;
	if(nColumn==0)
        strText=lp->GetItemText();
	else
	{
        if(nColumn>nSubCount)
			strText=_T("");
		else
			strText=lp->GetSubItem(nColumn-1);
	}
	int nLenth=strText.GetLength();
	if(nLenth>nFontCount)
		nFontCount=nLenth;
	POSITION POS = GetHeadPosition(hTreeItem);
	while(POS)
	{
		CTreeItem *hChildItem = GetNextChild(hTreeItem,POS);
		cal_column_font_count(hChildItem,nColumn,nFontCount);
	}
}

	
BOOL CSuperGridCtrl::OnVKMultiply(CTreeItem *pItem, int nIndex)
{
	return 1;
}



BOOL CSuperGridCtrl::OnVkSubTract(CTreeItem *pItem, int nIndex)
{
	return 1;
}



BOOL CSuperGridCtrl::OnVKAdd(CTreeItem *pItem, int nIndex)
{
	return 1;
}


BOOL CSuperGridCtrl::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	return 1;
}



BOOL CSuperGridCtrl::OnItemExpanding(CTreeItem *pItem, int iItem)
{
	return 1;
}


BOOL CSuperGridCtrl::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	return 1;
}


BOOL CSuperGridCtrl::OnCollapsing(CTreeItem *pItem)
{
	return 1;
}


BOOL CSuperGridCtrl::OnItemCollapsed(CTreeItem *pItem)
{
	return 1;
}


BOOL CSuperGridCtrl::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	return 1;
}


BOOL CSuperGridCtrl::OnVkReturn(void)
{
	return 0;
}


void CSuperGridCtrl::OnSysColorChange() 
{
	CListCtrl::OnSysColorChange();
	//nop nothing yet
}



UINT CSuperGridCtrl::_GetCount(CTreeItem* pItem, UINT& nCount)
{
	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos)
	{
		CTreeItem *pChild = (CTreeItem*)pItem->m_listChild.GetNext(pos);
		nCount = _GetCount(pChild, nCount);
		nCount++;				
	}
	return nCount;
}



UINT CSuperGridCtrl::GetCount(void) 
{
	UINT nCount=0;
	UINT _nCount=0;
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CTreeItem * pRoot = (CTreeItem*)m_RootItems.GetNext(pos);
		nCount += _GetCount(pRoot, _nCount) + 1;
	}
	return nCount;
}



CSuperGridCtrl::CTreeItem* CSuperGridCtrl::GetTreeItem(int nIndex /*nIndex must be valid of course*/ ) 
{
	return reinterpret_cast<CTreeItem*>(GetItemData(nIndex));
}



int CSuperGridCtrl::GetSelectedItem(void) const
{
	return GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
}

//////////////////////////////////////////////////////////////////////////
//
// not much but ... 
//
//////////////////////////////////////////////////////////////////////////

CSuperGridCtrl::CTreeItem::~CTreeItem()
{
	// delete child nodes
	POSITION pos = m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		CTreeItem* pItem = (CTreeItem*)m_listChild.GetNext(pos);
		if(pItem!=NULL)
		{
			if(pItem->m_lpNodeInfo!=NULL)
				delete pItem->m_lpNodeInfo;
			delete pItem;
		}
	}
	m_listChild.RemoveAll();
}



//////////////////////////////////////////////////////////////////////////
//
// Simple class CRectangle for the + - sign, 
//
//////////////////////////////////////////////////////////////////////////

CRectangle::CRectangle(CSuperGridCtrl* pCtrl, CDC* pDC, int iIndent, const CRect& rcBounds)
{
	m_pDC=pDC;
	int nHalfImage = (pCtrl->m_cxImage >> 1);
	int nBottomDown = rcBounds.top + nHalfImage + ((rcBounds.Height() - pCtrl->m_cyImage) >> 1);
	m_right_bottom.cx = (pCtrl->m_cxImage>>1)+2+1;
	m_right_bottom.cy = (pCtrl->m_cyImage>>1)+2+1;
	m_left = rcBounds.left  + iIndent * pCtrl->m_cxImage - nHalfImage;
	m_top = nBottomDown - (m_right_bottom.cy >> 1);
	m_left_top.x = m_left -  (m_right_bottom.cx >> 1);
	m_left_top.y = m_top;
	m_topdown = nBottomDown;
}

	
void CRectangle::DrawRectangle(CSuperGridCtrl* pCtrl)
{
	//erase bkgrnd
	CRect rc(m_left_top, m_right_bottom);
	m_pDC->FillRect(rc, &pCtrl->m_brushErase);
	//draw rectangle	
	CPen* pPenRectangle = m_pDC->SelectObject(&pCtrl->m_psRectangle);
	m_pDC->Rectangle(rc);
	m_pDC->SelectObject(pPenRectangle);		
}


CRectangle::~CRectangle()
{
}


BOOL CRectangle::HitTest(CPoint pt)
{
	CRect rc = GetHitTestRect();
	return rc.PtInRect(pt);
}


void CRectangle::DrawPlus(void)
{
	m_pDC->MoveTo(m_left, m_topdown-2);
	m_pDC->LineTo(m_left, m_topdown+3);

	m_pDC->MoveTo(m_left-2, m_topdown);
	m_pDC->LineTo(m_left+3, m_topdown);
}


void CRectangle::DrawMinus(void)
{
	m_pDC->MoveTo(m_left-2, m_topdown);
	m_pDC->LineTo(m_left+3, m_topdown);
}


void CSuperGridCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	int nSel= GetSelectedItem();
	if(nSel>=0)
	{
		CSuperGridCtrl::CTreeItem *hItem=GetTreeItem(nSel);
		CItemInfo *pInfo=(CItemInfo *)GetData(hItem);
		if ((pInfo)
			&&(pInfo->GetData())
			&&(GetParent()))
		{
			GetParent()->SendMessage(WM_USER_LBUTTON_DOWN,(WPARAM)(pInfo->GetData()),0);
		}
	}
}

