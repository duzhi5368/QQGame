#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//应用程序对象
CGameClientApp theApp;
#ifdef VIDEO_GAME
CVideoServiceManager g_VideoServiceManager;
#endif
//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientApp::CGameClientApp()
{
}

//析构函数
CGameClientApp::~CGameClientApp() 
{
}

//////////////////////////////////////////////////////////////////////////
