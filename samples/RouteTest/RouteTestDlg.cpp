// RouteTestDlg.cpp : implementation file
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
#include "RouteTest.h"
#include "RouteTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UM_NEWCALL		(WM_USER + 100)
#define UM_CALLCHANGE	(WM_USER + 101)

extern CRouteTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyLine
//
// Override of the CTapiLine for notifications
//
class CMyLine : public CTapiLine
{
	DECLARE_DYNCREATE (CMyLine)
protected:
    virtual void OnNewCall (CTapiCall* pCall) {
		CTapiLine::OnNewCall(pCall);
		((CRouteTestDlg*)theApp.m_pMainWnd)->PostMessage(UM_NEWCALL, (WPARAM)pCall);
	}

    virtual void OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail, DWORD dwPrivilage) {
		CTapiLine::OnCallStateChange (hCall, dwState, dwStateDetail, dwPrivilage);
		((CRouteTestDlg*)theApp.m_pMainWnd)->PostMessage(UM_CALLCHANGE, (WPARAM)GetCallFromHandle(hCall));
	}
};

IMPLEMENT_DYNCREATE (CMyLine, CTapiLine)

BOOL CRouteTestApp::InitInstance()
{
    // Initialize a connection with TAPI and determine if there 
    // are any TAPI complient devices installed.
    if (GetTAPIConnection()->Init ("RouteTest", RUNTIME_CLASS(CMyLine)) != 0 ||
        GetTAPIConnection()->GetLineDeviceCount() == 0)
    {
        AfxMessageBox ("There are no TAPI devices installed!");
        return FALSE;
    }

	CRouteTestDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CRouteTestDlg dialog

CRouteTestDlg::CRouteTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRouteTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRouteTestDlg)
	m_strNumber = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRouteTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRouteTestDlg)
	DDX_Control(pDX, IDC_RPLIST, m_lbActive);
	DDX_Control(pDX, IDC_ROUTEPOINTS, m_cbRoutePoints);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_LOG, m_lbLog);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Text(pDX, IDC_NUMBER, m_strNumber);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRouteTestDlg, CDialog)
	ON_MESSAGE(UM_CALLCHANGE, OnNewCall)
	//{{AFX_MSG_MAP(CRouteTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_CBN_SELCHANGE(IDC_ROUTEPOINTS, OnSelchangeRoutePoint)
	ON_LBN_SELCHANGE(IDC_RPLIST, OnSelchangeActive)
	ON_EN_CHANGE(IDC_NUMBER, OnChangeNumber)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRouteTestDlg message handlers

BOOL CRouteTestDlg::OnInitDialog()
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

	// Connect our controls.
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Fill in all the route points
    for (DWORD dwCount = 0; dwCount < GetTAPIConnection()->GetLineDeviceCount(); dwCount++)
    {
        CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwCount);
        if (pLine)
        {
			DWORD dwAddrTotal = pLine->GetAddressCount();
			for (DWORD dwAddrCount = 0; dwAddrCount < dwAddrTotal; dwAddrCount++)
			{
				CTapiAddress* pAddr = pLine->GetAddress(dwAddrCount);
				if (pAddr)
				{
					LPLINEADDRESSCAPS lpAddrCaps = pAddr->GetAddressCaps();
					if (lpAddrCaps && (lpAddrCaps->dwAddrCapFlags & LINEADDRCAPFLAGS_ROUTEPOINT))
					{
						CString strName = pLine->GetProviderInfo() + CString(" ") + pLine->GetLineName();
						int iPos = m_cbRoutePoints.AddString (strName);
						ASSERT (iPos >= 0);
						m_cbRoutePoints.SetItemDataPtr(iPos, pLine);
						break;
					}
				}
			}
        }
    }
    
    // Automatically select the first entry in the combo box.
    if (m_cbRoutePoints.GetCount() > 0)
    {
        m_cbRoutePoints.SetCurSel(0);
        OnSelchangeRoutePoint();
    }                             

	return TRUE;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRouteTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRouteTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRouteTestDlg::OnAdd() 
{
	int nCurSel = m_cbRoutePoints.GetCurSel();
	if (nCurSel == CB_ERR)
		return;

	CTapiLine* pLine = (CTapiLine*) m_cbRoutePoints.GetItemDataPtr(nCurSel);
	for (int i = 0; i < m_lbActive.GetCount(); i++)
	{
		if (pLine == (CTapiLine*) m_lbActive.GetItemDataPtr(i))
		{
			m_lbActive.DeleteString(i);
			break;
		}
	}

	UpdateData(TRUE);

	CString strBuff = pLine->GetLineName() + CString("=") + m_strNumber;
	nCurSel = m_lbActive.AddString(strBuff);
	ASSERT (nCurSel != LB_ERR);
	m_lbActive.SetItemDataPtr(nCurSel, pLine);

	if (pLine->IsOpen() == FALSE)
	{
		// Open for ALL media modes first.
		DWORD dwMediaMode = 0;
		const LPLINEDEVCAPS lpCaps = pLine->GetLineCaps();
		if (lpCaps)
			dwMediaMode = (lpCaps->dwMediaModes & ~LINEMEDIAMODE_UNKNOWN);

		// Open the line
		TraceMsg("Opening line %s, rc=0x%lx", pLine->GetLineName(), 
			pLine->Open (LINECALLPRIVILEGE_OWNER | LINECALLPRIVILEGE_MONITOR, dwMediaMode));
	}

	m_lbActive.SetCurSel(nCurSel);
	OnSelchangeActive();
}

