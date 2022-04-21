////////////////////////////////////////////////////////////////////////
//            XHOST 5.3s: 리눅스용 BBS HOST                            //
//			  개발자 : 김성대  xhost@xhost.co.kr (016-320-7882)		  //
//            이 소스를 변경하여 상용으로 사용하실때에는              //
//            개발자의 승인을 받아야 합니다.                          //
////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//검색 화일명 찾기
/////////////////////////////////////////////////////////////////////

#include "util.c"

char bufff[30];
char buffm[50];
char buffn[30];
char buffi[30];
char bufft[30];
char buffd[30];
char buffg1[30];
char buffg2[30];
char dir[100];

main(argc, argv)
int argc;
char *argv[];
{
    int find_mode;

    if (argc<3) {
        printf("\n사용법  findbbs <mode> <동호호코드>\n");
        printf("mode=0: 자료실 찾기\n");
        printf("mode=1: 게시판 찾기\n");
		getchar();
		return;
	}
    find_mode=atoi(argv[1]);
    find_club(argv[2]);
    club_mode=!is_same(club_go.code,"main");
	findport(tty); //접속포트 찾기
	read_cfg();
	set_tmpfile();		 //임시화일지정하기
	set_nowmenu();		 //데이타화일지정하기
    read_ccfg();
    strcpy(dir,club_go.dir);

    if (find_mode==1) find_pds();
    if (find_mode==2) find_bbs();
    unlink(TMP_FILE);
    unlink(TMP_FILE2);
}

find_pds()
{
	FILE *fp;
    char buff[140],buff2[80];
    int i,mode,num;
    mode=0;if (club_mode) mode=1;
    strcpy(bufff,"");
    strcpy(buffi,"");
    strcpy(buffm,"");
    strcpy(buffi,"");
    strcpy(bufft,"");
    strcpy(buffd,"");
    strcpy(buffg1,"");
    strcpy(buffg2,"");

    while (1) {
        cls();
        printfL2();
        printf(" 자료실의 화일을 찾습니다. (조건을 지정후 enter를 치면 됩니다.)  \n");
        printf(" FB:게시판검색 FF:화일검색          (검색동호회:%s)\n",club_go.code);
        printf(" 메뉴 파일 : %s \n",NOW_MENU);
        printfL2();
        if (mode==0) strcpy(buffm,"(메인 자료실)");
        else         sprintf(buffm,"(동호회자료실 %s)",club_go.dir);
        printf("      1.   검색 화 일 명 지정 (%-12s)\n",bufff);
        printf("      2.   검색 아 이 디 지정 (%-12s)\n",buffi);
        printf("      3.   검색 성    명 지정 (%-12s)\n",buffn);
        printf("      4.   검색 제목문자 지정 (%-12s)\n",bufft);
        printf("      5.   검색 날짜이후 지정 (%-12s)\n",buffd);
        printf("      6.   메인 / 동호회 지정 %-30s\n",buffm);
        printf("      7.   가기코드 포함 지정 (%-12s)\n",buffg1);
        printf("      8.   가기코드 제목 지정 (%-12s)\n",buffg2);
        printf("      9.   자료 찾기 도움말\n");
        printf("  enter:   지정 조건으로 검색 시작 (위의 조건에 해당되는 자료)\n");
        printfL2();
        printf(" 번호를 선택하세요 (q:취소) >>");
        lineinput(buff,5);
        printf("\n");
        num=atoi(buff);
        i=0;
        if (is_char("QqPpXx0",buff[0])) return;
        if (num==1) {
            printf(" 검색할 화일명중 일부 입력 (예:abc:abc가 포함된 화일명) >>");
            lineinput(bufff,13);
            del_space(bufff);
            del_esc(bufff);
        }
        if (num==2) {
            printf(" 검색할 아이디중 일부 입력 >>");
            lineinput(buffi,12);
            del_space(buffi);
            del_esc(buffi);
        }
        if (num==3) {
            printf(" 검색할 성명중 일부 입력 >>");
            lineinput(buffn,12);
            del_space(buffn);
            del_esc(buffn);
        }
        if (num==4) {
            printf(" 검색할 제목중 일부 입력 >>");
            lineinput(bufft,12);
            del_space(bufft);
            del_esc(bufft);
        }
        if (num==5) {
            printf(" 검색할 날짜 (예:96.12.31->96.12.31이후 자료) >>");
            lineinput(buffd,12);
            del_space(buffd);
            del_esc(buffd);
        }
        if (num==6) {
            if (mode==0) mode=1;
            else         mode=0;
        }
        if (num==7) {
            printf(" 검색할 코드중 가기코드에  포함문자 >>");
            lineinput(buffg1,8);del_space(buffg1);del_esc(buffg1);
        }
        if (num==8) {
            printf(" 검색할 코드중 가기 제목에  포함문자 >>");
            lineinput(buffg2,8);del_space(buffg2);del_esc(buffg2);
        }
        if (num==9) {
            logo("logo/_findpds.log",Yes);
        }
        if (num==0&&buff[0]==0) {
            if (bufff[0]==0&&
                buffi[0]==0&&
                buffn[0]==0&&
                bufft[0]==0&&
                buffd[0]==0&&
                yesno("\n 전체 자료를 다 찾으시겠습니까? (y/N) >>",No)==No) continue;
            printf("\n");
            search_file(mode);
        }
    }
}


