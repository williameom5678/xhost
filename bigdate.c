#include <ctype.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termio.h>
#include <time.h>
#include <unistd.h>    //µð·ºÅä¸® È­ÀÏ¸í º¯È¯ÇÔ¼ö¶§¹®¿¡ ÇÊ¿ä
#include <dirent.h>    //µð·ºÅä¸® È­ÀÏ¸í º¯È¯ÇÔ¼ö¶§¹®¿¡ ÇÊ¿ä

#define No 0
#define Yes 1

char bstr[7][200];
char astr[7][200];
char ostr[7][200];
main(argc, argv)
int argc;
char *argv[];
{
    int i,i2,n;
    char buff1[200];
    char buff2[200];
 //                 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678
    strcpy(bstr[0],"           ¡á¡á       ¡á     ¡á¡á   ¡á¡á¡á¡á     ¡á   ¡á¡á¡á¡á     ¡á   ¡á¡á¡á¡á   ¡á¡á     ¡á¡á                     ");
    strcpy(bstr[1],"         ¡á    ¡á   ¡á¡á   ¡á    ¡á       ¡á   ¡á¡á   ¡á         ¡á           ¡á ¡á    ¡á ¡á    ¡á               ¡á  ");
    strcpy(bstr[2],"         ¡á    ¡á     ¡á         ¡á     ¡á   ¡á  ¡á   ¡á¡á¡á   ¡á¡á¡á       ¡á     ¡á¡á   ¡á    ¡á    ¡á        ¡á   ");
    strcpy(bstr[3],"         ¡á    ¡á     ¡á       ¡á         ¡á ¡á  ¡á         ¡á ¡á    ¡á   ¡á     ¡á    ¡á   ¡á¡á¡á             ¡á    ");
    strcpy(bstr[4],"         ¡á    ¡á     ¡á     ¡á           ¡á ¡á¡á¡á¡á       ¡á ¡á    ¡á   ¡á     ¡á    ¡á       ¡á    ¡á      ¡á     ");
    strcpy(bstr[5],"           ¡á¡á     ¡á¡á¡á ¡á¡á¡á¡á ¡á¡á¡á       ¡á   ¡á¡á¡á     ¡á¡á     ¡á       ¡á¡á     ¡á¡á                     ");

	srand(time(NULL));			//³­¼öÀÇ ÃÊ±âÈ­
    n=rand()%80*2;
    strcpy(buff1,"¡Ú¡Ù¡â¡ä¢·¢¹¡ã¡å¢¸¢º¢Æ¢Ê£À£££¤£¥£Þ£¦£ª--==**¥£¥²¥Õ¥Î¥÷++||__£­£½£ª£«£ü£ß¡Å¡ñ¡¤¡¥¡§..£®£º¡Ï¡Û¢Á¡Ý¡Ü¡Ý¢´¡Þ¡ß¡Ú¡Ù¡á­¢¢Ñ¢Ð­¢­£¢Î¢Ï¡Ø¢Í¢Ã¢Â¡×¡ë¢»¢¼¢½¢¾¢¿¢À¢Á¢Ä¢Å¢Û¢Ü¢Ý");
    buff2[0]=buff1[n];
    buff2[1]=buff1[n+1];
    buff2[2]=0;

    if (is_dir("user")) now2str3(buff1);   // 99/99/99
    if (is_dir("bin"))  now2str2(buff2);   // 12:31:12
    conv1(buff1);
    for (i2=0;i2<6;i2++) {
        xy(5,i2+5);
        printf(ostr[i2]);
    }
    conv1(buff2);
    for (i2=0;i2<6;i2++) {
        xy(5,i2+13);
        printf(ostr[i2]);
    }
}

conv1(char *buff)
{
    int i,i2,n;
    for (i2=0;i2<6;i2++) strcpy(ostr[i2],"");
    for (i=0;i<8;i++) {
        conv2(buff[i]);
        for (i2=0;i2<6;i2++) {
            strcat(ostr[i2],astr[i2]);
        }
    }
}

conv2(int c)  //c -> ¹®ÀÚ¿­·Î
{
    int i,i2,n;
    if      (c>='0'&&c<='9') n=c-'0'+1;
    else if (c==':')         n=11;
    else if (c=='/')         n=12;
    else                     n=0;
    for (i=0;i<6;i++) {
        for (i2=0;i2<9;i2++) astr[i][i2]=bstr[i][n*9+i2];
        astr[i][i2]=0;
    }
}


now2str2(char *buff)
{
	struct tm *tp;
	time_t t;
	time(&t); //12311231
	tp = localtime(&t); //12311231
	sprintf(buff,"%02d:%02d:%02d",tp->tm_hour,tp->tm_min,tp->tm_sec);
}



now2str3(char *buff)
{
	struct tm *tp;
	time_t t;
	time(&t); //12311231
	tp = localtime(&t); //12311231
	sprintf(buff,"%02d/%02d/%02d",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
}



xy(int x,int y)  {
	printf("[%d;%dH",y,x);
}


//ÇöÀçÀÇ µð·ºÅä¸®°¡ Á¸ÀçÇÏ´Â°¡? ÀÖÀ¸¸é Yes ¾øÀ¸¸é No
is_dir(char *dirname)
{
    DIR *dp;
    struct dirent *fp;
    struct stat fs;
    if((dp= opendir(dirname)) == NULL) return No;
        /* µð·ºÅä¸® ¸ñ·Ï ¾ò±â */
    while((fp= readdir(dp)) != NULL) {
        if(strcmp(fp->d_name,".")==0) return Yes;      //.ÀÌ ÀÖ´Ù.
    }
    return No;
}


strchng(char *dest, char *src, char *search, char *change) {
   int i, j, len_search, len_change;

   len_search= strlen(search);
   len_change= strlen(change);
   memset((void *)dest, 0, (size_t)strlen(dest));
   for(i=j=0; src[i];) {
	  if(src[i] == search[0]) {
		 if(strncmp(&src[i], search, len_search) == 0) {
			strcat(&dest[j], change);
			i+= len_search;
			j+= len_change;
		 }
		 else dest[j++] = src[i++];
	  }
	  else dest[j++] = src[i++];
   }
   dest[j] = 0;
}

