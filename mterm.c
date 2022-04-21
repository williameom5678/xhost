#include "bbs.h"
#include "chat.h"

log_out()
{
	int mode;
	FILE *fp;
	go_top(0);
	check_bbsrec(1);   //°Ô½ÃÆÇ±â·Ï °­Á¦Á¾·á
	clear_node();
	rm_tmpup();
	if (!bit(allmode,27)) ioctl(0,TCSETA,&termold);   //¿ø·¡ ÅÍ¹Ì³¯ ¸ðµå º¹±¸
	exit(1);
}



set_id_end()	   //ÇØ´ç ¾ÆÀÌµð Á¢¼Ó±â·Ï Á¾°á
{
	int n;
	if (!guest_mode&&read_pf4(pf.id)) { 		//-> pf2¿¡ ÀúÀå
		time(&pf2.logout);						//Á¾°á½Ã°£ ¼¼ÆÃ
		n=how_old(START_TIME);
		pf2.all_log   ++;
		pf2.month_log ++;
		pf2.month_time+=n;
		pf2.all_time  +=n;
		pf2.mymode=set_bit(pf2.mymode,29,1);;;;;;;;;;;;;;;
		if (pf2.type>0) save_pf2(29);
	}
    quit_all_room(1);
	save_login(2);			//Á¾·á±â·Ï
//    disp_room3(4);
	check_bbsrec(1);   //°Ô½ÃÆÇ±â·Ï °­Á¦Á¾·á
	club_mode=No;strcpy(club_go.code,"main");
	check_another_rec();		//µ¿È£È¸À§Ä¡º¯°æ½Ã ±â·Ï
	set_rec(2,23);						//¸ÞÀÎÁ¾·á
	unlink(TMP_FILE);
	unlink(TMP_FILE1);
	unlink(TMP_FILE2);
	unlink(LT_FILE);
}


