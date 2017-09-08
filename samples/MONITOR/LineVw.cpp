// LineView.cpp : implementation of the CLineView class
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
#include "LineVw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Arrays used to query the line/address.  The string positions in 
// the arrays match the bit position for the respective field being
// queried.
const char * g_pszBearerModes[] = {
    { "Voice" }, {"Speech"}, {"MultiUse"}, {"Data"}, {"AltSpeechData"},
    { "NonCallSignaling"}, {"Passthrough"}, {"RestrictedData"}, NULL };

const char * g_pszMediaModes[] = {
    {""}, {"Unknown"}, {"InteractiveVoice"}, {"AutomatedVoice"},
    {"DataModem"}, {"G3FAX"}, {"TDD"}, {"G4FAX"},
    {"DigitalData"}, {"Teletex"}, {"VideoTex"},
    {"Telex"}, {"Mixed"}, {"ADSI"}, {"VoiceView"}, NULL };

const char * g_pszAddressModes[] = {
	{"AddressID"}, {"Dialable"}, NULL };

const char * g_pszAnswerModes[] = {
	{"None"}, {"Drop"}, {"Hold"}, NULL };


const char * g_pszDevCapFlags[] = {
	{"CrossAddrConference"}, {"HiLevelComp"}, {"LoLevelComp"},
	{"MediaControl"}, {"MultipleAddr"}, {"CloseDrop"},
	{"DialBilling"}, {"DialQuiet"}, {"WaitForDialTone"}, NULL };

const char * g_pszGenerateDigitModes[] = {
	{"Pulse"}, {"DTMF"}, {"DTMFEnd"}, NULL };

const char * g_pszGenerateToneModes[] = {
	{"Custom"}, {"RingBack"}, {"Busy"}, {"Beep"}, {"Billing"}, NULL };

const char * g_pszLineFeatures[] = {
	{"DevSpecific"}, {"DevSpecificFeature"}, {"Forward"},
	{"MakeCall"}, {"SetMediaCtl"}, {"SetTerminal"}, {"SetDevStatus"},
	{"ForwardUnconditional"}, {"DoNotDisturb"}, NULL };

const char * g_pszLineStates[] = {
	{"Other"}, {"Ringing"}, {"Connected"}, {"Disconnected"},
	{"MsgWaitOn"}, {"MsgWaitOff"}, {"InService"}, {"OutOfService"},
	{"Maintenance"}, {"Open"}, {"Close"}, {"NumCalls"}, {"NumCompletions"},
	{"Terminals"}, {"RoamMode"}, {"Battery"}, {"Signal"}, {"DevSpecific"},
	{"ReInit"}, {"Lock"}, {"CapsChange"}, {"ConfigChange"}, {"XlateChange"},
	{"ComplCancel"}, {"Removed"}, NULL };

/////////////////////////////////////////////////////////////////////////////
// CLineView

IMPLEMENT_DYNCREATE(CLineView, CFormView)

BEGIN_MESSAGE_MAP(CLineView, CFormView)
	//{{AFX_MSG_MAP(CLineView)
	ON_CBN_SELCHANGE(IDC_LINES, OnLineChange)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineView construction/destruction

