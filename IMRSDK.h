// Copyright (c) Intel Corporation, 2003 - 2011  All Rights Reserved.

#ifndef _IMR_SDK_H_
#define _IMR_SDK_H_

#ifdef __IMR_WIN__
#ifndef _LIB
#define EXPORTED_FUNC __declspec(dllexport)
#else
#define EXPORTED_FUNC
#endif
#else
#define EXPORTED_FUNC
#endif

#ifndef _WIN32
#define __stdcall 
//Just a declaration for Linux compilation
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef BOOL
	typedef int            BOOL;
	#define TRUE			1
	#define	FALSE			0
#endif

#define MAX_IP_LEN			128
#define MAX_NAME_LEN		128
#define MAX_COMP_NAME		128
#define MAX_PSWD_LEN		128
#define KEY_LEN				20
#define GUID_LEN			16
#define MAX_WIN_UN_LEN 257 //256 + NULL
#define MAX_WIN_DN_LEN 254 //253 + NULL

// This constant is not used by the redirection library.
// Applications should define their own (preferably larger)
// constant as this one may one day be removed.
#define MAX_SOL_MESSAGE		0xFF

#define INVALID_CLIENT_ID (unsigned int)-1
typedef unsigned int ClientID;

#define INVALID_SESSION_ID (unsigned int)-1
typedef unsigned int SessionID;

typedef enum {
	IMR_RES_OK, 
	IMR_RES_ERROR, 
	IMR_RES_INVALID_PARAMETER,
	IMR_RES_NOT_INITIALIZED,
	IMR_RES_ALREADY_INITIALIZED,
	IMR_RES_MEMALLOC_FAILED,
	IMR_RES_UNSUPPORTED,
	IMR_RES_CLIENT_NOT_FOUND,
	IMR_RES_DUPLICATE_CLIENT,
	IMR_RES_CLIENT_NOT_ACTIVE,
	IMR_RES_CLIENT_ACTIVE,
	IMR_RES_SESSION_ALREADY_OPEN,
	IMR_RES_SESSION_CLOSED,
	IMR_RES_SOCKET_ERROR,
	IMR_RES_UNKNOWN_PROTOCOL,
	IMR_RES_PROTOCOL_ALREADY_REGISTERED,
	IMR_RES_PENDING,
	IMR_RES_UNEXPECTED_PACKET,
	IMR_RES_TIMEOUT,
	IMR_RES_CORRUPT_PACKET,
	IMR_RES_OS_ERROR,
	IMR_RES_IDER_VERSION_NOT_SUPPORTED,
	IMR_RES_IDER_COMMAND_RUNNING,
	IMR_RES_STORAGE_FAILURE,
	IMR_RES_UNKNOWN,
	IMR_RES_AUTH_FAILED,
	IMR_RES_CLIENT_TYPE_UNKNOWN,
	IMR_RES_CLIENT_BUSY,
	IMR_RES_CLIENT_UNSUPPORTED,
	IMR_RES_CLIENT_ERROR,
	IMR_RES_NOT_ENOUGH_SPACE,
	IMR_RES_SESSION_LOOPBACK,
	IMR_RES_TLS_CONNECTION_FAILED,
	IMR_RES_SOCKS_ERROR,
	IMR_RES_SOCKS_AUTH_FAIL,
	IMR_RES_OOB_SOL_DISABLED,
	IMR_RES_AUDIT_LOG_FULL,
	IMR_RES_CLIENT_INVALID_OPTION,
	IMR_RES_USER_CONSENT_REQUIRED,
	IMR_RES_COUNT,   // must be last entry

} IMRResult;


// Valid enum values when optname is SET_CLIENT_OPTION_LINK_PREFERENCE
typedef enum
{
	LEGACY_ME_LINK_CONTROL = 0,
	CONSOLE_LINK_PREFERENCE = 1
}CLIENT_LINK_PREFERENCE_OPTION;

// Valid values for optname when using the functions
// IMR_SetOpt / IMR_GetOpt
typedef enum {
	// This option should be used prior opening a session
	SET_CLIENT_OPTION_LINK_PREFERENCE = 0 /* Value should be 0 or 1 */
} SetClientOption;


typedef struct {
	unsigned short major;
	unsigned short minor;
} IMRVersion;

typedef enum {CLI_TCP = 1, CLI_TLS = 2, CLI_UNKNOWN } ClientType;


typedef char GUIDType[GUID_LEN];

typedef struct {
	ClientType	type;
	char		ip[MAX_IP_LEN];
	GUIDType	guid;
} ClientInfo;

//IN Parameter for IDER/SOL session 

typedef struct {
	char user_name[MAX_NAME_LEN]; // user name - nul terminated string 
	char user_pswd[MAX_PSWD_LEN]; // user password - nul terminated string 
} TCPSessionParams;

