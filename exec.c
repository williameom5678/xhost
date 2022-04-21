////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 이 소스를 변경하여 상용으로 사용하실때에는  개발자의 승인을 받아야 합니다.
// 내용 : 외부명령루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

exec_menu(int mode)
{
	char buff[480];
	char buff1[480];
	char buff2[480];
	char buff3[480];
	char buff4[480];
	char buffcmd[400];
	int ret=Yes,n;

	sprintf(buff,"bin/%s",go.code);
	if (strlen(go.dir)>0) strcpy(buffcmd,go.dir);
	else if (is_file(buff)) strcpy(buffcmd,buff);
	else sprintf(buffcmd,"xgame/%s/%s",go.code);
	strchng2(buffcmd,"_S"," ");all_trim(buffcmd);
	all_trim(go.dir);

	del_space(go.wtimes);
	check_bbsrec(0);					  //게시판 기록 관련
	if (!is_char(EXETYPE,go.type)) ;
	else if (check_type('j')) ;
	else if (check_type('i')) ;
	else if (check_type('r')) ;
	else if (no_exec_port(go.code)) ;
	else if (ss_check(5))  ;			   //외부명령금지
	else {
		if (read_bcfg()) {
			bbs_down_up(1,pf.id,1);
		}
		sprintf(buff4,"%s/system/%s.exx",club_go.dir,buffcmd);
		del_space(buff4);
		if (club_mode&&!is_file(buff4)) sprintf(buff4,"system/%s.exx",buffcmd);
		del_space(buff4);
		if (pf.type<18&&is_file(buff4)&&!in_file2(buff4,pf.id)) {
			sprintf(buff4,"%s/logo/%s.lox",club_go.dir,buffcmd);
			del_space(buff4);
			if (club_mode&&!is_file(buff4)) sprintf(buff4,"logo/%s.lox",buffcmd);
			del_space(buff4);
			view(buff4);
			goto _quit;
		}
		if (!view("%s/logo/%s.log",club_go.dir,go.code)) view("logo/%s.log",go.code);

		if (go.type=='n') {
			telnet_job(buffcmd);
			ret=Yes;
			goto _quit;
		}
		if (is_char(go.wtimes,'j')) ioctl(0,TCSETA,&termold);
			 if (stristr(go.wtimes,"ex1")) log_exec(1,buffcmd);   //외부로 바로 JUMP
		else if (stristr(go.wtimes,"ex2")) log_exec(2,buffcmd);   //외부로 바로 JUMP
		else if (stristr(go.wtimes,"ex3")) log_exec(3,buffcmd);   //외부로 바로 JUMP
		else if (stristr(go.wtimes,"ex4")) log_exec(4,buffcmd);   //외부로 바로 JUMP
		else if (stristr(go.wtimes,"ex9")) log_exec(9,buffcmd);   //외부로 바로 JUMP
		else if (stristr(go.wtimes,"ex"))  do_execl(go.dir);
		else {
			if		(is_char(go.wtimes,'1' )) sprintf(buff1,"%s '%s'",buffcmd,pf.id);
			else if (is_char(go.wtimes,'2' )) sprintf(buff1,"%s '%s' '%s'",buffcmd,go.code,pf.id);
			else if (is_char(go.wtimes,'3' )) sprintf(buff1,"%s '%s' '%s' '%s'",buffcmd,club_go.code,go.code,pf.id);
			else if (is_char(go.wtimes,'4' )) sprintf(buff1,"%s '%s' '%s' '%s' %d",buffcmd,club_go.code,go.code,pf.id,pf.type);
			else if (is_char(go.wtimes,'5' )) sprintf(buff1,"%s '%s' '%d'",buffcmd,pf.id,pf.type);
			else if (stristr(go.wtimes,"xsl")) execl("/bin/slirp","slirp","ppp","-b","56000",NULL);
			else if (stristr(go.wtimes,"sd")) sprintf(buff1,"bin/se_down '%s' '%d' '%s' '%s'",pf.id,pf.type,go.code,go.dir);
			else if (stristr(go.wtimes,"xs")) sprintf(buff1,"%s %s %s %s %s %d %c _ _ _ ",buffcmd,pf.id,pf.name,go.code,datestr(9,pf.birthday),pf.moon,pf.sex);
			else if (stristr(go.wtimes,"xq")) sprintf(buff1,"bin/quiz '%s' '%s'",pf.id,go.code);
			else if (stristr(go.wtimes,"xc")) sprintf(buff1,"bin/xcd 2 '%s' '%s'",pf.id,go.code);
			else if (stristr(go.wtimes,"xm")) sprintf(buff1,"bin/xmenu '%s' '%s' '%s'",club_go.code,go.code,pf.id);
			else if (stristr(go.wtimes,"xb")) sprintf(buff1,"%s xbase '%s' '%s' .. 0000/00/00 양 00 남",buffcmd,pf.id,pf.name,pf.nick,pf.sex);
			else if (stristr(go.wtimes,"xt")) {sprintf(buff2,"%s",datestr(32,pf.birthday));sprintf(buff1,"%s '%s' '%s' '%s' '%s' '%d' '%c' '%s'",buffcmd,pf.id,pf.name,pf.nick,datestr(27,pf.birthday),pf.moon,pf.sex,buff2);}
			else strcpy(buff1,buffcmd);
			strchng2(buff1,"_S"," ");
			do_exec(buff1);
			if (is_char(go.wtimes,';')) pressenter();
			if (!view("%s/logo/%s.loe",club_go.dir,go.code)) view("logo/%s.loe",go.code);
		}
	}

	_quit:
	if (is_char(go.wtimes,'j')) ioctl(0,TCSETA,&termall);
	if (is_char(EXETYPE,go.type)) go_prev();
	check_bbsrec(0);					  //게시판 기록 관련
	re_print=Yes;
}


