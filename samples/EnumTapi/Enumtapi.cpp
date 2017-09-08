// ENUMTAPI.CPP
//
// Enumerate all the available TAPI service providers and provide
// configuration and capabilities display.
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
#include "enumtapi.h"

// Dynamic object support
IMPLEMENT_DYNCREATE(CTestDocument, CDocument)
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
IMPLEMENT_DYNCREATE(CEnumTapiView, CFormView)

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Global variables

CEnumTapi NEAR theApp;                  // Required for MFC

const char * g_pszBearerModes[] = {
    { "Voice" }, {"Speech"}, {"MultiUse"}, {"Data"}, {"AltSpeechData"},
    { "NonCallSignaling"}, {"Passthrough"}, {"Restricted Data"}, NULL };

const char * g_pszMediaModes[] = {
    {""}, {"Unknown"}, {"InteractiveVoice"}, {"AutomatedVoice"},
    {"DataModem"}, {"G3FAX"}, {"TDD"}, {"G4FAX"},
    {"DigitalData"}, {"Teletex"}, {"VideoTex"},
    {"Telex"}, {"Mixed"}, {"ADSI"}, {"VoiceView"}, NULL };

const char * g_pszLineFeatures[] = {
	{"DeviceSpecific"}, {"DeviceSpecificFeature"}, {"Forward"}, 
	{"MakeCall"}, {"SetMediaControl"}, {"SetTerminal"}, 
	{"SetDeviceStatus"}, {"ForwardFWD"}, {"ForwardDND"}, NULL};

const char * g_pszAddressSharing[] = {
    {"Private"}, {"Bridged-Exclusive"}, {"Bridged-New"}, 
    {"Bridged-Shared"}, {"Monitored"}, NULL };

const char * g_pszCallFeatures[] = {
    {"Accept"}, {"AddToConf"}, {"Answer"}, {"BlindXfer"},
    {"CompleteCall"}, {"CompleteXfer"}, {"Dial"}, {"Drop"},
    {"GatherDigits"}, {"GenerateDigits"}, {"GenerateTone"},
    {"Hold"}, {"MonitorDigits"}, {"MonitorMedia"}, {"MonitorTones"},
    {"Park"}, {"PrepareAddToConf"}, {"Redirect"},
    {"RemoveFromConf"}, {"SecureCall"}, {"SendUserUserInfo"}, 
    {"SetCallParams"}, {"SetMediaControl"}, {"SetTerminal"},
    {"SetupConf"}, {"SetupXfer"}, {"SwapHold"}, {"UnHold"}, 
    {"ReleaseUserUserInfo"}, {"SetCallTreatment"}, {"SetQOS"}, 
	{"SetCallData"}, NULL };

const char * g_pszAddrFeatures[] = {
    {"Forward"}, {"MakeCall"}, {"Pickup"}, {"SetMediaControl"},
    {"SetTerminal"}, {"SetupConf"}, {"UncompleteCall"}, {"Unpark"},
	{"PickupHelp"}, {"PickupGroup"}, {"PickupDirect"}, {"PickupWaiting"},
	{"ForwardFWD"}, {"ForwardDND"}, NULL };

