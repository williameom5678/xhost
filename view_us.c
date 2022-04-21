//////////////////////////////////////////////////////////////////////
//호스트에서 사용되는 유틸리티모음
//////////////////////////////////////////////////////////////////////
#include "bbs.h"

int auscount=0;
char ausman[2000][10];
MONI amoni[2000];
PF nowpf;
main(int argc, char **argv)
{
    FILE *fp;
    int mode=1;
    char tid[10];
    char buff[100];
    char buff1[100];
    char buff2[100];
    char filename[300];
	char buffstr[350];
    char buffstr1[350];
    char buffstr2[350];
	char cont=No;
    char not_stop=No;
    int  i,n,i2,count=0,maxline,key;

	findport(tty);
	read_cfg();
	set_tmpfile();
    if (argc>1) mode=atoi(argv[1]);
    auscount=0;
    if (argc>1&&stristr(argv[1],"/")) chdir(argv[1]);

    sprintf(filename,"user/moni");
    if (!is_file(filename)) {
        chdir("/bbs/");
        if (!is_file(filename)) {
            chdir("/xhost/");
            if (!is_file(filename)) {
                chdir("/bbs/bbs1/");
                if (!is_file(filename)) {
                    chdir("/bbs/bbs11/");
                }
            }
        }
    }

    //출력양식읽어오기
    strcpy(buffstr,"");strcpy(buffstr1,"");strcpy(buffstr2,"");
    sprintf(buff,"frm/us/us%d.frm",mode);
	if (!is_file(buff)) sprintf(buff,"frm/us/us.frm");
	file2buff(buffstr,buff,300);
    if ( mode==4||mode==5||mode==6 ) {
        sprintf(buff,"frm/us/us%d-1.frm",mode);
        file2buff(buffstr1,buff,300);
        sprintf(buff,"frm/us/us%d-2.frm",mode);
        file2buff(buffstr2,buff,300);
    }

	maxline=20;
	if (mode>2&&mode<8) maxline=10;

    view("frm/us/us.log");
    while(1) {
        if (!view("frm/us/us%d.top",mode)) view("frm/us/us.top");
        i=0;

        for (n=0;n<20;n++) {
            sprintf(filename,"user/moni/moni%d.dat",n);
            if ((fp=fopen(filename, "r"))==NULL) continue;
            while (fread(&moni,sizeof_moni,1,fp)) {
                if (moni.mode<1||strlen(moni.id)<3||strlen(moni.tty)<1) continue;
                if (mode!=8&&moni.mode==99) continue;
                if (moni.type<1) clear_pf();
                else {
                    if (moni.pfnum>0) pf.num=moni.pfnum;
                    if (!read_pf(moni.id)) clear_pf();
                }
                amoni[i+1]=moni;
                if ((i+1)<=99)  sprintf(g_buff4,"%2d.",i+1);
                else            sprintf(g_buff4,"%3d",i+1);
                sprintf(g_buff3,"%2d",i+1);
                sprintf(g_buff1,"%3d",i+1);
                for (i2=0;i2<auscount;i2++) {
                    if (is_same(ausman[i2],moni.id))
                        moni.mode2=set_bit(moni.mode2,1,1);
                }
                if (mode==4) {
                    if (i%4==0)       view3(buffstr1);  //첫칸
                    else if (i%4==3)  view3(buffstr2);  //마지막
                    else              view3(buffstr);   //가운데
                }
                else if (mode==5) {
                    if (i%5==0)       view3(buffstr1);  //첫칸
                    else if (i%5==4)  view3(buffstr2);  //마지막
                    else              view3(buffstr);   //가운데
                }
                else if (mode==6) {
                    if (i%3==0)       view3(buffstr1);  //첫칸
                    else if (i%3==2)  view3(buffstr2);  //마지막
                    else              view3(buffstr);   //가운데
                }
                else {
                    view3(buffstr);     //출력양식출력
                }
                i++;
            }
            fclose(fp);
        }

    //allmode=set_bit(allmode,11,1);
        if (mode==4&&i>0&&i%4!=0) view("frm/us/us%d-%d.end",mode,i%4);
        if (mode==5&&i>0&&i%5!=0) view("frm/us/us%d-%d.end",mode,i%5);
        if (mode==6&&i>0&&i%3!=0) view("frm/us/us%d-%d.end",mode,i%3);
        if (!view("frm/us/us%d.end",mode)) view("frm/us/us.end");
        sleep(3);
    }

}