typedef enum {PROXY_NO_PROXY, PROXY_SOCKS_V5} PROXY_TYPE;

typedef struct {
		PROXY_TYPE type;
		char server[MAX_IP_LEN]; // hostname or IPv4 (IPv6 not supported yet) “hostname”, “a.b.c.d”
		int port;
		char user[MAX_NAME_LEN]; // user name - nul terminated string 
		char password[MAX_PSWD_LEN]; // user password - nul terminated string 
} ProxySettings;

typedef struct {
	int version; // It must be 2.
	char user_name[MAX_NAME_LEN]; // user name - nul terminated string 
	char user_pswd[MAX_PSWD_LEN]; // user password - nul terminated string 
	ProxySettings proxy_settings;
} TCPSessionParamsEx;

typedef struct {
	int version; // It must be 3.
	char user_name[MAX_WIN_UN_LEN]; // user name - NULL terminated string 
	char user_pswd[MAX_PSWD_LEN]; // user password - NULL terminated string 
	char user_domain[MAX_WIN_DN_LEN]; //user domain - NULL terminated string 
	ProxySettings proxy_settings;
} TCPSessionParamsEx2;

//IN Parameter for SOL session 
typedef struct {
	unsigned short	tx_over_timeout;
	unsigned short	tx_buf_timeout;
	unsigned short	hb_interval;
	unsigned short	fifo_rx_flush_timeout;
	unsigned short	rx_timeout;
} SOLTout; 

//IN Parameter for IDER session 
typedef struct {
	unsigned short	rx_timeout;	//client host RX timeout as defined in the IDER spec.
	unsigned short	tx_timeout; //client host TX timeout as defined in the IDER spec.
	unsigned short	hb_timeout; //client FW HB interval  as defined in the IDER spec.
} IDERTout; 

//OUT parameter for querying supported IDER client features
typedef struct {
	BOOL ider_dev_pri; //TRUE if client FW supports enable/disable of primary IDE devices
	BOOL ider_dev_sec; //TRUE if client FW supports enable/disable of secondary IDE devices
	BOOL reserved[30];
}FeaturesSupported;

typedef enum {IDER_ENABLE, IDER_DISABLE, IDER_NOP } SetOperation;
//Timing options for the IDE device enabling/disabling:
//ONRESET - Client should perform command at the next FW RESET.
//GRACEFULLY - Client should perform command if there is no other host command in progress.
//IMMEDIATELY - Client should perform command immediately.
typedef enum {IDER_SET_ONRESET, IDER_SET_GRACEFULLY, IDER_SET_IMMEDIATELY,  } SetOption;

typedef enum { IDER_DISABLED, IDER_ENABLED  } DeviceState;

//Client returns REJECTED if there is a Host Command in progress
//and the Disable/Enable command can't be performed at this time
typedef enum { IDER_REJECTED, IDER_DONE } SetResult;

//IN parameter for querying client IDE devices state
typedef struct  {
	DeviceState	pri_default;	//Default primary devices state at the Client FW
	DeviceState	pri_current;	//Current primary devices state at the Client FW
	DeviceState	sec_default;	//Default secondary devices state at the Client FW
	DeviceState	sec_current;	//Default secondary devices state at the Client FW
} IDERDeviceState;

//IN paramter for changing client IDE devices state
typedef struct {
	SetOperation	pri_op;		//Pimary device operation (enable or disable)
	SetOption		pri_timing; //Pimary device operation timing 
	SetOperation	sec_op;		//Not supported by the SDK yet 
	SetOption		sec_timing; //Not supported by the SDK yet
} IDERDeviceCmd;

//OUT parameter - IDERDeviceCmd execution result returned by the client
typedef struct {
	SetResult	pri_res;
	SetResult	sec_res;
} IDERDeviceResult;

typedef struct {
		BOOL				error_state;	//TRUE if session is in ERROR state
		BOOL				data_transfer;  //TRUE if there is a read/write command in progress
		unsigned short		num_reopen;		//number of session re-opens due to error recovery
		unsigned int		num_error;		//number of ErrorOccured messages received
		unsigned int		num_reset;		//number of ResetOccured messages received
		unsigned int		last_cmd_length;//last data transfer (read/write) length in bytes
		unsigned int		data_sent;		//bytes of data sent to the client
		unsigned int		data_received;	//bytes of data received from the client
		unsigned int		packets_sent;	//mesages sent during the session 
		unsigned int		packets_received;//messages received during the session
} IDERStatistics;

typedef enum {SEC_LEVEL_NONE, SEC_LEVEL_AUTH, SEC_LEVEL_ENCR, SEC_LEVEL_AUTH_ENCR } SecLevel;

