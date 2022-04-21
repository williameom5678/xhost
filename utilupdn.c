
sz(char *filename)
{
	int n;
    if (strlen(cfg.sz)<1) strcpy(cfg.sz,"sz");
    if (strlen(filename)<1) n=-1;
	else n=sys_tem("%s %s",cfg.sz,filename);
	printf("\n");
    fflush(NULL);
	if (bit(cfg.downmode,22)) n=0;
	return n;
}

rz()
{
	int n;
    if (strlen(cfg.rz)<1) strcpy(cfg.rz,"rz -e");
	n=sys_tem("%s",cfg.rz);
	printf("\n");
    fflush(NULL);
	if (bit(cfg.downmode,22)) n=0;
	return n;
}


kermitsz(char *filename)
{
}
kermitrz()
{
}


sz_file(char *filename)
{
	int n,n2,ret;
	char buff[280];

	if (!is_file(filename)) goto _fail;
	strcpy(g_buff4,pf.type==19?"9.DNS  ":"");
	view("frm/down/select.log");
	lineinput(buff,2);check_han(buff);printf("\n");n2=atoi(buff);
	if (strlen(buff)<1) n2=1;
	if (is_char("nNqQxXpP0",buff[0])) goto _fail;
	set_statdown(8);	   //메세지 수신방지
	if (n2==2)		{view("frm/down/kermit.log");kermitsz(filename);}
	else if (pf.type==19&&n2==9) {
		_cont4:
		view("frm/down/dns.log");
		dosinput(DEF_DNS,50);check_han(DEF_DNS);printf("\n");
		if (strlen(DEF_DNS)<2) goto _fail;
		if (!is_dir(DEF_DNS)||in_file2("system/cannot.dir",DEF_DNS)) {
			printf(" 디렉토리를 찾을수 없습니다.\n");
			goto _cont4;
		}
		sys_tem("cp -f %s %s",filename,DEF_DNS);
	}
	else			{view("frm/down/zmodem.log");sz(filename);      }
	view("frm/down/downok.log");   //전송취소
	set_statdown(OLD_MONI_MODE);	   //메세지 수신방지
	return Yes;

	_fail:
	view("frm/down/downfaip.log");   //전송취소
	set_statdown(OLD_MONI_MODE);	   //메세지 수신방지
	return No;
}

