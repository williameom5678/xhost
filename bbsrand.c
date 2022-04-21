////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 게시판 담당자의 메뉴
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"

main(argc, argv)
int argc;
char *argv[];
{
    char ccode[100];
    char tcode[100];
    char tid[100];
    int mode;
    strcpy(ccode,argv[1]);              //동호회 코드
    strcpy(tcode,argv[2]);              //가기코드
    strcpy(tid  ,argv[3]);              //가기코드
    mode=atoi(argv[4]);                 //가기코드

	srand(time(NULL));		//난수의 초기화
    findport(tty); //접속포트 찾기
    read_cfg();
	club_mode=find_club(ccode);
    set_nowmenu();
    if (!find_go(tcode)) return;    //찾는 가기코드가 없다
    read_pf(tid);

	sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
	sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,go.code);
    set_tmpfile();
    unlink(TMP_FILE);
    if (mode==0) view_bbs_random(1,25,100,TMP_FILE);
    if (mode==1) view_bbs_random(1,25,999,TMP_FILE);
    if (mode==2) view_bbs_random(3,15,100,TMP_FILE);
    if (mode==3) view_bbs_random(3,15,999,TMP_FILE);
}

//도움말을 보여주는 루틴 (최근 num개중에서)
view_bbs_random(int s1,int s2,int num,char *outfile)
{
    FILE *fp,*fp2;
	BBS bbs2;
	char c,buff[100];
	char filename[100];
    int n,i,i2,n1,n2,count,found,count2;

    unlink(outfile);
    sprintf(filename,"%s/menu/%s.tit",club_go.dir,go.code);
	if (!is_file(filename)) return;
	fp=fopen(filename,"r+");
	if (fp==NULL) return;
	fseekend(fp);
	n=ftell(fp)/sizeof_bbs;
    if (n<1||num<1) {
        fclose(fp);
        return;
    }
    if (n<num) num=n;
	bbs2.num=0;
	i2=0;
    found=No;
	while (i2++<100) {
		n1=rand()%(num+1);
		n2=n-n1;
		if (n2<0||n2>n) continue;
		fseek(fp,n2*sizeof_bbs,SEEK_SET);
		if (fread(&bbs2,sizeof_bbs,1,fp)<1||bbs2.del==Yes) continue;
        found=Yes;
		break;
	}
	fclose(fp);
    if (!found) {unlink(outfile);return;}
    sprintf(filename,"%s/menu/%s.txt",club_go.dir,go.code);
	if (bbs2.num<1||bbs2.size<3||!is_file(filename)) return;
	fp=fopen(filename,"r+");
    fp2=fopen(outfile,"w");
	if (fp==NULL) return;
	fseek(fp,bbs2.posi,SEEK_SET);
    count=s1;
    count2=0;
    for (i=0;i<bbs2.size&&i<3000;i++) {
        if((c = fgetc(fp)) == '\n') count++;
        if(c==0) break;
        if(count>s2) break;
        fputc(c,fp2);
        count2++;
    }
	fclose(fp);
    fclose(fp2);
    if (count2>0) {
        cls();
        view("logo/_bbshelp.scr");
        view(outfile);
    }

}    
