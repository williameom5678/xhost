///////////////////////////////////////////////////////////////////////////
//∞≥πﬂ : ±Ëº∫¥Î  xhost@xhost.co.kr (016-320-7882)
// √§∆√ µ∂∏≥«¡∑Œ±◊∑•
///////////////////////////////////////////////////////////////////////////
//√ﬂ∞°±‚¥…
// «œ¥‹ ¡Ÿº±≈√,ªˆ±Ú∏∂¿Ω¥Î∑Œ º±≈√∞°¥…
//roomtype=1~38:¿œπ›  39 µø»£»∏
//√—∫–∑˘∞°¥…:30∞≥∫–∑˘
//∞¢∫–∑˘∫∞ ¥Î»≠Ω« ∞≥º≥ºˆ:300
//∞¢¥Î»≠Ω« »∏ø¯√÷¥Î 	:20

#include "bbs.h"
#include "chat.h"
#include <dirent.h>    //µ∑∫≈‰∏Æ »≠¿œ∏Ì ∫Ø»Ø«‘ºˆ∂ßπÆø° « ø‰
char tosay2[12];
do_chat(int mode)
{
	FILE *fp;
	char buff[280];
	char buff2[280];
	int n,nroom=0;
	int pid;
	re_print=Yes;

	check_dir("user/data/");
	check_dir("user/chat/");

	strcpy(tosay,"");strcpy(tosay2,"");
	strcpy(userid,pf.id);
	strcpy(userna,pf.name);
	strcpy(userni,pf.nick);
	check_same();

	//±‚¡∏πÊ ªË¡¶ (∞∞¿∫ tty,∞∞¿∫ »£Ω∫∆Æπ¯»£)
	quit_all_room(0);
	if (bit(limit_mode,5)) {
		if (!view("frm/pf/notchat.log")) mess("¥Î»≠Ω«±›¡ˆ");
		return;
	}
	if (pf.type<18&&view("frm/down/noinchat.log")) return;

	roomnum=roomtype=0;
	if (mode==2&&club_mode&&!is_same(club_go.code,"main")) {
		if (!is_same(go.code,"cchat")) find_go("cchat");
		if (!club_mode||!is_same(go.code,"cchat")) {mess("µø»£»∏ ¥Î»≠πÊ¿ª ªÁøÎ«“ºˆ æ¯Ω¿¥œ¥Ÿ.");return;}
		roomtype=39;  //µø»£»∏
	}
	else if (stristr(go.code,"chat")) {
		n=atoi(go.wtimes);
		if (n>0&&n<49) roomtype=n;
		else if (n>500) {		//πŸ∑Œ¡¯¿‘¥Î»≠πÊ
			roomtype=49;
			roomnum=n;
		}
	}
	if (is_same(go.wtimes,"3bch")) {sys_tem("bin/stelnet %s",go.dir);return;}
	if (bit(cfg.chatmode,20)) {sys_tem("bin/stelnet www.3b.co.kr");return;}
	if (bit(cfg.chatmode,21)) {sys_tem("bin/stelnet chat.xhost.co.kr");return;}

	if (pf.type<cfg.chattype) {
		if (!view("frm/error/chatoff.log")) mess("¥Î»≠Ω«¡¯¿‘±›¡ˆ");
		return;
	}
	tag_monichar(); //«ˆ¿Á ±‚∑œ
	if (ss_check(8)) return;
	if (check_warn(2)) return;	//¥Î»≠πÊ ±›¡ˆ
	set_stat(3,"");             //«ˆ¿Á ªÛ≈¬∏¶ ¥Î»≠πÊø° ¿÷¿Ω¿ª æÀ∏≤
	time(&CHECK_TIME);			//¡¢º”Ω√¿€Ω√∞£
	set_rec(1,3);
	check_bbsrec(1);   //∞‘Ω√∆«±‚∑œ ∞≠¡¶¡æ∑·

	now_st=1;my_color1=my_color2=99;quit_chat=No;bye_chat=No;cont_chat=No;
	my_mode=0;room_sec=0;			//»≠∏È¿Ã ∞ªΩ≈µ»¥Ÿ.
	chaty1=4;chaty2=21;
	if (cfg.chatst==0) cfg.chatst=1;
	printf("[1;45r[20;1H\n\n\n\n");
	if (roomtype<1||roomtype>50) roomtype=1;
	if (roomnum<1) roomnum=1;
	set_signal();	 //Ω√±◊≥Ø √≥∏Æ
	do_xchat();
	printf("[1;45r[20;1H\n\n\n\n");
	set_stat(1,"¥Î»≠πÊ");             // ∫∏≈ÎªÛ≈¬∑Œ »Øø¯
	set_rec(2,3);
	re_print=Yes;
	roomnum=0;
	if (bye_chat) host_end(0);

	pf.todaymode=set_bit(pf.todaymode,6,1);   //ø¿¥√¿€æ˜
}



do_xchat()
{
	FILE *fp1;
	char buff[180];
	char buff1[180];
	int i,n,n1,mode,key;

	taja_on=quiz_on=No;
	xrace_on=quiz_on=No;
	key=Yes;
	st_mode=cfg.chatst; 		   //¥Î»≠Ω«Ω√¿€∏µÂ
	my_mode=0;
	//¡¯¿‘»≠∏È√‚∑¬
	if (!view("frm/chat/chat%d.log",roomtype))
		 view("frm/chat/chat.log");

		 if (roomtype==39) {join_clubroom();  return;} //µø»£»∏
	else if (roomtype==49) {join_directroom();return;} //πŸ∑Œ∞°±‚

	cont_chat=Yes;
	//πÊ¡¯¿‘
	st_mode=cfg.chatst; 					//¥Î»≠Ω«Ω√¿€∏µÂ
	while(1) {
		roomnum=0;
		add_croom();						//¥Î±‚Ω«ø° ¿˙¿Â
		printf("[1;50r[1;1H");
		cls();
		quit_chat=bye_chat=No;
		roomnum=select_room();				//πÊº±≈√
		if (!cont_chat||bye_chat||quit_chat) break;
		strcpy(auto_cmd,"");
		if (roomnum==-1)   {				//Ω≈±‘πÊ∏∏µÈ±‚
			roomnum=0;
			make_quit_croom(0); 			//¥Î±‚πÊ ªË¡¶
			join_newroom(); 				//πÊªı∑Œ ∏∏µÈ±‚
			st_mode=cfg.chatst;
		}
		else if (roomnum>0) {  //±‚¡∏πÊ¡¢º”
			int old_roomnum=roomnum;
			roomnum=0;
			make_quit_croom(0); 			//¥Î±‚πÊ ªË¡¶
			roomnum=old_roomnum;
			join_croom();
			st_mode=cfg.chatst;
		}
		if (!cont_chat||bye_chat||quit_chat) break;
	}
	roomnum=0;
	make_quit_croom(0); 			//¥Î±‚πÊ ªË¡¶
}


