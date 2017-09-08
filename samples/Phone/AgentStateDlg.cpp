// AgentStateDlg.cpp : implementation file
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
#include "phone.h"
#include "AgentStateDlg.h"
#include "PhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAgentStateDlg dialog

CAgentStateDlg::CAgentStateDlg(CWnd* pParent, CTapiAddress* pAddr)
	: CDialog(CAgentStateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgentStateDlg)
	m_strCurrentState = _T("");
	m_strGroup = _T("");
	m_strGroup2 = _T("");
	m_strNextState = _T("");
	m_pAddr = pAddr;
	//}}AFX_DATA_INIT

	// Create the free-floating window.
	Create(CAgentStateDlg::IDD, NULL);
}

void CAgentStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgentStateDlg)
	DDX_Control(pDX, IDC_ACTIVITY, m_cbActivity);
	DDX_Control(pDX, IDC_GROUP2, m_edtGroup2);
	DDX_Control(pDX, IDC_GROUP, m_edtGroup);
	DDX_Control(pDX, IDC_WORKINGAFTERCALL, m_btnCallWork);
	DDX_Control(pDX, IDC_SETGROUP, m_btnSetGroup);
	DDX_Control(pDX, IDC_READY, m_btnReady);
	DDX_Control(pDX, IDC_NOTREADY, m_btnNotReady);
	DDX_Control(pDX, IDC_LOGOUT, m_btnLogout);
	DDX_Control(pDX, IDC_GROUPLIST, m_lbGroups);
	DDX_Text(pDX, IDC_CURRSTATE, m_strCurrentState);
	DDX_Text(pDX, IDC_GROUP, m_strGroup);
	DDX_Text(pDX, IDC_GROUP2, m_strGroup2);
	DDX_Text(pDX, IDC_CURRNEXTSTATE, m_strNextState);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAgentStateDlg, CDialog)
	//{{AFX_MSG_MAP(CAgentStateDlg)
	ON_BN_CLICKED(IDC_LOGOUT, OnLogout)
	ON_BN_CLICKED(IDC_NOTREADY, OnNotready)
	ON_BN_CLICKED(IDC_READY, OnReady)
	ON_BN_CLICKED(IDC_SETGROUP, OnSetGroup)
	ON_BN_CLICKED(IDC_WORKINGAFTERCALL, OnCallWork)
	ON_MESSAGE(UM_AGENTCHANGE, 	i_OnAgentChange)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_UPDATE, OnUpdateS)
	ON_CBN_SELCHANGE(IDC_ACTIVITY, OnSelchangeActivity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAgentStateDlg::OnAvailableGroupsChange()
{
	// Delete all the information from our edit/listbox.
	for (int i = 0; i < m_lbGroups.GetCount(); i++)
		FreeMem((LPAGENTGROUP)m_lbGroups.GetItemData(i));
	m_lbGroups.ResetContent();
	m_strGroup = "";
	m_strGroup2 = "";

	// First, grab all the group entries for this address
	CPtrArray arrGroups;
	if (m_pAddr->GetAgentGroupList(arrGroups) == 0 && arrGroups.GetSize() > 0)
	{
		while (arrGroups.GetSize() > 0)
		{
			LPAGENTGROUP pGroup = (LPAGENTGROUP) arrGroups[0];
			arrGroups.RemoveAt(0);

			CString strName(pGroup->strName);
			if (strName.IsEmpty())
				strName.Format("0x%lx 0x%lx 0x%lx 0x%lx", 
						pGroup->GroupID.dwGroupID1,
						pGroup->GroupID.dwGroupID2,
						pGroup->GroupID.dwGroupID3,
						pGroup->GroupID.dwGroupID4);
			int iPos = m_lbGroups.AddString(strName);
			ASSERT (iPos != LB_ERR);
			m_lbGroups.SetItemDataPtr(iPos, pGroup);
		}
	}

	else
		m_lbGroups.EnableWindow(FALSE);

	UpdateData(FALSE);
		
}// CAgentStateDlg::OnAvailableGroupsChange

void CAgentStateDlg::OnAgentStateChange()
{
	// Get the agent status and select the groups the agent belongs to.
	CPtrArray arrGroups;
	if (m_pAddr->GetCurrentAgentGroupList(arrGroups) == 0 && arrGroups.GetSize() > 0)
	{
		if (m_lbGroups.GetCount() == 0)
		{
			LPAGENTGROUP pGroup = (LPAGENTGROUP) arrGroups[0];
			m_strGroup = pGroup->strName;
			if (m_strGroup.IsEmpty())
				m_strGroup.Format("0x%lx 0x%lx 0x%lx 0x%lx", 
						pGroup->GroupID.dwGroupID1,
						pGroup->GroupID.dwGroupID2,
						pGroup->GroupID.dwGroupID3,
						pGroup->GroupID.dwGroupID4);

			m_strGroup2 = _T("");
			if (arrGroups.GetSize() > 1)
			{
				pGroup = (LPAGENTGROUP) arrGroups[1];
				m_strGroup2 = pGroup->strName;
				if (m_strGroup2.IsEmpty())
					m_strGroup2.Format("0x%lx 0x%lx 0x%lx 0x%lx", 
							pGroup->GroupID.dwGroupID1,
							pGroup->GroupID.dwGroupID2,
							pGroup->GroupID.dwGroupID3,
							pGroup->GroupID.dwGroupID4);
			}
		}

		int iEditUsed = 0;
		while (arrGroups.GetSize() > 0)
		{
			LPAGENTGROUP pGroup = (LPAGENTGROUP) arrGroups[0];
			arrGroups.RemoveAt(0);
			for (int i = 0; i < m_lbGroups.GetCount(); i++)
			{
				LPAGENTGROUP pGroup2 = (LPAGENTGROUP) m_lbGroups.GetItemData(i);
				if (!memcmp(&pGroup->GroupID, &pGroup2->GroupID, sizeof(pGroup->GroupID)))
					m_lbGroups.SetSel(i, TRUE);
			}

			if (i == m_lbGroups.GetCount() && iEditUsed < 2)
			{
				if (iEditUsed == 0)
				{
					m_strGroup = pGroup->strName;
					if (m_strGroup.IsEmpty())
						m_strGroup.Format("0x%lx 0x%lx 0x%lx 0x%lx", 
								pGroup->GroupID.dwGroupID1,
								pGroup->GroupID.dwGroupID2,
								pGroup->GroupID.dwGroupID3,
								pGroup->GroupID.dwGroupID4);
				}
				else if (iEditUsed == 1)
				{
					m_strGroup2 = pGroup->strName;
					if (m_strGroup2.IsEmpty())
						m_strGroup2.Format("0x%lx 0x%lx 0x%lx 0x%lx", 
								pGroup->GroupID.dwGroupID1,
								pGroup->GroupID.dwGroupID2,
								pGroup->GroupID.dwGroupID3,
								pGroup->GroupID.dwGroupID4);
				}
				iEditUsed++;
			}
			delete pGroup;
		}
	}

	// Get our agent status.
	LPLINEAGENTSTATUS lpStatus = m_pAddr->GetAgentStatus();
	if (lpStatus != NULL)
	{
		m_strCurrentState = GetAgentStateString(lpStatus->dwState);
		m_strNextState = GetAgentStateString(lpStatus->dwNextState);

		for (int i = 0; i < m_cbActivity.GetCount(); i++)
		{
			if (m_cbActivity.GetItemData(i) == lpStatus->dwActivityID)
			{
				if (i != m_cbActivity.GetCurSel())
					m_cbActivity.SetCurSel(i);
			}
		}
	}

	UpdateData(FALSE);
	RecalcFeatures();

}// CAgentStateDlg::OnAgentStateChange

/////////////////////////////////////////////////////////////////////////////
// CAgentStateDlg message handlers

BOOL CAgentStateDlg::OnInitDialog() 
{
	// Change our title.
	CString strTitle, strBuff;
	GetWindowText(strTitle);
	strBuff.Format("%s - %s %s", strTitle, m_pAddr->GetLineOwner()->GetLineName(), m_pAddr->GetDialableAddress());
	SetWindowText(strBuff);	

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

	OnAvailableActivitiesChange();
	OnAvailableGroupsChange();
	OnAgentStateChange();

	UpdateData(FALSE);

	return TRUE;
}

void CAgentStateDlg::RecalcFeatures()
{
	if (!IsWindow(GetSafeHwnd()))
		return;

	LPLINEAGENTSTATUS lpStatus = m_pAddr->GetAgentStatus();
	if (lpStatus == NULL)
	{
		m_btnReady.EnableWindow(FALSE);
		m_btnNotReady.EnableWindow(FALSE);
		m_btnLogout.EnableWindow(FALSE);
		m_btnCallWork.EnableWindow(FALSE);
		m_btnSetGroup.EnableWindow(FALSE);
		return;
	}

	if (lpStatus->dwAgentFeatures & LINEAGENTFEATURE_SETAGENTSTATE)
	{
		DWORD dwStates = (lpStatus->dwValidStates | lpStatus->dwValidNextStates);
		if (dwStates > 0)
		{
			m_btnReady.EnableWindow((dwStates & LINEAGENTSTATE_READY) > 0);
			m_btnNotReady.EnableWindow((dwStates & LINEAGENTSTATE_NOTREADY) > 0);
			m_btnLogout.EnableWindow((dwStates & LINEAGENTSTATE_LOGGEDOFF) > 0);
			m_btnCallWork.EnableWindow((dwStates & LINEAGENTSTATE_WORKINGAFTERCALL) > 0);
		}
		else
		{
			m_btnReady.EnableWindow(TRUE);
			m_btnNotReady.EnableWindow(TRUE);
			m_btnLogout.EnableWindow(TRUE);
			m_btnCallWork.EnableWindow(TRUE);
		}
	}
	else
	{
			m_btnReady.EnableWindow(FALSE);
			m_btnNotReady.EnableWindow(FALSE);
			m_btnLogout.EnableWindow(FALSE);
			m_btnCallWork.EnableWindow(FALSE);
	}

	m_btnSetGroup.EnableWindow((lpStatus->dwAgentFeatures & LINEAGENTFEATURE_SETAGENTGROUP) > 0);
	if ((lpStatus->dwAgentFeatures & LINEAGENTFEATURE_SETAGENTGROUP) == 0)
	{
		m_edtGroup.EnableWindow(FALSE);
		m_edtGroup2.EnableWindow(FALSE);
		m_lbGroups.EnableWindow(FALSE);
	}
	else
	{
		m_edtGroup.EnableWindow(TRUE);
		m_edtGroup2.EnableWindow(TRUE);
		if (m_lbGroups.GetCount() > 0)
			m_lbGroups.EnableWindow(TRUE);
		else
			m_lbGroups.EnableWindow(FALSE);
	}

	if ((lpStatus->dwAgentFeatures & LINEAGENTFEATURE_SETAGENTACTIVITY) == 0 ||
		m_cbActivity.GetCount() == 0)
	{
		m_cbActivity.EnableWindow(FALSE);
	}
	else
	{
		m_cbActivity.EnableWindow(TRUE);
	}

}// CAgentStateDlg::RecalcFeatures

void CAgentStateDlg::OnSetGroup() 
{
	ClearErrors();

	UpdateData(TRUE);

	// If we have a listbox, then use the selected groups.
	AGENTGROUP ag1, ag2;
	CPtrArray arrGroups;

	if (m_strGroup.IsEmpty() == FALSE)
	{
		ag1.GroupID.dwGroupID1 = 
		ag1.GroupID.dwGroupID2 = 
		ag1.GroupID.dwGroupID3 = 
		ag1.GroupID.dwGroupID4 = 0;
		ag1.strName = m_strGroup;
		arrGroups.Add(&ag1);
	}

	if (m_strGroup2.IsEmpty() == FALSE)
	{
		ag2.GroupID.dwGroupID1 = 
		ag2.GroupID.dwGroupID2 = 
		ag2.GroupID.dwGroupID3 = 
		ag2.GroupID.dwGroupID4 = 0;
		ag2.strName = m_strGroup2;
		ag2.strName.TrimRight();
		arrGroups.Add(&ag2);
	}

	if (m_lbGroups.IsWindowEnabled())
	{
		for (int i = 0; i < m_lbGroups.GetCount(); i++)
		{
			LPAGENTGROUP pGroup = (LPAGENTGROUP) m_lbGroups.GetItemData(i);
			if (m_lbGroups.GetSel(i))
				arrGroups.Add(pGroup);
		}
	}

	LONG lResult = GetTAPIConnection()->WaitForReply(m_pAddr->SetAgentGroup(arrGroups));
	if (lResult != 0)
		ShowError("lineSetAgentGroup", lResult);
	OnAgentStateChange();
}

void CAgentStateDlg::OnLogout() 
{
	ClearErrors();

	LPLINEAGENTSTATUS lpStatus = m_pAddr->GetAgentStatus();
	if (lpStatus == NULL)
		return;

	DWORD dwState = LINEAGENTSTATE_LOGGEDOFF, dwNextState = 0;
	if ((lpStatus->dwValidStates | lpStatus->dwValidNextStates) > 0)
	{
		if ((lpStatus->dwValidStates & dwState) == 0)
		{
			dwNextState = dwState;
			dwState = 0;
		}
	}

	LONG lResult = GetTAPIConnection()->WaitForReply(
		m_pAddr->SetAgentState(dwState, dwNextState));
	if (lResult != 0)
		ShowError("lineSetAgentState", lResult);
	OnAgentStateChange();
}

void CAgentStateDlg::OnNotready() 
{
	ClearErrors();

	LPLINEAGENTSTATUS lpStatus = m_pAddr->GetAgentStatus();
	if (lpStatus == NULL)
		return;

	DWORD dwState = LINEAGENTSTATE_NOTREADY, dwNextState = 0;
	if ((lpStatus->dwValidStates | lpStatus->dwValidNextStates) > 0)
	{
		if ((lpStatus->dwValidStates & dwState) == 0)
		{
			dwNextState = dwState;
			dwState = 0;
		}
	}

	LONG lResult = GetTAPIConnection()->WaitForReply(
		m_pAddr->SetAgentState(dwState, dwNextState));
	if (lResult != 0)
		ShowError("lineSetAgentState", lResult);
	OnAgentStateChange();
}

void CAgentStateDlg::OnReady() 
{
	ClearErrors();

	LPLINEAGENTSTATUS lpStatus = m_pAddr->GetAgentStatus();
	if (lpStatus == NULL)
		return;

	DWORD dwState = LINEAGENTSTATE_READY, dwNextState = 0;
	if ((lpStatus->dwValidStates | lpStatus->dwValidNextStates) > 0)
	{
		if ((lpStatus->dwValidStates & dwState) == 0)
		{
			dwNextState = dwState;
			dwState = 0;
		}
	}

	LONG lResult = GetTAPIConnection()->WaitForReply(
		m_pAddr->SetAgentState(dwState, dwNextState));
	if (lResult != 0)
		ShowError("lineSetAgentState", lResult);
	OnAgentStateChange();
}

void CAgentStateDlg::OnCallWork() 
{
	ClearErrors();

	LPLINEAGENTSTATUS lpStatus = m_pAddr->GetAgentStatus();
	if (lpStatus == NULL)
		return;

	DWORD dwState = LINEAGENTSTATE_WORKINGAFTERCALL, dwNextState = 0;
	if ((lpStatus->dwValidStates | lpStatus->dwValidNextStates) > 0)
	{
		if ((lpStatus->dwValidStates & dwState) == 0)
		{
			dwNextState = dwState;
			dwState = 0;
		}
	}

	LONG lResult = GetTAPIConnection()->WaitForReply(
		m_pAddr->SetAgentState(dwState, dwNextState));
	if (lResult != 0)
		ShowError("lineSetAgentState", lResult);
	OnAgentStateChange();
}

LPCSTR CAgentStateDlg::GetAgentStateString(DWORD dwAgentState)
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

}// CAgentStateDlg::GetAgentStateString