search_file(int mode)
{
    char afn[1000][9];
    char fn[100];
    char buffsize[50];
    char bufffname[50];
    int found;
    int i,j;
    int afn_num=0;
    FILE *fp;
    FILE *fp2;
	struct tm *tp;
    char buff[100];
    GO tgo;

    for (i=0;i<1000;i++) strcpy(afn[i],"");
    if ((fp=fopen(NOW_MENU,"r"))==NULL) return;

    while(fread(&tgo,sizeof_go,1,fp)) {
        if (strlen(buffg1)>0&&!stristr(tgo.code,buffg1)) continue;
        if (strlen(buffg2)>0&&!stristr(tgo.name,buffg2)) continue;
        if (strlen(tgo.code)>0&&is_char(PDSTYPE,tgo.type)
            &&!is_same(tgo.code,"bbs_all")
            &&!is_same(tgo.code,"pds_all")
            &&!is_char("zdc",tgo.type)) {
            found=No;
            for (j=0;j<afn_num;j++) {
                if (is_same(afn[j],tgo.code)) {
                    found=Yes;
                    break;
                }
            }
            if (!found&&afn_num<999) strcpy(afn[afn_num++],tgo.code);
        }
    }
    findport(tty);      //현재 사용되고 있는 포트찾기
    fp2=fopen(TMP_FILE,"w");
    fprintf(fp2,"가기코드 번호 화일명    화일크기 ID       제  목\n");
    for (i=0;i<afn_num;i++) {
        if (mode==0) sprintf(fn,"menu/%s.tit",afn[i]);
        else         sprintf(fn,"%s/menu/%s.tit",club_go.dir,afn[i]);
        fp=fopen(fn,"r");
        if (fp==NULL) continue;
        while (fread(&bbs,sizeof_bbs,1,fp)) {
            if ( !bbs.del&&
                 (bufff[0]==0||stristr(bbs.filename,bufff))&&
                 (buffi[0]==0||stristr(bbs.id,buffi))&&
                 (buffn[0]==0||stristr(bbs.name,buffn))&&
                 (bufft[0]==0||stristr(bbs.title,bufft)) ) {
                strcpy(buffsize,byte2str(bbs.filesize));
                file2dot(bufffname,bbs.filename);
                tp = localtime(&bbs.date);
                fprintf(fp2,"(%2d/%2d/%2d) %-8s%5d %s%6s %-8s %-25.25s\n",
                    tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,afn[i],bbs.num,bufffname,buffsize,bbs.id,bbs.title);
            }
        }
        fclose(fp);
    }
    fclose(fp2);
    xsort(1,TMP_FILE,TMP_FILE2);
    view_text(TMP_FILE2);
    unlink(TMP_FILE);
    unlink(TMP_FILE2);
}



find_bbs()
{
	FILE *fp;
    char buff[140],buff2[80];
    int i,mode,num;
    mode=0;if (club_mode) mode=1;

    strcpy(buffi,"");
    strcpy(buffm,"");
    strcpy(buffi,"");
    strcpy(bufft,"");
    strcpy(buffd,"");
    strcpy(buffg1,"");
    strcpy(buffg2,"");

    while (1) {
        cls();
        printfL2();
        printf("(findbbs) 게시판의 항목을 찾습니다. (조건을 지정후 enter를 치면 됩니다.)  \n");
        printf(" FB:게시판검색 FF:화일검색          (검색동호회:%s)\n",club_go.code);
        printfL2();
        if (mode==0) strcpy(buffm,"(메인 자료실)");
        else         sprintf(buffm,"(동호회자료실 %s)",club_go.dir);
        printf("      1.   검색 아 이 디 지정 (%-12s)\n",buffi);
        printf("      2.   검색 성    명 지정 (%-12s)\n",buffn);
        printf("      3.   검색 제목문자 지정 (%-12s)\n",bufft);
        printf("      4.   검색 날짜이후 지정 (%-12s)\n",buffd);
        printf("      6.   메인 / 동호회 지정 %-30s\n",buffm);
        printf("      7.   가기코드 포함 지정 (%-12s)\n",buffg1);
        printf("      8.   가기코드 제목 지정 (%-12s)\n",buffg2);
        printf("  enter:   지정 조건으로 검색 시작 (위의 조건에 해당되는 자료)\n");
        printfL2();
        printf(" 번호를 선택하세요 (q:취소) >>");
        lineinput(buff,5);
        printf("\n");
        num=atoi(buff);
        i=0;
        if (is_char("QqPpXx0",buff[0])) return;
        if (num==1) {
            printf(" 검색할 아이디중 일부 입력 >>");
            lineinput(buffi,12);
            del_space(buffi);
            del_esc(buffi);
        }
        if (num==2) {
            printf(" 검색할 성명중 일부 입력 >>");
            lineinput(buffn,12);
            del_space(buffn);
            del_esc(buffn);
        }
        if (num==3) {
            printf(" 검색할 제목중 일부 입력 >>");
            lineinput(bufft,12);
            del_space(bufft);
            del_esc(bufft);
        }
        if (num==4) {
            printf(" 검색할 날짜 (예:96.12.31->96.12.31이후 자료) >>");
            lineinput(buffd,12);
            del_space(buffd);
            del_esc(buffd);
        }
        if (num==6) {
            if (mode==0) mode=1;
            else         mode=0;
        }
        if (num==7) {
            printf(" 검색할 코드중 가기코드에  포함문자 >>");
            lineinput(buffg1,8);del_space(buffg1);del_esc(buffg1);
        }
        if (num==8) {
            printf(" 검색할 코드중 가기 제목에  포함문자 >>");
            lineinput(buffg2,8);del_space(buffg2);del_esc(buffg2);
        }
        if (num==0&&buff[0]==0) {
            if (buffi[0]==0&&
                buffn[0]==0&&
                bufft[0]==0&&
                buffd[0]==0&&
                yesno("\n 전체 자료를 다 찾으시겠습니까? (y/N) >>",No)==No) continue;
            printf("\n");
            search_bbs(mode);
        }
    }
}


