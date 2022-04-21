//PS 정보에서 사용되지 않는 US를 KILL 함
#include "bbs.h"
#include "chat.h"
#define PS_TEMP  "/tmp/killps.tmp"

int a_pid[1000];
int a_nid[1000];
char a_tty[1000][20];

main(argc, argv)
int argc;
char *argv[];
{
	char buff[300],buff1[300],buff2[300],buff3[300],buff4[300];
	FILE *fp;
	char filename[100];
	int i,n=0,count,count_man,count_pid,echo=No;
	int found=No;
	int mode,pid;

	if (argc<4) {
		printf(" 비사용 US 정보 삭제 \n");
		printf(" 사용법:  killus <호스트위치> <검색문자열1> <검색문자열2>\n");
		printf(" 예)      killus /bbs /bbs/bin/main main\n");
		printf(" 예)      killus /bbs ... ...    (전체 PS 할경우)\n");
		printf(" 호스트위치: /bbs , /xhost 등 호스트의 설치위치\n");
		printf(" 검색문자열: ps 정보에서 추출될 문자열\n");
		printf(" ps ax | grep 검색문자열1 | grep 검색문자열2 방식으로 추출\n");
		return;
	}
	if (argc>4&&is_same(argv[4],"-p")) echo=Yes;

	for(i=0;i<1000;i++) {strcpy(a_tty[i],"");a_pid[i]=0;a_nid[i]=0;}

	chdir(argv[1]);
	if (!is_file("user/moni")) {
		chdir("/bbs");
		if (!is_file("user/moni")) chdir("/xhost");
	}
	read_cfg();

	if (is_file(PS_TEMP)) {sleep(20);unlink(PS_TEMP);}
	sprintf(filename,"user/moni/moni%d.dat",cfg.hostno);
	if (echo) printf("FILENAME : %s\n",filename);
	if (strcmp(argv[2],"...")==0)
		sprintf(buff,"ps ax > %s",PS_TEMP);
	else
//        sprintf(buff,"ps ax | grep '%s' | grep '%s' > %s",argv[2],argv[3],PS_TEMP);
        sprintf(buff,"ps ax > %s",PS_TEMP);
    system(buff);

	if ((fp=fopen(PS_TEMP,"r+"))==NULL) return;
	count=0;
	while (count<1000&&fgets(buff,200,fp)!=NULL) {
        sscanf(buff,"%s %s %s %s\n",buff1,buff2,buff3,buff4);
        if (strlen(buff4)<3) continue;
        if (stristr(buff2,"pts/")&&stristr(buff,"xmain")) {
            if (is_oldtty(buff2,5*60*60)&&atoi(buff1)>100) {
                a_pid[count]=atoi(buff1);
                strcpy(a_tty[count],buff2);
                if (echo) printf("%d -> %d %s %s _%s_ (KILL TIME OUT)\n",count,a_pid[count],a_tty[count],buff3,buff4);
                count++;
                kill(atoi(buff1),SIGKILL);
            }
		}
        else if(!strcmp(buff2,"?") && stristr(buff,"xmain")) {
		    /* jplee가 첨가한 부분입니다. 
		     * ps로 xmain이 실행되어 있는 상태를 화면에 출력했을 때
		     * tty부분에 '?'로 나타나는 프로세스를 kill 시킵니다.
		     */
		    int p_id;
		    
		    p_id = atoi(buff1);
            if(p_id > 100) {
                a_pid[count]=atoi(buff1);
                strcpy(a_tty[count],buff2);
                if (echo) printf("%d -> %d %s %s _%s_ (KILL ? ZOMBI)\n",count,a_pid[count],a_tty[count],buff3,buff4);
                count++;
                kill(atoi(buff1),SIGKILL);
            }
		}
        else if (strlen(buff2)>1&&strlen(buff2)<20) {
			a_pid[count]=atoi(buff1);
			strcpy(a_tty[count],buff2);
            if (echo) printf("%d -> %d %s %s _%s_ (CHECK TIME)\n",count,a_pid[count],a_tty[count],buff3,buff4);
			count++;
		}
	}
	fclose(fp);
	unlink(PS_TEMP);

	if ((fp=fopen(filename, "r+"))==NULL) return;
	n=0;
	count_pid=0;
	while (1) {
		fseek(fp,n*sizeof_moni,SEEK_SET);
		if (!fread(&moni,sizeof_moni,1,fp)) break;
        if (moni.mode>0&&strlen(moni.tty)>1&&moni.hostno==cfg.hostno) {
			found=No;
			sprintf(buff,"/dev/%s",moni.tty);
			for (i=0;i<count;i++) {
				if (strcasecmp(a_tty[i],buff)==0) {found=Yes;break;}
			}
			if (!found&&moni.pid>100) {
				for (i=0;i<count;i++) {
					if (a_pid[i]==moni.pid) {
						a_pid[i]=0;
						found=Yes;
						break;
					}
				}
			}
            if (!found&&is_oldtty(moni.tty,60*60)) {
				if (echo) printf("REMOVE MONI : %d %s %s\n",moni.pid,moni.id,moni.tty);
				if (moni.pid>100) {
					a_nid[count_pid++]=moni.pid;
					if (echo) printf("    -> KILL PID : %d\n",moni.pid);
				}
				moni.mode=0;moni.hostno=0;moni.login=0;
				strcpy(moni.tty,"");strcpy(moni.id,"");
				fseek(fp,n*sizeof_moni,SEEK_SET);
				fwrite(&moni,sizeof_moni,1,fp);
			}
		}
		n++;
	}
	fclose(fp);
	sleep(2);
	for (i=0;i<count_pid;i++) {
		if (a_nid[i]>0) {
			kill(a_nid[i],SIGHUP);
		}
	}

	sleep(5);
	if ((fp=fopen(filename, "r"))==NULL) return;
	count_man=0;
	while (fread(&moni,sizeof_moni,1,fp)) {
		if (moni.mode>0&&strlen(moni.tty)>1)
			count_man++;
	}
	fclose(fp);
	if (count_man<1) {
		if (echo) printf("    -> %s 삭제\n",filename);
		unlink(filename);
	}
}
