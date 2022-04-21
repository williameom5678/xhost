////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: �������� BBS HOST							  //
//			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //
//			  �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����			  //
//			  �������� ������ �޾ƾ� �մϴ�.						  //
////////////////////////////////////////////////////////////////////////
/*
	��ǥ�� ���α׷� �����ñ׷�
*/
#include "bbs.h"


#ifndef sizeof_vote
// ��ǥ�� ���� ȭ�����
typedef struct {			// �Խù� ����
	int num;				// ��ǥ�� ��ȣ
	char id[9]; 			// ����� ���̵�
	char nick[9];			// �ۼ��� ����
	char name[70];			// ��ǥ�� ����
	int  mode;				// ��ǥ����
	int  open;				// ��������
	int  max;				// �ִ� ���ù�ȣ
	int  ans;				// ��ǥ�μ�
	int  read;				// ����Ƚ��
	time_t date;			// �ۼ� ����
	time_t date2;			// �ۼ� ����
	int avote[100]; 		 // ���� ���
} VOTE;
#define sizeof_vote (sizeof(VOTE))
VOTE vote;

// ��ǥ�� ���� ȭ�����
typedef struct {			// �Խù� ����
	int num;				// ��ǥ�� ��ȣ
	char id[9]; 			// ����� ���̵�
	char nick[9];			// �ۼ��� ����
	int  ans;				// ��ǥ�μ�
	time_t date;			// �ۼ� ����
} VOTEMAN;
VOTEMAN voteman;
#define sizeof_voteman (sizeof(VOTEMAN))

#endif

int total;
char now_id[80];

main(argc, argv)
int argc;
char *argv[];
{
	char buff[80];
    char buff2[80];
    char tcode[80];
	int num;
	FILE *fp;

	who=0;

    if (argc<4) {   //root���� ���� �׳�exec
        printf ("\n ��ǥ�� ����  xvote <��ȣȸ> <������̵�> <�����>\n");
		return;
	}

	findport(tty); //������Ʈ ã��
    strcpy(tcode,argv[1]);
    strcpy(now_id,argv[2]);
    strcpy(pf.nick,now_id);
    club_mode=find_club(tcode);
	if (is_same(now_id,"root"))         who=5;
	else if (is_same(cfg.sysop,now_id)) who=4;
	else if(read_pf(now_id)) {	  //���̵� �б�
		who=0;
        if (club_mode) read_cpf(now_id);
        if (pf.type==19) who=4;  //root
        if (pf.type==18) who=3;  //�νû�
        if (pf.type==17) who=2;  //���
        if (pf.type==16) who=2;  //���
        if (club_mode&&cpf.type==19) who=4;
        if (club_mode&&cpf.type==18) who=3;
        if (club_mode&&cpf.type==17) who=2;
        if (club_mode&&cpf.type==16) who=2;
        if (is_same(now_id,argv[3])) who=2;
	}
    if (pf.type<cfg.vote_type) {
        view("user/vote/vote.er1");
        return;
    }
    sprintf(buff2,"%s/user",club_go.dir);check_dir(buff2);
    sprintf(buff2,"%s/user/vote",club_go.dir);check_dir(buff2);
    sprintf(buff2,"%s/user/vote/vote.log",club_go.dir);
    logo(buff2,Yes);    //����ȭ��
    sprintf(NOW_FILE,"%s/user/vote/voteall.dat",club_go.dir);
    sprintf(TMP_FILE,"%s/user/vote/voteall2.dat",club_go.dir);
    touch_file(NOW_FILE);
    touch_file(TMP_FILE);

	while (1) {
        disp_vote_name();     //��ǥ�� ����� ���
        if (pf.type>=cfg.vote_opentype) view("%s/user/vote/vote.cmd",club_go.dir);
        else                            view("%s/user/vote/vote-1.cmd",club_go.dir);
        lineinput(buff,10);check_han(buff);printf("\n");
        check_cmd(buff);
		if (mmn[1]>9999) mmn[1]=9999;
        if (is_quit(buff)) break;
        if      (who>1&&is_same(mmc[0],"n")&&pf.type>=cfg.vote_opentype) new_vote_num(mmn[1]);
        else if (who>1&&is_same(mmc[0],"d")&&pf.type>=cfg.vote_opentype) del_vote_num(mmn[1]);
        else if (who>1&&is_same(mmc[0],"l")&&pf.type>=cfg.vote_opentype) disp_vote_num(mmn[1]);
        else if (who>1&&is_same(mmc[0],"e")&&pf.type>=cfg.vote_opentype) edit_vote_num(mmn[1]);
		else if (mmn[0]==0||is_same(mmc[0],"p")) break;
        else                                 enter_vote_num(mmn[0]);
	}
}