rawmode()
{
	struct termio tdes;
	if (bit(allmode,27)) return;
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



log_endset()	 //Á¢¼ÓÁ¾·áÃ³¸®
{
	char buff[100];
	time_t t;
	time(&t);
	if (pf.type>0&&pf.type<cfg.relog&&strlen(pf.id)>0) del_stat_id(pf.id);
	else del_stat(tty);  //»óÅÂ±â·Ï È­ÀÏ ÇØ´ç Æ÷Æ®ÀÇ Á¢¼ÓÀ» ÇØÁ¦ÇÑ´Ù.
	sys_tem("rm tmp/up*%d%s -rf",cfg.hostno,no_spc(tty));
	sys_tem("rm tmp/tmp*%d%s -rf",cfg.hostno,no_spc(tty));
	set_id_end();		 //ÇØ´ç ¾ÆÀÌµð Á¢¼Ó±â·Ï Á¾°á
	if (!bit(allmode,27)) ioctl(0,TCSETA,&termold);   //¿ø·¡ ÅÍ¹Ì³¯ ¸ðµå º¹±¸
}

log_rc(int mode)
{
	char buff[100];
	log_endset();	  //Á¢¼ÓÁ¾·áÃ³¸®
	if (!guest_mode&&mode==1) { 	//¾ÆÀÌµðº¯°æ¾ø´Â ÀçÁ¢¼Ó
		sprintf(buff,"tmp/direct.%s",no_spc(tty));
		put_line(pf.id,buff);
	}

	if (bit(allmode2,11)&&is_file("bin/xmain2")) execl("bin/xmain2","xmain2",NULL);
	if (bit(allmode2,12)&&is_file("bin/xmain1")) execl("bin/xmain1","xmain1",NULL);
	if (bit(allmode2,13)&&is_file("bin/xmain" )) execl("bin/xmain","xmain",NULL);
	execl("bin/main","main",NULL);
}

log_exec(int mode,char *buffcmd)
{
	char dir[200];
	char buff[200];
	char buff1[200];
	char buff2[200];
	strcpy(buff,buffcmd);del_space(buff);del_esc3(buff);
	if (strlen(buff)<1) return;
	if (stristr(buff,"/bin/main"));
	else if (stristr(buff,"/bin/bbs"));
	else if (stristr(buff,"/bin/start"));
	else if (stristr(buff,"/bin/")) return;
	else if (stristr(buff,"/sbin/")) return;
	file_dir(buff,dir,buff1);
//	  printf("*%s*%s*%s",buff,buff1,buff2);pressenter();
	sprintf(buff2,"-l%s",pf.id);
	log_endset();	  //Á¢¼ÓÁ¾·áÃ³¸®
	if (stristr(go.wtimes,"ex")&&stristr(go.wtimes,"j")) chdir(dir);
	if (mode==1) execl(buff,buff1,buff2,NULL);
	if (mode==2) execl(buff,buff1,club_go.code,go.code,pf.id,pf.name,NULL);
	if (mode==3) execl(buff,buff1,pf.id,pf.name,NULL);
	if (mode==9) execl(buff,buff1,NULL);
	execl(buffcmd,buff1,NULL);
}


void sig_handler(int signo)
{
	char buff[200];
	char ttyfile[100];
	char *ttyname();
	int n,no_key,time_tty;
	struct stat stbuf;
	struct utimebuf timestamp,*times=&timestamp;
	no_key=(int)pf.offmin;
	if (how_old(START_TIME)>6*10) {
		if (is_file("tmp/stop%d%s.tmp",cfg.hostno,no_spc(tty))) {
			unlink_file("tmp/stop%d%s.tmp",cfg.hostno,no_spc(tty));
			host_end();
		}
	}
	if (no_key<1) no_key=(int)cfg.waitkey;
	if (bit(cfg.mode,28)) return;
	if (no_key<1) no_key=5;
	if (signo==SIGALRM) {
		set_alarm(0);
		sprintf(ttyfile,ttyname(1));
		if (stat(ttyfile,&stbuf)==-1) {
			host_end();
			return;
		}
		time_tty=how_old(stbuf.st_mtime);
		if (bit(allmode2,2)&&time_tty<2*60*60) return;
		times->actime =stbuf.st_atime;
		times->modtime=stbuf.st_mtime;

		disp_timelogo();
		disp_timenews();
		if (bit(cfg.logomode,7)) disp_todaymode();
		if (bit(allmode,13)) no_key=0;
		if (no_key>0&&ascr_count%6==1) {
			if (time_tty>no_key*60) {
				if (bit(allmode,6)) {
					kill(getpid(),SIGKILL);
					return;
				}
				sprintf(g_buff4,"%d",no_key);
				view("frm/system/nokey2.log");
				sleep(1);
				utime(ttyfile,times);
				nostop=Yes;
				allmode=set_bit(allmode,6,1);  //KILLÁß..
				kill(getpid(),SIGHUP);
				set_alarm(1);
				utime(ttyfile,times);
				return;
			}
			if (!bit(allmode,17)&&no_key>0&&how_old(stbuf.st_mtime)>(no_key-1)*60) {
				allmode=set_bit(allmode,17,1);
				view("frm/system/nokey1.log");
			}
		}
		utime(ttyfile,times);
	}
	ascr_count++;
	set_alarm(1);
}



disp_timelogo()
{
	char filename[200];
	if (!bit(allmode,15)||!bit(allmode,16)||bit(pf.mymode,21)) return;
	if (!club_mode&&!bit(cfg.logomode,3)) return;
	if (club_mode&&!bit(ccfg.logomode,3)) return;
	if (!bit(cfg.logomode,4)&&bit(pf.mymode,8)) return;  //setani
	if (newsmode[0]) return;
	sprintf(filename,"%s/logo/%s.ta%d",club_go.dir,go.code,ascr_count%10);
	if (is_file(filename)) {
		printf("[s");fflush(NULL);
		view(filename);
		printf("[u");fflush(NULL);
	}
}


disp_timenews()
{
	FILE *fp;
	char news_logo[300],num_file[300];
	char buff[300],buff1[100],buff2[100],buff3[100],buff4[100];
	int i,i1,i2,i3,n,n1,n2,count,found,sizeof_fnews=(sizeof_bbs+300+80*4);
	BBS oldbbs;

	oldbbs=bbs;

	if (bit(cfg.newsmode,0)||!bit(cfg.newsmode,30)||bit(pf.mymode,22)) return;
	if (bit(bcfg.mode,20)) return;
	if (pf.type<cfg.newstype) return;
	if (newsmode[1]) return;
	if (!club_mode) {
		if (is_same(go.code,"top")&&bit(cfg.newsmode,0)) return;
		if (!is_same(go.code,"top")&&go.type=='m'&&bit(cfg.newsmode,1)) return;
		if (is_bbspds&&bit(cfg.newsmode,2)) return;
	}
	else {
		if (is_same(go.code,"top")&&(bit(ccfg.mode,11)||bit(cfg.newsmode,4))) return;
		if (!is_same(go.code,"top")&&go.type=='m'&&(bit(ccfg.mode,12)||bit(cfg.newsmode,5))) return;
		if (is_bbspds&&(bit(ccfg.mode,13)||bit(cfg.newsmode,6))) return;
	}

	if (news_count>0)  news_count++;
	if (news_count>10) news_count=0;
	if (news_count>0) return;					  //º¯È­¾øÀ½
	if (!set_bbs_logo("nws",news_logo))  goto _cont1;  //È­¸é¾øÀ½


	if (strlen(news_code)<1) strcpy(news_code,cfg.newsfile);
	if (strlen(news_code)<1) goto _cont1;
	//±â·ÏÆÄÀÏÀÌ ¾øÀ¸¸é °»½ÅÇÑ´Ù.
	del_space(news_code);del_esc3(news_code);
	sprintf(num_file,"tmp/news/%s.nws",news_code);
	if (is_old_file(num_file,10*60)||!is_file2(num_file)) {
		sys_tem("bin/ed_bbs makenews '%s'",cfg.newsfile);  //ÆÄÀÏ°»½Å
	}

	for (i=0;i<3;i++) {
		n=count_rec(num_file,sizeof_fnews);
		if (n<1) return;
		n1=n/(rand()%10+1)+1;
		count=rand()%n1;
	//	  printf("%d %d %d",n,n1,n2); pressenter();;;;;;;;;;
		if ((fp=fopen(num_file,"r"))==NULL) goto _cont1;
		fseek(fp,count*sizeof_fnews,SEEK_SET);
		if (!fread(&bbs,sizeof_bbs,1,fp)) bbs.del=Yes;
		else {
			fread(&g_buff1,300,1,fp);
			fread(&g_buff2,80,1,fp);
			fread(&g_buff3,80,1,fp);
			fread(&g_buff4,80,1,fp);
			fread(&g_buff5,80,1,fp);
		}
		fclose(fp);
		if (strlen(bbs.title)>0&&!bbs.del) {
			if (is_file(news_logo)) {
				printf("[s");fflush(NULL);
				view(news_logo);
				printf("[u");fflush(NULL);
			}
			break;
		}
	}

	_cont1:
	bbs=oldbbs;
}


//Á¢¼Ó³¡ Ã³¸®
host_end()
{
	char ttyfile[300];
	struct stat stbuf;
	if (bit(allmode,31)) exit(1);	 //ÀÌ¹Ì Á¾·áÃ³¸®Áß

	if (is_same(tty,"www")) return;
	sprintf(ttyfile,"/dev/%s",tty);
	if (stat(ttyfile,&stbuf)==-1) goto _cont1;

	allmode=set_bit(allmode,31,1);	 //Á¾·áÃ³¸®Áß¼¼ÆÃ
	if (!is_same(auto_cmd,"x")&&bit(cfg.startmode,17)) goto _cont1;
	printf("[01;40r");
	cls();
	if (!bit(cfg.infomode,8)) {
		if (!bit(cfg.logomode,22)) {	  //Á¾·áÈ­¸é¹«½Ã
			view("frm/end/endhost.log");
			view("frm/end/endhost.%d",rand()%100);
			view("frm/end/endhost.bye");
		}
	}

	_cont1:
	set_id_end();		   //ÇØ´ç ¾ÆÀÌµð Á¢¼Ó±â·Ï Á¾°á
	del_stat(tty);			 //»óÅÂ±â·Ï È­ÀÏ ÇØ´ç Æ÷Æ®ÀÇ Á¢¼ÓÀ» ÇØÁ¦ÇÑ´Ù.
	fflush(NULL);
	log_out();
}


endhost_ok()
{
	char buff[200];
	re_print=Yes;
	if (bit(cfg.infomode,8)) host_end();
	if (bit(cfg.mode,6)) host_end();
	else {
		//DNCART ³²¾ÆÀÖÀ½
		sprintf(buff,"tmp/tmpa%d%s",cfg.hostno,no_spc(tty));
		if (is_file(buff)) {
			if (view("frm/down/dncarte.log")) {
				if (yesno2("",Yes)) {
					strcpy(auto_cmd,"dncart");
					return;
				}
			}
			unlink(buff);
		}
		view("frm/end/endok.log");
		view("frm/end/endok.%d",rand()%10);
		view("frm/end/endok.cmd");
		lineinput(buff,40);check_han(buff);printf("\n");
		if (is_same(buff,"y")||strlen(buff)<1) {
			host_end();
		}
		else strcpy(auto_cmd,buff);
	}
}


log_in()
{
	unlink_file("tmp/stop%d%s.tmp",cfg.hostno,no_spc(tty));
	if (bit(allmode,26)) return;
	if (bit(allmode,27)) return;
	ioctl(0,TCGETA, &termold);
	umask(0111);
	ioctl(0, TCGETA, &termnew);
	rawmode();	// ³Í Ä³³ë´ÏÄ® ¸ðµå
	ioctl(0, TCGETA, &termall);
	umask(0111);
	quit_all_room(0);
	printf("[1;50r");
}


set_signal()
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGHUP, (__sighandler_t)host_end);
	signal(SIGSEGV,(__sighandler_t)host_end);
	signal(SIGBUS, (__sighandler_t)host_end);
	signal(SIGCLD,SIG_IGN);
	signal(SIGCHLD,SIG_IGN);
	signal(SIGPIPE,SIG_IGN);
	signal(SIGURG,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTTOU,SIG_IGN);
	signal(SIGPWR,(__sighandler_t)host_end);
	signal(SIGSTOP,SIG_IGN);
	signal(SIGWINCH,SIG_IGN);
	signal(SIGXCPU,SIG_IGN);
	signal(SIGXFSZ,(__sighandler_t)host_end);
	signal(SIGVTALRM,SIG_IGN);
	signal(SIGALRM,SIG_IGN);
	if (bit(cfg.mode,28)==0) set_alarm(1);
}

