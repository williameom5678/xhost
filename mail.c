////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: �������� BBS HOST							  //
//			  //			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //	  //
//			  �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����			  //
//			  �������� ������ �޾ƾ� �մϴ�.						  //
////////////////////////////////////////////////////////////////////////
//	  ���ϰ��� ��ƾ
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//������� ������
do_sysmail(int mode)
{
	int i;
	int ret=No;
	char buff[100];
	if (guest_mode) return;
	strcpy(buff,go.dir);
	del_space(buff);del_esc(buff);buff[8]=0;
	if (strlen(buff)>2&&read_pf4(buff)&&pf2.type>0) {
		printf("\n %s(%s)�� ���� ������ �����ڽ��ϱ�? ",pf2.id,pf2.name);
		if (yesno("(Y/n)>>",Yes)) {
			printf("\n ***  [ %s(%s)�� ���Ÿ��� ]  ***\n",pf2.id,pf2.name);
			for(i=0;i<10;i++) strcpy(mmc[i],"");
			strcpy(mmc[1],pf2.id);
			wmail(1,pf2.id);
		}
	}
}


wmail(int wmode,char *tidstr)
{
	FILE *fp1,*fp2;
	char abook[1000][10]; //�ּҷϿ��� �̾Ƴ� ���̵�
	PF apf[50]; 	 //�ּҷϿ����� ���̵� ���� (���� �߼۰��ɾ��̵�)
	PF oldpf;
	int i,i2,n,nn=0;
	char ch;
	int mailnum=0;
	int key,ret,found=0;
	char mailid[1000];
	char mailid2[500];
	char tid[50],filename[500],filename2[500];
	char buff[1500],buff1[1500],stemp[9];	   //������ȣ
	char file_send=No;
	GO oldgo=go;
	GO oldclub_go=club_go;
	int oldclub_mode=club_mode;
	BBS oldbbs=bbs;

	_last2=6;;;;;;;;;;;;;;;;;;;;;;;;
	strcpy(tid,tidstr);
	re_print=Yes;			   //ȭ�����
	re_logo=Yes;			   //����ȭ�����
	re_bbs=Yes; 			   //�Խ��Ƿε�
	list_mode=No;
	key=Yes;
	ret=Yes;
	unlink(TMP_FILE2);			//�ӽ�ȭ�� ����
//	  printf("%d %s %s %s",bbs.num,bbs.id,tid,bbs.title);pressenter();
	if (wmode==2&&strlen(tid)>0) {
		if (bit(cfg.mailmode,10)) {wmode=1;goto _cont1;}
		bbs2tmp();	  //����BBS�� ������ TMP�� ����
		if ((fp1=fopen(TMP_FILE,"r"))==NULL) goto _cont1;
		if ((fp2=fopen(TMP_FILE2,"w"))==NULL) goto _cont1;
		fprintf(fp2,"> ������:%s\n",bbs.id);
		fprintf(fp2,"> �޴���:%s\n",pf.id);
		fprintf(fp2,"> ��  ¥:%s\n",datestr(33,bbs.date));
		fprintf(fp2,"> ��  ��:%s\n",bbs.title);
		while(fgets(buff,1000,fp1)) {
			fprintf(fp2,"> %s",buff);
		}
		fprintf(fp2,"\n");
		fclose(fp1);
		fclose(fp2);
	}
	_cont1:
	unlink(TMP_FILE);		   //�ӽ�ȭ�� ����

	if (!is_same(go.code,"wmail")) {
		go_top(0);
		go_code("wmail");
	}
	strcpy(mailid2,"");
	if (pf.type<18&&view("frm/down/noinmail.log")) goto _fail;

	if (wmode==98) {
		form_mail();
		if (!is_file(TMP_FILE)) goto _fail;
	}

	set_stat(9,"");
	if (strlen(tid)<1) strcpy(tid,mmc[1]);
	strcpy(mailid,"");if (strlen(tid)>0) strcpy(mailid,tid);
	if (is_same(mailid,"wmail")||is_same(mailid,"rmail")) strcpy(mailid,mmc[2]);
	if (is_same(mailid,"wmail")||is_same(mailid,"rmail")) strcpy(mailid,"");
	strchng2(mailid,"��","@");
	strchng2(mailid,"��","#");
	if (mailid[0]=='#'&&!guest_mode) goto _cont3;
	if (stristr(go.wtimes,"e")) view("frm/mail/wemail.log");
	else						view("frm/mail/wmail.log");

	_err1:
	view("frm/mail/mailid1.log");
	if (strlen(mailid)<3) lineinput(mailid,301);
	else				  printf("%s",mailid);
	printf("\n");
	check_han(mailid);strscheck(mailid);all_trim(mailid);
	if (is_same(mailid,"p")||is_same(mailid,"pp")||
		is_same(mailid,"x")||is_same(mailid,"xx")) goto _fail;
	if (strlen(mailid)<2) goto _fail;
	strchng2(mailid,";"," ");
	strchng2(mailid,","," ");
	sscanf(mailid,"%s",buff);
	xstrncpy(g_id,buff,50);
	if (!stristr(buff,"#")) {
		if (!read_pf4(buff)) {
			view("frm/mail/mailid1.err");
			strcpy(mailid,"");
			goto _err1;
		}
		printf("        %s(%s)\n",pf2.id,pf2.name);
	}

	view("frm/mail/mailid2.log");         //����
	lineinput(mailid2,490);printf("\n");check_han(buff);strscheck(buff);
	if (is_quit(mailid2)) goto _fail;
	if (strlen(mailid2)<3) strcpy(mailid2,"");

	_cont3:
	for (i=0;i<1000;i++) bzero(abook[i],10);
	mailnum=0;
	if (mailid[0]=='#'&&!guest_mode) {
		strcpy(buff,&mailid[1]);
		n=atoi(buff); if(n<1) n=1;
		check_dir("userdata/%s/%s",c_han(pf.id),pf.id);  //�����ּҷ�ȭ�� ����
		sprintf(buff,"userdata/%s/%s/book%d.dat",c_han(pf.id),pf.id,n);  //�����ּҷ�ȭ�� ����
		if ((fp1=fopen(buff,"r"))==NULL) {
			mess("%d �� �ּҷ��� �����ϴ�. �ּҷϰ���(MAILBOOK)",n);
			goto _fail;
		}
		//�ּҷϿ��� �����ּҷ��� �о amailid�� �����Ѵ�.
		while (mailnum<1000&&fgets(buff,300,fp1)) {
			check_cmd(buff);
			for (i=0;i<10;i++) {
				if (strlen(mmc[i])>2&&!is_esc(mmc[i])) {
					xstrncpy(abook[mailnum++],mmc[i],8);
					printf(" %3d. %-8.8s",mailnum,mmc[i]);
					if (mailnum%4==0) printf("\n");
				}
			}
		}
		fclose(fp1);
		printf("\n");
		if (mailnum<1) mess("%d �ּҷϿ� �Էµ� ���̵� �����ϴ�.",n);
	}
	else {			 //�ּҷ��� �ƴҶ� 1������ �����Ѵ�.
		strcat(mailid," ");
		strcat(mailid,mailid2);
		strchng2(mailid,";"," ");
		strchng2(mailid,","," ");
		strchng2(mailid,"    "," ");
		strchng2(mailid,"   "," ");
		strchng2(mailid,"  "," ");
		strchng2(mailid,"  "," ");
		i2=0;
		check_cmd(mailid);
		for (i=0;i2<10;i2++) {
			if (strlen(mmc[i])>2)
				strcpy(abook[mailnum++],mmc[i]);
		}
	}
	if (mailnum<1) {
		mess("���õ� ID�� �����ϴ�.");
		goto _fail;
	}
	if (pf.type<cfg.mailwrite) {view("frm/mail/notmail.log"); goto _fail;}

	for (n=0;n<mailnum;n++) {
		strcpy(mailid,abook[n]);		//ù��° ���Ϲ��� ���̵�
		if (strlen(mailid)<3) {strcpy(abook[n],"");continue;}
		if (stristr(mailid,"@")) continue;  //E���ϰ�� Ȯ�ξ���
		if (strlen(mailid)<3||stristr(mailid,"@")) continue;  //E���ϰ�� Ȯ�ξ���
		abook[n][8]=0;
		mailid[8]=0;
		nn=0;
		while (nn++<30) {				//���� 30���� �ٸ����̵�κ��������� ��
			key=0;
			found=read_pf4(mailid); 	//���̵� ã��
            del_space(pf2.tmail);del_esc3(pf2.tmail);
			if (is_same(pf2.id,mailid)) 			 {	   //���̵� ã�´�
                if      (pf2.tmail[0]==0)           { key= 0;found=Yes;break; }   //�������
                else if (is_same(mailid,pf2.tmail)) { key= 0;found=Yes;break; }   //�������
                else if (pf2.tmail[0]=='*')         { key=-1;found=Yes;break; }   //���Űź�
				else								 { key= 1;found=Yes;	   }   //�ٸ����
			}
			if (key==0||key==-1||!found) break;
			if (key==1) {
                printf("\n [%s]���� ������ [%s]���� �޽��ϴ�.\n",mailid,pf2.tmail);
                strcpy(mailid,pf2.tmail);    //�ٸ����̵�� �����
				strcpy(abook[n],mailid);		//1���϶� ���̵��� ������ �����Ѵ�.
			}
		}
		if (found==No||key==-1||pf2.type<1) {
			if (pf2.type<2)   printf("%s���� ������ ID�Դϴ�.\n",mailid);
			else if (key==-1) printf("%s���� ���� ������ �ź��ϰ� �ֽ��ϴ�.\n",mailid);
			else			  printf("%s��� ���̵� �����ϴ�.\n",mailid);
			//������ �����ּҷ��� ��ġ�� �����Ѵ�.
			strcpy(abook[n],"");
		}
	}

	mailnum=0;
	for (n=0;n<100;n++) {
		found=No;
		for (i=n-1;n>0&&i>=0;i--) {
			if (is_same(abook[n],abook[i])) {
				found=Yes;
				break;
			}
		}
		if (found||strlen(abook[n])<3) {
			for (i=n;i<99;i++) {
				strcpy(abook[i],abook[i+1]);
				strcpy(abook[99],"");
			}
		}
		else mailnum++;
	}

	if (read_pf4(abook[0])) view("userdata/%s/%s/pfinfo.mai",c_han(pf2.id),pf2.id);

	view("frm/mail/write.log");

	n=5;
	strcpy(bbs.title,"");
	strcpy(bbs.key,"");
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	while (--n) {
		view("frm/mail/writet.log");
		dosinput(bbs.title,60);printf("\n");
		all_trim(bbs.title);
		if (bbs.title[0]==0) continue;
		if (is_same(bbs.title,"q")||is_same(bbs.title,"p")||
			is_same(bbs.title,"x")||is_same(bbs.title,"xx")) strcpy(bbs.title,"");
		break;
	}
	if (bbs.title[0]==0) goto _fail;   //��ҽ�
	if (wmode==98) goto _cont4;

	unlink(TMP_FILE);
	if (wmode!=2) unlink(TMP_FILE2);

	_input1:
	strcpy(g_buff4,pf.type>=18?"99.UPS":"");

	view("frm/mail/select.log");
	lineinput(buff,2);check_han(buff);printf("\n");
	if (is_same(buff,"q")||is_same(buff,"n")||is_same(buff,"0")) goto _fail;
	n=atoi(buff);
	if (n==2)  {
		if (wmode==2) view(TMP_FILE2);
		ret=bbs_editor(2);
		if (!is_file(TMP_FILE)) ret=No;
		if (wmode==2) {file_add(TMP_FILE2,TMP_FILE);file_move(TMP_FILE2,TMP_FILE);}
	}
	else if (wmode==98) ;
	else if (n==3||n==4||n==99) {	   //�������۽�
		i=0;
		if (pf.type<18&&n==99) n=3;    //UPSUP;
		if (n==99) i=99;
		else {
			view("frm/mail/selectb.log");
			lineinput(buff,2);check_han(buff);printf("\n");
			if (is_same(buff,"q")||is_same(buff,"n")||is_same(buff,"0")) goto _fail;
			i=atoi(buff);
		}

		if (i==2) strcpy(bbs.key,"vtx");  //vtx
		else if (i==3); 				  //�ϼ���TEXT
		else i=1;						  //���̳ʸ� ����
		if (i==1&&pf.type<cfg.mailbintype) {
			view("frm/mail/notbin.log"); //���̳ʸ� �Ұ��ɵ��
			goto _fail;
		}
			 if (n== 4) ret=up_file( 2,filename,No);	 //kermit
		else if (n==99) ret=up_file(99,filename,No);	 //kermit
		else			ret=up_file( 1,filename,No);	 //zmodem
		if (!ret) goto _fail;

		if (i==1&&is_file(filename)) {	   //���������ǰ��
			file_send=Yes;
			file_copy("frm/mail/filesend.dat",TMP_FILE);        //���̳ʸ������� ����
			bbs.filesize=file_size(filename);
			str_filedir(filename,buff1,buff);  //buff�� ���丮�� ���ϸ����� ����
			del_space(buff);
			xstrncpy(bbs.filename,buff,12);
		}
		else if ((i==2||i==3)&&is_file(filename)) {  //���������ǰ��
			file_copy(filename,TMP_FILE);
		}
		else ret=No;
	}
	else {		//��������
		set_stat(9,"");
		if (wmode==2) {
			file_move(TMP_FILE2,TMP_FILE);
			ret=bbs_editor(11); 	//TMP��������
		}
		else ret=bbs_editor(1);
		if (!ret||!is_file(TMP_FILE)) goto _input1;   //��ҽ�
	}

	_cont4:
	if (!ret||!is_file(TMP_FILE)) goto _fail;	//��ҽ�

	if (wmode==98) strcpy(bbs.key,"ī��");
	if (strlen(bbs.key)<1) {
		view("frm/mail/selecty.log");
		lineinput(buff,2);check_han(buff);printf("\n");
		if (is_same(buff,"q")||is_same(buff,"n")||is_same(buff,"0")) goto _fail;
		i=atoi(buff);
			 if (i==2) strcpy(bbs.key,"���");
		else if (i==3) strcpy(bbs.key,"����");
		else if (i==4) strcpy(bbs.key,"����");
		else strcpy(bbs.key,"");
	}

	//�߼�Ȯ��
	printf("\n");
	printf(" ���� : %s\n",bbs.title);
	printf(" ���� : %s(%s)",pf2.id,pf2.name);
	if (mailnum>1) printf("���� ��%d��",mailnum);
	printf("\n");
	printf(" ���� : ");
		 if (is_same(bbs.key,"���")) printf("������");
	else if (is_same(bbs.key,"����")) printf("��������");
	else if (is_same(bbs.key,"����")) printf("����+����");
	else if (is_same(bbs.key,"vtx"))  printf("VTX����");
	else if (is_same(bbs.key,"ī��")) printf("ī������");
	else							  printf("�Ϲ�����");
		printf("\n");
	if (file_send)
	printf(" ȭ�ϸ� : %s  ũ�� : %d ����Ʈ\n",bbs.filename,bbs.filesize);
	view("frm/mail/selecto.log");
	set_stat(9,"");
	lineinput(buff,2);check_han(buff);printf("\n");
	if (is_same(buff,"q")||is_same(buff,"n")||is_same(buff,"0")) goto _fail;
	i=atoi(buff);

	strcpy(bbs.id,pf.id);		//�߽���
	strcpy(bbs.name,pf.nick);	//�߽���
	strcpy(bbs.tcode,"");       //�����з�
	bbs.gd=' ';                 //ȭ��ũ��
	time(&bbs.date);			//�߽� ��¥ ����
	strcpy(bbs.passwd,"");
	bbs.date2=0;				//���� ��¥ ���� (���ſ���)
	bbs.read=0;
	bbs.down=0;
	bbs.del=0;
	bbs.ok=0;

	view("frm/mail/send1.log");
	//������ �߼��Ѵ�.
	for (n=0;n<mailnum;n++) {
		//�Խù����� ����
		//���̵� ã�´�. ->pf2�� ����
		if (strlen(abook[n])<3) continue;
		if (!read_pf4(abook[n])) continue;	//������ ���� ���̵�
		if (stristr(abook[n],"@")) {
			wmail_email(abook[n],filename);
			continue;
		}

		//���������Կ� ���������Կ� ����
		check_dir("userdata/%s/%s",c_han(pf2.id),pf2.id);  //�����ּҷ�ȭ�� ����
		sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf2.id),pf2.id);
		sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf2.id),pf2.id);
		touch_file(NOW_FILE);touch_file(NOW_FILED);

		if (file_send&&read_pf4(abook[n])&&strlen(bbs.filename)>0) {
			check_dir("userroom/%s/%s",c_han(pf.id),pf.id);
			check_dir("userroom/%s/%s",c_han(pf2.id),pf2.id);
			//�޴»�� ����
			for (i=0;i<200;i++) {
				sprintf(filename2,"userroom/%s/%s/%s",c_han(pf2.id),pf2.id,bbs.filename);
				if (!is_file(filename2)) break;
				incfilename(filename2);
				str_filedir(filename2,buff1,buff);	//buff�� ���丮�� ���ϸ����� ����
				del_space(buff);
				printf("\n### %s ���� ���丮�� �̹� %s������ �����մϴ�.\n",pf2.id,bbs.filename);
				printf("\n### ���ϸ��� %s -> %s�� �����մϴ�.",bbs.filename,buff);
				pressenter();
				xstrncpy(bbs.filename,buff,12);
			}
			file_copy(filename,filename2);
			sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
			if (!is_same(pf.id,pf2.id)) {
				unlink(buff);
				sys_tem("ln -sf %s %s",filename2,buff);
			}
			//����ȭ�� ���� ���� Ȯ��
			bbs.filesize=file_size(filename2);
			if (!is_file(filename2)) {
				printf("### ÷�εǴ� ȭ��(%s)�� ������ ���� �ʾҽ��ϴ�.\n",bbs.filename);
				strcpy(bbs.filename,"");
				strcpy(bbs.key,"����");
			}
		}
		//�Խù����� ����
		set_rec(1,9);
		bbs.date2=0;

		bbs_add(1); 	   //������ �����Ѵ�. ���뺹��
		strcpy(g_id,pf2.id);
		strcpy(g_name,pf2.name);
		strcpy(g_buff1,"");
		if (mailnum>0) sprintf(g_buff1,"<%d��>",mailnum);
		view("frm/mail/send2.log");
		sprintf(buff,"\n ** %s �����κ��� ������ �����߽��ϴ�.**\n",pf.id);
		if (bit(cfg.mailmode,5)) strcat(buff,"\007");
		to_mess(pf2.id,buff);
		re_answer(pf2.id,pf.id);		//�ڵ� �������
	}
	//�մ��϶��� �������
	if (pf.type<1||guest_mode) goto _cont2;
	strcpy(bbs.id,pf2.id);		//�߽���
	strcpy(bbs.name,pf2.nick);	//�߽���
	//�������� ����
	if (mailnum>1) {
		sprintf(buff,"<%d��>",mailnum);
		strcat(buff,bbs.title);
		xstrncpy(bbs.title,buff,60);
		strcpy(bbs.name,"<�ټ�>");
	}
	check_dir("userdata/%s/%s",c_han(pf.id),pf.id);  //�����ּҷ�ȭ�� ����
	sprintf(NOW_FILE ,"userdata/%s/%s/send.tit",c_han(pf.id),pf.id);
	sprintf(NOW_FILED,"userdata/%s/%s/send.txt",c_han(pf.id),pf.id);
	touch_file(NOW_FILE);touch_file(NOW_FILED);
	bbs.gd=0;
	bbs.del=0;
	bbs.ok=0;
	bbs.date2=0;
	bbs_add(1);   //������ �����Ѵ�. ���� ����

	_cont2:
	unlink(TMP_FILE);		   //�ӽ�ȭ�� ����
	check_cmd("");
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
	view("frm/mail/mailend.log");
	goto _recover;

	_fail:
	unlink(TMP_FILE);		   //�ӽ�ȭ�� ����
	check_cmd("");
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
	view("frm/mail/failmail.log");

	_recover:
	unlink(TMP_FILE2);			//�ӽ�ȭ�� ����
	go=oldgo;
	club_go=oldclub_go;
	club_mode=oldclub_mode;
	bbs=oldbbs;
	re_print=Yes;
}