//��ǥ�ϱ�
enter_vote_num(int n)
{
	int i,n2;
	FILE *fp,*fp2;
	int now_vote=0;

	if (n<1) return;

	if (!read_vote_num(n)) {
		printf("  *** %d�� �������� �ʴ� ��ǥ�� ��ȣ �Դϴ�. ***\n",n);
		pressenter();
		return;
	}

    if (who>=2||is_same(now_id,vote.id)) now_vote=1;
	disp_vote_logo(n);	   //ȭ�����

    if (vote.mode==0||!check_vote(n,now_id,0,0)) {  //��ǥ�ߴ°�?
        view("user/vote/vote1.cmd");
		i=numinput();
		printf("\n");
		if (i>0&&i<=vote.max&&i<=100) {
            vote.avote[i-1]++;          //���ù�ȣ����
            vote.ans++;                 //��ǥ�� ����
            time(&vote.date2);          //������ ��ǥ�ð�
            check_vote(n,now_id,1,i);   //��ǥ�� ����
            view("user/vote/vote2.cmd");
		}
		else {
            view("user/vote/vote3.cmd");
		}
    }
	vote.read++;
	save_vote_num(n);		  //����
	while (1) {
        if (vote.open||now_vote==1)         view("user/vote/vote4-1.cmd");// printf("3.������ǥ��  4.��ǥ���  ");
        if (who>1||is_same(now_id,vote.id)) view("user/vote/vote4-2.cmd");// printf("9.ȭ�����  ");
        else                                view("user/vote/vote4-3.cmd");// printf("(��ǥ�κ����)  ");
        i=numinput();
		printf("\n");
		if (i<1) return;
		else if (i==1) read_vote_memo(n,0);
		else if (i==2) read_vote_memo(n,1);
        else if ((vote.open==1||now_vote==1)&&i==3) check_vote(n,now_id,2,0);
        else if ((vote.open==1||now_vote==1)&&i==4) view_vote_result(n);
        else if ((vote.open==2||now_vote==1)&&i==4&&who>2) view_vote_result(n);
        else if (i==4&&who>=3)                      view_vote_result(n);
        else if ((who>1||is_same(now_id,vote.id))&&i==9) edit_vote_logo(n);
	}
}


