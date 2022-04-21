////////////////////////////////////////////////////////////////////////
// XHOST: 리눅스용 BBS HOST
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
main(int argc, char **argv)
{
    FILE *fp;
    int i,n,count;
	char filename[300];
    char buff[300],buff2[300];
    char a_str[30][200];

	if (argc>2&&is_char(argv[2],'/')) {chdir(argv[2]);}
	if (argc>2&&is_char(argv[1],'/')) {chdir(argv[1]);strcpy(argv[1],argv[2]);}
	if (!is_file(ID_FILE)) {
		chdir("/bbs");
		if (!is_file(ID_FILE)) {
			chdir("/xhost");
			if (!is_file(ID_FILE)) {
				chdir("/bbs/bbs1");
			}
		}
	}
	findport(tty);
    for (i=0;i<20;i++) strcpy(a_str[i],"");
    cls();
    while (1) {
        read_cfg();
        set_tmpfile();
        for (i=0;i<40;i++) a_num[i]=0;
        for (n=0;n<=cfg.count_server;n++) {
            sprintf(filename,"user/moni/moni%d.dat",n);
            if ((fp=fopen(filename, "r"))==NULL) continue;
            while (fread(&moni,sizeof_moni,1,fp)) {
                if (moni.mode<1||strlen(moni.id)<3||strlen(moni.tty)<1) continue;
                a_num[0]++;       //총인원
                if (n>0&&n<20) a_num[n]++;       //호스트당 총인원
                if (moni.mode==3)  a_num[21]++;      //대화실
                if (moni.mode==15) a_num[22]++;      //텔넷
                if (moni.mode>=5&&moni.mode<=8) a_num[23]++;      //자료
                if (moni.type<1) a_num[24]++;
                if (moni.type>0&&moni.type<18) a_num[25]++;
                if (moni.type>17) a_num[26]++;
            }
            fclose(fp);
        }
        if (a_num[0]>0) {
            sprintf(filename,"%s/log_%s.txt",REC_DIR,now_str(12));
            put_line(buff,filename);
            sprintf(buff,"%s %3d %3d %3d %3d : %3d %3d %3d : %3d %3d %3d %3d %3d",
            now_str(3),a_num[0],a_num[24],a_num[25],a_num[26],
            a_num[21],a_num[22],a_num[23],
            a_num[7],a_num[8],a_num[9],a_num[10],a_num[11]);
            for (i=0;i<19;i++) strcpy(a_str[i],a_str[i+1]);
            strcpy(a_str[19],buff);

            xy(1,1);
            printf("        시스템 현황   %s  HNO:%d \n",now_str(8),cfg.count_server);
            printf("날짜/총접속/손님/일반/시삽 : 대화/텔넷/자료 : 서버접속\n");
            for (i=0;i<20;i++) {
                printf("%s\n",a_str[i]);
            }
        }
        sleep(1*60);
    }
}
