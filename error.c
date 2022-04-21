////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 내용 : ERROR 상태 확인 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

// 개발자 상황보기
dev()
{
    char OLDDIR[300];
	getcwd(OLDDIR,sizeof(OLDDIR));		//현재의 디렉토리
    printf("\n");
    printf("동호회  :%s %s\n",club_go.code,club_go.dir);
    printf("가기코드:%s*%s*(타입%c  깊이%d No:%d)\n",go.code,go.name,go.type,go.deep,go.rec);
	printf("디렉토리:%s  (현재:%s)\n",go.dir,OLDDIR);
    printf("메뉴파일:%s\n",NOW_MENU);
    printf("NOW_FILE:%s(%d) %s(%d)\n",NOW_FILE,file_size(NOW_FILE),NOW_FILED,file_size(NOW_FILED));
    printf("TMP_FILE:%s(%d) %s(%d)\n",TMP_FILE,file_size(TMP_FILE),TMP_FILE2,file_size(TMP_FILE2));
	printf("게시판담당:%s  동호회담당:%s\n",bcfg.man1,ccfg.sysop);
	printf("작업아이디:%s\n",pf.id);
	printf("등급    :%d\n",now_type);
	printf("작업레벨:%d \n",pf.level);
	printf("작업등급:%d \n",pf.type);
	printf("re_club :%d\n",re_club );
	printf("re_print:%d\n",re_print);
	printf("re_bbs  :%d\n",re_bbs  );
	printf("re_logo :%d\n",re_logo );
	printf("club_mode:%d\n",club_mode);
	printf("list_mode:%d\n",list_mode);
	getchar();
}


check_error(int mode)
{
    if (is_file("system/xcheck9")) {
        cls();
        dev();
        printf(" %s %s\n",VER1,VER2);
        printf(" mode=%d",mode);system("id");
        printf(" 아이디NO:%d 아이디:%s 성명:%s 번호:%d TTY:%s\n",pf.num+1,pf.id,pf.name,pf.num+1,tty);
        printf(" us:%s %s %s\n",moni.id,moni.name,moni.gocode);
        printf(" 위의 화면을 갈무리 해서 운영자에게 알려 주시기 바랍니다.\n");
        pressenter();
    }
}


