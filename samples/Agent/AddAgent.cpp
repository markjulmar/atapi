// AddAgent.cpp : implementation file
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddAgent dialog

CAddAgent::CAddAgent(CWnd* pParent /*=NULL*/)
	: CDialog(CAddAgent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddAgent)
	m_fReload = FALSE;
	//}}AFX_DATA_INIT
}


void CAddAgent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddAgent)
	DDX_Control(pDX, IDC_AGENTLIST, m_lbAddress);
	DDX_Check(pDX, IDC_RELOAD, m_fReload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddAgent, CDialog)
	//{{AFX_MSG_MAP(CAddAgent)
	ON_BN_CLICKED(IDOK, OnAddAgents)
	ON_BN_CLICKED(IDCANCEL, OnClose)
	ON_LBN_SELCHANGE(IDC_AGENTLIST, OnSelchangeAgent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddAgent message handlers

void CAddAgent::OnAddAgents() 
{
	UpdateData(TRUE);

	for (int i = 0; i < m_lbAddress.GetCount(); i++)
	{
		if (m_lbAddress.GetSel(i) > 0)
			AgentDlg()->AddAgentToList(static_cast<DWORD>(m_lbAddress.GetItemData(i)));
	}

	if (m_fReload)
	{
		AfxGetApp()->WriteProfileInt("JTAgent", "Reload", 1);
		AfxGetApp()->WriteProfileInt("JTAgent", "ReloadCount", m_lbAddress.GetCount());
		int iEntry = 0;
		for (i = 0; i < m_lbAddress.GetCount(); i++)
		{
			if (m_lbAddress.GetSel(i) > 0)
			{
				CString strEntry, strAgent;
				strEntry.Format("Reload%d", iEntry++);
				strAgent.Format("%ld", m_lbAddress.GetItemData(i));
				AfxGetApp()->WriteProfileString("JTAgent", strEntry, strAgent);
			}
		}
	}
	else
		AfxGetApp()->WriteProfileInt("JTAgent", "Reload", 0);

	EndDialog(IDOK);
}

void CAddAgent::OnClose() 
{
	EndDialog(IDCANCEL);
}

BOOL CAddAgent::OnInitDialog() 
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
	
	CDialog::OnInitDialog();

	// Now add all the addresses to the list which support agent activity.
    for (DWORD dwCount = 0; dwCount < GetTAPIConnection()->GetLineDeviceCount(); dwCount++)
    {
        CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwCount);
        if (pLine != NULL)
        {
			CString strLineName = pLine->GetProviderInfo() + CString(" ") + pLine->GetLineName();
			for (DWORD dwAddress = 0; dwAddress < pLine->GetAddressCount(); dwAddress++)
			{
				CTapiAddress* pAddress = pLine->GetAddress(dwAddress);
				DWORD dwIndex = MAKELONG((WORD)dwCount, (WORD)dwAddress);
				if (AgentDlg()->FindAgentByID(dwIndex) == -1 &&
					pAddress->SupportsAgents())
				{
					CString strAddress = pAddress->GetDialableAddress();
					if (strAddress.IsEmpty())
						strAddress = "Address 0";
					CString strName = strLineName + CString(":") + strAddress;

					int iPos = m_lbAddress.AddString(strName);
					ASSERT (iPos != LB_ERR);
					m_lbAddress.SetItemData(iPos, MAKELONG((WORD)dwCount, (WORD)dwAddress));
				}
			}
        }
    }
	return TRUE;
}

void CAddAgent::OnSelchangeAgent() 
{
	GetDlgItem(IDOK)->EnableWindow(m_lbAddress.GetSelCount() > 0);
}
