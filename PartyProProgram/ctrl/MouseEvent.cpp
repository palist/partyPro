#include "StdAfx.h"
#include "MouseEvent.h" 

#define IN_CIRCLE	1
#define IN_BRIGHT	2
#define IN_NOTHING	0


#define RADUIS	100

BOOL CMouseEvent::InCircle(CPoint pt)
{
	return Distance(pt, m_Centre) <= RADIUS;
}

//判断当前鼠标是否点在彩虹上
void CMouseEvent::TrackPoint(CPoint pt)
{
	if(m_nMouseIn == IN_CIRCLE)
	{
		CClientDC dc(m_pDlg);
		DrawMarkers(&dc);

		hsvColor.h = (int)RAD2DEG(AngleFromPoint(pt,m_Centre));
		if(hsvColor.h < 0)
		{
			hsvColor.h += 360;
		}
		hsvColor.s = (int)SCALETOMAX(Distance(pt,m_Centre));
		if(hsvColor.s > 255) hsvColor.s = 255;

		SetDIBPalette();
		CalcRects();

		m_pDlg->InvalidateRect(&brightRect,FALSE);

		DrawMarkers(&dc);

		color = hsvColor.toRGB();
		SetEditVals();
		DrawLines(&dc);
	}
}


//鼠标移动
void CMouseEvent::mouse_move(UINT nFlags, CPoint point)
{
	if(GetCapture() == m_pDlg->GetSafeHwnd() && m_nMouseIn)
	{
		TrackPoint(point);
	}
	else if(GetCapture() == m_pDlg->GetSafeHwnd() && m_bInMouse)
	{
		double val;
		BOOL bChange = FALSE;
		if(nIndex == RED)
		{
			if(point.y > Vertex.y)
			{
				point.y = Vertex.y;
			}
			point.x = Vertex.x;
			val = Distance(point,Vertex);
			if(val > RedLen)
			{
				val = RedLen;
			}
			CClientDC dc(m_pDlg);
			DrawLines(&dc);
			val = (val/RedLen)*255;
			color.r = (int)val;
			CPoint pt;
			pt = PointOnLine(Vertex,Top,(color.r*RedLen)/255,RedLen);
			rects[RED] = CRect(pt.x - RECT_WIDTH ,pt.y-RECT_WIDTH ,pt.x+RECT_WIDTH ,pt.y+RECT_WIDTH );
			DrawLines(&dc);
			bChange = TRUE;
		}
		else if(nIndex == GREEN)
		{
			if(point.x > Vertex.x)
			{
				point.x = Vertex.x;
			}
			point.y = rects[GREEN].top + RECT_WIDTH;
			val = Distance(point,Vertex);
			if(val > GreenLen)
			{
				val = GreenLen;
			}
			CClientDC dc(m_pDlg);
			DrawLines(&dc);
			val = (val/GreenLen)*255;
			color.g = (int)val;
			CPoint pt;
			pt = PointOnLine(Vertex,Left,(color.g*GreenLen)/255,GreenLen);
			rects[GREEN] = CRect(pt.x - RECT_WIDTH ,pt.y-RECT_WIDTH ,pt.x+RECT_WIDTH ,pt.y+RECT_WIDTH );
			DrawLines(&dc);
			bChange = TRUE;
		}
		else if(nIndex == BLUE)
		{
			if(point.x < Vertex.x)
			{
				point.x = Vertex.x;
			}
			point.y = rects[BLUE].top + RECT_WIDTH;
			val = Distance(point,Vertex);
			if(val > BlueLen)
			{
				val = BlueLen;
			}
			CClientDC dc(m_pDlg);
			DrawLines(&dc);
			val = (val/BlueLen)*255;
			color.b = (int)val;
			CPoint pt;
			pt = PointOnLine(Vertex,Right,(color.b*GreenLen)/255,BlueLen);
			rects[BLUE] = CRect(pt.x - RECT_WIDTH ,pt.y-RECT_WIDTH ,pt.x+RECT_WIDTH ,pt.y+RECT_WIDTH );
			DrawLines(&dc);
			bChange = TRUE;
		}
		if(bChange)
		{
			hsvColor = color.toHSV();
			SetEditVals();
			CClientDC dc(m_pDlg);
			DrawMarkers(&dc);
			CalcRects();
			SetDIBPalette();

			m_pDlg->InvalidateRect(&brightRect,FALSE);
			DrawHSB(&dc);
		}
	}
}

//鼠标点击
void CMouseEvent::mouse_down(UINT nFlags, CPoint point)
{
	if(hsbRect.PtInRect(point))
	{
		m_bInMouse = FALSE;
		if(InCircle(point))
		{
			m_nMouseIn = IN_CIRCLE;
		}
		else
		{
			m_nMouseIn = IN_NOTHING;
		}
		if(m_nMouseIn)
		{
			m_pDlg->SetCapture();
			TrackPoint(point);
		}
	}

}

