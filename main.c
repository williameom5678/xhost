
////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���� : ���� ���α׷� main.c
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
	allmode=0;					 //��ü���
	allmode2=0; 				 //��ü���
	if (argc>1&&is_same(argv[1],"/ver")) {
		printf("\n    VER:%s %s Date:%s Build:%s\n  ",VER1,VER2,MAKEDATE,MAKENUM);
		fflush(stdout);msleep(5);fflush(stdout);
		return;
	}
	read_cfg(); 				//config �б�

	cfg.waitkey=5;
	cfg.hostno=0;
	_last2=1;;;;;;;;;;;;;;;;;;;;;;;;
	allmode=limit_mode=0;
	findport(tty); //������Ʈ ã��
	sprintf(buff,"/dev/%s",tty);chmod(buff,0666);
	set_signal();
	if (argc>1&&strlen(argv[1])>2&&stristr(argv[1],"/")) {
		ch_dir(argv[1]);
	}
	strcpy(buff2,argv[0]);
	pure_name(2,buff2);   //ȭ�ϸ��� Ȯ���� ���� �̾Ƴ�
	if (strlen(buff2)<1) {
		strcpy(buff2,argv[0]);
		pure_name(0,buff2);   //ȭ�ϸ��� Ȯ���� ���� �̾Ƴ�
	}
	n=0;
	strchng2(buff2,"-bbs","bbs");
         if (is_same(buff2,"xmain2")||is_same(buff2,"xmain9")) {n=1;allmode2=set_bit(allmode2,11,1);ch_dir("/xhost");}
    else if (is_same(buff2,"xmain" )||is_same(buff2,"xmain7")) {n=1;allmode2=set_bit(allmode2,13,1);ch_dir("/bbs");}
	else if (stristr(buff2,"bbs")) {        //bbs�� ����ȭ�ϸ� ���ԵǾ��°�?
		sprintf(buff,"/bbs/%s",buff2);
		ch_dir(buff);
		allmode=set_bit(allmode,19,1);
	}

	xstrncpy(hostname,view_network(tty),20);
	if (n==1&&argc>2&&is_same(argv[1],"-h")) {
		strcpy(hostname,argv[2]);
		if (strlen(hostname)>0&&is_char(hostname,'.')) {
			allmode2=set_bit(allmode2,10,1); //HOSTNO�� �Ķ��Ÿ�� �ѱ�
		  //12991 pts/58   S	  0:00 /bbs/bin/xmain -h 203.254.78.211 -p
		}
	}

	if (bit(cfg.mode,28)==0) set_alarm(1);

	netmode=0;					 //��ü���
	pf.todaymode=0; 			   //��������
	ascr_count=0;
	mymoni_rec=0;
	c_proc=No;
	c_proc2=No;
	c_proc3=No;
	autoup=No;
	c_proc =is_file("system/c_proc.tag");
	c_proc2=is_file("system/c_proc2.tag");
	c_proc3=is_file("system/c_proc3.tag");
	if (c_proc) printf("HOST �׽�Ʈ��..step 1  TTY:%s\n",ttyname(1));
	CONT_DAY=0; 				//��������ϼ�

	allmode=set_bit(allmode,18,is_file("system/alliprec"));
	if (c_proc) printf("HOST �׽�Ʈ��..step 3\n");
	check_login_init(); 			//�ʱ⼳��
	log_in();						//�ʱ⼳�� �͹̳� ������ ����

	fflush(NULL);					//5

	SEND_KB=0;
	time(&START_TIME);			//���ӽ��۽ð�
	time(&CLUB_START_TIME); 		 //���ӽ��۽ð�
	time(&CHECK_TIME);			//���ӽ��۽ð�
	time(&pf.login);			//���ӽ��۽ð�
	strcpy(pf.id,"������");
	strcpy(pf.nick,"������");
	read_cfg(); 				//config �б�

	if (is_file("filelock.tag")) cfg.mprocmode=set_bit(cfg.mprocmode,3,1);

	set_tmpfile();
	read_bbsgood_color();
	strcpy(buff,"system/notushost.txt");
	if (in_file2(buff,hostname)) strcpy(hostname,tty);
	if (c_proc) printf("HOST �׽�Ʈ��..step 5\n");
	view("frm/log/loghost.log");
	//�ش�ȣ��Ʈ�� ��Ͼ���
	if (in_file2("system/rec_noth.txt",hostname)) allmode=set_bit(allmode,8,1);

	srand(time(NULL));			//������ �ʱ�ȭ
	now_cmd_num=rand()%20;		//������ ���� ��ȣ
	DM=DM1=DM2=DM3=No;
	printf("[1;50r[;H");
	setenv("TERM","vt220",1);
	sprintf(buff,"/dev/%s",tty);chmod(buff,0666);
	del_stat(tty);			 //���±�� ȭ�� �ش� ��Ʈ�� ������ �����Ѵ�.
	go_top(0);
	strcpy(go.name,"���̵��Է���");
	sprintf(moni.goname,"%-20.20s",go.name);
	set_stat(23,"������");
	cashmax1=0;
	cashmax2=0;
	g_yes=No;
	allmode=set_bit(allmode,20,in_file2("system/node_net.txt",tty));
	set_guest_();
	bbsdownnum=rand()+1000;   //�Խ��Ǵٿ��ȣ	�Ƶ�.��ȣ

	if (!bit(cfg.infomode,0)&&!bit(cfg.infomode,5)) start_logo(0);
	/* ���� ȭ�Ͽ� 0�� ����ִ´�. �̰��� ������¶�� ���� �˸��� ���̴�. */
	read_atype();				//����� �̸��� �о��	->atype
	read_ajob();				//������ �о��
	go_top(0);
	strcpy(club_go.code,"main");             //��ȣȸ�����ڵ�
	strcpy(club_go.dir,".");
	guest_mode=Yes;
	club_mode=No;
	strcpy(auto_cmd,"");
	re_print=Yes;			//ȭ�����
	re_logo=Yes;			//����ȭ�����
	re_bbs=Yes; 			//�Խ��Ƿε�
	re_club=No; 			//��ȣȸ�ʱ�ȭ
	p_command=No;			//������ �Է��� P�ΰ�?
	bbs_ltmode=No;			//lt������ �˻��� mode
	list_mode=No;			//���� ��ȣ ���ø��
	dd_mode=No; 			//�����Ȱͺ�����
	dark_mode=0;			//�û��� ��� 1:�϶�
	is_disp_help=Yes;
	now_type=0; 				//��ȣȸ�� ���ǥ��
	strcpy(auto_cmd,"");    //�մ��϶�
	strcpy(DEF_UPS,"");
	strcpy(DEF_DNS,"");
	strcpy(news_code,cfg.newsfile);
	UP_LEVEL=0; 			//���ù��� ����
	UP_CASH =0; 			//���ù��� ĳ��
	N_page=0;

	tag_monichar(); //���� ���
	strcpy(last_club,club_go.code);
	if (bit(cfg.startmode,1)) goto start_now1;		  //�ٷ� ����
	if (bit(cfg.notice,2)) view_notice(1,"notice");            //�������� ����

	//////////////////////ID �Է���
	if (!bit(cfg.infomode,6)) {
		if (argc>1) start_id(0,argv[1]);  //-l ID ó��
		else		start_id(0,"");       //���̵� �Է¹ް� �ʱ� ȭ���� �����ش�.
	}
	else {
		set_guest_mode2();
	}
    quit_all_room(3);        //��ȭ�濡 �ִ� �ش�ID,����tty,��Ʈ�� �����Ѵ�.
	no_spc(pf.id);
	//�ش�ID�� ��Ͼ���
	i=No;
	sprintf(buff,"%s/f_nid.dat",REC_DIR);   if (i==No&&in_file2(buff,pf.id   )) allmode=set_bit(allmode,29,1);
	sprintf(buff,"%s/f_ntty.dat",REC_DIR);  if (i==No&&in_file2(buff,tty     )) allmode=set_bit(allmode,29,1);
	sprintf(buff,"%s/f_nhost.dat",REC_DIR); if (i==No&&in_file2(buff,hostname)) allmode=set_bit(allmode,29,1);
	if (in_file2("system/rec_noti.txt",pf.id))    allmode=set_bit(allmode,9,1);


    save_login(1);          //���۱��
	check_all_limit();		//��ü ���� ����
	set_rec(1,23);		// ���ӽ���,���,size,����
	sys_tem("rm -f tmp/tmp*%d%s",cfg.hostno,no_spc(tty));   //ȣ��Ʈ��ȣ���ӽ�ȭ��
	sys_tem("rm -rf tmp/up%d%s" ,cfg.hostno,no_spc(tty));   //ȣ��Ʈ��ȣ���ӽ�ȭ��
	set_stat2(23,"������");
	//���18�̻��� Ű��� ����
	if ((bit(cfg.mode,0)&&pf.type>=18)||in_file2("system/nokey.id",pf.id))
		allmode=set_bit(allmode,13,1);

	check_wait(0);				//��ٸ����� �˻�
	strcpy(mymoni,"");

	clear_node();
	rm_tmpup();
	if (pf.type>0) check_smemo();			   //������Ÿ޸� �˻��Ѵ�.
	set_pftype();  //ȸ���� ���,���ϻ�밡�ɽð��� Ȯ���Ѵ�.

	if (!bit(cfg.infomode,7)) {
		view_loginhelp();		 //���Խ� ��񵵿� ���̱�
		view_notice(1,"notice");   //�������� ����
	}
	if (auto_cmd[0]==0&&is_gocode(pf.gofirst))
		sprintf(auto_cmd,"go %s",pf.gofirst); //�ڵ������ڵ�
	auto_go_start(auto_cmd,pf.type);  //��޺� ��������
	check_warn_mess();				  //������ ����ΰ�?
	set_stat2(1,"�ʱ�ȭ��");

	sprintf(buff,"user/fcmd/fcmd1.%s",no_spc(tty));      //�����������ΰ�?
	if (is_file2(buff)) file_line(auto_cmd,buff,1);
	sprintf(buff,"user/fcmd/fcmd2.%s",pf.id);    //�����������ΰ�?
	if (is_file2(buff)) file_line(auto_cmd,buff,1);

	start_now1: 	   //�ٷ� ����
	if (bit(cfg.guestmode,14)) {ch_dir("/xguest");execl("bin/main","main",NULL);}
	for (i=0;i<20;i++) bzero(acmd[i],78);
	cmdlinenum=0;
	re_logo=Yes;
	//���Խ� �ϵ� ��ũ�뷮 Ȯ��
	if (bit(cfg.startmode,16)&&check_free()<50) {
		view("frm/system/nospace.log");
		if (pf.type<13) host_end();
	}

		 if (bit(cfg.zmode,23)) check_zbtotal(1*60*60,0);
	else if (bit(cfg.zmode,24)) check_zbtotal(6*60*60,0);

	//�Խ��Ǳ�ϰ���
	strcpy(bbsrec_club,"");
	strcpy(bbsrec_code,"");
	bbsrec_time=0;
    add_logcount();      //���ӱ��
	if (guest_mode&&bit(cfg.guestmode,1)) strcpy(auto_cmd,"bye");
	while(1) {
		if ( is_char("mr",go.type )) check_bbsrec(1);  //�Խ��� ��� ���� ����
		else						 check_bbsrec(0);  //�Խ��� ��� ���� ����
		nostop=No;				  //������ ����(�ΰ�������)
		if (strlen(pf.id)<3) {
			check_error(1);
			log_rc(0);
		}
		if (re_club) {
			printf(VTXOFF);
			if (club_mode) club_init();    //��ȣȸ�� �ʱ�ȭ
			else		   main_init();    //��ȣȸ�� �ƴҶ� �ٽ� ������
			re_club=No;
			re_print=Yes;
			time(&CHECK_TIME);			//���ӽ��۽ð�
			tag_monichar(); //���� ���
			continue;
		}

		if (is_char("rejnp",go.type)) {
			go_prev();
			continue;
		}
		check_another_rec();		//��ȣȸ��ġ����� ���
		if (re_logo) {
			view_pos=0;
			if (bit(cfg.mode,28)==0) set_alarm(1);
			read_bbsgood_color();
			if (check_type('i')) {
				go_prev();
				continue;
			}
			if (auto_cmd[0]==0) disp_menu_logo();	//�޴����Խ� �����ִ� �ΰ�
			re_logo=No;
			re_print=Yes;
			list_mode=No;
			re_print=Yes;
		}
		strcpy(_last1,"");

		// ȭ���� ǥ���Ѵ�.
		input_0:
		if (re_print) {
			allmode2=set_bit(allmode2,2,0);  //�ٿ�,���� �ð�Ȯ�α�������
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

			set_news(Yes);			   //��������
			for (i=0;i<50;i++) list_anum[i]=0;	 //ȭ����� ��ȣ
			if (pf.warn%10>=8) go_code("_warn");
			if (list_mode) set_stat(1,bbs.title);  //������ġ ����
			else		   set_stat(1,""       );  //������ġ ����
			if (strlen(auto_cmd)<1) disp_screen();	   //�޴�,����Ʈ
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
		disp_mid(); 		 //No�̸� ���� ��¾��� ����Է¹ޱ��� �޼��� ǥ��
		sprintf(buff,"user/fcmd/fcmd3.%s",no_spc(tty));      //�����������ΰ�?
		if (is_file2(buff)) {
			file_line(auto_cmd,buff,1);
			unlink(buff);
		}
		all_trim(auto_cmd);
		if (strlen(auto_cmd)>0) {		   //�ڵ����ó��
			del_esc(auto_cmd);
			strcpy(buff,auto_cmd);
			strcpy(auto_cmd,"");
		}
		else if (sub_go[0]!=0) {		//�ڵ����ó��
			strcpy(buff,sub_go);
			strcpy(sub_go,"");
		}
		else {						   //��ɾ� �Է·�ƾ
			if (!bit(cfg.mode,4)&&!bit(pf.mymode,0)) printf(ESCENG);  //�������� ��ȯ �ѱ�:h
			input_2:
			printf("7");
			//���ø���Է�
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
			if (strlen(buff)<5) strchng2(buff,"� ","");
			if (strlen(buff)>0) next_acmd();

			if (is_same(buff,"ver")) {view_ver();continue;}
			if (stristr(buff,"B080")) {printf("8[K");goto input_2;}
			if (go.type=='m'&&strlen(buff)<1) {re_print=Yes;goto input_0;printf("\n");}
			set_news(No);
		}
		if (buff[0]!='_') {
			check_short(1,buff) ;				  //������Ű ����
			check_short(2,buff) ;				  //���κ������ɺм�
		}
		input_4:

		if (bit(cfg.mrecmode,11)&&strlen(buff)>0) cmd_record(buff);

		check_cmd(buff);					  //��ɾ� ����
		i=strlen(mmc[0]);

		if (i>0) {
			allmode=set_bit(allmode,16,0);	 //ȭ��ִϸ� ���� ó��
			allmode=set_bit(allmode,17,0);	 //Ű�Է´�⸦ ���� ó��
			printf("\n");
		}

		if (i<1) {						  //�׳��Է½�
				 if (is_bbspds&&!list_mode)  bbs_next();
			else if (is_bbspds&& list_mode)  bbs_read(1);
			else {
				goto input_1;
			}
		}
		else if (is_same(mmc[0],"go")&&mmc[2][0]=='_'&&(mmc[2][1]=='#'||mmc[2][1]=='*')) {
			if (!check_direct_go(1,buff)) {printf("\n");goto input_1;}
		}
		else if (do_menu_command()) 			  ; //�޴����� ����ΰ�?
		else if (do_basic_command(buff,buff))	  ; //�⺻��ɾ��ΰ�?
		else if (do_list_command()) 			  ; //�Խ����� �ڷ��ȣ�ΰ�?
		else if (do_order_command(mmc[0],buff))   ; //�⺻��ɾ��ΰ�?
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

	if (strlen(mmc[1])>0&&is_same(mmc[0],"��")) {strcpy(mmc[0],"go");goto _cont3;}
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
		set_filename(); 						//����ǥ���� ȭ�ϸ�
		read_bcfg();
		re_bbsnum(1);			//BREC 0:�б� 1:�ڵ�  2:����  3:��ü����  8:1���߰� 9:1������
		re_bbs=re_logo=re_print=No;
		if (check_type('i')||no_exec_port(go.code)) {
			go_top(0);
			return; //��밡�� �޴��ΰ�?
		}
		check_bbs();		  //�ڷ��� ������ �ľ��ϴ� ��ƾ
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
/// 	   ��ȣȸ ���� �۾�
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
					mess("��ȣȸ ���Ա����Ҽ� ���� �����Դϴ�");
				}
			}
			club_mode=No;
			go_top(0);
			return;
		}
	}

	if (bit(limit_mode,4)) {
		if (!view("frm/pf/notclub.log")) mess("��ȣȸ����");
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
	bbs_ltmode=No;		  //lt������ �˻��� ���
	re_bbs=Yes;
	for (i=0;i<20;i++) strcpy(atype[i],"");
	pf.todaymode=set_bit(pf.todaymode,10,1);   //�����۾�
	newsmode[0]=newsmode[1]=newsmode[2]=newsmode[3]=0;
	_last2=8;;;;;;;;;;;;;;;;;;;;;;;;

	bzero(auto_cmd,30);
	if (!is_file2(club_go.dir)) {
		sprintf(g_buff1,"%s (%s)",club_go.code,club_go.name);
		sprintf(g_buff2,"���丮:%s",club_go.dir);
		if (yesno3("frm/system/noclub.log",No)) {
			view("doc/makeclub.txt");
			pressenter();
		}
        go_top(0); //��ȣȸ �����޴��� �̵�
		return;
	}
	sprintf(buff,"%s/menustru/menu.dat",club_go.dir);
	if (!is_file2(buff)) {
		if (yesno2("��ȣȸ�޴��� ��ȯ�ұ��?(Y/n)>>",Yes)) {
			strcpy(buff,"upgrade/men2_50");
			if (!is_file2(buff)) mess("%s������ �����ϴ�.",buff);
			else				sys_tem("%s '%s'",buff,club_go.dir);
		}
	}

	// ��ȣȸ ���Ա��� ����
	// 0. ����� 0�� �ƴ�.
	// 1. ����� ���԰��� �������
	// 2. ����� ��ȣȸ�̸鼭 ȸ���� �ƴ�
	// 3. ����� ��ȣȸ�� ��� ��ȣ�� ���
	// 4. ��û������� ���
	set_nowmenu();
	read_ccfg();
	sys_tem("touch %s/system/clubid.dat",club_go.dir);
	check_cpf();			 //����б� -> now_type
	go_top(1);
	time(&CLUB_START_TIME);   //��ȣȸ ���ӽ��۽ð�
	key=Yes;
	for (i=0;i<10;i++) {
		sprintf(afile[i],"%s/frm/error/whynot%d.log",club_go.dir,i);
		if (!is_file2(afile[i])) sprintf(afile[i],"frm/error/whynot%d.log",i);
	}
	if (now_type==0) {view(afile[0]);key=No;}	 //��ȣȸ �����ּҵ��
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
	if (ss_check(6))   {view(afile[5]);key=No;}    //��ü ��ȣȸ ���Ա���
	if (check_warn(4)) {view(afile[6]);key=No;}    //��ȣȸ ������
	if (key==No) {go_top(0);re_club=Yes;return;}

	read_actype();			 //��ȣȸ����� �̸��� �о��  ->actype
	sprintf(buff,"���:%d",now_type);
	start_logo(0);	//���Խ� �����ִ� �޼���
	if (!bit(cfg.infomode,0)&&cpf.all_log==1) { 	  //��ȣȸ ó�� ����
		view("%s/frm/log/firstlo.log",club_go.dir);
		read_pf4(ccfg.sysop);
		strcpy(bbs.id,pf2.id);
		strcpy(bbs.name,pf2.name);
		sprintf(buff,"%s/frm/log/firstl1.log",club_go.dir);
		if (is_file2(buff))
			wmail4(pf.id,buff,"��ȣȸ ������ �����մϴ�.",1); //���Ϲ߼�
		sprintf(buff,"%s/frm/log/firstl2.log",club_go.dir);
		if (is_file2(buff))
			wmail4(pf.id,buff,"��ȣȸ ������ �������� ȯ���մϴ�.",1); //���Ϲ߼�
	}
	if (strlen(sub_go)==0) view_notice(1,"notice");         //��ȣȸ ��������
	strcpy(mmc[1],"");
	strcpy(mmc[2],"");
	check_warn_mess();			//��ȣȸ�� ����ΰ�?
	if (!bit(ccfg.logomode,3)&&now_type>0&&now_type<20) {
		strcpy(buff,actype[now_type]);
		del_space(buff);
		if (strlen(buff)>0) message(" %s���� [%s]�Դϴ�.",pf.id,actype[now_type]);
	}
	set_rec(1,2);	  //��ȣȸ���ӽ���
	check_maxmenu();  // ��ȣȸ �ִ�޴��� Ȯ��
}


