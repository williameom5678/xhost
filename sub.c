////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����  �������� ������ �޾ƾ� �մϴ�.
// ���� : ���� ��� ��ƾ
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
	pf.todaymode=set_bit(pf.todaymode,9,1);   //�����۾�
	while (1) {
		if (buff1[0]==0)  {
			find_mode=Yes;
			if (!view("frm/system/find.scr")) {
				printf("\n");
				printfL2();
				printf("       ȣ��Ʈ ���� �˻� �޴�\n");
				printfL2();
				printf("  1. �Խ��� �˻�       :�Խ��� ��ü ���� Ư�� ���� �˻�\n");
				printf("  2. �ڷ�� �˻�       :�ڷ�� ��ü ���� Ư�� ���� �˻�\n");
				printf("  3. ��ü ���� �˻�    :��ü ���뿡�� Ư�� ���ڿ� �˻�\n");
				printf("  4. �Խù� ��Ȳ �˻�  :�Խù�/�ڷ�� ��ü ��Ȳ ��ȸ\n");
				printf("  5. ���ȸ��    �˻�  \n");
				printf("  6. ȸ��        �˻�  \n");
				printf("  7. ����/�б�   �˻�  \n");
				printf("  8. ���        �˻�  \n");
				if (club_mode) printf("  7. ��ȣȸ ȸ�� �˻�  \n");
				printfL2();
				printf("  ���� >>");
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
			sys_tem(buff2) ; //ȭ��ã��
		}
		else if (is_same(buff1,"best")) {
			sprintf(buff2,"bin/findbest %s",club_go.code) ;
			sys_tem(buff2) ; //���ȸ��ã��
		}
		else if (is_same(buff1,"findpds")||is_same(buff1,"ff")||
				 is_same(buff1,"fp")||is_same(buff1,"findfile")) {
			sprintf(buff2,"bin/findbbs 1 %s",club_go.code) ; //ȭ��ã��
			set_stat2(19,buff2);	 //�۾�
			sys_tem(buff2) ; //ȭ��ã��
		}
		else if (is_same(buff1,"findbbs")||is_same(buff1,"fb")) {
			sprintf(buff2,"bin/findbbs 2 %s",club_go.code) ; //ȭ��ã��
			set_stat2(19,buff2);	 //�۾�
			sys_tem(buff2) ; //ȭ��ã��
		}
		else if (is_same(buff1,"pf")||is_same(buff1,"findpf")||is_same(buff1,"findpf1")||is_same(buff1,"findpf2")||is_same(buff1,"findpf3")) {
				 if (is_same(buff1,"findpf"))  sprintf(buff2,"bin/pf '%s' set_userfind",pf.id);
				 if (is_same(buff1,"findpf1")) sprintf(buff2,"bin/pf '%s' set_userfind",pf.id);
			else if (is_same(buff1,"findpf2")) sprintf(buff2,"bin/pf '%s' set_userfind2",pf.id);
			else if (is_same(buff1,"findpf3")) sprintf(buff2,"bin/pf '%s' set_userfind3",pf.id);
			set_stat2(19,buff2);	 //�۾�
			sys_tem(buff2) ; //ȭ��ã��
		} //�˻�
		strcpy(buff1,"");
		if (n==0||!find_mode) break;
	}
}


//ȭ�鿡 ������ ��ù� ���̴°�?
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
		message("������ �Խù��� ������ �ʽ��ϴ�.");
	}
	else {
		dd_mode=Yes;
		message("������ �Խù��� ���Դϴ�.");
	}
}


//ȭ����¸��
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
	if (mode== 2) strcpy(buff,rbit(pf.mymode, 2,"�ΰ���","�ΰ�Ⱥ���"));
	if (mode== 4) strcpy(buff,rbit(pf.mymode, 4,"TAB��ȯON","TAB��ȯOFF")); //settab
	if (mode== 6) strcpy(buff,rbit(pf.mymode, 6,"�Ƚú���","�ȽþȺ���")); //setansi
	if (mode== 7) strcpy(buff,rbit(pf.mymode, 7,"�ΰ���","�ΰ�Ⱥ���")); //setjob
	if (mode== 8) strcpy(buff,rbit(pf.mymode, 8,"����ON","����OFF"));       //setani
	if (mode==11) strcpy(buff,rbit(pf.mymode,11,"��������","�ڵ��ٳѱ�")); //setline
	if (oldmode!=pf.mymode&&strlen(buff)>0) {
		printf("\n ### ��������  %s ###\n",buff);fflush(stdout);
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
		else mess("xvote�� �޴��� �����ϴ�.");
	}
	else {
		sprintf(buff,"bin/svote '%s' '%s' ...",club_go.code,pf.id);
		sys_tem(buff);
	}
	re_print=Yes;
}

