// ConfListDlg.cpp : implementation file
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
#include "ConfListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfListDlg dialog


CConfListDlg::CConfListDlg(CWnd* pParent, CTapiCall* pCall)
	: CDialog(CConfListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfListDlg)
	m_pCall = pCall;
	//}}AFX_DATA_INIT
}


void CConfListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfListDlg)
	DDX_Control(pDX, IDC_LIST, m_lbList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfListDlg, CDialog)
	//{{AFX_MSG_MAP(CConfListDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfListDlg message handlers

BOOL CConfListDlg::OnInitDialog() 
{
    // Reset the font to all be ANSI fixed.
    CFont fntAnsi;
    fntAnsi.CreateStockObject (ANSI_FIXED_FONT);
    CWnd* pwndChild = GetWindow (GW_CHILD);
    while (pwndChild != NULL && IsChild(pwndChild))
    {
        pwndChild->SetFont(&fntAnsi);
        pwndChild = pwndChild->GetWindow(GW_HWNDNEXT);
    }

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

	// Load the list with the conference members.
	CObList lstCalls;
	m_pCall->GetConferenceRelatedCalls(lstCalls);
	for (POSITION pos = lstCalls.GetHeadPosition(); pos != NULL;)
	{
		CTapiCall* pCall = (CTapiCall*) lstCalls.GetNext(pos);
		CString strBuff;
		if (pCall->GetCallInfo()->dwOrigin == LINECALLORIGIN_CONFERENCE)
		{
			strBuff.Format("Owner x%lx - %s", pCall, pCall->GetCallStateString());
			m_lbList.InsertString(0, strBuff);
		}
		else
		{
			strBuff.Format("   Member x%lx - %s", pCall, pCall->GetCallStateString());
			m_lbList.AddString(strBuff);
		}
	}

	return TRUE;
}
