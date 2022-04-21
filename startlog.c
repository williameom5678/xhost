////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
char *moon();

//컴파일 방법 //(수학함수link)  cc startlog.o util.o -o startlog -lm
#include <math.h>

startlog()
{
    int n,n2,yy,mm,dd,hour,min,sec;
    char buff[100];
    char buff2[100];
    time_t t;

    calc_date("20000101000000");
    if (now_year()<2000) {
//        printf(" [7m[밀레니엄] 2000년이 앞으로 %s 남았습니다.[m\n\n",g_buff9);
    }
    c_proc=No;
    t = time(NULL);
    if (strlen(pf.id)<3||pf.type<1) return;
	n=now_year();
    if (n<=100) n+=1900;
	n+=2333;
    if (!bit(cfg.startmode,4)&&pf.type>0) printf(" [최근사용] %s\n",datestr(42,pf.logout));
    if (pf.type>0&&pf.birthday!=0) xbio(pf.birthday);
    if (!bit(cfg.startmode,7)&&pf.type>0) printf(" [사용시간] %s (%d월1일부터)\n",datestr(20,pf.month_time),now_month());
    if (!bit(cfg.startmode,8)&&pf.type>0) {
        n2=check_new_mail();
        if (n2>0)
        printf(" [편지확인] 수신된 편지가 [7m%d통[m 있습니다.\n",n2);
        if (bit(cfg.mailmode,6)) printf("\007");
    }
    if (pf.type>0&&pf.birthday!=0&&check_birthday()) view("frm/log/birthday.log");
}



