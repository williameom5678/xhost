////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: 리눅스용 BBS HOST							  //
//			  //			  개발자 : 김성대  xhost@xhost.co.kr (016-320-7882)		  //	  //
//			  이 소스를 변경하여 상용으로 사용하실때에는			  //
//			  개발자의 승인을 받아야 합니다.						  //
////////////////////////////////////////////////////////////////////////
/*
    작은 투표장 프로그램 독립플그램
*/
#include "util.c"


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

    if (argc<3) {   //root모드로 설정 그냥exec
        printf ("\n 투표장 사용법  xvote <동호회> <현재아이디>\n");
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
	}

    sprintf(buff,"%s/user",club_go.dir);check_dir(buff);
    sprintf(buff,"%s/user/vote",club_go.dir);check_dir(buff);
    sprintf(NOW_FILE,"%s/user/vote/svote.dat",club_go.dir);
    sprintf(NOW_FILED,"%s/user/vote/svote.vot",club_go.dir);
    sprintf(MNU_FILE,"%s/user/vote/svote.mnu",club_go.dir);
    enter_vote_num(0);
}


//투표하기
enter_vote_num()
{
	int i,n2;
	FILE *fp,*fp2;
	int now_vote=0;

    read_vote_num();

    if (who>=2||is_same(now_id,vote.id)) now_vote=1;
    view(MNU_FILE);     //화면출력

    if (vote.mode==0||!check_vote(now_id,0,0)) {  //투표했는가?
        printf(" 투표를 하십시요  (번호:선택(0~%d) ENTER:보류) ",vote.max);
        if (vote.mode==1) printf("(기회:1번) ");
        if (vote.open==1) printf("(결과공개) ");
        printf(">>");
		i=numinput();
		printf("\n");
		if (i>0&&i<=vote.max&&i<=100) {
			vote.avote[i-1]++; //선택번호증가
			vote.ans++;   //투표인 증가
			time(&vote.date2); //마지막 투표시간
            check_vote(now_id,1,i);  //투표자 저장
			printf(" *** 투표를 해주셔서 감사합니다. 귀중한 한표가 될것입니다. ***\n");
			pressenter();
		}
		else {
			printf(" *** 투표가 보류 되었습니다. 다음 기회에는 꼭 한표를 부탁합니다. ***\n");
			pressenter();
		}
	}
	vote.read++;
    save_vote_num();         //저장
	while (1) {
        printf("\n 0.복귀 1.참고적기 2.참고읽기 ");
        if (vote.open==Yes||now_vote==1) printf("3.공개투표인 4.투표결과 ");
        if (who>2||is_same(now_id,vote.id)) printf("8상태 9화면");
		printf("선택>>");
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
	if (mode==0) {		 //검색
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
	else if (mode==1) { 	  //저장(추가)
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
	else if (mode==2) { 	  //확인
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
	printf("           %d 투표장 총 투표 결과   (총투표수:%d)\n",vote.num,vote.ans);
	printfL2();
	for (i=0;i<vote.max&&vote.ans>0;i++) {
		printf(" 선택%3d.->%5d  (%3d%%)\n",i+1,vote.avote[i],vote.avote[i]*100/vote.ans);
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




//편집하기
edit_vote_num()
{
	int i,n2;
	FILE *fp,*fp2;
    char filename[100];

	disp_vote_stat();
    if (who<3) {
        printf("\n\n  *** 이 투표장은 시삽진만이 편집 할수 있습니다.\n");
		pressenter();
		return;
	}
	disp_vote_stat();
	edit_vote_stat();
    save_vote_num();  //저장함
    xedit(MNU_FILE);
}




read_vote_num()     //투표장 읽기 -> vote
{
	FILE *fp;
    int i;
	//초기화
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
	printf("                 %d.번  투표장 상태\n",vote.num);
	printfL2();
	printf(" 1. 투표장담당자: %-8s(%-8s)\n",vote.id,vote.nick);
    printf(" 2. 투표장개설일: %s\n",datestr(0,vote.date));
    printf(" 3. 마지막투표일: %s\n",datestr(0,vote.date2));
	printf(" 4. 읽은 총횟수 : %d\n",vote.read);
	printf(" 5. 총   응답수 : %d\n",vote.ans );
	printf(" 6. 투표장  제목: %s\n",vote.name);
	printf(" 7. 투  표  형식: %d  (0:무제한 투표 1:한번만 투 표)\n"     ,vote.mode);
	printf(" 8. 결과공개여부: %d  (0:비공개      1:결과항상공개)\n"     ,vote.open);
	printf(" 9. 최대선택번호: %d  (0:선택불가능  2:1~2  99:1~99)\n"      ,vote.max);
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


//화면 편집
edit_vote_logo()
{
	char filename[100];
	int i;
    printf(" 투표장 화면을 등록합니다.\n");
    up_text(0,MNU_FILE);   //화일을 전송받아 filename에 저장한다.
}

