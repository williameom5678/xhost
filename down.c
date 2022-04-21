///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
//	  °Ô½ÃÆÇ °ü·Ã ·çÆ¾
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include <unistd.h>    //µð·ºÅä¸® È­ÀÏ¸í º¯È¯ÇÔ¼ö¶§¹®¿¡ ÇÊ¿ä
#include <dirent.h>    //µð·ºÅä¸® È­ÀÏ¸í º¯È¯ÇÔ¼ö¶§¹®¿¡ ÇÊ¿ä

#ifndef CD
// ½Ãµð·Ò È­ÀÏ ¸ñ·Ï
typedef struct {
	int num;				// ÀÏ·Ã ¹øÈ£
	char dir[150];			// µð·ºÅä¸®
	char filename[50];		// È­ÀÏ¸í
	int  filesize;			// È­ÀÏÀÇ Å©±â
} CD;
#endif

BBS abbs[102];


//°Ô½ÃÆÇ ´Ù¿î		mode==2:SEE
bbs_down(int mode)
{
	FILE *fp,*fp2;
	char buff[200];
	char buff2[200];
	char newdir[100];
	char filename[200];
	char filename2[200];
	char ch;
	BBS oldbbs;
	int old_level=pf.level;
	int old_down=pf.down;
	int i,n,i2,key,count,ret,tsize;
	int tNOW_NO,tNOW_REC,tNOW_ALLREC,tNOW_TOPREC;
	tNOW_NO=NOW_NO;tNOW_REC=NOW_REC;tNOW_TOPREC=NOW_TOPREC;tNOW_ALLREC=NOW_ALLREC;

	oldbbs=bbs;

    if (bit(bcfg.modelist,21)&&!bbs_man(pf.id)) {
        mess("´Ù¿î¸í·ÉÀÌ ±ÝÁöµÇ¾ú½À´Ï´Ù.");
        return No;
    }

    re_print=Yes;
    if (bit(limit_mode,3)) {
        if (!view("frm/pf/notdown.log")) mess("´Ù¿î±ÝÁö");
        return No;
    }
    _last2=4;;;;;;;;;;;;;;;;;;;;;;;;

	sprintf(buff,"%s/logo/%s.ndn",club_go.dir,go.code);
	if (is_file(buff)) {view(buff);goto _fail;}
    sprintf(buff,"%s/logo/all.ndn",club_go.dir);
	if (is_file(buff)) {view(buff);goto _fail;}

	sprintf(filename,"%s/logo/%s.lir",club_go.dir,go.code);

	if (!is_char("gy",go.type)&&ss_check(3))   goto _fail;   //´Ù¿î ±ÝÁö
	if (!is_char("gy",go.type)&&check_warn(3)) goto _fail;  //°æ°í½Ã ´Ù¿î±ÝÁö

	if (is_same(go.code,"rmail")||is_same(go.code,"mbox")||is_same(go.code,"cmail")) {
        set_stat(8,bbs.title);     //´Ù¿îÁßÀÓ
		mail_down();
        set_stat(1,"");     //´Ù¿îÁßÀÓ
		return Yes;
	}
	else if (bit(bcfg.modelist,5)&&!in_file2(filename,pf.id)) return No; //ÁöÁ¤µÈ ID¸¸ °¡´ÉÇÑ°¡?
	else if (is_same(go.code,"bbs_all"));
	else if (is_typepds) return pds_down(mode);  //ÀÚ·á½ÇÀÌ¸é ÆÄÀÏ´Ù¿îÀ¸·Î..


	key=No;  //ÀÎÅÍ³Ý¶óÀÎ

	view("frm/down/bbsdown.log");

	//È­¸é Ãâ·Â¸ðµåÀÏ¶§ dn¸¸ ´©¸§
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	if (mmn[1]==0) {
		_input2:
		view("frm/down/nonumbbs.log");
		lineinput(buff,160);check_han(buff);printf("\n");
		check_cmd(buff);
		if (mmn[1]==0) {
			view("frm/down/nonumbbs.err");
			if (yesno2("",Yes)==No) goto _input2;
			goto _fail;
		}
	}
	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));check_dir(newdir);

	//¹ÞÀ» ÆÄÀÏ¸í
	if (bbsdownnum<0) bbsdownnum+=1000;
	sprintf(filename,"%s.%d",pf.id,bbsdownnum%1000);
	sprintf(filename2,"%s/%s",newdir,filename);
	strcpy(g_buff1,filename);
	view("frm/down/filename.log");
	lineinput(buff,12);printf("\n");
	if (strlen(buff)>0) strcpy(filename,buff);
	del_space(filename);del_esc3(filename);
	sprintf(filename2,"%s/%s",newdir,filename);

	count=0;  //´Ù¿î¹ÞÀ» È­ÀÏ¼ö
	tsize=0;  //´Ù¿î¹Þ´Â ¿ë·®
	count=0;
	if ((fp=fopen(filename2,"w+"))==NULL) goto _fail;
	if ((fp2=fopen(NOW_FILED,"r"))==NULL) {
		touch_file(NOW_FILED);
		if ((fp2=fopen(NOW_FILED,"r"))==NULL) goto _fail;
	}
	for (i=1;i<100;i++) {
		if (mmn[i]==0||!bbs_num(mmn[i])||bbs.del) continue;
		if (mode==2&&!stristr(bbs.filename,".jpg")) continue;
		//¾ÏÈ£°¡ ÁöÁ¤µÇ¾î ÀÖÀ»¶§
        if (!bit(bcfg.mode,31)&&strlen(bbs.passwd)>0&&!check_pass(bbs.passwd,4,"ÀÌ ÀÚ·á´Â ¾ÏÈ£°¡ ÁöÁ¤µÇ¾î ÀÖ½À´Ï´Ù.")) continue;
		printf(".");fflush(stdout);
		fprintf(fp,"¹ø  È£ : %d\n",bbs.num);
		fprintf(fp,"°Ô½ÃÀÚ : %-8s (%-8s)\n",bbs.id,bbs.name);
		fprintf(fp,"µî·ÏÀÏ : %s\n",datestr(8,bbs.date));
		fprintf(fp,"Á¦  ¸ñ : %s\n",bbs.title);
		fprintf(fp,"\n");
		fseek(fp2,bbs.posi,SEEK_SET);
		for (n=0;n<bbs.size&&!feof(fp2);n++) {
		   ch=fgetc(fp2);
		   if (ch!='\0') fputc(ch,fp);
		}
		fprintf(fp,"\n");
		bbs_read_num(); 			  //ÀÐÀº È½¼öÁõ°¡
		add_read_numn(bbs.num); 	  //ÀÐÀ½ Ç¥½Ã Ãß°¡
		count++;
	}
	printf("\n");
	fclose(fp2);
	fclose(fp);
	if (count<1) {
		printf(" ÇØ´çµÇ´Â ÀÚ·á°¡ ¾ø½À´Ï´Ù\n");
		goto _fail;
	}
	tsize=file_size(filename2);

	//´Ù¿îºÒ°¡´É Á¶°Ç
	ret=No;
	if (guest_mode&&bit(cfg.downmode,6)==1)   pf.down=100000000;
		 if (is_char("gy",go.type))           ret=Yes;
	else if (bit(cfg.infomode,3))			  ret=Yes;
	else if (count==0)						  printf("\n### ÇØ´çµÇ´Â È­ÀÏÀÌ ¾ø½À´Ï´Ù.");
	else if (club_mode&&pf.type>14) 		  ret=Yes;	//µ¿È£È¸ÀÇ °æ¿ì ¿î¿µÁø ÀÌ»ó ¹«½Ã
	else if (bcfg.fread>0&&pf.type>=bcfg.fread) ret=Yes;  //ÀÚÀ¯´Ù¿îµî±Þ
	else if (bcfg.fread==99)				  ret=Yes;	//ÀÚÀ¯´Ù¿îµî±Þ
	else if (check_type('i'))                 ret=No; //¸Þ´º °¡´É µî±ÞÈ®ÀÎ
	else if (check_type('r'))                 ret=No; //¸Þ´º °¡´É µî±ÞÈ®ÀÎ
	else ret=Yes;
	if (!ret) goto _fail;

	set_stat(8,bbs.title);	   //´Ù¿îÁßÀÓ
	set_rec_(1,8,tsize,filename2);
	sz_file(filename2);
	bbsdownnum++;
	goto _end;

	_fail:
	view("frm/down/downfaib.log");
	bbs=oldbbs;
	ret=No;
	goto _end;

	_end:
	bbs=oldbbs;
	bbs_num(bbs.num);
	NOW_NO=tNOW_NO;NOW_REC=tNOW_REC;NOW_TOPREC=tNOW_TOPREC;NOW_ALLREC=tNOW_ALLREC;
	rm_tmpup();
	return (ret);
}




