///////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 사이버홈 담당자의 메뉴
///////////////////////////////////////////////////////////////////////////
#include <string.h> /* strrev, strcpy */
#include "bbs.h"
int mode_lo1;
int total;
PF logpf,apf1[132],apf2[132],apf3[132];

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	char id[20];
	char tclub[20];
	int mode;
	if (argc<4) {
		printf("\n사용법  viewlog <mode> <ID> <club_code>\n");
		printf("mode 1:개인용  2:운영진용 4:레벨없는 순위\n");
		return;
	}

	findport(tty); //접속포트 찾기
	set_tmpfile();		 //임시화일지정하기
	read_cfg();
	mode_lo1=No;

	mode=atoi(argv[1]); 				//메뉴번호
	if (mode>=997) {
		rank(mode,argv[2],argv[3]);
		return;
	}
	strcpy(id,argv[2]); 				//아이디
	del_space(id);del_esc(id);
	if (!is_same(id,"root")&&!read_pf(id)) {
		mess("ID %s 검색실패",id);
		return;
	}
	if (is_same(id,"root")) {
		pf.type=19;
	}

	strcpy(tclub,argv[3]);				//아이디
	if (!find_club(tclub)) {
		mess("%s 동호회 검색 실패",tclub);
		return;
	}
	if	(mode==1)  view_log1(1);
	else if (mode==2)  view_log2(1);
	else if (mode==3)  view_log2(2);
	else if (mode==5)  view_log3(1);
	else if (mode==6)  view_log4(1);
        else if (mode==62) view_log4(4);
	else if (mode==7)  view_log4(2);
	else if (mode==8)  view_log4(3);
}


//접속기록보기 개인용
view_log1(int mode)
{
	FILE *fp,*fp2;
	char buff[500];
	char buff1[500];
	char filename[500];
	int i,n,nn,count;
	int year,month,day,job;
	if (guest_mode||pf.type<1) return;
	year=now_year();
	month=now_month();
	day=0;
	job=0;

	while (1) {
		if (year<=100) year+=1900;
		view("frm/log/mylog.scr");
		lineinput(buff,20);check_han(buff);printf("\n");n=atoi(buff);
		if (is_same(buff,".lo1")) mode_lo1=mode_lo1?No:Yes;
		if (is_same(buff,"h")) {view("frm/log/mylog.hlp");continue;}
		if (is_quit(buff)) break;
		if (n<1||n>9) continue;
		if (n==1) {
			year=now_year();month=now_month();day=0;job=23;
		}
		else if (n==2) {
			year=now_year();month=now_month();day=0;job=0;
		}
		else if (n==3) {
			printf("\n   검색 년도  >>");
			lineinput(buff,10);year=atoi(buff);
			if (is_quit(buff)) break;
			if (strlen(buff)==0) year=now_year();
			printf(" ->  %d 년\n",year);
			printf("\n   검색 월    >>");
			lineinput(buff,10);month=atoi(buff);
			if (is_quit(buff)) break;
			if (month<1) month=now_month()%100;
			printf(" ->  %d 월\n",month);
			printf("\n   검색 대상일>>");
			lineinput(buff,10);day=atoi(buff);
			if (is_quit(buff)) break;
			printf(" ->  %d 일\n",day);
			printf("\n   검색 작업  >>");
			lineinput(buff,10);job=atoi(buff);
			if (is_quit(buff)) break;
			printf(" ->  작업번호:%d\n",job);
		}
		else if (n==4) {
			year=now_year();month=now_month();day=0;job=99;
		}
		sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
		printf("\n   %s님. 잠깐만 기다려 주세요.. 검색을 시작합니다... \n",pf.id);
		printf("     기록 파일 : %s  크기:%d byte \n",filename,file_size(filename));
		fflush(stdout);
		if (!is_file(filename)) {mess("%s 기록 화일이 없습니다.",filename);continue;}
		if ((fp=fopen(filename,"r"))==NULL) break;
		if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
		count=0;
		nn=0;
//	6-02 14:26	77 p5	돌멩이	  1 main	 top	  23 접속1 210.99.207.4 STA 0
//	6-02 14:26	77 p1	onex	 12 main	 top	  23 접속2 xhost.co.kr	END 6349

		while (fgets(buff,300,fp)!=NULL) {
			if (strlen(buff)<10||buff[0]==';') continue;
			xstrncpy(buff1,&buff[21],8);del_space(buff1);
			if (!is_same(buff1,pf.id)) continue;
			if (mode_lo1) printf("  BUFF:%s\n",buff);
			nn++;
			if (mode_lo1&&nn%100==99) pressenter();
			if (day>0) {
				xstrncpy(buff1,&buff[3],2);
				if (mode_lo1) printf("  day:%s\n",buff1);
				if (day!=atoi(buff1)) continue;
			}
			if (job>0) {
				xstrncpy(buff1,&buff[51],2);
				i=atoi(buff1);
				if (mode_lo1) printf("  job:%d\n",i);
				if (job==99&&i!=10&&i!=11&&i!=12) continue;
				if (job<99&&i!=job) continue;
			}
			count++;
			if (strlen(buff)>77) {buff[77]=0;strcat(buff,"\n");}
			fprintf(fp2,"%s",buff);    //해당되는 줄을 보여줌
		}
		fclose(fp);
		fclose(fp2);
		if (count<1) mess("해당되는 접속기록이 없습니다.");
		else		 view_text(TMP_FILE);
		unlink(TMP_FILE);
	}
}


