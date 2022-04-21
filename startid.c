//////////////////////////////////////////////////////////////////////////////
/// 	  ¾ÆÀÌµð ÀÔ·Â ·çÆ¾
//////////////////////////////////////////////////////////////////////////////
#include "bbs.h"
//mode==0 : ´Ü¼øÀÔ·Â  1:ÀüºÎ¼Õ´ÔÀÏ¶§ ¾ÆÀÌµðÀÔ·Â
//defid-> ÁöÁ¤µÇ¸é IDÀÔ·ÂÇÑ°ÍÀ¸·Î °£ÁÖÇÔ
start_id(int mode,char *defid)
{
    char buff[300];
    char buff1[300];
    char buff2[300];
    char buff3[300];
    char buff4[300];
    char tbuff[300];
    char tbuff1[300];
    char tbuff2[300];
    char tbuff3[300];
    char tbuff4[300];
    char filename[300];
	int found=No,fix;
	int n=0,tt,i,i2,n1,n2,n3,n4,n5,n6;
	int id_not_found=No;
	struct tm *tp;
	time_t t;
	time_t today;
	FILE *fp;
    PF oldpf;
    strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");strcpy(buff4,"");
    set_tmpfile();
    set_guest_mode();
    strcpy(buff1,cfg.guestid);strcpy(buff2,cfg.guestid);strcpy(buff3,cfg.guestid);strcpy(buff4,"");
    unlink(TMP_FILE1);
    fflush(stdout);
    if (strlen(defid)>4&&defid[0]=='-'&&stristr(defid,"-l")) {
        strcpy(buff,defid);
        strchng2(buff,"-l","");
        if (read_pf2(buff)&&pf2.type<18) {
            guest_mode=No;
            pf=pf2;
            goto find_id;
        }
    }

    strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");strcpy(buff4,"");


    start_logid(mode);       //////// ¾ÆÀÌµð/ºñ¹Ð¹øÈ£ ÀÔ·Â

    strcpy(tbuff,c_han(pf.id));
    check_dir("userdata/%s/%s",tbuff,pf.id);
    check_dir("userroom/%s",tbuff);
    if (in_file2("system/testid.id",pf.id)) {
        view("frm/log/testid.log");          //TEST ID
        allmode=set_bit(allmode,3,1);     //Å×½ºÆ®¿ëÀ¸·Î ÁöÁ¤
    }
    if (in_file2("system/monich2.id",pf.id)) {
        allmode=set_bit(allmode,4,1); //°¨½ÃÁ¦¿Ü
    }
    guest_mode=No;
    //Ãß°¡ ÀÎÁõ¾ÏÈ£
    sprintf(filename,"user/epass/%s.dat",pf.id);
    if (pf.type>0&&!guest_mode&&(bit(cfg.guestmode,25)||bit(cfg.guestmode,26))&&is_file(filename)) {
        if (bit(cfg.guestmode,27)) {
            check_epass(0);
        }
        else {
            if (bit(cfg.guestmode,25)) {
                read_pf(pf.id);
                pf.type=0;
                view("frm/guest/epass_1.log");
            }
            else if (bit(cfg.guestmode,26)) {
                read_pf(pf.id);
                if (pf.type<1) pf.type=1;
                view("frm/guest/epass_2.log");
            }
        }
    }
    if (pf.type==0||(mode==0&&bit(cfg.infomode,4))) guest_mode=Yes;
    //¼Õ´ÔÀÏ°æ¿ì
    if (guest_mode||pf.type<1||pf.type>19) {
        set_guest_mode2();
        if (!bit(cfg.guestmode,17)) set_nick_input();  //º°¸íÀÔ·Â¹ÞÀ½
        if (bit(cfg.guestmode,18))       //¼Õ´Ô ¾ÆÀÌµð¸¦ ¼øÂ÷¹øÈ£·Î ÁöÁ¤ÇÔ
            pf.num=cfg.hostno*100000+inc_numfile(0,"id/guest.num")%10000;
        else
            pf.num=cfg.hostno*100000+rand()%10000;
        if (pf.num<0) pf.num*=-1;
        guest_mode=Yes;
        pf.login=START_TIME;        //Á¢¼Ó½ÃÀÛ½Ã°£
	}

    find_id:
    no_spc(pf.id);
    if (guest_mode) return;

    oldpf=pf;
    n=time_num(pf.login);  //Á¢¼ÓÇÏ±â ÀüÀÇ ³¯Â¥
    pf.login=START_TIME;        //Á¢¼Ó½ÃÀÛ½Ã°£
    n2=time_num(pf.login); //¿À´Ã ³¯Â¥
    if (n!=n2) {
        pf.today=0; //¿À´Ã Ã³À½ Á¢¼ÓÀÇ °æ¿ì
        for (i=0;i<32;i++) {
            if (bit(cfg.check_today,i)) //¿À´Ã1ÀÏ´ç..
            pf.todaymode=set_bit(cfg.check_today,i,0);
        }
    }
    if (n/100!=n2/100) {
        pf.month_log=0;  //ÀÌ¹ø´Þ µé¾î
        pf.month_time=0; //Ã³À½ Á¢¼ÓÀÇ °æ¿ì
    }
    pf.all_log++;
    pf.month_log++;
    pf_today(1,0,0);       //Á¢¼Ó¼ö Áõ°¡
	if (pf.job>18||pf.job<0)  pf.job=0; 	//Á÷¾÷
	if (pf.email[0]==0) strcpy(pf.email,pf.id);
	if (pf.type<0||pf.type>19) pf.type=0;
    del_space(pf.id);del_esc3(pf.id);
    del_space(pf.name);del_esc3(pf.name);
    del_space(pf.mailid);del_esc3(pf.mailid);
    del_space(pf.email);del_esc3(pf.email);
    //¿¬¼ÓÃâ·ÂÀÏ¼ö

    //Á¢¼Ó±â·Ï
    if (bit(cfg.infomode,0)&&bit(cfg.startmode,11)) {
        CONT_DAY=check_logday(pf.id);             //¸ÅÀÏÁ¢¼Ó±â·ÏÀúÀå
        if (pf.type<17&&n!=n2) {              //¿À´Ã Ã³À½ Á¢¼ÓÀÇ °æ¿ì
            if (file_line(tbuff,"system/contday.dat",CONT_DAY)&&tbuff[0]!=0) {
                n1=n2=n3=0;
                del_esc(tbuff);
                strcat(tbuff," 0 0 0 0 0 0 0");
                sscanf(tbuff,"%s%s%s",tbuff1,tbuff2,tbuff3);
                n1=atoi(tbuff1);
                n2=atoi(tbuff2);
                n3=atoi(tbuff3);
                pf.level+=n1;
                pf.down+=(n2*1000);
                pf.cash+=n3;
            }
            view("frm/log/contday%d.log",CONT_DAY);
        }
    }
    del_space(pf.id);del_space(pf.name);del_space(pf.nick);
    del_esc(pf.id);del_esc(pf.name);del_esc(pf.nick);
    if (strlen(pf.name)<1) strcpy(pf.name,pf.id);
    if (strlen(pf.nick)<1) strcpy(pf.nick,pf.id);
    all_trim(pf.addr);all_trim(pf.passwd);all_trim(pf.id_no);
    pf.mymode=set_bit(pf.mymode,29,0);
    pf.login=START_TIME;        //Á¢¼Ó½ÃÀÛ½Ã°£
    if (pf.type>0) {
        save_pf(35);
        if (!bit(cfg.infomode,5)) startlog();
    }
    if (!bit(cfg.startmode,13)) pressenter();
    sys_tem("rm tmp/up*%d%s -rf",cfg.hostno,no_spc(tty));
    sys_tem("rm tmp/tmp*%d%s -rf",cfg.hostno,no_spc(tty));
}


