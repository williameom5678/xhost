////////////////////////////////////////////////////////////////////////
//            XHOST 5.3s: �������� BBS HOST                            //
//			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //
//            �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����              //
//            �������� ������ �޾ƾ� �մϴ�.                          //
////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//�˻� ȭ�ϸ� ã��
/////////////////////////////////////////////////////////////////////

#include "util.c"

char bufff[30];
char buffm[50];
char buffn[30];
char buffi[30];
char bufft[30];
char buffd[30];
char buffg1[30];
char buffg2[30];
char dir[100];

main(argc, argv)
int argc;
char *argv[];
{
    int find_mode;

    if (argc<3) {
        printf("\n����  findbbs <mode> <��ȣȣ�ڵ�>\n");
        printf("mode=0: �ڷ�� ã��\n");
        printf("mode=1: �Խ��� ã��\n");
		getchar();
		return;
	}
    find_mode=atoi(argv[1]);
    find_club(argv[2]);
    club_mode=!is_same(club_go.code,"main");
	findport(tty); //������Ʈ ã��
	read_cfg();
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
	set_nowmenu();		 //����Ÿȭ�������ϱ�
    read_ccfg();
    strcpy(dir,club_go.dir);

    if (find_mode==1) find_pds();
    if (find_mode==2) find_bbs();
    unlink(TMP_FILE);
    unlink(TMP_FILE2);
}

find_pds()
{
	FILE *fp;
    char buff[140],buff2[80];
    int i,mode,num;
    mode=0;if (club_mode) mode=1;
    strcpy(bufff,"");
    strcpy(buffi,"");
    strcpy(buffm,"");
    strcpy(buffi,"");
    strcpy(bufft,"");
    strcpy(buffd,"");
    strcpy(buffg1,"");
    strcpy(buffg2,"");

    while (1) {
        cls();
        printfL2();
        printf(" �ڷ���� ȭ���� ã���ϴ�. (������ ������ enter�� ġ�� �˴ϴ�.)  \n");
        printf(" FB:�Խ��ǰ˻� FF:ȭ�ϰ˻�          (�˻���ȣȸ:%s)\n",club_go.code);
        printf(" �޴� ���� : %s \n",NOW_MENU);
        printfL2();
        if (mode==0) strcpy(buffm,"(���� �ڷ��)");
        else         sprintf(buffm,"(��ȣȸ�ڷ�� %s)",club_go.dir);
        printf("      1.   �˻� ȭ �� �� ���� (%-12s)\n",bufff);
        printf("      2.   �˻� �� �� �� ���� (%-12s)\n",buffi);
        printf("      3.   �˻� ��    �� ���� (%-12s)\n",buffn);
        printf("      4.   �˻� ������ ���� (%-12s)\n",bufft);
        printf("      5.   �˻� ��¥���� ���� (%-12s)\n",buffd);
        printf("      6.   ���� / ��ȣȸ ���� %-30s\n",buffm);
        printf("      7.   �����ڵ� ���� ���� (%-12s)\n",buffg1);
        printf("      8.   �����ڵ� ���� ���� (%-12s)\n",buffg2);
        printf("      9.   �ڷ� ã�� ����\n");
        printf("  enter:   ���� �������� �˻� ���� (���� ���ǿ� �ش�Ǵ� �ڷ�)\n");
        printfL2();
        printf(" ��ȣ�� �����ϼ��� (q:���) >>");
        lineinput(buff,5);
        printf("\n");
        num=atoi(buff);
        i=0;
        if (is_char("QqPpXx0",buff[0])) return;
        if (num==1) {
            printf(" �˻��� ȭ�ϸ��� �Ϻ� �Է� (��:abc:abc�� ���Ե� ȭ�ϸ�) >>");
            lineinput(bufff,13);
            del_space(bufff);
            del_esc(bufff);
        }
        if (num==2) {
            printf(" �˻��� ���̵��� �Ϻ� �Է� >>");
            lineinput(buffi,12);
            del_space(buffi);
            del_esc(buffi);
        }
        if (num==3) {
            printf(" �˻��� ������ �Ϻ� �Է� >>");
            lineinput(buffn,12);
            del_space(buffn);
            del_esc(buffn);
        }
        if (num==4) {
            printf(" �˻��� ������ �Ϻ� �Է� >>");
            lineinput(bufft,12);
            del_space(bufft);
            del_esc(bufft);
        }
        if (num==5) {
            printf(" �˻��� ��¥ (��:96.12.31->96.12.31���� �ڷ�) >>");
            lineinput(buffd,12);
            del_space(buffd);
            del_esc(buffd);
        }
        if (num==6) {
            if (mode==0) mode=1;
            else         mode=0;
        }
        if (num==7) {
            printf(" �˻��� �ڵ��� �����ڵ忡  ���Թ��� >>");
            lineinput(buffg1,8);del_space(buffg1);del_esc(buffg1);
        }
        if (num==8) {
            printf(" �˻��� �ڵ��� ���� ����  ���Թ��� >>");
            lineinput(buffg2,8);del_space(buffg2);del_esc(buffg2);
        }
        if (num==9) {
            logo("logo/_findpds.log",Yes);
        }
        if (num==0&&buff[0]==0) {
            if (bufff[0]==0&&
                buffi[0]==0&&
                buffn[0]==0&&
                bufft[0]==0&&
                buffd[0]==0&&
                yesno("\n ��ü �ڷḦ �� ã���ðڽ��ϱ�? (y/N) >>",No)==No) continue;
            printf("\n");
            search_file(mode);
        }
    }
}


