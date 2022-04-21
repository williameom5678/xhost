////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 이 소스를 변경하여 상용으로 사용하실때에는  개발자의 승인을 받아야 합니다.
// 내용 : 각종 기능 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include "chat.h"


bbs_find(char *buff)
{
	int n=0;
	char buff1[100];
	char buff2[100];
	int find_mode=No;
	strcpy(buff1,buff);
	re_print=Yes;
	pf.todaymode=set_bit(pf.todaymode,9,1);   //오늘작업
	while (1) {
		if (buff1[0]==0)  {
			find_mode=Yes;
			if (!view("frm/system/find.scr")) {
				printf("\n");
				printfL2();
				printf("       호스트 통합 검색 메뉴\n");
				printfL2();
				printf("  1. 게시판 검색       :게시판 전체 에서 특정 조건 검색\n");
				printf("  2. 자료실 검색       :자료실 전체 에서 특정 조건 검색\n");
				printf("  3. 전체 내용 검색    :전체 내용에서 특정 문자열 검색\n");
				printf("  4. 게시물 현황 검색  :게시물/자료실 전체 현황 조회\n");
				printf("  5. 우수회원    검색  \n");
				printf("  6. 회원        검색  \n");
				printf("  7. 직장/학교   검색  \n");
				printf("  8. 취미        검색  \n");
				if (club_mode) printf("  7. 동호회 회원 검색  \n");
				printfL2();
				printf("  선택 >>");
			}
			n=numinput();
			if (n==0) return;
			if (n==1) strcpy(buff1,"findbbs");
			if (n==2) strcpy(buff1,"findpds");
			if (n==3) strcpy(buff1,"lst");
			if (n==4) strcpy(buff1,"bbs_stat");
			if (n==5) strcpy(buff1,"best");
			if (n==6) strcpy(buff1,"findpf");
			if (n==7) strcpy(buff1,"findpf2");
			if (n==8) strcpy(buff1,"findpf3");
			if (club_mode&&n==6) strcpy(buff1,"findcpf");
		}

		if (buff1[0]==0)  return;
		else if (is_same(buff1,"bbs_stat"))  {
			sprintf(buff2,"bin/bbs_stat %s",club_go.code) ;
			sys_tem(buff2) ; //화일찾기
		}
		else if (is_same(buff1,"best")) {
			sprintf(buff2,"bin/findbest %s",club_go.code) ;
			sys_tem(buff2) ; //우수회원찾기
		}
		else if (is_same(buff1,"findpds")||is_same(buff1,"ff")||
				 is_same(buff1,"fp")||is_same(buff1,"findfile")) {
			sprintf(buff2,"bin/findbbs 1 %s",club_go.code) ; //화일찾기
			set_stat2(19,buff2);	 //작업
			sys_tem(buff2) ; //화일찾기
		}
		else if (is_same(buff1,"findbbs")||is_same(buff1,"fb")) {
			sprintf(buff2,"bin/findbbs 2 %s",club_go.code) ; //화일찾기
			set_stat2(19,buff2);	 //작업
			sys_tem(buff2) ; //화일찾기
		}
		else if (is_same(buff1,"pf")||is_same(buff1,"findpf")||is_same(buff1,"findpf1")||is_same(buff1,"findpf2")||is_same(buff1,"findpf3")) {
				 if (is_same(buff1,"findpf"))  sprintf(buff2,"bin/pf '%s' set_userfind",pf.id);
				 if (is_same(buff1,"findpf1")) sprintf(buff2,"bin/pf '%s' set_userfind",pf.id);
			else if (is_same(buff1,"findpf2")) sprintf(buff2,"bin/pf '%s' set_userfind2",pf.id);
			else if (is_same(buff1,"findpf3")) sprintf(buff2,"bin/pf '%s' set_userfind3",pf.id);
			set_stat2(19,buff2);	 //작업
			sys_tem(buff2) ; //화일찾기
		} //검색
		strcpy(buff1,"");
		if (n==0||!find_mode) break;
	}
}


//화면에 삭제된 계시물 보이는가?
//1:Yes 2:No
set_ddmode(int mode)
{
	re_print=Yes;
	allmode2=set_bit(allmode2,1,0);
	if (mode==1) mmn[1]=Yes;
	else if (mode==2) mmn[1]=No;
	else if (mode==3) {allmode2=set_bit(allmode2,1,1);mmn[1]=Yes;}
	else if (mmc[1][0]==0) {
		e_mess("ddmuse",Yes);
		return;
	}

	if (mmn[1]==0) {
		dd_mode=No;
		message("삭제된 게시물은 보이지 않습니다.");
	}
	else {
		dd_mode=Yes;
		message("삭제된 게시물도 보입니다.");
	}
}