check_epass(int mode)
{
    FILE *fp;
    char buff[300];
    char buff1[300];
    char buff2[300];
    char buff3[300];
    char epass[100];
    char filename[300];
    PF oldpf;

    oldpf=pf;

    if (mode==1) {
        if (pf.type==0) {
            _cont1:
            pf=oldpf;
            view("frm/guest/epass_3.log");
            lineinput(buff1,8);
            if (strlen(buff1)<3) {pf=oldpf;return;}
            if (!read_pf(buff1)||pf.type>=18) {
                mess(" %s ID¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù.\n",buff1);
                goto _cont1;
            }
            printf("  ¾ÏÈ£>>");
            passinput(buff2,8);  //¾ÏÈ£ÀÔ·Â
            if (strcmp(pf.passwd,buff2)!=0) {
                mess(" %s ¾ÏÈ£°¡ Æ²¸³´Ï´Ù.\n",pf.id);
                goto _cont1;
            }
        }
        else view("frm/guest/epass_4.log");
    }
    if (pf.type==0||strlen(pf.id)<3) {
        mess(" È¸¿ø ID¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù.");
        pf=oldpf;
        return;
    }

    sprintf(filename,"user/epass/%s.dat",pf.id);
    strcpy(epass,"");
    if ((fp=fopen(filename,"r"))==NULL) {
        view("frm/guest/epass_9.log");
    }
    else {
        if (fgets(buff,200,fp)) {
            sscanf(buff,"%s %s",epass,buff2);
        }
        fclose(fp);
    }
    if (strlen(epass)>0) {
        if (bit(cfg.guestmode,25)) view("frm/guest/epass1.log");
        else                       view("frm/guest/epass2.log");
        lineinput(buff2,20);
        if (is_same(epass,buff2)) {
            view("frm/guest/epass3.log");
            unlink(filename);
        }
        else {
            view("frm/guest/epass4.log");
            if (bit(cfg.guestmode,25)) pf.type=0;
            else if (bit(cfg.guestmode,26)&&pf.type<1) pf.type=1;
            return;
        }
    }
    if (pf.type==0) guest_mode=Yes;
    if (mode==1) pf=oldpf;
}


set_guest_mode()
{
	int n;
	set_guest_();
}


set_guest_mode2()
{
    char buff[100];
    char bufftty[100];
	set_guest_();
    strcpy(bufftty,no_spc(tty));
    strchng2(bufftty,"tty","");
    strchng2(bufftty,"pts","t");
    sprintf(buff,"%s%s",cfg.guestid,bufftty);
    del_space(buff);del_esc(buff);
    xstrncpy(pf.id,buff,8);
    strcpy(pf.name,cfg.guestid);
    strcpy(pf.nick,cfg.guestid);
}


//¼Õ´Ô½Ã º°¸íÀÔ·Â¹ÞÀ½
set_nick_input()
{
	int n;
    char buff[100];
	if (bit(cfg.guestmode,17)==1) return;
	n=0;
	while (n++<3&&is_same(pf.nick,cfg.guestid)) {
        print_amess(25);      //printf("\n   ## ÀÓ½Ã·Î »ç¿ëÇÒ ÀÌ¸§(º°¸í)À» Àû¾îÁÖ¼¼¿ä >>");
        lineinput(buff,8);printf("\n");
        del_space(buff);del_esc(buff);
        if (strlen(buff)<1) {
            strcpy(pf.nick,pf.name);
            break;
        }
        else if (strlen(buff)<3) {
            print_amess(26);      //3ÀÚÀÌ»ó
            printf("\n");
            strcpy(pf.nick,pf.name);
        }
        //»ç¿ëÀÌ °ÅºÎµÈ ¾ÆÀÌµðÀÎ°¡?
        else if (in_file("system/notguest.txt",buff)) {
            print_amess(27);
            pressenter();
            strcpy(pf.nick,pf.name);
        }
        else {
            strcpy(pf.nick,buff);
            break;
        }
	}
    if (n>2||strlen(pf.nick)<3) strcpy(pf.nick,cfg.guestid);
}


