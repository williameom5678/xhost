////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
////////////////////////////////////////////////////////////////////////
#include "util.c"
char nowid[10];
char jobid[10];
PF nowpf,jobpf;

main(int argc, char **argv)
{
	int mode,i;
	mode=atoi(argv[1]);
	strcpy(jobid,argv[2]);
	strcpy(nowid,argv[3]);
	who=atoi(argv[4]);

	if (!read_pf2(jobid)) {mess(" %s 회원을 찾을수 없습니다.",jobid);return;}
	jobpf=pf2;
	read_pf2(nowid);
	nowpf=pf2;
	findport(tty);
	read_ajob();	//직업읽기
	read_atype();
	read_cfg();
	set_tmpfile();
	if (is_same(nowid,"root")) {
		strcpy(nowpf.id,cfg.sysop);
		strcpy(nowpf.name,cfg.sysop);
		nowpf.type=19;
		who=5;
	}
	pf=jobpf;
	for (i=0;i<23;i++) printf("\n");
		 if (mode==1) viewpf1();
	else if (mode==8&&who<5&&pf.type>=18) viewpf1();
	else if (mode==8) viewpf8();
	else if (mode==9&&who>=4) viewpf8();
	else if (mode==9&&who<=3) viewpf1();
}


viewpf1()
{
	int num=1;
	view("frm/pf/pf.scr");
							printf("  (%2d) 등      급 : %2d (%s)\n",num++,pf.type,atype[pf.type]);
							printf("  (%2d) 레      벨 : %d\n",num++,pf.level);
							printf("  (%2d) 사이버머니 : %d\n",num++,pf.cash);
							if (is_moni(pf.id))
							printf("  (%2d) 접속  시작 : %s 부터 사용중\n",num++,datestr(33,pf.login));
							else
							printf("  (%2d) 최근  시간 : %s 에 접속종결\n",num++,datestr(33,pf.logout));
							printf("  (%2d) 인터넷메일 : %s\n",num++,pf.email);
	if (bit(pf.myflag, 1))	printf("  (%2d) 생      일 : %s\n",num++,datestr(1,pf.birthday));
	if (bit(pf.myflag, 2))	printf("  (%2d) 성      별 : %s\n",num++,is_char("0f",pf.sex)?"여":"남");
	if (bit(pf.myflag,10))	printf("  (%2d) 직      업 : %2d %s\n",num++,pf.job,ajob[pf.job]);
	if (bit(pf.myflag, 3))	printf("  (%2d) 우      편 : %s\n",num++,pf.pos1);
	if (bit(pf.myflag, 4))	printf("  (%2d) 주      소 : %s\n",num++,pf.addr);
	if (bit(pf.myflag,11))	printf("  (%2d) 직장／학교 : %s\n",num++,pf.offi);
	if (bit(pf.myflag,17))	printf("  (%2d) 취      미 : %s\n",num++,pf.user4);
	if (bit(pf.myflag,13))	printf("  (%2d) 하고싶은말 : %s\n",num++,pf.memo1);
	if (bit(pf.myflag,15))	printf("  (%2d) 관심  분야 : %s\n",num++,pf.user2);
	if (bit(pf.myflag,16))	printf("  (%2d) 컴퓨터경력 : %s\n",num++,pf.user3);
	view("frm/pf/pf.end");
}

viewpf8()
{
	view("frm/pf/pfs.scr");
	view("frm/pf/pfs.end");
}
