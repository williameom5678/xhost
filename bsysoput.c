////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 게시판 담당자의 메뉴
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include <unistd.h>    //디렉토리 화일명 변환함수때문에 필요
#include <dirent.h>    //디렉토리 화일명 변환함수때문에 필요

int asearch0[31];
int asearch1[31];
int asearch2[31];
int asearch3[31];
int asearch4[31];
int n;
int b_tota1;
int b_tota2;
int b_tota3;
int b_tota4;
int b_tota5;
int b_tota6;
int b_tota7;
int b_tota8;
int b_tota9;
int b_tota10;
int b_size1;
int b_size2;

disp_status()
{
	FILE *fp;
	int i,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11;
	int s1,s2,s3,s4,s5,s6,d1,d2,d3,d4,d5,d6;
	n1=n2=n3=n4=n5=n6=d1=d2=d3=d4=d5=d6=s1=s2=s3=s4=s5=s6=0;
	if ((fp=fopen(NOW_FILE,"r"))!=NULL) {
		while(fread(&bbs,sizeof_bbs,1,fp)) {
			i=time_day(bbs.date);
			if (i<7) n3++;
			else if (i<30) n4++;
			if (bbs.del) {
				n2++;
				d1+=bbs.size;
				d2+=bbs.filesize;
				d3+=bbs.read;
				d4+=bbs.down;
				if (bbs.filesize>0) d5++;
			}
			else		 {
				n1++;
				s1+=bbs.size;
				s2+=bbs.filesize;
				s3+=bbs.read;
				s4+=bbs.down;
				if (bbs.filesize>0) s5++;
			}
		}
		fclose(fp);
	}
	cls();
	printf("  ========= 게시판 상태 =========\n");
	printf(" 작업자 ID   : %s  (등급:%d)\n",pf.id,pf.type);
	printf(" 동호회      : %s  (%s)  \n",club_go.code,club_go.dir);
	printf(" 가기코드    : %s  (%s 타입:%c WT:%s)  \n",go.code,go.name,go.type,go.wtimes);
	printf(" 메뉴디렉토리: %s        \n",go.dir);
	printf(" 게시판제목  : %s  %10d byte\n",NOW_FILE ,file_size(NOW_FILE ));
	printf(" 게시판내용  : %s  %10d byte\n",NOW_FILED,file_size(NOW_FILED));
	printf(" 게시판총수  : %8d  (삭제:%8d)\n",n1,n2);
	printf(" 최근 7일수  : %8d  (최근30일:%d)\n",n3,n4);
	printf(" 내용총크기  : %8d  (삭제:%8d)\n",s1,d1);
	printf(" 파일총크기  : %8d  (삭제:%8d)\n",s2,d2);
	printf(" 읽기총수    : %8d  (삭제:%8d)\n",s3,d3);
	printf(" 다운총수    : %8d  (삭제:%8d)\n",s4,d4);
	printf(" 파일포함수  : %8d  (삭제:%8d)\n",s5,d5);
	printf(" 분당요금    : %8d  \n",bcfg.bill1);
	printf(" KB당요금    : %8d  \n",bcfg.bill2);
	printf("  ========= 기록   상태 =========\n");
	printf(" 전체게시물수: %8d (오늘게시물)\n",brec.allnum,brec.newnum);
	printf(" 전체사용시간: %8d (이번달:%8d  지난달:8d)\n",brec.time1    ,brec.time2    ,brec.time3    );
	printf(" 사용요금시간: %8d (이번달:%8d  지난달:8d)\n",brec.timebill1,brec.timebill2,brec.timebill3);
	printf(" 전체사용SIZE: %8d (이번달:%8d  지난달:8d)\n",brec.size1    ,brec.size2    ,brec.size3    );
	printf(" 사용요금SIZE: %8d (이번달:%8d  지난달:8d)\n",brec.sizebill1,brec.sizebill2,brec.sizebill3);
	pressenter();
}


reset_menuind() 	//메뉴SUB TAG 재정리
{
	FILE *fp,*fp2;
	char filename[120];
	int n,deep;
	GO tgo;
	if (!club_mode&&pf.type>0&&pf.localmenu>0&&bit(cfg.gomode,11))
		sys_tem("rm -f menustru/%d/*.ind",pf.localmenu);
	else
		sys_tem("rm -f %s/menustru/*.ind",club_go.dir);

	if ((fp=fopen(NOW_MENU,"r"))==NULL) return;
	deep=0;
	n=-1;
	while(1) {
		n++;
		fseek(fp,n*sizeof_go,SEEK_SET);
		if (!fread(&tgo,sizeof_go,1,fp)) break;
		if (tgo.type=='m'&&strlen(tgo.code)>0) {
			if (!club_mode&&pf.type>0&&pf.localmenu>0&&bit(cfg.gomode,11))
				sprintf(filename,"menustru/%d/%s.ind",pf.localmenu,tgo.code);
			else
				sprintf(filename,"%s/menustru/%s.ind",club_go.dir,tgo.code);

			if (!fread(&tgo,sizeof_go,1,fp)) continue;
			if ((fp2=fopen(filename,"w"))!=NULL) {
				deep=tgo.deep;
				if (deep==tgo.deep&&tgo.type>' '&&strlen(tgo.code)>0)
					fwrite(&tgo,sizeof_go,1,fp2);
				while(fread(&tgo,sizeof_go,1,fp)&&deep<=tgo.deep) {
					if (deep==tgo.deep&&tgo.type>' '&&strlen(tgo.code)>0)
						fwrite(&tgo,sizeof_go,1,fp2);
				}
				fclose(fp2);
			}
		}
	}
}



char sdate1[10],sdate2[10],snum[10];
char sid1[9],sid2[9],stitle[61];


//해당 게시물을 읽은 경우 읽은 횟수에 따른 IP 설정을 한다.
view_iprec()
{
	FILE *fp;
	char buff[200];
	char buff1[200];
	char buff2[200];
	char buff3[200];
	char buff4[200];
	char buff5[200];
	char filename[300];
	int i,n;
	int smonth,count;
	smonth=count=0;
	smonth=now_month();
	strcpy(sdate1,"");strcpy(sdate2,"");strcpy(stitle,"");
	strcpy(snum,"");strcpy(sid1,"");strcpy(sid2,"");
	while (1) {
		cls();
		printfL2();
		printf(" 1. 검색월    MM       :%d\n",smonth);
		printf(" 2. 시작날짜  YY-MM-DD :%s\n",sdate1);
		printf(" 3. 끝  날짜  YY-MM-DD :%s\n",sdate2);
		printf(" 4. 등록  ID           :%s\n",sid1);
		printf(" 5. 검색  ID           :%s\n",sid2);
		printf(" 6. 검색번호           :%s\n",snum);
		printf(" 7. 검색제목           :%s\n",stitle);
		printf("98. 일별 통계\n");
		printf("99. 검색 시작\n");
		printf(" 선택 >>");
		lineinput(buff,10); check_han(buff);printf("\n");n=atoi(buff);
		xy(25,n+1);
		if (is_same(buff,"q")||is_same(buff,"p")||is_same(buff,"x")||is_same(buff,"m")) break;
		if (n==1) smonth=get_num(smonth,2);
		if (n==2) get_str(sdate1,8);
		if (n==3) get_str(sdate2,8);
		if (n==4) get_str(sid1,8);
		if (n==5) get_str(sid2,8);
		if (n==6) get_str(snum,6);
		if (n==7) get_str(stitle,60);
		if (n==98) view_daybbs();
		if (n==99) {
			sprintf(filename,"%s/bbsdata/menurec/%s.%d",club_go.dir,go.code,smonth);
			if ((fp=fopen(filename,"r"))==NULL) {
				printf("\n");
				mess("IP 기록 %s 없음",filename);
				continue;
			}
			count=0;
			while (fgets(buff,200,fp)!=NULL) {
				if (strlen(buff)<10||buff[0]=='#') continue;
				xstrncpy(buff1,&buff[0],8);del_space(buff2);
				xstrncpy(buff2,&buff[15],8);del_space(buff2);
				xstrncpy(buff3,&buff[24],8);del_space(buff3);
				xstrncpy(buff4,&buff[32],6);del_space(buff4);
				xstrncpy(buff5,&buff[50],60);all_trim(buff5);
//				  printf("%s %s %s %s %s",buff1,buff2,buff3,buff4,buff5);pressenter();
					 if (strlen(sdate1)>0&&strcmp(sdate1,buff1)<0) ;
				else if (strlen(sdate2)>0&&strcmp(sdate2,buff1)>0) ;
				else if (strlen(sid1  )>0&&!is_same(sid1,buff2)) ;
				else if (strlen(sid2  )>0&&!is_same(sid2,buff3)) ;
				else if (atoi(snum)>0	&&atoi(buff4)!=atoi(snum)) ;
				else if (strlen(stitle)>0&&!stristr(buff5,stitle)) ;
				else {
					printf("%5d. %s",count+1,buff);
					count++;
				}
			}
			fclose(fp);
			if (count<1) printf(" ## 해당자료가 없습니다.\n");
			pressenter();
		}
	}
}


