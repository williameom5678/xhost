////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����  �������� ������ �޾ƾ� �մϴ�.
// ���� : ��ɾ�м�
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//Ư���� �޴��ΰ�?
check_smenu(char *buffcmd)
{
	char buff[200],buff1[200];
	int i;
	if (go.type=='s') {      //��ȣȸ�� �����Ͽ���.
		club_go=go;
		club_mode=Yes;
		re_club=Yes;
		strcpy(sub_go,mmc[2]);
		p_command=Yes;
	}
	else if (do_order_command(go.code,buffcmd));
	else if (go.type=='t') {
		strcpy(buff1,"?");
		if (is_same(go.wtimes,"loc1"))      sprintf(buff1,"%d",pf.localmenu);
		else if (is_same(go.wtimes,"loc2")) sprintf(buff1,"%d",pf.type);
		else if (is_same(go.wtimes,"loc3")) sprintf(buff1,"%d",pf.job);
		else if (is_same(go.wtimes,"loc4")) sprintf(buff1,"%d",get_age(pf)/10);
		else if (is_same(go.wtimes,"loc5")) sprintf(buff1,"%d",get_age(pf)/10);
		strchng2(go.dir,"?",buff1);
		strcpy(auto_cmd,go.dir);
		if (strlen(auto_cmd)<1) strcpy(auto_cmd,go.code);
	}
	else if (go.type=='r') {
		if (is_same(go.code,"wcmemo")) write_cmemo();
		else if (is_same(go.wtimes,"cmd")) {
			strcpy(auto_cmd,go.dir);
			if (strlen(auto_cmd)<1) strcpy(auto_cmd,go.code);
			return;
		}
		else {
			sprintf(buff,"%s",go.code);
			strcpy(mmc[0],go.code);
			strcpy(mmc[1],"");strcpy(mmc[2],"");strcpy(mmc[3],"");
			mmn[0]=0;mmn[1]=0;mmn[2]=0;mmn[3]=0;
		}
		if (in_file2("system/nocode.txt",mmc[0])) return;
		if (do_basic_command(buff,buff));
		else  do_order_command(buff,buffcmd);
		if (go.type=='r') go_prev();
	}
	else if (go.type=='p'||go.type=='h') {
		run_pagescreen();
		re_print=Yes;
		goto _fail;
	}
	else if (is_char(EXETYPE,go.type)) {		  //�ܺθ���϶�
		exec_menu(0);
	}
	return;

	_fail:
	i=0;
	while(i++<30&&is_char("phrt",go.type)) {
		go_prev();
	}
}


//command �� �޴��� �ִ� ���ù�ȣ�ΰ�?
do_menu_command()
{
	FILE *fp;
	int ret=No;
	GO	oldgo,tgo,oldclub;
	int n,i;
	int found=No;
	char buff[180];
	char buff2[180];
	char deep;

	if (strlen(mmc[0])==0||strlen(mmc[0])>4||go.type!='m') return No;
	p_command=No;			//������ �Է��� P�ΰ�?
	if (strlen(mmc[0])==1&&
	   (strcasecmp(mmc[0],"z")==0||strcasecmp(mmc[0],"p")==0||
		strcasecmp(mmc[0],"t")==0||strcasecmp(mmc[0],"n")==0||
		strcasecmp(mmc[0],"w")==0||strcasecmp(mmc[0],"x")==0||
		strcasecmp(mmc[0],"a")==0||strcasecmp(mmc[0],"b")==0)) return No;
	if (strcasecmp(mmc[0],"ls")==0||strcasecmp(mmc[0],"xx")==0||
        strcasecmp(mmc[0],"lz")==0||strcasecmp(mmc[0],"xx")==0||
		strcasecmp(mmc[0],"say")==0||strcasecmp(mmc[0],"say")==0) return No;
	oldgo=go;	//������ �����ڵ� ����
	oldclub=club_go;
	found=No;

	//�⺻������ �޴������д´�.
	sprintf(buff,"%s/menustru/%s.ind",club_go.dir,go.code);
	local_logo(buff);
	if ((fp=fopen(buff,"r"))!=NULL) {
		while(fread(&tgo,sizeof_go,1,fp)) {
			if (is_same(tgo.num,mmc[0])) {
				found=Yes;
				break;
			}
		}
		fclose(fp);
	}
	if (!found) {  //������ �����޴����� �˻� ��������
		if ((fp=fopen(NOW_MENU,"r"))==NULL) return No;
		fseek(fp,(go.rec+1)*sizeof_go,SEEK_SET);
		while(fread(&tgo,sizeof_go,1,fp)) {
			if (tgo.type>' '&&strlen(tgo.code)>1) break;
		}
		deep=tgo.deep;
		fseek(fp,tgo.rec*sizeof_go,SEEK_SET);
		while(fread(&tgo,sizeof_go,1,fp)&&deep<=tgo.deep) {
	//		printf("1113*%d*%s->%s*%s*\n",tgo.deep,go.code,tgo.code,mmc[0]);
			if (deep==tgo.deep&&is_same(tgo.num,mmc[0])&&strlen(tgo.code)>0&&tgo.type>' ') {
				found=Yes;
				break;
			}
		}
		fclose(fp);
	}
	if (!found) return No;

	go=tgo;
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	list_mode=No;
	ret=Yes;
	read_bcfg();
	up_level_cash();

	if (go.type=='t') check_smenu("");
	else if (check_type('j')||check_type('i')) {
		go=oldgo;		//�ٽ� ������
		re_logo=No;
		return No;	   //���԰��ɵ��?
	}
	else if (go.type=='r') {
		strcpy(mmc[0],go.code);
		do_order_command(mmc[0],"");
		go=oldgo;		//�ٽ� ������
		p_command=Yes;
		if (go.type=='r') go_prev();
	}
	else if (is_char(EXETYPE,go.type)) {		  //�ܺθ���϶�
		p_command=Yes;
		exec_menu();
		go=oldgo;		//�ٽ� ������
		if (is_char(EXETYPE,go.type)) go_prev();
	}
	else if (go.type=='s') {      //��ȣȸ�� �����Ͽ���.
		check_smenu("");
	}
	else if (go.type=='p'||go.type=='h') check_smenu("");
	else if (is_same(go.code,"wmail"))   check_smenu("");
	return Yes;
}


