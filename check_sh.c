////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 시삽명령사용여부   exit(2) 일때 OK
////////////////////////////////////////////////////////////////////////
#include <stdio.h>
main(argc, argv)
int argc;
char *argv[];
{
    FILE *fp,*fp1,*fp2;
    char c;
	char buff[90];
    char buff1[90];
    char buff2[90];
    char buff3[90];
    char pass1[90];
    char pass2[90];
    char pass3[90];

    if ((fp=fopen("system/shellpas.txt","r"))!=NULL) {
        fgets(pass1,30,fp);
        fgets(pass2,30,fp);
        fgets(pass3,30,fp);
    }
    del_enter(pass1);del_enter(pass2);del_enter(pass3);
    printf("\n");
    printf(" SHELL 암호1 >>");passinput(buff1,8);printf("\n");
    printf(" SHELL 암호2 >>");passinput(buff2,8);printf("\n");
    printf(" SHELL 암호3 >>");passinput(buff2,8);printf("\n");
    if (strcmp(buff1,pass1)==0&&
        strcmp(buff2,pass2)==0&&
        strcmp(buff3,pass3)==0) exit(2);
    exit(0);
}


passinput(char *str,int len)
{
	lineinput_(str,len,3);
}


lineinput_(char *str,int len,int echo)
{
	int i = 0;
	int ret=0;
	unsigned char ch;
	if (len<1) return ret;
	while((ch=getchar()) != '\n' ) {
		if(ch == '\b') {
			if(i > 0) {
				putchar(ch); putchar(' '); putchar(ch);
				if(i > 0) i--;
			}
		}
		else if(echo==9&&(ch<' '||ch==138)) i=0;      //다시입력받음
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
	fflush(stdout);
	return ret;
}

del_enter(unsigned char *buff)
{
	int i,n;
	char buff2[300];
	i=0;
	n=0;
	while (buff[i]!=0) {
		if ( buff[i]!='\n'&&buff[i]!='\r' ) buff2[n++]=buff[i];
		i++;
	}
	buff2[n]=0;
	strcpy(buff,buff2);
}
