////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// �Խ��� ������� �޴�
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"
main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
    PF oldpf;
    char buff[100];
    char filename[100];

    if (argc<2) {
        printf("\n����  get_mypf <ȭ�ϸ�> [tty]\n");
        printf("\n��°��-> ȭ�Ͽ� ���̵� �̸� ���� ��� ���� �ٿ� �Ӵ� �ð� ���� ������ ���\n");
        getchar();
        return;
    }
    strcpy(filename,argv[1]);              //�����ڵ�
    unlink(filename);
    if (argc==3) strcpy(tty,argv[2]);
    else         findport(tty); //������Ʈ ã��
    if (!findttypf(pf.id,pf.name,tty)) {     //���� �� �޴��� ����ϴ����� �ľ���
        mess("%s ��Ʈ ����ڸ� ã���� �����ϴ�.",tty);
		return;
	}
    oldpf=pf;
    if (!read_pf(pf.id)) {
        strcpy(pf.id,"........");
        strcpy(pf.name,"........");
        strcpy(pf.nick,"........");
        pf.type=0;
        pf.level=0;
        pf.cash=0;
        pf.down=0;
        pf.left_time=0;
        mess("%s ȸ���� ã���� �����ϴ�.",oldpf.id);
    }
    if ((fp=fopen(filename,"w"))!=NULL) {
        fprintf(fp,"%s %s %s %d %d %d %d %d %s 0 0 0 0\n",pf.id,pf.name,pf.nick,pf.type,pf.level,pf.down,pf.cash,pf.left_time,dateyang(11,pf.birthday,pf.moon));
        fclose(fp);
    }
}

findttypf(char *id,char *name, char*tty)	//���� �� �޴��� ����ϴ����� �ľ���
{
	FILE *fp;
	MONI moni2;
    int i,n,ret=No;
    char filename[100];
	strcpy(id,"");strcpy(name,"");

    for (n=0;n<20;n++) {
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename, "r"))==NULL) continue;
        while(fread(&moni2,sizeof_moni,1,fp)) {
            if (moni2.mode>0&&
                strlen(moni2.id)>0&&
                moni2.hostno==cfg.hostno&&
                is_same(moni2.tty,tty)) {
                strcpy(id,moni2.id);
                strcpy(name,moni2.name);
                ret=Yes;
                break;
            }
        }
        fclose(fp);
    }
	return ret;
}


