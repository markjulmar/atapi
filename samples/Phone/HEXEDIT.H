// HEXEDIT.H
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

#ifndef __HEXEDIT_INC__
#define __HEXEDIT_INC__

/****************************************************************************
** CHexEdit
**
** Hex editor class
**
*****************************************************************************/
class CHexEdit : public CEdit
{
// Class data
protected:
	LPVOID m_lpBuff;		// Buffer we are editing
	DWORD  m_dwSize;		// Size of the buffer
	CPoint m_ptCurPos;		// Current cursor position (in CHARACTERS)

// Constructor
public:
	CHexEdit();
	virtual ~CHexEdit();

// Methods
public:
	BOOL Init(HWND hwndEdit);
	void SetData(LPVOID lpvBuff, DWORD dwSize);
	void GetData(LPVOID* ppvBuff, DWORD* pdwSize);

// Internal functions
protected:
	void FormatEdit();

// Overrides
protected:
	// Generated message map functions
	//{{AFX_MSG(CHexEdit)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __HEXEDIT_INC__
