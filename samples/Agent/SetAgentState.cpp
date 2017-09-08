// SetAgentState.cpp : implementation file
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
#include "SetAgentState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetAgentState dialog


CSetAgentState::CSetAgentState(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAgentState::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetAgentState)
	m_strNextState = _T("");
	m_strState = _T("");
	//}}AFX_DATA_INIT
}


void CSetAgentState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetAgentState)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_AGENTS, m_cbAgent);
	DDX_Control(pDX, IDC_NEXTSTATE, m_cbNextState);
	DDX_Control(pDX, IDC_STATE, m_cbState);
	DDX_Text(pDX, IDC_CURRNEXTSTATE, m_strNextState);
	DDX_Text(pDX, IDC_CURRSTATE, m_strState);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetAgentState, CDialog)
	//{{AFX_MSG_MAP(CSetAgentState)
	ON_CBN_SELCHANGE(IDC_AGENTS, OnChangeAgent)
	ON_CBN_SELCHANGE(IDC_NEXTSTATE, OnChange)
	ON_CBN_SELCHANGE(IDC_STATE, OnChangeState)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetAgentState message handlers

BOOL CSetAgentState::OnInitDialog() 
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

	// Load all the agents from our parent dialog.
	DWORD dwSelected = AgentDlg()->GetAgent(AgentDlg()->GetSelectedAgent());
	for (int i = 0; i < AgentDlg()->GetAgentCount(); i++)
	{
		DWORD dwIndex = AgentDlg()->GetAgent(i);
		CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(LOWORD(dwIndex));
		if (pLine != NULL)
		{
			CString strLineName = pLine->GetLineName();
			CString strAddress = pLine->GetAddress(HIWORD(dwIndex))->GetDialableAddress();
			CString strName = strLineName + CString(":") + strAddress;

			int iPos = m_cbAgent.AddString(strName);
			ASSERT (iPos != CB_ERR);
			m_cbAgent.SetItemData(iPos, dwIndex);

			if (dwSelected == dwIndex)
				m_cbAgent.SetCurSel(iPos);
		}
	}

	// Load our states and settings.
	OnChangeAgent();

	return TRUE;
}

void CSetAgentState::OnChangeAgent() 
{
	DWORD_PTR dwIndex = m_cbAgent.GetItemData(m_cbAgent.GetCurSel());
	CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(LOWORD(dwIndex));
	CTapiAddress* pAddr = pLine->GetAddress(HIWORD(dwIndex));

	// Delete all our state/next state information
	m_cbState.ResetContent();
	m_cbNextState.ResetContent();

	// Get our agent status.
	LPLINEAGENTSTATUS lpStatus = pAddr->GetAgentStatus();
	if (lpStatus != NULL)
	{
		m_strState = AgentDlg()->GetAgentStateString(lpStatus->dwState);
		m_strNextState = AgentDlg()->GetAgentStateString(lpStatus->dwNextState);

		// Load the valid states
		if (lpStatus->dwValidStates != 0)
		{
			int i = 1;
			do
			{
				if (lpStatus->dwValidStates & i)
				{
					int iPos = m_cbState.AddString(AgentDlg()->GetAgentStateString(i));
					ASSERT (iPos != CB_ERR);
					m_cbState.SetItemData(iPos, (DWORD)i);
				}
				i = i << 1;
			} while (i <= LINEAGENTSTATE_UNAVAIL);
		}
		else
		{
			int i = 1;
			do
			{
				int iPos = m_cbState.AddString(AgentDlg()->GetAgentStateString(i));
				ASSERT (iPos != CB_ERR);
				m_cbState.SetItemData(iPos, (DWORD)i);
				i = i << 1;
			} while (i <= LINEAGENTSTATE_UNAVAIL);
		}

		if (lpStatus->dwValidNextStates != 0)
		{
			lpStatus->dwValidNextStates |= LINEAGENTSTATE_UNKNOWN;

			int i = 1;
			do
			{
				if (lpStatus->dwValidNextStates & i)
				{
					int iPos = m_cbNextState.AddString(AgentDlg()->GetAgentStateString(i));
					ASSERT (iPos != CB_ERR);
					m_cbNextState.SetItemData(iPos, (DWORD)i);
				}
				i = i << 1;
			} while (i <= LINEAGENTSTATE_UNAVAIL);
		}
		else
		{
			int i = 1;
			do
			{
				int iPos = m_cbNextState.AddString(AgentDlg()->GetAgentStateString(i));
				ASSERT (iPos != CB_ERR);
				m_cbNextState.SetItemData(iPos, (DWORD)i);
				i = i << 1;
			} while (i <= LINEAGENTSTATE_UNAVAIL);
		}
	}
	else
	{
		m_strState = "";
		m_strNextState = "";
	}

	m_cbState.EnableWindow(m_cbState.GetCount() > 0);
	m_cbNextState.EnableWindow(m_cbNextState.GetCount() > 0);

	UpdateData(FALSE);
}

