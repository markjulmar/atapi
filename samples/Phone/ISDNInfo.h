// ISDNInfo.h : header file
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

#if !defined(AFX_ISDNINFO_H__91236516_B667_11D2_AD1C_BE6AD8000000__INCLUDED_)
#define AFX_ISDNINFO_H__91236516_B667_11D2_AD1C_BE6AD8000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hexedit.h"

/////////////////////////////////////////////////////////////////////////////
// CISDNInfo dialog

class CISDNInfo : public CDialog
{
// Construction
public:
	CISDNInfo(CWnd* pParent, CTapiCall* pCall);

// Dialog Data
	//{{AFX_DATA(CISDNInfo)
	enum { IDD = IDD_ISDNINFO };
	CTapiCall* m_pCall;
	CHexEdit m_edtHex1;
	CHexEdit m_edtHex2;
	CHexEdit m_edtHex3;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CISDNInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CISDNInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISDNINFO_H__91236516_B667_11D2_AD1C_BE6AD8000000__INCLUDED_)
