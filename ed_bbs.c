////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
//	  �Խ��� ���� ��ƾ
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//�Խ��� ����
//�Է� : ������ �Խù� ��ȣ
//��� : NOW_FILE�� �����Ѵ�.

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	char tcode[59];
	char ccode[59];
	char id[9];
	char tclub[100];
    BBS bbs2;
	int num;
	int mode;
    int found;

    if (is_same(argv[1],"makenews")) {
        make_news(argv[2]);
        return;
    }

	mode=atoi(argv[1]);
	strcpy(NOW_FILE,argv[2]);			   //�����ڵ�
	strcpy(NOW_FILED,NOW_FILE);
	strchng(NOW_FILED,NOW_FILE,".tit",".txt");
    if (mode==999) {  //�Խù� �������
        NOW_REC=atoi(argv[5]);
        //BBS �Ķ��Ÿ�б�
        if ((fp=fopen(argv[3],"r+"))==NULL) return;
        fread(&bbs,sizeof_bbs,1,fp);
        fclose(fp);
        //NOW_FILE Ȯ�� ��˻�
        if ((fp=fopen(NOW_FILE,"r+"))==NULL) return;
//        printf("*%s*%s*%d*%d*",NOW_FILE,argv[4],NOW_REC,bbs.num);pressenter();;;;;;;;;;;;;;;;
        fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
        fread(&bbs2,sizeof_bbs,1,fp);
        found=Yes;
        if (bbs2.num!=bbs.num) {   //�ٸ���?
            found=No;
            printf("\n �Խù���ȣ%d ��˻�\n",bbs.num);
            NOW_REC=0;
            rewind(fp);
            while(fread(&bbs2,sizeof_bbs,1,fp)) {
                if (bbs2.num==bbs.num) {
                    found=Yes;
                    break;
                }
                NOW_REC++;
            }
        }
        fclose(fp);
        if (!found) {
            mess("%d ã���� ����",bbs.num);
            return;
        }
//        printf("->*%s*%s*%d*%d*",argv[3],argv[4],NOW_REC,bbs.num);pressenter();;;;;;;;;;;;;;;;
        c_lock(2,NOW_FILE);
        ed_bbs_text(argv[4]);
        c_lock(0,NOW_FILE);
        return;
    }

	strcpy(id,argv[3]);
	NOW_REC=atoi(argv[4]);
	strcpy(tclub,argv[5]);
	strcpy(tcode,argv[6]);
	findport(tty);
	read_cfg();
	set_tmpfile();
	unlink(TMP_FILE);
	unlink(TMP_FILE2);
    if (!read_pf(id)) {
        if (mode==0) {
            mess("%s ID�� ã���� �����ϴ�.",id);
            return;
        }
    }
	find_club(tclub);
	find_go(tcode);

    if ((fp=fopen(NOW_FILE,"r+"))==NULL) {
        mess("%s ������ ã���� �����ϴ�.",NOW_FILE);
        return;
    }
	NOW_ALLREC=count_frec(fp,sizeof_bbs);
	if (NOW_REC<0||NOW_REC>NOW_ALLREC) {fclose(fp);return;}

	fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
	if (feof(fp)||!fread(&bbs,sizeof_bbs,1,fp)) return;
	fclose(fp);
	if (bbs.size>1000000) bbs.size=1000000;
	if (bbs.posi<1) bbs.posi=0;

	if (mode==1) {
		c_lock(1,NOW_FILE);
		replace_text(argv[7],atoi(argv[8]));
		c_lock(0,NOW_FILE);
	}
    else if (mode==2) bbs_edit(2);
    else              bbs_edit(0);
}


