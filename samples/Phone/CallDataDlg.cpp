// CallDataDlg.cpp : implementation file
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
#include "Phone.h"
#include "PhoneDlg.h"
#include "CallDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallDataDlg dialog


CCallDataDlg::CCallDataDlg(CWnd* pParent, CTapiCall* pCall)
	: CDialog(CCallDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCallDataDlg)
	m_strCallData = _T("");
	m_pCall = pCall;
	//}}AFX_DATA_INIT
}

void CCallDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallDataDlg)
	DDX_Text(pDX, IDC_CALLDATA, m_strCallData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCallDataDlg, CDialog)
	//{{AFX_MSG_MAP(CCallDataDlg)
	ON_EN_CHANGE(IDC_CALLDATA, OnChangeCalldata)
	ON_BN_CLICKED(IDOK, OnSetCallData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallDataDlg message handlers

void CCallDataDlg::OnChangeCalldata() 
{
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(!m_strCallData.IsEmpty());
}

void CCallDataDlg::OnSetCallData() 
{
	UpdateData(TRUE);

	// Set the call data in the call.
	LONG lResult = m_pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		lResult = GetTAPIConnection()->WaitForReply(m_pCall->SetCallData((LPVOID)((LPCSTR)m_strCallData), m_strCallData.GetLength()+1));
		if (lResult != 0)
			ShowError("lineSetCallData", lResult);
	}

	// If we have CALLdata in our call now, retrieve it.
	LPLINECALLINFO lpCallInfo = m_pCall->GetCallInfo();
	if (lpCallInfo->dwCallDataSize > 0)
	{
		LPVOID lpBuff = (LPVOID) ((LPBYTE)lpCallInfo + lpCallInfo->dwCallDataOffset);
		m_edtHex.SetData(lpBuff, lpCallInfo->dwCallDataSize);
	}
	else
		m_edtHex.SetData(NULL, 0);
}

BOOL CCallDataDlg::OnInitDialog() 
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

	m_edtHex.Init(GetDlgItem(IDC_HEXDATA)->GetSafeHwnd());

	// Enable/Disable the edit.
	CTapiAddress* pAddr = m_pCall->GetAddressInfo();
	GetDlgItem(IDC_CALLDATA)->EnableWindow((m_pCall->GetCallStatus()->dwCallFeatures & LINECALLFEATURE_SETCALLDATA) > 0);
	((CEdit*)GetDlgItem(IDC_CALLDATA))->LimitText(pAddr->GetAddressCaps()->dwMaxCallDataSize);

	// If we have CALLdata in our call now, retrieve it.
	LPLINECALLINFO lpCallInfo = m_pCall->GetCallInfo();
	if (lpCallInfo->dwCallDataSize > 0)
	{
		LPVOID lpBuff = (LPVOID) ((LPBYTE)lpCallInfo + lpCallInfo->dwCallDataOffset);
		m_edtHex.SetData(lpBuff, lpCallInfo->dwCallDataSize);
	}
	else
		m_edtHex.SetData(NULL, 0);

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

	return TRUE;
}