//command �� �Խ����� �ڷ� ���ù�ȣ�ΰ�?
do_list_command()
{
	int ret=No;
	int key;
	int nn=0;
	int short_no=No;
	int found=No;

	if (empty(mmc[0])||mmn[0]<1||!is_bbspds) return (No);
	//���� ȭ�鿡 ��Ÿ�� ������ ������ 2�ڸ��� �Է�������
	nn=mmn[0];
	if (mmc[0][0]!='0'&&nn<99&&NOW_TOPNO>99&&bit(cfg.mode,10)==0) {
		nn=(int)(NOW_TOPNO/100)*100+nn;
		if (NOW_TOPNO%100>nn%100) nn+=100;
		if (nn>NOW_ALLNO&&NOW_ALLNO>100) nn-=100;
		short_no=Yes;
	}
	if (nn>0) {
		if (bbs_num(nn)==Yes)  found=Yes;
		if (!found&&short_no&&bbs_num(mmn[0])) found=Yes;
		if (found) {
			bbsreadall=No;				  //�������ִ�.
			bbs_read_num(); 			  //���� Ƚ������
			cls();
			bbs_read(0);				   //�ش��ȣ �������
			add_read_numn(bbs.num); 	  //���� ǥ�� �߰�
		}
		else {
			message("�ش��ȣ�� ã���������ϴ�.");
		}
		ret=Yes;
	}
	return (ret);
}


