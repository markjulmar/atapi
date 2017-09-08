// PhoneCapsDlg.cpp : implementation file
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
#include "phone.h"
#include "PhoneCapsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCSTR g_pszButtonModes[] = {
	"Dummy ",
	"Call ",
	"Feature ",
	"KeyPad ",
	"Local ",
	"Display "
};

LPCSTR g_pszButtonFunctions[] = {
	"Unknown",
	"Conference",
	"Transfer",
	"Drop",
	"Hold",
	"Recall",
	"Disconnect",
	"Connect",
	"MsgWaitOn",
	"MsgWaitOff",
	"SelectRing",
	"AbbrevDial",
	"Forward",
	"Pickup",
	"RingAgain",
	"Park",
	"Reject",
	"Redirect",
	"Mute",
	"Volume Up",
	"Volume Down",
	"Speaker On",
	"Speaker Off",
	"Flash",
	"Data On",
	"Data Off",
	"Do Not Disturb",
	"Intercom",
	"BridgedApp",
	"Busy",
	"Call Appearance",
	"Date/Time",
	"Directory",
	"Cover",
	"CallID",
	"Last Number",
	"Night Service",
	"Send Calls",
	"Msg Indicator",
	"Rep Dial",
	"SetRep Dial",
	"System Speed Dial",
	"Station Speed Dial"
	"Camp On",
	"Save Repeat",
	"Queue Call",
	"None"
};

LPCSTR g_pszLampModes[] = {
	"Dummy ",
	"Off ",
	"Steady ",
	"Wink ",
	"Flash ",
	"Flutter ",
	"Broken Flutter ",
	"Unknown "
};

/////////////////////////////////////////////////////////////////////////////
// CPhoneCapsDlg dialog

CPhoneCapsDlg::CPhoneCapsDlg(CWnd* pParent, CTapiPhone* pPhone)
	: CDialog(CPhoneCapsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhoneCapsDlg)
	m_iHandset = 0;
	m_iHeadset = 0;
	m_iSpeaker = 0;
	m_pPhone = pPhone;
	m_strDisplayXY = _T("");
	m_strButtonFunction = _T("");
	m_strButtonMode = _T("");
	m_strButtonText = _T("");
	m_fConnected = FALSE;
	m_strLampMode = _T("");
	m_fSuspended = FALSE;
	//}}AFX_DATA_INIT

	// Create the dialog
	Create(CPhoneCapsDlg::IDD, pParent);
}

void CPhoneCapsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhoneCapsDlg)
	DDX_Control(pDX, IDC_VOLUME_RINGER, m_ctlVolRinger);
	DDX_Control(pDX, IDC_RINGMODES, m_cbRingModes);
	DDX_Control(pDX, IDC_CONFIGURE, m_btnConfig);
	DDX_Control(pDX, IDC_BUTTONS, m_lbButtons);
	DDX_Control(pDX, IDC_DISPLAY, m_edtDisplay);
	DDX_Control(pDX, IDC_VOLUME_SPEAKER, m_ctlVolSpeaker);
	DDX_Control(pDX, IDC_VOLUME_HEADSET, m_ctlVolHeadset);
	DDX_Control(pDX, IDC_VOLUME_HANDSET, m_ctlVolHandset);
	DDX_Control(pDX, IDC_SETDISPLAY, m_btnSetDisplay);
	DDX_Control(pDX, IDC_GETDISPLAY, m_btnGetDisplay);
	DDX_Control(pDX, IDC_GAIN_SPEAKER, m_ctlGainSpeaker);
	DDX_Control(pDX, IDC_GAIN_HEADSET, m_ctlGainHeadset);
	DDX_Control(pDX, IDC_GAIN_HANDSET, m_ctlGainHandset);
	DDX_Radio(pDX, IDC_ONHOOK_HANDSET, m_iHandset);
	DDX_Radio(pDX, IDC_ONHOOK_HEADSET, m_iHeadset);
	DDX_Radio(pDX, IDC_ONHOOK_SPEAKER, m_iSpeaker);
	DDX_Text(pDX, IDC_DISPXY, m_strDisplayXY);
	DDX_Text(pDX, IDC_BUTTONFUNCTION, m_strButtonFunction);
	DDX_Text(pDX, IDC_BUTTONMODE, m_strButtonMode);
	DDX_Text(pDX, IDC_BUTTONTEXT, m_strButtonText);
	DDX_Check(pDX, IDC_CONNECTED, m_fConnected);
	DDX_Text(pDX, IDC_LAMPMODE, m_strLampMode);
	DDX_Check(pDX, IDC_SUSPENDED, m_fSuspended);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhoneCapsDlg, CDialog)
	//{{AFX_MSG_MAP(CPhoneCapsDlg)
	ON_EN_CHANGE(IDC_DISPLAY, OnChangeDisplay)
	ON_BN_CLICKED(IDC_GETDISPLAY, OnGetDisplay)
	ON_BN_CLICKED(IDC_SETDISPLAY, OnSetDisplay)
	ON_BN_CLICKED(IDC_MIC_HANDSET, OnChangeHandset)
	ON_BN_CLICKED(IDC_MIC_HEADSET, OnChangeHeadset)
	ON_BN_CLICKED(IDC_MIC_SPEAKER, OnChangeSpeaker)
	ON_WM_HSCROLL()
	ON_LBN_SELCHANGE(IDC_BUTTONS, OnChangeButton)
	ON_BN_CLICKED(IDC_CONFIGURE, OnConfigure)
	ON_WM_CLOSE()
	ON_MESSAGE(UM_PHONECHANGE, OnDevStateChange)
	ON_CBN_SELCHANGE(IDC_RINGMODES, OnSelchangeRingmodes)
	ON_BN_CLICKED(IDC_MICSPKR_HANDSET, OnChangeHandset)
	ON_BN_CLICKED(IDC_ONHOOK_HANDSET, OnChangeHandset)
	ON_BN_CLICKED(IDC_SPKR_HANDSET, OnChangeHandset)
	ON_BN_CLICKED(IDC_MICSPKR_HEADSET, OnChangeHeadset)
	ON_BN_CLICKED(IDC_ONHOOK_HEADSET, OnChangeHeadset)
	ON_BN_CLICKED(IDC_SPKR_HEADSET, OnChangeHeadset)
	ON_BN_CLICKED(IDC_MICSPKR_SPEAKER, OnChangeSpeaker)
	ON_BN_CLICKED(IDC_ONHOOK_SPEAKER, OnChangeSpeaker)
	ON_BN_CLICKED(IDC_SPKR_SPEAKER, OnChangeSpeaker)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneCapsDlg message handlers

BOOL CPhoneCapsDlg::OnInitDialog() 
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

	// Change our titlebar
	CString strWindow;
	GetWindowText(strWindow);
	strWindow += m_pPhone->GetPhoneName();
	SetWindowText(strWindow);

	// Connect all the controls via DDX
	CDialog::OnInitDialog();

	// Set the font for the display to be FIXED.
	CFont fntAnsiFixed;
    fntAnsiFixed.CreateStockObject (ANSI_FIXED_FONT);
	m_edtDisplay.SetFont(&fntAnsiFixed);

	InitPhone();

	return TRUE;
}

