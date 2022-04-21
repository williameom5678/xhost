////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 내용 : 시삽메뉴 관련 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
int n,nn,n2;

job_sysops()
{
	while (1) {
		cls();
		xy(2,1);
		print_(3,"%s  <쉘전용 시삽메뉴>",cfg.hostname);
		printf("  1. 시삽 / 운영진 설정\n");
		printf("  2. 회원 정보 전체검색\n");
		printf("  3. 회원 정보 빠른수정\n");
		printf("  4. 포트 입력 문자기록\n");
		printf("  5. 가입코드 관련 설정\n");
		printf("  7. 분산처리/LOCK 관련 설정    :%s\n",rbit(cfg.mprocmode,0,"분산않함","분산처리"));
		printf("  8. 인터넷   관련 설정\n");
		printf("  9. 접속기록 / 과금관련 설정  (.log/log)\n");
		printf(" 10. 회원번호 시작 지정\n");
		printf(" 11. UPS모드  사용 지정\n");
		printf(" 12. 로칼메뉴 설정\n");
		printf(" 21. 접속시 (1~99회) 접속횟수별 가산점수 (system/_first.add)\n");
		printf(" 22. BBS 실행권한/그룹 지정              (system/bbsuser.txt)\n");
		printf(" 23. 주민등록번호 통과문자열 지정        (system/freeidno.txt)\n");
		printf(" 24. 운영진 전용명령 명령어 편집         (system/syscmd.dat)\n");
		printf(" 25. ID 파일 관리 (추출/삭제) \n");
		n=numselect();printf("\n");
		if (n<= 0) break;
		if (n== 1) root_sysid();
		if (n== 2) sys_tem("bin/pfinfo root");
		if (n== 3) sys_tem("bin/pf root");
		if (n== 4) root_monichar();
		if (n== 5) root_free();
		if (n== 7) root_mproc();
		if (n== 8) root_internet();
		if (n== 9) root_mrec();
		if (n==10) edit_idnum();
		if (n==11) root_ups();
		if (n==12) job_local();
		if (n==21) root_first();
		if (n==22) xedit("system/bbsuser.txt");
		if (n==23) xedit("system/freeidno.id");
		if (n==24) edit_syscmd();
        if (n==25) root_idfile();
	}
}

root_free()
{
	FILE *fp;
	FCFG fcfg;
	int i,n,num,n2;
	char buff[100];
	char filename[200];
	strcpy(filename,"freecode/freecode.cfg");
	if (is_file(filename)) {
		fp=fopen(filename,"r+");
		if (fp!=NULL) fread(&fcfg,sizeof_fcfg,1,fp);
		if (fp!=NULL) fclose(fp);
	}

	while (1) {
		cls();
		printf("  ###  가입코드 관련 작업 ###\n");
		printf("  1. 손님 -> guest2 사용가능  (0:가능 1:불가)     :%d\n",bit(fcfg.mode,0));
		printf("  2. freeid 사용가능          (0:가능 1:불가)     :%d\n",bit(fcfg.mode,1));
		printf("  3.                                              :%d\n",bit(fcfg.mode,2));
		printf("  4.                                              :%d\n",bit(fcfg.mode,3));
		printf("  5. 가입코드 강제 등급  지정         (0:무시)    :%d\n",fcfg.fr_type);
		printf("  6. 가입코드 강제 LEVEL 지정         (0:무시)    :%d\n",fcfg.fr_level);
		printf("  7. 가입코드 강제 CASH  지정         (0:무시)    :%d\n",fcfg.fr_cash);
		printf("  8. 가입코드 강제 DOWN  지정  (K)    (0:무시)    :%d\n",fcfg.fr_down);
		printf("  9. 가입코드 강제 시간잔량지정(분)   (0:무시)    :%d\n",fcfg.fr_time);
		printfL2();
		n=numselect();
		xy(50,n+1);
		if (n<=0)  break;
		if (n==1)  fcfg.mode	  =gtt_bit(fcfg.mode	  , 0);
		if (n==2)  fcfg.mode	  =gtt_bit(fcfg.mode	  , 1);
		if (n==3)  fcfg.mode	  =gtt_bit(fcfg.mode	  , 2);
		if (n==4)  fcfg.mode	  =gtt_bit(fcfg.mode	  , 3);
		if (n==5)  fcfg.fr_type   =get_num(fcfg.fr_type   , 2);
		if (n==6)  fcfg.fr_level  =get_num(fcfg.fr_level  , 6);
		if (n==7)  fcfg.fr_cash   =get_num(fcfg.fr_cash   , 6);
		if (n==8)  fcfg.fr_down   =get_num(fcfg.fr_down   , 6);
		if (n==9)  fcfg.fr_time   =get_num(fcfg.fr_time   , 6);
	}
	fp=fopen(filename,"w+");
	fwrite(&fcfg,sizeof_fcfg,1,fp);
	if (fp!=NULL) fclose(fp);
}