CLineView::CLineView()
	: CFormView(CLineView::IDD)
{
	//{{AFX_DATA_INIT(CLineView)
	m_strPermLineID = _T("");
	m_strProviderName = _T("");
	m_strStringFormat = _T("");
	m_strSwitchInfo = _T("");
	m_strMaxActiveCalls = _T("");
	m_strMaxRate = _T("");
	m_strNumAddresses = _T("");
	m_strAddressModes = _T("");
	m_strAnswerModes = _T("");
	m_strBearerModes = _T("");
	m_strDeviceCapsFlags = _T("");
	m_strDigitGatherMaxTimeout = _T("");
	m_strDigitGatherMinTimeout = _T("");
	m_strGenDigitModes = _T("");
	m_strGenToneMax = _T("");
	m_strGenToneModes = _T("");
	m_strLineFeatures = _T("");
	m_strLineStates = _T("");
	m_strMediaCtlCallStateMax = _T("");
	m_strMediaCtlDigitMaxSize = _T("");
	m_strMediaCtlMediaMaxSize = _T("");
	m_strMediaCtlToneMaxSize = _T("");
	m_strMediaModes = _T("");
	m_strMonDigitModes = _T("");
	m_strMonToneMaxCount = _T("");
	m_strMonToneMaxFreq = _T("");
	m_strRingModes = _T("");
	m_strNegotiatedVer = _T("");
	m_strUUIAccept = _T("");
	m_strUUIAnswer = _T("");
	m_strUUICallInfo = _T("");
	m_strUUIDrop = _T("");
	m_strUUIMakeCall = _T("");
	m_strUUISendInfo = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CLineView::~CLineView()
{
}

void CLineView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineView)
	DDX_Control(pDX, IDC_TERMLIST, m_lbTerminals);
	DDX_Control(pDX, IDC_DEVCLASS, m_lbDevClass);
	DDX_Control(pDX, IDC_LINES, m_cbLines);
	DDX_Text(pDX, IDC_PERM_LINEID, m_strPermLineID);
	DDX_Text(pDX, IDC_PROVIDERNAME, m_strProviderName);
	DDX_Text(pDX, IDC_STRINGFORMAT, m_strStringFormat);
	DDX_Text(pDX, IDC_SWITCHINFO, m_strSwitchInfo);
	DDX_Text(pDX, IDC_MAX_CALLS, m_strMaxActiveCalls);
	DDX_Text(pDX, IDC_MAX_RATE, m_strMaxRate);
	DDX_Text(pDX, IDC_NUM_ADDRESSES, m_strNumAddresses);
	DDX_Text(pDX, IDC_ADDRESSMODES, m_strAddressModes);
	DDX_Text(pDX, IDC_ANSWERMODE, m_strAnswerModes);
	DDX_Text(pDX, IDC_BEARERMODES, m_strBearerModes);
	DDX_Text(pDX, IDC_DEVICECAPSFLAGS, m_strDeviceCapsFlags);
	DDX_Text(pDX, IDC_GATHER_MAX_TIMEOUT, m_strDigitGatherMaxTimeout);
	DDX_Text(pDX, IDC_GATHER_MIN_TIMEOUT, m_strDigitGatherMinTimeout);
	DDX_Text(pDX, IDC_GENDIGIT_MODES, m_strGenDigitModes);
	DDX_Text(pDX, IDC_GENTONE_MAX, m_strGenToneMax);
	DDX_Text(pDX, IDC_GENTONE_MODES, m_strGenToneModes);
	DDX_Text(pDX, IDC_LINEFEATURES, m_strLineFeatures);
	DDX_Text(pDX, IDC_LINESTATES, m_strLineStates);
	DDX_Text(pDX, IDC_MEDIACTL_CALLSTATE_MAX_SIZE, m_strMediaCtlCallStateMax);
	DDX_Text(pDX, IDC_MEDIACTL_DIGIT_MAX_SIZE, m_strMediaCtlDigitMaxSize);
	DDX_Text(pDX, IDC_MEDIACTL_MEDIA_MAX_SIZE, m_strMediaCtlMediaMaxSize);
	DDX_Text(pDX, IDC_MEDIACTL_TONE_MAX_SIZE, m_strMediaCtlToneMaxSize);
	DDX_Text(pDX, IDC_MEDIAMODES, m_strMediaModes);
	DDX_Text(pDX, IDC_MONDIGIT_MODES, m_strMonDigitModes);
	DDX_Text(pDX, IDC_MONTONE_MAX_COUNT, m_strMonToneMaxCount);
	DDX_Text(pDX, IDC_MONTONE_MAX_FREQ, m_strMonToneMaxFreq);
	DDX_Text(pDX, IDC_RINGMODES, m_strRingModes);
	DDX_Text(pDX, IDC_NEGTAPIVER, m_strNegotiatedVer);
	DDX_Text(pDX, IDC_UUI_ACCEPT, m_strUUIAccept);
	DDX_Text(pDX, IDC_UUI_ANSWER, m_strUUIAnswer);
	DDX_Text(pDX, IDC_UUI_CALLINFO, m_strUUICallInfo);
	DDX_Text(pDX, IDC_UUI_DROP, m_strUUIDrop);
	DDX_Text(pDX, IDC_UUI_MAKECALL, m_strUUIMakeCall);
	DDX_Text(pDX, IDC_UUI_SENDINFO, m_strUUISendInfo);
	//}}AFX_DATA_MAP
}

