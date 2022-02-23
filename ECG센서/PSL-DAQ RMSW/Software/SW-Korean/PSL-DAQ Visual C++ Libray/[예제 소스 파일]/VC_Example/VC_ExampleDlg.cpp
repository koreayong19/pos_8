// VC_ExampleDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "VC_Example.h"
#include "VC_ExampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//내가 추가한 부분
//소켓통신을 위한
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
	//소켓통신을 위한 코드__ 내가 추가한것
   //SOCKET sock, clientsock;
   //WSADATA wsa;
   //struct sockaddr_in sockinfo, clientinfo;
   //int clientsize;
   //char message[] = "3.123\n";

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		//m_DllStatus.AddString(_T("초기화 실패"));
		printf("초기화 실패\n");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		//m_DllStatus.AddString(_T("소켓 생성 실패"));
		printf("소켓 생성 실패\n");
	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(1234); //포트번호 입력
	sockinfo.sin_addr.s_addr = htonl(INADDR_ANY); //IP주소 정보입력

	if (bind(sock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
		//m_DllStatus.AddString(_T(" bind 실패 "));
		printf(" bind 실패 ");

	if (listen(sock, 5) == SOCKET_ERROR)
		//m_DllStatus.AddString(_T(" 대기열 실패"));
		printf(" 대기열 실패 ");

	//이후 listen() 함수로 클라이언트 접속 요청을 확인한다.
	clientsize = sizeof(clientinfo);
	//m_DllStatus.AddString(_T("클라디언트로부터 접속을 기다리고 있습니다...\n"));
	printf("클라디언트로부터 접속을 기다리고 있습니다...\n");
	clientsock = accept(sock, (SOCKADDR*)&clientinfo, &clientsize);
	;

	//if (clientsock == INVALID_SOCKET) {
	//	//m_DllStatus.AddString(_T(" 클라이언트 소켓 연결 실패 "));
	//	printf(" 클라이언트 소켓 연결 실패 ");
	//	exit(0);
	//}

	//send(clientsock, message, sizeof(message), 0);

	/*closesocket(sock);
	closesocket(clientsock);

	WSACleanup();*/
}

// CVC_ExampleDlg 대화 상자

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
	// 사용자 정의 메세지 처리 
	ON_MESSAGE(WM_RECEIVE_DATA,USB_ReceiveData)
	ON_BN_CLICKED(IDC_DLL_STATUS_LIST_CLEAR_BUTTON, &CVC_ExampleDlg::OnBnClickedDllStatusListClearButton)
END_MESSAGE_MAP()


// CVC_ExampleDlg 메시지 처리기



// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CVC_ExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CVC_ExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//****************************************************************************
//
//
// 초기화 
//
//
//****************************************************************************

BOOL CVC_ExampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	// *****
	//
	// PSLDAQ_Dll 초기화 및 BME-DAQ 장치 열기 및 장치 핸들 얻기 
	//  - 에러가 발생한 경우 예제에서는  OnOK()를 통해 프로그램 종료하도록 코딩해 둠 
	//  - 참고로 BMEDAQ 관련 USB 드라이버를 설치해야 정상적으로 Open됨. 
	// 
	// 두번째 인자인 DeviceSerialNum을 기록할 경우 DeviceName과 일치하는 BME-DAQ 장치만 
	// 열려고 시도함. 잘못 기록된 경우 장치를 열 수 없으므로 주의바람.
	// 여러개의 장치를 동시에 열때 유용함. 
	//
	// 여러개의 장치를 연결할 경우는 PSLDAQ_Open()을 여러회 호출하여 고유의
	// 장치핸들을 여러개 받아야 함. 
	// 
	// PSLDAQ_Open 함수에서 오류가 발생한 경우(CD에서 제공한 드라이버 장치를 설치하지 않은 경우
	// 등) 프로그램 종료 등의 처리를 권장함  
	if(PSLDAQ_Open(m_hWnd,NULL,&m_PSLDAQ_DeviceHandle))
	{
		AfxMessageBox(_T("Error: PSLDAQ_Open"),MB_OK | MB_ICONERROR);
		OnOK();		
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}



//****************************************************************************
//
//
// 종료시 처리 
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// *****
	//
	// PSLDAQ_Dll 사용 완료, 장치와의 연결 해지
	PSLDAQ_Close(m_PSLDAQ_DeviceHandle);
}


//****************************************************************************
//
//
// 버튼 Click: 데이터 획득 시작
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnBnClickedAcquireStartButtion()
{
	// *****
	//
	// 획득 시작 명령 전송
	PSLDAQ_AcquireStart(m_PSLDAQ_DeviceHandle);
}


//****************************************************************************
//
//
// 버튼 Click: 데이터 획득 중지
// 
//
//****************************************************************************
void CVC_ExampleDlg::OnBnClickedAcquireStopButtion()
{
	// *****
	//
	// 획득 중지명령 전송 
	PSLDAQ_AcquireStop(m_PSLDAQ_DeviceHandle);
	
	
	/////소켓통신 중지
	closesocket(sock);
	closesocket(clientsock);

	WSACleanup();
}


