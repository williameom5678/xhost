////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: �������� BBS HOST							  //
//			  //			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //	  //
//			  �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����			  //
//			  �������� ������ �޾ƾ� �մϴ�.						  //
////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//	��ȣȸ ȸ�� ���� �޴�
///////////////////////////////////////////////////////////////////////
#include "bbs.h"
char now_id[9];
int test=No;	 //���Կ���
PF now_pf;
char id_file[300];

//��ȣȸ ���Ը��η�ƾ
main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	int n=0;
	int key=No;
	int found,num;
	char buff[140];
	char buff1[140];
	char tcode[60];

	if (argc<3) {
		printf("\n����  cguest <��ȣȸ �ڵ�> <��û�ھ��̵�>\n");
		return;
	}

	read_cfg();
	set_tmpfile();
	club_mode=No;
	read_atype();	//����б�
	read_ajob();	//�����б�
	strcpy(tcode,argv[1]);				//��ȣȸ �ڵ�
	if (is_same(tcode,"main")) return;
	strcpy(now_id,argv[2]); 		  //��û�� ���̵�
	if (!find_club(tcode)) {mess("%s��ȣȸ �˻�����",tcode);return;}
	club_mode=Yes;
	read_ccfg();
	sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
	read_atype();						//����б�
	read_actype();						//��ȣȸ����� �̸��� �о��  ->actype
	club_mode=Yes;
	if (!read_pf(now_id)) {
		mess("%s ID�� ã���� �����ϴ�.",now_id);
		return;
	}
	now_pf=pf;
	if (pf.type==0) {
		mess("%s ���� �����Ҽ� ���� ����Դϴ�.",now_id);
		return;
	}
	read_cpf(now_id);

	while(1) {
		if (!view("%s/frm/guest/cguest.mnu",club_go.dir))
			view("frm/guest/cguest.mnu");
		lineinput(buff,2);check_han(buff);printf("\n");n=atoi(buff);
		if (is_char("NnQqPpXx",buff[0])) break;
		if (n==1) {
			if (!view("%s/logo/cintro.mnu",club_go.dir))
				view("frm/guest/cintro.mnu");
		}
		else if (n==2) cguest_in(now_id);
		else if (n==3) cguest_check(now_id);
		else if (n==4) cguest_find();
	}
}




//��ȣȸ ���Ը��η�ƾ
cguest_in(char *id)
{
	FILE *fp;
	int n=0;
	int key=No;
	int found,num;
	CPF cpf2;
	char buff[140];
	char buff1[140];
	char tcode[60];

	found=No;
    fp=fopen(id_file,"r");
	while (fread(&cpf2,sizeof_cpf,1,fp)) {
		if (is_same(cpf2.id,id)) {
			found=Yes;
			if (cpf2.type>=16||now_pf.type>=16) {
				test=Yes;  //����
				found=No;
				printf("\n �̹� ������ �Ǿ� �ֽ��ϴ�. \n");
				printf("\n ���Խ�û �׽�Ʈ�Դϴ�.(����ȵ�)\n");
				pressenter();
				break;
			}
			else if (cpf2.type==0) mess("�̹� ������ �Ǿ� �ִ� ���̵��Դϴ�. (���0)");
			else if (cpf2.type==1) mess("�̹� ���� ��û���� ���̵��Դϴ�.");
			else mess("�̹� ������ �� ���̵��Դϴ�.");
			break;
		}
	}
	fclose(fp);
	if (found) {
		mess("������ ��ҵǾ����ϴ�.");
		return;
	}

	sprintf(buff1,"%s/logo/notenter.log",club_go.dir);
	cls();

	if (ccfg.autoguest==2) {
		if (!view(buff1)) mess("�� ��ȣȸ�� ���Խ�û�� ���� �ʽ��ϴ�.");
		return;
	}
	if (ccfg.stop) {
		if (!view(buff1)) mess("�� ��ȣȸ�� ���� �ӽ� ��� ���Դϴ�.");
		return;
	}
	if (ccfg.intype>0&&pf.type<ccfg.intype) {
		if (!view(buff1)) mess("�� ��ȣȸ�� �����Ҽ� �ִ� ����� �ƴմϴ�.");
		return;
	}
	if (ccfg.level>0&&pf.level<ccfg.level) {
		if (!view(buff1)) mess("�� ��ȣȸ�� �����Ҽ� �ִ� ������ �ƴմϴ�.");
		return;
	}
	n=get_age(pf);
	if ((ccfg.gage1>0&&n<ccfg.gage1)||(ccfg.gage2>0&&n>ccfg.gage2)) {
		if (!view(buff1)) mess("�� ��ȣȸ�� �����Ҽ� �ִ� ������ �ƴմϴ�.");
		return;
	}
	sprintf(buff1,"%s/system/notguest.txt",club_go.dir);
	if (in_file2(buff1,pf.id)) {
		sprintf(buff1,"%s/logo/notguest.log",club_go.dir);
		if (!view(buff1)) {
			printf("�� ��ȣȸ�� �����Ҽ� ���� ���̵��Դϴ�.\n");
			pressenter();
		}
		return;
	}

	strcpy(cpf.id	,pf.id);
	strcpy(cpf.name ,pf.name);
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
	cpf.type=2; 		//ȸ�����	0:�ź�/Ż�� 1:�մ� 2:��� 3:ȸ��-

	cls();
	sprintf(buff,"%s/logo/_guest.log",club_go.dir);
	if (!is_file(buff)) sprintf(buff,"frm/guest/cgust.log");
	if (view(buff)) {
		if (!yesno("",Yes)) {
			mess("���Խ�û�� ��� �Ǿ����ϴ�.");
			return;
		}
	}
	n=0;
	while (n++<7) {
		key=No;
		strcpy(g_buff4,ccfg.autoguest?"�ڵ� ����ó��":"");
		disp_cguest();
		if (!bit(ccfg.memo_input,1)) {
			xy(14,12);lineinput(buff,64);if (buff[0]!=0) strcpy(cpf.memo1,buff);
			if (is_same(buff,"q")) {key=No;break;}
		}
		if (!bit(ccfg.memo_input,2)) {
			xy(14,13);lineinput(buff,64);if (buff[0]!=0) strcpy(cpf.memo2,buff);
			if (is_same(buff,"q")) {key=No;break;}
		}
		if (!bit(ccfg.memo_input,3)) {
			xy(14,14);lineinput(buff,64);if (buff[0]!=0) strcpy(cpf.memo3,buff);
			if (is_same(buff,"q")) {key=No;break;}
		}
		if (!bit(ccfg.memo_input,4)) {
			xy(14,15);lineinput(buff,64);if (buff[0]!=0) strcpy(cpf.memo4,buff);
			if (is_same(buff,"q")) {key=No;break;}
		}
		if (!bit(ccfg.memo_input,5)) {
			xy(14,16);lineinput(buff,64);if (buff[0]!=0) strcpy(cpf.memo5,buff);
			if (is_same(buff,"q")) {key=No;break;}
		}
		disp_cguest();
		xy(1,18);
		printf("��ȣȸ ���� ��û�� �ұ��? (Y:��û N:���Է�/���� Q:��û���)>>");
		lineinput(buff,2);check_han(buff);
		buff[0]=tolower(buff[0]);
		if (buff[0]=='y') {key=Yes;break;}
		if (buff[0]=='q') {key=No ;break;}
		if (buff[0]=='n') {key=No ;n=0;}
	}

	if (key) {
        fp=fopen(id_file,"r+");
		if (fp==NULL) fp=fopen(id_file,"w+");
        fseekend(fp);
        cpf.num=ftell(fp)/sizeof_cpf+1;
        fseek(fp,cpf.num*sizeof_cpf,SEEK_SET);
		//0:�մ� 1:��û 2:ȸ��
		if (ccfg.autoguest) cpf.type=2;
		else				cpf.type=1;
        printf("club:%s file:%s\n",club_go.code,id_file);
        printf("id:%s id:%s pos:%d\n",id,cpf.id,ftell(fp));;;;;;;;
		if (!test) {
			fwrite(&cpf,sizeof_cpf,1,fp);
		}
		fclose(fp);
		cls();
        if (ccfg.autoguest==1) {
			sprintf(buff,"%s/logo/_guestau.log",club_go.dir);
			view(buff);
		}
		else {
			sprintf(buff,"%s/logo/_guestok.log",club_go.dir);
			view(buff);
		}
		xy(1,19);
		mess("���Խ�û�� �Ǿ����ϴ�.");
	}
	else {
		xy(1,19);
		cpf.type=0;
		mess("���Խ�û�� ��� �Ǿ����ϴ�.");
	}
}



