// AgentStateDlg.h : header file
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

#if !defined(AFX_AGENTSTATEDLG_H__56E41075_C59B_11D1_BBA9_006097D5EC19__INCLUDED_)
#define AFX_AGENTSTATEDLG_H__56E41075_C59B_11D1_BBA9_006097D5EC19__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CAgentStateDlg dialog

class CAgentStateDlg : public CDialog
{
// Construction
public:
	void OnAvailableActivitiesChange();
	CAgentStateDlg(CWnd* pParent, CTapiAddress* pAddr);

// Dialog Data
	//{{AFX_DATA(CAgentStateDlg)
	enum { IDD = IDD_SETAGENTSTATE };
	CComboBox	m_cbActivity;
	CEdit	m_edtGroup2;
	CEdit	m_edtGroup;
	CButton	m_btnCallWork;
	CButton	m_btnSetGroup;
	CButton	m_btnReady;
	CButton	m_btnNotReady;
	CButton	m_btnLogout;
	CListBox	m_lbGroups;
	CString	m_strCurrentState;
	CString	m_strGroup;
	CString	m_strGroup2;
	CString	m_strNextState;
	CTapiAddress* m_pAddr;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentStateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void OnAvailableGroupsChange();
	void OnAgentStateChange();
	void RecalcFeatures();
	LPCSTR GetAgentStateString(DWORD dwAgentState);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAgentStateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLogout();
	afx_msg void OnNotready();
	afx_msg void OnReady();
	afx_msg void OnSetGroup();
	afx_msg void OnCallWork();
	afx_msg LRESULT i_OnAgentChange(WPARAM, LPARAM);
	afx_msg void OnClose();
	afx_msg void OnUpdateS();
	afx_msg void OnSelchangeActivity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGENTSTATEDLG_H__56E41075_C59B_11D1_BBA9_006097D5EC19__INCLUDED_)
