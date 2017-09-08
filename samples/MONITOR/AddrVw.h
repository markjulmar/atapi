// AddrVw.h : header file
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

#if !defined(AFX_ADDRVW_H__9392BF97_5760_11D1_9C3D_006097D5E97D__INCLUDED_)
#define AFX_ADDRVW_H__9392BF97_5760_11D1_9C3D_006097D5E97D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "monitordoc.h"

/////////////////////////////////////////////////////////////////////////////
// CAddrView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CAddrView : public CFormView
{
protected:
	CAddrView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAddrView)

// Form Data
public:
	//{{AFX_DATA(CAddrView)
	enum { IDD = IDD_ADDR_FORM };
	CComboBox	m_cbAddress;
	//}}AFX_DATA

// Attributes
public:
	CMonitorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddrView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAddrView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CAddrView)
	afx_msg void OnAddressChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDRVW_H__9392BF97_5760_11D1_9C3D_006097D5E97D__INCLUDED_)
