// PredDialDlg.cpp : implementation file
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
#include "PredDialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static struct
{
	DWORD dwCallState;
	LPCTSTR pszState;
} g_CallStates[] = {
	{ LINECALLSTATE_OFFERING, _T("LINECALLSTATE_OFFERING") },
	{ LINECALLSTATE_ACCEPTED, _T("LINECALLSTATE_ACCEPTED") },
	{ LINECALLSTATE_DIALTONE, _T("LINECALLSTATE_DIALTONE") },
	{ LINECALLSTATE_DIALING, _T("LINECALLSTATE_DIALING") },
	{ LINECALLSTATE_RINGBACK, _T("LINECALLSTATE_RINGBACK") },
	{ LINECALLSTATE_BUSY, _T("LINECALLSTATE_BUSY") },
	{ LINECALLSTATE_SPECIALINFO, _T("LINECALLSTATE_SPECIALINFO") },
	{ LINECALLSTATE_CONNECTED, _T("LINECALLSTATE_CONNECTED") },
	{ LINECALLSTATE_PROCEEDING, _T("LINECALLSTATE_PROCEEDING") },
	{ LINECALLSTATE_ONHOLD, _T("LINECALLSTATE_ONHOLD") },
	{ LINECALLSTATE_CONFERENCED, _T("LINECALLSTATE_CONFERENCED") },
	{ LINECALLSTATE_ONHOLDPENDCONF, _T("LINECALLSTATE_ONHOLDPENDCONF") },
	{ LINECALLSTATE_ONHOLDPENDTRANSFER, _T("LINECALLSTATE_ONHOLDPENDTRANSFER") },
	{ LINECALLSTATE_DISCONNECTED, _T("LINECALLSTATE_DISCONNECTED") },
	{ 0, NULL },
};

/////////////////////////////////////////////////////////////////////////////
// CPredDialDlg dialog

CPredDialDlg::CPredDialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPredDialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPredDialDlg)
	m_strTarget = _T("");
	m_nTimeout = 0;
	m_dwCallStates = 0;
	//}}AFX_DATA_INIT
}

void CPredDialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPredDialDlg)
	DDX_Control(pDX, IDC_CALLSTATES, m_lbCallStates);
	DDX_Control(pDX, IDC_SPIN1, m_ctlSpinner);
	DDX_Text(pDX, IDC_NUMBER, m_strTarget);
	DDX_Text(pDX, IDC_TIMEOUT, m_nTimeout);
	DDV_MinMaxInt(pDX, m_nTimeout, 0, 9999);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPredDialDlg, CDialog)
	//{{AFX_MSG_MAP(CPredDialDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPredDialDlg message handlers

BOOL CPredDialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_dwCallStates == 0)
	{
		m_lbCallStates.EnableWindow(FALSE);
	}
	else
	{
		for (int i = 0; g_CallStates[i].pszState != NULL; i++)
		{
			if ((g_CallStates[i].dwCallState & m_dwCallStates) != 0)
			{
				int iPos = m_lbCallStates.AddString(g_CallStates[i].pszState);
				m_lbCallStates.SetItemData(iPos, g_CallStates[i].dwCallState);
			}
		}
	}

	reinterpret_cast<CEdit*>(GetDlgItem(IDC_NUMBER))->LimitText(20);
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_TIMEOUT))->LimitText(4);

	if (m_nTimeout == 0)
	{
		m_ctlSpinner.EnableWindow(FALSE);
		GetDlgItem(IDC_TIMEOUT)->EnableWindow(FALSE);
	}
	else
		m_ctlSpinner.SetRange(0, static_cast<short>(m_nTimeout));

	m_nTimeout = 0;
	UpdateData(FALSE);

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPredDialDlg::OnOK() 
{
	if (UpdateData(TRUE))
	{
		// Calc the call states
		m_dwCallStates = 0;
		for (int i = 0; i < m_lbCallStates.GetCount(); i++)
		{
			if (m_lbCallStates.GetSel(i))
				m_dwCallStates |= m_lbCallStates.GetItemData(i);
		}
		CDialog::OnOK();
	}
}
