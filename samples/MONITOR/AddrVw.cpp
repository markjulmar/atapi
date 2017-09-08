// AddrVw.cpp : implementation file
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
#include "Monitor.h"
#include "AddrVw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char * g_pszAddressSharing[] = {
    {"Private"}, {"Bridged-Exclusive"}, {"Bridged-New"}, 
    {"Bridged-Shared"}, {"Monitored"}, NULL };

const char * g_pszAddrFeatures[] = {
    {"Forward"}, {"MakeCall"}, {"Pickup"}, {"SetMediaControl"},
    {"SetTerminal"}, {"SetupConf"}, {"UncompleteCall"}, {"Unpark"}, NULL};

/////////////////////////////////////////////////////////////////////////////
// CAddrView

IMPLEMENT_DYNCREATE(CAddrView, CFormView)

CAddrView::CAddrView()
	: CFormView(CAddrView::IDD)
{
	//{{AFX_DATA_INIT(CAddrView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAddrView::~CAddrView()
{
}

void CAddrView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddrView)
	DDX_Control(pDX, IDC_ADDRESS, m_cbAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddrView, CFormView)
	//{{AFX_MSG_MAP(CAddrView)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_ADDRESS, OnAddressChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddrView diagnostics

#ifdef _DEBUG
void CAddrView::AssertValid() const
{
	CFormView::AssertValid();
}

void CAddrView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

BOOL CAddrView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

CMonitorDoc* CAddrView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMonitorDoc)));
	return (CMonitorDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CAddrView message handlers

void CAddrView::OnInitialUpdate() 
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
    
    // Connect the dialog form up to our dialog controls.
    CFormView::OnInitialUpdate();
}

HBRUSH CAddrView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == 65535)
		pDC->SetTextColor(RGB(0,0,255));
	return hbr;
}


/////////////////////////////////////////////////////////////////////////////
// CAddrView message handlers

void CAddrView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/) 
{
	if (m_cbAddress.GetSafeHwnd() == NULL)
		return;

	CMonitorDoc* pDoc = GetDocument();
	if (lHint == HINT_LINECHANGE)
	{
		// Reload our address list.
		m_cbAddress.ResetContent();
		CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(pDoc->m_dwLineDevice);
		ASSERT (pLine != NULL);

		CString strName;
		for (DWORD i = 0; i < pLine->GetAddressCount(); i++)
		{
			CTapiAddress* pAddr = pLine->GetAddress(i);
			strName.Format("%ld - %s", pAddr->GetAddressID(), pAddr->GetDialableAddress());
			int iPos = m_cbAddress.AddString(strName);
			ASSERT (iPos >= 0);
			m_cbAddress.SetItemData(iPos, i);
		}

		if (m_cbAddress.GetCount() > 0)
		{
			m_cbAddress.SetCurSel(0);
			OnAddressChange();
		}
	}
}

void CAddrView::OnAddressChange() 
{

}
