///////////////////////////////////////////////////////////////////////
// XHOST 5.3s: �������� BBS HOST
// �ý��� ������ ����Ǵ� Ư���� �۾���
//////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include <unistd.h>    //���丮 ȭ�ϸ� ��ȯ�Լ������� �ʿ�
#include <dirent.h>    //���丮 ȭ�ϸ� ��ȯ�Լ������� �ʿ�

sys_menu(char *buffstr)
{
    char buff[200],buff1[200],buff2[200],buff3[200],buff4[200];
	char tid2[9];
	char filename[200];
	char cmd[100];
	SYSCMD syscmd[30];		  //Ư�� ȯ��ȭ��
	FILE *fp;
    int i,n;
	char old_now_type=now_type;
    PF pf3,old_pf;
	GO old_club_go,old_go;
	char old_club_mode=club_mode;
	strcpy(cmd,mmc[0]);
	strcpy(tid2,mmc[1]);
	re_print=Yes;

    old_pf=pf;
	old_club_go=club_go;
	old_go=go;
	if (pf.type<1) return;		//�⺻ȸ���̻��϶�
	strcpy(filename,"system/syscmd.dat");
	if ((fp=fopen(filename,"r"))==NULL) {mess("%s Error",filename);return;}
	for (i=0;i<30;i++) fread(&syscmd[i],sizeof(syscmd[0]),1,fp);
	fclose(fp);
	tag_monichar(); //���� ���
	//��ȣ����
	for (i=0;i<30;i++) {
		del_space(syscmd[i].cm2);del_space(syscmd[i].cmd);
		if (is_same(cmd,syscmd[i].cmd)||is_same(cmd,syscmd[i].cm2)) {
			strcpy(cmd,syscmd[i].cmd); //��2���ϰ�� 1������� ��ȯ
			if (pf.type<syscmd[i].type) {
				mess("%s����� ����Ҽ� ���� ����Դϴ�.",cmd);
				return;
			}
			if (!check_sys_pass(i)) return;
			break;
		}
	}
	set_stat(19,mmc[0]);	//�۾�
		 if (pf.type>10&&(is_same(mmc[0],".lo")||is_same(mmc[0],".lo1"))) allmode=gtt_bit(allmode,10);
	else if (pf.type>10&&is_same(mmc[0],".lo2")) allmode=gtt_bit(allmode,11);
	else if (pf.type>10&&is_same(mmc[0],".lo3")) allmode=gtt_bit(allmode,12);
	else if (pf.type>=14&&is_same(mmc[0],".rescan")) sys_tem2("bin/zbtotal '%s' -f &",club_go.code);
    else if (pf.type>=14&&is_same(mmc[0],".rebnum")) sys_tem2("bin/zbtotal '%s' rebnum",club_go.code);
	else if (is_same(cmd,".."))                  help_sys_menu();     //��� ����
	else if (pf.type>=16&&is_same(cmd,".xlogo1")) sys_xlogo(1);
	else if (is_same(cmd,".xlogo2"))              sys_xlogo(2);
	else if (pf.type>=18&&stristr(cmd,".proc")) {
		strchng2(cmd,".","");
		sprintf(buff,"system/c_%s.tag",cmd);
		if (is_file(buff)) {unlink(buff);mess("%s ���� ���� (�����ӽõ��۵�)",buff);}
		else			   {touch_file(buff);mess("%s ���� ���� (�����ӽõ��۵�)",buff);}
	}
    else if (is_same(cmd,".nw1")) set_notwrite(1);
    else if (is_same(cmd,".nw2")) set_notwrite(2);
    else if (bbs_man(pf.id)&&is_same(cmd,".notre")) xedit2("%s/logo/all.nre",club_go.dir);
	else if (bbs_man(pf.id)&&is_same(cmd,".notre2")) xedit2("%s/logo/%s.nre",club_go.dir,go.code);
	else if (bbs_man(pf.id)&&is_same(cmd,".notwr")) xedit2("%s/logo/all.nwr",club_go.dir);
	else if (bbs_man(pf.id)&&is_same(cmd,".notwr2")) xedit2("%s/logo/%s.nwr",club_go.dir,go.code);
	else if (bbs_man(pf.id)&&is_same(cmd,".notup")) xedit2("%s/logo/all.nup",club_go.dir);
	else if (bbs_man(pf.id)&&is_same(cmd,".notup2")) xedit2("%s/logo/%s.nup",club_go.dir,go.code);
	else if (bbs_man(pf.id)&&is_same(cmd,".notdn")) xedit2("%s/logo/all.ndn",club_go.dir);
	else if (bbs_man(pf.id)&&is_same(cmd,".notdn2")) xedit2("%s/logo/%s.ndn",club_go.dir,go.code);
    else if (bbs_man(pf.id)&&is_same(cmd,".renum")) {sys_tem("bin/bsysop '_renum_bbs' '%s' '%s'",NOW_FILE,NOW_FILED);}
	else if (is_same(cmd,".datestr")) sys_tem("bin/xfrm datestr");
	else if (is_same(cmd,".clog")&&(pf.type>=15||now_type>=14))  sys_tem("bin/viewlog 8 '%s' '%s'",pf.id,club_go.code);
	else if (is_same(cmd,".txt")&&is_char(BBSTYPE,go.type)&&(pf.type>=17||(club_mode&&now_type>=17)||bbs_man(pf.id))) sys_tem("bin/txt2bbs %s %s %s",club_go.code,go.code,pf.id);
	else if (is_same(cmd,".lr"))   sys_tem("bin/viewlog 4 '%s' '%s'",pf.id,club_go.code);
	else if (is_same(cmd,".news")) sys_tem("rm -rf tmp/news/");
	else if (is_same(cmd,".port")) sys_tem("bin/viewlog 5 '%s' '%s'",pf.id,club_go.code);
	else if (pf.type>15&&is_same(cmd,".set")) {cls();sys_tem("set");pressenter();}
	else if (is_same(cmd,".dansi")&&(pf.type>=17||(club_mode&&now_type>=17))) bbs_dansiall(); //��ü �Ƚ�����
	else if (pf.type>=15&&is_same(cmd,".1"))      sys_add_down(1);      //1�ް� ����
	else if (pf.type>=15&&is_same(cmd,".5"))      sys_add_down(5);      //10�ް� ����
	else if (pf.type>=15&&is_same(cmd,".10"))     sys_add_down(10);     //10�ް� ����
	else if (pf.type>=16&&is_same(cmd,".50"))     sys_add_down(50);     //50�ް� ����
	else if (pf.type>=16&&is_same(cmd,".100"))    sys_add_down(100);    //50�ް� ����
    else if (is_same(cmd,".xmessd")) {
        printf(" xmessd ó�� �׽�Ʈ(����ȵ�) :%s\n",rbit(cfg.messmode,7,"tty���", "xmessd���"));
        cfg.messmode=set_bit(cfg.messmode,7,!yesno2("tty���(Y)  xmessd���(N) >>",Yes));
        sprintf(buff," HOST:%d TTY:%s �޼������� �׽�Ʈ (%s)",cfg.hostno,tty,rbit(cfg.messmode,7,"tty", "xmessd"));
        to_tty2(cfg.hostno,tty,buff);
        fflush(NULL);
        pressenter();
    }
	else if (pf.type>=16&&is_same(cmd,".local"))  {
		printf("\n  ��Į�޴���ȣ (����:%d)>>",pf.localmenu);
		pf.localmenu=get_num(pf.localmenu,3);
        if (pf.type>0) save_pf(36);
		re_flash();
	}
	else if (pf.type>=18&&is_same(cmd,".pass_del")) sys_tem("bin/bsysop '%s' '%s' all_pass_del",go.code,club_go.code);
	else if (pf.type>=18&&is_same(cmd,".mail_del")) sys_tem("bin/bsysop '%s' '%s' all_mail_del",go.code,club_go.code);
	else if (pf.type>=16&&is_same(cmd,".sort"))   sys_tem("bin/bsysop '%s' '%s' '%s' 101",go.code,club_go.code,pf.id);
	else if (pf.type>=16&&is_same(cmd,".size"))   sys_tem("bin/bsysop '%s' '%s' '%s' 102",go.code,club_go.code,pf.id);
	else if (pf.type>=16&&is_same(cmd,".sm") )      xedit2("system/sysmess.txt");
	else if (pf.type>=16&&is_same(cmd,".sm1"))      xedit2("system/sysmess1.txt");
	else if (pf.type>=16&&is_same(cmd,".sm2"))      xedit2("system/sysmess2.txt");
	else if (pf.type>=18&&is_same(cmd,".ups"))      {printf("\n �⺻���� UPS ���丮\n>>");get_str(DEF_UPS,80);del_space(DEF_UPS);}
	else if (pf.type>=18&&is_same(cmd,".ls"))       view_pdsdir();
	else if (pf.type>=18&&is_same(cmd,".pass"))     sys_change_passwd();
	else if (pf.type>=18&&is_same(cmd,syscmd[21].cmd))	do_shell();
	else if (pf.type>=18&&is_same(cmd,".time"))     system("bin/setdate");
	else if (pf.type>=18&&is_same(cmd,".epass"))    {sprintf(buff,"user/epass/%s.lst",now_str(12));view_text(buff);}
	else if (pf.type>=19&&is_same(cmd,".fcode"))    system("freecode/freemake");
	else if (pf.type>=18&&is_same(cmd,".freem"))    {system("free");pressenter();}
    else if (pf.type>=18&&is_same(cmd,".limit"))    set_limit();
	else if (pf.type>=18&&(
		is_same(cmd,".ps")||
		is_same(cmd,".who")||
		is_same(cmd,".netstat"))) {
		if (is_same(cmd,".ps")) sprintf(buff,"ps aux  > %s",TMP_FILE);
		if (is_same(cmd,".who")) sprintf(buff,"who -T    > %s",TMP_FILE);
		if (is_same(cmd,".netstat")) sprintf(buff,"netstat -r > %s",TMP_FILE);
		sys_tem(buff);
		view_text(TMP_FILE);
		unlink(TMP_FILE);
	}
	else if (pf.type>=18&&is_same(cmd,".touch"))  {
		if (is_char(BBSTYPE,go.type)) {
			sprintf(buff,"touch %s",NOW_FILE);sys_tem(buff);
			sprintf(buff,"touch %s",NOW_FILED);sys_tem(buff);
			mess("%s ȭ���� ����� �����ϴ�.",NOW_FILE);
		}
	}
	else if (pf.type>=14&&is_same(cmd,syscmd[23].cmd)) {
		if (!is_char(BBSTYPE,go.type)) mess("�Խ���/�ڷ�ǿ��� ����ϴ� ����Դϴ�.");
		else sys_tem("bin/bbscopy '%s' '%s' '%s'",go.code,club_go.code,pf.id);
	}
	else if (is_same(cmd,syscmd[0].cmd))   do_sysop(1);
	else if (is_same(cmd,syscmd[22].cmd))  do_sysop(2);
	else if (is_same(cmd,syscmd[24].cmd))  do_sysop(3);
	else if (is_same(cmd,syscmd[1].cmd))   do_csysop();
	else if (is_same(cmd,syscmd[2].cmd))   do_bsysop();
	else if (is_same(cmd,syscmd[5].cmd)&&(pf.type>=16||now_type>17))
		sys_tem("bin/xfrm 1 '%s' '%s' '%s'",club_go.code,go.code,pf.id);
	else if (is_same(cmd,syscmd[15].cmd)&&(pf.type>=16||now_type>17))
		sys_tem("bin/xfrm 2 '%s' '%s' '%s'",club_go.code,go.code,pf.id);
	else if (is_same(cmd,syscmd[6].cmd)&&((!club_mode&&pf.type>16)||(club_mode&&now_type>16)))
		sys_tem("bin/xlogo '%s' '%s' '%s'",club_go.code,go.code,pf.id);
	else if (is_same(cmd,syscmd[12].cmd)&&((!club_mode&&pf.type>16)||(club_mode&&now_type>16)||in_file2("system/syswarn.id",pf.id)))
		sys_tem("bin/set_warn '%s' '%s'",pf.id,club_go.code);
	else if (is_same(cmd,syscmd[16].cmd)) sys_tem("bin/viewlog 1 '%s' '%s'",tid2,club_go.code);
	else if (is_same(cmd,syscmd[3].cmd)) sys_tem("bin/viewlog 2 '%s' '%s'",pf.id,club_go.code);
	else if (is_same(cmd,syscmd[17].cmd)) sys_tem("bin/viewlog 3 '%s' '%s'",pf.id,club_go.code);
	else if (is_same(cmd,syscmd[7].cmd)) sys_tem("bin/ed_lev %s",pf.id);
	else if (is_same(cmd,syscmd[4].cmd)&&(pf.type>=16||(club_mode&&cpf.type>=17)))	 {
		if (!club_mode&&pf.type<18&&!in_file2("system/sysmenu.id",pf.id)) {
			mess("���� �޴��������� ID�� �ƴմϴ�.");
			return;
		}
		sys_tem("bin/ed_menu '%s' '%s' %d",pf.id,club_go.code,pf.localmenu);
		re_flash();
		re_bbs=Yes;
	}
    else if (is_same(cmd,syscmd[11].cmd)) kill_tty_port();   //�ش� ��Ʈ ���̱�
	else if (is_same(cmd,syscmd[13].cmd)) say_wall(1);	  //��ü�����޼���
	else if (pf.type>=19&&is_same(cmd,syscmd[18].cmd)){
		//�ٸ� ȸ���� ���̵�� �ٷΰ�
		pf3=pf;
		if (strlen(mmc[1])>2) strcpy(buff,mmc[1]);
		else {
			printf("\n ġȯ�� ȸ���� ���̵� �Է��ϼ���.  >>");
			lineinput(buff,8);printf("\n");
		}
        if (strlen(buff)>2&&read_pf4(buff)&&pf2.type<18) {
			del_stat(tty);			 //���±�� ȭ�� �ش� ��Ʈ�� ������ �����Ѵ�.
			pf=pf2;
			time(&pf.login);			   //���� �ð� ����
            if (pf.type>0) save_pf(37);
			mess("%s���� %s(%s)�� ġȯ�Ǿ����ϴ�.",pf3.id,pf.id,pf.name);
			return;
		}
	}
	else if (is_same(cmd,syscmd[ 9].cmd)&&(pf.type>=18||in_file2("system/darkman.id",pf.id))) {
		if (dark_mode) {dark_mode=No;check_wait(0);}
		else		   {dark_mode=Yes;}
	}
	else if (pf.type>=18&&is_same(cmd,syscmd[10].cmd))	{
		if (dark_mode) {dark_mode=No;check_wait(0);}
        if (cfg.count_server==1) system("rm -f user/moni/*.dat");
        else           {dark_mode=Yes;for (n=0;n<=cfg.count_server;n++) unlink_file("user/moni/moni%d.dat",n);}
	}
	else if (pf.type>=18&&(is_same(cmd,syscmd[19].cmd)||is_same(cmd,syscmd[20].cmd)))  {
		if (strlen(mmc[1])>0) {
			if (is_same(cmd,syscmd[19].cmd)) sprintf(buff2,"logo/%s",mmc[1]);
			else							 sprintf(buff2,"system/%s",mmc[1]);
			xedit(buff2);
		}
		else {
			printf("\n ### TEXT ȭ�ϸ� >>");
			lineinput(buff,20);check_han(buff);printf("\n");
			if (strlen(buff)>2) {
				if (is_same(cmd,syscmd[19].cmd)) sprintf(buff2,"logo/%s",buff);
				else							 sprintf(buff2,"system/%s",buff);
				xedit(buff2);
			}
		}
	}
	set_stat(1,"");
	re_bbs=Yes;
	re_print=Yes;
	club_go=old_club_go;
	club_mode=old_club_mode;
    pf=old_pf;
	now_type=old_now_type;
	go=old_go;
	set_nowmenu();
	read_bcfg();
}


