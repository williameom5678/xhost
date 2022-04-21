//XHOST ����:�輺�� 1996.10.15 ���� ���߽�����
// ������/���н��� ��Ƽ��� ȣ��Ʈ
//moni.mode �� : 0.����   1"����"; 2"���"; 3"�ٿ�"; 4"��ȭ"; 5"����"; 6"��ȸ";
//				  7"����"; 8"�۾�"; 9"�ź�";11"�ڳ�";99"���";
// menudata   : �Խ��ǵ���Ÿ  �Խ���������,������ ��  �����ڵ�.tit,txt
// menurec	  : �Խ��Ǳ��	: �����Ȳ,���,ip��ϵ��� ��
// menucfg	  : �޴��� ������ ȯ���̵�	(*.cfg,*.men,*.ipg,*.cas,*.bil)
// menulogo   : �޴����� ���Ǵ� ȭ���� ��
// menuexec   : �޴����� ���Ǵ� �ܺθ��
// menustru   : �޴����� ����
// daterec	  : �Ϻ���� ����  daterec/19991010/.......

#ifndef XHOST_H
#define XHOST_H

//#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <malloc.h>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <termio.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
//#include <error.h>


#define VER1  "XHOST 5.22d"
#define VER2  "����:�輺��"
#include "header.h"
#include "makenum.h"

#define ishangul(c) ((unsigned char)(c)>=0xa1&&(unsigned char)(c)<=0xfe)
#define not(n) ((n)?0:1)

#define BBSTYPE "abdgcikvwloq+-fzyul1234567890"  //�Խ���/�ڷ��
#define BBBTYPE "abgiklvoq+-1234567890"          //�Խ���/�ڷ��
#define PDSTYPE "fcduywz"               //�ڷ��
#define PDSTYPE2 "fcduyw"               //�ڷ��
#define EXETYPE "exn"                   //exec �޴�

#include "ansi.h"
#include "base.h"

#define MENU_FILE		"menustru/menu.dat"
#define ID_FILE 		"id/id.dat"
#define GUEST_FILE		"id/guest.dat"
#define GUESTNOT_FILE	"id/guestnot.dat"
#define GUESTWAI_FILE	"id/guestwai.dat"
#define GUESTCAN_FILE	"id/guestcan.dat"
#define GUESTOUT_FILE	"id/guestout.dat"
#define GUESTDEL_FILE	"id/guestdel.dat"
#define GUESTDEP_FILE	"id/guestdep.dat"
#define BBS_CFG 		"system/bbscfg.dat"
#define MONI_FILE		"user/moni/moni.dat"
#define GOOD_FILE		"system/bbsgood.goo"
#define GOOL_FILE		"system/bbsgood.gol"

#define is_main()	  (is_same(club_go.code,"main"));
#define is_club()	  (!is_same(club_go.code,"main"));
#define is_bbspds	  (is_char(BBSTYPE,go.type))
#define is_pdsbbs	  (is_char(BBSTYPE,go.type))
#define is_typesub	  (go.type=='m')
#define is_typetel	  (go.type=='n')
#define is_typebbs	  (is_char(BBBTYPE,go.type))
#define is_typepds	  (is_char(PDSTYPE,go.type))
#define is_pds		  (is_char(PDSTYPE2,go.type))
#define is_typeexe	  (is_char(EXETYPE,go.type))

#define is_typeans	  (go.type=='q')
#define is_typevtx	  (go.type=='v')
#define is_typeansi   (go.type=='+')
#define is_typewtx	  (go.type=='w')
#define is_typeform    bit(bcfg.form,4)
#define is_typeview    bit(bcfg.form,5)
#define is_typeebs	   bit(bcfg.form,6)
#define is_typeank	   bit(bcfg.form,7)
#define is_typequiz    bit(bcfg.form,8)
#define is_typetalk    bit(bcfg.form,9)
#define is_typevote    bit(bcfg.form,10)
#define is_typesecu   (go.type=='o')
#define is_typecons   (go.type=='k')
#define is_typeday	   bit(bcfg.form,16)
#define is_typeseek    bit(bcfg.form,17)
#define is_typequest   bit(bcfg.form,18)
#define is_typewrite   bit(bcfg.form,19)
#define is_typeup	   bit(bcfg.form,20)
#define is_typeshop    bit(bcfg.form,21)


