typedef struct {               // 게시물 정보
	int num;				   // 일련 번호
	char id[9]; 			   // 작성자 아이디
	char name[9];			   // 작성자 이름
	time_t date;			   // 작성 일자
	time_t date2;			   // 작성 일자
	int  line;				   // 내용 길이(줄수)
	int  read;				   // 조회수
	int  down;				   // 다운회수
	word ok;				   // 찬성
	char title[61]; 		   // 제목
	int  posi;				   // 내용 포인터
	int  size;				   // 내용 크기
	char key[9];			   // 검색어
	char gd;				   // 추천자료	   0:보통 1:우수 2:최우수
	char del;				   // 삭제여부	   0:보통 1:삭제
    char passwd[5];            // 암호
	char filename[13];		   // 화일명
	int  filesize;			   // 화일크기
	char tcode[9];			   // 임시가기코드
} BBS3;
BBS3 bbs3;
#define sizeof_bbs3 (sizeof(BBS3))
