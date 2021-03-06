////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 내용 : 시삽메뉴 관련 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include "chat.h"

int n,nn,n2,num;
int top;
int total;
int last;
int key;
long lastnow=0; 	  //마지막 시간
long lastnow2=0;	   //마지막 시간
int ast[20];
int disp_mode;
int mrand;
char ss_1[50];
char ss_2[50];
char ss_3[50];

char menu_file[200];
char filename[200];
char now_code[200];
char now_dir[200];
char now_id[9];

#include "sysoput.c"     //보완변수

main(int argc, char **argv)
{
	char buff[90];
	char buff2[90];
	int key;
	int num,n,i;
	FILE *fp;
	struct termio term_old;
	who=0;
	c_proc=is_file("system/c_proc.tag");
	c_proc2=is_file("system/c_proc2.tag");
	c_proc3=is_file("system/c_proc3.tag");
	for(i=0;i<20;i++) ast[i]=0;

	printf("\n<%s>\n\n\n\n",VER1);
	fflush(NULL);

	if (!is_file(ID_FILE)) {   //root모드로 설정 그냥exec
		mess("%s 파일이 없습니다.",ID_FILE);
		if (yesno(" ID디렉토리와 빈ID파일을 만들까요?",Yes)) {
			check_dir("id");
			touch_file(ID_FILE);
			make_ind(0,"make_ind_all");
		}
		return;
	}


	check_init_dir();

	ioctl(0,TCGETA,&termold);	//원래 터미날 모드 복구
	srand(time(NULL));			//난수의 초기화
	mrand=rand()%10000000;
	club_mode=No;
	strcpy(club_go.dir,"./");
	strcpy(club_go.code,"main");
	setenv("TERM","vt100",1);
	setenv("LANG","ko",1);       //국가 지정
	disp_mode=0;
	findport(tty);
	read_cfg();
	if (strlen(cfg.sysop)<3) strcpy(cfg.sysop,"sysop");
	set_tmpfile();
	club_mode=No;
	read_pftype();	//회원의 등급,일일사용가능시간을 확인한다.
	read_atype();	//등급읽기
	read_ajob();	//직업읽기
	if (argc<2) {
		if (!check_pass("zz",8,"PASS >>")) return;
	}

	if (argc<2||(argc==2&&strlen(argv[1])<3)) {
		printf("Usage:bin/sysop ????\n");
		if (yesno("인덱스재정리를 할까요? (Y/n) >>",Yes)) {
			printf("\n 인덱스파일을 재정리합니다...\n");
			make_ind(0,"make_ind_all");
		}
		return;
	}
	else if (argc==2&&is_same(argv[1],"root")) {
		strcpy(now_id,cfg.sysop);
		printf("\n ### 현재 쉘전용 시삽모드 상태입니다. ###\n");
		if (!read_pf(cfg.sysop)) {
			printf(" sysop으로 지정된 '%s' ID를 찾을수 없습니다.\n",cfg.sysop);
			if (yesno("인덱스재정리를 할까요? (Y/n) >>",Yes)) {
				printf("\n 인덱스파일을 재정리합니다...\n");
				make_ind(0,"make_ind_all");
			}
		}
		strcpy(pf.id,"sysop");
		pf.type=19;
		who=5;
	}
	else {
		strcpy(now_id,argv[1]);
		if (!read_pf3(now_id)) {
			mess("%s ID를 찾을수 없습니다.",now_id);
			if (yesno("인덱스재정리를 할까요? (Y/n) >>",Yes)) {
				make_ind(0,"make_ind_all");
				printf("\n회원 아이디 인덱스 화일 재정리 완료\n");
			}
			goto _quit;
		}
		if(pf.type<16) {
			mess("%s 시삽메뉴 가능등급이 아닙니다.",now_id);
			goto _quit;
		}
		who=0;
			 if (is_same(cfg.sysop,now_id)) who=4;
		else if (pf.type==19)				who=4;	//root
		else if (pf.type==18)				who=3;	//부시삽
		else if (pf.type==17)				who=2;	//운영진
		if (who<2||who>5) {
			mess("%s님은 시삽메뉴를 사용할수 없는 등급입니다.",now_id);
			goto _quit;
		}
	}

	rawmode();	// 넌 캐노니칼 모드

	stat_pf();	//회원자료 확인
	while (1) {
		cls();
		xy(1,1);
		printf(" ┌───────────────────────┐\n");
		if (who==5) printf(" │[7m  %-22s## 쉘 전용 시삽 메뉴  [m│\n",cfg.hostname);
		else		printf(" │[7m  %-22s  %8s W:%d  #:%3d [m│\n",cfg.hostname,now_id,who,cfg.hostno);
		printf(" ├───────────┬───────────┤\n");
		printf(" │   1. 회원목록        │  21. 시스템환경설정  │\n");
		printf(" │   2. 회원관리/설정   │  22. 금지/제한/담당  │\n");
		printf(" │   3. 등급/레벨/직업  │  23. 포트/노드/분류  │\n");
		printf(" │                      │  24. 자료실 업/다운  │\n");
		printf(" │   5. 손님설정        │  25. 게시판,편집기   │\n");
		printf(" │   6. 가입설정        │  26. 화상게시판      │\n");
		printf(" │   7. 가입해지        │  27. 게시판 자동추천 │\n");
		printf(" │   8. 메뉴편집        │  28. 시스템 백업     │\n");
		printf(" │   9. 유틸리티        │  29. 공지사항설정    │\n");
		printf(" ├───────────┼───────────┤\n");
		printf(" │  11. 동호회          │  31. 업다운파라메타  │\n");
		printf(" │  12. 화면관리        │  32. 전체자료실설정  │\n");
		printf(" │  13. 메일 설정       │  33. 통계관련작업    │\n");
		printf(" │  14. 대화실설정      │  34. 인포샵관련설정  │\n");
		printf(" │  15. 메세지관련      │                      │\n");
		printf(" │  16. 진입/종료관련   │                      │\n");
		printf(" │  17. 명령/입력관련   │                      │\n");
		if (who==5)
		printf(" │[7m  98. 시삽명령어편집  [m│[7m  99. 쉘전용시삽설정  [m│\n");
		printf(" └───────────┴───────────┘\n");
		printf(" 선택번호? (종료:P 저장취소:N 상태:?) >>");
		lineinput(buff,10);check_han(buff);printf("\n");num=atoi(buff);
		if (is_same(buff,"n")) goto _quit;
		if (is_quit(buff)) break;
		if (num== 1) sys_tem("bin/pfinfo '%s'",who==5?"root":now_id);
		if (num== 2) job_pf();
		if (num== 3) job_type();
		if (num== 5) job_guest();
		if (num== 6) job_guestinput();
		if (num== 7) job_idout();
		if (num== 8) job_menu();
		if (num== 9) job_util();
		if (num==11) job_club();
		if (num==12) job_screen();
		if (num==13) job_mail();
		if (num==14) job_chat();
		if (num==15) job_mess();
		if (num==16) job_start();
		if (num==17) job_cmd();
		if (num==21) job_system();
		if (num==22) job_limit();
		if (num==23) job_node();
		if (num==24) job_updown();
		if (num==25) job_setbbs();
		if (num==26) job_jpg();
		if (num==27) job_good();
		if (num==28) job_backmode();
		if (num==29) job_notice();
		if (num==31) job_para();
		if (num==32) job_zbtotal();
		if (num==33) job_stat();
		if (num==34) job_info();
		if (who==5&&num==98) edit_syscmd();
		if (who==5&&num==99) job_sysops();
	}
	save_cfg();
	_quit:
	ioctl(0,TCSETA,&termold);	//원래 터미날 모드 복구
}


job_util()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"유틸리티작업");
		printf("  1. 투표장 작업 \n");
		printf("  2. 뉴스 속보   \n");
		printf("  3. 사이버워    \n");
		printf("  4. 사이버홈    \n");
		printf("  5. 사이버머니  \n");
		printf("  6. IP(정보제공)\n");
		printf("  7. 사용 유도 도움말(today mess)설정\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) job_vote();
		if (n== 2) job_news();
		if (n== 3) job_war();
		if (n== 5) job_money();
		if (n== 6) job_ip();
		if (n== 7) job_todaymess();
	}
}




//회원 관련 작업
job_stat()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"통계 관련 작업");
		printf("  1. 일별 회원 가입현황                        \n");
		printf("  2. 회  원   통 계\n");
		printf("  3. 운영진   통 계\n");
		printf("  4. 일반기록 통 계\n");
		printf("  5. 운영기록 통 계\n");
		printf("  6. 접속기록 통 계\n");
		printf("  7. 접속기록 통 계2\n");
		printf("  8. 다운기록 통 계\n");
		printf("  9. 동호회사용기록 통 계\n");
		printf(" 99. 회원 통계\n");
		xy(1,20);
		n=numselect();
		printf("\n\n");
		if (n<= 0) break;
		if (n== 1) view_daypf();
		if (n== 2) view_stat(1);
		if (n== 3) view_stat(2);
		if (n== 4) sys_tem("bin/viewlog 2 '%s' main",cfg.sysop);
		if (n== 5) sys_tem("bin/viewlog 5 '%s' main",cfg.sysop);
		if (n== 6) sys_tem("bin/viewlog 6 '%s' main",cfg.sysop);
		if (n== 7) sys_tem("bin/viewlog 62 '%s' main",cfg.sysop);
		if (n== 8) sys_tem("bin/viewlog 7 '%s' main",cfg.sysop);
		if (n== 9) sys_tem("bin/viewlog 8 '%s' main",cfg.sysop);
		if (n==99) {disp_set_stat();pressenter();}		  //현재 상태 보여주기
	}
}

view_stat(int mode)
{
	FILE *fpid;
	FILE *fpsave;
	int  n,count,i,i2,i3;
	struct tm *tp;

	total=count_pf();				   //회원 총 수 계산
	fpid = fopen(ID_FILE, "r+");
	fpsave = fopen(TMP_FILE, "w+");

	fprintf(fpsave,"          총회원:%5d명         \n",total);

	fseek(fpid,0,SEEK_SET);
	for (n=0;n<20;n++) {
		if (mode==1&&n<14) continue;
		count=0;
		fprintf(fpsave,"   ==========   레벨 %4d  ========== \n",n);
		fseek(fpid,0,SEEK_SET);
		while(fread(&pf,sizeof_pf,1,fpid)) {		 //아이디 저장
			if (strlen(pf.id)<3) continue;
			if (pf.type==n) {
				tp = localtime(&pf.login);
				fprintf(fpsave," %3d.  %-8s %-8s %5d %5d %5d %5d ",
				count+1,pf.id, pf.name, pf.level,pf.countw,pf.countu,pf.countd);
				fprintf(fpsave,"%02d/%02d/%02d\n",
				tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
				count++;
			}
		}
		fprintf(fpsave,"                       == 총 %4d명 == \n",count);
	}

	fclose(fpid);
	fclose(fpsave);
	view_text(TMP_FILE);
	unlink(TMP_FILE);
}



//회원 관련 작업
job_pf()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"회원 관리 작업");
		printf("  1. 전체 회원 검색          (목록형식)        \n");
		printf("  2. ID/성명 빠른 검색/수정                    \n");
		printf("  3. 회원 검색,수정          (분류별검색)      \n");
		printf("  4. 가입대기 회원 가입처리  (신규가입허가 %d명대기)\n",count_rec(GUEST_FILE   ,sizeof_pf));
		printf("  5. 가입대기 회원 선택 가입처리\n");
		printf("  6. 등록거부 회원 추가,해제 (guestnot.dat %d명거부)\n",count_rec(GUESTNOT_FILE,sizeof_pf));
		printf("  7. 회원 재정리/삭제처리    (등급 0 회원처리) \n");
		printf("  8. 신규 회원 아이디 만들기                   \n");
		printf("  9. 지정 번호 아이디 강제수정 (검색실패ID수정)\n");
		printf(" 10. 회원 아이디 인덱스 화일 재정리\n");
		printf(" 11. 회원 READ 시 빠른검색 (실패시->전체검색) %s\n",rbit(cfg.pfmode,1,"일반검색","빠른검색"));
		printf(" 12. 회원신상(PF)                             %s\n",rbit(cfg.pfmode,21,"기본","사용자"));
		printf(" 13. 제한시간,날짜경과시 접속                 %s\n",rbit(cfg.mode,1,"종료","재접속"));
		printf(" 14. 납입날짜제한제도(회비)실시               %s\n",rbit(cfg.mode,3,"무시","실시"));
		printf(" 15. 연습용 아이디 지정                       system/testid.id\n");
		printf(" 16. 사용/진입금지 아이디 지정                system/notlog.id\n");
		printf(" 17. 접속확인아이디(user/rec/log_id.rec)      system/monilog.id\n");
		printf(" 18. 삭제된 회원 전체 복구 등급0 -> 등급1로  \n");
		printf(" 19. 삭제 대상 회원 조회 (등급  0인 회원)    \n");
		printf(" 20. 운 영 진  회원 조회 (등급 16이상인 회원)\n");
		printf(" 21. 등급별 무조건 가기 설정                 (system/auto_go.txt)\n");
        printf(" 22. 전체회원 등급,레벨,접속시간,성별 갱신   \n");
		printf(" 23. 손님지정ID (회원검색안함:등급0지정)     (system/guestid.id)\n");
		printf(" 24. RANK(등수) 시 레벨처리(RK <-> RK2)       %s\n",rbit(cfg.levelmode,2,"레벨보임","안보임"));
		printf(" 25. IND  -> QUICK 검색여부                   %s\n",rbit(cfg.mprocmode,2,"QUICK","순차"));
		n=numselect();printf("\n");
		if (n<= 0) break;
		if (n== 1) sys_tem("bin/pfinfo '%s'",who==5?"root":now_id);
		if (n== 2) sys_tem("bin/pf '%s' set_search",now_id);
		if (n== 3) sys_tem("bin/pf '%s'",now_id);
		if (n== 4) sys_tem("bin/guest 999");
		if (n== 5) sys_tem("bin/guest 998");
		if (n== 6) notfree();
		if (n== 7) sort_pf();	  //회원 재정리
		if (n== 8) add_new_pf();
		if (n== 9) id_num_edit();
		if (n==10) make_ind(0,"make_ind_all");
		if (n==11) cfg.pfmode = gtt_bit(cfg.pfmode,1);
		if (n==12) cfg.pfmode = gtt_bit(cfg.pfmode,21);
		if (n==13) cfg.mode 	  =gtt_bit(cfg.mode,1);
		if (n==14) cfg.mode 	  =gtt_bit(cfg.mode,3);
		if (n==15) xedit("system/testid.id");
		if (n==16) xedit("system/notlog.id");
		if (n==17) xedit("system/monilog.id");
		if (n==18) recover();
		if (n==19) check_pf(1,who);
		if (n==20) check_pf(2,who);
		if (n==21) auto_go();
		if (n==22) reset_all();
		if (n==23) xedit("system/guestid.id");
		if (n==24) cfg.levelmode=gtt_bit(cfg.levelmode,2);
		if (n==25) cfg.mprocmode=gtt_bit(cfg.mprocmode,2);
	}
}