//ÀÚ·á½Ç´Ù¿î
//multi_down ÀÌÀúÁ¤ µÇ¸é ¸ÖÆ¼´Ù¿î °¡´É
pds_down(int mode)
{
	FILE *fp;
	char buff[200];
	char buff2[200];
	char TNOW_FILE[300];
	char TNOW_FILED[300];
	char newdir[100];
	char adnfile[102][151];
	char filename[200];
	char newfile[200];
	BBS oldbbs,bbs2;
	GO	oldgo,oldgo2,old_club;
	int old_clubmode,count;
	int old_level=pf.level;
	int old_down=pf.down;
	int i,n,i2,key,ret,tsize;
	int from_userroom=No;	//»ç¿ëÀÚ¿µ¿ªÀÏ¶§
	int to_userroom=No;   //»ç¿ëÀÚ¿µ¿ªÀ¸·Î Àü¼Û
	int mult_down=No;	//´ÙÁß´Ù¿îÀÏ¶§
	int rz_mode=0,fail=No;
	time_t old_t,now_t;
	int tNOW_NO,tNOW_REC,tNOW_ALLREC,tNOW_TOPREC;
	tNOW_NO=NOW_NO;tNOW_REC=NOW_REC;tNOW_TOPREC=NOW_TOPREC;tNOW_ALLREC=NOW_ALLREC;

    if (bit(limit_mode,3)) {
        if (!view("frm/pf/notdown.log")) mess("´Ù¿î±ÝÁö");
        return No;
    }
	oldgo=go;
	oldbbs=bbs;
	strcpy(TNOW_FILE ,NOW_FILE);
	strcpy(TNOW_FILED,NOW_FILED);

	if (!is_char("gy",go.type)&&ss_check(3))   goto _fail;   //´Ù¿î ±ÝÁö
	if (!is_char("gy",go.type)&&check_warn(3)) goto _fail;  //°æ°í½Ã ´Ù¿î±ÝÁö

    if (pf.type<18&&!club_mode&&view("frm/down/nodnmain.log")) goto _fail;
    if (pf.type<18&& club_mode&&view("frm/down/nodnclub.log")) goto _fail;

	ret=No;
	re_print=Yes;
	key=No; 													 //ÀÎÅÍ³Ý¶óÀÎ

	sprintf(buff,"%s/logo/%s.ndn",club_go.dir,go.code);
	if (is_file(buff)) {view(buff);goto _fail;}
    sprintf(buff,"%s/logo/all.ndn",club_go.dir);
	if (is_file(buff)) {view(buff);goto _fail;}

	view("%s/logo/%s.dow",club_go.dir,go.code);
	if (!view("%s/frm/down/down.log",club_go.dir)) view("frm/down/down.log");

	//»ç¿ëÀÚ¿µ¿ªÀÏ¶§
	from_userroom=No;	 //»ç¿ëÀÚ¿µ¿ªÀ¸·Î Àü¼Û
	if (is_same(go.code,"userroom")) from_userroom=Yes;
	//È­¸é Ãâ·Â¸ðµåÀÏ¶§ dn¸¸ ´©¸§
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;

	if (mmn[1]==0) {
		_input2:
		view("frm/down/nonum.log");
		lineinput(buff,160);
		check_cmd(buff);
		if (mmn[1]==0) {
			view("frm/down/nonum.err");
			if (yesno("",Yes)==No) goto _input2;
			goto _fail;
		}
	}

	count=0;  //´Ù¿î¹ÞÀ» È­ÀÏ¼ö
	tsize=0;  //´Ù¿î¹Þ´Â ¿ë·®
    to_userroom=No;   //»ç¿ëÀÚ¿µ¿ªÀ¸·Î Àü¼Û
    mult_down=Yes;    //´ÙÁß´Ù¿îÀÏ¶§


	del_space(go.dir);if (strlen(go.dir)==0) strcpy(go.dir,".");
	del_space(club_go.dir);if (strlen(club_go.dir)==0) strcpy(club_go.dir,".");

	view("frm/down/title.log");
	for (i=0;i<100;i++) {
		abbs[i].num=abbs[i].filesize=-1;
		strcpy(abbs[i].filename,"");
		strcpy(adnfile[i],"");
	}
	count=0;
	for (i=0;i<100;i++) {
		if (mmn[i]==0||!bbs_num(mmn[i])||bbs.del) continue;
		if (mode==2&&!stristr(bbs.filename,".jpg")) continue;
		//¾ÏÈ£°¡ ÁöÁ¤µÇ¾î ÀÖÀ»¶§
        if (!bit(bcfg.mode,31)&&strlen(bbs.passwd)>0&&
            !check_pass(bbs.passwd,4,"ÀÌ ÀÚ·á´Â ¾ÏÈ£°¡ ÁöÁ¤µÇ¾î ÀÖ½À´Ï´Ù.")) continue;
		strcpy(filename,"");
             if (is_same(go.code,"userroom")) sprintf(filename,"userroom/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
        else if (is_same(go.code,"dncart")) make_code_filename(filename,bbs.key,bbs.tcode,bbs.filename);
        else if (go.type=='z') make_code_filename(filename,bbs.key,bbs.tcode,bbs.filename);
		else if (go.type=='c') get_cdrom(filename,bbs.num);
		else sprintf(filename,"%s/%s",go.dir,bbs.filename);
		del_space(filename);del_esc3(filename);

		if (strlen(filename)<1) 	 printf("      %d.¹ø ->ºó ÆÄÀÏ\n",mmn[i]);
		else if (!is_file(filename)) printf("      %d.¹ø ->¾ø´Â ÆÄÀÏ(%s)\n",mmn[i],filename);
		else {
			abbs[count]=bbs;
			strcpy(adnfile[count],filename);
			sprintf(g_buff1,"%3d",count+1);
			sprintf(g_buff2,adnfile[count]);
			if (mode!=2)
			view("frm/down/downlist.log");
			tsize+=bbs.filesize;
			uall.down++;
			uall.downsize+=bbs.filesize;
			count++;
			if (pf.type<18&&(cfg.dn_mult_limit>0||bcfg.dn_mult_limit>0)) {
				if ( (cfg.dn_mult_limit>0&&count>cfg.dn_mult_limit)||
					 (bcfg.dn_mult_limit>0&&count>bcfg.dn_mult_limit) ) {
					view("frm/down/dn_mult.log");
					break;
				}
			}
		}
	}

	if (count<1) goto _fail;
	if (count>1) {
		sprintf(g_buff1,"%3d",count);
		sprintf(g_buff2,"%10d",tsize);
		if (mode!=2)
		view("frm/down/downsum.log");
	}
    if (read_pf4(pf.id)) pf.down=pf2.down;

	//´Ù¿îºÒ°¡´É Á¶°Ç
	if (guest_mode)   pf.down=100000000;
	ret=No;
		 if (is_char("gy",go.type))           ret=Yes;
	else if (bit(cfg.infomode,3))			  ret=Yes;
	else if (guest_mode&&!bit(cfg.downmode,6)) printf("\n### ¼Õ´ÔÀº ´Ù¿î¹ÞÀ»¼ö ¾ø½À´Ï´Ù.");
	else if (count==0)						  printf("\n### ÇØ´çµÇ´Â È­ÀÏÀÌ ¾ø½À´Ï´Ù.");
	else if (tsize==0)						  printf("\n### È­ÀÏÀÇ Å©±â°¡ 0ÀÔ´Ï´Ù.");
	else if (club_mode&&pf.type>14) 		  ret=Yes;	//µ¿È£È¸ÀÇ °æ¿ì ¿î¿µÁø ÀÌ»ó ¹«½Ã
	else if (from_userroom) 				  ret=Yes;	//»ç¿ëÀÚ¿µ¿ª¿¡¼­´Â ¹«Á¶°Ç ok
	else if (!club_mode&&pf.type>=15&&go.type=='u')  ret=Yes;  //¹«Á¶°Ç ´Ù¿î
	else if (club_mode&&cpf.type>=15&&go.type=='u')  ret=Yes;  //¹«Á¶°Ç ´Ù¿î
	else if (go.type=='u')                    printf("µî·ÏÀü¿ëÀÚ·á½Ç¿¡¼± ´Ù¿îÀÌ µÇÁö ¾Ê½À´Ï´Ù.");  //ÀÚÀ¯´Ù¿î¸ðµå¿¡¼­´Â ¹«Á¶°Ç ´Ù¿î
	else if (bcfg.fdown>0&&pf.type>=bcfg.fdown) ret=Yes;  //ÀÚÀ¯´Ù¿îµî±Þ
	else if (bcfg.fdown==99)				  ret=Yes;	//ÀÚÀ¯´Ù¿îµî±Þ
	else if (check_type('i'))                 ret=No; //¸Þ´º °¡´É µî±ÞÈ®ÀÎ
	else if (check_type('d'))                 ret=No; //¸Þ´º °¡´É µî±ÞÈ®ÀÎ
	else if (pf.type<cfg.downfree&&(pf.down<=0||pf.down<tsize)) printf("\n### %s´ÔÀÇ ´Ù¿î°¡´ÉÇÑ ¿ë·®( %d KB) À» ÃÊ°úÇÕ´Ï´Ù.",pf.id,pf.down/1000);
	else ret=Yes;

	if (!ret) goto _fail;

	if (c_proc) {
		printf("ID:%s %s T:%d  G:%s GT:%c\n",pf.id,pf.name,pf.type,go.code,go.type);
		printf("°¡´É:%dB ÇÊ¿ä:%dB freetype:%d Bfdown:%dn\n",pf.down,tsize,cfg.downfree,bcfg.fdown);
		pressenter();
	}

	//¸ÖÆ¼´Ù¿î°¡´É ¿©ºÎ?
	//È¸¿ø±¸ºÐº° ´Ù¿î/¾÷ÀÌ ÁöÁ¤µÇ¾î ÀÖ´Ù.
	mult_down=Yes;	  //¸ÖÆ¼´Ù¿î°¡´ÉÇÏ´Ù.
	to_userroom=No;
		 if (count<2) ;
	else if (!club_mode&&bcfg.fdown>0&&pf.type>=bcfg.fdown) ;
	else if (pf.type>=14||now_type>=14) ;
	else if (cfg.downmult);
	else if (from_userroom);
	else if (go.type=='y');          //ÀÚÀ¯´Ù¿î¸ðµå¿¡¼­´Â ¹«Á¶°Ç ´Ù¿î
	else if (go.type=='g');          //ÀÚÀ¯´Ù¿î¸ðµå¿¡¼­´Â ¹«Á¶°Ç ´Ù¿î
	else if (pf.down>100000000);	 //100¸Þ°¡ ÀÌ»óÀÏ¶§ ¹«Á¶°Ç ´Ù¿î
	else mult_down=No;				 //¸ÖÆ¼´Ù¿îºÒ°¡´É
	if (guest_mode&&from_userroom) goto _fail;
	if (guest_mode&&to_userroom)   to_userroom=No;

	if (!ret) goto _fail;

	if (pf.type<18&&(cfg.dn_limit||bcfg.dn_limit)) {
		key=No;
		if (cfg.dn_limit>0) {
			i=set_tag_down(4);	//ÇöÀç ÀüÃ¼¾÷/´Ù¿î¼ö
            if (i>=cfg.dn_limit) {a_num[8]=i;a_num[9]=cfg.dn_limit;key=Yes;}
		}
		if (!key&&bcfg.dn_limit>0) {
			i=set_tag_down(6);	//ÇöÀç ÀüÃ¼¾÷/´Ù¿î¼ö
            if (i>=bcfg.dn_limit) {a_num[8]=i;a_num[9]=bcfg.dn_limit;key=Yes;}
		}
		if (key) {
			view("frm/editor/dn_limit.log");
			goto _fail;
		}
	}

	//ÀÚ·á½Ç ÇÁ·ÎÅäÄÝ ¼±ÅÃ
	i2=1;
	if (mode!=2) {
        strcpy(g_buff4,pf.type==19?"9.DNS  ":"");
		if (from_userroom) view("frm/down/selectu.log");
        else if (is_same(go.code,"dncart")) view("frm/down/selectc.log");
        else view("frm/down/selectp.log");
        lineinput(buff,4);check_han(buff);printf("\n");
		i2=atoi(buff);
	}
	if (i2==0&&strlen(buff)==1) goto _fail;
	if (guest_mode&&i2==3) i2=1;
    if (is_same(go.code,"dncart")&&i2==4) i2=1;
	if (from_userroom&&i2==3) i2=1;

	set_stat(8,bbs.title);	   //´Ù¿îÁßÀÓ
	if (mode!=2)
	view("frm/down/wait.log");

	if (!bit(cfg.downmode,0)) {  //´Ù¿îÀü °¨¼Ò
		//´Ù¿î¼öÁöÁ¤À» ÇÔ
		set_bbs_down(count);	  //´Ù¿î¼öÁöÁ¤
		bbs_down_down(tsize);
	}
	pf.todaymode=set_bit(pf.todaymode,5,1);   //¿À´ÃÀÛ¾÷

	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	sys_tem("rm -rf %s",newdir);
	check_dir(newdir);
	if (i2==3) {	//»ç¿ëÀÚ ¿µ¿ª¿¡ ¸µÅ©·Î º¹»ç
		//»ç¿ëÀÚ ¿µ¿ª ¹æÀ» ¸¸µë
		sprintf(newdir,"userroom/%s/%s",c_han(pf.id),pf.id);
		check_dir(newdir);	   //¹æÀÌ ¾øÀ¸¸é »õ·Î ¸¸µë
		for (i=0;i<100;i++) {
			if (strlen(adnfile[i])>0&&is_file(adnfile[i])) {
				sprintf(newfile,"%s/%s",newdir,abbs[i].filename);
				sys_tem("ln -sf %s %s",adnfile[i],newfile);
				bbs=abbs[i];
                strcpy(bbs.passwd,"");
				bbs.del=No;
				bbs_add_userroom();  //°Ô½ÃÆÇ Ãß°¡(»ç¿ëÀÚ¿µ¿ª¿¡..)
			}
		}
		if (mode!=2)
		view("frm/down/downoku.log");
		goto _done;
	}
    else if (i2==4) {    //DNCART
		for (i=0;i<100;i++) {
            bbs=abbs[i];
            bbs2tmp();    //ÇöÀçBBSÀÇ ³»¿ëÀ» TMP·Î ÀúÀå
            dncart();
		}
        view("frm/down/downca.log");
		goto _done;
	}
	else if (i2==9&&pf.type==19) {	  //ÁöÁ¤µð·ºÅä¸®¿¡ º¹»ç
		_cont4:
		view("frm/down/dns.log");
		dosinput(DEF_DNS,50);check_han(DEF_DNS);printf("\n");
		if (strlen(DEF_DNS)<2) goto _fail;
		if (!is_dir(DEF_DNS)||in_file2("system/cannot.dir",DEF_DNS)) {
			printf(" µð·ºÅä¸®¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù.\n");
			goto _cont4;
		}
		for (i=0;i<100;i++) {
			if (strlen(adnfile[i])>0&&is_file(adnfile[i])) {
				sys_tem("cp -f %s %s",adnfile[i],DEF_DNS);
			}
		}
		view("frm/down/downok.log");
		goto _done;
	}
	else {		//ÀÏ¹Ý´Ù¿îÀÏ¶§
		count=0;
		for (i=0;i<100;i++) {
			if (strlen(adnfile[i])<1) continue;
			if (bit(cfg.downmode,15)) {
				n=sys_tem("ln -sf %s %s",adnfile[i],newdir);
				if (n!=0) {
					printf(" ¸µÅ© ½ÇÆÐ %s %s\n",adnfile[i],newdir);
					goto _fail_ln;
				}
			}
			else {
				if (bit(cfg.downmode,21)&&check_free()<50) {view("frm/system/nospace.log");goto _fail;}
				_fail_ln:
                if (strlen(adnfile[i])>0&&strlen(newdir)>0)
                    sys_tem("cp -f %s %s",adnfile[i],newdir);
			}
			printf(".");fflush(stdout);
			count++;
		}
		sprintf(buff,"ÃÑ%d°Ç",count);
		set_rec_(1,8,tsize,buff);
		if (mode!=2)
		printf("[80D[2K");
		sprintf(buff,"tmp/up%d%s/*",cfg.hostno,no_spc(tty));
		if (i2==2) {
			view("frm/down/kermit.log");
			n=kermitsz(buff);
		}
		else {
			if (mode!=2)
			view("frm/down/zmodem.log");
			n=sz(buff);
		}
		//´Ù¿îÃë¼Ò½Ã
		if (n!=0&&bit(cfg.downmode,16)==0) {
			view("frm/down/down.err");
			goto _fail;
		}
		if (mode!=2)
		view("frm/down/downok.log");
		goto _done;
	}

	_fail:
	view("frm/down/downfaip.log");
	bbs=oldbbs;
	ret=No;
	goto _end;

	_done:
    if (bit(cfg.downmode,0)||is_same(go.code,"dncart")) {  //´Ù¿îÈÄ °¨¼Ò
		//´Ù¿î¼öÁöÁ¤À» ÇÔ
		set_bbs_down(count);	  //´Ù¿î¼öÁöÁ¤
        if (!is_same(go.code,"dncart"))
            bbs_down_down(tsize);
	}
	ret=Yes;
	if (bit(cfg.downmode,12)==0) {
		if (view("frm/down/downstop.log")) {
			while (1) {
				printf("7");
				lineinput9(buff,20,6);
				if (stristr(buff,"^B0")||stristr(buff,"rz")||stristr(buff,"*B0")) {printf("7");continue;}
				break;
			}
			printf("\n");
		}
	}
	goto _end;

	_end:


	strcpy(NOW_FILE ,TNOW_FILE);
	strcpy(NOW_FILED,TNOW_FILED);
    if (is_same(go.code,"dncart")) {
        count=0;
        if ((fp=fopen(NOW_FILE,"r"))!=NULL) {
            while (fread(&bbs,sizeof_bbs,1,fp)) {
                if (bbs.down==0) {
                    count++;
                    break;
                }
            }
            fclose(fp);
            if (count==0) {
                unlink(NOW_FILE);
                unlink(NOW_FILED);
            }
        }
    }
	bbs=oldbbs;
	go=oldgo;
	bbs_num(bbs.num);
	NOW_NO=tNOW_NO;NOW_REC=tNOW_REC;NOW_TOPREC=tNOW_TOPREC;NOW_ALLREC=tNOW_ALLREC;
	set_stat(1,"");     //´Ù¿îÁß ÇØÁ¦
	rm_tmpup();
	return (ret);
}


//ÇöÀç °Ô½ÃÆÇÀÇ ³»¿ëÀ» ÇöÀç »ç¿ëÀÚ ¿µ¿ª¿¡ º¹»çÇÑ´Ù.
bbs_add_userroom()
{
	BBS oldbbs;
	FILE *fp1;
	FILE *fp2;
	int num;
	char c;
	int i;
	char oldNOW_FILE[100];
	char oldNOW_FILED[100];

	oldbbs=bbs;
	//ÇöÀçÀÇ bbsÀÇ ³»¿ëÀ» ÀÓ½ÃÈ­ÀÏ·Î ÀúÀåÇÑ´Ù.
	strcpy(oldNOW_FILE,NOW_FILE);
	strcpy(oldNOW_FILED,NOW_FILED);

	unlink(TMP_FILE);
	//³»¿ëÀ» ÀÓ½ÃÈ­ÀÏ·Î ÀúÀå
	fp1=fopen(NOW_FILED,"r+");
	fp2=fopen(TMP_FILE,"w");
	fseek(fp1,bbs.posi,SEEK_SET);
	for (i=0;i<bbs.size;i++) {
		c=fgetc(fp1);
		fputc(c,fp2);
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);

    if (!read_pf4(pf.id)) return;

	//ÀúÀåµÈ ÀÓ½ÃÈ­ÀÏ°ú bbs¸¦ »ç¿ëÀÚ¿µ¿ª¿¡ ÀúÀåÇÑ´Ù.
	sprintf(NOW_FILE ,"userdata/%s/%s/room.tit",c_han(pf2.id),pf2.id);
	sprintf(NOW_FILED,"userdata/%s/%s/room.txt",c_han(pf2.id),pf2.id);
	time(&bbs.date);  //³¯Â¥¸¦ ¿À´Ã³¯Â¥·Î ¼¼ÆÃ
	bbs_add(1);
	strcpy(NOW_FILE ,oldNOW_FILE);
	strcpy(NOW_FILED,oldNOW_FILED);
	unlink(TMP_FILE);
}



set_bbs_down(int count)
{
	FILE *fp;
	GO	oldgo,oldgo2,old_club;
	int i,i2,n,key;
	BBS bbs2,oldbbs;

	char filename[200];
	int arec[110];

	for (i=0;i<100;i++) arec[i]=-1;

	oldgo=go;
	oldbbs=bbs;
	if (count<1) return;

	//¸ÕÀú ÇØ´ç¹øÈ£ REC Ã£´Â´Ù.
	if (!is_typepds) return;
	if ((fp=fopen(NOW_FILE,"r"))==NULL) return;
	for (i=0;i<count;i++) {
		rewind(fp);
		i2=0;
		while (fread(&bbs,sizeof_bbs,1,fp)) {
			if (bbs.num==abbs[i].num) {
				arec[i]=i2;
				break;
			}
			i2++;
		}
	}
	fclose(fp);

	//Ã£ÀºREC¿¡ Ãß°¡ÇÔ
	c_lock(1,NOW_FILE);
	if ((fp=fopen(NOW_FILE,"r+"))==NULL) return;
	for (i=0;i<count;i++) {
		if (arec[i]<0) continue;
		fseek(fp,arec[i]*sizeof_bbs,SEEK_SET);
		if (fread(&bbs,sizeof_bbs,1,fp)) {
			bbs.down++; 		   //´Ù¿î¼öÁõ°¡
			time(&bbs.date2);	   //¸¶Áö¸·³¯ÀÚ¼ö
			fseek(fp,arec[i]*sizeof_bbs,SEEK_SET);
			del_esc3(bbs.filename);del_esc3(bbs.id);del_esc3(bbs.name);
			if (!feof(fp )) fwrite(&bbs,sizeof_bbs,1,fp);
		}
	}
	fclose(fp);
	c_lock(0,NOW_FILE);


	//ÀüÃ¼ÀÚ·á½Ç¿¡¼­ ´Ù¿îÇßÀ»¶§ ¿ø·¡¿¡ ´Ù¿î¼ö Ãß°¡
	if (go.type=='z') { //ÀüÃ¼ÀÚ·á½ÇÀÏ¶§
		for (i=0;i<count;i++) {
			sprintf(filename,"%s/menu/%s.tit",club_go.dir,abbs[i].tcode);
			if ((fp=fopen(filename,"r+"))==NULL) continue;
			i2=0;
			while (fread(&bbs,sizeof_bbs,1,fp)) {
				if (is_same(bbs.title,abbs[i].title)&&
					is_same(bbs.filename,abbs[i].filename)&&
					bbs.date==abbs[i].date) {
					bbs.down++;
					time(&bbs.date2);
					fseek(fp,i2*sizeof_bbs,SEEK_SET);
					fwrite(&bbs,sizeof_bbs,1,fp);
					break;
				}
				i2++;
			}
			fclose(fp);
		}
	}
	bbs=oldbbs;
	go=oldgo;
}



//½Ãµð·Ò ÀÚ·á½ÇÀÏ¶§ *.cd¿¡¼­ È­ÀÏ¸íÀ» ÀÐ¾î¿È
get_cdrom(char *filename,int num)
{
	CD cd;
	FILE *fp;
	strcpy(filename,"");
	sprintf(filename,"%s/menu/%s.cd" ,club_go.dir,go.code);
	fp=fopen(filename,"r+");
	if (fp==NULL) return;
	while (fread(&cd,sizeof(cd),1,fp)) {   //ÀÐ±â
		if (cd.num==num) {
			sprintf(filename,"%s%s",cd.dir,cd.filename);
			break;
		}
	}
	fclose(fp);
}


cpf_down_down(int w,int d)	  //ÇØ´çµ¿È£È¸ ´Ù¿î¼ö Áõ°¡
{
	FILE *fp;
	int i;
	char id_file[300];
	CPF cpf2;

	if (!club_mode) return;
	sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
	if ((fp=fopen(id_file,"r+"))!=NULL) {
		i=-1;
		while (fread(&cpf2,sizeof_cpf,1,fp)&&i++)
			if (is_same(cpf2.id,cpf.id)) {
				fseek(fp,i*sizeof_cpf,SEEK_SET);
				if (is_char(BBBTYPE)) cpf2.countw+=w;
				else				  cpf2.countu+=w;
				cpf2.countd+=d;
				fread(&cpf2,sizeof_cpf,1,fp);
				break;
			}
		fclose(fp);
	}
}



//°Ô½Ã¹°ÀÇ ³»¿ëÀ» ´Ù¿î¹Þ´Â´Ù.  0:ÀÐ±â 1:´Ù¿î
bbs_pr(int mode)
{
	char ch;
	char OLDDIR[500];
	char buff[200];
	char buff2[200];
	char buff3[50];
	char filename[200];
	char filename2[100];
	BBS oldbbs;
	BBS bbs2;
	BBS abbs[102];

	int tNOW_NO=NOW_NO;
	int tNOW_REC=NOW_REC;
	int tNOW_ALLREC=NOW_ALLREC;
	int i,i2,key,count,ret,tsize_bbs;
	int count_file,tsize_file;
	FILE *fp;
	FILE *fp1;
	FILE *fp2;
	struct tm *tp;
	time_t today;
	int mail_mode=No;
	int bbs_down=Yes;
	int file_down=No;

    oldbbs=bbs;
	getcwd(OLDDIR,sizeof(OLDDIR));		//ÇöÀçÀÇ µð·ºÅä¸®
	re_print=Yes;

    //»ç¿ëÀÚ¿µ¿ªÀÏ¶§
	//È­¸é Ãâ·Â¸ðµåÀÏ¶§ dn¸¸ ´©¸§
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	if (pf.type<18&&bit(bcfg.form,1)) {mess("PR/DN±ÝÁö ¿µ¿ªÀÔ´Ï´Ù.");return No;}
	if (go.type=='k') {message("ºñ°ø°³´ëÈ­¹æÀÔ´Ï´Ù.");return No;}
    if (bit(limit_mode,3)) {
        if (!view("frm/pf/notdown.log")) mess("PR±ÝÁö");
        return No;
    }
	if (mmn[1]==0) {
		printf("\n °Ô½ÃÆÇ È­ÀÏ ´Ù¿î ¹æ¹ý pr [¹øÈ£]\n");
		printf("   ¿¹) pr 1,2,3 1-5   (¿¬¼ÓÀ¸·Î 100°³±îÁö ´Ù¿îÀÌ °¡´ÉÇÕ´Ï´Ù.)\n");
		printf("   ¿¹) pr 10 11 12 13-15 (10,11,12,13,14,15¹ø ´Ù¿î)\n");
		pressenter();
		return No;
	}
	if (mode==0) {
		printf("\n PRINTER/CAPTURE(°¥¹«¸®)¸¦ ÁØºñÇÏ½Ã°í  [ENTER] ¸¦ ´©¸£½Ê½Ã¿À.\n");
        lineinput(buff,10);
        if (is_quit(buff)) return No;
	}

	if (is_same(go.code,"rmail")||is_same(go.code,"mbox")||is_same(go.code,"cmail")) mail_mode=Yes;
	printf("\n");
	count=0;
	count_file=0;
	tsize_file=0;
	tsize_bbs =0;
	for (i=1;i<101;i++) {
        if (mmn[i]<=0||!bbs_num(mmn[i])||bbs.del) continue;
        key=Yes;
        if (pf.type<18&&!bit(bcfg.mode,31)&&strlen(bbs.passwd)>0)    //¾ÏÈ£°¡ ÁöÁ¤µÇ¾î ÀÖÀ»¶§
            key=check_pass(bbs.passwd,4,"ÀÌ ÀÚ·á´Â ¾ÏÈ£°¡ ÁöÁ¤µÇ¾î ÀÖ½À´Ï´Ù.");
        if (!key) continue;
        abbs[count]=bbs;
        tsize_bbs+=bbs.size;
        strcpy(abbs[count].filename,"");
        abbs[count].filesize=0;
        count++;
        if (mode==1&&mail_mode) {
            sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
            if (is_file(buff)) {
                i2=file_size(buff);
                if (i2>0) {
                    count_file++;
                    tsize_file+=i2;
                    strcpy(abbs[count].filename,bbs.filename);
                    abbs[count].filesize=i2;
                }
            }
        }
	}

	printfL2();
	sprintf(buff,"tmp/up%d%s",cfg.hostno,no_spc(tty));check_dir(buff);
	strcpy(buff3,"");
	for (i=0;i<count;i++) {
		if (abbs[i].filename[i]!=0) strcpy(buff2,byte2str(abbs[i].filesize));
		else						strcpy(buff2,byte2str(abbs[i].size));
        if (mode==1) {
            printf("%3d µî·Ï:%-8s %-34.34s %-12.12s%s\n"
            ,abbs[i].num,abbs[i].id,abbs[i].title,abbs[i].filename,buff2);
        }
		if (abbs[i].filename[0]!=0) strcpy(buff3,abbs[i].filename);
	}

    sprintf(filename2,"%s.cap",now_str(11));
    sprintf(filename,"tmp/up%d%s",cfg.hostno,no_spc(tty));check_dir(filename);
    sprintf(filename,"tmp/up%d%s/%s",cfg.hostno,no_spc(tty),filename2);
    if (mode==1) {
        printf("\n");
        printf(" ´Ù¿î·ÎµåÇÏ¿© ÀÚ½ÅÀÇ PC¿¡ ÀúÀåµÉ È­ÀÏ¸íÀ» ÀÔ·ÂÇÏ½Ê½Ã¿À.(¿¹: i1.txt)\n");
        printf(" EnterÅ°¸¦ ´©¸£¸é \"³¯Â¥½Ã°£.CAP\" È­ÀÏ¸íÀÌ ÀÚµ¿À¸·Î ÁöÁ¤µË´Ï´Ù.\n");
        printf(" (Q:Ãë¼Ò) >>");
        lineinput(buff,13);del_space(buff);del_esc(buff);
        if (is_same(buff,"q")) goto not_pr;
        if (buff[0]==0) printf(filename2);
        printf("\n");
        if (buff[0]!=0&&good_filename(buff)) strcpy(filename2,buff);

        sprintf(filename,"tmp/up%d%s/%s",cfg.hostno,no_spc(tty),filename2);
        printf(" ### ÃÑ %3d°³ °Ô½Ã¹°   ÃÑÈ­ÀÏÅ©±â %5d KB  È­ÀÏ¸í:%s\n",count,tsize_bbs/1000,filename2);
        if (count_file>0&&tsize_file>0&&mode==1&&mail_mode)
        printf(" ### ÃÑ %3d°³ È­  ÀÏ   ÃÑÈ­ÀÏÅ©±â %5d KB  È­ÀÏ¸í:%s\n",count_file,tsize_file/1000,buff3);

        ret=No;
        if (!is_char("gy",go.type)&&guest_mode) printf("¼Õ´ÔÀº »ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù.");
        else if (count==0)                      printf("ÇØ´çµÇ´Â È­ÀÏÀÌ ¾ø½À´Ï´Ù.");
        else if (bbs_man(pf.id))                ret=Yes;
        else if (check_type('r'))               ret=No;
        else if (count_file>0&&check_type('d')) ret=No;
        else ret=Yes;

        if (ret==No) goto not_pr;

        bbs_down=Yes;
        file_down=No;
        if (count_file>0&&tsize_file>0&&mail_mode&&mode==1) {
            printf("\n 1.¸ÞÀÏ+Ã·ºÎÈ­ÀÏ´Ù¿î 2.¸ÞÀÏ ´Ù¿î 3.È­ÀÏ¸¸ ´Ù¿î  0.Ãë¼Ò >>");
            lineinput(buff,1);printf("\n");
            if      (buff[0]==0||buff[0]=='1')  {bbs_down=Yes;file_down=Yes;}
            else if (buff[0]=='2')              {bbs_down=Yes;file_down=No; }
            else if (buff[0]=='3')              {bbs_down=No ;file_down=Yes;}
            else goto not_pr;
        }
    }

    //°Ô½ÃÆÇ °¥¹«¸®
	if (bbs_down) {
		fp1=fopen(filename,"w+");
		if (fp1==NULL) {
			printf("%s È­ÀÏ OPEN ½ÇÆÐ",filename);
			goto not_pr;
		}
		fp2=fopen(NOW_FILED,"r+");
		if (fp2==NULL) {
			printf("%s È­ÀÏ OPEN ½ÇÆÐ",NOW_FILED);
			goto not_pr;
		}
		for (i=0;i<count;i++) {
			tp=localtime(&abbs[i].date);
			fprintf(fp1,"#%-7d",abbs[i].num);
			fprintf(fp1,"%-8s(%-8s)\n",abbs[i].id,abbs[i].name);
			fprintf(fp1,"%-60.60s %02d/%02d %02d:%02d%6d\n",
			abbs[i].title,tp->tm_mon+1,tp->tm_mday,tp->tm_mday,
			tp->tm_hour,tp->tm_min,abbs[i].line);
			fseek(fp2,abbs[i].posi,SEEK_SET);
			for(i2=0;i2<abbs[i].size;i2++) {
				ch = fgetc(fp2);
				fputc(ch,fp1);
			}
		}
		if (fp1!=NULL) fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
	}

	//ÀÐÀºÈ½¼ö Áõ°¡
	fp=fopen(NOW_FILE,"r+");
	for (i=0;i<count;i++) {
		if (bbs_num(abbs[i].num)) {
			if (fread(&bbs2,sizeof_bbs,1,fp)&&bbs2.num==abbs[i].num) {
				time(&bbs2.date2);
				bbs2.read++;
				if (mode!=0&&abbs[i].filename[0]!=0&&file_down&&is_typepds) bbs2.down++;
				fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
				del_esc3(bbs2.filename);del_esc3(bbs2.id);del_esc3(bbs2.name);
				if (NOW_REC<1000000) {
					if (!feof(fp )) fwrite(&bbs2,sizeof_bbs,1,fp);
				}
				bbs=bbs2;
			}
		}
	}
	if (fp!=NULL) fclose(fp);

	if (mode==0) {
		view_text(filename);
	}
	else {	   //°Ô½ÃÆÇ ´Ù¿îÀÏ¶§
		ret=Yes;
		set_stat(8,bbs2.title); 	//´Ù¿îÁßÀÓ
		sprintf(buff,"PR %s",bbs2.title);
		set_rec_(1,8,0,buff);
		if (file_down&&mail_mode&&mode==1) {
			for (i=0;i<count;i++) {  //¹ÙÀÌ³Ê¸® Àü¼Û
				if (abbs[i].filename[0]!=0) {
					sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,abbs[i].filename);
					if (is_file(buff)) sys_tem("ln -sf %s tmp/up%d%s",buff,cfg.hostno,no_spc(tty));
				}
			}
		}
		printf("\n ## zmodem È­ÀÏÀü¼ÛÀ» ½ÃÀÛÇÕ´Ï´Ù. ## \n");
		sys_tem("%s tmp/up%d%s/*",cfg.sz,cfg.hostno,no_spc(tty));
	}

	set_stat(1,"");     //´Ù¿îÁß ÇØÁ¦
	printf("\n");
	bbs=oldbbs;
	NOW_NO=tNOW_NO; 			//¿ø·¡ÀÇ ¹øÈ£ º¹±¸
	NOW_REC=tNOW_REC;
	NOW_ALLREC=tNOW_ALLREC;
    if (mode==1) {
        printf("\n ## °Ô½ÃÆÇ ´Ù¿îÀÌ ³¡³µ½À´Ï´Ù. [Enter]\n");getchar();
    }
	return Yes;

	not_pr:
		bbs=oldbbs;
		NOW_NO=tNOW_NO; 			//¿ø·¡ÀÇ ¹øÈ£ º¹±¸
		NOW_REC=tNOW_REC;
		NOW_ALLREC=tNOW_ALLREC;
		sprintf(buff,"rm tmp/up%d%s -rf",cfg.hostno,no_spc(tty));sys_tem(buff);
		printf("\n ## °Ô½ÃÆÇ ´Ù¿îÀÌ Ãë¼ÒµÇ¾ú½À´Ï´Ù. [Enter]\n");getchar();
		return No;
}



