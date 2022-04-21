////////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
// ³»¿ë : ½Ã»ð¸Þ´º °ü·Ã ·çÆ¾
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include "chat.h"

int n,nn,n2,num;
int top;
int total;
int last;
int key;
long lastnow=0; 	  //¸¶Áö¸· ½Ã°£
long lastnow2=0;	   //¸¶Áö¸· ½Ã°£
int ast[20];
int disp_mode;
int mrand;
char ss_1[50];
char ss_2[50];
char ss_3[50];

char menu_file[200];
char filename[200];
char now_code[200];
char now_dir[200];
char now_id[9];

#include "sysoput.c"     //º¸¿Ïº¯¼ö

main(int argc, char **argv)
{
	char buff[90];
	char buff2[90];
	int key;
	int num,n,i;
	FILE *fp;
	struct termio term_old;
	who=0;
	c_proc=is_file("system/c_proc.tag");
	c_proc2=is_file("system/c_proc2.tag");
	c_proc3=is_file("system/c_proc3.tag");
	for(i=0;i<20;i++) ast[i]=0;

	printf("\n<%s>\n\n\n\n",VER1);
	fflush(NULL);

	if (!is_file(ID_FILE)) {   //root¸ðµå·Î ¼³Á¤ ±×³Éexec
		mess("%s ÆÄÀÏÀÌ ¾ø½À´Ï´Ù.",ID_FILE);
		if (yesno(" IDµð·ºÅä¸®¿Í ºóIDÆÄÀÏÀ» ¸¸µé±î¿ä?",Yes)) {
			check_dir("id");
			touch_file(ID_FILE);
			make_ind(0,"make_ind_all");
		}
		return;
	}


	check_init_dir();

	ioctl(0,TCGETA,&termold);	//¿ø·¡ ÅÍ¹Ì³¯ ¸ðµå º¹±¸
	srand(time(NULL));			//³­¼öÀÇ ÃÊ±âÈ­
	mrand=rand()%10000000;
	club_mode=No;
	strcpy(club_go.dir,"./");
	strcpy(club_go.code,"main");
	setenv("TERM","vt100",1);
	setenv("LANG","ko",1);       //±¹°¡ ÁöÁ¤
	disp_mode=0;
	findport(tty);
	read_cfg();
	if (strlen(cfg.sysop)<3) strcpy(cfg.sysop,"sysop");
	set_tmpfile();
	club_mode=No;
	read_pftype();	//È¸¿øÀÇ µî±Þ,ÀÏÀÏ»ç¿ë°¡´É½Ã°£À» È®ÀÎÇÑ´Ù.
	read_atype();	//µî±ÞÀÐ±â
	read_ajob();	//Á÷¾÷ÀÐ±â
	if (argc<2) {
		if (!check_pass("zz",8,"PASS >>")) return;
	}

	if (argc<2||(argc==2&&strlen(argv[1])<3)) {
		printf("Usage:bin/sysop ????\n");
		if (yesno("ÀÎµ¦½ºÀçÁ¤¸®¸¦ ÇÒ±î¿ä? (Y/n) >>",Yes)) {
			printf("\n ÀÎµ¦½ºÆÄÀÏÀ» ÀçÁ¤¸®ÇÕ´Ï´Ù...\n");
			make_ind(0,"make_ind_all");
		}
		return;
	}
	else if (argc==2&&is_same(argv[1],"root")) {
		strcpy(now_id,cfg.sysop);
		printf("\n ### ÇöÀç ½©Àü¿ë ½Ã»ð¸ðµå »óÅÂÀÔ´Ï´Ù. ###\n");
		if (!read_pf(cfg.sysop)) {
			printf(" sysopÀ¸·Î ÁöÁ¤µÈ '%s' ID¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù.\n",cfg.sysop);
			if (yesno("ÀÎµ¦½ºÀçÁ¤¸®¸¦ ÇÒ±î¿ä? (Y/n) >>",Yes)) {
				printf("\n ÀÎµ¦½ºÆÄÀÏÀ» ÀçÁ¤¸®ÇÕ´Ï´Ù...\n");
				make_ind(0,"make_ind_all");
			}
		}
		strcpy(pf.id,"sysop");
		pf.type=19;
		who=5;
	}
	else {
		strcpy(now_id,argv[1]);
		if (!read_pf3(now_id)) {
			mess("%s ID¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù.",now_id);
			if (yesno("ÀÎµ¦½ºÀçÁ¤¸®¸¦ ÇÒ±î¿ä? (Y/n) >>",Yes)) {
				make_ind(0,"make_ind_all");
				printf("\nÈ¸¿ø ¾ÆÀÌµð ÀÎµ¦½º È­ÀÏ ÀçÁ¤¸® ¿Ï·á\n");
			}
			goto _quit;
		}
		if(pf.type<16) {
			mess("%s ½Ã»ð¸Þ´º °¡´Éµî±ÞÀÌ ¾Æ´Õ´Ï´Ù.",now_id);
			goto _quit;
		}
		who=0;
			 if (is_same(cfg.sysop,now_id)) who=4;
		else if (pf.type==19)				who=4;	//root
		else if (pf.type==18)				who=3;	//ºÎ½Ã»ð
		else if (pf.type==17)				who=2;	//¿î¿µÁø
		if (who<2||who>5) {
			mess("%s´ÔÀº ½Ã»ð¸Þ´º¸¦ »ç¿ëÇÒ¼ö ¾ø´Â µî±ÞÀÔ´Ï´Ù.",now_id);
			goto _quit;
		}
	}

	rawmode();	// ³Í Ä³³ë´ÏÄ® ¸ðµå

	stat_pf();	//È¸¿øÀÚ·á È®ÀÎ
	while (1) {
		cls();
		xy(1,1);
		printf(" ¦£¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤\n");
		if (who==5) printf(" ¦¢[7m  %-22s## ½© Àü¿ë ½Ã»ð ¸Þ´º  [m¦¢\n",cfg.hostname);
		else		printf(" ¦¢[7m  %-22s  %8s W:%d  #:%3d [m¦¢\n",cfg.hostname,now_id,who,cfg.hostno);
		printf(" ¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¨¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n");
		printf(" ¦¢   1. È¸¿ø¸ñ·Ï        ¦¢  21. ½Ã½ºÅÛÈ¯°æ¼³Á¤  ¦¢\n");
		printf(" ¦¢   2. È¸¿ø°ü¸®/¼³Á¤   ¦¢  22. ±ÝÁö/Á¦ÇÑ/´ã´ç  ¦¢\n");
		printf(" ¦¢   3. µî±Þ/·¹º§/Á÷¾÷  ¦¢  23. Æ÷Æ®/³ëµå/ºÐ·ù  ¦¢\n");
		printf(" ¦¢                      ¦¢  24. ÀÚ·á½Ç ¾÷/´Ù¿î  ¦¢\n");
		printf(" ¦¢   5. ¼Õ´Ô¼³Á¤        ¦¢  25. °Ô½ÃÆÇ,ÆíÁý±â   ¦¢\n");
		printf(" ¦¢   6. °¡ÀÔ¼³Á¤        ¦¢  26. È­»ó°Ô½ÃÆÇ      ¦¢\n");
		printf(" ¦¢   7. °¡ÀÔÇØÁö        ¦¢  27. °Ô½ÃÆÇ ÀÚµ¿ÃßÃµ ¦¢\n");
		printf(" ¦¢   8. ¸Þ´ºÆíÁý        ¦¢  28. ½Ã½ºÅÛ ¹é¾÷     ¦¢\n");
		printf(" ¦¢   9. À¯Æ¿¸®Æ¼        ¦¢  29. °øÁö»çÇ×¼³Á¤    ¦¢\n");
		printf(" ¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦«¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n");
		printf(" ¦¢  11. µ¿È£È¸          ¦¢  31. ¾÷´Ù¿îÆÄ¶ó¸ÞÅ¸  ¦¢\n");
		printf(" ¦¢  12. È­¸é°ü¸®        ¦¢  32. ÀüÃ¼ÀÚ·á½Ç¼³Á¤  ¦¢\n");
		printf(" ¦¢  13. ¸ÞÀÏ ¼³Á¤       ¦¢  33. Åë°è°ü·ÃÀÛ¾÷    ¦¢\n");
		printf(" ¦¢  14. ´ëÈ­½Ç¼³Á¤      ¦¢  34. ÀÎÆ÷¼¥°ü·Ã¼³Á¤  ¦¢\n");
		printf(" ¦¢  15. ¸Þ¼¼Áö°ü·Ã      ¦¢                      ¦¢\n");
		printf(" ¦¢  16. ÁøÀÔ/Á¾·á°ü·Ã   ¦¢                      ¦¢\n");
		printf(" ¦¢  17. ¸í·É/ÀÔ·Â°ü·Ã   ¦¢                      ¦¢\n");
		if (who==5)
		printf(" ¦¢[7m  98. ½Ã»ð¸í·É¾îÆíÁý  [m¦¢[7m  99. ½©Àü¿ë½Ã»ð¼³Á¤  [m¦¢\n");
		printf(" ¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦ª¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥\n");
		printf(" ¼±ÅÃ¹øÈ£? (Á¾·á:P ÀúÀåÃë¼Ò:N »óÅÂ:?) >>");
		lineinput(buff,10);check_han(buff);printf("\n");num=atoi(buff);
		if (is_same(buff,"n")) goto _quit;
		if (is_quit(buff)) break;
		if (num== 1) sys_tem("bin/pfinfo '%s'",who==5?"root":now_id);
		if (num== 2) job_pf();
		if (num== 3) job_type();
		if (num== 5) job_guest();
		if (num== 6) job_guestinput();
		if (num== 7) job_idout();
		if (num== 8) job_menu();
		if (num== 9) job_util();
		if (num==11) job_club();
		if (num==12) job_screen();
		if (num==13) job_mail();
		if (num==14) job_chat();
		if (num==15) job_mess();
		if (num==16) job_start();
		if (num==17) job_cmd();
		if (num==21) job_system();
		if (num==22) job_limit();
		if (num==23) job_node();
		if (num==24) job_updown();
		if (num==25) job_setbbs();
		if (num==26) job_jpg();
		if (num==27) job_good();
		if (num==28) job_backmode();
		if (num==29) job_notice();
		if (num==31) job_para();
		if (num==32) job_zbtotal();
		if (num==33) job_stat();
		if (num==34) job_info();
		if (who==5&&num==98) edit_syscmd();
		if (who==5&&num==99) job_sysops();
	}
	save_cfg();
	_quit:
	ioctl(0,TCSETA,&termold);	//¿ø·¡ ÅÍ¹Ì³¯ ¸ðµå º¹±¸
}


job_util()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"À¯Æ¿¸®Æ¼ÀÛ¾÷");
		printf("  1. ÅõÇ¥Àå ÀÛ¾÷ \n");
		printf("  2. ´º½º ¼Óº¸   \n");
		printf("  3. »çÀÌ¹ö¿ö    \n");
		printf("  4. »çÀÌ¹öÈ¨    \n");
		printf("  5. »çÀÌ¹ö¸Ó´Ï  \n");
		printf("  6. IP(Á¤º¸Á¦°ø)\n");
		printf("  7. »ç¿ë À¯µµ µµ¿ò¸»(today mess)¼³Á¤\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) job_vote();
		if (n== 2) job_news();
		if (n== 3) job_war();
		if (n== 5) job_money();
		if (n== 6) job_ip();
		if (n== 7) job_todaymess();
	}
}




//È¸¿ø °ü·Ã ÀÛ¾÷
job_stat()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"Åë°è °ü·Ã ÀÛ¾÷");
		printf("  1. ÀÏº° È¸¿ø °¡ÀÔÇöÈ²                        \n");
		printf("  2. È¸  ¿ø   Åë °è\n");
		printf("  3. ¿î¿µÁø   Åë °è\n");
		printf("  4. ÀÏ¹Ý±â·Ï Åë °è\n");
		printf("  5. ¿î¿µ±â·Ï Åë °è\n");
		printf("  6. Á¢¼Ó±â·Ï Åë °è\n");
		printf("  7. Á¢¼Ó±â·Ï Åë °è2\n");
		printf("  8. ´Ù¿î±â·Ï Åë °è\n");
		printf("  9. µ¿È£È¸»ç¿ë±â·Ï Åë °è\n");
		printf(" 99. È¸¿ø Åë°è\n");
		xy(1,20);
		n=numselect();
		printf("\n\n");
		if (n<= 0) break;
		if (n== 1) view_daypf();
		if (n== 2) view_stat(1);
		if (n== 3) view_stat(2);
		if (n== 4) sys_tem("bin/viewlog 2 '%s' main",cfg.sysop);
		if (n== 5) sys_tem("bin/viewlog 5 '%s' main",cfg.sysop);
		if (n== 6) sys_tem("bin/viewlog 6 '%s' main",cfg.sysop);
		if (n== 7) sys_tem("bin/viewlog 62 '%s' main",cfg.sysop);
		if (n== 8) sys_tem("bin/viewlog 7 '%s' main",cfg.sysop);
		if (n== 9) sys_tem("bin/viewlog 8 '%s' main",cfg.sysop);
		if (n==99) {disp_set_stat();pressenter();}		  //ÇöÀç »óÅÂ º¸¿©ÁÖ±â
	}
}

view_stat(int mode)
{
	FILE *fpid;
	FILE *fpsave;
	int  n,count,i,i2,i3;
	struct tm *tp;

	total=count_pf();				   //È¸¿ø ÃÑ ¼ö °è»ê
	fpid = fopen(ID_FILE, "r+");
	fpsave = fopen(TMP_FILE, "w+");

	fprintf(fpsave,"          ÃÑÈ¸¿ø:%5d¸í         \n",total);

	fseek(fpid,0,SEEK_SET);
	for (n=0;n<20;n++) {
		if (mode==1&&n<14) continue;
		count=0;
		fprintf(fpsave,"   ==========   ·¹º§ %4d  ========== \n",n);
		fseek(fpid,0,SEEK_SET);
		while(fread(&pf,sizeof_pf,1,fpid)) {		 //¾ÆÀÌµð ÀúÀå
			if (strlen(pf.id)<3) continue;
			if (pf.type==n) {
				tp = localtime(&pf.login);
				fprintf(fpsave," %3d.  %-8s %-8s %5d %5d %5d %5d ",
				count+1,pf.id, pf.name, pf.level,pf.countw,pf.countu,pf.countd);
				fprintf(fpsave,"%02d/%02d/%02d\n",
				tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
				count++;
			}
		}
		fprintf(fpsave,"                       == ÃÑ %4d¸í == \n",count);
	}

	fclose(fpid);
	fclose(fpsave);
	view_text(TMP_FILE);
	unlink(TMP_FILE);
}



//È¸¿ø °ü·Ã ÀÛ¾÷
job_pf()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"È¸¿ø °ü¸® ÀÛ¾÷");
		printf("  1. ÀüÃ¼ È¸¿ø °Ë»ö          (¸ñ·ÏÇü½Ä)        \n");
		printf("  2. ID/¼º¸í ºü¸¥ °Ë»ö/¼öÁ¤                    \n");
		printf("  3. È¸¿ø °Ë»ö,¼öÁ¤          (ºÐ·ùº°°Ë»ö)      \n");
		printf("  4. °¡ÀÔ´ë±â È¸¿ø °¡ÀÔÃ³¸®  (½Å±Ô°¡ÀÔÇã°¡ %d¸í´ë±â)\n",count_rec(GUEST_FILE   ,sizeof_pf));
		printf("  5. °¡ÀÔ´ë±â È¸¿ø ¼±ÅÃ °¡ÀÔÃ³¸®\n");
		printf("  6. µî·Ï°ÅºÎ È¸¿ø Ãß°¡,ÇØÁ¦ (guestnot.dat %d¸í°ÅºÎ)\n",count_rec(GUESTNOT_FILE,sizeof_pf));
		printf("  7. È¸¿ø ÀçÁ¤¸®/»èÁ¦Ã³¸®    (µî±Þ 0 È¸¿øÃ³¸®) \n");
		printf("  8. ½Å±Ô È¸¿ø ¾ÆÀÌµð ¸¸µé±â                   \n");
		printf("  9. ÁöÁ¤ ¹øÈ£ ¾ÆÀÌµð °­Á¦¼öÁ¤ (°Ë»ö½ÇÆÐID¼öÁ¤)\n");
		printf(" 10. È¸¿ø ¾ÆÀÌµð ÀÎµ¦½º È­ÀÏ ÀçÁ¤¸®\n");
		printf(" 11. È¸¿ø READ ½Ã ºü¸¥°Ë»ö (½ÇÆÐ½Ã->ÀüÃ¼°Ë»ö) %s\n",rbit(cfg.pfmode,1,"ÀÏ¹Ý°Ë»ö","ºü¸¥°Ë»ö"));
		printf(" 12. È¸¿ø½Å»ó(PF)                             %s\n",rbit(cfg.pfmode,21,"±âº»","»ç¿ëÀÚ"));
		printf(" 13. Á¦ÇÑ½Ã°£,³¯Â¥°æ°ú½Ã Á¢¼Ó                 %s\n",rbit(cfg.mode,1,"Á¾·á","ÀçÁ¢¼Ó"));
		printf(" 14. ³³ÀÔ³¯Â¥Á¦ÇÑÁ¦µµ(È¸ºñ)½Ç½Ã               %s\n",rbit(cfg.mode,3,"¹«½Ã","½Ç½Ã"));
		printf(" 15. ¿¬½À¿ë ¾ÆÀÌµð ÁöÁ¤                       system/testid.id\n");
		printf(" 16. »ç¿ë/ÁøÀÔ±ÝÁö ¾ÆÀÌµð ÁöÁ¤                system/notlog.id\n");
		printf(" 17. Á¢¼ÓÈ®ÀÎ¾ÆÀÌµð(user/rec/log_id.rec)      system/monilog.id\n");
		printf(" 18. »èÁ¦µÈ È¸¿ø ÀüÃ¼ º¹±¸ µî±Þ0 -> µî±Þ1·Î  \n");
		printf(" 19. »èÁ¦ ´ë»ó È¸¿ø Á¶È¸ (µî±Þ  0ÀÎ È¸¿ø)    \n");
		printf(" 20. ¿î ¿µ Áø  È¸¿ø Á¶È¸ (µî±Þ 16ÀÌ»óÀÎ È¸¿ø)\n");
		printf(" 21. µî±Þº° ¹«Á¶°Ç °¡±â ¼³Á¤                 (system/auto_go.txt)\n");
        printf(" 22. ÀüÃ¼È¸¿ø µî±Þ,·¹º§,Á¢¼Ó½Ã°£,¼ºº° °»½Å   \n");
		printf(" 23. ¼Õ´ÔÁöÁ¤ID (È¸¿ø°Ë»ö¾ÈÇÔ:µî±Þ0ÁöÁ¤)     (system/guestid.id)\n");
		printf(" 24. RANK(µî¼ö) ½Ã ·¹º§Ã³¸®(RK <-> RK2)       %s\n",rbit(cfg.levelmode,2,"·¹º§º¸ÀÓ","¾Èº¸ÀÓ"));
		printf(" 25. IND  -> QUICK °Ë»ö¿©ºÎ                   %s\n",rbit(cfg.mprocmode,2,"QUICK","¼øÂ÷"));
		n=numselect();printf("\n");
		if (n<= 0) break;
		if (n== 1) sys_tem("bin/pfinfo '%s'",who==5?"root":now_id);
		if (n== 2) sys_tem("bin/pf '%s' set_search",now_id);
		if (n== 3) sys_tem("bin/pf '%s'",now_id);
		if (n== 4) sys_tem("bin/guest 999");
		if (n== 5) sys_tem("bin/guest 998");
		if (n== 6) notfree();
		if (n== 7) sort_pf();	  //È¸¿ø ÀçÁ¤¸®
		if (n== 8) add_new_pf();
		if (n== 9) id_num_edit();
		if (n==10) make_ind(0,"make_ind_all");
		if (n==11) cfg.pfmode = gtt_bit(cfg.pfmode,1);
		if (n==12) cfg.pfmode = gtt_bit(cfg.pfmode,21);
		if (n==13) cfg.mode 	  =gtt_bit(cfg.mode,1);
		if (n==14) cfg.mode 	  =gtt_bit(cfg.mode,3);
		if (n==15) xedit("system/testid.id");
		if (n==16) xedit("system/notlog.id");
		if (n==17) xedit("system/monilog.id");
		if (n==18) recover();
		if (n==19) check_pf(1,who);
		if (n==20) check_pf(2,who);
		if (n==21) auto_go();
		if (n==22) reset_all();
		if (n==23) xedit("system/guestid.id");
		if (n==24) cfg.levelmode=gtt_bit(cfg.levelmode,2);
		if (n==25) cfg.mprocmode=gtt_bit(cfg.mprocmode,2);
	}
}