char   DM			 ; //������ �÷�
char   DM1			 ; //������ �÷�
char   DM2			 ; //�ӽ� �߱� �÷�  ON�̸� 500�� �̻� ����
char   DM3			 ; //������ȣ�� �ƴѰ�?
char   NOW_MENU [130] ; //����޴�ȭ�ϸ�
char   NOW_FILE [130] ; //������ �Խ���ȭ�ϸ�
char   NOW_FILED[130] ; //������ �Խ���ȭ�ϸ�  (����)
char   LT_FILE[130]   ; //
char   TMP_FILE [90] ; //
char   TMP_FILE1[90] ; //
char   TMP_FILE2[90] ; //
char   EBS_FILE[90]  ; //
char   BCFG_FILE[200]; // ȯ��ȭ��
char   DEF_UPS[81];    // UPS �⺻DIR
char   DEF_DNS[51];    // DNS �⺻DIR
byte   CONT_DAY;	   // ��������ϼ�
byte   who;
byte   newsmode[5];
int    SERVER_PORT[5];
char   subkey4[12];

int    NOW_REC		 ; // �����ڷ��ȣ
int    NOW_TOPREC	 ; // ����ȭ�� ��ܹ�ȣ
int    NOW_BOTREC	 ; // ����ȭ�� �ϴܹ�ȣ
int    NOW_ALLREC	 ; // �������ڷ��ȣ
int    NOW_NO		 ; // �����ڷ��ȣ
int    NOW_TOPNO	 ; // ����ȭ�� ��ܹ�ȣ
int    NOW_ALLNO	 ; // �������ڷ��ȣ
int    TODAY_TIM1	 ; // ��޺� ��ü ���ӽ� ���ð�
int    TODAY_TIM2	 ; // ��޺� �ѹ� ���ӽ� ���ð�
int    TODAY_DOWN	 ; // ��޺� ���� �ٿ�뷮
int    UP_LEVEL 	 ; // ���� ���� ����
int    UP_CASH		 ; // ���� ���� ĳ��

int    bbsnum_t 	 ; // ����Խ��� �ֻ�����ȣ
int    bbsnum_e 	 ; // ����Խ��� ����ȣ
int    bbsnum_a 	 ; // ����Խ��� ��ü��

char   p_command	 ; //�������� p�� ��� �Դ°�?
char   auto_cmd[70]  ; //�ڵ� ó�� ���
char   hostname[21]  ; //����ȣ��Ʈ��
char   REC_DIR[21]	 ; //��ϵ��丮	/user/rec
char   news_code[9]  ;

time_t START_TIME	 ; //���ӽ��� �ð�
time_t CLUB_START_TIME ; //���ӽ��� �ð�
time_t CHECK_TIME	 ; //���ӽ��� �ð� ��꺰 ���ð�
int    SEND_KB		 ; //����Byte

char re_print;		// ȭ�� ������ο� ���� �Ű���
char re_bbs  ;		// �Խ��� ��ε�����
char re_logo;		// �ΰ� �� ǥ�ÿ� ���� ����
char re_club;		// ��ȣȸ�� �ʱ�ȭ�ΰ�?
char list_mode; 	// ����Խ����� ������¸��
char bbs_ltmode;	// ����Խ����� ������¸��
char noti_mode; 	// ���������� �����ִ� ����ΰ�?
char guest_mode;	//�մ����Ӹ��
char dd_mode;		//������ �ڷ� �����ֱ� ���
char nostop;		//�ΰ� �����ٶ� ������ ����
char limit; 		//����,���,�ٿ�� ����
int cashmax1; //�̹����� �ִ� ����
int cashmax2;
char mymoni[21];	 //�ڱ��� ����
char c_proc,c_proc2,c_proc3;
char autoup;
byte news_count;		//��½ð�

int N_page;
int view_pos;
char view_num;
char g_char;
char buffnum[8];
char jpg_tag;
int  alast_read[30];
char ajob[19][11];			//��������
char bbsrec_club[9];
char bbsrec_code[9];
char bbsrec_jobmode;
char bbsrec_gotype;
time_t bbsrec_time;
int mymoni_rec;

int NOW_REC;				//������ �����ϴ� bbs�� ���ڵ� ��ȣ
int  cmdnum;				//������ ��ȣ
char mmc[10][21];			 //�Է��� ���
int  mmn[101];				//�����Է�->����
char dark_mode;
char sub_go[9]; 				//���������ڵ�
char pf_crt;
char aread_nums[50][30];		 //�ڷ� ���� ��ȣ
int RATE;						//ip�� �����
int allmode;					//��ü�ӽø��
int allmode2;					//��ü�ӽø��
int limit_mode; 				//��ü���Ѹ��
int netmode;					//��Ʈ��ũ���
int now_cmd_num;	 //������ ���� ��ȣ
char bbs_mode;					//bbs�� ���
char _cmdline1[31];
char _cmdline2[31];
char _last1[15];	  ;;;;;;;;;;;//Ȯ�ο�
int  _last2;		  ;;;;;;;;;;;//Ȯ�ο�

