////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 게시판 담당자의 메뉴
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include <unistd.h>    //디렉토리 화일명 변환함수때문에 필요
#include <dirent.h>    //디렉토리 화일명 변환함수때문에 필요

int asearch0[31];
int asearch1[31];
int asearch2[31];
int asearch3[31];
int asearch4[31];
int n;
int now_local;
int tot[20];

main(int argc, char **argv)
{
	FILE *fp;
	int i,n=0,n1,n2;
	char buff[140];
	char buff2[140];
	char filename[200];
	char tcode[9];
	char ccode[9];
    char id[19];
	char menu_change=No;

	findport(tty); //접속포트 찾기
	read_cfg();
	set_tmpfile();		 //임시화일지정하기

    if (argc<4) {
        printf("사용법  bsysop <가기코드> <동호회코드> <ID> [모드] \n");
        printf("사용법  bsysop hello game 시삽\n");
		getchar();
		return;
	}
    if (is_same(argv[1],"_renum_bbs")) {   //강제번호정리
        strcpy(NOW_FILE,argv[2]);
        strcpy(NOW_FILED,argv[3]);
        go.type='l';
        bbs_renum2();
        return;
    }
    for (i=0;i<20;i++) tot[i]=0;

    strcpy(tcode,argv[1]);                 //가기코드
    strcpy(ccode,argv[2]);                 //동호회 코드
    strcpy(id,argv[3]);                    //아이디

	set_nowmenu();		 //데이타화일지정하기
	if (!find_club(ccode)) {
		mess("%s 동호회 검색 실패",ccode);
		return;
	}
//    printf("%s %s %s",ccode,tcode,NOW_MENU);pressenter();
	set_nowmenu();		 //데이타화일지정하기
	if (!find_go(tcode)) {
		mess("%s 가기코드 검색 실패",tcode);
		return;    //찾는 가기코드가 없다
	}

	read_ccfg();
	set_bcfgfile(); 					//BCFG_FILE에 저장
	read_bcfg();
    	if (is_same(id,"all_pass_del")) {all_pass_del();return;}
    	if (is_same(id,"all_mail_del")) {all_mail_del();return;}
	
	if (!read_pf(id)) {
		mess("%s 검색실패",id);
		return;
	}
	now_local=pf.localmenu;
	

	strcpy(NOW_FILE,"");
	strcpy(NOW_FILED,"");
	if (is_typebbs||is_typepds) {
		sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
		sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,go.code);
		del_space(NOW_FILE);
		del_space(NOW_FILED);
		sys_tem("touch %s",NOW_FILE );
		sys_tem("touch %s",NOW_FILED);
	}
	read_cpf(id);					 //해당되는 동호회의 아이디를 찾는다.
	read_atype();					 //등급읽기
	read_actype();					 //동호회등급의 이름을 읽어옴  ->actype
	who=check_who();				 //1:동호회 운영진 2:동호회 시삽  3:root권한

	if (who<1) {
		mess("%d,%d 게시판메뉴사용불가능등급",who,pf.type);
		return;
	}

    if (argc>=5) {
        i=atoi(argv[4]);
			 if (i==101) bbs_all_sort();
		else if (i==102) ed_rsize();
		if (i>100) return;
	}


	while (1) {
		cls();
		xy(1,1);
        print_(1," %s(%s)(%c) 메뉴 (담당:%s) LOCAL:%d",go.code,go.name,go.type,bcfg.man1,now_local);
        printf("┏━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━┓\n");
        printf("┃    1. 시스템 메뉴세팅        ┃    31. 전체게시물삭제복구    ┃\n");
        printf("┃    2. 게시판 환경설정        ┃    32. 전체게시물업/다운     ┃\n");
        printf("┃    3. 게시판 화면관리        ┃    33. 전체게시물세팅        ┃\n");
        printf("┃    4. 게시판 진입제한        ┃    34. 번호재정리,소트       ┃\n");
        printf("┃    5. 화상게시판 관련        ┃    35. 자료실화일갱신        ┃\n");
        printf("┃    6. 게시판기록 관련        ┃    36. 삭제자료완전정리      ┃\n");
        printf("┃    7. 게시판모양 관련        ┃    37. 미등록화일읽기        ┃\n");
        printf("┃    8. 게시판목록 관련        ┃    38. 특정게시물추출BDN     ┃\n");
        printf("┃    9. 자료업/다운설정        ┃                              ┃\n");
        printf("┃   10. 게시판 쓰기관련        ┃    40. 통계/조회기록확인     ┃\n");
        printf("┃   11. 잠깐설문   관련        ┃    41. 자동처리 관련         ┃\n");
        printf("┃   12. 찬성/보류  관련        ┃    42. 상품관련 관련         ┃\n");
        printf("┣━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━┫\n");
        printf("┃   21. IP      개인IP         ┃    51. 게시물자동변환        ┃\n");
        printf("┃   22. WAR   전투모드         ┃    52. 시디디렉토리읽기      ┃\n");
        printf("┃   23. CASH사이버머니         ┃    53. 사용기록조회현황      ┃\n");
        printf("┃   24. BILL      과금         ┃    54. 자료갯수 재정리       ┃\n");
        printf("┗━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┛\n");
		printf(" 선택번호? (종결:Q 저장취소:N 상태:?) >>");
		lineinput(buff,10);check_han(buff);printf("\n");n=atoi(buff);
		if (is_quit(buff)) break;
		if (is_same(buff,"n")) return;
		else if (is_same(buff,"?")) disp_status();
		else if (n== 1) {set_menucfg();menu_change=Yes;}
		else if (n== 2) set_bcfg1();	 //환경설정
		else if (n== 3) set_blogo();
		else if (n== 4) set_limit();	//환경설정
		else if (n== 5) set_jpgcfg();	//환경설정
		else if (n== 6) set_reccfg();	//환경설정
		else if (n== 7) set_formcfg();	//환경설정
		else if (n== 8) set_listcfg();
		else if (n== 9) set_updown();
		else if (n==10) set_write();
		else if (n==11) set_quest();
		else if (n==12) set_ok();
		else if (n==21) bset_ipgo();	//사이버 설정
		else if (n==22) bset_wars();	//전투
		else if (n==23) bset_cash();	//사이버 설정
		else if (n==24) bset_bill();	//사이버 설정
		else if (n==31) bbs_all_del();
		else if (n==32) bbs_all_down();
		else if (n==33) bbs_all_set();
		else if (n==34) bbs_all_sort();
		else if (n==35) ed_rsize();
		else if (n==36) bbs_pack();
		else if (n==37) up_nofile();
        else if (n==38) sys_tem("bin/bdn %s %s %s",go.code,club_go.code,pf.id);
		else if (n==40) view_iprec();
        else if (n==41) set_auto();
        else if (n==42) set_gift();
		else if (n==51) txt2bbs();
		else if (n==52) read_cdrom();
		else if (n==53) stat_rec();
        else if (n==54) re_bbsnum(3);

	}
	del_space(bcfg.man1);del_space(bcfg.man2);del_space(bcfg.man3);del_space(bcfg.man4);del_space(bcfg.man5);
	save_bcfg();
	if (menu_change) {
		GO go2;
		if ((fp=fopen(NOW_MENU,"r+"))!=NULL) {
			fseek(fp,go.rec*sizeof_go,SEEK_SET);
			fread(&go2,sizeof_go,1,fp);
			if (is_same(go2.code,go.code)) {
				fseek(fp,go.rec*sizeof_go,SEEK_SET);
				fwrite(&go,sizeof_go,1,fp);
			}
			fclose(fp);
			reset_menuind();	 //메뉴SUB TAG 재정리
		}
	}
}

re_count()
{
    FILE *fp;
    int i;
    char brecfilename[200];
    for (i=0;i<20;i++) tot[i]=0;
    fp=fopen(NOW_FILE,"r");
	while (fread(&bbs,sizeof_bbs,1,fp)) {
        if (bbs.del) {
            tot[2]++;
            continue;
        }
        tot[1]++;
        if (bbs.gd!=' ') tot[3]++;
        tot[4]+=bbs.read;
        tot[5]+=bbs.down;
        tot[6]+=bbs.line/20+1;
        if (strlen(bbs.filename)>0) tot[10]+=bbs.filesize;
        i=time_day(bbs.date);
        if (i==0) tot[7]++;
        else if (i<=7) tot[8]++;
        else if (i<=30) tot[9]++;
	}
	fclose(fp);
    tot[0]=count_rec(NOW_FILE,sizeof_bbs);
    sprintf(brecfilename,"%s/menurec/%s.bre",club_go.dir,go.code);
    if ((fp=fopen(brecfilename,"r"))!=NULL) {
		fread(&brec,sizeof_brec,1,fp);
		fclose(fp);
	}
}