search_file(int mode)
{
    char afn[1000][9];
    char fn[100];
    char buffsize[50];
    char bufffname[50];
    int found;
    int i,j;
    int afn_num=0;
    FILE *fp;
    FILE *fp2;
	struct tm *tp;
    char buff[100];
    GO tgo;

    for (i=0;i<1000;i++) strcpy(afn[i],"");
    if ((fp=fopen(NOW_MENU,"r"))==NULL) return;

    while(fread(&tgo,sizeof_go,1,fp)) {
        if (strlen(buffg1)>0&&!stristr(tgo.code,buffg1)) continue;
        if (strlen(buffg2)>0&&!stristr(tgo.name,buffg2)) continue;
        if (strlen(tgo.code)>0&&is_char(PDSTYPE,tgo.type)
            &&!is_same(tgo.code,"bbs_all")
            &&!is_same(tgo.code,"pds_all")
            &&!is_char("zdc",tgo.type)) {
            found=No;
            for (j=0;j<afn_num;j++) {
                if (is_same(afn[j],tgo.code)) {
                    found=Yes;
                    break;
                }
            }
            if (!found&&afn_num<999) strcpy(afn[afn_num++],tgo.code);
        }
    }
    findport(tty);      //���� ���ǰ� �ִ� ��Ʈã��
    fp2=fopen(TMP_FILE,"w");
    fprintf(fp2,"�����ڵ� ��ȣ ȭ�ϸ�    ȭ��ũ�� ID       ��  ��\n");
    for (i=0;i<afn_num;i++) {
        if (mode==0) sprintf(fn,"menu/%s.tit",afn[i]);
        else         sprintf(fn,"%s/menu/%s.tit",club_go.dir,afn[i]);
        fp=fopen(fn,"r");
        if (fp==NULL) continue;
        while (fread(&bbs,sizeof_bbs,1,fp)) {
            if ( !bbs.del&&
                 (bufff[0]==0||stristr(bbs.filename,bufff))&&
                 (buffi[0]==0||stristr(bbs.id,buffi))&&
                 (buffn[0]==0||stristr(bbs.name,buffn))&&
                 (bufft[0]==0||stristr(bbs.title,bufft)) ) {
                strcpy(buffsize,byte2str(bbs.filesize));
                file2dot(bufffname,bbs.filename);
                tp = localtime(&bbs.date);
                fprintf(fp2,"(%2d/%2d/%2d) %-8s%5d %s%6s %-8s %-25.25s\n",
                    tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,afn[i],bbs.num,bufffname,buffsize,bbs.id,bbs.title);
            }
        }
        fclose(fp);
    }
    fclose(fp2);
    xsort(1,TMP_FILE,TMP_FILE2);
    view_text(TMP_FILE2);
    unlink(TMP_FILE);
    unlink(TMP_FILE2);
}



find_bbs()
{
	FILE *fp;
    char buff[140],buff2[80];
    int i,mode,num;
    mode=0;if (club_mode) mode=1;

    strcpy(buffi,"");
    strcpy(buffm,"");
    strcpy(buffi,"");
    strcpy(bufft,"");
    strcpy(buffd,"");
    strcpy(buffg1,"");
    strcpy(buffg2,"");

    while (1) {
        cls();
        printfL2();
        printf("(findbbs) �Խ����� �׸��� ã���ϴ�. (������ ������ enter�� ġ�� �˴ϴ�.)  \n");
        printf(" FB:�Խ��ǰ˻� FF:ȭ�ϰ˻�          (�˻���ȣȸ:%s)\n",club_go.code);
        printfL2();
        if (mode==0) strcpy(buffm,"(���� �ڷ��)");
        else         sprintf(buffm,"(��ȣȸ�ڷ�� %s)",club_go.dir);
        printf("      1.   �˻� �� �� �� ���� (%-12s)\n",buffi);
        printf("      2.   �˻� ��    �� ���� (%-12s)\n",buffn);
        printf("      3.   �˻� ������ ���� (%-12s)\n",bufft);
        printf("      4.   �˻� ��¥���� ���� (%-12s)\n",buffd);
        printf("      6.   ���� / ��ȣȸ ���� %-30s\n",buffm);
        printf("      7.   �����ڵ� ���� ���� (%-12s)\n",buffg1);
        printf("      8.   �����ڵ� ���� ���� (%-12s)\n",buffg2);
        printf("  enter:   ���� �������� �˻� ���� (���� ���ǿ� �ش�Ǵ� �ڷ�)\n");
        printfL2();
        printf(" ��ȣ�� �����ϼ��� (q:���) >>");
        lineinput(buff,5);
        printf("\n");
        num=atoi(buff);
        i=0;
        if (is_char("QqPpXx0",buff[0])) return;
        if (num==1) {
            printf(" �˻��� ���̵��� �Ϻ� �Է� >>");
            lineinput(buffi,12);
            del_space(buffi);
            del_esc(buffi);
        }
        if (num==2) {
            printf(" �˻��� ������ �Ϻ� �Է� >>");
            lineinput(buffn,12);
            del_space(buffn);
            del_esc(buffn);
        }
        if (num==3) {
            printf(" �˻��� ������ �Ϻ� �Է� >>");
            lineinput(bufft,12);
            del_space(bufft);
            del_esc(bufft);
        }
        if (num==4) {
            printf(" �˻��� ��¥ (��:96.12.31->96.12.31���� �ڷ�) >>");
            lineinput(buffd,12);
            del_space(buffd);
            del_esc(buffd);
        }
        if (num==6) {
            if (mode==0) mode=1;
            else         mode=0;
        }
        if (num==7) {
            printf(" �˻��� �ڵ��� �����ڵ忡  ���Թ��� >>");
            lineinput(buffg1,8);del_space(buffg1);del_esc(buffg1);
        }
        if (num==8) {
            printf(" �˻��� �ڵ��� ���� ����  ���Թ��� >>");
            lineinput(buffg2,8);del_space(buffg2);del_esc(buffg2);
        }
        if (num==0&&buff[0]==0) {
            if (buffi[0]==0&&
                buffn[0]==0&&
                bufft[0]==0&&
                buffd[0]==0&&
                yesno("\n ��ü �ڷḦ �� ã���ðڽ��ϱ�? (y/N) >>",No)==No) continue;
            printf("\n");
            search_bbs(mode);
        }
    }
}


