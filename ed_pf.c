////////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
// ÀÚ±â½Å»ó¼öÁ¤
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

char ed_ok=No;
char ed_id_ok=No;
PF pfjob;
PF oldpf;
int edit_type;
main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	int found=No,ret,key;
	int job=0;
	int n,n1,n2;
	int direct=0;
	char now_id[10];
	char job_id[10];
	char buff[180];
	char buff1[180];
	char buff2[180];
	PF pf3;
	edit_type=No;
	strcpy(now_id,argv[1]);
	strcpy(job_id,argv[2]);
	if (argc>3) direct=atoi(argv[3]);

	findport(tty);
	read_ajob();	//Á÷¾÷ÀÐ±â
	read_atype();
	read_cfg();
	set_tmpfile();
	who=0;
	if (is_same(job_id,"edit_id_ok")) {
		strcpy(job_id,now_id);
		ed_id_ok=Yes;
	}

	if (is_same(now_id,"root")) {
		strcpy(pf.id,cfg.sysop);
		strcpy(pf.name,cfg.sysop);
		pf.type=19;
		who=5;
	}
	else {
		if (!read_pf(now_id)) {
			strcpy(pf.id,"...");
			pf.type=1;
		}
		if (pf.type==19) who=4;
	}
	pfjob=pf;	//µ¿ÀÛ½ÃÅ²ID

	if (!read_pf(job_id)) {
		printf("\n %s¿¡ ÇØ´çµÇ´Â ¾ÆÀÌµð°¡ ¾ø½À´Ï´Ù.\n",job_id);
		pressenter();
		return;
	}
	oldpf=pf;
	if (who>=4||pfjob.type==19) {
		ed_ok=Yes;
		edit_type=Yes;
		if (who<5&&pf.type>=18) edit_type=No;
	}
	if (ed_id_ok) {
		pedit_id();
		return;
	}


	if (ed_ok&&who==4&&pf.type>=18) ed_ok=No;
	if (direct==-8) {mailbook();return;}  //¸ÞÀÏBOOKÆíÁý
	if (direct==-9) {ed_ok=Yes;direct=0;}

	if (direct==0) view("frm/pf/ed_pf.log");
	while(1) {
		if (direct>0) {n=direct;strcpy(buff,"");goto _direct;}
		strcpy(g_buff1,"");
		strcpy(g_buff2,"");
		strcpy(g_buff3,"");
		strcpy(g_buff4,"");
		if (edit_type) {
			strcpy(g_buff1,"96. µî±Þ/·¹º§ ¼öÁ¤");
			strcpy(g_buff2,"97. ¼¼ºÎ ½Å»ó ¼öÁ¤");
		}
		view("frm/pf/ed_pf.scr");
		lineinput(buff,5);check_han(buff);printf("\n");
		n=atoi(buff);
		_direct:
			 if (is_char("TtswmxXpP",buff[0])) break;
		else if (is_char("qQ",buff[0])) {
			if (yesno("\n ## º¯°æÇÏ½Å ³»¿ëÀ» Ãë¼ÒÇÒ±î¿ä? (Y/n) >>",Yes)) exit(1);
			continue;
		}
		else if (n== 1) {view("frm/pf/mypf.scr");pressenter();}
		else if (n== 2) pedit_name();
		else if (n== 3) pedit_pass();
		else if (n== 4) pedit_birthday();
		else if (n== 5) pedit_addr();
		else if (n== 6) pedit_tel();
		else if (n== 7) pedit_job();
		else if (n== 8) pedit_wife();
		else if (n== 9) pedit_memo();
		else if (n==10) pedit_mail();
		else if (n==11) pedit_cfg();
		else if (n==12) pedit_pfjpg();
		else if (n==13) pedit_open();
		else if (n==14) pedit_else(1);
		else if (edit_type&&n==96) pedit_type();
		else if (edit_type&&n==97) {
			key=pedit_spf();
			if (key==No) {
				strcpy(buff,"q");
				goto _direct;
			}
		}
		else if (n==99) pedit_out();
		if (direct>0) break;
	}
	//IDº¯°æ¿©ºÎ?
	if (!is_same(oldpf.id,pf.id)) {
        change_id(oldpf,pf);
	}
	else {
		save_pf(27);
	}
	view("frm/pf/ed_pf.bye");
}


pedit_type()
{
	int n,n1;
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ µî±Þ º¯°æ >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf(" 1. µî±Þ     : %d  (%s)\n",pf.type,ajob[pf.job]);
		printf(" 2. ·¹º§     : %d      \n",pf.level);
		printf(" 3. ´Ù¿î¿ë·® : %d MB   \n",pf.down);
		printf(" 3. »çÀÌ¹öM  : %d      \n",pf.cash);
		printf(" ------------------------------------------------------------------------------\n");
		n1=numselect();
		printf("\n\n");
		xy(16,2+n1);
		if (n1<=0) break;
		if (n1==1) {
			n=get_num(pf.type,2);
			if (pf.type<18&&n<18) pf.type=n;
			else if (who==5)	  pf.type=n;
		}
		if (n1==2) pf.level=get_num(pf.level,10);
		if (n1==3) pf.down =get_num(pf.down,10);
		if (n1==4) pf.cash =get_num(pf.cash,10);
	}
}

pedit_name()
{
	int n,n1;
	char buff[200];
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ ½Å»ó º¯°æ >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf(" 1. ID             : %-8s   %s\n",pf.id,ed_ok?"(¼öÁ¤°¡´É)":"(¼öÁ¤ºÒ°¡´É)");
		printf(" 2. ¼º¸í           : %-8s   %s\n",pf.name,ed_ok||bit(cfg.ed_pfmode,0)?"(¼öÁ¤°¡´É)":"(¼öÁ¤ºÒ°¡´É)");
		printf(" 3. º°¸í           : %-8s\n",pf.nick);
		printf(" 4. ¼ºº°           : %s\n",is_char("0f¿",pf.sex)?"¿©":"³²");
		printf(" ------------------------------------------------------------------------------\n");
		n1=numselect();
		printf("\n\n");
		if (n1<=0) break;
		if (n1==1&&ed_ok) {
			view("frm/pf/id.log");
			strcpy(buff,pf.id);
			get_str(buff,8);
			del_space(buff);del_esc(buff);
			if (strlen(buff)>3&&!check_file_char(buff,"frm/guest/notid.chr"))
				strcpy(pf.id,buff);
		}
		if (n1==2&&(ed_ok||bit(cfg.ed_pfmode,0))) {
			printf(" ÇöÀç ¼º¸í : %s\n",pf.name);
			view("frm/pf/name.log");
			strcpy(buff,pf.name);
			get_str(buff,8);
			del_space(buff);del_esc(buff);
			if (strlen(buff)>3&&!check_file_char(buff,"frm/guest/notname.chr"))
				strcpy(pf.name,buff);
		}
		if (n1==3) {
			printf(" ÇöÀç º°¸í : %s\n",pf.nick);
			view("frm/pf/nick.log");
			get_str(pf.nick,8);
		}
		if (n1==4&&(ed_ok||bit(cfg.ed_pfmode,0))) {
			printf(" 1.³²ÀÚ 2.¿©ÀÚ ¼±ÅÃ>>");
			n=numinput();
			if (n==1) pf.sex='m';
			if (n==2) pf.sex='f';
		}
	}
}


