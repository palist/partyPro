
// PartyProProgram.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPartyProProgramApp:
// �йش����ʵ�֣������ PartyProProgram.cpp
//

class CPartyProProgramApp : public CWinApp
{
public:
	CPartyProProgramApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPartyProProgramApp theApp;