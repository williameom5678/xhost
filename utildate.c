/*
timetoa(char *buff,time_t l)
{
  static char rtn[26];
  static char *weeks[7] = {"일","월","화","수","목","금","토"};
  struct tm *t;

  localtime(&l);
  t = time(&l);
  sprintf(rtn,"19%2d %2d %2d %2s %2d:%2d:%2d\n\r\0",t->tm_year%100,t->tm_mon+1,t->tm_mday,weeks[t->tm_wday],t->tm_hour,t->tm_min,t->tm_sec);
  strcpy (buff,rtn);
}
*/

time_week(int mode,char *buff,time_t t)
{
  char rtn[26];
  static char *weeks[7] = {"일","월","화","수","목","금","토"};
  struct tm *tp;
  tp = localtime(&t);
  strcpy (buff,weeks[tp->tm_wday]);
}


is_oldtime(time_t t,int n)
{
	time_t now;
	time(&now); //12311231
	return ((now-t)>n);
}


how_old(time_t t)
{
	time_t now;
	time(&now); //12311231
	return (now-t);
}

how_old_tty(char *dtty)
{
	char ttyfile[100];
	struct stat stbuf;
	struct utimebuf timestamp,*times=&timestamp;
	sprintf(ttyfile,"/dev/%s",dtty);
    strchng2(ttyfile,"/dev/dev/","/dev/");
    if (strlen(dtty)==0||stat(ttyfile,&stbuf)==-1) return 99999;
	times->actime =stbuf.st_atime;
	times->modtime=stbuf.st_mtime;
	utime(ttyfile,times);
    return how_old(stbuf.st_mtime);
}



//오늘지난날짜인가?
is_old_day(time_t t)
{
    struct tm *tp,*tp2;
	time_t now;
    time(&now);
    return (time_num(now)-time_num(t));
}


//오늘과의 날짜 차이?
time_day(time_t t)
{
	time_t now;
	time(&now); //12311231
	return ((now-t)/86400);
}

//19961231
time_num(time_t t)
{
	struct tm *tp;
	tp = localtime(&t);
    return ((tp->tm_year+1900)*10000+(tp->tm_mon+1)*100+tp->tm_mday);
}

//19961231
now_time_num()
{
	struct tm *tp;
	time_t today;
	time(&today);	//오늘의 날짜 지정
	tp = localtime(&today);
    return ((tp->tm_year+1900)*10000+(tp->tm_mon+1)*100+tp->tm_mday);
}

yearmonth(time_t t)     //9912
{
	struct tm *tp;
	tp = localtime(&t);
	return (((tp->tm_year%100)*100+tp->tm_mon+1)%10000);
}

now_yearmonth()
{
	time_t t;
	time(&t);
    return (yearmonth(t));
}


last_yearmonth(time_t t)
{
	struct tm *tp;
	tp = localtime(&t);
    return ( (tp->tm_year-1)*100+12 );
}


last_now_yearmonth()
{
    return ( (now_year()-1)*100+12 );
}


char *now_str(int mode)
{
	static char buff[50];
	time_t t;
	time(&t);
	strcpy(buff,datestr(mode,t));
	return (buff);
}


