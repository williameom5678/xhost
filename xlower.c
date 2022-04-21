////////////////////////////////////////////////////////////////////////
//            XHOST 5.3s: 리눅스용 BBS HOST                            //
//			  개발자 : 김성대  xhost@xhost.co.kr (016-320-7882)		  //
//            이 소스를 변경하여 상용으로 사용하실때에는              //
//            개발자의 승인을 받아야 합니다.                          //
////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "base.h"

#define FNAME_BIG  1     /* 화일이름 대문자 */
#define FNAME_SML  0     /* 화일이름 소문자 */

int count;               /* 이름바꾼 갯수 */

/* gcc 2.6.3버젼에는 strlwr, strupr 함수가 없어서 간단히 만든 것입니다. */
/* 대문자문자열을 소문자문자열로  변환*/

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

/* 화일이름을 변경 */
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
   
   
/* 현재 디렉토리의 화일이름을 얻어서 작업하는 곳 */
int big2sml() {
   DIR *dp;
   struct dirent *fp;
   
   if((dp= opendir("./")) == NULL) {
      perror("big2sml");
      return -1;
   }
   /* 디렉토리 목록 얻기 */
   while((fp= readdir(dp)) != NULL) {

      /* 화일이름이 '.'나 '..'나 '.'로 시작되는 숨김화일이라면 그냥 통과*/ 
      if(*(fp->d_name) == '.')
         continue;
      else 
         if(verify_big(fp->d_name) == FNAME_BIG) { /* 대문자라면 */
            fname_change( fp->d_name);             /* 소문자로 변환 */
            count++;
         }
   }
   if(count != 0) printf("Succeeded ! %d Filename Change.\n", count);
   closedir(dp);
   
   return 0;
}

// 화일이름이 대문자있나?
int verify_big(char file[]) {
	char *p;
	for(p= file; *p != '\0'; p++)
		if(*p >= 'A' && *p <= 'Z') return Yes;
	return No;
}


int main() {
   return big2sml();
}