//���۽� �Ǵ� ��ȣȸ���� ���� ���ö�
main_init()
{
	while (go.type=='s') go_prev2();    //��ȣȸ ������������
	now_type=0; 				//������ ���
	read_atype();				//����� �̸��� �о��	->atype
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


// ��ȣȸ�� ȸ�� �Ż��� �а� ����� �ľ��Ѵ�.
check_cpf()
{
	int i;
	char buff[30];
	char buff2[180];
	int found=No;

	now_type=1;  //�⺻�� �մ����� ���Ѵ�.
	strcpy(cpf.id,"");
	strcpy(cpf.name,"");
	cpf.level=0;
	cpf.warn=0;
	cpf.all_log=0;
	if (!club_mode) return;

	//��ȣȸ����̰ų�	��ȣȸ�� �û��ΰ��
	if (pf.type==17)  now_type=18;
	if (pf.type>=18)  now_type=19;
	if (now_type<1)   now_type=1;
	if (now_type<=18) {
		//ȸ�������� �о�´�
		if (read_cpf(pf.id)) {
			if (cpf.type==0||cpf.type>now_type) now_type=cpf.type;
			strcpy(cpf.id  ,pf.id  );		//�̸������ �ڵ����� �ø�
			strcpy(cpf.name,pf.name);		//�̸������ �ڵ����� �ø�
			time(&cpf.logout);				//��ȣȸ ���� �ð� ����
			cpf.all_log++;				//��ȣȸ ���� Ƚ��
			if (bit(ccfg.mode,1)) cpf.type=now_type=pf.type;
			if (is_same(ccfg.sysop,pf.id)) cpf.type=19;
			save_cpf(pf.id);				//ȸ���ڷ� ����
		}
	}
	if (now_type>19) now_type=19;
	if (now_type<19&&bit(ccfg.mode,1)) now_type=pf.type;
	if (is_same(ccfg.sysop,pf.id)) now_type=19;
	cpf.type=now_type;
}



//�޴����۽� ���Էΰ�
//lo2�� �ְ�.. pf.mymode=0�̸� �ΰ�2�� �����ش�.
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
	if (p_command==No&&bit(pf.mymode,2)==0) {			//������ �Է��� P�ΰ�?
		sprintf(filename1,"%s/logo/%s.log",club_go.dir,go.code);
		local_logo(filename1);	  //��Į�޴��ΰ�ó���ϱ�
		strchng(filename2,filename1,".log",".lo2"); //setansi
		strchng(filename3,filename1,".log",".lo3"); //�߰�
		strchng(filename4,filename1,".log",".lo4"); //�մ�
		if (bit(pf.mymode,2)==0) {
			if (now_hour()>23&&now_hour()<5&&(key=view(filename3)));
			else if (guest_mode&&view(filename4));
			else if (bit(pf.mymode,6)==0) {
				if(!(key=view(filename2))) key=view(filename1);    //����ȭ�����  *.log
			}
			else {
				key=view(filename1);	//����ȭ�����	*.log
			}
			mview(Yes,"%s/logo/%s.vtx",club_go.dir,go.code);
			mview(Yes,"%s/logo/%s.jpg",club_go.dir,go.code);
		}
	}
	if (is_bbspds&&bit(bcfg.form,4)) pressenter();
	if (is_bbspds&&bit(bcfg.form,5)) strcpy(auto_cmd,"!");
	if (is_bbspds&&bit(bcfg.form,6)) strcpy(auto_cmd,"w");
}


