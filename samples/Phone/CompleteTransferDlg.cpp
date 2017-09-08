// CompleteTransferDlg.cpp : implementation file
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
#include "CompleteTransferDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDT_TIMER 100

/////////////////////////////////////////////////////////////////////////////
// CCompleteTransferDlg dialog


CCompleteTransferDlg::CCompleteTransferDlg(CWnd* pParent, CTapiCall* pCall, CTapiCall* pCons, BOOL fIsConf)
	: CDialog(CCompleteTransferDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompleteTransferDlg)
	m_strCalledName = _T("Not Available");
	m_strCalledNum = _T("");
	m_strCallerName = _T("Not Available");
	m_strCallerNum = _T("");
	m_fConference = fIsConf;
	m_strElapsedTime = _T("00:00:00");
	m_pCall = pCall;
	m_pCons = pCons;
	m_strCallState1 = _T("");
	m_strCallState2 = _T("");
	//}}AFX_DATA_INIT
	GetSystemTime(&m_st);
}

void CCompleteTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompleteTransferDlg)
	DDX_Control(pDX, IDC_CONFERENCE, m_btnConf);
	DDX_Text(pDX, IDC_CALLEDNAME, m_strCalledName);
	DDX_Text(pDX, IDC_CALLEDNUM, m_strCalledNum);
	DDX_Text(pDX, IDC_CALLERNAME, m_strCallerName);
	DDX_Text(pDX, IDC_CALLERNUM, m_strCallerNum);
	DDX_Check(pDX, IDC_CONFERENCE, m_fConference);
	DDX_Text(pDX, IDC_ELAPSEDTIME, m_strElapsedTime);
	DDX_Text(pDX, IDC_CALLSTATE1, m_strCallState1);
	DDX_Text(pDX, IDC_CALLSTATE2, m_strCallState2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompleteTransferDlg, CDialog)
	//{{AFX_MSG_MAP(CCompleteTransferDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompleteTransferDlg message handlers

void CCompleteTransferDlg::OnOK() 
{
	if (m_btnConf.IsWindowVisible())
	{
		if ((m_pCons->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_COMPLETETRANSF) == 0 &&
			(m_pCall->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_COMPLETETRANSF) == 0)
		{
			ErrorMsg("Call not in proper state to complete the transfer");
			return;
		}
	}
	else
	{
		if ((m_pCons->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_ADDTOCONF) == 0)
		{
			ErrorMsg("Call not in proper state to add to the conference");
			return;
		}
	}

	CDialog::OnOK();
}

BOOL CCompleteTransferDlg::OnInitDialog() 
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
		m_strCallerNum = m_pCall->GetCalledIDNumber();
		m_strCallerName = m_pCall->GetCalledIDName();
	}
	else
	{
		m_strCallerNum = m_pCall->GetCallerIDNumber();
		m_strCallerName = m_pCall->GetCallerIDName();
	}

	if (m_pCons->GetCallInfo()->dwOrigin & LINECALLORIGIN_OUTBOUND)
	{
		m_strCalledNum = m_pCons->GetCalledIDNumber();
		m_strCalledName = m_pCons->GetCalledIDName();
	}
	else
	{
		m_strCalledNum = m_pCons->GetCallerIDNumber();
		m_strCalledName = m_pCons->GetCallerIDName();
	}

	m_strCallState1 = m_pCall->GetCallStateString();
	m_strCallState2 = m_pCons->GetCallStateString();

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

	// If the conference button is already selected, then this is a COMPLETE CONFERENCE
	// change our title.
	if (m_fConference)
	{
		SetWindowText("Complete the Conference");
		GetDlgItem(IDOK)->SetWindowText("Conference");
		m_btnConf.ShowWindow(FALSE);
		m_btnConf.EnableWindow(FALSE);
	}
	else
	{
		// If the owner address doesn't allow conference transfer mode, then
		// disable the button.
		CTapiAddress* pAddr = m_pCall->GetAddressInfo();
		if (pAddr)
		{
			LPLINEADDRESSCAPS lpCaps = pAddr->GetAddressCaps();
			if (lpCaps)
			{
				if ((lpCaps->dwTransferModes & LINETRANSFERMODE_CONFERENCE) == 0)
				{
					m_fConference = FALSE;
					UpdateData(FALSE);
					m_btnConf.EnableWindow(FALSE);
				}
				else if ((lpCaps->dwTransferModes & LINETRANSFERMODE_TRANSFER) == 0)
				{
					m_fConference = TRUE;
					UpdateData(FALSE);
					m_btnConf.EnableWindow(FALSE);
				}
			}
		}
	}

	SetTimer(IDT_TIMER, 1000, NULL);

	return TRUE;
}

void CCompleteTransferDlg::OnTimer(UINT /*nIDEvent*/) 
{
	UpdateData(TRUE);
	CTime tmStamp(m_st);
	SYSTEMTIME st;
	GetSystemTime(&st);
	CTime tmNow(st);
	CTimeSpan tmDiff = tmNow - tmStamp;
	m_strElapsedTime = tmDiff.Format("%H:%M:%S");
	m_strCallState1 = m_pCall->GetCallStateString();
	m_strCallState2 = m_pCons->GetCallStateString();
	UpdateData(FALSE);
}