set_alarm(int mode)
{
	if (mode) {
		alarm(0);
		alarm(cfg.alarm_time);
		signal(SIGALRM,sig_handler);
	}
	else {
		alarm(0);
		signal(SIGALRM,SIG_IGN);
		alarm(0);
	}
}


//Æ¯Á¤Æ÷Æ®¶Ç´Â ID¸¦ kill ÇÔ
kill_portid(int hostno,char *tid,char *tty)
{
	FILE *fp,*fp2;
	char buff[300],buff2[300],filename[100];
	int i,ii,n,pid=getpid(),found=No;
	del_space(tty);
	if (cfg.count_server==1) hostno=cfg.hostno;
	sprintf(filename,"user/moni/moni%d.dat",hostno);
	if ((fp=fopen(filename, "r+"))==NULL) return No;
	ii=-1;
	while (1) {
		ii++;
		fseek(fp,ii*sizeof_moni,SEEK_SET);
		if (!fread(&moni,sizeof_moni,1,fp)) break;
		if (cfg.count_server==1) moni.hostno=cfg.hostno;
		del_space(moni.tty);del_space(moni.id);

		if (moni.hostno!=cfg.hostno) continue;
		if (moni.hostno==cfg.hostno&&moni.pid==pid) continue;
		if (moni.mode<1||strlen(moni.tty)<1) goto _delete;
		if (moni.pid<100) goto _delete;
		if ((strlen(tty)>0&&is_same(moni.tty,tty))||
			(strlen(tid)>0&&is_same(moni.id ,tid))) {
			sprintf(buff,"\n ## »ç¿ëÀÚÀÇ ¿ä±¸·Î º»Á¢¼ÓÀº ÇØÁ¦µË´Ï´Ù.##\n");
			to_tty(moni.hostno,moni.tty,buff);
			if (cfg.count_server==1||moni.hostno==cfg.hostno) {
				kill(moni.pid,SIGHUP);
			}
			else {
				sprintf(buff,"KK%d",moni.pid);
				svr_send(moni.hostno,buff,"killtty");
			}
			found=Yes;
			sys_tem("touch tmp/stop%d%s.tmp",moni.hostno,no_spc(moni.tty));
			_delete:
			fseek(fp,ii*sizeof_moni,SEEK_SET);
			moni.mode=0;strcpy(moni.tty,"");strcpy(moni.id,"");
			fwrite(&moni,sizeof_moni,1,fp);
		}
	}
	fclose(fp);
	return found;
}


//mode==0 : ·Î±×ÀÎ 1:Á¢¼Ó
save_login(int mode)
{
	char filename[300];
	char buff[300],buff2[300];
	if (mode==1) sprintf(buff,"STA %4d %s",0,hostname);
	else		 sprintf(buff,"END %4d ...",how_old(START_TIME)/60);
	sprintf(filename,"%s/time%s.txt",REC_DIR,now_str(12));
	sprintf(buff2,"%s %3d %-8s %s",now_str(3),cfg.hostno,pf.id,buff);
	put_line(buff2,filename);
}


