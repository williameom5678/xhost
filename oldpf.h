// ȸ���� �����̴�.
typedef struct {
	int num;				// ȸ�� �Ϸ� ��ȣ (���ڵ� ��ȣ�� ����)
	char id[9]; 			// ���̵�
	char passwd[9]; 		// ��й�ȣ
	char name[9];			// ����
	int  flag;				// ȸ������ ���� �÷���
	char sex;				// ���� ȸ������
	char birthday[11];		// ������� //64/04/30��
	byte warn;				// ���з�
	byte point	;			// ���
	time_t inday;			// �������� (5����Ʈ��)
	time_t login;			// ���ӽ���
	time_t logout;			// ������ ������
	char id_no[15]; 		// �ֹε�Ϲ�ȣ
	char post[8];			// �����ȣ
	char addr[69];			// ���ּ�
	char tele[69];			// ����ȭ��ȣ
	char offi[69];			// �����
	char memo1[65]; 		// �Ұ�1
	char memo2[65]; 		// �Ұ�2
	char memo3[65]; 		// �Ұ�3
	char memo4[65]; 		// �Ұ�4  �Ұ�5�� ������

	char nick[9];			// ����
	int  cash;				// ���̹�ĳ��
	int  mymode;			// ���� �ڽ��� ����
	char job;				// ��������
	char editor;			// ������
    char sp_5;              // ���� ó��(��ȭ�濡��)
	char lasttty[16];		// ������ ���� ���
    char email[9];          // �û�޸�
    char stype;             // �з�
    char sp_6;              // ���
	time_t left_time;		// ���� �ð�
    int  out_day;           // ��� ��¥
    int  proccode;          // ����ó���ڵ�
	char pay_mode;			// ���� ���
	char cash_mode; 		// ���̹� ĳ��

	char loginmode; 		// �������ѿ���  '1' ���� '2'��� '3'���� '4'ȣ��
	char dm;				// ȭ��ǥ�� ���
	char crt;				// ȭ��ǥ�� �ټ�
	int  down;				// �ٿ�ɿ뷮
	int  countw;			// w,up,down Ƚ�� �� 3�ڸ���.
	int  countu;			// w,up,down Ƚ�� �� 3�ڸ���.
	int  countd;			// w,up,down Ƚ�� �� 3�ڸ���.
	int  level; 			// ���� (0x400)
	int  logcount;			// ����Ƚ��
	time_t totaltime;		// ���̿�ð���� (��)
	time_t today;			// ���� ���̿�ð���� (��)
	time_t payday;			// ȸ�񳳺αⰣ
	char gofirst[9];		// ó������ �޴��ڵ�
	char type;				// ȸ��Ÿ�� (0:�ź� 9:�û�)
	char mailid[9]; 		// ���ϼ��ſ��� ��ĭ:���� * �̸� �ź� �׿� ���̵�
							// ���̵� �����Ǹ� ���ϴ�� �׾��̵� ��ż���
/*
	int num;				// ȸ�� �Ϸ� ��ȣ (���ڵ� ��ȣ�� ����)
	char id[9]; 			// ���̵�
	char passwd[9]; 		// ��й�ȣ
	char name[9];			// ����
	int  flag;				// ȸ������ ���� �÷���
	char sex;				// ���� ȸ������
	char birthday[11];		// ������� //64/04/30��
	byte warn;				// ���з�
	byte point	;			// ���
	time_t inday;			// �������� (5����Ʈ��)
	time_t login;			// ���ӽ���
	time_t logout;			// ������ ������
	char id_no[15]; 		// �ֹε�Ϲ�ȣ
	char post[8];			// �����ȣ
	char addr[69];			// ���ּ�
	char tele[69];			// ����ȭ��ȣ
	char offi[69];			// �����
	char memo1[65]; 		// �Ұ�1
	char memo2[65]; 		// �Ұ�2
	char memo3[65]; 		// �Ұ�3
    char memo4[65];         // �Ұ�4

	char nick[9];			// ����
	int  cash;				// ���̹�ĳ��
	int  mymode;			// ���� �ڽ��� ����
	char job;				// ��������
	char editor;			// ������
    char sp_5;              // ���� ó��(��ȭ�濡��)
	char lasttty[16];		// ������ ���� ���
    char email[9];          // �û�޸�
    char stype;             // �з�
    char sp_6;              // ���
    time_t lefttime;        // ���� �ð�
    int  out_day;           // ��� ��¥
    int  proccode;          // ����ó���ڵ�
    int  paymode;           // ���� ���
	char cash_mode; 		// ���̹� ĳ��

	char loginmode; 		// �������ѿ���  '1' ���� '2'��� '3'���� '4'ȣ��
    char sp_7;              // ȭ��ǥ�� ���
    char sp_8;              // ȭ��ǥ�� �ټ�
	int  down;				// �ٿ�ɿ뷮
	int  countw;			// w,up,down ȸ�� �� 3�ڸ���.
	int  countu;			// w,up,down ȸ�� �� 3�ڸ���.
	int  countd;			// w,up,down ȸ�� �� 3�ڸ���.
	int  level; 			// ���� (0x400)
    int  alllog ;           // ����ȸ��
    time_t alltime ;        // ���̿�ð���� (��)
	time_t today;			// ���� ���̿�ð���� (��)
	time_t payday;			// ȸ�񳳺αⰣ
	char gofirst[9];		// ó������ �޴��ڵ�
	char type;				// ȸ��Ÿ�� (0:�ź� 9:�û�)
	char mailid[9]; 		// ���ϼ��ſ��� ��ĭ:���� * �̸� �ź� �׿� ���̵�
							// ���̵� �����Ǹ� ���ϴ�� �׾��̵� ��ż���
*/
} OLDPF;
OLDPF oldpf;
//����ü ũ�⸦ ��� �������� ����
#define sizeof_oldpf (sizeof(OLDPF))
