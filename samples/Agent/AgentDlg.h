// AgentDlg.h : header file
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

#if !defined(AFX_AGENTDLG_H__F6B4E4DB_C01A_11D1_BB98_006097D5EC19__INCLUDED_)
#define AFX_AGENTDLG_H__F6B4E4DB_C01A_11D1_BB98_006097D5EC19__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"

class CMainApp : public CWinApp
{
// Overrides
public:
	virtual BOOL InitInstance();
};

inline void DisplayError(LPCSTR pszAPI, LONG lResult)
{
	CString strBuff;
	strBuff.Format("%s failed, rc=0x%lx", pszAPI, lResult);
	AfxMessageBox (strBuff);
}

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg dialog

class CAgentDlg : public CDialog
{
// Construction
public:
	CAgentDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAgentDlg)
	enum { IDD = IDD_AGENT_DIALOG };
	CButton	m_btnSetAgentState;
	CButton	m_btnSetAgentGroup;
	CButton	m_btnSetAgentActivity;
	CButton	m_btnRemove;
	CButton	m_btnAdd;
	CListCtrl	m_lcAgents;
	HICON m_hIcon;
	//}}AFX_DATA

public:
	int FindAgentByID(DWORD dwID);
	void AddAgentToList(DWORD dwID);
	int GetAgentCount();
	DWORD GetAgent(int iPos);
	int GetSelectedAgent();
	LPCSTR GetAgentStateString(DWORD dwAgentState);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	void UpdateButtons();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAgentDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnAgentStateChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAddAgent();
	afx_msg void OnRemoveAgent();
	afx_msg void OnSetAgentActivity();
	afx_msg void OnSetAgentGroup();
	afx_msg void OnSetAgentState();
	afx_msg void OnAgentChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkAgents(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGENTDLG_H__F6B4E4DB_C01A_11D1_BB98_006097D5EC19__INCLUDED_)