typedef enum { SOL_LOOPBACK_NONE = 0, SOL_LOOPBACK_RS232C = 1 } SOLLoopbackMode;

/*
	Callback function will be used in order to delegate the socket managements
	to another application
*/

typedef  int (__stdcall *Open)(unsigned int clientID, unsigned int conID);
typedef  int (__stdcall *Close)(unsigned int conID);
typedef  int (__stdcall *Receive)(unsigned char* buffer, unsigned int* length, unsigned int conID);
typedef  int (__stdcall *Send)(unsigned char* buffer, unsigned int length, unsigned int conID);

typedef struct _SocketsCallbacks
{
	Open   		open_connection;
	Close  		close_connection;
	Receive		receive_data;
	Send		send_data;
}SocketsCallbacks;

//General:
EXPORTED_FUNC IMRResult IMR_Init(IMRVersion *version, char *ini_file);
EXPORTED_FUNC IMRResult IMR_InitEx(IMRVersion *version, char *ini_file, SocketsCallbacks* socketsCallbacks);
EXPORTED_FUNC IMRResult IMR_ReadyReadSock(unsigned int connId);
EXPORTED_FUNC IMRResult IMR_Close();
EXPORTED_FUNC IMRResult IMR_GetErrorStringLen(IMRResult, int * str_len);
EXPORTED_FUNC IMRResult IMR_GetErrorString(IMRResult, char *str);
EXPORTED_FUNC IMRResult IMR_SetCertificateInfo(const char *root_cert, const char *private_cert, const char *cert_pass);
#ifdef _WIN32
EXPORTED_FUNC IMRResult IMR_SetClientCertificate(const char *common_name);
#endif

//Client List:
EXPORTED_FUNC IMRResult IMR_AddClient( ClientType new_client_type, char *client_ip, GUIDType client_guid, ClientID *new_client_id);
EXPORTED_FUNC IMRResult IMR_RemoveClient(ClientID client_id);
EXPORTED_FUNC IMRResult IMR_RemoveAllClients();
EXPORTED_FUNC IMRResult IMR_GetAllClients(ClientID *client_list, int *client_list_size);

EXPORTED_FUNC IMRResult IMR_GetClientInfo(ClientID client_id, ClientInfo * );



//SOL:
EXPORTED_FUNC IMRResult IMR_SOLOpenTCPSession(ClientID  client_id, TCPSessionParams * params,
							SOLTout* touts , SOLLoopbackMode *loopback);
EXPORTED_FUNC IMRResult IMR_SOLOpenTCPSessionEx(ClientID  client_id, TCPSessionParamsEx * params,
							SOLTout* touts , SOLLoopbackMode *loopback);
EXPORTED_FUNC IMRResult IMR_SOLOpenTCPSessionEx2(ClientID  client_id, TCPSessionParamsEx2 * params,
							SOLTout* touts , SOLLoopbackMode *loopback);
EXPORTED_FUNC IMRResult IMR_SOLOpenTCPSessionOOB(ClientID  client_id, TCPSessionParamsEx * params,
							SOLTout* touts , SOLLoopbackMode *loopback);
EXPORTED_FUNC IMRResult IMR_SOLOpenTCPSessionOOBEx(ClientID  client_id, TCPSessionParamsEx2 * params,
							SOLTout* touts , SOLLoopbackMode *loopback);

EXPORTED_FUNC IMRResult IMR_SOLCloseSession(ClientID  client_id);
EXPORTED_FUNC IMRResult IMR_SOLSendText(ClientID  client_id, unsigned char *data, int data_len);
EXPORTED_FUNC IMRResult IMR_SOLReceiveText(ClientID  client_id, unsigned char *data, int *data_len);

//IDER:

//
//client_id - unique client ID, as returned by the IMR_AddClient
//params	- pointer to the IDER over TCP/TLS session paramters structure
//touts		- pointer to the IDER over TCP/TLS session timeouts structure
//drive0	- floppy drive name for the IDER over TCP/TLS session. should be null terminated.
//drive1    - CD drive name for the IDER over TCP/TLS session. should be null terminated.
//returns   - IMR_RES_OK on success, error otherwise.
//
//Use this function to establish a new IDER session with the client. 
//Function will create a new connection and exchange messages needed in order
//to open IDER session. It will return only when the session is established, or 
//error occured. After the session opening client will initiate data transfers.
EXPORTED_FUNC IMRResult IMR_IDEROpenTCPSession(ClientID  client_id, TCPSessionParams * params,
								IDERTout* touts ,
								char *drive0, char *drive1);

