
////////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
// ³»¿ë : ¸ÞÀÎ ÇÁ·Î±×·¥ main.c
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include "chat.h"

char cmdlinenum;
char acmd[20][100];

main(int argc, char **argv)
{
	char buff[250],buff2[80];
	int i,n;
	char is_disp_help=Yes;
	allmode=0;					 //ÀüÃ¼¸ðµå
	allmode2=0; 				 //ÀüÃ¼¸ðµå
	if (argc>1&&is_same(argv[1],"/ver")) {
		printf("\n    VER:%s %s Date:%s Build:%s\n  ",VER1,VER2,MAKEDATE,MAKENUM);
		fflush(stdout);msleep(5);fflush(stdout);
		return;
	}
	read_cfg(); 				//config ÀÐ±â

	cfg.waitkey=5;
	cfg.hostno=0;
	_last2=1;;;;;;;;;;;;;;;;;;;;;;;;
	allmode=limit_mode=0;
	findport(tty); //Á¢¼ÓÆ÷Æ® Ã£±â
	sprintf(buff,"/dev/%s",tty);chmod(buff,0666);
	set_signal();
	if (argc>1&&strlen(argv[1])>2&&stristr(argv[1],"/")) {
		ch_dir(argv[1]);
	}
	strcpy(buff2,argv[0]);
	pure_name(2,buff2);   //È­ÀÏ¸í¿¡¼­ È®ÀåÀÚ Á¦¿Ü »Ì¾Æ³¿
	if (strlen(buff2)<1) {
		strcpy(buff2,argv[0]);
		pure_name(0,buff2);   //È­ÀÏ¸í¿¡¼­ È®ÀåÀÚ Á¦¿Ü »Ì¾Æ³¿
	}
	n=0;
	strchng2(buff2,"-bbs","bbs");
         if (is_same(buff2,"xmain2")||is_same(buff2,"xmain9")) {n=1;allmode2=set_bit(allmode2,11,1);ch_dir("/xhost");}
    else if (is_same(buff2,"xmain" )||is_same(buff2,"xmain7")) {n=1;allmode2=set_bit(allmode2,13,1);ch_dir("/bbs");}
	else if (stristr(buff2,"bbs")) {        //bbs°¡ ½ÇÁ¦È­ÀÏ¸í¿¡ Æ÷ÇÔµÇ¾ú´Â°¡?
		sprintf(buff,"/bbs/%s",buff2);
		ch_dir(buff);
		allmode=set_bit(allmode,19,1);
	}

	xstrncpy(hostname,view_network(tty),20);
	if (n==1&&argc>2&&is_same(argv[1],"-h")) {
		strcpy(hostname,argv[2]);
		if (strlen(hostname)>0&&is_char(hostname,'.')) {
			allmode2=set_bit(allmode2,10,1); //HOSTNO¸¦ ÆÄ¶ó¸ÞÅ¸·Î ³Ñ±è
		  //12991 pts/58   S	  0:00 /bbs/bin/xmain -h 203.254.78.211 -p
		}
	}

	if (bit(cfg.mode,28)==0) set_alarm(1);

	netmode=0;					 //ÀüÃ¼¸ðµå
	pf.todaymode=0; 			   //¿À´ÃÇÑÀÏ
	ascr_count=0;
	mymoni_rec=0;
	c_proc=No;
	c_proc2=No;
	c_proc3=No;
	autoup=No;
	c_proc =is_file("system/c_proc.tag");
	c_proc2=is_file("system/c_proc2.tag");
	c_proc3=is_file("system/c_proc3.tag");
	if (c_proc) printf("HOST Å×½ºÆ®Áß..step 1  TTY:%s\n",ttyname(1));
	CONT_DAY=0; 				//¿¬¼ÓÃâ·ÂÀÏ¼ö

	allmode=set_bit(allmode,18,is_file("system/alliprec"));
	if (c_proc) printf("HOST Å×½ºÆ®Áß..step 3\n");
	check_login_init(); 			//ÃÊ±â¼³Á¤
	log_in();						//ÃÊ±â¼³Á¤ ÅÍ¹Ì³¯ ¼³Á¤µî ¼¼ÆÃ

	fflush(NULL);					//5

	SEND_KB=0;
	time(&START_TIME);			//Á¢¼Ó½ÃÀÛ½Ã°£
	time(&CLUB_START_TIME); 		 //Á¢¼Ó½ÃÀÛ½Ã°£
	time(&CHECK_TIME);			//Á¢¼Ó½ÃÀÛ½Ã°£
	time(&pf.login);			//Á¢¼Ó½ÃÀÛ½Ã°£
	strcpy(pf.id,"Á¢¼ÓÁß");
	strcpy(pf.nick,"Á¢¼ÓÁß");
	read_cfg(); 				//config ÀÐ±â

	if (is_file("filelock.tag")) cfg.mprocmode=set_bit(cfg.mprocmode,3,1);

	set_tmpfile();
	read_bbsgood_color();
	strcpy(buff,"system/notushost.txt");
	if (in_file2(buff,hostname)) strcpy(hostname,tty);
	if (c_proc) printf("HOST Å×½ºÆ®Áß..step 5\n");
	view("frm/log/loghost.log");
	//ÇØ´çÈ£½ºÆ®´Â ±â·Ï¾ÈÇÔ
	if (in_file2("system/rec_noth.txt",hostname)) allmode=set_bit(allmode,8,1);

	srand(time(NULL));			//³­¼öÀÇ ÃÊ±âÈ­
	now_cmd_num=rand()%20;		//ÇöÀçÀÇ µµ¿ò¸» ¹øÈ£
	DM=DM1=DM2=DM3=No;
	printf("[1;50r[;H");
	setenv("TERM","vt220",1);
	sprintf(buff,"/dev/%s",tty);chmod(buff,0666);
	del_stat(tty);			 //»óÅÂ±â·Ï È­ÀÏ ÇØ´ç Æ÷Æ®ÀÇ Á¢¼ÓÀ» ÇØÁ¦ÇÑ´Ù.
	go_top(0);
	strcpy(go.name,"¾ÆÀÌµðÀÔ·ÂÁß");
	sprintf(moni.goname,"%-20.20s",go.name);
	set_stat(23,"Á¢¼ÓÁß");
	cashmax1=0;
	cashmax2=0;
	g_yes=No;
	allmode=set_bit(allmode,20,in_file2("system/node_net.txt",tty));
	set_guest_();
	bbsdownnum=rand()+1000;   //°Ô½ÃÆÇ´Ù¿î¹øÈ£	¾Æµð.¹øÈ£

	if (!bit(cfg.infomode,0)&&!bit(cfg.infomode,5)) start_logo(0);
	/* »óÅÂ È­ÀÏ¿¡ 0À» Áý¾î³Ö´Â´Ù. ÀÌ°ÍÀº º¸Åë»óÅÂ¶ó´Â °ÍÀ» ¾Ë¸®´Â °ÍÀÌ´Ù. */
	read_atype();				//µî±ÞÀÇ ÀÌ¸§À» ÀÐ¾î¿È	->atype
	read_ajob();				//Á÷¾÷À» ÀÐ¾î¿È
	go_top(0);
	strcpy(club_go.code,"main");             //µ¿È£È¸°¡±âÄÚµå
	strcpy(club_go.dir,".");
	guest_mode=Yes;
	club_mode=No;
	strcpy(auto_cmd,"");
	re_print=Yes;			//È­¸éÃâ·Â
	re_logo=Yes;			//º¸Á¶È­¸éÃâ·Â
	re_bbs=Yes; 			//°Ô½ÃÆÇ·Îµù
	re_club=No; 			//µ¿È£È¸ÃÊ±âÈ­
	p_command=No;			//¸¶Áö¸· ÀÔ·ÂÀÌ PÀÎ°¡?
	bbs_ltmode=No;			//ltµîÀ¸·Î °Ë»çÇÑ mode
	list_mode=No;			//ÇöÀç ¹øÈ£ ¼±ÅÃ¸ðµå
	dd_mode=No; 			//»èÁ¦µÈ°Íº¸¿©ÁÜ
	dark_mode=0;			//½Ã»ð°¨Ãã ¸ðµå 1:ÀÏ¶§
	is_disp_help=Yes;
	now_type=0; 				//µ¿È£È¸ÀÇ µî±ÞÇ¥½Ã
	strcpy(auto_cmd,"");    //¼Õ´ÔÀÏ¶§
	strcpy(DEF_UPS,"");
	strcpy(DEF_DNS,"");
	strcpy(news_code,cfg.newsfile);
	UP_LEVEL=0; 			//¿À´Ã¹ÞÀº ·¹º§
	UP_CASH =0; 			//¿À´Ã¹ÞÀº Ä³½¬
	N_page=0;

	tag_monichar(); //ÇöÀç ±â·Ï
	strcpy(last_club,club_go.code);
	if (bit(cfg.startmode,1)) goto start_now1;		  //¹Ù·Î ÁøÀÔ
	if (bit(cfg.notice,2)) view_notice(1,"notice");            //°øÁö»çÇ× º¸±â

	//////////////////////ID ÀÔ·ÂÁß
	if (!bit(cfg.infomode,6)) {
		if (argc>1) start_id(0,argv[1]);  //-l ID Ã³¸®
		else		start_id(0,"");       //¾ÆÀÌµð¸¦ ÀÔ·Â¹Þ°í ÃÊ±â È­¸éÀ» º¸¿©ÁØ´Ù.
	}
	else {
		set_guest_mode2();
	}
    quit_all_room(3);        //´ëÈ­¹æ¿¡ ÀÖ´Â ÇØ´çID,°°Àºtty,Æ÷Æ®¸¦ Á¦°ÅÇÑ´Ù.
	no_spc(pf.id);
	//ÇØ´çID´Â ±â·Ï¾ÈÇÔ
	i=No;
	sprintf(buff,"%s/f_nid.dat",REC_DIR);   if (i==No&&in_file2(buff,pf.id   )) allmode=set_bit(allmode,29,1);
	sprintf(buff,"%s/f_ntty.dat",REC_DIR);  if (i==No&&in_file2(buff,tty     )) allmode=set_bit(allmode,29,1);
	sprintf(buff,"%s/f_nhost.dat",REC_DIR); if (i==No&&in_file2(buff,hostname)) allmode=set_bit(allmode,29,1);
	if (in_file2("system/rec_noti.txt",pf.id))    allmode=set_bit(allmode,9,1);


    save_login(1);          //½ÃÀÛ±â·Ï
	check_all_limit();		//ÀüÃ¼ Á¦ÇÑ ¼³Á¤
	set_rec(1,23);		// Á¢¼Ó½ÃÀÛ,¸ðµå,size,Âü°í
	sys_tem("rm -f tmp/tmp*%d%s",cfg.hostno,no_spc(tty));   //È£½ºÆ®¹øÈ£º°ÀÓ½ÃÈ­ÀÏ
	sys_tem("rm -rf tmp/up%d%s" ,cfg.hostno,no_spc(tty));   //È£½ºÆ®¹øÈ£º°ÀÓ½ÃÈ­ÀÏ
	set_stat2(23,"Á¢¼ÓÁß");
	//µî±Þ18ÀÌ»óÀº Å°´ë±â ¹«½Ã
	if ((bit(cfg.mode,0)&&pf.type>=18)||in_file2("system/nokey.id",pf.id))
		allmode=set_bit(allmode,13,1);

	check_wait(0);				//±â´Ù¸²¿©ºÎ °Ë»ö
	strcpy(mymoni,"");

	clear_node();
	rm_tmpup();
	if (pf.type>0) check_smemo();			   //¿î¿µÁø¼ö½Å¸Þ¸ð¸¦ °Ë»öÇÑ´Ù.
	set_pftype();  //È¸¿øÀÇ µî±Þ,ÀÏÀÏ»ç¿ë°¡´É½Ã°£À» È®ÀÎÇÑ´Ù.

	if (!bit(cfg.infomode,7)) {
		view_loginhelp();		 //ÁøÀÔ½Ã Àá±ñµµ¿ò¸» º¸ÀÌ±â
		view_notice(1,"notice");   //°øÁö»çÇ× º¸±â
	}
	if (auto_cmd[0]==0&&is_gocode(pf.gofirst))
		sprintf(auto_cmd,"go %s",pf.gofirst); //ÀÚµ¿°¡±âÄÚµå
	auto_go_start(auto_cmd,pf.type);  //µî±Þº° °­Á¦°¡±â
	check_warn_mess();				  //¸ÞÀÎÀÇ °æ°íÀÎ°¡?
	set_stat2(1,"ÃÊ±âÈ­¸é");

	sprintf(buff,"user/fcmd/fcmd1.%s",no_spc(tty));      //°­Á¦ÀçÁ¢¼ÓÀÎ°¡?
	if (is_file2(buff)) file_line(auto_cmd,buff,1);
	sprintf(buff,"user/fcmd/fcmd2.%s",pf.id);    //°­Á¦ÀçÁ¢¼ÓÀÎ°¡?
	if (is_file2(buff)) file_line(auto_cmd,buff,1);

	start_now1: 	   //¹Ù·Î ÁøÀÔ
	if (bit(cfg.guestmode,14)) {ch_dir("/xguest");execl("bin/main","main",NULL);}
	for (i=0;i<20;i++) bzero(acmd[i],78);
	cmdlinenum=0;
	re_logo=Yes;
	//ÁøÀÔ½Ã ÇÏµå µð½ºÅ©¿ë·® È®ÀÎ
	if (bit(cfg.startmode,16)&&check_free()<50) {
		view("frm/system/nospace.log");
		if (pf.type<13) host_end();
	}

		 if (bit(cfg.zmode,23)) check_zbtotal(1*60*60,0);
	else if (bit(cfg.zmode,24)) check_zbtotal(6*60*60,0);

	//°Ô½ÃÆÇ±â·Ï°ü·Ã
	strcpy(bbsrec_club,"");
	strcpy(bbsrec_code,"");
	bbsrec_time=0;
    add_logcount();      //Á¢¼Ó±â·Ï
	if (guest_mode&&bit(cfg.guestmode,1)) strcpy(auto_cmd,"bye");
	while(1) {
		if ( is_char("mr",go.type )) check_bbsrec(1);  //°Ô½ÃÆÇ ±â·Ï °ü·Ã ÇØÁ¦
		else						 check_bbsrec(0);  //°Ô½ÃÆÇ ±â·Ï °ü·Ã ¼³Á¤
		nostop=No;				  //¸ØÃßÁö ¾ÊÀ½(·Î°íÆíÁý½Ã)
		if (strlen(pf.id)<3) {
			check_error(1);
			log_rc(0);
		}
		if (re_club) {
			printf(VTXOFF);
			if (club_mode) club_init();    //µ¿È£È¸ÀÇ ÃÊ±âÈ­
			else		   main_init();    //µ¿È£È¸°¡ ¾Æ´Ò¶§ ´Ù½Ã º¹±¸ÇÔ
			re_club=No;
			re_print=Yes;
			time(&CHECK_TIME);			//Á¢¼Ó½ÃÀÛ½Ã°£
			tag_monichar(); //ÇöÀç ±â·Ï
			continue;
		}

		if (is_char("rejnp",go.type)) {
			go_prev();
			continue;
		}
		check_another_rec();		//µ¿È£È¸À§Ä¡º¯°æ½Ã ±â·Ï
		if (re_logo) {
			view_pos=0;
			if (bit(cfg.mode,28)==0) set_alarm(1);
			read_bbsgood_color();
			if (check_type('i')) {
				go_prev();
				continue;
			}
			if (auto_cmd[0]==0) disp_menu_logo();	//¸Þ´ºÁøÀÔ½Ã º¸¿©ÁÖ´Â ·Î°í
			re_logo=No;
			re_print=Yes;
			list_mode=No;
			re_print=Yes;
		}
		strcpy(_last1,"");

		// È­¸éÀ» Ç¥½ÃÇÑ´Ù.
		input_0:
		if (re_print) {
			allmode2=set_bit(allmode2,2,0);  //´Ù¿î,¾÷½Ã ½Ã°£È®ÀÎ±ÝÁöÇØÁ¦
			if (go.type=='m'&&!is_same(go.code,"top")) {
				if (pf.type<cfg.limit_m) {go_top(0);continue;}
				sprintf(buff,"%s/logo/%s.ndn",club_go.dir,go.code);
				if (is_file2(buff)) {view(buff);prev();continue;}
			}
			allmode=set_bit(allmode,30,0);
			sprintf(buff,"%s/f_ncode.dat",REC_DIR);
			if (in_file2(buff,go.code)) allmode=set_bit(allmode,30,1);
			else {
				sprintf(buff,"%s/f_nclub.dat",REC_DIR);
				if (in_file2(buff,club_go.code)) allmode=set_bit(allmode,30,1);
			}

			set_news(Yes);			   //´º½º°¡´É
			for (i=0;i<50;i++) list_anum[i]=0;	 //È­¸éÃâ·Â ¹øÈ£
			if (pf.warn%10>=8) go_code("_warn");
			if (list_mode) set_stat(1,bbs.title);  //ÇöÀçÀ§Ä¡ ÀúÀå
			else		   set_stat(1,""       );  //ÇöÀçÀ§Ä¡ ÀúÀå
			if (strlen(auto_cmd)<1) disp_screen();	   //¸Þ´º,¸®½ºÆ®
			N_page=1;
			re_print=No;
			allmode=set_bit(allmode,16,1);
			if (!bit(cfg.logomode,7)) disp_todaymode();
			if (go.type=='m'&&bit(cfg.mode,11)) xy(1,23);
			if (!bit(cfg.logomode,10)) {
				set_alarm(1);
				sig_handler(SIGALRM);
			}
		}
		input_1:
		disp_mid(); 		 //NoÀÌ¸é µµ¿ò¸» Ãâ·Â¾ÈÇÔ ¸í·ÉÀÔ·Â¹Þ±âÀü ¸Þ¼¼Áö Ç¥½Ã
		sprintf(buff,"user/fcmd/fcmd3.%s",no_spc(tty));      //°­Á¦ÀçÁ¢¼ÓÀÎ°¡?
		if (is_file2(buff)) {
			file_line(auto_cmd,buff,1);
			unlink(buff);
		}
		all_trim(auto_cmd);
		if (strlen(auto_cmd)>0) {		   //ÀÚµ¿¸í·ÉÃ³¸®
			del_esc(auto_cmd);
			strcpy(buff,auto_cmd);
			strcpy(auto_cmd,"");
		}
		else if (sub_go[0]!=0) {		//ÀÚµ¿¸í·ÉÃ³¸®
			strcpy(buff,sub_go);
			strcpy(sub_go,"");
		}
		else {						   //¸í·É¾î ÀÔ·Â·çÆ¾
			if (!bit(cfg.mode,4)&&!bit(pf.mymode,0)) printf(ESCENG);  //¿µ¹®À¸·Î º¯È¯ ÇÑ±Û:h
			input_2:
			printf("7");
			//¼±ÅÃ¸í·ÉÀÔ·Â
			if (!bit(cfg.newsmode,8)) allmode=set_bit(allmode,15,is_same(go.code,"top"));
			else					  allmode=set_bit(allmode,15,1);
			strcpy(buff,"");
			if (bit(cfg.mode,8)==0) {
				i=read_acmd(70);
				allmode=set_bit(allmode,15,0);
				if (i==1) {cmdlinenum--;printf("8[K");goto input_2;}
				if (i==2) {cmdlinenum++;printf("8[K");goto input_2;}
				strcpy(buff,acmd[cmdlinenum]);
			}
			else {
				lineinput(buff,200);
			}
			check_han(buff);
			if (strlen(buff)<5) strchng2(buff,"Š ","");
			if (strlen(buff)>0) next_acmd();

			if (is_same(buff,"ver")) {view_ver();continue;}
			if (stristr(buff,"B080")) {printf("8[K");goto input_2;}
			if (go.type=='m'&&strlen(buff)<1) {re_print=Yes;goto input_0;printf("\n");}
			set_news(No);
		}
		if (buff[0]!='_') {
			check_short(1,buff) ;				  //´ÜÃà¸í·ÉÅ° ÁöÁ¤
			check_short(2,buff) ;				  //°³ÀÎº°´ÜÃà¸í·ÉºÐ¼®
		}
		input_4:

		if (bit(cfg.mrecmode,11)&&strlen(buff)>0) cmd_record(buff);

		check_cmd(buff);					  //¸í·É¾î ºÐÇØ
		i=strlen(mmc[0]);

		if (i>0) {
			allmode=set_bit(allmode,16,0);	 //È­¸é¾Ö´Ï¸¦ À§ÇÑ Ã³¸®
			allmode=set_bit(allmode,17,0);	 //Å°ÀÔ·Â´ë±â¸¦ À§ÇÑ Ã³¸®
			printf("\n");
		}

		if (i<1) {						  //±×³ÉÀÔ·Â½Ã
				 if (is_bbspds&&!list_mode)  bbs_next();
			else if (is_bbspds&& list_mode)  bbs_read(1);
			else {
				goto input_1;
			}
		}
		else if (is_same(mmc[0],"go")&&mmc[2][0]=='_'&&(mmc[2][1]=='#'||mmc[2][1]=='*')) {
			if (!check_direct_go(1,buff)) {printf("\n");goto input_1;}
		}
		else if (do_menu_command()) 			  ; //¸Þ´º»óÀÇ ¸í·ÉÀÎ°¡?
		else if (do_basic_command(buff,buff))	  ; //±âº»¸í·É¾îÀÎ°¡?
		else if (do_list_command()) 			  ; //°Ô½ÃÆÇÀÇ ÀÚ·á¹øÈ£ÀÎ°¡?
		else if (do_order_command(mmc[0],buff))   ; //±âº»¸í·É¾îÀÎ°¡?
		else if (!bit(cfg.gomode,1)&&!is_same(mmc[0],"go")&&(mmc[0][0]=='0'||atoi(mmc[0])==0)&&strlen(mmc[0])>1) {
			if (!check_direct_go(0,buff)) {printf("\n");goto input_1;}
		}
	}
}

