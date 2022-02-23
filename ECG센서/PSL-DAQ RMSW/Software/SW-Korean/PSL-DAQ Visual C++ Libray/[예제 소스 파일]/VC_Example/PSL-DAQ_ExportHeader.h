#if !defined _PSLDAQ_EXPORT_HEADER_H_
#define _PSLDAQ_EXPORT_HEADER_H_



#ifdef __cplusplus
	extern "C" {
#endif


//********************************************
//
// 데이터 수신시 체널 수 
//
//********************************************
#define NUM_OF_CHANNEL	 2	


//********************************************
//
// 데이터 수신 및 Event 발생시 dll에서 부모 윈도우로 전달하는 메세지
//
//********************************************
#define WM_RECEIVE_DATA  (WM_USER+1)		// WPARAM: 0: 데이터 패킷, 1: LPARAM에 통신 관련 정보 보냄  


//********************************************
//
// WM_RECEIVE_DATA의 LPARAM의 의미  
//
//********************************************
enum WM_REVEIVE_DATA_LPARAM_ENUM
{
	LP_DEV_OPEN = 1,					// 장치 열림
	LP_DEV_CLOSE,						// 장치 닫힘 


	LP_DEV_BUF_OVERFLOW,				// 장치 데이터 Queue Overflow
	LP_PC_QUEUE_OVERFLOW,				// PC의 Queue Overflow 
	LP_RCV_PACKET_LENGTH_ERROR,			// 수신 패킷 길이 오류 

	LP_COMM_INIT,						// 통신 초기화 명령 응답 받음 			

	LP_D2P_DATA_STOP,					// 실시간 수신 데이터 중지
	LP_D2P_DATA_START,					// 실시간 수신 데이터, 시작  

	LP_D2P_DATA_IN,						// 수신 데이터 들어옴 
};




//*****************************************************************************
//
// 장치 열기 및 장치 핸들 생성  
// 인자: MsgWnd: 데이터 수신 등의 메세지를 받기 위한 윈도우 핸들 
//						SDI, MDI의 경우 일반적으로 View, 다이얼로그의 경우 다이얼로그 윈도우 핸들 등
//						수신 데이터 처리 및 여러가지 메세지를 받아 처리할 윈도우의 핸들 
//			DeviceSerialNum: 여러개의 장치를 열 때 장치 선택 대화상자 없이 장치를 열고자 할때 장치명 기록, 
// 							여러개의 장치가 있을 때 장치 선택 대화 상자를 나타낼 경우 NULL(일반적인 경우임)                            
//			DeviceHandle: BMEDAQ 제어용 장치 핸들 
//
//  리턴값: 오류시 1, 정상시 0
//      
//*****************************************************************************
__declspec(dllimport) int PSLDAQ_Open(HWND MsgWnd, char *DeviceName,unsigned int *DeviceHandle);


//*****************************************************************************
//
// 종료 함수  
//  - 프로그램 종료 전 (stop, close 등) 반드시 호출 필요 
//  
//  인자: DeviceHandle: 장치 핸들 
//
//*****************************************************************************
__declspec(dllimport) void PSLDAQ_Close(unsigned int DeviceHandle);



//*****************************************************************************
//
// PSL-DAQ 장치로부터 데이터 획득 시작  
//
//  인자: DeviceHandle: 장치 핸들 
//
//*****************************************************************************
__declspec(dllimport) void PSLDAQ_AcquireStart(unsigned int DeviceHandle);

//*****************************************************************************
//
// PSL-DAQ 장치로부터 데이터 획득 중지  
//
//  인자: DeviceHandle: 장치 핸들 
//
//*****************************************************************************
__declspec(dllimport) void PSLDAQ_AcquireStop(unsigned int DeviceHandle);



//*****************************************************************************
//
// 데이터 Read 함수
//
// 인자: DeviceHandle: 장치 핸들 
//       pdData: double[채널수]형 배열 pointer , PSL-DAQ의 경우 채널수는 2
//       piData: INT16 [채널수]형 배열 pointer , 송수신 데이터간 오류 체크에만 사용됨 
//       
//  리턴값: 0: 읽을 데이터 없음
//          1: 읽을 데이터 있음 
// 
//*****************************************************************************
__declspec(dllimport) unsigned char PSLDAQ_Read(unsigned int DeviceHandle, double *pdData, INT16 *pIData);



//*****************************************************************************
//
// 연결된 장치의 Device ID를 얻음 
//
// 인자: DeviceHandle: 장치 핸들 
//       DeviceID: 장치의 Device ID, 외부에서 char [256]의 배열을 생성 후 입력 
// 
//*****************************************************************************
__declspec(dllimport) void PSLDAQ_GetDeviceID(unsigned int DeviceHandle, char *DeviceID);




#ifdef __cplusplus
	}
#endif

#endif