//cpf�� ���̵� ���
disp_cguest()
{
	PF oldpf;
	oldpf=pf;
	read_pf(cpf.id);	//id�� �о� pf�� ����
	if (!view("%s/frm/guest/cguest.scr",club_go.dir))
		view("frm/guest/cguest.scr");
	pf=oldpf;
}


//��ȣȸ ����Ȯ��
cguest_check(char *id)
{
	FILE *fp;
	CPF cpf2;
	int i,found=No;

	fp=fopen(id_file,"r+");
	i=0;
	while (fread(&cpf2,sizeof_cpf,1,fp)) {
		if (is_same(cpf2.id,id)) {
			if (cpf2.type==0) {
				mess("�̹� ������ �Ǿ� �ִ� ���̵��Դϴ�. (���0)");
			}
			if (cpf2.type==1) {
				mess("���� ��û���� ���̵��Դϴ�.");
				if (yesno("���Դ���û�� ����ϰڽ��ϱ�?(y/N)>>",No)) {
					printf("\n");
					strcpy(cpf2.id,"");
					strcpy(cpf2.name,"");
					cpf2.type=0;
					fseek(fp,i*sizeof_cpf,SEEK_SET);
					fwrite(&cpf2,sizeof_cpf,1,fp);
					mess("���Խ�û�� ��ҵǾ����ϴ�.");
				}
			}
			else {
				mess("�̹� ������ �� ���̵��Դϴ�.");
			}
			found=Yes;
			break;
		}
		i++;
	}
	fclose(fp);
	if (!found) {
		mess("%s ���� ã��� �����ϴ�.",id);
	}
}

cguest_find()
{
	FILE *fp;
	char buff[10],buff2[10];
	int found=No;
	CPF cpf2;

	printf("\n\n �˻��� ID �Ǵ� ���� >>");
	lineinput(buff,8);check_han(buff);printf("\n");
	if (strlen(buff)<3) return;

	fp=fopen(id_file,"r+");
	while (fread(&cpf2,sizeof_cpf,1,fp)) {
		if (is_same(cpf2.id,buff)||is_same(cpf2.name,buff)) {
			found=Yes;
			printf(" %-8s(%-8s)   (Y/n)>>",cpf2.id,cpf2.name);
			lineinput(buff2,2);check_han(buff2);printf("\n");
			if (is_same(buff2,"q")) break;
			if (!is_same(buff2,"n")) {
				disp_cpf(cpf2,0);
				break;
			}
		}
	}
	fclose(fp);
	if (!found) {
		mess("%s���� ã���� �����ϴ�.",buff);
	}
}

