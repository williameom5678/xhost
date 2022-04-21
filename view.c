////////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
// ÀÌ ¼Ò½º¸¦ º¯°æÇÏ¿© »ó¿ëÀ¸·Î »ç¿ëÇÏ½Ç¶§¿¡´Â  °³¹ßÀÚÀÇ ½ÂÀÎÀ» ¹Þ¾Æ¾ß ÇÕ´Ï´Ù.
// ³»¿ë : °¢Á¾ ±â´É ·çÆ¾
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include "chat.h"

view_level()
{
	char buff[100];
	//¿¬µ¿¾ÈµÉ¶§
	if (club_mode&&!bit(ccfg.mode,1)) view("frm/pf/viewclv.scr");
	else		   view("frm/pf/viewlv.scr");
	re_print=Yes;
}

view_ver()
{
	printf("[19;20H[=1G[=15F%s %s %s %s",VER1,VER2,MAKENUM,MAKEDATE);
	printf("[24;30H");pressenter();
}



help_cmd(char *tdir,char *tcode)
{
    char buff[100];
    int n;
    if (strlen(tdir)<1) return;
    view("frm/%s/%s.log",tdir,tcode);
	while(1) {
        view("frm/%s/%s.scr",tdir,tcode);
		lineinput(buff,10);check_han(buff);printf("\n");
		n=atoi(buff);
        if (is_same(buff,"h")) {
            view("frm/%s/%s.hlp",tdir,tcode);
            continue;
        }
        if (is_quit(buff)) break;
        if (n>0&&n<=99) {
            if (is_same(tcode,"helpuse")) {
                if (file_line(buff,"frm/help/helpuse.dat",n)) {
                    view_text(buff);
                }
            }
            else {
                view("frm/%s/%s.%d",tdir,tcode,n);
            }
		}
	}
    view("frm/%s/%s.bye",tdir,tcode);
}


view_who(char *tid)
{
	FILE *fp;
    PF nowpf;
    int i,n,found;
    char filename[100];
    MONI moni2;
    moni.mode=0;
    strcpy(moni.id,"");
    found=No;
    nowpf=pf;

    if (strlen(tid)<3) {
        if (!view("frm/us/who.cmd"))
            printf(" Ã£°í ½ÍÀº ID ¶Ç´Â ÀÌ¸§ >>");
        lineinput(tid,8);check_han(tid);printf("\n");
        if (strlen(tid)<3) return;
    }
    if (strlen(tid)<3) return;

    for (n=0;!found&&n<cfg.count_server;n++) {
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename,"r"))==NULL) continue;
        while (fread(&moni2,sizeof_moni,1,fp)) {
            if (moni2.mode<1||strlen(moni2.id)<3||strlen(moni2.tty)<1) continue;
            if (is_same(moni2.id,tid)||is_same(moni2.name,tid)) {
                moni=moni2;
                found=moni.mode;
                break;
            }
        }
        fclose(fp);
    }
    if (!found) {
        if (!read_pf4(tid)) printf("\n ## %s ´ÔÀ» Ã£À»¼ö ¾ø½À´Ï´Ù.\n",tid);
        printf("\n ## %s ´ÔÀº Á¢¼ÓÁßÀÌ ¾Æ´Õ´Ï´Ù.\n",tid);
        printf(" ## %s¿¡ Á¢¼ÓÁ¾·á ÇÏ¿´½À´Ï´Ù.\n",tid,now_str(39,pf2.logout));
        pressenter();
        return;
    }
    if (read_pf4(tid)) pf=pf2;
    view("frm/us/who.frm");
    pf=nowpf;
}




//±¸Á¶Ã¼¸¦ º¯¼ö·Î ³Ñ±â¹Ç·Î ->¿¬»êÀÚ¸¦ »ç¿ëÇØ¾ßÇÔ
int comp1(MONI *moni1,MONI *moni2) {
    return (moni1->login-moni2->login);
}

#define AMONI_MAX 1000
MONI amoni[AMONI_MAX];
int amoni_count;

