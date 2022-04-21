////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 동호회진입기록
////////////////////////////////////////////////////////////////////////

#include "util.c"

int nn;
int top;
int last;
int key;

char id_file[300];
char menu_file[300];
char cfg_file[300];
char filename[300];
char now_code[100];
char now_id1[60];
char now_id2[60];
double ntotal1,ntotal2,ntotal3,ntotal4;

BANK bank;
BANKCFG bankcfg;

//bank.mode=1  일반송금
//			2  일반대출
//          3  특별송금
//			8  운영자조정
//			9  수수료입금

main(argc, argv)
int argc;
char *argv[];
{
    int n=0,i,n2,send;
	char tid[9];
    char buff2[40];
	FILE *fp;
	char filename[100];

    if (argc<4) {
		return;
	}
    strcpy(tid,argv[1]);
    strcpy(tcode,argv[2]);
    ttype=aoti(argv[3]);
    check_dir("user/recclub/");
    sprintf(filename,"user/recclub/%s.log",tcode);
    fp=fopen(filename,"a+");if (fp==NULL) fp=fopen(filename,"w+");
    if (fp!=NULL) return;
    fprintf(fp,"%s %-8s %-2d\n",
            datestr(2,bank.date),tid,bank.mode,bank.id1,bank.id2,bank.money,bank.title);
	fclose(fp);
}


today_send(char *id)
{
	return 0;
}


