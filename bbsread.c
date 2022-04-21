///////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
//	  °Ô½ÃÆÇ °ü·Ã ·çÆ¾
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//mode=0:ÀÏ¹Ý 1:»õ·Î 8:¿¬¼Ó 9:pr ·Î º¸¿©ÁØ´Ù.
//mode=-1:µÚ·Î  1:´ÙÀ½
bbs_read(int mode)	//ÇØ´ç¹øÈ£ ³»¿ëÃâ·Â
{
	FILE *fp1,*fp2;
	char buf[280];
	char buff[280];
	char filename[280];
	char buff1[3502];
	char buff2[3502];
	char ch=0;
	char all_mailmode=No;
    char mailmode=No;
	char view_jpg_ok=No;
	char found=No;
	char old_gotype=go.type;
    int temp_key,mail_mode=No;
	int pagepos[5000];
	int i,ii,disp_x,i2,is_dsp=No;
    int page,pos,count,disp_y;
	int stop_count,last_pos,last_pos2,last_pos3;
	int bbs_mode=is_typebbs;
	BBS bbs2,nowbbs;
    int str1_count=0;
	char TMP_FILE4[280];
	char jpg_file[200];

    _last2=5;;;;;;;;;;;;;;;;;;;;;;;;
	if ((is_same(go.code,"rmail")||
         is_same(go.code,"wmail")||
         is_same(go.code,"mbox")||
		 is_same(go.code,"cmail"))) {
         mailmode=Yes;
		if (bbs.del) {
			list_mode=No;
			re_print=Yes;
			return;
		}
		if (dd_mode) set_ddmode(2); 	 //»èÁ¦µÈ°Í ¾Èº¸ÀÌ°Ô
	}
    else {
        sprintf(buff,"%s/logo/%s.nre",club_go.dir,go.code);
        if (is_file(buff)) {view(buff);goto _fail;}
        sprintf(buff,"%s/logo/all.nre",club_go.dir);
        if (is_file(buff)) {view(buff);goto _fail;}
    }
    if (!mailmode&&bit(limit_mode,2)) {
        if (!view("frm/pf/notread.log")) mess("ÀÐ±â±ÝÁö");
        return;
    }

	if (go.type=='w'&&bbsreadall&&!bit(bcfg.jpgmode,2)) {        //³»¿ë´õÀÌ»ó¾øÀ½
		list_mode=No;
		re_print=Yes;
		return;
	}
    if (bit(bcfg.modelist,8)&&bbs.gd=='*') bbs.gd=' ';
	nowbbs=bbs;
	g_char=gd_chr(bbs_mode);	 //ÃßÃµ/Á¶È¸/´Ù¿îÁöÁ¤
	sprintf(filename,"%s/logo/%s.lir",club_go.dir,go.code);
	if (check_bbspasswd()==No) return;
	if (guest_mode&&(is_same(go.code,"rmail")||is_same(go.code,"mbox")||is_same(go.code,"cmail"))) return;

    mail_mode=(is_same(go.code,"rmail")||is_same(go.code,"cmail")||is_same(go.code,"mbox"));

	if (bbs_man(pf.id));
	else if (bbs_mode&&bit(bcfg.mode,29)&&in_file2("system/node_net.txt",tty)) return;
    else if (mail_mode) ;
	else if (is_same(go.code,"notice"));
	else if (check_type('r')) return;   //¸Þ´º °¡´É µî±ÞÈ®ÀÎ
	else if (ss_check(7)) return;	//ÀÐ±â ±ÝÁö µî±Þ
	else if (bit(bcfg.modelist,5)&&!in_file2(filename,pf.id)) return;

	if (bit(bcfg.automode,1)||
		bit(bcfg.automode,2)||
		bit(bcfg.automode,3)) auto_bbs();	  //ÀÚµ¿¸ðµå
	re_print=No;

	list_mode=Yes;		//³»¿ëÃâ·Â¸ðµå
	if (bcfg.listline<1) bcfg.listline=16;
	//Ãâ·Â½ÃÀÛÀ§Ä¡Ã£±â
	disp_x=bcfg.disp_x;
	disp_y=bcfg.disp_y;
	bbs.title[60]=0;del_tab(bbs.title);all_trim(bbs.title);
	del_space(bbs.filename);del_tab(bbs.filename);del_esc(bbs.filename);bbs.filename[12]=0;
	//vtxÀÌÁö¸¸ µ¥ÀÌÅ¸°¡ ¾Æ´Ô
	if (set_bbs_logo("tor",filename)) view(filename);
	if (bit(bcfg.modelist,0)) return;	//³»¿ë¾Èº¸ÀÓ
	if (bit(bcfg.mode,24)|| 				//³»¿ë->formÀ¸·Î
        go.type=='i'||(mail_mode&&is_same(bbs.key,"Ä«µå"))) {            //·Î°í°Ô½ÃÆÇ
		view7(NOW_FILED,bbs.posi,bbs.size);
		getchar2();
		if (bbs_mode) {uall.read++;uall.readsize+=bbs.size;}
		return;
	}

    if (go.type=='v'||go.type=='+') {
		cls();
		if (go.type=='v') set_stat2(14,"VTXÁ¤º¸ °Ë»öÁß");
		else			  set_stat2(14,"ANSIÁ¤º¸ °Ë»öÁß");
		if (go.type=='v') printf(ESCVTX);
		view6(NOW_FILED,bbs.posi,bbs.size);
		getchar2();
		if(go.type=='v') printf(VTXOFF);
		if (bbs_mode) {uall.read++;uall.readsize+=bbs.size;}
		re_print=Yes;
		list_mode=No;
		set_stat(1,"");
		return;
	}

    pf.todaymode=set_bit(pf.todaymode,4,1);   //¿À´ÃÀÛ¾÷
	all_mailmode=No;
	//ÀüÃ¼¸ÞÀÏÀÏ¶§Ã³¸® ÀÐ±â,´Ù¿î¼öÃ³¸®
	if (is_same(go.code,"rmail")||is_same(go.code,"mbox")) {
		if (is_same(bbs.key,"ÀüÃ¼¸ÞÀÏ")||is_same(bbs.key,"ÀüÃ¼Ä«µå")) {
			strcpy(filename,"menu/_allmail.tit");
			if ((fp2=fopen(filename,"r"))!=NULL) {
				i=0;
				i2=-1;
				found=No;
				while (fread(&bbs2,sizeof_bbs,1,fp2)) {
					if (!bbs.del&&is_same(bbs2.id,bbs.id)&&bbs2.date==bbs.date) {
						bbs.posi=bbs2.posi;
						bbs.size=bbs2.size;
						bbs.line=bbs2.line;
						strcpy(bbs.filename,bbs2.filename);
						strcpy(bbs.title,bbs2.title);
						if (!is_same(go.code,"cmail")) found=Yes;
						all_mailmode=Yes;
						i2=i;
						break;
					}
					i++;
				}
				fclose(fp2);
				//ÀüÃ¼¸ÞÀÏÀÐÀº¼ö Áõ°¡
				if (i2>=0&&found&&(fp2=fopen(filename,"r+"))!=NULL) {
					bbs2.read++;		 //ÀÐ±â¼ö Áõ°¡
					time(&bbs2.date2);
					fseek(fp2,i2*sizeof_bbs,SEEK_SET);
					fwrite(&bbs2,sizeof_bbs,1,fp2);
					fclose(fp2);
				}
			}
		}
	}

	//µ¥ÀÌÅ¸ÆÄÀÏ
	strcpy(filename,all_mailmode?"menu/_allmail.txt":NOW_FILED);
	if ((fp1=fopen(filename,"r"))==NULL) goto view_ok2;
    if (view_pos>=bbs.posi+bbs.size-1) view_pos=bbs.posi+bbs.size-1;
    if (view_pos<bbs.posi) view_pos=bbs.posi;


	//ÇöÀçpage Ã£±â
	N_page=1;
	stop_count=14;		  //È­¸éÀÌ ¸ØÃß´Â ÁÙ¼ö
    //ÇöÀçPAGE¼ö,ÀÌÀüÀ§Ä¡°è»ê
	fseek(fp1,bbs.posi,SEEK_SET);
	count=0;
    str1_count=0;

    for(i=bbs.posi;i<view_pos&&!feof(fp1);i++) {
        ch=fgetc(fp1);if(ch=='\n'||str1_count++>(79-disp_x)) {count++;str1_count=0;}
		if (count>=bcfg.listline||!bit(bcfg.mode,3)&&count>=stop_count) {
			count=0;
			N_page++;
		}
	}
    if (mode==-1) {      //ÀÌÀüÀ§Ä¡
        count=0;
        for(;view_pos>=bbs.posi&&!feof(fp1);view_pos--) {
            fseek(fp1,view_pos,SEEK_SET);
            ch=fgetc(fp1);if(ch=='\n'||str1_count++>(79-disp_x)) {count++;str1_count=0;}
            if (count>bcfg.listline+1||!bit(bcfg.mode,3)&&count>stop_count+1) {
                view_pos++;
                count=0;
                N_page--;
                break;
            }
        }
    }
    if (mode==1) {      //´ÙÀ½À§Ä¡
        count=0;
        fseek(fp1,view_pos,SEEK_SET);
        ii=view_pos;
        for(;view_pos<bbs.posi+bbs.size&&!feof(fp1);view_pos++) {
            ch=fgetc(fp1);if(ch=='\n'||str1_count++>(79-disp_x)) {count++;str1_count=0;}
            if (count>bcfg.listline||!bit(bcfg.mode,3)&&count>stop_count) {
                view_pos++;
                count=0;
                N_page++;
                break;
            }
        }
        if (view_pos>=bbs.posi+bbs.size) view_pos=ii;
    }

    if (view_pos>bbs.posi+bbs.size-1) view_pos=bbs.posi+bbs.size-1;
    if (view_pos<bbs.posi) view_pos=bbs.posi;
	if (N_page<1) N_page=1;

	view_jpg_ok=No;
	if (go.type=='w'&&view_num<99&&stristr(bbs.filename,".jpg")) {
		//±×¸²È­ÀÏ¸íÈ®ÀÎ
		sprintf(jpg_file,"%s/%s",go.dir,bbs.filename);
		file_of_num(jpg_file,view_num);
		view_jpg_ok=is_file(jpg_file);
	}

	sprintf(buff,"%s/%d.jpg",go.dir,bbs.num);
	if (!view_jpg_ok&&bit(bcfg.jpgmode,1)&&is_file(buff)) {
		go.type='w';
		strcpy(jpg_file,buff);
		view_jpg_ok=Yes;
	}

	//»ó´ÜÈ­¸é Ãâ·Â
	if (bcfg.pos_y1>1) xy(1,bcfg.pos_y1);
	if (set_bbs_logo("ftm",buff)) {
		if (!view_jpg_ok) strchng2(buff,".wtx",".wt2");
		view(buff);
	}

	if (view_jpg_ok) {
		disp_x=bcfg.jpg_x;disp_y=bcfg.jpg_y;
	}
	if (disp_x<1) disp_x=1;
	if (disp_y<1) disp_y=7;
	xy(disp_x,disp_y);
	if (bit(cfg.mode,13)) printf(" ");

	if (fp1==NULL) goto view_ok2;
    fseek(fp1,view_pos,SEEK_SET);
	if (feof(fp1)||bbs.size<1) {
		xy(1,10);
		goto view_ok2;
	}

	count=0;
	page=0;
	stop_count=14;		  //È­¸éÀÌ ¸ØÃß´Â ÁÙ¼ö
	temp_key=No;
    str1_count=0;
	ch=0;

    for(ii=view_pos;ii<(bbs.posi+bbs.size);ii++) {
		//´ÙÀ½ÁÙÀÌ ³ª¿Ã¶§±îÁö ¹Ýº¹ÇÏ¿© È­¸é¿¡ Ãâ·ÂÇÑ´Ù.
		ch=fgetc(fp1);
        if (ch=='') str1_count=0;
		putchar(ch);
		if (feof(fp1)) break;
		if(ch=='\n'||str1_count++>(79-disp_x)) {
            count++;
            disp_y++;
            str1_count=0;
            if (count>=bcfg.listline) break;
            if (!bit(bcfg.mode,3)&&count>stop_count) break;
            if (disp_x>1) xy(disp_x,disp_y);
            if (bit(cfg.mode,13)) printf(" ");
		}
	}

	view_ok2:

/*
    if (bbs.size>1&&!feof(fp1)&&view_pos<(bbs.posi+bbs.size)) {
		view_pos=ftell(fp1);  //º¼³»¿ë³²¾ÒÀ»¶§
	}
*/
    bbsreadall=No;           //³»¿ë³²À½
    if (bbs.size<1||view_pos>=(bbs.posi+bbs.size)) {
		bbsreadall=Yes; 	 //³»¿ë´ÙÀÐÀ½
	}
//    printf("Size:%d pos:%d VIEW=%d READALL=%d",bbs.size,view_pos,view_pos,bbsreadall);pressenter();;;;;;

	if (fp1!=NULL) fclose(fp1);
	if (is_typewtx&&view_num<99&&view_jpg_ok) {
		set_stat2(14,"È­»óÁ¤º¸ °Ë»öÁß");    //¿ÜºÎ
		printf("[s");
		mview(No,jpg_file);
		printf("[u");
		set_stat(1,"");
		if (is_same(bbs.key,"¿¬¼Ó")||is_same(bbs.key,"¸¸È­")||is_same(bbs.key,"¿¬¼Ó¸¸È­")) view_num++;
		else view_num=99;	//±×ÀÌ¿Ü´Â ¾Èº¸¿©ÁÜ
	}
	if (ch!='\n') printf("\n");    //´ÙÀ½ÁÙÇ¥½Ã
	if (disp_x>1) printf("[70D");    //´ÙÀ½ÁÙÇ¥½Ã
    if (bbs.size<1||disp_y<5) xy(1,10-1);
    if (bcfg.pos_y2>1) xy(1,bcfg.pos_y2-1);
    printf("\n");
	if (!view("%s/logo/%s.bl2",club_go.dir,go.code)) {
		set_bbs_logo("bl2",buff);
		if (!view(buff)) {
			strchng2(buff,"bl2","bl1");
			view(buff);
		}
	}

	if (is_same(bbs.key,"ÀÀ´ä")||bit(bcfg.modeok,1)) {
		sprintf(buff,"%s/logo/%s.ok2",club_go.dir,go.code);
		if (!view(buff)) view("frm/bbs/bbsok2.scr");
		i=numinput();
		if (i==1) bbs_ok(1);
		if (i==2) bbs_ok(0);
		if (i==3) bbs_ok(2);
		if (i==4) bbs_ok(3);
		if (i==5) bbs_ok(4);
	}
	if (is_same(bbs.key,"º¸·ù")||bit(bcfg.modeok,11)) {
		sprintf(buff,"%s/logo/%s.ok2",club_go.dir,go.code);
		if (!view(buff)) view("frm/bbs/bbsok2.scr");
		i=numinput();
		if (i==1) bbs_ok(2);
		if (i==2) bbs_ok(3);
		if (i==3) bbs_ok(4);
	}
	if (is_same(bbs.key,"´äº¯")&&!is_same(bbs.id,pf.id)&&(bbs_man(bbs.id)||bit(bcfg.modelist,4))) strcpy(auto_cmd,"rn");
	if (is_same(bbs.key,"´äÀå")&&!is_same(bbs.id,pf.id)&&(bbs_man(bbs.id)||bit(bcfg.modelist,4))) strcpy(auto_cmd,"rs");
	if (view_jpg_ok&&bit(bcfg.jpgmode,1)) go.type=old_gotype;
	bbs=nowbbs;
	if (!is_read_numn(bbs.num)) {
		if (is_char("v+w",go.type)) set_rec(1,14);
		else						set_rec(1,7);
	}
    _fail:
    return;
}