//������ ���̵��ȣ�� ������ ������ ����
wmail4(char *id, char *filename,char *title,int mode)
{
	FILE *fp1;
	int found;
	char buff[100];
	PF pf2;
	if (guest_mode) return;
	if		(mode==8) strcpy(bbs.key,"��ü");
	else if (mode==9) strcpy(bbs.key,"��ü����");
	else			  strcpy(bbs.key,"�Ϲ�");
	strcpy(bbs.title,title);
	check_dir("userdata/%s/%s",c_han(id),id);
	sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(id),id);
	sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(id),id);
	strcpy(buff,TMP_FILE);
	strcpy(TMP_FILE,filename);
	bbs.gd=0;
	bbs.del=0;
	bbs.ok=0;
	strcpy(bbs.passwd,"");
	bbs.read=0;
	bbs.date2=0;
	if (mode==9) bbs_add(9);  //������ �߰����� �ʴ´�.
	else		 bbs_add(1);  //�ӽ�ȭ���� �������� �ʴ´�.
	strcpy(TMP_FILE,buff);
}



// ���� ���� ���� ó��
mail_readok()
{
	FILE *fp1,*fp2,*fp3,*fp4;
	int i,found,new_num,new_posi;
	BBS oldbbs;

	_cont:
	view("frm/mail/rmailok.log");
	// ����(1) ó��(1.���� 2.���� 3.���� 9.���Űźε��) ��ȣ >> !����!
	i=numinput();printf("\n");
	if (i==0) {
		if(bit(cfg.mailmode,7)) i=1;
		else if(bit(cfg.mailmode,8)) i=2;
		else if(bit(cfg.mailmode,9)) i=3;
		else i=3;
	}
	if (i==9) {
		mess("���� �������� �ʽ��ϴ�.");
		goto _cont;
	}
	if (i==3) return;
	if (i==1||i==2) {
		if (i==2) {
			bbs2tmp();	  //����BBS�� ������ TMP�� ����
			strcpy(go.code,"mbox");
			set_filename();   //����ȭ�ϸ� ����
			time(&bbs.date2);
			bbs_add(1);   //������ �����Ѵ�. ���뺹��
			strcpy(go.code,"rmail");
			set_filename();
			unlink(TMP_FILE);
		}
		bbs_set_(1,bbs.num,0);		 //����

		fp1=fopen(NOW_FILE,"r");      //����ȭ��
		fp2=fopen(NOW_FILED,"r");      //����ȭ��
		fp3=fopen(TMP_FILE,"w");      //�ӽ�ȭ��
		fp4=fopen(TMP_FILE2,"w");      //�ӽ�ȭ��
		fseek(fp1,0,SEEK_SET);
		new_posi=0;
		found=No;
		new_num=1;
		while (fread(&bbs,sizeof_bbs,1,fp1)>0) {
			oldbbs=bbs;
			if (!bbs.del) { 	  //������ �Խù��� �ƴҶ�
				if (bbs.size<0) bbs.size=0;
				if (bbs.size>100000) bbs.size=100000;
				if (fp2==NULL) bbs.size=0;
				else {
					fseek(fp2,bbs.posi,SEEK_SET);
					fseek(fp4,new_posi,SEEK_SET);
					for (i=0;i<bbs.size&&!feof(fp2);i++) {
						fputc(fgetc(fp2),fp4);
					}
				}
				bbs.posi=new_posi;
				fseek(fp3,(new_num-1)*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp3);
				new_posi+=bbs.size;
				new_num++;
			}
		}
		if (fp1!=NULL) fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
		if (fp3!=NULL) fclose(fp3);
		if (fp4!=NULL) fclose(fp4);

		if (found) {
			file_move(TMP_FILE ,NOW_FILE );
			file_move(TMP_FILE2,NOW_FILED);
		}
		else {
			unlink(TMP_FILE);
			unlink(TMP_FILE2);
		}
		re_bbs=Yes;
	}
}



