// 회원의 정보이다.
typedef struct {
	int num;				// 회원 일련 번호 (레코드 번호와 동일)
	char id[9]; 			// 아이디
	char passwd[9]; 		// 비밀번호
	char name[9];			// 성명
	int  flag;				// 회원정보 공개 플래그
	char sex;				// 성별 회원구분
	char birthday[11];		// 생년월일 //64/04/30양
	byte warn;				// 경고분류
	byte point	;			// 상벌
	time_t inday;			// 가입일자 (5바이트임)
	time_t login;			// 접속시작
	time_t logout;			// 마지막 접속일
	char id_no[15]; 		// 주민등록번호
	char post[8];			// 우편번호
	char addr[69];			// 집주소
	char tele[69];			// 집전화번호
	char offi[69];			// 직장명
	char memo1[65]; 		// 소개1
	char memo2[65]; 		// 소개2
	char memo3[65]; 		// 소개3
	char memo4[65]; 		// 소개4  소개5는 없어짐

	char nick[9];			// 별명
	int  cash;				// 사이버캐쉬
	int  mymode;			// 각종 자신의 설정
	char job;				// 직업선택
	char editor;			// 편집기
    char sp_5;              // 역상 처리(대화방에서)
	char lasttty[16];		// 마지막 접속 노드
    char email[9];          // 시삽메모
    char stype;             // 분류
    char sp_6;              // 비고
	time_t left_time;		// 남은 시간
    int  out_day;           // 취소 날짜
    int  proccode;          // 내부처리코드
	char pay_mode;			// 납입 방법
	char cash_mode; 		// 사이버 캐쉬

	char loginmode; 		// 접속제한여부  '1' 금지 '2'경고 '3'감시 '4'호출
	char dm;				// 화면표시 모드
	char crt;				// 화면표시 줄수
	int  down;				// 다운가능용량
	int  countw;			// w,up,down 횟수 각 3자리씩.
	int  countu;			// w,up,down 횟수 각 3자리씩.
	int  countd;			// w,up,down 횟수 각 3자리씩.
	int  level; 			// 레벨 (0x400)
	int  logcount;			// 접속횟수
	time_t totaltime;		// 총이용시간기록 (초)
	time_t today;			// 오늘 총이용시간기록 (초)
	time_t payday;			// 회비납부기간
	char gofirst[9];		// 처음가기 메뉴코드
	char type;				// 회원타입 (0:거부 9:시삽)
	char mailid[9]; 		// 메일수신여부 빈칸:정상 * 이면 거부 그외 아이디
							// 아이디가 지정되면 메일대신 그아이디가 대신수신
/*
	int num;				// 회원 일련 번호 (레코드 번호와 동일)
	char id[9]; 			// 아이디
	char passwd[9]; 		// 비밀번호
	char name[9];			// 성명
	int  flag;				// 회원정보 공개 플래그
	char sex;				// 성별 회원구분
	char birthday[11];		// 생년월일 //64/04/30양
	byte warn;				// 경고분류
	byte point	;			// 상벌
	time_t inday;			// 가입일자 (5바이트임)
	time_t login;			// 접속시작
	time_t logout;			// 마지막 접속일
	char id_no[15]; 		// 주민등록번호
	char post[8];			// 우편번호
	char addr[69];			// 집주소
	char tele[69];			// 집전화번호
	char offi[69];			// 직장명
	char memo1[65]; 		// 소개1
	char memo2[65]; 		// 소개2
	char memo3[65]; 		// 소개3
    char memo4[65];         // 소개4

	char nick[9];			// 별명
	int  cash;				// 사이버캐쉬
	int  mymode;			// 각종 자신의 설정
	char job;				// 직업선택
	char editor;			// 편집기
    char sp_5;              // 역상 처리(대화방에서)
	char lasttty[16];		// 마지막 접속 노드
    char email[9];          // 시삽메모
    char stype;             // 분류
    char sp_6;              // 비고
    time_t lefttime;        // 남은 시간
    int  out_day;           // 취소 날짜
    int  proccode;          // 내부처리코드
    int  paymode;           // 납입 방법
	char cash_mode; 		// 사이버 캐쉬

	char loginmode; 		// 접속제한여부  '1' 금지 '2'경고 '3'감시 '4'호출
    char sp_7;              // 화면표시 모드
    char sp_8;              // 화면표시 줄수
	int  down;				// 다운가능용량
	int  countw;			// w,up,down 회수 각 3자리씩.
	int  countu;			// w,up,down 회수 각 3자리씩.
	int  countd;			// w,up,down 회수 각 3자리씩.
	int  level; 			// 레벨 (0x400)
    int  alllog ;           // 접속회수
    time_t alltime ;        // 총이용시간기록 (초)
	time_t today;			// 오늘 총이용시간기록 (초)
	time_t payday;			// 회비납부기간
	char gofirst[9];		// 처음가기 메뉴코드
	char type;				// 회원타입 (0:거부 9:시삽)
	char mailid[9]; 		// 메일수신여부 빈칸:정상 * 이면 거부 그외 아이디
							// 아이디가 지정되면 메일대신 그아이디가 대신수신
*/
} OLDPF;
OLDPF oldpf;
//구조체 크기를 상수 수식으로 선언
#define sizeof_oldpf (sizeof(OLDPF))