char *datestr(int mode,time_t t)
{
	static char buff[50];
	struct tm *tp;
	int n1,n2,n3;
	char buff1[20];
	char buff2[20];
	char buff3[20];
	static char *weeks[7] = {"일","월","화","수","목","금","토"};
	tp = localtime(&t);
		 if (mode== 0) sprintf(buff,"%02d-%02d-%02d",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
	else if (mode== 1) sprintf(buff,"%04d-%02d-%02d",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday);
	else if (mode== 2) sprintf(buff,"%02d-%02d-%02d %02d:%02d",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
	else if (mode== 3) sprintf(buff,"%02d-%02d %02d:%02d",tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
	else if (mode== 4) sprintf(buff,"%02d:%02d",tp->tm_hour,tp->tm_min);
	else if (mode== 5) sprintf(buff,"%02d%02d%02d",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
	else if (mode== 6) sprintf(buff,"%02d:%02d%c",tp->tm_hour%12,tp->tm_min,tp->tm_hour/12?'p':'a');
	else if (mode== 7) sprintf(buff,"%d:%02d",t/3600,(t/60)%60);
	else if (mode== 8) sprintf(buff,"%04d-%02d-%02d %02d:%02d",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
	else if (mode== 9) sprintf(buff,"%02d%02d%02d%02d%02d",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
	else if (mode==10) sprintf(buff,"%02d-%02d-%02d %02d:%02d:%02d",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
	else if (mode==11) sprintf(buff,"%02d%02d%02d%02d",tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
	else if (mode==12) sprintf(buff,"%02d%02d",tp->tm_year%100,tp->tm_mon+1);
	else if (mode==13) sprintf(buff,"%02d:%02d:%02d",tp->tm_hour,tp->tm_min,tp->tm_sec);
	else if (mode==14) sprintf(buff,"%cM %02d:%02d:%02d",tp->tm_hour>11?'P':'A',tp->tm_hour%12,tp->tm_min,tp->tm_sec);
	else if (mode==15) sprintf(buff,"%cM %02d:%02d",tp->tm_hour>11?'P':'A',tp->tm_hour%12,tp->tm_min);
    else if (mode==16) sprintf(buff,"%02d %02d:%02d%",tp->tm_mday,tp->tm_hour,tp->tm_min);
	else if (mode==17) sprintf(buff,"%02d-%02d",tp->tm_mon+1,tp->tm_mday);
	else if (mode==18) sprintf(buff,"%02d%02d",tp->tm_mon+1,tp->tm_mday);
	else if (mode==19) sprintf(buff,"%04d-%02d-%02d %02d:%02d:%02d",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
	else if (mode==24) strcpy(buff,datestr9(mode,tp->tm_year%100,tp->tm_mon+1,tp->tm_mday));
	else if (mode==25) strcpy(buff,datestr9(mode,tp->tm_hour,tp->tm_min,tp->tm_sec));
	else if (mode==20||mode==21||mode==22||mode==23||mode==26) {
		n1=t/3600;n2=(t/60)%60;n3=0;n3=t%60;
		strcpy(buff,datestr9(mode,n1,n2,n3));
	}
	else if (mode==27) sprintf(buff,"%04d%02d%02d",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday);
	else if (mode==31) sprintf(buff,"%02d%02d",tp->tm_hour%12,tp->tm_min);
	else if (mode==32) sprintf(buff,"%02d%02d",tp->tm_hour,tp->tm_min);
	else if (mode==33) sprintf(buff,"%04d-%02d-%02d(%s) %02d:%02d",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday],tp->tm_hour,tp->tm_min);
	else if (mode==34) sprintf(buff,"%4d년 %2d월 %2d일",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday);
	else if (mode==35) sprintf(buff,"%4d년 %2d월 %2d일 (%s)",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday]);
	else if (mode==36) sprintf(buff,"%4d년 %2d월 %2d일",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday);
	else if (mode==37) sprintf(buff,"%4d년 %2d월 %2d일 (%s)",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday]);
	else if (mode==38) sprintf(buff,"%4d년 %2d월 %2d일 %s시 %2d분",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,ampm(1,tp->tm_hour),tp->tm_min);
	else if (mode==39) sprintf(buff,"%4d년 %2d월 %2d일 (%s) %s시 %2d분",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday],ampm(1,tp->tm_hour),tp->tm_min);
	else if (mode==40) sprintf(buff,"%s시 %d분",ampm(1,tp->tm_hour),tp->tm_min);
	else if (mode==41) sprintf(buff,"%s %2d시 %2d분",ampm(5,tp->tm_hour),tp->tm_hour%12,tp->tm_min);
    else if (mode==42) sprintf(buff,"%04d-%02d-%02d(%s) %02d:%02d:%02d",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday],tp->tm_hour,tp->tm_min,tp->tm_sec);
    else if (mode==43) {if(t<0) t=0;sprintf(buff,"%3d:%02d",t/3600,(t/60)%60);}
    else if (mode==44) sprintf(buff,"%4d년 %2d월",tp->tm_year+1900,tp->tm_mon+1);
    else if (mode==45) sprintf(buff,"%2d년 %2d월",tp->tm_year%100,tp->tm_mon+1);
    else if (mode==46) sprintf(buff,"%02d%02d%02d%02d%02d%02d",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    else if (mode==47) sprintf(buff,"%2d년 %2d월 %2d일 (%s)",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday]);
    else if (mode==48) sprintf(buff,"%2d년%2d월%2d일",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday);
    else if (mode==49) sprintf(buff,"%2d년%2d월%2d일(%s)",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday]);
    else if (mode==50) sprintf(buff,"%2d-%2d-%2d(%s)",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday]);
    else if (mode==51) sprintf(buff,"%02d%02d %02d%02d%02d",tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    else if (mode==52) sprintf(buff,"%04d년 %2d월 %2d일 (%s) %s시 %2d분 %2d초",tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,weeks[tp->tm_wday],ampm(1,tp->tm_hour),tp->tm_min,tp->tm_sec);
    else if (mode==53) sprintf(buff,"%d월 %d일 %d시",tp->tm_mon+1,tp->tm_mday,tp->tm_hour);
    else if (mode==54) sprintf(buff,"%04d-%02d",tp->tm_year+1900,tp->tm_mon+1);
    else if (mode==55) sprintf(buff,"%02d-%02d",tp->tm_year%100,tp->tm_mon+1);
	else if (mode==81) sprintf(buff,"%d",tp->tm_year+1900);
    else if (mode==82) sprintf(buff,"%d",tp->tm_year%100);
	else if (mode==83) sprintf(buff,"%d",tp->tm_mon+1);
    else if (mode==84) sprintf(buff,"%d",tp->tm_mday);
	else if (mode==85) sprintf(buff,"%s",weeks[tp->tm_wday]);
	else if (mode==86) sprintf(buff,"%d",tp->tm_hour);
	else if (mode==87) sprintf(buff,"%c",tp->tm_hour>11?'P':'A');
	else if (mode==88) sprintf(buff,"%d",tp->tm_hour%12);
	else if (mode==89) sprintf(buff,"%d",tp->tm_min);
	else if (mode==90) sprintf(buff,"%d",tp->tm_sec);
	else if (mode==93) sprintf(buff,"%2d",tp->tm_mon+1);
    else if (mode==94) sprintf(buff,"%2d",tp->tm_mday);
	else if (mode==95) sprintf(buff,"%2d",tp->tm_hour);
	else if (mode==96) sprintf(buff,"%2d",tp->tm_hour%12);
	else if (mode==97) sprintf(buff,"%2d",tp->tm_min);
	else if (mode==98) sprintf(buff,"%2d",tp->tm_sec);
	return (buff);
}


char *ampm(int mode,int hour)
{
	static char buff[20];
	strcpy(buff,"");
		 if (mode==0) sprintf(buff,"%s%2d" ,hour>11?"P":"A",hour%12);
	else if (mode==1) sprintf(buff,"%s %2d",hour>11?"오후":"오전",hour%12);
	else if (mode==2) sprintf(buff,"%s %2d",hour>11?"PM":"AM",hour%12);
	else if (mode==3) sprintf(buff,"%s %d" ,hour>11?"pm":"am",hour%12);
	else if (mode==4) sprintf(buff,"%s",hour>11?"P":"A");
	else if (mode==5) sprintf(buff,"%s",hour>11?"오후":"오전");
	else if (mode==6) sprintf(buff,"%s",hour>11?"PM":"AM");
	else if (mode==7) sprintf(buff,"%s",hour>11?"pm":"am");
	return (buff);
}

char *datestr9(int mode,int n1,int n2,int n3)
{
	static char buff[30];
	char buff1[100],buff2[100],buff3[100],buff4[100];
	strcpy(buff,"");
	strcpy(buff1,"");
	strcpy(buff2,"");
	strcpy(buff3,"");
	if (mode==20) {
		if (n1) sprintf(buff1,"%d시간 ",n1);
		if (n2) sprintf(buff2,"%d분 "  ,n2);
		if (n3) sprintf(buff3,"%d초 "  ,n3);
		if (n1==0&&n2==0&&n3==0) sprintf(buff3,"0초");
	}
	else if (mode==21) {
		strcpy(buff1,"");strcpy(buff2,"");
		if (n1) sprintf(buff1,"%d시간 ",n1);
		if (n2) sprintf(buff2,"%d분 "  ,n2);
		if (n1==0&&n2==0) sprintf(buff2,"0분");
	}
	else if (mode==22) {
		sprintf(buff1,"%d:%02d",n1,n2);
	}
	else if (mode==23) {
		strcpy(buff1,"   ");strcpy(buff2,"   ");
		if (n1>0)		sprintf(buff1,"%02d:",n1);
		if (n1>0||n2>0) sprintf(buff2,"%02d:",n2);
						sprintf(buff3,"%02d" ,n3);
	}
	else if (mode==24) {
		sprintf(buff1,"%d년 ",n1);
		sprintf(buff2,"%d월 ",n2);
		sprintf(buff3,"%d일" ,n3);
	}
	else if (mode==25) {
		sprintf(buff1,"%d시 ",n1);
		sprintf(buff2,"%d분",n2);
	}
	else if (mode==26) {
		if ((n1/24)>0) sprintf(buff1,"%d:",n1/24);
		if (n1%24>0||n2>0) {
			sprintf(buff2,"%02d:",n1%24);
			sprintf(buff3,"%02d",n2);
		}
	}
	sprintf(buff,"%s%s%s",buff1,buff2,buff3);
	return (buff);
}



day_calc(time_t time_t1,time_t time_t2)
{
	return (time_t1-time_t2)/(24*3600);
}


char *now_week()   {
	static char buff[30];
	time_t t;
	time(&t); //12311231
	time_week(0,buff,t);
	return (buff);
}

char *date_week(time_t date2)  {
	static char buff[30];
	time_week(0,buff,date2);
	return (buff);
}


//현재년도
now_now(int mode,int mode2,time_t date2)
{
	struct tm *tp;
	time_t t;
	int i;
	if (mode2==0) {
		time(&t); //12311231
		tp = localtime(&t); //12311231
	}
	else tp = localtime(&date2); //12311231

    if      (mode==1&&tp->tm_year<=100) return tp->tm_year+1900;
    else if (mode==1) return tp->tm_year;
	else if (mode==2) return tp->tm_mon+1;
	else if (mode==3) return tp->tm_mday;
	else if (mode==4) return tp->tm_hour;
	else if (mode==5) return tp->tm_min;
	else if (mode==15) return tp->tm_sec;
	else if (mode==6) return tp->tm_wday;
	else if (mode==7) return (tp->tm_year*10000+(tp->tm_mon+1)*100+tp->tm_mday);
	else if (mode==8) return ((tp->tm_mon+1)*100+tp->tm_mday);
	else if (mode==9) return ((tp->tm_mon+1)*31+tp->tm_mday);
	else if (mode==10) {
        i=tp->tm_year;
        if (i<=100&&i>50) i+=1900;
        else if (i<=100) i+=2000;
		if(i>=1998) i-=1998;
		if(i<0||i>1000) i=0;
		return (i*12+tp->tm_mon+1);
	}
	else if (mode==11) return (tp->tm_year*100+(tp->tm_mon+1));
	return 0;
}


//날짜를 변환시킴 시간은 그대로 현재의 시간으로..
//	  time_t t; t=set_date(99,1,1); printf("%s  %d\n",ctime(&t),t);
time_t set_date(int year, char mon, char day) {
	time_t t;
	struct tm *tp;

	time(&t);
	tp= localtime(&t);
    if (year<=100&&year>50) year+=1900;
    else if (year<=100) year+=2000;
	tp->tm_year= year;
	tp->tm_year= year-1900;
	tp->tm_mon = (int)mon-1;
	tp->tm_mday= (int)day;
	t= mktime(tp);
	return t;
}


//날짜를 변환시킴
//	  YYYY/MM/DD  , YY/MM/DD
time_t str2date(char *buff) {
	int year;
	int mon;
	int day;
	char buff2[100];
	bzero(buff2,99);
	if (buff[4]=='/'||buff[4]=='-') {
		strcpy(buff2,buff)	  ;buff2[4]=0;year=atoi(buff2);
		strcpy(buff2,&buff[5]);buff2[4]=0;mon =atoi(buff2);
		strcpy(buff2,&buff[8]);buff2[2]=0;day =atoi(buff2);
	}
	else {
		strcpy(buff2,buff)	  ;buff2[2]=0;year=atoi(buff2);
		strcpy(buff2,&buff[3]);buff2[2]=0;mon =atoi(buff2);
		strcpy(buff2,&buff[6]);buff2[2]=0;day =atoi(buff2);
	}
	if (year<0||mon<1||mon>12||day<1||day>31) return 0;
	return(set_date(year,mon,day));
}


time_t set_date2(int year, char mon, char day,char hour,char min) {
	time_t t;
	struct tm *tp;

	time(&t);
	tp= localtime(&t);
    if (year<=100&&year>50) year+=1900;
    else if (year<=100) year+=2000;
	tp->tm_year= year-1900;
	tp->tm_mon = (int)mon-1;
	tp->tm_mday= (int)day;
    tp->tm_hour= (int)hour;
	tp->tm_min = (int)min;
	t=mktime(tp);
	return t;
}


//날짜를 변환시킴
//	  YYYY-MM-DD 12:31	, YY-MM-DD 12:31
time_t str2date2(char *buff) {
    int year,mon,day,hour,min,sec;
	char buff2[100];
	time_t t;
	struct tm *tp;
	time(&t);
	tp= localtime(&t);
	bzero(buff2,99);
	if (buff[4]=='/'||buff[4]=='-') {
		strcpy(buff2,buff)	   ;buff2[4]=0;year=atoi(buff2);
		strcpy(buff2,&buff[ 5]);buff2[4]=0;mon =atoi(buff2);
		strcpy(buff2,&buff[ 8]);buff2[2]=0;day =atoi(buff2);
		strcpy(buff2,&buff[11]);buff2[2]=0;hour=atoi(buff2);
		strcpy(buff2,&buff[14]);buff2[2]=0;min =atoi(buff2);
        strcpy(buff2,&buff[17]);buff2[2]=0;sec =atoi(buff2);
	}
	else {
		strcpy(buff2,buff)	   ;buff2[2]=0;year=atoi(buff2);
		strcpy(buff2,&buff[3 ]);buff2[2]=0;mon =atoi(buff2);
		strcpy(buff2,&buff[6 ]);buff2[2]=0;day =atoi(buff2);
		strcpy(buff2,&buff[9 ]);buff2[2]=0;hour=atoi(buff2);
		strcpy(buff2,&buff[12]);buff2[2]=0;min =atoi(buff2);
        strcpy(buff2,&buff[15]);buff2[2]=0;sec =atoi(buff2);
	}
	if (year<0||mon<1||mon>12||day<1||day>31) return 0;
    if (year<=100&&year>50) year+=1900;
    else if (year<=100) year+=2000;
	tp->tm_year= year-1900;
    tp->tm_mon = mon-1;
    tp->tm_mday= day;
    tp->tm_hour= hour;
    tp->tm_min = min;
    tp->tm_sec = sec;
	t=mktime(tp);
    return(t);
}


//날짜를 변환시킴
//	  MM/DD 12:31
time_t str2date3(char *buff) {
	char buff2[100];
	sprintf(buff2,"%4d-%s",now_year(),buff);
	return str2date2(buff2);
}

//날짜를 변환시킴
//	  YYYYMMDD,YYMMDD ->날짜로 지정
//    YYYYMMDDHHMMSS,YYMMDDHHMMSS ->날짜로 지정
//    YYYYMMDDHHMMSS,YYMMDDHHMMSS ->날짜로 지정
//    YYMMDDHHMM ->날짜로 지정
time_t str2date4(char *buff) {
    static char buff2[100];
         if (strlen(buff)== 8) sprintf(buff2,"%c%c%c%c-%c%c-%c%c",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7]);
    else if (strlen(buff)== 6) sprintf(buff2,"%c%c-%c%c-%c%c",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5]);
    else if (strlen(buff)==12) sprintf(buff2,"%c%c-%c%c-%c%c %c%c:%c%c:%c%c",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7],buff[8],buff[9],buff[10],buff[11]);
    else if (strlen(buff)==14) sprintf(buff2,"%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7],buff[8],buff[9],buff[10],buff[11],buff[12],buff[13]);
    else if (strlen(buff)==10) sprintf(buff2,"%c%c-%c%c-%c%c %c%c:%c%c",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7],buff[8],buff[9]);
	return str2date2(buff2);
}