set_guest_()
{
	strcpy(pf.name,cfg.guestid);
	strcpy(pf.nick,cfg.guestid);
	strcpy(pf.gofirst,"");
	guest_mode=Yes;
	pf.type=0;
	pf.mymode=0;
	time(&pf.login);
	time(&pf.logout);
}


print_amess(int mode)
{
	char buff[180];
	file_line(buff,"logo/_startid.mes",mode);
    printf(buff);
}



find_guestwait(char *id)
{
	FILE *fp;
	int ok=No;
	if ((fp=fopen(GUEST_FILE,"r+"))!=NULL) {
		while (fread(&pf2,sizeof_pf,1,fp)>0)
			if (is_same(pf2.id,id)) {
				ok=Yes;
				break;
			}
		fclose(fp);
	}
	return ok;
}

//¸¶Áö¸· 10°³ °Ô½Ã¹°¸¸ º¸¿©ÁØ´Ù. notice¶ó´Â °¡±âÄÚµå·Î Á¤ÇØ ÀÖ´Ù.
//È­¸éÀº notice.log¸¦ ¸ÕÀú º¸¿©ÁØ´Ù.
//mode=1ÀÌ¸é ½ÃÀÛ½Ã È£ÃâÇÑ°ÍÀÓ
view_notice(int mode,char *code)
{
	char buff[250];
	char buffdate[50];
    char inv1[100];
    char inv2[100];
	char dir[300];
    char char3;
	int anum[51];
	FILE *fp1;
    int i,n,n2,maxline,key,noti_mode;
    GO oldgo,oldclub_go;
    int oldclub_mode;

    oldgo=go;
    oldclub_go=club_go;
    oldclub_mode=club_mode;
    list_mode=No;
    if (ccfg.noticex<1) ccfg.noticex=3;
    if (ccfg.noticey<1) ccfg.noticey=6;
    if ( cfg.noticex<1)  cfg.noticex=3;
    if ( cfg.noticey<5)  cfg.noticey=6;

    if (!find_go("notice")) goto _quit;
    read_bcfg();
    if (mode==1) {
        if (!club_mode&&!guest_mode&&bit( cfg.notice,0)) goto _quit;
        if (!club_mode&& guest_mode&&bit( cfg.notice,1)) goto _quit;
        if ( club_mode&&cpf.type> 1&&bit(ccfg.notice,0)) goto _quit;
        if ( club_mode&&cpf.type<=1&&bit(ccfg.notice,1)) goto _quit;
    }
    set_stat(1,"");

    set_filename();       //ÇöÀçÇ¥½ÃÇÒ È­ÀÏ¸í
    if (!is_file(NOW_FILE)&&is_file("menu/notibase.tit")) {
        strcpy(NOW_FILE ,"menu/notibase.tit");
        strcpy(NOW_FILED,"menu/notibase.txt");
    }
    check_bbs();  //ÀÚ·áÀÇ °¹¼ö¸¦ ÆÄ¾ÇÇÏ´Â ·çÆ¾
    view_pos=0;

    while (1) {
		if (!list_mode) {
            view_pos=0;
			//°øÁö»çÇ× »ó´ÜÀÇ È­¸éÀ» º¸¿©ÁØ´Ù.
			cls();
            if (!view("%s/frm/notice/notice.scr",club_go.dir))
                view("frm/notice/notice.scr");
            if ( (!club_mode&&!bit( cfg.notice,4))||( club_mode&&!bit(ccfg.notice,4))) {
                view("%s/frm/notice/notice.dat",club_go.dir);
                view("%s/frm/notice/notice.da%d",club_go.dir,rand()%10);
            }
			xy(1,1);
            n=0;
			//È­¸é Ç¥½Ã ½ÃÀÛ
            NOW_ALLREC=count_rec(NOW_FILE,sizeof_bbs);
            fp1=fopen(NOW_FILE,"r");
            if (fp1==NULL) break;
			for (i=0;i<51;i++) anum[i]=0;

			i=NOW_ALLREC;
			n=0;
            maxline=club_mode?ccfg.noticeline:cfg.noticeline;
            if (maxline<1) maxline=10;
            while (i>0&&n<maxline) {
				fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
                if (!fread(&bbs,sizeof_bbs,1,fp1)) break;
                if (!bbs.del&&bbs.gd>' ') {
                    if (bbs.gd!='*') {
                        sprintf(inv1,"[s%s",gd_str(bbs.gd));      //ÃßÃµº°»ö»ó
                        strcpy(inv2,"[u");
                    }
                    else {strcpy(inv1,"");strcpy(inv2,"");}

                    char3=gd_chr(1);     //ÃßÃµ/Á¶È¸/´Ù¿îÁöÁ¤
                    if (bbs.gd!=' ') strcpy(buff,gd_str(bbs.gd));      //ÃßÃµº°»ö»ó
                    else             strcpy(buff,gd_str(char3));       //ÃßÃµº°»ö»ó
                    strcpy(g_buff7,buff);
                    strcpy(g_buff8,"");
                    strcpy(g_buff5,inv1);strcpy(g_buff6,inv2);

                    anum[n+1]=bbs.num;
					if (club_mode) xy(ccfg.noticex,ccfg.noticey+n);
					else		   xy(cfg.noticex,cfg.noticey+n);
                    a_num[4]=n+1;
                    n2=club_mode?ccfg.noticet:cfg.noticet;
                    if   (view("frm/notice/notice.fm%d",n2));
                    else if (n2==1) printf("%2d.  %s%-58.58s %s%5d%s\n",n+1,inv1,bbs.title,datestr(0,bbs.date),bbs.read,inv2);
                    else if (n2==2) printf("%2d.  %s%-70.70s %s\n",n+1,inv1,bbs.title,inv2);
                    else if (n2==3) printf("%2d  %s %s%-62.62s%s\n",n+1,inv1,datestr(5,bbs.date),bbs.title,inv2);
                    else            printf("%2d %s%-60.60s[m %s%5d%s\n",n+1,inv1,bbs.title,datestr(0,bbs.date),bbs.read,inv2);
					n++;
				}
				i--;
			}
            fclose(fp1);
		}
        if (!view("%s/frm/notice/notice.adc",club_go.dir)) view("frm/notice/notice.adc");
		lineinput(buff,20);printf("\n");check_han(buff);check_cmd(buff);
		n=atoi(buff);

        if (is_same(buff,"z")) continue;
        else if (strlen(mmc[0])<1) {
                 if (!list_mode) break;
            else if (list_mode) {
                bbs_read(1);
                continue;
            }
        }
        else if (n>0&&n<20) {         //ÇØ´ç¹øÈ£ ³»¿ëÃâ·Â
            if (anum[n]>0&&bbs_num(anum[n])) {
                bbs_read_num();   //ÀÐÀ½ Ã³¸®
                bbs_read(0);
			}
			continue;
		}
        else if (n>0) {         //ÇØ´ç¹øÈ£ ³»¿ëÃâ·Â
            if (bbs_num(n)) {
                bbs_read_num();   //ÀÐÀ½ Ã³¸®
                bbs_read(0);
			}
			continue;
		}
        else if (is_same(mmc[0],"no")) sprintf(auto_cmd,"go notice"); //ÀÚµ¿°¡±âÄÚµå
        else if (!guest_mode&&is_same(mmc[0],"r")) sprintf(auto_cmd,"go rmail"); //ÀÚµ¿°¡±âÄÚµå
        else if (pf.type>14&&is_same(mmc[0],"w")) {go_code("notice");oldgo=go;sprintf(auto_cmd,"w");return;} //ÀÚµ¿°¡±âÄÚµå
		else if ((mmc[0][0]==0||is_same(mmc[0],"p"))&&list_mode) {list_mode=No;continue;}
        else if (is_same(mmc[0],"p")) break;
		else if (strlen(buff)>0) strcpy(auto_cmd,buff);    //¹Ù·Î°¡±â
		break;
	}
    go_top(1);
    p_command=No;
    re_logo=Yes;

    _quit:
	list_mode=No;
	re_print=Yes;
    re_bbs=Yes;
    go=oldgo;
    club_go=oldclub_go;
    club_mode=oldclub_mode;
    set_filename();       //ÇöÀçÇ¥½ÃÇÒ È­ÀÏ¸í
    check_bbs();          //ÀÚ·áÀÇ °¹¼ö¸¦ ÆÄ¾ÇÇÏ´Â ·çÆ¾
}



