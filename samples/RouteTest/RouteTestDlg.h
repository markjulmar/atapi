// RouteTestDlg.h : header file
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

#if !defined(AFX_ROUTETESTDLG_H__7051298B_C4AD_11D1_BBA5_006097D5EC19__INCLUDED_)
#define AFX_ROUTETESTDLG_H__7051298B_C4AD_11D1_BBA5_006097D5EC19__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CRouteTestDlg dialog

class CRouteTestDlg : public CDialog
{
// Construction
public:
	CRouteTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRouteTestDlg)
	enum { IDD = IDD_ROUTETEST_DIALOG };
	CListBox	m_lbActive;
	CComboBox	m_cbRoutePoints;
	CButton	m_btnRemove;
	CListBox	m_lbLog;
	CButton	m_btnAdd;
	CString	m_strNumber;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRouteTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	void RouteCall(CTapiCall* pCall);
	void TraceMsg (const char * pszStr, ...);
	CString GetDN(int nCurSel);

	// Generated message map functions
	//{{AFX_MSG(CRouteTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnSelchangeRoutePoint();
	afx_msg void OnSelchangeActive();
	afx_msg void OnChangeNumber();
	afx_msg LRESULT OnNewCall(WPARAM wParam, LPARAM);
	afx_msg void OnClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUTETESTDLG_H__7051298B_C4AD_11D1_BBA5_006097D5EC19__INCLUDED_)