const char * g_pszAddrCaps[] = {
    {"Number of rings for \"No Answer\" may be specified on when forwarding calls.\r\n"},
    {"A group ID is required for call pickup.\r\n"},
    {"Calls on this address may be made secure at setup time.\r\n"},
    {"Caller ID information is blocked when placing calls.\r\n"},
    {"Caller ID block may be overriden.\r\n"},
    {"Destination addresses may be dialed on this address.\r\n"},
    {"The originating party's phone can automatically be taken off-hook when placing calls.\r\n"},
    {"The called party's phone can be forced offhook when making calls.\r\n"},
    {"Call forwarding requires a consultantation call.\r\n"},
    {"Conferences start with no initial call.\r\n"},
    {"Dropping a consultation call reconnects to the original party.\r\n"},
    {"The CompletionID information is unique.\r\n"},
    {"OnHold calls may be transferred.\r\n"},
    {"New calls may be transferred.\r\n"},
    {"OnHold calls may be conferenced.\r\n"},
    {"New calls may be conferenced into an existing conference.\r\n"},
    {"Partial Dialing is supported.\r\n"},
    {"Forwarding status is valid.\r\n"},
    {"Internal and external calls may be forwarded to different destinations.\r\n"},
    {"Busy and no-answer calls may be forwarded to different destinations.\r\n"},
    {"Offering calls must be accepted before station alert is active.\r\n"},
    {"Dropping a conference disconnects all parties involved.\r\n"},
    {"Calls detected through call-waiting may be picked up.\r\n"},
	{"Supports Predictive Dialer Capabilities.\r\n"},
	{"This is an ACD Queue.\r\n"}, 
	{"This is a Route Point.\r\n"},
	{"Placing a call onHold creates a new call.\r\n"},
	{"Internal calls are not supported.\r\n"},
	{"External calls are not supported.\r\n"},
	{"Calling Party can be set by application.\r\n"},
    NULL };
    
/////////////////////////////////////////////////////////////////////////////
// CEnumTapi::InitInstance
//
// Initialize the TAPI library and create the main window.
//
BOOL CEnumTapi::InitInstance()
{
#if _MSC_VER < 1300
    // Standard initialization
	Enable3dControls();
#endif

	InitCommonControls();

    // Initialize a connection with TAPI and determine if there 
    // are any TAPI complient devices installed.
    if (GetTAPIConnection()->Init ("ENUMTAPI") != 0 ||
        GetTAPIConnection()->GetLineDeviceCount() == 0)
    {
        AfxMessageBox ("There are no TAPI devices installed!");
        return FALSE;
    }

    // Register the document template
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(IDR_MAINFRAME, RUNTIME_CLASS(CTestDocument),
                            RUNTIME_CLASS(CMainFrame), RUNTIME_CLASS(CEnumTapiView));
    AddDocTemplate(pDocTemplate);

    // Create the SDI application and tell MFC to run a message loop.
    OnFileNew();
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEnumTapiView message map

BEGIN_MESSAGE_MAP(CEnumTapiView, CFormView)
    //{{AFX_MSG_MAP(CEnumTapiView)
    ON_BN_CLICKED(IDC_CONFIGURE, OnConfigure)
    ON_CBN_SELCHANGE(IDC_DEVICES, OnSelchangeDevices)
    ON_LBN_SELCHANGE(IDC_ADDRESSLIST, OnSelchangeAddresslist)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnumTapiView::CEnumTapiView
//
// Constructor for the view object
//
CEnumTapiView::CEnumTapiView(): CFormView(CEnumTapiView::IDD)
{
    m_strCaps = "";
    
}// CEnumTapiView::CEnumTapiView

/////////////////////////////////////////////////////////////////////////////
// CEnumTapiView::OnInitialUpdate
//
// Initial setup of the dialog
//
void CEnumTapiView::OnInitialUpdate()
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
    
    // Resize the dialog to fit.
    ResizeParentToFit();
    GetParentFrame()->RecalcLayout();
    CenterWindow();
                       
    // Connect the dialog form up to our dialog controls.
    CFormView::OnInitialUpdate();

    // Setup our column listbox with provider information.
    m_lbServiceProviders.SetFont (&fntAnsi);
    m_lbServiceProviders.InsertColumn (0, "ProviderID", LVCFMT_LEFT, 100);
	m_lbServiceProviders.InsertColumn (1, "Provider Name", LVCFMT_LEFT, 200);
    
    TAPIPROVIDER Provider;
    if (GetTAPIConnection()->GetFirstProvider(&Provider))
    {
        do
        {   
			LV_ITEM Item;
            CString strBuff;
            wsprintf (strBuff.GetBuffer(10), "0x%lX", Provider.dwPermanentProviderID);
            strBuff.ReleaseBuffer();

			Item.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
			Item.iItem = 0;
			Item.iSubItem = 0;
			Item.state = 0;
			Item.pszText = (LPSTR)(LPCSTR) strBuff;
			Item.lParam = (LONG) Provider.dwPermanentProviderID;
			
			int iPos = m_lbServiceProviders.InsertItem(&Item);
			ASSERT (iPos >= 0);
			
			m_lbServiceProviders.SetItemText (iPos, 1, Provider.strProviderName);
        
        } while (GetTAPIConnection()->GetNextProvider(&Provider));
    }

    // Now go through and enumerate all the line devices in the system.
    // These get placed into our combo box.
    for (DWORD dwCount = 0; 
		 dwCount < GetTAPIConnection()->GetLineDeviceCount(); 
		 dwCount++)
    {
        CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwCount);
        if (pLine)
        {
            int iPos = m_cbDevices.AddString (pLine->GetLineName());
            ASSERT (iPos >= 0);
            m_cbDevices.SetItemData(iPos, dwCount);
        }
    }
    
    // Automatically select the first entry in the combo box.
    if (m_cbDevices.GetCount() > 0)
    {
        m_cbDevices.SetCurSel(0);
        OnSelchangeDevices();
    }                             
    
}// CEnumTapiView::OnInitialUpdate

