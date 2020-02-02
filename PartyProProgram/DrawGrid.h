#pragma once


//
// ���Ʊ��
//
class CDrawGrid
{
public:
	CDrawGrid(void);
	
	~CDrawGrid(void);
 
	//�ؼ�id
private:
	int m_ctrl_start_id;	//X������ʾ�õĿؼ�
	int m_ctrl_end_id;		//Y������ʾ�õĿؼ�
	int m_ctrl_draw_id;		//��ͼ�����Ӧ�Ŀؼ�

private:

	CDialog	*m_pDlg;

	CRect	m_draw_rect;

	//Բ������
	POINT	m_cir_point;

	//���״̬
	BOOL	m_mouse_flag;

private:

	//����������
	void cal_point(CPoint point);
	POINT get_true_position(POINT pos, int left, int top);

	//��������ʾ����
	void show_position_txt();


	//�ⲿ��Ҫ���õļ����¼�����
public:
	void on_paint(CPaintDC *dc);
	void mouse_down(CPoint point);
	void mouse_up(CPoint point);
	void mouse_move(CPoint point);

	//��ʼ��ʱ����
public:
	void init(CDialog *dlg, int id);
	void set_ctrl(int start, int end);

	//��ͼ��
private:
	void draw_y_axis(CPaintDC *hDC);
	void draw_x_axis(CPaintDC *hDC);
	void draw_cir(CPaintDC *dc);
	void draw_rect(CPaintDC *dc);
};