char g_yes; 		  //�ΰ��� ���������� �ڵ�Yes  :!�ڵ�!
char g_id[30];		  //�������� ���̵�
char g_name[30];	  //�������� �̸�
char g_buff1[301];	  //�������� ����
char g_buff2[301];	  //�������� ����
char g_buff3[301];	  //�������� ����
char g_buff4[301];	  //�������� ����
char g_buff5[301];	   //�������� ����
char g_buff6[30];	  //�������� ����
char g_buff7[30];	  //�������� ����
char g_buff8[30];	  //�������� ����
char g_buff9[30];	  //�������� ����
char g_buff10[30];	  //�������� ����
int  a_num[100];	   //����
char a_str[100][71];   //����
int  ascr_count;
int  list_anum[50]; 	  //����Խ��� ��ȣ LIST
int  bbsdownnum;
char bbsreadall;		  //������̻����

struct termio termold;			//�����ϱ��� ���
struct termio termnew;			//�߰���� (�������)
struct termio termall;			//�������ø��

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

//ȸ������ ����
#include "pf.h"
// �Խù� ���� ȭ�����
typedef struct {			   // �Խù� ����
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
	char passwd[5]; 		   // ��ȣ
	char filename[13];		   // ȭ�ϸ�
	int  filesize;			   // ȭ��ũ��
	char tcode[9];			   // �ӽð����ڵ�
} BBS;
BBS bbs;
#define sizeof_bbs (sizeof(BBS))

char now_type;				//������ ��� (��ȣȸ�϶� ��ȣȸ�� ���)
int addlevel;				//���� ��������
int adddown;				//���� �ٿ�����

char atype[20][9];			//ȸ����޺� �̸�
char actype[20][9]; 		//��ȣȸȸ����޺� �̸�


// ��ü�����ڵ� ǥ
typedef struct {
	char name[21];			 //�̸� �ڵ�
	char code[9];			 //���� �ڵ�
}GG;
#define sizeof_gg  (sizeof(GG))