//µø»£»∏ ¡¯¿‘  πÊπ¯»£ 3000~4000π¯±Ó¡ˆ «“¥Á«—¥Ÿ.
int join_clubroom()
{
	FILE *fp;
	int key,i,found=No;
	int myroom=No,newroom=No;
	char buff[180],buff2[100];

	if (ccfg.chatnum<3000||ccfg.chatnum>5000) ccfg.chatnum=3000;
	roomnum=ccfg.chatnum;
	if (!read_croom()) {
		strcpy(croom.club,club_go.code);
		sprintf(croom.title,"%s µø»£»∏¥Î»≠Ω«",club_go.code);
	}
	read_user_room();
	croom.roomtype=roomtype;
	croom.roomnum=roomnum;
	roomtype_limit();
	if (!check_join_ok()) return No;	 //¡¯¿‘∞°¥…»Æ¿Œ
	check_same();		//∞∞¿∫ ID∞° ¿÷¥¬∞°?
	add_croom();
	save_croom();
	check_newboss();
	allmode=set_bit(allmode,4,1);		//∞®Ω√¡¶ø‹
	chatstart();						//¥Î»≠Ω√¿€
	chat_title(1);
	allmode=set_bit(allmode,4,0);		//∞®Ω√¡¶ø‹
	return Yes;
}


select_room()
{
	int i,n,n2=0;
	char buff[200];
	strcpy(buff,"");
	n=disp_room1();
	if (n<0) {cont_chat=No;return 0;}
	if (n>0) {
		strcpy(auto_cmd,"");
		sprintf(buff,"%d",n);
		check_cmd(buff);
	}
	else if (strlen(auto_cmd)>0) {
		strcpy(buff,auto_cmd);
		strcpy(auto_cmd,"");
		check_cmd(buff);
	}
	else {
		_cont1:
		printf(" ¥Î»≠¬¸ø©(π¯»£) ∞≥º≥(O) ∏Ò∑œ(LS,ST) µµøÚ∏ª(H) >>");
		lineinput(buff,30);printf("\n");check_han(buff);
		check_cmd(buff);
	}
	n=mmn[0];n2=mmn[1];if (n2<1) n2=atoi(&buff[1]);
	if (n>3000&&n<5000) {
		mess("µø»£»∏ø°º≠ ¡¯¿‘«œººø‰(µø»£»∏¿¸øÎ¥Î»≠πÊ)");
		return 0;
	}
	else if (n>0) return n;
	else if (is_same(mmc[0],"h")) {
		if (!view("frm/chat/chat%s.hlp",mmc[1]))
			view("frm/chat/chat.hlp");
		goto _cont1;
	}
	else if (strlen(mmc[0])==1&&is_char("QqPpmMxX",**mmc)) cont_chat=No;
	else if (strlen(mmc[0])==1&&is_char("nNOo0",**mmc)) return -1;    //ªı∑Œ∏∏µÈ±‚
	else if (is_quit(mmc[0])) cont_chat=No;
	else if (strlen(mmc[0])==1&&is_char("IiJj",**mmc)&&n2<1) {printf("\n ¥Î»≠Ω« π¯»£ :");return (numinput());}
	else if (strlen(mmc[0])==1&&is_char("IiJj",**mmc)      ) return n2;
	else if (is_same(mmc[0],"/x")||is_same(mmc[0],"xx")||is_same(mmc[0],"bye")) bye_chat=Yes;
	else if (is_same(mmc[0],"/p")||is_same(mmc[0],"/q"))  cont_chat=No;
	else if (is_same(mmc[0],"wait")||is_same(mmc[1],"wait")||is_same(mmc[0],"/wait")||is_same(mmc[1],"/wait")) st_mode=2;
	else if (is_same(mmc[0],"cl"  )||is_same(mmc[1],"cl"  )||is_same(mmc[0],"/clo" )||is_same(mmc[1],"/cl" ))  st_mode=7;
	else if (is_same(mmc[0],"close")||is_same(mmc[1],"close")||is_same(mmc[0],"/clo" )||is_same(mmc[1],"/close")) st_mode=7;
	else if (is_same(mmc[0],"op")||is_same(mmc[1],"op")||is_same(mmc[0],"/op" )||is_same(mmc[1],"/op"))           st_mode=8;
	else if (is_same(mmc[0],"open")||is_same(mmc[1],"open")||is_same(mmc[0],"/open" )||is_same(mmc[1],"/open"))   st_mode=8;
	else if (is_same(mmc[0],"lt"  )||is_same(mmc[1],"lt"  )||is_same(mmc[0],"/lt"  )||is_same(mmc[1],"/lt"  ))    st_mode=4;
	else if (is_same(mmc[0],"all" )||is_same(mmc[1],"all" )||is_same(mmc[0],"/all" )||is_same(mmc[1],"/all" ))    st_mode=6;
	else if (is_same(mmc[0],"all9" )||is_same(mmc[1],"all9" )||is_same(mmc[0],"/all9" )||is_same(mmc[1],"/all9" )) st_mode=9;
	else if (is_same(mmc[0],"st"  )||is_same(mmc[1],"st"  )||is_same(mmc[0],"/st"  )||is_same(mmc[1],"/st"  ))    st_mode=2;
	else if (is_same(mmc[0],"ls")||is_same(mmc[0],"/ls"))   st_mode=1;
	return 0;
}


//±‚¡∏πÊø° ¡¯¿‘
int join_croom()
{
	FILE *fp;
	int key,i,found=No;
	int myroom=No,newroom=No;
	char buff[180],buff2[100];


	if(roomnum<1) return No;
	if (!read_croom()||croom.count<1||croom.roomtype!=roomtype) {
		mess(" %d π¯¿∫ ∞≥º≥µ«æÓ ¿÷¡ˆ æ ¿∫ ¥Î»≠Ω«¿‘¥œ¥Ÿ.",roomnum);
		cont_chat=Yes;
		return No;
	}
	read_user_room();
	roomtype_limit();
	if (!check_join_ok()) return No;	 //¡¯¿‘∞°¥…»Æ¿Œ
	if (strlen(croom.passwd)>0) {
		printf("\n %dπ¯ %s¥‘ ∞≥º≥ (%s)\n",roomnum,croom.id,croom.title);
		printf(" ¥Î»≠Ω« ∫Òπ–π¯»£¥¬?>>");
		passinput(buff,8);
		printf("\n");
		if (!is_same(croom.passwd,buff)) {
			mess("**∫Òπ–π¯»£∞° ∆≤∏≥¥œ¥Ÿ. **");
			return No;
		}
	}
	check_same();		//∞∞¿∫ ID∞° ¿÷¥¬∞°?
	add_croom();
	save_croom();
	check_newboss();

	allmode=set_bit(allmode,4,1);		//∞®Ω√¡¶ø‹
	chatstart();		 //¥Î»≠Ω√¿€
	chat_title(1);
	allmode=set_bit(allmode,4,0);		//∞®Ω√¡¶ø‹
	return Yes;
}



