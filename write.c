////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
//	  �Խ��� ���� ��ƾ
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

bbs_write(int ups_mode)
{
	char TNOW_FILE[300],TNOW_FILED[300];

    _last2=3;;;;;;;;;;;;;;;;;;;;;;;;
	if (bbs_ltmode) {		 //����������
		bbs_ltmode=No;
		set_filename();
		check_bbs();
	}
	strcpy(TNOW_FILE,NOW_FILE);strcpy(TNOW_FILED,NOW_FILED);
	//�����̸� ���� ��ƾ�� �ٸ�
    if (is_same(go.code,"rmail")||is_same(go.code,"wmail")||is_same(go.code,"cmail")||is_same(go.code,"mbox")) {
        wmail(1,"");
        return;
	}
    if (bit(pf.mymode,24)) {
        if (!view("frm/pf/notwrite2.log"))
            printf("\n### %s���� ��������Ǿ����ϴ�.\n",pf.id);
        return;
    }
	touch_file(NOW_FILE);touch_file(NOW_FILED);

    if (bit(limit_mode,1)) {
        if (!view("frm/pf/notwrite.log")) mess("�������");
        return;
    }
	if (pf.type<19&&ups_mode) ups_mode=0;
	else if (is_typebbs) bbs_writebbs(ups_mode);
	else				 bbs_writepds(ups_mode);
	strcpy(NOW_FILE,TNOW_FILE);strcpy(NOW_FILED,TNOW_FILED);
    _last2=14;;;;;;;;;;;;;;;;;;;;;;;;
}


