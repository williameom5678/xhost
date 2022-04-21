////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
//	  �Խ��� ���� ��ƾ
//	 ������ �ٷ� PACK ��� ����...
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//�ش� �Ǵ� ��ȣ�� ã�� bbs�� ����
int bbs_num(int num)
{
	FILE *fp;
	BBS bbs1;
	int i,n,found=No;
	int last,first,now;
	view_pos=0; 	   //����Խ��ǳ��� ���� ��ġ
	view_num=0; 	   //�׸����������ȣ
	if (bbs_ltmode) fp=fopen(LT_FILE,"r+");
	else			fp=fopen(NOW_FILE,"r+");
	if (fp==NULL) return No;
	//���� ������ġ�ΰ�?
	fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
	if (fread(&bbs1,sizeof_bbs,1,fp)&&bbs1.num==num) {
		found=Yes;
		fclose(fp);
		goto _ok;
	}
	//�ε��� �˻�
	first=1;
	fseekend(fp);
	last=ftell(fp)/sizeof_bbs;
	now=last/2+1;
    while(1) {
		fseek(fp,(now-1)*sizeof_bbs,SEEK_SET);
		if (fread(&bbs1,sizeof_bbs,1,fp)) {
			if (bbs1.num==num) {
				found=Yes;
				NOW_REC=now-1;
				break;
			}
			else if ((last-first<5)||now<0) break;
			else if (bbs1.num>num) last=now;
			else				   first=now;
			now=(last-first)/2+first+1;
		}
		else break;
	}
	if (!found) {
		i=now-15;if (i<0) i=0;
		for (n=i;n<now+30;n++) {
			fseek(fp,n*sizeof_bbs,SEEK_SET);
			if (fread(&bbs1,sizeof_bbs,1,fp)) {
				if (bbs1.num==num) {
					NOW_REC=n;
					found=Yes;
					break;
				}
			}
		}
	}

	if (!found) {  //�˻� ���н� ó������ �˻�
		fseekend(fp);
		last=ftell(fp)/sizeof_bbs;
		rewind(fp);
		for (i=0;i<last;i++) {
			fread(&bbs1,sizeof_bbs,1,fp);
			if (bbs1.num==num) {
				found=Yes;
				NOW_REC=i;
				break;
			}
		}
	}
	fclose(fp);

	_ok:
	g_char=' ';
	jpg_tag=No;
    if (found&&bbs1.del) {
        if (pf.type<18&&bit(bcfg.mode,21)) found=No;
        else if (bbs_man(pf.id)) found=Yes;
        else if (!dd_mode) found=No;
        else if (club_mode&&cpf.type>16) found=Yes;
        else if (!club_mode&&pf.type>15) found=Yes;
        else found=No;
    }
	if (found) {
		re_print=Yes;
		bbs=bbs1;
		del_tab(bbs.title);all_trim(bbs.title);
		del_esc(bbs.key);all_trim(bbs.key);
		del_esc(bbs.id);del_esc(bbs.name);del_space(bbs.id);
		del_space(bbs.filename);del_tab(bbs.filename);del_esc(bbs.filename);bbs.filename[12]=0;
		NOW_NO=bbs.num;
		N_page=1;
		g_char=gd_chr(is_typebbs);	 //��õ/��ȸ/�ٿ�����
		jpg_tag=is_jpgfile();
		set_buffnum(buffnum,bbs.num,jpg_tag);	   //buffnum�����
	}
	return(found);
}



//�Խ��� ����
//�Է� : ������ �Խù� ��ȣ
//��� : NOW_FILE�� �����Ѵ�.

bbs_edit(int num,int mode)
{
	char buff[280];
	int tNOW_NO,tNOW_REC,tNOW_ALLREC,tNOW_TOPREC;
	tNOW_NO=NOW_NO;tNOW_REC=NOW_REC;tNOW_TOPREC=NOW_TOPREC;tNOW_ALLREC=NOW_ALLREC;
	if (guest_mode) {
		view("frm/error/noguest.err");
		return;
	}
	if (list_mode&&num==0) num=bbs.num;
	//�ӽ÷� �û��� �ø����̸� ���������ϵ��� �Ǿ� ����..

    list_mode=No;
	bbs_ltmode=No;		  //lt������ �˻��� ���
	set_filename(); 	  //����ǥ���� ȭ�ϸ�
	check_bbs();		  //�ڷ��� ������ �ľ��ϴ� ��ƾ
	if(!bbs_num(num)) {
		mess(" %d�� �ڷḦ ã���� �����ϴ�.",num);
		return;
	}
	if (check_del()) {	   //���������� �����ΰ�?
		sys_tem("bin/ed_bbs %d '%s' '%s' '%d' '%s' '%s'",mode,NOW_FILE,pf.id,NOW_REC,club_go.code,go.code);
	}
	else mess("���������� �����ϴ�.");
	NOW_NO=tNOW_NO;NOW_REC=tNOW_REC;NOW_TOPREC=tNOW_TOPREC;NOW_ALLREC=tNOW_ALLREC;
    bbs_num(num);
	re_print=Yes;
}

bbs_jump(int num)
{
	char buff[280];
	if (list_mode&&num==0) num=bbs.num;
	if(bbs_num(num)&&strlen(bbs.key)>0)
		strcpy(auto_cmd,bbs.key);
}

view_last_num(int mode)
{
	FILE *fp1;
	BBS bbs2;
	int now_rec;
	int now_allrec;
	int found=No;
	//ȭ���� ������ ���� ����
    set_filename();                         //����ǥ���� ȭ�ϸ�
    read_bcfg();
	if (bbs_ltmode) fp1=fopen(LT_FILE,"r+");
	else			fp1=fopen(NOW_FILE,"r+");
	if( fp1!=NULL ) {
		fseekend(fp1);
		now_allrec=ftell(fp1)/sizeof_bbs;
		while (now_allrec>0) {
			fseek(fp1,(now_allrec-1)*sizeof_bbs,SEEK_SET);
			if (fread(&bbs2,sizeof_bbs,1,fp1)) {
				if (!bbs2.del) {
					bbs=bbs2;
					NOW_REC=now_allrec;
					if (NOW_REC>NOW_ALLREC) {
						check_bbs();
						NOW_REC=now_allrec;
					}
					found=Yes;
					break;
				}
			}
			now_allrec--;
		}
		fclose(fp1);
	}
    if (mode) {
        if (found) {
            bbs_read_num();   //���� ó��
            add_read_numn(bbs.num);       //���� ǥ�� �߰�
        }
        return;
    }
	if (found) {
		bbs_read_num();   //���� ó��
        cls();
		bbs_read(0);
		add_read_numn(bbs.num); 	  //���� ǥ�� �߰�
	}
}


