// �����ڵ� ǥ
typedef struct {
	char deep;				//�޴��� ���� 0:top
	char type;				//�޴� ����
	char num[5];			//���� ��ȣ
	char code[9];			//���� �ڵ�
	char name[31];			//�޴� ����
	char temp[16];			//������
	char wtimes[4]; 		//���� ���� ȸ������
	int  level; 			//���� ������
	char ma_[9];			//�����
	char rtype; 			//������ �ִ� ���
	char wtype; 			//������ �ִ� ���
	char dtype; 			//�ٿ� ������ �ִ� ���
	char dir[61];			//ȭ����ġ ���丮
	int  passwd;			//������
	char itype; 			//���԰���
}OLDGO;
OLDGO oldgo;

#define sizeof_oldgo  (sizeof(OLDGO))

typedef struct {
    char man1   [9];            //�����1
    char man2   [9];            //�����1
	char man3	[9];			//�����1
	char man4	[9];			//�����1
	char man5	[9];			//�����1
	char pass	[9];			//�û�޴��� ��ȣ
	int  passwd;				//���Ծ�ȣ
	int  intime;				//���� �ð�
	int  mode	   ;			//ȭ����¸��
	char headnum   ;		   //�⺻���øӸ���
	char sp1   [ 8];		   //��
	char sp2   [ 9];		   //��
	char sp3   [14];		   //��
	int  listmode  ;
	char itemlen[50];		   //ĭ��������
	int  bbssize   ;		   //�Խù�ũ�� ����
	int  cashtime1 ;
	int  rcash1    ;
	int  rcash2    ;
	int  rcash3    ;
	int  rsize1    ;
	int  freeday   ;
	char freetype1 ;
	int  cashtime2 ;
	int  wcash1    ;
	int  wcash2    ;
	int  wcash3    ;
	int  wsize1    ;
	char freetype2 ;
	char rrate1    ;
	char rrate2    ;
	char rrate3    ;
	char rrate4    ;
	char rrate5    ;
	char cashmain  ;
	char cashmode1 ;
	char cashmode2 ;
	int  cashmode  ;
	char listposi  ;
	char listline  ;
	char rcash4    ;
	char wcash4    ;
	int  wcashmax  ; //�ִ밪 ����
	int  rcashmax  ; //�ִ밪 �ٿ�
	int  wcashmode ;
	int  rcashmode ;
	char pos_x	   ; //ȭ����ġ
	char pos_y	   ; //ȭ����ġ
    int  outday    ; //������¥ ������
    byte wdown     ; //������ ����
    byte rdown     ; //�ٿ��پ�� ����
    byte downfree  ; //�����ٿ�/�б���
    byte freeup    ; //�������/������
} BBSCFG;
BBSCFG bbscfg;		 //��ȣȸ ȯ������
#define sizeof_bbscfg (sizeof(BBSCFG))
