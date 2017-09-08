// HLINKCTL.H
//
// Hyper-link static control
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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __HLINKCTRL_INC__
#define __HLINKCTRL_INC__

/**************************************************************************
** CHLinkCtrl
**
** Hyper link control class which overrides a static control
**
***************************************************************************/
class CHLinkCtrl : public CStatic
{
// Class data
protected:
	enum HState {
		Initial,
		Highlighted,
		Visited
	};

	HCURSOR  m_hcursLink;		// Hand cursor
	COLORREF m_clrNormal;		// Normal color
	COLORREF m_clrVisited;		// Visited color
	COLORREF m_clrHighlight;	// Highlight color
	HState m_stCurrent;			// Current state
	HState m_stLast;			// Previous state
	CFont m_fntLink;			// Underline font

public:
	//Constructors / Destructors
	CHLinkCtrl();
	virtual ~CHLinkCtrl();

protected:
	//{{AFX_VIRTUAL(CHLinkCtrl)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHLinkCtrl)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __HLINKCTRL_INC__