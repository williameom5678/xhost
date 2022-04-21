////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 이 소스를 변경하여 상용으로 사용하실때에는  개발자의 승인을 받아야 합니다.
// 내용 : 상태 확인 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include <dirent.h>
#include <fcntl.h>


check_type(int mode)  //메뉴 가능 등급확인	 불가능 -> Yes
{
    char buff[300];
    char buff2[300];
	int old_club_mode=club_mode;
	int key=Yes;
	int passkey=Yes;
	int i,n,cc=0;
	char filename[300];

    if (mode=='i'||mode=='j') {
        sprintf(buff,"%s/logo/%s.sid",club_go.dir,go.code);
        if (pf.type<18&&is_file(buff)) {
            if (in_file2(buff,pf.id)) return No;
            sprintf(buff,"진입불가능");
            goto not_enter2;  //해당 코드 접속 금지 설정
        }
        sprintf(buff,"%s/logo/%s.sii",club_go.dir,go.code);
        if (pf.type<18&&in_file2(buff,pf.id)) {
            sprintf(buff,"진입불가능");
            goto not_enter2;  //해당 코드 접속 금지 설정
        }
    }

	if (mode=='i'&&pf.type<18&&!bbs_man(pf.id)) {
        if (bit(bcfg.imode,11)&&(pf.type<1||pf.sex!='f'))     {set_bbs_logo("lm1",buff);goto fail_enter;}
        if (bit(bcfg.imode,13)&&(pf.type<1||pf.sex=='f'))     {set_bbs_logo("lm2",buff);goto fail_enter;}
        if (bit(bcfg.imode,15)&&(pf.type<1||get_age(pf)<19))  {set_bbs_logo("lm3",buff);goto fail_enter;}
        if (bit(bcfg.imode,17)&&(pf.type<1||get_age(pf)>=19)) {set_bbs_logo("lm4",buff);goto fail_enter;}
	}

	if (mode=='w'&&pf.type<18&&!bbs_man(pf.id)) {
        if (bit(bcfg.imode,12)&&(pf.type<1||pf.sex!='f'))     {set_bbs_logo("lm5",buff);goto fail_enter;}
        if (bit(bcfg.imode,14)&&(pf.type<1||pf.sex=='f'))     {set_bbs_logo("lm6",buff);goto fail_enter;}
        if (bit(bcfg.imode,16)&&(pf.type<1||get_age(pf)<19))  {set_bbs_logo("lm7",buff);goto fail_enter;}
        if (bit(bcfg.imode,18)&&(pf.type<1||get_age(pf)>=19)) {set_bbs_logo("lm8",buff);goto fail_enter;}
	}

	if (is_same(go.code,"top")||is_same(go.code,"guest")||is_same(go.code,"cguest"))
		return No;
	if (pf.type>17||(club_mode&&cpf.type>17)||bbs_man(pf.id)) return No;

	strcpy(buff,"");
    if (club_mode) {
		if (mode=='i'&& ((bcfg.itype< 30&&bcfg.itype>now_type)||
						 (bcfg.itype>=30&&bcfg.itype-30>pf.type))) key=No;
		if (mode=='r'&& ((bcfg.rtype< 30&&bcfg.rtype>now_type)||
						 (bcfg.rtype>=30&&bcfg.rtype-30>pf.type))) key=No;
		if (mode=='w'&& ((bcfg.wtype< 30&&bcfg.wtype>now_type)||
						 (bcfg.wtype>=30&&bcfg.wtype-30>pf.type))) key=No;
		if (mode=='d'&& ((bcfg.dtype< 30&&bcfg.dtype>now_type)||
						 (bcfg.dtype>=30&&bcfg.dtype-30>pf.type))) key=No;
	} else {
		if (mode=='i'&&bcfg.itype>pf.type ) key=No;
		if (mode=='r'&&bcfg.rtype>pf.type ) key=No;
		if (mode=='w'&&bcfg.wtype>pf.type ) key=No;
		if (mode=='d'&&bcfg.dtype>pf.type ) key=No;
	}
	if (key&&!is_char("gy",go.type)) {
		if (mode=='r'&&pf.type<cfg.limit_r) key=No;
		if (mode=='w'&&pf.type<cfg.limit_w) key=No;
		if (mode=='d'&&pf.type<cfg.limit_d) key=No;
		if (!key) {
				 if (mode=='r') {cc=12;sprintf(buff,"읽기금지! %s이상 가능",atype[cfg.limit_r]);}
			else if (mode=='w') {cc=13;sprintf(buff,"쓰기금지! %s이상 가능",atype[cfg.limit_w]);}
			else if (mode=='d') {cc=14;sprintf(buff,"다운금지! %s이상 가능",atype[cfg.limit_d]);}
			else				{cc=11;sprintf(buff,"진입금지! %s이상 가능",atype[cfg.limit_r]);}
		}
	}
	if (!key&&mode=='r'&&bit(cfg.infomode,1)) key=Yes;
	if (!key&&mode=='w'&&bit(cfg.infomode,2)) key=Yes;
	if (!key&&mode=='d'&&bit(cfg.infomode,3)) key=Yes;

	if (key==No) {
		if (strlen(buff)<1) {
			if (is_char(EXETYPE,go.type)) {cc=11;sprintf(buff,"진입금지! %s이상 가능",club_mode?actype[bcfg.itype]:atype[bcfg.itype]);}
			else if (mode=='i') {cc=11;sprintf(buff,"진입금지! %s이상 가능",club_mode?actype[bcfg.itype]:atype[bcfg.itype]);}
			else if (mode=='r') {cc=12;sprintf(buff,"읽기금지! %s이상 가능",club_mode?actype[bcfg.rtype]:atype[bcfg.rtype]);}
			else if (mode=='w') {cc=13;sprintf(buff,"쓰기금지! %s이상 가능",club_mode?actype[bcfg.wtype]:atype[bcfg.wtype]);}
			else if (mode=='d') {cc=14;sprintf(buff,"다운금지! %s이상 가능",club_mode?actype[bcfg.dtype]:atype[bcfg.dtype]);}
			else				{cc=11;sprintf(buff,"진입금지! %s이상 가능",club_mode?actype[bcfg.dtype]:atype[bcfg.itype]);}
		}
		goto not_enter2;  //해당 코드 접속 금지 설정
	}

	if (key&&pf.type<16&&!bbs_man(pf.id)) {
        strcpy(buff,"");strcpy(buff2,"");
		if (pf.type>15);
		else if (!club_mode&&pf.type>14);
		else if (club_mode&&now_type>14);
        else if (mode=='i'&&bcfg.icash >0&&pf.cash<bcfg.icash ) sprintf(buff2,"%s CASH 부족(%d)",go.code,bcfg.icash);
        else if (mode=='i'&&bcfg.ilevel>0&&pf.cash<bcfg.ilevel) sprintf(buff2,"%s 레벨 부족(%d)",go.code,bcfg.ilevel);
		else if (mode=='j'&&check_no_code(go.code))
			sprintf(buff2,"%s 사용금지메뉴",go.code);
        else if (mode=='j'&&bcfg.itime>0&&check_intime(bcfg.itime,buff))
			sprintf(buff2,"%s 진입제한메뉴",buff);
        else if (mode=='d'&&bcfg.dtime>0&&check_intime(bcfg.dtime,buff))
			sprintf(buff2,"%s 다운제한메뉴",buff);
        else if (mode=='r'&&bcfg.rtime>0&&check_intime(bcfg.rtime,buff))
			sprintf(buff2,"%s 읽기제한메뉴",buff);
        else if (mode=='w'&&bcfg.wtime>0&&check_intime(bcfg.wtime,buff))
			sprintf(buff2,"%s 쓰기제한메뉴",buff);

        if (strlen(buff2)>0) {
			strcpy(buff,buff2);
			cc=3;
			goto not_enter2;  //해당 코드 접속 금지 설정
		}
		if (mode=='j'&&strlen(bcfg.passin)>0) {
			passkey=check_pass(bcfg.passin,8," ## 게시판 진입암호를 입력하세요 >>");
			cc= 2;
			sprintf(buff,"암호가 틀립니다. 정확한 게시판 암호를 입력하세요");
			key=No;
			goto not_enter2;  //해당 코드 접속 금지 설정
		}
	}
	if (key&&mode=='i'&&bcfg.li_w>0) {      //쓰기제한인가 검사
		n=get_pf_today(2);					//쓰기수검사
		printf("쓰기 수 확인 쓰기:%d  제한:%d\n",n,bcfg.li_w);fflush(stdout);
		if (n<bcfg.li_w) {
			sprintf(g_buff1,"%d",n);
			sprintf(g_buff2,"%d",bcfg.li_w);
			view("frm/error/limit_w.log",club_go.dir,cc);
			key=No;
		}
	}
	if (key&&mode=='i'&&go.type=='n') {
		if (pf.type<cfg.telnettype) {
			sprintf(g_buff1,"%d",cfg.telnettype);
			view("frm/error/teloff.log");
			key=No;
		}
	}

	cc=0;
	not_enter2:
	club_mode=old_club_mode;
    if (!key||!passkey||strlen(buff)>0) {
		printf("\n");
		printfL2();
		printf("%s\n",center_line(buff));
		printfL2();
		pressenter();
		if (mode=='i') go_top(1);       //동호회의 처음가기명령
		return Yes;
	}
	return No;

    fail_enter:
    view(buff);
    go_prev();
    return Yes;

}

