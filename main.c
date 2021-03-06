
////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 내용 : 메인 프로그램 main.c
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
	allmode=0;					 //전체모드
	allmode2=0; 				 //전체모드
	if (argc>1&&is_same(argv[1],"/ver")) {
		printf("\n    VER:%s %s Date:%s Build:%s\n  ",VER1,VER2,MAKEDATE,MAKENUM);
		fflush(stdout);msleep(5);fflush(stdout);
		return;
	}
	read_cfg(); 				//config 읽기

	cfg.waitkey=5;
	cfg.hostno=0;
	_last2=1;;;;;;;;;;;;;;;;;;;;;;;;
	allmode=limit_mode=0;
	findport(tty); //접속포트 찾기
	sprintf(buff,"/dev/%s",tty);chmod(buff,0666);
	set_signal();
	if (argc>1&&strlen(argv[1])>2&&stristr(argv[1],"/")) {
		ch_dir(argv[1]);
	}
	strcpy(buff2,argv[0]);
	pure_name(2,buff2);   //화일명에서 확장자 제외 뽑아냄
	if (strlen(buff2)<1) {
		strcpy(buff2,argv[0]);
		pure_name(0,buff2);   //화일명에서 확장자 제외 뽑아냄
	}
	n=0;
	strchng2(buff2,"-bbs","bbs");
         if (is_same(buff2,"xmain2")||is_same(buff2,"xmain9")) {n=1;allmode2=set_bit(allmode2,11,1);ch_dir("/xhost");}
    else if (is_same(buff2,"xmain" )||is_same(buff2,"xmain7")) {n=1;allmode2=set_bit(allmode2,13,1);ch_dir("/bbs");}
	else if (stristr(buff2,"bbs")) {        //bbs가 실제화일명에 포함되었는가?
		sprintf(buff,"/bbs/%s",buff2);
		ch_dir(buff);
		allmode=set_bit(allmode,19,1);
	}

	xstrncpy(hostname,view_network(tty),20);
	if (n==1&&argc>2&&is_same(argv[1],"-h")) {
		strcpy(hostname,argv[2]);
		if (strlen(hostname)>0&&is_char(hostname,'.')) {
			allmode2=set_bit(allmode2,10,1); //HOSTNO를 파라메타로 넘김
		  //12991 pts/58   S	  0:00 /bbs/bin/xmain -h 203.254.78.211 -p
		}
	}

	if (bit(cfg.mode,28)==0) set_alarm(1);

	netmode=0;					 //전체모드
	pf.todaymode=0; 			   //오늘한일
	ascr_count=0;
	mymoni_rec=0;
	c_proc=No;
	c_proc2=No;
	c_proc3=No;
	autoup=No;
	c_proc =is_file("system/c_proc.tag");
	c_proc2=is_file("system/c_proc2.tag");
	c_proc3=is_file("system/c_proc3.tag");
	if (c_proc) printf("HOST 테스트중..step 1  TTY:%s\n",ttyname(1));
	CONT_DAY=0; 				//연속출력일수

	allmode=set_bit(allmode,18,is_file("system/alliprec"));
	if (c_proc) printf("HOST 테스트중..step 3\n");
	check_login_init(); 			//초기설정
	log_in();						//초기설정 터미날 설정등 세팅

	fflush(NULL);					//5

	SEND_KB=0;
	time(&START_TIME);			//접속시작시간
	time(&CLUB_START_TIME); 		 //접속시작시간
	time(&CHECK_TIME);			//접속시작시간
	time(&pf.login);			//접속시작시간
	strcpy(pf.id,"접속중");
	strcpy(pf.nick,"접속중");
	read_cfg(); 				//config 읽기

	if (is_file("filelock.tag")) cfg.mprocmode=set_bit(cfg.mprocmode,3,1);

	set_tmpfile();
	read_bbsgood_color();
	strcpy(buff,"system/notushost.txt");
	if (in_file2(buff,hostname)) strcpy(hostname,tty);
	if (c_proc) printf("HOST 테스트중..step 5\n");
	view("frm/log/loghost.log");
	//해당호스트는 기록안함
	if (in_file2("system/rec_noth.txt",hostname)) allmode=set_bit(allmode,8,1);

	srand(time(NULL));			//난수의 초기화
	now_cmd_num=rand()%20;		//현재의 도움말 번호
	DM=DM1=DM2=DM3=No;
	printf("[1;50r[;H");
	setenv("TERM","vt220",1);
	sprintf(buff,"/dev/%s",tty);chmod(buff,0666);
	del_stat(tty);			 //상태기록 화일 해당 포트의 접속을 해제한다.
	go_top(0);
	strcpy(go.name,"아이디입력중");
	sprintf(moni.goname,"%-20.20s",go.name);
	set_stat(23,"접속중");
	cashmax1=0;
	cashmax2=0;
	g_yes=No;
	allmode=set_bit(allmode,20,in_file2("system/node_net.txt",tty));
	set_guest_();
	bbsdownnum=rand()+1000;   //게시판다운번호	아디.번호

	if (!bit(cfg.infomode,0)&&!bit(cfg.infomode,5)) start_logo(0);
	/* 상태 화일에 0을 집어넣는다. 이것은 보통상태라는 것을 알리는 것이다. */
	read_atype();				//등급의 이름을 읽어옴	->atype
	read_ajob();				//직업을 읽어옴
	go_top(0);
	strcpy(club_go.code,"main");             //동호회가기코드
	strcpy(club_go.dir,".");
	guest_mode=Yes;
	club_mode=No;
	strcpy(auto_cmd,"");
	re_print=Yes;			//화면출력
	re_logo=Yes;			//보조화면출력
	re_bbs=Yes; 			//게시판로딩
	re_club=No; 			//동호회초기화
	p_command=No;			//마지막 입력이 P인가?
	bbs_ltmode=No;			//lt등으로 검사한 mode
	list_mode=No;			//현재 번호 선택모드
	dd_mode=No; 			//삭제된것보여줌
	dark_mode=0;			//시삽감춤 모드 1:일때
	is_disp_help=Yes;
	now_type=0; 				//동호회의 등급표시
	strcpy(auto_cmd,"");    //손님일때
	strcpy(DEF_UPS,"");
	strcpy(DEF_DNS,"");
	strcpy(news_code,cfg.newsfile);
	UP_LEVEL=0; 			//오늘받은 레벨
	UP_CASH =0; 			//오늘받은 캐쉬
	N_page=0;

	tag_monichar(); //현재 기록
	strcpy(last_club,club_go.code);
	if (bit(cfg.startmode,1)) goto start_now1;		  //바로 진입
	if (bit(cfg.notice,2)) view_notice(1,"notice");            //공지사항 보기

	//////////////////////ID 입력중
	if (!bit(cfg.infomode,6)) {
		if (argc>1) start_id(0,argv[1]);  //-l ID 처리
		else		start_id(0,"");       //아이디를 입력받고 초기 화면을 보여준다.
	}
	else {
		set_guest_mode2();
	}
    quit_all_room(3);        //대화방에 있는 해당ID,같은tty,포트를 제거한다.
	no_spc(pf.id);
	//해당ID는 기록안함
	i=No;
	sprintf(buff,"%s/f_nid.dat",REC_DIR);   if (i==No&&in_file2(buff,pf.id   )) allmode=set_bit(allmode,29,1);
	sprintf(buff,"%s/f_ntty.dat",REC_DIR);  if (i==No&&in_file2(buff,tty     )) allmode=set_bit(allmode,29,1);
	sprintf(buff,"%s/f_nhost.dat",REC_DIR); if (i==No&&in_file2(buff,hostname)) allmode=set_bit(allmode,29,1);
	if (in_file2("system/rec_noti.txt",pf.id))    allmode=set_bit(allmode,9,1);


    save_login(1);          //시작기록
	check_all_limit();		//전체 제한 설정
	set_rec(1,23);		// 접속시작,모드,size,참고
	sys_tem("rm -f tmp/tmp*%d%s",cfg.hostno,no_spc(tty));   //호스트번호별임시화일
	sys_tem("rm -rf tmp/up%d%s" ,cfg.hostno,no_spc(tty));   //호스트번호별임시화일
	set_stat2(23,"접속중");
	//등급18이상은 키대기 무시
	if ((bit(cfg.mode,0)&&pf.type>=18)||in_file2("system/nokey.id",pf.id))
		allmode=set_bit(allmode,13,1);

	check_wait(0);				//기다림여부 검색
	strcpy(mymoni,"");

	clear_node();
	rm_tmpup();
	if (pf.type>0) check_smemo();			   //운영진수신메모를 검색한다.
	set_pftype();  //회원의 등급,일일사용가능시간을 확인한다.

	if (!bit(cfg.infomode,7)) {
		view_loginhelp();		 //진입시 잠깐도움말 보이기
		view_notice(1,"notice");   //공지사항 보기
	}
	if (auto_cmd[0]==0&&is_gocode(pf.gofirst))
		sprintf(auto_cmd,"go %s",pf.gofirst); //자동가기코드
	auto_go_start(auto_cmd,pf.type);  //등급별 강제가기
	check_warn_mess();				  //메인의 경고인가?
	set_stat2(1,"초기화면");

	sprintf(buff,"user/fcmd/fcmd1.%s",no_spc(tty));      //강제재접속인가?
	if (is_file2(buff)) file_line(auto_cmd,buff,1);
	sprintf(buff,"user/fcmd/fcmd2.%s",pf.id);    //강제재접속인가?
	if (is_file2(buff)) file_line(auto_cmd,buff,1);

	start_now1: 	   //바로 진입
	if (bit(cfg.guestmode,14)) {ch_dir("/xguest");execl("bin/main","main",NULL);}
	for (i=0;i<20;i++) bzero(acmd[i],78);
	cmdlinenum=0;
	re_logo=Yes;
	//진입시 하드 디스크용량 확인
	if (bit(cfg.startmode,16)&&check_free()<50) {
		view("frm/system/nospace.log");
		if (pf.type<13) host_end();
	}

		 if (bit(cfg.zmode,23)) check_zbtotal(1*60*60,0);
	else if (bit(cfg.zmode,24)) check_zbtotal(6*60*60,0);

	//게시판기록관련
	strcpy(bbsrec_club,"");
	strcpy(bbsrec_code,"");
	bbsrec_time=0;
    add_logcount();      //접속기록
	if (guest_mode&&bit(cfg.guestmode,1)) strcpy(auto_cmd,"bye");
	while(1) {
		if ( is_char("mr",go.type )) check_bbsrec(1);  //게시판 기록 관련 해제
		else						 check_bbsrec(0);  //게시판 기록 관련 설정
		nostop=No;				  //멈추지 않음(로고편집시)
		if (strlen(pf.id)<3) {
			check_error(1);
			log_rc(0);
		}
		if (re_club) {
			printf(VTXOFF);
			if (club_mode) club_init();    //동호회의 초기화
			else		   main_init();    //동호회가 아닐때 다시 복구함
			re_club=No;
			re_print=Yes;
			time(&CHECK_TIME);			//접속시작시간
			tag_monichar(); //현재 기록
			continue;
		}

		if (is_char("rejnp",go.type)) {
			go_prev();
			continue;
		}
		check_another_rec();		//동호회위치변경시 기록
		if (re_logo) {
			view_pos=0;
			if (bit(cfg.mode,28)==0) set_alarm(1);
			read_bbsgood_color();
			if (check_type('i')) {
				go_prev();
				continue;
			}
			if (auto_cmd[0]==0) disp_menu_logo();	//메뉴진입시 보여주는 로고
			re_logo=No;
			re_print=Yes;
			list_mode=No;
			re_print=Yes;
		}
		strcpy(_last1,"");

		// 화면을 표시한다.
		input_0:
		if (re_print) {
			allmode2=set_bit(allmode2,2,0);  //다운,업시 시간확인금지해제
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

			set_news(Yes);			   //뉴스가능
			for (i=0;i<50;i++) list_anum[i]=0;	 //화면출력 번호
			if (pf.warn%10>=8) go_code("_warn");
			if (list_mode) set_stat(1,bbs.title);  //현재위치 저장
			else		   set_stat(1,""       );  //현재위치 저장
			if (strlen(auto_cmd)<1) disp_screen();	   //메뉴,리스트
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
		disp_mid(); 		 //No이면 도움말 출력안함 명령입력받기전 메세지 표시
		sprintf(buff,"user/fcmd/fcmd3.%s",no_spc(tty));      //강제재접속인가?
		if (is_file2(buff)) {
			file_line(auto_cmd,buff,1);
			unlink(buff);
		}
		all_trim(auto_cmd);
		if (strlen(auto_cmd)>0) {		   //자동명령처리
			del_esc(auto_cmd);
			strcpy(buff,auto_cmd);
			strcpy(auto_cmd,"");
		}
		else if (sub_go[0]!=0) {		//자동명령처리
			strcpy(buff,sub_go);
			strcpy(sub_go,"");
		}
		else {						   //명령어 입력루틴
			if (!bit(cfg.mode,4)&&!bit(pf.mymode,0)) printf(ESCENG);  //영문으로 변환 한글:h
			input_2:
			printf("7");
			//선택명령입력
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
			if (strlen(buff)<5) strchng2(buff,"? ","");
			if (strlen(buff)>0) next_acmd();

			if (is_same(buff,"ver")) {view_ver();continue;}
			if (stristr(buff,"B080")) {printf("8[K");goto input_2;}
			if (go.type=='m'&&strlen(buff)<1) {re_print=Yes;goto input_0;printf("\n");}
			set_news(No);
		}
		if (buff[0]!='_') {
			check_short(1,buff) ;				  //단축명령키 지정
			check_short(2,buff) ;				  //개인별단축명령분석
		}
		input_4:

		if (bit(cfg.mrecmode,11)&&strlen(buff)>0) cmd_record(buff);

		check_cmd(buff);					  //명령어 분해
		i=strlen(mmc[0]);

		if (i>0) {
			allmode=set_bit(allmode,16,0);	 //화면애니를 위한 처리
			allmode=set_bit(allmode,17,0);	 //키입력대기를 위한 처리
			printf("\n");
		}

		if (i<1) {						  //그냥입력시
				 if (is_bbspds&&!list_mode)  bbs_next();
			else if (is_bbspds&& list_mode)  bbs_read(1);
			else {
				goto input_1;
			}
		}
		else if (is_same(mmc[0],"go")&&mmc[2][0]=='_'&&(mmc[2][1]=='#'||mmc[2][1]=='*')) {
			if (!check_direct_go(1,buff)) {printf("\n");goto input_1;}
		}
		else if (do_menu_command()) 			  ; //메뉴상의 명령인가?
		else if (do_basic_command(buff,buff))	  ; //기본명령어인가?
		else if (do_list_command()) 			  ; //게시판의 자료번호인가?
		else if (do_order_command(mmc[0],buff))   ; //기본명령어인가?
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

	if (strlen(mmc[1])>0&&is_same(mmc[0],"해")) {strcpy(mmc[0],"go");goto _cont3;}
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
		set_filename(); 						//현재표시할 화일명
		read_bcfg();
		re_bbsnum(1);			//BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
		re_bbs=re_logo=re_print=No;
		if (check_type('i')||no_exec_port(go.code)) {
			go_top(0);
			return; //사용가능 메뉴인가?
		}
		check_bbs();		  //자료의 갯수를 파악하는 루틴
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
/// 	   동호회 관련 작업
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
					mess("동호회 진입금지할수 없는 나이입니다");
				}
			}
			club_mode=No;
			go_top(0);
			return;
		}
	}

	if (bit(limit_mode,4)) {
		if (!view("frm/pf/notclub.log")) mess("동호회금지");
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
	bbs_ltmode=No;		  //lt등으로 검사중 모드
	re_bbs=Yes;
	for (i=0;i<20;i++) strcpy(atype[i],"");
	pf.todaymode=set_bit(pf.todaymode,10,1);   //오늘작업
	newsmode[0]=newsmode[1]=newsmode[2]=newsmode[3]=0;
	_last2=8;;;;;;;;;;;;;;;;;;;;;;;;

	bzero(auto_cmd,30);
	if (!is_file2(club_go.dir)) {
		sprintf(g_buff1,"%s (%s)",club_go.code,club_go.name);
		sprintf(g_buff2,"디렉토리:%s",club_go.dir);
		if (yesno3("frm/system/noclub.log",No)) {
			view("doc/makeclub.txt");
			pressenter();
		}
        go_top(0); //동호회 상위메뉴로 이동
		return;
	}
	sprintf(buff,"%s/menustru/menu.dat",club_go.dir);
	if (!is_file2(buff)) {
		if (yesno2("동호회메뉴를 변환할까요?(Y/n)>>",Yes)) {
			strcpy(buff,"upgrade/men2_50");
			if (!is_file2(buff)) mess("%s파일이 없습니다.",buff);
			else				sys_tem("%s '%s'",buff,club_go.dir);
		}
	}

	// 동호회 진입금지 조건
	// 0. 등급이 0이 아님.
	// 1. 등급이 진입가능 등급이하
	// 2. 비공개 동호회이면서 회원이 아님
	// 3. 비공개 동호회의 경우 암호를 물어봄
	// 4. 잠시사용금지의 경우
	set_nowmenu();
	read_ccfg();
	sys_tem("touch %s/system/clubid.dat",club_go.dir);
	check_cpf();			 //등급읽기 -> now_type
	go_top(1);
	time(&CLUB_START_TIME);   //동호회 접속시작시간
	key=Yes;
	for (i=0;i<10;i++) {
		sprintf(afile[i],"%s/frm/error/whynot%d.log",club_go.dir,i);
		if (!is_file2(afile[i])) sprintf(afile[i],"frm/error/whynot%d.log",i);
	}
	if (now_type==0) {view(afile[0]);key=No;}	 //동호회 진입최소등급
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
	if (ss_check(6))   {view(afile[5]);key=No;}    //전체 동호회 진입금지
	if (check_warn(4)) {view(afile[6]);key=No;}    //동호회 사용금지
	if (key==No) {go_top(0);re_club=Yes;return;}

	read_actype();			 //동호회등급의 이름을 읽어옴  ->actype
	sprintf(buff,"등급:%d",now_type);
	start_logo(0);	//진입시 보여주는 메세지
	if (!bit(cfg.infomode,0)&&cpf.all_log==1) { 	  //동호회 처음 접속
		view("%s/frm/log/firstlo.log",club_go.dir);
		read_pf4(ccfg.sysop);
		strcpy(bbs.id,pf2.id);
		strcpy(bbs.name,pf2.name);
		sprintf(buff,"%s/frm/log/firstl1.log",club_go.dir);
		if (is_file2(buff))
			wmail4(pf.id,buff,"동호회 가입을 축하합니다.",1); //메일발송
		sprintf(buff,"%s/frm/log/firstl2.log",club_go.dir);
		if (is_file2(buff))
			wmail4(pf.id,buff,"동호회 가입을 진심으로 환영합니다.",1); //메일발송
	}
	if (strlen(sub_go)==0) view_notice(1,"notice");         //동호회 공지사항
	strcpy(mmc[1],"");
	strcpy(mmc[2],"");
	check_warn_mess();			//동호회의 경고인가?
	if (!bit(ccfg.logomode,3)&&now_type>0&&now_type<20) {
		strcpy(buff,actype[now_type]);
		del_space(buff);
		if (strlen(buff)>0) message(" %s님은 [%s]입니다.",pf.id,actype[now_type]);
	}
	set_rec(1,2);	  //동호회접속시작
	check_maxmenu();  // 동호회 최대메뉴수 확인
}