//화면출력모드
set_off(int mode)
{
	int n,oldmode=pf.mymode;
	char buff[100];
	pf.mymode=set_bit(pf.mymode,mode,bit(pf.mymode,mode)?0:1);
	if (!guest_mode&&read_pf4(pf.id)) {
		pf2.mymode=pf.mymode;
		if (pf2.type>0) save_pf2(4);
	}
	re_print=Yes;
	strcpy(buff,"");
	if (mode== 2) strcpy(buff,rbit(pf.mymode, 2,"로고보임","로고안보임"));
	if (mode== 4) strcpy(buff,rbit(pf.mymode, 4,"TAB변환ON","TAB변환OFF")); //settab
	if (mode== 6) strcpy(buff,rbit(pf.mymode, 6,"안시보임","안시안보임")); //setansi
	if (mode== 7) strcpy(buff,rbit(pf.mymode, 7,"로고보임","로고안보임")); //setjob
	if (mode== 8) strcpy(buff,rbit(pf.mymode, 8,"광고ON","광고OFF"));       //setani
	if (mode==11) strcpy(buff,rbit(pf.mymode,11,"라인유지","자동줄넘김")); //setline
	if (oldmode!=pf.mymode&&strlen(buff)>0) {
		printf("\n ### 설정변경  %s ###\n",buff);fflush(stdout);
		sleep(1);
	}
}


xvote(int mode)
{
	char buff[200];
	if (mode==0) {
		if (find_go("xvote")) {
			sprintf(buff,"bin/xvote '%s' '%s' '%s' ...",club_go.code,pf.id,bcfg.man1);
			sys_tem(buff);
		}
		else mess("xvote가 메뉴에 없습니다.");
	}
	else {
		sprintf(buff,"bin/svote '%s' '%s' ...",club_go.code,pf.id);
		sys_tem(buff);
	}
	re_print=Yes;
}

//손님가입메뉴
do_guest()
{
	char buff[100];
	int n,mode=0;
	re_print=Yes;
	go_top(0);
	go_code("guest");
	if (!is_same(go.code,"guest")) return No;
	if (no_exec_port("guest")) return No;
	if (bit(cfg.guestmode,14)) return No;  //가입불가능
	if (bit(cfg.infomode,9)||guest_mode||pf.type>=14) {
		set_stat2(18,"/guest");    //외부
		set_rec(1,18);
		mode=atoi(go.wtimes);
		view("frm/guest/guestwai.log");
		sprintf(buff,"%d",mode);
		n=sys_tem("bin/guest %d",mode);
//		  n=sys_exec(1,30*60,"bin/guest","guest",buff,"","");
//		  set_signal();  //시그날 재설정
		set_stat(1,"");
		if (bit(cfg.mode,28)==0) set_alarm(1);
		if (n==1) ;
		else if (n==2) log_rc(0);
		else if (n==3) host_end(0);
		else if (bit(cfg.guestmode,4)) log_rc(0);
		else if (bit(cfg.guestmode,3)) host_end(0);
	}
	else message("손님만 가능합니다");
	go_prev();
	re_print=Yes;
	return (Yes);
}



//동호회 손님가입메뉴
//동호회의 회원자료에서 타입이 1로 등록한다.
// 0:거부 1:손님 2:대기 3:회원
do_cguest()
{
	int oldtype=cpf.type;
	if (!club_mode) return;
	sys_tem("bin/cguest '%s' '%s'",club_go.code,pf.id);
//	  set_signal();  //시그날 재설정
	if (go.type=='r') go_prev();
	check_cpf();			 //등급읽기 -> now_type
	if (oldtype==0&&cpf.type!=oldtype) re_club=Yes;
	re_print=Yes;
}


//가입해지신청을 함
do_pfout()
{
	char buff[100];
	if (guest_mode||bit(allmode,3)) return;
	if (bit(cfg.mode,18)) return;
	sys_tem("bin/ed_pf '%s' '%s' 99",pf.id,pf.id);
//	  sys_exec(1,30*60,"bin/ed_pf","ed_pf",pf.id,pf.id,"99");
//	  set_signal();  //시그날 재설정
	re_print=Yes;
}


