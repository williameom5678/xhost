#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>

#define	MAXLINE		1024
#define MAX_SOCK 	 512
#define SERVER_PORT	8000

typedef struct {
    unsigned char m_code[4];
    char 	  tty[10];
    char 	  msg[996];
} MESS;

MESS	mess;

int 	quite = 0;
unsigned char 	id_code[] = {255, 254, 255, 'M'};
FILE	*fp;


void del_space(char *str)
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
        else
	    x++;
    }
}

int main(int argc,char *argv[]) {
    char rline[MAXLINE],my_msg[MAXLINE];
    char *start = "Welcome to talking room...\n";
    char buff1[256], buff2[256];
    int  i, j, n;
    int  s, client_fd, clilen;
    int  nfds;
    unsigned short int port_num;
    fd_set read_fds;
    int  num_chat = 0;
    int  client_s[MAX_SOCK];
    struct sockaddr_in client_addr,server_addr;
    int  fout;
    
    if(argc < 2) {
	printf("\nusage:%s port_number(8000부터) [-q]\n",argv[0]);
	exit(0);
    }

    port_num = atoi(argv[1]);
    if(port_num < 8000) {
        printf("\nusage:%s port_number(8000부터) [-q]\n",argv[0]);
        exit(0);
    }

    if(argc == 3) {
	if(!strcmp(argv[2],"-q"))
	    quite = 1;
    }

    //printf("port number:%d\n",port_num);
    //printf("quite: %d\n",quite);
    
    if(!quite)
	printf("talking room server initializing....\n");

    if((s=socket(PF_INET,SOCK_STREAM,0)) < 0) {
	if(!quite)
	    printf("Server : Can't open stream socket.");
	close(s);
	return -1;
    }
    
    bzero((char *)&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port_num);
    
    if(bind(s,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
	if(!quite)
	    printf("Server : Can't bind local address.\n");
	return -1;
    }

    listen(s,20);

    nfds = s + 1;
    FD_ZERO(&read_fds);

    while(1) {
    usleep(500);
	//fp = fopen("/tmp/select.temp","w");
	//fprintf(fp,"화일문 시작하자 마자\n");
	//fclose(fp);
	if((num_chat-1) >= 0) nfds = client_s[num_chat-1] + 1;
	FD_SET(s, &read_fds);
	for(i=0; i < num_chat; i++) FD_SET(client_s[i],&read_fds);
    if(select(nfds, &read_fds, (fd_set *)0, (fd_set *)0,(struct timeval *)0) < 0) {
		if(!quite)
		    printf("select error\n");
		return -1;
        }

    //fp = fopen("/tmp/select.temp","a");
	//fprintf(fp,"select문 바로 다음\n");
	//fclose(fp);

	if(FD_ISSET(s, &read_fds)) {
	   clilen = sizeof(client_addr);
	   client_fd = accept(s, (struct sockaddr *)&client_addr, &clilen);
	   if(client_fd != -1) {
	       client_s[num_chat] = client_fd;
	       num_chat++;
               // 접속된 클라이언트에게 환영 메세지를 보냄
	       // send(client_fd, start, strlen(start), 0);
	       // printf("No. of user : %d\n", num_chat);
	       //fp = fopen("/tmp/select.temp","a");
	       //fprintf(fp,"No. of user : %d\n", num_chat);
	       //fclose(fp);
           }
        }
	
	//fp = fopen("/tmp/select.temp","a");
	//fprintf(fp,"for문 수행전\n");
	//fclose(fp);
	for(i=0; i < num_chat; i++) {

	    if(FD_ISSET(client_s[i], &read_fds)) {
	        memset(&mess,0,sizeof(MESS));
	        if((n=recv(client_s[i], &mess, sizeof(MESS), 0)) > 0) {
	            //rline[n] = '\0';
		    mess.tty[9] = 0;
	            //fp = fopen("/tmp/select.temp","a");
	            //fprintf(fp,"recv문 바로 다음\n");
	            //fclose(fp);
		    //printf("\nmessage testing.......\n\n");

		    if(mess.m_code[0] == 't' && mess.m_code[1] == 'e' && mess.m_code[2] == 's' && mess.m_code[3] == 't') {
			send(client_s[i], &mess, sizeof(MESS), 0);
		        shutdown(client_s[i], 2);
		        if(i != num_chat-1) client_s[i] = client_s[num_chat-1];
		        num_chat--;
			//printf("\n%c%c%c%c %s %s\n",mess.m_code[0],mess.m_code[1],mess.m_code[2],mess.m_code[3],mess.tty,mess.msg);
			continue;
		    }
		    
		    shutdown(client_s[i], 2);
		    if(i != num_chat-1) client_s[i] = client_s[num_chat-1];
		    num_chat--;
		    
		    if(mess.m_code[3] != id_code[3]) {
                if(!quite) {
                    perror("Client를 신뢰할 수 없습니다.\n");
                    perror("Close connect of Client...\n");
                }
                continue;
            }
		    
		    if(mess.tty[0] == 'K' && mess.tty[1] == 'K') {
                strcpy(buff1,&mess.tty[2]);
                if(atoi(buff1) > 100) kill(atoi(buff1),SIGHUP);
                continue;
		    }
		    
		    sprintf(buff1,"/dev/%s",mess.tty);
		    del_space(buff1);
		    if(!quite)
		        printf("[%s][%s] %s\n",mess.tty,buff1,mess.msg);
		    fout = open(buff1,O_WRONLY|O_NDELAY);
		    if(fout == -1) {
	                //fp = fopen("/tmp/select.temp","a");
	                //fprintf(fp,"open문 실패시\n");
	                //fclose(fp);
			sprintf(buff2,"(%s)포트를 열 수 없습니다.",buff1);
			if(!quite)
			    perror(buff2);
	                fp = fopen("/tmp/select.temp","a");
			fprintf(fp,"%s %s %s\n",buff1,mess.tty,mess.msg);
	                fclose(fp);
		    }
		    else {
			if(strlen(mess.msg) > 0) {
	                    //fp = fopen("/tmp/select.temp","a");
	                    //fprintf(fp,"wirte문 수행직전\n");
	                    //fclose(fp);
			    write(fout,mess.msg,strlen(mess.msg));
	                    //fp = fopen("/tmp/select.temp","a");
	                    //fprintf(fp,"wirte문 수행직후\n");
	                    //fclose(fp);
			}
			close(fout);
		    }
                }
            }
	}
	//fp = fopen("/tmp/select.temp","a");
	//fprintf(fp,"for문 수행후\n\n");
	//fclose(fp);
    }
}