bbs_writebbs(int ups_mode)
{
	FILE *fp;
	char buff[250];
	char buff2[250];
	char dir[280];
	char filename[280];
	char qfilename[180];
	char filename2[180];
	GO oldgo;
	BBS oldbbs;
	int n,nn,i,ret;
	int quest_mode;
	int quest_sec_mode=No;	//������ ���������
	int is_nofile=No;
	int bbs_mode=is_typebbs;
	int old_level=pf.level;
	int old_down=pf.down;
	int now_num;
	char from_user=No;		//����ڿ������� ����
	time_t down_start_time;
	time_t down_end_time;

	oldgo=go;
	oldbbs=bbs;
	ret=Yes;
	re_bbs=Yes;
	re_print=Yes;


    if (pf.type<18) {
        if (!club_mode&&view("frm/down/nowrmain.log")) goto _fail;
        if ( club_mode&&view("frm/down/nowrclub.log")) goto _fail;
    }

	sprintf(qfilename,"%s/logo/%s.qst",club_go.dir,go.code); //������ȭ��

    sprintf(buff,"%s/logo/%s.nwr",club_go.dir,go.code);
    if (is_file(buff)) {view(buff);goto _fail;}
    sprintf(buff,"%s/logo/all.nwr",club_go.dir);
	if (is_file(buff)) {view(buff);goto _fail;}

	if (pf.type<19&&ups_mode) ups_mode=0;
	if (guest_mode) {
		if (bit(cfg.infomode,1));
		else if (go.type=='g');
		else if (go.type=='y');
		else  {
			if (!view("frm/error/errorw.log")) mess("�մ��� ������ �����ϴ�.");
			return No;
		}
		pressenter();
	}

	//��Ͽ�������
	if (!bit(bcfg.wmode,18)) {
		if (view("frm/editor/uploadq.%s",bbs_tail())) {
			if (!yesno2("",!bit(bcfg.downmode,16))) goto _fail;
		}
	}

	sprintf(filename,"%s/logo/%s.liw",club_go.dir,go.code);
	ret=No;
	if (bit(limit,1)) view2("frm/_nodeall.log");
	else if (!is_char(BBSTYPE,go.type)) 	message("��� ���� �޴��� �ƴմϴ�.");
    else if (pf.type<18&&(go.type=='z'||go.type=='b'))      message("��ü�ڷ�ǿ� ����Ҽ� �����ϴ�.");
	else if (bbs_man(pf.id))				ret=Yes;  //������϶� ���
	else if (bit(bcfg.mode,28)&&in_file2("system/node_net.txt",tty)) ret=No;
	else if (bit(bcfg.modelist,6)&&!in_file2(filename,pf.id)) return No;
	else if (is_same(go.code,"userroom"))   message("����� �������� ������ �ȵ˴ϴ�.");
	else if (check_type('w'))               ret=No; //�޴� ���� ���Ȯ��
	else ret=Yes;

	if (!ret) return No;
	if (ss_check(1))		 return No;  //���� ����
	if (check_warn(1))		 return No;  //���� ���� ����
	if (!guest_mode&&wtimes()) return No;	  //���� Ƚ�� �˻�

	//��Ͻþȳ�ȭ��
	view("%s/logo/%s.bbs",club_go.dir,go.code);

	strcpy(bbs.id,"");
    strcpy(bbs.passwd,"");
	strcpy(bbs.name,"");
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	bbs.posi=0;
	bbs.gd=' ';
	bbs.del=No;
	bbs.size=0;

	unlink(TMP_FILE);
	unlink(TMP_FILE1);
	unlink(TMP_FILE2);

	//��������
	quest_mode=No;
	if (!bit(bcfg.wmode,6)&&is_file(qfilename)) {
		if (bit(bcfg.wmode,7)||yesno3("frm/write/sul1.log",Yes)) {
			file_line(buff,qfilename,1);	//ù�� �ľ�
			if (stristr(buff,"_{���")&&yesno3("frm/write/sul2.log",Yes))
				quest_sec_mode=Yes;  //������ ���������
			set_stat2(5,"��������");     //����
			sys_tem("bin/quest %s %s %s",pf.id,qfilename,TMP_FILE2);
			quest_mode=Yes;
			if (!is_file(TMP_FILE2)) goto _fail;	  //��ҽ�
		}
	}

	//�����Է�
    if (write_bbs_title()==No) goto _fail;      //��ҽ�

	set_stat(5,bbs.title);								   //����

	//�⺻�����Է�
	if (write_bbs_quest()==No) goto _fail;

	if (pf.type<19&&ups_mode) ups_mode=0;
	//�����Է�
	unlink(TMP_FILE);
	if (go.type=='-'||bit(bcfg.wmode,11));      //�������
	else {										//���������� �ƴϸ�?
		if (ups_mode) ret=input_tmp(99); // ->TMP_FILE�� ����
		else		  ret=input_tmp(0);
		if (!is_file(TMP_FILE)||ret==No) goto _fail;			  //��ҽ�
	}

	strcpy(bbs.key,"");
    strcpy(bbs.passwd,"");
	bbs.read=bbs.del=bbs.ok=bbs.down=bbs.size=bbs.posi=0;
	bbs.gd=' ';
    if (strlen(bcfg.write_id)>2&&bbs_man(pf.id)) strcpy(bbs.id,bcfg.write_id); //������ ���̵�,�̸�
    else                                         strcpy(bbs.id,pf.id);             //������ ���̵�,�̸�
	strcpy(bbs.name,bit(bcfg.mode,8)?pf.nick:pf.name);
	if (quest_sec_mode==Yes) {
		strcpy(bbs.id,"�����");             //������ ���̵�,�̸�
		strcpy(bbs.name,"�����");
	}
	time(&bbs.date);
	time(&bbs.date2);
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	//�˻�������
	if (bit(bcfg.wmode,10)) bbs_key_edit(0);

	//�Խù����� ����
	if (is_file(TMP_FILE2)) {
		file_add(TMP_FILE2,TMP_FILE);		//TMP_FILE2 �� TMP_FILE�� �߰�
		file_move(TMP_FILE2,TMP_FILE);
	}
	//�ڽ��� ������ �߰���
	if (!guest_mode&&!is_char("okvz",go.type)&&!bit(cfg.mode,9)&&!bit(bcfg.wmode,9)) {
		sprintf(buff,"userdata/%s/%s/pfinfo.sig",c_han(pf.id),pf.id);
		file_add(TMP_FILE,buff);
	}
	if (is_same(go.code,"notice")) bbs.gd='*';
	if (bit(bcfg.wmode,25)) bbs.gd='*';
	if ((go.type=='-'||bit(bcfg.mode,25))&&is_file(TMP_FILE)) unlink(TMP_FILE);
	//�Խù��߰�
	if (bit(bcfg.wmode,5)) {
		file_line(buff,TMP_FILE,1);
		sprintf(buff2,"%s %s",bbs.title,buff);
		all_trim(buff2);
		xstrncpy(bbs.title,buff2,60);
	}
	if (!bit(bcfg.wmode,8)) {
		if (strlen(bcfg.outbbs)>0) {
			//���� �Խ��ǿ� ����+����
			if (bit(bcfg.wmode,24)) {
				ret=bbs_add(1);
			}
			//���� �Խ��ǿ� ����
			if (bit(bcfg.wmode,24)||gtt_bit(bcfg.wmode,23)) {
				sprintf(NOW_FILE,"%s/menu/%s.tit",club_go.dir,bcfg.outbbs);
				sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,bcfg.outbbs);
				ret=bbs_add(1);
			}
		}
		else ret=bbs_add(0);
	}
	else {
		wmail4(bcfg.man1,TMP_FILE,bbs.title,0);
        sprintf(buff,"\n **%s ���� ���ϵ���(���)\n",pf.id);
        to_mess(bcfg.man1,buff);
		ret=Yes;
	}
	now_num=bbs.num;
	//��ü�ڷ������
	if (!ret) goto _fail;	//��ҽ�

	pf.todaymode=set_bit(pf.todaymode,1,1);   //���ñ۾���

	for (i=0;i<5;i++) {
		if (is_same(cfg.check_wgo[i],go.code)) {
			pf.todaymode=set_bit(pf.todaymode,21+i,1);	 //���ñ۾���
			break;
		}
	}

    _last2=10;;;;;;;;;;;;;;;;;;;;;;;;
	cpf_down_down(1,0);   //�ش絿ȣȸ �ٿ�� ����
	if (bbs_mode) {uall.write++;uall.writesize+=bbs.size;}
	else		  {uall.up	 ++;uall.upsize   +=bbs.filesize;}
	if (!is_char("guo",go.type)) {    //������ �ڷ���� �ƴҶ�
		if (!from_user||bit(cfg.downmode,3)) {
			//����,�ٿ�뷮����
			bbs_down_up(bbs.filesize,pf.id,1);
		}
	}
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	set_rec(1,5);  //�۾����� ȭ�� ����
	if (!bit(bcfg.wmode,8)) add_zgo();		//��ü�ڷ�� ���
	unlink(TMP_FILE);	//�ӽ�ȭ�� ����
	check_bbs();
	re_print=Yes;
	set_stat(1,"");     //��������

	if (!bit(bcfg.wmode,3)) view("frm/editor/writeok.%s",bbs_tail());

	//���� 1����
	if (bit(bcfg.form,11)&&bbs_num(now_num)) {
		i=time_num(bbs.date);
		go_next_list();
		if (i!=time_num(bbs.date)) {
			if (bcfg.uplevel>0||bcfg.updown>0) {
			}
		}
	}
	check_bbs();
	bbs=oldbbs;
	pf_today(0,1,0);	   //����� ����
    if (pf.type>0) save_pf(21);
    unlink("tmp/zbtotalb.tmp");
    if (bit(cfg.zmode,25)) check_zbtotal(30*60,2);
    re_bbsnum(3);           //BREC 0:�б� 1:�ڵ�  2:����  3:��ü����  8:1���߰� 9:1������
    _last2=16;;;;;;;;;;;;;;;;;;;;;;;;
	return Yes;

	_fail:
	check_bbs();
	bbs=oldbbs;
	if (is_file(TMP_FILE)) unlink(TMP_FILE);   //�ӽ�ȭ�� ����
	view("frm/editor/writeerr.%s",bbs_tail());
	re_bbs=Yes;
	re_print=Yes;
	set_stat(1,"");     //��������
	check_cmd("");
    re_bbsnum(3);           //BREC 0:�б� 1:�ڵ�  2:����  3:��ü����  8:1���߰� 9:1������
    _last2=15;;;;;;;;;;;;;;;;;;;;;;;;
	return No;
}