check_sameid2(char *id)
{
	if (read_pf4(id)) {
		strcpy(g_buff1,pf2.id);
		strcpy(g_buff2,pf2.name);
		strcpy(g_buff3,datestr(34,pf2.login));
		return Yes;
	}
	return No;
}

//ÇöÀçÀÇ½ÅÃ»³»¿ªÀÇ Á¤´çÇÔÀ» È®ÀÎ
check_waitid(char *id)
{
	FILE *fp;
	PF pf2;
	if ((fp=fopen(GUEST_FILE,"r"))!=NULL) {
		while (fread(&pf2,sizeof_pf,1,fp)) {
			if (is_same(pf2.id,id)) {
				strcpy(g_buff1,pf2.id);
				strcpy(g_buff2,pf2.name);
				strcpy(g_buff3,datestr(34,pf2.login));
				fclose(fp);
				return Yes;
			}
		}
		fclose(fp);
	}
	return No;
}




pedit_pass()
{
	int n1;
	char buff[200];
	char buff2[200];
	strcpy(g_buff4,"");
	if (who>=4&&pf.type<19) {
		sprintf(g_buff4,"[ÇöÀç¾ÏÈ£:%s]",pf.passwd);
	}
	if (!ed_ok&&who<3) {   //½Ã»ðÀÌ ÀÏ¹ÝÈ¸¿ø°íÄ¥°æ¿ì È®ÀÎ¾ÊÇÔ
		view("frm/pf/passwd.log");
		passinput(buff,8);printf("\n");
		if (strcmp(pf.passwd,buff)) {
			view("frm/pf/passwd1.err");
			goto _fail;
		}
	}
	view("frm/pf/passwd1.log");
	passinput(buff, 8);printf("\n");
	if (strlen(buff)<3) {
		view("frm/pf/passwd2.err");
		goto _fail;
	}
	view("frm/pf/passwd2.log");
	passinput(buff2, 8);printf("\n");
	if (strcmp(buff,buff2)==0) {
		strcpy(pf.passwd,buff);
		view("frm/pf/passwdok.log");
	}
	else {
		view("frm/pf/passwd3.err");
		goto _fail;
	}
	view("frm/pf/passwd.end");
	return;
	_fail:
	view("frm/pf/passwd.err");
	return;
}


pedit_birthday()
{
	int n1;
	char buff[200];
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ ÇöÀç ±â³äÀÏ >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf(" 1. »ýÀÏ           : %s\n",dateyang(371,pf.birthday,pf.moon));
		printf(" 2. ¹è¿ìÀÚ»ýÀÏ     : %s\n",dateyang(371,pf.wbirthday,pf.wmoon));
		printf(" 3. °áÈ¥±â³äÀÏ     : %s\n",datestr(37,pf.marryday));
		printf(" ------------------------------------------------------------------------------\n");
		n1=numselect();
		printf("\n\n");
		if (n1<=0) break;
		if (n1==1) {
			view("frm/pf/birthday.log");
			printf(" ÇöÀç »ý³â¿ùÀÏ : %s\n",datestr(1,pf.birthday));
			printf("  >> »ý³â¿ùÀÏ : ");
			strcpy(buff,datestr(1,pf.birthday));
			dateinput(buff);printf("\n");
			if (strlen(buff)<5) continue;
			pf.birthday=str2date(buff);
			printf("     »ý³â¿ùÀÏÀÌ ¾ç·ÂÀÔ´Ï±î?(Y/n)>>");
			pf.moon=yesno("",Yes);
		}
		if (n1==2) {
			view("frm/pf/wbirthday.log");
			printf(" ¹è¿ìÀÚÀÇ »ý³â¿ùÀÏ : %s\n",datestr(1,pf.wbirthday));
			printf("  >> »ý³â¿ùÀÏ : ");
			strcpy(buff,datestr(1,pf.wbirthday));
			dateinput(buff);printf("\n");
			if (strlen(buff)<5) continue;
			pf.wbirthday=str2date(buff);
			printf("     »ý³â¿ùÀÏÀÌ ¾ç·ÂÀÔ´Ï±î?(Y/n)>>");
			pf.wmoon=yesno("",Yes);
		}
		if (n1==3) {
			view("frm/pf/marryday.log");
			printf(" °áÈ¥±â³äÀÏ : %s\n",datestr(1,pf.marryday));
			printf("  >> °áÈ¥±â³äÀÏ : ");
			strcpy(buff,datestr(1,pf.marryday));
			dateinput(buff);printf("\n");
			if (strlen(buff)<5) continue;
			pf.marryday=str2date(buff);
		}
	}
}


pedit_addr()
{
	int n1;
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ ÇöÀç ÁÖ¼Ò >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf(" 1. ÁÖ¼Ò           : %s\n",pf.addr);
		printf(" 2. ¿ìÆí¹øÈ£       : %s\n",pf.pos1);
		printf(" 3. ¼Ò½ÄÁöÁÖ¼Ò     : %s\n",pf.add2);
		printf(" 4. ¼Ò½ÄÁö¿ìÆí¹øÈ£ : %s\n",pf.pos2);
		printf(" ------------------------------------------------------------------------------\n");
		n1=numselect();
		if (n1<=0) break;
		printf("\n\n");
		if (n1==1) {
			view("frm/pf/address.log");
			printf("ÇöÀç:%s\n>>",pf.addr);
			get_str(pf.addr,70);
		}
		else if (n1==2) {
			printf("ÇöÀç:%s\n>>",pf.pos1);
			get_str(pf.pos1,7);
		}
		else if (n1==3) {
			view("frm/pf/address2.log");
			printf("ÇöÀç:%s\n>>",pf.add2);
			get_str(pf.add2,70);
		}
		else if (n1==4) {
			printf("ÇöÀç:%s\n>>",pf.pos2);
			get_str(pf.pos2,7);
		}
	}
}