view_daybbs()		 //일별 가입현황
{
	FILE *fp,*fp2;
	time_t indate,date1,date2;
	int inday;
	int i,n,count1,count2,down1,read1;
	fp=fopen(NOW_FILE,"r");
	i=0;
	printf("\n");
	indate=0;
	inday=count1=count2=down1=read1=0;
	fp2=fopen(TMP_FILE,"w");
	date1=str2date(sdate1);
	date2=str2date(sdate2);
	while (fread(&bbs,sizeof_bbs,1,fp)) {
			 if (strlen(sdate1)>0&&bbs.date<date1) ;
		else if (strlen(sdate2)>0&&bbs.date>date2) ;
		else if (strlen(sid1  )>0&&!is_same(sid1,bbs.id)) ;
		else if (strlen(stitle)>0&&!stristr(bbs.title,stitle)) ;
		else {
			n=time_num(bbs.date);
			if (inday>0&&inday!=n) {
				fprintf(fp2,"%s %6d명 쓰기   조회:%5d   다운:%5d  누적%6d명\n",datestr(35,indate),count1,read1,down1,count2);
				count1=0;
				down1=read1=0;
			}
			indate=bbs.date;
			inday=n;
			count1++;
			count2++;
			down1+=bbs.down;
			read1+=bbs.read;
		}
	}
	fclose(fp);
	fclose(fp2);
	view_text(TMP_FILE);
}



select_bform()
{
	int i,nn,now=1;
	char buff[100];
	char inv1[150],inv2[150];
	char char3		  ;

	bbs.gd=' ';
	strcpy(bbs.title,"이것은 제목입니다.901234567890123456789012345678901234567890");
	strcpy(bbs.id	,"긴아이디");
	strcpy(bbs.name ,"이름네자");
	strcpy(bbs.key	,"키워드임");
	strcpy(bbs.filename,"파일명임.ext");
	strcpy(bbs.tcode,"T-코드임");
	time(&bbs.date);time(&bbs.date2);
	bbs.down=bbs.read=9999;
	bbs.line=999;bbs.filesize=999999;bbs.size=9999;
	now=bcfg.bform;
	if (now==0)  now=1;
	while(1) {
		bbs.num=99990;
		if (now<1)	now=200;
		if (now>200) now=1;
		cls();
		view("frm/screen/%d.top",now);
		for (i=0;i<5;i++) {
			bbs.num++;
			if (i==1) bbs.gd='!';
			if (i==2) bbs.gd='@';
			if (i==3) bbs.gd='%';
			if (i==4) bbs.gd='&';
			char3=gd_chr(1);	 //추천/조회/다운지정
			strcpy(inv1,"");strcpy(inv2,"");
			if (bbs.gd!=' ') strcpy(buff,gd_str(bbs.gd));      //추천별색상
			else			 strcpy(buff,gd_str(char3));	   //추천별색상
			strcpy(g_buff7,buff);
			sprintf(g_buff4,"%c",char3>=' '?char3:' ');

			//강제지정색상인가?
			strcpy(g_buff8,"");
			if (strlen(buff)>0) {
				sprintf(inv1,"[s%s",buff);
				strcpy(inv2,"[u");
			}
			strcpy(g_buff5,inv1);strcpy(g_buff6,inv2);

			view("frm/screen/%d.frm",now);
		}
		view("frm/screen/%d.bl1",now);
		view("frm/screen/%d.cm2",now);

		printf(" 화면번호:%d 이전(B) 다음(Enter) 편집(E) 선택(Y) 삭제(N) 취소(Q) (번호,Z,LS)>>",now);
		lineinput(buff,3);check_han(buff);printf("\n");nn=atoi(buff);
			 if (is_same(buff,"n")) {bcfg.bform=0;break;}
		else if (is_quit(buff)) break;
		else if (is_same(buff,"b")) now--;
		else if (is_same(buff,"y")) {bcfg.bform=now;break;}
		else if (is_same(buff,"e")) edit_bform(now);
		else if (is_same(buff,"z")) continue;
		else if (is_same(buff,"ls")) {sys_tem("ls -l frm/bform");pressenter();}
		else if (nn>0&&nn<=200) 	now=nn;
		else if (strlen(buff)==0)	now++;
	}
}


char gd_chr(int mode)
{
	int i,i2,n,nn,n2;
	char char3;
	if (bit(cfg.bbsmode,0)) return ' ';    //동작중지
	for (i=0;i<4;i++) {
		if (mode==1&&(i==1||i==3)) continue;   //게시판인경우
		if (mode==0&&(i==0||i==2)) continue;   //자료실인경우
		for (i2=0;i2<10;i2++) {
			if (agood[i][i2].count>0) {
				if ( (i==0&&bbs.read>agood[i][i2].count)||
					 (i==1&&bbs.down>agood[i][i2].count)||
					 (i==2&&calc_ok(1)>agood[i][i2].count)||
					 (i==3&&calc_ok(1)>agood[i][i2].count)) {
						if (agood[i][i2].mark<' ') return ' '; //읽기표시문자
						else return agood[i][i2].mark;		   //읽기표시문자
				}
			}
		}
	}
	return ' ';
}



edit_bform(int now)
{
	char filename[10][100];
	int num;
	if (who<3) {mess("편집할수 없는 등급입니다.");return;}
	sprintf(filename[1],"frm/screen/%d.top",now);
	sprintf(filename[2],"frm/screen/%d.frm",now);
	sprintf(filename[3],"frm/screen/%d.bl1",now);
	sprintf(filename[4],"frm/screen/%d.bl2",now);
	sprintf(filename[5],"frm/screen/%d.cm1",now);
	sprintf(filename[6],"frm/screen/%d.cm2",now);
	sprintf(filename[7],"frm/screen/%d.tor",now);
	sprintf(filename[8],"frm/screen/%d.ftm",now);
	printf("\n");
	printf("  1. TOP 편집 11.등록 21.다운 %s\n",filename[1]);
	printf("  2. FRM 편집 12.등록 22.다운 %s\n",filename[2]);
	printf("  3. BL1 편집 13.등록 23.다운 %s\n",filename[3]);
	printf("  4. BL2 편집 13.등록 23.다운 %s\n",filename[4]);
	printf("  5. CM1 편집 14.등록 24.다운 %s\n",filename[5]);
	printf("  6. CM2 편집 15.등록 25.다운 %s\n",filename[6]);
	printf("  7. TOR 편집 15.등록 25.다운 %s\n",filename[7]);
	printf("  8. FTM 편집 15.등록 25.다운 %s\n",filename[8]);
	num=numselect();
	if (num<1) return;
		 if (num/10==0) xedit(filename[num%10]);
	else if (num/10==1) rz_file(filename[num%10]);
	else if (num/10==2) sz_file(filename[num%10]);
}