/*
Á¢¼Ó½Ã logoÈ­ÀÏ
start1.log -> start9.log
start.31   -> ³¯Â¥º°
startday.0 ,1,2,3,4,5,6  ÀÏ.¿ù.È­...
starttim.0 ,1,2,3,4,5,24 ½Ã°£´ëº°
*/
start_logo()
{
    int i;
	time_t t;
	struct tm *tp1;
    if (!club_mode&&bit(cfg.logomode,1)) return;         //½ÃÀÛÈ­¸é¹«½Ã
    if (club_mode&&bit(ccfg.logomode,1)) return;         //½ÃÀÛÈ­¸é¹«½Ã
    view("%s/frm/log/start.log",club_go.dir);
    for (i=1;i<10;i++) {
        mview(No ,"%s/frm/log/start%d.log",club_go.dir,i);
        mview(Yes,"%s/frm/log/start%d.vtx",club_go.dir,i);
        mview(Yes,"%s/frm/log/start%d.jpg",club_go.dir,i);
    }
    for (i=1;i<50;i++) {
        if (view("%s/frm/log/startr%d.log",club_go.dir,rand()%10)) break;
    }
	time(&t);
	tp1 = localtime(&t);

    view("%s/frm/log/week%d.log",club_go.dir,tp1->tm_wday);
    view("%s/frm/log/day%02d%02d.log",club_go.dir,tp1->tm_mon+1,tp1->tm_mday);
    view("%s/frm/log/day%d.log",club_go.dir,tp1->tm_mday);
    view("%s/frm/log/hour%d.log",club_go.dir,tp1->tm_hour);
}



//µî±Þº° ¹Ù·Î°¡±â
auto_go_start(char *auto_cmd,int type)
{
	char buff[200];
	char buff2[200];
	if (file_line(buff,"system/auto_go.txt",type+1)) {
		strcpy(buff2,buff);
		del_space(buff2);
		if (strlen(buff2)>0) strcpy(auto_cmd,buff);
	}
}

// È¸¿øÀÎµ¦½º Á¤º¸ÀÌ´Ù.
typedef struct {
	char id[9]; 			// ¾ÆÀÌµð
	char ret;
} TID;
TID tid;
//±¸Á¶Ã¼ Å©±â¸¦ »ó¼ö ¼ö½ÄÀ¸·Î ¼±¾ð
#define sizeof_tid (sizeof(TID))

