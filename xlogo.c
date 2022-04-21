////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ȭ�� ���հ���
///////////////////////////////////////////////////////////////////////////

#include "bbs.h"
#include <dirent.h>

typedef struct {
    char mode;             //�ִ°�?
    char filename[13];
    char title[41];
    int  filesize;
    time_t date;
} XFRM;
XFRM xfrm;
#define sizeof_xfrm (sizeof(XFRM))
XFRM afrm[3000];

char fdir[300];
char ndir[300];
char ntit[300];
int total;
int total_size;
int re_load;
char datafile[500];
char afilename[10][300];

main(argc, argv)
int argc;
char *argv[];
{
    int  i,n;
    char buff[300];
    char buff2[300];
    char abuff[30][100];
    char adir[30][20];
    char atit[30][40];
    char tcode[9];
    char ccode[9];
    char now_id[9];

	findport(tty); //������Ʈ ã��
	read_cfg();
    ;
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
    set_nowmenu();       //����Ÿȭ�������ϱ�

    strcpy(ccode,argv[1]);              //��ȣȸ �ڵ�
    strcpy(tcode,argv[2]);              //��ȣȸ �ڵ�
    strcpy(now_id,argv[3]);             //���̵�

    if (!find_club(ccode)) {
		mess("%s ��ȣȸ �˻� ����",ccode);
		return;
	}
	read_ccfg();
    read_pf(now_id);

    strcpy(buff,"");
    if (club_mode) sprintf(buff,"%s/",club_go.dir);
    sprintf(afilename[0],"%slogo/%s.mnu",buff,tcode);
    sprintf(afilename[1],"%slogo/%s.mn2",buff,tcode);
    sprintf(afilename[2],"%slogo/%s.mn3",buff,tcode);
    sprintf(afilename[3],"%slogo/%s.mn4",buff,tcode);
    sprintf(afilename[4],"%slogo/%s.mn5",buff,tcode);

    //����Ÿ �����о���� -> ����ũ��.������¥Ȯ��
    while (1) {
        cls();
        printf(" [�޴� ȭ�� ����]       %s(%s) %s.mnu\n",club_go.code,club_go.dir,tcode);
        for (i=0;i<5;i++) {
            printf(" %d. %s %6d Byte\n",i+1,afilename[i],file_size(afilename[i]));
        }
        printf("  ����(E) ����(D) ��ü(CH) ����(CP) ���(UP) �ٿ�(DN) \n");
        printf("  ����>>");
        lineinput(buff,20);check_han(buff);printf(buff);check_cmd(buff);
        if      (is_same(mmc[0],"e"))                      edit_logo();
        else if (is_same(mmc[0],"d"))                      delete_logo();
        else if (is_same(mmc[0],"ch"))                     change_logo();
        else if (is_same(mmc[0],"cp"))                     copy_logo();
        else if (is_same(mmc[0],"up"))                     upload_logo();
        else if (is_same(mmc[0],"dn"))                     dnload_logo();
        else if (mmn[0]>0)                                 disp_logo(mmn[0]);
        else if (strlen(buff)<1);
        else break;
    }
}


edit_logo()
{
    if (mmn[1]<1||mmn[1]>5) mmn[1]=1;
    xedit(afilename[mmn[1]-1]);
}


disp_logo(int n)
{
    if (n<1||n>5) n=1;
    view(afilename[n-1]);
    pressenter();
}


delete_logo()
{
    if (mmn[1]<1||mmn[1]>5) return;
    view(afilename[mmn[1]-1]);
    if (yesno("\n �� ������ �����ұ��? (y/N)>>",No)) {
        unlink(afilename[mmn[1]-1]);
    }
}


change_logo()
{
    if (mmn[1]<1||mmn[1]>5) return;
    if (mmn[2]<1||mmn[2]>5) return;
    printf(" %s,%s ������ ���� ��ü�ұ��? (Y/n)>>",afilename[mmn[1]-1],afilename[mmn[2]-1]);
    if (yesno("",Yes)) {
        unlink(TMP_FILE);
        view(afilename[mmn[1]-1]);
        file_move(afilename[mmn[1]-1],TMP_FILE);
        file_move(afilename[mmn[2]-1],afilename[mmn[1]-1]);
        file_move(TMP_FILE,afilename[mmn[2]-1]);
    }
}


copy_logo()
{
    if (mmn[1]<1||mmn[1]>5) return;
    if (mmn[2]<1||mmn[2]>5) return;
    if (!view(afilename[mmn[1]-1])) return;
    printf("\n %s ������ %s �� �����ұ��? (Y/n)>>",afilename[mmn[1]-1],afilename[mmn[2]-1]);
    if (yesno("",Yes)) {
        file_copy(afilename[mmn[1]-1],afilename[mmn[2]-1]);
    }
}


upload_logo()
{
    char filename[300];
    char filename2[300];
    int key;
    if (mmn[1]<1||mmn[1]>5) return;
    key=up_file(0,filename,No);   //ȭ���� ���۹޾� filename�� �����Ѵ�.
    if (key&&is_file(filename)) {
        view(filename);
        printf("\n �� ������ %s �� �����ұ��? (Y/n)>>",afilename[mmn[1]-1]);
        if (yesno("",Yes)) {
            file_copy(filename,afilename[mmn[1]-1]);
        }
        sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
    }
}


dnload_logo()
{
    if (mmn[1]<1||mmn[1]>5) return;
    sz_file(afilename[mmn[1]-1]);
}
