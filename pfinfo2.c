////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���� : �û�޴� ���� ��ƾ
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

char idfile[300];
//char atit[10][30];
//char afile[10][300];
char atit[][30]={"ȸ������","���Խ�û","���Ժ���","���԰ź�","�������","ȸ��Ż��","ȸ������","ȸ����ü"};
char afile[][300]={ID_FILE,GUEST_FILE,GUESTWAI_FILE,GUESTNOT_FILE,GUESTCAN_FILE,GUESTOUT_FILE,GUESTDEL_FILE,GUESTDEP_FILE};
int nn,top,dsp;
int tot[10];
int allrec;
char nowid[10];
struct termio term_old;

main(int argc, char **argv)
{
    FILE *fp;
    int i,i2,n,k;
    char buff[200];
    int  arec[100];
    int sysmode=No;
    PF  apf[100];
    PF oldpf;
    findport(tty);
    read_cfg();
    set_tmpfile();
    read_ajob();      //������ �о��
    strcpy(nowid,argv[1]);
    if (is_same(nowid,"root")) sysmode=Yes;
	ioctl(0, TCGETA, &term_old);
	rawmode();	// �� ĳ���Į ���
    if (argc>1) strcpy(buff,argv[2]);
    else {
        printf("\n ������ ID, ����Ǵ� �ֹε�Ϲ�ȣ�Ϻκ��� �����ϼ��� >>");
        keyinput2(buff,8);
    }
    if (strlen(buff)<2) goto all_quit;
    if ((fp=fopen(ID_FILE,"r"))==NULL) goto all_quit;
    n=i=0;
    while(n<100&&fread(&pf,sizeof_pf,1,fp)) {
        if (stristr(pf.id,buff)||stristr(pf.name,buff)||
            stristr(pf.id_no,buff)||stristr(pf.offi,buff)||
            stristr(pf.tele,buff)||stristr(pf.addr,buff)) {
            pf.num=i;
            apf[n++]=pf;
        }
        i++;
    }
    //�Ѽ� �ľ�
    fseek(fp,1,SEEK_END);
    allrec=ftell(fp)/sizeof_pf;
    fclose(fp);
    if (n<1) {
        mess("%s �ڷḦ ã���� �����ϴ�.",buff);
        goto all_quit;
    }
    if (n>1) {
        printf("\n        < ������ ȸ���߿��� �����ϼ��� > \n");
        for (i=0;i<n;i++) {
            printf(" %3d. %-8s (%-8s) %14s %-30.30s\n",i+1,apf[i].id,apf[i].name,apf[i].id_no,apf[i].addr);
            if (i%20==19) getchar3();
        }
        printf(" ���� >>");
        lineinput(buff,9);
        i=atoi(buff);
        if (i<1||i>n) goto all_quit;
        apf[0]=apf[i-1];
    }
    pf=apf[0];
    oldpf=pf;

re_start:
    pf=oldpf;
    cls();
    disp_pf9();
    strcpy(buff,pf.id);
    xy(15,3);k=keyinput2(buff, 8);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
    if (strlen(buff)>0) strcpy(pf.id,buff);
    strcpy(buff,pf.name);
    xy(15,4);k=keyinput2(buff, 8);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
    if (strlen(buff)>0) strcpy(pf.name,buff);
    //�û��� �����Ұ����ϰ�
    if (sysmode||pf.type<18) {
        strcpy(buff,pf.passwd);
        xy(15,5);k=keyinput2(buff, 8);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
        if (strlen(buff)>0) strcpy(pf.passwd,buff);
        sprintf(buff,"%d",pf.type);
        xy(15,6);k=keyinput2(buff, 3);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
        if (strlen(buff)>0) pf.type=atoi(buff);
        sprintf(buff,"%d",pf.level);
        xy(15,7);k=keyinput2(buff, 8);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
        if (strlen(buff)>0) pf.level=atoi(buff);
    }
    disp_pf9();
    sprintf(buff,"%d",pf.down);
    xy(15,8);k=keyinput2(buff,12);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
    if (strlen(buff)>0) pf.down=atoi(buff);
    sprintf(buff,"%d",pf.cash);
    xy(15,9);k=keyinput2(buff,12);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
    if (strlen(buff)>0) pf.cash=atoi(buff);
    sprintf(buff,"%s",pf.addr);
    xy(15,13);k=keyinput2(buff,70);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
    if (strlen(buff)>0) strcpy(pf.addr,buff);
    sprintf(buff,"%s",pf.tele);
    xy(15,14);k=keyinput2(buff,20);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
    if (strlen(buff)>0) strcpy(pf.tele,buff);
    sprintf(buff,"%s",pf.bibi);
    xy(15,15);k=keyinput2(buff,14);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
    if (strlen(buff)>0) strcpy(pf.bibi,buff);
    sprintf(buff,"%s",pf.hand);
    xy(15,16);k=keyinput2(buff,14);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
    if (strlen(buff)>0) strcpy(pf.hand,buff);
    sprintf(buff,"%s",pf.id_no);
    xy(15,17);k=keyinput2(buff,14);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
    if (strlen(buff)>0) strcpy(pf.id_no,buff);

save_ok:
    cls();
    disp_pf9();
    printf(" �̴�� �����ұ��?  ����(Y) ���(q) ���Է�(r) >>");
    keyinput2(buff,10);check_han(buff);printf("\n");
    if (is_same(buff,"q")) goto all_quit;
    if (is_same(buff,"r")) goto re_start;
    if (pf.num>=0&&(fp=fopen(ID_FILE,"r+"))!=NULL) {
        fseek(fp,pf.num*sizeof_pf,SEEK_SET);
//      fwrite(&pf,sizeof_pf,1,fp);
        gwrite(187,pf,fp);
        fclose(fp);
    }
    goto all_quit;

save_no:
    cls();
    pf=oldpf;
    disp_pf9();
    printf(" ������� �����Ǿ����ϴ�. [Enter]");getchar3();
    goto all_quit;

all_quit:
	ioctl(0, TCSETA, &term_old);
    printf("\n");
    return;
}

