
// XHOST 개발:김성대 1996.10.15 부터 개발시작함
// 리눅스/유닉스용 멀티노드 호스트
#ifndef XHOST_CHAT
#define XHOST_CHAT

					// Client가 사용할 Port

//대화방 전체 인원
typedef struct {
	char id    [13];	//개설자의 아이디
	char name  [13];	//개설자의 아이디
	char nick  [13];	//별명
	char tty   [10];
	char image	   ;	//개인그림번호
	char imageact  ;	//개인동작그림번호
	char hostname[21];
	char note  [31];	//자기설명
	int  point	   ;	//자기점수
	int  score	   ;
	byte check	   ;
	byte sex	   ;
	byte age	   ;
	byte area	   ;
	byte hostno    ;
	byte mode	   ;	//개인별 모드
	byte color1    ;	//칼라지정
	byte color2    ;	//칼라지정
	char age2[10]  ;	//~부터~까지 나이 문자열
	byte sp_2;
	byte sp_3;
	byte sp_4;
	byte sp_5;
	int  pid;			//PID
	int  sp_7;
	int  sp_8;
	int  sp_9;
	time_t date    ;	//진입시간
	time_t ctime   ;	//마지막입력시간
	time_t rtime   ;	//복권입력시간
	time_t gtime   ;	//게임시작시간
} ROOM;
ROOM room;
#define sizeof_room (sizeof(ROOM))

//대화방 전체 인원
typedef struct {
	int  roomnum;
	byte roomtype  ;	//방종류
	byte count	   ;	//현재인원
	byte max	   ;	//최대인원수
	char id[13]    ;	//개설자의 아이디
	char name[13]  ;	//개설자의 아이디
	char hostname[21];	//방장의 호스트명
	char boss2[13] ;	//보조방장
	byte dm 	   ;	//출력방법
	byte openmode  ;	//방별모드
	byte wmode	   ;	//방별모드 (인터넷모드)
	byte dispmode  ;	//출력방법
	time_t date    ;	//개설시간
	char title[151];	//제목
	char stitle[61];	//보조제목
	char ctitle[21];	//모임명
	char passwd[5] ;	//진입시 암호 지정
	char image	   ;	//배경그림번호
	byte itype	   ;	//진입등급
	byte age1	   ;	//진입나이
	byte age2	   ;	//진입나이
	char sex	   ;	//진입성별
	byte rcount    ;	//복권출력시간카운터
	char club[13]  ;	//동호회
	char sp_1	   ;
	char sp_2	   ;
	char sp_3	   ;
	char sp_4	   ;
	char sp_5	   ;
	int  sp_6	   ;
	int  sp_7	   ;
	int  sp_8	   ;
	int  sp_9	   ;
	int  sp_10	   ;
	char sbuff1[61];
	char sbuff2[61];
	char sbuff3[13];
	char sbuff4[13];
	char sbuff5[13];
	int  troom[10] ;	//다른방으로 FORWORDING
	ROOM room[20]  ;	//접속자
} CROOM;
CROOM croom;
#define sizeof_croom (sizeof(CROOM))


//대화방 전체 인원
typedef struct {
	int  roomnum   ;
	byte type1	   ;	//방종류
	byte type2	   ;	//방종류
	byte type3	   ;	//방종류
	char id[13]    ;	//개설자의 아이디
	char title[151];	//제목
	char passwd[13];	//암호
	char email[21] ;	//담당Email
	byte html_l    ;	//왼쪽Html
	byte html_t    ;	//상단Html
	byte html_r    ;	//return Html (방QUIT)
	byte openmode  ;	//방별모드
	int  mode	   ;	//방별모드
	int  point	   ;	//점수
	int  badpoint  ;	//감점
	time_t date    ;	//개설시간
	time_t ltime   ;	//마지막 접속시간
	int  count	   ;	//총접속수
	int  count1    ;	//총접속수 이번달
	int  count2    ;	//총접속수 지난번달
	int  uset	   ;	//총사용수
	int  uset1	   ;	//총사용수 이번달
	int  uset2	   ;	//총사용수 지난번달
} UROOM;
#define sizeof_uroom (sizeof(UROOM))


//대화실기록
typedef struct {			//게시물 정보
	time_t date1;			//접속시간
	time_t date2;			//나간시간
	char id[13];			//아이디
	char name[13];			//이름
	char nick[13];			//대화명
	char tty[10] ;			//대화명
	byte sex	 ;			//성별
	byte age	 ;			//나이
	byte area	 ;			//지역
	char hostname[21];		//호스트명
} CUS;


//대화실기록
typedef struct {			//게시물 정보
    int  today;             //오늘 접속
	int  count; 			//총접속기록
	int  ttime; 			//총접속시간
	time_t date1;			//개설날짜
	time_t date2;			//최종사용날짜
	int  acount[(12-1)*31*25+31*25+23+3];  //달,일,시간
	int  atime [12*32	];	//달,일
	int  sp_1[100]; 		//예비
	char sp_2[200]; 		//예비
	CUS  cus[50];			//50명 이전접속자 기록
} CREC;
#define sizeof_crec (sizeof(CREC))


char ROOM_FILE[200];
int  roomtype,roomnum;
char my_mode;
char my_color1;
char my_color2;
time_t room_sec;
static char *acstr[20]= { "흑","푸","연","감","주",
						  "보","황","하","회","군",
						  "녹","청","적","분","노",
						  "백","백","백","백","백"};

int quit_chat,bye_chat,cont_chat ; /* 대화실에서 /x로 바로 종결함. */
int now_st;
int c_num1,c_num2,c_num3;
char st_mode;
char para_file[300];
byte mynum;
char *myroomall();
char *myroomdata();
struct termio term_old;
char taja_on,quiz_on,xrace_on;
char re_chat;
byte cfg_hostno;
char moni_chat;
char game_mode;
char chaty1,chaty2;    // 대화방 Y위치
#define ALARM_TIME 60
#define MAX_OUT (30*60)

char userid[21];
char userna[21];
char userni[21];
char tosay[21];
char userage[10];
char userarea;

#endif	/* XHOST_H */