read_cdromfile()
{
	char buff2[200];
	if (go.type=='c'&&pf.type>=18) {
		printf("\n 자동으로 읽을 시디롬 디렉토리는? (Q:취소)\n>>");
		lineinput(buff2,79);
		if (!is_same(buff2,"q")&&!is_same(buff2,"p")&&buff2[0]!=0) {
			if (yesno("\n 기존의 자료는 지워집니다. 자료를 읽을까요? (Y/n)",Yes)) {
				printf("\n");
				sys_tem("bin/cd2bbs %s %s/menu %s %s %s",
						buff2,club_go.dir,go.code,pf.id,pf.nick);
			}
		}
		printf("\n");
	}
}


up_nofile()
{
	char buff[300];
	if (is_bbspds) {
		printf("*** %s.tit 화일이 지워지면서 새로 저장됩니다.\n",go.code);
		if (yesno(" 지우고 새로 저장할까요? (y/N)",No)==Yes) {
			printf("\n");
			if (!is_file("system/no_del.dat"))
				sys_tem("bin/dir2bbs %s %s/menu/%s 1",go.dir,club_go.dir,go.code);
		}
	}
}


read_cdrom()
{
	char buff2[300];
	if (pf.type>=18) {
		printf("\n 자동으로 읽을 시디롬 디렉토리는? (Q:취소)\n>>");
		lineinput(buff2,79);
		if (!is_same(buff2,"q")&&!is_same(buff2,"p")&&buff2[0]!=0) {
			if (yesno("\n 기존의 자료는 지워집니다. 자료를 읽을까요? (Y/n)",Yes)) {
				printf("\n");
				sys_tem("bin/cd2bbs %s %s/menu %s %s %s",
						buff2,club_go.dir,go.code,pf.id,pf.nick);
			}
		}
		printf("\n");
	}
}



txt2bbs()
{
	sys_tem("bin/txt2bbs %s %s %s",club_go.code,go.code,pf.id);
}



del_same()	//같은 자료 삭제
{
	FILE *fp1;
	int i,i2,n;
	int count;
	BBS bbs2;
	char buff1[100];
	char buff2[100];

	fp1=fopen(NOW_FILE,"r+");  //제목화일
	if (fp1==NULL) return;

	fseekend(fp1);
	n=ftell(fp1)/sizeof_bbs;
	count=0;
	for (i=n;i>0;i--) {
		fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
		if (feof(fp1)) break;
		if(fread(&bbs,sizeof_bbs,1,fp1)<1) break;
		if (bbs.del) continue;
		for (i2=i-1;i2>0;i2--) {
			fseek(fp1,(i2-1)*sizeof_bbs,SEEK_SET);
			if (feof(fp1)) break;
			if(fread(&bbs2,sizeof_bbs,1,fp1)<1) break;
/*
			if (stristr(bbs.title,"DEMO")&&stristr(bbs2.title,"DEMO")) {
				printf("%s %s %d %d\n",bbs.title,bbs.id,bbs.size,bbs.line);
				printf("%s %s %d %d\n",bbs2.title,bbs2.id,bbs2.size,bbs2.line);
				pressenter();
			}
*/
			if (!bbs2.del
				&&bbs.num!=bbs2.num
				&&bbs.size==bbs2.size
				&&bbs.line==bbs2.line
				&&is_same(bbs.id,bbs2.id)
                &&strcmp(bbs.filename,bbs2.filename)==0) {
				bbs2.del=Yes;
				fseek(fp1,(i2-1)*sizeof_bbs,SEEK_SET);
				fwrite(&bbs2,sizeof_bbs,1,fp1);
				printf("삭제:%5d. %8s %s\n",bbs2.num,bbs2.id,bbs2.title);
				count++;
			}
		}
	}
	printf("\n 총 %d 자료중 %d 개의 같은 자료가 삭제 되었습니다.\n",n,count);
	pressenter();
	if (fp1!=NULL) fclose(fp1);
}



del_allbbs()
{
	printf("\n");
	printfL2();
	if (yesno("게시물의 전체자료를 전부 삭제하겠습니까? (Y:삭제) >>",No)==No) return;
	if (yesno("\n\n한번 삭제하면 복구할수 없습니다. 삭제할까요? (Y:삭제) >>",No)==No) return;
	unlink(NOW_FILE);
	unlink(NOW_FILED);
}



title_strchange()
{
	FILE *fp1;
	int num;
	char buff[150];
	char buff2[150];
	char buffstr[50];
	char buffstr2[50];
	int i;

	printf("\n 제목중 포함된 문자열 변경 처리 \n");
	printf(" 검색할 문자열은? >>");
	lineinput(buffstr,20);printf("\n");
	if (is_quit(buffstr)) return;
	if (buffstr[0]==0) return;
	printf(" 변경할 문자열은? >>");
	lineinput(buffstr2,20);printf("\n");
	if (is_quit(buffstr2)) return;

	printf(" %s 게시판의 제목중 \"%s\" 을 \"%s\" 로 변경합니다. (Y/n) >>",
	go.code,buffstr,buffstr2);
	if (!yesno("",Yes)) return;
	printf("\n");

	fp1=fopen(NOW_FILE,"r+");      //제목화일
	if (fp1!=NULL) {
		fseekend(fp1);
		num=ftell(fp1)/sizeof_bbs;
		rewind(fp1);
		for (i=1;i<=num;i++) {
			fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
			fread(&bbs,sizeof_bbs,1,fp1);
			if (stristr(bbs.title,buffstr)) {
				printf("%5d. %-8s %s\n",bbs.num,bbs.id,bbs.title);
				strcpy(buff2,"");
				strchng(buff2,bbs.title,buffstr,buffstr2);
				buff2[60]=0;
				strcpy(bbs.title,buff2);
				printf("  ->제목변경 %-50.50s\n",bbs.title);
				fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp1);
			}
		}
		fclose(fp1);
	}
	pressenter();
}



id_strchange()
{
	FILE *fp1;
	int num;
	char buff[150];
	char buff2[150];
	char buffstr[50];
	char buffstr2[50];
	char buffstr3[50];
	int i;

	printf(" 검색할 ID는? >>");
	lineinput(buffstr,8);printf("\n");
	if (is_quit(buffstr)) return;
	if (strlen(buffstr)<3) return;
	printf(" 변경할 ID는? >>");
	lineinput(buffstr2,8);printf("\n");
	if (is_quit(buffstr2)) return;
	if (strlen(buffstr2)<3) return;
	printf(" 변경할 이름? >>");
	lineinput(buffstr3,8);printf("\n");
	if (is_quit(buffstr3)) return;
	if (strlen(buffstr3)<3) return;

	fp1=fopen(NOW_FILE,"r+");      //제목화일
	if (fp1!=NULL) {
		fseekend(fp1);
		num=ftell(fp1)/sizeof_bbs;
		rewind(fp1);
		for (i=1;i<=num;i++) {
			fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
			fread(&bbs,sizeof_bbs,1,fp1);
			if (is_same(bbs.id,buffstr)) {
				printf("%5d. %s %s  %-20.20s",bbs.num,bbs.id,bbs.name,bbs.title);
				strcpy(bbs.id,buffstr2);
				strcpy(bbs.name,buffstr3);
				printf("  ->ID변경 %s %s\n",bbs.id,bbs.name);
				fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp1);
			}
		}
		fclose(fp1);
	}
	pressenter();
}


all_pass_del()
{
	FILE *fp;
	printf("메뉴명: %s 총자료수:%d\n",NOW_MENU,count_rec(NOW_MENU,sizeof_go));
	if ((fp=fopen(NOW_MENU,"r"))==NULL) return;
	while(fread(&go,sizeof_go,1,fp)) {
		sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
		sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,go.code);
		if (is_file(NOW_FILE)) {
			printf("%s 처리중...\n",NOW_FILE);
            set_all_file(1);
		}
	}
	fclose(fp);
	mess("처리끝");
}

