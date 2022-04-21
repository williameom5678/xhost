////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: 리눅스용 BBS HOST							  //
// 손님가입메뉴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
int passidno=No;

main(int argc, char **argv)
{
	int now_mode=0;
	int ret=0;
	srand(time(NULL));			//난수의 초기화
	findport(tty);
	read_cfg();
	set_tmpfile();
	read_ajob();	//직업읽기
	allmode=set_bit(allmode,7,1);  //암호화면보이기(VIEW에서)
	touch_file(GUEST_FILE);
	touch_file(ID_FILE);
	if (argc>1) now_mode=atoi(argv[1]); 	   //선택모드
    if (now_mode==998) {wait_free2();return;}   //가입처리
    if (now_mode==999) {wait_free();return;}   //가입처리

	if (now_mode<2) ret=guest_norm();		   //일반가입
	exit(ret);
	//ret=0:취소
	//1:가입(계속접속)
	//2:가입(재접속)
	//3:가입(접속종료)
}


guest_norm()
{
	int n,i,key=No;
	char buff[140];

	clear_pf();
	_cont1:
	view("frm/guest/guestsel.log");
	lineinput(buff,5);check_han(buff);printf("\n");
	if (is_char("NnQqPpXx",buff[0])) {key=No;goto quit_;}
	if (is_same(buff,"*jum5")) goto jump5;
	if (is_same(buff,"*jum6")) goto jump6;
	key=Yes;
	if (key&&!bit(cfg.guestin,1)) key=gg_id();
	if (key&&read_pf2(pf.id)) {
		mess("이 아이디는 이미 신청이 된 ID입니다.");
		clear_pf();
		goto _cont1;
	}
	if (key&&!bit(cfg.guestin,2))  key=gg_name();
	if (key&&!bit(cfg.guestin,3))  key=gg_idno();
	if (key&&passidno) goto _done;		  //자동입력 (비번)
	if (key&&!bit(cfg.guestin,4))  key=gg_birth();
	if (key&&!bit(cfg.guestin,5))  key=gg_sex();
	if (key&&!bit(cfg.guestin,6))  key=gg_pass();
	if (key&&!bit(cfg.guestin,7))  key=gg_add1();
	if (key&&!bit(cfg.guestin,8))  key=gg_tele();
	if (key&&!bit(cfg.guestin,9))  key=gg_add2();
	if (key&&!bit(cfg.guestin,10)) key=gg_bibi();
	if (key&&!bit(cfg.guestin,11)) key=gg_hand();
	if (key&&!bit(cfg.guestin,12)) key=gg_job();
	if (key&&!bit(cfg.guestin,13)) key=gg_offi();
	if (key&&!bit(cfg.guestin,14)) key=gg_otel();
	if (key&&!bit(cfg.guestin,15)) key=gg_hobby(1);
	if (key&&!bit(cfg.guestin,16)) key=gg_hobby(2);
	if (key&&!bit(cfg.guestin,17)) key=gg_hobby(3);
	if (key&&!bit(cfg.guestin,18)) key=gg_hobby(4);
	if (key&&!bit(cfg.guestin,19)) key=gg_hobby(5);
	if (!key) goto quit_;

	jump5:
	_done:
	while (key) {
		_cont2:
		view("frm/guest/guestok.scr");
		lineinput(buff,10);printf("\n");check_han(buff);
		if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return 0;
		if (strlen(buff)==1&&is_char("Yy",buff[0])) break;
		n=atoi(buff);
		if (n== 1&&!bit(cfg.guestin,1))  key=gg_id();
		if (key&&read_pf2(pf.id)) {
			mess("이 아이디는 이미 신청이 된 ID입니다.");
			goto _cont2;
		}
		if (key&&n== 2&&!bit(cfg.guestin,2))  key=gg_name();
		if (key&&n== 3&&!bit(cfg.guestin,3))  key=gg_idno();
		if (key&&n== 4&&!bit(cfg.guestin,4))  key=gg_birth();
		if (key&&n== 5&&!bit(cfg.guestin,5))  key=gg_sex();
		if (key&&n== 6&&!bit(cfg.guestin,6))  key=gg_pass();
		if (key&&n== 7&&!bit(cfg.guestin,7))  key=gg_add1();
		if (key&&n== 8&&!bit(cfg.guestin,8))  key=gg_tele();
		if (key&&n== 9&&!bit(cfg.guestin,9))  key=gg_add2();
		if (key&&n==10&&!bit(cfg.guestin,10)) key=gg_bibi();
		if (key&&n==11&&!bit(cfg.guestin,11)) key=gg_hand();
		if (key&&n==12&&!bit(cfg.guestin,12)) key=gg_job();
		if (key&&n==13&&!bit(cfg.guestin,13)) key=gg_offi();
		if (key&&n==14&&!bit(cfg.guestin,14)) key=gg_otel();
		if (key&&n==15&&!bit(cfg.guestin,15)) key=gg_hobby(1);
		if (key&&n==16&&!bit(cfg.guestin,16)) key=gg_hobby(2);
		if (key&&n==17&&!bit(cfg.guestin,17)) key=gg_hobby(3);
		if (key&&n==18&&!bit(cfg.guestin,18)) key=gg_hobby(4);
		if (key&&n==19&&!bit(cfg.guestin,19)) key=gg_hobby(5);
		if (key&&n==99) break;
		if (!key) goto quit_;
	}
	view("frm/guest/wait.scr");

	reset_pf_local();	   //로칼메뉴지정
	if (bit(cfg.guestmode,15)) save_auto();
	else					   save_guest();
	jump6:
	view("frm/guest/bye.scr");
	if (bit(cfg.guestmode,3)) return 3;  //가입후 종료
	if (bit(cfg.guestmode,4)) return 2;  //가입후 재접속
	lineinput(buff,2);printf("\n");
	n=atoi(buff);
	if (n>5||n<1) n=1;
	return (n);

	quit_:
	view("frm/guest/cancel.end");
	return 0;
}


