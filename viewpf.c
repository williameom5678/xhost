////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
////////////////////////////////////////////////////////////////////////
#include "util.c"
char nowid[10];
char jobid[10];
PF nowpf,jobpf;

main(int argc, char **argv)
{
	int mode,i;
	mode=atoi(argv[1]);
	strcpy(jobid,argv[2]);
	strcpy(nowid,argv[3]);
	who=atoi(argv[4]);

	if (!read_pf2(jobid)) {mess(" %s ȸ���� ã���� �����ϴ�.",jobid);return;}
	jobpf=pf2;
	read_pf2(nowid);
	nowpf=pf2;
	findport(tty);
	read_ajob();	//�����б�
	read_atype();
	read_cfg();
	set_tmpfile();
	if (is_same(nowid,"root")) {
		strcpy(nowpf.id,cfg.sysop);
		strcpy(nowpf.name,cfg.sysop);
		nowpf.type=19;
		who=5;
	}
	pf=jobpf;
	for (i=0;i<23;i++) printf("\n");
		 if (mode==1) viewpf1();
	else if (mode==8&&who<5&&pf.type>=18) viewpf1();
	else if (mode==8) viewpf8();
	else if (mode==9&&who>=4) viewpf8();
	else if (mode==9&&who<=3) viewpf1();
}


viewpf1()
{
	int num=1;
	view("frm/pf/pf.scr");
							printf("  (%2d) ��      �� : %2d (%s)\n",num++,pf.type,atype[pf.type]);
							printf("  (%2d) ��      �� : %d\n",num++,pf.level);
							printf("  (%2d) ���̹��Ӵ� : %d\n",num++,pf.cash);
							if (is_moni(pf.id))
							printf("  (%2d) ����  ���� : %s ���� �����\n",num++,datestr(33,pf.login));
							else
							printf("  (%2d) �ֱ�  �ð� : %s �� ��������\n",num++,datestr(33,pf.logout));
							printf("  (%2d) ���ͳݸ��� : %s\n",num++,pf.email);
	if (bit(pf.myflag, 1))	printf("  (%2d) ��      �� : %s\n",num++,datestr(1,pf.birthday));
	if (bit(pf.myflag, 2))	printf("  (%2d) ��      �� : %s\n",num++,is_char("0f",pf.sex)?"��":"��");
	if (bit(pf.myflag,10))	printf("  (%2d) ��      �� : %2d %s\n",num++,pf.job,ajob[pf.job]);
	if (bit(pf.myflag, 3))	printf("  (%2d) ��      �� : %s\n",num++,pf.pos1);
	if (bit(pf.myflag, 4))	printf("  (%2d) ��      �� : %s\n",num++,pf.addr);
	if (bit(pf.myflag,11))	printf("  (%2d) ���壯�б� : %s\n",num++,pf.offi);
	if (bit(pf.myflag,17))	printf("  (%2d) ��      �� : %s\n",num++,pf.user4);
	if (bit(pf.myflag,13))	printf("  (%2d) �ϰ������ : %s\n",num++,pf.memo1);
	if (bit(pf.myflag,15))	printf("  (%2d) ����  �о� : %s\n",num++,pf.user2);
	if (bit(pf.myflag,16))	printf("  (%2d) ��ǻ�Ͱ�� : %s\n",num++,pf.user3);
	view("frm/pf/pf.end");
}

viewpf8()
{
	view("frm/pf/pfs.scr");
	view("frm/pf/pfs.end");
}
