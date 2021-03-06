
//동호회 새로 만들기
club_make(char *now_dir)
{
	char buff[200];
	char buff2[200];
	int i,n;
	i=0;
	n=0;
	// 기본 디렉토리 부터 만들기
	while (i<=(int)strlen(now_dir)) {
		if (i==(int)strlen(now_dir)||(i>0&&now_dir[i]=='/')) {
			buff2[n]=0;
			if (!is_file(buff2)) {
				sprintf(buff,"\n %s 디렉토리를 만들까요? (만듬:Y) >>",buff2);
                if (!yesno2(buff,Yes)) return;
				mkdir3(buff2);
			}
		}
		buff2[n]=now_dir[i];
		n++;
		i++;
	}
    check_dir("%s/system",now_dir);
    check_dir("%s/menu"  ,now_dir);
    check_dir("%s/logo"  ,now_dir);
    check_dir("%s/pds"   ,now_dir);
    check_dir("%s/bbsdata"  ,now_dir);
    check_dir("%s/bbsdata/iprec"  ,now_dir);
    check_dir("%s/bbsdata/menurec",now_dir);
    check_dir("%s/bbsdata/menucfg",now_dir);
    check_dir("%s/user"     ,now_dir);
    check_dir("%s/menustru" ,now_dir);
    check_dir("%s/frm"      ,now_dir);
    sys_tem("touch %s/system/clubcfg.dat",now_dir);
    sys_tem("touch %s/system/clubid.dat",now_dir);
    if (is_file("system/default/clubmenu.dat")) {
        sys_tem("cp -f system/default/clubmenu.dat %s/menustru",now_dir);
    }
    sys_tem("touch %s/menustru/menu.dat",now_dir);
    mess("%s 의 디렉토리가 만들어 졌습니다.",now_dir);
}


disp_set_stat()        //현재 상태 보여주기
{
	FILE *fp;
	int n,x,i,i2;
	time_t tnow;
	PF pf3;
	struct tm *tp;
    char count_file[100],buff[200];

    for(i=0;i<20;i++) ast[i]=0;
	time(&tnow);   //현재시간
	tp = localtime(&tnow); //12311231
	if ( (tnow-lastnow)>30 ) {	//30초 경과 여부
        ast[1]=count_moni();
		stat_pf();
        ast[2]=total;
        ast[3]=0;
		fp=fopen(GUEST_FILE,"r+");
		if (fp!=NULL) {
			fseekend(fp);
            ast[3]=ftell(fp)/sizeof_pf;
			fclose(fp);
		}
		if ( (tnow-lastnow2)>90 ) {  //30초 경과 여부
            ast[4]=ast[5]=ast[6]=ast[7]=ast[8]=ast[9]=ast[10]=ast[11]=ast[14]=0;
            if ((fp=fopen(ID_FILE,"r"))!=NULL) {
				while (fread(&pf3,sizeof_pf,1,fp)) {
                    if (pf3.type==0) ast[4]++;
					else {
                        if (pf3.warn%10>0)        ast[5]++;      //경고
                        if (pf3.type>=14)         ast[6]++;
                        if (pf3.type>=18)         ast[14]++;
                        if (bit(pf3.paymode,1))   ast[7]++;        //유료
                        if (bit(pf3.paymode,2))   ast[8]++;        //연체
                        if (pf3.cash<0)           ast[10]++;      //파산회원
                        if (pf3.cash>99999999)    ast[11]++;      //재벌회원
					}
				}
				fclose(fp);
			}
			lastnow2=tnow;
		}
		lastnow=tnow;
	}

	strcpy(ss_1,"");strcpy(ss_2,"");strcpy(ss_3,"");
	strcpy(ss_1,bit(cfg.guestmode,15)?"자동가입":"수동가입");
	x=58;n=2;
    xy(x,n++);printf("현재접속자:%6d명 ",ast[1]);
    xy(x,n++);printf("총회원수  :%6d명 ",ast[2]);
    xy(x,n++);printf("가입대기자:%6d명 ",ast[3]);
    xy(x,n++);printf("삭제회원  :%6d명 ",ast[4]);
    xy(x,n++);printf("경고회원  :%6d명 ",ast[5]);
    xy(x,n++);printf("운영진수  :%6d명 ",ast[6]);
    xy(x,n++);printf("유료회원  :%6d명 ",ast[7]);
    xy(x,n++);printf("연체회원  :%6d명 ",ast[8]);
    xy(x,n++);printf("IP  회원  :%6d명 ",ast[9]);
    xy(x,n++);printf("파산회원  :%6d명 ",ast[10]);
    xy(x,n++);printf("재벌회원  :%6d명 ",ast[11]);
    xy(x,n++);printf("총게시판수:%6d   ",ast[12]);
    xy(x,n++);printf("총자료실수:%6d   ",ast[13]);
    xy(x,n++);printf("등급18이상:%6d명 ",ast[14]);
	xy(x,n++);printf("신규가입  :%s",ss_1);
	xy(x,n++);printf("D,W 제한  :%s",ss_2);
	xy(x,n++);printf("ID  입력  :%s",bit(cfg.guestmode,20)?"전부손님":"정상입력");
	xy(x,n++);printf("손님  ID  :%s",bit(cfg.guestmode,18)?"일련번호":cfg.guestid);
}