gg_id()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	_id:
	if (count++>5)	return No;
	view("frm/guest/id.scr");
	lineinput(buff,8);printf("\n");all_trim(buff);
	del_space(buff);
	if (is_quit(buff)) return No;
	if (strlen(buff)<3||check_file_char(buff,"frm/guest/notid.chr")) {
		if (yesno3("frm/guest/id1.err",Yes)!=Yes) return No;
		goto _id;
	}
	if (check_badname(buff)) {view("frm/guest/id2.err");goto _id;}
	if (check_badchar(buff)) {view("frm/guest/id3.err");goto _id;}
	if (check_sameid2(buff)) {view("frm/guest/id4.err");goto _id;}
	if (check_waitid(buff))  {view("frm/guest/id5.err");goto _id;}
	if (check_badid(buff) )  {view("frm/guest/id6.err");goto _id;}
	strcpy(pf.id,buff);
	return Yes;
}



gg_pass()
{
	int count,i,n,ret;
	char buff[100],buff1[100];
	char buff2[100],buff3[100];
	count=0;
	_pass:
	strcpy(buff1,"");strcpy(buff2,"");
	if (count++>8)	return No;
	view("frm/guest/pass1.scr");
	passinput(buff1,8);printf("\n");check_han(buff1);del_space(buff1);
	if (strlen(buff1)==1&&is_char("NnQqPpXx",buff1[0])) return No;
	if (strlen(buff1)<3) {
		if (yesno3("frm/guest/pass1.err",Yes)) goto _pass;
		else return No;
	}
	if (bit(cfg.guestin,24)&&check_easypass(buff1)) {
		if (yesno3("frm/guest/pass2.err",Yes)) goto _pass;
		else return No;
	}
	view("frm/guest/pass2.scr");
	passinput(buff2,8);printf("\n");check_han(buff2);del_space(buff2);
	if (strlen(buff2)==1&&is_char("NnQqPpXx",buff2[0])) return No;
	if (strcmp(buff2,buff1)!=0) {
		if (yesno3("frm/guest/pass3.err",Yes)) goto _pass;
		else return No;
	}
	strcpy(pf.passwd,buff1);
	view("frm/guest/pass.end");
	return Yes;
}

gg_name()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	while (1) {
		if (count++>5)	return No;
		view("frm/guest/name.scr");
		del_space(buff);
		lineinput(buff,8);printf("\n");all_trim(buff);
		if (is_quit(buff)) return No;
		if (strlen(buff)<4||check_file_char(buff,"frm/guest/notname.chr")) {
			if (yesno3("frm/guest/name1.err",Yes)!=Yes) {
				return No;
			}
			continue;
		}
		if (check_badname(buff)) {view("frm/guest/name2.err");continue;}
		break;
	}
	strcpy(pf.name,buff);
	return Yes;
}