//��񿡽� ȯ�漳��
typedef struct {
	//�ý��۰��� ����
	byte hostno;			//ȣ��Ʈ��ȣ
	char hostname[21];		//ȣ��Ʈ�̸�
	char sysop[9];			//�û��� ���̵�
	int  sp_4	;			//Ű�Է´��ð�
	int  mode;				//����
	int  backmode;			//�ý��۹��
	int  hackmode;			//���ð���
	char monichar;

	//ȭ�����
	int dispmode;
	int usermode;			//US���
	char usdisp;			//���� USǥ������

	//��������
	int  levelmode; 		//�������
	int  levelmax;
	char clublevel; 		//��ȣȸ�� �ִ� ��������

	//���ϰ���
	int  mailmode;			//���ϸ��
	char mailwrite; 		//���� ��� ���� ���
	char emailwrite;		//E���� ��� ���� ���
	char wmalltype; 		//��ü���ϵ��
	char mailbintype;		//��ü���ϵ��

	//��,�ٿ����
	int  downmode;			//�ٿ���� ���� bit()
	char downfree;			//������ �ٿ�� Ÿ��
	char downmult;
	char clubmaxup; 		//��ȣȸ�ִ�ٿ����(.1)
	char rz[81];
	char sz[81];
	char kermitrz[81];
	char kermitsz[81];
	byte drate; 		   //�ٿ�� �������
	byte urate; 		   //��Ͻ� ����

	//�Խ��ǰ���
	int  bbsmode;			//�Խ��Ǹ��
	byte helptype;			//��� ������ �����ִ� ���
	byte helplevel; 		//��� ������ �����ִ� ����
	byte helplog;		   //��� ������ �����ִ� ���Ӽ�
	int  newsmode;			//��������
	byte notice;
	byte noticet;
	byte noticex;
	byte noticey;
	byte noticeline;
	char gdstr[11]; 		//��õ����
	char jpgchar[3];		//ȭ����

	//�մ�/ȸ������
	int  guestin;			//�մ԰���
	int  guestmode; 		//�մԸ��
	int  guestitem; 		//ID�Է¸��   (�Է��׸����)
	int  pfmode;			//��¸��
	int  loginmode; 		//���̵��Է¸��
	char guestid[9];		//�մ� ���� ���̵�
	byte chatst;			//��ȭ����۸��
	byte guesttype; 		//�ʱ� ���
	int  guestlevel;		//�ʱ� ����
	int  guesttime; 		//�ʱ� �ð� (��)
	int  guestdown; 		//�ʱ� �ٿ�뷮
	int  guestcash; 		//�ʱ� �ݾ�
	byte ed_pfmode; 		//�Ż�����ȯ��
	byte freelogin; 		//���Ϻ�,�ð��뺰 ���õ��
	byte relog; 			//�����ӵ��
	byte limittype; 		//���ѵ��

	//��ȭ�Ǽ���
	int  chatmode;			//��ȭ�Ǹ��
	int  messmode;			//�޼������
	char chatwarn;			//ü�ð��ð�(��)
	byte chattype;			//��ȭ ���� ���
	byte sayline;			//say�ִ�����

	//���̹�Ȩ
	int  homemode;
	byte hometype;			//Ȩ�������
	byte homepds;			//�ڷ�Ǻ������

	//���̹�ĳ��,IP,����
	int  cashmode;			//��ücashmode
	int  ipmode;
	int  billmode;
	int  cashmax;			//�ִ뺸��CASH
	int  cashmaxday;		//1�� �ִ� ����
	char cashfree;			//������ �ݾ׻�� ȸ�����
	char billfree;			//������ �ݾ׻�� ȸ�����
	int cashmax1; //�̹����� �ִ� ����
	int cashmax2;

	//��Ϲ�ȣ
	int  procnum;			//�۾��߱޹�ȣ
	time_t procdate;		//�۾��߱޹�ȣ

	//���������
	int  infomode;
	int  mprocmode; 		//�л�ó�����

	//ȭ����ø��
	int  jpgmode;

	byte telnettimeout; 	//�ڳ�Ȯ�νð�
	int  startmode; 		//���԰��ü���
	char jpgstr[81];		//ȭ�����۹��ڿ�
	char jpgsz[81]; 		//ȭ��������������
	char utmpfile[31];		//utmp����
	char no_logotype;		//setlogo �����
	int  logomode;
	byte waitkey;			//���ð�(��)
	char comp[5];			//��ȣ
	int  mrecmode;
	int  price1;			//�д�ݾ�
	int  price2;			//KB��ݾ� (x.x);
	char allsay1;			//��ü�޼���
	char allsay2;			//��ü�޼���
	char allsay3;			//��ü�޼���
	char telnettype;		//�ڳ����ӵ��
	char sp_____[9];		//�ӽ�
	int  guestet;			 //�մ԰��Խ� �������� �׸�
	char txtmode;			//�ڷắȯ���
	word guestp_ph; 			 //ü��
	word guestp_it; 			 //����
	word guestp_at; 			 //�ŷ�
	word guestp_pr; 			 //����
	int  guestp_mo; 			 //���
	byte sp_1;
	byte sp_11;
	byte up_limit;			//��� �� ����
	byte dn_limit;			//�ٿ� �� ����
	byte dn_mult_limit; 		  //��Ƽ �ٿ� �� ����
	char newsfile[9];		//��������
	char newstype;			//��������
	byte newsoldtime;		//��������
	int  sp_2;
	byte limit_r;			//�ּҰ��ɵ��
	byte limit_w;			//
	byte limit_u;			//
	byte limit_d;			//
	int  vote;				//��ǥ����
	byte vote_type; 		//��ǥ����
	byte vote_opentype; 	//��ǥ����
	int  limit_slog;		//���Ӽ� ��½� �ִ밪
	byte limit_m;			//�����޴� ���԰��ɵ��
	char check_wgo[5][9];	// ����˻�
	int  check_today;		// ���ø� �˻�?
	int  check_todayon; 	// ��¿��ΰ˻�?
	byte alarm_time;
	int  level_size;
	char usdisp2;			//��ȭ��
	int  zmode; 			//��ü�ڷ�Ǹ��
	int  gomode;
	int  limit_login;		//�������ڼ�
	char sip[15];
	char nip[21];
	byte count_server;		//���Ӽ�����
	char lock_ip_addr[20];
} CFG;
CFG cfg;		//���� ȯ��ȭ��
#define sizeof_cfg (sizeof(CFG))


//�����ڵ� ���� ����
typedef struct {
	int mode;
	int fr_type;
	int fr_down;
	int fr_level;
	int fr_cash;
	int fr_time;
} FCFG;
#define sizeof_fcfg (sizeof(FCFG))

