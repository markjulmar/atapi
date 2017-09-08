// MonitorDoc.cpp : implementation of the CMonitorDoc class
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

#include "stdafx.h"
#include "Monitor.h"
#include "MonitorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitorDoc

IMPLEMENT_DYNCREATE(CMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMonitorDoc, CDocument)
	//{{AFX_MSG_MAP(CMonitorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitorDoc construction/destruction

CMonitorDoc::CMonitorDoc()
{
	m_dwLineDevice = 0;
}

CMonitorDoc::~CMonitorDoc()
{
}

BOOL CMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMonitorDoc serialization

void CMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMonitorDoc diagnostics

#ifdef _DEBUG
void CMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

CString CMonitorDoc::AddCapabilities(DWORD dwFlags, const char *pszArray[])
{           
    CString strCaps, strSpace = " ";

    for (int i = 0; pszArray[i] != NULL; i++)
    {
        BOOL fSet = (BOOL) (dwFlags & 0x1);
        dwFlags = dwFlags >> 1;
        if (fSet)     
        {
            if (strchr(pszArray[i],'\r') == NULL)
			{
				if (strCaps.IsEmpty() == FALSE)
					strCaps += strSpace;
				strCaps += pszArray[i] + strSpace;
			}
            else
                strCaps += pszArray[i];
        }                
    }

	if (strCaps.IsEmpty())
		strCaps += "None";

	return strCaps;

}
