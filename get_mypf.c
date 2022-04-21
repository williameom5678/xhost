////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 게시판 담당자의 메뉴
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"
main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
    PF oldpf;
    char buff[100];
    char filename[100];

    if (argc<2) {
        printf("\n사용법  get_mypf <화일명> [tty]\n");
        printf("\n출력결과-> 화일에 아이디 이름 별명 등급 레벨 다운 머니 시간 생일 순으로 기록\n");
        getchar();
        return;
    }
    strcpy(filename,argv[1]);              //가기코드
    unlink(filename);
    if (argc==3) strcpy(tty,argv[2]);
    else         findport(tty); //접속포트 찾기
    if (!findttypf(pf.id,pf.name,tty)) {     //누가 이 메뉴를 사용하는지를 파악함
        mess("%s 포트 사용자를 찾을수 없습니다.",tty);
		return;
	}
    oldpf=pf;
    if (!read_pf(pf.id)) {
        strcpy(pf.id,"........");
        strcpy(pf.name,"........");
        strcpy(pf.nick,"........");
        pf.type=0;
        pf.level=0;
        pf.cash=0;
        pf.down=0;
        pf.left_time=0;
        mess("%s 회원을 찾을수 없습니다.",oldpf.id);
    }
    if ((fp=fopen(filename,"w"))!=NULL) {
        fprintf(fp,"%s %s %s %d %d %d %d %d %s 0 0 0 0\n",pf.id,pf.name,pf.nick,pf.type,pf.level,pf.down,pf.cash,pf.left_time,dateyang(11,pf.birthday,pf.moon));
        fclose(fp);
    }
}

findttypf(char *id,char *name, char*tty)	//누가 이 메뉴를 사용하는지를 파악함
{
	FILE *fp;
	MONI moni2;
    int i,n,ret=No;
    char filename[100];
	strcpy(id,"");strcpy(name,"");

    for (n=0;n<20;n++) {
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename, "r"))==NULL) continue;
        while(fread(&moni2,sizeof_moni,1,fp)) {
            if (moni2.mode>0&&
                strlen(moni2.id)>0&&
                moni2.hostno==cfg.hostno&&
                is_same(moni2.tty,tty)) {
                strcpy(id,moni2.id);
                strcpy(name,moni2.name);
                ret=Yes;
                break;
            }
        }
        fclose(fp);
    }
	return ret;
}


