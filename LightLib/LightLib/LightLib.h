
// LightLib.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLightLibApp:
// �йش����ʵ�֣������ LightLib.cpp
//

class CLightLibApp : public CWinApp
{
public:
	CLightLibApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLightLibApp theApp;