void CPhoneCapsDlg::InitPhone()
{
	m_ctlVolRinger.SetRange(0, 0xffff);
	m_ctlVolHandset.SetRange(0, 0xffff);
	m_ctlVolHeadset.SetRange(0, 0xffff);
	m_ctlVolSpeaker.SetRange(0, 0xffff);
	m_ctlGainHandset.SetRange(0, 0xffff);
	m_ctlGainHeadset.SetRange(0, 0xffff);
	m_ctlGainSpeaker.SetRange(0, 0xffff);

	// Enable/Disable the appropriate controls
	LPPHONECAPS lpCaps = m_pPhone->GetPhoneCaps();
	if (lpCaps != NULL)
	{
		m_cbRingModes.ResetContent();
		for (unsigned int i = 0; i < lpCaps->dwNumRingModes; i++)
		{
			CString strRingMode;
			strRingMode.Format(_T("Ring Mode #%d"), i+1);
			m_cbRingModes.AddString(strRingMode);
		}
		m_cbRingModes.EnableWindow((lpCaps->dwNumRingModes > 0));

		if ((lpCaps->dwHookSwitchDevs & PHONEHOOKSWITCHDEV_HANDSET) == 0)
		{
			GetDlgItem(IDC_ONHOOK_HANDSET)->EnableWindow(FALSE);
			GetDlgItem(IDC_MIC_HANDSET)->EnableWindow(FALSE);
			GetDlgItem(IDC_MICSPKR_HANDSET)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPKR_HANDSET)->EnableWindow(FALSE);
		}
		else
		{
			if ((lpCaps->dwHandsetHookSwitchModes & PHONEHOOKSWITCHMODE_ONHOOK) == 0)
				GetDlgItem(IDC_ONHOOK_HANDSET)->EnableWindow(FALSE);
			if ((lpCaps->dwHandsetHookSwitchModes & PHONEHOOKSWITCHMODE_MIC) == 0)
				GetDlgItem(IDC_MIC_HANDSET)->EnableWindow(FALSE);
			if ((lpCaps->dwHandsetHookSwitchModes & PHONEHOOKSWITCHMODE_SPEAKER) == 0)
				GetDlgItem(IDC_SPKR_HANDSET)->EnableWindow(FALSE);
			if ((lpCaps->dwHandsetHookSwitchModes & PHONEHOOKSWITCHMODE_MICSPEAKER) == 0)
				GetDlgItem(IDC_MICSPKR_HANDSET)->EnableWindow(FALSE);
		}

		if ((lpCaps->dwHookSwitchDevs & PHONEHOOKSWITCHDEV_SPEAKER) == 0)
		{
			GetDlgItem(IDC_ONHOOK_SPEAKER)->EnableWindow(FALSE);
			GetDlgItem(IDC_MIC_SPEAKER)->EnableWindow(FALSE);
			GetDlgItem(IDC_MICSPKR_SPEAKER)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPKR_SPEAKER)->EnableWindow(FALSE);
		}
		else
		{
			if ((lpCaps->dwSpeakerHookSwitchModes & PHONEHOOKSWITCHMODE_ONHOOK) == 0)
				GetDlgItem(IDC_ONHOOK_SPEAKER)->EnableWindow(FALSE);
			if ((lpCaps->dwSpeakerHookSwitchModes & PHONEHOOKSWITCHMODE_MIC) == 0)
				GetDlgItem(IDC_MIC_SPEAKER)->EnableWindow(FALSE);
			if ((lpCaps->dwSpeakerHookSwitchModes & PHONEHOOKSWITCHMODE_SPEAKER) == 0)
				GetDlgItem(IDC_MICSPKR_SPEAKER)->EnableWindow(FALSE);
			if ((lpCaps->dwSpeakerHookSwitchModes & PHONEHOOKSWITCHMODE_MICSPEAKER) == 0)
				GetDlgItem(IDC_SPKR_SPEAKER)->EnableWindow(FALSE);
		}

		if ((lpCaps->dwHookSwitchDevs & PHONEHOOKSWITCHDEV_HEADSET) == 0)
		{
			GetDlgItem(IDC_ONHOOK_HEADSET)->EnableWindow(FALSE);
			GetDlgItem(IDC_MIC_HEADSET)->EnableWindow(FALSE);
			GetDlgItem(IDC_MICSPKR_HEADSET)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPKR_HEADSET)->EnableWindow(FALSE);
		}
		else
		{
			if ((lpCaps->dwHeadsetHookSwitchModes & PHONEHOOKSWITCHMODE_ONHOOK) == 0)
				GetDlgItem(IDC_ONHOOK_HEADSET)->EnableWindow(FALSE);
			if ((lpCaps->dwHeadsetHookSwitchModes & PHONEHOOKSWITCHMODE_MIC) == 0)
				GetDlgItem(IDC_MIC_HEADSET)->EnableWindow(FALSE);
			if ((lpCaps->dwHeadsetHookSwitchModes & PHONEHOOKSWITCHMODE_SPEAKER) == 0)
				GetDlgItem(IDC_SPKR_HEADSET)->EnableWindow(FALSE);
			if ((lpCaps->dwHeadsetHookSwitchModes & PHONEHOOKSWITCHMODE_MICSPEAKER) == 0)
				GetDlgItem(IDC_MICSPKR_HEADSET)->EnableWindow(FALSE);
		}

		if ((lpCaps->dwVolumeFlags & PHONEHOOKSWITCHDEV_HANDSET) == 0)
			m_ctlVolHandset.EnableWindow(FALSE);
		if ((lpCaps->dwVolumeFlags & PHONEHOOKSWITCHDEV_HEADSET) == 0)
			m_ctlVolHeadset.EnableWindow(FALSE);
		if ((lpCaps->dwVolumeFlags & PHONEHOOKSWITCHDEV_SPEAKER) == 0)
			m_ctlVolSpeaker.EnableWindow(FALSE);

		if ((lpCaps->dwGainFlags & PHONEHOOKSWITCHDEV_HANDSET) == 0)
			m_ctlGainHandset.EnableWindow(FALSE);
		if ((lpCaps->dwGainFlags & PHONEHOOKSWITCHDEV_HEADSET) == 0)
			m_ctlGainHeadset.EnableWindow(FALSE);
		if ((lpCaps->dwGainFlags & PHONEHOOKSWITCHDEV_SPEAKER) == 0)
			m_ctlGainSpeaker.EnableWindow(FALSE);

		if (lpCaps->dwNumButtonLamps > 0)
		{
			m_lbButtons.ResetContent();
			m_lbButtons.EnableWindow(TRUE);
			for (int i = 0; i < (int) lpCaps->dwNumButtonLamps; i++)
			{
				CString strButton;
				strButton.Format("Button %d", i);
				m_lbButtons.AddString(strButton);
			}
			m_lbButtons.SetCurSel(0);
			OnChangeButton();
		}
		else
			m_lbButtons.EnableWindow(FALSE);

		m_strDisplayXY.Format("%ld x %ld", lpCaps->dwDisplayNumColumns, lpCaps->dwDisplayNumRows);
		if (lpCaps->dwDisplayNumRows == 0 &&
			lpCaps->dwDisplayNumColumns == 0)
		{
			m_edtDisplay.EnableWindow(FALSE);
			m_btnSetDisplay.EnableWindow(FALSE);
			m_btnGetDisplay.EnableWindow(FALSE);
		}
		else
			OnGetDisplay();

		if ((lpCaps->dwPhoneFeatures & PHONEFEATURE_SETDISPLAY) != 0)
			m_edtDisplay.SetReadOnly(FALSE);
		else
			m_edtDisplay.SetReadOnly(TRUE);
	}

	// Now attempt to retrieve the phone status block.
	LPPHONESTATUS lpStatus = m_pPhone->GetPhoneStatus();
	if (lpStatus != NULL)
	{
		m_fConnected = ((lpStatus->dwStatusFlags & PHONESTATUSFLAGS_CONNECTED) != 0);
		m_fSuspended = ((lpStatus->dwStatusFlags & PHONESTATUSFLAGS_SUSPENDED) != 0);

		m_cbRingModes.SetCurSel(m_pPhone->GetPhoneStatus()->dwRingMode);

		switch (lpStatus->dwHandsetHookSwitchMode)
		{
			case PHONEHOOKSWITCHMODE_ONHOOK:		m_iHandset = 0; break;
			case PHONEHOOKSWITCHMODE_MIC:			m_iHandset = 1; break;
			case PHONEHOOKSWITCHMODE_SPEAKER:		m_iHandset = 3; break;
			case PHONEHOOKSWITCHMODE_MICSPEAKER:	m_iHandset = 2; break;
			default:								m_iHandset = -1; break;
		}

		switch (lpStatus->dwSpeakerHookSwitchMode)
		{
			case PHONEHOOKSWITCHMODE_ONHOOK:		m_iSpeaker = 0; break;
			case PHONEHOOKSWITCHMODE_MIC:			m_iSpeaker = 1; break;
			case PHONEHOOKSWITCHMODE_SPEAKER:		m_iSpeaker = 3; break;
			case PHONEHOOKSWITCHMODE_MICSPEAKER:	m_iSpeaker = 2; break;
			default:								m_iSpeaker = -1; break;
		}

		switch (lpStatus->dwHeadsetHookSwitchMode)
		{
			case PHONEHOOKSWITCHMODE_ONHOOK:		m_iHeadset = 0; break;
			case PHONEHOOKSWITCHMODE_MIC:			m_iHeadset = 1; break;
			case PHONEHOOKSWITCHMODE_SPEAKER:		m_iHeadset = 3; break;
			case PHONEHOOKSWITCHMODE_MICSPEAKER:	m_iHeadset = 2; break;
			default:								m_iHeadset = -1; break;
		}

		// Set the initial volume/gain.
		m_ctlVolHandset.SetPos(lpStatus->dwHandsetVolume);
		m_ctlVolHeadset.SetPos(lpStatus->dwHeadsetVolume);
		m_ctlVolSpeaker.SetPos(lpStatus->dwSpeakerVolume);
		m_ctlGainHandset.SetPos(lpStatus->dwHandsetGain);
		m_ctlGainHeadset.SetPos(lpStatus->dwHeadsetGain);
		m_ctlGainSpeaker.SetPos(lpStatus->dwSpeakerGain);
		m_ctlVolRinger.SetPos(lpStatus->dwRingVolume);

		if ((lpCaps->dwPhoneFeatures & PHONEFEATURE_SETRING) == 0)
		{
			m_ctlVolRinger.EnableWindow(FALSE);
			m_cbRingModes.EnableWindow(FALSE);
		}
	}

	UpdateData(FALSE);

}// CPhoneCapsDlg::InitPhone