//접속인원수 계산
count_moni()
{
    int count=0,n;
    FILE *fp;
    char filename[100];
    for (n=0;n<20;n++) {
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename, "r"))==NULL) continue;
        while (fread(&moni,sizeof_moni,1,fp)) {
            if (moni.mode<1||strlen(moni.id)<3||strlen(moni.tty)<1) continue;
            count++;
        }
        fclose(fp);
    }
	return count;
}


new_cfg()
{
	strcpy(cfg.hostname,"xhost");
	strcpy(cfg.sysop,"sysop");
	cfg.waitkey=5;
	cfg.levelmax=5000;
	cfg.cashmax=900000000;
	cfg.cashmaxday=900000000;
	cfg.downfree=14;		 // 무제한 다운가능 타입
	cfg.cashfree=16;		 // 무제한 다운가능 타입
	cfg.billfree=16;		 // 무제한 다운가능 타입
	cfg.helplevel=100 ; 	 // 잠깐 도움말을 보여주는 레벨임
	cfg.helptype =1   ; 	 // 잠깐 도움말을 보여주는 레벨임
	cfg.helplog  =10  ; 	 // 잠깐 도움말을 보여주는 레벨임
	strcpy(cfg.guestid,"guest");         // 손님 접속 아이디
	cfg.guestlevel=10;		 // 초기 레벨
	cfg.guesttime=100;		 // 초기 시간 (분)
	cfg.guestdown=500;		 // 초기 다운용량
	cfg.guestcash=100;		 // 초기 금액
	cfg.chatwarn=100;		 // 체팅경고시간 (분)
	cfg.chattype=16;		 // 비공개 대화 진입 등급
}


//회원자료 확인하기
stat_pf()
{
	FILE *fp;
	if ((fp=fopen(ID_FILE,"r+"))!=NULL) {
		fseekend(fp);
		total=ftell(fp)/sizeof_pf;
		fclose (fp);
	}
}


disp_del_pf()
{
	FILE *fp;
	char buff1[100];
	char buff2[100];

	printf("\n 삭제처리 회원 확인\n");
	printfL2();
	fp=fopen(ID_FILE,"r+");
	while (!feof(fp)) {
		if (!fread(&pf,sizeof_pf,1,fp)) break;
		if (pf.type==0||pf.id[0]==0||pf.warn%10==9) {
            printf("%5d.%-8s(%-8s) %2d %5d %2d %s %s\n",pf.num+1,pf.id,pf.name,pf.type,pf.level,pf.warn%10,
			datestr(2,pf.inday),datestr(2,pf.logout));
		}
	}
	printfL2();
	if (fp!=NULL) fclose (fp);
	pressenter();
}