mail_set_read()
{
	BBS bbs2;
	FILE *fp1;
	int i=0;
	int num=bbs.num;
	char filename[120];


	//���� ���Ͽ� ���� ǥ�� ����
	if (bbs.read==0||(word)bbs.date2<1000) {  //ó�� ������
		time(&bbs.date2);  //���� ��¥ ����
		if (read_pf4(bbs.id)) { 		//���ϼ����� ã�� ->pf2�� ����
			check_dir("userdata/%s/%s",c_han(pf2.id),pf2.id);  //�����ּҷ�ȭ�� ����
			sprintf(filename,"userdata/%s/%s/send.tit",c_han(pf2.id),pf2.id);
			if ((fp1=fopen(filename,"r+"))==NULL) return;
			i=0;
			while (fread(&bbs2,sizeof_bbs,1,fp1)) {
				if (bbs.date==bbs2.date&&is_same(pf.id,bbs2.id)) {
					fseek(fp1,i*sizeof_bbs,SEEK_SET);
					time(&bbs2.date2);		//���� ��¥ ����
					fwrite(&bbs2,sizeof_bbs,1,fp1);
					break;
				}
				i++;
			}
			fclose(fp1);
		}
	}
}


// 123.  -> 124.
// 123.4  -> 124.4
// 123.45  -> 124.45
// 123.456	-> 124.456