LRESULT CAgentStateDlg::i_OnAgentChange(WPARAM wParam, LPARAM lParam)
{
	if (m_pAddr == NULL)
		return 0;

	if ((CTapiAddress*)wParam == m_pAddr)
	{
		if (lParam & LINEAGENTSTATUS_ACTIVITYLIST)
			OnAvailableActivitiesChange();
		if (lParam & LINEAGENTSTATUS_GROUPLIST)
			OnAvailableGroupsChange();
		if (lParam & (LINEAGENTSTATUS_GROUP | LINEAGENTSTATUS_STATE | 
				LINEAGENTSTATUS_NEXTSTATE | LINEAGENTSTATUS_ACTIVITY))
			OnAgentStateChange();
	}
	return 0L;

}// CAgentStateDlg::i_OnAgentChange

void CAgentStateDlg::OnClose()
{
	((CPhoneDlg*)AfxGetMainWnd())->OnAgentClose(m_pAddr);
	m_pAddr = NULL;
	DestroyWindow();
	delete this;

}// CAgentStateDlg::OnClose

void CAgentStateDlg::OnUpdateS() 
{
	i_OnAgentChange((WPARAM)m_pAddr, 0xffffffff);	
}

void CAgentStateDlg::OnSelchangeActivity() 
{
	int nCurSel = m_cbActivity.GetCurSel();
	if (nCurSel != CB_ERR)
	{
		DWORD dwActivity = m_cbActivity.GetItemData(nCurSel);
		LONG lResult = GetTAPIConnection()->WaitForReply(
			m_pAddr->SetAgentActivity(dwActivity));
		if (lResult != 0)
			ShowError("lineSetAgentActivity", lResult);
		OnAgentStateChange();
	}
}

void CAgentStateDlg::OnAvailableActivitiesChange()
{
	// Delete all the information from combobox
	m_cbActivity.ResetContent();

	// First, grab all the activity entries for this address
	CPtrArray arrActivities;
	if (m_pAddr->GetAgentActivityList(arrActivities) == 0 && arrActivities.GetSize() > 0)
	{
		while (arrActivities.GetSize() > 0)
		{
			LPAGENTACTIVITY pActivity = (LPAGENTACTIVITY) arrActivities[0];
			arrActivities.RemoveAt(0);

			CString strName(pActivity->strName);
			if (strName.IsEmpty())
				strName.Format("Activity 0x%lx", pActivity->dwActivityID);
			int iPos = m_cbActivity.AddString(strName);
			ASSERT (iPos != CB_ERR);
			m_cbActivity.SetItemData(iPos, pActivity->dwActivityID);
			delete pActivity;
		}
	}

	else
		m_cbActivity.EnableWindow(FALSE);

	UpdateData(FALSE);
}
