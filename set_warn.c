////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���� : ȸ�� ��� ���� ���� ���
////////////////////////////////////////////////////////////////////////
#include "util.c"
int wtotal[11];
char ccode[9];
char now_id[9];
char id_file[90];
char warn_name[26][35];

main(argc, argv)
int argc;
char *argv[];
{
	char buff[90];
	char buff2[90];
	int num,n,i;
	FILE *fp;

	findport(tty);
	read_cfg();
	read_cfg();
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
	set_nowmenu();		 //����Ÿȭ�������ϱ�

    for(i=0;i<10;i++) wtotal[i]=0;

    strcpy(now_id,argv[1]);                 //���̵�
    strcpy(ccode,argv[2]);              //��ȣȸ �ڵ�
	findport(tty); //������Ʈ ã��
	read_cfg();
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
	if (!find_club(ccode)) {
		mess("%s ��ȣȸ �˻� ����",ccode);
		return;
	}
	set_nowmenu();		 //����Ÿȭ�������ϱ�
	read_ccfg();
	set_bcfgfile(); 					//BCFG_FILE�� ����
	read_bcfg();
    if (!read_pf(now_id)) {
        mess("%s �˻�����",now_id);
		return;
	}
    read_cpf(now_id);                    //�ش�Ǵ� ��ȣȸ�� ���̵� ã�´�.
	read_atype();					 //����б�
	read_actype();					 //��ȣȸ����� �̸��� �о��  ->actype
	who=check_who();				 //1:��ȣȸ ��� 2:��ȣȸ �û�  3:root����
	if (who<1) {
        mess("%d,%d ���޴����Ұ��ɵ��",who,pf.type);
		return;
	}
    read_warn_name();

    if (club_mode) sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
    else           sprintf(id_file,ID_FILE);

	check_total();
	while (1) {
		cls();
        if (bit(cfg.mode,20)) {
            disp_wtotal();
        }
        xy(1,1);
        printfL2();
        printf("   ȸ�� ��� ���� �۾�  (%s W:%d I:%s) \n",club_go.code,who,now_id);
		printfL2();
        printf("                                      \n");
        printf("  1. ȸ�� ���  ���� / ����           \n");
        printf("  2. ��� ȸ��  ����Ʈ                \n");
        printf("  3. ��� ����  ����                  \n");
        xy(1,15);
		printfL2();
		printf(" ���ù�ȣ?>>");
		num=numinput();
		printf("\n");
             if (num==0) break;
        else if (num==1) set_warn1();
        else if (num==2) warn_list();
        else if (num==3) edit_warn_name();
	}
}


