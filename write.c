////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
//	  게시판 관련 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

bbs_write(int ups_mode)
{
	char TNOW_FILE[300],TNOW_FILED[300];

    _last2=3;;;;;;;;;;;;;;;;;;;;;;;;
	if (bbs_ltmode) {		 //조건지정시
		bbs_ltmode=No;
		set_filename();
		check_bbs();
	}
	strcpy(TNOW_FILE,NOW_FILE);strcpy(TNOW_FILED,NOW_FILED);
	//메일이면 적는 루틴이 다름
    if (is_same(go.code,"rmail")||is_same(go.code,"wmail")||is_same(go.code,"cmail")||is_same(go.code,"mbox")) {
        wmail(1,"");
        return;
	}
    if (bit(pf.mymode,24)) {
        if (!view("frm/pf/notwrite2.log"))
            printf("\n### %s님은 쓰기금지되었습니다.\n",pf.id);
        return;
    }
	touch_file(NOW_FILE);touch_file(NOW_FILED);

    if (bit(limit_mode,1)) {
        if (!view("frm/pf/notwrite.log")) mess("쓰기금지");
        return;
    }
	if (pf.type<19&&ups_mode) ups_mode=0;
	else if (is_typebbs) bbs_writebbs(ups_mode);
	else				 bbs_writepds(ups_mode);
	strcpy(NOW_FILE,TNOW_FILE);strcpy(NOW_FILED,TNOW_FILED);
    _last2=14;;;;;;;;;;;;;;;;;;;;;;;;
}


