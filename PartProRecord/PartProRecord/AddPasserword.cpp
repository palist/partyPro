#include "StdAfx.h"
#include "AddPasserword.h"

CAddPasserword::CAddPasserword(void)
{
	 m_password = 10801080;
}

CAddPasserword::~CAddPasserword(void)
{
}
BOOL CAddPasserword::ecfile(LPCTSTR fpath)
{
	char *data;
	CFile *file;
	DWORD flen;
	//	m_password = epass();//密码初始化
	file = new CFile;
	if ( !file->Open(fpath, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		return FALSE;
	}
	flen = (DWORD)file->GetLength();
	data = new char[(int)flen];//为文件分配内存
	file->SeekToBegin();
	file->Read(data, flen);
	//这里把文件的所有字节都进行加密了
	for(int i=0; i<(int)flen; i++)
	{
		data[i] ^= m_password;
		data[i] ^= flen;                //因为每次加密后文件的大小都会增加5字节,所以如果两次用同一密码进行加密也没关系
	}
	file->SeekToBegin();
	file->Write(data, flen);
	delete[] data;                          //先释放内存
	//添加密码验证信息
	char cpass[5] = "love";
	for(int j=0; j<5; j++)
	{
		cpass[j] ^= m_password;
	}
	file->SeekToEnd();
	file->Write(&cpass, 5);        //在文件尾添加密码严整部分,,这样文件就会增加5字节
	file->Close();
	delete file;
	return TRUE;
}
/*
UINT64 CAddPasserword::epass()
{
		DWORD plen;
		char *ppass;
		UINT64 mc= 1112223334445;
	
		plen = strlen(ppass);
		for(int i=0; i<(int)plen; i++)
		{
				mc ^= ppass[i]|128;
		}
			return mc;
}*/
//给文件解密的函数
BOOL CAddPasserword::dcfile(LPCTSTR fpath)
{
	char *data;
	CFile *file;
	DWORD flen;
	char love[5];
	file = new CFile;
	if( !file->Open(fpath, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		return FALSE;
	}
	flen = (DWORD)file->GetLength();
	data = new char[(int)flen];
	//检验密码是不是正确
	file->Seek(-5, CFile::end);
	file->Read(&love, 5);
	//		m_password = epass();
	for(int i=0; i<5; i++)
	{
		love[i] ^= m_password;
	}
	if(strcmp(love, "love")!=0)
	{
		return FALSE;
	}
	//解密
	file->SeekToBegin();
	file->Read(data, flen);
	//按照原来的方法进行解密
	for(int j=0; j<(int)flen; j++)
	{
		data[j] ^= m_password;
		data[j] ^= (flen-5);
	}
	file->SeekToBegin();
	file->Write(data, flen);
	file->SetLength(flen-5);     //删除加密是添加的密码验证部分
	file->Close();
	delete[] data;
	delete file;
	return TRUE;
}