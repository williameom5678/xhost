int is_file(const char *fmt,...)
{
	char buff[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	if (strlen(buff)<1) return No;
	return (access(buff,0)!=-1);
}

void check_dir(const char *fmt,...)
{
	char buff[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	if (strlen(buff)>0&&!is_file(buff)) mkdir3(buff);
}


int view(const char *fmt,...)
{
	char buff[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	if (bit(allmode,11)) {
		printf("*LOGO=%s(%dB)*",buff,file_size(buff));
		getchar();
	}
    if (strlen(buff)<1||buff[strlen(buff)-1]=='/') return No;
	return view_(1,buff,"",0);
}


mview(int mode,const char *fmt,...)
{
	char filename[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(filename,fmt, ap);
	va_end(ap);
	if (!is_file(filename)) return;
		 if (stristr(filename,".jpg")) view_jpg(filename,mode);
	else if (stristr(filename,".vtx")) view_vtx(filename,mode);
	else if (stristr(filename,".ra"))  view_ra(filename,mode);
	else if (stristr(filename,".rm"))  view_ra(filename,mode);
	else if (stristr(filename,".wav")) view_ra(filename,mode);
	else if (stristr(filename,".mp2")) view_ra(filename,mode);
	else if (stristr(filename,".mp3")) view_ra(filename,mode);
	else view(filename);
}


sys_tem(const char *fmt,...)
{
	char buff[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	return sys_tem_(buff,1);
}

sys_tem2(const char *fmt,...)
{
	char buff[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	return sys_tem_(buff,2);
}

sys_tem3(const char *fmt,...)
{
	char buff[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	return sys_tem_(buff,3);
}


view_jpg(char *filename,int mode)
{
	char buff[200];
	if (is_file(filename)&&stristr(filename,".jpg")) {
		strcpy(buff,cfg.jpgstr);
		strconv3(buff);
		if (!bit(allmode,24)) view3(buff);		//printf("7\n8[5;5H");
		if (!bit(allmode,25)) sys_tem("%s '%s'",cfg.jpgsz,filename);
		else				  sys_tem("sz '%s'",filename);
		if (mode) getchar3();
	}
}


view_ra(char *filename,int mode)
{
	if (strlen(filename)>0&&is_file(filename)) {
		sys_tem("%s '%s'",cfg.sz,filename);
		if (mode) getchar3();
	}
}


view_vtx(char *filename,int mode)
{
	if (is_file(filename)) {
		printf(ESCVTX);
		view_text1(filename);
		if (mode) getchar3();
		printf(VTXOFF);
	}
}



yesno(char *mess,int defaultkey)
{
	char buff[10];
	int n;
	printf(mess);
	if (g_yes) {
		printf(g_yes?"Yes":"No");
		n=g_yes;
		g_yes=0;
		return n;
	}
	lineinput(buff,2);check_han(buff);
	if (strlen(buff)<1) return defaultkey;
	if (is_same(buff,"q")) return -1;
	if (is_same(buff,"a")) return -2;
	if (is_same(buff,"s")) return -3;
	if (is_same(buff,"n")) return No;
	if (is_same(buff,"0")) return 0;
	if (is_same(buff,"y")) return Yes;
	if (is_same(buff,"1")) return 1;
	return defaultkey;
}

yesno2(char *mess,int defaultkey)
{
	int ret;
	ret=yesno(mess,defaultkey);
	printf("\n");
	return (ret);
}

yesno3(char *filename,int defaultkey)
{
	int ret;
	view(filename);
	ret=yesno("",defaultkey);
	printf("\n");
	return (ret);
}

//¹®ÀÚ¿­¿¡ ÇØ´ç¹®ÀÚ°¡ Æ÷ÇÔµÇ¾î ÀÖ´Â°¡?
int is_char(char *s1,char c)
{
	int i=0;
	while (s1[i]) if (s1[i++]==c) return Yes;
	return (No);
}

int is_digit(char c)
{
	if (c<'0'||c>'9') return No;
	return Yes;
}


char *lwr(byte *str)
{
	static char buff[200];
	strcpy(buff,str);
	strlwr(buff);
	return (buff);
}


char *upr(byte *str)
{
	static char buff[200];
	strcpy(buff,str);
	strupr(buff);
	return (buff);
}


char *strlwr(byte *str)
{
	for(; *str ;str++)
	if(isupper(*str))
		*str=tolower(*str);
	return (str);
}


char *strupr(byte *str)
{
	for(; *str ;str++)
	if(islower(*str))
		*str=toupper(*str);
	return (str);
}




//µÎ°³ÀÇ ¹®ÀÚ°¡ ¿ÏÀüÈ÷ °°Àº°¡?
is_same(char *buff1,char *buff2)
{
	if (strcasecmp(buff1,buff2)==0) return Yes;
	return No;
}

//µÎ°³ÀÇ ¹®ÀÚ°¡ ¿ÏÀüÈ÷ °°Àº°¡?	  100ÀÚ¸¸ ºñ±³ÇÔ
is_same2(char *buff1,char *buff2)
{
	if (strcmp(buff1,buff2)==0) return Yes;
	return No;
}

//µÎ°³ÀÇ ¹®ÀÚ¿­À» Æ÷ÇÔ¿©ºÎÈ®ÀÎ Æ÷ÇÔµÇ¸é Yes
int stristr(const char *s1,const char *s2)
{
	int i=0,nn;
	char ss1[5000];
	char ss2[5000];
	if (s1[0]==0||s2[0]==0||(int)strlen(s1)<(int)strlen(s2)) return No;
	strcpy(ss1,s1);
	strcpy(ss2,s2);
	strlwr(ss1);
	strlwr(ss2);
	if (strstr(ss1,ss2)==NULL) return No;
	return Yes;
}


//°ø¹éÀ» ¾ø¾Ø´Ù
del_space(char *str)
{
	register int x,y;
	x=0;
	while (str[x]) {
		if (str[x] == ' '||str[x]=='\n') {
			y=x;
			while (str[y]) {
				str[y] = str[y+1];
				if (str[y]==0) break;
				y++;
			}
		}
		else x++;
	}
}


printfL0()
{
	printf("\n");
}
printfL1()
{
	printf("+---------+---------+---------+---------+---------+---------+---------+--------\n");
}
printfL2()
{
	printf(" ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡\n");
}
printfL3()
{
	printf(" ¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬\n");
}



void gotoXY(int x,int y)  {
	printf("[%d;%dH",y,x);
}


empty(char *buff)
{ return (buff[0]==0);
}


pressenter()
{
	char buff[500];
	bzero(buff,200);
	_cont1:
	printf("[Enter] Å°¸¦ ´©¸£½Ê½Ã¿ä.");
	fflush(stdout);
	lineinput(buff,400);
	printf("\n");
	if (strlen(buff)>0&&is_esc(buff)) goto _cont1;
}

clrscr()
{
	printf("\033[2J\033[;H");
	fflush(stdout);
}



cls()
{
	printf("\033[2J\033[;H");
	fflush(stdout);
}


//ESC,°ø¹é,Enter¸¦ ÀüºÎÁ¦°ÅÇÑ´Ù.
del_clear(char *buff)
{
	int i,n;
	char buff2[300];
	byte nn,a,b,c;
	i=0;
	n=0;
	a=32;
	b=126;
	c=160;
	while (buff[i]!=0) {
		nn=(byte)buff[i];
		if (((nn>=a&&nn<=b)||nn>=c)&&nn!='\n'&&nn!=' ')
			 buff2[n++]=buff[i];
		i++;
	}
	buff2[n]=0;
	strcpy(buff,buff2);
}


is_esc(unsigned char *buff)
{
	int i,n,ret=No;
	byte nn,a,b,c;
	i=0;
	n=0;
	a=32;
	b=126;
	c=160;
	while (buff[i]!=0) {
		nn=(byte)buff[i];
		if (! ( (nn>=a&&nn<=b) || nn>=c ) ) ret=Yes;
		i++;
	}
	return ret;
}


del_esc(unsigned char *buff)
{
	int i,n;
	char buff2[300];
	byte nn,a,b,c;
	i=0;
	n=0;
	a=32;
	b=126;
	c=160;
	while (buff[i]!=0) {
		nn=(byte)buff[i];
		if ( (nn>=a&&nn<=b) || nn>=c )
			 buff2[n++]=buff[i];
		i++;
	}
	buff2[n]=0;
	strcpy(buff,buff2);
}


//¹®ÀÚÁß¿¡¼­ ; * µîÀ» Á¦°ÅÇÑ´Ù.  ÀÓÀÇ SHELLÀ» ¸·´Â´Ù.
del_esc3(unsigned char *buff)
{
	int i,n;
	char buff2[300];
	unsigned char nn;
	i=0;
	n=0;
	while (n<300&&(nn=(unsigned char)buff[i])) {
		if ( ((nn>=32&&nn<=126) || nn>=160 )
			 &&buff[i]!=';'&&buff[i]!='<'
			 &&buff[i]!='>'&&buff[i]!='*'
			 &&buff[i]!='\\'
			 &&buff[i]!='&')
			 buff2[n++]=buff[i];
		i++;
	}
	buff2[n]=0;
	strcpy(buff,buff2);
}

del_char1(char *buff,char ch)
{
	int i;
	for (i=0;i<(int)strlen(buff);i++) {
		if (buff[i]==ch) {
			buff[i]=0;
			all_trim(buff);
			break;
		}
	}
}



//¹®ÀÚÁß¿¡¼­ ¼ýÀÚ,-¸¸ ¹Þ¾Æµå¸°´Ù.
del_char2(char *buff)
{
	int i,i2;
	char buff2[300];
	bzero(buff2,300);
	i=i2=0;
	while (buff[i]!=0) {
		if ((buff[i]>='0'&&buff[i]<='9')||
			buff[i]=='-'||buff[i]=='('||buff[i]==')')
				buff2[i2++]=buff[i];
		i++;
	}
	strcpy(buff,buff2);
}


del_enter(unsigned char *buff)
{
	int i,n;
	char buff2[3000];
	i=0;
	n=0;
	while (buff[i]!=0) {
		if ( buff[i]!='\n'&&buff[i]!='\r' ) buff2[n++]=buff[i];
		i++;
	}
	if (n>(int)strlen(buff)) n=(int)strlen(buff);
	buff2[n]=0;
	strcpy(buff,buff2);
}



del_tab(unsigned char *buff)
{
	int i,n;
	char buff2[400];
	i=0;n=0;
	bzero(buff2,400);
	while (buff[i]!=0) {
		if ( buff[i]!='\t'&&buff[i]!='\n'&&buff[i]!='\r' ) buff2[n++]=buff[i];
		i++;
	}
	strcpy(buff,buff2);
}


all_trim(char *buff)	  //¾çÂÊ¿¡¼­ ºó°ø°£ Á¦°ÅÇÏ±â  2->1
{
	char buff1[2500];
	strcpy(buff1,buff);
	alltrim(buff,buff1);	  //¾çÂÊ¿¡¼­ ºó°ø°£ Á¦°ÅÇÏ±â  2->1
}


rtrim(char *buff)	   //¾çÂÊ¿¡¼­ ºó°ø°£ Á¦°ÅÇÏ±â  2->1
{
	int i;
	for (i=(int)strlen(buff);i>=0;i--) {
		if (buff[i]==0||buff[i]==' ') buff[i]=0;
		else break;
	}
}


alltrim(char *buff1,char *buff2)	  //¾çÂÊ¿¡¼­ ºó°ø°£ Á¦°ÅÇÏ±â  2->1
{
	char buff3[2500];
	char buff4[2500];
	int  first;
	unsigned int i,i2;

	strcpy(buff3,buff2);
	i2=0;
	first=Yes;
	strcpy(buff1,"");
	strcpy(buff3,"");
	for (i=0;i<strlen(buff2);i++) {
		if (!first||buff2[i]!=' ') {
			buff3[i2]=buff2[i];
			buff3[i2+1]=0;
			i2++;
			first=No;
		}
	}

	strcpy(buff1,"");
	i2=0;
	for (i=0;i<strlen(buff3);i++)
		if (buff3[i]!='\n'&&buff3[i]!='\r') {
			buff1[i2]=buff3[i];
			buff1[i2+1]=0;
			i2++;
		}
	rtrim(buff1);
}


dateinput0(time_t t)
{
	char buff[100];
	strcpy(buff,datestr(1,t));
	if (dateinput(buff)) t=str2date(buff);
	return t;
}


dateinput(char *str)
{
	int i,j;
	int ret=0;
	unsigned char ch;
	fflush(stdout);
//1999-12-31
	for (i=0;i<10;i++)
		if (str[i]=='\0') for (j=i;j<10;j++) str[j]=' ';
	for (i=0;i<10;i++) if (str[i]!='-'&&(str[i]<'0'||str[i]>'9')) str[i]=' ';
	str[10]=0;
	str[4]=str[7]='-';
	printf("7%s8",str);
	i=2;
	printf("%c%c",str[0],str[1]);
	while(1) {
		str[4]=str[7]='-';
		ch=getchar();
		if (ch=='\n') break;
		else if(ch == '\b') {
			str[i]=' ';
			if(i==5||i==8) {
				printf("\b-\b\b \b");
				i-=2;
			}
			else if(i > 0) {
				printf("\b \b");
				i--;
			}
			fflush(stdout);
		}
		else if((ch == 0x1b) | (ch == 0x18) | (ch == 0x0f));
		else if(i < 10) {
			str[i++] = ch;
			putchar(ch);
			if (i==4||i==7) {putchar('-');i++;}
		}
	}
	for (i=0;i<10;i++) if (str[i]<' ') str[i]=' ';
	str[10]=0;
	fflush(stdout);
	return ret;
}

lineinput_(char *str,int len,int echo)
{
	int i = 0;
	int ret=0;
	unsigned char ch;
	fflush(stdout);
	if (len<1) return ret;
	while((ch=getchar()) != '\n' ) {
		if(ch == '\b') {
			if(i > 0) {
				printf("\b \b");
				i--;
			}
		}
		else if(echo==9&&(ch<' '||ch==138)) i=0;      //´Ù½ÃÀÔ·Â¹ÞÀ½
		else if(echo==5&&(ch<' '||ch>'~'));
		else if(echo==6&&(ch<'0'||ch>'z'));
		else if(echo==7&&(ch<'0'||ch>'z'||ch=='r'||ch=='R')) ret=1;
		else if((ch == 0x1b) | (ch == 0x18) | (ch == 0x0f));
		else if(i < len) {
			str[i++] = ch;
			if	   (echo==0) ;
			else if(echo==2) putchar(' ');
			else if(echo==3) putchar('*');
			else			 putchar(ch);
		}
	}
	str[i] = 0;
	if (!bit(allmode,4)&&echo!=3) {
			 if (!bit(cfg.monichar,1)&&pf.type< 18) save_monichar(str,0);
		else if (!bit(cfg.monichar,2)&&pf.type>=18) save_monichar(str,1);
		else if (!bit(cfg.monichar,4)&&pf.type==0 ) save_monichar(str,4);
	}
	xstrncpy(_last1,str,14);
	fflush(stdout);
	return ret;
}


save_monichar(char *str,int mode)
{
	FILE *fp;
	char filename[300];
	char buff[300];
	char stty[20];
	if (strlen(str)<1) return;
	strcpy(stty,&tty[3]);strlwr(stty);
	if (bit(cfg.monichar,6)) sprintf(buff,"user/monich/%04d",now_yearmonth());
	else					 sprintf(buff,"/tmp/monich/%04d",now_yearmonth());
		 if (mode==0) sprintf(filename,"%s/t_%s.rec",buff,no_spc(stty));
	else if (mode==1) sprintf(filename,"%s/sy.rec",buff);
	else if (mode==2) sprintf(filename,"%s/ch%03d.rec",buff,roomnum);
	else if (mode==3) sprintf(filename,"%s/id.rec",buff);
	else if (mode==4) sprintf(filename,"%s/gu.rec",buff);
	else			  sprintf(filename,"%s/t_%s.rec",buff,no_spc(stty));
	strlwr(filename);
	if ((fp=fopen(filename,"a+"))==NULL) {
		check_dir(buff);
		fp=fopen(filename,"w+");
	}
	if (fp!=NULL) {
		fprintf(fp,"%s|%-8s|%s\n",now_str(16),pf.id,str);
		fclose(fp);
	}
}


tag_monichar()
{
	char buff[100];
	if (bit(cfg.monichar,5)) return;
	sprintf(buff,"*%s G:%s/%s T%d",now_str(10),club_go.code,go.code,pf.type);
	save_monichar(buff,0);
}

lineinput(char *str,int len)
{
	lineinput_(str,len,1);
}


lineinput2(char *str,int len)
{
	lineinput_(str,len,1);
	printf("\n");
}


lineinput3(char *mess,char *str,int len)
{
	printf(mess);
	lineinput_(str,len,1);
	printf("\n");
}


lineinput9(char *str,int len,int mode)
{
	return lineinput_(str,len,mode);		 //Æ¯¼öÅ°´Â ÀüºÎ¹«½Ã
}


lineinputx(char *str,int len)
{
	lineinput_(str,len,1);
	check_han(str);
	printf("\n");
	if (strlen(str)<3&&(is_same(str,"q")||is_same(str,"p")||is_same(str,"x")||is_same(str,"n")))
		return No;
	return Yes;


}



int numinput()
{
	char buff[20];
	lineinput_(buff,10,1);
	return atoi(buff);
}


passinput(char *str,int len)
{
	lineinput_(str,len,3);
}



//buff1 ¿¡¼­ buff2¿¡ Æ÷ÇÔµÈ ¹®ÀÚ¸¦ ÀüºÎ C·Î º¯°æÇÑ´Ù.
strign(char *buff1,byte *buff2,char c)
{
	int i;
	for(;*buff1;buff1++)
		for (i=0;i<(int)strlen(buff2);i++) if (*buff1==buff2[i]) *buff1=c;
}


strscheck(char *buff)		   //Æ¯¼öÇÑ¹®ÀÚ¸¦ ¹«½ÃÇÑ´Ù.
{
	strign(buff,"<>?*;&`'\"",'_');
}


getchar2()
{
	char buff[200];
	fflush(stdout);
	lineinput(buff,100);
	return buff[0];
}


getchar3()
{
	char buff[200];
	fflush(stdout);
	while(1) {
		lineinput_(buff,100,0);
		if (strlen(buff)<1) break;
	}
}


msleep(int n)
{
	fflush(stdout);
	usleep(n*100000);
}

mmsleep(int n)
{
	fflush(stdout);
	usleep(n*10000);
}


ssleep(int n)
{
	fflush(stdout);
	sleep(n);
}



print_(int mode,const char *fmt,...)
{
	va_list ap;
	char buff7[10];
	strcpy(buff7,"[7m");
	if (bit(cfg.logomode,8)) strcpy(buff7,"");
		 if (mode==1) printf("[K ### %s",buff7);
	else if (mode==3) printf("%s [78X[80D ### ",buff7);
	else			  printf("[K ### ");
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
		 if (mode==1) printf("[0;1m ###\n");
	else if (mode==3) printf(" ###[0;1m\n");
	else			  printf(" ###\n");
}




//ÇØ´çÆ÷Æ® 1ÁÙ ¸Þ¼¼Áö¸¦ º¸³½´Ù.
xprintf(char *filename,const char *fmt,...)
{
	FILE *fp;
	char mess[200];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(mess,fmt, ap);
	va_end(ap);
	if ((fp=fopen(filename,"a+"))==NULL ) fp=fopen(filename,"w+");
	if (fp!=NULL) {
		fprintf(fp,"%s",mess);
		fclose(fp);
	}
}


//buff ÀÇ ³»¿ëÁß ÀÌ»óÇÑ ¹®ÀÚ ÀÖ´Â°¡?
sys_tem_(char *buff,int mode)
{
	unsigned char c=0;
	char OLDDIR[200];
	FILE *fp;
	int i=0;
	if (bit(allmode,12)) {
		printf("B:'%s' M:%d\n",buff,mode);
		getchar();;
	}
	if (strlen(buff)<2) return 0;
	if (stristr(buff,"bin/sh")||stristr(buff,"bin/bash")||
		stristr(buff,"bin/ksh")||stristr(buff,"etc/passwd")) goto _err1;
	while ((c=buff[i++])) {
			 if (c>='0'&&c<='9') ;
		else if (c>='a'&&c<='z') ;
		else if (c>='A'&&c<='Z') ;
		else if (c==39||c>=160||c==34||c==39||c==64) ;
		else if (c==33||c==39||c==126) ;
		else if (c==' '||c=='#'||c=='/'||c=='_'||c=='-'||c=='.'||c=='>') ;
		else if ((mode==2||mode==3)&&c=='&') ;
		else if ((mode==2||mode==3)&&c=='<') ;
		else if (c==')'||c=='('||c=='?'||c=='*') ;
		else {
			_err1:
			if ((fp=fopen("/tmp/system2.tmp","a+"))!=NULL) {
				fprintf(fp,"%s %-8s%-8s C=%d %s\n",now_str(9),pf.id,tty,c,buff);
				fclose(fp);
			}
			printf("¿ÜºÎ ¸í·É ¿À·ù : %s %-8s%-8s C=%d %s\n",now_str(9),pf.id,tty,c,buff);
			pressenter();
			return 0;
		}
	}

	if ((fp=fopen("/tmp/system1.tmp","a+"))!=NULL) {
		fprintf(fp,"%s %-8s%-8s %s\n",now_str(9),pf.id,tty,buff);
		fclose(fp);
	}
	fflush(stdout);
	signal(SIGCLD,SIG_IGN);
	i=system(buff)/256;
	return (i);
}




mess(const char *fmt,...)
{
	char buff[300];
	char buff2[300];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	sprintf(buff2,"### %s ###[Enter]",buff);
	printf("%s\n",center_line(buff2));
	getchar2();
	printf("\n");
}



xy(int x,int y)  {
	printf("[%d;%dH",y,x);
}


char *findport(char *buff)		//ÇöÀç »ç¿ëµÇ°í ÀÖ´Â Æ÷Æ®Ã£±â
{
	char buff2[20];
	char *ttyname();
	strcpy(buff2,ttyname(1));
	buff2[19]=0;
	strcpy(buff,&buff2[5]);
	del_space(buff);
	if (strlen(buff)<1) printf("\n TTYÆ÷Æ® ERR\n");
	return (buff);
}


int numselect()
{
	char buff[20];
	printf(" ¼±ÅÃ >> ");
	return numselect2();
}


int numselect2()
{
	char buff[20];
	lineinput_(buff,7,1);printf("\n");check_han(buff);
	if (is_same(buff,"q")||
		is_same(buff,"x")||
		is_same(buff,"xx")||
		is_same(buff,"p")) return -1;
	return atoi(buff);
}


int get_num(int old,int len)
{
	char buff[100];
	if (len<1) len=10;
	lineinput(buff,len);
	if (strlen(buff)<1) return old;
	return atoi(buff);
}



float get_float(float old,int len)
{
	char buff[100];
	float f;
	f=0.0;
	if (len<1) len=10;
	lineinput(buff,len);
	if (strlen(buff)<1) return old;
	sscanf(buff,"%f",&f);
	return f;
}


char get_chr(char chr)
{
	char buff[10];
	lineinput(buff,3);
	if (buff[0]==0) return chr;
	return buff[0];
}


get_str(char *str,int len)
{
	char buff[100];
	lineinput(buff,len);
	if (buff[0]!=0) strcpy(str,buff);
}



mess1(const  char *fmt,...)
{
	va_list ap;
	char buff7[10];
	strcpy(buff7,"[7m");
	if (bit(cfg.logomode,8)) strcpy(buff7,"");
	printf("\n");
	printfL2();
	printf("[s%s",buff7);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\n");
	printfL2();
	pressenter();
}



//Ã£´Â¹®ÀÚ¿­ ³ª¿À¸é ±×Àü±îÁö ÀüºÎ»èÁ¦
strchng8(char *src, char *search)
{
   int i, i2, len,len_search;
   len=strlen(src);
   len_search= strlen(search);
   for(i=0;src[i];i++) {
	  if(src[i]==search[0]) {
		 if(strncasecmp(&src[i], search, len_search) == 0) {
			for(i2=0;i2<len;i2++) {
				src[i2]=src[i+i2];
				src[i2+1]=0;
			}
			return Yes;
		 }
	  }
   }
   return No;
}


//Ã£´Â¹®ÀÚ¿­ ³ª¿À¸é ±×À§Ä¡ºÎÅÍ ÀüºÎ»èÁ¦
strchng9(char *src, char *search)
{
   int i, len_search;
   len_search=(int)strlen(search);
   for(i=0; src[i];i++) {
	  if(src[i] == search[0]) {
		 if(strncasecmp(&src[i], search, len_search) == 0) {
			src[i]=0;
			return Yes;
		 }
	  }
   }
   return No;
}

get_bit(int num,int bitnum)
{
	char buff[100];
	lineinput(buff,1);
	if		(buff[0]=='0') num=set_bit(num,bitnum,0);
	else if (buff[0]=='1') num=set_bit(num,bitnum,1);
	return num;
}


char *bitstr(int i)
{
	int n,n2;
	static char buff[50];
	bzero(buff,33);
	n2=0;
	for (n=0;n<32;n++) {
		buff[n2++]=bit(i,n)?'1':'0';
		if (n%10==9) buff[n2++]=' ';
	}
	return buff;
}


view_bit(unsigned int n)
{
    unsigned int n1,n2,n3,n4;
    int i;
    for (i=0;i<32;i++) {
        if (i>0&&i%8==0) printf(" ");
        n2=n<<i;
        n2=n2&0x80000000;
        printf("%d",(n2!=0));
    }
}



//ÇØ´ç ÀÚ¸®°¡ 1 ÀÎ°¡?
bit(unsigned int n,int i)
{
	return ((n>>i)&1);
}

//ÇØ´ç ÀÚ¸®ÁöÁ¤
set_bit(unsigned int n,int i,unsigned int n2)
{
	unsigned i1,i2;
	//ÇØ´ç ÀÚ¸® 0À¸·Î ¸¸µë
	i1=1<<i;	  //ÇØ´ç ÀÚ¸®¸¸ 1·Î ¸¸µë	 ³ª¸ÓÁö´Â ÀüºÎ 0
	i2=~i1; 	  //ÇØ´ç iÀÚ¸®¸¸ 0À¸·Î ¸¸µë  ³ª¸ÓÁö´Â ÀüºÎ 1
	n=n&i2; 	   //n¿¡¼­ ÇØ´ç ÀÚ¸®¸é 0 À¸·Î
	if (n2==1) n=n|i1;	 //1ÀÌ¸é 1·Î ¼¼ÆÃ
	return (n);
}


char *c_han(char *buff)
{
	static char out[10];
	char c;
	strcpy(out,"");
	c=buff[0];
	if ( (c>='0'&&c<='9')||
		 (c>='a'&&c<='z')||
		 (c>='A'&&c<='Z') ) sprintf(out,"%c",tolower(c));
	else if (strcmp(buff,"°¡")<0)  strcpy(out,"_");
	else if (strcmp(buff,"³ª")<0)  strcpy(out,"°¡");
	else if (strcmp(buff,"´Ù")<0)  strcpy(out,"³ª");
	else if (strcmp(buff,"¶ó")<0)  strcpy(out,"´Ù");
	else if (strcmp(buff,"¸¶")<0)  strcpy(out,"¶ó");
	else if (strcmp(buff,"¹Ù")<0)  strcpy(out,"¸¶");
	else if (strcmp(buff,"»ç")<0)  strcpy(out,"¹Ù");
	else if (strcmp(buff,"¾Æ")<0)  strcpy(out,"»ç");
	else if (strcmp(buff,"ÀÚ")<0)  strcpy(out,"¾Æ");
	else if (strcmp(buff,"Â÷")<0)  strcpy(out,"ÀÚ");
	else if (strcmp(buff,"Ä«")<0)  strcpy(out,"Â÷");
	else if (strcmp(buff,"Å¸")<0)  strcpy(out,"Ä«");
	else if (strcmp(buff,"ÆÄ")<0)  strcpy(out,"Å¸");
	else if (strcmp(buff,"ÇÏ")<0)  strcpy(out,"ÆÄ");
	else						   strcpy(out,"ÇÏ");
	return (out);
}


is_han(char *buff)
{
	int i,key=No;
	for(i=0;i<(int)strlen(buff);i++)
		if (ishangul(buff[i])) {
			key=Yes;
			break;
		}
	return key;
}


dosinput(char *str,int len)
{
	return doskey(str,len,2);
}

keyinput(char *str,int len)
{
	strcpy(str,"");
	return doskey(str,len,2);
}


//mode==0:º¸Åë1ÁÙÀÔ·Â
//mode==1:Ã³À½ºÎÅÍÇÏ±â
//mode==2:»óÇÏÈ­»ìÇ¥»ç¿ë
//return 0:º¸Åë 1:À§ 2:¾Æ·¡ 3:CTRL+X
int doskey(char *str, int length,int mode)
{
	char ch;	// Å° ÀÔ·Â¹Þ±âÀ§ÇØ
	int i = 0;			 // i´Â ÇöÀçÀÇ Ä­ À§Ä¡¸¦ ÁöÁ¤ÇÑ´Ù.
	int a = 0;			 // for¹®¿¡¼­ »ç¿ëÇÏ±â À§ÇØ
	int current = 0;	 // ÇöÀç ¹è¿­ÀÇ À§Ä¡
	if (length < 1) 	 // length°¡ 1ÀÌÇÏÀÌ¸é Á¾·á.
		return 0;

	//³ª¸ÓÁö´ÂÀüºÎ 0À¸·Î
	for (a=0;a<length;a++) {
		if (str[a]==0) {
			for(i=a;i<length;i++) str[i]=0;
			break;
		}
	}
	i=a;		   //i=4;
//	  if (mode==2) i=0;
	printf("7"); // Ä¿¼­À§Ä¡±â¾ï
	if (i==0) printf("8%s 8", str);
	else	  printf("8%s 8[%dC", str,i);

	while((ch = getchar()) != '\n' )
	{
		if (i>=length) i=length;
		if (i<0) i=0;
		if (ch == '\b') {               // ¹é½ºÆäÀÌ½º Å°¸¦ ´©¸¦°æ¿ì
			if (i>0) {
				for(a = i - 1; a < length-1; a++) str[a] = str[a+1];
				str[a]=0;
				i--;
				str[length-1] =0;
			}
		}
		else if(ch ==24) {return 3;}	// ctrl+x ´©¸¦°æ¿ì
		else if(ch == 4) {			  // ctrl+d ´©¸¦°æ¿ì
			if(i>=0) {
				for(a = i ; a < length; a++) str[a] = str[a+1];
				str[length] =0;
			}
		}
		else if (ch == '\x1B') {        // ¹æÇâÅ° Á¦¾î
			if((ch=getchar())== '[') {
				ch = getchar();
					 if (mode==0&&ch=='A') ch='D';         //À§
				else if (mode==0&&ch=='B') ch='C';       //¾Æ·¡
					 if (ch=='A') return 1;         //À§
				else if (ch=='B') return 2;       //¾Æ·¡
				else if (ch=='C') {if(i < length&&str[i]!=0) i++;}  //¿À¸¥ÂÊ
				else if (ch=='D') {if(i > 0) i--;}      //¿ÞÂÊ
			}
		}
		else {// ÀÏ¹Ý Å° ÀÔ·Â
			for(a = length;a>i&&a>0; a--) str[a] = str[a - 1];
			str[i] = ch;
			str[length] =0;
			i++;
		}

		if (i>=length) i=length;
		if (i==0) printf("8%s 8", str);
		else	  printf("8%s 8[%dC", str,i);
	}
	str[length] = '\0'; // ¸¶Áö¸·¿¡ Á¾·á¹®ÀÚ ÀÔ·Â
	if (!bit(allmode,4)) {
		if (!bit(cfg.monichar,1))			   save_monichar(str,0);
		if (!bit(cfg.monichar,2)&&pf.type>=18) save_monichar(str,1);
		if (!bit(cfg.monichar,4)&&pf.type==0 ) save_monichar(str,4);
	}
	fflush(stdout);
	return 0;
}




strconv3(char *buff)
{
	char buff2[200],str[20];
	char ch;
	int i,i2=0,len=(int)strlen(buff);
	bzero(buff2,200);
//	  for (i=0;i<(int)strlen(buff2);i++) {printf(" %d. %d  <%c>\n",i,buff[i],buff[i]);}pressenter();
	for (i=0;i<len;i++) {
		ch=buff[i];
		if (ch=='\\') {
			str[0]=buff[i+1];
			if (str[0]=='\\') {
				buff2[i2++]=ch;
				i+=1;
				continue;
			}
			else if (str[0]=='n') {
				buff2[i2++]='\n';
				i+=1;
				continue;
			}
			str[1]=buff[i+1];
			str[2]=buff[i+2];
			str[3]=buff[i+3];
			str[4]=0;
			ch=atoi(str);
			i+=3;
		}
		buff2[i2++]=ch;
	}
	strcpy(buff,buff2);
//	  for (i=0;i<(int)strlen(buff2);i++) {printf(" %d. %d  <%c>\n",i,buff[i],buff[i]);}pressenter();
}

char *starlen(char *buffstr)
{
	static char buff[100];
	xstrncpy(buff,"*******************",(int)strlen(buffstr));
	return (buff);
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

strchng2(char *src, char *search, char *change)
{
	char buff1[2000];
	bzero(buff1,2000);
	if (strchng(buff1,src,search,change)) {
		strcpy(src,buff1);
		return Yes;
	}
	return No;
}



//Æ¯Á¤ÆÄÀÏ¿¡ Æ÷ÇÔµÈ ¹®ÀÚ´Â °ÅºÎÇÑ´Ù.
check_file_char(char *buff,char *filename)
{
	int i,i2,found;
	char buff2[300];
	char buff3[300];
	bzero(buff2,300);
	file2buff(buff3,filename,300);
	i=i2=0;
	found=No;
	for (i=0;i<(int)strlen(buff);i++) {
		if (is_char(buff3,buff[i])) found=Yes;
		else buff2[i2++]=buff[i];
	}
	strcpy(buff,buff2);
	return found;
}


char *sbit(int i,int n)
{
	static char buff[10];
	sprintf(buff,bit(i,n)?"(¡Û)":"(¡¿)");
	return(buff);
}

char *nbit(int i,char *buff1,char *buff2)
{
	static char buff[100];
	char buff7[5];
	strcpy(buff7,"[7m");
	if (bit(cfg.logomode,8)) strcpy(buff7,"");
	if (i) sprintf(buff,"¡¼  %s /*[7m%s[0;1m ¡½",buff1,buff2);
	else   sprintf(buff,"¡¼ *[7m%s[0;1m / %s ¡½",buff1,buff2);
	return(buff);
}


char *rbit(int i,int n,char *buff1,char *buff2)
{
	static char buff[100];
	if (bit(cfg.logomode,8)) {
		if (bit(i,n)) sprintf(buff,"%s",buff2);
		else		  sprintf(buff,"%s",buff1);
	}
	else {
		if (bit(i,n)) sprintf(buff,"¡¼  %s /*[7m%s[0;1m ¡½",buff1,buff2);
		else		  sprintf(buff,"¡¼ *[7m%s[0;1m / %s ¡½",buff1,buff2);
	}
	return(buff);
}


char *rbit2(int i,int n,char *buff1,char *buff2)
{
	static char buff[100];
	if (bit(i,n)) sprintf(buff,"%s",buff2);
	else		  sprintf(buff,"%s",buff1);
	return(buff);
}


int gtt_bit(int i,int n)
{
	return (set_bit(i,n,bit(i,n)?0:1));
}

xstrncpy(char *a,char *b, int c)
{
	int i;
	i=strlen(b);
	if(i<1) i=1;
	if (c>i||c<1) c=i;
	strncpy(a,b,c);
	a[c]=0;
}

xstrtcpy(char *a,char *b, int c)
{
	if (c<(int)strlen(b)) strcpy(a,&b[c]);
	else strcpy(a,"");
}

is_quit(char *buff)
{
	check_han(buff);
	if (strcasecmp(buff,"q")==0||strcasecmp(buff,"p")==0||
		strcasecmp(buff,"x")==0||strcasecmp(buff,"m")==0||
		strcasecmp(buff,"xx")==0||strcasecmp(buff,"pp")==0||
		strcasecmp(buff,"qq")==0) return Yes;
	return No;
}

char *center_line(char *str)
{
	static char buffout[300];
	char buff[200];
	char buff2[200];
	int i;
	strcpy(buff,str);all_trim(buff);
	i=39-strlen(buff)/2;
	if (i>1&&i<40) {
		sprintf(buff2,"%%%ds%%s",i);
		sprintf(buffout,buff2,"",buff);
	}
	else {
		strcpy(buffout,buff);
	}
	return (buffout);
}


//			say  sysop ¸Þ¼¼Áö  ÀÌ·¸°Ô µÉ¶§ ¸Þ¼¼Áö¸¸ »Ì¾Æ¼­ mess¿¡ ÀúÀå
// º¸ÅëÀº nn==2 	 buffstr->mess
buff2cmd(char *buffstr,char *mess,int nn)  //¸Þ¼¼Áö¸¦ °°ÀÌ ÀÔ·ÂÇÔ
{
	int i,n,count=0,start=No;;
	strcpy(mess,"");
	for (i=0;i<(int)strlen(buffstr);i++) {
//		  printf("%d %d %d %d '%c' <BR>",i,strlen(buffstr),nn,count,buffstr[i]);
		if (buffstr[i]!=' ') start=Yes;
		if (start&&buffstr[i]==' ') {
			count++;
			if (count>=nn) {
				strcpy(mess,&buffstr[i]);
				break;
			}
		}
	}
}


buff2buff1(char *buffstr,char *buff1,char *buff2)
{
	char buff3[3000];
	char buff4[3000];
	buff2buff_(buffstr,buff1,buff2,buff3,buff4,1);	//¸Þ¼¼Áö¸¦ °°ÀÌ ÀÔ·ÂÇÔ
}

buff2buff2(char *buffstr,char *buff1,char *buff2,char *buff3)
{
	char buff4[3000];
	buff2buff_(buffstr,buff1,buff2,buff3,buff4,2);	//¸Þ¼¼Áö¸¦ °°ÀÌ ÀÔ·ÂÇÔ
}

buff2buff3(char *buffstr,char *buff1,char *buff2,char *buff3,char *buff4)
{
	buff2buff_(buffstr,buff1,buff2,buff3,buff4,3);	//¸Þ¼¼Áö¸¦ °°ÀÌ ÀÔ·ÂÇÔ
}


//buffstr -> buff1,buff2 ·Î ºÐÇØ
buff2buff_(char *buffstr,char *buff1,char *buff2,char *buff3,char *buff4,int num)  //¸Þ¼¼Áö¸¦ °°ÀÌ ÀÔ·ÂÇÔ
{
	char str1[1000];
	char str2[1000];
	char str3[1000];
	char str4[1000];
	strcpy(str1,"");strcpy(str2,"");strcpy(str3,"");strcpy(str4,"");
	strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");strcpy(buff4,"");
	sscanf(buffstr,"%s%s%s%s",str1,str2,str3,str4);
	if (num==1) {
		buff2cmd(buffstr,buff2,1);
		strcpy(buff1,str1);
	}
	else if (num==2) {
		buff2cmd(buffstr,buff3,2);
		strcpy(buff1,str1);
		strcpy(buff2,str2);
	}
	else if (num==3) {
		buff2cmd(buffstr,buff4,3);
		strcpy(buff1,str1);
		strcpy(buff2,str2);
		strcpy(buff3,str3);
	}
}


char *no_spc(char *buff)
{
	static char buff2[100];
	int i,n;
	unsigned char nn;
	i=n=0;
	while (n<100&&(nn=(unsigned char)buff[i])) {
		if ( ((nn>=32&&nn<=126) || nn>=160 )
			 &&buff[i]!=';'&&buff[i]!='<'
			 &&buff[i]!='>'&&buff[i]!='*'
			 &&buff[i]!='/'&&buff[i]!='.'
			 &&buff[i]!='\\'
			 &&buff[i]!='?'&&buff[i]!='&')
			 buff2[n++]=buff[i];
		i++;
	}
	buff2[n]=0;
	return(buff2);
}

/* ¼ýÀÚ¿¡ ÄÞ¸¶³Ö¾î¼­ º¸¿©ÁÖ´Â ÇÁ·Î±×·¥ Ver1,0*/
//Æ¯¼º: 123456789 -> 12,345,678 ·Î ¸¸µå´Â ·çÆ¾
char *n2c(int num)
{
	static char pbuff[100];
	char ch,snum[100],xnum[100];
	int i, j, cm,old_num=num ;
	cm = 1 ; /* ÄÞ¸¶¸¦ Âï¾î¾ß ÇÏ´Â À§Ä¡¸¦ ÀúÀå */
	strcpy(pbuff,"");
	if (num<0) num*=-1;
	sprintf(snum,"%d",num);

	for (i=0,j=strlen(snum)-1; j>=0; j--)
	{
	  if (cm%4 == 0) /* 3°³´ÜÀ§·Î ³ª´©±â À§ÇØ¼­ */
	  {
		xnum[i++] = ',' ; /* ÄÞ¸¶¸¦ ³ÖÀ½, ´ÙÀ½¿ä¼Ò·Î ÀÌµ¿  */
		cm++ ; /* ÀÏ´Ü ÄÞ¸¶¸¦ ³ÖÀº ÀÚ¸® ´ÙÀ½À¸·Î ÀÌµ¿ */
	  }
	  xnum[i++] = snum[j] ; /* º¸Åë ¼ö¸¦ ³ÖÀ½ */
	  cm++ ; /* ÀÌ¿¡ ´ëÇÑ ÀÚ¸®, ´ÙÀ½À¸·Î ÀÌµ¿ */
	}
	xnum[i] = '\0' ; /* ³Î¹®ÀÚ ³ÖÀ½, ¹®ÀÚ¿­ ³¡ ÀÎ½ÄÀ§ÇØ */
	j=strlen(xnum);
	bzero(snum,100);

	if (old_num>=0) {
		for (i=0;j>0&&i<j;i++) {
			snum[j-i-1]=xnum[i];
		}
	}
	else {	//À½¼öÀÏ¶§
		for (i=0;j>0&&i<j;i++) {
			snum[j-i]=xnum[i];
		}
		snum[0]='-';
	}
	strcpy(pbuff,snum);
	/* Ã³¸®¹æ¹ý¿¡¼­ 1234 -> 432,1 ÀÌ µÇ¾ú±â ¶§¹®¿¡
	   ¹®ÀÚ¿­À» ¹Ý´ë·Î reverse ½ÃÅ´ -> 1,234 */
	return (pbuff);
}
