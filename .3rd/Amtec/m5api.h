// ========================================================================= //
//                                                                           //
//     Copyright 1999,2000,2001												 //
//                                                                           //
//     AMTEC GmbH                                                            //
//     Automatisierung, Mess- und Testtechnologien                           //
//     13127 Berlin                                                          //
//     Germany                                                               //
//                                                                           //
//     Tel   : +49 30 474990 0                                               //
//     Fax   : +49 30 474990 99                                              //
//     Email : support@amtec-robotics.com                                    //
//                                                                           //
// ========================================================================= //

#ifndef M5API_H
#define M5API_H


#define M5DLLVERSION 0x7000
#define MAX_MODULES 31

#if defined (_WIN32)
#ifndef WINAPI
	#define WINAPI __stdcall
#endif

#ifdef M5APIW32_EXPORTS
	#define M5DLL_API __declspec(dllexport)
#else
	#define M5DLL_API __declspec(dllimport)
#endif
#endif


extern M5DLL_API int g_iM5DebugLevel;
extern M5DLL_API bool g_bM5DebugFile;
extern M5DLL_API bool g_bM5Debug;

//-----------------------------------------------------------------------
// new PCube Module States
//-----------------------------------------------------------------------
#define STATEID_MOD_ERROR						0x00000001L
#define STATEID_MOD_HOME						0x00000002L
#define STATEID_MOD_HALT						0x00000004L
#define STATEID_MOD_POWERFAULT				0x00000008L

#define STATEID_MOD_TOW_ERROR					0x00000010L
#define STATEID_MOD_COMM_ERROR				0x00000020L
#define STATEID_MOD_SWR							0x00000040L
#define STATEID_MOD_SW1							0x00000080L

#define STATEID_MOD_SW2							0x00000100L
#define STATEID_MOD_BRAKEACTIVE 				0x00000200L
#define STATEID_MOD_CURLIMIT					0x00000400L
#define STATEID_MOD_MOTION						0x00000800L

#define STATEID_MOD_RAMP_ACC					0x00001000L
#define STATEID_MOD_RAMP_STEADY				0x00002000L
#define STATEID_MOD_RAMP_DEC					0x00004000L
#define STATEID_MOD_RAMP_END					0x00008000L

#define STATEID_MOD_INPROGRESS				0x00010000L
#define STATEID_MOD_FULLBUFFER				0x00020000L
#define STATEID_MOD_POW_VOLT_ERR				0x00040000L
#define STATEID_MOD_POW_FET_TEMP				0x00080000L

#define STATEID_MOD_POW_WDG_TEMP				0x00100000L
#define STATEID_MOD_POW_SHORTCUR				0x00200000L
#define STATEID_MOD_POW_HALLERR				0x00400000L
#define STATEID_MOD_POW_INTEGRALERR			0x00800000L
#define STATEID_MOD_CPU_OVERLOAD				0x01000000L
#define STATEID_MOD_BEYOND_HARD				0x02000000L
#define STATEID_MOD_BEYOND_SOFT				0x04000000L

//-----------------------------------------------------------------------
// new PCube Module Config
//-----------------------------------------------------------------------
#define CONFIGID_MOD_ENCODER_FEEDBACK			0x00000001L //OS: 25xx from 35xx in SETUP
#define CONFIGID_MOD_RESOLVER_FEEDBACK			0x00000002L //OS: 25xx from 35xx in SETUP
#define CONFIGID_MOD_ABSOLUTE_FEEDBACK			0x00000004L //OS: 25xx from 35xx in SETUP
#define CONFIGID_MOD_BRAKE_PRESENT				0x00000008L
#define CONFIGID_MOD_BRAKE_AT_POWERON			0x00000010L
#define CONFIGID_MOD_SWR_WITH_ENCODERZERO		0x00000020L
#define CONFIGID_MOD_SWR_AT_FALLING_EDGE		0x00000040L
#define CONFIGID_MOD_CHANGE_SWR_TO_LIMIT		0x00000080L
#define CONFIGID_MOD_SWR_ENABLED        		0x00000100L
#define CONFIGID_MOD_SWR_LOW_ACTIVE				0x00000200L
#define CONFIGID_MOD_SWR_EQ_POSLIMIT			0x00000400L //OS: 25xx
#define CONFIGID_MOD_SWR_USE_EXTERNAL			0x00000400L //OS: 35xx
#define CONFIGID_MOD_SW1_ENABLED				0x00000800L
#define CONFIGID_MOD_SW1_LOW_ACTIVE				0x00001000L
#define CONFIGID_MOD_SW1_EQ_POSLIMIT			0x00002000L //OS: 25xx
#define CONFIGID_MOD_SW1_USE_EXTERNAL			0x00002000L //OS: 35xx
#define CONFIGID_MOD_SW2_ENABLED				0x00004000L
#define CONFIGID_MOD_SW2_LOW_ACTIVE				0x00008000L
#define CONFIGID_MOD_SW2_EQ_POSLIMIT			0x00010000L //OS: 25xx
#define CONFIGID_MOD_SW2_USE_EXTERNAL			0x00010000L //OS: 35xx
#define CONFIGID_MOD_LINEAR						0x00020000L
#define CONFIGID_MOD_OPENBRAKE_ON_SW2			0x00040000L	//OS: 25xx but not in 2512 from 35xx in SETUP
#define CONFIGID_MOD_ALLOW_FULL_CUR				0x00080000L
#define CONFIGID_MOD_M3_COMPATIBLE				0x00100000L
#define CONFIGID_MOD_LINEAR_SCREW				0x00200000L
#define CONFIGID_MOD_DISABLE_ON_HALT			0x00800000L
#define CONFIGID_MOD_WATCHDOG_ENABLE			0x01000000L
#define CONFIGID_MOD_ZERO_MOVE_AFTER_HOK		0x02000000L //OS: 35xx
#define CONFIGID_MOD_DISABLE_ACK				0x04000000L
#define CONFIGID_MOD_SYNC_MOTION				0x08000000L
#define CONFIGID_MOD_SYNC_TRANSMIT				0x10000000L //OS: 36xx
#define CONFIGID_MOD_CAN_RS232_COMM				0x40000000L //OS: 25xx
#define CONFIGID_MOD_PROFI_RS485_COMM			0x80000000L //OS: 25xx