check_direct_go(int mode,char *buffstr)
{
	char buff[200],buff2[300];
	GO oldgo,oldclub;
	int old_clubmode=0;
	oldgo=go;
	oldclub=club_go;
	old_clubmode=club_mode;
	strcpy(buff,buffstr);
	if (mode) goto _cont3;

	if (strlen(mmc[1])>0&&is_same(mmc[0],"ÇØ")) {strcpy(mmc[0],"go");goto _cont3;}
	strcpy(buff2,buff);
	sprintf(buff,"go %s",buff2);
	strcpy(sub_go,"");
	check_cmd(buff);
	_cont3:
	if (!go_code(mmc[1])) return No;
	check_smenu(buff);
	if (is_bbspds&&mmc[2][0]=='_'&&mmc[2][1]=='#') {
		read_bbs_direct(0,mmc[2]);
	}
	else if (is_bbspds&&mmc[2][0]=='_'&&mmc[2][1]=='*') {
		read_bbs_direct(1,mmc[2]);
		club_mode=old_clubmode;
		club_go=oldclub;
		go=oldgo;
		set_nowmenu();
		go_code(go.code);
		re_logo=Yes;
		list_mode=No;
	}
	return Yes;
}


read_bbs_direct(int mode,char *str)
{
	char buff[200];
	int n;
	strcpy(buff,str);
	strchng2(buff,"_#","");
	strchng2(buff,"_*","");
	if (atoi(buff)>0||is_same(buff,"!")) {
		check_cmd(buff);
		set_filename(); 						//ÇöÀçÇ¥½ÃÇÒ È­ÀÏ¸í
		read_bcfg();
		re_bbsnum(1);			//BREC 0:ÀÐ±â 1:ÀÚµ¿  2:¿À´Ã  3:ÀüÃ¼°»½Å  8:1°³Ãß°¡ 9:1°³»èÁ¦
		re_bbs=re_logo=re_print=No;
		if (check_type('i')||no_exec_port(go.code)) {
			go_top(0);
			return; //»ç¿ë°¡´É ¸Þ´ºÀÎ°¡?
		}
		check_bbs();		  //ÀÚ·áÀÇ °¹¼ö¸¦ ÆÄ¾ÇÇÏ´Â ·çÆ¾
		re_bbs=No;
		if (is_same(buff,"!")) {
			view_last_num(1);
			NOW_NO=bbs.num;
			NOW_TOPREC=NOW_REC+1;
			NOW_TOPNO =NOW_NO;
			do_list_command();
		}
		else if (bbs_num(atoi(buff))) {
			NOW_NO=bbs.num;
			NOW_TOPREC=NOW_REC+1;
			NOW_TOPNO =NOW_NO;
			do_list_command();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/// 	   µ¿È£È¸ °ü·Ã ÀÛ¾÷
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
club_init()
{
	int key=Yes;
	int i,n;
	char buff[300];
	char afile[10][300];


	if (ccfg.age>0) {
		n=get_age(pf);
		if (pf.type<1||n<ccfg.age/100||n>ccfg.age%100) {
			if (!view("%s/frm/pf/notage.log",club_go.dir)) {
				if (!view("frm/pf/notage.log")) {
					mess("µ¿È£È¸ ÁøÀÔ±ÝÁöÇÒ¼ö ¾ø´Â ³ªÀÌÀÔ´Ï´Ù");
				}
			}
			club_mode=No;
			go_top(0);
			return;
		}
	}

	if (bit(limit_mode,4)) {
		if (!view("frm/pf/notclub.log")) mess("µ¿È£È¸±ÝÁö");
		club_mode=No;
		go_top(0);
		return;
	}
	if (pf.type<18&&view("frm/down/noinclub.log")) {
		club_mode=No;
		go_top(0);
		return;
	}
	now_type=1;
	cpf.warn=0;
	re_club=No;
	club_mode=Yes;
	bbs_ltmode=No;		  //ltµîÀ¸·Î °Ë»çÁß ¸ðµå
	re_bbs=Yes;
	for (i=0;i<20;i++) strcpy(atype[i],"");
	pf.todaymode=set_bit(pf.todaymode,10,1);   //¿À´ÃÀÛ¾÷
	newsmode[0]=newsmode[1]=newsmode[2]=newsmode[3]=0;
	_last2=8;;;;;;;;;;;;;;;;;;;;;;;;

	bzero(auto_cmd,30);
	if (!is_file2(club_go.dir)) {
		sprintf(g_buff1,"%s (%s)",club_go.code,club_go.name);
		sprintf(g_buff2,"µð·ºÅä¸®:%s",club_go.dir);
		if (yesno3("frm/system/noclub.log",No)) {
			view("doc/makeclub.txt");
			pressenter();
		}
        go_top(0); //µ¿È£È¸ »óÀ§¸Þ´º·Î ÀÌµ¿
		return;
	}
	sprintf(buff,"%s/menustru/menu.dat",club_go.dir);
	if (!is_file2(buff)) {
		if (yesno2("µ¿È£È¸¸Þ´º¸¦ º¯È¯ÇÒ±î¿ä?(Y/n)>>",Yes)) {
			strcpy(buff,"upgrade/men2_50");
			if (!is_file2(buff)) mess("%sÆÄÀÏÀÌ ¾ø½À´Ï´Ù.",buff);
			else				sys_tem("%s '%s'",buff,club_go.dir);
		}
	}

	// µ¿È£È¸ ÁøÀÔ±ÝÁö Á¶°Ç
	// 0. µî±ÞÀÌ 0ÀÌ ¾Æ´Ô.
	// 1. µî±ÞÀÌ ÁøÀÔ°¡´É µî±ÞÀÌÇÏ
	// 2. ºñ°ø°³ µ¿È£È¸ÀÌ¸é¼­ È¸¿øÀÌ ¾Æ´Ô
	// 3. ºñ°ø°³ µ¿È£È¸ÀÇ °æ¿ì ¾ÏÈ£¸¦ ¹°¾îº½
	// 4. Àá½Ã»ç¿ë±ÝÁöÀÇ °æ¿ì
	set_nowmenu();
	read_ccfg();
	sys_tem("touch %s/system/clubid.dat",club_go.dir);
	check_cpf();			 //µî±ÞÀÐ±â -> now_type
	go_top(1);
	time(&CLUB_START_TIME);   //µ¿È£È¸ Á¢¼Ó½ÃÀÛ½Ã°£
	key=Yes;
	for (i=0;i<10;i++) {
		sprintf(afile[i],"%s/frm/error/whynot%d.log",club_go.dir,i);
		if (!is_file2(afile[i])) sprintf(afile[i],"frm/error/whynot%d.log",i);
	}
	if (now_type==0) {view(afile[0]);key=No;}	 //µ¿È£È¸ ÁøÀÔÃÖ¼Òµî±Þ
	else if (now_type<18&&(ccfg.stop>=1&&ccfg.stop<=5)) {view(afile[2]);key=No;}
	else if (pf.type<18&&ccfg.stop==9) {view(afile[3]);key=No;}
	else if (pf.type<17&&now_type<17&&ccfg.enterp[0]!=0) {
		view(afile[4]);
		if (check_pass(ccfg.enterp,4,"")==No) key=No;
		key=No;
	}
	else if (pf.type<18&&pf.type<ccfg.intype) {
		sprintf(g_buff1,"%d",ccfg.intype);sprintf(g_buff2,"%s",atype[ccfg.intype]);
		view(afile[7]);key=No;
	}
	else if (now_type<18&&now_type<ccfg.cintype) {
		sprintf(g_buff1,"%d",ccfg.cintype);sprintf(g_buff2,"%s",actype[ccfg.cintype]);
		view(afile[8]);key=No;
	}
	if (ss_check(6))   {view(afile[5]);key=No;}    //ÀüÃ¼ µ¿È£È¸ ÁøÀÔ±ÝÁö
	if (check_warn(4)) {view(afile[6]);key=No;}    //µ¿È£È¸ »ç¿ë±ÝÁö
	if (key==No) {go_top(0);re_club=Yes;return;}

	read_actype();			 //µ¿È£È¸µî±ÞÀÇ ÀÌ¸§À» ÀÐ¾î¿È  ->actype
	sprintf(buff,"µî±Þ:%d",now_type);
	start_logo(0);	//ÁøÀÔ½Ã º¸¿©ÁÖ´Â ¸Þ¼¼Áö
	if (!bit(cfg.infomode,0)&&cpf.all_log==1) { 	  //µ¿È£È¸ Ã³À½ Á¢¼Ó
		view("%s/frm/log/firstlo.log",club_go.dir);
		read_pf4(ccfg.sysop);
		strcpy(bbs.id,pf2.id);
		strcpy(bbs.name,pf2.name);
		sprintf(buff,"%s/frm/log/firstl1.log",club_go.dir);
		if (is_file2(buff))
			wmail4(pf.id,buff,"µ¿È£È¸ °¡ÀÔÀ» ÃàÇÏÇÕ´Ï´Ù.",1); //¸ÞÀÏ¹ß¼Û
		sprintf(buff,"%s/frm/log/firstl2.log",club_go.dir);
		if (is_file2(buff))
			wmail4(pf.id,buff,"µ¿È£È¸ °¡ÀÔÀ» Áø½ÉÀ¸·Î È¯¿µÇÕ´Ï´Ù.",1); //¸ÞÀÏ¹ß¼Û
	}
	if (strlen(sub_go)==0) view_notice(1,"notice");         //µ¿È£È¸ °øÁö»çÇ×
	strcpy(mmc[1],"");
	strcpy(mmc[2],"");
	check_warn_mess();			//µ¿È£È¸ÀÇ °æ°íÀÎ°¡?
	if (!bit(ccfg.logomode,3)&&now_type>0&&now_type<20) {
		strcpy(buff,actype[now_type]);
		del_space(buff);
		if (strlen(buff)>0) message(" %s´ÔÀº [%s]ÀÔ´Ï´Ù.",pf.id,actype[now_type]);
	}
	set_rec(1,2);	  //µ¿È£È¸Á¢¼Ó½ÃÀÛ
	check_maxmenu();  // µ¿È£È¸ ÃÖ´ë¸Þ´º¼ö È®ÀÎ
}


//½ÃÀÛ½Ã ¶Ç´Â µ¿È£È¸¿¡¼­ ºüÁ® ³ª¿Ã¶§
main_init()
{
	while (go.type=='s') go_prev2();    //µ¿È£È¸ ¼±ÅÃÀÌÀüÀ¸·Î
	now_type=0; 				//ÇöÀçÀÇ µî±Þ
	read_atype();				//µî±ÞÀÇ ÀÌ¸§À» ÀÐ¾î¿È	->atype
	re_print=Yes;
	re_bbs=Yes;
	re_club=No;
	newsmode[0]=newsmode[1]=newsmode[2]=newsmode[3]=0;
	club_mode=No;
	p_command=No;
	strcpy(club_go.dir,".");
	strcpy(club_go.dir,".");
	strcpy(club_go.code,"main");
	strcpy(club_go.name,"top");
	set_nowmenu();
	_last2=9;;;;;;;;;;;;;;;;;;;;;;;;
}


// µ¿È£È¸ÀÇ È¸¿ø ½Å»óÀ» ÀÐ°í µî±ÞÀ» ÆÄ¾ÇÇÑ´Ù.
check_cpf()
{
	int i;
	char buff[30];
	char buff2[180];
	int found=No;

	now_type=1;  //±âº»Àº ¼Õ´ÔÀ¸·Î Á¤ÇÑ´Ù.
	strcpy(cpf.id,"");
	strcpy(cpf.name,"");
	cpf.level=0;
	cpf.warn=0;
	cpf.all_log=0;
	if (!club_mode) return;

	//µ¿È£È¸´ã´çÀÌ°Å³ª	µ¿È£È¸ÀÇ ½Ã»ðÀÎ°æ¿ì
	if (pf.type==17)  now_type=18;
	if (pf.type>=18)  now_type=19;
	if (now_type<1)   now_type=1;
	if (now_type<=18) {
		//È¸¿øÁ¤º¸¸¦ ÀÐ¾î¿Â´Ù
		if (read_cpf(pf.id)) {
			if (cpf.type==0||cpf.type>now_type) now_type=cpf.type;
			strcpy(cpf.id  ,pf.id  );		//ÀÌ¸§º¯°æ½Ã ÀÚµ¿À¸·Î ¿Ã¸²
			strcpy(cpf.name,pf.name);		//ÀÌ¸§º¯°æ½Ã ÀÚµ¿À¸·Î ¿Ã¸²
			time(&cpf.logout);				//µ¿È£È¸ Á¢¼Ó ½Ã°£ Àû±â
			cpf.all_log++;				//µ¿È£È¸ Á¢¼Ó È½¼ö
			if (bit(ccfg.mode,1)) cpf.type=now_type=pf.type;
			if (is_same(ccfg.sysop,pf.id)) cpf.type=19;
			save_cpf(pf.id);				//È¸¿øÀÚ·á Àû±â
		}
	}
	if (now_type>19) now_type=19;
	if (now_type<19&&bit(ccfg.mode,1)) now_type=pf.type;
	if (is_same(ccfg.sysop,pf.id)) now_type=19;
	cpf.type=now_type;
}



//¸Þ´º½ÃÀÛ½Ã ÁøÀÔ·Î°í
//lo2°¡ ÀÖ°í.. pf.mymode=0ÀÌ¸é ·Î°í2¸¦ º¸¿©ÁØ´Ù.
disp_menu_logo()
{
	char key=No;
	char filename[200];
	char filename1[200];
	char filename2[200];
	char filename3[200];
	char filename4[200];
	strcpy(filename1,"");strcpy(filename2,"");
	if (is_same(go.code,"top")) {printf("[;r");cls();}
	if (p_command==No&&bit(pf.mymode,2)==0) {			//¸¶Áö¸· ÀÔ·ÂÀÌ PÀÎ°¡?
		sprintf(filename1,"%s/logo/%s.log",club_go.dir,go.code);
		local_logo(filename1);	  //·ÎÄ®¸Þ´º·Î°íÃ³¸®ÇÏ±â
		strchng(filename2,filename1,".log",".lo2"); //setansi
		strchng(filename3,filename1,".log",".lo3"); //¾ß°£
		strchng(filename4,filename1,".log",".lo4"); //¼Õ´Ô
		if (bit(pf.mymode,2)==0) {
			if (now_hour()>23&&now_hour()<5&&(key=view(filename3)));
			else if (guest_mode&&view(filename4));
			else if (bit(pf.mymode,6)==0) {
				if(!(key=view(filename2))) key=view(filename1);    //ÁøÀÔÈ­¸éÃâ·Â  *.log
			}
			else {
				key=view(filename1);	//ÁøÀÔÈ­¸éÃâ·Â	*.log
			}
			mview(Yes,"%s/logo/%s.vtx",club_go.dir,go.code);
			mview(Yes,"%s/logo/%s.jpg",club_go.dir,go.code);
		}
	}
	if (is_bbspds&&bit(bcfg.form,4)) pressenter();
	if (is_bbspds&&bit(bcfg.form,5)) strcpy(auto_cmd,"!");
	if (is_bbspds&&bit(bcfg.form,6)) strcpy(auto_cmd,"w");
}


// È­¸éÀ» Ç¥½ÃÇÑ´Ù.
disp_menu_screen()
{
	char filename1[300];
	char filename2[300];
	char filename3[300];
	char filename4[300];
	char buff[80];
	if (go.type!='m') return No;
	list_mode=No;

	/* ¸Þ´ºÈ­¸é Ãâ·Â  */
	sprintf(filename1,"%s/logo/%s.mnu",club_go.dir,go.code);
	local_logo(filename1);	  //·ÎÄ®¸Þ´º·Î°íÃ³¸®ÇÏ±â

	strcpy(filename2,filename1);strcpy(filename3,filename1);strcpy(filename4,filename1);
	strchng(filename2,filename1,".mnu",".mn2");  //setansiÇÒ°æ¿ì
	strchng(filename3,filename1,".mnu",".mn3");  //¾ß°£ÀÛ¾÷°æ¿ì
	strchng(filename4,filename1,".mnu",".mn4");  //¼Õ´ÔÀü¿ë·Î°í
	if (bit(pf.mymode,7)) { 	   //ÀÛ¾÷¿ëÈ­¸éÀÏ°æ¿ì
		strchng2(filename1,".mnu",".mn5");
		strcpy(filename2,filename1);
		strcpy(filename3,filename1);
		strcpy(filename4,filename1);
	}
	if		(bit(pf.mymode,2)&&pf.type>=cfg.no_logotype) no_logofile();
	else if (now_hour()>23&&now_hour()<5&&view(filename3));
	else if (guest_mode&&view(filename4));
	else if (bit(pf.mymode,6)&&(view(filename2)||view(filename1))) ;  //¸Þ´ºÈ­¸éÃâ·Â  *.mnu ½ÇÆÐ
	else if (!bit(pf.mymode,6)&&view(filename1)) ;	//¸Þ´ºÈ­¸éÃâ·Â	*.mnu ½ÇÆÐ
	else no_logofile(); //¸Þ´ºÅ¸ÀÔÀÏ¶§ È­¸éÀÌ ¾øÀ»¶§
	if (bit(pf.mymode,2)==0) {
		sprintf(filename1,"%s/logo/%s.ad%d",club_go.dir,go.code,now_cmd_num%10);
		local_logo(filename1);	  //·ÎÄ®¸Þ´º·Î°íÃ³¸®ÇÏ±â
		view(filename1);		   //±¤°í
	}
	return Yes;
}


disp_screen()
{
	if (go.type=='m') disp_menu_screen();
	else if (is_char(BBSTYPE,go.type)) {
		if (list_mode) bbs_read(0); //ÇØ´ç¹øÈ£ ³»¿ëÃâ·Â
		else	  run_bbsscreen();
	}
}


disp_mid()
{
	FILE *fp;
	int ret=No;
	int i,n,count;
	char buff[600];
	char buff1[600];
	char buff2[200];
	char filename[600];
	strcpy(buff,"");
	//¸í·ÉÁÙ ¿ìÃø ±¤°í
	sprintf(buff,"");
	now_cmd_num++;
	if (!list_mode&&bit(bcfg.mode,22)) return;	 //ÇÏ´Ü ¸í·É¾î ¼±ÅÃ °¨Ãã
	if (list_mode &&bit(bcfg.mode,23)) return;	 //ÇÏ´Ü ¸í·É¾î ¼±ÅÃ °¨Ãã

	//½Ã»ð ¸Þ¼¼Áö ¿Â°ÍÀÌ ÀÖ´Â°¡?
	view("system/sysmess.txt");  //ÀüÃ¼¸Þ¼¼Áö
	if (go.type=='m') view("system/sysmess1.txt");  //ÀüÃ¼¸Þ¼¼Áö
	if (go.type=='m'&&is_same(go.code,"top")&&!club_mode) view("system/sysmess2.txt");  //ÀüÃ¼¸Þ¼¼Áö
	//¸Þ¼¼Áö ¿Â°ÍÀÌ ÀÖ´Â°¡?
	sprintf(filename,"userdata/%s/%s/mess.dat",c_han(pf.id),pf.id);
	if (view(filename)) {pressenter();printf("\n");unlink(filename);}
	if (!bit(cfg.infomode,0)&&!club_mode&&guest_mode&&(is_same(go.code,"top")||!bit(cfg.guestmode,9))) {
		view("frm/guest/guestin%d.msg",now_cmd_num%10);
	}
	if (!is_pdsbbs) list_mode=No;

	check_samemoni();  //µ¿½ÃÁ¢¼ÓÃâ·Â
	if (bit(cfg.logomode,5)) {
		if (bcfg.cmd_x>0||bcfg.cmd_y>0) xy(bcfg.cmd_x,bcfg.cmd_y);
		printf("[0;1m[80D[K ¼±ÅÃ>>");
	}
	else {
		if (list_mode) sprintf(filename,"%s/logo/%s.cm2",club_go.dir,go.code);
		else		   sprintf(filename,"%s/logo/%s.cm1",club_go.dir,go.code);
		local_logo(filename);	 //·ÎÄ®¸Þ´º·Î°íÃ³¸®ÇÏ±â
		if (!is_file2(filename)) {
			set_bbs_logo("cm2",filename);
			if (list_mode) set_bbs_logo("cm2",filename);
			else		   set_bbs_logo("cm1",filename);
		}
		if (!is_file2(filename)) strchng2(filename,"cm2","cm1");
		if (bit(allmode,5)) {
			strcpy(buff,filename);
			if (list_mode) strchng2(buff,"cm2","cm4");
			else		   strchng2(buff,"cm1","cm3");
			if (is_file2(buff)) strcpy(filename,buff);
		}
		if (bcfg.cmd_x>0||bcfg.cmd_y>0) xy(bcfg.cmd_x,bcfg.cmd_y);
		if (!view(filename)) printf("[0;1m[80D[K");
	}

	//ÀÓ½Ã¸Þ¼¼Áö
	sprintf(buff,"");

	//ÇÏ´Ü ±¤°íº¸¿©ÁÜ
	sprintf(buff1,"%s/frm/ad/ad.dat",club_go.dir);
	if (!is_file2(buff1)) sprintf(buff1,"frm/ad/ad.dat");
	strcpy(buff2,"frm/guest/guestad.dat");
	if (pf.type<1&&!club_mode&&!bit(cfg.guestmode,9)&&is_file2(buff2)) strcpy(buff1,buff2);
	count=0;
	if ((fp=fopen(buff1,"r"))!=NULL) {
		while (fgets(buff1,500,fp))
			if (count++==now_cmd_num%50+1) {
				xstrncpy(buff,buff1,80);
				del_enter(buff);
				break;
			}
		fclose(fp);
	}

	sprintf(filename,"user/message/%s.mes",no_spc(tty));
	if (is_file2(filename)) {
		file2buff(buff,filename,500);
		del_enter(buff);
		unlink(filename);

	}
	if (strlen(buff)>1) {
		printf("7[60D[30C[K");   //ÇÏ´Ü±¤°íÇÏ±â
		view3(buff);
		printf("[0;1m8");   //ÇÏ´Ü±¤°íÇÏ±â
	}
}



disp_todaymode()
{
	int i;
	char buff[200];
	/*1.¾²±â2.ÀÚ·áµî·Ï3.say·Î¸Þ¼¼Áöº¸³»±â3.say·Î¸Þ¼¼Áöº¸³»±â
	4.ÀÚ·áÀÐ±â5.ÀÚ·á´Ù¿î6.´ëÈ­½Ç7.µµ¿ò¸»(H,HELP)8.½Å»óÆíÁýABC9.°Ë»ö10.µ¿È£È¸ÁøÀÔ*/
	if (bit(cfg.newsmode,0)||!bit(cfg.newsmode,30)||bit(pf.mymode,22)) return;
	if (bit(bcfg.mode,20)) return;
	if (pf.type<cfg.newstype) return;
	if (newsmode[2]) return;
	if (go.type=='m'&&!club_mode&&is_same(go.code,"top")&&!bit(cfg.logomode,6)) {
		for (i=0;i<32;i++) {
			if (!bit(cfg.check_todayon,i)&&!bit(pf.todaymode,i)) {	 //¿À´ÃÀÛ¾÷
				sprintf(buff,"frm/help/mess%d-%d.log",i,rand()%10);
				if (is_file(buff)) {
					printf("[s");fflush(NULL);
					view("frm/help/mess%d-%d.log",i,rand()%10);
					printf("[u");fflush(NULL);
				}
			}
		}
	}
}


set_pftype()  //È¸¿øÀÇ µî±Þ,ÀÏÀÏ»ç¿ë°¡´É½Ã°£À» È®ÀÎÇÑ´Ù.
{
	int old_type;
	int i,n;
	FILE *fp;
	char buff[100];

	TODAY_TIM1=24*60*60;
	TODAY_TIM2=24*60*60;
	TODAY_DOWN=1000000;
	if (pf.type<1 )    pf.type=0;
	if (pf.type>19)    pf.type=1;
	old_type=pf.type;
	fp=fopen("system/pftype.dat","r");   //µî±ÞÈ­ÀÏ
	if (fp==NULL) return;
	for (i=0;i<14;i++) {
		fread(&n,sizeof(n),1,fp);	   //¼Õ´Ô,¿î¿µÁøÀº Àû¿ë¾ÈµÊ
		if (pf.type>0&&pf.type<14) {
			if (pf.level>n) {
				if (i>0) pf.type=i;
			}
		}
	}
	for (i=0;i<14;i++) {			 //ÇÑ¹ø Á¢¼Ó °¡´É½Ã°£
		fread(&n,sizeof(n),1,fp);
		if (pf.type==i) TODAY_TIM1=n*60;
	}

	for (i=0;i<14;i++) {			 //ÀÏÀÏ »ç¿ë°¡´É½Ã°£
		fread(&n,sizeof(n),1,fp);
		if (pf.type==i) TODAY_TIM2=n*60;
	}
	for (i=0;i<14;i++) {			 //ÀÏÀÏ´Ù¿î°¡´É¿ë·®
		fread(&n,sizeof(n),1,fp);
		if (pf.type==i) TODAY_DOWN=n;
	}
	if (fp!=NULL) fclose(fp);

	if (!bit(cfg.levelmode,1)) pf.type=old_type;
	if (pf.type!=old_type) {	//µî±Þº¯°æ½Ã ÀúÀåÇÔ
		if (pf.type>0) save_pf(3);
		sprintf(g_buff1,"%d",old_type);
		if (pf.type>old_type) view("frm/level/levelup.%d",pf.type);
		else				  view("frm/level/leveldn.%d",pf.type);
	}
}


set_news(int mode)
{
	char filename[200];
	cfg.newsmode=set_bit(cfg.newsmode,30,mode);
	news_count=0;
}


view_loginhelp()		//ÁøÀÔ½Ã Àá±ñµµ¿ò¸» º¸ÀÌ±â
{
	if (bit(cfg.infomode,0)) return;
	if ( (guest_mode&&bit(cfg.messmode,2)) ||
		 (!guest_mode&&bit(cfg.messmode,3)) ) return;
	if (pf.type<cfg.helptype||pf.level<cfg.helplevel||pf.all_log<cfg.helplog) {
		sys_tem("bin/bbsrand main bbs_help guest 0");
	}
}


read_acmd(int len)
{
	if (cmdlinenum< 0) cmdlinenum=19;
	if (cmdlinenum>19) cmdlinenum=0;
//	  printf("*%s*%s*%s*",auto_cmd,sub_go,acmd[cmdlinenum]);
	if (strlen(acmd[cmdlinenum])>76) strcpy(acmd[cmdlinenum],"");
	return doskey(acmd[cmdlinenum],len,1);
}


next_acmd()
{
	int i,n=0,i1,i2;
//	for (i1=0;i1<19;i1++) printf("%2d , %2d=%s\n",cmdlinenum,i1,acmd[i1]); pressenter();
	i1=cmdlinenum+11;
	while(n++<10) {
		if (i1>19) i1-=20;
		if (i1< 0) i1+=20;
		i2=i1-1; if (i2<0) i2=19;
		if (strlen(acmd[i2])>78) strcpy(acmd[i2],"");
		strcpy(acmd[i1],acmd[i2]);strcpy(acmd[i2],"");
		i1--;
	}
//	  for (i1=0;i1<19;i1++) printf("%2d , %2d=%s\n",cmdlinenum,i1,acmd[i1]); pressenter();
	cmdlinenum++;
	if (cmdlinenum>19) cmdlinenum=0;
}



local_logo(char *filename)
{
	char old_filename[300],buff[300],buff2[300];
	if (!club_mode&&pf.type>0&&pf.localmenu>0&&bit(cfg.gomode,11)) {
		strcpy(old_filename,filename);
		sprintf(buff2,"logo/%d/",pf.localmenu);
		if (!strchng2(filename,"logo/",buff2)) {
			sprintf(buff2,"menustru/%d/",pf.localmenu);
			strchng2(filename,"menustru/",buff2);
		}
		if (!is_file2(filename)) strcpy(filename,old_filename);
	}
}


cmd_record(char *cmd)
{
	FILE *fp;
	char filename[50];
	strcpy(filename,"user/rec/all_cmd.rec");
	if ((fp=fopen(filename,"a+"))==NULL) fp=fopen(filename,"w+");
	if (fp!=NULL) {
		fprintf(fp,"%s %-8s '%s'\n",now_str(51),pf.id,cmd);
		fclose(fp);
	}
}