gg_idno()
{
	int count,i,n,ret;
	char buff[100],buff2[100],buff3[100];
	if (bit(cfg.guestin,1)) {strcpy(pf.id_no,"");return Yes;}
	count=0;
	_idno:
	if (count++>8)	goto _fail;
	view("frm/guest/idno.scr");
	lineinput(buff,14);printf("\n");del_space(buff);check_han(buff);
	if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
	if (in_file2("system/passidno.txt",buff)) {
		passidno=Yes;
		strcpy(pf.id_no,"000000-0000000");
		goto _done;
	}
	if (strlen(buff)<12) {
		if (yesno3("frm/guest/idno1.err",Yes)) goto _idno;
		else goto _fail;
	}
	strchng2(buff,"-","");
	xstrncpy(buff2,buff,6);
	strcpy(buff3,&buff[6]);
	sprintf(buff,"%s-%s",buff2,buff3);
    buff[14]=0;
	strcpy(g_buff1,buff);
	if (check_badidno(buff))   {view("frm/guest/idno2.err");goto _idno;}
	if (check_sameidno(buff))  {view("frm/guest/idno3.err");goto _idno;}
	if (check_waitidno(buff))  {view("frm/guest/idno4.err");goto _idno;}
	if (!is_good_idno(buff))   {view("frm/guest/idno5.err");goto _idno;}
	strcpy(pf.id_no,buff);
	_done:
	view("frm/guest/idno.end");
	if (bit(cfg.guestin,5)) {	//성별묻지않음
		if (pf.id_no[7]=='2'|| pf.id_no[7]=='4') pf.sex='f';
		else pf.sex='m';
	}
	if (bit(cfg.guestin,4)) {	//생일묻지않음
		sprintf(buff,"%c%c/%c%c/%c%c",pf.id_no[0],pf.id_no[1],pf.id_no[2],pf.id_no[3],pf.id_no[4],pf.id_no[5]);
		pf.birthday=str2date(buff);
		pf.moon=1;
	}
	return Yes;
	_fail:
	strcpy(pf.id_no,"");
	return No;

}

gg_birth()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	if (count++>8)	return No;
	view("frm/guest/birth1.scr");
	strcpy(buff,datestr(1,pf.birthday));
	del_space(buff);	//1999-12-31
	dateinput(buff);printf("\n");all_trim(buff);
	pf.birthday=str2date(buff);
	view("frm/guest/birth2.scr");
	lineinput(buff,2);printf("\n");all_trim(buff);check_han(buff);
	if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
	if (is_same(buff,"m")||is_same(buff,"음")||is_same(buff,"2")) pf.moon=2;
	else pf.moon=1;
	return Yes;
}


gg_sex()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	_cont:
	if (count++>8)	return No;
	view("frm/guest/sex.scr");
	lineinput(buff,4);check_han(buff);printf("\n");all_trim(buff);
	if (is_quit(buff)) return No;
	if (is_same(buff,"f")||is_same(buff,"여")||is_same(buff,"2")) pf.sex='f';
	else pf.sex='m';
	return Yes;
}



gg_add1()
{
	int count,i,n,ret;
	char buff[300],buff2[300];
	count=0;
	_addr1:
	if (count++>8)	return No;
	view("frm/guest/addr1.scr");
	lineinput(buff,14);check_han(buff);printf("\n");all_trim(buff);
	if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
	if (strlen(buff)<4) {
		if (yesno3("frm/guest/addr1.err",Yes)) goto _addr1;
		else return No;
	}
	strcpy(g_buff1,buff);
	i=check_selectaddr(buff,buff2);
	if (i==2) goto _addr1;
    if (i==1) {view("frm/guest/addr2.err");goto _addr1;}//없으면..
	xstrncpy(pf.addr,buff ,70);
	xstrncpy(pf.pos1,buff2,7);

	count=0;
	_addr2:
	if (count++>8)	return No;
	if (bit(cfg.guestin,25)==0) {
		strcpy(g_buff2,pf.pos1);
		strcpy(g_buff3,"");
		view("frm/guest/addr2.scr");
		strcpy(buff,pf.addr);
		dosinput(buff,70);printf("\n");all_trim(buff);
		if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
		if (strlen(buff)<4) {
			if (yesno3("frm/guest/addr3.err",Yes)) goto _addr1;
			else return No;
		}
		all_trim(buff);xstrncpy(pf.addr,buff,70);
	}
	else {
		strcpy(g_buff2,pf.pos1);
		strcpy(g_buff3,pf.addr);
		view("frm/guest/addr2.scr");
		lineinput(buff,60);check_han(buff);printf("\n");
		if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
		if (strlen(buff)<4) {
			if (yesno3("frm/guest/addr3.err",Yes)) goto _addr1;
			else return No;
		}
		strcpy(buff2,pf.addr);
		strcat(buff2," ");
		strcat(buff2,buff);
		all_trim(buff2);xstrncpy(pf.addr,buff2,70);
	}
	view("frm/guest/addr.end");
	return Yes;
}

