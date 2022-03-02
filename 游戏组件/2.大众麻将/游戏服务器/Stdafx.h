#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
#define WINVER 0x0602		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。
#define _WIN32_WINNT 0x0602	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0602 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。
#define _WIN32_IE 0x0602	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将为显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////

//组件头文件
#include "..\消息定义\CMD_Sparrow.h"
#include "..\..\开发库\Include\GameServiceExport.h"
#include "..\..\开发库\Include\Module.h"

//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
////产品版本号
//#define PRODUCT_VER1
//
////版本定义
//#define INTERFACE_VERSION(cbMainVer,cbSubVer)\
//(DWORD)(\
//(((BYTE)(PRODUCT_VER))<<24)+\
//(((BYTE)(cbMainVer))<<16)+\
//((BYTE)(cbSubVer)<<8))
//
////获取产品版本
//inline BYTE GetProductVer(DWORD dwVersion)
//{
//
//    BYTE * cbVersion = (BYTE *)&dwVersion;
//    return cbVersion[3];
//
//}
//
////获取主版本
//inline BYTE GetMainVer(DWORD dwVersion)
//{
//
//    BYTE * cbVersion = (BYTE *)&dwVersion;
//    return cbVersion[2];
//
//}
//
////获取子版本
//inline BYTE GetSubVer(DWORD dwVersion)
//{
//
//    BYTE * cbVersion = (BYTE *)&dwVersion;
//    return cbVersion[1];
//
//}
//
////获取编译版本
//inline BYTE GetBulidVer(DWORD dwVersion)
//{
//
//    BYTE * cbVersion = (BYTE *)&dwVersion;
//    return cbVersion[0];
//
//}
//
////版本对比函数
//inline bool InterfaceVersionCheck(DWORD dwQueryVer, DWORD dwInterfaceVer)
//{
//
//    if (GetProductVer(dwQueryVer) != GetProductVer(dwInterfaceVer)) return false;
//    if (GetMainVer(dwQueryVer) != GetMainVer(dwInterfaceVer)) return false;
//    if (GetSubVer(dwQueryVer)>GetSubVer(dwInterfaceVer)) return false;
//    return true;
//
//};
//
////////////////////////////////////////////////////////////////////////////
//
////接口定义
//#define STD_METHOD(TYPE,METHOD) virtual TYPE __cdecl METHOD
//
////接口组件创建函数说明
//typedef void * (__cdecl ModuleCreateFunc)(const GUID & Gudi, DWORD dwInterfaceVer);
//
////////////////////////////////////////////////////////////////////////////
//
//#define VER_IUnknownEx INTERFACE_VERSION(1,1)
//static const GUID IID_IUnknownEx = {
//    0x92d87e91, 0x5a5b, 0x4a45, 0x8c, 0xfd, 0x38, 0x2f, 0xe6, 0xb3, 0x2b, 0xb
//};
//
////基础接口
//interface IUnknownEx
//{
//
//    //是否有效
//    virtual bool __cdecl IsValid() = NULL;
//    //释放对象
//    virtual bool __cdecl Release() = NULL;
//    //接口查询
//    virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer) = NULL;
//
//};
//
////////////////////////////////////////////////////////////////////////////
//
////接口查询宏
//#define QUERYINTERFACE(Interface,Guid,dwQueryVer)\
//if ((Guid==IID_##Interface)&&(InterfaceVersionCheck(dwQueryVer,VER_##Interface)))\
//return static_cast<Interface *>(this);
//
//#define QUERYINTERFACE_IUNKNOWNEX(BaseInterface,Guid,dwQueryVer)\
//if ((Guid==IID_IUnknownEx)&&(InterfaceVersionCheck(dwQueryVer,VER_IUnknownEx)))\
//return static_cast<IUnknownEx *>(static_cast<BaseInterface *>(this));
//
////获取接口宏
//#define GET_MYSELF_INTERFACE(Interface)\
//((Interface *)QueryInterface(IID_##Interface,VER_##Interface))
//
//#define GET_OBJECT_INTERFACE(Object,Interface)\
//((Interface *)Object.QueryInterface(IID_##Interface,VER_##Interface))

#define GET_OBJECTPTR_INTERFACE(pObject,Interface)\
((pObject==NULL)?NULL:((Interface *)pObject->QueryInterface(IID_##Interface,VER_##Interface)))