//-----------------------------------------------------------------------
// new PCube Module Setup
//-----------------------------------------------------------------------
// ---- FEEDBACK SETUP -----------
#define SETUPID_MOD_ENCODER_FEEDBACK			0x00000001L
#define SETUPID_MOD_RESOLVER_FEEDBACK			0x00000002L
#define SETUPID_MOD_ABSOLUTE_FEEDBACK			0x00000004L
// ---- DIGITAL IO SETUP ---------
#define SETUPID_MOD_4IN_4OUT					0x00000008L
#define SETUPID_MOD_3IN_ENCODER_IN				0x00000010L
#define SETUPID_MOD_3IN_ENCODER_OUT				0x00000020L
// ---- COMM INTERFACE -----------
#define SETUPID_MOD_RS232						0x00000040L
#define SETUPID_MOD_CAN							0x00000200L
#define SETUPID_MOD_PROFIBUS					0x00000400L
// ---- CAN ID SETUP -------------
#define SETUPID_MOD_USE_M3ID					0x00000800L
#define SETUPID_MOD_USE_M4ID					0x00001000L
#define SETUPID_MOD_USE_CANOPEN					0x00002000L
// ---- MOTOR SETUP --------------
#define SETUPID_MOD_INVERT_MOTORDIR				0x00004000L
// ---- INPUT USAGE --------------
#define SETUPID_MOD_USE_SW2_AS_ENABLE			0x00008000L
#define SETUPID_MOD_USE_SW2_AS_BRAKE			0x00010000L
// ---- OUTPUT USAGE -------------
#define SETUPID_MOD_ERROR_TO_OUT0				0x00020000L
// ---- IN/OUTPUT USAGE -------------
#define SETUPID_MOD_IO_GRIPPER_CONTROL			0x00080000L

//-----------------------------------------------------------------------
// new PCube Module Types
//-----------------------------------------------------------------------
#define TYPEID_MOD_ROTARY						0x0f
#define TYPEID_MOD_LINEAR						0xf0

//-----------------------------------------------------------------------
// new PCube Module DIO's
//-----------------------------------------------------------------------
#define DIOID_MOD_INBIT0						0x00000001L
#define DIOID_MOD_INBIT1						0x00000002L
#define DIOID_MOD_INBIT2						0x00000004L
#define DIOID_MOD_INBIT3						0x00000008L
 
#define DIOID_MOD_OUTBIT0						0x00000010L
#define DIOID_MOD_OUTBIT1						0x00000020L
#define DIOID_MOD_OUTBIT2						0x00000040L
#define DIOID_MOD_OUTBIT3						0x00000080L
 
#define DIOID_MOD_INSWR							0x00000100L
#define DIOID_MOD_INSW1							0x00000200L
#define DIOID_MOD_INSW2							0x00000400L

//-----------------------------------------------------------------------
// new EMS Module Types
//-----------------------------------------------------------------------
#define TYPEID_EMS_DIG24V_OUT					10
#define TYPEID_EMS_DIG24V_IN					11
#define TYPEID_EMS_ANA10V_OUT					12
#define TYPEID_EMS_ANA10V_IN					13
#define TYPEID_EMS_ANA25MA_OUT				14
#define TYPEID_EMS_ANA25MA_IN					15

//-----------------------------------------------------------------------
// new PCube Module Baudrates
//-----------------------------------------------------------------------
#define BAUDRATEID_MOD_CAN_75K					0
#define BAUDRATEID_MOD_CAN_250K					1
#define BAUDRATEID_MOD_CAN_500K					2
#define BAUDRATEID_MOD_CAN_1000K				3