all_mail_del()
{
	FILE *fp;
	if ((fp=fopen(ID_FILE,"r"))==NULL) return;
	while(fread(&pf2,sizeof_pf,1,fp)) {
		if (pf2.type>0&&strlen(pf2.id)>2) {
			printf("  %s 처리중...\n",NOW_FILE);
			sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf2.id),pf2.id);
			sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf2.id),pf2.id);
            set_all_file(1);
			sprintf(NOW_FILE ,"userdata/%s/%s/send.tit",c_han(pf2.id),pf2.id);
			sprintf(NOW_FILED,"userdata/%s/%s/send.txt",c_han(pf2.id),pf2.id);
            set_all_file(1);
		}
	}
	fclose(fp);
	mess("처리끝");
}

// 게시물 정보 화일헤더
typedef struct {			   // 게시물 정보
	int num;				   // 일련 번호
	char id[9]; 			   // 작성자 아이디
	char name[9];			   // 작성자 이름
	time_t date;			   // 작성 일자
	time_t date2;			   // 작성 일자
	int  line;				   // 내용 길이(줄수)
	int  read;				   // 조회수
	int  down;				   // 다운회수
	word ok;				   // 찬성
	char title[61]; 		   // 제목
	int  posi;				   // 내용 포인터
	int  size;				   // 내용 크기
	char key[9];			   // 검색어
	char gd;				   // 추천자료	   0:보통 1:우수 2:최우수
	char del;				   // 삭제여부	   0:보통 1:삭제
	int  passwd;			   // 암호
	char gdc;				   // 추천색상
	char filename[13];		   // 화일명
	int  filesize;			   // 화일크기
	char tcode[9];			   // 임시가기코드
} BBS3;
BBS3 bbs3;
#define sizeof_bbs3 (sizeof(BBS3))



set_all_file(int mode)
{
	FILE *fp1,*fp2;
	int num;
	time_t today;			 // 작성 일자
	char buff[150];
	char buffjob[50];
	int i;
    if (mode==0) {
        mess("이 기능은 %s 파일의 이상 파일명을 고칩니다",NOW_FILE);
        printf("이전에 동작한적이 없는 가기코드이어야 합니다. (동작시 파일명깨어짐)\n");
        if (!yesno("동작시킬까요?(Y/n)>>",No)) return;
    }
	if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
	if ((fp1=fopen(NOW_FILE,"r"))==NULL) return;
	while(fread(&bbs3,sizeof_bbs3,1,fp1)) {
		bzero(bbs.filename,13);
		bbs.num=bbs3.num;
		strcpy(bbs.id,bbs3.id);
		strcpy(bbs.name,bbs3.name);
		bbs.date=bbs3.date;
		bbs.date2=bbs3.date2;
		bbs.line=bbs3.line;
		bbs.read=bbs3.read;
		bbs.down=bbs3.down;
		bbs.ok=bbs3.ok;
		strcpy(bbs.title,bbs3.title);
		bbs.posi=bbs3.posi;
		bbs.size=bbs3.size;
		strcpy(bbs.key,bbs3.key);
		bbs.gd=bbs3.gd;
		bbs.del=bbs3.del;
		strcpy(bbs.filename,bbs3.filename);
		bbs.filesize=bbs3.filesize;
		strcpy(bbs.tcode,bbs3.tcode);
		strcpy(bbs.passwd,"");
		fwrite(&bbs,sizeof_bbs,1,fp2);
	}
	fclose(fp1);
	fclose(fp2);
	file_move(TMP_FILE,NOW_FILE);

    if (mode==0) {
        printf(" %s 파일의 처리가 끝났습니다.",NOW_FILE);
        printf(" 이 기능을 한번 더 동작시키면 파일명이 삭제되니 주의하십시요\n");
        pressenter();
    }
}

//추천지우기
bbs_del_all(int mode)
{
	FILE *fp;
	int num;
	time_t today;			 // 작성 일자
	time(&today);

	if ((fp=fopen(NOW_FILE,"r+"))==NULL) return No;
	num=0;
	while (fread(&bbs,sizeof_bbs,1,fp)) {
			 if (mode==1) bbs.gd=' ';
		else if (mode==2) bbs.ok=0;
		else if (mode==3) strcpy(bbs.passwd,"");
		else if (mode==5) bbs.down=0;
		else if (mode==6) bbs.read=0;
		else if (mode==7) strcpy(bbs.key,"");
		else if (mode==8) {strcpy(bbs.id,".......");strcpy(bbs.name,".......");}
		else if (mode==9) bbs.date=bbs.date2=today;  //읽은 날짜 지정
		else if (mode==99) strcpy(bbs.passwd,"");
		fseek(fp,num*sizeof_bbs,SEEK_SET);
		fwrite(&bbs,sizeof_bbs,1,fp);
		num++;
	}
	fclose(fp);
	return Yes;
}


bbs_del_filename()
{
	FILE *fp,*fp2;
	DIR *dp;
	struct dirent *dfp;
	struct stat stbuf;
	char buff[100];
	char afile[5000][14];
	int  asize[5000];
	int count,num,i,find_num;
	time_t today;			 // 작성 일자
	time(&today);

	if (!is_typepds||strlen(go.dir)<1) {mess("자료실이 아니거나 디렉토리가 없음");return No;}
	dp=opendir(go.dir);
	count=0;
	while(count<5000&&(dfp=readdir(dp)) != NULL) {
		if(*(dfp->d_name) != '.') {
			sprintf(buff,"%s/%s",go.dir,dfp->d_name);
			if (stat(buff,&stbuf)==-1||strlen(dfp->d_name)<1||stbuf.st_size<1) continue;  //화일없을때
			strcpy(afile[count],dfp->d_name);
			asize[count]=stbuf.st_size;    //화일크기
			count++;
		}
	}
	closedir(dp);

	if ((fp=fopen(NOW_FILE,"r+"))==NULL) return No;
	num=0;
	while (fread(&bbs,sizeof_bbs,1,fp)) {
		if (strlen(bbs.filename)<1&&bbs.filesize>0) {
			find_num=0;
			for (i=0;i<count;i++) {
				if (asize[i]==bbs.filesize) {
					find_num++;
					strcpy(bbs.filename,afile[i]);
				}
			}
			if (find_num==1) {
				fseek(fp,num*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp);
				bbs.title[50]=0;
				printf("검색 %5d %13s %8d Byte %s\n",bbs.num,bbs.filename,bbs.filesize,bbs.title);
			}
			else if (find_num>1) {
				bbs.title[50]=0;
				printf("중복 %5d %13s %8d Byte %s\n",bbs.num,bbs.filename,bbs.filesize,bbs.title);
			}
		}
		num++;
	}
	fclose(fp);
	pressenter();
	return Yes;
}


bbs_del2_filename()
{
	FILE *fp,*fp2;
	DIR *dp;
	struct dirent *dfp;
	struct stat stbuf;
	char buff[100];
	char afile[5000][14];
	int  asize[5000];
	int count,num,i,find_num;
	time_t today;			 // 작성 일자
	time(&today);

    if ((fp=fopen(NOW_FILE,"r+"))==NULL) return No;
	num=0;
    while (1) {
        fseek(fp,num*sizeof_bbs,SEEK_SET);
        if (!fread(&bbs,sizeof_bbs,1,fp)) break;
        if (strlen(bbs.filename)<1) goto _cont;
        sprintf(buff,"%s/%s",go.dir,bbs.filename);
        if (is_file(buff)) goto _cont;

        dp=opendir(go.dir);
        count=0;
        while((dfp=readdir(dp)) != NULL) {
            if(*(dfp->d_name) != '.') {
                if (strstr(dfp->d_name,bbs.filename)) {
                    printf(" %d %s -> %s \n",num,bbs.filename,dfp->d_name);
                    strcpy(bbs.filename,dfp->d_name);
                    break;
                }
            }
        }
        closedir(dp);

        _cont:
        fseek(fp,num*sizeof_bbs,SEEK_SET);
        fwrite(&bbs,sizeof_bbs,1,fp);
        num++;
	}
	fclose(fp);
    mess("작업끝");
	return Yes;
}


