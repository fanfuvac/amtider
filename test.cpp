#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>

#include  <iostream>
#include <string>
using namespace std;
int main(void){
unsigned char * data=new unsigned char [10];
memcpy ( data, "s bla", strlen("s bla")+1 );
cout<<"a"<<data[3]<<endl;
}