bbs_writebbs(int ups_mode)
{
	FILE *fp;
	char buff[250];
	char buff2[250];
	char dir[280];
	char filename[280];
	char qfilename[180];
	char filename2[180];
	GO oldgo;
	BBS oldbbs;
	int n,nn,i,ret;
	int quest_mode;
	int quest_sec_mode=No;	//설문지 비공개여부
	int is_nofile=No;
	int bbs_mode=is_typebbs;
	int old_level=pf.level;
	int old_down=pf.down;
	int now_num;
	char from_user=No;		//사용자영역에서 전송
	time_t down_start_time;
	time_t down_end_time;

	oldgo=go;
	oldbbs=bbs;
	ret=Yes;
	re_bbs=Yes;
	re_print=Yes;


    if (pf.type<18) {
        if (!club_mode&&view("frm/down/nowrmain.log")) goto _fail;
        if ( club_mode&&view("frm/down/nowrclub.log")) goto _fail;
    }

	sprintf(qfilename,"%s/logo/%s.qst",club_go.dir,go.code); //설문지화일

    sprintf(buff,"%s/logo/%s.nwr",club_go.dir,go.code);
    if (is_file(buff)) {view(buff);goto _fail;}
    sprintf(buff,"%s/logo/all.nwr",club_go.dir);
	if (is_file(buff)) {view(buff);goto _fail;}

	if (pf.type<19&&ups_mode) ups_mode=0;
	if (guest_mode) {
		if (bit(cfg.infomode,1));
		else if (go.type=='g');
		else if (go.type=='y');
		else  {
			if (!view("frm/error/errorw.log")) mess("손님은 적을수 없습니다.");
			return No;
		}
		pressenter();
	}

	//등록여부질문
	if (!bit(bcfg.wmode,18)) {
		if (view("frm/editor/uploadq.%s",bbs_tail())) {
			if (!yesno2("",!bit(bcfg.downmode,16))) goto _fail;
		}
	}

	sprintf(filename,"%s/logo/%s.liw",club_go.dir,go.code);
	ret=No;
	if (bit(limit,1)) view2("frm/_nodeall.log");
	else if (!is_char(BBSTYPE,go.type)) 	message("등록 가능 메뉴가 아닙니다.");
    else if (pf.type<18&&(go.type=='z'||go.type=='b'))      message("전체자료실엔 등록할수 없습니다.");
	else if (bbs_man(pf.id))				ret=Yes;  //담당자일때 통과
	else if (bit(bcfg.mode,28)&&in_file2("system/node_net.txt",tty)) ret=No;
	else if (bit(bcfg.modelist,6)&&!in_file2(filename,pf.id)) return No;
	else if (is_same(go.code,"userroom"))   message("사용자 영역에는 저장이 안됩니다.");
	else if (check_type('w'))               ret=No; //메뉴 가능 등급확인
	else ret=Yes;

	if (!ret) return No;
	if (ss_check(1))		 return No;  //쓰기 금지
	if (check_warn(1))		 return No;  //경고시 쓰기 금지
	if (!guest_mode&&wtimes()) return No;	  //적는 횟수 검사

	//등록시안내화면
	view("%s/logo/%s.bbs",club_go.dir,go.code);

	strcpy(bbs.id,"");
    strcpy(bbs.passwd,"");
	strcpy(bbs.name,"");
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	bbs.posi=0;
	bbs.gd=' ';
	bbs.del=No;
	bbs.size=0;

	unlink(TMP_FILE);
	unlink(TMP_FILE1);
	unlink(TMP_FILE2);

	//설문응답
	quest_mode=No;
	if (!bit(bcfg.wmode,6)&&is_file(qfilename)) {
		if (bit(bcfg.wmode,7)||yesno3("frm/write/sul1.log",Yes)) {
			file_line(buff,qfilename,1);	//첫줄 파악
			if (stristr(buff,"_{비공")&&yesno3("frm/write/sul2.log",Yes))
				quest_sec_mode=Yes;  //설문지 비공개여부
			set_stat2(5,"설문응답");     //쓰기
			sys_tem("bin/quest %s %s %s",pf.id,qfilename,TMP_FILE2);
			quest_mode=Yes;
			if (!is_file(TMP_FILE2)) goto _fail;	  //취소시
		}
	}

	//제목입력
    if (write_bbs_title()==No) goto _fail;      //취소시

	set_stat(5,bbs.title);								   //쓰기

	//기본설명입력
	if (write_bbs_quest()==No) goto _fail;

	if (pf.type<19&&ups_mode) ups_mode=0;
	//내용입력
	unlink(TMP_FILE);
	if (go.type=='-'||bit(bcfg.wmode,11));      //내용통과
	else {										//제목전용이 아니면?
		if (ups_mode) ret=input_tmp(99); // ->TMP_FILE에 저장
		else		  ret=input_tmp(0);
		if (!is_file(TMP_FILE)||ret==No) goto _fail;			  //취소시
	}

	strcpy(bbs.key,"");
    strcpy(bbs.passwd,"");
	bbs.read=bbs.del=bbs.ok=bbs.down=bbs.size=bbs.posi=0;
	bbs.gd=' ';
    if (strlen(bcfg.write_id)>2&&bbs_man(pf.id)) strcpy(bbs.id,bcfg.write_id); //현재의 아이디,이름
    else                                         strcpy(bbs.id,pf.id);             //현재의 아이디,이름
	strcpy(bbs.name,bit(bcfg.mode,8)?pf.nick:pf.name);
	if (quest_sec_mode==Yes) {
		strcpy(bbs.id,"비공개");             //현재의 아이디,이름
		strcpy(bbs.name,"비공개");
	}
	time(&bbs.date);
	time(&bbs.date2);
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	//검색어지정
	if (bit(bcfg.wmode,10)) bbs_key_edit(0);

	//게시물저장 시작
	if (is_file(TMP_FILE2)) {
		file_add(TMP_FILE2,TMP_FILE);		//TMP_FILE2 에 TMP_FILE을 추가
		file_move(TMP_FILE2,TMP_FILE);
	}
	//자신의 서명을 추가함
	if (!guest_mode&&!is_char("okvz",go.type)&&!bit(cfg.mode,9)&&!bit(bcfg.wmode,9)) {
		sprintf(buff,"userdata/%s/%s/pfinfo.sig",c_han(pf.id),pf.id);
		file_add(TMP_FILE,buff);
	}
	if (is_same(go.code,"notice")) bbs.gd='*';
	if (bit(bcfg.wmode,25)) bbs.gd='*';
	if ((go.type=='-'||bit(bcfg.mode,25))&&is_file(TMP_FILE)) unlink(TMP_FILE);
	//게시물추가
	if (bit(bcfg.wmode,5)) {
		file_line(buff,TMP_FILE,1);
		sprintf(buff2,"%s %s",bbs.title,buff);
		all_trim(buff2);
		xstrncpy(bbs.title,buff2,60);
	}
	if (!bit(bcfg.wmode,8)) {
		if (strlen(bcfg.outbbs)>0) {
			//지정 게시판에 복사+쓰기
			if (bit(bcfg.wmode,24)) {
				ret=bbs_add(1);
			}
			//지정 게시판에 쓰기
			if (bit(bcfg.wmode,24)||gtt_bit(bcfg.wmode,23)) {
				sprintf(NOW_FILE,"%s/menu/%s.tit",club_go.dir,bcfg.outbbs);
				sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,bcfg.outbbs);
				ret=bbs_add(1);
			}
		}
		else ret=bbs_add(0);
	}
	else {
		wmail4(bcfg.man1,TMP_FILE,bbs.title,0);
        sprintf(buff,"\n **%s 님의 메일도착(담당)\n",pf.id);
        to_mess(bcfg.man1,buff);
		ret=Yes;
	}
	now_num=bbs.num;
	//전체자료수증가
	if (!ret) goto _fail;	//취소시

	pf.todaymode=set_bit(pf.todaymode,1,1);   //오늘글쓰기

	for (i=0;i<5;i++) {
		if (is_same(cfg.check_wgo[i],go.code)) {
			pf.todaymode=set_bit(pf.todaymode,21+i,1);	 //오늘글쓰기
			break;
		}
	}

    _last2=10;;;;;;;;;;;;;;;;;;;;;;;;
	cpf_down_down(1,0);   //해당동호회 다운수 증가
	if (bbs_mode) {uall.write++;uall.writesize+=bbs.size;}
	else		  {uall.up	 ++;uall.upsize   +=bbs.filesize;}
	if (!is_char("guo",go.type)) {    //업전용 자료실이 아닐때
		if (!from_user||bit(cfg.downmode,3)) {
			//레벨,다운용량증가
			bbs_down_up(bbs.filesize,pf.id,1);
		}
	}
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	set_rec(1,5);  //작업내용 화일 저장
	if (!bit(bcfg.wmode,8)) add_zgo();		//전체자료실 등록
	unlink(TMP_FILE);	//임시화일 삭제
	check_bbs();
	re_print=Yes;
	set_stat(1,"");     //쓰기해제

	if (!bit(bcfg.wmode,3)) view("frm/editor/writeok.%s",bbs_tail());

	//매일 1등모드
	if (bit(bcfg.form,11)&&bbs_num(now_num)) {
		i=time_num(bbs.date);
		go_next_list();
		if (i!=time_num(bbs.date)) {
			if (bcfg.uplevel>0||bcfg.updown>0) {
			}
		}
	}
	check_bbs();
	bbs=oldbbs;
	pf_today(0,1,0);	   //쓰기수 증가
    if (pf.type>0) save_pf(21);
    unlink("tmp/zbtotalb.tmp");
    if (bit(cfg.zmode,25)) check_zbtotal(30*60,2);
    re_bbsnum(3);           //BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
    _last2=16;;;;;;;;;;;;;;;;;;;;;;;;
	return Yes;

	_fail:
	check_bbs();
	bbs=oldbbs;
	if (is_file(TMP_FILE)) unlink(TMP_FILE);   //임시화일 삭제
	view("frm/editor/writeerr.%s",bbs_tail());
	re_bbs=Yes;
	re_print=Yes;
	set_stat(1,"");     //쓰기해제
	check_cmd("");
    re_bbsnum(3);           //BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
    _last2=15;;;;;;;;;;;;;;;;;;;;;;;;
	return No;
}


