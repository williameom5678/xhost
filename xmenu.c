////////////////////////////////////////////////////////////////////////
// XHOST: 리눅스용 BBS HOST  개발:김성대(053-964-1002) BBS:053-963-2190
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"
main(argc, argv)
int argc;
char *argv[];
{
    char buff[500];
    char logo_file[500];
    char help_file[500];
    char filename[500];
    int n,who;
	char tcode[9];
	char ccode[9];
	char id[9];

    club_mode=!is_same(club_go.code,"main");

    strcpy(ccode,argv[1]);              //동호회 코드
    strcpy(tcode,argv[2]);              //가기코드
    strcpy(id,argv[3]);                 //아이디
	findport(tty); //접속포트 찾기
	read_cfg();
	;
	set_tmpfile();		 //임시화일지정하기
	set_nowmenu();		 //데이타화일지정하기
	if (!find_club(ccode)) {
		mess("%s 동호회 검색 실패",ccode);
		return;
	}
	if (!find_go(tcode)) {
		mess("%s 가기코드 검색 실패",tcode);
		return;    //찾는 가기코드가 없다
	}

	read_ccfg();
	set_bcfgfile(); 					//BCFG_FILE에 저장
	read_bcfg();
	if (!read_pf(id)) {
		mess("%s 검색실패",id);
		return;
	}
	read_cpf(id);					 //해당되는 동호회의 아이디를 찾는다.
	read_atype();					 //등급읽기
	read_actype();					 //동호회등급의 이름을 읽어옴  ->actype
	who=check_who();				 //1:동호회 운영진 2:동호회 시삽  3:root권한

    mview(Yes,"%s/logo/%s.vtx",club_go.dir,go.code);
    mview(Yes,"%s/logo/%s.jpg",club_go.dir,go.code);

    sprintf(logo_file,"%s/logo/%s.mnu",club_go.dir,go.code);
    sprintf(help_file,"%s/logo/%s.hlp",club_go.dir,go.code);
    cls();
	while(1) {
        view(logo_file);
        lineinput(buff,5);printf("\n");
        check_han(buff);
		n=atoi(buff);
        if ((is_same(buff,"cfg")||is_same(buff,".s")||
            is_same(buff,".sys")||is_same(buff,".s"))&&
            (club_mode&&cpf.type>15||!club_mode&&pf.type>15)) edit_scr();
        if (is_char("XxQqQPp",buff[0])) break;
        if (is_char("Hh",buff[0])) view(help_file);
        if (is_same(buff,"0")||(n>0&&n<99)) {
            sprintf(filename,"%s/logo/%s.%d",club_go.dir,go.code,n);
            if (!view(filename)) {
                sprintf(filename,"%s/logo/%s.err",club_go.dir,go.code);
                view(filename);
            }
        }
	}
}


edit_scr()
{
	FILE *fp;
    int n=0,i,n2=0;
	int key;
	int can_edit=No;		//편집가능함?
	char buff[240];
	char tcode[60];
	char now_id[60];
	char filename[200];
	char filename2[200];
	char id_file[200];
	char cfg_file[200];
	char dir[200];
	char cmd;

	strcpy(tcode,go.code);			  //가기코드
    sprintf(dir,"%s/logo",club_go.dir);
	while (1) {
		cls();
        printf("   ### %s 화면 편집 메뉴  \n",tcode);
        printf(" 진입로고  (%s.log)    1.직접편집   2.등록   3.다운   4.삭제 \n",tcode);
        printf(" 메인화면  (%s.mnu)   11.직접편집  12.등록  13.다운  14.삭제 \n",tcode);
        printf(" 선택오류  (%s.err)   21.직접편집  22.등록  23.다운  24.삭제 \n",tcode);
        printf(" 도 움 말  (%s.hlp)   31.직접편집  32.등록  33.다운  34.삭제 \n",tcode);
        printf(" 선택화면  (%s.???)   41.직접편집  42.등록  43.다운  44.삭제 \n",tcode);
        printf(" 진입VTX   (%s.vtx)   51.직접편집  52.등록  53.다운  54.삭제 \n",tcode);
        printf(" 진입JPG   (%s.jpg)                62.등록  63.다운  64.삭제 \n",tcode);
        n=numselect();
        if (n/10==4) {
            printf(" 편집할 화면의 선택번호는? (%s.???) 취소(Q) >>",tcode);
            lineinput(buff,5);printf("\n");
            n2=atoi(buff);
            if (is_char("qQpPxX",buff[0])||buff[0]==0) continue;
        }
        if      (n== 0) break;
        else if (n< 10) sprintf(filename,"%s/%s.log",dir,tcode);
        else if (n< 20) sprintf(filename,"%s/%s.mnu",dir,tcode);
        else if (n< 30) sprintf(filename,"%s/%s.err",dir,tcode);
        else if (n< 40) sprintf(filename,"%s/%s.hlp",dir,tcode);
        else if (n< 50) sprintf(filename,"%s/%s.%d",dir,tcode,n2);
        else if (n==51) continue;
        else if (n< 60) sprintf(filename,"%s/%s.vtx",dir,tcode);
        else if (n==61) continue;
        else if (n< 70) sprintf(filename,"%s/%s.jpg",dir,tcode);

		if (n%10==1) {
			xedit(filename);
		}
		if (n%10==2) {
            up_text(0,filename);   //화일을 전송받아 filename에 저장한다.
		}
        if (n%10==3) sz_file(filename);   //화일전송
		if (n%10==4) {
			if (is_file(filename)) {
				printf(" %s 화일을 삭제합니다.\n",filename);
				if (yesno("삭제를 할까요? (Y:삭제)",Yes)) {
                    printf("\n %s 화일을 삭제했습니다.\n",filename);
					unlink(filename);
				}
			}
			else {
                printf("\n %s 화일이 없습니다.\n",filename);
                pressenter();
			}
		}
	}
}

