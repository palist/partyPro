#pragma once

class CAddPasserword
{
public:
	UINT64 m_password;

	CAddPasserword(void);
	~CAddPasserword(void);

	UINT64 epass();        //�����ʼ������
	BOOL ecfile(LPCTSTR fpath);//�ļ����ܺ���
	BOOL dcfile(LPCTSTR fpath);//�ļ����ܺ���
};