//�մ԰��Ը޴�
do_guest()
{
	char buff[100];
	int n,mode=0;
	re_print=Yes;
	go_top(0);
	go_code("guest");
	if (!is_same(go.code,"guest")) return No;
	if (no_exec_port("guest")) return No;
	if (bit(cfg.guestmode,14)) return No;  //���ԺҰ���
	if (bit(cfg.infomode,9)||guest_mode||pf.type>=14) {
		set_stat2(18,"/guest");    //�ܺ�
		set_rec(1,18);
		mode=atoi(go.wtimes);
		view("frm/guest/guestwai.log");
		sprintf(buff,"%d",mode);
		n=sys_tem("bin/guest %d",mode);
//		  n=sys_exec(1,30*60,"bin/guest","guest",buff,"","");
//		  set_signal();  //�ñ׳� �缳��
		set_stat(1,"");
		if (bit(cfg.mode,28)==0) set_alarm(1);
		if (n==1) ;
		else if (n==2) log_rc(0);
		else if (n==3) host_end(0);
		else if (bit(cfg.guestmode,4)) log_rc(0);
		else if (bit(cfg.guestmode,3)) host_end(0);
	}
	else message("�մԸ� �����մϴ�");
	go_prev();
	re_print=Yes;
	return (Yes);
}



//��ȣȸ �մ԰��Ը޴�
//��ȣȸ�� ȸ���ڷῡ�� Ÿ���� 1�� ����Ѵ�.
// 0:�ź� 1:�մ� 2:��� 3:ȸ��
do_cguest()
{
	int oldtype=cpf.type;
	if (!club_mode) return;
	sys_tem("bin/cguest '%s' '%s'",club_go.code,pf.id);
//	  set_signal();  //�ñ׳� �缳��
	if (go.type=='r') go_prev();
	check_cpf();			 //����б� -> now_type
	if (oldtype==0&&cpf.type!=oldtype) re_club=Yes;
	re_print=Yes;
}


//����������û�� ��
do_pfout()
{
	char buff[100];
	if (guest_mode||bit(allmode,3)) return;
	if (bit(cfg.mode,18)) return;
	sys_tem("bin/ed_pf '%s' '%s' 99",pf.id,pf.id);
//	  sys_exec(1,30*60,"bin/ed_pf","ed_pf",pf.id,pf.id,"99");
//	  set_signal();  //�ñ׳� �缳��
	re_print=Yes;
}


do_edid()
{
	if (guest_mode) return;
	sys_tem("bin/ed_pf '%s' edit_id_ok",pf.id);
	if (!read_pf4(pf.id)) strcpy(auto_cmd,"rc");
}

// ������ �����ִ� ���
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


// ���� �ڵ��ٿ�
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
		printf("\n %s �� �����ұ��?(Y/n)>>",filename);
	if (!yesno2("",Yes)) return;
	if (!stristr(filename,".zip")&&
		!stristr(filename,".arj")&&
		!stristr(filename,".rar")&&
		!stristr(filename,".tgz")&&
		!stristr(filename,".cap")&&
		!stristr(filename,".exe")&&
		!stristr(filename,".txt")) {
		mess("%s ���� ������ �ƴմϴ�.",filename);
		return;
	}
	if (!is_file(filename)||stristr(filename,"system/")||
		stristr(filename,"user/")||
		stristr(filename,"etc/")||
		stristr(filename,"bin/")) {
		mess("%s ������ �����ϴ�.",filename);
		return;
	}
	sz_file(filename);
	if (!is_char(go.wtimes,'x')) {  //��Ͼ���
		sprintf(buff2,"AUD %s",filename2);
		save_rec(1,8,buff2);
	}
}



//���� �޸���
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
			printf("\n 1. Ȯ��\n 2. ���\n 3. ����\n 4. ����\n ����>>");
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
			if (input_tmp(0)) { 		   //������ �Է¹޴´�.
				file_copy(TMP_FILE,filename);
				unlink(TMP_FILE);
			}
		}
		else if (n==3) xedit(filename);
		else if (n==4) sz_file(filename);
	}
}


//is_login	�ش�Ǵ� ���̵� �������ΰ�?
//return 0: ����  1:���̵� ����  2:�޼������Űź�
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
	ret=is_moni(buff);		//�������ΰ�?

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
	if(ret==0) sprintf(mess,"%s ���� %s ��������",buff,lasttime);
	if(ret!=0) sprintf(mess,"%s ���� %s���� ������",buff,lasttime);
	message(mess);
	return ret;
}


set_mymoni(char *buffstr)
{
	char buff[300];
	buff2cmd(buffstr,buff,1);  //�޼����� ���� �Է���
	if (strlen(buff)>1) {
		strcpy(mymoni,buff);
	}
	else {
		printf("\n ���� US�� �������� ������ ������ �ѱ� 10�� �̳��� �����ּ���.\n>>");
		lineinput(mymoni,20);printf("\n");
	}
	if (strlen(mymoni)<1) printf(" ### US ������ �����Ǿ����ϴ�.\n");
	else printf(" ### ��������:'%s'\n",mymoni);
	set_stat(1,"");
}