#define BAUDRATEID_MOD_RS232_1200				0
#define BAUDRATEID_MOD_RS232_2400				1
#define BAUDRATEID_MOD_RS232_4800				2
#define BAUDRATEID_MOD_RS232_9600				3
#define BAUDRATEID_MOD_RS232_19200				4
#define BAUDRATEID_MOD_RS232_38400				5

//-----------------------------------------------------------------------
// new PCube Device Error Codes
//-----------------------------------------------------------------------
#define ERRID_DEV							      (-200L)		// device error
#define ERRID_DEV_FUNCTIONNOTAVAILABLE  	(ERRID_DEV - 1)	
#define ERRID_DEV_NOINITSTRING				(ERRID_DEV - 2)
#define ERRID_DEV_NODEVICENAME				(ERRID_DEV - 3)
#define ERRID_DEV_BADINITSTRING				(ERRID_DEV - 4)
#define ERRID_DEV_INITERROR					(ERRID_DEV - 5)
#define ERRID_DEV_NOTINITIALIZED			   (ERRID_DEV - 6)
#define ERRID_DEV_WRITEERROR				   (ERRID_DEV - 7)
#define ERRID_DEV_READERROR					(ERRID_DEV - 8)
#define ERRID_DEV_WRITETIMEOUT				(ERRID_DEV - 9)
#define ERRID_DEV_READTIMEOUT				   (ERRID_DEV - 10)
#define ERRID_DEV_WRONGMESSAGEID		   	(ERRID_DEV - 11)
#define ERRID_DEV_WRONGCOMMANDID			   (ERRID_DEV - 12)
#define ERRID_DEV_WRONGPARAMETERID			(ERRID_DEV - 13)
#define ERRID_DEV_EXITERROR					(ERRID_DEV - 14)
#define ERRID_DEV_NOMODULES					(ERRID_DEV - 15)
#define ERRID_DEV_WRONGDEVICEID				(ERRID_DEV - 16)
#define ERRID_DEV_NOLIBRARY					(ERRID_DEV - 17)
#define ERRID_DEV_ISINITIALIZED				(ERRID_DEV - 18)
#define ERRID_DEV_WRONGEMSMODULEID			(ERRID_DEV - 19)
#define ERRID_DEV_EMSNOTINITIALIZED			(ERRID_DEV - 20)
#define ERRID_DEV_EMSMAXNUMBER				(ERRID_DEV - 21)
#define ERRID_DEV_EMSINITERROR				(ERRID_DEV - 22)
#define ERRID_DEV_WRONGEMSTYPE				(ERRID_DEV - 23)
#define ERRID_DEV_WRONGEMSCHANNELID			(ERRID_DEV - 24)
#define ERRID_DEV_WRONGMP55MODULEID			(ERRID_DEV - 25)
#define ERRID_DEV_WRONGSCHUNKMODULEID		(ERRID_DEV - 26)
#define ERRID_DEV_WRONGMODULEID				(ERRID_DEV - 27)
#define ERRID_DEV_MODULEERROR			   	(ERRID_DEV - 28)
#define ERRID_DEV_WAITTIMEOUT			   	(ERRID_DEV - 29)
#define ERRID_DEV_OPENINIFILE			   	(ERRID_DEV - 30)

//-----------------------------------------------------------------------
// old PCube Device Error Codes
//-----------------------------------------------------------------------
#define CLD_OK							0
#define CLD_ERR							-1
#define CLDERR_ERROR					(-200)
#define CLERR_FUNCTIONNOTAVAILABLE  	(CLDERR_ERROR - 1)	
#define CLERR_NOINITSTRING				(CLDERR_ERROR - 2)
#define CLERR_NODEVICENAME				(CLDERR_ERROR - 3)
#define CLERR_BADDEVICEINITSTRING		(CLDERR_ERROR - 4)
#define CLDERR_INITIALIZATIONERROR		(CLDERR_ERROR - 5)	
#define CLERR_DEVICENOTOPEN				(CLDERR_ERROR - 6)
#define CLDERR_TRANSMISSIONERROR		(CLDERR_ERROR - 7)
#define CLDERR_RECEIVEERROR				(CLDERR_ERROR - 8)
#define CLERR_COMM_SENDTIMEOUT			(CLDERR_ERROR - 9)
#define CLERR_COMM_RECVTIMEOUT			(CLDERR_ERROR - 10)
#define CLDERR_DRIVE_NOMODULEFOUND		(CLDERR_ERROR - 15)
#define CLERR_WRONGHANDLE				(CLDERR_ERROR - 16)
#define CLDERR_LIBRARYNOTFOUND			(CLDERR_ERROR - 17)	