void CPhoneCapsDlg::OnChangeDisplay() 
{
	m_btnSetDisplay.EnableWindow(TRUE);
}

void CPhoneCapsDlg::OnGetDisplay() 
{
	m_pPhone->GetPhoneStatus(TRUE);
	m_edtDisplay.SetWindowText(m_pPhone->GetDisplay());
	m_btnSetDisplay.EnableWindow(FALSE);
}

void CPhoneCapsDlg::OnSetDisplay() 
{
	CString strDisplay;
	m_edtDisplay.GetWindowText(strDisplay);
	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetDisplay(strDisplay));
	if (lResult != 0)
		ShowError("phoneSetDisplay", lResult);
	OnGetDisplay();
}

void CPhoneCapsDlg::OnChangeHandset() 
{
	int iOriginal = m_iHandset;
	UpdateData(TRUE);
	DWORD dwMode;
	switch (m_iHandset)
	{
		case 0: dwMode = PHONEHOOKSWITCHMODE_ONHOOK; break;
		case 1: dwMode = PHONEHOOKSWITCHMODE_MIC; break;
		case 2: dwMode = PHONEHOOKSWITCHMODE_MICSPEAKER; break;
		case 3: dwMode = PHONEHOOKSWITCHMODE_SPEAKER; break;
		default: dwMode = 0; break;
	}

	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetHandsetHookswitch(dwMode));
	if (lResult != 0)
	{
		ShowError("phoneSetHookSwitch", lResult);
		m_iHandset = iOriginal;
		UpdateData(FALSE);
	}
}

