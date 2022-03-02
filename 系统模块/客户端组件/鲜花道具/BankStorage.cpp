#include "Stdafx.h"
#include "resource.h"
#include "BankStorage.h"
#include ".\bankstorage.h"

//////////////////////////////////////////////////////////////////////////

//定时器I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//更新界面
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//更新界面

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBankStorage, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CBankDrawing, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CBankStorage::CBankStorage(CWnd* pParent):CDialog(IDD_BANK_STORAGE, pParent)
{
	m_lInCount=0;
	m_lGameGold=0;
	m_lStorageGold=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_pClientSocket=NULL;
}

//析构函数
CBankStorage::~CBankStorage()
{
}

//控件绑定
void CBankStorage::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Text(pDX, IDC_GAME_GOLD, m_strGameGold);
	DDX_Text(pDX, IDC_STORAGE_GOLD, m_strStorageGold);
}

//提取按钮
void CBankStorage::OnBnClickedOk()
{
	UpdateData(TRUE);

	//参数验证
	if (IsButtonSelected(IDC_DRAWOUT) && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"), m_lStorageGold);
		else strMessage.Format(TEXT("你的存储游戏币数目为0,不能进行提取操作!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("系统消息")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//参数验证
	if (IsButtonSelected(IDC_STORAGE) && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"),m_lGameGold );
		else strMessage.Format(TEXT("你的当前游戏币数目为0,不能进行存储操作!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("系统消息")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//密码验证
	//TCHAR szPassword[PASS_LEN]=TEXT("");
	//GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));
	//if (szPassword[0]==0)
	//{
	//	ShowInformationEx(TEXT("密码不能为空，请重新输入密码！"),0 ,MB_ICONINFORMATION,TEXT("系统消息")) ;
	//	GetDlgItem(IDC_USER_PASSWORD)->SetWindowText("");
	//	GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
	//	return;
	//}
	//TCHAR szTempPassword[PASS_LEN]=TEXT("");
	//CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	//CMD5Encrypt::EncryptData(szTempPassword,szPassword);

	//if (IsButtonSelected(IDC_DRAWOUT))
	//{
	//	//发送信息
	//	CMD_GF_BankGet BankGetGold;
	//	ZeroMemory(&BankGetGold, sizeof(BankGetGold));
	//	BankGetGold.lGetValue = m_lInCount;
	//	CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));

	//	SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	//}
	//else
	{
		//发送信息
		CMD_GF_BankStorage BankStorage;
		ZeroMemory(&BankStorage, sizeof(BankStorage));
		BankStorage.lStorageValue = m_lInCount;
		//CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	}

	SetDlgItemText(IDC_IN_COUNT,TEXT(""));

	return;
}

//初始化函数
BOOL CBankStorage::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateData(FALSE);

	SetWindowText(TEXT("银行"));

	//设置数值
	SetButtonSelected(IDC_STORAGE,true);

	//定时更新
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	if (NULL==m_brBakcground.m_hObject) m_brBakcground.CreateSolidBrush(pSkinAttribute->m_crBackGround);

	return TRUE;
}

//更新界面
void CBankStorage::UpdateView()
{
	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	LONG OutNum = 0;
	MakeStringToNum(strinCount,OutNum);
	m_lInCount=OutNum;
	if(OutNum!=0)
	{
		MakeString(strinCount,OutNum);
		SetDlgItemText(IDC_IN_COUNT,strinCount);
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strinCount.GetLength(),strinCount.GetLength(),TRUE); 
	}
	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//设置信息
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	MakeString(m_strGameGold,m_lGameGold);
	MakeString(m_strStorageGold,m_lStorageGold);

	UpdateData(FALSE);
}

//鼠标信息
void CBankStorage::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);
}

//鼠标信息
void CBankStorage::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
}

//命令信息
BOOL CBankStorage::PreTranslateMessage(MSG* pMsg)
{
	//过滤字母
	if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}

	////过滤回车
	//if(pMsg->message==WM_KEYDOWN)
	//{
	//	BYTE bMesValue = (BYTE)(pMsg->wParam);
	//	if(bMesValue==VK_RETURN) return TRUE;
	//}

	return __super::PreTranslateMessage(pMsg);
}

//时间消息
void CBankStorage::OnTimer(UINT nIDEvent)
{
	//时间消息
	switch(nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:		//更新界面
		{
			//更新界面
			UpdateView();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//输入信息
void CBankStorage::OnEnChangeInCount()
{
	//CString strCount;
	//GetDlgItem(IDC_IN_COUNT)->GetWindowText(strCount);
	//m_lInCount=_tstoi64(strCount);

	return ;
}

//是否选中
bool CBankStorage::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//选中按钮
void CBankStorage::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//设置信息
void CBankStorage::SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData)
{
	ASSERT((pClientKernel==NULL && pClientSocket!=NULL)||(pClientKernel!=NULL && pClientSocket==NULL));

	//设置信息
	m_pIClientKernel = pClientKernel;
	m_pClientSocket= pClientSocket;
	m_pMeUserData= pMeUserData;

	return;
}

//发送信息
void CBankStorage::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT((m_pIClientKernel==NULL && m_pClientSocket!=NULL)||(m_pIClientKernel!=NULL && m_pClientSocket==NULL));

	//发送信息
	if(m_pClientSocket!=NULL)m_pClientSocket->SendData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	else m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////


void CBankStorage::OnPaint()
{
	CPaintDC dc(this); 

	//绘画背景
	CRect ClipRect,ClientRect;
	dc.GetClipBox(&ClipRect);
	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	dc.FillSolidRect(&ClipRect,pSkinAttribute->m_crBackGround);
}

HBRUSH CBankStorage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (CTLCOLOR_STATIC==nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_brBakcground;
	}
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CBankDrawing::CBankDrawing(CWnd* pParent):CDialog(IDD_BANK_DRAWING, pParent)
{
	m_lInCount=0;
	m_lGameGold=0;
	m_lStorageGold=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_pClientSocket=NULL;
}

//析构函数
CBankDrawing::~CBankDrawing()
{
}

//控件绑定
void CBankDrawing::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Text(pDX, IDC_GAME_GOLD, m_strGameGold);
	DDX_Text(pDX, IDC_STORAGE_GOLD, m_strStorageGold);
}

//提取按钮
void CBankDrawing::OnBnClickedOk()
{
	UpdateData(TRUE);

	//参数验证
	if (IsButtonSelected(IDC_DRAWOUT) && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"), m_lStorageGold);
		else strMessage.Format(TEXT("你的存储游戏币数目为0,不能进行提取操作!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("系统消息")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//参数验证
	if (IsButtonSelected(IDC_STORAGE) && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"),m_lGameGold );
		else strMessage.Format(TEXT("你的当前游戏币数目为0,不能进行存储操作!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("系统消息")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//密码验证
	TCHAR szPassword[PASS_LEN]=TEXT("");
	GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));
	if (szPassword[0]==0)
	{
		ShowInformationEx(TEXT("密码不能为空，请重新输入密码！"),0 ,MB_ICONINFORMATION,TEXT("系统消息")) ;
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText(_T(""));
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}
	TCHAR szTempPassword[PASS_LEN]=TEXT("");
	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	CMD5Encrypt::EncryptData(szTempPassword,szPassword);

	if (IsButtonSelected(IDC_DRAWOUT))
	{
		//发送信息
		CMD_GF_BankGet BankGetGold;
		ZeroMemory(&BankGetGold, sizeof(BankGetGold));
		BankGetGold.lGetValue = m_lInCount;
		CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	}
	else
	{
		//发送信息
		CMD_GF_BankStorage BankStorage;
		ZeroMemory(&BankStorage, sizeof(BankStorage));
		BankStorage.lStorageValue = m_lInCount;
//		CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	}

	SetDlgItemText(IDC_USER_PASSWORD,TEXT(""));
	SetDlgItemText(IDC_IN_COUNT,TEXT(""));

	return;
}

//初始化函数
BOOL CBankDrawing::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateData(FALSE);

	SetWindowText(TEXT("银行"));

	//设置数值
	SetButtonSelected(IDC_DRAWOUT,true);

	//定时更新
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	if (NULL==m_brBakcground.m_hObject) m_brBakcground.CreateSolidBrush(pSkinAttribute->m_crBackGround);

	return TRUE;
}

//更新界面
void CBankDrawing::UpdateView()
{
	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	LONG OutNum = 0;
	MakeStringToNum(strinCount,OutNum);
	m_lInCount=OutNum;
	if(OutNum!=0)
	{
		MakeString(strinCount,OutNum);
		SetDlgItemText(IDC_IN_COUNT,strinCount);
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strinCount.GetLength(),strinCount.GetLength(),TRUE); 
	}

	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//设置信息
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	MakeString(m_strGameGold,m_lGameGold);
	MakeString(m_strStorageGold,m_lStorageGold);

	UpdateData(FALSE);
}

//鼠标信息
void CBankDrawing::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);
}

//鼠标信息
void CBankDrawing::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
}

//命令信息
BOOL CBankDrawing::PreTranslateMessage(MSG* pMsg)
{
	//过滤字母
	if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//时间消息
void CBankDrawing::OnTimer(UINT nIDEvent)
{
	//时间消息
	switch(nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:		//更新界面
		{
			//更新界面
			UpdateView();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//输入信息
void CBankDrawing::OnEnChangeInCount()
{
	//CString strCount;
	//GetDlgItem(IDC_IN_COUNT)->GetWindowText(strCount);
	//m_lInCount=_tstoi64(strCount);

	return ;
}

//是否选中
bool CBankDrawing::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//选中按钮
void CBankDrawing::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//设置信息
void CBankDrawing::SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData)
{
	ASSERT((pClientKernel==NULL && pClientSocket!=NULL)||(pClientKernel!=NULL && pClientSocket==NULL));

	//设置信息
	m_pIClientKernel = pClientKernel;
	m_pClientSocket= pClientSocket;
	m_pMeUserData= pMeUserData;

	return;
}

//发送信息
void CBankDrawing::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT((m_pIClientKernel==NULL && m_pClientSocket!=NULL)||(m_pIClientKernel!=NULL && m_pClientSocket==NULL));

	//发送信息
	if(m_pClientSocket!=NULL)m_pClientSocket->SendData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	else m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

void CBankDrawing::OnPaint()
{
	CPaintDC dc(this); 

	//绘画背景
	CRect ClipRect,ClientRect;
	dc.GetClipBox(&ClipRect);
	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	dc.FillSolidRect(&ClipRect,pSkinAttribute->m_crBackGround);
}

HBRUSH CBankDrawing::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (CTLCOLOR_STATIC==nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_brBakcground;
	}
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

void MakeString(CString &strNum,LONG lNumber)
{
	CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	bool blfirst = true;
	bool bLongNum = false;
	if(lNumber>=1000)
	{
		bLongNum = true;
	}
	while (lNumber>0)
	{
		
		strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==3)
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;

			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);

			}
			
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(bLongNum)
	    	strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	strNum = strNumber ;


}

void MakeStringToNum(CString str,LONG &Num)
{

	CString str1;
	int Pos = 0;
	int len = 0;
	int Start = 0;
	while (1)
	{
		len = str.GetLength();
		Pos = str.Find(_T(","),Pos);
		if(Pos==-1)
		{
			str1+=str;
			break;
		}else
		{
			
			char Tmp[126];
			memset(Tmp,0,126);
			memcpy(Tmp,&str.GetBuffer()[Start],Pos);
			if(Pos+1<=str.GetLength())
			{
				str = &str.GetBuffer()[Pos+1];
				str1+=Tmp;
				Start =  0;
				Pos = 0;

			}else
			{
				break;
			}
			
			
		}
	}

	_snwscanf(str1.GetBuffer(),str1.GetLength(),_TEXT("%ld"),&Num);
	

}