do_edid()
{
	if (guest_mode) return;
	sys_tem("bin/ed_pf '%s' edit_id_ok",pf.id);
	if (!read_pf4(pf.id)) strcpy(auto_cmd,"rc");
}

// 문서를 보여주는 명령
run_pagescreen()
{
	char buff[200];
	if (go.type!='p'&&go.type!='h') return No;
	if (stristr(go.wtimes,"dn")) {
		auto_page_down();
		return Yes;
	}
	cls();
	sprintf(buff,"%s/logo/%s.mnu",club_go.dir,go.code);
	if (go.type=='h') view(buff);
	else			  view_text(buff);
	return Yes;
}


// 파일 자동다운
auto_page_down()
{
	char filename[200];
	char filename2[100];
	char buff[200];
	char buff2[200];
	char buff3[200];
	strcpy(filename,go.dir);
	str_filedir(filename,buff,filename2);
	strcpy(g_buff1,buff);
	strcpy(g_buff2,filename2);
	a_num[1]=file_size(filename);
	strcpy(bbs.filename,filename2);
	bbs.filesize=a_num[1];
	strcpy(g_buff3,byte2str(bbs.filesize));

	if (!view("%s/logo/%s.mnu",club_go.dir,go.code))
		printf("\n %s 를 전송할까요?(Y/n)>>",filename);
	if (!yesno2("",Yes)) return;
	if (!stristr(filename,".zip")&&
		!stristr(filename,".arj")&&
		!stristr(filename,".rar")&&
		!stristr(filename,".tgz")&&
		!stristr(filename,".cap")&&
		!stristr(filename,".exe")&&
		!stristr(filename,".txt")) {
		mess("%s 압축 파일이 아닙니다.",filename);
		return;
	}
	if (!is_file(filename)||stristr(filename,"system/")||
		stristr(filename,"user/")||
		stristr(filename,"etc/")||
		stristr(filename,"bin/")) {
		mess("%s 파일이 없습니다.",filename);
		return;
	}
	sz_file(filename);
	if (!is_char(go.wtimes,'x')) {  //기록안함
		sprintf(buff2,"AUD %s",filename2);
		save_rec(1,8,buff2);
	}
}



//게인 메모장
per_memo()
{
	char filename[100];
	char buff[10];
	int n;

	re_print=Yes;
	if (guest_mode||pf.type<1) {
		view("frm/error/noguest.err");
		return;
	}
	view("frm/pf/pm.log");
	sprintf(filename,"userdata/%s/%s/pmemo.txt",c_han(pf.id),pf.id);

	re_print=Yes;
	while (1) {
		if (!view("frm/pf/pm.cmd")) {
			printf("\n 1. 확인\n 2. 등록\n 3. 편집\n 4. 전송\n 선택>>");
		}
		lineinput(buff,5);printf("\n");check_han(buff);
		n=atoi(buff);
		if (is_quit(buff)) break;
		else if (n==1) {
			printf("\n");
			printfL2();
			view_text(filename);
			printf("\n");
		}
		else if (n==2) {
			if (input_tmp(0)) { 		   //내용을 입력받는다.
				file_copy(TMP_FILE,filename);
				unlink(TMP_FILE);
			}
		}
		else if (n==3) xedit(filename);
		else if (n==4) sz_file(filename);
	}
}


//is_login	해당되는 아이디가 접속중인가?
//return 0: 없음  1:아이디 있음  2:메세지수신거부
is_login(char *buff)
{
	FILE *fp1;
	char mess[80];
	char lasttime[80];
	int ret;
	PF pf2;
	struct tm *tp;
	MONI moni;

	if (buff[0]==0) return 0;
	ret=is_moni(buff);		//접속중인가?

	if ((fp1=fopen("id/id.dat","r"))!=NULL) {
		while(fread(&pf2,sizeof_pf,1,fp1))
			if(is_same(pf2.id,buff)==Yes) {
				if(ret==0) tp = localtime(&pf2.logout);
				else	   tp = localtime(&pf2.login) ;
				sprintf(lasttime,"%02d/%02d/%02d %02d:%02d",
					   tp->tm_year,tp->tm_mon+1,tp->tm_mday,
					   tp->tm_hour,tp->tm_min);
				break;
			}
		fclose(fp1);
	}
	if(ret==0) sprintf(mess,"%s 님은 %s 접속종료",buff,lasttime);
	if(ret!=0) sprintf(mess,"%s 님은 %s부터 접속중",buff,lasttime);
	message(mess);
	return ret;
}


