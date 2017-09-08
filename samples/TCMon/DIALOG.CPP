// DIALOG.CPP - Dialog to run the TCMON code
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

/*------------------------------------------------------------------------------
    LISTVIEW COLUMN STRUCTURE
  This structure defines the column information which is setup for
  a listview control on our main view.
--------------------------------------------------------------------------------*/
struct
{
	const char * pszName;		// Name for the column
	int iWidth;					// Size of the column in characters

} g_CallColumns[] = {
	{ "Line",       15 },
	{ "Call",		8  },
	{ "State",		15 },
	{ "Caller",		15 },
	{ "Called",     15 }
};
#define TOTAL_CALL_COLUMNS (sizeof(g_CallColumns) / sizeof(g_CallColumns[0]))

const UINT IDT_TIMER = 100;
static DWORD g_dwLastError = 0;

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

   theApp.m_pMainWnd->SetDlgItemText(IDC_STATUS, szBuff);
   g_dwLastError = GetTickCount();

}// ErrorMsg

/////////////////////////////////////////////////////////////////////////////
// ClearErrors
//
// Clear any errors on the console
//
void ClearErrors()
{
   theApp.m_pMainWnd->SetDlgItemText(IDC_STATUS, "");

}// ClearErrors

/////////////////////////////////////////////////////////////////////////////
// CMainApp::InitInstance
//
// Initialize the main application
//
BOOL CMainApp::InitInstance()
{
    // Initialize a connection with TAPI and determine if there 
    // are any TAPI complient devices installed.
    if (GetTAPIConnection()->Init ("TapiCallMonitor", RUNTIME_CLASS(CMyLine)) != 0 ||
        GetTAPIConnection()->GetLineDeviceCount() == 0)
    {
        AfxMessageBox ("There are no TAPI devices installed!");
		return FALSE;
    }

	// Create our dialog.
	CMonitorDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;

}// CMainApp::InitInstance

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::CMonitorDlg
//
// Constructor for the answer dialog
//
CMonitorDlg::CMonitorDlg(CWnd* pParent) : CDialog(CMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonitorDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}// CMonitorDlg::CMonitorDlg

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::~CMonitorDlg
//
// Destructor for the answer dialog
//
CMonitorDlg::~CMonitorDlg()
{
}// CMonitorDlg::~CMonitorDlg

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::DoDataExchange
//
// Dialog data exchange for the answer dialog
//
void CMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitorDlg)
	DDX_Control(pDX, IDC_UNHOLD, m_btnUnhold);
	DDX_Control(pDX, IDC_HOLD, m_btnHold);
	DDX_Control(pDX, IDC_CALLLIST, m_lcCalls);
	DDX_Control(pDX, IDC_DROP, m_btnDrop);
	DDX_Control(pDX, IDC_ANSWER, m_btnAnswer);
	DDX_Control(pDX, IDC_ACCEPT, m_btnAccept);
	//}}AFX_DATA_MAP

}// CMonitorDlg::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg message map

BEGIN_MESSAGE_MAP(CMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CMonitorDlg)
	ON_BN_CLICKED(IDC_ACCEPT, OnAccept)
	ON_BN_CLICKED(IDC_ANSWER, OnAnswer)
	ON_BN_CLICKED(IDC_DROP, OnDrop)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_CALLLIST, OnCallGetDispInfo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CALLLIST, OnCallChange)
	ON_BN_CLICKED(IDC_HOLD, OnHold)
	ON_BN_CLICKED(IDC_UNHOLD, OnUnhold)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnInitDialog
//
// Manage the creation of our dialog box.
//
BOOL CMonitorDlg::OnInitDialog()
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Setup our list view.
	// Get the metrics which are being used for this control.
	CDC* pDC = m_lcCalls.GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	m_lcCalls.ReleaseDC(pDC);

	// Setup the TAPI call list
	for (int i = 0; i < TOTAL_CALL_COLUMNS; i++)
		m_lcCalls.InsertColumn (i, g_CallColumns[i].pszName, LVCFMT_LEFT,
							tm.tmAveCharWidth*(g_CallColumns[i].iWidth+1));

	// Open all our lines.
	OpenAllLines();

	// Set our interval timer
	SetTimer(IDT_TIMER, 1000, NULL);

	// Setup the initial button states
	UpdateButtons();

	return TRUE;

}// CMonitorDlg::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnPaint
//
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
//
void CMonitorDlg::OnPaint() 
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
}// CMonitorDlg::OnPaint

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnQueryDragIcon
//
// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
//
HCURSOR CMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;

}// CMonitorDlg::OnQueryDragIcon

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::UpdateButtons
//
// Enable/Disable the buttons based on our current states
//
void CMonitorDlg::UpdateButtons()
{
	m_btnAnswer.EnableWindow (FALSE);
	m_btnAccept.EnableWindow (FALSE);
	m_btnHold.EnableWindow (FALSE);
	m_btnUnhold.EnableWindow (FALSE);
	m_btnDrop.EnableWindow (FALSE);

	CTapiCall* pCall = GetSelectedCall();
	if (pCall != NULL)
	{
		const LPLINECALLSTATUS pStat = pCall->GetCallStatus();
		if (pStat != NULL)
		{
			if (pStat->dwCallFeatures & LINECALLFEATURE_DROP)
				m_btnDrop.EnableWindow (TRUE);
			if (pStat->dwCallFeatures & LINECALLFEATURE_ACCEPT)
				m_btnAccept.EnableWindow (TRUE);
			if (pStat->dwCallFeatures & LINECALLFEATURE_ANSWER)
				m_btnAnswer.EnableWindow (TRUE);
			if (pStat->dwCallFeatures & LINECALLFEATURE_HOLD)
				m_btnHold.EnableWindow (TRUE);
			if (pStat->dwCallFeatures & LINECALLFEATURE_UNHOLD)
				m_btnUnhold.EnableWindow (TRUE);
		}
	}

}// CMonitorDlg::UpdateButtons

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnAccept
//
// Accept the offering call
//
void CMonitorDlg::OnAccept() 
{
	ClearErrors();

	CTapiCall* pCall = GetSelectedCall();
	if (pCall)
	{
		// Make sure we are owner.
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ErrorMsg ("lineSetCallPrivilege failed, rc=0x%lx", lResult);
		else
		{
			lResult = GetTAPIConnection()->WaitForReply(pCall->Accept());
			if (lResult != 0)
				ErrorMsg("lineAccept failed, rc=0x%lx", lResult);
		}
	}

}// CMonitorDlg::OnAccept

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnAnswer
//
// Answer an offering call
//
void CMonitorDlg::OnAnswer() 
{
	ClearErrors();

	CTapiCall* pCall = GetSelectedCall();
	if (pCall)
	{
		// Make sure we are owner.
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ErrorMsg ("lineSetCallPrivilege failed, rc=0x%lx", lResult);
		else
		{
			lResult = GetTAPIConnection()->WaitForReply(pCall->Answer());
			if (lResult != 0)
				ErrorMsg("lineAnswer failed, rc=0x%lx", lResult);
		}
	}

}// CMonitorDlg::OnAnswer

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnDrop
//
// Drop an active call
//
void CMonitorDlg::OnDrop() 
{
	ClearErrors();

	CTapiCall* pCall = GetSelectedCall();
	if (pCall)
	{
		// Make sure we are owner.
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ErrorMsg ("lineSetCallPrivilege failed, rc=0x%lx", lResult);
		else
		{
			lResult = GetTAPIConnection()->WaitForReply(pCall->Drop());
			if (lResult != 0)
				ErrorMsg("lineDrop failed, rc=0x%lx", lResult);
		}
	}

}// CMonitorDlg::OnDrop

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnHold
//
// Place the call on hold
//
void CMonitorDlg::OnHold() 
{
	ClearErrors();

	CTapiCall* pCall = GetSelectedCall();
	if (pCall)
	{
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ErrorMsg ("lineSetCallPrivilege failed, rc=0x%lx", lResult);
		else
		{
			lResult = GetTAPIConnection()->WaitForReply(pCall->Hold());
			if (lResult != 0)
				ErrorMsg("lineHold failed, rc=0x%lx", lResult);
		}
	}

}// CMonitorDlg::OnHold

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnUnhold
//
// Take the call off hold
//
void CMonitorDlg::OnUnhold() 
{
	ClearErrors();

	CTapiCall* pCall = GetSelectedCall();
	if (pCall)
	{
		LONG lResult = pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
		if (lResult != 0)
			ErrorMsg ("lineSetCallPrivilege failed, rc=0x%lx", lResult);
		else
		{
			lResult = GetTAPIConnection()->WaitForReply(pCall->Unhold());
			if (lResult != 0)
				ErrorMsg("lineUnhold failed, rc=0x%lx", lResult);
		}
	}

}// CMonitorDlg::OnUnhold

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OpenAllLines
//
// Opens all the lines we can see.
//
void CMonitorDlg::OpenAllLines()
{
	for (DWORD dwLine = 0; dwLine < GetTAPIConnection()->GetLineDeviceCount(); dwLine++)
	{
		CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwLine);		
		DWORD dwMediaMode = 0xffffffff;
		const LPLINEDEVCAPS lpCaps = pLine->GetLineCaps();
		if (lpCaps != NULL)
		{
			dwMediaMode &= lpCaps->dwMediaModes;
			LONG lResult = pLine->Open (LINECALLPRIVILEGE_MONITOR, dwMediaMode);
			if (lResult == 0)
			{
				pLine->SetStatusMessages(LINEDEVSTATE_CONNECTED | LINEDEVSTATE_DISCONNECTED, 0L);

				// Now find all the active calls and add them to our list.
				CObList callList;
				if (pLine->GetNewCalls(callList) == 0)
				{
					for (POSITION pos = callList.GetHeadPosition(); pos != NULL;)
					{
						CTapiCall* pCall = (CTapiCall*) callList.GetNext(pos);
						if (pCall->GetCallState() != LINECALLSTATE_IDLE)
							OnNewCall(pCall);
					}
				}
			}
		}
	}

}// CMonitorDlg::OpenAllLines

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnOK
//
// Dismiss the dialog
//
void CMonitorDlg::OnOK() 
{
	CDialog::OnOK();

}// CMonitorDlg::OnOK

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnCallStateChange
//
// Our call has changed state
//
void CMonitorDlg::OnCallStateChange (HCALL hCall, DWORD dwState, DWORD /*dwStateDetail*/)
{
	for (int i = 0; i < m_lcCalls.GetItemCount(); i++)
	{
		CTapiCall* pCall = (CTapiCall*) m_lcCalls.GetItemData(i);
		if (pCall && pCall->GetCallHandle() == hCall)
		{
			// Transitioning to IDLE?
			if (dwState == LINECALLSTATE_IDLE)
			{
				pCall->Deallocate();
				m_lcCalls.DeleteItem(i);
			}
			else
			{
				m_lcCalls.RedrawItems (i, i);
				m_lcCalls.EnsureVisible(i, FALSE);
			}
		}
	}

	UpdateButtons();

}// CMonitorDlg::OnCallStateChange

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnCallerID
//
// Called when a call has CALLERID information
//
void CMonitorDlg::OnCallerID(HCALL hCall)
{
	for (int i = 0; i < m_lcCalls.GetItemCount(); i++)
	{
		CTapiCall* pCall = (CTapiCall*) m_lcCalls.GetItemData(i);
		if (pCall && pCall->GetCallHandle() == hCall)
		{
			m_lcCalls.RedrawItems (i, i);
			m_lcCalls.EnsureVisible(i, FALSE);
		}
	}

}// CMonitorDlg::OnCallerID

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnNewCall
//
// A new incoming call has been detected.
//
void CMonitorDlg::OnNewCall(CTapiCall* pCall)
{
	// Search the box for this call.
	for (int i = 0; i < m_lcCalls.GetItemCount(); i++)
	{
		CTapiCall* pCall2 = (CTapiCall*) m_lcCalls.GetItemData(i);
		if (pCall2 && pCall2->GetCallHandle() == pCall->GetCallHandle())
			return;
	}

	// Use the following LV_ITEM structure to add all
	// the item information.
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	lvItem.pszText = LPSTR_TEXTCALLBACK;
	lvItem.cchTextMax = 0;
	lvItem.iSubItem = 0;
	lvItem.lParam = (LONG) pCall;

	lvItem.iItem = m_lcCalls.GetItemCount();
	i = m_lcCalls.InsertItem (&lvItem);

	// Now set the other rows.
	for (int x = 1; x < TOTAL_CALL_COLUMNS; x++)
		m_lcCalls.SetItemText (i, x, LPSTR_TEXTCALLBACK);

}// CMonitorDlg::OnNewCall

