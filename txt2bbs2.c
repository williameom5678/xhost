////////////////////////////////////////////////////////////////////////
//	XHOST �������� BBS HOST ������ : �輺��  (�뱸:964-1002)
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
        printf("txt2bbs2  �������ϸ�(Ȯ��������) ���ο����ϸ�.out\n");
        return;
    }
    sprintf(NOW_FILE ,"%s.tit",argv[1]);
    sprintf(NOW_FILED,"%s.txt",argv[1]);

	fp1=fopen(NOW_FILE ,"r");
	fp2=fopen(NOW_FILED,"r");
	if (fp1==NULL||fp2==NULL) return;
    sprintf(filename,"%s",argv[2]);
    fp3=fopen(filename,"w+");
    printf("\n..%s..-> %s..ȭ�� ������.............\n",NOW_FILE,filename);
	now_posi=0;
	count=0;
	while (fread(&bbs,sizeof_bbs,1,fp1)){
		if (bbs.del)		continue;
		fprintf(fp3,"__[[��ȣ[[%d\n",bbs.num);
		fprintf(fp3,"__[[����[[%s\n",bbs.title);
		fprintf(fp3,"__[[����[[%s\n",bbs.id);
		fprintf(fp3,"__[[�̸�[[%s\n",bbs.name);
		fprintf(fp3,"__[[��¥[[%s\n",datestr(8,bbs.date));
		fprintf(fp3,"__[[����[[%s\n",datestr(8,bbs.date2));
		fprintf(fp3,"__[[�ټ�[[%d\n",bbs.line);
		if (bbs.read			!=0) fprintf(fp3,"__[[��ȸ[[%d\n",bbs.read);
		if (bbs.down			!=0) fprintf(fp3,"__[[�ٿ�[[%d\n",bbs.down);
        if (bbs.gd              !=0) fprintf(fp3,"__[[��õ[[%d\n",bbs.gd);
        if (bbs.ok              !=0) {
            if (calc_ok(1)>0) fprintf(fp3,"__[[����[[%u\n",calc_ok(1));
            if (calc_ok(2)>0) fprintf(fp3,"__[[�ݴ�[[%u\n",calc_ok(2));
            if (calc_ok(3)>0) fprintf(fp3,"__[[����[[%u\n",calc_ok(3));
        }
		if (strlen(bbs.filename )>0) fprintf(fp3,"__[[����[[%s\n",bbs.filename);
        if (bbs.filesize        !=0) fprintf(fp3,"__[[��ũ[[%u\n",bbs.filesize);
		if (strlen(bbs.tcode	)>0) fprintf(fp3,"__[[Ƽ��[[%s\n",bbs.tcode);
		if (strlen(bbs.key		)>0) fprintf(fp3,"__[[Ű��[[%s\n",bbs.key);
        if (bbs.size>0&&bbs.size<1000000) {
            fprintf(fp3,"__[[����[[\n");
            ch=0;
            fseek(fp2,bbs.posi,SEEK_SET);
            for (i=0;i<bbs.size;i++) {
                ch=fgetc(fp2);
                if (ch==0||ch==-1) break;
                fputc(ch,fp3);             //���뿡 ����
            }
            if (ch!='\n') fprintf(fp3,"\n");
        }
		count++;
        fprintf(fp3,"__[[�Ϸ�[[\n");
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}
