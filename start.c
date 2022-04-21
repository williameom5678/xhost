#include <stdio.h>

char *findport(char *buff)		//현재 사용되고 있는 포트찾기
{
	char buff2[20];
	char *ttyname();
	strcpy(buff2,ttyname(1));
	buff2[19]=0;
	strcpy(buff,&buff2[5]);
	return (buff);
}


#include <utmp.h>
#define STRUCT_UTMP struct utmp
view_network(char *hostbuff,char *tty)
{
	register STRUCT_UTMP utmp_entry;
	FILE *fp;
	STRUCT_UTMP utmp;
	strcpy(hostbuff,tty);
    if ((fp=fopen("/var/run/utmp","r"))!=NULL) {
        while (fread (&utmp, 1, sizeof(utmp), fp)>0) {  //읽어냄
            if (utmp.ut_name[0]&&!strncmp (tty, utmp.ut_line, sizeof (utmp.ut_line))) {
                strcpy(hostbuff,utmp.ut_host);
                break;
            }
        }
        fclose (fp);
    }
}

main()
{
    char buff[300];
    char tty[300];
    char hostname[300];
    findport(tty);
    view_network(hostname,tty);
    execl("bin/main","main",tty,hostname,NULL);
}
