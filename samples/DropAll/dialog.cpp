// DIALOG.CPP - Dialog to run the lineDrop example
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
    virtual void OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail, DWORD dwPrivilage);
	virtual void OnNewCall (CTapiCall* pCall);
};

IMPLEMENT_DYNCREATE (CMyLine, CTapiLine)

/////////////////////////////////////////////////////////////////////////////
// Globals

CMainApp theApp;
#define IDT_TIMER 1

// States we allow drop on
struct
{
	const char* szName;
	DWORD dwState;
} g_pszStates[] = {
	{ "Offering", LINECALLSTATE_OFFERING },
	{ "Accepted", LINECALLSTATE_ACCEPTED },
	{ "Dialtone", LINECALLSTATE_DIALTONE },
	{ "Dialing", LINECALLSTATE_DIALING },
	{ "Proceeding", LINECALLSTATE_PROCEEDING },
	{ "Ringback", LINECALLSTATE_RINGBACK },
	{ "Connected", LINECALLSTATE_CONNECTED },
	{ "Conferenced", LINECALLSTATE_CONFERENCED },
	{ "OnHold", LINECALLSTATE_ONHOLD },
	{ "Busy", LINECALLSTATE_BUSY },
	{ NULL , 0 },
};

/////////////////////////////////////////////////////////////////////////////
// CMainApp::InitInstance
//
// Initialize the main application
//
BOOL CMainApp::InitInstance()
{
    // Initialize a connection with TAPI and determine if there 
    // are any TAPI complient devices installed.
    if (GetTAPIConnection()->Init ("DropCalls", RUNTIME_CLASS(CMyLine)) != 0 ||
        GetTAPIConnection()->GetLineDeviceCount() == 0)
    {
        AfxMessageBox ("There are no TAPI devices installed!");
		return FALSE;
    }

	// Create our dialog.
    CDropDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;

}// CMainApp::InitInstance

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::CDropDlg
//
// Constructor for the MAKEDROP dialog
//
CDropDlg::CDropDlg(CWnd* pParent) : CDialog(CDropDlg::IDD, pParent)
{
        //{{AFX_DATA_INIT(CDropDlg)
	m_pLine = NULL;
	m_pCall = NULL;
	m_fRunning = false;
	m_fAccept = FALSE;
	m_fAnswer = FALSE;
	//}}AFX_DATA_INIT

}// CDropDlg::CDropDlg

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::~CDropDlg
//
// Destructor for the MAKEDROP dialog
//
CDropDlg::~CDropDlg()
{
	if (m_pLine)
		m_pLine->Close();

}// CDropDlg::~CDropDlg

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::DoDataExchange
//
// Dialog data exchange for the MAKEDROP dialog
//
void CDropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDropDlg)
	DDX_Control(pDX, IDC_STATE, m_cbCallState);
	DDX_Control(pDX, IDC_DROP, m_btnStart);
	DDX_Control(pDX, IDC_STATUS, m_lbStatus);
	DDX_Control(pDX, IDC_LINES, m_cbLines);
	DDX_Check(pDX, IDC_ACCEPT, m_fAccept);
	DDX_Check(pDX, IDC_ANSWER, m_fAnswer);
	//}}AFX_DATA_MAP

}// CDropDlg::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CDropDlg message map