sys_xlogo(int mode)
{
	int n;
	char buff[100];
	if (mode==1) printf("\n   ����� XLOGO ��ȣ>>");
	else		 printf("\n   �ٿ���� XLOGO ��ȣ>>");
	lineinput(buff,2);check_han(buff);printf("\n");n=atoi(buff);
	if (strlen(buff)<1||is_quit(buff)||n<1||n>10) return;
	sprintf(buff,"logo/_xlogo%d.log",n);
	if (mode==1) up_text(0,buff);	//ȭ���� ���۹޾� filename�� �����Ѵ�.
	else		 sz_file(buff);
}



help_sys_menu()
{
	cls();
	printfL2();
	view_text("frm/help/syscmd.hlp");
}


//�û�޴����
do_sysop(int mode)
{
	char buff[80];
	char filename[80];
	//�û����� ���
	if (!in_file2("system/sysopid.txt",pf.id)) {
		mess("%s���� �û�޴��� ��ϵ� ID�� �ƴմϴ�.",pf.id);
		return No;
	}
	set_stat2(19,"bin/sysop");     //�۾�
	set_rec_(3,19,0,buff);

	if (mode==1) sys_tem("bin/sysop '%s'",pf.id);
	else if (mode==2) sys_tem("bin/sysop root");
    else if (mode==3) sys_tem("bin/pf %s",pf.id);
	set_signal();  //�ñ׳� �缳��
	set_stat(1,"");     //�۾�
	re_flash(); 		//�޴� �ٽ� �б�
	return Yes;
}