set_all_del(int mode)
{
	FILE *fp1;
	int num;
	time_t today;			 // 작성 일자
	char buff[150];
	char buffjob[50];
	int i;

	strcpy(buffjob,mode==1?"삭제":"복구");
    sprintf(buff,"\n 전체 게시물을 %s 처리 할까요? (%sY / 취소n)",buffjob,buffjob);

	if (yesno(buff,Yes)==No) return;
	printf("\n");
	fp1=fopen(NOW_FILE,"r+");      //제목화일
	fseekend(fp1);
	num=ftell(fp1)/sizeof_bbs;
	rewind(fp1);
	time(&today);
	for (i=1;i<=num;i++) {
		fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
		fread(&bbs,sizeof_bbs,1,fp1);
		if (mode==1) bbs.del=Yes;
		else		 bbs.del=No;
		fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
		fwrite(&bbs,sizeof_bbs,1,fp1);
	}
	if (fp1!=NULL) fclose(fp1);
	printf("\n *** 전체게시물을 %s 처리 했습니다.***\n",buffjob);
	pressenter();
}





set_all_key()
{
	FILE *fp1;
	int num;
	time_t today;			 // 작성 일자
	char buff[150];
	char buffjob[50];
	int i;

	sprintf(buff,"\n 전체 게시물의 키워드를 변경 할까요? (Y/n)");

	if (yesno(buff,Yes)==No) return;
	printf("\n");
	printf("전체 새로 지정할 키워드는? (Q:취소) >>");
	lineinput(buff,8);printf("\n");check_han(buff);
	if (is_same(buff,"q")||is_same(buff,"p")) return;

	if ((fp1=fopen(NOW_FILE,"r+"))!=NULL) {      //제목화일
		fseekend(fp1);
		num=ftell(fp1)/sizeof_bbs;
		rewind(fp1);
		time(&today);
		for (i=1;i<=num;i++) {
			fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
			if (fread(&bbs,sizeof_bbs,1,fp1)) {
				strcpy(bbs.key,buff);
				fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp1);
			}
		}
		fclose(fp1);
	}
}



set_all_id(int mode)
{
	FILE *fp1;
	int num;
	char buff[150];
	char buffid[50];
	char buffna[50];
	int i;

	printf("\n 전체 게시물을 지정할 ID는  ? >>");
	lineinput(buffid,8);printf("\n");
	if (is_same(buffid,"q")||is_same(buffid,"p")) return;
	printf("\n 전체 게시물을 지정할 이름은? >>");
	lineinput(buffna,8);printf("\n");
	if (is_same(buffna,"q")||is_same(buffna,"p")) return;
	if (buffid[0]==0&&buffna[0]==0) return;

	if ((fp1=fopen(NOW_FILE,"r+"))!=NULL) {      //제목화일
		fseekend(fp1);
		num=ftell(fp1)/sizeof_bbs;
		rewind(fp1);
		for (i=1;i<=num;i++) {
			fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
			fread(&bbs,sizeof_bbs,1,fp1);
			if (buffid[0]!=0) strcpy(bbs.id,buffid);
			if (buffna[0]!=0) strcpy(bbs.name,buffid);
			fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
			fwrite(&bbs,sizeof_bbs,1,fp1);
		}
		fclose(fp1);
		printf("\n *** 전체게시물의 아이디 지정 처리 했습니다.***\n");
	}
	pressenter();
}


del_all_size(int mode)
{
	FILE *fp1;
	int num;
	char buff[150];
	char buffid[50];
	char buffna[50];
	int n,i;

	if (mode==0) printf("\n 얼마 크기(Byte)이하의 게시물을 삭제할까요? >>");
	else		 printf("\n 얼마 크기(Byte)이상의 게시물을 삭제할까요? >>");
	n=numinput();printf("\n");
	if (n>0&&(fp1=fopen(NOW_FILE,"r+"))!=NULL) {      //제목화일
		fseekend(fp1);
		num=ftell(fp1)/sizeof_bbs;
		rewind(fp1);
		for (i=1;i<=num;i++) {
			fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
			fread(&bbs,sizeof_bbs,1,fp1);
			if ( (mode==0&&bbs.size<n&&!bbs.del)||
				 (mode==1&&bbs.size>n&&!bbs.del) ) {
				printf("%6d. %-8s %6d Byte %s ->삭제\n",bbs.num,bbs.id,bbs.size,bbs.title);
				bbs.del=Yes;
				fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp1);
			}
		}
		fclose(fp1);
		if (mode==0) printf("\n *** %d 크기 이하의 게시물 삭제 처리 했습니다.***\n",n);
		else		 printf("\n *** %d 크기 이상의 게시물 삭제 처리 했습니다.***\n",n);
	}
}


del_strin()
{
	FILE *fp1;
	int num;
	char buff[150];
	char buffstr[50];
	char buffna[50];
	int i;

	printf("\n 제목중 포함된 문자열 삭제 처리 \n");
	printf(" 검색하여 삭제할 문자열은? >>");
	lineinput(buffstr,20);printf("\n");
	if (is_same(buffstr,"q")||is_same(buffstr,"p")) return;
	if (buffstr[0]==0) return;

	fp1=fopen(NOW_FILE,"r+");      //제목화일
	if (fp1!=NULL) {
		fseekend(fp1);
		num=ftell(fp1)/sizeof_bbs;
		rewind(fp1);
		for (i=1;i<=num;i++) {
			fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
			fread(&bbs,sizeof_bbs,1,fp1);
            if (!bbs.del&&stristr(bbs.title,buffstr)) {
				printf("삭제:%5d. %-8s %s\n",bbs.num,bbs.id,bbs.title);
				bbs.del=Yes;
				fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp1);
			}
		}
		fclose(fp1);
	}
    mess("작업끝");
}



del_strid()
{
	FILE *fp1;
	int num;
	char buff[150];
	char buffstr[50];
	char buffna[50];
	int i;

    printf(" 검색하여 삭제할 게시판의 ID? >>");
    lineinput(buffstr,8);printf("\n");
    if (is_quit(buffstr)||strlen(buffstr)<3) return;

	fp1=fopen(NOW_FILE,"r+");      //제목화일
	if (fp1!=NULL) {
		fseekend(fp1);
		num=ftell(fp1)/sizeof_bbs;
		rewind(fp1);
		for (i=1;i<=num;i++) {
			fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
			fread(&bbs,sizeof_bbs,1,fp1);
            if (!bbs.del&&is_same(bbs.id,buffstr)) {
				printf("삭제:%5d. %-8s %s\n",bbs.num,bbs.id,bbs.title);
				bbs.del=Yes;
				fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp1);
			}
		}
		fclose(fp1);
	}
    mess("작업끝");
}



