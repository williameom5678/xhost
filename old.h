typedef struct {               // �Խù� ����
	int num;				   // �Ϸ� ��ȣ
	char id[9]; 			   // �ۼ��� ���̵�
	char name[9];			   // �ۼ��� �̸�
	time_t date;			   // �ۼ� ����
	time_t date2;			   // �ۼ� ����
	int  line;				   // ���� ����(�ټ�)
	int  read;				   // ��ȸ��
	int  down;				   // �ٿ�ȸ��
	word ok;				   // ����
	char title[61]; 		   // ����
	int  posi;				   // ���� ������
	int  size;				   // ���� ũ��
	char key[9];			   // �˻���
	char gd;				   // ��õ�ڷ�	   0:���� 1:��� 2:�ֿ��
	char del;				   // ��������	   0:���� 1:����
    char passwd[5];            // ��ȣ
	char filename[13];		   // ȭ�ϸ�
	int  filesize;			   // ȭ��ũ��
	char tcode[9];			   // �ӽð����ڵ�
} BBS3;
BBS3 bbs3;
#define sizeof_bbs3 (sizeof(BBS3))
