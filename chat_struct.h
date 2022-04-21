#include "my_net.h"
/*
 * chat_struct.h
 * chat ������ �ʿ��� �ڷ� ���� ����
 */

#define SEMA_OFF 'y'						// Semafore off �ñ׳�

typedef struct uinfo{						// ����� ���� ����
	int pid ;						// PID
	char tty[6] ;						// TTY
	char name[10] ;						// ����� �̸� or ����
	char id_code[4] ;					// ���� �ڵ�
        } user_info ;

typedef struct cinfo{						// Chat ������ Info ������ ���� Client ���� ����
	struct sockaddr_in client_address ;			// Client�� �ּ�
	char id_code[4] ;					// ���� �ڵ�
	char buf[10] ;						// ����
        } connect_info ;

typedef struct psig{						// Info ������ Client�� ���� �³� �ڵ�
	char id_code[4] ;					// �����ڵ�
	char buf[10] ;						// ����
        } permit_sig ;

typedef struct ssig{						// Chat ������ Info ������ ���� Semafore off ���
	char id_code[4] ;					// �����ڵ�
	char buf[10] ;						// ����
        } sema_sig ;

