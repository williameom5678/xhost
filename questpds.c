////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���� : �ڷ�� ����Է�
#include "bbs.h"

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
    int n,i;
	char buff[200];
	char filename[200];
    char datafile[200];
    char outfile[200];
    char ans[10][20];
    char abuff[10][200];

    strcpy(datafile,argv[1]);
    strcpy(outfile,argv[2]);
    for (i=0;i<9;i++) strcpy(ans[i],"");
    for (i=0;i<9;i++) strcpy(abuff[i],"");

    restart1:
    printf("\n OS ������ ������ �ֽʽÿ�.\n");
    printf(" 1.WINDOWS  2.WIN-NT  3.DOS  4.UNIX  5.MAC  6.OS/2  0.���\n");
    printf(" ��ȣ>>");lineinput(buff,5);printf("\n\n");n=atoi(buff);
         if (n==1) strcpy(ans[0],"WINDOWS");
    else if (n==2) strcpy(ans[0],"WIN-NT");
    else if (n==3) strcpy(ans[0],"DOS");
    else if (n==4) strcpy(ans[0],"UNIX");
    else if (n==5) strcpy(ans[0],"MAC");
    else if (n==6) strcpy(ans[0],"OS/2");
    else     goto quit1;
    printf(" PC ȯ���� ������ �ֽʽÿ�.\n");
    printf(" 1.Pentium�̻�  2.486�̻�  3.386�̻�  4.��Ÿ  0.���\n");
    printf(" ��ȣ>>");lineinput(buff,5);printf("\n\n");n=atoi(buff);
         if (n==1) strcpy(ans[1],"Pentium�̻�");
    else if (n==2) strcpy(ans[1],"486�̻�");
    else if (n==3) strcpy(ans[1],"386�̻�");
    else if (n==4) strcpy(ans[1],"��Ÿ");
    else     goto quit1;
    printf(" ���డ���� �޸�(RAM) �뷮�� ������ �ֽʽÿ�.\n");
    printf(" 1. 8MB  2. 16MB  3. 32MB  4.��Ÿ  0.���\n");
    printf(" ��ȣ>>");lineinput(buff,5);printf("\n\n");n=atoi(buff);
         if (n==1) strcpy(ans[2],"8MB");
    else if (n==2) strcpy(ans[2],"16MB");
    else if (n==3) strcpy(ans[2],"32MB");
    else if (n==4) strcpy(ans[2],"��Ÿ");
    else     goto quit1;
    printf(" ���� ���θ� ������ �ֽʽÿ�.\n");
    printf(" 1.����  2.�������  3.�������  4.��Ÿ  0.���\n");
    printf(" ��ȣ>>");lineinput(buff,5);printf("\n");n=atoi(buff);
         if (n==1) strcpy(ans[3],"����");
    else if (n==2) strcpy(ans[3],"�������");
    else if (n==3) strcpy(ans[3],"�������");
    else if (n==4) strcpy(ans[3],"��Ÿ");
    else     goto quit1;
// ������������������������������������������������������������������
// ��    OS ȯ�� : WINDOWS               PC  ȯ�� : Pentium�̻�    ��
// ��    RAM     : 8MB                   �������� : ����           ��
// ������������������������������������������������������������������


    sprintf(abuff[0],"  ������������������������������������������������������������������");
    sprintf(abuff[1],"  ��   [7m OS ȯ�� : %-20s  PC  ȯ�� : %-12s[m   ��",ans[0],ans[1]);
    sprintf(abuff[2],"  ��   [7m RAM     : %-20s  �������� : %-12s[m   ��",ans[2],ans[3]);
    sprintf(abuff[3],"  ������������������������������������������������������������������");
    for (i=0;i<4;i++) {
        printf("%s\n",abuff[i]);
    }
    printf(" �Է��� ������ �½��ϱ�? �����Ͻ÷��� 'n'�� �Է��ϼ���.(Y/n)");
    if (yesno2("",Yes)==No) goto restart1;
    if ((fp=fopen(outfile,"w"))!=NULL) {
        for (i=0;i<4;i++) {
            fprintf(fp,"%s\n",abuff[i]);
        }
        fclose(fp);
        exit(1);
    }
    quit1:
    exit(-1);
}