//-----------------------------------------------------------------------
// old PCube Module States
//-----------------------------------------------------------------------
#define STATE_ERROR							0x00000001L
#define STATE_HOME_OK						0x00000002L
#define STATE_HALTED						0x00000004L
#define STATE_POWERFAULT					0x00000008L
#define STATE_TOW_ERROR						0x00000010L
#define STATE_COMM_ERROR					0x00000020L
#define STATE_SWR							0x00000040L
#define STATE_SW1							0x00000080L
#define STATE_SW2							0x00000100L
#define STATE_BRAKEACTIVE 					0x00000200L
#define STATE_CURLIMIT						0x00000400L
#define STATE_MOTION						0x00000800L
#define STATE_RAMP_ACC						0x00001000L
#define STATE_RAMP_STEADY					0x00002000L
#define STATE_RAMP_DEC						0x00004000L
#define STATE_RAMP_END						0x00008000L
#define STATE_INPROGRESS					0x00010000L
#define STATE_FULLBUFFER					0x00020000L
#define STATE_POW_VOLT_ERR					0x00040000L
#define STATE_POW_FET_TEMP					0x00080000L
#define STATE_POW_WDG_TEMP					0x00100000L
#define STATE_POW_SHORTCUR					0x00200000L
#define STATE_POW_HALLERR					0x00400000L
#define STATE_POW_INTEGRALERR				0x00800000L
#define STATE_CPU_OVERLOAD					0x01000000L
#define STATE_BEYOND_HARD				   	0x02000000L
#define STATE_BEYOND_SOFT					0x04000000L

//-----------------------------------------------------------------------
// old PCube Module Types
//-----------------------------------------------------------------------
#define	ROTARY_DRIVE						TYPEID_MOD_ROTARY
#define LINEAR_DRIVE						TYPEID_MOD_LINEAR

//-----------------------------------------------------------------------
// old EMS Module Types
//-----------------------------------------------------------------------
#define TW_DIG24V_OUT						TYPEID_EMS_DIG24V_OUT
#define TW_DIG24V_IN						TYPEID_EMS_DIG24V_IN
#define TW_ANA10V_OUT						TYPEID_EMS_ANA10V_OUT
#define TW_ANA10V_IN						TYPEID_EMS_ANA10V_IN
#define TW_ANA25MA_OUT						TYPEID_EMS_ANA25MA_OUT
#define TW_ANA25MA_IN						TYPEID_EMS_ANA25MA_IN

//-----------------------------------------------------------------------
// old PCube Module Config
//-----------------------------------------------------------------------

#define CONFIG_ENCODER_FEEDBACK				0x00000001L
#define CONFIG_RESOLVER_FEEDBACK			0x00000002L
#define CONFIG_ABSOLUTE_FEEDBACK			0x00000004L
#define CONFIG_BRAKE_PRESENT				0x00000008L
#define CONFIG_BRAKE_AT_POWERON				0x00000010L
#define CONFIG_SWR_WITH_ENCODERZERO			0x00000020L
#define CONFIG_SWR_AT_FALLING_EDGE			0x00000040L
#define CONFIG_CHANGE_SWR_TO_LIMIT			0x00000080L
#define CONFIG_SWR_ENABLED        			0x00000100L
#define CONFIG_SWR_LOW_ACTIVE				0x00000200L
#define CONFIG_SWR_EQ_POSLIMIT				0x00000400L
#define CONFIG_SW1_ENABLED					0x00000800L
#define CONFIG_SW1_LOW_ACTIVE				0x00001000L
#define CONFIG_SW1_EQ_POSLIMIT				0x00002000L
#define CONFIG_SW2_ENABLED					0x00004000L
#define CONFIG_SW2_LOW_ACTIVE				0x00008000L
#define CONFIG_SW2_EQ_POSLIMIT				0x00010000L
#define CONFIG_LINEAR_DRIVE					0x00020000L
#define CONFIG_OPENBRAKE_ON_SW2				0x00040000L
#define CONFIG_ALLOW_FULL_CUR				0x00080000L
#define CONFIG_M3_COMPATIBLE				0x00100000L
#define CONFIG_LINEAR_SCREW					0x00200000L
#define CONFIG_SYNC_MOTION					0x08000000L
#define CONFIG_CAN_RS232_COMM				0x40000000L
#define CONFIG_PROFI_RS485_COMM				0x80000000L