CMouseEvent::~CMouseEvent()
{
	if(m_HsbBitmap.GetSafeHandle())
	{
		m_HsbBitmap.DeleteObject();
	}
}

CMouseEvent::CMouseEvent()
{

}

void CMouseEvent::init(CDialog* dlg, COLORREF old_color)
{
	m_pDlg = dlg;

	Vertex = CPoint(102,108);
	Top = CPoint(102,9);
	Left = CPoint(23,147);
	Right = CPoint(181,147);

	color.r = GetRValue(old_color);
	color.g = GetGValue(old_color);
	color.b = GetBValue(old_color);

	m_OldColor = color;
	hsvColor = color.toHSV();
	m_bInMouse = FALSE;
	m_bInitOver = FALSE;
	m_bInDrawAll = FALSE;
}


//返回当前选择的结果
COLORREF CMouseEvent::GetColor() 
{ 
	return color.color();
}



void CMouseEvent::mouse_up(UINT nFlags, CPoint point)
{
	if(GetCapture() == m_pDlg->GetSafeHwnd())
	{
		ReleaseCapture();
		m_bInMouse = FALSE;
	}
}

void CMouseEvent::onPaint(CPaintDC *dc)
{
	DrawFilledColor(dc, OldColorRect, m_OldColor.color());
	DrawHSB(dc);
}


//
//鼠标点击颜色带时的小矩形
void CMouseEvent::DrawMarkers(CDC *pDC)
{
	if(m_CurrentRect.Width())
	{
		CPen *oldPen;
		CBrush *oldBrush;
		int oldMode;
		CRect cr = m_CurrentRect;

		oldPen = (CPen *)pDC->SelectStockObject(WHITE_PEN);
		oldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);

		oldMode = pDC->SetROP2(R2_XORPEN);

		pDC->Rectangle(&cr);

		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
		pDC->SetROP2(oldMode); 
	}
}

void CMouseEvent::DrawHSB(CDC *pDC)
{
	if(m_HsbBitmap.GetSafeHandle())
	{
		CBitmap *pOldBitmap ;
		pOldBitmap  = (CBitmap *)memDC.SelectObject(&m_HsbBitmap);
		pDC->BitBlt(hsbRect.left,hsbRect.top,
			hsbWidth,hsbHeight,&memDC,0,0,SRCCOPY);
		DrawMarkers(pDC);
		memDC.SelectObject(pOldBitmap);
	}
}

void CMouseEvent::DrawFilledColor(CDC *pDC,CRect cr,COLORREF c)
{
	pDC->FillSolidRect(&cr,c);
	pDC->Draw3dRect(&cr,RGB(0,0,0),RGB(0,0,0));
	cr.InflateRect(-1,-1);
	pDC->Draw3dRect(&cr,RGB(192,192,192),RGB(128,128,128));
}



void CMouseEvent::DrawLines(CDC *pDC)
{ 
	DrawFilledColor(pDC, NewColorRect,color.color());
}


void CMouseEvent::DrawAll()
{
	if(m_bInitOver && !m_bInDrawAll)
	{
		CClientDC dc(m_pDlg);
		DrawMarkers(&dc);
		DrawLines(&dc);
		m_bInDrawAll = TRUE;
		CalcRects();
		SetDIBPalette();
		DrawHSB(&dc);
		SetEditVals();
		m_bInDrawAll = FALSE;
	}
}



void CMouseEvent::CreateBrightDIB()
{
	CDIB& d = m_BrightDIB;
	d.Create(brightRect.Width(),brightRect.Height(),8);
	for(int i=0; i < d.Height(); i++)
	{
		memset(d.GetLinePtr(i),i,d.Width());
	}
}

void CMouseEvent::SetDIBPalette()
{
	BYTE palette[768],*p;
	HSVType h = hsvColor;
	double d;
	d = 255.0/brightRect.Height();
	p = palette;
	for(int i=brightRect.Height()-1; i >= 0 ;i--,p+=3)
	{
		h.v = (int)((double)i * d);
		RGBType rgb = h.toRGB();
		p[0] = rgb.r;
		p[1] = rgb.g;
		p[2] = rgb.b;
	}
	m_BrightDIB.SetPalette(palette);
}

void CMouseEvent::CalcRects()
{
	CPoint pt;
	pt = PtFromAngle(hsvColor.h,hsvColor.s,m_Centre);
	m_CurrentRect = CRect(pt.x - RECT_WIDTH,pt.y - RECT_WIDTH,pt.x+RECT_WIDTH,pt.y + RECT_WIDTH);	

	int y;
	y = (int)(((double)hsvColor.v/255)*brightRect.Height()); 	
	y = brightRect.bottom - y;
	brightMark = CRect(brightRect.left - 2, y - 4, brightRect.right+2,y+4);
}