void CPhoneCapsDlg::OnChangeHeadset() 
{
	int iOriginal = m_iHeadset;
	UpdateData(TRUE);

	DWORD dwMode;
	switch (m_iHeadset)
	{
		case 0: dwMode = PHONEHOOKSWITCHMODE_ONHOOK; break;
		case 1: dwMode = PHONEHOOKSWITCHMODE_MIC; break;
		case 2: dwMode = PHONEHOOKSWITCHMODE_MICSPEAKER; break;
		case 3: dwMode = PHONEHOOKSWITCHMODE_SPEAKER; break;
		default: dwMode = 0; break;
	}

	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetHeadsetHookswitch(dwMode));
	if (lResult != 0)
	{
		ShowError("phoneSetHookSwitch", lResult);
		m_iHeadset = iOriginal;
		UpdateData(FALSE);
	}
}

void CPhoneCapsDlg::OnChangeSpeaker() 
{
	int iOriginal = m_iSpeaker;
	UpdateData(TRUE);
	DWORD dwMode;
	switch (m_iSpeaker)
	{
		case 0: dwMode = PHONEHOOKSWITCHMODE_ONHOOK; break;
		case 1: dwMode = PHONEHOOKSWITCHMODE_MIC; break;
		case 2: dwMode = PHONEHOOKSWITCHMODE_MICSPEAKER; break;
		case 3: dwMode = PHONEHOOKSWITCHMODE_SPEAKER; break;
		default: dwMode = 0; break;
	}

	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetSpeakerHookswitch(dwMode));
	if (lResult != 0)
	{
		ShowError("phoneSetHookSwitch", lResult);
		m_iSpeaker = iOriginal;
		UpdateData(FALSE);
	}
}

void CPhoneCapsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	if (pScrollBar->GetSafeHwnd() == m_ctlVolHandset.GetSafeHwnd())
		OnChangeVolumeHandset(nPos);
	else if (pScrollBar->GetSafeHwnd() == m_ctlVolHeadset.GetSafeHwnd())
		OnChangeVolumeHeadset(nPos);
	else if (pScrollBar->GetSafeHwnd() == m_ctlVolSpeaker.GetSafeHwnd())
		OnChangeVolumeSpeaker(nPos);
	else if (pScrollBar->GetSafeHwnd() == m_ctlGainHandset.GetSafeHwnd())
		OnChangeGainHandset(nPos);
	else if (pScrollBar->GetSafeHwnd() == m_ctlGainHeadset.GetSafeHwnd())
		OnChangeGainHeadset(nPos);
	else if (pScrollBar->GetSafeHwnd() == m_ctlGainSpeaker.GetSafeHwnd())
		OnChangeGainSpeaker(nPos);
	else if (pScrollBar->GetSafeHwnd() == m_ctlVolRinger.GetSafeHwnd())
		OnChangeVolumeRinger(nPos);
}

void CPhoneCapsDlg::OnChangeVolumeHandset(UINT nPos)
{
	nPos = m_ctlVolHandset.GetPos();
	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetHandsetVolume(nPos));
	if (lResult != 0)
	{
		ShowError("phoneSetVolume", lResult);
		m_ctlVolHandset.SetPos(m_pPhone->GetPhoneStatus()->dwHandsetVolume);
	}

}// CPhoneCapsDlg::OnChangeVolumeHandset

void CPhoneCapsDlg::OnChangeVolumeHeadset(UINT nPos)
{
	nPos = m_ctlVolHeadset.GetPos();
	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetHeadsetVolume(nPos));
	if (lResult != 0)
	{
		ShowError("phoneSetVolume", lResult);
		m_ctlVolHeadset.SetPos(m_pPhone->GetPhoneStatus()->dwHeadsetVolume);
	}

}// CPhoneCapsDlg::OnChangeVolumeHeadset

void CPhoneCapsDlg::OnChangeVolumeSpeaker(UINT nPos)
{
	nPos = m_ctlVolSpeaker.GetPos();
	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetSpeakerVolume(nPos));
	if (lResult != 0)
	{
		ShowError("phoneSetVolume", lResult);
		m_ctlVolSpeaker.SetPos(m_pPhone->GetPhoneStatus()->dwSpeakerVolume);
	}

}// CPhoneCapsDlg::OnChangeVolumeSpeaker

void CPhoneCapsDlg::OnChangeVolumeRinger(UINT nPos)
{
	nPos = m_ctlVolRinger.GetPos();
	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetRing(m_cbRingModes.GetCurSel(), nPos));
	if (lResult != 0)
	{
		ShowError("phoneSetRing", lResult);
		m_ctlVolRinger.SetPos(m_pPhone->GetPhoneStatus()->dwRingVolume);
	}

}// CPhoneCapsDlg::OnChangeVolumeHeadset

void CPhoneCapsDlg::OnChangeGainHandset(UINT nPos)
{
	nPos = m_ctlGainHandset.GetPos();
	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetHandsetGain(nPos));
	if (lResult != 0)
	{
		ShowError("phoneSetGain", lResult);
		m_ctlGainHandset.SetPos(m_pPhone->GetPhoneStatus()->dwHandsetGain);
	}

}// CPhoneCapsDlg::OnChangeGainHandset

