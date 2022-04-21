///////////////////////////////////////////////////////////////////////////////
// XHOST 개발:김성대 1996.10.15 부터 개발시작함
///////////////////////////////////////////////////////////////////////////////
#include "bbs.h"
char filename5[300];
char now_id[100];
char ind_file_name[100],ind_file_idno[100];
main(int argc, char **argv)
{
    int i;
    char cmd[100];
    strcpy(cmd,"");
    if (argc==2) strcpy(cmd,argv[1]);
    else if (argc>2) strcpy(cmd,argv[2]);
    if (argc>2&&is_char(argv[2],'/')) {chdir(argv[2]);strcpy(cmd,argv[1]);}
    if (argc>2&&is_char(argv[1],'/')) {chdir(argv[1]);strcpy(cmd,argv[2]);}
	if (!is_file(ID_FILE)) {
		chdir("/bbs");
		if (!is_file(ID_FILE)) {
			chdir("/xhost");
			if (!is_file(ID_FILE)) {
				chdir("/bbs/bbs1");
			}
		}
	}
	findport(tty);
	read_cfg();
	set_tmpfile();
	read_ajob();	  //직업을 읽어옴
    for (i=1;i<argc;i++) {
        if (is_same(argv[i],"reindex_all")) {
            make_ind(1,"make_ind_all");
            return;
        }
    }
	strcpy(now_id,"...");
	if (argc<2) {
		if (!check_pass("zz",8,"PASS >>")) return;
		strcpy(now_id,"root");
	}
	strcpy(ind_file_name,"id/id_name.ind");
	strcpy(ind_file_idno,"id/id_idno.ind");
	if (argc>1&&strlen(argv[1])>2&&strlen(argv[1])<=8) strcpy(now_id,argv[1]);

	who=0;
		 if (is_same(now_id,"root"))    who=5;
	else if (is_same(cfg.sysop,now_id)) who=4;
	else if (pf.type==19)				who=4;	//root
	else if (pf.type==18)				who=3;	//부시삽
	else if (pf.type==17)				who=2;	//운영진

         if (argc>2&&is_same(cmd,"set_cfindpf"))   set_cfindpf(argv[3]);
    else if (argc>2&&is_same(cmd,"set_idbackup"))  set_idbackup(0);
    else if (argc>2&&is_same(cmd,"set_idrecover")) set_idrecover(0);
    else if (argc>2&&is_same(cmd,"set_idrecover2")) set_idrecover2(0);
    else if (argc>2&&is_same(cmd,"set_idrecover3")) set_idrecover2(1);
    else if (argc>2&&is_same(cmd,"set_idcheck"))   {if (set_idbackup()) set_idrecover(0);}
    else if (argc>2&&is_same(cmd,"set_idindex"))   make_name_index(0);
    else if (argc>2&&is_same(cmd,"set_search1"))   check_data(1);
    else if (argc>2&&is_same(cmd,"set_search2"))   check_data(2);
    else if (argc>2&&is_same(cmd,"set_search3"))   check_data(3);
    else if (argc>2&&is_same(cmd,"set_search"))    check_data(1);
    else if (argc>2&&is_same(cmd,"set_userfind"))  user_find(1);
    else if (argc>2&&is_same(cmd,"set_userfind1")) user_find(1);
    else if (argc>2&&is_same(cmd,"set_userfind2")) user_find(2);
    else if (argc>2&&is_same(cmd,"set_userfind3")) user_find(3);
	else pf_util();
}

pf_util()
{
	int num;
	strcpy(filename5,"");
	while(1) {
		cls();
		print_(1,"회원유틸리티");
		printf("   1. 전체 회원 목록\n");
		printf("   2. 아이디/성명 검색             \n");
        printf("   3. 학교/직장   검색             \n");
        printf("   4. 취미        검색             \n");
        printf("   5. 주민 등록 번호 검색          \n");
        printf("   6. 전체 회원 자료 검색          \n");
        printf("   7. 전체 통계 확인               \n");
        printf("   8. 일별 회원 가입현황           \n");
        printf("   9. 가입대기 회원 가입처리  :%d명대기\n",count_rec(GUEST_FILE,sizeof_pf));
        printf("  10. 가입대기 회원 선택 가입처리\n");
        printf("  11. 최근 가입순 회원 조회        \n");
        printf("  12. 최근  1년간 가입 통계        \n");
        printf("  13. 회원 주소록 출력\n");
		printf("  14. 회원자료 외부 추가/출력\n");
		printf("  99. 검색 인덱스 갱신    ID:%dKb NAME:%dKb\n",
			file_size(ind_file_name)/1024,file_size(ind_file_idno)/1024);
		num=numselect();
		if (num<= 0) break;
		if (num== 1) sys_tem("bin/pfinfo '%s'",now_id);
		if (num== 2) check_data(1);
        if (num== 3) user_find(2);
        if (num== 4) user_find(3);
        if (num== 5) check_data(2);
        if (num== 6) check_data(3);
        if (num== 7) check_stat();
        if (num== 8) view_daypf();
        if (num== 9) sys_tem("bin/guest 999");
        if (num==10) sys_tem("bin/guest 998");
        if (num==11) check_new(who);
        if (num==12) check_day(who);
        if (num==13) id_addr();
        if (num==14) pf_inout();
        if (num==91) {
            if (yesno2("ID 백업(Y/n)>>",Yes)) set_idbackup(1);
        }
        if (num==92) {
            if (yesno2("ID 복원(Y/n)>>",Yes)) set_idrecover(1);
        }
        if (num==93) {
            if (yesno2("ID 복원+백업(Y/n)>>",Yes)) set_idrecover2(1);
        }
		if (num==99) make_name_index(1);
	}
}

id_addr()
{
	FILE *fp1,*fp2;
	char buff[300];
	char filename1[100];
	char filename2[100];
	int num;

	//회원 ID 파일 지정
	//출력 결과 파일 지정
	strcpy(filename2,"pf1.txt");
	fp1=fopen(ID_FILE,"r");
	if (fp1==NULL) {
		return;
	}
	fp2=fopen(filename2,"w");
	if (fp2==NULL) {
		printf(" 출력 결과 파일(%s)을 만들수 없습니다.\n",filename2);
		return;
	}
	num=0;
	while(fread(&pf,sizeof_pf,1,fp1)) {
		if (pf.type<0||pf.type>=18||strlen(pf.id)<3) continue;
		//현재회원을 화면으로 출력한다.
//		  printf("%6d. %-8s %-8s %-30.30s \n",pf.num+1,pf.id,pf.name,pf.addr);
		num++;
		//파일로 자료를 출력한다.
		fprintf(fp2,"\n");
		fprintf(fp2,"%s\n%s\n%s 귀하\n",pf.pos1,pf.addr,pf.name);
	}
	fclose(fp1);
	fclose(fp2);
	view_text(filename2);
}


pf_inout()
{
	int n;
	while (1) {
		cls();
		print_(1,"회원자료 추가입력");
		printf("  1. XFORMAT 외부 TEXT 파일 전체 추가\n");
		printf("  2. XFORMAT 외부 TEXT 파일 전체 출력\n");
		printf("  3. 외부 ID파일 전체 추가\n");
		printf("  4. 외부 ID파일 지정ID 추가\n");
		printf("  5. 신규 회원 추가          \n");
		printf("  9. 인덱스파일 재정리\n");
		n=numselect();
		if (n<= 0) break;
		if (n== 1) xformat_in();
		if (n== 2) xformat_out();
		if (n== 3) append_pf1();
		if (n== 4) append_pf2();
		if (n== 5) add_new_pf();
		if (n== 9) {
			if (yesno("\n 인덱스를 새로 정리할까요? (y/N)>>",No)) {
                make_ind(0,"make_ind_all");
			}
		}
	}
}