check_logday(char *id)
{
    FILE *fp;
    int i,n,count;
	time_t t;
	time(&t);
    count=1;
    if (find_logday(t,id)) return 1;  //ÀÌ¹Ì ¿À´Ã Á¢¼ÓÇÑ °æ¿ì
    add_logday(id);
    for (i=0;i<7;i++) {
        t=t-24*60*60;
        if (find_logday(t,id)) count++;
        else break;
    }
    return count;
}



find_logday(time_t t,char *id)
{
    FILE *fp;
    int i,n,first,last,now,found;
    char buff[200];
    char filename[300];
    sprintf(filename,"user/log/d%s.log",datestr(5,t));
    if ((fp=fopen(filename,"r"))==NULL) return No;
    first=1;
    fseekend(fp);
    last=ftell(fp)/sizeof_tid;
    now=last/2+1;if (now<1) now=1;
    found=No;
    while(1) {
        fseek(fp,(now-1)*sizeof_tid,SEEK_SET);
        if (!fread(&tid,sizeof_tid,1,fp)) break;
        del_esc(tid.id);del_space(tid.id);
        n=strcasecmp(tid.id,id);
        if (n==0) {
            found=Yes;
            break;
        }
        else if ((last-first<5)||now<0) break;
        else if (n>0) last=now;
        else          first=now;
        now=(last-first)/2+first+1;
    }
    i=now-10;if (i<0) i=0;
    if (found) return (found);
    for (n=i;n<now+20;n++) {
        fseek(fp,n*sizeof_tid,SEEK_SET);
        if (fread(&tid,sizeof_tid,1,fp)) {
            del_esc(tid.id);del_space(tid.id);
            if (strcasecmp(tid.id,id)==0) {
                found=Yes;
                break;
            }
        }
    }
    fclose(fp);
    return (found);
}


add_logday(char *id)
{
    FILE *fp;
    int i,n,first,last;
    char buff[200];
    char buff2[200];
    char filename[300];
	time_t t;
	time(&t);
    sprintf(filename,"user/log/d%s.log",now_str(5));
    c_lock(1,filename);
    if ((fp=fopen(filename,"r+"))==NULL) fp=fopen(filename,"w+");
    if (fp==NULL) return;
    fseekend(fp);
    last=ftell(fp)/sizeof_tid;
    fseek(fp,last*sizeof_tid,SEEK_SET);
    sprintf(tid.id,"%-8.8s",id);
    tid.ret='\n';
    fwrite(&tid,sizeof_tid,1,fp);
    fclose(fp);
    xsort(filename,TMP_FILE);
    file_move(TMP_FILE,filename);
    unlink(TMP_FILE);
    c_lock(0,filename);
}

//Á¢¼Ó ¾²±â µî·Ï
pf_today(int n1,int n2,int n3)       //Á¢¼Ó¼ö Áõ°¡
{
    int i1,i2,i3;
    i1=pf.today/1000000+n1;
    i2=(pf.today/1000)%1000+n2;
    i3=pf.today%1000+n3;
    pf.today=i1*1000000+i2*1000+i3;
}

get_pf_today(int mode)       //Á¢¼Ó¼ö Áõ°¡
{
    int i1,i2,i3;
         if (mode==1) return pf.today/1000000;          //Á¢¼Ó
    else if (mode==2) return (pf.today/1000)%1000;      //¾²±â
    else if (mode==3) return pf.today%1000;             //µî·Ï
    else              return pf.today;
}

//USÁ¤º¸ Á¤·Ä
int comp_moni(MONI *moni1,MONI *moni2)
{
	return (moni1->login-moni2->login);
}


