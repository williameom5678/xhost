#include <stdio.h>
#include <string.h>
#include <termio.h>
#include <ctype.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <dirent.h>    //디렉토리 화일명 변환함수때문에 필요
#include <fcntl.h>
#include <stdarg.h>
#define Yes 1
#define No	0

int nn1,nn2;
int quest;
char tty[100];



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


//공백을 없앤다
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


char *findport(char *buff)		//현재 사용되고 있는 포트찾기
{
	char buff2[20];
	char *ttyname();
	strcpy(buff2,ttyname(1));
	buff2[19]=0;
	strcpy(buff,&buff2[5]);
	del_space(buff);
	if (strlen(buff)<1) printf("\n TTY포트 ERR\n");
	return (buff);
}


//문자열에 해당문자가 포함되어 있는가?
int is_char(char *s1,char c)
{
	int i=0;
	while (s1[i]) if (s1[i++]==c) return Yes;
	return (No);
}


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


change(char *filename,char *str1,char *str2)
{
	int i,n,count;
	char buff1[100000];
	char buff2[100000];
	char filename1[300];
	char filename2[300];
	char filename3[300];
	FILE *fp1,*fp2;

	strcpy(filename1,filename);
	findport(tty);
	sprintf(filename2,"%s_%s.tmp",filename1,no_spc(tty));
	sprintf(filename3,"%s.bak",filename1);
	if (!is_file(filename1)) {
		printf("ERR: %s 화일이 없습니다. 경로를 확인하세요.\n",filename1);
		return 0;
	}

	fp1=fopen(filename1,"r+"); if (fp1==NULL) {printf("ERR: %s READ  ERROR\n",filename1);return 0;}
	fp2=fopen(filename2,"w+"); if (fp2==NULL) {printf("ERR: %s WRITE ERROR\n",filename2);return 0;}
	count=0;
	while (fgets(buff1,sizeof(buff1),fp1)!=NULL) {
		if (quest) {
			if (stristr(buff1,str1)) {
				strcpy(buff2,buff1);
				strchng2(buff2,str1,str2);
				printf("%s -> %s (Y/n)>>",buff1,buff2);
				if (yesno("",Yes)&&strchng2(buff1,str1,str2)) count++;
				printf("\n");
			}
		}
		else {
			if (strchng2(buff1,str1,str2)) count++;
		}
		fputs(buff1,fp2);
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (count>0) {
		printf(" \"%s\"->\"%s\" %5d번 변환",str1,str2,count);
		sprintf(buff1,"cp -f %s %s",filename1,filename3);system(buff1);
		sprintf(buff1,"cp -f %s %s",filename2,filename1);system(buff1);
		printf(" %20s.bak백업\n",filename1);
		nn1++;
		nn2+=count;
	}
	else {
		unlink(filename2);
	}
	return count;
}


//두개의 문자가 완전히 같은가?	  100자만 비교함
is_same(char *buff1,char *buff2)
{
	if (strcasecmp(buff1,buff2)==0) return Yes;
	return No;
}

char *strlwr(char *str)
{
	for(; *str ;str++)
	if(isupper(*str))
		*str=tolower(*str);
	return (str);
}


//두개의 문자열을 포함여부확인 포함되면 Yes
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


yesno(char *mess,int defaultkey)
{
	char *buf,buff[100];
	int n;
	printf(mess);
	buf="                ";
	gets(&buf);
	strcpy(buff,buf);
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

main(argc, argv)
int argc;
char *argv[];
{
	int i,n,count;
	char buff[100000];
	char str1[100000];
	char str2[100000];
	DIR *dp;
	struct dirent *dfp;
	if (argc<4||argv[argc-2]==0||strcmp(argv[argc-2],argv[argc-1])==0) {
		printf("  up2down <-모드> <변환화일명> <문자1> <문자2> \n");
		printf("  원래화일명은 *.bak로 저장됨\n");
		printf("  빈칸인 경우 \"\"로 표시하세요..\n");
		printf("  *,? 등의 사용이 가능합니다.\n");
		return;
	}

	nn1=nn2=0;
	quest=No;
	strcpy(str1,argv[argc-2]);
	strcpy(str2,argv[argc-1]);
	for (i=0;i<argc-2;i++) {
		strcpy(buff,argv[i+1]);
//		  printf("%s*%s*%s*",buff,str1,str2);getchar();
		if (is_same(buff,"-y")) quest=Yes;
		if (!stristr(buff,".bak")&&
			is_file(buff)&&
			!is_same(".")&&
			!is_same("..")&&
			!is_char(buff,'*')&&
			!is_char(buff,'?')) {
				change(buff,str1,str2);
			}
	}
	printf("### 총 %d 화일 %d 건 변경완료\n",nn1,nn2);
}


