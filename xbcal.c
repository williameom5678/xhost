////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: 리눅스용 BBS HOST							  //
//			  개발자 : 김성대  xhost@xhost.co.kr (016-320-7882)		  //	  //
//			  이 소스를 변경하여 상용으로 사용하실때에는			  //
//			  개발자의 승인을 받아야 합니다.						  //
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
int mdays[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

/*===========================================================================
 * weekday() - return day-of-week for first day of month.
 *=========================================================================*/
short weekday(int y, int m)
{
--y;
if (y > 1752-1 || (y == 1752-1 && m > 8))
   return (mdays[m] + y + y / 4 - y / 100 + y / 400 + 1 - 0) % 7;
else
   return (mdays[m] + y + y / 4 					+ 6 - 0) % 7;
}



main()
{
	int i,x,n,dow;
	time_t today;
	struct tm *tp;
	int d[50];
	static char *weeks[7] = {"일","월","화","수","목","금","토"};

	time(&today);	//오늘의 날짜 지정
	tp = localtime(&today);

	dow = weekday(tp->tm_year+1900,tp->tm_mon);   //매월1일의 시작요일

	for (x=0;x<7*6;x++) d[x]=0;
	for (i=1;i<=days[tp->tm_mon];i++) d[i-1+dow]=i;

	printf("┌──────────────┐\n");
	printf("│ %4d년 %2d월 %2d일(%2s) %2d:%02d │\n",
		tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday],tp->tm_hour,tp->tm_min);
	printf("├──────────────┤\n");
	printf("│ 일  월  화  수  목  금  토 │\n");
	for(x=0;x<7*6;x++) {
		if (x%7==0) 				printf("│");
		if (d[x]==0)				printf("    ");
		else if (d[x]==tp->tm_mday) printf("<%2d>",d[x]);
		else						printf(" %2d ",d[x]);
		if (x%7==6) printf("│\n");
	}
	printf("└──────────────┘\n");
}