void CRouteTestDlg::OnRemove() 
{
	int nCurSel = m_lbActive.GetCurSel();
	if (nCurSel != LB_ERR)
	{
		CTapiLine* pLine = (CTapiLine*) m_lbActive.GetItemDataPtr(nCurSel);
		if (pLine->IsOpen())
			TraceMsg("Closing line %s, rc=0x%lx", pLine->GetLineName(), pLine->Close());
		m_lbActive.DeleteString(nCurSel);
	}
}

void CRouteTestDlg::OnSelchangeRoutePoint() 
{
	int nCurSel = m_cbRoutePoints.GetCurSel();
	if (nCurSel == CB_ERR)
		return;

	// Search for this route point in the "active" list.
	CTapiLine* pLine = (CTapiLine*) m_cbRoutePoints.GetItemDataPtr(nCurSel);
	for (int i = 0; i < m_lbActive.GetCount(); i++)
	{
		if (pLine == (CTapiLine*) m_lbActive.GetItemDataPtr(i))
		{
			m_lbActive.SetCurSel(i);
			OnSelchangeActive();
			break;
		}
	}
}

void CRouteTestDlg::OnSelchangeActive() 
{
	int nCurSel = m_lbActive.GetCurSel();
	m_btnRemove.EnableWindow (nCurSel != LB_ERR);
	m_strNumber = GetDN(nCurSel);
	UpdateData(FALSE);
}

CString CRouteTestDlg::GetDN(int nCurSel)
{
	if (nCurSel == LB_ERR)
		return "";

	CString strBuff;
	CTapiLine* pLine = (CTapiLine*) m_lbActive.GetItemDataPtr(nCurSel);
	m_lbActive.GetText(nCurSel, strBuff);

	return strBuff.Mid(CString(pLine->GetLineName()).GetLength()+1);
}

void CRouteTestDlg::OnChangeNumber() 
{
	UpdateData(TRUE);
	m_btnAdd.EnableWindow(!m_strNumber.IsEmpty());
}

LRESULT CRouteTestDlg::OnNewCall(WPARAM wParam, LPARAM)
{
	CTapiCall* pCall = (CTapiCall*) wParam;
	if (pCall != NULL)
	{
		CTapiLine* pLine = pCall->GetLineOwner();
		LPLINECALLSTATUS lpStatus = pCall->GetCallStatus();

		if (pCall->GetCallState() == LINECALLSTATE_OFFERING)
		{
			TraceMsg("Call 0x%lx (%s) %s, CanRoute=%s", pCall->GetCallHandle(), pLine->GetLineName(), 
						pCall->GetCallStateString(),
						(lpStatus->dwCallFeatures & LINECALLFEATURE_REDIRECT) ? "Yes" : "No");
			RouteCall(pCall);
		}
		else
		{
			TraceMsg("Call 0x%lx (%s) %s", pCall->GetCallHandle(), pLine->GetLineName(), 
						pCall->GetCallStateString());
			if (pCall->GetCallState() == LINECALLSTATE_IDLE)
				pCall->Deallocate();
		}
	}
	return 0L;

}// CRouteTestDlg::OnNewCall

void CRouteTestDlg::RouteCall(CTapiCall* pCall)
{
	LPLINECALLSTATUS lpStatus = pCall->GetCallStatus();
	if (lpStatus->dwCallFeatures & LINECALLFEATURE_REDIRECT)
	{
		CTapiLine* pLine = pCall->GetLineOwner();
		for (int i = 0; i < m_lbActive.GetCount(); i++)
		{
			if (pLine == (CTapiLine*) m_lbActive.GetItemDataPtr(i))
			{
				CString strNum = GetDN(i);
				TraceMsg("lineRedirect 0x%lx to %s rc=0x%lx", pCall->GetCallHandle(), 
					(LPCSTR)strNum, GetTAPIConnection()->WaitForReply(pCall->Redirect(strNum)));
				break;
			}
		}
	}
}

void CRouteTestDlg::TraceMsg (const char * pszStr, ...)
{
   va_list args;
   static char szBuff[1024];
   static int g_iItemsPerBox;
  
   va_start(args, pszStr);
   vsprintf(szBuff, pszStr, args);
   va_end(args);

   if (g_iItemsPerBox == 0)
   {
	   CRect rcBox;
	   m_lbLog.GetClientRect(&rcBox);
	   g_iItemsPerBox = (rcBox.Height() / m_lbLog.GetItemHeight(0));
   }

   m_lbLog.SetRedraw(FALSE);
   int iPos = m_lbLog.AddString (szBuff);
   if (iPos >= 500)
	   m_lbLog.DeleteString(0);
   
   if (iPos - m_lbLog.GetTopIndex() > g_iItemsPerBox)
	   m_lbLog.SetTopIndex(iPos - g_iItemsPerBox);

   m_lbLog.SetRedraw(TRUE);
   m_lbLog.Invalidate();
   m_lbLog.UpdateWindow();

   GetDlgItem(IDC_CLEAR)->EnableWindow(TRUE);
}

void CRouteTestDlg::OnClear() 
{
	m_lbLog.ResetContent();
	GetDlgItem(IDC_CLEAR)->EnableWindow(FALSE);
}
