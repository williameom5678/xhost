////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ȭ�� ���հ���
///////////////////////////////////////////////////////////////////////////
#include <dirent.h>
#include "bbs.h"

typedef struct {
    char mode;             //�ִ°�?
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
	findport(tty); //������Ʈ ã��
	read_cfg();
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
    set_nowmenu();       //����Ÿȭ�������ϱ�
    if (is_same(argv[1],"datestr")) {
        view_alldatestr();
        return;
    }
    mode=atoi(argv[1]);                 //��ȣȸ ���
    strcpy(ccode,argv[2]);              //��ȣȸ �ڵ�
    strcpy(tcode,argv[3]);              //��ȣȸ �ڵ�
    strcpy(now_id,argv[4]);             //���̵�

    find_club(ccode);
	set_nowmenu();		 //����Ÿȭ�������ϱ�
    find_go(tcode);
    read_ccfg();
    strcpy(fdir,club_go.dir);
	if (is_same(now_id,"root")) {
        who=5;
        strcpy(now_id,cfg.sysop);
        read_pf(now_id);    //���̵� �б�
	}
    else if (is_same(cfg.sysop,now_id)) {
        read_pf(now_id);    //���̵� �б�
        who=4;
    }
    else if(read_pf(now_id)) {    //���̵� �б�
		who=0;
		if (pf.type==19) who=4;  //root
		if (pf.type==18) who=3;  //�νû�
		if (pf.type==17) who=2;  //���
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
            strcpy(abuff[i++],"frm/bbs/        �Խ�������      ");
            strcpy(abuff[i++],"frm/bio/        ���̿�����      ");
            strcpy(abuff[i++],"frm/chat/       ��ȭ��          ");
            strcpy(abuff[i++],"frm/down/       �ٿ�ε�        ");
            strcpy(abuff[i++],"frm/editor/     ������,����     ");
            strcpy(abuff[i++],"frm/end/        ����            ");
            strcpy(abuff[i++],"frm/error/      ����            ");
            strcpy(abuff[i++],"frm/guest/      �մ԰���        ");
            strcpy(abuff[i++],"frm/help/       ����          ");
            strcpy(abuff[i++],"frm/level/      ���            ");
            strcpy(abuff[i++],"frm/log/        ���ӽ���        ");
            strcpy(abuff[i++],"frm/mail/       ����            ");
            strcpy(abuff[i++],"frm/notice/     ����            ");
            strcpy(abuff[i++],"frm/pf/         �Ż�            ");
            strcpy(abuff[i++],"frm/rec/        ���            ");
            strcpy(abuff[i++],"frm/say/        �޼���          ");
            strcpy(abuff[i++],"frm/screen/     �Խ���/�޴�ȭ�� ");
            strcpy(abuff[i++],"frm/system/     �ý���          ");
            strcpy(abuff[i++],"frm/telnet/     �ڳ�            ");
            strcpy(abuff[i++],"frm/typegame/   Ÿ�ڴ�ȭ��      ");
            strcpy(abuff[i++],"frm/us/         ������          ");
            strcpy(abuff[i++],"frm/xfrm/       ȭ����������Ÿ  ");
            strcpy(abuff[i++],"logo/           ���θ޴�        ");
        }
        else {      //��ȣȸ�϶�
            strcpy(abuff[i++],"frm/bbs/        �Խ�������      ");
            strcpy(abuff[i++],"frm/end/        ����            ");
            strcpy(abuff[i++],"frm/error/      ����            ");
            strcpy(abuff[i++],"frm/guest/      �մ԰���        ");
            strcpy(abuff[i++],"frm/level/      ���            ");
            strcpy(abuff[i++],"frm/log/        ���ӽ���        ");
            strcpy(abuff[i++],"frm/notice/     ����            ");
            strcpy(abuff[i++],"frm/pf/         �Ż�            ");
            strcpy(abuff[i++],"frm/screen/     �Խ���/�޴�ȭ�� ");
            strcpy(abuff[i++],"logo/           ���θ޴�        ");
        }
        for (i=0;i<30;i++) {
            if (strlen(abuff[i])>1) {
                xstrncpy(adir[i],abuff[i],16);
                strcpy(atit[i],&abuff[i][16]);
                del_space(adir[i]);all_trim(adir[i]);all_trim(atit[i]);
            }
        }
    }

    //����Ÿ �����о���� -> ����ũ��.������¥Ȯ��
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
        print_(1,"%s ���� ���� �۾�",club_go.code);
        printf("  1. �������� ���ȭ��                 frm/notice/notice.scr\n");
        printf("  2. �������� ���� �⺻                frm/notice/notice.dat\n");
        printf("  3. �������� ���� ����                frm/notice/notice.da?\n");
        printf(" 11. ���Խ� ���� �⺻                  frm/log/start.log\n");
        printf(" 12. ���Խ� ���� ����                  frm/log/start?.log\n");
        printf(" 13. ���Խ� ���� ����                  frm/log/startr?.log\n");
        printf(" 14. ���Խ� ���� ����                  frm/log/dayMMDD.log\n");
        printf(" 15. ���Խ� ���� ����                  frm/log/day?.log\n");
        printf(" 16. ���Խ� ���� ����                  frm/log/week?.log\n");
        printf(" 17. ���Խ� ���� �ð�                  frm/log/hour?.log\n");
        printf(" 21. ��ü �ϴ� ���ٱ��� �Ϲ�ȸ��       frm/ad/ad.dat\n");
        printf(" 22. ��ü �ϴ� ���ٱ��� �մ�           frm/guest/guestad.dat\n");
        printf(" 23. ��ü �û� �޼���                  system/sysmess.txt\n");
        printf(" 24. %-8s �޴� �ϴ� ����               logo/%s.ad?\n",go.code,go.code);
        printf(" 25. %-8s �޴� �ڵ� �Ƚ�               logo/%s.ta?\n",go.code,go.code);
        printf(" 31. ���� ���̵� �Է� ���� �⺻        frm/log/startid.add\n");
        printf(" 32. ���� ���̵� �Է� ���� ����        frm/log/startid.ad?\n");
        printf(" 33. ���� ���̵� �Է� ���� ����        frm/log/dayMMDD.add\n");
        printf(" 34. ���� ���̵� �Է� ���� ����        frm/log/day?.add\n");
        printf(" 35. ���� ���̵� �Է� ���� ����        frm/log/week?.add\n");
        printf(" 36. ���� ���̵� �Է� ���� �ð�        frm/log/hour?.add\n");
        printf(" ����(D)������(E)����(UP)�ٿ�(DN)����(��ȣ)����(H)����(P) >>");
        lineinput(buff,20);printf("\n");check_han(buff);check_cmd(buff);
        if (is_quit(buff)||buff[0]==0) break;
        n=atoi(mmc[1]);n2=0;
        if (atoi(mmc[0])>0) n=atoi(mmc[0]);
        if (atoi(mmc[2])>0) n2=atoi(mmc[2]);
        if (club_mode&&n>=31) continue;
        else if (n==0) {
            mess(" ���� <���> <��ȣ>  ��: E 11  ,D 33");
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
            printf("  ȭ�� ��ȣ (0-9) >>");
            lineinput(buff,2);n2=atoi(buff);printf("\n");
		}
        else if (n==14||n==33) {
            printf("  ���� ���� (�� 12��31�� -> 1231) >>");
            lineinput(buff,4);n2=atoi(buff);printf("\n");
        }
        else if (n==15||n==34) {
            printf("  ���� ���� (�� 31�� -> 31) >>");
            lineinput(buff,2);n2=atoi(buff);printf("\n");
        }
        else if (n==16||n==35) {
            printf("  ���� ���� (��:0 ��:1 ȭ:2 ��:3 ��:4 ��:5 ��:6)  (0~6) >>");
            lineinput(buff,2);n2=atoi(buff);printf("\n");
        }
        else if (n==17||n==36) {
            printf("  �ð� ���� (0~23) >>");
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
            mess("%s ȭ���� �����ϴ�.",filename);
        }
        else if (is_same(mmc[0],"d")) {
            view(filename);
            printf("\n %s ȭ���� ���� �ұ��? ����(Y) >>",filename);
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
            printf("\n ## %s ȭ���� ����մϴ�.\n",filename);
            up_text(0,filename);   //ȭ���� ���۹޾� filename�� �����Ѵ�.
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
    if (n<0||n>=30) {mess(" �׸� ���� ����:  e [�����ҹ�ȣ] ");return;}
    printf("\n  %d. ���丮 %-20s >>",n+1,adir[n]);
    get_str(adir[n],16);
    printf("\n  %d. ����     %-20s >>",n+1,atit[n]);
    get_str(atit[n],20);
}