bbs_writepds(int ups_mode)
{
	FILE *fp;
	char buff[250];
	char buff2[250];
	char dir[280];
	char filename[280];
	char qfilename[180];
	char filename2[180];
	GO oldgo;
	BBS oldbbs;
	int n,nn,i,ret,key;
	int quest_mode;
	int quest_sec_mode=No;	//������ ���������
	int is_nofile=No;
	int bbs_mode=is_typebbs;
	int old_level=pf.level;
	int old_down=pf.down;
	int now_num;
	char from_user=No;		//����ڿ������� ����
	time_t down_start_time;
	time_t down_end_time;
    char tNOW_FILE[300],tNOW_FILED[300],tgo_dir[300];

    strcpy(tNOW_FILE ,NOW_FILE );
    strcpy(tNOW_FILED,NOW_FILED);
    strcpy(tgo_dir   ,go.dir   );

	ret=No;
	oldgo=go;
	oldbbs=bbs;
	ret=Yes;
	re_bbs=Yes;
	re_print=Yes;
	sprintf(qfilename,"%s/logo/%s.qst",club_go.dir,go.code); //������ȭ��

    sprintf(buff,"%s/logo/%s.nwr",club_go.dir,go.code);
    if (is_file(buff)) {view(buff);goto _fail;}
    sprintf(buff,"%s/logo/all.nwr",club_go.dir);
	if (is_file(buff)) {view(buff);goto _fail;}

    if (pf.type<18) {
        if (!club_mode&&view("frm/down/noupmain.log")) goto _fail;
        if ( club_mode&&view("frm/down/noupclub.log")) goto _fail;
    }

	if (pf.type<19&&ups_mode) ups_mode=0;
	if (guest_mode) {
		if (bit(cfg.infomode,2));
		else if (go.type=='g');
		else if (go.type=='y');
		else  {
			if (!view("frm/error/errorup.log")) mess("�մ��� ����Ҽ� �����ϴ�.");
			return No;
		}
	}
	//��Ͽ�������
	if (ups_mode) printf(" ##UPS MODE##\n");
	else if (!bit(bcfg.wmode,18)) {
		if (view("frm/editor/uploadq.%s",bbs_tail())) {
			if (!yesno2("",!bit(bcfg.downmode,16))) goto _fail;
		}
	}
	if (pf.type<18&&(cfg.up_limit||bcfg.up_limit)) {
		key=No;
		if (cfg.up_limit>0) {
			i=set_tag_down(4);	//���� ��ü��/�ٿ��
            if (i>=cfg.up_limit) {a_num[8]=i;a_num[9]=cfg.up_limit;key=Yes;}
		}
		if (!key&&bcfg.up_limit>0) {
			i=set_tag_down(6);	//���� ��ü��/�ٿ��
            if (i>=bcfg.up_limit) {a_num[8]=i;a_num[9]=bcfg.up_limit;key=Yes;}
		}
		if (key) {
			view("frm/editor/up_limit.log");
			goto _fail;
		}
	}

	sprintf(filename,"%s/logo/%s.liw",club_go.dir,go.code);
	ret=No;
	if (bit(limit,1)) view2("frm/_nodeall.log");
	else if (!is_char(BBSTYPE,go.type)) 	message("��� ���� �޴��� �ƴմϴ�.");
    else if (pf.type<18&&(go.type=='z'||go.type=='b'))      message("��ü�ڷ�ǿ� ����Ҽ� �����ϴ�.");
	else if (bbs_man(pf.id))				ret=Yes;  //������϶� ���
	else if (bit(bcfg.mode,28)&&in_file2("system/node_net.txt",tty)) ret=No;
	else if (bit(bcfg.modelist,6)&&!in_file2(filename,pf.id)) return No;
	else if (is_same(go.code,"userroom"))   message("����� �������� ������ �ȵ˴ϴ�.");
	else if (go.type=='c')                  message("CD �� �ڷ�ǿ� ����Ҽ� �����ϴ�.");
	else if (go.type=='d')                  message("���丮 �ڷ�ǿ� ����Ҽ� �����ϴ�.");
	else if (check_type('w'))               ret=No; //�޴� ���� ���Ȯ��
	else ret=Yes;

	if (!ret) return No;
	if (is_typebbs&&ss_check(1))	return No;	//���� ����
	if (is_typepds&&ss_check(2))	return No;	//��� ����
	if (check_warn(1))				return No;	//���� ���� ����
	if (!guest_mode&&is_bbspds&&wtimes()) return No;	 //���� Ƚ�� �˻�

	//��Ͻþȳ�ȭ��
	view("%s/logo/%s.bbs",club_go.dir,go.code);

	strcpy(bbs.id,"");
    strcpy(bbs.passwd,"");
	strcpy(bbs.name,"");
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	bbs.posi=0;
	bbs.gd=' ';
	bbs.del=No;
	bbs.size=0;

	unlink(TMP_FILE);
	unlink(TMP_FILE2);

	//�����Է�
    if (write_bbs_title()==No) goto _fail;      //��ҽ�
	set_stat(5,bbs.title);								   //����

	//�⺻�����Է�
	unlink(TMP_FILE1);
	if (write_bbs_quest()==No) goto _fail;

	//�����Է�
	unlink(TMP_FILE);
	if (go.type=='-'||bit(bcfg.wmode,11));      //�������
	else {										//���������� �ƴϸ�?
		ret=input_tmp(0);						//������ups�ƴ�
		if (ret==No) goto _fail;			  //��ҽ�
	}
	strcpy(bbs.key,"");
    strcpy(bbs.passwd,"");
	bbs.read=bbs.del=bbs.ok=bbs.down=bbs.size=bbs.posi=0;
	bbs.gd=' ';
    if (strlen(bcfg.write_id)>2&&bbs_man(pf.id)) strcpy(bbs.id,bcfg.write_id); //������ ���̵�,�̸�
    else                                         strcpy(bbs.id,pf.id);             //������ ���̵�,�̸�
	strcpy(bbs.name,bit(bcfg.mode,8)?pf.nick:pf.name);
	if (quest_sec_mode==Yes) {
		strcpy(bbs.id,"�����");             //������ ���̵�,�̸�
		strcpy(bbs.name,"�����");
	}
	time(&bbs.date);
	time(&bbs.date2);

	strcpy(bbs.filename,"");
	//ȭ�ϸ��Է�,����
	check_dir(go.dir);
	if (!bbs_man(pf.id)&&pf.type<14&&bit(bcfg.downmode,1))		 is_nofile=Yes;
	else if (!bbs_man(pf.id)&&pf.type>=14&&bit(bcfg.downmode,2)) is_nofile=Yes;
	else {
		time(&down_start_time);    //�����۽ð�
		if	 (ups_mode) ret=up_file(99,buff,No);  //�û����� �� ->filename
		else {
			if (bit(cfg.downmode,20)&&check_free()<50) {view("frm/system/nospace.log");goto _fail;}
			ret=up_file(0,buff,Yes);   //�� ->filename
		}
		bbs.filesize=file_size(buff);
		str_filedir(buff,dir,filename);  //buff�� ���丮�� ���ϸ����� ����
        if (is_same(filename,"id.dat")||is_same(filename,"passwd")) {
            mess("%s ��ϺҰ�",filename);
            goto _fail;
        }
		filename[12]=0;del_space(filename);del_esc3(filename);
		xstrncpy(bbs.filename,filename,13);
		time(&down_end_time);
		if (!is_file(buff)||strlen(filename)==0||!ret) {
			if (bit(bcfg.downmode,11)) {
				strcpy(bbs.filename,bit(bcfg.downmode,13)?"ȭ�Ͼ���":"");
				ret=Yes;
				is_nofile=Yes;
			}
			else goto _fail;   //��ҽ�
		}
		if (bit(bcfg.wmode,12)&&bbs.filesize==0) goto _fail;	 //��ҽ�
		if (!is_nofile) {				   //�ڷ���� ���
			if (strlen(go.dir)<1) sprintf(go.dir,"%s/%s",club_go.dir,go.code);
			check_dir(go.dir);
			sprintf(filename,"%s/%s",dir,bbs.filename);
			if (!is_file(go.dir)||is_same(go.dir,"/")) {
				mess("### �ڷ�� ���丮(%s)�� �����ϴ�. \n",go.dir);
				goto _fail;
			}
			if (!is_file(filename)) {
				mess("### %s ȭ���� �����ϴ�. \n",filename);
				goto _fail;
			}
			del_esc3(go.dir);check_dir(go.dir);
			sprintf(buff2,"%s/%s",go.dir,bbs.filename);
            if (strlen(filename)>0&&strlen(buff2)>0) {
                sys_tem("cp -f '%s' '%s'",filename,buff2);
            }
			rm_tmpup();
			check_achive();  //������ bbs,go���� ȭ���� raró���Ѵ�.
		}
	}

	//�˻�������
	if (bit(bcfg.wmode,10)) bbs_key_edit(0);

	//�Խù����� ����
	if (is_file(TMP_FILE2)) {
		file_add(TMP_FILE2,TMP_FILE);		//TMP_FILE2 �� TMP_FILE�� �߰�
		file_move(TMP_FILE2,TMP_FILE);
	}
	//�ڽ��� ������ �߰���
	if (!guest_mode&&!is_char("okvz",go.type)&&!bit(bcfg.wmode,9)) {
		sprintf(buff,"userdata/%s/%s/pfinfo.sig",c_han(pf.id),pf.id);
		file_add(TMP_FILE,buff);
	}
	if (is_same(go.code,"notice")) bbs.gd='*';
	if (bit(bcfg.wmode,25)) bbs.gd='*';
	if ((go.type=='-'||bit(bcfg.mode,25))&&is_file(TMP_FILE)) unlink(TMP_FILE);
	//�Խù��߰�
	if (bit(bcfg.wmode,5)) {
		file_line(buff,TMP_FILE,1);
		sprintf(buff2,"%s %s",bbs.title,buff);
		all_trim(buff2);
		xstrncpy(bbs.title,buff2,60);
	}
	if (!bit(bcfg.wmode,8)) {
		if (strlen(bcfg.outbbs)>0) {
			//���� �Խ��ǿ� ����+����
			if (bit(bcfg.wmode,24)) {
				ret=bbs_add(1);
			}
			//���� �Խ��ǿ� ����
			if (bit(bcfg.wmode,24)||gtt_bit(bcfg.wmode,23)) {
				sprintf(NOW_FILE,"%s/menu/%s.tit",club_go.dir,bcfg.outbbs);
				sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,bcfg.outbbs);
				ret=bbs_add(1);
			}
		}
		else ret=bbs_add(0);
	}
	now_num=bbs.num;
	//��ü�ڷ������
	if (!ret) goto _fail;	//��ҽ�

	pf.todaymode=set_bit(pf.todaymode,2,1);   //�����۾�
	cpf_down_down(1,0);   //�ش絿ȣȸ �ٿ�� ����
	if (bbs_mode) {uall.write++;uall.writesize+=bbs.size;}
	else		  {uall.up	 ++;uall.upsize   +=bbs.filesize;}
	if (!is_char("guo",go.type)) {    //������ �ڷ���� �ƴҶ�
		if (!from_user||bit(cfg.downmode,3)) {
			//����,�ٿ�뷮����
			bbs_down_up(bbs.filesize,pf.id,1);
		}
	}
	set_rec(1,6);  //�۾����� ȭ�� ����

	if (!bit(bcfg.wmode,8)) add_zgo();		//��ü�ڷ�� ���
	unlink(TMP_FILE);	//�ӽ�ȭ�� ����
	check_bbs();
	re_print=Yes;
	set_stat(1,"");     //��������

	if (autoup) nostop=Yes; 	//�ڵ����
	if ( (is_typepds&&!bit(bcfg.downmode,18))||
		 (is_typebbs&&!bit(bcfg.wmode,3)) )
		view("frm/editor/writeok.%s",bbs_tail());

	//���� 1����
	if (bit(bcfg.form,11)&&bbs_num(now_num)) {
		i=time_num(bbs.date);
		go_next_list();
		if (i!=time_num(bbs.date)) {
			if (bcfg.uplevel>0||bcfg.updown>0) {
			}
		}
	}
    _last2=11;;;;;;;;;;;;;;;;;;;;;;;;
	check_bbs();
	bbs=oldbbs;
	pf_today(0,0,1);	   //���� ����
    if (pf.type>0) save_pf(22);
    unlink("tmp/zbtotalp.tmp");
    if (bit(cfg.zmode,25)) check_zbtotal(30*60,1);
    strcpy(NOW_FILE ,tNOW_FILE );
    strcpy(NOW_FILED,tNOW_FILED);
    strcpy(go.dir   ,tgo_dir   );
    re_bbsnum(3);           //BREC 0:�б� 1:�ڵ�  2:����  3:��ü����  8:1���߰� 9:1������
    _last2=12;;;;;;;;;;;;;;;;;;;;;;;;
	return Yes;

	_fail:
	check_bbs();
	bbs=oldbbs;
	rm_tmpup();
	if (is_file(TMP_FILE)) unlink(TMP_FILE);   //�ӽ�ȭ�� ����
	view("frm/editor/writeerr.%s",bbs_tail());
	re_bbs=Yes;
	re_print=Yes;
	set_stat(1,"");     //��������
	check_cmd("");
    strcpy(NOW_FILE ,tNOW_FILE );
    strcpy(NOW_FILED,tNOW_FILED);
    strcpy(go.dir   ,tgo_dir   );
    re_bbsnum(3);           //BREC 0:�б� 1:�ڵ�  2:����  3:��ü����  8:1���߰� 9:1������
    _last2=13;;;;;;;;;;;;;;;;;;;;;;;;
	return No;
}