xformat_in()
{
	FILE *fp1;
	char buff[30];
	char buff1[300];
	int found,n;
	printf("추가할 ID가 있는 XFORMAT TEXT 데이타 파일\n>>");
	dosinput(filename5,70);
	if (!is_file(filename5)) {mess(" %s 파일이 없습니다.",filename5);return;}
	printf("파일 크기:%d B \n",file_size(filename5));
	if ((fp1=fopen(filename5,"r"))==NULL) return;
	clear_pf();
	while (fgets(buff,200,fp1)!=NULL) {
		strcpy(buff1,&buff[9]);del_enter(buff1);all_trim(buff1);
		if (stristr(buff,"_[[완료[[")) {
			t_save_pf();
			clear_pf();
		}
		else if (stristr(buff,"_[[순번[[")) pf.idnum=atoi(buff1);
		else if (stristr(buff,"_[[아이[[")) strcpy(pf.id,buff1);
		else if (stristr(buff,"_[[암호[[")) strcpy(pf.passwd,buff1);
		else if (stristr(buff,"_[[이름[[")) strcpy(pf.name,buff1);
		else if (stristr(buff,"_[[별명[[")) strcpy(pf.nick,buff1);
		else if (stristr(buff,"_[[생일[[")) pf.birthday=str2date(buff1);
		else if (stristr(buff,"_[[양음[[")) pf.moon=stristr("2m음",buff1)?2:1;
		else if (stristr(buff,"_[[성별[[")) pf.moon=buff1[0];
		else if (stristr(buff,"_[[주민[[")) strcpy(pf.id_no,buff1);
		else if (stristr(buff,"_[[주소[[")) {strchng2(buff1,"  "," ");strcpy(pf.addr,buff1);}
		else if (stristr(buff,"_[[우편[[")) strcpy(pf.pos1,buff1);
		else if (stristr(buff,"_[[직업[[")) pf.job=atoi(buff1);
		else if (stristr(buff,"_[[직장[[")) {strchng2(buff1,"  "," ");strcpy(pf.offi,buff1);}
		else if (stristr(buff,"_[[직주[[")) {strchng2(buff1,"  "," ");strcpy(pf.add2,buff1);}
		else if (stristr(buff,"_[[직전[[")) {strchng2(buff1,"  "," ");strcpy(pf.otel,buff1);}
		else if (stristr(buff,"_[[직우[[")) strcpy(pf.pos2,buff1);
		else if (stristr(buff,"_[[가입[[")) pf.inday=str2date2(buff1);
		else if (stristr(buff,"_[[최종[[")) pf.logout=str2date2(buff1);
		else if (stristr(buff,"_[[등급[[")) pf.type=atoi(buff1);
		else if (stristr(buff,"_[[레벨[[")) pf.level=atoi(buff1);
		else if (stristr(buff,"_[[다운[[")) pf.down =atoi(buff1);
		else if (stristr(buff,"_[[캐시[[")) pf.cash =atoi(buff1);
		else if (stristr(buff,"_[[수다[[")) pf.countd=atoi(buff1);
		else if (stristr(buff,"_[[수쓰[[")) pf.countw=atoi(buff1);
		else if (stristr(buff,"_[[수업[[")) pf.countu=atoi(buff1);
		else if (stristr(buff,"_[[양음[[")) pf.moon =atoi(buff1);
		else if (stristr(buff,"_[[우수[[")) pf.good =atoi(buff1);
		else if (stristr(buff,"_[[총사[[")) pf.all_time =atoi(buff1);
		else if (stristr(buff,"_[[총접[[")) pf.all_log  =atoi(buff1);
		else if (stristr(buff,"_[[월사[[")) pf.month_time=atoi(buff1);
		else if (stristr(buff,"_[[월접[[")) pf.month_log =atoi(buff1);
		else if (stristr(buff,"_[[지역[[")) pf.localmenu =atoi(buff1);
		else if (stristr(buff,"_[[전화[[")) {strchng2(buff1,"  "," ");strcpy(pf.tele,buff1);}
		else if (stristr(buff,"_[[호출[[")) {strchng2(buff1,"  "," ");strcpy(pf.bibi,buff1);}
		else if (stristr(buff,"_[[삐삐[[")) {strchng2(buff1,"  "," ");strcpy(pf.bibi,buff1);}
		else if (stristr(buff,"_[[휴대[[")) {strchng2(buff1,"  "," ");strcpy(pf.hand,buff1);}
		else if (stristr(buff,"_[[참고[[")) strcpy(pf.memo1,buff1);
		else if (stristr(buff,"_[[참01[[")) strcpy(pf.memo1,buff1);
		else if (stristr(buff,"_[[참02[[")) strcpy(pf.memo2,buff1);
		else if (stristr(buff,"_[[참11[[")) strcpy(pf.user1,buff1);
		else if (stristr(buff,"_[[참12[[")) strcpy(pf.user2,buff1);
		else if (stristr(buff,"_[[참13[[")) strcpy(pf.user3,buff1);
		else if (stristr(buff,"_[[참14[[")) strcpy(pf.user4,buff1);
		else if (stristr(buff,"_[[초가[[")) strcpy(pf.gofirst,buff1);
		else if (stristr(buff,"_[[설01[[")) pf.mymode    =atoi(buff1);
		else if (stristr(buff,"_[[설03[[")) pf.offmin    =atoi(buff1);
		else if (stristr(buff,"_[[설04[[")) pf.paymode   =atoi(buff1);
		else if (stristr(buff,"_[[설05[[")) pf.cashmode  =atoi(buff1);
		else if (stristr(buff,"_[[설06[[")) pf.today     =atoi(buff1);
		else if (stristr(buff,"_[[설07[[")) pf.myflag    =atoi(buff1);
		else if (stristr(buff,"_[[설08[[")) pf.loginmode =atoi(buff1);
		else if (stristr(buff,"_[[설09[[")) pf.istat     =atoi(buff1);
		else if (stristr(buff,"_[[설10[[")) pf.itype     =atoi(buff1);
		else if (stristr(buff,"_[[중지[[")) break;
	}
	t_save_pf();
	fclose(fp1);
}


