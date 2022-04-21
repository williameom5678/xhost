////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: �������� BBS HOST							  //
//			  //			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //	  //
//			  �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����			  //
//			  �������� ������ �޾ƾ� �մϴ�.						  //
////////////////////////////////////////////////////////////////////////
/*
    ���� ��ǥ�� ���α׷� �����ñ׷�
*/
#include "util.c"


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
char MNU_FILE[100];
char now_id[80];

main(argc, argv)
int argc;
char *argv[];
{
    char buff[180];
    char buff2[180];
    char tcode[180];
	int num;
	FILE *fp;

	who=0;

    if (argc<3) {   //root���� ���� �׳�exec
        printf ("\n ��ǥ�� ����  xvote <��ȣȸ> <������̵�>\n");
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
	}

    sprintf(buff,"%s/user",club_go.dir);check_dir(buff);
    sprintf(buff,"%s/user/vote",club_go.dir);check_dir(buff);
    sprintf(NOW_FILE,"%s/user/vote/svote.dat",club_go.dir);
    sprintf(NOW_FILED,"%s/user/vote/svote.vot",club_go.dir);
    sprintf(MNU_FILE,"%s/user/vote/svote.mnu",club_go.dir);
    enter_vote_num(0);
}


//��ǥ�ϱ�
enter_vote_num()
{
	int i,n2;
	FILE *fp,*fp2;
	int now_vote=0;

    read_vote_num();

    if (who>=2||is_same(now_id,vote.id)) now_vote=1;
    view(MNU_FILE);     //ȭ�����

    if (vote.mode==0||!check_vote(now_id,0,0)) {  //��ǥ�ߴ°�?
        printf(" ��ǥ�� �Ͻʽÿ�  (��ȣ:����(0~%d) ENTER:����) ",vote.max);
        if (vote.mode==1) printf("(��ȸ:1��) ");
        if (vote.open==1) printf("(�������) ");
        printf(">>");
		i=numinput();
		printf("\n");
		if (i>0&&i<=vote.max&&i<=100) {
			vote.avote[i-1]++; //���ù�ȣ����
			vote.ans++;   //��ǥ�� ����
			time(&vote.date2); //������ ��ǥ�ð�
            check_vote(now_id,1,i);  //��ǥ�� ����
			printf(" *** ��ǥ�� ���ּż� �����մϴ�. ������ ��ǥ�� �ɰ��Դϴ�. ***\n");
			pressenter();
		}
		else {
			printf(" *** ��ǥ�� ���� �Ǿ����ϴ�. ���� ��ȸ���� �� ��ǥ�� ��Ź�մϴ�. ***\n");
			pressenter();
		}
	}
	vote.read++;
    save_vote_num();         //����
	while (1) {
        printf("\n 0.���� 1.�������� 2.�����б� ");
        if (vote.open==Yes||now_vote==1) printf("3.������ǥ�� 4.��ǥ��� ");
        if (who>2||is_same(now_id,vote.id)) printf("8���� 9ȭ��");
		printf("����>>");
		i=numinput();
		printf("\n");
		if (i<1) return;
        else if (i==1) read_vote_memo(0);
        else if (i==2) read_vote_memo(1);
        else if ((vote.open==Yes||now_vote==1)&&i==3) check_vote(now_id,2,0);
        else if ((vote.open==Yes||now_vote==1)&&i==4) view_vote_result();
        else if (who>=3&&i==4)                        view_vote_result();
        else if ((who>2||is_same(now_id,vote.id))&&i==8) edit_vote_num();
        else if ((who>2||is_same(now_id,vote.id))&&i==9) edit_vote_logo();
	}
}


