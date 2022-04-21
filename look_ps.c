/*

          XHOST 용 좀비프로세스 강제 KILL 프로그램

사용방법
 1. bbs 디렉토리에 look_ps.c를 복사한다.
 2. make look_ps 한다. -> look_ps라는 화일이 생성된다.
 3. vi /etc/crontab 한뒤 다음의 문장을 추가한다.
    (/bbs디렉토리에 look_ps 가 있는경우)
    */5 * * * * root /bbs/look_ps
 4. 5분마다 한번씩 look_ps 가 프로세스를 검색하여
    cpu 점유율이 80%가 넘는 프로세스를 KILL 한다.

*/


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
#include <fcntl.h>
int a_pid[3][100];
char a_buff[100][300];

main(argc, argv)
int argc;
char *argv[];
{
    int i,i1,i2;
    find_ps(0,80);
    sleep(10);
    find_ps(1,80);
    for (i1=0;i1<100;i1++) {
        for (i2=0;a_pid[0][i1]>0&&i2<100;i2++) {
            if (a_pid[0][i1]==a_pid[1][i2]) {
//                printf("num=%d %s",a_pid[0][i1],a_buff[i1]);
                kill(a_pid[0][i1],SIGHUP);  //동일하면 KILL
            }
        }
    }
}


find_ps(int mode,int per)
{
	FILE *fp;
    int i,n;
    char astr[10][100];
    char filename[200];
	char buff[300];
    strcpy(filename,"/tmp/look_ps.tmp");
    sprintf(buff,"ps aux > %s",filename);
    system(buff);
    for(i=0;i<100;i++) a_pid[mode][i]=0;
    if (mode==0) for(i=0;i<100;i++) strcpy(a_buff[i],"");
    if ((fp=fopen(filename,"r+"))==NULL) return;
    i=0;
    while (i<100&&fgets(buff,300,fp)!=NULL) {
        sscanf(buff,"%s%s%s%s",astr[0],astr[1],astr[2],astr[3]);
//        printf("%s %d %d",buff,atoi(astr[2]),atoi(astr[1]));getchar();
        if (atoi(astr[2])>per&&atoi(astr[1])>0) {
            if (mode==0) strcpy(a_buff[i],buff);
            a_pid[mode][i++]=atoi(astr[1]);
        }
	}
	fclose(fp);
}

/*
[root@www /bbs]# ps axu
USER       PID %CPU %MEM  SIZE   RSS TTY STAT START   TIME COMMAND
bbs        893  0.0  0.1  1156   660  pf S   22:49   0:00 /bin/login -h sysv -p
bbs        902  0.0  0.2  1468   800  pf S   22:49   0:00 -main
bbs       1398  0.0  0.1  1184   532  pf S   22:50   0:00 bash xutil/ppp
bbs       1399  0.0  0.1  1212   692  pf S   22:50   0:03 slirp ppp -b 56000
bbs       1764  0.0  0.1  1156   660  v3 S   22:51   0:00 /bin/login -h sysv -p
bbs       1775  0.0  0.2  1468   800  v3 D   22:51   0:00 -main
bbs       1857  0.0  0.1  1184   532  v3 S   22:52   0:00 bash xutil/ppp
bbs       1858  0.0  0.2  1380   856  v3 S   22:52   0:00 slirp ppp -b 56000
*/
