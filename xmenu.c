////////////////////////////////////////////////////////////////////////
// XHOST: �������� BBS HOST  ����:�輺��(053-964-1002) BBS:053-963-2190
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"
main(argc, argv)
int argc;
char *argv[];
{
    char buff[500];
    char logo_file[500];
    char help_file[500];
    char filename[500];
    int n,who;
	char tcode[9];
	char ccode[9];
	char id[9];

    club_mode=!is_same(club_go.code,"main");

    strcpy(ccode,argv[1]);              //��ȣȸ �ڵ�
    strcpy(tcode,argv[2]);              //�����ڵ�
    strcpy(id,argv[3]);                 //���̵�
	findport(tty); //������Ʈ ã��
	read_cfg();
	;
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
	set_nowmenu();		 //����Ÿȭ�������ϱ�
	if (!find_club(ccode)) {
		mess("%s ��ȣȸ �˻� ����",ccode);
		return;
	}
	if (!find_go(tcode)) {
		mess("%s �����ڵ� �˻� ����",tcode);
		return;    //ã�� �����ڵ尡 ����
	}

	read_ccfg();
	set_bcfgfile(); 					//BCFG_FILE�� ����
	read_bcfg();
	if (!read_pf(id)) {
		mess("%s �˻�����",id);
		return;
	}
	read_cpf(id);					 //�ش�Ǵ� ��ȣȸ�� ���̵� ã�´�.
	read_atype();					 //����б�
	read_actype();					 //��ȣȸ����� �̸��� �о��  ->actype
	who=check_who();				 //1:��ȣȸ ��� 2:��ȣȸ �û�  3:root����

    mview(Yes,"%s/logo/%s.vtx",club_go.dir,go.code);
    mview(Yes,"%s/logo/%s.jpg",club_go.dir,go.code);

    sprintf(logo_file,"%s/logo/%s.mnu",club_go.dir,go.code);
    sprintf(help_file,"%s/logo/%s.hlp",club_go.dir,go.code);
    cls();
	while(1) {
        view(logo_file);
        lineinput(buff,5);printf("\n");
        check_han(buff);
		n=atoi(buff);
        if ((is_same(buff,"cfg")||is_same(buff,".s")||
            is_same(buff,".sys")||is_same(buff,".s"))&&
            (club_mode&&cpf.type>15||!club_mode&&pf.type>15)) edit_scr();
        if (is_char("XxQqQPp",buff[0])) break;
        if (is_char("Hh",buff[0])) view(help_file);
        if (is_same(buff,"0")||(n>0&&n<99)) {
            sprintf(filename,"%s/logo/%s.%d",club_go.dir,go.code,n);
            if (!view(filename)) {
                sprintf(filename,"%s/logo/%s.err",club_go.dir,go.code);
                view(filename);
            }
        }
	}
}


edit_scr()
{
	FILE *fp;
    int n=0,i,n2=0;
	int key;
	int can_edit=No;		//����������?
	char buff[240];
	char tcode[60];
	char now_id[60];
	char filename[200];
	char filename2[200];
	char id_file[200];
	char cfg_file[200];
	char dir[200];
	char cmd;

	strcpy(tcode,go.code);			  //�����ڵ�
    sprintf(dir,"%s/logo",club_go.dir);
	while (1) {
		cls();
        printf("   ### %s ȭ�� ���� �޴�  \n",tcode);
        printf(" ���Էΰ�  (%s.log)    1.��������   2.���   3.�ٿ�   4.���� \n",tcode);
        printf(" ����ȭ��  (%s.mnu)   11.��������  12.���  13.�ٿ�  14.���� \n",tcode);
        printf(" ���ÿ���  (%s.err)   21.��������  22.���  23.�ٿ�  24.���� \n",tcode);
        printf(" �� �� ��  (%s.hlp)   31.��������  32.���  33.�ٿ�  34.���� \n",tcode);
        printf(" ����ȭ��  (%s.???)   41.��������  42.���  43.�ٿ�  44.���� \n",tcode);
        printf(" ����VTX   (%s.vtx)   51.��������  52.���  53.�ٿ�  54.���� \n",tcode);
        printf(" ����JPG   (%s.jpg)                62.���  63.�ٿ�  64.���� \n",tcode);
        n=numselect();
        if (n/10==4) {
            printf(" ������ ȭ���� ���ù�ȣ��? (%s.???) ���(Q) >>",tcode);
            lineinput(buff,5);printf("\n");
            n2=atoi(buff);
            if (is_char("qQpPxX",buff[0])||buff[0]==0) continue;
        }
        if      (n== 0) break;
        else if (n< 10) sprintf(filename,"%s/%s.log",dir,tcode);
        else if (n< 20) sprintf(filename,"%s/%s.mnu",dir,tcode);
        else if (n< 30) sprintf(filename,"%s/%s.err",dir,tcode);
        else if (n< 40) sprintf(filename,"%s/%s.hlp",dir,tcode);
        else if (n< 50) sprintf(filename,"%s/%s.%d",dir,tcode,n2);
        else if (n==51) continue;
        else if (n< 60) sprintf(filename,"%s/%s.vtx",dir,tcode);
        else if (n==61) continue;
        else if (n< 70) sprintf(filename,"%s/%s.jpg",dir,tcode);

		if (n%10==1) {
			xedit(filename);
		}
		if (n%10==2) {
            up_text(0,filename);   //ȭ���� ���۹޾� filename�� �����Ѵ�.
		}
        if (n%10==3) sz_file(filename);   //ȭ������
		if (n%10==4) {
			if (is_file(filename)) {
				printf(" %s ȭ���� �����մϴ�.\n",filename);
				if (yesno("������ �ұ��? (Y:����)",Yes)) {
                    printf("\n %s ȭ���� �����߽��ϴ�.\n",filename);
					unlink(filename);
				}
			}
			else {
                printf("\n %s ȭ���� �����ϴ�.\n",filename);
                pressenter();
			}
		}
	}
}

