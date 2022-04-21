#include <stdio.h>

main(int argc, char **argv)
{
    char buff[3000];
    int n,i;
	FILE *fp;
    if (argc<4||atoi(argv[2])<1||strlen(argv[3])<1) {
        printf("Usage: %s 파일 위치 삽입문자열\n",argv[0]);
        printf("   예) %s test.txt 30 \"    \"  : test.txt 30번째 열에 4개의 공백을 삽입\n",argv[0]);
        return;
    }

    if ((fp=fopen(argv[1],"r"))==NULL) {
        printf("##  %s 파일 열기 실패\n",argv[1]);
        return;
    }
    while (fgets(buff,3000,fp)) {
        for (i=0;i<(int)strlen(buff);i++) {
            if (i==atoi(argv[2])-1) printf("%s",argv[3]);
            if (buff[i]=='\n'||buff[i]=='\r') continue;
            printf("%c",buff[i]);
        }
        printf("\n");
    }
    fclose(fp);
}

