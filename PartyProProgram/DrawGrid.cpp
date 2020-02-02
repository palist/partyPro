#include "stdafx.h"
#include "DrawGrid.h"
#include <math.h>

//该变量，代表格子间距
const int g_dis = 20;

CDrawGrid::CDrawGrid(void)
{
	m_mouse_flag = FALSE;
}

CDrawGrid::~CDrawGrid(void)
{
}

void CDrawGrid::init(CDialog *dlg, int id)
{
	m_ctrl_draw_id = id;
	m_pDlg = dlg;
	CWnd *pWnd = dlg->GetDlgItem(id);
	if(pWnd == NULL)
		return;

	pWnd->GetWindowRect(&m_draw_rect);
	dlg->ScreenToClient(&m_draw_rect);
	m_cir_point.x = m_draw_rect.left + 128;
	m_cir_point.y = m_draw_rect.top + 128;
}

//画大板的X轴
void CDrawGrid::draw_x_axis(CPaintDC *hDC)
{
	CPen penBlack;
	penBlack.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	CPen *pOldPen=hDC->SelectObject(&penBlack);
	 
	int i = m_draw_rect.left;
	int wid = m_draw_rect.right;

	int x_ave = wid / g_dis;

	int top = m_draw_rect.top;
	int bottom = m_draw_rect.bottom;
	//画X轴
	for (i; i < wid; i+= g_dis)
	{
		 hDC->MoveTo(i, top);
		 hDC->LineTo(i, bottom);
	}

	hDC->SelectObject(pOldPen);
}

//画大板的X轴
void CDrawGrid::draw_y_axis(CPaintDC *hDC)
{
	CPen penBlack;
	penBlack.CreatePen(PS_DOT, 1, RGB(255,0,0));
	CPen *pOldPen=hDC->SelectObject(&penBlack);

	//画Y轴
	int left = m_draw_rect.left; 
	int right = m_draw_rect.right;
	int top = m_draw_rect.top;
	int bottom = m_draw_rect.bottom;

	for (top; top < bottom; top+= g_dis)
	{
		hDC->MoveTo(left, top);
		hDC->LineTo(right, top);
	}

	hDC->SelectObject(pOldPen);
}

void CDrawGrid::draw_cir(CPaintDC *pDC)
{
	int px = m_cir_point.x;
	int py = m_cir_point.y;
	int pt_size = 5;
	CPen penBlack;//定义一个画笔变量
	penBlack.CreatePen(PS_NULL, 1, RGB(255,0,0));//创建画笔
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 0, 0));
	pDC->SelectObject(brush);

	CPen *oldPen=pDC->SelectObject(&penBlack);
	{
		pDC->Ellipse(px-pt_size, 
			py-pt_size, 
			px+pt_size, 
			py+pt_size);
	}
	pDC->SelectObject(oldPen);
}

void CDrawGrid::draw_rect(CPaintDC *dc)
{
	CPen penBlack;//定义一个画笔变量
	penBlack.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));//创建画笔
	CPen *oldPen = dc->SelectObject(&penBlack);

	//
	dc->MoveTo(m_draw_rect.left, m_draw_rect.top-2);
	dc->LineTo(m_draw_rect.left, m_draw_rect.bottom);
	dc->MoveTo(m_draw_rect.left - 2, m_draw_rect.top);
	dc->LineTo(m_draw_rect.right, m_draw_rect.top);

	dc->SelectObject(oldPen);
}

void CDrawGrid::on_paint(CPaintDC *dc)
{
	draw_rect(dc);
	draw_x_axis(dc);
	draw_y_axis(dc);

	//画一个红色圆球
	draw_cir(dc);
}

void CDrawGrid::mouse_down(CPoint point)
{
	m_mouse_flag = TRUE;
	cal_point(point); 
}

void CDrawGrid::mouse_up(CPoint point)
{
	m_mouse_flag = FALSE;
}

