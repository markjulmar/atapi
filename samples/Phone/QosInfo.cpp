// QosInfo.cpp : implementation file
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
#include "QosInfo.h"
#include <qos.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQosInfo dialog


CQosInfo::CQosInfo(CWnd* pParent, CTapiCall* pCall)
	: CDialog(CQosInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQosInfo)
	m_pCall = pCall;
	//}}AFX_DATA_INIT
}


void CQosInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQosInfo)
	DDX_Control(pDX, IDOK, m_btnSendQOS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQosInfo, CDialog)
	//{{AFX_MSG_MAP(CQosInfo)
	ON_BN_CLICKED(IDOK, OnSendQOS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQosInfo message handlers

BOOL CQosInfo::OnInitDialog() 
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
	m_edtHex2.Init(GetDlgItem(IDC_HEXDATA4)->GetSafeHwnd());

	// Input our receiving flowspec and sending flowspec information
	LPLINECALLINFO lpCallInfo = m_pCall->GetCallInfo();
	if (lpCallInfo->dwSendingFlowspecSize > 0)
	{
		LPVOID lpBuff = (LPVOID) ((LPBYTE)lpCallInfo + lpCallInfo->dwSendingFlowspecOffset);
		m_edtHex1.SetData(lpBuff, lpCallInfo->dwSendingFlowspecSize);
	}
	else
		m_edtHex1.SetData(NULL, 0);

	if (lpCallInfo->dwReceivingFlowspecSize > 0)
	{
		LPVOID lpBuff = (LPVOID) ((LPBYTE)lpCallInfo + lpCallInfo->dwReceivingFlowspecOffset);
		m_edtHex2.SetData(lpBuff, lpCallInfo->dwReceivingFlowspecSize);
	}
	else
		m_edtHex2.SetData(NULL, 0);

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

	LPLINECALLSTATUS lpStatus = m_pCall->GetCallStatus();
	m_btnSendQOS.EnableWindow((lpStatus->dwCallFeatures & LINECALLFEATURE_SETQOS) > 0);

	return TRUE;
}

void CQosInfo::OnSendQOS() 
{
	// Set the QOS for the call
	LONG lResult = m_pCall->SetPrivilege(LINECALLPRIVILEGE_OWNER);
	if (lResult != 0)
		ShowError("lineSetCallPrivilege", lResult);
	else
	{
		FLOWSPEC snd, rcv;

		rcv.TokenRate = snd.TokenRate = 0xffffffff;
		rcv.TokenBucketSize = snd.TokenBucketSize = 0xffffffff;
		rcv.PeakBandwidth = snd.PeakBandwidth = 0xffffffff;
		rcv.Latency = snd.Latency = 100;
		rcv.DelayVariation = snd.DelayVariation = 1000;
		rcv.ServiceType = snd.ServiceType = SERVICETYPE_BESTEFFORT;
		rcv.MaxSduSize = snd.MaxSduSize = 4096;
		rcv.MinimumPolicedSize = snd.MinimumPolicedSize = 1024;

		lResult = GetTAPIConnection()->WaitForReply(m_pCall->SetQualityOfService(
			&snd, sizeof(FLOWSPEC), &rcv, sizeof(FLOWSPEC)));
		if (lResult != 0)
			ShowError("lineCallSetQualityOfService", lResult);
	}
}