bbs_writepds(int ups_mode)
{
	FILE *fp;
	char buff[250];
	char buff2[250];
	char dir[280];
	char filename[280];
	char qfilename[180];
	char filename2[180];
	GO oldgo;
	BBS oldbbs;
	int n,nn,i,ret,key;
	int quest_mode;
	int quest_sec_mode=No;	//설문지 비공개여부
	int is_nofile=No;
	int bbs_mode=is_typebbs;
	int old_level=pf.level;
	int old_down=pf.down;
	int now_num;
	char from_user=No;		//사용자영역에서 전송
	time_t down_start_time;
	time_t down_end_time;
    char tNOW_FILE[300],tNOW_FILED[300],tgo_dir[300];

    strcpy(tNOW_FILE ,NOW_FILE );
    strcpy(tNOW_FILED,NOW_FILED);
    strcpy(tgo_dir   ,go.dir   );

	ret=No;
	oldgo=go;
	oldbbs=bbs;
	ret=Yes;
	re_bbs=Yes;
	re_print=Yes;
	sprintf(qfilename,"%s/logo/%s.qst",club_go.dir,go.code); //설문지화일

    sprintf(buff,"%s/logo/%s.nwr",club_go.dir,go.code);
    if (is_file(buff)) {view(buff);goto _fail;}
    sprintf(buff,"%s/logo/all.nwr",club_go.dir);
	if (is_file(buff)) {view(buff);goto _fail;}

    if (pf.type<18) {
        if (!club_mode&&view("frm/down/noupmain.log")) goto _fail;
        if ( club_mode&&view("frm/down/noupclub.log")) goto _fail;
    }

	if (pf.type<19&&ups_mode) ups_mode=0;
	if (guest_mode) {
		if (bit(cfg.infomode,2));
		else if (go.type=='g');
		else if (go.type=='y');
		else  {
			if (!view("frm/error/errorup.log")) mess("손님은 등록할수 없습니다.");
			return No;
		}
	}
	//등록여부질문
	if (ups_mode) printf(" ##UPS MODE##\n");
	else if (!bit(bcfg.wmode,18)) {
		if (view("frm/editor/uploadq.%s",bbs_tail())) {
			if (!yesno2("",!bit(bcfg.downmode,16))) goto _fail;
		}
	}
	if (pf.type<18&&(cfg.up_limit||bcfg.up_limit)) {
		key=No;
		if (cfg.up_limit>0) {
			i=set_tag_down(4);	//현재 전체업/다운수
            if (i>=cfg.up_limit) {a_num[8]=i;a_num[9]=cfg.up_limit;key=Yes;}
		}
		if (!key&&bcfg.up_limit>0) {
			i=set_tag_down(6);	//현재 전체업/다운수
            if (i>=bcfg.up_limit) {a_num[8]=i;a_num[9]=bcfg.up_limit;key=Yes;}
		}
		if (key) {
			view("frm/editor/up_limit.log");
			goto _fail;
		}
	}

	sprintf(filename,"%s/logo/%s.liw",club_go.dir,go.code);
	ret=No;
	if (bit(limit,1)) view2("frm/_nodeall.log");
	else if (!is_char(BBSTYPE,go.type)) 	message("등록 가능 메뉴가 아닙니다.");
    else if (pf.type<18&&(go.type=='z'||go.type=='b'))      message("전체자료실엔 등록할수 없습니다.");
	else if (bbs_man(pf.id))				ret=Yes;  //담당자일때 통과
	else if (bit(bcfg.mode,28)&&in_file2("system/node_net.txt",tty)) ret=No;
	else if (bit(bcfg.modelist,6)&&!in_file2(filename,pf.id)) return No;
	else if (is_same(go.code,"userroom"))   message("사용자 영역에는 저장이 안됩니다.");
	else if (go.type=='c')                  message("CD 롬 자료실엔 등록할수 없습니다.");
	else if (go.type=='d')                  message("디렉토리 자료실엔 등록할수 없습니다.");
	else if (check_type('w'))               ret=No; //메뉴 가능 등급확인
	else ret=Yes;

	if (!ret) return No;
	if (is_typebbs&&ss_check(1))	return No;	//쓰기 금지
	if (is_typepds&&ss_check(2))	return No;	//등록 금지
	if (check_warn(1))				return No;	//경고시 쓰기 금지
	if (!guest_mode&&is_bbspds&&wtimes()) return No;	 //적는 횟수 검사

	//등록시안내화면
	view("%s/logo/%s.bbs",club_go.dir,go.code);

	strcpy(bbs.id,"");
    strcpy(bbs.passwd,"");
	strcpy(bbs.name,"");
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	bbs.posi=0;
	bbs.gd=' ';
	bbs.del=No;
	bbs.size=0;

	unlink(TMP_FILE);
	unlink(TMP_FILE2);

	//제목입력
    if (write_bbs_title()==No) goto _fail;      //취소시
	set_stat(5,bbs.title);								   //쓰기

	//기본설명입력
	unlink(TMP_FILE1);
	if (write_bbs_quest()==No) goto _fail;

	//내용입력
	unlink(TMP_FILE);
	if (go.type=='-'||bit(bcfg.wmode,11));      //내용통과
	else {										//제목전용이 아니면?
		ret=input_tmp(0);						//내용은ups아님
		if (ret==No) goto _fail;			  //취소시
	}
	strcpy(bbs.key,"");
    strcpy(bbs.passwd,"");
	bbs.read=bbs.del=bbs.ok=bbs.down=bbs.size=bbs.posi=0;
	bbs.gd=' ';
    if (strlen(bcfg.write_id)>2&&bbs_man(pf.id)) strcpy(bbs.id,bcfg.write_id); //현재의 아이디,이름
    else                                         strcpy(bbs.id,pf.id);             //현재의 아이디,이름
	strcpy(bbs.name,bit(bcfg.mode,8)?pf.nick:pf.name);
	if (quest_sec_mode==Yes) {
		strcpy(bbs.id,"비공개");             //현재의 아이디,이름
		strcpy(bbs.name,"비공개");
	}
	time(&bbs.date);
	time(&bbs.date2);

	strcpy(bbs.filename,"");
	//화일명입력,전송
	check_dir(go.dir);
	if (!bbs_man(pf.id)&&pf.type<14&&bit(bcfg.downmode,1))		 is_nofile=Yes;
	else if (!bbs_man(pf.id)&&pf.type>=14&&bit(bcfg.downmode,2)) is_nofile=Yes;
	else {
		time(&down_start_time);    //업시작시간
		if	 (ups_mode) ret=up_file(99,buff,No);  //시삽지정 업 ->filename
		else {
			if (bit(cfg.downmode,20)&&check_free()<50) {view("frm/system/nospace.log");goto _fail;}
			ret=up_file(0,buff,Yes);   //업 ->filename
		}
		bbs.filesize=file_size(buff);
		str_filedir(buff,dir,filename);  //buff를 디렉토리와 파일명으로 분해
        if (is_same(filename,"id.dat")||is_same(filename,"passwd")) {
            mess("%s 등록불가",filename);
            goto _fail;
        }
		filename[12]=0;del_space(filename);del_esc3(filename);
		xstrncpy(bbs.filename,filename,13);
		time(&down_end_time);
		if (!is_file(buff)||strlen(filename)==0||!ret) {
			if (bit(bcfg.downmode,11)) {
				strcpy(bbs.filename,bit(bcfg.downmode,13)?"화일없음":"");
				ret=Yes;
				is_nofile=Yes;
			}
			else goto _fail;   //취소시
		}
		if (bit(bcfg.wmode,12)&&bbs.filesize==0) goto _fail;	 //취소시
		if (!is_nofile) {				   //자료실의 경우
			if (strlen(go.dir)<1) sprintf(go.dir,"%s/%s",club_go.dir,go.code);
			check_dir(go.dir);
			sprintf(filename,"%s/%s",dir,bbs.filename);
			if (!is_file(go.dir)||is_same(go.dir,"/")) {
				mess("### 자료실 디렉토리(%s)가 없습니다. \n",go.dir);
				goto _fail;
			}
			if (!is_file(filename)) {
				mess("### %s 화일이 없습니다. \n",filename);
				goto _fail;
			}
			del_esc3(go.dir);check_dir(go.dir);
			sprintf(buff2,"%s/%s",go.dir,bbs.filename);
            if (strlen(filename)>0&&strlen(buff2)>0) {
                sys_tem("cp -f '%s' '%s'",filename,buff2);
            }
			rm_tmpup();
			check_achive();  //현재의 bbs,go에서 화일을 rar처리한다.
		}
	}

	//검색어지정
	if (bit(bcfg.wmode,10)) bbs_key_edit(0);

	//게시물저장 시작
	if (is_file(TMP_FILE2)) {
		file_add(TMP_FILE2,TMP_FILE);		//TMP_FILE2 에 TMP_FILE을 추가
		file_move(TMP_FILE2,TMP_FILE);
	}
	//자신의 서명을 추가함
	if (!guest_mode&&!is_char("okvz",go.type)&&!bit(bcfg.wmode,9)) {
		sprintf(buff,"userdata/%s/%s/pfinfo.sig",c_han(pf.id),pf.id);
		file_add(TMP_FILE,buff);
	}
	if (is_same(go.code,"notice")) bbs.gd='*';
	if (bit(bcfg.wmode,25)) bbs.gd='*';
	if ((go.type=='-'||bit(bcfg.mode,25))&&is_file(TMP_FILE)) unlink(TMP_FILE);
	//게시물추가
	if (bit(bcfg.wmode,5)) {
		file_line(buff,TMP_FILE,1);
		sprintf(buff2,"%s %s",bbs.title,buff);
		all_trim(buff2);
		xstrncpy(bbs.title,buff2,60);
	}
	if (!bit(bcfg.wmode,8)) {
		if (strlen(bcfg.outbbs)>0) {
			//지정 게시판에 복사+쓰기
			if (bit(bcfg.wmode,24)) {
				ret=bbs_add(1);
			}
			//지정 게시판에 쓰기
			if (bit(bcfg.wmode,24)||gtt_bit(bcfg.wmode,23)) {
				sprintf(NOW_FILE,"%s/menu/%s.tit",club_go.dir,bcfg.outbbs);
				sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,bcfg.outbbs);
				ret=bbs_add(1);
			}
		}
		else ret=bbs_add(0);
	}
	now_num=bbs.num;
	//전체자료수증가
	if (!ret) goto _fail;	//취소시

	pf.todaymode=set_bit(pf.todaymode,2,1);   //오늘작업
	cpf_down_down(1,0);   //해당동호회 다운수 증가
	if (bbs_mode) {uall.write++;uall.writesize+=bbs.size;}
	else		  {uall.up	 ++;uall.upsize   +=bbs.filesize;}
	if (!is_char("guo",go.type)) {    //업전용 자료실이 아닐때
		if (!from_user||bit(cfg.downmode,3)) {
			//레벨,다운용량증가
			bbs_down_up(bbs.filesize,pf.id,1);
		}
	}
	set_rec(1,6);  //작업내용 화일 저장

	if (!bit(bcfg.wmode,8)) add_zgo();		//전체자료실 등록
	unlink(TMP_FILE);	//임시화일 삭제
	check_bbs();
	re_print=Yes;
	set_stat(1,"");     //쓰기해제

	if (autoup) nostop=Yes; 	//자동등록
	if ( (is_typepds&&!bit(bcfg.downmode,18))||
		 (is_typebbs&&!bit(bcfg.wmode,3)) )
		view("frm/editor/writeok.%s",bbs_tail());

	//매일 1등모드
	if (bit(bcfg.form,11)&&bbs_num(now_num)) {
		i=time_num(bbs.date);
		go_next_list();
		if (i!=time_num(bbs.date)) {
			if (bcfg.uplevel>0||bcfg.updown>0) {
			}
		}
	}
    _last2=11;;;;;;;;;;;;;;;;;;;;;;;;
	check_bbs();
	bbs=oldbbs;
	pf_today(0,0,1);	   //업수 증가
    if (pf.type>0) save_pf(22);
    unlink("tmp/zbtotalp.tmp");
    if (bit(cfg.zmode,25)) check_zbtotal(30*60,1);
    strcpy(NOW_FILE ,tNOW_FILE );
    strcpy(NOW_FILED,tNOW_FILED);
    strcpy(go.dir   ,tgo_dir   );
    re_bbsnum(3);           //BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
    _last2=12;;;;;;;;;;;;;;;;;;;;;;;;
	return Yes;

	_fail:
	check_bbs();
	bbs=oldbbs;
	rm_tmpup();
	if (is_file(TMP_FILE)) unlink(TMP_FILE);   //임시화일 삭제
	view("frm/editor/writeerr.%s",bbs_tail());
	re_bbs=Yes;
	re_print=Yes;
	set_stat(1,"");     //쓰기해제
	check_cmd("");
    strcpy(NOW_FILE ,tNOW_FILE );
    strcpy(NOW_FILED,tNOW_FILED);
    strcpy(go.dir   ,tgo_dir   );
    re_bbsnum(3);           //BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
    _last2=13;;;;;;;;;;;;;;;;;;;;;;;;
	return No;
}


