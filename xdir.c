#include "bbs.h"

/*
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <termio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
*/


// �õ� FTP �ڷ��
typedef struct {			// �Խù� ����
	char mode;				//���丮�ΰ�? ȭ���ΰ�?
    char filename[21];      // �ۼ��ھ��̵�
	int  filesize;			// �ۼ����̸�
	time_t date;			// �������
	int    inode;			// i-����ȣ
} XFTP;
XFTP xftp;

//����ü ũ�⸦ ��� �������� ����
#define sizeof_xftp (sizeof(XFTP))

// xdir [���̵�] [���] [���۵��丮] [���]
// ��� 10111 : �ٿ�/��/�̵�/����/�游��� ����

main () {
    DIR *dp;
	struct dirent *fp;
	struct stat fs;
	XFTP aftp[3002];
    int i,total,i1,i2,top,nn;
	char dir_name[100];
	char filename[100];
	char buff[100];
    int re_dir;
	int dir_mode;
    int key;
    char


    strcpy(dir_name,"/bbs");

    chdir(dir_name);
    re_dir=Yes;        //���丮 ���� ����
    key=Yes;           //�ߴܵǸ� No
    total=0;
    while (key) {

        if (re_dir) {
            total=0;
            for (i2=3000;i2>=0;i2--) strcpy(aftp[i2].filename,"\255");
            if((dp= opendir("./")) == NULL) return;
            i=0;
            while((fp= readdir(dp)) != NULL&&i<3000) {          //���丮 ��� ���
                if(!strcmp(fp->d_name,".")) continue;
                sprintf(filename,"%s/%s",dir_name,fp->d_name);
                stat(fp->d_name, &fs);
                dir_mode=0;
                if      (S_ISREG(fs.st_mode))  dir_mode=1;
                else if (S_ISDIR(fs.st_mode))  dir_mode=2;
                else                           dir_mode=0;


                sprintf(buff,"%-20.20s",fp->d_name);buff[20]=0;del_space(buff);
                sprintf(filename,"%-20.20s",buff);
                for (i2=3000;i2>=0;i2--) {
                    if (strcasecmp(fp->d_name,aftp[i2].filename)>0) break;
                    aftp[i2+1]=aftp[i2];
                    strcpy(aftp[i2].filename,fp->d_name);
                    aftp[i2].mode=dir_mode;
                    aftp[i2].filesize=fs.st_size;
                }
                i++;
            }
            total=i;
            closedir(dp);
            re_dir=No;
        }
        top=0;
        while (1) {
            if (top>total) top-=15*3;
            if (top<0) top=0;

            cls();
        //          1234.12345678.12312345678  1234.12345678.123 1234567  1234.12345678.123 1234567
            printf("\n");
            printf(" ��ȣ  ȭ �� ��      ũ��   ��ȣ  ȭ �� ��      ũ��   ��ȣ  ȭ �� ��      ũ�� \n");
            printf("��������������������������������������������������������������������������������\n");
            i1=0;
            for (i=top;i<total;i++) {
                xy(1+(i1/15)*27,4+(i1%15));
                if      (aftp[i].mode==2) strcpy(buff,"  [DIR]");
                else if (aftp[i].filesize<99999999) sprintf(buff,"%8d",aftp[i].filesize);
                else                                sprintf(buff,"%8.2dK",aftp[i].filesize/1000);
                printf("%4d.%-12.12s%8.8s",i+1,aftp[i].filename,buff);
                i1++;
                if (i1==15*3) break;
            }
            xy(1,20);
            printf("��������������������������������������������������������������������������������\n");
            printf(" �̵�:��ȣ �ٿ�(DN) ����(H) ����(P) ����/����(A/N) ����(.) >>");
            lineinput(buff,30);
            nn=atoi(buff);
            if      (is_same(buff,"q")) {key=No;break;}
            else if (is_same(buff,"p")) {key=No;break;}
            else if (is_same(buff,"x")) {key=No;break;}
            else if (is_same(buff,"n")) top+=15*3;
            else if (is_same(buff,"f")) top+=15*3;
            else if (is_same(buff,"a")) top-=15*3;
            else if (is_same(buff,"b")) top-=15*3;
            else if (is_same(buff,".")||is_same(buff,"..")) {
                chdir("..");
                re_dir=Yes;
                break;
            }
            else if (nn>0&&nn<=total&&aftp[nn-1].mode==2) {    //���丮����
                chdir(aftp[nn-1].filename);
                re_dir=Yes;
                break;
            }
            else if (nn>0&&nn<=total&&aftp[nn-1].mode==1) {    //ȭ�ϼ���
                re_dir=Yes;
                break;
            }
        }
        printf("\r\n");
    }


}