//�⺻��ɾ� ó��
do_basic_command(char *buff,char *buffstr)
{
	int ret=Yes;
	int i;
	char ch,buff2[100];
	//�⺻��� ����
	//�����϶� ó��
	if (is_same(go.code,"rmail")&&list_mode&&(**mmc==0||stristr("f b p m go re an ls dir w r x xy xx",mmc[0])))
		mail_readok();
	if (mmc[0][0]==0&&go.type=='m') return (Yes);

	if (in_file2("system/nocode.txt",mmc[0])) return Yes;
	ret=No;
	if (is_bbspds) {
		ret=Yes;
			 if (is_same(mmc[0],"f"))        bbs_next();
		else if (is_same(mmc[0],"b")&&list_mode) bbs_read(-1);
		else if (is_same(mmc[0],"b"))        bbs_back();
		else if (is_same(mmc[0],"ls"))       bbs_list(mmn[1]);
        else if (is_same(mmc[0],"lz"))       bbs_list(mmn[1]);
		else if (is_same(mmc[0],"j"))        bbs_jump(mmn[1]);
		else if (is_same(mmc[0],"dir"))      bbs_list(mmn[1]);
		else if (is_same(mmc[0],"d"))        bbs_del(3); //���� (�����ڷ�Ƿ� �ű�)
		else if (is_same(mmc[0],"dd"))       bbs_del(0); //����
		else if (is_same(mmc[0],"ddd"))      bbs_del(2); //���� (�������Ҿȵ�)
		else if (is_same(mmc[0],"del"))      bbs_del(0);
		else if (is_same(mmc[0],"dr"))       bbs_del(1); //����
		else if (is_same(mmc[0],"ud"))       bbs_edit(mmn[1],0);
		else if (is_same(mmc[0],"ap"))       bbs_edit(mmn[1],0);    //����,����,ȭ�ϸ���
		else if (is_same(mmc[0],"e"))        bbs_edit(mmn[1],0);
		else if (is_same(mmc[0],"ee"))       bbs_edit(mmn[1],0);
		else if (is_same(mmc[0],"ed"))       bbs_edit(mmn[1],0);
		else if (is_same(mmc[0],"ekey"))     bbs_edit(mmn[1],2);
		else if (is_same(mmc[0],"dansi"))    bbs_dansi(mmn[1]); //����,�Ƚ�����
		else if (is_same(mmc[0],"w"))        bbs_write(0);
		else if (is_same(mmc[0],"www"))      bbs_addblank();
		else if (is_same(mmc[0],"ws"))       bbs_write(99);
		else if (is_same(mmc[0],"up"))       bbs_write(0);
		else if (is_same(mmc[0],"ups"))      bbs_write(99);
		else if (is_same(mmc[0],"down"))     bbs_down(0);          //�ڷ�ٿ�
		else if (is_same(mmc[0],"dn"))       bbs_down(0);          //�ڷ�ٿ�
		else if (is_same(mmc[0],"see"))      bbs_down(2);          //�ڷ�ٿ�
		else if (is_same(mmc[0],"dna"))      bbs_arj();           //�ڷ���ຸ��
		else if (is_same(mmc[0],"la"))       bbs_arj();           //���ຸ��
		else if (is_same(mmc[0],"mj"))       bbs_arj();           //�ڷ�
		else if (is_same(mmc[0],"ra"))       bbs_re();
		else if (is_same(mmc[0],"rn"))       bbs_re();
		else if (is_same(mmc[0],"rt"))       bbs_re(); //�亯
		else if (is_same(mmc[0],"rr"))       bbs_re();            //���
		else if (stristr(go.code,"mail")&&is_same(mmc[0],"r"))     bbs_re();            //����
		else if (is_same(mmc[0],"re"))       bbs_re();            //����
		else if (is_same(mmc[0],"an"))       bbs_re();            //�亯������
		else if (is_same(mmc[0],"at"))       bbs_re();
		else if (is_same(mmc[0],"li"))       bbs_search();
		else if (is_same(mmc[0],"lt"))       bbs_search();
		else if (is_same(mmc[0],"ln"))       bbs_search();
		else if (is_same(mmc[0],"lf"))       bbs_search();
		else if (is_same(mmc[0],"lk"))       bbs_search();
		else if (is_same(mmc[0],"le"))       bbs_search();
		else if (is_same(mmc[0],"lu"))       bbs_search();
		else if (is_same(mmc[0],"ld"))       bbs_search();
		else if (is_same(mmc[0],"lg"))       bbs_search();
		else if (is_same(mmc[0],"lc"))       bbs_search();
		else if (is_same(mmc[0],"lkw"))      bbs_search();
		else if (is_same(mmc[0],"gd"))       bbs_gd(0);
		else if (is_same(mmc[0],"gdg"))      bbs_gd(1);
		else if (is_same(mmc[0],"gds"))      bbs_gd(3);
		else if (is_same(mmc[0],"gdd"))      bbs_gd(2);
		else if (is_same(mmc[0],"mv"))       bbs_copy(buff);
		else if (is_same(mmc[0],"mvv"))      bbs_copy(buff);
		else if (is_same(mmc[0],"mvmain"))   bbs_copy(buff);
		else if (is_same(mmc[0],"mvvmain"))  bbs_copy(buff);
		else if (is_same(mmc[0],"mvclub"))   bbs_copy(buff);
		else if (is_same(mmc[0],"mvvclub"))  bbs_copy(buff);
		else if (is_same(mmc[0],"cp"))       bbs_copy(buff);
		else if (is_same(mmc[0],"cpp"))      bbs_copy(buff);
		else if (is_same(mmc[0],"cpmain"))   bbs_copy(buff);
		else if (is_same(mmc[0],"cppmain"))  bbs_copy(buff);
		else if (is_same(mmc[0],"cpclub"))   bbs_copy(buff);
		else if (is_same(mmc[0],"cppclub"))  bbs_copy(buff);
		else if (is_same(mmc[0],"pr"))       bbs_pr(0);
		else if (is_same(mmc[0],"prdn"))     bbs_pr(1);
		else if (bbs_man(pf.id)&&is_same(mmc[0],"ok1"))   bbs_ok(6);
		else if (bbs_man(pf.id)&&is_same(mmc[0],"ok2"))   bbs_ok(7);
		else if (bbs_man(pf.id)&&is_same(mmc[0],"okdel")) bbs_ok(8);
		else if (bbs_man(pf.id)&&is_same(mmc[0],"ok11"))  bbs_ok(11);
		else if (bbs_man(pf.id)&&is_same(mmc[0],"ok12"))  bbs_ok(12);
		else if (is_same(mmc[0],"no"))       bbs_ok(0);
		else if (is_same(mmc[0],"ok"))       bbs_ok(1);
		else if (is_same(mmc[0],"de"))       bbs_ok(2);
		else if (is_same(mmc[0],"ke"))       bbs_ok(3);
		else if (is_same(mmc[0],"pa"))       bbs_ok(4);
		else if (is_same(mmc[0],"type"))     bbs_type();
		else if (is_same(mmc[0],"bdn"))      bdn();               //�Խ��Ǵٿ�
		else if (is_same(mmc[0],"!"))        view_last_num(0);
		else ret=No;
	}
	if (ret==No) {
		ret=Yes;
			 if (is_same(mmc[0],"load"))     re_flash();          //�ý��� �ٽ� �б�
		else if (is_same(mmc[0],"llock"))     lock_file_test(buffstr);               //�Խ��Ǵٿ�
        else if (is_same(mmc[0],"lz")&&go.type=='m');
        else if (is_same(mmc[0],"ls")&&go.type=='m');
		else if (is_same(mmc[0],"z"))        {
			re_print=Yes;	  //ȭ����ǥ��
			printf("[01;29r");
			printf(VTXOFF);
		}
		else if (is_same(mmc[0],"go"))       {
			if (mmc[1][0]==0) findmenu(0);
			else if (go_code(mmc[1])) {strcpy(sub_go,mmc[2]);check_smenu(buffstr);}
			else re_print=No;
		} //�����ɼ���
		else if (is_same(mmc[0],"?1"))       disp_bbsstat();
		else if (is_same(mmc[0],"?2"))       dev();
		else if (is_same(mmc[0],"now"))      set_mymoni(buffstr);
		else if (is_same(mmc[0],"w")&&is_same(go.code,"top")) go_code("wmail"); //���Ϲٷΰ���
		else if (is_same(mmc[0],"w")&&is_same(go.code,"mail")) go_code("wmail"); //���Ϲٷΰ���
		else if (is_same(mmc[0],"w")&&is_same(go.code,"rmail")) go_code("wmail"); //���Ϲٷΰ���
		else if (is_same(mmc[0],"w")&&is_same(go.code,"smail")) go_code("wmail"); //���Ϲٷΰ���
		else if (list_mode&&is_same(mmc[0],"vv")) bbs_vv();
		else if (is_same(mmc[0],"wm"))       go_code("wmail"); //���Ϲٷΰ���
		else if (is_same(mmc[0],"pall"))     go_code("pds_all");
		else if (is_same(mmc[0],"ball"))     go_code("bbs_all");
		else if (is_same(mmc[0],"rm"))       go_code("rmail");
		else if (is_same(mmc[0],"s"))        go_code("ad");          //����
		else if (is_same(mmc[0],"t"))        go_top(0);               //ó������
		else if (is_same(mmc[0],"n"))        go_next(0);
		else if (is_same(mmc[0],"nn"))       go_next(1);  //������ ��������
		else if (is_same(mmc[0],"a"))        go_back(0);
		else if (is_same(mmc[0],"aa"))       go_back(1);  //������ ��������
		else if (is_same(mmc[0],"m"))        go_prev();
		else if (!club_mode&&!list_mode&&go.type=='m'&&is_same(go.code,"top")&&bit(cfg.mode,30)&&is_same(mmc[0],"p")) host_end(0);
		else if (is_same(mmc[0],"p"))        go_prev();
		else if (is_same(mmc[0],"pp"))       {go_prev();go_prev();}
		else if (is_same(mmc[0],"ii"))       go_code("notice");   //�������׺���
		else if (is_same(mmc[0],"r"))        go_code("rmail");
		else if (is_same(mmc[0],"i"))        view_notice(0,"notice");
		else if (is_same(mmc[0],"id")||is_same(mmc[0],"lv"))       view_level();        //������������
		else if (is_same(mmc[0],"xed"))      xed_file();      //��������
//        else if (is_same(mmc[0],"wtest1"))   wtest(1);  //�����׽�Ʈ
//        else if (is_same(mmc[0],"wtest2"))   wtest(2);  //�����׽�Ʈ
		else if (is_same(mmc[0],"tty"))      {
			system("id");fflush(NULL);
			printf("ID:%s NAME:%s TYPE:%d PORT:%s PID:%d HOSTNO:%d PORT:%d\n",pf.id,pf.name,pf.type,tty,getpid(),cfg.hostno,SERVER_PORT[0]);      //��������
			printf("HOST:%s  NIP:%s ������:%d\n",hostname,cfg.sip,cfg.count_server);
			printf("�л�:%s ",rbit(cfg.mprocmode,0,"����","��"));
			printf("LOCK:%s ",rbit(cfg.mprocmode,1,"����","����"));
			printf("%s\n",rbit(cfg.mprocmode,3,"FILELOCK","XLOCK"));
			printf("����IP:( ");
			for (i=0;i<=cfg.count_server;i++) printf("%d ",cfg.nip[i]);
			printf(")\n");
			fflush(NULL);
		}
		else if (is_same(mmc[0],"tty2"))      {
			for (i=0;i<5;i++)
				printf("PORT:%d\n",SERVER_PORT[i]);      //��������
			fflush(NULL);
		}
		else if (is_same(mmc[0],"mk"))       my_mark(0);      //�ҷ�����
		else if (is_same(mmc[0],"mks"))      my_mark(1);      //�����ϱ�
		else if (strlen(mmc[0])>2&&is_same(mmc[1],"say"))   to_say(0,mmc[0],"");    //�ӼӸ� ������
//��ڿ���
		else if (is_same(mmc[0],"whoami"))   whoami(mmc[1]);
		else ret=No;

//		  if (ret) check_smenu(buffstr);
	}

	return (ret);
}