write_bbs_quest()		//�⺻����
{
	int ret=Yes;
	char filename2[300];
	if (bit(bcfg.qmode,7)) { //�̸��غ������ �Է¹���
		ret=bit(bcfg.qmode,8);
		if (!ret) {
			view("frm/editor/quest%s.log",bbs_tail());
			ret=yesno2("",No);
		}
		if (ret) {
			if (bit(bcfg.qmode,11)) {
				sprintf(filename2,"bin/quest%s",bbs_tail());
				if (is_file(filename2)) {
					if (sys_tem("%s '%s' '%s'",filename2,TMP_FILE1,TMP_FILE2)==-1) return No;
				}
			}
			else {
				sprintf(filename2,"%s/logo/%s.qsh",club_go.dir,go.code);  //�̸��غ������
				if (!is_file(filename2)) sprintf(filename2,"%s/frm/editor/questh.%s",club_go.dir,bbs_tail());  //�̸��غ������
				if (is_file(filename2))
					sys_tem("bin/quest '%s' '%s' '%s'",pf.id,filename2,TMP_FILE2);
			}
		}
	}
	return Yes;
}


//�Խ��� �����Է�
write_bbs_title()
{
	int n,head_num=0;
	char buff[200];
    char head_buff[100];
	n=3;
	strcpy(head_buff,"");
	strcpy(bbs.title,"");
	if (bit(bcfg.wmode,4)) {
		strcpy(bbs.title,go.name);
		goto _skip;   //�����Է������ʰ� �ǳʶ�
	}

	while (n--) {
		strcpy(head_buff,"");
		if (!bit(bcfg.wmode,1)) {
			head_num=select_head(head_buff);  //�Ӹ��� ����
			if (head_num<0) return No;
		}
		view("frm/editor/writetit.%s",bbs_tail());
		if (head_num==0) doskey(bbs.title,60,1);
		else {
			printf("%s ",head_buff);
			doskey(bbs.title,60-(int)strlen(head_buff),1);
		}
		printf("\n");
		if (is_same(bbs.title,"q")||is_same(bbs.title,"p")) return No;
		if (bbs.title[0]==0&&go.type=='a') strcpy(bbs.title,go.name);
		if (strlen(bbs.title)<3) {
			sprintf(buff,"frm/editor/writeerq.%s",bbs_tail());
			if (yesno3(buff,Yes)) return No;
			continue;
		}
		break;
	}
	if (n<0||strlen(bbs.title)<3) return No;

	_skip:
    if (strlen(head_buff)>0) {
		sprintf(buff,"%s %s",head_buff,bbs.title);
        buff[60]=0;strcpy(bbs.title,buff);
	}
	return Yes;
}




