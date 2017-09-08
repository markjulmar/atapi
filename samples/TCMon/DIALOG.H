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

class CMonitorDlg : public CDialog
{
// Dialog Data
protected:
	//{{AFX_DATA(CMonitorDlg)
	enum { IDD = IDD_TCMON_DIALOG };
	CButton	m_btnUnhold;
	CButton	m_btnHold;
	CListCtrl m_lcCalls;
	CButton	m_btnDrop;
	CButton	m_btnAnswer;
	CButton	m_btnAccept;
	HICON m_hIcon;
	//}}AFX_DATA

// Construction
public:
	CMonitorDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CMonitorDlg();

// Access methods
public:
	void OnCallStateChange (HCALL hCall, DWORD dwState, DWORD dwStateDetail);
	void OnNewCall (CTapiCall* pCall);
	void OnCallerID(HCALL hCall);
	void OpenAllLines();

// Implementation
protected:
	void UpdateButtons();
	CTapiCall* GetSelectedCall();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitorDlg)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAccept();
	afx_msg void OnAnswer();
	afx_msg void OnDrop();
	virtual void OnOK();
	afx_msg void OnCallGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCallChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHold();
	afx_msg void OnUnhold();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