//ȸ�� ��� ����
set_warn1()
{
	FILE *fp;
	char buff[100];
	char buffstr[100];
	PF pf2;
	CPF cpf2;
	int n,i;

	while (1) {
		printf(" ��� ����/���� ȸ���� ���̵� �̸� >>");
		lineinput(buffstr,20);
		printf("\n");
		if (buffstr[0]==0) break;
		if (is_same(buffstr,"q")) break;

		if (!club_mode) {
			fp=fopen(id_file,"r+");
			if (fp==NULL) return;
			while (1) {
				if (!fread(&pf,sizeof_pf,1,fp)) break;
				if (stristr(pf.id,buffstr)||stristr(pf.name,buffstr)||stristr(pf.nick,buffstr)) {
					cls();
                    view_pfw(pf.id,who);
					printf("�� ȸ���� �½��ϱ�? (Y/n/q)>>");
                    lineinput(buff,3);check_han(buff);printf("\n");
					if (is_same(buff,"n")) continue;
					if (is_same(buff,"p")) break;
					if (is_same(buff,"q")) break;
					strcpy(buff,"");
					if (pf.warn%10==0) printf(" %s(%s)���� ��� �����ϴ�.\n",pf.id,pf.name);
					else			   printf(" %s(%s)���� ���%d �����Ǿ� �ֽ��ϴ�. \n",pf.id,pf.name,pf.warn%10);
					printf(" ������ ����� ����? (0:���� 1~9:���1~9  enter:���)>>");
					lineinput(buff,20);printf("\n");
					n=atoi(buff);
					if		(is_same(buff,"0")) {
						pf.warn=0;
						printf(" %s(%s)���� ������� �Ǿ����ϴ�.\n",pf.id,pf.name);
					}
					else if (n>=1&&n<=9) {
						pf.warn=n;
						printf(" %s(%s)���� ���%d ���� �Ǿ����ϴ�.\n",pf.id,pf.name,n);
						printfL2();
						for (i=0;i<20;i++) {
							printf(" %2d.%20s",i+1,warn_name[i+1]);
							if (i%3==2) printf("\n");
						}
						printf("\n");
						printfL2();
						printf(" ��� ���� ��ȣ�� ������ �ֽʽÿ� (1-20)>>");
						i=numinput();
						if (n<1||n>20) i=1;
						pf.warn=i*10+n;
						printf(" %s(%s)���� ���%d (����:%s) ���� �Ǿ����ϴ�.\n",pf.id,pf.name,pf.warn%10,warn_name[pf.warn/10]);
					}
					else break;
					//������
                    fseek(fp,pf.num*sizeof_pf,SEEK_SET);
					fread(&pf2,sizeof_pf,1,fp);
					if (is_same(pf2.id,pf.id)) {
                        fseek(fp,pf.num*sizeof_pf,SEEK_SET);
//                        fwrite(&pf,sizeof_pf,1,fp);
                        gwrite(201,pf,fp);
					}
					break;
				}
			}
			fclose(fp);
		}

		else {
			fp=fopen(id_file,"r+");
			if (fp==NULL) return;
			while (1) {
				if (!fread(&cpf,sizeof_cpf,1,fp)) break;
				if (stristr(cpf.id,buffstr)||stristr(cpf.name,buffstr)) {
                    view_cpf(pf.id,17);
					printf("�� ȸ���� �½��ϱ�? (Y/n/q)>>");
					lineinput(buff,1);
					printf("\n");
					if (is_same(buff,"n")) continue;
					if (is_same(buff,"p")) break;
					if (is_same(buff,"q")) break;
                    if (cpf.warn%10==0) printf(" %s(%s)���� ��� �����ϴ�.\n",cpf.id,cpf.name);
                    else                printf(" %s(%s)���� ���%d �����Ǿ� �ֽ��ϴ�. \n",cpf.id,cpf.name,cpf.warn%10);
					printf(" ������ ����� ����? (0:���� 1~9:���1~9  enter:���)>>");
					lineinput(buff,20);printf("\n");
					n=atoi(buff);
					if		(is_same(buff,"0")) {
                        cpf.warn=0;
                        printf(" %s(%s)���� ������� �Ǿ����ϴ�.\n",cpf.id,cpf.name);
					}
					else if (n>=1&&n<=9) {
                        cpf.warn=n;
                        printf(" %s(%s)���� ���%d ���� �Ǿ����ϴ�.\n",cpf.id,cpf.name,n);
						printfL2();
						for (i=0;i<20;i++) {
							printf(" %2d.%20s",i+1,warn_name[i+1]);
							if (i%3==2) printf("\n");
						}
						printf("\n");
						printfL2();
						printf(" ��� ���� ��ȣ�� ������ �ֽʽÿ� (1-20)>>");
                        i=numinput();printf("\n");
						if (n<1||n>20) i=1;
                        cpf.warn=i*10+n;
                        printf(" %s(%s)���� ���%d (����:%s) ���� �Ǿ����ϴ�.\n",cpf.id,cpf.name,cpf.warn%10,warn_name[cpf.warn/10]);
					}
					else break;
					//������
					fseek(fp,(cpf.num-1)*sizeof_cpf,SEEK_SET);
					fread(&cpf2,sizeof_cpf,1,fp);
					if (is_same(cpf2.id,cpf.id)) {
						fseek(fp,(cpf.num-1)*sizeof_cpf,SEEK_SET);
						fwrite(&cpf,sizeof_cpf,1,fp);
					}

					break;
				}
			}
			fclose(fp);
		}
	}
	check_total();
}


read_warn_name()
{
	FILE *fp;
	char filename[100];
	char buff[100];
	int i;
    sprintf(filename,"%s/system/warnname.txt",club_go.dir);
	for (i=0;i<21;i++) strcpy(warn_name[i],"");
	fp=fopen(filename,"r+");
	if (fp!=NULL) {
		i=0;
		while (i<20&&fgets(buff,100,fp)!=NULL) {
			del_esc(buff);buff[29]=0;
			strcpy(warn_name[i+1],buff);
            i++;
		}
	}
	fclose(fp);
}