/////////////////////////////////////////////////////////////////////////////
// CMyLine::OnCallStateChange
//
// A call on this line has changed state
//
void CMyLine::OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail, DWORD dwPrivilage)
{
	CTapiLine::OnCallStateChange (hCall, dwState, dwStateDetail, dwPrivilage);
	CMonitorDlg* pDlg = (CMonitorDlg*) AfxGetApp()->m_pMainWnd;
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
	if (dwCallInfoState == LINECALLINFOSTATE_CALLERID ||
		dwCallInfoState == LINECALLINFOSTATE_CALLEDID)
	{
		CMonitorDlg* pDlg = (CMonitorDlg*) AfxGetApp()->m_pMainWnd;
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
	CTapiLine::OnDeviceStateChange(dwState, dwDetail1, dwDetail2);

}// CMyLine::OnCallStateChange

/////////////////////////////////////////////////////////////////////////////
// CMyLine::OnNewCall
//
// Handle a new incoming call.
//
void CMyLine::OnNewCall (CTapiCall* pCall)
{
	CTapiLine::OnNewCall (pCall);
	CMonitorDlg* pDlg = (CMonitorDlg*) AfxGetApp()->m_pMainWnd;
	pDlg->OnNewCall (pCall);

}// CMyLine::OnNewCall

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnCallGetDispInfo
//
// Something on the call has changed
//
void CMonitorDlg::OnCallGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &pDispInfo->item;
	CTapiCall* pCall = (CTapiCall*) pItem->lParam;
	CTapiLine* pLine = pCall->GetLineOwner();
	static char szBuffer[80];

	// See if it wants text for one of the fields.
	if (pItem->mask & LVIF_TEXT)
	{
		switch (pItem->iSubItem)
		{
			case 0: // Line
				strcpy (szBuffer, pLine->GetLineName());
				pItem->pszText = szBuffer;
				break;
			case 1: // CallID
			{
				const LINECALLINFO* pCallInfo = pCall->GetCallInfo();
				if (pCallInfo != NULL)
					wsprintf (szBuffer, "0x%lx", pCall->GetCallInfo()->dwCallID);
				else
					strcpy (szBuffer, "0");
				pItem->pszText = szBuffer;
				break;
			}
			case 2: // State
				strcpy (szBuffer, pCall->GetCallStateString());
				pItem->pszText = szBuffer;
				break;
			case 3: // Caller ID
			{
				const LINECALLINFO* pCallInfo = pCall->GetCallInfo();
				if (pCallInfo != NULL)
				{
					if (pCallInfo->dwCallerIDFlags & LINECALLPARTYID_BLOCKED)
						strcpy (szBuffer, "BLOCKED (PRIVATE)");
					else if (pCallInfo->dwCallerIDFlags & LINECALLPARTYID_OUTOFAREA)
						strcpy (szBuffer, "OUT OF AREA");
					else 
					{
						wsprintf (szBuffer, "%s %s", 
							(pCallInfo->dwCallerIDFlags & LINECALLPARTYID_ADDRESS) ?
								pCall->GetCallerIDNumber() : "",
							(pCallInfo->dwCallerIDFlags & LINECALLPARTYID_NAME) ?
								pCall->GetCallerIDName() : "");
					}
				}
				else
					szBuffer[0] = '\0';
				pItem->pszText = szBuffer;
				break;
			}
			case 4: // Called ID
			{
				const LINECALLINFO* pCallInfo = pCall->GetCallInfo();
				if (pCallInfo != NULL)
				{
					if (pCallInfo->dwCalledIDFlags & LINECALLPARTYID_BLOCKED)
						strcpy (szBuffer, "BLOCKED (PRIVATE)");
					else if (pCallInfo->dwCalledIDFlags & LINECALLPARTYID_OUTOFAREA)
						strcpy (szBuffer, "OUT OF AREA");
					else 
					{
						wsprintf (szBuffer, "%s %s", 
							(pCallInfo->dwCalledIDFlags & LINECALLPARTYID_ADDRESS) ?
								pCall->GetCalledIDNumber() : "",
							(pCallInfo->dwCalledIDFlags & LINECALLPARTYID_NAME) ?
								pCall->GetCalledIDName() : "");
					}
				}
				else
					szBuffer[0] = '\0';
				pItem->pszText = szBuffer;
				break;
			}
		}
	}

	*pResult = 0;

}// CMonitorDlg::OnCallGetDispInfo

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::GetSelectedCall
//
// Return the selected call
//
CTapiCall* CMonitorDlg::GetSelectedCall()
{
	for (int i = 0; i < m_lcCalls.GetItemCount(); i++)
	{
		if (m_lcCalls.GetItemState(i, LVIS_SELECTED))
			return (CTapiCall*) m_lcCalls.GetItemData(i);
	}
	return NULL;

}// CMonitorDlg::GetSelectedCall

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnCallChange
//
// Call has changed in list
//
void CMonitorDlg::OnCallChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
	UpdateButtons();
	*pResult = 0;

}// CMonitorDlg::OnCallChange

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg::OnTimer
//
// Clear the error line after 10 seconds.
//
void CMonitorDlg::OnTimer(UINT) 
{
	if (g_dwLastError + 10000 < GetTickCount())
		ClearErrors();

}// CMonitorDlg::OnTimer