void CSetAgentState::OnChange() 
{
	m_btnOK.EnableWindow(TRUE);
}

void CSetAgentState::OnOK() 
{
	m_btnOK.EnableWindow(FALSE);

	DWORD_PTR dwIndex = m_cbAgent.GetItemData(m_cbAgent.GetCurSel());
	CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(LOWORD(dwIndex));
	CTapiAddress* pAddr = pLine->GetAddress(HIWORD(dwIndex));

	// Get the currently selected state
	LPLINEAGENTSTATUS lpStatus = pAddr->GetAgentStatus();
	if (lpStatus != NULL)
	{
		DWORD dwState = lpStatus->dwState;
		DWORD dwNextState = LINEAGENTSTATE_UNKNOWN;

		if (m_cbState.GetCurSel() != CB_ERR)
			dwState = static_cast<DWORD>(m_cbState.GetItemData(m_cbState.GetCurSel()));
		if (m_cbNextState.GetCurSel() != CB_ERR)
			dwNextState = static_cast<DWORD>(m_cbNextState.GetItemData(m_cbNextState.GetCurSel()));

		LONG lResult = GetTAPIConnection()->WaitForReply(pAddr->SetAgentState(dwState, dwNextState));
		if (IsTapiError(lResult))
			DisplayError("lineSetAgentState", lResult);
		OnChangeAgent();
	}

	CDialog::OnOK();
}

void CSetAgentState::OnChangeState() 
{
	OnChange();
	
	int iCurSel = m_cbState.GetCurSel();
	if (iCurSel == CB_ERR)
		return;

	DWORD_PTR dwState = m_cbState.GetItemData(iCurSel);
	DWORD dwNextState = LINEAGENTSTATE_UNKNOWN;

	if (dwState == LINEAGENTSTATE_READY)
		dwNextState = LINEAGENTSTATE_BUSYACD;

	else if (dwState & (LINEAGENTSTATE_BUSYACD |
		LINEAGENTSTATE_BUSYINCOMING |
		LINEAGENTSTATE_BUSYOUTBOUND |
		LINEAGENTSTATE_BUSYOTHER))
		dwNextState = LINEAGENTSTATE_WORKINGAFTERCALL;

	else if (dwState == LINEAGENTSTATE_WORKINGAFTERCALL)
		dwNextState = LINEAGENTSTATE_READY;

	m_cbNextState.SetCurSel(CB_ERR);
	for (int i = 0; i < m_cbNextState.GetCount(); i++)
	{
		if (m_cbNextState.GetItemData(i) == LINEAGENTSTATE_UNKNOWN)
			iCurSel = i;

		if (m_cbNextState.GetItemData(i) == dwNextState)
		{
			m_cbNextState.SetCurSel(i);
			break;
		}
	}

	if (m_cbNextState.GetCurSel() == CB_ERR)
		m_cbNextState.SetCurSel(iCurSel);
}