//진입가능 시간 확인
check_intime(int intime,char *buff)
{
	int now,start,end;
	struct tm *tp;
	time_t today;
	time(&today);	//오늘의 날짜 지정
	tp = localtime(&today);
    //현재시간을 2359형태로
    now=(tp->tm_hour*100+tp->tm_min)%10000;
	start=intime/10000;  //시작시간
	end  =intime%10000;  //종결시간
    strcpy(buff,"");
    if (now>=start&&now<=end) return No;
	strcpy(buff,time_limitstr(intime));
    return Yes;
}


//노드의 초기화 작업을 한다.
clear_node()
{
	char buff[80];
	set_tmpfile();
	unlink(TMP_FILE);
	unlink(TMP_FILE1);
	unlink(TMP_FILE2);
	unlink(LT_FILE);
}


//현재의 아이디는 현재의 게시판을 삭제,이동 가능한가?
check_del()
{
	if (now_type>14||pf.type>14||
		 is_same(bbs.id,pf.id)||			//자기가 쓴 게시판의 경우
		 bbs_man(pf.id))	return Yes; //게시판의 담당자의 경우
	return No;
}


//현재의 아이디는 현재의 게시판을 삭제,이동 가능한가?
check_del2()
{
	if (now_type>=14||pf.type>=14||
		 is_same(bbs.id,pf.id)||			//자기가 쓴 게시판의 경우
		 bbs_man(pf.id))	return Yes; //게시판의 담당자의 경우
	return (No);
}