//시작시 또는 동호회에서 빠져 나올때
main_init()
{
	while (go.type=='s') go_prev2();    //동호회 선택이전으로
	now_type=0; 				//현재의 등급
	read_atype();				//등급의 이름을 읽어옴	->atype
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


// 동호회의 회원 신상을 읽고 등급을 파악한다.
check_cpf()
{
	int i;
	char buff[30];
	char buff2[180];
	int found=No;

	now_type=1;  //기본은 손님으로 정한다.
	strcpy(cpf.id,"");
	strcpy(cpf.name,"");
	cpf.level=0;
	cpf.warn=0;
	cpf.all_log=0;
	if (!club_mode) return;

	//동호회담당이거나	동호회의 시삽인경우
	if (pf.type==17)  now_type=18;
	if (pf.type>=18)  now_type=19;
	if (now_type<1)   now_type=1;
	if (now_type<=18) {
		//회원정보를 읽어온다
		if (read_cpf(pf.id)) {
			if (cpf.type==0||cpf.type>now_type) now_type=cpf.type;
			strcpy(cpf.id  ,pf.id  );		//이름변경시 자동으로 올림
			strcpy(cpf.name,pf.name);		//이름변경시 자동으로 올림
			time(&cpf.logout);				//동호회 접속 시간 적기
			cpf.all_log++;				//동호회 접속 횟수
			if (bit(ccfg.mode,1)) cpf.type=now_type=pf.type;
			if (is_same(ccfg.sysop,pf.id)) cpf.type=19;
			save_cpf(pf.id);				//회원자료 적기
		}
	}
	if (now_type>19) now_type=19;
	if (now_type<19&&bit(ccfg.mode,1)) now_type=pf.type;
	if (is_same(ccfg.sysop,pf.id)) now_type=19;
	cpf.type=now_type;
}



//메뉴시작시 진입로고
//lo2가 있고.. pf.mymode=0이면 로고2를 보여준다.
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
	if (p_command==No&&bit(pf.mymode,2)==0) {			//마지막 입력이 P인가?
		sprintf(filename1,"%s/logo/%s.log",club_go.dir,go.code);
		local_logo(filename1);	  //로칼메뉴로고처리하기
		strchng(filename2,filename1,".log",".lo2"); //setansi
		strchng(filename3,filename1,".log",".lo3"); //야간
		strchng(filename4,filename1,".log",".lo4"); //손님
		if (bit(pf.mymode,2)==0) {
			if (now_hour()>23&&now_hour()<5&&(key=view(filename3)));
			else if (guest_mode&&view(filename4));
			else if (bit(pf.mymode,6)==0) {
				if(!(key=view(filename2))) key=view(filename1);    //진입화면출력  *.log
			}
			else {
				key=view(filename1);	//진입화면출력	*.log
			}
			mview(Yes,"%s/logo/%s.vtx",club_go.dir,go.code);
			mview(Yes,"%s/logo/%s.jpg",club_go.dir,go.code);
		}
	}
	if (is_bbspds&&bit(bcfg.form,4)) pressenter();
	if (is_bbspds&&bit(bcfg.form,5)) strcpy(auto_cmd,"!");
	if (is_bbspds&&bit(bcfg.form,6)) strcpy(auto_cmd,"w");
}