do_order_command(char *buff,char *buffstr)
{
	int key=Yes;
	char buff2[200];
	int i2;

	if (in_file2("system/nocode.txt",buff)) return Yes;
	i2=DM;				   //������
		 if (is_same(buff,"wmail")) wmail(1,"");
	else if (is_same(buff,"wmall")) wmall();
    else if (is_same(buff,"fmail")) wmail(98,"");
	else if (is_same(buff,"vmail")) wmail(98,"");
	else if (is_same(buff,"chat" )) {
		if (!is_same(go.code,"chat")) {go_top(0);find_go("chat");}
		do_chat(1);
	}
	else if (stristr(buff,"chat_" )) {
		if (!stristr(go.code,"chat_")) {go_top(0);find_go(buff);}
		do_chat(1);
	}
	else if (is_same(buff,"find"    )) findmenu(3);
	else if (is_same(buff,"findpds2")) findmenu(4);                //���丮�˻�
	else if (is_same(buff,"findpds3")) findmenu(5);                //���丮�˻�
	else if (is_same(buff,"helpuse" )) help_cmd("help","helpuse");
	else if (is_same(buff,"helpcmd" )) help_cmd(go.dir,go.dir);
	else if (is_same(buff,"csysop"  )) do_cguest();       //��ȣȸ���Խ�û
	else if (is_same(buff,"cfindpf" )) sys_tem("bin/pf '%s' set_cfindpf '%s'",pf.id,pf.id);       //��ȣȸ���Խ�û
	else if (is_same(buff,"cguest"  )) do_cguest();       //��ȣȸ���Խ�û
	else if (club_mode&&is_same(buff,"cchat")) do_chat(2);        //��ȣȸ��ȭ��
	else if (is_same(buff,"guest"   )) do_guest();  //ȸ�����Ը޴�
	else if (is_same(buff,"pf_out"  )) do_pfout();        //���Խ�û����
	else if (is_same(buff,"ed_id"   )) do_edid();
	else if (is_same(buff,"sysmail" )) do_sysmail(1);   //�û��������
	else if (is_same(buff,"sysmail2")) do_sysmail(2);   //�û��������
	else if (is_same(buff,"sysmail3")) do_sysmail(3);   //�û��������
	else if (is_same(buff,"postfind")) do_postfind(mmc[1]);   //�����ȣ�˻�
	else if (is_same(buff,"maillist")&&pf.type>0) sys_tem("bin/ed_pf '%s' '%s' -8",pf.id,pf.id);
	else if (is_same(buff,"mailbook")&&pf.type>0) sys_tem("bin/ed_pf '%s' '%s' -8",pf.id,pf.id);
	else if (is_same(buff,"user"    )) go_code("userroom");
	else if (is_same(buff,"user2"   )) user_dir();
	else if (is_same(buff,"userdir" )) user_dir();
	else if (is_same(buff,"xvote"   )) xvote(0);             //��ǥ��
	else if (is_same(buff,"svote"   )) xvote(1);             //���� ��ǥ��
	else if (is_same(buff,"u1"      )) uu_exec(1);    //����� ��ƿ
	else if (is_same(buff,"u2"      )) uu_exec(2);    //����� ��ƿ
	else if (is_same(buff,"u3"      )) uu_exec(3);    //����� ��ƿ
	else if (is_same(buff,"u4"      )) uu_exec(4);    //����� ��ƿ
	else if (is_same(buff,"u5"      )) uu_exec(5);    //����� ��ƿ
	else if (is_same(buff,"u6"      )) uu_exec(6);    //����� ��ƿ
	else if (is_same(buff,"u7"      )) uu_exec(7);    //����� ��ƿ
	else if (is_same(buff,"u8"      )) uu_exec(8);    //����� ��ƿ
	else if (is_same(buff,"u9"      )) uu_exec(9);    //����� ��ƿ
	else if (is_same(buff,"gg"      )) gg_code(0);    //��õ�����ڵ�
	else if (is_same(buff,"g0"      )) gg_code(0);    //��õ�����ڵ�
	else if (is_same(buff,"g1"      )) gg_code(1);    //��õ�����ڵ�
	else if (is_same(buff,"g2"      )) gg_code(2);    //��õ�����ڵ�
	else if (is_same(buff,"g3"      )) gg_code(3);    //��õ�����ڵ�
	else if (is_same(buff,"g4"      )) gg_code(4);    //��õ�����ڵ�
	else if (is_same(buff,"g5"      )) gg_code(5);    //��õ�����ڵ�
	else if (is_same(buff,"g6"      )) gg_code(6);    //��õ�����ڵ�
	else if (is_same(buff,"g7"      )) gg_code(7);    //��õ�����ڵ�
	else if (is_same(buff,"g8"      )) gg_code(8);    //��õ�����ڵ�
	else if (is_same(buff,"g9"      )) gg_code(9);    //��õ�����ڵ�
	else if (is_same(buff,"g10"     )) gg_code(10);   //��õ�����ڵ�
	else if (is_same(buff,"m0"      )) mm_code(0);    //��õ�����ڵ�
	else if (is_same(buff,"m1"      )) mm_code(1);    //��õ�����ڵ�
	else if (is_same(buff,"m2"      )) mm_code(2);    //��õ�����ڵ�
	else if (is_same(buff,"m3"      )) mm_code(3);    //��õ�����ڵ�
	else if (is_same(buff,"m4"      )) mm_code(4);    //��õ�����ڵ�
	else if (is_same(buff,"m5"      )) mm_code(5);    //��õ�����ڵ�
	else if (is_same(buff,"m6"      )) mm_code(6);    //��õ�����ڵ�
	else if (is_same(buff,"m7"      )) mm_code(7);    //��õ�����ڵ�
	else if (is_same(buff,"m8"      )) mm_code(8);    //��õ�����ڵ�
	else if (is_same(buff,"m9"      )) mm_code(9);    //��õ�����ڵ�
	else if (is_same(buff,"n0")||is_same(buff,"m10")) mm_code(10);    //��õ�����ڵ�
	else if (is_same(buff,"n1")||is_same(buff,"m11")) mm_code(11);    //��õ�����ڵ�
	else if (is_same(buff,"n2")||is_same(buff,"m12")) mm_code(12);    //��õ�����ڵ�
	else if (is_same(buff,"n3")||is_same(buff,"m13")) mm_code(13);    //��õ�����ڵ�
	else if (is_same(buff,"n4")||is_same(buff,"m14")) mm_code(14);    //��õ�����ڵ�
	else if (is_same(buff,"n5")||is_same(buff,"m15")) mm_code(15);    //��õ�����ڵ�
	else if (is_same(buff,"n6")||is_same(buff,"m16")) mm_code(16);    //��õ�����ڵ�
	else if (is_same(buff,"n7")||is_same(buff,"m17")) mm_code(17);    //��õ�����ڵ�
	else if (is_same(buff,"n8")||is_same(buff,"m18")) mm_code(18);    //��õ�����ڵ�
	else if (is_same(buff,"n9")||is_same(buff,"m19")) mm_code(19);    //��õ�����ڵ�
	else if (is_same(buff,"m20"))      mm_code(20);    //��õ�����ڵ�
	else if (is_same(buff,"m21"))      mm_code(21);    //��õ�����ڵ�
	else if (is_same(buff,"m22"))      mm_code(22);    //��õ�����ڵ�
	else if (is_same(buff,"m23"))      mm_code(23);    //��õ�����ڵ�
	else if (is_same(buff,"m24"))      mm_code(24);    //��õ�����ڵ�
	else if (is_same(buff,"m25"))      mm_code(25);    //��õ�����ڵ�
	else if (is_same(buff,"m26"))      mm_code(26);    //��õ�����ڵ�
	else if (is_same(buff,"m27"))      mm_code(27);    //��õ�����ڵ�
	else if (is_same(buff,"m28"))      mm_code(28);    //��õ�����ڵ�
	else if (is_same(buff,"m29"))      mm_code(29);    //��õ�����ڵ�
	else if (is_same(buff,"m30"))      mm_code(30);    //��õ�����ڵ�
	else if (is_same(buff,"news1"))    set_newsmode(1,mmc[1]);    //�������̱�
	else if (is_same(buff,"news2"))    set_newsmode(2,mmc[1]);    //�������̱�
	else if (is_same(buff,"news3"))    set_newsmode(3,mmc[1]);    //�������̱�
	else if (is_same(buff,"news4"))    set_newsmode(4,mmc[1]);    //�������̱�
	else if (is_same(buff,"ddm"     )) set_ddmode(0);    //�������ڷ� ����
	else if (is_same(buff,"/ear"    )) to_say(0,"",buffstr);    //�ӼӸ� ������
	else if (is_same(buff,"ear"     )) {set_stat(1,"");to_say(0,"",buffstr);}    //�ӼӸ� ������
	else if (is_same(buff,"ear2"    )) to_say(3,"",buffstr);    //�ӼӸ� ������
	else if (is_same(buff,"/say"    )) to_say(0,"",buffstr);    //�ӼӸ� ������
	else if (is_same(buff,"say"     )) {set_stat(1,"");to_say(0,"",buffstr);}    //�ӼӸ� ������
	else if (is_same(buff,"say2"    )) to_say(3,"",buffstr);    //�ӼӸ� ������
	else if (is_same(buff,"say5"    )) to_say(5,"",buffstr);    //��Ʈ��ũ �޼���
	else if (is_same(buff,"all"     )) to_all_say(0,0,buffstr); //��ü�޼���
	else if (is_same(buff,"all1"    )) to_all_say(1,0,buffstr); //��ü�޼���
	else if (is_same(buff,"all2"    )) to_all_say(2,0,buffstr); //��ü�޼���
	else if (is_same(buff,"all3"    )) to_all_say(3,0,buffstr); //��ü�޼���
	else if (is_same(buff,"all4"    )) to_all_say(4,0,buffstr); //��ü�޼���
	else if (is_same(buff,"all9"    )) to_all_say(9,0,buffstr); //��ü�޼���
	else if (is_same(buff,"to"      )) {set_stat(1,"");to_say(0,"",buffstr);}    //�ӼӸ� ������
	else if (is_same(buff,"tele"    )) {set_stat(1,"");to_say(0,"",buffstr);}    //�ӼӸ� ������
	else if (is_same(buff,"call"    )) to_say(0,"",buffstr);    //�ӼӸ� ������
	else if (is_same(buff,"say5"    )) to_say(5,"",buffstr);    //��Ʈ��ũ �޼���
	else if (is_same(buff,"/m"      )) {buff2cmd(buffstr,buff2,2);to_memo(0,mmc[1],buff2);}  //�޼���������
	else if (is_same(buff,"/mm"     )) {buff2cmd(buffstr,buff2,2);to_memo(0,mmc[1],buff2);}  //�޼���������
	else if (is_same(buff,"mm"      )) {buff2cmd(buffstr,buff2,2);to_memo(0,mmc[1],buff2);}  //�޼���������
	else if (is_same(buff,"/x"))       host_end(0);             //����
	else if (is_same(buff,"//"      )) view("frm/say/macro.txt");
	else if (buff[0]=='/')             to_say(0,"",buffstr);    //�ӼӸ�
	else if (buff[0]=='.')             sys_menu(buffstr);       //�ý��� ���� Ư���۾�
	else if (is_same(buff,"setlogo"))  set_off(2);      //ȭ����¸��
	else if (is_same(buff,"setjob"))   set_off(7);      //ȭ����¸��
	else if (is_same(buff,"setansi"))  set_off(6);      //ȭ����¸��
	else if (is_same(buff,"setline"))  set_off(11);     //ȭ����¸��
	else if (is_same(buff,"settab"))   set_off(4);      //ȭ����¸��
	else if (is_same(buff,"setani"))   set_off(8);      //ȭ����¸��
	else if (is_same(buff,"setyes"))   set_off(19);     //ȭ����¸��
	else if (is_same(buff,"repass"))   check_epass(1);      //ȭ����¸��
	else if (is_same(buff,"pm"))       per_memo();      //���θ޸���
	else if (is_same(buff,"findbbs"))  bbs_find("findbbs");
	else if (is_same(buff,"findpds"))  bbs_find("findpds");
	else if (is_same(buff,"findpf"))   bbs_find("findpf");
	else if (is_same(buff,"findpf1"))  bbs_find("findpf1");
	else if (is_same(buff,"findpf2"))  bbs_find("findpf2");
	else if (is_same(buff,"findpf3"))  bbs_find("findpf3");
	else if (is_same(buff,"countlog")) view_countlog(0);
	else if (is_same(buff,"findall"))  bbs_find("");
	else if (is_same(buff,"bbsfind"))  per_memo();
	else if (is_same(buff,"logodndn")) logodndn();      //����α״ٿ�
	else if (is_same(buff,"pmemo"))    per_memo();      //���θ޸���
	else if (is_same(buff,"wmemo"))    write_smemo(mmc[1]);   //��� ���Ÿ޸�
	else if (is_same(buff,"smemo"))    write_smemo(mmc[1]);   //��� ���Ÿ޸�
	else if (is_same(buff,"rmemo"))    {if (!check_smemo()) printf("\n ## [%s]�Կ��� �� ������ �����ϴ�.\n",pf.id);}
	else if (is_same(buff,"wait"))     check_wait(1);
	else if (is_same(buff,"brk"))      sys_tem("bin/viewlog 997 '%s' '%s'",club_go.code,go.code);
	else if (is_same(buff,"rk")||is_same(buff,"rank"))   {sys_tem("bin/viewlog '%s' '%s' '%s'",
		bit(cfg.levelmode,2)?"997":"998",pf.id,go.wtimes);re_print=Yes;}
	else if (is_same(buff,"rk2")||is_same(buff,"rank2")) {sys_tem("bin/viewlog '%s' '%s' '%s'",
		bit(cfg.levelmode,2)?"998":"997",pf.id,go.wtimes);re_print=Yes;}
	else if (is_same(buff,"rl"))                       {sys_tem("bin/viewlog 999 '%s' '%s'",pf.id,go.wtimes);re_print=Yes;}
	else if (is_same(buff,"df"))       {do_exec("bin/findfree");pressenter();} //�뷮
	else if (is_same(buff,"free"))     {do_exec("bin/findfree");pressenter();} //�뷮
	else if (is_same(buff,"log"))      {
		if (pf.type>=16&&strlen(mmc[1])>0) sys_tem("bin/viewlog 1 '%s' '%s'",mmc[1],club_go.code);
		else sys_tem("bin/viewlog 1 '%s' '%s'",pf.id,club_go.code);
	}
	else if (is_same(buff,"vtree"))    findmenu(0);
	else if (is_same(buff,"vtreem"))   findmenu(2);
	else if (is_same(buff,"rc"))       log_rc(0);               //������
	else if (is_same(buff,"rcc"))      log_rc(1);               //������
	else if (is_same(buff,"login"))    log_rc(0);               //������
	else if (is_same(buff,"xdic"))     xdic(1);
	else if (is_same(buff,"xdic2"))    xdic(2);
	else if (is_same(buff,"re-news"))  {system("rm -f tmp/news/*.nws");mess("�������� ����");}
	else if (is_same(buff,"v"))        printf(VTXOFF);
	else if (is_same(buff,"cmd"     ))      change_helpcmd();
	else if (now_type>=18&&strlen(buffstr)>2&&is_char("?~.!",buffstr[strlen(buffstr)-1])) to_all_say(8,0,buffstr); //��ü�޼���
	else if (is_same(buff,"zz"))       {
		printf(VTXOFF);printf(WTXOFF);view("frm/system/_zz.scr");
		re_print=Yes;
	}
	else if (is_same(buff,"rlog")||is_same(buff,"relog")||is_same(buff,"login"))     {
		set_id_end();	   //�ش� ���̵� ���ӱ�� ����
		start_id(1,"");
		re_print=Yes;			//ȭ�����
		re_logo=Yes;			//����ȭ�����
		re_bbs=Yes; 			//�Խ��Ƿε�
		re_club=No; 			//��ȣȸ�ʱ�ȭ
		go_top(0);
	}
	else if (is_same(buff,"xx")||is_same(buff,"xy")||is_same(buff,"bye"))      {strcpy(auto_cmd,"x");host_end();}            //����
	else if (is_same(buff,"x"))        {strcpy(auto_cmd,"x");endhost_ok();}
	else if (is_same(buff,"tm"))       sys_tem("bin/xbcal");
	else if (is_same(buff,"cal"))      sys_tem("bin/xbcal");
	else if (is_same(buff,"us" ))      view_user(1,1,mmc[1]);
	else if (is_same(buff,"who"))      view_who(mmc[1]);
	else if (is_same(buff,"us1"))      view_user(0,1,mmc[1]);
	else if (is_same(buff,"us2"))      view_user(2,1,mmc[1]);
	else if (is_same(buff,"us3"))      view_user(3,1,mmc[1]);
	else if (is_same(buff,"us4"))      view_user(4,1,mmc[1]);
	else if (is_same(buff,"us5"))      view_user(5,1,mmc[1]);
	else if (is_same(buff,"us6"))      view_user(6,1,mmc[1]);
	else if (is_same(buff,"us7"))      view_user(7,1,mmc[1]);
	else if (is_same(buff,"us8"))      view_user(8,1,mmc[1]);
	else if (is_same(buff,"us9"))      view_user(9,1,mmc[1]);
	else if (is_same(buff,"as"))       view_user(1,9,mmc[1]);
	else if (is_same(buff,"as1"))      view_user(0,9,mmc[1]);
	else if (is_same(buff,"as2"))      view_user(2,9,mmc[1]);
	else if (is_same(buff,"as3"))      view_user(3,9,mmc[1]);
	else if (is_same(buff,"h"))        view_help(0);     //����
	else if (is_same(buff,"?"))        view_help(1);        //����
	else if (is_same(buff,"c"))        view_help(1);        //��ɾ�ȳ�
	else if (is_same(buff,"help"))     view_help(2);
	else if (is_same(buff,"abc"))      edit_pf(mmc[1],1);    //ȸ���Ż����
	else if (is_same(buff,"pfc"))      edit_pf(mmc[1],1);    //
	else if (is_same(buff,"pf"))       {
		if (strlen(mmc[1])>2&&!is_same(mmc[1],pf.id)) view_pfh(mmc[1]); 		//ȸ���Ż�
		else if (pf.type>0) view_pf(pf.id);
		else mess("�մ��� ����Ҽ� �����ϴ�.");
	}
	else if ((is_same(buff,"pfs")||is_same(buff,"spf"))&&pf.type>=18) {
		if (strlen(mmc[1])<3&&sel_user("frm/pf/pfsel.mes",0))
			strcpy(mmc[1],moni.id);
		view_pfs(mmc[1]);	  //ȸ���Ż�
	}
	else if (is_same(buff,"cpf"))      {
		if (strlen(mmc[1])<1) strcpy(mmc[1],pf.id);
		view_cpf(mmc[1],1); 	//��ȣȸ�Ż�
		pressenter();
	}
	else if (is_same(buff,"cpfs")&&cpf.type>=17) {
		if (strlen(mmc[1])<1) strcpy(mmc[1],pf.id);
		view_cpf(mmc[1],cpf.type);	   //��ȣȸ�Ż�
		pressenter();
	}
	else if (is_same(buff,"pfb")&&!is_char("ok",go.type)) view_pfb("");

	else if (is_same(buff,"in")||is_same(buff,"fi"))       is_login(mmc[1]);    //�������ӿ���
	else if (is_same(buff,"lst"))      bbs_search();        //��ü���� ã��
	else if (is_same(buff,"lsc"))      bbs_search();        //������ ���� ã��
	else if (is_same(buff,"lsl"))      bbs_search();        //������ ���� ã��
	else if (is_same(buff,"msg")||is_same(buff,"me"))  {set_msg(0,mmc[1]);set_stat(1,"");}  //������ġ ����
	else if (is_same(buff,"han"))     check_short(0,buff2);
	else if (is_same(buff,"_ver"))  mess("V:%s M:%s D:%s",VER1,MAKENUM,MAKEDATE);
	else key=No;
	if (key&&is_char("rejnp",go.type)) go_prev();
	return key;
}