set_mymoni(char *buffstr)
{
	char buff[300];
	buff2cmd(buffstr,buff,1);  //메세지를 같이 입력함
	if (strlen(buff)>1) {
		strcpy(mymoni,buff);
	}
	else {
		printf("\n 현재 US의 설명으로 지정할 내용을 한글 10자 이내로 적어주세요.\n>>");
		lineinput(mymoni,20);printf("\n");
	}
	if (strlen(mymoni)<1) printf(" ### US 설명이 삭제되었습니다.\n");
	else printf(" ### 설명지정:'%s'\n",mymoni);
	set_stat(1,"");
}


//상태기록 화일 저장
//각 tty별로 저장한다.
set_stat(int mode,char *buffstr)
{
	char buff[180],buff2[180],filename[100];
	FILE *fp;
	int n,i,rec=0;
	MONI moni2;
	int found=No;

	strcpy(buff2,buffstr);
	strcpy(moni.hostname,hostname);
	moni.type=pf.type;
	moni.mode2=0;
	moni.hostno=cfg.hostno;

	xstrncpy(moni.what,buff2,70);
	strcpy(moni.club,club_go.code);
	strcpy(moni.gocode,go.code);
	moni.mode=mode;
	if (mode==3&&is_same(go.code,"chat")&&atoi(go.wtimes)/100==7) {
		moni.mode2=set_bit(moni.mode2,3,1);
	}
	if (dark_mode) moni.mode=99;
	if (moni.mode<1) moni.mode=1;
	moni.pfnum=pf.num;
	strcpy(moni.id,pf.id);
	strcpy(moni.name,pf.name);
	strcpy(moni.nick,pf.nick);
	moni.msg=bit(pf.mymode,9);	   //초대 메세지
	moni.pid=getpid();
	moni.login=START_TIME;		  //접속시작시간
	strcpy(moni.tty,tty);
	if (go.type=='m'&&is_same(go.code,"top")&&!club_mode) goto _cont2;
	if ( (is_char("ok",go.type)&&mode==5)||
		 (is_char(BBSTYPE,go.type)&&mode==5&&bit(bcfg.mode,15)) ) {
		_cont2:
		strcpy(moni.goname,"메인메뉴");
		strcpy(moni.gocode,"top");
		strcpy(moni.club,"main");
	}

	sprintf(filename,"user/moni/moni%d.dat",cfg.hostno);
	if ((fp=fopen(filename, "r+"))==NULL) {check_dir("user/moni");touch_file(filename);return;}
	found=No;
	if (strlen(mymoni)>1) xstrncpy(moni.goname,mymoni,20);
	else				  xstrncpy(moni.goname,go.name,20);

	i=0;
//	  printf("%s",mymoni,moni.goname);pressenter();
	fseektop(fp);
	fseek(fp,mymoni_rec*sizeof_moni,SEEK_SET);
	if (fread(&moni2,sizeof_moni,1,fp)&&
		moni2.mode>0&&
		is_same(moni2.tty,tty)) {
		fseek(fp,mymoni_rec*sizeof_moni,SEEK_SET);
		fwrite(&moni,sizeof_moni,1,fp);
		found=Yes;
	}
	if (!found) {
		fseektop(fp);
		i=0;
		while (fread(&moni2,sizeof_moni,1,fp)) {
			if (moni2.mode<1) goto _cont1;
			if (is_same(moni2.tty,tty)) {
				mymoni_rec=i;
				fseek(fp,i*sizeof_moni,SEEK_SET);
//				  mess("2 %s",moni.tty);
				fwrite(&moni,sizeof_moni,1,fp);
				found=Yes;
				//남은것은 모드 삭제처리
				i++;
				while(1) {
					fseek(fp,i*sizeof_moni,SEEK_SET);
					if (!fread(&moni2,sizeof_moni,1,fp)) break;
					if (is_same(moni2.tty,tty)) {
						moni2.mode=0;moni2.hostno=0;moni2.login=0;
						strcpy(moni2.tty,"");strcpy(moni2.id,"");
						fseek(fp,i*sizeof_moni,SEEK_SET);
						fwrite(&moni2,sizeof_moni,1,fp);
					}
					i++;
				}
				break;
			}
			_cont1:
			i++;
		}
	}
	if (!found) {
		fseektop(fp);
		i=0;
		while (fread(&moni2,sizeof_moni,1,fp)) {
			if (moni2.mode<1||
				strlen(moni2.tty)<1||
				strlen(moni2.id)<1||
				is_same(moni2.tty,tty)) {
				mymoni_rec=i;
				fseek(fp,i*sizeof_moni,SEEK_SET);
				fwrite(&moni,sizeof_moni,1,fp);
//				  mess("3 %s",moni.tty);
				found=Yes;
				break;
			}
			i++;
		}
	}
	if (!found) {
		i=count_frec(fp,sizeof_moni);
		mymoni_rec=i;
		fseek(fp,i*sizeof_moni,SEEK_SET);
		fwrite(&moni,sizeof_moni,1,fp);
//				  mess("4 %s",moni.tty);
	}
	fclose(fp);
}