//���ϸ��� ���ڸ��� �����Ѵ�.
incfilename2(char *filename,int pos)
{
	int i;
	if (pos<0||pos>=(int)strlen(filename)) return;
	i=(filename[pos]+1);
	if (i>(int)'z'||i<(int)'0') i=(int)'0';
	filename[pos]=(char)(i);
}

incfilename(char *filename)
{
	int len=strlen(filename);
	if (len<1) return;
	if (filename[len-1]=='.') incfilename2(filename,len-2);
	else if (filename[len-2]=='.') incfilename2(filename,len-3);
	else if (filename[len-3]=='.') incfilename2(filename,len-4);
	else if (filename[len-4]=='.') incfilename2(filename,len-5);
	else incfilename2(filename,len-1);
}



//���� ��ü ���� ������
wmall()
{
	FILE *fp;
	int i,key,key2,n=0,nn,n1,n2;
	char buff[300];
	char buff2[140];
	char id[9];
	char torank1,torank2;
	char tojob1,tojob2;
	int tolevel1,tolevel2,tosex,num,num_all,num_send;
	char tosexstr[10],dir[300];

    if (guest_mode) return;
	if (club_mode) {wmall_club();return;}
	key=Yes;
    if (in_file2("system/wmall.id",pf.id)) ;
    else if (pf.type<18&&pf.type<cfg.wmalltype) {
        mess("��ü ������ ������ ���� ����Դϴ�.");
        go_top(0);
        return;
    }

	torank1=0;torank2=14;tojob1=0;tojob2=19;tolevel1=0;tolevel2=999999;tosex=0;
	num_all=count_pf();

	strcpy(bbs.id,pf.id);
	strcpy(bbs.name,pf.name);
	strcpy(bbs.filename,"");
	strcpy(bbs.key,"��ü����");
	bbs.size=0;bbs.filesize=0;
	bcfg.wmode=0;
	strcpy(go.code,"wmall");strcpy(bbs.title,"");
	unlink(TMP_FILE);
	strcpy(NOW_FILE ,"menu/_allmail.tit");strcpy(NOW_FILED,"menu/_allmail.txt");
	touch_file(NOW_FILE);touch_file(NOW_FILED);
	while (1) {
		if (torank1<1)	torank1=0;if (torank1>19) torank1=19;
		if (torank2<1)	torank2=0;if (torank2>19) torank2=9;
		if (tojob1<1)  tojob1=0;if (tojob1>19)	tojob1=19;
		if (tojob2<1)  tojob2=0;if (tojob2>19)	tojob2=19;
		if (tolevel1<1) tolevel1=0;if (tolevel1>999999) tolevel1=999999;
		if (tolevel2<1) tolevel2=0;if (tolevel2>999999) tolevel2=999999;
		if		(tosex==1) strcpy(tosexstr,"����ȸ��");
		else if (tosex==2) strcpy(tosexstr,"����ȸ��");
		else			   strcpy(tosexstr,"���о���");
		bbs.size=file_size(TMP_FILE);

		cls();
		printf(" *** %s ȸ�� ��ü ���� ������ (��:%d��)***\n",cfg.hostname,num_all);
		printfL2();
		printf("  1. ȸ �� �� �� : %d - %d \n",torank1,torank2);
		printf("  2. ȸ �� �� �� : %s      \n",tosexstr);
		printf("  3. ȸ �� �� �� : %d - %d \n",tolevel1,tolevel2);
		printf("  4. �� �� �� �� : %d - %d \n",tojob1,tojob2);
		printf(" 10. �� �� �� �� : %s\n",bbs.title);
		printf(" 11. �� �� �� �� : %d Byte\n",bbs.size);
		if (pf.type>17) {
		printf(" 12. �� �� �� ID : %s\n",bbs.id);
		printf(" 13. ������ �̸� : %s\n",bbs.name);
		}
		printf("\n");
		printf(" 99. ���� ���� ȸ������ ���� ������\n");
		printfL2();
		printf("  ����  ����(Q) >>");
		n=numinput();
		printf("\n");
		if (n<=0) break;
		if (n==1) {
			printf(" ���ȸ�� ��� ����  >>");torank1=get_num(torank1,2);printf("\n");
			printf(" ���ȸ�� ��� ����  >>");torank2=get_num(torank2,2);printf("\n");
		}
		if (n==2) {
			printf(" ���ȸ�� ����  ���о���(0) ����(1) ����(2) >>");tosex=get_num(tosex,1);printf("\n");
		}
		if (n==3) {
			printf(" ���ȸ�� ���� ����  >>");tolevel1=get_num(tolevel1,6);printf("\n");
			printf(" ���ȸ�� ���� ����  >>");tolevel2=get_num(tolevel2,6);printf("\n");
		}
		if (n==4) {
			view("frm/guest/job.scr");
			printf("\n ���ȸ�� ������ȣ ����  >>");tojob1=get_num(tojob1,2);printf("\n");
			printf(" ���ȸ�� ������ȣ ����  >>");tojob2=get_num(tojob2,2);printf("\n");
		}
		if (n==10) {
			view("frm/editor/writetit.wma");
			get_str(bbs.title,60);
		}
		if (n==11) {  //����
			input_tmp(0);
		}
		if (n==12&&(pf.type>17)) {
			printf(" ���� �߼� ID   >>");
			get_str(bbs.id,8);
		}
		if (n==13&&(pf.type>17)) {
			printf(" ���� �߼� �̸� >>");
			get_str(bbs.name,8);
		}
		if (n==99) {
			if (strlen(bbs.title)<1) {mess("�߼� ������ ������ �����ϼ���..");continue;}
			if (bbs.size<1) 		 {mess("�߼� ������ ������ �����ϼ���..");continue;}
			//����ο� ���
			num_send=0;
			if ((fp=fopen(ID_FILE,"r"))==NULL) return;
			printf("\n");
			printfL2();
			key=key2=Yes;
			while (fread(&pf2,sizeof_pf,1,fp)) {
				if (pf2.type<torank1||pf2.type>torank2) continue;
				if (pf2.level<tolevel1||pf2.level>tolevel2) continue;
				if (pf2.job<tojob1||pf2.job>tojob2) continue;
				if ((tosex==1&&pf2.sex!='m')||(tosex==2&&pf2.sex!='f')) continue;
				num_send++;
			}
			fclose(fp);
			printf("\n");
			printf(" ### ��ü ���� ��� ȸ���� %d �� ###\n",num_send);
			printf(" ### �߼����� : %s\n",bbs.title);
			if (num_send==0) {mess("�ش� ������ ȸ���� 0 ���Դϴ�.");continue;}
			if (!yesno2("     ��ü ������ �����ڽ��ϱ�?  (Y/n) >>",Yes)) continue;

			num=1;
			key=Yes;
			key2=Yes;
			fp=fopen(ID_FILE,"r");
			time(&bbs.date);  //�߽� ��¥ ����
			bbs.date2=0;
			strcpy(bbs.id,pf.id);
			strcpy(bbs.name,pf.name);
			bbs.gd=' ';
			bbs.del=No;
			strcpy(bbs.passwd,"");
			strcpy(bbs.key,"��ü����");
			bbs=bbs;
			printf(" ### ��ü���Ϲ߼۽���....... ###\n");
			while (key&&fread(&pf2,sizeof_pf,1,fp)) {
				del_space(pf2.id);del_esc(pf2.id);
				del_esc3(pf2.id);del_esc3(pf2.name);del_esc3(pf2.nick);
				if (strlen(pf2.id)<3) continue;
				if (pf2.type<torank1||pf2.type>torank2) continue;
				if (pf2.job<tojob1||pf2.job>tojob2) continue;
				if (pf2.level<tolevel1||pf2.level>tolevel2) continue;
				if ((tosex==1&&pf2.sex!='m')||(tosex==2&&pf2.sex!='f')) continue;
				nn=0;
				while (nn++<30) {				//���� 30���� �ٸ����̵�κ��������� ��
                    if (pf2.tmail[0]<3||is_same(pf2.id,pf2.tmail)) break;
                    if (strlen(pf2.tmail)>2&&!read_pf2(pf2.tmail)) {
						strcpy(pf2.id,"");
						break;
					}
				}
				if (strlen(pf2.id)<1) continue;
                if (strlen(pf2.tmail)>0&&strlen(pf2.tmail)<3) continue;
				wmail4(pf2.id,TMP_FILE,bbs.title,9);
				printf(" %4d. %-8s(%-8s) ",num++,pf2.id,pf2.name);
				if (num%3==0) printf("\n");
				if (key2&&num%30==0) {
					printf(" ### ��ü ������ ��� �������? ���(Y/n) ����(S)>>");
					lineinput(buff,2);check_han(buff);printf("\n");
					if (is_same(buff,"n")) key=No;
					else if (is_same(buff,"s")) key2=No; //����
				}
			}
			fclose(fp);
			wmail4(pf.id,TMP_FILE,bbs.title,8);
			strcpy(NOW_FILE ,"menu/_allmail.tit");strcpy(NOW_FILED,"menu/_allmail.txt");
			touch_file(NOW_FILE);touch_file(NOW_FILED);
			time(&bbs.date2);
			bbs_add(1);
			printf("\n");
			mess("���� �߼��� ��� �������ϴ�.");
			unlink(TMP_FILE);
		}
	}
	unlink(TMP_FILE);
    go_top(0);
}