view_user(int mode,int mode2,char *tid)
{
	FILE *fp;
    char buff[350],filename[100],buffstr[350],buffstr1[350],buffstr2[350];
	char cont=No;
    char not_stop=No;
    int  i,ihostno=0,n,i2,count=0,maxline,key,select_mode=No,test_mode=No;
    int  mode3;
    time_t t;
    MONI moni2;
    PF nowpf;
    nowpf=pf;
    amoni_count=0;
    time(&t);
    for(i=0;i<AMONI_MAX;i++) {strcpy(amoni[i].id,"");amoni[i].mode=0;amoni[i].login=t+10000;}

    allmode2=set_bit(allmode2,1,0);
    if (strlen(tid)>2&&!is_moni(tid)) {
        printf("\n ## %s ´ÔÀº Á¢¼ÓÁßÀÌ ¾Æ´Õ´Ï´Ù.\n",tid);
        return;
    }
    if (mode==1&&cfg.usdisp>0) mode=cfg.usdisp;
		 if (is_same(go.code,"chat")) mode3=2;
	else if (club_mode) 			  mode3=1;
	else							  mode3=0;

    if (mode>9980) select_mode=Yes;
    mode%=10;
    if (mode2==8) {mode2=1;test_mode=Yes;}
    if (mode2==9) {mode2=1;not_stop=Yes;}
	if (mode3==0&&bit(cfg.usermode,1)) return;
	if (mode3==1&&bit(cfg.usermode,3)) return;
	if (mode3==2&&bit(cfg.usermode,3)) return;
	if (nowpf.type<16&&mode>=8) mode=1; //	µî±Þ16¹Ì¸¸ÀÌ mode18,19»ç¿ë
    if (cfg.usdisp>=30) mode=cfg.usdisp%10;
	if (mode<1) mode=1;

	//Ãâ·Â¾ç½ÄÀÐ¾î¿À±â
    strcpy(buffstr,"");strcpy(buffstr1,"");strcpy(buffstr2,"");
    sprintf(buff,"frm/us/us%d.frm",mode);
	if (!is_file(buff)) sprintf(buff,"frm/us/us.frm");
	file2buff(buffstr,buff,300);
    if ( mode==4||mode==5||mode==6 ) {
        sprintf(buff,"frm/us/us%d-1.frm",mode);
        file2buff(buffstr1,buff,300);
        sprintf(buff,"frm/us/us%d-2.frm",mode);
        file2buff(buffstr2,buff,300);
    }

	maxline=20;
	if (mode>2&&mode<8) maxline=10;

    view("frm/us/us.log");
    if (!view("frm/us/us%d.top",mode)) view("frm/us/us.top");
    i=atoi(tid);
    if (i>0&&i<99) {ihostno=i;strcpy(tid,"");}
    amoni_count=0;
    for (n=0;n<=cfg.count_server;n++) {
        if (ihostno>0&&ihostno!=n) continue;
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename, "r"))==NULL) continue;
        while (amoni_count<AMONI_MAX) {
            if (!fread(&moni2,sizeof_moni,1,fp)) break;
            if (test_mode) goto _cont2;
            if (moni2.mode<1||strlen(moni2.id)<3||strlen(moni2.tty)<1) continue;
            if (strlen(tid)>0&&!is_same(moni2.id,tid)) continue;
            if (mode!=8&&moni2.mode==99) continue;
            if (is_same(moni2.id,"Á¢¼ÓÁß")) continue;
            _cont2:
            amoni[amoni_count]=moni2;
            amoni[amoni_count].hostno=n;
            amoni_count++;
        }
        fclose(fp);
    }
    if (!test_mode) {
        qsort(amoni,AMONI_MAX,sizeof(amoni[0]),comp1);
    }

    for (i=0;i<amoni_count;i++) {
        moni=amoni[i];
        if (moni.mode<1||strlen(moni.id)<3||strlen(moni.tty)<1) continue;
        if (moni.type<1) clear_pf();
        else {
            if (moni.pfnum>0) pf.num=moni.pfnum;
            if (bit(allmode2,2)) {
                strcpy(pf.id,moni.id);
                strcpy(pf.name,moni.name);
            }
            else if (!read_pf3(moni.id)) clear_pf();
        }
        if ((i+1)<=99)  sprintf(g_buff4,"%2d.",i+1);
        else            sprintf(g_buff4,"%3d",i+1);
        sprintf(g_buff3,"%2d",i+1);
        sprintf(g_buff1,"%3d",i+1);
        if (mode==4) {
            if (i%4==0)       view3(buffstr1);  //Ã¹Ä­
            else if (i%4==3)  view3(buffstr2);  //¸¶Áö¸·
            else              view3(buffstr);   //°¡¿îµ¥
        }
        else if (mode==5) {
            if (i%5==0)       view3(buffstr1);  //Ã¹Ä­
            else if (i%5==4)  view3(buffstr2);  //¸¶Áö¸·
            else              view3(buffstr);   //°¡¿îµ¥
        }
        else if (mode==6) {
            if (i%3==0)       view3(buffstr1);  //Ã¹Ä­
            else if (i%3==2)  view3(buffstr2);  //¸¶Áö¸·
            else              view3(buffstr);   //°¡¿îµ¥
        }
        else {
            view3(buffstr);     //Ãâ·Â¾ç½ÄÃâ·Â
        }
        key=No;
        if (!not_stop&&!cont) {
            if (mode==4) {
                if (i%80==79) key=Yes;
            }
            else if (mode==5) {
                if (i%100==99) key=Yes;
            }
            else if (mode==6) {
                if (i%60==59) key=Yes;
            }
            else if (i%maxline==(maxline-1)) key=Yes;
            if (key) {
                if (!view("frm/us/us%d.pag",mode)) view("frm/us/us.pag");
                lineinput(buff,8);check_han(buff);printf("[80D[2K");
                if (is_same(buff,"q")||is_same(buff,"p")||
                    is_same(buff,"n")||is_same(buff,"x") ) {i=0;break;}
                else if (is_same(buff,"s")) cont=Yes;
                else if (strlen(buff)>0) {
                    strcpy(auto_cmd,buff);
                    break;
                }
            }
		}
	}
    allmode2=set_bit(allmode2,1,0);
    pf=nowpf;