//ÇöÀç °Ô½ÃÆÇÀÇ ³»¿ëÀ» Àü¼ÛÇÑ´Ù.
bdn()
{
	char buff[300];
	char bdn_file[300];
	sprintf(bdn_file,"%s/logo/%s.bdn",club_go.code,go.code);
	if (ss_check(3))  return;  //´Ù¿î ±ÝÁö
	if (pf.type>=18);
	else if (bit(bcfg.downmode,20));
	else if (is_char(go.wtimes,'#'));
	else if (in_file2("system/bdn_id.dat",pf.id));
	else if (in_file2(bdn_file,pf.id));
	else {
		mess(" BDN ±â´ÉÀ» »ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù.");
		return;
	}
	sys_tem("bin/bdn '%s' '%s' '%s'",go.code,club_go.code,pf.id);
	re_print=Yes;
}


//´Ù¿î¿ë·®Áõ°¡ ½ÃÅ´  ¾÷/Àû±â ½Ã  mode 1:Áõ°¡ -1:°¨¼Ò
bbs_down_up(int tsize,char *tid,int mode)
{
	int i,olddown;
	PF pf3,oldpf;
	char buff1[30],buff2[30],buff3[30];

	oldpf=pf;
	if (guest_mode) 		 return;
    if (!read_pf4(tid))      return;
	if (pf2.type<1) 		 return;
	if (bcfg.fdown>0&&pf2.type>=bcfg.fdown) return;   //ÀÚÀ¯´Ù¿î°¡´ÉÀÚÀÏ¶§
	olddown=pf2.down;
	if (club_mode&&cfg.clublevel!=0&&(byte)bcfg.level>(byte)cfg.clublevel)
		bcfg.level=(byte)cfg.clublevel;
	if (mode==-1)  {	 //°¨¼âÀÏ¶§ (ÀÚ·á »èÁ¦½Ã)
        if ((word)bcfg.level_size!=0) pf2.level -=(word)(tsize/bcfg.level_size);
		else if (is_typepds&&(word)cfg.level_size>0) pf2.level -=(word)(tsize/cfg.level_size);
		else						 pf2.level -=bcfg.level;
		if (is_same(pf2.id,pf.id)) {
			UP_LEVEL-=bcfg.level;
		}
		pf2.down  -=(tsize*cfg.drate)/10;	//°¡»ê´Ù¿î¿ë·® Á¶Á¤

	}
	else {
		if ((word)bcfg.level_size>0) pf2.level +=(word)(tsize/bcfg.level_size);
		else if (is_typepds&&(word)cfg.level_size>0) pf2.level +=(word)(tsize/cfg.level_size);
		else						 pf2.level +=bcfg.level;
		if (cfg.urate >0) tsize=(tsize*cfg.urate )/10;	  //µî·Ï½Ã ºñÀ²
		if (bcfg.uratet>0&&in_hour(bcfg.uhour1,bcfg.uhour2)) tsize=(tsize*bcfg.uratet)/10;	//½É¾ßµî·Ï½Ã ºñÀ²
		else if (bcfg.urate>0) tsize=(tsize*bcfg.urate)/10; 								//ÀÏ¹Ýµî·Ï½Ã ºñÀ²
		pf2.down  +=tsize;
		if (is_same(pf2.id,pf.id)) UP_LEVEL+=bcfg.level;
	}
	if (pf2.down>500000000) pf2.down=500000000;
	if (pf2.down<0		  ) pf2.down=0;
	if (pf2.level<0 	  ) pf2.level=0;
	if (is_typepds) 		pf2.countu++;
	else					pf2.countw++;
    if (pf2.type>0) save_pf2(0);
	if (is_bbspds&&is_same(pf.id,pf2.id)) {
		pf.down=pf2.down;
		pf.level=pf2.level;
//		pf.countu=pf2.countu;
//		pf.countw=pf2.countw;
		if (bit(cfg.downmode,14)) {
			if (oldpf.down !=pf.down  ) printf(" *** ID:%s ÃÑÅ©±â:%dK ´Ù¿î·®º¯°æ %dK -> %dK \n",pf.id,tsize/1000,oldpf.down/1000,pf.down/1000);
			if (oldpf.level!=pf.level ) printf(" *** ID:%s ·¹º§º¯°æ %d -> %d \n",pf.id,oldpf.level,pf.level);
		}
	}
}