gg_tele()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	_tele:
	if (count++>8)	return No;
	view("frm/guest/tele.scr");
	lineinput(buff,20);check_han(buff);printf("\n");
	if (strlen(buff)==0) {
		strcpy(pf.tele,"");
		return Yes;
	}
	if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
	if (strlen(buff)<6) {
		if (yesno3("frm/guest/tele1.err",Yes)) goto _tele;
		else return No;
	}
	strcpy(pf.tele,buff);
	return Yes;
}

gg_add2()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	_addr:
	if (yesno3("frm/guest/addr3.scr",No)==No) {
		strcpy(pf.add2,pf.addr);
		strcpy(pf.pos2,pf.pos1);
		return Yes;
	}
	_addr3:
	if (count++>8)	return No;
	strcpy(g_buff2,pf.pos2);
	strcpy(g_buff3,pf.add2);
	view("frm/guest/addr4.scr");
	lineinput(buff,14);check_han(buff);printf("\n");
	if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
	if (strlen(buff)<4) {
		if (yesno3("frm/guest/addr4.err",Yes)) goto _addr;
		else return No;
	}
	strcpy(g_buff1,buff);
	i=check_selectaddr(buff,buff2);
	if (i==2) goto _addr3;
	if (i==1) {view("frm/guest/addr5.err");goto _addr3;}
	buff[70]=0;

	strcpy(pf.add2 ,buff );
	strcpy(pf.pos2 ,buff2);

	count=0;
	_addr4:
	if (count++>8)	return No;
	if (bit(cfg.guestin,25)==0) {
		strcpy(g_buff2,pf.pos2);
		strcpy(g_buff3,"");
		view("frm/guest/addr5.scr");
		strcpy(buff,pf.add2);
		dosinput(buff,70);printf("\n");all_trim(buff);
		if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
		if (strlen(buff)<4) {
			if (yesno3("frm/guest/addr6.err",Yes)) goto _addr3;
			else return No;
		}
		all_trim(buff);xstrncpy(pf.add2,buff,70);
	}
	else {
		strcpy(g_buff2,pf.pos2);
		strcpy(g_buff3,pf.add2);
		view("frm/guest/addr5.scr");
		lineinput(buff,50);check_han(buff);printf("\n");
		if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
		if (strlen(buff)<4) {
			if (yesno3("frm/guest/addr6.err",Yes)) goto _addr3;
			else return No;
		}
		strcpy(buff2,pf.add2);
		strcat(buff2," ");
		strcat(buff2,buff);
		all_trim(buff2);xstrncpy(pf.add2,buff2,70);
	}
	view("frm/guest/add2.end");
	return Yes;
}

gg_bibi()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	_bibi:
	if (count++>8)	return No;
	view("frm/guest/bibi.scr");
	lineinput(buff,14);check_han(buff);printf("\n");
	if (strlen(buff)==0) {
		strcpy(pf.bibi,"");
		return Yes;
	}
	if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
	if (check_badbibi(buff)) {
		if (yesno3("frm/guest/bibi1.err",Yes)) goto _bibi;
		strcpy(buff,"");
	}
	strcpy(pf.bibi,buff);
	return Yes;
}

gg_hand()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	_hand:
	if (count++>8)	return No;
	view("frm/guest/hand.scr");
	lineinput(buff,14);check_han(buff);printf("\n");
	if (strlen(buff)==0) {
		strcpy(pf.hand,"");
		return Yes;
	}
	if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
	if (check_badhand(buff)) {
		if (yesno3("frm/guest/hand1.err",Yes)) goto _hand;
		strcpy(buff,"");
	}
	strcpy(pf.hand,buff);
	return Yes;
}


gg_job()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	_job:
	if (count++>8)	return No;
	view("frm/guest/job.scr");
	lineinput(buff,2);check_han(buff);printf("\n");check_han(buff);
	if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
	n=atoi(buff);
	if (n<1||n>19) {
		if (yesno3("frm/guest/job1.err",Yes)) goto _job;
		strcpy(buff,"");
		return No;
	}
	if (n<1||n>19) n=1;
	pf.job=n;
	return Yes;
}