char *moon(time_t t)
{
	static char outbuff[100];
	static char kk[163][13] = {

	  /*1881*/	 1, 2, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1,
				 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 0,
				 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 0,
				 2, 1, 1, 2, 1, 3, 2, 1, 2, 2, 1, 2, 2,
				 2, 1, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 0,
				 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 0,
				 2, 2, 1, 2, 3, 2, 1, 1, 2, 1, 2, 1, 2,
				 2, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 1, 0,
				 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0,
				 1, 2, 3, 2, 1, 2, 2, 1, 2, 1, 2, 1, 2,

	  /*1891*/	 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 0,
				 1, 1, 2, 1, 1, 2, 3, 2, 2, 1, 2, 2, 2,
				 1, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 0,
				 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 0,
				 2, 1, 2, 1, 2, 3, 1, 2, 1, 2, 1, 2, 1,
				 2, 2, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 0,
				 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
				 2, 1, 2, 3, 2, 2, 1, 2, 1, 2, 1, 2, 1,
				 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 0,
				 1, 2, 1, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2,

	  /*1901*/	 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 1, 0,
				 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 0,
				 1, 2, 1, 2, 1, 3, 2, 1, 1, 2, 2, 1, 2,
				 2, 2, 1, 2, 1, 1, 2, 1, 1, 2, 2, 1, 0,
				 2, 2, 1, 2, 2, 1, 1, 2, 1, 2, 1, 2, 0,
				 1, 2, 2, 1, 4, 1, 2, 1, 2, 1, 2, 1, 2,
				 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 0,
				 2, 1, 1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 0,
				 1, 2, 3, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2,
				 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 1, 0,

	  /*1911*/	 2, 1, 2, 1, 1, 2, 3, 1, 2, 2, 1, 2, 2,
				 2, 1, 2, 1, 1, 2, 1, 1, 2, 2, 1, 2, 0,
				 2, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 0,
				 2, 2, 1, 2, 2, 3, 1, 2, 1, 2, 1, 1, 2,
				 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0,
				 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 0,
				 2, 1, 3, 2, 1, 2, 2, 1, 2, 2, 1, 2, 1,
				 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2, 0,
				 1, 2, 1, 1, 2, 1, 2, 3, 2, 2, 1, 2, 2,
				 1, 2, 1, 1, 2, 1, 1, 2, 2, 1, 2, 2, 0,

	  /*1921*/	 2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 0,
				 2, 1, 2, 2, 1, 3, 2, 1, 1, 2, 1, 2, 2,
				 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 0,
				 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 1, 0,
				 2, 1, 2, 2, 3, 2, 1, 2, 2, 1, 2, 1, 2,
				 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 0,
				 2, 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 0,
				 1, 2, 3, 1, 2, 1, 1, 2, 2, 1, 2, 2, 2,
				 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 0,
				 1, 2, 2, 1, 1, 2, 3, 1, 2, 1, 2, 2, 1,

	  /*1931*/	 2, 2, 2, 1, 1, 2, 1, 1, 2, 1, 2, 1, 0,
				 2, 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 0,
				 1, 2, 2, 1, 2, 4, 1, 2, 1, 2, 1, 1, 2,
				 1, 2, 1, 2, 2, 1, 2, 2, 1, 2, 1, 2, 0,
				 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 0,
				 2, 1, 1, 4, 1, 2, 1, 2, 1, 2, 2, 2, 1,
				 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 0,
				 2, 2, 1, 1, 2, 1, 1, 4, 1, 2, 2, 1, 2,
				 2, 2, 1, 1, 2, 1, 1, 2, 1, 2, 1, 2, 0,
				 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 0,

	  /*1941*/	 2, 2, 1, 2, 2, 1, 4, 1, 1, 2, 1, 2, 1,
				 2, 1, 2, 2, 1, 2, 2, 1, 2, 1, 1, 2, 0,
				 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 2, 0,
				 1, 1, 2, 1, 4, 1, 2, 1, 2, 2, 1, 2, 2,
				 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 0,
				 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 0,
				 2, 2, 3, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2,
				 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 0,
				 2, 2, 1, 2, 1, 2, 1, 3, 2, 1, 2, 1, 2,
				 2, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 1, 0,

	  /*1951*/	 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0,
				 1, 2, 1, 2, 1, 4, 2, 1, 2, 1, 2, 1, 2,
				 1, 2, 1, 1, 2, 2, 1, 2, 2, 1, 2, 2, 0,
				 1, 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 2, 0,
				 2, 1, 1, 4, 1, 1, 2, 1, 2, 1, 2, 2, 2,
				 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 0,
				 2, 1, 2, 1, 2, 1, 1, 2, 3, 2, 1, 2, 2,
				 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 0,
				 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
				 2, 1, 2, 1, 2, 2, 3, 2, 1, 2, 1, 2, 1,

	  /*1961*/	 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 0,
				 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 2, 1, 0,
				 2, 1, 2, 1, 3, 2, 1, 2, 1, 2, 2, 2, 1,
				 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 0,
				 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 2, 1, 0,
				 2, 2, 2, 3, 2, 1, 1, 2, 1, 1, 2, 2, 1,
				 2, 2, 1, 2, 2, 1, 1, 2, 1, 2, 1, 2, 0,
				 1, 2, 2, 1, 2, 1, 2, 3, 2, 1, 2, 1, 2,
				 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 0,
				 2, 1, 1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 0,

	  /*1971*/	 1, 2, 1, 1, 2, 3, 2, 1, 2, 2, 2, 1, 2,
				 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 1, 0,
				 2, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 1, 0,
				 2, 2, 1, 2, 3, 1, 2, 1, 1, 2, 2, 1, 2,
				 2, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 0,
				 2, 2, 1, 2, 1, 2, 1, 2, 3, 2, 1, 1, 2,
				 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1, 0,
				 2, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 0,
				 2, 1, 1, 2, 1, 2, 4, 1, 2, 2, 1, 2, 1,
				 2, 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 0,

	  /*1981*/	 1, 2, 1, 1, 2, 1, 1, 2, 2, 1, 2, 2, 0,
				 2, 1, 2, 1, 3, 2, 1, 1, 2, 2, 1, 2, 2,
				 2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 0,
				 2, 1, 2, 2, 1, 1, 2, 1, 1, 2, 3, 2, 2,
				 1, 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 0,
				 1, 2, 2, 1, 2, 2, 1, 2, 1, 2, 1, 1, 0,
				 2, 1, 2, 2, 1, 2, 3, 2, 2, 1, 2, 1, 2,
				 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 0,
				 2, 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 0,
				 1, 2, 1, 1, 2, 3, 1, 2, 1, 2, 2, 2, 2,

	  /*1991*/	 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 0,
				 1, 2, 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 0,
				 1, 2, 2, 3, 2, 1, 2, 1, 1, 2, 1, 2, 1,
				 2, 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 0,
				 1, 2, 2, 1, 2, 2, 1, 2, 3, 2, 1, 1, 2,
				 1, 2, 1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 0,
				 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 0,
				 2, 1, 1, 2, 1, 3, 2, 2, 1, 2, 2, 2, 1,
				 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 0,
				 2, 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 1, 0,

	  /*2001*/	 2, 2, 2, 1, 3, 2, 1, 1, 2, 1, 2, 1, 2,
				 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 0,
				 2, 2, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 0,
				 1, 2, 3, 2, 2, 1, 2, 1, 2, 2, 1, 1, 2,
				 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 2, 0,
				 1, 1, 2, 1, 2, 1, 2, 3, 2, 2, 1, 2, 2,
				 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 0,
				 2, 1, 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 0,
				 2, 2, 1, 1, 2, 3, 1, 2, 1, 2, 1, 2, 2,
				 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 0,

	  /*2011*/	 2, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 1, 0,
				 2, 1, 2, 4, 2, 1, 2, 1, 1, 2, 1, 2, 1,
				 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0,
				 1, 2, 1, 2, 1, 2, 1, 2, 2, 3, 2, 1, 2,
				 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 0,
				 1, 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 2, 0,
				 2, 1, 1, 2, 1, 3, 2, 1, 2, 1, 2, 2, 2,
				 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 0,
				 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 0,
				 2, 1, 2, 2, 3, 2, 1, 1, 2, 1, 2, 1, 2,

	  /*2021*/	 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
				 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 0,
				 1, 2, 3, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2,
				 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 2, 1, 0,
				 2, 1, 2, 1, 1, 2, 3, 2, 1, 2, 2, 2, 1,
				 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 0,
				 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 0,
				 1, 2, 2, 1, 2, 3, 1, 2, 1, 1, 2, 2, 1,
				 2, 2, 1, 2, 2, 1, 1, 2, 1, 1, 2, 2, 0,
				 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 0,

	  /*2031*/	 2, 1, 2, 3, 2, 1, 2, 2, 1, 2, 1, 2, 1,
				 2, 1, 1, 2, 1, 2, 2, 1, 2, 2, 1, 2, 0,
				 1, 2, 1, 1, 2, 1, 2, 3, 2, 2, 2, 1, 2,
				 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 2, 1, 0,
				 2, 1, 2, 1, 1, 2, 1, 1, 2, 2, 1, 2, 0,
				 2, 2, 1, 2, 1, 1, 4, 1, 1, 2, 1, 2, 2,
				 2, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 0,
				 2, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 0,
				 2, 2, 1, 2, 2, 3, 2, 1, 2, 1, 2, 1, 1,
				 2, 1, 2, 2, 1, 2, 2, 1, 2, 1, 2, 1, 0,

	  /*2041*/	 2, 1, 1, 2, 1, 2, 2, 1, 2, 2, 1, 2, 0,
				 1, 2, 3, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2,
				 1, 2, 1, 1, 2, 1, 1, 2, 2, 1, 2, 2, 0 };
	  int syear, smonth, sday, shour, smin, ssec;	   /* year, month, day */
	  int lyear, lmonth, lday;		/* calculated solar date */
	  int m1, m2, i, j, i1, j1, jcount, ll, w, m0;
	  int dt[163];
      char yun[20];
	  long td, td0, td1, td2, k11;
	  static m[12] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	  char temp[10];
      struct tm *tp;

      tp = localtime(&t);
      for(i=0; i<163; i++) {
		 dt[i] = 0;
		 for(j=0; j<12; j++) {
			switch(kk[i][j])
			{
				case 1:
				case 3:
					 dt[i] = dt[i] + 29;
					 break;
				case 2:
				case 4:
					 dt[i] = dt[i] + 30;
			}
		 }
		 switch(kk[i][12])
		 {
			case 0:
				 break;
			case 1:
			case 3:
				 dt[i] = dt[i] + 29;
				 break;
			case 2:
			case 4:
				 dt[i] = dt[i] + 30;
				 break;
		 }
	  }

	  /* 1. 1. 1. - 1910. 2. 10. */
	  td1 = 1880*365L + 1880/4 - 1880/100 + 1880/400 + 30;

	  syear = tp->tm_year;
	  smonth = tp->tm_mon+1;
	  sday = tp->tm_mday;
	  shour = tp->tm_hour;
	  smin = tp->tm_min;
	  ssec = tp->tm_sec;

      if(syear <=100) syear+=1900;

	  /* ## 1. 1. 1. - syear. smonth. sday. ## */
	  k11 = (long)(syear-1);
	  td2 = k11*365L + k11/4L - k11/100L + k11/400L;
	  ll = syear%400==0 || syear%100!=0 && syear%4==0;

	  if(ll) m[1] = 29;
	  else	 m[1] = 28;
	  if( sday > m[smonth-1] ) {				   /* Input */
		 printf("\nInput data error !!");  /* Data  */
         return (outbuff);                        /* Check */
	  }
	  for(i=0; i<smonth-1; i++) td2 = td2 + (long)m[i];
	  td2 = td2 + (long)sday;

	  /* ## 1881. 1. 30. - syear. smonth. sday. ## */
      td = td2 - td1 + 1;

	  /* ## Lunar Year Caculation ## */
	  td0 = (long)dt[0];
	  for(i=0; i<163; i++) {
		if( td <= td0 ) break;
		td0 = td0 + (long)dt[i+1];
	  }
	  lyear = i + 1881;  /* Calculated Lunar Year */

	  /* ## Lunar Month Calculation ## */
	  td0 = td0 - (long)dt[i];
	  td = td - td0;
	  if(kk[i][12] != 0) jcount = 13;
	  else				 jcount = 12;
	  m2 = 0;
	  for(j=0; j<jcount; j++) {
		 if( kk[i][j] <=2 ) m2++;
		 if( kk[i][j] <=2 ) m1 = kk[i][j] + 28;
		 else				m1 = kk[i][j] + 26;
		 if( td <= (long)m1 ) break;
		 td = td - (long)m1;
	  }
	  m0 = j;
	  lmonth = m2;	/* Calculated Lunar Month */

      lday = td;    /* Calculated Lunar Day */

	  i = (int)( (td2+4L) % 10L);
	  j = (int)( (td2+2L) % 12L);
	  i1 = ( lyear + 6 ) % 10;
	  j1 = ( lyear + 8 ) % 12;

      strcpy(yun,"");
      if( (int)kk[lyear-1881][12] != 0 && (int)kk[lyear-1881][m0] > 2 ) strcpy(yun,"윤년");
      sprintf(outbuff,"%d-%02d-%02d",lyear, lmonth, lday);
	  return(outbuff);
}