bbs_read_num()
{
	int i;
	BBS bbs2;
	FILE *fp1;
	int found;
	int tNOW_NO,tNOW_REC,tNOW_ALLREC,tNOW_TOPREC;
	tNOW_NO=NOW_NO;tNOW_REC=NOW_REC;tNOW_TOPREC=NOW_TOPREC;tNOW_ALLREC=NOW_ALLREC;
	if (is_same(go.code,"rmail")) {  //���Ͽ����� Ư�� ó��
		mail_set_read();			 //���� ���� ���� ó��
	}
	if (is_same(go.code,"cmail")) return;
    if (bit(bcfg.recmode,3)) return;
	//�Խ����� ����Ƚ�� ����  (������ �ƴҶ�),�������� �����Խù��� �ƴҶ�
    if ( (is_same(go.code,"rmail"))||bit(bcfg.recmode,5)||
		 (!is_same(bbs.id,pf.id)&&!is_read_numn(bbs.num))) {
		if ((fp1=fopen(NOW_FILE,"r+"))==NULL) goto _quit;
		c_lock(1,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
		if ((fp1=fopen(NOW_FILE,"r+"))!=NULL) {
			fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
			if (fread(&bbs2,sizeof_bbs,1,fp1)&&bbs2.num==bbs.num) {
				bbs2.read++;
				bbs.read++;
                if (bit(bcfg.recmode,3)) bbs.down++;
				if (is_same(go.code,"rmail")&&is_same(bbs.key,"���")) {
					bbs.gd=' ';
					bbs2.gd=' ';
				}
				time(&bbs2.date2);	//���� ��¥ ����
				time(&bbs.date2);  //���� ��¥ ����
				fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
				fwrite(&bbs2,sizeof_bbs,1,fp1);
			}
			fclose(fp1);
		}
		c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
	}
    sync();
	_quit:
	NOW_NO=tNOW_NO;NOW_REC=tNOW_REC;NOW_TOPREC=tNOW_TOPREC;NOW_ALLREC=tNOW_ALLREC;
}



//������ ���� ����
bbs_set_del()
{
	int i;
	BBS bbs2;
	FILE *fp1;
	c_lock(1,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
	if ((fp1=fopen(NOW_FILE,"r+"))!=NULL) {
		i=0;
		while (fread(&bbs2,sizeof_bbs,1,fp1)) {
			if (bbs2.num==bbs.num) {
				bbs2.del=1;
				fseek(fp1,i*sizeof_bbs,SEEK_SET);
				if (i<1000000) {
					if (!feof(fp1)) fwrite(&bbs2,sizeof_bbs,1,fp1);
				}
				break;
			}
			i++;
		}
		fclose(fp1);
	}
    sync();
	c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
}


//�Խ��� �亯
bbs_re()
{
	int n,ret,mode;
	char buff[80];
	char buff2[80];
	char buff3[30];
    char buff4[30];
	unsigned int i;
	 re_print=Yes;
	if (!is_char(BBSTYPE,go.type))	return;
	if (!(is_same(mmc[0],"re")||
          is_same(mmc[0],"r")||
          is_same(mmc[0],"ra")||
		  is_same(mmc[0],"rs")||
		  is_same(mmc[0],"rn")||
		  is_same(mmc[0],"rt")||
		  is_same(mmc[0],"at")||
		  is_same(mmc[0],"an")||
		  is_same(mmc[0],"rr")))    return;
	if (is_typepds&&
         (is_same(mmc[0],"ra")||
		  is_same(mmc[0],"rn")||
          is_same(mmc[0],"rt")||
          is_same(mmc[0],"an")||
          is_same(mmc[0],"rr")||
		  is_same(mmc[0],"rr")))    return;
	if (go.type=='k') return;  //����� ���
	ret=Yes;
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	if (mmn[1]==0) ret=No;	//���ù�ȣ ������
	list_mode=No;

	if (bit(limit,1)) {
		view2("frm/_nodeall.log");
		return;
	}

	if (ret==No) {
		printf("\n *****�亯������ �����****** \n");
		printf("ra [��ȣ]  �ش�Խù��� �߰��� ���� [�߰�]....\n");
		printf("rn [��ȣ]  �ش�Խù��� �亯�� ���� [�亯]....\n");
		printf("rt [��ȣ]  �ش�Խù��� ������ ���� [����]....\n");
		printf("rr [��ȣ]  �ش�Խù��� ����� ���� [���]....\n");
		printf("re [��ȣ]  �ش�Խù� �ۼ��ڿ��� ��������� ����\n");
		printf("rs         �ش�Խù� ����ڿ��� ������ ����\n");
		printf("an [��ȣ]  �ش�Խù� �亯���� �Խù��� �亯����\n");
		printf("at [��ȣ]  �ش�Խù� �߰�(���ζǴ�,�����) ����\n");
		pressenter();
		return;
	}

	if (guest_mode&&!is_char("gun",go.type)) {  /* �մ��� ��쿡�� */
		message("�մ��� ����Ҽ� ���� ����Դϴ�.");
		return;
	}

    if (bbs_ltmode) {        //����������
		bbs_ltmode=No;
		set_filename();
		check_bbs();
	}
	if (check_type('w')) return;   //�����������
	if (!guest_mode&&wtimes()) return ; 		//���� Ƚ�� �˻�

	if (!bbs_num(mmn[1])) {
		message("�ش��ȣ�� ã���� �����ϴ�.");
		return;   //�ش��ȣ ã��
	}
	strcpy(buff3,bbs.id);
	strcpy(buff,bbs.title);
    strcpy(buff2,"");
    if (is_same(mmc[0],"an")||is_same(mmc[0],"at")) { //�����и��������
        if (strlen(mmc[3])>0&&atoi(mmc[3])==0) strcpy(buff2,mmc[3]);
        else if (strlen(mmc[2])>0&&atoi(mmc[2])==0) strcpy(buff2,mmc[2]);
    }

    if (is_same(mmc[0],"an")) {bbs_an(0,buff2);return;}
    else if (is_same(mmc[0],"at")) {bbs_an(1,buff2);return;}
    else if (is_same(mmc[0],"re")) {wmail(2,buff3);return;}
    else if (is_same(mmc[0],"r"))  {wmail(1,buff3);return;}
	else if (is_same(buff,"rs"))       {
		for(i=0;i<10;i++) strcpy(mmc[i],"");
		strcpy(mmc[1],bcfg.man1);  //����ڿ��� ����
		if (mmc[1][0]==0) strcpy(mmc[1],bcfg.man2);  //����ڿ��� ����
		if (mmc[1][0]==0) strcpy(mmc[1],bcfg.man3);  //����ڿ��� ����
		if (mmc[1][0]==0&&!club_mode) strcpy(mmc[1],cfg.sysop);
		if (mmc[1][0]==0&& club_mode) strcpy(mmc[1],ccfg.sysop);
        wmail(1,mmc[1]);
		return;
	}
	sprintf(buff4," (%s)",bbs.id);
	if (is_same(mmc[0],"ra")) strcpy(buff2,"[�߰�]");
	if (is_same(mmc[0],"rn")) strcpy(buff2,"[�亯]");
	if (is_same(mmc[0],"rt")) strcpy(buff2,"[����]");
	if (is_same(mmc[0],"rr")) strcpy(buff2,"[���]");
	n=6;
	mode=0;
	for(i=0;i<strlen(buff);i++) {
		if		(buff[i]=='[') mode=1;
		else if (mode==1&&buff[i]==']') mode=2;
		else if (mode==2) buff2[n++]=buff[i];
	}
	if (mode<2) strcat(buff2,buff);
	else buff2[n]=0;
	buff2[60]=0;
	strcpy(buff,buff2);
	strcat(buff,buff4);
	buff[60]=0;

	strcpy(bbs.title,buff);
	strcpy(bbs.name,pf.name);
	strcpy(bbs.key,"");
    strcpy(bbs.passwd,"");
	bbs.read=0;
	bbs.del =0;
	bbs.ok	=0;
	bbs.down=0;
	strcpy(bbs.id,pf.id);			  //������ ���̵�,�̸�
	strcpy(bbs.name,bit(bcfg.mode,8)?pf.nick:pf.name);
	time(&bbs.date);
	time(&bbs.date2);
	bbs.gd=' ';
	sprintf(bbs.key,"%d",bbs.num);
	printf("\n    < %s >\n",bbs.title);
	unlink(TMP_FILE);
	ret=bbs_editor(1);
	if (ret) ret=bbs_add(0);  //�Խù����� ����
	if (ret) {
		//ȭ�ϸ��Է�,����
		sprintf(buff,"      ** %s�Բ� �亯�Ͻ� ���� ����Ǿ����ϴ�. **.",buff3);
		message(buff);
		add_zgo();		//��ü�ڷ�� ���
        unlink("tmp/zbtotalb.tmp");
	}
	unlink(TMP_FILE);
	re_print=Yes;
}


//�ش� �Ǵ� �Խ��ǿ� ���� ���� �߰��ϱ�
//num:�Խù� ��ȣ,mode:0: �亯,1:�߰�
bbs_an(int mode,char *cmdstr)
{
    char buff[80],buff1[80],c;
	FILE *fp1,*fp2;
	// �亯 ���� ���� Ȯ��
	if ( !(go.type=='q'||now_type>15||pf.type>15||bbs_man(pf.id)) ) {
		mess("�亯�� ������ �����ϴ�.");
		return;
	}

    if (strlen(cmdstr)>0) {     //�����и�� ������
        sprintf(buff,"frm/bbs/%s.txt",cmdstr);
        if (!is_file(buff)) {
            mess(" [%s] �ش� �亯������ �����ϴ�.",cmdstr);
            return;
        }
        else {
            bbs2tmp();    //����BBS�� ������ TMP�� ����
            file_move(TMP_FILE,TMP_FILE2);
            touch_file(TMP_FILE2);
            printf("\n\n %s(%s) %-40.40s...  [%s]\n",bbs.id,bbs.name,bbs.title,mode==0?"�亯":"�߰�");
            printf("  [%s] ����\n",cmdstr);
            file_copy(buff,TMP_FILE);
            goto _cont1;
        }

    }
	//������ �Է¹޴´�. ->TMP_FILE��
	bbs2tmp();	  //����BBS�� ������ TMP�� ����
	file_move(TMP_FILE,TMP_FILE2);
	touch_file(TMP_FILE2);
	printf("\n\n %s(%s) %-40.40s...  [%s]\n",bbs.id,bbs.name,bbs.title,mode==0?"�亯":"�߰�");
	if (input_tmp(0)==No) return;

    _cont1:
	if ((fp2=fopen(TMP_FILE,"r+"))==NULL) return;
	if ((fp1=fopen(TMP_FILE2,"a+"))==NULL) fp1=fopen(TMP_FILE2,"w+");
	fprintf(fp1,"\n   ###### [%s%d] ID: %s (%s) �Ͻ� %s ######\n",mode==0?"�亯":"�߰�",calc_ok(3)+1,pf.id,pf.nick,now_str(3));
    while (!feof(fp2)) {
        c=fgetc(fp2);
        if (c==0||(byte)c==255) continue;
        fputc(c,fp1);
    }
	fclose (fp1);
	fclose (fp2);
	if ((fp1=fopen(TMP_FILE1,"w+"))!=NULL) {
		bbs.ok+=1000000;  //����Ƚ�� �߰�
		fwrite(&bbs,sizeof_bbs,1,fp1);	//���纯��� ũ�� ����
	}
	fclose (fp1);
    sync();
	sys_tem("bin/ed_bbs 999 %s %s %s %d",NOW_FILE,TMP_FILE1,TMP_FILE2,NOW_REC);
    unlink("tmp/zbtotalb.tmp");
	unlink(TMP_FILE);
	unlink(TMP_FILE1);
    unlink(TMP_FILE2);
    sync();
}


//����	0:���� 1:���� 2:������ �ٿ �谨 ���� 3:�����ڷ�Ƿ� �ű�
bbs_del(int mode)
{
	FILE *fp1;
    char buff[80];
	int num;
	int i;
	int old_level=pf.level;
	int old_down=pf.down;
	char modestr[20];
	char OLDDIR[300];
	time_t t;
	if (guest_mode) return No;
	getcwd(OLDDIR,sizeof(OLDDIR));		//������ ���丮

	if (!is_char(BBSTYPE,go.type)) return No;
	if (mode==1) strcpy(modestr,"����");
	else		 strcpy(modestr,"����");
	if ((mode==2||mode==3)&&!in_file2("system/bbsdel.id",pf.id)&&pf.type<17) mode=0;
	if (pf.type<17&&now_type<17&&bit(bcfg.form,3)) {
		mess(" �����Ҽ� ���� �Խ����Դϴ�.");
		return No;
	}
	re_print=Yes;
	num=mmn[1];
	if (num==0&&list_mode) {
		num=bbs.num;
		mmn[1]=num;
		list_mode=No;
	}
	if (num<1) {
		printf("%s �� �ڷ� ��ȣ ����  >>",modestr);
		lineinput(buff,30); 	   //��ɾ� �Է�
		check_cmd(buff);		   //��ɾ�� ����
		sprintf(buff,"%s�� ��ҵǾ����ϴ�.",modestr);
		if (mmn[1]<1) { message(buff);return No; }
	}

	re_print=Yes;
	for (i=1;i<101;i++) {
		if (mmn[i]>0) {
			if (!bbs_num(mmn[i])&&NOW_REC<NOW_ALLREC){
				printf(" %d ���� �ڷᰡ �����ϴ�.\n",mmn[i]);
			}
			else {
				sprintf(buff,"M:%d S:%dK",mode,bbs.filesize);
				if (is_same(go.code,"cmail")) {      //�����϶� ���� ���� ����
					printf("\n\n ## �����Ҹ���: %d�� %s\n",bbs.num,bbs.title);
					printf(" ## %s �Կ��� ���� ������ ����ڽ��ϱ�? (Y/n) >>",bbs.id);
					if (yesno("",Yes)) {
						sys_tem("bin/delmail '%s' %d",pf.id,bbs.num);
						re_bbs=Yes;
						re_print=Yes;
						continue;
					}
					printf("\n");
				}
				if (check_del()||is_same(go.code,"userroom")) {      //��������
					//�Խù� �Ѽ�
					bbs_set_(1,bbs.num,mode);	  //����,����
					if ((mode==0||mode==3)&&
						!is_same("userroom",go.code)&&
						!is_same("bbs_all",go.code)&&
						!is_same("pds_all",go.code)&&
						!is_char("ozdun",go.type) ) {
						if (!bit(bcfg.mode,1)||(!bit(bcfg.mode,2)&&time_day(bbs.date)<3))
                            bbs_down_up(bbs.filesize,bbs.id,-1);
                        if (is_typepds) set_rec(1,26);  //����
                        else            set_rec(1,25);
					}
					if (pf.type>13&&mode==3&&is_typepds) {
                        if (!is_file("system/no_del.dat")) {
							sprintf(buff,"%s/%s",go.dir,bbs.filename);
                            if (is_file(buff)) unlink(buff);
						}
					}
					if (mode==3&&is_same("userroom",go.code)) {
						sprintf(buff,"roombin/%s/%s/%s",c_han(pf2.id),pf2.id,bbs.filename);
						unlink(buff);
					}
					if (mode==3&&is_same("rmail",go.code)) {
						sprintf(buff,"roombin/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
						unlink(buff);
					}
				}
			}
		}
	}
//    re_bbsnum(3);           //BREC 0:�б� 1:�ڵ�  2:����  3:��ü����  8:1���߰� 9:1������
    if (is_typebbs) unlink("tmp/zbtotalb.tmp");
    else            unlink("tmp/zbtotalp.tmp");
	return Yes;
}


//����Խù� ��ȯ
bbs_gd(int mode)
{
	FILE *fp;
	char buff[80];
	int num;
	int ret,i,i2,found,count=0;
	BBS bbs2;

	if (is_char(BBSTYPE,go.type)==No) return No;
	ret=No;
	if (bbs_man(pf.id)) ret=Yes; //�Խ����� ������� ���
	if (pf.type>14) 	ret=Yes; //����̻��϶�
	if (now_type>=14)	ret=Yes; //����̻��϶�
	if (!ret) {message("��õ ������ �����ϴ�.");return No;}

	num=mmn[1];
	if (num<1) {
		printf("��� �ڷ�� ������ �ڷ� ��ȣ �Է� >>");
		lineinput(buff,10); 	   //��ɾ� �Է�
		check_cmd(buff);		   //��ɾ�� ����
		if (mmn[0]<1) { message("��ҵǾ����ϴ�.");return No;  }
	}

	for (i=0;i<101;i++) {
		if (bbs_num(mmn[i])) {
			if ((fp=fopen(NOW_FILE,"r+"))==NULL) return No;
			fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
			if (fread(&bbs2,sizeof_bbs,1,fp)) {
				i2=0;
				if		(mode==1) {i2=1;bbs2.gd='#';}
				else if (mode==2) {i2=0;bbs2.gd=' ';}
                else if (mode==3) {i2=0;bbs2.gd='*';}
				else {
					found=No;
					for (i2=0;i2<9;i2++) {
						if (cfg.gdstr[i2]==bbs2.gd) {
							bbs2.gd=cfg.gdstr[i2+1];
							found=Yes;
							break;
						}
					}
					if (!found) bbs2.gd=cfg.gdstr[1];
					if (bbs2.gd==0) bbs2.gd=' ';
				}
				c_lock(1,NOW_FILE);
				fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
				if (!feof(fp )) fwrite(&bbs2,sizeof_bbs,1,fp);
				c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
				count++;
				printf("%7d. %-8s %-40.40s  -> ��õ:%d('%c') ����\n",
				bbs2.num,bbs2.id,bbs2.title,i2,bbs2.gd);
			}
			fclose(fp);
		}
	}
    sync();
	bbs_num(mmn[0]);
	return Yes;
}



//mode1:���� 2:��õ 3:����
//mode2=0:���� 1:����
bbs_set_(int mode1,int num,int mode2)	 //�ش� ��ȣ �Խù� ó��
{
	FILE *fp;
	BBS bbs2;
	int i2,total,found;
	int old_rec=NOW_REC;
	char buff[100];

	c_lock(1,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����

	if (!bbs_num(num)) goto _quit1;
	fp=fopen(NOW_FILE,"r+");      //����ȭ��
	fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
	if (fread(&bbs2,sizeof_bbs,1,fp)&&bbs2.num==num) {
		if (mode1==1) {
            if (mode2==1) bbs2.del=No;
			else		  bbs2.del=Yes;
            time(&bbs2.date2);  //���� ��¥ ����
		}
		else if (mode1==2) {
			if		(mode2==1)		bbs2.gd='#';
			else if (mode2==2)		bbs2.gd=' ';
			else if (mode2==3)		bbs2.ok+=1000000;  //����Ƚ�� �߰�
			else {
				found=No;
				for (i2=0;i2<9;i2++) {
					if (cfg.gdstr[i2]==bbs2.gd) {
						bbs2.gd=cfg.gdstr[i2+1];
						found=Yes;
						break;
					}
				}
				if (!found) bbs2.gd=cfg.gdstr[1];
				if (bbs2.gd==0) bbs2.gd=' ';
			}
		}
		fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
		if (!feof(fp )) fwrite(&bbs2,sizeof_bbs,1,fp);
	}
	fclose(fp);
    sync();

	_quit1:
	c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
	NOW_REC=old_rec;
}



//ls,ll ó��
bbs_list(int n)
{
	list_mode=No;
	re_print=Yes;
	if (n==0) {
		NOW_TOPREC=NOW_ALLREC;
		NOW_NO=NOW_ALLNO;
		NOW_REC=NOW_ALLREC-1;
        NOW_TOPREC=NOW_REC+1;
        NOW_TOPNO =NOW_NO;
		re_bbs=Yes;
	}
	else {
		if (bbs_num(n)) {
//            bbs_back();
//            disp_bbsstat();
//            dev();
			NOW_NO=bbs.num;
            NOW_TOPREC=NOW_REC+1;
			NOW_TOPNO =NOW_NO;
		}
		else bbs_list(0);
	}
}


//������ �����ڵ��� ���丮�� ȭ���� �ִ°�?
//������ Yes ������ No
is_gofile(GO ttgo,char *filename)
{
	char buff[220];
	if(filename[0]==0) return No;
	if (is_same(ttgo.code,"userroom")) {
		sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,filename);
        return (is_file(buff));
	}
	if(ttgo.dir[0]==0) return No;
	sprintf(buff,"%s/%s",ttgo.dir,filename);
	return (access(buff,0)!=-1);
}


//���� ���� ���� Ȯ�� ���������� No �ȵǸ� Yes
wtimes()
{
	FILE *fp;
	int key=No,ret=No;
	char st[10];
	char st1[10];
	char c,buff[550];
	int i,ii,num,mode;
	del_space(go.wtimes);
	if (guest_mode&&go.wtimes[0]==0||now_type>=14||pf.type>=14||bbs_man(pf.id)) return No;
	ii=0;
	mode=0;
	if		(is_char(go.wtimes,'a')) {mode=1;strcpy(st,"��ü");}
	else if (is_char(go.wtimes,'t')) {mode=1;strcpy(st,"��ü");}
	else if (is_char(go.wtimes,'m')) {mode=2;strcpy(st,"�Ѵ޿�");}
	else if (is_char(go.wtimes,'w')) {mode=3;strcpy(st,"���ֿ�");}
	else if (is_char(go.wtimes,'d')) {mode=4;strcpy(st,"�Ϸ翡");}
	bzero(st1,5);
	for (i=0,ii=0;i<5;i++) {c=go.wtimes[i];if (c>='0'&&c<='9') st1[ii++]=c;}
	del_space(st1);del_esc(st1);num=atoi(st1);
	if (mode!=0&&num!=0&&count_bbs(mode,num)) {  //�Խù��� �����
		printf("\n\n*** �� �Խ����� %s %d �� ������ ������ �ֽ��ϴ�. ***\n",st,num);
		printf("*** ���� ���� �Խù��� ����� ����Ͻʽÿ�. ***\n");
		pressenter();
		return Yes;
	}
	return No;
}


count_bbs(int mode,int n)
{
	FILE *fp;
	int num=0,total,i;
	int nn;
	struct tm *localtime();
	time_t today;
	time_t liday;
	BBS bbs2;
	struct tm *tp1;
	struct tm *tp2;

	time(&today);	//������ ��¥ ����
	tp1 = localtime(&today);
	tp1->tm_hour= 23;
	tp1->tm_min= 59;
	tp1->tm_sec= 59;
	today=mktime(tp1);

	liday = 0								 ;	 //������� ��¥
	if (mode==1) liday = 0					 ;	 //������� ��¥
	if (mode==2) liday = today - (30*24*3600);	 //������� ��¥
	if (mode==3) liday = today - ( 7*24*3600);	 //������� ��¥
	if (mode==4) liday = today - ( 1*24*3600);	 //������� ��¥

	fp=fopen(NOW_FILE,"r+");
	if (fp==NULL) return No;
	fseekend(fp);
	total=ftell(fp)/sizeof_bbs;

	for (i=total;i>0&&num<30&&num<=n;i--) {
		fseek(fp,(i-1)*sizeof_bbs,SEEK_SET);
		if (fread(&bbs2,sizeof_bbs,1,fp)<1) break;
		if (bbs2.date<liday) break;
		if (!bbs2.del&&is_same(bbs2.id,pf.id)) num++;
	}
	if (fp!=NULL) fclose(fp);
	if (num<n) return No;
	return Yes;
}


//������ȣ�� �Խù�
go_back_list()
{
	BBS bbs1;
	int i,iold;
	FILE *fp;

	view_pos=0;
	re_print=Yes;
	iold=NOW_REC;

	fp=fopen(NOW_FILE,"r+");      //����ȭ��
	if (fp==NULL) return Yes;
	for (i=NOW_REC+1;i<NOW_ALLREC;i++) {
		fseek(fp,i*sizeof_bbs,SEEK_SET);
		fread(&bbs1,sizeof_bbs,1,fp);
		if (!bbs1.del) break;
	}
	if (i<NOW_ALLREC) {
		bbs=bbs1;
		NOW_NO=bbs1.num;
		NOW_TOPNO=bbs1.num;
		NOW_REC=i;
		bbs_read_num();
		add_read_numn(bbs.num); 	  //���� ǥ�� �߰�
	}
	if (fp!=NULL) fclose(fp);
	if ((i>=NOW_ALLREC||iold==NOW_REC)) {
		message("[7m << ��ó���Դϴ� >>[m");
		bbs_list(0);
	}
	return(Yes);
}


//������ȣ�� �Խù�
go_next_list()
{
	BBS bbs1;
	int i,iold;
	FILE *fp;

	view_pos=0;
	re_print=Yes;
	iold=NOW_REC;

	fp=fopen(NOW_FILE,"r+");      //����ȭ��
	if (fp==NULL) return No;
	for (i=NOW_REC-1;i>=0;i--) {
		fseek(fp,i*sizeof_bbs,SEEK_SET);
		fread(&bbs1,sizeof_bbs,1,fp);
		if (bbs1.del==0) break;
	}
	if (i>=0) {
		bbs=bbs1;
		NOW_NO=bbs1.num;
		NOW_TOPNO=bbs1.num;
		NOW_REC=i;
		bbs_read_num();
		add_read_numn(bbs.num); 	  //���� ǥ�� �߰�
	}
	if ((i<=0||iold==NOW_REC)&&bbs.line<15) message("[7m << �������Դϴ� >>[m");
	if (fp!=NULL) fclose(fp);
	return(Yes);
}


//��ü�ڷ�ǿ� �����Ѵ�.
//���� ������ �ڷῡ�� �о� �����Ѵ�.
add_zgo()
{
	char filename[130];
	char buff[130];
	char oldid	[30];
	char oldname[30];
	char tNOW_FILE[100];
	char tNOW_FILED[100];
	BBS oldbbs;
	FILE *fp;
    oldbbs=bbs;
	if (bit(cfg.mode,24)) return;
	if (bit(bcfg.wmode,15)) return;
    if (go.type=='z'||go.type=='b') return;
	if (is_char("ok",go.type)) {
		strcpy(bbs.id  ,"........");
		strcpy(bbs.name,"........");
	}
	if (stristr(NOW_FILE,"/bbs_all.tit")) return;
	strcpy(tNOW_FILE ,NOW_FILE );
	strcpy(tNOW_FILED,NOW_FILED);

	strcpy(NOW_FILE ,"menu/bbs_all.tit");
	strcpy(NOW_FILED,"menu/bbs_all.txt");
	if (is_typepds) {
	strcpy(NOW_FILE ,"menu/pds_all.tit");
	strcpy(NOW_FILED,"menu/pds_all.txt");
	}
	strcpy(bbs.tcode,go.code);
	sprintf(bbs.key,club_go.code);
    bbs.gd=' ';
	bbs_add(1);  //�Խù����� ���� ���� ������
	strcpy(NOW_FILE ,tNOW_FILE);
	strcpy(NOW_FILED,tNOW_FILED);
    bbs=oldbbs;
}

//�Խ��� ���� ���,�Խ��Ǵ�縸 ����
//cp hello 1 2 3 4 5....
//mode2= 0:����<->����,��ȣȸ<->��ȣȸ	(cp/mv)
//		 1:����->��ȣȸ (cpclub/mvclub)  2:��ȣȸ->���� (cpmain/mvmain)
bbs_copy(char *cmd)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char buff[180];
	char buff2[180];
	char NOW_FILE2[180];
	char NOW_FILED2[180];
	char cpstr[80];
	char c;
	int type_mode=0;
	int found=No;
	int ret,i,i2,new_posi;
	GO oldgo,oldclub_go,newgo,ttgo;
	BBS lastbbs;	 //�̵������ ������ BBS
	char newgo_dir[150];
	int oldclub_mode;
	int mode,mode2,mode3;
	int bbs_rec;
	int new_bbs_rec;   //���ο� �Խ����� ���ڵ��ȣ
	int ii=0,count=0;
	int cont_mode=No;  //���Ӹ��
	int quit_mode=No;
    int no_check_file=No;

	list_mode=No;
	re_print=Yes;
	ret=Yes;
	mode=mode2=0;
	oldgo=go;
	oldclub_go=club_go;
	oldclub_mode=club_mode;
	strcpy(cpstr,"����");

	check_cmd(cmd);
	printf("\n");
	mode=mode2=mode3=0;
	if (guest_mode||!is_char(BBSTYPE,go.type)) goto fail_bbscopy;
    if (pf.type<18&&go.type=='k') {
		sprintf(buff,"�������� �ʴ� �޴�Ÿ�� %c �Դϴ�.",go.type);
		goto fail_bbscopy;
	}
	if (pf.type<14&&(is_same(go.code,"rmail")||is_same(go.code,"mbox")||is_same(go.code,"cmail"))) {
		sprintf(buff,"����,�̵������ ���� ���� �Խ���");
		goto fail_bbscopy;
	}
    if (pf.type<18&&!bbs_man(pf.id)) {
		sprintf(buff,"����,�̵������ ���� ���� ���");
		goto fail_bbscopy;
	}
	else if (is_same(mmc[0],"cp"    )) {mode=0;mode2=0;}
	else if (is_same(mmc[0],"cpp"   )) {mode=0;mode2=0;mode3=1;}
	else if (is_same(mmc[0],"cpmain")) {mode=0;mode2=1;}
	else if (is_same(mmc[0],"cppmain")){mode=0;mode2=1;mode3=1;}
	else if (is_same(mmc[0],"cpclub")) {mode=0;mode2=2;} //cpclub pbal1 100
	else if (is_same(mmc[0],"cppclub")){mode=0;mode2=2;mode3=1;} //cpclub pbal1 100
	else if (is_same(mmc[0],"mv"    )) {mode=1;mode2=0;}
	else if (is_same(mmc[0],"mvv"   )) {mode=1;mode2=0;mode3=1;}
	else if (is_same(mmc[0],"mvmain")) {mode=1;mode2=1;}
	else if (is_same(mmc[0],"mvvmain")){mode=1;mode2=1;mode3=1;}
	else if (is_same(mmc[0],"mvclub")) {mode=1;mode2=2;}
	else if (is_same(mmc[0],"mvvclub")){mode=1;mode2=2;mode3=1;}

    if (mmc[1][0]==0) ret=No;
	if (mode2<=1&&atoi(mmc[2])==0) ret=No;
	if (mode2==2&&atoi(mmc[2])!=0) ret=No;
	if (ret==No) {
		view("frm/help/bbscp.hlp");
		return;
	}
	strcpy(cpstr,mode?"�̵�":"����");

    if (is_typepds) type_mode=1; //�ڷ��Ÿ��
    else            type_mode=0; //�Խ���Ÿ��

	ret=Yes;
	//���� ��� ��ȣȸ �ڷ� �˻�
	if (mode2==0) { 				 //����->����,��ȣȸ->��ȣȸ
		ret=find_go(mmc[1]);
		if (!ret) {
			sprintf(buff,"[%s] �����ڵ尡 �����ϴ�.",mmc[1]);
			goto fail_bbscopy;
		}
		set_filename(); 			 //����ǥ���� ȭ�ϸ�
		strcpy(NOW_FILE2,NOW_FILE);
		strcpy(NOW_FILED2,NOW_FILED);
		newgo=go;
		strcpy(newgo_dir,go.dir);		  //�ڷ���� ��쿡�� ���
	}
	else if (mode2==1) {						 //��ȣȸ->����
		set_top();
		set_nowmenu();
		ret=find_go(mmc[1]);
		if (!ret) {
			sprintf(buff,"[%s] �����ڵ尡 �����ϴ�.",mmc[1]);
			goto fail_bbscopy;
		}
		set_filename(); 						 //����ǥ���� ȭ�ϸ�
		strcpy(NOW_FILE2,NOW_FILE);
		strcpy(NOW_FILED2,NOW_FILED);
		newgo=go;
		strcpy(newgo_dir,go.dir);		  //�ڷ���� ��쿡�� ���
	}
	else if (mode2==2) {			  //����->��ȣȸ, ��ȣȸ->Ÿ��ȣȸ
		club_mode=Yes;
		ret=find_club(mmc[1]);					  //��ȣȸ ã��
		if (!ret) {
			sprintf(buff,"[%s] ��ȣȸ�� �����ϴ�.",mmc[1]);
			goto fail_bbscopy;
		}
		ret=find_go(mmc[2]);		   //�ű� �����ڵ� ã��
		if (!ret) {
			sprintf(buff,"[%s] �����ڵ尡 �����ϴ�.",mmc[2]);
			goto fail_bbscopy;
		}
		set_filename(); 	  //����ǥ���� ȭ�ϸ�
		strcpy(NOW_FILE2,NOW_FILE);
		strcpy(NOW_FILED2,NOW_FILED);
		newgo=go;
		strcpy(newgo_dir,go.dir);		  //�ڷ���� ��쿡�� ���
	}
	if (mode2==0&&is_same(oldgo.code,newgo.code)) {
		sprintf(buff,"���� %s �ڵ� %s �Ұ���",oldgo.code,cpstr);
		goto fail_bbscopy;
	}
    if (pf.type<18&&!is_char(BBSTYPE,newgo.type)) {
		sprintf(buff,"�Խ��ǶǴ� �ڷ�� Ÿ���̾�� �մϴ�.");
		goto fail_bbscopy;
	}

	go=oldgo;
	club_mode=oldclub_mode;
	club_go=oldclub_go;
	set_nowmenu();
	set_filename(); 							//�ű��� �� ȭ�ϸ�

	fp1=fopen(NOW_FILE,"r+");       //����ȭ��
	if (fp1==NULL) {
		sprintf(buff,"%s ȭ���� �����ϴ�.",NOW_FILE);
		goto fail_bbscopy;
	}
	fp2=fopen(NOW_FILED,"r+");      //����ȭ��
	fp3=fopen(NOW_FILE2,"r+");      //����ȭ�� (����� �����ڵ�)
	if (fp3==NULL) {
		sprintf(buff,"%s�� �Խ���ȭ��(%s)�� �����ϴ�.(�������)",cpstr,NOW_FILE2);
		if (fp1!=NULL) fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
		goto fail_bbscopy;
	}
	fp4=fopen(NOW_FILED2,"r+");      //����ȭ��

	//�̵��������� ������ ��ȣ �ľ�
	fseekend(fp3);
	new_bbs_rec=ftell(fp3)/sizeof_bbs;
	if (new_bbs_rec<1) {
		lastbbs.num=0;
		lastbbs.posi=0;
		lastbbs.size=0;
	}
	else {
		fseek(fp3,(new_bbs_rec-1)*sizeof_bbs,SEEK_SET);  //���ϸ�������ȣ��
		fread(&lastbbs,sizeof_bbs,1,fp3);
	}


	for (i2=0;i2<101;i2++) {
		found=Yes;
		if (mmn[i2]>0) {
			//�ش�Ǵ� ��ȣ ã��
			rewind(fp1);
			found=No;
			bbs_rec=-1;    //ã�� ��ġ
			while (fread(&bbs,sizeof_bbs,1,fp1)>0) {
				bbs_rec++;
				if (bbs.num==mmn[i2]) {  //��ȣ�� ����.
					found=Yes;
					if (bbs.del&&!dd_mode) found=No;  //������ �ڷ��϶��� �����
                    if (bbs.filesize==0&&!bit(cfg.bbsmode,5)&&!bit(bcfg.modelist,19)&&!dd_mode&&is_pds) found=No;
                    if (found&&go.type!='z'&&go.type!='b') {
						printf("%d %s ",bbs.num,bbs.id);
						if (is_typepds) printf("%s ",bbs.filename);
						printf("%s\n",bbs.title);
						if (cont_mode) found=Yes;	//���Ӹ��
						else {
							printf(" �ڷḦ %s�ұ��? (����:Y ���:N ����:A ����:Q) >>",cpstr);
							lineinput(buff,2);check_han(buff);printf("\n");
							if (is_same(buff,"n")) found=No;
							if (is_same(buff,"a")) cont_mode=Yes;
							if (is_same(buff,"q")) quit_mode=Yes;
						}
					}
					break;
				}
			}
			if (quit_mode) break;
			if (bbs.id[0]==0) strcpy(bbs.id,pf.id);
			if (bbs.name[0]==0) strcpy(bbs.name,bit(bcfg.mode,8)?pf.nick:pf.name);

			//�ڷ���� ��� �̵��̸� �����ڷ� ����,����� �״�� ��
			if (found&&is_typepds&&!is_char("z",oldgo.type)) {
				sprintf(buff,"%s/%s",newgo.dir,bbs.filename);
				sprintf(buff2,"%s/%s",oldgo.dir,bbs.filename);
				if (oldgo.type=='c') get_cdrom(buff2,bbs.num);
				printf("\n %s  -> %s %s�� \n",buff2,buff,cpstr);
                if (!is_file(buff2)) found=No;
                else if (found&&is_file(buff)) {
					printf("\n %s �ڷ�ǿ� %s ȭ���� �̹� �ֽ��ϴ�.\n",newgo.dir,bbs.filename);
					if (cont_mode) found=Yes;
					else if (yesno("�׷��� ��� �ϰڽ��ϱ�? (Y:����� n:���) >>",No)==No) found=No;
				}
				if (found) {
					sprintf(buff,"%s/%s",oldgo.dir,bbs.filename);
					if (oldgo.type=='c') get_cdrom(buff,bbs.num);
                    if (is_file(buff)) {
						sprintf(buff2,"cp %s %s/%s -f",buff,newgo.dir,bbs.filename);
						sys_tem(buff2);
						sprintf(buff,"%s/%s",newgo.dir,bbs.filename);
                        if (!is_file(buff)) {
							printf("\n%s ȭ�� %s ����\n",buff,cpstr);
							printf("\n%s ���丮�� Ȯ���Ͻʽÿ�.\n",buff);
							found=No;
						}
						if (found&&mode==1&&oldgo.type!='c') {
							sprintf(buff,"%s/%s",oldgo.dir,bbs.filename);
							unlink(buff);
						}
					}
				}
                if (no_check_file) found=Yes;
                else if (!found&&pf.type<18) {
                    printf("\n ### ���Ϻ������ ���(Y) ���(N) ����(A)>>");
                    lineinput2(buff,3);check_han();
                    if (is_same(buff,"a")) {no_check_file=Yes;found=Yes;}
                    else if (!is_same(buff,"n")) found=Yes;
                }
                else if (!found) printf("\n **** %s ȭ�� %s ����. ****\n",bbs.filename,cpstr);
			}

			if (bbs_rec>=0&&found) {
				count++;
				//���� bbs �б�
				c_lock(1,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
				c_lock(1,NOW_FILE2);				   //LOCKȮ��  1:Ȯ�� 0:����
				fseek(fp1,bbs_rec*sizeof_bbs,SEEK_SET);
				fread(&bbs,sizeof_bbs,1,fp1);
				if (mode==1) {	//���� mode=1�̸� ������ �Ѵ�(�̵��϶�)
					bbs.del=Yes;	 //����ó��
					fseek(fp1,bbs_rec*sizeof_bbs,SEEK_SET);
					if (!feof(fp1)) fwrite(&bbs,sizeof_bbs,1,fp1);
				}

				//fp2 -> fp4 (����)
                sync();
				fseek(fp2,bbs.posi,SEEK_SET);
				bbs.posi=lastbbs.posi+lastbbs.size; //���ο� �Խ�����ġ
				fseek(fp4,bbs.posi,SEEK_SET);
				for (i=0;i<bbs.size;i++) fputc(fgetc(fp2),fp4);

				//fp1 -> fp3 (����)
				//���ο� �Խ����� ������ ��ȣ ����
				bbs.num=lastbbs.num+1;
				if (bbs.id[0]==0||bbs.name[0]==0) {
					strcpy(bbs.id,pf.id);
					strcpy(bbs.name,pf.name);
				}
				fseek(fp3,new_bbs_rec*sizeof_bbs,SEEK_SET);
				bbs.del=No;
				if (mode3==0) time(&bbs.date);	 //mode3=1 �̸� ��¥����Ƚ�Ŵ
				if (!feof(fp3)) fwrite(&bbs,sizeof_bbs,1,fp3);
                sync();
				new_bbs_rec++;
				lastbbs=bbs;	//���ο� ������ �Խ���
				if (is_char("un",oldgo.type)) {   //������ �ڷ���϶� �ٿ�/��������
					ttgo=go;
					go=newgo;
                    read_bcfg();
					bbs_down_up(bbs.filesize,bbs.id,1);
                    go=ttgo;
                    read_bcfg();
				}
				c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
				c_lock(0,NOW_FILE2);				   //LOCKȮ��  1:Ȯ�� 0:����
                sync();
			}
		}
	}

	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (fp3!=NULL) fclose(fp3);
	if (fp4!=NULL) fclose(fp4);
    sync();

	go=oldgo;
	club_mode=oldclub_mode;
	club_go=oldclub_go;
	set_nowmenu();
	set_filename(); 								//�ű��� �� ȭ�ϸ�
	re_bbsnum(3);			//BREC 0:�б� 1:�ڵ�  2:����  3:��ü����  8:1���߰� 9:1������
	return;

	fail_bbscopy:
	if (strlen(buff)<1) sprintf(buff,"%s �۾��� ��ҵǾ����ϴ�.",cpstr);
	mess(buff);
	go=oldgo;
	club_mode=oldclub_mode;
	club_go=oldclub_go;
	set_nowmenu();
	set_filename(); 								//�ű��� �� ȭ�ϸ�
	re_bbsnum(3);			//BREC 0:�б� 1:�ڵ�  2:����  3:��ü����  8:1���߰� 9:1������
	return;
}




file_of_num(char *filename,int num)
{
	char buff[200];
	int i;
	if (num>0) {
		for (i=strlen(filename);i>1;i--)
			if (filename[i]=='.') filename[i-1]+=(char)num;
	}
}




//�Խ��� ã��
//lt ���� ������� ã�� �Ǹ� �ش�Ǵ� �Խù���
//�ӽ�ȭ�Ͽ� ���� �����Ѵ�.
//re_bbs�ÿ� �ٽ� ������� ���� �´�.
//w �Ǵ� ���� ����� ����ɶ� �ٽÿ����� ȭ�Ϸ� ���ƿ´�.
//���� �˻�����϶� bbs_ltmode=Yes;
bbs_search()
{
	int mode,n,i;
	int mode2;
	FILE *fp1,*fp2;
	char buff[100];
	int ok;
	int found_num=0; //�˻��� ��

    if (bit(bcfg.modelist,20)&&!bbs_man(pf.id)) {
        mess("�˻������ �����Ǿ����ϴ�.");
        return;
    }
	re_print=Yes;
	list_mode=No;
	if (mmc[1][0]==0&&!is_same(mmc[0],"lg")&&!is_same(mmc[0],"lst")) {
		printf("\n");
		view("frm/help/_search.hlp");
		bbs_ltmode=No;
		re_bbs=Yes;
		set_filename();
		pressenter();
		return;
	}
	if		(is_same(mmc[0],"lt")) mode=1;
	else if (is_same(mmc[0],"li")) mode=2;
    else if (is_same(mmc[0],"ln")) mode=11;
	else if (is_same(mmc[0],"lf")) mode=3;
	else if (is_same(mmc[0],"lk")) mode=4;
	else if (is_same(mmc[0],"le")) mode=5;
	else if (is_same(mmc[0],"lg")) mode=6;
	else if (is_same(mmc[0],"lsc")) mode=7;
	else if (is_same(mmc[0],"lsl")) mode=8;
	else if (is_same(mmc[0],"lu")) mode=9;
	else if (is_same(mmc[0],"lc")) mode=10;
	else return;
    if (go.type=='k'||go.type=='o') {
        if (mode==2||mode==11) return;  //����� ���
    }
	if (bbs_ltmode) {	   //lt���¿��� �� �θ����
		bbs_ltmode=No;
		set_filename();
		check_bbs();
	}

	fp1=fopen(NOW_FILE,"r+");
	fp2=fopen(LT_FILE,"w+");
	strcpy(buff,mmc[1]);
	mode2=0;
	if (is_same(mmc[2],"or")||is_same(mmc[2],"+")) mode2=1;
	if (is_same(mmc[2],"and")||is_same(mmc[2],"*")) mode2=2;
	if (mmc[3][0]==0) mode2=0;


	printf(" \n ............. �˻��� �����մϴ�................\n");
	while (fread(&bbs,sizeof_bbs,1,fp1)>0) {
		ok=No;
		if (mode==1)
			if ( (mode2==0&&stristr(bbs.title,buff))||
				 (mode2==1&&(stristr(bbs.title,buff)||stristr(bbs.title,mmc[3])))||
				 (mode2==2&&(stristr(bbs.title,buff)&&stristr(bbs.title,mmc[3])))) ok=Yes;
		if (mode==2)
			if ( (mode2==0&&stristr(bbs.id,buff))||
				 (mode2==1&&(stristr(bbs.id,buff)||stristr(bbs.id,mmc[3])))||
				 (mode2==2&&(stristr(bbs.id,buff)&&stristr(bbs.id,mmc[3])))) ok=Yes;
		if (mode==3)
			if ( (mode2==0&&stristr(bbs.filename,buff))||
				 (mode2==1&&(stristr(bbs.filename,buff)||stristr(bbs.filename,mmc[3])))||
				 (mode2==2&&(stristr(bbs.filename,buff)&&stristr(bbs.filename,mmc[3])))) ok=Yes;
		if (mode==4)
			if ( (mode2==0&&stristr(bbs.key,buff))||
				 (mode2==1&&(stristr(bbs.key,buff)||stristr(bbs.key,mmc[3])))||
				 (mode2==2&&(stristr(bbs.key,buff)&&stristr(bbs.key,mmc[3])))) ok=Yes;
		if (mode==4&&stristr(bbs.key,buff)) ok=Yes;
		if (mode==5&&bbs.line<=atoi(buff))	ok=Yes;
		if (mode==6&&bbs.gd>' ')            ok=Yes;
		if (mode==7&&bbs_lsc(buff,0)) ok=Yes;
		if (mode==8&&bbs_lsc(buff,1)) ok=Yes;
		if (mode==9&&(stristr(bbs.title,buff)||bbs_lsc(buff,0))) ok=Yes;
		if (mode==10&&is_same(bbs.tcode,buff)) ok=Yes;
        if (mode==11)
            if ( (mode2==0&&stristr(bbs.name,buff))||
                 (mode2==1&&(stristr(bbs.name,buff)||stristr(bbs.name,mmc[3])))||
                 (mode2==2&&(stristr(bbs.name,buff)&&stristr(bbs.name,mmc[3])))) ok=Yes;

		if (ok) {
			if (go.type=='8') sprintf(bbs.tcode,"%s",found_num);
			if (!feof(fp2)) fwrite(&bbs,sizeof_bbs,1,fp2);
			if (found_num++>5000) {
				printf("�˻��� �ڷᰡ 5000 ���� �Ѿ� �ߴ��մϴ�.");
				pressenter();
				break;
			}
		}
	}
    sync();
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (found_num<1) {
		message("�ش�Ǵ� ������ �ڷᰡ �����ϴ�.");
		bbs_ltmode=No;
		set_filename();
		check_bbs();
	}
	else {
		bbs_ltmode=Yes;
		check_bbs();  //�ڷ��� ������ �ľ��ϴ� ��ƾ
		sprintf(buff,"## �� %d ���� �ڷ� �˻� ##",found_num);
		message(buff);
		list_mode=No;
	}
}


//������ ����ã��	mode:0 :��ҹ��� ��������
bbs_lsc(char *str,int mode)
{
	int i,i2,i3,n;
	int found=No;
	char buff[100];
	char c;
	FILE *fp;
	int s1,s2;

	fp=fopen(NOW_FILED,"r+");
	if (fp==NULL) return No;

	strcpy(buff,str);
	n=strlen(buff);
	if (mode==0) strlwr(buff);	   //0 �̸� �ҹ��ڷ� �˻�

	i2=0;
	i=0;
	i3=bbs.posi;
	found=No;
	fseek(fp,bbs.posi,SEEK_SET);
	while (i++<bbs.size&&!found) {
		c=fgetc(fp);
		if (mode==0) c=tolower(c);
		if (i2==0&&c!=buff[i2]) i3++;
		else if (c!=buff[i2])  {
			fseek(fp,i3+1,SEEK_SET);	 //�ٽ� �˻� ���
			i2=0;
			i3++;
		}
		else if (++i2>=n) found=Yes;
	}
	if (fp!=NULL) fclose(fp);
	return found;
}

//��ü �Խ��ǿ��� �ش� ���ڿ� ã��
//�������� ��������
bbs_next()
{
	FILE *fp;
	int i,back=No,oldrec;
	BBS bbs2;
	char buff[200];
	re_print=Yes;
	NOW_TOPREC=NOW_BOTREC	;  // ����ȭ�� ������ ��ȣ
	i=NOW_TOPREC;
	if (bbs_ltmode)   fp=fopen(LT_FILE,"r+");
	else			  fp=fopen(NOW_FILE,"r+");
	if (fp!=NULL) {
		while (1) {
			fseek(fp,(i-1)*sizeof_bbs,SEEK_SET);
			if (!fread(&bbs2,sizeof_bbs,1,fp)) {back=Yes;break;}	//�ڷ����
			if (bbs2.del&&!dd_mode) {
				i++;
				NOW_TOPREC=i;
				back=Yes;
				continue;
			}	 //����� �ƴҶ� �����Ȱ� �Ⱥ���
			break;
		}
		fclose(fp);
	}
	if (back) {
		file2buff(buff,"frm/error/bbsend.err",100);
		del_enter(buff);
		message(buff);
	}
}

//�������� ��������
bbs_back()
{
	FILE *fp;
	BBS bbs2;
	char buff[200];
	int i;
	re_print=Yes;

	if (NOW_TOPREC>=NOW_ALLREC) {
		file2buff(buff,"frm/error/bbstop.err",100);
		del_enter(buff);
		message(buff);
		NOW_TOPREC=NOW_ALLREC;
		return;
	}
	i=bcfg.maxline; if (i<1) i=15;
	if (bbs_ltmode)   fp=fopen(LT_FILE,"r+");
	else			  fp=fopen(NOW_FILE,"r+");
	if (fp!=NULL) {
		while (NOW_TOPREC<NOW_ALLREC) {
			NOW_TOPREC++;
			fseek(fp,(NOW_TOPREC-1)*sizeof_bbs,SEEK_SET);
			if (fread(&bbs2,sizeof_bbs,1,fp)<=0) break;
			if (bbs2.del&&!dd_mode) continue ;	  //����� �ƴҶ� �����Ȱ� �Ⱥ���
			if (i--<1) break;
		}
		fclose(fp);
	}
}


//ȭ�Ϻ���
bbs_arj()
{
	char filename[200];
	char buff[200];
	re_print=Yes;
	if (list_mode) mmn[1]=bbs.num;
	if (mmn[1]<1||!bbs_num(mmn[1])) {
		message("�ش�Ǵ� ��ȣ�� �����ϴ�.");
		return;
	}
	sprintf(filename,"%s/%s",go.dir,bbs.filename);
    if (bbs.filename[0]==0||!is_file(filename)) {
		message("�ش�Ǵ� �ڷ��� ȭ���� �����ϴ�.");
		return;
	}
	if (!((stristr(bbs.filename,".rar"))||
		  (stristr(bbs.filename,".r0"))||
		  (stristr(bbs.filename,".arj"))||
		  (stristr(bbs.filename,".a0"))||
		  (stristr(bbs.filename,".tgz"))||
		  (stristr(bbs.filename,".tar"))||
		  (stristr(bbs.filename,".lha"))||
		  (stristr(bbs.filename,".lzh"))||
		  (stristr(bbs.filename,".zip")))) {
			printf("\n �����ϴ� ���� ������ ������ �����ϴ�.\n");
			printf(" RAR / ARJ / TGZ / TAR / LHA / ZIP\n");
			pressenter();
			return;
	}
	unlink(TMP_FILE2);
	if		(stristr(bbs.filename,".rar")) {sprintf(buff,"rar l %s > %s",filename,TMP_FILE2);sys_tem(buff);    }
	else if (stristr(bbs.filename,".r0")) {sprintf(buff,"rar l %s > %s",filename,TMP_FILE2);sys_tem(buff);    }
	else if (stristr(bbs.filename,".arj")) {sprintf(buff,"unarj l %s > %s",filename,TMP_FILE2);sys_tem(buff);  }
	else if (stristr(bbs.filename,".a0")) {sprintf(buff,"unarj l %s > %s",filename,TMP_FILE2);sys_tem(buff);  }
	else if (stristr(bbs.filename,".tgz")) {sprintf(buff,"tar tvfz %s > %s",filename,TMP_FILE2);sys_tem(buff);}
	else if (stristr(bbs.filename,".tar")) {sprintf(buff,"tar tvf %s > %s",filename,TMP_FILE2);sys_tem(buff);  }
	else {
		printf("\n\n        ���� ȭ�� ����   %s\n",bbs.filename);
		sprintf(buff,"bin/al %s",filename);sys_tem(buff);
		pressenter();
	}
    if (is_file(TMP_FILE2)) {
		printf("\n\n        ���� ȭ�� ����   %s\n",bbs.filename);
		view_text(TMP_FILE2);
		unlink(TMP_FILE2);
	}
	re_print=Yes;
}


//���񿡼� �Ƚø� �����.
bbs_dansi(int num)
{
	int i;
	BBS bbs2;
	FILE *fp;

	if(bbs_num(num)==No) {
		printf("�ش�Ǵ� ��ȣ�� �ڷḦ ã���� �����ϴ�.");
		pressenter();
		return;
	}

	if(check_del()) {	  //���������� �����ΰ�?
		printf("\n ���� ����:%s\n",bbs.title);
		del_esc(bbs.title);
		del_esc(bbs.filename);
		del_esc(bbs.id);
		del_esc(bbs.name);
		del_esc(bbs.key);
		printf(" ���� ����:%s\n",bbs.title);
		if (yesno("��������� �����ұ��?",Yes)) {
			c_lock(1,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
			fp=fopen(NOW_FILE,"r+");
			if (fp!=NULL) {
				i=0;
				while (fread(&bbs2,sizeof_bbs,1,fp)) {
					if (bbs2.num==bbs.num) {
						fseek(fp,i*sizeof_bbs,SEEK_SET);
						if (!feof(fp )) fwrite(&bbs,sizeof_bbs,1,fp);
						break;
					}
					i++;
				}
				fclose(fp);
			}
			c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
		}
	}
	re_print=Yes;
}


//��ü���� ���񿡼� �Ƚø� �����.
bbs_dansiall()
{
	int i,total;
	BBS bbs2;
	FILE *fp;

	printf ("\n %s ȭ���� �Ƚø� ���� �����ұ��?",NOW_FILE);
	if (yesno("(����:Y ���:N)",No)) {
		if(check_del()) {	  //���������� �����ΰ�?
			c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
			fp=fopen(NOW_FILE,"r+");
			if (fp!=NULL) {
				i=0;
				fseekend(fp);
				total=ftell(fp)/sizeof_bbs;
				for (i=0;i<total;i++) {
					fseek(fp,i*sizeof_bbs,SEEK_SET);
					if (fread(&bbs2,sizeof_bbs,1,fp)<1) break;
					del_esc(bbs2.title);
					del_esc(bbs2.filename);
					del_esc(bbs2.id);
					del_esc(bbs2.name);
					del_esc(bbs2.key);
					fseek(fp,i*sizeof_bbs,SEEK_SET);
					if (!feof(fp )) fwrite(&bbs2,sizeof_bbs,1,fp);
					printf(".");
				}
				fclose(fp);
			}
			c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
		}
	}
	re_print=Yes;
}




//�ڷ�ǿ��� ȭ���� �ٷ� �����ش�.
bbs_type()	  //�ش��ȣ �������
{
	char filename[200];
	re_print=Yes;
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	if (mmn[1]==0) return;
	if (!bbs_num(mmn[1])) {
		printf("\n ### %d �� �ڷ�� �����ϴ�. ###\n",mmn[1]);
		pressenter();
		return;
	}
	if (check_bbspasswd()==No) return;
	if (!is_typepds) {
		printf("\n �ڷ�ǿ��� ȭ���� ������ �ٷ� �����ִ� ����Դϴ�.\n");
		pressenter();
		return;
	}
	sprintf(filename,"%s/%s",go.dir,bbs.filename);
    if (bbs.filename[0]==0||!is_file(filename)) return;
	if (stristr(filename,".doc")||
		stristr(filename,".txt")||
		stristr(filename,".cap")||
		stristr(filename,".bat")||
		stristr(filename,".scf")) {
		printf("\n %s %s\n",bbs.filename,bbs.title);
		printfL2();
		view_text(filename);
		printf("\n");
		printfL2();
		pressenter();
		return;
	}
	printf("\n ������ ���� ���� ȭ���Դϴ�.\n");
	pressenter();
	return;
}



typedef struct {
	char  id[9];
	time_t date;
	char  mode;
	int   num;
} OKREC;


bbs_ok(int mode)    //ok ó��  0:�ݴ� 1:���� 2:���� 3:���� 4:���  6:  7:  8:
{
	FILE *fp,*fp2;
	int key=No;
    int num,i,n,n2,count,i1,i2,i3,mode2;
    char buff[200];
    char buff2[200];
	char filename[300];
	BBS bbs1;
	OKREC okrec;
	re_print=Yes;
	printf("\n");

	if (!is_typebbs&&(mode==2||mode==3||mode==4||mode==12)) {
		message("�Խ���������");
		return;
	}
	strcpy(buff2,"");
		 if (mode==0) strcpy(buff2,"NO(�ݴ�)");
	else if (mode==1) strcpy(buff2,"OK(����)");
	else if (mode==2) strcpy(buff2,"DE(����)");
	else if (mode==3) strcpy(buff2,"KE(����)");
	else if (mode==4) strcpy(buff2,"PA(���)");

	if (bbs_ltmode||guest_mode) {
		message("���� ���������� �ȵ˴ϴ�.");
		return;
	}
	if (mmn[0]==0) mmn[0]=mmn[1];	  //�׳� ok�� ������
	if (list_mode&&mmn[0]==0) mmn[0]=NOW_NO;	 //�׳� ok�� ������
    mode2=mmn[0];
    if (mode<5&&mmn[0]<=0) {
		printf("\n OK [������ȣ]  NO [�ݴ��ȣ]  DE [������ȣ]  KE [������ȣ]  PA [������ȣ]\n");
		pressenter();
		return;
	}
	if (mode==11||(mode==12&&is_typebbs)) {
		for (i=0;i<101;i++) {
			if (mmn[i]>0) {
				if (bbs_num(mmn[i])==Yes){	//�ڷ��ȣ ã��
					//�Խù� �Ѽ�
					c_lock(1,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
					if ((fp=fopen(NOW_FILE,"r+"))!=NULL) {      //����ȭ��
						fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
							 if (mode==11) bbs.ok=0;
						else if (mode==12) bbs.down=0;
						if (!feof(fp )) fwrite(&bbs,sizeof_bbs,1,fp);
						fclose(fp);
					}
					c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
				}
			}
		}
		return;
	}


	if (mode==6||mode==7) {
		n=now_month();
		if (mode==7) {
			n--;if (n<1) n+=12;
		}
		get_okrec_filename(filename,n);
		i1=i2=i3=0;
        if ((fp2=fopen(TMP_FILE,"w+"))==NULL) return;
        if ((fp=fopen(filename,"r"))==NULL) {fclose(fp2);return;}
        while (fread(&okrec,sizeof(okrec),1,fp)) {
            if (mode2==0||(mode2>0&&mode2==okrec.num)) {
                mode2=mmn[0];
                i1++;
                if (okrec.mode=='1') i2++;
                else                 i3++;
                fprintf(fp2,"%s %6d %-8s %s   OK:%d NO:%d ALL:%d\n",datestr(3,okrec.date),okrec.num,okrec.id,okrec.mode=='1'?"OK":"NO",i2,i3,i1);
            }
        }
        fclose(fp);
        fclose(fp2);
		view_text(TMP_FILE);
		unlink(TMP_FILE);
		return;
	}

	if (mode==8) {
		n=now_month();
		if (yesno("\n   �̹����� ����/�ݴ� �ڷḦ �����ϰڽ��ϱ�? (Y/n) >>")) {
			get_okrec_filename(filename,n);
			unlink(filename);
			printf(" ���� �Ǿ����ϴ�. \n");
		}
		else printf(" ��� �Ǿ����ϴ�. \n");
		pressenter();
		return;
	}


	//���� ȭ�Ͽ��� �˻�
	n=now_month()-2;
	if (n<1) n+=12;
	get_okrec_filename(filename,n);
    if (is_file(filename)) {
		sprintf(filename,"rm -f %s/bbsdata/okrec/%s.*",club_go.dir,go.code);
		sys_tem(filename);
	}

	get_okrec_filename(filename,now_month());

	if ((fp=fopen(filename,"r+"))!=NULL) {
		while (fread(&okrec,sizeof(okrec),1,fp)) {
			if (is_same(okrec.id,pf.id)) {
				for (i=0;i<101;i++) if (mmn[i]>0&&okrec.num==mmn[i]) mmn[i]=0;
			}
		}
		fclose(fp);
	}

	count=0;
	num=mmn[0];
	fp2=fopen(filename,"r+");
    if (fp2==NULL) {
        sprintf(buff,"%s/bbsdata/okrec/",club_go.dir);check_dir(buff);
        fp2=fopen(filename,"w+");
    }
    if (fp2==NULL) return;
	if (mmn[0]==mmn[1]) mmn[1]=0;

	for (n=0;n<101;n++) {
		if (mmn[n]>0&&bbs_num(mmn[n])){  //�ڷ��ȣ ã��
			//�Խù� �Ѽ�
			if ((fp=fopen(NOW_FILE,"r+"))==NULL) continue;     //����ȭ��
			fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
			fread(&bbs1,sizeof_bbs,1,fp);
			bbs1.ok+=1000000;
			okrec.mode='0';
			bbs1.ok+=1000000;
				 if (mode==0) {bbs1.ok+=1000		;okrec.mode='0';}
			else if (mode==1) {bbs1.ok++			;okrec.mode='1';}
			else if (mode==2) {bbs1.down++			;okrec.mode='2';}       //de
			else if (mode==3) {bbs1.down+=1000		;okrec.mode='3';}       //ke
			else if (mode==4) {bbs1.down+=1000000	;okrec.mode='4';}       //pe
			else			  { 					;okrec.mode='9';}
			c_lock(1,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
			fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
			if (!feof(fp )) fwrite(&bbs1,sizeof_bbs,1,fp);
			fclose(fp);
			c_lock(0,NOW_FILE); 				  //LOCKȮ��  1:Ȯ�� 0:����
			sprintf(buff,"%d �� �ڷῡ [%s] ������ �Ǿ����ϴ�.",mmn[n],buff2);
			printf("  %s\n",buff);
			strcpy(okrec.id,pf.id);
			okrec.num=mmn[n];
			time(&okrec.date);
            if (fp2!=NULL&&okrec.num>0) {
                fseekend(fp2);
                n2=ftell(fp2)/sizeof(okrec);
                if (n2<0||n2>99999) n2=0;
                fseek(fp2,n2*sizeof(okrec),SEEK_SET);
                fwrite(&okrec,sizeof(okrec),1,fp2);
            }
			count++;
		}
	}
	if (fp2!=NULL) fclose(fp2);
	if (count==0) printf(" ### �̹� OK/NO ������ �Ͽ����ϴ�. ###");
	pressenter();
}



get_okrec_filename(char *filename,int n)
{
	sprintf(filename,"%s/bbsdata/okrec/%s.%d",club_go.dir,go.code,n);
}





read_bbsgood_color()
{
	FILE *fp;
	int i,i2;
	char buff[300];
	if (club_mode) sprintf(buff,"%s/system/bbsgood.goo",club_go.dir);
    if (!club_mode||!is_file(buff)) sprintf(buff,GOOD_FILE);
	//��õ
	if ((fp=fopen(buff,"r+"))!=NULL) {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				fread(&agood[i][i2],sizeof_agood,1,fp);
			}
		}
		fclose(fp);
	}
	else {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				agood[i][i2].mark=0;
				agood[i][i2].count=0;
			}
		}
	}
	//�����б�
	strchng2(buff,".goo",".gol");
	if ((fp=fopen(buff,"r+"))!=NULL) {
		for(i=0;i<10;i++) {
			fread(&acolor[i],sizeof_acolor,1,fp);
		}
		fclose(fp);
	}
	else {
		for(i=0;i<10;i++) {
			acolor[i].mark=0;
			acolor[i].color_char=0;
			acolor[i].color_back=0;
			strcpy(acolor[i].name,"");
		}
	}
	return;
}


check_bbs()  //�ڷ��� ������ �ľ��ϴ� ��ƾ
{

	FILE *fp;
    int ii,n,count=0;
	NOW_REC=0			;	   // �����ڷ��ȣ
	NOW_TOPREC=0		;	   // ����ȭ�� ��ܹ�ȣ
	NOW_BOTREC=0		;	   // ����ȭ�� ��ܹ�ȣ
	NOW_ALLREC=0		;	   // �������ڷ��ȣ
	NOW_NO=0			;	   // �����ڷ��ȣ
	NOW_ALLNO=0 		;	   // �������ڷ��ȣ
	NOW_TOPNO=0 		;	   // ����ȭ���� ��ȣ
	//ȭ���� ������ ���� ����
	if (bbs_ltmode) fp=fopen(LT_FILE,"r+");
	else			fp=fopen(NOW_FILE,"r+");
	if( fp==NULL ) return;
	//�Խù� �Ѽ�
	fseekend(fp);
	n=ftell(fp);
	NOW_ALLREC=n/sizeof_bbs;
	if (NOW_ALLREC>0) {
		ii=NOW_ALLREC;
        while (ii>0) {
			fseek(fp,(ii-1)*sizeof_bbs,SEEK_SET);
            if (count++>5000) goto _break;
			if (!fread(&bbs,sizeof_bbs,1,fp)) break;
				if (bbs.del&&!dd_mode) {ii--;continue;}
                if (bbs.filesize==0&&!bit(cfg.bbsmode,5)&&!bit(bcfg.modelist,19)&&!dd_mode&&is_pds) {ii--;continue;}
				if (bbs.num<0) bbs.num=0;
                _break:
				NOW_TOPREC=NOW_ALLREC;
				NOW_REC=NOW_ALLREC-1;
				NOW_ALLNO=bbs.num;
				NOW_TOPNO=bbs.num;
				NOW_NO=bbs.num;
				break;
		}
	}
	fclose(fp);
}

//�Խ��� ��ȣ�� ã��
view_pfb()
{
	BBS oldbbs;
	if (is_char("ok",go.type)||!is_bbspds) return;
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	oldbbs=bbs;
    if (bbs_num(mmn[1])) view_pf(bbs.id);
	bbs=oldbbs;
}





/*

 [ PUT ]	  �⺻��ƿ��Ƽ(���:)					   452/466		 (��405��)
��������������������������������������������������������������������������������
 ��  ��:dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
 ȭ�ϸ�:dddddd		   ũ��:30615		 UP:06/15  DN:98/09/08 13:33  �����:xxx
 ������������������������������������������������������������������������������

 ��  ��:1234567890123456789012345678901234567890
��������������������������������������������������������������������������������
 ��  ��:[GREEN] �׸�ȣ��Ʈ VER 2.40 			 �з�:COMM	   ����:9	   1/ 6
 ȭ�ϸ�:grn240.lzh	   ũ��:380786	 UP:98/08/31 DN:98/09/08   �����:ct0052
 ������������������������������������������������������������������������������
 (c) GREEN SOFT

��������������������������������������������������������������������������������
 ��  ��:dddddddddddddddddddddddddddddddddddddddd �з�:put	   ����:   3   1/1
 ȭ�ϸ�:dddddd		   ũ��:30615	   UP:98/06/15	DN:98/09/08   �����:xxxx
 ������������������������������������������������������������������������������
*/