BEGIN_MESSAGE_MAP(CDropDlg, CDialog)
    //{{AFX_MSG_MAP(CDropDlg)
	ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_DROP, OnBegin)
	ON_CBN_SELCHANGE(IDC_LINES, OnSelchangeLines)
	ON_BN_CLICKED(IDC_ACCEPT, OnCheckButton)
	ON_BN_CLICKED(IDC_ANSWER, OnCheckButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::OnInitDialog
//
// Manage the creation of our dialog box.
//
BOOL CDropDlg::OnInitDialog()
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

	// Fill in our states
	for (int i = 0; g_pszStates[i].szName != NULL; i++)
	{
		int iPos = m_cbCallState.AddString(g_pszStates[i].szName);
		ASSERT (iPos != CB_ERR);
		m_cbCallState.SetItemData(iPos, g_pszStates[i].dwState);
	}

	m_cbCallState.SetCurSel(0);

    // Now go through and enumerate all the line devices in the system.
    // These get placed into our combo box.
    for (DWORD dwCount = 0; dwCount < GetTAPIConnection()->GetLineDeviceCount(); dwCount++)
    {
        CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwCount);
        if (pLine)
        {
			int iPos = m_cbLines.AddString (pLine->GetLineName());
			ASSERT (iPos >= 0);
			m_cbLines.SetItemDataPtr(iPos, pLine);
        }
    }
    
    // Automatically select the first entry in the combo box.
    if (m_cbLines.GetCount() > 0)
    {
        m_cbLines.SetCurSel(0);
        OnSelchangeLines();
    }        

	return TRUE;

}// CDropDlg::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::OnBegin
//
// MAKEDROP an offering call
//
void CDropDlg::OnBegin() 
{
	if (m_fRunning)
	{
		m_fRunning = false;
		m_btnStart.SetWindowText("&Start");
		if (m_pLine != NULL)
		{
			TraceMsg ("Line %ld closed, rc=0x%lx", m_pLine->GetDeviceID(), m_pLine->Close());
			m_pLine = NULL;
		}
		return;
	}

	int iCurSel = m_cbLines.GetCurSel();
	if (iCurSel >= 0)
	{
		DWORD dwType = 0xffffffff;
		m_pLine = (CTapiLine*) m_cbLines.GetItemDataPtr(iCurSel);
		if (m_pLine != NULL)
		{
			LPLINEDEVCAPS lpCaps = m_pLine->GetLineCaps();
			if (lpCaps != NULL)
				dwType &= lpCaps->dwMediaModes;

			LONG lResult = m_pLine->Open(LINECALLPRIVILEGE_MONITOR, dwType);
			TraceMsg ("Line %ld opened (MONITOR), rc=0x%lx", m_pLine->GetDeviceID(), lResult);
			if (lResult != 0)
				m_pLine = NULL;
			else
			{
				m_btnStart.SetWindowText("&Stop");
				m_fRunning = true;
			}
		}
	}

}// CDropDlg::OnBegin

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::OnSelchangeLines
//
// Change the currently selected line
//
void CDropDlg::OnSelchangeLines() 
{
	if (m_fRunning)
	{
		m_btnStart.SetWindowText("&Start");
		m_fRunning = false;
	}

	if (m_pLine != NULL)
	{
		TraceMsg ("Line %ld closed, rc=0x%lx", m_pLine->GetDeviceID(), m_pLine->Close());
		m_pLine = NULL;
	}

}// CDropDlg::OnSelchangeLines

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::OnClose
//
// Dismiss the dialog
//
void CDropDlg::OnClose() 
{
	if (m_pLine)
		m_pLine->Close();
	m_pLine = NULL;
	EndDialog(IDOK);

}// CDropDlg::OnClose

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::OnOK
//
// Ignore
//
void CDropDlg::OnOK()
{
}// CDropDlg::OnOK

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::OnCancel
//
// Ignore
//
void CDropDlg::OnCancel()
{
}// CDropDlg::OnCancel

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::TraceMsg
//
// Function to place a string in our status listbox.
//
void CDropDlg::TraceMsg (const char * pszStr, ...)
{
   va_list args;
   static char szBuff[1024];
   static int g_iItemsPerBox;
  
   va_start(args, pszStr);
   vsprintf(szBuff, pszStr, args);
   va_end(args);

   if (g_iItemsPerBox == 0)
   {
	   CRect rcBox;
	   m_lbStatus.GetClientRect(&rcBox);
	   g_iItemsPerBox = (rcBox.Height() / m_lbStatus.GetItemHeight(0));
   }

   m_lbStatus.SetRedraw(FALSE);
   int iPos = m_lbStatus.AddString (szBuff);
   if (iPos >= 500)
	   m_lbStatus.DeleteString(0);
   
   if (iPos - m_lbStatus.GetTopIndex() > g_iItemsPerBox)
	   m_lbStatus.SetTopIndex(iPos - g_iItemsPerBox);

   m_lbStatus.SetRedraw(TRUE);
   m_lbStatus.Invalidate();
   m_lbStatus.UpdateWindow();

}// CDropDlg::TraceMsg

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::OnCheckButton
//
// Called when the accept/answer button is clicked
//
void CDropDlg::OnCheckButton()
{
	UpdateData(TRUE);

}// CDropDlg::OnCheckButton

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::OnCallStateChange
//
// Our call has changed state
//
void CDropDlg::OnCallStateChange (HCALL hCall, DWORD dwState, DWORD /*dwStateDetail*/)
{
	if (m_pLine == NULL)
	{
		if (dwState != LINECALLSTATE_IDLE)
			lineDrop(hCall, NULL, 0);
		lineDeallocateCall(hCall);
		return;
	}

	// Find the call by this handle.
	CTapiCall* pCall = m_pLine->GetCallFromHandle(hCall);
	if (pCall != NULL)
	{
		// Otherwise, report a status change.
		TraceMsg ("Call 0x%lx <%s>", pCall->GetCallHandle(), pCall->GetCallStateString());
		if (dwState == LINECALLSTATE_IDLE)
		{
			TraceMsg ("Deallocating call 0x%lx", pCall->GetCallHandle());
			pCall->Deallocate();
		}
		else if (dwState == m_cbCallState.GetItemData(m_cbCallState.GetCurSel()))
		{
			TraceMsg ("lineSetCallPrivilege call 0x%lx, rc=%lx", pCall->GetCallHandle(), pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER));
			TraceMsg ("lineDrop call 0x%lx, rc=0x%lx", pCall->GetCallHandle(), pCall->Drop());
		}
		else if (dwState == LINECALLSTATE_OFFERING && m_fAccept)
		{
			TraceMsg ("lineSetCallPrivilege call 0x%lx, rc=%lx", pCall->GetCallHandle(), pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER));
			TraceMsg ("lineAccept call 0x%lx, rc=0x%lx", pCall->GetCallHandle(), pCall->Accept());			
		}
		else if (m_fAnswer && 
			(dwState == LINECALLSTATE_OFFERING && !m_fAccept) ||
			(dwState == LINECALLSTATE_ACCEPTED))
		{
			TraceMsg ("lineSetCallPrivilege call 0x%lx, rc=%lx", pCall->GetCallHandle(), pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER));
			TraceMsg ("lineAnswer call 0x%lx, rc=0x%lx", pCall->GetCallHandle(), pCall->Answer());			
		}
	}
	else
	{
		if (dwState != LINECALLSTATE_IDLE)
			lineDrop(hCall, NULL, 0);
		lineDeallocateCall(hCall);
	}

}// CDropDlg::OnCallStateChange

