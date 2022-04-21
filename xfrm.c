////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 화면 통합관리
///////////////////////////////////////////////////////////////////////////
#include <dirent.h>
#include "bbs.h"

typedef struct {
    char mode;             //있는가?
    char filename[13];
    char title[41];
    int  filesize;
    time_t date;
} XFRM;
XFRM xfrm;
#define sizeof_xfrm (sizeof(XFRM))
#define MAX 3000
XFRM afrm[MAX+1];


char fdir[300];
char ndir[300];
char ntit[300];
char adir[30][20];
char atit[30][40];
int total;
int total_size;
int re_load;
char datafile[500];

main(argc, argv)
int argc;
char *argv[];
{
    int  i,n,nn;
    char buff[300];
    char tcode[9];
    char ccode[9];
    char now_id[9];
    char filename[300];
    int  mode;

    club_mode=No;
	findport(tty); //접속포트 찾기
	read_cfg();
	set_tmpfile();		 //임시화일지정하기
    set_nowmenu();       //데이타화일지정하기
    if (is_same(argv[1],"datestr")) {
        view_alldatestr();
        return;
    }
    mode=atoi(argv[1]);                 //동호회 모드
    strcpy(ccode,argv[2]);              //동호회 코드
    strcpy(tcode,argv[3]);              //동호회 코드
    strcpy(now_id,argv[4]);             //아이디

    find_club(ccode);
	set_nowmenu();		 //데이타화일지정하기
    find_go(tcode);
    read_ccfg();
    strcpy(fdir,club_go.dir);
	if (is_same(now_id,"root")) {
        who=5;
        strcpy(now_id,cfg.sysop);
        read_pf(now_id);    //아이디 읽기
	}
    else if (is_same(cfg.sysop,now_id)) {
        read_pf(now_id);    //아이디 읽기
        who=4;
    }
    else if(read_pf(now_id)) {    //아이디 읽기
		who=0;
		if (pf.type==19) who=4;  //root
		if (pf.type==18) who=3;  //부시삽
		if (pf.type==17) who=2;  //운영진
	}
    check_dir("%s/frm/bbs/",club_go.dir);
    check_dir("%s/frm/end/",club_go.dir);
    check_dir("%s/frm/error/",club_go.dir);
    check_dir("%s/frm/guest/",club_go.dir);
    check_dir("%s/frm/level/",club_go.dir);
    check_dir("%s/frm/log/",club_go.dir);
    check_dir("%s/frm/notice/",club_go.dir);
    check_dir("%s/frm/pf/",club_go.dir);
    check_dir("%s/frm/ad/",club_go.dir);
    check_dir("%s/frm/screen/",club_go.dir);
    if (mode==1) job_scr();
    if (mode==2) job_ad();
}