start_logid(int mode)
{
	FILE *fp;
    char buff[180],filename[100];
    char buff1[180],buff2[180],buff3[180],buff4[180];
    int  found=No,fix,sameid1=No,old;
    int  n,nn,tt,i,i2,n1,n2,n3,n4,n5,n6;
    PF   samepf;
	char big[200];
	char dbuff1[10];
	char dbuff2[10];
	char dev1;
	time_t t;
	struct tm *tp1;
	n=nn=0;
    netmode=0;

	dev1=is_file("system/xls");

	strcpy(auto_cmd,"");
	if (strlen(cfg.guestid)<3) strcpy(cfg.guestid,"guest");
    if (mode==0&&bit(cfg.guestmode,20)) goto quit_3;      //ÀüºÎ´Ù ¼Õ´ÔÀ¸·Î Ã³¸®

	set_guest();
	//¾ÆÀÌµð ÀÔ·ÂÈ­¸é Ãâ·Â
	nn=0;
	cont_1:
	n=0;
	if (nn++>9) goto quit_2;		//¼Õ´ÔÀ¸·ÎÁ¢¼Ó
	cls();
    if (!view("frm/log/startid-%s.scr",now_str(18))) {
        view("frm/log/startid.scr");
        view("frm/log/startid.add");
        view("frm/log/startid.ad%d",rand()%10);
        time(&t);
        tp1 = localtime(&t);
        view("frm/log/week%d.add",tp1->tm_wday);
        view("frm/log/day%02d%02d.add",tp1->tm_mon+1,tp1->tm_mday);
        view("frm/log/day%d.add",tp1->tm_mday);
        view("frm/log/hour%d.add",tp1->tm_hour);
        view("frm/log/startid.cmd");
    }

	//¾ÆÀÌµðÀÔ·Â
	n=0;
	cont_2:
	if (n++>5) goto quit_2;
	pf.type=0;strcpy(pf.id,"");
	view("frm/log/id.scr");
    lineinput(buff1,15);
	del_space(buff1);check_han(buff1);del_esc3(buff1);printf("\n");
	buff1[8]=0;
	if (is_same(buff1,"q")||is_same(buff1,"qq")||
		is_same(buff1,"p")||is_same(buff1,"pp")||
		is_same(buff1,"x")||is_same(buff1,"xx")) goto quit_1;
	if (strlen(buff1)==0&&bit(cfg.guestmode,21)) goto quit_2;	 //¼Õ´ÔÀ¸·Î
	if (is_same(buff1,"r")||is_same(buff1,"rc")) goto quit_4;
	if (is_same(buff1,".lo" )) {allmode=gtt_bit(allmode,10);goto cont_2;}
	if (is_same(buff1,".lo2")) {allmode=gtt_bit(allmode,11);goto cont_2;}
	if (in_file2("frm/log/helpid.txt",buff1)) {
		view("frm/log/helpid.hlp");
		goto cont_2;
	}
    if (strlen(buff1)<3) goto cont_2;
	if (check_file_char(buff1,"frm/guest/notid.chr")) {
		view("frm/log/notid.err");
		goto cont_2;	  //±ÝÁö±ÛÀÚÀÖÀ»¶§
	}
	//¼Õ´ÔÀ¸·Î ÁöÁ¤µÈ °æ¿ì
	if (is_same(cfg.guestid,buff1)||is_same(cfg.hostname,buff1)) goto quit_2;
	if (is_same(buff1,"guest")||is_same(buff1,"¼Õ´Ô")) goto quit_2;
	if (is_same(buff1,"root")||stristr(buff1,"guest")||stristr(buff1,"¼Õ´Ô")) goto quit_2;


    //ÀÚµ¿º¯°æID
    sameid1=No;
    strcpy(samepf.id,"");
    strcpy(samepf.name,"");
    strcpy(g_buff1,"");
    strcpy(g_buff2,"");

/*
    if (check_sameid(buff1)) {
        strcpy(samepf.id,g_buff1);
        strcpy(samepf.name,g_buff2);
        view("frm/log/sameid1.log");
        strcpy(buff1,g_buff2);
        sameid1=Yes;
    }
*/

    sprintf(buff,"id/id_%u.ind",(byte)tolower(buff1[0]));
    if (!is_file(buff)) make_ind(1,buff1);


    old=cfg.pfmode;
    cfg.pfmode=set_bit(cfg.pfmode,1,1);
    found=read_pf3(buff1);
    if (!found) {
        cfg.pfmode=set_bit(cfg.pfmode,1,0);
        found=read_pf3(buff1);
    }
    cfg.pfmode=old;

    if (!found) goto cont_7;
//    mess("1 BUFF:%s Ã£±â:%d  ID:'%s PASS:'%s'",buff1,found,pf.id,pf.passwd);;;;;;
    del_space(pf.passwd);
	if (found&&(is_same(pf.passwd,"********")||in_file2("system/_nopass.id",pf.id))) {
		strcpy(buff2,pf.passwd);
		goto found_1;
	}

    //ºñ¹Ð¹øÈ£ÀÔ·Â
	cont_3:
	if (n++>10) goto cont_1;
	view("frm/log/pass.scr");
    passinput(buff2,15);
    if (is_same(buff2,"`")) strcpy(buff2,buff1);

//    mess("2 '%d'%s'%s'",found,buff2,pf.passwd);;;;;;;;;
	check_han(buff2);del_space(buff2);printf("\n");
	buff2[8]=0;

	if (is_same(buff2,"q")||is_same(buff2,"qq")||
		is_same(buff2,"p")||is_same(buff2,"pp")||
		is_same(buff2,"x")||is_same(buff2,"xx")) goto quit_1;
	if (strlen(buff1)<3) {
		view("frm/log/badpass.log");
		goto cont_3;
	}

    cont_7:
    if (!found) {
		strcpy(g_id,buff1);
			 if (find_in_file(GUEST_FILE   ,buff1)) {if (!view("frm/log/in_g.log")) printf("\n ###°¡ÀÔ´ë±âÁßÀÔ´Ï´Ù.\n");}
		else if (find_in_file(GUESTNOT_FILE,buff1)) {if (!view("frm/log/in_gnot.log")) printf("\n ###°¡ÀÔ°ÅºÎ IDÀÔ´Ï´Ù.\n");}
		else										{if (!view("frm/log/badid.log")) printf("\n ### \"%s\" ID¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù.\n",buff1);}
		goto cont_2;
	}

	if (is_same(pf.passwd,"########")) strcpy(buff2,"####");
	if (pf.type<15&&in_file2("system/passpass.txt",buff2)) strcpy(buff2,pf.passwd);
	cont_4:
	del_space(pf.passwd);
    if (strcmp(buff2,pf.passwd)!=0) {
		view("frm/log/badpass.log");
		goto cont_2;
	}

	//°ÅºÎ È¸¿øÀÇ °æ¿ì
	if (pf.type==0||in_file2("system/notlog.id",pf.id)) {
		view("frm/log/whynot.log");  //°ÅºÎÀÌÀ¯
		goto cont_1;
	}

    if (sameid1) {
        strcpy(g_buff1,samepf.id);
        strcpy(g_buff2,samepf.name);
        if (view("frm/log/sameid2.log")&&yesno("",No)) {
            sys_tem("bin/ed_pf '%s' edit_id_ok",pf.id);
        }
    }
	found_1:
	//ID »õ·Î ÀÐ¾îµéÀÓ
    if (!read_pf3(pf.id)) goto cont_1;
	if (check_idrelog())  {nn=0;n=0;goto cont_1;}	//ÀÌÁß¾ÆÀÌµðÁ¢¼Ó ±ÝÁö¿©ºÎ È®ÀÎ


	//ÀÏ¹ÝÈ¸¿øÀ¸·Î ·Î±×ÀÎ
	strcpy(big,c_han(pf.id));
	if (pf.all_log>=0&&pf.all_log<=100&&is_file("system/_first.add")) {
        check_dir("userdata/%s",big);
		n1=n2=n3=n4=n5=0;
        strcpy(buff,"");
		if (file_line(buff,"system/_first.add",pf.all_log+1)&&strlen(buff)>0) {
			strcat(buff," 0 0 0 0 0 0 0");
			sscanf(buff,"%s%s%s%s%s",mmc[1],mmc[2],mmc[3],mmc[4],mmc[5]);
			n1=atoi(mmc[1]);
			n2=atoi(mmc[2]);
			n3=atoi(mmc[3]);
			n4=atoi(mmc[4]);
			n5=atoi(mmc[5]);
			pf.level+=n1;
			pf.down+=(n2*1000);
			pf.cash+=n3;
			if (pf.type+n4<14) pf.type+=n4;
			if (pf.type>0) {
                if (n1||n2||n3||n4||n5) save_pf(2);
			}
		}
		sprintf(buff,"frm/log/first%d.log",pf.all_log);view(buff);
		sprintf(buff,"frm/log/first%d.mai",pf.all_log);
        if (is_file(buff)) {
            sprintf(buff2,"¾È³çÇÏ¼¼¿ä %sÀÔ´Ï´Ù.",cfg.hostname);
            wmail4(pf.id,buff,buff2,9); //ÇØ´ç ÆÄÀÏ ÀÚµ¿ ¸ÞÀÏ¹ß¼Û
        }
	}
	//°Ë»ö°á°ú Àû±â

	quit_0: 		//°Ë»ö¿Ï·á
	view("frm/log/endid1.log");
    if (pf.type>0&&pf.type<cfg.relog&&strlen(pf.id)>0) del_stat_id(pf.id);
    else del_stat(tty);  //»óÅÂ±â·Ï È­ÀÏ ÇØ´ç Æ÷Æ®ÀÇ Á¢¼ÓÀ» ÇØÁ¦ÇÑ´Ù.
    return;

	quit_1: 		//Á¢¼ÓÁ¾·á
	set_guest();
	view("frm/log/endid2.log");
	del_stat(tty);		 //ÇöÀçÆ÷Æ®ÀüºÎ »èÁ¦
    host_end();
    return;

	quit_2: 									//¼Õ´ÔÀ¸·Î Á¾·á
	if (bit(cfg.guestmode,13)) goto quit_1; 	//Á¾·áÃ³¸®
	set_guest();
    if (!bit(cfg.infomode,0)&&!bit(cfg.infomode,5)) {
        view("frm/log/guestlo.log");
        view("frm/log/guestlo.ad%d",rand()%10);
        view("frm/log/endid3.log");
    }
	i=0;
	if (bit(cfg.guestmode,8)&&view("frm/log/guestin.scr")) i=numinput();
	//¹Ù·Î°¡ÀÔÀÌ°Å³ª 1À» ¼±ÅÃ
	if (bit(cfg.guestmode,16)||i==1) strcpy(auto_cmd,"guest");
	else if (i==2) strcpy(auto_cmd,"guidedoc");
	else if (i==3) strcpy(auto_cmd,"guide");
	else if (i==4) strcpy(auto_cmd,"bye");
	else if (bit(cfg.guestmode,2)) strcpy(auto_cmd,"guide");   //ÀÚµ¿°¡±âÄÚµå
    else if (bit(cfg.guestmode,1)) strcpy(auto_cmd,"bye");     //ÀÚµ¿°¡±âÄÚµå
	else if (bit(cfg.guestmode,6)) strcpy(auto_cmd,"guidedoc");   //ÀÚµ¿°¡±âÄÚµå
	del_stat(tty);		 //ÇöÀçÆ÷Æ®ÀüºÎ »èÁ¦
    return;

	quit_3: 		//±×³ÉÁ¾·á(¼Õ´ÔÀ¸·Î)
	set_guest();
    view("frm/log/endid4.log");
	del_stat(tty);		 //ÇöÀçÆ÷Æ®ÀüºÎ »èÁ¦
    return;

	quit_4: 		//±×³ÉÁ¾·á(¼Õ´ÔÀ¸·Î)
    set_guest();
    view("frm/log/endid4.log");
	strcpy(auto_cmd,"rc");      //ÀçÁ¢¼Ó
	del_stat(tty);		 //ÇöÀçÆ÷Æ®ÀüºÎ »èÁ¦
    return;
}