void CDrawGrid::mouse_move(CPoint point)
{
	if(m_mouse_flag)
	{
		cal_point(point); 
	}
}

//获取实际坐标
POINT CDrawGrid::get_true_position(POINT pos, int left, int top)
{
	POINT ret_pt;
	ret_pt.x = pos.x;
	ret_pt.y = pos.y;
	return ret_pt;

	// -- bak
	double dx=pos.x;
	double dy=pos.y;
	double x_unit= g_dis;
	double y_unit= g_dis;
	int cx=(int)(dx/x_unit);
	int cy=(int)(dy/y_unit);
	if(dx>0.0)
	{
		if(fabs(dx-cx*x_unit)<fabs(dx-(cx+1)*x_unit))
			dx=cx*x_unit;
		else
			dx=(cx+1)*x_unit;
	}
	else
	{
		if(fabs(dx-cx*x_unit)<fabs(dx-(cx-1)*x_unit))
			dx=cx*x_unit;
		else
			dx=(cx-1)*x_unit;
	}
	if(dy>0.0)
	{
		if(fabs(dy-cy*y_unit)<fabs(dy-(cy+1)*y_unit))
			dy=cy*y_unit;
		else
			dy=(cy+1)*y_unit;
	}
	else
	{
		if(fabs(dy-cy*y_unit)<fabs(dy-(cy-1)*y_unit))
			dy=cy*y_unit;
		else
			dy=(cy-1)*y_unit;
	}

	POINT tmp;
	tmp.x = left +dx;
	tmp.y = top + dy;
	return tmp;
}

RECT toRect(POINT pt)
{
	RECT tmp;
	tmp.left = pt.x - 5;
	tmp.right = pt.x + 5;
	tmp.top = pt.y - 5;
	tmp.bottom = pt.y + 5;
	return tmp;
}

//点是否在绘制区域内
bool is_in_rect(RECT rect, POINT pt)
{
	if (pt.x < rect.left)
		return false;
	if (pt.x > rect.right)
		return false;
	if (pt.y < rect.top)
		return false;
	if (pt.y > rect.bottom)
		return false;

	return true;
}

void CDrawGrid::cal_point(CPoint point)
{
	//if (!is_in_rect(m_draw_rect, point))
	//	return;

	//把旧的红点删除
	m_pDlg->InvalidateRect(&toRect(m_cir_point), TRUE);

	POINT tmppt;
	tmppt.x = point.x;
	tmppt.y = point.y;

	if (point.x < m_draw_rect.left) {
		tmppt.x = m_draw_rect.left;
		m_mouse_flag = FALSE;
	}
	if (point.x > m_draw_rect.right){
		tmppt.x = m_draw_rect.right;
		m_mouse_flag = FALSE;
}
	if (point.y < m_draw_rect.top){
		tmppt.y = m_draw_rect.top;
		m_mouse_flag = FALSE;
	}
	if (point.y > m_draw_rect.bottom){
		tmppt.y = m_draw_rect.bottom;
		m_mouse_flag = FALSE;
	}


	m_cir_point = get_true_position(tmppt, m_draw_rect.left, m_draw_rect.top);
	 
	//把新的红点画出来
	m_pDlg->InvalidateRect(&toRect(m_cir_point), TRUE);

	//
	show_position_txt();
}

//
void CDrawGrid::set_ctrl(int start, int end)
{
	m_ctrl_start_id = start;
	m_ctrl_end_id = end;
}

//显示坐标，把坐标映射到256，256的坐标系中
void CDrawGrid::show_position_txt()
{
	double x = m_cir_point.x - m_draw_rect.left;
	double y = m_cir_point.y - m_draw_rect.top;
	int x_pos = (x / m_draw_rect.Width()) * 255.0;
	int y_pos = (y / m_draw_rect.Height()) * 255.0;

	m_pDlg->SetDlgItemInt(m_ctrl_start_id, x_pos);
	m_pDlg->SetDlgItemInt(m_ctrl_end_id, y_pos);
}