pack_del_pf(int mode)
{
	FILE *fp,*fp2;
	char buff1[100],buff2[100];
	int n,allyes,i;
	time_t tdate;

	allyes=No;
	fp=fopen(ID_FILE,"r+");
	if (mode==1||mode==2) {
		n=0;
		while (1) {
			fseek(fp,n*sizeof_pf,SEEK_SET);
            if (n<0||!fread(&pf,sizeof_pf,1,fp)) break;
			if (pf.type==0&&pf.id[0]!=0) {
                printf("%5d.%-8s(%-8s) %2d %5d 삭제처리(Y/n/q/a) >>",pf.num+1,pf.id,pf.name,pf.type,pf.level);
				if (allyes) i=1;
				else		i=yesno("",Yes);
				if (i==-2) {allyes=Yes;i=Yes;}
				if (i==-1) break;
				if (i) {
					fseek(fp,n*sizeof_pf,SEEK_SET);
					strcpy(pf.id,"");strcpy(pf.name,"");
//                    if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
                    if (n<0||!gwrite(205,pf,fp)) break;
					if (mode==2&&pf.id_no[0]!=0) {
						printf(" --> 주민등록 사용금지 처리");
						if ((fp2=fopen("system/bad_idno.txt","a+"))!=NULL) {
							fprintf(fp2,"%s\n",pf.id_no);
							fclose (fp2);
						}
					}
					else printf(" --> 삭제");
				}
				else printf(" --> 보류");
				printf("\n");
			}
			n++;
		}
	}
	else if (mode==3) {
		printf(" 최종접속삭제처리기준날짜  (예:96/12/31 ->96/12/31이전 전부 삭제)>>");
		lineinput(buff2,8);
		tdate=str2date(buff2);
		if (tdate>0) {
			n=0;
			while (1) {
				fseek(fp,n*sizeof_pf,SEEK_SET);
                if (n<0||!fread(&pf,sizeof_pf,1,fp)) break;
				if (pf.logout<tdate&&pf.login<tdate&&pf.inday<tdate&&pf.type>0&&pf.id[0]!=0) {
                    printf("%5d.%-8s(%-8s) %2d %5d 최종%s 삭제처리(Y/n/q/a) >>",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(2,pf.logout));
					if (allyes) i=1;
					else		i=yesno("",Yes);
					if (i==-2) {allyes=Yes;i=Yes;}
					if (i==-1) break;
                    if (i&&n>=0) {
						fseek(fp,n*sizeof_pf,SEEK_SET);
						pf.type=0;
//                        if (!fwrite(&pf,sizeof_pf,1,fp)) break;
                        if (!gwrite(206,pf,fp)) break;
						printf(" ->삭제");
					}
					else printf(" ->보류");
					printf("\n");
				}
				n++;
			}
		}
	}
	else if (mode==4) {
		n=0;
        while (1) {
			fseek(fp,n*sizeof_pf,SEEK_SET);
            if (n<0||!fread(&pf,sizeof_pf,1,fp)) break;
			if (pf.warn%10==9&&pf.type>0&&pf.id[0]!=0) {
                printf("%5d.%-8s(%-8s) %2d %5d 경고9 최종%s 삭제처리(Y/n/q/a) >>",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(2,buff2,pf.logout));
				if (allyes) i=1;
				else		i=yesno("",Yes);
				if (i==-2) {allyes=Yes;i=Yes;}
				if (i==-1) break;
                if (i&&n>=0) {
					fseek(fp,n*sizeof_pf,SEEK_SET);
					pf.type=0;
//                    if (!fwrite(&pf,sizeof_pf,1,fp)) break;
                    if (!gwrite(207,pf,fp)) break;
					printf(" ->삭제");
				}
				else printf(" ->보류");
				printf("\n");
			}
			n++;
		}
	}
	else if (mode==5) {
		printf("\n아직 지원되지 않습니다.\n");
	}
	if (fp!=NULL) fclose(fp);
	printf("\n .......... 자료 재정리 완료.............\n");
	pressenter();
}


