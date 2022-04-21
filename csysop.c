////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 동호회 시삽 전용 메뉴
////////////////////////////////////////////////////////////////////////

#include "bbs.h"

int nn;
int top;
int last;
int key;
int n;

char id_file[300];
char menu_file[300];
char filename[300];
char now_code[100];
char now_id[60];
int nwait0,nwait1,ntotal1,ntotal2,ntotal3;


main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	PF pf2;
	int n=0,i;
	int key=No;
	char buff[140];
	char tcode[60];

	printf("\n<%s>\n",VER1);msleep(3);
    if (argc<4) {
        printf("\n사용법  csysop <동호회 코드> <가기코드> <작업자 아이디>\n");
		getchar();
		return;
	}
	check_initdir();
	strcpy(tcode,argv[1]);				//동호회 코드
    if (is_same(tcode,"main")) {mess("동호회 전용명령");return;}
    strcpy(now_id,argv[3]);              //작업자 아이디
	findport(tty); //접속포트 찾기
	read_cfg();
	set_tmpfile();
    if (!read_pf3(now_id)) {mess("%s ID검색실패",now_id);return;}
	if (!find_club(tcode)) {
		mess("%s동호회 검색실패",tcode);
		return;
	}
    find_go(argv[2]);

	check_dir("%s/system",club_go.dir);
	club_mode=Yes;
	read_ccfg();
	read_cpf(now_id);					//해당되는 동호회의 아이디를 찾는다.
    sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
	read_atype();						//등급읽기
	read_actype();						//동호회등급의 이름을 읽어옴  ->actype
	if (pf.type==16)				 cpf.type=17;
	if (pf.type==17)				 cpf.type=18;
	if (pf.type>=18)				 cpf.type=19;
	if (is_same(ccfg.sysop,pf.id))	 cpf.type=19;
	if (cpf.type<16) {
		printf("\n 동호회메뉴를 사용할수 없는 등급입니다.\n");
		getchar();
		return;
	}

	key=No;
	who=0;		   //1:동호회 운영진 2:동호회 시삽	3:root권한
	if (cpf.type>=17)				  who=1;
	if (pf.type==16)				  who=1;
	if (pf.type==17)				  who=2;
	if (is_same(ccfg.sysop,now_id))   who=2;
	if (cpf.type==18)				  who=2;
	if (cpf.type==19)				  who=2;
	if (pf.type==18)				  who=3;
	if (pf.type==19)				  who=4;
	if (who==0) {
		printf("\n 동호회 시삽메뉴를 사용할수 없는 등급입니다.\n");
		getchar();
		return;
	}
	if (pf.type<17&&who<3&&ccfg.passsys[0]!=0) {
		printf("\n %s 시삽메뉴의 암호가 지정되어 있습니다. 암호는? >>",club_go.code);
		passinput(buff,8);
		printf("\n");
		if (!is_same(ccfg.passsys,buff)) {
			printf("암호가 틀립니다. 암호를 확인하십시요\n");
			getchar();
			return;
		}
	}

	if (bit(cfg.backmode,5)==0) sys_back(club_go.dir) ; //시스템 백업

	key=Yes;
	while (1) {
		cls();
		xy(60, 7);printf("가입모드 :%s\n",(ccfg.autoguest==1)?"자동":"수동");
		if (ccfg.stop!=0) {
		xy(60,13);printf("임시폐쇄 :%d",ccfg.stop);
		}
		xy(1,1);
		printfL2();
		printf("          동호회 관리       작업자:%s 등급:%d\n",now_id,who);
		printfL2();
        printf("s 1. 동호회 코드                : %s\n",club_go.code);
        printf("s 2. 동호회 이름                : %s\n",club_go.name);
        printf("s 3. 동호회 디렉토리            : %s\n",club_go.dir);
        printf("s 4. 동호회 시삽                : %-8s\n",ccfg.sysop);
        printf("s 5. 최대메뉴수     (0:무제한)  : %d\n",ccfg.maxmenu);
        printf("s 6. 최대자료실수   (0:무제한)  : %d\n",ccfg.maxpdsmenu);
        printf("s 7. 최대자료실크기 (0:무제한)  : %d Mbyte\n",ccfg.maxpdssize);
        printf("s 8. 대화방 고정번호  (0:없음)  : %d (3000~4999)\n",ccfg.chatnum);
        printf("\n");
        printf(" 11. 동호회 환경/설정 관련\n");
        printf(" 12. 동호회 회원/가입 관련\n");
        printf(" 13. 동호회 화면/메뉴 관련\n");
        printf(" 14. 동호회 하위 디렉토리 전체 검사\n");
        printf(" 15. 작업 회원 지정     \n");
        printf(" 16. 사이버머니 관련 작업\n");
        printf(" 17. 현황 통계  관련 작업\n");
		printfL2();
		printf("선택번호? 메뉴복귀(q) >>");
		lineinput(buff,10);
		check_han(buff);
		n=atoi(buff);
		printf("\n");
		if (is_char("xXqpQq0",buff[0])) break;
		xy(30,n+3);
        if(who>=3&&n==1) {
			lineinput(buff,8);del_space(buff);strlwr(buff);
			if (buff[0]!=0) strcpy(club_go.code,buff);
		}
        if(who>=3&&n==2) {
			lineinput(buff,30);
			if (buff[0]!=0) strcpy(club_go.name,buff);
		}
        if(who>=3&&n==3) {
			lineinput(buff,60);del_space(buff);strlwr(buff);
			if (buff[0]!=0) strcpy(club_go.dir,buff);
		}
        if(who>=3&&n==4) get_str(ccfg.sysop,8);
        if(who>=3&&n==5) ccfg.maxmenu   =get_num(ccfg.maxmenu   ,5);
        if(who>=3&&n==6) ccfg.maxpdsmenu=get_num(ccfg.maxpdsmenu,5);
        if(who>=3&&n==7) ccfg.maxpdssize=get_num(ccfg.maxpdssize,5);
        if(who>=3&&n==8) ccfg.chatnum   =get_num(ccfg.chatnum   ,5);
		if(n==11) job_cfg();
		if(n==12) job_cid();
		if(n==13) job_menu();
		if(n==14) { 		//하위 디렉토리 확인
			sys_tem("ls -R -L -G -l %s > %s",club_go.dir,TMP_FILE);
			view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
		if(n==15) job_idset();
		if(n==16) job_money();
        if(n==17) job_stat();
	}
    if (ccfg.chatnum<3000||ccfg.chatnum>5000) ccfg.chatnum=3000;

	//해당 동호회의 환경을 저장한다.
	sprintf(buff,"%s/system/clubcfg.dat",club_go.dir);
	if ((fp=fopen(buff,"w"))!=NULL) {
		fwrite(&ccfg,sizeof_ccfg,1,fp) ;
		fclose(fp);
	}
	if (who>=3) {
		if ((fp=fopen(MENU_FILE,"r+"))!=NULL) {
			n=0;
			while (1) {
				fseek(fp,n*sizeof_go,SEEK_SET);
				if (!fread(&go,sizeof_go,1,fp)) break;
				if (go.type=='s'&&is_same(go.code,club_go.code)) {
					c_lock(1,MENU_FILE);
					go=club_go;
					fseek(fp,n*sizeof_go,SEEK_SET);
					fwrite(&go,sizeof_go,1,fp);
					c_lock(0,MENU_FILE);
				}
				n++;
			}
			fclose(fp);
		}
	}
}