//��ȣȸ ��ü ���� ������
wmall_club()
{
	FILE *fp;
	int i,key,key2,n=0,nn,n1,n2;
	char buff[300];
	char buff2[140];
	char id[9];
	char torank1,torank2;
	char tojob1,tojob2;
	int tolevel1,tolevel2,tosex,num,num_all,num_send;
	char tosexstr[10],dir[300],id_file[300];
	CPF cpf2;

	key=Yes;
	if (pf.type<17&&cpf.type<ccfg.wmalltype) key=No;
	if (!key&&in_file2("system/wmall.id",pf.id)) key=Yes;
	if (!key) {mess("��ü ������ ������ ���� ��ȣȸ ����Դϴ�.");return;}

	torank1=0;torank2=14;tojob1=0;tojob2=19;tolevel1=0;tolevel2=999999;tosex=0;
	sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
	num_all=count_cpf(id_file);

	strcpy(bbs.id,pf.id);
	strcpy(bbs.name,pf.name);
	strcpy(bbs.filename,"");
	strcpy(bbs.key,"��ü����");
	bbs.size=0;bbs.filesize=0;
	bcfg.wmode=0;
	strcpy(go.code,"wmall");strcpy(bbs.title,"");
	unlink(TMP_FILE);
	strcpy(NOW_FILE ,"menu/_allmail.tit");strcpy(NOW_FILED,"menu/_allmail.txt");
	touch_file(NOW_FILE);touch_file(NOW_FILED);
	while (1) {
		if (torank1<1)	torank1=0;if (torank1>19) torank1=19;
		if (torank2<1)	torank2=0;if (torank2>19) torank2=9;
		if (tojob1<1)  tojob1=0;if (tojob1>19)	tojob1=19;
		if (tojob2<1)  tojob2=0;if (tojob2>19)	tojob2=19;
		if (tolevel1<1) tolevel1=0;if (tolevel1>999999) tolevel1=999999;
		if (tolevel2<1) tolevel2=0;if (tolevel2>999999) tolevel2=999999;
		if		(tosex==1) strcpy(tosexstr,"����ȸ��");
		else if (tosex==2) strcpy(tosexstr,"����ȸ��");
		else			   strcpy(tosexstr,"���о���");
		bbs.size=file_size(TMP_FILE);

		cls();
		printf(" *** ��ȣȸ ȸ�� ��ü ���� ������ (��:%d��)***\n",num_all);
		printfL2();
		printf("  1. ȸ �� �� �� : %d - %d \n",torank1,torank2);
		printf("  2. ȸ �� �� �� : %s      \n",tosexstr);
		printf("  3. ȸ �� �� �� : %d - %d \n",tolevel1,tolevel2);
		printf("  4. �� �� �� �� : %d - %d \n",tojob1,tojob2);
		printf(" 10. �� �� �� �� : %s\n",bbs.title);
		printf(" 11. �� �� �� �� : %d Byte\n",bbs.size);
		if (pf.type>17) {
		printf(" 12. �� �� �� ID : %s\n",bbs.id);
		printf(" 13. ������ �̸� : %s\n",bbs.name);
		}
		printf("\n");
		printf(" 99. ���� ���� ȸ������ ���� ������\n");
		printfL2();
		printf("  ����  ����(Q) >>");
		n=numinput();
		printf("\n");
		if (n<=0) break;
		if (n==1) {
			printf(" ���ȸ�� ��� ����  >>");torank1=get_num(torank1,2);printf("\n");
			printf(" ���ȸ�� ��� ����  >>");torank2=get_num(torank2,2);printf("\n");
		}
		if (n==2) {
			printf(" ���ȸ�� ����  ���о���(0) ����(1) ����(2) >>");tosex=get_num(tosex,1);printf("\n");
		}
		if (n==3) {
			printf(" ���ȸ�� ���� ����  >>");tolevel1=get_num(tolevel1,6);printf("\n");
			printf(" ���ȸ�� ���� ����  >>");tolevel2=get_num(tolevel2,6);printf("\n");
		}
		if (n==4) {
			view("frm/guest/job.scr");
			printf("\n ���ȸ�� ������ȣ ����  >>");tojob1=get_num(tojob1,2);printf("\n");
			printf(" ���ȸ�� ������ȣ ����  >>");tojob2=get_num(tojob2,2);printf("\n");
		}
		if (n==10) {
			view("frm/editor/writetit.wma");
			get_str(bbs.title,60);
		}
		if (n==11) {  //����
			input_tmp(0);
		}
		if (n==12&&(pf.type>17)) {
			printf(" ���� �߼� ID   >>");
			get_str(bbs.id,8);
		}
		if (n==13&&(pf.type>17)) {
			printf(" ���� �߼� �̸� >>");
			get_str(bbs.name,8);
		}
		if (n==99) {
			if (strlen(bbs.title)<1) {mess("�߼� ������ ������ �����ϼ���..");continue;}
			if (bbs.size<1) 		 {mess("�߼� ������ ������ �����ϼ���..");continue;}
			//����ο� ���
			num_send=0;
			if ((fp=fopen(id_file,"r"))==NULL) return;
			printf("\n");
			printfL2();
			key=key2=Yes;
			while (fread(&cpf2,sizeof_cpf,1,fp)) {
				if (cpf2.type<torank1||cpf2.type>torank2) continue;
				if (!read_pf4(cpf2.id)) continue;
				if (pf2.level<tolevel1||pf2.level>tolevel2) continue;
				if (pf2.job<tojob1||pf2.job>tojob2) continue;
				if ((tosex==1&&pf2.sex!='m')||(tosex==2&&pf2.sex!='f')) continue;
				num_send++;
			}
			printf("\n");
			printf(" ### ��ü ���� ��� ȸ���� %d �� ###\n",num_send);
			printf(" ### �߼����� : %s\n",bbs.title);
			if (num_send==0) {mess("�ش� ������ ȸ���� 0 ���Դϴ�.");continue;}
			if (!yesno2("     ��ü ������ �����ڽ��ϱ�?  (Y/n) >>",Yes)) continue;

			num=1;
			key=Yes;
			key2=Yes;
			fp=fopen(id_file,"r");
			time(&bbs.date);  //�߽� ��¥ ����
			bbs.date2=0;
			strcpy(bbs.id,pf.id);
			strcpy(bbs.name,pf.name);
			bbs.gd=' ';
			bbs.del=No;
			strcpy(bbs.passwd,"");
			strcpy(bbs.key,"��ü����");
			bbs=bbs;
			printf(" ### ��ü���Ϲ߼۽���....... ###\n");
			while (key&&fread(&cpf2,sizeof_cpf,1,fp)) {
				if (cpf2.type<torank1||cpf2.type>torank2) continue;
				if (!read_pf2(cpf2.id)) continue;
				del_esc3(pf2.id);del_esc3(pf2.name);del_esc3(pf2.nick);
				if (strlen(pf2.id)<3) continue;
				if (pf2.level<tolevel1||pf2.level>tolevel2) continue;
				if (pf2.job<tojob1||pf2.job>tojob2) continue;
				if ((tosex==1&&pf2.sex!='m')||(tosex==2&&pf2.sex!='f')) continue;
                if (strlen(pf2.tmail)>0&&strlen(pf2.tmail)<3) continue;
				nn=0;
				while (nn++<30) {				//���� 30���� �ٸ����̵�κ��������� ��
                    if (pf2.tmail[0]<3||is_same(pf2.id,pf2.tmail)) break;
                    if (strlen(pf2.tmail)>2&&!read_pf2(pf2.tmail)) {
						strcpy(pf2.id,"");
						break;
					}
				}
				if (strlen(pf2.id)<1) continue;
                if (strlen(pf2.tmail)>0&&strlen(pf2.tmail)<3) continue;
				wmail4(pf2.id,TMP_FILE,bbs.title,9);
				printf(" %4d. %-8s(%-8s) ",num++,pf2.id,pf2.name);
				if (num%3==0) printf("\n");
				if (key2&&num%30==0) {
					printf(" ### ��ȣȸȸ������ ��� �������? ���(Y/n) ����(S)>>");
					lineinput(buff,2);check_han(buff);printf("\n");
					if (is_same(buff,"n")) key=No;
					else if (is_same(buff,"s")) key2=No; //����
				}
			}
			fclose(fp);
			wmail4(pf.id,TMP_FILE,bbs.title,8);
			strcpy(NOW_FILE ,"menu/_allmail.tit");strcpy(NOW_FILED,"menu/_allmail.txt");
			touch_file(NOW_FILE);touch_file(NOW_FILED);
			time(&bbs.date2);
			bbs_add(1);
			printf("\n");
			mess("���� �߼��� ��� �������ϴ�.");
			unlink(TMP_FILE);
		}
	}
	unlink(TMP_FILE);
}