root_ups()
{
	FILE *fp;
	FCFG fcfg;
	int i,n,num,n2;
	char buff[100];
	char filename[200];
	while (1) {
		cls();
		printf("  ###  UPS MODE 관련 작업 ###\n");
		printf("  1. UPS 모드 등급 18      사용가능               :%s\n",rbit(cfg.downmode,17,"동작","무시"));
		printf("  2. UPS 모드 등급 19      사용가능               :%s\n",rbit(cfg.downmode,18,"동작","무시"));
		printf("  3. UPS 파일 찾기 디렉토리 순서                  (system/upsdir.txt)\n");
		printfL2();
		n=numselect();
		xy(50,n+1);
		if (n<=0)  break;
		if (n==1)  cfg.downmode =gtt_bit(cfg.downmode	,17);
		if (n==2)  cfg.downmode =gtt_bit(cfg.downmode	,18);
		if (n==3)  xedit("system/upsdir.txt");
	}
	fp=fopen(filename,"w+");
	fwrite(&fcfg,sizeof_fcfg,1,fp);
	if (fp!=NULL) fclose(fp);
}


//분산처리모드
root_mproc()
{
	char buff[500];
	int i;
	file_line(buff,"hostno.txt",1);
	while (1) {
		cls();
		print_(0,"분산처리");
		printf("  1. 분산처리 동작         :%s\n",rbit(cfg.mprocmode,0,"않함","함"));
		printf("  2. 호스트번호     (0~19) :%d\n",cfg.hostno);
		printf("  3. LOCK 동작여부         :%s\n",rbit(cfg.mprocmode,1,"동작","무시"));
		printf("  4. FILE_LOCK 동작여부    :%s\n",rbit(cfg.mprocmode,3,"FILE_LOCK","XLOCKD데몬"));
		printf("  5. 기본호스트번호        :%s\n",cfg.sip);
		printf("  6. 분산서버 수           :%d\n",cfg.count_server);
		printf("  7. 분산 LOCK 파일 서버IP :%s\n",cfg.lock_ip_addr);
		printf("  8. 연결호스트번호 (0~19) :%d %d %d %d %d\n",cfg.nip[0],cfg.nip[1],cfg.nip[2],cfg.nip[3],cfg.nip[4],cfg.nip[5]);
		n=numselect();
		xy(50,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.mprocmode  =gtt_bit(cfg.mprocmode,0);
		if (n== 2) cfg.hostno	  =get_num(cfg.hostno,3);
		if (n== 3) cfg.mprocmode  =gtt_bit(cfg.mprocmode,1);
		if (n== 4) cfg.mprocmode  =gtt_bit(cfg.mprocmode,3);
		if (n== 5) get_str(cfg.sip,14);
		if (n== 6) cfg.count_server=get_num(cfg.count_server,2);
		if (n== 7) get_str(cfg.lock_ip_addr,19);
		if (n== 8) {
			printf("\n");
			for (i=0;i<19;i++) {
				printf("%3d: %s.%u ",i,cfg.sip,cfg.nip[i]);
				if (i%3==2) printf("\n");
			}
			printf("\n");
			for (i=0;i<19;i++) {
				printf("%3d : %15s.%-3u   >>",i,cfg.sip,cfg.nip[i]);
				sprintf(buff,"%u",cfg.nip[i]);
				get_str(buff,5);printf("\n");
				if (is_quit(buff)||is_same(buff,"w")) break;
				if (atoi(buff)>=0&&atoi(buff)<255) cfg.nip[i]=atoi(buff);
			}
		}
	}
	cfg.hostno%=100;
}


root_internet()
{
	FILE *fp;
	FCFG fcfg;
	int i,n,num,n2;
	char buff[100];
	char filename[200];
	while (1) {
		cls();
		printf("  ###  인터넷/네트워크 관련 작업 ###\n");
		printf("  1. Email 용 주소지정 (호스트명)              (system/hostaddr.txt)\n");
		printf("  2. 인터넷 전용 노드 설정 (네트워크설정)      (system/node_net.txt)\n");
		printf("  3. UTMP파일 (US시 IP 확인) :%s\n",cfg.utmpfile);
		printf("  4. 모든 접속 IP 기본 기록  :%s\n",rbit(cfg.hackmode,1,"기록","무시"));
		printf("  5. tty 포트     기록 파일  :%s\n",rbit(cfg.hackmode,2,"기록","무시"));
		printf("  6. 모든 접속 IP 기록 파일  :%s\n",rbit(cfg.hackmode,3,"통합","개별"));
		n=numselect();
		if (n<= 0)	break;
		if (n== 1)	xedit("system/hostaddr.txt");
		if (n== 2)	xedit("system/node_net.txt");
		if (n== 3)	{
			printf("\n US시 IP 접속정보를 확인할 UTMP파일을 지정하세요.\n");
			printf("(예)  /var/run/utmp ,/var/log/wtmp \n  >>");
			get_str(cfg.utmpfile,30);
		}
		if (n== 4)	cfg.hackmode=gtt_bit(cfg.hackmode,1);
		if (n== 5)	cfg.hackmode=gtt_bit(cfg.hackmode,2);
		if (n== 6)	cfg.hackmode=gtt_bit(cfg.hackmode,3);
	}
    if (strlen(cfg.utmpfile)<1) strcpy(cfg.utmpfile,"/var/run/utmp");
}


root_mrec()
{
	while (1) {
		cls();
		print_(0,"접속기록/과금관련설정");
		printf("  1. 등급 19     기록 여부                        :%s\n",rbit(cfg.mrecmode,1,"기록","무시"));
		printf("  2. 등급 16이상 기록 여부                        :%s\n",rbit(cfg.mrecmode,2,"기록","무시"));
		printf("  3. 등급 0(손님)기록 여부                        :%s\n",rbit(cfg.mrecmode,3,"기록","무시"));
		printf("  4. 일반 기록 여부          (전체상황기록)       :%s\n",rbit(cfg.mrecmode,4,"기록","무시"));
		printf("  5. 게시판   진입 기록                           :%s\n",rbit(cfg.mrecmode,5,"기록","무시"));
		printf("  6. 게시판   종결 기록                           :%s\n",rbit(cfg.mrecmode,6,"기록","무시"));
		printf("  7. 외부명령 진입 기록                           :%s\n",rbit(cfg.mrecmode,7,"기록","무시"));
		printf("  8. 외부명령 종결 기록                           :%s\n",rbit(cfg.mrecmode,8,"기록","무시"));
		printf("  9. 진입부분 전체 기록   (user/rec/all_YYMM.rec) :%s\n",rbit(cfg.mrecmode,9,"기록","무시"));
		printf(" 10. 포트별 사용 시간 기록                        :%s\n",rbit(cfg.mrecmode,10,"무시","기록"));
		printf(" 11. 사용자 입력 명령 기록 (user/rec/all_cmd.rec) :%s\n",rbit(cfg.mrecmode,11,"무시","기록"));
		printf(" 21. 과금기록상호코드                             :%s\n",cfg.comp);
		printf(" 22. 과금기록 금액 (분당)                         :%d\n",cfg.price1);
		printf(" 23. 과금기록 금액 ( K당)                         :%d\n",cfg.price2);
		printf(" 24. 기록파일 저장위치 (기본:user/rec)            :(recdir.txt)\n");
		n=numselect();
		xy(50,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.mrecmode =gtt_bit(cfg.mrecmode,1);
		if (n== 2)	cfg.mrecmode =gtt_bit(cfg.mrecmode,2);
		if (n== 3)	cfg.mrecmode =gtt_bit(cfg.mrecmode,3);
		if (n== 4)	cfg.mrecmode =gtt_bit(cfg.mrecmode,4);
		if (n== 5)	cfg.mrecmode =gtt_bit(cfg.mrecmode,5);
		if (n== 6)	cfg.mrecmode =gtt_bit(cfg.mrecmode,6);
		if (n== 7)	cfg.mrecmode =gtt_bit(cfg.mrecmode,7);
		if (n== 8)	cfg.mrecmode =gtt_bit(cfg.mrecmode,8);
		if (n== 9)	cfg.mrecmode =gtt_bit(cfg.mrecmode,9);
		if (n==10)	cfg.mrecmode =gtt_bit(cfg.mrecmode,10);
		if (n==11)	cfg.mrecmode =gtt_bit(cfg.mrecmode,11);
		if (n==21)	get_str(cfg.comp,4);
		if (n==22)	cfg.price1	 =get_num(cfg.price1  , 5);
		if (n==23)	cfg.price2	 =get_num(cfg.price2  , 5);
		if (n==24)	xedit2("recdir.txt");
	}
}


root_sysid()
{
	FILE *fp;
	while (1) {
		cls();
		print_(0,"운영진 관련 회원 지정");
		printf("  1. 시삽(root)의 아이디                 :%s\n",cfg.sysop      );
		printf("  2. 쉘 사용 가능 회원 지정              (system/sysshell.id)\n");
		printf("  3. 시삽메뉴 사용 가능 회원 리스트      (system/sysopid.txt)\n");
		printf("  4. 등급18이상 가능 회원                (system/systype.id)\n");
		printf("  5. .dark명령 사용 가능 회원 리스트     (system/darkman.id )\n");
		printf("  6. .warn기능사용 가능 회원 리스트      (system/syswarn.id)\n");
		printf("  7. .kill기능사용 가능 회원 리스트      (system/syskill.id)\n");
		printf("  8. .menu기능사용 가능 회원 리스트      (system/sysmenu.id)\n");
		printf("  9. 등급14이상 보조암호 지정 회원 PASS2 (system/syspass.id)\n");
		n=numselect();
		xy(50,n+1);
		if (n<= 0) break;
		if (n== 1) get_str(cfg.sysop	 ,8 );
		if (n== 2) xedit("system/sysshell.id");
		if (n== 3) xedit("system/sysopid.txt");
		if (n== 4) xedit("system/systype.id");
		if (n== 5) xedit("system/darkman.id");
		if (n== 6) xedit("system/syswarn.id");
		if (n== 7) xedit("system/syskill.id");
		if (n== 8) xedit("system/sysmenu.id");
		if (n== 9) xedit("system/syspass.id");
	}
}


root_monichar()
{
	while (1) {
		cls();
		print_(0,"전체 문자 기록");
		printf("  1. 일반회원 문자 기록      %s\n",rbit(cfg.monichar,1,"기록","무시"));
		printf("  2. 시삽진   문자 기록      %s\n",rbit(cfg.monichar,2,"기록","무시"));
		printf("  3. 대화실   문자 기록      %s\n",rbit(cfg.monichar,3,"기록","무시"));
		printf("  4. 손님     문자 기록      %s\n",rbit(cfg.monichar,4,"기록","무시"));
		printf("  5. 가기코드 변경 기록      %s\n",rbit(cfg.monichar,5,"기록","무시"));
        printf("  6. 일반문자 기록 위치      %s\n",rbit(cfg.monichar,6,"/tmp/monich/","user/monich"));
		printf(" 10. 입력문자 감지대상 아이디 지정       (system/monich.id)\n");
		printf(" 11. 입력문자 감지대상 아이디 제외 지정  (system/monich2.id)\n");
		n=numselect();
		if (n<= 0) break;
		if (n== 1) cfg.monichar=gtt_bit(cfg.monichar,1);
		if (n== 2) cfg.monichar=gtt_bit(cfg.monichar,2);
		if (n== 3) cfg.monichar=gtt_bit(cfg.monichar,3);
		if (n== 4) cfg.monichar=gtt_bit(cfg.monichar,4);
		if (n== 5) cfg.monichar=gtt_bit(cfg.monichar,5);
        if (n== 6) cfg.monichar=gtt_bit(cfg.monichar,6);
		if (n==10) xedit("system/monich.id");
		if (n==11) xedit("system/monich2.id");
	}
}

root_first()
{
	cls();
	printfL2();
	printf(" 이 기능은 초기가입후 접속한뒤 각 접속횟수별로 점수/등급등을\n");
	printf(" 가산해주는 기능이므로 주의해야 합니다.\n");
	printf(" 각줄은 가입횟수별 줄수이며 레벨증가,다운량증가(KB),사이버머니증가,\n");
	printf(" 등급증가,시간잔량증가(분)을 한칸씩 뛰어서 수치만 입력하면 됩니다.\n");
	printf(" 예를 들어 첫줄에 10 1000 0 0 0 로 지정하면 첫접속때 레벨10,다운량1000K\n");
	printf(" 증가됩니다. 둘째줄은 두번째 접속,세째줄은 3번째.. 99번접속까지 가능\n");
	printfL2();
	printf(" 이 기능을 편집하길 원하십니까? (y/N)>>");
	if (yesno("",No)) xedit("system/_first.add");
}


edit_syscmd()
{
	SYSCMD syscmd[30];		  //특별 환경화일
	FILE *fp;
	char buff[300];
	char buff1[200];
	char buff2[200];
	char buff3[200];
	char buff4[200];
	char filename[300];
	int i,n;
	static char *adef[] = {".sys  ",".csys ",".bsys ",".log  ",".menu ",
						   ".scr  ",".mlogo",".lev  ","      ",".dark ",
						   ".dark2",".kill ",".warn ",".w    ","      ",
						   ".scr  ",".ad   ",".lbs  ",".repf ",".ed   ",
						   ".ed2  ",".shell",".sss  ",".bcp  ",".pfs  ",
						   "      ","      ","      ","      ","      ",NULL};
	strcpy(filename,"system/syscmd.dat");
	if (is_file(filename)&&file_size(filename)!=840) {
		mess("%s 의 버젼이 맞지않습니다. 새로 지정하세요..");
		unlink(filename);
	}
	for (i=0;i<30;i++) {
		strcpy(syscmd[i].cmd,"");strcpy(syscmd[i].cm2,"");strcpy(syscmd[i].pass,"");syscmd[i].type=15;
	}
	if ((fp=fopen(filename,"r"))!=NULL) {
		for (i=0;i<30;i++) {
			fread(&syscmd[i],sizeof(syscmd[0]),1,fp);
		}
		fclose(fp);
	}
	for (i=0;i<30;i++) {
		del_esc(syscmd[i].cmd);del_space(syscmd[i].cmd);del_enter(syscmd[i].cmd);
		del_esc(syscmd[i].cm2);del_space(syscmd[i].cm2);del_enter(syscmd[i].cm2);
		del_esc(syscmd[i].pass);del_space(syscmd[i].pass);del_enter(syscmd[i].pass);
		if (strlen(syscmd[i].cmd)<1) xstrncpy(syscmd[i].cmd,adef[i],8);
		if (strlen(syscmd[i].cm2)<1) xstrncpy(syscmd[i].cm2,adef[i],8);
	}
	while (1) {
		cls();
		printf("  1. 시삽명령            (.sys)    : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 0].cmd,syscmd[ 0].cm2,syscmd[ 0].pass,syscmd[ 0].type );
		printf("  2. 동호회시삽명령      (.csys)   : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 1].cmd,syscmd[ 1].cm2,syscmd[ 1].pass,syscmd[ 1].type );
		printf("  3. 게시판담당명령      (.bsys)   : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 2].cmd,syscmd[ 2].cm2,syscmd[ 2].pass,syscmd[ 2].type );
		printf("  4. 접속기록확인        (.log )   : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 3].cmd,syscmd[ 3].cm2,syscmd[ 3].pass,syscmd[ 3].type );
		printf("  5. 메뉴편집명령        (.menu)   : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 4].cmd,syscmd[ 4].cm2,syscmd[ 4].pass,syscmd[ 4].type );
		printf("  6. 화면편집명령        (.scr )   : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 5].cmd,syscmd[ 5].cm2,syscmd[ 5].pass,syscmd[ 5].type );
		printf("  7. 현재화면편집명령    (.mlogo)  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 6].cmd,syscmd[ 6].cm2,syscmd[ 6].pass,syscmd[ 6].type );
		printf("  8. 레벨편집명령        (.lev)    : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 7].cmd,syscmd[ 7].cm2,syscmd[ 7].pass,syscmd[ 7].type );
		printf("  9.                     (      )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 8].cmd,syscmd[ 8].cm2,syscmd[ 8].pass,syscmd[ 8].type );
		printf(" 10. 감춤명령1           (.dark)   : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[ 9].cmd,syscmd[ 9].cm2,syscmd[ 9].pass,syscmd[ 9].type );
		printf(" 11. 감춤명령2           (.dark2)  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[10].cmd,syscmd[10].cm2,syscmd[10].pass,syscmd[10].type );
		printf(" 12. 강제종료명령        (.kill )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[11].cmd,syscmd[11].cm2,syscmd[11].pass,syscmd[11].type );
		printf(" 13. 경고명령            (.warn )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[12].cmd,syscmd[12].cm2,syscmd[12].pass,syscmd[12].type );
		printf(" 14. 전체 메세지 전송    (.w    )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[13].cmd,syscmd[13].cm2,syscmd[13].pass,syscmd[13].type );
		printf(" 15.                     (      )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[14].cmd,syscmd[14].cm2,syscmd[14].pass,syscmd[14].type );
		printf(" 16. 광고관련 화면편집   (.ad   )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[15].cmd,syscmd[15].cm2,syscmd[15].pass,syscmd[15].type );
		printf(" 17. 다른ID 접속기록     (.vlog )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[16].cmd,syscmd[16].cm2,syscmd[16].pass,syscmd[16].type );
		printf(" 18. BBS별 IP 기록확인   (.lbs  )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[17].cmd,syscmd[17].cm2,syscmd[17].pass,syscmd[17].type );
		printf(" 19. 다른ID로 치환하기   (.repf )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[18].cmd,syscmd[18].cm2,syscmd[18].pass,syscmd[18].type );
		printf(" 20. logo  /TEXT편집     (.ed   )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[19].cmd,syscmd[19].cm2,syscmd[19].pass,syscmd[19].type );
		printf(" 21. system/TEXT편집     (.ed2  )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[20].cmd,syscmd[20].cm2,syscmd[20].pass,syscmd[20].type );
		printf(" 22. 쉘사용명령          (.shell)  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[21].cmd,syscmd[21].cm2,syscmd[21].pass,syscmd[21].type );
		printf(" 23. 쉘전용시삽명령      (.sss  )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[22].cmd,syscmd[22].cm2,syscmd[22].pass,syscmd[22].type );
		printf(" 24. BBSCOPY 명령        (.bcp  )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[23].cmd,syscmd[23].cm2,syscmd[23].pass,syscmd[23].type );
		printf(" 25. 회원관리전용명령    (.pfs  )  : %-8s/ %-8s/ %-8s/ %2d\n",syscmd[24].cmd,syscmd[24].cm2,syscmd[24].pass,syscmd[24].type );
		printf(" 편집할 명령번호 (종료:Q)>>");
		lineinput(buff,3);n=atoi(buff);printf("\n");
		if (n<= 0)	break;
		if (n>0&&n<=24)  {
			printf("\n 기본 선택 명령1 (%s) >>",syscmd[n-1].cmd);
			get_str(syscmd[n-1].cmd,8 );
			printf("\n 보조 선택 명령2 (%s) >>",syscmd[n-1].cm2);
			get_str(syscmd[n-1].cm2,8 );
			if (strlen(syscmd[n-1].cm2)<1) strcpy(syscmd[n-1].cm2,syscmd[n-1].cmd);
			xy(58,n);
			printf("\n 선택 명령 진입암호 (%s) >>",syscmd[n-1].pass);
			get_str(syscmd[n-1].pass,8 );
			printf("\n 선택 명령 진입등급 (%d) >>",syscmd[n-1].type);
			syscmd[n-1].type=(byte)get_num(syscmd[n-1].type,2);
		}
	}
	if ((fp=fopen(filename,"w"))!=NULL) {
		i=0;
		for (i=0;i<30;i++) {
			del_esc(syscmd[i].cmd);del_space(syscmd[i].cmd);del_enter(syscmd[i].cmd);
			del_esc(syscmd[i].cm2);del_space(syscmd[i].cm2);del_enter(syscmd[i].cm2);
			del_esc(syscmd[i].pass);del_space(syscmd[i].pass);del_enter(syscmd[i].pass);
			fwrite(&syscmd[i],sizeof(syscmd[0]),1,fp);
		}
		fclose(fp);
	}
}


edit_idnum()
{
	FILE *fp;
	char buff[100];
	char filename[200];
	int n=0,n2;
	strcpy(filename,"system/idnum.dat");
	n=inc_numfile(0,filename);
	if (n<1) n=1;
	printf("\n 회원의 가입시 고유번호의 시작위치를 지정합니다.\n");
	printf("\n 현재의 최종 번호는 %d 번입니다.\n",n);
	printf("\n 중복되지 않게 유의 하십시요 >>");
	n=numinput();
	if (n<1) return;
	c_lock(1,filename);
	if ((fp=fopen(filename,"w+"))!=NULL) {
		fwrite(&n,sizeof(n),1,fp);
		fclose(fp);
	}
	c_lock(0,filename);
}


reset_pf() {
	//가입신청신상의 초기화
	strcpy(pf.id   ,"");
	strcpy(pf.name ,"");
	strcpy(pf.nick ,"");
	strcpy(pf.passwd,"");
	strcpy(pf.pos1 ,"");
	strcpy(pf.pos2 ,"");
	strcpy(pf.addr ,"");
	strcpy(pf.tele ,"");
	strcpy(pf.offi ,"");
	strcpy(pf.gofirst,"");
	strcpy(pf.id_no,"");
	strcpy(pf.mailid,"");
	strcpy(pf.memo1,"");
	strcpy(pf.memo2,"");
	strcpy(pf.user1,"");
	strcpy(pf.user2,"");
	strcpy(pf.user3,"");
	strcpy(pf.user4,"");
	strcpy(pf.email,"");
	pf.num=0;
	pf.itype=0;
	pf.myflag=0;
	pf.sex='m';
	pf.job= 0;
	time(&pf.inday);
	time(&pf.login);
	time(&pf.logout);
	pf.birthday=0;
	pf.countw=0;
	pf.countd=0;
	pf.countu=0;
	pf.level=cfg.guestlevel;
	pf.type =cfg.guesttype ;
	pf.cash=cfg.guestcash;
	pf.left_time=cfg.guesttime;
	pf.down=cfg.guestdown*1000;
	pf.all_log=0;			//총접속횟수
	pf.month_log=0; 		//이번달총접속횟수
	pf.all_time=0;			//총이용시간기록  (초)
	pf.month_time=0;		//이번달총이용시간(초)
	pf.p_ph=0;				//체력
	pf.p_it=0;				//지력
	pf.cash=0;				//사이버머니
	pf.p_at=0;				//매력
	pf.p_pr=0;				//방어력
	pf.p_mo=0;				//모드
}

job_local()
{
	int num;
	read_cfg();
	while (1) {
		cls();
		print_(0,"로칼메뉴설정");
		printf("   1. 로칼 영역 할당 파일편집\n");
		printf("   2. 전체 회원 로칼영역 갱신\n");
		printf("   3. 로칼메뉴 기능 사용여부 :%s\n",rbit(cfg.gomode,11,"무시","사용함"));
		num=numselect();printf("\n");
		if (num<= 0) break;
		else if (num==1) xedit2("system/local.txt");
		else if (num==2) reset_pf_local();
		else if (num==3) cfg.gomode=gtt_bit(cfg.gomode,11);
	}
	save_cfg();
}

// 회원인덱스 정보이다.
// 0	1	  2    3		4
//번호 지역1 지역2 나이시작  나이종결
typedef struct {
	char addr1[21];
	char addr2[21];
	int  age1;
	int  age2;
	int  num;
} LOCAL;


reset_pf_local()
{
	FILE *fp;
	LOCAL alocal[100];
	int i,n,nn,i2,n2,rec;
	char buff[300],buff1[300];
	char mms[10][300];

	for (i=0;i<100;i++) {
		strcpy(alocal[i].addr1,"");strcpy(alocal[i].addr2,"");
		alocal[i].age1=alocal[i].age2=alocal[i].num=0;
	}
	if ((fp=fopen("system/local.txt","r"))==NULL) return;
	nn=0;
	while (nn<100&&fgets(buff1,200,fp)!=NULL) {
		if (strlen(buff1)<10||buff1[0]=='#'||buff1[1]==';') continue;
		for (i=0;i<6;i++) bzero(mms[i],200);
		sscanf(buff1,"%s%s%s%s%s%s",mms[0],mms[1],mms[2],mms[3],mms[4],mms[5]);
		if (strlen(mms[1])<=1&&strlen(mms[2])<=1&&atoi(mms[3])<1&&atoi(mms[4])<1) continue;
		if (strlen(mms[1])>1) sprintf(alocal[nn].addr1,"**%-15.15s",mms[1]);
		if (strlen(mms[2])>1) sprintf(alocal[nn].addr2,"%-20.20s",mms[2]);
		del_space(alocal[nn].addr1);del_space(alocal[nn].addr2);
		alocal[nn].age1=atoi(mms[3]);alocal[nn].age2=atoi(mms[4]);
		alocal[nn].num=atoi(mms[0]);
		nn++;
	}
	fclose(fp);
	if (nn<=0) {
		mess("system/local.txt 에 로칼분류정보가 없습니다.");
		return;
	}

	if ((fp=fopen(ID_FILE,"r+"))==NULL) return;
	rec=0;
	while((fread(&pf,sizeof_pf,1,fp))) {
		if (pf.type<1||strlen(pf.id)<3) goto _cont3;
		sprintf(buff1,"**%s",pf.addr);del_space(buff1);
		i2=get_age(pf);
		for (i=0;i<nn;i++) {
			if (strlen(alocal[i].addr1)>2&&!stristr(buff1,alocal[i].addr1)) continue;
			if (strlen(alocal[i].addr2)>0&&!stristr(pf.addr,alocal[i].addr2)) continue;
			if (alocal[i].age1>0&&i2<alocal[i].age1) continue;
			if (alocal[i].age2>0&&i2>alocal[i].age2) continue;
			pf.localmenu=alocal[i].num;
			goto _cont2;
		}
		pf.localmenu=0;
		_cont2:
		fseek(fp,rec*sizeof_pf,SEEK_SET);
//        fwrite(&pf,sizeof_pf,1,fp);
        gwrite(204,pf,fp);
		_cont3:
		rec++;
		printf("%d %-8s %-20.20s %s -> %d\n",rec,pf.id,pf.addr,datestr(0,pf.birthday),pf.localmenu);
	}
	fclose(fp);
}




root_idfile()
{
    FILE *fp1,*fp2;
    char buff[290];
    char buff1[290];
    char buff2[290];
    char date1[90];
    char date2[90];
    char filename1[290];
    char filename2[290];
    char filename3[290];
    int num,n,i,i1,i2,i3,n1,n2,n3,count;
    int type1,type2,rec1,rec2,listmode;
	who=0;

    strcpy(filename1,"id/id.dat");
    strcpy(filename2,"id/id.tmp");
    strcpy(filename3,"id/id.txt");
    listmode=1;
    rec1=rec2=0;
    type1=type2=0;
    strcpy(date1,"");
    strcpy(date2,"");
	while (1) {
        if (rec2==0) rec2=9999999;
        cls();
        print_(1," 회원 ID 파일 추출 관리 작업");
        printf("     ------------파일선택---------- \n");
        printf("  1. 작업 원본 파일                 : %s  ( 자료수:%8d )\n",filename1,count_rec(filename1,sizeof_pf));
        printf("  2. 작업 결과 파일 (ID구조)        : %s  ( 자료수:%8d )\n",filename2,count_rec(filename2,sizeof_pf));
        printf("  3. 작업 결과 파일 ( TEXT )        : %s  ( 파일크기:%8d )\n",filename3,file_size(filename3));
        printf("  4. TEXT 출력 모양 지정            : %d  \n",listmode);

        printf("\n     ------------범위선택---------- \n");
        printf(" 11. 추출 레코드 번호               : %d - %d\n",rec1,rec2);
        printf(" 12. 추출 등급                      : %d - %d\n",type1,type2);
        printf(" 13. 추출 최종 접속 날짜 YYYY-MM-DD : %s - %s\n",date1,date2);

        printf("\n     ------------작업선택---------- \n");
        printf(" 91. 해당 범위 자료 추출  (원본의 해당범위  -> ID구조 결과파일저장)\n");
        printf(" 92. 해당 범위 TEXT 추출  (원본의 해당범위  -> TEXT로 결과파일저장)\n");
        printf(" 93. 해당 범위 자료 삭제  (원본 파일 데이타만 삭제)\n");
        printf("*94. 해당 범위 자료 삭제  (USER/ID 디렉토리까지 삭제)\n");
        printf("*95. 해당 범위 USER/ID 디렉토리 삭제\n");
        printf("*96. 원본 파일에 없는 회원 USER/ID 디렉토리 삭제\n");
        n=numselect();
        xy(1,21);printf("\n");
        if (n== 0) break;
        if (n== 1) {printf("파일명>>");get_str(filename1,80);}
        if (n== 2) {printf("파일명>>");get_str(filename2,80);}
        if (n== 3) {printf("파일명>>");get_str(filename3,80);}
        if (n== 4) {printf("출력모양 (%d)>>",listmode);listmode=get_num(listmode,2);}
        if (n==11) {
            printf("시작레코드 (%d)>>",rec1);rec1=get_num(rec1,8);
            printf("\n종결레코드 (%d)>>",rec2);rec2=get_num(rec2,8);
        }
        if (n==12) {
            printf("추출등급 (%d)>>",type1);type1=get_num(type1,8);
            printf("\n종결등급 (%d)>>",type2);type2=get_num(type2,8);
        }
        if (n==13) {
            printf("추출최종접속일자 시작 (%s)>>",date1);get_str(date1,10);
            printf("\n추출최종접속일자 끝   (%s)>>",date2);get_str(date2,10);
        }
        if (n>=91&&n<94) {
            if (n==91&&!yesno2("ID구조로 저장할까요?(y/N)>>",No)) continue;
            if (n==92&&!yesno2("TEXT로 저장할까요?(y/N)>>",No)) continue;
            if (n==93&&!yesno2("해당 범위를 지울까요?(y/N)>>",No)) continue;
            if (n==94&&!yesno2("해당 범위를 지울까요?(y/N)>>",No)) continue;
            if (n==95&&!yesno2("해당 범위USER/ID를 지울까요?(y/N)>>",No)) continue;
            if ((fp1=fopen(filename1,"r"))==NULL) return;
            strcpy(buff,filename2);
            if (n==92) strcpy(buff,filename3);
            if (n==93) strcpy(buff,TMP_FILE2);
            if ((fp2=fopen(buff,"w"))==NULL) return;
            if (rec1>0) fseek(fp1,rec1*sizeof_pf,SEEK_SET);
            n2=rec1;
            strcpy(buff,date1);strchng2(buff,"-","");i1=atoi(buff);
            strcpy(buff,date2);strchng2(buff,"-","");i2=atoi(buff);
            count=0;
            while(n2<=rec2&&fread(&pf,sizeof_pf,1,fp1)) {
                n2++;
                if (strlen(pf.id)<2) continue;
                if (pf.type<type1||(type2>0&&pf.type>type2)) continue;
                n3=time_num(pf.logout);
                if (i1>0&&n3<i1) continue;
                if (i2>0&&n3<i2) continue;
                count++;
                strcpy(buff1,datestr(0,pf.logout));
                strcpy(buff2,datestr(0,pf.inday));
                if (n==91||n==93) {
                    sprintf(buff,"%5d %-8s %-8s %-8s %2d %-8s %s %s",pf.num+1,pf.id,pf.name,pf.nick,pf.type,pf.user1,buff2,buff1);
                    printf("%4d %s\n",count,buff);
//                    fwrite(&pf,sizeof_pf,1,fp2);
                    gwrite(167,pf,fp2);
                }
                else if (n==92) {
                    pf.num=count-1;
                    if      (listmode==1) sprintf(buff,"%5d %-8s %-8s %-8s %2d %-8s %-8s %-8s %-8s",pf.num+1,pf.id,pf.name,pf.nick,pf.type,pf.user1,pf.email,pf.mailid,pf.gofirst);
                    else if (listmode==2) sprintf(buff,"%5d %-8s %-8s %5d %s %s%5d%8.8s%5d%4d%4d%4dM",pf.num+1,pf.id,pf.name,pf.level%100000,buff2,buff1,pf.all_log,datestr(26,pf.all_time),pf.countw,pf.countu,pf.countd,(pf.down/(1024*1024))%10000);
                    else if (listmode==3) sprintf(buff,"%5d %-8s %-8s %2d %5d %7d %6d%6d%6d%5d/%5d",pf.num+1,pf.id,pf.name,pf.type,pf.level%100000,pf.cash,pf.countw,pf.countu,pf.countd,pf.month_log,pf.all_log);
                    else if (listmode==4) sprintf(buff,"%5d %-8s %-8s %s %14s %-25.25s",pf.num+1,pf.id,pf.name,dateyang(3,pf.birthday,pf.moon),pf.id_no,pf.addr);
                    else if (listmode==5) sprintf(buff,"%5d %-8s %-8s %-8s %s %14s %s",pf.num+1,pf.id,pf.name,pf.nick,dateyang(3,pf.birthday,pf.moon),pf.id_no,ajob[pf.job]);
                    else if (listmode==6) sprintf(buff,"%5d %-8s %-8s %-8s %-20.20s %-20.20s",pf.num+1,pf.id,pf.name,pf.user1,pf.user2,pf.user3);
                    else if (listmode==7) sprintf(buff,"%5d %-8s %-8s %-30.30s %-20.20s",pf.num+1,pf.id,pf.name,pf.memo1,pf.memo2);
                    else if (listmode==8) sprintf(buff,"%5d %-8s %-8s %s %s %s",pf.num+1,pf.id,pf.name,pf.user1,pf.user2,pf.user3);
                    else                  sprintf(buff,"%5d %-8s %-8s %s %14s",pf.num+1,pf.id,pf.name,dateyang(3,pf.birthday,pf.moon),pf.id_no);
                    fprintf(fp2,"%s\n",buff);
                }
            }
            fclose(fp1);
            fclose(fp2);

            mess("총 %d 개의 자료가 추출되었습니다.",count);

            if (n==92) view_text(filename3);
            if (n==93) {
                if (count==0&&!yesno(" 현재 추출된 자료가 없으므로 모든자료가 삭제 됩니다. 삭제할까요?(y/N)>>",No)) continue;
                file_move(TMP_FILE2,filename1);
            }
        }
	}
}

