// VC_ExampleDlg.cpp : source file
//

#include "stdafx.h"
#include "VC_Example.h"
#include "VC_ExampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVC_ExampleDlg dialog




CVC_ExampleDlg::CVC_ExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVC_ExampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVC_ExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLL_STATUS_LIST, m_DllStatus);
	DDX_Control(pDX, IDC_CH1DATA_EDIT, m_Ch1ValueEdit);
	DDX_Control(pDX, IDC_CH2DATA_EDIT, m_Ch2ValueEdit);
}

BEGIN_MESSAGE_MAP(CVC_ExampleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ACQUIRE_START_BUTTION, &CVC_ExampleDlg::OnBnClickedAcquireStartButtion)
	ON_BN_CLICKED(IDC_ACQUIRE_STOP_BUTTION, &CVC_ExampleDlg::OnBnClickedAcquireStopButtion)

	// *****
	// User-Defined Message Handler for DLL
	ON_MESSAGE(WM_RECEIVE_DATA,USB_ReceiveData)

	ON_BN_CLICKED(IDC_DLL_STATUS_LIST_CLEAR_BUTTON, &CVC_ExampleDlg::OnBnClickedDllStatusListClearButton)
END_MESSAGE_MAP()



// If you add a minimize button to your dialog box, you will need the program source code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done by the framework.

void CVC_ExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls function to obtain the cursor to display while the user drags
//   the minimized window.
HCURSOR CVC_ExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//****************************************************************************
//
//
// Initialize : BME DAQ KIT Device Open  
//
//
//****************************************************************************

BOOL CVC_ExampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//   when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);		// Set big icon	
	SetIcon(m_hIcon, FALSE);	// Set small icon	




	// *****
	//
	// initialize DLL for PSL-DAQ and get device handle
	//  - if error returns, recommand to quit program by OnOK()
	//  - for reference, this function runs correctly after USB driver for PSL-DAQ is installed.  
	// 
	// if second parameter(DeviceName) is not empty string, connection will be tried and established only to the PSL-DAQ module with ID same as DeviceName.
	// DeviceName is useful to Open multiple PSL-DAQ Devices.
	// caution: if Worong DeviceName, it is impossible to open PSL-DAQ.
	//
	// to open multiple PSL-DAQ devices, multiple PSLDAQ_Open() are called to get unique DeviceHandle(third parameter) for each device.
	//
	if(PSLDAQ_Open(m_hWnd,NULL,&m_PSLDAQ_DeviceHandle))
	{
		AfxMessageBox(_T("Error: PSLDAQ_Open"),MB_OK | MB_ICONERROR);
		OnOK();		
	}


	return TRUE; 
}



//****************************************************************************
//
//
// Destruction window
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// *****
	//
	// Finishing DLL function(disconnecting communications and so on) 
	PSLDAQ_Close(m_PSLDAQ_DeviceHandle);
}


//****************************************************************************
//
//
// Button Click Message Processing: start data acquisition
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnBnClickedAcquireStartButtion()
{
	// *****
	//
	// Send Acquisition start command to PSL-DAQ Device  
	PSLDAQ_AcquireStart(m_PSLDAQ_DeviceHandle);		
}


//****************************************************************************
//
//
// Button Click Message Processing: stop data acquisition
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnBnClickedAcquireStopButtion()
{
	// *****
	//
	// Send Acquisition stop command to PSL-DAQ Device
	PSLDAQ_AcquireStop(m_PSLDAQ_DeviceHandle);		
}


//****************************************************************************
//
//
// Button Click Message Processing: Clear Dll State Showing List Control
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnBnClickedDllStatusListClearButton()
{
	m_DllStatus.ResetContent();
}