check_addr()
{
	char buff[50];
	char buff2[150];
	strcpy(buff,"");
	if (guest_mode||pf.type>=14) return;
	if (strlen(pf.tele)<5) strcpy(buff,"전화");
	if (strlen(pf.addr)<20) strcpy(buff,"주소");
	if (buff[0]!=0&&rand()%5!=1) {
		sprintf(buff2,"*** %s를 정확하게 적어주세요 ***",buff);
		message(buff2);
	}
}



check_wait(int mode)	  //기다림 지정하기 0:검색 1:지정
{
	char filename[200];
	WAITFOR waitfor;
	int i,i2,n,nn,found;
	FILE *fp;
	char buff[200];
	char messbuff[200];
	char tid[120];
	char aid[20][100];
	char id[100];
	re_print=Yes;

	if (guest_mode) return;
	strcpy(filename,"system/waitfor.dat");
	printf("\n");

	if (mode==0) {
		if((fp=fopen(filename,"r+"))==NULL) return;
		n=0;
		i=0;
		while (fread(&waitfor,sizeof(waitfor),1,fp)) {
//			  printf("%s %s %s\n",waitfor.id1,waitfor.id2,waitfor.title);getchar();
			if (waitfor.mode>0&&is_same(waitfor.id1,pf.id)) {
				if (n++==0) printf("\n ====================  %s 님을 기다리고 있었습니다. ===================\n",pf.id);
				strcpy(buff,datestr(3,waitfor.date));
				printf(" %-8s %s :%s\n",waitfor.id2,buff,waitfor.title);
				sprintf(buff,"\n #### %s(%s) 님이 접속하셨습니다. ####\n",pf.id,pf.nick);
				if (bit(cfg.mailmode,5)) strcat(buff,"\007\007");
				if (!to_moni(waitfor.id2,0,buff)) to_memo(0,waitfor.id2,buff);
				fseek(fp,i*sizeof(waitfor),SEEK_SET);
				waitfor.mode=0;
				fwrite(&waitfor,sizeof(waitfor),1,fp);
				fseek(fp,(i+1)*sizeof(waitfor),SEEK_SET);
			}
			i++;
		}
		if (fp!=NULL) fclose(fp);
		if (n>0) {
			printfL2();
			pressenter();
			printf("\n\n\n\n");
		}
		return;
	}
	else {
		printf("\n");
		strcpy(mmc[0],"");
        if (mmc[1][0]==0||!read_pf4(mmc[1])) {
			while (1) {
				printf("\n 기다릴 회원의 아이디를 입력하세요 ( 여러명지정가능, q:취소 검색:회원찾기)\n>>");
				lineinput(buff,70);
				printf("\n");
				if (is_same(buff,"q")) return;
				if (is_same(buff,"검색")) {
					sprintf(buff,"bin/findname main %s",pf.id);sys_tem(buff);
					continue;
				}
				check_cmd(buff);
				break;
			}
		}

		printf("  기다릴 회원이 접속할때 보여줄 메세지를 입력하세요.  (1줄, q:취소)\n>>");
		lineinput(messbuff,50);
		printf("\n");
		if (is_same(messbuff,"q")) return;

		touch_file(filename);

		for (i2=0;i2<10;i2++) {
			strcpy(aid[i2],"");
		}
		nn=0;
		for (i2=0;i2<10;i2++) {
			strcpy(tid,mmc[i2]);
			if (tid[0]==0)		continue;
            if (!read_pf4(tid)) continue;
			if (is_moni(tid)) {
				printf("\n *****%s(%s)님은 이미 접속하고 계십니다.\n",pf2.id,pf2.name);
				continue;
			}
			found=No;
			for(i=0;i<10;i++) if (is_same(aid[i],tid)) found=Yes;
			if (found) continue;

			fp=fopen(filename,"r+");
			i=0;
			while (fread(&waitfor,sizeof(waitfor),1,fp)) {
				if (waitfor.mode==0||(START_TIME-waitfor.date)>20*24*60*60) {
					found=Yes;
					break;
				}
				i++;
			}
			if (!found) {
				fseekend(fp);
				i=ftell(fp)/sizeof(waitfor);
			}
			fseek(fp,i*sizeof(waitfor),SEEK_SET);
			strcpy(waitfor.id1,pf2.id);
			strcpy(waitfor.id2,pf.id);
			waitfor.mode=1;
			strcpy(waitfor.title,messbuff);
			time(&waitfor.date);   //오늘의 날짜 지정
			fwrite(&waitfor,sizeof(waitfor),1,fp);
			if (fp!=NULL) fclose(fp);
			printf("     %s 님을 기다림 처리 하였습니다.\n",waitfor.id1);
			strcpy(aid[i2],waitfor.id1);
			nn++;
		}
		if (nn<1) mess("기다림 처리 한 회원이 없습니다.");
		else	  mess("기다림 처리 완료");
	}
}