//현재 게시판의 내용을 전송한다.
bbs_all_down()
{
	char file[250];
	char file1[250];
	char file2[250];
	char buff[300];
	char olddir[480];
	int n,size1,size2;
	getcwd(olddir,sizeof(olddir));		//현재의 디렉토리

	sprintf(buff,"%s/menu/%s.tit",club_go.dir,go.code);
	size1=file_size(buff);
	sprintf(buff,"%s/menu/%s.txt",club_go.dir,go.code);
	size2=file_size(buff);
	sprintf(file,"%s.zip",go.code);
	sprintf(file1,"%s.tit",go.code);
	sprintf(file2,"%s.txt",go.code);
	cls();
	print_(1,"게시판을 압축 다운/등록메뉴");
	printf("  가기코드 : %s (%s)\n",go.code,go.name);
	printf("  제목화일 : %s:%dbyte\n",file1,size1);
	printf("  내용화일 : %s:%dbyte\n",file2,size2);
	printf("  압축화일 : %s\n\n",file);
	printf("  1. 현재게시판을 압축화일 %s로 다운받기  (다운)\n",file);
	printf("  2. 현재게시판을 압축화일 %s로 덮어쒸우기(등록)\n",file);
	n=numselect();
	if (n==1&&yesno(" 압축화일을 다운받으시겠습니까? 다운(Y)",Yes)) {
		printf("\n");
		sprintf(buff,"%s/menu",club_go.dir);
		chdir(buff);
		if (is_file(file1)) {
			sys_tem("zip '%s' '%s' '%s'",file,file1,file2);
		}
		chdir(olddir);
		sprintf(file,"%s/menu/%s.zip",club_go.dir,go.code);
		if (is_file(file)) {
			printf("\n 압축화일명:%s.zip  압축화일크기:%dbyte\n",go.code,file_size(file));
			if (yesno("이 화일을 전송할까요? (Y/n) >>",Yes)) {
				sz(file);
				unlink(file);
			}
		}
	}
	else if (n==2&&yesno(" 현재게시판을 덮어쒸우겠습니까?  (y/N)",No)) {
		printf("\n");
		sprintf(buff,"%s/menu",club_go.dir);
		if (!rz_file(file)||!is_file(file)) return;
		if (!stristr(file,".zip")) return;
		check_dir("tmp/up%d%s",cfg.hostno,no_spc(tty));
		sys_tem("unzip -j-n-qq -dtmp/up%d%s '%s' %s.t?t",cfg.hostno,no_spc(tty),file,go.code);
		sprintf(file,"tmp/up%d%s/%s.tit",cfg.hostno,no_spc(tty),go.code);
		n=file_size(file);
		if (n>3&&yesno("등록한 화일로 덮어쒸울까요?(Y/n)>>",Yes)) {
			c_lock(1,NOW_FILE);
			sprintf(file2,"%s/menu/%s.tit",club_go.dir,go.code);
			file_copy(file,file2);
			sprintf(file,"tmp/up%d%s/%s.txt",cfg.hostno,no_spc(tty),go.code);
			touch_file(file);
			sprintf(file2,"%s/menu/%s.txt",club_go.dir,go.code);
			file_copy(file,file2);
			mess("%s 등록이 끝났습니다.",file);
			c_lock(0,NOW_FILE);
		}
		rm_tmpup();
	}
}



//삭제된 게시물 정리
bbs_pack()
{
    FILE *fp1,*fp2,*fp3,*fp4;
    int re_num=No,size_zero=No;
    int i,found,new_num,new_posi;
	BBS oldbbs;

	if (is_file("system/no_del.dat")) return;
	if (is_char(BBSTYPE,go.type)==No) return;
	if (yesno("\n삭제된 게시물을 완전 삭제(PACK)하겠습니까? (Y/n) >>",Yes)==No) return;
	re_num=yesno("\n삭제시 번호를 재정리 할까요? (Y/n) >>",Yes);
    if (is_typepds&&yesno("\n삭제시 파일크기 0인 자료를 삭제할까요? (Y/n) >>",Yes))
        size_zero=Yes;
	fp1=fopen(NOW_FILE,"r");      //제목화일
	fp2=fopen(NOW_FILED,"r");      //제목화일
	fp3=fopen(TMP_FILE,"w");      //임시화일
	fp4=fopen(TMP_FILE2,"w");      //임시화일
	fseek(fp1,0,SEEK_SET);
	new_posi=0;
	found=No;
	new_num=1;
	while (fread(&bbs,sizeof_bbs,1,fp1)>0) {
		oldbbs=bbs;
		if (size_zero&&bbs.filesize<=0) bbs.del=Yes;
		if (!bbs.del) { 	  //삭제된 게시물이 아닐때
            if (bbs.size<0) bbs.size=0;
            if (bbs.size>100000) bbs.size=100000;
            if (fp2==NULL) bbs.size=0;
			else {
				fseek(fp2,bbs.posi,SEEK_SET);
				fseek(fp4,new_posi,SEEK_SET);
				for (i=0;i<bbs.size&&!feof(fp2);i++) {
					fputc(fgetc(fp2),fp4);
				}
			}
			bbs.posi=new_posi;
			if (re_num) bbs.num=new_num;
            fseek(fp3,(new_num-1)*sizeof_bbs,SEEK_SET);
			fwrite(&bbs,sizeof_bbs,1,fp3);
			new_posi+=bbs.size;
			new_num++;
		}
        else {
			found=Yes;		  //삭제된것이 있을때
            if (strlen(bbs.title)>30) bbs.title[30]=0;
			printf(" 삭제 -> %5d. %-8s %s \n",bbs.num,bbs.id,bbs.title);
		}
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (fp3!=NULL) fclose(fp3);
	if (fp4!=NULL) fclose(fp4);

	if (found) {
		file_move(TMP_FILE ,NOW_FILE );
		file_move(TMP_FILE2,NOW_FILED);
	}
	unlink(TMP_FILE);
	unlink(TMP_FILE2);
    mess("작업끝");
}


//번호 재정리
bbs_renum()
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char buff[180];
	char c;
	char str[10];
	BBS bbs;
	int found=No;

	int num;
	int ret,i,new_posi;

	re_print=Yes;
	num=mmn[1];
	ret=No;
	re_print=Yes;
    if ((fp1=fopen(NOW_FILE,"r+"))==NULL) return;      //제목화일
	fseekend(fp1);
	num=ftell(fp1)/sizeof_bbs;
	rewind(fp1);
	for (i=1;i<=num;i++) {
		fseek(fp1,(i-1)*sizeof_bbs,SEEK_SET);
		fwrite(&i,4,1,fp1);
	}
    fclose(fp1);
	re_print=Yes;
	re_bbs=Yes;
}


//번호 재정리
bbs_renum2()
{
    FILE *fp1;
	BBS bbs;
    int num=0;
    if ((fp1=fopen(NOW_FILE,"r+"))==NULL) return;      //제목화일
    while(num<100000) {
        fseek(fp1,num*sizeof_bbs,SEEK_SET);
        if (!fread(&bbs,sizeof_bbs,1,fp1)) break;
        bbs.num=num+1;
        fseek(fp1,num*sizeof_bbs,SEEK_SET);
        fwrite(&bbs,sizeof_bbs,1,fp1);
        num++;
    }
    fclose(fp1);
	re_bbs=Yes;
}