write_bbs_quest()		//기본설명
{
	int ret=Yes;
	char filename2[300];
	if (bit(bcfg.qmode,7)) { //미리준비된응답 입력받음
		ret=bit(bcfg.qmode,8);
		if (!ret) {
			view("frm/editor/quest%s.log",bbs_tail());
			ret=yesno2("",No);
		}
		if (ret) {
			if (bit(bcfg.qmode,11)) {
				sprintf(filename2,"bin/quest%s",bbs_tail());
				if (is_file(filename2)) {
					if (sys_tem("%s '%s' '%s'",filename2,TMP_FILE1,TMP_FILE2)==-1) return No;
				}
			}
			else {
				sprintf(filename2,"%s/logo/%s.qsh",club_go.dir,go.code);  //미리준비된응답
				if (!is_file(filename2)) sprintf(filename2,"%s/frm/editor/questh.%s",club_go.dir,bbs_tail());  //미리준비된응답
				if (is_file(filename2))
					sys_tem("bin/quest '%s' '%s' '%s'",pf.id,filename2,TMP_FILE2);
			}
		}
	}
	return Yes;
}


//게시판 제목입력
write_bbs_title()
{
	int n,head_num=0;
	char buff[200];
    char head_buff[100];
	n=3;
	strcpy(head_buff,"");
	strcpy(bbs.title,"");
	if (bit(bcfg.wmode,4)) {
		strcpy(bbs.title,go.name);
		goto _skip;   //제목입력하지않고 건너뜀
	}

	while (n--) {
		strcpy(head_buff,"");
		if (!bit(bcfg.wmode,1)) {
			head_num=select_head(head_buff);  //머리글 선택
			if (head_num<0) return No;
		}
		view("frm/editor/writetit.%s",bbs_tail());
		if (head_num==0) doskey(bbs.title,60,1);
		else {
			printf("%s ",head_buff);
			doskey(bbs.title,60-(int)strlen(head_buff),1);
		}
		printf("\n");
		if (is_same(bbs.title,"q")||is_same(bbs.title,"p")) return No;
		if (bbs.title[0]==0&&go.type=='a') strcpy(bbs.title,go.name);
		if (strlen(bbs.title)<3) {
			sprintf(buff,"frm/editor/writeerq.%s",bbs_tail());
			if (yesno3(buff,Yes)) return No;
			continue;
		}
		break;
	}
	if (n<0||strlen(bbs.title)<3) return No;

	_skip:
    if (strlen(head_buff)>0) {
		sprintf(buff,"%s %s",head_buff,bbs.title);
        buff[60]=0;strcpy(bbs.title,buff);
	}
	return Yes;
}