//메일 도착 확인
check_new_mail()
{
    FILE *fp;
    int num=0,count=0;
    char filename[320];
	if (guest_mode) return 0;
    sprintf(filename,"userdata/%s/%s/letter.tit",c_han(pf.id),pf.id);
    if ((fp=fopen(filename,"r"))!=NULL) {
		num=0;
        while (num<1000&&count<10000&&fread(&bbs,sizeof_bbs,1,fp)) {
            count++;
            if (!bbs.del&&bbs.read<=0&&strlen(bbs.title)>0&&strlen(bbs.id)>2) num++;
		}
        fclose(fp);
	}
	return num;
}

/*

 [생체리듬] 신체지수: -52(▲)  감성지수: 43(▲)  지성지수: -69(▲)
            신체적으로나 지성적으로 저조한 날. 창조적인 일에
            몰두하면 가장 좋다. 감성리듬의 이점을 최대한 활용
            할 것.
*/


check_birthday()
{
    char buff[100];
    char buff2[100];
    time_t t;

    t = time(NULL);
    if (pf.moon<=1) {
        strcpy(buff,now_str(17));
        if (is_same(buff,datestr(17,pf.birthday))) return Yes;
    }
    else {
        strcpy(buff,moon(t));
        strcpy(buff2,&buff[5]);
        if (is_same(buff2,datestr(17,pf.birthday))) return Yes;
    }
    return No;
}