void CPhoneCapsDlg::OnChangeGainHeadset(UINT nPos)
{
	nPos = m_ctlGainHeadset.GetPos();
	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetHeadsetGain(nPos));
	if (lResult != 0)
	{
		ShowError("phoneSetGain", lResult);
		m_ctlGainHeadset.SetPos(m_pPhone->GetPhoneStatus()->dwHeadsetGain);
	}

}// CPhoneCapsDlg::OnChangeGainHeadset

void CPhoneCapsDlg::OnChangeGainSpeaker(UINT nPos)
{
	nPos = m_ctlGainSpeaker.GetPos();
	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetSpeakerGain(nPos));
	if (lResult != 0)
	{
		ShowError("phoneSetGain", lResult);
		m_ctlGainSpeaker.SetPos(m_pPhone->GetPhoneStatus()->dwSpeakerGain);
	}

}// CPhoneCapsDlg::OnChangeGainSpeaker

void CPhoneCapsDlg::OnChangeButton() 
{
	int nCurSel = m_lbButtons.GetCurSel();
	if (nCurSel == LB_ERR)
	{
		m_strButtonFunction = _T("");
		m_strButtonMode = _T("");
		m_strButtonText = _T("");
		m_strLampMode = _T("");
	}
	else
	{
		BUTTONINFO Button;
		LONG lResult = m_pPhone->GetButtonInfo(nCurSel, &Button);
		if (lResult == 0)
		{
			m_strButtonText.Format("\"%s\"", Button.strButtonText);

			if (Button.dwButtonFunction > PHONEBUTTONFUNCTION_NONE)
				m_strButtonFunction.Format("0x%lx", Button.dwButtonFunction);
			else
				m_strButtonFunction = g_pszButtonFunctions[Button.dwButtonFunction];
			m_strButtonMode = _T("");

			int i = 0;
			while (Button.dwButtonMode > 0)
			{
				if (Button.dwButtonMode & 1)
					m_strButtonMode += g_pszButtonModes[i];
				Button.dwButtonMode = Button.dwButtonMode >> 1;
				i++;
			}
			
			DWORD dwLampMode = m_pPhone->GetLampMode(nCurSel);
			i = 0;
			m_strLampMode = _T("");
			while (dwLampMode > 0)
			{
				if (dwLampMode & 1)
					m_strLampMode += g_pszLampModes[i];
				dwLampMode = dwLampMode >> 1;
				i++;
			}
		}
		else
		{
			m_strButtonFunction = _T("");
			m_strButtonMode = _T("");
			m_strButtonText = _T("");
			m_strLampMode = _T("");
		}
	}
	UpdateData(FALSE);

}// CPhoneCapsDlg::OnChangeButton

void CPhoneCapsDlg::OnConfigure() 
{
	m_pPhone->Config(this, NULL);
	
}// CPhoneCapsDlg::OnConfigure

void CPhoneCapsDlg::OnClose() 
{
	m_pPhone->Close();
	((CMyPhone*)m_pPhone)->m_pPhone = NULL;
	CDialog::OnClose();
}

LRESULT CPhoneCapsDlg::OnDevStateChange(WPARAM wParam, LPARAM lParam)
{
	DWORD dwParam1 = (DWORD) wParam;
	DWORD dwParam2 = (DWORD) lParam;

	UpdateData(TRUE);

	// Retrieve the latest phone status structure
	const LPPHONESTATUS pStatus = m_pPhone->GetPhoneStatus(TRUE);

	// Look at all fields in case the provider merges more than
	// one notification flag into the structure.
	DWORD dwMask = 1;
	while (dwMask != 0x80000000)
	{
		if ((dwParam1 & dwMask) == dwMask)
			AdjustFlags(dwMask, dwParam2, pStatus);
		dwMask <<= 1;
	}

	UpdateData(FALSE);

	return 0;

}// CPhoneCapsDlg::OnDevStateChange

