////////////////////////////////////////////////////////////////////////
//            XHOST 5.3s: �������� BBS HOST                            //
//			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //
//            �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����              //
//            �������� ������ �޾ƾ� �մϴ�.                          //
////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ȸ�� ���� �ٿ ����
//////////////////////////////////////////////////////////////////////////

#include "bbs.h"

char now_id[9];
char now_time[100];

main(argc, argv)
int argc;
char *argv[];
{
	char buff[80];
	time_t t;
	struct tm *tp;
    FILE *fp;
    int i;

    read_cfg();
    
    strcpy(now_id,cfg.sysop);

	if (argc==2) strcpy(now_id,argv[1]);
    if (!read_pf(now_id)) return;

	//����� �źε� ���̵��ΰ�?
    if (pf.type<19&&in_file2("system/ed_level.txt",pf.id)) {
        printf(" **  �� ����� ����� ������ ����Դϴ�. **\n");
        pressenter();
        return;
    }

	time(&t);
	tp=localtime(&t);	  //���� �ð�
	sprintf(now_time,"%02d/%02d/%02d %02d:%02d",
        tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);

    read_atype();

	cls();
	while (1) {
		printf("\n");
        printf("                  ȸ�� ����/�ٿ�뷮 ����\n");
		printfL2();
        printf("  <�̱���� Ȱ���� ������ �� ȸ���� �������μ� ���Ǿ���մϴ�.>\n");
        printf("  <   �������� ��������  ����ϴ� ����� �� ¡��ó�� �մϴ�.  >\n");
		printfL2();
        printf("  1. ȸ��  ��    ��  ����       \n");
        printf("  2. ȸ��  �ٿ�뷮  ����       \n");
        printf("  3. ȸ��  ���ð�  ����       \n");
        printf("  4. ȸ��  ���泻��  Ȯ��       \n");
        printf("  5. ȸ��  ���� +30  �ٿ� +5MB  (ȸ������ ���� 50 �̻� ��������)\n");
        printf("  6. ȸ��  ���� +50  �ٿ� +8MB  (ȸ������ �ٿ� 8MB�̻� ��������)\n");
        printf("  7. ȸ��  ����+100  �ٿ�+10MB  \n");
        printf("  8. ȸ��  ����+500  �ٿ�+50MB  \n");
        printf(" 11. ȸ��  ���� -30  �ٿ� -5MB  (ȸ������ ���� 50 �̻� ���ұ���)\n");
        printf(" 12. ȸ��  ���� -50  �ٿ� -8MB  (ȸ������ ���� 8MB�̻� ���ұ���)\n");
        printf(" 20. ȸ��  ��    ��  Ȯ��       \n");
        printf(" 21. ������ ��� ����       (���18�̻�)\n");
		printfL2();
		printf("  ���� >>");
		lineinput(buff,2);
		printf("\n");
        i=atoi(buff);
        if      (i==1)   add_pflevel();
        else if (i==2)   add_pfdown();
        else if (i==3)   add_pftime();
        else if (i==4)   add_pflist();
        else if (i==5)   add_up(1);
        else if (i==6)   add_up(2);
        else if (i==7)   add_up(3);
        else if (i==8)   add_up(4);
        else if (i==11)  add_up(8);
        else if (i==12)  add_up(9);
        else if (i==20)  {
            sprintf(buff,"bin/findname main %s",pf.id);system(buff);
        }
        else if (pf.type>17&&i==21) xedit("system/ed_level.txt");
		else break;
	}
}



add_up(int mode)
{
	FILE *fp;
    int n,old1,old2;
	char buff[200];
	while (1) {
        printf("\n");
        if (mode==1) printf("���� +30  �ٿ� +5MB  ȸ�����̵� >>");
        if (mode==2) printf("���� +50  �ٿ� +8MB  ȸ�����̵� >>");
        if (mode==3) printf("����+100  �ٿ�+10MB  ȸ�����̵� >>");
        if (mode==4) printf("����+500  �ٿ�+50MB  ȸ�����̵� >>");
        if (mode==8) printf("���� -30  �ٿ� -5MB  ȸ�����̵� >>");
        if (mode==9) printf("���� -50  �ٿ� -8MB  ȸ�����̵� >>");
		lineinput(buff,8);
		if (buff[0]==0) return;
		if (!read_pf2(buff)) return;
        if (pf2.type>=14) {
            printf(" �����Ҽ� ���� ȸ���� ����Դϴ�.\n");
			continue;
		}
        old1=pf2.level;
        old2=pf2.down ;
        if (mode==1) pf2.level+=30;
        if (mode==2) pf2.level+=50;
        if (mode==3) pf2.level+=100;
        if (mode==4) pf2.level+=500;
        if (mode==8) pf2.level-=30;
        if (mode==9) pf2.level-=50;
        if (mode==1) pf2.down+=5000000;
        if (mode==2) pf2.down+=8000000;
        if (mode==3) pf2.down+=10000000;
        if (mode==4) pf2.down+=50000000;
        if (mode==8) pf2.down-=3000000;
        if (mode==9) pf2.down-=5000000;
        if (cfg.levelmax>0&&pf2.level>cfg.levelmax) pf2.level=cfg.levelmax;
		if (pf2.level<10  ) pf2.level=10;
        if (pf2.down>500000000)  pf2.down=500000000;
		if (pf2.down<0		  )  pf2.down=0;

        if ((fp=fopen(ID_FILE,"r+"))!=NULL) {
            fseek(fp,pf2.num*sizeof_pf,SEEK_SET);
//            fwrite(&pf2,sizeof_pf,1,fp);
            gwrite(131,pf2,fp);
            fclose(fp);
        }
        printf("\n");
        sprintf(buff,"%8s(%8s)L:%6dL->%6dL",pf2.id,pf2.name,old1,pf2.level);
        save_rec(1,19,buff);
        sprintf(buff,"%8s(%8s)D:%6dM->%6dM",pf2.id,pf2.name,old2/1000000,pf2.down/1000000);
        save_rec(1,19,buff);
	}
}


