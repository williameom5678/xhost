////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���� : �û�޴� ���� ��ƾ
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include "chat.h"

int n,nn,n2,num;
int top;
int total;
int last;
int key;
long lastnow=0; 	  //������ �ð�
long lastnow2=0;	   //������ �ð�
int ast[20];
int disp_mode;
int mrand;
char ss_1[50];
char ss_2[50];
char ss_3[50];

char menu_file[200];
char filename[200];
char now_code[200];
char now_dir[200];
char now_id[9];

#include "sysoput.c"     //���Ϻ���

main(int argc, char **argv)
{
	char buff[90];
	char buff2[90];
	int key;
	int num,n,i;
	FILE *fp;
	struct termio term_old;
	who=0;
	c_proc=is_file("system/c_proc.tag");
	c_proc2=is_file("system/c_proc2.tag");
	c_proc3=is_file("system/c_proc3.tag");
	for(i=0;i<20;i++) ast[i]=0;

	printf("\n<%s>\n\n\n\n",VER1);
	fflush(NULL);

	if (!is_file(ID_FILE)) {   //root���� ���� �׳�exec
		mess("%s ������ �����ϴ�.",ID_FILE);
		if (yesno(" ID���丮�� ��ID������ ������?",Yes)) {
			check_dir("id");
			touch_file(ID_FILE);
			make_ind(0,"make_ind_all");
		}
		return;
	}


	check_init_dir();

	ioctl(0,TCGETA,&termold);	//���� �͹̳� ��� ����
	srand(time(NULL));			//������ �ʱ�ȭ
	mrand=rand()%10000000;
	club_mode=No;
	strcpy(club_go.dir,"./");
	strcpy(club_go.code,"main");
	setenv("TERM","vt100",1);
	setenv("LANG","ko",1);       //���� ����
	disp_mode=0;
	findport(tty);
	read_cfg();
	if (strlen(cfg.sysop)<3) strcpy(cfg.sysop,"sysop");
	set_tmpfile();
	club_mode=No;
	read_pftype();	//ȸ���� ���,���ϻ�밡�ɽð��� Ȯ���Ѵ�.
	read_atype();	//����б�
	read_ajob();	//�����б�
	if (argc<2) {
		if (!check_pass("zz",8,"PASS >>")) return;
	}

	if (argc<2||(argc==2&&strlen(argv[1])<3)) {
		printf("Usage:bin/sysop ????\n");
		if (yesno("�ε����������� �ұ��? (Y/n) >>",Yes)) {
			printf("\n �ε��������� �������մϴ�...\n");
			make_ind(0,"make_ind_all");
		}
		return;
	}
	else if (argc==2&&is_same(argv[1],"root")) {
		strcpy(now_id,cfg.sysop);
		printf("\n ### ���� ������ �û��� �����Դϴ�. ###\n");
		if (!read_pf(cfg.sysop)) {
			printf(" sysop���� ������ '%s' ID�� ã���� �����ϴ�.\n",cfg.sysop);
			if (yesno("�ε����������� �ұ��? (Y/n) >>",Yes)) {
				printf("\n �ε��������� �������մϴ�...\n");
				make_ind(0,"make_ind_all");
			}
		}
		strcpy(pf.id,"sysop");
		pf.type=19;
		who=5;
	}
	else {
		strcpy(now_id,argv[1]);
		if (!read_pf3(now_id)) {
			mess("%s ID�� ã���� �����ϴ�.",now_id);
			if (yesno("�ε����������� �ұ��? (Y/n) >>",Yes)) {
				make_ind(0,"make_ind_all");
				printf("\nȸ�� ���̵� �ε��� ȭ�� ������ �Ϸ�\n");
			}
			goto _quit;
		}
		if(pf.type<16) {
			mess("%s �û�޴� ���ɵ���� �ƴմϴ�.",now_id);
			goto _quit;
		}
		who=0;
			 if (is_same(cfg.sysop,now_id)) who=4;
		else if (pf.type==19)				who=4;	//root
		else if (pf.type==18)				who=3;	//�νû�
		else if (pf.type==17)				who=2;	//���
		if (who<2||who>5) {
			mess("%s���� �û�޴��� ����Ҽ� ���� ����Դϴ�.",now_id);
			goto _quit;
		}
	}

	rawmode();	// �� ĳ���Į ���

	stat_pf();	//ȸ���ڷ� Ȯ��
	while (1) {
		cls();
		xy(1,1);
		printf(" ��������������������������������������������������\n");
		if (who==5) printf(" ��[7m  %-22s## �� ���� �û� �޴�  [m��\n",cfg.hostname);
		else		printf(" ��[7m  %-22s  %8s W:%d  #:%3d [m��\n",cfg.hostname,now_id,who,cfg.hostno);
		printf(" ��������������������������������������������������\n");
		printf(" ��   1. ȸ�����        ��  21. �ý���ȯ�漳��  ��\n");
		printf(" ��   2. ȸ������/����   ��  22. ����/����/���  ��\n");
		printf(" ��   3. ���/����/����  ��  23. ��Ʈ/���/�з�  ��\n");
		printf(" ��                      ��  24. �ڷ�� ��/�ٿ�  ��\n");
		printf(" ��   5. �մԼ���        ��  25. �Խ���,������   ��\n");
		printf(" ��   6. ���Լ���        ��  26. ȭ��Խ���      ��\n");
		printf(" ��   7. ��������        ��  27. �Խ��� �ڵ���õ ��\n");
		printf(" ��   8. �޴�����        ��  28. �ý��� ���     ��\n");
		printf(" ��   9. ��ƿ��Ƽ        ��  29. �������׼���    ��\n");
		printf(" ��������������������������������������������������\n");
		printf(" ��  11. ��ȣȸ          ��  31. ���ٿ��Ķ��Ÿ  ��\n");
		printf(" ��  12. ȭ�����        ��  32. ��ü�ڷ�Ǽ���  ��\n");
		printf(" ��  13. ���� ����       ��  33. �������۾�    ��\n");
		printf(" ��  14. ��ȭ�Ǽ���      ��  34. ���������ü���  ��\n");
		printf(" ��  15. �޼�������      ��                      ��\n");
		printf(" ��  16. ����/�������   ��                      ��\n");
		printf(" ��  17. ���/�Է°���   ��                      ��\n");
		if (who==5)
		printf(" ��[7m  98. �û��ɾ�����  [m��[7m  99. ������û���  [m��\n");
		printf(" ��������������������������������������������������\n");
		printf(" ���ù�ȣ? (����:P �������:N ����:?) >>");
		lineinput(buff,10);check_han(buff);printf("\n");num=atoi(buff);
		if (is_same(buff,"n")) goto _quit;
		if (is_quit(buff)) break;
		if (num== 1) sys_tem("bin/pfinfo '%s'",who==5?"root":now_id);
		if (num== 2) job_pf();
		if (num== 3) job_type();
		if (num== 5) job_guest();
		if (num== 6) job_guestinput();
		if (num== 7) job_idout();
		if (num== 8) job_menu();
		if (num== 9) job_util();
		if (num==11) job_club();
		if (num==12) job_screen();
		if (num==13) job_mail();
		if (num==14) job_chat();
		if (num==15) job_mess();
		if (num==16) job_start();
		if (num==17) job_cmd();
		if (num==21) job_system();
		if (num==22) job_limit();
		if (num==23) job_node();
		if (num==24) job_updown();
		if (num==25) job_setbbs();
		if (num==26) job_jpg();
		if (num==27) job_good();
		if (num==28) job_backmode();
		if (num==29) job_notice();
		if (num==31) job_para();
		if (num==32) job_zbtotal();
		if (num==33) job_stat();
		if (num==34) job_info();
		if (who==5&&num==98) edit_syscmd();
		if (who==5&&num==99) job_sysops();
	}
	save_cfg();
	_quit:
	ioctl(0,TCSETA,&termold);	//���� �͹̳� ��� ����
}


job_util()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"��ƿ��Ƽ�۾�");
		printf("  1. ��ǥ�� �۾� \n");
		printf("  2. ���� �Ӻ�   \n");
		printf("  3. ���̹���    \n");
		printf("  4. ���̹�Ȩ    \n");
		printf("  5. ���̹��Ӵ�  \n");
		printf("  6. IP(��������)\n");
		printf("  7. ��� ���� ����(today mess)����\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) job_vote();
		if (n== 2) job_news();
		if (n== 3) job_war();
		if (n== 5) job_money();
		if (n== 6) job_ip();
		if (n== 7) job_todaymess();
	}
}




//ȸ�� ���� �۾�
job_stat()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"��� ���� �۾�");
		printf("  1. �Ϻ� ȸ�� ������Ȳ                        \n");
		printf("  2. ȸ  ��   �� ��\n");
		printf("  3. ���   �� ��\n");
		printf("  4. �Ϲݱ�� �� ��\n");
		printf("  5. ���� �� ��\n");
		printf("  6. ���ӱ�� �� ��\n");
		printf("  7. ���ӱ�� �� ��2\n");
		printf("  8. �ٿ��� �� ��\n");
		printf("  9. ��ȣȸ����� �� ��\n");
		printf(" 99. ȸ�� ���\n");
		xy(1,20);
		n=numselect();
		printf("\n\n");
		if (n<= 0) break;
		if (n== 1) view_daypf();
		if (n== 2) view_stat(1);
		if (n== 3) view_stat(2);
		if (n== 4) sys_tem("bin/viewlog 2 '%s' main",cfg.sysop);
		if (n== 5) sys_tem("bin/viewlog 5 '%s' main",cfg.sysop);
		if (n== 6) sys_tem("bin/viewlog 6 '%s' main",cfg.sysop);
		if (n== 7) sys_tem("bin/viewlog 62 '%s' main",cfg.sysop);
		if (n== 8) sys_tem("bin/viewlog 7 '%s' main",cfg.sysop);
		if (n== 9) sys_tem("bin/viewlog 8 '%s' main",cfg.sysop);
		if (n==99) {disp_set_stat();pressenter();}		  //���� ���� �����ֱ�
	}
}

view_stat(int mode)
{
	FILE *fpid;
	FILE *fpsave;
	int  n,count,i,i2,i3;
	struct tm *tp;

	total=count_pf();				   //ȸ�� �� �� ���
	fpid = fopen(ID_FILE, "r+");
	fpsave = fopen(TMP_FILE, "w+");

	fprintf(fpsave,"          ��ȸ��:%5d��         \n",total);

	fseek(fpid,0,SEEK_SET);
	for (n=0;n<20;n++) {
		if (mode==1&&n<14) continue;
		count=0;
		fprintf(fpsave,"   ==========   ���� %4d  ========== \n",n);
		fseek(fpid,0,SEEK_SET);
		while(fread(&pf,sizeof_pf,1,fpid)) {		 //���̵� ����
			if (strlen(pf.id)<3) continue;
			if (pf.type==n) {
				tp = localtime(&pf.login);
				fprintf(fpsave," %3d.  %-8s %-8s %5d %5d %5d %5d ",
				count+1,pf.id, pf.name, pf.level,pf.countw,pf.countu,pf.countd);
				fprintf(fpsave,"%02d/%02d/%02d\n",
				tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
				count++;
			}
		}
		fprintf(fpsave,"                       == �� %4d�� == \n",count);
	}

	fclose(fpid);
	fclose(fpsave);
	view_text(TMP_FILE);
	unlink(TMP_FILE);
}



//ȸ�� ���� �۾�
job_pf()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"ȸ�� ���� �۾�");
		printf("  1. ��ü ȸ�� �˻�          (�������)        \n");
		printf("  2. ID/���� ���� �˻�/����                    \n");
		printf("  3. ȸ�� �˻�,����          (�з����˻�)      \n");
		printf("  4. ���Դ�� ȸ�� ����ó��  (�ű԰����㰡 %d����)\n",count_rec(GUEST_FILE   ,sizeof_pf));
		printf("  5. ���Դ�� ȸ�� ���� ����ó��\n");
		printf("  6. ��ϰź� ȸ�� �߰�,���� (guestnot.dat %d��ź�)\n",count_rec(GUESTNOT_FILE,sizeof_pf));
		printf("  7. ȸ�� ������/����ó��    (��� 0 ȸ��ó��) \n");
		printf("  8. �ű� ȸ�� ���̵� �����                   \n");
		printf("  9. ���� ��ȣ ���̵� �������� (�˻�����ID����)\n");
		printf(" 10. ȸ�� ���̵� �ε��� ȭ�� ������\n");
		printf(" 11. ȸ�� READ �� �����˻� (���н�->��ü�˻�) %s\n",rbit(cfg.pfmode,1,"�Ϲݰ˻�","�����˻�"));
		printf(" 12. ȸ���Ż�(PF)                             %s\n",rbit(cfg.pfmode,21,"�⺻","�����"));
		printf(" 13. ���ѽð�,��¥����� ����                 %s\n",rbit(cfg.mode,1,"����","������"));
		printf(" 14. ���Գ�¥��������(ȸ��)�ǽ�               %s\n",rbit(cfg.mode,3,"����","�ǽ�"));
		printf(" 15. ������ ���̵� ����                       system/testid.id\n");
		printf(" 16. ���/���Ա��� ���̵� ����                system/notlog.id\n");
		printf(" 17. ����Ȯ�ξ��̵�(user/rec/log_id.rec)      system/monilog.id\n");
		printf(" 18. ������ ȸ�� ��ü ���� ���0 -> ���1��  \n");
		printf(" 19. ���� ��� ȸ�� ��ȸ (���  0�� ȸ��)    \n");
		printf(" 20. �� �� ��  ȸ�� ��ȸ (��� 16�̻��� ȸ��)\n");
		printf(" 21. ��޺� ������ ���� ����                 (system/auto_go.txt)\n");
        printf(" 22. ��üȸ�� ���,����,���ӽð�,���� ����   \n");
		printf(" 23. �մ�����ID (ȸ���˻�����:���0����)     (system/guestid.id)\n");
		printf(" 24. RANK(���) �� ����ó��(RK <-> RK2)       %s\n",rbit(cfg.levelmode,2,"��������","�Ⱥ���"));
		printf(" 25. IND  -> QUICK �˻�����                   %s\n",rbit(cfg.mprocmode,2,"QUICK","����"));
		n=numselect();printf("\n");
		if (n<= 0) break;
		if (n== 1) sys_tem("bin/pfinfo '%s'",who==5?"root":now_id);
		if (n== 2) sys_tem("bin/pf '%s' set_search",now_id);
		if (n== 3) sys_tem("bin/pf '%s'",now_id);
		if (n== 4) sys_tem("bin/guest 999");
		if (n== 5) sys_tem("bin/guest 998");
		if (n== 6) notfree();
		if (n== 7) sort_pf();	  //ȸ�� ������
		if (n== 8) add_new_pf();
		if (n== 9) id_num_edit();
		if (n==10) make_ind(0,"make_ind_all");
		if (n==11) cfg.pfmode = gtt_bit(cfg.pfmode,1);
		if (n==12) cfg.pfmode = gtt_bit(cfg.pfmode,21);
		if (n==13) cfg.mode 	  =gtt_bit(cfg.mode,1);
		if (n==14) cfg.mode 	  =gtt_bit(cfg.mode,3);
		if (n==15) xedit("system/testid.id");
		if (n==16) xedit("system/notlog.id");
		if (n==17) xedit("system/monilog.id");
		if (n==18) recover();
		if (n==19) check_pf(1,who);
		if (n==20) check_pf(2,who);
		if (n==21) auto_go();
		if (n==22) reset_all();
		if (n==23) xedit("system/guestid.id");
		if (n==24) cfg.levelmode=gtt_bit(cfg.levelmode,2);
		if (n==25) cfg.mprocmode=gtt_bit(cfg.mprocmode,2);
	}
}


