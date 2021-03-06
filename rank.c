////////////////////////////////////////////////////////////////////////
//            XHOST 5.3s: 리눅스용 BBS HOST                            //
//			  개발자 : 김성대  xhost@xhost.co.kr (016-320-7882)		  //
//            이 소스를 변경하여 상용으로 사용하실때에는              //
//            개발자의 승인을 받아야 합니다.                          //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//  순위표 :단독동작 프로그램
////////////////////////////////////////////////////////////////////

#include "util.c"
int total;
PF logpf,apf1[132],apf2[132],apf3[132];

//rank mode1 mode2 mode3
//mode1:1 rank  2:접속
//mode2:1 일반  2:운영진
//mode3:1 금월  2:전체
void main(argc, argv)
int argc;
char *argv[];
{
	char buff[512];
    int i,n,n1,n2;
    int mode=1,mode2=0;


	findport(tty); //접속포트 찾기
	read_cfg();
	set_tmpfile();		 //임시화일지정하기
	set_nowmenu();		 //데이타화일지정하기
    read_pf(argv[1]);
    logpf=pf;
    mode=atoi(argv[2]);
    mode2=0;
    if (is_same(argv[3],"rk1")) mode2=1;
    if (is_same(argv[3],"rk2")) mode2=2;
    if (is_same(argv[3],"rk3")) mode2=3;
    if (is_same(argv[3],"rk4")) mode2=4;
    if (mode2>0) {
        n=mode2;
        goto _direct;
    }


    if (mode==1) {
        while (1) {
            strcpy(g_buff1,"");
            strcpy(g_buff2,"");
            if (logpf.type>=14) {
                strcpy(g_buff1,"3. 운영진 금월 순위");
                strcpy(g_buff2,"4. 운영진 전체 순위");
            }
            view("frm/rank/rank.sel");
            lineinput(buff,3);check_han(buff);
            if (is_same(buff,"q")||is_same(buff,"p")||
                is_same(buff,"qq")||is_same(buff,"pp")||
                is_same(buff,"x")||is_same(buff,"xx")) return;
            n=atoi(buff);
            _direct:
            clear_apf();
            if (logpf.type<14&&(n==3||n==4)) n=1;
                 if (n==1) rescan1(TMP_FILE2,1,1);      //mode1:1:일반 2:운영진   mode2:1:월별 2:전체
            else if (n==2) rescan1(TMP_FILE2,1,2);      //mode1:1:일반 2:운영진   mode2:1:월별 2:전체
            else if (n==3) rescan1(TMP_FILE2,2,1);      //mode1:1:일반 2:운영진   mode2:1:월별 2:전체
            else if (n==4) rescan1(TMP_FILE2,2,2);      //mode1:1:일반 2:운영진   mode2:1:월별 2:전체
            else if (n==5) rescan3(TMP_FILE2,1,1);      //일반인 쓰기 순위
            else if (n==6) rescan3(TMP_FILE2,2,1);      //전체   쓰기 순위
            else if (n==7) rescan4(TMP_FILE2,1,1);      //일반인 쓰기 순위
            else if (n==8) rescan4(TMP_FILE2,2,1);      //전체   쓰기 순위
            cls();
            view("frm/rank/rank%d.scr",n);
            view_text_line(TMP_FILE2,15);
            if (mode2>0) break;
        }
    }
    else if (mode==2) {
        clear_apf();
        rescan2(TMP_FILE2);
        cls();
        view("frm/rank/ranklog.scr");
        view_text_line(TMP_FILE2,15);
    }
    unlink(TMP_FILE2);
}



//마지막 자료를 찾아 자료의 수를 계산함
//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
rescan1(char *filename,int mode1,int mode2) {
    FILE *fp;
    int  i,n1,n2,n3;

    printf("\n 잠깐만 기다려 주십시요.. 지금 자료정리 중입니다.\n");

    fp = fopen(ID_FILE, "r");
    total=0;
    rewind(fp);
    while(fread(&pf,sizeof_pf,1,fp)) {         //아이디 저장
        if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
        if (mode1==1&&pf.type>=14) continue;
        total++;
        for (i=120;i>=0;i--) {
            if (pf.level<apf1[i].level) break;
            apf1[i+1]=apf1[i];
            apf1[i]=pf;
        }
        if (mode2==1) {
            for (i=120;i>=0;i--) {
                if (pf.month_log==0||pf.month_log<apf2[i].month_log) break;
                apf2[i+1]=apf2[i];
                apf2[i]=pf;
            }
            for (i=120;i>=0;i--) {
                if (pf.month_time==0||pf.month_time<apf3[i].month_time) break;
                apf3[i+1]=apf3[i];
                apf3[i]=pf;
            }
        }
        else {
            for (i=120;i>=0;i--) {
                if (pf.all_log==0||pf.all_log<apf2[i].all_log) break;
                apf2[i+1]=apf2[i];
                apf2[i]=pf;
            }
            for (i=120;i>=0;i--) {
                if (pf.all_time==0||pf.all_time<apf3[i].all_time) break;
                apf3[i+1]=apf3[i];
                apf3[i]=pf;
            }
        }
    }

    fclose(fp);

    if ((fp=fopen(filename, "w"))==NULL) return;
    fprintf(fp,"[2;4H총회원:%5d명",total);
    for (i=0;i<120;i++) {
        if (mode2==1) {
            n2=apf2[i].month_log;
            n3=apf3[i].month_time;
        }
        else {
            n2=apf2[i].all_log;
            n3=apf3[i].all_time;
        }
        fprintf(fp,"[%02d;5H%3d.  %-8s %6d   | %3d. %-8s %6d   | %3d. %-8s%4d:%02d\n",
        (i%15)+6,
        i+1,apf1[i].id, apf1[i].level,
        i+1,apf2[i].id, n2,
        i+1,apf3[i].id, n3/3600,(n3%3600)/60);
    }
    fprintf(fp,"[21;01H");
    fclose(fp);
}