/////////////////////////////////////////////////////////////////////////////
// CEnumTapiView::DoDataExchange
//
// Dialog data exchange for the dialog.
//
void CEnumTapiView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEnumTapiView)
    DDX_Control(pDX, IDC_SERVICELIST, m_lbServiceProviders);
    DDX_Control(pDX, IDC_ADDRESSLIST, m_lbAddress);
    DDX_Control(pDX, IDC_DEVICES, m_cbDevices);
    DDX_Text (pDX, IDC_CAPS, m_strCaps);
    //}}AFX_DATA_MAP
    
}// CEnumTapiView::DoDataExchange

/////////////////////////////////////////////////////////////////////////////
// CEnumTapiView::OnConfigure
//
// Invoke the configuration dialog for a particular line device.
//
void CEnumTapiView::OnConfigure() 
{
    int iSel = m_cbDevices.GetCurSel();           
    if (iSel != LB_ERR)
    {
        DWORD dwData = m_cbDevices.GetItemData (iSel);
        CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwData);
        pLine->Config (this, NULL);    
    }        
    
}// CEnumTapiView::OnConfigure

/////////////////////////////////////////////////////////////////////////////
// CEnumTapiView::OnSelchangeDevices
//
// Selected device has changed.
//
void CEnumTapiView::OnSelchangeDevices() 
{   
    // Delete any address information present.
    m_lbAddress.ResetContent();
    int iSel = m_cbDevices.GetCurSel();           
    if (iSel != LB_ERR)
    {
        // Get the line connection object which is selected in the
        // combo box.
        DWORD dwData = m_cbDevices.GetItemData (iSel);
        CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwData);
        ASSERT (pLine != NULL);
        
        // Open the line as a monitor so we can query address information
        // and such.  It will automatically be closed by the library when
        // the destructor is called.
        if (pLine->Open (LINECALLPRIVILEGE_NONE, 0) == 0)
        {
            for (dwData = 0; dwData < pLine->GetAddressCount(); dwData++)
            {
                CTapiAddress* pAddr = pLine->GetAddress(dwData);
                if (pAddr)
                {
                    int iPos = m_lbAddress.AddString (pAddr->GetDialableAddress());
                    ASSERT (iPos >= 0);
                    m_lbAddress.SetItemData (iPos, dwData);
                }
            }
        }
        
        // Now determine which service provider is managing this device, and
        // select it in our list of providers.  We can only do this if the provider
		// supports the "tapi/provider" key.
		DWORD dwProviderID = pLine->GetProviderID();
		if (dwProviderID > 0)
		{
			for (int i = 0; i < m_lbServiceProviders.GetItemCount(); i++)
			{
				if (dwProviderID == m_lbServiceProviders.GetItemData(i))
					m_lbServiceProviders.SetItemState (i, LVIS_SELECTED, LVIS_SELECTED);
			}            
		}
    }
    
    // Set the selection to the first address if available.
    if (m_lbAddress.GetCount() > 0)
        m_lbAddress.SetCurSel(0);
    
    // Populate the edit with LINE/ADDRESS caps.
    SetupLineAddressCaps();

}// CEnumTapiView::OnSelchangeDevices

