// LineView.h : interface of the CLineView class
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

#if !defined(AFX_MONITORVIEW_H__9392BF8D_5760_11D1_9C3D_006097D5E97D__INCLUDED_)
#define AFX_MONITORVIEW_H__9392BF8D_5760_11D1_9C3D_006097D5E97D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "monitordoc.h"

class CLineView : public CFormView
{
protected: // create from serialization only
	CLineView();
	DECLARE_DYNCREATE(CLineView)

public:
	//{{AFX_DATA(CLineView)
	enum { IDD = IDD_LINE_FORM };
	CListBox	m_lbTerminals;
	CListBox	m_lbDevClass;
	CComboBox	m_cbLines;
	CString	m_strPermLineID;
	CString	m_strProviderName;
	CString	m_strStringFormat;
	CString	m_strSwitchInfo;
	CString	m_strMaxActiveCalls;
	CString	m_strMaxRate;
	CString	m_strNumAddresses;
	CString	m_strAddressModes;
	CString	m_strAnswerModes;
	CString	m_strBearerModes;
	CString	m_strDeviceCapsFlags;
	CString	m_strDigitGatherMaxTimeout;
	CString	m_strDigitGatherMinTimeout;
	CString	m_strGenDigitModes;
	CString	m_strGenToneMax;
	CString	m_strGenToneModes;
	CString	m_strLineFeatures;
	CString	m_strLineStates;
	CString	m_strMediaCtlCallStateMax;
	CString	m_strMediaCtlDigitMaxSize;
	CString	m_strMediaCtlMediaMaxSize;
	CString	m_strMediaCtlToneMaxSize;
	CString	m_strMediaModes;
	CString	m_strMonDigitModes;
	CString	m_strMonToneMaxCount;
	CString	m_strMonToneMaxFreq;
	CString	m_strRingModes;
	CString	m_strNegotiatedVer;
	CString	m_strUUIAccept;
	CString	m_strUUIAnswer;
	CString	m_strUUICallInfo;
	CString	m_strUUIDrop;
	CString	m_strUUIMakeCall;
	CString	m_strUUISendInfo;
	//}}AFX_DATA

// Attributes
public:
	CMonitorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLineView)
	afx_msg void OnLineChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MonitorView.cpp
inline CMonitorDoc* CLineView::GetDocument()
   { return (CMonitorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORVIEW_H__9392BF8D_5760_11D1_9C3D_006097D5E97D__INCLUDED_)
