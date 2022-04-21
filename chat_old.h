
// XHOST ����:�輺�� 1996.10.15 ���� ���߽�����
// ������/���н��� ��Ƽ��� ȣ��Ʈ
#ifndef XHOST_CHAT
#define XHOST_CHAT

					// Client�� ����� Port

//��ȭ�� ��ü �ο�
typedef struct {
	char id    [13];	//�������� ���̵�
	char name  [13];	//�������� ���̵�
	char nick  [13];	//����
	char tty   [10];
	char image	   ;	//���α׸���ȣ
	char imageact  ;	//���ε��۱׸���ȣ
	char hostname[21];
	char note  [31];	//�ڱ⼳��
	int  point	   ;	//�ڱ�����
	int  score	   ;
	byte check	   ;
	byte sex	   ;
	byte age	   ;
	byte area	   ;
	byte hostno    ;
	byte mode	   ;	//���κ� ���
	byte color1    ;	//Į������
	byte color2    ;	//Į������
	char age2[10]  ;	//~����~���� ���� ���ڿ�
	byte sp_2;
	byte sp_3;
	byte sp_4;
	byte sp_5;
	int  pid;			//PID
	int  sp_7;
	int  sp_8;
	int  sp_9;
	time_t date    ;	//���Խð�
	time_t ctime   ;	//�������Է½ð�
	time_t rtime   ;	//�����Է½ð�
	time_t gtime   ;	//���ӽ��۽ð�
} ROOM;
ROOM room;
#define sizeof_room (sizeof(ROOM))

//��ȭ�� ��ü �ο�
typedef struct {
	int  roomnum;
	byte roomtype  ;	//������
	byte count	   ;	//�����ο�
	byte max	   ;	//�ִ��ο���
	char id[13]    ;	//�������� ���̵�
	char name[13]  ;	//�������� ���̵�
	char hostname[21];	//������ ȣ��Ʈ��
	char boss2[13] ;	//��������
	byte dm 	   ;	//��¹��
	byte openmode  ;	//�溰���
	byte wmode	   ;	//�溰��� (���ͳݸ��)
	byte dispmode  ;	//��¹��
	time_t date    ;	//�����ð�
	char title[151];	//����
	char stitle[61];	//��������
	char ctitle[21];	//���Ӹ�
	char passwd[5] ;	//���Խ� ��ȣ ����
	char image	   ;	//���׸���ȣ
	byte itype	   ;	//���Ե��
	byte age1	   ;	//���Գ���
	byte age2	   ;	//���Գ���
	char sex	   ;	//���Լ���
	byte rcount    ;	//������½ð�ī����
	char club[13]  ;	//��ȣȸ
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
	int  troom[10] ;	//�ٸ������� FORWORDING
	ROOM room[20]  ;	//������
} CROOM;
CROOM croom;
#define sizeof_croom (sizeof(CROOM))


//��ȭ�� ��ü �ο�
typedef struct {
	int  roomnum   ;
	byte type1	   ;	//������
	byte type2	   ;	//������
	byte type3	   ;	//������
	char id[13]    ;	//�������� ���̵�
	char title[151];	//����
	char passwd[13];	//��ȣ
	char email[21] ;	//���Email
	byte html_l    ;	//����Html
	byte html_t    ;	//���Html
	byte html_r    ;	//return Html (��QUIT)
	byte openmode  ;	//�溰���
	int  mode	   ;	//�溰���
	int  point	   ;	//����
	int  badpoint  ;	//����
	time_t date    ;	//�����ð�
	time_t ltime   ;	//������ ���ӽð�
	int  count	   ;	//�����Ӽ�
	int  count1    ;	//�����Ӽ� �̹���
	int  count2    ;	//�����Ӽ� ��������
	int  uset	   ;	//�ѻ���
	int  uset1	   ;	//�ѻ��� �̹���
	int  uset2	   ;	//�ѻ��� ��������
} UROOM;
#define sizeof_uroom (sizeof(UROOM))


//��ȭ�Ǳ��
typedef struct {			//�Խù� ����
	time_t date1;			//���ӽð�
	time_t date2;			//�����ð�
	char id[13];			//���̵�
	char name[13];			//�̸�
	char nick[13];			//��ȭ��
	char tty[10] ;			//��ȭ��
	byte sex	 ;			//����
	byte age	 ;			//����
	byte area	 ;			//����
	char hostname[21];		//ȣ��Ʈ��
} CUS;


//��ȭ�Ǳ��
typedef struct {			//�Խù� ����
    int  today;             //���� ����
	int  count; 			//�����ӱ��
	int  ttime; 			//�����ӽð�
	time_t date1;			//������¥
	time_t date2;			//������볯¥
	int  acount[(12-1)*31*25+31*25+23+3];  //��,��,�ð�
	int  atime [12*32	];	//��,��
	int  sp_1[100]; 		//����
	char sp_2[200]; 		//����
	CUS  cus[50];			//50�� ���������� ���
} CREC;
#define sizeof_crec (sizeof(CREC))


char ROOM_FILE[200];
int  roomtype,roomnum;
char my_mode;
char my_color1;
char my_color2;
time_t room_sec;
static char *acstr[20]= { "��","Ǫ","��","��","��",
						  "��","Ȳ","��","ȸ","��",
						  "��","û","��","��","��",
						  "��","��","��","��","��"};

int quit_chat,bye_chat,cont_chat ; /* ��ȭ�ǿ��� /x�� �ٷ� ������. */
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
char chaty1,chaty2;    // ��ȭ�� Y��ġ
#define ALARM_TIME 60
#define MAX_OUT (30*60)

char userid[21];
char userna[21];
char userni[21];
char tosay[21];
char userage[10];
char userarea;

#endif	/* XHOST_H */