xformat_out()
{
	FILE *fp1,*fp2;
	int n,i,num;
	if ((fp1=fopen(ID_FILE,"r"))==NULL) return;
	if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
	num=0;
	while(fread(&pf,sizeof_pf,1,fp1)) {
		if (pf.type<1||strlen(pf.id)<3) continue;
		fprintf(fp2,"_[[레코[[%d\n",pf.num);
		fprintf(fp2,"_[[순번[[%d\n",pf.idnum);
		fprintf(fp2,"_[[아이[[%s\n",pf.id   );
		fprintf(fp2,"_[[이름[[%s\n",pf.name   );
		if (strlen(pf.nick		 )>0) fprintf(fp2,"_[[별명[[%s\n",pf.nick       );
		if (strlen(pf.passwd	 )>0) fprintf(fp2,"_[[암호[[%s\n",pf.passwd     );
		if (pf.type 			 !=0) fprintf(fp2,"_[[등급[[%d\n",pf.type       );
		if (strlen(pf.email 	 )>0) fprintf(fp2,"_[[영아[[%s\n",pf.email      );
        if (strlen(pf.tmail      )>0) fprintf(fp2,"_[[대메[[%s\n",pf.tmail      );
        if (strlen(pf.mailid     )>0) fprintf(fp2,"_[[이메[[%s\n",pf.mailid     );
		if (pf.birthday 		 !=0) fprintf(fp2,"_[[생일[[%s\n",datestr(1,pf.birthday));
		if (strlen(pf.id_no 	 )>0) fprintf(fp2,"_[[주민[[%s\n",pf.id_no        );
		if (pf.sex!=' '&&pf.sex!='m') fprintf(fp2,"_[[성별[[%c\n",pf.sex          );
		if (pf.moon 			 !=0) fprintf(fp2,"_[[양음[[%d\n",pf.moon         );
		if (pf.good 			 !=0) fprintf(fp2,"_[[우수[[%d\n",pf.good         );
		if (strlen(pf.pos1		 )>0) fprintf(fp2,"_[[우편[[%s\n",pf.pos1         );
		if (strlen(pf.tele		 )>0) fprintf(fp2,"_[[전화[[%s\n",pf.tele         );
		if (strlen(pf.bibi		 )>0) fprintf(fp2,"_[[호출[[%s\n",pf.bibi         );
		if (strlen(pf.hand		 )>0) fprintf(fp2,"_[[휴대[[%s\n",pf.hand         );
		if (strlen(pf.addr		 )>0) fprintf(fp2,"_[[주소[[%s\n",pf.addr         );
		if (pf.localmenu		 !=0) fprintf(fp2,"_[[지역[[%d\n",pf.localmenu    );
		if (pf.job				 !=0) fprintf(fp2,"_[[직업[[%d\n",pf.job          );
		if (strlen(pf.pos2		 )>0) fprintf(fp2,"_[[직우[[%s\n",pf.pos2         );
		if (strlen(pf.offi		 )>0) fprintf(fp2,"_[[직장[[%s\n",pf.offi         );
		if (strlen(pf.otel		 )>0) fprintf(fp2,"_[[직전[[%s\n",pf.otel         );
		if (strlen(pf.add2		 )>0) fprintf(fp2,"_[[직주[[%s\n",pf.add2         );
		if (pf.level			 !=0) fprintf(fp2,"_[[레벨[[%d\n",pf.level        );
		if (pf.down 			 !=0) fprintf(fp2,"_[[다운[[%d\n",pf.down         );
		if (pf.cash 			 !=0) fprintf(fp2,"_[[캐시[[%d\n",pf.cash         );
		if (pf.countd			 !=0) fprintf(fp2,"_[[수다[[%d\n",pf.countd       );
		if (pf.countw			 !=0) fprintf(fp2,"_[[수쓰[[%d\n",pf.countw       );
		if (pf.countu			 !=0) fprintf(fp2,"_[[수업[[%d\n",pf.countu       );
		if (pf.all_time 		 !=0) fprintf(fp2,"_[[총사[[%d\n",pf.all_time     );
		if (pf.all_log			 !=0) fprintf(fp2,"_[[총접[[%d\n",pf.all_log      );
		if (pf.month_time		 !=0) fprintf(fp2,"_[[월사[[%d\n",pf.month_time   );
		if (pf.month_log		 !=0) fprintf(fp2,"_[[월접[[%d\n",pf.month_log    );
		if (pf.inday			 !=0) fprintf(fp2,"_[[가입[[%s\n",datestr(8,pf.inday));
		if (pf.logout			 !=0) fprintf(fp2,"_[[최종[[%s\n",datestr(8,pf.logout));
		if (strlen(pf.memo1 	 )>0) fprintf(fp2,"_[[참고[[%s\n",pf.memo1        );
		if (strlen(pf.memo1 	 )>0) fprintf(fp2,"_[[참01[[%s\n",pf.memo1        );
		if (strlen(pf.memo2 	 )>0) fprintf(fp2,"_[[참02[[%s\n",pf.memo2        );
		if (strlen(pf.user1 	 )>0) fprintf(fp2,"_[[참11[[%s\n",pf.user1        );
		if (strlen(pf.user2 	 )>0) fprintf(fp2,"_[[참12[[%s\n",pf.user2        );
		if (strlen(pf.user3 	 )>0) fprintf(fp2,"_[[참13[[%s\n",pf.user3        );
		if (strlen(pf.user4 	 )>0) fprintf(fp2,"_[[참14[[%s\n",pf.user4        );
		if (strlen(pf.gofirst	 )>0) fprintf(fp2,"_[[초가[[%s\n",pf.gofirst      );
		if (pf.mymode			 !=0) fprintf(fp2,"_[[설01[[%d\n",pf.mymode       );
		if (pf.offmin			 !=0) fprintf(fp2,"_[[설03[[%d\n",pf.offmin       );
		if (pf.paymode			 !=0) fprintf(fp2,"_[[설04[[%d\n",pf.paymode      );
		if (pf.cashmode 		 !=0) fprintf(fp2,"_[[설05[[%d\n",pf.cashmode     );
		if (pf.today			 !=0) fprintf(fp2,"_[[설06[[%d\n",pf.today        );
		if (pf.myflag			 !=0) fprintf(fp2,"_[[설07[[%d\n",pf.myflag       );
		if (pf.loginmode		 !=0) fprintf(fp2,"_[[설08[[%d\n",pf.loginmode    );
		if (pf.istat			 !=0) fprintf(fp2,"_[[설09[[%d\n",pf.istat        );
		if (pf.itype			 !=0) fprintf(fp2,"_[[설10[[%d\n",pf.itype        );
		num++;
		fprintf(fp2,"_[[완료[[  %d\n",num);
	}
	fclose(fp1);
	fclose(fp2);
	view_text(TMP_FILE);
}



append_pf1()
{
	FILE *fp,*fp1,*fp2;
	char buff[30];
	int found,n;
	printf("추가할 ID가 있는 ID 데이타 파일(id.dat,id.txt)?\n>>");
	dosinput(filename5,70);
	if (!is_file(filename5)) {mess(" %s 파일이 없습니다.",filename5);return;}
	printf("파일 크기:%d B  자료수:%d\n",file_size(filename5),count_rec(filename5,sizeof_pf));
	if ((fp1=fopen(filename5,"r"))==NULL) return;
	if ((fp2=fopen(ID_FILE,"r+"))==NULL) fp2=fopen(ID_FILE,"w+");
	while (fread(&pf2,sizeof_pf,1,fp1)) {
		if (strlen(pf2.id)>2) {
			printf(" ID:%s 이름:%s 등급:%d  ",pf2.id,pf2.name,pf2.type);
			if (yesno("등록(Y/n)>>",Yes)) {
				fseekend(fp2);
				n=ftell(fp2)/sizeof_pf; //현재 레코드수
				pf2.num=n;				 //마지막번호에 추가함
				fseek(fp2,n*sizeof_pf,SEEK_SET);
//                fwrite(&pf2,sizeof_pf,1,fp2);
                gwrite(181,pf2,fp2);
				printf(" -->추가\n");
			}
			else printf(" -->취소\n");
		}
	}
	fclose(fp1);
	fclose(fp2);
}