rescan2(char *filename) {
    FILE *fp;
    int  i;
    char str[100];

    printf("\n\n\n *** 잠깐만 기다려 주세요 지금 자료 정리중입니다. *** \n\n\n");
    fp = fopen(ID_FILE, "r");
    total=0;                  //회원 총 수 계산
    while(fread(&pf,sizeof_pf,1,fp)) {
        if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
        total++;
        for (i=120;i>=0;i--) {
            if (pf.login>pf.logout) pf.logout=pf.login;
            if (pf.logout<apf1[i].logout) break;
            apf1[i+1]=apf1[i];
            apf1[i]=pf;
        }
    }
    fclose(fp);

    if ((fp=fopen(filename, "w"))==NULL) return;
    for (i=0;i<120;i++) {
        if (strlen(apf1[i].id)<3) continue;
        fprintf(fp,"[%02d;5H%3d.  %-8s(%-8s) %2d    ",
                (i%15)+6,i+1,apf1[i].id, apf1[i].name,apf1[i].type);
        fprintf(fp,"%s ∼ ",datestr(2,apf1[i].login));
        fprintf(fp,"%s ",datestr(3,apf1[i].logout));
        fprintf(fp,"%4d /%4d\n",apf1[i].month_log,apf1[i].all_log);

    }
    fprintf(fp,"[21;01H");
    fclose(fp);

}

//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
rescan3(char *filename,int mode1,int mode2) {
    FILE *fp;
    int  i,n1,n2,n3;

    printf("\n 잠깐만 기다려 주십시요.. 지금 자료정리 중입니다.\n");

    fp = fopen(ID_FILE, "r");
    total=0;
    rewind(fp);
    while(fread(&pf,sizeof_pf,1,fp)) {         //아이디 저장
        if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
        if (mode1==1&&pf.type>=14) continue;
        total++;
        for (i=120;i>=0;i--) {
            if (pf.countw==0||pf.countw<apf1[i].countw) break;
            apf1[i+1]=apf1[i];
            apf1[i]=pf;
        }
        for (i=120;i>=0;i--) {
            if (pf.countu==0||pf.countu<apf2[i].countu) break;
            apf2[i+1]=apf2[i];
            apf2[i]=pf;
        }
        for (i=120;i>=0;i--) {
            if (pf.countd==0||pf.countd<apf3[i].countd) break;
            apf3[i+1]=apf3[i];
            apf3[i]=pf;
        }
    }

    fclose(fp);

    if ((fp=fopen(filename, "w"))==NULL) return;
    fprintf(fp,"[2;4H총회원:%5d명",total);
    for (i=0;i<120;i++) {
        fprintf(fp,"[%02d;5H%3d.  %-8s %6d   | %3d. %-8s %6d   | %3d. %-8s %6d\n",
        (i%15)+6,
        i+1,apf1[i].id, apf1[i].countw,
        i+1,apf2[i].id, apf2[i].countu,
        i+1,apf3[i].id, apf3[i].countd);
    }
    fprintf(fp,"[21;01H");
    fclose(fp);
}


//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
rescan4(char *filename,int mode1,int mode2) {
    FILE *fp;
    int  i,n1,n2,n3;

    printf("\n 잠깐만 기다려 주십시요.. 지금 자료정리 중입니다.\n");

    fp = fopen(ID_FILE, "r");
    total=0;
    rewind(fp);
    while(fread(&pf,sizeof_pf,1,fp)) {         //아이디 저장
        if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
        if (mode1==1&&pf.type>=14) continue;
        total++;
        for (i=120;i>=0;i--) {
            if (pf.warn==0||pf.warn<apf1[i].warn) break;
            apf1[i+1]=apf1[i];
            apf1[i]=pf;
        }
        for (i=120;i>=0;i--) {
            if (pf.point==0||pf.point<apf2[i].point) break;
            apf2[i+1]=apf2[i];
            apf2[i]=pf;
        }
        for (i=120;i>=0;i--) {
            if (pf.good==0||pf.good<apf3[i].good) break;
            apf3[i+1]=apf3[i];
            apf3[i]=pf;
        }
    }

    fclose(fp);

    if ((fp=fopen(filename, "w"))==NULL) return;
    fprintf(fp,"[2;4H총회원:%5d명",total);
    for (i=0;i<120;i++) {
        fprintf(fp,"[%02d;5H%3d.  %-8s %6d   | %3d. %-8s %6d   | %3d. %-8s %6d\n",
        (i%15)+6,
        i+1,apf1[i].id, apf1[i].warn,
        i+1,apf2[i].id, apf2[i].point,
        i+1,apf3[i].id, apf3[i].good);
    }
    fprintf(fp,"[21;01H");
    fclose(fp);
}



clear_apf()
{
    int i;
    PF apf;
    strcpy(apf.id,"");
    apf.level=0;
    apf.month_log=0;
    apf.all_log=0;
    apf.month_time=0;
    apf.all_time=0;
    apf.type=0;
    apf.login=0;
    apf.logout=0;
    apf.countu=0;
    apf.countd=0;
    apf.countw=0;
    apf.good=0;
    apf.warn=0;
    apf.point=0;
    for (i=0;i<122;i++) {
        apf1[i]=apf;
        apf2[i]=apf;
        apf3[i]=apf;
    }
}