#define P3502_CONFIG_BRAKE_PRESENT          0x00000008L
#define P3502_CONFIG_BRAKE_AT_POWERON       0x00000010L
#define P3502_CONFIG_SWR_WITH_ENCODERZERO   0x00000020L
#define P3502_CONFIG_SWR_AT_FALLING_EDGE    0x00000040L
#define P3502_CONFIG_CHANGE_SWR_TO_LIMIT    0x00000080L
#define P3502_CONFIG_SWR_ENABLED        	0x00000100L
#define P3502_CONFIG_SWR_LOW_ACTIVE         0x00000200L
#define P3502_CONFIG_SWR_USE_EXTERNAL       0x00000400L
#define P3502_CONFIG_SW1_ENABLED            0x00000800L
#define P3502_CONFIG_SW1_LOW_ACTIVE         0x00001000L
#define P3502_CONFIG_SW1_USE_EXTERNAL       0x00002000L
#define P3502_CONFIG_SW2_ENABLED			0x00004000L
#define P3502_CONFIG_SW2_LOW_ACTIVE         0x00008000L
#define P3502_CONFIG_SW2_USE_EXTERNAL		0x00010000L
#define P3502_CONFIG_LINEAR_DRIVE			0x00020000L
#define P3502_CONFIG_ALLOW_FULL_CUR			0x00080000L
#define P3502_CONFIG_M3_COMPATIBLE			0x00100000L
#define P3502_CONFIG_LINEAR_SCREW			0x00200000L
#define P3502_CONFIG_DISABLE_ON_HALT		0x00800000L
#define P3502_CONFIG_WATCHDOG_ENABLE		0x01000000L
#define P3502_CONFIG_ZERO_MOVE_AFTER_HOK	0x02000000L
#define P3502_CONFIG_DISABLE_ACK			0x04000000L

//-----------------------------------------------------------------------
// old PCube Module Setup
//-----------------------------------------------------------------------

// ---- FEEDBACK SETUP -----------
#define P3502_SETUP_ENCODER_FEEDBACK		0x00000001L
#define P3502_SETUP_RESOLVER_FEEDBACK		0x00000002L
#define P3502_SETUP_ABSOLUTE_FEEDBACK		0x00000004L
// ---- DIGITAL IO SETUP ---------
#define P3502_SETUP_4IN_4OUT                0x00000008L
#define P3502_SETUP_3IN_ENCODER_IN			0x00000010L
#define P3502_SETUP_3IN_ENCODER_OUT			0x00000020L
// ---- COMM INTERFACE -----------
#define P3502_SETUP_RS232					0x00000040L
#define P3502_SETUP_RS485HD					0x00000080L
#define P3502_SETUP_RS485FD					0x00000100L
#define P3502_SETUP_CAN						0x00000200L
#define P3502_SETUP_PROFIBUS				0x00000400L
// ---- CAN ID SETUP -------------
#define P3502_SETUP_USE_M3ID				0x00000800L
#define P3502_SETUP_USE_M4ID				0x00001000L
#define P3502_SETUP_USE_CANOPEN				0x00002000L
// ---- MOTOR SETUP --------------
#define P3502_SETUP_INVERT_MOTORDIR         0x00004000L
// ---- INPUT USAGE --------------
#define P3502_SETUP_USE_SW2_AS_ENABLE		0x00008000L
#define P3502_SETUP_USE_SW2_AS_BRAKE		0x00010000L
// ---- OUTPUT USAGE -------------
#define P3502_SETUP_ERROR_TO_OUT0			0x00020000L
// ---- COUNTER SETUP ------------
#define P3502_SETUP_INVERT_COUNTER			0x00040000L

//-----------------------------------------------------------------------
// old PCube Functionnames
//-----------------------------------------------------------------------

#define PCube_getCubeState			PCube_getModuleState
#define PCube_getDefCubeVersion	PCube_getModuleVersion
#define PCube_getModulesIdMap		PCube_getModuleIdMap
#define PCube_getDefBurncount		PCube_getDefBurnCount
#define PCube_getDefSerialNo		PCube_getModuleSerialNo
#define PCube_getDefPulsesPerTurn	PCube_getDefIncPerTurn
#define PCube_getDefDioSetup		PCube_getDefDioData
#define PCube_getActPos				PCube_getPos
#define PCube_getActVel				PCube_getVel
#define PCube_getActPosInc			PCube_getPosInc
#define PCube_syncModule			PCube_homeModule
#define PCube_moveCurrent			PCube_moveCur
#define PCube_moveCurrentInc		PCube_moveCurInc
#define PCube_moveCurrentExtended	PCube_moveCurExtended
// #define DLR_FTSInit					PCube_initDLR_FTS
// #define DLR_FTSGetData				PCube_getDataDLR_FTS
#define PCube_getStatusInternal		PCube_getStateInternal
#define PCube_ConfigFromFile		PCube_configFromFile
#define PCube_getInitString			PCube_getDeviceInitString
#define PCube_getName				PCube_getDeviceName
#define PCube_getRevision			PCube_getDeviceRevision
#define PCube_setTargetVel			PCube_setRampVel
#define PCube_setTargetAcc			PCube_setRampAcc

//-----------------------------------------------------------------------
// new PCube Functionnames
//-----------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

M5DLL_API int WINAPI PCube_getDllVersion( void );
M5DLL_API int WINAPI PCube_setDllDebug( bool bDebug, int iDebugLevel, bool bDebugFile );