append_pf2()
{
	FILE *fp;
	char buff[30];
	int found,n;

	printf("추가할 ID가 있는 ID 데이타 파일(id.dat,id.txt)?\n>>");
	dosinput(filename5,70);
	if (!is_file(filename5)) {mess(" %s 파일이 없습니다.",filename5);return;}
	printf("파일 크기:%d B  자료수:%d\n",file_size(filename5),count_rec(filename5,sizeof_pf));
	while (1) {
		printf("\n 추가 회원ID >>");
		lineinput(buff,8);
		if (is_quit(buff)) break;
		if ((fp=fopen(filename5,"r"))==NULL) break;
		found=No;
		while (fread(&pf2,sizeof_pf,1,fp)) {
			if (is_same(pf2.id,buff)||is_same(pf2.name,buff)) {
				printf(" ID:%s 이름:%s 등급:%d (Y/n)>>",pf2.id,pf2.name,pf2.type);
				if (yesno("",Yes)) {
					found=Yes;
					break;
				}
			}
		}
		fclose(fp);
		if (!found) {printf(" %s ID(이름)검색실패",buff);continue;}
		if ((fp=fopen(ID_FILE,"r+"))==NULL) fp=fopen(ID_FILE,"w+");
		if (fp==NULL) break;
		fseekend(fp);
		n=ftell(fp)/sizeof_pf; //현재 레코드수
		pf2.num=n;				 //마지막번호에 추가함
		fseek(fp,n*sizeof_pf,SEEK_SET);
        pf.id_no[14]=0;
//        fwrite(&pf2,sizeof_pf,1,fp);
        gwrite(181,pf2,fp);
		fclose(fp);
		printf("추가",pf2.id,pf2.name);
	}
}

add_new_pf()
{
	char buff[100];
	char buff1[100];
	char buff2[100];
	char buff3[100];
	char buff4[100];
	char buff5[100];
	char buff6[100];
	int n,i;
	printf("\n 새로 만들 회원 아이디 >>");lineinput(buff1,8);printf("\n");
	if (strlen(buff1)<3) return;
	if (read_pf(buff1)) {
		mess(" 해당 회원의 아이디가 이미 존재합니다.  %s(%s)",pf.id,pf.name);
		return;
	}
	printf("만들 회원 성명을  입력하세요  (8자) >>");lineinput(buff2,8);printf("\n");
	if (buff2[0]==0) strcpy(buff2,buff1);
	printf("만들 회원 등급을  입력하세요  (0-17)>>");lineinput(buff3,2);printf("\n");
	if (buff3[0]==0) strcpy(buff3,"1");
	printf("만들 회원 레벨을  입력하세요(0-5000)>>");lineinput(buff4,6);printf("\n");
	if (buff4[0]==0) strcpy(buff4,"100");
	printf("만들 회원 다운용량을 입력하세요 (MB)>>");lineinput(buff5,6);printf("\n");
	if (buff5[0]==0) strcpy(buff5,"5");
	printf("만들 회원 CASH를  입력하세요(냥)    >>");lineinput(buff6,9);printf("\n");
	if (buff6[0]==0) strcpy(buff6,"5000");
	clear_pf();
	time(&pf.inday);
	time(&pf.login);
	time(&pf.logout);
	strcpy(pf.id,buff1);
	strcpy(pf.name,buff2);
	strcpy(pf.nick,buff2);
	pf.type=atoi(buff3);
	pf.level=atoi(buff4);
	pf.down=atoi(buff5)*1000000;
	pf.cash=atoi(buff6)*1000000;
	strcpy(pf.passwd,pf.id);
	view_pfw2(pf,who);
	xy(1,20);
	if (yesno(" 신입회원 가입처리를 할까요? (Y/n)",Yes)) t_save_pf();
}