bbs_edit(int cmd)
{
	char buff[280],buff1[280],buff2[280];
	char filename[200];
	int i,n,mode,n1,n2,n3,nn,num;
	BBS bbs2;
	FILE *fp;
	int title_chage=No;
	int text_chage=No;
	int file_chage=No;

	mode=0;
	num=bbs.num;
	while(1) {
        if (cmd!=0) nn=cmd;
        else {
            cls();
            printf(" ### %s(%s)  NO:%d REC:%d AREC:%d\n",go.code,go.name,bbs.num,NOW_REC,NOW_ALLREC);
            printf(" 1. ��  ��  : %s\n",bbs.title);
            printf(" 2. Ű����  : %s\n",bbs.key);
            printf(" 3. ȭ�ϸ�  : %s (%d byte)\n",bbs.filename,bbs.filesize);
            printf(" 4. ��  ȣ  : %s\n",bbs.passwd);
            printf(" 5. �������: %d byte\n",bbs.size);
            printf(" 6. ���� ������->�ϼ��� �ѱ��ڵ庯ȯ\n");
            if (pf.type>=17&&bit(cfg.bbsmode,3)) {
            printf("21. D / R   : %d / %d \n",bbs.down,bbs.read);
            printf("22. Ok/No/An: %d / %d /%d\n",bbs.ok%1000,(bbs.ok/1000)%1000,bbs.ok/1000000);
            printf("23. ��ϳ�¥: %s\n",datestr(3,bbs.date));
            printf("24. ��ȸ��¥: %s\n",datestr(3,bbs.date2));
            printf("25. �ϰ�����: \n");
            printf("26. ���̵�  : %s\n",bbs.id);
            printf("27. ��  ��  : %s\n",bbs.name);
            printf("28. tcode   : %s\n",bbs.tcode);
            }
            if (pf.type>=18&&bit(cfg.bbsmode,4)) {
            printf("91. bbsposi : %d\n",bbs.posi);
            printf("92. bbssize : %d\n",bbs.size);
            }
            printf(" �Ϸ�(P) ���(0) ����>>");
            lineinput(buff2,4);check_han(buff2);nn=atoi(buff2);printf("\n");
            if (strlen(buff2)==1&&is_char("0",buff2[0])) {
                title_chage=No;
                break;
            }
            if (strlen(buff2)==1&&is_char("0QqPpXxmM",buff2[0])) break;
        }
		if (nn==1) {
			printf("��  �� :");
			strcpy(buff,bbs.title);
			dosinput(bbs.title,60);
			del_esc(bbs.title);
			title_chage=!is_same(bbs.title,buff);
		}
		else if (nn==2) {
			printf("Ű���� :");
			strcpy(buff,bbs.key);
			dosinput(bbs.key,8);
			del_esc(bbs.key);
			title_chage=!is_same(bbs.key,buff);
		}
		else if ((pf.type>17||(club_mode&&now_type>16))&&nn==3) {
			printf("              +---------+----\n");
			printf("����ȭ�ϸ�   :%s  (ȭ��ũ��:%dByte)\n",bbs.filename,bbs.filesize);
			printf("              +---------+----\n");
			printf("��ȭ�ϸ�     :");
			lineinput(buff,12);
			if (buff[0]!=0) {
				del_esc(buff);
				del_space(buff);
				if(good_filename(buff)) {
					sprintf(filename,"%s/%s",go.dir,bbs.filename);
					del_space(filename);del_esc(filename);
                    if (is_typepds&&strlen(filename)>2&&strlen(bbs.filename)>0&&is_file(filename)) {
						printf("\n %s ȭ���� �����մϴ�. ���� �ٲܱ��? (Y/n)",filename);
						if (yesno("",Yes)) {
							sprintf(buff2,"cp %s %s/%s -f",filename,go.dir,buff);
							sys_tem(buff2);
							sprintf(buff2,"rm %s -f",filename);
							sys_tem(buff2);
						}
					}
					strcpy(bbs.filename,buff);
					sprintf(filename,"%s/%s",go.dir,bbs.filename);
					bbs.filesize=file_size(filename);
					title_chage=Yes;
				}
			}
		}
        else if (nn==4) {
			printf("              +---------+----\n");
			printf("�űԾ�ȣ(4��):");
			lineinput(buff,4);
			if (buff[0]!=0) {
				del_esc(buff);
				del_space(buff);
                strcpy(bbs.passwd,buff);
				title_chage=Yes;
			}
		}
        else if (nn==5) {
			c_lock(1,NOW_FILE);
			text_chage=bbs_edit_text();
			c_lock(0,NOW_FILE);
			title_chage=No;
		}
        else if (nn==6) bbs_joks(num);
        else if (pf.type>18&&nn==91&&bit(cfg.bbsmode,4)) {
			printf("POSI:(%d)  ����  :",bbs.posi);
			n=get_num(bbs.posi,10);
			if (n!=bbs.posi&&n>=0) {
				bbs.posi=n;
				title_chage=Yes;
			}
		}
        else if (pf.type>18&&nn==92&&bit(cfg.bbsmode,4)) {
			printf("SIZE:(%d)  ����  :",bbs.size);
			n=get_num(bbs.size,10);
			if (n!=bbs.size&&n>=0) {
				bbs.size=n;
				title_chage=Yes;
			}
		}
		else if (pf.type>=18&&nn==99) {
			text_chage=bbs_edit_text();
			title_chage=No;
		}
        else if (pf.type>18&&nn==21&&bit(cfg.bbsmode,3)) {
			printf(" �ٿ�� : %d   >>",bbs.down);bbs.down=get_num(bbs.down,5);printf("\n");
            printf(" ��ȸ�� : %d   >>",bbs.read);bbs.read=get_num(bbs.read,5);printf("\n");
			title_chage=Yes;
		}
        else if (pf.type>18&&nn==22&&bit(cfg.bbsmode,3)) {
			n1=bbs.ok%1000;
			n2=(bbs.ok/1000)%1000;
			n3=bbs.ok/1000000;
			printf(" OK     : %d   >>",n1);n1=get_num(n1,3);printf("\n");
			printf(" No     : %d   >>",n2);n2=get_num(n2,3);printf("\n");
			printf(" An     : %d   >>",n3);n3=get_num(n3,3);printf("\n");
			bbs.ok=n3*1000000+n2*1000+n1;
			title_chage=Yes;
		}
        else if (pf.type>18&&nn==23&&bit(cfg.bbsmode,3)) {
			printf(" ��ϳ�¥ : %s >>",datestr(3,bbs.date));
			get_str(buff,20);printf("\n");
			bbs.date=str2date2(buff);
        }
        else if (pf.type>18&&nn==24&&bit(cfg.bbsmode,3)) {
            printf(" ��ȸ��¥ : %s >>",datestr(3,bbs.date2));
			get_str(buff,20);printf("\n");
			bbs.date2=str2date2(buff);
			title_chage=Yes;
		}
        else if (pf.type>=18&&nn==25&&bit(cfg.bbsmode,3)) {
			if ((fp=fopen(TMP_FILE,"w+"))!=NULL) {
				fprintf(fp,"%s\n",bbs.title);
				fprintf(fp,"%s\n",bbs.key);
				fprintf(fp,"%s\n",bbs.filename);
				fprintf(fp,"%s\n",bbs.id);
				fprintf(fp,"%s\n",bbs.name);
                fprintf(fp,"%s\n",bbs.passwd);
				fclose(fp);
			}
			xedit(TMP_FILE);
			if ((fp=fopen(TMP_FILE,"r"))!=NULL) {
				if (fgets(buff,200,fp)) {del_enter(buff);all_trim(buff);xstrncpy(bbs.title	,buff,60);}
				if (fgets(buff,200,fp)) {del_enter(buff);all_trim(buff);xstrncpy(bbs.key 	,buff,8 );}
				if (fgets(buff,200,fp)) {del_enter(buff);all_trim(buff);xstrncpy(bbs.filename,buff,12);}
				if (fgets(buff,200,fp)) {del_enter(buff);all_trim(buff);xstrncpy(bbs.id		,buff,8 );}
				if (fgets(buff,200,fp)) {del_enter(buff);all_trim(buff);xstrncpy(bbs.name	,buff,8 );}
                if (fgets(buff,200,fp)) {del_enter(buff);all_trim(buff);xstrncpy(bbs.passwd ,buff,4 );}
			}
			title_chage=Yes;
		}
        else if (pf.type>=17&&nn==26&&bit(cfg.bbsmode,3)) {
			printf("ID     :");
			strcpy(buff,bbs.id);
			dosinput(bbs.id,8);
			del_esc(bbs.id);
			title_chage=!is_same(bbs.id,buff);
		}
        else if (pf.type>=17&&nn==27&&bit(cfg.bbsmode,3)) {
			printf("�̸�   :");
			strcpy(buff,bbs.name);
			dosinput(bbs.name,8);
			del_esc(bbs.name);
			title_chage=!is_same(bbs.name,buff);
		}
        else if (pf.type>=17&&nn==28&&bit(cfg.bbsmode,3)) {
            printf("TCODE   :");
            dosinput(bbs.tcode,8);
            title_chage=Yes;
		}
        if (cmd>0) break;
	}
	if (title_chage) {
		c_lock(1,NOW_FILE);
		if ((fp=fopen(NOW_FILE,"r+"))!=NULL) {
			fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
			fread(&bbs2,sizeof_bbs,1,fp);
			if (bbs2.num==bbs.num) {
				fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp);
			}
			fclose(fp);
		}
		c_lock(0,NOW_FILE);
	}
	if (is_file(TMP_FILE))	unlink(TMP_FILE);
	if (is_file(TMP_FILE2)) unlink(TMP_FILE2);
}


