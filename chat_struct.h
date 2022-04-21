#include "my_net.h"
/*
 * chat_struct.h
 * chat 구현시 필요한 자료 구조 선언
 */

#define SEMA_OFF 'y'						// Semafore off 시그널

typedef struct uinfo{						// 사용자 정보 구조
	int pid ;						// PID
	char tty[6] ;						// TTY
	char name[10] ;						// 사용자 이름 or 계정
	char id_code[4] ;					// 인증 코드
        } user_info ;

typedef struct cinfo{						// Chat 서버가 Info 서버에 보낼 Client 접속 정보
	struct sockaddr_in client_address ;			// Client의 주소
	char id_code[4] ;					// 인증 코드
	char buf[10] ;						// 예비
        } connect_info ;

typedef struct psig{						// Info 서버가 Client에 보낼 승낙 코드
	char id_code[4] ;					// 인증코드
	char buf[10] ;						// 예비
        } permit_sig ;

typedef struct ssig{						// Chat 서버가 Info 서버에 보낼 Semafore off 명령
	char id_code[4] ;					// 인증코드
	char buf[10] ;						// 예비
        } sema_sig ;