pedit_tel()
{
	int n1;
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ ÇöÀç ¿¬¶ôÃ³ >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf(" 1. ÀüÈ­           : %s\n",pf.tele);
		printf(" 2. »ß»ß           : %s\n",pf.bibi);
		printf(" 3. ÇÚµåÆù         : %s\n",pf.hand);
		printf(" 4. Á÷Àå(ÇÐ±³)ÀüÈ­ : %s\n",pf.otel);
		printf(" ------------------------------------------------------------------------------\n");
		n1=numselect();
		printf("\n\n");
		if (n1<=0) break;
		if (n1==1) {
			view("frm/pf/tele.log");
			printf(" ÇöÀç:%s\n",pf.tele);
			get_str(pf.tele,20);
		}
		if (n1==2) {
			view("frm/pf/bibi.log");
			printf(" ÇöÀç:%s\n",pf.bibi);
			get_str(pf.bibi,14);
		}
		if (n1==3) {
			view("frm/pf/hand.log");
			printf(" ÇöÀç:%s\n",pf.hand);
			get_str(pf.hand,14);
		}
		if (n1==4) {
			view("frm/pf/otel.log");
			printf(" ÇöÀç:%s\n",pf.otel);
			get_str(pf.otel,20);
		}
	}
}


pedit_job()
{
	int n1,n2;
	while(1) {
		if (pf.job<0||pf.job>19) pf.job=0;
		if (!view("frm/pf/ed_job.scr")) {
			cls();
			printf(" << %s(%s)´ÔÀÇ ÇöÀç Á÷¾÷(±Ù¹«Ã³,ÀüÈ­¹øÈ£) >>\n",pf.id,pf.name);
			printf(" ------------------------------------------------------------------------------\n");
			printf(" 1. Á÷¾÷(Å¬·´)     : %s\n",ajob[pf.job]);
			printf(" 2. Á÷Àå(ÇÐ±³)¸í   : %s\n",pf.offi);
			printf(" 3. Á÷Àå(ÇÐ±³)ÀüÈ­ : %s\n",pf.otel);
			printf(" ------------------------------------------------------------------------------\n");
			printf(" ¼±ÅÃ>>");
		}
		n1=numselect2();
		printf("\n\n");
		if (n1<=0) break;
		if (n1==1) {
			if (!view("frm/pf/e_job.log")) {
				printf(" ÇöÀç Á÷¾÷           : %s\n",ajob[pf.job]);
			}
			if (yesno("º¯°æÇÏ½Ã°Ú½À´Ï±î?(y/N)",No)) {
				view("frm/jobsel.scr");
				n2=get_num(pf.job,2);
				if (n2>0||n2<=19) pf.job=n2;
			}
		}
		if (n1==2) {
			if (!view("frm/pf/e_offi.log")) {
				view("frm/pf/offi.log");
				printf(" ÇöÀç Á÷Àå(ÇÐ±³)¸í   : %s\n>>",pf.offi);
			}
			get_str(pf.offi,70);
		}
		if (n1==3) {
			if (!view("frm/pf/e_otel.log")) {
				view("frm/pf/otel.log");
				printf(" ÇöÀç Á÷Àå(ÇÐ±³)ÀüÈ­ : %s\n>>",pf.otel);
			}
			get_str(pf.otel,20);
		}
	}
}

pedit_wife()
{
	int n1;
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ ¹è¿ìÀÚ / °¡Á· ID ÁöÁ¤ >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf(" 1. ¹è¿ìÀÚID       : %s\n",pf.wifeid );
		printf(" ------------------------------------------------------------------------------\n");
		n1=numselect();
		if (n1<=0) break;
		printf("\n\n");
		if (n1==1) {
			view("frm/pf/wifeid.log");
			printf(" ÇöÀç ¹è¿ìÀÚ ID : %s\n >>",pf.wifeid);
			get_str(pf.wifeid,8);
		}
	}
}


pedit_memo()
{
	int n1;
	char apfuser[5][30];
	char buff[200];
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ ÇöÀç ¼Ò°³ >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf(" 1. ¼Ò°³: %s\n",pf.memo1);
		printf(" 2. ÃßÃµ: %s\n",pf.user1);
		printf(" 3. °ü½É: %s\n",pf.user2);
		printf(" 4. °æ·Â: %s\n",pf.user3);
		printf(" 5. Ãë¹Ì: %s\n",pf.user4);
		printf(" ------------------------------------------------------------------------------\n");
		n1=numselect();
		printf("\n\n");
		if (n1<=0) break;
		if (n1==1) get_str(pf.memo1,40);
		if (n1==2) get_str(pf.user1,8);
		if (n1==3) get_str(pf.user2,20);
		if (n1==4) get_str(pf.user3,20);
		if (n1==5) get_str(pf.user4,40);
	}
}


pedit_mail()
{
	int n;
	char buff[300];
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ ¸ÞÀÏ ÁöÁ¤ >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf(" 1. ¸ÞÀÏ ¼ö½Å°¡´É    : %s\n",bit(pf.mymode,12)?"°Å ºÎ":"°¡ ´É");
		printf(" 2. EMAIL¼ö½Å°¡´É    : %s\n",bit(pf.mymode,13)?"°Å ºÎ":"°¡ ´É");
		printf(" 3. ¸ÞÀÏ ´ë¸® ¼ö½ÅID : %s\n",pf.tmail);
		printf(" 4. EMAILÀü¿ë ¿µ¹®ID : %s\n",pf.email);
		printf(" 5. EMAIL ¼ö½ÅID     : %s\n",pf.mailid);
		printf(" 6. µ¿º¸¸ÞÀÏ ÁÖ¼Ò·Ï ÁöÁ¤\n");
		printf(" ------------------------------------------------------------------------------\n");
		n=numselect();
		printf("\n\n");
		if (n<=0) break;
		if (n==1) pf.mymode=set_bit(pf.mymode,12,bit(pf.mymode,12)?0:1);
		if (n==2) pf.mymode=set_bit(pf.mymode,13,bit(pf.mymode,13)?0:1);
		if (n==3) {
			view("frm/pf/mailid.log");
			printf("  ¸ÞÀÏ ´ë¸® ¼ö½ÅID : %s\n>>",pf.tmail);
			get_str(pf.tmail,8);
		}
		if (n==4) {
			printf("  EMAIL Àü¿ë¿µ¹®ID : %s\n>>",pf.email);
			get_str(pf.email,8);
		}
		if (n==5) {
			view("frm/pf/email.log");
			printf("  EMAIL ¼ö½Å ID    : %s\n>>",pf.mailid);
			get_str(pf.mailid,40);
		}
		if (n==6) mailbook();
		del_space(pf.mailid);del_esc3(pf.mailid);
		del_space(pf.email);del_esc3(pf.email);
	}
}