//��ȣȸ ��� �޴����
do_csysop()
{
	char buf[80];
	//��ȣȸ����� ���
	if (!club_mode) return;
	if (pf.type<16&&now_type<14) {mess("��ȣȸ ��� �޴��� ����Ҽ� ���� ����Դϴ�.");return;}
	if (pf.type>=18||check_pass(ccfg.passsys,8,"��ȣȸ����ɾ�ȣ>>")) {
		set_stat2(19,"bin/csysop");     //�۾�
		set_rec_(2,19,0,"bin/csysop");
		sys_tem("bin/csysop '%s' '%s' '%s'",club_go.code,go.code,pf.id);
		set_signal();  //�ñ׳� �缳��
		set_stat(1,"");     //�۾�
	}
	read_ccfg();
	re_print=Yes;
}


//�Խ��� ����ڿ� ��� �޴�
do_bsysop()
{
	char buff2[80];
	char oldcode[80];

	re_print=Yes;

	//��ȣȸ����� ���
	if (pf.type<17&&(!is_char(BBSTYPE,go.type))) {
		goto _error_bsysop;
	}
	if (now_type>=17||pf.type>=17||bbs_man(pf.id)) {
		if (pf.type>=16||check_pass(bcfg.passsys,4,"�Խ��Ǵ���ɾ�ȣ>>")) {
			strcpy(oldcode,go.code);
			if (strlen(mmc[1])>0&&pf.type>=18) strcpy(go.code,mmc[1]);
			set_stat2(19,"bin/bsysop");     //�۾�
			set_rec_(2,19,0,"bin/bsysop");
//			  sys_exec(1,0,"bin/bsysop","bsysop",go.code,club_go.code,pf.id);
			sys_tem("bin/bsysop '%s' '%s' '%s'",go.code,club_go.code,pf.id);
			set_signal();  //�ñ׳� �缳��
			strcpy(go.code,oldcode);
			set_stat(1,"");         //�۾�
		}
	}
	else goto _error_bsysop;
	re_flash();
	return;

	_error_bsysop:
	message("�Խ��� �Ǵ� �ڷ�ǿ��� ����ڰ� ����ϴ� ����Դϴ�.");
	return;
}


