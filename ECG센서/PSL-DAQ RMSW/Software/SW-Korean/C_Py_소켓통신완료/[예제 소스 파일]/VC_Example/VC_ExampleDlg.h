// VC_ExampleDlg.h : ��� ����
//

#pragma once

// *****
#include "PSL-DAQ_ExportHeader.h"

#include "afxwin.h"



// CVC_ExampleDlg ��ȭ ����
class CVC_ExampleDlg : public CDialog
{
// �����Դϴ�.
public:
	CVC_ExampleDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VC_EXAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	


	// *****
	// ��ġ �ڵ� 
	//
	UINT32 m_PSLDAQ_DeviceHandle;

	// *****
	// Dll ���� ǥ�� ListBox
	CListBox m_DllStatus;

	// *****
	// ���ŵ� ������ ǥ�ð� Edit
	CEdit m_Ch1ValueEdit, m_Ch2ValueEdit;


	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedAcquireStartButtion();
	afx_msg void OnBnClickedAcquireStopButtion();

	// *****
	// USB ������ ���� �Լ�
	// ����� ���� �޼��� ó��
	afx_msg LRESULT USB_ReceiveData(WPARAM WP,LPARAM LP);

	afx_msg void OnBnClickedDllStatusListClearButton();
};