//��ȣȸ ȯ�漳��
typedef struct {
	char sysop	[9];			//�û��� ���̵�
	char passsys[9];			//�û�޴��� ��ȣ
	char enterp [5];			//���Խ� ��ȣ ����
	char notice    ;			//���۽� ������¾���
	byte noticet;
    byte sp_4      ;            //
	char sp_3	   ;			//
	int  dir	   ;			//�ڷ�� ���丮 ��������
	int  autoguest ;			//�ڵ�����	0:���� 1:�ڵ� 2:���Խ�û����
	int  intype    ;			//��ȣȸ �����ּҵ��;
	int  level	   ;			//��ȣȸ �����ּҷ���;
	int  stop	   ;			//��ȣȸ ������
	int  age	   ;			//�������� 012034 (����-��)
	char note[5][9];			//����� ����
	char gtype; 				//��û���� �ּ� ���
	byte gage1; 				//��û���� �ּ� ����
	byte gage2; 				//��û���� �ִ� ����
	int  gcash; 				//��û���� �ּ� ���̹�ĳ��
	char modeopen  ;			//��������
	char wmalltype; 			//��ü���� ��� ���
	char memo_disp ;			//������� ���
	char cintype   ;			//��ȣȸ �����ּ� ��ȣȸ���;
	int  chatmode;				// ��ȭ����
	int  mode;					//��ȣȸ���
	int  maxmenu;				//�ִ�޴���
	int  mode2; 				 //��ȣȸ���
	int  mode3; 				 //��ȣȸ���
	int  mode4; 				 //��ȣȸ���
	int  mode5; 				 //��ȣȸ���
	char memo_input;			 //�޸����Է�
	char sp2;					 //��ȣȸ���
	char noticex;
	char noticey;
	char noticeline;
	char bankman[9];
	char pds_man[9];		//��ü�ڷ�Ǵ��
	char bbs_man[9];		//��ü�Խ��Ǵ��
    int  chatnum;           //��ȣȸ��ȭ�� ��ȣ
    int  sp_72 ;            //����� KB��
	int  logomode;
	int  maxpdsmenu;		//�ִ��ڷ�Ǽ�
	int  maxpdssize;		//�ִ��ڷ�ǿ뷮
} CCFG;
CCFG ccfg;		 //��ȣȸ ȯ��ȭ��
#define sizeof_ccfg (sizeof(CCFG))


// �������ü���
// ���������ȰԽ��ǿ� ���� �ش�Ǵ� ��ġ
// menucfg/�ڵ�.war
typedef struct {
	int  mode	   ;
	int  time1	   ;			//���Խ� ����(�д�)
	int  time2	   ;			//���Խ� ����(�Ǵ�)
	int  read1	   ;			//������ ����(�Ǵ�)
	int  read2	   ;			//������ ����(1K��)
	int  down1	   ;			//�ٿ�� ����(�Ǵ�)
	int  down2	   ;			//�ٿ�� ����(1K��)
	int  writ1	   ;			//����	 ����(�Ǵ�)
	int  writ2	   ;			//����	 ����(1K��)
	int  uplo1	   ;			//��Ͻ� ����(�Ǵ�)
	int  uplo2	   ;			//��Ͻ� ����(1K��)
	int  basetype  ;			//����Ÿ��
	int  basemode  ;			//����
	int  skil	   ;			//���
	int  prot	   ;			//���
	int  arm	   ;			//����
	int  move	   ;			//�̵�
	int  dest	   ;			//�ı�
	int  rese	   ;			//�ڿ�
	int  numb	   ;			//�ο�
	int  wait	   ;			//����ݽð�
}WARS;
WARS wars;		 //��ȣȸ ȯ��ȭ��
#define sizeof_wars (sizeof(WARS))


// ���̹��Ӵϼ���
// �ش�Խù��� ������ �ش���̵𿡰� �����Ѵ�.
// bbsdata/�ڵ�/cash
typedef struct {
	int  mode	   ;
	int  time1	   ;			//���Խ� ����(�д�)
	int  time2	   ;			//���Խ� ����(�Ǵ�)
	int  read1	   ;			//������ ����(�Ǵ�)
	int  read2	   ;			//������ ����(1K��)
	int  down1	   ;			//�ٿ�� ����(�Ǵ�)
	int  down2	   ;			//�ٿ�� ����(1K��)
	int  writ1	   ;			//����	 ����(�Ǵ�)
	int  writ2	   ;			//����	 ����(1K��)
	int  uplo1	   ;			//��Ͻ� ����(�Ǵ�)
	int  uplo2	   ;			//��Ͻ� ����(1K��)
} CASH;
CASH cash;		 //��ȣȸ ȯ��ȭ��
#define sizeof_cash (sizeof(CASH))

// ���ݼ���
// bbsdata/�ڵ�/bill
typedef struct {
	int  mode	   ;			//���ݸ��
	int  time1	   ;			//���Խ� ����(�д�)
	int  time2	   ;			//���Խ� ����(�Ǵ�)
	int  read1	   ;			//������ ����(�Ǵ�)
	int  read2	   ;			//������ ����(1K��)
	int  down1	   ;			//�ٿ�� ����(�Ǵ�)
	int  down2	   ;			//�ٿ�� ����(1K��)
	int  writ1	   ;			//����	 ����(�Ǵ�)
	int  writ2	   ;			//����	 ����(1K��)
	int  uplo1	   ;			//��Ͻ� ����(�Ǵ�)
	int  uplo2	   ;			//��Ͻ� ����(1K��)
} BILL;
BILL bill;		 //��ȣȸ ȯ��ȭ��
#define sizeof_bill (sizeof(BILL))