//상태기록 화일 저장  현재 이름 바로 지정
set_stat2(int mode,char *buff)
{
	GO oldgo;
	char buff2[100];
	oldgo=go;
	strcpy(buff2,buff);
		 if (stristr(buff,"ranklog" )) strcpy(buff2,"최종접속");
	else if (stristr(buff,"rank"    )) strcpy(buff2,"순위확인");
	else if (stristr(buff,"bsysop"  )) strcpy(buff2,"게시판담당메뉴");
	else if (stristr(buff,"csysop"  )) strcpy(buff2,"동호회시삽메뉴");
	else if (stristr(buff,"sysop"   )) strcpy(buff2,"시삽메뉴");
	else if (stristr(buff,"ed_pf"   )) strcpy(buff2,"회원신상수정");
	else if (stristr(buff,"findname")) strcpy(buff2,"회원검색");
	else if (stristr(buff,"findbbs" )) strcpy(buff2,"자료검색");
	else if (stristr(buff,"quest"   )) strcpy(buff2,"설문지응답중");
	else if (stristr(buff,"/guest"  )) strcpy(buff2,"신규가입신청중");
	else if (stristr(buff,"/cguest" )) strcpy(buff2,"동호회가입신청중");
	else if (stristr(buff,"shell"   )) strcpy(buff2,"시스템쉘작업");
	else if (stristr(buff,"xcd"     )) strcpy(buff2,"사용자영역");
	else if (stristr(buff,"cashsend")) strcpy(buff2,"송금중");
	else							   strcpy(buff2,go.name);
	sprintf(go.name,"%-30.30s",buff2);
	set_stat(mode,buff);
	go=oldgo;
}



del_stat(char *tty) 	  //상태기록 화일 해당 포트의 접속을 해제한다.
{
	del_stat_(tty,"");
}

del_stat_id(char *tid)		 //상태기록 화일 해당 포트의 접속을 해제한다.
{
	del_stat_("",tid);
}


del_stat_(char *tty,char *tid)		 //상태기록 화일 해당 포트의 접속을 해제한다.
{
	FILE *fp;
	int i=0,n;
	MONI moni2;
	char filename[100];

	for (n=0;n<=cfg.count_server;n++) {
		if (cfg.count_server!=1&&strlen(tty)>0&&n!=cfg.hostno) continue;
		sprintf(filename,"user/moni/moni%d.dat",n);
		if ((fp=fopen(filename, "r+"))==NULL) continue;
		i=0;
		while (1) {
			fseek(fp,i*sizeof_moni,SEEK_SET);
			if (!fread(&moni2,sizeof_moni,1,fp)) break;
			if ((strlen(tty)>0&&is_same(moni2.tty,tty))||
				(strlen(tid)>0&&is_same(moni2.id ,tid))) {
				moni2.mode=0;
				moni2.hostno=0;
				moni2.login=0;
				strcpy(moni2.tty,"");
				strcpy(moni2.id,"");
				fseek(fp,i*sizeof_moni,SEEK_SET);
				fwrite(&moni2,sizeof_moni,1,fp);
			}
			i++;
		}
		fclose(fp);
	}
}


//해당 포트에서 허용되지 않는가?
no_exec_port(char *tcode) {
	FILE *fp;
	char buff[300];
	int i=0;
	int ret=No;
	if (pf.type>=17) return (ret);

	if ( (fp=fopen("system/no_exec.txt","r+"))==NULL ) return No;
	while (i++<300&&fgets(buff,200,fp)!=NULL) {
		check_cmd(buff);
		if (is_same(mmc[0],tcode)&&is_same(mmc[1],tty)) {
			printf("\n 현재 %s 포트 에서는 %s 메뉴가 허용되지 않습니다.\n",mmc[1],mmc[0]);
			pressenter();
			ret=Yes;
			break;
		}
	}
	if (fp!=NULL) fclose(fp);
	return (ret);
}


