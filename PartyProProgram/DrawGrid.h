#pragma once


//
// 绘制表格
//
class CDrawGrid
{
public:
	CDrawGrid(void);
	
	~CDrawGrid(void);
 
	//控件id
private:
	int m_ctrl_start_id;	//X坐标显示用的控件
	int m_ctrl_end_id;		//Y坐标显示用的控件
	int m_ctrl_draw_id;		//绘图区域对应的控件

private:

	CDialog	*m_pDlg;

	CRect	m_draw_rect;

	//圆球坐标
	POINT	m_cir_point;

	//鼠标状态
	BOOL	m_mouse_flag;

private:

	//计算红点坐标
	void cal_point(CPoint point);
	POINT get_true_position(POINT pos, int left, int top);

	//把坐标显示出来
	void show_position_txt();


	//外部需要调用的几个事件函数
public:
	void on_paint(CPaintDC *dc);
	void mouse_down(CPoint point);
	void mouse_up(CPoint point);
	void mouse_move(CPoint point);

	//初始化时调用
public:
	void init(CDialog *dlg, int id);
	void set_ctrl(int start, int end);

	//画图用
private:
	void draw_y_axis(CPaintDC *hDC);
	void draw_x_axis(CPaintDC *hDC);
	void draw_cir(CPaintDC *dc);
	void draw_rect(CPaintDC *dc);
};
