////////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
// ³»¿ë : ÀÚ·á½Ç µî·ÏÀÔ·Â
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
    printf("\n OS Á¾·ù¸¦ ¼±ÅÃÇØ ÁÖ½Ê½Ã¿À.\n");
    printf(" 1.WINDOWS  2.WIN-NT  3.DOS  4.UNIX  5.MAC  6.OS/2  0.Ãë¼Ò\n");
    printf(" ¹øÈ£>>");lineinput(buff,5);printf("\n\n");n=atoi(buff);
         if (n==1) strcpy(ans[0],"WINDOWS");
    else if (n==2) strcpy(ans[0],"WIN-NT");
    else if (n==3) strcpy(ans[0],"DOS");
    else if (n==4) strcpy(ans[0],"UNIX");
    else if (n==5) strcpy(ans[0],"MAC");
    else if (n==6) strcpy(ans[0],"OS/2");
    else     goto quit1;
    printf(" PC È¯°æÀ» ¼±ÅÃÇØ ÁÖ½Ê½Ã¿À.\n");
    printf(" 1.PentiumÀÌ»ó  2.486ÀÌ»ó  3.386ÀÌ»ó  4.±âÅ¸  0.Ãë¼Ò\n");
    printf(" ¹øÈ£>>");lineinput(buff,5);printf("\n\n");n=atoi(buff);
         if (n==1) strcpy(ans[1],"PentiumÀÌ»ó");
    else if (n==2) strcpy(ans[1],"486ÀÌ»ó");
    else if (n==3) strcpy(ans[1],"386ÀÌ»ó");
    else if (n==4) strcpy(ans[1],"±âÅ¸");
    else     goto quit1;
    printf(" ½ÇÇà°¡´ÉÇÑ ¸Þ¸ð¸®(RAM) ¿ë·®À» ¼±ÅÃÇØ ÁÖ½Ê½Ã¿À.\n");
    printf(" 1. 8MB  2. 16MB  3. 32MB  4.±âÅ¸  0.Ãë¼Ò\n");
    printf(" ¹øÈ£>>");lineinput(buff,5);printf("\n\n");n=atoi(buff);
         if (n==1) strcpy(ans[2],"8MB");
    else if (n==2) strcpy(ans[2],"16MB");
    else if (n==3) strcpy(ans[2],"32MB");
    else if (n==4) strcpy(ans[2],"±âÅ¸");
    else     goto quit1;
    printf(" °ø°³ ¿©ºÎ¸¦ ¼±ÅÃÇØ ÁÖ½Ê½Ã¿À.\n");
    printf(" 1.°ø°³  2.½¦¾î¿þ¾î  3.µ¥¸ð¹öÁ¯  4.±âÅ¸  0.Ãë¼Ò\n");
    printf(" ¹øÈ£>>");lineinput(buff,5);printf("\n");n=atoi(buff);
         if (n==1) strcpy(ans[3],"°ø°³");
    else if (n==2) strcpy(ans[3],"½¦¾î¿þ¾î");
    else if (n==3) strcpy(ans[3],"µ¥¸ð¹öÁ¯");
    else if (n==4) strcpy(ans[3],"±âÅ¸");
    else     goto quit1;
// ¦£¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤
// ¦¢    OS È¯°æ : WINDOWS               PC  È¯°æ : PentiumÀÌ»ó    ¦¢
// ¦¢    RAM     : 8MB                   °ø°³¿©ºÎ : °ø°³           ¦¢
// ¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥


    sprintf(abuff[0],"  ¦£¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤");
    sprintf(abuff[1],"  ¦¢   [7m OS È¯°æ : %-20s  PC  È¯°æ : %-12s[m   ¦¢",ans[0],ans[1]);
    sprintf(abuff[2],"  ¦¢   [7m RAM     : %-20s  °ø°³¿©ºÎ : %-12s[m   ¦¢",ans[2],ans[3]);
    sprintf(abuff[3],"  ¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥");
    for (i=0;i<4;i++) {
        printf("%s\n",abuff[i]);
    }
    printf(" ÀÔ·ÂÇÑ ³»¿ëÀÌ ¸Â½À´Ï±î? ¼öÁ¤ÇÏ½Ã·Á¸é 'n'À» ÀÔ·ÂÇÏ¼¼¿ä.(Y/n)");
    if (yesno2("",Yes)==No) goto restart1;
    if ((fp=fopen(outfile,"w"))!=NULL) {
        for (i=0;i<4;i++) {
            fprintf(fp,"%s\n",abuff[i]);
        }
        fclose(fp);
        exit(1);
    }
    quit1:
    exit(-1);
}