//´Ù¿î¿ë·®°¨¼Ò
bbs_down_down(int tsize)
{
	int fdown=No,olddown;
	PF oldpf;
	char buff1[30],buff2[30],buff3[30];
	oldpf=pf;
	if (guest_mode) 			return;
    if (!read_pf4(pf.id))       return;             //ÇØ´çÈ¸¿øÀÌ ¾øÀ»¶§
	if (!is_typepds) return;
	if (bbs_man(pf.id)) 		return;
	if (stristr(go.code,"mail")||stristr(go.code,"mbox")) return;

	olddown=pf2.down;
	if (is_char("yg",go.type)||pf2.type>=cfg.downfree)   fdown=Yes;
	if (cfg.drate >0) tsize=(tsize*cfg.drate )/10;	  //´Ù¿î½Ã ºñÀ²
	if (bcfg.dratet>0&&in_hour(bcfg.dhour1,bcfg.dhour2)) tsize=(tsize*bcfg.dratet)/10;	//½É¾ß´Ù¿î½Ã ºñÀ²
	else if (bcfg.drate>0) tsize=(tsize*bcfg.drate)/10;    //´Ù¿î½Ã ºñÀ²			  //ÀÏ¹Ý´Ù¿î½Ã ºñÀ²
	if (pf2.type>0&&!fdown)  pf2.down  -=tsize ; //°¡»ê´Ù¿î¿ë·® Á¶Á¤
	if (pf2.down<0) 		 pf2.down=0;
	pf2.countd++;
    if (pf2.type>0) save_pf2(0);
	if (is_bbspds&&is_same(pf.id,pf2.id)) {
		pf.down=pf2.down;
//		pf.level=pf2.level;
//		pf.countu=pf2.countu;
//		pf.countw=pf2.countw;
		if (bit(cfg.downmode,14)) {
			if (oldpf.down !=pf.down  ) printf(" *** ID:%s ÃÑÅ©±â:%dK ´Ù¿î·®º¯°æ %dK -> %dK \n",pf.id,tsize/1000,oldpf.down/1000,pf.down/1000);
			if (oldpf.level!=pf.level ) printf(" *** ID:%s ·¹º§º¯°æ %d -> %d \n",pf.id,oldpf.level,pf.level);
		}
	}
	cpf_down_down(0,1);   //ÇØ´çµ¿È£È¸ ´Ù¿î¼ö Áõ°¡
}