job_scr()
{
    FILE *fp;
    int  i,n,nn;
    char buff[300];
    char buff2[300];
    char abuff[30][100];
    char filename[300];
    int  change=No;
    for (i=0;i<30;i++) bzero(abuff[i],80);
    sprintf(filename,"system/xfrm%s.dat",club_mode?"club":"main");
    if ((fp=fopen(filename,"r"))!=NULL) {
        i=0;
        while(i<30&&fgets(buff,100,fp)) {
            del_enter(buff);all_trim(buff);
            if (strlen(buff)>0) {
                xstrncpy(adir[i],buff,16);
                strcpy(atit[i],&buff[16]);
                del_space(adir[i]);all_trim(adir[i]);all_trim(atit[i]);
                i++;
            }
        }
        fclose(fp);
        if (i<1) goto _cont1;
    }
    else {
        _cont1:
        change=Yes;
        if (!club_mode) {
            i=0;
            strcpy(abuff[i++],"frm/bbs/        게시판편집      ");
            strcpy(abuff[i++],"frm/bio/        바이오리듬      ");
            strcpy(abuff[i++],"frm/chat/       대화방          ");
            strcpy(abuff[i++],"frm/down/       다운로드        ");
            strcpy(abuff[i++],"frm/editor/     편집기,쓰기     ");
            strcpy(abuff[i++],"frm/end/        종료            ");
            strcpy(abuff[i++],"frm/error/      오류            ");
            strcpy(abuff[i++],"frm/guest/      손님가입        ");
            strcpy(abuff[i++],"frm/help/       도움말          ");
            strcpy(abuff[i++],"frm/level/      등급            ");
            strcpy(abuff[i++],"frm/log/        접속시작        ");
            strcpy(abuff[i++],"frm/mail/       메일            ");
            strcpy(abuff[i++],"frm/notice/     공지            ");
            strcpy(abuff[i++],"frm/pf/         신상            ");
            strcpy(abuff[i++],"frm/rec/        기록            ");
            strcpy(abuff[i++],"frm/say/        메세지          ");
            strcpy(abuff[i++],"frm/screen/     게시판/메뉴화면 ");
            strcpy(abuff[i++],"frm/system/     시스템          ");
            strcpy(abuff[i++],"frm/telnet/     텔넷            ");
            strcpy(abuff[i++],"frm/typegame/   타자대화방      ");
            strcpy(abuff[i++],"frm/us/         접속자          ");
            strcpy(abuff[i++],"frm/xfrm/       화면편집데이타  ");
            strcpy(abuff[i++],"logo/           메인메뉴        ");
        }
        else {      //동호회일때
            strcpy(abuff[i++],"frm/bbs/        게시판편집      ");
            strcpy(abuff[i++],"frm/end/        종료            ");
            strcpy(abuff[i++],"frm/error/      오류            ");
            strcpy(abuff[i++],"frm/guest/      손님가입        ");
            strcpy(abuff[i++],"frm/level/      등급            ");
            strcpy(abuff[i++],"frm/log/        접속시작        ");
            strcpy(abuff[i++],"frm/notice/     공지            ");
            strcpy(abuff[i++],"frm/pf/         신상            ");
            strcpy(abuff[i++],"frm/screen/     게시판/메뉴화면 ");
            strcpy(abuff[i++],"logo/           메인메뉴        ");
        }
        for (i=0;i<30;i++) {
            if (strlen(abuff[i])>1) {
                xstrncpy(adir[i],abuff[i],16);
                strcpy(atit[i],&abuff[i][16]);
                del_space(adir[i]);all_trim(adir[i]);all_trim(atit[i]);
            }
        }
    }

    //데이타 파일읽어오기 -> 파일크기.최종날짜확인
    while (1) {
        cls();
        view("frm/xfrm/menu.mnu");
        printf(C_SAVE);
        for (i=0;i<30;i++) {
            del_space(adir[i]);
            if (strlen(adir[i])>1) {
                xy(i<=14?1:40,i%15+4);
                printf("%3d. %-16s %s\n",i+1,adir[i],atit[i]);
            }
        }
        printf(C_UNDO);
        lineinput(buff,20);check_han(buff);check_cmd(buff);printf("\n");
        n=mmn[0];
        if (strlen(buff)==1&&is_char("qQpPxX0",buff[0])) break;
        else if (is_same(mmc[0],"e")) {edit_abuff();change=Yes;}
        else if (is_same(mmc[0],"d")) {del_abuff() ;change=Yes;}
        else if (is_same(mmc[0],"m")) {move_abuff();change=Yes;}
        else if (n>=1&&n<=30&&strlen(adir[n-1])>1) {
            strcpy(ndir,adir[n-1]);
            strcpy(ntit,atit[n-1]);
            sprintf(buff,"*%s",ndir);
            strchng2(buff,"*frm/","");
            strchng2(buff,"/","");
            strchng2(buff,"*","");
            sprintf(datafile,"frm/xfrm/%s.dat",buff);
            touch_file(datafile);
            browse_data();
        }

    }
    if (change&&(fp=fopen(filename,"w"))!=NULL) {
        for (i=0;i<30;i++) {
            if (strlen(adir[i])>0) {
                fprintf(fp,"%-16s%s\n",adir[i],atit[i]);
            }
        }
        fclose(fp);
    }
}