// 화면을 표시한다.
disp_menu_screen()
{
	char filename1[300];
	char filename2[300];
	char filename3[300];
	char filename4[300];
	char buff[80];
	if (go.type!='m') return No;
	list_mode=No;

	/* 메뉴화면 출력  */
	sprintf(filename1,"%s/logo/%s.mnu",club_go.dir,go.code);
	local_logo(filename1);	  //로칼메뉴로고처리하기

	strcpy(filename2,filename1);strcpy(filename3,filename1);strcpy(filename4,filename1);
	strchng(filename2,filename1,".mnu",".mn2");  //setansi할경우
	strchng(filename3,filename1,".mnu",".mn3");  //야간작업경우
	strchng(filename4,filename1,".mnu",".mn4");  //손님전용로고
	if (bit(pf.mymode,7)) { 	   //작업용화면일경우
		strchng2(filename1,".mnu",".mn5");
		strcpy(filename2,filename1);
		strcpy(filename3,filename1);
		strcpy(filename4,filename1);
	}
	if		(bit(pf.mymode,2)&&pf.type>=cfg.no_logotype) no_logofile();
	else if (now_hour()>23&&now_hour()<5&&view(filename3));
	else if (guest_mode&&view(filename4));
	else if (bit(pf.mymode,6)&&(view(filename2)||view(filename1))) ;  //메뉴화면출력  *.mnu 실패
	else if (!bit(pf.mymode,6)&&view(filename1)) ;	//메뉴화면출력	*.mnu 실패
	else no_logofile(); //메뉴타입일때 화면이 없을때
	if (bit(pf.mymode,2)==0) {
		sprintf(filename1,"%s/logo/%s.ad%d",club_go.dir,go.code,now_cmd_num%10);
		local_logo(filename1);	  //로칼메뉴로고처리하기
		view(filename1);		   //광고
	}
	return Yes;
}


