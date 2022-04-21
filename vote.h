
// 투표장 정보 화일헤더
typedef struct {			// 게시물 정보
	int num;				// 투표장 번호
	char id[9]; 			// 담당자 아이디
	char nick[9];			// 작성자 별명
	char name[70];			// 투표장 제목
	int  mode;				// 투표형식
	int  open;				// 공개여부
	int  max;				// 최대 선택번호
	int  ans;				// 투표인수
	int  read;				// 읽은회수
	time_t date;			// 작성 일자
	time_t date2;			// 작성 일자
	int avote[100]; 		 // 응답 결과
} VOTE;
#define sizeof_vote (sizeof(VOTE))
VOTE vote;

// 투표장 정보 화일헤더
typedef struct {			// 게시물 정보
	int num;				// 투표장 번호
	char id[9]; 			// 담당자 아이디
	char nick[9];			// 작성자 별명
	int  ans;				// 투표인수
	time_t date;			// 작성 일자
} VOTEMAN;
VOTEMAN voteman;
#define sizeof_voteman (sizeof(VOTEMAN))