//È¸¿ø °ü·Ã ÀÛ¾÷
reset_all()
{
	int num,i,type1,type2;
	char buff[100];
	type1=type2=i=0;
	mess("ÀÌ ±â´ÉÀº ÀüÃ¼ È¸¿ø ¼³Á¤ ±â´ÉÀÔ´Ï´Ù. (¹é¾÷±ÇÀå)");
	while (1) {
		cls();
		print_(1,"ÀüÃ¼ È¸¿ø µî±Þ,·¹º§ ¼³Á¤");
		printf(" 1. º¯°æ ´ë»ó µî±Þ ÁöÁ¤     %d ºÎÅÍ  %d µî±Þ \n",type1,type2);
		printf(" 2. ÀüÃ¼ È¸¿ø µî±Þ ÀçÁ¶Á¤                    \n");
		printf(" 3. ÀüÃ¼ È¸¿ø ·¹º§ ÀçÁ¶Á¤                    \n");
		printf(" 4. ÀüÃ¼ È¸¿ø ÀüÃ¼Á¢¼Ó¼ö ÀçÁ¶Á¤              \n");
		printf(" 5. ÀüÃ¼ È¸¿ø ±Ý¿ùÁ¢¼Ó¼ö ÀçÁ¶Á¤              \n");
		printf(" 6. ÀüÃ¼ È¸¿ø ÀüÃ¼»ç¿ë½Ã°£ÀçÁ¶Á¤             \n");
		printf(" 7. ÀüÃ¼ È¸¿ø ±Ý¿ù»ç¿ë½Ã°£ÀçÁ¶Á¤             \n");
		printf(" 8. ÀüÃ¼ È¸¿ø ÁÖ¹Îµî·Ï¹øÈ£ »èÁ¦              \n");
		printf(" 9. ÀüÃ¼ È¸¿ø ÁÖ¹Îµî·Ï¹øÈ£ -> ¼ºº°ÁöÁ¤       \n");
		printf("10. ÀüÃ¼ È¸¿ø ¾²±â/µî·Ï/´Ù¿î¼ö 0À¸·Î ÁöÁ¤    \n");
        printf("11. ÀüÃ¼ È¸¿ø WWW ¾²±â         0À¸·Î ÁöÁ¤    \n");
        printf("12. ÀüÃ¼ È¸¿ø CASH ÁöÁ¤                      \n");
		n=numselect();printf("\n");
		if (n<= 0) break;
		if (n== 1) {
			printf("´ë»óµî±Þ ½ÃÀÛ? >>");type1=get_num(type1,2);
			printf("´ë»óµî±Þ ³¡  ? >>");type2=get_num(type2,2);
		}
		else if (n== 2) {
			printf("ÀçÁ¶Á¤ µî±Þ? >>");i=numselect2();
			if (i<1||i>14) mess(" °¡´Éµî±Þ 1~14");
			else make_reset(n,i,type1,type2);
		}
		else if (n== 3) {
			printf("ÀçÁ¶Á¤ ·¹º§? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 4) {
			printf("ÀçÁ¶Á¤ ÀüÃ¼Á¢¼Ó¼ö? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 5) {
			printf("ÀçÁ¶Á¤ ±Ý¿ùÁ¢¼Ó¼ö? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 6) {
			printf("ÀçÁ¶Á¤ ÀüÃ¼»ç¿ë½Ã°£? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 7) {
			printf("ÀçÁ¶Á¤ ±Ý¿ù»ç¿ë½Ã°£? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 8) {
			if (yesno("ÀüÃ¼ È¸¿øÀÇ ÁÖ¹Îµî·Ï ¹øÈ£¸¦ »èÁ¦ÇÒ±î¿ä? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
		else if (n== 9) {
			if (yesno("ÁÖ¹Îµî·Ï¹øÈ£·Î ¼ºº°À» ÁöÁ¤ÇÒ±î¿ä? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
		else if (n==10) {
			if (yesno("¾²±â/µî·Ï/´Ù¿î¼ö¸¦ 0À¸·Î ÁöÁ¤ÇÒ±î¿ä? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
        else if (n==11) {
            if (yesno("WWW ¾²±â¼ö¸¦ 0À¸·Î ÁöÁ¤ÇÒ±î¿ä? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
        else if (n==12) {
			printf("ÀçÁ¶Á¤ CASH? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
	}
}


make_reset(int mode ,int num,int type1,int type2)
{
	int i,n,n2,nn,count,stop=Yes;
	char sex;
	char buff[100];
	FILE *fp;
	if (!yesno2("\n ### ÀüÃ¼ È¸¿øÀ» ¼¼ÆÃÇÒ±î¿ä? (y/N)>>",No)) return;
	if ((fp=fopen(ID_FILE,"r+"))==NULL) return;
	i=0;
	count=0;
	printf("\n");
	if (type1==0&&type2==0) {mess("º¯°æ´ë»óµî±ÞÀ» ÁöÁ¤ÇÏ¼¼¿ä");return;}
	while (1) {
		fseek(fp,i*sizeof_pf,SEEK_SET);
		if (i<0||!fread(&pf,sizeof_pf,1,fp)) break;
		if (pf.type<type1||pf.type>type2||pf.type>=18) goto _cont;
		printf("%-8s (%-8s) ",pf.id,pf.name);
			 if (mode==2) {printf("µî±Þº¯°æ %d -> %d\n",pf.type,num);pf.type=num;}
		else if (mode==3) {printf("·¹º§º¯°æ %d -> %d\n",pf.level,num);pf.level=num;}
		else if (mode==4) {printf("Á¢¼Ó¼öº¯°æ %d -> %d\n",pf.all_log,num);pf.all_log=num;}
		else if (mode==5) {printf("¿ùº°Á¢¼Ó¼öº¯°æ %d -> %d\n",pf.month_log,num);pf.month_log=num;}
		else if (mode==6) {printf("ÀüÃ¼»ç¿ë½Ã°£º¯°æ %d -> %d\n",pf.all_time,num);pf.all_time=num;}
		else if (mode==7) {printf("¿ùº°»ç¿ë½Ã°£º¯°æ %d -> %d\n",pf.month_time,num);pf.month_time=num;}
		else if (mode==8) {printf("ÁÖ¹Îµî·Ï»èÁ¦ %s -> »èÁ¦\n",pf.id_no);strcpy(pf.id_no,"");}
		else if (mode==9) {
			sex=pf.sex;
			pf.sex=pf.id_no[7]=='2'?'f':'m';
			printf("¼ºº°º¯°æ     %c -> %c\n",pf.sex,sex);pf.sex=sex;
		}
		fseek(fp,i*sizeof_pf,SEEK_SET);
		pf.num=i;
//		  fwrite(&pf,sizeof_pf,1,fp);
		gwrite(202,pf,fp);
		count++;
		if (count%30==0&&stop) {
			printf("\n ÇöÀç %d ÀÚ·áÃ³¸®Áß  °è¼Ó(Enter) ÁßÁö(Q) ¿¬¼Ó(S)>>",count);
			lineinput(buff,2);printf("\n");
			if (is_same(buff,"s")) stop=No;
			else if (is_quit(buff)) break;
		}
		else if (mode==10) {printf("¾²±â/µî·Ï/´Ù¿î¼ö»èÁ¦\n");pf.countw=pf.countu=pf.countd=0;}
        else if (mode==11) {printf("WWW ¾²±â»èÁ¦\n");pf.wwrite=0;}
        else if (mode==12) {printf("CASH º¯°æ %d -> %d\n",pf.cash,num);pf.cash=num;}
		_cont:
		i++;
	}
	fclose (fp);
	mess("ÃÑ %d¸íÀÇ ÀÚ·á°¡ º¯°æµÇ¾ú½À´Ï´Ù.",count);
}


job_guest()
{
	while (1) {
		cls();
		print_(1,"¼Õ´Ô°ü·Ã ¼³Á¤");
		printf("  1. ¼Õ´Ô Á¢¼Ó ¾ÆÀÌµð           :%s\n",cfg.guestid    );
		printf("  2. ¼Õ´Ô ÀÚµ¿ °¡ÀÔ             :%s\n",rbit(cfg.guestmode,15, "¼öµ¿°¡ÀÔ", "ÀÚµ¿°¡ÀÔ"));
		printf("  3. ¼Õ´Ô Á¢¼ÓÈÄ ¹Ù·Î °¡ÀÔ      :%s\n",rbit(cfg.guestmode,16, "º¸Åë", "¹Ù·Î°¡ÀÔ"));
		printf("  4. ¼Õ´Ô Á¢¼Ó½Ã º°¸í ÀÔ·Â      :%s\n",rbit(cfg.guestmode,17, "ÀÔ·Â", "Åë°ú"));
		printf("  5. ¼Õ´Ô½Ã ÀÏ·Ã ¹øÈ£           :%s\n",rbit(cfg.guestmode,18, "º¸Åë", "¹øÈ£"));
		printf("  7. ¼Õ´Ô Á¢¼Ó½Ã ¼­ºñ½º¸Þ´º·Î   :%s\n",rbit(cfg.guestmode,2,  "º¸Åë", "°¡±â"));
		printf("  8. ¼Õ´Ô Á¢¼Ó½Ã °¡ÀÔµµ¿ò¸»     :%s\n",rbit(cfg.guestmode,6,  "º¸Åë", "°¡±â"));
		printf("  9. ¼Õ´Ô Á¢¼Ó½Ã ³¡³¿           :%s\n",rbit(cfg.guestmode,1,  "º¸Åë", "³¡³¿"));
		printf(" 10. ¼Õ´Ô SAY ¸í·É »ç¿ë         :%s\n",rbit(cfg.chatmode,10,  "°¡´É", "ºÒ°¡"));
		printf(" 11. ¼Õ´Ô MM  ¸í·É »ç¿ë         :%s\n",rbit(cfg.chatmode,11,  "°¡´É", "ºÒ°¡"));
		printf(" 12. ¼Õ´Ô ÁøÀÔ ½Ã ÃÊ±â (¼±ÅÃ)   :%s\n",rbit(cfg.guestmode,8,  "Åë°ú", "¼±ÅÃ"));
		printf(" 13. Á¢¼Ó½ÃEnter ¹Ù·Î¼Õ´ÔÃ³¸®   :%s\n",rbit(cfg.guestmode,21, "º¸Åë", "¼Õ´Ô"));
		printf(" 14. ¾ÆÀÌµðÀÔ·Â ¿©ºÎ            :%s\n",rbit(cfg.guestmode,20, "°¡´É", "¹¯Áö¾ÊÀ½,ÀüºÎ¼Õ´Ô"));
		printf(" 15. ¼Õ´ÔÁ¢¼Ó½Ã ÇÏ´Ü °¡ÀÔ¾È³»   :%s\n",rbit(cfg.guestmode,9,  "¾È³»", "¹«½Ã"));
		printf(" 16. IDÀÔ·Â5È¸ÃÊ°ú½Ã Á¢¼ÓÃ³¸®   :%s\n",rbit(cfg.guestmode,13, "¼Õ´ÔÀ¸·Î","Á¾·á"));
		printf(" 17. ¼Õ´ÔÁ¢¼Ó½Ã /xguest µ¿ÀÛ    :%s\n",rbit(cfg.guestmode,14, "ÀÏ¹Ý¸Þ´º","¼Õ´ÔÀü¿ë"));
		n=numselect();xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)					get_str(cfg.guestid  , 8);
		if (n== 2)	cfg.guestmode  =gtt_bit(cfg.guestmode,15);
		if (n== 3)	cfg.guestmode  =gtt_bit(cfg.guestmode,16);
		if (n== 4)	cfg.guestmode  =gtt_bit(cfg.guestmode,17);
		if (n== 5)	cfg.guestmode  =gtt_bit(cfg.guestmode,18);
		if (n== 6)	cfg.guestmode  =gtt_bit(cfg.guestmode, 5);
		if (n== 7)	cfg.guestmode  =gtt_bit(cfg.guestmode, 2);
		if (n== 8)	cfg.guestmode  =gtt_bit(cfg.guestmode, 6);
		if (n== 9)	cfg.guestmode  =gtt_bit(cfg.guestmode, 1);
		if (n==10)	cfg.chatmode   =gtt_bit(cfg.chatmode ,10);
		if (n==11)	cfg.chatmode   =gtt_bit(cfg.chatmode ,11);
		if (n==12)	cfg.guestmode  =gtt_bit(cfg.guestmode, 8);
		if (n==13)	cfg.guestmode  =gtt_bit(cfg.guestmode,21);
		if (n==14)	cfg.guestmode  =gtt_bit(cfg.guestmode,20);
		if (n==15)	cfg.guestmode  =gtt_bit(cfg.guestmode, 9);
		if (n==16)	cfg.guestmode  =gtt_bit(cfg.guestmode,13);
		if (n==17)	cfg.guestmode  =gtt_bit(cfg.guestmode,14);
	}
}


job_guestinput()
{
	int n,i;
	char buff[300];
	while (1) {
		cls();
		print_(0,"°¡ÀÔ °ü·Ã ¼³Á¤");
		printf("  1. ¼Õ´Ô Á¢¼Ó½Ã °¡ÀÔÈÄÁ¾·á           %s\n",rbit(cfg.guestmode,3,"º¸Åë","Á¾·á"));
		printf("  2. ¼Õ´Ô Á¢¼Ó½Ã °¡ÀÔÈÄÀçÁ¢¼Ó         %s\n",rbit(cfg.guestmode,4,"º¸Åë","Á¢¼Ó"));
		printf("  3. ¼Õ´Ô Áú¹® Ç×¸ñ ¼±ÅÃ              \n");
		printf("  4. °¡ÀÔÈÄ ÃÊ±â ±âº»°ªÁöÁ¤ (µî±Þ..)  :%d\n",cfg.guesttype  );
		printf("  5. ¼Õ´Ô ½ÂÀÎ ºñ¹Ð ¹øÈ£ °ü¸®         \n");
		printf("  9. °¡ÀÔ ±ÝÁö ¾ÆÀÌµð ÁöÁ¤                       (system/notguest.txt)\n");
		printf(" 10. °¡ÀÔ ±ÝÁö ÁÖ¹Îµî·Ï¹øÈ£ ÁöÁ¤                 (system/bad_idno.txt)\n");
		printf(" 11. °¡ÀÔ ±ÝÁö ¾ÆÀÌµð ÁöÁ¤ »çÀ¯ È­¸é             (logo/notguest.log)\n");
		printf(" 12. °¡ÀÔ ±ÝÁö »çÀ¯(°¡ÀÔÁ¶°Ç¼³¸í)È­¸é            (logo/notenter.log)\n");
		printf(" 13. ¾ÆÀÌµð ¿µ¹®     °¡´É             :%s\n",rbit(cfg.guestin,20, "°¡´É", "ºÒ°¡"));
		printf(" 14. ¾ÆÀÌµð ÇÑ±Û     °¡´É             :%s\n",rbit(cfg.guestin,21, "°¡´É", "ºÒ°¡"));
		printf(" 15. ¾ÆÀÌµð ÇÑÀÚ     °¡´É             :%s\n",rbit(cfg.guestin,22, "°¡´É", "ºÒ°¡"));
		printf(" 16. ¾ÆÀÌµð Æ¯¼ö¹®ÀÚ °¡´É             :%s\n",rbit(cfg.guestin,23, "°¡´É", "ºÒ°¡"));
		printf(" 17. ºñ¹Ð¹øÈ£½Ã ½¬¿îºñ¹Ð¹øÈ£ °¡´É     :%s\n",rbit(cfg.guestin,24, "Çã¿ë", "ºÒ°¡"));
		printf(" 18. ÁÖ¼ÒÀÔ·Â½Ã ÀüÃ¼ÁÖ¼Ò ÆíÁý         :%s\n",rbit(cfg.guestin,25, "Çã¿ë", "ºÒ°¡"));
		printf(" 19. °¡ÀÔ½Ã Áú¹® Ç×¸ñ ¼³Á¤\n");
		printf(" 20. ÁÖ¹Îµî·Ï¹øÈ£ ÀÔ·Â½Ã ÀÚµ¿ Åë°ú ¹®ÀÚ¿­        (system/passindo.txt)    \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.guestmode  =gtt_bit(cfg.guestmode,3);
		if (n== 2) cfg.guestmode  =gtt_bit(cfg.guestmode,4);
		if (n== 3) guest_input_item();
		if (n== 4) guest_init();
		if (n== 5) job_epass();
		if (n== 9) xedit("system/notguest.txt");
		if (n==10) xedit("system/bad_idno.txt");
		if (n==11) xedit("frm/guest/notguest.log");
		if (n==12) xedit("frm/guest/notenter.log");
		if (n==13) cfg.guestin=gtt_bit(cfg.guestin,20);
		if (n==14) cfg.guestin=gtt_bit(cfg.guestin,21);
		if (n==15) cfg.guestin=gtt_bit(cfg.guestin,22);
		if (n==16) cfg.guestin=gtt_bit(cfg.guestin,23);
		if (n==17) cfg.guestin=gtt_bit(cfg.guestin,24);
		if (n==18) cfg.guestin=gtt_bit(cfg.guestin,25);
		if (n==19) guest_input_item();
		if (n==20) xedit("system/passidno.txt");
	}
}

job_epass()
{
	int n,i,eenum;
	char buff[300];
	char filename[300];

	strcpy(filename,"system/epass.num");
	while (1) {
		file_line(buff,filename,1);
		eenum=atoi(buff);
		cls();
		print_(0,"½ÂÀÎ ¹øÈ£ °ü·Ã ¼³Á¤");
		printf("  1. °¡ÀÔÈÄ ½ÂÀÎ ¾ÏÈ£ÀÔ·ÂÈÄ µî±Þ1ÁöÁ¤   %s\n",rbit(cfg.guestmode,25,"º¸Åë","ÁöÁ¤"));
		printf("  2. °¡ÀÔÈÄ ½ÂÀÎ ¾ÏÈ£ÀÔ·ÂÈÄ ½Â±Þ2°¡´É   %s\n",rbit(cfg.guestmode,26,"º¸Åë","ÁöÁ¤"));
		printf("  3. ID ÀÔ·ÂÈÄ   ½ÂÀÎ ¾ÏÈ£ÀÔ·Â          %s\n",rbit(cfg.guestmode,27,"º¸Åë","ÁöÁ¤"));
		printf("  4. ½ÂÀÎ¹øÈ£ ÀÚµ¿ ÁöÁ¤ (0:³­¼ö #:ÁöÁ¤) %d\n",eenum);
		printf("  7. È¸¿ø ½ÂÀÎ ¾ÏÈ£ ÀÓÀÇ ÁöÁ¤         \n");
		printf("  8. È¸¿ø ½ÂÀÎ ¾ÏÈ£ ÀÓÀÇ »èÁ¦(ÆíÁý)   \n");
		printf("  9. È¸¿ø ½ÂÀÎ ¾ÏÈ£ ±â·Ï È®ÀÎ         \n");
		n=numselect();
		if (n<= 0) break;
		if (n== 1) cfg.guestmode =gtt_bit(cfg.guestmode,25);
		if (n== 2) cfg.guestmode =gtt_bit(cfg.guestmode,26);
		if (n== 3) cfg.guestmode =gtt_bit(cfg.guestmode,27);
		if (n== 4) xedit(filename);
		if (n== 7) make_epass(1);
		if (n== 8) make_epass(2);
		if (n== 9) make_epass(3);
	}
}

make_epass(int mode)
{
	FILE *fp;
	char buff[300];
	char buff2[300];
	char buff3[300];
	char filename[300];

	if (mode==1) {
		while(1) {
			printf("\n  ½ÂÀÎ ¾ÏÈ£ ÁöÁ¤ ID :");
			lineinput(buff,8);
			if (strlen(buff)<3) break;
			if (!read_pf4(buff)) {
				printf("  %s ID¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù. \n",buff);
				continue;
			}
			printf(" ÀÌ¸§:%s  µî±Þ:%d",pf.id,pf.type);
			printf("\n  ½ÂÀÎ ¾ÏÈ£ ÀÔ·Â (4~12ÀÚ¸®) :");
			lineinput(buff2,8);
			if (strlen(buff2)<4) continue;
			sprintf(buff,"user/epass/%s.dat",pf2.id);
			if ((fp=fopen(buff,"w"))!=NULL) {
				sprintf(buff3,"%s %-8s %-8s %s",buff2,pf2.id,pf2.name,now_str(2));
				fprintf(fp,"%s\n",buff3);
				fclose(fp);
			}
			sprintf(buff,"user/epass/%s.lst",now_str(12));
			if ((fp=fopen(buff,"a+"))==NULL) fp=fopen(buff,"w+");
			if (fp!=NULL) {
				fprintf(fp,"%s\n",buff3);
				fclose(fp);
			}
			printf("  %s -> %s ½ÂÀÎÁöÁ¤",pf2.id,buff2);
		}
	}
	else if (mode==2) {
		while(1) {
			printf("\n  ½ÂÀÎ ¾ÏÈ£ ÆíÁý/»èÁ¦ ID :");
			lineinput(buff2,8);
			if (strlen(buff2)<3) break;
			sprintf(filename,"user/epass/%s.dat",buff2);
			if (!is_file(filename)) {
				printf("  %s ½ÂÀÎ¹øÈ£ÆÄÀÏÀÌ ¾ø½À´Ï´Ù. \n",filename);
				continue;
			}
			xedit(filename);
			if (file_size(filename)<5) {
				unlink(filename);
				printf("\n  %s ½ÂÀÎ¹øÈ£ÆÄÀÏÀÌ »èÁ¦µÇ¾ú½À´Ï´Ù. \n",filename);
			}
		}
	}
	else if (mode==3) {
		while(1) {
			printf("\n  ±â·ÏÀ» È®ÀÎÇÒ ³âµµ¿ù  (¿¹99³â5¿ù:9905)>>");
			lineinput(buff2,5);
			if (strlen(buff2)==0) strcpy(buff2,now_str(12));
			if (strlen(buff2)<3) break;
			sprintf(filename,"user/epass/%s.lst",buff2);
			xedit(filename);
		}
	}
}



guest_init()
{
	int n,i;
	char buff[300];
	while (1) {
		cls();
		print_(0,"°¡ÀÔ½Ã ÃÊ±â°ª °ü·Ã ¼³Á¤");
		printf("  1. °¡ÀÔÈÄ ÃÊ±â µî±Þ (0-16)      :%d\n",cfg.guesttype  );
		printf("  2. °¡ÀÔÈÄ ÃÊ±â ·¹º§             :%d\n",cfg.guestlevel );
		printf("  3. °¡ÀÔÈÄ ÃÊ±â ½Ã°£ (ºÐ)        :%d\n",cfg.guesttime  );
		printf("  4. °¡ÀÔÈÄ ÃÊ±â ´Ù¿î¿ë·®K        :%d\n",cfg.guestdown  );
		printf("  5. °¡ÀÔÈÄ ÃÊ±â ±Ý¾×             :%d\n",cfg.guestcash  );
		printf("  6. °¡ÀÔÈÄ ÃÊ±â Ã¼·Â             :%d\n",cfg.guestp_ph  );
		printf("  7. °¡ÀÔÈÄ ÃÊ±â Áö·Â             :%d\n",cfg.guestp_it  );
		printf("  8. °¡ÀÔÈÄ ÃÊ±â ¸Å·Â             :%d\n",cfg.guestp_at  );
		printf("  9. °¡ÀÔÈÄ ÃÊ±â ¹æ¾î·Â           :%d\n",cfg.guestp_pr  );
		printf(" 10. °¡ÀÔÈÄ ÃÊ±â ¸ðµå             :%d\n",cfg.guestp_mo  );
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.guesttype  =get_num(cfg.guesttype  , 2);
		if (n== 2) cfg.guestlevel =get_num(cfg.guestlevel , 6);
		if (n== 3) cfg.guesttime  =get_num(cfg.guesttime  , 8);
		if (n== 4) cfg.guestdown  =get_num(cfg.guestdown  , 8);
		if (n== 5) cfg.guestcash  =get_num(cfg.guestcash  , 8);
		if (n== 6) cfg.guestp_ph  =get_num(cfg.guestp_ph  , 8);
		if (n== 7) cfg.guestp_it  =get_num(cfg.guestp_it  , 8);
		if (n== 8) cfg.guestp_at  =get_num(cfg.guestp_at  , 8);
		if (n== 9) cfg.guestp_pr  =get_num(cfg.guestp_pr  , 8);
		if (n==10) cfg.guestp_mo  =get_num(cfg.guestp_mo  , 8);
	}
}


guest_input_item()
{
	while (1) {
		cls();
		print_(0,"°¡ÀÔ½Ã Áú¹® °ü·Ã ¼³Á¤");
		printf("  1. °¡ÀÔ½Ã ID   ¹°¾îº½  %s\n",rbit(cfg.guestin, 1," ÇÔ ","¾ÊÇÔ"));
		printf("  2. °¡ÀÔ½Ã ÀÌ¸§ ¹°¾îº½  %s\n",rbit(cfg.guestin, 2," ÇÔ ","¾ÊÇÔ"));
		printf("  3. ÁÖ¹Îµî·Ï¹øÈ£¹°¾îº½  %s\n",rbit(cfg.guestin, 3," ÇÔ ","¾ÊÇÔ"));
		printf("  4. »ý³â¿ùÀÏ ¹°¾îº½     %s\n",rbit(cfg.guestin, 4," ÇÔ ","¾ÊÇÔ"));
		printf("  5. ¼ºº°     ¹°¾îº½     %s\n",rbit(cfg.guestin, 5," ÇÔ ","¾ÊÇÔ"));
		printf("  6. ¾ÏÈ£¹°¾îº½          %s\n",rbit(cfg.guestin, 6," ÇÔ ","¾ÊÇÔ"));
		printf("  7. ÁÖ¼Ò¹°¾îº½          %s\n",rbit(cfg.guestin, 7," ÇÔ ","¾ÊÇÔ"));
		printf("  8. ÀüÈ­¹°¾îº½          %s\n",rbit(cfg.guestin, 8," ÇÔ ","¾ÊÇÔ"));
		printf("  9. ¿ìÆíÁÖ¼Ò            %s\n",rbit(cfg.guestin, 9," ÇÔ ","¾ÊÇÔ"));
		printf(" 10. »ß»ß¹°¾îº½          %s\n",rbit(cfg.guestin,10," ÇÔ ","¾ÊÇÔ"));
		printf(" 11. ÇÚµåÆù¹°¾îº½        %s\n",rbit(cfg.guestin,11," ÇÔ ","¾ÊÇÔ"));
		printf(" 12. Á÷¾÷¹°¾îº½          %s\n",rbit(cfg.guestin,12," ÇÔ ","¾ÊÇÔ"));
		printf(" 13. Á÷Àå¹°¾îº½          %s\n",rbit(cfg.guestin,13," ÇÔ ","¾ÊÇÔ"));
		printf(" 14. Á÷ÀåÀüÈ­            %s\n",rbit(cfg.guestin,14," ÇÔ ","¾ÊÇÔ"));
		printf(" 15. Âü°í»çÇ× 1          %s\n",rbit(cfg.guestin,15," ÇÔ ","¾ÊÇÔ"));
		printf(" 16. Âü°í»çÇ× 2          %s\n",rbit(cfg.guestin,16," ÇÔ ","¾ÊÇÔ"));
		printf(" 17. Âü°í»çÇ× 3          %s\n",rbit(cfg.guestin,17," ÇÔ ","¾ÊÇÔ"));
		printf(" 18. Âü°í»çÇ× 4          %s\n",rbit(cfg.guestin,18," ÇÔ ","¾ÊÇÔ"));
		printf(" 19. Âü°í»çÇ× 5          %s\n",rbit(cfg.guestin,19," ÇÔ ","¾ÊÇÔ"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.guestin=gtt_bit(cfg.guestin,1);
		if (n== 2) cfg.guestin=gtt_bit(cfg.guestin,2);
		if (n== 3) cfg.guestin=gtt_bit(cfg.guestin,3);
		if (n== 4) cfg.guestin=gtt_bit(cfg.guestin,4);
		if (n== 5) cfg.guestin=gtt_bit(cfg.guestin,5);
		if (n== 6) cfg.guestin=gtt_bit(cfg.guestin,6);
		if (n== 7) cfg.guestin=gtt_bit(cfg.guestin,7);
		if (n== 8) cfg.guestin=gtt_bit(cfg.guestin,8);
		if (n== 9) cfg.guestin=gtt_bit(cfg.guestin,9);
		if (n==10) cfg.guestin=gtt_bit(cfg.guestin,10);
		if (n==11) cfg.guestin=gtt_bit(cfg.guestin,11);
		if (n==12) cfg.guestin=gtt_bit(cfg.guestin,12);
		if (n==13) cfg.guestin=gtt_bit(cfg.guestin,13);
		if (n==14) cfg.guestin=gtt_bit(cfg.guestin,14);
		if (n==15) cfg.guestin=gtt_bit(cfg.guestin,15);
		if (n==16) cfg.guestin=gtt_bit(cfg.guestin,16);
		if (n==17) cfg.guestin=gtt_bit(cfg.guestin,17);
		if (n==18) cfg.guestin=gtt_bit(cfg.guestin,18);
		if (n==19) cfg.guestin=gtt_bit(cfg.guestin,19);
	}
}


job_idout()
{
	while (1) {
		cls();
		printf("            °¡ÀÔ ÇØÁö °ü·Ã ¼³Á¤\n");
		printfL2();
		printf("  1.  °¡ÀÔ ÇØÁö °¡´É                :%s\n",rbit(cfg.loginmode,18, "°¡´É", "ºÒ°¡"));
		printf("  2.  °¡ÀÔ ÇØÁö½Ã ÁÖ¹Îµî·Ï º¸°ü     :%s\n",rbit(cfg.loginmode,19, "º¸°ü", "»èÁ¦"));
		printfL2();
		n=numselect();
		xy(60,n+2);
		if (n<=0)  break;
		if (n==1)  cfg.loginmode  =gtt_bit(cfg.loginmode,18);
		if (n==2)  cfg.loginmode  =gtt_bit(cfg.loginmode,19);
	}
}


job_notice()
{
	while (1) {
		cls();
		print_(1,"°øÁö»çÇ× ¼³Á¤");
		printf(" 1. ÀÏ¹ÝÈ¸¿ø Á¢¼Ó½Ã Ãâ·Â¿©ºÎ           :%s\n",rbit(cfg.notice,0,"Ãâ·Â","Åë°ú"));
		printf(" 2. ¼Õ´Ô     Á¢¼Ó½Ã Ãâ·Â¿©ºÎ           :%s\n",rbit(cfg.notice,1,"Ãâ·Â","Åë°ú"));
		printf(" 3. °øÁö»çÇ× Ãâ·Â¹æ¹ý ID ÀÔ·ÂÀü        :%s\n",rbit(cfg.notice,2,"¾Èº¸ÀÓ","º¸ÀÓ"));
		printf(" 4. °øÁö»çÇ× Ãâ·Â¹æ¹ý TOPÁøÀÔÀü        :%s\n",rbit(cfg.notice,3,"¾Èº¸ÀÓ","º¸ÀÓ"));
		printf(" 5. °øÁö»çÇ× ±¤°í Ãâ·Â                 :%s\n",rbit(cfg.notice,4,"ÇÔ","¾ÈÇÔ"));
		printf(" 6. °øÁö»çÇ× Ãâ·Â¸ð¾ç (0~9)            :%d\n",cfg.noticet);
		printf(" 7. °øÁö»çÇ× Ãâ·Â½ÃÀÛ À§Ä¡ X           :%d\n",cfg.noticex);
		printf(" 8. °øÁö»çÇ× Ãâ·Â½ÃÀÛ À§Ä¡ Y           :%d\n",cfg.noticey);
		printf(" 9. °øÁö»çÇ× Ãâ·Â½ÃÀÛ ÁÙ±æÀÌ           :%d\n",cfg.noticeline);
		n=numselect();xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.notice	  =gtt_bit(cfg.notice ,0);
		if (n== 2) cfg.notice	  =gtt_bit(cfg.notice ,1);
		if (n== 3) cfg.notice	  =gtt_bit(cfg.notice ,2);
		if (n== 4) cfg.notice	  =gtt_bit(cfg.notice ,3);
		if (n== 5) cfg.notice	  =gtt_bit(cfg.notice ,4);
		if (n== 6) cfg.noticet	  =get_num(cfg.noticet,2);
		if (n== 7) cfg.noticex	  =get_num(cfg.noticex,2);
		if (n== 8) cfg.noticey	  =get_num(cfg.noticey,2);
		if (n== 9) cfg.noticeline =get_num(cfg.noticeline,2);
	}
}



//µî±Þº° ¹Ù·Î°¡±â
auto_go()
{
	FILE *fp;
	char buff[200];
	char filename[200];
	char auto_gocode[20][51];
	int i;
	for (i=0;i<20;i++) strcpy(auto_gocode[i],"");
	strcpy(filename,"system/auto_go.txt");
	if ((fp=fopen(filename,"r+"))!=NULL) {
		for (i=0;i<20;i++) {
			if (fgets(buff,200,fp)) {
				del_enter(buff);
				del_esc(buff);
				buff[50]=0;
				strcpy(auto_gocode[i],buff);
			}
		}
		fclose(fp);
	}
	while (1) {
		cls();
		print_(0,"µî±Þº° ÀÚµ¿ °¡±â ¸í·É ¼³Á¤");
		for (i=0;i<20;i++) {
			printf("  µî±Þ %2d. %s\n",i+1,auto_gocode[i]);
		}
		printf(" ¼±ÅÃ (0~19)  Ãë¼Ò(Q)  ÀúÀå(W) >>");
		lineinput(buff,20);n=atoi(buff);
		if (is_quit(buff)) break;
		if (is_same(buff,"w")) break;
		if (buff[0]==0) break;
		if (n==0&&!is_same(buff,"0")) continue;
		if (n>=0&&n<=20) {
			xy(40,n+2);
			get_str(auto_gocode[n-1],50);
		}
	}
	if ((fp=fopen(filename,"w+"))!=NULL) {
		for (i=0;i<20;i++) {
			del_enter(auto_gocode[i]);
			del_esc(auto_gocode[i]);
			fprintf(fp,"%s\n",auto_gocode[i]);
		}
		fclose(fp);
	}
}



//ºÐ·ù/ÄÚµå °ü·Ã ÀÛ¾÷
job_type()
{
	while (1) {
		cls();
		print_(0,"µî±Þ/ºÐ·ù/ÄÚµå ¼³Á¤");
		printf("  1. ÀÚµ¿ µî±Þ Á¶Á¤ ±â´É                       :%s\n",rbit(cfg.levelmode,1, "¼öµ¿", "ÀÚµ¿"));
		printf("  2. È¸¿ø ·¹º§ »óÇÑ                            :%d\n",cfg.levelmax   );
		printf("  3. µ¿È£È¸ÀÇ ÃÖ´ë Áõ°¡ °¡´É ·¹º§ (0:¹«½Ã~250) :%u\n",cfg.clublevel  );
		printf("  4. µî±Þº° ¸íÄª      ¼öÁ¤ \n");
		printf("  5. µî±Þº° ½Â±Þ/ÁøÀÔ·¹º§ ¼öÁ¤ \n");
		printf("  6. µî±Þº° ÀÏÀÏ»ç¿ë Á¦ÇÑ ½Ã°£ \n");
		printf("  7. µî±Þº° 1È¸ Á¢¼Ó Á¦ÇÑ ½Ã°£ \n");
		printf("  8. Á÷¾÷º° ºÐ·ù ÄÚµå ¼öÁ¤ \n");
		printf("  9. ÁÖ¼Òº° ºÐ·ù ÄÚµå ¼öÁ¤ \n");
		printf(" 10. °Ô½ÃÆÇ/µ¿È£È¸ ÃßÃµÄÚµå\n");
		n=numselect();xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.levelmode =gtt_bit(cfg.levelmode , 1);
		if (n== 2) cfg.levelmax  =get_num(cfg.levelmax	, 8);
		if (n== 3) cfg.clublevel =get_num(cfg.clublevel , 3);
		if (n== 4) edit_atype(1);
		if (n== 5) edit_atype(2);
		if (n== 6) edit_atype(3);
		if (n== 7) edit_atype(4);
		if (n== 8) edit_ajob();
		if (n== 9) edit_addr();
		if (n==10) edit_gg();
	}
}



job_jpg()
{
	while (1) {
		cls();
		print_(0,"È­»ó°ü·Ã ¼³Á¤");
		printf(" 1. ÀÚ·á½Ç¿¡¼­ SEE »ç¿ë°¡´É       :%s\n",rbit(cfg.jpgmode,0, "Çã¿ë", "ºÒÇã"));
		printf(" 2. È­»óÀÚ·á JPG ¹®ÀÚ ¢À          :%s\n",cfg.jpgchar);
		printf(" 3. È­»ó°Ô½ÃÆÇ¿¡¼­ JPG ¹®ÀÚ Ç¥½Ã  :%s\n",rbit(cfg.jpgmode,1, "Ç¥½Ã", "°ø¹é"));
		printf(" 4. ÀÏ¹Ý°Ô½ÃÆÇ¿¡¼­ JPG ¹®ÀÚ Ç¥½Ã  :%s\n",rbit(cfg.jpgmode,2, "Ç¥½Ã", "°ø¹é"));
		printf(" 5. ÀÏ¹ÝÀÚ·á½Ç¿¡¼­ JPG ¹®ÀÚ Ç¥½Ã  :%s\n",rbit(cfg.jpgmode,3, "Ç¥½Ã", "°ø¹é"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 0);
		if (n== 2)	get_str(cfg.jpgchar,2);
		if (n== 3)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 1);
		if (n== 4)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 2);
		if (n== 5)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 3);
	}
}





job_war()
{
	while (1) {
		cls();
		printf(" 1.  ¸Þ  ÀÎ ÃÖ¼Òº¸È£¸Á  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 2.  µ¿È£È¸ ÃÖ¼Òº¸È£¸Á  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 3.  È¸  ¿ø ÃÖ¼Òº¸È£¸Á  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 4.  ÀüÅõ´ë ÃÖ¼Òº¸È£¸Á  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 5.  ¼Ò¸ðÀÓ ÃÖ¼Òº¸È£¸Á  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 6.  È¸¿ø ÀÏÀÏÃÖ´ë¼Õ½Ç  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 7.  È¸¿ø ÀÏÀÏÃÖ´ëÀÌµæ  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printfL2();
		n=numselect();
		xy(60,n);
		if (n<= 0)	break;
	}
}

job_mess()
{
	while (1) {
		cls();
		printf(" ### ¸Þ¼¼Áö °ü·Ã ¼³Á¤\n");
		printf("  1.  ÅÚ³Ý½Ã ¸Þ¼¼Áö Àü¼Û             :%s\n",rbit(cfg.messmode,1,"Çã¿ë", "±ÝÁö"));
		printf("  2.  ±Û¾²±â½Ã ¸Þ¼¼Áö Àü¼Û           :%s\n",rbit(cfg.messmode,4,"Çã¿ë", "±ÝÁö"));
		printf("  3.  ´ëÈ­½Ã ¸Þ¼¼Áö Àü¼Û             :%s\n",rbit(cfg.messmode,5,"Çã¿ë", "±ÝÁö"));
		printf("  4.  ¿ÜºÎ¸í·É½Ã ¸Þ¼¼Áö Àü¼Û         :%s\n",rbit(cfg.messmode,6,"Çã¿ë", "±ÝÁö"));
		printf("  5.  ¼Õ´ÔÁ¢¼Ó½Ã     Àá±ñµµ¿ò¸»      :%s\n",rbit(cfg.messmode,2,"º¸ÀÓ", "¹«½Ã"));
		printf("  6.  ÀÏ¹ÝÈ¸¿øÁ¢¼Ó½Ã Àá±ñµµ¿ò¸»      :%s\n",rbit(cfg.messmode,3,"º¸ÀÓ", "¹«½Ã"));
		printf("  7.  Àá±ñµµ¿ò¸» º¸ÀÏ ·¹º§           :%d\n",cfg.helplevel);
		printf("  8.  Àá±ñµµ¿ò¸» º¸ÀÏ µî±Þ           :%d\n",cfg.helptype);
		printf("  9.  Àá±ñµµ¿ò¸» º¸ÀÏ Á¢¼Ó¼ö         :%d\n",cfg.helplog);
		printf(" 10.  ÀüÃ¼ ¸Þ¼¼Áö »ç¿ë µî±Þ          :%d\n",cfg.allsay1);
		printf(" 11.  ÀüÃ¼ ¸Þ¼¼Áö »ç¿ë µî±Þ(¸ÞÀÎ)    :%d\n",cfg.allsay2);
		printf(" 12.  ÀüÃ¼ ¸Þ¼¼Áö »ç¿ë µî±Þ(µ¿È£È¸)  :%d\n",cfg.allsay3);
		printf(" 13.  °°Àº È£½ºÆ® -> xmessd Ã³¸®     :%s\n",rbit(cfg.messmode,7,"tty»ç¿ë", "xmessd»ç¿ë"));
		printf(" 14.  MESS/SAY ½Ã ¼±ÅÃ¹æ¹ý           :%s\n",rbit(cfg.messmode,8,"US È­¸é","¸í´Ü¸ñ·Ï"));
		printf(" 20.  US °ü·Ã ¼³Á¤               \n");
		n=numselect();
		printf("\n");
		xy(60,n);
		if (n<= 0)	break;
		if (n== 1)	cfg.messmode=gtt_bit(cfg.messmode,1);
		if (n== 2)	cfg.messmode=gtt_bit(cfg.messmode,4);
		if (n== 3)	cfg.messmode=gtt_bit(cfg.messmode,5);
		if (n== 4)	cfg.messmode=gtt_bit(cfg.messmode,6);
		if (n== 5)	cfg.messmode=gtt_bit(cfg.messmode,2);
		if (n== 6)	cfg.messmode=gtt_bit(cfg.messmode,3);
		if (n== 7)	cfg.helplevel=get_num(cfg.helplevel,3);
		if (n== 8)	cfg.helptype =get_num(cfg.helptype,3);
		if (n== 9)	cfg.helplog  =get_num(cfg.helplog,3);
		if (n==10)	cfg.allsay1  =get_num(cfg.allsay1,2);
		if (n==11)	cfg.allsay2  =get_num(cfg.allsay2,2);
		if (n==12)	cfg.allsay3  =get_num(cfg.allsay3,2);
		if (n==13)	cfg.messmode=gtt_bit(cfg.messmode,7);
		if (n==14)	cfg.messmode=gtt_bit(cfg.messmode,8);
		if (n==20)	job_us();
	}
}

//ÁøÀÔ °ü·Ã ÀÛ¾÷
job_start()
{
	while (1) {
		cls();
		print_(1,"ÁøÀÔ °ü·Ã ¼³Á¤");
		printf("  1. ÁøÀÔ½Ã ¹Ù·Î ¸ÞÀÎ¸Þ´º ÁøÀÔ                 :%s\n",rbit(cfg.startmode,1,"º¸Åë","¹Ù·ÎÁøÀÔ"));
		printf("  2. ÁøÀÔ½Ã ½ÃÀÛ ·Î°í Ãâ·Â                     :%s\n",rbit(cfg.startmode,2,"Ãâ·Â","¹«½Ã"));
		printf("  3. ÁøÀÔ½Ã ÀüÃ¼ ÀÓ½Ã ID ÀÚµ¿ ºÎ¿©             :%s\n",rbit(cfg.startmode,3,"Ãâ·Â","¹«½Ã"));
		printf("  4. ¾ÆÀÌµðÀÔ·ÂÈÄ ÃÖÁ¾Á¢¼Ó  Ãâ·Â               :%s\n",rbit(cfg.startmode,4,"Ãâ·Â","¹«½Ã"));
		printf("  5. ¾ÆÀÌµðÀÔ·ÂÈÄ ÇöÀç½Ã°£  Ãâ·Â               :%s\n",rbit(cfg.startmode,5,"Ãâ·Â","¹«½Ã"));
		printf("  6. ¾ÆÀÌµðÀÔ·ÂÈÄ »ýÀÏÈ®ÀÎ  Ãâ·Â               :%s\n",rbit(cfg.startmode,9,"Ãâ·Â","¹«½Ã"));
		printf("  7. ¾ÆÀÌµðÀÔ·ÂÈÄ »ýÃ¼¸®µë  Ãâ·Â               :%s\n",rbit(cfg.startmode,6,"Ãâ·Â","¹«½Ã"));
		printf("  8. ¾ÆÀÌµðÀÔ·ÂÈÄ »ýÃ¼¼³¸í  Ãâ·Â               :%s\n",rbit(cfg.startmode,10,"Ãâ·Â","¹«½Ã"));
		printf("  9. ¾ÆÀÌµðÀÔ·ÂÈÄ ¸ÞÀÏÈ®ÀÎ  Ãâ·Â               :%s\n",rbit(cfg.startmode,7,"Ãâ·Â","¹«½Ã"));
		printf(" 10. ¾ÆÀÌµðÀÔ·ÂÈÄ EnterÅ°   ÀÔ·Â               :%s\n",rbit(cfg.startmode,8,"Enter","1ÃÊ´ë±â"));
		printf(" 11. Á¢¼Ó½Ã 7ÀÏµ¿¾È ¿¬¼ÓÁ¢¼Ó¿©ºÎ È®ÀÎ          :%s\n",rbit(cfg.startmode,11,"¾ÈÇÔ","È®ÀÎÇÔ"));
		printf(" 12. ¿¬¼ÓÁ¢¼Ó½Ã ·¹º§/´Ù¿î·® Áõ°¡Ä¡ (system/contday.dat)\n");
		printf(" 13. °øÁöÀü EnterÅ°   ÀÔ·Â                     :%s\n",rbit(cfg.startmode,13,"Enter","¹«½Ã"));
		printf(" 20. ÁøÀÔ½Ã ÀÚµ¿½ºÅ©¸³ÅÍ                       :%s\n",rbit(cfg.startmode,14,"µ¿ÀÛ","¹«½Ã"));
		printf(" 21. ÁøÀÔ½Ã ´ëÈ­½Ç Á¢¼Ó È®ÀÎ                   :%s\n",rbit(cfg.startmode,15,"µ¿ÀÛ","¹«½Ã"));
		printf(" 22. ÁøÀÔ½Ã ÇÏµå ÀÜ·® 50MB È®ÀÎ                :%s\n",rbit(cfg.startmode,16,"¹«½Ã","È®ÀÎ"));
		printf(" 23. Á¾°á½Ã Y/n Áú¹®ÇÔ                         :%s\n",rbit(cfg.mode,6, "Áú¹®", "¹«½Ã"));
		printf(" 24. Á¾·á½Ã È­¸é Ãâ·Â                          :%s\n",rbit(cfg.logomode,22,"º¸Åë","¾ÈÇÔ"));
		printf(" 25. ½Ã°£Áö¿¬ Á¾·á½Ã Á¾·á È­¸é¾øÀÌ Á¾·á        :%s\n",rbit(cfg.startmode,17,"º¸Åë","¾øÀ½"));
		n=numselect();
		if (n<= 0) break;
		if (n== 1) cfg.startmode=gtt_bit(cfg.startmode, 1);
		if (n== 2) cfg.startmode=gtt_bit(cfg.startmode, 2);
		if (n== 3) cfg.startmode=gtt_bit(cfg.startmode, 3);
		if (n== 4) cfg.startmode=gtt_bit(cfg.startmode, 4);
		if (n== 5) cfg.startmode=gtt_bit(cfg.startmode, 5);
		if (n== 6) cfg.startmode=gtt_bit(cfg.startmode, 9);
		if (n== 7) cfg.startmode=gtt_bit(cfg.startmode, 6);
		if (n== 8) cfg.startmode=gtt_bit(cfg.startmode,10);
		if (n== 9) cfg.startmode=gtt_bit(cfg.startmode, 7);
		if (n==10) cfg.startmode=gtt_bit(cfg.startmode, 8);
		if (n==11) cfg.startmode=gtt_bit(cfg.startmode,11);
		if (n==12) xedit2("system/contday.dat");
		if (n==13) cfg.startmode=gtt_bit(cfg.startmode,13);
		if (n==20) cfg.startmode=gtt_bit(cfg.startmode,14);
		if (n==21) cfg.startmode=gtt_bit(cfg.startmode,15);
		if (n==22) cfg.startmode=gtt_bit(cfg.startmode,16);
		if (n==23) cfg.mode 	=gtt_bit(cfg.mode, 6);
		if (n==24) cfg.logomode =gtt_bit(cfg.logomode,22);
		if (n==25) cfg.startmode=gtt_bit(cfg.startmode,17);
	}
}



job_cmd()
{
	while (1) {
		cls();
		print_(1,"¸í·É/ÀÔ·Â °ü·Ã ¼³Á¤");
		printf(" 1. ¸ÞÀÎ¸Þ´º¿¡¼­ P»ç¿ë½Ã Á¾·á ¿©ºÎ            :%s\n",rbit(cfg.mode,30,"º¸Åë","Á¾·á"));
		printf(" 2. ¸í·ÉÀÔ·Â½Ã ÀÚµ¿¿µ¹®ÀüÈ¯                   :%s\n",rbit(cfg.mode,4,"ÀÚµ¿º¯È¯","¼öµ¿º¯È¯"));
		printf(" 3. ¸í·É¾î ÀÚµ¿ ±â¾ï(È­»ìÇ¥ÀÌµ¿) ±â´É         :%s\n",rbit(cfg.mode,8,"»ç¿ë","¾ÈÇÔ"));
		printf(" 4. ¼­ºê¸Þ´º ÇÏ´Ü¸í·ÉÀÔ·ÂÀ§Ä¡ °íÁ¤            :%s\n",rbit(cfg.mode,11,"°¡º¯","°íÁ¤"));
		printf(" 5. ´ÜÃà¸í·ÉÅ° ÁöÁ¤         (system/shortcmd.dat)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.mode=gtt_bit(cfg.mode,30);
		if (n== 2) cfg.mode=gtt_bit(cfg.mode,4);
		if (n== 3) cfg.mode=gtt_bit(cfg.mode,8);
		if (n== 4) cfg.mode=gtt_bit(cfg.mode,11);
		if (n== 5) xedit2("system/shortcmd.dat");
	}
}





job_chat()
{
	while (1) {
		cls();
		print_(0,"´ëÈ­½Ç¼³Á¤");
		printf("  1.  ´ëÈ­½Ç °æ°í½ÃÀÛ½Ã°£                    :%d\n",cfg.chatwarn   );
		printf("  2.  ´ëÈ­½Ç ÁøÀÔ°¡´Éµî±Þ                    :%d\n",cfg.chattype   );
		printf("  3.  ¸Þ¼¼Áö Àü¼Û½Ã º¸³½ID ¹ÝÀüÇÔ            :%s\n",rbit(cfg.mode,16, "º¸Åë", "¹ÝÀü"));
		printf("  4.  µ¿È£È¸ ´ëÈ­½Ç STÇ¥½Ã                   :%s\n",rbit(cfg.mode,23, "Ç¥½Ã", "°¨Ãã"));
		printf("  5.  ´ëÈ­½Ç ºÐ·ùº°¼±ÅÃ±â´É                  :%s\n",rbit(cfg.chatmode,0, "¼±ÅÃ", "ÅëÇÕ"));
		printf("  6.  ¼Õ´Ô   ÀüÃ¼ ´ëÈ­½Ç »ç¿ë                :%s\n",rbit(cfg.chatmode,13, "ºÒ°¡´É", "°¡´É"));
		printf("  7.  ´ëÈ­½Ç ÁøÀÔ½Ã º°¸í»ç¿ë                 :%s\n",rbit(cfg.chatmode,14, "º°¸í", "º»¸í"));
		printf("  8.  ¼Õ´Ô½Ã 99´ëÈ­½Ç »ç¿ë                   :%s\n",rbit(cfg.chatmode,15, "ºÒ°¡´É", "°¡´É"));
		printf("  9.  ´ëÈ­½Ç ÁøÀÔ½Ã ºñ »ç¿ëÀÚ Á¦°Å           :%s\n",rbit(cfg.chatmode,16, "Á¦°Å", "¹«½Ã"));
		printf(" 10.  ´ëÈ­½Ç ÁøÀÔ½Ã ÃÊ±â ¸ðµå (0~12)         :%d\n",cfg.chatst);
		printf(" 11.  SAY½ÃÃÖ´ëÀü¼ÛÁÙ¼ö(0:¹«Á¦ÇÑ 1-255:Á¦ÇÑ) :%d\n",cfg.sayline);
		printf(" 12.  ´ëÈ­½Ç¿¡¼­ Å¸ÀÚ¹æ µ¿ÀÛ¿©ºÎ             :%s\n",rbit(cfg.chatmode,18, "¸ðµç´ëÈ­½Ç", "Å¸ÀÔ:71?"));
		printf(" 13.  ´ëÈ­½Ç¿¡¼­ ÄûÁî¹æ µ¿ÀÛ¿©ºÎ             :%s\n",rbit(cfg.chatmode,19, "¸ðµç´ëÈ­½Ç", "Å¸ÀÔ:72?"));
        printf(" 14.  www.3B.co.kr ÀÎÅÍ³Ý´ëÈ­½Ç ¿¬°á         :%s\n",rbit(cfg.chatmode,20, "¾ÈÇÔ","¸ðµç´ëÈ­½Ç"));
        printf(" 15.  chat.xhost.co.kr ÀÎÅÍ³Ý´ëÈ­½Ç ¿¬°á     :%s\n",rbit(cfg.chatmode,21, "¾ÈÇÔ","¸ðµç´ëÈ­½Ç"));
        printf(" 19.  SAY2 ¸í·É »ç¿ë °¡´É ¾ÆÀÌµð             (system/say2.id)\n");
		printf(" 20.  ´ëÈ­½Ç ÁøÀÔ±ÝÁö ID ÁöÁ¤                (system/nochat.id)\n");
		printf(" 21.  ´ëÈ­½Ç ±ÝÁö »çÀ¯ (Á¢¼ÓÈ¸¿ø)            (frm/error/chatoff.log)\n");
		printf(" 22.  ´ëÈ­½Ç ¹æÀå °¡´É ID (/sys,ÁøÀÔ°¡´É)    (system/chatsys.id)\n");
		printf(" 23.  ´ëÈ­½Ç ºÐ·ùº° ¼³Á¤ (ÁøÀÔÁ¦ÇÑ)          (system/chattype.dat)\n");
		n=numselect();
		xy(60,n+3);
		if (n<= 0)	break;
		if (n== 1)	cfg.chatwarn  =get_num(cfg.chatwarn ,6);
		if (n== 2)	cfg.chattype  =get_num(cfg.chattype ,2);
		if (n== 3)	cfg.mode=gtt_bit(cfg.mode,16);
		if (n== 4)	cfg.mode=gtt_bit(cfg.mode,23);
		if (n== 5)	cfg.chatmode=gtt_bit(cfg.chatmode,0);
		if (n== 6)	cfg.chatmode=gtt_bit(cfg.chatmode,13);
		if (n== 7)	cfg.chatmode=gtt_bit(cfg.chatmode,14);
		if (n== 8)	cfg.chatmode=gtt_bit(cfg.chatmode,15);
		if (n== 9)	cfg.chatmode=gtt_bit(cfg.chatmode,16);
		if (n==10)	cfg.chatst =get_num(cfg.chatst ,2);
		if (n==11)	cfg.sayline=get_num(cfg.sayline,3);
		if (n==12)	cfg.chatmode=gtt_bit(cfg.chatmode,18);
		if (n==13)	cfg.chatmode=gtt_bit(cfg.chatmode,19);
        if (n==14)  cfg.chatmode=gtt_bit(cfg.chatmode,20);
        if (n==15)  cfg.chatmode=gtt_bit(cfg.chatmode,21);
        if (n==19)  xedit2("system/say2.id");
		if (n==20)	xedit2("system/nochat.id");
		if (n==21)	xedit2("frm/error/chatoff.log");
		if (n==22)	xedit2("system/chatsys.id");
		if (n==23)	set_roomtype_limit();	  //ºÐ·ùº° Á¦ÇÑ¼³Á¤
	}
}

job_screen()
{
	char buff[300];
	while (1) {
		cls();
		printf(" ### È­¸é °ü·Ã È¯°æ¼³Á¤\n");
		printf("  1.  PF½Ã µî±Þ/·¹º§ Ç¥½Ã¿©ºÎ    :%s\n",rbit(cfg.mode,14,"Ç¥½Ã", "ºÒ°¡"));
		printf("  3.  ÇÏ´Ü ±¤°í º¸¿©ÁÜ           :%s\n",rbit(cfg.mode,27,"º¸ÀÓ", "¹«½Ã"));
		printf("  4.  »ç¿ë À¯µµ µµ¿ò¸»(today mess)¼³Á¤\n");
		printf("  5.  Á¾·á½Ã È­¸é Ãâ·Â           :%s\n",rbit(cfg.logomode,2,"º¸Åë","¾ÈÇÔ"));
		printf("  6.  ½ÃÀÛ½Ã È­¸é Ãâ·Â           :%s\n",rbit(cfg.logomode,1,"º¸Åë","¾ÈÇÔ"));
		printf("  7.  TOP ¾Ö´Ï¸ÞÀÌ¼Ç ·Î°í È­¸é   :%s\n",rbit(cfg.logomode,3,"¾ÈÇÔ","µ¿ÀÛÇÔ"));
		printf("  8.  ¾Ö´Ï¸ÞÀÌ¼Ç ÀüÃ¼È¸¿ø ÁöÁ¤   :%s\n",rbit(cfg.logomode,4,"°³ÀÎº°","°­Á¦µ¿ÀÛ"));
		printf("  9.  setlogo »ç¿ë°¡´É µî±Þ      :%d\n",cfg.no_logotype);
		printf(" 10.  US °ü·Ã ¼³Á¤               \n");
		printf(" 11.  ÇÏ´Ü¼±ÅÃ ¸í·É¾î ÀÐ±â       :%s\n",rbit(cfg.logomode,5,"FRMÆÄÀÏ","³»Àå¸í·É"));
		printf(" 12.  Á¢¼ÓÃâ·Â½Ã ÃÖ´ë Á¢¼Ó¼ö*¸ð¾ç:%d\n",cfg.limit_slog);
		printf(" 13.  ¼³Á¤ ¸í·É¼±ÅÃ½Ã ¼±ÅÃº¸ÀÌ±â :%s\n",rbit(cfg.logomode,8,"µÑ´Ùº¸ÀÓ","¼±ÅÃº¸ÀÓ"));
		printf(" 14.  [7m ¹ÝÀü ¾ÈÇÔ              :%s\n",rbit(cfg.logomode,9,"¹ÝÀüÇÔ","¾ÈÇÔ"));
		printf(" 15.  logodndn ±â´É(·Î°í´Ù¿î)    :%s\n",rbit(cfg.logomode,11,"»ç¿ë°ÅºÎ","»ç¿ëÇã°¡"));
		printf(" 90.  ÅëÇÕ È­¸é ¼³Á¤ ¸ðµå        (.scr)             \n");

		n=numselect();
		xy(60,n+1);
		if (n<=0)  break;
		if (n==1)  cfg.mode 	  =gtt_bit(cfg.mode,14);
		if (n==2)  cfg.mode 	  =gtt_bit(cfg.mode,17);
		if (n==3)  cfg.mode 	  =gtt_bit(cfg.mode,27);
		if (n==4)  job_todaymess();
		if (n==5)  cfg.logomode   =gtt_bit(cfg.logomode,2);
		if (n==6)  cfg.logomode   =gtt_bit(cfg.logomode,1);
		if (n==7)  cfg.logomode   =gtt_bit(cfg.logomode,3);
		if (n==8)  cfg.logomode   =gtt_bit(cfg.logomode,4);
		if (n==9)  cfg.no_logotype=get_num(cfg.no_logotype,2);
		if (n==10) job_us();
		if (n==11) cfg.logomode   =gtt_bit(cfg.logomode,5);
		if (n==12) cfg.limit_slog =get_num(cfg.limit_slog,3);
		if (n==13) cfg.logomode   =gtt_bit(cfg.logomode,8);
		if (n==14) cfg.logomode   =gtt_bit(cfg.logomode,9);
		if (n==15) cfg.logomode   =gtt_bit(cfg.logomode,11);
		if (n==90) {sys_tem("bin/xfrm 1 '%s' '%s' '%s'",club_go.code,go.code,now_id);}
	}
}


job_todaymess()
{
	int  check_today;		// ¿À´Ã¸¸ °Ë»ç?
	int  check_todayon; 	// Ãâ·Â¿©ºÎ°Ë»ç?
	char buff[300];
	int i;
	while (1) {
		cls();
		printf(" ### »ç¿ëÀ¯µµ µµ¿ò¸» ¼³Á¤\n");
		printf(" 1. »ç¿ë À¯µµ ¾È³» ¸Þ¼¼Áö help    :%s\n",rbit(cfg.logomode,6,"º¸¿©ÁÜ","¾Èº¸ÀÓ"));
		printf(" 2. »ç¿ë À¯µµ ¾È³» ALARM ÀÚµ¿Ãâ·Â :%s\n",rbit(cfg.logomode,7,"º¸Åë","ÀÚµ¿µ¿ÀÛ"));
		printf(" 3. »ç¿ë À¯µµ È®ÀÎ ±Û¾²±â ÄÚµå1:%s 2:%s 3:%s 4:%s 5:%s\n",cfg.check_wgo[0],cfg.check_wgo[1],cfg.check_wgo[2],cfg.check_wgo[3],cfg.check_wgo[4]);
		printf(" Ãâ·Â¿©ºÎ: ");
		for (i=0;i<32;i++) {
			printf("%2d.%s ",i+31,rbit2(cfg.check_today  ,i,"O","x"));
			if (i==4||i==14) printf("  ");
			if (i==9||i==19) printf("\n           ");
		}
		printf("\n ÀÏÀÏ°»½Å: ");
		for (i=0;i<32;i++) {
			printf("%2d.%s ",i+61,rbit2(cfg.check_todayon,i,"A","t"));
			if (i==4||i==14) printf("  ");
			if (i==9||i==19) printf("\n           ");
		}
		printf("\n");
		n=numselect();
		xy(60,n+1);
		if (n<=0) break;
		if (n==1) cfg.logomode	 =gtt_bit(cfg.logomode,6);
		if (n==2) cfg.logomode	 =gtt_bit(cfg.logomode,7);
		if (n==3) {for (i=0;i<5;i++) get_str(cfg.check_wgo[i],8);}
		if (n>=31&&n<=60) cfg.check_today  =gtt_bit(cfg.check_today  ,n-31);
		if (n>=61&&n<=90) cfg.check_todayon=gtt_bit(cfg.check_todayon,n-61);
	}
}


job_us()
{
	while (1) {
		cls();
		printf(" ### US °ü·Ã È¯°æ¼³Á¤\n");
		printf("  1.  ¸ÞÀÎ  ¿¡¼­ USÇ¥½Ã ¿©ºÎ     :%s\n",rbit(cfg.usermode,1, "°¡´É", "ºÒ°¡"));
		printf("  2.  ´ëÈ­½Ç¿¡¼­ USÇ¥½Ã ¿©ºÎ     :%s\n",rbit(cfg.usermode,2, "°¡´É", "ºÒ°¡"));
		printf("  3.  µ¿È£È¸¿¡¼­ USÇ¥½Ã ¿©ºÎ     :%s\n",rbit(cfg.usermode,3, "°¡´É", "ºÒ°¡"));
		printf("  4.  ¸Þ¼¼ÁöÀü¼Û USÇ¥½Ã ¿©ºÎ     :%s\n",rbit(cfg.usermode,4, "°¡´É", "ºÒ°¡"));
		printf("  5.  US½Ã µî±Þ/·¹º§ Ç¥½Ã¿©ºÎ    :%s\n",rbit(cfg.usermode,5, "Ç¥½Ã", "ºÒ°¡"));
		printf("  6.  US½Ã °£·«ÇÏ°Ô º¸¿©ÁÜ       :%s\n",rbit(cfg.usermode,6, "º¸Åë", "°£·«"));
		printf("  7.  US½Ã º»¸í´ë½Å º°¸íº¸¿©ÁÜ   :%s\n",rbit(cfg.usermode,7, "º°¸í", "º»¸í"));
		printf("  8.  US½Ã IPÁÖ¼Ò´ë½Å TTYº¸¿©ÁÜ  :%s\n",rbit(cfg.usermode,8, "IP ", "TTY"));
		printf("  9.  È­¸é³Ñ¾î°¥¶§ EnterÅ° ¸ØÃã  :%s\n",rbit(cfg.usermode,9, "¸ØÃã", "Åë°ú"));
		printf(" 10.  °­Á¦USÇ¥½Ã¹æ¹ýÁöÁ¤ (0~39)  :%d\n",cfg.usdisp);
		printf(" 11.  °­Á¦USÇ¥½Ã(¸Þ¼¼Áö´ë»ó¼±ÅÃ) :%d\n",cfg.usdisp2);
		printf(" 12.  ÀÏ¹Ý ±â·Ï½Ã US Á¤º¸ Á¤·Ä   :%s\n",rbit(cfg.usermode,11,"Á¤·Ä","¹«½Ã"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.usermode	=gtt_bit(cfg.usermode,1);
		if (n== 2)	cfg.usermode	=gtt_bit(cfg.usermode,2);
		if (n== 3)	cfg.usermode	=gtt_bit(cfg.usermode,3);
		if (n== 4)	cfg.usermode	=gtt_bit(cfg.usermode,4);
		if (n== 5)	cfg.usermode	=gtt_bit(cfg.usermode,5);
		if (n== 6)	cfg.usermode	=gtt_bit(cfg.usermode,6);
		if (n== 7)	cfg.usermode	=gtt_bit(cfg.usermode,7);
		if (n== 8)	cfg.usermode	=gtt_bit(cfg.usermode,8);
		if (n== 9)	cfg.usermode	=gtt_bit(cfg.usermode,9);
		if (n==10)	cfg.usdisp		=get_num(cfg.usdisp,2);
		if (n==11)	cfg.usdisp2 	=get_num(cfg.usdisp2,2);
		if (n==12)	cfg.usermode	=gtt_bit(cfg.usermode,11);
	}
}



job_system()
{
	while (1) {
		cls();
		printf("  ### ½Ã½ºÅÛ °ü·Ã ¼³Á¤\n");
		printf("  1.  È£½ºÆ®ÀÇ ÀÌ¸§                      :%s\n",cfg.hostname   );
		printf("  2.  Å° ÀÔ·Â ´ë±â ½Ã°£   (0~255ºÐ)      :%d(ºÐ)\n",cfg.waitkey);
		printf("  3.  H,C -> HELP ·Î ÀüÈ¯ ¿©ºÎ           :%s\n",rbit(cfg.mode,12,"º¸Åë", "HELP"));
		printf("  4.  Á¢¼Ó½Ã ¸Þ¼¼ÁöÆ÷Æ®½ÇÆÐTTY »èÁ¦      :%s\n",rbit(cfg.mode,26, "Ã³¸®", "¹«½Ã"));
		printf("  5.  µî±Þ18ÀÌ»ó Å°´ë±â ¼³Á¤             :%s\n",rbit(cfg.mode,0, "Á¦ÇÑ", "¹«Á¦ÇÑ"));
		printf("  6.  ½Ã°£Á¦ÇÑ¾ø´Â ID ÁöÁ¤               (system/nokey.id)\n");
		printf("  7.  ´ÜÃà¸í·ÉÅ° ÁöÁ¤ È­ÀÏ               (system/shortcmd.dat)\n" );
		printf("  8.  ¾Ë¶÷ ½Ã±×³¯ Ã³¸®                   :%s\n",rbit(cfg.mode,28, "µ¿ÀÛ", "ÁßÁö"));
		printf("  9.  È­¸é°»½Å½Ã ¾Ë¶÷½Ã±×³¯µ¿ÀÛ(´º½º..)  :%s\n",rbit(cfg.logomode,10, "µ¿ÀÛ", "ÁßÁö"));
		printf(" 10.  ¾Ë¶÷ ½Ã±×³¯ ÁÖ±â  (0~255)          :%d\n",cfg.alarm_time);
		printf(" 11.  Á¢¼Ó½Ã¿¡ Á¦ÇÑ½Ã°£ÃÊ°ú Á¢¼ÓÀÚÈ®ÀÎ   :%s\n",rbit(cfg.loginmode,1,"È®ÀÎ","Åë°ú"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	get_str(cfg.hostname  ,20);
		if (n== 2)	cfg.waitkey   =(byte)get_num(cfg.waitkey   ,3 );
		if (n== 3)	cfg.mode	  =gtt_bit(cfg.mode,12);
		if (n== 4)	cfg.mode	  =gtt_bit(cfg.mode,26);
		if (n== 5)	cfg.mode	  =gtt_bit(cfg.mode,0 );
		if (n== 6)	xedit("system/nokey.id");
		if (n== 7)	xedit("system/shortcmd.dat");
		if (n== 8)	cfg.mode	  =gtt_bit(cfg.mode,28);
		if (n== 9)	cfg.logomode  =gtt_bit(cfg.logomode,10);
		if (n==10)	cfg.alarm_time=get_num(cfg.alarm_time,3);
		if (n==11)	cfg.loginmode =gtt_bit(cfg.loginmode,1);
	}
	del_space(cfg.sysop);
}


//½Ã½ºÅÛ °ü·Ã ¼¼ÆÃ ÁöÁ¤  (Æ¯Á¤ ¸í·É ÀÔ·Â ¸¶´Ù È®ÀÎÇÔ)
job_limit()
{
	int n;
	char buff[200];
	FILE *fp;
	while (1) {
		cls();
		printf(" ###  ½Ã½ºÅÛ ±ÝÁö Á¦ÇÑ ¼³Á¤ °ü·Ã\n");
		printf("  1.  ÀÎÅÍ³Ý Àü¿ë ³ëµå ¼³Á¤ (³×Æ®¿öÅ©¼³Á¤)     (system/node_net.txt)\n");
		printf("  4.  ¿ÜºÎ ¸Þ´º »ç¿ë ±ÝÁö »çÀ¯                 (logo/_runoff.log)\n");
		printf("  5.  ÅÚ³Ý ¸Þ´º ±ÝÁö »çÀ¯                      (frm/error/teloff.log)\n");
		printf("  6.  ÀÓ½Ã ½Ã»ð ¸Þ¼¼Áö (Á¢¼ÓÈ¸¿ø)              (logo/_sysmess.dat)\n");
		printf("  7.  ³ëµåº°,Æ÷Æ®º° Á¢¼Ó½Ã Á¢¼Ó±ÝÁö/Çã¿ë¼³Á¤    \n");
		printf("  8.  ÁøÀÔ°¡´É/Á¦ÇÑ ¸Þ´º ÁöÁ¤                  (system/_nocode.dat) \n" );
		printf("  9.  ÁøÀÔ°¡´É/Á¦ÇÑ È­¸é ÆíÁý                   \n" );
		printf(" 10.  ÀüÃ¼ ÁøÀÔ Á¦ÇÑ µî±Þ ÁöÁ¤                 %d\n",cfg.limittype);
		printf(" 11.  ÀüÃ¼ ÁøÀÔ Á¦ÇÑ »çÀ¯ ÁöÁ¤                 (frm/limit/limit.log)\n");
		printf(" 12.  ½Ã°£´ëº°/¿äÀÏº° ¹«Á¦ÇÑ È¸¿øµî±Þ          %d\n",cfg.freelogin);
		printf(" 13.  µ¿½Ã Á¢¼Ó °¡´É µî±Þ                      %d\n",cfg.relog);
		printf(" 14.  µ¿½Ã Á¢¼Ó½Ã ¸Þ¼¼Áö Ãâ·Â                  %s\n",rbit(cfg.mode,7,"ÇÔ", "¾ÈÇÔ"));
		printf(" 15.  MARK ±â´É »ç¿ë°¡´É                       %s\n",rbit(cfg.mode,5,"°¡´É", "±ÝÁö"));
		printf(" 16.  ÅÚ³Ý   ÁøÀÔ °¡´É µî±Þ                    %d\n",cfg.telnettype);
		printf(" 17.  Æ¯Á¤ ¸í·É µ¿ÀÛ ±ÝÁö ÁöÁ¤                 (system/nocode.txt)\n");
		printf(" 18.  ÁøÀÔ ÃÖ´ë ÀÎ¿ø¼ö (frm/limit/full.log)    %d\n",cfg.limit_login);
		printf(" 19.  W¾²±â RÀÐ±â D´Ù¿î C´ëÈ­ Sµ¿È£È¸ ±ÝÁö/Á¦ÇÑ(system/limit.id)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) xedit("system/node_net.txt");
		if (n== 4) xedit("logo/_runoff.log");
		if (n== 5) xedit("frm/error/teloff.log");
		if (n== 6) {
			printf(" Á¢¼ÓÈ¸¿ø¿¡°Ô º¸¿©ÁÙ ÀÓ½Ã ¸Þ¼¼Áö¸¦ ÀÔ·ÂÇÏ¼¼¿ä.. \n>>");
			lineinput(buff,76);
			if (buff[0]==0 ) unlink("logo/_sysmess.dat");
			else {
				fp=fopen("logo/_sysmess.dat","w");
				fprintf(fp,"%s",buff);
				if (fp!=NULL) fclose (fp);
			}
		}
		if (n== 7) job_node();
		if (n== 8) set_no_code();
		if (n== 9) set_no_logo();
		if (n==10) cfg.limittype=get_num(cfg.limittype,2);
		if (n==11) xedit("frm/limit/limit.log");
		if (n==12) cfg.freelogin=get_num(cfg.freelogin,2);
		if (n==13) cfg.relog	=get_num(cfg.relog	  ,2);
		if (n==14) cfg.mode 	=gtt_bit(cfg.mode,7);
		if (n==15) cfg.mode 	=gtt_bit(cfg.mode,5);
		if (n==16) cfg.telnettype =get_num(cfg.telnettype,2);
		if (n==17) xedit("system/nocode.txt");
		if (n==18) cfg.limit_login=get_num(cfg.limit_login,4);
		if (n==19) set_limit_id();
	}
}


set_limit_id()
{
	FILE *fp;
	char buff[300],buff1[100],buff2[100],buff3[100];
	char filename[100],filename2[100];
	int n;
	while (1) {
		cls();
		printf(" ### W¾²±â RÀÐ±â D´Ù¿î C´ëÈ­ Sµ¿È£È¸ ±ÝÁöID\n");
		printf(" 1.  W¾²±â RÀÐ±â D´Ù¿î C´ëÈ­ Sµ¿È£È¸ ±ÝÁöID   (system/limit.id)\n");
		printf(" 2.  W¾²±â ¸Þ¼¼Áö (frm/pf/notwrite.log)\n");
		printf(" 3.  RÀÐ±â ¸Þ¼¼Áö (frm/pf/notread.log)\n");
		printf(" 4.  D´Ù¿î ¸Þ¼¼Áö (frm/pf/notdown.log)\n");
		printf(" 5.  C´ëÈ­ ¸Þ¼¼Áö (frm/pf/notchat.log)\n");
		printf(" 6.  Sµ¿È£ ¸Þ¼¼Áö (frm/pf/notclub.log)\n");
		printf(" 7.  ÀÏ¹Ý°Ô½ÃÆÇ ±ÛÀÐ±â°¡´É µî±Þ :%d\n",cfg.limit_r);
		printf(" 8.  ÀÏ¹Ý°Ô½ÃÆÇ ±Û¾²±â°¡´É µî±Þ :%d\n",cfg.limit_w);
		printf(" 9.  ÀÏ¹ÝÀÚ·á½Ç µî·Ï  °¡´É µî±Þ :%d\n",cfg.limit_u);
		printf("10.  ÀÏ¹ÝÀÚ·á½Ç ´Ù¿î  °¡´É µî±Þ :%d\n",cfg.limit_d);
		printf("11.  ¼­¹ö¸Þ´º(M)  ÁøÀÔ°¡´É µî±Þ :%d\n",cfg.limit_m);
		printf("12.  ¾÷/´Ù¿î/¾²±â ±ÝÁö »çÀ¯ (logo/_downoff.log)\n");
		printf("13.  ÀÐ±â ±ÝÁö         »çÀ¯ (logo/_readoff.log)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
			 if (n==1) edit_limit_id_file();
		else if (n==2) xedit2("frm/pf/notwrite.log");
		else if (n==3) xedit2("frm/pf/notread.log");
		else if (n==4) xedit2("frm/pf/notdown.log");
		else if (n==5) xedit2("frm/pf/notchat.log");
		else if (n==6) xedit2("frm/pf/notclub.log");
		else if (n== 7)  cfg.limit_r=get_num(cfg.limit_r,2);
		else if (n== 8)  cfg.limit_w=get_num(cfg.limit_w,2);
		else if (n== 9)  cfg.limit_u=get_num(cfg.limit_u,2);
		else if (n==10)  cfg.limit_d=get_num(cfg.limit_d,2);
		else if (n==11)  cfg.limit_m=get_num(cfg.limit_m,2);
		else if (n==12) xedit2("logo/_downoff.log");
		else if (n==13) xedit2("logo/_readoff.log");
	}
}


//³ëµå °ü·Ã ÁöÁ¤
job_node()
{
	int n,n2;
	char buff[100];
	n=n2=0;
	while (1) {
		cls();
		printf(" ###      ³ëµåº°,Æ÷Æ®º° Á¢¼Ó½Ã Á¢¼Ó±ÝÁö/Çã¿ë¼³Á¤         \n");
		printf("  1.  ÀÎÅÍ³ÝÀü¿ë ³ëµå ¸®½ºÆ®                        (system/node_net.txt)\n");
		printf("  2.  ³ëµåº° Á¢¼Ó ½Ã°£ ÁöÁ¤                         (system/nodetime.txt)\n");
		printf("  3.  ³ëµåº° Á¢¼Ó µî±Þ ÁöÁ¤                         (system/nodetype.txt)\n");
		printf("  4.  ³ëµåº° ½Ã»ð ¸Þ¼¼Áö ÁöÁ¤                       (system/nodemess.txt)\n");
		printf("  5.  ³ëµåº° »ç¿ë ºÒ°¡´É ¸Þ´º ÁöÁ¤                  (system/nodemenu.txt)\n");
		printf("  6.  ³ëµåº° Æ¯Á¤ ID¸¸ Á¢¼Ó°¡´É                     (system/nodeid.txt  )\n");
		printf("  7.  ³ëµåº° Æ¯Á¤ È£½ºÆ®¸¸ Á¢¼Ó °¡´É                (system/nodehost.txt)\n");
		printf("  9.  Á¢¼ÓÁ¦ÇÑ ¹«Á¦ÇÑ ¾ÆÀÌµð ÁöÁ¤                   (system/nodelog.id  \n");
		printf(" 10.  Á¢¼Ó±ÝÁö ³ëµå,È£½ºÆ®¸í ÁöÁ¤  (µî±Þº°¾²±â±ÝÁö) (system/nodety?.txt \n");
		printf(" 11.  Á¢¼Ó±ÝÁö ³ëµå,È£½ºÆ®¸í ÁöÁ¤  (Á¢¼ÓÁ¾·á)       (system/nodelog1.txt\n");
		printf(" 12.  Á¢¼Ó°¡´É ³ëµå,È£½ºÆ®¸í ÁöÁ¤                   (system/nodelog2.txt\n");
		printf(" 13.  ÁöÁ¤ ³ëµå,È£½ºÆ®¸í ¹«Á¶°Ç ¼Õ´ÔÁöÁ¤            (system/nodelog3.txt\n");
		printf(" 14.  ¾²±â±ÝÁö (¼Õ´ÔÁ¢¼Ó)³ëµå¾È³»È­¸é               (logo/_nodeno0.log) \n");
		printf(" 15.  Á¢¼Ó±ÝÁö ³ëµå,È£½ºÆ®¸í ¾È³»È­¸é               (logo/_nodeno1.log) \n");
		printf(" 16.  ¿äÀÏº° ¹«Á¦ÇÑ »ç¿ë ¾ÆÀÌµð                     (system/_day.id7)\n");
		printf(" 17.  ¿äÀÏº° »ç¿ë°¡´É ¾ÆÀÌµð ÁöÁ¤                   (system/_day.id?)\n");
		printf(" 18.  ¿äÀÏº° »ç¿ë ºÒ°¡´É½Ã ¾È³»È­¸é                 (logo/_dayid?.log)\n");
		printfL2();
		n=numselect();
		xy(60,n+1);
		if (n<=0) break;
		if (n==1) xedit("system/node_net.txt");
		if (n==2) xedit("system/nodetime.txt");
		if (n==3) xedit("system/nodetype.txt");
		if (n==4) xedit("system/nodemess.txt");
		if (n==5) xedit("system/nodemenu.txt");
		if (n==6) xedit("system/nodeid.txt");
		if (n==7) xedit("system/nodehost.txt");
		if (n==9) xedit("system/nodelog.id");
		if (n==10) {
			printf("\nÁ¦ÇÑµî±Þ? >>");
			n2=numinput();
			if (n2>=0&&n2<20) {
				sprintf(buff,"system/nodety%d.txt",n2);
				xedit(buff);
			}
		}
		if (n==11) xedit("system/nodelog1.txt");
		if (n==12) xedit("system/nodelog2.txt");
		if (n==13) xedit("system/nodelog3.txt");
		if (n==14) xedit("logo/_nodeno0.log");
		if (n==15) xedit("logo/_nodeno1.log");
		if (n==16) xedit("system/_day.id7");
		if (n==17||n==18) {
			printf("\n ¿äÀÏº° ¹øÈ£(0ÀÏ 1¿ù 2È­ 3¼ö 4¸ñ 5±Ý 6Åä >>");
			n2=numinput();
			if (n2>=0&&n2<6) {
				if (n==27) sprintf(buff,"system/_day.id%d",n2);
				else	   sprintf(buff,"logo/_dayid%d.log",n2);
				xedit(buff);
			}
		}
	}
}


// µ¿È£È¸ °ü·Ã ÀÛ¾÷
job_club()
{
	char buff[80];
	char buff2[80];
	int num;
	while (1) {
		cls();
		printfL2();
		printf("          µ¿È£È¸ °ü·Ã ÀÛ¾÷  \n");
		printfL2();
		printf(" 1.  ½Å±Ô µ¿È£È¸ ¸¸µé±â  \n");
		printf(" 2.  µ¿È£È¸ ¹é¾÷ ¸¸µé±â  \n");
		printf(" 3.  µ¿È£È¸ »èÁ¦ ÇÏ±â    \n");
		printf(" 4.  µ¿È£È¸ ½Ã»ð ¸Þ´º    \n");
		printf(" 5.  ÀüÃ¼ µ¿È£È¸ ÇöÈ²(°£´Ü)\n");
		printf(" 6.  ÀüÃ¼ µ¿È£È¸ ÇöÈ²(Åë°è)\n");
		printf(" 7.  ÀüÃ¼ µ¿È£È¸ ÇöÈ²(¼¼ºÎ)\n");
		printf(" 9.  µ¿È£È¸ ¸¸µé±â µµ¿ò¸»\n");
		printfL2();
		num=numselect();
		if (num<=0) break;
		if (num==1) {
			printf("\nÀÛ¾÷ÇÒ µ¿È£È¸ ÄÚµå´Â? >>");
			lineinput(buff,8);if (strlen(buff)<2) break;
			if (find_club(buff)) {
				printf("\n %s (%s) µ¿È£È¸´Â ÀÌ¹Ì %s µð·ºÅä¸®¿¡ ÀÖ½À´Ï´Ù.\n",
						buff,club_go.name,club_go.dir);
				printf(" %s µð·ºÅä¸®·Î µ¿È£È¸¸¦ ¸¸µé±î¿ä? (Y/n)>>",club_go.dir);
				if (yesno2("",Yes)) club_make(club_go.dir);
				else {
					mess("µ¿È£È¸ ÀÛ¾÷À» Ãë¼ÒÇÕ´Ï´Ù.");
				}

			}
			else {
				printf("\n *** %s µ¿È£È¸¸¦ ¸¸µé±î¿ä?  ",buff);
				if (yesno2(" (Y/n)>>",Yes)) {
					printf(" µ¿È£È¸ µð·ºÅä¸®´Â? >>");
					lineinput(buff2,60);printf("\n");
					if (strlen(buff2)>1) {
						club_make(buff2);
						mess("¸Þ´ºÆíÁý¿¡¼­ µ¿È£È¸¸¦ ÁöÁ¤ÇÏ¼¼¿ä.");
					}
				}
			}
		}
		if (num==2) {
			printf("\n¹é¾÷ÇÒ µ¿È£È¸ ÄÚµå´Â? >>");lineinput(buff,8);
			if (!find_club(buff)) {
				printf("\n %s µ¿È£È¸¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù.\n",buff);
			}
			else {
				printf("\n %s (%s) (%s) \n",club_go.code,club_go.name,club_go.dir);
				if (yesno("µ¿È£È¸¸¦ ¹é¾÷ÇÒ±î¿ä? (¹é¾÷:Y)",No)) {
					sys_tem2("tar cfvz %s.tgz %s &",club_go.code,club_go.dir);
				}
			}
		}
		if (num==3) {
			printf("\n»èÁ¦ÇÒ µ¿È£È¸ ÄÚµå´Â? >>");lineinput(buff,8);printf("\n");
			if (strlen(buff)<1);
			else if (!find_club(buff)) {
				mess("%s µ¿È£È¸¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù.",buff);
			}
			else {
				printf("\n %s (%s) (%s) \n",club_go.code,club_go.name,club_go.dir);
				if (yesno2("ÀÌ µ¿È£È¸ÀÇ ¸ðµç ÀÚ·á¸¦ »èÁ¦ÇÒ±î¿ä? (»èÁ¦:Y)",No)) {
					if (!yesno("ÇÑ¹ø »èÁ¦ÇÏ¸é Àý´ë º¹±¸ÇÒ¼ö ¾ø½À´Ï´Ù. Ãë¼ÒÇÒ±î¿ä? (Ãë¼Ò:Y)",Yes)) {
						printf("\n");
						sprintf(buff2,"rm %s -i -R",club_go.dir);
						sys_tem(buff2);
					}
				}
			}
		}
		if (num==4) {
			printf("\nÀÛ¾÷ÇÒ µ¿È£È¸ ÄÚµå´Â? >>");lineinput(buff,8);
			if (strlen(buff)<1);
			else if (find_club(buff)) sys_tem("bin/csysop %s %s %s",club_go.code,"top",cfg.sysop);
			else mess("%s µ¿È£È¸¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù. ¸ÞÀÎ ¸Þ´º È®ÀÎ",buff);
		}
		if (num==5) view_all_club(1);
		if (num==6) view_all_club(2);
		if (num==7) view_all_club(3);
		if (num==9) {view("doc/makeclub.txt");pressenter();}
	}
}



//¸ÞÀÏ°ü·Ã
job_mail()
{
	int n,n2,n3;
	char buff[100];
	while (1) {
		cls();
		printf("          ¸ÞÀÏ °ü·Ã ÀÛ¾÷\n");
		printfL2();
		printf("  1. ±â°£ Áö³­ ¸ÞÀÏ »èÁ¦                       \n");
		printf("  2. ÀÏ¹Ý¸ÞÀÏ »ç¿ë°¡´É µî±Þ                    :%d\n",cfg.mailwrite);
		printf("  3. E-MAIL »ç¿ë°¡´É µî±Þ                      :%d\n",cfg.emailwrite);
		printf("  4. E-MAIL Àü¿ë ¾ÆÀÌµð »ç¿ëÇÔ                 :%s\n",rbit(cfg.mailmode,1,"ÇÔ","¾ÈÇÔ"));
		printf("  5. ¸ÞÀÏ¹ÙÀÌ³Ê¸®»ç¿ë                          :%s\n",rbit(cfg.mailmode,2,"»ç¿ë¾ÈÇÔ","»ç¿ëÇÔ"));
		printf("  6. ÀüÃ¼¸ÞÀÏ ¹ß¼Û °¡´É µî±Þ                   :%d\n",cfg.wmalltype );
		printf("  7. ÀüÃ¼¸ÞÀÏ »ç¿ë °¡´É È¸¿ø ÁöÁ¤              (system/wmall.id)\n");
		printf("  8. ¸ÞÀÏ ÀÚµ¿ ´äÀå ID/³»¿ë ÁöÁ¤               (system/manswer.id)\n");
		printf("  9. ¸ÞÀÏ µµÂø½Ã º§¼Ò¸®                        :%s\n",rbit(cfg.mailmode,6,"¾È³¿","³¿"));
		printf(" 10. ÇÑ±Û Email ID °æ°íÇÔ.                     :%s\n",rbit(cfg.mailmode,2,"ÇÔ","¾ÈÇÔ"));
		printf(" 11. ÇÑ±Û Email ID °æ°í         ¾È³»È­¸é       (frm/mail/emailh.log)\n");
		printf(" 12. Email ¿ë ÁÖ¼ÒÁöÁ¤ (È£½ºÆ®¸í)              (system/hostaddr.txt)\n");
		printf(" 13. ¸ÞÀÏ»ç¿ë ±ÝÁö »çÀ¯ ÀÔ·Â                   (frm/mail/notmail.log)\n");
		printf(" 14. E¸ÞÀÏ»ç¿ë ±ÝÁö »çÀ¯ ÀÔ·Â                  (frm/mail/notemail.log)\n");
		printf(" 15. ¸ÞÀÏÀÐ±âÈÄ ÆíÁöº¸°üÇÔ »èÁ¦ Áú¹®           :%s\n",rbit(cfg.mailmode,7,"ÇÔ","¾ÈÇÔ"));
		printf(" 16. ¸ÞÀÏÀÐ±âÈÄ ÆíÁöº¸°üÇÔ ÀúÀå Áú¹®           :%s\n",rbit(cfg.mailmode,8,"ÇÔ","¾ÈÇÔ"));
		printf(" 17. ¸ÞÀÏÀÐ±âÈÄ ÆíÁöº¸°üÇÔ º¸°ü Áú¹®           :%s\n",rbit(cfg.mailmode,9,"ÇÔ","¾ÈÇÔ"));
		printf(" 18. ¸ÞÀÏÀÐ±âÈÄ ´äÀå(RE) -> ¿øº»ÆíÁöÃß°¡       :%s\n",rbit(cfg.mailmode,10,"ÇÔ","¾ÈÇÔ"));
		n=numselect();printf("\n");
		xy(60,n+2);
		if (n<=0) break;
		if (n==1) {
			xy(1,20);
			printf("  ÀÏ¹ÝÆíÁö ¸ÞÀÏ»èÁ¦ ±â°£Àº?  (5-999ÀÏ) >>");
			n2=numinput();printf("\n");
			if (n2>4&&n2<1000) {
				printf("  º¸°üÆíÁö ¸ÞÀÏ»èÁ¦ ±â°£Àº?  (30-999ÀÏ) >>");
				n3=numinput();printf("\n");
				if (n3>30&&n3<1000) {
				if (yesno2("±â°£°æ°ú ¸ÞÀÏ»èÁ¦¸¦ ½ÃÀÛÇÒ±î¿ä?(Y/n)>>",Yes))
					sys_tem2("bin/delmail _all_maildel %d %d &",n2,n3);
				}
			}
		}
		if (n== 2) cfg.mailwrite=get_num(cfg.mailwrite,1);
		if (n== 3) cfg.emailwrite=get_num(cfg.emailwrite,1);
		if (n== 4) cfg.mailmode=gtt_bit(cfg.mailmode,1);
		if (n== 5) cfg.mailmode=gtt_bit(cfg.mailmode,2);
		if (n== 6) cfg.wmalltype=get_num(cfg.wmalltype,2);
		if (n== 7) xedit("system/wmall.id");
		if (n== 8) xedit("system/manswer.id");
		if (n== 9) cfg.mailmode=gtt_bit(cfg.mailmode,6);
		if (n==10) cfg.mailmode=gtt_bit(cfg.mailmode,2);
		if (n==11) xedit("frm/mail/emailh.log");
		if (n==12) xedit("system/hostaddr.txt");
		if (n==14) xedit("frm/mail/notemail.log");
		if (n==15) cfg.mailmode=gtt_bit(cfg.mailmode,7);
		if (n==16) cfg.mailmode=gtt_bit(cfg.mailmode,8);
		if (n==17) cfg.mailmode=gtt_bit(cfg.mailmode,9);
		if (n==18) cfg.mailmode=gtt_bit(cfg.mailmode,10);
	}
}


job_backmode()
{
	int n;
	while (1) {
		cls();
		print_(0,"½Ã½ºÅÛ ¹é¾÷°ü·Ã ¼³Á¤");
		printf("  1.  ¸Þ´ºÆíÁý½Ã OLD,BAK ÀúÀå        :%s\n",rbit(cfg.backmode,1, "ÀúÀå", "¹«½Ã"));
		printf("  2.  Áö³­´Þ °æ°ú½Ã REC ÀÚµ¿¹é¾÷     :%s\n",rbit(cfg.backmode,2, "¹é¾÷", "¹«½Ã"));
		printf("  3.  È­¸éµî·Ï½Ã    È­¸éÀÚµ¿¹é¾÷     :%s\n",rbit(cfg.backmode,3, "¹é¾÷", "¹«½Ã"));
		printf("  4.  ½Ã»ð¸Þ´ºÁøÀÔ½Ã    ÀÚµ¿¹é¾÷     :%s\n",rbit(cfg.backmode,4, "¹é¾÷", "¹«½Ã"));
		printf("  5.  µ¿È£È¸  ÁøÀÔ½Ã    ÀÚµ¿¹é¾÷     :%s\n",rbit(cfg.backmode,5, "¹é¾÷", "¹«½Ã"));
		printf("  9.  ¹é¾÷\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.backmode  =gtt_bit(cfg.backmode,1);
		if (n== 2)	cfg.backmode  =gtt_bit(cfg.backmode,2);
		if (n== 3)	cfg.backmode  =gtt_bit(cfg.backmode,3);
		if (n== 4)	cfg.backmode  =gtt_bit(cfg.backmode,4);
		if (n== 5)	cfg.backmode  =gtt_bit(cfg.backmode,5);
		if (n== 9)	job_backup();
	}
}



set_no_code()
{
	char filename[200];
	char buff[100];
	FILE *fp,*fp2;
	int found=No,i;
	struct CCODE {
		char mode;
		char code[9];
	} ccode;

	sprintf(filename,"%s/system/_nocode.dat",club_go.dir);

	key=Yes;
	while (key) {
		cls();
		printf("           ÁøÀÔÁ¦ÇÑ °¡±âÄÚµå ¸ñ·Ï        °¡±âÄÚµå(Á¦ÇÑ¸ðµå)\n");
		if ((fp=fopen(filename,"r+"))!=NULL) {
			i=0;
			while (fread(&ccode,sizeof(ccode),1,fp)) {
				printf(" %-8s(%d) ",ccode.code,ccode.mode);
				if (i++%6==5) printf("\n");
			}
			if (fp!=NULL) fclose(fp);
			if (i%6!=0) printf("\n");
		}
		printfL2();
		while (key) {
			printf("  Ãß°¡:A <ÄÚµå> <¸ðµå>  »èÁ¦:D <ÄÚµå> ¸ñ·Ï:L Á¾°á:Q  ¼±ÅÃ>>");
			lineinput(buff,12);printf("\n");
			check_cmd(buff);
			if (is_char("QqPpXx",buff[0])) key=No;
			else if (is_char("Ll",buff[0])) break;
			else if (is_char("AaIi",buff[0])) {
				if (mmc[1][0]==0||atoi(mmc[2])==0) {
					printf("\n Á¦ÇÑ »ç¿ë¹ý : A <°¡±âÄÚµå> <¸ðµå>    ¿¹) A hello 1\n");
					printf("   °¡±âÄÚµå:Á¦ÇÑÇÒ ÄÚµå  ¸ðµå:ÁøÀÔÁ¦ÇÑ½Ã ¸Þ¼¼Áö Ãâ·Â \n");
				}
				else {
					strcpy(ccode.code,mmc[1]);
					i=atoi(mmc[2]);if (i>9) i=9;
					ccode.mode=i;
					if ((fp=fopen(filename,"a+"))==NULL) fp=fopen(filename,"w+");
					if (fp!=NULL) {
						fwrite(&ccode,sizeof(ccode),1,fp);
						printf(" %-8s(%d)  Ãß°¡µÊ\n",ccode.code,ccode.mode);
						fclose(fp);
					}
				}
			}
			else if (is_char("Dd",buff[0])) {
				if (mmc[1][0]==0) {
					printf("\n »èÁ¦ »ç¿ë¹ý : D <°¡±âÄÚµå>   ¿¹) D hello\n");
				}
				else {
					if ((fp=fopen(filename,"r+"))!=NULL) {
						if ((fp2=fopen(TMP_FILE,"w+"))!=NULL) {
							while (fread(&ccode,sizeof(ccode),1,fp)) {
								if (!is_same(ccode.code,mmc[1])&&ccode.mode>0&&ccode.code[0]!=0)
									fwrite(&ccode,sizeof(ccode),1,fp2);
							}
							fclose(fp);
							fclose(fp2);
							file_copy(TMP_FILE,filename);
							unlink(TMP_FILE);
						}
					}
				}
			}
		}
	}

}

set_no_logo()
{
	int n;
	char buff[100];
	char filename[200];
	while (1) {
		cls();
		printf("          ÁøÀÔ Á¦ÇÑ È­¸é ¸Þ¼¼Áö ÆíÁý\n");
		printfL2();
		printf("  1. ÁøÀÔÁ¦ÇÑ ¸Þ¼¼Áö 1         (logo/_nocode1.log)\n");
		printf("  2. ÁøÀÔÁ¦ÇÑ ¸Þ¼¼Áö 2         (logo/_nocode2.log)\n");
		printf("  3. ÁøÀÔÁ¦ÇÑ ¸Þ¼¼Áö 3         (logo/_nocode3.log)\n");
		printf("  4. ÁøÀÔÁ¦ÇÑ ¸Þ¼¼Áö 4         (logo/_nocode4.log)\n");
		printf("  5. ÁøÀÔÁ¦ÇÑ ¸Þ¼¼Áö 5         (logo/_nocode5.log)\n");
		printf("  6. ÁøÀÔÁ¦ÇÑ ¸Þ¼¼Áö 6         (logo/_nocode6.log)\n");
		printf("  7. ÁøÀÔÁ¦ÇÑ ¸Þ¼¼Áö 7         (logo/_nocode7.log)\n");
		printf("  8. ÁøÀÔÁ¦ÇÑ ¸Þ¼¼Áö 8         (logo/_nocode8.log)\n");
		printf("  9. ÁøÀÔÁ¦ÇÑ ¸Þ¼¼Áö 9         (logo/_nocode9.log)\n");
		printfL2();
		n=numselect();
		sprintf(filename,"%s/logo/_nocode%d.log",club_go.dir,n);
		printf("\n");
		if (n<= 0) break;
		if (n== 1) xedit(filename);
		if (n== 2) xedit(filename);
		if (n== 3) xedit(filename);
		if (n== 4) xedit(filename);
		if (n== 5) xedit(filename);
		if (n== 6) xedit(filename);
		if (n== 7) xedit(filename);
		if (n== 8) xedit(filename);
		if (n== 9) xedit(filename);
	}
}


job_money()
{
	FILE *fp;
	int i,n,num,n2;
	char buff[100];

	while (1) {
		cls();
		printf("  ###  »çÀÌ¹ö¸Ó´Ï °ü·Ã ÀÛ¾÷  ###\n");
		printf("  1. È¸¿ø ±Ý¾× »óÇÑ   (¿ø)                      :%d\n",cfg.cashmax    );
		printf("  2. ¹«Á¦ÇÑ ±Ý¾× »ç¿ë µî±Þ                      :%d\n",cfg.cashfree   );
		printf("  3. ÀÐ±â½Ã »çÀÌ¹ö¸Ó´Ï µ¿ÀÛ                     :%s\n",rbit(cfg.cashmode,0, "ÇÔ", "¾ÈÇÔ"));
		printf("  4. ´Ù¿î½Ã »çÀÌ¹ö¸Ó´Ï µ¿ÀÛ                     :%s\n",rbit(cfg.cashmode,1, "ÇÔ", "¾ÈÇÔ"));
		printf("  5. ¾²±â½Ã »çÀÌ¹ö¸Ó´Ï µ¿ÀÛ                     :%s\n",rbit(cfg.cashmode,2, "ÇÔ", "¾ÈÇÔ"));
		printf("  6. µî·Ï½Ã »çÀÌ¹ö¸Ó´Ï µ¿ÀÛ                     :%s\n",rbit(cfg.cashmode,3, "ÇÔ", "¾ÈÇÔ"));
		printf("  7. °Ô½ÃÆÇ»èÁ¦»çÀÌ¹ö¸Ó´Ïµ¿ÀÛ                   :%s\n",rbit(cfg.cashmode,4, "ÇÔ", "¾ÈÇÔ"));
		printf("  8. ÀÚ·á½Ç»èÁ¦»çÀÌ¹ö¸Ó´Ïµ¿ÀÛ                   :%s\n",rbit(cfg.cashmode,5, "ÇÔ", "¾ÈÇÔ"));
		printf("  9. 1È¸ Á¢¼Ó ÃÖ´ë ÀÌÀÍ CASH (0:¹«Á¦ÇÑ)         :%d\n",cfg.cashmax1);
		printf(" 10. 1ÀÏ Á¢¼Ó ÃÖ´ë ÀÌÀÍ CASH (0:¹«Á¦ÇÑ)         :%d\n",cfg.cashmax2);
		printf(" 11. »çÀÌ¹ö¸Ó´Ï 0ÀÌÇÏ È¸¿ø ´Ù¿î                 :%s\n",rbit(cfg.cashmode,6, "±ÝÁö", "Çã¿ë"));
		printf(" 12. ÀüÃ¼È¸¿ø »çÀÌ¹ö¸Ó´Ï ÀÏ°ýÁöÁ¤               \n");
		printfL2();
		n=numselect();
		xy(60,n);
		if (n<=0)  break;
		if (n==1)  cfg.cashmax	 =get_num(cfg.cashmax	,10);
		if (n==2)  cfg.cashfree  =get_num(cfg.cashfree	, 2);
		if (n==3)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 0);
		if (n==4)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 1);
		if (n==5)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 2);
		if (n==6)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 3);
		if (n==7)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 4);
		if (n==8) cfg.cashmode	 =gtt_bit(cfg.cashmode	, 5);
		if (n==9) cfg.cashmax1	 =get_num(cfg.cashmax1 ,10);
		if (n==10) cfg.cashmax2  =get_num(cfg.cashmax2 ,10);
		if (n==11) cfg.cashmode  =gtt_bit(cfg.cashmode	, 6);
		if (n==12) {
			printf("ÀüÃ¼È¸¿ø »çÀÌ¹öÄ³½Ã (´ÜÀ§ CC) :");
			lineinput(buff,9);
			n2=atoi(buff);
			if (buff[0]==0||n2<0) continue;
			xy(1,21);
			printf("\n Áö±ÝºÎÅÍ ÀüÃ¼È¸¿øÀÇ »çÀÌ¹öÄ³½Ã¸¦ ¼¼ÆÃÇÕ´Ï´Ù. °¥¹«¸® ÇÏ¼¼¿ä \n\n");
			printf("    ÀüÃ¼È¸¿øÀÇ »çÀÌ¹öÄ³½Ã¸¦ %dCC ·Î ¼¼ÆÃÇÒ±î¿ä? (Y/n)>>",n2);
			if (yesno("",Yes)==No) continue;
			fp=fopen(ID_FILE,"r+");
			i=0;
			printf("\n");
			while (1) {
				fseek(fp,i*sizeof_pf,SEEK_SET);
				if (!fread(&pf,sizeof_pf,1,fp)) break;
				printf("%-8s (%-8s)  %9d ->",pf.id,pf.name,pf.cash);
				pf.cash=n2;
				printf("%9d\n",pf.cash);
				fseek(fp,i*sizeof_pf,SEEK_SET);
				i++;
			}
			if (fp!=NULL) fclose(fp);
			pressenter();
		}
	}
}


job_ip()
{
	FILE *fp;
	int i,n,num,n2;
	char buff[100];

	while (1) {
		cls();
		printf("  ###  IP °ü·Ã ¼³Á¤  ###\n");
		printf("  1. IP ÀÚ·á ±â·Ï                  :%s\n",rbit(cfg.ipmode,0,"ÇÔ", "¾ÈÇÔ"));
		printfL2();
		n=numselect();
		xy(60,n);
		if (n<= 0)	break;
		if (n== 1) cfg.ipmode	 =gtt_bit(cfg.ipmode  , 0);
	}
}

save_guestnot(PF pf2,int mode)
{
	FILE *fp;
	int i;
	char filename[200];
	char buff[200];
	strcpy(filename,GUESTNOT_FILE);
	if (mode==1) {
		printf("\n µî·Ï°ÅºÎµî·Ï »çÀ¯ (Ãë¼Ò:Q)>>");
		lineinput(buff,60);check_han(buff);printf("\n");
		if (is_quit(buff)) return No;
		strcpy(pf.memo2,buff);
		mess("°ÅºÎÃ³¸®°¡ ÁöÁ¤ µÇ¾ú½À´Ï´Ù.\n");
	}
	fp=fopen(filename,"r+");   //°ÅºÎÀÚ¾ÆÀÌµð¿¡ Ãß°¡ÇÑ´Ù.
	if (fp==NULL) fp=fopen(filename,"w+");   //±âÁ¸¾ÆÀÌµð¿¡ Ãß°¡ÇÑ´Ù.
	if (fp==NULL) {
		mess("%s È­ÀÏÀ» ¿­¼ö ¾ø½À´Ï´Ù.",filename);
		return No;
	}
	fseekend(fp);
	i=ftell(fp)/sizeof_pf;
	if (i>=0) {
		pf2.num=i;
		fseek(fp,i*sizeof_pf,SEEK_SET);
//		  fwrite(&pf2,sizeof_pf,1,fp);
		gwrite(203,pf2,fp);
	}
	fclose (fp);
	return No;
}



notfree()
{
	FILE *fp;
	FCFG fcfg;
	int i,n,num,n2;
	while (1) {
		cls();
		printf("  ###  µî·Ï°ÅºÎ È¸¿ø Ãß°¡,ÇØÁ¦   ###\n");
		printf("  1. µî·Ï°ÅºÎÈ¸¿ø Ãß°¡        (½Å±ÔÁöÁ¤)\n");
		printf("  2. ±âÁ¸È¸¿ø -> µî·Ï°ÅºÎÈ¸¿ø (°ÅºÎÁöÁ¤)\n");
		printf("  3. ±âÁ¸È¸¿ø <- µî·Ï°ÅºÎÈ¸¿ø (°ÅºÎÇØÁ¦)\n");
		printf("  4. °ÅºÎÈ¸¿ø »èÁ¦            (°ÅºÎÃë¼Ò)\n");
		printf("  5. °ÅºÎÈ¸¿ø ÀüÃ¼ »èÁ¦       (È­ÀÏ»èÁ¦)\n");
		printf("  6. °ÅºÎÈ¸¿ø °Ë»ö            (°ÅºÎÃë¼Ò)\n");
		n=numselect();
		xy(60,n);
		if (n<= 0)	break;
	}
}



job_updown()
{
	int n,i;
	char buff[100];
	while (1) {
		cls();
		print_(0,"¾÷,´Ù¿î ¼³Á¤°ü·Ã");
		printf("  1. ¾÷  ->´Ù¿î¿ë·® º¸»ó                          :%3.1f ¹è\n",(float)cfg.urate/10 );
		printf("  2. ´Ù¿î->´Ù¿î¿ë·® °¨¼Ò                          :%3.1f ¹è\n",(float)cfg.drate/10 );
		printf("  3. ¹«Á¦ÇÑ ´Ù¿î µî±Þ                             :%d\n",cfg.downfree   );
		printf("  4. ¸ÖÆ¼´Ù¿î °¡´É µî±Þ                           :%d\n",cfg.downmult  );
		printf("  5. ´Ù¿î·®Á¶Á¤ ½Ã±â (DNÈÄ¸é Ãë¼Ò½Ã °¨¼Ò¾ÈµÊ)     :%s\n",rbit(cfg.downmode,0,"DNÀü","DNÈÄ"));
		printf("  6. ¾÷ÈÄ ÀÚµ¿¾ÐÃà                                :%s\n",rbit(cfg.downmode,10," ÇÔ ","¾ÈÇÔ"));
		printf("  7. ÀÚ·á µî·Ï È­ÀÏ¸íÀÚµ¿ÀÔ·Â                     :%s\n",rbit(cfg.downmode,1,"ÀÚµ¿","¼öµ¿"));
		printf("  8. ÀÚ·á µî·Ï½Ã ÀÏÀÏ »ç¿ë½Ã°£ º¸»ó (2¹è Áõ°¡)    :%s\n",rbit(cfg.downmode,2," ÇÔ ","¾ÈÇÔ"));
		printf("  9. È­ÀÏ¾ø´ÂÀÚ·á -> È­ÀÏ¸íºóÄ­Ã³¸®               :%s\n",rbit(cfg.downmode,11,"º¸Åë","ºóÄ­"));
		printf(" 10. UPS ¸í·É½Ã È®ÀÎ¾ÈÇÔ                          :%s\n",rbit(cfg.downmode,5,"È®ÀÎ","¾ÈÇÔ"));
		printf(" 11. ÆÄÀÏ´Ù¿îÁØºñ½Ã ÀÓ½Ã¿µ¿ª¿¡ ÆÄÀÏº¹»ç ¹æ¹ý      :%s\n",rbit(cfg.downmode,15,"º¹»ç","¸µÅ©"));
		printf(" 12. ¾÷´Ù¿î½Ã ·¹º§,¿ë·® Áõ°¡¸Þ¼¼Áö                :%s\n",rbit(cfg.downmode,17,"Ãâ·Â","¹«½Ã"));
		printf(" 13. ¾÷½Ã ÇÏµåÀÜ·®(50MB) È®ÀÎ                     :%s\n",rbit(cfg.downmode,20,"¹«½Ã","È®ÀÎ"));
		printf(" 14. ´Ù¿î½Ã ÇÏµåÀÜ·®(50MB) È®ÀÎ                   :%s\n",rbit(cfg.downmode,21,"¹«½Ã","È®ÀÎ"));
		printf(" 15. µ¿½Ã ¾÷   µî·Ï ¼ö Á¦ÇÑ  (0:¹«Á¦ÇÑ)           :%d\n",cfg.up_limit );
		printf(" 16. µ¿½Ã ´Ù¿î Àü¼Û ¼ö Á¦ÇÑ  (0:¹«Á¦ÇÑ)           :%d\n",cfg.dn_limit );
		printf(" 17. ¸ÖÆ¼ ´Ù¿î ¼ö      Á¦ÇÑ  (0:¹«Á¦ÇÑ)           :%d\n",cfg.dn_mult_limit );
		printf(" 18. ÀÚ·áµî·Ï½Ã Å©±âº° ·¹º§ º¸»ó (1Á¡´ç Byte)     :%d\n",cfg.level_size);
		printf(" 22. ´Ù¿î °ü·Ã ¸Þ¼¼Áö/±ÝÁö ¼³Á¤  \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.urate=get_num(cfg.urate,3);
		if (n== 2)	cfg.drate=get_num(cfg.drate,3);
		if (n== 3)	cfg.downfree   =get_num(cfg.downfree,2);
		if (n== 4)	cfg.downmult   =get_num(cfg.downmult,2);
		if (n== 5)	cfg.downmode   =gtt_bit(cfg.downmode,0);
		if (n== 6)	cfg.downmode   =gtt_bit(cfg.downmode,10);
		if (n== 7)	cfg.downmode   =gtt_bit(cfg.downmode,1);
		if (n== 8)	cfg.downmode   =gtt_bit(cfg.downmode,2);
		if (n== 9)	cfg.downmode   =gtt_bit(cfg.downmode,11);
		if (n==10)	cfg.downmode   =gtt_bit(cfg.downmode,5);
		if (n==11)	cfg.downmode   =gtt_bit(cfg.downmode,15);
		if (n==12)	cfg.downmode=gtt_bit(cfg.downmode,17);
		if (n==13)	cfg.downmode=gtt_bit(cfg.downmode,20);
		if (n==14)	cfg.downmode=gtt_bit(cfg.downmode,21);
		if (n==15)	cfg.up_limit   =get_num(cfg.up_limit,3);
		if (n==16)	cfg.dn_limit   =get_num(cfg.dn_limit,3);
		if (n==17)	cfg.dn_mult_limit=get_num(cfg.dn_mult_limit,3);
		if (n==18)	cfg.level_size =get_num(cfg.level_size,8);
		if (n==18)	down_after();
		if (n==22)	job_updown2();
	}
}


job_updown2()
{
	int n,i;
	char buff[100];
	while (1) {
		cls();
		print_(0,"´Ù¿î °ü·Ã ¸Þ¼¼Áö/±ÝÁö ¼³Á¤");
		printf("  1. ´Ù¿îÁ¾·á/Áß´Ü½Ã ÀÛ¾÷/È­¸é¸Þ¼¼Áö Ã³¸®          \n");
		printf("  2. ÀÚ·á ¾ÐÃàÈ­ÀÏ È®ÀåÀÚÁöÁ¤ (system/achive.txt)  \n");
		printf("  3. ÀüÃ¼ ÀÚ·á½Ç ³»¿ë ¼³Á¤    (system/zbtotal.dat) \n");
		printf("  4. µ¿½Ã ´Ù¿î ±ÝÁö Æ÷Æ® ¼³Á¤ (system/_downpt.txt) \n");
		printf("  5. ¾÷´Ù¿î½Ã ÁÖÀÇ»çÇ× / Âü°í / ¾È³» È­¸é ÆíÁý     \n");
		printf("  6. »ç¿ëÀÚ¿µ¿ª ¾÷/´Ù¿î °ü·Ã ¼³Á¤                  \n");
		printf("  7. ¾÷ ´Ù¿î ÆÄ¶ó¸ÞÅ¸                              \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) down_after();
		if (n== 2) xedit("system/achive.txt");
		if (n== 3) xedit("system/zbtotal.dat");
		if (n== 4) xedit("system/_downpt.txt");
		if (n== 5) updown_note();
		if (n== 6) updown_user();
		if (n== 7) job_para();
	}
}


down_after()
{
	int n,i1,i2;
	while (1) {
		cls();
		printf("  ### ´Ù¿îÁ¾·á/Áß´ÜÈÄ Ã³¸®¹æ¹ý/È­¸é¼³Á¤\n");
		printf("  1. ÀÚ·á ´Ù¿îÈÄ Enter È®ÀÎÇÔ         :%s\n",rbit(cfg.downmode,12,"È®ÀÎ", "¹«½Ã"));
		printf("  2. ÀÚ·á ´Ù¿îÈÄ »çÀÌ¹ö¸Ó´Ï °á°úÃâ·Â  :%s\n",rbit(cfg.downmode,13,"¹«½Ã", "Ãâ·Â"));
		printf("  3. ÀÚ·á ´Ù¿îÈÄ ´Ù¿î·®º¯È­ °á°úÃâ·Â  :%s\n",rbit(cfg.downmode,14,"¹«½Ã", "Ãâ·Â"));
		printf("  4. ´Ù¿î Ãë¼Ò/Áß´Ü½Ã Ã³¸®            :%s\n",rbit(cfg.downmode,16,"Áß´ÜÃ³¸®", "¹«½Ã"));
		printf("  5. ´Ù¿îÁß´Ü½Ã ´Ù¿î¿ë·® 1°³´Ù¿î½Ã    :%s\n",rbit(cfg.downmode,9,"ÁÙÀÓ", "Ãë¼Ò"));
		printf("  6. ´Ù¿îÁß´Ü½Ã ´Ù¿î¿ë·® ¸ÖÆ¼´Ù¿î½Ã   :%s\n",rbit(cfg.downmode,10,"ÁÙÀÓ", "Ãë¼Ò"));
		printf("  7. ¾÷´Ù¿î½Ã ÁÖÀÇ»çÇ× / Âü°í / ¾È³» È­¸é ÆíÁý     \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.downmode   =gtt_bit(cfg.downmode,12);
		if (n== 2)	cfg.downmode   =gtt_bit(cfg.downmode,13);
		if (n== 3)	cfg.downmode   =gtt_bit(cfg.downmode,14);
		if (n== 4)	cfg.downmode   =gtt_bit(cfg.downmode,16);
		if (n== 5)	cfg.downmode   =gtt_bit(cfg.downmode,9);
		if (n== 6)	cfg.downmode   =gtt_bit(cfg.downmode,10);
		if (n== 7)	updown_note();
	}
}


updown_note()
{
	int n;
	while (1) {
		cls();
		printf("  ### ¾÷,´Ù¿î °ü·Ã È­¸é ÆíÁý\n");
		printf("  1. µ¿½Ã ´Ù¿î ±ÝÁö Æ÷Æ® »çÀ¯     (logo/_downpt.log)  \n");
		printf("  2. ¾÷½Ã   ÁÖÀÇ»çÇ×(ÀüÃ¼ ¾÷)     (logo/_up.scr)      \n");
		printf("  3. ¾÷½Ã   Áú¹®»çÇ×(ÀüÃ¼ ¾÷)     (logo/_up.qsh)      \n");
		printf("  4. ´Ù¿î½Ã ÁÖÀÇ»çÇ×(ÀüÃ¼´Ù¿î)    (logo/_dn.scr)      \n");
		printf("  5. ¾÷ÈÄ   ÁÖÀÇ»çÇ×(ÀüÃ¼ ¾÷)     (logo/_up2.scr)     \n");
		printf("  6. ´Ù¿îÈÄ ÁÖÀÇ»çÇ×(ÀüÃ¼ ¾÷)     (logo/_dn2.scr)     \n");
		printf(" 11. µ¿½Ã ¾÷   µî·Ï ¼ö Á¦ÇÑ ¸Þ¼¼Áö(frm/editor/up_limit.log)\n");
		printf(" 12. µ¿½Ã ´Ù¿î Àü¼Û ¼ö Á¦ÇÑ ¸Þ¼¼Áö(frm/editor/dn_limit.log)\n");
		printf(" 13. ¸ÖÆ¼ ´Ù¿î ¼ö      Á¦ÇÑ ¸Þ¼¼Áö(frm/down/dn_mult.log)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	xedit("logo/_downpt.log");
		if (n== 2)	xedit("logo/_up.scr");
		if (n== 3)	xedit("logo/_up.qsh");
		if (n== 4)	xedit("logo/_dn.scr");
		if (n== 5)	xedit("logo/_up2.scr");
		if (n== 6)	xedit("logo/_dn2.scr");
		if (n==11)	xedit("frm/editor/up_limit.log");
		if (n==12)	xedit("frm/editor/dn_limit.log");
		if (n==13)	xedit("frm/down/dn_mult.log");
	}
}


updown_user()
{
	int n;
	while (1) {
		cls();
		printf("  ### »ç¿ëÀÚ ¿µ¿ª °ü·Ã ¼³Á¤\n");
		printf("  1. USERROOM¿¡¼­ È­ÀÏUP   ¿ë·®º¸»ó     :%s\n",rbit(cfg.downmode,3,"¾ÈÇÔ","ÇÔ"));
		printf("  2. USERROOM¿¡¼­ È­ÀÏUPÈÄ È­ÀÏ»èÁ¦     :%s\n",rbit(cfg.downmode,4,"¾ÈÇÔ","ÇÔ"));
		xy(1,20);
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.downmode=gtt_bit(cfg.downmode,3);
		if (n== 2)	cfg.downmode=gtt_bit(cfg.downmode,4);
	}
}



job_para()
{
	int n,i;
	char buff[100];
	while (1) {
		del_esc(cfg.jpgsz);del_esc(cfg.jpgstr);
		cls();
		print_(0,"¾÷,´Ù¿î ÆÄ¶ó¸ÞÅ¸ ¼öÁ¤");
		printf(" 1. ´Ù¿î(Àü¼ÛSZ) ¸í·É :%s\n",cfg.sz);
		printf(" 2. ¾÷  (µî·ÏRZ) ¸í·É :%s\n",cfg.rz);
		printf(" 3. KERMIT´Ù¿î   ¸í·É :%s\n",cfg.kermitsz);
		printf(" 4. KETMIT¾÷     ¸í·É :%s\n",cfg.kermitrz);
		printf(" 5. È­»óÀü¼Û (sz)¸í·É :%s\n",cfg.jpgsz);
		printf(" 6. È­»óÀü¼Û¾È½Ã¹®ÀÚ¿­:%s\n",cfg.jpgstr);       //        printf("7\n8[5;5H");
		printf(" 7. ÀÚ·á ¾÷/´Ù¿î½Ã Àü¼Û°á°ú :%s\n",rbit(cfg.downmode,22,"Ã³¸®","¹«½Ã"));
		printf("99. ±âº» ¼³Á¤À¸·Î º¯°æÇÏ±â (DEFAULT)\n");
		printf("    (ESC¹®ÀÚÇ¥Çö \\027 Çü½ÄÀ¸·Î  ¿¹\\024\\n)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	{xy(30,n+1);get_str(cfg.sz,80);}
		if (n== 2)	{xy(30,n+1);get_str(cfg.rz,80);}
		if (n== 3)	{xy(30,n+1);get_str(cfg.kermitsz,80);}
		if (n== 4)	{xy(30,n+1);get_str(cfg.kermitrz,80);}
		if (n== 5)	{xy(30,n+1);get_str(cfg.jpgsz,80);}
		if (n== 6)	{xy(30,n+1);get_str(cfg.jpgstr,80);}
		if (n== 7)	cfg.downmode=gtt_bit(cfg.downmode,22);
		if (n==99)	{
			strcpy(cfg.sz,"sz");
			strcpy(cfg.rz,"rz");
			strcpy(cfg.kermitsz,"kermit -s");
			strcpy(cfg.kermitrz,"kermit -r");
			strcpy(cfg.jpgsz,"sz");
			strcpy(cfg.jpgstr,"\\024\\n");
		}
	}
}

job_zbtotal()
{
	int n,i;
	char buff[100];
	while (1) {
		cls();
		print_(0,"ÀüÃ¼ÀÚ·á½Ç ¼³Àú");
		printf(" 1. ÀüÃ¼ÀÚ·á½Ç ÀÛ¾÷»óÈ² Ãâ·Â         :%s\n",rbit(cfg.zmode,1,"¾ÈÇÔ","Ãâ·Â"));
		printf(" 2. ÀüÃ¼°Ô½ÃÆÇ¸¸ ÀÛ¾÷                :%s\n",rbit(cfg.zmode,2,"º¸Åë","ÇÑÁ¤"));
		printf(" 3. ÀüÃ¼°Ô½ÃÆÇ¸¸ ÀÛ¾÷                :%s\n",rbit(cfg.zmode,3,"º¸Åë","ÇÑÁ¤"));
		printf(" 4. º¯µ¿»çÇ× °Ë»ç¿©ºÎ                :%s\n",rbit(cfg.zmode,4,"ÇÔ","¾ÈÇÔ"));
		printf(" 5. ÀüÃ¼ÀÚ·á½ÇÁøÀÔ½Ã 1½Ã°£ º¯µ¿È®ÀÎ  :%s\n",rbit(cfg.zmode,21,"¾ÈÇÔ","È®ÀÎ"));
		printf(" 6. ÀüÃ¼ÀÚ·á½ÇÁøÀÔ½Ã 6½Ã°£ º¯µ¿È®ÀÎ  :%s\n",rbit(cfg.zmode,22,"¾ÈÇÔ","È®ÀÎ"));
		printf(" 7. ½ÃÀÛ½Ã           1½Ã°£ º¯µ¿È®ÀÎ  :%s\n",rbit(cfg.zmode,23,"¾ÈÇÔ","È®ÀÎ"));
		printf(" 8. ½ÃÀÛ½Ã           6½Ã°£ º¯µ¿È®ÀÎ  :%s\n",rbit(cfg.zmode,24,"¾ÈÇÔ","È®ÀÎ"));
		printf(" 9. ¾²±â/´Ù¿î½Ã      30ºÐ  º¯µ¿È®ÀÎ  :%s\n",rbit(cfg.zmode,25,"¾ÈÇÔ","È®ÀÎ"));
		printf("10. ÀüÃ¼ ÀÚ·á½Ç ³»¿ë ¼³Á¤    (system/zbtotal.dat) \n");
		printf("³»¿ëÁÙÀÓ¸ðµå 11.(%s) ",rbit2(cfg.zmode,11,"o"," "));
		printf("12.(%s) ",rbit2(cfg.zmode,12,"o"," "));
		printf("13.(%s) ",rbit2(cfg.zmode,13,"o"," "));
		printf("14.(%s) ",rbit2(cfg.zmode,14,"o"," "));
		printf("15.(%s) ",rbit2(cfg.zmode,15,"o"," "));
		printf("16.(%s) ",rbit2(cfg.zmode,16,"o"," "));
		printf("17.(%s)\n",rbit2(cfg.zmode,17,"o"," "));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.zmode=gtt_bit(cfg.zmode,1);
		if (n== 2)	cfg.zmode=gtt_bit(cfg.zmode,2);
		if (n== 3)	cfg.zmode=gtt_bit(cfg.zmode,3);
		if (n== 4)	cfg.zmode=gtt_bit(cfg.zmode,4);
		if (n== 5)	cfg.zmode=gtt_bit(cfg.zmode,21);
		if (n== 6)	cfg.zmode=gtt_bit(cfg.zmode,22);
		if (n== 7)	cfg.zmode=gtt_bit(cfg.zmode,23);
		if (n== 8)	cfg.zmode=gtt_bit(cfg.zmode,24);
		if (n== 9)	cfg.zmode=gtt_bit(cfg.zmode,25);
		if (n==10)	xedit("system/zbtotal.dat");
		if (n==11)	cfg.zmode=gtt_bit(cfg.zmode,11);
		if (n==12)	cfg.zmode=gtt_bit(cfg.zmode,12);
		if (n==13)	cfg.zmode=gtt_bit(cfg.zmode,13);
		if (n==14)	cfg.zmode=gtt_bit(cfg.zmode,14);
		if (n==15)	cfg.zmode=gtt_bit(cfg.zmode,15);
		if (n==16)	cfg.zmode=gtt_bit(cfg.zmode,16);
		if (n==17)	cfg.zmode=gtt_bit(cfg.zmode,17);
	}
}

//ÅõÇ¥Àå
job_vote()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"ÅõÇ¥ °ü·Ã ÀÛ¾÷");
		printf("  1. ÅõÇ¥ Âü¿© °¡´É µî±Þ              :%d\n",cfg.vote_type);
		printf("  2. ÅõÇ¥ °³¼³ °¡´É µî±Þ              :%d\n",cfg.vote_opentype);
		printf(" 10. µ¿È£È¸ ÅõÇ¥ÀÚ Çã¿ë¿©ºÎ           :%s\n",rbit(cfg.vote,1,"Çã¿ë","±ÝÁö"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.vote_type	=get_num(cfg.vote_type	  ,2);
		if (n== 2) cfg.vote_opentype=get_num(cfg.vote_opentype,2);
		if (n==10) cfg.vote 		=gtt_bit(cfg.vote		 ,10);
	}
}



job_setbbs()
{
	int n,i;
	char buff[300];
	while (1) {
		cls();
		print_(0,"°Ô½ÃÆÇ,ÆíÁý±â °ü·Ã¼³Á¤");
		printf(" 1.  BDN(°Ô½ÃÆÇ´Ù¿î) ±â´É °¡´É ¾ÆÀÌµð   (system/bdn_id.dat)\n");
		printf(" 2.  °Ô½Ã¹°->bbs_all ÀÚ·á½Ç->pds_all    :%s\n",rbit(cfg.mode,24,"ÀÚµ¿ÀúÀå","¹«½Ã"));
		printf(" 3.  °Ô½ÃÆÇ µÎÀÚ¸®¹øÈ£¼±ÅÃ½Ã -> ÀÚµ¿ÁöÁ¤:%s\n",rbit(cfg.mode,10,"ÀÚµ¿ÁöÁ¤","¹«½Ã"));
		printf(" 4.  °Ô½ÃÆÇ ÆíÁý½Ã ´Ù¿î/Á¶È¸/³¯Â¥ ¼öÁ¤  :%s\n",rbit(cfg.bbsmode,3,"¼öÁ¤ºÒ°¡","¼öÁ¤°¡´É"));
		printf(" 5.  °Ô½ÃÆÇ ÆíÁý½Ã À§Ä¡,³»¿ëÅ©±â ¼öÁ¤   :%s\n",rbit(cfg.bbsmode,4,"¼öÁ¤ºÒ°¡","¼öÁ¤°¡´É"));
		printf(" 6.  ¾²±â½Ã °³ÀÎ»çÀÎ Ã·ºÎ               :%s\n",rbit(cfg.mode,9,"Çã¿ë","±ÝÁö"));
		printf(" 7.  ÀÚ·á½Ç Å©±â 0ÀÎ ÀÚ·á ¾Èº¸ÀÌ±â      :%s\n",rbit(cfg.bbsmode,5,"ÀÚ·á½Çº°¼³Á¤","ÀüÃ¼º¸ÀÓ"));
		printf(" 8.  ÆíÁý±â2 -> ³»ÀåÆíÁý±â ÁöÁ¤         :%s\n",rbit(cfg.bbsmode,6,"¿ÜºÎÆíÁý±â","³»ÀåÆíÁý±â"));
		printf(" 9.  W¾²±â RÀÐ±â D´Ù¿î C´ëÈ­ Sµ¿È£È¸ ±ÝÁö/Á¦ÇÑ(system/limit.id)\n");
		printf("10.  ÀÚµ¿ÃßÃµ/ÀÚµ¿Âù¼º °ü·Ã ¼³Á¤        \n");
		n=numselect();xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) xedit("system/bdn_id.dat");
		if (n== 2) cfg.mode=gtt_bit(cfg.mode,24);
		if (n== 3) cfg.mode=gtt_bit(cfg.mode,10);
		if (n== 4) cfg.bbsmode=gtt_bit(cfg.bbsmode,3);
		if (n== 5) cfg.bbsmode=gtt_bit(cfg.bbsmode,4);
		if (n== 6) cfg.mode=gtt_bit(cfg.mode,9);
		if (n== 7) cfg.bbsmode=gtt_bit(cfg.bbsmode,5);
		if (n== 8) cfg.bbsmode=gtt_bit(cfg.bbsmode,6);
		if (n== 9) set_limit_id();
		if (n==10) job_good();
	}
}

job_good()
{
	char buff[300];
	while (1) {
		cls();
		print_(1,"°Ô½Ã¹°/ÀÚ·á½Ç ÃßÃµ ¼³Á¤");
		printf("  1. ÀÚµ¿ÃßÃµ/Âù¼º±â´É µ¿ÀÛ¸ðµå :%s\n",rbit(cfg.bbsmode,0,"µ¿ÀÛ","ÁßÁö"));
		printf("  2. ÀÚµ¿ÃßÃµ/Âù¼º±â´É »ö»ó¸ðµå :%s\n",rbit(cfg.bbsmode,1,"µ¿ÀÛ","ÁßÁö"));
		printf("  3. GD(ÃßÃµ¸í·É)º° ÃßÃµ¼ø¼­    (%-10s) \n",cfg.gdstr);
		printf(" 11. ÃßÃµÇ¥½Ãº° »ö»ó ÁöÁ¤  %s\n",GOOL_FILE);
		printf(" 12. °Ô½ÃÆÇÀÐ±â->ÀÚµ¿ ÃßÃµ È½¼ö,Ç¥½Ã¹®ÀÚ,»ö±ò ÁöÁ¤ \n");
		printf(" 13. ÀÚ·á½Ç´Ù¿î->ÀÚµ¿ ÃßÃµ È½¼ö,Ç¥½Ã¹®ÀÚ,»ö±ò ÁöÁ¤ \n");
		printf(" 14. °Ô½ÃÆÇÂù¼º->ÀÚµ¿ ÃßÃµ È½¼ö,Ç¥½Ã¹®ÀÚ,»ö±ò ÁöÁ¤ \n");
		printf(" 15. ÀÚ·á½ÇÂù¼º->ÀÚµ¿ ÃßÃµ È½¼ö,Ç¥½Ã¹®ÀÚ,»ö±ò ÁöÁ¤ \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1) cfg.bbsmode=gtt_bit(cfg.bbsmode,0);
		if (n== 2) cfg.bbsmode=gtt_bit(cfg.bbsmode,1);
		if (n== 3) get_str(cfg.gdstr,10);
		if (n==11) cfg_gool();
		if (n>=12&&n<=15) cfg_good(n-12);
	}
}




cfg_good(int mode)
{
	FILE *fp;
	char buff[90];
	int n,n2,i,i2;

	if ((fp=fopen(GOOD_FILE,"r+"))!=NULL) {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				fread(&agood[i][i2],sizeof_agood,1,fp);
			}
		}
		fclose(fp);
	}
	while(1) {
		cls();
		i=mode;
		if (i==0) print_(0,"°Ô½ÃÆÇÀÐ±â ÀÚµ¿ ÃßÃµ ¼³Á¤ %s",GOOD_FILE);
		if (i==1) print_(0,"ÀÚ·á½Ç´Ù¿î ÀÚµ¿ ÃßÃµ ¼³Á¤ %s",GOOD_FILE);
		if (i==2) print_(0,"°Ô½ÃÆÇÂù¼º ÀÚµ¿ ÃßÃµ ¼³Á¤ %s",GOOD_FILE);
		if (i==3) print_(0,"ÀÚ·á½ÇÂù¼º ÀÚµ¿ ÃßÃµ ¼³Á¤ %s",GOOD_FILE);
		for(i2=0;i2<10;i2++) {
			printf(" %2d:",i2+1);
			bzero(buff,10);buff[0]=agood[i][i2].mark;
			printf(" Ç¥½Ã¹®ÀÚ %2s ",buff);
			printf(" Ç¥½Ã¼³¸í %-8s ",acolor[i2].name);
			printf(" ÀÚµ¿ÃßÃµ¼ö %4d ",agood[i][i2].count);
			printf(" ±ÛÀÚ»ö:%2d  ¹è°æ»ö:%2d [s%s[ ÇöÀç»ö ][u\n",
			acolor[i2].color_char,acolor[i2].color_back,get_gool(agood[i][i2].mark));
		}
		printf(" ¼±ÅÃ(Q,W)>>");
		lineinput(buff,10);check_han(buff);printf("\n");
		i2=atoi(buff)-1;
		if (is_quit(buff)) break;
		if (is_same(buff,"q")) break;
		if (is_same(buff,"w")) break;
		if (is_same(buff,"x")) break;
		if (is_same(buff,"xx")) break;
		if (buff[0]==0) continue;
		if (i2<0||i2>10) continue;
		printf(" %2d:",i2+1);
		bzero(buff,10);buff[0]=agood[i][i2].mark;
		printf(" Ç¥½Ã¹®ÀÚ   :%2s º¯°æÇ¥½Ã¹®ÀÚ ",buff);
		lineinput(buff,2);printf("\n");
		if (buff[0]!=0) agood[i][i2].mark=buff[0];
		printf(" ÀÚµ¿ÃßÃµ¼ö :%4d º¯°æÃßÃµÈ½¼ö ",agood[i][i2].count);
		agood[i][i2].count=get_num(agood[i][i2].count,999999);printf("\n");
		bzero(buff,10);buff[0]=agood[i][i2].mark;
	}

	if ((fp=fopen(GOOD_FILE,"w+"))!=NULL) {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				fwrite(&agood[i][i2],sizeof_agood,1,fp);
			}
		}
		fclose(fp);
	}

}


cfg_gool()
{
	char buff[90];
	int n,n2,i,i2;
	FILE *fp;

	//ÇØ´çÈ­ÀÏÁ¤º¸ÀÐ±â
	if ((fp=fopen(GOOL_FILE,"r+"))!=NULL) {
		for(i=0;i<10;i++) {
			fread(&acolor[i],sizeof_acolor,1,fp);
		}
		fclose(fp);
	}

	while(1) {
		cls();
		print_(1,"ÃßÃµÇ¥½Ãº° »ö»ó ÁöÁ¤ %s",GOOL_FILE);
		for(i2=0;i2<10;i2++) {
			printf(" %2d:",i2+1);
			bzero(buff,10);buff[0]=acolor[i2].mark;
			printf(" Ç¥½Ã¹®ÀÚ %2s ",buff);
			printf(" Ç¥½Ã¼³¸í %-8s ",acolor[i2].name);
			printf(" ±ÛÀÚ»ö:%2d  ¹è°æ»ö:%2d [s%s[ ÇöÀç»ö ][u\n",acolor[i2].color_char,acolor[i2].color_back,get_gool(acolor[i2].mark));
		}
		printf(" ¼±ÅÃ(Q,W)>>");
		lineinput(buff,10);check_han(buff);printf("\n");
		i2=atoi(buff)-1;
		if (is_quit(buff)) break;
		if (is_same(buff,"q")) break;
		if (is_same(buff,"w")) break;
		if (is_same(buff,"x")) break;
		if (is_same(buff,"xx")) break;
		if (buff[0]==0) continue;
		if (i2<0||i2>10) continue;
		printf(" %2d:",i2+1);
		bzero(buff,10);buff[0]=acolor[i2].mark;
		printf(" Ç¥½Ã¹®ÀÚ   :%2s º¯°æÇ¥½Ã¹®ÀÚ ",buff);
		lineinput(buff,2);printf("\n");
		if (buff[0]!=0) acolor[i2].mark=buff[0];
		printf(" Ç¥½Ã¼³¸í   :%-8s º¯°æÇ¥½Ã¼³¸í ",acolor[i2].name);
		lineinput(buff,8);printf("\n");
		if (buff[0]!=0) strcpy(acolor[i2].name,buff);
		printf(" ÃßÃµ±ÛÀÚ»ö :%2d º¯°æ¹è°æ»ö   ",acolor[i2].color_char);
		acolor[i2].color_char=get_num(acolor[i2].color_char,16);printf("\n");
		printf(" ÃßÃµ¹è°æ»ö :%2d º¯°æ±ÛÀÚ»ö   ",acolor[i2].color_back);
		acolor[i2].color_back=get_num(acolor[i2].color_back,16);printf("\n");
	}

	//ÇØ´çÈ­ÀÏÁ¤º¸ÀúÀå
	if ((fp=fopen(GOOL_FILE,"w+"))!=NULL) {
		for(i=0;i<10;i++) {
			fwrite(&acolor[i],sizeof_acolor,1,fp);
		}
		fclose(fp);
	}
}


job_menu()
{
	while (1) {
		cls();
		print_(0,"¸Þ´ºÆíÁý");
		printf("   1. ¸ÞÀÎ¸Þ´ºÆíÁý                     :%s\n",MENU_FILE);
		printf("   2. °¡±âÄÚµå ÀÔ·Â½Ã GO ¾ø¾îµµ °Ë»ö   :%s\n",rbit(cfg.gomode,1,"°Ë»ö", "¹«½Ã"));
		printf("   3. °¡±âÄÚµå ÀÔ·Â½Ã °¡±â Á¦¸ñ °Ë»ö   :%s\n",rbit(cfg.gomode,2,"°Ë»ö", "¹«½Ã"));
		printf("   4. ·ÎÄ® ¸Þ´º ¼³Á¤\n");
		num=numselect();
		xy(60,n+1);
		if (num<= 0) break;
		if (num== 1) sys_tem("bin/ed_menu %s main",now_id,club_go.code);
		if (num== 2) cfg.gomode=gtt_bit(cfg.gomode,1);
		if (num== 3) cfg.gomode=gtt_bit(cfg.gomode,2);
		if (num== 4) sys_tem("bin/pf '%s' set_localmenu",pf.id);
	}
}


sort_pf()
{
	int num;
	while (1) {
		cls();
		printfL2();
		printf("###  È¸¿ø Á¤¸®ÀÛ¾÷  ###\n");
		printf("  1. È¸¿ø ¹øÈ£ ÀçÁ¤¸®  (Áßº¹È¸¿ø¹øÈ£/Æ¯¼ö¹®ÀÚID µî ÀçÁ¤¸®)  \n");
		printf("  2. »èÁ¦µÈ È¸¿ø ÀÚ·á È®ÀÎ (µî±Þ0,»èÁ¦Ã³¸® È¸¿ø È®ÀÎ)       \n");
		printf("  3. µî±Þ 0 ÀÎ È¸¿ø -> »èÁ¦ Ã³¸® (¾ÆÀÌµð/¼º¸í -> °ø¹éÃ³¸®   \n");
		printf("  4. µî±Þ 0 ÀÎ È¸¿ø -> »èÁ¦ Ã³¸® (ÁÖ¹Îµî·Ï¹øÈ£ »ç¿ë±ÝÁöÃ³¸®)\n");
		printf("  5. ÀÏÁ¤±â°£ Á¢¼Ó ¾ø´ÂÈ¸¿ø »èÁ¦ Ã³¸® -> µî±Þ 0 Ã³¸®        \n");
		printf("  6. °æ°í 9 ÇØ´ç       È¸¿ø »èÁ¦ Ã³¸® -> µî±Þ 0 Ã³¸®        \n");
		printf(" *7. ¿ÏÀü»èÁ¦µÈ È¸¿ø (ID,¼º¸í ºóÄ­)   -> ¿ÏÀü »èÁ¦Ã³¸®      \n");
		printf("  9. È¸¿ø ÀÚ·á ÀüÃ¼ ÀÏ·Ã ¹øÈ£(³»ºÎÃ³¸®¿ë) ÁöÁ¤              \n");
		printfL2();
		num=numselect();
		if (num<= 0) break;
		if (num== 1) renum_pf();
		if (num== 2) disp_del_pf();
		if (num== 3) pack_del_pf(1);
		if (num== 4) pack_del_pf(2);
		if (num== 5) pack_del_pf(3);
		if (num== 6) pack_del_pf(4);
		if (num== 7) pack_del_pf(5);
		if (num== 9) renum_proccode();	 //È¸¿ø ÀçÁ¤¸®
	}
}



job_news()
{
	int i,n;
	char buff1[200];

	while (1) {
		cls();
		printf("  1.  ´º½º±â´É µ¿ÀÛ¿©ºÎ                         :%s\n",rbit(cfg.newsmode,0,"°¡´É","ºÒ°¡"));
		printf("  2.  µ¥ÀÌÅ¸È­ÀÏ °¡±âÄÚµå                       :%s\n",cfg.newsfile);
		printf("  3.  ´º½ºº¸±â °¡´É µî±Þ        (0:¼Õ´Ô 19:½Ã»ð):%d\n",cfg.newstype);
		printf("  4.  ´º½ºº¸±â ÃÖ½ÅÀÚ·á½Ã°£     (0:¹«½Ã #:½Ã°£) :%d\n",cfg.newsoldtime);
		printf("  9.  È­¸é°»½Å½Ã ¾Ë¶÷½Ã±×³¯µ¿ÀÛ(´º½º..)         :%s\n",rbit(cfg.logomode,10, "µ¿ÀÛ", "ÁßÁö"));
		printf(" 10.  ¸ÞÀÎ¸Þ´º     ´º½º °¡´É                    :%s\n",rbit(cfg.newsmode,1,"°¡´É","°ÅºÎ"));
		printf(" 11.  º¸Á¶¸Þ´º     ´º½º °¡´É                    :%s\n",rbit(cfg.newsmode,2,"°¡´É","°ÅºÎ"));
		printf(" 12.  °Ô½ÃÆÇ       ´º½º °¡´É                    :%s\n",rbit(cfg.newsmode,3,"°¡´É","°ÅºÎ"));
		printf(" 13.  µ¿È£È¸¸ÞÀÎ   ´º½º °¡´É                    :%s\n",rbit(cfg.newsmode,4,"°¡´É","°ÅºÎ"));
		printf(" 14.  µ¿È£È¸º¸Á¶   ´º½º °¡´É                    :%s\n",rbit(cfg.newsmode,5,"°¡´É","°ÅºÎ"));
		printf(" 15.  µ¿È£È¸°Ô½ÃÆÇ ´º½º °¡´É                    :%s\n",rbit(cfg.newsmode,6,"°¡´É","°ÅºÎ"));
		printf(" 16.  ÃßÃµµÈ ÀÚ·á¸¸ ¼±ÅÃ                        :%s\n",rbit(cfg.newsmode,7,"ÀüºÎ","ÃßÃµ"));
		printf(" 17.  *.TA? ¸ÞÀÎTOP¿¡¼­¸¸ Ãâ·Â                  :%s\n",rbit(cfg.newsmode,8,"TOP¸¸","ÀüºÎ"));
		printfL2();
		n=numselect();
		if (n<= 0)	break;
		if (n== 1)	cfg.newsmode =gtt_bit(cfg.newsmode,0);
		if (n== 2)	{xy(2,20);get_str(cfg.newsfile,8);}
		if (n== 3)	cfg.newstype  =get_num(cfg.newstype,2 );
		if (n== 4)	cfg.newsoldtime=get_num(cfg.newsoldtime,5);
		if (n== 9)	cfg.logomode  =gtt_bit(cfg.logomode,10);
		if (n==10)	cfg.newsmode =gtt_bit(cfg.newsmode,1);
		if (n==11)	cfg.newsmode =gtt_bit(cfg.newsmode,2);
		if (n==12)	cfg.newsmode =gtt_bit(cfg.newsmode,3);
		if (n==13)	cfg.newsmode =gtt_bit(cfg.newsmode,4);
		if (n==14)	cfg.newsmode =gtt_bit(cfg.newsmode,5);
		if (n==15)	cfg.newsmode =gtt_bit(cfg.newsmode,6);
		if (n==16)	cfg.newsmode =gtt_bit(cfg.newsmode,7);
		if (n==17)	cfg.newsmode =gtt_bit(cfg.newsmode,8);
	}
}


del_mail()
{
	FILE *fp;
	int dday;

	dday=30;
	if (dday==0) return;

	read_cfg();
	set_tmpfile();

	if ((fp=fopen("id/id.dat","r"))!=NULL) {
		while (fread(&pf,sizeof_pf,1,fp)) {
			if (pf.type>=18) continue;
			sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf.id),pf.id);
			sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf.id),pf.id);
			if (is_file(NOW_FILE)) bbs_old_del(dday);
			sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf.id),pf.id);
			sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf.id),pf.id);
			if (is_file(NOW_FILE)) bbs_old_del(dday);
		}
		fclose(fp);
	}
	printf("\n");
}


//»èÁ¦µÈ °Ô½Ã¹° Á¤¸®
bbs_old_del(int dday)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char buff[180];
	char c;
	char str[10];
	int found=No;
	int ret,i,new_posi;
	time_t today;
	time_t today1;
	time_t today2;
	int bbs_num;

	time(&today);	  //¿À´ÃÀÇ ½Ã°£
	today1 = today - (dday*24*3600);   //»èÁ¦´ë»ó ³¯Â¥

	fp1=fopen(NOW_FILE,"r");      //Á¦¸ñÈ­ÀÏ
	fp2=fopen(NOW_FILED,"r");      //Á¦¸ñÈ­ÀÏ
	fp3=fopen(TMP_FILE,"w");      //ÀÓ½ÃÈ­ÀÏ
	fp4=fopen(TMP_FILE2,"w");      //ÀÓ½ÃÈ­ÀÏ

	new_posi=0;
	bbs_num=0;
	while (fread(&bbs,sizeof_bbs,1,fp1)) {
		if (bbs.date<today1) {
			sprintf(buff,"userdata/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
			unlink(buff);
			continue;
		}
		fseek(fp2,bbs.posi,SEEK_SET);
		fseek(fp4,new_posi,SEEK_SET);
		for (i=0;i<bbs.size;i++) fputc(fgetc(fp2),fp4);
		bbs.posi=new_posi;
		new_posi+=bbs.size;
		bbs.num=++bbs_num;
		fwrite(&bbs,sizeof_bbs,1,fp3);
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (fp3!=NULL) fclose(fp3); if (fp4!=NULL) fclose(fp4);

	file_move(TMP_FILE ,NOW_FILE );
	file_move(TMP_FILE2,NOW_FILED);
	if (file_size(NOW_FILE)==0) {
		unlink(NOW_FILE );
		unlink(NOW_FILED);
	}
}

int apf_level[20];
int atoday_tim1[20];
int atoday_tim2[20];
int atoday_down[20];
GG gg[10];

edit_atype(int mode) {
	char buff[80];
	while (1) {
		disp_type();
		edit_type(mode);
		disp_type();
		while (1) {
			xy(5,25);
			printf("   ÀúÀå:W  Ãë¼Ò:Q  ÀçÀÔ·Â:R ±âº»µî±Þ¸í:D  ¼±ÅÃ>>");
			lineinput(buff,2);
			printf("\n");
			if (is_char("wWqQrRDd",buff[0])) break;
		}
		if (is_same(buff,"w")) {
			save_atype();
			save_pftype();
			break;
		}
		if (is_same(buff,"d")) {
			def_atype();
		}
		xy(5,26);
		if (is_same(buff,"Q")&&yesno("ÀúÀåÀ» Ãë¼ÒÇÒ±î¿ä? (Y/n) >>",Yes)) {
			break;
		}
	}
}



disp_type()
{
	int i;
	cls();
	printf("           È¸¿ø µî±Þº° È¯°æ¼³Á¤ \n");
	printfL2();
	printf(" µî±Þ    µî±ÞÀÌ¸§    ÁøÀÔ·¹º§ ÀÏÀÏÁ¦ÇÑ½Ã°£ 1È¸Á¢¼ÓÁ¦ÇÑ\n");
	printfL2();
	for (i=0;i<20;i++) {
		disp_type2(i);
	}
}


disp_type2(int i)
{
		xy(1,i+5);
		if (i>=14)
			printf("  %2d    %-8s\n",
			i,atype[i],apf_level[i]);
		else
			printf("  %2d    %-8s  %8d  %8d  %8d\n",
			i,atype[i],apf_level[i],atoday_tim1[i],atoday_tim2[i]);
}


edit_type(int mode)
{
	int i;
	char buff[50];
	if (mode==1) {
		for (i=0;i<20;i++) {
			xy(9,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) strcpy(atype[i],buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
			del_space(atype[i]);
		}
	}
	else if (mode==2) {
		for (i=0;i<14;i++) {
			xy(20,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) apf_level[i]=atoi(buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
		}
	}
	else if (mode==3) {
		for (i=0;i<14;i++) {
			xy(30,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) atoday_tim1[i]=atoi(buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
		}
	}
	else if (mode==4) {
		for (i=0;i<14;i++) {
			xy(40,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) atoday_tim2[i]=atoi(buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
		}
	}
}



save_atype()
{
	char type_file[140];
	FILE *fp;
	int i;
	sprintf(type_file,"system/typename.dat");
	fp=fopen(type_file,"w");
	for (i=0;i<20;i++) fwrite(&atype[i],sizeof(atype[i]),1,fp);
	if (fp!=NULL) fclose(fp);
}


read_pftype()  //È¸¿øÀÇ µî±Þ,ÀÏÀÏ»ç¿ë°¡´É½Ã°£À» È®ÀÎÇÑ´Ù.
{
	int old_type;
	int i,n;
	FILE *fp;
	for (i=0;i<20;i++) apf_level[i]=999999;
	for (i=0;i<20;i++) atoday_tim1[i]=99999;
	for (i=0;i<20;i++) atoday_tim2[i]=99999;
	for (i=0;i<20;i++) atoday_down[i]=99999;
	fp=fopen("system/pftype.dat","r");   //µî±ÞÈ­ÀÏ
	if (fp==NULL) return;
	for (i=0;i<14;i++) fread(&apf_level  [i],sizeof(apf_level  [i]),1,fp);
	for (i=0;i<14;i++) fread(&atoday_tim1[i],sizeof(atoday_tim1[i]),1,fp);
	for (i=0;i<14;i++) fread(&atoday_tim2[i],sizeof(atoday_tim2[i]),1,fp);
	for (i=0;i<14;i++) fread(&atoday_down[i],sizeof(atoday_down[i]),1,fp);
	if (fp!=NULL) fclose(fp);
}


save_pftype()  //È¸¿øÀÇ µî±Þ,ÀÏÀÏ»ç¿ë°¡´É½Ã°£À» È®ÀÎÇÑ´Ù.
{
	int old_type;
	int i,n;
	FILE *fp;
	fp=fopen("system/pftype.dat","w");   //µî±ÞÈ­ÀÏ
	if (fp==NULL) return;
	for (i=0;i<14;i++) fwrite(&apf_level  [i],sizeof(apf_level	[i]),1,fp);
	for (i=0;i<14;i++) fwrite(&atoday_tim1[i],sizeof(atoday_tim1[i]),1,fp);
	for (i=0;i<14;i++) fwrite(&atoday_tim2[i],sizeof(atoday_tim2[i]),1,fp);
	for (i=0;i<14;i++) fwrite(&atoday_down[i],sizeof(atoday_down[i]),1,fp);
	fclose(fp);
}


def_atype()
{
	strcpy(atype[0],"¼Õ´Ô");
	strcpy(atype[1],"È¸¿ø1");
	strcpy(atype[2],"È¸¿ø2");
	strcpy(atype[3],"È¸¿ø3");
	strcpy(atype[4],"È¸¿ø4");
	strcpy(atype[5],"È¸¿ø5");
	strcpy(atype[6],"È¸¿ø6");
	strcpy(atype[7],"È¸¿ø7");
	strcpy(atype[8],"È¸¿ø8");
	strcpy(atype[9],"È¸¿ø9");
	strcpy(atype[10],"È¸¿ø10");
	strcpy(atype[11],"È¸¿ø11");
	strcpy(atype[12],"È¸¿ø12");
	strcpy(atype[13],"È¸¿ø13");
	strcpy(atype[14],"µ¿È£½Ã»ð");
	strcpy(atype[15],"ºÎ¿î¿µÁø");
	strcpy(atype[16],"¿î¿µÁø");
	strcpy(atype[17],"½Ã»ðÁø");
	strcpy(atype[18],"ºÎ½Ã»ð");
	strcpy(atype[19],"½Ã»ð");
}



edit_ajob()
{
	int i;
	char buff[50];
	FILE *fp1;

	read_ajob();	//Á÷¾÷ÀÐ±â
	cls();
	xy(1,1);printf("              ¹øÈ£º° Á÷¾÷¸í ¼öÁ¤\n");
	printfL2();
	for (i=1;i<19;i++) printf("%6d. %s\n",i,ajob[i]);
	printfL2();
	for (i=1;i<19;i++) {
		xy(20,i+2);
		printf(" ¼öÁ¤>>");
		lineinput(buff,10);
		if (is_quit(buff)) break;
		if (buff[0]!=0) strcpy(ajob[i],buff);
	}
	if (!is_same(buff,"q")) {
		fp1=fopen("system/ajob.dat","w");
		for (i=1;i<19;i++) fwrite(&ajob[i],sizeof(ajob[i]),1,fp1);
		if (fp1!=NULL) fclose(fp1);
	}
}


edit_addr()
{
	xedit("frm/guest/_post.txt");
}


gg_read()
{
	FILE *fp;
	int i;

	for (i=0;i<10;i++) {
		strcpy(gg[i].code,"");
		strcpy(gg[i].name,"");
	}

	fp=fopen("system/gg.dat","r+");
	if (fp!=NULL)
		for (i=0;i<10;i++) fread(&gg[i],sizeof_gg,1,fp) ;
	if (fp!=NULL) fclose(fp);
}


disp_gg()
{
	int i;
	printf("\n");
	printfL2();
	for (i=0;i<10;i++)
		printf("       [  G%-2d ] %-20s ( %-8s )\n",i+1,gg[i].name,gg[i].code);
}

edit_gg()
{
	char buff[30];
	FILE *fp;
	int i;

	gg_read();
	while (1) {
		printf("\n");
		printfL2();
		printf("     ÃßÃµ µ¿È£È¸/°Ô½ÃÆÇ ÆíÁý \n");
		disp_gg();
		printfL2();
		printf("    ÆíÁýÇÒ ÃßÃµ ¹øÈ£  Á¾·á:0 >>");
		lineinput(buff,4);
		i=atoi(buff);
		if (i==0) {
			buff[0]=' ';
			i=atoi(buff);
		}
		if (i<1||i>10) break;
		printf("\n º¯°æ  %d. °¡±âÀÌ¸§ >>",i);
		lineinput(buff,20);
		if (buff[0]!=0) strcpy(gg[i-1].name,buff);
		printf("\n º¯°æ  %d. °¡±âÄÚµå >>",i);
		lineinput(buff,8);
		if (buff[0]!=0) strcpy(gg[i-1].code,buff);
	}
	fp=fopen("system/gg.dat","w");
	for (i=0;i<10;i++) fwrite(&gg[i],sizeof_gg,1,fp) ;
	if (fp!=NULL) fclose(fp);
	printf("\n\n  ÀúÀåÀÌ ¿Ï·áµÇ¾ú½À´Ï´Ù. \n\n");
	getchar();
}






check_init_dir()
{
	check_dir("bbsdata/okrec");
	check_dir("menurec");
	check_dir("menucfg");
	check_dir("/tmp/xhost/rec");
	check_dir("/tmp/monich");
	check_dir("id");
	check_dir("menu");
	check_dir("sys_back");
	check_dir("userdata");
	check_dir("userroom");
	check_dir("user");
	check_dir("user/rec");
	check_dir("user/log");
	check_dir("user/score");
	check_dir("user/chat");
	check_dir("user/monich");
	check_dir("user/news");
	check_dir("user/fcmd");
	check_dir("user/moni");
	check_dir("user/lock");
	check_dir("user/message");
	check_dir("tmp");
	system("chmod 777 /tmp/xhost -R &");
	system("chmod 777 /tmp/system* &");
	system("chmod 777 /tmp/monich -R &");
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



view_daypf()		//ÀÏº° °¡ÀÔÇöÈ²
{
	FILE *fp,*fp2;
	time_t indate;
	int inday;
	int i,n,count1,count2;
	fp=fopen(ID_FILE,"r");
	i=0;
	printf("\n");
	indate=0;
	inday=count1=count2=0;
	fp2=fopen(TMP_FILE,"w");
	while (fread(&pf,sizeof_pf,1,fp)) {
		if (pf.inday<1) continue;
		n=time_num(pf.inday);
		if (inday>0&&inday!=n) {
			fprintf(fp2,"%s %6d¸í °¡ÀÔ  ´©Àû%6d¸í\n",datestr(35,indate),count1,count2);
			count1=0;
		}
		indate=pf.inday;
		inday=n;
		count1++;
		count2++;
	}
	fclose(fp);
	fclose(fp2);
	view_text(TMP_FILE);
}


view_all_club(int mode)
{
	FILE *fp1,*fp2;
	char id_file[300];
	char buff[200],a_code[1000][70];
	int anum[100],found;
	int i,count=0,club_count=0;
	for (i=0;i<100;i++) anum[i]=0;
	if ((fp1=fopen(MENU_FILE,"r"))==NULL) return;
	if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
	printf("\n ... Àá½Ã¸¸ ±â´Ù·Á ÁÖ½Ê½Ã¿ä. µ¿È£È¸¸¦ °Ë»öÁßÀÔ´Ï´Ù....\n");
	club_mode=Yes;
	fprintf(fp2,"¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬\n");
	if (mode==1) {
		fprintf(fp2,"    µ¿È£ÄÚµå µ¿È£È¸ÀÌ¸§          µ¿È£È¸Àå µ¿È£È¸µð·ºÅä¸®\n");
	}
	else if (mode==2) {
		fprintf(fp2,"    µ¿È£ÄÚµå µ¿È£È¸Àå ÃÑÈ¸¿ø ÃÑ¸Þ´º (¸Þ´º/ÀÚ·á½Ç/Å©±â) °¡ÀÔ/°ø°³/ÁøÀÔ\n");
	}
	else {
		fprintf(fp2,"    µ¿È£ÄÚµå µ¿È£È¸ÀÌ¸§          µ¿È£È¸Àå µ¿È£È¸µð·ºÅä¸®\n");
		fprintf(fp2," ÃÑÈ¸¿ø ÃÑ¸Þ´º (¸Þ´º/ÀÚ·á½Ç/Å©±â) °¡ÀÔ/°ø°³/ÁøÀÔ\n");
	}
	fprintf(fp2,"¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬\n");
	for (i=0;i<1000;i++) strcpy(a_code[i],"");
	while(fread(&club_go,sizeof_go,1,fp1)) {
		if (strlen(club_go.code)<1) continue;
		if (club_go.type=='s') {
			sprintf(buff,"%s%s",club_go.code,club_go.dir);
			found=No;
			for (i=0;i<club_count;i++) {
				if (is_same(a_code[i],buff)) {
					found=Yes;
					break;
				}
			}
			if (found) continue;
			strcpy(a_code[club_count++],buff);
			anum[3]++;
		}
		if (club_go.type!=' ') {anum[1]++;anum[2]++;}
			 if (is_char(PDSTYPE,club_go.type)) anum[4]++;
		else if (is_char(BBBTYPE,club_go.type)) anum[5]++;
		else if (club_go.type=='n') anum[7]++;
		else if (is_char(EXETYPE,club_go.type)) anum[6]++;

		if (club_go.type!='s') continue;
		read_ccfg();
		if (mode==1) {
			fprintf(fp2,"%-3d %-8.8s %-20.20s %-8s %-30.30s\n",
			count+1,club_go.code,club_go.name,ccfg.sysop,club_go.dir);
		}
		else if (mode==2) {
			sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
			sprintf(NOW_MENU ,"%s/%s",club_go.dir,MENU_FILE);
			fprintf(fp2,"%-3d %-8.8s %-8s %6d %6d (%4d %4d %6d) %2d %2d %2d\n",
			count+1,club_go.code,ccfg.sysop,
			count_cpf(id_file),count_menu(NOW_MENU),
			ccfg.maxmenu,ccfg.maxpdsmenu,ccfg.maxpdssize,
			ccfg.autoguest,ccfg.modeopen,ccfg.intype);
		}
		else {
			sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
			sprintf(NOW_MENU ,"%s/%s",club_go.dir,MENU_FILE);
			fprintf(fp2,"%-3d %-8.8s %-30.30s %-8s %-30.30s ",
			count+1,club_go.code,club_go.name,ccfg.sysop,club_go.dir);
			fprintf(fp2,"%6d %6d (%4d %4d %6d) %2d %2d %2d\n",
			count_cpf(id_file),count_menu(NOW_MENU),
			ccfg.maxmenu,ccfg.maxpdsmenu,ccfg.maxpdssize,
			ccfg.autoguest,ccfg.modeopen,ccfg.intype);
		}
		count++;
	}
	fprintf(fp2,"¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬\n");
	fprintf(fp2,"  ÃÑ¸Þ´º:%d  ¸ÞÀÎ¸Þ´º:%d µ¿È£È¸:%d ÀÚ·á½Ç:%d °Ô½ÃÆÇ:%d ¿ÜºÎ¸í·É:%d ÅÚ³Ý:%d\n",
			anum[1],anum[2],anum[3],anum[4],anum[5],anum[6],anum[7]);
	fclose(fp1);
	fclose(fp2);
	view_text(TMP_FILE);
	unlink(TMP_FILE);
}


edit_limit_id_file()
{
	FILE *fp;
	char filename[300];
	char buff[300],buff1[300],buff2[300],buff3[300];
	check_dir("user/limit");
	strcpy(filename,"user/limit/limit.id");
	xedit2(filename);
	system("rm -f user/limit/limit_*.id");
	if ((fp=fopen(filename,"r"))==NULL) return;
	while(fgets(buff,200,fp)!=NULL) {
		strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");
		sscanf(buff,"%s %s %s",buff1,buff2,buff3);
		if (strlen(buff1)<3) continue;
		sprintf(buff,"%s %s",buff1,buff2);
		sprintf(buff3,"user/limit/limit_%u.id",(word)buff1[0]%255);
		put_line(buff,buff3);
	}
	fclose(fp);
}
job_info()
{
	FILE *fp;
	FCFG fcfg;
	int i,n,num,n2;
	char buff[100];
	char filename[200];

	while (1) {
		cls();
		printf("  ###  ÀÎÆ÷¼¥ °ü·Ã ÀÛ¾÷ ###\n");
		printf("  1. ÀÎÆ÷¼¥¿ë : ±âº» ¼³Á¤ ÁöÁ¤     :%s\n",rbit(cfg.infomode,0,"ÀÏ¹Ý","ÀÎÆ÷¼¥ÁöÁ¤"));
		printf("  2. ÀÎÆ÷¼¥¿ë : ¸ðµçÈ¸¿ø ¾²±â°¡´É  :%s\n",rbit(cfg.infomode,1,"ºÒ°¡´É","°¡´É"));
		printf("  3. ÀÎÆ÷¼¥¿ë : ¸ðµçÈ¸¿ø µî·Ï°¡´É  :%s\n",rbit(cfg.infomode,2,"ºÒ°¡´É","°¡´É"));
		printf("  4. ÀÎÆ÷¼¥¿ë : ¸ðµçÈ¸¿ø ´Ù¿î°¡´É  :%s\n",rbit(cfg.infomode,3,"ºÒ°¡´É","°¡´É"));
		printf("  5. ÀÎÆ÷¼¥¿ë : ÀüÃ¼È¸¿ø ¼Õ´ÔÁ¢¼Ó  :%s\n",rbit(cfg.infomode,4,"IDÀÔ·Â","IDÅë°ú"));
		printf("  6. ÀÎÆ÷¼¥¿ë : ÁøÀÔ·Î°í ¾Èº¸ÀÓ    :%s\n",rbit(cfg.infomode,5,"º¸ÀÓ","Åë°ú"));
		printf("  7. ÀÎÆ÷¼¥¿ë : ¾ÆÀÌµð ÀÔ·Â¹ÞÀ½    :%s\n",rbit(cfg.infomode,6,"¹ÞÀ½","Åë°ú"));
		printf("  8. ÀÎÆ÷¼¥¿ë : °øÁö»çÇ× º¸¿©ÁÜ    :%s\n",rbit(cfg.infomode,7,"º¸ÀÓ","Åë°ú"));
		printf("  9. ÀÎÆ÷¼¥¿ë : Á¾°á½Ã   Áú¹®ÇÔ    :%s\n",rbit(cfg.infomode,8,"Áú¹®","Åë°ú"));
		printf(" 10. ÀÎÆ÷¼¥¿ë : ¸ðµç¼Õ´Ô->µî±Þ2È¸¿ø:%s\n",rbit(cfg.infomode,9,"º¸Åë","ÁöÁ¤"));
		n=numselect();
		xy(50,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.infomode=gtt_bit(cfg.infomode,0);
		if (n== 2)	cfg.infomode=gtt_bit(cfg.infomode,1);
		if (n== 3)	cfg.infomode=gtt_bit(cfg.infomode,2);
		if (n== 4)	cfg.infomode=gtt_bit(cfg.infomode,3);
		if (n== 5)	cfg.infomode=gtt_bit(cfg.infomode,4);
		if (n== 6)	cfg.infomode=gtt_bit(cfg.infomode,5);
		if (n== 7)	cfg.infomode=gtt_bit(cfg.infomode,6);
		if (n== 8)	cfg.infomode=gtt_bit(cfg.infomode,7);
		if (n== 9)	cfg.infomode=gtt_bit(cfg.infomode,8);
		if (n==10)	cfg.infomode=gtt_bit(cfg.infomode,9);
	}
}


set_roomtype_limit()	 //ºÐ·ùº° Á¦ÇÑ¼³Á¤
{
	FILE *fp;
	CROOM tcroom;
	int found=No,n;
	char filename[100],buff[100];
	strcpy(filename,"system/chattype.dat");
	while (1) {
		printf("\n ## ¼³Á¤ ´ëÈ­½Ç Å¸ÀÔ (1~39) >>");
		lineinput2(buff,3);
		n=atoi(buff);
		if (is_quit(buff)) return;
		if (n==0) continue;
		if (n<1||n>39) return;
		tcroom.age1=tcroom.age1=tcroom.itype=tcroom.sex=0;
		if ((fp=fopen(filename,"r"))!=NULL) {
			fseek(fp,n*sizeof_croom,SEEK_SET);
			fread(&tcroom,sizeof_croom,1,fp);
			fclose(fp);
		}
		disp_room_limit(n,tcroom);
		printf("\n  ´ëÈ­½Ç Å¸ÀÔ¹øÈ£: %d",n);
		printf("\n  ÁøÀÔ ³ªÀÌ ½ÃÀÛ : %2d >>",tcroom.age1 );tcroom.age1 =get_num(tcroom.age1 ,2);
		printf("\n  ÁøÀÔ ³ªÀÌ  ³¡  : %2d >>",tcroom.age2 );tcroom.age2 =get_num(tcroom.age2 ,2);
		printf("\n  ÁøÀÔ µî±Þ      : %2d >>",tcroom.itype);tcroom.itype=get_num(tcroom.itype,2);
		printf("\n  ¼ºº° 1:³² 2:¿© : %2d >>",tcroom.sex  );tcroom.sex  =get_num(tcroom.sex  ,1);
		printf("\n");
		disp_room_limit(n,tcroom);
		if ((fp=fopen(filename,"r+"))==NULL)
			  fp=fopen(filename,"w+");
		fseek(fp,n*sizeof_croom,SEEK_SET);
		fwrite(&tcroom,sizeof_croom,1,fp);
		fclose(fp);
	}
}


disp_room_limit(int n,CROOM tcroom)
{
	printf(" ## ´ëÈ­½Ç Å¸ÀÔ¹øÈ£: %d \n",n);
	printf(" ## ÁøÀÔ ³ªÀÌ ½ÃÀÛ : %2d - %2d \n",tcroom.age1,tcroom.age2);
	printf(" ## ÁøÀÔ µî±Þ      : %2d \n",tcroom.itype);
	printf(" ## ¼ºº° 1:³² 2:¿© : %2d \n",tcroom.sex  );
}