check_vote(int n,char *id,int mode,int ans)
{
	FILE *fp;
	char filename[100];
	char buff[100];
	int found;
	int i,n2;

    sprintf(filename,"%s/user/vote/vote%d.dat",club_go.dir,n);
    touch_file(filename);
	found=No;
	if (mode==0) {		 //�˻�
		fp=fopen(filename,"r+");
		if (fp==NULL) return No;
		fseek(fp,0,SEEK_END);
		total=ftell(fp)/sizeof_voteman;
		if (fp==NULL) total=0;
		fseek(fp,0,SEEK_SET);
		for (i=0;i<total;i++) {
			fread(&voteman,sizeof_voteman,1,fp);
			if (is_same(voteman.id,id)) {
				found=Yes;
				break;
			}
		}
        if (fp!=NULL) fclose(fp);
	}
	else if (mode==1) { 	  //����(�߰�)
		fp=fopen(filename,"r+");
		fseek(fp,0,SEEK_END);
		total=ftell(fp)/sizeof_voteman;
		if (fp==NULL) total=0;
		if (fp==NULL) fp=fopen(filename,"w+");
        strcpy(voteman.id,id);
		strcpy(voteman.nick,pf.nick);
		time(&voteman.date);
		voteman.ans=ans;
		fseek(fp,total*sizeof_voteman,SEEK_SET);
		fwrite(&voteman,sizeof_voteman,1,fp);
        if (fp!=NULL) fclose(fp);
	}
	else if (mode==2) { 	  //Ȯ��
		fp=fopen(filename,"r+");
		if (fp==NULL) return No;
		fseek(fp,0,SEEK_END);
		total=ftell(fp)/sizeof_voteman;
		fseek(fp,0,SEEK_SET);
		for (i=0;i<total;i++) {
			fread(&voteman,sizeof_voteman,1,fp);
            strcpy(buff,datestr(2,voteman.date));
			printf("%-8s(%-8s) ->%4d %s\n",voteman.id,voteman.nick,voteman.ans,buff);
		}
        if (fp!=NULL) fclose(fp);
	}
	return found;
}



view_vote_result()
{
	int i;
	cls();
	printfL2();
    printf("           %d ��ǥ�� �� ��ǥ ���  (����ǥ��:%d) �����:%s\n",vote.num,vote.ans,vote.id);
	printfL2();
	for (i=0;i<vote.max&&vote.ans>0;i++) {
		printf(" ����%3d.->%5d  (%3d%%)\n",i+1,vote.avote[i],vote.avote[i]*100/vote.ans);
	}
	printfL2();
	pressenter();

}

read_vote_memo(int n,int mode)
{
	FILE *fp;
	char filename[100];
	char buff1[100];
	char buff2[100];
	struct tm *tp;
	time_t today;

    sprintf(filename,"%s/user/vote/vote%d.txt",club_go.dir,n);
	if (mode==0) {
		fp=fopen(filename,"a+");
		if (fp==NULL) fp=fopen(filename,"w");
		time(&today);
		tp=localtime(&today);
        sprintf(buff1,"%-8s(%2d/%2d)>> ",now_id,tp->tm_mon+1,tp->tm_mday);
		printf("%s",buff1);
		lineinput(buff2,60);
		if (buff2[0]!=0) fprintf(fp,"%s%s\n",buff1,buff2);
        if (fp!=NULL) fclose(fp);
	}
    else view_text(filename);
}




//���� �����
new_vote_num(int n)
{
	int i;
	if (n<1) {
        printf("���� ���� ��ǥ�� ��ȣ��? (1~9999)>");
		n=numinput();
		printf("\n");
	}

	if (n<1||n>9999) return;

	if (read_vote_num(n)) {
		printf("  *** %d�� �̹� �����ϴ� ��ǥ�� ��ȣ �Դϴ�. ***\n",n);
		pressenter();
		return;
	}

	//�ʱ�ȭ
	vote.num=n;
	vote.mode=1;
	vote.open=1;
	vote.ans=0;
	vote.max=2;
	time(&vote.date);
	time(&vote.date2);
    strcpy(vote.id,now_id);
	strcpy(vote.nick,pf.nick);
	strcpy(vote.name,"");
	for (i=0;i<99;i++) vote.avote[i]=0;

	disp_vote_stat();
	edit_vote_stat();
	save_vote_num(n);  //������
    printf("\n\n\n ���� ��ǥ���� ����� �����ϴ�. ��ǥ���� ȭ���� ����ʽÿ�.\n");
	edit_vote_logo(n);
}