t_save_pf()
{
	FILE *fp;
	int n;
	del_esc(pf.id);del_space(pf.id);
	del_esc(pf.name);del_space(pf.name);
	if (strlen(pf.id)<3||strlen(pf.name)<3) return;
	if (pf.type>19||pf.type<1) pf.type=1;
	if ((fp=fopen(ID_FILE,"r+"))==NULL) fp=fopen(ID_FILE,"w+");
	if (fp==NULL) return;
	fseekend(fp);
	n=ftell(fp)/sizeof_pf; //현재 레코드수
	pf.num=n;				//마지막번호에 추가함
	fseek(fp,n*sizeof_pf,SEEK_SET);
	if (strlen(pf.nick)<3) strcpy(pf.nick,pf.id);
//    fwrite(&pf,sizeof_pf,1,fp);
    gwrite(181,pf,fp);
	fclose(fp);
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


set_idbackup(int mode)
{
	FILE *fp1,*fp2;
	char filename[100];
	int found=No;
	if ((fp1=fopen(ID_FILE,"r"))==NULL) return No;
	sprintf(filename,"id/id_bk%d.id",now_day()%10);
	if ((fp2=fopen(filename,"w"))==NULL) return No;
	while (fread(&pf,sizeof_pf,1,fp1)) {
        if (pf.type<0||pf.type>19) {
            if (mode==1) printf("ERR: %5d %-8s %-8s %3d %8d %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,datestr(0,pf.inday));
            found=Yes;
        }
		if (pf.type>0&&pf.type<=19) {
//            fwrite(&pf,sizeof_pf,1,fp2);
            gwrite(181,pf,fp2);
		}
	}
	fclose (fp1);
	fclose (fp2);
	return found;
}


set_idrecover(int mode)
{
	FILE *fp1,*fp2,*fp3;
	char filename[100],buff[100];
	int i,n,found=No;
	n=now_day()%10-1;


    for (i=0;i<12;i++) {
		if (n<0) n=9;
        sprintf(filename,"id/id_bk%d.id",n);
        if (is_file(filename)) break;
        n--;
	}
    if ((fp1=fopen(ID_FILE,"r+"))==NULL)   {
        if (mode==1) printf("ERR: %s OPEN\n",ID_FILE);
        return;
    }
    if ((fp2=fopen(filename,"r"))==NULL)   {
        if (mode==1) printf("BAK: %s OPEN\n",filename);
        return;
    }
    printf("ID : %s\n",ID_FILE);
    printf("BK : %s\n",filename);
	sprintf(buff,"id/id_er%d.id",now_day());
	if ((fp3=fopen(buff,"w"))==NULL) return;
	n=0;
	while (1) {
		fseek(fp1,n*sizeof_pf,SEEK_SET);
		if (!fread(&pf,sizeof_pf,1,fp1)) break;
        if (strlen(pf.id)>0&&strlen(pf.passwd)>0&&!is_esc(pf.id)&&pf.type<=0&&pf.level<=0&&pf.cash<=0) goto _cont2;
        if (pf.type>=0&&pf.type<=19&&!is_esc(pf.id)) {n++;continue;}
		if (pf.type==0&&strlen(pf.id)==0) {n++;continue;}
        if (pf.type>=0&&pf.type<=19) {n++;continue;}
        _cont2:
        if (mode==1) printf("ERR: %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
//        fwrite(&pf,sizeof_pf,1,fp3);  //이상ID 저장
        gwrite(182,pf,fp3);
		//백업에서 찾기
		fseektop(fp2);
        found=No;
		while (fread(&pf2,sizeof_pf,1,fp2)) {
			if (strlen(pf2.id)<3) continue;
			if (!is_same(pf.id,pf2.id)||!is_same(pf.name,pf2.name)) continue;
			//찾기성공 복구함
            if (mode==1) printf("ER1: %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
			pf2pf(pf2);
            if (mode==1) printf(" ->  %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
			fseek(fp1,n*sizeof_pf,SEEK_SET);
			pf.num=n;
//            fwrite(&pf,sizeof_pf,1,fp1);
            gwrite(183,pf,fp1);
            found=Yes;

			//다음자료 복구 읽기
			fseek(fp1,(n+1)*sizeof_pf,SEEK_SET);
			if (!fread(&pf,sizeof_pf,1,fp1)) break;
			if (strlen(pf.id)>2&&!is_esc(pf.id)&&!is_esc(pf.name)) break;
			if (strlen(pf.id_no)<1) break;
//          fwrite(&pf,sizeof_pf,1,fp3);  //이상ID 저장
            gwrite(183,pf,fp3);
			//주민등록으로 삭제된 자료 검색
			fseektop(fp2);
			while (fread(&pf2,sizeof_pf,1,fp2)) {
                if (is_same(pf.id_no,pf2.id_no)) {  //찾기성공
                    if (mode==1) printf("ER2: %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
					pf2pf2(pf2);
                    if (mode==1) printf(" ->  %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
					fseek(fp1,(n+1)*sizeof_pf,SEEK_SET);
					pf.num=n+1;
//                  fwrite(&pf,sizeof_pf,1,fp1);
                    gwrite(184,pf,fp1);
//                  fwrite(&pf,sizeof_pf,1,fp3);  //이상ID 저장
                    gwrite(185,pf,fp3);
					found=Yes;
					break;
				}
			}
			break;
		}
        if (!found) {
            if (mode==1) {
                printf("검색실패: %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
                if (yesno("등급조정(Y/n)>>",Yes)) {
                    fseek(fp1,n*sizeof_pf,SEEK_SET);
                    pf.num=n;
                    pf.type=1;
//                  fwrite(&pf,sizeof_pf,1,fp1);
                    gwrite(186,pf,fp1);
                }
            }
        }
		n++;
	}
    chmod(ID_FILE,0777);
	fclose (fp1);
	fclose (fp2);
	fclose (fp3);
}


set_idrecover2(int mode)
{
	FILE *fp1,*fp2,*fp3;
    char filename[100],buff[300],buff2[300];
    int i,n,nn,nnn,err=0,found=No;
    nnn=now_day()%10;

    for (nn=0;nn<10;nn++) {
        err=0;
        nnn--;
        if (nnn<0) nnn=9;
        sprintf(filename,"id/id_bk%d.id",nnn);
        sprintf(buff2,"%s %s <- %s",now_str(2),ID_FILE,filename);
        put_line(buff2,"/tmp/idrecover.rec");
        if (mode==1) {
            printf("ID : %s\n",ID_FILE);
            printf("BK : %s\n",filename);
        }
        if (!is_file(filename)) continue;
        if ((fp1=fopen(ID_FILE,"r+"))==NULL)   continue;
        if ((fp2=fopen(filename,"r"))==NULL)   continue;
        n=0;
        while (1) {
            fseek(fp1,n*sizeof_pf,SEEK_SET);
            if (!fread(&pf,sizeof_pf,1,fp1)) break;
            if (strlen(pf.id)>0&&strlen(pf.passwd)>0&&!is_esc(pf.id)&&pf.type<=0&&pf.level<=0) goto _cont2;
            if (pf.type>=0&&pf.type<=19&&!is_esc(pf.id)) {n++;continue;}
            if (pf.type==0&&strlen(pf.id)==0) {n++;continue;}
            if (pf.type>=0&&pf.type<=19) {n++;continue;}
            _cont2:
            if (mode==1) printf("ERR: %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));

            //백업에서 찾기
            fseektop(fp2);
            found=No;
            while (fread(&pf2,sizeof_pf,1,fp2)) {
                if (strlen(pf2.id)<3) continue;
                if (!is_same(pf.id,pf2.id)||!is_same(pf.name,pf2.name)) continue;
                //찾기성공 복구함
                if (mode==1) printf("ER1: %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
                pf2pf(pf2);
                if (mode==1) printf(" ->  %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
                sprintf(buff2," ->  %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
                put_line(buff2,"/tmp/idrecover.rec");
                fseek(fp1,n*sizeof_pf,SEEK_SET);
                pf.num=n;
                gwrite(183,pf,fp1);
                found=Yes;

                //다음자료 복구 읽기
                fseek(fp1,(n+1)*sizeof_pf,SEEK_SET);
                if (!fread(&pf,sizeof_pf,1,fp1)) break;
                if (strlen(pf.id)>2&&!is_esc(pf.id)&&!is_esc(pf.name)) break;
                if (strlen(pf.id_no)<1) break;
                //주민등록으로 삭제된 자료 검색
                fseektop(fp2);
                while (fread(&pf2,sizeof_pf,1,fp2)) {
                    if (is_same(pf.id_no,pf2.id_no)) {  //찾기성공
                        if (mode==1) printf("ER2: %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
                        pf2pf2(pf2);
                        if (mode==1) printf(" ->  %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
                        sprintf(buff2," ->  %5d %-8s %-8s %3d %8d %s %s\n",pf.num,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
                        put_line(buff2,"/tmp/idrecover.rec");
                        fseek(fp1,(n+1)*sizeof_pf,SEEK_SET);
                        pf.num=n+1;
                        gwrite(184,pf,fp1);
                        found=Yes;
                        break;
                    }
                }
                break;
            }
            if (!found) {
                err++;
                if (mode==1) printf("현재 %d명 오류\n",err);
                sprintf(buff2,"현재 %d명 오류\n",err);
                put_line(buff2,"/tmp/idrecover.rec");
            }
            n++;
        }
        fclose (fp1);
        fclose (fp2);
        if (err==0) {
            if (mode==1) printf("현재 오류 없음\n",err);
            sprintf(buff2,"현재 오류 없음\n",err);
            put_line(buff2,"/tmp/idrecover.rec");
            break;  //오류없음
        }
    }
    set_idbackup(mode);
    chmod(ID_FILE,0777);
}


//pf2 -> pf로 복구
pf2pf(PF pf2)
{
	pf.istat	   =pf2.istat	  ;
	pf.itype	   =pf2.itype	  ;
	pf.localmenu   =pf2.localmenu ;
	pf.loginmode   =pf2.loginmode ;
	pf.offmin	   =pf2.offmin	  ;
	pf.point	   =pf2.point	  ;
	pf.warn 	   =pf2.warn	  ;
	pf.all_log	   =pf2.all_log   ;
	pf.all_time    =pf2.all_time  ;
	pf.cash 	   =pf2.cash	  ;
	pf.cashmode    =pf2.cashmode  ;
	pf.countd	   =pf2.countd	  ;
	pf.countu	   =pf2.countu	  ;
	pf.countw	   =pf2.countw	  ;
	pf.down 	   =pf2.down	  ;
	pf.good 	   =pf2.good	  ;
	pf.left_time   =pf2.left_time ;
	pf.level	   =pf2.level	  ;
	pf.month_log   =pf2.month_log ;
	pf.month_time  =pf2.month_time;
	pf.myflag	   =pf2.myflag	  ;
	pf.mymode	   =pf2.mymode	  ;
	pf.p_mo 	   =pf2.p_mo	  ;
	pf.paymode	   =pf2.paymode   ;
	pf.today	   =pf2.today	  ;
	pf.todaymode   =pf2.todaymode ;
	pf.inday	   =pf2.inday	  ;
	pf.login	   =pf2.login	  ;
	pf.logout	   =pf2.logout	  ;
	pf.p_at 	   =pf2.p_at	  ;
	pf.p_it 	   =pf2.p_it	  ;
	pf.p_ph 	   =pf2.p_ph	  ;
	pf.p_pr 	   =pf2.p_pr	  ;
	pf.type 	   =pf2.type	  ;

	strcpy(pf.add2	,pf2.add2  )   ;
	strcpy(pf.addr	,pf2.addr  )   ;
	strcpy(pf.bibi	,pf2.bibi  )   ;
	strcpy(pf.hand	,pf2.hand  )   ;
	strcpy(pf.id_no ,pf2.id_no )   ;
	strcpy(pf.memo1 ,pf2.memo1 )   ;
	strcpy(pf.memo2 ,pf2.memo2 )   ;
	strcpy(pf.offi	,pf2.offi  )   ;
	strcpy(pf.otel	,pf2.otel  )   ;
	strcpy(pf.pos1	,pf2.pos1  )   ;
	strcpy(pf.pos2	,pf2.pos2  )   ;
	strcpy(pf.tele	,pf2.tele  )   ;
	strcpy(pf.user1 ,pf2.user1 )   ;
	strcpy(pf.user2 ,pf2.user2 )   ;
	strcpy(pf.user3 ,pf2.user3 )   ;
	strcpy(pf.user4 ,pf2.user4 )   ;
	strcpy(pf.wifeid,pf2.wifeid)   ;
}


//pf2 -> pf로 복구
pf2pf2(PF pf2)
{

	strcpy(pf.email  ,pf2.email  );
	strcpy(pf.gofirst,pf2.gofirst);
	strcpy(pf.id	 ,pf2.id	 );
    strcpy(pf.tmail  ,pf2.tmail  );
    strcpy(pf.mailid ,pf2.mailid );
	strcpy(pf.memo1  ,pf2.memo1  );
	strcpy(pf.memo2  ,pf2.memo2  );
	strcpy(pf.name	 ,pf2.name	 );
	strcpy(pf.nick	 ,pf2.nick	 );
	strcpy(pf.passwd ,pf2.passwd );
	strcpy(pf.user1  ,pf2.user1  );
	strcpy(pf.user2  ,pf2.user2  );
	strcpy(pf.user3  ,pf2.user3  );
	strcpy(pf.user4  ,pf2.user4  );

	pf.moon 	 = pf2.moon 	 ;
	pf.job		 = pf2.job		 ;
	pf.sex		 = pf2.sex		 ;
	pf.wmoon	 = pf2.wmoon	 ;
	pf.idnum	 = pf2.idnum	 ;
	pf.birthday  = pf2.birthday  ;
	pf.marryday  = pf2.marryday  ;
	pf.wbirthday = pf2.wbirthday ;
}

set_cfindpf(char *tid)
{
	FILE *fp1,*fp2;
	char datestr1[100],datestr2[100];
	GO tgo;
	int count=0;
	if (!read_pf3(tid)||pf.type<1) {mess("%s ID를 찾을수 없습니다.",tid);return;}
	if ((fp1=fopen(MENU_FILE,"r"))==NULL) return;
	if ((fp2=fopen(TMP_FILE,"w+"))==NULL) return;
	printf("\n ... 잠시만 기다려 주십시요. 동호회를 검색중입니다....\n");
	club_mode=Yes;
	fprintf(fp2,"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	fprintf(fp2,"   동호회이름          동호회코드       가입일/최종접속일   진입 등급 동호회장\n");
	fprintf(fp2,"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	while(fread(&club_go,sizeof_go,1,fp1)) {
		if (club_go.type!='s'||strlen(club_go.code)<1||strlen(club_go.dir)<1) continue;
		read_ccfg();
		if (read_cpf(pf.id)) {
			strcpy(datestr1,datestr(2,cpf.inday));
			strcpy(datestr2,datestr(2,cpf.logout));
			fprintf(fp2,"%-3d%-19.19s %-8s %s %s%5d %2d %-8s\n",
			count+1,club_go.name,club_go.code,
			datestr1,datestr2,cpf.all_log,cpf.type,ccfg.sysop);
			count++;
		}
	}
	fclose(fp1);
	fclose(fp2);
	if (count==0) mess("가입된 동호회가 없습니다.");
	else view_text(TMP_FILE);
	unlink(TMP_FILE);
}




count_guest()
{
	int n2=0;
	FILE *fp;
	fp=fopen(GUEST_FILE,"r+");   //총대기자 파악
	if (fp!=NULL) {
		fseekend(fp);
		n2=ftell(fp)/sizeof_pf;
		fclose(fp);
	}
	return n2;
}


//해당 등급의 회원수 찾기
count_pftype(int type) {
	int rec=0;
	FILE *fp;
	PF pf;
	fp=fopen(ID_FILE,"r+");
	if (fp==NULL) return rec;
	while (fread(&pf,sizeof_pf,1,fp)) {
		if (pf.type==type) rec++;
	}
	fclose(fp);
	return rec;
}


check_day(int who)
{
	int i,n;
	FILE *fp;
	char buff[100];
	char buff2[100];
	int anum[366];
	PF pf;

	time_t today;

	time(&today);
	today=today/(24*3600)+1;  //시간 끝으로 지정
	today*=(24*3600);

	for (i=0;i<366;i++) anum[i]=0;

	fp=fopen(ID_FILE,"r+");
	while (fp!=NULL&&fread(&pf,sizeof_pf,1,fp)>0) {
		n=day_calc(today,pf.inday);
		if ( n>=0&&n<=365) anum[n]++;
	}
	fclose(fp);

	cls();
	fp=fopen(TMP_FILE,"w+");
	fprintf(fp," ####     최근 1 년간 가입 회원 통계    ##### \n");
	n=0;
	for (i=0;i<366;i++) {
		if (anum[i]>0) {
			fprintf(fp,"   %s :   %5d 명 가입\n",datestr(37,today-(i+1)*24*3600),anum[i]);
		}
	}
	fclose(fp);
	view_text(TMP_FILE);
	unlink(TMP_FILE);
}


check_idname(int who)
{
	int i=0;
	FILE *fp;
	char buff[100];
	char buff2[100];
	int found=No;
	PF pf;

	printf(" 검색할 성명 또는 ID 를 입력하세요. (일부만 입력가능)\n >>");
	lineinput(buff2,8);
	printf("\n");
	printfL2();
	if (buff2[0]==0) return;

	if (!club_mode) {
		fp=fopen(ID_FILE,"r+");
		while (fp!=NULL&&fread(&pf,sizeof_pf,1,fp)>0) {
			if (stristr(pf.name,buff2)||stristr(pf.id,buff2)||stristr(pf.nick,buff2)) {
				printf(" %-8s (%-8s)(%-13.13s) %2d ",pf.id,pf.name,pf.addr,pf.type);
				if (i++%2==1) printf("\n");
				found=Yes;
			}
		}
		fclose(fp);
		fp=fopen(GUEST_FILE,"r");
		while (fp!=NULL&&fread(&pf,sizeof_pf,1,fp)>0) {
			if (stristr(pf.name,buff2)||stristr(pf.id,buff2)||stristr(pf.nick,buff2)) {
				printf("대기 %-8s (%-8s)(%-13.13s) %2d ",pf.id,pf.name,pf.addr,pf.type);
				if (i++%2==1) printf("\n");
				found=Yes;
			}
		}
		fclose(fp);
	}

	else {
		fp=fopen(ID_FILE,"r");
		while (fp!=NULL&&fread(&cpf,sizeof_cpf,1,fp)>0) {
			if (stristr(cpf.name,buff2)||stristr(cpf.id,buff2)) {
				printf(" %-8s (%-8s) %2d   ",cpf.id,cpf.name,cpf.type);
				if (i++%2==1) printf("\n");
				found=Yes;
			}
		}
		fclose(fp);
	}
	if (!found) i=0;
	printf("\n  총 %d 명이 검색되었습니다.\n",i);
	getchar();
}



check_jumin(int who)
{
	int i=0;
	FILE *fp;
	char buff2[100];
	int found=No;
	PF pf;

	printf(" 검색할 주민등록번호의 끝자리 7자리 숫자를 입력하세요. >>");
	lineinput(buff2,7);
	printf("\n");
	printfL2();
	if (buff2[0]!=0&&strlen(buff2)==7) {
		fp=fopen(ID_FILE,"r+");
		while (fread(&pf,sizeof_pf,1,fp)>0) {
			if (stristr(pf.id_no,buff2)) {
				printf(" %-8s (%-8s)%d ",pf.id,pf.name,pf.type);
				if (i++%3==2) printf("\n");
				found=Yes;
			}
		}
		fclose(fp);
	}

	if (!found&&buff2[0]!=0&&strlen(buff2)==7) {
		fp=fopen(GUEST_FILE,"r+");
		while (fread(&pf,sizeof_pf,1,fp)>0) {
			if (stristr(pf.id_no,buff2)) {
				printf(" %-8s (%-8s)%d ",pf.id,pf.name,pf.type);
				if (i++%3==2) printf("\n");
				found=Yes;
			}
		}
		fclose(fp);
	}
	if (!found) i=0;
	printf("\n  총 %d 명이 검색되었습니다.\n",i);
	getchar();

}


check_job(int who)
{
	int i=0,n;
	FILE *fp;
	char buff2[100];
	int found=No;
	PF pf;

	printf(" 검색할 직업 번호 또는 직업난에 포함될 문자를 입력하세요. >>");
	lineinput(buff2,7);
	printf("\n");
	printfL2();
	if (buff2[0]==0) return;
	n=-1;
	if (is_same(buff2,"0")) n=0;
	fp=fopen(ID_FILE,"r+");
	while (fread(&pf,sizeof_pf,1,fp)>0) {
		if ( (n==-1&&stristr(pf.offi,buff2))||(n>=0&&pf.job==n) ) {
				printf(" %-8s (%-8s)%d ",pf.id,pf.name,pf.type);
			if (i++%3==2) printf("\n");
			found=Yes;
		}
	}
	fclose(fp);

	fp=fopen(GUEST_FILE,"r+");
	while (fread(&pf,sizeof_pf,1,fp)>0) {
		if ( (n==-1&&stristr(pf.offi,buff2))||(n>=0&&pf.job==n) ) {
			printf(" %-8s (%-8s)%d ",pf.id,pf.name,pf.type);
			if (i++%3==2) printf("\n");
			found=Yes;
		}
	}
	fclose(fp);

	if (!found) i=0;
	printf("\n  총 %d 명이 검색되었습니다.\n",i);
	getchar();

}


check_new(int who)
{
	char buff[512];
	char tcode[512];
	char tid[20];
	int i,n,total;
	FILE *fp;
	int found=No;
	PF pf,pf2;
	CPF cpf;

	fp = fopen(ID_FILE , "r+");
	if (fp==NULL) return;
	total=count_pf();
	i=total;
	while (i>0) {
		fseek(fp,(i-1)*sizeof_pf,SEEK_SET);
		if(fread(&pf,sizeof_pf,1,fp)<1) break;		   //아이디 읽기
		if (pf.job>18||pf.job<0)  pf.job=0; 	//직업
		del_space(pf.nick);
		del_esc(pf.nick);
		view_pfw2(pf,who);					   //보기
		xy(60,1);printf("총:%d명 W:%d",total,who);
		xy(0,21);
		printf(" 이전(A,ENTER) 다음(N) 끝내기(Q) 수정(E) 번호:레코드 문자:검색 >>");
		lineinput(buff,8);
		if		(is_same(buff,"n")) i++;
		else if (is_same(buff,"q")) break;
		else if (is_same(buff,"p")) break;
		else if (is_same(buff,"x")) break;
		else if (is_same(buff,"e")&&who>3) {
			sprintf(buff,"bin/ed_pf %s %d 1",pf.id,who);
			system(buff);
		}
		else if (strlen(buff)<3)	i--;
		else {
			found=No;
			for (n=0;n<total;n++) {
				fseek(fp,n*sizeof_pf,SEEK_SET);
				fread(&pf,sizeof_pf,1,fp);		   //아이디 저장
				if (stristr(pf.id,buff)
				  ||stristr(pf.name,buff)
				  ||stristr(pf.nick,buff)) {
					view_pfw2(pf,who);					   //보기
					found=Yes;
					xy(0,21);
					if (yesno("이 회원이 맞습니까?  검색끝(Y) 계속검색(N) >>",Yes)) {
						i=n+1;
						break;
					}
				}
			}
			n=atoi(buff);
			if (!found&&n>0&&n<=total) i=n;
		}
		if (i<1) i=1;
		if (i>total) i=total;
	}
	fclose(fp);
}


//오늘 가입회원
int  count_npf() {
	int i,n,rec=0;
	struct tm *tp, *localtime();
	FILE *fp;
	time_t t;
	int nday;
	PF pf,pf2;

	fp=fopen(ID_FILE,"r+");
	if (fp==NULL) return 0;
	fseekend(fp);
	time(&t); //12311231
	tp=localtime(&t);
	nday=tp->tm_mon*100+tp->tm_mday;
	n=ftell(fp)/sizeof_pf;
	for (i=n;i>0;i--) {
		fseek(fp,(i-1)*sizeof_pf,SEEK_SET);
		fread(&pf2,sizeof_pf,1,fp);
		tp=localtime(&pf2.inday);
		if (nday!=tp->tm_mon*100+tp->tm_mday) break;
		rec++;
	}
	fclose(fp);
	return rec;
}


check_stat()
{
	int i=0,mode;
	FILE *fp;
	char buff[100];
	int found=No;
	PF pf;
	int n1,n2,n3,n4;
	n1=n2=n3=n4=0;

	n1=count_pf();				 //총인원수 파악
	n2=count_guest();			 //대기자 현황
	n3=count_pftype(0); 		 //삭제자 명단
	n4=count_npf(); 			 //금일가입회원
	n1=n1-n3;
	cls();
	printf(" 금일가입: %5d 명\n\n",n4);
	printf(" 총인원: %5d 명\n",n1);
	printf(" 대기자: %5d 명\n",n2);
	printf(" 삭제자: %5d 명\n",n3);
	printf(" 현상태: %s\n",bit(cfg.guestmode,15)?"자동가입처리":"수동가입처리");
	printf(" 삭제 대상   회원 조회\n");
	for (mode=1;mode<3;mode++) {
		printfL2();
		if (mode==1) printf(" 삭제 회원 검색\n");
		else		 printf(" 운영진    검색\n");
		fp=fopen(ID_FILE,"r+");
		i=0;
		found=No;
		while (fp!=NULL&&fread(&pf,sizeof_pf,1,fp)>0) {
			if ( (mode==1&&pf.type==0)||(mode==2&&pf.type>=14)) {
				printf(" %-8s(%-8s)%2d ",pf.id,pf.name,pf.type);
				if (i++%3==2) printf("\n");
				found=Yes;
			}
		}
		fclose(fp);
		if (!found) i=0;
		printf("\n  총 %d 명이 검색되었습니다.\n",i);
	}
	pressenter();
}

check_data(int mode)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char filename[100],tbuff[100],buff[200],buff1[200],buff2[200];
	int i,n,count,now_num;
	PF apf[100];

	while (1) {
		printf("\n");
		strcpy(filename,"");
		if (mode==1) {
			strcpy(filename,"id/id_name.ind");
			printf(" 검색 ID,이름>>");
		}
		else if (mode==2) {
			strcpy(filename,"id/id_idno.ind");
			printf(" 검색 주민등록번호>>");
		}
		else if (mode==3) {
			printf(" ID/이름/별명/주소/전화번호/이동전화 >>");
		}
		lineinput(tbuff,8);printf("\n");
		if (strlen(tbuff)<1) continue;
		if (strlen(tbuff)<3) return;
		for (i=0;i<100;i++) strcpy(apf[i].id,"");
		count=0;
		if (mode==3) {
			fp1=fopen(ID_FILE,"r");
			while (count<100&&fread(&pf2,sizeof_pf,1,fp1)) {
				if (strlen(pf2.id)<3||pf2.type<0||pf2.type>=18) continue;
				if ( stristr(pf2.id   ,tbuff)	||
					 stristr(pf2.name ,tbuff)	||
					 stristr(pf2.id_no,tbuff)	||
					 stristr(pf2.tele ,tbuff)	||
					 stristr(pf2.offi ,tbuff)	||
					 stristr(pf2.memo1,tbuff)	||
                     stristr(pf2.user1,tbuff)   ||
					 stristr(pf2.addr ,tbuff) ) {
					apf[count++]=pf2;
				}
			}
			fclose (fp1);
		}
		else {
			if ((fp1=fopen(filename,"r"))==NULL) {
				mess("%s 파일없음",filename);
				return;
			}
			while(count<100&&fgets(buff,100,fp1)!=NULL) {
				if (strlen(buff2)<3) continue;
				if (!stristr(buff,tbuff)) continue;
				bzero(buff1,30);
				sscanf(buff,"%s %s",&buff1,&buff2);
				if (read_pf2(buff2)&&pf2.type<18) {
					apf[count++]=pf2;
				}
			}
			fclose(fp1);
		}

		if (count<1) {
			printf(" ## 검색대상 회원이 없습니다.\n");
			continue;
		}
		now_num=1;
		while (1) {
			printf("\n");
			printfL2();
			printf("     ID       성명    등급 암호     주민등록          전화번호   주소\n");
			printfL2();
			for (i=0;i<count;i++) {
				printf("%3d. %-8s %-8s %2d %-8s %14.14s %13.13s %-14.14s\n",
				i+1,apf[i].id,apf[i].name,apf[i].type,apf[i].passwd,apf[i].id_no,apf[i].tele,apf[i].addr);
				if (i%20==19) {
					printf(" 계속(Enter) 수정(E 번호) 종결(Q)>>");
					lineinput(buff,6);printf("\n");
					if (strlen(buff)>0) goto _cont2;
				}
			}
			_cont1:
			printf(" 세부정보(번호)  수정(E 번호) 다음(N) 이전(B) 종결(Q)>>");
			lineinput(buff,6);printf("\n");
			_cont2:
			if (strlen(buff)<1||is_same(buff,"p")) break;
			if (is_same(buff,"n")) sprintf(buff,"%d",now_num+1);
			else if (now_num>1&&is_same(buff,"b")) sprintf(buff,"%d",now_num-1);
			else if (is_quit(buff)) return;
			i=atoi(buff);
			check_cmd(buff);
			if (i>0) now_num=i;
			if (is_same(mmc[0],"e")&&mmn[1]>0&&mmn[1]<=count) {
				now_num=mmn[1];
				if (who==5) sys_tem("bin/ed_pf root '%s' -9",apf[mmn[1]-1].id);
				else		sys_tem("bin/ed_pf '%s' '%s'",now_id,apf[mmn[1]-1].id);
			}
			else if (mmn[0]>0&&mmn[0]<=count) {
				now_num=mmn[0];
				view_pfw2(apf[mmn[0]-1],who);
				goto _cont1;
			}
		}
	}
}


make_name_index(int mode)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char buff[200],buff1[200],buff2[200];
	if ((fp1=fopen(ID_FILE,"r"))==NULL) return;
	fp2=fopen(TMP_FILE1,"w");
	fp3=fopen(TMP_FILE2,"w");
	if (mode) printf("\n ### 자료 추출... \n");
	while((fread(&pf,sizeof_pf,1,fp1))) {
		del_space(pf.name);del_space(pf.id_no);del_space(pf.id);
		if (strlen(pf.id)<3) continue;
		if (strlen(pf.name )>2) fprintf(fp2,"%04hx %04hx %04hx %04hx %04hx %04hx %04hx %04hx %s\n",pf.name[0],pf.name[1],pf.name[2],pf.name[3],pf.name[4],pf.name[5],pf.name[6],pf.name[7],pf.id);
		if (strlen(pf.id_no)>0) fprintf(fp3,"%s %s\n",pf.id_no,pf.id);
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	if (mode) printf(" ### 자료 정렬... \n");
	sys_tem("sort %s -o%s",TMP_FILE1,TMP_FILE);
	sys_tem("sort %s -o%s",TMP_FILE2,ind_file_idno);
	if (mode) printf(" ### INDEX생성... \n");
	if ((fp1=fopen(TMP_FILE,"r"))==NULL) return;
	if ((fp2=fopen(ind_file_name,"w"))==NULL) return;
	while(fgets(buff,200,fp1)!=NULL) {
		bzero(buff1,30);
		sscanf(buff,"%x %x %x %x %x %x %x %x %s",&buff1[0],&buff1[1],&buff1[2],&buff1[3],&buff1[4],&buff1[5],&buff1[6],&buff1[7],buff2);
		buff1[8]=0;del_space(buff1);del_space(buff2);
		fprintf(fp2,"%s %s\n",buff1,buff2);
	}
	if (mode) printf(" ### 작업끝...... \n");
	fclose(fp1);
	fclose(fp2);
	unlink(TMP_FILE);
	unlink(TMP_FILE1);
	unlink(TMP_FILE2);
}



//일반 회원이 검색
user_find(int mode)
{
    int i,n,found=No;
    FILE *fp;
    char buff[100];
         if (mode==1) printf(" 검색할 성명,ID >>");
    else if (mode==2) printf(" 검색할 학교 또는 직장 >>");
    else if (mode==3) printf(" 검색할 취미 >>");
    lineinput(buff,15);printf("\n");

    if (strlen(buff)<3) return;
    printfL2();
    fp=fopen(ID_FILE,"r");
    i=n=0;
    while (fread(&pf,sizeof_pf,1,fp)>0) {
        if (strlen(pf.id)<3) continue;
        if ( (mode==1&&(stristr(pf.name,buff)||stristr(pf.id,buff)||stristr(pf.nick,buff)))||
             (mode==2&&stristr(pf.offi,buff))||
             (mode==3&&stristr(pf.user4,buff)) ) {
            printf(" %-8s (%-8s)",pf.id,pf.name);
            if (i++%4==3) printf("\n");
            found=Yes;
        }
    }
    fclose(fp);
    if (i>0) {
        if (i%4!=0) printf("\n");
        printfL2();
    }
    n+=i;
    i=0;
    if ((fp=fopen(GUEST_FILE,"r"))!=NULL) {
        while (fread(&pf,sizeof_pf,1,fp)>0) {
            if (stristr(pf.name,buff)||stristr(pf.id,buff)||stristr(pf.nick,buff)) {
                printf(" 대기:%-8s(%-8s)",pf.id,pf.name);
                if (i++%4==3) printf("\n");
                found=Yes;
            }
        }
        fclose(fp);
    }
    if (i>0&&i%4!=0) printfL2();
    if (!found) printf("  %s 님을 찾을수 없습니다.\n",buff);
    else        printf("  총 %d 명이 검색되었습니다. ",n);
    pressenter();
}
