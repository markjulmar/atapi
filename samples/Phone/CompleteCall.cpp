// CompleteCall.cpp : implementation file
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
#include "CompleteCall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct
{
	const char *pszText;
	DWORD dwMode;
} g_Modes[] = {

	{ "Camp On",	LINECALLCOMPLMODE_CAMPON },
	{ "Call back",	LINECALLCOMPLMODE_CALLBACK },
	{ "Intrude",    LINECALLCOMPLMODE_INTRUDE },
	{ "Message",    LINECALLCOMPLMODE_MESSAGE },
	{ NULL, 0 },
};

/////////////////////////////////////////////////////////////////////////////
// CCompleteCall dialog


CCompleteCall::CCompleteCall(CWnd* pParent, CTapiCall* pCall)
	: CDialog(CCompleteCall::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompleteCall)
	m_strCallerName = _T("Not Available");
	m_strCallerID = _T("");
	m_pCall = pCall;
	m_dwMode = 0;
	m_dwMessageID = 0;
	//}}AFX_DATA_INIT
}

void CCompleteCall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompleteCall)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_MODE, m_cbMode);
	DDX_Control(pDX, IDC_MESSAGEID, m_cbMessageID);
	DDX_Text(pDX, IDC_CALLERNAME, m_strCallerName);
	DDX_Text(pDX, IDC_CALLERNUM, m_strCallerID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompleteCall, CDialog)
	//{{AFX_MSG_MAP(CCompleteCall)
	ON_CBN_SELCHANGE(IDC_MODE, OnSelChange)
	ON_CBN_SELCHANGE(IDC_MESSAGEID, OnSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompleteCall message handlers

void CCompleteCall::OnOK() 
{
	m_dwMode = m_cbMode.GetItemData(m_cbMode.GetCurSel());
	m_dwMessageID = m_cbMessageID.GetItemData(m_cbMessageID.GetCurSel());
	CDialog::OnOK();
}

BOOL CCompleteCall::OnInitDialog() 
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

	// Fill in the caller id information
	if (m_pCall->GetCallInfo()->dwOrigin & LINECALLORIGIN_OUTBOUND)
	{
		m_strCallerID = m_pCall->GetCalledIDNumber();
		m_strCallerName = m_pCall->GetCalledIDName();
	}
	else
	{
		m_strCallerID = m_pCall->GetCallerIDNumber();
		m_strCallerName = m_pCall->GetCallerIDName();
	}

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

	// Fill in the available completion modes
	CTapiAddress* pAddr = m_pCall->GetAddressInfo();
	LPLINEADDRESSCAPS lpCaps = pAddr->GetAddressCaps();
	for (int i = 0; g_Modes[i].pszText != NULL; i++)
	{
		if ((lpCaps->dwCallCompletionModes & g_Modes[i].dwMode) != 0)
		{
			int iPos = m_cbMode.AddString(g_Modes[i].pszText);
			ASSERT (iPos != CB_ERR);
			m_cbMode.SetItemData(iPos, g_Modes[i].dwMode);
		}
	}	

	// Fill in the messages
	LPCSTR pszMessage = (LPCSTR)lpCaps + lpCaps->dwCompletionMsgTextOffset;
	int iSize = (int) lpCaps->dwCompletionMsgTextEntrySize;
	for (i = 0; i < (int) lpCaps->dwNumCompletionMessages; i++)
	{
		CString strBuff(pszMessage, iSize);
		if (strBuff.IsEmpty() == FALSE)
		{
			int iPos = m_cbMessageID.AddString(strBuff);
			ASSERT (iPos != CB_ERR);
			m_cbMessageID.SetItemData(iPos, (DWORD)i);
		}
		pszMessage += iSize;
	}

	if (m_cbMode.GetCount() > 0)
		m_cbMode.SetCurSel(0);
	OnSelChange();
	return TRUE;
}

void CCompleteCall::OnSelChange() 
{
	int iPos = m_cbMode.GetCurSel();
	if (iPos != CB_ERR)
	{
		if (m_cbMode.GetItemData(iPos) == LINECALLCOMPLMODE_MESSAGE)
		{
			m_cbMessageID.EnableWindow(TRUE);
			GotoDlgCtrl(&m_cbMessageID);
			m_btnOK.EnableWindow(m_cbMessageID.GetCurSel() != CB_ERR);
		}
		else
		{
			m_cbMessageID.EnableWindow(FALSE);
			m_btnOK.EnableWindow(TRUE);
		}
	}
	else
		m_btnOK.EnableWindow(FALSE);
}
