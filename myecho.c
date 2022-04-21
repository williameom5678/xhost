#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUF_LEN 128
#define IP_ADDRESS "168.126.145.11"
#define LOCK_PORT 4000

int main(int argc,char *argv[]) {
    int s,n,len_in,len_out;
    struct sockaddr_in server_addr;
    char *haddr;
    char buf[BUF_LEN+1];
    
    if(argc != 2) {
	printf("usage : %s IP ADDRESS\n",argv[0]);
	return -1;
    }
    haddr=argv[1];
    if((s=socket(PF_INET,SOCK_STREAM,0)) < 0) {
	printf("can't create socket\n");
	return -1;
    }
    bzero((char *)&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr(haddr);
    server_addr.sin_port=htons(LOCK_PORT);
    
    if(connect(s,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
	printf("can't connect\n");
	return -1;
    }
    
    printf("Input any string : ");
    if(fgets(buf,BUF_LEN,stdin)) {
	buf[BUF_LEN]='\0';
	len_out=strlen(buf);
    }
    else {
	printf("fgets error\n");
	return -1;
    }
    
    if(write(s,buf,len_out) < 0) {
	printf("write error\n");
	return -1;
    }
    
    n = 0;
    while(1) {
	if((n=read(s,buf,sizeof(buf))) < 0) {
	    printf("read error\n");
	    return -1;
        }
	if(n > 0) break;
    }

    buf[n] = '\0';
    printf("%s\n",buf);
    close(s);
}