BOOL CLineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLineView diagnostics

#ifdef _DEBUG
void CLineView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLineView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMonitorDoc* CLineView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMonitorDoc)));
	return (CMonitorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLineView message handlers

void CLineView::OnInitialUpdate() 
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
    
    // Connect the dialog form up to our dialog controls.
    CFormView::OnInitialUpdate();

	// Add the providers to our combo box.
	CString strName;
	for (DWORD i = 0; i < GetTAPIConnection()->GetLineDeviceCount(); i++)
	{
		CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(i);
		strName.Format("0x%04x - %s", pLine->GetDeviceID(), pLine->GetLineName());
		int iPos = m_cbLines.AddString(strName);
		ASSERT (iPos >= 0);
		m_cbLines.SetItemData(iPos, i);
	}	

	if (m_cbLines.GetCount() > 0)
	{
		m_cbLines.SetCurSel(0);
		OnLineChange();
	}

	// Force the mainfram to resize around us.
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
	ResizeParentToFit(TRUE);

	GetParentFrame()->CenterWindow();
}

HBRUSH CLineView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == 65535)
		pDC->SetTextColor(RGB(0,0,255));
	return hbr;
}

void CLineView::OnLineChange() 
{
	int iPos = m_cbLines.GetCurSel();

	CMonitorDoc* pDoc = GetDocument();

	// Assign the new device.
	pDoc->m_dwLineDevice = m_cbLines.GetItemData(iPos);
	CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(pDoc->m_dwLineDevice);
	ASSERT (pLine != NULL);

	const LPLINEDEVCAPS pDevCaps = pLine->GetLineCaps();
	if (pDevCaps == NULL)
		return;

	m_strNegotiatedVer.Format("TAPI %x.%x", 
			HIWORD(pLine->GetNegotiatedAPIVersion()),
            LOWORD(pLine->GetNegotiatedAPIVersion()));

	m_strPermLineID.Format ("0x%lX", pDevCaps->dwPermanentLineID);
	m_strProviderName = pLine->GetProviderInfo();
	m_strSwitchInfo = pLine->GetSwitchInfo();

	switch (pDevCaps->dwStringFormat)
	{
		case STRINGFORMAT_ASCII: m_strStringFormat = "Ascii"; break;
		case STRINGFORMAT_DBCS:  m_strStringFormat = "DBCS"; break;
		case STRINGFORMAT_UNICODE: m_strStringFormat = "Unicode"; break;
		case STRINGFORMAT_BINARY: m_strStringFormat = "Binary"; break;
		default: m_strStringFormat.Format("0x%lx", pDevCaps->dwStringFormat); break;
	}

	m_strMaxActiveCalls.Format("%ld", pDevCaps->dwMaxNumActiveCalls);
	m_strMaxRate.Format("%ld", pDevCaps->dwMaxRate);
	m_strNumAddresses.Format("%ld", pDevCaps->dwNumAddresses);

	m_strBearerModes.Format("%s",
		pDoc->AddCapabilities (pDevCaps->dwBearerModes, g_pszBearerModes));
	m_strMediaModes.Format("%s",
		pDoc->AddCapabilities (pDevCaps->dwMediaModes, g_pszMediaModes));
	m_strAddressModes.Format("%s",
		pDoc->AddCapabilities (pDevCaps->dwAddressModes, g_pszAddressModes));

	m_strDeviceCapsFlags.Format("%s",
		pDoc->AddCapabilities(pDevCaps->dwDevCapFlags, g_pszDevCapFlags));

	m_strAnswerModes.Format("%s",
			pDoc->AddCapabilities (pDevCaps->dwAnswerMode, g_pszAnswerModes));
	m_strGenDigitModes.Format("%s",
		pDoc->AddCapabilities(pDevCaps->dwGenerateDigitModes, g_pszGenerateDigitModes));

	m_strDigitGatherMaxTimeout.Format("%ld", pDevCaps->dwGatherDigitsMaxTimeout);
	m_strDigitGatherMinTimeout.Format("%ld", pDevCaps->dwGatherDigitsMinTimeout);
	m_strGenToneMax.Format("%ld", pDevCaps->dwGenerateToneMaxNumFreq);
	m_strGenToneModes.Format("%s",
		pDoc->AddCapabilities(pDevCaps->dwGenerateToneModes, g_pszGenerateToneModes));
	m_strLineFeatures.Format("%s",
		pDoc->AddCapabilities(pDevCaps->dwLineFeatures, g_pszLineFeatures));
	m_strLineStates.Format("%s",
		pDoc->AddCapabilities(pDevCaps->dwLineStates, g_pszLineStates));

	m_strMonDigitModes.Format("%s",
		pDoc->AddCapabilities(pDevCaps->dwMonitorDigitModes, g_pszGenerateDigitModes));

	m_strMediaCtlCallStateMax.Format("%ld", pDevCaps->dwMedCtlCallStateMaxListSize);
	m_strMediaCtlDigitMaxSize.Format("%ld", pDevCaps->dwMedCtlDigitMaxListSize);
	m_strMediaCtlMediaMaxSize.Format("%ld", pDevCaps->dwMedCtlMediaMaxListSize);
	m_strMediaCtlToneMaxSize.Format("%ld", pDevCaps->dwMedCtlToneMaxListSize);
	m_strMonToneMaxCount.Format("%ld", pDevCaps->dwMonitorToneMaxNumEntries);
	m_strMonToneMaxFreq.Format("%ld", pDevCaps->dwMonitorToneMaxNumFreq);
	m_strRingModes.Format("%ld", pDevCaps->dwRingModes);

	m_strUUIAccept.Format("%ld", pDevCaps->dwUUIAcceptSize);
	m_strUUIAnswer.Format("%ld", pDevCaps->dwUUIAnswerSize);
	m_strUUICallInfo.Format("%ld", pDevCaps->dwUUICallInfoSize);
	m_strUUIDrop.Format("%ld", pDevCaps->dwUUIDropSize);
	m_strUUIMakeCall.Format("%ld", pDevCaps->dwUUIMakeCallSize);
	m_strUUISendInfo.Format("%ld", pDevCaps->dwUUISendUserUserInfoSize);

	// Now load the terminals.
	m_lbTerminals.ResetContent();
	if (pDevCaps->dwNumTerminals > 0)
	{
		LPCSTR pszText = (LPCSTR)(pDevCaps) + pDevCaps->dwTerminalTextOffset;
		for (DWORD i = 0; i < pDevCaps->dwNumTerminals; i++)
		{
			CString strText(pszText, pDevCaps->dwTerminalTextSize);
			m_lbTerminals.AddString(strText);
		}
	}

	// And the DEVCLASS information
#if (TAPI_CURRENT_VERSION >= 0x00020000)
	m_lbDevClass.ResetContent();
	if (pDevCaps->dwDeviceClassesSize > 0)
	{
		LPCSTR pszText = (LPCSTR)(pDevCaps) + pDevCaps->dwDeviceClassesOffset;
		while (*pszText != '\0')
		{
			m_lbDevClass.AddString(pszText);
			pszText += strlen(pszText)+1;
		}
	}
#endif
	UpdateData (FALSE);

	// Ask all other views to update.
	pDoc->UpdateAllViews(this, HINT_LINECHANGE);
}