check_vote(char *id,int mode,int ans)
{
	FILE *fp;
	char buff[100];
	int found;
	int i,n2;

	found=No;
	if (mode==0) {		 //�˻�
        fp=fopen(NOW_FILED,"r+");
		if (fp==NULL) return No;
		fseekend(fp);
		total=ftell(fp)/sizeof_voteman;
		if (fp==NULL) total=0;
		rewind(fp);
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
        fp=fopen(NOW_FILED,"r+");
		fseekend(fp);
		total=ftell(fp)/sizeof_voteman;
		if (fp==NULL) total=0;
        if (fp==NULL) fp=fopen(NOW_FILED,"w+");
        strcpy(voteman.id,now_id);
		strcpy(voteman.nick,pf.nick);
		time(&voteman.date);
		voteman.ans=ans;
		fseek(fp,total*sizeof_voteman,SEEK_SET);
		fwrite(&voteman,sizeof_voteman,1,fp);
        if (fp!=NULL) fclose(fp);
	}
	else if (mode==2) { 	  //Ȯ��
        fp=fopen(NOW_FILED,"r+");
		if (fp==NULL) return No;
		fseekend(fp);
		total=ftell(fp)/sizeof_voteman;
		rewind(fp);
		for (i=0;i<total;i++) {
			fread(&voteman,sizeof_voteman,1,fp);
            printf("%-8s(%-8s) ->%4d %s\n",voteman.id,voteman.nick,voteman.ans,datestr(2,voteman.date));
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
	printf("           %d ��ǥ�� �� ��ǥ ���   (����ǥ��:%d)\n",vote.num,vote.ans);
	printfL2();
	for (i=0;i<vote.max&&vote.ans>0;i++) {
		printf(" ����%3d.->%5d  (%3d%%)\n",i+1,vote.avote[i],vote.avote[i]*100/vote.ans);
	}
	printfL2();
	pressenter();

}

read_vote_memo(int mode)
{
	FILE *fp;
	char filename[100];
	char buff1[100];
	char buff2[100];
	struct tm *tp;
	time_t today;

    sprintf(filename,"%s.txt",NOW_FILE);
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




//�����ϱ�
edit_vote_num()
{
	int i,n2;
	FILE *fp,*fp2;
    char filename[100];

	disp_vote_stat();
    if (who<3) {
        printf("\n\n  *** �� ��ǥ���� �û������� ���� �Ҽ� �ֽ��ϴ�.\n");
		pressenter();
		return;
	}
	disp_vote_stat();
	edit_vote_stat();
    save_vote_num();  //������
    xedit(MNU_FILE);
}




read_vote_num()     //��ǥ�� �б� -> vote
{
	FILE *fp;
    int i;
	//�ʱ�ȭ
    vote.num=1;
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
	fp=fopen(NOW_FILE,"r+");
    if (fp!=NULL) fread(&vote,sizeof_vote,1,fp);
    if (fp!=NULL) fclose(fp);
}


save_vote_num()
{
	FILE *fp;
    fp=fopen(NOW_FILE,"w+");
	fwrite(&vote,sizeof_vote,1,fp);
    if (fp!=NULL) fclose(fp);
}


disp_vote_stat()
{
	char buff[100];
	char buff2[100];

    datestr(0,vote.date);
    datestr(0,vote.date);
	cls();
	printfL2();
	printf("                 %d.��  ��ǥ�� ����\n",vote.num);
	printfL2();
	printf(" 1. ��ǥ������: %-8s(%-8s)\n",vote.id,vote.nick);
    printf(" 2. ��ǥ�尳����: %s\n",datestr(0,vote.date));
    printf(" 3. ��������ǥ��: %s\n",datestr(0,vote.date2));
	printf(" 4. ���� ��Ƚ�� : %d\n",vote.read);
	printf(" 5. ��   ����� : %d\n",vote.ans );
	printf(" 6. ��ǥ��  ����: %s\n",vote.name);
	printf(" 7. ��  ǥ  ����: %d  (0:������ ��ǥ 1:�ѹ��� �� ǥ)\n"     ,vote.mode);
	printf(" 8. �����������: %d  (0:�����      1:����׻����)\n"     ,vote.open);
	printf(" 9. �ִ뼱�ù�ȣ: %d  (0:���úҰ���  2:1~2  99:1~99)\n"      ,vote.max);
	printfL2();
}


edit_vote_stat()
{
	char buff[100];
    xy(19,4 );lineinput(buff,8);if(buff[0]!=0)  strcpy(vote.id,buff);
    xy(19,9 );lineinput(buff,60);if(buff[0]!=0) strcpy(vote.name,buff);
	xy(19,10);lineinput(buff,1 );if(buff[0]!=0) vote.mode=atoi(buff);
    xy(19,11);lineinput(buff,1 );if(buff[0]!=0) vote.open=atoi(buff);
    xy(19,12);lineinput(buff,2 );if(buff[0]!=0) vote.max=atoi(buff);
	if (vote.max<0) vote.max=0;
	if (vote.max>99) vote.max=99;
}


//ȭ�� ����
edit_vote_logo()
{
	char filename[100];
	int i;
    printf(" ��ǥ�� ȭ���� ����մϴ�.\n");
    up_text(0,MNU_FILE);   //ȭ���� ���۹޾� filename�� �����Ѵ�.
}

