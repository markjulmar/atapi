// UUIDlg.cpp : implementation file
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
#include "UUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUUIDlg dialog


CUUIDlg::CUUIDlg(CWnd* pParent, CTapiCall* pCall)
	: CDialog(CUUIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUUIDlg)
	m_strUUI = _T("");
	m_pCall = pCall;
	//}}AFX_DATA_INIT
}


void CUUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUUIDlg)
	DDX_Control(pDX, IDOK, m_btnRelease);
	DDX_Control(pDX, IDC_SENDUUI, m_btnSend);
	DDX_Text(pDX, IDC_CALLDATA, m_strUUI);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUUIDlg, CDialog)
	//{{AFX_MSG_MAP(CUUIDlg)
	ON_BN_CLICKED(IDOK, OnReleaseUUI)
	ON_EN_CHANGE(IDC_CALLDATA, OnChange)
	ON_BN_CLICKED(IDC_SENDUUI, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUUIDlg message handlers

void CUUIDlg::OnReleaseUUI() 
{
	// Set the call data in the call.
	LONG lResult = m_pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		lResult = GetTAPIConnection()->WaitForReply(m_pCall->ReleaseUserUserInfo());
		if (lResult != 0)
			ShowError("lineReleaseUserUserInfo", lResult);
	}

	LPLINECALLSTATUS lpStatus = m_pCall->GetCallStatus();
	m_btnRelease.EnableWindow((lpStatus->dwCallFeatures & LINECALLFEATURE_RELEASEUSERUSERINFO) > 0);

	// If we have new UUI, retrieve it.
	LPLINECALLINFO lpCallInfo = m_pCall->GetCallInfo();
	if (lpCallInfo->dwUserUserInfoSize > 0)
	{
		LPVOID lpBuff = (LPVOID) ((LPBYTE)lpCallInfo + lpCallInfo->dwUserUserInfoOffset);
		m_edtHex.SetData(lpBuff, lpCallInfo->dwUserUserInfoSize);
	}
	else
		m_edtHex.SetData(NULL, 0);
}

void CUUIDlg::OnSend() 
{
	UpdateData(TRUE);

	// Set the UUI for the call
	LONG lResult = m_pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		lResult = GetTAPIConnection()->WaitForReply(m_pCall->SendUserUserInfo(m_strUUI, m_strUUI.GetLength()+1));
		if (lResult != 0)
			ShowError("lineSendUserUserInfo", lResult);
	}
}

BOOL CUUIDlg::OnInitDialog() 
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

	// If we have UUI in our call now, retrieve it.
	LPLINECALLINFO lpCallInfo = m_pCall->GetCallInfo();
	if (lpCallInfo->dwUserUserInfoSize > 0)
	{
		LPVOID lpBuff = (LPVOID) ((LPBYTE)lpCallInfo + lpCallInfo->dwUserUserInfoOffset);
		m_edtHex.SetData(lpBuff, lpCallInfo->dwUserUserInfoSize);
	}
	else
		m_edtHex.SetData(NULL, 0);

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

	LPLINECALLSTATUS lpStatus = m_pCall->GetCallStatus();
	m_btnRelease.EnableWindow((lpStatus->dwCallFeatures & LINECALLFEATURE_RELEASEUSERUSERINFO) > 0);
	m_btnSend.EnableWindow((lpStatus->dwCallFeatures & LINECALLFEATURE_SENDUSERUSER) > 0);

	// Enable/Disable the edit.
	CTapiLine* pLine = m_pCall->GetLineOwner();
	GetDlgItem(IDC_CALLDATA)->EnableWindow((lpStatus->dwCallFeatures & LINECALLFEATURE_SENDUSERUSER) > 0);
	((CEdit*)GetDlgItem(IDC_CALLDATA))->LimitText(pLine->GetLineCaps()->dwUUISendUserUserInfoSize);

	return TRUE;
}

void CUUIDlg::OnChange() 
{
	UpdateData(TRUE);
	m_btnSend.EnableWindow(!m_strUUI.IsEmpty());
}