renum_pf()
{
	FILE *fp;
	char buff2[100];
	int n;
	int old;

	printf("\n .......... 번호 재정리 중 ..............\n");
	fp=fopen(ID_FILE,"r+");
	n=0;
	while (1) {
		fseek(fp,n*sizeof_pf,SEEK_SET);
		if (!fread(&pf,sizeof_pf,1,fp)) break;
		old=pf.num;
        pf.num=n;
		if (old!=pf.num) {
			printf("### %-8s(%-8s) 번호 %6d ->%6d 로 변경 \n",pf.id,pf.name,old,pf.num);
			fseek(fp,n*sizeof_pf,SEEK_SET);
//          if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
            if (n<0||!gwrite(208,pf,fp)) break;
		}
		strcpy(buff2,pf.id);del_space(buff2);del_esc(buff2);
		if (!is_same(pf.id,buff2)) {
			printf("### %-8s -> %-8s(%-8s) 로 변경  \n",pf.id,buff2,pf.name);
			strcpy(pf.id,buff2);
			fseek(fp,n*sizeof_pf,SEEK_SET);
//            if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
            if (n<0||!gwrite(209,pf,fp)) break;
		}
		strcpy(buff2,pf.name);del_space(buff2);del_esc(buff2);
		if (!is_same(pf.name,buff2)) {
			printf("### %-8s -> %-8s(%-8s) 로 변경  \n",pf.name,buff2,pf.name);
			strcpy(pf.name,buff2);
			fseek(fp,n*sizeof_pf,SEEK_SET);
//            if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
            if (n<0||!gwrite(209,pf,fp)) break;
		}
		n++;
	}
	if (fp!=NULL) fclose(fp);
	printf("\n .......... 번호 재정리 완료.............\n");
	pressenter();
}


renum_proccode()
{
	FILE *fp;
	char buff2[100];
	int n,n2;
	int old;

	printf("\n");
	printf(" 이 기능은 회원의 발송/처리용 내부 식별코드를 재정리 하는 기능입니다.\n");
	printf(" 기존의 정보를 무시하고 새로 지정되므로 주의하여야 합니다.\n");
	printf(" 새로 재정리를 하시겠습니까? (y/N) >>");
	if (yesno("",No)==No) return;
	printf("\n 식별번호 부여시 시작번호를 지정하세요. (1~100000)>>");
	n2=numinput();
	if (n2<0) n2=1;
	if ((fp=fopen(ID_FILE,"r+"))!=NULL) {
		n=0;
		while (1) {
			fseek(fp,n*sizeof_pf,SEEK_SET);
			if (!fread(&pf,sizeof_pf,1,fp)) break;
			pf.idnum=n2;
			fseek(fp,n*sizeof_pf,SEEK_SET);
//            if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
            if (n<0||!gwrite(210,pf,fp)) break;
			n++;
			n2++;
		}
		fclose(fp);
	}
	printf("\n .......... 식별번호 재정리 완료.............\n");
	pressenter();
}