//�˻������ mode=0 : �ű�	mode=1:����
bbs_key_edit(int mode)
{
	char buff[80];
	printf(" ### �˻��� (������,Ű���� / ã�¸��:lk Ű����)\n");
	printf(" �˻���� Ÿ���� �ڷḦ ��������� �˻��ϱ����� �ǹ��ִ� �ܾ��Դϴ�.\n");
	printf(" �˻���� 2~8bytes ������ �����ϱ� ������ �ܾ�� ���Ͻʽÿ�.\n");
	printf(" �˻�� �Է��ϼ���: (8��) ");
	if (mode) {
		printf ("(����%s) (0:�˻������) ",bbs.key);
		strcpy(buff,bbs.key);
        lineinput(bbs.key,8);check_han(buff);
        if (bbs.key[0]==0)    strcpy(bbs.key,buff);
        else if (is_quit(bbs.key)) strcpy(bbs.key,"");
	}
	else
		lineinput(bbs.key,8);
}


//���� bbs ����ü�� �����Ѵ�.(������ TMP_FILE�� ������ �Ǿ�� �Ѵ�.)
//mode=1 �̸� �Ϻ��ϰ� ������ (bbs,NOW_REC ���)
//mode=9 �����߰� ����
bbs_add(int mode)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	time_t t;
    BBS  bbs2,oldbbs;
    int i,lastposi,change_size,n,n2;
	char c;
	char buff[180];
	int tNOW_ALLNO	 =NOW_ALLNO ;	//������ ����
	int tNOW_NO 	 =NOW_NO	;
	int tNOW_ALLREC  =NOW_ALLREC;
	int tNOW_REC	 =NOW_REC	;

	oldbbs=bbs;
	//�Խù� �Ѽ� �ľ�
	n=0;
	//���������� ������
	if (bbs_ltmode) {bbs_ltmode=No;set_filename();check_bbs();}
    c_lock(1,NOW_FILE);                    //LOCKȮ��  1:Ȯ�� 2:����
	//�ӽÿ��� �о� �������� (fp2->fp3)
	bbs.line=1;
	bbs.size=0;
    bbs.del=No;
    bbs.ok=0;
    bbs.posi=bbs.size=bbs.read=bbs.down=0;

    //���� �߰�
    n2=file_size(TMP_FILE);    //�������
    if ((fp2=fopen(NOW_FILED,"r+"))==NULL) {
        touch_file(NOW_FILE);
        touch_file(NOW_FILED);
        fp2=fopen(NOW_FILED,"w+");         //����ȭ��
    }
    if (fp2==NULL) {mess("%s ���Ͽ������",NOW_FILED);goto return_no;}
    fseek(fp2,1,SEEK_END);
    bbs.posi=ftell(fp2)-1;
    if (bbs.posi<0) bbs.posi=0;
    if (mode==9||n2<1) ;    //�������
    else {
        if ((fp3=fopen(TMP_FILE,"r"))!=NULL) {          //�ӽ��ۼ�ȭ��
            fseek(fp2,bbs.posi,SEEK_SET);
            while (bbs.size<n2) {
                c=fgetc(fp3);             //�ӽÿ��� �о�
                if (c==0||(byte)c==255) break;
                if (c=='\n') bbs.line++;  //�ټ� ���
                fputc(c,fp2);             //���뿡 ����
                bbs.size++;
                if (bcfg.limitsize>0&&bbs.size>=bcfg.limitsize) break;
            }
            fclose(fp3);
        }
    }
    fclose(fp2);

	if (go.type=='o') {strcpy(bbs.id,"........");strcpy(bbs.name,"........");}
	del_tab(bbs.title);
	//����ȭ�� ����
    if ((fp1=fopen(NOW_FILE,"r+"))==NULL) {                   //����ȭ�� ������ �߰�
        fp1=fopen(NOW_FILE,"w+");                   //����ȭ�� ������ �߰�
    }
    if (fp1==NULL) goto return_no;
    fseekend(fp1);
    n=ftell(fp1)/sizeof_bbs;
    if (n>1000000) n=0;
    bbs2.num=1;
    bbs.num=1;
    if (n>0) {
        fseek(fp1,(n-1)*sizeof_bbs,SEEK_SET);
        if (fread(&bbs2,sizeof_bbs,1,fp1)) {
            bbs.num=bbs2.num+1;
        }
    }
    fseek(fp1,n*sizeof_bbs,SEEK_SET);
    del_esc3(bbs.filename);del_esc3(bbs.id);del_esc3(bbs.name);
    if (bbs.line>250&&is_bbspds&&bit(bcfg.wmode,26)) {
        printf("\n ## �� �Խù��� 250���� �ʰ��Ͽ� ����ó���Ǿ����ϴ�. ## \n");
        bbs.del=Yes;
    }
    fwrite(&bbs,sizeof_bbs,1,fp1);