// IP����
// �Խ��Ǻ������̸� �ٸ������ �ٿ�/������
// �ش�ID���� ������ �Ѵ�.
// bbsdata/�ڵ�/ipgo
typedef struct {
	int  mode	   ;			//IP���
	int  time1	   ;			//���Խ� ����(�д�)
	int  time2	   ;			//���Խ� ����(�Ǵ�)
	int  read1	   ;			//������ ����(�Ǵ�)
	int  read2	   ;			//������ ����(1K��)
	int  down1	   ;			//�ٿ�� ����(�Ǵ�)
	int  down2	   ;			//�ٿ�� ����(1K��)
	int  writ1	   ;			//����	 ����(�Ǵ�)
	int  writ2	   ;			//����	 ����(1K��)
	int  uplo1	   ;			//��Ͻ� ����(�Ǵ�)
	int  uplo2	   ;			//��Ͻ� ����(1K��)
} IPGO;
IPGO ipgo;
#define sizeof_ipgo (sizeof(IPGO))


typedef struct {			   //�Խù� ����
	time_t date;			   //���������
	char lastid[9]; 		   //���������
	int  login; 			   //���Ӽ�
	int  login_type[20];	   //��޺����Ӽ�
	int  login_port[20];	   //��Ʈ�����Ӽ�
	int  utime; 			   //���ð�
	int  utime_type[20];	   //��޺����ð�
	int  utime_port[20];	   //��Ʈ�����ð�
	int  read;				   //������
	int  readsize;			   //�б��
	int  write; 			   //����
	int  writesize; 		   //�����
	int  up;				   //���
	int  upsize;			   //���ũ��
	int  down;				   //�ٿ�
	int  downsize;			   //�Ѵٿ�ũ��
	int  del;				   //����
	int  cash;
	int  sp3;
	int  sp4;
	int  sp5;
	int  sp6;
	int  sp7;
	int  sp8;
	int  sp9;
	int  sp10;
} LOGREC;
#define sizeof_logrec (sizeof(LOGREC))


//�����ڵ�
typedef struct {
	int  rec;				//�޴�REC
	char deep;				//�޴��� ���� 0:top
	char type;				//�޴�����
	char num[5] ;			//���ù�ȣ
	char code[9];			//�����ڵ�
	char name[41];			//�޴���
	char wtimes[5]; 		//��������
	char dir[61];			//�޴���
}GO;
GO go;
#define sizeof_go			(sizeof(GO))

GO club_go; 				 //���� ��ȣȸ ������
char last_club[9];			 //������������ȣȸ
GO last_go; 				 //�����������ڵ�
char club_mode; 			 // ��ȣȸ ����϶� Yes