// ȭ���� ǥ���Ѵ�.
disp_menu_screen()
{
	char filename1[300];
	char filename2[300];
	char filename3[300];
	char filename4[300];
	char buff[80];
	if (go.type!='m') return No;
	list_mode=No;

	/* �޴�ȭ�� ���  */
	sprintf(filename1,"%s/logo/%s.mnu",club_go.dir,go.code);
	local_logo(filename1);	  //��Į�޴��ΰ�ó���ϱ�

	strcpy(filename2,filename1);strcpy(filename3,filename1);strcpy(filename4,filename1);
	strchng(filename2,filename1,".mnu",".mn2");  //setansi�Ұ��
	strchng(filename3,filename1,".mnu",".mn3");  //�߰��۾����
	strchng(filename4,filename1,".mnu",".mn4");  //�մ�����ΰ�
	if (bit(pf.mymode,7)) { 	   //�۾���ȭ���ϰ��
		strchng2(filename1,".mnu",".mn5");
		strcpy(filename2,filename1);
		strcpy(filename3,filename1);
		strcpy(filename4,filename1);
	}
	if		(bit(pf.mymode,2)&&pf.type>=cfg.no_logotype) no_logofile();
	else if (now_hour()>23&&now_hour()<5&&view(filename3));
	else if (guest_mode&&view(filename4));
	else if (bit(pf.mymode,6)&&(view(filename2)||view(filename1))) ;  //�޴�ȭ�����  *.mnu ����
	else if (!bit(pf.mymode,6)&&view(filename1)) ;	//�޴�ȭ�����	*.mnu ����
	else no_logofile(); //�޴�Ÿ���϶� ȭ���� ������
	if (bit(pf.mymode,2)==0) {
		sprintf(filename1,"%s/logo/%s.ad%d",club_go.dir,go.code,now_cmd_num%10);
		local_logo(filename1);	  //��Į�޴��ΰ�ó���ϱ�
		view(filename1);		   //����
	}
	return Yes;
}