//
//client_id - unique client ID, as returned by the IMR_AddClient
//params	- pointer to the IDER over TCP/TLS session paramters extended structure
//touts		- pointer to the IDER over TCP/TLS session timeouts structure
//drive0	- floppy drive name for the IDER over TCP/TLS session. should be null terminated.
//drive1    - CD drive name for the IDER over TCP/TLS session. should be null terminated.
//returns   - IMR_RES_OK on success, error otherwise.
//
//Use this function to establish a new IDER session with the client. 
//Function will create a new connection and exchange messages needed in order
//to open IDER session. It will return only when the session is established, or 
//error occured. After the session opening client will initiate data transfers.
EXPORTED_FUNC IMRResult IMR_IDEROpenTCPSessionEx(ClientID  client_id, TCPSessionParamsEx * params,
								IDERTout* touts ,
								char *drive0, char *drive1);

//client_id - unique client ID, as returned by the IMR_AddClient
//params	- pointer to the IDER over TCP/TLS session paramters extended structure
//touts		- pointer to the IDER over TCP/TLS session timeouts structure
//drive0	- floppy drive name for the IDER over TCP/TLS session. should be null terminated.
//drive1    - CD drive name for the IDER over TCP/TLS session. should be null terminated.
//returns   - IMR_RES_OK on success, error otherwise.
//
//Use this function to establish a new IDER session with the client. 
//Function will create a new connection and exchange messages needed in order
//to open IDER session. It will return only when the session is established, or 
//error occured. After the session opening client will initiate data transfers.
EXPORTED_FUNC IMRResult IMR_IDEROpenTCPSessionEx2(ClientID  client_id, TCPSessionParamsEx2 * params,
								IDERTout* touts ,
								char *drive0, char *drive1);
//
//client_id - unique client ID, as returned by the IMR_AddClient
//returns   - IMR_RES_OK on success, error otherwise.
//
//Use this function to close an active IDER session. After the function returns
//no other operations can be done succesfully on the session. Closing a session during 
//write data operation can cause data corruption. 
EXPORTED_FUNC IMRResult IMR_IDERCloseSession(ClientID  client_id);
//
//client_id - unique client ID, as returned by the IMR_AddClient
//supported - pointer to the FeaturesSupported structure filled by the SDK
//returns   - IMR_RES_OK on success, error otherwise.
//Use this function to query the Client about the special features it supports.
//Currently the only special feature defined is an ability to disable/enable host IDE devices.
EXPORTED_FUNC IMRResult IMR_IDERClientFeatureSupported(ClientID  client_id, FeaturesSupported *supported);
//
//client_id - unique client ID, as returned by the IMR_AddClient
//state		- pointer to the IDERDeviceState structure filled by the SDK
//returns   - IMR_RES_OK on success, error otherwise.
//Use this function to query host IDE device(s) state. 
EXPORTED_FUNC IMRResult IMR_IDERGetDeviceState(ClientID  client_id, IDERDeviceState *state);
//
//client_id - unique client ID, as returned by the IMR_AddClient
//cmd 		- pointer to the IDERDeviceCmd structure that shoudl contain disable/enable command and options
//result	- pointer to the IDERDeviceResult structure filled by the SDK
//returns   - IMR_RES_OK on success, error otherwise.
//Use this function to control host IDE device(s) state. Devices can be disabled/enabled through
//this API. 
EXPORTED_FUNC IMRResult IMR_IDERSetDeviceState(ClientID  client_id, IDERDeviceCmd *cmd, IDERDeviceResult *result);
//
//client_id - unique client ID, as returned by the IMR_AddClient
//stat		- pointer to the IDERStatistics filled by the SDK
//returns   - IMR_RES_OK on success, error otherwise.
//
//Use this functon in order to poll active IDER session.
//If the IDER session for the corresponding client was closed due to timeout/protocol error
//function will fill the stat structure with the latest data, but will return IMR_RES_SESSION_CLOSED.
EXPORTED_FUNC IMRResult IMR_IDERGetSessionStatistics(ClientID  client_id, IDERStatistics * stat );

// valid optnames defined in SetClientOption enum
//
// if SetClientOption is SET_CLIENT_OPTION_LINK_PREFERENCE
// 		the possible values are:
// 			LEGACY_ME_LINK_CONTROL(default)
//			CONSOLE_LINK_PREFERENCE
// 		and the Optlen is sizeof(int).

//If ClientID is INVALID_CLIENT_ID then the setting corresponds to a global setting.
// Otherwise, id must be a valid client id
EXPORTED_FUNC IMRResult IMR_SetOpt(/*IN*/ClientID id, /*IN*/int optname, /*IN*/ const char *optval, /*IN*/ int optlen);

//If ClientID is INVALID_CLIENT_ID then the setting corresponds to a global setting.
// Otherwise, id must be a valid client id
EXPORTED_FUNC IMRResult IMR_GetOpt(/*IN*/ClientID id, /*IN*/int optname, /*OUT*/ char *optval, /*OUT*/ int* optlen);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_IMR_SDK_H_