//게시판 합치기 루틴
b_add()
{
	FILE *fp1;
	FILE *fp2;
	FILE *fp3;
	FILE *fp4;
	int i,n,new_posi,new_num;
	char buff[540];
	char buff2[540];
	char add_file1[540];
	char add_file2[540];
	char filename[540];

	printf("\n");
	printfL2();
	printf(" 이 메뉴는 게시판을 다른 게시판과 합치는 명령입니다.\n");
	printf(" 내용을 전송할때에는 반드시 제목화일(*.tit)와 내용화일(*.txt)이 \n");
	printf(" 원래 그대로 또는 zip으로 압축이 되어야 합니다.\n");
	rz_file(filename);	//화일전송받기
	if (!is_file(filename)) goto fail_badd;
	if (stristr(filename,".zip"))
		sys_tem("unzip -j-n-qq -dtmp/up%d%s '%s' %s.t?t",cfg.hostno,no_spc(tty),filename,go.code);
	sprintf(buff,"tmp/up%d%s/%s.tit",cfg.hostno,no_spc(tty),go.code);
	sprintf(add_file1,"tmp/up%d%s/%s.tit",cfg.hostno,no_spc(tty),go.code);
	sprintf(add_file2,"tmp/up%d%s/%s.txt",cfg.hostno,no_spc(tty),go.code);
	if (!is_file(add_file1)||!is_file(add_file2)) goto fail_badd;

	//마지막 자료 위치 파악
	new_posi=0;
	new_num=1;
	n=0;
	fp1=fopen(NOW_FILE,"r");                     //제목화일
	if (fp1!=NULL) {
		fseekend(fp1);
		n=ftell(fp1)/sizeof_bbs;
		fseek(fp1,n*sizeof_bbs,SEEK_SET);
		fread(&bbs,sizeof_bbs,1,fp1);
		new_posi=bbs.posi+bbs.size; 	//새로운 시작 위치
		new_num=bbs.num+1;
		fclose(fp1);
	}


	//fp3->fp1	fp4->fp2
	fp1=fopen(NOW_FILE,"r+");                     //제목화일
	if (fp1==NULL) fp1=fopen(NOW_FILE,"w");                     //제목화일
	fp2=fopen(NOW_FILED,"r+");                    //내용화일
	if (fp2==NULL) fp2=fopen(NOW_FILED,"w");                     //제목화일
	fp3=fopen(add_file1,"r+");                    //제목화일
	if (fp3==NULL) {
		if (fp1!=NULL) fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
		if (fp3!=NULL) fclose(fp3);
		mess("%s 를 열수 없습니다.",add_file1);
		return;
	}
	fp4=fopen(add_file2,"r+");                    //내용화일

	printf("\n자료추가 : %s -> %s\n\n",add_file1,NOW_FILE);
	while (fread(&bbs,sizeof_bbs,1,fp3)) {
		//새로운 게시판 내용 추가
		fseek(fp4,bbs.posi,SEEK_SET);
		fseek(fp2,new_posi,SEEK_SET);
		for (i=0;i<bbs.size;i++) fputc(fgetc(fp4),fp2);
		//새로운 게시판 제목 추가
		bbs.posi=new_posi;
		bbs.num=new_num;
		fseekend(fp1);
		del_space(bbs.filename);del_esc(bbs.filename);
		fwrite(&bbs,sizeof_bbs,1,fp1);
		printf("%5d. %-8s %13s %s\n",bbs.num,bbs.id,bbs.filename,bbs.title);
		new_posi+=bbs.size;
		new_num++;
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (fp3!=NULL) fclose(fp3);
	if (fp4!=NULL) fclose(fp4);
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
	mess("게시판 합치기가 끝났습니다.");
	return;

	fail_badd:
	mess("해당되는 게시판화일이 없습니다.");
}


//화일크기 이상할때
check_bbs()
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char buff[180];
	char c;
	char str[10];
	BBS bbs;
	int found=No;
	int ret,i,new_posi;
	int new_num;

	re_print=Yes;
	ret=No;
	re_print=Yes;
	fp1=fopen(NOW_FILE,"r+");      //제목화일
	fp2=fopen(NOW_FILED,"r+");      //제목화일
	fp3=fopen(TMP_FILE,"w");      //임시화일
	fp4=fopen(TMP_FILE2,"w");      //임시화일
	fseek(fp1,0,SEEK_SET);
	new_posi=0;
	new_num=1;

	while (fread(&bbs,sizeof_bbs,1,fp1)>0) {
			fseek(fp2,bbs.posi,SEEK_SET);
			fseek(fp4,new_posi,SEEK_SET);
			if (bbs.size>1000000) bbs.size=1000000;
			for (i=0;i<bbs.size;i++) {
				c=fgetc(fp2);
				fputc(c,fp4);
				if (c==0) {
					bbs.size=i+1;
					break;
				}
			}
			bbs.posi=new_posi;
			bbs.num=new_num;
			fwrite(&bbs,sizeof_bbs,1,fp3);
			new_posi+=bbs.size;
			new_num++;
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (fp3!=NULL) fclose(fp3);
	if (fp4!=NULL) fclose(fp4);

	unlink(NOW_FILE);
	unlink(NOW_FILED);
	if (is_file(TMP_FILE)) {
		sprintf(buff,"cp %s %s -rf",TMP_FILE,NOW_FILE);
		sys_tem(buff);
	}

	if (is_file(TMP_FILE2)) {
		sprintf(buff,"cp %s %s -rf",TMP_FILE2,NOW_FILED);
		sys_tem(buff);
	}
	return Yes;
}




//화일의 내용을 0으로 만듬
set_zero(char *code)
{
	char filename1[200];
	char filename2[200];
	FILE *fp;

	sprintf(filename1,"%s/menu/%s.tit",club_go.dir,code);
	sprintf(filename2,"%s/menu/%s.txt",club_go.dir,code);
	fp=fopen(filename1,"w");
	if (fp!=NULL) fclose(fp);
	fp=fopen(filename2,"w");
	if (fp!=NULL) fclose(fp);
}



ed_rsize()
{
	FILE *fp1;
	int i,num,oldsize;
	char buff[100];
	char dir[200];
	char filename[200];

	if (!is_bbspds) return;
	sprintf(buff,"\n%s (%s) 를 화일크기를 다시 세팅 할까요?",NOW_FILE,go.dir);
	if (!yesno(buff,Yes)) return;
	printf("\n");

	c_lock(1,NOW_FILE);
	fp1=fopen(NOW_FILE,"r+");      //제목화일
	if (fp1==NULL) return;
	fseekend(fp1);
	num=ftell(fp1)/sizeof_bbs;
	rewind(fp1);
	for (i=0;i<num;i++) {
		fseek(fp1,i*sizeof_bbs,SEEK_SET);
		fread(&bbs,sizeof_bbs,1,fp1);
		oldsize=bbs.filesize;
		sprintf(filename,"%s/%s",go.dir,bbs.filename);
		bbs.filesize=file_size(filename);	 //화일의 크기 없으면 -1
		fseek(fp1,i*sizeof_bbs,SEEK_SET);
		fwrite(&bbs,sizeof_bbs,1,fp1);
		printf("%5d. 화일명:%-13s  화일의 크기:%8d",
		bbs.num,bbs.filename,bbs.filesize);
		if (oldsize!=bbs.filesize) printf("  <-변경");
		printf("\n");
	}
	fclose(fp1);
	c_lock(0,NOW_FILE);
	pressenter();
}

//date 와 strdate를 비교하여 결과 return;
//1:== 2!= 3:>	4:>=  5:<  6:<=
cmp_date(time_t date,char *strdate,int mode)
{
	char buff[100];
	int nn;
	nn=strcmp(datestr(0,date),strdate);
	if (mode==1&&nn==0) return Yes;
	if (mode==2&&nn!=0) return Yes;
	if (mode==3&&nn> 0) return Yes;
	if (mode==4&&nn>=0) return Yes;
	if (mode==5&&nn< 0) return Yes;
	if (mode==6&&nn<=0) return Yes;
	return No;
}


reset_search()
{
	FILE *fp;
	int i,n;
	char buff[100];
	char filename[200];

	reset_sss();
	sprintf(filename,"%s/bbsdata/menucfg/%s.lx",club_go.dir,go.code);
	fp=fopen(filename,"r+");
	if (fp!=NULL) {
		for (i=0;i<30;i++) {
			fscanf(fp,"%2d %2d %2d %2d %2d\n",&asearch0[i],&asearch1[i],&asearch2[i],&asearch3[i],&asearch4[i]);
		}
	}
	if (fp!=NULL) fclose(fp);

	cls();
	printfL2();
	printf("  ####     LX 검색시 검색항목,위치 조정   <w:완료 q:취소 r:reset> #### \n");
	printfL2();
	for (i=0;i<15;i++) {
		xy(1,i+4);
		printf("  %2d. 위치:(%2d,%2d)  항목:%2d  타입:%2d   길이:%2d \n",i+1,asearch0[i],asearch1[i],asearch2[i],asearch3[i],asearch4[i]);
	}
	for (i=0;i<15;i++) {
		xy(13,i+ 4);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch0[i]=atoi(buff);
		xy(16,i+ 4);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch1[i]=atoi(buff);
		xy(26,i+ 4);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch2[i]=atoi(buff);
		xy(36,i+ 4);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch3[i]=atoi(buff);
		xy(46,i+ 4);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch4[i]=atoi(buff);
	}
	if (is_same(buff,"r+")) {reset_sss();goto save_search;}
	if (is_same(buff,"q")) return;
	if (is_same(buff,"w")) goto save_search;
	cls();
	printfL2();
	printf("  ####     LX 검색시 검색항목,위치 조정   <w:완료 q:취소 r:reset> #### \n");
	printfL2();
	for (i=15;i<30;i++) {
		xy(1,i-11);
		printf("  %2d. 위치:(%2d,%2d)  항목:%2d  타입:%2d   길이:%2d \n",i+1,asearch0[i],asearch1[i],asearch2[i],asearch3[i],asearch4[i]);
	}
	for (i=15;i<30;i++) {
		xy(13,i-11);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch0[i]=atoi(buff);
		xy(16,i-11);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch1[i]=atoi(buff);
		xy(26,i-11);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch2[i]=atoi(buff);
		xy(36,i-11);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch3[i]=atoi(buff);
		xy(46,i-11);lineinput(buff,2);if (is_same(buff,"q")||is_same(buff,"w")) break;
		if (is_same(buff,"r+")) break;
		if (buff[0]!=0) asearch4[i]=atoi(buff);
	}
	if (is_same(buff,"r+")) {reset_sss();goto save_search;}
	if (is_same(buff,"q")) return;
	if (is_same(buff,"w")) goto save_search;
	return;
	save_search:
	fp=fopen(filename,"w+");
	for (i=0;i<30;i++) {
		fprintf(fp,"%2d %2d %2d %2d %2d\n",asearch0[i],asearch1[i],asearch2[i],asearch3[i],asearch4[i]);
	}
	if (fp!=NULL) fclose(fp);
}


reset_sss()
{
	int i;
	for (i=0;i<30;i++) {
		asearch0[i]=0;
		asearch1[i]=0;
		asearch2[i]=0;
		asearch3[i]=0;
		asearch4[i]=0;
	}
}


set_blogo()
{
	FILE *fp;
	int n=0,i,count;
	int key;
	int can_edit=No;		//편집가능함?
	char buff[240];
	char buff2[240];
	char tcode[60];
	char now_id[60];
	char filename[200];
	char filename2[200];
	char id_file[200];
	char cfg_file[200];
	char dir[200];
	char cmd;
	char olddir[180];
	char *aname[40]={
	"진입로고  ","심야진입  ","메뉴화면  ","등록화면  ","등록완료  ",
	"상단화면  ","말 머 리  ","말머리선택","명령도움  ","읽기도움  ",
	"하단명령  ","읽기선택  ","하단선택  ","기본설명  ","출력형태  ",
	"읽기상단  ","읽기형태  ","FORM설정  ","LX  화면  ","진입 VTX  ",
	"진입 JPG  ","설문자료  ","M1~M9 명령","하단 BL1"  ,"하단 BL2  ",
	"손님진입  ","야간진입  ","비안시화면","진입아디"  ,""          ,
	"손님진입 M","야간진입 M","비안시   M",""          ,""          ,
	""          ,""          ,""          ,"광고 AD?"  ,"광고 TA?" };
	char *atail[40]={
	"log","lo5","mnu","bbs","bb2",
	"top","hea","hsl","hlp","hl2",
	"bot","cm1","cm2","qsh","frm",
	"ftm","dsp","for","slx","vtx",
	"jpg","qst","dim","bl1","bl2",
	"lo4","lo5","lo2","sid","tmp",
	"mn4","mn5","mn2","tmp","tmp",
	"tmp","tmp","tmp","ad0","ta0"};
	char afile[40][50];

	getcwd(olddir,sizeof(olddir));		//현재의 디렉토리
	sprintf(dir,"%s/logo",club_go.dir);
	strcpy(tcode,go.code);			  //가기코드

	club_mode=No;
	for (i=0;i<40;i++) sprintf(afile[i],"%s.%s",tcode,atail[i]);

	while (1) {
		cls();
		print_(" %s 게시판 화면 편집 메뉴",tcode);
		for (i=0;i<40;i++) {
			xy((i/20)*42,(i%20)+3);
			sprintf(buff,"%s/logo/%s",club_go.dir,afile[i]);
			printf("%2d.%-10s(%-12s)%6dB\n",i+1,aname[i],afile[i],file_size(buff));
		}
		xy(1,23);
		printf(" 편집(E) 삭제(D) 등록(UP) 다운(DN) 도움(H)\n");
		printf(" 전체다운(AD) 전체업(AU) 전체삭제(AK)  >>");
		lineinput(buff,5);check_han(buff);printf("\n");
		if (is_char("pPqQxX0",buff[0])) break;
		check_cmd(buff);
		strcpy(filename,"");
		if (mmn[1]==0) mmn[1]=mmn[0];
		if (mmn[1]>0&&mmn[1]<=40) {
			sprintf(filename,"%s/logo/%s",club_go.dir,afile[mmn[1]-1]);
		}

		if ((mmn[1]==39||mmn[1]==40)&&
			(is_same(mmc[0],"e")||is_same(mmc[0],"d")||is_same(mmc[0],"up")||is_same(mmc[0],"dn"))) {
			printf(" 화면선택번호는? (0~9) >>");
			lineinput(buff,2);
			n=atoi(buff);
			if (is_quit(buff)||strlen(buff)==0||n>9) continue;
			if (mmn[1]==39) {
				sprintf(buff2,"ad%d",n);
				strchng2(filename,"ad0",buff2);
			}
			else {
				sprintf(buff2,"ta%d",n);
				strchng2(filename,"ta0",buff2);
			}
		}

		if (mmn[0]>0&&mmn[0]<=40) {
			view(filename);
			printf("파일명:\"%s\"  크기:%dbyte\n",filename,file_size(filename));
			pressenter();
			continue;
		}
			 if (is_same(mmc[0],"h")) view_text("frm/help/logo.hlp");
		else if (is_same(mmc[0],"ad")) {
			if (yesno("해당 게시판의 전체화면를 다운받겠습니까? (Y/n) >>",Yes))
				sys_tem("%s '%s/logo/%s.*'",cfg.sz,club_go.dir,go.code);
			continue;
		}
		else if (is_same(mmc[0],"ak")) {
			if (yesno("해당 게시판의 전체화면를 삭제하겠습니까? (y/N) >>",No))
				sys_tem("rm -f %s/logo/%s.*",club_go.dir,go.code);
			continue;
		}
		else if (is_same(buff,"au")) {
			if (yesno("해당 게시판의 전체화면를 자동등록하겠습니까? (Y/n) >>",Yes)) {
				printf("\n ******* 연속업로드를 해도 됩니다. *********\n");
				getcwd(olddir,sizeof(olddir));		//현재의 디렉토리
				sys_tem("rm tmp/up%d%s -rf",cfg.hostno,no_spc(tty));
				sprintf(buff2,"tmp/up%d%s",cfg.hostno,no_spc(tty));
				check_dir(buff2);
				chdir(buff2);
				rz();
				chdir(olddir);
				sys_tem("cp -f tmp/up%d%s/%s.* %s/logo/",cfg.hostno,no_spc(tty),go.code,club_go.dir);
				rm_tmpup(); 	   //임시디렉토리 지움
			}
			continue;
		}
		else if (strlen(filename)<1);
		else if (is_same(mmc[0],"e" )) xedit(filename);
		else if (is_same(mmc[0],"up")) {
			printf(" ### %s 로고를 등록합니다. ###\n",filename);
			if (up_file(0,buff,No)&&is_file(buff)) {   //화일을 전송받아 filename에 저장한다.
				file_copy(buff,filename);
				rm_tmpup(); 	   //임시디렉토리 지움
			}
		}
		else if (is_same(mmc[0],"dn")) sz_file(filename);   //화일전송
		else if (is_same(mmc[0],"d" )) {
			printf(" %s 화일을 삭제합니다. 삭제를 할까요? (Y/n) >>",filename);
			if (yesno("",Yes)) unlink(filename);
		}
	}
}

