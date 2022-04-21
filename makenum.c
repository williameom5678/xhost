#include <stdio.h>
#include <time.h>
#include "bbs.h"
#include "base.h"
main()
{
    FILE *fp;
    char buff[300];
    int n=0;
	time_t t;
	struct tm *tp;
	time(&t);
	tp = localtime(&t);

    if ((fp=fopen("makenum.h","r"))!=NULL) {
        fgets(buff,200,fp);
        buff[40]=0;
        strchng2(buff,"\"","");
        n=atoi(&buff[20])+1;
        fclose(fp);
    }
    if ((fp=fopen("makenum.h","w"))!=NULL) {
        fprintf(fp,"#define MAKENUM             \"%d\"                      \n",n);
        if (tp->tm_year<=2000) tp->tm_year+=1900;
        sprintf(buff,"\"%04d-%02d-%02d %02d:%02d\"",tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
        fprintf(fp,"#define MAKEDATE            %s\n",buff);
        fprintf(fp,"//%s\n",VER1);
        fprintf(fp,"//%s\n",VER2);
        fclose(fp);
        printf("make_num=%d  make_date=%s\n",n,buff);
        fflush(stdout);
    }
    if (n%30==0) {
        sprintf(buff,"mkdir bk%02d%02d%02d",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
        system(buff);
        sprintf(buff,"cp -f *.c *.h bk%02d%02d%02d/",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
        system(buff);
    }
}


strchng2(char *src, char *search, char *change)
{
	int n;
	char buff1[500];
	bzero(buff1,500);
	n=strchng(buff1,src,search,change);
	strcpy(src,buff1);
	return n;
}


strchng(char *dest, char *src, char *search, char *change)
{
   int i, j, len_search, len_change,found=No;
   len_search= strlen(search);
   len_change= strlen(change);
   bzero(dest,strlen(dest));
   for(i=j=0; src[i];) {
	  if(src[i] == search[0]) {
		 if(strncmp(&src[i], search, len_search) == 0) {
			found=Yes;
			strcat(&dest[j], change);
			i+= len_search;
			j+= len_change;
		 }
		 else dest[j++] = src[i++];
	  }
	  else dest[j++] = src[i++];
   }
   dest[j] = 0;
   return found;
}