gg_offi()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	_offi:
	if (count++>8)	return No;
	view("frm/guest/offi.scr");
	lineinput(buff,70);check_han(buff);printf("\n");
	if (strlen(buff)==0) {
		strcpy(pf.offi,"");
		return Yes;
	}
	if (strlen(buff)==1&&is_char("NnQqPpXx",buff[0])) return No;
	if (strlen(buff)<4) {
		if (yesno3("frm/guest/offi1.err",Yes)) goto _offi;
		else return No;
	}
	strcpy(pf.offi,buff);
	return Yes;
}

gg_otel()
{
	int count,i,n,ret;
	char buff[100],buff2[100];
	count=0;
	_otel:
	if (count++>8)	return No;
	view("frm/guest/otel.scr");
	lineinput(buff,20);check_han(buff);printf("\n");
	if (strlen(buff)==0) {
		strcpy(pf.otel,"");
		return Yes;
	}
	if (strlen(buff)<6) {
		if (yesno3("frm/guest/otel1.err",Yes)) goto _otel;
		else return No;
	}
	strcpy(pf.otel,buff);
	return Yes;
}

gg_hobby(int mode)
{
	view("frm/guest/memo%d.scr",mode);
	if (mode==1) get_str(pf.memo1,40);		//소개
	if (mode==2) get_str(pf.user1,8);		//추천인
	if (mode==3) get_str(pf.user2,20);		//관심분야
	if (mode==4) get_str(pf.user3,20);		//통신경력
	if (mode==5) get_str(pf.user4,40);		//취미
	printf("\n");
	return Yes;
}


save_guest()
{
	FILE *fp;
	int n,n2;
	n=inc_numfile(1,"system/idnum.dat");
	pf.idnum=n;
	c_lock(1,GUEST_FILE);
	if ((fp=fopen(GUEST_FILE,"r+"))==NULL) fp=fopen(GUEST_FILE,"w+");
	if (fp==NULL) return;
	fseekend(fp);
	n=ftell(fp)/sizeof_pf; //현재 레코드수
	strcpy(pf.nick,pf.name);
	pf.type=cfg.guesttype;
	if (pf.type<1) pf.type=1;
	pf.level=cfg.guestlevel;
	pf.down=cfg.guestdown*1000;
	pf.p_ph=cfg.guestp_ph;				//체력
	pf.p_it=cfg.guestp_it;				//지력
	pf.p_at=cfg.guestp_at;				//매력
	pf.p_pr=cfg.guestp_pr;				//방어력
	pf.p_mo=cfg.guestp_mo;				//모드
	pf.cash=cfg.guestcash;				//모드
	pf.left_time=cfg.guesttime;
	time(&pf.inday);
    pf.id_no[14]=0;
	fseek(fp,n*sizeof_pf,SEEK_SET);
	pf.num=n;
//    fwrite(&pf,sizeof_pf,1,fp);
    gwrite(165,pf,fp);
	fclose(fp);
	c_lock(0,GUEST_FILE);
	view("frm/guest/save1.log");
}

save_auto()
{
	FILE *fp;
    char buff[500],buff2[500];
	int n,n2;
	n=inc_numfile(1,"system/idnum.dat");
	pf.idnum=n;
	c_lock(1,ID_FILE);
	if ((fp=fopen(ID_FILE,"r+"))==NULL) fp=fopen(ID_FILE,"w+");
	if (fp==NULL) return;
	strcpy(pf.nick,pf.name);
	pf.type=cfg.guesttype;
	if (pf.type<1) pf.type=1;
	pf.level=cfg.guestlevel;
	pf.down=cfg.guestdown*1000;
	pf.left_time=cfg.guesttime;
	time(&pf.inday);
    pf.id_no[14]=0;
	pf.num=count_frec(fp,sizeof_pf);
	fseek(fp,pf.num*sizeof_pf,SEEK_SET);
//    fwrite(&pf,sizeof_pf,1,fp);
    gwrite(165,pf,fp);
	fclose(fp);
	c_lock(0,ID_FILE);
	printf(" ### 가입처리중....\n");
    make_ind(0,pf.id);
    sprintf(buff,"user/rec/guestbbs.rec");
    sprintf(buff2,"%6d %s %-8s %-8s %s %s %s\n",pf.num,now_str(2),pf.id,pf.name,pf.id_no,pf.tele,pf.addr);
    put_line(buff2,buff);
	view("frm/guest/save2.log");
	if (bit(cfg.guestmode,25)||bit(cfg.guestmode,26)) make_epass();
}