/////////////////////////////////////////////////////////////////////////////
// CDropDlg::OnNewCall
//
// New call reported from TAPI
//
void CDropDlg::OnNewCall (CTapiCall* pCall)
{
	TraceMsg("New call (0x%lx) detected", pCall->GetCallHandle());

}// CDropDlg::OnNewCall

/////////////////////////////////////////////////////////////////////////////
// CMyLine::OnCallStateChange
//
// A call on this line has changed state
//
void CMyLine::OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail, DWORD dwPrivilage)
{
	CTapiLine::OnCallStateChange (hCall, dwState, dwStateDetail, dwPrivilage);
    CDropDlg* pDlg = (CDropDlg*) AfxGetApp()->m_pMainWnd;
	pDlg->OnCallStateChange (hCall, dwState, dwStateDetail);

}// CMyLine::OnCallStateChange

/////////////////////////////////////////////////////////////////////////////
// CMyLine::OnNewCall
//
// Handle a new incoming call - immediately drop it.
//
void CMyLine::OnNewCall (CTapiCall* pCall)
{
	CTapiLine::OnNewCall (pCall);
    CDropDlg* pDlg = (CDropDlg*) AfxGetApp()->m_pMainWnd;
	pDlg->OnNewCall (pCall);

}// CMyLine::OnNewCall

