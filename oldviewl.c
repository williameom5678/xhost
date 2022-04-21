#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termio.h>
#include <time.h>

main()
{
	FILE *fp1,*fp2;
	char buff[300],buff1[300],buff2[300];
	char filename1[300],filename2[300];
    int year,month,count;
	struct tm *tp;
	time_t today;
	time(&today);	//오늘의 날짜 지정
	tp = localtime(&today);
	year=tp->tm_year+1900;
	month=tp->tm_mon+1;
	sprintf(filename1,"user/rec/%03d%02d.rec",year%100+100,month);
	sprintf(filename2,"user/rec/%02d%02d.rec",year%100,month);
	if ((fp1=fopen(filename1,"r"))==NULL) {
		printf("\n ## %s 파일이 없습니다.\n",filename1);
		return;
	}
    count=0;
	if ((fp2=fopen(filename2,"w"))==NULL) return;
	while (fgets(buff,300,fp1)!=NULL) {
        if (buff[0]!='1') continue;
		fprintf(fp2,"%s",&buff[1]);
        count++;
	}
	fclose(fp1);
	fclose(fp2);
    printf(" ## FILE:%s\n ## 총 %d 자료 변경 완료\n",filename2,count);
}
