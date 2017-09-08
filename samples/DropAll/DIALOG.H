// DIALOG.H - Dialog to run the lineDrop example
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

class CDropDlg : public CDialog
{
// Dialog Data
protected:
        //{{AFX_DATA(CDropDlg)
	enum { IDD = IDD_DROP_DIALOG };
	CComboBox	m_cbCallState;
	CButton	m_btnStart;
	CListBox	m_lbStatus;
	CComboBox	m_cbLines;
	BOOL m_fAccept;
	BOOL m_fAnswer;
	CTapiLine* m_pLine;
	CTapiCall* m_pCall;
	bool m_fRunning;
	//}}AFX_DATA

// Construction
public:
    CDropDlg(CWnd* pParent = NULL);       // standard constructor
    virtual ~CDropDlg();

// Access methods
public:
	void OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail);
	void OnNewCall (CTapiCall* pCall);

// Implementation
protected:
	void TraceMsg (const char * pszStr, ...);

	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDropDlg)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
        //{{AFX_MSG(CDropDlg)
	virtual BOOL OnInitDialog();
    afx_msg void OnBegin();
	afx_msg void OnSelchangeLines();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnCheckButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
