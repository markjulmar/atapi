// Phone.h : main header file for the PHONE application
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

#if !defined(AFX_PHONE_H__FD6DFFA9_C258_11D1_BB9C_006097D5EC19__INCLUDED_)
#define AFX_PHONE_H__FD6DFFA9_C258_11D1_BB9C_006097D5EC19__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define UM_NEWCALL		 (WM_USER + 100)
#define UM_CALLCHANGE	 (WM_USER + 101)
#define UM_ADDRESSCHANGE (WM_USER + 102)
#define UM_LINECHANGE    (WM_USER + 103)
#define UM_FLASHWINDOW   (WM_USER + 104)
#define UM_AGENTCHANGE   (WM_USER + 200)
#define UM_PHONECHANGE   (WM_USER + 300)

class CMainApp : public CWinApp
{
// Overrides
public:
	virtual BOOL InitInstance();
};

#include "hlinkctl.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
// Class data
protected:
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHLinkCtrl m_ctlWebAddress;
	//}}AFX_DATA

// Constructor
public:
	CAboutDlg() : CDialog(CAboutDlg::IDD) {}
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX) {
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_WEBLINK, m_ctlWebAddress);
	}
	//}}AFX_VIRTUAL
};

void ErrorMsg(LPCSTR pszBuff, ...);
LPCSTR LookupError(LONG lResult);
void ClearErrors();

/////////////////////////////////////////////////////////////////////////////
// CMyPhone
//
// Override of the CTapiPhone for notifications
//
class CMyPhone : public CTapiPhone
{
public:
	CWnd* m_pPhone;
	DECLARE_DYNCREATE (CMyPhone)
protected:
	CMyPhone() : CTapiPhone(), m_pPhone(0) {}
    virtual void OnDeviceStateChange (DWORD dwDeviceState, DWORD dwStateDetail) {
		CTapiPhone::OnDeviceStateChange(dwDeviceState, dwStateDetail);
		if (m_pPhone != NULL)
			m_pPhone->SendMessage(UM_PHONECHANGE, dwDeviceState, dwStateDetail);
	}
};

/////////////////////////////////////////////////////////////////////////////
// ShowError
//
// Show an error message
//
inline void ShowError(LPCSTR pszFunction, LONG lResult)
{
	ErrorMsg("%s (0x%lx) %s", pszFunction, lResult, LookupError(lResult));

}// ShowError

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONE_H__FD6DFFA9_C258_11D1_BB9C_006097D5EC19__INCLUDED_)