void CMouseEvent::LoadMappedBitmap(CBitmap& bitmap,UINT nIdResource,CSize& size)
{
	CBitmap *pOldBitmap;

	if(bitmap.GetSafeHandle()) bitmap.DeleteObject();

	if(bitmap.LoadBitmap(nIdResource))
	{

		int width,height;
		BITMAP bmInfo;
		::GetObject(bitmap.m_hObject,sizeof(bmInfo),&bmInfo);	
		width = bmInfo.bmWidth;
		height = bmInfo.bmHeight;

		COLORREF colorWindow = ::GetSysColor(COLOR_3DFACE);
		COLORREF sourceColor = RGB(192,192,192);

		pOldBitmap = (CBitmap *)memDC.SelectObject(&bitmap);

		int i,j;

		for(i=0; i < height; i++)
		{
			for(j=0; j < width; j++)
			{
				if(memDC.GetPixel(j,i) == sourceColor)
				{
					memDC.SetPixel(j,i,colorWindow);
				}
			}
		}

		memDC.SelectObject(&pOldBitmap);
		size = CSize(width,height);
	}
}

//
void CMouseEvent::UpdateGColor(int g)
{
	if(g != color.g && m_bInitOver)
	{
		color.g = g;
		if(color.g < 0) color.g = 0;
		if(color.g > 255) color.g = 255;
		hsvColor = color.toHSV();
		DrawAll();
	}
}

void CMouseEvent::UpdateBColor(int b)
{
	if( b != color.b && m_bInitOver)
	{
		color.b = b;
		if(color.b < 0) color.b = 0;
		if(color.b > 255) color.b = 255;
		hsvColor = color.toHSV();
		DrawAll();
	}
}

void CMouseEvent::UpdateRColor(int r)
{
	if(r != color.r && m_bInitOver)
	{
		color.r = r;
		if(color.r < 0) color.r = 0;
		if(color.r > 255) color.r = 255;
		hsvColor = color.toHSV();
		DrawAll();
	}
}

//初始化结束
void CMouseEvent::EndInit()
{
	m_bInitOver = TRUE;
}

//初始化开始
void CMouseEvent::StartInit(int hsb_id, int old_id, int new_id, int bmp_id)
{
	m_pDlg->GetDlgItem(hsb_id)->GetWindowRect(&hsbRect);
	m_pDlg->ScreenToClient(&hsbRect);
	m_pDlg->GetDlgItem(old_id)->GetWindowRect(&OldColorRect);
	m_pDlg->GetDlgItem(new_id)->GetWindowRect(&NewColorRect);
	m_pDlg->ScreenToClient(&OldColorRect);
	m_pDlg->ScreenToClient(&NewColorRect);

	CWindowDC dc(NULL);
	CSize bmSize;

	//	Set Up HSB
	memDC.CreateCompatibleDC(&dc);

	LoadMappedBitmap(m_HsbBitmap,bmp_id,bmSize);
	hsbWidth = bmSize.cx;
	hsbHeight = bmSize.cy;

	hsbRect.InflateRect(-5,-5);
	hsbRect.top += 20;
	hsbRect.left += 10;

	m_Centre = CPoint(RADIUS,RADIUS);
	m_Centre += CPoint(hsbRect.left,hsbRect.top);

	brightRect = CRect(hsbRect.left+hsbWidth+20,hsbRect.top,hsbRect.left+hsbWidth+20+20,hsbRect.top + hsbHeight);
	CreateBrightDIB();
	CalcRects();
	SetDIBPalette();
}

//初始化拖拉控件
void CMouseEvent::InitSpinCtrl(int r_id, int g_id, int b_id)
{
	m_slider_r = r_id;
	m_slider_g = g_id;
	m_slider_b = b_id;
	((CSliderCtrl *)m_pDlg->GetDlgItem(r_id))->SetRange(0,255);
	((CSliderCtrl *)m_pDlg->GetDlgItem(g_id))->SetRange(0,255);
	((CSliderCtrl *)m_pDlg->GetDlgItem(b_id))->SetRange(0,255);

	SetEditVals();
}

 //设置拖拉控件值
void CMouseEvent::SetEditVals()
{
	((CSliderCtrl *)m_pDlg->GetDlgItem(m_slider_r))->SetPos(color.r);
	((CSliderCtrl *)m_pDlg->GetDlgItem(m_slider_g))->SetPos(color.g);
	((CSliderCtrl *)m_pDlg->GetDlgItem(m_slider_b))->SetPos(color.b);

	//
	CString str;
	str.Format("%d", color.r);
	m_pDlg->SetDlgItemText(m_edit_r, str);

	str.Format("%d", color.g);
	m_pDlg->SetDlgItemText(m_edit_g, str);

	str.Format("%d", color.b);
	m_pDlg->SetDlgItemText(m_edit_b, str);
}

void CMouseEvent::InitRGBEdit(int r_id, int g_id, int b_id)
{
	m_edit_r = r_id;
	m_edit_g = g_id;
	m_edit_b = b_id;
}