/////////////////////////////////////////////////////////////////////////////
// CEnumTapiView::SetupLineAddressCaps
//
// Grab the linecaps and address caps and put them into our edit
// control.
//
void CEnumTapiView::SetupLineAddressCaps()
{                                              
    CString strCR = "\r\n";
	CString strBuff;
    
	m_strCaps = "";
                                   
    int iSel = m_cbDevices.GetCurSel();           
    if (iSel != CB_ERR)
    {
        // Grab the line device capabilities from the line object.  This
        // will query TAPI for the current LINECAPS if necessary.  When this
        // call comes back, we will have the full LINEDEVCAPS retrieved.
        DWORD dwData = m_cbDevices.GetItemData (iSel);
        CTapiLine* pLine = GetTAPIConnection()->GetLineFromDeviceID(dwData);
        LPLINEDEVCAPS lpLineCaps = pLine->GetLineCaps();
        LPLINEDEVSTATUS lpLineStatus = pLine->GetLineStatus();
		if (lpLineCaps == NULL || lpLineStatus == NULL)
		{
			UpdateData(FALSE);
			return;
		}
        
        // Begin outputing our information.
        m_strCaps += "Negotiated Version: ";
        strBuff.Format("%x.%x\r\n", HIWORD(pLine->GetNegotiatedAPIVersion()),
                        LOWORD(pLine->GetNegotiatedAPIVersion()));
        m_strCaps += strBuff;                        
        
        m_strCaps += "Provider Info: " + pLine->GetProviderInfo() + strCR;
        m_strCaps += "Switch Info: " + pLine->GetSwitchInfo() + strCR;
        
        strBuff.Format("PermanentLineID = 0x%lX\r\n", lpLineCaps->dwPermanentLineID);
        m_strCaps += strBuff;
        
        strBuff.Format("Bearer Modes: (0x%lX)  ", lpLineCaps->dwBearerModes);
        m_strCaps += strBuff;
        AddCapabilities (lpLineCaps->dwBearerModes, g_pszBearerModes);
        
		strBuff.Format("Max Data Rate: %ld", lpLineCaps->dwMaxRate);
        m_strCaps += strBuff + strCR;
        
        strBuff.Format("Media Modes: (0x%lx)  ", lpLineCaps->dwMediaModes);
        m_strCaps += strBuff;
        AddCapabilities (lpLineCaps->dwMediaModes, g_pszMediaModes);
                                    
        // See which device classes are supported.
		CStringArray arrDevClass;
		pLine->GetValidIDs(arrDevClass);
        for (int i = 0; i < arrDevClass.GetSize(); i++)
        {
			if (i == 0)
				m_strCaps += "Supports: ";
			else
				m_strCaps += ", ";
			strBuff.Format("\"%s\"", arrDevClass[i]);
            m_strCaps += strBuff;
        }
		if (i > 0)
			m_strCaps += "\r\n";
        
        // Now pull the line device status from the line and see how many
        // calls are active/holding on the line.    
        int iCalls = (int)(lpLineStatus->dwNumActiveCalls +
                     lpLineStatus->dwNumOnHoldCalls +
                     lpLineStatus->dwNumOnHoldPendCalls);
        if (iCalls > 0)
		{
			strBuff.Format ("Line has %d active calls", iCalls);
			m_strCaps += strBuff;
		}

        if (lpLineStatus->dwOpenMediaModes > 0)
            m_strCaps += "Line has monitoring applications\r\n";
        
		// Add our features.
		strBuff.Format("Line Features (0x%lx) ", lpLineCaps->dwLineFeatures);
		m_strCaps += strBuff;
		AddCapabilities(lpLineCaps->dwLineFeatures, g_pszLineFeatures);

		strBuff.Format("Available Line Features (0x%lx) ", lpLineStatus->dwLineFeatures);
		m_strCaps += strBuff;
		AddCapabilities(lpLineStatus->dwLineFeatures, g_pszLineFeatures);

        // If we have an address selected, grab the information from the address
        // capabilities and status.
        iSel = m_lbAddress.GetCurSel();           
        if (iSel != LB_ERR)
        {
            dwData = m_lbAddress.GetItemData (iSel);
            CTapiAddress* pAddr = pLine->GetAddress(dwData);
            LPLINEADDRESSCAPS lpAddrCaps = pAddr->GetAddressCaps();
			if (lpAddrCaps != NULL)
			{
				m_strCaps += "Address Type: ";
				AddCapabilities (lpAddrCaps->dwAddressSharing, g_pszAddressSharing);
            
				strBuff.Format("Address Capability Flags (0x%lx)\r\n", lpAddrCaps->dwAddrCapFlags);
				m_strCaps += strBuff;
				AddCapabilities (lpAddrCaps->dwAddrCapFlags, g_pszAddrCaps);
                
				strBuff.Format("Max number of active calls: %ld", lpAddrCaps->dwMaxNumActiveCalls);
				m_strCaps += strBuff + strCR;
				strBuff.Format("Max number of held calls: %ld", (lpAddrCaps->dwMaxNumOnHoldCalls+lpAddrCaps->dwMaxNumOnHoldPendingCalls));
				m_strCaps += strBuff + strCR;
				strBuff.Format("Max number of conferenced calls: %ld", lpAddrCaps->dwMaxNumConference);
				m_strCaps += strBuff + strCR;
            
				strBuff.Format("Address Features: (0x%lx) ", lpAddrCaps->dwAddressFeatures);
				m_strCaps += strBuff;
				AddCapabilities (lpAddrCaps->dwAddressFeatures, g_pszAddrFeatures);
            
				strBuff.Format("Call Features: (0x%lx) ", lpAddrCaps->dwCallFeatures);
				m_strCaps += strBuff;
				AddCapabilities (lpAddrCaps->dwCallFeatures, g_pszCallFeatures);
            
				LPLINEADDRESSSTATUS lpAddrStatus = pAddr->GetAddressStatus();
				if (lpAddrStatus != NULL)
				{
					strBuff.Format("Number of stations using address: %ld", lpAddrStatus->dwNumInUse);
					m_strCaps += strBuff + strCR;
					strBuff.Format("Number of active calls: %ld", lpAddrStatus->dwNumActiveCalls);
					m_strCaps += strBuff + strCR;
					strBuff.Format("Number of held calls: %ld", (lpAddrStatus->dwNumOnHoldCalls+lpAddrStatus->dwNumOnHoldPendCalls));
					m_strCaps += strBuff + strCR;
            
					strBuff.Format("Available Address Features: (0x%lx) ", lpAddrStatus->dwAddressFeatures);
					m_strCaps += strBuff;
					AddCapabilities (lpAddrStatus->dwAddressFeatures, g_pszAddrFeatures);
				}
			}
        }
    }
    
    UpdateData (FALSE);   

}// CEnumTapiView::SetupLineAddressCaps

/////////////////////////////////////////////////////////////////////////////
// CEnumTapiView::OnSelchangeAddresslist
//
// The selected address on the line has changed.
// 
void CEnumTapiView::OnSelchangeAddresslist()
{                                        
    SetupLineAddressCaps();

}// CEnumTapiView::OnSelchangeAddresslist

/////////////////////////////////////////////////////////////////////////////
// CEnumTapiView::AddCapabilities
//
// Run through a bit field and an associated array and output which
// bits are set.
//
void CEnumTapiView::AddCapabilities(DWORD dwFlags, const char *pszArray[])
{           
    CString strSpace = " ";
                          
    for (int i = 0; pszArray[i] != NULL; i++)
    {
        BOOL fSet = (BOOL) (dwFlags & 0x1);
        dwFlags = dwFlags >> 1;
        if (fSet)     
        {
            if (strchr(pszArray[i],'\r') == NULL)               
                m_strCaps += strSpace + pszArray[i] + strSpace;
            else
                m_strCaps += pszArray[i];
        }                
    }

    if (m_strCaps.Right(2) != "\r\n")    
        m_strCaps += "\r\n";

}// CEnumTapiView::AddCapabilities