job_ad()
{
	FILE *fp;
    int n,n2,i;
	char buff[240];
    char filename[340];
    sprintf(buff,"%s/frm/ad/",club_go.dir);     check_dir(buff);
    sprintf(buff,"%s/frm/bbs/",club_go.dir);    check_dir(buff);
    sprintf(buff,"%s/frm/down/",club_go.dir);   check_dir(buff);
    sprintf(buff,"%s/frm/editor/",club_go.dir); check_dir(buff);
    sprintf(buff,"%s/frm/end/",club_go.dir);    check_dir(buff);
    sprintf(buff,"%s/frm/help/",club_go.dir);   check_dir(buff);
    sprintf(buff,"%s/frm/log/",club_go.dir);    check_dir(buff);
    sprintf(buff,"%s/frm/notice/",club_go.dir); check_dir(buff);
    sprintf(buff,"%s/frm/pf/",club_go.dir);     check_dir(buff);
    sprintf(buff,"%s/frm/screen/",club_go.dir); check_dir(buff);
    sprintf(buff,"%s/frm/system/",club_go.dir); check_dir(buff);
    sprintf(buff,"%s/frm/xfrm/",club_go.dir);   check_dir(buff);

	cls();
    while (1) {
        cls();
        print_(1,"%s 광고 관련 작업",club_go.code);
        printf("  1. 공지사항 배경화면                 frm/notice/notice.scr\n");
        printf("  2. 공지사항 광고 기본                frm/notice/notice.dat\n");
        printf("  3. 공지사항 광고 난수                frm/notice/notice.da?\n");
        printf(" 11. 진입시 광고 기본                  frm/log/start.log\n");
        printf(" 12. 진입시 광고 연속                  frm/log/start?.log\n");
        printf(" 13. 진입시 광고 난수                  frm/log/startr?.log\n");
        printf(" 14. 진입시 광고 월일                  frm/log/dayMMDD.log\n");
        printf(" 15. 진입시 광고 일자                  frm/log/day?.log\n");
        printf(" 16. 진입시 광고 요일                  frm/log/week?.log\n");
        printf(" 17. 진입시 광고 시간                  frm/log/hour?.log\n");
        printf(" 21. 전체 하단 한줄광고 일반회원       frm/ad/ad.dat\n");
        printf(" 22. 전체 하단 한줄광고 손님           frm/guest/guestad.dat\n");
        printf(" 23. 전체 시삽 메세지                  system/sysmess.txt\n");
        printf(" 24. %-8s 메뉴 하단 광고               logo/%s.ad?\n",go.code,go.code);
        printf(" 25. %-8s 메뉴 자동 안시               logo/%s.ta?\n",go.code,go.code);
        printf(" 31. 메인 아이디 입력 광고 기본        frm/log/startid.add\n");
        printf(" 32. 메인 아이디 입력 광고 난수        frm/log/startid.ad?\n");
        printf(" 33. 메인 아이디 입력 광고 월일        frm/log/dayMMDD.add\n");
        printf(" 34. 메인 아이디 입력 광고 일자        frm/log/day?.add\n");
        printf(" 35. 메인 아이디 입력 광고 요일        frm/log/week?.add\n");
        printf(" 36. 메인 아이디 입력 광고 시간        frm/log/hour?.add\n");
        printf(" 삭제(D)에디터(E)전송(UP)다운(DN)보기(번호)설명(H)복귀(P) >>");
        lineinput(buff,20);printf("\n");check_han(buff);check_cmd(buff);
        if (is_quit(buff)||buff[0]==0) break;
        n=atoi(mmc[1]);n2=0;
        if (atoi(mmc[0])>0) n=atoi(mmc[0]);
        if (atoi(mmc[2])>0) n2=atoi(mmc[2]);
        if (club_mode&&n>=31) continue;
        else if (n==0) {
            mess(" 사용법 <명령> <번호>  예: E 11  ,D 33");
            continue;
        }
        else if (is_same(mmc[0],"h")) {
            view_text("frm/help/logo.hlp");
			cls();
            continue;
		}
        else if (is_char("pqx0",mmc[0][0])) break;

        if (n2>0) ;
        else if (n==3||n==12||n==13||n==32||n==24||n==25) {
            printf("  화면 번호 (0-9) >>");
            lineinput(buff,2);n2=atoi(buff);printf("\n");
		}
        else if (n==14||n==33) {
            printf("  월일 선택 (예 12월31일 -> 1231) >>");
            lineinput(buff,4);n2=atoi(buff);printf("\n");
        }
        else if (n==15||n==34) {
            printf("  일자 선택 (예 31일 -> 31) >>");
            lineinput(buff,2);n2=atoi(buff);printf("\n");
        }
        else if (n==16||n==35) {
            printf("  요일 선택 (일:0 월:1 화:2 수:3 목:4 금:5 토:6)  (0~6) >>");
            lineinput(buff,2);n2=atoi(buff);printf("\n");
        }
        else if (n==17||n==36) {
            printf("  시간 선택 (0~23) >>");
            lineinput(buff,2);n2=atoi(buff);printf("\n");
        }
        strcpy(filename,"");
        if      (n== 1) sprintf(filename,"%s/frm/notice/notice.scr",club_go.dir);
        else if (n== 2) sprintf(filename,"%s/frm/notice/notice.dat",club_go.dir);
        else if (n== 3) sprintf(filename,"%s/frm/notice/notice.da%d",club_go.dir,n2);
        else if (n==11) sprintf(filename,"%s/frm/log/start.log",club_go.dir);
        else if (n==12) sprintf(filename,"%s/frm/log/start?.log",club_go.dir,n2);
        else if (n==13) sprintf(filename,"%s/frm/log/startr?.log",club_go.dir,n2);
        else if (n==14) sprintf(filename,"%s/frm/log/day%04d.log",club_go.dir,n2);
        else if (n==15) sprintf(filename,"%s/frm/log/day%d.log",club_go.dir,n2);
        else if (n==16) sprintf(filename,"%s/frm/log/week%d.log",club_go.dir,n2);
        else if (n==17) sprintf(filename,"%s/frm/log/hour%d.log",club_go.dir,n2);
        else if (n==21) sprintf(filename,"%s/frm/ad/ad.dat",club_go.dir);
        else if (n==22) sprintf(filename,"%s/frm/guest/guestad.dat",club_go.dir);
        else if (n==23) sprintf(filename,"%s/system/sysmess.txt",club_go.dir);
        else if (n==24) sprintf(filename,"%s/logo/%s.ad%d",club_go.dir,go.code,n2);
        else if (n==25) sprintf(filename,"%s/logo/%s.ta%d",club_go.dir,go.code,n2);
        else if (n==31) sprintf(filename,"%s/frm/log/startid.add",club_go.dir);
        else if (n==32) sprintf(filename,"%s/frm/log/startid.ad%d",club_go.dir,n2);
        else if (n==33) sprintf(filename,"%s/frm/log/day%04d.add",club_go.dir,n2);
        else if (n==34) sprintf(filename,"%s/frm/log/day%d.add",club_go.dir,n2);
        else if (n==35) sprintf(filename,"%s/frm/log/week%d.add",club_go.dir,n2);
        else if (n==36) sprintf(filename,"%s/frm/log/hour%d.add",club_go.dir,n2);
        else continue;

        if (is_same(mmc[0],"d")&&!is_file(filename)) {
            mess("%s 화일이 없습니다.",filename);
        }
        else if (is_same(mmc[0],"d")) {
            view(filename);
            printf("\n %s 화일을 삭제 할까요? 삭제(Y) >>",filename);
            if (yesno("",No)) {
                if (is_file(filename)) {
                    sprintf(buff,"cp %s %s/logo/zdelete/",filename,club_go.dir);
                    sys_tem(buff);
                    sprintf(buff,"rm %s",filename);
                    sys_tem(buff);
                }
            }
		}
        else if (is_same(mmc[0],"up")) {
            printf("\n ## %s 화일을 등록합니다.\n",filename);
            up_text(0,filename);   //화일을 전송받아 filename에 저장한다.
		}
        else if (is_same(mmc[0],"e")) xedit(filename);
		else if (is_same(mmc[0],"dn")) {
            sz_file(filename);
		}
        else if (view(filename)) pressenter();
	}
}