check_not_sn()			//사용금지등록번호 리스트
{
	// X0X-0000  <- X자리에는 0 이 들어가면 임시버젼으로 됨
	//스피드넷				  / 70200004  -> 취소
}


//사용가능 확인 절차
//1:쓰기 2:등록 3:다운 4:텔넷 5:외부 6:동호회진입 7:읽기 8.대화방
ss_check(int mode)
{
	int n;
	int ret=No;
	char buff[100];

	/*
	n=pf.type;
	strcpy(buff,"");
	if (guest_mode) n=0;
	if (mode==1&&n<bcfg.riteoff) {strcpy(buff,"logo/_downoff.log");ret=Yes;}
	if (mode==2&&n<cfg.upoff)	 {strcpy(buff,"logo/_downoff.log");ret=Yes;}
	if (mode==3&&n<cfg.downoff)  {strcpy(buff,"logo/_downoff.log");ret=Yes;}
	if (mode==5&&n<cfg.runoff)	 {strcpy(buff,"logo/_runoff.log" );ret=Yes;}
	if (mode==6&&n<cfg.cluboff)  {strcpy(buff,"logo/_downoff.log");ret=Yes;}
	if (mode==7&&n<cfg.readoff)  {strcpy(buff,"logo/_readoff.log");ret=Yes;}
	if (mode==8&&n<cfg.chatoff)  {strcpy(buff,"logo/_chatoff.log");ret=Yes;}
	if (mode==1&&bit(cfg.infomode,1)) ret=No;
	if (mode==2&&bit(cfg.infomode,2)) ret=No;
	if (mode==3&&bit(cfg.infomode,3)) ret=No;
	if (pf.type>=18) ret=No;
	if (ret&&buff[0]!=0) {
		if (!logo(buff,Yes)) {
			mess("등급이 부족하여 사용이 불가능합니다.");
		}
	}
	*/
	return ret;
}