bbs_edit_text()
{
	FILE *fp1,*fp2,*fp3;
	char buff[280],buff1[280];
	int i,n,key,ret=No;
	check_dir("tmp");
	if (!is_file(NOW_FILED)) touch_file(NOW_FILED);
	//������ �ӽ�ȭ�Ϸ� ����
	bbs2tmp();	  //����BBS�� ������ TMP�� ����
    file_copy(TMP_FILE,TMP_FILE2);

	strcpy(g_buff4,pf.type>=18?"99.UPS":"");
	view("frm/editor/select.%s",bbs_tail());
	lineinput(buff,10);check_han(buff);printf("\n");
	n=atoi(buff);
    if (is_char("qp0QPxX",buff[0])) return;
         if (n== 3) ret=up_text(1,TMP_FILE);  //�ٿ�޾Ƽ�����ȭ�Ϸ� �ٲ۴�.
	else if (n== 4) ret=up_text(2,TMP_FILE);  //�ٿ�޾Ƽ�����ȭ�Ϸ� �ٲ۴�.
	else if (n==99) ret=up_text(99,TMP_FILE);
	else			ret=xedit(TMP_FILE);
    if (ret==No||!is_file(TMP_FILE)) return;
    if (bbs.size!=file_size(TMP_FILE2)||!is_samefile(TMP_FILE,TMP_FILE2))
        ed_bbs_text(TMP_FILE);
    unlink(TMP_FILE);
    unlink(TMP_FILE2);
}