disp_screen()
{
	if (go.type=='m') disp_menu_screen();
	else if (is_char(BBSTYPE,go.type)) {
		if (list_mode) bbs_read(0); //해당번호 내용출력
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
	//명령줄 우측 광고
	sprintf(buff,"");
	now_cmd_num++;
	if (!list_mode&&bit(bcfg.mode,22)) return;	 //하단 명령어 선택 감춤
	if (list_mode &&bit(bcfg.mode,23)) return;	 //하단 명령어 선택 감춤

	//시삽 메세지 온것이 있는가?
	view("system/sysmess.txt");  //전체메세지
	if (go.type=='m') view("system/sysmess1.txt");  //전체메세지
	if (go.type=='m'&&is_same(go.code,"top")&&!club_mode) view("system/sysmess2.txt");  //전체메세지
	//메세지 온것이 있는가?
	sprintf(filename,"userdata/%s/%s/mess.dat",c_han(pf.id),pf.id);
	if (view(filename)) {pressenter();printf("\n");unlink(filename);}
	if (!bit(cfg.infomode,0)&&!club_mode&&guest_mode&&(is_same(go.code,"top")||!bit(cfg.guestmode,9))) {
		view("frm/guest/guestin%d.msg",now_cmd_num%10);
	}
	if (!is_pdsbbs) list_mode=No;

	check_samemoni();  //동시접속출력
	if (bit(cfg.logomode,5)) {
		if (bcfg.cmd_x>0||bcfg.cmd_y>0) xy(bcfg.cmd_x,bcfg.cmd_y);
		printf("[0;1m[80D[K 선택>>");
	}
	else {
		if (list_mode) sprintf(filename,"%s/logo/%s.cm2",club_go.dir,go.code);
		else		   sprintf(filename,"%s/logo/%s.cm1",club_go.dir,go.code);
		local_logo(filename);	 //로칼메뉴로고처리하기
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

	//임시메세지
	sprintf(buff,"");

	//하단 광고보여줌
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
		printf("7[60D[30C[K");   //하단광고하기
		view3(buff);
		printf("[0;1m8");   //하단광고하기
	}
}



disp_todaymode()
{
	int i;
	char buff[200];
	/*1.쓰기2.자료등록3.say로메세지보내기3.say로메세지보내기
	4.자료읽기5.자료다운6.대화실7.도움말(H,HELP)8.신상편집ABC9.검색10.동호회진입*/
	if (bit(cfg.newsmode,0)||!bit(cfg.newsmode,30)||bit(pf.mymode,22)) return;
	if (bit(bcfg.mode,20)) return;
	if (pf.type<cfg.newstype) return;
	if (newsmode[2]) return;
	if (go.type=='m'&&!club_mode&&is_same(go.code,"top")&&!bit(cfg.logomode,6)) {
		for (i=0;i<32;i++) {
			if (!bit(cfg.check_todayon,i)&&!bit(pf.todaymode,i)) {	 //오늘작업
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


set_pftype()  //회원의 등급,일일사용가능시간을 확인한다.
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
	fp=fopen("system/pftype.dat","r");   //등급화일
	if (fp==NULL) return;
	for (i=0;i<14;i++) {
		fread(&n,sizeof(n),1,fp);	   //손님,운영진은 적용안됨
		if (pf.type>0&&pf.type<14) {
			if (pf.level>n) {
				if (i>0) pf.type=i;
			}
		}
	}
	for (i=0;i<14;i++) {			 //한번 접속 가능시간
		fread(&n,sizeof(n),1,fp);
		if (pf.type==i) TODAY_TIM1=n*60;
	}

	for (i=0;i<14;i++) {			 //일일 사용가능시간
		fread(&n,sizeof(n),1,fp);
		if (pf.type==i) TODAY_TIM2=n*60;
	}
	for (i=0;i<14;i++) {			 //일일다운가능용량
		fread(&n,sizeof(n),1,fp);
		if (pf.type==i) TODAY_DOWN=n;
	}
	if (fp!=NULL) fclose(fp);

	if (!bit(cfg.levelmode,1)) pf.type=old_type;
	if (pf.type!=old_type) {	//등급변경시 저장함
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


view_loginhelp()		//진입시 잠깐도움말 보이기
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



