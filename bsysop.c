////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// �Խ��� ������� �޴�
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include <unistd.h>    //���丮 ȭ�ϸ� ��ȯ�Լ������� �ʿ�
#include <dirent.h>    //���丮 ȭ�ϸ� ��ȯ�Լ������� �ʿ�

int asearch0[31];
int asearch1[31];
int asearch2[31];
int asearch3[31];
int asearch4[31];
int n;
int now_local;
int tot[20];

main(int argc, char **argv)
{
	FILE *fp;
	int i,n=0,n1,n2;
	char buff[140];
	char buff2[140];
	char filename[200];
	char tcode[9];
	char ccode[9];
    char id[19];
	char menu_change=No;

	findport(tty); //������Ʈ ã��
	read_cfg();
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�

    if (argc<4) {
        printf("����  bsysop <�����ڵ�> <��ȣȸ�ڵ�> <ID> [���] \n");
        printf("����  bsysop hello game �û�\n");
		getchar();
		return;
	}
    if (is_same(argv[1],"_renum_bbs")) {   //������ȣ����
        strcpy(NOW_FILE,argv[2]);
        strcpy(NOW_FILED,argv[3]);
        go.type='l';
        bbs_renum2();
        return;
    }
    for (i=0;i<20;i++) tot[i]=0;

    strcpy(tcode,argv[1]);                 //�����ڵ�
    strcpy(ccode,argv[2]);                 //��ȣȸ �ڵ�
    strcpy(id,argv[3]);                    //���̵�

	set_nowmenu();		 //����Ÿȭ�������ϱ�
	if (!find_club(ccode)) {
		mess("%s ��ȣȸ �˻� ����",ccode);
		return;
	}
//    printf("%s %s %s",ccode,tcode,NOW_MENU);pressenter();
	set_nowmenu();		 //����Ÿȭ�������ϱ�
	if (!find_go(tcode)) {
		mess("%s �����ڵ� �˻� ����",tcode);
		return;    //ã�� �����ڵ尡 ����
	}

	read_ccfg();
	set_bcfgfile(); 					//BCFG_FILE�� ����
	read_bcfg();
    	if (is_same(id,"all_pass_del")) {all_pass_del();return;}
    	if (is_same(id,"all_mail_del")) {all_mail_del();return;}
	
	if (!read_pf(id)) {
		mess("%s �˻�����",id);
		return;
	}
	now_local=pf.localmenu;
	

	strcpy(NOW_FILE,"");
	strcpy(NOW_FILED,"");
	if (is_typebbs||is_typepds) {
		sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
		sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,go.code);
		del_space(NOW_FILE);
		del_space(NOW_FILED);
		sys_tem("touch %s",NOW_FILE );
		sys_tem("touch %s",NOW_FILED);
	}
	read_cpf(id);					 //�ش�Ǵ� ��ȣȸ�� ���̵� ã�´�.
	read_atype();					 //����б�
	read_actype();					 //��ȣȸ����� �̸��� �о��  ->actype
	who=check_who();				 //1:��ȣȸ ��� 2:��ȣȸ �û�  3:root����

	if (who<1) {
		mess("%d,%d �Խ��Ǹ޴����Ұ��ɵ��",who,pf.type);
		return;
	}

    if (argc>=5) {
        i=atoi(argv[4]);
			 if (i==101) bbs_all_sort();
		else if (i==102) ed_rsize();
		if (i>100) return;
	}


	while (1) {
		cls();
		xy(1,1);
        print_(1," %s(%s)(%c) �޴� (���:%s) LOCAL:%d",go.code,go.name,go.type,bcfg.man1,now_local);
        printf("������������������������������������������������������������������\n");
        printf("��    1. �ý��� �޴�����        ��    31. ��ü�Խù���������    ��\n");
        printf("��    2. �Խ��� ȯ�漳��        ��    32. ��ü�Խù���/�ٿ�     ��\n");
        printf("��    3. �Խ��� ȭ�����        ��    33. ��ü�Խù�����        ��\n");
        printf("��    4. �Խ��� ��������        ��    34. ��ȣ������,��Ʈ       ��\n");
        printf("��    5. ȭ��Խ��� ����        ��    35. �ڷ��ȭ�ϰ���        ��\n");
        printf("��    6. �Խ��Ǳ�� ����        ��    36. �����ڷ��������      ��\n");
        printf("��    7. �Խ��Ǹ�� ����        ��    37. �̵��ȭ���б�        ��\n");
        printf("��    8. �Խ��Ǹ�� ����        ��    38. Ư���Խù�����BDN     ��\n");
        printf("��    9. �ڷ��/�ٿ��        ��                              ��\n");
        printf("��   10. �Խ��� �������        ��    40. ���/��ȸ���Ȯ��     ��\n");
        printf("��   11. ��񼳹�   ����        ��    41. �ڵ�ó�� ����         ��\n");
        printf("��   12. ����/����  ����        ��    42. ��ǰ���� ����         ��\n");
        printf("������������������������������������������������������������������\n");
        printf("��   21. IP      ����IP         ��    51. �Խù��ڵ���ȯ        ��\n");
        printf("��   22. WAR   �������         ��    52. �õ���丮�б�      ��\n");
        printf("��   23. CASH���̹��Ӵ�         ��    53. �������ȸ��Ȳ      ��\n");
        printf("��   24. BILL      ����         ��    54. �ڷ᰹�� ������       ��\n");
        printf("������������������������������������������������������������������\n");
		printf(" ���ù�ȣ? (����:Q �������:N ����:?) >>");
		lineinput(buff,10);check_han(buff);printf("\n");n=atoi(buff);
		if (is_quit(buff)) break;
		if (is_same(buff,"n")) return;
		else if (is_same(buff,"?")) disp_status();
		else if (n== 1) {set_menucfg();menu_change=Yes;}
		else if (n== 2) set_bcfg1();	 //ȯ�漳��
		else if (n== 3) set_blogo();
		else if (n== 4) set_limit();	//ȯ�漳��
		else if (n== 5) set_jpgcfg();	//ȯ�漳��
		else if (n== 6) set_reccfg();	//ȯ�漳��
		else if (n== 7) set_formcfg();	//ȯ�漳��
		else if (n== 8) set_listcfg();
		else if (n== 9) set_updown();
		else if (n==10) set_write();
		else if (n==11) set_quest();
		else if (n==12) set_ok();
		else if (n==21) bset_ipgo();	//���̹� ����
		else if (n==22) bset_wars();	//����
		else if (n==23) bset_cash();	//���̹� ����
		else if (n==24) bset_bill();	//���̹� ����
		else if (n==31) bbs_all_del();
		else if (n==32) bbs_all_down();
		else if (n==33) bbs_all_set();
		else if (n==34) bbs_all_sort();
		else if (n==35) ed_rsize();
		else if (n==36) bbs_pack();
		else if (n==37) up_nofile();
        else if (n==38) sys_tem("bin/bdn %s %s %s",go.code,club_go.code,pf.id);
		else if (n==40) view_iprec();
        else if (n==41) set_auto();
        else if (n==42) set_gift();
		else if (n==51) txt2bbs();
		else if (n==52) read_cdrom();
		else if (n==53) stat_rec();
        else if (n==54) re_bbsnum(3);

	}
	del_space(bcfg.man1);del_space(bcfg.man2);del_space(bcfg.man3);del_space(bcfg.man4);del_space(bcfg.man5);
	save_bcfg();
	if (menu_change) {
		GO go2;
		if ((fp=fopen(NOW_MENU,"r+"))!=NULL) {
			fseek(fp,go.rec*sizeof_go,SEEK_SET);
			fread(&go2,sizeof_go,1,fp);
			if (is_same(go2.code,go.code)) {
				fseek(fp,go.rec*sizeof_go,SEEK_SET);
				fwrite(&go,sizeof_go,1,fp);
			}
			fclose(fp);
			reset_menuind();	 //�޴�SUB TAG ������
		}
	}
}

re_count()
{
    FILE *fp;
    int i;
    char brecfilename[200];
    for (i=0;i<20;i++) tot[i]=0;
    fp=fopen(NOW_FILE,"r");
	while (fread(&bbs,sizeof_bbs,1,fp)) {
        if (bbs.del) {
            tot[2]++;
            continue;
        }
        tot[1]++;
        if (bbs.gd!=' ') tot[3]++;
        tot[4]+=bbs.read;
        tot[5]+=bbs.down;
        tot[6]+=bbs.line/20+1;
        if (strlen(bbs.filename)>0) tot[10]+=bbs.filesize;
        i=time_day(bbs.date);
        if (i==0) tot[7]++;
        else if (i<=7) tot[8]++;
        else if (i<=30) tot[9]++;
	}
	fclose(fp);
    tot[0]=count_rec(NOW_FILE,sizeof_bbs);
    sprintf(brecfilename,"%s/menurec/%s.bre",club_go.dir,go.code);
    if ((fp=fopen(brecfilename,"r"))!=NULL) {
		fread(&brec,sizeof_brec,1,fp);
		fclose(fp);
	}
}


