////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 내용 : 네트워크 테스팅
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string.h>
#include <strings.h>

#define HTTPD_PORT  80
#define TELNET_PORT  23
int net;
int TIMEOUT;           //10초후정지
char iaddr[200];
char haddr[200];
char status[200];

void sigalarm(int sig)
{
    char buff[100];
    if (net>0) close(net);
    printf("\n10초의 시간초과로 네트워크에 연결되지 않습니다.\n");
	signal(SIGALRM,SIG_IGN);
    exit(-1);
    return;
}




main(argc, argv)
int argc;
char *argv[];
{
    char hostp[100];
    int ret,n;
    char buff[100];

	if (argc<2) {	//root모드로 설정 그냥exec
        printf ("\n telnet 접속확인 프로그램  check_tel <주소>\n");
        printf ("해당 연결이 되면 exit 1\n");
		return;
	}
    read_cfg();
    TIMEOUT=(int)cfg.telnettimeout;
    strcpy(hostp,argv[1]);
    all_trim(hostp);
    strchng9(hostp," ");
    n=check_host(hostp);
    if (n!=0) {
        exit(-1);      //잘못될때
    }
    exit (0);
}

int check_host(char *hostp)
{
    register struct hostent *host = 0;
    struct sockaddr_in sin;
    struct servent *sp = 0;
    long atemp;
    extern char *inet_ntoa();
    char *srp = 0, *strrchr();
    unsigned long sourceroute(), srlen;
    char *cmd, *user = 0;
    int tos;
    int ret,i;

    signal(SIGALRM,sigalarm);
    alarm(TIMEOUT);

    /* clear the socket address prior to use */
    bzero((char *)&sin, sizeof(sin));

	srp = 0;

//    sin.sin_addr.s_addr = atemp;
    sin.sin_family = AF_INET;

    host = gethostbyname(hostp);

    if (host) {
        sin.sin_family = host->h_addrtype;
        memcpy((caddr_t)&sin.sin_addr, host->h_addr, host->h_length);
    }
    else {
        printf(" %s 호스트를 찾을수 없습니다.\n",hostp);
        strcpy(status,"호스트검색실패");
        return -2;
    }

    sin.sin_port = TELNET_PORT;
    net=-1;

    strcpy(iaddr,inet_ntoa(sin.sin_addr));
    printf("\n%s 텔넷 접속을 테스트합니다...\n",iaddr);

    sin.sin_addr.s_addr = inet_addr(hostp);
    sin.sin_port = htons(TELNET_PORT);
    net=socket(AF_INET,SOCK_STREAM, 0);
    ret=connect(net,(struct sockaddr *)&sin,sizeof(sin));
    close(net);
    if (net < 0) {
        printf("텔넷 접속이 허용되지 않습니다.  n:%d r:%d\n",net,ret);
        strcpy(status,"텔넷접속거부");
        return -1;
    }
    strcpy(status,"정상동작");
    return 0;
}

read_cfg()
{
	FILE *fp;
	if ((fp=fopen(BBS_CFG,"r"))!=NULL) {
		fread(&cfg,sizeof_cfg,1,fp);
		fclose (fp);
	}
}


//찾는문자열 나오면 그위치부터 전부삭제
strchng9(char *src, char *search)
{
   int i, len_search;
   len_search=(int)strlen(search);
   for(i=0; src[i];i++) {
	  if(src[i] == search[0]) {
		 if(strncasecmp(&src[i], search, len_search) == 0) {
			src[i]=0;
			return Yes;
		 }
	  }
   }
   return No;
}


all_trim(char *buff)	  //양쪽에서 빈공간 제거하기  2->1
{
	char buff1[500];
	strcpy(buff1,buff);
	alltrim(buff,buff1);	  //양쪽에서 빈공간 제거하기  2->1
}


rtrim(char *buff)	   //양쪽에서 빈공간 제거하기  2->1
{
	int i;
	for (i=(int)strlen(buff);i>=0;i--) {
		if (buff[i]==0||buff[i]==' ') buff[i]=0;
		else break;
	}
}


alltrim(char *buff1,char *buff2)	  //양쪽에서 빈공간 제거하기  2->1
{
	char buff3[500];
	char buff4[500];
	int  first;
	unsigned int i,i2;

	strcpy(buff3,buff2);
	i2=0;
	first=Yes;
	strcpy(buff1,"");
	strcpy(buff3,"");
	for (i=0;i<strlen(buff2);i++) {
		if (!first||buff2[i]!=' ') {
			buff3[i2]=buff2[i];
			buff3[i2+1]=0;
			i2++;
			first=No;
		}
	}

	strcpy(buff1,"");
	i2=0;
	for (i=0;i<strlen(buff3);i++)
		if (buff3[i]!='\n'&&buff3[i]!='\r') {
			buff1[i2]=buff3[i];
			buff1[i2+1]=0;
			i2++;
		}
	rtrim(buff1);
}




