// AGENTDLG.CPP - Dialog to run the AGENT code
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

#include "stdafx.h"
#include "AgentDlg.h"
#include "AddAgent.h"
#include "SetAgentGroup.h"
#include "SetAgentState.h"
#include "AgentProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UM_AGENTCHANGE (WM_USER + 100)

class CMyLine : public CTapiLine
{
	DECLARE_DYNCREATE (CMyLine)

// Overrides
protected:
	virtual void OnAgentStateChange (DWORD dwAddressID, DWORD dwFields, DWORD dwState) {
		CTapiLine::OnAgentStateChange(dwAddressID, dwFields, dwState);
		if (AfxGetApp()->m_pMainWnd != NULL)
			AfxGetApp()->m_pMainWnd->PostMessage(UM_AGENTCHANGE, (WPARAM)GetDeviceID(), (LPARAM)dwAddressID);
	}
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

} g_AgentColumns[] = {
	{ "Line",			15 },
	{ "Address",		15 },
	{ "Agent State",	15 },
	{ "Next State",		15 }
};
#define TOTAL_CALL_COLUMNS (sizeof(g_AgentColumns) / sizeof(g_AgentColumns[0]))

/////////////////////////////////////////////////////////////////////////////
// CMainApp::InitInstance
//
// Initialize the main application
//
BOOL CMainApp::InitInstance()
{
    // Initialize a connection with TAPI and determine if there 
    // are any TAPI complient devices installed.
    if (GetTAPIConnection()->Init ("AgentMonitor", RUNTIME_CLASS(CMyLine)) != 0 ||
        GetTAPIConnection()->GetLineDeviceCount() == 0)
    {
        AfxMessageBox ("There are no TAPI devices installed!");
		return FALSE;
    }

	// Create our dialog.
	CAgentDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	m_pMainWnd = NULL;

	return FALSE;

}// CMainApp::InitInstance

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg::CAgentDlg
//
// Constructor for the agent dialog
//
CAgentDlg::CAgentDlg(CWnd* pParent) : CDialog(CAgentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgentDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}// CAgentDlg::CAgentDlg

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg::DoDataExchange
//
// Dialog data exchange for the answer dialog
//
void CAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgentDlg)
	DDX_Control(pDX, IDC_SETAGENTSTATE, m_btnSetAgentState);
	DDX_Control(pDX, IDC_SETAGENTGROUP, m_btnSetAgentGroup);
	DDX_Control(pDX, IDC_SETAGENTACTIVITY, m_btnSetAgentActivity);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_AGENTS, m_lcAgents);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg message map

BEGIN_MESSAGE_MAP(CAgentDlg, CDialog)
	//{{AFX_MSG_MAP(CAgentDlg)
	ON_MESSAGE(UM_AGENTCHANGE, OnAgentStateChange)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD, OnAddAgent)
	ON_BN_CLICKED(IDC_REMOVE, OnRemoveAgent)
	ON_BN_CLICKED(IDC_SETAGENTACTIVITY, OnSetAgentActivity)
	ON_BN_CLICKED(IDC_SETAGENTGROUP, OnSetAgentGroup)
	ON_BN_CLICKED(IDC_SETAGENTSTATE, OnSetAgentState)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_AGENTS, OnAgentChange)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_AGENTS, OnGetDispInfo)
	ON_NOTIFY(NM_DBLCLK, IDC_AGENTS, OnDblclkAgents)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg::OnInitDialog
