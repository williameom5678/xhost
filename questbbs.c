////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 내용 : 자료실 등록입력
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
    printf(" 게시물 종류선택 \n");
    printf(" 1.WINDOWS  2.WIN-NT  3.DOS  4.UNIX  5.MAC  6.OS/2  0.취소\n");
    printf(" 번호>>");lineinput(buff,5);printf("\n\n");n=atoi(buff);
         if (n==1) strcpy(ans[0],"WINDOWS");
    else if (n==2) strcpy(ans[0],"WIN-NT");
    else if (n==3) strcpy(ans[0],"DOS");
    else if (n==4) strcpy(ans[0],"UNIX");
    else if (n==5) strcpy(ans[0],"MAC");
    else if (n==6) strcpy(ans[0],"OS/2");
    else     goto quit1;
    printf(" PC 환경을 선택해 주십시오.\n");
    printf(" 1.Pentium이상  2.486이상  3.386이상  4.기타  0.취소\n");
    printf(" 번호>>");lineinput(buff,5);printf("\n\n");n=atoi(buff);
         if (n==1) strcpy(ans[1],"Pentium이상");
    else if (n==2) strcpy(ans[1],"486이상");
    else if (n==3) strcpy(ans[1],"386이상");
    else if (n==4) strcpy(ans[1],"기타");
    else     goto quit1;
    printf(" 실행가능한 메모리(RAM) 용량을 선택해 주십시오.\n");
    printf(" 1. 8MB  2. 16MB  3. 32MB  4.기타  0.취소\n");
    printf(" 번호>>");lineinput(buff,5);printf("\n\n");n=atoi(buff);
         if (n==1) strcpy(ans[2],"8MB");
    else if (n==2) strcpy(ans[2],"16MB");
    else if (n==3) strcpy(ans[2],"32MB");
    else if (n==4) strcpy(ans[2],"기타");
    else     goto quit1;
    printf(" 공개 여부를 선택해 주십시오.\n");
    printf(" 1.공개  2.쉐어웨어  3.데모버젼  4.기타  0.취소\n");
    printf(" 번호>>");lineinput(buff,5);printf("\n");n=atoi(buff);
         if (n==1) strcpy(ans[3],"공개");
    else if (n==2) strcpy(ans[3],"쉐어웨어");
    else if (n==3) strcpy(ans[3],"데모버젼");
    else if (n==4) strcpy(ans[3],"기타");
    else     goto quit1;
// ┌───────────────────────────────┐
// │    OS 환경 : WINDOWS               PC  환경 : Pentium이상    │
// │    RAM     : 8MB                   공개여부 : 공개           │
// └───────────────────────────────┘
    sprintf(abuff[0]," ┌───────────────────────────────┐");
    sprintf(abuff[1]," │[7m    OS 환경 : %-20s  PC  환경 : %-15s[m│",ans[0],ans[1]);
    sprintf(abuff[2]," │[7m    RAM     : %-20s  공개여부 : %-15s[m│",ans[2],ans[3]);
    sprintf(abuff[3]," └───────────────────────────────┘");
    for (i=0;i<4;i++) {
        printf("%s\n",abuff[i]);
    }
    printf("입력한 내용이 맞습니까? 수정하시려면 'n'을 입력하세요.(Y/n)");
    if (yesno2("",Yes)==No) goto restart1;
    if ((fp=fopen(outfile,"w"))!=NULL) {
        for (i=0;i<4;i++) fprintf(fp,"%s\n",abuff[i]);
        fclose(fp);
        exit(1);
    }
    quit1:
    exit(-1);
}


