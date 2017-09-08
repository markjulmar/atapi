// SetAgentGroup.cpp : implementation file
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
#include "SetAgentGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetAgentGroup dialog


CSetAgentGroup::CSetAgentGroup(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAgentGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetAgentGroup)
	m_strGroup = _T("");
	//}}AFX_DATA_INIT
}


void CSetAgentGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetAgentGroup)
	DDX_Control(pDX, IDC_GROUP, m_edtGroup);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_GROUPLIST, m_lbGroups);
	DDX_Control(pDX, IDC_AGENTS, m_cbAgent);
	DDX_Text(pDX, IDC_GROUP, m_strGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetAgentGroup, CDialog)
	//{{AFX_MSG_MAP(CSetAgentGroup)
	ON_CBN_SELCHANGE(IDC_AGENTS, OnChangeAgent)
	ON_EN_CHANGE(IDC_GROUP, OnChangeGroup)
	ON_LBN_SELCHANGE(IDC_GROUPLIST, OnChangeGroup)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetAgentGroup message handlers

void CSetAgentGroup::OnChangeAgent() 
{
	DWORD_PTR dwIndex = m_cbAgent.GetItemData(m_cbAgent.GetCurSel());
	CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(LOWORD(dwIndex));
	CTapiAddress* pAddr = pLine->GetAddress(HIWORD(dwIndex));

	// Delete all the information from our edit/listbox.
	for (int i = 0; i < m_lbGroups.GetCount(); i++)
		FreeMem((LPAGENTGROUP)m_lbGroups.GetItemData(i));
	m_lbGroups.ResetContent();
	m_strGroup = "";

	// First, grab all the group entries for this address
	CPtrArray arrGroups;
	if (pAddr->GetAgentGroupList(arrGroups) == 0 &&
		arrGroups.GetSize() > 0)
	{
		m_edtGroup.EnableWindow(FALSE);
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
	{
		m_lbGroups.EnableWindow(FALSE);
		m_edtGroup.EnableWindow(TRUE);
	}

	// Next, get the agent status and select the groups the agent belongs to.
	if (pAddr->GetCurrentAgentGroupList(arrGroups) == 0 &&
		arrGroups.GetSize() > 0)
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
		}

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
			delete pGroup;
		}
	}

	UpdateData(FALSE);
}

void CSetAgentGroup::OnChangeGroup() 
{
	m_btnOK.EnableWindow(TRUE);
}

BOOL CSetAgentGroup::OnInitDialog() 
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

	// Load our groups and settings.
	OnChangeAgent();

	return TRUE;
}

void CSetAgentGroup::OnOK() 
{
	m_btnOK.EnableWindow(FALSE);

	DWORD_PTR dwIndex = m_cbAgent.GetItemData(m_cbAgent.GetCurSel());
	CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(LOWORD(dwIndex));
	CTapiAddress* pAddr = pLine->GetAddress(HIWORD(dwIndex));

	UpdateData(TRUE);

	// If we have a listbox, then use the selected groups.
	AGENTGROUP ag;
	CPtrArray arrGroups;
	if (m_lbGroups.IsWindowEnabled())
	{
		for (int i = 0; i < m_lbGroups.GetCount(); i++)
		{
			LPAGENTGROUP pGroup = (LPAGENTGROUP) m_lbGroups.GetItemData(i);
			if (m_lbGroups.GetSel(i))
				arrGroups.Add(pGroup);
		}
	}
	else if (m_strGroup.IsEmpty() == FALSE)
	{
		ag.GroupID.dwGroupID1 = 
		ag.GroupID.dwGroupID2 = 
		ag.GroupID.dwGroupID3 = 
		ag.GroupID.dwGroupID4 = 0;
		ag.strName = m_strGroup;
		arrGroups.Add(&ag);
	}

	LONG lResult = pAddr->SetAgentGroup(arrGroups);
	if (IsTapiError(lResult))
		DisplayError("lineSetAgentGroup", lResult);

	CDialog::OnOK();
}

void CSetAgentGroup::OnDestroy() 
{
	for (int i = 0; i < m_lbGroups.GetCount(); i++)
		FreeMem((LPAGENTGROUP)m_lbGroups.GetItemData(i));
	m_lbGroups.ResetContent();
	CDialog::OnDestroy();
}