check_warn(int mode)
{
	char warn_ok=No;
	char can_write=Yes;
	char can_dn=Yes;
	char can_chat=Yes;
	char can_club=Yes;
	char buff[200];
	int ret;
	int n=0;

	if (guest_mode) return No;
	n=pf.warn;
	if (club_mode) {
		n=cpf.warn;
		if (pf.warn%10>=7) n=pf.warn;
	}

	if(n%10<4) return No;
	if(n%10==4) {can_write=No;}
	if(n%10==5) {can_write=No;can_dn=No;}
	if(n%10==6) {can_write=No;can_dn=No;can_chat=No;}
	if(n%10==7) {can_write=No;can_dn=No;can_chat=No;can_club=No;}
	if(n%10==8) {can_write=No;can_dn=No;can_chat=No;can_club=No;strcpy(auto_cmd,"go _warn");}
    if(n%10==9&&!club_mode) {
        can_write=No;can_dn=No;can_chat=No;can_club=No;
        pf.down=0;pf.level=0;
        if (pf.type>0) save_pf(33);
    }
	strcpy(buff,"");
	ret=Yes;
	if		(mode==1&&!is_char("guy",go.type)&&is_typebbs&&!can_write) sprintf(buff,"%s님은 경고처리로 쓰기 금지 입니다.",pf.id);
	else if (mode==1&&!is_char("guy",go.type)&&is_typepds&&!can_write) sprintf(buff,"%s님은 경고처리로 등록 금지 입니다.",pf.id);
	else if (mode==2&&!can_chat)				sprintf(buff,"%s님은 경고처리로 대화방 사용 금지 입니다.",pf.id);
	else if (mode==3&&!can_dn)					sprintf(buff,"%s님은 경고처리로 다운 금지 입니다.",pf.id);
	else if (mode==4&&club_mode&&!can_club) 	sprintf(buff,"%s님은 경고처리로 동호회사용금지입니다.",pf.id);
	else if (mode==9&&n%10==8)					sprintf(buff,"%s님은 경고처리중입니다.",pf.id);
	else if (mode==9&&n%10==9)					sprintf(buff,"%s님은 경고처리중입니다.",pf.id);
	else ret=No;
	if (ret&&buff[0]!=0) if (buff[0]!=0) mess(buff);
	if (ret&&club_mode&&!can_club)	go_top(0);
	if (n%10==9) strcpy(auto_cmd,"go _warn");
	return ret;
}


//암호수정	mode=0 : 신규  mode=1:수정
check_bbspasswd()
{
	char buff[80];
	if (is_char("lfzo-+",go.type)==No) return Yes;
	if (strlen(bbs.passwd)<1||bit(bcfg.mode,31)) return Yes;

	if (pf.type>15||now_type>15||bbs_man(pf.id)) return Yes;
	printf("\n이 자료에 등록자 %s님이 암호를 지정하였습니다.\n",bbs.id);
	printf("\n현재의 자료에 지정된 암호를 입력하세요: (4자)>>");
	if (check_del()) printf("(암호:%s)",bbs.passwd);
	lineinput(buff,4);
	if (is_same(bbs.passwd,buff)==No) {
		message("암호가 틀립니다.");
		list_mode=No;	   //내용츌력모드
		re_print=Yes;
		return No;
	}
	return Yes;
}

