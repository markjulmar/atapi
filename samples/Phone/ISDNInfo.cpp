// ISDNInfo.cpp : implementation file
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
#include "ISDNInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CISDNInfo dialog


CISDNInfo::CISDNInfo(CWnd* pParent, CTapiCall* pCall)
	: CDialog(CISDNInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CISDNInfo)
	m_pCall = pCall;
	//}}AFX_DATA_INIT
}


void CISDNInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CISDNInfo)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CISDNInfo, CDialog)
	//{{AFX_MSG_MAP(CISDNInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CISDNInfo message handlers

BOOL CISDNInfo::OnInitDialog() 
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

	m_edtHex1.Init(GetDlgItem(IDC_HEXDATA)->GetSafeHwnd());
	m_edtHex2.Init(GetDlgItem(IDC_HEXDATA2)->GetSafeHwnd());
	m_edtHex3.Init(GetDlgItem(IDC_HEXDATA3)->GetSafeHwnd());

	// If we have charging information in our structure then set it.
	LPLINECALLINFO lpCallInfo = m_pCall->GetCallInfo();
	if (lpCallInfo->dwChargingInfoSize > 0)
	{
		LPVOID lpBuff = (LPVOID) ((LPBYTE)lpCallInfo + lpCallInfo->dwChargingInfoOffset);
		m_edtHex1.SetData(lpBuff, lpCallInfo->dwChargingInfoSize);
	}
	else
		m_edtHex1.SetData(NULL, 0);

	// Low-level info
	if (lpCallInfo->dwLowLevelCompSize > 0)
	{
		LPVOID lpBuff = (LPVOID) ((LPBYTE)lpCallInfo + lpCallInfo->dwLowLevelCompOffset);
		m_edtHex2.SetData(lpBuff, lpCallInfo->dwLowLevelCompSize);
	}
	else
		m_edtHex2.SetData(NULL, 0);

	// Hi-level info
	if (lpCallInfo->dwHighLevelCompSize > 0)
	{
		LPVOID lpBuff = (LPVOID) ((LPBYTE)lpCallInfo + lpCallInfo->dwHighLevelCompOffset);
		m_edtHex3.SetData(lpBuff, lpCallInfo->dwHighLevelCompSize);
	}
	else
		m_edtHex3.SetData(NULL, 0);

	// Connect all the controls via DDX
	CDialog::OnInitDialog();
	
	return TRUE;

}// CISDNInfo::OnInitDialog