rz_file(char *filename)  //화일전송받기  ->파일명에 저장
{
	int n,n2,ret;
	char olddir[280];
	char newdir[280];
	char buff[280];
	char rzfilename[280];

	_cont:
	strcpy(filename,"");
	getcwd(olddir,sizeof(olddir));		//현재의 디렉토리
	strcpy(g_buff4,"");
	if (bit(allmode,14)||(pf.type==18&&!bit(cfg.downmode,17))||(pf.type==19&&!bit(cfg.downmode,18)))
		strcpy(g_buff4,"99.UPS");
	view("frm/down/upload.log");
	lineinput(buff,4);check_han(buff);n2=atoi(buff);
	if (strlen(buff)<1) n2=1;
	if (is_char("nNqQxXpP0",buff[0])) goto _fail;
	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	if (c_proc) printf("NEWDIR:%s\n",newdir);
	rm_tmpup();
	if (n2==1||n2==2) {
		set_statdown(6);	  //메세지 수신방지
		check_dir(newdir);
		if (n2==1) {
			view("frm/down/uploadz.log");
            ch_dir(newdir);
			rz();
			if (c_proc) {
				getcwd(buff,sizeof(buff));		//현재의 디렉토리
				printf("현재위치:%s  전송완료\n",buff);
				system("ls -l");
				pressenter();
			}
		}
		else {
			view("frm/down/uploadk.log");
            ch_dir(newdir);
			if (c_proc) {
				getcwd(buff,sizeof(buff));		//현재의 디렉토리
				printf("현재위치:%s\n",buff);
			}
			kermitrz();
		}
		set_statdown(OLD_MONI_MODE);	   //메세지 수신방지
//        sys_tem("%s/bin/xlower",olddir);      //현재의 디렉토리
		if (dir_filename(rzfilename,"./")>0&&strlen(rzfilename)>0)
			sprintf(filename,"tmp/up%d%s/%s",cfg.hostno,no_spc(tty),rzfilename);
        ch_dir(olddir);              //원래 디렉토리
	}
	else if (n2==88) {
		autoup=autoup?No:Yes;
		goto _cont;
	}
	else if (n2==99&&(bit(allmode,14)||(pf.type==18&&!bit(cfg.downmode,17))||(pf.type==19&&!bit(cfg.downmode,18)))) {
		up_file(99,filename,Yes);	//시삽 지정 업
	}
	else goto _fail;

	if (autoup) nostop=Yes; 	//자동등록
	view("frm/down/uploado.log");   //Enter키
	strcpy(buff,filename);
	n=file_size(filename);
	if (c_proc) {
		getcwd(buff,sizeof(buff));		//현재의 디렉토리
		printf("파일명:%s 크기:%d\n",filename,n);
	}
	if (strlen(filename)<1||!is_file(filename)||n<1) goto _fail;
	pure_name(0,buff);				//화일명에서 순수한 화일부분만 뽑아냄
	strcpy(g_buff1,buff);			//화일명
	sprintf(g_buff2,"%d",n);        //화일크기
	if (autoup) nostop=Yes; 	//자동등록
	view("frm/down/uploade.log");   //전송완료
	if (!good_filename(filename)) goto _fail;
	if (autoup||yesno("",Yes)) chmod(filename,0777);
	else goto _fail;
	return Yes;

	_fail:
    ch_dir(olddir);      //원래 디렉토리
	view("frm/down/uploadc.log");   //전송취소
	if (is_file(filename)) unlink(filename);
	rm_tmpup();
	return No;
}


//TMP_FILE 에 적는다.  mode=99:UPS저장	mode==98:비게시판내용입력
input_tmp(int mode)
{
	char buff[100];
	int ret=Yes,n=0;
	strcpy(buff,"");
	if (mode==99)				 n=99;
	else if (bit(bcfg.wmode,19)) n=3;
	else if (bit(bcfg.wmode, 7)) n=1;
	else {
		strcpy(g_buff4,pf.type>=18?"99.UPS":"");
        if (!view("frm/editor/select.%s",mode==98?".log":bbs_tail())) {
            printf(" 1.화면편집기 2.라인편집기 3.설명Zmodem 4.Kermit %s 0.취소\n",g_buff4);
            printf(" 번호선택(ENTER:화면편집기) >> ");
        }
		lineinput(buff,10);check_han(buff);printf("\n");
		n=atoi(buff);
		if (is_char("qp0QPxX",buff[0])) goto _fail;
	}

	unlink(TMP_FILE);
		 if (n== 3) ret=up_text(1,TMP_FILE);  //다운받아서지정화일로 바꾼다.
	else if (n== 4) ret=up_text(2,TMP_FILE);  //다운받아서지정화일로 바꾼다.
	else if (n==99) ret=up_text(99,TMP_FILE);
	else if (n== 2&&bit(cfg.bbsmode,6)) ret=editor2(TMP_FILE);
	else if (n== 2) ret=bbs_editor(2);
	else			ret=bbs_editor(1);

	if (ret==No||!is_file(TMP_FILE)) goto _fail;	  //취소시
	return ret;

	_fail:
	unlink(TMP_FILE);
	return No;
}


