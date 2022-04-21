///////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 접속기록/과금보기
///////////////////////////////////////////////////////////////////////////
#include "util.c"
byte sall,syear,smonth,smode1,smode2;
byte shostno1,shostno2,stype1,stype2;
int ssec1,ssec2,sprice11,sprice12;
int skb1,skb2,sprice21,sprice22;
byte dispmode;
char sid[9],sname[9],sttyport[9],shostname[41],scomp[30];
time_t slogin1,slogin2;

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
    char tid[20];
    char tcode[20];
    char ccode[20];
    char tclub[20];
	int mode;
    if (argc<4) {
        printf("\n사용법  mlog <ID>\n");
		return;
	}

	findport(tty); //접속포트 찾기
	read_cfg();
    set_tmpfile();                    //임시화일지정하기
    set_nowmenu();                    //데이타화일지정하기
    strcpy(tid,argv[1]);              //ID
    if (!read_pf(tid))     {mess("%s ID 검색 실패",tid);return;}
    while(1) {
        cls();
        printf("  접속 기록 확인\n");
        printfL2();
        printf("  1. 작업선택           :%d  (0:조건 1:접속통계 2:사용통계 3:과금통계)
        printf("  2. 작업분류           :%d  (0:전체 1:접속시작 2:접속종료 9:단순기록)
        printf("  3. 대상분류           :%d  (0:전체 1:메인 2:동호 3:게시 4:다운 5:대화 6:텔넷)
        printf("  4. 작업동호회         :%s  (공백:전체  main,동호회가기코드)
        printf("  5. 작업가기코드       :%s  (공백:전체  top,가기코드)
        printf("  6. 확인대상ID(사용자) :%s  (공백:전체)
        printf("  7. 정보제공ID(등록자) :%s  (공백:전체)
        printf("  8. 사용자등급  시작~끝:%d ~ %d
        printf("  9. 접속호스트         :%s
        printf(" 10. 접속시간    시작~끝:%s ~ %s\n",,
        printf(" 11. 사용시간    시작~끝:%d ~ %d
        printf(" 12. 다운용량    시작~끝:%d ~ %d
        printf(" 13. 분당금액    시작~끝:%d ~ %d
        printf(" 14. KB당금액    시작~끝:%d ~ %d

        printf(" 91. 계산제외ID지정
        printf(" 92. 계산제외호스트지정
        printf(" 99. 위의 조건으로 검색시작
        printf(" 선택>>");
        n=numinput();
             if (n==0) return;
        else if (n>=1&&n<99) set_filter(n);
        else if (n==99) start_mlog();
    }
}


set_filter(int mode)
{
    if (mode>=1&&mode<=12) xy(27,2+mode);
         if (mode==1) smode=get_num(smode,1);
    else if (mode==2) stype=get_num(stype,1);
    else if (mode==3) sjob =get_num(sjob ,1);
    else if (mode==4) get_str(sclub,3);
    else if (mode==5) get_str(scode,3);
    else if (mode==6) get_str(sid  ,3);
    else if (mode==7) get_str(sbid ,3);
    else if (mode==8) {
        sitype1=get_num(sitype1,2);
        sitype2=get_num(sitype2,2);
    }
    else if (mode==9) shost=get_str(shost,3);
    else if (mode==10) {
        get_str(sdate1,14);
        get_str(sdate2,14);
    }
    else if (mode==11) {
        smin1=get_num(smin1,7);
        smin2=get_num(smin2,7);
    }
    else if (mode==12) {
        skb1=get_num(skb1,7);
        skb2=get_num(skb2,7);
    }
    else if (mode==13) {
        sprmin1=get_num(sprmin1,7);
        sprmin2=get_num(sprmin2,7);
    }
    else if (mode==14) {
        sprkb1=get_num(sprkb1,7);
        sprkb2=get_num(sprkb2,7);
    }
    else if (mode==91) {
        xedit("system/mlog.nid");
        xedit("system/mlog.nho");
    }
}

view_log()
{
    FILE *fp,*fp2;
    char buff[300],buff1[100],buff2[50],buff3[50],buff5[50];
    char deffile[200];
    char filename[200];
    int i,n;

    strcpy(deffile,"system/vlog.cfg");
    if ((fp2=fopen(deffile,"r"))!=NULL) {
        if (fgets(buff,200,fp2)) syear =atoi(buff);
        if (fgets(buff,200,fp2)) smonth=atoi(buff);
        if (fgets(buff,200,fp2)) smode1=atoi(buff);
        if (fgets(buff,200,fp2)) smode2=atoi(buff);
        if (fgets(buff,200,fp2)) strcpy(sid,buff);del_clear(sid);
        if (fgets(buff,200,fp2)) strcpy(sname,buff);del_clear(sname);
        if (fgets(buff,200,fp2)) stype1=atoi(buff);
        if (fgets(buff,200,fp2)) stype2=atoi(buff);
        if (fgets(buff,200,fp2)) strcpy(shostname,buff);del_clear(shostname);
        if (fgets(buff,200,fp2)) shostno1=atoi(buff);
        if (fgets(buff,200,fp2)) shostno2=atoi(buff);
        if (fgets(buff,200,fp2)) strcpy(scomp,buff);del_clear(scomp);
        if (fgets(buff,200,fp2)) ssec1=atoi(buff);
        if (fgets(buff,200,fp2)) ssec2=atoi(buff);
        if (fgets(buff,200,fp2)) sprice11=atoi(buff);
        if (fgets(buff,200,fp2)) sprice12=atoi(buff);
        if (fgets(buff,200,fp2)) skb1=atoi(buff);
        if (fgets(buff,200,fp2)) skb2=atoi(buff);
        if (fgets(buff,200,fp2)) sprice21=atoi(buff);
        if (fgets(buff,200,fp2)) sprice22=atoi(buff);
        if (fgets(buff,200,fp2)) dispmode=atoi(buff);
        if (fgets(buff,200,fp2)) sall=atoi(buff);
        fclose(fp2);
    }
    while(1) {
        cls();
        view("frm/rec/vlog.scr");
        del_space(sttyport);del_space(sid);del_space(sname);del_space(shostname);del_space(scomp);
        if (strlen(sttyport   )<1) strcpy(sttyport   ,"전체");
        if (strlen(sid        )<1) strcpy(sid        ,"전체");
        if (strlen(sname      )<1) strcpy(sname      ,"전체");
        if (strlen(shostname  )<1) strcpy(shostname  ,"전체");
        if (strlen(scomp      )<1) strcpy(scomp      ,"전체");
        if (dispmode>9) dispmode=0;

        if ((fp2=fopen(deffile,"w"))!=NULL) {
            fprintf(fp2,"%d\n",syear );
            fprintf(fp2,"%d\n",smonth);
            fprintf(fp2,"%d\n",smode1);
            fprintf(fp2,"%d\n",smode2);
            fprintf(fp2,"%s\n",sid);
            fprintf(fp2,"%s\n",sname);
            fprintf(fp2,"%d\n",stype1);
            fprintf(fp2,"%d\n",stype2);
            fprintf(fp2,"%s\n",shostname);
            fprintf(fp2,"%d\n",shostno1);
            fprintf(fp2,"%d\n",shostno2);
            fprintf(fp2,"%s\n",scomp);
            fprintf(fp2,"%d\n",ssec1);
            fprintf(fp2,"%d\n",ssec2);
            fprintf(fp2,"%d\n",sprice11);
            fprintf(fp2,"%d\n",sprice12);
            fprintf(fp2,"%d\n",skb1);
            fprintf(fp2,"%d\n",skb2);
            fprintf(fp2,"%d\n",sprice21);
            fprintf(fp2,"%d\n",sprice22);
            fprintf(fp2,"%d\n",dispmode);
            fprintf(fp2,"%d\n",sall);
            fclose(fp2);
        }

        if (bit(cfg.mlogrecmode,4)) sprintf(filename,"/tmp/xhost/rec/mlog%02d%02d.rec",syear,smonth);
        else                        sprintf(filename,"user/rec/mlog%02d%02d.rec",syear,smonth);
        xy(40, 4);printf("%s    %d Byte  %d Record\n",filename,file_size(filename),count_rec(filename,sizeof_mlogrec));
        xy(40, 5);printf("%s\n",sall?"전체":"조건");
        xy(40, 6);printf("%2d년 %2d월\n",syear,smonth);
        xy(40, 7);printf("%3d - %3d\n",smode1,smode2);
        xy(40, 8);printf("%s\n",sid);
        xy(40, 9);printf("%s\n",sname);
        xy(40,10);printf("%3d - %3d\n",stype1,stype2);
        xy(40,11);printf("%s\n",sttyport);
        xy(40,12);printf("%s\n",shostname);
        xy(40,13);printf("%3d - %3d\n",shostno1,shostno2);
        xy(40,14);printf("%3s\n",scomp);
        xy(40,15);printf("%s - %s\n",datestr(2,slogin1),datestr(2,slogin2));
        xy(40,16);printf("%8d - %8d\n",ssec1,ssec2);
        xy(40,17);printf("%8d - %8d\n",sprice11,sprice12);
        xy(40,18);printf("%8d - %8d\n",skb1,skb2);
        xy(40,19);printf("%8d - %8d\n",sprice21,sprice22);
        xy(40,20);printf("%d\n",dispmode);
        xy(40,22);
        lineinput(buff,20);check_cmd(buff);printf("\n");n=atoi(buff);
        xy(40,n+5);
             if (is_char("qQxXpPmM",buff[0])) break;
        else if (buff[0]=='0') sall=sall?No:Yes;
        else if (n== 1) {xy(40,n+5);syear=get_num(syear,2);xy(45,n+5);smonth=get_num(smonth,2);}
        else if (n== 2) {xy(40,7);smode1=get_num(smode1,3);xy(46,7);smode2=get_num(smode2,3);}
        else if (n== 3) get_str(sid,8);
        else if (n== 4) get_str(sname,8);
        else if (n== 5) {xy(40,10);stype1=get_num(stype1,3);xy(46,10);stype2=get_num(stype2,3);}
        else if (n== 6) get_str(sttyport,10);
        else if (n== 7) get_str(shostname,30);
        else if (n== 8) {xy(40,13);shostno1=get_num(shostno1,3);xy(46,13);shostno2=get_num(shostno2,3);}
        else if (n== 9) get_str(scomp,4);
        else if (n==10) {
            strcpy(buff1,datestr(2,slogin1));
            strcpy(buff2,datestr(2,slogin2));
            xy(40,15);get_str(buff1,10);
            xy(55,15);get_str(buff2,10);
            slogin1=str2date2(buff1);
            slogin2=str2date2(buff2);
        }
        else if (n==11) {xy(40,16);ssec1=get_num(ssec1,8);xy(51,16);ssec2=get_num(ssec2,8);}
        else if (n==12) {xy(40,17);sprice11=get_num(sprice11,8);xy(51,17);sprice12=get_num(sprice12,8);}
        else if (n==13) {xy(40,18);skb1 =get_num(skb1 ,8);xy(51,18);skb2 =get_num(skb2 ,8);}
        else if (n==14) {xy(40,19);sprice21=get_num(sprice21,8);xy(51,17);sprice22=get_num(sprice22,8);}
        else if (n==15) dispmode++;
        else if (n==99) start_vlog();           //검색시작
    }
}


start_vlog()
{
    FILE *fp,*fp2;
    MLOGREC mlogrec;
    int i1,i2,i3,i4,i5;
    char filename[300];

    view("frm/rec/vlogwait.log");
    if (is_same(sid      ,"전체")) strcpy(sid      ,"");
    if (is_same(sname    ,"전체")) strcpy(sname    ,"");
    if (is_same(sttyport ,"전체")) strcpy(sttyport ,"");
    if (is_same(scomp    ,"전체")) strcpy(scomp    ,"");
    if (is_same(shostname,"전체")) strcpy(shostname,"");
    i1=i2=i3=i4=i5=0;
    if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
    if (bit(cfg.mlogrecmode,4)) sprintf(filename,"/tmp/xhost/rec/mlog%02d%02d.rec",syear,smonth);
    else                        sprintf(filename,"user/rec/mlog%02d%02d.rec",syear,smonth);
    fprintf(fp2,"\n< FILE:%s %d Byte  %d Record >\n",filename,file_size(filename),count_rec(filename,sizeof_mlogrec));
    if (dispmode==1) fprintf(fp2,"순번  아이디  성명 등급 분류 포트 호스트  사용시간 사용금액1 사용금액2\n");
    if (dispmode==2) fprintf(fp2,"순번  아이디  등급 사용시간   KB  사용금액\n");
    fprintf(fp2,"======================================================================\n");
    if ((fp=fopen(filename,"r"))==NULL) goto not_found1;
    while (fread(&mlogrec,sizeof_mlogrec,1,fp)) {
        if (!sall) {
            if (mlogrec.mode<smode1||mlogrec.mode>smode2) continue;
            if (mlogrec.type<stype1||mlogrec.type>stype2) continue;
            if (mlogrec.sec<ssec1||mlogrec.sec>ssec2) continue;
            if (mlogrec.kb <skb1 ||mlogrec.kb >skb2 ) continue;
            if (mlogrec.price1<sprice11||mlogrec.price1>sprice12) continue;
            if (mlogrec.price2<sprice21||mlogrec.price2>sprice22) continue;
            if (strlen(sid      )>0&&!is_same(mlogrec.id       ,sid      )) continue;
            if (strlen(sname    )>0&&!is_same(mlogrec.name     ,sname    )) continue;
            if (strlen(sttyport )>0&&!stristr(mlogrec.tty      ,sttyport )) continue;
            if (strlen(scomp    )>0&&!stristr(mlogrec.comp     ,scomp    )) continue;
            if (strlen(shostname)>0&&!stristr(mlogrec.hostname ,shostname)) continue;
            if (mlogrec.login<slogin1||mlogrec.login>slogin2)               continue;
        }
        i1++;
        i2+=mlogrec.price1;
        i3+=mlogrec.price2;
        i4+=mlogrec.sec;
        i5+=mlogrec.kb;
        if (dispmode==1)
            fprintf(fp2,"%5d. %-8s %-8s %2d %2d %-6s %-26s %3d %8d %8d\n",
            i1,mlogrec.id,mlogrec.name,mlogrec.type,mlogrec.mode,
            mlogrec.tty,mlogrec.hostname,mlogrec.sec,
            mlogrec.price1,mlogrec.price2);
        if (dispmode==2)
            fprintf(fp2,"%5d. %-8s %2d %8d %4d %8d\n",
            i1,mlogrec.id,mlogrec.type,mlogrec.sec,mlogrec.kb,
            mlogrec.price1+mlogrec.price2);
    }
    fclose(fp);
not_found1:
    fprintf(fp2,"======================================================================\n");
    fprintf(fp2,"  총건수:%d  총사용시간:%d  총금액:%d   총전송KB:%d  총금액:%d\n",i1,i4,i2,i5,i3);
    fclose(fp2);
    view_text(TMP_FILE);
    unlink(TMP_FILE);
}