bbs_all_sort()
{
	FILE *fp1;
	FILE *fp2;
	FILE *fp3;
	FILE *fp4;
	FILE *fp5;
	int mode,i,num,new_posi,new_num;
	int n,n1,n2;
	char buff[200];
	char buff2[200];
	char dir[200];
	char tclub[100];
	char gocode1[100];
	char gocode2[100];
	char filename1[200];
	char filename2[200];
	char filename3[200];
	char filename4[200];
	char SORT_FILE[200];
	int order,renum;

	order=renum=No;

	while (1) {
		cls();
		print_(1,"�ڷ� ����");
		printf("  1. ��ȣ ������\n");
		printf("  2. ��¥ �� ����   %s\n",order?"(��������)":"(��������)");
		printf("  3. ���� �� ����   %s\n",order?"(��������)":"(��������)");
		printf("  4. ID   �� ����   %s\n",order?"(��������)":"(��������)");
		printf("  5. KEY  �� ����   %s\n",order?"(��������)":"(��������)");
		printf("  6. TCODE�� ����   %s\n",order?"(��������)":"(��������)");
		printf("  7. ȭ�ϸ������   %s\n",order?"(��������)":"(��������)");
		printf("  8. ����ũ���     %s\n",order?"(��������)":"(��������)");
		printf("  9. ȭ��ũ���     %s\n",order?"(��������)":"(��������)");
		printf(" 10. ȭ�ϴٿ��     %s\n",order?"(��������)":"(��������)");
		printf(" 11. �Խù��б��   %s\n",order?"(��������)":"(��������)");
		printf(" 12. �Խù��ټ���   %s\n",order?"(��������)":"(��������)");
		printf(" 13. ������ȣ  ��   %s\n",order?"(��������)":"(��������)");
		printf(" 21. ���Ĺ��  ���� %s\n",order?"(��������)":"(��������)");
		printf(" 22. ��ȣ���������� %s\n",renum?"( ������ )":"(������ȣ)");
		printf(" ���� >>");
		mode=numinput();
			 if (mode==21) {order=order?0:1;continue;}
		else if (mode==22) {renum=renum?0:1;continue;}
		else if (mode== 1) {
			if (yesno("�Խ��� �ڷ��� ��ȣ�� ������ �Ͻðڽ��ϱ�? (Y/n) >>",Yes))
                bbs_renum2();
			continue;
		}
		else if (mode< 1||mode>19) break;

		set_tmpfile();		 //�ӽ�ȭ�������ϱ�
		set_nowmenu();		//����ȭ�ϸ� ����
		sprintf(filename1,NOW_FILE );
		sprintf(filename2,NOW_FILED);
		sprintf(filename3,TMP_FILE );
		sprintf(filename4,TMP_FILE2);
        sprintf(SORT_FILE,"tmp/tmp8%d%s.tmp",cfg.hostno,no_spc(tty));

		printf("\n  %s(%s) �� �������մϴ�.\n ",filename1,go.name);
		printf(" ������ �Ǹ� ������ �ڷ�� ������ �����˴ϴ�.\n");
		if (yesno("  ������ �����ұ��?  (Y/n)",Yes)==No) continue;
		printf("\n");

		fp1=fopen(filename1,"r");
		if (fp1==NULL) {
			mess(" %s ȭ���� �����ϴ�.",filename1);
			continue;
		}
		fseek(fp1,0,SEEK_SET);
		if ((fp5=fopen(TMP_FILE,"w"))==NULL) {
			mess("%s ���� ���� ����",TMP_FILE);
			continue;
		}
		while (fread(&bbs,sizeof_bbs,1,fp1)) {
			num=ftell(fp1)/sizeof_bbs;
			if (bbs.del==No) {
				if		(mode== 2) fprintf(fp5,"%15d %d\n",bbs.date,num);
				else if (mode== 3) {del_space(bbs.title);fprintf(fp5,"%-60.60s %d\n",bbs.title,num);}
				else if (mode== 4) fprintf(fp5,"%15s %d\n",bbs.id,num);
				else if (mode== 5) fprintf(fp5,"%15s %d\n",bbs.key,num);
				else if (mode== 6) fprintf(fp5,"%15s %d\n",bbs.tcode,num);
				else if (mode== 7) fprintf(fp5,"%15s %d\n",bbs.filename,num);
				else if (mode== 8) fprintf(fp5,"%15d %d\n",bbs.size,num);
				else if (mode== 9) fprintf(fp5,"%15d %d\n",bbs.filesize,num);
				else if (mode==10) fprintf(fp5,"%15d %d\n",bbs.down,num);
				else if (mode==11) fprintf(fp5,"%15d %d\n",bbs.read,num);
				else if (mode==12) fprintf(fp5,"%15d %d\n",bbs.line,num);
				else if (mode==13) fprintf(fp5,"%15d %d\n",bbs.num,num);
				else			   fprintf(fp5,"%15d %d\n",bbs.num,num);
			}
		}
		if (fp1!=NULL) fclose(fp1);
		if (fp5!=NULL) fclose(fp5);
		unlink(SORT_FILE);

		c_lock(1,NOW_FILE);
		printf("\n *** ��� ��ٷ� �ּ���... ���� ��Ʈ���Դϴ�. ... *** \n");
        if (order) xsort(1,TMP_FILE,SORT_FILE);
        else       xsort(0,TMP_FILE,SORT_FILE);
		if (!is_file(SORT_FILE)) {
			c_lock(0,NOW_FILE);
			return;
		}

		fp1=fopen(filename1,"r+"); //����
		fp2=fopen(filename2,"r+"); //����
		fp3=fopen(filename3,"w"); //������
		fp4=fopen(filename4,"w"); //������
		fp5=fopen(SORT_FILE,"r+");
		n=1;
		new_posi=0; //���ο� ��ġ
		new_num=1;	//���ο� ��ȣ
		printf("\n");

		while (n>0) {
			if (mode==3) n=fscanf(fp5,"%60s %d",&buff,&n2);
			else		 n=fscanf(fp5,"%15s %d",&buff,&n2);
			if (n<1) break;
			//�����б� fp1->fp3
			fseek(fp1,(n2-1)*sizeof_bbs,SEEK_SET);
			if (!fread(&bbs,sizeof_bbs,1,fp1)) continue;
			//�������� -> ������  fp2->fp4
			fseek(fp2,bbs.posi,SEEK_SET);
			fseek(fp4,new_posi,SEEK_SET);
			for (i=0;i<bbs.size&&i<1000000;i++) fputc(fgetc(fp2),fp4); //���뿡 ����
			bbs.posi=new_posi;
			if (renum) bbs.num =new_num;
			if (bbs.gd<' ') bbs.gd=' ';
			//������ ���� ->fp3
			fwrite(&bbs,sizeof_bbs,1,fp3);
			printf(" %5d %-8s %s\n",bbs.num,bbs.id,bbs.title);
			//���ο� ��ġ����
			new_posi+=bbs.size;
			new_num++;
		}
		if (fp1!=NULL) fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
		if (fp3!=NULL) fclose(fp3);
		if (fp4!=NULL) fclose(fp4);
		if (fp5!=NULL) fclose(fp5);
		//ȭ���� ������ 0���� ����
		set_zero(go.code);
		file_copy(filename3,filename1);
		file_copy(filename4,filename2);
		unlink(filename3);
		unlink(filename4);
		unlink(SORT_FILE);
		c_lock(0,NOW_FILE);
		printf("\n  %s �����ļ�(%d) \n",filename1,new_num-1);
		mess(" �ڷắȯ�� ���� �����ϴ�.\n");
	}
}


//IP���� ������ ����
bset_ipgo()
{
}



//���̹��Ӵ� ���� ������ ����
bset_cash()
{
	while (1) {
		cls();
		print_(1,"���̹��Ӵ� ���� ���� (%s:%s)",go.code,go.name);
		printf("  1. ���Խ� ����(�д�)                  :%d\n",cash.time1  );
		printf("  2. ���Խ� ����(�Ǵ�)                  :%d\n",cash.time2  );
		printf("  3. ������ ����(�Ǵ�)                  :%d\n",cash.read1  );
		printf("  4. ������ ����(1K��)                  :%d\n",cash.read2  );
		printf("  5. �ٿ�� ����(�Ǵ�)                  :%d\n",cash.down1  );
		printf("  6. �ٿ�� ����(1K��)                  :%d\n",cash.down2  );
		printf("  7. ����   ����(�Ǵ�)                  :%d\n",cash.writ1  );
		printf("  8. ����   ����(1K��)                  :%d\n",cash.writ2  );
		printf("  9. ��Ͻ� ����(�Ǵ�)                  :%d\n",cash.uplo1  );
		printf(" 10. ��Ͻ� ����(1K��)                  :%d\n",cash.uplo2  );
		printf(" 11. ������ ���̹��Ӵ� ������           :%s\n",rbit(cash.mode,1,"����","����"));
		printf(" 12. 3���� ������ ���̹��Ӵ� ������     :%s\n",rbit(cash.mode,2,"����","����"));
		n=numselect();xy(60,n+1);
        if (n<= 0) break;
		if (n== 1) cash.time1=get_num(cash.time1,6);
		if (n== 2) cash.time2=get_num(cash.time2,6);
		if (n== 3) cash.read1=get_num(cash.read1,6);
		if (n== 4) cash.read2=get_num(cash.read2,6);
		if (n== 5) cash.down1=get_num(cash.down1,6);
		if (n== 6) cash.down2=get_num(cash.down2,6);
		if (n== 7) cash.writ1=get_num(cash.writ1,6);
		if (n== 8) cash.writ2=get_num(cash.writ2,6);
		if (n== 9) cash.uplo1=get_num(cash.uplo1,6);
		if (n==10) cash.uplo2=get_num(cash.uplo2,6);
		if (n==11) cash.mode =gtt_bit(cash.mode ,1);
		if (n==12) cash.mode =gtt_bit(cash.mode ,2);
	}
}




//���� ���� ������ ����
bset_bill()
{
	while (1) {
		cls();
		print_(1,"���� ���� ���� (%s:%s)",go.code,go.name);
		printf("  1. ���Խ� ����(�д�)                  :%d\n",bill.time1  );
		printf("  2. ���Խ� ����(�Ǵ�)                  :%d\n",bill.time2  );
		printf("  3. ������ ����(�Ǵ�)                  :%d\n",bill.read1  );
		printf("  4. ������ ����(1K��)                  :%d\n",bill.read2  );
		printf("  5. �ٿ�� ����(�Ǵ�)                  :%d\n",bill.down1  );
		printf("  6. �ٿ�� ����(1K��)                  :%d\n",bill.down2  );
		printf("  7. ����   ����(�Ǵ�)                  :%d\n",bill.writ1  );
		printf("  8. ����   ����(1K��)                  :%d\n",bill.writ2  );
		printf("  9. ��Ͻ� ����(�Ǵ�)                  :%d\n",bill.uplo1  );
		printf(" 10. ��Ͻ� ����(1K��)                  :%d\n",bill.uplo2  );
		n=numselect();xy(60,n+1);
        if (n<= 0) break;
		if (n== 1) bill.time1=get_num(bill.time1,6);
		if (n== 2) bill.time2=get_num(bill.time2,6);
		if (n== 3) bill.read1=get_num(bill.read1,6);
		if (n== 4) bill.read2=get_num(bill.read2,6);
		if (n== 5) bill.down1=get_num(bill.down1,6);
		if (n== 6) bill.down2=get_num(bill.down2,6);
		if (n== 7) bill.writ1=get_num(bill.writ1,6);
		if (n== 8) bill.writ2=get_num(bill.writ2,6);
		if (n== 9) bill.uplo1=get_num(bill.uplo1,6);
		if (n==10) bill.uplo2=get_num(bill.uplo2,6);
	}
}

//�������� ������ ����
bset_wars()
{
	while (1) {
		cls();
		print_(1,"���� ���� ���� (%s:%s)",go.code,go.name);
		printf("  1. ���Խ� ����(�д�)                  :%d\n",wars.time1  );
		printf("  2. ���Խ� ����(�Ǵ�)                  :%d\n",wars.time2  );
		printf("  3. ������ ����(�Ǵ�)                  :%d\n",wars.read1  );
		printf("  4. ������ ����(1K��)                  :%d\n",wars.read2  );
		printf("  5. �ٿ�� ����(�Ǵ�)                  :%d\n",wars.down1  );
		printf("  6. �ٿ�� ����(1K��)                  :%d\n",wars.down2  );
		printf("  7. ����   ����(�Ǵ�)                  :%d\n",wars.writ1  );
		printf("  8. ����   ����(1K��)                  :%d\n",wars.writ2  );
		printf("  9. ��Ͻ� ����(�Ǵ�)                  :%d\n",wars.uplo1  );
		printf(" 10. ��Ͻ� ����(1K��)                  :%d\n",wars.uplo2  );
		printf(" 11. �������¸�弳�� \n");
		printf(" 12. �������¸�弳�� \n");
		printf(" 13. ������  ��弳�� \n");
		n=numselect();xy(60,n+1);
        if (n<= 0) break;
		if (n== 1) wars.time1=get_num(wars.time1,6);
		if (n== 2) wars.time2=get_num(wars.time2,6);
		if (n== 3) wars.read1=get_num(wars.read1,6);
		if (n== 4) wars.read2=get_num(wars.read2,6);
		if (n== 5) wars.down1=get_num(wars.down1,6);
		if (n== 6) wars.down2=get_num(wars.down2,6);
		if (n== 7) wars.writ1=get_num(wars.writ1,6);
		if (n== 8) wars.writ2=get_num(wars.writ2,6);
		if (n== 9) wars.uplo1=get_num(wars.uplo1,6);
		if (n==10) wars.uplo2=get_num(wars.uplo2,6);
	}
}


