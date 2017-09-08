// DIALOG.H - Dialog to run the lineMAKEDROP example
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

#include "resource.h"		// main symbols

class CMainApp : public CWinApp
{
// Overrides
public:
	virtual BOOL InitInstance();
};

class CMAKEDROPDlg : public CDialog
{
// Dialog Data
protected:
        //{{AFX_DATA(CMAKEDROPDlg)
	enum { IDD = IDD_MAKEDROP_DIALOG };
	CSpinButtonCtrl	m_ctlSpin;
	CComboBox	m_cbCallState;
	CButton	m_btnStart;
	CListBox	m_lbStatus;
	CComboBox	m_cbLines;
	CTapiLine* m_pLine;
	CTapiCall* m_pCall;
	BOOL m_fTimerRunning;
	CString	m_strNumber;
	DWORD	m_dwMsec;
	//}}AFX_DATA

// Construction
public:
    CMAKEDROPDlg(CWnd* pParent = NULL);       // standard constructor
    virtual ~CMAKEDROPDlg();

// Access methods
public:
	void OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail);
	void OnNewCall (CTapiCall* pCall);

// Implementation
protected:
	void TraceMsg (const char * pszStr, ...);

	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMAKEDROPDlg)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
        //{{AFX_MSG(CMAKEDROPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT uidTimer);
    afx_msg void OnMAKEDROP();
	afx_msg void OnSelchangeLines();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnChangeTimeout();
	afx_msg void OnChangeNumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
