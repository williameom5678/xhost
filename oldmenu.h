// 가기코드 표
typedef struct {
	char deep;				//메뉴의 깊이 0:top
	char type;				//메뉴 형태
	char num[5];			//선택 번호
	char code[9];			//가기 코드
	char name[31];			//메뉴 제목
	char temp[16];			//예비기능
	char wtimes[4]; 		//일일 적는 회수제한
	int  level; 			//레벨 증가분
	char ma_[9];			//담당자
	char rtype; 			//읽을수 있는 등급
	char wtype; 			//적을수 있는 등급
	char dtype; 			//다운 받을수 있는 등급
	char dir[61];			//화일위치 디렉토리
	int  passwd;			//예비기능
	char itype; 			//진입가능
}OLDGO;
OLDGO oldgo;

#define sizeof_oldgo  (sizeof(OLDGO))

typedef struct {
    char man1   [9];            //담당자1
    char man2   [9];            //담당자1
	char man3	[9];			//담당자1
	char man4	[9];			//담당자1
	char man5	[9];			//담당자1
	char pass	[9];			//시삽메뉴의 암호
	int  passwd;				//진입암호
	int  intime;				//가능 시간
	int  mode	   ;			//화면출력모드
	char headnum   ;		   //기본선택머리말
	char sp1   [ 8];		   //비
	char sp2   [ 9];		   //비
	char sp3   [14];		   //비
	int  listmode  ;
	char itemlen[50];		   //칸수별정의
	int  bbssize   ;		   //게시물크기 제한
	int  cashtime1 ;
	int  rcash1    ;
	int  rcash2    ;
	int  rcash3    ;
	int  rsize1    ;
	int  freeday   ;
	char freetype1 ;
	int  cashtime2 ;
	int  wcash1    ;
	int  wcash2    ;
	int  wcash3    ;
	int  wsize1    ;
	char freetype2 ;
	char rrate1    ;
	char rrate2    ;
	char rrate3    ;
	char rrate4    ;
	char rrate5    ;
	char cashmain  ;
	char cashmode1 ;
	char cashmode2 ;
	int  cashmode  ;
	char listposi  ;
	char listline  ;
	char rcash4    ;
	char wcash4    ;
	int  wcashmax  ; //최대값 쓰기
	int  rcashmax  ; //최대값 다운
	int  wcashmode ;
	int  rcashmode ;
	char pos_x	   ; //화상위치
	char pos_y	   ; //화상위치
    int  outday    ; //지난날짜 삭제함
    byte wdown     ; //업보상 비율
    byte rdown     ; //다운줄어듬 비율
    byte downfree  ; //자유다운/읽기등급
    byte freeup    ; //자유등록/쓰기등급
} BBSCFG;
BBSCFG bbscfg;		 //동호회 환경파일
#define sizeof_bbscfg (sizeof(BBSCFG))