//�Խ��� ȯ�漳��
//�Խ��Ǻ� ��� bbsdata/�ڵ�/bcfg
typedef struct {
	int  mode	   ;			//�⺻���
	char man1	[9];			//�Ѵ��
	char man2	[9];			//�δ��
	char man3	[9];			//�������
	char man4	[9];			//IP���
	char man5	[9];			//BANK���
	char key[31];				//�޴��˻�Ű����	 â��,imf,�ξ�,����,�ϴ�,����
	char bdntype   ;			//BDN����Ÿ��
	byte wdown	   ;			//�Ϲݾ�����(.1)
	byte wdown2    ;			//�ɾ߾�����(.1)
	byte rdown	   ;			//�Ϲݴٿ��پ��(.1)
	byte rdown2    ;			//�ɾߴٿ��پ��(.1)
	byte level	   ;			//����� �������
	char passsys[5];			//�û�޴��� ��ȣ
	char passin [5];			//���Ծ�ȣ
	int  itime; 				//���ɽð�
	int  dtime; 				//���ɽð�
	int  rtime; 				//���ɽð�
	int  wtime; 				//���ɽð�
	int  outday;				//������¥������
	int  runday;				//����������
	char itype; 				//���԰���
	char rtype; 				//�бⰡ��
	char dtype; 				//�ٿ��
	char wtype; 				//���Ⱑ��
	char fdown; 				//�����ٿ���
	char fread; 				//�����б���
	char outbbs[9]; 			//�ٸ��Խ������� ����
	//������
	int  form	   ;			//�޴����
	int  modelist  ;			//�������
	char headnum   ;			//�⺻���øӸ���
	char jpg_x	   ;			//ȭ��� �Խù���ġ
	char jpg_y	   ;			//ȭ��� �Խù���ġ
	char list_x    ;			//�ϹݰԽù���ġ
	char list_y    ;			//�ϹݰԽù���ġ
	char disp_x    ;			//�ϹݰԽù���ġ
	char disp_y    ;			//�ϹݰԽù���ġ
	char listline  ;			//��ȭ���ִ�ǥ����
	int  limitsize ;			//�Խù�����ũ������
	//�ٿ����
	int  downmode  ;
	char uhour1    ;
	char uhour2    ;
	char dhour1    ;
	char dhour2    ;
	char urate	   ;			//��Ͻð���
	char drate	   ;			//�ٿ�ð���
	char uratet    ;			//��Ͻð���  �ð�����
	char dratet    ;			//�ٿ�ð���  �ð�����
	//���̹�����
	char cashtype  ;			//���̹��Ӵϰ���
	char wartype   ;			//����Ÿ��
	char billtype  ;			//���ݰ���
	char iptype    ;			//IP����
	int  bill1	   ;			//�д���
	int  bill2	   ;			//KB����
	int  wmode	   ;			//������ø��
	char comp[5];				//IP��ȣ
	int  modeok    ;			//�����ݴ�
	int  qmode	   ;			//�������ñ��
	char maxline   ;
	char pos_y1    ;			//�б�� �����ġ
	char pos_y2    ;			//�б�� �ϴ���ġ
	int  price1;				//����� �д�
	int  price2;				//����� KB��
	int  automode;				//�ڵ� ó�� ���
	byte level2    ;			//�б�� �������
	byte level3    ;			//�ٿ�� �������
	int  uplevel;				//��ǰ�� ��������
	int  upcash ;				//��ǰ�� ��������
	byte updown ;				//��ǰ�� ��������
	int  jpgmode;				//ȭ����� ����
	byte li_w;					//���� ���� ?ȸ �̻� ����
	byte cmd_x; 				//����� ��ġ
	byte cmd_y;
	int  recmode;				//��ϰ��� ����
	char inbbs[9];				//�Է°Խ���
	char inbbskey[11];			//Ű��������
	byte up_limit;				//��� �� ����
	byte dn_limit;				//�ٿ� �� ����
	byte dn_mult_limit; 		//��Ƽ �ٿ� �� ����
	byte bform; 				//�Խ��Ǹ��
	char write_id[9];			//����� ����ID
	word level_size;			//1�������� ����ũ��
	int  imode; 				//���԰��ü���
	int  ilevel;
	int  icash;
	int  idlevel;				//���Խ� �߰�
	int  idcash;				//���Խ� ����
} BCFG;
BCFG bcfg;		 //��ȣȸ ȯ��ȭ��
#define sizeof_bcfg (sizeof(BCFG))


//�Խ��Ǻ����
//�Խ����� �аų� �ٿ������ �ӽú����� �����ѵ�
//�Խ��ǿ��� ����������(p,go,x..) ����Ѵ�.
//�Խ��Ǻ� ��� bbsrec/�ڵ�.bre
typedef struct {			//�Խù� ����
	time_t numdate; 		//�Խ�������������¥
	int  newnum;			//���û��Խù�
	int  allnum;			//��ü�Խù���
	char sp[20];
	time_t numdate1;		//�Խ�������������¥
	int  time1; 			//��ü ���ð�(��)    ��ü
	int  timebill1; 		//�ð��� ��ݴ���
	int  size1; 			//��ü ���SIZE(KB)
	int  sizebill1; 		//SIZE�� ��ݴ���
	time_t numdate2;		//�Խ�������������¥
	int  time2; 			//��ü ���ð�(��)    �ݿ�
	int  timebill2; 		//�ð��� ��ݴ���
	int  size2; 			//��ü ���SIZE(KB)
	int  sizebill2; 		//SIZE�� ��ݴ���
	time_t numdate3;		//�Խ�������������¥
	int  time3; 			//��ü ���ð�(��)    ����
	int  timebill3; 		//�ð��� ��ݴ���
	int  size3; 			//��ü ���SIZE(KB)
	int  sizebill3; 		//SIZE�� ��ݴ���
	time_t numdate4;		//�Խ�������������¥
	int  time4; 			//��ü ���ð�(��)    2����
	int  timebill4; 		//�ð��� ��ݴ���
	int  size4; 			//��ü ���SIZE(KB)
	int  sizebill4; 		//SIZE�� ��ݴ���
} BREC;
BREC brec;				//���ӱ��
#define sizeof_brec (sizeof(BREC))


//��ü��������ڷ�/�Ϻ���������ڷ�
//��¥�� ����� �ϸ� ���ڷ�� status/stotal�� �����Ѵ�.
//�Ϻ������ �Ѵ� daterec/stotal.rec ?	(��ü���)
typedef struct {
	int  count; 				//ȸ����
	int  guest; 				//ȸ����
	int  login; 				//���Ӽ�
	int  slogin;				//ȸ����
	int  time;					//���ð�(��)
	int  read;					//������
	int  write; 				//������
	int  up  ;					//������
	int  down;					//�ٿ��
	int  size;					//�ٿ
	int  chat;					//��ȭ��
	int  game;					//���Ӽ�
	int  ip  ;					//IP
} STOTAL;
STOTAL stotal;
#define sizeof_stotal (sizeof(STOTAL))