M5DLL_API int WINAPI PCube_configFromFile( const char* acFileName );
M5DLL_API int WINAPI PCube_openDevice( int* piDeviceId, const char* acInitString );
M5DLL_API int WINAPI PCube_closeDevice( int iDeviceId );
M5DLL_API int WINAPI PCube_closeDevices( void );
M5DLL_API int WINAPI PCube_getDeviceCount( void );
M5DLL_API int WINAPI PCube_getDeviceIdMap( int* aiIdMap );
M5DLL_API const char* WINAPI PCube_getDeviceRevision( int iDeviceId );
M5DLL_API const char* WINAPI PCube_getDeviceInitString( int iDeviceId );
M5DLL_API const char* WINAPI PCube_getDeviceName( int iDeviceId );
M5DLL_API int WINAPI PCube_setDeviceName( int iDeviceId, const char* acDeviceName );
M5DLL_API int WINAPI PCube_setDeviceDebug( int iDeviceId, bool bDebug, int iDebugLevel, bool bDebugFile );

M5DLL_API int WINAPI PCube_getModuleCount( int iDeviceId );
M5DLL_API int WINAPI PCube_getModuleIdMap( int iDeviceId, int* aiIdMap );
M5DLL_API int WINAPI PCube_getModuleState( int iDeviceId, int iModuleId, unsigned long* puiState );
M5DLL_API int WINAPI PCube_getModuleType( int iDeviceId, int iModuleId, unsigned char* pucValue );
M5DLL_API int WINAPI PCube_getModuleVersion( int iDeviceId, int iModuleId, unsigned short* puiValue );
M5DLL_API int WINAPI PCube_getModuleSerialNo( int iDeviceId, int iModuleId, unsigned long* puiValue );