set_guest()
{
	int i,n;
	char buff[200];
	guest_mode=Yes;
	if (bit(cfg.guestmode,18)) {	  //¼Õ´Ô ¾ÆÀÌµð¸¦ ¼øÂ÷¹øÈ£·Î ÁöÁ¤ÇÔ
		n=inc_numfile(1,"id/guest.num");
		if (n>9999999) save_numfile(1,"id/guest.num");
		sprintf(pf.id,"g%07d",n);
	}
	else {
		sprintf(buff,"%s%s",cfg.guestid,no_spc(tty));
        strchng2(buff,"tty","");strchng2(buff,"pts","t");
		del_space(buff);del_esc(buff);buff[8]=0;strcpy(pf.id,buff);
	}
	strcpy(pf.name,cfg.guestid);
	strcpy(pf.nick,cfg.guestid);
	strcpy(pf.gofirst,"");
	pf.type=0;
	pf.mymode=0;
    if (bit(cfg.guestmode,18))       //¼Õ´Ô ¾ÆÀÌµð¸¦ ¼øÂ÷¹øÈ£·Î ÁöÁ¤ÇÔ
        pf.num=cfg.hostno*100000+inc_numfile(0,"id/guest.num")%10000;
    else
        pf.num=cfg.hostno*100000+rand()%10000;
	time(&pf.login);
	time(&pf.logout);
	if (!bit(cfg.guestmode,17)) {
		n=0;
		while (n++<3&&strlen(pf.nick)<3) {
			view("frm/log/nick.scr");
			strcpy(pf.nick,"");
			lineinput(buff,8);check_han(buff);printf("\n");
			if (is_same(buff,"q")||is_same(buff,"x")) break;
			view("frm/log/nickerr.scr");
			if (strlen(buff)>=3&&!in_file("system/notguest.txt",buff)) {
				strcpy(pf.nick,buff);
				if (is_same(pf.id,pf.name)) strcpy(pf.name,buff);
			}
		}
		if (strlen(pf.nick)<3) strcpy(pf.nick,cfg.guestid);
	}
}