//allmode=set_bit(allmode,11,1);
    if (mode==4&&i>0&&i%4!=0) view("frm/us/us%d-%d.end",mode,i%4);
    if (mode==5&&i>0&&i%5!=0) view("frm/us/us%d-%d.end",mode,i%5);
    if (mode==6&&i>0&&i%3!=0) view("frm/us/us%d-%d.end",mode,i%3);
    if (!view("frm/us/us%d.end",mode)) view("frm/us/us.end");
    if (select_mode) return;
    view("frm/us/us.bye");
}



//ÇØ´çID ¼±ÅÃÇÑµÚ MONI¿¡ ÀúÀå
sel_user(char *filename,int mode)
{
	FILE *fp;
    char buff[350],buff2[100],buffstr[350];
    char tid[30];
    int  i,n,i2;
	int  found=No;

    mode%=10;
    strcpy(auto_cmd,"");
    view_user(mode+9990,0,""); //¼±ÅÃ¸ðµå
    mode%=10;
	//¼±ÅÃ
	_select:
    strcpy(tid,"");strcpy(buff,"");
    if (!view(filename)) printf(" ¹øÈ£ ¶Ç´Â ID¸¦ ÀÔ·ÂÇØÁÖ¼¼¿ä >>");
    if (strlen(auto_cmd)>0) {strcpy(buff,auto_cmd);printf(auto_cmd);}
    else {
        sprintf(buff2,"tmp/tmp7%d%s",cfg.hostno,no_spc(tty));
        fget_str(buff,buff2,10);
        dosinput(buff,8);
    }
    check_han(buff);printf("\n");
    strcpy(tid,buff);
    strcpy(moni.id,"");
    strcpy(g_id,"");strcpy(g_name,"");

    found=No;
    _cont2:
    n=atoi(tid);
    if (is_same(tid,"q")||is_same(tid,"p")||is_same(tid,"/q"))  return No;
    if (is_same(tid,"x")||is_same(tid,"xx")||is_same(tid,"/x")) return No;
    if (strlen(tid)<1||(n==0&&strlen(tid)==1)) goto _select;

    if (strlen(tid)>=3) {
        for (i=0;i<amoni_count;i++) {
            if (!is_same(amoni[i].id,tid)) continue;
            moni=amoni[i];
            found=Yes;
            break;
        }
    }
    if (!found&&n>0&&n<=amoni_count) {
        if (strlen(amoni[n-1].id)<3) goto _select;
        moni=amoni[n-1];
        found=Yes;
    }
    if (!found) {
        moni.mode=0;
        if (read_pf4(tid)) {
            strcpy(moni.id,pf2.id);
            strcpy(moni.name,pf2.name);
            strcpy(g_id,pf2.id);
            strcpy(g_name,pf2.name);
            found=Yes;
        }
        else {
            strcpy(moni.id,tid);
            strcpy(g_id,tid);
            view("frm/us/sus.err");
        }
		if (strlen(tid)<3) strcpy(tid,"");
	}
    strcpy(auto_cmd,"");
    sprintf(buff,"tmp/tmp7%d%s",cfg.hostno,no_spc(tty));
    fput_str(moni.id,buff,10);
    return found;
}