add_new_pf()
{
	char buff[100];
	char buff1[100];
	char buff2[100];
	char buff3[100];
	char buff4[100];
	char buff5[100];
	char buff6[100];
	int n,i;
	FILE *fp;
	while(1) {
		printf("\n");
		printfL2();
		printf("       신규 회원 수동으로 만들기    (Enter:종결)\n");
		printfL2();
        printf("만들 회원 아이디를   입력하세요     >>");
        lineinput(buff1,8);printf("\n");
        if (strlen(buff1)<2||is_quit(buff1)) break;
		if (read_pf2(buff1)) {
			printf(" 해당 회원의 아이디가 이미 존재합니다.  %s(%s)\n",pf2.id,pf2.name);
			continue;
		}
		printf("만들 회원 성명을  입력하세요  (8자) >>");lineinput(buff2,8);printf("\n");
        if (is_quit(buff2)) break;
		if (buff2[0]==0) strcpy(buff2,buff1);
		printf("만들 회원 등급을  입력하세요  (0-17)>>");lineinput(buff3,2);printf("\n");
        if (is_quit(buff3)) break;
		if (buff3[0]==0) strcpy(buff3,"1");
		printf("만들 회원 레벨을  입력하세요(0-5000)>>");lineinput(buff4,6);printf("\n");
        if (is_quit(buff4)) break;
		if (buff4[0]==0) strcpy(buff4,"100");
		printf("만들 회원 다운용량을 입력하세요 (MB)>>");lineinput(buff5,6);printf("\n");
        if (is_quit(buff5)) break;
		if (buff5[0]==0) strcpy(buff5,"5");
		printf("만들 회원 CASH를  입력하세요(냥)    >>");lineinput(buff6,9);printf("\n");
        if (is_quit(buff6)) break;
		if (buff6[0]==0) strcpy(buff6,"5000");
		strcpy(pf2.pos1 ,"");
		strcpy(pf2.addr ,"");
		strcpy(pf2.tele ,"");
		strcpy(pf2.offi ,"");
		strcpy(pf2.gofirst,"");
		strcpy(pf2.id_no,"");
		pf2.birthday=0;
		strcpy(pf2.mailid,"");
		strcpy(pf2.memo1,"");
		strcpy(pf2.memo2,"");
        strcpy(pf2.user1,"");
        strcpy(pf2.user2,"");
        strcpy(pf2.user3,"");
        strcpy(pf2.user4,"");
		strcpy(pf2.email,"");
		pf2.num=0;
		pf2.myflag=0xffffffff;
		pf2.sex='m';
		pf2.job= 0;
		time(&pf2.inday);
		time(&pf2.login);
		time(&pf2.logout);
		pf2.countw=0;
		pf2.all_time=0;
		pf2.all_log=0;
		pf2.month_time=0;
		pf2.month_log=0;
		pf2.level=10;
		strcpy(pf2.id,buff1);
		del_space(pf2.id);del_esc(pf2.id);strlwr(pf2.id);strcpy(pf2.name,buff2);
		strcpy(pf2.nick,buff2);
		pf2.type=atoi(buff3);
		pf2.level=atoi(buff4);
		pf2.down=atoi(buff5)*1000000;
		pf2.cash=atoi(buff6)*1000000;
		strcpy(pf2.passwd,pf2.id);
        if (pf2.type>17||pf2.type<1) pf2.type=1;
        view_pfw2(pf2,who);
		xy(1,20);
		if (yesno(" 신입회원 가입처리를 할까요? (Y/n)",Yes)) {
			fp=fopen(ID_FILE,"r+");
			if(fp!=NULL) {
				fseekend(fp);
				n=ftell(fp)/sizeof_pf; //현재 레코드수
                if (n>=0) {
                    pf2.num=n;               //마지막번호에 추가함
                    fseek(fp,n*sizeof_pf,SEEK_SET);
//                    fwrite(&pf2,sizeof_pf,1,fp);
                    gwrite(211,pf2,fp);
                }
				if (fp!=NULL) fclose(fp);
				sprintf(buff,"%s(%s) 회원이 저장되었습니다.",pf2.id,pf2.name);
				mess(buff);
			}
		}
	}
    if (yesno("인덱스 재정리를 할까요? (Y/n)>>",Yes)) make_ind(0,"make_ind_all");
}

id_num_edit()
{
	int n,n2;
	char buff[200];
	FILE *fp;

	fp=fopen(ID_FILE,"r+");
	fseekend(fp);
	n2=ftell(fp)/sizeof_pf; //현재 레코드수
	fclose(fp);
	printf("\n 수정할 회원번호를 입력하세요. (1-%d) >>",n2);
	lineinput(buff,8);printf("\n");
	n=atoi(buff);
	if (n<1||n>n2) return;
    n--;
    if (n<0) n=0;
	fp=fopen(ID_FILE,"r+");
	fseek(fp,n*sizeof_pf,SEEK_SET);
	fread(&pf2,sizeof_pf,1,fp);
	printf(" 회원번호:%5d  ID:%-8s  성명:%-8s  별명:%-8s  등급:%2d  레벨:%5d\n"
    ,pf2.num+1,pf2.id,pf2.name,pf2.nick,pf2.type,pf2.level);
	printf("# 회원 변경 ID  :(%8s) >>",pf2.id);
	get_str(pf2.id,8);printf("  ->%s\n",pf2.id);
	printf("# 회원 변경 성명:(%8s) >>",pf2.name);
	get_str(pf2.name,8);printf("  ->%s\n",pf2.name);
	fseek(fp,n*sizeof_pf,SEEK_SET);
    pf2.num=n;
//    fwrite(&pf2,sizeof_pf,1,fp);
    gwrite(212,pf2,fp);
    fclose(fp);
    make_ind(0,"make_ind_all");
}


