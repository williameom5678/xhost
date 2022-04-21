////////////////////////////////////////////////////////////////////////
//            XHOST 5.3s: �������� BBS HOST                            //
//			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //
//            �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����              //
//            �������� ������ �޾ƾ� �մϴ�.                          //
////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "base.h"

#define FNAME_BIG  1     /* ȭ���̸� �빮�� */
#define FNAME_SML  0     /* ȭ���̸� �ҹ��� */

int count;               /* �̸��ٲ� ���� */

/* gcc 2.6.3�������� strlwr, strupr �Լ��� ��� ������ ���� ���Դϴ�. */
/* �빮�ڹ��ڿ��� �ҹ��ڹ��ڿ���  ��ȯ*/

static char *strlwr(char org[]) {  
   static char dest[256];
   int i;
   
   for(i= 0; org[i] != '\0'; i++) {
      if(org[i] >= 'A' && org[i] <= 'Z') 
         dest[i]= org[i]+('a'-'A');
      else 
         dest[i]= org[i];
   }
   dest[i]= '\0';
   
   return dest;
}

/* ȭ���̸��� ���� */
int fname_change(char org[]) {
   char dest[256];
   char buff[300];
   
   strcpy(dest, strlwr(org));
   sprintf(buff,"cp -rf '%s' '%s'\n",org,dest);
   system(buff);
   if(unlink(org) < 0) {
      unlink(dest);
      return -1;
   }
   return 0;
}
   
   
/* ���� ���丮�� ȭ���̸��� �� �۾��ϴ� �� */
int big2sml() {
   DIR *dp;
   struct dirent *fp;
   
   if((dp= opendir("./")) == NULL) {
      perror("big2sml");
      return -1;
   }
   /* ���丮 ��� ��� */
   while((fp= readdir(dp)) != NULL) {

      /* ȭ���̸��� '.'�� '..'�� '.'�� ���۵Ǵ� ����ȭ���̶�� �׳� ���*/ 
      if(*(fp->d_name) == '.')
         continue;
      else 
         if(verify_big(fp->d_name) == FNAME_BIG) { /* �빮�ڶ�� */
            fname_change( fp->d_name);             /* �ҹ��ڷ� ��ȯ */
            count++;
         }
   }
   if(count != 0) printf("Succeeded ! %d Filename Change.\n", count);
   closedir(dp);
   
   return 0;
}

// ȭ���̸��� �빮���ֳ�?
int verify_big(char file[]) {
	char *p;
	for(p= file; *p != '\0'; p++)
		if(*p >= 'A' && *p <= 'Z') return Yes;
	return No;
}


int main() {
   return big2sml();
}
