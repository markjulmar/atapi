// AgentProperties.cpp : implementation file
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
#include "AgentProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAgentProperties dialog


CAgentProperties::CAgentProperties(CWnd* pParent, DWORD dwIndex)
	: CDialog(CAgentProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgentProperties)
	m_strName = _T("");
	m_strState = _T("");
	m_strNextState = _T("");
	m_dwIndex = dwIndex;
	m_strActivity = _T("");
	//}}AFX_DATA_INIT
}


void CAgentProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgentProperties)
	DDX_Control(pDX, IDC_GROUPS, m_lbGroups);
	DDX_Text(pDX, IDC_ADDRESS, m_strName);
	DDX_Text(pDX, IDC_CURRSTATE, m_strState);
	DDX_Text(pDX, IDC_NEXTSTATE, m_strNextState);
	DDX_Text(pDX, IDC_ACTIVITY, m_strActivity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAgentProperties, CDialog)
	//{{AFX_MSG_MAP(CAgentProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAgentProperties message handlers

BOOL CAgentProperties::OnInitDialog() 
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

	CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(LOWORD(m_dwIndex));
	CTapiAddress* pAddr = pLine->GetAddress(HIWORD(m_dwIndex));

	// Fill in the name
	if (pAddr->GetDialableAddress().IsEmpty())
		m_strName.Format("%s %s - Address %ld", pLine->GetProviderInfo(), pLine->GetLineName(), pAddr->GetAddressID());
	else
		m_strName.Format("%s %s - %s", pLine->GetProviderInfo(), pLine->GetLineName(), pAddr->GetDialableAddress());

	// Get our agent status.
	LPLINEAGENTSTATUS lpStatus = pAddr->GetAgentStatus();
	if (lpStatus != NULL)
	{
		m_strState = AgentDlg()->GetAgentStateString(lpStatus->dwState);
		m_strNextState = AgentDlg()->GetAgentStateString(lpStatus->dwNextState);

		// Next, get the agent status and select the groups the agent belongs to.
		CPtrArray arrGroups;
		if (pAddr->GetCurrentAgentGroupList(arrGroups) == 0 &&
			arrGroups.GetSize() > 0)
		{
			while (arrGroups.GetSize() > 0)
			{
				LPAGENTGROUP pGroup = (LPAGENTGROUP) arrGroups[0];
				arrGroups.RemoveAt(0);

				CString strName;(pGroup->strName);
				strName.Format("0x%lx-0x%lx-0x%lx-0x%lx %s", 
							pGroup->GroupID.dwGroupID1,
							pGroup->GroupID.dwGroupID2,
							pGroup->GroupID.dwGroupID3,
							pGroup->GroupID.dwGroupID4, (LPCSTR) pGroup->strName);
				m_lbGroups.AddString(strName);
				FreeMem(pGroup);
			}
		}

		// If the activity ID is filled in, use it.
		if (lpStatus->dwActivitySize > 0 && lpStatus->dwActivityOffset > 0)
			m_strActivity.Format("0x%lx %s", lpStatus->dwActivityID,
					(LPCSTR)((LPBYTE)lpStatus + lpStatus->dwActivityOffset));
		else
			m_strActivity.Format("0x%lx Unknown Activity", lpStatus->dwActivityID);
	}

	UpdateData(FALSE);

	return TRUE;
}
