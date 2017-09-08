// PhoneCapsDlg.h : header file
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

#if !defined(AFX_PHONECAPSDLG_H__DC1CB9EA_C8C2_11D1_BBAB_006097D5EC19__INCLUDED_)
#define AFX_PHONECAPSDLG_H__DC1CB9EA_C8C2_11D1_BBAB_006097D5EC19__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CPhoneCapsDlg dialog

class CPhoneCapsDlg : public CDialog
{
// Construction
public:
	CPhoneCapsDlg(CWnd* pParent, CTapiPhone* pPhone);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPhoneCapsDlg)
	enum { IDD = IDD_PHONE };
	CSliderCtrl	m_ctlVolRinger;
	CComboBox	m_cbRingModes;
	CButton	m_btnConfig;
	CListBox	m_lbButtons;
	CEdit	m_edtDisplay;
	CSliderCtrl	m_ctlVolSpeaker;
	CSliderCtrl	m_ctlVolHeadset;
	CSliderCtrl	m_ctlVolHandset;
	CButton	m_btnSetDisplay;
	CButton	m_btnGetDisplay;
	CSliderCtrl	m_ctlGainSpeaker;
	CSliderCtrl	m_ctlGainHeadset;
	CSliderCtrl	m_ctlGainHandset;
	int		m_iHandset;
	int		m_iHeadset;
	int		m_iSpeaker;
	CTapiPhone* m_pPhone;
	CString	m_strDisplayXY;
	CString	m_strButtonFunction;
	CString	m_strButtonMode;
	CString	m_strButtonText;
	BOOL	m_fConnected;
	CString	m_strLampMode;
	BOOL	m_fSuspended;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneCapsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitPhone();
	void OnChangeVolumeHandset(UINT nPos);
	void OnChangeVolumeHeadset(UINT nPos);
	void OnChangeVolumeSpeaker(UINT nPos);
	void OnChangeGainHandset(UINT nPos);
	void OnChangeGainHeadset(UINT nPos);
	void OnChangeGainSpeaker(UINT nPos);
	void OnChangeVolumeRinger(UINT nPos);
	void AdjustFlags(DWORD dwParam1, DWORD dwParam2, const LPPHONESTATUS pStatus);

	// Generated message map functions
	//{{AFX_MSG(CPhoneCapsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDisplay();
	afx_msg void OnGetDisplay();
	afx_msg void OnSetDisplay();
	afx_msg void OnChangeHandset();
	afx_msg void OnChangeHeadset();
	afx_msg void OnChangeSpeaker();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeButton();
	afx_msg void OnConfigure();
	afx_msg void OnClose();
	afx_msg LRESULT OnDevStateChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeRingmodes();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONECAPSDLG_H__DC1CB9EA_C8C2_11D1_BBAB_006097D5EC19__INCLUDED_)