bbs_all_sort()
{
	FILE *fp1;
	FILE *fp2;
	FILE *fp3;
	FILE *fp4;
	FILE *fp5;
	int mode,i,num,new_posi,new_num;
	int n,n1,n2;
	char buff[200];
	char buff2[200];
	char dir[200];
	char tclub[100];
	char gocode1[100];
	char gocode2[100];
	char filename1[200];
	char filename2[200];
	char filename3[200];
	char filename4[200];
	char SORT_FILE[200];
	int order,renum;

	order=renum=No;

	while (1) {
		cls();
		print_(1,"자료 정리");
		printf("  1. 번호 재정리\n");
		printf("  2. 날짜 순 정리   %s\n",order?"(내림차순)":"(오름차순)");
		printf("  3. 제목 순 정리   %s\n",order?"(내림차순)":"(오름차순)");
		printf("  4. ID   순 정리   %s\n",order?"(내림차순)":"(오름차순)");
		printf("  5. KEY  순 정리   %s\n",order?"(내림차순)":"(오름차순)");
		printf("  6. TCODE순 정리   %s\n",order?"(내림차순)":"(오름차순)");
		printf("  7. 화일명순정리   %s\n",order?"(내림차순)":"(오름차순)");
		printf("  8. 내용크기순     %s\n",order?"(내림차순)":"(오름차순)");
		printf("  9. 화일크기순     %s\n",order?"(내림차순)":"(오름차순)");
		printf(" 10. 화일다운순     %s\n",order?"(내림차순)":"(오름차순)");
		printf(" 11. 게시물읽기순   %s\n",order?"(내림차순)":"(오름차순)");
		printf(" 12. 게시물줄수순   %s\n",order?"(내림차순)":"(오름차순)");
		printf(" 13. 원래번호  순   %s\n",order?"(내림차순)":"(오름차순)");
		printf(" 21. 정렬방법  선택 %s\n",order?"(내림차순)":"(오름차순)");
		printf(" 22. 번호재정리선택 %s\n",renum?"( 재정리 )":"(원래번호)");
		printf(" 선택 >>");
		mode=numinput();
			 if (mode==21) {order=order?0:1;continue;}
		else if (mode==22) {renum=renum?0:1;continue;}
		else if (mode== 1) {
			if (yesno("게시판 자료의 번호를 재정리 하시겠습니까? (Y/n) >>",Yes))
                bbs_renum2();
			continue;
		}
		else if (mode< 1||mode>19) break;

		set_tmpfile();		 //임시화일지정하기
		set_nowmenu();		//현재화일명 설정
		sprintf(filename1,NOW_FILE );
		sprintf(filename2,NOW_FILED);
		sprintf(filename3,TMP_FILE );
		sprintf(filename4,TMP_FILE2);
        sprintf(SORT_FILE,"tmp/tmp8%d%s.tmp",cfg.hostno,no_spc(tty));

		printf("\n  %s(%s) 을 재정리합니다.\n ",filename1,go.name);
		printf(" 정리가 되면 삭제된 자료는 완전히 삭제됩니다.\n");
		if (yesno("  정렬을 시작할까요?  (Y/n)",Yes)==No) continue;
		printf("\n");

		fp1=fopen(filename1,"r");
		if (fp1==NULL) {
			mess(" %s 화일이 없습니다.",filename1);
			continue;
		}
		fseek(fp1,0,SEEK_SET);
		if ((fp5=fopen(TMP_FILE,"w"))==NULL) {
			mess("%s 파일 열기 실패",TMP_FILE);
			continue;
		}
		while (fread(&bbs,sizeof_bbs,1,fp1)) {
			num=ftell(fp1)/sizeof_bbs;
			if (bbs.del==No) {
				if		(mode== 2) fprintf(fp5,"%15d %d\n",bbs.date,num);
				else if (mode== 3) {del_space(bbs.title);fprintf(fp5,"%-60.60s %d\n",bbs.title,num);}
				else if (mode== 4) fprintf(fp5,"%15s %d\n",bbs.id,num);
				else if (mode== 5) fprintf(fp5,"%15s %d\n",bbs.key,num);
				else if (mode== 6) fprintf(fp5,"%15s %d\n",bbs.tcode,num);
				else if (mode== 7) fprintf(fp5,"%15s %d\n",bbs.filename,num);
				else if (mode== 8) fprintf(fp5,"%15d %d\n",bbs.size,num);
				else if (mode== 9) fprintf(fp5,"%15d %d\n",bbs.filesize,num);
				else if (mode==10) fprintf(fp5,"%15d %d\n",bbs.down,num);
				else if (mode==11) fprintf(fp5,"%15d %d\n",bbs.read,num);
				else if (mode==12) fprintf(fp5,"%15d %d\n",bbs.line,num);
				else if (mode==13) fprintf(fp5,"%15d %d\n",bbs.num,num);
				else			   fprintf(fp5,"%15d %d\n",bbs.num,num);
			}
		}
		if (fp1!=NULL) fclose(fp1);
		if (fp5!=NULL) fclose(fp5);
		unlink(SORT_FILE);

		c_lock(1,NOW_FILE);
		printf("\n *** 잠깐만 기다려 주세요... 지금 소트중입니다. ... *** \n");
        if (order) xsort(1,TMP_FILE,SORT_FILE);
        else       xsort(0,TMP_FILE,SORT_FILE);
		if (!is_file(SORT_FILE)) {
			c_lock(0,NOW_FILE);
			return;
		}

		fp1=fopen(filename1,"r+"); //제목
		fp2=fopen(filename2,"r+"); //내용
		fp3=fopen(filename3,"w"); //새제목
		fp4=fopen(filename4,"w"); //새내용
		fp5=fopen(SORT_FILE,"r+");
		n=1;
		new_posi=0; //새로운 위치
		new_num=1;	//새로운 번호
		printf("\n");

		while (n>0) {
			if (mode==3) n=fscanf(fp5,"%60s %d",&buff,&n2);
			else		 n=fscanf(fp5,"%15s %d",&buff,&n2);
			if (n<1) break;
			//제목읽기 fp1->fp3
			fseek(fp1,(n2-1)*sizeof_bbs,SEEK_SET);
			if (!fread(&bbs,sizeof_bbs,1,fp1)) continue;
			//내용읽음 -> 새내용  fp2->fp4
			fseek(fp2,bbs.posi,SEEK_SET);
			fseek(fp4,new_posi,SEEK_SET);
			for (i=0;i<bbs.size&&i<1000000;i++) fputc(fgetc(fp2),fp4); //내용에 저장
			bbs.posi=new_posi;
			if (renum) bbs.num =new_num;
			if (bbs.gd<' ') bbs.gd=' ';
			//새제목 저장 ->fp3
			fwrite(&bbs,sizeof_bbs,1,fp3);
			printf(" %5d %-8s %s\n",bbs.num,bbs.id,bbs.title);
			//새로운 위치지정
			new_posi+=bbs.size;
			new_num++;
		}
		if (fp1!=NULL) fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
		if (fp3!=NULL) fclose(fp3);
		if (fp4!=NULL) fclose(fp4);
		if (fp5!=NULL) fclose(fp5);
		//화일의 내용을 0으로 만듬
		set_zero(go.code);
		file_copy(filename3,filename1);
		file_copy(filename4,filename2);
		unlink(filename3);
		unlink(filename4);
		unlink(SORT_FILE);
		c_lock(0,NOW_FILE);
		printf("\n  %s 총정렬수(%d) \n",filename1,new_num-1);
		mess(" 자료변환이 끝이 났습니다.\n");
	}
}


//IP관련 사항의 설정
bset_ipgo()
{
}



//사이버머니 관련 사항의 설정
bset_cash()
{
	while (1) {
		cls();
		print_(1,"사이버머니 관련 설정 (%s:%s)",go.code,go.name);
		printf("  1. 진입시 감소(분당)                  :%d\n",cash.time1  );
		printf("  2. 진입시 감소(건당)                  :%d\n",cash.time2  );
		printf("  3. 읽을때 감소(건당)                  :%d\n",cash.read1  );
		printf("  4. 읽을때 감소(1K당)                  :%d\n",cash.read2  );
		printf("  5. 다운시 감소(건당)                  :%d\n",cash.down1  );
		printf("  6. 다운시 감소(1K당)                  :%d\n",cash.down2  );
		printf("  7. 쓸때   증가(건당)                  :%d\n",cash.writ1  );
		printf("  8. 쓸때   증가(1K당)                  :%d\n",cash.writ2  );
		printf("  9. 등록시 증가(건당)                  :%d\n",cash.uplo1  );
		printf(" 10. 등록시 증가(1K당)                  :%d\n",cash.uplo2  );
		printf(" 11. 삭제시 사이버머니 삭제함           :%s\n",rbit(cash.mode,1,"삭제","무시"));
		printf(" 12. 3일전 삭제시 사이버머니 삭제함     :%s\n",rbit(cash.mode,2,"삭제","무시"));
		n=numselect();xy(60,n+1);
        if (n<= 0) break;
		if (n== 1) cash.time1=get_num(cash.time1,6);
		if (n== 2) cash.time2=get_num(cash.time2,6);
		if (n== 3) cash.read1=get_num(cash.read1,6);
		if (n== 4) cash.read2=get_num(cash.read2,6);
		if (n== 5) cash.down1=get_num(cash.down1,6);
		if (n== 6) cash.down2=get_num(cash.down2,6);
		if (n== 7) cash.writ1=get_num(cash.writ1,6);
		if (n== 8) cash.writ2=get_num(cash.writ2,6);
		if (n== 9) cash.uplo1=get_num(cash.uplo1,6);
		if (n==10) cash.uplo2=get_num(cash.uplo2,6);
		if (n==11) cash.mode =gtt_bit(cash.mode ,1);
		if (n==12) cash.mode =gtt_bit(cash.mode ,2);
	}
}




//과금 관련 사항의 설정
bset_bill()
{
	while (1) {
		cls();
		print_(1,"과금 관련 설정 (%s:%s)",go.code,go.name);
		printf("  1. 진입시 감소(분당)                  :%d\n",bill.time1  );
		printf("  2. 진입시 감소(건당)                  :%d\n",bill.time2  );
		printf("  3. 읽을때 감소(건당)                  :%d\n",bill.read1  );
		printf("  4. 읽을때 감소(1K당)                  :%d\n",bill.read2  );
		printf("  5. 다운시 감소(건당)                  :%d\n",bill.down1  );
		printf("  6. 다운시 감소(1K당)                  :%d\n",bill.down2  );
		printf("  7. 쓸때   증가(건당)                  :%d\n",bill.writ1  );
		printf("  8. 쓸때   증가(1K당)                  :%d\n",bill.writ2  );
		printf("  9. 등록시 증가(건당)                  :%d\n",bill.uplo1  );
		printf(" 10. 등록시 증가(1K당)                  :%d\n",bill.uplo2  );
		n=numselect();xy(60,n+1);
        if (n<= 0) break;
		if (n== 1) bill.time1=get_num(bill.time1,6);
		if (n== 2) bill.time2=get_num(bill.time2,6);
		if (n== 3) bill.read1=get_num(bill.read1,6);
		if (n== 4) bill.read2=get_num(bill.read2,6);
		if (n== 5) bill.down1=get_num(bill.down1,6);
		if (n== 6) bill.down2=get_num(bill.down2,6);
		if (n== 7) bill.writ1=get_num(bill.writ1,6);
		if (n== 8) bill.writ2=get_num(bill.writ2,6);
		if (n== 9) bill.uplo1=get_num(bill.uplo1,6);
		if (n==10) bill.uplo2=get_num(bill.uplo2,6);
	}
}

//전투관련 사항의 설정
bset_wars()
{
	while (1) {
		cls();
		print_(1,"전투 관련 설정 (%s:%s)",go.code,go.name);
		printf("  1. 진입시 감소(분당)                  :%d\n",wars.time1  );
		printf("  2. 진입시 감소(건당)                  :%d\n",wars.time2  );
		printf("  3. 읽을때 감소(건당)                  :%d\n",wars.read1  );
		printf("  4. 읽을때 감소(1K당)                  :%d\n",wars.read2  );
		printf("  5. 다운시 감소(건당)                  :%d\n",wars.down1  );
		printf("  6. 다운시 감소(1K당)                  :%d\n",wars.down2  );
		printf("  7. 쓸때   증가(건당)                  :%d\n",wars.writ1  );
		printf("  8. 쓸때   증가(1K당)                  :%d\n",wars.writ2  );
		printf("  9. 등록시 증가(건당)                  :%d\n",wars.uplo1  );
		printf(" 10. 등록시 증가(1K당)                  :%d\n",wars.uplo2  );
		printf(" 11. 기지형태모드설정 \n");
		printf(" 12. 전투형태모드설정 \n");
		printf(" 13. 전투력  모드설정 \n");
		n=numselect();xy(60,n+1);
        if (n<= 0) break;
		if (n== 1) wars.time1=get_num(wars.time1,6);
		if (n== 2) wars.time2=get_num(wars.time2,6);
		if (n== 3) wars.read1=get_num(wars.read1,6);
		if (n== 4) wars.read2=get_num(wars.read2,6);
		if (n== 5) wars.down1=get_num(wars.down1,6);
		if (n== 6) wars.down2=get_num(wars.down2,6);
		if (n== 7) wars.writ1=get_num(wars.writ1,6);
		if (n== 8) wars.writ2=get_num(wars.writ2,6);
		if (n== 9) wars.uplo1=get_num(wars.uplo1,6);
		if (n==10) wars.uplo2=get_num(wars.uplo2,6);
	}
}


