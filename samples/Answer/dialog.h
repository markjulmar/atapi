// DIALOG.H - Dialog to run the lineAnswer example
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

class CAnswerDlg : public CDialog
{
// Dialog Data
protected:
	//{{AFX_DATA(CAnswerDlg)
	enum { IDD = IDD_ANSWER_DIALOG };
	CListBox	m_lbStatus;
	CButton	m_btnOpen;
	CComboBox	m_cbLines;
	CButton	m_btnDrop;
	CButton	m_btnAnswer;
	CButton	m_btnAccept;
	CTapiLine* m_pLine;
	CTapiCall* m_pCall;
	//}}AFX_DATA

// Construction
public:
	CAnswerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CAnswerDlg();

// Access methods
public:
	void OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail);
	void OnNewCall (CTapiCall* pCall);
	void OnCallerID(HCALL hCall);
	void OnRing();
	void OnMsgWait(BOOL fOn);

// Implementation
protected:
	void UpdateButtons();
	void TraceMsg (const char * pszStr, ...);
	CString GetCallState(DWORD dwState, DWORD dwDetail);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnswerDlg)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CAnswerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAccept();
	afx_msg void OnAnswer();
	afx_msg void OnDrop();
	afx_msg void OnSelchangeLines();
	afx_msg void OnOpen();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