//bbs,go에서 처리함
check_achive()
{
	FILE *fp;
	char buff[200];
	char buff2[200];
	char filename[200];
	char filename2[200];
	char akey[99][5];
	char amode[10];
	char OLDDIR[300];
	int i,i2;

	getcwd(OLDDIR,sizeof(OLDDIR));		//현재의 디렉토리
	if (bit(cfg.downmode,10)) return;			   //압?? 지원안함
	sprintf(filename,"%s/%s",go.dir,bbs.filename);
	if (!is_file(filename)) return;
	if (file_size(filename)<1024) return;		 //압?? 지원안함
	if (stristr(bbs.filename,".rar")) return;
	if (stristr(bbs.filename,".zip")) return;
	if (stristr(bbs.filename,".arj")) return;
	if (stristr(bbs.filename,".lzh")) return;
	if (stristr(bbs.filename,".tgz")) return;
	if (stristr(bbs.filename,".exe")) return;

	//확장자들 읽기
	fp=fopen("system/achive.txt","r+");
	if (fp==NULL) return;
	for (i=0;i<99;i++) strcpy(akey[i],"");
	i=0;
	while (i<99&&fgets(buff,200,fp)!=NULL) {
		strcpy(akey[i],buff);
		del_esc(akey[i]);
		del_space(akey[i]);
		i++;
	}
	if (fp!=NULL) fclose(fp);

	for (i=0;i<99;i++) {
		if (akey[i][0]!=0) {
			strcpy(buff,".");
			strcat(buff,akey[i]);
			if (stristr(bbs.filename,buff)) return;  //해당확장자임
		}
	}
	printf("\n### %s(%d byte) 파일은 압축되지 않았습니다.\n",bbs.filename,file_size(filename));
	if (!yesno("### 압축할까요?  (Y/n) >>",Yes)) return;
	printf("\n");
	strcpy(buff,bbs.filename);
	for (i=0;i<8;i++)
		if (buff[i]=='.') buff[i]=0;
	buff[8]=0;
	strcpy(amode,"");
	sprintf(buff2,"%s/%s.arj",go.dir,buff);
	if (!is_file(buff2)) strcpy(amode,"arj");
	sprintf(buff2,"%s/%s.lzh",go.dir,buff);
	if (!is_file(buff2)) strcpy(amode,"lzh");
	sprintf(buff2,"%s/%s.zip",go.dir,buff);
	if (!is_file(buff2)) strcpy(amode,"zip");
	sprintf(buff2,"%s/%s.rar",go.dir,buff);
	if (!is_file(buff2)) strcpy(amode,"rar");
	if (amode[0]==0) {
		printf("\n 같은 이름의 압축화일이 있습니다.\n");
		pressenter();
		return;
	}
	sprintf(filename2,"%s.%s",buff,amode);
	filename2[12]=0;
	strcpy(buff,"");
    ch_dir(go.dir);
	if (!is_file(bbs.filename)) {
        ch_dir(OLDDIR);
		return;
	}
	if (is_same(amode,"rar")) sprintf(buff,"rar a %s %s",filename2,bbs.filename);   //압축시작
	if (is_same(amode,"zip")) sprintf(buff,"zip   %s %s",filename2,bbs.filename);   //압축시작
	if (is_same(amode,"lzh")) sprintf(buff,"lha a %s %s",filename2,bbs.filename);   //압축시작
	if (is_same(amode,"arj")) sprintf(buff,"arj a %s %s",filename2,bbs.filename);   //압축시작
	sys_tem(buff);
	printf("\n");
	if (!is_file(filename2)) {
        ch_dir(OLDDIR);
		printf("\n%s 압축 에 실패했습니다.\n",buff2);
		pressenter();
		return;
	}
	unlink(bbs.filename);
    ch_dir(OLDDIR);
	strcpy(bbs.filename,filename2);
	bbs.filesize=file_size(buff2);
}