//****************************************************************************
//
//
// 버튼 Click: Dll 상태 표시 List Clear
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
//  - dll에서 보내는 사용자 정의 메세지 처리 
//  - 데이터를 수신하거나 dll에서 보내는 통신 Event 처리 
//
//****************************************************************************
LRESULT CVC_ExampleDlg::USB_ReceiveData(WPARAM WP,LPARAM LP)
{
	
	//--------------------------------------------------------
	static bool  RcvDataProcEnable=0;

	//-------------------------------------------------
	//
	// 통신 관련 Event에 대한 처리
	//
	//-------------------------------------------------
	if(WP)
	{
		//
		// 장치 열림 - 무시해도 됨
		//
		if(LP==LP_DEV_OPEN)
		{
			m_DllStatus.AddString(_T("* 장치 연결됨(USB 장치 인식됨)"));
			SocketCommunication();
			return 0;
		}


		//
		// 장치 초기화 응답 응답 받음  
		//
		if (LP == LP_COMM_INIT)
		{
			m_DllStatus.AddString(_T("*** 장치 통신 연결됨(초기화 응답 받음)"));			
			((CButton *)GetDlgItem(IDC_CONNECTION_CHECK))->SetCheck(BST_CHECKED);

			// 필요시 접속한 장치 ID 얻음 
			 char DeviceID[256];
			 
			 PSLDAQ_GetDeviceID(m_PSLDAQ_DeviceHandle, DeviceID);

			 CString st=_T("     : DeviceID=");
			 st+=DeviceID;
			 m_DllStatus.AddString(st);

			return 0;
		}	

		//
		// 장치 닫힘 
		//
		if(LP==LP_DEV_CLOSE)
		{
			m_DllStatus.AddString(_T("** 장치 닫힘"));
			((CButton *)GetDlgItem(IDC_CONNECTION_CHECK))->SetCheck(BST_UNCHECKED);

			return 0;
		}

		//
		// 실시간 데이터 수신 중지 응답 받음  
		//
		if (LP == LP_D2P_DATA_STOP)
		{
			m_DllStatus.AddString(_T("데이터 수신 중지 응답 받음"));
			RcvDataProcEnable=0;
			return 0;
		}
		

		//
		// 실시간 데이터 수신 시작 응답 받음  
		//
		if (LP == LP_D2P_DATA_START)
		{
			m_DllStatus.AddString(_T("데이터 수신 시작 응답 받음"));
			RcvDataProcEnable=1;
			return 0;
		}


		//
		// 장치 버퍼 Overflow 
		//
		if (LP == LP_DEV_BUF_OVERFLOW)
		{
			// 데이터 수신 중에만 오류가 의미를 가짐 
			if(RcvDataProcEnable)
			{
				m_DllStatus.AddString(_T("[Error] 장치 내부 버퍼 Overflow"));

				RcvDataProcEnable=0;
				OnBnClickedAcquireStopButtion();
			}
			return 0;
		}


		//
		// Dll 내부 수신 버퍼 Overflow 
		//
		if (LP == LP_PC_QUEUE_OVERFLOW)
		{
			// 데이터 수신 중에만 오류가 의미를 가짐 
			if(RcvDataProcEnable)
			{
				m_DllStatus.AddString(_T("[Error] Dll 내부 수신 버퍼 Overflow"));

				RcvDataProcEnable=0;
				OnBnClickedAcquireStopButtion();
			}
			return 0;
		}


		//
		// dll에서 데이터 수신시 패킷 길이 오류인 경우  
		//
		if (LP == LP_RCV_PACKET_LENGTH_ERROR)
		{
			// 데이터 수신 중에만 오류가 의미를 가짐 
			if(RcvDataProcEnable)
			{
				m_DllStatus.AddString(_T("[Error] 수신한 패킷 길이"));

				RcvDataProcEnable=0;
				OnBnClickedAcquireStopButtion();
			}
			return 0;
		}

		return 0; 
	}




	//-------------------------------------------------
	//
	// 수신 데이터 처리 
	//
	//-------------------------------------------------
	// 첫번째 데이터가 들어오지 않으면 처리하지 않음 
	if(!RcvDataProcEnable)
		return 0;


	// Queue로 부터 데이터를 읽음
	static INT16 RxIData[NUM_OF_CHANNEL];
	static double RxdData[NUM_OF_CHANNEL];
	//m_DllStatus.AddString(_T("계속 시작되나??"));
	//-------------------------------------------------
	// 수신 실시간 데이터 처리
	//-------------------------------------------------
	if (LP == LP_D2P_DATA_IN)
	{
		// 소켓통신 코드

		CString st;

		while (1)
		{
			// 데이터를 읽음. 읽을 데이터 없으면 벗어남 
			if(!PSLDAQ_Read(m_PSLDAQ_DeviceHandle, RxdData, RxIData))
				return 0; 


			// 그래프를 그린다. 
			//m_Graph[0].GetData(RxdData[0]); 
			//m_Graph[1].GetData(RxdData[1]); 

			// 에디트 박스에  결과 표시 			
			// 너무 빠른 변경은 눈에 보이지 않으므로 1/20초(50 Sample)마다 갱신 
			static int cnt=0;

			cnt++;
			if(cnt==50)
			{
				cnt=0;

				//RxdData[0] 이게 heart? 값
				st.Format(_T("%.3f"),RxdData[0]);
				m_Ch1ValueEdit.SetWindowTextW(st);
				//RxdData[1] 이게 beat값
				st.Format(_T("%.3f"),RxdData[1]);
				m_Ch2ValueEdit.SetWindowTextW(st);
				
				if (clientsock == INVALID_SOCKET) {
					m_DllStatus.AddString(_T(" 클라이언트 소켓 연결 실패 "));
					//printf(" 클라이언트 소켓 연결 실패 ");
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
