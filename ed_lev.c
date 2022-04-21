////////////////////////////////////////////////////////////////////////
//            XHOST 5.3s: ¸®´ª½º¿ë BBS HOST                            //
//			  °³¹ßÀÚ : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)		  //
//            ÀÌ ¼Ò½º¸¦ º¯°æÇÏ¿© »ó¿ëÀ¸·Î »ç¿ëÇÏ½Ç¶§¿¡´Â              //
//            °³¹ßÀÚÀÇ ½ÂÀÎÀ» ¹Þ¾Æ¾ß ÇÕ´Ï´Ù.                          //
////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// È¸¿ø ·¹º§ ´Ù¿î·® ¼öÁ¤
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

	//»ç¿ëÀÌ °ÅºÎµÈ ¾ÆÀÌµðÀÎ°¡?
    if (pf.type<19&&in_file2("system/ed_level.txt",pf.id)) {
        printf(" **  ÀÌ ±â´ÉÀÇ »ç¿ëÀ» ±ÝÁöÇÑ ¿î¿µÁøÀÔ´Ï´Ù. **\n");
        pressenter();
        return;
    }

	time(&t);
	tp=localtime(&t);	  //ÇöÀç ½Ã°¢
	sprintf(now_time,"%02d/%02d/%02d %02d:%02d",
        tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);

    read_atype();

	cls();
	while (1) {
		printf("\n");
        printf("                  È¸¿ø ·¹º§/´Ù¿î¿ë·® °ü¸®\n");
		printfL2();
        printf("  <ÀÌ±â´ÉÀº È°µ¿À» ¿­½ÉÈ÷ ÇÑ È¸¿øÀÇ Æ÷»óÀ¸·Î¼­ »ç¿ëµÇ¾î¾ßÇÕ´Ï´Ù.>\n");
        printf("  <   °³ÀÎÀûÀÎ ¸ñÀûÀ¸·Î  »ç¿ëÇÏ´Â ¿î¿µÁøÀº Áß Â¡°èÃ³¸® ÇÕ´Ï´Ù.  >\n");
		printfL2();
        printf("  1. È¸¿ø  ·¹    º§  Á¶Á¤       \n");
        printf("  2. È¸¿ø  ´Ù¿î¿ë·®  Á¶Á¤       \n");
        printf("  3. È¸¿ø  »ç¿ë½Ã°£  Á¶Á¤       \n");
        printf("  4. È¸¿ø  º¯°æ³»¿ª  È®ÀÎ       \n");
        printf("  5. È¸¿ø  ·¹º§ +30  ´Ù¿î +5MB  (È¸¿ø¿¡°Ô ·¹º§ 50 ÀÌ»ó Áõ°¡±ÝÁö)\n");
        printf("  6. È¸¿ø  ·¹º§ +50  ´Ù¿î +8MB  (È¸¿ø¿¡°Ô ´Ù¿î 8MBÀÌ»ó Áõ°¡±ÝÁö)\n");
        printf("  7. È¸¿ø  ·¹º§+100  ´Ù¿î+10MB  \n");
        printf("  8. È¸¿ø  ·¹º§+500  ´Ù¿î+50MB  \n");
        printf(" 11. È¸¿ø  ·¹º§ -30  ´Ù¿î -5MB  (È¸¿ø¿¡°Ô ·¹º§ 50 ÀÌ»ó °¨¼Ò±ÝÁö)\n");
        printf(" 12. È¸¿ø  ·¹º§ -50  ´Ù¿î -8MB  (È¸¿ø¿¡°Ô ·¹º§ 8MBÀÌ»ó °¨¼Ò±ÝÁö)\n");
        printf(" 20. È¸¿ø  °Ë    »ö  È®ÀÎ       \n");
        printf(" 21. »ç¿ë±ÝÁö ¿î¿µÁø ÁöÁ¤       (µî±Þ18ÀÌ»ó)\n");
		printfL2();
		printf("  ¼±ÅÃ >>");
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
        if (mode==1) printf("·¹º§ +30  ´Ù¿î +5MB  È¸¿ø¾ÆÀÌµð >>");
        if (mode==2) printf("·¹º§ +50  ´Ù¿î +8MB  È¸¿ø¾ÆÀÌµð >>");
        if (mode==3) printf("·¹º§+100  ´Ù¿î+10MB  È¸¿ø¾ÆÀÌµð >>");
        if (mode==4) printf("·¹º§+500  ´Ù¿î+50MB  È¸¿ø¾ÆÀÌµð >>");
        if (mode==8) printf("·¹º§ -30  ´Ù¿î -5MB  È¸¿ø¾ÆÀÌµð >>");
        if (mode==9) printf("·¹º§ -50  ´Ù¿î -8MB  È¸¿ø¾ÆÀÌµð >>");
		lineinput(buff,8);
		if (buff[0]==0) return;
		if (!read_pf2(buff)) return;
        if (pf2.type>=14) {
            printf(" ¼öÁ¤ÇÒ¼ö ¾ø´Â È¸¿øÀÇ µî±ÞÀÔ´Ï´Ù.\n");
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
		printf("È¸¿ø¾ÆÀÌµð >>");
		lineinput(buff,8);
		if (buff[0]==0) return;
		if (!read_pf2(buff)) return;
		printf("    %8s(%8s) ·¹º§:%d ´Ù¿î¿ë·®:%d\n",pf2.id,pf2.name,pf2.level,pf2.down/1000);
        if (pf.type<18&&pf2.type>=16) {
			printf("¼öÁ¤ÇÒ¼ö ¾ø´Â È¸¿øÀÇ µî±ÞÀÔ´Ï´Ù.\n");
			continue;
		}
		printf("°¡»ê ·¹ º§ >>");
        lineinput(buff,10);
		n=atoi(buff);
        if (pf.type<18&&(n<-10000||n>10000)) {
            printf(" ¹üÀ§ÃÊ°ú (-10000 ~ 10000)\n");
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
		printf("È¸¿ø¾ÆÀÌµð >>");
		lineinput(buff,8);
		if (buff[0]==0) return;
		if (!read_pf2(buff)) return;
		printf("    %8s(%8s) ·¹º§:%d ´Ù¿î¿ë·®:%dKB\n",pf2.id,pf2.name,pf2.level,pf2.down/1000);
        if (pf.type<18&&pf2.type>=16) {
			printf("¼öÁ¤ÇÒ¼ö ¾ø´Â È¸¿øÀÇ µî±ÞÀÔ´Ï´Ù.\n");
			continue;
		}
		printf("°¡»ê ´Ù¿î¿ë·® (???MB) >>");
        lineinput(buff,10);
		n=atoi(buff);
        if (pf.type<18&&(n<-10||n>10)) {
			printf(" ¹üÀ§ÃÊ°ú (-10MB ~ 10MB)\n");
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



//strÀÌ Æ÷ÇÔµÈ ÁÙ¸¸ ´Ùº¸¿©ÁÜ
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
			printf("%s",buff);    //ÇØ´çµÇ´Â ÁÙÀ» º¸¿©ÁÜ
			line ++;
			if (mode>0&&line%mode ==0) {
				s[0]=0;
				printf(" ########(P%d/%d) °è¼Ó[ENTER] Ãâ·ÂÁßÁö(Q) ´Ù½Ãº¸±â(R) ¿¬¼Ó(S) ÀçÃâ·Â(A) ",line/mode,tline/mode+1);
                lineinput(s,5);printf("[80D");
				if (s[0]=='r'||s[0]=='R') { line=0;rewind(fp);}
				if (s[0]=='a'||s[0]=='A') { line=0;rewind(fp);mode=99999;}
				if (s[0]=='p'||s[0]=='P'||s[0]=='q'||s[0]=='Q') break;
				if (s[0]=='S'||s[0]=='s') mode=99999;
			}
		}
	}
	if (mode>0&&(line%mode) !=0) {
		printf("\n############### ³»¿ëÀÇ ³¡ÀÔ´Ï´Ù. [ENTER] : ################");
		lineinput(s,5);
		printf("\n");
	}
    if (fp!=NULL) fclose(fp);
	return Yes;
}