//mode=1 : æ¯¿∏∏È πÊ∏∏µÎ 2:¡¯¿‘
int join_newroom()
{
	FILE *fp;
	int key,i,found=No;
	int myroom=No,newroom=No;
	char buff[180],buff2[100];
	CROOM tcroom;
	newroom=Yes;
	myroom=Yes;
	croom.openmode=0;
	croom.itype=0;
	croom.sex=0;
	croom.max=20;
	croom.age1=0;
	croom.age2=0;
	croom.dm=0;
	strcpy(croom.club,"");
	croom.roomtype=roomtype;
	strcpy(croom.passwd,"");
	xstrncpy(croom.id,userid,12);
	xstrncpy(croom.name,userna,12);
	xstrncpy(croom.hostname,hostname,20);
	time(&croom.date);
	roomtype_limit();
	if (!check_join_ok()) return;	  //¡¯¿‘∞°¥…»Æ¿Œ
	//πÊ¡¶∏Ò
	printf(" ¥Î»≠πÊ¿« ¡÷¡¶∏¶ ¿‘∑¬«œΩ Ω√ø¿.\n");
	printf(" ¡¶∏Ò>>");
	lineinput(buff,80);
	if (is_quit(buff)) return;
	strcpy(croom.title,buff);
	if (strlen(croom.title)<2) {
		sprintf(croom.title,"%s ¥Î»≠Ω«",cfg.hostname);
		printf(croom.title);
	}
	printf("\n");
	printf(" ¥Î»≠πÊ »Øøµ ∏ﬁΩ√¡ˆ∏¶ ¿‘∑¬«œΩ Ω√ø¿.\n");
	printf(" ≥ªøÎ>>");
	lineinput(buff,60);
	if (is_quit(buff)) return;
	strcpy(croom.stitle,buff);
	printf("\n");

	//πÊ¡æ∑˘
	printf(" ¥Î»≠Ω« ¡æ∑˘∏¶ º±≈√«œø©¡÷ººø‰...\n");
	printf(" 1.∞¯∞≥¥Î»≠Ω«  2.∫Ò∞¯∞≥\n");
	printf(" º±≈√ >> ");
	lineinput2(buff,2);check_han(buff);printf("\n");
	if (is_quit(buff)) return;
	croom.openmode=atoi(buff);
	if (croom.openmode==2) {
		lineinput3(" ¥Î»≠Ω« ¡¯¿‘∫Òπ–π¯»£:(2¿⁄¿ÃªÛ)>>",croom.passwd,4);
		if (strlen(croom.passwd)<2) {
			strcpy(croom.passwd,"");
			croom.openmode=1;
		}
	}
	//√÷¥Î¿Œø¯
	printf(" ¥Î»≠ «„øÎ ¿Œø¯¿∫ (2-20∏Ì, Enter:6∏Ì) >>");
	lineinput2(buff,3);
	if (atoi(buff)>1) croom.max=atoi(buff);
	else			  croom.max=6;
	if (croom.max>20) croom.max=20;
	if (croom.max<2 ) croom.max=2 ;

	view("frm/chat/copen2.log");
	time(&croom.date);
	croom.roomtype=roomtype;
	xstrncpy(croom.id,userid,20);
	xstrncpy(croom.name,userni,20);

	//∫ÛπÊ√£±‚	1π¯∫Œ≈Õ 2000π¯¿ª ±Ó¡ˆ¡ﬂø° √£¥¬¥Ÿ.
	_cont1:
	found=No;
	roomnum=0;
	for (i=1;i<2000&&!found;i++) {
		sprintf(ROOM_FILE,"user/data/croom.%d",i);
		if (!is_file(ROOM_FILE)) {
			found=Yes;
			roomnum=i;
		}
	}

	if (!found) {
		//2000∞≥∞° ≤À¬˘∞ÊøÏ ƒ´øÓ≈Õ∏¶ »Æ¿Œ«—¥Ÿ. ->¥ŸΩ√ ∞Àªˆ
		for (i=1;i<2000;i++) {
			sprintf(ROOM_FILE,"user/data/croom.%d",i);
			if ((fp=fopen(ROOM_FILE,"r"))==NULL) {found=Yes;break;}
			if (!fread(&tcroom,sizeof_croom,1,fp)) tcroom.count=0;
			if (how_old(tcroom.date)>36*60*60) tcroom.count=0;	//36Ω√∞£¿Ã¿¸∞Õµµ ªË¡¶
			fclose(fp);
			if (tcroom.count<1) del_room_file(i);
		}
		goto _cont1;
	}

	for (i=0;i<CMAX;i++) bzero(&user_room[i],sizeof_room);

	croom.roomnum=roomnum;
	croom.roomtype=roomtype;
	roomtype_limit();
	if (roomnum==0||!found) return;

	del_room_file(roomnum);
	add_croom();
	check_mynum2(); 					//≥™¿« ¥Î»≠πÊπ¯»£ √£±‚

	croom.roomnum=roomnum;
	croom.roomtype=roomtype;
	check_mynum2(); 					//≥™¿« ¥Î»≠πÊπ¯»£ √£±‚
	unlink_file("user/chat/wchat.%d.%d",roomnum,mynum);

	chatstart();		 //¥Î»≠Ω√¿€
	chat_title(1);
}

int join_directroom()
{
	char buff[180],buff2[100];

	if (roomnum<500) roomnum=500;

	read_croom();
	read_user_room();
	croom.roomtype=roomtype;
	croom.roomnum=roomnum;
	roomtype_limit();
	if (!check_join_ok()) return No;	 //¡¯¿‘∞°¥…»Æ¿Œ
	check_same();		//∞∞¿∫ ID∞° ¿÷¥¬∞°?
	add_croom();
	save_croom();
	check_newboss();
	chatstart();		 //¥Î»≠Ω√¿€
	chat_title(1);
	return Yes;
}