search_bbs(int mode)
{
    char afn[1000][9];
    char fn[100];
    char bufffname[50];
    int found;
    int i,j;
    int afn_num=0;
    FILE *fp;
    FILE *fp2;
	struct tm *tp;
    char buff[100];
    GO tgo;

    for (i=0;i<1000;i++) strcpy(afn[i],"");
    if ((fp=fopen(NOW_MENU,"r"))==NULL) return;

    while(fread(&tgo,sizeof_go,1,fp)) {
        if (strlen(buffg1)>0&&!stristr(tgo.code,buffg1)) continue;
        if (strlen(buffg2)>0&&!stristr(tgo.name,buffg2)) continue;
        if (strlen(tgo.code)>0&&is_char(BBSTYPE,tgo.type)
            &&!is_same(tgo.code,"bbs_all")
            &&!is_same(tgo.code,"pds_all")
            &&!is_char("zdc",tgo.type)) {
            found=No;
            for (j=0;j<afn_num;j++) {
                if (is_same(afn[j],tgo.code)) {
                    found=Yes;
                    break;
                }
            }
            if (!found&&afn_num<999) strcpy(afn[afn_num++],tgo.code);
        }
    }
    findport(tty);      //현재 사용되고 있는 포트찾기
    sprintf(TMP_FILE,"tmp/tmp1%d%s",cfg.hostno,no_spc(tty)); // 임시화일
    sprintf(TMP_FILE2,"tmp/tmp2%d%s",cfg.hostno,no_spc(tty)); // 임시화일
    fp2=fopen(TMP_FILE,"w");
    fprintf(fp2,"날짜  가기코드 번호 ID  이름    제  목\n");
    for (i=0;i<afn_num;i++) {
        if (mode==0) sprintf(fn,"menu/%s.tit",afn[i]);
        else         sprintf(fn,"%s/menu/%s.tit",club_go.dir,afn[i]);
        fp=fopen(fn,"r+");
        if (fp!=NULL) {
            while (fread(&bbs,sizeof_bbs,1,fp)) {
                if ( !bbs.del&&
                     (buffi[0]==0||stristr(bbs.id,buffi))&&
                     (buffn[0]==0||stristr(bbs.name,buffn))&&
                     (bufft[0]==0||stristr(bbs.title,bufft)) ) {
                    tp = localtime(&bbs.date);
                    fprintf(fp2,"(%2d/%2d/%2d) %-8s%5d %-8s %-40.40s\n",
                        tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,afn[i],bbs.num,bbs.id,bbs.title);
                }
            }
            fclose(fp);
        }
    }
    fclose(fp2);

    xsort(1,TMP_FILE,TMP_FILE2);
    view_text(TMP_FILE2);
}

file2dot(char *buff,const char *filename)
{
	unsigned int i=0;
	int n=0;
	char buff2[50];
	strcpy(buff2,filename);
	del_space(buff2);
	n=strlen(buff2);
		 if (buff2[0]==0	) sprintf(buff,"            ");
	else if (buff2[n-1]=='.') sprintf(buff,"%9s   ",buff2);
	else if (buff2[n-2]=='.') sprintf(buff,"%10s  ",buff2);
	else if (buff2[n-3]=='.') sprintf(buff,"%11s ",buff2);
	else					  sprintf(buff,"%12.12s",buff2);
	return i;
}