//°Ô½Ã¹° ÀÐÀ»½Ã Æ¯Á¤ ¹øÈ£¿¡ ÇØ´ç µÇ´Â TEXT ÀÐ¾î¿À±â
auto_bbs()
{
	char filename[300];
	char buffkey[30];
	int oldview_pos=view_pos;
	if (strlen(go.dir)<1) return;
	strcpy(buffkey,bbs.key);
	del_space(buffkey);
	if (bit(bcfg.automode,6)) sprintf(filename,"%s%s.txt",go.dir,buffkey);
	else					  sprintf(filename,"%s%d.txt",go.dir,bbs.num);
	del_space(filename);del_esc(filename);
	if (!is_file(filename)) return;
	if (bit(bcfg.automode,5)&&bbs.date==file_time(filename)&&bbs.size==file_size(filename)) return;    //fileÀÇ ÃÖÁ¾¼öÁ¤½Ã°£ °°À»¶§
	//±× ÀÌ¿ÜÀÇ °æ¿ì ÆÄÀÏ¿¡¼­ ÀÐ¾î¼­ ³»¿ëÀ¸·Î ¸¸µë
	sys_tem("bin/ed_bbs 1 '%s' '%s' '%d' '%s' '%s' '%s' %d",NOW_FILE,pf.id,NOW_REC,club_go.code,go.code,filename,bcfg.automode);
	bbs_num(bbs.num);
	view_pos=oldview_pos;
}


