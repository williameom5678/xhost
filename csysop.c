////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ��ȣȸ �û� ���� �޴�
////////////////////////////////////////////////////////////////////////

#include "bbs.h"

int nn;
int top;
int last;
int key;
int n;

char id_file[300];
char menu_file[300];
char filename[300];
char now_code[100];
char now_id[60];
int nwait0,nwait1,ntotal1,ntotal2,ntotal3;


main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	PF pf2;
	int n=0,i;
	int key=No;
	char buff[140];
	char tcode[60];

	printf("\n<%s>\n",VER1);msleep(3);
    if (argc<4) {
        printf("\n����  csysop <��ȣȸ �ڵ�> <�����ڵ�> <�۾��� ���̵�>\n");
		getchar();
		return;
	}
	check_initdir();
	strcpy(tcode,argv[1]);				//��ȣȸ �ڵ�
    if (is_same(tcode,"main")) {mess("��ȣȸ ������");return;}
    strcpy(now_id,argv[3]);              //�۾��� ���̵�
	findport(tty); //������Ʈ ã��
	read_cfg();
	set_tmpfile();
    if (!read_pf3(now_id)) {mess("%s ID�˻�����",now_id);return;}
	if (!find_club(tcode)) {
		mess("%s��ȣȸ �˻�����",tcode);
		return;
	}
    find_go(argv[2]);

	check_dir("%s/system",club_go.dir);
	club_mode=Yes;
	read_ccfg();
	read_cpf(now_id);					//�ش�Ǵ� ��ȣȸ�� ���̵� ã�´�.
    sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
	read_atype();						//����б�
	read_actype();						//��ȣȸ����� �̸��� �о��  ->actype
	if (pf.type==16)				 cpf.type=17;
	if (pf.type==17)				 cpf.type=18;
	if (pf.type>=18)				 cpf.type=19;
	if (is_same(ccfg.sysop,pf.id))	 cpf.type=19;
	if (cpf.type<16) {
		printf("\n ��ȣȸ�޴��� ����Ҽ� ���� ����Դϴ�.\n");
		getchar();
		return;
	}

	key=No;
	who=0;		   //1:��ȣȸ ��� 2:��ȣȸ �û�	3:root����
	if (cpf.type>=17)				  who=1;
	if (pf.type==16)				  who=1;
	if (pf.type==17)				  who=2;
	if (is_same(ccfg.sysop,now_id))   who=2;
	if (cpf.type==18)				  who=2;
	if (cpf.type==19)				  who=2;
	if (pf.type==18)				  who=3;
	if (pf.type==19)				  who=4;
	if (who==0) {
		printf("\n ��ȣȸ �û�޴��� ����Ҽ� ���� ����Դϴ�.\n");
		getchar();
		return;
	}
	if (pf.type<17&&who<3&&ccfg.passsys[0]!=0) {
		printf("\n %s �û�޴��� ��ȣ�� �����Ǿ� �ֽ��ϴ�. ��ȣ��? >>",club_go.code);
		passinput(buff,8);
		printf("\n");
		if (!is_same(ccfg.passsys,buff)) {
			printf("��ȣ�� Ʋ���ϴ�. ��ȣ�� Ȯ���Ͻʽÿ�\n");
			getchar();
			return;
		}
	}

	if (bit(cfg.backmode,5)==0) sys_back(club_go.dir) ; //�ý��� ���

	key=Yes;
	while (1) {
		cls();
		xy(60, 7);printf("���Ը�� :%s\n",(ccfg.autoguest==1)?"�ڵ�":"����");
		if (ccfg.stop!=0) {
		xy(60,13);printf("�ӽ���� :%d",ccfg.stop);
		}
		xy(1,1);
		printfL2();
		printf("          ��ȣȸ ����       �۾���:%s ���:%d\n",now_id,who);
		printfL2();
        printf("s 1. ��ȣȸ �ڵ�                : %s\n",club_go.code);
        printf("s 2. ��ȣȸ �̸�                : %s\n",club_go.name);
        printf("s 3. ��ȣȸ ���丮            : %s\n",club_go.dir);
        printf("s 4. ��ȣȸ �û�                : %-8s\n",ccfg.sysop);
        printf("s 5. �ִ�޴���     (0:������)  : %d\n",ccfg.maxmenu);
        printf("s 6. �ִ��ڷ�Ǽ�   (0:������)  : %d\n",ccfg.maxpdsmenu);
        printf("s 7. �ִ��ڷ��ũ�� (0:������)  : %d Mbyte\n",ccfg.maxpdssize);
        printf("s 8. ��ȭ�� ������ȣ  (0:����)  : %d (3000~4999)\n",ccfg.chatnum);
        printf("\n");
        printf(" 11. ��ȣȸ ȯ��/���� ����\n");
        printf(" 12. ��ȣȸ ȸ��/���� ����\n");
        printf(" 13. ��ȣȸ ȭ��/�޴� ����\n");
        printf(" 14. ��ȣȸ ���� ���丮 ��ü �˻�\n");
        printf(" 15. �۾� ȸ�� ����     \n");
        printf(" 16. ���̹��Ӵ� ���� �۾�\n");
        printf(" 17. ��Ȳ ���  ���� �۾�\n");
		printfL2();
		printf("���ù�ȣ? �޴�����(q) >>");
		lineinput(buff,10);
		check_han(buff);
		n=atoi(buff);
		printf("\n");
		if (is_char("xXqpQq0",buff[0])) break;
		xy(30,n+3);
        if(who>=3&&n==1) {
			lineinput(buff,8);del_space(buff);strlwr(buff);
			if (buff[0]!=0) strcpy(club_go.code,buff);
		}
        if(who>=3&&n==2) {
			lineinput(buff,30);
			if (buff[0]!=0) strcpy(club_go.name,buff);
		}
        if(who>=3&&n==3) {
			lineinput(buff,60);del_space(buff);strlwr(buff);
			if (buff[0]!=0) strcpy(club_go.dir,buff);
		}
        if(who>=3&&n==4) get_str(ccfg.sysop,8);
        if(who>=3&&n==5) ccfg.maxmenu   =get_num(ccfg.maxmenu   ,5);
        if(who>=3&&n==6) ccfg.maxpdsmenu=get_num(ccfg.maxpdsmenu,5);
        if(who>=3&&n==7) ccfg.maxpdssize=get_num(ccfg.maxpdssize,5);
        if(who>=3&&n==8) ccfg.chatnum   =get_num(ccfg.chatnum   ,5);
		if(n==11) job_cfg();
		if(n==12) job_cid();
		if(n==13) job_menu();
		if(n==14) { 		//���� ���丮 Ȯ��
			sys_tem("ls -R -L -G -l %s > %s",club_go.dir,TMP_FILE);
			view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
		if(n==15) job_idset();
		if(n==16) job_money();
        if(n==17) job_stat();
	}
    if (ccfg.chatnum<3000||ccfg.chatnum>5000) ccfg.chatnum=3000;

	//�ش� ��ȣȸ�� ȯ���� �����Ѵ�.
	sprintf(buff,"%s/system/clubcfg.dat",club_go.dir);
	if ((fp=fopen(buff,"w"))!=NULL) {
		fwrite(&ccfg,sizeof_ccfg,1,fp) ;
		fclose(fp);
	}
	if (who>=3) {
		if ((fp=fopen(MENU_FILE,"r+"))!=NULL) {
			n=0;
			while (1) {
				fseek(fp,n*sizeof_go,SEEK_SET);
				if (!fread(&go,sizeof_go,1,fp)) break;
				if (go.type=='s'&&is_same(go.code,club_go.code)) {
					c_lock(1,MENU_FILE);
					go=club_go;
					fseek(fp,n*sizeof_go,SEEK_SET);
					fwrite(&go,sizeof_go,1,fp);
					c_lock(0,MENU_FILE);
				}
				n++;
			}
			fclose(fp);
		}
	}
}