make_epass()
{
	FILE *fp;
	char buff[300];
	char buff2[300];
	char abuff1[100],abuff2[100];
	char filename[300];
	int log_found;
	unsigned int eenum;
	log_found=view("frm/system/epass.log");
	if (log_found) {
		xy(20,10);lineinput(abuff1,60);
		xy(22,10);lineinput(abuff2,60);
	}
	strcpy(filename,"system/epass.num");
	file_line(buff,filename,1);
	eenum=(unsigned int)atoi(buff);
	if (eenum>0) {
		strcpy(buff,pf.id_no);
		buff[6]=0;
		eenum=eenum+(unsigned int)atoi(buff);
	}
	else eenum=rand();

	sprintf(buff,"user/epass/%s.dat",pf.id);
	if ((fp=fopen(buff,"w"))!=NULL) {
		sprintf(buff2,"%12u %-8s %-8s %s",eenum,pf.id,pf.name,now_str(2),abuff1,abuff2);
		fprintf(fp,"%s\n",buff2);
		if (log_found) fprintf(fp,"        %s\n        %s\n",abuff1,abuff2);
		fclose(fp);
	}
	sprintf(buff,"user/epass/%s.lst",now_str(12));
	if ((fp=fopen(buff,"a+"))==NULL) fp=fopen(buff,"w+");
	if (fp!=NULL) {
		fprintf(fp,"%s\n",buff2);
		if (log_found) fprintf(fp,"        %s\n        %s\n",abuff1,abuff2);
		fclose(fp);
	}
}



