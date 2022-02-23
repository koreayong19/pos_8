// VC_ExampleDlg.h : header file
//

#pragma once

// *****
#include "PSL-DAQ_ExportHeader.h"

#include "afxwin.h"



// CVC_ExampleDlg dialog
class CVC_ExampleDlg : public CDialog
{
// Construction
public:
	CVC_ExampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VC_EXAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	


	// *****
	// Device Handle
	//
	UINT32 m_PSLDAQ_DeviceHandle;

	// *****
	// ListBox Control to show DLL state
	CListBox m_DllStatus;

	// *****
	// Edit Control to show received Data
	CEdit m_Ch1ValueEdit, m_Ch2ValueEdit;


	// Creates message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedAcquireStartButtion();
	afx_msg void OnBnClickedAcquireStopButtion();

	// *****
	// function for processing user-defined message "WM_RECEIVE_DATA" 
	// 
	afx_msg LRESULT USB_ReceiveData(WPARAM WP,LPARAM LP);

	afx_msg void OnBnClickedDllStatusListClearButton();
};
