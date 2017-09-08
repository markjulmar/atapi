// PhoneDlg.h : header file
//

#if !defined(AFX_PHONEDLG_H__FD6DFFAB_C258_11D1_BB9C_006097D5EC19__INCLUDED_)
#define AFX_PHONEDLG_H__FD6DFFAB_C258_11D1_BB9C_006097D5EC19__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CPhoneDlg dialog

class CPhoneDlg : public CDialog
{
// Construction
public:
	CPhoneDlg(CWnd* pParent = NULL);	// standard constructor
	void OnAgentClose(CTapiAddress* pAddr);

// Dialog Data
	//{{AFX_DATA(CPhoneDlg)
	enum { IDD = IDD_PHONE_DIALOG };
	CButton	m_btnQOS;
	CButton	m_btnISDN;
	CButton	m_btnConfig;
	CButton m_btnForward;
	CEdit	m_edtNumber;
	CButton	m_btnConfList;
	CButton	m_btnPark;
	CButton	m_btnCompleteCall;
	CButton	m_btnSwapHold;
	CButton	m_btnAccept;
	CComboBox	m_cbAddress;
	CButton	m_btnUserUserInfo;
	CButton	m_btnUnpark;
	CButton	m_btnTransfer;
	CButton	m_btnStartSession;
	CButton	m_btnSetupConf;
	CSpinButtonCtrl	m_ctlSession;
	CButton	m_btnRemoveFromConf;
	CButton	m_btnRedirect;
	CButton	m_btnPickup;
	CButton	m_btnPhoneInfo;
	CButton	m_btnMakeCall;
	CComboBox	m_cbLines;
	CButton	m_btnHold;
	CButton	m_btnDrop;
	CButton	m_btnDial;
	CButton	m_btnComplete;
	CSpinButtonCtrl	m_ctlCalls;
	CButton	m_btnCallData;
	CButton	m_btnAnswer;
	CButton	m_btnAgentInfo;
	CButton	m_btnAddToConf;
	CString	m_strOrigin;
	CString	m_strCallerID;
	CString	m_strCallID;
	CString m_strConnectedID;
	CString	m_strCallNumber;
	CString	m_strCallState;
	CString	m_strReason;
	CString	m_strMaxCalls;
	CString	m_strMediaMode;
	BOOL	m_fMsgWaiting;
	CString	m_strNumber;
	CString	m_strRedirectedNumber;
	CString	m_strSessionNum;
	CString	m_strLastStateChangeTime;
	CString	m_strTrunkID;
	BOOL	m_fLocked;
	CPtrArray m_arrSessions;
	int m_iSession;
	CPtrArray m_arrCalls;
	int m_iCall;
	CString	m_strCalledID;
	BOOL	m_fConnected;
	BOOL	m_fInService;
	CMapPtrToPtr m_mapAgentDlg;
	CString	m_strRelatedCallID;
	CString	m_strRedirectionNumber;
	CFont m_fntBold;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	void OnLoadLineInfo(CTapiLine* pLine);
	void NoLineSelected();
	void NoCallSelected();
	void RemoveCall(CTapiCall* pCall);
	void RemoveSession(CTapiLine* pLine);
	void UpdateSession(CTapiLine* pLine);
	void AddSession(CTapiLine* pLine);
	void EnableLineButtons(DWORD dwCaps);
	void MoveToCall(int iCall);
	void OnNewCall(CTapiCall* pCall);
	void OnCallChange(CTapiCall* pCall);
	void EnableCallButtons(DWORD dwFeatures);
	CTapiCall* GetActiveCall();
	CTapiLine* GetActiveLine();
	CTapiAddress* GetActiveAddress();

	// Generated message map functions
	//{{AFX_MSG(CPhoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeLine();
	afx_msg void OnStartSession();
	afx_msg void OnDeltaposSession(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddressChange();
	afx_msg void OnForward();
	afx_msg void OnChangeCall(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT i_OnCallChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT i_OnNewCall(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT i_OnAddressChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT i_OnLineChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT i_OnAgentChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnDropCall();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAcceptCall();
	afx_msg void OnAnswerCall();
	afx_msg void OnMakeCall();
	afx_msg void OnHoldCall();
	afx_msg void OnSwapHoldCall();
	afx_msg void OnCallData();
	afx_msg void OnUserUserInfo();
	afx_msg void OnDial();
	afx_msg void OnRedirectCall();
	afx_msg void OnUnparkCall();
	afx_msg void OnPickupCall();
	afx_msg void OnChangeLineStatus();
	afx_msg void OnAgentInfo();
	afx_msg void OnConferenceList();
	afx_msg void OnTransfer();
	afx_msg void OnParkCall();
	afx_msg void OnCompleteTransfer();
	afx_msg void OnCompleteCall();
	afx_msg void OnSetupConference();
	afx_msg void OnAddToConference();
	afx_msg void OnRemoveFromConference();
	afx_msg void OnPhoneInfo();
	afx_msg void OnConfig();
	afx_msg void OnClose();
	afx_msg void OnISDN();
	afx_msg void OnQos();
	afx_msg LRESULT OnFlashWindow(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONEDLG_H__FD6DFFAB_C258_11D1_BB9C_006097D5EC19__INCLUDED_)