//mode2=0:���� 1:ù��->�������� 2:��°��->���ϸ�
replace_text(char *filename,int mode2)
{
	FILE *fp1,*fp2,*fp3;
	char buff[280],buff1[280];
	char TMP_FILE3[200];
	int i,n,key,ret=No;
	int skip_line=0;
	BBS oldbbs;
	BBS bbs2;
	int change_size,start,total;
	char c;
	char change;

	bcfg.automode=mode2;

	check_dir("tmp");
    sprintf(TMP_FILE3,"tmp/tmp4%d%s",cfg.hostno,no_spc(tty)); // �ӽ�ȭ��
	oldbbs=bbs;
	if (!is_file(NOW_FILED)) touch_file(NOW_FILED);

	//1. ������ ������ �ӽ�ȭ�Ϸ� ���� (������)
	fp1=fopen(NOW_FILED,"r");
	fp2=fopen(TMP_FILE2,"w");
	if (fp1!=NULL&&fp2!=NULL) {
		for (i=0;i<bbs.posi;i++) {
			c=fgetc(fp1);
            fputc(c,fp2);
		}
	}
	if (fp1!=NULL) fclose(fp1);

	//2. ���� ����� ȭ���� ����
	fp1=fopen(filename,"r");
	if (fp1==NULL) {
		if (fp1!=NULL) fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
		return No;
	}
	rewind(fp1);
	//����ù���� �ڵ����� ����,���ϸ����� ��
	if (bit(bcfg.automode,2)&&fgets(buff,200,fp1)) {
		del_enter(buff);
		all_trim(buff);
		if (bit(bcfg.automode,4)) skip_line++;
		if (strlen(buff)>0) {
			xstrncpy(bbs.title,buff,60);
		}
	}
	if (bit(bcfg.automode,3)&&fgets(buff,200,fp1)) {
		del_enter(buff);
		del_space(buff);
		if (bit(bcfg.automode,4)) skip_line++;
		if (strlen(buff)>0&&strlen(buff)<13) {
			xstrncpy(bbs.filename,buff,12);
		}
	}
	rewind(fp1);
	while(skip_line>0&&!feof(fp1)) {
		c=fgetc(fp1);
		if (c==0) break;
		if (c=='\n') {
			if (skip_line==1) break;
			else skip_line--;
		}
	}
	bbs.size=0; 		 //�����ۼ��� �Խù��� ����
	bbs.line=0;
	while(!feof(fp1)) {
		c=fgetc(fp1);
		if (c==0) break;
		fputc(c,fp2);
		bbs.size++;
		if (c=='\n') bbs.line++;
	}
	fclose(fp1);
	bbs.date=file_time(filename);	   //file�� ���������ð�

	//3. ������ȭ���� ���� ����� ȭ���� ����
	fp1=fopen(NOW_FILED,"r+");
	if (fp1!=NULL) {
		fseekend(fp1);
		total=ftell(fp1) ;			//����ȭ���� ������
		start=oldbbs.posi+oldbbs.size;
		fseek(fp1,start,SEEK_SET);
		for (i=start+1;i<=total;i++) {
			c=fgetc(fp1);
			fputc(c,fp2);
		}
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);

	//4. NOW_FILED ����
	file_copy(TMP_FILE2,NOW_FILED);  //   TMP_FILE2->NOW_FILED
	unlink(TMP_FILE);
	unlink(TMP_FILE2);

	//����� ����ȭ���� �ٽ� ����
	fp1=fopen(NOW_FILE,"r+");      //����ȭ��
	if (fp1!=NULL) {
		fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
		fwrite(&bbs,sizeof_bbs,1,fp1);	//���纯��� ũ�� ����
		if (bbs.size<1) bbs.size=0;
		change_size=bbs.size-oldbbs.size;  //����� ũ��

		for(i=NOW_REC+1;i<NOW_ALLREC;i++) {
//			  printf("*%d %d %d %d*",NOW_REC,NOW_ALLREC,i,change_size);
			fseek(fp1,i*sizeof_bbs,SEEK_SET);
			if (fread(&bbs2,sizeof_bbs,1,fp1)) {
				bbs2.posi+=change_size;
				fseek(fp1,i*sizeof_bbs,SEEK_SET);
				fwrite(&bbs2,sizeof_bbs,1,fp1);
			}
		}
		fclose(fp1);
	}
	return Yes;
	_fail:
	unlink(TMP_FILE);
	unlink(TMP_FILE3);
	return No;
}




