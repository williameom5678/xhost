////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// �Խ��� ������� �޴�
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//����  bsysop hello 1 forum

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp1,*fp2,*fp3,*fp4;
	PF pf2;
	int i,n=0,n1,n2;
	int key=No;
	int now_posi,count;
    char buff[140],buff1[140],buff2[140],buff3[140],buff4[140];
	char filename[140];
	char tcode[9];
	char ccode[9];
	char id[9];
	char old_dir[100];
	char ch,c;
	time_t date1,date2;
	getcwd(old_dir,sizeof(old_dir));	  //������ ���丮

	strcpy(tcode,argv[1]);				//�����ڵ�
	strcpy(ccode,argv[2]);				//��ȣȸ �ڵ�
	strcpy(id,argv[3]); 				//���̵�
	findport(tty); //������Ʈ ã��
	read_cfg();
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
	if (!find_club(ccode)) {
		mess("%s ��ȣȸ �˻� ����",ccode);
		return;
	}
	set_nowmenu();		 //����Ÿȭ�������ϱ�
	if (!find_go(tcode)) {
		mess("%s �����ڵ� �˻� ����",tcode);
		return;    //ã�� �����ڵ尡 ����
	}

	read_ccfg();
	set_bcfgfile(); 					//BCFG_FILE�� ����
	read_bcfg();
	read_pf(id);
	read_cpf(id);					 //�ش�Ǵ� ��ȣȸ�� ���̵� ã�´�.
	read_atype();					 //����б�
	read_actype();					 //��ȣȸ����� �̸��� �о��  ->actype
	who=check_who();				 //1:��ȣȸ ��� 2:��ȣȸ �û�  3:root����

	n1=n2=date1=date2=0;
	strcpy(buff1,"");
	strcpy(buff2,"");
	strcpy(buff3,"");
    strcpy(buff4,"");

	while (1) {
		if (n2==0) n2=99999999;
		if (n1>n2) {n=n1;n1=n2;n2=n;}
		if (date2==0) time(&date2);
		if (strlen(buff3)<1) strcpy(buff3,go.code);
		sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
		sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,go.code);
		sprintf(filename,"%s.zip",buff3);
		cls();
		printfL2();
		printf("  %s(%s)      �Խ��� ���� �ٿ� �۾�\n",go.code,go.name);
		printfL2();
		printf(" * �����ڵ�� : %s (%s)\n",go.code,go.name);
		printf(" * ����ȭ�ϸ� : %s (%s)\n",NOW_FILE,NOW_FILED);
		printf(" * ����ȭ�ϸ� : %s\n",filename);
		printfL2();
		printf(" 1.  �ڷ� ��ȣ ���� - ��  : %d - %d\n",n1,n2);
		strcpy(buff,datestr(1,date1));
		printf(" 2.  �ڷ� ��¥ ���� - ��  : %s ~ %s\n",buff,datestr(1,date2));
		printf(" 3.  ������ ���̵�        : %s\n",buff1);
        printf(" 4.  ������ TCODE         : %s\n",buff4);
        printf(" 5.  ������ �����ڿ�    : %s\n",buff2);
        printf(" 6.  ���� ���ϸ�          : %s\n",buff3);
        printf(" 7.  ���� �������� %s �Խ��� �ٿ� ����\n",go.code);
        printf(" 8.  ���� �������� %s XFORMAT�ٿ� ����\n",go.code);
		printfL2();
		printf(" ����  ���(Q) ����(H) >> ");
		lineinput(buff,5);check_han(buff);printf("\n");
		n=atoi(buff);
		if (is_same(buff,"x")||is_same(buff,"q")||is_same(buff,"p")) return;
		else if (is_same(buff,"h")) {view("frm/help/bdn.hlp");pressenter();}
		else if (n==1) {
			printf(" �ٿ���� ��ȣ�� ���۹�ȣ�� �Է����ּ���.. >>");
			n1=numinput();printf("\n");
			printf(" �ٿ���� ��ȣ�� ��  ��ȣ�� �Է����ּ���.. >>");
			n2=numinput();printf("\n");
		}
		else if (n==2) {
			strcpy(buff,datestr(1,date1));
			printf(" �ٿ���� �ڷ��� ���۳�¥�� �Է����ּ���..(%s) >>",buff);
			dosinput(buff,10);date1=str2date(buff);printf("\n");
			strcpy(buff,datestr(1,date2));
			printf(" �ٿ���� �ڷ��� ��  ��¥�� �Է����ּ���..(%s) >>",buff);
			dosinput(buff,10);date2=str2date(buff);printf("\n");
		}
		else if (n==3) {
			printf(" ������ ���̵� �Է��ϼ���. (������Enter) >>");
			lineinput(buff1,8);printf("\n");
			if (is_quit(buff1)) return;
		}
        else if (n==4) {
            printf(" ������ TCODE �� �Է��ϼ���. (������Enter) >>");
            lineinput(buff4,8);printf("\n");
            if (is_quit(buff4)) return;
		}
        else if (n==5) {
			printf(" ������ ������ ���ڿ� �Է�   (������Enter) >>");
			lineinput(buff2,10);printf("\n");
			if (is_quit(buff2)) return;
		}
        else if (n==6) {
			printf(" ������ ���ϸ� �Է�   (Ȯ��������) >>");
			lineinput(buff3,8);printf("\n");
			strchng2(buff3,".","");del_esc3(buff3);
			if (is_quit(buff3)) return;
		}
        else if (n==7&&yesno2(" �� �Խ����� �ٿ�ްڽ��ϱ�? (Y/n)>>",Yes)) {
			printf("\n......ȭ�� ������.............\n");
			fflush(stdout);
			fp1=fopen(NOW_FILE ,"r");
			fp2=fopen(NOW_FILED,"r");
			if (fp1==NULL||fp2==NULL) return;
			sys_tem("rm tmp/up%d%s -rf",cfg.hostno,no_spc(tty));
			check_dir("tmp/up%d%s",cfg.hostno,no_spc(tty));
			sprintf(TMP_FILE,"tmp/up%d%s/%s.tit",cfg.hostno,no_spc(tty),buff3); // �ӽ�ȭ��
			sprintf(TMP_FILE2,"tmp/up%d%s/%s.txt",cfg.hostno,no_spc(tty),buff3); // �ӽ�ȭ��
			fp3=fopen(TMP_FILE ,"w+");
			fp4=fopen(TMP_FILE2,"w+");
			now_posi=0;
			count=0;
			while (fread(&bbs,sizeof_bbs,1,fp1)){
				fseek(fp2,bbs.posi,SEEK_SET);
				fseek(fp4,now_posi,SEEK_SET);
				if (bbs.del||bbs.num<n1||bbs.num>n2)		continue;
				if (buff1[0]!=0&&!is_same(bbs.id,buff1))	continue;
                if (buff4[0]!=0&&!is_same(bbs.tcode,buff4))    continue;
				if (buff2[0]!=0&&!stristr(bbs.title,buff2)) continue;
				if (bbs.date<date1||bbs.date>date2) 		continue;
				for (i=0;i<bbs.size;i++)
					fputc(fgetc(fp2),fp4);			   //���뿡 ����
				bbs.posi=now_posi;
				now_posi+=bbs.size;
				fwrite(&bbs,sizeof_bbs,1,fp3);
				count++;
			}
			fclose(fp1);
			fclose(fp2);
			fclose(fp3);
			fclose(fp4);
			printf("  �� %d��  %d byte ���� �Ϸ�\n",count,now_posi);
			fflush(stdout);
			set_statdown(8);	   //�޼��� ���Ź���
			if (count>0) {
				sprintf(buff,"tmp/up%d%s",cfg.hostno,no_spc(tty));
				chdir(buff);
				sprintf(filename,"%s.zip",buff3);
				sys_tem("zip %s %s.tit %s.txt",filename,buff3,buff3);
				chdir(old_dir); 	 //������ ���丮
				sprintf(buff,"tmp/up%d%s/%s",cfg.hostno,no_spc(tty),filename);
				if (is_file(buff)) {
					sz_file(buff);
				}
			}
			chdir(old_dir); 	 //������ ���丮
			set_statdown(OLD_MONI_MODE);	   //�޼��� ���Ź���
			sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty)); // �ӽ�ȭ��
		}
        else if (n==8&&yesno2(" �� �Խ����� �ٿ�ްڽ��ϱ�? (Y/n)>>",Yes)) {
			printf("\n......ȭ�� ������.............\n");
			fflush(stdout);
			fp1=fopen(NOW_FILE ,"r");
			fp2=fopen(NOW_FILED,"r");
			if (fp1==NULL||fp2==NULL) return;
			sys_tem("rm tmp/up%d%s -rf",cfg.hostno,no_spc(tty));
			check_dir("tmp/up%d%s",cfg.hostno,no_spc(tty));
			sprintf(TMP_FILE,"tmp/up%d%s/%s.cap",cfg.hostno,no_spc(tty),buff3); // �ӽ�ȭ��
			fp3=fopen(TMP_FILE ,"w+");
			now_posi=0;
			count=0;
			while (fread(&bbs,sizeof_bbs,1,fp1)){
				if (bbs.del||bbs.num<n1||bbs.num>n2)		continue;
				if (buff1[0]!=0&&!is_same(bbs.id,buff1))	continue;
                if (buff4[0]!=0&&!is_same(bbs.tcode,buff4))    continue;
				if (buff2[0]!=0&&!stristr(bbs.title,buff2)) continue;
				if (bbs.date<date1||bbs.date>date2) 		continue;
				fprintf(fp3,"__[[����[[%s\n",bbs.id);
				fprintf(fp3,"__[[�̸�[[%s\n",bbs.name);
				fprintf(fp3,"__[[��¥[[%s\n",datestr(8,bbs.date));
				fprintf(fp3,"__[[����[[%s\n",datestr(8,bbs.date2));
				fprintf(fp3,"__[[�ټ�[[%d\n",bbs.line);
				fprintf(fp3,"__[[��ȸ[[%d\n",bbs.read);
				fprintf(fp3,"__[[�ٿ�[[%d\n",bbs.down);
				fprintf(fp3,"__[[����[[%s\n",bbs.filename);
				fprintf(fp3,"__[[Ű��[[%s\n",bbs.key);
				fprintf(fp3,"__[[����[[%s\n",bbs.key);
				ch=0;
				fseek(fp2,bbs.posi,SEEK_SET);
				for (i=0;i<bbs.size;i++) {
					ch=fgetc(fp2);
					if (ch==0||ch==-1) break;
					fputc(ch,fp3);			   //���뿡 ����
				}
				if (ch!='\n') fprintf(fp3,"\n");
				count++;
				fprintf(fp3,"_[[�Ϸ�[[\n");
			}
			fclose(fp1);
			fclose(fp2);
			fclose(fp3);
			fflush(stdout);
			set_statdown(8);	   //�޼��� ���Ź���
			if (count>0) {
				sprintf(buff,"tmp/up%d%s",cfg.hostno,no_spc(tty));
				chdir(buff);
				sprintf(filename,"%s.zip",buff3);
				sys_tem("zip %s %s.tit %s.txt",filename,buff3,buff3);
				chdir(old_dir); 	 //������ ���丮
				sprintf(buff,"tmp/up%d%s/%s",cfg.hostno,no_spc(tty),filename);
				if (is_file(buff)) {
					sz_file(buff);
				}
			}
			chdir(old_dir); 	 //������ ���丮
			set_statdown(OLD_MONI_MODE);	   //�޼��� ���Ź���
			sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty)); // �ӽ�ȭ��
		}
	}
}

