#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <limits.h>

#define BUF_LEN 128
#define FILE_MAX 500
#define LOCK_PORT 4000


char lock[500][80];
int  index, maxind;
int  flag;

int main(int argc, char *argv[]) {
    register int i, j;
    struct sockaddr_in server_addr, client_addr;
    int server_fd, client_fd;
    int len, len_out;
    int port,nn;
    int quit;
    char buf[BUF_LEN+1],buff[80];
    
    quit = 0;
    if(argc == 2) {
    if(strcasecmp(argv[1], "-q")==0) quit = 1;
    }
	    
    flag = 0;
    index = 0;
    maxind = 0;
    for(i=0; i < FILE_MAX; i++) {
	lock[i][0] = '\0';
    }

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	if(!quit) printf("Server : Can't open stream socket.");
	return -1;
    }
    
//bzero((char *)&server_addr, sizeof(server_addr));
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(LOCK_PORT);
    
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
	if(!quit) printf("Server : Can't bind local address.\n");
	return -1;
    }
    
    listen(server_fd, 5);
    nn=0;
    
    while(1) {
        if(!quit) printf("Server : waiting connection request. %d..\n",nn++);
        len = sizeof(client_addr);

        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
        if(client_fd < 0) {
            if(!quit) printf("Server : accept failed.\n");
            return 0;
            }
        if(!quit) printf("Server : client connected.\n");
        memset(buf,'\0',sizeof(buf));
        len_out = read(client_fd, buf, sizeof(buf));
        for(j=0; buf[j] != ',' && buf[j] != '\0'; j++);
        if(buf[j] == '\0') return -1;
        j++;
        flag = 0;
        for(i=0; i < FILE_MAX; i++) {
            if(!strcmp(lock[i],&buf[j])) {
                index = i;
                break;
            }
            else if(flag == 0 && lock[i][0] == '\0') {
                index = i;
                flag = 1;
            }
        }

        if(!strncmp(buf,"before",6)) {
            if(lock[index][0] == '\0') {
            strcpy(lock[index],&buf[j]);
            strcpy(buf,"true");
                len_out = strlen(buf);
            }
            else {
                strcpy(buf,"false");
                len_out = strlen(buf);
            }
        }
        else if(!strncmp(buf,"after",5)) {  ///풀기할때 무조건 return;
            lock[index][0] = '\0';
            close(client_fd);
            continue;
        }
        else {
            if(!quit) printf("ERROR : undefined value!!!\n");
            strcpy(buf,"undefined");
            len_out = strlen(buf);
        }

        if(!quit) printf("len_out = %d\n",len_out);
        write(client_fd, buf, len_out);
        close(client_fd);
        if(!quit) printf("lock[%d] ==> %s\n",index,lock[index]);
    }
    close(server_fd);
}