kill_tty_port()
{
	int n;
    char buff[100];
    if (pf.type<18&&!in_file2("system/syskill.id",pf.id)) return;
    if (strlen(mmc[1])<3) {
        printf("\n ���� ���� �����ų ID >>");
        lineinput(buff,8);check_cmd(buff);printf("\n");
        strcpy(mmc[1],mmc[0]);
    }
    if (strlen(mmc[1])>2) {
        for(n=0;n<=cfg.count_server;n++) kill_portid(n,mmc[1],"");
    }
}


//50�ް� �Ǵ� 100�ް� �÷���
sys_add_down(int num)
{
	char buff[100];
	int who=0;
	int i,n;
	int old,new;
	re_print=Yes;
	if (mmc[1][0]==0) {
		view_user(1);
		printf("�ٿ�뷮�� �ø� �ش��ȣ �Ǵ� ���̵� �Է��ϼ��� >>");
        lineinput(buff,8);check_cmd(buff);printf("\n");
        strcpy(mmc[1],buff);            //�ڱ��� �Ż󺸿���
	}
    if (mmc[1][0]==0||read_pf4(mmc[1])==No) {                  //ã���� pf2�� ����
        mess("�ش�Ǵ� ȸ���� �����ϴ�.");
		return;
	}
	old=pf2.down;
	pf2.down  +=(num*1000000);
	new=pf2.down;
    if (pf2.type>0) save_pf2(38);
	sprintf(buff,"%s(%s)�� �ٿ�뷮  %dMB -> %dMB",pf2.id,pf2.name,old/1000000,new/1000000);
	message(buff);
}