//������ ���������� �ϼ������� �ٲ۴�.
bbs_joks(int num)
{
	FILE *fp1;
	FILE *fp2;
	char buff[80];
	char c;
	int i,i2;

	if (is_char(BBSTYPE,go.type)==No) return No;
	re_print=Yes;
	if (num<1) return No;

	unlink(TMP_FILE);
	unlink(TMP_FILE2);
	//�Խù� �Ѽ�
	if ((fp1=fopen(NOW_FILED,"r+"))!=NULL) {              //����ȭ��
		if ((fp2=fopen(TMP_FILE,"w+"))!=NULL) {                //����ȭ��
			fseek(fp1,bbs.posi,SEEK_SET);
			if (bbs.size<1) bbs.size=0;
			for (i2=0;i2<bbs.size;i2++) {
				c=fgetc(fp1);			  //�ӽÿ��� �о�
				fputc(c,fp2);			  //���뿡 ����
			}
			fclose(fp2);
		}
		fclose(fp1);
	}
	if (is_file(TMP_FILE)) {
		unlink(TMP_FILE2);
        sprintf(buff,"bin/xhcode %s %s",TMP_FILE,TMP_FILE2);sys_tem(buff);
		if (!is_file(TMP_FILE2)) {
            mess("hcode ��ȯ ���� %s",TMP_FILE2);
		}
		else {
			printf("\n");
			printfL2();
			view(TMP_FILE2);
			printfL2();
			if (yesno("\n �ϼ��� ��ȯ�� �� �Ǿ�����? ó��(Y) ���(n) >>",Yes)) {
				fp1=fopen(NOW_FILED,"r+");      //����ȭ��
				fp2=fopen(TMP_FILE2,"r+");      //����ȭ��
				if (fp1!=NULL&&fp2!=NULL) {
					fseek(fp1,bbs.posi,SEEK_SET);
					for (i2=0;i2<bbs.size;i2++) {
						c=fgetc(fp2);			  //�ӽÿ��� �о�
						fputc(c,fp1);			  //���뿡 ����
					}
				}
				if (fp1!=NULL) fclose(fp1);
				if (fp2!=NULL) fclose(fp2);
			}
		}
	}
	return Yes;
}


