#include "bbs.h"

#ifndef CD
// 시디롬 화일 목록
typedef struct {
	int num;				// 일련 번호
    char dir[150];          // 디렉토리
    char filename[50];      // 화일명
    int  filesize;          // 화일의 크기
} CD;
#endif

CD cd;


main(argc, argv)
int argc;
char *argv[];
{
    char buff[300];
    char buff2[300];
    char dir[300];
    char dirmenu[300];
    char dirfile[300];
    char NOW_FILE2[300];
    char NOW_FILED2[300];
    char OLDDIR[400];
    FILE *fp1,*fp2,*fp3,*fp4,*fp5;
    int key,n2;


    if (argc<6) {
        printf("\n사용법  cd2bbs <CD 디렉토리> <게시판디렉토리> <게시판화일명> <아이디> <이름>\n");
        printf("\n사용법  cd2bbs /cdrom /xhost/menu hello 시삽 운영자\n");
		getchar();
		return;
	}


    strcpy(dir,argv[1]);              //디렉토리
    strcpy(dirmenu,argv[2]);          //게시판 디렉토리
    strcpy(dirfile,argv[3]);          //게시판 화일명
    strcpy(pf.id,argv[4]);            //아이디
    strcpy(pf.name,argv[5]);          //이름
    getcwd(OLDDIR,sizeof(OLDDIR));    //현재의 디렉토리

    sprintf(TMP_FILE  ,"%s/%s/TMP__1",OLDDIR,dirmenu);
    sprintf(NOW_FILE  ,"%s/%s/%s.cd" ,OLDDIR,dirmenu,dirfile);
    sprintf(NOW_FILE2 ,"%s/%s/%s.tit",OLDDIR,dirmenu,dirfile);
    sprintf(NOW_FILED2,"%s/%s/%s.txt",OLDDIR,dirmenu,dirfile);

    printf("\n *****       잠깐만 기다려 주십시요..       *****\n");
    printf(" ***** 지금 %s를 읽고 있습니다. *****\n",dir);
    if (!is_file(dir)) return;
    sprintf(buff,"bin/cd2bbs.sh %s %s",dir,TMP_FILE);   //현재의 디렉토리의 전체 용량
    system(buff);

    fp1=fopen(TMP_FILE,"r+");
    if (fp1==NULL) return;
    fp3=fopen(NOW_FILE,"w");
    fp4=fopen(NOW_FILE2,"w");
    fp5=fopen(NOW_FILED2,"w");
    n2=1;
    bbs.size=0;
    bbs.posi=0;
    while (fgets(buff,200,fp1)!=NULL) {
        buff2dir(buff);                  //buff->cd로 변환
        if (cd.filesize>0&&cd.filename[0]!=0) {
            cd.num=n2++;
            fwrite(&cd,sizeof(cd),1,fp3);   //저장
            make_good_filename(bbs.filename,cd.filename);
            sprintf(buff,"%s%s",cd.dir,cd.filename);
            sprintf(bbs.title,"%-59.59s",buff);
            bbs.num=cd.num;
            strcpy(bbs.id,pf.id);
            strcpy(bbs.name,pf.name);
            strcpy(bbs.key,"");
            time(&bbs.date);
            bbs.gd=' ';
            bbs.read=0;
            bbs.filesize=cd.filesize;
            sprintf(buff,"디렉토리: %s\n화 일 명: %s\n",cd.dir,cd.filename);
            bbs.size=(int)strlen(buff);
            fseek(fp4,(bbs.num-1)*sizeof_bbs,SEEK_SET);
            fwrite(&bbs,sizeof_bbs,1,fp4);  //제목저장
            fseek(fp5,bbs.posi,SEEK_SET);
            fwrite(&buff,bbs.size,1,fp5);   //내용저장
            bbs.posi+=bbs.size;
            printf("디렉토리: %s 화일: %s\n",cd.dir,cd.filename);
        }
    }
    fclose(fp1);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    unlink(TMP_FILE);
}


//buff에서 cd로 추출
buff2dir(char *buff)
{
    char buff2[200];
    char buff3[200];
    int i,i2,i3,key;
    char c;

    key=No; //화일명 상태?
    i2=0;
    i3=0;
    strcpy(buff2,"");   //디렉토리
    strcpy(buff3,"");
    strcpy(cd.dir,"");
    strcpy(cd.filename,"");
    cd.filesize=-1;

    for (i=0;i<(int)strlen(buff);i++) {
        c=buff[i];
        if (key==No&&(c==' '||c=='\t')) {                  //화일크기 끝
            cd.filesize=atoi(buff2);
            strcpy(buff2,"");
            i2=0;
            key=Yes;
        }
        else if (key==No&&c!=' '&&c!='\t') {                  //화일크기 끝
            buff2[i2]=c;
            buff2[i2+1]=0;
            i2++;
        }
        else if (key==Yes&&c==' '&&c!='\t') {                 //화일명 대기(빈칸처리)
            continue;
        }
        else if (c=='\n'||c=='\r') {
            strcpy(cd.dir,buff2);
            strcpy(cd.filename,buff3);
            break;
        }
        else if (c=='/') {          //디렉토리 상태일때
            strcat(buff2,buff3);            //화일명에 있는것
            i2=(int)strlen(buff2);          //디렉토리로 보냄;
            buff2[i2]=c;
            buff2[i2+1]=0;
            strcpy(buff3,"");
            i3=0;
        }
        else {                              //그외 화일명 처리
            buff3[i3]=c;
            buff3[i3+1]=0;
            i3++;
        }
    }
}


is_file(char *filename)
{
	return (access(filename,0)!=-1);
}


//buff2의 화일명 -> buff1으로
make_good_filename(char *buff1,char *buff2)
{
    int i;
    byte c;
    int key;
    char buff3[200];

    //우선 화일명에서 두개이상의 . 짜르기
    key=Yes;
    for (i=0;i<(int)strlen(buff2);i++) {
        c=buff2[i];
        if (key==Yes&&c=='.') key=No;
        else if (key=='.') c='_';
        buff1[i]=c;
        buff1[i+1]=0;
    }

    strcpy(buff3,buff1);
    //우선 화일명에서 특수문자 짜르기
    key=Yes;
    for (i=0;i<(int)strlen(buff3);i++) {
        c=buff3[i];
        if (c==63||c==47||c==59||c==42) c='_';
        buff1[i]=c;
        buff1[i+1]=0;
    }

    //우선 화일명이 13자보다 큰가?
    buff1[12]=0;
}