pedit_cfg()
{
	int n;
	char filename[200];
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ °³ÀÎ ¼³Á¤ >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf(" 1. ¸Þ¼¼Áö ¼ö½Å°¡´É  : %s\n",rbit(pf.mymode,14,"°¡ ´É","°Å ºÎ"));
		printf(" 2. ´ëÈ­½Ç ÃÊ´ë°¡´É  : %s\n",rbit(pf.mymode,15,"°¡ ´É","°Å ºÎ"));
		printf(" 3. ÇÑ¿µ ÀÚµ¿ º¯È¯   : %s\n",rbit(pf.mymode,16,"ÀÚ µ¿","¼ö µ¿"));
		printf(" 4. ¸ÞÀÏµµÂø½Ã ¼Ò¸®  : %s\n",rbit(pf.mymode,17,"³¿   ","¾È ³¿"));
		printf(" 5. ÀüÀÚ »ç¼­ÇÔ È®ÀÎ : %s\n",rbit(pf.mymode,18,"ÇÔ   ","¾È ÇÔ"));
		printf(" 7. ½ÃÀÛ½Ã Àá±ñµµ¿ò¸»: %s\n",rbit(pf.mymode,10,"º¸ÀÓ","¾Èº¸ÀÓ"));
		printf(" 8. È­¸é ¿¡´Ï¸ÞÀÌ¼Ç  : %s\n",rbit(pf.mymode,21,"º¸ÀÓ","¾Èº¸ÀÓ"));
		printf(" 9. ÀÚµ¿ ´º½º ¼Óº¸   : %s\n",rbit(pf.mymode,22,"º¸ÀÓ","¾Èº¸ÀÓ"));
		printf("10. ¾È½Ã È­¸é ÁöÁ¤   : %s\n",rbit(pf.mymode,6 ,"º¸ÀÓ","¾Èº¸ÀÓ"));
		printf("11. ÆíÁý±â ±äÁÙ Ã³¸® : %s\n",rbit(pf.mymode,11,"ÀÚµ¿ÁÙ³Ñ±è","¼öµ¿ÁÙ³Ñ±è"));
		printf("12. ¸Þ¼¼ÁöµµÂø½Ã¼Ò¸® : %s\n",rbit(pf.mymode,23,"³¿   ","¾È ³¿"));
		printf("18. ÃÊ±â È­¸é ¼³Á¤   : %s\n",strlen(pf.gofirst)<1?"top":pf.gofirst);
		printf("19. Å°ÀÔ·Â Á¦ÇÑ½Ã°£  : %d ºÐ\n",pf.offmin);
		printf("20. °³ÀÎº° ´ÜÃà¸í·É¾î ÆíÁý \n");
		printf(" ------------------------------------------------------------------------------\n");
		n=numselect();
		printf("\n\n");
		if (n<=0) break;
		if (n==1) pf.mymode=gtt_bit(pf.mymode,14);
		if (n==2) pf.mymode=gtt_bit(pf.mymode,15);
		if (n==3) pf.mymode=gtt_bit(pf.mymode,16);
		if (n==4) pf.mymode=gtt_bit(pf.mymode,17);
		if (n==5) pf.mymode=gtt_bit(pf.mymode,18);
		if (n==6) pf.mymode=gtt_bit(pf.mymode,20);
		if (n==7) pf.mymode=gtt_bit(pf.mymode,10);
		if (n==8) pf.mymode=gtt_bit(pf.mymode,21);
		if (n==9) pf.mymode=gtt_bit(pf.mymode,22);
		if (n==10) pf.mymode=gtt_bit(pf.mymode,6);
		if (n==11) pf.mymode=gtt_bit(pf.mymode,11);
		if (n==12) pf.mymode=gtt_bit(pf.mymode,23);
		if (n==18) {
			view("frm/pf/gofirst.log");
			printf("  ÃÊ±â È­¸é ¼³Á¤   : %s\n",strlen(pf.gofirst)<1?"top":pf.gofirst);
			get_str(pf.gofirst,12);
		}
		if (n==19) {
			view("frm/pf/offmin.log");
			printf("  Å°ÀÔ·Â Á¦ÇÑ½Ã°£  : %d ºÐ  (0:±âº»¼³Á¤)  >>",pf.offmin);
			pf.offmin=get_num(pf.offmin,3);
		}
		if (n==20) {
			view("frm/pf/shordcmd.log");
			sprintf(filename,"userdata/%s/%s/shortcmd.dat",c_han(pf.id),pf.id);
			xedit(filename);
			del_emptyline(filename);	 //ºóÁÙÁö¿ò
			if (file_size(filename)<10) unlink(filename);
		}
	}
}