//단축명령키 지정
check_short(int mode,char *buffstr)
{
	FILE *fp,*fp2;
	char abuff[5][70];
	char kbuff[100][11];
	char sbuff[100][51];
	char buff[250],buff1[250],buff2[100],buff3[100],buff4[100];
	char filename[200];
	int i,i2,found=No;

	if (mode!=0&&strlen(buffstr)<2) return No;
	sscanf(buffstr,"%s",buff1);
	del_space(buff1);
	if (mode!=0&&strlen(buff1)>1) {
		if (stristr("lt li ln lk le lg lu lsc lsl lst",buff1)) return No;
		if (stristr("pf say to mm",buff1)) return No;
	}

	if (mode==2) sprintf(filename,"userdata/%s/%s/pfinfo.sho",c_han(pf.id),pf.id);
	else		 strcpy(filename,"system/shortcmd.dat");
	if((fp=fopen(filename,"r"))==NULL) return No;

	for (i=0;i<100;i++) strcpy(kbuff[i],"");
	for (i=0;i<100;i++) strcpy(sbuff[i],"");
	i=0;
	while (fgets(buff,200,fp)&&i<100) {
		if (buff[0]==0) break;
		if (buff[0]=='#') continue;
		strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");strcpy(buff4,"");
		sscanf(buff,"%s%s%s%s",&buff1,&buff2,&buff3,&buff4);
		sprintf(buff,"%s %s %s",buff2,buff3,buff4);
		strcpy(kbuff[i],buff1);
		strcpy(sbuff[i],buff);
		i++;
	}
	if (fp!=NULL) fclose(fp);

	if (mode==0) {
		if((fp2=fopen(TMP_FILE,"w"))==NULL) return No;
		fprintf(fp2,"%20s   한글 명령어 리스트\n",cfg.hostname);

		for (i=0;i<100&&kbuff[i][0]!=0;i++) {
			fprintf(fp2,"%10s -> %-25s ",kbuff[i],sbuff[i]);
			if (i%2==1) fprintf(fp2,"\n");
		}

		re_print=Yes;
		if (fp2!=NULL) fclose(fp2);
		cls();
		view_text(TMP_FILE);
		return No;
	}

	strcpy(abuff[0],"");strcpy(abuff[1],"");strcpy(abuff[2],"");strcpy(abuff[3],"");strcpy(abuff[4],"");
	sscanf(buffstr,"%s%s%s%s%s",abuff[0],abuff[1],abuff[2],abuff[3],abuff[4]);
	for (i=0;i<5;i++) {
		for (i2=0;abuff[i][0]!=0&&i2<100;i2++) {
			if (is_same(abuff[i],kbuff[i2])) {
				strcpy(abuff[i],sbuff[i2]);
				found=Yes;
			}
		}
	}
	if (found) sprintf(buffstr,"%s %s %s %s %s",abuff[0],abuff[1],abuff[2],abuff[3],abuff[4]);
	return found;
}


check_no_code(char *tcode)		   //해당 코드 접속 금지 설정
{
	char filename[200];
	FILE *fp;
	int found=No;
	struct CCODE {
		char mode;
		char code[9];
	} ccode;

	sprintf(filename,"%s/system/_nocode.dat",club_go.dir);
	if ((fp=fopen(filename,"r"))!=NULL) {
		while (fread(&ccode,sizeof(ccode),1,fp)&&!found)
			found=is_same(ccode.code,tcode);
		fclose(fp);
	}
	if (found&&ccode.mode>0) {
		sprintf(filename,"%s/logo/_nocode%d.log",club_go.dir,ccode.mode);
		if (view(filename)) {
			getchar();
		}
		/*
			 if (ccode.mode==1) go.wtype=19;
		else if (ccode.mode==2) {go.wtype=19;go.dtype=19;}
		else if (ccode.mode==3) {go.wtype=19;go.dtype=19;go.rtype=19;}
		else					return ccode.mode;
		*/
	}
	return No;
}



check_login_init()
{
	char OLDDIR[200];
	getcwd(OLDDIR,140); 	 //현재의 디렉토리
	if (!is_file("id/id.dat")) {
		printf("\n## ID검색실패## 현재의 디렉토리: %s\n",OLDDIR);
		printf("## 쉘에서 BBS를 동작하려면 bin/main 또는 telnet localhost\n");
		printf("## id/id.dat 검색실패\n");
		exit(1);
	}
	check_dir("user");
	check_dir("user/data");
	check_dir("system");

}


check_warn_mess()				  //경고인가?
{
	char filename[200];
	char buff2[200];
	int n;
	n=(club_mode)?cpf.warn:pf.warn;
	if (n==0) return;
//	  sprintf(buff2,"%s님 [경고%d] 사유:%s",pf.id,n%10,buff);
	sprintf(filename,"%s/frm/_warn%d.txt",club_go.dir,n%10);
	view(filename);
	if (club_mode) strcpy(sub_go,"_warn"); //자동가기코드
	else		   sprintf(auto_cmd,"go _warn");
}


