#include "StdAfx.h"
#include "AtlBase.h"
#include "InitParamter.h"

//////////////////////////////////////////////////////////////////////////

CInitParamter::CInitParamter() 
{ 
	InitParamter();
}

CInitParamter::~CInitParamter()
{
}

//初始化
void CInitParamter::InitParamter()
{
	//网络设置
	m_wMaxConnect=512;
	m_wListenPort=PORT_CENTER_SERVER;

	//信息数据库
	m_wServerDataBasePort=1433;
	m_szServerDataBaseAddr[0]=0;
	m_szServerDataBaseUser[0]=0;
	m_szServerDataBasePass[0]=0;
	lstrcpyn(m_szServerDataBaseName,
				szServerInfoDB,
				CountArray(m_szServerDataBaseName));

	return ;
}

//加载配置
void CInitParamter::LoadInitParamter()
{
	//初始化参数
	InitParamter();

	//打开注册表
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_snwprintf(szRegName,
				sizeof(szRegName),
				TEXT("Software\\%s\\CenterServer"),
				g_szSystemRegKey);
	if (RegParamter.Open(HKEY_CURRENT_USER,szRegName,KEY_READ)!=ERROR_SUCCESS) 
	{
		return ;
	}

	//变量定义
	TCHAR szReadData[1024]=TEXT("");
	DWORD dwDataType,dwReadData,dwDataSize;

	//最大连接
	dwDataSize=sizeof(dwReadData);
	LONG lErrorCode = RegParamter.QueryValue(TEXT("MaxConnect"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		m_wMaxConnect=(WORD)dwReadData;
	}

	//监听端口
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ListenPort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		m_wListenPort=(WORD)dwReadData;
	}

	//数据库端口
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerDataBasePort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		m_wServerDataBasePort=(WORD)dwReadData;
	}

	//数据库地址
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
    lErrorCode = RegParamter.QueryValue(TEXT("ServerDataBaseAddr"), &dwDataType, m_szServerDataBaseAddr, &dwDataSize);
	if (lErrorCode==ERROR_SUCCESS)
	{
		//CXOREncrypt::CrevasseData(szReadData,m_szServerDataBaseAddr, ARRAYSIZE(m_szServerDataBaseAddr));
	}

	//数据库用户名
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
    lErrorCode = RegParamter.QueryValue(TEXT("ServerDataBaseUser"), &dwDataType, m_szServerDataBaseUser, &dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		//CXOREncrypt::CrevasseData(szReadData,m_szServerDataBaseUser, ARRAYSIZE(m_szServerDataBaseUser));
	}

	//数据库密码
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
    lErrorCode = RegParamter.QueryValue(TEXT("ServerDataBasePass"), &dwDataType, m_szServerDataBasePass, &dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		//CXOREncrypt::CrevasseData(szReadData,m_szServerDataBasePass, ARRAYSIZE(m_szServerDataBasePass));
	}

	//数据库名字
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
    lErrorCode = RegParamter.QueryValue(TEXT("ServerDataBaseName"), &dwDataType, m_szServerDataBaseName, &dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) 
	{
		//CXOREncrypt::CrevasseData(szReadData,m_szServerDataBaseName, ARRAYSIZE(m_szServerDataBaseName));
	}

	return;
}

//保存配置
void CInitParamter::SaveInitParamter(bool bSaveAll)
{
	//打开注册表
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_snwprintf(szRegName, sizeof(szRegName), TEXT("Software\\%s\\CenterServer"), g_szSystemRegKey);
	if (RegParamter.Create(HKEY_CURRENT_USER,szRegName)!=ERROR_SUCCESS) return;

	//变量定义
	TCHAR szWriteBuffer[1024]=TEXT("");

    //加密和解密函数在从char变成wchar_t时不能发生匹配了，暂且不加解密了, by zhangyl
	//信息数据库
	RegParamter.SetDWORDValue(TEXT("ServerDataBasePort"),m_wServerDataBasePort);
	//CXOREncrypt::EncryptData(m_szServerDataBaseAddr,szWriteBuffer,ARRAYSIZE(szWriteBuffer));
    RegParamter.SetKeyValue(TEXT(""), m_szServerDataBaseAddr, TEXT("ServerDataBaseAddr"));
	//CXOREncrypt::EncryptData(m_szServerDataBaseUser,szWriteBuffer, ARRAYSIZE(szWriteBuffer));
    RegParamter.SetKeyValue(TEXT(""), m_szServerDataBaseUser, TEXT("ServerDataBaseUser"));
	//CXOREncrypt::EncryptData(m_szServerDataBasePass,szWriteBuffer, ARRAYSIZE(szWriteBuffer));
    RegParamter.SetKeyValue(TEXT(""), m_szServerDataBasePass, TEXT("ServerDataBasePass"));

	if (bSaveAll==true)
	{
		//网络配置
		RegParamter.SetDWORDValue(TEXT("MaxConnect"),m_wMaxConnect);
		RegParamter.SetDWORDValue(TEXT("ListenPort"),m_wListenPort);

		//信息数据库
		//CXOREncrypt::EncryptData(m_szServerDataBaseName,szWriteBuffer, ARRAYSIZE(szWriteBuffer));
        RegParamter.SetKeyValue(TEXT(""), m_szServerDataBaseName, TEXT("ServerDataBaseName"));
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