pedit_open()
{
	int n;
	view("frm/pf/open.log");
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ ÇöÀç °ø°³/ºñ°ø°³ ¼±ÅÃÇöÈ² >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf("    1. »ýÀÏ : %s\n",rbit(pf.myflag, 1,"ºñ°ø°³","°ø  °³"));
		printf("    2. ¼ºº° : %s\n",rbit(pf.myflag, 2,"ºñ°ø°³","°ø  °³"));
		printf("    3. ¿ìÆí : %s\n",rbit(pf.myflag, 3,"ºñ°ø°³","°ø  °³"));
		printf("    4. ÁÖ¼Ò : %s\n",rbit(pf.myflag, 4,"ºñ°ø°³","°ø  °³"));
		printf("    5. ¹ß¼Û : %s\n",rbit(pf.myflag, 5,"ºñ°ø°³","°ø  °³"));
		printf("    6. ÁÖ¼Ò : %s\n",rbit(pf.myflag, 6,"ºñ°ø°³","°ø  °³"));
		printf("    7. ÀüÈ­ : %s\n",rbit(pf.myflag, 7,"ºñ°ø°³","°ø  °³"));
		printf("    8. »ß»ß : %s\n",rbit(pf.myflag, 8,"ºñ°ø°³","°ø  °³"));
		printf("    9. ÈÞ´ë : %s\n",rbit(pf.myflag, 9,"ºñ°ø°³","°ø  °³"));
		printf("   10. Á÷¾÷ : %s\n",rbit(pf.myflag,10,"ºñ°ø°³","°ø  °³"));
		printf("   11. Á÷Àå : %s\n",rbit(pf.myflag,11,"ºñ°ø°³","°ø  °³"));
		printf("   12. ÀüÈ­ : %s\n",rbit(pf.myflag,12,"ºñ°ø°³","°ø  °³"));
		printf("   13. ¼Ò°³ : %s\n",rbit(pf.myflag,13,"ºñ°ø°³","°ø  °³"));
		printf("   14. ÃßÃµ : %s\n",rbit(pf.myflag,14,"ºñ°ø°³","°ø  °³"));
		printf("   15. °ü½É : %s\n",rbit(pf.myflag,15,"ºñ°ø°³","°ø  °³"));
		printf("   16. °æ·Â : %s\n",rbit(pf.myflag,16,"ºñ°ø°³","°ø  °³"));
		printf("   17. Ãë¹Ì : %s\n",rbit(pf.myflag,17,"ºñ°ø°³","°ø  °³"));
		printf(" -----------------------------------------------------------------------------\n");
		n=numselect();
		if (n<= 0) break;
		if (n== 1) pf.myflag=gtt_bit(pf.myflag, 1);
		if (n== 2) pf.myflag=gtt_bit(pf.myflag, 2);
		if (n== 3) pf.myflag=gtt_bit(pf.myflag, 3);
		if (n== 4) pf.myflag=gtt_bit(pf.myflag, 4);
		if (n== 5) pf.myflag=gtt_bit(pf.myflag, 5);
		if (n== 6) pf.myflag=gtt_bit(pf.myflag, 6);
		if (n== 7) pf.myflag=gtt_bit(pf.myflag, 7);
		if (n== 8) pf.myflag=gtt_bit(pf.myflag, 8);
		if (n== 9) pf.myflag=gtt_bit(pf.myflag, 9);
		if (n==10) pf.myflag=gtt_bit(pf.myflag,10);
		if (n==11) pf.myflag=gtt_bit(pf.myflag,11);
		if (n==12) pf.myflag=gtt_bit(pf.myflag,12);
		if (n==13) pf.myflag=gtt_bit(pf.myflag,13);
		if (n==14) pf.myflag=gtt_bit(pf.myflag,14);
		if (n==15) pf.myflag=gtt_bit(pf.myflag,15);
		if (n==16) pf.myflag=gtt_bit(pf.myflag,16);
		if (n==17) pf.myflag=gtt_bit(pf.myflag,17);
	}
}



pedit_pfjpg()
{
	char afile[10][200];
	char buff[200];
	char filename[200];
	int n;
	sprintf(afile[0],"userdata/%s/%s/pfinfo.scr",c_han(pf.id),pf.id);
	sprintf(afile[1],"userdata/%s/%s/pfinfo.jpg",c_han(pf.id),pf.id);
	sprintf(afile[2],"userdata/%s/%s/pfinfo.sig",c_han(pf.id),pf.id);
	sprintf(afile[3],"userdata/%s/%s/pfinfo.mai",c_han(pf.id),pf.id);
	sprintf(afile[4],"userdata/%s/%s/pfinfo.vtx",c_han(pf.id),pf.id);
	sprintf(afile[5],"userdata/%s/%s/pfinfo.say",c_han(pf.id),pf.id);
	sprintf(afile[6],"userdata/%s/%s/pfinfo.sho",c_han(pf.id),pf.id);
	sprintf(afile[7],"userdata/%s/%s/pfinfo.wav",c_han(pf.id),pf.id);
	view("frm/pf/pfjpg.log");
	while(1) {
		cls();
		printf(" << %s(%s)´ÔÀÇ °³ÀÎ ¼Ò°³ >>\n",pf.id,pf.name);
		printf(" ------------------------------------------------------------------------------\n");
		printf("  11.°³ÀÎ¼Ò°³ È®ÀÎ   12.º¯°æ   13.µî·Ï   14.´Ù¿î   15:»èÁ¦    (%8d byte)\n",file_size(afile[0]) );
		printf("  21.°³ÀÎ»çÁø È®ÀÎ             23.µî·Ï   24.´Ù¿î   25:»èÁ¦    (%8d byte)\n",file_size(afile[1]) );
		printf("  31.°³ÀÎ»çÀÎ È®ÀÎ   32.º¯°æ   33.µî·Ï   34.´Ù¿î   35:»èÁ¦    (%8d byte)\n",file_size(afile[2]) );
		printf("  41.¸ÞÀÏ¾È³» È®ÀÎ   42.º¯°æ   43.µî·Ï   44.´Ù¿î   45:»èÁ¦    (%8d byte)\n",file_size(afile[3]) );
		printf("  51.°³ÀÎVTX  È®ÀÎ             53.µî·Ï   54.´Ù¿î   55:»èÁ¦    (%8d byte)\n",file_size(afile[4]) );
		printf("  61.°³ÀÎSAY  È®ÀÎ   62.º¯°æ   63.µî·Ï   64.´Ù¿î   65:»èÁ¦    (%8d byte)\n",file_size(afile[5]) );
		printf("  71.°³ÀÎ´ÜÃàÅ°È®ÀÎ  72.º¯°æ   73.µî·Ï   74.´Ù¿î   75:»èÁ¦    (%8d byte)\n",file_size(afile[6]) );
		printf("  81.°³ÀÎWAV  È®ÀÎ             83.µî·Ï   84.´Ù¿î   85:»èÁ¦    (%8d byte)\n",file_size(afile[7]) );
		printf(" -----------------------------------------------------------------------------\n");
		n=numselect();
		if (n<=0) break;
		if (n>=11&&n<=75) sprintf(filename,afile[n/10-1]);
		else continue;
			 if (n%10==1) {mview(1,filename);pressenter();}
		else if (n==12||n==32||n==42||n==62||n==72) xedit(filename);
		else if (n%10==3&&rz_file(buff)) file_move(buff,filename);
		else if (n%10==4) sz_file(filename);
		else if (n%10==5&&yesno("ÇØ´ç ÀÚ·á¸¦ »èÁ¦ÇÒ±î¿ä?(Y/n)>>",Yes)) unlink(filename);
		if (n>=11&&n<=85&&is_file(filename)&&file_size(filename)<4) unlink(filename);
	}
}