sel_user2()
{
	FILE *fp;
    char buff[350],tbuff[300],buff2[100],filename[150];
    int  i,n,i2;
    MONI moni2;
    time_t t;

    time(&t);
    for(i=0;i<AMONI_MAX;i++) {strcpy(amoni[i].id,"");amoni[i].mode=0;amoni[i].login=t+10000;}
    strcpy(auto_cmd,"");
    if (!view("frm/say/select2.cm1")) printf(" ¾ÆÀÌµð>>");
    sprintf(filename,"tmp/tmp7%d%s",cfg.hostno,no_spc(tty));
    fget_str(tbuff,filename,10);
    if (is_esc(tbuff)||strlen(tbuff)>8||strlen(tbuff)<3) strcpy(tbuff,"");
    dosinput(tbuff,8);check_han(tbuff);printf("\n");
    if (strlen(tbuff)>=3&&is_moni(tbuff)) goto _ok;
    if (is_quit(tbuff)) goto _fail;

    view("frm/say/select2.top");
    amoni_count=0;
    for (n=0;n<=cfg.count_server;n++) {
        sprintf(buff,"user/moni/moni%d.dat",n);
        if ((fp=fopen(buff, "r"))==NULL) continue;
        while (amoni_count<AMONI_MAX) {
            if (!fread(&moni2,sizeof_moni,1,fp)) break;
            if (moni2.mode==99||moni2.mode<1||strlen(moni2.id)<3||strlen(moni2.tty)<1) continue;
            if (is_same(moni2.id,"Á¢¼ÓÁß")) continue;
            amoni[amoni_count].hostno=n;
            amoni[amoni_count]=moni2;
            amoni_count++;
        }
        fclose(fp);
    }
    qsort(amoni,AMONI_MAX,sizeof(amoni[0]),comp1);

    for (i=0;i<amoni_count;i++) {
        if (amoni[i].mode==3||amoni[i].mode==5||amoni[i].mode==6||amoni[i].mode==8||
            amoni[i].mode==14||amoni[i].mode==3||amoni[i].mode==4||
            amoni[i].mode==15||amoni[i].mode==16||amoni[i].mode==19) {
            printf(" %3d. %-8s[%-8s]",i+1,amoni[i].id,amoni[i].name);
        }
        else {
            printf(" %3d. %-8s(%-8s)",i+1,amoni[i].id,amoni[i].name);
        }
        if (i%3==2) printf("\n");
    }
    if (i>0&&i%3!=0) printf("\n");
    if (!view("frm/say/select2.cm2"))
        printf(" ¼±ÅÃ ¹øÈ£ ¶Ç´Â ¾ÆÀÌµð>>");
    dosinput(tbuff,8);check_han(tbuff);printf("\n");
    i=atoi(tbuff);
    if (i==0&&strlen(tbuff)<3) goto _fail;
    if (i>0&&i<=amoni_count) {
        moni=amoni[i-1];
        goto _ok;
    }
    for (i=0;i<amoni_count;i++) {
        if (is_same(amoni[i].id,tbuff)) {
            moni=amoni[i];
            goto _ok;
        }
    }
    _fail:
    strcpy(moni.id,"");strcpy(moni.name,"");
    return No;
    _ok:
    sprintf(buff,"tmp/tmp7%d%s",cfg.hostno,no_spc(tty));
    fput_str(moni.id,buff,10);
    return Yes;
}

view_help(int mode)
{
	pf.todaymode=set_bit(pf.todaymode,7,1);   //¿À´ÃÀÛ¾÷
	if (bit(cfg.mode,12)||mode==2) {
		sys_tem("bin/xhelp _help '%s'",pf.id);
		re_print=Yes;
	}
	else if (mode==0) { 	//ÀüÃ¼È­¸éÀ¸·Î º¸¿©ÁÜ
		if		(view("%s/logo/%s.hlp",club_go.dir,go.code));
		else if (club_mode) 			  view("frm/help/hclub.hlp");
		else if (is_same(go.code,"top"))  view("frm/help/htop.hlp" );
		else if (stristr(go.code,"mail")) view("frm/help/hmail.hlp");
		else if (go.type=='m')            view("frm/help/hmenu.hlp");
		else if (go.type=='l')            view("frm/help/hbbs.hlp" );
		else if (go.type=='f')            view("frm/help/hpds.hlp" );
		else							  view("frm/help/hmenu.hlp");
		re_print=Yes;
	}
	else {	   //ºÎºÐ¸¸ º¸¿©ÁÜ (¸í·É¾î¸¸..)
		if		(view("%s/logo/%s.hl2",club_go.dir,go.code));
		else if (is_same(go.code,"top")) view("frm/help/ctop.hlp");
		else if (stristr(go.code,"mail")) view("frm/help/cmail.hlp");
		else if (go.type=='m') view("frm/help/cmenu.hlp");
		else if (go.type=='l') view("frm/help/cbbs.hlp");
		else if (go.type=='f') view("frm/help/cpds.hlp");
		else				   view("frm/help/cmenu.hlp");
	}
}