//****************************************************************************
//
//
// Processing of user-defined Mesage 'WM_RECEIVE_DATA' sended from DLL.
//  - processing the Communication Event from DLL 
//  - processing the acquired data 
//
//****************************************************************************
LRESULT CVC_ExampleDlg::USB_ReceiveData(WPARAM WP,LPARAM LP)
{
	static bool  RcvDataProcEnable=0;

	//-------------------------------------------------
	// *****
	// Processing the Event related with communication. 
	//
	//-------------------------------------------------
	if(WP)
	{
		//
		// USB Device is conneted(Detected) - ignorable Event. 
		//
		if(LP==LP_DEV_OPEN)
		{
			m_DllStatus.AddString(_T("* PSL-DAQ is Connected(USB Device is recognized)"));
			return 0;
		}


		//
		// Comunication is Initialized   
		//
		if (LP == LP_COMM_INIT)
		{
			m_DllStatus.AddString(_T("*** PSL-DAQ is Linked(the response of initialization)"));		
			((CButton *)GetDlgItem(IDC_CONNECTION_CHECK))->SetCheck(BST_CHECKED);

			// if need, get Device ID(Serial Number) 
			 char DeviceID[256];
			 
			 PSLDAQ_GetDeviceID(m_PSLDAQ_DeviceHandle, DeviceID);

			 CString st=_T("     : DeviceID=");
			 st+=DeviceID;
			 m_DllStatus.AddString(st);

			return 0;
		}	

		//
		// Device USB connection is disconnected
		//
		if(LP==LP_DEV_CLOSE)
		{
			m_DllStatus.AddString(_T("**  PSL-DAQ is Disconnected"));
			((CButton *)GetDlgItem(IDC_CONNECTION_CHECK))->SetCheck(BST_UNCHECKED);

			return 0;
		}

		//
		// get the response to data acquisition stop command   
		//
		if (LP == LP_D2P_DATA_STOP)
		{
			m_DllStatus.AddString(_T("Receiving the response to stop the acquistion"));
			RcvDataProcEnable=0;
			return 0;
		}
		

		//
		// get the response to data acquisition start command   
		//
		if (LP == LP_D2P_DATA_START)
		{
			m_DllStatus.AddString(_T("Receiving the response to start the acquistion"));
			RcvDataProcEnable=1;
			return 0;
		}


		//
		// PSL-DAQ Device buffer Overflow 
		//
		if (LP == LP_DEV_BUF_OVERFLOW)
		{
			// "if(RcvDataProcEnable)" is only for running code on data receiving
			if(RcvDataProcEnable)
			{
				m_DllStatus.AddString(_T("[Error] PSL-DAQ Device internal buffer Overflow"));

				RcvDataProcEnable=0;
				OnBnClickedAcquireStopButtion();
			}
			return 0;
		}


		//
		// internal reception buffer in Dll is Overflow 
		//
		if (LP == LP_PC_QUEUE_OVERFLOW)
		{
			// "if(RcvDataProcEnable)" is only for running code on data receiving
			if(RcvDataProcEnable)
			{
				m_DllStatus.AddString(_T("[Error] Overflow of Receive buffer in Dll"));

				RcvDataProcEnable=0;
				OnBnClickedAcquireStopButtion();
			}
			return 0;
		}


		//
		// received data packet has wrong length  
		//
		if (LP == LP_RCV_PACKET_LENGTH_ERROR)
		{
			// "if(RcvDataProcEnable)" is only for running code on data receiving
			if(RcvDataProcEnable)
			{
				m_DllStatus.AddString(_T("[Error] Receiving Packet Length"));

				RcvDataProcEnable=0;
				OnBnClickedAcquireStopButtion();
			}
			return 0;
		}

		return 0; 
	}




	//-------------------------------------------------
	// *****
	// Processing Received Data
	// 
	//-------------------------------------------------
	 // after getting response of data acquisition start command, Receceived data will process.
	if(!RcvDataProcEnable)
		return 0;


	// buffer for receiving data
	static INT16 RxIData[NUM_OF_CHANNEL];
	static double RxdData[NUM_OF_CHANNEL];

	//-------------------------------------------------
	// received data processing
	//-------------------------------------------------
	if (LP == LP_D2P_DATA_IN)
	{
		CString st;

		while (1)
		{
			// read data, if no data to read, returns 0 and exit while()
			if(!PSLDAQ_Read(m_PSLDAQ_DeviceHandle, RxdData, RxIData))
				return 0; 


			// draw graph
			//m_Graph[0].GetData(RxdData[0]); 
			//m_Graph[1].GetData(RxdData[1]); 

			// show received data to edit. update every 1/20 second. 
			static int cnt=0;

			cnt++;
			if(cnt==50)
			{
				cnt=0;

				st.Format(_T("%.3f"),RxdData[0]);
				m_Ch1ValueEdit.SetWindowTextW(st);

				st.Format(_T("%.3f"),RxdData[1]);
				m_Ch2ValueEdit.SetWindowTextW(st);
			}

		}	// while ()
	}	// 	if (LP == LP_D2P_DATA_IN)


	return 1;
}