edit_abuff()
{
    int n=mmn[1]-1;
    if (n<0||n>=30) {mess(" 항목 편집 사용법:  e [편집할번호] ");return;}
    printf("\n  %d. 디렉토리 %-20s >>",n+1,adir[n]);
    get_str(adir[n],16);
    printf("\n  %d. 설명     %-20s >>",n+1,atit[n]);
    get_str(atit[n],20);
}

del_abuff()
{
    int n=mmn[1]-1;
    if (n<0||n>=30) {mess(" 항목 삭제 사용법:  d [편집할번호] ");return;}
    printf("\n  %d. %-16s  %s ",n+1,adir[n],atit[n]);
    if (yesno("  이 항목을 삭제할까요?(y/N) >>",No)) {
        strcpy(adir[n],"");
        strcpy(atit[n],"");
    }
}

move_abuff()
{
    int n1=mmn[1]-1;
    int n2=mmn[2]-1;
    int i;
    char str_adir[100];
    char str_atit[100];
    if (n1<0||n1>=30||n2<0||n2>30) {mess(" 항목 이동 사용법:  m [이동할번호]  [이동될위치]");return;}
    strcpy(str_adir,adir[n1]);
    strcpy(str_atit,atit[n1]);
	if (n1<n2) {		//n1->n2
		for (i=n1;i<n2;i++) {
            strcpy(adir[i],adir[i+1]);
            strcpy(atit[i],atit[i+1]);
		}
        strcpy(adir[n2],str_adir);
        strcpy(atit[n2],str_atit);
	}
	else {		  //n1->n2
		for (i=n1;i>n2;i--) {
            strcpy(adir[i],adir[i-1]);
            strcpy(atit[i],atit[i-1]);
		}
        strcpy(adir[n2+1],str_adir);
        strcpy(atit[n2+1],str_atit);
	}
}



