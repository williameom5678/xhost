
// ��ǥ�� ���� ȭ�����
typedef struct {			// �Խù� ����
	int num;				// ��ǥ�� ��ȣ
	char id[9]; 			// ����� ���̵�
	char nick[9];			// �ۼ��� ����
	char name[70];			// ��ǥ�� ����
	int  mode;				// ��ǥ����
	int  open;				// ��������
	int  max;				// �ִ� ���ù�ȣ
	int  ans;				// ��ǥ�μ�
	int  read;				// ����ȸ��
	time_t date;			// �ۼ� ����
	time_t date2;			// �ۼ� ����
	int avote[100]; 		 // ���� ���
} VOTE;
#define sizeof_vote (sizeof(VOTE))
VOTE vote;

// ��ǥ�� ���� ȭ�����
typedef struct {			// �Խù� ����
	int num;				// ��ǥ�� ��ȣ
	char id[9]; 			// ����� ���̵�
	char nick[9];			// �ۼ��� ����
	int  ans;				// ��ǥ�μ�
	time_t date;			// �ۼ� ����
} VOTEMAN;
VOTEMAN voteman;
#define sizeof_voteman (sizeof(VOTEMAN))


