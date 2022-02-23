// VC_ExampleDlg.h : 헤더 파일
//

#pragma once

// *****
#include "PSL-DAQ_ExportHeader.h"

#include "afxwin.h"



// CVC_ExampleDlg 대화 상자
class CVC_ExampleDlg : public CDialog
{
// 생성입니다.
public:
	CVC_ExampleDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VC_EXAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	


	// *****
	// 장치 핸들 
	//
	UINT32 m_PSLDAQ_DeviceHandle;

	// *****
	// Dll 상태 표시 ListBox
	CListBox m_DllStatus;

	// *****
	// 수신된 데이터 표시값 Edit
	CEdit m_Ch1ValueEdit, m_Ch2ValueEdit;


	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedAcquireStartButtion();
	afx_msg void OnBnClickedAcquireStopButtion();

	// *****
	// USB 데이터 수신 함수
	// 사용자 정의 메세지 처리
	afx_msg LRESULT USB_ReceiveData(WPARAM WP,LPARAM LP);

	afx_msg void OnBnClickedDllStatusListClearButton();
};