check_idrelog()
{
	char buff[100];
    MONI moni2;
    int n;
	if (!is_moni(pf.id)) return No; //->moni¿¡ ÀúÀå
	if (pf.type>=cfg.relog) {
		if (!bit(cfg.mode,7)) view("frm/log/samelog1.log");
		return No;
	}
	view("frm/log/samelog.log");
    moni2=moni;
	if (yesno("",No)) {
        for (n=0;n<=cfg.count_server;n++) kill_portid(n,pf.id,tty);
		view("frm/log/samelog2.log");
        del_stat(moni2.tty);       //»óÅÂ±â·Ï È­ÀÏ ÇØ´ç Æ÷Æ®ÀÇ Á¢¼ÓÀ» ÇØÁ¦ÇÑ´Ù.
	}
	return Yes;
}


check_pass3(char *tid)
{
	char buff1[300];
	char buff2[300];
	char buff3[300];
	int found=No,i=0;
	FILE *fp;
	found=Yes;
	if ((fp=fopen("system/syspass.id","r+"))==NULL) return Yes;
	while (fgets(buff2,200,fp)!=NULL) {
		check_cmd(buff2);
		del_esc(mmc[0]);del_esc(mmc[1]);
		if (is_same(mmc[0],tid)) {
			i=0;
			while (i++<5) {
				printf("\n %s pass2>>",tid);passinput(buff1,8);printf("\n");
				del_esc(buff1);del_enter(buff1);
				found=is_same(mmc[1],buff1);
				if (found) goto _found;
			}
			goto _fail;
		}
	}
	_found:
	fclose(fp);
	return Yes;
	_fail:
	fclose(fp);
	return No;

}


add_logcount()      //Á¢¼Ó±â·Ï
{
	FILE *fp;
    int i,n,num[4],total=0;
	char filename[100];
	sprintf(filename,"user/rec/count%02d.dat",now_year()%100);
    if ((fp=fopen(filename,"r+"))==NULL) fp=fopen(filename,"w+");
	if (fp==NULL) return;
	num[0]=(now_month()-1)*31*25+now_day()*25+now_hour()+1+1;
	num[1]=(now_month()-1)*31*25+now_day()*25+1;
	num[2]=(now_month()-1)*31*25+1;
	num[3]=0;
	for (i=0;i<4;i++) {
		if (num[i]<1) num[i]=0;
		fseek(fp,num[i]*sizeof(n),SEEK_SET);
		if (!fread(&n,sizeof(n),1,fp)) n=0;
        if (n<0) n=0;
		n++;
		fseek(fp,num[i]*sizeof(n),SEEK_SET);
		fwrite(&n,sizeof(n),1,fp);
        if (i==3) total=n;
	}

    if (total==0||now_hour()==6) {
        total=0;
        for (i=0;i<12;i++) {
            fseek(fp,(i*31*25+1)*sizeof(n),SEEK_SET);
            if (fread(&n,sizeof(n),1,fp)&&n>0) total+=n;
        }
        fseek(fp,0*sizeof(n),SEEK_SET);
        fwrite(&total,sizeof(n),1,fp);
    }
	fclose(fp);
}


view_countlog(int mode)
{
	time_t today;
	int i,i2,n,n2,n3,n4,yy,mm,dd;
	char str1[200];
	view("frm/log/countlog.lo1");
	if (mode>0) cfg.limit_slog=mode;
	if (cfg.limit_slog<1) cfg.limit_slog=50;
	for (i=0;i<10;i++) {
		time(&today);
		today-=i*24*60*60;
		n=time_num(today)%1000000;
		yy=n/10000;mm=(n/100)%100;dd=n%100;
		n2=calc_logcount(yy,mm,dd,-1);
		n3=n2;
		if (n3>cfg.limit_slog) n3=cfg.limit_slog;
		n4=(n3*50)/cfg.limit_slog;
		strcpy(str1,"");for (i2=0;i2<n4;i2++) strcat(str1,"*");
		printf(" %s %5u %s\n",datestr(50,today),n2,str1);
	}
	view("frm/log/countlog.lo2");
	if (!yesno2("",Yes)) return;
	for (i=0;i<24;i++) {
		time(&today);
		n=time_num(today)%1000000;
		yy=n/10000;mm=(n/100)%100;dd=n%100;
		n2=calc_logcount(yy,mm,dd,i);
		if (n2<0) n2=0;
		n3=n2;
		if (n3>cfg.limit_slog) n3=cfg.limit_slog;
		n4=(n3*50)/cfg.limit_slog;
		strcpy(str1,"");for (i2=0;i2<n4;i2++) strcat(str1,"*");
		printf(" %s %2dH%5d %s\n",datestr(0,today),i,n2,str1);
	}
	view("frm/log/countlog.lo3");
}


find_in_file(char *filename,char *tid)
{
	FILE *fp;
	int found=No;
	PF pf2;
	if (strlen(tid)<3||(fp=fopen(filename,"r"))==NULL) return No;
	while (fread(&pf2,sizeof_pf,1,fp)) {
		if (is_same(pf2.id,tid)) {
			found=Yes;
			break;
		}
	}
	fclose(fp);
	return found;
}