add_pflevel()
{
	FILE *fp;
	int n,old;
	char buff[200];
	while (1) {
		printf("ȸ�����̵� >>");
		lineinput(buff,8);
		if (buff[0]==0) return;
		if (!read_pf2(buff)) return;
		printf("    %8s(%8s) ����:%d �ٿ�뷮:%d\n",pf2.id,pf2.name,pf2.level,pf2.down/1000);
        if (pf.type<18&&pf2.type>=16) {
			printf("�����Ҽ� ���� ȸ���� ����Դϴ�.\n");
			continue;
		}
		printf("���� �� �� >>");
        lineinput(buff,10);
		n=atoi(buff);
        if (pf.type<18&&(n<-10000||n>10000)) {
            printf(" �����ʰ� (-10000 ~ 10000)\n");
			continue;
		}
		old=pf2.level;
		pf2.level+=n;
		if (pf2.level<10  ) pf2.level=10;

        if ((fp=fopen(ID_FILE,"r+"))!=NULL) {
            fseek(fp,pf2.num*sizeof_pf,SEEK_SET);
//          fwrite(&pf2,sizeof_pf,1,fp);
            gwrite(134,pf2,fp);
            fclose(fp);
        }

        sprintf(buff,"%8s(%8s)%6d->%6d",pf2.id,pf2.name,old,pf2.level);
        save_rec(1,19,buff);
	}
}


add_pfdown()
{
	FILE *fp;
	int n;
	int old;
	char buff[200];
	while (1) {
		printf("ȸ�����̵� >>");
		lineinput(buff,8);
		if (buff[0]==0) return;
		if (!read_pf2(buff)) return;
		printf("    %8s(%8s) ����:%d �ٿ�뷮:%dKB\n",pf2.id,pf2.name,pf2.level,pf2.down/1000);
        if (pf.type<18&&pf2.type>=16) {
			printf("�����Ҽ� ���� ȸ���� ����Դϴ�.\n");
			continue;
		}
		printf("���� �ٿ�뷮 (???MB) >>");
        lineinput(buff,10);
		n=atoi(buff);
        if (pf.type<18&&(n<-10||n>10)) {
			printf(" �����ʰ� (-10MB ~ 10MB)\n");
			continue;
		}
		old=pf2.down;
		pf2.down+=n*1000000;
		if (pf2.down>100000000)  pf2.down=100000000;
		if (pf2.down<0		  )  pf2.down=0;

        if ((fp=fopen(ID_FILE,"r+"))!=NULL) {
            fseek(fp,pf2.num*sizeof_pf,SEEK_SET);
//            fwrite(&pf2,sizeof_pf,1,fp);
            gwrite(134,pf2,fp);
            fclose(fp);
        }

        sprintf(buff,"%8s(%8s)%6d->%6d",pf2.id,pf2.name,old/1000,pf2.down/1000);
        save_rec(1,19,buff);

	}
}


add_pftime()
{
}

add_pflist()
{
}



//str�� ���Ե� �ٸ� �ٺ�����
view_text4(char *filename,int mode,char *str)
{
	char c, s[10];
	char buff[205];
	int line =0;
	int tline=0;
	FILE *fp;
	if ((fp=fopen(filename,"r+"))==NULL) return No;
	while ((c=getc(fp))!=EOF) if (c=='\n') tline++;
	rewind(fp);
	while (fgets(buff,200,fp)!=NULL) {
		if (buff[0]!=0&&(str[0]==0||stristr(buff,str))) {
			printf("%s",buff);    //�ش�Ǵ� ���� ������
			line ++;
			if (mode>0&&line%mode ==0) {
				s[0]=0;
				printf(" ########(P%d/%d) ���[ENTER] �������(Q) �ٽú���(R) ����(S) �����(A) ",line/mode,tline/mode+1);
                lineinput(s,5);printf("[80D");
				if (s[0]=='r'||s[0]=='R') { line=0;rewind(fp);}
				if (s[0]=='a'||s[0]=='A') { line=0;rewind(fp);mode=99999;}
				if (s[0]=='p'||s[0]=='P'||s[0]=='q'||s[0]=='Q') break;
				if (s[0]=='S'||s[0]=='s') mode=99999;
			}
		}
	}
	if (mode>0&&(line%mode) !=0) {
		printf("\n############### ������ ���Դϴ�. [ENTER] : ################");
		lineinput(s,5);
		printf("\n");
	}
    if (fp!=NULL) fclose(fp);
	return Yes;
}