//회원 관련 작업
job_stat()
{
	int i,num;
	char buff[100];
	char type_file[140];
	FILE *fp;
	while (1) {
		cls();
        printf("          회원 통계 작업\n");
        check_total();
		xy(60, 8);printf("총 회 원 :%6d명\n",ntotal1);
		xy(60, 9);printf("등급 0   :%6d명\n",nwait0);
		xy(60,10);printf("가입대기 :%6d명\n",nwait1);
		xy(60,11);printf("운 영 진 :%6d명\n",ntotal2);
		xy(60,12);printf("7일간가입:%6d명\n",ntotal3);
        xy(1,20);
		printf(" 선택 : >>");
		num=numinput();
		printf("\n");
		xy(60,num+1);
		if (num== 0) break;
    }
}





//회원 관련 작업
job_cid()
{
	int i,num;
	char buff[100];
	char type_file[140];
	FILE *fp;
    check_total();
	while (1) {
		cls();
		printf("          회원 관련 작업\n");
		printf(" 1. 동호회 회원 신상 조회                         :\n");
		printf(" 2. 동호회 회원 등급 조정                         :\n");
        printf(" 3. 동호회 회원 직접(수동) 가입                   :\n");
		printf(" 4. 동호회 회원 가입허가  (등급1->등급2로)    대기:%d 명:\n",nwait1);
		printf(" 5. 동호회 회원 자동 가입 (0:수동 1:자동 2:불허)  :%d\n",ccfg.autoguest);
		printf(" 6. 동호회 진입 가능 메인 등급 (0:손님 1-19회원)  :%d\n",ccfg.intype);
		printf(" 7. 동호회 진입 가능 동호 등급 (1:손님 2-19회원)  :%d\n",ccfg.cintype);
		printf(" 8. 동호회 등급 <- 메인등급연동                   :%s\n",rbit(ccfg.mode,1,"독자","연동"));
		printf(" 9. 동호회 신입 회원 신청가능 등급 (0-19)         :%d\n",ccfg.gtype);
		printf("10. 동호회 신입 회원 신청가능 나이 (시작-끝)      :%2d-%2d\n",ccfg.gage1,ccfg.gage2);
		printf("11. 동호회 참고 항목 선택,항목명 수정             \n");
		printf("12. 동호회 등급별 이름 수정                       \n");
		printf("13. 동호회 가입 불가능 아이디 지정   (system/notguest.txt)\n");
		printf("14. 가입 금지 아이디 지정 사유 화면  (logo/_whynot6.log)\n");
        printf("15. 날짜별 동호회 가입현황                              \n");
        printf("16. 동호회 회원 통계                                    \n");
        printf("17. 동호회 가입자 파일등록  (자동 가입 처리)            \n");
        printf("18. 동호회 진입 가능 나이 (시작-끝)               :%02d-%02d\n",ccfg.age/100,ccfg.age%100);
		printfL2();
		printf(" 선택 : >>");
		num=numinput();
		printf("\n");
		xy(60,num+1);
		if (num== 0) break;
		if (num== 1) edit_cpf(who);
		if (num== 2) set_cpf_type();  //등급조정
		if (num== 3) free_cwait(0);   //수동가입
		if (num== 4) free_cwait(1);
		if (num== 5) ccfg.autoguest=get_num(ccfg.autoguest,1);
		if (num== 6) ccfg.intype=get_num(ccfg.intype,2);
		if (num== 7) ccfg.cintype=get_num(ccfg.cintype,2);
		if (num== 8) ccfg.mode=gtt_bit(ccfg.mode,1);
		if (num== 9) ccfg.gtype=get_num(ccfg.gtype,2);
		if (num==10) {
			xy(60,num+3);
			ccfg.gage1=get_num(ccfg.gage1,2);
			xy(64,num+3);
			ccfg.gage2=get_num(ccfg.gage2,2);
		}
		if (num==11) cguest_item();
		if (num==12) {
			disp_ctype();
			for (i=0;i<20;i++) {
				xy(9,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) strcpy(actype[i],buff);
				disp_ctype2(i);
				if (is_same(buff,"q")) break;
				del_space(actype[i]);
			}
			disp_ctype(i);
			if (yesno("\n 위의 동호회 등급명을 저장할까요? (Y/n) >>",Yes)) {
				printf("\n");
				sprintf(type_file,"%s/system/typename.dat",club_go.dir);
				fp=fopen(type_file,"w");
				for (i=0;i<20;i++) fwrite(&actype[i],sizeof(actype[i]),1,fp);
				if (fp!=NULL) fclose(fp);
			}
		}
		if (num==13) {
			sprintf(buff,"%s/system/notguest.txt",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (num==14) {
			sprintf(buff,"%s/logo/_whynot6.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
        if (num==15) view_daycpf();
        if (num==16) view_stat();
        if (num==17) free_auto_file();
        if (num==18) ccfg.age=numinput();
    }
}


free_auto_file()
{
    FILE *fp1,*fp2;
    char buff[100],buff1[100],buff2[100],buff3[100];
    int i,n,count;
    count=0;
    if (!input_tmp(0)) return;
    if ((fp1=fopen(TMP_FILE,"r"))==NULL) return;
    while(fgets(buff,200,fp1)!=NULL) {
        strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");
        sscanf(buff,"%s %s %s",buff1,buff2,buff3);
        if (strlen(buff1)<3) continue;
        i=atoi(buff2);
        if (i<3) i=3;
        printf("%5d %-8s %4d  -> ",count+1,pf2.id,i);
        if (!read_pf4(buff1)) {printf("등록ID 아님\n");continue;}
        if (read_cpf(pf2.id)) {printf("동호회 이미등록됨\n");continue;}
        cpf.type=i;
        strcpy(cpf.id   ,pf2.id);
        strcpy(cpf.name ,pf2.name);
        strcpy(cpf.memo1,"");
        strcpy(cpf.memo2,"");
        strcpy(cpf.memo3,"");
        strcpy(cpf.memo4,"");
        strcpy(cpf.memo5,"");
        time(&cpf.inday);
        time(&cpf.login);
        time(&cpf.logout);
        cpf.countu=0;
        cpf.countw=0;
        cpf.countd=0;
        cpf.all_log=0;
        cpf.all_time=0;
        fp2=fopen(id_file,"a+");
        if (fp2==NULL) fp2=fopen(id_file,"w+");
        if (fp2!=NULL) {
            cpf.num=ftell(fp2)/sizeof_cpf+1;
            fwrite(&cpf,sizeof_cpf,1,fp2);
            fclose(fp2);
            printf(" %d 등급 가입\n",cpf.type);
        }
    }
    fclose(fp1);
}


//동호회 현황
view_stat() {
    FILE *fpid;
    FILE *fpsave;
    char id_file[300];
    int  n,count,i,i2,i3,total;
    struct tm *tp;
	// arr1:레벨 arr2:접속횟수 arr3:접속시간


    sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
    total=count_cpf(id_file);                  //회원 총 수 계산
    fpid = fopen(id_file, "r+");
    fpsave = fopen(TMP_FILE, "w+");

    fprintf(fpsave,"            *** %s 동호회  총회원:%5d명  ***\n",club_go.code,total);

    fseek(fpid,0,SEEK_SET);
    for (n=0;n<20;n++) {
        count=0;
        fprintf(fpsave,"   ========== %d   레벨 %4d  ========== \n",club_go.code,n);
        fseek(fpid,0,SEEK_SET);
        for (i=0;i<=total;i++) {
            fread(&cpf,sizeof_cpf,1,fpid);         //아이디 저장
            if (cpf.type==n) {
                tp = localtime(&cpf.login);
                fprintf(fpsave," %3d.  %-8s %-8s %5d %5d %5d ",
                count+1,cpf.id, cpf.name, cpf.countw,cpf.countu, cpf.countd);
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
cguest_item()
{
	int num;
	char buff[100];
	int i;

	while (1) {
		cls();
		printf(" 회원가입 항목지정/편집\n");
		printf(" 1. 동호회 신청시 참고사항1 항목입력  %-8s    :%s\n",ccfg.note[0],rbit(ccfg.memo_input,1,"출력","통과"));
		printf(" 2. 동호회 신청시 참고사항2 항목입력  %-8s    :%s\n",ccfg.note[1],rbit(ccfg.memo_input,2,"출력","통과"));
		printf(" 3. 동호회 신청시 참고사항3 항목입력  %-8s    :%s\n",ccfg.note[2],rbit(ccfg.memo_input,3,"출력","통과"));
		printf(" 4. 동호회 신청시 참고사항4 항목입력  %-8s    :%s\n",ccfg.note[3],rbit(ccfg.memo_input,4,"출력","통과"));
		printf(" 5. 동호회 신청시 참고사항5 항목입력  %-8s    :%s\n",ccfg.note[4],rbit(ccfg.memo_input,5,"출력","통과"));
		printf(" 6. 동호회 신상조회시 참고1 항목출력  %-8s    :%s\n",ccfg.note[0],rbit(ccfg.memo_disp,1,"출력","통과"));
		printf(" 7. 동호회 신상조회시 참고2 항목출력  %-8s    :%s\n",ccfg.note[1],rbit(ccfg.memo_disp,2,"출력","통과"));
		printf(" 8. 동호회 신상조회시 참고3 항목출력  %-8s    :%s\n",ccfg.note[2],rbit(ccfg.memo_disp,3,"출력","통과"));
		printf(" 9. 동호회 신상조회시 참고4 항목출력  %-8s    :%s\n",ccfg.note[3],rbit(ccfg.memo_disp,4,"출력","통과"));
		printf("10. 동호회 신상조회시 참고5 항목출력  %-8s    :%s\n",ccfg.note[4],rbit(ccfg.memo_disp,5,"출력","통과"));
		printf("11. 동호회 참고사항 이름 편집                     :\n");
		printfL2();
		printf(" 선택 : >>");
		num=numinput();
		printf("\n");
		xy(60,num+1);
		if (num== 0) break;
		if (num== 1) ccfg.memo_input=gtt_bit(ccfg.memo_input,1);
		if (num== 2) ccfg.memo_input=gtt_bit(ccfg.memo_input,2);
		if (num== 3) ccfg.memo_input=gtt_bit(ccfg.memo_input,3);
		if (num== 4) ccfg.memo_input=gtt_bit(ccfg.memo_input,4);
		if (num== 5) ccfg.memo_input=gtt_bit(ccfg.memo_input,5);
		if (num== 6) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,1);
		if (num== 7) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,2);
		if (num== 8) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,3);
		if (num== 9) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,4);
		if (num==10) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,5);
		if (num==11) {
			cls();
			printfL2();
			printf("         동호회의 회원 신상의 참고 사항 항목 이름 편집\n");
			printfL2();
            for(i=0;i<5;i++) {
                printf("참고항목%2d: 현재:%-8s  신규:>>",i+1,ccfg.note[i]);
                lineinput(buff,8);printf("\n");
				if (buff[0]!=0)
					strcpy(ccfg.note[i],buff);
			}
		}
	}
}



//환경설정
job_cfg()
{
	int num;
	char buff[100];
	while (1) {
			cls();
		printfL2();
		printf("          동호회 환경 설정 메뉴\n");
		printfL2();
		printf(" 1. 동호회 자료실 디렉토리 지정 (0:자동   1:수동)              :%d\n",ccfg.dir   );
		printf(" 2. 동호회 시삽 메뉴 암호변경                                  :%s\n",ccfg.passsys  );
		printf(" 3. 동호회 진입 암호변경                                       :%s\n",ccfg.enterp);
		printf(" 4. 동호회 손님 공개 방법                                      :%d\n",ccfg.modeopen);
		printf(" 6. 동호회 임시 폐쇄  (0:보통 1:임시폐쇄 8:동장폐쇄 9:시삽폐쇄):%d\n",ccfg.stop  );
		printf(" 7. 전체 자료실 담당지정                                       :%s\n",ccfg.pds_man);
		printf(" 8. 전체 게시판 담당지정                                       :%s\n",ccfg.bbs_man);
		printf("11. 진입금지 메세지 (회원등급:0)                      (_whynot1.log)\n");
		printf("12. 진입금지 메세지 (비공개 동호회)                   (_whynot2.log)\n");
		printf("13. 진입금지 메세지 (동호회 임시 폐쇄)                (_whynot7.log)\n");
		printf("14. 진입금지 메세지 (동호회 영구 폐쇄:시삽)           (_whynot9.log)\n");
		printfL2();
		printf(" 선택 : >>");
		num=numinput();
		printf("\n");
		xy(70,num+3);
		if (num==0) break;
		if (who>3&&num==1) ccfg.dir=get_num(ccfg.dir,1);
		if (num==2) get_str(ccfg.passsys,8);
		if (num==3) get_str(ccfg.enterp,4);
		if (num==4) ccfg.modeopen=get_num(ccfg.modeopen,1);
		if (num==6) ccfg.stop=get_num(ccfg.stop,1);
		if (num==7) get_str(ccfg.pds_man,8);
		if (num==8) get_str(ccfg.bbs_man,8);
		if (num==11) {
			sprintf(buff,"%s/logo/_whynot1.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (num==12) {
			sprintf(buff,"%s/logo/_whynot2.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (num==13) {
            sprintf(buff,"%s/logo/_whynot7.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (num==14) {
			sprintf(buff,"%s/logo/_whynot9.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
	}
	del_space(ccfg.passsys);
	del_space(ccfg.enterp);
}


//메뉴,게시판
job_menu()
{
	int num;
	char buff[100];
	while (1) {
		cls();
		printfL2();
		printf("          메뉴,게시판,화면 설정\n");
		printfL2();
		printf(" 1. 동호회 전체 메뉴 확인                         \n");
		printf(" 2. 동호회 메뉴 편집                              \n");
		printf(" 3. 동호회 메뉴 링크 (메인메뉴와연동)             \n");
        printf(" 4. 동호회 전체자료실 내용 설정                   \n");
        printf(" 5. 동호회 하단 광고 편집                         \n");
        printf(" 6. 동호회 공지 사항 관련 설정                    \n");
        printf(" 7. 진입가능/제한 메뉴 지정  (system/_nocode.dat) \n" );
        printf(" 8. 진입가능/제한 화면 편집                       \n" );
        printf(" 9. 전체메일 발송 가능 등급     :%d\n",ccfg.wmalltype );
		printf("11. 메인메뉴   뉴스 속보                  :%s\n",rbit(ccfg.mode,11,"허용","거부"));
		printf("12. 보조메뉴   뉴스 속보                  :%s\n",rbit(ccfg.mode,12,"허용","거부"));
		printf("13. 게시판     뉴스 속보                  :%s\n",rbit(ccfg.mode,13,"허용","거부"));
        printf("14. 진입시 진입/시작화면                  :%s\n",rbit(ccfg.logomode,1,"보임","안보임"));
        printf("15. TOP 애니메이션 로고 화면              :%s\n",rbit(ccfg.logomode,2,"안함","동작함"));
        printf("16. 진입시 등급/레벨표시                  :%s\n",rbit(ccfg.logomode,3,"보임","안보임"));
        printf("21. 동호회 화면 관리                              \n");
        printf("22. 동호회 광고 관리                              \n");
		printf(" 선택 : >>");
		num=numinput();
		printf("\n");
		xy(60,num+3);
		if (num== 0) break;
        else if (num== 1) findmenu(0);
		else if (num== 2) sys_tem("bin/ed_menu %s %s",now_id,club_go.code);
		else if (num== 3) link_menu();
        else if (num== 4) xedit("%s/system/zbtotal.dat",club_go.dir);
        else if (num== 5) xedit("%s/logo/_ad.dat",club_go.dir);
        else if (num== 6) job_notice();
        else if (num== 7) set_no_code();
        else if (num== 8) set_no_logo();
        else if (num== 9) ccfg.wmalltype =get_num(ccfg.wmalltype , 2);
		else if (num==11) ccfg.mode=gtt_bit(ccfg.mode,11);
		else if (num==12) ccfg.mode=gtt_bit(ccfg.mode,12);
		else if (num==13) ccfg.mode=gtt_bit(ccfg.mode,13);
        else if (num==14) ccfg.logomode=gtt_bit(ccfg.logomode,1);
        else if (num==15) ccfg.logomode=gtt_bit(ccfg.logomode,2);
        else if (num==16) ccfg.logomode=gtt_bit(ccfg.logomode,3);
        else if (num==21) sys_tem("bin/xfrm 1 '%s' '%s' '%s'",club_go.code,go.code,pf.id);
        else if (num==22) sys_tem("bin/xfrm 2 '%s' '%s' '%s'",club_go.code,go.code,pf.id);
	}
}


//동호회 회원 등급 조정
set_cpf_type()
{
	FILE *fp1;
	int i;
	char buffid[255];
	char buff[255];
	while (1) {
		xy(1,20);printf(" 등급조정할 회원의 아이디 또는 이름을 입력하세요 >>");
		lineinput(buffid, 8);
		if (buffid[0]==0) break;
		fp1=fopen(id_file,"r+");   //동호회 아이디화일
		if (fp1==NULL) break;
		i=0;
		while (1) {
			fseek(fp1,i*sizeof_cpf,SEEK_SET);
			if (!fread(&cpf,sizeof_cpf,1,fp1)) break;
			if (is_same(cpf.id,buffid)||is_same(cpf.name,buffid)) {
				cls();
                disp_cpf(cpf,who);
				xy(1,20);
				if (yesno("이 동호회 회원의 등급을 조정할까요? (Y/n) >>",Yes)) {
					xy(62, 3);
					lineinput(buff, 2);
					if (buff[0]!=0) cpf.type=atoi(buff);
					fseek(fp1,i*sizeof_cpf,SEEK_SET);
					fwrite(&cpf,sizeof_cpf,1,fp1);
					break;
				}
			}
			i++;
		}
		if (fp1!=NULL) fclose(fp1);
	}
}


//미가입 회원을 가입시킨다.
free_cwait(int mode)
{
	char buff[200];
	FILE *fp;

	if (mode==0) {
		xy(1,20);
		while (1) {
			printf("\n 가입시킬회원 ID >>");
			lineinput(buff,8);
			if (is_same(buff,"q")||is_same(buff,"p")||buff[0]==0) return;
            if (!read_pf4(buff)) {printf(" ->해당아이디가 없습니다.");continue;}
			printf("  ->%s(%s) 등급:%d\n",pf2.id,pf2.name,pf.level);
			if (read_cpf(buff)) {printf("  ->이미 가입되어 있는 아이디입니다.\n",pf2.id);continue;}
			strcpy(cpf.id	,pf2.id);strcpy(cpf.name ,pf2.name);
			strcpy(cpf.memo1,"");strcpy(cpf.memo2,"");strcpy(cpf.memo3,"");
			strcpy(cpf.memo4,"");strcpy(cpf.memo5,"");
			time(&cpf.inday);time(&cpf.login);time(&cpf.logout);
			cpf.countu=0;cpf.countw=0;cpf.countd=0;cpf.all_log=0;cpf.all_time=0;
			cpf.type=2; 		//회원대기	0:거부/탈퇴 1:손님 2:대기 3:회원-
			printf(" 가입시킬회원의 등급 (0-16)>>");
			cpf.type=get_num(cpf.type,2);
			if (cpf.type>16) cpf.type=16;
			if (cpf.type==0) {printf(" ->취소됨");continue;}

			fp=fopen(id_file,"a+");
			if (fp==NULL) fp=fopen(id_file,"w+");
			if (fp!=NULL) {
				cpf.num=ftell(fp)/sizeof_cpf+1;
				fwrite(&cpf,sizeof_cpf,1,fp);
				fclose(fp);
			}
			printf(" %d 등급으로 가입 됨",cpf.type);
		}
	}
	else {
		fp=fopen(id_file,"r+");
		if (fp!=NULL) {
			while (fread(&cpf,sizeof_cpf,1,fp)>0) {
				if (cpf.type==1) {
					disp_cpf(cpf,who);
					printf("선택 (Y:가입허가 N:거부 ENTER:보류 q:작업중지) 선택>>");
					lineinput(buff,2);
					check_han(buff);
					buff[0]=tolower(buff[0]);
					if		(buff[0]=='y') {            //아이디로 저장
						cpf.type=3;
						save_cpf(cpf.id);					 //저장함
					}
					else if (buff[0]=='n') {
						cpf.type=0;
						save_cpf(cpf.id);					 //저장함
					}
					else if (buff[0]=='q') break;       //끝
				}
			}
			fclose(fp);
		}
	}
	check_total();
}


//동호회 회원 조회,변경
edit_cpf(int who)
{
	FILE *fp1;
	int i,n,linenum;
	char buff[255];
	char st[10];
	struct tm *tp;
	time_t today;
	int key;

	key=Yes;
	top=1;
	strcpy(st,"   ");
	top=1;
	check_total();
	if (ntotal1<1) {
		mess("현재 회원이 한명도 없습니다.");
		return;
	}

	while (key==Yes) {
		clrscr();
		printf("회원 정보     총%4d명\n",ntotal1);
		printfL2();
		printf("번호 아 이 디 타입 분류 가입일자 최종접속 횟수 쓰기 등록 다운  총이용\n");
		//		1234 12345678	0  대기 99/99/99 99/99/99 9999 9999 9999 9999 9999:99
		printfL2();

		if (top<1) top=1;
		if (top>ntotal1) top=ntotal1;
		fp1=fopen(id_file,"r+");   //동호회 아이디화일
		for (i=top;i<top+15;i++) {
			fseek(fp1,(i-1)*sizeof_cpf,SEEK_SET);
			if (fread(&cpf,sizeof_cpf,1,fp1)>0) {
				if (cpf.type<0||cpf.type>19) cpf.type=0;
				printf("%4d %-8s %2d %8s ",
				cpf.num,cpf.id,cpf.type,atype[cpf.type]);
				tp=localtime(&cpf.inday);
				printf("%02d/%02d/%02d ",
                tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
				tp=localtime(&cpf.logout);
				printf("%02d/%02d/%02d ",
                tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
				printf("%4d %4d %4d %4d\n",
				cpf.all_log,cpf.countw,cpf.countu,cpf.countd);
			}
			else printf("\n");
		}
		printfL2();
		if (fp1!=NULL) fclose(fp1);

		printf("명령 (번호:확인 E:수정 L:등급 D:삭제 S:검색 T:처음 B:이전 R:회원정리) >>");
		lineinput(buff,5);
		printf("\n");
		check_cmd(buff);
		nn=mmn[1];
		if (mmn[0]>0&&mmn[0]<=ntotal1) {
			if (read_cpfn(mmn[0])) {
				if (disp_cpf(cpf,who)) {
					pressenter();
					top=mmn[0];
				}
			}
			continue;
		}
		switch (**mmc) {
		case 'e' :
					if (read_cpfn(nn)) { //해당 번호 아이디 읽어 cpf에 저장
						sedit_cpf();
						save_cpf(cpf.id);
					}
					break;
		case 'd' :
					if (read_cpfn(nn)) { //해당 번호 아이디 읽어 cpf에 저장
						cpf.type=0;
						save_cpf(cpf.id);
					}
					break;
		case 'r' :  ssort_cpf();    break;
		case 'l' :  stype_cpf(nn);  break;
		case 'p' :
		case 'x' :
		case 'q' :  key=No      ; break;
		case 's' :  ssearch_cpf(); break;
		case 't' :  top=1       ; break;
		case 'a' :
		case 'b' :  top-=15     ; break;
		default  :	top+=15 	; break;
		}
	}
	check_total();	//총수 파악
}



stype_cpf(int nn)
{
	if (read_cpfn(nn)) { //해당 번호 아이디 읽어 cpf에 저장
		cls();
		disp_cpf(cpf,who);
		xy(62, 3);
		cpf.type=get_num(cpf.type,2);
		save_cpf(cpf.id);
	}
}


//cpf의 내용을 수정한다. 번호를 선택함
sedit_cpf()
{
	FILE *fp;
	char buff[140];
	int n1,n2,n3,n;
	struct tm *tp;
    CPF oldcpf;

    oldcpf=cpf;
	n=1;
	cls();
    while (1) {
        if (!view("%s/frm/pf/cpfe.scr",club_go.dir)) view("frm/pf/cpfe.scr");
        xy(14, 3);lineinput(buff, 8);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.id,buff);
        xy(38, 3);lineinput(buff, 8);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.name,buff);
        xy(62, 3);lineinput(buff, 2);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.type=atoi(buff)    ;
        xy(64, 5);lineinput(buff, 6);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.all_log=atoi(buff);
        xy(56, 6);lineinput(buff, 6);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.countw=atoi(buff)    ;
        xy(64, 6);lineinput(buff, 6);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.countu=atoi(buff)    ;
        xy(72, 6);lineinput(buff, 6);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.countd=atoi(buff)    ;
        xy(38, 7);lineinput(buff, 9);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.all_time=atoi(buff)     ;
        xy(14,13);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo1,buff);
        xy(14,14);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo2,buff);
        xy(14,15);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo3,buff);
        xy(14,16);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo4,buff);
        xy(14,17);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo5,buff);
        del_esc(cpf.memo1)  ;
        del_esc(cpf.memo2)  ;
        del_esc(cpf.memo3)  ;
        del_esc(cpf.memo4)  ;
        del_esc(cpf.memo5)  ;
        break;
    }
    if (is_same(buff,"q")) cpf=oldcpf;
}



//검색
ssearch_cpf()
{
	FILE *fp;
	char buff[140],buff2[80];
	int mode;
	int key,found;
	CPF cpf2;

	printf("1.ID 2.성명 3.회원구분 0.취소 >>");
	lineinput(buff,2);
	mode=atoi(buff);
	if (mode==0) return;
	printf("검색할 문자를 입력하세요. >>");
	lineinput(buff,20);
	printf("\n");
	if (buff[0]==0) return;

	fp=fopen(id_file,"r+");
	key=Yes;
	found=No;
	while (key==Yes&&fread(&cpf2,sizeof_cpf,1,fp)>0) {
		found=No;
		switch (mode) {
		case (1) : found=stristr(cpf2.id,buff)		  ;break;
		case (2) : found=stristr(cpf2.name,buff)	  ;break;
		case (3) : if (cpf2.type==buff[0]-'0') found=Yes  ;break;
		default  : break;
		}
		if (found) {
			disp_cpf(cpf2,who);
			if (yesno("이 회원이 맞습니까? (Y:해당자료로 이동 N:계속검색)>>",Yes)) {
				found=Yes;
				key=No;
			}
		}
	}
	if (fp!=NULL) fclose(fp);
	if (found) top=cpf2.num;
}


//회원 정리  (type=0인 회원을 정리함);
ssort_cpf()
{
	char buff[140];
	char id_file2[140];
	FILE *fp1,*fp2;
	int n;

	if (!yesno("\n\n회원등급이 0인 회원(불가)을 정리할까요? (y/N)",No)) return;

	printf("\n");
	strcpy(id_file2,id_file);
	strcat(id_file2,".new");

	fp1=fopen(id_file,"r+");
	fp2=fopen(id_file2,"w");
	n=1;
	while (fread(&cpf,sizeof_cpf,1,fp1)>0)
		if (cpf.type>0) {
			cpf.num=n;
			fwrite(&cpf,sizeof_cpf,1,fp2);
			n++;
		}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (is_file(id_file2)) {
		unlink(id_file);
		rename(id_file2,id_file);
	}
}


//총회원 수 파악
check_total()
{
	FILE *fp;
	CPF cpf2;
	nwait1=0;
	ntotal1=0;
	ntotal2=0;
	ntotal3=0;
	fp=fopen(id_file,"r+");
	if (fp!=NULL) {
		fseekend(fp);
		ntotal1=ftell(fp)/sizeof_cpf;
		rewind(fp);
		while (fread(&cpf2,sizeof_cpf,1,fp)) {
			if		(cpf2.type==0) nwait0++;
			else if (cpf2.type==1) nwait1++;
			else if (cpf2.type>13) ntotal2++;
			if (time_day(cpf2.inday)<7) ntotal3++;
		}
	}
	if (fp!=NULL) fclose(fp);
}



disp_ctype()
{
	int i;
	cls();
	printf("           동호회 회원 등급별 환경설정 \n");
	printfL2();
	printf(" 등급    등급이름\n");
	printfL2();
	for (i=0;i<20;i++) {
		disp_ctype2(i);
	}
}


disp_ctype2(int i)
{
		xy(1,i+5);
		printf("  %2d    %-8s\n",i,actype[i]);
}


sys_back(char *dir)
{
	struct tm *tp;
	time_t today;
	int now_day;
	char filename1[100];
	char filename2[100];
	char buff[100];
	struct stat stbuf;

	time(&today);	//오늘의 날짜 지정
	tp = localtime(&today);
	now_day=tp->tm_mday;

	sprintf(buff,"%s/sys_back",dir);check_dir(buff);
	sprintf(filename1,"%s/sys_back/id%02d.lzh",dir,now_day);
	sprintf(filename2,"%s/sys_back/menu%02d.lzh",dir,now_day);
	//마지막으로  데이타를 수정한시간
	if (is_old_file(filename1,24*60*60)) {
		sprintf(buff,"%s/sys_back",dir);
		check_dir(buff);
		printf("\n\n");
		printf(" ID  -> %s 에 백업합니다.\n",filename1);
		printf(" 메뉴-> %s 에 백업합니다.\n",filename2);
		sprintf(buff,"%s/system/clubid.dat",dir);
		if (is_file(buff)) {
			sprintf(buff,"lha aq %s %s/system/clubid.dat &",filename1,dir);
			sys_tem2(buff);
		}
		sprintf(buff,"%s/system/menu.dat",dir);
		if (is_file(buff)) {
			sprintf(buff,"lha aq %s %s/system/menu.dat &",filename2,dir);
			sys_tem2(buff);
		}
	}
}

//현재의 동호회의 메뉴와 메인을 링크시킨다.
link_menu()
{
	int i,found;
	char buff[400];
	char buff1[50];
	char buff2[50];
	char file1[300];
	char file2[300];
	char file3[300];
	char file4[300];
	char olddir[280];
	if (is_file("system/no_del.dat")) return;

	getcwd(olddir,sizeof(olddir));		//현재의 디렉토리

	xy(1,22);
	printf("\n\n\n\n\n\n\n링크시킬 동호회의 메뉴코드 >>");
	lineinput(buff1,9); if (buff1[0]==0) return;
	printf("\n링크시킬 메인의   메뉴코드 >>");
	lineinput(buff2,9); if (buff2[0]==0) return;

	printf("\n");
	if (!good_filename(buff1)||!good_filename(buff2) ) {
		printf("적당한 가기코드가 아닙니다.\n");
		getchar();
		return;
	}
	sprintf(buff,"menu/%s.tit",buff2);
	if (!is_file(buff)) {
		printf("%s 화일을 찾을수 없습니다.\n",buff);
		getchar();
		return;
	}

	if (!find_go(buff1)) {
		mess("%s 가기코드가 동호회에 없습니다.\n",buff1);
		return;
	}

	sprintf(buff,"%s/menu/%s.tit",club_go.dir,buff1);
	if (is_file(buff)) {
		printf("\n동호회 에 %s 화일이 있습니다 먼저 삭제할까요? >>",buff);
		if (yesno("",No)==No) return;
	}

	sprintf(file2,"%s/menu/%s",olddir,buff2);
	sprintf(file1,"%s/menu/%s",club_go.dir,buff1);
	sprintf(file4,"%s/logo/%s",olddir,buff2);
	sprintf(file3,"%s/logo/%s",club_go.dir,buff1);

	sys_tem("ln -sf %s.tit %s.tit",file2,file1);
	sys_tem("ln -sf %s.txt %s.txt",file2,file1);
	sys_tem("ln -sf %s.log %s.log",file4,file3);
	mess("메인 %s -> 동호회 %s 링크",file2,club_go.code);
}


//현재의 동호회에 번호로 아이디를 찾는다.  ->cpf에 저장
read_cpfn(int num)
{
	FILE *fp;
	int ret=No;
	fp=fopen(id_file,"r+");
	if (fp==NULL) return No;   //아이디 없음
	fseek(fp,(num-1)*sizeof_cpf,SEEK_SET);
	if (fread(&cpf,sizeof_cpf,1,fp)>0&&num==cpf.num) ret=Yes;
	if (fp!=NULL) fclose(fp);
	return ret;
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
						if (fp!=NULL) fclose(fp);
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
							if (fp!=NULL) fclose(fp);
							if (fp2!=NULL) fclose(fp2);
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

job_idset()
{
	int n;
	char buff[200];
	while (1) {
		cls();
		printfL2();
		printf("            제한 설정 가능 회원 지정\n");
		printfL2();
		printf("  1.  .add(광고편집)  기능 가능 아이디          (system/edit_ad.id)\n");
		printfL2();
		printfL2();
		n=numselect();
		xy(60,n+3);
             if (n<=0)  break;
		else if (n==1)	{
			sprintf(buff,"%s/system/edit_ad.id",club_go.dir);
			xedit(buff);
		}
	}
}


job_money()
{
	int n;
	while (1) {
		cls();
		printf("  ###  사이버머니 관련 작업  ###\n");
		printf("  1. 동호회 사이버머니 BANK 회원                :%s\n",ccfg.bankman);
		printfL2();
		n=numselect();
		xy(60,n);
        if (n<=0) break;
		if (n==1) get_str(ccfg.bankman,8);
	}
}


job_notice()
{
	while (1) {
		cls();
		print_(1,"공지사항 설정");
		printf(" 1. 일반회원 접속시 공지사항 출력      :%s\n",rbit(ccfg.notice,0,"출력","통과"));
		printf(" 2. 손님     접속시 공지사항 출력      :%s\n",rbit(ccfg.notice,1,"출력","통과"));
		printf(" 3. 공지사항 광고 출력                 :%s\n",rbit(ccfg.notice,4,"함","안함"));
		printf(" 4. 공지사항 출력모양 (0~9)            :%d\n",ccfg.noticet);
		printf(" 5. 공지사항 출력시작 위치 X           :%d\n",ccfg.noticex);
		printf(" 6. 공지사항 출력시작 위치 Y           :%d\n",ccfg.noticey);
		printf(" 7. 공지사항 출력시작 줄길이           :%d\n",ccfg.noticeline);
		n=numselect();xy(60,n+1);
        if (n<= 0) break;
		if (n== 1) ccfg.notice	   =gtt_bit(ccfg.notice ,0);
		if (n== 2) ccfg.notice	   =gtt_bit(ccfg.notice ,1);
		if (n== 3) ccfg.notice	   =gtt_bit(ccfg.notice ,4);
		if (n== 4) ccfg.noticet    =get_num(ccfg.noticet,2);
		if (n== 5) ccfg.noticex    =get_num(ccfg.noticex,2);
		if (n== 6) ccfg.noticey    =get_num(ccfg.noticey,2);
		if (n== 7) ccfg.noticeline =get_num(ccfg.noticeline,2);
	}
}


check_initdir()
{
}

findmenu(int mode)
{
	char buff[300];
	unlink(TMP_FILE2);
    sys_tem("bin/findmenu %d '%s' '%s' ",mode,club_go.code,TMP_FILE2);
    file2buff(auto_cmd,TMP_FILE2,30);
}


view_daycpf()        //일별 가입현황
{
    FILE *fp,*fp2;
    time_t indate;
    int inday;
    int i,n,count1,count2;
    fp=fopen(id_file,"r");
    i=0;
    printf("\n");
    indate=0;
    inday=count1=count2=0;
    fp2=fopen(TMP_FILE,"w");
    while (fread(&cpf,sizeof_cpf,1,fp)) {
        n=time_num(cpf.inday);
        if (inday>0&&inday!=n) {
            fprintf(fp2,"%s %6d명 가입  누적%6d명\n",datestr(35,indate),count1,count2);
            count1=0;
        }
        indate=cpf.inday;
        inday=n;
        count1++;
        count2++;
    }
    fclose(fp);
    fclose(fp2);
    view_text(TMP_FILE);
}
