////////////////////////////////////////////////////////////////////////
//			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //
//   ��� : ���� ���丮�� ȭ���̸��� �� 000��ȣ.* -> ��ȣ.*�� ��ȯ
////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

main() {
   DIR *dp;
   struct dirent *fp;
   int i,ii,i2,is_dot,is_zero;
   int count=0;
   char buff[500],buff1[500],buff2[500],buff3[500],buff4[500],filename[500];
   
   if((dp= opendir("./")) == NULL) {
      perror("big2sml");
      return -1;
   }
   /* ���丮 ��� ��� */
   while((fp= readdir(dp)) != NULL) {

      /* ȭ���̸��� '.'�� '..'�� '.'�� ���۵Ǵ� ����ȭ���̶�� �׳� ���*/ 
      if(*(fp->d_name) == '.')
         continue;
      else {
         bzero(buff1,30);
         bzero(buff2,30);
         bzero(buff3,30);
         strcpy(buff3,fp->d_name);
         if (strlen(buff3)<3) continue;
         is_dot=0;
         ii=0;
         for (i=0;i<(int)strlen(buff3);i++) {
            if (buff3[i]=='.') is_dot=1;
            else if (is_dot==0) buff1[i]=buff3[i];  //. ����
            else buff2[ii++]=buff3[i];              //.����
         }
         sprintf(filename,"%d.%s",atoi(buff1),buff2);
         if (strcmp(filename,fp->d_name)!=0&&  //���ϸ� �ٸ���
             atoi(buff1)>0&&
             strlen(buff2)>0) {
            sprintf(buff,"mv %s %s",fp->d_name,filename);
            system(buff);
            printf("%s\n",buff);
            count++;
         }
      }
   }
   closedir(dp);
   printf("�� %d�� ��ȯ\n",count);
}