//�����ϱ�
del_vote_num(int n)
{
	int i,n2;
	FILE *fp,*fp2;
    char buff[200];

	if (n<1) {
        printf("������ ��ǥ�� ��ȣ��? (1~9999)>");
		n=numinput();
		printf("\n");
	}

	if (n<1||n>9999) return;

	if (!read_vote_num(n)) {
		printf("  *** %d�� �������� �ʴ� ��ǥ�� ��ȣ �Դϴ�. ***\n",n);
		pressenter();
		return;
	}


	disp_vote_num(n);
    if (who<2&&!is_same(now_id,vote.id)) {
		printf("\n\n  *** �� ��ǥ���� �����ڸ��� ���� �Ҽ� �ֽ��ϴ�.\n");
		pressenter();
		return;
	}

	if (yesno("\n\n *** �� ��ǥ���� ������? (y/N) >>",No)==Yes) {
		fp=fopen(NOW_FILE,"r+");
		fp2=fopen(TMP_FILE,"w");
		fseek(fp,0,SEEK_END);
		total=ftell(fp)/sizeof_vote;
		if (fp==NULL) total=0;
		for (i=0;i<total;i++) {
			fseek(fp,i*sizeof_vote,SEEK_SET);
			fread(&vote,sizeof_vote,1,fp);
			if (vote.num!=n&&vote.num>0)
				fwrite(&vote,sizeof_vote,1,fp2);
		}
        if (fp!=NULL) fclose(fp);
        if (fp2!=NULL) fclose(fp2);
		file_change(NOW_FILE,TMP_FILE);
		unlink(TMP_FILE);
        sprintf(buff,"rm -f user/vote/vote%d.*",n);system(buff);
		printf("\n *** %d �� ��ǥ���� ���������ϴ�. ***\n",n);
		pressenter();
		return;
	}
	printf("\n *** %d �� ��ǥ�� ������ ��ҵǾ����ϴ�. ***\n",n);
	pressenter();
}




//�����ϱ�
edit_vote_num(int n)
{
	int i,n2;
	FILE *fp,*fp2;
    char filename[100];

	if (n<1) {
        printf("������ ��ǥ�� ��ȣ��? (1~9999)>");
		n=numinput();
		printf("\n");
	}

	if (n<1||n>9999) return;

	if (!read_vote_num(n)) {
		printf("  *** %d�� �������� �ʴ� ��ǥ�� ��ȣ �Դϴ�. ***\n",n);
		pressenter();
		return;
	}


	disp_vote_stat();
    if (who<2&&!is_same(now_id,vote.id)) {
		printf("\n\n  *** �� ��ǥ���� �����ڸ��� ���� �Ҽ� �ֽ��ϴ�.\n");
		pressenter();
		return;
	}
	disp_vote_stat();
	edit_vote_stat();
	save_vote_num(n);  //������
    sprintf(filename,"%s/user/vote/vote%d.scr",club_go.dir,n);
    xedit(filename);
}




disp_vote_name()	 //��ǥ�� ����� ���
{
	FILE *fp;
	int i,yy;
	VOTE vote;

	fp=fopen(NOW_FILE,"r+");
    if (fp==NULL) return;
	fseek(fp,0,SEEK_END);
	total=ftell(fp)/sizeof_vote;
	if (fp==NULL) total=0;
	yy=0;
	disp_vote_title();
	fseek(fp,0,SEEK_SET);
	//��ǥ�� ȭ�� ���
	for (i=0;i<total;i++) {
		fread(&vote,sizeof_vote,1,fp);
		if (vote.num>0) {
			xy(2,(yy%15)+4);
            printf("%4d.%-56.56s%d%d %-8s/%4d\n",
            vote.num,vote.name,vote.mode,vote.open,vote.id,vote.ans);
			if ((yy++)%15==14) {
				printf("(%d,%d)",i,total);
				pressenter();
				disp_vote_logo();
			}
		}
	}
    if (fp!=NULL) fclose(fp);
}


