#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <dirent.h>    //디렉토리 화일명 변환함수때문에 필요
#include <time.h>
#define No 0
#define Yes 1
#define byte   unsigned char
#define word   unsigned int
int total_line;
char a_buff[5000][5000];

//#define SENDMAIL_PATH   "/usr/lib/sendmail -bs > /dev/null"
#define SENDMAIL_PATH   "/usr/lib/sendmail -bs"
int count=0;

char *datestr()
{
	static char buff[50];
	struct tm *tp;
	time_t t;
	time(&t);
	tp = localtime(&t);
	sprintf(buff,"%02d-%02d-%02d %02d:%02d",tp->tm_year%100,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
	return (buff);
}

void del_enter(char *str) {
	register i, j;

	for(i=0; str[i] != '\0'; i++)
	if(str[i] == '\n' || str[i] == '\r') {
		str[i] = '\0';
		return;
	}
}
//문자열에 해당문자가 포함되어 있는가?
int is_char(char *s1,char c)
{
	int i=0;
	while (s1[i]) if (s1[i++]==c) return Yes;
	return (No);
}


main(int argc, char **argv)
{
	FILE *fp;
    char buff[5000],buffs[400];
	int i,i2;
    int seekok=1;
	if (argc<3) {
		printf("  sm [email주소파일] [메일파일]\n");
		return;
	}

    if ((fp=fopen(argv[2],"r"))==NULL) {
		printf(" %s 파일을 읽을수가 없습니다.\n",argv[2]);
		return;
	}
    total_line=0;
    while(fgets(buff,sizeof(buff),fp)) {
        del_enter(buff);
        all_trim(buff);      //양쪽에서 빈공간 제거하기  2->1
        strcpy(a_buff[total_line++],buff);
    }
	fclose(fp);

    if ((fp=fopen(argv[1],"r"))==NULL) {
		printf(" %s 파일을 읽을수가 없습니다.\n",argv[1]);
		return;
	}
	count=0;
	while (fgets(buff,50,fp)!=NULL) {
		if (is_char(" ;#*",buff[0])) continue;
        del_enter(buff);
        all_trim(buff);      //양쪽에서 빈공간 제거하기  2->1
		if (strlen(buff)<3||strlen(buff)>30||!is_char(buff,'@')) continue;
        if (seekok&&in_file3("/tmp/smail.out",buff)) {
	    printf(" %d -> %s skip\n",count+1,buff);
	    continue;
	}

        //    보내는메일,받는메일,보낼내용파일
        sprintf(buffs,"shop@te%d.com",rand()%100000,rand()%100000);
        if (strstr(buff,"hanmail.")) sprintf(buffs,"test%d@test.com",rand()%100000,rand()%100000);
        send(buffs,"쇼핑뉴스",buff,"오늘의 패션소식과 요즈음 뜨고 있는 채팅방 정보");
	sleep(1);
	}
	fclose(fp);
}

send(char *mailfrom, char *mailName, char *mailto,char *title)
{
	int i, cnt, retval;
    char ch, buf[5000],buff[5000];
	char out_file[500];
    FILE *fp;

    printf("_____________%s \n",mailfrom);

	if((fp=popen(SENDMAIL_PATH,"w")) == NULL) {
		fprintf(stderr,"sendmail open 에러");
		return;
	}

	/* sendmail 데몬과 통신을 시작한다. */
    fprintf(fp,"HELO localhost\n");
    fprintf(fp,"MAIL FROM: <%s>\n", mailfrom);
    fprintf(fp,"RCPT TO: <%s>\n", mailto);
    fprintf(fp,"DATA\n");
    fflush(NULL);
    fprintf(fp,"To: \"%s\" <%s>\n",mailto,mailto);
    fprintf(fp,"From: %s <%s>\n",mailName,mailfrom);
    fprintf(fp,"Reply-To: %s\n",mailfrom);
	fprintf(fp,"Subject: %s\n",title);
	fprintf(fp,"Mime-Version: 1.0\n");
	fprintf(fp,"Content-Transfer-Encoding: 8bit\n");
	fprintf(fp,"Content-Type: text/html; charset=euc-kr\n");
    for (i=0;i<total_line;i++) {
        fprintf(fp,"%s\n",a_buff[i]);
    }
	fprintf(fp,".\n");
    fprintf(fp,"quit\n");
	retval=pclose(fp);
    fflush(NULL);

	if(retval) {
		fprintf(stderr,"sendmail close 에러");
	}
	else {
        printf("%5d. 발신:%s  수신:%s\n",++count,mailfrom,mailto);
		strcpy(out_file,"/tmp/smail.out");
		if ((fp=fopen(out_file,"a+"))==NULL) fp=fopen(out_file,"w+");
		if (fp!=NULL) {
            strcpy(buff,title);
            buff[30]=0;
            fprintf(fp,"%-30.30s %5d. %s %s\n",mailto,count,datestr(),buff);
			fclose(fp);
		}
	}
}



/*
To: "=?EUC-KR?B?uN7Az7jFsMXB+CCxuLW2wNo=?=" <monitor@ezpaper.co.kr>
From: "=?EUC-KR?B?wMzB9sbkwMzG2yC538fgwM4=?=" <fail@ezpaper.co.kr>
Reply-To: oddddae@hanmail.net
Subject: =?EUC-KR?B?W7zSvcRdIEFCSVQsIExJTlVYLUdlbnR1cyAyLjAgud/HpQ==?=
Mime-Version: 1.0
Content-Transfer-Encoding: 8bit
Content-Type: text/html; charset=euc-kr
*/
//화일속의 문자열 이 포함되면 Yes; (동일하여야함)
in_file3(char *filename,char *buffstr)
{
    char buff[500],buff2[500];
	int found=No;
	FILE *fp;
    strcpy(buff2,buffstr);
    buff2[29]=0;all_trim(buff2);
	if ((fp=fopen(filename,"r"))!=NULL) {
		while (fgets(buff,200,fp)!=NULL) {
            buff[29]=0;del_enter(buff);all_trim(buff);
            if (is_same(buff,buff2)) {
				found=Yes;
				break;
			}
		}
		fclose(fp);
	}
	return (found);
}

all_trim(char *buff)	  //양쪽에서 빈공간 제거하기  2->1
{
	char buff1[2500];
	strcpy(buff1,buff);
	alltrim(buff,buff1);	  //양쪽에서 빈공간 제거하기  2->1
}
rtrim(char *buff)	   //양쪽에서 빈공간 제거하기  2->1
{
	int i;
	for (i=(int)strlen(buff);i>=0;i--) {
		if (buff[i]==0||buff[i]==' ') buff[i]=0;
		else break;
	}
}


alltrim(char *buff1,char *buff2)	  //양쪽에서 빈공간 제거하기  2->1
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


//두개의 문자가 완전히 같은가?
is_same(char *buff1,char *buff2)
{
	if (strcasecmp(buff1,buff2)==0) return Yes;
	return No;
}

