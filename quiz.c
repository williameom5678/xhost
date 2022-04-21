#include "util.c"

main(argc, argv)
int argc;
char *argv[];
{
    FILE *fp;
    char afile[20][300];
    int i,i2,score;
    char buff[300];
    char ans[300];
    char logo[300];
    char tcode[200],id[200];
    char buff3[500000];

	srand(time(NULL));			//난수의 초기화
    strcpy(id,argv[1]);                 //아이디
    strcpy(tcode,argv[2]);              //가기코드

    sprintf(logo,"quiz/%s.log",tcode);
    view(logo);
    for (i=0;i<10;i++) {
        for (i2=0;i2<10;i2++) {
            sprintf(afile[i],"quizdata/%s.%03d",tcode,i*100+rand()%100);
            if (is_file(afile[i])) break;
        }
    }

    score=0;
    for(i=0;i<10;i++) {
        strcpy(ans,"");
        if (file_size(afile[i])<5) continue;
        if ((fp=fopen(afile[i],"r+"))!=NULL) {
            if (!fgets(ans,200,fp)) continue;
            del_esc(ans);del_space(ans);
            cls();
            strcpy(buff3,"");
            while(fgets(buff,300,fp)) strcat(buff3,buff);
         	fclose(fp);
            view3(buff3);
        }
        lineinput(buff,30);
        del_esc(buff);del_space(buff);
        if (is_same(buff,ans)) score++;
    }
    if (score>=10) sprintf(buff,"quizdata/%s.scx",tcode,score);
    else           sprintf(buff,"quizdata/%s.sc%d",tcode,score);
    view(buff);
}