search_bbs(int mode)
{
    char afn[1000][9];
    char fn[100];
    char bufffname[50];
    int found;
    int i,j;
    int afn_num=0;
    FILE *fp;
    FILE *fp2;
	struct tm *tp;
    char buff[100];
    GO tgo;

    for (i=0;i<1000;i++) strcpy(afn[i],"");
    if ((fp=fopen(NOW_MENU,"r"))==NULL) return;

    while(fread(&tgo,sizeof_go,1,fp)) {
        if (strlen(buffg1)>0&&!stristr(tgo.code,buffg1)) continue;
        if (strlen(buffg2)>0&&!stristr(tgo.name,buffg2)) continue;
        if (strlen(tgo.code)>0&&is_char(BBSTYPE,tgo.type)
            &&!is_same(tgo.code,"bbs_all")
            &&!is_same(tgo.code,"pds_all")
            &&!is_char("zdc",tgo.type)) {
            found=No;
            for (j=0;j<afn_num;j++) {
                if (is_same(afn[j],tgo.code)) {
                    found=Yes;
                    break;
                }
            }
            if (!found&&afn_num<999) strcpy(afn[afn_num++],tgo.code);
        }
    }
    findport(tty);      //���� ���ǰ� �ִ� ��Ʈã��
    sprintf(TMP_FILE,"tmp/tmp1%d%s",cfg.hostno,no_spc(tty)); // �ӽ�ȭ��
    sprintf(TMP_FILE2,"tmp/tmp2%d%s",cfg.hostno,no_spc(tty)); // �ӽ�ȭ��
    fp2=fopen(TMP_FILE,"w");
    fprintf(fp2,"��¥  �����ڵ� ��ȣ ID  �̸�    ��  ��\n");
    for (i=0;i<afn_num;i++) {
        if (mode==0) sprintf(fn,"menu/%s.tit",afn[i]);
        else         sprintf(fn,"%s/menu/%s.tit",club_go.dir,afn[i]);
        fp=fopen(fn,"r+");
        if (fp!=NULL) {
            while (fread(&bbs,sizeof_bbs,1,fp)) {
                if ( !bbs.del&&
                     (buffi[0]==0||stristr(bbs.id,buffi))&&
                     (buffn[0]==0||stristr(bbs.name,buffn))&&
                     (bufft[0]==0||stristr(bbs.title,bufft)) ) {
                    tp = localtime(&bbs.date);
                    fprintf(fp2,"(%2d/%2d/%2d) %-8s%5d %-8s %-40.40s\n",
                        tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,afn[i],bbs.num,bbs.id,bbs.title);
                }
            }
            fclose(fp);
        }
    }
    fclose(fp2);

    xsort(1,TMP_FILE,TMP_FILE2);
    view_text(TMP_FILE2);
}

file2dot(char *buff,const char *filename)
{
	unsigned int i=0;
	int n=0;
	char buff2[50];
	strcpy(buff2,filename);
	del_space(buff2);
	n=strlen(buff2);
		 if (buff2[0]==0	) sprintf(buff,"            ");
	else if (buff2[n-1]=='.') sprintf(buff,"%9s   ",buff2);
	else if (buff2[n-2]=='.') sprintf(buff,"%10s  ",buff2);
	else if (buff2[n-3]=='.') sprintf(buff,"%11s ",buff2);
	else					  sprintf(buff,"%12.12s",buff2);
	return i;
}




