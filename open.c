#define byte    unsigned char
#define word    unsigned int
#define No 0
#define Yes 1
char *c_han();
cls()
{
    printf("\033[2J\033[;H");
}


lineinput_(char *str,int len,int echo)
{
    int i = 0;
    char ch;
    while((ch=getchar()) != '\n' ) {
        if(ch == '\b') {
            if(i > 0) {
                putchar(ch); putchar(' '); putchar(ch);
                if(i > 0) i--;
            }
        }
        else if((ch == 0x1b) | (ch == 0x18) | (ch == 0x0f));
        else if(i < len) {
            str[i++] = ch;
            if     (echo==0) ;
            else if(echo==2) putchar(' ');
            else if(echo==3) putchar('*');
            else             putchar(ch);
        }
    }
    str[i] = 0;
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


//µÎ°³ÀÇ ¹®ÀÚ°¡ ¿ÏÀüÈ÷ °°Àº°¡?    100ÀÚ¸¸ ºñ±³ÇÔ
is_same(char *buff1,char *buff2)
{
    byte buff3[330],buff4[330];
    if (strcasecmp(buff1,buff2)==0) return Yes;
    return No;
}

view_text1(char *filename)
{
    char c;
    FILE *fp;
    if ((fp=fopen(filename,"r"))==NULL) return No;
    while (!feof(fp)) putchar(getc(fp));
    fclose(fp);
    return Yes;
}


#define MAX_V 100000
view(char *filename)
{
	char buff[80],ch,str[50],buff2[80];
	FILE *fp1;
    int n,i,i2,i3,i4,len;
	int num=0;
	int iii=0;
    char ss[MAX_V];        //¹Ş´Â¹öÆÛ
    bzero(ss,100);
    if((fp1=fopen(filename,"r"))==NULL) return No;
    fseek(fp1,0,SEEK_END);
    len=ftell(fp1);
    rewind(fp1);
    for (i=0;i<MAX_V&&i<len;i++) ss[i]=fgetc(fp1);
    len=i;
    for (n=i;n<i+10;n++) ss[n]=0;
    fclose(fp1);
	iii=0;
	while(iii<MAX_V&&iii<len) {
		ch = ss[iii++];
		if (ch=='\0') break;
		if (ch!='!') putchar(ch);
		else {
			str[0]=ch;
			str[1]=ss[iii++];
            if(iii>=len) return Yes;
				 if(str[1]=='\n') { printf("!\n");continue;}
			else if(str[1]=='[')  { printf("[");continue;}
			else if(str[1]=='!')  { printf("!!");continue;}
			str[2]=ss[iii++];
			str[3]=ss[iii++];
			str[4]=ss[iii++];
			str[5]=ss[iii++];
			str[6]=0;
            if(iii>=len) return Yes;
			if (str[1]=='\n'||str[2]=='\n'||str[3]=='\n'||str[4]=='\n'||str[5]!='!') {
				printf("!");
				iii-=5;if (iii<0) iii=1;
				continue;
			}
                 if (strcasecmp(str,"!stop!")==0) break;
            else if (strcasecmp(str,"!quit!")==0) break;
            else if (strcasecmp(str,"!cls_!")==0) cls();
            else if (strcasecmp(str,"!wait!")==0) getchar3();
            else if (strcasecmp(str,"!pres!")==0) pressenter();
            else if (strcasecmp(str,"!sec_!")==0) {str[0]=ss[iii++];str[1]=0;n=atoi(str);sleep(n);}
            else if (strcasecmp(str,"!del_!")==0) {str[0]=ss[iii++];str[1]=0;n=atoi(str);usleep(n*100000);}
            else if (strcasecmp(str,"!exec!")==0) {  //text È­¸é º¸¿©ÁÜ
				bzero(str,30);
                for (i=0;i<30;i++) {
                    ch=ss[iii++];
                    if (ch==' '||ch=='\n') break;
                    str[i]=ch;
                }
                if(is_file(str)) system(str);
			}
            else if (strcasecmp(str,"!logo!")==0) {  //text È­¸é º¸¿©ÁÜ
				bzero(str,30);
                for (i=0;i<30;i++) {
                    ch=ss[iii++];
                    if (ch==' '||ch=='\n') break;
                    str[i]=ch;
                }
                view(str);
			}
			else printf(str);
		}
	}
    return (Yes);
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


pressenter()
{
	char buff[500];
	printf("\r\n              *** [Enter] Å°¸¦ ´©¸£¼¼¿ä ***");
	lineinput(buff,450);
	printf("\n");
}
//ÇöÀçÀÇ µğ·ºÅä¸®¿¡ È­ÀÏÀÌ ÀÖ´Â°¡? ÀÖÀ¸¸é Yes ¾øÀ¸¸é No
is_file(char *filename)
{
	return (access(filename,0)!=-1);
}


yesno(char *mess,int defaultkey)
{
	char buff[10];
	printf(mess);
    lineinput(buff,2);c_han(buff);
	if (buff[0]==0) return defaultkey;
	if (buff[0]=='q'||buff[0]=='Q') return -1;
	if (buff[0]=='a'||buff[0]=='A') return -2;
	if (buff[0]=='s'||buff[0]=='S') return -3;
	if (buff[0]=='Y'||buff[0]=='y'||buff[0]=='1') return Yes;
	return No;
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
    else                           strcpy(out,"ÇÏ");
    return (out);
}
