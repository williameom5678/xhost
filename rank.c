////////////////////////////////////////////////////////////////////////
//            XHOST 5.3s: ¸®´ª½º¿ë BBS HOST                            //
//			  °³¹ßÀÚ : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)		  //
//            ÀÌ ¼Ò½º¸¦ º¯°æÇÏ¿© »ó¿ëÀ¸·Î »ç¿ëÇÏ½Ç¶§¿¡´Â              //
//            °³¹ßÀÚÀÇ ½ÂÀÎÀ» ¹Þ¾Æ¾ß ÇÕ´Ï´Ù.                          //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//  ¼øÀ§Ç¥ :´Üµ¶µ¿ÀÛ ÇÁ·Î±×·¥
////////////////////////////////////////////////////////////////////

#include "util.c"
int total;
PF logpf,apf1[132],apf2[132],apf3[132];

//rank mode1 mode2 mode3
//mode1:1 rank  2:Á¢¼Ó
//mode2:1 ÀÏ¹Ý  2:¿î¿µÁø
//mode3:1 ±Ý¿ù  2:ÀüÃ¼
void main(argc, argv)
int argc;
char *argv[];
{
	char buff[512];
    int i,n,n1,n2;
    int mode=1,mode2=0;


	findport(tty); //Á¢¼ÓÆ÷Æ® Ã£±â
	read_cfg();
	set_tmpfile();		 //ÀÓ½ÃÈ­ÀÏÁöÁ¤ÇÏ±â
	set_nowmenu();		 //µ¥ÀÌÅ¸È­ÀÏÁöÁ¤ÇÏ±â
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
                strcpy(g_buff1,"3. ¿î¿µÁø ±Ý¿ù ¼øÀ§");
                strcpy(g_buff2,"4. ¿î¿µÁø ÀüÃ¼ ¼øÀ§");
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
                 if (n==1) rescan1(TMP_FILE2,1,1);      //mode1:1:ÀÏ¹Ý 2:¿î¿µÁø   mode2:1:¿ùº° 2:ÀüÃ¼
            else if (n==2) rescan1(TMP_FILE2,1,2);      //mode1:1:ÀÏ¹Ý 2:¿î¿µÁø   mode2:1:¿ùº° 2:ÀüÃ¼
            else if (n==3) rescan1(TMP_FILE2,2,1);      //mode1:1:ÀÏ¹Ý 2:¿î¿µÁø   mode2:1:¿ùº° 2:ÀüÃ¼
            else if (n==4) rescan1(TMP_FILE2,2,2);      //mode1:1:ÀÏ¹Ý 2:¿î¿µÁø   mode2:1:¿ùº° 2:ÀüÃ¼
            else if (n==5) rescan3(TMP_FILE2,1,1);      //ÀÏ¹ÝÀÎ ¾²±â ¼øÀ§
            else if (n==6) rescan3(TMP_FILE2,2,1);      //ÀüÃ¼   ¾²±â ¼øÀ§
            else if (n==7) rescan4(TMP_FILE2,1,1);      //ÀÏ¹ÝÀÎ ¾²±â ¼øÀ§
            else if (n==8) rescan4(TMP_FILE2,2,1);      //ÀüÃ¼   ¾²±â ¼øÀ§
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



//¸¶Áö¸· ÀÚ·á¸¦ Ã£¾Æ ÀÚ·áÀÇ ¼ö¸¦ °è»êÇÔ
//mode1:1:ÀÏ¹Ý 2:¿î¿µÁø   mode2:1:¿ùº° 2:ÀüÃ¼
rescan1(char *filename,int mode1,int mode2) {
    FILE *fp;
    int  i,n1,n2,n3;

    printf("\n Àá±ñ¸¸ ±â´Ù·Á ÁÖ½Ê½Ã¿ä.. Áö±Ý ÀÚ·áÁ¤¸® ÁßÀÔ´Ï´Ù.\n");

    fp = fopen(ID_FILE, "r");
    total=0;
    rewind(fp);
    while(fread(&pf,sizeof_pf,1,fp)) {         //¾ÆÀÌµð ÀúÀå
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
    fprintf(fp,"[2;4HÃÑÈ¸¿ø:%5d¸í",total);
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

    printf("\n\n\n *** Àá±ñ¸¸ ±â´Ù·Á ÁÖ¼¼¿ä Áö±Ý ÀÚ·á Á¤¸®ÁßÀÔ´Ï´Ù. *** \n\n\n");
    fp = fopen(ID_FILE, "r");
    total=0;                  //È¸¿ø ÃÑ ¼ö °è»ê
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
        fprintf(fp,"%s ¡­ ",datestr(2,apf1[i].login));
        fprintf(fp,"%s ",datestr(3,apf1[i].logout));
        fprintf(fp,"%4d /%4d\n",apf1[i].month_log,apf1[i].all_log);

    }
    fprintf(fp,"[21;01H");
    fclose(fp);

}

//mode1:1:ÀÏ¹Ý 2:¿î¿µÁø   mode2:1:¿ùº° 2:ÀüÃ¼
rescan3(char *filename,int mode1,int mode2) {
    FILE *fp;
    int  i,n1,n2,n3;

    printf("\n Àá±ñ¸¸ ±â´Ù·Á ÁÖ½Ê½Ã¿ä.. Áö±Ý ÀÚ·áÁ¤¸® ÁßÀÔ´Ï´Ù.\n");

    fp = fopen(ID_FILE, "r");
    total=0;
    rewind(fp);
    while(fread(&pf,sizeof_pf,1,fp)) {         //¾ÆÀÌµð ÀúÀå
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
    fprintf(fp,"[2;4HÃÑÈ¸¿ø:%5d¸í",total);
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


//mode1:1:ÀÏ¹Ý 2:¿î¿µÁø   mode2:1:¿ùº° 2:ÀüÃ¼
rescan4(char *filename,int mode1,int mode2) {
    FILE *fp;
    int  i,n1,n2,n3;

    printf("\n Àá±ñ¸¸ ±â´Ù·Á ÁÖ½Ê½Ã¿ä.. Áö±Ý ÀÚ·áÁ¤¸® ÁßÀÔ´Ï´Ù.\n");

    fp = fopen(ID_FILE, "r");
    total=0;
    rewind(fp);
    while(fread(&pf,sizeof_pf,1,fp)) {         //¾ÆÀÌµð ÀúÀå
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
    fprintf(fp,"[2;4HÃÑÈ¸¿ø:%5d¸í",total);
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