//조회기록
stat_rec()
{
	FILE *fp1,*fp2;
	int filesize;
    int it1,it2,it3,i,n,n1,count;
    int year,month,day1,day2;
	char st[30][100];
    char buff[300],buff1[300],buff2[300],buff3[300];
    char id1[10],id2[10],tstr[50];
    char filename[300];

    strcpy(id1,"");strcpy(id2,"");strcpy(tstr,"");
    year=now_year()%100;
	month=now_month();
    day1=1;day2=31;

	while (1) {
		cls();
		print_(1,"조회현황 / IP 관련 검색  (%s:%s 담당:%s)",go.code,go.name,bcfg.man1);
        printf("  1. 검색 일자 기간   년/월 시작-끝일   :%d/%d %d-%d\n",year%100,month,day1,day2);
        printf("  2. 자료/게시물 등록자 ID              :%s \n",id1);
        printf("  3. 검색 대상   회원   ID              :%s \n",id2);
        printf("  4. 포함문자중일부                     :%s\n",tstr);
        printf(" 96. %s 전체 사용 기록 확인\n",go.code);
        printf(" 97. %s 자료 등록/쓰기 기록 확인\n",go.code);
        printf(" 98. %s 자료 조회/다운 기록 확인\n",go.code);
		n=numselect();
		if (n<30) xy(50,n+1);
        if (n<= 0)  break;
        if (n== 1)  {year=get_num(year,2);month=get_num(month,2);
                     day1=get_num(day1,2);day2=get_num(day2,2);}
		if (n== 2)	get_str(id1,8);
		if (n== 3)	get_str(id2,8);
        if (n== 4)  get_str(tstr,10);
        if (n>=96&&n<=99)  {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			printf("   [%s] 검색을 시작합니다... \n",filename);
			if (!is_file(filename)) {mess("%s 기록 화일이 없습니다.",filename);continue;}
            if ((fp1=fopen(filename,"r"))==NULL) break;
            if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
			count=it1=it2=it3=0;
            if (n==96) {
                fprintf(fp2,"┏━━━━━━━┳━━━━┳━━━┳━━━━━━━━━━━━━━━━━━━━┓\n");    //해당되는 줄을 보여줌
                fprintf(fp2,"┃ 접속 시간  ##┃ 사용자 ┃ 작업 ┃                  제 목                 ┃\n");    //해당되는 줄을 보여줌
                fprintf(fp2,"┣━━━━━━━╋━━━━╋━━━╋━━━━━━━━━━━━━━━━━━━━┫\n");    //해당되는 줄을 보여줌
            }
            else if (is_typepds) {
                fprintf(fp2,"┏━━━━━━━┳━━━━┳━━━━┳━━━━━━┳━━━┳━━━━━━━━┓\n");    //해당되는 줄을 보여줌
                fprintf(fp2,"┃ 접속 시간  ##┃ 사용자 ┃ 등록자 ┃  파 일 명  ┃크기 K┃  제 목         ┃\n");    //해당되는 줄을 보여줌
                fprintf(fp2,"┣━━━━━━━╋━━━━╋━━━━╋━━━━━━╋━━━╋━━━━━━━━┫\n");    //해당되는 줄을 보여줌
            }
            else {
                fprintf(fp2,"┏━━━━━━━┳━━━━┳━━━━┳━━━━━━━━━━━━━━━━━━━┓\n");    //해당되는 줄을 보여줌
                fprintf(fp2,"┃ 접속 시간  ##┃ 사용자 ┃ 등록자 ┃                  제 목               ┃\n");    //해당되는 줄을 보여줌
                fprintf(fp2,"┣━━━━━━━╋━━━━╋━━━━╋━━━━━━━━━━━━━━━━━━━┫\n");    //해당되는 줄을 보여줌
            }
            while (fgets(buff,200,fp1)!=NULL) {
                if (strlen(buff)<40||buff[0]==';') continue;
				for (i=0;i<30;i++) strcpy(st[i],"");
				sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
				strcpy(buff2,&buff[91]);
				sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
                if (strlen(st[1])!=5||strlen(st[2])!=5||strlen(st[5])<3) continue;
                if (n==96) strcpy(st[25],&buff[91]);
                else if (is_same(st[21],"B")) strcpy(st[25],&buff[102]);
/*
1	   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 동호1 kslab						   STA 0 1
05-26 00:40  77 p1	 시삽	  19 main	  f1		8 다운1 kslab						   P 시삽	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 시삽	  19 main	  f1		6 등록1 kslab						   P 시삽	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 접속1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 접속2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 접속1 kslab		 STA 0
01-01 12:49   1 /1   okisland  9 main     hostbbs   5 쓰기1 capi.pe.kr                     B okisland [시스템] 2000년 문제
01-01 12:49   1 /1   okisland  9 main     hostbbs   7 읽기1 capi.pe.kr                     B okisland [시스템] 2000년 문제
01-01 12:49   1 /1   okisland  9 main     hostbbs   7 읽기1 capi.pe.kr                     B kjh714   [회  원] 가입금지 아
01-03 19:35   1 /0   시삽     19 main     pxhost    6 등록1 top.xhost.co.kr                P 시삽     viewlog 170414 [xhos
01-04 17:02   1 /0   시삽     19 main     pxhost    6 등록1 xbase.xhost.co.kr              P 시삽     bin522.tgz 3619458 [
01-04 17:04   1 /0   시삽     19 main     pxhost    6 등록1 xbase.xhost.co.kr              P 시삽     bin522_.tgz 3601344 
01-04 17:16   1 /1   guestt1   0 main     pxhost    7 읽기1 U211-33-81-242.thrun           B 시삽     [xhost5.22] 업그레이
01-04 17:17   1 /1   guestt1   0 main     pxhost    8 다운1 U211-33-81-242.thrun           P 시삽     bin522_.tgz 3601344 
*/
                if (!is_same(st[7],club_go.code)||!is_same(st[8],go.code)) continue;
                if (strlen(tstr)>0&&!stristr(buff,tstr)) continue;
                if (strlen(id2)>0&&!is_same(id2,st[5])) continue;
                if (strlen(id1)>0&&!is_same(id1,st[22])) continue;
                if (n==96) ;
                else if (is_typepds) {
                    if (n==98&&!is_same(st[10],"다운1")) continue;
                    if (n==97&&!is_same(st[10],"등록1")) continue;
                }
                else {
                    if (n==98&&!is_same(st[10],"읽기1")) continue;
                    if (n==97&&!is_same(st[10],"쓰기1")) continue;
                }
                xstrncpy(buff1,&buff[3],2);
                i=atoi(buff1);
                if (i<day1||i>day2) continue;
				count++;
				n1=atoi(st[24])/1024;
				it1+=n1;
                if (n==96) {
                    sprintf(buff,"┃%5.5s %5.5s%3.3s┃%-8s┃%-6s┃%-40.40s┃",
                                st[1],st[2],st[3],
                                st[5],st[10],st[25]);
                }
                else if (is_typepds) {
                    sprintf(buff,"┃%5.5s %5.5s%3.3s┃%-8s┃%-8s┃%-12.12s┃%6d┃%-16.16s┃",
                                st[1],st[2],st[3],
                                st[5],st[22],st[23],
                                n1,st[25]);
                }
                else {
                    sprintf(buff,"┃%5.5s %5.5s%3.3s┃%-8s┃%-8s┃%-38.38s┃",
                                st[1],st[2],st[3],
                                st[5],st[22],st[25]);
                }
				del_enter(buff);
				fprintf(fp2,"%s\n",buff);    //해당되는 줄을 보여줌
			}
            if (n==96) {
                fprintf(fp2,"┗━━━━━━━┻━━━━┻━━━┻━━━━━━━━━━━━━━━━━━━━┛\n");    //해당되는 줄을 보여줌
                fprintf(fp2,"총%4d건\n",count);    //해당되는 줄을 보여줌
            }
            else if (is_typepds) {
                fprintf(fp2,"┗━━━━━━━┻━━━━┻━━━━┻━━━━━━┻━━━┻━━━━━━━━┛\n");    //해당되는 줄을 보여줌
                fprintf(fp2,"총%4d건  총%d Mb  \n",count,it1/1024);    //해당되는 줄을 보여줌
            }
            else {
                fprintf(fp2,"┗━━━━━━━┻━━━━┻━━━━┻━━━━━━┻━━━┻━━━━━━━━┛\n");    //해당되는 줄을 보여줌
                fprintf(fp2,"총%4d건  \n",count);    //해당되는 줄을 보여줌
            }
            fclose(fp1);
			fclose(fp2);
			if (count<1) mess("해당되는 접속기록이 없습니다.");
			else		 view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
	}
}



set_jpgcfg()
{
	int n,i,n1,n2;
	char buff[100];
	while (1) {
		cls();
		print_(1,"%8s 화상 게시판 관련 설정",go.code);
		printf("  1. 화상게시판 내용 X 위치    (0.기본 1 - 80) :%d \n",bcfg.jpg_x);
		printf("  2. 화상게시판 내용 Y 위치    (0.기본 1 - 25) :%d \n",bcfg.jpg_y);
		printf("  3. 비 화상    내용 X 위치    (0.기본 1 - 80) :%d \n",bcfg.disp_x);
		printf("  4. 비 화상    내용 Y 위치    (0.기본 1 - 25) :%d \n",bcfg.disp_y);
		printf("  5. 번호 선택시 번호.jpg 보여줌               :%s \n",rbit(bcfg.jpgmode,1,"보통","지정"));
		printf("  6. 그림보기후 Enter -> 목록으로 돌아옴       :%s \n",rbit(bcfg.jpgmode,2,"돌아옴","남음"));
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
		if (n==1) bcfg.jpg_x	 =get_num(bcfg.jpg_x	 ,2);
		if (n==2) bcfg.jpg_y	 =get_num(bcfg.jpg_y	 ,2);
		if (n==3) bcfg.disp_x	 =get_num(bcfg.disp_x	 ,2);
		if (n==4) bcfg.disp_y	 =get_num(bcfg.disp_y	 ,2);
		if (n==5) bcfg.jpgmode	 =gtt_bit(bcfg.jpgmode,1);
		if (n==6) bcfg.jpgmode	 =gtt_bit(bcfg.jpgmode,2);
	}
}



set_reccfg()
{
	int n,i,n1,n2;
	char buff[100];
	while (1) {
		cls();
		printf("              %8s 게시판 기록 관련 설정\n",go.code);
		printf("  1. 조회시 조회현황 menudata/menurec 기록     :%s \n",rbit(bcfg.recmode,1,"함","안함"));
		printf("  2. 게시판 진입/종결시 기록                   :%s \n",rbit(bcfg.recmode,2,"함","안함"));
		printf("  3. 조회시 읽기수 증가 함                     :%s \n",rbit(bcfg.recmode,3,"함","안함"));
		printf("  4. 조회시 다운수 증가 함                     :%s \n",rbit(bcfg.recmode,4,"안함","함"));
		printf("  5. 조회시 무조건 증가 함 (등록자여부비교안함):%s \n",rbit(bcfg.recmode,5,"안함","함"));
		printfL2();
		printf(" 선택 : >>");
		n=numinput();
		printf("\n");
        if (n<=0) break;
		if (n==1) bcfg.recmode=gtt_bit(bcfg.recmode,1);
		if (n==2) bcfg.recmode=gtt_bit(bcfg.recmode,2);
		if (n==3) bcfg.recmode=gtt_bit(bcfg.recmode,3);
		if (n==4) bcfg.recmode=gtt_bit(bcfg.recmode,4);
		if (n==5) bcfg.recmode=gtt_bit(bcfg.recmode,5);
	}
}



bbs_all_del()
{
	int n,i,n1,n2;
	char buff[100];
	while (1) {
		cls();
        print_("%8s 게시판 전체 삭제/복구",go.code);
		printf("  1. 전체게시물 삭제 처리                                \n");
		printf("  2. 전체게시물 복구 처리                                \n");
        printf("  3. 같은 자료 지우기 (제목/화일명/내용길이)             \n");
        printf("  6. 내용중 일정 BYTE이하 게시물 전부 삭제처리           \n");
        printf("  7. 내용중 일정 BYTE이상 게시물 전부 삭제처리           \n");
        printf("  8. 전체게시물 완전 삭제                                \n");
        printf("  9. 제목중 특정 문자열 포함 자료 지우기           \n");
        printf(" 10. 특정 ID 자료 지우기           \n");
        printf(" 11. 다른게시판->자동등록                                \n");
		printf(" 선택 : >>");
		n=numinput();
		printf("\n");
             if (n<=0) break;
        else if (n==1) set_all_del(1);
        else if (n==2) set_all_del(2);
        else if (n==3) del_same();
        else if (n==6) del_all_size(0);
        else if (n==7) del_all_size(1);
        else if (n==8) del_allbbs();
        else if (n==9) del_strin();
        else if (n==10) del_strid();
        else if (n==11) b_add();
	}
}






bbs_all_set()
{
	int n;
	while (1) {
		cls();
		printf("              %8s 게시판 세팅\n",go.code);
		printfL2();
        printf("  1. 전체게시물 추천 전부 지우기                   \n");
        printf("  2. 전체게시물 OK,NO 전부 지우기                  \n");
        printf("  3. 전체게시물 암호 없애기                        \n");
        printf("  4. 전체게시물 강제색상 없애기                    \n");
        printf("  5. 전체게시물 다운수 없애기                      \n");
        printf("  6. 전체게시물 조회수 없애기                      \n");
        printf("  7. 전체게시물 키워드 없애기                      \n");
        printf("  8. 전체게시물 ID/이름 없애기                     \n");
        printf("  9. 전체게시물 날짜 오늘날짜로                    \n");
        printf(" 10. 전체게시물 자료크기비교후 일치파일명 갱신     \n");
        printf(" 11. 전체게시물 특정ID/이름으로 변경               \n");
        printf(" 12. 전체게시물 키워드 임의 정의                   \n");
        printf(" 14. 제목중 특정 문자열 문자 치환                  \n");
        printf(" 15. 특정ID/이름 검색후 변경                       \n");
        printf(" 16. 이상파일명 갱신(1번만 할것)                   \n");
        printf(" 17. 유사파일명 자동갱신                           \n");
		printfL2();
		printf(" 선택 : >>");
		n=numinput();
		printf("\n");
             if (n<=0)  break;
        else if (n==1)  bbs_del_all(1);
        else if (n==2)  bbs_del_all(2);
        else if (n==3)  bbs_del_all(3);
        else if (n==4)  bbs_del_all(4);
        else if (n==5)  bbs_del_all(5);
        else if (n==6)  bbs_del_all(6);
        else if (n==7)  bbs_del_all(7);
        else if (n==8)  bbs_del_all(8);
        else if (n==9)  bbs_del_all(9);
        else if (n==10) bbs_del_filename();
        else if (n==11) set_all_id();
        else if (n==12) set_all_key();
        else if (n==14) title_strchange();
        else if (n==15) id_strchange();
        else if (n==16) set_all_file(0);
        else if (n==17) bbs_del2_filename();
	}
}


set_gift()
{
	while (1) {
		cls();
		print_(0," 상품관련 설정");
		printf("  1. 1등 지정 ->   상 품 지 정          : %s\n",rbit(bcfg.form,11,"보통","지정"));
		printf("  2. 처음읽기 ->   상 품 지 정          : %s\n",rbit(bcfg.form,12,"보통","지정"));
		printf("  3. 처음다운 ->   상 품 지 정          : %s\n",rbit(bcfg.form,13,"보통","지정"));
		xy(1,20);printf(" 50. 상품 입력  레벨:%-8d  캐시:%-8d  다운:%-3dM\n",bcfg.uplevel,bcfg.upcash,bcfg.updown);
		n=numselect();
        if (n<= 0) break;
		if (n== 1) bcfg.form=gtt_bit(bcfg.form,11);
		if (n== 2) bcfg.form=gtt_bit(bcfg.form,12);
		if (n== 3) bcfg.form=gtt_bit(bcfg.form,13);
		if (n==50&&pf.type>18) {
			xy(30,20);bcfg.uplevel=get_num(bcfg.uplevel,8);
			xy(45,20);bcfg.upcash =get_num(bcfg.upcash ,8);
			xy(60,20);bcfg.updown =get_num(bcfg.updown ,3);
		}
	}
}



set_menucfg()
{
	int n,i,n1,n2,key;
	char buff[100];
	GO go2;
	FILE *fp;
	char menu_file[200];
	char filename[200];

	while (1) {
		if (bcfg.rdown>100) bcfg.rdown=100;
		if (bcfg.wdown>100) bcfg.wdown=100;
		del_esc(go.dir);
		cls();
		print_(2,"게시판 메뉴 세팅");
		printf("s  1. 메뉴 가기코드수정        :%s\n",go.code);
		printf("   2. 메뉴 이름 수정           :%s\n",go.name);
		printf("s  3. 메뉴 형태 지정           :%c\n",go.type);
		printf("s  4. 메뉴 디렉토리지정        :%s\n",go.dir );
        printf("   5.등급제한:  I:%2d      R:%2d      D:%2d      W:%2d   IL:%-8d IC:%-8d\n",
                bcfg.itype,bcfg.rtype,bcfg.dtype,bcfg.wtype,bcfg.ilevel,bcfg.icash);
        printf("   6.등급제한: FD:%2d     FR:%2d\n",bcfg.fdown,bcfg.fread);
        printf("s  7.보상설정: UR:%2d     MR:%2d     DD:%2d     MD:%2d   LV:%d   LC:%d\n",
                bcfg.wdown,bcfg.wdown2,bcfg.rdown,bcfg.rdown2,bcfg.idlevel,bcfg.idcash);
        printf("s  8.레벨보상: 쓰기:%3d  읽기:%3d  다운:%3d\n",
				bcfg.level,bcfg.level2,bcfg.level3);
        printf("s  9.Byte/1점 보상: %u \n",bcfg.level_size);
        printf("  10. 쓰기 제한                :%-3s  (D일 W주 M월 A전체) \n",go.wtimes);
        printf("  11. 하단 바로가기 메뉴       :%s.dim\n",go.code);
        printf("  12. 게시판 시간 제한         \n");
        printf("s 13. 진입암호 변경 현재       :%s\n",bcfg.passin);
        printf("s 14. 담당암호 변경 현재       :%s\n",is_same(bcfg.man1,pf.id)?bcfg.passin:"****");
        printf("s 15. 진입제한                 :%s\n",rbit(bcfg.mode,0,"보통","제한"));
        printf("  16. 게시판 담당자            :(%-8s %-8s %-8s %-8s %-8s)\n",bcfg.man1,bcfg.man2,bcfg.man3,bcfg.man4,bcfg.man5);
		n=numselect();
		xy(50,n+1);
        if (n<= 0) break;
        if (n== 1&&pf.type>=17) get_str(go.code,8);
		if (n== 2) get_str(go.name,30);
        if (n== 3&&(pf.type>=17||cpf.type>=17)) {
            sprintf(buff,"%c",go.type);
            get_str(buff,3);
            check_han(buff);
            go.type=buff[0];
        }
        if (n== 4&&pf.type>=17) {
			strcpy(buff,go.dir);
			get_str(buff,60);check_han(buff);strlwr(buff);
			strchng2(buff,"_S"," ");all_trim(buff);
			//문자열에 화일속의 내용이 포함되면 Yes;
			if (strlen(buff)>0) {
				if (pf.type<19&&((stristr(buff,"bin/")||stristr(buff,"system/")||
					stristr(buff,";")||stristr(buff,"&")||
					stristr(buff,"*")||stristr(buff,"?")||
					stristr(buff,"cfg/")||stristr(buff,"sys_back")||
					stristr(buff,"/etc/")||stristr(buff,"/system/"))));
				else if (in_file3("system/notdir.txt",buff));
				else strcpy(go.dir,buff);
			}
		}
        if (n== 5) {
            xy(18, n+1);bcfg.itype =get_num(bcfg.itype ,2);
            xy(28, n+1);bcfg.rtype =get_num(bcfg.rtype ,2);
            xy(38, n+1);bcfg.dtype =get_num(bcfg.dtype ,2);
            xy(48, n+1);bcfg.wtype =get_num(bcfg.wtype ,2);
            xy(58, n+1);bcfg.ilevel=get_num(bcfg.ilevel,9);
            xy(68, n+1);bcfg.icash =get_num(bcfg.icash ,9);
		}
        if (n== 6) {
            xy(18, n+1);bcfg.fdown =get_num(bcfg.fdown ,2);
            xy(28, n+1);bcfg.fread =get_num(bcfg.fread ,2);
		}
        if (n== 7&&(pf.type>=17||cpf.type>=17)) {
            xy(18, n+1);bcfg.wdown =get_num(bcfg.wdown ,2);
            xy(28, n+1);bcfg.wdown2=get_num(bcfg.wdown2,2);
            xy(38, n+1);bcfg.rdown =get_num(bcfg.rdown ,2);
            xy(48, n+1);bcfg.rdown2=get_num(bcfg.rdown2,2);
            xy(58, n+1);bcfg.idlevel=get_num(bcfg.idlevel,8);
            xy(68, n+1);bcfg.idcash=get_num(bcfg.idcash,8);
		}
        if (n== 8&&(pf.type>=17||cpf.type>=17)) {
            xy(18, n+1);bcfg.level =get_num(bcfg.level ,3);
            xy(28, n+1);bcfg.level2=get_num(bcfg.level2,3);
            xy(38, n+1);bcfg.level3=get_num(bcfg.level3,3);
		}
        if (n== 9&&(pf.type>=17||cpf.type>=17)) {
            bcfg.level_size=get_num(bcfg.level_size,10);
        }
        if (n==10) get_str(go.wtimes,4);
        if (n==11) xedit2("%s/logo/%s.dim",club_go.dir,go.code);
        if (n==12) set_timecfg();
        if (n==13&&(pf.type>=17||cpf.type>=17||is_same(bcfg.man1,bcfg.man1))) get_str(bcfg.passin,4);
        if (n==14&&(pf.type>=17||cpf.type>=17||is_same(bcfg.man1,bcfg.man1))) get_str(bcfg.passsys,4);
        if (n==15&&(pf.type>=17||cpf.type>=17)) bcfg.mode=gtt_bit(bcfg.mode,0);
        if (n==16&&(pf.type>=17||cpf.type>=17||is_same(bcfg.man1,bcfg.man1))) {
            get_str(bcfg.man1,8);get_str(bcfg.man2,8);get_str(bcfg.man3,8);
            get_str(bcfg.man4,8);get_str(bcfg.man5,8);
		}
	}
    if (strlen(bcfg.passin)<2) strcpy(bcfg.passin,"");
    if (strlen(bcfg.passsys)<2) strcpy(bcfg.passsys,"");
}


set_timecfg()
{
	int n,i,n1,n2,key;
	char buff[100];
	GO go2;
	FILE *fp;
	char menu_file[200];
	char filename[200];

	while (1) {
		cls();
        print_(2,"게시판 시간제한");
        printf("  1. 메뉴 진입가능 시간       :%s \n",time_limitstr(bcfg.itime));
        printf("  2. 메뉴 다운가능 시간       :%s \n",time_limitstr(bcfg.dtime));
        printf("  3. 메뉴 읽기가능 시간       :%s \n",time_limitstr(bcfg.rtime));
        printf("  4. 메뉴 등록가능 시간       :%s \n",time_limitstr(bcfg.wtime));
		n=numselect();
		xy(50,n+1);
        if (n<= 0) break;
        if (n== 1) bcfg.itime=get_num(bcfg.itime,8);
        if (n== 2) bcfg.dtime=get_num(bcfg.dtime,8);
        if (n== 3) bcfg.rtime=get_num(bcfg.rtime,8);
        if (n== 4) bcfg.wtime=get_num(bcfg.wtime,8);
    }
}


set_bcfg1()
{
	while (1) {
		cls();
		print_(0,"게시판 환경 설정");
		printf("  1. 손님시 진입시 ID질문                      :%s\n",rbit(bcfg.mode,18,"보통","질문"));
		printf("  2. 손님시 등록시 ID질문                      :%s\n",rbit(bcfg.mode,19,"보통","질문"));
		printf("  3. 일정날짜 경과자료 자동삭제(0:무시 ?.삭제) :%d\n",bcfg.outday);
		printf("  4. 외부 등록 호출 프로그램                   :%s\n",rbit(bcfg.mode,2 ,"불가","호출"));
		printf("  5. KEY = 답장/답변자동처리                   :%s\n",rbit(bcfg.mode,11,"보통","자동"));
		printf("  6. 내용 PR/DN 금지                           :%s\n",rbit(bcfg.form,1 ,"보통","금지"));
		printf("  7. 이동/복사  금지                           :%s\n",rbit(bcfg.form,2 ,"보통","금지"));
		printf("  8. 삭제/편집  금지                           :%s\n",rbit(bcfg.form,3 ,"보통","금지"));
		printf("  9. 지정된포트 등록/쓰기 금지                 :%s\n",rbit(bcfg.mode,28,"허용","금지"));
		printf(" 10. 지정된포트 다운/읽기 금지                 :%s\n",rbit(bcfg.mode,29,"허용","금지"));
		printf(" 11. 게시판에서 별명사용                       :%s\n",rbit(bcfg.mode,8 ,"본명사용","별명사용"));
		printf(" 12. 키워드사용                                :%s\n",rbit(bcfg.mode,9 ,"사용안함","사용함"));
		printf(" 13. 진입시 Enter 키입력 받음                  :%s\n",rbit(bcfg.form,4 ,"통과","멈춤"));
		printf(" 14. 진입시 제일 마지막 게시판 읽음            :%s\n",rbit(bcfg.form,5 ,"보통","읽음"));
		printf(" 15. 진입시 무조건 글쓰기 모드                 :%s\n",rbit(bcfg.form,6 ,"보통","쓰기"));
		printf(" 16. 담당,등급14 삭제된 게시물 읽기            :%s\n",rbit(bcfg.mode,21,"가능","불가"));
        printf(" 17. 게시판 읽기/다운시 암호 질문              :%s\n",rbit(bcfg.mode,31,"확인","무시"));
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
		if (n== 1) bcfg.mode=gtt_bit(bcfg.mode,18);
		if (n== 2) bcfg.mode=gtt_bit(bcfg.mode,19);
		if (n== 3) bcfg.outday=get_num(bcfg.outday,8);
		if (n== 4) bcfg.mode=gtt_bit(bcfg.mode,2 );
		if (n== 5) bcfg.mode=gtt_bit(bcfg.mode,11);
		if (n== 6) bcfg.form=gtt_bit(bcfg.form,1 );
		if (n== 7) bcfg.form=gtt_bit(bcfg.form,2 );
		if (n== 8) bcfg.form=gtt_bit(bcfg.form,3 );
		if (n== 9) bcfg.mode=gtt_bit(bcfg.mode,28);
		if (n==10) bcfg.mode=gtt_bit(bcfg.mode,29);
		if (n==11) bcfg.mode=gtt_bit(bcfg.mode,8 );
		if (n==12) bcfg.mode=gtt_bit(bcfg.mode,9 );
		if (n==13) bcfg.form=gtt_bit(bcfg.form,4 );
		if (n==14) bcfg.form=gtt_bit(bcfg.form,5 );
		if (n==15) bcfg.form=gtt_bit(bcfg.form,6 );
        if (n==16) bcfg.mode=gtt_bit(bcfg.mode,31);
        if (n==17) bcfg.mode=gtt_bit(bcfg.mode,31);
	}
}

set_limit()
{
	while (1) {
		cls();
		print_(0,"게시판 진입제한/감춤/보안 설정");
        printf("  1. 쓰기시 아이디/이름삭제처리                :%s\n",rbit(bcfg.mode,5,"보통","감춤"));
        printf("  2. 일반회원에게 아이디감춤                   :%s\n",rbit(bcfg.mode,6,"보통","감춤"));
        printf("  3. 전체회원에게 아이디감춤                   :%s\n",rbit(bcfg.mode,7,"보통","감춤"));
        printf("  4. 진입시 가기코드 감춤                      :%s\n",rbit(bcfg.mode,15,"보통","감춤"));
		printf("  5. 읽을때 지정된 아이디 (%s.lir) \n",go.code);
		printf("  6. 쓸때   지정된 아이디 (%s.liw) \n",go.code);
        printf("  7. 읽을때 지정된 아이디만가능                :%s\n",rbit(bcfg.modelist,5,"보통","제한"));
        printf("  8. 쓸때   지정된 아이디만가능                :%s\n",rbit(bcfg.modelist,6,"보통","제한"));
		printf("  9. 진입시 일일 일정 쓰기수 이상 진입         :%d \n",bcfg.li_w);
        printf(" 10. 남자 진입가능              (가기코드.lm1) :%s\n",rbit(bcfg.imode,11,"가능","불가"));
        printf(" 11. 남자 쓰기가능              (가기코드.lm3) :%s\n",rbit(bcfg.imode,12,"가능","불가"));
        printf(" 12. 여자 진입가능              (가기코드.lm5) :%s\n",rbit(bcfg.imode,13,"가능","불가"));
        printf(" 13. 여자 쓰기가능              (가기코드.lm7) :%s\n",rbit(bcfg.imode,14,"가능","불가"));
        printf(" 14. 19세미만 진입가능          (가기코드.lm2) :%s\n",rbit(bcfg.imode,15,"가능","불가"));
        printf(" 15. 19세미만 쓰기가능          (가기코드.lm4) :%s\n",rbit(bcfg.imode,16,"가능","불가"));
        printf(" 16. 19세이상 진입가능          (가기코드.lm6) :%s\n",rbit(bcfg.imode,17,"가능","불가"));
        printf(" 17. 19세이상 쓰기가능          (가기코드.lm8) :%s\n",rbit(bcfg.imode,18,"가능","불가"));
        printf(" 18. 진입가능 ID 지정 (%s.sid)                  \n",go.code);
        printf(" 19. 진입제한 ID 지정 (%s.sii)                  \n",go.code);
        printf(" 20. lt,li,ln 등의 검색명령                    :%s\n",rbit(bcfg.modelist,20,"가능","불가"));
        printf(" 21. cp,mv,dn,pr 등의 검색명령                 :%s\n",rbit(bcfg.modelist,21,"가능","불가"));
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
		if (n==1) bcfg.mode=gtt_bit(bcfg.mode,5);
		if (n==2) bcfg.mode=gtt_bit(bcfg.mode,6);
		if (n==3) bcfg.mode=gtt_bit(bcfg.mode,7);
		if (n==4) bcfg.mode=gtt_bit(bcfg.mode,15);
		if (n==5) xedit2("%s/logo/%s.lir",club_go.dir,go.code);
		if (n==6) xedit2("%s/logo/%s.liw",club_go.dir,go.code);
		if (n==7) bcfg.modelist=gtt_bit(bcfg.modelist,5);
		if (n==8) bcfg.modelist=gtt_bit(bcfg.modelist,6);
		if (n==9) bcfg.li_w    =get_num(bcfg.li_w,3);
        if (n==10) bcfg.imode=gtt_bit(bcfg.imode,11);
        if (n==11) bcfg.imode=gtt_bit(bcfg.imode,12);
        if (n==12) bcfg.imode=gtt_bit(bcfg.imode,13);
        if (n==13) bcfg.imode=gtt_bit(bcfg.imode,14);
        if (n==14) bcfg.imode=gtt_bit(bcfg.imode,15);
        if (n==15) bcfg.imode=gtt_bit(bcfg.imode,16);
        if (n==16) bcfg.imode=gtt_bit(bcfg.imode,17);
        if (n==17) bcfg.imode=gtt_bit(bcfg.imode,18);
        if (n==18) xedit2("%s/logo/%s.sid",club_go.dir,go.code);
        if (n==19) xedit2("%s/logo/%s.sii",club_go.dir,go.code);
        if (n==20) bcfg.modelist=gtt_bit(bcfg.modelist,20);
        if (n==21) bcfg.modelist=gtt_bit(bcfg.modelist,21);
	}
}




set_updown()
{
	int n,i,n1,n2;
    char buff[200];
	while (1) {
		cls();
		print_(0,"자료 업/다운 환경 설정");
		printf("  1. 일반회원(0~14) 화일전송 금지              :%s\n",rbit(bcfg.downmode, 1,"허가","금지"));
		printf("  2. 운영진 (14~19) 화일전송 금지              :%s\n",rbit(bcfg.downmode, 2,"허가","금지"));
		printf("  3. 화일없음 전송 ->취소처리                  :%s\n",rbit(bcfg.downmode,11,"취소처리","전송허용"));
		printf("  4. 화일없음 자료 ->삭제처리                  :%s\n",rbit(bcfg.downmode,12,"삭제보류","자동삭제"));
		printf("  5. 화일없음 자료 ->파일명 공백처리           :%s\n",rbit(bcfg.downmode,13,"공백처리","화일없음"));
		printf("  6. 자료번호 선택시 자동다운                  :%s\n",rbit(bcfg.downmode,14,"보통","자동다운"));
		printf("  7. 중복자료(화일명동일)등록 처리             :%s\n",rbit(bcfg.downmode,15,"새로지정","덮어쒸움"));
		printf("  8. 등록동의,질문에서 Enter시 기본답변        :%s\n",rbit(bcfg.downmode,16,"Yes","No"));
		printf("  9. 자료 다운후 완료메세지+ENTER              :%s\n",rbit(bcfg.downmode,17,"Enter","무시"));
		printf(" 10. 자료 등록후 완료메세지+ENTER              :%s\n",rbit(bcfg.downmode,18,"Enter","무시"));
		printf(" 11. 가장 먼저 다운받는사람 파일삭제함         :%s\n",rbit(bcfg.downmode,19,"보통","자동삭제"));
		printf(" 12. 삭제시 다운용량 삭제함                    :%s\n",rbit(bcfg.mode,1,"삭제","무시"));
		printf(" 13. 3일전 삭제시 다운용량 삭제함              :%s\n",rbit(bcfg.mode,2,"삭제","무시"));
		printf(" 14. 업  ->다운용량보상                        :%3.1f 배\n",(float)bcfg.urate/10 );
		printf(" 15. 다운->다운용량감소                        :%3.1f 배\n",(float)bcfg.drate/10 );
		printf(" 16. 시간지정 업  ->다운용량 보상  %2d-%2d     :%3.1f 배\n",bcfg.uhour1,bcfg.uhour2,(float)bcfg.uratet/10 );
		printf(" 17. 시간지정 다운->다운용량 감소  %2d-%2d     :%3.1f 배\n",bcfg.dhour1,bcfg.dhour2,(float)bcfg.dratet/10 );
		printf(" 18. 동시 업   등록 수 제한  (0:무제한)        :%d\n",bcfg.up_limit   );
		printf(" 19. 동시 다운 전송 수 제한  (0:무제한)        :%d\n",bcfg.dn_limit   );
        printf(" 20. 멀티 다운 전송 수 제한  (0:무제한)        :%d\n",bcfg.dn_mult_limit);
        printf(" 21. 현재 다운 임시 금지 메세지 (logo/%s.ndn)\n",go.code);
        printf(" 22. 전체 게시판 BDN 가능 지정                 :%s\n",rbit(bcfg.downmode,20,"불가능","가능"));
        printf(" 23. BDN 가능 ID 지정           (logo/%s.bdn)\n",go.code);
		n=numselect();
		xy(50,n+1);
        if (n<= 0) break;
		if (n== 1) bcfg.downmode =gtt_bit(bcfg.downmode, 1);
		if (n== 2) bcfg.downmode =gtt_bit(bcfg.downmode, 2);
		if (n== 3) bcfg.downmode =gtt_bit(bcfg.downmode,11);
		if (n== 4) bcfg.downmode =gtt_bit(bcfg.downmode,12);
		if (n== 5) bcfg.downmode =gtt_bit(bcfg.downmode,13);
		if (n== 6) bcfg.downmode =gtt_bit(bcfg.downmode,14);
		if (n== 7) bcfg.downmode =gtt_bit(bcfg.downmode,15);
		if (n== 8) bcfg.downmode =gtt_bit(bcfg.downmode,16);
		if (n== 9) bcfg.downmode =gtt_bit(bcfg.downmode,17);
		if (n==10) bcfg.downmode =gtt_bit(bcfg.downmode,18);
		if (n==11) bcfg.downmode =gtt_bit(bcfg.downmode,19);
		if (n==12) bcfg.mode =gtt_bit(bcfg.mode ,1);
		if (n==13) bcfg.mode =gtt_bit(bcfg.mode ,2);
		if (n==14) bcfg.urate=get_num(bcfg.urate,3);
		if (n==15) bcfg.drate=get_num(bcfg.drate,3);
		if (n==16)	{
			bcfg.uhour1=get_num(bcfg.uhour1,2);bcfg.uhour2=get_num(bcfg.uhour2,2);
			bcfg.uratet=get_num(bcfg.uratet,3);
		}
		if (n==17)	{
			bcfg.dhour1=get_num(bcfg.dhour1,2);bcfg.dhour2=get_num(bcfg.dhour2,2);
			bcfg.dratet=get_num(bcfg.dratet,3);
		}
		if (n==18)	bcfg.up_limit	=get_num(bcfg.up_limit,3);
		if (n==19)	bcfg.dn_limit	=get_num(bcfg.dn_limit,3);
		if (n==20)	bcfg.dn_mult_limit=get_num(bcfg.dn_mult_limit,3);
        if (n==21)  xedit2("%s/logo/%s.ndn",club_go.dir,go.code);
        if (n==22)  bcfg.downmode =gtt_bit(bcfg.downmode,20);
        if (n==23)  xedit2("%s/logo/%s.bdn",club_go.dir,go.code);
	}
}



set_write()
{
	while (1) {
        if (strlen(bcfg.outbbs)<2) strcpy(bcfg.outbbs,"");
		cls();
		print_(0,"게시판 쓰기 관련 설정");
		printf("  1. 쓰기시 기본지정머리말     (0.보통 ?.강제) :%d\n",bcfg.headnum);
		printf("  2. 쓰기시 매일 선착순제한    (0.보통 ?.제한) :%d\n",bcfg.runday);
		printf("  3. 쓰기시 게시물내용크기 제한(0.보통 ?.제한) :%d\n",bcfg.limitsize);
		printf("  4. 쓰기시 자동으로 지정게시판에 내용 복사    :%s\n",bcfg.outbbs);
		printf("  5. 쓰기시 자동으로 지정게시판 쓰기           :%s\n",rbit(bcfg.wmode,23,"무시","쓰기"));
		printf("  6. 쓰기시 자동으로 지정게시판 복사           :%s\n",rbit(bcfg.wmode,24,"무시","복사"));
        printf("  7. 쓰기시 무조건 추천으로 지정               :%s\n",rbit(bcfg.wmode,25,"무시","지정"));
        printf("  8. 쓰기시 10페이지 (250줄)이상 게시물 금지   :%s\n",rbit(bcfg.wmode,26,"무시","금지"));
        printf("  9. 시삽/게시판담당이 쓰기시 사용하는ID       :%s\n",bcfg.write_id);
        xy(1,11);
        printf("11.머리말 동작여부 %s\n",rbit(bcfg.wmode,1,"동작","통과"));
        printf("12.머리말 강제여부 %s\n",rbit(bcfg.wmode,2,"선택","강제"));
        printf("13.쓰기완료 메세지 %s\n",rbit(bcfg.wmode,3,"Enter","무시"));
        printf("14.제목입력안함    %s\n",rbit(bcfg.wmode,4,"입력","무시"));
        printf("15.제목입력내용첫줄%s\n",rbit(bcfg.wmode,5,"보통","첫줄대치"));
        printf("16.편집기1 강제지정%s\n",rbit(bcfg.wmode,6,"보통","강제지정"));
        printf("17.                %s\n",rbit(bcfg.wmode,7,"보통","지정"));
        printf("18.쓰기->메일쓰기  %s\n",rbit(bcfg.wmode,8,"보통","발송"));
        printf("19.개인사인 첨부   %s\n",rbit(bcfg.wmode,9,"허용","금지"));
        printf("20.KEY 입력        %s\n",rbit(bcfg.wmode,10,"통과","질문"));
        xy(40,11);printf(" 21.내용입력받음    %s\n",rbit(bcfg.wmode,11,"보통","통과"));
        xy(40,12);printf(" 22.내용없음->취소  %s\n",rbit(bcfg.wmode,12,"보통","취소"));
        xy(40,13);printf(" 23.내용 Zmodem금지 %s\n",rbit(bcfg.wmode,13,"보통","금지"));
        xy(40,14);printf(" 24.내용 Zmodem등록 %s\n",rbit(bcfg.wmode,14,"보통","강제"));
        xy(40,15);printf(" 25.bbs/pds_all저장 %s\n",rbit(bcfg.wmode,15,"저장","안함"));
        xy(40,16);printf("26.자동담당2메일  %s\n",rbit(bcfg.wmode,16,"보통","발송"));
        xy(40,17);printf("27.10 Byte미만취소%s\n",rbit(bcfg.wmode,17,"무시","취소"));
        xy(40,18);printf("28.주의사항 경고  %s\n",rbit(bcfg.wmode,18,"함","무시"));
        xy(40,19);printf("29.내용 자동 전송 %s\n",rbit(bcfg.wmode,19,"선택","Zmodem자동"));
        xy(40,20);printf("30.파일 자동 전송 %s\n",rbit(bcfg.wmode,20,"선택","Zmodem자동"));
        xy(40,21);printf("31.등록 확인      %s\n",rbit(bcfg.wmode,21,"확인","무시"));
        xy(40,22);printf("32.레벨증가메세지 %s\n",rbit(bcfg.wmode,22,"출력","무시"));
		xy(1,23);
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
		if (n== 1) bcfg.headnum=get_num(bcfg.headnum,2);
		if (n== 2) bcfg.runday =get_num(bcfg.runday ,2);
		if (n== 3) bcfg.limitsize=get_num(bcfg.limitsize,10);
		if (n== 4) get_str(bcfg.outbbs,8);
		if (n== 5) bcfg.wmode=gtt_bit(bcfg.wmode,23);
		if (n== 6) bcfg.wmode=gtt_bit(bcfg.wmode,24);
        if (n== 7) bcfg.wmode=gtt_bit(bcfg.wmode,25);
        if (n== 8) bcfg.wmode=gtt_bit(bcfg.wmode,26);
        if (n== 9) get_str(bcfg.write_id,8);
		if (n==11) bcfg.wmode=gtt_bit(bcfg.wmode,1);
		if (n==12) bcfg.wmode=gtt_bit(bcfg.wmode,2);
		if (n==13) bcfg.wmode=gtt_bit(bcfg.wmode,3);
		if (n==14) bcfg.wmode=gtt_bit(bcfg.wmode,4);
		if (n==15) bcfg.wmode=gtt_bit(bcfg.wmode,5);
		if (n==16) bcfg.wmode=gtt_bit(bcfg.wmode,6);
		if (n==17) bcfg.wmode=gtt_bit(bcfg.wmode,7);
		if (n==18) bcfg.wmode=gtt_bit(bcfg.wmode,8);
		if (n==19) bcfg.wmode=gtt_bit(bcfg.wmode,9);
		if (n==20) bcfg.wmode=gtt_bit(bcfg.wmode,10);
		if (n==21) bcfg.wmode=gtt_bit(bcfg.wmode,11);
		if (n==22) bcfg.wmode=gtt_bit(bcfg.wmode,12);
		if (n==23) bcfg.wmode=gtt_bit(bcfg.wmode,13);
		if (n==24) bcfg.wmode=gtt_bit(bcfg.wmode,14);
		if (n==25) bcfg.wmode=gtt_bit(bcfg.wmode,15);
		if (n==26) bcfg.wmode=gtt_bit(bcfg.wmode,16);
		if (n==27) bcfg.wmode=gtt_bit(bcfg.wmode,17);
		if (n==28) bcfg.wmode=gtt_bit(bcfg.wmode,18);
		if (n==29) bcfg.wmode=gtt_bit(bcfg.wmode,19);
		if (n==30) bcfg.wmode=gtt_bit(bcfg.wmode,20);
		if (n==31) bcfg.wmode=gtt_bit(bcfg.wmode,21);
		if (n==32) bcfg.wmode=gtt_bit(bcfg.wmode,22);
	}
}


set_auto()
{
	while (1) {
		cls();
		print_(0,"게시판 자동 처리 관련 설정");
		printf("  1. 디렉토리지정 자동 내용처리 (번호.txt)     :%s\n",rbit(bcfg.automode,1,"보통","자동"));
		printf("  2. 디렉토리지정 자동 제목처리 (번호.txt)     :%s\n",rbit(bcfg.automode,2,"보통","자동"));
		printf("  3. 디렉토리지정 자동 파일처리 (번호.txt)     :%s\n",rbit(bcfg.automode,3,"보통","자동"));
		printf("  4. 자동제목/파일줄삭제한뒤 내용처리          :%s\n",rbit(bcfg.automode,4,"보통","삭제"));
		printf("  5. 새로읽을때 날짜,크기 비교한뒤 새로읽음    :%s\n",rbit(bcfg.automode,5,"항상새로","비교후"));
		printf("  6. 번호.TXT대신 키워드.TXT로  (KEY.txt)      :%s\n",rbit(bcfg.automode,6,"번호","KEY"));
		printf("                                                  \n");
		printf("                                                  \n");
		printf("                                                  \n");
		printf("                                                  \n");
		printf(" 11. 내용 읽은뒤 자동 삭제처리                 :%s\n",rbit(bcfg.automode,11,"보통","삭제"));
		printf(" 12. 내용 읽은뒤 파일 자동삭제                 :%s\n",rbit(bcfg.automode,12,"보통","삭제"));
		printf("                                                  \n");
		printf("                                                  \n");
		printf(" 15. 읽기시 특정 게시판 자동 불러오기 지정     :%s\n",rbit(bcfg.automode,15,"보통","자동부르기"));
		printf(" 16. 읽기시 날짜 비교후 불러오기               :%s\n",rbit(bcfg.automode,16,"비교후","무조건"));
		printf(" 17. 읽기시 검색 대상 키워드 검색 방법         :%s\n",rbit(bcfg.automode,17,"제목검색","키워드검색"));
		printf(" 18. 읽기시 자동으로 불러올 게시판             :%s\n",bcfg.inbbs);
		printf(" 19. 읽기시 자동으로 불러올 게시판 키워드      :%s\n",bcfg.inbbskey);
		n=numselect();
		xy(49,n+1);
        if (n<=0) break;
		if (who<3) {mess("지정 불가능");break;}
		if (n== 1) bcfg.automode=gtt_bit(bcfg.automode,1);
		if (n== 2) bcfg.automode=gtt_bit(bcfg.automode,2);
		if (n== 3) bcfg.automode=gtt_bit(bcfg.automode,3);
		if (n== 4) bcfg.automode=gtt_bit(bcfg.automode,4);
		if (n== 5) bcfg.automode=gtt_bit(bcfg.automode,5);
		if (n== 6) bcfg.automode=gtt_bit(bcfg.automode,6);
		if (n==11) bcfg.automode=gtt_bit(bcfg.automode,11);
		if (n==12) bcfg.automode=gtt_bit(bcfg.automode,12);
		if (n==15) bcfg.automode=gtt_bit(bcfg.automode,15);
		if (n==16) bcfg.automode=gtt_bit(bcfg.automode,16);
		if (n==17) bcfg.automode=gtt_bit(bcfg.automode,17);
		if (n==18) get_str(bcfg.inbbs,8);
		if (n==19) get_str(bcfg.inbbskey,10);
	}
}


set_listcfg()
{
	while (1) {
		cls();
		print_(0,"게시판 목록 모양 관련 설정");
		printf(" 1. 게시물 목록(LIST) 감추기                  :%s \n",rbit(bcfg.modelist,7,"보임","감춤"));
		printf(" 2. 게시물 자료실 BYTE 형태       99999B      :%s \n",rbit(bcfg.modelist,11,"보통","지정"));
		printf(" 3. 게시물 자료실 BYTE 형태       99999K      :%s \n",rbit(bcfg.modelist,12,"보통","지정"));
		printf(" 4. 게시물 자료실 BYTE 형태       99999M      :%s \n",rbit(bcfg.modelist,13,"보통","지정"));
		printf(" 5. 게시물 자료실 BYTE 형태                   :%s \n",rbit(bcfg.modelist,14,"보통","공백"));
		printf(" 6. 게시물 한화면 출력 자료수     (0-120)     :%d \n",bcfg.maxline);
		printf(" 7. 크기 0인 자료 안보이기                    :%s \n",rbit(bcfg.modelist,19,"안보임","보임"));
		printf(" 8. 게시물 목록 출력 위치 (X)                 :%d \n",bcfg.list_x);
		printf(" 9. 게시물 목록 출력 위치 (Y)                 :%d \n",bcfg.list_y);
        printf("10. 추천자료만 보이기                         :%s \n",rbit(bcfg.modelist,22,"보통","추천자료만보임"));
		n=numselect();
        if (n<=0) break;
		if (n==1) bcfg.modelist=gtt_bit(bcfg.modelist,7);
		if (n==2) bcfg.modelist=gtt_bit(bcfg.modelist,11);
		if (n==3) bcfg.modelist=gtt_bit(bcfg.modelist,12);
		if (n==4) bcfg.modelist=gtt_bit(bcfg.modelist,13);
		if (n==5) bcfg.modelist=gtt_bit(bcfg.modelist,14);
		if (n==6) bcfg.maxline =get_num(bcfg.maxline , 2);
		if (n==7) bcfg.modelist=gtt_bit(bcfg.modelist,19);
		if (n==8) bcfg.list_x  =get_num(bcfg.list_x  , 2);
		if (n==9) bcfg.list_y  =get_num(bcfg.list_y  , 2);
        if (n==10) bcfg.modelist=gtt_bit(bcfg.modelist,22);
	}
}


//잠깐 설문 관련
set_quest()
{
	while (1) {
		cls();
		print_(0,"잠깐 설문 기능 설정");
		printf(" 1. 메뉴     진입전 질문                      :%s\n",rbit(bcfg.qmode, 1,"감춤","질문"));
		printf(" 2. 게시물   화면 출력후 질문                 :%s\n",rbit(bcfg.qmode, 2,"감춤","질문"));
		printf(" 3. 게시물   재진입시 결과 보임               :%s\n",rbit(bcfg.qmode, 3,"감춤","보임"));
		printf(" 4. 서브메뉴 화면 출력후 질문                 :%s\n",rbit(bcfg.qmode, 4,"감춤","질문"));
		printf(" 5. 게시물   화면 출력후 질문                 :%s\n",rbit(bcfg.qmode, 5,"감춤","질문"));
		printf(" 6. 게시물   재진입시 결과 보임               :%s\n",rbit(bcfg.qmode, 6,"감춤","보임"));
		printf(" 7. 쓰기시 기본설명 동작여부                  :%s\n",rbit(bcfg.qmode, 7,"통과","동작"));
		printf(" 8. 쓰기시 기본설명 강제여부                  :%s\n",rbit(bcfg.qmode, 8,"선택","강제"));
		printf(" 9. 쓰기시 설문조사 동작여부                  :%s\n",rbit(bcfg.qmode, 9,"동작","통과"));
		printf("10. 쓰기시 설문조사 강제여부                  :%s\n",rbit(bcfg.qmode,10,"선택","강제"));
		printf("11. 쓰기시 외부 기본설명사용                  :%s\n",rbit(bcfg.qmode,11,"보통","외부설명"));
		n=numselect();
        if (n<=0) break;
		if (n== 1) bcfg.qmode=gtt_bit(bcfg.qmode, 1);
		if (n== 2) bcfg.qmode=gtt_bit(bcfg.qmode, 2);
		if (n== 3) bcfg.qmode=gtt_bit(bcfg.qmode, 3);
		if (n== 4) bcfg.qmode=gtt_bit(bcfg.qmode, 4);
		if (n== 5) bcfg.qmode=gtt_bit(bcfg.qmode, 5);
		if (n== 6) bcfg.qmode=gtt_bit(bcfg.qmode, 6);
		if (n== 7) bcfg.qmode=gtt_bit(bcfg.qmode, 7);
		if (n== 8) bcfg.qmode=gtt_bit(bcfg.qmode, 8);
		if (n== 9) bcfg.qmode=gtt_bit(bcfg.qmode, 9);
		if (n==10) bcfg.qmode=gtt_bit(bcfg.qmode,10);
		if (n==11) bcfg.qmode=gtt_bit(bcfg.qmode,11);
	}
}



//찬성반대관련
set_ok()
{
	char buff[200];
	while (1) {
		cls();
		print_(0,"잠깐 설문 기능 설정");
		printf(" 1. 찬성 반대 강제 질문                       :%s \n",rbit(bcfg.modeok,1,"안함","함"));
		printf(" 2. 찬성 반대 순서 임의 발생                  :%s \n",rbit(bcfg.modeok,2,"순서대로","임의순서"));
		printf(" 3. 투표후 찬성 결과 보여 주기                :%s \n",rbit(bcfg.modeok,3,"보여줌","무시"));
		printf(" 4. 취소 허용                                 :%s \n",rbit(bcfg.modeok,3,"허용","불허"));
		printf(" 5. 찬성 질문 안내 화면 편집                       :(logo/%s.ok1)\n",go.code);
		printf("11. 보존 보류 강제 질문                       :%s \n",rbit(bcfg.modeok,11,"안함","함"));
		printf("12. 보존 보류 순서 임의 발생                  :%s \n",rbit(bcfg.modeok,12,"순서대로","임의순서"));
		printf("13. 투표후 찬성 결과 보여 주기                :%s \n",rbit(bcfg.modeok,13,"보여줌","무시"));
		printf("14. 기권 허용                                 :%s \n",rbit(bcfg.modeok,13,"허용","불허"));
		printf("15. 찬성 질문 안내 화면 편집                       :(logo/%s.ok2)\n",go.code);
		n=numselect();
        if (n<= 0) break;
		if (n== 1) bcfg.modeok=gtt_bit(bcfg.modeok, 1);
		if (n== 2) bcfg.modeok=gtt_bit(bcfg.modeok, 2);
		if (n== 3) bcfg.modeok=gtt_bit(bcfg.modeok, 3);
		if (n== 4) bcfg.modeok=gtt_bit(bcfg.modeok, 4);
		if (n== 5) {
			sprintf(buff,"%s/logo/%s.ok1",club_go.dir,go.code);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (n==11) bcfg.modeok=gtt_bit(bcfg.modeok,11);
		if (n==12) bcfg.modeok=gtt_bit(bcfg.modeok,12);
		if (n==13) bcfg.modeok=gtt_bit(bcfg.modeok,13);
		if (n==14) bcfg.modeok=gtt_bit(bcfg.modeok,14);
		if (n==15) {
			sprintf(buff,"%s/logo/%s.ok2",club_go.dir,go.code);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
	}
}



set_formcfg()
{
	int n,i,n1,n2;
	char buff[100];
	while (1) {
		cls();
		print_(0,"게시판 모양 관련 설정");
        printf("  1. 항목별 고정 칸수 지정  (FRM,DSP에서 사용기능)\n");
        printf("  2. LX 검색(자유항목)시 검색위치,내용 지정       \n");
		printf("  3. 내용 출력 위치                            :(X:%2d Y:%2d) \n",bcfg.disp_x,bcfg.disp_y);
        printf("  4. 내용 출력 줄수    지정 (0.무시 1-99.줄수) :%d\n",bcfg.listline);
        printf("  5. 내용 출력                                 :%s\n",rbit(bcfg.modelist,0,"함","안함"));
        printf("  6. 읽을때 내용->ansi화면형태                 :%s\n",rbit(bcfg.mode,24,"보통","안시"));
		printf("  7. 내용 상단 / 하단   고정줄위치             :(Y1:%2d Y2:%2d) \n",bcfg.pos_y1,bcfg.pos_y2);
        printf("  8. 읽은뒤 목록으로 돌아옴                    :%s\n",rbit(bcfg.modelist,1,"함","안함"));
        printf("  9. 읽은뒤 enter 입력 받음                    :%s\n",rbit(bcfg.modelist,2,"보통","무시"));
        printf(" 10. 내용시 줄수초과시 ENTER                   :%s\n",rbit(bcfg.modelist,3,"멈춤","무시"));
		printf(" 11. 하단명령어 선택위치 (X,Y좌표)             :(X:%2d Y:%2d) \n",bcfg.cmd_x,bcfg.cmd_y);
        printf(" 13. 게시물 광고판 모양1 출력                  :%s\n",rbit(bcfg.mode,16,"보통","광고"));
        printf(" 14. 게시물 광고판 모양2 출력                  :%s\n",rbit(bcfg.mode,17,"보통","광고"));
		printf(" 15. 게시물 보기시 왼쪽 1칸 공백               :%s\n",rbit(bcfg.mode,10,"보통","공백"));
		printf(" 16. 게시물 기본 화면출력시  게시판:자료수출력 :%s\n",rbit(bcfg.modelist,15,"보통","출력"));
		printf(" 17. 게시물 기본 화면출력시  게시판:자료수출력 :%s\n",rbit(bcfg.modelist,16,"보통","출력"));
		printf(" 18. 게시물 기본 화면출력시  텔넷  :주소  출력 :%s\n",rbit(bcfg.modelist,17,"무시","출력"));
		printf(" 19. 게시물 기본 화면출력시  외부  :경로  출력 :%s\n",rbit(bcfg.modelist,18,"무시","출력"));
		printf(" 20. 게시물 뉴스,속보 출력 기능                :%s\n",rbit(bcfg.mode,20,"허용","거부"));
        printf(" 21. 메뉴 하단의 명령어 선택 화면 (목록)       :%s\n",rbit(bcfg.mode,22,"보임","감춤"));
        printf(" 22. 메뉴 하단의 명령어 선택 화면 (읽기)       :%s\n",rbit(bcfg.mode,23,"보임","감춤"));
        printf(" 23. 추천자료 * 표시 무시                      :%s\n",rbit(bcfg.modelist,8,"적용","무시"));
        printf(" 24. 게시판 특수 모양 지정                     :%d\n",bcfg.bform);
		n=numselect();
		xy(50,n+1);
        if (n<= 0) break;
		if (n== 2) reset_search();
		if (n== 3) {bcfg.disp_x=get_num(bcfg.disp_x,2);bcfg.disp_y=get_num(bcfg.disp_y,2);}
		if (n== 4) bcfg.listline=get_num(bcfg.listline,2);
		if (n== 5) bcfg.modelist=gtt_bit(bcfg.modelist,0);
		if (n== 6) bcfg.mode=gtt_bit(bcfg.mode,24);
		if (n== 7) {bcfg.pos_y1=get_num(bcfg.pos_y1,2);bcfg.pos_y2=get_num(bcfg.pos_y2,2);}
		if (n== 8) bcfg.modelist=gtt_bit(bcfg.modelist,1);
		if (n== 9) bcfg.modelist=gtt_bit(bcfg.modelist,2);
		if (n==10) bcfg.modelist=gtt_bit(bcfg.modelist,3);
		if (n==11) {bcfg.cmd_x=get_num(bcfg.cmd_x,2);bcfg.cmd_y=get_num(bcfg.cmd_y,2);}
		if (n==13) bcfg.mode=gtt_bit(bcfg.mode,16);
		if (n==14) bcfg.mode=gtt_bit(bcfg.mode,17);
		if (n==15) bcfg.mode=gtt_bit(bcfg.mode,10);
		if (n==16) bcfg.modelist=gtt_bit(bcfg.modelist,15);
		if (n==17) bcfg.modelist=gtt_bit(bcfg.modelist,16);
		if (n==18) bcfg.modelist=gtt_bit(bcfg.modelist,17);
		if (n==19) bcfg.modelist=gtt_bit(bcfg.modelist,18);
		if (n==20) bcfg.mode=gtt_bit(bcfg.mode,20);
        if (n==21) bcfg.mode=gtt_bit(bcfg.mode,22);
        if (n==22) bcfg.mode=gtt_bit(bcfg.mode,23);
        if (n==23) bcfg.modelist=gtt_bit(bcfg.modelist,8);
        if (n==24) select_bform();

	}
}


//게시판담당용 순위보기
view_rank(int mode,char *_str1,char *_str2)
{
    FILE *fp,*fp2;
    char buff[500],buff1[500],buff2[500],tstr[100];
	char filename[500];
    char a_nid[500][10],a_ntty[500][10],a_nhost[500][30];
    char a_ncode[500][10],a_nclub[500][10];
    char st[30][100];
    int c_nid,c_ntty,c_nhost,c_ncode,c_nclub,re_aload=Yes;
    int i,n,n1,n2,nn,count,pr1;
	int it1,it2,it3;
	int year,month,day1,day2,job1,job2,hour1,hour2,type1,type2,hostno1,hostno2;
	char tid[10],tcode[10],tclub[10],thost[30],ttty[30];
    char tfile[30],tid2[10],tip[30];
    time_t date1,date2;
    int tot[20];
	pr1=10;
	it1=it2=it3=0;
    c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;
    date1=0;
    time(&date2);
    year=now_year()%100;
	month=now_month()%100;
	day1=day2=job1=job2=hour1=hour2=type1=type2=hostno1=hostno2=0;
    strcpy(tclub,_str1);
    strcpy(tcode,_str2);
    while (1) {
        del_space(thost);del_space(ttty);del_space(tclub);del_space(tcode);
        del_space(tfile);del_space(tid);del_space(tid2);del_space(tstr);
        a_num[4]=hostno1;a_num[5]=hostno2;
        a_num[1]=year;a_num[2]=month;
        a_num[6]=day1;a_num[7]=day2;
        a_num[8]=hour1;a_num[9]=hour2;
        a_num[10]=type1;a_num[11]=type2;
		strcpy(g_buff3,tclub);
		strcpy(g_buff4,tcode);
        a_num[12]=job1;a_num[13]=job2;
        a_num[14]=pr1;

        print_(0,"게시판 순위보기");
        printf("  1. 게시물 등록 시간 시작                     :%s\n",datestr(1,date1));
        printf("  2. 게시물 등록 시간 끝                       :%s\n",datestr(1,date2));
        printf("  9. 게시물 종합 순위\n");
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
        if (n==1) date1=dateinput0(date1);
        if (n==2) date2=dateinput0(date2);
        if (n==9) {
            for (i=0;i<20;i++) tot[i]=0;
            count=0;
            sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
            sprintf(NOW_FILED,"%s/menu/%s.tit",club_go.dir,go.code);
            if ((fp=fopen(NOW_FILE,"r"))==NULL) break;
            if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
            while (fread(&bbs,sizeof_bbs,1,fp)) {
                if (bbs.date<date1||bbs.date>date1) continue;
                if (bbs.del) tot[7]++;
                else {
                    tot[2]++;
                    tot[3]+=bbs.line/20;
                    tot[4]+=bbs.size;
                    tot[5]+=bbs.read;
                    tot[6]+=bbs.down;
                    tot[8]+=bbs.filesize;
                }
                del_enter(buff);
            }
            /*
            ┃전체   인원수 :     19 명    tot[1]
            ┃전체   글  수 :     56 개    tot[2]
            ┃전체   쪽  수 :     76 쪽    tot[3]
            ┃전체   용  량 :     76 KB    tot[4]
            ┃전체   조회수 :   6391 번    tot[5]
            ┃전체   다운수 :   6391 번    tot[6]
            ┃삭제된 글  수 :      1 개    tot[7]
            */
            fclose(fp);
            fclose(fp2);
            if (count<1) {
                mess("해당되는 접속기록이 없습니다.");
                unlink(TMP_FILE);
                continue;
            }
            sprintf(buff,"sort +4 -b -o%s %s",TMP_FILE2,TMP_FILE);
            system(buff);
            view_text(TMP_FILE2);

            unlink(TMP_FILE);
            unlink(TMP_FILE2);
		}
	}
}


/*
    else if (mode==10) view_log3(0,0) ;  //메뉴
    else if (mode==11) view_log3(5,0) ;  //쓰기
    else if (mode==12) view_log3(7,0) ;  //읽기
    else if (mode==13) view_log3(6,0) ;  //등록수
    else if (mode==13) view_log3(6,1) ;  //등록용량
    else if (mode==14) view_log3(8,0) ;  //다운
    else if (mode==15) view_log3(23,0);  //접속수
    else if (mode==16) view_log3(23,1);  //접속시간
*/

//순위보기
rank3(int mode,char *tclub,char *tcode)
{
    FILE *fp,*fp2,*fp3;
    char buff[500],buff1[500],buff2[500],tstr[100];
    char buff3[500],buff4[500],buff5[500],buff6[500],nowbuff[100];
	char filename[500];
    char st[30][100];
    int i,n,n1,n2,nn,count,year,month,day;

    if (!find_club(tclub)) return;
    set_nowmenu();       //데이타화일지정하기
    if (!find_go(tcode)) return;    //찾는 가기코드가 없다
    read_ccfg();
    set_bcfgfile();                     //BCFG_FILE에 저장
    read_bcfg();

	year=now_year()%100;
	month=now_month()%100;

    sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
    printf("\n   ... 잠깐만 기다려 주세요.. \n");
    printf("   [%s] 검색을 시작합니다... \n",filename);

    sprintf(tstr," %-8s %-8s ",club_go.code,go.code);
    sys_tem2("grep '%s' '%s'",tstr,filename,TMP_FILE);
//  sys_tem2("grep '%s' '%s' | sort +4 -b -o%s",tstr,filename,TMP_FILE);
    view_text(TMP_FILE);
    return;


         if (mode== 1) strcpy(tstr," 5 쓰기1");
    else if (mode== 2) strcpy(tstr," 7 읽기1");
    else if (mode== 3) strcpy(tstr," 6 등록1");
    else if (mode== 4) strcpy(tstr," 6 등록1");  //용량
    else if (mode== 5) strcpy(tstr," 8 다운1");
    else if (mode== 6) strcpy(tstr,"23 접속2");
    else if (mode== 7) strcpy(tstr,"23 접속2");  //시간
    else return;
    if (!is_file(filename)) {mess("%s 기록 화일이 없습니다.",filename);return;}

    sprintf(buff,"grep '%s' %s | sort +4 -b -o%s",tstr,filename,TMP_FILE);
    system(buff);
    if ((fp2=fopen(TMP_FILE,"r"))==NULL) return;
    if ((fp3=fopen(TMP_FILE2,"w"))==NULL) return;
    count=0;
    strcpy(nowbuff,"");
    n1=n2=0;
    while (fgets(buff,200,fp2)!=NULL) {
        if (strlen(buff)<40||buff[0]==';') continue;
        strncpy(buff2,&buff[30],3);  //type
        if (atoi(buff2)<1||atoi(buff2)>17) continue;
        nn++;
        count++;
        xstrncpy(buff2,&buff[21],8);  //ID
        del_space(buff2);
        if (!is_same(nowbuff,buff2)) {
            if (strlen(nowbuff)>2&&n1>0) fprintf(fp3,"%-8s%8d%8d\n",nowbuff,n1,n2);
            strcpy(nowbuff,buff2);
            n1=n2=0;
        }
        if (mode==23) {
            strcpy(buff3,&buff[95]);  //사용시간
            n2+=atoi(buff3);
        }
        if (mode==13||mode==14) {
            strcpy(buff3,&buff[102]);  //사용시간
            sscanf(buff3,"%s%s%s",buff4,buff5,buff6);
            n2+=atoi(buff5);
        }
        n1++;
    }
    if (strlen(nowbuff)>2&&n1>0) fprintf(fp3,"%-8s%8d%8d\n",nowbuff,n1,n2);
    fclose(fp2);
    fclose(fp3);
    if (count<1) {
        mess("해당되는 접속기록이 없습니다.");
        return;
    }
    sprintf(buff,"sort +1 -b -o%s %s",TMP_FILE,TMP_FILE2);
    system(buff);
    view_text(TMP_FILE2);
    view_text(TMP_FILE);

    unlink(TMP_FILE);
    unlink(TMP_FILE2);
}



/*

┃동호회    : main
┃가기코드  : hello
┃가기이름  : 가입인사
┃통계 기간 : ０６６０１∼０６６２９
┃통계일수  :  24 일
┃통계일    : 1999.07.03
┃전체   인원수 :     19 명    tot1
┃전체   글  수 :     56 개    tot2
┃전체   쪽  수 :     76 쪽    tot3
┃전체   용  량 :     76 KB    tot4
┃전체   조회수 :   6391 번    tot5
┃전체   다운수 :   6391 번    tot6
┃삭제된 글  수 :      1 개    tot7
┃일평균 글  수 :    2.3 개
┃일평균 쪽  수 :    2.3 쪽
┃일평균 용  량 :    2.3 개
┃글평균 조회수 :  114.1 번
┃글평균 다운수 :  114.1 번


┃Best10 조회
┃  9266 김성암   Kiev     06-01  199  1 [축하] 미리 축하, 풀뿌리 7주년.
┃  9267 조재춘   Value    06-01  198  1 [꾸벅] 반갑습니다..
┃  9268 풀뿌리   k2pbri1  06-02  186  1 [신규] 6월 2일자 신입회원 명단
┃Best10 다운
┃  9266 김성암   Kiev     06-01  199  1 12345678.123 [축하] 미리 축하, 풀뿌리 7주년.
┃  9267 조재춘   Value    06-01  198  1 12345678.123 [꾸벅] 반갑습니다..
┃  9268 풀뿌리   k2pbri1  06-02  186  1 12345678.123 [신규] 6월 2일자 신입회원 명단
┃
┃점수 〓 글수 *1  + 쪽(줄)수 *2  + 조회수 *2  + 활동일 *0  + 도배수 *0    │
┃
┃순위   점수
┃ 순위   아이디   이  름   글수   쪽수   용량    조회   다운   삭제  점 수
┃    1  STUDIO24  전종섭     16     23   1611M   1611   1611     16   3284
┃    2  k2pbri1   풀뿌리     13     13   1416M   1416   1416     13   2871
┃    3  파랭이    육소영      7      7    857M    857    857      7   1735
┃ 총계  ??????건           ????    ???   ????M   ????
┃
┃
┃순위   글수
┃ 순위   아이디   이  름   글수   쪽수   용량    조회   다운   삭제  점 수
┃    1  STUDIO24  전종섭     16     23   1611M   1611   1611     16   3284
┃    2  k2pbri1   풀뿌리     13     13   1416M   1416   1416     13   2871
┃    3  파랭이    육소영      7      7    857M    857    857      7   1735
┃
┃순위   쪽수
┃ 순위   아이디   이  름   글수   쪽수   용량    조회   다운   삭제  점 수
┃    1  STUDIO24  전종섭     16     23   1611M   1611   1611     16   3284
┃    2  k2pbri1   풀뿌리     13     13   1416M   1416   1416     13   2871
┃    3  파랭이    육소영      7      7    857M    857    857      7   1735
┃
┃순위   용량
┃ 순위   아이디   이  름   글수   쪽수   용량    조회   다운   삭제  점 수
┃    1  STUDIO24  전종섭     16     23   1611M   1611   1611     16   3284
┃    2  k2pbri1   풀뿌리     13     13   1416M   1416   1416     13   2871
┃    3  파랭이    육소영      7      7    857M    857    857      7   1735
┃
┃순위   조회
┃ 순위   아이디   이  름   글수   쪽수   용량    조회   다운   삭제  점 수
┃    1  STUDIO24  전종섭     16     23   1611M   1611   1611     16   3284
┃    2  k2pbri1   풀뿌리     13     13   1416M   1416   1416     13   2871
┃    3  파랭이    육소영      7      7    857M    857    857      7   1735
┃
┃순위   다운
┃ 순위   아이디   이  름   글수   쪽수   용량    조회   다운   삭제  점 수
┃    1  STUDIO24  전종섭     16     23   1611M   1611   1611     16   3284
┃    2  k2pbri1   풀뿌리     13     13   1416M   1416   1416     13   2871
┃    3  파랭이    육소영      7      7    857M    857    857      7   1735
┃
┃순위   삭제
┃ 순위   아이디   이  름   글수   쪽수   용량    조회   다운   삭제  점 수
┃    1  STUDIO24  전종섭     16     23   1611M   1611   1611     16   3284
┃    2  k2pbri1   풀뿌리     13     13   1416M   1416   1416     13   2871
┃    3  파랭이    육소영      7      7    857M    857    857      7   1735
┃
  일별글현황
  날짜    글수  쪽수  용량수
  1일 : 건수
┃일평균 글  수 :    2.3 개
┃일평균 쪽  수 :    2.3 쪽
┃일평균 용  량 :    2.3 개


07-01 00:16  77 p2   시삽     19 main     pdrv     21 게IP1 kslab                          STA 0
07-01 00:17  77 p2   시삽     19 main     pds      21 게IP1 kslab                          END 65
07-01 00:18  77 p3   guestp3   0 main     top      23 접속1 210.114.25.95                  STA 0
07-01 00:18  77 p3   guestp3   0 main     pgame    21 게IP1 210.114.25.95                  STA 0
07-01 00:19  77 p3   guestp3   0 main     pgame     7 읽기1 210.114.25.95                  B flame80  [GAM] 문명1
07-01 00:19  77 p3   guestp3   0 main     pgame     8 다운1 210.114.25.95                  P flame80  civ.arj 1457126 [GAM
07-01 00:19  77 p3   guestp3   0 main     pgame    21 게IP1 210.114.25.95                  END 70
07-01 00:19  77 p3   guestp3   0 main     pgame    23 접속2 210.114.25.95                  END 82

*/