edit_warn_name()
{
	FILE *fp;
	char filename[100];
	char buff[100];
	int i,n;
	while (1) {
		cls();
		printf("         ��� ���� ����\n");
		printfL2();
        for(i=0;i<20;i++) printf("%5d.%s\n",i+1,warn_name[i+1]);
		printfL2();
		printf(" ������ ��ȣ ���� >>");
		n=numinput();
		if (n==0) break;
        else if (n>0&&n<21) {
			xy(40,n+2);
            lineinput(warn_name[n],30);
		}
	}

    sprintf(filename,"%s/system/warnname.txt",club_go.dir);
	fp=fopen(filename,"w");
	if (fp!=NULL) {
        for(i=0;i<20;i++) {
            del_esc(warn_name[i]);warn_name[i][29]=0;
            fprintf(fp,"%s\n",warn_name[i]);
		}
	}
	fclose(fp);
}



//ȸ�� ����Ʈ
warn_list()
{
	FILE *fp;
	char buff[100];
	char buff2[100];
    int i,n,n1,n2;

    printf("\n Ȯ���� ����� ������? \n");
    printf(" (1:���1 2:���2 9:���9 99:��ü��� enter:���) >>");
    lineinput(buff,2);printf("\n");
    n1=atoi(buff);
    if (n1==0||buff[0]==0) return;
    printf(" Ȯ���� ����� ������ȣ�� (99:��ü 1-20) >>");
    lineinput(buff,2);n2=atoi(buff);printf("\n");
	if (!club_mode) {
		fp=fopen(id_file,"r+");
		if (fp==NULL) return;
		i=0;
		while (fread(&pf,sizeof_pf,1,fp)) {
            if (pf.warn==0) continue;
            if ( (n1==99||pf.warn%10==n1)&&
                 (n2==99||pf.warn/10==n2) ) {
                printf("%5d. %-8s %-8s %2d  %5d  ",i,pf.id,pf.name,pf.type,pf.level);
                printf("���%d ����%2d %s\n",pf.warn%10,pf.warn/10,warn_name[pf.warn/10+1]);
				if (i%20==19) {
					printf(" [enter]�� �������� (q:����)");
					lineinput(buff,1);
					printf("\n");
					if (is_same(buff,"q")) break;
				}
                i++;
			}
		}
		fclose(fp);
		pressenter();
	}
	else {
		fp=fopen(id_file,"r+");
		if (fp==NULL) return;
		i=0;
		while (fread(&cpf,sizeof_cpf,1,fp)) {
            if (cpf.warn==0) continue;
            if ( (n1==99||cpf.warn%10==n1)&&
                 (n2==99||cpf.warn/10==n2) ) {
                printf("%5d. %-8s %-8s %2d  %5d  ",i,cpf.id,cpf.name,cpf.type,cpf.level);
                printf("���%d ����%2d %s\n",cpf.warn%10,cpf.warn/10,warn_name[cpf.warn/10+1]);
				if (i%20==19) {
					printf(" [enter]�� �������� (q:����)");
					lineinput(buff,1);
					printf("\n");
					if (is_same(buff,"q")) break;
				}
                i++;
			}
		}
		fclose(fp);
		pressenter();
	}
}


check_total()
{
	FILE *fp;
	int i;

    for(i=0;i<10;i++) wtotal[i]=0;

	if (!club_mode) {
		fp=fopen(id_file,"r+");
		if (fp==NULL) return;
		while (fread(&pf,sizeof_pf,1,fp)) {
			if (pf.type==0) continue;
            wtotal[0]++;
            if (pf.warn%10!=0) wtotal[pf.warn%10]++;
		}
		fclose(fp);
	}
	else {
		fp=fopen(id_file,"r+");
		if (fp==NULL) return;
		while (fread(&cpf,sizeof_cpf,1,fp)) {
			if (cpf.type==0) continue;
            wtotal[0]++;
            if (cpf.warn%10!=0) wtotal[cpf.warn%10]++;
		}
		fclose(fp);
	}
}


disp_wtotal()
{
    xy(50, 5); printf("��üȸ��: %8d\n",wtotal[0]);
    xy(50, 6); printf("���1 ��: %8d\n",wtotal[1]);
    xy(50, 7); printf("���2 ��: %8d\n",wtotal[2]);
    xy(50, 8); printf("���3 ��: %8d\n",wtotal[3]);
    xy(50, 9); printf("���4 ��: %8d\n",wtotal[4]);
    xy(50,10); printf("���5 ��: %8d\n",wtotal[5]);
    xy(50,11); printf("���6 ��: %8d\n",wtotal[6]);
    xy(50,12); printf("���7 ��: %8d\n",wtotal[7]);
    xy(50,13); printf("���8 ��: %8d\n",wtotal[8]);
    xy(50,14); printf("���9 ��: %8d\n",wtotal[9]);
}