browse_data()
{
    int top,linenum,i,n,nn;
    char str[300];
    char buff[300];
    char buff2[300];

	top=0;
    total=0;
    re_load=Yes;
	while (1) {
        if (re_load) {re_loadfrm();re_load=No;}
        strcpy(g_buff1,ndir);
        strcpy(g_buff2,ntit);
        sprintf(g_buff3,"%d",total);
        sprintf(g_buff4,"%d",total_size);
        view("frm/xfrm/xfrm.scr");
        if (top>=total) top=total-15;
		if (top<0) top=0;
        linenum=0;
        i=top;
        printf(C_SAVE);
        while(linenum<15&&i<total) {
            xfrm=afrm[i];
            if (strlen(xfrm.filename)>0) {
                xy(1,linenum+5);
                sprintf(buff,"%6d",xfrm.filesize);
                if (xfrm.mode==0) sprintf(buff,"  없음");
                printf("%3d %-12s%s %s %s\n",
                i+1,xfrm.filename,buff,datestr(3,xfrm.date),xfrm.title);
            }
            linenum++;
            i++;
		}
        printf(C_UNDO);
		lineinput(buff,20);check_han(buff);check_cmd(buff);printf("\n");
		nn=mmn[1];
        if      (is_same(mmc[0],"e"))                      edit_frm(nn);
        else if (is_same(mmc[0],"ed"))                     edit_frm(nn);
        else if (is_same(mmc[0],"up"))                     upload_frm(nn);
        else if (is_same(mmc[0],"dn"))                     dnload_frm(nn);
        else if (is_same(mmc[0],"d"))                      delete_frm(nn);
        else if (club_mode&&is_same(mmc[0],"g"))           get_main(nn);
        else if (pf.type>=18&&is_same(mmc[0],"td"))        delete_data(nn);
        else if (pf.type>=18&&is_same(mmc[0],"ta"))        add_data();
        else if (pf.type>=18&&is_same(mmc[0],"te"))        edit_data(nn);
        else if (pf.type>=18&&is_same(mmc[0],"tf"))        sort_data(1);
        else if (pf.type>=18&&is_same(mmc[0],"tn"))        sort_data(2);
        else if (pf.type>=18&&is_same(mmc[0],"ts"))        sort_data(3);
        else if (is_same(mmc[0],"ls"))                     {if (mmn[1]>0) top=mmn[1]-1;else top=0;}
		else if (is_same(mmc[0],"x"))                      break;
		else if (is_same(mmc[0],"p")||is_same(mmc[0],"q")) break;
        else if (is_same(mmc[0],"xx")||is_same(mmc[0],"qq")) break;
		else if (is_same(mmc[0],"t"))                      top=0;
        else if (is_same(mmc[0],"b"))                      top-=15;
        else if (is_same(mmc[0],"f")||strlen(buff)<1)      top+=15;
        else if (mmn[0]>0)                                 disp_frm(mmn[0]);
	}
}