check_pf(int mode,int who)
{
	int i=0;
	FILE *fp;
	char buff[100];
	int found=No;
	PF pf;

	printfL2();
	fp=fopen(ID_FILE,"r+");
	while (fp!=NULL&&fread(&pf,sizeof_pf,1,fp)>0) {
		if ( (mode==1&&pf.type==0)||(mode==2&&pf.type>=16)) {
			printf(" %-8s(%-8s)%2d ",pf.id,pf.name,pf.type);
			if (i++%3==2) printf("\n");
			found=Yes;
		}
	}
	fclose(fp);
	if (!found) i=0;
	printf("\n  총 %d 명이 검색되었습니다.\n",i);
	getchar();
}






sys_back()
{
	struct tm *tp;
	time_t today;
	int now_day;
	char filename1[100];
	char filename2[100];
	char buff[100];
	struct stat stbuf;
	time(&today);	//오늘의 날짜 지정
	tp = localtime(&today);
	now_day=tp->tm_mday;

    sprintf(filename1,"sys_back/id%d.tgz",now_day%10);
    sprintf(filename2,"sys_back/cfg%d.tgz",now_day%10);
	//마지막으로  데이타를 수정한시간
	if (!is_file(filename1)||is_old_file(filename1,24*60*60)) {
		job_backup();
	}
}




job_backup()
{
	char filename1[300];
	char filename2[300];
	char filename3[300];
	char buff[300];
    check_dir("sys_back");
    sprintf(filename1,"sys_back/id%d.tgz",now_day());
    sprintf(filename2,"sys_back/cfg%d.tgz",now_day());
	printf("\n\n");
	printf(" ID  -> %s 에 백업합니다.\n",filename1);
    printf(" CFG -> %s 에 백업합니다.\n",filename2);
    sprintf(buff,"tar cfvz %s id/id.dat & > /dev/NULL",filename1);
    system(buff);
    sprintf(buff,"tar cfvz %s system/bbscfg.dat system/syscmd.dat & > /dev/NULL",filename3);
    system(buff);
}