//
// Manage the creation of our dialog box.
//
BOOL CAgentDlg::OnInitDialog()
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
	CDC* pDC = m_lcAgents.GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	m_lcAgents.ReleaseDC(pDC);

	// Setup the TAPI call list
	for (int i = 0; i < TOTAL_CALL_COLUMNS; i++)
		m_lcAgents.InsertColumn (i, g_AgentColumns[i].pszName, LVCFMT_LEFT,
							tm.tmAveCharWidth*(g_AgentColumns[i].iWidth+1));

	// Setup the initial button states
	UpdateButtons();

	if (AfxGetApp()->GetProfileInt("JTAgent", "Reload", 0))
	{
		int iCount = AfxGetApp()->GetProfileInt("JTAgent", "ReloadCount", 0);
		for (int i = 0; i < iCount; i++)
		{
			CString strEntry, strAgent;
			strEntry.Format("Reload%d", i);
			strAgent = AfxGetApp()->GetProfileString("JTAgent", strEntry);
			DWORD dwAgent = (DWORD) atol(strAgent);
			AddAgentToList(dwAgent);
		}
	}
	return TRUE;

}// CAgentDlg::OnInitDialog

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg::OnPaint
//
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
//
void CAgentDlg::OnPaint() 
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
}// CAgentDlg::OnPaint

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg::OnQueryDragIcon
//
// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
//
HCURSOR CAgentDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;

}// CAgentDlg::OnQueryDragIcon

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg::UpdateButtons
//
// Enable/Disable the buttons based on our current states
//
void CAgentDlg::UpdateButtons()
{
	int iPos = GetSelectedAgent();
	m_btnRemove.EnableWindow(iPos  != -1);
	if (iPos >= 0)
	{
		DWORD_PTR dwIndex = m_lcAgents.GetItemData(iPos);
		CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(LOWORD(dwIndex));
		if (pLine != NULL)
		{
			LPLINEAGENTSTATUS lpStatus = pLine->GetAddress(HIWORD(dwIndex))->GetAgentStatus();
			if (lpStatus != NULL)
			{
				m_btnSetAgentState.EnableWindow((lpStatus->dwAgentFeatures & LINEAGENTFEATURE_SETAGENTSTATE) != 0);
				m_btnSetAgentGroup.EnableWindow((lpStatus->dwAgentFeatures & LINEAGENTFEATURE_SETAGENTGROUP) != 0);
				m_btnSetAgentActivity.EnableWindow((lpStatus->dwAgentFeatures & LINEAGENTFEATURE_SETAGENTACTIVITY) != 0);
				return;
			}
		}

		m_btnSetAgentState.EnableWindow(FALSE);
		m_btnSetAgentGroup.EnableWindow(FALSE);
		m_btnSetAgentActivity.EnableWindow(FALSE);
	}

}// CAgentDlg::UpdateButtons

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg::GetSelectedAgent
//
// Return the first selected agent
//
int CAgentDlg::GetSelectedAgent()
{
	for (int i = 0; i < m_lcAgents.GetItemCount(); i++)
	{
		if (m_lcAgents.GetItemState(i, LVIS_SELECTED))
			return i;
	}
	return -1;

}// CAgentDlg::GetSelectedAgent

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg::OnAddAgent
//
// Adds an agent (or more than one) to our list view.
//
void CAgentDlg::OnAddAgent() 
{
	m_btnAdd.EnableWindow(FALSE);

	CAddAgent dlg(this);
	dlg.DoModal();

	m_btnAdd.EnableWindow(TRUE);

}// CAgentDlg::OnAddAgent

void CAgentDlg::OnRemoveAgent() 
{
	int iPos = GetSelectedAgent();
	m_lcAgents.DeleteItem(iPos);
	if (m_lcAgents.GetItemCount() > 0)
		m_lcAgents.SetItemState(iPos, LVIS_SELECTED | LVIS_FOCUSED, 0);
}

void CAgentDlg::OnSetAgentActivity() 
{
	m_lcAgents.RedrawItems (0, m_lcAgents.GetItemCount());
}

void CAgentDlg::OnSetAgentGroup() 
{
	m_btnSetAgentGroup.EnableWindow(FALSE);

	CSetAgentGroup dlg(this);
	dlg.DoModal();

	m_btnSetAgentGroup.EnableWindow(TRUE);
	m_lcAgents.RedrawItems (0, m_lcAgents.GetItemCount());
}

void CAgentDlg::OnSetAgentState() 
{
	m_btnSetAgentState.EnableWindow(FALSE);
	CSetAgentState dlg(this);
	dlg.DoModal();
	m_btnSetAgentState.EnableWindow(TRUE);
	m_lcAgents.RedrawItems (0, m_lcAgents.GetItemCount());
}

int CAgentDlg::FindAgentByID(DWORD dwID)
{
	for (int i = 0; i < m_lcAgents.GetItemCount(); i++)
	{
		if (m_lcAgents.GetItemData(i) == dwID)
			return i;
	}			
	return -1;
}