wait_free()
{
	char buff[200];
    int n,n1,i,i1,i2,count,all_yes=No;
	FILE *fp,*fp1,*fp2,*fp3;

	findport(tty);
	read_cfg();
	set_tmpfile();

	touch_file(GUEST_FILE);
	touch_file(GUESTNOT_FILE);
	touch_file(GUESTOUT_FILE);
	touch_file(ID_FILE);
	unlink(TMP_FILE);
	unlink(TMP_FILE1);
	unlink(TMP_FILE2);

	n=count_rec(GUEST_FILE,sizeof_pf);
	if ((fp1=fopen(GUEST_FILE,"r+"))!=NULL) {
		fseekend(fp1);
		n=ftell(fp1)/sizeof_pf;
	}
	if (fp1==NULL||n<1) {
		printf("가입신청대기중인 회원이 없습니다.\n");
		pressenter();
		return;
	}

	n1=0;
	count=0;
	while (1) {
		allmode=set_bit(allmode,7,1);
		n=count_frec(fp1,sizeof_pf);
		fseek(fp1,n1*sizeof_pf,SEEK_SET);
		if (fread(&pf,sizeof_pf,1,fp1)<1) break;
		n1++;
        if (strlen(pf.id)<3) continue;
		sprintf(g_buff1,"%d",n1);
		sprintf(g_buff2,"%d",n);

        //가입허가(Y) 전부가입(YY) 불허가(N)  거부등록(S)  보류(Enter) 작업중지(Q)
		cont2:
		view("frm/guest/waitpf.scr");
        printf("  가입허가(Y) 전부가입(YY) 불허가(N)  거부등록(S)  보류(Enter) 작업중지(Q)\n");
        printf("  선택>>");
        if (all_yes) printf("Yes\n");
        else {lineinput(buff,2);check_han(buff);printf("\n");}
        if (is_same(buff,"YY")) all_yes=Yes;
        if (all_yes||is_same(buff,"Y")) {       //아이디로 저장
			c_lock(1,ID_FILE);
			if ((fp2=fopen(ID_FILE,"r+"))==NULL) {
				mess("%s 화일을 열수 없습니다.",ID_FILE);
				fclose (fp1);
				return;
			}
			pf.num=count_frec(fp2,sizeof_pf);
			pf.type=cfg.guesttype;
			if (pf.type<0) pf.type=1;
			pf.level=cfg.guestlevel;
			pf.down=cfg.guestdown*1000;
			pf.left_time=cfg.guesttime;
			time(&pf.inday);
            pf.id_no[14]=0;
			fseek(fp2,pf.num*sizeof_pf,SEEK_SET);
//            fwrite(&pf,sizeof_pf,1,fp2);
            gwrite(165,pf,fp2);
			fclose (fp2);
			c_lock(0,ID_FILE);
			count++;
            printf(" %d. %-8s %-8s 님이 등급%d 로 가입되었습니다.\n",count,pf.id,pf.name,pf.type);
		}
		else if (is_same(buff,"S")) {            //거부등록
			if (!save_guestnot(pf,1)) goto cont2;
            printf(" %d. %-8s %-8s 님이 등록거부되었습니다.\n",count,pf.id,pf.name,pf.type);
		}
        else if (is_same(buff,"N")) {
            printf(" %d. %-8s %-8s 님이 불허가되었습니다.\n",count,pf.id,pf.name,pf.type);
            continue;    //통과 (거부)
        }
        else if (is_same(buff,"Q")||is_same(buff,"x")||is_same(buff,"xx")) {
			if ((fp3=fopen(TMP_FILE1,"r+"))==NULL) fp3=fopen(TMP_FILE1,"w+");   //기존아이디에 추가한다.
			if (fp3!=NULL) {
				fseekend(fp3);
				i=ftell(fp3)/sizeof_pf;
				if (i>=0) {
					fseek(fp1,n1*sizeof_pf,SEEK_SET);
					while (fread(&pf,sizeof_pf,1,fp1)) {
						pf.num=i;
						fseek(fp3,i*sizeof_pf,SEEK_SET);
                        pf.id_no[14]=0;
//                        fwrite(&pf,sizeof_pf,1,fp3);  //보류로
                        gwrite(166,pf,fp3);
						i++;
					}
				}
				fclose (fp3);
			}
			break;
		}
		else if (strlen(buff)==0) { 	 //보류
			if ((fp3=fopen(TMP_FILE1,"r+"))==NULL) fp3=fopen(TMP_FILE1,"w+");   //기존아이디에 추가한다.
			if (fp3!=NULL) {
				fseekend(fp3);
				i=ftell(fp3)/sizeof_pf;
				if (i>=0) {
					fseek(fp3,i*sizeof_pf,SEEK_SET);
					pf.num=i;
                    pf.id_no[14]=0;
//                    fwrite(&pf,sizeof_pf,1,fp3);  //보류
                    gwrite(166,pf,fp3);
				}
				fclose (fp3);
			}
            printf(" %d. %-8s %-8s 님이 보류되었습니다.\n",count,pf.id,pf.name);
		}
	}
	fclose (fp1);
	unlink(GUEST_FILE);
	if (is_file(TMP_FILE1)) {
		file_move(TMP_FILE1,GUEST_FILE); //새로운 신청화일
	}
	if (count>0) {
		printf(" ## 총 %d명이 가입되었습니다.\n 인덱스를 재정리 할까요?(Y/n)>>",count);
        if (yesno("",Yes)) make_ind(0,"make_ind_all");
	}
}


