// TransferDlg.cpp : implementation file
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
#include "TransferDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg dialog


CTransferDlg::CTransferDlg(CWnd* pParent, CTapiCall* pCall)
	: CDialog(CTransferDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferDlg)
	m_strCallerID = _T("");
	m_strCallerName = _T("Not Available");
	m_fConsultation = FALSE;
	m_strNumber = _T("");
	m_pCall = pCall;
	//}}AFX_DATA_INIT
}


void CTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferDlg)
	DDX_Control(pDX, IDC_CONSXFER, m_btnConsTrans);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_CALLERNUM, m_strCallerID);
	DDX_Text(pDX, IDC_CALLERNAME, m_strCallerName);
	DDX_Check(pDX, IDC_CONSXFER, m_fConsultation);
	DDX_Text(pDX, IDC_NUMBER, m_strNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransferDlg, CDialog)
	//{{AFX_MSG_MAP(CTransferDlg)
	ON_EN_CHANGE(IDC_NUMBER, OnChangeNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg message handlers

BOOL CTransferDlg::OnInitDialog() 
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

	// If the call doesn't allow for blind transfer, then uncheck/disable the box.
	if ((m_pCall->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_BLINDTRANSFER) == 0)
	{
		m_fConsultation = TRUE;
		UpdateData(FALSE);
		m_btnConsTrans.EnableWindow(FALSE);
	}
	else if ((m_pCall->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_SETUPTRANSFER) == 0)
	{
		m_fConsultation = FALSE;
		UpdateData(FALSE);
		m_btnConsTrans.EnableWindow(FALSE);
	}

	return TRUE;
}

void CTransferDlg::OnChangeNumber() 
{
	UpdateData(TRUE);
	m_btnOK.EnableWindow(!m_strNumber.IsEmpty());
}

void CTransferDlg::OnOK() 
{
	CDialog::OnOK();
}