chatstart()
{
	FILE *fp,*fp2;
	int i,j,key;
	int ps;
	char ch;
	char s[200];
	char buff[200];
	char chatbuff[500],oldchat1[500],oldchat2[500],filename[100];
	char quit_chat=No;
	time_t taja_time;
	struct tm *tp;
	int icount,now_roomnum=roomnum;
	int fail_open=No;
	int old_count;

	read_user_room();
	check_mynum();
	unlink_file("user/chat/wchat.%d.%d",roomnum,mynum);
	strcpy(oldchat1,"");strcpy(oldchat2,"");

	time(&room.ctime);
	time(&user_room[mynum-1].ctime);
	for (i=0;i<CMAX;i++) {
		if (how_old(user_room[i].ctime)>10*60) strcpy(user_room[i].id,"");
	}
	save_user_room(mynum);;  //«ÿ¥Áπ¯»£¿« user_room ¿˙¿Â
	save_croom();

	cls();
	//¿ßƒ°¡¶æÓ
	chat_title(1);
	printf("[%d;%dr[%d;1H",chaty1,chaty2,chaty1+3);
	view("frm/chat/chatin.mes");
	sprintf(buff,"<u> %s </u>¥‘¿Ã ¿‘¿Â«œø¥Ω¿¥œ¥Ÿ.",room.nick);
	chat_line(92,buff);
	printf("[%d;1H",chaty2);fflush(NULL);
	chat_line(90,"<script>update();</script>");
	if (strlen(croom.stitle)>0) chat_line(98,croom.stitle);

	key=Yes;

	strcpy(filename,"");
	now_roomnum=roomnum;
	old_count=croom.count;
	while(!quit_chat&&!bye_chat) {
		fail_open=No;
		roomnum=now_roomnum;
		sprintf(ROOM_FILE,"user/data/croom.%d",roomnum);
		key=Yes;
		printf("[%d;1H[K",chaty2+3);
//		  printf("\n %d %s %d %d\n",roomnum,ROOM_FILE,file_time(ROOM_FILE),file_size(ROOM_FILE));
		if (strlen(tosay)>1) printf("[%d;1H %s >>[K",chaty2+2,tosay);
		else if (strlen(tosay2)>1) printf("[%d;1H [%s] >>[K",chaty2+2,tosay2);
		else printf("[%d;1H¥Î»≠ >>[K",chaty2+2);
		lineinput(chatbuff,200);check_han(chatbuff);

		if (!read_croom()) {
			msleep(1);
			if (!read_croom()) {fail_open=Yes;}
			chat_title(1);
		}
		read_user_room();
		if (old_count!=croom.count) {
			old_count=croom.count;
			chat_title(0);
		}
		check_mynum();
		//¿ÃªÛø©∫Œ»Æ¿Œ
		if (croom.count>0&&mynum==0) add_croom();

		if (chatbuff[0]=='\\'&&chatbuff[1]=='/') {chatbuff[0]=' ';all_trim(chatbuff);}
		if (strlen(chatbuff)<1) {
			if (strlen(tosay)>0) strcpy(tosay,"");
			if (strlen(tosay2)>0) strcpy(tosay2,"");
			if (roomtype!=7&&!view("frm/chat/chatad.%d",rand()%50))
				view("frm/chat/chatad.0");
			continue;
		}
		if (mynum>0) {
			time(&room.ctime);
			time(&user_room[mynum-1].ctime);
			save_user_room(mynum);;  //«ÿ¥Áπ¯»£¿« user_room ¿˙¿Â
		}
		else fail_open=Yes;

		if(chatbuff[0]=='/') {
			check_cmd(chatbuff);
			strcpy(buff,mmc[0]);
			buff[0]=' ';del_space(buff);check_han(buff);
			strcpy(mmc[0],"/");strcat(mmc[0],buff);
			key=No;
			if		(is_same(mmc[0],"//º”∫∏" )) key=Yes;
			else if (is_same(mmc[0],"/p" )) {quit_chat=Yes;break;}
			else if (is_same(mmc[0],"/q" )) {quit_chat=Yes;break;}
			else if (is_same(mmc[0],"/x" )) {quit_chat=Yes;bye_chat=Yes;break;}
			else if (is_same(mmc[0],"/n")||is_same(mmc[0],"/ni")) nick_name(userid,mmc[1]);
			else if (is_same(mmc[0],"/nn")||is_same(mmc[0],"/al")) nick_name(userid,mmc[1]);
			else if (is_same(mmc[0],"/h" ))  {
				printf("[%d;1H\n",chaty2);
				if (!view("frm/chat/chat%s.hlp",mmc[1]))
					view("frm/chat/chat.hlp");
			}
			else if (is_same(mmc[0],"/sys")||is_same(mmc[0],"/πÊ"))  {
				if (is_same(croom.id,room.id)&&
					is_same(croom.name,room.nick)&&
					is_same(croom.hostname,room.hostname)) room_sys();
				else if (pf.type>16) room_sys();
				else chat_line(93,"πÊ¿Â∏∏ ªÁøÎ∞°¥…«’¥œ¥Ÿ.");
			}
			else if (is_same(mmc[0],"/st")) disp_room2(mmc[1]);
			else if (is_same(mmc[0],"/c"))  {cls();chat_title(0);}
			else if (is_same(mmc[0],"/cls")) {cls();chat_title(0);}
			else if (is_same(mmc[0],"/in")||is_same(mmc[0],"/i")) invite_man(mmc[1],chatbuff);
			else if (is_same(mmc[0],"/ear")) to_say2(mmc[1]);
			else if (is_same(mmc[0],"/say")) to_say2(mmc[1]);
			else if (is_same(mmc[0],"/to"))  to_say2(mmc[1]);
			else if (is_same(mmc[0],"/us"))  show_view_user(1,mmc[1]);
			else if (is_same(mmc[0],"/st")) disp_room2(mmc[1]);
			else if (is_same(mmc[0],"/me")) {printf("[s[%d;1H\n",chaty2);to_memo(2,"","");printf("[u");}
			else if (is_same(mmc[0],"/msg")) {printf("[s[%d;1H\n",chaty2);set_msg(2,mmc[1]);printf("[u");}
			else if (is_same(mmc[0],"/pf"))  view_chat_pf(1,mmc[1]);
			else if (is_same(mmc[0],"/pfs"))  {if (pf.type>=18) view_chat_pf(2,mmc[1]);}
			else if (is_same(mmc[0],"/now")) set_mymoni(chatbuff);
			else if (is_same(mmc[0],"/m" ))  to_memo(2,mmc[1],"");
			else if (is_same(mmc[0],"/mm" )) to_memo(2,mmc[1],"");
			else key=Yes;
			if (strlen(mmc[0])>1) printf("[%d;%dr[%d;1H",chaty1,chaty2,chaty2-2);
		}
		if (key&&strlen(chatbuff)>0) {
			xstrncpy(buff,chatbuff,4);
			chat_line(0,chatbuff);
		}
	}
	sprintf(buff,"<u> %s </u>¥‘¿Ã ≈¿Â«œø¥Ω¿¥œ¥Ÿ.",room.nick);
	read_croom();
	read_user_room();
	check_mynum();
	del_user_room();
	save_croom();
	save_user_room(0);
	chat_title(1);
	chat_line(92,buff);
	check_mynum();		 //≥™¿« ¥Î»≠πÊπ¯»£ √£±‚
	check_newboss();
	chat_line(90,"<script>update();</script>");
	unlink_file("user/chat/wchat.%d.%d",roomnum,mynum);
	if (croom.count==0) {
		unlink_file("user/data/croom.%d",roomnum);
		unlink_file("user/chat/wchat.%d",roomnum);
	}
}