sys_change_passwd()
{
	char buff[200];
	if (mmc[1][0]==0) {
        printf("\n ��ȣ���� ȸ�� ���̵� >>");
        lineinput(buff,8);printf("\n");check_cmd(buff);
	}
    if (mmc[1][0]==0||!read_pf4(mmc[1])) {     //ã���� pf2�� ����
		message("�ش�Ǵ� ȸ���� �����ϴ�.");
		return;
	}
    if (pf2.type>=18) {
        mess("�����Ҽ� ���� ȸ������Դϴ�.");
        return;
    }
	sprintf(buff,"bin/ed_pf %s %s %d 2",pf.id,pf2.id,3);
	sys_tem(buff);
}


do_shell()
{
	int n;
	if (pf.type>17&&in_file2("system/sysshell.id",pf.id)) {
		if (sys_tem("bin/check_sh")==2) {
			set_stat(19,"shell");     //�۾�
			ioctl(0,TCSETA,&termold);	//���� �͹̳� ��� ����
			system("/bin/sh");
			ioctl(0,TCSETA,&termall);	//���� �͹̳� ��� ����
			set_stat(1,"");
		}
	}
}


say_wall()
{
    FILE *fp;
    char filename[200],buff[3000],buff2[3000],messbuff[3000];
    MONI amoni[1500];
    int i,ii,i2,n,nn;
    int count,found;
    strcpy(filename,"system/wall.msg");
    xedit2(filename);
	cls();
    if (!is_file(filename)) return;
    if ((fp=fopen(filename,"r"))==NULL) return;
    ii=0;
    strcpy(messbuff,"");
    while (strlen(messbuff)<2000&&fgets(buff,200,fp)!=NULL) {
        strcat(messbuff,buff);
    }
    strcat(messbuff,"\n");
    fclose(fp);
    if (strlen(messbuff)<3) return;
    printf("%s",messbuff);
    if (ii<0||!yesno("\n ���� �޼����� �����ұ��? (Y/n)>>",Yes)) return;


    for (i=0;i<1500;i++) strcpy(amoni[i].id,"");
    count=0;
    for (n=0;n<=cfg.count_server;n++) {
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename,"r"))==NULL) continue;
        while (count<1500&&fread(&moni,sizeof_moni,1,fp)) {
            if (strlen(moni.tty)==0||strlen(moni.id)<3||moni.mode==0) continue;
            if ((moni.mode==5||moni.mode==6||moni.mode==8||
                  moni.mode==14||moni.mode==16||moni.mode==18||moni.mode==19)) continue;
            found=No;
            for (i=0;i<count;i++) {
                if (strlen(amoni[i].id)>1&&amoni[i].hostno==moni.hostno&&is_same(amoni[i].tty,moni.tty)) {
                    found=Yes;
                    break;
                }
            }
            if (!found) amoni[count++]=moni;
        }
        fclose(fp);
    }
    for (i=0;i<count;i++) {
        if (amoni[i].mode==3) { //��ȭ��
            sprintf(buff,"7[21;1H\n[21;1H%s8",messbuff);
            to_tty(amoni[i].hostno,amoni[i].tty,buff);      //��ȭ���϶�
        }
        else to_tty(amoni[i].hostno,amoni[i].tty,messbuff);
    }
}