view_pfh(char *tidstr)
{
    int i,n;
    char buff[100],tid[10];
    char a_file[10][100];
    PF oldpf;
    oldpf=pf;
    strcpy(tid,tidstr);
	if (!read_pf4(tid)) goto _fail;
	if (strcmp(tid,pf2.id)!=0) {
		if (check_sameid(tid)) {
			mess("%s ´ÔÀº %s ID·Î º¯°æµÇ¾ú½À´Ï´Ù",tid,g_buff2);
			strcpy(tid,g_buff2);
			if (!read_pf4(tid)) goto _fail;
			goto _cont1;
		}
	}

	_cont1:
	pf=pf2;
	for (i=0;i<23;i++) printf("\n");
    viewpf1();
    sprintf(a_file[0],"userdata/%s/%s/pfinfo.scr",c_han(tid),tid);
    sprintf(a_file[1],"userdata/%s/%s/pfinfo.jpg",c_han(tid),tid);
    sprintf(a_file[2],"userdata/%s/%s/pfinfo.vtx",c_han(tid),tid);
    sprintf(a_file[3],"userdata/%s/%s/pfinfo.wav",c_han(tid),tid);
    a_num[0]=file_size(a_file[0]);
    a_num[1]=file_size(a_file[1]);
    a_num[2]=file_size(a_file[2]);
    a_num[3]=file_size(a_file[3]);
    for (i=1;i<4;i++) {
        if (a_num[i]==0);
        else if (a_num[i]<1024) a_num[i]=1;
        else a_num[i]/=1024;
    }
    if (!view("frm/say/pfsay.cmd")) {
        printf(" 1.´ëÈ­(SAY )  2.¸Þ¸ð(WMEMO)  3.ÂÊÁö(MM  )  4.ÆíÁö(WMAIL)  5.±â´Ù¸²(WAIT)\n");
        printf(" 6.¼Ò°³(%3dB)  7.»çÁø(%4dK)  8.VTX (%3dK)  9.À½¼º(%4dK)\n",a_num[0],a_num[1],a_num[2],a_num[3]);
        printf(" ¿øÇÏ´Â ¹øÈ£¸¦ ¼±ÅÃÇÏ¼¼¿ä. (ENTER:º¹±Í) >> ");
    }
    pf=oldpf;
    strcpy(mmc[1],tid);
    lineinput(buff,10);printf("\n");
    n=atoi(buff);
         if (n==1) to_say(0,tid,""); //±Ó¼Ó¸» º¸³»±â
    else if (n==2) write_smemo(tid);   //¿î¿µÁø ¼ö½Å¸Þ¸ð
    else if (n==3) to_memo(0,tid,"");
    else if (n==4) wmail(1,tid);
    else if (n==5) check_wait(1);
    else if (n==6) {mview2(0,a_file[0],tid);pressenter();}
    else if (n==7) {mview2(0,a_file[1],tid);pressenter();}
    else if (n==8) {mview2(0,a_file[2],tid);pressenter();}
    else if (n==9) {mview2(0,a_file[3],tid);pressenter();}
    if (n>=6&&n<=9) goto _cont1;
    return;

	_fail:
	pf=oldpf;
	mess(" %s È¸¿øÀ» Ã£À»¼ö ¾ø½À´Ï´Ù.",tid);return;
	return;

}


mview2(int mode,char *file,char *tid)
{
    char filename[300],filename1[300],newdir[300];
    strcpy(filename,file);
	if (!is_file(filename)) return;
    if (stristr(filename,".jpg")||stristr(filename,".wav")) {
        sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
        check_dir(newdir);
        if (stristr(filename,".jpg")) sprintf(filename1,"%s/%s.jpg",newdir,tid);
        else sprintf(filename1,"%s/%s.wav",newdir,tid);
        file_copy(filename,filename1);
        mview(mode,filename);
        unlink(filename1);
    }
    else if (stristr(filename,".vtx")) view_vtx(filename,mode);
    else if (stristr(filename,".ra"))  view_ra(filename,mode);
    else if (stristr(filename,".rm"))  view_ra(filename,mode);
    else if (stristr(filename,".mp2")) view_ra(filename,mode);
    else if (stristr(filename,".mp3")) view_ra(filename,mode);
    else view(filename);
}