//���κ� ��ٸ� ���� ȭ�����
//ID�� ����� �Ѵ� userdata/ù����/���̵�/wait
typedef struct {			// �Խù� ����
	char id1[9];			// ����� ���̵�
	char id2[9];			// ��û�� ���̵�
	time_t date;
	char mode;				// �ۼ��� ���
	char title[60]; 		// ��� �޼���
} WAITFOR;


//IP�����Ȳ  menurec/�����ڵ�.ipr
typedef struct {			//�Խù� ����
	time_t date;			//�����ð�
	char mode;				//���
	char id[9]; 			//����� ���̵�
	char id2[9];			//����� ���̵�
	char title[21]; 		//�Խù�����
	char filename[13];		//ȭ�ϸ�
	char club[9];			//��ȣȸ
	char code[9];			//�����ڵ�
	int  bill1; 			//�����
	int  bill2; 			//���޿��
} IPREC;
IPREC iprec;
#define sizeof_iprec (sizeof(IPREC))

char OLD_MONI_MODE;

//���� ��Ȳ ����� �ڷ��̴�
//���ȭ�� user/moni/moni.dat
typedef struct {
	char id[9];
	char name[9];
	char nick[9];
	char type;
	char mode;		 //�۾����۸��  0:���� 1:���� 2:�޼����ź�
	char stat;		 //�۾����۸��  W:�Խ������� D:�ٿ��� U:���� C:��ȭ E:�ܺ�
	char club[9];
	char tty[9];
	char gocode[9];
	char goname[21];
	char what[71];			//���� �۾��ΰ�?
	char hostname[30];		//ȣ��Ʈ��
	time_t login;			//���ӽ��۽ð�
	time_t logout;			//���ӽ��۽ð�
	char msg ;				//�޼��� ����
	char mode2; 			//�޼��� ����
	char hostno;
	int  pfnum; 			 //ȸ����ȣ
	int  pid;
} MONI;
MONI moni;
#define sizeof_moni (sizeof(MONI))

struct utimebuf{
	time_t actime;
	time_t modtime;
};


//���ӱ������
typedef struct {
	int   read;
	int   readsize;
	int   write;
	int   writesize;
	int   up;
	int   upsize;
	int   down;
	int   downsize;
	int   del;
	int   cash;
} UALL;
UALL uall;


//��õ��������
typedef struct {
	char mark;
	int  count;
} AGOOD;
#define sizeof_agood (sizeof(AGOOD))
AGOOD agood[4][10];
//��õ��������
typedef struct {
	char mark;
	byte color_char;
	byte color_back;
	char name[9];
} ACOLOR;
#define sizeof_acolor (sizeof(ACOLOR))
ACOLOR acolor[10];
char a_is_file[10][51];

//�������
typedef struct {
	char cmd[9];	   // ���
	char cm2[9];	   //
	char pass[9];	   // ��ȣ
	char type;		   //���ɵ��
} SYSCMD;

#define now_year()		  now_now(1,0,0)
#define now_year2() 	  (now_now(1,0,0)+1900)
#define now_month() 	  now_now(2,0,0)
#define now_day()		  now_now(3,0,0)
#define now_hour()		  now_now(4,0,0)
#define now_min()		  now_now(5,0,0)
#define now_wday()		  now_now(6,0,0)
#define now_sec()		  now_now(15,0,0)
#define now_dnum()		  now_now(7,0,0)
#define now_mday()		  now_now(8,0,0)
#define now_cday()		  now_now(9,0,0)
#define now_cyear() 	  now_now(10,0,0)
#define date_year(date2)  now_now(1,1,date2)
#define date_month(date2) now_now(2,1,date2)
#define date_day(date2)   now_now(3,1,date2)
#define date_hour(date2)  now_now(4,1,date2)
#define date_min(date2)   now_now(5,1,date2)
#define date_wday(date2)  now_now(6,1,date2)
#define date_sec(date2)   now_now(15,1,date2)
#define date_dnum(date2)  now_now(7,1,date2);
#define date_mday(date2)  now_now(8,1,date2);
#define date_cday(date2)  now_now(9,1,date2);
#define date_cyear(date2) now_now(10,1,date2);

static char *awork[30]= { "    ","�Ϲ�","��ȣ","��ȭ","����",
						  "����","���","�б�","�ٿ�","����",
						  "�д�","�Ǵ�","��IP","�ӵ�","ȭ��",
						  "�ܺ�","�ڳ�","���","����","�۾�",
						  "��IP","��IP","�Խ�","����","��Ÿ",
						  "���","����","�Խ�","����","��Ÿ"
						  };
#endif	/* XHOST_H */
