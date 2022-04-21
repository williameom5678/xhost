#ifndef XHOST_PFHEADER
#define XHOST_PFHEADER

#ifndef XHOST_H
#define byte	unsigned char
#define word	unsigned int
#define No 0
#define Yes 1

#define Nostop 0
#define NoStop 0
#define Stop   1
char tty[20]; // 접속 포트
#endif	/* XHOST_HEADER */


// 회원의 정보이다.
typedef struct {
	int    num; 			  //회원 일련 번호 (레코드 번호와 동일)
	int    idnum;			  //회원고유번호   (시작부터 새로 설정됨)
	char   id[9];			  //아이디
	char   passwd[9];		  //비밀번호
	char   nick[9]; 		  //별명
	char   name[9]; 		  //성명
	char   email[9];		  //email
	char   mailid[41];		  //
	char   sex; 			  //성별 회원구분
	time_t birthday;		  //생년월일
	time_t wbirthday;		  //생년월일
	time_t marryday;		  //생년월일
	char   moon;			  //양/음  0:양  1:음
	char   wmoon;			  //양/음  0:양  1:음
    char   job;               //직업선택
	char   gofirst[13]; 	  //처음가기 메뉴코드

	char   memo1[41];		  //취미
    char   memo2[66];         //취미  ->없앰
    char   sp_1[66];         //취미  ->없앰
    char   user1[9];
    char   user2[21];
    char   user3[21];
    char   user4[41];

	char   id_no[15];		  //주민등록번호
	char   pos1[8]; 		  //우편번호
	char   addr[71];		  //집주소
	char   pos2[8]; 		  //우편번호
	char   add2[71];		  //집주소
	char   tele[21];		  //집전화번호
	char   bibi[15];		  //삐삐
	char   hand[15];		  //핸드폰
	char   offi[71];		  //직장명
	char   otel[21];		  //집전화번호
	char   wifeid[9];		  //배우자ID

	char   type;			  //회원타입 (0:거부 9:시삽)
	byte   istat;			  //회원분류 (정상이용자)
	byte   itype;			  //회원분류 (유료이용자(개인))
	byte   loginmode;		  //접속제한여부  '1' 금지 '2'경고 '3'감시 '4'호출
	byte   warn;			  //경고분류
	byte   point;			  //상벌
    int    good;              //우수점수
	int    level;			  //레벨
	int    myflag;			  //회원정보 공개 플래그

	time_t inday;			  //가입일자
	time_t login;			  //접속시작
	time_t logout;			  //마지막접속일
	int    all_log; 		  //총접속회수
	int    month_log;		  //이번달총접속회수
	int    all_time;		  //총이용시간기록	(초)
	int    month_time;		  //이번달총이용시간(초)
    int    left_time;         //

	int    mymode;			  //각종 자신의 설정
	int    homemode;		  //사이버홈
	byte   offmin;			  //키입력없을때 OFF(분)
	int    paymode; 		  //납입방법
    int    todaymode;         //
    int    cashmode;          //CASH모드
    word   p_ph;              //체력
    word   p_it;              //지력
    int    cash;              //사이버머니
    word   p_at;              //매력
    word   p_pr;              //방어력
    int    p_mo;              //모드

	int    down;			  //다운가능용량
	int    countw;			  //WRITE
	int    countu;			  //UPLOAD
	int    countd;			  //DOWNLOAD

    int    today;             //오늘쓰기  접속:3 쓰기:3 다운:3
    int    wwrite;            //웹에서 적은숫자
    byte   localmenu;         //지역별메뉴
    char   tmail[9];          //다른사람ID
    char   sp_4;
} PF;
PF pf;
PF pf2;
//구조체 크기를 상수 수식으로 선언
#define sizeof_pf (sizeof(PF))
#define sizeof_pf2 (sizeof(PF2))


typedef struct {
    int    num;               //회원 일련 번호 (레코드 번호와 동일)
    int    idnum;             //회원고유번호   (시작부터 새로 설정됨)
    char   id[9];             //아이디
} SMALLPF;
#define sizeof_smallpf (sizeof(SMALLPF))

// 회원인덱스 정보이다.
typedef struct {
	char id[9]; 			// 아이디
	char cnum[7];			// 회원 일련 번호 (레코드 번호와 동일)
	char ret;
} IND;
//구조체 크기를 상수 수식으로 선언
#define sizeof_ind (sizeof(IND))

// 회원인덱스 정보이다.
typedef struct {
	char id[9]; 			// 아이디
    int  cnum;           // 회원 일련 번호 (레코드 번호와 동일)
} IND2;
#define sizeof_ind2 (sizeof(IND2))

// 동호회 회원의 정보이다.
typedef struct {
	int num;				// 회원 일련 번호 (레코드 번호와 동일)
	char id[9]; 			// 아이디
	char name[9];			// 이름
	char memo1[65]; 		// 소개1
	char memo2[65]; 		// 소개2
	char memo3[65]; 		// 소개3
	char memo4[65]; 		// 소개4
	char memo5[65]; 		// 소개5
	time_t inday;			// 가입일자 (0x04)	 (5바이트임)
	time_t login;			// 마지막 접속일
	time_t logout;			// 마지막 접속일
    int  countw;            // w,up,down 회수
    int  countu;            // w,up,down 회수
    int  countd;            // w,up,down 회수
	int  all_log;			// 접속회수
    int  all_time;          // 총이용시간기록 (분)
	char type;				// 동호회의 회원타입 (0:거부 9:시삽)
	int  level;
	int  mymode;
    int  sp3      ;         // 남은 시간
    int  sp4     ;          // 남은 날짜
    int  sp5     ;          // 남은 접속수
	char pay_mode;			// 납입 방법
	char cash_mode; 		// 사이버 캐쉬
    int  cash;              // 사이버캐쉬
    int  month_log;         //이번달총접속회수
    int  month_time;        //이번달총이용시간(초)
    int  sp1;
    int  sp2;
	byte point; 			// 포인터
	char sp6;
	char sp7;
	char warn;
} CPF;

CPF cpf;
//구조체 크기를 상수 수식으로 선언
#define sizeof_cpf (sizeof(CPF))


#endif	/* XHOST_PFHEADER */
