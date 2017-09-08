// HLINKCTL.CPP
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

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "stdafx.h"

#define INITGUID
#include "resource.h"
#include "hlinkctl.h"
#include <objbase.h>
#include <initguid.h>
#include <winnetwk.h>
#include <winnls.h>
#include <shlobj.h>
#include <intshcut.h>

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// CHLinkCtrl message map
/*---------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(CHLinkCtrl, CStatic)
	//{{AFX_MSG_MAP(CHLinkCtrl)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
** Procedure:  CHLinkCtrl::CHLinkCtrl
**
** Arguments: void
**
** Returns:    void
**
** Description:  Hyperlink control constructor
**
*****************************************************************************/
CHLinkCtrl::CHLinkCtrl() : m_clrNormal(RGB(0,0,255)), m_clrVisited(RGB(128,0,128)),
	m_clrHighlight(RGB(255,0,0)), m_stCurrent(Initial), m_stLast(Initial)
{
	m_hcursLink = AfxGetApp()->LoadCursor(IDC_HYPERHAND);

}// CHLinkCtrl::CHLinkCtrl

/*****************************************************************************
** Procedure:  CHLinkCtrl::~CHLinkCtrl
**
** Arguments: void
**
** Returns:    void
**
** Description:  Destructor for the hyperlink control
**
*****************************************************************************/
CHLinkCtrl::~CHLinkCtrl()
{
	DestroyCursor(m_hcursLink);

}// CHLinkCtrl::~CHLinkCtrl

/*****************************************************************************
** Procedure:  CHLinkCtrl::OnSetCursor
**
** Arguments: 'pwnd' - Window owner
**            'nHitTest' - Where the cursor is
**            'nMsg' - Message which caused this event
**
** Returns:    TRUE if the cursor was set
**
** Description:  Sets the current cursor for the control
**
*****************************************************************************/
BOOL CHLinkCtrl::OnSetCursor(CWnd* /*pwnd*/, UINT /*nHitTest*/, UINT /*nMsg*/) 
{
	if (m_hcursLink != NULL)
	{
  		::SetCursor(m_hcursLink);
		return TRUE;
	}
	return FALSE;

}// CHLinkCtrl::OnSetCursor

/*****************************************************************************
** Procedure:  CHLinkCtrl::OnLButtonDown
**
** Arguments: 'nFlags' - Control/Alt key flags
**            'pt' - Cursor point
**
** Returns:    void
**
** Description:  Manages the WM_LBUTTONDOWN event
**
*****************************************************************************/
void CHLinkCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint /*pt*/) 
{
	CWaitCursor cursor;

	// Get the link to test
	CString strLink;
	GetWindowText(strLink);

	// Spawn any .HTM handler installed
	HINSTANCE hInstance = ShellExecute(GetParent()->GetSafeHwnd(), 
		_T("open"), strLink, NULL, NULL, SW_SHOW);
	if (reinterpret_cast<LONG>(hInstance) < 32L)
		return;

	// Change our tag color
	m_stCurrent = Visited;
	Invalidate();

}// CHLinkCtrl::OnLButtonDown

/*****************************************************************************
** Procedure:  CHLinkCtrl::OnMouseMove
**
** Arguments: 'nFlags' - Control/Alt key flags
**            'pt' - Cursor point
**
** Returns:    void
**
** Description:  Manages the mouse movement over this control
**
*****************************************************************************/
void CHLinkCtrl::OnMouseMove(UINT nFlags, CPoint pt) 
{
	CRect rc;
	GetClientRect(rc);
	if (rc.PtInRect(pt))
	{
		if (m_stCurrent != Highlighted)
		{
			SetCapture();
			m_stLast = m_stCurrent;
			m_stCurrent = Highlighted;
			Invalidate();
		} 
	}
	else
	{
		if (m_stCurrent == Highlighted)
		{
			m_stCurrent = m_stLast;
			Invalidate();
			ReleaseCapture();
		}
	}
	CStatic::OnMouseMove(nFlags, pt);

}// CHLinkCtrl::OnMouseMove

/*****************************************************************************
** Procedure:  CHLinkCtrl::CtlColor
**
** Arguments: 'pDC' - Device context
**            'nCtlColor' - Type of color requested
**
** Returns:  Brush to paint background with
**
** Description:  Changes the text color based on the state of the link
**
*****************************************************************************/
HBRUSH CHLinkCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// Make sure we are notified about mouse events.
	DWORD dwStyle = GetStyle();
	if (!(dwStyle & SS_NOTIFY)) 
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	// Select the underline font
	if (m_fntLink.GetSafeHandle() == NULL)
	{
		LOGFONT lf;
		GetFont()->GetObject(sizeof(lf), &lf);
		lf.lfUnderline = TRUE;
		m_fntLink.CreateFontIndirect(&lf);
	}

	pDC->SelectObject(&m_fntLink);

	// Set the text color and paint mode
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor((m_stCurrent == Initial) ? m_clrNormal : 
					  (m_stCurrent == Visited) ? m_clrVisited : m_clrHighlight);

	// Return hollow brush since we don't paint background (parent does)
	return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);

}// CHLinkCtrl::CtlColor