/*
 ¼±ÅÃ È­ÀÏ ÀÌ¸§
	¢Ñtest.zip
 ÆÄÀÏ Àü¼Û ÇÁ·ÎÅäÄÝÀ» ¼±ÅÃÇÏ½Ê½Ã¿À.
 1.[7mZmodem[m  2.Kermit	0.Ãë¼Ò
 ¹øÈ£¼±ÅÃ(ENTER:Zmodem) >> !ÁßÁö!

 ÆÄÀÏ Àü¼Û ÇÁ·Î±×·¥À» ½ÇÇà½ÃÄÑ ÁÖ½Ê½Ã¿À !!
Àá½Ã ±â´Ù¸®½Ê½Ã¿À.
*/
mail_down()
{
	BBS abbs[101],oldbbs=bbs;
	int i,i2,n,n2,count=0,tsize;
	int count_file,tsize_bbs,tsize_file;
	char buff[300];
	char buff2[300];
	char newdir[300];

	if (guest_mode) goto _fail;
	count_file=tsize_bbs=tsize_file=0;
	view("frm/down/title.log");
	for (i=0;i<100;i++) {
		abbs[i].num=abbs[i].filesize=-1;
		strcpy(abbs[i].filename,"");
	}

	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	for (i=1;i<101;i++) {
		if (mmn[i]==0||!bbs_num(mmn[i])||bbs.del) continue;
		//¾ÏÈ£°¡ ÁöÁ¤µÇ¾î ÀÖÀ»¶§
        if (!bit(bcfg.mode,31)&&strlen(bbs.passwd)>0&&
            !check_pass(bbs.passwd,4,"ÀÌ ÀÚ·á´Â ¾ÏÈ£°¡ ÁöÁ¤µÇ¾î ÀÖ½À´Ï´Ù.")) continue;
		abbs[count++]=bbs;
	}
	count=0;
	tsize=0;
	for (i=0;i<100;i++) {
		if (abbs[i].num>=0) {
			sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,abbs[i].filename);
			if (strlen(abbs[i].filename)>0&&is_file(buff)) {
				printf("    ¢Ñ%-13s %8d ¹ÙÀÌÆ® (ÆÄÀÏ)\n",abbs[i].filename,abbs[i].filesize);
				count++;
				tsize+=abbs[i].filesize;
			}
			else {
				sprintf(abbs[i].filename,"%s.%d",pf.id,i+1);
				abbs[i].filesize=-1;
				printf("    ¢Ñ%-13s %8d ¹ÙÀÌÆ® (¸ÞÀÏ)\n",abbs[i].filename,abbs[i].size);
				count++;
				tsize+=abbs[i].size;
			}
		}
	}
	if (count<1) goto _fail;
	if (count>1) printf("    ÃÑ %d °³ È­ÀÏ  %10d ¹ÙÀÌÆ®\n",count,tsize);

	//ÇÁ·ÎÅäÄÝ ¼±ÅÃ
	view("frm/down/select.log");
	lineinput(buff,2);check_han(buff);printf("\n");
	i2=atoi(buff);
	if (i2==0&&strlen(buff)==1) goto _fail;

	view("frm/down/wait.log");
	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	check_dir(newdir);
	for (i=0;i<100;i++) {
		if (strlen(abbs[i].filename)>0&&abbs[i].num>=0) {
			sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,abbs[i].filename);
            if (is_file(buff)) {
				sys_tem("cp -f '%s' %s/",buff,newdir);
				printf(".");fflush(stdout);
			}
			else if (bbs_num(abbs[i].num)) {
				bbs2tmp(); //ÇöÀçBBSÀÇ ³»¿ëÀ» TMP·Î ÀúÀå
				sprintf(buff,"%s/%s",newdir,abbs[i].filename);
				file_move(TMP_FILE,buff);
				if (is_file(buff)) {
					printf(".");fflush(stdout);
				}
			}
		}
	}
	sprintf(buff,"tmp/up%d%s/*",cfg.hostno,no_spc(tty));
	if (i2==2) {
		view("frm/down/kermit.log");
		n=kermitsz(buff);
	}
	else {
		view("frm/down/zmodem.log");
		n=sz(buff);
	}
	sys_tem("rm -rf %s",newdir);
	view("frm/down/downok.log");
	return Yes;

	_fail:
	view("frm/down/downfail.log");
	return No;

}