//¥Î»≠Ω« ªÛ¥‹ ¡¶∏Ò «•Ω√
chat_title(int mode)
{
	char buff[3000];
	char buff1[3000];
	char buff2[10];
	char buff3[10];
	char top_line[300];
	char filename[100];
	int i;
	if(xrace_on) return;

	if (croom.count<1) {
		read_croom();
		read_user_room();
	}
	//ªÛ¥‹ 3π¯¬∞ ¡Ÿ ¿–±‚
	sprintf(g_buff1,"%d",roomnum);
	strcpy(g_buff2,strlen(croom.passwd)<3?"":"[[7m∫Ò∞¯∞≥[0;1m]");
	sprintf(filename,"frm/chat/chat%d.top",roomtype%100);
	if (file2buff(buff,filename,3000)<0) {
		strcpy(filename,"frm/chat/chat1.top");
		file2buff(buff,filename,3000);
	}
	sprintf(buff1,"%d",croom.count);
	sprintf(buff2,"%d",croom.max);
	strchng2(buff,"!¥Î»≠!1",croom.title);
	strchng2(buff,"!¥Î»≠!2",croom.id);
	strchng2(buff,"!¥Î»≠!3",croom.name);
	strchng2(buff,"!¥Î»≠!4",buff1);
	strchng2(buff,"!¥Î»≠!5",buff2);
	strchng2(buff,"!»£Ω∫!",cfg.hostname);
	strchng2(buff,"!±§ø™!1",g_buff1);
	strchng2(buff,"!±§ø™!2",g_buff2);
	strcpy(buff1,"[s");
	strcat(buff1,buff);
	if (strlen(croom.stitle)>3) {
		strcat(buff1,"[3;1H");
		strcat(buff1,croom.stitle);
	}
	strcat(buff1,"[u");
	if (mode==0) view3(buff1);
	else {
		for (i=0;i<CMAX;i++) {
			to_tty2(user_room[i].hostno,user_room[i].tty,buff1);
		}
	}
}

int compr_num(int *x,int *y) {
  if (*x>*y) return 1;
  if (*x<*y) return -1;
  return 0;
}



//∞¢πÊ¿« ªÛ≈¬∏¶ ∫∏ø©¡ÿ¥Ÿ.
disp_room1()
{
	FILE *fp,*fp2;
	char buff[4100],buff2[300];
	int i,i1,ii,i2,n,ret=0,count=0;
	int old_roomnum=roomnum;
	int old_roomtype=roomtype;
	DIR *dp;
	struct dirent *dfp;
	char st_file[300];
	CROOM oldcroom;
	ROOM troom,aroom[20];
	int r_num[1000];

	for (i=0;i<1000;i++) r_num[i]=999999;

	if (st_mode<1) st_mode=1;

	sprintf(st_file,"tmp/tmpc%d%s",cfg.hostno,no_spc(tty));   //»£Ω∫∆Æπ¯»£∫∞¿”Ω√»≠¿œ
	oldcroom=croom;
	//st ∆ƒ¿œ ∏∏µÈ±‚
	if ((fp=fopen(st_file,"w+"))==NULL) return ret;
	if (st_mode==2||st_mode==6||st_mode==11) {
		sprintf(buff2,"frm/chat/stwait0.log");
		file2buff(buff,buff2,0);
		fprintf(fp,buff);
		//¥Î±‚Ω«
		i2=0;
		strcpy(buff,"");
		if ((fp2=fopen("user/data/croom.0","r"))!=NULL) {
			while(strlen(buff)<4000&&fread(&troom,sizeof_room,1,fp2)) {
				if (strlen(troom.id)>1) {
					sprintf(buff2,"%s(%s)      ",troom.nick,troom.id);
					buff2[19]=0;
					strcat(buff,buff2);
					if (i2%4==3) strcat(buff,"\n");
					i2++;
				}
			}
			fclose(fp2);
			if (i2>0) {
				if (i2%4!=0) strcat(buff,"\n");
				fprintf(fp,"%s",buff);
			}
		}
	}
	sprintf(buff2,"frm/chat/stwait%d.log",st_mode);
	file2buff(buff,buff2,0);
	fprintf(fp,buff);
	if (st_mode==11) goto _cont2;		//¥Î±‚Ω«∏∏ ∫∏±‚


	//µ∑∫≈‰∏Æø°º≠ croom.*¿ª ¿–æÓ≥Ω¥Ÿ.
	ii=i1=0;
	dp= opendir("user/data/");
	while(ii<1000&&(dfp=readdir(dp))!= NULL) {
		if(stristr(dfp->d_name,"croom.")) {
			strcpy(buff,&dfp->d_name[6]);
			i1=atoi(buff);
			if (i1<1) continue;
			if (cmtype<=1) r_num[ii++]=i1;
			else if (cmtype<=3&&i1<10000) r_num[ii++]=i1;
			else if (cmtype>=4&&i1>=10000) r_num[ii++]=i1;
		}
	}
	closedir(dp);

	//qsort∑Œ r_num ¿ª ¡§∑ƒ
	qsort(r_num,sizeof(r_num)/sizeof(int),sizeof(int),compr_num);

	count=0;
	roomnum=old_roomnum;
	for (n=0;n<ii;n++) {
		roomnum=r_num[n];
		if (roomnum<1) continue;
		// printf("%d %d %d<BR>",n,ii,roomnum);
		if (roomnum>999998) continue;
		read_croom();
		read_user_room();
		if (cmtype==5&&bit(croom.openmode,1)) continue;
		if (cmtype==6&&bit(croom.openmode,1)) continue;
		if (cmtype==7&&bit(croom.openmode,1)) continue;
		if (cmtype==8&&bit(croom.openmode,1)) continue;
		if (cmtype==9&&!bit(croom.openmode,1)) continue;
		// if (strlen(subkey)>0&&strlen(subkey)>0&&croom.roomtype!=atoi(subkey)) continue;
		count_croom();
		if (croom.count<1) continue;
		count++;
		a_num[96]+=croom.count;  //√—¿Œø¯ºˆ
		a_num[98]++;  //√—∞≥º≥ºˆ

		if (st_mode!=6&&croom.roomtype!=roomtype) continue;
		open_str(croom,buff,roomnum);
		if (st_mode==6) sprintf(buff2,"#%2d-%3d",croom.roomtype,croom.roomnum);
		else			sprintf(buff2,"#%3d",croom.roomnum);
		fprintf(fp,"%s: %-7s (%2d/%2d) [%-8s] %s\n",
				 buff2,buff,croom.count,croom.max,croom.id,croom.title);
		if (st_mode!=1) {
			for (i2=0;i2<20;i2++) aroom[i2]=user_room[i2];
			qsort(aroom,20, sizeof_room,comp_9);
			strcpy(buff,"");
			ii=0;
			for (i2=0;i2<20&&strlen(buff)<270;i2++) {
				if (strlen(aroom[i2].id)>1) {
					if (ii%3==0) strcat(buff,"               ");
					sprintf(buff2,"%s(%s)      ",aroom[i2].nick,aroom[i2].id);
					buff2[21]=0;
					strcat(buff,buff2);
					if (ii%3==2) strcat(buff,"\n");
					ii++;
				}
			}
			if (ii%3!=0) strcat(buff,"\n");
			fprintf(fp,"%s",buff);
		}
	}

	_cont2:
	fclose(fp);
	roomtype=old_roomtype;
	roomnum =old_roomnum ;
	croom=oldcroom;

	if (st_mode<1) st_mode=1;

	strcpy(auto_cmd,"");
	if (!view("frm/chat/st-%d.scr",roomtype))
		view("frm/chat/st.scr");
	if ((fp=fopen(st_file,"r"))!=NULL) {
		while (fgets(buff,2000,fp)!=NULL) {
			printf("%s",buff);
			if (count++%20==19) {
				if (!view("frm/chat/st.cmd"))
					printf(" ¥Î»≠¬¸ø©(π¯»£) ∞≥º≥(o) ∞Ëº”(Y/n) > ");
				lineinput(buff,10);
				printf("[80D[2K");
				ret=atoi(buff);
				if (is_quit(buff)) {ret=-1;break;}
				if (ret>0) break;
				if (strlen(buff)>0) {
					strcpy(auto_cmd,buff);
					break;
				}
			}
		}
		fclose(fp);
	}
	return ret;
}