bbs_editor(int mode)
{
	int key;
	char buff[60];
	FILE *fp1;
	FILE *fp2;
	int i;
	char c;
	int ret=Yes;
	if (strlen(TMP_FILE)<1) {printf("\n### TMP_FILE 파일지정안됨\n");return No;}
	if (mode<10) unlink(TMP_FILE);
	mode%=10;
	if (mode<=1) for (i=0;i<30;i++) printf("\n");
	if (mode==1&&strlen(pf.id)>0&&bit(pf.mymode,11))
		 sys_tem("bin/editor%d -w '%s'",mode,TMP_FILE);
	else sys_tem("bin/editor%d '%s'",mode,TMP_FILE);
	if (!is_file(TMP_FILE)) return No;
	if (bit(bcfg.wmode,17)&&file_size(TMP_FILE)<10) {
		unlink(TMP_FILE);
		view("frm/editor/writeert.%s",bbs_tail());
		return No;
	}
	return (ret);
}




rm_tmpup()
{
	char buff[300];
	if (strlen(tty)<1) return;
    sprintf(buff,"rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
	system(buff);
}


//mode=1:zmodem  2:kermit  99:UPS 0:선택
//check 파일 존재여부 검사
up_file(int mode,char *filename,int check)	 //화일을 전송받아 filename에 저장한다.
{
	FILE *fp;
	int i,i2,n,ret=No,num=1;
	char buff[380],buff1[380],buff2[380],dir[380];
	char from_user=No;
	char newdir[300];
	char OLDDIR[300];
	char filename2[380];
	char adir[20][100];
	int downmode=0,count=0;

	getcwd(OLDDIR,sizeof(OLDDIR));		//현재의 디렉토리
	strcpy(filename,"");
	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	if (mode==99&&(bit(allmode,14)||pf.type>=18)) num=99;
	else {
		if (is_file(newdir)) sys_tem("rm %s -rf",newdir);
		if (bit(bcfg.wmode,19)) num=1;		//강제적으로 Zmode선택
		else {
			_cont:
			strcpy(g_buff4,"");
			if (bit(allmode,14)||(pf.type==18&&!bit(cfg.downmode,17))||(pf.type==19&&!bit(cfg.downmode,18)))
				strcpy(g_buff4,"99.UPS");
			if (!view("frm/editor/upload.%s",bbs_tail()))
				view("frm/editor/upload.def");
			lineinput(buff,4);printf("\n");check_han(buff);
			num=atoi(buff);
			if (num==88) {
				autoup=autoup?No:Yes;
				goto _cont;
			}
		}
	}
	_ups:
	if (num==99) {
		for (i=0;i<20;i++) strcpy(adir[i],"");
		if ((fp=fopen("system/upsdir.txt","r+"))!=NULL) {
			i=0;
			while (i<20&&fgets(buff1,100,fp)!=NULL) {
				del_space(buff1);del_esc(buff1);
				strcpy(adir[i++],buff1);
			}
			fclose(fp);
		}
		count=0;
		_cont1:
		if (!bit(allmode,14)&&pf.type<18) goto _fail;
		if (count++>5)	goto _fail;
		if (!(bit(allmode,14)||(pf.type==18&&!bit(cfg.downmode,17))||(pf.type==19&&!bit(cfg.downmode,18)))) goto _fail;

		for (i2=0;i2<3;i2++) {
			view("frm/editor/upsfile.log");
			ret=get_filename (dir,filename2,DEF_UPS);
			if (is_same(filename2,"q")||is_same(filename2,"p")) goto _fail;
			sprintf(filename,"%s/%s",dir,filename2);
			del_space(filename);
			if (!ret) goto _fail;
			for (i=0;i<20;i++) {
				if (is_file(filename)) break;
				if (strlen(adir[i])<1) continue;
				sprintf(filename,"%s/%s",adir[i],filename2);
			}
			if (!is_file(filename)) goto _cont1;
			if (!bit(cfg.downmode,5)) {
				xstrncpy(g_buff2,dir,99);
				strcpy(g_buff3,filename2);
				n=file_size(filename);
				sprintf(g_buff4,"%d",n);
				if (view("frm/editor/confirmu.log")) {
					if (!yesno2("",Yes)) goto _fail;
				}
			}
			return Yes;
		}
	}
	if (is_char("0qQxXpP",buff[0])) goto _fail;
	if (num== 2)	  view("frm/editor/kermit.log");
	else if (num==99) goto _ups;
	else			  view("frm/editor/zmodem.log");

	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	check_dir(newdir);
    ch_dir(newdir);
	getcwd(buff,sizeof(buff));		//현재의 디렉토리
	if (is_same(buff,OLDDIR)) {
		printf(" 전송디렉토리 만들기 실패:*%s*\n",newdir);
		goto _fail;
	}
	view("frm/editor/up_start.log");
	if (num== 2) n=kermitrz();
	else		 n=rz();
	if (c_proc) {
		getcwd(buff,sizeof(buff));		//현재의 디렉토리
		printf("현재위치:%s  전송완료\n",buff);
		system("ls -l");
		pressenter();
	}
	if (n!=0||dir_filenum("./")<1) {
		view("%s/frm/editor/upload.err",OLDDIR);
		goto _fail;
	}
	//전송성공
	sys_tem("chmod 777 *");
//    sys_tem("%s/bin/xlower",OLDDIR);  //소문자로 변경
	dir_filename(buff,"./");     //현재방에 있는 화일명중 1개
    ch_dir(OLDDIR);      //원래 디렉토리

	if (!check) goto _ok;
	re_name1:
	if (!good_filename(buff)) goto _fail;
	sprintf(buff1,"%s/%s/%s",OLDDIR,newdir,buff);
	sprintf(buff2,"%s/%s/%s/%s",OLDDIR,club_go.dir,go.dir,buff);
	//중복파일 있을때
	if (!bit(bcfg.downmode,15)&&is_file(buff1)&&is_file(buff2)) {
		strcpy(g_buff3,buff);
		re_name2:
        if (!view("frm/editor/samefile.log")) printf(" ## %s 파일중복 새파일명?>>",buff);
        dosinput(buff,12);printf("\n");check_han(buff);
        del_space(buff);del_esc(buff);
		if (buff[0]==0||!good_filename(buff)) goto re_name2;
        if (is_quit(buff)) goto _fail;
		sprintf(buff2,"%s/%s/%s",OLDDIR,newdir,buff);
		file_move(buff1,buff2);
		goto re_name1;
	}

	//등록 확인
	_ok:
    ch_dir(OLDDIR);      //원래 디렉토리
	sprintf(buff1,"%s/%s",newdir,buff);
	if (!is_file(buff1)) goto _fail;
	xstrncpy(g_buff2,buff1,99);
	strcpy(g_buff3,buff);
	sprintf(g_buff4,"%d",file_size(buff1));
	if (autoup) nostop=Yes; 	//자동등록
	view("frm/editor/confirm.log");
	if (!autoup&&!yesno2("",Yes)) goto _fail;
	strcpy(filename,buff1);
	return Yes;

	_fail:
    ch_dir(OLDDIR);      //원래 디렉토리
	strcpy(filename,"");
	rm_tmpup();
	return No;
}

//mode=1:zmodem  2:kermit  99:UPS 0:선택
up_text(int mode,char *filename)   //화일을 전송받아 filename에 저장한다.
{
	int i,n,ret=No,num;
	char buff[380];
	char buff2[380];
	char from_user=No;
	char newdir[300];
	char OLDDIR[300];
	char dir[380];
	char filename2[380];

	getcwd(OLDDIR,sizeof(OLDDIR));		//현재의 디렉토리
	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	rm_tmpup();
	check_dir(newdir);

	if (mode==0) {
		strcpy(g_buff1,"");strcpy(g_buff2,"");strcpy(g_buff3,"");strcpy(g_buff4,"");
		strcpy(g_buff4,"");
		if (bit(allmode,14)||(pf.type==18&&!bit(cfg.downmode,17))||(pf.type==19&&!bit(cfg.downmode,18)))
			strcpy(g_buff4,"99.UPS");
		if (!view("frm/editor/uploadt.%s",bbs_tail()))
			view("frm/editor/upload.def");
        fflush(NULL);
		lineinput(buff,20);check_han(buff);printf("\n");
		if (is_char("0qQxXpP",buff[0])) goto _fail;
		mode=atoi(buff);
	}
	_ups:
	if (mode==99) {
		if (!bit(allmode,14)&&pf.type<18) return No;
		view("frm/editor/upstext.log");
		ret=get_filename(dir,filename2,DEF_UPS);
		sprintf(buff,"%s/%s",dir,filename2);
		del_space(buff);
		if (!ret||!is_file(buff)) goto _fail;
		file_copy(buff,filename);
		if (!bit(cfg.downmode,5)) {
			xstrncpy(g_buff2,dir,99);
			strcpy(g_buff3,filename2);
			sprintf(g_buff4,"%d",file_size(filename));
			view("frm/editor/confirmt.log");
			if (!yesno2("",Yes)) goto _fail;
		}
		return Yes;
	}
	else if (mode==2) view("frm/editor/kermit.log");
	else		 view("frm/editor/zmodem.log");

	//파일 받기 시작
	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	check_dir(newdir);
    ch_dir(newdir);
	getcwd(buff,sizeof(buff));		//현재의 디렉토리
	if (is_same(buff,OLDDIR)) {
		printf(" 전송디렉토리 만들기 실패:*%s*\n",newdir);
		goto _fail;
	}
	if (c_proc) {
		printf("현재위치:%s\n",buff);
		printf("NEWDIR:%s\n",newdir);
	}
	view("frm/editor/up_start.log");
	if (mode==2) n=kermitrz();
	else		 n=rz();
	if (c_proc) {
		getcwd(buff,sizeof(buff));		//현재의 디렉토리
		printf("현재위치:%s  전송완료 n=%d\n",buff,n);
		system("ls -l");
		pressenter();
	}
	fflush(stdout);
	if (n!=0||dir_filenum("./")<1) {
		view("%s/frm/editor/upload.err",OLDDIR);
		goto _fail;
	}
	//전송성공
	sys_tem("chmod 777 *");
//    sys_tem("%s/bin/xlower",OLDDIR);  //소문자로 변경
	ret=No;
	dir_filename(buff,"./");          //현재방에 있는 화일명중 1개
    ch_dir(OLDDIR);                    //원래 디렉토리

	if (!good_filename(buff)) goto _fail;
	sprintf(buff2,"%s/%s/%s",OLDDIR,newdir,buff);
	file_move(buff2,filename);
	chmod(filename,0777);
	rm_tmpup();
	return Yes;

	_fail:
    ch_dir(OLDDIR);      //원래 디렉토리
	unlink(filename);
	rm_tmpup();
	return No;
}


int currentlinenum;  /* 라인넘버를 표시하기 위한 변수 */
struct line *t_start, *t_now, *t_end, *t_tmp;


struct line {			 /* 에디터에서 쓰일 구조체(라인 에디터임) */
	struct line *left;	 /* 전 줄 */
	struct line *right;  /* 다음 줄 */
	int linenum;		 /* 라인넘버 (reserved) */
	char text[80];		 /* 줄 내용 */
} *t_start, *t_now, *t_end, *t_tmp;
int Line_total; 		//총 자료수



editor2(char *filename)
{
	char buf[80];
	int key;

	key=Yes;
	if (strlen(filename)<2) return;
	printf(" 에디터쓰기 (끝낼때는 마지막줄 첫칸에 '.')\n");
	printf("0---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+---\n");
	ltext_read(filename); /* 텍스트구조체로 읽어온다. */
	currentlinenum=1;
	key=ltext_append();
	while (1) {
		printf(" 명령(편집종료:'Enter', 편집취소:'Q', 도움말:'H'): ");
		lineinput(buf,10);check_cmd(buf);check_han(buf);printf("\n");
		if (is_same(mmc[0],"l")) key=ltext_list(mmn[1]);
		else if (is_same(mmc[0],"a")) {
			printf("0---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+---\n");
			key=ltext_append();
		}
		else if (is_same(mmc[0],"e")) key=ltext_edit(mmn[1]);
		else if (is_same(mmc[0],"d")) key=ltext_delete(mmn[1]);
		else if (is_same(mmc[0],"i")) key=ltext_insert(mmn[1]);
		else if (is_same(mmc[0],"h")) printf(" E:편집종료 Q:작성취소 A:줄의추가 I:줄의삽입 L:내용보기 D:줄의삭제\n");
		else if (is_same(mmc[0],"q")) {
			if (yesno(" 편집 취소?(y/N)>>",No)) {
				printf("\n");
				key=No;unlink(filename);
				break;
			}
		}
		else {
			ltext_write(filename);
			break;
		}
	}
	ltext_unlink();   //메모리 해제
}


/* 편집기 구조체를 파일로 저장한다. */
ltext_write(char *filename)
{
	FILE *fp;

	if((fp=fopen(filename,"w"))==NULL) {
		printf("파일을 저장할수 없습니다.\n");
		return (No);
	}

	t_now=t_start;
	while(t_now->right != '\0') {
		fputs(t_now->text,fp);
		fputc('\r',fp);
		fputc('\n',fp);
		t_now=t_now->right;
	}
	if (fp!=NULL) fclose(fp);
	return (Yes);
}


// text를 현재 번호부터 입력한다.
ltext_append()
{
	while(1) {
		lineinput(t_now->text,79);
		if(strcmp(t_now->text,".")==0 || t_now->text[0] == 0x1a) {
			t_now->right = '\0';
			break;
		}
		printf("\n");
		t_end = (struct line *)malloc(sizeof(struct line));

		t_now->right = t_end;
		t_end->left = t_now;
		t_now = t_end;
		currentlinenum++;
		Line_total++;
	}
	return (Yes);
}


/* text를 출력한다 */
ltext_list(int mode)
{

	int i = 0, j, count ;
	char buff[5];

	if (mode>0) i=mode;
	else {
		printf("\n(총%d줄)  출력시작할 줄번호  (전체:enter) >>",Line_total);
		lineinput(buff,4);
		printfL0();
		printfL2();
		i= atoi(buff);
		if (i>Line_total) i=Line_total;
		if (i<1) i=1 ;
	}

	t_now = t_start;
	for(j = 1; j < i; j++) {
		t_now = t_now->right;
		if(t_now->right == '\0') {
			t_now = t_now->left;
			break;
		}
	}

	count=0;
	while(t_now->right != '\0') {
		currentlinenum=i++;
		if(count<20+1) {
			printf("%3d:%s\n",currentlinenum,t_now->text);
			t_now = t_now->right;
		}
		else {
			count = 0;
			printf("줄번호 %d/%d  (계속:enter 중지:Q  다시:R)  >>",
					currentlinenum-1,Line_total);
			lineinput(buff,1);
			printf("\r");
			if (buff[0]=='q'||buff[0]=='Q') break  ;
			if (buff[0]=='r'||buff[0]=='R') {
				t_now=t_start ;
				i=1;
			}
		}
	}
	printf("\n");
	return (Yes);
}


ltext_edit(int mode)
{
	int i, j;
	char buff[5];
	char buff2[100];

	printfL2();
	ltext_list(1);
	printfL2();
	if (mode>0) i=mode;
	else {
		printf(" (총%d줄)  편집할 줄번호  (취소:enter) >>",Line_total);
		lineinput(buff,4);printf("\n");
		i= atoi(buff);
	}
	printfL2();
	if (i>Line_total) i=Line_total;

	t_now = t_start;
	if(i>0) {
		//줄번호를 찾는다.
		for(j = 0; j < i-1; j++) {
			t_now = t_now->right;
			if(t_now->right == '\0') {
				t_now = t_now->left;
				break;
			}
		}
		printf("\n%3d:%s",i,t_now->text);
		printf("\n%3d:",i);
		lineinput(buff2,79);
		if (buff2[0]!=0) strcpy(t_now->text,buff2);
	}
	return (Yes);
}


/* 줄번호 삭제 */
ltext_delete(int mode)
{
	int i, j, chk = 1;
	char buff[5];
	int ret;

	ret=No;
	if (mode>0) i=mode;
	else {
		printf("(총%d줄)  삭제할 줄번호  (취소:enter) >>",Line_total);
		lineinput(buff,4);
		i = atoi(buff);
		printfL0();
		printfL2();
	}

	t_now = t_start;

	if(i > 0) {
		for(j = 1; j < i; j++) {
			t_now = t_now->right;
			if(t_now->right == '\0') {
				chk = 0;
				printf("해당 번호 없음\n");
				break;
			}
		}
		if(chk) {
			if(t_now->left == '\0') {
				t_start = t_start->right;
				t_start->left = '\0';
			}
			else {
				t_tmp = t_now->left;
				t_tmp->right = t_now->right;
				t_tmp = t_now->right;
				t_tmp->left = t_now->left;
			}
			free(t_now);
			currentlinenum--;
			Line_total--;
			ret=Yes;
		}

	}
	return (ret);
}



ltext_insert(int mode)
{
	int i, j, chk = 1;
	char buff[5];
	int ret;
	ret=No;
	if (mode>0) i=mode;
	else {
		printf("\n(총%d줄)  삽입할 줄번호  (취소:enter) >>",Line_total);
		lineinput(buff,4);printf("\n");
		i = atoi(buff);
	}
	printfL2();
	if (i>Line_total) i=Line_total;
	t_now = t_start;
	if(i > 0) {
		for(j = 1; j < i; j++) {
			t_now = t_now->right;
			if(t_now->right == '\0') {
				chk = 0;
				printf("\n해당 번호 없음\n");
				break;
			}
		}
		if(chk) {

			printf("\n"); printf("%3d:",i);
			t_tmp = (struct line *)malloc(sizeof(struct line));
			lineinput(t_tmp->text,79);
		   if(t_now->left == '\0') {
				t_now->left = t_tmp;
				t_tmp->left = '\0';
			}
			else {
				t_tmp->right = t_now;
				t_now = t_now->left;
				t_tmp->left = t_now;
				t_now->right = t_tmp;
				t_now = t_tmp->right;
				t_now->left = t_tmp;
			}
			currentlinenum++;
			Line_total++;
			ret=Yes;
		}
	}
	return (ret);
}


//파일을 읽어 구조체에 등록시킨다.
ltext_read(char *filename)
{
	FILE *fp;
	char c;
	int i;
	int ret;
	ret=Yes;


	t_start = (struct line *) malloc(sizeof(struct line));
	t_start->left = '\0';
	t_now = t_start;
	t_end = t_now;
	currentlinenum=0;
	i = 0;
	Line_total=0;

	if ((fp=fopen(filename,"r+"))==NULL) return No;
	while((c = fgetc(fp)) != EOF) {
		if ( c == '\r' ) {
			t_end = (struct line *)malloc(sizeof(struct line));
			t_now->right = t_end;
			t_end->left = t_now;
			t_now = t_end;
			i=0;
			currentlinenum++;
		}
		else if ( c =='\n') ;
		else {
			t_now->text[i]=c;
			i++;
		}
	}
	t_now->right= '\0';
	Line_total=currentlinenum;	  //총 줄수

	if (fp!=NULL) fclose(fp);
	return Yes;
}

//메모리를 해제 한다.
ltext_unlink()
{
	t_now = t_start;
	while(t_now->right != '\0') {
		t_now = t_now->right;
		free(t_start);
		t_start = t_now;
	}
	free(t_now);
}