re_flash()
{
	char buff[10];
	int old_club_mode=club_mode;
	GO oldclub=club_go;
	if (is_same(mmc[0],"load")) mess("�޴��� �ý��� ������ �ٽ� �ε�Ǿ����ϴ�.");
	read_cfg();
    if (read_pf4(pf.id)) pf=pf2;
	strcpy(buff,go.code);
	strcpy(go.code,"");
	re_bbs=Yes;
	re_print=Yes;
	read_bbsgood_color();
	if (old_club_mode) {
		club_mode=Yes;
		read_ccfg();
		read_actype();			 //��ȣȸ����� �̸��� �о��  ->actype
		read_cpf(pf.id);
		check_cpf();			 //����б� -> now_type
		club_go=oldclub;
		re_club=No;
	}
	go_code(buff);
}



check_sys_pass(int num)
{
	FILE *fp;
	char filename[200];
	SYSCMD syscmd[30];		  //Ư�� ȯ��ȭ��
	char cmd[100];
	int i;
	strcpy(cmd,mmc[0]);
	strcpy(filename,"system/syscmd.dat");
	if ((fp=fopen(filename,"r"))!=NULL) {
		for (i=0;i<30;i++) {
			fread(&syscmd[i],sizeof(syscmd[0]),1,fp);
		}
		fclose(fp);
		//��ȣ����
		if (num<30&&strlen(syscmd[num].pass)>0&&
			!check_pass(syscmd[num].pass,8,"SPASS>>")) {
			return No;
		}
	}
	return Yes;
}

