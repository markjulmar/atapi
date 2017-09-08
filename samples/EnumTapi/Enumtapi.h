// ENUMTAPI.H
//
// Enumerate all the available TAPI service providers and provide
// configuration and capabilities display.
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

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEnumTapi main WinApp class

class CEnumTapi : public CWinApp
{
// Overrides
public:
    virtual BOOL InitInstance();
};

/////////////////////////////////////////////////////////////////////////////
// Main document object

class CTestDocument : public CDocument
{
protected: // create from serialization only
    CTestDocument() {}
    DECLARE_DYNCREATE(CTestDocument)
};

/////////////////////////////////////////////////////////////////////////////
// Main frame object

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
    CMainFrame() {}
    DECLARE_DYNCREATE(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
	{
		if (CFrameWnd::PreCreateWindow(cs))
		{
			cs.hMenu = NULL;
			cs.dwExStyle = WS_EX_DLGMODALFRAME;
			cs.style &= ~(FWS_ADDTOTITLE | WS_THICKFRAME | WS_MAXIMIZEBOX);
			cs.style |= WS_DLGFRAME;
			return TRUE;
		}
		return FALSE;
	}
};

/////////////////////////////////////////////////////////////////////////////
// Main dialog view object

class CEnumTapiView : public CFormView
{
protected: // create from serialization only
    CEnumTapiView();
    DECLARE_DYNCREATE(CEnumTapiView)

public:
    //{{AFX_DATA(CEnumTapiView)
    enum { IDD = IDD_ENUMTAPI_FORM };
    CListCtrl m_lbServiceProviders;
    CListBox m_lbAddress;
    CComboBox m_cbDevices;   
    CString m_strCaps;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnInitialUpdate();
    void SetupLineAddressCaps();
    void AddCapabilities(DWORD dwFlags, const char *pszArray[]);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
	{
		if (CFormView::PreCreateWindow(cs))
		{
			cs.dwExStyle = 0;
			return TRUE;
		}
		return FALSE;
	}

// Generated message map functions
protected:
    //{{AFX_MSG(CEnumTapiView)
    afx_msg void OnConfigure();
    afx_msg void OnSelchangeDevices();
    afx_msg void OnSelchangeAddresslist();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