wait_free2()
{
	char buff[200];
    int nn,n1,i,i1,i2,count;
	FILE *fp,*fp1,*fp2,*fp3;

	findport(tty);
	read_cfg();
	set_tmpfile();

	touch_file(GUEST_FILE);
	touch_file(GUESTNOT_FILE);
	touch_file(GUESTOUT_FILE);
	touch_file(ID_FILE);
	unlink(TMP_FILE);
	unlink(TMP_FILE1);
	unlink(TMP_FILE2);


	n1=0;
	count=0;
	while (1) {
        if ((fp1=fopen(GUEST_FILE,"r"))==NULL) {mess("가입신청대기중인 회원이 없습니다.");return;}
        nn=0;
        while(fread(&pf,sizeof_pf,1,fp1)) {
            if (strlen(pf.id)>2) nn++;
        }
        fclose(fp1);
        printf(" 현재 %d 명이 가입대기중입니다.\n 가입처리할 ID 또는 이름은? >>",nn);
        lineinput2(buff,10);check_han(buff);
        if (is_quit(buff)) break;
        if (strlen(buff)<3) continue;

        n1=0;
        fp1=fopen(GUEST_FILE,"r+");
        while(1) {
            fseek(fp1,n1*sizeof_pf,SEEK_SET);
            if (!fread(&pf,sizeof_pf,1,fp1)) break;
            if (stristr(pf.id,buff)||stristr(pf.name,buff)) {
                sprintf(g_buff1,"%d",n1);
                sprintf(g_buff2,"%d",nn);
                view("frm/guest/waitpf.scr");
                if (yesno2(" 가입허가(y/N)>>",No)) {
                    if ((fp2=fopen(ID_FILE,"r+"))==NULL) {
                        mess("%s 화일을 열수 없습니다.",ID_FILE);
                        fclose (fp1);
                        return;
                    }
                    pf.num=count_frec(fp2,sizeof_pf);
                    pf.type=cfg.guesttype;
                    if (pf.type<0) pf.type=1;
                    pf.level=cfg.guestlevel;
                    pf.down=cfg.guestdown*1000;
                    pf.left_time=cfg.guesttime;
                    time(&pf.inday);
                    pf.id_no[14]=0;
                    fseek(fp2,pf.num*sizeof_pf,SEEK_SET);
                    gwrite(165,pf,fp2);
                    fclose (fp2);
                    count++;
                    printf(" %d. %-8s %-8s 님이 등급%d 로 가입되었습니다.\n",count,pf.id,pf.name,pf.type);
                    fseek(fp1,n1*sizeof_pf,SEEK_SET);
                    strcpy(pf.id,"");strcpy(pf.name,"");strcpy(pf.id_no,"");
                    fwrite(&pf,sizeof_pf,1,fp1);
                }
            }
            n1++;
        }
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
		printf("\n 등록거부등록 사유 (취소:Q)>>");
		lineinput(buff,40);check_han(buff);printf("\n");
		if (is_same(buff,"q")) {
			mess("거부처리가 취소 되었습니다.\n");
			return Yes;
		}
		strcpy(pf.user4,buff);
	}
	fp=fopen(filename,"r+");   //거부자아이디에 추가한다.
	if (fp==NULL) fp=fopen(filename,"w+");   //기존아이디에 추가한다.
	if (fp==NULL) return Yes;
	fseekend(fp);
	i=ftell(fp)/sizeof_pf;
	if (i>=0) {
		pf2.num=i;
		fseek(fp,i*sizeof_pf,SEEK_SET);
//        fwrite(&pf2,sizeof_pf,1,fp);
        gwrite(167,pf2,fp);
	}
	fclose (fp);
	return No;
}


// 회원인덱스 정보이다.
// 0	1	  2    3		4
//번호 지역1 지역2 나이시작  나이종결
typedef struct {
	char addr1[21];
	char addr2[21];
	int  age1;
	int  age2;
	int  num;
} LOCAL;


reset_pf_local()
{
	FILE *fp;
	LOCAL alocal[100];
	int i,n,nn,i2,n2,rec;
	char buff[300],buff1[300];
	char mms[10][300];
	pf.localmenu=0;

	for (i=0;i<100;i++) {
		strcpy(alocal[i].addr1,"");strcpy(alocal[i].addr2,"");
		alocal[i].age1=alocal[i].age2=alocal[i].num=0;
	}
	if ((fp=fopen("system/local.txt","r"))==NULL) return;
	nn=0;
	while (nn<100&&fgets(buff1,200,fp)!=NULL) {
		if (strlen(buff1)<10||buff1[0]=='#'||buff1[1]==';') continue;
		for (i=0;i<6;i++) bzero(mms[i],200);
		sscanf(buff1,"%s%s%s%s%s%s",mms[0],mms[1],mms[2],mms[3],mms[4],mms[5]);
		if (strlen(mms[1])<=1&&strlen(mms[2])<=1&&atoi(mms[3])<1&&atoi(mms[4])<1) continue;
		if (strlen(mms[1])>1) sprintf(alocal[nn].addr1,"**%-15.15s",mms[1]);
		if (strlen(mms[2])>1) sprintf(alocal[nn].addr2,"%-20.20s",mms[2]);
		del_space(alocal[nn].addr1);del_space(alocal[nn].addr2);
		alocal[nn].age1=atoi(mms[3]);alocal[nn].age2=atoi(mms[4]);
		alocal[nn].num=atoi(mms[0]);
		nn++;
	}
	fclose(fp);
	if (nn<=0) return;

	sprintf(buff1,"**%s",pf.addr);del_space(buff1);
	i2=get_age(pf);
	for (i=0;i<nn;i++) {
		if (strlen(alocal[i].addr1)>2&&!stristr(buff1,alocal[i].addr1)) continue;
		if (strlen(alocal[i].addr2)>0&&!stristr(pf.addr,alocal[i].addr2)) continue;
		if (alocal[i].age1>0&&i2<alocal[i].age1) continue;
		if (alocal[i].age2>0&&i2>alocal[i].age2) continue;
		pf.localmenu=alocal[i].num;
		return;
	}
	return;
}