check_st_mode(char *cmdbuff)
{
	if (strlen(cmdbuff)==0) st_mode=cfg.chatst;
	if (is_same(cmdbuff,"ls")) st_mode=1;
	if (is_same(cmdbuff,"title")) st_mode=1;
	if (is_same(cmdbuff,"wait")) st_mode=11;
	if (is_same(cmdbuff,"all" )) st_mode=6;
	if (is_same(cmdbuff,"cl"  )) st_mode=7;
	if (is_same(cmdbuff,"close")) st_mode=7;
	if (is_same(cmdbuff,"op"))   st_mode=8;
	if (is_same(cmdbuff,"open")) st_mode=8;
}

disp_room2(char *cmdbuff)
{
	int old_st_mode=st_mode;
	if (strlen(cmdbuff)<1) st_mode=10;
	else check_st_mode(cmdbuff);
	printf("[%d;1H\n",chaty2);
	disp_room1();
	st_mode=old_st_mode;
}


clear_roominit(int num)
{
	unlink_file("tmp/xtaja/perm.%d",num);
	unlink_file("tmp/xtaja/taja.%d",num);
	unlink_file("tmp/xtaja/chatmess.%d",num);
	unlink_file("tmp/xquiz/open.%d",num);
	unlink_file("tmp/xquiz/perm.%d",num);
	unlink_file("tmp/xquiz/quiz.%d",num);
}


in_croom(char *tnick)	//πÊø° ¿÷¥¬∞°?
{
	int i;
	for (i=0;i<20;i++) {
		if (is_same(user_room[i].nick,tnick)) {
			room=user_room[i];
			return Yes;
		}
	}
	return No;
}



set_my_room()
{
	strcpy(room.id,userid);
	strcpy(room.name,userna);
	strcpy(room.nick,userni);
	strcpy(room.tty,tty);
	strcpy(room.hostname,hostname);
	room.mode=0;		//∫∏≈Î¿« ªÛ≈¬
	room.area=userarea; 	   //∫∏≈Î¿« ªÛ≈¬
	room.color1=99;
	room.color2=99;
	room.hostno=cfg.hostno;
	time(&room.ctime);
	time(&room.date);
}


check_same()
{
	int i,found=No;
	while (1) {
		found=No;
		for (i=0;i<20;i++) {
			if (strlen(user_room[i].id)>1&&
				is_same(user_room[i].name,userna)&&
				is_same(user_room[i].nick,userni)) {
				found=Yes;
				break;
			}
		}
		if (!found&&strlen(userni)>1&&!stristr(userni,"guest")) break;
		if (stristr(userni,"guest")) printf("\n ¥Ÿ∏• ¥Î»≠∏Ì¿ª º±≈√«œººø‰.\n");
		else printf("\n ¥Î»≠∏Ì¿Ã ¡ﬂ∫πµÀ¥œ¥Ÿ. ¥Ÿ∏• ¥Î»≠∏Ì¿ª º±≈√«œººø‰.\n");
		printf("¥Î»≠∏Ì >>");
		lineinput2(userni,12);
	}
}

to_say2(char *tstr2)
{
	FILE *fp;
	char aid[1000][20];
	char aid2[20][20];
	char anick[1000][20];
	char buff[300];
	char tstr[30];
	int i,n,ii=0,count,found;
	MONI moni2;

	strcpy(tstr,tstr2);
	strcpy(tosay,"");strcpy(tosay2,"");
	for (i=0;i<1000;i++) {strcpy(aid[i],"");strcpy(anick[i],"");}
	for (i=0;i<20;i++) strcpy(aid2[i],"");
	for (i=0;i<20;i++) {
		if (strlen(user_room[ii].id)>1) {
			strcpy(aid[ii],user_room[ii].id);
			strcpy(aid2[ii],user_room[ii].nick);
			strcpy(anick[ii],user_room[ii].nick);
			ii++;
		}
	}
	count=ii;

	for (n=0;n<=cfg.count_server;n++) {
		sprintf(buff,"user/moni/moni%d.dat",n);
		if ((fp=fopen(buff,"r"))==NULL) continue;
		while (ii<1000) {
			if (!fread(&moni2,sizeof_moni,1,fp)) break;
			if (moni2.mode==99||moni2.mode<1||strlen(moni2.id)<3||strlen(moni2.tty)<1) continue;
			if (is_same(moni2.id,"¡¢º”¡ﬂ")) continue;
			found=No;
			for (i=0;i<ii;i++)
				if (is_same(aid[i],moni2.id)) found=Yes;
			if (found) continue;
			strcpy(aid[ii],moni2.id);
			strcpy(anick[ii],moni2.name);
			ii++;
		}
		fclose(fp);
	}

	if (strlen(tstr)<2) {
		printf("[%d;1H\n",chaty2);
		for (i=0;i<ii;i++) {
			printf(" %2d. %-12s",i+1,anick[i]);
			if (i%4==3) printf("\n");
		}
		printf("[%d;1H ±”º”∏ª º±≈√ ¿Ã∏ß(π¯»£)>>[K",chaty2+2);
		lineinput2(buff,13);i=atoi(buff);
		if (i>0&&i<=count) strcpy(tstr,anick[i-1]);
		else if (i>0&&i<=ii) strcpy(tstr,aid[i-1]);
		else if (strlen(buff)>1) strcpy(tstr,buff);
	}
	for (i=0;i<20;i++) {
		if (strlen(tstr)>0&&is_same(aid2[i],tstr)) {strcpy(tosay,aid2[i]);return;}
	}
	for (i=0;i<ii;i++) {
		if (strlen(tstr)>0&&is_same(aid[i],tstr)) {strcpy(tosay2,aid[i]);return;}
	}
}