//ȸ�� ���� �۾�
job_stat()
{
	int i,num;
	char buff[100];
	char type_file[140];
	FILE *fp;
	while (1) {
		cls();
        printf("          ȸ�� ��� �۾�\n");
        check_total();
		xy(60, 8);printf("�� ȸ �� :%6d��\n",ntotal1);
		xy(60, 9);printf("��� 0   :%6d��\n",nwait0);
		xy(60,10);printf("���Դ�� :%6d��\n",nwait1);
		xy(60,11);printf("�� �� �� :%6d��\n",ntotal2);
		xy(60,12);printf("7�ϰ�����:%6d��\n",ntotal3);
        xy(1,20);
		printf(" ���� : >>");
		num=numinput();
		printf("\n");
		xy(60,num+1);
		if (num== 0) break;
    }
}





//ȸ�� ���� �۾�
job_cid()
{
	int i,num;
	char buff[100];
	char type_file[140];
	FILE *fp;
    check_total();
	while (1) {
		cls();
		printf("          ȸ�� ���� �۾�\n");
		printf(" 1. ��ȣȸ ȸ�� �Ż� ��ȸ                         :\n");
		printf(" 2. ��ȣȸ ȸ�� ��� ����                         :\n");
        printf(" 3. ��ȣȸ ȸ�� ����(����) ����                   :\n");
		printf(" 4. ��ȣȸ ȸ�� �����㰡  (���1->���2��)    ���:%d ��:\n",nwait1);
		printf(" 5. ��ȣȸ ȸ�� �ڵ� ���� (0:���� 1:�ڵ� 2:����)  :%d\n",ccfg.autoguest);
		printf(" 6. ��ȣȸ ���� ���� ���� ��� (0:�մ� 1-19ȸ��)  :%d\n",ccfg.intype);
		printf(" 7. ��ȣȸ ���� ���� ��ȣ ��� (1:�մ� 2-19ȸ��)  :%d\n",ccfg.cintype);
		printf(" 8. ��ȣȸ ��� <- ���ε�޿���                   :%s\n",rbit(ccfg.mode,1,"����","����"));
		printf(" 9. ��ȣȸ ���� ȸ�� ��û���� ��� (0-19)         :%d\n",ccfg.gtype);
		printf("10. ��ȣȸ ���� ȸ�� ��û���� ���� (����-��)      :%2d-%2d\n",ccfg.gage1,ccfg.gage2);
		printf("11. ��ȣȸ ���� �׸� ����,�׸�� ����             \n");
		printf("12. ��ȣȸ ��޺� �̸� ����                       \n");
		printf("13. ��ȣȸ ���� �Ұ��� ���̵� ����   (system/notguest.txt)\n");
		printf("14. ���� ���� ���̵� ���� ���� ȭ��  (logo/_whynot6.log)\n");
        printf("15. ��¥�� ��ȣȸ ������Ȳ                              \n");
        printf("16. ��ȣȸ ȸ�� ���                                    \n");
        printf("17. ��ȣȸ ������ ���ϵ��  (�ڵ� ���� ó��)            \n");
        printf("18. ��ȣȸ ���� ���� ���� (����-��)               :%02d-%02d\n",ccfg.age/100,ccfg.age%100);
		printfL2();
		printf(" ���� : >>");
		num=numinput();
		printf("\n");
		xy(60,num+1);
		if (num== 0) break;
		if (num== 1) edit_cpf(who);
		if (num== 2) set_cpf_type();  //�������
		if (num== 3) free_cwait(0);   //��������
		if (num== 4) free_cwait(1);
		if (num== 5) ccfg.autoguest=get_num(ccfg.autoguest,1);
		if (num== 6) ccfg.intype=get_num(ccfg.intype,2);
		if (num== 7) ccfg.cintype=get_num(ccfg.cintype,2);
		if (num== 8) ccfg.mode=gtt_bit(ccfg.mode,1);
		if (num== 9) ccfg.gtype=get_num(ccfg.gtype,2);
		if (num==10) {
			xy(60,num+3);
			ccfg.gage1=get_num(ccfg.gage1,2);
			xy(64,num+3);
			ccfg.gage2=get_num(ccfg.gage2,2);
		}
		if (num==11) cguest_item();
		if (num==12) {
			disp_ctype();
			for (i=0;i<20;i++) {
				xy(9,i+5);lineinput(buff,8);if (!is_same(buff,"q")&&buff[0]!=0) strcpy(actype[i],buff);
				disp_ctype2(i);
				if (is_same(buff,"q")) break;
				del_space(actype[i]);
			}
			disp_ctype(i);
			if (yesno("\n ���� ��ȣȸ ��޸��� �����ұ��? (Y/n) >>",Yes)) {
				printf("\n");
				sprintf(type_file,"%s/system/typename.dat",club_go.dir);
				fp=fopen(type_file,"w");
				for (i=0;i<20;i++) fwrite(&actype[i],sizeof(actype[i]),1,fp);
				if (fp!=NULL) fclose(fp);
			}
		}
		if (num==13) {
			sprintf(buff,"%s/system/notguest.txt",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (num==14) {
			sprintf(buff,"%s/logo/_whynot6.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
        if (num==15) view_daycpf();
        if (num==16) view_stat();
        if (num==17) free_auto_file();
        if (num==18) ccfg.age=numinput();
    }
}


free_auto_file()
{
    FILE *fp1,*fp2;
    char buff[100],buff1[100],buff2[100],buff3[100];
    int i,n,count;
    count=0;
    if (!input_tmp(0)) return;
    if ((fp1=fopen(TMP_FILE,"r"))==NULL) return;
    while(fgets(buff,200,fp1)!=NULL) {
        strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");
        sscanf(buff,"%s %s %s",buff1,buff2,buff3);
        if (strlen(buff1)<3) continue;
        i=atoi(buff2);
        if (i<3) i=3;
        printf("%5d %-8s %4d  -> ",count+1,pf2.id,i);
        if (!read_pf4(buff1)) {printf("���ID �ƴ�\n");continue;}
        if (read_cpf(pf2.id)) {printf("��ȣȸ �̵̹�ϵ�\n");continue;}
        cpf.type=i;
        strcpy(cpf.id   ,pf2.id);
        strcpy(cpf.name ,pf2.name);
        strcpy(cpf.memo1,"");
        strcpy(cpf.memo2,"");
        strcpy(cpf.memo3,"");
        strcpy(cpf.memo4,"");
        strcpy(cpf.memo5,"");
        time(&cpf.inday);
        time(&cpf.login);
        time(&cpf.logout);
        cpf.countu=0;
        cpf.countw=0;
        cpf.countd=0;
        cpf.all_log=0;
        cpf.all_time=0;
        fp2=fopen(id_file,"a+");
        if (fp2==NULL) fp2=fopen(id_file,"w+");
        if (fp2!=NULL) {
            cpf.num=ftell(fp2)/sizeof_cpf+1;
            fwrite(&cpf,sizeof_cpf,1,fp2);
            fclose(fp2);
            printf(" %d ��� ����\n",cpf.type);
        }
    }
    fclose(fp1);
}


//��ȣȸ ��Ȳ
view_stat() {
    FILE *fpid;
    FILE *fpsave;
    char id_file[300];
    int  n,count,i,i2,i3,total;
    struct tm *tp;
	// arr1:���� arr2:����Ƚ�� arr3:���ӽð�


    sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
    total=count_cpf(id_file);                  //ȸ�� �� �� ���
    fpid = fopen(id_file, "r+");
    fpsave = fopen(TMP_FILE, "w+");

    fprintf(fpsave,"            *** %s ��ȣȸ  ��ȸ��:%5d��  ***\n",club_go.code,total);

    fseek(fpid,0,SEEK_SET);
    for (n=0;n<20;n++) {
        count=0;
        fprintf(fpsave,"   ========== %d   ���� %4d  ========== \n",club_go.code,n);
        fseek(fpid,0,SEEK_SET);
        for (i=0;i<=total;i++) {
            fread(&cpf,sizeof_cpf,1,fpid);         //���̵� ����
            if (cpf.type==n) {
                tp = localtime(&cpf.login);
                fprintf(fpsave," %3d.  %-8s %-8s %5d %5d %5d ",
                count+1,cpf.id, cpf.name, cpf.countw,cpf.countu, cpf.countd);
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
cguest_item()
{
	int num;
	char buff[100];
	int i;

	while (1) {
		cls();
		printf(" ȸ������ �׸�����/����\n");
		printf(" 1. ��ȣȸ ��û�� �������1 �׸��Է�  %-8s    :%s\n",ccfg.note[0],rbit(ccfg.memo_input,1,"���","���"));
		printf(" 2. ��ȣȸ ��û�� �������2 �׸��Է�  %-8s    :%s\n",ccfg.note[1],rbit(ccfg.memo_input,2,"���","���"));
		printf(" 3. ��ȣȸ ��û�� �������3 �׸��Է�  %-8s    :%s\n",ccfg.note[2],rbit(ccfg.memo_input,3,"���","���"));
		printf(" 4. ��ȣȸ ��û�� �������4 �׸��Է�  %-8s    :%s\n",ccfg.note[3],rbit(ccfg.memo_input,4,"���","���"));
		printf(" 5. ��ȣȸ ��û�� �������5 �׸��Է�  %-8s    :%s\n",ccfg.note[4],rbit(ccfg.memo_input,5,"���","���"));
		printf(" 6. ��ȣȸ �Ż���ȸ�� ����1 �׸����  %-8s    :%s\n",ccfg.note[0],rbit(ccfg.memo_disp,1,"���","���"));
		printf(" 7. ��ȣȸ �Ż���ȸ�� ����2 �׸����  %-8s    :%s\n",ccfg.note[1],rbit(ccfg.memo_disp,2,"���","���"));
		printf(" 8. ��ȣȸ �Ż���ȸ�� ����3 �׸����  %-8s    :%s\n",ccfg.note[2],rbit(ccfg.memo_disp,3,"���","���"));
		printf(" 9. ��ȣȸ �Ż���ȸ�� ����4 �׸����  %-8s    :%s\n",ccfg.note[3],rbit(ccfg.memo_disp,4,"���","���"));
		printf("10. ��ȣȸ �Ż���ȸ�� ����5 �׸����  %-8s    :%s\n",ccfg.note[4],rbit(ccfg.memo_disp,5,"���","���"));
		printf("11. ��ȣȸ ������� �̸� ����                     :\n");
		printfL2();
		printf(" ���� : >>");
		num=numinput();
		printf("\n");
		xy(60,num+1);
		if (num== 0) break;
		if (num== 1) ccfg.memo_input=gtt_bit(ccfg.memo_input,1);
		if (num== 2) ccfg.memo_input=gtt_bit(ccfg.memo_input,2);
		if (num== 3) ccfg.memo_input=gtt_bit(ccfg.memo_input,3);
		if (num== 4) ccfg.memo_input=gtt_bit(ccfg.memo_input,4);
		if (num== 5) ccfg.memo_input=gtt_bit(ccfg.memo_input,5);
		if (num== 6) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,1);
		if (num== 7) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,2);
		if (num== 8) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,3);
		if (num== 9) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,4);
		if (num==10) ccfg.memo_disp =gtt_bit(ccfg.memo_disp,5);
		if (num==11) {
			cls();
			printfL2();
			printf("         ��ȣȸ�� ȸ�� �Ż��� ���� ���� �׸� �̸� ����\n");
			printfL2();
            for(i=0;i<5;i++) {
                printf("�����׸�%2d: ����:%-8s  �ű�:>>",i+1,ccfg.note[i]);
                lineinput(buff,8);printf("\n");
				if (buff[0]!=0)
					strcpy(ccfg.note[i],buff);
			}
		}
	}
}



//ȯ�漳��
job_cfg()
{
	int num;
	char buff[100];
	while (1) {
			cls();
		printfL2();
		printf("          ��ȣȸ ȯ�� ���� �޴�\n");
		printfL2();
		printf(" 1. ��ȣȸ �ڷ�� ���丮 ���� (0:�ڵ�   1:����)              :%d\n",ccfg.dir   );
		printf(" 2. ��ȣȸ �û� �޴� ��ȣ����                                  :%s\n",ccfg.passsys  );
		printf(" 3. ��ȣȸ ���� ��ȣ����                                       :%s\n",ccfg.enterp);
		printf(" 4. ��ȣȸ �մ� ���� ���                                      :%d\n",ccfg.modeopen);
		printf(" 6. ��ȣȸ �ӽ� ���  (0:���� 1:�ӽ���� 8:������� 9:�û����):%d\n",ccfg.stop  );
		printf(" 7. ��ü �ڷ�� �������                                       :%s\n",ccfg.pds_man);
		printf(" 8. ��ü �Խ��� �������                                       :%s\n",ccfg.bbs_man);
		printf("11. ���Ա��� �޼��� (ȸ�����:0)                      (_whynot1.log)\n");
		printf("12. ���Ա��� �޼��� (����� ��ȣȸ)                   (_whynot2.log)\n");
		printf("13. ���Ա��� �޼��� (��ȣȸ �ӽ� ���)                (_whynot7.log)\n");
		printf("14. ���Ա��� �޼��� (��ȣȸ ���� ���:�û�)           (_whynot9.log)\n");
		printfL2();
		printf(" ���� : >>");
		num=numinput();
		printf("\n");
		xy(70,num+3);
		if (num==0) break;
		if (who>3&&num==1) ccfg.dir=get_num(ccfg.dir,1);
		if (num==2) get_str(ccfg.passsys,8);
		if (num==3) get_str(ccfg.enterp,4);
		if (num==4) ccfg.modeopen=get_num(ccfg.modeopen,1);
		if (num==6) ccfg.stop=get_num(ccfg.stop,1);
		if (num==7) get_str(ccfg.pds_man,8);
		if (num==8) get_str(ccfg.bbs_man,8);
		if (num==11) {
			sprintf(buff,"%s/logo/_whynot1.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (num==12) {
			sprintf(buff,"%s/logo/_whynot2.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (num==13) {
            sprintf(buff,"%s/logo/_whynot7.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
		if (num==14) {
			sprintf(buff,"%s/logo/_whynot9.log",club_go.dir);
			xedit(buff);
			if (file_size(buff)<10) unlink(buff);
		}
	}
	del_space(ccfg.passsys);
	del_space(ccfg.enterp);
}


//�޴�,�Խ���
job_menu()
{
	int num;
	char buff[100];
	while (1) {
		cls();
		printfL2();
		printf("          �޴�,�Խ���,ȭ�� ����\n");
		printfL2();
		printf(" 1. ��ȣȸ ��ü �޴� Ȯ��                         \n");
		printf(" 2. ��ȣȸ �޴� ����                              \n");
		printf(" 3. ��ȣȸ �޴� ��ũ (���θ޴��Ϳ���)             \n");
        printf(" 4. ��ȣȸ ��ü�ڷ�� ���� ����                   \n");
        printf(" 5. ��ȣȸ �ϴ� ���� ����                         \n");
        printf(" 6. ��ȣȸ ���� ���� ���� ����                    \n");
        printf(" 7. ���԰���/���� �޴� ����  (system/_nocode.dat) \n" );
        printf(" 8. ���԰���/���� ȭ�� ����                       \n" );
        printf(" 9. ��ü���� �߼� ���� ���     :%d\n",ccfg.wmalltype );
		printf("11. ���θ޴�   ���� �Ӻ�                  :%s\n",rbit(ccfg.mode,11,"���","�ź�"));
		printf("12. �����޴�   ���� �Ӻ�                  :%s\n",rbit(ccfg.mode,12,"���","�ź�"));
		printf("13. �Խ���     ���� �Ӻ�                  :%s\n",rbit(ccfg.mode,13,"���","�ź�"));
        printf("14. ���Խ� ����/����ȭ��                  :%s\n",rbit(ccfg.logomode,1,"����","�Ⱥ���"));
        printf("15. TOP �ִϸ��̼� �ΰ� ȭ��              :%s\n",rbit(ccfg.logomode,2,"����","������"));
        printf("16. ���Խ� ���/����ǥ��                  :%s\n",rbit(ccfg.logomode,3,"����","�Ⱥ���"));
        printf("21. ��ȣȸ ȭ�� ����                              \n");
        printf("22. ��ȣȸ ���� ����                              \n");
		printf(" ���� : >>");
		num=numinput();
		printf("\n");
		xy(60,num+3);
		if (num== 0) break;
        else if (num== 1) findmenu(0);
		else if (num== 2) sys_tem("bin/ed_menu %s %s",now_id,club_go.code);
		else if (num== 3) link_menu();
        else if (num== 4) xedit("%s/system/zbtotal.dat",club_go.dir);
        else if (num== 5) xedit("%s/logo/_ad.dat",club_go.dir);
        else if (num== 6) job_notice();
        else if (num== 7) set_no_code();
        else if (num== 8) set_no_logo();
        else if (num== 9) ccfg.wmalltype =get_num(ccfg.wmalltype , 2);
		else if (num==11) ccfg.mode=gtt_bit(ccfg.mode,11);
		else if (num==12) ccfg.mode=gtt_bit(ccfg.mode,12);
		else if (num==13) ccfg.mode=gtt_bit(ccfg.mode,13);
        else if (num==14) ccfg.logomode=gtt_bit(ccfg.logomode,1);
        else if (num==15) ccfg.logomode=gtt_bit(ccfg.logomode,2);
        else if (num==16) ccfg.logomode=gtt_bit(ccfg.logomode,3);
        else if (num==21) sys_tem("bin/xfrm 1 '%s' '%s' '%s'",club_go.code,go.code,pf.id);
        else if (num==22) sys_tem("bin/xfrm 2 '%s' '%s' '%s'",club_go.code,go.code,pf.id);
	}
}


//��ȣȸ ȸ�� ��� ����
set_cpf_type()
{
	FILE *fp1;
	int i;
	char buffid[255];
	char buff[255];
	while (1) {
		xy(1,20);printf(" ��������� ȸ���� ���̵� �Ǵ� �̸��� �Է��ϼ��� >>");
		lineinput(buffid, 8);
		if (buffid[0]==0) break;
		fp1=fopen(id_file,"r+");   //��ȣȸ ���̵�ȭ��
		if (fp1==NULL) break;
		i=0;
		while (1) {
			fseek(fp1,i*sizeof_cpf,SEEK_SET);
			if (!fread(&cpf,sizeof_cpf,1,fp1)) break;
			if (is_same(cpf.id,buffid)||is_same(cpf.name,buffid)) {
				cls();
                disp_cpf(cpf,who);
				xy(1,20);
				if (yesno("�� ��ȣȸ ȸ���� ����� �����ұ��? (Y/n) >>",Yes)) {
					xy(62, 3);
					lineinput(buff, 2);
					if (buff[0]!=0) cpf.type=atoi(buff);
					fseek(fp1,i*sizeof_cpf,SEEK_SET);
					fwrite(&cpf,sizeof_cpf,1,fp1);
					break;
				}
			}
			i++;
		}
		if (fp1!=NULL) fclose(fp1);
	}
}


//�̰��� ȸ���� ���Խ�Ų��.
free_cwait(int mode)
{
	char buff[200];
	FILE *fp;

	if (mode==0) {
		xy(1,20);
		while (1) {
			printf("\n ���Խ�ųȸ�� ID >>");
			lineinput(buff,8);
			if (is_same(buff,"q")||is_same(buff,"p")||buff[0]==0) return;
            if (!read_pf4(buff)) {printf(" ->�ش���̵� �����ϴ�.");continue;}
			printf("  ->%s(%s) ���:%d\n",pf2.id,pf2.name,pf.level);
			if (read_cpf(buff)) {printf("  ->�̹� ���ԵǾ� �ִ� ���̵��Դϴ�.\n",pf2.id);continue;}
			strcpy(cpf.id	,pf2.id);strcpy(cpf.name ,pf2.name);
			strcpy(cpf.memo1,"");strcpy(cpf.memo2,"");strcpy(cpf.memo3,"");
			strcpy(cpf.memo4,"");strcpy(cpf.memo5,"");
			time(&cpf.inday);time(&cpf.login);time(&cpf.logout);
			cpf.countu=0;cpf.countw=0;cpf.countd=0;cpf.all_log=0;cpf.all_time=0;
			cpf.type=2; 		//ȸ�����	0:�ź�/Ż�� 1:�մ� 2:��� 3:ȸ��-
			printf(" ���Խ�ųȸ���� ��� (0-16)>>");
			cpf.type=get_num(cpf.type,2);
			if (cpf.type>16) cpf.type=16;
			if (cpf.type==0) {printf(" ->��ҵ�");continue;}

			fp=fopen(id_file,"a+");
			if (fp==NULL) fp=fopen(id_file,"w+");
			if (fp!=NULL) {
				cpf.num=ftell(fp)/sizeof_cpf+1;
				fwrite(&cpf,sizeof_cpf,1,fp);
				fclose(fp);
			}
			printf(" %d ������� ���� ��",cpf.type);
		}
	}
	else {
		fp=fopen(id_file,"r+");
		if (fp!=NULL) {
			while (fread(&cpf,sizeof_cpf,1,fp)>0) {
				if (cpf.type==1) {
					disp_cpf(cpf,who);
					printf("���� (Y:�����㰡 N:�ź� ENTER:���� q:�۾�����) ����>>");
					lineinput(buff,2);
					check_han(buff);
					buff[0]=tolower(buff[0]);
					if		(buff[0]=='y') {            //���̵�� ����
						cpf.type=3;
						save_cpf(cpf.id);					 //������
					}
					else if (buff[0]=='n') {
						cpf.type=0;
						save_cpf(cpf.id);					 //������
					}
					else if (buff[0]=='q') break;       //��
				}
			}
			fclose(fp);
		}
	}
	check_total();
}


//��ȣȸ ȸ�� ��ȸ,����
edit_cpf(int who)
{
	FILE *fp1;
	int i,n,linenum;
	char buff[255];
	char st[10];
	struct tm *tp;
	time_t today;
	int key;

	key=Yes;
	top=1;
	strcpy(st,"   ");
	top=1;
	check_total();
	if (ntotal1<1) {
		mess("���� ȸ���� �Ѹ� �����ϴ�.");
		return;
	}

	while (key==Yes) {
		clrscr();
		printf("ȸ�� ����     ��%4d��\n",ntotal1);
		printfL2();
		printf("��ȣ �� �� �� Ÿ�� �з� �������� �������� Ƚ�� ���� ��� �ٿ�  ���̿�\n");
		//		1234 12345678	0  ��� 99/99/99 99/99/99 9999 9999 9999 9999 9999:99
		printfL2();

		if (top<1) top=1;
		if (top>ntotal1) top=ntotal1;
		fp1=fopen(id_file,"r+");   //��ȣȸ ���̵�ȭ��
		for (i=top;i<top+15;i++) {
			fseek(fp1,(i-1)*sizeof_cpf,SEEK_SET);
			if (fread(&cpf,sizeof_cpf,1,fp1)>0) {
				if (cpf.type<0||cpf.type>19) cpf.type=0;
				printf("%4d %-8s %2d %8s ",
				cpf.num,cpf.id,cpf.type,atype[cpf.type]);
				tp=localtime(&cpf.inday);
				printf("%02d/%02d/%02d ",
                tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
				tp=localtime(&cpf.logout);
				printf("%02d/%02d/%02d ",
                tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
				printf("%4d %4d %4d %4d\n",
				cpf.all_log,cpf.countw,cpf.countu,cpf.countd);
			}
			else printf("\n");
		}
		printfL2();
		if (fp1!=NULL) fclose(fp1);

		printf("��� (��ȣ:Ȯ�� E:���� L:��� D:���� S:�˻� T:ó�� B:���� R:ȸ������) >>");
		lineinput(buff,5);
		printf("\n");
		check_cmd(buff);
		nn=mmn[1];
		if (mmn[0]>0&&mmn[0]<=ntotal1) {
			if (read_cpfn(mmn[0])) {
				if (disp_cpf(cpf,who)) {
					pressenter();
					top=mmn[0];
				}
			}
			continue;
		}
		switch (**mmc) {
		case 'e' :
					if (read_cpfn(nn)) { //�ش� ��ȣ ���̵� �о� cpf�� ����
						sedit_cpf();
						save_cpf(cpf.id);
					}
					break;
		case 'd' :
					if (read_cpfn(nn)) { //�ش� ��ȣ ���̵� �о� cpf�� ����
						cpf.type=0;
						save_cpf(cpf.id);
					}
					break;
		case 'r' :  ssort_cpf();    break;
		case 'l' :  stype_cpf(nn);  break;
		case 'p' :
		case 'x' :
		case 'q' :  key=No      ; break;
		case 's' :  ssearch_cpf(); break;
		case 't' :  top=1       ; break;
		case 'a' :
		case 'b' :  top-=15     ; break;
		default  :	top+=15 	; break;
		}
	}
	check_total();	//�Ѽ� �ľ�
}



stype_cpf(int nn)
{
	if (read_cpfn(nn)) { //�ش� ��ȣ ���̵� �о� cpf�� ����
		cls();
		disp_cpf(cpf,who);
		xy(62, 3);
		cpf.type=get_num(cpf.type,2);
		save_cpf(cpf.id);
	}
}


//cpf�� ������ �����Ѵ�. ��ȣ�� ������
sedit_cpf()
{
	FILE *fp;
	char buff[140];
	int n1,n2,n3,n;
	struct tm *tp;
    CPF oldcpf;

    oldcpf=cpf;
	n=1;
	cls();
    while (1) {
        if (!view("%s/frm/pf/cpfe.scr",club_go.dir)) view("frm/pf/cpfe.scr");
        xy(14, 3);lineinput(buff, 8);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.id,buff);
        xy(38, 3);lineinput(buff, 8);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.name,buff);
        xy(62, 3);lineinput(buff, 2);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.type=atoi(buff)    ;
        xy(64, 5);lineinput(buff, 6);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.all_log=atoi(buff);
        xy(56, 6);lineinput(buff, 6);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.countw=atoi(buff)    ;
        xy(64, 6);lineinput(buff, 6);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.countu=atoi(buff)    ;
        xy(72, 6);lineinput(buff, 6);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.countd=atoi(buff)    ;
        xy(38, 7);lineinput(buff, 9);if(is_same(buff,"q")) break;if (buff[0]!=0) cpf.all_time=atoi(buff)     ;
        xy(14,13);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo1,buff);
        xy(14,14);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo2,buff);
        xy(14,15);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo3,buff);
        xy(14,16);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo4,buff);
        xy(14,17);lineinput(buff,64);if(is_same(buff,"q")) break;if (buff[0]!=0) strcpy(cpf.memo5,buff);
        del_esc(cpf.memo1)  ;
        del_esc(cpf.memo2)  ;
        del_esc(cpf.memo3)  ;
        del_esc(cpf.memo4)  ;
        del_esc(cpf.memo5)  ;
        break;
    }
    if (is_same(buff,"q")) cpf=oldcpf;
}



//�˻�
ssearch_cpf()
{
	FILE *fp;
	char buff[140],buff2[80];
	int mode;
	int key,found;
	CPF cpf2;

	printf("1.ID 2.���� 3.ȸ������ 0.��� >>");
	lineinput(buff,2);
	mode=atoi(buff);
	if (mode==0) return;
	printf("�˻��� ���ڸ� �Է��ϼ���. >>");
	lineinput(buff,20);
	printf("\n");
	if (buff[0]==0) return;

	fp=fopen(id_file,"r+");
	key=Yes;
	found=No;
	while (key==Yes&&fread(&cpf2,sizeof_cpf,1,fp)>0) {
		found=No;
		switch (mode) {
		case (1) : found=stristr(cpf2.id,buff)		  ;break;
		case (2) : found=stristr(cpf2.name,buff)	  ;break;
		case (3) : if (cpf2.type==buff[0]-'0') found=Yes  ;break;
		default  : break;
		}
		if (found) {
			disp_cpf(cpf2,who);
			if (yesno("�� ȸ���� �½��ϱ�? (Y:�ش��ڷ�� �̵� N:��Ӱ˻�)>>",Yes)) {
				found=Yes;
				key=No;
			}
		}
	}
	if (fp!=NULL) fclose(fp);
	if (found) top=cpf2.num;
}


//ȸ�� ����  (type=0�� ȸ���� ������);
ssort_cpf()
{
	char buff[140];
	char id_file2[140];
	FILE *fp1,*fp2;
	int n;

	if (!yesno("\n\nȸ������� 0�� ȸ��(�Ұ�)�� �����ұ��? (y/N)",No)) return;

	printf("\n");
	strcpy(id_file2,id_file);
	strcat(id_file2,".new");

	fp1=fopen(id_file,"r+");
	fp2=fopen(id_file2,"w");
	n=1;
	while (fread(&cpf,sizeof_cpf,1,fp1)>0)
		if (cpf.type>0) {
			cpf.num=n;
			fwrite(&cpf,sizeof_cpf,1,fp2);
			n++;
		}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (is_file(id_file2)) {
		unlink(id_file);
		rename(id_file2,id_file);
	}
}


//��ȸ�� �� �ľ�
check_total()
{
	FILE *fp;
	CPF cpf2;
	nwait1=0;
	ntotal1=0;
	ntotal2=0;
	ntotal3=0;
	fp=fopen(id_file,"r+");
	if (fp!=NULL) {
		fseekend(fp);
		ntotal1=ftell(fp)/sizeof_cpf;
		rewind(fp);
		while (fread(&cpf2,sizeof_cpf,1,fp)) {
			if		(cpf2.type==0) nwait0++;
			else if (cpf2.type==1) nwait1++;
			else if (cpf2.type>13) ntotal2++;
			if (time_day(cpf2.inday)<7) ntotal3++;
		}
	}
	if (fp!=NULL) fclose(fp);
}



disp_ctype()
{
	int i;
	cls();
	printf("           ��ȣȸ ȸ�� ��޺� ȯ�漳�� \n");
	printfL2();
	printf(" ���    ����̸�\n");
	printfL2();
	for (i=0;i<20;i++) {
		disp_ctype2(i);
	}
}


disp_ctype2(int i)
{
		xy(1,i+5);
		printf("  %2d    %-8s\n",i,actype[i]);
}


sys_back(char *dir)
{
	struct tm *tp;
	time_t today;
	int now_day;
	char filename1[100];
	char filename2[100];
	char buff[100];
	struct stat stbuf;

	time(&today);	//������ ��¥ ����
	tp = localtime(&today);
	now_day=tp->tm_mday;

	sprintf(buff,"%s/sys_back",dir);check_dir(buff);
	sprintf(filename1,"%s/sys_back/id%02d.lzh",dir,now_day);
	sprintf(filename2,"%s/sys_back/menu%02d.lzh",dir,now_day);
	//����������  ����Ÿ�� �����ѽð�
	if (is_old_file(filename1,24*60*60)) {
		sprintf(buff,"%s/sys_back",dir);
		check_dir(buff);
		printf("\n\n");
		printf(" ID  -> %s �� ����մϴ�.\n",filename1);
		printf(" �޴�-> %s �� ����մϴ�.\n",filename2);
		sprintf(buff,"%s/system/clubid.dat",dir);
		if (is_file(buff)) {
			sprintf(buff,"lha aq %s %s/system/clubid.dat &",filename1,dir);
			sys_tem2(buff);
		}
		sprintf(buff,"%s/system/menu.dat",dir);
		if (is_file(buff)) {
			sprintf(buff,"lha aq %s %s/system/menu.dat &",filename2,dir);
			sys_tem2(buff);
		}
	}
}

//������ ��ȣȸ�� �޴��� ������ ��ũ��Ų��.
link_menu()
{
	int i,found;
	char buff[400];
	char buff1[50];
	char buff2[50];
	char file1[300];
	char file2[300];
	char file3[300];
	char file4[300];
	char olddir[280];
	if (is_file("system/no_del.dat")) return;

	getcwd(olddir,sizeof(olddir));		//������ ���丮

	xy(1,22);
	printf("\n\n\n\n\n\n\n��ũ��ų ��ȣȸ�� �޴��ڵ� >>");
	lineinput(buff1,9); if (buff1[0]==0) return;
	printf("\n��ũ��ų ������   �޴��ڵ� >>");
	lineinput(buff2,9); if (buff2[0]==0) return;

	printf("\n");
	if (!good_filename(buff1)||!good_filename(buff2) ) {
		printf("������ �����ڵ尡 �ƴմϴ�.\n");
		getchar();
		return;
	}
	sprintf(buff,"menu/%s.tit",buff2);
	if (!is_file(buff)) {
		printf("%s ȭ���� ã���� �����ϴ�.\n",buff);
		getchar();
		return;
	}

	if (!find_go(buff1)) {
		mess("%s �����ڵ尡 ��ȣȸ�� �����ϴ�.\n",buff1);
		return;
	}

	sprintf(buff,"%s/menu/%s.tit",club_go.dir,buff1);
	if (is_file(buff)) {
		printf("\n��ȣȸ �� %s ȭ���� �ֽ��ϴ� ���� �����ұ��? >>",buff);
		if (yesno("",No)==No) return;
	}

	sprintf(file2,"%s/menu/%s",olddir,buff2);
	sprintf(file1,"%s/menu/%s",club_go.dir,buff1);
	sprintf(file4,"%s/logo/%s",olddir,buff2);
	sprintf(file3,"%s/logo/%s",club_go.dir,buff1);

	sys_tem("ln -sf %s.tit %s.tit",file2,file1);
	sys_tem("ln -sf %s.txt %s.txt",file2,file1);
	sys_tem("ln -sf %s.log %s.log",file4,file3);
	mess("���� %s -> ��ȣȸ %s ��ũ",file2,club_go.code);
}


//������ ��ȣȸ�� ��ȣ�� ���̵� ã�´�.  ->cpf�� ����
read_cpfn(int num)
{
	FILE *fp;
	int ret=No;
	fp=fopen(id_file,"r+");
	if (fp==NULL) return No;   //���̵� ����
	fseek(fp,(num-1)*sizeof_cpf,SEEK_SET);
	if (fread(&cpf,sizeof_cpf,1,fp)>0&&num==cpf.num) ret=Yes;
	if (fp!=NULL) fclose(fp);
	return ret;
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
						if (fp!=NULL) fclose(fp);
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
							if (fp!=NULL) fclose(fp);
							if (fp2!=NULL) fclose(fp2);
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

job_idset()
{
	int n;
	char buff[200];
	while (1) {
		cls();
		printfL2();
		printf("            ���� ���� ���� ȸ�� ����\n");
		printfL2();
		printf("  1.  .add(��������)  ��� ���� ���̵�          (system/edit_ad.id)\n");
		printfL2();
		printfL2();
		n=numselect();
		xy(60,n+3);
             if (n<=0)  break;
		else if (n==1)	{
			sprintf(buff,"%s/system/edit_ad.id",club_go.dir);
			xedit(buff);
		}
	}
}


job_money()
{
	int n;
	while (1) {
		cls();
		printf("  ###  ���̹��Ӵ� ���� �۾�  ###\n");
		printf("  1. ��ȣȸ ���̹��Ӵ� BANK ȸ��                :%s\n",ccfg.bankman);
		printfL2();
		n=numselect();
		xy(60,n);
        if (n<=0) break;
		if (n==1) get_str(ccfg.bankman,8);
	}
}


job_notice()
{
	while (1) {
		cls();
		print_(1,"�������� ����");
		printf(" 1. �Ϲ�ȸ�� ���ӽ� �������� ���      :%s\n",rbit(ccfg.notice,0,"���","���"));
		printf(" 2. �մ�     ���ӽ� �������� ���      :%s\n",rbit(ccfg.notice,1,"���","���"));
		printf(" 3. �������� ���� ���                 :%s\n",rbit(ccfg.notice,4,"��","����"));
		printf(" 4. �������� ��¸�� (0~9)            :%d\n",ccfg.noticet);
		printf(" 5. �������� ��½��� ��ġ X           :%d\n",ccfg.noticex);
		printf(" 6. �������� ��½��� ��ġ Y           :%d\n",ccfg.noticey);
		printf(" 7. �������� ��½��� �ٱ���           :%d\n",ccfg.noticeline);
		n=numselect();xy(60,n+1);
        if (n<= 0) break;
		if (n== 1) ccfg.notice	   =gtt_bit(ccfg.notice ,0);
		if (n== 2) ccfg.notice	   =gtt_bit(ccfg.notice ,1);
		if (n== 3) ccfg.notice	   =gtt_bit(ccfg.notice ,4);
		if (n== 4) ccfg.noticet    =get_num(ccfg.noticet,2);
		if (n== 5) ccfg.noticex    =get_num(ccfg.noticex,2);
		if (n== 6) ccfg.noticey    =get_num(ccfg.noticey,2);
		if (n== 7) ccfg.noticeline =get_num(ccfg.noticeline,2);
	}
}


check_initdir()
{
}

findmenu(int mode)
{
	char buff[300];
	unlink(TMP_FILE2);
    sys_tem("bin/findmenu %d '%s' '%s' ",mode,club_go.code,TMP_FILE2);
    file2buff(auto_cmd,TMP_FILE2,30);
}


view_daycpf()        //�Ϻ� ������Ȳ
{
    FILE *fp,*fp2;
    time_t indate;
    int inday;
    int i,n,count1,count2;
    fp=fopen(id_file,"r");
    i=0;
    printf("\n");
    indate=0;
    inday=count1=count2=0;
    fp2=fopen(TMP_FILE,"w");
    while (fread(&cpf,sizeof_cpf,1,fp)) {
        n=time_num(cpf.inday);
        if (inday>0&&inday!=n) {
            fprintf(fp2,"%s %6d�� ����  ����%6d��\n",datestr(35,indate),count1,count2);
            count1=0;
        }
        indate=cpf.inday;
        inday=n;
        count1++;
        count2++;
    }
    fclose(fp);
    fclose(fp2);
    view_text(TMP_FILE);
}