//ȸ�� ���� �۾�
reset_all()
{
	int num,i,type1,type2;
	char buff[100];
	type1=type2=i=0;
	mess("�� ����� ��ü ȸ�� ���� ����Դϴ�. (�������)");
	while (1) {
		cls();
		print_(1,"��ü ȸ�� ���,���� ����");
		printf(" 1. ���� ��� ��� ����     %d ����  %d ��� \n",type1,type2);
		printf(" 2. ��ü ȸ�� ��� ������                    \n");
		printf(" 3. ��ü ȸ�� ���� ������                    \n");
		printf(" 4. ��ü ȸ�� ��ü���Ӽ� ������              \n");
		printf(" 5. ��ü ȸ�� �ݿ����Ӽ� ������              \n");
		printf(" 6. ��ü ȸ�� ��ü���ð�������             \n");
		printf(" 7. ��ü ȸ�� �ݿ����ð�������             \n");
		printf(" 8. ��ü ȸ�� �ֹε�Ϲ�ȣ ����              \n");
		printf(" 9. ��ü ȸ�� �ֹε�Ϲ�ȣ -> ��������       \n");
		printf("10. ��ü ȸ�� ����/���/�ٿ�� 0���� ����    \n");
        printf("11. ��ü ȸ�� WWW ����         0���� ����    \n");
        printf("12. ��ü ȸ�� CASH ����                      \n");
		n=numselect();printf("\n");
		if (n<= 0) break;
		if (n== 1) {
			printf("����� ����? >>");type1=get_num(type1,2);
			printf("����� ��  ? >>");type2=get_num(type2,2);
		}
		else if (n== 2) {
			printf("������ ���? >>");i=numselect2();
			if (i<1||i>14) mess(" ���ɵ�� 1~14");
			else make_reset(n,i,type1,type2);
		}
		else if (n== 3) {
			printf("������ ����? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 4) {
			printf("������ ��ü���Ӽ�? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 5) {
			printf("������ �ݿ����Ӽ�? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 6) {
			printf("������ ��ü���ð�? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 7) {
			printf("������ �ݿ����ð�? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
		else if (n== 8) {
			if (yesno("��ü ȸ���� �ֹε�� ��ȣ�� �����ұ��? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
		else if (n== 9) {
			if (yesno("�ֹε�Ϲ�ȣ�� ������ �����ұ��? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
		else if (n==10) {
			if (yesno("����/���/�ٿ���� 0���� �����ұ��? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
        else if (n==11) {
            if (yesno("WWW ������� 0���� �����ұ��? (y/N)>>",No))
				make_reset(n,0,type1,type2);
		}
        else if (n==12) {
			printf("������ CASH? >>");i=numinput();
			if (i>=0) make_reset(n,i,type1,type2);
		}
	}
}


make_reset(int mode ,int num,int type1,int type2)
{
	int i,n,n2,nn,count,stop=Yes;
	char sex;
	char buff[100];
	FILE *fp;
	if (!yesno2("\n ### ��ü ȸ���� �����ұ��? (y/N)>>",No)) return;
	if ((fp=fopen(ID_FILE,"r+"))==NULL) return;
	i=0;
	count=0;
	printf("\n");
	if (type1==0&&type2==0) {mess("���������� �����ϼ���");return;}
	while (1) {
		fseek(fp,i*sizeof_pf,SEEK_SET);
		if (i<0||!fread(&pf,sizeof_pf,1,fp)) break;
		if (pf.type<type1||pf.type>type2||pf.type>=18) goto _cont;
		printf("%-8s (%-8s) ",pf.id,pf.name);
			 if (mode==2) {printf("��޺��� %d -> %d\n",pf.type,num);pf.type=num;}
		else if (mode==3) {printf("�������� %d -> %d\n",pf.level,num);pf.level=num;}
		else if (mode==4) {printf("���Ӽ����� %d -> %d\n",pf.all_log,num);pf.all_log=num;}
		else if (mode==5) {printf("�������Ӽ����� %d -> %d\n",pf.month_log,num);pf.month_log=num;}
		else if (mode==6) {printf("��ü���ð����� %d -> %d\n",pf.all_time,num);pf.all_time=num;}
		else if (mode==7) {printf("�������ð����� %d -> %d\n",pf.month_time,num);pf.month_time=num;}
		else if (mode==8) {printf("�ֹε�ϻ��� %s -> ����\n",pf.id_no);strcpy(pf.id_no,"");}
		else if (mode==9) {
			sex=pf.sex;
			pf.sex=pf.id_no[7]=='2'?'f':'m';
			printf("��������     %c -> %c\n",pf.sex,sex);pf.sex=sex;
		}
		fseek(fp,i*sizeof_pf,SEEK_SET);
		pf.num=i;
//		  fwrite(&pf,sizeof_pf,1,fp);
		gwrite(202,pf,fp);
		count++;
		if (count%30==0&&stop) {
			printf("\n ���� %d �ڷ�ó����  ���(Enter) ����(Q) ����(S)>>",count);
			lineinput(buff,2);printf("\n");
			if (is_same(buff,"s")) stop=No;
			else if (is_quit(buff)) break;
		}
		else if (mode==10) {printf("����/���/�ٿ������\n");pf.countw=pf.countu=pf.countd=0;}
        else if (mode==11) {printf("WWW �������\n");pf.wwrite=0;}
        else if (mode==12) {printf("CASH ���� %d -> %d\n",pf.cash,num);pf.cash=num;}
		_cont:
		i++;
	}
	fclose (fp);
	mess("�� %d���� �ڷᰡ ����Ǿ����ϴ�.",count);
}


job_guest()
{
	while (1) {
		cls();
		print_(1,"�մ԰��� ����");
		printf("  1. �մ� ���� ���̵�           :%s\n",cfg.guestid    );
		printf("  2. �մ� �ڵ� ����             :%s\n",rbit(cfg.guestmode,15, "��������", "�ڵ�����"));
		printf("  3. �մ� ������ �ٷ� ����      :%s\n",rbit(cfg.guestmode,16, "����", "�ٷΰ���"));
		printf("  4. �մ� ���ӽ� ���� �Է�      :%s\n",rbit(cfg.guestmode,17, "�Է�", "���"));
		printf("  5. �մԽ� �Ϸ� ��ȣ           :%s\n",rbit(cfg.guestmode,18, "����", "��ȣ"));
		printf("  7. �մ� ���ӽ� ���񽺸޴���   :%s\n",rbit(cfg.guestmode,2,  "����", "����"));
		printf("  8. �մ� ���ӽ� ���Ե���     :%s\n",rbit(cfg.guestmode,6,  "����", "����"));
		printf("  9. �մ� ���ӽ� ����           :%s\n",rbit(cfg.guestmode,1,  "����", "����"));
		printf(" 10. �մ� SAY ��� ���         :%s\n",rbit(cfg.chatmode,10,  "����", "�Ұ�"));
		printf(" 11. �մ� MM  ��� ���         :%s\n",rbit(cfg.chatmode,11,  "����", "�Ұ�"));
		printf(" 12. �մ� ���� �� �ʱ� (����)   :%s\n",rbit(cfg.guestmode,8,  "���", "����"));
		printf(" 13. ���ӽ�Enter �ٷμմ�ó��   :%s\n",rbit(cfg.guestmode,21, "����", "�մ�"));
		printf(" 14. ���̵��Է� ����            :%s\n",rbit(cfg.guestmode,20, "����", "��������,���μմ�"));
		printf(" 15. �մ����ӽ� �ϴ� ���Ծȳ�   :%s\n",rbit(cfg.guestmode,9,  "�ȳ�", "����"));
		printf(" 16. ID�Է�5ȸ�ʰ��� ����ó��   :%s\n",rbit(cfg.guestmode,13, "�մ�����","����"));
		printf(" 17. �մ����ӽ� /xguest ����    :%s\n",rbit(cfg.guestmode,14, "�Ϲݸ޴�","�մ�����"));
		n=numselect();xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)					get_str(cfg.guestid  , 8);
		if (n== 2)	cfg.guestmode  =gtt_bit(cfg.guestmode,15);
		if (n== 3)	cfg.guestmode  =gtt_bit(cfg.guestmode,16);
		if (n== 4)	cfg.guestmode  =gtt_bit(cfg.guestmode,17);
		if (n== 5)	cfg.guestmode  =gtt_bit(cfg.guestmode,18);
		if (n== 6)	cfg.guestmode  =gtt_bit(cfg.guestmode, 5);
		if (n== 7)	cfg.guestmode  =gtt_bit(cfg.guestmode, 2);
		if (n== 8)	cfg.guestmode  =gtt_bit(cfg.guestmode, 6);
		if (n== 9)	cfg.guestmode  =gtt_bit(cfg.guestmode, 1);
		if (n==10)	cfg.chatmode   =gtt_bit(cfg.chatmode ,10);
		if (n==11)	cfg.chatmode   =gtt_bit(cfg.chatmode ,11);
		if (n==12)	cfg.guestmode  =gtt_bit(cfg.guestmode, 8);
		if (n==13)	cfg.guestmode  =gtt_bit(cfg.guestmode,21);
		if (n==14)	cfg.guestmode  =gtt_bit(cfg.guestmode,20);
		if (n==15)	cfg.guestmode  =gtt_bit(cfg.guestmode, 9);
		if (n==16)	cfg.guestmode  =gtt_bit(cfg.guestmode,13);
		if (n==17)	cfg.guestmode  =gtt_bit(cfg.guestmode,14);
	}
}


job_guestinput()
{
	int n,i;
	char buff[300];
	while (1) {
		cls();
		print_(0,"���� ���� ����");
		printf("  1. �մ� ���ӽ� ����������           %s\n",rbit(cfg.guestmode,3,"����","����"));
		printf("  2. �մ� ���ӽ� ������������         %s\n",rbit(cfg.guestmode,4,"����","����"));
		printf("  3. �մ� ���� �׸� ����              \n");
		printf("  4. ������ �ʱ� �⺻������ (���..)  :%d\n",cfg.guesttype  );
		printf("  5. �մ� ���� ��� ��ȣ ����         \n");
		printf("  9. ���� ���� ���̵� ����                       (system/notguest.txt)\n");
		printf(" 10. ���� ���� �ֹε�Ϲ�ȣ ����                 (system/bad_idno.txt)\n");
		printf(" 11. ���� ���� ���̵� ���� ���� ȭ��             (logo/notguest.log)\n");
		printf(" 12. ���� ���� ����(�������Ǽ���)ȭ��            (logo/notenter.log)\n");
		printf(" 13. ���̵� ����     ����             :%s\n",rbit(cfg.guestin,20, "����", "�Ұ�"));
		printf(" 14. ���̵� �ѱ�     ����             :%s\n",rbit(cfg.guestin,21, "����", "�Ұ�"));
		printf(" 15. ���̵� ����     ����             :%s\n",rbit(cfg.guestin,22, "����", "�Ұ�"));
		printf(" 16. ���̵� Ư������ ����             :%s\n",rbit(cfg.guestin,23, "����", "�Ұ�"));
		printf(" 17. ��й�ȣ�� �����й�ȣ ����     :%s\n",rbit(cfg.guestin,24, "���", "�Ұ�"));
		printf(" 18. �ּ��Է½� ��ü�ּ� ����         :%s\n",rbit(cfg.guestin,25, "���", "�Ұ�"));
		printf(" 19. ���Խ� ���� �׸� ����\n");
		printf(" 20. �ֹε�Ϲ�ȣ �Է½� �ڵ� ��� ���ڿ�        (system/passindo.txt)    \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.guestmode  =gtt_bit(cfg.guestmode,3);
		if (n== 2) cfg.guestmode  =gtt_bit(cfg.guestmode,4);
		if (n== 3) guest_input_item();
		if (n== 4) guest_init();
		if (n== 5) job_epass();
		if (n== 9) xedit("system/notguest.txt");
		if (n==10) xedit("system/bad_idno.txt");
		if (n==11) xedit("frm/guest/notguest.log");
		if (n==12) xedit("frm/guest/notenter.log");
		if (n==13) cfg.guestin=gtt_bit(cfg.guestin,20);
		if (n==14) cfg.guestin=gtt_bit(cfg.guestin,21);
		if (n==15) cfg.guestin=gtt_bit(cfg.guestin,22);
		if (n==16) cfg.guestin=gtt_bit(cfg.guestin,23);
		if (n==17) cfg.guestin=gtt_bit(cfg.guestin,24);
		if (n==18) cfg.guestin=gtt_bit(cfg.guestin,25);
		if (n==19) guest_input_item();
		if (n==20) xedit("system/passidno.txt");
	}
}

job_epass()
{
	int n,i,eenum;
	char buff[300];
	char filename[300];

	strcpy(filename,"system/epass.num");
	while (1) {
		file_line(buff,filename,1);
		eenum=atoi(buff);
		cls();
		print_(0,"���� ��ȣ ���� ����");
		printf("  1. ������ ���� ��ȣ�Է��� ���1����   %s\n",rbit(cfg.guestmode,25,"����","����"));
		printf("  2. ������ ���� ��ȣ�Է��� �±�2����   %s\n",rbit(cfg.guestmode,26,"����","����"));
		printf("  3. ID �Է���   ���� ��ȣ�Է�          %s\n",rbit(cfg.guestmode,27,"����","����"));
		printf("  4. ���ι�ȣ �ڵ� ���� (0:���� #:����) %d\n",eenum);
		printf("  7. ȸ�� ���� ��ȣ ���� ����         \n");
		printf("  8. ȸ�� ���� ��ȣ ���� ����(����)   \n");
		printf("  9. ȸ�� ���� ��ȣ ��� Ȯ��         \n");
		n=numselect();
		if (n<= 0) break;
		if (n== 1) cfg.guestmode =gtt_bit(cfg.guestmode,25);
		if (n== 2) cfg.guestmode =gtt_bit(cfg.guestmode,26);
		if (n== 3) cfg.guestmode =gtt_bit(cfg.guestmode,27);
		if (n== 4) xedit(filename);
		if (n== 7) make_epass(1);
		if (n== 8) make_epass(2);
		if (n== 9) make_epass(3);
	}
}

make_epass(int mode)
{
	FILE *fp;
	char buff[300];
	char buff2[300];
	char buff3[300];
	char filename[300];

	if (mode==1) {
		while(1) {
			printf("\n  ���� ��ȣ ���� ID :");
			lineinput(buff,8);
			if (strlen(buff)<3) break;
			if (!read_pf4(buff)) {
				printf("  %s ID�� ã���� �����ϴ�. \n",buff);
				continue;
			}
			printf(" �̸�:%s  ���:%d",pf.id,pf.type);
			printf("\n  ���� ��ȣ �Է� (4~12�ڸ�) :");
			lineinput(buff2,8);
			if (strlen(buff2)<4) continue;
			sprintf(buff,"user/epass/%s.dat",pf2.id);
			if ((fp=fopen(buff,"w"))!=NULL) {
				sprintf(buff3,"%s %-8s %-8s %s",buff2,pf2.id,pf2.name,now_str(2));
				fprintf(fp,"%s\n",buff3);
				fclose(fp);
			}
			sprintf(buff,"user/epass/%s.lst",now_str(12));
			if ((fp=fopen(buff,"a+"))==NULL) fp=fopen(buff,"w+");
			if (fp!=NULL) {
				fprintf(fp,"%s\n",buff3);
				fclose(fp);
			}
			printf("  %s -> %s ��������",pf2.id,buff2);
		}
	}
	else if (mode==2) {
		while(1) {
			printf("\n  ���� ��ȣ ����/���� ID :");
			lineinput(buff2,8);
			if (strlen(buff2)<3) break;
			sprintf(filename,"user/epass/%s.dat",buff2);
			if (!is_file(filename)) {
				printf("  %s ���ι�ȣ������ �����ϴ�. \n",filename);
				continue;
			}
			xedit(filename);
			if (file_size(filename)<5) {
				unlink(filename);
				printf("\n  %s ���ι�ȣ������ �����Ǿ����ϴ�. \n",filename);
			}
		}
	}
	else if (mode==3) {
		while(1) {
			printf("\n  ����� Ȯ���� �⵵��  (��99��5��:9905)>>");
			lineinput(buff2,5);
			if (strlen(buff2)==0) strcpy(buff2,now_str(12));
			if (strlen(buff2)<3) break;
			sprintf(filename,"user/epass/%s.lst",buff2);
			xedit(filename);
		}
	}
}



guest_init()
{
	int n,i;
	char buff[300];
	while (1) {
		cls();
		print_(0,"���Խ� �ʱⰪ ���� ����");
		printf("  1. ������ �ʱ� ��� (0-16)      :%d\n",cfg.guesttype  );
		printf("  2. ������ �ʱ� ����             :%d\n",cfg.guestlevel );
		printf("  3. ������ �ʱ� �ð� (��)        :%d\n",cfg.guesttime  );
		printf("  4. ������ �ʱ� �ٿ�뷮K        :%d\n",cfg.guestdown  );
		printf("  5. ������ �ʱ� �ݾ�             :%d\n",cfg.guestcash  );
		printf("  6. ������ �ʱ� ü��             :%d\n",cfg.guestp_ph  );
		printf("  7. ������ �ʱ� ����             :%d\n",cfg.guestp_it  );
		printf("  8. ������ �ʱ� �ŷ�             :%d\n",cfg.guestp_at  );
		printf("  9. ������ �ʱ� ����           :%d\n",cfg.guestp_pr  );
		printf(" 10. ������ �ʱ� ���             :%d\n",cfg.guestp_mo  );
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.guesttype  =get_num(cfg.guesttype  , 2);
		if (n== 2) cfg.guestlevel =get_num(cfg.guestlevel , 6);
		if (n== 3) cfg.guesttime  =get_num(cfg.guesttime  , 8);
		if (n== 4) cfg.guestdown  =get_num(cfg.guestdown  , 8);
		if (n== 5) cfg.guestcash  =get_num(cfg.guestcash  , 8);
		if (n== 6) cfg.guestp_ph  =get_num(cfg.guestp_ph  , 8);
		if (n== 7) cfg.guestp_it  =get_num(cfg.guestp_it  , 8);
		if (n== 8) cfg.guestp_at  =get_num(cfg.guestp_at  , 8);
		if (n== 9) cfg.guestp_pr  =get_num(cfg.guestp_pr  , 8);
		if (n==10) cfg.guestp_mo  =get_num(cfg.guestp_mo  , 8);
	}
}


guest_input_item()
{
	while (1) {
		cls();
		print_(0,"���Խ� ���� ���� ����");
		printf("  1. ���Խ� ID   ���  %s\n",rbit(cfg.guestin, 1," �� ","����"));
		printf("  2. ���Խ� �̸� ���  %s\n",rbit(cfg.guestin, 2," �� ","����"));
		printf("  3. �ֹε�Ϲ�ȣ���  %s\n",rbit(cfg.guestin, 3," �� ","����"));
		printf("  4. ������� ���     %s\n",rbit(cfg.guestin, 4," �� ","����"));
		printf("  5. ����     ���     %s\n",rbit(cfg.guestin, 5," �� ","����"));
		printf("  6. ��ȣ���          %s\n",rbit(cfg.guestin, 6," �� ","����"));
		printf("  7. �ּҹ��          %s\n",rbit(cfg.guestin, 7," �� ","����"));
		printf("  8. ��ȭ���          %s\n",rbit(cfg.guestin, 8," �� ","����"));
		printf("  9. �����ּ�            %s\n",rbit(cfg.guestin, 9," �� ","����"));
		printf(" 10. �߻߹��          %s\n",rbit(cfg.guestin,10," �� ","����"));
		printf(" 11. �ڵ������        %s\n",rbit(cfg.guestin,11," �� ","����"));
		printf(" 12. �������          %s\n",rbit(cfg.guestin,12," �� ","����"));
		printf(" 13. ���幰�          %s\n",rbit(cfg.guestin,13," �� ","����"));
		printf(" 14. ������ȭ            %s\n",rbit(cfg.guestin,14," �� ","����"));
		printf(" 15. ������� 1          %s\n",rbit(cfg.guestin,15," �� ","����"));
		printf(" 16. ������� 2          %s\n",rbit(cfg.guestin,16," �� ","����"));
		printf(" 17. ������� 3          %s\n",rbit(cfg.guestin,17," �� ","����"));
		printf(" 18. ������� 4          %s\n",rbit(cfg.guestin,18," �� ","����"));
		printf(" 19. ������� 5          %s\n",rbit(cfg.guestin,19," �� ","����"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.guestin=gtt_bit(cfg.guestin,1);
		if (n== 2) cfg.guestin=gtt_bit(cfg.guestin,2);
		if (n== 3) cfg.guestin=gtt_bit(cfg.guestin,3);
		if (n== 4) cfg.guestin=gtt_bit(cfg.guestin,4);
		if (n== 5) cfg.guestin=gtt_bit(cfg.guestin,5);
		if (n== 6) cfg.guestin=gtt_bit(cfg.guestin,6);
		if (n== 7) cfg.guestin=gtt_bit(cfg.guestin,7);
		if (n== 8) cfg.guestin=gtt_bit(cfg.guestin,8);
		if (n== 9) cfg.guestin=gtt_bit(cfg.guestin,9);
		if (n==10) cfg.guestin=gtt_bit(cfg.guestin,10);
		if (n==11) cfg.guestin=gtt_bit(cfg.guestin,11);
		if (n==12) cfg.guestin=gtt_bit(cfg.guestin,12);
		if (n==13) cfg.guestin=gtt_bit(cfg.guestin,13);
		if (n==14) cfg.guestin=gtt_bit(cfg.guestin,14);
		if (n==15) cfg.guestin=gtt_bit(cfg.guestin,15);
		if (n==16) cfg.guestin=gtt_bit(cfg.guestin,16);
		if (n==17) cfg.guestin=gtt_bit(cfg.guestin,17);
		if (n==18) cfg.guestin=gtt_bit(cfg.guestin,18);
		if (n==19) cfg.guestin=gtt_bit(cfg.guestin,19);
	}
}


job_idout()
{
	while (1) {
		cls();
		printf("            ���� ���� ���� ����\n");
		printfL2();
		printf("  1.  ���� ���� ����                :%s\n",rbit(cfg.loginmode,18, "����", "�Ұ�"));
		printf("  2.  ���� ������ �ֹε�� ����     :%s\n",rbit(cfg.loginmode,19, "����", "����"));
		printfL2();
		n=numselect();
		xy(60,n+2);
		if (n<=0)  break;
		if (n==1)  cfg.loginmode  =gtt_bit(cfg.loginmode,18);
		if (n==2)  cfg.loginmode  =gtt_bit(cfg.loginmode,19);
	}
}


job_notice()
{
	while (1) {
		cls();
		print_(1,"�������� ����");
		printf(" 1. �Ϲ�ȸ�� ���ӽ� ��¿���           :%s\n",rbit(cfg.notice,0,"���","���"));
		printf(" 2. �մ�     ���ӽ� ��¿���           :%s\n",rbit(cfg.notice,1,"���","���"));
		printf(" 3. �������� ��¹�� ID �Է���        :%s\n",rbit(cfg.notice,2,"�Ⱥ���","����"));
		printf(" 4. �������� ��¹�� TOP������        :%s\n",rbit(cfg.notice,3,"�Ⱥ���","����"));
		printf(" 5. �������� ���� ���                 :%s\n",rbit(cfg.notice,4,"��","����"));
		printf(" 6. �������� ��¸�� (0~9)            :%d\n",cfg.noticet);
		printf(" 7. �������� ��½��� ��ġ X           :%d\n",cfg.noticex);
		printf(" 8. �������� ��½��� ��ġ Y           :%d\n",cfg.noticey);
		printf(" 9. �������� ��½��� �ٱ���           :%d\n",cfg.noticeline);
		n=numselect();xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.notice	  =gtt_bit(cfg.notice ,0);
		if (n== 2) cfg.notice	  =gtt_bit(cfg.notice ,1);
		if (n== 3) cfg.notice	  =gtt_bit(cfg.notice ,2);
		if (n== 4) cfg.notice	  =gtt_bit(cfg.notice ,3);
		if (n== 5) cfg.notice	  =gtt_bit(cfg.notice ,4);
		if (n== 6) cfg.noticet	  =get_num(cfg.noticet,2);
		if (n== 7) cfg.noticex	  =get_num(cfg.noticex,2);
		if (n== 8) cfg.noticey	  =get_num(cfg.noticey,2);
		if (n== 9) cfg.noticeline =get_num(cfg.noticeline,2);
	}
}



//��޺� �ٷΰ���
auto_go()
{
	FILE *fp;
	char buff[200];
	char filename[200];
	char auto_gocode[20][51];
	int i;
	for (i=0;i<20;i++) strcpy(auto_gocode[i],"");
	strcpy(filename,"system/auto_go.txt");
	if ((fp=fopen(filename,"r+"))!=NULL) {
		for (i=0;i<20;i++) {
			if (fgets(buff,200,fp)) {
				del_enter(buff);
				del_esc(buff);
				buff[50]=0;
				strcpy(auto_gocode[i],buff);
			}
		}
		fclose(fp);
	}
	while (1) {
		cls();
		print_(0,"��޺� �ڵ� ���� ��� ����");
		for (i=0;i<20;i++) {
			printf("  ��� %2d. %s\n",i+1,auto_gocode[i]);
		}
		printf(" ���� (0~19)  ���(Q)  ����(W) >>");
		lineinput(buff,20);n=atoi(buff);
		if (is_quit(buff)) break;
		if (is_same(buff,"w")) break;
		if (buff[0]==0) break;
		if (n==0&&!is_same(buff,"0")) continue;
		if (n>=0&&n<=20) {
			xy(40,n+2);
			get_str(auto_gocode[n-1],50);
		}
	}
	if ((fp=fopen(filename,"w+"))!=NULL) {
		for (i=0;i<20;i++) {
			del_enter(auto_gocode[i]);
			del_esc(auto_gocode[i]);
			fprintf(fp,"%s\n",auto_gocode[i]);
		}
		fclose(fp);
	}
}



//�з�/�ڵ� ���� �۾�
job_type()
{
	while (1) {
		cls();
		print_(0,"���/�з�/�ڵ� ����");
		printf("  1. �ڵ� ��� ���� ���                       :%s\n",rbit(cfg.levelmode,1, "����", "�ڵ�"));
		printf("  2. ȸ�� ���� ����                            :%d\n",cfg.levelmax   );
		printf("  3. ��ȣȸ�� �ִ� ���� ���� ���� (0:����~250) :%u\n",cfg.clublevel  );
		printf("  4. ��޺� ��Ī      ���� \n");
		printf("  5. ��޺� �±�/���Է��� ���� \n");
		printf("  6. ��޺� ���ϻ�� ���� �ð� \n");
		printf("  7. ��޺� 1ȸ ���� ���� �ð� \n");
		printf("  8. ������ �з� �ڵ� ���� \n");
		printf("  9. �ּҺ� �з� �ڵ� ���� \n");
		printf(" 10. �Խ���/��ȣȸ ��õ�ڵ�\n");
		n=numselect();xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.levelmode =gtt_bit(cfg.levelmode , 1);
		if (n== 2) cfg.levelmax  =get_num(cfg.levelmax	, 8);
		if (n== 3) cfg.clublevel =get_num(cfg.clublevel , 3);
		if (n== 4) edit_atype(1);
		if (n== 5) edit_atype(2);
		if (n== 6) edit_atype(3);
		if (n== 7) edit_atype(4);
		if (n== 8) edit_ajob();
		if (n== 9) edit_addr();
		if (n==10) edit_gg();
	}
}



job_jpg()
{
	while (1) {
		cls();
		print_(0,"ȭ����� ����");
		printf(" 1. �ڷ�ǿ��� SEE ��밡��       :%s\n",rbit(cfg.jpgmode,0, "���", "����"));
		printf(" 2. ȭ���ڷ� JPG ���� ��          :%s\n",cfg.jpgchar);
		printf(" 3. ȭ��Խ��ǿ��� JPG ���� ǥ��  :%s\n",rbit(cfg.jpgmode,1, "ǥ��", "����"));
		printf(" 4. �ϹݰԽ��ǿ��� JPG ���� ǥ��  :%s\n",rbit(cfg.jpgmode,2, "ǥ��", "����"));
		printf(" 5. �Ϲ��ڷ�ǿ��� JPG ���� ǥ��  :%s\n",rbit(cfg.jpgmode,3, "ǥ��", "����"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 0);
		if (n== 2)	get_str(cfg.jpgchar,2);
		if (n== 3)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 1);
		if (n== 4)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 2);
		if (n== 5)	cfg.jpgmode    =gtt_bit(cfg.jpgmode, 3);
	}
}





job_war()
{
	while (1) {
		cls();
		printf(" 1.  ��  �� �ּҺ�ȣ��  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 2.  ��ȣȸ �ּҺ�ȣ��  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 3.  ȸ  �� �ּҺ�ȣ��  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 4.  ������ �ּҺ�ȣ��  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 5.  �Ҹ��� �ּҺ�ȣ��  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 6.  ȸ�� �����ִ�ս�  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printf(" 7.  ȸ�� �����ִ��̵�  A:%-5d  B:%-5d  C:%-5d  D:%-5d  E:%-5d\n",0,0,0,0,0);
		printfL2();
		n=numselect();
		xy(60,n);
		if (n<= 0)	break;
	}
}

job_mess()
{
	while (1) {
		cls();
		printf(" ### �޼��� ���� ����\n");
		printf("  1.  �ڳݽ� �޼��� ����             :%s\n",rbit(cfg.messmode,1,"���", "����"));
		printf("  2.  �۾���� �޼��� ����           :%s\n",rbit(cfg.messmode,4,"���", "����"));
		printf("  3.  ��ȭ�� �޼��� ����             :%s\n",rbit(cfg.messmode,5,"���", "����"));
		printf("  4.  �ܺθ�ɽ� �޼��� ����         :%s\n",rbit(cfg.messmode,6,"���", "����"));
		printf("  5.  �մ����ӽ�     ��񵵿�      :%s\n",rbit(cfg.messmode,2,"����", "����"));
		printf("  6.  �Ϲ�ȸ�����ӽ� ��񵵿�      :%s\n",rbit(cfg.messmode,3,"����", "����"));
		printf("  7.  ��񵵿� ���� ����           :%d\n",cfg.helplevel);
		printf("  8.  ��񵵿� ���� ���           :%d\n",cfg.helptype);
		printf("  9.  ��񵵿� ���� ���Ӽ�         :%d\n",cfg.helplog);
		printf(" 10.  ��ü �޼��� ��� ���          :%d\n",cfg.allsay1);
		printf(" 11.  ��ü �޼��� ��� ���(����)    :%d\n",cfg.allsay2);
		printf(" 12.  ��ü �޼��� ��� ���(��ȣȸ)  :%d\n",cfg.allsay3);
		printf(" 13.  ���� ȣ��Ʈ -> xmessd ó��     :%s\n",rbit(cfg.messmode,7,"tty���", "xmessd���"));
		printf(" 14.  MESS/SAY �� ���ù��           :%s\n",rbit(cfg.messmode,8,"US ȭ��","��ܸ��"));
		printf(" 20.  US ���� ����               \n");
		n=numselect();
		printf("\n");
		xy(60,n);
		if (n<= 0)	break;
		if (n== 1)	cfg.messmode=gtt_bit(cfg.messmode,1);
		if (n== 2)	cfg.messmode=gtt_bit(cfg.messmode,4);
		if (n== 3)	cfg.messmode=gtt_bit(cfg.messmode,5);
		if (n== 4)	cfg.messmode=gtt_bit(cfg.messmode,6);
		if (n== 5)	cfg.messmode=gtt_bit(cfg.messmode,2);
		if (n== 6)	cfg.messmode=gtt_bit(cfg.messmode,3);
		if (n== 7)	cfg.helplevel=get_num(cfg.helplevel,3);
		if (n== 8)	cfg.helptype =get_num(cfg.helptype,3);
		if (n== 9)	cfg.helplog  =get_num(cfg.helplog,3);
		if (n==10)	cfg.allsay1  =get_num(cfg.allsay1,2);
		if (n==11)	cfg.allsay2  =get_num(cfg.allsay2,2);
		if (n==12)	cfg.allsay3  =get_num(cfg.allsay3,2);
		if (n==13)	cfg.messmode=gtt_bit(cfg.messmode,7);
		if (n==14)	cfg.messmode=gtt_bit(cfg.messmode,8);
		if (n==20)	job_us();
	}
}

//���� ���� �۾�
job_start()
{
	while (1) {
		cls();
		print_(1,"���� ���� ����");
		printf("  1. ���Խ� �ٷ� ���θ޴� ����                 :%s\n",rbit(cfg.startmode,1,"����","�ٷ�����"));
		printf("  2. ���Խ� ���� �ΰ� ���                     :%s\n",rbit(cfg.startmode,2,"���","����"));
		printf("  3. ���Խ� ��ü �ӽ� ID �ڵ� �ο�             :%s\n",rbit(cfg.startmode,3,"���","����"));
		printf("  4. ���̵��Է��� ��������  ���               :%s\n",rbit(cfg.startmode,4,"���","����"));
		printf("  5. ���̵��Է��� ����ð�  ���               :%s\n",rbit(cfg.startmode,5,"���","����"));
		printf("  6. ���̵��Է��� ����Ȯ��  ���               :%s\n",rbit(cfg.startmode,9,"���","����"));
		printf("  7. ���̵��Է��� ��ü����  ���               :%s\n",rbit(cfg.startmode,6,"���","����"));
		printf("  8. ���̵��Է��� ��ü����  ���               :%s\n",rbit(cfg.startmode,10,"���","����"));
		printf("  9. ���̵��Է��� ����Ȯ��  ���               :%s\n",rbit(cfg.startmode,7,"���","����"));
		printf(" 10. ���̵��Է��� EnterŰ   �Է�               :%s\n",rbit(cfg.startmode,8,"Enter","1�ʴ��"));
		printf(" 11. ���ӽ� 7�ϵ��� �������ӿ��� Ȯ��          :%s\n",rbit(cfg.startmode,11,"����","Ȯ����"));
		printf(" 12. �������ӽ� ����/�ٿ ����ġ (system/contday.dat)\n");
		printf(" 13. ������ EnterŰ   �Է�                     :%s\n",rbit(cfg.startmode,13,"Enter","����"));
		printf(" 20. ���Խ� �ڵ���ũ����                       :%s\n",rbit(cfg.startmode,14,"����","����"));
		printf(" 21. ���Խ� ��ȭ�� ���� Ȯ��                   :%s\n",rbit(cfg.startmode,15,"����","����"));
		printf(" 22. ���Խ� �ϵ� �ܷ� 50MB Ȯ��                :%s\n",rbit(cfg.startmode,16,"����","Ȯ��"));
		printf(" 23. ����� Y/n ������                         :%s\n",rbit(cfg.mode,6, "����", "����"));
		printf(" 24. ����� ȭ�� ���                          :%s\n",rbit(cfg.logomode,22,"����","����"));
		printf(" 25. �ð����� ����� ���� ȭ����� ����        :%s\n",rbit(cfg.startmode,17,"����","����"));
		n=numselect();
		if (n<= 0) break;
		if (n== 1) cfg.startmode=gtt_bit(cfg.startmode, 1);
		if (n== 2) cfg.startmode=gtt_bit(cfg.startmode, 2);
		if (n== 3) cfg.startmode=gtt_bit(cfg.startmode, 3);
		if (n== 4) cfg.startmode=gtt_bit(cfg.startmode, 4);
		if (n== 5) cfg.startmode=gtt_bit(cfg.startmode, 5);
		if (n== 6) cfg.startmode=gtt_bit(cfg.startmode, 9);
		if (n== 7) cfg.startmode=gtt_bit(cfg.startmode, 6);
		if (n== 8) cfg.startmode=gtt_bit(cfg.startmode,10);
		if (n== 9) cfg.startmode=gtt_bit(cfg.startmode, 7);
		if (n==10) cfg.startmode=gtt_bit(cfg.startmode, 8);
		if (n==11) cfg.startmode=gtt_bit(cfg.startmode,11);
		if (n==12) xedit2("system/contday.dat");
		if (n==13) cfg.startmode=gtt_bit(cfg.startmode,13);
		if (n==20) cfg.startmode=gtt_bit(cfg.startmode,14);
		if (n==21) cfg.startmode=gtt_bit(cfg.startmode,15);
		if (n==22) cfg.startmode=gtt_bit(cfg.startmode,16);
		if (n==23) cfg.mode 	=gtt_bit(cfg.mode, 6);
		if (n==24) cfg.logomode =gtt_bit(cfg.logomode,22);
		if (n==25) cfg.startmode=gtt_bit(cfg.startmode,17);
	}
}



job_cmd()
{
	while (1) {
		cls();
		print_(1,"���/�Է� ���� ����");
		printf(" 1. ���θ޴����� P���� ���� ����            :%s\n",rbit(cfg.mode,30,"����","����"));
		printf(" 2. ����Է½� �ڵ�������ȯ                   :%s\n",rbit(cfg.mode,4,"�ڵ���ȯ","������ȯ"));
		printf(" 3. ��ɾ� �ڵ� ���(ȭ��ǥ�̵�) ���         :%s\n",rbit(cfg.mode,8,"���","����"));
		printf(" 4. ����޴� �ϴܸ���Է���ġ ����            :%s\n",rbit(cfg.mode,11,"����","����"));
		printf(" 5. ������Ű ����         (system/shortcmd.dat)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.mode=gtt_bit(cfg.mode,30);
		if (n== 2) cfg.mode=gtt_bit(cfg.mode,4);
		if (n== 3) cfg.mode=gtt_bit(cfg.mode,8);
		if (n== 4) cfg.mode=gtt_bit(cfg.mode,11);
		if (n== 5) xedit2("system/shortcmd.dat");
	}
}





job_chat()
{
	while (1) {
		cls();
		print_(0,"��ȭ�Ǽ���");
		printf("  1.  ��ȭ�� �����۽ð�                    :%d\n",cfg.chatwarn   );
		printf("  2.  ��ȭ�� ���԰��ɵ��                    :%d\n",cfg.chattype   );
		printf("  3.  �޼��� ���۽� ����ID ������            :%s\n",rbit(cfg.mode,16, "����", "����"));
		printf("  4.  ��ȣȸ ��ȭ�� STǥ��                   :%s\n",rbit(cfg.mode,23, "ǥ��", "����"));
		printf("  5.  ��ȭ�� �з������ñ��                  :%s\n",rbit(cfg.chatmode,0, "����", "����"));
		printf("  6.  �մ�   ��ü ��ȭ�� ���                :%s\n",rbit(cfg.chatmode,13, "�Ұ���", "����"));
		printf("  7.  ��ȭ�� ���Խ� ������                 :%s\n",rbit(cfg.chatmode,14, "����", "����"));
		printf("  8.  �մԽ� 99��ȭ�� ���                   :%s\n",rbit(cfg.chatmode,15, "�Ұ���", "����"));
		printf("  9.  ��ȭ�� ���Խ� �� ����� ����           :%s\n",rbit(cfg.chatmode,16, "����", "����"));
		printf(" 10.  ��ȭ�� ���Խ� �ʱ� ��� (0~12)         :%d\n",cfg.chatst);
		printf(" 11.  SAY���ִ������ټ�(0:������ 1-255:����) :%d\n",cfg.sayline);
		printf(" 12.  ��ȭ�ǿ��� Ÿ�ڹ� ���ۿ���             :%s\n",rbit(cfg.chatmode,18, "����ȭ��", "Ÿ��:71?"));
		printf(" 13.  ��ȭ�ǿ��� ����� ���ۿ���             :%s\n",rbit(cfg.chatmode,19, "����ȭ��", "Ÿ��:72?"));
        printf(" 14.  www.3B.co.kr ���ͳݴ�ȭ�� ����         :%s\n",rbit(cfg.chatmode,20, "����","����ȭ��"));
        printf(" 15.  chat.xhost.co.kr ���ͳݴ�ȭ�� ����     :%s\n",rbit(cfg.chatmode,21, "����","����ȭ��"));
        printf(" 19.  SAY2 ��� ��� ���� ���̵�             (system/say2.id)\n");
		printf(" 20.  ��ȭ�� ���Ա��� ID ����                (system/nochat.id)\n");
		printf(" 21.  ��ȭ�� ���� ���� (����ȸ��)            (frm/error/chatoff.log)\n");
		printf(" 22.  ��ȭ�� ���� ���� ID (/sys,���԰���)    (system/chatsys.id)\n");
		printf(" 23.  ��ȭ�� �з��� ���� (��������)          (system/chattype.dat)\n");
		n=numselect();
		xy(60,n+3);
		if (n<= 0)	break;
		if (n== 1)	cfg.chatwarn  =get_num(cfg.chatwarn ,6);
		if (n== 2)	cfg.chattype  =get_num(cfg.chattype ,2);
		if (n== 3)	cfg.mode=gtt_bit(cfg.mode,16);
		if (n== 4)	cfg.mode=gtt_bit(cfg.mode,23);
		if (n== 5)	cfg.chatmode=gtt_bit(cfg.chatmode,0);
		if (n== 6)	cfg.chatmode=gtt_bit(cfg.chatmode,13);
		if (n== 7)	cfg.chatmode=gtt_bit(cfg.chatmode,14);
		if (n== 8)	cfg.chatmode=gtt_bit(cfg.chatmode,15);
		if (n== 9)	cfg.chatmode=gtt_bit(cfg.chatmode,16);
		if (n==10)	cfg.chatst =get_num(cfg.chatst ,2);
		if (n==11)	cfg.sayline=get_num(cfg.sayline,3);
		if (n==12)	cfg.chatmode=gtt_bit(cfg.chatmode,18);
		if (n==13)	cfg.chatmode=gtt_bit(cfg.chatmode,19);
        if (n==14)  cfg.chatmode=gtt_bit(cfg.chatmode,20);
        if (n==15)  cfg.chatmode=gtt_bit(cfg.chatmode,21);
        if (n==19)  xedit2("system/say2.id");
		if (n==20)	xedit2("system/nochat.id");
		if (n==21)	xedit2("frm/error/chatoff.log");
		if (n==22)	xedit2("system/chatsys.id");
		if (n==23)	set_roomtype_limit();	  //�з��� ���Ѽ���
	}
}

job_screen()
{
	char buff[300];
	while (1) {
		cls();
		printf(" ### ȭ�� ���� ȯ�漳��\n");
		printf("  1.  PF�� ���/���� ǥ�ÿ���    :%s\n",rbit(cfg.mode,14,"ǥ��", "�Ұ�"));
		printf("  3.  �ϴ� ���� ������           :%s\n",rbit(cfg.mode,27,"����", "����"));
		printf("  4.  ��� ���� ����(today mess)����\n");
		printf("  5.  ����� ȭ�� ���           :%s\n",rbit(cfg.logomode,2,"����","����"));
		printf("  6.  ���۽� ȭ�� ���           :%s\n",rbit(cfg.logomode,1,"����","����"));
		printf("  7.  TOP �ִϸ��̼� �ΰ� ȭ��   :%s\n",rbit(cfg.logomode,3,"����","������"));
		printf("  8.  �ִϸ��̼� ��üȸ�� ����   :%s\n",rbit(cfg.logomode,4,"���κ�","��������"));
		printf("  9.  setlogo ��밡�� ���      :%d\n",cfg.no_logotype);
		printf(" 10.  US ���� ����               \n");
		printf(" 11.  �ϴܼ��� ��ɾ� �б�       :%s\n",rbit(cfg.logomode,5,"FRM����","������"));
		printf(" 12.  ������½� �ִ� ���Ӽ�*���:%d\n",cfg.limit_slog);
		printf(" 13.  ���� ��ɼ��ý� ���ú��̱� :%s\n",rbit(cfg.logomode,8,"�Ѵٺ���","���ú���"));
		printf(" 14.  [7m ���� ����              :%s\n",rbit(cfg.logomode,9,"������","����"));
		printf(" 15.  logodndn ���(�ΰ�ٿ�)    :%s\n",rbit(cfg.logomode,11,"���ź�","����㰡"));
		printf(" 90.  ���� ȭ�� ���� ���        (.scr)             \n");

		n=numselect();
		xy(60,n+1);
		if (n<=0)  break;
		if (n==1)  cfg.mode 	  =gtt_bit(cfg.mode,14);
		if (n==2)  cfg.mode 	  =gtt_bit(cfg.mode,17);
		if (n==3)  cfg.mode 	  =gtt_bit(cfg.mode,27);
		if (n==4)  job_todaymess();
		if (n==5)  cfg.logomode   =gtt_bit(cfg.logomode,2);
		if (n==6)  cfg.logomode   =gtt_bit(cfg.logomode,1);
		if (n==7)  cfg.logomode   =gtt_bit(cfg.logomode,3);
		if (n==8)  cfg.logomode   =gtt_bit(cfg.logomode,4);
		if (n==9)  cfg.no_logotype=get_num(cfg.no_logotype,2);
		if (n==10) job_us();
		if (n==11) cfg.logomode   =gtt_bit(cfg.logomode,5);
		if (n==12) cfg.limit_slog =get_num(cfg.limit_slog,3);
		if (n==13) cfg.logomode   =gtt_bit(cfg.logomode,8);
		if (n==14) cfg.logomode   =gtt_bit(cfg.logomode,9);
		if (n==15) cfg.logomode   =gtt_bit(cfg.logomode,11);
		if (n==90) {sys_tem("bin/xfrm 1 '%s' '%s' '%s'",club_go.code,go.code,now_id);}
	}
}


job_todaymess()
{
	int  check_today;		// ���ø� �˻�?
	int  check_todayon; 	// ��¿��ΰ˻�?
	char buff[300];
	int i;
	while (1) {
		cls();
		printf(" ### ������� ���� ����\n");
		printf(" 1. ��� ���� �ȳ� �޼��� help    :%s\n",rbit(cfg.logomode,6,"������","�Ⱥ���"));
		printf(" 2. ��� ���� �ȳ� ALARM �ڵ���� :%s\n",rbit(cfg.logomode,7,"����","�ڵ�����"));
		printf(" 3. ��� ���� Ȯ�� �۾��� �ڵ�1:%s 2:%s 3:%s 4:%s 5:%s\n",cfg.check_wgo[0],cfg.check_wgo[1],cfg.check_wgo[2],cfg.check_wgo[3],cfg.check_wgo[4]);
		printf(" ��¿���: ");
		for (i=0;i<32;i++) {
			printf("%2d.%s ",i+31,rbit2(cfg.check_today  ,i,"O","x"));
			if (i==4||i==14) printf("  ");
			if (i==9||i==19) printf("\n           ");
		}
		printf("\n ���ϰ���: ");
		for (i=0;i<32;i++) {
			printf("%2d.%s ",i+61,rbit2(cfg.check_todayon,i,"A","t"));
			if (i==4||i==14) printf("  ");
			if (i==9||i==19) printf("\n           ");
		}
		printf("\n");
		n=numselect();
		xy(60,n+1);
		if (n<=0) break;
		if (n==1) cfg.logomode	 =gtt_bit(cfg.logomode,6);
		if (n==2) cfg.logomode	 =gtt_bit(cfg.logomode,7);
		if (n==3) {for (i=0;i<5;i++) get_str(cfg.check_wgo[i],8);}
		if (n>=31&&n<=60) cfg.check_today  =gtt_bit(cfg.check_today  ,n-31);
		if (n>=61&&n<=90) cfg.check_todayon=gtt_bit(cfg.check_todayon,n-61);
	}
}


job_us()
{
	while (1) {
		cls();
		printf(" ### US ���� ȯ�漳��\n");
		printf("  1.  ����  ���� USǥ�� ����     :%s\n",rbit(cfg.usermode,1, "����", "�Ұ�"));
		printf("  2.  ��ȭ�ǿ��� USǥ�� ����     :%s\n",rbit(cfg.usermode,2, "����", "�Ұ�"));
		printf("  3.  ��ȣȸ���� USǥ�� ����     :%s\n",rbit(cfg.usermode,3, "����", "�Ұ�"));
		printf("  4.  �޼������� USǥ�� ����     :%s\n",rbit(cfg.usermode,4, "����", "�Ұ�"));
		printf("  5.  US�� ���/���� ǥ�ÿ���    :%s\n",rbit(cfg.usermode,5, "ǥ��", "�Ұ�"));
		printf("  6.  US�� �����ϰ� ������       :%s\n",rbit(cfg.usermode,6, "����", "����"));
		printf("  7.  US�� ������ ��������   :%s\n",rbit(cfg.usermode,7, "����", "����"));
		printf("  8.  US�� IP�ּҴ�� TTY������  :%s\n",rbit(cfg.usermode,8, "IP ", "TTY"));
		printf("  9.  ȭ��Ѿ�� EnterŰ ����  :%s\n",rbit(cfg.usermode,9, "����", "���"));
		printf(" 10.  ����USǥ�ù������ (0~39)  :%d\n",cfg.usdisp);
		printf(" 11.  ����USǥ��(�޼��������) :%d\n",cfg.usdisp2);
		printf(" 12.  �Ϲ� ��Ͻ� US ���� ����   :%s\n",rbit(cfg.usermode,11,"����","����"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.usermode	=gtt_bit(cfg.usermode,1);
		if (n== 2)	cfg.usermode	=gtt_bit(cfg.usermode,2);
		if (n== 3)	cfg.usermode	=gtt_bit(cfg.usermode,3);
		if (n== 4)	cfg.usermode	=gtt_bit(cfg.usermode,4);
		if (n== 5)	cfg.usermode	=gtt_bit(cfg.usermode,5);
		if (n== 6)	cfg.usermode	=gtt_bit(cfg.usermode,6);
		if (n== 7)	cfg.usermode	=gtt_bit(cfg.usermode,7);
		if (n== 8)	cfg.usermode	=gtt_bit(cfg.usermode,8);
		if (n== 9)	cfg.usermode	=gtt_bit(cfg.usermode,9);
		if (n==10)	cfg.usdisp		=get_num(cfg.usdisp,2);
		if (n==11)	cfg.usdisp2 	=get_num(cfg.usdisp2,2);
		if (n==12)	cfg.usermode	=gtt_bit(cfg.usermode,11);
	}
}



job_system()
{
	while (1) {
		cls();
		printf("  ### �ý��� ���� ����\n");
		printf("  1.  ȣ��Ʈ�� �̸�                      :%s\n",cfg.hostname   );
		printf("  2.  Ű �Է� ��� �ð�   (0~255��)      :%d(��)\n",cfg.waitkey);
		printf("  3.  H,C -> HELP �� ��ȯ ����           :%s\n",rbit(cfg.mode,12,"����", "HELP"));
		printf("  4.  ���ӽ� �޼�����Ʈ����TTY ����      :%s\n",rbit(cfg.mode,26, "ó��", "����"));
		printf("  5.  ���18�̻� Ű��� ����             :%s\n",rbit(cfg.mode,0, "����", "������"));
		printf("  6.  �ð����Ѿ��� ID ����               (system/nokey.id)\n");
		printf("  7.  ������Ű ���� ȭ��               (system/shortcmd.dat)\n" );
		printf("  8.  �˶� �ñ׳� ó��                   :%s\n",rbit(cfg.mode,28, "����", "����"));
		printf("  9.  ȭ�鰻�Ž� �˶��ñ׳�����(����..)  :%s\n",rbit(cfg.logomode,10, "����", "����"));
		printf(" 10.  �˶� �ñ׳� �ֱ�  (0~255)          :%d\n",cfg.alarm_time);
		printf(" 11.  ���ӽÿ� ���ѽð��ʰ� ������Ȯ��   :%s\n",rbit(cfg.loginmode,1,"Ȯ��","���"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	get_str(cfg.hostname  ,20);
		if (n== 2)	cfg.waitkey   =(byte)get_num(cfg.waitkey   ,3 );
		if (n== 3)	cfg.mode	  =gtt_bit(cfg.mode,12);
		if (n== 4)	cfg.mode	  =gtt_bit(cfg.mode,26);
		if (n== 5)	cfg.mode	  =gtt_bit(cfg.mode,0 );
		if (n== 6)	xedit("system/nokey.id");
		if (n== 7)	xedit("system/shortcmd.dat");
		if (n== 8)	cfg.mode	  =gtt_bit(cfg.mode,28);
		if (n== 9)	cfg.logomode  =gtt_bit(cfg.logomode,10);
		if (n==10)	cfg.alarm_time=get_num(cfg.alarm_time,3);
		if (n==11)	cfg.loginmode =gtt_bit(cfg.loginmode,1);
	}
	del_space(cfg.sysop);
}


//�ý��� ���� ���� ����  (Ư�� ��� �Է� ���� Ȯ����)
job_limit()
{
	int n;
	char buff[200];
	FILE *fp;
	while (1) {
		cls();
		printf(" ###  �ý��� ���� ���� ���� ����\n");
		printf("  1.  ���ͳ� ���� ��� ���� (��Ʈ��ũ����)     (system/node_net.txt)\n");
		printf("  4.  �ܺ� �޴� ��� ���� ����                 (logo/_runoff.log)\n");
		printf("  5.  �ڳ� �޴� ���� ����                      (frm/error/teloff.log)\n");
		printf("  6.  �ӽ� �û� �޼��� (����ȸ��)              (logo/_sysmess.dat)\n");
		printf("  7.  ��庰,��Ʈ�� ���ӽ� ���ӱ���/��뼳��    \n");
		printf("  8.  ���԰���/���� �޴� ����                  (system/_nocode.dat) \n" );
		printf("  9.  ���԰���/���� ȭ�� ����                   \n" );
		printf(" 10.  ��ü ���� ���� ��� ����                 %d\n",cfg.limittype);
		printf(" 11.  ��ü ���� ���� ���� ����                 (frm/limit/limit.log)\n");
		printf(" 12.  �ð��뺰/���Ϻ� ������ ȸ�����          %d\n",cfg.freelogin);
		printf(" 13.  ���� ���� ���� ���                      %d\n",cfg.relog);
		printf(" 14.  ���� ���ӽ� �޼��� ���                  %s\n",rbit(cfg.mode,7,"��", "����"));
		printf(" 15.  MARK ��� ��밡��                       %s\n",rbit(cfg.mode,5,"����", "����"));
		printf(" 16.  �ڳ�   ���� ���� ���                    %d\n",cfg.telnettype);
		printf(" 17.  Ư�� ��� ���� ���� ����                 (system/nocode.txt)\n");
		printf(" 18.  ���� �ִ� �ο��� (frm/limit/full.log)    %d\n",cfg.limit_login);
		printf(" 19.  W���� R�б� D�ٿ� C��ȭ S��ȣȸ ����/����(system/limit.id)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) xedit("system/node_net.txt");
		if (n== 4) xedit("logo/_runoff.log");
		if (n== 5) xedit("frm/error/teloff.log");
		if (n== 6) {
			printf(" ����ȸ������ ������ �ӽ� �޼����� �Է��ϼ���.. \n>>");
			lineinput(buff,76);
			if (buff[0]==0 ) unlink("logo/_sysmess.dat");
			else {
				fp=fopen("logo/_sysmess.dat","w");
				fprintf(fp,"%s",buff);
				if (fp!=NULL) fclose (fp);
			}
		}
		if (n== 7) job_node();
		if (n== 8) set_no_code();
		if (n== 9) set_no_logo();
		if (n==10) cfg.limittype=get_num(cfg.limittype,2);
		if (n==11) xedit("frm/limit/limit.log");
		if (n==12) cfg.freelogin=get_num(cfg.freelogin,2);
		if (n==13) cfg.relog	=get_num(cfg.relog	  ,2);
		if (n==14) cfg.mode 	=gtt_bit(cfg.mode,7);
		if (n==15) cfg.mode 	=gtt_bit(cfg.mode,5);
		if (n==16) cfg.telnettype =get_num(cfg.telnettype,2);
		if (n==17) xedit("system/nocode.txt");
		if (n==18) cfg.limit_login=get_num(cfg.limit_login,4);
		if (n==19) set_limit_id();
	}
}


set_limit_id()
{
	FILE *fp;
	char buff[300],buff1[100],buff2[100],buff3[100];
	char filename[100],filename2[100];
	int n;
	while (1) {
		cls();
		printf(" ### W���� R�б� D�ٿ� C��ȭ S��ȣȸ ����ID\n");
		printf(" 1.  W���� R�б� D�ٿ� C��ȭ S��ȣȸ ����ID   (system/limit.id)\n");
		printf(" 2.  W���� �޼��� (frm/pf/notwrite.log)\n");
		printf(" 3.  R�б� �޼��� (frm/pf/notread.log)\n");
		printf(" 4.  D�ٿ� �޼��� (frm/pf/notdown.log)\n");
		printf(" 5.  C��ȭ �޼��� (frm/pf/notchat.log)\n");
		printf(" 6.  S��ȣ �޼��� (frm/pf/notclub.log)\n");
		printf(" 7.  �ϹݰԽ��� ���бⰡ�� ��� :%d\n",cfg.limit_r);
		printf(" 8.  �ϹݰԽ��� �۾��Ⱑ�� ��� :%d\n",cfg.limit_w);
		printf(" 9.  �Ϲ��ڷ�� ���  ���� ��� :%d\n",cfg.limit_u);
		printf("10.  �Ϲ��ڷ�� �ٿ�  ���� ��� :%d\n",cfg.limit_d);
		printf("11.  �����޴�(M)  ���԰��� ��� :%d\n",cfg.limit_m);
		printf("12.  ��/�ٿ�/���� ���� ���� (logo/_downoff.log)\n");
		printf("13.  �б� ����         ���� (logo/_readoff.log)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
			 if (n==1) edit_limit_id_file();
		else if (n==2) xedit2("frm/pf/notwrite.log");
		else if (n==3) xedit2("frm/pf/notread.log");
		else if (n==4) xedit2("frm/pf/notdown.log");
		else if (n==5) xedit2("frm/pf/notchat.log");
		else if (n==6) xedit2("frm/pf/notclub.log");
		else if (n== 7)  cfg.limit_r=get_num(cfg.limit_r,2);
		else if (n== 8)  cfg.limit_w=get_num(cfg.limit_w,2);
		else if (n== 9)  cfg.limit_u=get_num(cfg.limit_u,2);
		else if (n==10)  cfg.limit_d=get_num(cfg.limit_d,2);
		else if (n==11)  cfg.limit_m=get_num(cfg.limit_m,2);
		else if (n==12) xedit2("logo/_downoff.log");
		else if (n==13) xedit2("logo/_readoff.log");
	}
}


//��� ���� ����
job_node()
{
	int n,n2;
	char buff[100];
	n=n2=0;
	while (1) {
		cls();
		printf(" ###      ��庰,��Ʈ�� ���ӽ� ���ӱ���/��뼳��         \n");
		printf("  1.  ���ͳ����� ��� ����Ʈ                        (system/node_net.txt)\n");
		printf("  2.  ��庰 ���� �ð� ����                         (system/nodetime.txt)\n");
		printf("  3.  ��庰 ���� ��� ����                         (system/nodetype.txt)\n");
		printf("  4.  ��庰 �û� �޼��� ����                       (system/nodemess.txt)\n");
		printf("  5.  ��庰 ��� �Ұ��� �޴� ����                  (system/nodemenu.txt)\n");
		printf("  6.  ��庰 Ư�� ID�� ���Ӱ���                     (system/nodeid.txt  )\n");
		printf("  7.  ��庰 Ư�� ȣ��Ʈ�� ���� ����                (system/nodehost.txt)\n");
		printf("  9.  �������� ������ ���̵� ����                   (system/nodelog.id  \n");
		printf(" 10.  ���ӱ��� ���,ȣ��Ʈ�� ����  (��޺��������) (system/nodety?.txt \n");
		printf(" 11.  ���ӱ��� ���,ȣ��Ʈ�� ����  (��������)       (system/nodelog1.txt\n");
		printf(" 12.  ���Ӱ��� ���,ȣ��Ʈ�� ����                   (system/nodelog2.txt\n");
		printf(" 13.  ���� ���,ȣ��Ʈ�� ������ �մ�����            (system/nodelog3.txt\n");
		printf(" 14.  ������� (�մ�����)���ȳ�ȭ��               (logo/_nodeno0.log) \n");
		printf(" 15.  ���ӱ��� ���,ȣ��Ʈ�� �ȳ�ȭ��               (logo/_nodeno1.log) \n");
		printf(" 16.  ���Ϻ� ������ ��� ���̵�                     (system/_day.id7)\n");
		printf(" 17.  ���Ϻ� ��밡�� ���̵� ����                   (system/_day.id?)\n");
		printf(" 18.  ���Ϻ� ��� �Ұ��ɽ� �ȳ�ȭ��                 (logo/_dayid?.log)\n");
		printfL2();
		n=numselect();
		xy(60,n+1);
		if (n<=0) break;
		if (n==1) xedit("system/node_net.txt");
		if (n==2) xedit("system/nodetime.txt");
		if (n==3) xedit("system/nodetype.txt");
		if (n==4) xedit("system/nodemess.txt");
		if (n==5) xedit("system/nodemenu.txt");
		if (n==6) xedit("system/nodeid.txt");
		if (n==7) xedit("system/nodehost.txt");
		if (n==9) xedit("system/nodelog.id");
		if (n==10) {
			printf("\n���ѵ��? >>");
			n2=numinput();
			if (n2>=0&&n2<20) {
				sprintf(buff,"system/nodety%d.txt",n2);
				xedit(buff);
			}
		}
		if (n==11) xedit("system/nodelog1.txt");
		if (n==12) xedit("system/nodelog2.txt");
		if (n==13) xedit("system/nodelog3.txt");
		if (n==14) xedit("logo/_nodeno0.log");
		if (n==15) xedit("logo/_nodeno1.log");
		if (n==16) xedit("system/_day.id7");
		if (n==17||n==18) {
			printf("\n ���Ϻ� ��ȣ(0�� 1�� 2ȭ 3�� 4�� 5�� 6�� >>");
			n2=numinput();
			if (n2>=0&&n2<6) {
				if (n==27) sprintf(buff,"system/_day.id%d",n2);
				else	   sprintf(buff,"logo/_dayid%d.log",n2);
				xedit(buff);
			}
		}
	}
}


// ��ȣȸ ���� �۾�
job_club()
{
	char buff[80];
	char buff2[80];
	int num;
	while (1) {
		cls();
		printfL2();
		printf("          ��ȣȸ ���� �۾�  \n");
		printfL2();
		printf(" 1.  �ű� ��ȣȸ �����  \n");
		printf(" 2.  ��ȣȸ ��� �����  \n");
		printf(" 3.  ��ȣȸ ���� �ϱ�    \n");
		printf(" 4.  ��ȣȸ �û� �޴�    \n");
		printf(" 5.  ��ü ��ȣȸ ��Ȳ(����)\n");
		printf(" 6.  ��ü ��ȣȸ ��Ȳ(���)\n");
		printf(" 7.  ��ü ��ȣȸ ��Ȳ(����)\n");
		printf(" 9.  ��ȣȸ ����� ����\n");
		printfL2();
		num=numselect();
		if (num<=0) break;
		if (num==1) {
			printf("\n�۾��� ��ȣȸ �ڵ��? >>");
			lineinput(buff,8);if (strlen(buff)<2) break;
			if (find_club(buff)) {
				printf("\n %s (%s) ��ȣȸ�� �̹� %s ���丮�� �ֽ��ϴ�.\n",
						buff,club_go.name,club_go.dir);
				printf(" %s ���丮�� ��ȣȸ�� ������? (Y/n)>>",club_go.dir);
				if (yesno2("",Yes)) club_make(club_go.dir);
				else {
					mess("��ȣȸ �۾��� ����մϴ�.");
				}

			}
			else {
				printf("\n *** %s ��ȣȸ�� ������?  ",buff);
				if (yesno2(" (Y/n)>>",Yes)) {
					printf(" ��ȣȸ ���丮��? >>");
					lineinput(buff2,60);printf("\n");
					if (strlen(buff2)>1) {
						club_make(buff2);
						mess("�޴��������� ��ȣȸ�� �����ϼ���.");
					}
				}
			}
		}
		if (num==2) {
			printf("\n����� ��ȣȸ �ڵ��? >>");lineinput(buff,8);
			if (!find_club(buff)) {
				printf("\n %s ��ȣȸ�� ã���� �����ϴ�.\n",buff);
			}
			else {
				printf("\n %s (%s) (%s) \n",club_go.code,club_go.name,club_go.dir);
				if (yesno("��ȣȸ�� ����ұ��? (���:Y)",No)) {
					sys_tem2("tar cfvz %s.tgz %s &",club_go.code,club_go.dir);
				}
			}
		}
		if (num==3) {
			printf("\n������ ��ȣȸ �ڵ��? >>");lineinput(buff,8);printf("\n");
			if (strlen(buff)<1);
			else if (!find_club(buff)) {
				mess("%s ��ȣȸ�� ã���� �����ϴ�.",buff);
			}
			else {
				printf("\n %s (%s) (%s) \n",club_go.code,club_go.name,club_go.dir);
				if (yesno2("�� ��ȣȸ�� ��� �ڷḦ �����ұ��? (����:Y)",No)) {
					if (!yesno("�ѹ� �����ϸ� ���� �����Ҽ� �����ϴ�. ����ұ��? (���:Y)",Yes)) {
						printf("\n");
						sprintf(buff2,"rm %s -i -R",club_go.dir);
						sys_tem(buff2);
					}
				}
			}
		}
		if (num==4) {
			printf("\n�۾��� ��ȣȸ �ڵ��? >>");lineinput(buff,8);
			if (strlen(buff)<1);
			else if (find_club(buff)) sys_tem("bin/csysop %s %s %s",club_go.code,"top",cfg.sysop);
			else mess("%s ��ȣȸ�� ã���� �����ϴ�. ���� �޴� Ȯ��",buff);
		}
		if (num==5) view_all_club(1);
		if (num==6) view_all_club(2);
		if (num==7) view_all_club(3);
		if (num==9) {view("doc/makeclub.txt");pressenter();}
	}
}



//���ϰ���
job_mail()
{
	int n,n2,n3;
	char buff[100];
	while (1) {
		cls();
		printf("          ���� ���� �۾�\n");
		printfL2();
		printf("  1. �Ⱓ ���� ���� ����                       \n");
		printf("  2. �Ϲݸ��� ��밡�� ���                    :%d\n",cfg.mailwrite);
		printf("  3. E-MAIL ��밡�� ���                      :%d\n",cfg.emailwrite);
		printf("  4. E-MAIL ���� ���̵� �����                 :%s\n",rbit(cfg.mailmode,1,"��","����"));
		printf("  5. ���Ϲ��̳ʸ����                          :%s\n",rbit(cfg.mailmode,2,"������","�����"));
		printf("  6. ��ü���� �߼� ���� ���                   :%d\n",cfg.wmalltype );
		printf("  7. ��ü���� ��� ���� ȸ�� ����              (system/wmall.id)\n");
		printf("  8. ���� �ڵ� ���� ID/���� ����               (system/manswer.id)\n");
		printf("  9. ���� ������ ���Ҹ�                        :%s\n",rbit(cfg.mailmode,6,"�ȳ�","��"));
		printf(" 10. �ѱ� Email ID �����.                     :%s\n",rbit(cfg.mailmode,2,"��","����"));
		printf(" 11. �ѱ� Email ID ���         �ȳ�ȭ��       (frm/mail/emailh.log)\n");
		printf(" 12. Email �� �ּ����� (ȣ��Ʈ��)              (system/hostaddr.txt)\n");
		printf(" 13. ���ϻ�� ���� ���� �Է�                   (frm/mail/notmail.log)\n");
		printf(" 14. E���ϻ�� ���� ���� �Է�                  (frm/mail/notemail.log)\n");
		printf(" 15. �����б��� ���������� ���� ����           :%s\n",rbit(cfg.mailmode,7,"��","����"));
		printf(" 16. �����б��� ���������� ���� ����           :%s\n",rbit(cfg.mailmode,8,"��","����"));
		printf(" 17. �����б��� ���������� ���� ����           :%s\n",rbit(cfg.mailmode,9,"��","����"));
		printf(" 18. �����б��� ����(RE) -> ���������߰�       :%s\n",rbit(cfg.mailmode,10,"��","����"));
		n=numselect();printf("\n");
		xy(60,n+2);
		if (n<=0) break;
		if (n==1) {
			xy(1,20);
			printf("  �Ϲ����� ���ϻ��� �Ⱓ��?  (5-999��) >>");
			n2=numinput();printf("\n");
			if (n2>4&&n2<1000) {
				printf("  �������� ���ϻ��� �Ⱓ��?  (30-999��) >>");
				n3=numinput();printf("\n");
				if (n3>30&&n3<1000) {
				if (yesno2("�Ⱓ��� ���ϻ����� �����ұ��?(Y/n)>>",Yes))
					sys_tem2("bin/delmail _all_maildel %d %d &",n2,n3);
				}
			}
		}
		if (n== 2) cfg.mailwrite=get_num(cfg.mailwrite,1);
		if (n== 3) cfg.emailwrite=get_num(cfg.emailwrite,1);
		if (n== 4) cfg.mailmode=gtt_bit(cfg.mailmode,1);
		if (n== 5) cfg.mailmode=gtt_bit(cfg.mailmode,2);
		if (n== 6) cfg.wmalltype=get_num(cfg.wmalltype,2);
		if (n== 7) xedit("system/wmall.id");
		if (n== 8) xedit("system/manswer.id");
		if (n== 9) cfg.mailmode=gtt_bit(cfg.mailmode,6);
		if (n==10) cfg.mailmode=gtt_bit(cfg.mailmode,2);
		if (n==11) xedit("frm/mail/emailh.log");
		if (n==12) xedit("system/hostaddr.txt");
		if (n==14) xedit("frm/mail/notemail.log");
		if (n==15) cfg.mailmode=gtt_bit(cfg.mailmode,7);
		if (n==16) cfg.mailmode=gtt_bit(cfg.mailmode,8);
		if (n==17) cfg.mailmode=gtt_bit(cfg.mailmode,9);
		if (n==18) cfg.mailmode=gtt_bit(cfg.mailmode,10);
	}
}


job_backmode()
{
	int n;
	while (1) {
		cls();
		print_(0,"�ý��� ������� ����");
		printf("  1.  �޴������� OLD,BAK ����        :%s\n",rbit(cfg.backmode,1, "����", "����"));
		printf("  2.  ������ ����� REC �ڵ����     :%s\n",rbit(cfg.backmode,2, "���", "����"));
		printf("  3.  ȭ���Ͻ�    ȭ���ڵ����     :%s\n",rbit(cfg.backmode,3, "���", "����"));
		printf("  4.  �û�޴����Խ�    �ڵ����     :%s\n",rbit(cfg.backmode,4, "���", "����"));
		printf("  5.  ��ȣȸ  ���Խ�    �ڵ����     :%s\n",rbit(cfg.backmode,5, "���", "����"));
		printf("  9.  ���\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.backmode  =gtt_bit(cfg.backmode,1);
		if (n== 2)	cfg.backmode  =gtt_bit(cfg.backmode,2);
		if (n== 3)	cfg.backmode  =gtt_bit(cfg.backmode,3);
		if (n== 4)	cfg.backmode  =gtt_bit(cfg.backmode,4);
		if (n== 5)	cfg.backmode  =gtt_bit(cfg.backmode,5);
		if (n== 9)	job_backup();
	}
}



set_no_code()
{
	char filename[200];
	char buff[100];
	FILE *fp,*fp2;
	int found=No,i;
	struct CCODE {
		char mode;
		char code[9];
	} ccode;

	sprintf(filename,"%s/system/_nocode.dat",club_go.dir);

	key=Yes;
	while (key) {
		cls();
		printf("           �������� �����ڵ� ���        �����ڵ�(���Ѹ��)\n");
		if ((fp=fopen(filename,"r+"))!=NULL) {
			i=0;
			while (fread(&ccode,sizeof(ccode),1,fp)) {
				printf(" %-8s(%d) ",ccode.code,ccode.mode);
				if (i++%6==5) printf("\n");
			}
			if (fp!=NULL) fclose(fp);
			if (i%6!=0) printf("\n");
		}
		printfL2();
		while (key) {
			printf("  �߰�:A <�ڵ�> <���>  ����:D <�ڵ�> ���:L ����:Q  ����>>");
			lineinput(buff,12);printf("\n");
			check_cmd(buff);
			if (is_char("QqPpXx",buff[0])) key=No;
			else if (is_char("Ll",buff[0])) break;
			else if (is_char("AaIi",buff[0])) {
				if (mmc[1][0]==0||atoi(mmc[2])==0) {
					printf("\n ���� ���� : A <�����ڵ�> <���>    ��) A hello 1\n");
					printf("   �����ڵ�:������ �ڵ�  ���:�������ѽ� �޼��� ��� \n");
				}
				else {
					strcpy(ccode.code,mmc[1]);
					i=atoi(mmc[2]);if (i>9) i=9;
					ccode.mode=i;
					if ((fp=fopen(filename,"a+"))==NULL) fp=fopen(filename,"w+");
					if (fp!=NULL) {
						fwrite(&ccode,sizeof(ccode),1,fp);
						printf(" %-8s(%d)  �߰���\n",ccode.code,ccode.mode);
						fclose(fp);
					}
				}
			}
			else if (is_char("Dd",buff[0])) {
				if (mmc[1][0]==0) {
					printf("\n ���� ���� : D <�����ڵ�>   ��) D hello\n");
				}
				else {
					if ((fp=fopen(filename,"r+"))!=NULL) {
						if ((fp2=fopen(TMP_FILE,"w+"))!=NULL) {
							while (fread(&ccode,sizeof(ccode),1,fp)) {
								if (!is_same(ccode.code,mmc[1])&&ccode.mode>0&&ccode.code[0]!=0)
									fwrite(&ccode,sizeof(ccode),1,fp2);
							}
							fclose(fp);
							fclose(fp2);
							file_copy(TMP_FILE,filename);
							unlink(TMP_FILE);
						}
					}
				}
			}
		}
	}

}

set_no_logo()
{
	int n;
	char buff[100];
	char filename[200];
	while (1) {
		cls();
		printf("          ���� ���� ȭ�� �޼��� ����\n");
		printfL2();
		printf("  1. �������� �޼��� 1         (logo/_nocode1.log)\n");
		printf("  2. �������� �޼��� 2         (logo/_nocode2.log)\n");
		printf("  3. �������� �޼��� 3         (logo/_nocode3.log)\n");
		printf("  4. �������� �޼��� 4         (logo/_nocode4.log)\n");
		printf("  5. �������� �޼��� 5         (logo/_nocode5.log)\n");
		printf("  6. �������� �޼��� 6         (logo/_nocode6.log)\n");
		printf("  7. �������� �޼��� 7         (logo/_nocode7.log)\n");
		printf("  8. �������� �޼��� 8         (logo/_nocode8.log)\n");
		printf("  9. �������� �޼��� 9         (logo/_nocode9.log)\n");
		printfL2();
		n=numselect();
		sprintf(filename,"%s/logo/_nocode%d.log",club_go.dir,n);
		printf("\n");
		if (n<= 0) break;
		if (n== 1) xedit(filename);
		if (n== 2) xedit(filename);
		if (n== 3) xedit(filename);
		if (n== 4) xedit(filename);
		if (n== 5) xedit(filename);
		if (n== 6) xedit(filename);
		if (n== 7) xedit(filename);
		if (n== 8) xedit(filename);
		if (n== 9) xedit(filename);
	}
}


job_money()
{
	FILE *fp;
	int i,n,num,n2;
	char buff[100];

	while (1) {
		cls();
		printf("  ###  ���̹��Ӵ� ���� �۾�  ###\n");
		printf("  1. ȸ�� �ݾ� ����   (��)                      :%d\n",cfg.cashmax    );
		printf("  2. ������ �ݾ� ��� ���                      :%d\n",cfg.cashfree   );
		printf("  3. �б�� ���̹��Ӵ� ����                     :%s\n",rbit(cfg.cashmode,0, "��", "����"));
		printf("  4. �ٿ�� ���̹��Ӵ� ����                     :%s\n",rbit(cfg.cashmode,1, "��", "����"));
		printf("  5. ����� ���̹��Ӵ� ����                     :%s\n",rbit(cfg.cashmode,2, "��", "����"));
		printf("  6. ��Ͻ� ���̹��Ӵ� ����                     :%s\n",rbit(cfg.cashmode,3, "��", "����"));
		printf("  7. �Խ��ǻ������̹��Ӵϵ���                   :%s\n",rbit(cfg.cashmode,4, "��", "����"));
		printf("  8. �ڷ�ǻ������̹��Ӵϵ���                   :%s\n",rbit(cfg.cashmode,5, "��", "����"));
		printf("  9. 1ȸ ���� �ִ� ���� CASH (0:������)         :%d\n",cfg.cashmax1);
		printf(" 10. 1�� ���� �ִ� ���� CASH (0:������)         :%d\n",cfg.cashmax2);
		printf(" 11. ���̹��Ӵ� 0���� ȸ�� �ٿ�                 :%s\n",rbit(cfg.cashmode,6, "����", "���"));
		printf(" 12. ��üȸ�� ���̹��Ӵ� �ϰ�����               \n");
		printfL2();
		n=numselect();
		xy(60,n);
		if (n<=0)  break;
		if (n==1)  cfg.cashmax	 =get_num(cfg.cashmax	,10);
		if (n==2)  cfg.cashfree  =get_num(cfg.cashfree	, 2);
		if (n==3)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 0);
		if (n==4)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 1);
		if (n==5)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 2);
		if (n==6)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 3);
		if (n==7)  cfg.cashmode  =gtt_bit(cfg.cashmode	, 4);
		if (n==8) cfg.cashmode	 =gtt_bit(cfg.cashmode	, 5);
		if (n==9) cfg.cashmax1	 =get_num(cfg.cashmax1 ,10);
		if (n==10) cfg.cashmax2  =get_num(cfg.cashmax2 ,10);
		if (n==11) cfg.cashmode  =gtt_bit(cfg.cashmode	, 6);
		if (n==12) {
			printf("��üȸ�� ���̹�ĳ�� (���� CC) :");
			lineinput(buff,9);
			n2=atoi(buff);
			if (buff[0]==0||n2<0) continue;
			xy(1,21);
			printf("\n ���ݺ��� ��üȸ���� ���̹�ĳ�ø� �����մϴ�. ������ �ϼ��� \n\n");
			printf("    ��üȸ���� ���̹�ĳ�ø� %dCC �� �����ұ��? (Y/n)>>",n2);
			if (yesno("",Yes)==No) continue;
			fp=fopen(ID_FILE,"r+");
			i=0;
			printf("\n");
			while (1) {
				fseek(fp,i*sizeof_pf,SEEK_SET);
				if (!fread(&pf,sizeof_pf,1,fp)) break;
				printf("%-8s (%-8s)  %9d ->",pf.id,pf.name,pf.cash);
				pf.cash=n2;
				printf("%9d\n",pf.cash);
				fseek(fp,i*sizeof_pf,SEEK_SET);
				i++;
			}
			if (fp!=NULL) fclose(fp);
			pressenter();
		}
	}
}


job_ip()
{
	FILE *fp;
	int i,n,num,n2;
	char buff[100];

	while (1) {
		cls();
		printf("  ###  IP ���� ����  ###\n");
		printf("  1. IP �ڷ� ���                  :%s\n",rbit(cfg.ipmode,0,"��", "����"));
		printfL2();
		n=numselect();
		xy(60,n);
		if (n<= 0)	break;
		if (n== 1) cfg.ipmode	 =gtt_bit(cfg.ipmode  , 0);
	}
}

save_guestnot(PF pf2,int mode)
{
	FILE *fp;
	int i;
	char filename[200];
	char buff[200];
	strcpy(filename,GUESTNOT_FILE);
	if (mode==1) {
		printf("\n ��ϰźε�� ���� (���:Q)>>");
		lineinput(buff,60);check_han(buff);printf("\n");
		if (is_quit(buff)) return No;
		strcpy(pf.memo2,buff);
		mess("�ź�ó���� ���� �Ǿ����ϴ�.\n");
	}
	fp=fopen(filename,"r+");   //�ź��ھ��̵� �߰��Ѵ�.
	if (fp==NULL) fp=fopen(filename,"w+");   //�������̵� �߰��Ѵ�.
	if (fp==NULL) {
		mess("%s ȭ���� ���� �����ϴ�.",filename);
		return No;
	}
	fseekend(fp);
	i=ftell(fp)/sizeof_pf;
	if (i>=0) {
		pf2.num=i;
		fseek(fp,i*sizeof_pf,SEEK_SET);
//		  fwrite(&pf2,sizeof_pf,1,fp);
		gwrite(203,pf2,fp);
	}
	fclose (fp);
	return No;
}



notfree()
{
	FILE *fp;
	FCFG fcfg;
	int i,n,num,n2;
	while (1) {
		cls();
		printf("  ###  ��ϰź� ȸ�� �߰�,����   ###\n");
		printf("  1. ��ϰź�ȸ�� �߰�        (�ű�����)\n");
		printf("  2. ����ȸ�� -> ��ϰź�ȸ�� (�ź�����)\n");
		printf("  3. ����ȸ�� <- ��ϰź�ȸ�� (�ź�����)\n");
		printf("  4. �ź�ȸ�� ����            (�ź����)\n");
		printf("  5. �ź�ȸ�� ��ü ����       (ȭ�ϻ���)\n");
		printf("  6. �ź�ȸ�� �˻�            (�ź����)\n");
		n=numselect();
		xy(60,n);
		if (n<= 0)	break;
	}
}



job_updown()
{
	int n,i;
	char buff[100];
	while (1) {
		cls();
		print_(0,"��,�ٿ� ��������");
		printf("  1. ��  ->�ٿ�뷮 ����                          :%3.1f ��\n",(float)cfg.urate/10 );
		printf("  2. �ٿ�->�ٿ�뷮 ����                          :%3.1f ��\n",(float)cfg.drate/10 );
		printf("  3. ������ �ٿ� ���                             :%d\n",cfg.downfree   );
		printf("  4. ��Ƽ�ٿ� ���� ���                           :%d\n",cfg.downmult  );
		printf("  5. �ٿ���� �ñ� (DN�ĸ� ��ҽ� ���Ҿȵ�)     :%s\n",rbit(cfg.downmode,0,"DN��","DN��"));
		printf("  6. ���� �ڵ�����                                :%s\n",rbit(cfg.downmode,10," �� ","����"));
		printf("  7. �ڷ� ��� ȭ�ϸ��ڵ��Է�                     :%s\n",rbit(cfg.downmode,1,"�ڵ�","����"));
		printf("  8. �ڷ� ��Ͻ� ���� ���ð� ���� (2�� ����)    :%s\n",rbit(cfg.downmode,2," �� ","����"));
		printf("  9. ȭ�Ͼ����ڷ� -> ȭ�ϸ��ĭó��               :%s\n",rbit(cfg.downmode,11,"����","��ĭ"));
		printf(" 10. UPS ��ɽ� Ȯ�ξ���                          :%s\n",rbit(cfg.downmode,5,"Ȯ��","����"));
		printf(" 11. ���ϴٿ��غ�� �ӽÿ����� ���Ϻ��� ���      :%s\n",rbit(cfg.downmode,15,"����","��ũ"));
		printf(" 12. ���ٿ�� ����,�뷮 �����޼���                :%s\n",rbit(cfg.downmode,17,"���","����"));
		printf(" 13. ���� �ϵ��ܷ�(50MB) Ȯ��                     :%s\n",rbit(cfg.downmode,20,"����","Ȯ��"));
		printf(" 14. �ٿ�� �ϵ��ܷ�(50MB) Ȯ��                   :%s\n",rbit(cfg.downmode,21,"����","Ȯ��"));
		printf(" 15. ���� ��   ��� �� ����  (0:������)           :%d\n",cfg.up_limit );
		printf(" 16. ���� �ٿ� ���� �� ����  (0:������)           :%d\n",cfg.dn_limit );
		printf(" 17. ��Ƽ �ٿ� ��      ����  (0:������)           :%d\n",cfg.dn_mult_limit );
		printf(" 18. �ڷ��Ͻ� ũ�⺰ ���� ���� (1���� Byte)     :%d\n",cfg.level_size);
		printf(" 22. �ٿ� ���� �޼���/���� ����  \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.urate=get_num(cfg.urate,3);
		if (n== 2)	cfg.drate=get_num(cfg.drate,3);
		if (n== 3)	cfg.downfree   =get_num(cfg.downfree,2);
		if (n== 4)	cfg.downmult   =get_num(cfg.downmult,2);
		if (n== 5)	cfg.downmode   =gtt_bit(cfg.downmode,0);
		if (n== 6)	cfg.downmode   =gtt_bit(cfg.downmode,10);
		if (n== 7)	cfg.downmode   =gtt_bit(cfg.downmode,1);
		if (n== 8)	cfg.downmode   =gtt_bit(cfg.downmode,2);
		if (n== 9)	cfg.downmode   =gtt_bit(cfg.downmode,11);
		if (n==10)	cfg.downmode   =gtt_bit(cfg.downmode,5);
		if (n==11)	cfg.downmode   =gtt_bit(cfg.downmode,15);
		if (n==12)	cfg.downmode=gtt_bit(cfg.downmode,17);
		if (n==13)	cfg.downmode=gtt_bit(cfg.downmode,20);
		if (n==14)	cfg.downmode=gtt_bit(cfg.downmode,21);
		if (n==15)	cfg.up_limit   =get_num(cfg.up_limit,3);
		if (n==16)	cfg.dn_limit   =get_num(cfg.dn_limit,3);
		if (n==17)	cfg.dn_mult_limit=get_num(cfg.dn_mult_limit,3);
		if (n==18)	cfg.level_size =get_num(cfg.level_size,8);
		if (n==18)	down_after();
		if (n==22)	job_updown2();
	}
}


job_updown2()
{
	int n,i;
	char buff[100];
	while (1) {
		cls();
		print_(0,"�ٿ� ���� �޼���/���� ����");
		printf("  1. �ٿ�����/�ߴܽ� �۾�/ȭ��޼��� ó��          \n");
		printf("  2. �ڷ� ����ȭ�� Ȯ�������� (system/achive.txt)  \n");
		printf("  3. ��ü �ڷ�� ���� ����    (system/zbtotal.dat) \n");
		printf("  4. ���� �ٿ� ���� ��Ʈ ���� (system/_downpt.txt) \n");
		printf("  5. ���ٿ�� ���ǻ��� / ���� / �ȳ� ȭ�� ����     \n");
		printf("  6. ����ڿ��� ��/�ٿ� ���� ����                  \n");
		printf("  7. �� �ٿ� �Ķ��Ÿ                              \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) down_after();
		if (n== 2) xedit("system/achive.txt");
		if (n== 3) xedit("system/zbtotal.dat");
		if (n== 4) xedit("system/_downpt.txt");
		if (n== 5) updown_note();
		if (n== 6) updown_user();
		if (n== 7) job_para();
	}
}


down_after()
{
	int n,i1,i2;
	while (1) {
		cls();
		printf("  ### �ٿ�����/�ߴ��� ó�����/ȭ�鼳��\n");
		printf("  1. �ڷ� �ٿ��� Enter Ȯ����         :%s\n",rbit(cfg.downmode,12,"Ȯ��", "����"));
		printf("  2. �ڷ� �ٿ��� ���̹��Ӵ� ������  :%s\n",rbit(cfg.downmode,13,"����", "���"));
		printf("  3. �ڷ� �ٿ��� �ٿ��ȭ ������  :%s\n",rbit(cfg.downmode,14,"����", "���"));
		printf("  4. �ٿ� ���/�ߴܽ� ó��            :%s\n",rbit(cfg.downmode,16,"�ߴ�ó��", "����"));
		printf("  5. �ٿ��ߴܽ� �ٿ�뷮 1���ٿ��    :%s\n",rbit(cfg.downmode,9,"����", "���"));
		printf("  6. �ٿ��ߴܽ� �ٿ�뷮 ��Ƽ�ٿ��   :%s\n",rbit(cfg.downmode,10,"����", "���"));
		printf("  7. ���ٿ�� ���ǻ��� / ���� / �ȳ� ȭ�� ����     \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.downmode   =gtt_bit(cfg.downmode,12);
		if (n== 2)	cfg.downmode   =gtt_bit(cfg.downmode,13);
		if (n== 3)	cfg.downmode   =gtt_bit(cfg.downmode,14);
		if (n== 4)	cfg.downmode   =gtt_bit(cfg.downmode,16);
		if (n== 5)	cfg.downmode   =gtt_bit(cfg.downmode,9);
		if (n== 6)	cfg.downmode   =gtt_bit(cfg.downmode,10);
		if (n== 7)	updown_note();
	}
}


updown_note()
{
	int n;
	while (1) {
		cls();
		printf("  ### ��,�ٿ� ���� ȭ�� ����\n");
		printf("  1. ���� �ٿ� ���� ��Ʈ ����     (logo/_downpt.log)  \n");
		printf("  2. ����   ���ǻ���(��ü ��)     (logo/_up.scr)      \n");
		printf("  3. ����   ��������(��ü ��)     (logo/_up.qsh)      \n");
		printf("  4. �ٿ�� ���ǻ���(��ü�ٿ�)    (logo/_dn.scr)      \n");
		printf("  5. ����   ���ǻ���(��ü ��)     (logo/_up2.scr)     \n");
		printf("  6. �ٿ��� ���ǻ���(��ü ��)     (logo/_dn2.scr)     \n");
		printf(" 11. ���� ��   ��� �� ���� �޼���(frm/editor/up_limit.log)\n");
		printf(" 12. ���� �ٿ� ���� �� ���� �޼���(frm/editor/dn_limit.log)\n");
		printf(" 13. ��Ƽ �ٿ� ��      ���� �޼���(frm/down/dn_mult.log)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	xedit("logo/_downpt.log");
		if (n== 2)	xedit("logo/_up.scr");
		if (n== 3)	xedit("logo/_up.qsh");
		if (n== 4)	xedit("logo/_dn.scr");
		if (n== 5)	xedit("logo/_up2.scr");
		if (n== 6)	xedit("logo/_dn2.scr");
		if (n==11)	xedit("frm/editor/up_limit.log");
		if (n==12)	xedit("frm/editor/dn_limit.log");
		if (n==13)	xedit("frm/down/dn_mult.log");
	}
}


updown_user()
{
	int n;
	while (1) {
		cls();
		printf("  ### ����� ���� ���� ����\n");
		printf("  1. USERROOM���� ȭ��UP   �뷮����     :%s\n",rbit(cfg.downmode,3,"����","��"));
		printf("  2. USERROOM���� ȭ��UP�� ȭ�ϻ���     :%s\n",rbit(cfg.downmode,4,"����","��"));
		xy(1,20);
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.downmode=gtt_bit(cfg.downmode,3);
		if (n== 2)	cfg.downmode=gtt_bit(cfg.downmode,4);
	}
}



job_para()
{
	int n,i;
	char buff[100];
	while (1) {
		del_esc(cfg.jpgsz);del_esc(cfg.jpgstr);
		cls();
		print_(0,"��,�ٿ� �Ķ��Ÿ ����");
		printf(" 1. �ٿ�(����SZ) ��� :%s\n",cfg.sz);
		printf(" 2. ��  (���RZ) ��� :%s\n",cfg.rz);
		printf(" 3. KERMIT�ٿ�   ��� :%s\n",cfg.kermitsz);
		printf(" 4. KETMIT��     ��� :%s\n",cfg.kermitrz);
		printf(" 5. ȭ������ (sz)��� :%s\n",cfg.jpgsz);
		printf(" 6. ȭ�����۾Ƚù��ڿ�:%s\n",cfg.jpgstr);       //        printf("7\n8[5;5H");
		printf(" 7. �ڷ� ��/�ٿ�� ���۰�� :%s\n",rbit(cfg.downmode,22,"ó��","����"));
		printf("99. �⺻ �������� �����ϱ� (DEFAULT)\n");
		printf("    (ESC����ǥ�� \\027 ��������  ��\\024\\n)\n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	{xy(30,n+1);get_str(cfg.sz,80);}
		if (n== 2)	{xy(30,n+1);get_str(cfg.rz,80);}
		if (n== 3)	{xy(30,n+1);get_str(cfg.kermitsz,80);}
		if (n== 4)	{xy(30,n+1);get_str(cfg.kermitrz,80);}
		if (n== 5)	{xy(30,n+1);get_str(cfg.jpgsz,80);}
		if (n== 6)	{xy(30,n+1);get_str(cfg.jpgstr,80);}
		if (n== 7)	cfg.downmode=gtt_bit(cfg.downmode,22);
		if (n==99)	{
			strcpy(cfg.sz,"sz");
			strcpy(cfg.rz,"rz");
			strcpy(cfg.kermitsz,"kermit -s");
			strcpy(cfg.kermitrz,"kermit -r");
			strcpy(cfg.jpgsz,"sz");
			strcpy(cfg.jpgstr,"\\024\\n");
		}
	}
}

job_zbtotal()
{
	int n,i;
	char buff[100];
	while (1) {
		cls();
		print_(0,"��ü�ڷ�� ����");
		printf(" 1. ��ü�ڷ�� �۾���Ȳ ���         :%s\n",rbit(cfg.zmode,1,"����","���"));
		printf(" 2. ��ü�Խ��Ǹ� �۾�                :%s\n",rbit(cfg.zmode,2,"����","����"));
		printf(" 3. ��ü�Խ��Ǹ� �۾�                :%s\n",rbit(cfg.zmode,3,"����","����"));
		printf(" 4. �������� �˻翩��                :%s\n",rbit(cfg.zmode,4,"��","����"));
		printf(" 5. ��ü�ڷ�����Խ� 1�ð� ����Ȯ��  :%s\n",rbit(cfg.zmode,21,"����","Ȯ��"));
		printf(" 6. ��ü�ڷ�����Խ� 6�ð� ����Ȯ��  :%s\n",rbit(cfg.zmode,22,"����","Ȯ��"));
		printf(" 7. ���۽�           1�ð� ����Ȯ��  :%s\n",rbit(cfg.zmode,23,"����","Ȯ��"));
		printf(" 8. ���۽�           6�ð� ����Ȯ��  :%s\n",rbit(cfg.zmode,24,"����","Ȯ��"));
		printf(" 9. ����/�ٿ��      30��  ����Ȯ��  :%s\n",rbit(cfg.zmode,25,"����","Ȯ��"));
		printf("10. ��ü �ڷ�� ���� ����    (system/zbtotal.dat) \n");
		printf("�������Ӹ�� 11.(%s) ",rbit2(cfg.zmode,11,"o"," "));
		printf("12.(%s) ",rbit2(cfg.zmode,12,"o"," "));
		printf("13.(%s) ",rbit2(cfg.zmode,13,"o"," "));
		printf("14.(%s) ",rbit2(cfg.zmode,14,"o"," "));
		printf("15.(%s) ",rbit2(cfg.zmode,15,"o"," "));
		printf("16.(%s) ",rbit2(cfg.zmode,16,"o"," "));
		printf("17.(%s)\n",rbit2(cfg.zmode,17,"o"," "));
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.zmode=gtt_bit(cfg.zmode,1);
		if (n== 2)	cfg.zmode=gtt_bit(cfg.zmode,2);
		if (n== 3)	cfg.zmode=gtt_bit(cfg.zmode,3);
		if (n== 4)	cfg.zmode=gtt_bit(cfg.zmode,4);
		if (n== 5)	cfg.zmode=gtt_bit(cfg.zmode,21);
		if (n== 6)	cfg.zmode=gtt_bit(cfg.zmode,22);
		if (n== 7)	cfg.zmode=gtt_bit(cfg.zmode,23);
		if (n== 8)	cfg.zmode=gtt_bit(cfg.zmode,24);
		if (n== 9)	cfg.zmode=gtt_bit(cfg.zmode,25);
		if (n==10)	xedit("system/zbtotal.dat");
		if (n==11)	cfg.zmode=gtt_bit(cfg.zmode,11);
		if (n==12)	cfg.zmode=gtt_bit(cfg.zmode,12);
		if (n==13)	cfg.zmode=gtt_bit(cfg.zmode,13);
		if (n==14)	cfg.zmode=gtt_bit(cfg.zmode,14);
		if (n==15)	cfg.zmode=gtt_bit(cfg.zmode,15);
		if (n==16)	cfg.zmode=gtt_bit(cfg.zmode,16);
		if (n==17)	cfg.zmode=gtt_bit(cfg.zmode,17);
	}
}

//��ǥ��
job_vote()
{
	char buff[100];
	while (1) {
		cls();
		print_(1,"��ǥ ���� �۾�");
		printf("  1. ��ǥ ���� ���� ���              :%d\n",cfg.vote_type);
		printf("  2. ��ǥ ���� ���� ���              :%d\n",cfg.vote_opentype);
		printf(" 10. ��ȣȸ ��ǥ�� ��뿩��           :%s\n",rbit(cfg.vote,1,"���","����"));
		n=numselect();
		xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) cfg.vote_type	=get_num(cfg.vote_type	  ,2);
		if (n== 2) cfg.vote_opentype=get_num(cfg.vote_opentype,2);
		if (n==10) cfg.vote 		=gtt_bit(cfg.vote		 ,10);
	}
}



job_setbbs()
{
	int n,i;
	char buff[300];
	while (1) {
		cls();
		print_(0,"�Խ���,������ ���ü���");
		printf(" 1.  BDN(�Խ��Ǵٿ�) ��� ���� ���̵�   (system/bdn_id.dat)\n");
		printf(" 2.  �Խù�->bbs_all �ڷ��->pds_all    :%s\n",rbit(cfg.mode,24,"�ڵ�����","����"));
		printf(" 3.  �Խ��� ���ڸ���ȣ���ý� -> �ڵ�����:%s\n",rbit(cfg.mode,10,"�ڵ�����","����"));
		printf(" 4.  �Խ��� ������ �ٿ�/��ȸ/��¥ ����  :%s\n",rbit(cfg.bbsmode,3,"�����Ұ�","��������"));
		printf(" 5.  �Խ��� ������ ��ġ,����ũ�� ����   :%s\n",rbit(cfg.bbsmode,4,"�����Ұ�","��������"));
		printf(" 6.  ����� ���λ��� ÷��               :%s\n",rbit(cfg.mode,9,"���","����"));
		printf(" 7.  �ڷ�� ũ�� 0�� �ڷ� �Ⱥ��̱�      :%s\n",rbit(cfg.bbsmode,5,"�ڷ�Ǻ�����","��ü����"));
		printf(" 8.  ������2 -> ���������� ����         :%s\n",rbit(cfg.bbsmode,6,"�ܺ�������","����������"));
		printf(" 9.  W���� R�б� D�ٿ� C��ȭ S��ȣȸ ����/����(system/limit.id)\n");
		printf("10.  �ڵ���õ/�ڵ����� ���� ����        \n");
		n=numselect();xy(60,n+1);
		if (n<= 0) break;
		if (n== 1) xedit("system/bdn_id.dat");
		if (n== 2) cfg.mode=gtt_bit(cfg.mode,24);
		if (n== 3) cfg.mode=gtt_bit(cfg.mode,10);
		if (n== 4) cfg.bbsmode=gtt_bit(cfg.bbsmode,3);
		if (n== 5) cfg.bbsmode=gtt_bit(cfg.bbsmode,4);
		if (n== 6) cfg.mode=gtt_bit(cfg.mode,9);
		if (n== 7) cfg.bbsmode=gtt_bit(cfg.bbsmode,5);
		if (n== 8) cfg.bbsmode=gtt_bit(cfg.bbsmode,6);
		if (n== 9) set_limit_id();
		if (n==10) job_good();
	}
}

job_good()
{
	char buff[300];
	while (1) {
		cls();
		print_(1,"�Խù�/�ڷ�� ��õ ����");
		printf("  1. �ڵ���õ/������� ���۸�� :%s\n",rbit(cfg.bbsmode,0,"����","����"));
		printf("  2. �ڵ���õ/������� ������ :%s\n",rbit(cfg.bbsmode,1,"����","����"));
		printf("  3. GD(��õ���)�� ��õ����    (%-10s) \n",cfg.gdstr);
		printf(" 11. ��õǥ�ú� ���� ����  %s\n",GOOL_FILE);
		printf(" 12. �Խ����б�->�ڵ� ��õ Ƚ��,ǥ�ù���,���� ���� \n");
		printf(" 13. �ڷ�Ǵٿ�->�ڵ� ��õ Ƚ��,ǥ�ù���,���� ���� \n");
		printf(" 14. �Խ�������->�ڵ� ��õ Ƚ��,ǥ�ù���,���� ���� \n");
		printf(" 15. �ڷ������->�ڵ� ��õ Ƚ��,ǥ�ù���,���� ���� \n");
		n=numselect();
		xy(60,n+1);
		if (n<= 0)	break;
		if (n== 1) cfg.bbsmode=gtt_bit(cfg.bbsmode,0);
		if (n== 2) cfg.bbsmode=gtt_bit(cfg.bbsmode,1);
		if (n== 3) get_str(cfg.gdstr,10);
		if (n==11) cfg_gool();
		if (n>=12&&n<=15) cfg_good(n-12);
	}
}




cfg_good(int mode)
{
	FILE *fp;
	char buff[90];
	int n,n2,i,i2;

	if ((fp=fopen(GOOD_FILE,"r+"))!=NULL) {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				fread(&agood[i][i2],sizeof_agood,1,fp);
			}
		}
		fclose(fp);
	}
	while(1) {
		cls();
		i=mode;
		if (i==0) print_(0,"�Խ����б� �ڵ� ��õ ���� %s",GOOD_FILE);
		if (i==1) print_(0,"�ڷ�Ǵٿ� �ڵ� ��õ ���� %s",GOOD_FILE);
		if (i==2) print_(0,"�Խ������� �ڵ� ��õ ���� %s",GOOD_FILE);
		if (i==3) print_(0,"�ڷ������ �ڵ� ��õ ���� %s",GOOD_FILE);
		for(i2=0;i2<10;i2++) {
			printf(" %2d:",i2+1);
			bzero(buff,10);buff[0]=agood[i][i2].mark;
			printf(" ǥ�ù��� %2s ",buff);
			printf(" ǥ�ü��� %-8s ",acolor[i2].name);
			printf(" �ڵ���õ�� %4d ",agood[i][i2].count);
			printf(" ���ڻ�:%2d  ����:%2d [s%s[ ����� ][u\n",
			acolor[i2].color_char,acolor[i2].color_back,get_gool(agood[i][i2].mark));
		}
		printf(" ����(Q,W)>>");
		lineinput(buff,10);check_han(buff);printf("\n");
		i2=atoi(buff)-1;
		if (is_quit(buff)) break;
		if (is_same(buff,"q")) break;
		if (is_same(buff,"w")) break;
		if (is_same(buff,"x")) break;
		if (is_same(buff,"xx")) break;
		if (buff[0]==0) continue;
		if (i2<0||i2>10) continue;
		printf(" %2d:",i2+1);
		bzero(buff,10);buff[0]=agood[i][i2].mark;
		printf(" ǥ�ù���   :%2s ����ǥ�ù��� ",buff);
		lineinput(buff,2);printf("\n");
		if (buff[0]!=0) agood[i][i2].mark=buff[0];
		printf(" �ڵ���õ�� :%4d ������õȽ�� ",agood[i][i2].count);
		agood[i][i2].count=get_num(agood[i][i2].count,999999);printf("\n");
		bzero(buff,10);buff[0]=agood[i][i2].mark;
	}

	if ((fp=fopen(GOOD_FILE,"w+"))!=NULL) {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				fwrite(&agood[i][i2],sizeof_agood,1,fp);
			}
		}
		fclose(fp);
	}

}


cfg_gool()
{
	char buff[90];
	int n,n2,i,i2;
	FILE *fp;

	//�ش�ȭ�������б�
	if ((fp=fopen(GOOL_FILE,"r+"))!=NULL) {
		for(i=0;i<10;i++) {
			fread(&acolor[i],sizeof_acolor,1,fp);
		}
		fclose(fp);
	}

	while(1) {
		cls();
		print_(1,"��õǥ�ú� ���� ���� %s",GOOL_FILE);
		for(i2=0;i2<10;i2++) {
			printf(" %2d:",i2+1);
			bzero(buff,10);buff[0]=acolor[i2].mark;
			printf(" ǥ�ù��� %2s ",buff);
			printf(" ǥ�ü��� %-8s ",acolor[i2].name);
			printf(" ���ڻ�:%2d  ����:%2d [s%s[ ����� ][u\n",acolor[i2].color_char,acolor[i2].color_back,get_gool(acolor[i2].mark));
		}
		printf(" ����(Q,W)>>");
		lineinput(buff,10);check_han(buff);printf("\n");
		i2=atoi(buff)-1;
		if (is_quit(buff)) break;
		if (is_same(buff,"q")) break;
		if (is_same(buff,"w")) break;
		if (is_same(buff,"x")) break;
		if (is_same(buff,"xx")) break;
		if (buff[0]==0) continue;
		if (i2<0||i2>10) continue;
		printf(" %2d:",i2+1);
		bzero(buff,10);buff[0]=acolor[i2].mark;
		printf(" ǥ�ù���   :%2s ����ǥ�ù��� ",buff);
		lineinput(buff,2);printf("\n");
		if (buff[0]!=0) acolor[i2].mark=buff[0];
		printf(" ǥ�ü���   :%-8s ����ǥ�ü��� ",acolor[i2].name);
		lineinput(buff,8);printf("\n");
		if (buff[0]!=0) strcpy(acolor[i2].name,buff);
		printf(" ��õ���ڻ� :%2d �������   ",acolor[i2].color_char);
		acolor[i2].color_char=get_num(acolor[i2].color_char,16);printf("\n");
		printf(" ��õ���� :%2d ������ڻ�   ",acolor[i2].color_back);
		acolor[i2].color_back=get_num(acolor[i2].color_back,16);printf("\n");
	}

	//�ش�ȭ����������
	if ((fp=fopen(GOOL_FILE,"w+"))!=NULL) {
		for(i=0;i<10;i++) {
			fwrite(&acolor[i],sizeof_acolor,1,fp);
		}
		fclose(fp);
	}
}


job_menu()
{
	while (1) {
		cls();
		print_(0,"�޴�����");
		printf("   1. ���θ޴�����                     :%s\n",MENU_FILE);
		printf("   2. �����ڵ� �Է½� GO ��� �˻�   :%s\n",rbit(cfg.gomode,1,"�˻�", "����"));
		printf("   3. �����ڵ� �Է½� ���� ���� �˻�   :%s\n",rbit(cfg.gomode,2,"�˻�", "����"));
		printf("   4. ��Į �޴� ����\n");
		num=numselect();
		xy(60,n+1);
		if (num<= 0) break;
		if (num== 1) sys_tem("bin/ed_menu %s main",now_id,club_go.code);
		if (num== 2) cfg.gomode=gtt_bit(cfg.gomode,1);
		if (num== 3) cfg.gomode=gtt_bit(cfg.gomode,2);
		if (num== 4) sys_tem("bin/pf '%s' set_localmenu",pf.id);
	}
}


sort_pf()
{
	int num;
	while (1) {
		cls();
		printfL2();
		printf("###  ȸ�� �����۾�  ###\n");
		printf("  1. ȸ�� ��ȣ ������  (�ߺ�ȸ����ȣ/Ư������ID �� ������)  \n");
		printf("  2. ������ ȸ�� �ڷ� Ȯ�� (���0,����ó�� ȸ�� Ȯ��)       \n");
		printf("  3. ��� 0 �� ȸ�� -> ���� ó�� (���̵�/���� -> ����ó��   \n");
		printf("  4. ��� 0 �� ȸ�� -> ���� ó�� (�ֹε�Ϲ�ȣ ������ó��)\n");
		printf("  5. �����Ⱓ ���� ����ȸ�� ���� ó�� -> ��� 0 ó��        \n");
		printf("  6. ��� 9 �ش�       ȸ�� ���� ó�� -> ��� 0 ó��        \n");
		printf(" *7. ���������� ȸ�� (ID,���� ��ĭ)   -> ���� ����ó��      \n");
		printf("  9. ȸ�� �ڷ� ��ü �Ϸ� ��ȣ(����ó����) ����              \n");
		printfL2();
		num=numselect();
		if (num<= 0) break;
		if (num== 1) renum_pf();
		if (num== 2) disp_del_pf();
		if (num== 3) pack_del_pf(1);
		if (num== 4) pack_del_pf(2);
		if (num== 5) pack_del_pf(3);
		if (num== 6) pack_del_pf(4);
		if (num== 7) pack_del_pf(5);
		if (num== 9) renum_proccode();	 //ȸ�� ������
	}
}



job_news()
{
	int i,n;
	char buff1[200];

	while (1) {
		cls();
		printf("  1.  ������� ���ۿ���                         :%s\n",rbit(cfg.newsmode,0,"����","�Ұ�"));
		printf("  2.  ����Ÿȭ�� �����ڵ�                       :%s\n",cfg.newsfile);
		printf("  3.  �������� ���� ���        (0:�մ� 19:�û�):%d\n",cfg.newstype);
		printf("  4.  �������� �ֽ��ڷ�ð�     (0:���� #:�ð�) :%d\n",cfg.newsoldtime);
		printf("  9.  ȭ�鰻�Ž� �˶��ñ׳�����(����..)         :%s\n",rbit(cfg.logomode,10, "����", "����"));
		printf(" 10.  ���θ޴�     ���� ����                    :%s\n",rbit(cfg.newsmode,1,"����","�ź�"));
		printf(" 11.  �����޴�     ���� ����                    :%s\n",rbit(cfg.newsmode,2,"����","�ź�"));
		printf(" 12.  �Խ���       ���� ����                    :%s\n",rbit(cfg.newsmode,3,"����","�ź�"));
		printf(" 13.  ��ȣȸ����   ���� ����                    :%s\n",rbit(cfg.newsmode,4,"����","�ź�"));
		printf(" 14.  ��ȣȸ����   ���� ����                    :%s\n",rbit(cfg.newsmode,5,"����","�ź�"));
		printf(" 15.  ��ȣȸ�Խ��� ���� ����                    :%s\n",rbit(cfg.newsmode,6,"����","�ź�"));
		printf(" 16.  ��õ�� �ڷḸ ����                        :%s\n",rbit(cfg.newsmode,7,"����","��õ"));
		printf(" 17.  *.TA? ����TOP������ ���                  :%s\n",rbit(cfg.newsmode,8,"TOP��","����"));
		printfL2();
		n=numselect();
		if (n<= 0)	break;
		if (n== 1)	cfg.newsmode =gtt_bit(cfg.newsmode,0);
		if (n== 2)	{xy(2,20);get_str(cfg.newsfile,8);}
		if (n== 3)	cfg.newstype  =get_num(cfg.newstype,2 );
		if (n== 4)	cfg.newsoldtime=get_num(cfg.newsoldtime,5);
		if (n== 9)	cfg.logomode  =gtt_bit(cfg.logomode,10);
		if (n==10)	cfg.newsmode =gtt_bit(cfg.newsmode,1);
		if (n==11)	cfg.newsmode =gtt_bit(cfg.newsmode,2);
		if (n==12)	cfg.newsmode =gtt_bit(cfg.newsmode,3);
		if (n==13)	cfg.newsmode =gtt_bit(cfg.newsmode,4);
		if (n==14)	cfg.newsmode =gtt_bit(cfg.newsmode,5);
		if (n==15)	cfg.newsmode =gtt_bit(cfg.newsmode,6);
		if (n==16)	cfg.newsmode =gtt_bit(cfg.newsmode,7);
		if (n==17)	cfg.newsmode =gtt_bit(cfg.newsmode,8);
	}
}


del_mail()
{
	FILE *fp;
	int dday;

	dday=30;
	if (dday==0) return;

	read_cfg();
	set_tmpfile();

	if ((fp=fopen("id/id.dat","r"))!=NULL) {
		while (fread(&pf,sizeof_pf,1,fp)) {
			if (pf.type>=18) continue;
			sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf.id),pf.id);
			sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf.id),pf.id);
			if (is_file(NOW_FILE)) bbs_old_del(dday);
			sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf.id),pf.id);
			sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf.id),pf.id);
			if (is_file(NOW_FILE)) bbs_old_del(dday);
		}
		fclose(fp);
	}
	printf("\n");
}


//������ �Խù� ����
bbs_old_del(int dday)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char buff[180];
	char c;
	char str[10];
	int found=No;
	int ret,i,new_posi;
	time_t today;
	time_t today1;
	time_t today2;
	int bbs_num;

	time(&today);	  //������ �ð�
	today1 = today - (dday*24*3600);   //������� ��¥

	fp1=fopen(NOW_FILE,"r");      //����ȭ��
	fp2=fopen(NOW_FILED,"r");      //����ȭ��
	fp3=fopen(TMP_FILE,"w");      //�ӽ�ȭ��
	fp4=fopen(TMP_FILE2,"w");      //�ӽ�ȭ��

	new_posi=0;
	bbs_num=0;
	while (fread(&bbs,sizeof_bbs,1,fp1)) {
		if (bbs.date<today1) {
			sprintf(buff,"userdata/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
			unlink(buff);
			continue;
		}
		fseek(fp2,bbs.posi,SEEK_SET);
		fseek(fp4,new_posi,SEEK_SET);
		for (i=0;i<bbs.size;i++) fputc(fgetc(fp2),fp4);
		bbs.posi=new_posi;
		new_posi+=bbs.size;
		bbs.num=++bbs_num;
		fwrite(&bbs,sizeof_bbs,1,fp3);
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (fp3!=NULL) fclose(fp3); if (fp4!=NULL) fclose(fp4);

	file_move(TMP_FILE ,NOW_FILE );
	file_move(TMP_FILE2,NOW_FILED);
	if (file_size(NOW_FILE)==0) {
		unlink(NOW_FILE );
		unlink(NOW_FILED);
	}
}

int apf_level[20];
int atoday_tim1[20];
int atoday_tim2[20];
int atoday_down[20];
GG gg[10];

edit_atype(int mode) {
	char buff[80];
	while (1) {
		disp_type();
		edit_type(mode);
		disp_type();
		while (1) {
			xy(5,25);
			printf("   ����:W  ���:Q  ���Է�:R �⺻��޸�:D  ����>>");
			lineinput(buff,2);
			printf("\n");
			if (is_char("wWqQrRDd",buff[0])) break;
		}
		if (is_same(buff,"w")) {
			save_atype();
			save_pftype();
			break;
		}
		if (is_same(buff,"d")) {
			def_atype();
		}
		xy(5,26);
		if (is_same(buff,"Q")&&yesno("������ ����ұ��? (Y/n) >>",Yes)) {
			break;
		}
	}
}



disp_type()
{
	int i;
	cls();
	printf("           ȸ�� ��޺� ȯ�漳�� \n");
	printfL2();
	printf(" ���    ����̸�    ���Է��� �������ѽð� 1ȸ��������\n");
	printfL2();
	for (i=0;i<20;i++) {
		disp_type2(i);
	}
}


disp_type2(int i)
{
		xy(1,i+5);
		if (i>=14)
			printf("  %2d    %-8s\n",
			i,atype[i],apf_level[i]);
		else
			printf("  %2d    %-8s  %8d  %8d  %8d\n",
			i,atype[i],apf_level[i],atoday_tim1[i],atoday_tim2[i]);
}


edit_type(int mode)
{
	int i;
	char buff[50];
	if (mode==1) {
		for (i=0;i<20;i++) {
			xy(9,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) strcpy(atype[i],buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
			del_space(atype[i]);
		}
	}
	else if (mode==2) {
		for (i=0;i<14;i++) {
			xy(20,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) apf_level[i]=atoi(buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
		}
	}
	else if (mode==3) {
		for (i=0;i<14;i++) {
			xy(30,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) atoday_tim1[i]=atoi(buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
		}
	}
	else if (mode==4) {
		for (i=0;i<14;i++) {
			xy(40,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) atoday_tim2[i]=atoi(buff);
			disp_type2(i);
			if (is_same(buff,"q")) break;
		}
	}
}



save_atype()
{
	char type_file[140];
	FILE *fp;
	int i;
	sprintf(type_file,"system/typename.dat");
	fp=fopen(type_file,"w");
	for (i=0;i<20;i++) fwrite(&atype[i],sizeof(atype[i]),1,fp);
	if (fp!=NULL) fclose(fp);
}


read_pftype()  //ȸ���� ���,���ϻ�밡�ɽð��� Ȯ���Ѵ�.
{
	int old_type;
	int i,n;
	FILE *fp;
	for (i=0;i<20;i++) apf_level[i]=999999;
	for (i=0;i<20;i++) atoday_tim1[i]=99999;
	for (i=0;i<20;i++) atoday_tim2[i]=99999;
	for (i=0;i<20;i++) atoday_down[i]=99999;
	fp=fopen("system/pftype.dat","r");   //���ȭ��
	if (fp==NULL) return;
	for (i=0;i<14;i++) fread(&apf_level  [i],sizeof(apf_level  [i]),1,fp);
	for (i=0;i<14;i++) fread(&atoday_tim1[i],sizeof(atoday_tim1[i]),1,fp);
	for (i=0;i<14;i++) fread(&atoday_tim2[i],sizeof(atoday_tim2[i]),1,fp);
	for (i=0;i<14;i++) fread(&atoday_down[i],sizeof(atoday_down[i]),1,fp);
	if (fp!=NULL) fclose(fp);
}


save_pftype()  //ȸ���� ���,���ϻ�밡�ɽð��� Ȯ���Ѵ�.
{
	int old_type;
	int i,n;
	FILE *fp;
	fp=fopen("system/pftype.dat","w");   //���ȭ��
	if (fp==NULL) return;
	for (i=0;i<14;i++) fwrite(&apf_level  [i],sizeof(apf_level	[i]),1,fp);
	for (i=0;i<14;i++) fwrite(&atoday_tim1[i],sizeof(atoday_tim1[i]),1,fp);
	for (i=0;i<14;i++) fwrite(&atoday_tim2[i],sizeof(atoday_tim2[i]),1,fp);
	for (i=0;i<14;i++) fwrite(&atoday_down[i],sizeof(atoday_down[i]),1,fp);
	fclose(fp);
}


def_atype()
{
	strcpy(atype[0],"�մ�");
	strcpy(atype[1],"ȸ��1");
	strcpy(atype[2],"ȸ��2");
	strcpy(atype[3],"ȸ��3");
	strcpy(atype[4],"ȸ��4");
	strcpy(atype[5],"ȸ��5");
	strcpy(atype[6],"ȸ��6");
	strcpy(atype[7],"ȸ��7");
	strcpy(atype[8],"ȸ��8");
	strcpy(atype[9],"ȸ��9");
	strcpy(atype[10],"ȸ��10");
	strcpy(atype[11],"ȸ��11");
	strcpy(atype[12],"ȸ��12");
	strcpy(atype[13],"ȸ��13");
	strcpy(atype[14],"��ȣ�û�");
	strcpy(atype[15],"�ο��");
	strcpy(atype[16],"���");
	strcpy(atype[17],"�û���");
	strcpy(atype[18],"�νû�");
	strcpy(atype[19],"�û�");
}



edit_ajob()
{
	int i;
	char buff[50];
	FILE *fp1;

	read_ajob();	//�����б�
	cls();
	xy(1,1);printf("              ��ȣ�� ������ ����\n");
	printfL2();
	for (i=1;i<19;i++) printf("%6d. %s\n",i,ajob[i]);
	printfL2();
	for (i=1;i<19;i++) {
		xy(20,i+2);
		printf(" ����>>");
		lineinput(buff,10);
		if (is_quit(buff)) break;
		if (buff[0]!=0) strcpy(ajob[i],buff);
	}
	if (!is_same(buff,"q")) {
		fp1=fopen("system/ajob.dat","w");
		for (i=1;i<19;i++) fwrite(&ajob[i],sizeof(ajob[i]),1,fp1);
		if (fp1!=NULL) fclose(fp1);
	}
}


edit_addr()
{
	xedit("frm/guest/_post.txt");
}


gg_read()
{
	FILE *fp;
	int i;

	for (i=0;i<10;i++) {
		strcpy(gg[i].code,"");
		strcpy(gg[i].name,"");
	}

	fp=fopen("system/gg.dat","r+");
	if (fp!=NULL)
		for (i=0;i<10;i++) fread(&gg[i],sizeof_gg,1,fp) ;
	if (fp!=NULL) fclose(fp);
}


disp_gg()
{
	int i;
	printf("\n");
	printfL2();
	for (i=0;i<10;i++)
		printf("       [  G%-2d ] %-20s ( %-8s )\n",i+1,gg[i].name,gg[i].code);
}

edit_gg()
{
	char buff[30];
	FILE *fp;
	int i;

	gg_read();
	while (1) {
		printf("\n");
		printfL2();
		printf("     ��õ ��ȣȸ/�Խ��� ���� \n");
		disp_gg();
		printfL2();
		printf("    ������ ��õ ��ȣ  ����:0 >>");
		lineinput(buff,4);
		i=atoi(buff);
		if (i==0) {
			buff[0]=' ';
			i=atoi(buff);
		}
		if (i<1||i>10) break;
		printf("\n ����  %d. �����̸� >>",i);
		lineinput(buff,20);
		if (buff[0]!=0) strcpy(gg[i-1].name,buff);
		printf("\n ����  %d. �����ڵ� >>",i);
		lineinput(buff,8);
		if (buff[0]!=0) strcpy(gg[i-1].code,buff);
	}
	fp=fopen("system/gg.dat","w");
	for (i=0;i<10;i++) fwrite(&gg[i],sizeof_gg,1,fp) ;
	if (fp!=NULL) fclose(fp);
	printf("\n\n  ������ �Ϸ�Ǿ����ϴ�. \n\n");
	getchar();
}






check_init_dir()
{
	check_dir("bbsdata/okrec");
	check_dir("menurec");
	check_dir("menucfg");
	check_dir("/tmp/xhost/rec");
	check_dir("/tmp/monich");
	check_dir("id");
	check_dir("menu");
	check_dir("sys_back");
	check_dir("userdata");
	check_dir("userroom");
	check_dir("user");
	check_dir("user/rec");
	check_dir("user/log");
	check_dir("user/score");
	check_dir("user/chat");
	check_dir("user/monich");
	check_dir("user/news");
	check_dir("user/fcmd");
	check_dir("user/moni");
	check_dir("user/lock");
	check_dir("user/message");
	check_dir("tmp");
	system("chmod 777 /tmp/xhost -R &");
	system("chmod 777 /tmp/system* &");
	system("chmod 777 /tmp/monich -R &");
}


rawmode()
{
	struct termio tdes;
	ioctl(0,TCGETA,&tdes);	 //���� �͹̳� ��带 �о�´�
	tdes.c_iflag = 0;
	tdes.c_iflag |= ICRNL ;
	tdes.c_iflag |= IXON ;
	tdes.c_oflag = 0;
	tdes.c_oflag |= OPOST ;
	tdes.c_oflag |= ONLCR ;
	tdes.c_cflag |= CRTSCTS ;
	tdes.c_cflag |= HUPCL ;
	tdes.c_lflag = 0;
	tdes.c_lflag |= IEXTEN;
	tdes.c_cc[VMIN]=1;
	tdes.c_cc[VTIME]=0;
	ioctl(0,TCSETA,&tdes);	 //���� �͹̳� ��带 �����Ѵ�.
}



view_daypf()		//�Ϻ� ������Ȳ
{
	FILE *fp,*fp2;
	time_t indate;
	int inday;
	int i,n,count1,count2;
	fp=fopen(ID_FILE,"r");
	i=0;
	printf("\n");
	indate=0;
	inday=count1=count2=0;
	fp2=fopen(TMP_FILE,"w");
	while (fread(&pf,sizeof_pf,1,fp)) {
		if (pf.inday<1) continue;
		n=time_num(pf.inday);
		if (inday>0&&inday!=n) {
			fprintf(fp2,"%s %6d�� ����  ����%6d��\n",datestr(35,indate),count1,count2);
			count1=0;
		}
		indate=pf.inday;
		inday=n;
		count1++;
		count2++;
	}
	fclose(fp);
	fclose(fp2);
	view_text(TMP_FILE);
}


view_all_club(int mode)
{
	FILE *fp1,*fp2;
	char id_file[300];
	char buff[200],a_code[1000][70];
	int anum[100],found;
	int i,count=0,club_count=0;
	for (i=0;i<100;i++) anum[i]=0;
	if ((fp1=fopen(MENU_FILE,"r"))==NULL) return;
	if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
	printf("\n ... ��ø� ��ٷ� �ֽʽÿ�. ��ȣȸ�� �˻����Դϴ�....\n");
	club_mode=Yes;
	fprintf(fp2,"������������������������������������������������������������������������������\n");
	if (mode==1) {
		fprintf(fp2,"    ��ȣ�ڵ� ��ȣȸ�̸�          ��ȣȸ�� ��ȣȸ���丮\n");
	}
	else if (mode==2) {
		fprintf(fp2,"    ��ȣ�ڵ� ��ȣȸ�� ��ȸ�� �Ѹ޴� (�޴�/�ڷ��/ũ��) ����/����/����\n");
	}
	else {
		fprintf(fp2,"    ��ȣ�ڵ� ��ȣȸ�̸�          ��ȣȸ�� ��ȣȸ���丮\n");
		fprintf(fp2," ��ȸ�� �Ѹ޴� (�޴�/�ڷ��/ũ��) ����/����/����\n");
	}
	fprintf(fp2,"������������������������������������������������������������������������������\n");
	for (i=0;i<1000;i++) strcpy(a_code[i],"");
	while(fread(&club_go,sizeof_go,1,fp1)) {
		if (strlen(club_go.code)<1) continue;
		if (club_go.type=='s') {
			sprintf(buff,"%s%s",club_go.code,club_go.dir);
			found=No;
			for (i=0;i<club_count;i++) {
				if (is_same(a_code[i],buff)) {
					found=Yes;
					break;
				}
			}
			if (found) continue;
			strcpy(a_code[club_count++],buff);
			anum[3]++;
		}
		if (club_go.type!=' ') {anum[1]++;anum[2]++;}
			 if (is_char(PDSTYPE,club_go.type)) anum[4]++;
		else if (is_char(BBBTYPE,club_go.type)) anum[5]++;
		else if (club_go.type=='n') anum[7]++;
		else if (is_char(EXETYPE,club_go.type)) anum[6]++;

		if (club_go.type!='s') continue;
		read_ccfg();
		if (mode==1) {
			fprintf(fp2,"%-3d %-8.8s %-20.20s %-8s %-30.30s\n",
			count+1,club_go.code,club_go.name,ccfg.sysop,club_go.dir);
		}
		else if (mode==2) {
			sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
			sprintf(NOW_MENU ,"%s/%s",club_go.dir,MENU_FILE);
			fprintf(fp2,"%-3d %-8.8s %-8s %6d %6d (%4d %4d %6d) %2d %2d %2d\n",
			count+1,club_go.code,ccfg.sysop,
			count_cpf(id_file),count_menu(NOW_MENU),
			ccfg.maxmenu,ccfg.maxpdsmenu,ccfg.maxpdssize,
			ccfg.autoguest,ccfg.modeopen,ccfg.intype);
		}
		else {
			sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
			sprintf(NOW_MENU ,"%s/%s",club_go.dir,MENU_FILE);
			fprintf(fp2,"%-3d %-8.8s %-30.30s %-8s %-30.30s ",
			count+1,club_go.code,club_go.name,ccfg.sysop,club_go.dir);
			fprintf(fp2,"%6d %6d (%4d %4d %6d) %2d %2d %2d\n",
			count_cpf(id_file),count_menu(NOW_MENU),
			ccfg.maxmenu,ccfg.maxpdsmenu,ccfg.maxpdssize,
			ccfg.autoguest,ccfg.modeopen,ccfg.intype);
		}
		count++;
	}
	fprintf(fp2,"������������������������������������������������������������������������������\n");
	fprintf(fp2,"  �Ѹ޴�:%d  ���θ޴�:%d ��ȣȸ:%d �ڷ��:%d �Խ���:%d �ܺθ��:%d �ڳ�:%d\n",
			anum[1],anum[2],anum[3],anum[4],anum[5],anum[6],anum[7]);
	fclose(fp1);
	fclose(fp2);
	view_text(TMP_FILE);
	unlink(TMP_FILE);
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
job_info()
{
	FILE *fp;
	FCFG fcfg;
	int i,n,num,n2;
	char buff[100];
	char filename[200];

	while (1) {
		cls();
		printf("  ###  ������ ���� �۾� ###\n");
		printf("  1. �������� : �⺻ ���� ����     :%s\n",rbit(cfg.infomode,0,"�Ϲ�","����������"));
		printf("  2. �������� : ���ȸ�� ���Ⱑ��  :%s\n",rbit(cfg.infomode,1,"�Ұ���","����"));
		printf("  3. �������� : ���ȸ�� ��ϰ���  :%s\n",rbit(cfg.infomode,2,"�Ұ���","����"));
		printf("  4. �������� : ���ȸ�� �ٿ��  :%s\n",rbit(cfg.infomode,3,"�Ұ���","����"));
		printf("  5. �������� : ��üȸ�� �մ�����  :%s\n",rbit(cfg.infomode,4,"ID�Է�","ID���"));
		printf("  6. �������� : ���Էΰ� �Ⱥ���    :%s\n",rbit(cfg.infomode,5,"����","���"));
		printf("  7. �������� : ���̵� �Է¹���    :%s\n",rbit(cfg.infomode,6,"����","���"));
		printf("  8. �������� : �������� ������    :%s\n",rbit(cfg.infomode,7,"����","���"));
		printf("  9. �������� : �����   ������    :%s\n",rbit(cfg.infomode,8,"����","���"));
		printf(" 10. �������� : ���մ�->���2ȸ��:%s\n",rbit(cfg.infomode,9,"����","����"));
		n=numselect();
		xy(50,n+1);
		if (n<= 0)	break;
		if (n== 1)	cfg.infomode=gtt_bit(cfg.infomode,0);
		if (n== 2)	cfg.infomode=gtt_bit(cfg.infomode,1);
		if (n== 3)	cfg.infomode=gtt_bit(cfg.infomode,2);
		if (n== 4)	cfg.infomode=gtt_bit(cfg.infomode,3);
		if (n== 5)	cfg.infomode=gtt_bit(cfg.infomode,4);
		if (n== 6)	cfg.infomode=gtt_bit(cfg.infomode,5);
		if (n== 7)	cfg.infomode=gtt_bit(cfg.infomode,6);
		if (n== 8)	cfg.infomode=gtt_bit(cfg.infomode,7);
		if (n== 9)	cfg.infomode=gtt_bit(cfg.infomode,8);
		if (n==10)	cfg.infomode=gtt_bit(cfg.infomode,9);
	}
}


set_roomtype_limit()	 //�з��� ���Ѽ���
{
	FILE *fp;
	CROOM tcroom;
	int found=No,n;
	char filename[100],buff[100];
	strcpy(filename,"system/chattype.dat");
	while (1) {
		printf("\n ## ���� ��ȭ�� Ÿ�� (1~39) >>");
		lineinput2(buff,3);
		n=atoi(buff);
		if (is_quit(buff)) return;
		if (n==0) continue;
		if (n<1||n>39) return;
		tcroom.age1=tcroom.age1=tcroom.itype=tcroom.sex=0;
		if ((fp=fopen(filename,"r"))!=NULL) {
			fseek(fp,n*sizeof_croom,SEEK_SET);
			fread(&tcroom,sizeof_croom,1,fp);
			fclose(fp);
		}
		disp_room_limit(n,tcroom);
		printf("\n  ��ȭ�� Ÿ�Թ�ȣ: %d",n);
		printf("\n  ���� ���� ���� : %2d >>",tcroom.age1 );tcroom.age1 =get_num(tcroom.age1 ,2);
		printf("\n  ���� ����  ��  : %2d >>",tcroom.age2 );tcroom.age2 =get_num(tcroom.age2 ,2);
		printf("\n  ���� ���      : %2d >>",tcroom.itype);tcroom.itype=get_num(tcroom.itype,2);
		printf("\n  ���� 1:�� 2:�� : %2d >>",tcroom.sex  );tcroom.sex  =get_num(tcroom.sex  ,1);
		printf("\n");
		disp_room_limit(n,tcroom);
		if ((fp=fopen(filename,"r+"))==NULL)
			  fp=fopen(filename,"w+");
		fseek(fp,n*sizeof_croom,SEEK_SET);
		fwrite(&tcroom,sizeof_croom,1,fp);
		fclose(fp);
	}
}


disp_room_limit(int n,CROOM tcroom)
{
	printf(" ## ��ȭ�� Ÿ�Թ�ȣ: %d \n",n);
	printf(" ## ���� ���� ���� : %2d - %2d \n",tcroom.age1,tcroom.age2);
	printf(" ## ���� ���      : %2d \n",tcroom.itype);
	printf(" ## ���� 1:�� 2:�� : %2d \n",tcroom.sex  );
}