check_join_ok() 	//¡¯¿‘∞°¥…»Æ¿Œ
{
	int key=No;
		 if (pf.type>=18) key=Yes;
	else if (in_file2("system/chatsys.id",pf.id)) key=Yes;
	else if (croom.openmode==5&&pf.type<croom.itype)  mess("¿Ã ¥Î»≠Ω«¿∫ øÓøµ¡¯ ¿¸øÎ¿‘¥œ¥Ÿ.");
	else if (croom.age1>0&&get_age(pf)<croom.age1)	  mess("¿Ã ¥Î»≠Ω«¿∫ ≥™¿Ã %d ¿ÃªÛ ¡¯¿‘ (%s¥‘ %dºº)",croom.age1,pf.id,get_age(pf));
	else if (croom.age2>0&&get_age(pf)>croom.age2)	  mess("¿Ã ¥Î»≠Ω«¿∫ ≥™¿Ã %d ¿Ã«œ ¡¯¿‘ (%s¥‘ %dºº)",croom.age2,pf.id,get_age(pf));
	else if (croom.sex==1&&pf.sex!='m')               mess("¿Ã ¥Î»≠Ω«¿∫ ≥≤º∫ ¿¸øÎ¿‘¥œ¥Ÿ.");
	else if (croom.sex==2&&pf.sex!='f')               mess("¿Ã ¥Î»≠Ω«¿∫ ø©º∫ ¿¸øÎ¿‘¥œ¥Ÿ.");
	else if (croom.count>=croom.max)				  mess("¿Ã ¥Î»≠Ω«¿« ¡§ø¯¿ª √ ∞˙«’¥œ¥Ÿ.");
	else if (croom.itype>pf.type)					  mess("¡¯¿‘µÓ±ﬁ¿Ã ∫Œ¡∑«’¥œ¥Ÿ.(%s¿ÃªÛ)",atype[croom.itype]);
	else key=Yes;
	return key;
}


//º≠∫Í∏ﬁ¥∫ √‚∑¬
read_croom()
{
	FILE *fp;
	int i,found=No;
	croom.count=0;
	strcpy(croom.id,"");strcpy(croom.title,"");
	for (i=0;i<20;i++) strcpy(user_room[i].id,"");
	if (roomnum==0) return Yes;
	sprintf(ROOM_FILE,"user/data/croom.%d",roomnum);
	if ((fp=fopen(ROOM_FILE,"r"))!=NULL) {
		if (fread(&croom,sizeof_croom,1,fp)) found=Yes;
		fclose(fp);
	}
	if (croom.max<2||croom.max>20) croom.max=6;
	return found;
}


read_user_room()
{
	int found=No,i,i2;
	FILE *fp;
	char filename[500];
	croom.count=0;
	for (i=0;i<CMAX;i++) bzero(&user_room[i],sizeof_room);
	if (roomnum<1) return found;
	sprintf(filename,"user/data/cuser.%d",roomnum);
	if ((fp=fopen(filename,"r"))!=NULL) {
		i=0;
		while(i<CMAX&&fread(&room,sizeof_room,1,fp)) {
			for (i2=0;i2<(int)strlen(room.nick);i2++) {
				if (is_char("\\/<>'\";&%$ ",room.nick[i2]))
					room.nick[i2]='_';
			}
			if (strlen(room.id)>0) {
				croom.count++;
				user_room[i]=room;
				found=Yes;
			}
			i++;
		}
	}
	return found;
}

del_user_room()
{
	int i,found=No;
	for (i=0;i<CMAX;i++) {
		if (is_same(user_room[i].id,userid)&&
		  is_same(user_room[i].name,userna)&&
		  is_same(user_room[i].nick,userni)&&
		  !is_same(user_room[i].tty,"www")&&
		  is_same(user_room[i].hostname,hostname)) {
		  bzero(&user_room[i],sizeof_room);
		  found=Yes;
		}
	}
	return found;
}



//mynum==0¿Ã∏È ¿¸∫Œ ¿˙¿Â
save_user_room(int mynum)  //«ÿ¥Áπ¯»£¿« user_room ¿˙¿Â
{
	FILE *fp;
	char filename[500];
	int i;
	if (roomnum<1||roomnum>99999) return;
	sprintf(filename,"user/data/cuser.%d",roomnum);
	if (mynum==0) {
		if ((fp=fopen(filename,"w+"))!=NULL) {
			for (i=0;i<CMAX;i++) {
				fwrite(&user_room[i],sizeof_room,1,fp);
			}
			fclose(fp);
		}
	}
	else if (mynum<=CMAX) {
		if ((fp=fopen(filename,"r+"))==NULL) fp=fopen(filename,"w+");
		if (fp!=NULL) {
			fseek(fp,(mynum-1)*sizeof_room,SEEK_SET);
			fwrite(&user_room[mynum-1],sizeof_room,1,fp);
			fclose(fp);
		}
	}
	if (rand()%10==0) chmod(filename,0666);
}