//������ ����� ������ �����Ѵ�.
ed_bbs_text(char *filename)
{
	FILE *fp1,*fp2,*fp3;
	char buff[280],buff1[280];
    char TMP_FILE4[200];
	int i,n,key,ret=No;
	int skip_line=0;
	BBS oldbbs;
	BBS bbs2;
	int change_size,start,total;
	char c;
	char change;

//    printf("num=%d\n",bbs.num);printf("tit=%s\n",bbs.title);pressenter();
//    view(filename);pressenter();

    sprintf(TMP_FILE4,"tmp/tmp4%d%s",cfg.hostno,no_spc(tty)); // �ӽ�ȭ��
	oldbbs=bbs;

	if (!is_file(NOW_FILED)) touch_file(NOW_FILED);

    //1. ������ ������ �ӽ�ȭ�Ϸ� ���� (������)  ->fp2 : TMP_FILE4
//    view(NOW_FILED);pressenter();;;;;;;;;;;;
    fp1=fopen(NOW_FILED,"r+");
    fp2=fopen(TMP_FILE4,"w+");
    fp3=fopen(filename,"r");
    if (fp1==NULL||fp2==NULL||fp3==NULL) return;
    for (i=0;i<bbs.posi&&!feof(fp1);i++)
        fputc(fgetc(fp1),fp2);
	//2. ���� ����� ȭ���� ����
    bbs.line=0;
    bbs.size=0;
    while(!feof(fp3)) {
        c=fgetc(fp3);
        if (c==0||(byte)c==255) break;
        if (c=='\n') bbs.line++;
        fputc(c,fp2);
        bbs.size++;
    }
    fseek(fp1,oldbbs.posi+oldbbs.size,SEEK_SET);
    while(!feof(fp1))
        fputc(fgetc(fp1),fp2);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
//    view(TMP_FILE4);pressenter();;;;;;;;;;;;

	//4. NOW_FILED ����
    if (!file_copy(TMP_FILE4,NOW_FILED)) return;  //   TMP_FILE2->NOW_FILED
    unlink(TMP_FILE4);

	//����� ����ȭ���� �ٽ� ����
	fp1=fopen(NOW_FILE,"r+");      //����ȭ��
    if (fp1==NULL) return;
    fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
    fwrite(&bbs,sizeof_bbs,1,fp1);  //���纯��� ũ�� ����
    change_size=bbs.size-oldbbs.size;  //����� ũ��
    i=NOW_REC+1;
    while (1) {
        fseek(fp1,i*sizeof_bbs,SEEK_SET);
        if (!fread(&bbs2,sizeof_bbs,1,fp1)) break;
        bbs2.posi+=change_size;
        fseek(fp1,i*sizeof_bbs,SEEK_SET);
        fwrite(&bbs2,sizeof_bbs,1,fp1);
        i++;
    }
    fclose(fp1);
    return;
}


