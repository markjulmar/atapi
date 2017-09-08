// DIALOG.CPP - Dialog to run the lineAnswer example
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
#include <stdarg.h>
#include "dialog.h"

class CMyLine : public CTapiLine
{
	DECLARE_DYNCREATE (CMyLine)

// Overrides
protected:
	virtual void OnDeviceStateChange(DWORD dwState, DWORD dwDetail1, DWORD dwDetail2);
    virtual void OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail, DWORD dwPrivilage);
	virtual void OnCallInfoChange (HCALL hCall, DWORD dwCallInfoState);
	virtual void OnNewCall (CTapiCall* pCall);
};

IMPLEMENT_DYNCREATE (CMyLine, CTapiLine)

/////////////////////////////////////////////////////////////////////////////
// Globals

CMainApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainApp::InitInstance
//
// Initialize the main application
//
BOOL CMainApp::InitInstance()
{
    // Initialize a connection with TAPI and determine if there 
    // are any TAPI complient devices installed.
    if (GetTAPIConnection()->Init ("ANSWER", RUNTIME_CLASS(CMyLine)) != 0 ||
        GetTAPIConnection()->GetLineDeviceCount() == 0)
    {
        AfxMessageBox ("There are no TAPI devices installed!");
		return FALSE;
    }

	// Create our dialog.
	CAnswerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;

}// CMainApp::InitInstance

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::CAnswerDlg
//
// Constructor for the answer dialog
//
CAnswerDlg::CAnswerDlg(CWnd* pParent) : CDialog(CAnswerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnswerDlg)
	m_pLine = NULL;
	m_pCall = NULL;
	//}}AFX_DATA_INIT

}// CAnswerDlg::CAnswerDlg

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::~CAnswerDlg
//
// Destructor for the answer dialog
//
CAnswerDlg::~CAnswerDlg()
{
	if (m_pLine)
		m_pLine->Close();

}// CAnswerDlg::~CAnswerDlg

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::DoDataExchange
//
// Dialog data exchange for the answer dialog
//
void CAnswerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnswerDlg)
	DDX_Control(pDX, IDC_STATUS, m_lbStatus);
	DDX_Control(pDX, IDC_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_LINES, m_cbLines);
	DDX_Control(pDX, IDC_DROP, m_btnDrop);
	DDX_Control(pDX, IDC_ANSWER, m_btnAnswer);
	DDX_Control(pDX, IDC_ACCEPT, m_btnAccept);
	//}}AFX_DATA_MAP

}// CAnswerDlg::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg message map

BEGIN_MESSAGE_MAP(CAnswerDlg, CDialog)
	//{{AFX_MSG_MAP(CAnswerDlg)
	ON_BN_CLICKED(IDC_ACCEPT, OnAccept)
	ON_BN_CLICKED(IDC_ANSWER, OnAnswer)
	ON_BN_CLICKED(IDC_DROP, OnDrop)
	ON_CBN_SELCHANGE(IDC_LINES, OnSelchangeLines)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnInitDialog
//
// Manage the creation of our dialog box.
//
BOOL CAnswerDlg::OnInitDialog()
{
    // Reset the font to all be ANSI var.
    CFont fntAnsi;
    fntAnsi.CreateStockObject (ANSI_VAR_FONT);
    CWnd* pwndChild = GetWindow (GW_CHILD);
    while (pwndChild != NULL && IsChild(pwndChild))
    {
        pwndChild->SetFont(&fntAnsi);
        pwndChild = pwndChild->GetWindow(GW_HWNDNEXT);
    }

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

    // Now go through and enumerate all the line devices in the system.
    // These get placed into our combo box.
    for (DWORD dwCount = 0; 
		 dwCount < GetTAPIConnection()->GetLineDeviceCount(); 
		 dwCount++)
    {
        CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwCount);
        if (pLine)
        {
			const LINEDEVCAPS* pCaps = pLine->GetLineCaps();
			if (pCaps && pCaps->dwMediaModes & (LINEMEDIAMODE_DATAMODEM | LINEMEDIAMODE_INTERACTIVEVOICE))
			{
				int iPos = m_cbLines.AddString (pLine->GetLineName());
				ASSERT (iPos >= 0);
				m_cbLines.SetItemData(iPos, dwCount);
			}
        }
    }
    
    // Automatically select the first entry in the combo box.
    if (m_cbLines.GetCount() > 0)
    {
        m_cbLines.SetCurSel(0);
        OnSelchangeLines();
    }        
	else
		AfxMessageBox ("None of the TAPI lines support DATAMODEM/VOICE access");

	// Setup the initial button states
	UpdateButtons();

	return TRUE;

}// CAnswerDlg::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::UpdateButtons
//
// Enable/Disable the buttons based on our current states
//
void CAnswerDlg::UpdateButtons()
{
	m_btnOpen.EnableWindow (m_pLine == NULL);
	m_btnAnswer.EnableWindow (FALSE);
	m_btnAccept.EnableWindow (FALSE);

	if (m_pCall != NULL)
	{
		m_btnDrop.EnableWindow (TRUE);
		if (m_pCall->GetCallState() == LINECALLSTATE_OFFERING)
		{
			m_btnAnswer.EnableWindow (TRUE);
			m_btnAccept.EnableWindow (TRUE);
		}
		else if (m_pCall->GetCallState() == LINECALLSTATE_ACCEPTED)
			m_btnAnswer.EnableWindow (TRUE);
	}
	else
		m_btnDrop.EnableWindow (FALSE);

}// CAnswerDlg::UpdateButtons

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnAccept
//
// Accept the offering call
//
void CAnswerDlg::OnAccept() 
{
	if (m_pCall)
		m_pCall->Accept();

}// CAnswerDlg::OnAccept

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnAnswer
//
// Answer an offering call
//
void CAnswerDlg::OnAnswer() 
{
	if (m_pCall)
		m_pCall->Answer();

}// CAnswerDlg::OnAnswer

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnDrop
//
// Drop an active call
//
void CAnswerDlg::OnDrop() 
{
	if (m_pCall)
		m_pCall->Drop();

}// CAnswerDlg::OnDrop

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnSelchangeLines
//
// Change the currently selected line
//
void CAnswerDlg::OnSelchangeLines() 
{
	LONG lResult;

	if (m_pLine != NULL)
	{
		lResult = m_pLine->Close();
		TraceMsg ("Line %ld closed, rc=0x%lx", m_pLine->GetDeviceID(), lResult);
		m_pLine = NULL;
	}

	UpdateButtons();

}// CAnswerDlg::OnSelchangeLines

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnOpen
//
// Open the specified line
//
void CAnswerDlg::OnOpen() 
{
	int iCurSel = m_cbLines.GetCurSel();
	if (iCurSel >= 0)
	{
		DWORD dwLineId = m_cbLines.GetItemData(iCurSel);
		m_pLine = GetTAPIConnection()->GetLineFromDeviceID(dwLineId);
		const LPLINEDEVCAPS lpCaps = m_pLine->GetLineCaps();
		DWORD dwType = (LINEMEDIAMODE_INTERACTIVEVOICE | LINEMEDIAMODE_DATAMODEM);
		if (lpCaps)
			dwType &= lpCaps->dwMediaModes;
		LONG lResult = m_pLine->Open (LINECALLPRIVILEGE_OWNER | LINECALLPRIVILEGE_MONITOR, dwType);
		if (lResult == LINEERR_INVALMEDIAMODE)
		{
			TraceMsg ("Line %ld failed open due to MEDIA MODE, retrying with only MODEM", m_pLine->GetDeviceID());
			lResult = m_pLine->Open (LINECALLPRIVILEGE_OWNER | LINECALLPRIVILEGE_MONITOR, LINEMEDIAMODE_DATAMODEM);
		}
		TraceMsg ("Line %ld opened (OWNER), rc=0x%lx", m_pLine->GetDeviceID(), lResult);

		if (lResult != 0)
			m_pLine = NULL;
		else
			m_pLine->SetStatusMessages(LINEDEVSTATE_RINGING | LINEDEVSTATE_MSGWAITON | 
										LINEDEVSTATE_MSGWAITOFF, 0L);
	}

	UpdateButtons();

}// CAnswerDlg::OnOpen

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnOK
//
// Dismiss the dialog
//
void CAnswerDlg::OnOK() 
{
	CDialog::OnOK();

}// CAnswerDlg::OnOK

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::TraceMsg
//
// Function to place a string in our status listbox.
//
void CAnswerDlg::TraceMsg (const char * pszStr, ...)
{
   va_list args;
   static char szBuff[1024];
  
   va_start(args, pszStr);
   vsprintf(szBuff, pszStr, args);
   va_end(args);

   m_lbStatus.AddString (szBuff);

}// CAnswerDlg::TraceMsg

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnCallStateChange
//
// Our call has changed state
//
void CAnswerDlg::OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail)
{
	// If its not our call, ignore it.
	if (m_pCall == NULL || m_pCall->GetCallHandle() != hCall)
		return;

	// Otherwise, report a status change.
	TraceMsg ("Call 0x%lx <%s>", m_pCall->GetCallHandle(), GetCallState(dwState, dwStateDetail));
	if (dwState == LINECALLSTATE_IDLE)
	{
		TraceMsg ("Deallocating call 0x%lx", m_pCall->GetCallHandle());
		m_pCall->Deallocate();
		m_pCall = NULL;
	}

	UpdateButtons();

}// CAnswerDlg::OnCallStateChange

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnCallerID
//
// Called when a call has CALLERID information
//
void CAnswerDlg::OnCallerID(HCALL hCall)
{
	// If its not our call, ignore it.
	if (m_pCall == NULL || m_pCall->GetCallHandle() != hCall)
		return;

	// Otherwise, report CALLERID information
	TraceMsg ("CallerID for 0x%lx", m_pCall->GetCallHandle());

	const LINECALLINFO* pCallInfo = m_pCall->GetCallInfo();
	if (pCallInfo->dwCallerIDFlags & LINECALLPARTYID_BLOCKED)
		TraceMsg ("   BLOCKED (PRIVATE)");
	if (pCallInfo->dwCallerIDFlags & LINECALLPARTYID_OUTOFAREA)
		TraceMsg ("   OUT OF AREA");
	if (pCallInfo->dwCallerIDFlags & LINECALLPARTYID_ADDRESS)
		TraceMsg ("   Number: %s", m_pCall->GetCallerIDNumber());
	if (pCallInfo->dwCallerIDFlags & LINECALLPARTYID_NAME)
		TraceMsg ("   Name <%s>", m_pCall->GetCallerIDName());

}// CAnswerDlg::OnCallerID

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnNewCall
//
// A new incoming call has been detected.
//
void CAnswerDlg::OnNewCall(CTapiCall* pCall)
{
	if (m_pCall == NULL)
	{
		m_pCall = pCall;
		TraceMsg ("New Call detected <0x%lx>", pCall->GetCallHandle());
	}

}// CAnswerDlg::OnNewCall

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnRing
//
// Line received RING
//
void CAnswerDlg::OnRing()
{
	TraceMsg ("!!! RING !!!");		

}// CAnswerDlg::OnRing

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::OnMsgWait
//
// Message waiting lamp changed.
//
void CAnswerDlg::OnMsgWait(BOOL fOn)
{
	TraceMsg ("Message Waiting lamp is now %s", (fOn) ? "On" : "Off");

}// CAnswerDlg::OnMsgWait