add_croom()
{
	FILE *fp;
	int i,ii,i2,found=No,all_save=No;

	sprintf(ROOM_FILE,"user/data/croom.%d",roomnum);
	if (roomnum<=0) {		//¥Î±‚Ω«ø° √ﬂ∞°
		ROOM aroom[1001];
		roomnum=0;
		ii=0;
		for (i=0;i<1001;i++) strcpy(aroom[i].id,"");
		ii=0;
		if ((fp=fopen(ROOM_FILE,"r"))!=NULL) {
			while(fread(&room,sizeof_room,1,fp)) {
				if (strlen(room.id)>0&&strlen(room.tty)>0) {
					aroom[ii++]=room;
				}
			}
			fclose(fp);
		}
		for (i=0;i<ii;i++) {
			if (is_same(aroom[i].id,userid)&&
				is_same(aroom[i].hostname,hostname)&&
				aroom[i].hostno==cfg.hostno) {
				strcpy(aroom[i].id,"");
			}
		}
		//ªı∑Œ¿˙¿Â
		set_my_room();		//roomº≥¡§
		aroom[ii++]=room;
		qsort(aroom,ii,sizeof_room,comp_9);
		if ((fp=fopen(ROOM_FILE,"w+"))!=NULL) {
			for (i=0;i<ii+1;i++) {
				room=aroom[i];
				if (strlen(room.id)>1) {
					fwrite(&room,sizeof_room,1,fp);
				}
			}
			fclose(fp);
		}
		return Yes;
	}

	//µø¿œ«— ¿⁄∑·∞° ¿÷¿∏∏È ªË¡¶«‘
	for (i=0;i<CMAX;i++) {
		if (is_same(user_room[i].id,"_3b¥Î»≠πÊ_")||
		   (is_same(user_room[i].id,userid)&&
			is_same(user_room[i].nick,userni)&&
			is_same(user_room[i].hostname,hostname))) {
		  bzero(&user_room[i],sizeof_room);
		  all_save=Yes;
		}
	}

	for (i=0;i<CMAX;i++) {
		if (strlen(user_room[i].id)<2) {
			room=user_room[i];
			cut_badhan(userid,12);cut_badhan(userna,12);cut_badhan(userni,12);
			xstrncpy(room.id  ,userid,12);cut_badhan(room.id,12);
			xstrncpy(room.name,userna,12);cut_badhan(room.name,12);
			xstrncpy(room.nick,userni,12);cut_badhan(room.nick,12);
			strcpy(room.tty ,tty);
			xstrncpy(room.hostname,hostname,20);
			room.sex =usersex;
			room.hostno=cfg.hostno;
			room.mode=0;
			room.color1=0;
			room.color2=0;
			room.pid=getpid();
			time(&room.ctime);
			time(&room.date);
			time(&room.rtime);	   //∫π±«∞°¥…Ω√∞£
			room.alive=0;		  //ªÏæ∆¿÷¿Ω»Æ¿Œ
			user_room[i]=room;
			found=Yes;
			break;
		}
	}
	count_croom();
	check_mynum2();
	if (mynum<1) found=No;
	if (!found||croom.count<2||all_save) save_user_room(0);  //«ÿ¥Á user_room ¿˙¿Â
	else if (mynum>0) save_user_room(mynum);  //«ÿ¥Á user_room ¿˙¿Â

	//πÊ¿Â¡÷±‚
	if (croom.count<=1) {
		for (i=0;i<CMAX;i++) {
			if (strlen(user_room[i].id)>1&&strlen(user_room[i].tty)>1) {
				xstrncpy(croom.id,user_room[i].id,12);
				xstrncpy(croom.name,user_room[i].nick,12);
				xstrncpy(croom.hostname,user_room[i].hostname,20);
				break;
			}
		}
	}
	save_croom();
	return found;
}


chat_line(int mode,char *chatbuff1)
{
	FILE *fp;
	int i,found=No;
	char buff[300],buff1[300],buff2[300],name[30];
	char cbuff1[300],cbuff2[300];
	char chatbuff[500];

	if (strlen(chatbuff1)<1) return;
	strcpy(chatbuff,chatbuff1);
	strcpy(name,"");
	if (mynum<1) check_mynum2();
	if (mynum<1) check_mynum();

	if (mynum>0) strcpy(name,user_room[mynum-1].nick);
	if (mode==93||mode==95||mode>=97) {add_chat_line(mynum,mode,userni,chatbuff);return;}
	else if (mode==90) {add_chat_line(0,mode,"",chatbuff);return;}

	if (mode<3&&bit(croom.dispmode,3)&&strlen(chatbuff)>2) {
		del_tag(chatbuff);
	}
	sscanf(chatbuff,"%s%s",cbuff1,cbuff2);

	if (is_same(cbuff1,"/tty"))  disp_tty(1,mynum);
	else if (strlen(tosay)>0&&!stristr(tosay,"¿¸√º")&&!stristr(tosay,"-")) {
		char tbuff[100];
		found=No;
		strcpy(tbuff,"");
		for (i=0;i<CMAX;i++) {
			if ((strlen(user_room[i].id)>1)&&
				is_same(user_room[i].nick,tosay)) {
				add_chat_line(i+1,94,userni,chatbuff);
				strcpy(tbuff,user_room[i].nick);
				found=Yes;
			}
		}
		if (!found) strcat(chatbuff,"[¿¸º€Ω«∆–]");
		strchng2(chatbuff,"\"","\\\"");
		strchng2(chatbuff,"\'","\\\'");
		add_chat_line(mynum,96,tbuff,chatbuff);
	}
	else if (croom.count>0) {
			strchng2(chatbuff,"\"","\\\"");
			strchng2(chatbuff,"\'","\\\'");
			add_chat_line(0,mode,name,chatbuff);
	}
}


add_chat_line(int mynum,int mode,char *name,char *chatbuff1)
{
	FILE *fp;
	char buff[2500],buff1[200],buff2[200],chat_file[300],chatbuff[500];
	int i,ii;
	int facenum,color,fontface;

	//cut_badhan(chatbuff1);
	strcpy(chatbuff,chatbuff1);

	sprintf(chat_file,"user/chat/wchat.%d",roomnum);
	if (mynum>0&&mynum<=CMAX) {
		if (!is_same(user_room[mynum-1].tty,"www")) {
			if (strlen(user_room[mynum-1].id)>1&&user_room[mynum-1].mode!=1) {
				del_tag(chatbuff);
				chat_say_tty(mode,user_room[mynum-1].hostno,user_room[mynum-1].tty,name,chatbuff);
			}
			return;
		}
		sprintf(chat_file,"user/chat/wchat.%d.%d",roomnum,mynum);
	}
	if ((fp=fopen(chat_file,"a+"))==NULL) fp=fopen(chat_file,"w+");
	if (fp==NULL) return;
	fprintf(fp,"%2d %2d %2d %d %d %-25.25s %s%s%s \n",
	mode,0,0,0,0,name,"","",chatbuff);
	fclose(fp);
	if (croom.count<2||rand()%5==0) chmod(chat_file,0666);

	del_tag(chatbuff);
	for (i=0;i<CMAX;i++) {
		if (mynum>0&&i!=mynum-1) continue;
		if (strlen(user_room[i].id)>1&&user_room[i].mode!=1) {
			chat_say_tty(mode, user_room[i].hostno,user_room[i].tty, name, chatbuff);
		}
	}
	fflush(NULL);
}



//7  8∫π±∏
chat_say_tty(int mode,int hostno,char *dtty,char *nick,char *mess_str)
{
	char out[2000],outstr[2000],buff1[10],buff2[10],buffi[50];
	if (strlen(dtty)<1||is_same(dtty,"www")) return;
	if (cfg.count_server<=1) hostno=cfg.hostno;
	if (mode==90||mode==95) return;
	else if (mode==94||mode==96) sprintf(outstr," [7m%12.12s ¢–[m %s",nick,mess_str);
	else if (mode>90) sprintf(outstr,center_line(mess_str));
	else if (is_same(dtty,tty))
		sprintf(outstr," [7m%12.12s ¢∫[m %s",nick,mess_str);
	else
		sprintf(outstr," [m%12.12s ¢∫ %s",nick,mess_str);

	sprintf(out,"[s[%d;1H\n%s[u",chaty2,outstr);
	to_tty2(hostno,dtty,out);
}