pedit_else(int mode)
{
	char afile[7][200];
	char buff[200];
	char filename[200];
	int n;
	if (mode==1) {
		sprintf(filename,"userdata/%s/%s/usman.dat",c_han(pf.id),pf.id);
		xedit(filename);
	}
}



//°¡ÀÔÇØÁö
pedit_out()
{
	int mode;
	int now,found,ret;
	int pass_edit=No;
	char now_id[10];
	char buff[80];
	char buff2[80];
	FILE *fp;

	cls();
	view_pf(pf.id);
	xy(0,21);
	if (pf.type>=14) {
		printf(" %s (%s)´ÔÀº Æ¯º°È¸¿øÀ¸·Î ÁöÁ¤µÇ¾î ÀÖÀ¸¹Ç·Î\n",pf.id,pf.name);
		printf(" ¿î¿µÁøÀÇ Çã¶ôÀÌ ÀÖ¾î¾ß °¡ÀÔÇØÁö ½ÅÃ»ÀÌ °¡´ÉÇÕ´Ï´Ù.\n");
		printf(" ´Ù½Ã ÇÑ¹ø È®ÀÎ ÇÏ½Ê½Ã¿ä.\n");
		printfL2();
		pressenter();
		return;
	}

	if (pf.type==0) {
		printf(" %s (%s)´ÔÀº °¡ÀÔÇØÁö »óÅÂÀÔ´Ï´Ù.\n",pf.id,pf.name);
		printf(" ´Ù½Ã ÇÑ¹ø È®ÀÎ ÇÏ½Ê½Ã¿ä.\n");
		printfL2();
		pressenter();
		return;
	}

	ret=No;
	if (pf.id_no[0]==0||bit(cfg.guestin,3)) goto _ok;
	else {
		printf(" °¡ÀÔ ÇØÁö ½ÅÃ»ÀÚÀÇ ÁÖ¹Îµî·Ï ¹øÈ£¸¦ È®ÀÎÇÕ´Ï´Ù.\n");
		printf(" ¿¹)701231-1234123  ÀÔ·Â >>");
		lineinput(buff,14);
		del_space(pf.id_no);
		del_space(buff);
		printf("\n");
		if (is_same(pf.id_no,buff)) goto _ok;
	}
	goto _fail;

	_ok:
	printfL2();
	printf(" %s(%s)´ÔÀÇ °¡ÀÔÇØÁö½ÅÃ»À» ÇÏ¸é ÀÏÁ¤±â°£ Àç°¡ÀÔÀÌ ±ÝÁöµË´Ï´Ù.\n",pf.id,pf.name);
	printf("¶ÇÇÑ ¿î¿µÁøÀÇ Çã¶ôÀÌ ÀÖ¾î¾ß »ç¿ëÀÌ °¡´ÉÇÕ´Ï´Ù.\n");
	if (!yesno("°¡ÀÔÇØÁö½ÅÃ»À» ÇÏ½Ã°Ú½À´Ï±î?",No)) goto _fail;
	pf.type=0;	 //½ÅÃ»ÀÚÀÇ Å¸ÀÔÀ» 0À¸·Î ¸¸µç´Ù.
	if (bit(cfg.mode,19)) strcpy(pf.id_no,"");
	save_pf(28);
	printfL2();
	printf(" %s (%s)´ÔÀº °¡ÀÔÀÌ ÇØÁöµÇ¾ú½À´Ï´Ù.\n",pf.id,pf.name);
	printf(" ¸¸ÀÏ ÀÏÁ¤±â°£³»¿¡ ´Ù½Ã ¼­¾îºñ½º¸¦ »ç¿ëÇÏ½Ã·Á¸é\n");
	printf(" ¼Õ´ÔÀ¸·Î Á¢¼ÓÇØ¼­ ¼Õ´ÔÀü¿ë°Ô½ÃÆÇ¿¡ Àû¾îÁÖ¼¼¿ä.\n");
	printf(" ±×µ¿¾È »ç¿ëÇØÁÖ¼Å¼­ °¨»çÇÕ´Ï´Ù.\n");
	printfL2();
	pressenter();
	return;

	_fail:
	printf("\n");
	printfL2();
	printf(" %s (%s)´ÔÀº °¡ÀÔÇØÁö½ÅÃ»ÀÌ µÇÁö ¾Ê¾Ò½À´Ï´Ù.\n",pf.id,pf.name);
	printf(" ´Ù½Ã È®ÀÎÇÏ½Ê½Ã¿ä. \n");
	printfL2();
	pressenter();
}




pedit_spf()
{
	FILE *fp;
	int i,i2,n;
	char buff[200];
	struct termio term_old;

	ioctl(0, TCGETA, &term_old);
	rawmode();	// ³Í Ä³³ë´ÏÄ® ¸ðµå

re_start:
	cls();
	disp_pf9();
	strcpy(buff,pf.id);
	xy(15,3);lineinput(buff, 8);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) strcpy(pf.id,buff);
	strcpy(buff,pf.name);
	xy(15,4);lineinput(buff, 8);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) strcpy(pf.name,buff);
	if (who>4||pf.type<18) {
		strcpy(buff,pf.passwd);
		xy(15,5);lineinput(buff, 8);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
		if (strlen(buff)>0) strcpy(pf.passwd,buff);
		sprintf(buff,"%d",pf.type);
		xy(15,6);lineinput(buff, 3);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
		if (strlen(buff)>0) pf.type=atoi(buff);
		sprintf(buff,"%d",pf.level);
		xy(15,7);lineinput(buff, 8);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
		if (strlen(buff)>0) pf.level=atoi(buff);
	}
	disp_pf9();
	sprintf(buff,"%d",pf.down);
	xy(15,8);lineinput(buff,12);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) pf.down=atoi(buff);
	sprintf(buff,"%d",pf.cash);
	xy(15,9);lineinput(buff,12);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) pf.cash=atoi(buff);
	sprintf(buff,"%s",pf.addr);
	xy(15,10);lineinput(buff,70);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) strcpy(pf.addr,buff);
	sprintf(buff,"%s",pf.tele);
	xy(15,11);lineinput(buff,20);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) strcpy(pf.tele,buff);
	sprintf(buff,"%s",pf.bibi);
	xy(15,12);lineinput(buff,14);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) strcpy(pf.bibi,buff);
	sprintf(buff,"%s",pf.hand);
	xy(15,13);lineinput(buff,14);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) strcpy(pf.hand,buff);
	sprintf(buff,"%s",pf.id_no);
	xy(15,14);lineinput(buff,14);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) strcpy(pf.id_no,buff);
	xy(15,15);lineinput(buff,14);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (strlen(buff)>0) pf.birthday=str2date(buff);
	xy(15,16);lineinput(buff,14);if (is_same(buff,"w")||is_same(buff,"q")||is_same(buff,"r")) goto _cont;
	if (is_char("mf",buff[0])) pf.sex=buff[0];

	xy(15,17);get_str(pf.memo1,40);
	xy(15,18);get_str(pf.user1,8);
	xy(15,19);get_str(pf.user2,20);
	xy(15,20);get_str(pf.user3,20);
	xy(15,21);get_str(pf.user4,40);

	strcpy(buff,"");