//��ȸ���
stat_rec()
{
	FILE *fp1,*fp2;
	int filesize;
    int it1,it2,it3,i,n,n1,count;
    int year,month,day1,day2;
	char st[30][100];
    char buff[300],buff1[300],buff2[300],buff3[300];
    char id1[10],id2[10],tstr[50];
    char filename[300];

    strcpy(id1,"");strcpy(id2,"");strcpy(tstr,"");
    year=now_year()%100;
	month=now_month();
    day1=1;day2=31;

	while (1) {
		cls();
		print_(1,"��ȸ��Ȳ / IP ���� �˻�  (%s:%s ���:%s)",go.code,go.name,bcfg.man1);
        printf("  1. �˻� ���� �Ⱓ   ��/�� ����-����   :%d/%d %d-%d\n",year%100,month,day1,day2);
        printf("  2. �ڷ�/�Խù� ����� ID              :%s \n",id1);
        printf("  3. �˻� ���   ȸ��   ID              :%s \n",id2);
        printf("  4. ���Թ������Ϻ�                     :%s\n",tstr);
        printf(" 96. %s ��ü ��� ��� Ȯ��\n",go.code);
        printf(" 97. %s �ڷ� ���/���� ��� Ȯ��\n",go.code);
        printf(" 98. %s �ڷ� ��ȸ/�ٿ� ��� Ȯ��\n",go.code);
		n=numselect();
		if (n<30) xy(50,n+1);
        if (n<= 0)  break;
        if (n== 1)  {year=get_num(year,2);month=get_num(month,2);
                     day1=get_num(day1,2);day2=get_num(day2,2);}
		if (n== 2)	get_str(id1,8);
		if (n== 3)	get_str(id2,8);
        if (n== 4)  get_str(tstr,10);
        if (n>=96&&n<=99)  {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			printf("   [%s] �˻��� �����մϴ�... \n",filename);
			if (!is_file(filename)) {mess("%s ��� ȭ���� �����ϴ�.",filename);continue;}
            if ((fp1=fopen(filename,"r"))==NULL) break;
            if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
			count=it1=it2=it3=0;
            if (n==96) {
                fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
                fprintf(fp2,"�� ���� �ð�  ##�� ����� �� �۾� ��                  �� ��                 ��\n");    //�ش�Ǵ� ���� ������
                fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
            }
            else if (is_typepds) {
                fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
                fprintf(fp2,"�� ���� �ð�  ##�� ����� �� ����� ��  �� �� ��  ��ũ�� K��  �� ��         ��\n");    //�ش�Ǵ� ���� ������
                fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
            }
            else {
                fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
                fprintf(fp2,"�� ���� �ð�  ##�� ����� �� ����� ��                  �� ��               ��\n");    //�ش�Ǵ� ���� ������
                fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
            }
            while (fgets(buff,200,fp1)!=NULL) {
                if (strlen(buff)<40||buff[0]==';') continue;
				for (i=0;i<30;i++) strcpy(st[i],"");
				sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
				strcpy(buff2,&buff[91]);
				sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
                if (strlen(st[1])!=5||strlen(st[2])!=5||strlen(st[5])<3) continue;
                if (n==96) strcpy(st[25],&buff[91]);
                else if (is_same(st[21],"B")) strcpy(st[25],&buff[102]);
/*
1	   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 ��ȣ1 kslab						   STA 0 1
05-26 00:40  77 p1	 �û�	  19 main	  f1		8 �ٿ�1 kslab						   P �û�	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 �û�	  19 main	  f1		6 ���1 kslab						   P �û�	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 ����1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 ����2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 ����1 kslab		 STA 0
01-01 12:49   1 /1   okisland  9 main     hostbbs   5 ����1 capi.pe.kr                     B okisland [�ý���] 2000�� ����
01-01 12:49   1 /1   okisland  9 main     hostbbs   7 �б�1 capi.pe.kr                     B okisland [�ý���] 2000�� ����
01-01 12:49   1 /1   okisland  9 main     hostbbs   7 �б�1 capi.pe.kr                     B kjh714   [ȸ  ��] ���Ա��� ��
01-03 19:35   1 /0   �û�     19 main     pxhost    6 ���1 top.xhost.co.kr                P �û�     viewlog 170414 [xhos
01-04 17:02   1 /0   �û�     19 main     pxhost    6 ���1 xbase.xhost.co.kr              P �û�     bin522.tgz 3619458 [
01-04 17:04   1 /0   �û�     19 main     pxhost    6 ���1 xbase.xhost.co.kr              P �û�     bin522_.tgz 3601344 
01-04 17:16   1 /1   guestt1   0 main     pxhost    7 �б�1 U211-33-81-242.thrun           B �û�     [xhost5.22] ���׷���
01-04 17:17   1 /1   guestt1   0 main     pxhost    8 �ٿ�1 U211-33-81-242.thrun           P �û�     bin522_.tgz 3601344 
*/
                if (!is_same(st[7],club_go.code)||!is_same(st[8],go.code)) continue;
                if (strlen(tstr)>0&&!stristr(buff,tstr)) continue;
                if (strlen(id2)>0&&!is_same(id2,st[5])) continue;
                if (strlen(id1)>0&&!is_same(id1,st[22])) continue;
                if (n==96) ;
                else if (is_typepds) {
                    if (n==98&&!is_same(st[10],"�ٿ�1")) continue;
                    if (n==97&&!is_same(st[10],"���1")) continue;
                }
                else {
                    if (n==98&&!is_same(st[10],"�б�1")) continue;
                    if (n==97&&!is_same(st[10],"����1")) continue;
                }
                xstrncpy(buff1,&buff[3],2);
                i=atoi(buff1);
                if (i<day1||i>day2) continue;
				count++;
				n1=atoi(st[24])/1024;
				it1+=n1;
                if (n==96) {
                    sprintf(buff,"��%5.5s %5.5s%3.3s��%-8s��%-6s��%-40.40s��",
                                st[1],st[2],st[3],
                                st[5],st[10],st[25]);
                }
                else if (is_typepds) {
                    sprintf(buff,"��%5.5s %5.5s%3.3s��%-8s��%-8s��%-12.12s��%6d��%-16.16s��",
                                st[1],st[2],st[3],
                                st[5],st[22],st[23],
                                n1,st[25]);
                }
                else {
                    sprintf(buff,"��%5.5s %5.5s%3.3s��%-8s��%-8s��%-38.38s��",
                                st[1],st[2],st[3],
                                st[5],st[22],st[25]);
                }
				del_enter(buff);
				fprintf(fp2,"%s\n",buff);    //�ش�Ǵ� ���� ������
			}
            if (n==96) {
                fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
                fprintf(fp2,"��%4d��\n",count);    //�ش�Ǵ� ���� ������
            }
            else if (is_typepds) {
                fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
                fprintf(fp2,"��%4d��  ��%d Mb  \n",count,it1/1024);    //�ش�Ǵ� ���� ������
            }
            else {
                fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
                fprintf(fp2,"��%4d��  \n",count);    //�ش�Ǵ� ���� ������
            }
            fclose(fp1);
			fclose(fp2);
			if (count<1) mess("�ش�Ǵ� ���ӱ���� �����ϴ�.");
			else		 view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
	}
}



set_jpgcfg()
{
	int n,i,n1,n2;
	char buff[100];
	while (1) {
		cls();
		print_(1,"%8s ȭ�� �Խ��� ���� ����",go.code);
		printf("  1. ȭ��Խ��� ���� X ��ġ    (0.�⺻ 1 - 80) :%d \n",bcfg.jpg_x);
		printf("  2. ȭ��Խ��� ���� Y ��ġ    (0.�⺻ 1 - 25) :%d \n",bcfg.jpg_y);
		printf("  3. �� ȭ��    ���� X ��ġ    (0.�⺻ 1 - 80) :%d \n",bcfg.disp_x);
		printf("  4. �� ȭ��    ���� Y ��ġ    (0.�⺻ 1 - 25) :%d \n",bcfg.disp_y);
		printf("  5. ��ȣ ���ý� ��ȣ.jpg ������               :%s \n",rbit(bcfg.jpgmode,1,"����","����"));
		printf("  6. �׸������� Enter -> ������� ���ƿ�       :%s \n",rbit(bcfg.jpgmode,2,"���ƿ�","����"));
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
		if (n==1) bcfg.jpg_x	 =get_num(bcfg.jpg_x	 ,2);
		if (n==2) bcfg.jpg_y	 =get_num(bcfg.jpg_y	 ,2);
		if (n==3) bcfg.disp_x	 =get_num(bcfg.disp_x	 ,2);
		if (n==4) bcfg.disp_y	 =get_num(bcfg.disp_y	 ,2);
		if (n==5) bcfg.jpgmode	 =gtt_bit(bcfg.jpgmode,1);
		if (n==6) bcfg.jpgmode	 =gtt_bit(bcfg.jpgmode,2);
	}
}



set_reccfg()
{
	int n,i,n1,n2;
	char buff[100];
	while (1) {
		cls();
		printf("              %8s �Խ��� ��� ���� ����\n",go.code);
		printf("  1. ��ȸ�� ��ȸ��Ȳ menudata/menurec ���     :%s \n",rbit(bcfg.recmode,1,"��","����"));
		printf("  2. �Խ��� ����/����� ���                   :%s \n",rbit(bcfg.recmode,2,"��","����"));
		printf("  3. ��ȸ�� �б�� ���� ��                     :%s \n",rbit(bcfg.recmode,3,"��","����"));
		printf("  4. ��ȸ�� �ٿ�� ���� ��                     :%s \n",rbit(bcfg.recmode,4,"����","��"));
		printf("  5. ��ȸ�� ������ ���� �� (����ڿ��κ񱳾���):%s \n",rbit(bcfg.recmode,5,"����","��"));
		printfL2();
		printf(" ���� : >>");
		n=numinput();
		printf("\n");
        if (n<=0) break;
		if (n==1) bcfg.recmode=gtt_bit(bcfg.recmode,1);
		if (n==2) bcfg.recmode=gtt_bit(bcfg.recmode,2);
		if (n==3) bcfg.recmode=gtt_bit(bcfg.recmode,3);
		if (n==4) bcfg.recmode=gtt_bit(bcfg.recmode,4);
		if (n==5) bcfg.recmode=gtt_bit(bcfg.recmode,5);
	}
}



bbs_all_del()
{
	int n,i,n1,n2;
	char buff[100];
	while (1) {
		cls();
        print_("%8s �Խ��� ��ü ����/����",go.code);
		printf("  1. ��ü�Խù� ���� ó��                                \n");
		printf("  2. ��ü�Խù� ���� ó��                                \n");
        printf("  3. ���� �ڷ� ����� (����/ȭ�ϸ�/�������)             \n");
        printf("  6. ������ ���� BYTE���� �Խù� ���� ����ó��           \n");
        printf("  7. ������ ���� BYTE�̻� �Խù� ���� ����ó��           \n");
        printf("  8. ��ü�Խù� ���� ����                                \n");
        printf("  9. ������ Ư�� ���ڿ� ���� �ڷ� �����           \n");
        printf(" 10. Ư�� ID �ڷ� �����           \n");
        printf(" 11. �ٸ��Խ���->�ڵ����                                \n");
		printf(" ���� : >>");
		n=numinput();
		printf("\n");
             if (n<=0) break;
        else if (n==1) set_all_del(1);
        else if (n==2) set_all_del(2);
        else if (n==3) del_same();
        else if (n==6) del_all_size(0);
        else if (n==7) del_all_size(1);
        else if (n==8) del_allbbs();
        else if (n==9) del_strin();
        else if (n==10) del_strid();
        else if (n==11) b_add();
	}
}






bbs_all_set()
{
	int n;
	while (1) {
		cls();
		printf("              %8s �Խ��� ����\n",go.code);
		printfL2();
        printf("  1. ��ü�Խù� ��õ ���� �����                   \n");
        printf("  2. ��ü�Խù� OK,NO ���� �����                  \n");
        printf("  3. ��ü�Խù� ��ȣ ���ֱ�                        \n");
        printf("  4. ��ü�Խù� �������� ���ֱ�                    \n");
        printf("  5. ��ü�Խù� �ٿ�� ���ֱ�                      \n");
        printf("  6. ��ü�Խù� ��ȸ�� ���ֱ�                      \n");
        printf("  7. ��ü�Խù� Ű���� ���ֱ�                      \n");
        printf("  8. ��ü�Խù� ID/�̸� ���ֱ�                     \n");
        printf("  9. ��ü�Խù� ��¥ ���ó�¥��                    \n");
        printf(" 10. ��ü�Խù� �ڷ�ũ����� ��ġ���ϸ� ����     \n");
        printf(" 11. ��ü�Խù� Ư��ID/�̸����� ����               \n");
        printf(" 12. ��ü�Խù� Ű���� ���� ����                   \n");
        printf(" 14. ������ Ư�� ���ڿ� ���� ġȯ                  \n");
        printf(" 15. Ư��ID/�̸� �˻��� ����                       \n");
        printf(" 16. �̻����ϸ� ����(1���� �Ұ�)                   \n");
        printf(" 17. �������ϸ� �ڵ�����                           \n");
		printfL2();
		printf(" ���� : >>");
		n=numinput();
		printf("\n");
             if (n<=0)  break;
        else if (n==1)  bbs_del_all(1);
        else if (n==2)  bbs_del_all(2);
        else if (n==3)  bbs_del_all(3);
        else if (n==4)  bbs_del_all(4);
        else if (n==5)  bbs_del_all(5);
        else if (n==6)  bbs_del_all(6);
        else if (n==7)  bbs_del_all(7);
        else if (n==8)  bbs_del_all(8);
        else if (n==9)  bbs_del_all(9);
        else if (n==10) bbs_del_filename();
        else if (n==11) set_all_id();
        else if (n==12) set_all_key();
        else if (n==14) title_strchange();
        else if (n==15) id_strchange();
        else if (n==16) set_all_file(0);
        else if (n==17) bbs_del2_filename();
	}
}


set_gift()
{
	while (1) {
		cls();
		print_(0," ��ǰ���� ����");
		printf("  1. 1�� ���� ->   �� ǰ �� ��          : %s\n",rbit(bcfg.form,11,"����","����"));
		printf("  2. ó���б� ->   �� ǰ �� ��          : %s\n",rbit(bcfg.form,12,"����","����"));
		printf("  3. ó���ٿ� ->   �� ǰ �� ��          : %s\n",rbit(bcfg.form,13,"����","����"));
		xy(1,20);printf(" 50. ��ǰ �Է�  ����:%-8d  ĳ��:%-8d  �ٿ�:%-3dM\n",bcfg.uplevel,bcfg.upcash,bcfg.updown);
		n=numselect();
        if (n<= 0) break;
		if (n== 1) bcfg.form=gtt_bit(bcfg.form,11);
		if (n== 2) bcfg.form=gtt_bit(bcfg.form,12);
		if (n== 3) bcfg.form=gtt_bit(bcfg.form,13);
		if (n==50&&pf.type>18) {
			xy(30,20);bcfg.uplevel=get_num(bcfg.uplevel,8);
			xy(45,20);bcfg.upcash =get_num(bcfg.upcash ,8);
			xy(60,20);bcfg.updown =get_num(bcfg.updown ,3);
		}
	}
}



set_menucfg()
{
	int n,i,n1,n2,key;
	char buff[100];
	GO go2;
	FILE *fp;
	char menu_file[200];
	char filename[200];

	while (1) {
		if (bcfg.rdown>100) bcfg.rdown=100;
		if (bcfg.wdown>100) bcfg.wdown=100;
		del_esc(go.dir);
		cls();
		print_(2,"�Խ��� �޴� ����");
		printf("s  1. �޴� �����ڵ����        :%s\n",go.code);
		printf("   2. �޴� �̸� ����           :%s\n",go.name);
		printf("s  3. �޴� ���� ����           :%c\n",go.type);
		printf("s  4. �޴� ���丮����        :%s\n",go.dir );
        printf("   5.�������:  I:%2d      R:%2d      D:%2d      W:%2d   IL:%-8d IC:%-8d\n",
                bcfg.itype,bcfg.rtype,bcfg.dtype,bcfg.wtype,bcfg.ilevel,bcfg.icash);
        printf("   6.�������: FD:%2d     FR:%2d\n",bcfg.fdown,bcfg.fread);
        printf("s  7.������: UR:%2d     MR:%2d     DD:%2d     MD:%2d   LV:%d   LC:%d\n",
                bcfg.wdown,bcfg.wdown2,bcfg.rdown,bcfg.rdown2,bcfg.idlevel,bcfg.idcash);
        printf("s  8.��������: ����:%3d  �б�:%3d  �ٿ�:%3d\n",
				bcfg.level,bcfg.level2,bcfg.level3);
        printf("s  9.Byte/1�� ����: %u \n",bcfg.level_size);
        printf("  10. ���� ����                :%-3s  (D�� W�� M�� A��ü) \n",go.wtimes);
        printf("  11. �ϴ� �ٷΰ��� �޴�       :%s.dim\n",go.code);
        printf("  12. �Խ��� �ð� ����         \n");
        printf("s 13. ���Ծ�ȣ ���� ����       :%s\n",bcfg.passin);
        printf("s 14. ����ȣ ���� ����       :%s\n",is_same(bcfg.man1,pf.id)?bcfg.passin:"****");
        printf("s 15. ��������                 :%s\n",rbit(bcfg.mode,0,"����","����"));
        printf("  16. �Խ��� �����            :(%-8s %-8s %-8s %-8s %-8s)\n",bcfg.man1,bcfg.man2,bcfg.man3,bcfg.man4,bcfg.man5);
		n=numselect();
		xy(50,n+1);
        if (n<= 0) break;
        if (n== 1&&pf.type>=17) get_str(go.code,8);
		if (n== 2) get_str(go.name,30);
        if (n== 3&&(pf.type>=17||cpf.type>=17)) {
            sprintf(buff,"%c",go.type);
            get_str(buff,3);
            check_han(buff);
            go.type=buff[0];
        }
        if (n== 4&&pf.type>=17) {
			strcpy(buff,go.dir);
			get_str(buff,60);check_han(buff);strlwr(buff);
			strchng2(buff,"_S"," ");all_trim(buff);
			//���ڿ��� ȭ�ϼ��� ������ ���ԵǸ� Yes;
			if (strlen(buff)>0) {
				if (pf.type<19&&((stristr(buff,"bin/")||stristr(buff,"system/")||
					stristr(buff,";")||stristr(buff,"&")||
					stristr(buff,"*")||stristr(buff,"?")||
					stristr(buff,"cfg/")||stristr(buff,"sys_back")||
					stristr(buff,"/etc/")||stristr(buff,"/system/"))));
				else if (in_file3("system/notdir.txt",buff));
				else strcpy(go.dir,buff);
			}
		}
        if (n== 5) {
            xy(18, n+1);bcfg.itype =get_num(bcfg.itype ,2);
            xy(28, n+1);bcfg.rtype =get_num(bcfg.rtype ,2);
            xy(38, n+1);bcfg.dtype =get_num(bcfg.dtype ,2);
            xy(48, n+1);bcfg.wtype =get_num(bcfg.wtype ,2);
            xy(58, n+1);bcfg.ilevel=get_num(bcfg.ilevel,9);
            xy(68, n+1);bcfg.icash =get_num(bcfg.icash ,9);
		}
        if (n== 6) {
            xy(18, n+1);bcfg.fdown =get_num(bcfg.fdown ,2);
            xy(28, n+1);bcfg.fread =get_num(bcfg.fread ,2);
		}
        if (n== 7&&(pf.type>=17||cpf.type>=17)) {
            xy(18, n+1);bcfg.wdown =get_num(bcfg.wdown ,2);
            xy(28, n+1);bcfg.wdown2=get_num(bcfg.wdown2,2);
            xy(38, n+1);bcfg.rdown =get_num(bcfg.rdown ,2);
            xy(48, n+1);bcfg.rdown2=get_num(bcfg.rdown2,2);
            xy(58, n+1);bcfg.idlevel=get_num(bcfg.idlevel,8);
            xy(68, n+1);bcfg.idcash=get_num(bcfg.idcash,8);
		}
        if (n== 8&&(pf.type>=17||cpf.type>=17)) {
            xy(18, n+1);bcfg.level =get_num(bcfg.level ,3);
            xy(28, n+1);bcfg.level2=get_num(bcfg.level2,3);
            xy(38, n+1);bcfg.level3=get_num(bcfg.level3,3);
		}
        if (n== 9&&(pf.type>=17||cpf.type>=17)) {
            bcfg.level_size=get_num(bcfg.level_size,10);
        }
        if (n==10) get_str(go.wtimes,4);
        if (n==11) xedit2("%s/logo/%s.dim",club_go.dir,go.code);
        if (n==12) set_timecfg();
        if (n==13&&(pf.type>=17||cpf.type>=17||is_same(bcfg.man1,bcfg.man1))) get_str(bcfg.passin,4);
        if (n==14&&(pf.type>=17||cpf.type>=17||is_same(bcfg.man1,bcfg.man1))) get_str(bcfg.passsys,4);
        if (n==15&&(pf.type>=17||cpf.type>=17)) bcfg.mode=gtt_bit(bcfg.mode,0);
        if (n==16&&(pf.type>=17||cpf.type>=17||is_same(bcfg.man1,bcfg.man1))) {
            get_str(bcfg.man1,8);get_str(bcfg.man2,8);get_str(bcfg.man3,8);
            get_str(bcfg.man4,8);get_str(bcfg.man5,8);
		}
	}
    if (strlen(bcfg.passin)<2) strcpy(bcfg.passin,"");
    if (strlen(bcfg.passsys)<2) strcpy(bcfg.passsys,"");
}


set_timecfg()
{
	int n,i,n1,n2,key;
	char buff[100];
	GO go2;
	FILE *fp;
	char menu_file[200];
	char filename[200];

	while (1) {
		cls();
        print_(2,"�Խ��� �ð�����");
        printf("  1. �޴� ���԰��� �ð�       :%s \n",time_limitstr(bcfg.itime));
        printf("  2. �޴� �ٿ�� �ð�       :%s \n",time_limitstr(bcfg.dtime));
        printf("  3. �޴� �бⰡ�� �ð�       :%s \n",time_limitstr(bcfg.rtime));
        printf("  4. �޴� ��ϰ��� �ð�       :%s \n",time_limitstr(bcfg.wtime));
		n=numselect();
		xy(50,n+1);
        if (n<= 0) break;
        if (n== 1) bcfg.itime=get_num(bcfg.itime,8);
        if (n== 2) bcfg.dtime=get_num(bcfg.dtime,8);
        if (n== 3) bcfg.rtime=get_num(bcfg.rtime,8);
        if (n== 4) bcfg.wtime=get_num(bcfg.wtime,8);
    }
}


set_bcfg1()
{
	while (1) {
		cls();
		print_(0,"�Խ��� ȯ�� ����");
		printf("  1. �մԽ� ���Խ� ID����                      :%s\n",rbit(bcfg.mode,18,"����","����"));
		printf("  2. �մԽ� ��Ͻ� ID����                      :%s\n",rbit(bcfg.mode,19,"����","����"));
		printf("  3. ������¥ ����ڷ� �ڵ�����(0:���� ?.����) :%d\n",bcfg.outday);
		printf("  4. �ܺ� ��� ȣ�� ���α׷�                   :%s\n",rbit(bcfg.mode,2 ,"�Ұ�","ȣ��"));
		printf("  5. KEY = ����/�亯�ڵ�ó��                   :%s\n",rbit(bcfg.mode,11,"����","�ڵ�"));
		printf("  6. ���� PR/DN ����                           :%s\n",rbit(bcfg.form,1 ,"����","����"));
		printf("  7. �̵�/����  ����                           :%s\n",rbit(bcfg.form,2 ,"����","����"));
		printf("  8. ����/����  ����                           :%s\n",rbit(bcfg.form,3 ,"����","����"));
		printf("  9. ��������Ʈ ���/���� ����                 :%s\n",rbit(bcfg.mode,28,"���","����"));
		printf(" 10. ��������Ʈ �ٿ�/�б� ����                 :%s\n",rbit(bcfg.mode,29,"���","����"));
		printf(" 11. �Խ��ǿ��� ������                       :%s\n",rbit(bcfg.mode,8 ,"������","������"));
		printf(" 12. Ű������                                :%s\n",rbit(bcfg.mode,9 ,"������","�����"));
		printf(" 13. ���Խ� Enter Ű�Է� ����                  :%s\n",rbit(bcfg.form,4 ,"���","����"));
		printf(" 14. ���Խ� ���� ������ �Խ��� ����            :%s\n",rbit(bcfg.form,5 ,"����","����"));
		printf(" 15. ���Խ� ������ �۾��� ���                 :%s\n",rbit(bcfg.form,6 ,"����","����"));
		printf(" 16. ���,���14 ������ �Խù� �б�            :%s\n",rbit(bcfg.mode,21,"����","�Ұ�"));
        printf(" 17. �Խ��� �б�/�ٿ�� ��ȣ ����              :%s\n",rbit(bcfg.mode,31,"Ȯ��","����"));
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
		if (n== 1) bcfg.mode=gtt_bit(bcfg.mode,18);
		if (n== 2) bcfg.mode=gtt_bit(bcfg.mode,19);
		if (n== 3) bcfg.outday=get_num(bcfg.outday,8);
		if (n== 4) bcfg.mode=gtt_bit(bcfg.mode,2 );
		if (n== 5) bcfg.mode=gtt_bit(bcfg.mode,11);
		if (n== 6) bcfg.form=gtt_bit(bcfg.form,1 );
		if (n== 7) bcfg.form=gtt_bit(bcfg.form,2 );
		if (n== 8) bcfg.form=gtt_bit(bcfg.form,3 );
		if (n== 9) bcfg.mode=gtt_bit(bcfg.mode,28);
		if (n==10) bcfg.mode=gtt_bit(bcfg.mode,29);
		if (n==11) bcfg.mode=gtt_bit(bcfg.mode,8 );
		if (n==12) bcfg.mode=gtt_bit(bcfg.mode,9 );
		if (n==13) bcfg.form=gtt_bit(bcfg.form,4 );
		if (n==14) bcfg.form=gtt_bit(bcfg.form,5 );
		if (n==15) bcfg.form=gtt_bit(bcfg.form,6 );
        if (n==16) bcfg.mode=gtt_bit(bcfg.mode,31);
        if (n==17) bcfg.mode=gtt_bit(bcfg.mode,31);
	}
}

set_limit()
{
	while (1) {
		cls();
		print_(0,"�Խ��� ��������/����/���� ����");
        printf("  1. ����� ���̵�/�̸�����ó��                :%s\n",rbit(bcfg.mode,5,"����","����"));
        printf("  2. �Ϲ�ȸ������ ���̵���                   :%s\n",rbit(bcfg.mode,6,"����","����"));
        printf("  3. ��üȸ������ ���̵���                   :%s\n",rbit(bcfg.mode,7,"����","����"));
        printf("  4. ���Խ� �����ڵ� ����                      :%s\n",rbit(bcfg.mode,15,"����","����"));
		printf("  5. ������ ������ ���̵� (%s.lir) \n",go.code);
		printf("  6. ����   ������ ���̵� (%s.liw) \n",go.code);
        printf("  7. ������ ������ ���̵𸸰���                :%s\n",rbit(bcfg.modelist,5,"����","����"));
        printf("  8. ����   ������ ���̵𸸰���                :%s\n",rbit(bcfg.modelist,6,"����","����"));
		printf("  9. ���Խ� ���� ���� ����� �̻� ����         :%d \n",bcfg.li_w);
        printf(" 10. ���� ���԰���              (�����ڵ�.lm1) :%s\n",rbit(bcfg.imode,11,"����","�Ұ�"));
        printf(" 11. ���� ���Ⱑ��              (�����ڵ�.lm3) :%s\n",rbit(bcfg.imode,12,"����","�Ұ�"));
        printf(" 12. ���� ���԰���              (�����ڵ�.lm5) :%s\n",rbit(bcfg.imode,13,"����","�Ұ�"));
        printf(" 13. ���� ���Ⱑ��              (�����ڵ�.lm7) :%s\n",rbit(bcfg.imode,14,"����","�Ұ�"));
        printf(" 14. 19���̸� ���԰���          (�����ڵ�.lm2) :%s\n",rbit(bcfg.imode,15,"����","�Ұ�"));
        printf(" 15. 19���̸� ���Ⱑ��          (�����ڵ�.lm4) :%s\n",rbit(bcfg.imode,16,"����","�Ұ�"));
        printf(" 16. 19���̻� ���԰���          (�����ڵ�.lm6) :%s\n",rbit(bcfg.imode,17,"����","�Ұ�"));
        printf(" 17. 19���̻� ���Ⱑ��          (�����ڵ�.lm8) :%s\n",rbit(bcfg.imode,18,"����","�Ұ�"));
        printf(" 18. ���԰��� ID ���� (%s.sid)                  \n",go.code);
        printf(" 19. �������� ID ���� (%s.sii)                  \n",go.code);
        printf(" 20. lt,li,ln ���� �˻����                    :%s\n",rbit(bcfg.modelist,20,"����","�Ұ�"));
        printf(" 21. cp,mv,dn,pr ���� �˻����                 :%s\n",rbit(bcfg.modelist,21,"����","�Ұ�"));
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
		if (n==1) bcfg.mode=gtt_bit(bcfg.mode,5);
		if (n==2) bcfg.mode=gtt_bit(bcfg.mode,6);
		if (n==3) bcfg.mode=gtt_bit(bcfg.mode,7);
		if (n==4) bcfg.mode=gtt_bit(bcfg.mode,15);
		if (n==5) xedit2("%s/logo/%s.lir",club_go.dir,go.code);
		if (n==6) xedit2("%s/logo/%s.liw",club_go.dir,go.code);
		if (n==7) bcfg.modelist=gtt_bit(bcfg.modelist,5);
		if (n==8) bcfg.modelist=gtt_bit(bcfg.modelist,6);
		if (n==9) bcfg.li_w    =get_num(bcfg.li_w,3);
        if (n==10) bcfg.imode=gtt_bit(bcfg.imode,11);
        if (n==11) bcfg.imode=gtt_bit(bcfg.imode,12);
        if (n==12) bcfg.imode=gtt_bit(bcfg.imode,13);
        if (n==13) bcfg.imode=gtt_bit(bcfg.imode,14);
        if (n==14) bcfg.imode=gtt_bit(bcfg.imode,15);
        if (n==15) bcfg.imode=gtt_bit(bcfg.imode,16);
        if (n==16) bcfg.imode=gtt_bit(bcfg.imode,17);
        if (n==17) bcfg.imode=gtt_bit(bcfg.imode,18);
        if (n==18) xedit2("%s/logo/%s.sid",club_go.dir,go.code);
        if (n==19) xedit2("%s/logo/%s.sii",club_go.dir,go.code);
        if (n==20) bcfg.modelist=gtt_bit(bcfg.modelist,20);
        if (n==21) bcfg.modelist=gtt_bit(bcfg.modelist,21);
	}
}




set_updown()
{
	int n,i,n1,n2;
    char buff[200];
	while (1) {
		cls();
		print_(0,"�ڷ� ��/�ٿ� ȯ�� ����");
		printf("  1. �Ϲ�ȸ��(0~14) ȭ������ ����              :%s\n",rbit(bcfg.downmode, 1,"�㰡","����"));
		printf("  2. ��� (14~19) ȭ������ ����              :%s\n",rbit(bcfg.downmode, 2,"�㰡","����"));
		printf("  3. ȭ�Ͼ��� ���� ->���ó��                  :%s\n",rbit(bcfg.downmode,11,"���ó��","�������"));
		printf("  4. ȭ�Ͼ��� �ڷ� ->����ó��                  :%s\n",rbit(bcfg.downmode,12,"��������","�ڵ�����"));
		printf("  5. ȭ�Ͼ��� �ڷ� ->���ϸ� ����ó��           :%s\n",rbit(bcfg.downmode,13,"����ó��","ȭ�Ͼ���"));
		printf("  6. �ڷ��ȣ ���ý� �ڵ��ٿ�                  :%s\n",rbit(bcfg.downmode,14,"����","�ڵ��ٿ�"));
		printf("  7. �ߺ��ڷ�(ȭ�ϸ���)��� ó��             :%s\n",rbit(bcfg.downmode,15,"��������","�����"));
		printf("  8. ��ϵ���,�������� Enter�� �⺻�亯        :%s\n",rbit(bcfg.downmode,16,"Yes","No"));
		printf("  9. �ڷ� �ٿ��� �Ϸ�޼���+ENTER              :%s\n",rbit(bcfg.downmode,17,"Enter","����"));
		printf(" 10. �ڷ� ����� �Ϸ�޼���+ENTER              :%s\n",rbit(bcfg.downmode,18,"Enter","����"));
		printf(" 11. ���� ���� �ٿ�޴»�� ���ϻ�����         :%s\n",rbit(bcfg.downmode,19,"����","�ڵ�����"));
		printf(" 12. ������ �ٿ�뷮 ������                    :%s\n",rbit(bcfg.mode,1,"����","����"));
		printf(" 13. 3���� ������ �ٿ�뷮 ������              :%s\n",rbit(bcfg.mode,2,"����","����"));
		printf(" 14. ��  ->�ٿ�뷮����                        :%3.1f ��\n",(float)bcfg.urate/10 );
		printf(" 15. �ٿ�->�ٿ�뷮����                        :%3.1f ��\n",(float)bcfg.drate/10 );
		printf(" 16. �ð����� ��  ->�ٿ�뷮 ����  %2d-%2d     :%3.1f ��\n",bcfg.uhour1,bcfg.uhour2,(float)bcfg.uratet/10 );
		printf(" 17. �ð����� �ٿ�->�ٿ�뷮 ����  %2d-%2d     :%3.1f ��\n",bcfg.dhour1,bcfg.dhour2,(float)bcfg.dratet/10 );
		printf(" 18. ���� ��   ��� �� ����  (0:������)        :%d\n",bcfg.up_limit   );
		printf(" 19. ���� �ٿ� ���� �� ����  (0:������)        :%d\n",bcfg.dn_limit   );
        printf(" 20. ��Ƽ �ٿ� ���� �� ����  (0:������)        :%d\n",bcfg.dn_mult_limit);
        printf(" 21. ���� �ٿ� �ӽ� ���� �޼��� (logo/%s.ndn)\n",go.code);
        printf(" 22. ��ü �Խ��� BDN ���� ����                 :%s\n",rbit(bcfg.downmode,20,"�Ұ���","����"));
        printf(" 23. BDN ���� ID ����           (logo/%s.bdn)\n",go.code);
		n=numselect();
		xy(50,n+1);
        if (n<= 0) break;
		if (n== 1) bcfg.downmode =gtt_bit(bcfg.downmode, 1);
		if (n== 2) bcfg.downmode =gtt_bit(bcfg.downmode, 2);
		if (n== 3) bcfg.downmode =gtt_bit(bcfg.downmode,11);
		if (n== 4) bcfg.downmode =gtt_bit(bcfg.downmode,12);
		if (n== 5) bcfg.downmode =gtt_bit(bcfg.downmode,13);
		if (n== 6) bcfg.downmode =gtt_bit(bcfg.downmode,14);
		if (n== 7) bcfg.downmode =gtt_bit(bcfg.downmode,15);
		if (n== 8) bcfg.downmode =gtt_bit(bcfg.downmode,16);
		if (n== 9) bcfg.downmode =gtt_bit(bcfg.downmode,17);
		if (n==10) bcfg.downmode =gtt_bit(bcfg.downmode,18);
		if (n==11) bcfg.downmode =gtt_bit(bcfg.downmode,19);
		if (n==12) bcfg.mode =gtt_bit(bcfg.mode ,1);
		if (n==13) bcfg.mode =gtt_bit(bcfg.mode ,2);
		if (n==14) bcfg.urate=get_num(bcfg.urate,3);
		if (n==15) bcfg.drate=get_num(bcfg.drate,3);
		if (n==16)	{
			bcfg.uhour1=get_num(bcfg.uhour1,2);bcfg.uhour2=get_num(bcfg.uhour2,2);
			bcfg.uratet=get_num(bcfg.uratet,3);
		}
		if (n==17)	{
			bcfg.dhour1=get_num(bcfg.dhour1,2);bcfg.dhour2=get_num(bcfg.dhour2,2);
			bcfg.dratet=get_num(bcfg.dratet,3);
		}
		if (n==18)	bcfg.up_limit	=get_num(bcfg.up_limit,3);
		if (n==19)	bcfg.dn_limit	=get_num(bcfg.dn_limit,3);
		if (n==20)	bcfg.dn_mult_limit=get_num(bcfg.dn_mult_limit,3);
        if (n==21)  xedit2("%s/logo/%s.ndn",club_go.dir,go.code);
        if (n==22)  bcfg.downmode =gtt_bit(bcfg.downmode,20);
        if (n==23)  xedit2("%s/logo/%s.bdn",club_go.dir,go.code);
	}
}



set_write()
{
	while (1) {
        if (strlen(bcfg.outbbs)<2) strcpy(bcfg.outbbs,"");
		cls();
		print_(0,"�Խ��� ���� ���� ����");
		printf("  1. ����� �⺻�����Ӹ���     (0.���� ?.����) :%d\n",bcfg.headnum);
		printf("  2. ����� ���� ����������    (0.���� ?.����) :%d\n",bcfg.runday);
		printf("  3. ����� �Խù�����ũ�� ����(0.���� ?.����) :%d\n",bcfg.limitsize);
		printf("  4. ����� �ڵ����� �����Խ��ǿ� ���� ����    :%s\n",bcfg.outbbs);
		printf("  5. ����� �ڵ����� �����Խ��� ����           :%s\n",rbit(bcfg.wmode,23,"����","����"));
		printf("  6. ����� �ڵ����� �����Խ��� ����           :%s\n",rbit(bcfg.wmode,24,"����","����"));
        printf("  7. ����� ������ ��õ���� ����               :%s\n",rbit(bcfg.wmode,25,"����","����"));
        printf("  8. ����� 10������ (250��)�̻� �Խù� ����   :%s\n",rbit(bcfg.wmode,26,"����","����"));
        printf("  9. �û�/�Խ��Ǵ���� ����� ����ϴ�ID       :%s\n",bcfg.write_id);
        xy(1,11);
        printf("11.�Ӹ��� ���ۿ��� %s\n",rbit(bcfg.wmode,1,"����","���"));
        printf("12.�Ӹ��� �������� %s\n",rbit(bcfg.wmode,2,"����","����"));
        printf("13.����Ϸ� �޼��� %s\n",rbit(bcfg.wmode,3,"Enter","����"));
        printf("14.�����Է¾���    %s\n",rbit(bcfg.wmode,4,"�Է�","����"));
        printf("15.�����Է³���ù��%s\n",rbit(bcfg.wmode,5,"����","ù�ٴ�ġ"));
        printf("16.������1 ��������%s\n",rbit(bcfg.wmode,6,"����","��������"));
        printf("17.                %s\n",rbit(bcfg.wmode,7,"����","����"));
        printf("18.����->���Ͼ���  %s\n",rbit(bcfg.wmode,8,"����","�߼�"));
        printf("19.���λ��� ÷��   %s\n",rbit(bcfg.wmode,9,"���","����"));
        printf("20.KEY �Է�        %s\n",rbit(bcfg.wmode,10,"���","����"));
        xy(40,11);printf(" 21.�����Է¹���    %s\n",rbit(bcfg.wmode,11,"����","���"));
        xy(40,12);printf(" 22.�������->���  %s\n",rbit(bcfg.wmode,12,"����","���"));
        xy(40,13);printf(" 23.���� Zmodem���� %s\n",rbit(bcfg.wmode,13,"����","����"));
        xy(40,14);printf(" 24.���� Zmodem��� %s\n",rbit(bcfg.wmode,14,"����","����"));
        xy(40,15);printf(" 25.bbs/pds_all���� %s\n",rbit(bcfg.wmode,15,"����","����"));
        xy(40,16);printf("26.�ڵ����2����  %s\n",rbit(bcfg.wmode,16,"����","�߼�"));
        xy(40,17);printf("27.10 Byte�̸����%s\n",rbit(bcfg.wmode,17,"����","���"));
        xy(40,18);printf("28.���ǻ��� ���  %s\n",rbit(bcfg.wmode,18,"��","����"));
        xy(40,19);printf("29.���� �ڵ� ���� %s\n",rbit(bcfg.wmode,19,"����","Zmodem�ڵ�"));
        xy(40,20);printf("30.���� �ڵ� ���� %s\n",rbit(bcfg.wmode,20,"����","Zmodem�ڵ�"));
        xy(40,21);printf("31.��� Ȯ��      %s\n",rbit(bcfg.wmode,21,"Ȯ��","����"));
        xy(40,22);printf("32.���������޼��� %s\n",rbit(bcfg.wmode,22,"���","����"));
		xy(1,23);
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
		if (n== 1) bcfg.headnum=get_num(bcfg.headnum,2);
		if (n== 2) bcfg.runday =get_num(bcfg.runday ,2);
		if (n== 3) bcfg.limitsize=get_num(bcfg.limitsize,10);
		if (n== 4) get_str(bcfg.outbbs,8);
		if (n== 5) bcfg.wmode=gtt_bit(bcfg.wmode,23);
		if (n== 6) bcfg.wmode=gtt_bit(bcfg.wmode,24);
        if (n== 7) bcfg.wmode=gtt_bit(bcfg.wmode,25);
        if (n== 8) bcfg.wmode=gtt_bit(bcfg.wmode,26);
        if (n== 9) get_str(bcfg.write_id,8);
		if (n==11) bcfg.wmode=gtt_bit(bcfg.wmode,1);
		if (n==12) bcfg.wmode=gtt_bit(bcfg.wmode,2);
		if (n==13) bcfg.wmode=gtt_bit(bcfg.wmode,3);
		if (n==14) bcfg.wmode=gtt_bit(bcfg.wmode,4);
		if (n==15) bcfg.wmode=gtt_bit(bcfg.wmode,5);
		if (n==16) bcfg.wmode=gtt_bit(bcfg.wmode,6);
		if (n==17) bcfg.wmode=gtt_bit(bcfg.wmode,7);
		if (n==18) bcfg.wmode=gtt_bit(bcfg.wmode,8);
		if (n==19) bcfg.wmode=gtt_bit(bcfg.wmode,9);
		if (n==20) bcfg.wmode=gtt_bit(bcfg.wmode,10);
		if (n==21) bcfg.wmode=gtt_bit(bcfg.wmode,11);
		if (n==22) bcfg.wmode=gtt_bit(bcfg.wmode,12);
		if (n==23) bcfg.wmode=gtt_bit(bcfg.wmode,13);
		if (n==24) bcfg.wmode=gtt_bit(bcfg.wmode,14);
		if (n==25) bcfg.wmode=gtt_bit(bcfg.wmode,15);
		if (n==26) bcfg.wmode=gtt_bit(bcfg.wmode,16);
		if (n==27) bcfg.wmode=gtt_bit(bcfg.wmode,17);
		if (n==28) bcfg.wmode=gtt_bit(bcfg.wmode,18);
		if (n==29) bcfg.wmode=gtt_bit(bcfg.wmode,19);
		if (n==30) bcfg.wmode=gtt_bit(bcfg.wmode,20);
		if (n==31) bcfg.wmode=gtt_bit(bcfg.wmode,21);
		if (n==32) bcfg.wmode=gtt_bit(bcfg.wmode,22);
	}
}


set_auto()
{
	while (1) {
		cls();
		print_(0,"�Խ��� �ڵ� ó�� ���� ����");
		printf("  1. ���丮���� �ڵ� ����ó�� (��ȣ.txt)     :%s\n",rbit(bcfg.automode,1,"����","�ڵ�"));
		printf("  2. ���丮���� �ڵ� ����ó�� (��ȣ.txt)     :%s\n",rbit(bcfg.automode,2,"����","�ڵ�"));
		printf("  3. ���丮���� �ڵ� ����ó�� (��ȣ.txt)     :%s\n",rbit(bcfg.automode,3,"����","�ڵ�"));
		printf("  4. �ڵ�����/�����ٻ����ѵ� ����ó��          :%s\n",rbit(bcfg.automode,4,"����","����"));
		printf("  5. ���������� ��¥,ũ�� ���ѵ� ��������    :%s\n",rbit(bcfg.automode,5,"�׻����","����"));
		printf("  6. ��ȣ.TXT��� Ű����.TXT��  (KEY.txt)      :%s\n",rbit(bcfg.automode,6,"��ȣ","KEY"));
		printf("                                                  \n");
		printf("                                                  \n");
		printf("                                                  \n");
		printf("                                                  \n");
		printf(" 11. ���� ������ �ڵ� ����ó��                 :%s\n",rbit(bcfg.automode,11,"����","����"));
		printf(" 12. ���� ������ ���� �ڵ�����                 :%s\n",rbit(bcfg.automode,12,"����","����"));
		printf("                                                  \n");
		printf("                                                  \n");
		printf(" 15. �б�� Ư�� �Խ��� �ڵ� �ҷ����� ����     :%s\n",rbit(bcfg.automode,15,"����","�ڵ��θ���"));
		printf(" 16. �б�� ��¥ ���� �ҷ�����               :%s\n",rbit(bcfg.automode,16,"����","������"));
		printf(" 17. �б�� �˻� ��� Ű���� �˻� ���         :%s\n",rbit(bcfg.automode,17,"����˻�","Ű����˻�"));
		printf(" 18. �б�� �ڵ����� �ҷ��� �Խ���             :%s\n",bcfg.inbbs);
		printf(" 19. �б�� �ڵ����� �ҷ��� �Խ��� Ű����      :%s\n",bcfg.inbbskey);
		n=numselect();
		xy(49,n+1);
        if (n<=0) break;
		if (who<3) {mess("���� �Ұ���");break;}
		if (n== 1) bcfg.automode=gtt_bit(bcfg.automode,1);
		if (n== 2) bcfg.automode=gtt_bit(bcfg.automode,2);
		if (n== 3) bcfg.automode=gtt_bit(bcfg.automode,3);
		if (n== 4) bcfg.automode=gtt_bit(bcfg.automode,4);
		if (n== 5) bcfg.automode=gtt_bit(bcfg.automode,5);
		if (n== 6) bcfg.automode=gtt_bit(bcfg.automode,6);
		if (n==11) bcfg.automode=gtt_bit(bcfg.automode,11);
		if (n==12) bcfg.automode=gtt_bit(bcfg.automode,12);
		if (n==15) bcfg.automode=gtt_bit(bcfg.automode,15);
		if (n==16) bcfg.automode=gtt_bit(bcfg.automode,16);
		if (n==17) bcfg.automode=gtt_bit(bcfg.automode,17);
		if (n==18) get_str(bcfg.inbbs,8);
		if (n==19) get_str(bcfg.inbbskey,10);
	}
}


set_listcfg()
{
	while (1) {
		cls();
		print_(0,"�Խ��� ��� ��� ���� ����");
		printf(" 1. �Խù� ���(LIST) ���߱�                  :%s \n",rbit(bcfg.modelist,7,"����","����"));
		printf(" 2. �Խù� �ڷ�� BYTE ����       99999B      :%s \n",rbit(bcfg.modelist,11,"����","����"));
		printf(" 3. �Խù� �ڷ�� BYTE ����       99999K      :%s \n",rbit(bcfg.modelist,12,"����","����"));
		printf(" 4. �Խù� �ڷ�� BYTE ����       99999M      :%s \n",rbit(bcfg.modelist,13,"����","����"));
		printf(" 5. �Խù� �ڷ�� BYTE ����                   :%s \n",rbit(bcfg.modelist,14,"����","����"));
		printf(" 6. �Խù� ��ȭ�� ��� �ڷ��     (0-120)     :%d \n",bcfg.maxline);
		printf(" 7. ũ�� 0�� �ڷ� �Ⱥ��̱�                    :%s \n",rbit(bcfg.modelist,19,"�Ⱥ���","����"));
		printf(" 8. �Խù� ��� ��� ��ġ (X)                 :%d \n",bcfg.list_x);
		printf(" 9. �Խù� ��� ��� ��ġ (Y)                 :%d \n",bcfg.list_y);
        printf("10. ��õ�ڷḸ ���̱�                         :%s \n",rbit(bcfg.modelist,22,"����","��õ�ڷḸ����"));
		n=numselect();
        if (n<=0) break;
		if (n==1) bcfg.modelist=gtt_bit(bcfg.modelist,7);
		if (n==2) bcfg.modelist=gtt_bit(bcfg.modelist,11);
		if (n==3) bcfg.modelist=gtt_bit(bcfg.modelist,12);
		if (n==4) bcfg.modelist=gtt_bit(bcfg.modelist,13);
		if (n==5) bcfg.modelist=gtt_bit(bcfg.modelist,14);
		if (n==6) bcfg.maxline =get_num(bcfg.maxline , 2);
		if (n==7) bcfg.modelist=gtt_bit(bcfg.modelist,19);
		if (n==8) bcfg.list_x  =get_num(bcfg.list_x  , 2);
		if (n==9) bcfg.list_y  =get_num(bcfg.list_y  , 2);
        if (n==10) bcfg.modelist=gtt_bit(bcfg.modelist,22);
	}
}


//��� ���� ����
set_quest()
{
	while (1) {
		cls();
		print_(0,"��� ���� ��� ����");
		printf(" 1. �޴�     ������ ����                      :%s\n",rbit(bcfg.qmode, 1,"����","����"));
		printf(" 2. �Խù�   ȭ�� ����� ����                 :%s\n",rbit(bcfg.qmode, 2,"����","����"));
		printf(" 3. �Խù�   �����Խ� ��� ����               :%s\n",rbit(bcfg.qmode, 3,"����","����"));
		printf(" 4. ����޴� ȭ�� ����� ����                 :%s\n",rbit(bcfg.qmode, 4,"����","����"));
		printf(" 5. �Խù�   ȭ�� ����� ����                 :%s\n",rbit(bcfg.qmode, 5,"����","����"));
		printf(" 6. �Խù�   �����Խ� ��� ����               :%s\n",rbit(bcfg.qmode, 6,"����","����"));
		printf(" 7. ����� �⺻���� ���ۿ���                  :%s\n",rbit(bcfg.qmode, 7,"���","����"));
		printf(" 8. ����� �⺻���� ��������                  :%s\n",rbit(bcfg.qmode, 8,"����","����"));
		printf(" 9. ����� �������� ���ۿ���                  :%s\n",rbit(bcfg.qmode, 9,"����","���"));
		printf("10. ����� �������� ��������                  :%s\n",rbit(bcfg.qmode,10,"����","����"));
		printf("11. ����� �ܺ� �⺻������                  :%s\n",rbit(bcfg.qmode,11,"����","�ܺμ���"));
		n=numselect();
        if (n<=0) break;
		if (n== 1) bcfg.qmode=gtt_bit(bcfg.qmode, 1);
		if (n== 2) bcfg.qmode=gtt_bit(bcfg.qmode, 2);
		if (n== 3) bcfg.qmode=gtt_bit(bcfg.qmode, 3);
		if (n== 4) bcfg.qmode=gtt_bit(bcfg.qmode, 4);
		if (n== 5) bcfg.qmode=gtt_bit(bcfg.qmode, 5);
		if (n== 6) bcfg.qmode=gtt_bit(bcfg.qmode, 6);
		if (n== 7) bcfg.qmode=gtt_bit(bcfg.qmode, 7);
		if (n== 8) bcfg.qmode=gtt_bit(bcfg.qmode, 8);
		if (n== 9) bcfg.qmode=gtt_bit(bcfg.qmode, 9);
		if (n==10) bcfg.qmode=gtt_bit(bcfg.qmode,10);
		if (n==11) bcfg.qmode=gtt_bit(bcfg.qmode,11);
	}
}



//�����ݴ����
set_ok()
{
	char buff[200];
	while (1) {
		cls();
		print_(0,"��� ���� ��� ����");
		printf(" 1. ���� �ݴ� ���� ����                       :%s \n",rbit(bcfg.modeok,1,"����","��"));
		printf(" 2. ���� �ݴ� ���� ���� �߻�                  :%s \n",rbit(bcfg.modeok,2,"�������","���Ǽ���"));
		printf(" 3. ��ǥ�� ���� ��� ���� �ֱ�                :%s \n",rbit(bcfg.modeok,3,"������","����"));
		printf(" 4. ��� ���                                 :%s \n",rbit(bcfg.modeok,3,"���","����"));
		printf(" 5. ���� ���� �ȳ� ȭ�� ����                       :(logo/%s.ok1)\n",go.code);
		printf("11. ���� ���� ���� ����                       :%s \n",rbit(bcfg.modeok,11,"����","��"));
		printf("12. ���� ���� ���� ���� �߻�                  :%s \n",rbit(bcfg.modeok,12,"�������","���Ǽ���"));
		printf("13. ��ǥ�� ���� ��� ���� �ֱ�                :%s \n",rbit(bcfg.modeok,13,"������","����"));
		printf("14. ��� ���                                 :%s \n",rbit(bcfg.modeok,13,"���","����"));
		printf("15. ���� ���� �ȳ� ȭ�� ����                       :(logo/%s.ok2)\n",go.code);
		n=numselect();
        if (n<= 0) break;
		if (n== 1) bcfg.modeok=gtt_bit(bcfg.modeok, 1);
		if (n== 2) bcfg.modeok=gtt_bit(bcfg.modeok, 2);
		if (n== 3) bcfg.modeok=gtt_bit(bcfg.modeok, 3);
		if (n== 4) bcfg.modeok=gtt_bit(bcfg.modeok, 4);
		if (n== 5) {
			sprintf(buff,"%s/logo/%s.ok1",club_go.dir,go.code);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (n==11) bcfg.modeok=gtt_bit(bcfg.modeok,11);
		if (n==12) bcfg.modeok=gtt_bit(bcfg.modeok,12);
		if (n==13) bcfg.modeok=gtt_bit(bcfg.modeok,13);
		if (n==14) bcfg.modeok=gtt_bit(bcfg.modeok,14);
		if (n==15) {
			sprintf(buff,"%s/logo/%s.ok2",club_go.dir,go.code);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
	}
}



set_formcfg()
{
	int n,i,n1,n2;
	char buff[100];
	while (1) {
		cls();
		print_(0,"�Խ��� ��� ���� ����");
        printf("  1. �׸� ���� ĭ�� ����  (FRM,DSP���� �����)\n");
        printf("  2. LX �˻�(�����׸�)�� �˻���ġ,���� ����       \n");
		printf("  3. ���� ��� ��ġ                            :(X:%2d Y:%2d) \n",bcfg.disp_x,bcfg.disp_y);
        printf("  4. ���� ��� �ټ�    ���� (0.���� 1-99.�ټ�) :%d\n",bcfg.listline);
        printf("  5. ���� ���                                 :%s\n",rbit(bcfg.modelist,0,"��","����"));
        printf("  6. ������ ����->ansiȭ������                 :%s\n",rbit(bcfg.mode,24,"����","�Ƚ�"));
		printf("  7. ���� ��� / �ϴ�   ��������ġ             :(Y1:%2d Y2:%2d) \n",bcfg.pos_y1,bcfg.pos_y2);
        printf("  8. ������ ������� ���ƿ�                    :%s\n",rbit(bcfg.modelist,1,"��","����"));
        printf("  9. ������ enter �Է� ����                    :%s\n",rbit(bcfg.modelist,2,"����","����"));
        printf(" 10. ����� �ټ��ʰ��� ENTER                   :%s\n",rbit(bcfg.modelist,3,"����","����"));
		printf(" 11. �ϴܸ�ɾ� ������ġ (X,Y��ǥ)             :(X:%2d Y:%2d) \n",bcfg.cmd_x,bcfg.cmd_y);
        printf(" 13. �Խù� ������ ���1 ���                  :%s\n",rbit(bcfg.mode,16,"����","����"));
        printf(" 14. �Խù� ������ ���2 ���                  :%s\n",rbit(bcfg.mode,17,"����","����"));
		printf(" 15. �Խù� ����� ���� 1ĭ ����               :%s\n",rbit(bcfg.mode,10,"����","����"));
		printf(" 16. �Խù� �⺻ ȭ����½�  �Խ���:�ڷ����� :%s\n",rbit(bcfg.modelist,15,"����","���"));
		printf(" 17. �Խù� �⺻ ȭ����½�  �Խ���:�ڷ����� :%s\n",rbit(bcfg.modelist,16,"����","���"));
		printf(" 18. �Խù� �⺻ ȭ����½�  �ڳ�  :�ּ�  ��� :%s\n",rbit(bcfg.modelist,17,"����","���"));
		printf(" 19. �Խù� �⺻ ȭ����½�  �ܺ�  :���  ��� :%s\n",rbit(bcfg.modelist,18,"����","���"));
		printf(" 20. �Խù� ����,�Ӻ� ��� ���                :%s\n",rbit(bcfg.mode,20,"���","�ź�"));
        printf(" 21. �޴� �ϴ��� ��ɾ� ���� ȭ�� (���)       :%s\n",rbit(bcfg.mode,22,"����","����"));
        printf(" 22. �޴� �ϴ��� ��ɾ� ���� ȭ�� (�б�)       :%s\n",rbit(bcfg.mode,23,"����","����"));
        printf(" 23. ��õ�ڷ� * ǥ�� ����                      :%s\n",rbit(bcfg.modelist,8,"����","����"));
        printf(" 24. �Խ��� Ư�� ��� ����                     :%d\n",bcfg.bform);
		n=numselect();
		xy(50,n+1);
        if (n<= 0) break;
		if (n== 2) reset_search();
		if (n== 3) {bcfg.disp_x=get_num(bcfg.disp_x,2);bcfg.disp_y=get_num(bcfg.disp_y,2);}
		if (n== 4) bcfg.listline=get_num(bcfg.listline,2);
		if (n== 5) bcfg.modelist=gtt_bit(bcfg.modelist,0);
		if (n== 6) bcfg.mode=gtt_bit(bcfg.mode,24);
		if (n== 7) {bcfg.pos_y1=get_num(bcfg.pos_y1,2);bcfg.pos_y2=get_num(bcfg.pos_y2,2);}
		if (n== 8) bcfg.modelist=gtt_bit(bcfg.modelist,1);
		if (n== 9) bcfg.modelist=gtt_bit(bcfg.modelist,2);
		if (n==10) bcfg.modelist=gtt_bit(bcfg.modelist,3);
		if (n==11) {bcfg.cmd_x=get_num(bcfg.cmd_x,2);bcfg.cmd_y=get_num(bcfg.cmd_y,2);}
		if (n==13) bcfg.mode=gtt_bit(bcfg.mode,16);
		if (n==14) bcfg.mode=gtt_bit(bcfg.mode,17);
		if (n==15) bcfg.mode=gtt_bit(bcfg.mode,10);
		if (n==16) bcfg.modelist=gtt_bit(bcfg.modelist,15);
		if (n==17) bcfg.modelist=gtt_bit(bcfg.modelist,16);
		if (n==18) bcfg.modelist=gtt_bit(bcfg.modelist,17);
		if (n==19) bcfg.modelist=gtt_bit(bcfg.modelist,18);
		if (n==20) bcfg.mode=gtt_bit(bcfg.mode,20);
        if (n==21) bcfg.mode=gtt_bit(bcfg.mode,22);
        if (n==22) bcfg.mode=gtt_bit(bcfg.mode,23);
        if (n==23) bcfg.modelist=gtt_bit(bcfg.modelist,8);
        if (n==24) select_bform();

	}
}


//�Խ��Ǵ��� ��������
view_rank(int mode,char *_str1,char *_str2)
{
    FILE *fp,*fp2;
    char buff[500],buff1[500],buff2[500],tstr[100];
	char filename[500];
    char a_nid[500][10],a_ntty[500][10],a_nhost[500][30];
    char a_ncode[500][10],a_nclub[500][10];
    char st[30][100];
    int c_nid,c_ntty,c_nhost,c_ncode,c_nclub,re_aload=Yes;
    int i,n,n1,n2,nn,count,pr1;
	int it1,it2,it3;
	int year,month,day1,day2,job1,job2,hour1,hour2,type1,type2,hostno1,hostno2;
	char tid[10],tcode[10],tclub[10],thost[30],ttty[30];
    char tfile[30],tid2[10],tip[30];
    time_t date1,date2;
    int tot[20];
	pr1=10;
	it1=it2=it3=0;
    c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;
    date1=0;
    time(&date2);
    year=now_year()%100;
	month=now_month()%100;
	day1=day2=job1=job2=hour1=hour2=type1=type2=hostno1=hostno2=0;
    strcpy(tclub,_str1);
    strcpy(tcode,_str2);
    while (1) {
        del_space(thost);del_space(ttty);del_space(tclub);del_space(tcode);
        del_space(tfile);del_space(tid);del_space(tid2);del_space(tstr);
        a_num[4]=hostno1;a_num[5]=hostno2;
        a_num[1]=year;a_num[2]=month;
        a_num[6]=day1;a_num[7]=day2;
        a_num[8]=hour1;a_num[9]=hour2;
        a_num[10]=type1;a_num[11]=type2;
		strcpy(g_buff3,tclub);
		strcpy(g_buff4,tcode);
        a_num[12]=job1;a_num[13]=job2;
        a_num[14]=pr1;

        print_(0,"�Խ��� ��������");
        printf("  1. �Խù� ��� �ð� ����                     :%s\n",datestr(1,date1));
        printf("  2. �Խù� ��� �ð� ��                       :%s\n",datestr(1,date2));
        printf("  9. �Խù� ���� ����\n");
		n=numselect();
		xy(50,n+1);
        if (n<=0) break;
        if (n==1) date1=dateinput0(date1);
        if (n==2) date2=dateinput0(date2);
        if (n==9) {
            for (i=0;i<20;i++) tot[i]=0;
            count=0;
            sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
            sprintf(NOW_FILED,"%s/menu/%s.tit",club_go.dir,go.code);
            if ((fp=fopen(NOW_FILE,"r"))==NULL) break;
            if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
            while (fread(&bbs,sizeof_bbs,1,fp)) {
                if (bbs.date<date1||bbs.date>date1) continue;
                if (bbs.del) tot[7]++;
                else {
                    tot[2]++;
                    tot[3]+=bbs.line/20;
                    tot[4]+=bbs.size;
                    tot[5]+=bbs.read;
                    tot[6]+=bbs.down;
                    tot[8]+=bbs.filesize;
                }
                del_enter(buff);
            }
            /*
            ����ü   �ο��� :     19 ��    tot[1]
            ����ü   ��  �� :     56 ��    tot[2]
            ����ü   ��  �� :     76 ��    tot[3]
            ����ü   ��  �� :     76 KB    tot[4]
            ����ü   ��ȸ�� :   6391 ��    tot[5]
            ����ü   �ٿ�� :   6391 ��    tot[6]
            �������� ��  �� :      1 ��    tot[7]
            */
            fclose(fp);
            fclose(fp2);
            if (count<1) {
                mess("�ش�Ǵ� ���ӱ���� �����ϴ�.");
                unlink(TMP_FILE);
                continue;
            }
            sprintf(buff,"sort +4 -b -o%s %s",TMP_FILE2,TMP_FILE);
            system(buff);
            view_text(TMP_FILE2);

            unlink(TMP_FILE);
            unlink(TMP_FILE2);
		}
	}
}


/*
    else if (mode==10) view_log3(0,0) ;  //�޴�
    else if (mode==11) view_log3(5,0) ;  //����
    else if (mode==12) view_log3(7,0) ;  //�б�
    else if (mode==13) view_log3(6,0) ;  //��ϼ�
    else if (mode==13) view_log3(6,1) ;  //��Ͽ뷮
    else if (mode==14) view_log3(8,0) ;  //�ٿ�
    else if (mode==15) view_log3(23,0);  //���Ӽ�
    else if (mode==16) view_log3(23,1);  //���ӽð�
*/

//��������
rank3(int mode,char *tclub,char *tcode)
{
    FILE *fp,*fp2,*fp3;
    char buff[500],buff1[500],buff2[500],tstr[100];
    char buff3[500],buff4[500],buff5[500],buff6[500],nowbuff[100];
	char filename[500];
    char st[30][100];
    int i,n,n1,n2,nn,count,year,month,day;

    if (!find_club(tclub)) return;
    set_nowmenu();       //����Ÿȭ�������ϱ�
    if (!find_go(tcode)) return;    //ã�� �����ڵ尡 ����
    read_ccfg();
    set_bcfgfile();                     //BCFG_FILE�� ����
    read_bcfg();

	year=now_year()%100;
	month=now_month()%100;

    sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
    printf("\n   ... ��� ��ٷ� �ּ���.. \n");
    printf("   [%s] �˻��� �����մϴ�... \n",filename);

    sprintf(tstr," %-8s %-8s ",club_go.code,go.code);
    sys_tem2("grep '%s' '%s'",tstr,filename,TMP_FILE);
//  sys_tem2("grep '%s' '%s' | sort +4 -b -o%s",tstr,filename,TMP_FILE);
    view_text(TMP_FILE);
    return;


         if (mode== 1) strcpy(tstr," 5 ����1");
    else if (mode== 2) strcpy(tstr," 7 �б�1");
    else if (mode== 3) strcpy(tstr," 6 ���1");
    else if (mode== 4) strcpy(tstr," 6 ���1");  //�뷮
    else if (mode== 5) strcpy(tstr," 8 �ٿ�1");
    else if (mode== 6) strcpy(tstr,"23 ����2");
    else if (mode== 7) strcpy(tstr,"23 ����2");  //�ð�
    else return;
    if (!is_file(filename)) {mess("%s ��� ȭ���� �����ϴ�.",filename);return;}

    sprintf(buff,"grep '%s' %s | sort +4 -b -o%s",tstr,filename,TMP_FILE);
    system(buff);
    if ((fp2=fopen(TMP_FILE,"r"))==NULL) return;
    if ((fp3=fopen(TMP_FILE2,"w"))==NULL) return;
    count=0;
    strcpy(nowbuff,"");
    n1=n2=0;
    while (fgets(buff,200,fp2)!=NULL) {
        if (strlen(buff)<40||buff[0]==';') continue;
        strncpy(buff2,&buff[30],3);  //type
        if (atoi(buff2)<1||atoi(buff2)>17) continue;
        nn++;
        count++;
        xstrncpy(buff2,&buff[21],8);  //ID
        del_space(buff2);
        if (!is_same(nowbuff,buff2)) {
            if (strlen(nowbuff)>2&&n1>0) fprintf(fp3,"%-8s%8d%8d\n",nowbuff,n1,n2);
            strcpy(nowbuff,buff2);
            n1=n2=0;
        }
        if (mode==23) {
            strcpy(buff3,&buff[95]);  //���ð�
            n2+=atoi(buff3);
        }
        if (mode==13||mode==14) {
            strcpy(buff3,&buff[102]);  //���ð�
            sscanf(buff3,"%s%s%s",buff4,buff5,buff6);
            n2+=atoi(buff5);
        }
        n1++;
    }
    if (strlen(nowbuff)>2&&n1>0) fprintf(fp3,"%-8s%8d%8d\n",nowbuff,n1,n2);
    fclose(fp2);
    fclose(fp3);
    if (count<1) {
        mess("�ش�Ǵ� ���ӱ���� �����ϴ�.");
        return;
    }
    sprintf(buff,"sort +1 -b -o%s %s",TMP_FILE,TMP_FILE2);
    system(buff);
    view_text(TMP_FILE2);
    view_text(TMP_FILE);

    unlink(TMP_FILE);
    unlink(TMP_FILE2);
}



/*

����ȣȸ    : main
�������ڵ�  : hello
�������̸�  : �����λ�
����� �Ⱓ : ����������������������
������ϼ�  :  24 ��
�������    : 1999.07.03
����ü   �ο��� :     19 ��    tot1
����ü   ��  �� :     56 ��    tot2
����ü   ��  �� :     76 ��    tot3
����ü   ��  �� :     76 KB    tot4
����ü   ��ȸ�� :   6391 ��    tot5
����ü   �ٿ�� :   6391 ��    tot6
�������� ��  �� :      1 ��    tot7
������� ��  �� :    2.3 ��
������� ��  �� :    2.3 ��
������� ��  �� :    2.3 ��
������� ��ȸ�� :  114.1 ��
������� �ٿ�� :  114.1 ��


��Best10 ��ȸ
��  9266 �輺��   Kiev     06-01  199  1 [����] �̸� ����, Ǯ�Ѹ� 7�ֳ�.
��  9267 ������   Value    06-01  198  1 [�ٹ�] �ݰ����ϴ�..
��  9268 Ǯ�Ѹ�   k2pbri1  06-02  186  1 [�ű�] 6�� 2���� ����ȸ�� ���
��Best10 �ٿ�
��  9266 �輺��   Kiev     06-01  199  1 12345678.123 [����] �̸� ����, Ǯ�Ѹ� 7�ֳ�.
��  9267 ������   Value    06-01  198  1 12345678.123 [�ٹ�] �ݰ����ϴ�..
��  9268 Ǯ�Ѹ�   k2pbri1  06-02  186  1 12345678.123 [�ű�] 6�� 2���� ����ȸ�� ���
��
������ �� �ۼ� *1  + ��(��)�� *2  + ��ȸ�� *2  + Ȱ���� *0  + ����� *0    ��
��
������   ����
�� ����   ���̵�   ��  ��   �ۼ�   �ʼ�   �뷮    ��ȸ   �ٿ�   ����  �� ��
��    1  STUDIO24  ������     16     23   1611M   1611   1611     16   3284
��    2  k2pbri1   Ǯ�Ѹ�     13     13   1416M   1416   1416     13   2871
��    3  �ķ���    ���ҿ�      7      7    857M    857    857      7   1735
�� �Ѱ�  ??????��           ????    ???   ????M   ????
��
��
������   �ۼ�
�� ����   ���̵�   ��  ��   �ۼ�   �ʼ�   �뷮    ��ȸ   �ٿ�   ����  �� ��
��    1  STUDIO24  ������     16     23   1611M   1611   1611     16   3284
��    2  k2pbri1   Ǯ�Ѹ�     13     13   1416M   1416   1416     13   2871
��    3  �ķ���    ���ҿ�      7      7    857M    857    857      7   1735
��
������   �ʼ�
�� ����   ���̵�   ��  ��   �ۼ�   �ʼ�   �뷮    ��ȸ   �ٿ�   ����  �� ��
��    1  STUDIO24  ������     16     23   1611M   1611   1611     16   3284
��    2  k2pbri1   Ǯ�Ѹ�     13     13   1416M   1416   1416     13   2871
��    3  �ķ���    ���ҿ�      7      7    857M    857    857      7   1735
��
������   �뷮
�� ����   ���̵�   ��  ��   �ۼ�   �ʼ�   �뷮    ��ȸ   �ٿ�   ����  �� ��
��    1  STUDIO24  ������     16     23   1611M   1611   1611     16   3284
��    2  k2pbri1   Ǯ�Ѹ�     13     13   1416M   1416   1416     13   2871
��    3  �ķ���    ���ҿ�      7      7    857M    857    857      7   1735
��
������   ��ȸ
�� ����   ���̵�   ��  ��   �ۼ�   �ʼ�   �뷮    ��ȸ   �ٿ�   ����  �� ��
��    1  STUDIO24  ������     16     23   1611M   1611   1611     16   3284
��    2  k2pbri1   Ǯ�Ѹ�     13     13   1416M   1416   1416     13   2871
��    3  �ķ���    ���ҿ�      7      7    857M    857    857      7   1735
��
������   �ٿ�
�� ����   ���̵�   ��  ��   �ۼ�   �ʼ�   �뷮    ��ȸ   �ٿ�   ����  �� ��
��    1  STUDIO24  ������     16     23   1611M   1611   1611     16   3284
��    2  k2pbri1   Ǯ�Ѹ�     13     13   1416M   1416   1416     13   2871
��    3  �ķ���    ���ҿ�      7      7    857M    857    857      7   1735
��
������   ����
�� ����   ���̵�   ��  ��   �ۼ�   �ʼ�   �뷮    ��ȸ   �ٿ�   ����  �� ��
��    1  STUDIO24  ������     16     23   1611M   1611   1611     16   3284
��    2  k2pbri1   Ǯ�Ѹ�     13     13   1416M   1416   1416     13   2871
��    3  �ķ���    ���ҿ�      7      7    857M    857    857      7   1735
��
  �Ϻ�����Ȳ
  ��¥    �ۼ�  �ʼ�  �뷮��
  1�� : �Ǽ�
������� ��  �� :    2.3 ��
������� ��  �� :    2.3 ��
������� ��  �� :    2.3 ��


07-01 00:16  77 p2   �û�     19 main     pdrv     21 ��IP1 kslab                          STA 0
07-01 00:17  77 p2   �û�     19 main     pds      21 ��IP1 kslab                          END 65
07-01 00:18  77 p3   guestp3   0 main     top      23 ����1 210.114.25.95                  STA 0
07-01 00:18  77 p3   guestp3   0 main     pgame    21 ��IP1 210.114.25.95                  STA 0
07-01 00:19  77 p3   guestp3   0 main     pgame     7 �б�1 210.114.25.95                  B flame80  [GAM] ����1
07-01 00:19  77 p3   guestp3   0 main     pgame     8 �ٿ�1 210.114.25.95                  P flame80  civ.arj 1457126 [GAM
07-01 00:19  77 p3   guestp3   0 main     pgame    21 ��IP1 210.114.25.95                  END 70
07-01 00:19  77 p3   guestp3   0 main     pgame    23 ����2 210.114.25.95                  END 82

*/