edit_frm(int nn)
{
    char filename[300];
    char filename2[300];
    char buff[300];
    int n,key=No;
    if (nn<1||nn>total) return;
    xfrm=afrm[nn-1];
    set_filename(filename);
    key=xedit(filename);
    re_load=Yes;
}


get_main(int nn)
{
    char filename[300];
    char filename2[300];
    char buff[300];
    int n,key=No;
    if (nn<1||nn>total) return;
    xfrm=afrm[nn-1];
    sprintf(filename2,"%s/%s",ndir,xfrm.filename);
    strchng2(filename2,"//","/");
    set_filename(filename);
    if (!is_file(filename2)) {
        printf("## %s파일이 없습니다.\n",filename2);
    }
    else {
        printf(" ## %s(%d Byte)\n",filename2,file_size(filename2));
        printf(" -> %s(%d Byte)\n",filename,file_size(filename));
        if (yesno("위 사항대로 복사할까요?(Y/n)>>",Yes)) {
            file_copy(filename2,filename);
            re_load=Yes;
        }
    }
}


upload_frm(int nn)
{
    char filename[300];
    char filename2[300];
    char buff[300];
    int n,key=No;
    if (nn<1) return;
    if (total>0&&nn>total) return;
    xfrm=afrm[nn-1];
    set_filename(filename);
    key=up_file(0,filename2,No);   //화일을 전송받아 filename에 저장한다.
    if (key&&is_file(filename2)) {
        file_copy(filename2,filename);
        rm_tmpup();
    }
    re_load=Yes;
}


dnload_frm(int nn)
{
    char filename[300];
    char filename2[300];
    char buff[300];
    int n,key=No;
    if (nn<1||nn>total) return;
    xfrm=afrm[nn-1];
    set_filename(filename);
    disp_info(nn);
    sz_file(filename);
    rm_tmpup();
}


delete_frm(int nn)
{
    char filename[300];
    if (nn<1||nn>MAX) return No;
    xfrm=afrm[nn-1];
    set_filename(filename);
    view(filename);
    printf("\n###############\n");
    disp_info(nn);
    printf("###############\n");
    printf(" ## 화면을 지울까요?(%s) (y/N)>>",filename);
    if (yesno("",No)) {
        unlink(filename);
        re_load=Yes;
        return Yes;
    }
    return No;
}


delete_data(int nn)
{
    char filename[300];
    if (nn<1||nn>MAX) return;
    xfrm=afrm[nn-1];
    set_filename(filename);
    disp_info(nn);
    printf("\n ## 이 데이타를 %s에서 지울까요? (y/N)>>",datafile);
    if (yesno("",No)) {
        strcpy(afrm[nn-1].title,"");
        save_data();
    }
}

/*
123 123456789012 1234567890123456789012345678901234567890 123456 123456
  1 select.bbs   설명
*/


save_data()
{
    FILE *fp;
    int i,n;
    if ((fp=fopen(datafile,"w"))!=NULL) {
        n=1;
        printf("\n... 잠깐만 기다려 주십시요.....\n");
		fflush(stdout);
        for (i=0;i<3000;i++) {
            xfrm=afrm[i];
            if (strlen(xfrm.filename)<1) continue;
            fprintf(fp,"%-3d %-12s %-40s %-6d %s\n",
                n++,xfrm.filename,xfrm.title,
                xfrm.filesize,datestr(2,xfrm.date));
            printf(".");fflush(stdout);
        }
        re_load=Yes;
        fclose(fp);
    }
}



add_data()
{
    total++;
    if (total>1) afrm[total-1]=afrm[total-2];
    if (total<1) total=1;
    edit_data(total);
}


