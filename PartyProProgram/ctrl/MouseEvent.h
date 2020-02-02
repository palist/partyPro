#pragma once

#include "Dib.h"
#include "Common.h"
 
class CDialog;

//����¼�
class CMouseEvent
{

public:
	CMouseEvent();
	~CMouseEvent();

	CDialog *m_pDlg;

public:
	//���ص�ǰѡ��Ľ��
	COLORREF GetColor() ;

	// ���
public:
	//����ƶ��¼�
	void mouse_move(UINT nFlags, CPoint point);

	//������¼�
	void mouse_down(UINT nFlags, CPoint point);

	//��굯��
	void mouse_up(UINT nFlags, CPoint point);

	//����
public:
	
	void onPaint(CPaintDC *dc);

	//
	void DrawAll();

	//
	void init(CDialog* dlg, COLORREF old_color);


public:
	void UpdateGColor(int g);
	void UpdateBColor(int b);
	void UpdateRColor(int r);

public:
	void EndInit();

	void StartInit(int hsb_id, int old_id, int new_id, int bmp_id);

	void InitSpinCtrl(int r_id, int g_id, int b_id);

	void InitRGBEdit(int r_id, int g_id, int b_id);

private:
	BOOL InCircle(CPoint pt);

	//�жϵ�ǰ����Ƿ���ڲʺ���
	void TrackPoint(CPoint pt);

	//���Ʋ�ɫ���ϵ�С����
	void DrawMarkers(CDC *pDC);

	void DrawHSB(CDC *pDC);

	//��䵱ǰѡ�����ɫ
	void DrawFilledColor(CDC *pDC,CRect cr,COLORREF c);

	//
	void DrawLines(CDC *pDC);

	void CreateBrightDIB();
	void SetDIBPalette();
	void CalcRects();

	void LoadMappedBitmap(CBitmap& bitmap,UINT nIdResource,CSize& size);
	void SetEditVals();

private:
	RGBType color, m_OldColor;

	CPoint Vertex,Top,Left,Right;

	BOOL m_bInMouse;
	BOOL m_bInitOver,m_bInDrawAll;


	CBitmap m_HsbBitmap;

	CDC memDC;
	CPoint m_Centre;
	CDIB m_BrightDIB;
	int rgbWidth,rgbHeight;
	int hsbWidth,hsbHeight;

	int m_nMouseIn;
	CRect m_CurrentRect,brightMark	;
	CRect brightRect;

	HSVType hsvColor;	


	CRect  rects[3];
	CPoint m_Cuboid[8];
	int nIndex;
	int RedLen,GreenLen,BlueLen;
	LineDesc lines[3];


	CRect hsbRect;
	CRect OldColorRect,NewColorRect;

	//�����ؼ�
	int m_slider_r;
	int m_slider_g;
	int m_slider_b;

	//RGBֵ��ʾ�ؼ�
	int m_edit_r;
	int m_edit_g;
	int m_edit_b;
};