//접속기록보기 운영진용
view_log2(int mode)
{
	FILE *fp,*fp2;
	char buff[500],buff1[500],buff2[500],tstr[100];
	char filename[500];
	char a_nid[500][10],a_ntty[500][10],a_nhost[500][30];
	char a_ncode[500][10],a_nclub[500][10];
	char st[30][100];
	int c_nid,c_ntty,c_nhost,c_ncode,c_nclub,re_aload=Yes;
	int i,n,n1,n2,nn,count,pr1;
	float pr2, pr3;
	int it1,it2,it3;
	int year,month,day1,day2,job1,job2,hour1,hour2,type1,type2,hostno1,hostno2;
	char tid[10],tcode[10],tclub[10],thost[30],ttty[30];
	char tfile[30],tid2[10],tip[30];
	PF oldpf;
	oldpf=pf;
	pr1=10;
	pr2=0.0;
	it1=it2=it3=0;
	c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;

	for (i=0;i<100;i++) {
		strcpy(a_nid[i],"");strcpy(a_ntty[i],"");strcpy(a_nhost[i],"");
		strcpy(a_ncode[i],"");strcpy(a_nclub[i],"");
	}
	if (pf.type<15) return;
	year=now_year();
	month=now_month();
	day1=day2=job1=job2=hour1=hour2=type1=type2=hostno1=hostno2=0;
	strcpy(tid,"");strcpy(tcode,"");strcpy(tclub,"");strcpy(thost,"");
	strcpy(ttty,"");strcpy(tstr,"");
	strcpy(tfile,"");strcpy(tid2,"");
	strcpy(tip,"");strcpy(tid2,"");
	strcpy(pf.user1,"");strcpy(pf.user2,"");strcpy(pf.user3,"");
	while (1) {
		if (re_aload) {
			c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;
			re_aload=No;
			for (i=0;i<500;i++) {
				strcpy(a_nid[i],"");strcpy(a_ntty[i],"");strcpy(a_nhost[i],"");
				strcpy(a_ncode[i],"");strcpy(a_nclub[i],"");
			}
			for (n=0;n<5;n++) {
					 if (n==0) sprintf(buff,"%s/f_nid.dat",REC_DIR);
				else if (n==1) sprintf(buff,"%s/f_ntty.dat",REC_DIR);
				else if (n==2) sprintf(buff,"%s/f_nhost.dat",REC_DIR);
				else if (n==3) sprintf(buff,"%s/f_ncode.dat",REC_DIR);
				else if (n==4) sprintf(buff,"%s/f_nclub.dat",REC_DIR);

				i=0;
				if ((fp=fopen(buff,"r"))!=NULL) {
					while (i<500&&fgets(buff,30,fp)) {
						del_enter(buff);del_space(buff);
							 if (n==0) strcpy(a_nid[i],buff);
						else if (n==1) strcpy(a_ntty[i],buff);
						else if (n==2) strcpy(a_nhost[i],buff);
						else if (n==3) strcpy(a_ncode[i],buff);
						else if (n==4) strcpy(a_nclub[i],buff);
						i++;
					}
					fclose(fp);
				}
					 if (n==0) c_nid=i;
				else if (n==1) c_ntty=i;
				else if (n==2) c_nhost=i;
				else if (n==3) c_ncode=i;
				else if (n==4) c_nclub=i;
			}
		}
		del_space(thost);del_space(ttty);del_space(tclub);del_space(tcode);
		del_space(tfile);del_space(tid);del_space(tid2);del_space(tstr);
		if (year<=100) year+=1900;
		cls();
		print_(1,"전체 접속 기록 확인");
		printf("  1. 시스 템번호   : %d - %d\n",hostno1,hostno2);
        printf("  2. 검색 년월일   : %d 년 %d 월  %d 일 - %d 일   %d 시 - %d 시\n",year%100,month,day1,day2,hour1,hour2);
		printf("  3. 검색   ID     : %s\n",tid);
		printf("  4. 등록   ID     : %s\n",tid2);
		printf("  5. 검색   등급   : %d - %d\n",type1,type2);
		printf("  6. 접속 호스트   : %s\n",thost);
		printf("  7. 접속   tty    : %s\n",ttty);
		printf("  8. 검색 동호회   : %s\n",tclub);
		printf("  9. 검색 가기코드 : %s\n",tcode);
		printf(" 10. 검색 작업번호 : %d - %d\n",job1,job2);
		printf(" 11. 검색 문자열   : %s\n",tstr);
		printf(" 12. 검색 파일명   : %s\n",tfile);
		printf(" 13. 과금 금액지정 : %d원/분  %5.1f원/KB\n",pr1,pr2);
		printf(" 14. 지정 HOST IP  : %s\n",tip);
		printf(" 15. 검색관련환경파일지정\n");
		printf(" 91. 전체 기록 검색      \n");
		printf(" 92. 자료실다운검색      \n");
		printf(" 96. 조건 검색  (일반형) \n");
		printf(" 97. 조건 검색  (간략형) \n");
		printf(" 98. 조건 검색  (공백형) \n");
		printf(" 99. 조건 검색  (세부항목)\n");
		printf(" 선택 >>");

		lineinput(buff,20);check_han(buff);printf("\n");n=atoi(buff);
		if (is_same(buff,".lo1")) mode_lo1=mode_lo1?No:Yes;
		if (is_same(buff,"h")) {view("frm/log/syslog.hlp");continue;}
		if (is_quit(buff)) break;
		if (n== 1) {printf(" 호스트번호시작 : ");hostno1=get_num(hostno1,3);printf("\n");
					printf(" 호스트번호끝   : ");hostno2=get_num(hostno2,3);printf("\n");}
		if (n== 2) {printf(" 현재년   YY : ");year =get_num(year ,4);printf("\n");
					printf(" 현재월   MM : ");month=get_num(month,2);printf("\n");
					printf(" 일  시작 DD : ");day1 =get_num(day1 ,2);printf("\n");
					printf(" 일끝     DD : ");day2 =get_num(day2 ,2);printf("\n");
					printf(" 시간시작 HH : ");hour1=get_num(hour1,2);printf("\n");
					printf(" 시간끝   HH : ");hour2=get_num(hour2,2);printf("\n");}
		if (n== 3) {printf(" 검색ID      : ");get_str(tid,8);}
		if (n== 4) {printf(" 등록ID      : ");get_str(tid2 , 8);}
		if (n== 5) {printf(" 등급시작    : ");type1=get_num(type1,2);printf("\n");
					printf(" 등급끝      : ");type2=get_num(type2,2);printf("\n");}
		if (n== 6) {printf(" 호스트명    : ");get_str(thost,20);}
		if (n== 7) {printf(" 접속TTY     : ");get_str(ttty,8);}
		if (n== 8) {printf(" 현재동호회  : ");get_str(tclub,8);}
		if (n== 9) {printf(" 현재가기코드: ");get_str(tcode,8);}
		if (n==10) {printf(" 작업시작번호: ");job1 =get_num(job1 ,2);printf("\n");
					printf(" 작업끝  번호: ");job2 =get_num(job2 ,2);printf("\n");}
		if (n==11) {printf(" 포함문자열  : ");get_str(tstr ,20);}
		if (n==12) {printf(" 검색파일명  : ");get_str(tfile,15);}
		if (n==13) {printf(" 분당과금금액: ");pr1=get_num(pr1, 4);printf("\n");
					printf(" KB당과금금액(XX.X): ");pr2=get_float(pr2,7);printf("\n");}
		if (n==14) {printf(" 지정호스트IP: ");get_str(tip ,25);}
		if (n==15) {
			printf(" 1. f_nid.dat       \n");
			printf(" 2. f_ntty.dat      \n");
			printf(" 3. f_nhost.dat     \n");
			printf(" 4. f_ncode.dat     \n");
			printf(" 5. f_nclub.dat     \n");
			i=numinput();
			if (i==1) xedit2("%s/f_nid.dat",REC_DIR);
			if (i==2) xedit2("%s/f_ntty.dat",REC_DIR);
			if (i==3) xedit2("%s/f_nhost.dat",REC_DIR);
			if (i==4) xedit2("%s/f_ncode.dat",REC_DIR);
			if (i==5) xedit2("%s/f_nclub.dat",REC_DIR);
		}
		if (n==91) {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			if (mode==2) strchng2(filename,"/rec_","/bbs_");
			if (!is_file(filename)) {mess("%s 기록 화일이 없습니다.",filename);continue;}
			view_text(filename);
		}
		else if (n==92) {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			printf("   [%s] 검색을 시작합니다... \n",filename);
			if (!is_file(filename)) {mess("%s 기록 화일이 없습니다.",filename);continue;}
			if ((fp=fopen(filename,"r"))==NULL) break;
			if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
			count=it1=it2=it3=0;
			nn=0;
			fprintf(fp2,"┏━━━━━━━┳━━━━┳━━━━┳━━━━┳━━━━━━┳━━━┳━━━┓\n");    //해당되는 줄을 보여줌
			fprintf(fp2,"┃ 접속 시간  ##┃ 동호회 ┃가기코드┃ 사용자 ┃  파 일 명  ┃크기 K┃ 금액 ┃\n");    //해당되는 줄을 보여줌
			fprintf(fp2,"┣━━━━━━━╋━━━━╋━━━━╋━━━━╋━━━━━━╋━━━╋━━━┫\n");    //해당되는 줄을 보여줌
			while (fgets(buff,200,fp)!=NULL) {
				if (strlen(buff)<40||buff[0]==';') continue;
				nn++;
				if (mode_lo1&&nn%100==99) pressenter();
				if (mode_lo1) printf("  BUFF:%s\n",buff);
				for (i=0;i<30;i++) strcpy(st[i],"");
				sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
				strcpy(buff2,&buff[91]);
				sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
/*
1	   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 동호1 kslab						   STA 0 1
05-26 00:40  77 p1	 시삽	  19 main	  f1		8 다운1 kslab						   P 시삽	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 시삽	  19 main	  f1		6 등록1 kslab						   P 시삽	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 접속1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 접속2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 접속1 kslab		 STA 0
*/
				if (!is_same(st[10],"다운1")||!is_same(st[21],"P")) continue;
				if (strlen(tstr)>0) {
					if (mode_lo1) printf("  tstr:%s\n",buff);
					if (!stristr(buff,tstr)) continue;
				}
				if (strlen(tid)>0) {
					if (mode_lo1) printf("  tid:%s\n",st[5]);
					if (!is_same(tid,st[5])) continue;
				}
				if (day1>0) {
					xstrncpy(buff1,&buff[3],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  tday:%d\n",i);
					if (i<day1||i>day2) continue;
				}
				if (hour1!=0||hour2!=0) {
					xstrncpy(buff1,&buff[6],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  hour:%d\n",i);
					if (i<hour1||i>hour2) continue;
				}
				if (type1!=0||type2!=0) {
					i=atoi(st[6]);
					if (mode_lo1) printf("  type:%d\n",i);
					if (i<type1||i>type2) continue;
				}
				if (strlen(thost)>0) {
					if (mode_lo1) printf("  host:%s\n",st[11]);
					if (!stristr(st[11],thost)) continue;
				}
				if (hostno1!=0||hostno2!=0) {
					i=atoi(st[3]);
					if (mode_lo1) printf("  hostno:%d\n",i);
					if (i<hostno1||i>hostno2) continue;
				}
				if (strlen(ttty)>0) {
					if (mode_lo1) printf("  tty:%s\n",st[4]);
					if (!is_same(st[4],ttty)) continue;
				}
				if (strlen(tclub)>0) {
					if (mode_lo1) printf("  club:%s\n",st[7]);
					if (!is_same(st[7],tclub)) continue;
				}
				if (strlen(tcode)>0) {
					if (mode_lo1) printf("  code:%s\n",st[8]);
					if (!is_same(st[8],tcode)) continue;
				}
				if (job1!=0||job2!=0) {
					i=atoi(st[9]);
					if (mode_lo1) printf("  job:%d\n",i);
					if (i<job1||i>job2) continue;
				}
				if (strlen(tip)>0) {
					if (mode_lo1) printf("  tip:%s\n",st[11]);
					if (!stristr(st[11],tip)) continue;
				}
/*
P 시삽	  autoexec.bat 280 ddd
P 시삽	  bomul136.zip 374677
*/
				if (strlen(tfile)>0) {			 //파일명검색
					i=atoi(st[24]);
					if (mode_lo1) printf("  file:%s %d %s\n",st[23],i,buff2);
					if (!stristr(st[23],tfile)) continue;
				}
				if (strlen(tid2)>0) {  //ID검색
					if (mode_lo1) printf("  id:%s\n",st[2]);
					if (!is_same(st[2],tid2)) continue;
				}
				count++;
				n1=n2=0;

/*
1	   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 동호1 kslab						   STA 0 1
05-26 00:40  77 p1	 시삽	  19 main	  f1		8 다운1 kslab						   P 시삽	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 시삽	  19 main	  f1		6 등록1 kslab						   P 시삽	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 접속1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 접속2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 접속1 kslab		 STA 0
*/


				n1=atoi(st[24])/1024;
				it1+=n1;
				it3+=(n1*pr2)/10;
				sprintf(buff,"┃%s %s%3s┃%-8s┃%-8s┃%-8s┃%-12.12s┃%6d┃%6.1f┃",
							st[1],st[2],st[3],
							st[7],st[8],st[5],st[23],
							n1,(n1*pr2)/10);
				del_enter(buff);
				fprintf(fp2,"%s\n",buff);    //해당되는 줄을 보여줌
			}
			fprintf(fp2,"┗━━━━━━━┻━━━━┻━━━━┻━━━━┻━━━━━━┻━━━┻━━━┛\n");    //해당되는 줄을 보여줌
			fprintf(fp2,"총%4d건  총%dKb  과금:%5.1f원/K  총액:%s원\n",
						count,it1,pr2,n2c(it3));	//해당되는 줄을 보여줌
			fclose(fp);
			fclose(fp2);
			if (count<1) mess("해당되는 접속기록이 없습니다.");
			else		 view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
		else if (n>95) {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			printf("\n   ... 잠깐만 기다려 주세요.. \n");
			printf("   [%s] 검색을 시작합니다... \n",filename);
			if (!is_file(filename)) {mess("%s 기록 화일이 없습니다.",filename);continue;}
			if ((fp=fopen(filename,"r"))==NULL) break;
			if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
			count=it1=it2=it3=0;
			nn=0;
			if (n==97) {
			fprintf(fp2,"┏━━━━━━┳━━━━┳━━━━┳━━━┳━━━━┓\n");    //해당되는 줄을 보여줌
			fprintf(fp2,"┃접속    시간┃가기코드┃ 사용자 ┃ 작업 ┃사용금액┃\n");    //해당되는 줄을 보여줌
			fprintf(fp2,"┣━━━━━━╋━━━━╋━━━━╋━━━╋━━━━┫\n");    //해당되는 줄을 보여줌
			}
			if (n==99) { //05-26 00:31 12 123
			fprintf(fp2,"┏━━━━━━━━━┳━━━━┳━━━━┳━━━┳━━━┳━━┳━━┳━━━┓\n");    //해당되는 줄을 보여줌
			fprintf(fp2,"┃접속   시간 ## tty┃가기코드┃ 사용자 ┃ 작업 ┃ 사용 ┃과금┃요금┃ 금액 ┃\n");    //해당되는 줄을 보여줌
			fprintf(fp2,"┣━━━━━━━━━╋━━━━╋━━━━╋━━━╋━━━╋━━╋━━╋━━━┫\n");    //해당되는 줄을 보여줌
			}
			while (fgets(buff,200,fp)!=NULL) {
				if (strlen(buff)<40||buff[0]==';') continue;
				nn++;
				if (mode_lo1&&nn%100==99) pressenter();
				if (mode_lo1) printf("  BUFF:%s\n",buff);
				for (i=0;i<30;i++) strcpy(st[i],"");
				sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
				strcpy(buff2,&buff[91]);
				sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
/*
1	   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 동호1 kslab						   STA 0 1
05-26 00:40  77 p1	 시삽	  19 main	  f1		8 다운1 kslab						   P 시삽	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 시삽	  19 main	  f1		6 등록1 kslab						   P 시삽	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 접속1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 접속2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 접속1 kslab		 STA 0
*/
				if (strlen(tstr)>0) {
					if (mode_lo1) printf("  tstr:%s\n",buff);
					if (!stristr(buff,tstr)) continue;
				}
				if (strlen(tid)>0) {
					if (mode_lo1) printf("  tid:%s\n",st[5]);
					if (!is_same(tid,st[5])) continue;
				}
				if (day1>0) {
					xstrncpy(buff1,&buff[3],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  tday:%d\n",i);
					if (i<day1||i>day2) continue;
				}
				if (hour1!=0||hour2!=0) {
					xstrncpy(buff1,&buff[6],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  hour:%d\n",i);
					if (i<hour1||i>hour2) continue;
				}
				if (type1!=0||type2!=0) {
					i=atoi(st[6]);
					if (mode_lo1) printf("  type:%d\n",i);
					if (i<type1||i>type2) continue;
				}
				if (strlen(thost)>0) {
					if (mode_lo1) printf("  host:%s\n",st[11]);
					if (!stristr(st[11],thost)) continue;
				}
				if (hostno1!=0||hostno2!=0) {
					i=atoi(st[3]);
					if (mode_lo1) printf("  hostno:%d\n",i);
					if (i<hostno1||i>hostno2) continue;
				}
				if (strlen(ttty)>0) {
					if (mode_lo1) printf("  tty:%s\n",st[4]);
					if (!is_same(st[4],ttty)) continue;
				}
				if (strlen(tclub)>0) {
					if (mode_lo1) printf("  club:%s\n",st[7]);
					if (!is_same(st[7],tclub)) continue;
				}
				if (strlen(tcode)>0) {
					if (mode_lo1) printf("  code:%s\n",st[8]);
					if (!is_same(st[8],tcode)) continue;
				}
				if (job1!=0||job2!=0) {
					i=atoi(st[9]);
					if (mode_lo1) printf("  job:%d\n",i);
					if (i<job1||i>job2) continue;
				}
				if (strlen(tip)>0) {
					if (mode_lo1) printf("  tip:%s\n",st[11]);
					if (!stristr(st[11],tip)) continue;
				}
/*
P 시삽	  autoexec.bat 280 ddd
P 시삽	  bomul136.zip 374677
*/
				if (is_same(st[21],"P")&&strlen(tfile)>0) {           //파일명검색
					i=atoi(st[24]);
					if (mode_lo1) printf("  file:%s %d %s\n",st[23],i,buff2);
					if (!stristr(st[23],tfile)) continue;
				}
				if ((is_same(st[21],"B")||is_same(st[21],"P"))&&strlen(tid2)>0) {  //ID검색
					if (mode_lo1) printf("  id:%s\n",st[2]);
					if (!is_same(st[2],tid2)) continue;
				}
				count++;
				n1=n2=0;
				if (is_same(st[12],"STA")||is_same(st[12],"END")) {
					n1=atoi(st[13]);it1+=n1;
					n2=(n1+30)/60;it2+=n2;
					it3+=(n2*pr1);
				}
				if (n==97) sprintf(buff,"┃%s  %s┃%-8s┃%-8s┃%-6s┃%7d ┃",
							st[1],st[2],st[8],st[5],st[10],n2*pr1);
				if (n==98) sprintf(buff,"%s %s%3s %3s %-8s %-8s %-6s %6d %4d %4d %6d",
							st[1],st[2],st[3],st[4],st[8],st[5],st[10],n1,n2,pr1,n2*pr1);
				if (n==99) sprintf(buff,"┃%s %s%3s %3s┃%-8s┃%-8s┃%-6s┃%6d┃%4d┃%4d┃%6d┃",
							st[1],st[2],st[3],st[4],st[8],st[5],st[10],n1,n2,pr1,n2*pr1);
				del_enter(buff);
				fprintf(fp2,"%s\n",buff);    //해당되는 줄을 보여줌
			}
			if (n==97) fprintf(fp2,"┗━━━━━━┻━━━━┻━━━━┻━━━┻━━━━┛\n");    //해당되는 줄을 보여줌
			if (n==99) fprintf(fp2,"┗━━━━━━━━━┻━━━━┻━━━━┻━━━┻━━━┻━━┻━━┻━━━┛\n");    //해당되는 줄을 보여줌
			fprintf(fp2,"총%4d건  사용:%d초  과금:%d분  총액:%s원\n",
						count,it1,it2,n2c(it3));	//해당되는 줄을 보여줌
			fclose(fp);
			fclose(fp2);
			if (count<1) mess("해당되는 접속기록이 없습니다.");
			else		 view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
	}
	pf=oldpf;
}



//접속기록보기 운영진용
view_log3(int mode)
{
	FILE *fp,*fp2;
	char buff[500],buff1[500],buff2[500],tstr[100];
	char filename[500];
	char a_nid[500][10],a_ntty[500][10],a_nhost[500][30];
	char a_ncode[500][10],a_nclub[500][10];
	char st[30][100];
	int c_nid,c_ntty,c_nhost,c_ncode,c_nclub,re_aload=Yes;
	int i,n,n1,n2,nn,count=0,pr1;
	int it1,it2,it3;
	int year,month,day1,day2,job1,job2,hour1,hour2,type1,type2,hostno1,hostno2;
	char tid[10],tcode[10],tclub[10],thost[30],ttty[30];
	char tfile[30],tid2[10],tip[30];
	PF oldpf;
	oldpf=pf;
	pr1=10;
	it1=it2=it3=0;
	c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;

	for (i=0;i<100;i++) {
		strcpy(a_nid[i],"");strcpy(a_ntty[i],"");strcpy(a_nhost[i],"");
		strcpy(a_ncode[i],"");strcpy(a_nclub[i],"");
	}
	if (pf.type<15) return;
	year=now_year();
	month=now_month();
	day1=day2=job1=job2=hour1=hour2=type1=type2=hostno1=hostno2=0;
	strcpy(tid,"");strcpy(tcode,"");strcpy(tclub,"");strcpy(thost,"");
	strcpy(ttty,"");strcpy(tstr,"");
	strcpy(tfile,"");strcpy(tid2,"");
	strcpy(tip,"");strcpy(tid2,"");
	strcpy(pf.user1,"");strcpy(pf.user2,"");strcpy(pf.user3,"");
	while (1) {
		del_space(thost);del_space(ttty);del_space(tclub);del_space(tcode);
		del_space(tfile);del_space(tid);del_space(tid2);del_space(tstr);
		if (year<=100) year+=1900;
		cls();
		print_(1,"통계작업");
		printf("  1. HOSTNO                 : %d - %d\n",hostno1,hostno2);
        printf("  2. 연도/월 (YYMM)         : %02d/%02d\n",year%100,month);
		printf("  3. 시작일-종결일          : %d - %d\n",day1,day2);
		printf("  4. 검색등급               : %d - %d\n",type1,type2);
		printf("  5. 검색동호회             : %s\n",tclub);
		printf("  6. 검색가기코드           : %s\n",tcode);
		printf("  7. 검색 ID                : %s\n",tid);
		printf("  8. 등록 ID                : %s\n",tid2);
		printf("  9. 포함문자열             : %s\n",tstr);
		printf(" 10. HOST IP                : %s\n",tip );
		printf(" 91. 전체   사용 시간 통계 (접속-종료)  \n");
		printf(" 92. HOSTNO 사용 시간 통계              \n");
		printf(" 93. 일별   사용 시간 통계              \n");
		printf(" 94. 동호회 사용 시간 통계              \n");
		printf(" 95. 게시판 사용 시간 통계              \n");
		printf(" 96. 등록ID 조회/다운 통계              \n");
		n=numselect();
		if (n<= 0) break;
		if (n== 1) {printf(" 호스트번호시작 : ");hostno1=get_num(hostno1,3);printf("\n");
					printf(" 호스트번호끝   : ");hostno2=get_num(hostno2,3);printf("\n");}
		if (n== 2) {printf(" 현재년   YY : ");year =get_num(year ,4);
					printf(" 현재월   MM : ");month=get_num(month,2);}
		if (n== 3) {printf(" 시작일   DD : ");day1 =get_num(day1 ,2);printf("\n");
					printf(" 종결일   DD : ");day2 =get_num(day2 ,2);printf("\n");}
		if (n== 4) {printf(" 등급시작    : ");type1=get_num(type1,2);printf("\n");
					printf(" 등급끝      : ");type2=get_num(type2,2);printf("\n");}
		if (n== 5) {printf(" 검색동호회  : ");get_str(tclub,8);}
		if (n== 6) {printf(" 검색가기코드: ");get_str(tcode,8);}
		if (n== 7) {printf(" 검색 ID     : ");get_str(tid,8);}
		if (n== 8) {printf(" 등록 ID      : ");get_str(tid2 , 8);}
		if (n== 9) {printf(" 포함문자열  : ");get_str(tstr ,20);}
		if (n==10) {printf(" 지정호스트IP: ");get_str(tip ,25);}

		if (n>90) {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			printf("\n   ... 잠깐만 기다려 주세요.. \n");
			printf("   [%s] 검색을 시작합니다... \n",filename);
			if (!is_file(filename)) {mess("%s 기록 화일이 없습니다.",filename);continue;}
			if ((fp=fopen(filename,"r"))==NULL) break;
			if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
			nn=0;
			while (fgets(buff,200,fp)!=NULL) {
				if (strlen(buff)<40||buff[0]==';') continue;
				nn++;
				for (i=0;i<30;i++) strcpy(st[i],"");
				sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
				strcpy(buff2,&buff[91]);
				sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
/*
1     2	     3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 동호1 kslab						   STA 0 1
05-26 00:40  77 p1	 시삽	  19 main	  f1		8 다운1 kslab						   P 시삽	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 시삽	  19 main	  f1		6 등록1 kslab						   P 시삽	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 접속1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 접속2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 접속1 kslab		 STA 0
*/
				if (strlen(tstr)>0) {
					if (mode_lo1) printf("  tstr:%s\n",buff);
					if (!stristr(buff,tstr)) continue;
				}
				if (strlen(tid)>0) {
					if (mode_lo1) printf("  tid:%s\n",st[5]);
					if (!is_same(tid,st[5])) continue;
				}
				if (day1>0) {
					xstrncpy(buff1,&buff[3],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  tday:%d\n",i);
					if (i<day1||i>day2) continue;
				}
				if (hour1!=0||hour2!=0) {
					xstrncpy(buff1,&buff[6],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  hour:%d\n",i);
					if (i<hour1||i>hour2) continue;
				}
				if (type1!=0||type2!=0) {
					i=atoi(st[6]);
					if (mode_lo1) printf("  type:%d\n",i);
					if (i<type1||i>type2) continue;
				}
				if (strlen(thost)>0) {
					if (mode_lo1) printf("  host:%s\n",st[11]);
					if (!stristr(st[11],thost)) continue;
				}
				if (hostno1!=0||hostno2!=0) {
					i=atoi(st[3]);
					if (mode_lo1) printf("  hostno:%d\n",i);
					if (i<hostno1||i>hostno2) continue;
				}
				if (strlen(ttty)>0) {
					if (mode_lo1) printf("  tty:%s\n",st[4]);
					if (!is_same(st[4],ttty)) continue;
				}
				if (strlen(tclub)>0) {
					if (mode_lo1) printf("  club:%s\n",st[7]);
					if (!is_same(st[7],tclub)) continue;
				}
				if (strlen(tcode)>0) {
					if (mode_lo1) printf("  code:%s\n",st[8]);
					if (!is_same(st[8],tcode)) continue;
				}
				if (job1!=0||job2!=0) {
					i=atoi(st[9]);
					if (mode_lo1) printf("  job:%d\n",i);
					if (i<job1||i>job2) continue;
				}
				if (strlen(tip)>0) {
					if (mode_lo1) printf("  tip:%s\n",st[11]);
					if (!stristr(st[11],tip)) continue;
				}
/*
P 시삽	  autoexec.bat 280 ddd
P 시삽	  bomul136.zip 374677
*/
				if (is_same(st[21],"P")&&strlen(tfile)>0) {           //파일명검색
					i=atoi(st[24]);
					if (mode_lo1) printf("  file:%s %d %s\n",st[23],i,buff2);
					if (!stristr(st[23],tfile)) continue;
				}
				if ((is_same(st[21],"B")||is_same(st[21],"P"))&&strlen(tid2)>0) {  //ID검색
					if (mode_lo1) printf("  id:%s\n",st[2]);
					if (!is_same(st[2],tid2)) continue;
				}
				count++;
				n1=n2=0;
				if (is_same(st[12],"STA")||is_same(st[12],"END")) {
					n1=atoi(st[13]);it1+=n1;
					n2=(n1+30)/60;it2+=n2;
					it3+=(n2*pr1);
				}
				if (n==97) sprintf(buff,"┃%s  %s┃%-8s┃%-8s┃%-6s┃%7d ┃",
							st[1],st[2],st[8],st[5],st[10],n2*pr1);
				if (n==98) sprintf(buff,"%s %s%3s %3s %-8s %-8s %-6s %6d %4d %4d %6d",
							st[1],st[2],st[3],st[4],st[8],st[5],st[10],n1,n2,pr1,n2*pr1);
				if (n==99) sprintf(buff,"┃%s %s%3s %3s┃%-8s┃%-8s┃%-6s┃%6d┃%4d┃%4d┃%6d┃",
							st[1],st[2],st[3],st[4],st[8],st[5],st[10],n1,n2,pr1,n2*pr1);
				del_enter(buff);
				fprintf(fp2,"%s\n",buff);    //해당되는 줄을 보여줌
			}
			if (n==97) fprintf(fp2,"┗━━━━━━┻━━━━┻━━━━┻━━━┻━━━━┛\n");    //해당되는 줄을 보여줌
			if (n==99) fprintf(fp2,"┗━━━━━━━━━┻━━━━┻━━━━┻━━━┻━━━┻━━┻━━┻━━━┛\n");    //해당되는 줄을 보여줌
			fprintf(fp2,"총%4d건  사용:%d초  과금:%d분  총액:%d원\n",
						count,it1,it2,it3);    //해당되는 줄을 보여줌
			fclose(fp);
			fclose(fp2);
			if (count<1) mess("해당되는 접속기록이 없습니다.");
			else		 view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
	}
	pf=oldpf;
}


//mode==1:접속 2:다운
view_log4(int mode)
{
	FILE *fp,*fp2;
	char buff[500],buff1[500],buff2[500],tstr[100];
	char filename[500];
	char a_nid[500][10],a_ntty[500][10],a_nhost[500][30];
	char a_ncode[500][10],a_nclub[500][10];
	char st[30][100];
	int c_nid,c_ntty,c_nhost,c_ncode,c_nclub,re_aload=Yes;
	int i,n,n1,n2,count;
	float pr1,pr2,pr3;
	int it1,it2,it3;
	int year,month,day1,day2,job1,job2,hour2,type1,type2,hostno1,hostno2;
	char tid[10],tcode[10],tclub[10],thost[30],ttty[30];
	char tfile[30],tid2[10],tip[30];
	PF oldpf;
	oldpf=pf;
	it1=it2=it3=0;
	c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;

	for (i=0;i<100;i++) {
		strcpy(a_nid[i],"");strcpy(a_ntty[i],"");strcpy(a_nhost[i],"");
		strcpy(a_ncode[i],"");strcpy(a_nclub[i],"");
	}
	if (pf.type<15) return;
	year=now_year();
	month=now_month();
	day1=day2=0;
	sprintf(buff,"%02d%02d",year%100,month);
	printf(" 기록확인년월 (YYMM)>>");
	dosinput(buff,4);printf("\n");
	i=atoi(buff);year=i/100;month=i%100;
	printf(" 기록확인시작일 (1~31)>>");
	day1=numinput();printf("\n");if (day1<1||day1>31) day1=1;
	printf(" 기록확인  끝일 (1~31)>>");
	day2=numinput();printf("\n");if (day2<1||day2>31) day2=31;
	pr1=9.0;    //과금 호스트 번호 30번으로 접속시
	pr3=15.0;    //과금 호스트 번호 50, 51,52 번으로 접속시 과금
	pr2=2.0;
	if (mode==1||mode==3||mode==4) {
		printf(" 014xy 과금 분당 금액 (%5.1f) >>",pr1);pr1=get_float(pr1,6);printf("\n");
	}
	if (mode==1||mode==3||mode==4) {
		printf(" 일반전화 분당 금액 (%5.1f)>>",pr3);pr3=get_float(pr3,6);printf("\n");
	}
	else if (mode==2){
		printf(" KB당   금액 (%5.1f)>>",pr2);pr2=get_float(pr2,6);printf("\n");
	}

	sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
	printf("     기록 파일 : %s  크기:%d byte \n",filename,file_size(filename));
	if (!is_file(filename)) {mess("%s 기록 화일이 없습니다.",filename);return;}
	if ((fp=fopen(filename,"r"))==NULL) return;
	if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
	count=it1=it2=it3=0;
	if (mode==1) {
		fprintf(fp2,"┏━━━━━━━┳━━━━┳━━┳━━━━━━━━━━━━┳━━━┳━━━━┓\n");    //해당되는 줄을 보여줌
		fprintf(fp2,"┃ 접속 시간  ##┃ 사용자 ┃TTY ┃         호스트         ┃ 시간 ┃  금액  ┃\n");    //해당되는 줄을 보여줌
		fprintf(fp2,"┣━━━━━━━╋━━━━╋━━╋━━━━━━━━━━━━╋━━━╋━━━━┫\n");    //해당되는 줄을 보여줌
	}
	else if (mode==2) {
		fprintf(fp2,"┏━━━━━━━┳━━━━┳━━━━┳━━━━┳━━━━━━┳━━━┳━━━┓\n");    //해당되는 줄을 보여줌
		fprintf(fp2,"┃ 접속 시간  ##┃ 동호회 ┃가기코드┃ 사용자 ┃  파 일 명  ┃크기 K┃ 금액 ┃\n");    //해당되는 줄을 보여줌
		fprintf(fp2,"┣━━━━━━━╋━━━━╋━━━━╋━━━━╋━━━━━━╋━━━╋━━━┫\n");    //해당되는 줄을 보여줌
	}
	else if (mode==3) {
		fprintf(fp2,"┏━━━━━━━┳━━━━┳━━┳━━━━┳━━━┳━━━━┓\n");    //해당되는 줄을 보여줌
		fprintf(fp2,"┃ 접속 시간  ##┃ 사용자 ┃TTY ┃ 동호회 ┃ 시간 ┃  금액  ┃\n");    //해당되는 줄을 보여줌
		fprintf(fp2,"┣━━━━━━━╋━━━━╋━━╋━━━━╋━━━╋━━━━┫\n");    //해당되는 줄을 보여줌
	}
        else if (mode==4) {
        fprintf(fp2,"");    //해당되는 줄을 보여줌
		fprintf(fp2," \n");    //해당되는 줄을 보여줌
	}
	while (fgets(buff,200,fp)!=NULL) {
		if (strlen(buff)<40||buff[0]==';') continue;
		for (i=0;i<30;i++) strcpy(st[i],"");
		sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
		strcpy(buff2,&buff[91]);
		sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
		if (strlen(st[1])!=5||strlen(st[2])!=5||strlen(st[5])<3) continue;
/*
1   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 동호1 kslab						   STA 0 1
05-26 00:40  77 p1	 시삽	  19 main	  f1		8 다운1 kslab						   P 시삽	 autoexec.bat 280 ddd
01-04 20:03   1 /5	 dunk1125  2 main	  hostbbs  23 접속2 203.232.36.225				   END 109
01-04 20:06   1 /4	 뭉게구름 10 main	  my79	   23 접속2 203.234.157.76				   END 664
*/
		xstrncpy(buff1,&buff[3],2);
		i=atoi(buff1);
		if (i<day1||i>day2) continue;
		xstrncpy(buff1,&buff[0],2);
		i=atoi(buff1);
		if (i!=month) continue;
		if (mode==1) {
			if (!is_same(st[10],"접속2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			sprintf(buff,"┃%5.5s %5.5s%3.3s┃%-8.8s┃%-4.8s┃%-24.24s┃%6d┃%8.1f┃",
						st[1],st[2],st[3],st[5],st[4],st[11],
						n1,(n1/60)*pr1);
		}
		else if (mode==2) {
			if (!is_same(st[10],"다운1")||!is_same(st[21],"P")) continue;
			n1=atoi(st[24])/1024;
			if (n1<0||n1>500*1024*1024) continue;
			it3+=(n1*pr2)/10;
			count++;
			it1+=n1/1024;
			sprintf(buff,"┃%5.5s %5.5s%3.3s┃%-8.8s┃%-8.8s┃%-8.8s┃%-12.12s┃%6d┃%6.1f┃",
						st[1],st[2],st[3],
						st[7],st[8],st[5],st[23],
						n1,(n1*pr2)/10);
		}
		else if (mode==3) {
			if (!is_same(st[10],"동호2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			sprintf(buff,"┃%5.5s %5.5s%3.3s┃%-8.8s┃%-4.4s┃%-8.8s┃%6d┃%8.1f┃",
						st[1],st[2],st[3],
						st[5],st[4],st[7],n1,(n1/60)*pr1);
		}
		else if ((mode==4)&&is_same(st[11],"168.126.145.30")) {
			if (!is_same(st[10],"접속2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			st[1][2]=';';
			st[2][2]=';';
			sprintf(buff,"%-5.5s;%-5.5s;%-4.4s;%-8.8s;%-15.15s;%-6d;%-8.1f ",
						st[1],st[2],st[3],
						st[5],st[11],n1,(n1/60)*pr1);
		}
		else if ((mode==4)&&(is_same(st[11],"168.126.145.50")||is_same(st[11],"168.126.145.51")||is_same(st[11],"168.126.145.52")) ) {
			if (!is_same(st[10],"접속2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			st[1][2]=';';
			st[2][2]=';';
			sprintf(buff,"%-5.5s;%-5.5s;%-4.4s;%-8.8s;%-15.15s;%-6d;%-8.1f ",
						st[1],st[2],st[3],
						st[5],st[11],n1,(n1/60)*pr3);
		}
		else if ((mode==4)&&(!is_same(st[11],"168.126.145.50"))&&(!is_same(st[11],"168.126.145.51"))&&
			(!is_same(st[11],"168.126.145.52"))&&(!is_same(st[11],"168.126.145.30")) ) {
			if (!is_same(st[10],"접속2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			st[1][2]=';';
			st[2][2]=';';
			sprintf(buff,"%-5.5s;%-5.5s;%-4.4s;%-8.8s;%-15.15s;%-6d;%-8.1f ",
						st[1],st[2],st[3],
						st[5],st[11],n1,0.0);
		}
		
		
		del_enter(buff);
		fprintf(fp2,"%s\n",buff);    //해당되는 줄을 보여줌
	}
	if (mode==1) {
		fprintf(fp2,"┗━━━━━━━┻━━━━┻━━┻━━━━━━━━━━━━┻━━━┻━━━━┛\n");    //해당되는 줄을 보여줌
		fprintf(fp2,"총%4d건  총 %d시간 %d분   과금:%5.1f원/분  총액:%s원\n",
					count,it1/60,it1%60,pr2,n2c(it3));	  //해당되는 줄을 보여줌
	}
	else if (mode==2) {
		fprintf(fp2,"┗━━━━━━━┻━━━━┻━━━━┻━━━━┻━━━━━━┻━━━┻━━━┛\n");    //해당되는 줄을 보여줌
		fprintf(fp2,"총%4d건  총 %d MB  과금:%5.1f원/K  총액:%s원\n",
					count,it1/1024,pr2,n2c(it3));	 //해당되는 줄을 보여줌
	}
	else if (mode==3) {
		fprintf(fp2,"┗━━━━━━━┻━━━━┻━━┻━━━━┻━━━┻━━━━┛\n");    //해당되는 줄을 보여줌
		fprintf(fp2,"총%4d건  총 %d MB  과금:%5.1f원/K  총액:%s원\n",
					count,it1/1024,pr2,n2c(it3));	 //해당되는 줄을 보여줌
	}
	else if (mode==4) {
		fprintf(fp2,"");    //해당되는 줄을 보여줌
        fprintf(fp2,"\n");  //해당되는 줄을 보여줌
	}
	fclose(fp);
	fclose(fp2);
	if (count<1) mess("해당되는 접속기록이 없습니다.");
	else		 view_text(TMP_FILE);
	unlink(TMP_FILE);
	pf=oldpf;
}


// viewlog.c에서 호출되는 부분
//rank mode1 mode2 mode3
//mode1:1 rank	2:접속
//mode2:1 일반	2:운영진
//mode3:1 금월	2:전체
rank(int mode,char *cmd1,char *cmd2)
{
	char buff[512];
	int i,n,n1,n2;
	int mode2=0;


	findport(tty); //접속포트 찾기
	read_cfg();
	set_tmpfile();		 //임시화일지정하기
	set_nowmenu();		 //데이타화일지정하기
	read_pf(cmd1);
	logpf=pf;
	mode2=0;
	if (is_same(cmd2,"rk1")) mode2=1;
	if (is_same(cmd2,"rk2")) mode2=2;
	if (is_same(cmd2,"rk3")) mode2=3;
	if (is_same(cmd2,"rk4")) mode2=4;
	if (mode2>0) {
		n=mode2;
		goto _direct;
	}

	if (mode==997) {
		while (1) {
			view("frm/rank/rank.sel");
			lineinput(buff,3);check_han(buff);
			if (is_same(buff,"q")||is_same(buff,"p")||
				is_same(buff,"qq")||is_same(buff,"pp")||
				is_same(buff,"x")||is_same(buff,"xx")) return;
			n=atoi(buff);
			clear_apf();
			if (logpf.type<14&&(n==3||n==4)) n=1;
				 if (n==1) rescan5(TMP_FILE2,1,1);		//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
			else if (n==2) rescan5(TMP_FILE2,1,2);		//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
			cls();
			view("frm/rank/rank%d.scr",n);
			view_text_line(TMP_FILE2,15);
			if (mode2>0) break;
		}
	}
	if (mode==998) {
		while (1) {
			strcpy(g_buff1,"");
			strcpy(g_buff2,"");
			if (logpf.type>=14) {
				strcpy(g_buff1,"3. 운영진 금월 순위");
				strcpy(g_buff2,"4. 운영진 전체 순위");
			}
			view("frm/rank/rank.sel");
			lineinput(buff,3);check_han(buff);
			if (is_same(buff,"q")||is_same(buff,"p")||
				is_same(buff,"qq")||is_same(buff,"pp")||
				is_same(buff,"x")||is_same(buff,"xx")) return;
			n=atoi(buff);
			_direct:
			clear_apf();
			if (logpf.type<14&&(n==3||n==4)) n=1;
				 if (n==1) rescan1(TMP_FILE2,1,1);		//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
			else if (n==2) rescan1(TMP_FILE2,1,2);		//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
			else if (n==3) rescan1(TMP_FILE2,2,1);		//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
			else if (n==4) rescan1(TMP_FILE2,2,2);		//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
			else if (n==5) rescan3(TMP_FILE2,1,1);		//일반인 쓰기 순위
			else if (n==6) rescan3(TMP_FILE2,2,1);		//전체	 쓰기 순위
			else if (n==7) rescan4(TMP_FILE2,1,1);		//일반인 쓰기 순위
			else if (n==8) rescan4(TMP_FILE2,2,1);		//전체	 쓰기 순위
			cls();
			view("frm/rank/rank%d.scr",n);
			view_text_line(TMP_FILE2,15);
			if (mode2>0) break;
		}
	}
	else if (mode==999) {
		clear_apf();
		rescan2(TMP_FILE2);
		cls();
		view("frm/rank/ranklog.scr");
		view_text_line(TMP_FILE2,15);
	}
	unlink(TMP_FILE2);
}



//마지막 자료를 찾아 자료의 수를 계산함
//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
rescan1(char *filename,int mode1,int mode2) {
	FILE *fp;
	int  i,n1,n2,n3;

	printf("\n 잠깐만 기다려 주십시요.. 지금 자료정리 중입니다.\n");

	fp = fopen(ID_FILE, "r");
	total=0;
	rewind(fp);
	while(fread(&pf,sizeof_pf,1,fp)) {		   //아이디 저장
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		if (mode1==1&&pf.type>=14) continue;
		total++;
		for (i=120;i>=0;i--) {
			if (pf.level<apf1[i].level) break;
			apf1[i+1]=apf1[i];
			apf1[i]=pf;
		}
		if (mode2==1) {
			for (i=120;i>=0;i--) {
				if (pf.month_log==0||pf.month_log<apf2[i].month_log) break;
				apf2[i+1]=apf2[i];
				apf2[i]=pf;
			}
			for (i=120;i>=0;i--) {
				if (pf.month_time==0||pf.month_time<apf3[i].month_time) break;
				apf3[i+1]=apf3[i];
				apf3[i]=pf;
			}
		}
		else {
			for (i=120;i>=0;i--) {
				if (pf.all_log==0||pf.all_log<apf2[i].all_log) break;
				apf2[i+1]=apf2[i];
				apf2[i]=pf;
			}
			for (i=120;i>=0;i--) {
				if (pf.all_time==0||pf.all_time<apf3[i].all_time) break;
				apf3[i+1]=apf3[i];
				apf3[i]=pf;
			}
		}
	}

	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	fprintf(fp,"[2;4H총회원:%5d명",total);
	for (i=0;i<120;i++) {
		if (mode2==1) {
			n2=apf2[i].month_log;
			n3=apf3[i].month_time;
		}
		else {
			n2=apf2[i].all_log;
			n3=apf3[i].all_time;
		}
		fprintf(fp,"[%02d;5H%3d.  %-8s %6d   | %3d. %-8s %6d   | %3d. %-8s%4d:%02d\n",
		(i%15)+6,
		i+1,apf1[i].id, apf1[i].level,
		i+1,apf2[i].id, n2,
		i+1,apf3[i].id, n3/3600,(n3%3600)/60);
	}
	fprintf(fp,"[21;01H");
	fclose(fp);
}


rescan2(char *filename) {
	FILE *fp;
	int  i;
	char str[100];

	printf("\n\n\n *** 잠깐만 기다려 주세요 지금 자료 정리중입니다. *** \n\n\n");
	fp = fopen(ID_FILE, "r");
	total=0;				  //회원 총 수 계산
	while(fread(&pf,sizeof_pf,1,fp)) {
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		total++;
		for (i=120;i>=0;i--) {
			if (pf.login>pf.logout) pf.logout=pf.login;
			if (pf.logout<apf1[i].logout) break;
			apf1[i+1]=apf1[i];
			apf1[i]=pf;
		}
	}
	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	for (i=0;i<120;i++) {
		if (strlen(apf1[i].id)<3) continue;
		fprintf(fp,"[%02d;5H%3d.  %-8s(%-8s) %2d    ",
				(i%15)+6,i+1,apf1[i].id, apf1[i].name,apf1[i].type);
		fprintf(fp,"%s ∼ ",datestr(2,apf1[i].login));
		fprintf(fp,"%s ",datestr(3,apf1[i].logout));
		fprintf(fp,"%4d /%4d\n",apf1[i].month_log,apf1[i].all_log);

	}
	fprintf(fp,"[21;01H");
	fclose(fp);

}

//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
rescan3(char *filename,int mode1,int mode2) {
	FILE *fp;
	int  i,n1,n2,n3;

	printf("\n 잠깐만 기다려 주십시요.. 지금 자료정리 중입니다.\n");

	fp = fopen(ID_FILE, "r");
	total=0;
	rewind(fp);
	while(fread(&pf,sizeof_pf,1,fp)) {		   //아이디 저장
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		if (mode1==1&&pf.type>=14) continue;
		total++;
		for (i=120;i>=0;i--) {
			if (pf.countw==0||pf.countw<apf1[i].countw) break;
			apf1[i+1]=apf1[i];
			apf1[i]=pf;
		}
		for (i=120;i>=0;i--) {
			if (pf.countu==0||pf.countu<apf2[i].countu) break;
			apf2[i+1]=apf2[i];
			apf2[i]=pf;
		}
		for (i=120;i>=0;i--) {
			if (pf.countd==0||pf.countd<apf3[i].countd) break;
			apf3[i+1]=apf3[i];
			apf3[i]=pf;
		}
	}

	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	fprintf(fp,"[2;4H총회원:%5d명",total);
	for (i=0;i<120;i++) {
		fprintf(fp,"[%02d;5H%3d.  %-8s %6d   | %3d. %-8s %6d   | %3d. %-8s %6d\n",
		(i%15)+6,
		i+1,apf1[i].id, apf1[i].countw,
		i+1,apf2[i].id, apf2[i].countu,
		i+1,apf3[i].id, apf3[i].countd);
	}
	fprintf(fp,"[21;01H");
	fclose(fp);
}


//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
rescan4(char *filename,int mode1,int mode2) {
	FILE *fp;
	int  i,n1,n2,n3;

	printf("\n 잠깐만 기다려 주십시요.. 지금 자료정리 중입니다.\n");

	fp = fopen(ID_FILE, "r");
	total=0;
	rewind(fp);
	while(fread(&pf,sizeof_pf,1,fp)) {		   //아이디 저장
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		if (mode1==1&&pf.type>=14) continue;
		total++;
		for (i=120;i>=0;i--) {
			if (pf.warn==0||pf.warn<apf1[i].warn) break;
			apf1[i+1]=apf1[i];
			apf1[i]=pf;
		}
		for (i=120;i>=0;i--) {
			if (pf.point==0||pf.point<apf2[i].point) break;
			apf2[i+1]=apf2[i];
			apf2[i]=pf;
		}
		for (i=120;i>=0;i--) {
			if (pf.good==0||pf.good<apf3[i].good) break;
			apf3[i+1]=apf3[i];
			apf3[i]=pf;
		}
	}

	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	fprintf(fp,"[2;4H총회원:%5d명",total);
	for (i=0;i<120;i++) {
		fprintf(fp,"[%02d;5H%3d.  %-8s %6d   | %3d. %-8s %6d   | %3d. %-8s %6d\n",
		(i%15)+6,
		i+1,apf1[i].id, apf1[i].warn,
		i+1,apf2[i].id, apf2[i].point,
		i+1,apf3[i].id, apf3[i].good);
	}
	fprintf(fp,"[21;01H");
	fclose(fp);
}

//마지막 자료를 찾아 자료의 수를 계산함
//mode1:1:일반 2:운영진   mode2:1:월별 2:전체
rescan5(char *filename,int mode1,int mode2) {
	FILE *fp;
	int  i,n1,n2,n3;

	printf("\n 잠깐만 기다려 주십시요.. 지금 자료정리 중입니다.\n");

	fp = fopen(ID_FILE, "r");
	total=0;
	rewind(fp);
	while(fread(&pf,sizeof_pf,1,fp)) {		   //아이디 저장
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		if (mode1==1&&pf.type>=14) continue;
		total++;
		if (mode2==1) {
			for (i=120;i>=0;i--) {
				if (pf.month_log==0||pf.month_log<apf2[i].month_log) break;
				apf2[i+1]=apf2[i];
				apf2[i]=pf;
			}
			for (i=120;i>=0;i--) {
				if (pf.month_time==0||pf.month_time<apf3[i].month_time) break;
				apf3[i+1]=apf3[i];
				apf3[i]=pf;
			}
		}
		else {
			for (i=120;i>=0;i--) {
				if (pf.all_log==0||pf.all_log<apf2[i].all_log) break;
				apf2[i+1]=apf2[i];
				apf2[i]=pf;
			}
			for (i=120;i>=0;i--) {
				if (pf.all_time==0||pf.all_time<apf3[i].all_time) break;
				apf3[i+1]=apf3[i];
				apf3[i]=pf;
			}
		}
	}

	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	fprintf(fp,"[2;4H총회원:%5d명",total);
	for (i=0;i<120;i++) {
		if (mode2==1) {
			n2=apf2[i].month_log;
			n3=apf3[i].month_time;
		}
		else {
			n2=apf2[i].all_log;
			n3=apf3[i].all_time;
		}
		fprintf(fp,"[%02d;5H    %3d. %-8s %6d        |       %3d. %-8s%4d:%02d\n",
		(i%15)+6,
		i+1,apf2[i].id, n2,
		i+1,apf3[i].id, n3/3600,(n3%3600)/60);
	}
	fprintf(fp,"[21;01H");
	fclose(fp);
}




clear_apf()
{
	int i;
	PF apf;
	strcpy(apf.id,"");
	apf.level=0;
	apf.month_log=0;
	apf.all_log=0;
	apf.month_time=0;
	apf.all_time=0;
	apf.type=0;
	apf.login=0;
	apf.logout=0;
	apf.countu=0;
	apf.countd=0;
	apf.countw=0;
	apf.good=0;
	apf.warn=0;
	apf.point=0;
	for (i=0;i<122;i++) {
		apf1[i]=apf;
		apf2[i]=apf;
		apf3[i]=apf;
	}
}


