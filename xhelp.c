////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// �ʺ��ڿ� ����
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"

main(argc, argv)
int argc;
char *argv[];
{
	char buff[200];
    char file[200];
	int n;

	findport(tty); //������Ʈ ã��
	read_cfg();
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�

    strcpy(file,argv[1]);
    read_pf(argv[2]);

    while(1) {
		cls();
        view("frm/help/%s.scr",file);
        lineinput(buff,2);check_han(buff);printf("\n");
		n=atoi(buff);
        if (is_char("qQpQ0",buff[0])) break;
        if (n>0&&n<999) {
            cls();
            view("frm/help/%s%d.hlp",file,n);
        }
	}
}


