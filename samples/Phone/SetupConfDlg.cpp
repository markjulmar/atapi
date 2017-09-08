// SetupConfDlg.cpp : implementation file
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
#include "SetupConfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupConfDlg dialog


CSetupConfDlg::CSetupConfDlg(CWnd* pParent, CTapiCall* pCall, BOOL fAllowMerge)
	: CDialog(CSetupConfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupConfDlg)
	m_strCallerName = _T("");
	m_strCallerID = _T("");
	m_strNumber = _T("");
	m_pCall = pCall;
	m_fMerge = FALSE;
	m_fAllowMerge = fAllowMerge;
	//}}AFX_DATA_INIT
}


void CSetupConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupConfDlg)
	DDX_Control(pDX, IDC_NUMBER, m_edtNumber);
	DDX_Control(pDX, IDC_MERGE, m_btnMerge);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_CALLERNAME, m_strCallerName);
	DDX_Text(pDX, IDC_CALLERNUM, m_strCallerID);
	DDX_Text(pDX, IDC_NUMBER, m_strNumber);
	DDX_Check(pDX, IDC_MERGE, m_fMerge);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupConfDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupConfDlg)
	ON_EN_CHANGE(IDC_NUMBER, OnChangeNumber)
	ON_BN_CLICKED(IDC_MERGE, OnMergeChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupConfDlg message handlers

void CSetupConfDlg::OnChangeNumber() 
{
	UpdateData(TRUE);
	m_btnOK.EnableWindow(!m_strNumber.IsEmpty());
}

BOOL CSetupConfDlg::OnInitDialog() 
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

	// Enable/Disable the appropriate things.
	if (m_fAllowMerge)
	{
		m_btnMerge.EnableWindow(TRUE);
		m_fMerge = TRUE;
		UpdateData(FALSE);
	}
	OnMergeChange();
	return TRUE;
}

void CSetupConfDlg::OnMergeChange() 
{
	UpdateData(TRUE);
	m_edtNumber.EnableWindow(!m_fMerge);
	if (m_fMerge)
	{
		m_strNumber.Empty();
		m_btnOK.EnableWindow(TRUE);
	}
	else
		GotoDlgCtrl(&m_edtNumber);
	UpdateData(FALSE);
}