set_tag_down(int mode)
{
	FILE *fp;
    int i,n,count=0;
    char filename[100];

    for (n=0;n<20;n++) {
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename, "r"))==NULL) continue;
        while (fread(&moni,sizeof_moni,1,fp)) {
            if (moni.mode<1||strlen(moni.id)<3||strlen(moni.tty)<1) continue;
            if(moni.mode<1||strlen(moni.tty)<1||strlen(moni.id)<1) continue;
                 if (mode==3&&moni.mode==6) count++;   //¾÷¼ö
            else if (mode==4&&moni.mode==8) count++;   //´Ù¿î¼ö
            else if (mode==5&&moni.mode==6&&is_same(moni.club,club_go.code)&&is_same(moni.gocode,go.code)) count++;   //¾÷¼ö
            else if (mode==6&&moni.mode==8&&is_same(moni.club,club_go.code)&&is_same(moni.gocode,go.code)) count++;   //´Ù¿î¼ö
            else break;
        }
        fclose(fp);
    }
	return count;
}


//¿¹ºñ´Ù¿î·Îµå
dncart()
{
    char filename1[300];
    char filename2[300];
    char old_NOW_FILE[300],old_NOW_FILED[300];
    if (strlen(bbs.filename)<1) return;
    strcpy(old_NOW_FILE ,NOW_FILE );
    strcpy(old_NOW_FILED,NOW_FILED);
    sprintf(filename1,"tmp/tmpa%d%s",cfg.hostno,no_spc(tty));   //È£½ºÆ®¹øÈ£º°ÀÓ½ÃÈ­ÀÏ
    sprintf(filename2,"tmp/tmpb%d%s",cfg.hostno,no_spc(tty));   //È£½ºÆ®¹øÈ£º°ÀÓ½ÃÈ­ÀÏ
    strcpy(NOW_FILE ,filename1);
    strcpy(NOW_FILED,filename2);
    strcpy(bbs.tcode,go.code);
	sprintf(bbs.key,club_go.code);
    bbs.date2=0;
    bbs.down=0;
    bbs_add(1);
    strcpy(NOW_FILE ,old_NOW_FILE );
    strcpy(NOW_FILED,old_NOW_FILED);
}