read_vote_num(int num)	   //��ǥ�� �б� -> vote
{
	FILE *fp;
	int ret=No;
	if (num<1||num>9999) return No;
	fp=fopen(NOW_FILE,"r+");
	fseek(fp,0,SEEK_SET);
	while (fread(&vote,sizeof_vote,1,fp)) {
		if (vote.num==num) {
			ret=Yes;
			break;
		}
	}
    if (fp!=NULL) fclose(fp);
	return ret;
}


save_vote_num(int num)
{
	FILE *fp;
	int i;
	VOTE vote2;

	if (num<1||num>9999) return;
	fp=fopen(NOW_FILE,"r+");
	if (fp==NULL) fp=fopen(NOW_FILE,"w");
	else {
		fseek(fp,0,SEEK_SET);
		i=0;
		while (fread(&vote2,sizeof_vote,1,fp)) {
			if (vote2.num==num) break;
			i++;
		}
		fseek(fp,i*sizeof_vote,SEEK_SET);
	}
	fwrite(&vote,sizeof_vote,1,fp);
    if (fp!=NULL) fclose(fp);
}


disp_vote_title()
{
    char buff2[100];
    sprintf(buff2,"%s/user/vote/vote.mnu",club_go.dir);
    if (!logo(buff2,No)) {     //���ȭ��
		cls();
		printfL2();
		printf("                          XHOST ���� ��ǥ��\n");
		printfL2();
	}
}


disp_vote_num(int num)
{
	if (num==0) {
        printf("Ȯ���� ��ǥ�� ��ȣ��? (1~9999)>");
        num=numinput();
		printf("\n");
	}
	if (read_vote_num(num)) {
		disp_vote_stat();
		pressenter();
	}

}

disp_vote_stat()
{
	char buff[100];
	char buff2[100];

    strcpy(buff,datestr(0,vote.date));
    strcpy(buff2,datestr(0,vote.date));
	cls();
	printfL2();
	printf("                 %d.��  ��ǥ�� ����\n",vote.num);
	printfL2();
	printf(" 1. ��ǥ������: %-8s(%-8s)\n",vote.id,vote.nick);
	printf(" 2. ��ǥ�尳����: %s\n",buff);
	printf(" 3. ��������ǥ��: %s\n",buff2);
	printf(" 4. ���� ��Ƚ�� : %d\n",vote.read);
	printf(" 5. ��   ����� : %d\n",vote.ans );
	printf(" 6. ��ǥ��  ����: %s\n",vote.name);
	printf(" 7. ��  ǥ  ����: %d  (0:������ ��ǥ 1:�ѹ��� �� ǥ)\n"     ,vote.mode);
    printf(" 8. �����������: %d  (0:�����      1:����׻����   2:�û𿡰Ը� ����)\n"  ,vote.open);
    printf(" 9. �ִ뼱�ù�ȣ: %d  (0:���úҰ���  2:1~2  99:1~99)\n"     ,vote.max);
	printfL2();
}


edit_vote_stat()
{
	char buff[100];
	xy(19,9 );lineinput(buff,60);if(buff[0]!=0) strcpy(vote.name,buff);
	xy(19,10);lineinput(buff,1 );if(buff[0]!=0) vote.mode=atoi(buff);
    xy(19,11);lineinput(buff,1 );if(buff[0]!=0) vote.open=atoi(buff);
    xy(19,12);lineinput(buff,2 );if(buff[0]!=0) vote.max=atoi(buff);
	if (vote.max<0) vote.max=0;
	if (vote.max>99) vote.max=99;
}


//ȭ�� ����
edit_vote_logo(int n)
{
	char filename[100];
    sprintf(filename,"%s/user/vote/vote%d.scr",club_go.dir,n);
    printf(" ��ǥ�� ȭ���� ����մϴ�.\n");
    up_text(0,filename);   //ȭ���� ���۹޾� filename�� �����Ѵ�.
}


//ȭ�� ���
disp_vote_logo(int n)
{
	char filename[100];
    sprintf(filename,"%s/user/vote/vote%d.scr",club_go.dir,n);
	cls();
	view(filename);
}
