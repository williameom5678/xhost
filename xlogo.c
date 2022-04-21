////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 화면 통합관리
///////////////////////////////////////////////////////////////////////////

#include "bbs.h"
#include <dirent.h>

typedef struct {
    char mode;             //있는가?
    char filename[13];
    char title[41];
    int  filesize;
    time_t date;
} XFRM;
XFRM xfrm;
#define sizeof_xfrm (sizeof(XFRM))
XFRM afrm[3000];

char fdir[300];
char ndir[300];
char ntit[300];
int total;
int total_size;
int re_load;
char datafile[500];
char afilename[10][300];

main(argc, argv)
int argc;
char *argv[];
{
    int  i,n;
    char buff[300];
    char buff2[300];
    char abuff[30][100];
    char adir[30][20];
    char atit[30][40];
    char tcode[9];
    char ccode[9];
    char now_id[9];

	findport(tty); //접속포트 찾기
	read_cfg();
    ;
	set_tmpfile();		 //임시화일지정하기
    set_nowmenu();       //데이타화일지정하기

    strcpy(ccode,argv[1]);              //동호회 코드
    strcpy(tcode,argv[2]);              //동호회 코드
    strcpy(now_id,argv[3]);             //아이디

    if (!find_club(ccode)) {
		mess("%s 동호회 검색 실패",ccode);
		return;
	}
	read_ccfg();
    read_pf(now_id);

    strcpy(buff,"");
    if (club_mode) sprintf(buff,"%s/",club_go.dir);
    sprintf(afilename[0],"%slogo/%s.mnu",buff,tcode);
    sprintf(afilename[1],"%slogo/%s.mn2",buff,tcode);
    sprintf(afilename[2],"%slogo/%s.mn3",buff,tcode);
    sprintf(afilename[3],"%slogo/%s.mn4",buff,tcode);
    sprintf(afilename[4],"%slogo/%s.mn5",buff,tcode);

    //데이타 파일읽어오기 -> 파일크기.최종날짜확인
    while (1) {
        cls();
        printf(" [메뉴 화면 편집]       %s(%s) %s.mnu\n",club_go.code,club_go.dir,tcode);
        for (i=0;i<5;i++) {
            printf(" %d. %s %6d Byte\n",i+1,afilename[i],file_size(afilename[i]));
        }
        printf("  편집(E) 삭제(D) 교체(CH) 복사(CP) 등록(UP) 다운(DN) \n");
        printf("  선택>>");
        lineinput(buff,20);check_han(buff);printf(buff);check_cmd(buff);
        if      (is_same(mmc[0],"e"))                      edit_logo();
        else if (is_same(mmc[0],"d"))                      delete_logo();
        else if (is_same(mmc[0],"ch"))                     change_logo();
        else if (is_same(mmc[0],"cp"))                     copy_logo();
        else if (is_same(mmc[0],"up"))                     upload_logo();
        else if (is_same(mmc[0],"dn"))                     dnload_logo();
        else if (mmn[0]>0)                                 disp_logo(mmn[0]);
        else if (strlen(buff)<1);
        else break;
    }
}


edit_logo()
{
    if (mmn[1]<1||mmn[1]>5) mmn[1]=1;
    xedit(afilename[mmn[1]-1]);
}


disp_logo(int n)
{
    if (n<1||n>5) n=1;
    view(afilename[n-1]);
    pressenter();
}


delete_logo()
{
    if (mmn[1]<1||mmn[1]>5) return;
    view(afilename[mmn[1]-1]);
    if (yesno("\n 이 파일을 삭제할까요? (y/N)>>",No)) {
        unlink(afilename[mmn[1]-1]);
    }
}


change_logo()
{
    if (mmn[1]<1||mmn[1]>5) return;
    if (mmn[2]<1||mmn[2]>5) return;
    printf(" %s,%s 파일을 서로 교체할까요? (Y/n)>>",afilename[mmn[1]-1],afilename[mmn[2]-1]);
    if (yesno("",Yes)) {
        unlink(TMP_FILE);
        view(afilename[mmn[1]-1]);
        file_move(afilename[mmn[1]-1],TMP_FILE);
        file_move(afilename[mmn[2]-1],afilename[mmn[1]-1]);
        file_move(TMP_FILE,afilename[mmn[2]-1]);
    }
}


copy_logo()
{
    if (mmn[1]<1||mmn[1]>5) return;
    if (mmn[2]<1||mmn[2]>5) return;
    if (!view(afilename[mmn[1]-1])) return;
    printf("\n %s 파일을 %s 로 복사할까요? (Y/n)>>",afilename[mmn[1]-1],afilename[mmn[2]-1]);
    if (yesno("",Yes)) {
        file_copy(afilename[mmn[1]-1],afilename[mmn[2]-1]);
    }
}


upload_logo()
{
    char filename[300];
    char filename2[300];
    int key;
    if (mmn[1]<1||mmn[1]>5) return;
    key=up_file(0,filename,No);   //화일을 전송받아 filename에 저장한다.
    if (key&&is_file(filename)) {
        view(filename);
        printf("\n 이 파일을 %s 로 복사할까요? (Y/n)>>",afilename[mmn[1]-1]);
        if (yesno("",Yes)) {
            file_copy(filename,afilename[mmn[1]-1]);
        }
        sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
    }
}


dnload_logo()
{
    if (mmn[1]<1||mmn[1]>5) return;
    sz_file(afilename[mmn[1]-1]);
}
