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


// 시디 FTP 자료실
typedef struct {			// 게시물 정보
	char mode;				//디렉토리인가? 화일인가?
    char filename[21];      // 작성자아이디
	int  filesize;			// 작성자이름
	time_t date;			// 등록일자
	int    inode;			// i-노드번호
} XFTP;
XFTP xftp;

//구조체 크기를 상수 수식으로 선언
#define sizeof_xftp (sizeof(XFTP))

// xdir [아이디] [등급] [시작디렉토리] [모드]
// 모드 10111 : 다운/업/이동/삭제/방만들기 가능

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
    re_dir=Yes;        //디렉토리 새로 읽음
    key=Yes;           //중단되면 No
    total=0;
    while (key) {

        if (re_dir) {
            total=0;
            for (i2=3000;i2>=0;i2--) strcpy(aftp[i2].filename,"\255");
            if((dp= opendir("./")) == NULL) return;
            i=0;
            while((fp= readdir(dp)) != NULL&&i<3000) {          //디렉토리 목록 얻기
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
            printf(" 번호  화 일 명      크기   번호  화 일 명      크기   번호  화 일 명      크기 \n");
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
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
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
            printf(" 이동:번호 다운(DN) 도움(H) 복귀(P) 이전/다음(A/N) 상위(.) >>");
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
            else if (nn>0&&nn<=total&&aftp[nn-1].mode==2) {    //디렉토리선택
                chdir(aftp[nn-1].filename);
                re_dir=Yes;
                break;
            }
            else if (nn>0&&nn<=total&&aftp[nn-1].mode==1) {    //화일선택
                re_dir=Yes;
                break;
            }
        }
        printf("\r\n");
    }


}