//    mess("%d %d %s",bbs.posi,bbs.num,bbs.title);
    fclose(fp1);

    NOW_ALLNO=bbs.num;
	NOW_NO=bbs.num;
	NOW_ALLREC++;
	NOW_REC=NOW_ALLREC-1;
        if (mode==1) {
		NOW_ALLNO	=tNOW_ALLNO ;
		NOW_NO		=tNOW_NO	;
		NOW_ALLREC	=tNOW_ALLREC;
		NOW_REC 	=tNOW_REC	;
		bbs=oldbbs;
	}
	return_yes:
	c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
    sync();fflush(NULL);
	return (Yes);

	return_no:
	c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
    sync();fflush(NULL);
	return No;
}


select_head(char *head_buff)     //�Ӹ��� ����
{
	char filename[100];
	int head_num=0;
	int i,n,count;
	char buff[100];
	char atit[100][110];
	FILE *fp;

	for (i=0;i<100;i++) strcpy(atit[i],"");

	sprintf(filename,"%s/logo/%s.hea",club_go.dir,go.code);
	if ((fp=fopen(filename,"r"))==NULL) return head_num;
    strcpy(head_buff,"");
	strcpy(buff,"");
	count=0;
	while (count<100&&fgets(buff,100,fp)!=NULL) {
		del_esc(buff);
		if (buff[0]!=0) strcpy(atit[count++],buff);
	}
	fclose(fp);
    n=bcfg.headnum;
	if (count>0&&n>0) {
        strcpy(head_buff,atit[n-1]);
		return n;
	}
	if (count>0) {
		while (1) {
			sprintf(filename,"%s/logo/%s.hsl",club_go.dir,go.code);
			printf("\n");
			view(filename);
			for (i=0;i<count;i++) {
				if (i%4==0) printf("\n");
				printf("[7m%2d[m.%-14.14s",i+1,atit[i]);
			}
			view("frm/editor/select.hea");
            lineinput(buff,3);check_han(buff);printf("\n");
			head_num=atoi(buff);
			if (is_char("QqXxPp",buff[0])) return -1;
			if (head_num<0||head_num>count) head_num=0;
            if (head_num==0) strcpy(head_buff,"");
            else strcpy(head_buff,atit[head_num-1]);
			if (bit(bcfg.wmode,2)&&head_num==0) continue;
			break;
		}
	}
    if (is_same(head_buff,"[�Ӹ�������]")||
        is_same(head_buff,"[���Ӹ�����]")||
        is_same(head_buff,"[���Ӹ�����]")||
        is_same(head_buff,"[����]")||
        is_same(head_buff,"[����]") ) {
        strcpy(head_buff,"");
        head_num=0;
    }

	return head_num;
}


bbs_addblank()
{
	if (pf.type<16&&now_type<16) return;
	strcpy(bbs.id,pf.id);
	strcpy(bbs.name,pf.name);
	strcpy(bbs.title,"");
	strcpy(bbs.filename,"");
	strcpy(bbs.key,"");
	bbs.filesize=0;
	bbs.read=bbs.down=bbs.gd=bbs.line=bbs.ok=bbs.gd=bbs.del=0;
	unlink(TMP_FILE);
	bbs_add(0);
    re_bbsnum(3);           //BREC 0:�б� 1:�ڵ�  2:����  3:��ü����  8:1���߰� 9:1������
	re_bbs=Yes;
	re_print=Yes;
}



