#pragma once

#include "Dib.h"
#include "Common.h"
 
class CDialog;

//鼠标事件
class CMouseEvent
{

public:
	CMouseEvent();
	~CMouseEvent();

	CDialog *m_pDlg;

public:
	//返回当前选择的结果
	COLORREF GetColor() ;

	// 鼠标
public:
	//鼠标移动事件
	void mouse_move(UINT nFlags, CPoint point);

	//鼠标点击事件
	void mouse_down(UINT nFlags, CPoint point);

	//鼠标弹起
	void mouse_up(UINT nFlags, CPoint point);

	//绘制
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

	//判断当前鼠标是否点在彩虹上
	void TrackPoint(CPoint pt);

	//绘制彩色带上的小矩形
	void DrawMarkers(CDC *pDC);

	void DrawHSB(CDC *pDC);

	//填充当前选择的颜色
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

	//拖拉控件
	int m_slider_r;
	int m_slider_g;
	int m_slider_b;

	//RGB值显示控件
	int m_edit_r;
	int m_edit_g;
	int m_edit_b;
};