void CAgentDlg::AddAgentToList(DWORD dwID)
{
	if (FindAgentByID(dwID) != -1)
		return;

    CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(LOWORD(dwID));
	if (pLine == NULL || pLine->IsValid() == FALSE)
		return;

	LONG lResult = pLine->Open (LINECALLPRIVILEGE_NONE, LINEMEDIAMODE_UNKNOWN);
	if (lResult == 0)
	{
		CString strLineName = pLine->GetLineName();
		
		// Use the following LV_ITEM structure to add all
		// the item information.
		LV_ITEM lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
		lvItem.state = 0;
		lvItem.stateMask = 0;
		lvItem.pszText = LPSTR_TEXTCALLBACK;
		lvItem.cchTextMax = 0;
		lvItem.iSubItem = 0;
		lvItem.lParam = (LONG) dwID;

		lvItem.iItem = m_lcAgents.GetItemCount();
		int i = m_lcAgents.InsertItem (&lvItem);

		// Now set the other rows.
		for (int x = 1; x < TOTAL_CALL_COLUMNS; x++)
			m_lcAgents.SetItemText (i, x, LPSTR_TEXTCALLBACK);
	}

	UpdateButtons();
}

void CAgentDlg::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &pDispInfo->item;
	DWORD dwIndex = (DWORD) pItem->lParam;
	
    CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(LOWORD(dwIndex));
	if (pLine == NULL)
	{
		m_lcAgents.DeleteItem(pItem->iItem);
		return;
	}

	LPLINEAGENTSTATUS lpStatus = pLine->GetAddress(HIWORD(dwIndex))->GetAgentStatus();
	if (lpStatus == NULL)
		return;

	static char szBuffer[255];

	// See if it wants text for one of the fields.
	if (pItem->mask & LVIF_TEXT)
	{
		switch (pItem->iSubItem)
		{
			case 0: // Line
				strcpy(szBuffer, pLine->GetLineName());
				pItem->pszText = szBuffer;
				break;

			case 1: // Address
				wsprintf (szBuffer, "0x%lx %s", HIWORD(dwIndex), pLine->GetAddress(HIWORD(dwIndex))->GetDialableAddress());
				pItem->pszText = szBuffer;
				break;

			case 2: // Agent State
			{
				wsprintf(szBuffer, "%s 0x%lx", GetAgentStateString(lpStatus->dwState), lpStatus->dwState);
				pItem->pszText = szBuffer;
				break;
			}

			case 3: // Agent Next State
			{
				wsprintf(szBuffer, "%s 0x%lx", GetAgentStateString(lpStatus->dwNextState), lpStatus->dwNextState);
				pItem->pszText = szBuffer;
				break;
			}
		}
	}

	*pResult = 0;

}// CAgentDlg::OnGetDispInfo

int CAgentDlg::GetAgentCount()
{
	return m_lcAgents.GetItemCount();

}// CAgentDlg::GetAgentCount

DWORD CAgentDlg::GetAgent(int iPos)
{
	if (iPos >= 0 && iPos < GetAgentCount())
		return static_cast<DWORD>(m_lcAgents.GetItemData(iPos));
	return static_cast<DWORD>(-1);

}// CAgentDlg::GetAgent

LRESULT CAgentDlg::OnAgentStateChange(WPARAM wParam, LPARAM lParam)
{
	int iPos = FindAgentByID(MAKELONG((WORD)wParam, (WORD)lParam));
	m_lcAgents.RedrawItems (iPos, iPos);
	return 0L;

}// CAgentDlg::OnAgentStateChange

void CAgentDlg::OnDblclkAgents(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	int iPos = GetSelectedAgent();
	if (iPos >= 0)
	{
		DWORD dwIndex = static_cast<DWORD>(m_lcAgents.GetItemData(iPos));
		CAgentProperties dlg(this, dwIndex);
		dlg.DoModal();
	}
	*pResult = 0;
}

void CAgentDlg::OnAgentChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
	UpdateButtons();
	*pResult = 0;

}// CAgentDlg::OnAgentChange

LPCSTR CAgentDlg::GetAgentStateString(DWORD dwAgentState)
{
	switch (dwAgentState)
	{
		case LINEAGENTSTATE_LOGGEDOFF:	return "Logged Off";
		case LINEAGENTSTATE_NOTREADY:	return "Not Ready"; 
		case LINEAGENTSTATE_READY:		return "Ready";
		case LINEAGENTSTATE_BUSYACD:	return "Busy ACD";
		case LINEAGENTSTATE_BUSYINCOMING: return "Busy Incoming"; 
		case LINEAGENTSTATE_BUSYOUTBOUND: return "Busy Outbound";
		case LINEAGENTSTATE_BUSYOTHER:	return "Busy Other";
		case LINEAGENTSTATE_WORKINGAFTERCALL: return "Call Work";
		case LINEAGENTSTATE_UNAVAIL: return "Unavailable";
	}
	return "Unknown";

}// CAgentDlg::GetAgentStateString