_cont:
	if (is_same(buff,"r")) goto re_start;
	ioctl(0, TCSETA, &term_old);
	printf("\n");
	if (is_same(buff,"q"))return No;
	return Yes;
}

disp_pf9()
{
	cls();
	printf("[0;1m << %s(%s)´ÔÀÇ ½Å»ó º¯°æ È¸¿ø¹øÈ£ : %d >>\n",pf.id,pf.name,pf.num+1);
	printf("¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬\n");
	printf(" 1. ID      : %-8s\n",pf.id);
	printf(" 2. ¼º¸í    : %-8s\n",pf.name);
	printf(" 3. ¾ÏÈ£    : %-8s\n",pf.type>=18?starlen(pf.passwd):pf.passwd);
	printf(" 4. µî±Þ    : %d\n",pf.type);
	printf(" 5. ·¹º§    : %d\n",pf.level);
	printf(" 6. ´Ù¿î¿ë·®: %d\n",pf.down);
	printf(" 7. »çÀÌ¹öM : %d\n",pf.cash);
	printf(" 8. ÁÖ¼Ò    : %s\n",pf.addr);
	printf(" 9. ÀüÈ­    : %s\n",pf.tele);
	printf("10. »ß»ß    : %s\n",pf.bibi);
	printf("11. ÇÚµåÆù  : %s\n",pf.hand);
	printf("12. ÁÖ¹Îµî·Ï: %s\n",pf.id_no);
	printf("13. »ý³â¿ùÀÏ: %s\n",datestr(1,pf.birthday));
	printf("14. ¼ºº°    : %c\n",pf.sex );
	printf("15. Âü°í»çÇ×: %s\n",pf.memo1);
	printf("16. Âü°í»çÇ×: %s\n",pf.user1);
	printf("17. Âü°í»çÇ×: %s\n",pf.user2);
	printf("18. Âü°í»çÇ×: %s\n",pf.user3);
	printf("19. Âü°í»çÇ×: %s\n",pf.user4);
	printf("¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬\n");
	xy(46,3);printf("°¡ÀÔÀÏ½Ã : %s\n",datestr(2,pf.inday));
	xy(46,4);printf("ÃÖÁ¾Á¢¼Ó : %s\n",datestr(2,pf.logout));
	xy(46,5);printf("»ç¿ë½Ã°£ : %s\n",datestr(21,pf.all_time));
	xy(46,6);printf("ÃÑÁ¢¼Ó¼ö : %d\n",pf.all_log);
	xy(46,7);printf("¿ù»ç¿ë   : %s\n",datestr(21,pf.month_time));
	xy(46,8);printf("¿ùÁ¢¼Ó¼ö : %d\n",pf.month_log);
	xy(1,18);
}

rawmode()
{
	struct termio tdes;
	ioctl(0,TCGETA,&tdes);	 //ÇöÀç ÅÍ¹Ì³¯ ¸ðµå¸¦ ÀÐ¾î¿Â´Ù
	tdes.c_iflag = 0;
	tdes.c_iflag |= ICRNL ;
	tdes.c_iflag |= IXON ;
	tdes.c_oflag = 0;
	tdes.c_oflag |= OPOST ;
	tdes.c_oflag |= ONLCR ;
	tdes.c_cflag |= CRTSCTS ;
	tdes.c_cflag |= HUPCL ;
	tdes.c_lflag = 0;
	tdes.c_lflag |= IEXTEN;
	tdes.c_cc[VMIN]=1;
	tdes.c_cc[VTIME]=0;
	ioctl(0,TCSETA,&tdes);	 //ÇöÀç ÅÍ¹Ì³¯ ¸ðµå¸¦ ÀúÀåÇÑ´Ù.
}



mailbook()
{
	FILE *fp;
	int i,n,n2;
	char buff[300],filename[300],namefile[300];
	char afilename[51][300];
	char aname[51][41];
	if (pf.type<1) return;
	sprintf(buff,"userdata/%s/%s",c_han(pf.id),pf.id);
	check_dir(buff);
	for (i=0;i<50;i++) strcpy(aname[i],"");
	sprintf(namefile,"userdata/%s/%s/bookname.dat",c_han(pf.id),pf.id);
	if ((fp=fopen(namefile,"r"))!=NULL) {
		for (i=1;i<50;i++)
			if (fread(&buff,sizeof(aname[1]),1,fp))
				strcpy(aname[i],buff);
		fclose(fp);
	}
	for (i=1;i<26;i++) sprintf(afilename[i],"userdata/%s/%s/book%d.dat",c_han(pf.id),pf.id,i);  //°³ÀÎÁÖ¼Ò·ÏÈ­ÀÏ ¿ÀÇÂ
	while (1) {
		cls();
		printf("  %s (%s) ´Ô ¸ÞÀÏ ÁÖ¼Ò·Ï ÆíÁý    (ÁÖ¼Ò·Ï»ç¿ë: #<¹øÈ£>)\n",pf.id,pf.name);
		printfL2();
		for (i=1;i<=40;i++) {
			n=file_size(afilename[i]);
			if (i>=21) xy(42,i%21+3);
			printf("%2d. %-26.26s%5d B\n",i,aname[i],n);

		}
		xy(1,24);
		printf(" º¸±â(¹øÈ£)  ÆíÁý(E ¹øÈ£)  »èÁ¦(D ¹øÈ£)  µî·Ï(UP ¹øÈ£)  ´Ù¿î(DN ¹øÈ£) >>");
		lineinput(buff,20);check_han(buff);check_cmd(buff);printf("\n");
		n=atoi(buff);n2=mmn[1];
		if (is_quit(buff)) break;
		else if (n>0&&n<50) view_text(afilename[n]);
		else if (is_same(mmc[0],"e")&&n2>0&&n2<50) {
			printf("%d.¹ø¸ÞÀÏ ¼³¸í>>",n2);
			dosinput(aname[n2],26);
			xedit2(afilename[n2]);
		}
		else if (is_same(mmc[0],"d")&&n2>0&&n2<26&&yesno2("»èÁ¦(Y/n)>>")) unlink(afilename[n2]);
		else if (is_same(mmc[0],"dn")&&n2>0&&n2<26) sz_file(afilename[n2]);
		else if (is_same(mmc[0],"up")&&n2>0&&n2<26) rz_file(afilename[n2]);
	}
	if ((fp=fopen(namefile,"w"))!=NULL) {
		for (i=1;i<50;i++) fwrite(&aname[i],sizeof(aname[1]),1,fp);
		fclose(fp);
	}
}


