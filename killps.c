//특정포트또는 ID를 kill 함

#include <ctype.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termio.h>
#include <time.h>
#define No 0
#define Yes 1;
#define byte	unsigned char
#define word	unsigned int
#define PS_TEMP  "/tmp/killps.tmp"


strlwr(byte *str)
{
	for(; *str ;str++)
	if(isupper(*str))
		*str=tolower(*str);
}


strupr(byte *str)
{
	for(; *str ;str++)
	if(islower(*str))
		*str=toupper(*str);
}




main(argc, argv)
int argc;
char *argv[];
{
	char buff[300];
	FILE *fp;
    int n;
    int found=No;
    int mode,pid;

	if (argc<2) {
        printf("\n지정 프로세스 KILL : 사용법  killps <프로세스명>\n");
		return;
	}
    if (argv[1]==0) return;
    sprintf(buff,"ps ax > %s",PS_TEMP);
    system(buff);
    if ((fp=fopen(PS_TEMP,"r+"))==NULL) return;
    while (fgets(buff,200,fp)!=NULL) {
        if (stristr(buff,argv[1])&&!stristr(buff,argv[0])) {
            n=atoi(buff);
            printf("KILL : %d   %s",n,buff);
            sprintf(buff,"kill -9 %d",n);
	    system(buff);
 //           kill(n,SIGHUP);
            found=Yes;
        }
    }
    if (fp!=NULL) fclose(fp);
    unlink(PS_TEMP);
    if (!found) printf(" ## (%s) PROCESSOR NOT FOUND!! ##\n",argv[1]);
}



//두개의 문자열을 포함여부확인 포함되면 Yes
int stristr(const char *s1,const char *s2)
{
	int i=0,nn;
	char ss1[300];
	char ss2[300];

	if (s1[0]==0||s2[0]==0) return No;
	strcpy(ss1,s1);
	strcpy(ss2,s2);

	strlwr(ss1);
	strlwr(ss2);
	if (strstr(ss1,ss2)==NULL) return No;
	return Yes;
}