form_mail()
{
	FILE *fp1,*fp2;
	char buff[780],buff1[780],buff2[780];
	char filename[200];
	int count,i,n,total,found;
	for (i=0;i<20;i++) strcpy(a_str[i],"");
	while (1) {
		_cont1:
		unlink(TMP_FILE);
		strcpy(filename,"");
		view("frm/fmail/fmail.scr");     //����ȭ�� ���
		lineinput(buff,30);check_cmd(buff);printf("\n");
		if (is_quit(buff)) break;
		if (strlen(buff)<1) continue;
		sprintf(filename,"frm/fmail/%s.txt",buff);
		if (pf.type>17&&is_same(mmc[0],"ed")&&strlen(mmc[1])>0) {
			if (is_same(mmc[1],"fmail.scr")) sprintf(filename,"frm/fmail/%s",mmc[1]);
			else sprintf(filename,"frm/fmail/%s.txt",mmc[1]);
			xedit2(filename);
			printf("\n");
			system("ls -l frm/fmail/");
			pressenter();
			continue;
		}
		else if (is_same(buff,"e")) {
			get_vmail_str();
			continue;
		}
		if (!view(filename)) continue;
		view(filename);printf(VTXOFF);
		get_vmail_str();
		_cont2:
		if((fp1=fopen(filename,"r"))==NULL) return;
		if((fp2=fopen(TMP_FILE,"w"))==NULL) return;
		while (fgets(buff,300,fp1)) {
			del_enter(buff);
			for (i=1;i<15;i++) {
				sprintf(buff1,"!����!%02d",i);
				strchng2(buff,buff1,a_str[i]);
			}
			fprintf(fp2,"%s\n",buff);
		}
		fclose(fp1);
		fclose(fp2);
		view(TMP_FILE);printf(VTXOFF);
		printf("\n ##  �� �������� �߼��ұ��? �߼�(Y) �缱��(N) ���(Q) �������(E) >>");
		lineinput2(buff,3);check_han(buff);printf("\n");
		if (is_same(buff,"e")) {get_vmail_str();goto _cont2;}
		if (is_same(buff,"n")) continue;
		if (is_quit(buff)) {unlink(TMP_FILE);return;}
		if (is_same(buff,"y")) return;
		goto _cont2;
	}
	unlink(TMP_FILE); //��ҽ�..
}