//���±�� ȭ�� ����
//�� tty���� �����Ѵ�.
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
	moni.msg=bit(pf.mymode,9);	   //�ʴ� �޼���
	moni.pid=getpid();
	moni.login=START_TIME;		  //���ӽ��۽ð�
	strcpy(moni.tty,tty);
	if (go.type=='m'&&is_same(go.code,"top")&&!club_mode) goto _cont2;
	if ( (is_char("ok",go.type)&&mode==5)||
		 (is_char(BBSTYPE,go.type)&&mode==5&&bit(bcfg.mode,15)) ) {
		_cont2:
		strcpy(moni.goname,"���θ޴�");
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
				//�������� ��� ����ó��
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

//���±�� ȭ�� ����  ���� �̸� �ٷ� ����
set_stat2(int mode,char *buff)
{
	GO oldgo;
	char buff2[100];
	oldgo=go;
	strcpy(buff2,buff);
		 if (stristr(buff,"ranklog" )) strcpy(buff2,"��������");
	else if (stristr(buff,"rank"    )) strcpy(buff2,"����Ȯ��");
	else if (stristr(buff,"bsysop"  )) strcpy(buff2,"�Խ��Ǵ��޴�");
	else if (stristr(buff,"csysop"  )) strcpy(buff2,"��ȣȸ�û�޴�");
	else if (stristr(buff,"sysop"   )) strcpy(buff2,"�û�޴�");
	else if (stristr(buff,"ed_pf"   )) strcpy(buff2,"ȸ���Ż����");
	else if (stristr(buff,"findname")) strcpy(buff2,"ȸ���˻�");
	else if (stristr(buff,"findbbs" )) strcpy(buff2,"�ڷ�˻�");
	else if (stristr(buff,"quest"   )) strcpy(buff2,"������������");
	else if (stristr(buff,"/guest"  )) strcpy(buff2,"�ű԰��Խ�û��");
	else if (stristr(buff,"/cguest" )) strcpy(buff2,"��ȣȸ���Խ�û��");
	else if (stristr(buff,"shell"   )) strcpy(buff2,"�ý��۽��۾�");
	else if (stristr(buff,"xcd"     )) strcpy(buff2,"����ڿ���");
	else if (stristr(buff,"cashsend")) strcpy(buff2,"�۱���");
	else							   strcpy(buff2,go.name);
	sprintf(go.name,"%-30.30s",buff2);
	set_stat(mode,buff);
	go=oldgo;
}



del_stat(char *tty) 	  //���±�� ȭ�� �ش� ��Ʈ�� ������ �����Ѵ�.
{
	del_stat_(tty,"");
}

del_stat_id(char *tid)		 //���±�� ȭ�� �ش� ��Ʈ�� ������ �����Ѵ�.
{
	del_stat_("",tid);
}


del_stat_(char *tty,char *tid)		 //���±�� ȭ�� �ش� ��Ʈ�� ������ �����Ѵ�.
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


//�ش� ��Ʈ���� ������ �ʴ°�?
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
			printf("\n ���� %s ��Ʈ ������ %s �޴��� ������ �ʽ��ϴ�.\n",mmc[1],mmc[0]);
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
	//ȭ���� ������ ������
	if (dir_filenum(dir_name)==0) { 	 //ȭ���� �� 1�� �ִ�
		sprintf(buff,"rm -rf %s",dir_name);
		sys_tem(buff);
	}
	re_print=Yes;

// xdir [���̵�] [���丮] [���]
// ��� 10111 : CD UP DN DD MD
}

set_filename()	 //����ȭ�ϸ� ����
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
		sprintf(NOW_FILE ,"tmp/tmpa%d%s",cfg.hostno,no_spc(tty));   //ȣ��Ʈ��ȣ���ӽ�ȭ��
		sprintf(NOW_FILED,"tmp/tmpb%d%s",cfg.hostno,no_spc(tty));   //ȣ��Ʈ��ȣ���ӽ�ȭ��
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
	pf.todaymode=set_bit(pf.todaymode,8,1);   //�����۾�
	if (pf.type<18&&!is_same(tid,pf.id)) strcpy(tid,"");
	if (strlen(tid)<3) strcpy(tid,pf.id);
	if (pf.type==0||guest_mode||bit(allmode,3)) {	  //�׽�Ʈ������ ����
		view("frm/pf/notguest.log");
		return;
	}
	else {
		sys_tem("bin/ed_pf '%s' '%s'",pf.id,tid);
		if (is_same(tid,pf.id)) read_pf3(pf.id);  //�ڱ��� �Ż��϶� �ٽ� ����
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
		mess(" ����  xed ���ϸ�");
		return No;
	}
	if ((fp=fopen("system/xed.dat","r"))==NULL) {
		mess("system/xed.dat ������ �����ϴ�");
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
		if (strlen(buff3)>2&&!check_pass(buff3,8,"���� ��ȣ >>")) return No;
		printf("\n  ���ϸ�:%s(%d B)  ���� 1.������ 2.Zmodem >> ",buff4,file_size(buff4));
		n=numinput();
		if (n==2) up_text(1,buff4);  //�ٿ�޾Ƽ�����ȭ�Ϸ� �ٲ۴�.
		else	  xedit(buff4);
		printf("\n  ���ϸ�:%s  ũ��:%d B \n",buff4,file_size(buff4));
		pressenter();
	}
	return (found);
}