char *yangstr(int mode,int moon)
{
	static char buff9[10];
	if (mode==1) {
        if (moon==2) strcpy(buff9,"(음력)");
        else         strcpy(buff9,"(양력)");
	}
	else if (mode==2) {
        if (moon==2) strcpy(buff9,"(음)");
        else         strcpy(buff9,"(양)");
	}
	else {
        if (moon==2) strcpy(buff9,"음");
        else         strcpy(buff9,"양");
	}
	return (buff9);
}


char *dateyang(int mode,time_t date,int moon)
{
	static char buff[50];
    strcpy(buff,dateyang_(mode/10,mode%10,date,moon));
	return (buff);
}


char *dateyang_(int mode1,int mode2,time_t date,int moon)
{
	static char buff[50];
	char buff2[50];
	strcpy(buff2,datestr(mode1,date));
	sprintf(buff,"%s%s",buff2,yangstr(mode2,moon));
	return (buff);
}

//현재시간이 포함되었는가?
in_hour(int n1,int n2)
{
	int n=now_hour();
	if (n1==0&&n2==0) return Yes;
	else if (n1==0&&n>=n1) return Yes;
	else if (n2==0&&n<=n2) return Yes;
	else if (n1==n2&&n==n2) return Yes;
	else if (n>=n1&&n<n2) return Yes;
	return No;
}


//진입가능 시간 확인
char *time_limitstr(int intime)
{
	static char buff[100];
	int now,start,end;
	start=intime/10000;  //시작시간
	end  =intime%10000;  //종결시간
	sprintf(buff,"%02d:%02d ∼ %02d:%02d",start/100,start%100,end/100,end%100);
	return (buff);
}
