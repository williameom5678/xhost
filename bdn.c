////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 게시판 담당자의 메뉴
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//사용법  bsysop hello 1 forum

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp1,*fp2,*fp3,*fp4;
	PF pf2;
	int i,n=0,n1,n2;
	int key=No;
	int now_posi,count;
    char buff[140],buff1[140],buff2[140],buff3[140],buff4[140];
	char filename[140];
	char tcode[9];
	char ccode[9];
	char id[9];
	char old_dir[100];
	char ch,c;
	time_t date1,date2;
	getcwd(old_dir,sizeof(old_dir));	  //현재의 디렉토리

	strcpy(tcode,argv[1]);				//가기코드
	strcpy(ccode,argv[2]);				//동호회 코드
	strcpy(id,argv[3]); 				//아이디
	findport(tty); //접속포트 찾기
	read_cfg();
	set_tmpfile();		 //임시화일지정하기
	if (!find_club(ccode)) {
		mess("%s 동호회 검색 실패",ccode);
		return;
	}
	set_nowmenu();		 //데이타화일지정하기
	if (!find_go(tcode)) {
		mess("%s 가기코드 검색 실패",tcode);
		return;    //찾는 가기코드가 없다
	}

	read_ccfg();
	set_bcfgfile(); 					//BCFG_FILE에 저장
	read_bcfg();
	read_pf(id);
	read_cpf(id);					 //해당되는 동호회의 아이디를 찾는다.
	read_atype();					 //등급읽기
	read_actype();					 //동호회등급의 이름을 읽어옴  ->actype
	who=check_who();				 //1:동호회 운영진 2:동호회 시삽  3:root권한

	n1=n2=date1=date2=0;
	strcpy(buff1,"");
	strcpy(buff2,"");
	strcpy(buff3,"");
    strcpy(buff4,"");

	while (1) {
		if (n2==0) n2=99999999;
		if (n1>n2) {n=n1;n1=n2;n2=n;}
		if (date2==0) time(&date2);
		if (strlen(buff3)<1) strcpy(buff3,go.code);
		sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
		sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,go.code);
		sprintf(filename,"%s.zip",buff3);
		cls();
		printfL2();
		printf("  %s(%s)      게시판 선택 다운 작업\n",go.code,go.name);
		printfL2();
		printf(" * 가기코드명 : %s (%s)\n",go.code,go.name);
		printf(" * 가기화일명 : %s (%s)\n",NOW_FILE,NOW_FILED);
		printf(" * 압축화일명 : %s\n",filename);
		printfL2();
		printf(" 1.  자료 번호 시작 - 끝  : %d - %d\n",n1,n2);
		strcpy(buff,datestr(1,date1));
		printf(" 2.  자료 날짜 시작 - 끝  : %s ~ %s\n",buff,datestr(1,date2));
		printf(" 3.  추출할 아이디        : %s\n",buff1);
        printf(" 4.  추출할 TCODE         : %s\n",buff4);
        printf(" 5.  추출할 제목문자열    : %s\n",buff2);
        printf(" 6.  받을 파일명          : %s\n",buff3);
        printf(" 7.  위의 조건으로 %s 게시판 다운 시작\n",go.code);
        printf(" 8.  위의 조건으로 %s XFORMAT다운 시작\n",go.code);
		printfL2();
		printf(" 선택  취소(Q) 도움(H) >> ");
		lineinput(buff,5);check_han(buff);printf("\n");
		n=atoi(buff);
		if (is_same(buff,"x")||is_same(buff,"q")||is_same(buff,"p")) return;
		else if (is_same(buff,"h")) {view("frm/help/bdn.hlp");pressenter();}
		else if (n==1) {
			printf(" 다운받을 번호의 시작번호를 입력해주세요.. >>");
			n1=numinput();printf("\n");
			printf(" 다운받을 번호의 끝  번호를 입력해주세요.. >>");
			n2=numinput();printf("\n");
		}
		else if (n==2) {
			strcpy(buff,datestr(1,date1));
			printf(" 다운받을 자료의 시작날짜를 입력해주세요..(%s) >>",buff);
			dosinput(buff,10);date1=str2date(buff);printf("\n");
			strcpy(buff,datestr(1,date2));
			printf(" 다운받을 자료의 끝  날짜를 입력해주세요..(%s) >>",buff);
			dosinput(buff,10);date2=str2date(buff);printf("\n");
		}
		else if (n==3) {
			printf(" 추출할 아이디를 입력하세요. (없으면Enter) >>");
			lineinput(buff1,8);printf("\n");
			if (is_quit(buff1)) return;
		}
        else if (n==4) {
            printf(" 추출할 TCODE 를 입력하세요. (없으면Enter) >>");
            lineinput(buff4,8);printf("\n");
            if (is_quit(buff4)) return;
		}
        else if (n==5) {
			printf(" 추출할 제목의 문자열 입력   (없으면Enter) >>");
			lineinput(buff2,10);printf("\n");
			if (is_quit(buff2)) return;
		}
        else if (n==6) {
			printf(" 추출할 파일명 입력   (확장자제외) >>");
			lineinput(buff3,8);printf("\n");
			strchng2(buff3,".","");del_esc3(buff3);
			if (is_quit(buff3)) return;
		}
        else if (n==7&&yesno2(" 이 게시판을 다운받겠습니까? (Y/n)>>",Yes)) {
			printf("\n......화일 추출중.............\n");
			fflush(stdout);
			fp1=fopen(NOW_FILE ,"r");
			fp2=fopen(NOW_FILED,"r");
			if (fp1==NULL||fp2==NULL) return;
			sys_tem("rm tmp/up%d%s -rf",cfg.hostno,no_spc(tty));
			check_dir("tmp/up%d%s",cfg.hostno,no_spc(tty));
			sprintf(TMP_FILE,"tmp/up%d%s/%s.tit",cfg.hostno,no_spc(tty),buff3); // 임시화일
			sprintf(TMP_FILE2,"tmp/up%d%s/%s.txt",cfg.hostno,no_spc(tty),buff3); // 임시화일
			fp3=fopen(TMP_FILE ,"w+");
			fp4=fopen(TMP_FILE2,"w+");
			now_posi=0;
			count=0;
			while (fread(&bbs,sizeof_bbs,1,fp1)){
				fseek(fp2,bbs.posi,SEEK_SET);
				fseek(fp4,now_posi,SEEK_SET);
				if (bbs.del||bbs.num<n1||bbs.num>n2)		continue;
				if (buff1[0]!=0&&!is_same(bbs.id,buff1))	continue;
                if (buff4[0]!=0&&!is_same(bbs.tcode,buff4))    continue;
				if (buff2[0]!=0&&!stristr(bbs.title,buff2)) continue;
				if (bbs.date<date1||bbs.date>date2) 		continue;
				for (i=0;i<bbs.size;i++)
					fputc(fgetc(fp2),fp4);			   //내용에 저장
				bbs.posi=now_posi;
				now_posi+=bbs.size;
				fwrite(&bbs,sizeof_bbs,1,fp3);
				count++;
			}
			fclose(fp1);
			fclose(fp2);
			fclose(fp3);
			fclose(fp4);
			printf("  총 %d개  %d byte 추출 완료\n",count,now_posi);
			fflush(stdout);
			set_statdown(8);	   //메세지 수신방지
			if (count>0) {
				sprintf(buff,"tmp/up%d%s",cfg.hostno,no_spc(tty));
				chdir(buff);
				sprintf(filename,"%s.zip",buff3);
				sys_tem("zip %s %s.tit %s.txt",filename,buff3,buff3);
				chdir(old_dir); 	 //현재의 디렉토리
				sprintf(buff,"tmp/up%d%s/%s",cfg.hostno,no_spc(tty),filename);
				if (is_file(buff)) {
					sz_file(buff);
				}
			}
			chdir(old_dir); 	 //현재의 디렉토리
			set_statdown(OLD_MONI_MODE);	   //메세지 수신방지
			sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty)); // 임시화일
		}
        else if (n==8&&yesno2(" 이 게시판을 다운받겠습니까? (Y/n)>>",Yes)) {
			printf("\n......화일 추출중.............\n");
			fflush(stdout);
			fp1=fopen(NOW_FILE ,"r");
			fp2=fopen(NOW_FILED,"r");
			if (fp1==NULL||fp2==NULL) return;
			sys_tem("rm tmp/up%d%s -rf",cfg.hostno,no_spc(tty));
			check_dir("tmp/up%d%s",cfg.hostno,no_spc(tty));
			sprintf(TMP_FILE,"tmp/up%d%s/%s.cap",cfg.hostno,no_spc(tty),buff3); // 임시화일
			fp3=fopen(TMP_FILE ,"w+");
			now_posi=0;
			count=0;
			while (fread(&bbs,sizeof_bbs,1,fp1)){
				if (bbs.del||bbs.num<n1||bbs.num>n2)		continue;
				if (buff1[0]!=0&&!is_same(bbs.id,buff1))	continue;
                if (buff4[0]!=0&&!is_same(bbs.tcode,buff4))    continue;
				if (buff2[0]!=0&&!stristr(bbs.title,buff2)) continue;
				if (bbs.date<date1||bbs.date>date2) 		continue;
				fprintf(fp3,"__[[아이[[%s\n",bbs.id);
				fprintf(fp3,"__[[이름[[%s\n",bbs.name);
				fprintf(fp3,"__[[날짜[[%s\n",datestr(8,bbs.date));
				fprintf(fp3,"__[[조날[[%s\n",datestr(8,bbs.date2));
				fprintf(fp3,"__[[줄수[[%d\n",bbs.line);
				fprintf(fp3,"__[[조회[[%d\n",bbs.read);
				fprintf(fp3,"__[[다운[[%d\n",bbs.down);
				fprintf(fp3,"__[[파일[[%s\n",bbs.filename);
				fprintf(fp3,"__[[키워[[%s\n",bbs.key);
				fprintf(fp3,"__[[내용[[%s\n",bbs.key);
				ch=0;
				fseek(fp2,bbs.posi,SEEK_SET);
				for (i=0;i<bbs.size;i++) {
					ch=fgetc(fp2);
					if (ch==0||ch==-1) break;
					fputc(ch,fp3);			   //내용에 저장
				}
				if (ch!='\n') fprintf(fp3,"\n");
				count++;
				fprintf(fp3,"_[[완료[[\n");
			}
			fclose(fp1);
			fclose(fp2);
			fclose(fp3);
			fflush(stdout);
			set_statdown(8);	   //메세지 수신방지
			if (count>0) {
				sprintf(buff,"tmp/up%d%s",cfg.hostno,no_spc(tty));
				chdir(buff);
				sprintf(filename,"%s.zip",buff3);
				sys_tem("zip %s %s.tit %s.txt",filename,buff3,buff3);
				chdir(old_dir); 	 //현재의 디렉토리
				sprintf(buff,"tmp/up%d%s/%s",cfg.hostno,no_spc(tty),filename);
				if (is_file(buff)) {
					sz_file(buff);
				}
			}
			chdir(old_dir); 	 //현재의 디렉토리
			set_statdown(OLD_MONI_MODE);	   //메세지 수신방지
			sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty)); // 임시화일
		}
	}
}