xbio(time_t birthday)
{
    FILE *fp;
    int BY,BM,BD;
    int i,e,j, k=0, p, SY=0, SM=0, SD=0;
	int PX, EX, IX, life, day366[13][32];
	int month[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};	
	int pastpx, pastex, pastix;
    struct tm *tp;
	time_t t;
    char buff[200];

    BY=date_year(birthday);
    BM=date_month(birthday);
    BD=date_day(birthday);
	t = time(NULL);
    tp = localtime(&t);

	for(i = 1 ; i <= 12; i++)
	{
	    for(j = 1; j <= month[i]; j++) 
	    {
		   	k++;
			day366[i][j] = k;	      
	   	}
	}

    SY = now_year();
    SM = now_month();
    SD = now_day();

    if (BY<=100) BY+=1900;
    if (SY<=100) SY+=1900;
	
    if(SY == BY) {
		life = day366[SM][SD] - day366[BM][BD] + 1;
		if((SY%4!=0) && (day366[SM][SD]>60) && (day366[BM][BD]<60))
			life--;
	}
    else {
		life = (SY-BY-1) * 365;
		life += (366 - day366[BM][BD] + 1);
		if((BY%4!=0) && (day366[BM][BD]<60)) 
			life--;
		life += (day366[SM][SD]);
		if((SY % 4 != 0)&&(day366[SM][SD]>60)) 
			life--;
		if(SY-BY-1 >= 1)
		{
			for(i=BY+1;i<=SY-1;i++)
			{
				if(i%4==0) 
					life++;         
			}
		}	
	}

	SD--;
	life -= 2;

    SD++;
    life++;
    if ((SM == 12)&&(SD == 32)) {
    	SY++;
    	SM = 1;
    	SD = 1;
    	goto label_1;
    }
    
    if (SY % 4 == 0) 
    	month[2]=29;
    else 
    	month[2]=28;
    
    if (SD > month[SM]) 
    {
    	SM++;
    	SD=1;
    }
    

label_1:
    pastpx=100*sin((2*M_PI)/23*((life-1)%23));
    pastex=100*sin((2*M_PI)/28*((life-1)%28));
    pastix=100*sin((2*M_PI)/33*((life-1)%33));
	    
    PX=100*sin(((2*M_PI)/23)*(life%23));
    EX=100*sin((2*M_PI)/28*(life%28));
    IX=100*sin((2*M_PI)/33*(life%33));
    p=PX/100;
    e=EX/100;
    i=IX/100;

	// pastxx가 1이면 저조기, 2이면 고조기, 0이면 중간이다.

    // -99 -70   : 1
    // -69 -10   : 2
    // -9    9   : 3
    // 10   69   : 4
    // 70   99   : 5
    
	fflush(stdout);
    if (!bit(cfg.startmode,9)) printf(" [생년월일] %d년 %d월 %d일   살아온날:%d일 살아온시간:%d시간\n",BY,BM,BD,life,life*24);
    if (!bit(cfg.startmode,6)) {
        printf(" [생체리듬] ");
        if(pastpx > PX) printf("신체지수: %d(▼)  ", PX);
        else            printf("신체지수: %d(▲)  ", PX);
        if(pastex > EX) printf("감성지수: %d(▼)  ", EX);
        else            printf("감성지수: %d(▲)  ", EX);
        if(pastix > IX) printf("지성지수: %d(▼)\n", IX);
        else            printf("지성지수: %d(▲)\n", IX);
    }
         if (PX <= -70)          pastpx = 1;
    else if (PX <= -10)          pastpx = 2;
    else if (PX <=   9)          pastpx = 3;
    else if (PX <=  69)          pastpx = 4;
    else if (PX <= 999)          pastpx = 5;

         if (EX <= -70)          pastex = 1;
    else if (EX <= -10)          pastex = 2;
    else if (EX <=   9)          pastex = 3;
    else if (EX <=  69)          pastex = 4;
    else if (EX <= 999)          pastex = 5;

         if (IX <= -70)          pastix = 1;
    else if (IX <= -10)          pastix = 2;
    else if (IX <=   9)          pastix = 3;
    else if (IX <=  69)          pastix = 4;
    else if (IX <= 999)          pastix = 5;

    if (!bit(cfg.startmode,10)) {
        sprintf(buff,"frm/bio/%d%d%d.txt",pastpx,pastex,pastix);
        if ((fp=fopen(buff,"r"))!=NULL) {
            fgets(buff,200,fp);
            fgets(buff,200,fp);
            while(fgets(buff,200,fp)) printf(buff);
            fclose(fp);
        }
        fflush(stdout);
    }
}


