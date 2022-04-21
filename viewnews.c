//뉴스를 보여주는 루틴
//					  view_news(n,0,i2,1);
#include "util.c"  //기본관련 유틸

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp,*fp2;
	BBS bbs2;
	char title[100][71];
	char filename1[100];
	char filename2[100];
    char buff2[30];
    int n,i,n2,i2,i3,mode,mode2,ndate,line,poem;
	time_t t;

	if (argc<5) {
		printf("\n사용법 확인\n");
		return;
	}
	srand(time(NULL));			//난수의 초기화
    poem=atoi(argv[1]);
    mode=atoi(argv[2]);
    ndate=atoi(argv[3]);
    line=atoi(argv[4]);
    mode2=atoi(argv[5]);

    findport(tty);
	read_cfg();
	set_tmpfile();

    strcpy(buff2,poem?"poem":"news");
    sprintf(filename1,"menu/%s_%d.tit",buff2,mode);
    sprintf(filename2,"menu/%s_%d.txt",buff2,mode);
	if (!is_file(filename1)) {
		sprintf(filename1,"forum/%s/menu/%s_%d.tit",buff2,buff2,mode);
		sprintf(filename2,"forum/%s/menu/%s_%d.txt",buff2,buff2,mode);
		if (!is_file(filename1)) {
			sprintf(filename1,"menu/%s_%d.tit",buff2,mode);
			sprintf(filename2,"menu/%s_%d.txt",buff2,mode);
			if (!is_file(filename1)) {
				sprintf(filename1,"%s/%s_%d.tit",buff2,buff2,mode);
				sprintf(filename2,"%s/%s_%d.txt",buff2,buff2,mode);
			}
		}
	}
    fp=fopen(filename1,"r+");
	if (fp==NULL) return;
	fseekend(fp);
	n=ftell(fp)/sizeof_bbs;
	if (n<1) {
		fclose(fp);
		return;
	}
	if (ndate<=0) { 										 //날짜 무시
		i3=0;
        while (i3++<50) {                                    //30반복
			if (n<100) n2=rand() % n;						 //뉴스 번호 임의 선택
			else	   n2=n-(rand()%100);
			if (n2<0) continue;
			if (n2>=n) continue;
			fseek(fp,n2*sizeof_bbs,SEEK_SET);
			if (!fread(&bbs2,sizeof_bbs,1,fp)) continue;
			if (bbs2.del) continue;
//			  printf("line=%d rec=%d total=%d filename2=%s",line,n2,n,filename2);getchar();
			if (line==0) printf("%s",bbs2.title);            //제목출력
            else         view_news2(mode2,bbs2.posi,bbs2.size,line,filename2,bbs2.title);    //내용출력
			break;
		}
	}
	else {
		//시작 날짜 찾기 -> n2
		n2=n;
		fseek(fp,(n2-1)*sizeof_bbs,SEEK_SET);
		time(&t);
		while (n2-->=0) {
			fseek(fp,(n2-1)*sizeof_bbs,SEEK_SET);
			if (!fread(&bbs2,sizeof_bbs,1,fp)) break;
			if ((t-bbs2.date)>ndate*24*60*60) break;
		}
		if (n2>=0) {
			i3=0;
            while (i3++<50) {                                    //30반복
				if (n2<0) n2=0;
				if (n<=n2) n2=n-1;
				i2=n-rand()%(n-n2);
				if (i2<0||i2>n) i2=n;
				fseek(fp,(i2-1)*sizeof_bbs,SEEK_SET);
				if (!fread(&bbs2,sizeof_bbs,1,fp)) continue;
				if (bbs2.del) continue;
				if (line==0) printf("%s",bbs2.title);            //제목출력
                else         view_news2(mode2,bbs2.posi,bbs2.size,line,filename2,bbs2.title);    //내용출력
				break;
			}
		}
	}
	if (fp!=NULL) fclose(fp);
}


view_news2(int mode2,int posi,int size,int line,char *filename2,char *title)
{
    FILE *fp1;
    FILE *fp2;
	int count,i;
    char bufftit[300];
    char TMP_FILE5[200];
	char c;

    sprintf(TMP_FILE5,"tmp/tmp5%d%s",cfg.hostno,no_spc(tty)); //호스트번호별임시화일
    fp1=fopen(TMP_FILE5,"w+");
    fp2=fopen(filename2,"r+");
    if (fp1==NULL||fp2==NULL) return;
    strcpy(bufftit,title);
    if (mode2==2) sprintf(bufftit,"【 %s 】",title);
    if (mode2==3) sprintf(bufftit,"### %s ###",title);
    if (mode2!=5) {
        if (mode2==4) fprintf(fp1,"%s\n",bufftit);
        else          fprintf(fp1,"%s\n",center_line(bufftit));
    }
	fseek(fp2,posi,SEEK_SET);
	count=0;
	for (i=0;i<size;i++) {
		if((c = fgetc(fp2)) == '\n') count++;
        if(count>line) break;
        fputc(c,fp1);
	}
    fclose(fp1);
    fclose(fp2);
    view(TMP_FILE5);
    unlink(TMP_FILE5);
}