void CPhoneCapsDlg::AdjustFlags(DWORD dwParam1, DWORD dwParam2, const LPPHONESTATUS pStatus)
{
	switch (dwParam1)
	{
		case PHONESTATE_CONNECTED:		m_fConnected = true; break;
		case PHONESTATE_DISCONNECTED:	m_fConnected = false; break;
		case PHONESTATE_SUSPEND:		m_fSuspended = true; break;
		case PHONESTATE_RESUME:			m_fSuspended = false; break;
		case PHONESTATE_DISPLAY:		OnGetDisplay(); break;
		case PHONESTATE_HANDSETHOOKSWITCH:
			switch (pStatus->dwHandsetHookSwitchMode)
			{
				case PHONEHOOKSWITCHMODE_ONHOOK: m_iHandset = 0; break;
				case PHONEHOOKSWITCHMODE_MIC:	 m_iHandset = 1; break;
				case PHONEHOOKSWITCHMODE_MICSPEAKER: m_iHandset = 2; break;
				case PHONEHOOKSWITCHMODE_SPEAKER:	 m_iHandset = 3; break;
			}
			break;
		case PHONESTATE_SPEAKERHOOKSWITCH:
			switch (pStatus->dwSpeakerHookSwitchMode)
			{
				case PHONEHOOKSWITCHMODE_ONHOOK: m_iSpeaker = 0; break;
				case PHONEHOOKSWITCHMODE_MIC:	 m_iSpeaker = 1; break;
				case PHONEHOOKSWITCHMODE_MICSPEAKER: m_iSpeaker = 2; break;
				case PHONEHOOKSWITCHMODE_SPEAKER:	 m_iSpeaker = 3; break;
			}
			break;
		case PHONESTATE_HEADSETHOOKSWITCH:
			switch (pStatus->dwHeadsetHookSwitchMode)
			{
				case PHONEHOOKSWITCHMODE_ONHOOK: m_iHeadset = 0; break;
				case PHONEHOOKSWITCHMODE_MIC:	 m_iHeadset = 1; break;
				case PHONEHOOKSWITCHMODE_MICSPEAKER: m_iHeadset = 2; break;
				case PHONEHOOKSWITCHMODE_SPEAKER:	 m_iHeadset = 3; break;
			}
			break;

		case PHONESTATE_HANDSETVOLUME:
			m_ctlVolHandset.SetPos(pStatus->dwHandsetVolume);
			break;

		case PHONESTATE_HANDSETGAIN:
			m_ctlGainHandset.SetPos(pStatus->dwHandsetGain);
			break;

		case PHONESTATE_HEADSETVOLUME:
			m_ctlVolHeadset.SetPos(pStatus->dwHeadsetVolume);
			break;

		case PHONESTATE_HEADSETGAIN:
			m_ctlGainHeadset.SetPos(pStatus->dwHeadsetGain);
			break;

		case PHONESTATE_SPEAKERVOLUME:
			m_ctlVolSpeaker.SetPos(pStatus->dwSpeakerVolume);
			break;

		case PHONESTATE_SPEAKERGAIN:
			m_ctlGainSpeaker.SetPos(pStatus->dwSpeakerGain);
			break;

		case PHONESTATE_OTHER:
		case PHONESTATE_CAPSCHANGE:
			InitPhone();
			break;

		case PHONESTATE_REINIT:
			SendMessage(WM_CLOSE);
			break;

		case PHONESTATE_LAMP:
			if (dwParam2 == (DWORD) m_lbButtons.GetCurSel())
				OnChangeButton();
			break;
		
		case PHONESTATE_RINGMODE:
			m_cbRingModes.SetCurSel(pStatus->dwRingMode);
			break;

		case PHONESTATE_RINGVOLUME:
			m_ctlVolRinger.SetPos(pStatus->dwRingVolume);
			break;
	}

}// CPhoneCapsDlg::AdjustFlags

void CPhoneCapsDlg::OnSelchangeRingmodes() 
{
	int iCurSel = m_cbRingModes.GetCurSel();
	LONG lResult = GetTAPIConnection()->WaitForReply(m_pPhone->SetRing(iCurSel, m_pPhone->GetPhoneStatus()->dwRingVolume));
	if (lResult != 0)
	{
		ShowError("phoneSetRing", lResult);
		m_cbRingModes.SetCurSel(m_pPhone->GetPhoneStatus()->dwRingMode);
	}
}

void CPhoneCapsDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	delete this;
}