//°Ô½Ã¹° ÁøÀÔ½Ã Æ¯Á¤ °Ô½ÃÆÇ ³»¿ë ºÒ·¯¿À±â
auto_bbs2()
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char filename[300];
	char TNOW_FILE[300];
	char TNOW_FILED[300];
	char buffkey[30];
	BBS bbs2,bbs3;
	char key_mode;
	int  i,n,new_pos;
	int oldview_pos=view_pos;
	del_space(bcfg.inbbs);del_esc3(bcfg.inbbs);
	if (list_mode||!bit(bcfg.automode,15)||strlen(bcfg.inbbs)<1||view_pos>0) return;
	if (is_same(go.code,bcfg.inbbs)) return;
	sprintf(TNOW_FILE,"%s/menu/%s.tit",club_go.dir,bcfg.inbbs);
	sprintf(TNOW_FILED,"%s/menu/%s.txt",club_go.dir,bcfg.inbbs);
	if (!is_file(TNOW_FILE)||!is_file(TNOW_FILED)) return;
	//fileÀÇ ÃÖÁ¾¼öÁ¤½Ã°£ °°À»¶§
	if (!bit(bcfg.automode,16)&&file_time(NOW_FILED)<file_time(TNOW_FILED)) return;
	fp1=fopen(TNOW_FILE,"r");
	fp2=fopen(TNOW_FILED,"r");
	if (fp1==NULL||fp2==NULL) {mess("%s ÆÄÀÏ ¿­±â½ÇÆÐ",NOW_FILE);return;}
	c_lock(1,NOW_FILE);
	fp3=fopen(NOW_FILE,"w");
	fp4=fopen(NOW_FILED,"w");
	if (fp3==NULL||fp4==NULL) {mess("%s ÆÄÀÏ ¿­±â½ÇÆÐ",TNOW_FILE);return;}
	key_mode=bit(bcfg.automode,17);
	all_trim(bcfg.inbbskey);
	if (strlen(bcfg.inbbskey)<1) key_mode=9;  //ÀüºÎÇØ´ç
	new_pos=0;
	printf(" ## Àá½Ã¸¸ ±â´Ù·Á ÁÖ¼¼¿ä.. ÀÚ·á°»½ÅÁßÀÔ´Ï´Ù....\n");
	fflush(stdout);
	while (fread(&bbs,sizeof_bbs,1,fp1)) {
		if (bbs.del) continue;
        else if (key_mode==0&&!stristr(bbs.title,bcfg.inbbskey)) continue;
		else if (key_mode==1&&!is_same(bbs.key,bcfg.inbbskey)) continue;
		fseek(fp2,bbs.posi,SEEK_SET);
		fseek(fp4,new_pos,SEEK_SET);
		i=0;
		while (!feof(fp2)&&i<bbs.size) {
			fputc(fgetc(fp2),fp4);			   //³»¿ë¿¡ ÀúÀå
			i++;
		}
		bbs.size=i;
		bbs.posi=new_pos;
		new_pos+=bbs.size;
		if (!fwrite(&bbs,sizeof_bbs,1,fp3)) break;
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	c_lock(0,NOW_FILE);
	touch_file(NOW_FILED);
	touch_file(TNOW_FILED);
	view_pos=oldview_pos;
}


view7(char *filename,int posi,int size)
{
	FILE *fp;
    char buff[100000];
    int i=0;
    bzero(buff,100000);
    if ((fp=fopen(filename,"r"))==NULL) return No;
    fseek(fp,posi,SEEK_SET);
    while (i<100000&&i<size&&!feof(fp)) {
        buff[i++]=getc(fp);
    }
    fclose(fp);
    view3(buff);
    return Yes;
}


