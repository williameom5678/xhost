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
char tty[20]; // ���� ��Ʈ
#endif	/* XHOST_HEADER */


// ȸ���� �����̴�.
typedef struct {
	int    num; 			  //ȸ�� �Ϸ� ��ȣ (���ڵ� ��ȣ�� ����)
	int    idnum;			  //ȸ��������ȣ   (���ۺ��� ���� ������)
	char   id[9];			  //���̵�
	char   passwd[9];		  //��й�ȣ
	char   nick[9]; 		  //����
	char   name[9]; 		  //����
	char   email[9];		  //email
	char   mailid[41];		  //
	char   sex; 			  //���� ȸ������
	time_t birthday;		  //�������
	time_t wbirthday;		  //�������
	time_t marryday;		  //�������
	char   moon;			  //��/��  0:��  1:��
	char   wmoon;			  //��/��  0:��  1:��
    char   job;               //��������
	char   gofirst[13]; 	  //ó������ �޴��ڵ�

	char   memo1[41];		  //���
    char   memo2[66];         //���  ->����
    char   sp_1[66];         //���  ->����
    char   user1[9];
    char   user2[21];
    char   user3[21];
    char   user4[41];

	char   id_no[15];		  //�ֹε�Ϲ�ȣ
	char   pos1[8]; 		  //�����ȣ
	char   addr[71];		  //���ּ�
	char   pos2[8]; 		  //�����ȣ
	char   add2[71];		  //���ּ�
	char   tele[21];		  //����ȭ��ȣ
	char   bibi[15];		  //�߻�
	char   hand[15];		  //�ڵ���
	char   offi[71];		  //�����
	char   otel[21];		  //����ȭ��ȣ
	char   wifeid[9];		  //�����ID

	char   type;			  //ȸ��Ÿ�� (0:�ź� 9:�û�)
	byte   istat;			  //ȸ���з� (�����̿���)
	byte   itype;			  //ȸ���з� (�����̿���(����))
	byte   loginmode;		  //�������ѿ���  '1' ���� '2'��� '3'���� '4'ȣ��
	byte   warn;			  //���з�
	byte   point;			  //���
    int    good;              //�������
	int    level;			  //����
	int    myflag;			  //ȸ������ ���� �÷���

	time_t inday;			  //��������
	time_t login;			  //���ӽ���
	time_t logout;			  //������������
	int    all_log; 		  //������ȸ��
	int    month_log;		  //�̹���������ȸ��
	int    all_time;		  //���̿�ð����	(��)
	int    month_time;		  //�̹������̿�ð�(��)
    int    left_time;         //

	int    mymode;			  //���� �ڽ��� ����
	int    homemode;		  //���̹�Ȩ
	byte   offmin;			  //Ű�Է¾����� OFF(��)
	int    paymode; 		  //���Թ��
    int    todaymode;         //
    int    cashmode;          //CASH���
    word   p_ph;              //ü��
    word   p_it;              //����
    int    cash;              //���̹��Ӵ�
    word   p_at;              //�ŷ�
    word   p_pr;              //����
    int    p_mo;              //���

	int    down;			  //�ٿ�ɿ뷮
	int    countw;			  //WRITE
	int    countu;			  //UPLOAD
	int    countd;			  //DOWNLOAD

    int    today;             //���þ���  ����:3 ����:3 �ٿ�:3
    int    wwrite;            //������ ��������
    byte   localmenu;         //�������޴�
    char   tmail[9];          //�ٸ����ID
    char   sp_4;
} PF;
PF pf;
PF pf2;
//����ü ũ�⸦ ��� �������� ����
#define sizeof_pf (sizeof(PF))
#define sizeof_pf2 (sizeof(PF2))


typedef struct {
    int    num;               //ȸ�� �Ϸ� ��ȣ (���ڵ� ��ȣ�� ����)
    int    idnum;             //ȸ��������ȣ   (���ۺ��� ���� ������)
    char   id[9];             //���̵�
} SMALLPF;
#define sizeof_smallpf (sizeof(SMALLPF))

// ȸ���ε��� �����̴�.
typedef struct {
	char id[9]; 			// ���̵�
	char cnum[7];			// ȸ�� �Ϸ� ��ȣ (���ڵ� ��ȣ�� ����)
	char ret;
} IND;
//����ü ũ�⸦ ��� �������� ����
#define sizeof_ind (sizeof(IND))

// ȸ���ε��� �����̴�.
typedef struct {
	char id[9]; 			// ���̵�
    int  cnum;           // ȸ�� �Ϸ� ��ȣ (���ڵ� ��ȣ�� ����)
} IND2;
#define sizeof_ind2 (sizeof(IND2))

// ��ȣȸ ȸ���� �����̴�.
typedef struct {
	int num;				// ȸ�� �Ϸ� ��ȣ (���ڵ� ��ȣ�� ����)
	char id[9]; 			// ���̵�
	char name[9];			// �̸�
	char memo1[65]; 		// �Ұ�1
	char memo2[65]; 		// �Ұ�2
	char memo3[65]; 		// �Ұ�3
	char memo4[65]; 		// �Ұ�4
	char memo5[65]; 		// �Ұ�5
	time_t inday;			// �������� (0x04)	 (5����Ʈ��)
	time_t login;			// ������ ������
	time_t logout;			// ������ ������
    int  countw;            // w,up,down ȸ��
    int  countu;            // w,up,down ȸ��
    int  countd;            // w,up,down ȸ��
	int  all_log;			// ����ȸ��
    int  all_time;          // ���̿�ð���� (��)
	char type;				// ��ȣȸ�� ȸ��Ÿ�� (0:�ź� 9:�û�)
	int  level;
	int  mymode;
    int  sp3      ;         // ���� �ð�
    int  sp4     ;          // ���� ��¥
    int  sp5     ;          // ���� ���Ӽ�
	char pay_mode;			// ���� ���
	char cash_mode; 		// ���̹� ĳ��
    int  cash;              // ���̹�ĳ��
    int  month_log;         //�̹���������ȸ��
    int  month_time;        //�̹������̿�ð�(��)
    int  sp1;
    int  sp2;
	byte point; 			// ������
	char sp6;
	char sp7;
	char warn;
} CPF;

CPF cpf;
//����ü ũ�⸦ ��� �������� ����
#define sizeof_cpf (sizeof(CPF))


#endif	/* XHOST_PFHEADER */
