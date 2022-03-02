#ifndef GAME_SERVER_MANAGER_HEAD_FILE
#define GAME_SERVER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//游戏服务器管理类
class CGameServiceManager : public IGameServiceManager
{
	//变量定义
protected:
	tagGameServiceAttrib				m_GameServiceAttrib;			//服务属性

	//函数定义
public:
	//构造函数
	CGameServiceManager(void);
	//析构函数
	virtual ~CGameServiceManager(void);

	//基础接口
public:
	//释放对象
	virtual VOID __cdecl Release() { return; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CGameServiceManager))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//参数接口
public:
	//获取属性
	virtual void __cdecl GetGameServiceAttrib(tagGameServiceAttrib & GameServiceAttrib);
	//修改参数
	virtual bool __cdecl RectifyServiceOption(tagGameServiceOption * pGameServiceOption);

	//创建接口
public:
	//创建桌子
	virtual VOID * __cdecl CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer);
	//创建机器
	virtual VOID * __cdecl CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer);
};

//////////////////////////////////////////////////////////////////////////

#endif