view_pdsdir()
{
	char buff[300];
	if (!is_typepds) return;
	del_space(go.dir);
	sprintf(buff,"ls %s -l > %s",go.dir,TMP_FILE);
	printf("\n  <  %s > \n",buff);
	system(buff);
	xedit(TMP_FILE);
	unlink(TMP_FILE);
}


set_limit()
{
    int n;
    char filename[11][100];
    strcpy(filename[1],"frm/down/noupmain.log");
    strcpy(filename[2],"frm/down/noupclub.log");
    strcpy(filename[3],"frm/down/nowrmain.log");
    strcpy(filename[4],"frm/down/nowrclub.log");
    strcpy(filename[5],"frm/down/nodnmain.log");
    strcpy(filename[6],"frm/down/nodnclub.log");
    strcpy(filename[7],"frm/down/noinchat.log");
    strcpy(filename[8],"frm/down/noinmail.log");
    strcpy(filename[9],"frm/down/noinclub.log");
    while(1) {
        printf("\n");
        printf("  1. ����   �������   :%s: %s\n",filename[1],nbit(is_file(filename[1]),"����","����"));
        printf("  2. ��ȣȸ �������   :%s: %s\n",filename[2],nbit(is_file(filename[2]),"����","����"));
        printf("  3. ����   ��������   :%s: %s\n",filename[3],nbit(is_file(filename[3]),"����","����"));
        printf("  4. ��ȣȸ ��������   :%s: %s\n",filename[4],nbit(is_file(filename[4]),"����","����"));
        printf("  5. ����   �ٿ�����   :%s: %s\n",filename[5],nbit(is_file(filename[5]),"����","����"));
        printf("  6. ��ȣȸ �ٿ�����   :%s: %s\n",filename[6],nbit(is_file(filename[6]),"����","����"));
        printf("  7. ��ȭ�� ��������   :%s: %s\n",filename[7],nbit(is_file(filename[7]),"����","����"));
        printf("  8. ����   ��������   :%s: %s\n",filename[8],nbit(is_file(filename[8]),"����","����"));
        printf("  9. ��ȣȸ ��������   :%s: %s\n",filename[9],nbit(is_file(filename[9]),"����","����"));
        printf(" 10. W���� R�б� D�ٿ� C��ȭ S��ȣȸ ����/����(system/limit.id)\n");
		n=numselect();
        if (n<=0) break;
        if (n==1) xedit2(filename[1]);
        if (n==2) xedit2(filename[2]);
        if (n==3) xedit2(filename[3]);
        if (n==4) xedit2(filename[4]);
        if (n==5) xedit2(filename[5]);
        if (n==6) xedit2(filename[6]);
        if (n==7) xedit2(filename[7]);
        if (n==8) xedit2(filename[8]);
        if (n==9) xedit2(filename[9]);
        if (n==10) edit_limit_id_file();
    }
}