//검색어수정 mode=0 : 신규	mode=1:수정
bbs_key_edit(int mode)
{
	char buff[80];
	printf(" ### 검색어 (주제어,키워드 / 찾는명령:lk 키워드)\n");
	printf(" 검색어는 타인이 자료를 가장빠르게 검색하기위한 의미있는 단어입니다.\n");
	printf(" 검색어는 2~8bytes 내용을 설명하기 적당한 단어로 정하십시요.\n");
	printf(" 검색어를 입력하세요: (8자) ");
	if (mode) {
		printf ("(원래%s) (0:검색어없앰) ",bbs.key);
		strcpy(buff,bbs.key);
        lineinput(bbs.key,8);check_han(buff);
        if (bbs.key[0]==0)    strcpy(bbs.key,buff);
        else if (is_quit(bbs.key)) strcpy(bbs.key,"");
	}
	else
		lineinput(bbs.key,8);
}


//현재 bbs 구조체를 저장한다.(내용은 TMP_FILE에 저장이 되어야 한다.)
//mode=1 이면 완벽하게 복구함 (bbs,NOW_REC 등등)
//mode=9 내용추가 안함
bbs_add(int mode)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	time_t t;
    BBS  bbs2,oldbbs;
    int i,lastposi,change_size,n,n2;
	char c;
	char buff[180];
	int tNOW_ALLNO	 =NOW_ALLNO ;	//복구를 위해
	int tNOW_NO 	 =NOW_NO	;
	int tNOW_ALLREC  =NOW_ALLREC;
	int tNOW_REC	 =NOW_REC	;

	oldbbs=bbs;
	//게시물 총수 파악
	n=0;
	//조건지정시 해제함
	if (bbs_ltmode) {bbs_ltmode=No;set_filename();check_bbs();}
    c_lock(1,NOW_FILE);                    //LOCK확인  1:확인 2:종료
	//임시에서 읽어 내용저장 (fp2->fp3)
	bbs.line=1;
	bbs.size=0;
    bbs.del=No;
    bbs.ok=0;
    bbs.posi=bbs.size=bbs.read=bbs.down=0;

    //내용 추가
    n2=file_size(TMP_FILE);    //내용없음
    if ((fp2=fopen(NOW_FILED,"r+"))==NULL) {
        touch_file(NOW_FILE);
        touch_file(NOW_FILED);
        fp2=fopen(NOW_FILED,"w+");         //내용화일
    }
    if (fp2==NULL) {mess("%s 파일열기실패",NOW_FILED);goto return_no;}
    fseek(fp2,1,SEEK_END);
    bbs.posi=ftell(fp2)-1;
    if (bbs.posi<0) bbs.posi=0;
    if (mode==9||n2<1) ;    //내용없음
    else {
        if ((fp3=fopen(TMP_FILE,"r"))!=NULL) {          //임시작성화일
            fseek(fp2,bbs.posi,SEEK_SET);
            while (bbs.size<n2) {
                c=fgetc(fp3);             //임시에서 읽어
                if (c==0||(byte)c==255) break;
                if (c=='\n') bbs.line++;  //줄수 계산
                fputc(c,fp2);             //내용에 저장
                bbs.size++;
                if (bcfg.limitsize>0&&bbs.size>=bcfg.limitsize) break;
            }
            fclose(fp3);
        }
    }
    fclose(fp2);

	if (go.type=='o') {strcpy(bbs.id,"........");strcpy(bbs.name,"........");}
	del_tab(bbs.title);
	//제목화일 저장
    if ((fp1=fopen(NOW_FILE,"r+"))==NULL) {                   //제목화일 저장함 추가
        fp1=fopen(NOW_FILE,"w+");                   //제목화일 저장함 추가
    }
    if (fp1==NULL) goto return_no;
    fseekend(fp1);
    n=ftell(fp1)/sizeof_bbs;
    if (n>1000000) n=0;
    bbs2.num=1;
    bbs.num=1;
    if (n>0) {
        fseek(fp1,(n-1)*sizeof_bbs,SEEK_SET);
        if (fread(&bbs2,sizeof_bbs,1,fp1)) {
            bbs.num=bbs2.num+1;
        }
    }
    fseek(fp1,n*sizeof_bbs,SEEK_SET);
    del_esc3(bbs.filename);del_esc3(bbs.id);del_esc3(bbs.name);
    if (bbs.line>250&&is_bbspds&&bit(bcfg.wmode,26)) {
        printf("\n ## 이 게시물은 250줄을 초과하여 삭제처리되었습니다. ## \n");
        bbs.del=Yes;
    }
    fwrite(&bbs,sizeof_bbs,1,fp1);
