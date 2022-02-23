#if !defined _PSLDAQ_EXPORT_HEADER_H_
#define _PSLDAQ_EXPORT_HEADER_H_



#ifdef __cplusplus
	extern "C" {
#endif


//********************************************
//
// Number of Data Channel
//
//********************************************
#define NUM_OF_CHANNEL	 2	


//********************************************
//
// This message is posted or sended  from PC DLL to DLL add-on programs 
// when it becomes necessary to inform about data reception or communication Event. 
//
//********************************************
#define WM_RECEIVE_DATA  (WM_USER+1)		// WPARAM: 0: existence of Received Data, 1: Event related with Communication  


//********************************************
//
// LPARAM of WM_RECEIVE_DATA  
//
//********************************************
enum WM_REVEIVE_DATA_LPARAM_ENUM
{
	LP_DEV_OPEN = 1,					// The USB device is recognized(Opened)
	LP_DEV_CLOSE,						// The USB device is disconnected(Closed)


	LP_DEV_BUF_OVERFLOW,				// Send Queue in PSL-DAQ is Overflow.
	LP_PC_QUEUE_OVERFLOW,				// Overflow of internal receive Queue in the DLL.  
	LP_RCV_PACKET_LENGTH_ERROR,			// Error of received packet from PSL-DAQ

	LP_COMM_INIT,						// After opening the USB device, the communication is established properly. 			

	LP_D2P_DATA_STOP,					// Reception of response to stop request(command) of data acquisition. 
	LP_D2P_DATA_START,					//Reception of response to start request(command) of data acquisition.   

	LP_D2P_DATA_IN,						// It notifies that data has been received. so, the PSLDAQ_Read() function is available to read the data.  
};



//*****************************************************************************
//
// Initializing Function(memory allocation, thread for DLL functions).
//  - Create device handle used in functions provided by DLL.
//  - Enables connection to PSL-DAQ(Communication connection activation).
//
// * Parameters
//  - MsgWnd(In): A window handle that processes WM_RECEIVE_DATA message transmitted from DLL
//  - DeviceName(In): It is the unique device name(ID) of the PSL-DAQ
//         If several devices are connected and DeviceName is empty string, the Device Selection Dialog will appear. 
//         If deviceName is recorded, connection will be tried and established only to the PSL-DAQ module with ID same as DeviceName.  
//  - DeviceHandle(out): device handle, used in all functions of DLL except for the PSLDAQ_Open() function.
//         In order to handle several devices, the number of PSLDAQ_Open() functions according to the devices must be called.
//		   each DeviceHandle has unique value.
//
//  
// * Return Value
//  - 0: correct
//  - 1: Error  
//
//*****************************************************************************
__declspec(dllimport) int PSLDAQ_Open(HWND MsgWnd, char *DeviceName,unsigned int *DeviceHandle);


//*****************************************************************************
//
// Finishing Function  
//  - Removal of memories, threads allocated by PSLDAQ_Open() 
//  - finishing DLL function(disconnecting communications and so on) 
//
// * Parameters
//   - DeviceHandle(In): Device Handle 
//
//*****************************************************************************
__declspec(dllimport) void PSLDAQ_Close(unsigned int DeviceHandle);



//*****************************************************************************
//
// Starting data acquisition from PSL-DAQ 
//
// * Parameters
//   - DeviceHandle(In): Device Handle 
//
//*****************************************************************************
__declspec(dllimport) void PSLDAQ_AcquireStart(unsigned int DeviceHandle);

//*****************************************************************************
//
// Stoping data acquisition from PSL-DAQ
//
// * Parameters
//   - DeviceHandle(In): Device Handle 
//
//*****************************************************************************
__declspec(dllimport) void PSLDAQ_AcquireStop(unsigned int DeviceHandle);



//*****************************************************************************
//
// Reading acquired data from PSL-DAQ.
//
// * Parameters
//   - DeviceHandle(In): Device Handle  
//   - pdData(Out): pdData points  double [2] array.  after function call,
//          double [2] array includes 1 sample of 2 channel double data obtained from PSL-DAQ. 
//   - piData(Out): pIData points  INT16 [2] array.  after function call, 
//         INT [2] array includes 1 sample of 2 channel INT16 data obtained from PSL-DAQ. this is not necessay to be used.
//  
// * Return Value
//  - 0: No data is readable. The value of pdData is invalid.(prohibitive to use.) 
//  - 1: Data is readable. The value of pdData is valid.(possible to use.) 
// 
//*****************************************************************************
__declspec(dllimport) unsigned char PSLDAQ_Read(unsigned int DeviceHandle, double *pdData, INT16 *pIData);



//*****************************************************************************
//
// Obtains a unique name(ID) from the linked PSL-DAQ devce.   
//
// * Parameters
//   - DeviceHandle(In): Device Handle  
//   - DeviceID: DeviceID points char [256] array.  after function call, 
//          char [256] array includes  unique device name(ID) of the Opened PSL-DAQ. 
// 
//*****************************************************************************
__declspec(dllimport) void PSLDAQ_GetDeviceID(unsigned int DeviceHandle, char *DeviceID);




#ifdef __cplusplus
	}
#endif

#endif