M5DLL_API int WINAPI PCube_getDefConfig( int iDeviceId, int iModuleId, unsigned long* puiValue );
M5DLL_API int WINAPI PCube_getDefSetup( int iDeviceId, int iModuleId, unsigned long* puiValue );	//OS: available from 35xx 
M5DLL_API int WINAPI PCube_getDefBaudRate( int iDeviceId, int iModuleId, unsigned char* pucValue );
M5DLL_API int WINAPI PCube_getDefBurnCount( int iDeviceId, int iModuleId, unsigned char* pucValue );
M5DLL_API int WINAPI PCube_getDefGearRatio( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefLinearRatio( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefCurRatio( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefBrakeTimeOut( int iDeviceId, int iModuleId, unsigned short* puiValue );
M5DLL_API int WINAPI PCube_getDefIncPerTurn( int iDeviceId, int iModuleId, unsigned long* puiValue );
M5DLL_API int WINAPI PCube_getDefDioData( int iDeviceId, int iModuleId, unsigned long* puiValue );	//OS: not available from 35xx 
M5DLL_API int WINAPI PCube_getDefA0( int iDeviceId, int iModuleId, short* piValue );
M5DLL_API int WINAPI PCube_getDefC0( int iDeviceId, int iModuleId, short* piValue );
M5DLL_API int WINAPI PCube_getDefDamp( int iDeviceId, int iModuleId, short* piValue );
M5DLL_API int WINAPI PCube_getDefHomeOffset( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefHomeVel( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefMinPos( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefMaxPos( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefMaxVel( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefMaxAcc( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefMaxCur( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDefMaxDeltaPos( int iDeviceId, int iModuleId, float* pfValue );

M5DLL_API int WINAPI PCube_getConfig( int iDeviceId, int iModuleId, unsigned long* puiValue );
M5DLL_API int WINAPI PCube_getIncRatio( int iDeviceId, int iModuleId, float* pValue );
M5DLL_API int WINAPI PCube_getStateDioPos( int iDeviceId, int iModuleId, unsigned long* puiShortState, unsigned char* pucDio, float* pfPos );	//OS: available from 2518 and 3518
M5DLL_API int WINAPI PCube_getDioData( int iDeviceId, int iModuleId, unsigned long* puiValue );
M5DLL_API int WINAPI PCube_getA0( int iDeviceId, int iModuleId, short* piValue );
M5DLL_API int WINAPI PCube_getC0( int iDeviceId, int iModuleId, short* piValue );
M5DLL_API int WINAPI PCube_getDamp( int iDeviceId, int iModuleId, short* piValue );
M5DLL_API int WINAPI PCube_getHomeOffset( int iDeviceId, int iModuleId, float* pValue );
M5DLL_API int WINAPI PCube_getHomeOffsetInc( int iDeviceId, int iModuleId, long* piValue );
M5DLL_API int WINAPI PCube_getHomeVel( int iDeviceId, int iModuleId, float* pfValue );		//OS: available from 2518 and 3518
M5DLL_API int WINAPI PCube_getHomeVelInc( int iDeviceId, int iModuleId, long* piValue );	//OS: available from 2518 and 3518
M5DLL_API int WINAPI PCube_getPos( int iDeviceId, int iModuleId, float* pfPos );
M5DLL_API int WINAPI PCube_getPosInc( int iDeviceId, int iModuleId, long* piValue );
M5DLL_API int WINAPI PCube_getPosCountInc( int iDeviceId, int iModuleId, long* piValue );

M5DLL_API int WINAPI PCube_getPosTime( int iDeviceId, int iModuleId, float* pfPos, unsigned short* puiTime ); // New Function, don't know which OS Version is needed

M5DLL_API int WINAPI PCube_getVel( int iDeviceId, int iModuleId, float* pfVel );
M5DLL_API int WINAPI PCube_getVelInc( int iDeviceId, int iModuleId, long* piValue );
M5DLL_API int WINAPI PCube_getIPolVel( int iDeviceId, int iModuleId, float* pValue );
M5DLL_API int WINAPI PCube_getCur( int iDeviceId, int iModuleId, float* pfCur );
M5DLL_API int WINAPI PCube_getCurInc( int iDeviceId, int iModuleId, short* piValue );
M5DLL_API int WINAPI PCube_getMinPos( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getMinPosInc( int iDeviceId, int iModuleId, long* piValue );
M5DLL_API int WINAPI PCube_getMaxPos( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getMaxPosInc( int iDeviceId, int iModuleId, long* piValue );
M5DLL_API int WINAPI PCube_getMaxVel( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getMaxVelInc( int iDeviceId, int iModuleId, long* piValue );
M5DLL_API int WINAPI PCube_getMaxAcc( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getMaxAccInc( int iDeviceId, int iModuleId, long* piValue );
M5DLL_API int WINAPI PCube_getMaxCur( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDeltaPos( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getDeltaPosInc( int iDeviceId, int iModuleId, long* piValue );
M5DLL_API int WINAPI PCube_getMaxDeltaPos( int iDeviceId, int iModuleId, float* pfValue );
M5DLL_API int WINAPI PCube_getMaxDeltaPosInc( int iDeviceId, int iModuleId, long* piValue );
M5DLL_API int WINAPI PCube_getSavePos( int iDeviceId, int iModuleId, float* pfValue );	//OS: available from 2518 and 3518
M5DLL_API int WINAPI PCube_getSyncTime( int iDeviceId, int iModuleId, short* piValue );	//OS: available from 3602

M5DLL_API int WINAPI PCube_setConfig( int iDeviceId, int iModuleId, unsigned long uiValue );
M5DLL_API int WINAPI PCube_setDioData( int iDeviceId, int iModuleId, unsigned long uiValue );
M5DLL_API int WINAPI PCube_setA0( int iDeviceId, int iModuleId, short iValue );
M5DLL_API int WINAPI PCube_setC0( int iDeviceId, int iModuleId, short iValue );
M5DLL_API int WINAPI PCube_setDamp( int iDeviceId, int iModuleId, short iValue );
M5DLL_API int WINAPI PCube_setHomeOffset( int iDeviceId, int iModuleId, float fValue );
M5DLL_API int WINAPI PCube_setHomeOffsetInc( int iDeviceId, int iModuleId, long iValue );
M5DLL_API int WINAPI PCube_setHomeVel( int iDeviceId, int iModuleId, float fValue );	//OS: available from 2518 and 3518
M5DLL_API int WINAPI PCube_setHomeVelInc( int iDeviceId, int iModuleId, long iValue );	//OS: available from 2518 and 3518
M5DLL_API int WINAPI PCube_setRampVel( int iDeviceId, int iModuleId, float fValue );			
M5DLL_API int WINAPI PCube_setRampVelInc( int iDeviceId, int iModuleId, long iValue );
M5DLL_API int WINAPI PCube_setRampAcc( int iDeviceId, int iModuleId, float fValue );			
M5DLL_API int WINAPI PCube_setRampAccInc( int iDeviceId, int iModuleId, long iValue );
M5DLL_API int WINAPI PCube_setMinPos( int iDeviceId, int iModuleId, float fValue );
M5DLL_API int WINAPI PCube_setMinPosInc( int iDeviceId, int iModuleId, long iValue );
M5DLL_API int WINAPI PCube_setMaxPos( int iDeviceId, int iModuleId, float fValue );
M5DLL_API int WINAPI PCube_setMaxPosInc( int iDeviceId, int iModuleId, long iValue );
M5DLL_API int WINAPI PCube_setMaxVel( int iDeviceId, int iModuleId, float fValue );
M5DLL_API int WINAPI PCube_setMaxVelInc( int iDeviceId, int iModuleId, long iValue );
M5DLL_API int WINAPI PCube_setMaxAcc( int iDeviceId, int iModuleId, float fValue );
M5DLL_API int WINAPI PCube_setMaxAccInc( int iDeviceId, int iModuleId, long iValue );
M5DLL_API int WINAPI PCube_setMaxCur( int iDeviceId, int iModuleId, float fValue );
M5DLL_API int WINAPI PCube_setMaxDeltaPos( int iDeviceId, int iModuleId, float fValue );
M5DLL_API int WINAPI PCube_setMaxDeltaPosInc( int iDeviceId, int iModuleId, long iValue );
M5DLL_API int WINAPI PCube_setSyncTime( int iDeviceId, int iModuleId, short iValue );	//OS: available from 3602

M5DLL_API int WINAPI PCube_homeModule( int iDeviceId, int iModuleId );
M5DLL_API int WINAPI PCube_haltModule( int iDeviceId, int iModuleId );
M5DLL_API int WINAPI PCube_resetModule( int iDeviceId, int iModuleId );
M5DLL_API int WINAPI PCube_recalcPIDParams( int iDeviceId, int iModuleId );

M5DLL_API int WINAPI PCube_movePos( int iDeviceId, int iModuleId, float fPos );
M5DLL_API int WINAPI PCube_movePosInc( int iDeviceId, int iModuleId, long iPos );
M5DLL_API int WINAPI PCube_movePosExtended( int iDeviceId, int iModuleId, float fPos, unsigned long* puiShortState, unsigned char* pucDio, float* pfPos );
M5DLL_API int WINAPI PCube_moveRamp( int iDeviceId, int iModuleId, float fPos, float fVel, float fAcc );
M5DLL_API int WINAPI PCube_moveRampInc( int iDeviceId, int iModuleId, long iPos, long iVel, long iAcc );
M5DLL_API int WINAPI PCube_moveRampExtended( int iDeviceId, int iModuleId, float fPos, float fVel, float fAcc, unsigned long* puiState, unsigned char* pucDio, float* pfPos );
M5DLL_API int WINAPI PCube_moveVel( int iDeviceId, int iModuleId, float fVel );
M5DLL_API int WINAPI PCube_moveVelInc( int iDeviceId, int iModuleId, long iVel );
M5DLL_API int WINAPI PCube_moveVelExtended( int iDeviceId, int iModuleId, float fCur, unsigned long* puiShortState, unsigned char* pucDio, float* pfPos );
M5DLL_API int WINAPI PCube_moveCur( int iDeviceId, int iModuleId, float fCur );
M5DLL_API int WINAPI PCube_moveCurInc( int iDeviceId, int iModuleId, long iCur );
M5DLL_API int WINAPI PCube_moveCurExtended( int iDeviceId, int iModuleId, float fCur, unsigned long* puiShortState, unsigned char* pucDio, float* pfPos );
M5DLL_API int WINAPI PCube_moveStep( int iDeviceId, int iModuleId, float fPos, unsigned short uiTime );
M5DLL_API int WINAPI PCube_moveStepInc( int iDeviceId, int iModuleId, long iPos, unsigned short uiTime );
M5DLL_API int WINAPI PCube_moveStepExtended( int iDeviceId, int iModuleId, float fPos, unsigned short uiTime, unsigned long* puiShortState, unsigned char* pucDio, float* pfPos );

M5DLL_API int WINAPI PCube_homeAll( int iDeviceId );
M5DLL_API int WINAPI PCube_resetAll( int iDeviceId );
M5DLL_API int WINAPI PCube_haltAll( int iDeviceId );
M5DLL_API int WINAPI PCube_serveWatchdogAll( int iDeviceId );
M5DLL_API int WINAPI PCube_setBaudRateAll( int iDeviceId, unsigned char pucValue );
M5DLL_API int WINAPI PCube_startMotionAll( int iDeviceId );
M5DLL_API int WINAPI PCube_savePosAll( int iDeviceId );

M5DLL_API int WINAPI PCube_waitForHomeEnd( int iDeviceId, int iModuleId, unsigned long uiTime );
M5DLL_API int WINAPI PCube_waitForMotionEnd( int iDeviceId, int iModuleId, unsigned long uiTime );
M5DLL_API int WINAPI PCube_waitForRampEnd( int iDeviceId, int iModuleId, unsigned long uiTime );
M5DLL_API int WINAPI PCube_waitForRampDec( int iDeviceId, int iModuleId, unsigned long uiTime );
M5DLL_API int WINAPI PCube_waitForRampSteady( int iDeviceId, int iModuleId, unsigned long uiTime );
M5DLL_API int WINAPI PCube_waitForHomeEndAll( int iDeviceId, unsigned long uiTime );
M5DLL_API int WINAPI PCube_waitForMotionEndAll( int iDeviceId, unsigned long uiTime );
M5DLL_API int WINAPI PCube_waitForRampEndAll( int iDeviceId, unsigned long uiTime );
M5DLL_API int WINAPI PCube_waitForStartMotionAll( int iDeviceId );

M5DLL_API int WINAPI PCube_xmit8Bytes( int iDeviceId, int iModuleId, void* pBytes );
M5DLL_API int WINAPI PCube_xack8Bytes( int iDeviceId, int iModuleId, void* pBytes );
M5DLL_API int WINAPI PCube_doInternal( int iDeviceId, int iModuleId, void* pBytes );
M5DLL_API int WINAPI PCube_getStateInternal( int iDeviceId, int iModuleId, unsigned long* puiState );
M5DLL_API int WINAPI PCube_updateModuleIdMap( int iDeviceId );
M5DLL_API int WINAPI PCube_getDefCurOffset( int iDeviceId, int iModuleId, float* pfValue );

#ifdef __cplusplus
	}
#endif

#endif