disp_screen()
{
	if (go.type=='m') disp_menu_screen();
	else if (is_char(BBSTYPE,go.type)) {
		if (list_mode) bbs_read(0); //�ش��ȣ �������
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
	//����� ���� ����
	sprintf(buff,"");
	now_cmd_num++;
	if (!list_mode&&bit(bcfg.mode,22)) return;	 //�ϴ� ��ɾ� ���� ����
	if (list_mode &&bit(bcfg.mode,23)) return;	 //�ϴ� ��ɾ� ���� ����

	//�û� �޼��� �°��� �ִ°�?
	view("system/sysmess.txt");  //��ü�޼���
	if (go.type=='m') view("system/sysmess1.txt");  //��ü�޼���
	if (go.type=='m'&&is_same(go.code,"top")&&!club_mode) view("system/sysmess2.txt");  //��ü�޼���
	//�޼��� �°��� �ִ°�?
	sprintf(filename,"userdata/%s/%s/mess.dat",c_han(pf.id),pf.id);
	if (view(filename)) {pressenter();printf("\n");unlink(filename);}
	if (!bit(cfg.infomode,0)&&!club_mode&&guest_mode&&(is_same(go.code,"top")||!bit(cfg.guestmode,9))) {
		view("frm/guest/guestin%d.msg",now_cmd_num%10);
	}
	if (!is_pdsbbs) list_mode=No;

	check_samemoni();  //�����������
	if (bit(cfg.logomode,5)) {
		if (bcfg.cmd_x>0||bcfg.cmd_y>0) xy(bcfg.cmd_x,bcfg.cmd_y);
		printf("[0;1m[80D[K ����>>");
	}
	else {
		if (list_mode) sprintf(filename,"%s/logo/%s.cm2",club_go.dir,go.code);
		else		   sprintf(filename,"%s/logo/%s.cm1",club_go.dir,go.code);
		local_logo(filename);	 //��Į�޴��ΰ�ó���ϱ�
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

	//�ӽø޼���
	sprintf(buff,"");

	//�ϴ� ��������
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
		printf("7[60D[30C[K");   //�ϴܱ����ϱ�
		view3(buff);
		printf("[0;1m8");   //�ϴܱ����ϱ�
	}
}



disp_todaymode()
{
	int i;
	char buff[200];
	/*1.����2.�ڷ���3.say�θ޼���������3.say�θ޼���������
	4.�ڷ��б�5.�ڷ�ٿ�6.��ȭ��7.����(H,HELP)8.�Ż�����ABC9.�˻�10.��ȣȸ����*/
	if (bit(cfg.newsmode,0)||!bit(cfg.newsmode,30)||bit(pf.mymode,22)) return;
	if (bit(bcfg.mode,20)) return;
	if (pf.type<cfg.newstype) return;
	if (newsmode[2]) return;
	if (go.type=='m'&&!club_mode&&is_same(go.code,"top")&&!bit(cfg.logomode,6)) {
		for (i=0;i<32;i++) {
			if (!bit(cfg.check_todayon,i)&&!bit(pf.todaymode,i)) {	 //�����۾�
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


set_pftype()  //ȸ���� ���,���ϻ�밡�ɽð��� Ȯ���Ѵ�.
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
	fp=fopen("system/pftype.dat","r");   //���ȭ��
	if (fp==NULL) return;
	for (i=0;i<14;i++) {
		fread(&n,sizeof(n),1,fp);	   //�մ�,����� ����ȵ�
		if (pf.type>0&&pf.type<14) {
			if (pf.level>n) {
				if (i>0) pf.type=i;
			}
		}
	}
	for (i=0;i<14;i++) {			 //�ѹ� ���� ���ɽð�
		fread(&n,sizeof(n),1,fp);
		if (pf.type==i) TODAY_TIM1=n*60;
	}

	for (i=0;i<14;i++) {			 //���� ��밡�ɽð�
		fread(&n,sizeof(n),1,fp);
		if (pf.type==i) TODAY_TIM2=n*60;
	}
	for (i=0;i<14;i++) {			 //���ϴٿ�ɿ뷮
		fread(&n,sizeof(n),1,fp);
		if (pf.type==i) TODAY_DOWN=n;
	}
	if (fp!=NULL) fclose(fp);

	if (!bit(cfg.levelmode,1)) pf.type=old_type;
	if (pf.type!=old_type) {	//��޺���� ������
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


view_loginhelp()		//���Խ� ��񵵿� ���̱�
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