disp_bbsstat()
{
	printf(".....................\n");
	printf(" file   :%s*\n",NOW_FILE);
	printf(" clubdir:%s*\n",club_go.dir);
	printf(" bbsid  :%s name:%s key:%s\n",bbs.id,bbs.name,bbs.key);
	printf(" title  :%s*\n",bbs.title);
	printf(" bbsnum :%d  REC:%d  ALLREC:%d*\n",bbs.num,NOW_REC,NOW_ALLREC);
	printf(" bbspos :%d  size:%d line:%d read:%d  down:%d  an:%d\n",bbs.posi,bbs.size,bbs.line,bbs.read,bbs.down,bbs.ok);
	printf(" bbsfile:%s* size:%d dir:%s*\n",bbs.filename,bbs.filesize,go.dir);
	printf(".....................");
	pressenter();
	re_print=No;
}


//MARK
typedef struct {
	char  mode;
	char  club[9];
	char  go  [9];
	char  sp  [40];
	int   sp1;
	int   sp2;
	time_t date;
} MARK;

my_mark(int mode)	   //0�ҷ�����	1:����
{
	FILE *fp;
	char filename[300];
	char buff[200];
	int i,n,found=No;
	MARK mark;
	GO old_club_go,old_go;
	int old_club_mode;
	old_go=go;
	old_club_go=club_go;
	old_club_mode=club_mode;
	if (guest_mode||bit(cfg.mode,5)) {
		mess("�޴������� ���Ұ���");
		return;
	}
	sprintf(filename,"userdata/%s/%s/mark.dat",c_han(pf.id),pf.id);
	n=mmn[1];
	if (n<0||n>9) n=0;
	if (mode==0) {
		if ((fp=fopen(filename,"r"))==NULL) return;
		fseek(fp,n*sizeof(mark),SEEK_SET);
		found=fread(&mark,sizeof(mark),1,fp);
		fclose(fp);
		if (is_same(old_club_go.code,mark.club)) strcpy(auto_cmd,mark.go);
		else sprintf(auto_cmd,"go %s %s",mark.club,mark.go);
		re_print=Yes;
		message("�޴���ġ %d ����",n);
	}
	else if (mode==1) {
		if ((fp=fopen(filename,"r+"))==NULL) fp=fopen(filename,"w+");
		if (fp!=NULL) {
			mark.mode=1;
			strcpy(mark.club,club_go.code);
			strcpy(mark.go,go.code);
			time(&mark.date);
			fseek(fp,n*sizeof(mark),SEEK_SET);
			fwrite(&mark,sizeof(mark),1,fp);
			fclose(fp);
		}
		message("�޴���ġ %d ����",n);
	}
}