//    mess("%d %d %s",bbs.posi,bbs.num,bbs.title);
    fclose(fp1);

    NOW_ALLNO=bbs.num;
	NOW_NO=bbs.num;
	NOW_ALLREC++;
	NOW_REC=NOW_ALLREC-1;
        if (mode==1) {
		NOW_ALLNO	=tNOW_ALLNO ;
		NOW_NO		=tNOW_NO	;
		NOW_ALLREC	=tNOW_ALLREC;
		NOW_REC 	=tNOW_REC	;
		bbs=oldbbs;
	}
	return_yes:
	c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
    sync();fflush(NULL);
	return (Yes);

	return_no:
	c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
    sync();fflush(NULL);
	return No;
}


select_head(char *head_buff)     //머리글 선택
{
	char filename[100];
	int head_num=0;
	int i,n,count;
	char buff[100];
	char atit[100][110];
	FILE *fp;

	for (i=0;i<100;i++) strcpy(atit[i],"");

	sprintf(filename,"%s/logo/%s.hea",club_go.dir,go.code);
	if ((fp=fopen(filename,"r"))==NULL) return head_num;
    strcpy(head_buff,"");
	strcpy(buff,"");
	count=0;
	while (count<100&&fgets(buff,100,fp)!=NULL) {
		del_esc(buff);
		if (buff[0]!=0) strcpy(atit[count++],buff);
	}
	fclose(fp);
    n=bcfg.headnum;
	if (count>0&&n>0) {
        strcpy(head_buff,atit[n-1]);
		return n;
	}
	if (count>0) {
		while (1) {
			sprintf(filename,"%s/logo/%s.hsl",club_go.dir,go.code);
			printf("\n");
			view(filename);
			for (i=0;i<count;i++) {
				if (i%4==0) printf("\n");
				printf("[7m%2d[m.%-14.14s",i+1,atit[i]);
			}
			view("frm/editor/select.hea");
            lineinput(buff,3);check_han(buff);printf("\n");
			head_num=atoi(buff);
			if (is_char("QqXxPp",buff[0])) return -1;
			if (head_num<0||head_num>count) head_num=0;
            if (head_num==0) strcpy(head_buff,"");
            else strcpy(head_buff,atit[head_num-1]);
			if (bit(bcfg.wmode,2)&&head_num==0) continue;
			break;
		}
	}
    if (is_same(head_buff,"[머리말없음]")||
        is_same(head_buff,"[말머리생략]")||
        is_same(head_buff,"[말머리없음]")||
        is_same(head_buff,"[없음]")||
        is_same(head_buff,"[생략]") ) {
        strcpy(head_buff,"");
        head_num=0;
    }

	return head_num;
}


bbs_addblank()
{
	if (pf.type<16&&now_type<16) return;
	strcpy(bbs.id,pf.id);
	strcpy(bbs.name,pf.name);
	strcpy(bbs.title,"");
	strcpy(bbs.filename,"");
	strcpy(bbs.key,"");
	bbs.filesize=0;
	bbs.read=bbs.down=bbs.gd=bbs.line=bbs.ok=bbs.gd=bbs.del=0;
	unlink(TMP_FILE);
	bbs_add(0);
    re_bbsnum(3);           //BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
	re_bbs=Yes;
	re_print=Yes;
}



