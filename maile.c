////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: �������� BBS HOST							  //
//			  //			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //	  //
//			  �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����			  //
//			  �������� ������ �޾ƾ� �մϴ�.						  //
////////////////////////////////////////////////////////////////////////
//	  ���ϰ��� ��ƾ
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
wmail_email(char *tid,char *filename)
{
	int i,n,nn=0;
	char mailid[51];
	char hostname[51];
	char buff[250];
	FILE *fp;

	if (pf.type<cfg.emailwrite) {view("frm/mail/notemail.log");return;}

	bzero(mailid,50);
	bzero(hostname,50);
	n=0;
	nn=0;
	for (i=0;i<(int)strlen(tid);i++) {
		if (tid[i]=='@') {
			n=1;nn=0;
		}
		else if (n==0) mailid[nn++]=tid[i];
		else if (n==1) hostname[nn++]=tid[i];
	}
//	  printf("*%s*%s*%s*",tid,mailid,hostname);getchar();//??????????????????
	if (mailid[0]==0||hostname[0]==0) {
		view("frm/mail/notmai3.log");
		return;
	}
	set_stat2(5,"E���� ������");

	view("frm/mail/emails.log");
	//E-MAIL�� �߼��Ѵ�.
	if ((fp=fopen(TMP_FILE2,"w"))==NULL) return;
	fprintf(fp,"%s\n",mailid);
	fprintf(fp,"%s\n",hostname);
	fprintf(fp,"%s\n",pf.id);
	fprintf(fp,"%s\n",bit(bcfg.mode,8)?pf.nick:pf.name);
	fprintf(fp,"%s\n",bbs.title);
	fprintf(fp,"%s\n",bbs.key);
	fprintf(fp,"%s\n",bbs.filename);
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fclose(fp);
	sprintf(buff,"bin/xmail %s %s &",TMP_FILE,TMP_FILE2);
	n=sys_tem2(buff);
	sprintf(buff,"E�߼� %s %s",mailid,hostname);
	set_rec_(1,9,0,buff);
}

