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
	//	m_password = epass();//�����ʼ��
	file = new CFile;
	if ( !file->Open(fpath, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		return FALSE;
	}
	flen = (DWORD)file->GetLength();
	data = new char[(int)flen];//Ϊ�ļ������ڴ�
	file->SeekToBegin();
	file->Read(data, flen);
	//������ļ��������ֽڶ����м�����
	for(int i=0; i<(int)flen; i++)
	{
		data[i] ^= m_password;
		data[i] ^= flen;                //��Ϊÿ�μ��ܺ��ļ��Ĵ�С��������5�ֽ�,�������������ͬһ������м���Ҳû��ϵ
	}
	file->SeekToBegin();
	file->Write(data, flen);
	delete[] data;                          //���ͷ��ڴ�
	//���������֤��Ϣ
	char cpass[5] = "love";
	for(int j=0; j<5; j++)
	{
		cpass[j] ^= m_password;
	}
	file->SeekToEnd();
	file->Write(&cpass, 5);        //���ļ�β���������������,,�����ļ��ͻ�����5�ֽ�
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
//���ļ����ܵĺ���
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
	//���������ǲ�����ȷ
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
	//����
	file->SeekToBegin();
	file->Read(data, flen);
	//����ԭ���ķ������н���
	for(int j=0; j<(int)flen; j++)
	{
		data[j] ^= m_password;
		data[j] ^= (flen-5);
	}
	file->SeekToBegin();
	file->Write(data, flen);
	file->SetLength(flen-5);     //ɾ����������ӵ�������֤����
	file->Close();
	delete[] data;
	delete file;
	return TRUE;
}