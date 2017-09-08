// DialDlg.cpp : implementation file
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
#include "Phone.h"
#include "DialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialDlg dialog


CDialDlg::CDialDlg(CWnd* pParent, CTapiLine* pLine, DWORD dwFeature)
	: CDialog(CDialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialDlg)
	m_strNumber = _T("");
	m_pLine = pLine;
	m_dwFeature = dwFeature;
	m_strInstr = _T("");
	//}}AFX_DATA_INIT
}

void CDialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialDlg)
	DDX_Control(pDX, IDC_NUMBER, m_edtNumber);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_TRANSLATE, m_btnTranslate);
	DDX_Text(pDX, IDC_NUMBER, m_strNumber);
	DDX_Text(pDX, IDC_INSTR, m_strInstr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialDlg, CDialog)
	//{{AFX_MSG_MAP(CDialDlg)
	ON_EN_CHANGE(IDC_NUMBER, OnChangeNumber)
	ON_BN_CLICKED(IDC_TRANSLATE, OnTranslate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialDlg message handlers

BOOL CDialDlg::OnInitDialog() 
{
    // Reset the font to all be ANSI var.
    CFont fntAnsi;
    fntAnsi.CreateStockObject (ANSI_VAR_FONT);
    CWnd* pwndChild = GetWindow (GW_CHILD);
    while (pwndChild != NULL && IsChild(pwndChild))
    {
        pwndChild->SetFont(&fntAnsi);
        pwndChild = pwndChild->GetWindow(GW_HWNDNEXT);
    }

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

	switch (m_dwFeature)
	{
		case LINECALLFEATURE_DIAL:  
			SetWindowText("Dial a Number");
			m_btnOK.SetWindowText("Dial");
			m_strInstr.LoadString(IDS_DIAL);
			m_edtNumber.LimitText(100);
			break;
		case LINECALLFEATURE_REDIRECT:
			SetWindowText("Redirect to Number");
			m_btnOK.SetWindowText("Redirect");
			m_strInstr.LoadString(IDS_REDIRECT);
			m_edtNumber.LimitText(6);
//			SetWindowLong(m_edtNumber.GetSafeHwnd(), GWL_STYLE, m_edtNumber.GetStyle() | ES_NUMBER);
			break;
		case LINECALLFEATURE_PARK:
			SetWindowText("Park a call");
			m_btnOK.SetWindowText("Park");
			m_strInstr.LoadString(IDS_PARK);
			m_edtNumber.LimitText(6);
//			SetWindowLong(m_edtNumber.GetSafeHwnd(), GWL_STYLE, m_edtNumber.GetStyle() | ES_NUMBER);
			break;
		default: ASSERT(FALSE); break;
	}

	if (m_dwFeature == LINECALLFEATURE_PARK)
		m_btnOK.EnableWindow(TRUE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialDlg::OnChangeNumber() 
{
	UpdateData(TRUE);
	m_btnOK.EnableWindow(!m_strNumber.IsEmpty() || m_dwFeature == LINECALLFEATURE_PARK);
	m_btnTranslate.EnableWindow(!m_strNumber.IsEmpty());
}

void CDialDlg::OnTranslate() 
{
	LPLINETRANSLATEOUTPUT lpto = (LPLINETRANSLATEOUTPUT) malloc(1024);
	lpto->dwTotalSize = 1024;

	LONG lResult = m_pLine->TranslateAddress(m_strNumber, 0, 0, lpto);
	if (lResult == 0)
		m_strNumber = (LPBYTE)lpto + lpto->dwDialableStringOffset;
	else
		ShowError("lineTranslateAddress", lResult);
	free(lpto);
}
