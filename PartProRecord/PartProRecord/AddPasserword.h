#pragma once

class CAddPasserword
{
public:
	UINT64 m_password;

	CAddPasserword(void);
	~CAddPasserword(void);

	UINT64 epass();        //密码初始化函数
	BOOL ecfile(LPCTSTR fpath);//文件加密函数
	BOOL dcfile(LPCTSTR fpath);//文件解密函数
};
