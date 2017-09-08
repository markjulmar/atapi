// PhoneDlg.cpp : implementation file
//
// This is a part of the TAPI Applications Classes C++ library.
// Original Copyright © 1995-2004 JulMar Entertainment Technology, Inc. All rights reserved.
//
// "This program is free software; you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation; version 2 of the License.
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General 
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program; if not, write 
// to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. 
// Or, contact: JulMar Technology, Inc. at: info@julmar.com." 
//

#include "stdafx.h"
#include "Phone.h"
#include "PhoneDlg.h"
#include "CallDataDlg.h"
#include "UUIDlg.h"
#include "DialDlg.h"
#include "AgentStateDlg.h"
#include "ConfListDlg.h"
#include "TransferDlg.h"
#include "CompleteTransferDlg.h"
#include "CompleteCall.h"
#include "SetupConfDlg.h"
#include "PhoneCapsDlg.h"
#include "PredDialDlg.h"
#include "ISDNInfo.h"
#include "QOSInfo.h"
#include "ForwardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Globals

CMainApp theApp;
DWORD g_dwLastError = 0;
const UINT IDT_TIMER = 100;

/////////////////////////////////////////////////////////////////////////////
// CMyLine
//
// Override of the CTapiLine for notifications
//
class CMyLine : public CTapiLine
{
	DECLARE_DYNCREATE (CMyLine)
protected:
    virtual void OnAddressStateChange (DWORD dwAddressID, DWORD dwState) {
		CTapiLine::OnAddressStateChange (dwAddressID, dwState);
		((CPhoneDlg*)theApp.m_pMainWnd)->SendMessage(UM_ADDRESSCHANGE, (WPARAM)GetAddress(dwAddressID));
	}

	virtual void OnAgentStateChange (DWORD dwAddressID, DWORD dwFields, DWORD dwState) {
		CTapiLine::OnAgentStateChange (dwAddressID, dwFields, dwState);
		((CPhoneDlg*)theApp.m_pMainWnd)->SendMessage(UM_AGENTCHANGE, (WPARAM)GetAddress(dwAddressID), dwFields);
	}

    virtual void OnDeviceStateChange (DWORD dwDeviceState, DWORD dwStateDetail1, DWORD dwStateDetail2) {
		CTapiLine::OnDeviceStateChange(dwDeviceState, dwStateDetail1, dwStateDetail2);
		((CPhoneDlg*)theApp.m_pMainWnd)->SendMessage(UM_LINECHANGE, (WPARAM)this);
	}

    virtual void OnNewCall (CTapiCall* pCall) {
		CTapiLine::OnNewCall(pCall);
		((CPhoneDlg*)theApp.m_pMainWnd)->SendMessage(UM_NEWCALL, (WPARAM)pCall);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CMyCall
//
// Override of the CTapiCall for notifications
//
class CMyCall : public CTapiCall
{
	DECLARE_DYNCREATE (CMyCall)
public:
    virtual void OnInfoChange (DWORD dwInfoState) {
		CTapiCall::OnInfoChange(dwInfoState);

		CWnd* pwnd = theApp.m_pMainWnd;
		if (pwnd != NULL && IsWindow(pwnd->GetSafeHwnd()))
		{
			if (dwInfoState & LINECALLINFOSTATE_USERUSERINFO)
				pwnd->PostMessage(UM_FLASHWINDOW, IDC_USERUSERINFO);
			if (dwInfoState & (LINECALLINFOSTATE_HIGHLEVELCOMP | LINECALLINFOSTATE_LOWLEVELCOMP | LINECALLINFOSTATE_CHARGINGINFO))
				pwnd->PostMessage(UM_FLASHWINDOW, IDC_ISDNINFO);
			if (dwInfoState & LINECALLINFOSTATE_QOS)
				pwnd->PostMessage(UM_FLASHWINDOW, IDC_QOS);
			if (dwInfoState & LINECALLINFOSTATE_CALLDATA)
				pwnd->PostMessage(UM_FLASHWINDOW, IDC_CALLDATA);

			((CPhoneDlg*)pwnd)->SendMessage(UM_CALLCHANGE, (WPARAM)this);
		}
	}

    virtual void OnStateChange (DWORD dwState, DWORD dwStateDetail, DWORD dwPrivilage) {
		CTapiCall::OnStateChange(dwState, dwStateDetail, dwPrivilage);
		((CPhoneDlg*)theApp.m_pMainWnd)->SendMessage(UM_CALLCHANGE, (WPARAM)this);
	}

	virtual void OnMediaModeChange (DWORD dwMediaMode) {
		CTapiCall::OnMediaModeChange(dwMediaMode);
		((CPhoneDlg*)theApp.m_pMainWnd)->SendMessage(UM_CALLCHANGE, (WPARAM)this);
	}
};

IMPLEMENT_DYNCREATE (CMyLine, CTapiLine)
IMPLEMENT_DYNCREATE (CMyCall, CTapiCall)
IMPLEMENT_DYNCREATE (CMyPhone, CTapiPhone)

/////////////////////////////////////////////////////////////////////////////
// CMainApp::InitInstance
//
// Initialize the main application
//
BOOL CMainApp::InitInstance()
{
#if _MSC_VER < 1300
	// Enable 3d controls
	Enable3dControlsStatic();
#endif

    // Initialize a connection with TAPI and determine if there 
    // are any TAPI complient devices installed.
    if (GetTAPIConnection()->Init("JPhoneDialer", 
			RUNTIME_CLASS(CMyLine), NULL, RUNTIME_CLASS(CMyCall),
			RUNTIME_CLASS(CMyPhone)) != 0 ||
        GetTAPIConnection()->GetLineDeviceCount() == 0)
    {
        AfxMessageBox ("There are no TAPI devices installed!");
		return FALSE;
    }

	// Create our dialog.
	CPhoneDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;

}// CMainApp::InitInstance

/////////////////////////////////////////////////////////////////////////////
// Message maps (required by MFC)
//
BEGIN_MESSAGE_MAP(CPhoneDlg, CDialog)
	//{{AFX_MSG_MAP(CPhoneDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_LINES, OnChangeLine)
	ON_BN_CLICKED(IDC_STARTSESSION, OnStartSession)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SESSIONSPIN, OnDeltaposSession)
	ON_CBN_SELCHANGE(IDC_ADDRESS, OnAddressChange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CALLSPIN, OnChangeCall)
	ON_MESSAGE(UM_CALLCHANGE, i_OnCallChange)
	ON_MESSAGE(UM_NEWCALL, i_OnNewCall)
	ON_MESSAGE(UM_ADDRESSCHANGE, i_OnAddressChange)
	ON_MESSAGE(UM_LINECHANGE, i_OnLineChange)
	ON_MESSAGE(UM_AGENTCHANGE, i_OnAgentChange)
	ON_MESSAGE(UM_FLASHWINDOW, OnFlashWindow)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DROP, OnDropCall)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ACCEPT, OnAcceptCall)
	ON_BN_CLICKED(IDC_ANSWER, OnAnswerCall)
	ON_BN_CLICKED(IDC_MAKECALL, OnMakeCall)
	ON_BN_CLICKED(IDC_HOLD, OnHoldCall)
	ON_BN_CLICKED(IDC_SWAPHOLD, OnSwapHoldCall)
	ON_BN_CLICKED(IDC_CALLDATA, OnCallData)
	ON_BN_CLICKED(IDC_USERUSERINFO, OnUserUserInfo)
	ON_BN_CLICKED(IDC_DIAL, OnDial)
	ON_BN_CLICKED(IDC_REDIRECT, OnRedirectCall)
	ON_BN_CLICKED(IDC_UNPARK, OnUnparkCall)
	ON_BN_CLICKED(IDC_PICKUP, OnPickupCall)
	ON_BN_CLICKED(IDC_CONNECTED, OnChangeLineStatus)
	ON_BN_CLICKED(IDC_AGENTINFO, OnAgentInfo)
	ON_BN_CLICKED(IDC_CONFLIST, OnConferenceList)
	ON_BN_CLICKED(IDC_TRANSFER, OnTransfer)
	ON_BN_CLICKED(IDC_PARK, OnParkCall)
	ON_BN_CLICKED(IDC_COMPLETE, OnCompleteTransfer)
	ON_BN_CLICKED(IDC_COMPLETECALL, OnCompleteCall)
	ON_BN_CLICKED(IDC_SETUPCONF, OnSetupConference)
	ON_BN_CLICKED(IDC_ADDTOCONF, OnAddToConference)
	ON_BN_CLICKED(IDC_REMOVEFROMCONF, OnRemoveFromConference)
	ON_BN_CLICKED(IDC_PHONEINFO, OnPhoneInfo)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_PHONENUMBER, OnAddressChange)
	ON_BN_CLICKED(IDC_INSERVICE, OnChangeLineStatus)
	ON_BN_CLICKED(IDC_MESSAGEWAITING, OnChangeLineStatus)
	ON_BN_CLICKED(IDC_LOCKED, OnChangeLineStatus)
	ON_BN_CLICKED(IDC_ISDNINFO, OnISDN)
	ON_BN_CLICKED(IDC_QOS, OnQos)
	ON_BN_CLICKED(IDC_FORWARD, OnForward)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LookupError
//
// Return a string representing a TAPI error
//
LPCSTR LookupError(LONG lResult)
{
	static char szBuff[1024];

	if (lResult == 0xffffffff)
		strcpy (szBuff, "Request timeout");
	else if (lResult > 0x80000000)
	{
		if (lResult >= 0x80000000 && lResult < 0x90000000)
		{
			lResult &= ~0x80000000;
			lResult |= 0x8000;
		}
		else
		{
			lResult &= ~0x90000000;
			lResult |= 0x9000;
		}
		if (LoadString(AfxGetResourceHandle(), (UINT) lResult, szBuff, 1024) == 0)
			strcpy (szBuff, "Unknown Error");
	}
	else
	{
		strcpy (szBuff, "Unknown Error");
	}

	return szBuff;

}// LookupError

/////////////////////////////////////////////////////////////////////////////
// ErrorMsg
//
// Show an error message on the status line.
//
void ErrorMsg(LPCSTR pszBuff, ...)
{
	va_list args;
	static char szBuff[1024];

	va_start(args, pszBuff);
	vsprintf(szBuff, pszBuff, args);
	va_end(args);

	CPhoneDlg* pDlg = (CPhoneDlg*) theApp.m_pMainWnd;
	if (pDlg->GetSafeHwnd() && ::IsWindow(pDlg->GetSafeHwnd()))
	{
		pDlg->SetDlgItemText(IDC_STATUS, szBuff);
		g_dwLastError = GetTickCount();
	}

}// ErrorMsg

/////////////////////////////////////////////////////////////////////////////
// ClearErrors
//
// Clear any errors on the console
//
void ClearErrors()
{
	CPhoneDlg* pDlg = (CPhoneDlg*) theApp.m_pMainWnd;
	if (pDlg->GetSafeHwnd() && ::IsWindow(pDlg->GetSafeHwnd()))
		pDlg->SetDlgItemText(IDC_STATUS, "");

}// ClearErrors

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::CPhoneDlg
//
// Constructor for the phone dialog