//전회원 세팅
all_set_level()
{
	int i,n,n2,nn;
	char buff[200];
	char buff1[200];
	char buff2[200];
	FILE *fp;
	while (1) {
		cls();
		printf(" ###  전체 회원 세팅\n");
        printf("  1.  전체 회원 전체 사용시간 0        세팅         \n" );
        printf("  2.  전체 회원 금월 사용시간 0        세팅         \n" );
		printf("  3.  전체 회원 전체 접속횟수 0        세팅         \n" );
        printf("  4.  전체 회원 금월 접속횟수 0        세팅         \n" );
        printf("  5.  전체 회원 다운량 조정                         \n" );
		printfL2();
		n=numselect();
        if (n<=0)  break;
        else if (n==1||n==2||n==3||n==4)  {
            printf("\n 지금부터 전체회원설정을 초기화합니다.\n");
            if (n==1) printf("전체사용시간");
            if (n==2) printf("금월사용시간");
            if (n==3) printf("전체접속횟수");
            if (n==4) printf("금월접속횟수");
            if (!yesno(" 초기화 세팅을 할까요?(y/N)>>",No)) continue;
			fp=fopen(ID_FILE,"r+");
			i=0;
			printf("\n");
			while (1) {
				if (!fread(&pf,sizeof_pf,1,fp)) break;
                printf("%-8s (%-8s)  ",pf.id,pf.name);
                if (n==1) {
                    printf("%8d -> 0\n",pf.all_time/60);
                    pf.all_time=0;
                }
                else if (n==2) {
                    printf("%8d -> 0\n",pf.month_time/60);
                    pf.month_time=0;
                }
                else if (n==3) {
                    printf("%8d -> 0\n",pf.all_log);
                    pf.all_log=0;
                }
                else if (n==4) {
                    printf("%8d -> 0\n",pf.month_log);
                    pf.month_log=0;
                }
                fseek(fp,i*sizeof_pf,SEEK_SET);
//                if (i<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
                if (i<0||!gwrite(212,pf,fp)) break;
				i++;
			}
			if (fp!=NULL) fclose (fp);
			pressenter();
        }
        if (n==5)  {
			xy(1,20);
			printf("전체회원 증가/감소 할 다운량(KB) >>");
			lineinput(buff,8);check_han(buff);printf("\n");
			n2=atoi(buff);
			if (n2==0) continue;
			xy(1,21);
			printf("    전체회원의 다운량을 %d KB를 증가/감소할까요? (Y/n)>>",n2);
			if (yesno("",Yes)==No) continue;
			fp=fopen(ID_FILE,"r+");
			i=0;
			printf("\n");
			while (1) {
				fseek(fp,i*sizeof_pf,SEEK_SET);
                if (i<0||!fread(&pf,sizeof_pf,1,fp)) break;
				printf("%-8s (%-8s)  %8d ->",pf.id,pf.name,pf.down/1000);
				pf.down+=n2*1000;
				if (pf.down<0) pf.down=0;
				printf("%8d KB\n",pf.down/1000);
				fseek(fp,i*sizeof_pf,SEEK_SET);
//                if (i<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
                if (i<0||!gwrite(212,pf,fp)) break;
				i++;
			}
			if (fp!=NULL) fclose (fp);
			pressenter();
		}
	}
}

//전회원 세팅
recover()
{
	int i,n,n2,nn,count;
	FILE *fp;
	printf("\n    삭제된 전체회원을 복구 처리 할까요? (Y/n)>>");
	if (yesno("",No)) {
		c_lock(1,ID_FILE);
		fp=fopen(ID_FILE,"r+");
		i=0;
		count=0;
		printf("\n");
		while (1) {
			fseek(fp,i*sizeof_pf,SEEK_SET);
            if (i<0||!fread(&pf,sizeof_pf,1,fp)) break;
			if (pf.type==0&&strlen(pf.id)>0) {
				printf("%-8s (%-8s)  복구처리\n",pf.id,pf.name);
				fseek(fp,i*sizeof_pf,SEEK_SET);
				pf.type=1;
                pf.num=i;
				fwrite(&pf,sizeof_pf,1,fp);
                gwrite(212,pf,fp);
				count++;
			}
			i++;
		}
        fclose (fp);
		c_lock(0,ID_FILE);
		printf(" 총 %d명의 복구 작업이 끝났습니다. \n",count);
		pressenter();
	}
}


save_cfg()
{
	FILE *fp;
	del_clear(cfg.guestid);del_clear(cfg.sysop);
    if (strlen(cfg.jpgsz)==0) strcpy(cfg.jpgsz,"7\n8[5;5H");
    if (strlen(cfg.sz)==0)    strcpy(cfg.sz,"sz");
    if (strlen(cfg.rz)==0)    strcpy(cfg.rz,"rz");
	if (strlen(cfg.guestid)<3) strcpy(cfg.guestid,"guest");
	if (strlen(cfg.sysop  )<3) strcpy(cfg.sysop  ,"guest");
    if (is_file("bbscfg.dat")) fp=fopen("bbscfg.dat","w+");
    else fp=fopen(BBS_CFG,"w+");
    if (fp!=NULL) {
        fwrite(&cfg,sizeof_cfg,1,fp);
		fclose (fp);
		touch_file(BBS_CFG);
	}
}