do_execl(char *filename)
{
	FILE *fp;
	char abuff[10][300],buff[300];
	int i;

	if (strlen(filename)<1||!is_file(filename)) {
		printf("%s execl 파일이 없습니다.",filename);
		return;
	}
	for (i=0;i<10;i++) strcpy(abuff[i],"");
	if ((fp=fopen(filename,"r"))==NULL) return;
	i=0;
	while (i<10&&fgets(buff,200,fp)!=NULL) {
		del_enter(buff);
		strcpy(abuff[i],buff);
		i++;
	}
	fclose(fp);
	execl(abuff[0],abuff[1],abuff[2],abuff[3],abuff[4],abuff[5],abuff[6],abuff[7],abuff[8],abuff[9],NULL);
}


do_exec(char *buff)
{
	int n,mode=1;
	re_print=Yes;
	if (strlen(buff)<1) return;
	if (stristr(buff,"xgame/")||is_char(go.wtimes,'g'))  mode=4;   //게임
	else mode=15;  //외부
	set_stat2(mode,buff);	 //외부
	set_rec(1,mode,0,buff);  //기록
	tag_monichar(); //현재 기록
	n=sys_tem(buff);
	if (c_proc2) {printf("명령:%s->%d\n",buff,n);pressenter();}
//	  set_signal();  //시그날 재설정
	printf("\n");
	set_stat(1,"");
}

#define MAXMIN 10

telnet_job(char *buffcmd)
{
	char buff[100];
	int n;

	strchng2(go.dir,"_S"," ");
	if (go.dir[0]!=0&&is_char(go.wtimes,'9')) {
		if (is_char(go.wtimes,'j')) ioctl(0,TCSETA,&termold);
		do_telnet(2,go.dir);
	}
	else if (go.dir[0]!=0&&!is_char(go.wtimes,'p')) {
		view("frm/telnet/wait.log");
		strcpy(buff,buffcmd);
		strchng9(buff," ");
		all_trim(buff);
		n=sys_tem("bin/check_te %s",buff);
//		  set_signal();  //시그날 재설정
		if (n!=0) {
			view("frm/telnet/fail.log");
			return;
		}
		view("frm/telnet/xtelnet.log");//진입화면출력  *.log
		if (is_char(go.wtimes,'j')) ioctl(0,TCSETA,&termold);
		do_telnet(1,go.dir);
	}
	else if (is_char(go.wtimes,'*')) {
		_cont3:
		if (!view("frm/telnet/open1.log")) printf("\n 텔넷 주소>>");
		lineinput(buff,50);check_han(buff);printf("\n");
		n=atoi(buff);
		if (n>0&&n<=99) file_line(buff,"frm/telnet/telnetn.txt",n);
		if (strlen(buff)>1) {
			if (is_char(go.wtimes,'j')) ioctl(0,TCSETA,&termold);
			do_telnet(1,buff);
		}
	}
	else if (strlen(go.dir)<1&&is_char(go.wtimes,'#')) {
		if (!view("frm/telnet/open2.log")) printf("텔넷 번호>>");
		lineinput(buff,3);check_han(buff);printf("\n");
		n=atoi(buff);
		if(n==99) goto _cont3;
		if(n<1||n>98) return;
		file_line(buff,"frm/telnet/telnetn.txt",n);
		if (strlen(buff)>1) {
			if (is_char(go.wtimes,'j')) ioctl(0,TCSETA,&termold);
			do_telnet(1,buff);
		}
	}
	else {
		view("frm/telnet/xtelnet.log");//진입화면출력  *.log
		if (is_char(go.wtimes,'j')) ioctl(0,TCSETA,&termold);
		do_telnet(1,go.dir);
	}
}




do_telnet(int mode2,char *buff)
{
	int mode=1;
	char buffstr[400],buff1[200],buff2[400],buff3[400];
	re_print=Yes;
	del_esc3(buff);
	strchng2(buff,"_S"," ");
	if (strlen(buff)<1) return;
	if (go.type=='n'&&is_char(go.wtimes,'m')) mode=13;  //머드
	else if (go.type=='n')                    mode=16;       //외부
	set_stat2(mode,buff);	 //외부
	set_rec(1,mode,0,"telnet");  //기록
	tag_monichar(); //현재 기록
//	  set_signal();  //시그날 재설정
	del_esc3(buff);
	if (mode2==2) sys_tem("bin/stelnet %s",buff);
	else		  sys_tem("bin/xtelnet %s",buff);
//	  set_signal();  //시그날 재설정
	printf("\n");
	set_stat(1,"");
}


//사용자 유틸 exec
uu_exec(int mode)
{
	char buff1[220];
	char buff2[120];
	sprintf(buff2,"bin/util%d",mode);
	if (is_file(buff2)) {
		ioctl(0,TCSETA,&termold);	//원래 터미날 모드 복구
		sprintf(buff1,"%s %s %s %s %s %d",buff2,pf.id,pf.type,club_go.code,club_go.dir,go.code);
		sys_tem(buff1);
		ioctl(0,TCSETA,&termall);	//원래 터미날 모드 복구
	}
}