//회원 관련 작업
reset_all()
{
	int num,i,type1,type2;
	char buff[100];
	type1=type2=i=0;
	mess("이 기능은 전체 회원 설정 기능입니다. (백업권장)");
	while (1) {
		cls();
		print_(1,"전체 회원 등급,레벨 설정");
		printf(" 1. 변경 대상 등급 지정     %d 부터  %d 등급 \n",type1,type2);
		printf(" 2. 전체 회원 등급 재조정                    \n");
		printf(" 3. 전체 회원 레벨 재조정                    \n");
		printf(" 4. 전체 회원 전체접속수 재조정              \n");
		printf(" 5. 전체 회원 금월접속수 재조정              \n");
		printf(" 6. 전체 회원 전체사용시간재조정             \n");
		printf(" 7. 전체 회원 금월사용시간재조정             \n");
		printf(" 8. 전체 회원 주민등록번호 삭제              \n");
		printf(" 9. 전체 회원 주민등록번호 -> 성별지정       \n");
		printf("10. 전체 회원 쓰기/등록/다운수 0으로 지정    \n");
        printf("11. 전체 회원 WWW 쓰기         0으로 지정    \n");
        printf("12. 전체 회원 CASH 지정                      \n");
		n=numselect();printf("\n");
		if (n<= 0) break;
		if (n== 1) {
			printf("대상등급 시작? >>");type1=get_num(type1,2);
			printf("대상등급 끝  ? >>");type2=get_num(type2,2);
		}
		else if (n== 2) {
			printf("재조정 등급? >>");i=numselect2();
			if (i<1||i>14) mess(" 가능등급 1~14");
			else make_reset(n,i,type1,type2);
		}
		else if (n== 3) {
			printf("재조정 레벨? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 4) {
			printf("재조정 전체접속수? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 5) {
			printf("재조정 금월접속수? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 6) {
			printf("재조정 전체사용시간? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 7) {
			printf("재조정 금월사용시간? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 8) {
			if (yesno("전체 회원의 주민등록 번호를 삭제할까요? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
		else if (n== 9) {
			if (yesno("주민등록번호로 성별을 지정할까요? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
		else if (n==10) {
			if (yesno("쓰기/등록/다운수를 0으로 지정할까요? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
        else if (n==11) {
            if (yesno("WWW 쓰기수를 0으로 지정할까요? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
        else if (n==12) {
			printf("재조정 CASH? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
	}
}


make_reset(int mode ,int num,int type1,int type2)
{
	int i,n,n2,nn,count,stop=Yes;
	char sex;
	char buff[100];
	FILE *fp;
	if (!yesno2("\n ### 전체 회원을 세팅할까요? (y/N)>>",No)) return;
	if ((fp=fopen(ID_FILE,"r+"))==NULL) return;
	i=0;
	count=0;
	printf("\n");
	if (type1==0&&type2==0) {mess("변경대상등급을 지정하세요");return;}
	while (1) {
		fseek(fp,i*sizeof_pf,SEEK_SET);
		if (i<0||!fread(&pf,sizeof_pf,1,fp)) break;
		if (pf.type<type1||pf.type>type2||pf.type>=18) goto _cont;
		printf("%-8s (%-8s) ",pf.id,pf.name);
			 if (mode==2) {printf("등급변경 %d -> %d\n",pf.type,num);pf.type=num;}
		else if (mode==3) {printf("레벨변경 %d -> %d\n",pf.level,num);pf.level=num;}
		else if (mode==4) {printf("접속수변경 %d -> %d\n",pf.all_log,num);pf.all_log=num;}
		else if (mode==5) {printf("월별접속수변경 %d -> %d\n",pf.month_log,num);pf.month_log=num;}
		else if (mode==6) {printf("전체사용시간변경 %d -> %d\n",pf.all_time,num);pf.all_time=num;}
		else if (mode==7) {printf("월별사용시간변경 %d -> %d\n",pf.month_time,num);pf.month_time=num;}
		else if (mode==8) {printf("주민등록삭제 %s -> 삭제\n",pf.id_no);strcpy(pf.id_no,"");}
		else if (mode==9) {
			sex=pf.sex;
			pf.sex=pf.id_no[7]=='2'?'f':'m';
			printf("성별변경     %c -> %c\n",pf.sex,sex);pf.sex=sex;
		}
		fseek(fp,i*sizeof_pf,SEEK_SET);
		pf.num=i;
//		  fwrite(&pf,sizeof_pf,1,fp);
		gwrite(202,pf,fp);
		count++;
		if (count%30==0&&stop) {
			printf("\n 현재 %d 자료처리중  계속(Enter) 중지(Q) 연속(S)>>",count);
			lineinput(buff,2);printf("\n");
			if (is_same(buff,"s")) stop=No;
			else if (is_quit(buff)) break;
		}
		else if (mode==10) {printf("쓰기/등록/다운수삭제\n");pf.countw=pf.countu=pf.countd=0;}
        else if (mode==11) {printf("WWW 쓰기삭제\n");pf.wwrite=0;}
        else if (mode==12) {printf("CASH 변경 %d -> %d\n",pf.cash,num);pf.cash=num;}
		_cont:
		i++;
	}
	fclose (fp);
	mess("총 %d명의 자료가 변경되었습니다.",count);
}


job_guest()
{
	while (1) {
		cls();
		print_(1,"손님관련 설정");
		printf("  1. 손님 접속 아이디           :%s\n",cfg.guestid    );
		printf("  2. 손님 자동 가입             :%s\n",rbit(cfg.guestmode,15, "수동가입", "자동가입"));
		printf("  3. 손님 접속후 바로 가입      :%s\n",rbit(cfg.guestmode,16, "보통", "바로가입"));
		printf("  4. 손님 접속시 별명 입력      :%s\n",rbit(cfg.guestmode,17, "입력", "통과"));
		printf("  5. 손님시 일련 번호           :%s\n",rbit(cfg.guestmode,18, "보통", "번호"));
		printf("  7. 손님 접속시 서비스메뉴로   :%s\n",rbit(cfg.guestmode,2,  "보통", "가기"));
		printf("  8. 손님 접속시 가입도움말     :%s\n",rbit(cfg.guestmode,6,  "보통", "가기"));
		printf("  9. 손님 접속시 끝냄           :%s\n",rbit(cfg.guestmode,1,  "보통", "끝냄"));
		printf(" 10. 손님 SAY 명령 사용         :%s\n",rbit(cfg.chatmode,10,  "가능", "불가"));
		printf(" 11. 손님 MM  명령 사용         :%s\n",rbit(cfg.chatmode,11,  "가능", "불가"));
		printf(" 12. 손님 진입 시 초기 (선택)   :%s\n",rbit(cfg.guestmode,8,  "통과", "선택"));
		printf(" 13. 접속시Enter 바로손님처리   :%s\n",rbit(cfg.guestmode,21, "보통", "손님"));
		printf(" 14. 아이디입력 여부            :%s\n",rbit(cfg.guestmode,20, "가능", "묻지않음,전부손님"));
		printf(" 15. 손님접속시 하단 가입안내   :%s\n",rbit(cfg.guestmode,9,  "안내", "무시"));
		printf(" 16. ID입력5회초과시 접속처리   :%s\n",rbit(cfg.guestmode,13, "손님으로","종료"));
		printf(" 17. 손님접속시 /xguest 동작    :%s\n",rbit(cfg.guestmode,14, "일반메뉴","손님전용"));
		n=numselect();xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)					get_str(cfg.guestid  , 8);
		if (n== 2)	cfg.guestmode  =gtt_bit(cfg.guestmode,15);
		if (n== 3)	cfg.guestmode  =gtt_bit(cfg.guestmode,16);
		if (n== 4)	cfg.guestmode  =gtt_bit(cfg.guestmode,17);
		if (n== 5)	cfg.guestmode  =gtt_bit(cfg.guestmode,18);
		if (n== 6)	cfg.guestmode  =gtt_bit(cfg.guestmode, 5);
		if (n== 7)	cfg.guestmode  =gtt_bit(cfg.guestmode, 2);
		if (n== 8)	cfg.guestmode  =gtt_bit(cfg.guestmode, 6);
		if (n== 9)	cfg.guestmode  =gtt_bit(cfg.guestmode, 1);
		if (n==10)	cfg.chatmode   =gtt_bit(cfg.chatmode ,10);
		if (n==11)	cfg.chatmode   =gtt_bit(cfg.chatmode ,11);
		if (n==12)	cfg.guestmode  =gtt_bit(cfg.guestmode, 8);
		if (n==13)	cfg.guestmode  =gtt_bit(cfg.guestmode,21);
		if (n==14)	cfg.guestmode  =gtt_bit(cfg.guestmode,20);
		if (n==15)	cfg.guestmode  =gtt_bit(cfg.guestmode, 9);
		if (n==16)	cfg.guestmode  =gtt_bit(cfg.guestmode,13);
		if (n==17)	cfg.guestmode  =gtt_bit(cfg.guestmode,14);
	}
}


job_guestinput()
{
	int n,i;
	char buff[300];
	while (1) {
		cls();
		print_(0,"가입 관련 설정");
		printf("  1. 손님 접속시 가입후종료           %s\n",rbit(cfg.guestmode,3,"보통","종료"));
		printf("  2. 손님 접속시 가입후재접속         %s\n",rbit(cfg.guestmode,4,"보통","접속"));
		printf("  3. 손님 질문 항목 선택              \n");
		printf("  4. 가입후 초기 기본값지정 (등급..)  :%d\n",cfg.guesttype  );
		printf("  5. 손님 승인 비밀 번호 관리         \n");
		printf("  9. 가입 금지 아이디 지정                       (system/notguest.txt)\n");
		printf(" 10. 가입 금지 주민등록번호 지정                 (system/bad_idno.txt)\n");
		printf(" 11. 가입 금지 아이디 지정 사유 화면             (logo/notguest.log)\n");
		printf(" 12. 가입 금지 사유(가입조건설명)화면            (logo/notenter.log)\n");
		printf(" 13. 아이디 영문     가능             :%s\n",rbit(cfg.guestin,20, "가능", "불가"));
		printf(" 14. 아이디 한글     가능             :%s\n",rbit(cfg.guestin,21, "가능", "불가"));
		printf(" 15. 아이디 한자     가능             :%s\n",rbit(cfg.guestin,22, "가능", "불가"));
		printf(" 16. 아이디 특수문자 가능             :%s\n",rbit(cfg.guestin,23, "가능", "불가"));
		printf(" 17. 비밀번호시 쉬운비밀번호 가능     :%s\n",rbit(cfg.guestin,24, "허용", "불가"));
		printf(" 18. 주소입력시 전체주소 편집         :%s\n",rbit(cfg.guestin,25, "허용", "불가"));
		printf(" 19. 가입시 질문 항목 설정\n");
		printf(" 20. 주민등록번호 입력시 자동 통과 문자열        (system/passindo.txt)    \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.guestmode  =gtt_bit(cfg.guestmode,3);
		if (n== 2) cfg.guestmode  =gtt_bit(cfg.guestmode,4);
		if (n== 3) guest_input_item();
		if (n== 4) guest_init();
		if (n== 5) job_epass();
		if (n== 9) xedit("system/notguest.txt");
		if (n==10) xedit("system/bad_idno.txt");
		if (n==11) xedit("frm/guest/notguest.log");
		if (n==12) xedit("frm/guest/notenter.log");
		if (n==13) cfg.guestin=gtt_bit(cfg.guestin,20);
		if (n==14) cfg.guestin=gtt_bit(cfg.guestin,21);
		if (n==15) cfg.guestin=gtt_bit(cfg.guestin,22);
		if (n==16) cfg.guestin=gtt_bit(cfg.guestin,23);
		if (n==17) cfg.guestin=gtt_bit(cfg.guestin,24);
		if (n==18) cfg.guestin=gtt_bit(cfg.guestin,25);
		if (n==19) guest_input_item();
		if (n==20) xedit("system/passidno.txt");
	}
}

job_epass()
{
	int n,i,eenum;
	char buff[300];
	char filename[300];

	strcpy(filename,"system/epass.num");
	while (1) {
		file_line(buff,filename,1);
		eenum=atoi(buff);
		cls();
		print_(0,"승인 번호 관련 설정");
		printf("  1. 가입후 승인 암호입력후 등급1지정   %s\n",rbit(cfg.guestmode,25,"보통","지정"));
		printf("  2. 가입후 승인 암호입력후 승급2가능   %s\n",rbit(cfg.guestmode,26,"보통","지정"));
		printf("  3. ID 입력후   승인 암호입력          %s\n",rbit(cfg.guestmode,27,"보통","지정"));
		printf("  4. 승인번호 자동 지정 (0:난수 #:지정) %d\n",eenum);
		printf("  7. 회원 승인 암호 임의 지정         \n");
		printf("  8. 회원 승인 암호 임의 삭제(편집)   \n");
		printf("  9. 회원 승인 암호 기록 확인         \n");
		n=numselect();
		if (n<= 0) break;
		if (n== 1) cfg.guestmode =gtt_bit(cfg.guestmode,25);
		if (n== 2) cfg.guestmode =gtt_bit(cfg.guestmode,26);
		if (n== 3) cfg.guestmode =gtt_bit(cfg.guestmode,27);
		if (n== 4) xedit(filename);
		if (n== 7) make_epass(1);
		if (n== 8) make_epass(2);
		if (n== 9) make_epass(3);
	}
}

make_epass(int mode)
{
	FILE *fp;
	char buff[300];
	char buff2[300];
	char buff3[300];
	char filename[300];

	if (mode==1) {
		while(1) {
			printf("\n  승인 암호 지정 ID :");
			lineinput(buff,8);
			if (strlen(buff)<3) break;
			if (!read_pf4(buff)) {
				printf("  %s ID를 찾을수 없습니다. \n",buff);
				continue;
			}
			printf(" 이름:%s  등급:%d",pf.id,pf.type);
			printf("\n  승인 암호 입력 (4~12자리) :");
			lineinput(buff2,8);
			if (strlen(buff2)<4) continue;
			sprintf(buff,"user/epass/%s.dat",pf2.id);
			if ((fp=fopen(buff,"w"))!=NULL) {
				sprintf(buff3,"%s %-8s %-8s %s",buff2,pf2.id,pf2.name,now_str(2));
				fprintf(fp,"%s\n",buff3);
				fclose(fp);
			}
			sprintf(buff,"user/epass/%s.lst",now_str(12));
			if ((fp=fopen(buff,"a+"))==NULL) fp=fopen(buff,"w+");
			if (fp!=NULL) {
				fprintf(fp,"%s\n",buff3);
				fclose(fp);
			}
			printf("  %s -> %s 승인지정",pf2.id,buff2);
		}
	}
	else if (mode==2) {
		while(1) {
			printf("\n  승인 암호 편집/삭제 ID :");
			lineinput(buff2,8);
			if (strlen(buff2)<3) break;
			sprintf(filename,"user/epass/%s.dat",buff2);
			if (!is_file(filename)) {
				printf("  %s 승인번호파일이 없습니다. \n",filename);
				continue;
			}
			xedit(filename);
			if (file_size(filename)<5) {
				unlink(filename);
				printf("\n  %s 승인번호파일이 삭제되었습니다. \n",filename);
			}
		}
	}
	else if (mode==3) {
		while(1) {
			printf("\n  기록을 확인할 년도월  (예99년5월:9905)>>");
			lineinput(buff2,5);
			if (strlen(buff2)==0) strcpy(buff2,now_str(12));
			if (strlen(buff2)<3) break;
			sprintf(filename,"user/epass/%s.lst",buff2);
			xedit(filename);
		}
	}
}



guest_init()
{
	int n,i;
	char buff[300];
	while (1) {
		cls();
		print_(0,"가입시 초기값 관련 설정");
		printf("  1. 가입후 초기 등급 (0-16)      :%d\n",cfg.guesttype  );
		printf("  2. 가입후 초기 레벨             :%d\n",cfg.guestlevel );
		printf("  3. 가입후 초기 시간 (분)        :%d\n",cfg.guesttime  );
		printf("  4. 가입후 초기 다운용량K        :%d\n",cfg.guestdown  );
		printf("  5. 가입후 초기 금액             :%d\n",cfg.guestcash  );
		printf("  6. 가입후 초기 체력             :%d\n",cfg.guestp_ph  );
		printf("  7. 가입후 초기 지력             :%d\n",cfg.guestp_it  );
		printf("  8. 가입후 초기 매력             :%d\n",cfg.guestp_at  );
		printf("  9. 가입후 초기 방어력           :%d\n",cfg.guestp_pr  );
		printf(" 10. 가입후 초기 모드             :%d\n",cfg.guestp_mo  );
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.guesttype  =get_num(cfg.guesttype  , 2);
		if (n== 2) cfg.guestlevel =get_num(cfg.guestlevel , 6);
		if (n== 3) cfg.guesttime  =get_num(cfg.guesttime  , 8);
		if (n== 4) cfg.guestdown  =get_num(cfg.guestdown  , 8);
		if (n== 5) cfg.guestcash  =get_num(cfg.guestcash  , 8);
		if (n== 6) cfg.guestp_ph  =get_num(cfg.guestp_ph  , 8);
		if (n== 7) cfg.guestp_it  =get_num(cfg.guestp_it  , 8);
		if (n== 8) cfg.guestp_at  =get_num(cfg.guestp_at  , 8);
		if (n== 9) cfg.guestp_pr  =get_num(cfg.guestp_pr  , 8);
		if (n==10) cfg.guestp_mo  =get_num(cfg.guestp_mo  , 8);
	}
}


guest_input_item()
{
	while (1) {
		cls();
		print_(0,"가입시 질문 관련 설정");
		printf("  1. 가입시 ID   물어봄  %s\n",rbit(cfg.guestin, 1," 함 ","않함"));
		printf("  2. 가입시 이름 물어봄  %s\n",rbit(cfg.guestin, 2," 함 ","않함"));
		printf("  3. 주민등록번호물어봄  %s\n",rbit(cfg.guestin, 3," 함 ","않함"));
		printf("  4. 생년월일 물어봄     %s\n",rbit(cfg.guestin, 4," 함 ","않함"));
		printf("  5. 성별     물어봄     %s\n",rbit(cfg.guestin, 5," 함 ","않함"));
		printf("  6. 암호물어봄          %s\n",rbit(cfg.guestin, 6," 함 ","않함"));
		printf("  7. 주소물어봄          %s\n",rbit(cfg.guestin, 7," 함 ","않함"));
		printf("  8. 전화물어봄          %s\n",rbit(cfg.guestin, 8," 함 ","않함"));
		printf("  9. 우편주소            %s\n",rbit(cfg.guestin, 9," 함 ","않함"));
		printf(" 10. 삐삐물어봄          %s\n",rbit(cfg.guestin,10," 함 ","않함"));
		printf(" 11. 핸드폰물어봄        %s\n",rbit(cfg.guestin,11," 함 ","않함"));
		printf(" 12. 직업물어봄          %s\n",rbit(cfg.guestin,12," 함 ","않함"));
		printf(" 13. 직장물어봄          %s\n",rbit(cfg.guestin,13," 함 ","않함"));
		printf(" 14. 직장전화            %s\n",rbit(cfg.guestin,14," 함 ","않함"));
		printf(" 15. 참고사항 1          %s\n",rbit(cfg.guestin,15," 함 ","않함"));
		printf(" 16. 참고사항 2          %s\n",rbit(cfg.guestin,16," 함 ","않함"));
		printf(" 17. 참고사항 3          %s\n",rbit(cfg.guestin,17," 함 ","않함"));
		printf(" 18. 참고사항 4          %s\n",rbit(cfg.guestin,18," 함 ","않함"));
		printf(" 19. 참고사항 5          %s\n",rbit(cfg.guestin,19," 함 ","않함"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.guestin=gtt_bit(cfg.guestin,1);
		if (n== 2) cfg.guestin=gtt_bit(cfg.guestin,2);
		if (n== 3) cfg.guestin=gtt_bit(cfg.guestin,3);
		if (n== 4) cfg.guestin=gtt_bit(cfg.guestin,4);
		if (n== 5) cfg.guestin=gtt_bit(cfg.guestin,5);
		if (n== 6) cfg.guestin=gtt_bit(cfg.guestin,6);
		if (n== 7) cfg.guestin=gtt_bit(cfg.guestin,7);
		if (n== 8) cfg.guestin=gtt_bit(cfg.guestin,8);
		if (n== 9) cfg.guestin=gtt_bit(cfg.guestin,9);
		if (n==10) cfg.guestin=gtt_bit(cfg.guestin,10);
		if (n==11) cfg.guestin=gtt_bit(cfg.guestin,11);
		if (n==12) cfg.guestin=gtt_bit(cfg.guestin,12);
		if (n==13) cfg.guestin=gtt_bit(cfg.guestin,13);
		if (n==14) cfg.guestin=gtt_bit(cfg.guestin,14);
		if (n==15) cfg.guestin=gtt_bit(cfg.guestin,15);
		if (n==16) cfg.guestin=gtt_bit(cfg.guestin,16);
		if (n==17) cfg.guestin=gtt_bit(cfg.guestin,17);
		if (n==18) cfg.guestin=gtt_bit(cfg.guestin,18);
		if (n==19) cfg.guestin=gtt_bit(cfg.guestin,19);
	}
}


job_idout()
{
	while (1) {
		cls();
		printf("            가입 해지 관련 설정\n");
		printfL2();
		printf("  1.  가입 해지 가능                :%s\n",rbit(cfg.loginmode,18, "가능", "불가"));
		printf("  2.  가입 해지시 주민등록 보관     :%s\n",rbit(cfg.loginmode,19, "보관", "삭제"));
		printfL2();
		n=numselect();
		xy(60,n+2);
		if (n<=0)  break;
		if (n==1)  cfg.loginmode  =gtt_bit(cfg.loginmode,18);
		if (n==2)  cfg.loginmode  =gtt_bit(cfg.loginmode,19);
	}
}


job_notice()
{
	while (1) {
		cls();
		print_(1,"공지사항 설정");
		printf(" 1. 일반회원 접속시 출력여부           :%s\n",rbit(cfg.notice,0,"출력","통과"));
		printf(" 2. 손님     접속시 출력여부           :%s\n",rbit(cfg.notice,1,"출력","통과"));
		printf(" 3. 공지사항 출력방법 ID 입력전        :%s\n",rbit(cfg.notice,2,"안보임","보임"));
		printf(" 4. 공지사항 출력방법 TOP진입전        :%s\n",rbit(cfg.notice,3,"안보임","보임"));
		printf(" 5. 공지사항 광고 출력                 :%s\n",rbit(cfg.notice,4,"함","안함"));
		printf(" 6. 공지사항 출력모양 (0~9)            :%d\n",cfg.noticet);
		printf(" 7. 공지사항 출력시작 위치 X           :%d\n",cfg.noticex);
		printf(" 8. 공지사항 출력시작 위치 Y           :%d\n",cfg.noticey);
		printf(" 9. 공지사항 출력시작 줄길이           :%d\n",cfg.noticeline);
		n=numselect();xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.notice	  =gtt_bit(cfg.notice ,0);
		if (n== 2) cfg.notice	  =gtt_bit(cfg.notice ,1);
		if (n== 3) cfg.notice	  =gtt_bit(cfg.notice ,2);
		if (n== 4) cfg.notice	  =gtt_bit(cfg.notice ,3);
		if (n== 5) cfg.notice	  =gtt_bit(cfg.notice ,4);
		if (n== 6) cfg.noticet	  =get_num(cfg.noticet,2);
		if (n== 7) cfg.noticex	  =get_num(cfg.noticex,2);
		if (n== 8) cfg.noticey	  =get_num(cfg.noticey,2);
		if (n== 9) cfg.noticeline =get_num(cfg.noticeline,2);
	}
}



//등급별 바로가기
auto_go()
{
	FILE *fp;
	char buff[200];
	char filename[200];
	char auto_gocode[20][51];
	int i;
	for (i=0;i<20;i++) strcpy(auto_gocode[i],"");
	strcpy(filename,"system/auto_go.txt");
	if ((fp=fopen(filename,"r+"))!=NULL) {
		for (i=0;i<20;i++) {
			if (fgets(buff,200,fp)) {
				del_enter(buff);
				del_esc(buff);
				buff[50]=0;
				strcpy(auto_gocode[i],buff);
			}
		}
		fclose(fp);
	}
	while (1) {
		cls();
		print_(0,"등급별 자동 가기 명령 설정");
		for (i=0;i<20;i++) {
			printf("  등급 %2d. %s\n",i+1,auto_gocode[i]);
		}
		printf(" 선택 (0~19)  취소(Q)  저장(W) >>");
		lineinput(buff,20);n=atoi(buff);
		if (is_quit(buff)) break;
		if (is_same(buff,"w")) break;
		if (buff[0]==0) break;
		if (n==0&&!is_same(buff,"0")) continue;
		if (n>=0&&n<=20) {
			xy(40,n+2);
			get_str(auto_gocode[n-1],50);
		}
	}
	if ((fp=fopen(filename,"w+"))!=NULL) {
		for (i=0;i<20;i++) {
			del_enter(auto_gocode[i]);
			del_esc(auto_gocode[i]);
			fprintf(fp,"%s\n",auto_gocode[i]);
		}
		fclose(fp);
	}
}



//분류/코드 관련 작업
job_type()
{
	while (1) {
		cls();
		print_(0,"등급/분류/코드 설정");
		printf("  1. 자동 등급 조정 기능                       :%s\n",rbit(cfg.levelmode,1, "수동", "자동"));
		printf("  2. 회원 레벨 상한                            :%d\n",cfg.levelmax   );
		printf("  3. 동호회의 최대 증가 가능 레벨 (0:무시~250) :%u\n",cfg.clublevel  );
		printf("  4. 등급별 명칭      수정 \n");
		printf("  5. 등급별 승급/진입레벨 수정 \n");
		printf("  6. 등급별 일일사용 제한 시간 \n");
		printf("  7. 등급별 1회 접속 제한 시간 \n");
		printf("  8. 직업별 분류 코드 수정 \n");
		printf("  9. 주소별 분류 코드 수정 \n");
		printf(" 10. 게시판/동호회 추천코드\n");
		n=numselect();xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.levelmode =gtt_bit(cfg.levelmode , 1);
		if (n== 2) cfg.levelmax  =get_num(cfg.levelmax	, 8);
		if (n== 3) cfg.clublevel =get_num(cfg.clublevel , 3);
		if (n== 4) edit_atype(1);
		if (n== 5) edit_atype(2);
		if (n== 6) edit_atype(3);
		if (n== 7) edit_atype(4);
		if (n== 8) edit_ajob();
		if (n== 9) edit_addr();
		if (n==10) edit_gg();
	}
}



job_jpg()
{
	while (1) {
		cls();
		print_(0,"화상관련 설정");
		printf(" 1. 자료실에서 SEE 사용가능       :%s\n",rbit(cfg.jpgmode,0, "허용", "불허"));
		printf(" 2. 화상자료 JPG 문자 ♣          :%s\n",cfg.jpgchar);
		printf(" 3. 화상게시판에서 JPG 문자 표시  :%s\n",rbit(cfg.jpgmode,1, "표시", "공백"));
		printf(" 4. 일반게시판에서 JPG 문자 표시  :%s\n",rbit(cfg.jpgmode,2, "표시", "공백"));
		printf(" 5. 일반자료실에서 JPG 문자 표시  :%s\n",rbit(cfg.jpgmode,3, "표시", "공백"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 0);
		if (n== 2)	get_str(cfg.jpgchar,2);
		if (n== 3)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 1);
		if (n== 4)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 2);
		if (n== 5)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 3);
	}
}





job_war()
{
	while (1) {
		cls();
		printf(" 1.  메  인 최소보호망  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 2.  동호회 최소보호망  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 3.  회  원 최소보호망  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 4.  전투대 최소보호망  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 5.  소모임 최소보호망  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 6.  회원 일일최대손실  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 7.  회원 일일최대이득  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printfL2();
		n=numselect();
		xy(60,n);
		if (n<= 0)	break;
	}
}

job_mess()
{
	while (1) {
		cls();
		printf(" ### 메세지 관련 설정\n");
		printf("  1.  텔넷시 메세지 전송             :%s\n",rbit(cfg.messmode,1,"허용", "금지"));
		printf("  2.  글쓰기시 메세지 전송           :%s\n",rbit(cfg.messmode,4,"허용", "금지"));
		printf("  3.  대화시 메세지 전송             :%s\n",rbit(cfg.messmode,5,"허용", "금지"));
		printf("  4.  외부명령시 메세지 전송         :%s\n",rbit(cfg.messmode,6,"허용", "금지"));
		printf("  5.  손님접속시     잠깐도움말      :%s\n",rbit(cfg.messmode,2,"보임", "무시"));
		printf("  6.  일반회원접속시 잠깐도움말      :%s\n",rbit(cfg.messmode,3,"보임", "무시"));
		printf("  7.  잠깐도움말 보일 레벨           :%d\n",cfg.helplevel);
		printf("  8.  잠깐도움말 보일 등급           :%d\n",cfg.helptype);
		printf("  9.  잠깐도움말 보일 접속수         :%d\n",cfg.helplog);
		printf(" 10.  전체 메세지 사용 등급          :%d\n",cfg.allsay1);
		printf(" 11.  전체 메세지 사용 등급(메인)    :%d\n",cfg.allsay2);
		printf(" 12.  전체 메세지 사용 등급(동호회)  :%d\n",cfg.allsay3);
		printf(" 13.  같은 호스트 -> xmessd 처리     :%s\n",rbit(cfg.messmode,7,"tty사용", "xmessd사용"));
		printf(" 14.  MESS/SAY 시 선택방법           :%s\n",rbit(cfg.messmode,8,"US 화면","명단목록"));
		printf(" 20.  US 관련 설정               \n");
		n=numselect();
		printf("\n");
		xy(60,n);
		if (n<= 0)	break;
		if (n== 1)	cfg.messmode=gtt_bit(cfg.messmode,1);
		if (n== 2)	cfg.messmode=gtt_bit(cfg.messmode,4);
		if (n== 3)	cfg.messmode=gtt_bit(cfg.messmode,5);
		if (n== 4)	cfg.messmode=gtt_bit(cfg.messmode,6);
		if (n== 5)	cfg.messmode=gtt_bit(cfg.messmode,2);
		if (n== 6)	cfg.messmode=gtt_bit(cfg.messmode,3);
		if (n== 7)	cfg.helplevel=get_num(cfg.helplevel,3);
		if (n== 8)	cfg.helptype =get_num(cfg.helptype,3);
		if (n== 9)	cfg.helplog  =get_num(cfg.helplog,3);
		if (n==10)	cfg.allsay1  =get_num(cfg.allsay1,2);
		if (n==11)	cfg.allsay2  =get_num(cfg.allsay2,2);
		if (n==12)	cfg.allsay3  =get_num(cfg.allsay3,2);
		if (n==13)	cfg.messmode=gtt_bit(cfg.messmode,7);
		if (n==14)	cfg.messmode=gtt_bit(cfg.messmode,8);
		if (n==20)	job_us();
	}
}

//진입 관련 작업
job_start()
{
	while (1) {
		cls();
		print_(1,"진입 관련 설정");
		printf("  1. 진입시 바로 메인메뉴 진입                 :%s\n",rbit(cfg.startmode,1,"보통","바로진입"));
		printf("  2. 진입시 시작 로고 출력                     :%s\n",rbit(cfg.startmode,2,"출력","무시"));
		printf("  3. 진입시 전체 임시 ID 자동 부여             :%s\n",rbit(cfg.startmode,3,"출력","무시"));
		printf("  4. 아이디입력후 최종접속  출력               :%s\n",rbit(cfg.startmode,4,"출력","무시"));
		printf("  5. 아이디입력후 현재시간  출력               :%s\n",rbit(cfg.startmode,5,"출력","무시"));
		printf("  6. 아이디입력후 생일확인  출력               :%s\n",rbit(cfg.startmode,9,"출력","무시"));
		printf("  7. 아이디입력후 생체리듬  출력               :%s\n",rbit(cfg.startmode,6,"출력","무시"));
		printf("  8. 아이디입력후 생체설명  출력               :%s\n",rbit(cfg.startmode,10,"출력","무시"));
		printf("  9. 아이디입력후 메일확인  출력               :%s\n",rbit(cfg.startmode,7,"출력","무시"));
		printf(" 10. 아이디입력후 Enter키   입력               :%s\n",rbit(cfg.startmode,8,"Enter","1초대기"));
		printf(" 11. 접속시 7일동안 연속접속여부 확인          :%s\n",rbit(cfg.startmode,11,"안함","확인함"));
		printf(" 12. 연속접속시 레벨/다운량 증가치 (system/contday.dat)\n");
		printf(" 13. 공지전 Enter키   입력                     :%s\n",rbit(cfg.startmode,13,"Enter","무시"));
		printf(" 20. 진입시 자동스크립터                       :%s\n",rbit(cfg.startmode,14,"동작","무시"));
		printf(" 21. 진입시 대화실 접속 확인                   :%s\n",rbit(cfg.startmode,15,"동작","무시"));
		printf(" 22. 진입시 하드 잔량 50MB 확인                :%s\n",rbit(cfg.startmode,16,"무시","확인"));
		printf(" 23. 종결시 Y/n 질문함                         :%s\n",rbit(cfg.mode,6, "질문", "무시"));
		printf(" 24. 종료시 화면 출력                          :%s\n",rbit(cfg.logomode,22,"보통","안함"));
		printf(" 25. 시간지연 종료시 종료 화면없이 종료        :%s\n",rbit(cfg.startmode,17,"보통","없음"));
		n=numselect();
		if (n<= 0) break;
		if (n== 1) cfg.startmode=gtt_bit(cfg.startmode, 1);
		if (n== 2) cfg.startmode=gtt_bit(cfg.startmode, 2);
		if (n== 3) cfg.startmode=gtt_bit(cfg.startmode, 3);
		if (n== 4) cfg.startmode=gtt_bit(cfg.startmode, 4);
		if (n== 5) cfg.startmode=gtt_bit(cfg.startmode, 5);
		if (n== 6) cfg.startmode=gtt_bit(cfg.startmode, 9);
		if (n== 7) cfg.startmode=gtt_bit(cfg.startmode, 6);
		if (n== 8) cfg.startmode=gtt_bit(cfg.startmode,10);
		if (n== 9) cfg.startmode=gtt_bit(cfg.startmode, 7);
		if (n==10) cfg.startmode=gtt_bit(cfg.startmode, 8);
		if (n==11) cfg.startmode=gtt_bit(cfg.startmode,11);
		if (n==12) xedit2("system/contday.dat");
		if (n==13) cfg.startmode=gtt_bit(cfg.startmode,13);
		if (n==20) cfg.startmode=gtt_bit(cfg.startmode,14);
		if (n==21) cfg.startmode=gtt_bit(cfg.startmode,15);
		if (n==22) cfg.startmode=gtt_bit(cfg.startmode,16);
		if (n==23) cfg.mode 	=gtt_bit(cfg.mode, 6);
		if (n==24) cfg.logomode =gtt_bit(cfg.logomode,22);
		if (n==25) cfg.startmode=gtt_bit(cfg.startmode,17);
	}
}



job_cmd()
{
	while (1) {
		cls();
		print_(1,"명령/입력 관련 설정");
		printf(" 1. 메인메뉴에서 P사용시 종료 여부            :%s\n",rbit(cfg.mode,30,"보통","종료"));
		printf(" 2. 명령입력시 자동영문전환                   :%s\n",rbit(cfg.mode,4,"자동변환","수동변환"));
		printf(" 3. 명령어 자동 기억(화살표이동) 기능         :%s\n",rbit(cfg.mode,8,"사용","안함"));
		printf(" 4. 서브메뉴 하단명령입력위치 고정            :%s\n",rbit(cfg.mode,11,"가변","고정"));
		printf(" 5. 단축명령키 지정         (system/shortcmd.dat)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.mode=gtt_bit(cfg.mode,30);
		if (n== 2) cfg.mode=gtt_bit(cfg.mode,4);
		if (n== 3) cfg.mode=gtt_bit(cfg.mode,8);
		if (n== 4) cfg.mode=gtt_bit(cfg.mode,11);
		if (n== 5) xedit2("system/shortcmd.dat");
	}
}





job_chat()
{
	while (1) {
		cls();
		print_(0,"대화실설정");
		printf("  1.  대화실 경고시작시간                    :%d\n",cfg.chatwarn   );
		printf("  2.  대화실 진입가능등급                    :%d\n",cfg.chattype   );
		printf("  3.  메세지 전송시 보낸ID 반전함            :%s\n",rbit(cfg.mode,16, "보통", "반전"));
		printf("  4.  동호회 대화실 ST표시                   :%s\n",rbit(cfg.mode,23, "표시", "감춤"));
		printf("  5.  대화실 분류별선택기능                  :%s\n",rbit(cfg.chatmode,0, "선택", "통합"));
		printf("  6.  손님   전체 대화실 사용                :%s\n",rbit(cfg.chatmode,13, "불가능", "가능"));
		printf("  7.  대화실 진입시 별명사용                 :%s\n",rbit(cfg.chatmode,14, "별명", "본명"));
		printf("  8.  손님시 99대화실 사용                   :%s\n",rbit(cfg.chatmode,15, "불가능", "가능"));
		printf("  9.  대화실 진입시 비 사용자 제거           :%s\n",rbit(cfg.chatmode,16, "제거", "무시"));
		printf(" 10.  대화실 진입시 초기 모드 (0~12)         :%d\n",cfg.chatst);
		printf(" 11.  SAY시최대전송줄수(0:무제한 1-255:제한) :%d\n",cfg.sayline);
		printf(" 12.  대화실에서 타자방 동작여부             :%s\n",rbit(cfg.chatmode,18, "모든대화실", "타입:71?"));
		printf(" 13.  대화실에서 퀴즈방 동작여부             :%s\n",rbit(cfg.chatmode,19, "모든대화실", "타입:72?"));
        printf(" 14.  www.3B.co.kr 인터넷대화실 연결         :%s\n",rbit(cfg.chatmode,20, "안함","모든대화실"));
        printf(" 15.  chat.xhost.co.kr 인터넷대화실 연결     :%s\n",rbit(cfg.chatmode,21, "안함","모든대화실"));
        printf(" 19.  SAY2 명령 사용 가능 아이디             (system/say2.id)\n");
		printf(" 20.  대화실 진입금지 ID 지정                (system/nochat.id)\n");
		printf(" 21.  대화실 금지 사유 (접속회원)            (frm/error/chatoff.log)\n");
		printf(" 22.  대화실 방장 가능 ID (/sys,진입가능)    (system/chatsys.id)\n");
		printf(" 23.  대화실 분류별 설정 (진입제한)          (system/chattype.dat)\n");
		n=numselect();
		xy(60,n+3);
		if (n<= 0)	break;
		if (n== 1)	cfg.chatwarn  =get_num(cfg.chatwarn ,6);
		if (n== 2)	cfg.chattype  =get_num(cfg.chattype ,2);
		if (n== 3)	cfg.mode=gtt_bit(cfg.mode,16);
		if (n== 4)	cfg.mode=gtt_bit(cfg.mode,23);
		if (n== 5)	cfg.chatmode=gtt_bit(cfg.chatmode,0);
		if (n== 6)	cfg.chatmode=gtt_bit(cfg.chatmode,13);
		if (n== 7)	cfg.chatmode=gtt_bit(cfg.chatmode,14);
		if (n== 8)	cfg.chatmode=gtt_bit(cfg.chatmode,15);
		if (n== 9)	cfg.chatmode=gtt_bit(cfg.chatmode,16);
		if (n==10)	cfg.chatst =get_num(cfg.chatst ,2);
		if (n==11)	cfg.sayline=get_num(cfg.sayline,3);
		if (n==12)	cfg.chatmode=gtt_bit(cfg.chatmode,18);
		if (n==13)	cfg.chatmode=gtt_bit(cfg.chatmode,19);
        if (n==14)  cfg.chatmode=gtt_bit(cfg.chatmode,20);
        if (n==15)  cfg.chatmode=gtt_bit(cfg.chatmode,21);
        if (n==19)  xedit2("system/say2.id");
		if (n==20)	xedit2("system/nochat.id");
		if (n==21)	xedit2("frm/error/chatoff.log");
		if (n==22)	xedit2("system/chatsys.id");
		if (n==23)	set_roomtype_limit();	  //분류별 제한설정
	}
}

job_screen()
{
	char buff[300];
	while (1) {
		cls();
		printf(" ### 화면 관련 환경설정\n");
		printf("  1.  PF시 등급/레벨 표시여부    :%s\n",rbit(cfg.mode,14,"표시", "불가"));
		printf("  3.  하단 광고 보여줌           :%s\n",rbit(cfg.mode,27,"보임", "무시"));
		printf("  4.  사용 유도 도움말(today mess)설정\n");
		printf("  5.  종료시 화면 출력           :%s\n",rbit(cfg.logomode,2,"보통","안함"));
		printf("  6.  시작시 화면 출력           :%s\n",rbit(cfg.logomode,1,"보통","안함"));
		printf("  7.  TOP 애니메이션 로고 화면   :%s\n",rbit(cfg.logomode,3,"안함","동작함"));
		printf("  8.  애니메이션 전체회원 지정   :%s\n",rbit(cfg.logomode,4,"개인별","강제동작"));
		printf("  9.  setlogo 사용가능 등급      :%d\n",cfg.no_logotype);
		printf(" 10.  US 관련 설정               \n");
		printf(" 11.  하단선택 명령어 읽기       :%s\n",rbit(cfg.logomode,5,"FRM파일","내장명령"));
		printf(" 12.  접속출력시 최대 접속수*모양:%d\n",cfg.limit_slog);
		printf(" 13.  설정 명령선택시 선택보이기 :%s\n",rbit(cfg.logomode,8,"둘다보임","선택보임"));
		printf(" 14.  [7m 반전 안함              :%s\n",rbit(cfg.logomode,9,"반전함","안함"));
		printf(" 15.  logodndn 기능(로고다운)    :%s\n",rbit(cfg.logomode,11,"사용거부","사용허가"));
		printf(" 90.  통합 화면 설정 모드        (.scr)             \n");

		n=numselect();
		xy(60,n+1);
		if (n<=0)  break;
		if (n==1)  cfg.mode 	  =gtt_bit(cfg.mode,14);
		if (n==2)  cfg.mode 	  =gtt_bit(cfg.mode,17);
		if (n==3)  cfg.mode 	  =gtt_bit(cfg.mode,27);
		if (n==4)  job_todaymess();
		if (n==5)  cfg.logomode   =gtt_bit(cfg.logomode,2);
		if (n==6)  cfg.logomode   =gtt_bit(cfg.logomode,1);
		if (n==7)  cfg.logomode   =gtt_bit(cfg.logomode,3);
		if (n==8)  cfg.logomode   =gtt_bit(cfg.logomode,4);
		if (n==9)  cfg.no_logotype=get_num(cfg.no_logotype,2);
		if (n==10) job_us();
		if (n==11) cfg.logomode   =gtt_bit(cfg.logomode,5);
		if (n==12) cfg.limit_slog =get_num(cfg.limit_slog,3);
		if (n==13) cfg.logomode   =gtt_bit(cfg.logomode,8);
		if (n==14) cfg.logomode   =gtt_bit(cfg.logomode,9);
		if (n==15) cfg.logomode   =gtt_bit(cfg.logomode,11);
		if (n==90) {sys_tem("bin/xfrm 1 '%s' '%s' '%s'",club_go.code,go.code,now_id);}
	}
}


job_todaymess()
{
	int  check_today;		// 오늘만 검사?
	int  check_todayon; 	// 출력여부검사?
	char buff[300];
	int i;
	while (1) {
		cls();
		printf(" ### 사용유도 도움말 설정\n");
		printf(" 1. 사용 유도 안내 메세지 help    :%s\n",rbit(cfg.logomode,6,"보여줌","안보임"));
		printf(" 2. 사용 유도 안내 ALARM 자동출력 :%s\n",rbit(cfg.logomode,7,"보통","자동동작"));
		printf(" 3. 사용 유도 확인 글쓰기 코드1:%s 2:%s 3:%s 4:%s 5:%s\n",cfg.check_wgo[0],cfg.check_wgo[1],cfg.check_wgo[2],cfg.check_wgo[3],cfg.check_wgo[4]);
		printf(" 출력여부: ");
		for (i=0;i<32;i++) {
			printf("%2d.%s ",i+31,rbit2(cfg.check_today  ,i,"O","x"));
			if (i==4||i==14) printf("  ");
			if (i==9||i==19) printf("\n           ");
		}
		printf("\n 일일갱신: ");
		for (i=0;i<32;i++) {
			printf("%2d.%s ",i+61,rbit2(cfg.check_todayon,i,"A","t"));
			if (i==4||i==14) printf("  ");
			if (i==9||i==19) printf("\n           ");
		}
		printf("\n");
		n=numselect();
		xy(60,n+1);
		if (n<=0) break;
		if (n==1) cfg.logomode	 =gtt_bit(cfg.logomode,6);
		if (n==2) cfg.logomode	 =gtt_bit(cfg.logomode,7);
		if (n==3) {for (i=0;i<5;i++) get_str(cfg.check_wgo[i],8);}
		if (n>=31&&n<=60) cfg.check_today  =gtt_bit(cfg.check_today  ,n-31);
		if (n>=61&&n<=90) cfg.check_todayon=gtt_bit(cfg.check_todayon,n-61);
	}
}


job_us()
{
	while (1) {
		cls();
		printf(" ### US 관련 환경설정\n");
		printf("  1.  메인  에서 US표시 여부     :%s\n",rbit(cfg.usermode,1, "가능", "불가"));
		printf("  2.  대화실에서 US표시 여부     :%s\n",rbit(cfg.usermode,2, "가능", "불가"));
		printf("  3.  동호회에서 US표시 여부     :%s\n",rbit(cfg.usermode,3, "가능", "불가"));
		printf("  4.  메세지전송 US표시 여부     :%s\n",rbit(cfg.usermode,4, "가능", "불가"));
		printf("  5.  US시 등급/레벨 표시여부    :%s\n",rbit(cfg.usermode,5, "표시", "불가"));
		printf("  6.  US시 간략하게 보여줌       :%s\n",rbit(cfg.usermode,6, "보통", "간략"));
		printf("  7.  US시 본명대신 별명보여줌   :%s\n",rbit(cfg.usermode,7, "별명", "본명"));
		printf("  8.  US시 IP주소대신 TTY보여줌  :%s\n",rbit(cfg.usermode,8, "IP ", "TTY"));
		printf("  9.  화면넘어갈때 Enter키 멈춤  :%s\n",rbit(cfg.usermode,9, "멈춤", "통과"));
		printf(" 10.  강제US표시방법지정 (0~39)  :%d\n",cfg.usdisp);
		printf(" 11.  강제US표시(메세지대상선택) :%d\n",cfg.usdisp2);
		printf(" 12.  일반 기록시 US 정보 정렬   :%s\n",rbit(cfg.usermode,11,"정렬","무시"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.usermode	=gtt_bit(cfg.usermode,1);
		if (n== 2)	cfg.usermode	=gtt_bit(cfg.usermode,2);
		if (n== 3)	cfg.usermode	=gtt_bit(cfg.usermode,3);
		if (n== 4)	cfg.usermode	=gtt_bit(cfg.usermode,4);
		if (n== 5)	cfg.usermode	=gtt_bit(cfg.usermode,5);
		if (n== 6)	cfg.usermode	=gtt_bit(cfg.usermode,6);
		if (n== 7)	cfg.usermode	=gtt_bit(cfg.usermode,7);
		if (n== 8)	cfg.usermode	=gtt_bit(cfg.usermode,8);
		if (n== 9)	cfg.usermode	=gtt_bit(cfg.usermode,9);
		if (n==10)	cfg.usdisp		=get_num(cfg.usdisp,2);
		if (n==11)	cfg.usdisp2 	=get_num(cfg.usdisp2,2);
		if (n==12)	cfg.usermode	=gtt_bit(cfg.usermode,11);
	}
}



job_system()
{
	while (1) {
		cls();
		printf("  ### 시스템 관련 설정\n");
		printf("  1.  호스트의 이름                      :%s\n",cfg.hostname   );
		printf("  2.  키 입력 대기 시간   (0~255분)      :%d(분)\n",cfg.waitkey);
		printf("  3.  H,C -> HELP 로 전환 여부           :%s\n",rbit(cfg.mode,12,"보통", "HELP"));
		printf("  4.  접속시 메세지포트실패TTY 삭제      :%s\n",rbit(cfg.mode,26, "처리", "무시"));
		printf("  5.  등급18이상 키대기 설정             :%s\n",rbit(cfg.mode,0, "제한", "무제한"));
		printf("  6.  시간제한없는 ID 지정               (system/nokey.id)\n");
		printf("  7.  단축명령키 지정 화일               (system/shortcmd.dat)\n" );
		printf("  8.  알람 시그날 처리                   :%s\n",rbit(cfg.mode,28, "동작", "중지"));
		printf("  9.  화면갱신시 알람시그날동작(뉴스..)  :%s\n",rbit(cfg.logomode,10, "동작", "중지"));
		printf(" 10.  알람 시그날 주기  (0~255)          :%d\n",cfg.alarm_time);
		printf(" 11.  접속시에 제한시간초과 접속자확인   :%s\n",rbit(cfg.loginmode,1,"확인","통과"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	get_str(cfg.hostname  ,20);
		if (n== 2)	cfg.waitkey   =(byte)get_num(cfg.waitkey   ,3 );
		if (n== 3)	cfg.mode	  =gtt_bit(cfg.mode,12);
		if (n== 4)	cfg.mode	  =gtt_bit(cfg.mode,26);
		if (n== 5)	cfg.mode	  =gtt_bit(cfg.mode,0 );
		if (n== 6)	xedit("system/nokey.id");
		if (n== 7)	xedit("system/shortcmd.dat");
		if (n== 8)	cfg.mode	  =gtt_bit(cfg.mode,28);
		if (n== 9)	cfg.logomode  =gtt_bit(cfg.logomode,10);
		if (n==10)	cfg.alarm_time=get_num(cfg.alarm_time,3);
		if (n==11)	cfg.loginmode =gtt_bit(cfg.loginmode,1);
	}
	del_space(cfg.sysop);
}


//시스템 관련 세팅 지정  (특정 명령 입력 마다 확인함)
job_limit()
{
	int n;
	char buff[200];
	FILE *fp;
	while (1) {
		cls();
		printf(" ###  시스템 금지 제한 설정 관련\n");
		printf("  1.  인터넷 전용 노드 설정 (네트워크설정)     (system/node_net.txt)\n");
		printf("  4.  외부 메뉴 사용 금지 사유                 (logo/_runoff.log)\n");
		printf("  5.  텔넷 메뉴 금지 사유                      (frm/error/teloff.log)\n");
		printf("  6.  임시 시삽 메세지 (접속회원)              (logo/_sysmess.dat)\n");
		printf("  7.  노드별,포트별 접속시 접속금지/허용설정    \n");
		printf("  8.  진입가능/제한 메뉴 지정                  (system/_nocode.dat) \n" );
		printf("  9.  진입가능/제한 화면 편집                   \n" );
		printf(" 10.  전체 진입 제한 등급 지정                 %d\n",cfg.limittype);
		printf(" 11.  전체 진입 제한 사유 지정                 (frm/limit/limit.log)\n");
		printf(" 12.  시간대별/요일별 무제한 회원등급          %d\n",cfg.freelogin);
		printf(" 13.  동시 접속 가능 등급                      %d\n",cfg.relog);
		printf(" 14.  동시 접속시 메세지 출력                  %s\n",rbit(cfg.mode,7,"함", "안함"));
		printf(" 15.  MARK 기능 사용가능                       %s\n",rbit(cfg.mode,5,"가능", "금지"));
		printf(" 16.  텔넷   진입 가능 등급                    %d\n",cfg.telnettype);
		printf(" 17.  특정 명령 동작 금지 지정                 (system/nocode.txt)\n");
		printf(" 18.  진입 최대 인원수 (frm/limit/full.log)    %d\n",cfg.limit_login);
		printf(" 19.  W쓰기 R읽기 D다운 C대화 S동호회 금지/제한(system/limit.id)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) xedit("system/node_net.txt");
		if (n== 4) xedit("logo/_runoff.log");
		if (n== 5) xedit("frm/error/teloff.log");
		if (n== 6) {
			printf(" 접속회원에게 보여줄 임시 메세지를 입력하세요.. \n>>");
			lineinput(buff,76);
			if (buff[0]==0 ) unlink("logo/_sysmess.dat");
			else {
				fp=fopen("logo/_sysmess.dat","w");
				fprintf(fp,"%s",buff);
				if (fp!=NULL) fclose (fp);
			}
		}
		if (n== 7) job_node();
		if (n== 8) set_no_code();
		if (n== 9) set_no_logo();
		if (n==10) cfg.limittype=get_num(cfg.limittype,2);
		if (n==11) xedit("frm/limit/limit.log");
		if (n==12) cfg.freelogin=get_num(cfg.freelogin,2);
		if (n==13) cfg.relog	=get_num(cfg.relog	  ,2);
		if (n==14) cfg.mode 	=gtt_bit(cfg.mode,7);
		if (n==15) cfg.mode 	=gtt_bit(cfg.mode,5);
		if (n==16) cfg.telnettype =get_num(cfg.telnettype,2);
		if (n==17) xedit("system/nocode.txt");
		if (n==18) cfg.limit_login=get_num(cfg.limit_login,4);
		if (n==19) set_limit_id();
	}
}


set_limit_id()
{
	FILE *fp;
	char buff[300],buff1[100],buff2[100],buff3[100];
	char filename[100],filename2[100];
	int n;
	while (1) {
		cls();
		printf(" ### W쓰기 R읽기 D다운 C대화 S동호회 금지ID\n");
		printf(" 1.  W쓰기 R읽기 D다운 C대화 S동호회 금지ID   (system/limit.id)\n");
		printf(" 2.  W쓰기 메세지 (frm/pf/notwrite.log)\n");
		printf(" 3.  R읽기 메세지 (frm/pf/notread.log)\n");
		printf(" 4.  D다운 메세지 (frm/pf/notdown.log)\n");
		printf(" 5.  C대화 메세지 (frm/pf/notchat.log)\n");
		printf(" 6.  S동호 메세지 (frm/pf/notclub.log)\n");
		printf(" 7.  일반게시판 글읽기가능 등급 :%d\n",cfg.limit_r);
		printf(" 8.  일반게시판 글쓰기가능 등급 :%d\n",cfg.limit_w);
		printf(" 9.  일반자료실 등록  가능 등급 :%d\n",cfg.limit_u);
		printf("10.  일반자료실 다운  가능 등급 :%d\n",cfg.limit_d);
		printf("11.  서버메뉴(M)  진입가능 등급 :%d\n",cfg.limit_m);
		printf("12.  업/다운/쓰기 금지 사유 (logo/_downoff.log)\n");
		printf("13.  읽기 금지         사유 (logo/_readoff.log)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
			 if (n==1) edit_limit_id_file();
		else if (n==2) xedit2("frm/pf/notwrite.log");
		else if (n==3) xedit2("frm/pf/notread.log");
		else if (n==4) xedit2("frm/pf/notdown.log");
		else if (n==5) xedit2("frm/pf/notchat.log");
		else if (n==6) xedit2("frm/pf/notclub.log");
		else if (n== 7)  cfg.limit_r=get_num(cfg.limit_r,2);
		else if (n== 8)  cfg.limit_w=get_num(cfg.limit_w,2);
		else if (n== 9)  cfg.limit_u=get_num(cfg.limit_u,2);
		else if (n==10)  cfg.limit_d=get_num(cfg.limit_d,2);
		else if (n==11)  cfg.limit_m=get_num(cfg.limit_m,2);
		else if (n==12) xedit2("logo/_downoff.log");
		else if (n==13) xedit2("logo/_readoff.log");
	}
}


//노드 관련 지정
job_node()
{
	int n,n2;
	char buff[100];
	n=n2=0;
	while (1) {
		cls();
		printf(" ###      노드별,포트별 접속시 접속금지/허용설정         \n");
		printf("  1.  인터넷전용 노드 리스트                        (system/node_net.txt)\n");
		printf("  2.  노드별 접속 시간 지정                         (system/nodetime.txt)\n");
		printf("  3.  노드별 접속 등급 지정                         (system/nodetype.txt)\n");
		printf("  4.  노드별 시삽 메세지 지정                       (system/nodemess.txt)\n");
		printf("  5.  노드별 사용 불가능 메뉴 지정                  (system/nodemenu.txt)\n");
		printf("  6.  노드별 특정 ID만 접속가능                     (system/nodeid.txt  )\n");
		printf("  7.  노드별 특정 호스트만 접속 가능                (system/nodehost.txt)\n");
		printf("  9.  접속제한 무제한 아이디 지정                   (system/nodelog.id  \n");
		printf(" 10.  접속금지 노드,호스트명 지정  (등급별쓰기금지) (system/nodety?.txt \n");
		printf(" 11.  접속금지 노드,호스트명 지정  (접속종료)       (system/nodelog1.txt\n");
		printf(" 12.  접속가능 노드,호스트명 지정                   (system/nodelog2.txt\n");
		printf(" 13.  지정 노드,호스트명 무조건 손님지정            (system/nodelog3.txt\n");
		printf(" 14.  쓰기금지 (손님접속)노드안내화면               (logo/_nodeno0.log) \n");
		printf(" 15.  접속금지 노드,호스트명 안내화면               (logo/_nodeno1.log) \n");
		printf(" 16.  요일별 무제한 사용 아이디                     (system/_day.id7)\n");
		printf(" 17.  요일별 사용가능 아이디 지정                   (system/_day.id?)\n");
		printf(" 18.  요일별 사용 불가능시 안내화면                 (logo/_dayid?.log)\n");
		printfL2();
		n=numselect();
		xy(60,n+1);
		if (n<=0) break;
		if (n==1) xedit("system/node_net.txt");
		if (n==2) xedit("system/nodetime.txt");
		if (n==3) xedit("system/nodetype.txt");
		if (n==4) xedit("system/nodemess.txt");
		if (n==5) xedit("system/nodemenu.txt");
		if (n==6) xedit("system/nodeid.txt");
		if (n==7) xedit("system/nodehost.txt");
		if (n==9) xedit("system/nodelog.id");
		if (n==10) {
			printf("\n제한등급? >>");
			n2=numinput();
			if (n2>=0&&n2<20) {
				sprintf(buff,"system/nodety%d.txt",n2);
				xedit(buff);
			}
		}
		if (n==11) xedit("system/nodelog1.txt");
		if (n==12) xedit("system/nodelog2.txt");
		if (n==13) xedit("system/nodelog3.txt");
		if (n==14) xedit("logo/_nodeno0.log");
		if (n==15) xedit("logo/_nodeno1.log");
		if (n==16) xedit("system/_day.id7");
		if (n==17||n==18) {
			printf("\n 요일별 번호(0일 1월 2화 3수 4목 5금 6토 >>");
			n2=numinput();
			if (n2>=0&&n2<6) {
				if (n==27) sprintf(buff,"system/_day.id%d",n2);
				else	   sprintf(buff,"logo/_dayid%d.log",n2);
				xedit(buff);
			}
		}
	}
}


// 동호회 관련 작업
job_club()
{
	char buff[80];
	char buff2[80];
	int num;
	while (1) {
		cls();
		printfL2();
		printf("          동호회 관련 작업  \n");
		printfL2();
		printf(" 1.  신규 동호회 만들기  \n");
		printf(" 2.  동호회 백업 만들기  \n");
		printf(" 3.  동호회 삭제 하기    \n");
		printf(" 4.  동호회 시삽 메뉴    \n");
		printf(" 5.  전체 동호회 현황(간단)\n");
		printf(" 6.  전체 동호회 현황(통계)\n");
		printf(" 7.  전체 동호회 현황(세부)\n");
		printf(" 9.  동호회 만들기 도움말\n");
		printfL2();
		num=numselect();
		if (num<=0) break;
		if (num==1) {
			printf("\n작업할 동호회 코드는? >>");
			lineinput(buff,8);if (strlen(buff)<2) break;
			if (find_club(buff)) {
				printf("\n %s (%s) 동호회는 이미 %s 디렉토리에 있습니다.\n",
						buff,club_go.name,club_go.dir);
				printf(" %s 디렉토리로 동호회를 만들까요? (Y/n)>>",club_go.dir);
				if (yesno2("",Yes)) club_make(club_go.dir);
				else {
					mess("동호회 작업을 취소합니다.");
				}

			}
			else {
				printf("\n *** %s 동호회를 만들까요?  ",buff);
				if (yesno2(" (Y/n)>>",Yes)) {
					printf(" 동호회 디렉토리는? >>");
					lineinput(buff2,60);printf("\n");
					if (strlen(buff2)>1) {
						club_make(buff2);
						mess("메뉴편집에서 동호회를 지정하세요.");
					}
				}
			}
		}
		if (num==2) {
			printf("\n백업할 동호회 코드는? >>");lineinput(buff,8);
			if (!find_club(buff)) {
				printf("\n %s 동호회를 찾을수 없습니다.\n",buff);
			}
			else {
				printf("\n %s (%s) (%s) \n",club_go.code,club_go.name,club_go.dir);
				if (yesno("동호회를 백업할까요? (백업:Y)",No)) {
					sys_tem2("tar cfvz %s.tgz %s &",club_go.code,club_go.dir);
				}
			}
		}
		if (num==3) {
			printf("\n삭제할 동호회 코드는? >>");lineinput(buff,8);printf("\n");
			if (strlen(buff)<1);
			else if (!find_club(buff)) {
				mess("%s 동호회를 찾을수 없습니다.",buff);
			}
			else {
				printf("\n %s (%s) (%s) \n",club_go.code,club_go.name,club_go.dir);
				if (yesno2("이 동호회의 모든 자료를 삭제할까요? (삭제:Y)",No)) {
					if (!yesno("한번 삭제하면 절대 복구할수 없습니다. 취소할까요? (취소:Y)",Yes)) {
						printf("\n");
						sprintf(buff2,"rm %s -i -R",club_go.dir);
						sys_tem(buff2);
					}
				}
			}
		}
		if (num==4) {
			printf("\n작업할 동호회 코드는? >>");lineinput(buff,8);
			if (strlen(buff)<1);
			else if (find_club(buff)) sys_tem("bin/csysop %s %s %s",club_go.code,"top",cfg.sysop);
			else mess("%s 동호회를 찾을수 없습니다. 메인 메뉴 확인",buff);
		}
		if (num==5) view_all_club(1);
		if (num==6) view_all_club(2);
		if (num==7) view_all_club(3);
		if (num==9) {view("doc/makeclub.txt");pressenter();}
	}
}



//메일관련
job_mail()
{
	int n,n2,n3;
	char buff[100];
	while (1) {
		cls();
		printf("          메일 관련 작업\n");
		printfL2();
		printf("  1. 기간 지난 메일 삭제                       \n");
		printf("  2. 일반메일 사용가능 등급                    :%d\n",cfg.mailwrite);
		printf("  3. E-MAIL 사용가능 등급                      :%d\n",cfg.emailwrite);
		printf("  4. E-MAIL 전용 아이디 사용함                 :%s\n",rbit(cfg.mailmode,1,"함","안함"));
		printf("  5. 메일바이너리사용                          :%s\n",rbit(cfg.mailmode,2,"사용안함","사용함"));
		printf("  6. 전체메일 발송 가능 등급                   :%d\n",cfg.wmalltype );
		printf("  7. 전체메일 사용 가능 회원 지정              (system/wmall.id)\n");
		printf("  8. 메일 자동 답장 ID/내용 지정               (system/manswer.id)\n");
		printf("  9. 메일 도착시 벨소리                        :%s\n",rbit(cfg.mailmode,6,"안냄","냄"));
		printf(" 10. 한글 Email ID 경고함.                     :%s\n",rbit(cfg.mailmode,2,"함","안함"));
		printf(" 11. 한글 Email ID 경고         안내화면       (frm/mail/emailh.log)\n");
		printf(" 12. Email 용 주소지정 (호스트명)              (system/hostaddr.txt)\n");
		printf(" 13. 메일사용 금지 사유 입력                   (frm/mail/notmail.log)\n");
		printf(" 14. E메일사용 금지 사유 입력                  (frm/mail/notemail.log)\n");
		printf(" 15. 메일읽기후 편지보관함 삭제 질문           :%s\n",rbit(cfg.mailmode,7,"함","안함"));
		printf(" 16. 메일읽기후 편지보관함 저장 질문           :%s\n",rbit(cfg.mailmode,8,"함","안함"));
		printf(" 17. 메일읽기후 편지보관함 보관 질문           :%s\n",rbit(cfg.mailmode,9,"함","안함"));
		printf(" 18. 메일읽기후 답장(RE) -> 원본편지추가       :%s\n",rbit(cfg.mailmode,10,"함","안함"));
		n=numselect();printf("\n");
		xy(60,n+2);
		if (n<=0) break;
		if (n==1) {
			xy(1,20);
			printf("  일반편지 메일삭제 기간은?  (5-999일) >>");
			n2=numinput();printf("\n");
			if (n2>4&&n2<1000) {
				printf("  보관편지 메일삭제 기간은?  (30-999일) >>");
				n3=numinput();printf("\n");
				if (n3>30&&n3<1000) {
				if (yesno2("기간경과 메일삭제를 시작할까요?(Y/n)>>",Yes))
					sys_tem2("bin/delmail _all_maildel %d %d &",n2,n3);
				}
			}
		}
		if (n== 2) cfg.mailwrite=get_num(cfg.mailwrite,1);
		if (n== 3) cfg.emailwrite=get_num(cfg.emailwrite,1);
		if (n== 4) cfg.mailmode=gtt_bit(cfg.mailmode,1);
		if (n== 5) cfg.mailmode=gtt_bit(cfg.mailmode,2);
		if (n== 6) cfg.wmalltype=get_num(cfg.wmalltype,2);
		if (n== 7) xedit("system/wmall.id");
		if (n== 8) xedit("system/manswer.id");
		if (n== 9) cfg.mailmode=gtt_bit(cfg.mailmode,6);
		if (n==10) cfg.mailmode=gtt_bit(cfg.mailmode,2);
		if (n==11) xedit("frm/mail/emailh.log");
		if (n==12) xedit("system/hostaddr.txt");
		if (n==14) xedit("frm/mail/notemail.log");
		if (n==15) cfg.mailmode=gtt_bit(cfg.mailmode,7);
		if (n==16) cfg.mailmode=gtt_bit(cfg.mailmode,8);
		if (n==17) cfg.mailmode=gtt_bit(cfg.mailmode,9);
		if (n==18) cfg.mailmode=gtt_bit(cfg.mailmode,10);
	}
}


job_backmode()
{
	int n;
	while (1) {
		cls();
		print_(0,"시스템 백업관련 설정");
		printf("  1.  메뉴편집시 OLD,BAK 저장        :%s\n",rbit(cfg.backmode,1, "저장", "무시"));
		printf("  2.  지난달 경과시 REC 자동백업     :%s\n",rbit(cfg.backmode,2, "백업", "무시"));
		printf("  3.  화면등록시    화면자동백업     :%s\n",rbit(cfg.backmode,3, "백업", "무시"));
		printf("  4.  시삽메뉴진입시    자동백업     :%s\n",rbit(cfg.backmode,4, "백업", "무시"));
		printf("  5.  동호회  진입시    자동백업     :%s\n",rbit(cfg.backmode,5, "백업", "무시"));
		printf("  9.  백업\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.backmode  =gtt_bit(cfg.backmode,1);
		if (n== 2)	cfg.backmode  =gtt_bit(cfg.backmode,2);
		if (n== 3)	cfg.backmode  =gtt_bit(cfg.backmode,3);
		if (n== 4)	cfg.backmode  =gtt_bit(cfg.backmode,4);
		if (n== 5)	cfg.backmode  =gtt_bit(cfg.backmode,5);
		if (n== 9)	job_backup();
	}
}



set_no_code()
{
	char filename[200];
	char buff[100];
	FILE *fp,*fp2;
	int found=No,i;
	struct CCODE {
		char mode;
		char code[9];
	} ccode;

	sprintf(filename,"%s/system/_nocode.dat",club_go.dir);

	key=Yes;
	while (key) {
		cls();
		printf("           진입제한 가기코드 목록        가기코드(제한모드)\n");
		if ((fp=fopen(filename,"r+"))!=NULL) {
			i=0;
			while (fread(&ccode,sizeof(ccode),1,fp)) {
				printf(" %-8s(%d) ",ccode.code,ccode.mode);
				if (i++%6==5) printf("\n");
			}
			if (fp!=NULL) fclose(fp);
			if (i%6!=0) printf("\n");
		}
		printfL2();
		while (key) {
			printf("  추가:A <코드> <모드>  삭제:D <코드> 목록:L 종결:Q  선택>>");
			lineinput(buff,12);printf("\n");
			check_cmd(buff);
			if (is_char("QqPpXx",buff[0])) key=No;
			else if (is_char("Ll",buff[0])) break;
			else if (is_char("AaIi",buff[0])) {
				if (mmc[1][0]==0||atoi(mmc[2])==0) {
					printf("\n 제한 사용법 : A <가기코드> <모드>    예) A hello 1\n");
					printf("   가기코드:제한할 코드  모드:진입제한시 메세지 출력 \n");
				}
				else {
					strcpy(ccode.code,mmc[1]);
					i=atoi(mmc[2]);if (i>9) i=9;
					ccode.mode=i;
					if ((fp=fopen(filename,"a+"))==NULL) fp=fopen(filename,"w+");
					if (fp!=NULL) {
						fwrite(&ccode,sizeof(ccode),1,fp);
						printf(" %-8s(%d)  추가됨\n",ccode.code,ccode.mode);
						fclose(fp);
					}
				}
			}
			else if (is_char("Dd",buff[0])) {
				if (mmc[1][0]==0) {
					printf("\n 삭제 사용법 : D <가기코드>   예) D hello\n");
				}
				else {
					if ((fp=fopen(filename,"r+"))!=NULL) {
						if ((fp2=fopen(TMP_FILE,"w+"))!=NULL) {
							while (fread(&ccode,sizeof(ccode),1,fp)) {
								if (!is_same(ccode.code,mmc[1])&&ccode.mode>0&&ccode.code[0]!=0)
									fwrite(&ccode,sizeof(ccode),1,fp2);
							}
							fclose(fp);
							fclose(fp2);
							file_copy(TMP_FILE,filename);
							unlink(TMP_FILE);
						}
					}
				}
			}
		}
	}

}

set_no_logo()
{
	int n;
	char buff[100];
	char filename[200];
	while (1) {
		cls();
		printf("          진입 제한 화면 메세지 편집\n");
		printfL2();
		printf("  1. 진입제한 메세지 1         (logo/_nocode1.log)\n");
		printf("  2. 진입제한 메세지 2         (logo/_nocode2.log)\n");
		printf("  3. 진입제한 메세지 3         (logo/_nocode3.log)\n");
		printf("  4. 진입제한 메세지 4         (logo/_nocode4.log)\n");
		printf("  5. 진입제한 메세지 5         (logo/_nocode5.log)\n");
		printf("  6. 진입제한 메세지 6         (logo/_nocode6.log)\n");
		printf("  7. 진입제한 메세지 7         (logo/_nocode7.log)\n");
		printf("  8. 진입제한 메세지 8         (logo/_nocode8.log)\n");
		printf("  9. 진입제한 메세지 9         (logo/_nocode9.log)\n");
		printfL2();
		n=numselect();
		sprintf(filename,"%s/logo/_nocode%d.log",club_go.dir,n);
		printf("\n");
		if (n<= 0) break;
		if (n== 1) xedit(filename);
		if (n== 2) xedit(filename);
		if (n== 3) xedit(filename);
		if (n== 4) xedit(filename);
		if (n== 5) xedit(filename);
		if (n== 6) xedit(filename);
		if (n== 7) xedit(filename);
		if (n== 8) xedit(filename);
		if (n== 9) xedit(filename);
	}
}


job_money()
{
	FILE *fp;
	int i,n,num,n2;
	char buff[100];

	while (1) {
		cls();
		printf("  ###  사이버머니 관련 작업  ###\n");
		printf("  1. 회원 금액 상한   (원)                      :%d\n",cfg.cashmax    );
		printf("  2. 무제한 금액 사용 등급                      :%d\n",cfg.cashfree   );
		printf("  3. 읽기시 사이버머니 동작                     :%s\n",rbit(cfg.cashmode,0, "함", "안함"));
		printf("  4. 다운시 사이버머니 동작                     :%s\n",rbit(cfg.cashmode,1, "함", "안함"));
		printf("  5. 쓰기시 사이버머니 동작                     :%s\n",rbit(cfg.cashmode,2, "함", "안함"));
		printf("  6. 등록시 사이버머니 동작                     :%s\n",rbit(cfg.cashmode,3, "함", "안함"));
		printf("  7. 게시판삭제사이버머니동작                   :%s\n",rbit(cfg.cashmode,4, "함", "안함"));
		printf("  8. 자료실삭제사이버머니동작                   :%s\n",rbit(cfg.cashmode,5, "함", "안함"));
		printf("  9. 1회 접속 최대 이익 CASH (0:무제한)         :%d\n",cfg.cashmax1);
		printf(" 10. 1일 접속 최대 이익 CASH (0:무제한)         :%d\n",cfg.cashmax2);
		printf(" 11. 사이버머니 0이하 회원 다운                 :%s\n",rbit(cfg.cashmode,6, "금지", "허용"));
		printf(" 12. 전체회원 사이버머니 일괄지정               \n");
		printfL2();
		n=numselect();
		xy(60,n);
		if (n<=0)  break;
		if (n==1)  cfg.cashmax	 =get_num(cfg.cashmax	,10);
		if (n==2)  cfg.cashfree  =get_num(cfg.cashfree	, 2);
		if (n==3)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 0);
		if (n==4)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 1);
		if (n==5)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 2);
		if (n==6)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 3);
		if (n==7)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 4);
		if (n==8) cfg.cashmode	 =gtt_bit(cfg.cashmode	, 5);
		if (n==9) cfg.cashmax1	 =get_num(cfg.cashmax1 ,10);
		if (n==10) cfg.cashmax2  =get_num(cfg.cashmax2 ,10);
		if (n==11) cfg.cashmode  =gtt_bit(cfg.cashmode	, 6);
		if (n==12) {
			printf("전체회원 사이버캐시 (단위 CC) :");
			lineinput(buff,9);
			n2=atoi(buff);
			if (buff[0]==0||n2<0) continue;
			xy(1,21);
			printf("\n 지금부터 전체회원의 사이버캐시를 세팅합니다. 갈무리 하세요 \n\n");
			printf("    전체회원의 사이버캐시를 %dCC 로 세팅할까요? (Y/n)>>",n2);
			if (yesno("",Yes)==No) continue;
			fp=fopen(ID_FILE,"r+");
			i=0;
			printf("\n");
			while (1) {
				fseek(fp,i*sizeof_pf,SEEK_SET);
				if (!fread(&pf,sizeof_pf,1,fp)) break;
				printf("%-8s (%-8s)  %9d ->",pf.id,pf.name,pf.cash);
				pf.cash=n2;
				printf("%9d\n",pf.cash);
				fseek(fp,i*sizeof_pf,SEEK_SET);
				i++;
			}
			if (fp!=NULL) fclose(fp);
			pressenter();
		}
	}
}


job_ip()
{
	FILE *fp;
	int i,n,num,n2;
	char buff[100];

	while (1) {
		cls();
		printf("  ###  IP 관련 설정  ###\n");
		printf("  1. IP 자료 기록                  :%s\n",rbit(cfg.ipmode,0,"함", "안함"));
		printfL2();
		n=numselect();
		xy(60,n);
		if (n<= 0)	break;
		if (n== 1) cfg.ipmode	 =gtt_bit(cfg.ipmode  , 0);
	}
}

save_guestnot(PF pf2,int mode)
{
	FILE *fp;
	int i;
	char filename[200];
	char buff[200];
	strcpy(filename,GUESTNOT_FILE);
	if (mode==1) {
		printf("\n 등록거부등록 사유 (취소:Q)>>");
		lineinput(buff,60);check_han(buff);printf("\n");
		if (is_quit(buff)) return No;
		strcpy(pf.memo2,buff);
		mess("거부처리가 지정 되었습니다.\n");
	}
	fp=fopen(filename,"r+");   //거부자아이디에 추가한다.
	if (fp==NULL) fp=fopen(filename,"w+");   //기존아이디에 추가한다.
	if (fp==NULL) {
		mess("%s 화일을 열수 없습니다.",filename);
		return No;
	}
	fseekend(fp);
	i=ftell(fp)/sizeof_pf;
	if (i>=0) {
		pf2.num=i;
		fseek(fp,i*sizeof_pf,SEEK_SET);
//		  fwrite(&pf2,sizeof_pf,1,fp);
		gwrite(203,pf2,fp);
	}
	fclose (fp);
	return No;
}



notfree()
{
	FILE *fp;
	FCFG fcfg;
	int i,n,num,n2;
	while (1) {
		cls();
		printf("  ###  등록거부 회원 추가,해제   ###\n");
		printf("  1. 등록거부회원 추가        (신규지정)\n");
		printf("  2. 기존회원 -> 등록거부회원 (거부지정)\n");
		printf("  3. 기존회원 <- 등록거부회원 (거부해제)\n");
		printf("  4. 거부회원 삭제            (거부취소)\n");
		printf("  5. 거부회원 전체 삭제       (화일삭제)\n");
		printf("  6. 거부회원 검색            (거부취소)\n");
		n=numselect();
		xy(60,n);
		if (n<= 0)	break;
	}
}



job_updown()
{
	int n,i;
	char buff[100];
	while (1) {
		cls();
		print_(0,"업,다운 설정관련");
		printf("  1. 업  ->다운용량 보상                          :%3.1f 배\n",(float)cfg.urate/10 );
		printf("  2. 다운->다운용량 감소                          :%3.1f 배\n",(float)cfg.drate/10 );
		printf("  3. 무제한 다운 등급                             :%d\n",cfg.downfree   );
		printf("  4. 멀티다운 가능 등급                           :%d\n",cfg.downmult  );
		printf("  5. 다운량조정 시기 (DN후면 취소시 감소안됨)     :%s\n",rbit(cfg.downmode,0,"DN전","DN후"));
		printf("  6. 업후 자동압축                                :%s\n",rbit(cfg.downmode,10," 함 ","안함"));
		printf("  7. 자료 등록 화일명자동입력                     :%s\n",rbit(cfg.downmode,1,"자동","수동"));
		printf("  8. 자료 등록시 일일 사용시간 보상 (2배 증가)    :%s\n",rbit(cfg.downmode,2," 함 ","안함"));
		printf("  9. 화일없는자료 -> 화일명빈칸처리               :%s\n",rbit(cfg.downmode,11,"보통","빈칸"));
		printf(" 10. UPS 명령시 확인안함                          :%s\n",rbit(cfg.downmode,5,"확인","안함"));
		printf(" 11. 파일다운준비시 임시영역에 파일복사 방법      :%s\n",rbit(cfg.downmode,15,"복사","링크"));
		printf(" 12. 업다운시 레벨,용량 증가메세지                :%s\n",rbit(cfg.downmode,17,"출력","무시"));
		printf(" 13. 업시 하드잔량(50MB) 확인                     :%s\n",rbit(cfg.downmode,20,"무시","확인"));
		printf(" 14. 다운시 하드잔량(50MB) 확인                   :%s\n",rbit(cfg.downmode,21,"무시","확인"));
		printf(" 15. 동시 업   등록 수 제한  (0:무제한)           :%d\n",cfg.up_limit );
		printf(" 16. 동시 다운 전송 수 제한  (0:무제한)           :%d\n",cfg.dn_limit );
		printf(" 17. 멀티 다운 수      제한  (0:무제한)           :%d\n",cfg.dn_mult_limit );
		printf(" 18. 자료등록시 크기별 레벨 보상 (1점당 Byte)     :%d\n",cfg.level_size);
		printf(" 22. 다운 관련 메세지/금지 설정  \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.urate=get_num(cfg.urate,3);
		if (n== 2)	cfg.drate=get_num(cfg.drate,3);
		if (n== 3)	cfg.downfree   =get_num(cfg.downfree,2);
		if (n== 4)	cfg.downmult   =get_num(cfg.downmult,2);
		if (n== 5)	cfg.downmode   =gtt_bit(cfg.downmode,0);
		if (n== 6)	cfg.downmode   =gtt_bit(cfg.downmode,10);
		if (n== 7)	cfg.downmode   =gtt_bit(cfg.downmode,1);
		if (n== 8)	cfg.downmode   =gtt_bit(cfg.downmode,2);
		if (n== 9)	cfg.downmode   =gtt_bit(cfg.downmode,11);
		if (n==10)	cfg.downmode   =gtt_bit(cfg.downmode,5);
		if (n==11)	cfg.downmode   =gtt_bit(cfg.downmode,15);
		if (n==12)	cfg.downmode=gtt_bit(cfg.downmode,17);
		if (n==13)	cfg.downmode=gtt_bit(cfg.downmode,20);
		if (n==14)	cfg.downmode=gtt_bit(cfg.downmode,21);
		if (n==15)	cfg.up_limit   =get_num(cfg.up_limit,3);
		if (n==16)	cfg.dn_limit   =get_num(cfg.dn_limit,3);
		if (n==17)	cfg.dn_mult_limit=get_num(cfg.dn_mult_limit,3);
		if (n==18)	cfg.level_size =get_num(cfg.level_size,8);
		if (n==18)	down_after();
		if (n==22)	job_updown2();
	}
}


job_updown2()
{
	int n,i;
	char buff[100];
	while (1) {
		cls();
		print_(0,"다운 관련 메세지/금지 설정");
		printf("  1. 다운종료/중단시 작업/화면메세지 처리          \n");
		printf("  2. 자료 압축화일 확장자지정 (system/achive.txt)  \n");
		printf("  3. 전체 자료실 내용 설정    (system/zbtotal.dat) \n");
		printf("  4. 동시 다운 금지 포트 설정 (system/_downpt.txt) \n");
		printf("  5. 업다운시 주의사항 / 참고 / 안내 화면 편집     \n");
		printf("  6. 사용자영역 업/다운 관련 설정                  \n");
		printf("  7. 업 다운 파라메타                              \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) down_after();
		if (n== 2) xedit("system/achive.txt");
		if (n== 3) xedit("system/zbtotal.dat");
		if (n== 4) xedit("system/_downpt.txt");
		if (n== 5) updown_note();
		if (n== 6) updown_user();
		if (n== 7) job_para();
	}
}


down_after()
{
	int n,i1,i2;
	while (1) {
		cls();
		printf("  ### 다운종료/중단후 처리방법/화면설정\n");
		printf("  1. 자료 다운후 Enter 확인함         :%s\n",rbit(cfg.downmode,12,"확인", "무시"));
		printf("  2. 자료 다운후 사이버머니 결과출력  :%s\n",rbit(cfg.downmode,13,"무시", "출력"));
		printf("  3. 자료 다운후 다운량변화 결과출력  :%s\n",rbit(cfg.downmode,14,"무시", "출력"));
		printf("  4. 다운 취소/중단시 처리            :%s\n",rbit(cfg.downmode,16,"중단처리", "무시"));
		printf("  5. 다운중단시 다운용량 1개다운시    :%s\n",rbit(cfg.downmode,9,"줄임", "취소"));
		printf("  6. 다운중단시 다운용량 멀티다운시   :%s\n",rbit(cfg.downmode,10,"줄임", "취소"));
		printf("  7. 업다운시 주의사항 / 참고 / 안내 화면 편집     \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.downmode   =gtt_bit(cfg.downmode,12);
		if (n== 2)	cfg.downmode   =gtt_bit(cfg.downmode,13);
		if (n== 3)	cfg.downmode   =gtt_bit(cfg.downmode,14);
		if (n== 4)	cfg.downmode   =gtt_bit(cfg.downmode,16);
		if (n== 5)	cfg.downmode   =gtt_bit(cfg.downmode,9);
		if (n== 6)	cfg.downmode   =gtt_bit(cfg.downmode,10);
		if (n== 7)	updown_note();
	}
}


updown_note()
{
	int n;
	while (1) {
		cls();
		printf("  ### 업,다운 관련 화면 편집\n");
		printf("  1. 동시 다운 금지 포트 사유     (logo/_downpt.log)  \n");
		printf("  2. 업시   주의사항(전체 업)     (logo/_up.scr)      \n");
		printf("  3. 업시   질문사항(전체 업)     (logo/_up.qsh)      \n");
		printf("  4. 다운시 주의사항(전체다운)    (logo/_dn.scr)      \n");
		printf("  5. 업후   주의사항(전체 업)     (logo/_up2.scr)     \n");
		printf("  6. 다운후 주의사항(전체 업)     (logo/_dn2.scr)     \n");
		printf(" 11. 동시 업   등록 수 제한 메세지(frm/editor/up_limit.log)\n");
		printf(" 12. 동시 다운 전송 수 제한 메세지(frm/editor/dn_limit.log)\n");
		printf(" 13. 멀티 다운 수      제한 메세지(frm/down/dn_mult.log)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	xedit("logo/_downpt.log");
		if (n== 2)	xedit("logo/_up.scr");
		if (n== 3)	xedit("logo/_up.qsh");
		if (n== 4)	xedit("logo/_dn.scr");
		if (n== 5)	xedit("logo/_up2.scr");
		if (n== 6)	xedit("logo/_dn2.scr");
		if (n==11)	xedit("frm/editor/up_limit.log");
		if (n==12)	xedit("frm/editor/dn_limit.log");
		if (n==13)	xedit("frm/down/dn_mult.log");
	}
}


updown_user()
{
	int n;
	while (1) {
		cls();
		printf("  ### 사용자 영역 관련 설정\n");
		printf("  1. USERROOM에서 화일UP   용량보상     :%s\n",rbit(cfg.downmode,3,"안함","함"));
		printf("  2. USERROOM에서 화일UP후 화일삭제     :%s\n",rbit(cfg.downmode,4,"안함","함"));
		xy(1,20);
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.downmode=gtt_bit(cfg.downmode,3);
		if (n== 2)	cfg.downmode=gtt_bit(cfg.downmode,4);
	}
}



job_para()
{
	int n,i;
	char buff[100];
	while (1) {
		del_esc(cfg.jpgsz);del_esc(cfg.jpgstr);
		cls();
		print_(0,"업,다운 파라메타 수정");
		printf(" 1. 다운(전송SZ) 명령 :%s\n",cfg.sz);
		printf(" 2. 업  (등록RZ) 명령 :%s\n",cfg.rz);
		printf(" 3. KERMIT다운   명령 :%s\n",cfg.kermitsz);
		printf(" 4. KETMIT업     명령 :%s\n",cfg.kermitrz);
		printf(" 5. 화상전송 (sz)명령 :%s\n",cfg.jpgsz);
		printf(" 6. 화상전송안시문자열:%s\n",cfg.jpgstr);       //        printf("7\n8[5;5H");
		printf(" 7. 자료 업/다운시 전송결과 :%s\n",rbit(cfg.downmode,22,"처리","무시"));
		printf("99. 기본 설정으로 변경하기 (DEFAULT)\n");
		printf("    (ESC문자표현 \\027 형식으로  예\\024\\n)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	{xy(30,n+1);get_str(cfg.sz,80);}
		if (n== 2)	{xy(30,n+1);get_str(cfg.rz,80);}
		if (n== 3)	{xy(30,n+1);get_str(cfg.kermitsz,80);}
		if (n== 4)	{xy(30,n+1);get_str(cfg.kermitrz,80);}
		if (n== 5)	{xy(30,n+1);get_str(cfg.jpgsz,80);}
		if (n== 6)	{xy(30,n+1);get_str(cfg.jpgstr,80);}
		if (n== 7)	cfg.downmode=gtt_bit(cfg.downmode,22);
		if (n==99)	{
			strcpy(cfg.sz,"sz");
			strcpy(cfg.rz,"rz");
			strcpy(cfg.kermitsz,"kermit -s");
			strcpy(cfg.kermitrz,"kermit -r");
			strcpy(cfg.jpgsz,"sz");
			strcpy(cfg.jpgstr,"\\024\\n");
		}
	}
}

job_zbtotal()
{
	int n,i;
	char buff[100];
	while (1) {
		cls();
		print_(0,"전체자료실 설저");
		printf(" 1. 전체자료실 작업상황 출력         :%s\n",rbit(cfg.zmode,1,"안함","출력"));
		printf(" 2. 전체게시판만 작업                :%s\n",rbit(cfg.zmode,2,"보통","한정"));
		printf(" 3. 전체게시판만 작업                :%s\n",rbit(cfg.zmode,3,"보통","한정"));
		printf(" 4. 변동사항 검사여부                :%s\n",rbit(cfg.zmode,4,"함","안함"));
		printf(" 5. 전체자료실진입시 1시간 변동확인  :%s\n",rbit(cfg.zmode,21,"안함","확인"));
		printf(" 6. 전체자료실진입시 6시간 변동확인  :%s\n",rbit(cfg.zmode,22,"안함","확인"));
		printf(" 7. 시작시           1시간 변동확인  :%s\n",rbit(cfg.zmode,23,"안함","확인"));
		printf(" 8. 시작시           6시간 변동확인  :%s\n",rbit(cfg.zmode,24,"안함","확인"));
		printf(" 9. 쓰기/다운시      30분  변동확인  :%s\n",rbit(cfg.zmode,25,"안함","확인"));
		printf("10. 전체 자료실 내용 설정    (system/zbtotal.dat) \n");
		printf("내용줄임모드 11.(%s) ",rbit2(cfg.zmode,11,"o"," "));
		printf("12.(%s) ",rbit2(cfg.zmode,12,"o"," "));
		printf("13.(%s) ",rbit2(cfg.zmode,13,"o"," "));
		printf("14.(%s) ",rbit2(cfg.zmode,14,"o"," "));
		printf("15.(%s) ",rbit2(cfg.zmode,15,"o"," "));
		printf("16.(%s) ",rbit2(cfg.zmode,16,"o"," "));
		printf("17.(%s)\n",rbit2(cfg.zmode,17,"o"," "));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.zmode=gtt_bit(cfg.zmode,1);
		if (n== 2)	cfg.zmode=gtt_bit(cfg.zmode,2);
		if (n== 3)	cfg.zmode=gtt_bit(cfg.zmode,3);
		if (n== 4)	cfg.zmode=gtt_bit(cfg.zmode,4);
		if (n== 5)	cfg.zmode=gtt_bit(cfg.zmode,21);
		if (n== 6)	cfg.zmode=gtt_bit(cfg.zmode,22);
		if (n== 7)	cfg.zmode=gtt_bit(cfg.zmode,23);
		if (n== 8)	cfg.zmode=gtt_bit(cfg.zmode,24);
		if (n== 9)	cfg.zmode=gtt_bit(cfg.zmode,25);
		if (n==10)	xedit("system/zbtotal.dat");
		if (n==11)	cfg.zmode=gtt_bit(cfg.zmode,11);
		if (n==12)	cfg.zmode=gtt_bit(cfg.zmode,12);
		if (n==13)	cfg.zmode=gtt_bit(cfg.zmode,13);
		if (n==14)	cfg.zmode=gtt_bit(cfg.zmode,14);
		if (n==15)	cfg.zmode=gtt_bit(cfg.zmode,15);
		if (n==16)	cfg.zmode=gtt_bit(cfg.zmode,16);
		if (n==17)	cfg.zmode=gtt_bit(cfg.zmode,17);
	}
}

//투표장
job_vote()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"투표 관련 작업");
		printf("  1. 투표 참여 가능 등급              :%d\n",cfg.vote_type);
		printf("  2. 투표 개설 가능 등급              :%d\n",cfg.vote_opentype);
		printf(" 10. 동호회 투표자 허용여부           :%s\n",rbit(cfg.vote,1,"허용","금지"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.vote_type	=get_num(cfg.vote_type	  ,2);
		if (n== 2) cfg.vote_opentype=get_num(cfg.vote_opentype,2);
		if (n==10) cfg.vote 		=gtt_bit(cfg.vote		 ,10);
	}
}



job_setbbs()
{
	int n,i;
	char buff[300];
	while (1) {
		cls();
		print_(0,"게시판,편집기 관련설정");
		printf(" 1.  BDN(게시판다운) 기능 가능 아이디   (system/bdn_id.dat)\n");
		printf(" 2.  게시물->bbs_all 자료실->pds_all    :%s\n",rbit(cfg.mode,24,"자동저장","무시"));
		printf(" 3.  게시판 두자리번호선택시 -> 자동지정:%s\n",rbit(cfg.mode,10,"자동지정","무시"));
		printf(" 4.  게시판 편집시 다운/조회/날짜 수정  :%s\n",rbit(cfg.bbsmode,3,"수정불가","수정가능"));
		printf(" 5.  게시판 편집시 위치,내용크기 수정   :%s\n",rbit(cfg.bbsmode,4,"수정불가","수정가능"));
		printf(" 6.  쓰기시 개인사인 첨부               :%s\n",rbit(cfg.mode,9,"허용","금지"));
		printf(" 7.  자료실 크기 0인 자료 안보이기      :%s\n",rbit(cfg.bbsmode,5,"자료실별설정","전체보임"));
		printf(" 8.  편집기2 -> 내장편집기 지정         :%s\n",rbit(cfg.bbsmode,6,"외부편집기","내장편집기"));
		printf(" 9.  W쓰기 R읽기 D다운 C대화 S동호회 금지/제한(system/limit.id)\n");
		printf("10.  자동추천/자동찬성 관련 설정        \n");
		n=numselect();xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) xedit("system/bdn_id.dat");
		if (n== 2) cfg.mode=gtt_bit(cfg.mode,24);
		if (n== 3) cfg.mode=gtt_bit(cfg.mode,10);
		if (n== 4) cfg.bbsmode=gtt_bit(cfg.bbsmode,3);
		if (n== 5) cfg.bbsmode=gtt_bit(cfg.bbsmode,4);
		if (n== 6) cfg.mode=gtt_bit(cfg.mode,9);
		if (n== 7) cfg.bbsmode=gtt_bit(cfg.bbsmode,5);
		if (n== 8) cfg.bbsmode=gtt_bit(cfg.bbsmode,6);
		if (n== 9) set_limit_id();
		if (n==10) job_good();
	}
}

job_good()
{
	char buff[300];
	while (1) {
		cls();
		print_(1,"게시물/자료실 추천 설정");
		printf("  1. 자동추천/찬성기능 동작모드 :%s\n",rbit(cfg.bbsmode,0,"동작","중지"));
		printf("  2. 자동추천/찬성기능 색상모드 :%s\n",rbit(cfg.bbsmode,1,"동작","중지"));
		printf("  3. GD(추천명령)별 추천순서    (%-10s) \n",cfg.gdstr);
		printf(" 11. 추천표시별 색상 지정  %s\n",GOOL_FILE);
		printf(" 12. 게시판읽기->자동 추천 횟수,표시문자,색깔 지정 \n");
		printf(" 13. 자료실다운->자동 추천 횟수,표시문자,색깔 지정 \n");
		printf(" 14. 게시판찬성->자동 추천 횟수,표시문자,색깔 지정 \n");
		printf(" 15. 자료실찬성->자동 추천 횟수,표시문자,색깔 지정 \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1) cfg.bbsmode=gtt_bit(cfg.bbsmode,0);
		if (n== 2) cfg.bbsmode=gtt_bit(cfg.bbsmode,1);
		if (n== 3) get_str(cfg.gdstr,10);
		if (n==11) cfg_gool();
		if (n>=12&&n<=15) cfg_good(n-12);
	}
}




cfg_good(int mode)
{
	FILE *fp;
	char buff[90];
	int n,n2,i,i2;

	if ((fp=fopen(GOOD_FILE,"r+"))!=NULL) {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				fread(&agood[i][i2],sizeof_agood,1,fp);
			}
		}
		fclose(fp);
	}
	while(1) {
		cls();
		i=mode;
		if (i==0) print_(0,"게시판읽기 자동 추천 설정 %s",GOOD_FILE);
		if (i==1) print_(0,"자료실다운 자동 추천 설정 %s",GOOD_FILE);
		if (i==2) print_(0,"게시판찬성 자동 추천 설정 %s",GOOD_FILE);
		if (i==3) print_(0,"자료실찬성 자동 추천 설정 %s",GOOD_FILE);
		for(i2=0;i2<10;i2++) {
			printf(" %2d:",i2+1);
			bzero(buff,10);buff[0]=agood[i][i2].mark;
			printf(" 표시문자 %2s ",buff);
			printf(" 표시설명 %-8s ",acolor[i2].name);
			printf(" 자동추천수 %4d ",agood[i][i2].count);
			printf(" 글자색:%2d  배경색:%2d [s%s[ 현재색 ][u\n",
			acolor[i2].color_char,acolor[i2].color_back,get_gool(agood[i][i2].mark));
		}
		printf(" 선택(Q,W)>>");
		lineinput(buff,10);check_han(buff);printf("\n");
		i2=atoi(buff)-1;
		if (is_quit(buff)) break;
		if (is_same(buff,"q")) break;
		if (is_same(buff,"w")) break;
		if (is_same(buff,"x")) break;
		if (is_same(buff,"xx")) break;
		if (buff[0]==0) continue;
		if (i2<0||i2>10) continue;
		printf(" %2d:",i2+1);
		bzero(buff,10);buff[0]=agood[i][i2].mark;
		printf(" 표시문자   :%2s 변경표시문자 ",buff);
		lineinput(buff,2);printf("\n");
		if (buff[0]!=0) agood[i][i2].mark=buff[0];
		printf(" 자동추천수 :%4d 변경추천횟수 ",agood[i][i2].count);
		agood[i][i2].count=get_num(agood[i][i2].count,999999);printf("\n");
		bzero(buff,10);buff[0]=agood[i][i2].mark;
	}

	if ((fp=fopen(GOOD_FILE,"w+"))!=NULL) {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				fwrite(&agood[i][i2],sizeof_agood,1,fp);
			}
		}
		fclose(fp);
	}

}


cfg_gool()
{
	char buff[90];
	int n,n2,i,i2;
	FILE *fp;

	//해당화일정보읽기
	if ((fp=fopen(GOOL_FILE,"r+"))!=NULL) {
		for(i=0;i<10;i++) {
			fread(&acolor[i],sizeof_acolor,1,fp);
		}
		fclose(fp);
	}

	while(1) {
		cls();
		print_(1,"추천표시별 색상 지정 %s",GOOL_FILE);
		for(i2=0;i2<10;i2++) {
			printf(" %2d:",i2+1);
			bzero(buff,10);buff[0]=acolor[i2].mark;
			printf(" 표시문자 %2s ",buff);
			printf(" 표시설명 %-8s ",acolor[i2].name);
			printf(" 글자색:%2d  배경색:%2d [s%s[ 현재색 ][u\n",acolor[i2].color_char,acolor[i2].color_back,get_gool(acolor[i2].mark));
		}
		printf(" 선택(Q,W)>>");
		lineinput(buff,10);check_han(buff);printf("\n");
		i2=atoi(buff)-1;
		if (is_quit(buff)) break;
		if (is_same(buff,"q")) break;
		if (is_same(buff,"w")) break;
		if (is_same(buff,"x")) break;
		if (is_same(buff,"xx")) break;
		if (buff[0]==0) continue;
		if (i2<0||i2>10) continue;
		printf(" %2d:",i2+1);
		bzero(buff,10);buff[0]=acolor[i2].mark;
		printf(" 표시문자   :%2s 변경표시문자 ",buff);
		lineinput(buff,2);printf("\n");
		if (buff[0]!=0) acolor[i2].mark=buff[0];
		printf(" 표시설명   :%-8s 변경표시설명 ",acolor[i2].name);
		lineinput(buff,8);printf("\n");
		if (buff[0]!=0) strcpy(acolor[i2].name,buff);
		printf(" 추천글자색 :%2d 변경배경색   ",acolor[i2].color_char);
		acolor[i2].color_char=get_num(acolor[i2].color_char,16);printf("\n");
		printf(" 추천배경색 :%2d 변경글자색   ",acolor[i2].color_back);
		acolor[i2].color_back=get_num(acolor[i2].color_back,16);printf("\n");
	}

	//해당화일정보저장
	if ((fp=fopen(GOOL_FILE,"w+"))!=NULL) {
		for(i=0;i<10;i++) {
			fwrite(&acolor[i],sizeof_acolor,1,fp);
		}
		fclose(fp);
	}
}


job_menu()
{
	while (1) {
		cls();
		print_(0,"메뉴편집");
		printf("   1. 메인메뉴편집                     :%s\n",MENU_FILE);
		printf("   2. 가기코드 입력시 GO 없어도 검색   :%s\n",rbit(cfg.gomode,1,"검색", "무시"));
		printf("   3. 가기코드 입력시 가기 제목 검색   :%s\n",rbit(cfg.gomode,2,"검색", "무시"));
		printf("   4. 로칼 메뉴 설정\n");
		num=numselect();
		xy(60,n+1);
		if (num<= 0) break;
		if (num== 1) sys_tem("bin/ed_menu %s main",now_id,club_go.code);
		if (num== 2) cfg.gomode=gtt_bit(cfg.gomode,1);
		if (num== 3) cfg.gomode=gtt_bit(cfg.gomode,2);
		if (num== 4) sys_tem("bin/pf '%s' set_localmenu",pf.id);
	}
}


sort_pf()
{
	int num;
	while (1) {
		cls();
		printfL2();
		printf("###  회원 정리작업  ###\n");
		printf("  1. 회원 번호 재정리  (중복회원번호/특수문자ID 등 재정리)  \n");
		printf("  2. 삭제된 회원 자료 확인 (등급0,삭제처리 회원 확인)       \n");
		printf("  3. 등급 0 인 회원 -> 삭제 처리 (아이디/성명 -> 공백처리   \n");
		printf("  4. 등급 0 인 회원 -> 삭제 처리 (주민등록번호 사용금지처리)\n");
		printf("  5. 일정기간 접속 없는회원 삭제 처리 -> 등급 0 처리        \n");
		printf("  6. 경고 9 해당       회원 삭제 처리 -> 등급 0 처리        \n");
		printf(" *7. 완전삭제된 회원 (ID,성명 빈칸)   -> 완전 삭제처리      \n");
		printf("  9. 회원 자료 전체 일련 번호(내부처리용) 지정              \n");
		printfL2();
		num=numselect();
		if (num<= 0) break;
		if (num== 1) renum_pf();
		if (num== 2) disp_del_pf();
		if (num== 3) pack_del_pf(1);
		if (num== 4) pack_del_pf(2);
		if (num== 5) pack_del_pf(3);
		if (num== 6) pack_del_pf(4);
		if (num== 7) pack_del_pf(5);
		if (num== 9) renum_proccode();	 //회원 재정리
	}
}



job_news()
{
	int i,n;
	char buff1[200];

	while (1) {
		cls();
		printf("  1.  뉴스기능 동작여부                         :%s\n",rbit(cfg.newsmode,0,"가능","불가"));
		printf("  2.  데이타화일 가기코드                       :%s\n",cfg.newsfile);
		printf("  3.  뉴스보기 가능 등급        (0:손님 19:시삽):%d\n",cfg.newstype);
		printf("  4.  뉴스보기 최신자료시간     (0:무시 #:시간) :%d\n",cfg.newsoldtime);
		printf("  9.  화면갱신시 알람시그날동작(뉴스..)         :%s\n",rbit(cfg.logomode,10, "동작", "중지"));
		printf(" 10.  메인메뉴     뉴스 가능                    :%s\n",rbit(cfg.newsmode,1,"가능","거부"));
		printf(" 11.  보조메뉴     뉴스 가능                    :%s\n",rbit(cfg.newsmode,2,"가능","거부"));
		printf(" 12.  게시판       뉴스 가능                    :%s\n",rbit(cfg.newsmode,3,"가능","거부"));
		printf(" 13.  동호회메인   뉴스 가능                    :%s\n",rbit(cfg.newsmode,4,"가능","거부"));
		printf(" 14.  동호회보조   뉴스 가능                    :%s\n",rbit(cfg.newsmode,5,"가능","거부"));
		printf(" 15.  동호회게시판 뉴스 가능                    :%s\n",rbit(cfg.newsmode,6,"가능","거부"));
		printf(" 16.  추천된 자료만 선택                        :%s\n",rbit(cfg.newsmode,7,"전부","추천"));
		printf(" 17.  *.TA? 메인TOP에서만 출력                  :%s\n",rbit(cfg.newsmode,8,"TOP만","전부"));
		printfL2();
		n=numselect();
		if (n<= 0)	break;
		if (n== 1)	cfg.newsmode =gtt_bit(cfg.newsmode,0);
		if (n== 2)	{xy(2,20);get_str(cfg.newsfile,8);}
		if (n== 3)	cfg.newstype  =get_num(cfg.newstype,2 );
		if (n== 4)	cfg.newsoldtime=get_num(cfg.newsoldtime,5);
		if (n== 9)	cfg.logomode  =gtt_bit(cfg.logomode,10);
		if (n==10)	cfg.newsmode =gtt_bit(cfg.newsmode,1);
		if (n==11)	cfg.newsmode =gtt_bit(cfg.newsmode,2);
		if (n==12)	cfg.newsmode =gtt_bit(cfg.newsmode,3);
		if (n==13)	cfg.newsmode =gtt_bit(cfg.newsmode,4);
		if (n==14)	cfg.newsmode =gtt_bit(cfg.newsmode,5);
		if (n==15)	cfg.newsmode =gtt_bit(cfg.newsmode,6);
		if (n==16)	cfg.newsmode =gtt_bit(cfg.newsmode,7);
		if (n==17)	cfg.newsmode =gtt_bit(cfg.newsmode,8);
	}
}


del_mail()
{
	FILE *fp;
	int dday;

	dday=30;
	if (dday==0) return;

	read_cfg();
	set_tmpfile();

	if ((fp=fopen("id/id.dat","r"))!=NULL) {
		while (fread(&pf,sizeof_pf,1,fp)) {
			if (pf.type>=18) continue;
			sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf.id),pf.id);
			sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf.id),pf.id);
			if (is_file(NOW_FILE)) bbs_old_del(dday);
			sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf.id),pf.id);
			sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf.id),pf.id);
			if (is_file(NOW_FILE)) bbs_old_del(dday);
		}
		fclose(fp);
	}
	printf("\n");
}


//삭제된 게시물 정리
bbs_old_del(int dday)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char buff[180];
	char c;
	char str[10];
	int found=No;
	int ret,i,new_posi;
	time_t today;
	time_t today1;
	time_t today2;
	int bbs_num;

	time(&today);	  //오늘의 시간
	today1 = today - (dday*24*3600);   //삭제대상 날짜

	fp1=fopen(NOW_FILE,"r");      //제목화일
	fp2=fopen(NOW_FILED,"r");      //제목화일
	fp3=fopen(TMP_FILE,"w");      //임시화일
	fp4=fopen(TMP_FILE2,"w");      //임시화일

	new_posi=0;
	bbs_num=0;
	while (fread(&bbs,sizeof_bbs,1,fp1)) {
		if (bbs.date<today1) {
			sprintf(buff,"userdata/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
			unlink(buff);
			continue;
		}
		fseek(fp2,bbs.posi,SEEK_SET);
		fseek(fp4,new_posi,SEEK_SET);
		for (i=0;i<bbs.size;i++) fputc(fgetc(fp2),fp4);
		bbs.posi=new_posi;
		new_posi+=bbs.size;
		bbs.num=++bbs_num;
		fwrite(&bbs,sizeof_bbs,1,fp3);
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (fp3!=NULL) fclose(fp3); if (fp4!=NULL) fclose(fp4);

	file_move(TMP_FILE ,NOW_FILE );
	file_move(TMP_FILE2,NOW_FILED);
	if (file_size(NOW_FILE)==0) {
		unlink(NOW_FILE );
		unlink(NOW_FILED);
	}
}

int apf_level[20];
int atoday_tim1[20];
int atoday_tim2[20];
int atoday_down[20];
GG gg[10];

edit_atype(int mode) {
	char buff[80];
	while (1) {
		disp_type();
		edit_type(mode);
		disp_type();
		while (1) {
			xy(5,25);
			printf("   저장:W  취소:Q  재입력:R 기본등급명:D  선택>>");
			lineinput(buff,2);
			printf("\n");
			if (is_char("wWqQrRDd",buff[0])) break;
		}
		if (is_same(buff,"w")) {
			save_atype();
			save_pftype();
			break;
		}
		if (is_same(buff,"d")) {
			def_atype();
		}
		xy(5,26);
		if (is_same(buff,"Q")&&yesno("저장을 취소할까요? (Y/n) >>",Yes)) {
			break;
		}
	}
}



disp_type()
{
	int i;
	cls();
	printf("           회원 등급별 환경설정 \n");
	printfL2();
	printf(" 등급    등급이름    진입레벨 일일제한시간 1회접속제한\n");
	printfL2();
	for (i=0;i<20;i++) {
		disp_type2(i);
	}
}


disp_type2(int i)
{
		xy(1,i+5);
		if (i>=14)
			printf("  %2d    %-8s\n",
			i,atype[i],apf_level[i]);
		else
			printf("  %2d    %-8s  %8d  %8d  %8d\n",
			i,atype[i],apf_level[i],atoday_tim1[i],atoday_tim2[i]);
}


edit_type(int mode)
{
	int i;
	char buff[50];
	if (mode==1) {
		for (i=0;i<20;i++) {
			xy(9,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) strcpy(atype[i],buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
			del_space(atype[i]);
		}
	}
	else if (mode==2) {
		for (i=0;i<14;i++) {
			xy(20,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) apf_level[i]=atoi(buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
		}
	}
	else if (mode==3) {
		for (i=0;i<14;i++) {
			xy(30,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) atoday_tim1[i]=atoi(buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
		}
	}
	else if (mode==4) {
		for (i=0;i<14;i++) {
			xy(40,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) atoday_tim2[i]=atoi(buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
		}
	}
}



save_atype()
{
	char type_file[140];
	FILE *fp;
	int i;
	sprintf(type_file,"system/typename.dat");
	fp=fopen(type_file,"w");
	for (i=0;i<20;i++) fwrite(&atype[i],sizeof(atype[i]),1,fp);
	if (fp!=NULL) fclose(fp);
}


read_pftype()  //회원의 등급,일일사용가능시간을 확인한다.
{
	int old_type;
	int i,n;
	FILE *fp;
	for (i=0;i<20;i++) apf_level[i]=999999;
	for (i=0;i<20;i++) atoday_tim1[i]=99999;
	for (i=0;i<20;i++) atoday_tim2[i]=99999;
	for (i=0;i<20;i++) atoday_down[i]=99999;
	fp=fopen("system/pftype.dat","r");   //등급화일
	if (fp==NULL) return;
	for (i=0;i<14;i++) fread(&apf_level  [i],sizeof(apf_level  [i]),1,fp);
	for (i=0;i<14;i++) fread(&atoday_tim1[i],sizeof(atoday_tim1[i]),1,fp);
	for (i=0;i<14;i++) fread(&atoday_tim2[i],sizeof(atoday_tim2[i]),1,fp);
	for (i=0;i<14;i++) fread(&atoday_down[i],sizeof(atoday_down[i]),1,fp);
	if (fp!=NULL) fclose(fp);
}


save_pftype()  //회원의 등급,일일사용가능시간을 확인한다.
{
	int old_type;
	int i,n;
	FILE *fp;
	fp=fopen("system/pftype.dat","w");   //등급화일
	if (fp==NULL) return;
	for (i=0;i<14;i++) fwrite(&apf_level  [i],sizeof(apf_level	[i]),1,fp);
	for (i=0;i<14;i++) fwrite(&atoday_tim1[i],sizeof(atoday_tim1[i]),1,fp);
	for (i=0;i<14;i++) fwrite(&atoday_tim2[i],sizeof(atoday_tim2[i]),1,fp);
	for (i=0;i<14;i++) fwrite(&atoday_down[i],sizeof(atoday_down[i]),1,fp);
	fclose(fp);
}


def_atype()
{
	strcpy(atype[0],"손님");
	strcpy(atype[1],"회원1");
	strcpy(atype[2],"회원2");
	strcpy(atype[3],"회원3");
	strcpy(atype[4],"회원4");
	strcpy(atype[5],"회원5");
	strcpy(atype[6],"회원6");
	strcpy(atype[7],"회원7");
	strcpy(atype[8],"회원8");
	strcpy(atype[9],"회원9");
	strcpy(atype[10],"회원10");
	strcpy(atype[11],"회원11");
	strcpy(atype[12],"회원12");
	strcpy(atype[13],"회원13");
	strcpy(atype[14],"동호시삽");
	strcpy(atype[15],"부운영진");
	strcpy(atype[16],"운영진");
	strcpy(atype[17],"시삽진");
	strcpy(atype[18],"부시삽");
	strcpy(atype[19],"시삽");
}



edit_ajob()
{
	int i;
	char buff[50];
	FILE *fp1;

	read_ajob();	//직업읽기
	cls();
	xy(1,1);printf("              번호별 직업명 수정\n");
	printfL2();
	for (i=1;i<19;i++) printf("%6d. %s\n",i,ajob[i]);
	printfL2();
	for (i=1;i<19;i++) {
		xy(20,i+2);
		printf(" 수정>>");
		lineinput(buff,10);
		if (is_quit(buff)) break;
		if (buff[0]!=0) strcpy(ajob[i],buff);
	}
	if (!is_same(buff,"q")) {
		fp1=fopen("system/ajob.dat","w");
		for (i=1;i<19;i++) fwrite(&ajob[i],sizeof(ajob[i]),1,fp1);
		if (fp1!=NULL) fclose(fp1);
	}
}


edit_addr()
{
	xedit("frm/guest/_post.txt");
}


gg_read()
{
	FILE *fp;
	int i;

	for (i=0;i<10;i++) {
		strcpy(gg[i].code,"");
		strcpy(gg[i].name,"");
	}

	fp=fopen("system/gg.dat","r+");
	if (fp!=NULL)
		for (i=0;i<10;i++) fread(&gg[i],sizeof_gg,1,fp) ;
	if (fp!=NULL) fclose(fp);
}


disp_gg()
{
	int i;
	printf("\n");
	printfL2();
	for (i=0;i<10;i++)
		printf("       [  G%-2d ] %-20s ( %-8s )\n",i+1,gg[i].name,gg[i].code);
}

edit_gg()
{
	char buff[30];
	FILE *fp;
	int i;

	gg_read();
	while (1) {
		printf("\n");
		printfL2();
		printf("     추천 동호회/게시판 편집 \n");
		disp_gg();
		printfL2();
		printf("    편집할 추천 번호  종료:0 >>");
		lineinput(buff,4);
		i=atoi(buff);
		if (i==0) {
			buff[0]=' ';
			i=atoi(buff);
		}
		if (i<1||i>10) break;
		printf("\n 변경  %d. 가기이름 >>",i);
		lineinput(buff,20);
		if (buff[0]!=0) strcpy(gg[i-1].name,buff);
		printf("\n 변경  %d. 가기코드 >>",i);
		lineinput(buff,8);
		if (buff[0]!=0) strcpy(gg[i-1].code,buff);
	}
	fp=fopen("system/gg.dat","w");
	for (i=0;i<10;i++) fwrite(&gg[i],sizeof_gg,1,fp) ;
	if (fp!=NULL) fclose(fp);
	printf("\n\n  저장이 완료되었습니다. \n\n");
	getchar();
}






check_init_dir()
{
	check_dir("bbsdata/okrec");
	check_dir("menurec");
	check_dir("menucfg");
	check_dir("/tmp/xhost/rec");
	check_dir("/tmp/monich");
	check_dir("id");
	check_dir("menu");
	check_dir("sys_back");
	check_dir("userdata");
	check_dir("userroom");
	check_dir("user");
	check_dir("user/rec");
	check_dir("user/log");
	check_dir("user/score");
	check_dir("user/chat");
	check_dir("user/monich");
	check_dir("user/news");
	check_dir("user/fcmd");
	check_dir("user/moni");
	check_dir("user/lock");
	check_dir("user/message");
	check_dir("tmp");
	system("chmod 777 /tmp/xhost -R &");
	system("chmod 777 /tmp/system* &");
	system("chmod 777 /tmp/monich -R &");
}


rawmode()
{
	struct termio tdes;
	ioctl(0,TCGETA,&tdes);	 //현재 터미날 모드를 읽어온다
	tdes.c_iflag = 0;
	tdes.c_iflag |= ICRNL ;
	tdes.c_iflag |= IXON ;
	tdes.c_oflag = 0;
	tdes.c_oflag |= OPOST ;
	tdes.c_oflag |= ONLCR ;
	tdes.c_cflag |= CRTSCTS ;
	tdes.c_cflag |= HUPCL ;
	tdes.c_lflag = 0;
	tdes.c_lflag |= IEXTEN;
	tdes.c_cc[VMIN]=1;
	tdes.c_cc[VTIME]=0;
	ioctl(0,TCSETA,&tdes);	 //현재 터미날 모드를 저장한다.
}



view_daypf()		//일별 가입현황
{
	FILE *fp,*fp2;
	time_t indate;
	int inday;
	int i,n,count1,count2;
	fp=fopen(ID_FILE,"r");
	i=0;
	printf("\n");
	indate=0;
	inday=count1=count2=0;
	fp2=fopen(TMP_FILE,"w");
	while (fread(&pf,sizeof_pf,1,fp)) {
		if (pf.inday<1) continue;
		n=time_num(pf.inday);
		if (inday>0&&inday!=n) {
			fprintf(fp2,"%s %6d명 가입  누적%6d명\n",datestr(35,indate),count1,count2);
			count1=0;
		}
		indate=pf.inday;
		inday=n;
		count1++;
		count2++;
	}
	fclose(fp);
	fclose(fp2);
	view_text(TMP_FILE);
}


view_all_club(int mode)
{
	FILE *fp1,*fp2;
	char id_file[300];
	char buff[200],a_code[1000][70];
	int anum[100],found;
	int i,count=0,club_count=0;
	for (i=0;i<100;i++) anum[i]=0;
	if ((fp1=fopen(MENU_FILE,"r"))==NULL) return;
	if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
	printf("\n ... 잠시만 기다려 주십시요. 동호회를 검색중입니다....\n");
	club_mode=Yes;
	fprintf(fp2,"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	if (mode==1) {
		fprintf(fp2,"    동호코드 동호회이름          동호회장 동호회디렉토리\n");
	}
	else if (mode==2) {
		fprintf(fp2,"    동호코드 동호회장 총회원 총메뉴 (메뉴/자료실/크기) 가입/공개/진입\n");
	}
	else {
		fprintf(fp2,"    동호코드 동호회이름          동호회장 동호회디렉토리\n");
		fprintf(fp2," 총회원 총메뉴 (메뉴/자료실/크기) 가입/공개/진입\n");
	}
	fprintf(fp2,"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	for (i=0;i<1000;i++) strcpy(a_code[i],"");
	while(fread(&club_go,sizeof_go,1,fp1)) {
		if (strlen(club_go.code)<1) continue;
		if (club_go.type=='s') {
			sprintf(buff,"%s%s",club_go.code,club_go.dir);
			found=No;
			for (i=0;i<club_count;i++) {
				if (is_same(a_code[i],buff)) {
					found=Yes;
					break;
				}
			}
			if (found) continue;
			strcpy(a_code[club_count++],buff);
			anum[3]++;
		}
		if (club_go.type!=' ') {anum[1]++;anum[2]++;}
			 if (is_char(PDSTYPE,club_go.type)) anum[4]++;
		else if (is_char(BBBTYPE,club_go.type)) anum[5]++;
		else if (club_go.type=='n') anum[7]++;
		else if (is_char(EXETYPE,club_go.type)) anum[6]++;

		if (club_go.type!='s') continue;
		read_ccfg();
		if (mode==1) {
			fprintf(fp2,"%-3d %-8.8s %-20.20s %-8s %-30.30s\n",
			count+1,club_go.code,club_go.name,ccfg.sysop,club_go.dir);
		}
		else if (mode==2) {
			sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
			sprintf(NOW_MENU ,"%s/%s",club_go.dir,MENU_FILE);
			fprintf(fp2,"%-3d %-8.8s %-8s %6d %6d (%4d %4d %6d) %2d %2d %2d\n",
			count+1,club_go.code,ccfg.sysop,
			count_cpf(id_file),count_menu(NOW_MENU),
			ccfg.maxmenu,ccfg.maxpdsmenu,ccfg.maxpdssize,
			ccfg.autoguest,ccfg.modeopen,ccfg.intype);
		}
		else {
			sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
			sprintf(NOW_MENU ,"%s/%s",club_go.dir,MENU_FILE);
			fprintf(fp2,"%-3d %-8.8s %-30.30s %-8s %-30.30s ",
			count+1,club_go.code,club_go.name,ccfg.sysop,club_go.dir);
			fprintf(fp2,"%6d %6d (%4d %4d %6d) %2d %2d %2d\n",
			count_cpf(id_file),count_menu(NOW_MENU),
			ccfg.maxmenu,ccfg.maxpdsmenu,ccfg.maxpdssize,
			ccfg.autoguest,ccfg.modeopen,ccfg.intype);
		}
		count++;
	}
	fprintf(fp2,"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	fprintf(fp2,"  총메뉴:%d  메인메뉴:%d 동호회:%d 자료실:%d 게시판:%d 외부명령:%d 텔넷:%d\n",
			anum[1],anum[2],anum[3],anum[4],anum[5],anum[6],anum[7]);
	fclose(fp1);
	fclose(fp2);
	view_text(TMP_FILE);
	unlink(TMP_FILE);
}


edit_limit_id_file()
{
	FILE *fp;
	char filename[300];
	char buff[300],buff1[300],buff2[300],buff3[300];
	check_dir("user/limit");
	strcpy(filename,"user/limit/limit.id");
	xedit2(filename);
	system("rm -f user/limit/limit_*.id");
	if ((fp=fopen(filename,"r"))==NULL) return;
	while(fgets(buff,200,fp)!=NULL) {
		strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");
		sscanf(buff,"%s %s %s",buff1,buff2,buff3);
		if (strlen(buff1)<3) continue;
		sprintf(buff,"%s %s",buff1,buff2);
		sprintf(buff3,"user/limit/limit_%u.id",(word)buff1[0]%255);
		put_line(buff,buff3);
	}
	fclose(fp);
}
job_info()
{
	FILE *fp;
	FCFG fcfg;
	int i,n,num,n2;
	char buff[100];
	char filename[200];

	while (1) {
		cls();
		printf("  ###  인포샵 관련 작업 ###\n");
		printf("  1. 인포샵용 : 기본 설정 지정     :%s\n",rbit(cfg.infomode,0,"일반","인포샵지정"));
		printf("  2. 인포샵용 : 모든회원 쓰기가능  :%s\n",rbit(cfg.infomode,1,"불가능","가능"));
		printf("  3. 인포샵용 : 모든회원 등록가능  :%s\n",rbit(cfg.infomode,2,"불가능","가능"));
		printf("  4. 인포샵용 : 모든회원 다운가능  :%s\n",rbit(cfg.infomode,3,"불가능","가능"));
		printf("  5. 인포샵용 : 전체회원 손님접속  :%s\n",rbit(cfg.infomode,4,"ID입력","ID통과"));
		printf("  6. 인포샵용 : 진입로고 안보임    :%s\n",rbit(cfg.infomode,5,"보임","통과"));
		printf("  7. 인포샵용 : 아이디 입력받음    :%s\n",rbit(cfg.infomode,6,"받음","통과"));
		printf("  8. 인포샵용 : 공지사항 보여줌    :%s\n",rbit(cfg.infomode,7,"보임","통과"));
		printf("  9. 인포샵용 : 종결시   질문함    :%s\n",rbit(cfg.infomode,8,"질문","통과"));
		printf(" 10. 인포샵용 : 모든손님->등급2회원:%s\n",rbit(cfg.infomode,9,"보통","지정"));
		n=numselect();
		xy(50,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.infomode=gtt_bit(cfg.infomode,0);
		if (n== 2)	cfg.infomode=gtt_bit(cfg.infomode,1);
		if (n== 3)	cfg.infomode=gtt_bit(cfg.infomode,2);
		if (n== 4)	cfg.infomode=gtt_bit(cfg.infomode,3);
		if (n== 5)	cfg.infomode=gtt_bit(cfg.infomode,4);
		if (n== 6)	cfg.infomode=gtt_bit(cfg.infomode,5);
		if (n== 7)	cfg.infomode=gtt_bit(cfg.infomode,6);
		if (n== 8)	cfg.infomode=gtt_bit(cfg.infomode,7);
		if (n== 9)	cfg.infomode=gtt_bit(cfg.infomode,8);
		if (n==10)	cfg.infomode=gtt_bit(cfg.infomode,9);
	}
}


set_roomtype_limit()	 //분류별 제한설정
{
	FILE *fp;
	CROOM tcroom;
	int found=No,n;
	char filename[100],buff[100];
	strcpy(filename,"system/chattype.dat");
	while (1) {
		printf("\n ## 설정 대화실 타입 (1~39) >>");
		lineinput2(buff,3);
		n=atoi(buff);
		if (is_quit(buff)) return;
		if (n==0) continue;
		if (n<1||n>39) return;
		tcroom.age1=tcroom.age1=tcroom.itype=tcroom.sex=0;
		if ((fp=fopen(filename,"r"))!=NULL) {
			fseek(fp,n*sizeof_croom,SEEK_SET);
			fread(&tcroom,sizeof_croom,1,fp);
			fclose(fp);
		}
		disp_room_limit(n,tcroom);
		printf("\n  대화실 타입번호: %d",n);
		printf("\n  진입 나이 시작 : %2d >>",tcroom.age1 );tcroom.age1 =get_num(tcroom.age1 ,2);
		printf("\n  진입 나이  끝  : %2d >>",tcroom.age2 );tcroom.age2 =get_num(tcroom.age2 ,2);
		printf("\n  진입 등급      : %2d >>",tcroom.itype);tcroom.itype=get_num(tcroom.itype,2);
		printf("\n  성별 1:남 2:여 : %2d >>",tcroom.sex  );tcroom.sex  =get_num(tcroom.sex  ,1);
		printf("\n");
		disp_room_limit(n,tcroom);
		if ((fp=fopen(filename,"r+"))==NULL)
			  fp=fopen(filename,"w+");
		fseek(fp,n*sizeof_croom,SEEK_SET);
		fwrite(&tcroom,sizeof_croom,1,fp);
		fclose(fp);
	}
}


disp_room_limit(int n,CROOM tcroom)
{
	printf(" ## 대화실 타입번호: %d \n",n);
	printf(" ## 진입 나이 시작 : %2d - %2d \n",tcroom.age1,tcroom.age2);
	printf(" ## 진입 등급      : %2d \n",tcroom.itype);
	printf(" ## 성별 1:남 2:여 : %2d \n",tcroom.sex  );
}
