// VC_ExampleDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "VC_Example.h"
#include "VC_ExampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//���� �߰��� �κ�
//��������� ����
//#include <windows.h>
#include "pch.h"
#include <stdio.h>
#include <WinSock2.h>

SOCKET sock, clientsock;
WSADATA wsa;
struct sockaddr_in sockinfo, clientinfo;
int clientsize;
char message[] ="Start";

void SocketCommunication()
{
	//��������� ���� �ڵ�__ ���� �߰��Ѱ�
   //SOCKET sock, clientsock;
   //WSADATA wsa;
   //struct sockaddr_in sockinfo, clientinfo;
   //int clientsize;
   //char message[] = "3.123\n";

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		//m_DllStatus.AddString(_T("�ʱ�ȭ ����"));
		printf("�ʱ�ȭ ����\n");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		//m_DllStatus.AddString(_T("���� ���� ����"));
		printf("���� ���� ����\n");
	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(1234); //��Ʈ��ȣ �Է�
	sockinfo.sin_addr.s_addr = htonl(INADDR_ANY); //IP�ּ� �����Է�

	if (bind(sock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
		//m_DllStatus.AddString(_T(" bind ���� "));
		printf(" bind ���� ");

	if (listen(sock, 5) == SOCKET_ERROR)
		//m_DllStatus.AddString(_T(" ��⿭ ����"));
		printf(" ��⿭ ���� ");

	//���� listen() �Լ��� Ŭ���̾�Ʈ ���� ��û�� Ȯ���Ѵ�.
	clientsize = sizeof(clientinfo);
	//m_DllStatus.AddString(_T("Ŭ����Ʈ�κ��� ������ ��ٸ��� �ֽ��ϴ�...\n"));
	printf("Ŭ����Ʈ�κ��� ������ ��ٸ��� �ֽ��ϴ�...\n");
	clientsock = accept(sock, (SOCKADDR*)&clientinfo, &clientsize);
	;

	//if (clientsock == INVALID_SOCKET) {
	//	//m_DllStatus.AddString(_T(" Ŭ���̾�Ʈ ���� ���� ���� "));
	//	printf(" Ŭ���̾�Ʈ ���� ���� ���� ");
	//	exit(0);
	//}

	//send(clientsock, message, sizeof(message), 0);

	/*closesocket(sock);
	closesocket(clientsock);

	WSACleanup();*/
}

// CVC_ExampleDlg ��ȭ ����

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
	// ����� ���� �޼��� ó�� 
	ON_MESSAGE(WM_RECEIVE_DATA,USB_ReceiveData)
	ON_BN_CLICKED(IDC_DLL_STATUS_LIST_CLEAR_BUTTON, &CVC_ExampleDlg::OnBnClickedDllStatusListClearButton)
END_MESSAGE_MAP()


// CVC_ExampleDlg �޽��� ó����



// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CVC_ExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CVC_ExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//****************************************************************************
//
//
// �ʱ�ȭ 
//
//
//****************************************************************************

BOOL CVC_ExampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.


	// *****
	//
	// PSLDAQ_Dll �ʱ�ȭ �� BME-DAQ ��ġ ���� �� ��ġ �ڵ� ��� 
	//  - ������ �߻��� ��� ����������  OnOK()�� ���� ���α׷� �����ϵ��� �ڵ��� �� 
	//  - ����� BMEDAQ ���� USB ����̹��� ��ġ�ؾ� ���������� Open��. 
	// 
	// �ι�° ������ DeviceSerialNum�� ����� ��� DeviceName�� ��ġ�ϴ� BME-DAQ ��ġ�� 
	// ������ �õ���. �߸� ��ϵ� ��� ��ġ�� �� �� �����Ƿ� ���ǹٶ�.
	// �������� ��ġ�� ���ÿ� ���� ������. 
	//
	// �������� ��ġ�� ������ ���� PSLDAQ_Open()�� ����ȸ ȣ���Ͽ� ������
	// ��ġ�ڵ��� ������ �޾ƾ� ��. 
	// 
	// PSLDAQ_Open �Լ����� ������ �߻��� ���(CD���� ������ ����̹� ��ġ�� ��ġ���� ���� ���
	// ��) ���α׷� ���� ���� ó���� ������  
	if(PSLDAQ_Open(m_hWnd,NULL,&m_PSLDAQ_DeviceHandle))
	{
		AfxMessageBox(_T("Error: PSLDAQ_Open"),MB_OK | MB_ICONERROR);
		OnOK();		
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}



//****************************************************************************
//
//
// ����� ó�� 
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// *****
	//
	// PSLDAQ_Dll ��� �Ϸ�, ��ġ���� ���� ����
	PSLDAQ_Close(m_PSLDAQ_DeviceHandle);
}


//****************************************************************************
//
//
// ��ư Click: ������ ȹ�� ����
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnBnClickedAcquireStartButtion()
{
	// *****
	//
	// ȹ�� ���� ��� ����
	PSLDAQ_AcquireStart(m_PSLDAQ_DeviceHandle);
}


//****************************************************************************
//
//
// ��ư Click: ������ ȹ�� ����
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnBnClickedAcquireStopButtion()
{
	// *****
	//
	// ȹ�� ������� ���� 
	PSLDAQ_AcquireStop(m_PSLDAQ_DeviceHandle);
	
	
	/////������� ����
	closesocket(sock);
	closesocket(clientsock);

	WSACleanup();
}


//****************************************************************************
//
//
// ��ư Click: Dll ���� ǥ�� List Clear
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
// ::USB_ReceiveData() 
//  - dll���� ������ ����� ���� �޼��� ó�� 
//  - �����͸� �����ϰų� dll���� ������ ��� Event ó�� 
//
//****************************************************************************
LRESULT CVC_ExampleDlg::USB_ReceiveData(WPARAM WP,LPARAM LP)
{
	
	//--------------------------------------------------------
	static bool  RcvDataProcEnable=0;

	//-------------------------------------------------
	//
	// ��� ���� Event�� ���� ó��
	//
	//-------------------------------------------------
	if(WP)
	{
		//
		// ��ġ ���� - �����ص� ��
		//
		if(LP==LP_DEV_OPEN)
		{
			m_DllStatus.AddString(_T("* ��ġ �����(USB ��ġ �νĵ�)"));
			SocketCommunication();
			return 0;
		}


		//
		// ��ġ �ʱ�ȭ ���� ���� ����  
		//
		if (LP == LP_COMM_INIT)
		{
			m_DllStatus.AddString(_T("*** ��ġ ��� �����(�ʱ�ȭ ���� ����)"));			
			((CButton *)GetDlgItem(IDC_CONNECTION_CHECK))->SetCheck(BST_CHECKED);

			// �ʿ�� ������ ��ġ ID ���� 
			 char DeviceID[256];
			 
			 PSLDAQ_GetDeviceID(m_PSLDAQ_DeviceHandle, DeviceID);

			 CString st=_T("     : DeviceID=");
			 st+=DeviceID;
			 m_DllStatus.AddString(st);

			return 0;
		}	

		//
		// ��ġ ���� 
		//
		if(LP==LP_DEV_CLOSE)
		{
			m_DllStatus.AddString(_T("** ��ġ ����"));
			((CButton *)GetDlgItem(IDC_CONNECTION_CHECK))->SetCheck(BST_UNCHECKED);

			return 0;
		}

		//
		// �ǽð� ������ ���� ���� ���� ����  
		//
		if (LP == LP_D2P_DATA_STOP)
		{
			m_DllStatus.AddString(_T("������ ���� ���� ���� ����"));
			RcvDataProcEnable=0;
			return 0;
		}
		

		//
		// �ǽð� ������ ���� ���� ���� ����  
		//
		if (LP == LP_D2P_DATA_START)
		{
			m_DllStatus.AddString(_T("������ ���� ���� ���� ����"));
			RcvDataProcEnable=1;
			return 0;
		}


		//
		// ��ġ ���� Overflow 
		//
		if (LP == LP_DEV_BUF_OVERFLOW)
		{
			// ������ ���� �߿��� ������ �ǹ̸� ���� 
			if(RcvDataProcEnable)
			{
				m_DllStatus.AddString(_T("[Error] ��ġ ���� ���� Overflow"));

				RcvDataProcEnable=0;
				OnBnClickedAcquireStopButtion();
			}
			return 0;
		}


		//
		// Dll ���� ���� ���� Overflow 
		//
		if (LP == LP_PC_QUEUE_OVERFLOW)
		{
			// ������ ���� �߿��� ������ �ǹ̸� ���� 
			if(RcvDataProcEnable)
			{
				m_DllStatus.AddString(_T("[Error] Dll ���� ���� ���� Overflow"));

				RcvDataProcEnable=0;
				OnBnClickedAcquireStopButtion();
			}
			return 0;
		}


		//
		// dll���� ������ ���Ž� ��Ŷ ���� ������ ���  
		//
		if (LP == LP_RCV_PACKET_LENGTH_ERROR)
		{
			// ������ ���� �߿��� ������ �ǹ̸� ���� 
			if(RcvDataProcEnable)
			{
				m_DllStatus.AddString(_T("[Error] ������ ��Ŷ ����"));

				RcvDataProcEnable=0;
				OnBnClickedAcquireStopButtion();
			}
			return 0;
		}

		return 0; 
	}




	//-------------------------------------------------
	//
	// ���� ������ ó�� 
	//
	//-------------------------------------------------
	// ù��° �����Ͱ� ������ ������ ó������ ���� 
	if(!RcvDataProcEnable)
		return 0;


	// Queue�� ���� �����͸� ����
	static INT16 RxIData[NUM_OF_CHANNEL];
	static double RxdData[NUM_OF_CHANNEL];
	//m_DllStatus.AddString(_T("��� ���۵ǳ�??"));
	//-------------------------------------------------
	// ���� �ǽð� ������ ó��
	//-------------------------------------------------
	if (LP == LP_D2P_DATA_IN)
	{
		// ������� �ڵ�

		CString st;

		while (1)
		{
			// �����͸� ����. ���� ������ ������ ��� 
			if(!PSLDAQ_Read(m_PSLDAQ_DeviceHandle, RxdData, RxIData))
				return 0; 


			// �׷����� �׸���. 
			//m_Graph[0].GetData(RxdData[0]); 
			//m_Graph[1].GetData(RxdData[1]); 

			// ����Ʈ �ڽ���  ��� ǥ�� 			
			// �ʹ� ���� ������ ���� ������ �����Ƿ� 1/20��(50 Sample)���� ���� 
			static int cnt=0;

			cnt++;
			if(cnt==50)
			{
				cnt=0;

				//RxdData[0] �̰� heart? ��
				st.Format(_T("%.3f"),RxdData[0]);
				m_Ch1ValueEdit.SetWindowTextW(st);
				//RxdData[1] �̰� beat��
				st.Format(_T("%.3f"),RxdData[1]);
				m_Ch2ValueEdit.SetWindowTextW(st);
				
				if (clientsock == INVALID_SOCKET) {
					m_DllStatus.AddString(_T(" Ŭ���̾�Ʈ ���� ���� ���� "));
					//printf(" Ŭ���̾�Ʈ ���� ���� ���� ");
					break;
				}
				//printf( "%.3f", &(RxdData[0]));
				//message = (RxdData[0]);
				sprintf(message, "%4.4f", RxdData[0]);
				send(clientsock, message, sizeof(message), 0);
				printf("\n");
			}
	
		}	// while ()

	}	// 	if (LP == LP_D2P_DATA_IN)

	
	return 1;
}