edit_limit_id_file()
{
    FILE *fp;
    char filename[300];
    char buff[300],buff1[300],buff2[300],buff3[300];
    check_dir("user/limit");
    strcpy(filename,"user/limit/limit.id");
    xedit2(filename);
    system("rm -f user/limit/limit_*.id");
    if ((fp=fopen(filename,"r"))==NULL) return;
    while(fgets(buff,200,fp)!=NULL) {
        strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");
        sscanf(buff,"%s %s %s",buff1,buff2,buff3);
        if (strlen(buff1)<3) continue;
        sprintf(buff,"%s %s",buff1,buff2);
        sprintf(buff3,"user/limit/limit_%u.id",(word)buff1[0]%255);
        put_line(buff,buff3);
    }
    fclose(fp);
}

set_notwrite(int mode)
{
    FILE *fp;
    char tid[10],buff[100];
    if (pf.type<15&&!in_file2("system/nwsys.id",pf.id)) return;
    if (mode==1) strcpy(tid,mmc[1]);
    else {
        if (!bbs_num(mmn[1])) goto _fail;
        strcpy(tid,bbs.id);
    }
    _cont1:
    if (!read_pf4(tid)) goto _fail;
    printf("\n %s(%s) �������:%s\n",pf2.id,pf2.name,rbit(pf2.mymode,24,"�������Ѿ���","�������"));
    printf(" �������(Y)  ��������(N)  ���(Q)  ����ID���(LS) >>");
    lineinput2(buff,10);check_han(buff);
         if (is_same(buff,"y")) pf2.mymode=set_bit(pf2.mymode,24,1);
    else if (is_same(buff,"n")) pf2.mymode=set_bit(pf2.mymode,24,0);
    else if (is_same(buff,"ls")) {
        fp=fopen(ID_FILE,"r");
        while(fread(&pf2,sizeof_pf,1,fp)){
            if (pf2.type>0&&bit(pf2.mymode,24)) {
                printf(" %-8s (%-8s) �����������\n",pf2.id,pf2.name);
            }
        }
        fclose(fp);
        pressenter();
        goto _cont1;
    }
    printf(" %s -> %s\n",pf2.id,rbit(pf2.mymode,24,"�������Ѿ���","�������"));
    save_pf2(11);
    return;

    _fail:
    printf("\n ## ���� ���� ��� ID ����  .nw1 <�Ƶ�>  .nw2 <�Խ��ǹ�ȣ> \n");
}