disp_pf9()
{
    cls();
    printf("[0;1m << %s(%s)���� �Ż� ���� ȸ����ȣ : %d / %d >>\n",pf.id,pf.name,pf.num+1,allrec);
    printf("��������������������������������������������������������������������\n");
    printf(" 1. ID      : %-8s\n",pf.id);
    printf(" 2. ����    : %-8s\n",pf.name);
    printf(" 3. ��ȣ    : %-8s\n",pf.type>=18?starlen(pf.passwd):pf.passwd);
    printf(" 4. ���    : %d\n",pf.type);
    printf(" 5. ����    : %d\n",pf.level);
    printf(" 6. �ٿ�뷮: %d\n",pf.down);
    printf(" 7. ���̹�M : %d\n",pf.cash);
    printf("11. �ּ�    : %s\n",pf.addr);
    printf("12. ��ȭ    : %s\n",pf.tele);
    printf("13. �߻�    : %s\n",pf.bibi);
    printf("14. �ڵ���  : %s\n",pf.hand);
    printf("15. �ֹε��: %s\n",pf.id_no);
    printf("                \n");
    printf("��������������������������������������������������������������������\n");
    xy(46,3);printf("�����Ͻ� : %s\n",datestr(2,pf.inday));
    xy(46,4);printf("�������� : %s\n",datestr(2,pf.logout));
    xy(46,5);printf("���ð� : %s\n",datestr(21,pf.all_time));
    xy(46,6);printf("�����Ӽ� : %d\n",pf.all_log);
    xy(46,7);printf("�����   : %s\n",datestr(21,pf.month_time));
    xy(46,8);printf("�����Ӽ� : %d\n",pf.month_log);
    xy(1,18);
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


keyinput2(char *buff,int len)
{
    lineinput(buff,len);
    return 0;
}