pedit_id()
{
	char buff[200],buff1[200],buff2[200];
    PF oldpf2;
	time_t t,date1,date2;
	if (!file_line(buff,"system/ed_id.dat",1)) goto _fail;
	sscanf(buff,"%s%s",buff1,buff2);
	date1=str2date2(buff1);
	date2=str2date2(buff2);
    oldpf2=pf;
    pf=oldpf;
	time(&t);
	if (t<date1||t>date2) goto _fail;
    if (pf.type<1||pf.type>=18) {mess("º¯°æ°¡´Éµî±ÞÀÌ ¾Æ´Õ´Ï´Ù.");goto _fail;}
	if (!view("frm/pf/ed_pf1.log")) printf("ID º¯°æ?(y/N)>>");
	if (!yesno2("",No)) goto _fail;
	if (!view("frm/pf/ed_pf2.log")) printf("ÁÖ¹Îµî·Ï¹øÈ£ >>");
	lineinput2(buff1,20);
	strchng2(buff1,"-","");del_space(buff1);
	strcpy(buff2,pf.id_no);
	strchng2(buff2,"-","");del_space(buff2);
    if (!is_same(buff1,buff2)) {mess("ÁÖ¹Îµî·Ï¹øÈ£¿À·ù");goto _fail;}
	if (!view("frm/pf/ed_pf3.log")) printf("ÇöÀçºñ¹Ð¹øÈ£ >>");
	passinput(buff1,20);
    if (!is_same(buff1,pf.passwd)) {mess("ºñ¹Ð¹øÈ£¿À·ù");goto _fail;}
	if (!view("frm/pf/ed_pf4.log")) printf("\n º¯°æÇÒID >>");
	lineinput2(buff1,8);
    if (strlen(buff1)<3) goto _fail2;
	if (!view("frm/pf/ed_pf5.log")) printf("º¯°æÇÒID È®ÀÎ >>");
	lineinput2(buff2,8);
    if (!is_same(buff1,buff2)) {mess("ID È®ÀÎ¿À·ù");goto _fail2;}
    if ( is_same(buff1,pf.id)) {mess("ID µ¿ÀÏ");goto _fail2;}
	del_space(buff1);
	del_esc(buff1);
	if (strlen(buff1)<3) goto _fail2;
    if (check_file_char(buff1,"frm/guest/notid.chr")) {mess("»ç¿ë±ÝÁö¹®ÀÚ");goto _fail2;}
    if (check_sameid2(buff1))  {mess("»ç¿ëÁß ID");goto _fail2;}
    if (check_waitid(buff1))   {mess("½ÅÃ»Áß ID");goto _fail2;}
	if (!view("frm/pf/ed_pf6.log")) printf("º¯°æÈ®ÀÎ(Y/n)>>");
	if (!yesno2("",Yes)) goto _fail;
    strcpy(pf.id,buff1);
    change_id(oldpf,pf);
	if (!view("frm/pf/ed_pf7.log")) printf("º¯°æ¿Ï·á");
	sprintf(buff,"%s %-8s -> %-8s %s",now_str(2),oldpf.id,pf.id,pf.id_no);
	put_line(buff,"user/rec/ed_id.log");
	return;

	_fail:
	if (!view("frm/pf/ed_pf8.log")) printf("º¯°æ ½ÇÆÐ");
    pf=oldpf2;
	return;

	_fail2:
	if (!view("frm/pf/ed_pf9.log")) printf("ID È®ÀÎ");
    pf=oldpf2;
	return;
}


change_id(PF oldpf,PF newpf)
{
    FILE *fp;
    char buff[100];
    char buff1[100];
    char buff2[100];
    int found,n;
    PF pf3;
    if (!read_pf2(oldpf.id)) return;
    oldpf=pf2;
    del_esc(oldpf.id);del_space(oldpf.id);
    del_esc(newpf.id);del_space(newpf.id);
    if (strcmp(oldpf.id,newpf.id)==0) return;
    if (strlen(oldpf.id)<3) return;
    if (strlen(newpf.id)<3) return;
    found=No;
    if ((fp=fopen(ID_FILE,"r+"))!=NULL) {
        fseek(fp,oldpf.num*sizeof_pf,SEEK_SET);
        if(fread(&pf3,sizeof_pf,1,fp)&&
           strlen(pf3.id)>1&&
           is_same(pf3.id,oldpf.id)&&
           pf3.type>0&&pf3.type<=19&&
           oldpf.num>=0) {
            fseek(fp,oldpf.num*sizeof_pf,SEEK_SET);
            strcpy(pf3.id,newpf.id);
            fwrite(&pf3,sizeof_pf,1,fp);
            found=Yes;
        }
        fclose(fp);
    }
    if (found) {
        for (n=0;n<3;n++) {
            strcpy(buff2,"userdata");
            if (n==1) strcpy(buff2,"userroom");
            sprintf(buff1,"%s/%s/%s",buff2,c_han(oldpf.id),oldpf.id);
            if (is_file(buff1)) {
                check_dir("%s/%s",buff2,c_han(newpf.id));
                sprintf(buff,"%s/%s/%s",buff2,c_han(newpf.id),newpf.id);
                if (is_file(buff)) sys_tem("cp -arf %s/* %s/%s/%s/",buff1,buff2,c_han(newpf.id),newpf.id);
                else               sys_tem("cp -arf %s %s/%s/%s",buff1,buff2,c_han(newpf.id),newpf.id);
                sys_tem("rm -rf %s",buff1);
            }
        }
        make_ind(1,oldpf.id);
        if (oldpf.id[0]!=newpf.id[0]) make_ind(1,newpf.id);
    }
}