user_dir()
{
	char dir_name[400];
	char buff[400];
	if (guest_mode) return;
	if (check_type('i')) return;
	sprintf(dir_name,"roombin/%s/%s",c_han(pf.id),pf.id);
	check_dir(dir_name);
	sys_tem("bin/xcd 1 '%s' '%s'",pf.id,dir_name);
	//화일이 없으면 삭제함
	if (dir_filenum(dir_name)==0) { 	 //화일의 수 1개 있다
		sprintf(buff,"rm -rf %s",dir_name);
		sys_tem(buff);
	}
	re_print=Yes;

// xdir [아이디] [디렉토리] [모드]
// 모드 10111 : CD UP DN DD MD
}

set_filename()	 //현재화일명 설정
{
	char buff[100];
	if		(is_same(go.code,"rmail"))
		sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf.id),pf.id);
	else if (is_same(go.code,"mbox"))
		sprintf(NOW_FILE ,"userdata/%s/%s/save.tit",c_han(pf.id),pf.id);
	else if (is_same(go.code,"cmail"))
		sprintf(NOW_FILE ,"userdata/%s/%s/send.tit",c_han(pf.id),pf.id);
	else if (is_same(go.code,"wmail"))
		sprintf(NOW_FILE ,"userdata/%s/%s/send.tit",c_han(pf.id),pf.id);
	else if (is_same(go.code,"userroom"))
		sprintf(NOW_FILE ,"userdata/%s/%s/room.tit",c_han(pf.id),pf.id);
	else if (is_same(go.code,"dncart")) {
		sprintf(NOW_FILE ,"tmp/tmpa%d%s",cfg.hostno,no_spc(tty));   //호스트번호별임시화일
		sprintf(NOW_FILED,"tmp/tmpb%d%s",cfg.hostno,no_spc(tty));   //호스트번호별임시화일
		return;
	}
	else if (club_mode)
		sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
	else
		sprintf(NOW_FILE ,"menu/%s.tit",go.code);
	del_space(NOW_FILE);
	strcpy(NOW_FILED,NOW_FILE);
	strchng2(NOW_FILED,".tit",".txt");
}



edit_pf(char *tid,int who)
{
	re_print=Yes;
	pf.todaymode=set_bit(pf.todaymode,8,1);   //오늘작업
	if (pf.type<18&&!is_same(tid,pf.id)) strcpy(tid,"");
	if (strlen(tid)<3) strcpy(tid,pf.id);
	if (pf.type==0||guest_mode||bit(allmode,3)) {	  //테스트용으로 지정
		view("frm/pf/notguest.log");
		return;
	}
	else {
		sys_tem("bin/ed_pf '%s' '%s'",pf.id,tid);
		if (is_same(tid,pf.id)) read_pf3(pf.id);  //자기의 신상일때 다시 읽음
	}
}



pf_filename(char *filename,char *tid,char *ext)
{
	sprintf(filename,"userdata/%s/%s/pfinfo.%s",c_han(tid),tid,ext);
}


xed_file()
{
	FILE *fp;
	char buff[300],buff1[300],buff2[300],buff3[300],buff4[300];
	int n=1;
	int found=No;
	if (strlen(mmc[1])<1) {
		mess(" 사용법  xed 파일명");
		return No;
	}
	if ((fp=fopen("system/xed.dat","r"))==NULL) {
		mess("system/xed.dat 파일이 없습니다");
		return No;
	}
	while (fgets(buff,200,fp)!=NULL) {
		sscanf(buff,"%s%s%s%s",buff1,buff2,buff3,buff4);
		if (is_same(buff1,mmc[1])&&strlen(buff4)>0) {
			found=Yes;
			break;
		}
	}
	fclose(fp);
	if (found) {
		if (pf.type<atoi(buff1)) return No;
		if (strlen(buff3)>2&&!check_pass(buff3,8,"문서 암호 >>")) return No;
		printf("\n  파일명:%s(%d B)  편집 1.편집기 2.Zmodem >> ",buff4,file_size(buff4));
		n=numinput();
		if (n==2) up_text(1,buff4);  //다운받아서지정화일로 바꾼다.
		else	  xedit(buff4);
		printf("\n  파일명:%s  크기:%d B \n",buff4,file_size(buff4));
		pressenter();
	}
	return (found);
}