del_abuff()
{
    int n=mmn[1]-1;
    if (n<0||n>=30) {mess(" �׸� ���� ����:  d [�����ҹ�ȣ] ");return;}
    printf("\n  %d. %-16s  %s ",n+1,adir[n],atit[n]);
    if (yesno("  �� �׸��� �����ұ��?(y/N) >>",No)) {
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
    if (n1<0||n1>=30||n2<0||n2>30) {mess(" �׸� �̵� ����:  m [�̵��ҹ�ȣ]  [�̵�����ġ]");return;}
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
                if (xfrm.mode==0) sprintf(buff,"  ����");
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
        printf("## %s������ �����ϴ�.\n",filename2);
    }
    else {
        printf(" ## %s(%d Byte)\n",filename2,file_size(filename2));
        printf(" -> %s(%d Byte)\n",filename,file_size(filename));
        if (yesno("�� ���״�� �����ұ��?(Y/n)>>",Yes)) {
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
    key=up_file(0,filename2,No);   //ȭ���� ���۹޾� filename�� �����Ѵ�.
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
    printf(" ## ȭ���� ������?(%s) (y/N)>>",filename);
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
    printf("\n ## �� ����Ÿ�� %s���� ������? (y/N)>>",datafile);
    if (yesno("",No)) {
        strcpy(afrm[nn-1].title,"");
        save_data();
    }
}

/*
123 123456789012 1234567890123456789012345678901234567890 123456 123456
  1 select.bbs   ����
*/


save_data()
{
    FILE *fp;
    int i,n;
    if ((fp=fopen(datafile,"w"))!=NULL) {
        n=1;
        printf("\n... ��� ��ٷ� �ֽʽÿ�.....\n");
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
    printf("  �� �� �� : ");
    lineinput(buff,12);printf("\n");
    if (strlen(buff)==1&&is_char("QqPpXx",buff[0])) return;
    if (strlen(buff)>1) strcpy(xfrm.filename,buff);
    printf("  ���ϼ��� : ");
    lineinput(buff,40);printf("\n");
    if (strlen(buff)==1&&is_char("QqPpXx",buff[0])) return;
    if (strlen(buff)>1) strcpy(xfrm.title,buff);
    afrm[nn-1]=xfrm;
    disp_info(nn);
    save_data();
}

//����ü�� ������ �ѱ�Ƿ� ->�����ڸ� ����ؾ���
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
    printf("  ��    ȣ : %d\n",nn);
    printf("  ��    �� : %s\n",filename);
    printf("  �� �� �� : %s\n",xfrm.filename);
    printf("  ���ϼ��� : %s\n",xfrm.title);
    printf("  ����ũ�� : %d\n",xfrm.filesize);
    printf("  ������¥ : %s\n",xfrm.mode==0?"���Ͼ���":datestr(2,xfrm.date));
}


disp_frm(int nn)
{
    char filename[300];
    if (nn<1||nn>MAX) return;
    xfrm=afrm[nn-1];
    set_filename(filename);
    if (!view(filename)) printf("\n ## %s ȭ���� �����ϴ�.\n",filename);
    else printf("\n ## ȭ���� ���Դϴ�.(%s)\n",filename);
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
        mess("%s�� ������ �����ϴ�.",datafile);
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