change_helpcmd()
{
	allmode=gtt_bit(allmode,5);
	if (bit(allmode,5)) view("frm/screen/helpcmd2.log");
	else				view("frm/screen/helpcmd1.log");
	re_print=Yes;
}


findmenu(int mode)
{
	char buff[300];
	unlink(TMP_FILE2);
	sys_tem("bin/findmenu %d '%s' '%s'",mode,club_go.code,TMP_FILE2);
	file2buff(auto_cmd,TMP_FILE2,30);
}


whoami(char *tid)
{
	if (!read_pf4(tid)) pf2=pf;
	printf("\n");
	printfL2();
	printf(" ID:%s �̸�:%s ����:%s ���:%d(%s)  ����:%d  ���:%s\n",
	pf2.id,pf2.name,pf2.nick,pf2.type,atype[pf2.type],pf2.level,datestr(20,how_old(START_TIME)));
	printfL2();
	printf("\n");
}

xdic(int mode)
{
	char para[10];
	strcpy(para,"-hi");
	if (is_han(mmc[1])) strcpy(para,"-h");
	unlink(TMP_FILE);
	if (strlen(mmc[1])<2) printf("\n ����:  xdic  �˻��ܾ�   (�ܾ�:2�� �̻�)\n");
	else if (mode==1) {
		sys_tem("zgrep %s '%s' /usr/share/engdic/* > %s",para,mmc[1],TMP_FILE);
		if (is_file(TMP_FILE)) view_text(TMP_FILE);
	}
	else if (mode==2) {
		view_str_file("xgame/xdic/xdic.dat",mmc[1]); //���ڿ����Ե� �ڷẸ��
	}
}


do_postfind(char *str)	 //�����ȣ�˻�
{
	char buff[100];
	strcpy(buff,str);
	if (strlen(buff)<1) {
		printf("\n �˻��� �����ȣ �Ǵ� �ּҸ� �Է��ϼ��� >>");
		lineinput(buff,10);printf("\n");
		if (strlen(buff)<2) return;
	}
	view_str_file("system/_post.txt",buff); //���ڿ����Ե� �ڷẸ��
	printf("\n");
}

lock_file_test(char *buffstr)
{
	printf("IP:%s -> %d \n",cfg.lock_ip_addr,c_lock(atoi(mmc[1]),mmc[2]));
}


logodndn()
{
	if (!bit(cfg.logomode,11)) {mess("logodndn ����");return;}
	if (go.type=='m') sys_tem("%s %s/logo/%s.mnu",cfg.sz,club_go.dir,go.code);
	if (is_bbspds	) sys_tem("%s %s/logo/%s.*",cfg.sz,club_go.dir,go.code);
}
