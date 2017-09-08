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

#include <stdafx.h>
#include "hexedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BYTES_PER_LINE		16
#define HEX_POSITION		9
#define FINAL_HEX_POSITION	51
#define ASCII_POSITION	(BYTES_PER_LINE + HEX_POSITION + BYTES_PER_LINE + 4)
#define LINE_LENGTH     (ASCII_POSITION + BYTES_PER_LINE + 2)

/////////////////////////////////////////////////////////////////////////////
// Message maps (required by MFC)
//
BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	//{{AFX_MSG_MAP(CHexEdit)
//	ON_WM_KEYDOWN()
//	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEdit::CHexEdit
//
// Constructor for the hex editor
//
CHexEdit::CHexEdit() : m_lpBuff(0), m_dwSize(0)
{
}// CHexEdit::CHexEdit

/////////////////////////////////////////////////////////////////////////////
// CHexEdit::~CHexEdit
//
// Destructor for the hex editor
//
CHexEdit::~CHexEdit()
{
	if (m_lpBuff != NULL)
		free(m_lpBuff);

}// CHexEdit::~CHexEdit

/////////////////////////////////////////////////////////////////////////////
// CHexEdit::Init
//
// Initialize our edit control - we must be connected to the real EDIT.
//
BOOL CHexEdit::Init(HWND hwndEdit)
{
	// Subclass the edit so we see all it's messages.
	if (!SubclassWindow(hwndEdit))
		return FALSE;

	// Must be multi-line edit
	ASSERT (GetStyle() & ES_MULTILINE);

	// Set a non-proportional font in-place.
	CFont fntFixed;
	fntFixed.CreateStockObject(ANSI_FIXED_FONT);
	SetFont(&fntFixed);

	// Force the edit to be READ-ONLY if we don't have a buffer.
	if (m_lpBuff == NULL)
	{
		SetReadOnly();
		SetWindowText("");
	}
	else
		FormatEdit();

	return TRUE;

}// CHexEdit::Init

/////////////////////////////////////////////////////////////////////////////
// CHexEdit::SetData
//
// Set the data for a control
//
void CHexEdit::SetData(LPVOID lpvBuff, DWORD dwSize)
{
	if (m_lpBuff != NULL)
		free(m_lpBuff);

	if (lpvBuff && dwSize > 0)
	{
		m_lpBuff = malloc(dwSize);
		if (m_lpBuff != NULL)
		{
			memcpy (m_lpBuff, lpvBuff, dwSize);
			m_dwSize = dwSize;
			FormatEdit();
		}
	}
	else
	{
		SetReadOnly(TRUE);
		m_lpBuff = NULL;
		m_dwSize = 0;
		SetWindowText("");
	}

}// CHexEdit::SetData

/////////////////////////////////////////////////////////////////////////////
// CHexEdit::FormatEdit
//
// Format the given buffer into HEX/ASCII form.
//
void CHexEdit::FormatEdit()
{
	if (GetSafeHwnd() == NULL)
		return;

    LPBYTE lpByte = (LPBYTE) m_lpBuff;
    DWORD dwCount = 0, dwLine = 0;
    char b[17], szBuff[256];
	CString strEdit;

    while (dwCount < m_dwSize)
    {
		// If this is a second line, append CRLF to force BREAK.
		if (strEdit.IsEmpty() == FALSE)
			strEdit += "\r\n";

        // Grab this portion of the buffer.
        for (int i = 0; i < BYTES_PER_LINE; i++)
        {
            if (m_dwSize-dwCount > 0)
            {
                b[i] = *lpByte++;
                dwCount++;
            }
            else
                b[i] = 0;
        }

        // Fill in the HEX portion
		memset (szBuff, ' ', sizeof(szBuff));
        wsprintf (szBuff, "%0.8lX %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X "\
                                 "%0.2X %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X", 
				dwLine,
                (int)b[0]&0xff, (int)b[1]&0xff, (int)b[2]&0xff, (int)b[3]&0xff, 
				(int)b[4]&0xff, (int)b[5]&0xff, (int)b[6]&0xff, (int)b[7]&0xff, 
				(int)b[8]&0xff, (int)b[9]&0xff, (int)b[10]&0xff,(int)b[11]&0xff, 
				(int)b[12]&0xff,(int)b[13]&0xff,(int)b[14]&0xff,(int)b[15]&0xff);
        dwLine = dwCount;
                      
        // Now do the ASCII portion.
        for (i = 0; i < BYTES_PER_LINE; i++)
        {
            if (!isprint(b[i]))
                b[i] = '.';
        }
        
        b[BYTES_PER_LINE] = _T('\0');
        strcpy(&szBuff[ASCII_POSITION], b);
		strEdit += szBuff;
    }

	SetWindowText(strEdit);

}// CHexEdit::FormatEdit