check_zbtotal(int sec,int mode)
{
	char buff[100];
	return;
	//전체자료실 임시로 막음
	strcpy(buff,"tmp/zbtotalp.tmp");
	if (mode==0&&(!is_file(buff)||is_old_file(buff,sec))) {
		sys_tem2("bin/zbtotal '%s' -f&",club_go.code);
		return;
	}
	strcpy(buff,"tmp/zbtotalp.tmp");
	if (mode==1&&(!is_file(buff)||is_old_file(buff,sec))) {
		sys_tem2("bin/zbtotal '%s' -m1 -f&",club_go.code);
	}
	strcpy(buff,"tmp/zbtotalb.tmp");
	if (mode==2&&(!is_file(buff)||is_old_file(buff,sec))) {
		sys_tem2("bin/zbtotal '%s' -m2 -f&",club_go.code);
	}
}


check_maxmenu()
{
    FILE *fp;
    int count1,count2;
    int key=No;
    GO tgo;
    if (!club_mode) return;
    if (cpf.type<2||cpf.type>13||pf.type>13) return;
    if (rand()%5) return;
    if (ccfg.maxmenu==0&&ccfg.maxpdsmenu==0) return;
    sprintf(NOW_MENU ,"%s/%s",club_go.dir,MENU_FILE);
    count1=count2=0;
    if ((fp=fopen(NOW_MENU,"r"))==NULL) return;
    while(fread(&tgo,sizeof_go,1,fp)) {
        if (strlen(tgo.code)<1||tgo.type==' ') continue;
        if (ccfg.maxmenu>0) {
            count1++;
            if (count1>ccfg.maxmenu) {key=Yes;break;}
        }
        if (ccfg.maxpdsmenu>0&&is_char(PDSTYPE,tgo.type)) {
            count2++;
            if (count2>ccfg.maxpdsmenu) {key=Yes;break;}
        }
    }
    fclose(fp);
    if (key) {
        mess("동호회 최대 메뉴수 추가로 진입이 안됩니다.");
        if (cpf.type<15) strcpy(auto_cmd,"go top");
    }
}




//접속인원수 계산
count_moni()
{
    int count=0,n;
    FILE *fp;
    char filename[100];
    MONI moni;
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

check_samemoni()		//동시접속파악
{
	FILE *fp;
	MONI moni2;
    char buff[300],buff2[20],filename[100];
    int n,count=0;
    if (pf.type<=13) return;
    strcpy(buff,"");
    for (n=0;n<cfg.count_server;n++) {
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename, "r"))==NULL) continue;
        while (fread(&moni2,sizeof_moni,1,fp)) {
            if (moni2.mode>0&&strlen(moni2.tty)>0&&is_same(moni2.id,pf.id)) {
				count++;
                sprintf(buff2,"%s(%d) ",moni2.tty,moni2.hostno);
                strcat(buff,buff2);
			}
        }
        fclose(fp);
	}
    if (count>1&&!bit(cfg.mode,7)) {
        printf(" ##동시접속:%s##\n",buff);
        if (is_same(go.code,"top")&&pf.type<cfg.relog) {
            mess("동시접속으로 접속이 종료됩니다.");
            host_end();
        }
    }
}


check_all_limit()    //전체 제한 설정
{
    FILE *fp;
    int i,found=No;
    char buff[100],buff1[100],buff2[100],buff3[100],filename[200];
    if (pf.type<0||pf.type>17||strlen(pf.id)<3) return;
    sprintf(filename,"user/limit/limit_%u.id",(word)pf.id[0]%255);
    if ((fp=fopen(filename,"r"))==NULL) return;
    while(!found&&fgets(buff,20,fp)!=NULL) {
        sscanf(buff,"%s %s %s",buff1,buff2,buff3);
        if (strlen(buff)>3&&is_same(buff1,pf.id)) {
            strlwr(buff2);
            if (strlen(buff2)==0) strcpy(buff2,"w");
            for (i=0;i<(int)strlen(buff2);i++) {
                if (buff2[i]=='w') limit_mode=set_bit(limit_mode,1,1);
                if (buff2[i]=='d') limit_mode=set_bit(limit_mode,2,1);
                if (buff2[i]=='r') limit_mode=set_bit(limit_mode,3,1);
                if (buff2[i]=='s') limit_mode=set_bit(limit_mode,4,1);
                if (buff2[i]=='c') limit_mode=set_bit(limit_mode,5,1);
                found=Yes;
                break;
            }
            break;
        }
    }
    fclose(fp);
}
