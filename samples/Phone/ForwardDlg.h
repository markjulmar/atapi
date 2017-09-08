// ForwardDlg.h : header file
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

#if !defined(AFX_FORWARDDLG_H__CBE48332_5CDC_11D3_A782_006097D5E97D__INCLUDED_)
#define AFX_FORWARDDLG_H__CBE48332_5CDC_11D3_A782_006097D5E97D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTapiLine;

/////////////////////////////////////////////////////////////////////////////
// CForwardDlg dialog

class CForwardDlg : public CDialog
{
// Construction
public:
	CForwardDlg(CWnd* pParent, CTapiLine* pLine);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CForwardDlg)
	enum { IDD = IDD_FORWARD };
	CButton	m_btnOK;
	CSpinButtonCtrl	m_ctlSpin;
	CComboBox	m_cbFwdModes;
	CComboBox	m_cbAddress;
	BOOL	m_bAllAddresses;
	BOOL    m_fSupportsDND;
	CString	m_strCaller;
	CString	m_strDest;
	UINT	m_iNumRings;
	CTapiLine* m_pLine;
	UINT m_iMinRings;
	UINT m_iMaxRings;
	DWORD m_dwAddress;
	DWORD m_dwFwdMode;
	DWORD m_dwAvailFwdModes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CForwardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CForwardDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChange();
	afx_msg void OnAllAddresses();
	afx_msg void OnSelchangeFwdmodes();
	afx_msg void OnAddressChange();
	afx_msg void OnUnforward();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORWARDDLG_H__CBE48332_5CDC_11D3_A782_006097D5E97D__INCLUDED_)
