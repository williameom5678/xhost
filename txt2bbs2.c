////////////////////////////////////////////////////////////////////////
//	XHOST 리눅스용 BBS HOST 개발자 : 김성대  (대구:964-1002)
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include "util.c"
main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp1,*fp2,*fp3,*fp4;
	int i,n=0,n1,n2;
	int key=No;
	int now_posi,count;
	char ch,c;
    char filename[500];

    if (argc<3) {
        printf("txt2bbs2  원래파일명(확장자제외) 새로운파일명.out\n");
        return;
    }
    sprintf(NOW_FILE ,"%s.tit",argv[1]);
    sprintf(NOW_FILED,"%s.txt",argv[1]);

	fp1=fopen(NOW_FILE ,"r");
	fp2=fopen(NOW_FILED,"r");
	if (fp1==NULL||fp2==NULL) return;
    sprintf(filename,"%s",argv[2]);
    fp3=fopen(filename,"w+");
    printf("\n..%s..-> %s..화일 추출중.............\n",NOW_FILE,filename);
	now_posi=0;
	count=0;
	while (fread(&bbs,sizeof_bbs,1,fp1)){
		if (bbs.del)		continue;
		fprintf(fp3,"__[[번호[[%d\n",bbs.num);
		fprintf(fp3,"__[[제목[[%s\n",bbs.title);
		fprintf(fp3,"__[[아이[[%s\n",bbs.id);
		fprintf(fp3,"__[[이름[[%s\n",bbs.name);
		fprintf(fp3,"__[[날짜[[%s\n",datestr(8,bbs.date));
		fprintf(fp3,"__[[조날[[%s\n",datestr(8,bbs.date2));
		fprintf(fp3,"__[[줄수[[%d\n",bbs.line);
		if (bbs.read			!=0) fprintf(fp3,"__[[조회[[%d\n",bbs.read);
		if (bbs.down			!=0) fprintf(fp3,"__[[다운[[%d\n",bbs.down);
        if (bbs.gd              !=0) fprintf(fp3,"__[[추천[[%d\n",bbs.gd);
        if (bbs.ok              !=0) {
            if (calc_ok(1)>0) fprintf(fp3,"__[[찬성[[%u\n",calc_ok(1));
            if (calc_ok(2)>0) fprintf(fp3,"__[[반대[[%u\n",calc_ok(2));
            if (calc_ok(3)>0) fprintf(fp3,"__[[응답[[%u\n",calc_ok(3));
        }
		if (strlen(bbs.filename )>0) fprintf(fp3,"__[[파일[[%s\n",bbs.filename);
        if (bbs.filesize        !=0) fprintf(fp3,"__[[파크[[%u\n",bbs.filesize);
		if (strlen(bbs.tcode	)>0) fprintf(fp3,"__[[티코[[%s\n",bbs.tcode);
		if (strlen(bbs.key		)>0) fprintf(fp3,"__[[키워[[%s\n",bbs.key);
        if (bbs.size>0&&bbs.size<1000000) {
            fprintf(fp3,"__[[내용[[\n");
            ch=0;
            fseek(fp2,bbs.posi,SEEK_SET);
            for (i=0;i<bbs.size;i++) {
                ch=fgetc(fp2);
                if (ch==0||ch==-1) break;
                fputc(ch,fp3);             //내용에 저장
            }
            if (ch!='\n') fprintf(fp3,"\n");
        }
		count++;
        fprintf(fp3,"__[[완료[[\n");
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}