/////////////////////////////////////////////////////////////////////////////
// CAnswerDlg::GetCallState
//
// Return a textual string describing the call state
//
CString CAnswerDlg::GetCallState (DWORD dwState, DWORD /*dwDetail*/)
{
	switch (dwState)
	{
		case LINECALLSTATE_IDLE: return "Idle";
		case LINECALLSTATE_OFFERING: return "Offering";
		case LINECALLSTATE_ACCEPTED: return "Accepted";
		case LINECALLSTATE_CONNECTED: return "Connected";
		case LINECALLSTATE_DISCONNECTED: return "Disconnected";
	}
	return "???";

}// CAnswerDlg::GetCallState

/////////////////////////////////////////////////////////////////////////////
// CMyLine::OnCallStateChange
//
// A call on this line has changed state
//
void CMyLine::OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail, DWORD dwPrivilage)
{
	CTapiLine::OnCallStateChange (hCall, dwState, dwStateDetail, dwPrivilage);
	CAnswerDlg* pDlg = (CAnswerDlg*) AfxGetApp()->m_pMainWnd;
	pDlg->OnCallStateChange (hCall, dwState, dwStateDetail);

}// CMyLine::OnCallStateChange

/////////////////////////////////////////////////////////////////////////////
// CMyLine::OnCallInfoChange
//
// A call has changed some CALLINFO information.
//
void CMyLine::OnCallInfoChange (HCALL hCall, DWORD dwCallInfoState)
{
	CTapiLine::OnCallInfoChange (hCall, dwCallInfoState);
	if (dwCallInfoState == LINECALLINFOSTATE_CALLERID)
	{
		CAnswerDlg* pDlg = (CAnswerDlg*) AfxGetApp()->m_pMainWnd;
		pDlg->OnCallerID (hCall);
	}

}// CMyLine::OnCallStateChange

/////////////////////////////////////////////////////////////////////////////
// CMyLine::OnDeviceStateChange
//
// Something on the line has changed - we watch for RING events
//
void CMyLine::OnDeviceStateChange(DWORD dwState, DWORD dwDetail1, DWORD dwDetail2)
{
	CAnswerDlg* pDlg = (CAnswerDlg*) AfxGetApp()->m_pMainWnd;
	CTapiLine::OnDeviceStateChange(dwState, dwDetail1, dwDetail2);
	if (dwState == LINEDEVSTATE_RINGING)
		pDlg->OnRing();
	else if (dwState == LINEDEVSTATE_MSGWAITON)
		pDlg->OnMsgWait(TRUE);
	else if (dwState == LINEDEVSTATE_MSGWAITOFF)
		pDlg->OnMsgWait(FALSE);

}// CMyLine::OnCallStateChange

/////////////////////////////////////////////////////////////////////////////
// CMyLine::OnNewCall
//
// Handle a new incoming call.
//
void CMyLine::OnNewCall (CTapiCall* pCall)
{
	CTapiLine::OnNewCall (pCall);
	CAnswerDlg* pDlg = (CAnswerDlg*) AfxGetApp()->m_pMainWnd;
	pDlg->OnNewCall (pCall);

}// CMyLine::OnNewCall
