////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: 리눅스용 BBS HOST							  //
//			  개발자 : 김성대  xhost@xhost.co.kr (016-320-7882)		  //
//			  이 소스를 변경하여 상용으로 사용하실때에는			  //
//			  개발자의 승인을 받아야 합니다.						  //
////////////////////////////////////////////////////////////////////////
/*
	투표장 프로그램 독립플그램
*/
#include "bbs.h"


#ifndef sizeof_vote
// 투표장 정보 화일헤더
typedef struct {			// 게시물 정보
	int num;				// 투표장 번호
	char id[9]; 			// 담당자 아이디
	char nick[9];			// 작성자 별명
	char name[70];			// 투표장 제목
	int  mode;				// 투표형식
	int  open;				// 공개여부
	int  max;				// 최대 선택번호
	int  ans;				// 투표인수
	int  read;				// 읽은횟수
	time_t date;			// 작성 일자
	time_t date2;			// 작성 일자
	int avote[100]; 		 // 응답 결과
} VOTE;
#define sizeof_vote (sizeof(VOTE))
VOTE vote;

// 투표장 정보 화일헤더
typedef struct {			// 게시물 정보
	int num;				// 투표장 번호
	char id[9]; 			// 담당자 아이디
	char nick[9];			// 작성자 별명
	int  ans;				// 투표인수
	time_t date;			// 작성 일자
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

    if (argc<4) {   //root모드로 설정 그냥exec
        printf ("\n 투표장 사용법  xvote <동호회> <현재아이디> <담당자>\n");
		return;
	}

	findport(tty); //접속포트 찾기
    strcpy(tcode,argv[1]);
    strcpy(now_id,argv[2]);
    strcpy(pf.nick,now_id);
    club_mode=find_club(tcode);
	if (is_same(now_id,"root"))         who=5;
	else if (is_same(cfg.sysop,now_id)) who=4;
	else if(read_pf(now_id)) {	  //아이디 읽기
		who=0;
        if (club_mode) read_cpf(now_id);
        if (pf.type==19) who=4;  //root
        if (pf.type==18) who=3;  //부시삽
        if (pf.type==17) who=2;  //운영진
        if (pf.type==16) who=2;  //운영진
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
    logo(buff2,Yes);    //진입화면
    sprintf(NOW_FILE,"%s/user/vote/voteall.dat",club_go.dir);
    sprintf(TMP_FILE,"%s/user/vote/voteall2.dat",club_go.dir);
    touch_file(NOW_FILE);
    touch_file(TMP_FILE);

	while (1) {
        disp_vote_name();     //투표장 제목들 출력
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


//투표하기
enter_vote_num(int n)
{
	int i,n2;
	FILE *fp,*fp2;
	int now_vote=0;

	if (n<1) return;

	if (!read_vote_num(n)) {
		printf("  *** %d는 존재하지 않는 투표장 번호 입니다. ***\n",n);
		pressenter();
		return;
	}

    if (who>=2||is_same(now_id,vote.id)) now_vote=1;
	disp_vote_logo(n);	   //화면출력

    if (vote.mode==0||!check_vote(n,now_id,0,0)) {  //투표했는가?
        view("user/vote/vote1.cmd");
		i=numinput();
		printf("\n");
		if (i>0&&i<=vote.max&&i<=100) {
            vote.avote[i-1]++;          //선택번호증가
            vote.ans++;                 //투표인 증가
            time(&vote.date2);          //마지막 투표시간
            check_vote(n,now_id,1,i);   //투표자 저장
            view("user/vote/vote2.cmd");
		}
		else {
            view("user/vote/vote3.cmd");
		}
    }
	vote.read++;
	save_vote_num(n);		  //저장
	while (1) {
        if (vote.open||now_vote==1)         view("user/vote/vote4-1.cmd");// printf("3.공개투표인  4.투표결과  ");
        if (who>1||is_same(now_id,vote.id)) view("user/vote/vote4-2.cmd");// printf("9.화면수정  ");
        else                                view("user/vote/vote4-3.cmd");// printf("(투표인비공개)  ");
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
	if (mode==0) {		 //검색
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
	else if (mode==1) { 	  //저장(추가)
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
	else if (mode==2) { 	  //확인
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
    printf("           %d 투표장 총 투표 결과  (총투표수:%d) 담당자:%s\n",vote.num,vote.ans,vote.id);
	printfL2();
	for (i=0;i<vote.max&&vote.ans>0;i++) {
		printf(" 선택%3d.->%5d  (%3d%%)\n",i+1,vote.avote[i],vote.avote[i]*100/vote.ans);
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




//새로 만들기
new_vote_num(int n)
{
	int i;
	if (n<1) {
        printf("새로 만들 투표장 번호는? (1~9999)>");
		n=numinput();
		printf("\n");
	}

	if (n<1||n>9999) return;

	if (read_vote_num(n)) {
		printf("  *** %d는 이미 존재하는 투표장 번호 입니다. ***\n",n);
		pressenter();
		return;
	}

	//초기화
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
	save_vote_num(n);  //저장함
    printf("\n\n\n 이제 투표장이 만들어 졌습니다. 투표장의 화면을 만드십시요.\n");
	edit_vote_logo(n);
}




//삭제하기
del_vote_num(int n)
{
	int i,n2;
	FILE *fp,*fp2;
    char buff[200];

	if (n<1) {
        printf("삭제할 투표장 번호는? (1~9999)>");
		n=numinput();
		printf("\n");
	}

	if (n<1||n>9999) return;

	if (!read_vote_num(n)) {
		printf("  *** %d는 존재하지 않는 투표장 번호 입니다. ***\n",n);
		pressenter();
		return;
	}


	disp_vote_num(n);
    if (who<2&&!is_same(now_id,vote.id)) {
		printf("\n\n  *** 이 투표장은 개설자만이 삭제 할수 있습니다.\n");
		pressenter();
		return;
	}

	if (yesno("\n\n *** 이 투표장을 지울까요? (y/N) >>",No)==Yes) {
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
		printf("\n *** %d 번 투표장이 지워졌습니다. ***\n",n);
		pressenter();
		return;
	}
	printf("\n *** %d 번 투표장 삭제가 취소되었습니다. ***\n",n);
	pressenter();
}




//편집하기
edit_vote_num(int n)
{
	int i,n2;
	FILE *fp,*fp2;
    char filename[100];

	if (n<1) {
        printf("편집할 투표장 번호는? (1~9999)>");
		n=numinput();
		printf("\n");
	}

	if (n<1||n>9999) return;

	if (!read_vote_num(n)) {
		printf("  *** %d는 존재하지 않는 투표장 번호 입니다. ***\n",n);
		pressenter();
		return;
	}


	disp_vote_stat();
    if (who<2&&!is_same(now_id,vote.id)) {
		printf("\n\n  *** 이 투표장은 개설자만이 편집 할수 있습니다.\n");
		pressenter();
		return;
	}
	disp_vote_stat();
	edit_vote_stat();
	save_vote_num(n);  //저장함
    sprintf(filename,"%s/user/vote/vote%d.scr",club_go.dir,n);
    xedit(filename);
}




disp_vote_name()	 //투표장 제목들 출력
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
	//투표장 화면 출력
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


read_vote_num(int num)	   //투표장 읽기 -> vote
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
    if (!logo(buff2,No)) {     //배경화면
		cls();
		printfL2();
		printf("                          XHOST 자유 투표장\n");
		printfL2();
	}
}


disp_vote_num(int num)
{
	if (num==0) {
        printf("확인할 투표장 번호는? (1~9999)>");
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
	printf("                 %d.번  투표장 상태\n",vote.num);
	printfL2();
	printf(" 1. 투표장담당자: %-8s(%-8s)\n",vote.id,vote.nick);
	printf(" 2. 투표장개설일: %s\n",buff);
	printf(" 3. 마지막투표일: %s\n",buff2);
	printf(" 4. 읽은 총횟수 : %d\n",vote.read);
	printf(" 5. 총   응답수 : %d\n",vote.ans );
	printf(" 6. 투표장  제목: %s\n",vote.name);
	printf(" 7. 투  표  형식: %d  (0:무제한 투표 1:한번만 투 표)\n"     ,vote.mode);
    printf(" 8. 결과공개여부: %d  (0:비공개      1:결과항상공개   2:시삽에게만 공개)\n"  ,vote.open);
    printf(" 9. 최대선택번호: %d  (0:선택불가능  2:1~2  99:1~99)\n"     ,vote.max);
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


//화면 편집
edit_vote_logo(int n)
{
	char filename[100];
    sprintf(filename,"%s/user/vote/vote%d.scr",club_go.dir,n);
    printf(" 투표장 화면을 등록합니다.\n");
    up_text(0,filename);   //화일을 전송받아 filename에 저장한다.
}


//화면 출력
disp_vote_logo(int n)
{
	char filename[100];
    sprintf(filename,"%s/user/vote/vote%d.scr",club_go.dir,n);
	cls();
	view(filename);
}