make_code_filename(char *filename,char *club,char *code,char *bbs_filename)
{
	FILE *fp;
	int found=No;
    char menufile[300];
    GO go,club_go;

    strcpy(filename,"");//¾øÀ½
    if (strlen(bbs_filename)<1) return No;
    strcpy(club_go.dir,"./");
    if (!is_same(club,"main")) {//µ¿È£È¸°¡¾Æ´Ò°æ¿ì
        found=No;
        fp=fopen(MENU_FILE,"r");
        while(fread(&go,sizeof_go,1,fp)) {
            if (go.type=='s'&&is_same(go.code,club)) {
                found=Yes;
                club_go=go;
                break;
            }
        }
        fclose(fp);
        if (!found) return No;
    }
    found=No;
    sprintf(menufile,"%s/%s",club_go.dir,MENU_FILE);
    if ((fp=fopen(menufile,"r"))==NULL) return No;
    while(fread(&go,sizeof_go,1,fp)) {
        if (is_char(PDSTYPE,go.type)&&is_same(go.code,code)) {
            found=Yes;
            break;
        }
    }
    fclose(fp);
    if (!found) return No;
    if (strlen(go.dir)<1) sprintf(go.dir,"%s/pds/%s",club_go.dir,go.code);
    sprintf(filename,"%s/%s",go.dir,bbs_filename);
    return Yes;
}