make_news(char *newsfile)
{
	FILE *fp,*fp1,*fp2,*fp3;
	char news_logo[300],num_file[300];
    char buff[400],buff1[400],buff2[400],buff3[400],buff4[400],buff5[400];
	char ch;
    int i,i1,i2,i3,n,n1,n2,count,found,sizeof_fnews=(sizeof_bbs+300+80*4);

    del_space(newsfile);del_esc3(newsfile);
    if (strlen(newsfile)<1) return;
    sprintf(NOW_FILE ,"menu/%s.tit",newsfile);
    sprintf(NOW_FILED,"menu/%s.txt",newsfile);
	//��������� ������ �����Ѵ�.
    sprintf(num_file,"tmp/news/%s.nws",newsfile);
    check_dir("tmp/news/");
    if ((fp=fopen(NOW_FILE,"r"))==NULL) return;
    if ((fp1=fopen(NOW_FILED,"r"))==NULL) return;
    if ((fp2=fopen(num_file,"w"))==NULL) return;
    n=count_frec(fp,sizeof_bbs);
    count=0;
    while (n>0&&count<100) {
        n--;
        fseek(fp,n*sizeof_bbs,SEEK_SET);
        if (!fread(&bbs,sizeof_bbs,1,fp)) break;
        if (cfg.newsoldtime>0&&is_oldtime(bbs.date,cfg.newsoldtime*60*60)) break;     //��ð����� �ð��ΰ�?
        if (bbs.del||bbs.size<1) continue;
        if (bit(cfg.newsmode,7)&&bbs.gd<=' ') continue;

        bzero(buff1,300);bzero(buff2,300);bzero(buff3,300);bzero(buff4,300);bzero(buff5,300);
        fseek(fp1,bbs.posi,SEEK_SET);
        n2=i1=0;
        for (i=0;!feof(fp1)&&i<bbs.size&&n2<=3;i++) {
            ch=fgetc(fp1);
            if (ch==EOF) break;
            if (i<295) buff1[i]=ch;
                 if (ch=='\n'||i1>79) {n2++;i1=0;}
            else if (n2==0) buff2[i1++]=ch;
            else if (n2==1) buff3[i1++]=ch;
            else if (n2==2) buff4[i1++]=ch;
            else if (n2==3) buff5[i1++]=ch;
            else if (n2>=4) break;
        }
        fseek(fp2,count*sizeof_fnews,SEEK_SET);
        fwrite(&bbs,sizeof_bbs,1,fp2);
        fwrite(&buff1,300,1,fp2);
        fwrite(&buff2,80,1,fp2);
        fwrite(&buff3,80,1,fp2);
        fwrite(&buff4,80,1,fp2);
        fwrite(&buff5,80,1,fp2);
        count++;
    }
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
}
