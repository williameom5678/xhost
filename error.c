////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���� : ERROR ���� Ȯ�� ��ƾ
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

// ������ ��Ȳ����
dev()
{
    char OLDDIR[300];
	getcwd(OLDDIR,sizeof(OLDDIR));		//������ ���丮
    printf("\n");
    printf("��ȣȸ  :%s %s\n",club_go.code,club_go.dir);
    printf("�����ڵ�:%s*%s*(Ÿ��%c  ����%d No:%d)\n",go.code,go.name,go.type,go.deep,go.rec);
	printf("���丮:%s  (����:%s)\n",go.dir,OLDDIR);
    printf("�޴�����:%s\n",NOW_MENU);
    printf("NOW_FILE:%s(%d) %s(%d)\n",NOW_FILE,file_size(NOW_FILE),NOW_FILED,file_size(NOW_FILED));
    printf("TMP_FILE:%s(%d) %s(%d)\n",TMP_FILE,file_size(TMP_FILE),TMP_FILE2,file_size(TMP_FILE2));
	printf("�Խ��Ǵ��:%s  ��ȣȸ���:%s\n",bcfg.man1,ccfg.sysop);
	printf("�۾����̵�:%s\n",pf.id);
	printf("���    :%d\n",now_type);
	printf("�۾�����:%d \n",pf.level);
	printf("�۾����:%d \n",pf.type);
	printf("re_club :%d\n",re_club );
	printf("re_print:%d\n",re_print);
	printf("re_bbs  :%d\n",re_bbs  );
	printf("re_logo :%d\n",re_logo );
	printf("club_mode:%d\n",club_mode);
	printf("list_mode:%d\n",list_mode);
	getchar();
}


check_error(int mode)
{
    if (is_file("system/xcheck9")) {
        cls();
        dev();
        printf(" %s %s\n",VER1,VER2);
        printf(" mode=%d",mode);system("id");
        printf(" ���̵�NO:%d ���̵�:%s ����:%s ��ȣ:%d TTY:%s\n",pf.num+1,pf.id,pf.name,pf.num+1,tty);
        printf(" us:%s %s %s\n",moni.id,moni.name,moni.gocode);
        printf(" ���� ȭ���� ������ �ؼ� ��ڿ��� �˷� �ֽñ� �ٶ��ϴ�.\n");
        pressenter();
    }
}