CPhoneDlg::CPhoneDlg(CWnd* pParent) : CDialog(CPhoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhoneDlg)
	m_strOrigin = _T("");
	m_strCallerID = _T("");
	m_strCallID = _T("");
	m_strRelatedCallID = _T("");
	m_strConnectedID = _T("");
	m_strCallNumber = _T("0 of 0");
	m_strCallState = _T("");
	m_strReason = _T("");
	m_strMaxCalls = _T("");
	m_strMediaMode = _T("");
	m_fMsgWaiting = FALSE;
	m_strNumber = _T("");
	m_strRedirectedNumber = _T("");
	m_strRedirectionNumber = _T("");
	m_strSessionNum = _T("0 Open");
	m_strLastStateChangeTime = _T("00:00:00");
	m_strTrunkID = _T("");
	m_fLocked = FALSE;
	m_iSession = 0;
	m_iCall = 0;
	m_strCalledID = _T("");
	m_fConnected = FALSE;
	m_fInService = FALSE;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}// CPhoneDlg::CPhoneDlg

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::DoDataExchange
//
// Dialog data exchange for the phone dialog
//
void CPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhoneDlg)
	DDX_Control(pDX, IDC_QOS, m_btnQOS);
	DDX_Control(pDX, IDC_ISDNINFO, m_btnISDN);
	DDX_Control(pDX, IDC_CONFIG, m_btnConfig);
	DDX_Control(pDX, IDC_PHONENUMBER, m_edtNumber);
	DDX_Control(pDX, IDC_CONFLIST, m_btnConfList);
	DDX_Control(pDX, IDC_PARK, m_btnPark);
	DDX_Control(pDX, IDC_COMPLETECALL, m_btnCompleteCall);
	DDX_Control(pDX, IDC_SWAPHOLD, m_btnSwapHold);
	DDX_Control(pDX, IDC_ACCEPT, m_btnAccept);
	DDX_Control(pDX, IDC_ADDRESS, m_cbAddress);
	DDX_Control(pDX, IDC_USERUSERINFO, m_btnUserUserInfo);
	DDX_Control(pDX, IDC_UNPARK, m_btnUnpark);
	DDX_Control(pDX, IDC_TRANSFER, m_btnTransfer);
	DDX_Control(pDX, IDC_STARTSESSION, m_btnStartSession);
	DDX_Control(pDX, IDC_SETUPCONF, m_btnSetupConf);
	DDX_Control(pDX, IDC_SESSIONSPIN, m_ctlSession);
	DDX_Control(pDX, IDC_REMOVEFROMCONF, m_btnRemoveFromConf);
	DDX_Control(pDX, IDC_REDIRECT, m_btnRedirect);
	DDX_Control(pDX, IDC_PICKUP, m_btnPickup);
	DDX_Control(pDX, IDC_PHONEINFO, m_btnPhoneInfo);
	DDX_Control(pDX, IDC_MAKECALL, m_btnMakeCall);
	DDX_Control(pDX, IDC_FORWARD, m_btnForward);
	DDX_Control(pDX, IDC_LINES, m_cbLines);
	DDX_Control(pDX, IDC_HOLD, m_btnHold);
	DDX_Control(pDX, IDC_DROP, m_btnDrop);
	DDX_Control(pDX, IDC_DIAL, m_btnDial);
	DDX_Control(pDX, IDC_COMPLETE, m_btnComplete);
	DDX_Control(pDX, IDC_CALLSPIN, m_ctlCalls);
	DDX_Control(pDX, IDC_CALLDATA, m_btnCallData);
	DDX_Control(pDX, IDC_ANSWER, m_btnAnswer);
	DDX_Control(pDX, IDC_AGENTINFO, m_btnAgentInfo);
	DDX_Control(pDX, IDC_ADDTOCONF, m_btnAddToConf);
	DDX_Text(pDX, IDC_CALLDIRECTION, m_strOrigin);
	DDX_Text(pDX, IDC_CALLERNUM, m_strCallerID);
	DDX_Text(pDX, IDC_CALLID, m_strCallID);
	DDX_Text(pDX, IDC_CONNECTEDNUM, m_strConnectedID);
	DDX_Text(pDX, IDC_CALLNUM, m_strCallNumber);
	DDX_Text(pDX, IDC_CALLSTATE, m_strCallState);
	DDX_Text(pDX, IDC_CALLTYPE, m_strReason);
	DDX_Text(pDX, IDC_MAXCALLS, m_strMaxCalls);
	DDX_Text(pDX, IDC_MEDIAMODE, m_strMediaMode);
	DDX_Check(pDX, IDC_MESSAGEWAITING, m_fMsgWaiting);
	DDX_Text(pDX, IDC_PHONENUMBER, m_strNumber);
	DDX_Text(pDX, IDC_REDIRECTNUM, m_strRedirectedNumber);
	DDX_Text(pDX, IDC_SESSION, m_strSessionNum);
	DDX_Text(pDX, IDC_STATECHGTIME, m_strLastStateChangeTime);
	DDX_Text(pDX, IDC_TRUNK, m_strTrunkID);
	DDX_Check(pDX, IDC_LOCKED, m_fLocked);
	DDX_Text(pDX, IDC_CALLEDNUM, m_strCalledID);
	DDX_Check(pDX, IDC_CONNECTED, m_fConnected);
	DDX_Check(pDX, IDC_INSERVICE, m_fInService);
	DDX_Text(pDX, IDC_RELCALLID, m_strRelatedCallID);
	DDX_Text(pDX, IDC_REDIRECTNUM2, m_strRedirectionNumber);
	//}}AFX_DATA_MAP

}// CPhoneDlg::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnInitDialog
//
// Process the initial dialog
//
BOOL CPhoneDlg::OnInitDialog()
{
    // Reset the font to all be ANSI var.
    CFont fntAnsi;
    fntAnsi.CreateStockObject (ANSI_VAR_FONT);

	LOGFONT lf;
	fntAnsi.GetObject(sizeof(LOGFONT), &lf);
	if (lf.lfHeight != 0)
		lf.lfHeight--;
	lf.lfWeight = FW_BOLD;
	m_fntBold.CreateFontIndirect(&lf);

    CWnd* pwndChild = GetWindow (GW_CHILD);
    while (pwndChild != NULL && IsChild(pwndChild))
    {
        pwndChild->SetFont(&fntAnsi);
        pwndChild = pwndChild->GetWindow(GW_HWNDNEXT);
    }

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctlSession.SetRange(0, 0);

	// Load our line devices with all the detected TAPI lines.
	for (DWORD dwLine = 0; dwLine < GetTAPIConnection()->GetLineDeviceCount(); dwLine++)
	{
		CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwLine);
		if (pLine != NULL)
		{
			int iPos = m_cbLines.AddString(pLine->GetLineName());
			ASSERT (iPos != CB_ERR);
			m_cbLines.SetItemDataPtr(iPos, pLine);
		}
	}

	// Select the first line
	if (m_cbLines.GetCount() > 0)
	{
		m_cbLines.SetCurSel(0);
		m_btnStartSession.EnableWindow(TRUE);
		OnChangeLine();
	}

	// Set our interval timer
	SetTimer(IDT_TIMER, 1000, NULL);

	return TRUE;

}// CPhoneDlg::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnSysCommand
//
// Process the WM_SYSCOMMAND message
//
void CPhoneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
		CDialog::OnSysCommand(nID, lParam);

}// CPhoneDlg::OnSysCommand

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnPaint
//
// Paint the minimized window
//
void CPhoneDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
		CDialog::OnPaint();

}// CPhoneDlg::OnPaint

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnQueryDragIcon
//
// Return the icon to drag
//
HCURSOR CPhoneDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;

}// CPhoneDlg::OnQueryDragIcon

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnChangeLine
//
// This is called when the user selects a new line in our combo box.
//
void CPhoneDlg::OnChangeLine() 
{
	// Remove any previous line errors
	ClearErrors();

	CTapiLine* pLine = GetActiveLine();
	if (pLine != NULL)
	{
		UpdateSession(pLine);

		if (!pLine->IsValid())
		{
			RemoveSession(pLine);
			AfxMessageBox("The line was removed by TAPI.");
			m_cbLines.DeleteString(m_cbLines.GetCurSel());
			m_cbLines.SetCurSel(0);
			OnChangeLine();
			return;
		}

		// Load the address information
		m_cbAddress.ResetContent();
		for (DWORD dwAddress = 0; dwAddress < pLine->GetAddressCount(); dwAddress++)
		{
			CTapiAddress* pAddr = pLine->GetAddress(dwAddress);
			CString strName = pAddr->GetDialableAddress();
			if (strName.IsEmpty())
				strName.Format("Address %ld", dwAddress);
			int iPos = m_cbAddress.AddString(strName);
			ASSERT (iPos != CB_ERR);
			m_cbAddress.SetItemDataPtr(iPos, pAddr);
		}

		// Select the first address by default.
		m_cbAddress.SetCurSel(0);
		OnAddressChange();

		// If the line is open, load all our info
		if (pLine->IsOpen())
		{
			m_arrCalls.RemoveAll();
			OnLoadLineInfo(pLine);
		}
		else
			NoLineSelected();
	}

}// CPhoneDlg::OnChangeLine

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnLoadLineInfo
//
// This loads all the line information for a newly picked line
//
void CPhoneDlg::OnLoadLineInfo(CTapiLine* pLine)
{
	m_btnStartSession.SetWindowText("End Session");
	m_btnConfig.EnableWindow(TRUE);
	m_edtNumber.EnableWindow(TRUE);
	m_btnPhoneInfo.EnableWindow(pLine->GetRelatedPhoneID() != 0xffffffff);

	// Set our checkflags
	LPLINEDEVSTATUS lpStatus = pLine->GetLineStatus();
	if (lpStatus == NULL)
		return;

	// Set our flags
	m_fMsgWaiting = ((lpStatus->dwDevStatusFlags & LINEDEVSTATUSFLAGS_MSGWAIT) != 0);
	m_fLocked = ((lpStatus->dwDevStatusFlags & LINEDEVSTATUSFLAGS_LOCKED) != 0);
	m_fConnected = ((lpStatus->dwDevStatusFlags & LINEDEVSTATUSFLAGS_CONNECTED) != 0);
	m_fInService = ((lpStatus->dwDevStatusFlags & LINEDEVSTATUSFLAGS_INSERVICE) != 0);

	// Enable/Disable the "settable" items.
	LPLINEDEVCAPS lpCaps = pLine->GetLineCaps();
	if (lpCaps)
	{
		GetDlgItem(IDC_CONNECTED)->EnableWindow ((lpCaps->dwSettableDevStatus & LINEDEVSTATUSFLAGS_CONNECTED) != 0);
		GetDlgItem(IDC_INSERVICE)->EnableWindow ((lpCaps->dwSettableDevStatus & LINEDEVSTATUSFLAGS_INSERVICE) != 0);
		GetDlgItem(IDC_LOCKED)->EnableWindow ((lpCaps->dwSettableDevStatus & LINEDEVSTATUSFLAGS_LOCKED) != 0);
		GetDlgItem(IDC_MESSAGEWAITING)->EnableWindow ((lpCaps->dwSettableDevStatus & LINEDEVSTATUSFLAGS_MSGWAIT) != 0);
	}

	// Grab all the call information on this line.
	CObList callList;
	pLine->GetNewCalls(callList);

	// Now add any other calls.
	if (m_arrCalls.GetSize() != pLine->GetCallCount())
	{
		m_arrCalls.RemoveAll();
		for (int iCall = 0; iCall < pLine->GetCallCount(); iCall++)
			OnNewCall(pLine->GetCall(iCall));
	}
	else
	{
		for (int iCall = 0; iCall < pLine->GetCallCount(); iCall++)
		{
			CTapiCall* pCall = pLine->GetCall(iCall);
			if (pCall == GetActiveCall())
			{
				MoveToCall(iCall);
				break;
			}
		}

		if (iCall == pLine->GetCallCount())
			MoveToCall(0);
	}
	UpdateData(FALSE);

}// CPhoneDlg::OnLoadLineInfo

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::MoveToCall
//
// Change to a given call
//
void CPhoneDlg::MoveToCall(int iCall)
{
	m_strOrigin = _T("");
	m_strCalledID = _T("");
	m_strCallerID = _T("");
	m_strCallID = _T("");
	m_strRelatedCallID = _T("");
	m_strConnectedID = _T("");
	m_strCallNumber = _T("0 of 0");
	m_strCallState = _T("");
	m_strReason = _T("");
	m_strMediaMode = _T("");
	m_strRedirectedNumber = _T("");
	m_strRedirectionNumber = _T("");
	m_strLastStateChangeTime = _T("00:00:00");
	m_strTrunkID = _T("");
	m_strCallNumber = _T("0 of 0");

	EnableCallButtons(0);
	m_btnUserUserInfo.EnableWindow (FALSE);
	m_btnCallData.EnableWindow (FALSE);
	m_btnQOS.EnableWindow(FALSE);
	m_btnISDN.EnableWindow(FALSE);

	if (iCall < 0 || iCall >= m_arrCalls.GetSize())
	{
		if (GetActiveCall() == NULL)
		{
			UpdateData(FALSE);
			return;
		}
		else
			MoveToCall(m_iCall);
	}

	m_iCall = iCall;
	m_strCallNumber.Format("%d of %d", m_iCall+1, m_arrCalls.GetSize());

	CTapiCall* pCall = (CTapiCall*) m_arrCalls[m_iCall];

	m_strCallState = pCall->GetCallStateString();
	
	LPLINECALLINFO lpCallInfo = pCall->GetCallInfo();
	LPLINECALLSTATUS lpCallStatus = pCall->GetCallStatus();

	if (lpCallInfo == NULL || lpCallStatus == NULL)
		return;

	m_strCallID.Format("0x%lX", lpCallInfo->dwCallID);
	m_strRelatedCallID.Format("0x%lX", lpCallInfo->dwRelatedCallID);
	m_strTrunkID.Format("0x%lX", lpCallInfo->dwTrunk);

	// Determine the call origin.
	m_strOrigin.Empty();
	if (lpCallInfo->dwOrigin & LINECALLORIGIN_CONFERENCE)
		m_strOrigin = "Conference";
	else if (lpCallInfo->dwOrigin & LINECALLORIGIN_OUTBOUND)
		m_strOrigin = "Outbound";
	else if (lpCallInfo->dwOrigin & 
				(LINECALLORIGIN_INTERNAL |
				 LINECALLORIGIN_EXTERNAL |		
				 LINECALLORIGIN_INBOUND))
		m_strOrigin = "Inbound";
	else if (lpCallInfo->dwOrigin == LINECALLORIGIN_UNAVAIL)
		m_strOrigin = "Unavail";

	if (m_strOrigin.IsEmpty() == FALSE)
	{
		if (lpCallInfo->dwOrigin & LINECALLORIGIN_INTERNAL)
			m_strOrigin += "/Int";
		else if (lpCallInfo->dwOrigin & LINECALLORIGIN_EXTERNAL)
			m_strOrigin += "/Ext";
	}
	else
		m_strOrigin = "Unknown";

	// Enable our conference list button
	m_btnConfList.EnableWindow((pCall->GetCallState() == LINECALLSTATE_CONFERENCED) ||
			(lpCallInfo->dwOrigin & LINECALLORIGIN_CONFERENCE) != 0);

	switch (lpCallInfo->dwReason)
	{
		case LINECALLREASON_DIRECT:			m_strReason = "Direct"; break;
		case LINECALLREASON_FWDBUSY:		m_strReason = "FwdBusy"; break;
		case LINECALLREASON_FWDNOANSWER:	m_strReason = "FwdNoAns"; break;
		case LINECALLREASON_FWDUNCOND:		m_strReason = "FwdUnc"; break;
		case LINECALLREASON_PICKUP:			m_strReason = "Pickup"; break;
		case LINECALLREASON_UNPARK:			m_strReason = "Unpark"; break;
		case LINECALLREASON_REDIRECT:		m_strReason = "Redirect"; break;
		case LINECALLREASON_CALLCOMPLETION:	m_strReason = "CallComp"; break;
		case LINECALLREASON_TRANSFER:		m_strReason = "Transfer"; break;
		case LINECALLREASON_REMINDER:		m_strReason = "Reminder"; break;
		case LINECALLREASON_UNAVAIL:		m_strReason = "Unavail"; break;
		case LINECALLREASON_INTRUDE:		m_strReason = "Intrude"; break;
		case LINECALLREASON_PARKED:			m_strReason = "Parked"; break;
		case LINECALLREASON_CAMPEDON:		m_strReason = "CampedOn"; break;
		case LINECALLREASON_ROUTEREQUEST:	m_strReason = "Route"; break;
		case LINECALLREASON_UNKNOWN:		
		default:							m_strReason = "Unknown"; break;
	}

	DWORD dwMediaMode = lpCallInfo->dwMediaMode & ~LINEMEDIAMODE_UNKNOWN;
	switch (dwMediaMode)
	{
		case LINEMEDIAMODE_INTERACTIVEVOICE: m_strMediaMode = "Voice"; break;
		case LINEMEDIAMODE_AUTOMATEDVOICE:	 m_strMediaMode = "Automated"; break;
		case LINEMEDIAMODE_DATAMODEM:		 m_strMediaMode = "Modem"; break;
		case LINEMEDIAMODE_G3FAX:			 m_strMediaMode = "G3FAX"; break;
		case LINEMEDIAMODE_TDD:				 m_strMediaMode = "TDD"; break;
		case LINEMEDIAMODE_G4FAX:			 m_strMediaMode = "G4FAX"; break;
		case LINEMEDIAMODE_DIGITALDATA:		 m_strMediaMode = "DigData"; break;
		case LINEMEDIAMODE_TELETEX:			 m_strMediaMode = "Teletex"; break;
		case LINEMEDIAMODE_VIDEOTEX:		 m_strMediaMode = "VidTex"; break;
		case LINEMEDIAMODE_TELEX:			 m_strMediaMode = "Telex"; break;
		case LINEMEDIAMODE_MIXED:			 m_strMediaMode = "Mixed"; break;
		case LINEMEDIAMODE_ADSI:			 m_strMediaMode = "ASDI"; break;
		case LINEMEDIAMODE_VOICEVIEW:		 m_strMediaMode = "VoiceView"; break;
		case LINEMEDIAMODE_UNKNOWN:			 
		default:							 m_strMediaMode = "Unknown"; break;
	}

	SYSTEMTIME localTime;
	CTimeSpan tmDiff;
	if (SystemTimeToTzSpecificLocalTime(NULL, &lpCallStatus->tStateEntryTime, &localTime))
		tmDiff = CTime::GetCurrentTime() - CTime(localTime);
	else
	{
		GetSystemTime(&localTime);
		tmDiff = CTime(localTime) - CTime(lpCallStatus->tStateEntryTime);
	}
	m_strLastStateChangeTime = tmDiff.Format("%H:%M:%S");

	m_strCalledID = _T("");
	m_strCallerID = _T("");
	m_strConnectedID = _T("");
	m_strRedirectedNumber = _T("");
	m_strRedirectionNumber = _T("");

	// Now fill in CALLER information.
	m_strCallerID = pCall->GetCallerIDName();
	if (!m_strCallerID.IsEmpty())
		m_strCallerID += CString(" ");
	m_strCallerID += pCall->GetCallerIDNumber();

	m_strCalledID = pCall->GetCalledIDName();
	if (!m_strCalledID.IsEmpty())
		m_strCalledID += CString(" ");
	m_strCalledID += pCall->GetCalledIDNumber();

	m_strConnectedID = pCall->GetConnectedIDName();
	if (!m_strConnectedID.IsEmpty())
		m_strConnectedID += CString(" ");
	m_strConnectedID += pCall->GetConnectedIDNumber();

	m_strRedirectedNumber = pCall->GetRedirectingIDName();
	if (!m_strRedirectedNumber.IsEmpty())
		m_strRedirectedNumber += CString(" ");
	m_strRedirectedNumber += pCall->GetRedirectingIDNumber();

	m_strRedirectionNumber = pCall->GetRedirectedFromIDName();
	if (!m_strRedirectionNumber.IsEmpty())
		m_strRedirectionNumber += CString(" ");
	m_strRedirectionNumber += pCall->GetRedirectedFromIDNumber();

	// Enable the appropriate call buttons.
	EnableCallButtons(lpCallStatus->dwCallFeatures);

	m_btnUserUserInfo.EnableWindow(lpCallInfo->dwUserUserInfoSize > 0 ||
			(lpCallStatus->dwCallFeatures & (LINECALLFEATURE_SENDUSERUSER | 
				LINECALLFEATURE_RELEASEUSERUSERINFO)));
	m_btnCallData.EnableWindow(lpCallInfo->dwCallDataSize > 0 || 
			(lpCallStatus->dwCallFeatures & LINECALLFEATURE_SETCALLDATA));
	m_btnQOS.EnableWindow(lpCallInfo->dwReceivingFlowspecSize > 0 ||
			lpCallInfo->dwSendingFlowspecSize > 0 ||
			(lpCallStatus->dwCallFeatures & LINECALLFEATURE_SETQOS));
	m_btnISDN.EnableWindow(lpCallInfo->dwHighLevelCompSize > 0 ||
			lpCallInfo->dwLowLevelCompSize > 0 ||
			lpCallInfo->dwChargingInfoSize > 0);

	UpdateData(FALSE);

}// CPhoneDlg::MoveToCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::NoLineSelected
//
// Turn off all the controls
//
void CPhoneDlg::NoLineSelected()
{
	m_btnStartSession.SetWindowText("Start Session");
	m_btnConfig.EnableWindow(FALSE);
	m_btnPhoneInfo.EnableWindow(FALSE);
	m_edtNumber.EnableWindow(FALSE);

	EnableLineButtons(0);

	NoCallSelected();

	m_strNumber = _T("");
	m_fLocked = FALSE;
	m_fMsgWaiting = FALSE;
	m_fConnected = FALSE;
	m_fInService = FALSE;

	// Turn off the call buttons.
	m_arrCalls.RemoveAll();
	m_iCall = 0;
	m_ctlCalls.SetRange(0,0);
	UpdateData(FALSE);

}// CPhoneDlg::NoLineSelected

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::NoCallSelected
//
// Turn off all the controls
//
void CPhoneDlg::NoCallSelected()
{
	m_strOrigin = _T("");
	m_strCalledID = _T("");
	m_strCallerID = _T("");
	m_strConnectedID = _T("");
	m_strCallID = _T("");
	m_strRelatedCallID = _T("");
	m_strCallNumber = _T("0 of 0");
	m_strCallState = _T("");
	m_strReason = _T("");
	m_strMediaMode = _T("");
	m_strRedirectedNumber = _T("");
	m_strRedirectionNumber = _T("");
	m_strLastStateChangeTime = _T("00:00:00");
	m_strTrunkID = _T("");
	UpdateData(FALSE);

	if (m_edtNumber.IsWindowEnabled())
		GotoDlgCtrl(&m_edtNumber);
	else
		GotoDlgCtrl(&m_cbLines);

	// Turn off all the buttons.
	EnableCallButtons(0);

	m_btnUserUserInfo.EnableWindow (FALSE);
	m_btnCallData.EnableWindow (FALSE);
	m_btnQOS.EnableWindow(FALSE);
	m_btnISDN.EnableWindow(FALSE);

}// CPhoneDlg::NoCallSelected

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::EnableCallButtons
//
// Enable/disable the various call buttons based on whether we support them.
//
void CPhoneDlg::EnableCallButtons(DWORD dwFeatures)
{
	m_btnTransfer.EnableWindow((dwFeatures & (LINECALLFEATURE_BLINDTRANSFER | LINECALLFEATURE_SETUPTRANSFER)) > 0);
	m_btnSetupConf.EnableWindow((dwFeatures & LINECALLFEATURE_SETUPCONF) > 0);
	m_btnRemoveFromConf.EnableWindow((dwFeatures & LINECALLFEATURE_REMOVEFROMCONF) > 0);
	m_btnRedirect.EnableWindow((dwFeatures & LINECALLFEATURE_REDIRECT) > 0);
	m_btnHold.EnableWindow((dwFeatures & (LINECALLFEATURE_HOLD | LINECALLFEATURE_UNHOLD)) > 0);
	m_btnDrop.EnableWindow((dwFeatures & LINECALLFEATURE_DROP) > 0);
	m_btnComplete.EnableWindow((dwFeatures & LINECALLFEATURE_COMPLETETRANSF) > 0);
	m_btnAnswer.EnableWindow((dwFeatures & LINECALLFEATURE_ANSWER) > 0);
	m_btnAddToConf.EnableWindow((dwFeatures & (LINECALLFEATURE_ADDTOCONF | LINECALLFEATURE_PREPAREADDCONF)) > 0);
	m_btnAccept.EnableWindow((dwFeatures & LINECALLFEATURE_ACCEPT) > 0);
	m_btnSwapHold.EnableWindow((dwFeatures & LINECALLFEATURE_SWAPHOLD) > 0);
	m_btnCompleteCall.EnableWindow((dwFeatures & LINECALLFEATURE_COMPLETECALL) > 0);
	m_btnPark.EnableWindow((dwFeatures & LINECALLFEATURE_PARK) > 0);
	m_btnDial.EnableWindow((dwFeatures & (LINECALLFEATURE_DIAL | LINECALLFEATURE_GENERATEDIGITS)) > 0);

	if (dwFeatures & LINECALLFEATURE_UNHOLD)
		m_btnHold.SetWindowText("Unho&ld");
	else
		m_btnHold.SetWindowText("Ho&ld");

}// CPhoneDlg::EnableCallButtons

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::EnableLineButtons
//
// Enable/disable the various line buttons based on whether we support them.
//
void CPhoneDlg::EnableLineButtons(DWORD dwCaps)
{
	UpdateData(TRUE);
	if (m_strNumber.IsEmpty())
	{
		m_btnMakeCall.EnableWindow(FALSE);
		m_btnPickup.EnableWindow(FALSE);
		m_btnUnpark.EnableWindow(FALSE);
	}
	else
	{
		m_btnMakeCall.EnableWindow((dwCaps & LINEADDRFEATURE_MAKECALL) > 0);
		m_btnPickup.EnableWindow((dwCaps & LINEADDRFEATURE_PICKUP) > 0);
		m_btnUnpark.EnableWindow((dwCaps & LINEADDRFEATURE_UNPARK) > 0);
	}

	CTapiLine* pLine = GetActiveLine();
	if (pLine != NULL)
	{
		LPLINEDEVSTATUS lpStatus = pLine->GetLineStatus();
		if (lpStatus != NULL)
			m_btnForward.EnableWindow((lpStatus->dwLineFeatures & (LINEFEATURE_FORWARD | LINEFEATURE_FORWARDDND | LINEFEATURE_FORWARDFWD)));
		else
			m_btnForward.EnableWindow(FALSE);
	}
	else m_btnForward.EnableWindow(FALSE);

}// CPhoneDlg::EnableLineButtons

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnStartSession
//
// Open a new session
//
void CPhoneDlg::OnStartSession() 
{
	// Priority of media modes
	static struct
	{
		DWORD dwMediaMode;
		LPCSTR pszName;
	} g_MediaModes[] = {
		{ LINEMEDIAMODE_INTERACTIVEVOICE,	"Voice" },
		{ LINEMEDIAMODE_DATAMODEM,			"DataModem" },
		{ LINEMEDIAMODE_AUTOMATEDVOICE,		"AutomatedVoice" }, 
		{ LINEMEDIAMODE_DIGITALDATA,		"DigitalData" },
		{ LINEMEDIAMODE_G3FAX,				"G3 FAX" },
		{ LINEMEDIAMODE_G4FAX,				"G4 FAX" },
		{ LINEMEDIAMODE_TDD,				"TDD" },
		{ LINEMEDIAMODE_TELETEX,			"TeleTex" },
		{ LINEMEDIAMODE_VIDEOTEX,			"VideoTex" },
		{ LINEMEDIAMODE_TELEX,				"Telex" },
		{ LINEMEDIAMODE_MIXED,				"Mixed" },
		{ LINEMEDIAMODE_ADSI,				"ADSI" },
		{ LINEMEDIAMODE_VOICEVIEW,			"VoiceView" },
		{ 0, NULL }
	};

	ClearErrors();
	NoLineSelected();

	CTapiLine* pLine = GetActiveLine();
	if (pLine == NULL)
		return;

	if (pLine->IsOpen())
	{
		RemoveSession(pLine);
		pLine->Close();
	}
	else
	{
		// Open for ALL media modes first.
		DWORD dwMediaMode = 0;
		const LPLINEDEVCAPS lpCaps = pLine->GetLineCaps();
		if (lpCaps)
			dwMediaMode = (lpCaps->dwMediaModes & ~LINEMEDIAMODE_UNKNOWN);

		// Open the line
		LONG lResult = pLine->Open (LINECALLPRIVILEGE_OWNER | LINECALLPRIVILEGE_MONITOR, dwMediaMode);

		// UNIMODEM only allows ONE media mode to be chosen.. pick the best one available.
		if (lResult == LINEERR_INVALMEDIAMODE)
		{
			// Pick only ONE media mode
			for (int i = 0; g_MediaModes[i].dwMediaMode != 0; i++)
			{
				if (dwMediaMode & g_MediaModes[i].dwMediaMode)
				{
					lResult = pLine->Open (LINECALLPRIVILEGE_OWNER | LINECALLPRIVILEGE_MONITOR, g_MediaModes[i].dwMediaMode);
					if (lResult == 0)
					{
						ErrorMsg("Forced to open line with media mode %s", g_MediaModes[i].pszName);
						break;
					}
				}
			}
		}

		// Show an error
		if (lResult != 0)
			ShowError("lineOpen", lResult);
		else
		{
			// Get the states we get notified on
			DWORD dwAddrSt = (LINEADDRESSSTATE_CAPSCHANGE + LINEADDRESSSTATE_CAPSCHANGE-1);
			CTapiAddress* pAddr = GetActiveAddress();
			if (pAddr)
			{
				LPLINEADDRESSCAPS lpACaps = pAddr->GetAddressCaps();
				if (lpACaps)
					dwAddrSt &= lpACaps->dwAddressStates;
			}
			DWORD dwStates = (LINEDEVSTATE_REMOVED + LINEDEVSTATE_REMOVED-1);
			if (lpCaps) 
				dwStates &= lpCaps->dwLineStates;
			lResult = pLine->SetStatusMessages(dwStates, dwAddrSt);
			if (lResult != 0)
				ShowError("lineSetStatusMessages", lResult);

			AddSession(pLine);
			OnLoadLineInfo(pLine);			
		}
	}
	OnAddressChange();

}// OnStartSession

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnNewCall
//
// Called to add a call to our list of "monitored" calls.
//
void CPhoneDlg::OnNewCall(CTapiCall* pCall)
{
	CTapiLine* pLine = GetActiveLine();
	if (pLine == NULL || pCall == NULL || pCall->GetLineOwner() != pLine)
		return;

	// Make sure it isn't there.
	for (int i = 0; i < m_arrCalls.GetSize(); i++)
	{
		if (pCall == (CTapiCall*) m_arrCalls[i])
			return;
	}

	// If the call is IDLE, deallocate it.
	if (pCall->GetCallState() == LINECALLSTATE_IDLE)
	{
		pCall->Deallocate();
		return;
	}

	// Add it to our list.
	int iPos = m_arrCalls.Add(pCall);

	// Change our counter.
	m_ctlCalls.SetRange(0, static_cast<short>(m_arrCalls.GetSize()));
	m_ctlCalls.Invalidate(TRUE);

	// Show this new call.
	MoveToCall(iPos);

}// CPhoneDlg::OnNewCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::RemoveCall
//
// This removes the specified call from our list when it goes IDLE.
//
void CPhoneDlg::RemoveCall(CTapiCall* pCall)
{
	// Make sure it is there
	for (int i = 0; i < m_arrCalls.GetSize(); i++)
	{
		if (pCall == (CTapiCall*) m_arrCalls[i])
		{
			m_arrCalls.RemoveAt(i);
			if (m_iCall == i)
			{
				if (--m_iCall < 0)
					m_iCall = 0;
			}
		}
	}

	// Change our counter.
	m_ctlCalls.SetRange(0, static_cast<short>(m_arrCalls.GetSize()));
	m_ctlCalls.Invalidate(TRUE);

	if (m_arrCalls.GetSize() == 0)
		NoCallSelected();
	else
		MoveToCall(0);

	// Remove the call
	pCall->Deallocate();

}// CPhoneDlg::RemoveCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnDeltaposSession
//
// This changes the session using the updown control.
//
void CPhoneDlg::OnDeltaposSession(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CTapiLine* pLine = (CTapiLine*) m_arrSessions[m_iSession];
	if (pNMUpDown->iDelta > 0)
	{
		if (m_iSession < m_arrSessions.GetSize()-1)
			pLine = (CTapiLine*) m_arrSessions[++m_iSession];
	}
	else
	{
		if (m_iSession > 0)
			pLine = (CTapiLine*) m_arrSessions[--m_iSession];
	}

	for (int i = 0; i < m_cbLines.GetCount(); i++)
	{
		if (pLine == m_cbLines.GetItemDataPtr(i))
		{
			ClearErrors();
			m_cbLines.SetCurSel(i);
			OnChangeLine();
			UpdateSession(pLine);
		}
	}
	*pResult = 0;

}// CPhoneDlg::OnDeltaposSession

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::UpdateSession
//
// This updates our current session number when the user changes it
// using the updown control
//
void CPhoneDlg::UpdateSession(CTapiLine* pLine)
{
	m_strSessionNum.Format("%d Open", m_arrSessions.GetSize());
	m_iSession = 0;

	if (pLine != NULL)
	{
		for (int i = 0; i < m_arrSessions.GetSize(); i++)
		{
			if (pLine == (CTapiLine*) m_arrSessions[i])
			{
				m_iSession = i;
				m_strSessionNum.Format("%d of %d", i+1, m_arrSessions.GetSize());
				break;
			}
		}
	}

	m_ctlSession.SetPos(m_iSession);
	UpdateData(FALSE);

}// CPhoneDlg::UpdateSession

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::AddSession
//
// This adds a new session (open line) to our list.
//
void CPhoneDlg::AddSession(CTapiLine* pLine)
{
	m_iSession = m_arrSessions.Add(pLine);
	m_ctlSession.SetRange(0, static_cast<short>(m_arrSessions.GetSize()));
	m_ctlSession.Invalidate(TRUE);
	UpdateSession(pLine);

}// CPhoneDlg::AddSession

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::RemoveSession
//
// This removes a newly closed line from our session list
//
void CPhoneDlg::RemoveSession(CTapiLine* pLine)
{
	for (int i = 0; i < m_arrSessions.GetSize(); i++)
	{
		if (pLine == (CTapiLine*) m_arrSessions[i])
		{
			m_arrSessions.RemoveAt(i);
			break;
		}
	}
	m_ctlSession.SetRange(0, static_cast<short>(m_arrSessions.GetSize()));
	m_ctlSession.Invalidate(TRUE);
	UpdateSession(NULL);

}// CPhoneDlg::RemoveSession

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnTimer
//
// Periodic interval timer.
//
void CPhoneDlg::OnTimer(UINT /*nIDEvent*/) 
{
	CTapiCall* pCall = GetActiveCall();
	if (pCall != NULL)
	{
		LPLINECALLSTATUS lpStatus = pCall->GetCallStatus();
		if (lpStatus != NULL)
		{
			SYSTEMTIME localTime;
			CTimeSpan tmDiff;
			if (SystemTimeToTzSpecificLocalTime(NULL, &lpStatus->tStateEntryTime, &localTime))
				tmDiff = CTime::GetCurrentTime() - CTime(localTime);
			else
			{
				GetSystemTime(&localTime);
				tmDiff = CTime(localTime) - CTime(lpStatus->tStateEntryTime);
			}
			m_strLastStateChangeTime = tmDiff.Format("%H:%M:%S");
		}
		else
			m_strLastStateChangeTime = _T("00:00:00");
		UpdateData(FALSE);
	}

	// Clear the error line periodically
	if ((g_dwLastError + 10000) < GetTickCount())
		ClearErrors();

}// CPhoneDlg::OnTimer

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnAddressChange
//
// This is called when the user selects a new address in our address
// combo box.
//
void CPhoneDlg::OnAddressChange() 
{
	// If no address is supported, kill all the line-call buttons
	CTapiAddress* pAddr = GetActiveAddress();
	if (pAddr == NULL)
	{
		EnableLineButtons(0);
		m_btnAgentInfo.EnableWindow(FALSE);
		return;
	}

	// Otherwise use the current address features to enable the buttons.
	LPLINEADDRESSSTATUS lpStatus = pAddr->GetAddressStatus(TRUE);
	LPLINEADDRESSCAPS lpCaps = pAddr->GetAddressCaps();
	EnableLineButtons( (lpStatus != NULL) ? lpStatus->dwAddressFeatures : 0);

	// Enable/Disable the agent button
	m_btnAgentInfo.EnableWindow((lpStatus != NULL) ? pAddr->SupportsAgents() : FALSE);

	// Set the "max" calls.  This is a combination of all Active/OnHold calls.
	int iMaxCalls = (int) (lpCaps->dwMaxNumActiveCalls + 
			lpCaps->dwMaxNumOnHoldCalls +
			lpCaps->dwMaxNumOnHoldPendingCalls);
	m_strMaxCalls.Format("%d", iMaxCalls);
	UpdateData(FALSE);

}// CPhoneDlg::OnAddressChange

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnChangeCall
//
// This is called when the user uses the updown control to walk through
// our call list.
//
void CPhoneDlg::OnChangeCall(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearErrors();

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (pNMUpDown->iDelta > 0)
	{
		if (m_iCall < m_arrCalls.GetSize()-1)
			MoveToCall(++m_iCall);
	}
	else
	{
		if (m_iCall > 0)
			MoveToCall(--m_iCall);
	}
	*pResult = 0;

}// CPhoneDlg::OnChangeCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::i_OnAgentChange
//
// An agent associated with some address we have open has changed.
//
LRESULT CPhoneDlg::i_OnAgentChange(WPARAM wParam, LPARAM lParam)
{
	CTapiAddress* pAddr = (CTapiAddress*)wParam;
	CAgentStateDlg* pDlg = (CAgentStateDlg*) m_mapAgentDlg[pAddr];
	if (pDlg != NULL)
		pDlg->PostMessage(UM_AGENTCHANGE, wParam, lParam);
	OnAddressChange();
	return 0L;

}// CPhoneDlg::i_OnAgentChange

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::i_OnCallChange
//
// Called when TAPI notifies us that a call has changed
//
LRESULT CPhoneDlg::i_OnCallChange(WPARAM wParam, LPARAM /*lParam*/)
{
	CTapiCall* pCall = (CTapiCall*) wParam;
	if (pCall->GetCallState() == LINECALLSTATE_IDLE)
		RemoveCall(pCall);
	else if (pCall == GetActiveCall())
		MoveToCall(m_iCall);
	return 0;

}// CPhoneDlg::i_OnCallChange

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::i_OnAddressChange
//
// Called when TAPI notifies us that the address changed
//
LRESULT CPhoneDlg::i_OnAddressChange(WPARAM wParam, LPARAM /*lParam*/)
{
	CTapiAddress* pAddr = (CTapiAddress*) wParam;
	if (GetActiveAddress() == pAddr)
		OnAddressChange();
	return 0;

}// CPhoneDlg::i_OnAddressChange

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::i_OnLineChange
//
// Called when TAPI notifies us that the line changed
//
LRESULT CPhoneDlg::i_OnLineChange(WPARAM wParam, LPARAM /*lParam*/)
{
	CTapiLine* pLine = (CTapiLine*) wParam;
	if (GetActiveLine() == pLine)
	{
		LPLINEDEVSTATUS lpStatus = pLine->GetLineStatus();
		if (lpStatus)
		{
			if ((lpStatus->dwDevStatusFlags & (LINEDEVSTATUSFLAGS_INSERVICE | LINEDEVSTATUSFLAGS_CONNECTED)) == 0)
				ErrorMsg("The line is out-of-service and disconnected.");
			else if ((lpStatus->dwDevStatusFlags & LINEDEVSTATUSFLAGS_CONNECTED) == 0)
				ErrorMsg("The line is disconnected.");
			else if ((lpStatus->dwDevStatusFlags & LINEDEVSTATUSFLAGS_INSERVICE) == 0)
				ErrorMsg("The line is out-of-service.");
		}
		OnLoadLineInfo(pLine);
	}
	return 0;

}// CPhoneDlg::i_OnLineChange

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::i_OnNewCall
//
// Called when TAPI notifies us that a new call appearance was created
//
LRESULT CPhoneDlg::i_OnNewCall(WPARAM wParam, LPARAM /*lParam*/)
{
	CTapiCall* pCall = (CTapiCall*) wParam;
	OnNewCall(pCall);
	return 0;

}// CPhoneDlg::i_OnNewCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::GetActiveCall
//
// Returns the currently active call (if any)
//
CTapiCall* CPhoneDlg::GetActiveCall()
{
	if (m_iCall >= 0 && m_iCall < m_arrCalls.GetSize())
		return (CTapiCall*) m_arrCalls[m_iCall];
	m_iCall = -1;
	return NULL;

}// CPhoneDlg::GetActiveCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::GetActiveLine
//
// Returns the currently active line
//
CTapiLine* CPhoneDlg::GetActiveLine()
{
	// Get the new line which is active.
	int iCurSel = m_cbLines.GetCurSel();
	if (iCurSel == CB_ERR)
		return NULL;
	return (CTapiLine*) m_cbLines.GetItemDataPtr(iCurSel);

}// CPhoneDlg::GetActiveLine

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::GetActiveAddress
//
// Returns the currently active address
//
CTapiAddress* CPhoneDlg::GetActiveAddress()
{
	int iCurSel = m_cbAddress.GetCurSel();
	if (iCurSel == CB_ERR)
		return NULL;
	return (CTapiAddress*) m_cbAddress.GetItemDataPtr(iCurSel);

}// CPhoneDlg::GetActiveLine

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnDropCall
//
// Drops the currently displayed call.
//
void CPhoneDlg::OnDropCall() 
{
	ClearErrors();

	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL)
		return;

	// Make sure we are owner.
	LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		lResult = GetTAPIConnection()->WaitForReply(pCall->Drop());
		if (lResult != 0)
			ShowError("lineDrop", lResult);
	}

}// CPhoneDlg::OnDropCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnAcceptCall
//
// Accepts the currently displayed call.
//
void CPhoneDlg::OnAcceptCall() 
{
	ClearErrors();

	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL)
		return;

	// Make sure we are owner.
	LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		lResult = GetTAPIConnection()->WaitForReply(pCall->Accept());
		if (lResult != 0)
			ShowError("lineAccept", lResult);
	}

}// CPhoneDlg::OnAcceptCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnAnswerCall
//
// Answers the currently displayed call.
//
void CPhoneDlg::OnAnswerCall() 
{
	ClearErrors();

	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL)
		return;

	// Make sure we are owner.
	LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		lResult = GetTAPIConnection()->WaitForReply(pCall->Answer());
		if (lResult != 0)
			ShowError("lineAnswer", lResult);
	}

}// CPhoneDlg::OnAnswerCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnMakeCall
//
// Places a new call
//
void CPhoneDlg::OnMakeCall() 
{
	CTapiLine* pLine = GetActiveLine();
	if (pLine == NULL)
		return;

	UpdateData(TRUE);
	ClearErrors();

	// If this is a predictive dialer then prompt the user for
	// predictive dialer information.
	LPLINECALLPARAMS lpCallParams = NULL;
	CTapiAddress* pAddr = pLine->GetAddress((DWORD)0);
	if (pAddr != NULL)
	{
		LPLINEADDRESSCAPS lpCaps = pAddr->GetAddressCaps();
		if (lpCaps && (lpCaps->dwAddrCapFlags & LINEADDRCAPFLAGS_PREDICTIVEDIALER) != 0)
		{
			CPredDialDlg dlg(this);
			dlg.m_dwCallStates = lpCaps->dwPredictiveAutoTransferStates;
			dlg.m_nTimeout = lpCaps->dwMaxNoAnswerTimeout;
			if (dlg.DoModal() == IDOK)
			{
				lpCallParams = (LPLINECALLPARAMS) new BYTE[sizeof(LINECALLPARAMS) + dlg.m_strTarget.GetLength()+1];
				memset(lpCallParams, 0, sizeof(LINECALLPARAMS));
				lpCallParams->dwTotalSize = sizeof(LINECALLPARAMS) + dlg.m_strTarget.GetLength()+1;
				lpCallParams->dwBearerMode = LINEBEARERMODE_VOICE;
				lpCallParams->dwMediaMode = LINEMEDIAMODE_INTERACTIVEVOICE;
				lpCallParams->dwCallParamFlags = 0;
				lpCallParams->dwAddressID = 0;
				lpCallParams->dwAddressMode = LINEADDRESSMODE_ADDRESSID;
				lpCallParams->dwPredictiveAutoTransferStates = dlg.m_dwCallStates;
				lpCallParams->dwTargetAddressOffset = sizeof(LINECALLPARAMS);
				lpCallParams->dwTargetAddressSize = dlg.m_strTarget.GetLength()+1;
				lstrcpy((LPTSTR)lpCallParams+lpCallParams->dwTargetAddressOffset, dlg.m_strTarget);
				lpCallParams->dwNoAnswerTimeout = dlg.m_nTimeout;
			}
		}
	}

	CTapiCall* pCall = NULL;
	LONG lResult = pLine->MakeCall(&pCall, m_strNumber, 0, lpCallParams);
	if (lResult != 0)
		ShowError("lineMakeCall", lResult);

	delete [] lpCallParams;

}// CPhoneDlg::OnMakeCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnUnparkCall
//
// Unpark a call from an extension
//
void CPhoneDlg::OnUnparkCall() 
{
	CTapiAddress* pAddress = GetActiveAddress();
	if (pAddress == NULL)
		return;

	UpdateData(TRUE);
	ClearErrors();

	CTapiCall* pCall = NULL;
	LONG lResult = GetTAPIConnection()->WaitForReply(
		pAddress->Unpark(&pCall, m_strNumber));
	if (lResult != 0)
		ShowError("lineUnpark", lResult);

}// CPhoneDlg::OnUnparkCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnPickupCall
//
// Pickup a call from an extension
//
void CPhoneDlg::OnPickupCall() 
{
	CTapiAddress* pAddress = GetActiveAddress();
	if (pAddress == NULL)
		return;

	UpdateData(TRUE);
	ClearErrors();

	CTapiCall* pCall = NULL;
	LONG lResult = GetTAPIConnection()->WaitForReply(
			pAddress->Pickup(&pCall, m_strNumber, NULL));
	if (lResult != 0)
		ShowError("linePickup", lResult);

}// CPhoneDlg::OnPickupCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnHoldCall
//
// Hold/Unhold the current call
//
void CPhoneDlg::OnHoldCall() 
{
	ClearErrors();

	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL)
		return;

	// Make sure we are owner.
	LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		if (pCall->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_HOLD)
		{
			lResult = GetTAPIConnection()->WaitForReply(pCall->Hold());
			if (lResult != 0)
				ShowError("lineHold", lResult);
		}
		else
		{
			lResult = GetTAPIConnection()->WaitForReply(pCall->Unhold());
			if (lResult != 0)
				ShowError("lineUnhold", lResult);
		}
	}

}// CPhoneDlg::OnHoldCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnSwapHoldCall
//
// SwapHold the current call
//
void CPhoneDlg::OnSwapHoldCall() 
{
	ClearErrors();

	// Get the active call
	CTapiCall* pCall = GetActiveCall();
	CTapiLine* pLine = GetActiveLine();
	if (pCall == NULL || pLine == NULL)
		return;

	// Find the other call.  If this call is NOT onHOLD, then
	// look for any onHold call with the SWAPHOLD bit set.
	CTapiCall* pCall_Cons = NULL;
	if ((pCall->GetCallState() & (LINECALLSTATE_ONHOLD | 
			LINECALLSTATE_ONHOLDPENDTRANSFER |
			LINECALLSTATE_ONHOLDPENDCONF)))
	{
		 pCall_Cons = pLine->FindCall((LINECALLSTATE_CONNECTED | 
				LINECALLSTATE_PROCEEDING |
				LINECALLSTATE_DIALING |
				LINECALLSTATE_DIALTONE), LINECALLFEATURE_SWAPHOLD);
	}
	else
	{
		pCall_Cons = pCall;
		pCall = pLine->FindCall((LINECALLSTATE_ONHOLD | 
				LINECALLSTATE_ONHOLDPENDTRANSFER |
				LINECALLSTATE_ONHOLDPENDCONF), LINECALLFEATURE_SWAPHOLD);
	}

	if (pCall == NULL || pCall_Cons == NULL)
	{
		ErrorMsg("Unable to locate two calls with lineSwapHold ability.");
		return;
	}

	// Make sure we are owner of both calls.
	LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		lResult = pCall_Cons->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ShowError("lineSetCallPrivilege", lResult);
		else
		{
			lResult = GetTAPIConnection()->WaitForReply(pLine->SwapHold(pCall_Cons, pCall));
			if (lResult != 0)
				ShowError("lineSwapHold", lResult);
		}
	}

}// CPhoneDlg::OnSwapHoldCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnCallData
//
// Set/Get the call data
//
void CPhoneDlg::OnCallData() 
{
	ClearErrors();

	CTapiCall* pCall = GetActiveCall();
	if (pCall != NULL)
	{
		CCallDataDlg dlg(this, pCall);
		dlg.DoModal();
	}

	LPLINECALLINFO lpCallInfo = pCall->GetCallInfo();
	LPLINECALLSTATUS lpCallStatus = pCall->GetCallStatus();
	m_btnCallData.EnableWindow(lpCallInfo->dwCallDataSize > 0 || 
			(lpCallStatus->dwCallFeatures & LINECALLFEATURE_SETCALLDATA));

}// CPhoneDlg::OnCallData

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnUserUserInfo
//
// Set/Get the UUI.
//
void CPhoneDlg::OnUserUserInfo() 
{
	ClearErrors();

	CTapiCall* pCall = GetActiveCall();
	if (pCall != NULL)
	{
		CUUIDlg dlg(this, pCall);
		dlg.DoModal();
	}

	LPLINECALLINFO lpCallInfo = pCall->GetCallInfo();
	LPLINECALLSTATUS lpCallStatus = pCall->GetCallStatus();
	m_btnUserUserInfo.EnableWindow(lpCallInfo->dwUserUserInfoSize > 0 ||
			(lpCallStatus->dwCallFeatures & (LINECALLFEATURE_SENDUSERUSER | 
				LINECALLFEATURE_RELEASEUSERUSERINFO)));

}// CPhoneDlg::OnUserUserInfo

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnISDN
//
// Display ISDN information (Charging, Low/Hi level compat).
//
void CPhoneDlg::OnISDN() 
{
	ClearErrors();

	CTapiCall* pCall = GetActiveCall();
	if (pCall != NULL)
	{
		CISDNInfo dlg(this, pCall);
		dlg.DoModal();
	}

	LPLINECALLINFO lpCallInfo = pCall->GetCallInfo();
	m_btnISDN.EnableWindow(lpCallInfo->dwHighLevelCompSize > 0 ||
			lpCallInfo->dwLowLevelCompSize > 0 ||
			lpCallInfo->dwChargingInfoSize > 0);

}// CPhoneDlg::OnISDN

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnQos
//
// Display/Set Quality of Service information
//
void CPhoneDlg::OnQos() 
{
	ClearErrors();

	CTapiCall* pCall = GetActiveCall();
	if (pCall != NULL)
	{
		CQosInfo dlg(this, pCall);
		dlg.DoModal();
	}

	LPLINECALLINFO lpCallInfo = pCall->GetCallInfo();
	LPLINECALLSTATUS lpCallStatus = pCall->GetCallStatus();
	m_btnQOS.EnableWindow(lpCallInfo->dwReceivingFlowspecSize > 0 ||
			lpCallInfo->dwSendingFlowspecSize > 0 ||
			(lpCallStatus->dwCallFeatures & LINECALLFEATURE_SETQOS));

}// CPhoneDlg::OnQos

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnDial
//
// Dial a number
//
void CPhoneDlg::OnDial() 
{
	ClearErrors();

	CTapiLine* pLine = GetActiveLine();
	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL || pLine == NULL)
		return;

	CDialDlg dlg(this, pLine, LINECALLFEATURE_DIAL);
	if (dlg.DoModal() == IDOK)
	{
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ShowError("lineSetCallPrivilege", lResult);
		else
		{
			if ((pCall->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_DIAL) != 0)
				lResult = GetTAPIConnection()->WaitForReply(pCall->Dial(dlg.m_strNumber));
			else
				lResult = GetTAPIConnection()->WaitForReply(pCall->GenerateDigits(LINEDIGITMODE_DTMF, dlg.m_strNumber));

			if (lResult != 0)
			{	
				const LINECALLSTATUS* lpStatus = pCall->GetCallStatus();
 				if (lpStatus && (lpStatus->dwCallFeatures & LINECALLFEATURE_DIAL) != 0)
					ShowError("lineDial", lResult);
				else
					ShowError("lineGenerateDigits", lResult);
			}
		}
	}

}// CPhoneDlg::OnDial

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnRedirectCall
//
// Redirect the call to another number
//
void CPhoneDlg::OnRedirectCall() 
{
	ClearErrors();

	CTapiLine* pLine = GetActiveLine();
	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL || pLine == NULL)
		return;

	CDialDlg dlg(this, pLine, LINECALLFEATURE_REDIRECT);
	if (dlg.DoModal() == IDOK)
	{
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ShowError("lineSetCallPrivilege", lResult);
		else
		{
			lResult = GetTAPIConnection()->WaitForReply(pCall->Redirect(dlg.m_strNumber));
			if (lResult != 0)
				ShowError("lineRedirect", lResult);
		}
	}

}// CPhoneDlg::OnRedirectCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnChangeLineStatus
//
// Update the LINEDEVSTATE flags in the provider from our config.
//
void CPhoneDlg::OnChangeLineStatus() 
{
	CTapiLine* pLine = GetActiveLine();
	if (pLine == NULL)
		return;

	// If the line supports lineSetDevStatus..
	LPLINEDEVCAPS lpCaps = pLine->GetLineCaps();
	if (lpCaps == NULL)
		return;
	if ((lpCaps->dwLineFeatures & LINEFEATURE_SETDEVSTATUS) == 0)
		return;

	// Store the CURRENT flags.
	BOOL fConnected = m_fConnected, fMsgWaiting = m_fMsgWaiting;
	BOOL fInService = m_fInService, fLocked = m_fLocked;

	UpdateData(TRUE);
	BOOL fSet = FALSE;
	DWORD dwDevState = 0;

	if (m_fConnected != fConnected)
	{
		dwDevState |= LINEDEVSTATUSFLAGS_CONNECTED;
		if (m_fConnected)
			fSet = TRUE;
	}

	else if (m_fInService != fInService)
	{
		dwDevState |= LINEDEVSTATUSFLAGS_INSERVICE;
		if (m_fInService)
			fSet = TRUE;
	}

	else if (m_fLocked != fLocked)
	{
		dwDevState |= LINEDEVSTATUSFLAGS_LOCKED;
		if (m_fLocked)
			fSet = TRUE;
	}

	else if (m_fMsgWaiting != fMsgWaiting)
	{
		dwDevState |= LINEDEVSTATUSFLAGS_MSGWAIT;
		if (m_fMsgWaiting)
			fSet = TRUE;
	}

	// Set the new status.
	LONG lResult = pLine->SetDeviceStatus(dwDevState, fSet);
	if (lResult != 0)
	{
		ShowError("lineSetLineDevStatus", lResult);
		m_fConnected = fConnected;
		m_fMsgWaiting = fMsgWaiting;
		m_fInService = fInService;
		m_fLocked = m_fLocked;
		UpdateData(FALSE);
	}

}// CPhoneDlg::OnChangeLineStatus

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnAgentInfo
//
// Open a new modaless agent information dialog
//
void CPhoneDlg::OnAgentInfo() 
{
	CTapiAddress* pAddr = GetActiveAddress();
	if (pAddr == NULL)
		return;

	CAgentStateDlg* pDlg = (CAgentStateDlg*) m_mapAgentDlg[pAddr];
	if (pDlg == NULL || !IsWindow(pDlg->GetSafeHwnd()))
	{
		delete pDlg;
		pDlg = new CAgentStateDlg(this, pAddr);
		m_mapAgentDlg[pAddr] = pDlg;
	}

	pDlg->ShowWindow(SW_SHOW);
	pDlg->SetActiveWindow();

}// CPhoneDlg::OnAgentInfo

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnDestroy
//
// This is called when our window is being destroyed - close all the
// child agent windows.
//
void CPhoneDlg::OnDestroy()
{
	for (POSITION pos = m_mapAgentDlg.GetStartPosition(); pos != NULL;)
	{
		void* pAddr, *pDlg;
		m_mapAgentDlg.GetNextAssoc(pos, pAddr, pDlg);
		delete ((CAgentStateDlg*)pDlg);
	}

}// CPhoneDlg::OnDestroy

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnAgentClose
//
// Notification of an agent dialog closing.
//
void CPhoneDlg::OnAgentClose(CTapiAddress* pAddr)
{
	CAgentStateDlg* pDlg = (CAgentStateDlg*) m_mapAgentDlg[pAddr];
	ASSERT (pDlg != NULL);
	m_mapAgentDlg[pAddr] = NULL;

}// CPhoneDlg::OnAgentClose

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnConferenceList
//
// List the members of a conference
//
void CPhoneDlg::OnConferenceList() 
{
	ClearErrors();
	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL)
		return;

	CConfListDlg dlg(this, pCall);
	dlg.DoModal();

}// CPhoneDlg::OnConferenceList

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnTransfer
//
// Initiate a consultation or blind transfer of a call.
//
void CPhoneDlg::OnTransfer() 
{
	ClearErrors();
	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL)
		return;

	LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
	{
		ShowError("lineSetCallPrivilege", lResult);
		return;
	}
		
	CTransferDlg dlgT(this, pCall);
	if (dlgT.DoModal() == IDOK)
	{
		// Consultation transfer..
		if (dlgT.m_fConsultation == TRUE)
		{
			// Setup the transfer..
			CTapiCall* pCCall = NULL;
			LONG lResult = GetTAPIConnection()->WaitForReply(pCall->SetupTransfer(&pCCall));
			if (lResult != 0)
			{
				ShowError("lineSetupTransfer", lResult);
				return;
			}

			// Wait for the call to have the DIAL feature..
			if (!pCCall->WaitForFeature(LINECALLFEATURE_DIAL, 5000))
			{
				ShowError("Dial feature unavailable", -1L);
				pCCall->Drop();
			}
			else
			{
				// Dial the number on the consultation call..
				lResult = pCCall->Dial(dlgT.m_strNumber);
				if (IsTapiError(lResult))
				{
					ShowError("lineDial", lResult);
					pCCall->Drop();
				}
				else
				{
					// Complete the transfer..
					CCompleteTransferDlg dlgC(this, pCall, pCCall);
					if (dlgC.DoModal() == IDOK)
					{
						DWORD dwTransferType = (dlgC.m_fConference == TRUE) ?
								LINETRANSFERMODE_CONFERENCE : LINETRANSFERMODE_TRANSFER;
						CTapiCall* pConfOwner;
						lResult = GetTAPIConnection()->WaitForReply(
								pCall->CompleteTransfer(pCCall, &pConfOwner, dwTransferType));
						if (lResult != 0)
							ShowError("lineCompleteTransfer", lResult);
					}
					// Canceled transfer, drop consultation call
					else
					{
						pCCall->Drop();
						if (pCall->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_UNHOLD)
							pCall->Unhold();
					}
				}
			}
		}

		// Blind transfer to number indicated.
		else
		{
			LONG lResult = GetTAPIConnection()->WaitForReply(pCall->BlindTransfer(dlgT.m_strNumber, 0));
			if (lResult != 0)
				ShowError("lineBlindTransfer", lResult);
		}
	}

}// CPhoneDlg::OnTransfer

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnParkCall
//
// Park a call to an extension
//
void CPhoneDlg::OnParkCall() 
{
	ClearErrors();
	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL)
		return;

	// Ask the user for the extension
	CDialDlg dlg(this, pCall->GetLineOwner(), LINECALLFEATURE_PARK);
	if (dlg.DoModal() == IDOK)
	{
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ShowError("lineSetCallPrivilege", lResult);
		else
		{
			DWORD dwParkMode = (dlg.m_strNumber.IsEmpty()) ? 
						LINEPARKMODE_NONDIRECTED : 
						LINEPARKMODE_DIRECTED;
			char szBuff[255];
			memset(szBuff, 0, 255);

			lResult = GetTAPIConnection()->WaitForReply(
					pCall->Park(dwParkMode, dlg.m_strNumber, szBuff, 255));
			if (lResult != 0)
				ShowError("linePark", lResult);
			else if (szBuff[0] != '\0')
				ErrorMsg("Call Parked at %s", szBuff);
		}
	}

}// CPhoneDlg::OnParkCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnCompleteTransfer
//
// Complete a transfer
//
void CPhoneDlg::OnCompleteTransfer() 
{
	ClearErrors();
	CTapiLine* pLine = GetActiveLine();
	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL || pLine == NULL)
		return;

	// Find the other call.  If this call is NOT onHOLD, then
	// look for any onHold call with the COMPLETETRANS bit set.
	CTapiCall* pCall_Cons = NULL;
	if ((pCall->GetCallState() & (LINECALLSTATE_ONHOLD | 
			LINECALLSTATE_ONHOLDPENDTRANSFER |
			LINECALLSTATE_ONHOLDPENDCONF)))
	{
		 pCall_Cons = pLine->FindCall((LINECALLSTATE_CONNECTED | 
				LINECALLSTATE_PROCEEDING |
				LINECALLSTATE_DIALING |
				LINECALLSTATE_DIALTONE), LINECALLFEATURE_COMPLETETRANSF);
	}
	else
	{
		pCall_Cons = pCall;
		pCall = pLine->FindCall((LINECALLSTATE_ONHOLD | 
				LINECALLSTATE_ONHOLDPENDTRANSFER |
				LINECALLSTATE_ONHOLDPENDCONF), LINECALLFEATURE_COMPLETETRANSF);
	}

	if (pCall == NULL || pCall_Cons == NULL)
	{
		ErrorMsg("Unable to locate two calls with lineCompleteTransfer ability");
		return;
	}

	// Set the privileges on both calls
	LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
	{
		ShowError("lineSetCallPrivilege", lResult);
		return;
	}

	lResult = pCall_Cons->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
	{
		ShowError("lineSetCallPrivilege", lResult);
		return;
	}

	// Complete the transfer
	CTapiCall* pConfOwner;
	lResult = GetTAPIConnection()->WaitForReply(
			pCall->CompleteTransfer(pCall_Cons, &pConfOwner, LINETRANSFERMODE_TRANSFER));
	if (lResult != 0)
		ShowError("lineCompleteTransfer", lResult);

}// CPhoneDlg::OnCompleteTransfer

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnCompleteCall
//
// Complete a call delivery
//
void CPhoneDlg::OnCompleteCall() 
{
	ClearErrors();
	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL)
		return;

	// Ask the user for the extension
	CCompleteCall dlg(this, pCall);
	if (dlg.DoModal() == IDOK)
	{
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ShowError("lineSetCallPrivilege", lResult);
		else
		{
			DWORD dwCompletionID;
			lResult = GetTAPIConnection()->WaitForReply(
					pCall->CompleteCall(&dwCompletionID, dlg.m_dwMode, dlg.m_dwMessageID));
			if (lResult != 0)
				ShowError("lineCompleteCall", lResult);
			else
				ErrorMsg("Call Completion ID 0x%lx", dwCompletionID);
		}
	}

}// CPhoneDlg::OnCompleteCall

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnSetupConference
//
// Setup a new conference using an existing call
//
void CPhoneDlg::OnSetupConference() 
{
	ClearErrors();
	CTapiLine* pLine = GetActiveLine();
	CTapiCall* pCall = GetActiveCall();
	if (pLine == NULL || pCall == NULL)
		return;

	// See if we have calls which can be placed into a conference via
	// transfer.  This requires at least one call on hold and one 
	// connected call.
	BOOL fMergeAllowed = FALSE;
	LPLINECALLSTATUS lpStatus = pCall->GetCallStatus();
	if ((lpStatus->dwCallFeatures & LINECALLFEATURE_COMPLETETRANSF &&
		 pCall->GetAddressInfo()->GetAddressCaps()->dwTransferModes & LINETRANSFERMODE_CONFERENCE) || 
		 (lpStatus->dwCallFeatures2 & LINECALLFEATURE2_TRANSFERCONF))
	{
		fMergeAllowed = TRUE;
	}

	CSetupConfDlg dlg(this, pCall, fMergeAllowed);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_fMerge == FALSE)
		{
			LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
			if (lResult != 0)
				ShowError("lineSetCallPrivilege", lResult);
			else
			{
				CTapiCall* pConf;
				CTapiCall* pCons;
				lResult = GetTAPIConnection()->WaitForReply(
						pCall->SetupConference(&pConf, &pCons, 3, NULL));
				if (lResult != 0)
					ShowError("lineSetupConference", lResult);

				// Complete the conference event.
				else
				{
					// Wait for the call to have the DIAL feature..
					if (!pCons->WaitForFeature(LINECALLFEATURE_DIAL, 5000))
					{
						ShowError("Dial feature unavailable", -1L);
						pCons->Drop();
						pConf->Drop();
					}
					else
					{
						// Dial the number on the consultation call.
						lResult = pCons->Dial(dlg.m_strNumber);
						if (IsTapiError(lResult))
						{
							ShowError("lineDial", lResult);
							pCons->Drop();
							pConf->Drop();
						}
						else
						{
							// Let the user decide when to complete the conference.
							CCompleteTransferDlg dlgC(this, pCall, pCons, TRUE);
							if (dlgC.DoModal() == IDOK)
							{
								lResult = GetTAPIConnection()->WaitForReply(
										pConf->AddToConference(pCons));
								if (lResult != 0)
									ShowError("lineAddToConference", lResult);
							}
							// Canceled conference, drop consultation call
							else
							{
								pCons->Drop();
								pCall->RemoveFromConference();
								pConf->Drop();
							}
						}
					}
				}
			}
		}
		// Merge conference call
		else
		{
			// Find the other call(s).  If this call is NOT onHOLD, then
			// look for any onHold call with the COMPLETETRANS bit set.
			CTapiCall* pCall_Cons = NULL;
			if ((pCall->GetCallState() & (LINECALLSTATE_ONHOLD | 
					LINECALLSTATE_ONHOLDPENDTRANSFER |
					LINECALLSTATE_ONHOLDPENDCONF)))
			{
				 pCall_Cons = pLine->FindCall((LINECALLSTATE_CONNECTED | 
						LINECALLSTATE_PROCEEDING |
						LINECALLSTATE_DIALING |
						LINECALLSTATE_DIALTONE), LINECALLFEATURE_COMPLETETRANSF);
			}
			else
			{
				pCall_Cons = pCall;
				pCall = pLine->FindCall((LINECALLSTATE_ONHOLD | 
						LINECALLSTATE_ONHOLDPENDTRANSFER |
						LINECALLSTATE_ONHOLDPENDCONF), LINECALLFEATURE_COMPLETETRANSF);
			}

			if (pCall == NULL || pCall_Cons == NULL)
			{
				ErrorMsg("Unable to locate two calls with lineCompleteTransfer ability");
				return;
			}

			// Set the privileges on both calls
			LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
			if (lResult != 0)
			{
				ShowError("lineSetCallPrivilege", lResult);
				return;
			}

			lResult = pCall_Cons->SetPrivilege(LINECALLPRIVILEGE_OWNER);
			if (lResult != 0)
			{
				ShowError("lineSetCallPrivilege", lResult);
				return;
			}

			// Complete the transfer
			CTapiCall* pConfOwner;
			lResult = GetTAPIConnection()->WaitForReply(
					pCall->CompleteTransfer(pCall_Cons, &pConfOwner, LINETRANSFERMODE_CONFERENCE));
			if (lResult != 0)
				ShowError("lineCompleteTransfer", lResult);

			// Otherwise see if we have additional calls that are onHOLD right now
			// and add them to this new conference.
			else
			{
				CTapiCall* pOldCall = pCall;
				int iCount = 0;
				do 
				{
					pCall = pLine->FindCall((LINECALLSTATE_ONHOLD | 
							LINECALLSTATE_ONHOLDPENDTRANSFER |
							LINECALLSTATE_ONHOLDPENDCONF), LINECALLFEATURE_COMPLETETRANSF);

					if (pCall == pOldCall)
					{
						if (++iCount > 5)
							break;
						Sleep(1000);
					}

					else if (pCall != NULL)
					{
						pOldCall = pCall;
						lResult = GetTAPIConnection()->WaitForReply(
							pConfOwner->AddToConference(pCall));
						if (lResult != 0)
							ShowError("lineAddToConference", lResult);
					}

				} while (pCall != NULL);
			}
		}
	}

}// CPhoneDlg::OnSetupConference

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnAddToConference
//
// Add a new party to a conference.
//
void CPhoneDlg::OnAddToConference() 
{
	ClearErrors();
	CTapiLine* pLine = GetActiveLine();
	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL || pLine == NULL)
		return;

	// If this is a conference call that is not connected, create a new party for
	// the call.
	if (pCall->GetCallInfo()->dwOrigin == LINECALLORIGIN_CONFERENCE &&
		pCall->GetCallState() == LINECALLSTATE_CONNECTED &&
		(pCall->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_PREPAREADDCONF) != 0)
	{
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ShowError("lineSetCallPrivilege", lResult);
		else
		{
			CTapiCall* pConsCall;
			lResult = GetTAPIConnection()->WaitForReply(pCall->PrepareAddToConference(&pConsCall, NULL));
			if (lResult != 0)
				ShowError("linePrepareAddToConference", lResult);
		}
		return;
	}

	// Find the other call.  If this call is NOT onHOLD, then
	// look for any onHold call with the COMPLETETRANS bit set.
	CTapiCall* pCall_Cons = NULL;
	if (pCall->GetCallInfo()->dwOrigin == LINECALLORIGIN_CONFERENCE)
	{
		 pCall_Cons = pLine->FindCall((LINECALLSTATE_CONNECTED | 
				LINECALLSTATE_PROCEEDING |
				LINECALLSTATE_DIALING |
				LINECALLSTATE_DIALTONE), LINECALLFEATURE_ADDTOCONF);
	}
	else
	{
		pCall_Cons = pCall;
		pCall = pLine->FindCall((LINECALLSTATE_ONHOLD | 
				LINECALLSTATE_ONHOLDPENDTRANSFER |
				LINECALLSTATE_ONHOLDPENDCONF), LINECALLFEATURE_ADDTOCONF);
		if (pCall == NULL)
			pCall = pLine->FindCall(LINECALLSTATE_ONHOLDPENDCONF, 0);
	}

	if (pCall == NULL || pCall_Cons == NULL)
	{
		ErrorMsg("Unable to locate two calls with lineAddToConference ability");
		return;
	}

	// Set the privileges on both calls
	LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
	{
		ShowError("lineSetCallPrivilege", lResult);
		return;
	}

	lResult = pCall_Cons->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
	{
		ShowError("lineSetCallPrivilege", lResult);
		return;
	}

	// Complete the transfer
	lResult = GetTAPIConnection()->WaitForReply(
			pCall->AddToConference(pCall_Cons));
	if (lResult != 0)
		ShowError("lineAddToConference", lResult);

}// CPhoneDlg::OnAddToConference

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnForward
//
// This is called to forward the phone by the user.
//
void CPhoneDlg::OnForward() 
{
	CTapiLine* pLine = GetActiveLine();
	if (pLine == NULL)
		return;

	UpdateData(TRUE);
	ClearErrors();

	CForwardDlg dlg(this, pLine);
	if (dlg.DoModal() == IDOK)
	{
		LINEFORWARDLIST* plist = NULL;		
		if (dlg.m_dwFwdMode != 0)
		{
			DWORD dwSize = dlg.m_strCaller.GetLength() + dlg.m_strDest.GetLength() + sizeof(LINEFORWARD) + 2;
			DWORD dwUsedSize = sizeof(LINEFORWARD) + sizeof(LINEFORWARDLIST);
			plist = (LINEFORWARDLIST*) new BYTE[dwSize + sizeof(LINEFORWARDLIST)];
			ZeroMemory(plist, dwSize + sizeof(LINEFORWARDLIST));
			if (plist == NULL) return;
			plist->dwNumEntries = 1;
			plist->dwTotalSize = dwSize + sizeof(LINEFORWARDLIST);
			LINEFORWARD* plf = &plist->ForwardList[0];
			plf->dwForwardMode = dlg.m_dwFwdMode;
			plf->dwDestCountryCode = 0;
			if (!dlg.m_strCaller.IsEmpty())
			{
				plf->dwCallerAddressOffset = dwUsedSize;
				plf->dwCallerAddressSize = dlg.m_strCaller.GetLength()+1;
				lstrcpyn((LPSTR)((LPBYTE)plist+dwUsedSize), dlg.m_strCaller, plf->dwCallerAddressSize);
				dwUsedSize += plf->dwCallerAddressSize;
			}

			if (!dlg.m_strDest.IsEmpty())
			{
				plf->dwDestAddressOffset = dwUsedSize;
				plf->dwDestAddressSize = dlg.m_strDest.GetLength()+1;
				lstrcpyn((LPSTR)((LPBYTE)plist+dwUsedSize), dlg.m_strDest, plf->dwDestAddressSize);
			}
		}

		CTapiCall* pCall = NULL;
		LONG lResult = GetTAPIConnection()->WaitForReply(
			pLine->Forward((dlg.m_bAllAddresses) ? -1L : dlg.m_dwAddress, plist, dlg.m_iNumRings, &pCall, NULL));
		if (lResult != 0)
			ShowError("lineForward", lResult);

		if (pCall != NULL)
		{
			pCall->Drop();
			pCall->Deallocate();
		}

		delete [] (BYTE*) plist;
	}
	
}// CPhoneDlg::OnForward

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnRemoveFromConference
//
// Remove a party from the conference
//
void CPhoneDlg::OnRemoveFromConference() 
{
	ClearErrors();
	CTapiCall* pCall = GetActiveCall();
	if (pCall == NULL)
		return;

	// Set the privileges on both calls
	LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		lResult = GetTAPIConnection()->WaitForReply(pCall->RemoveFromConference());
		if (lResult != 0)
			ShowError("lineRemoveFromConference", lResult);
	}

}// CPhoneDlg::OnRemoveFromConference

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnPhoneInfo
//
// Display the phone information
//
void CPhoneDlg::OnPhoneInfo() 
{
	ClearErrors();
	CTapiLine* pLine = GetActiveLine();
	if (pLine == NULL)
		return;

	DWORD dwPhoneID = pLine->GetRelatedPhoneID();
	if (dwPhoneID == 0xffffffff)
	{
		ErrorMsg("There is no phone related to this line.");
		return;
	}

	CMyPhone* pPhone = (CMyPhone*) GetTAPIConnection()->GetPhoneFromDeviceID(dwPhoneID);
	if (pPhone == NULL)
	{
		ErrorMsg("Invalid phoneID (%ld) from tapi/phone", dwPhoneID);
		return;
	}

	// If we have a dialog then just show it.
	if (pPhone->m_pPhone != NULL)
	{
		pPhone->m_pPhone->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		return;
	}

	// Open our phone device.
	LONG lResult = pPhone->Open(PHONEPRIVILEGE_OWNER);
	if (lResult != 0)
	{
		ShowError("phoneOpen", lResult);
		return;
	}

	lResult = pPhone->SetStatusMessages(0xffffff, 0, 0);
	if (lResult != 0)
		ShowError("phoneSetStatusMessages", lResult);
	pPhone->m_pPhone = new CPhoneCapsDlg(this, pPhone);

}// CPhoneDlg::OnPhoneInfo

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnConfig
//
// Display the line configuration for the open line
//
void CPhoneDlg::OnConfig() 
{
	ClearErrors();
	CTapiLine* pLine = GetActiveLine();
	if (pLine != NULL)
		pLine->Config(this, NULL);

}// CPhoneDlg::OnConfig

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnClose
//
// Signals the window to close
//
void CPhoneDlg::OnClose() 
{
	// Look to see if we have pending requests we are still waiting for.
	// If so, prompt the user and let them decide whether to wait or
	// to destroy them now.
	int iCount = GetTAPIConnection()->GetPendingRequestCount();
	if (iCount > 0)
	{
		TCHAR chBuff[256];
		wsprintf(chBuff, _T("There are %d pending TAPI requests which have not completed.\r\nDo you want to wait for these to complete?"), iCount);
		if (AfxMessageBox(chBuff, MB_YESNO) == IDNO)
			GetTAPIConnection()->StopWaitingForAllRequests();
	}

	EndDialog(IDCANCEL);

}// CPhoneDlg::OnClose

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg::OnFlashWindow
//
// Change a button font to BOLD to show that it is changed.
//
LRESULT CPhoneDlg::OnFlashWindow(WPARAM wParam, LPARAM /*lParam*/)
{
	static DWORD dwTickCount = 0;
	static DWORD dwLastWindow = 0;

	if (dwLastWindow == wParam &&
		dwTickCount + 1000 >= GetTickCount())
		return 0;

    CFont fntAnsi;
    fntAnsi.CreateStockObject (ANSI_VAR_FONT);

	CWnd* pwnd = GetDlgItem(wParam);
	if (pwnd != NULL)
	{
		for (int i = 0; i < 5; i++)
		{
			pwnd->SetFont(&m_fntBold);
			pwnd->UpdateWindow();
			Sleep(50);
			pwnd->SetFont(&fntAnsi);
			pwnd->UpdateWindow();
			Sleep(50);
		}
	}

	dwLastWindow = wParam;
	dwTickCount = GetTickCount();

	return 0;

}// CPhoneDlg::OnFlashWindow