edit_data(int nn)
{
    char buff[300];
    if (nn<1||nn>MAX) return;
    disp_info(nn);
    xfrm=afrm[nn-1];
    printf("  파 일 명 : ");
    lineinput(buff,12);printf("\n");
    if (strlen(buff)==1&&is_char("QqPpXx",buff[0])) return;
    if (strlen(buff)>1) strcpy(xfrm.filename,buff);
    printf("  파일설명 : ");
    lineinput(buff,40);printf("\n");
    if (strlen(buff)==1&&is_char("QqPpXx",buff[0])) return;
    if (strlen(buff)>1) strcpy(xfrm.title,buff);
    afrm[nn-1]=xfrm;
    disp_info(nn);
    save_data();
}

//구조체를 변수로 넘기므로 ->연산자를 사용해야함
int comp1(XFRM *xfrm1,XFRM *xfrm2) {
    return strcmp(xfrm1->filename,xfrm2->filename);
}

int comp2(XFRM *xfrm1,XFRM *xfrm2) {
    int n=strcmp(xfrm1->title,xfrm2->title);
    if (n==0) return (comp1(xfrm1,xfrm2));
    return n;
}

int comp3(XFRM *xfrm1,XFRM *xfrm2) {
  return (xfrm2->filesize-xfrm1->filesize);
}

sort_data(int mode)
{
    char buff[300];
    if (mode==1) qsort(afrm,3000,sizeof(afrm[0]),comp1);
    if (mode==2) qsort(afrm,3000,sizeof(afrm[0]),comp2);
    else         qsort(afrm,3000,sizeof(afrm[0]),comp3);
    save_data();
}


disp_info(int nn)
{
    char filename[300];
    if (nn<1||nn>MAX) return;
    xfrm=afrm[nn-1];
    set_filename(filename);
    printf("  번    호 : %d\n",nn);
    printf("  파    일 : %s\n",filename);
    printf("  파 일 명 : %s\n",xfrm.filename);
    printf("  파일설명 : %s\n",xfrm.title);
    printf("  파일크기 : %d\n",xfrm.filesize);
    printf("  최종날짜 : %s\n",xfrm.mode==0?"파일없음":datestr(2,xfrm.date));
}


disp_frm(int nn)
{
    char filename[300];
    if (nn<1||nn>MAX) return;
    xfrm=afrm[nn-1];
    set_filename(filename);
    if (!view(filename)) printf("\n ## %s 화면이 없습니다.\n",filename);
    else printf("\n ## 화면의 끝입니다.(%s)\n",filename);
    disp_info(nn);
    pressenter();
}


re_loadfrm()
{
    FILE *fp;
    char str[300];
    char buff[300];
    int i;

    total=0;
    total_size=0;
    for (i=0;i<3000;i++) strcpy(afrm[i].filename,"");
    if ((fp=fopen(datafile,"r"))==NULL) {
        mess("%s를 읽을수 없습니다.",datafile);
        return;
    }
    total=0;
    while (fgets(str,200,fp)) {
        del_enter(str);
        if (strlen(str)<10) continue;
        strcat(str,"                                                                  ");
        xstrncpy(xfrm.filename,&str[4],12);
        xstrncpy(xfrm.title   ,&str[17],40);
        del_space(xfrm.filename);
        all_trim(xfrm.title);
        if (strlen(xfrm.filename)<1) continue;
        if (is_same(xfrm.filename,".")||is_same(xfrm.filename,"..")) continue;
        sprintf(buff,"%s/%s/%s",fdir,ndir,xfrm.filename);
        if (!is_file(buff)) {
            xfrm.mode=0;
            xfrm.filesize=0;
            xfrm.date=0;
        }
        else                {
            xfrm.mode=1;
            xfrm.filesize=file_size(buff);
            xfrm.date=file_time(buff);
        }
        afrm[total++]=xfrm;
        total_size+=xfrm.filesize;
    }
    fclose(fp);
}


set_filename(char *filename)
{
    if (is_same(fdir,"./")||strlen(fdir)<1) {
        sprintf(filename,"%s/%s",ndir,xfrm.filename);
    }
    else {
        sprintf(filename,"%s/%s/%s",fdir,ndir,xfrm.filename);
    }
    strchng2(filename,"//","/");
}


view_alldatestr()
{
    time_t today;
    int i;
    time(&today);
    for (i=0;i<=99;i++) {
        printf("%2d. %s\n",i,datestr(i,today));
        if (i%20==19) pressenter();
    }
    pressenter();
}