get_vmail_str()
{
	int i,i2;
	char buff[100];
	i=1;
	printf(" ������ �ۼ��Ͻʽÿ�.\n");
	printf(" �ٴ� �ѱ�16�ڷ� 14�ٱ���,������:ùĭ���� . �Է�\n");
	while(i<15) {
		printf("%3d:",i);
		lineinput(buff,34);printf("\n");
		if (is_same(buff,".")||is_quit(buff)) {
			for (i2=i;i2<20;i2++) strcpy(a_str[i2],"");
			break;
		}
		strcpy(a_str[i++],buff);
	}
}

			//����		 �߽�
re_answer(char *id1,char *id2)		  //�ڵ� �������
{
	FILE *fp;
	char filename[100],title[100];
	char buff[300],buff1[300],buff2[300],buff3[300],buff4[300];
	sprintf(filename,"system/manswer.id");
	if ((fp=fopen(filename,"r"))==NULL) return;
	while (fgets(buff,390,fp)!=NULL) {
		sscanf(buff,"%s%s",buff1,buff2);
		if (is_same(buff1,id1)) {
			sprintf(title,"[����]%s",bbs.title);
			title[60]=0;
			sprintf(buff3,"frm/letter/%s.txt",buff2);
			strchng2(buff3,"../","./");
			strchng2(buff3,"//","/");
			wmail4(id2,buff3,title,1);
			break;
		}
	}
	fclose(fp);
}
