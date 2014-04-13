#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>
#include "IMRSDK.h"
#include "error-cast.h"
#include  <iostream>
#include <string>

using namespace std; 																																																																																										
#define MAX_CLIENT_NAME 128
#define DEF_FLOPPY_DEV "/dev/fd0"
#define DEF_CDROM_DEV "/dev/cdrom"

#define DEF_IDER_RX_TIMEOUT 10000
#define DEF_IDER_TX_TIMEOUT 0
#define DEF_IDER_HB_TIMEOUT 5000

#define DEF_SOL_RX_TIMEOUT 10000
#define DEF_SOL_BUF_TIMEOUT 100
#define DEF_SOL_HB_INTERVAL 5000
#define DEF_SOL_TX_OVER_TIMEOUT 0
#define DEF_SOL_FIFO_RX_FLUSH_TIMEOUT 100

#define MAX_FILE_NAME_LEN 1024
#define DEF_ROOT_CERT_FILE ""
#define DEF_PRIVATE_CERT_FILE ""
#define DEF_PRIVATE_CERT_PASS ""
void printErr(string text,IMRResult res){
	char *str;
	str=new char[100];
	IMR_GetErrorString(res,str);
	cout<<text<<" : "<<str<<endl;
	if(res!=0) exit(1);
	

}
//add client to list of clients, connot work with client without that
//IMR_AddClient(ClientType type,char *ip,GUIDType guid,ClientID *id);
ClientID * addClient(string host){
	ClientID *id;
	IMRResult res;
	res=IMR_AddClient(CLI_TCP,(char*)host.c_str(),NULL,id);
	printErr("Add actual client",res);
	return id;
}


void openSessionIDER(ClientID *id,string cd, string fd, char* password){
	IMRResult res;
	TCPSessionParams *params=new TCPSessionParams;
	char username[]="admin";
	memcpy ( params->user_name, username, strlen(username)+1 );
	memcpy ( params->user_pswd, password, strlen(password)+1 );
	res=IMR_IDEROpenTCPSession(*id,params,NULL,(char*)cd.c_str(),(char*)fd.c_str());
	
	printErr(string("IDER connect"),res);
}
void enableIDERdevice(ClientID *id){
	IMRResult res;
	IDERDeviceCmd cmd;
	cmd.pri_op=IDER_ENABLE;
	IDERDeviceResult result;
	res=IMR_IDERSetDeviceState(*id,&cmd,&result);
	printErr(string("IDER enable redirection"),res);
}
void usage(){
	cout<<"\
This is amtider, release 0.5, Ill establish\n\
IDE-R connections to your Intel AMT boxes.\n\
\n\
usage: amtider [options] host\n\
options:\n\
   -h            print this text\n\
   -v            verbose (default)\n\
   -q            quiet\n\
   -u user       username (default: admin)\n\
   -p pass       password (default: $AMT_PASSWORD)\n\
   -c path       path to *.iso image of redirected CD\n\
   -f path       path to *.img image of redirected FD\n\
\n\
By default port 16994 is used.\n\
If no password is given amtterm will ask for one.\n\
\n\
--\n\
(c) 2014 Václav Fanfule <fanfuvac@fit.cvut.cz>\n\
"<<endl;
}
int main(int argc, char** argv){
	string cd,fd,host;
	char * password;
	int pwdset=0;
	for (int i = 1; i < argc; ++i) {
		string arg=string(argv[i]);
		if (arg == "--cd" || arg == "-c"  ) {
		    if (i + 1 < argc) {
		        cd = argv[++i];
		    } else {
		          cerr << "--cd option requires one argument." << std::endl;
		        return 1;
		    }  
		} 
		else if  (arg == "--fd" || arg == "-f"  ){
		   if (i + 1 < argc) { 
		        fd = argv[++i]; 
		    } else {
		          cerr << "--fd option requires one argument." << std::endl;
		        return 1;
		    }   
		}
		else if  (arg == "--password" || arg == "-p"  ){
		   if (i + 1 < argc) { 
		        password = argv[++i]; 
			pwdset=1;
			cout<<password<<endl;
		    } else {
		          cerr << "--password option requires one argument." << std::endl;
		        return 1;
		    }   
		}
		
		else if (i + 1 == argc)
			host=arg;
    	}

	if (cd==""||fd==""||host==""){
		usage();
		return 1;
	}
	if(pwdset==0){
		password = (char*)getenv( "AMT_PASSWORD" );
		     if ( password == 0 ) {
			 cout<<"AMT_PASSWORD not set"<<endl;
			return 1;
		     }
	}
	IMRResult res;
	//init AMT
	res=IMR_Init(NULL, (char*)"MC.ini");
	printErr("Initialize",res);

	res=IMR_RemoveAllClients(); //could make problems if there were clients from last run
	printErr("Remove old clients",res);	
	ClientID *id=addClient(host);
	ClientID id2=*id;
	
	openSessionIDER(&id2,cd,fd,password);
	enableIDERdevice(&id2);
	cout<<"IDE redirection active"<<endl;
	while(1);



}
