////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
//	  게시판 관련 루틴
//	 삭제후 바로 PACK 기능 선택...
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//해당 되는 번호를 찾아 bbs에 저장
int bbs_num(int num)
{
	FILE *fp;
	BBS bbs1;
	int i,n,found=No;
	int last,first,now;
	view_pos=0; 	   //현재게시판내용 보는 위치
	view_num=0; 	   //그림보는현재번호
	if (bbs_ltmode) fp=fopen(LT_FILE,"r+");
	else			fp=fopen(NOW_FILE,"r+");
	if (fp==NULL) return No;
	//먼저 현재위치인가?
	fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
	if (fread(&bbs1,sizeof_bbs,1,fp)&&bbs1.num==num) {
		found=Yes;
		fclose(fp);
		goto _ok;
	}
	//인덱스 검색
	first=1;
	fseekend(fp);
	last=ftell(fp)/sizeof_bbs;
	now=last/2+1;
    while(1) {
		fseek(fp,(now-1)*sizeof_bbs,SEEK_SET);
		if (fread(&bbs1,sizeof_bbs,1,fp)) {
			if (bbs1.num==num) {
				found=Yes;
				NOW_REC=now-1;
				break;
			}
			else if ((last-first<5)||now<0) break;
			else if (bbs1.num>num) last=now;
			else				   first=now;
			now=(last-first)/2+first+1;
		}
		else break;
	}
	if (!found) {
		i=now-15;if (i<0) i=0;
		for (n=i;n<now+30;n++) {
			fseek(fp,n*sizeof_bbs,SEEK_SET);
			if (fread(&bbs1,sizeof_bbs,1,fp)) {
				if (bbs1.num==num) {
					NOW_REC=n;
					found=Yes;
					break;
				}
			}
		}
	}

	if (!found) {  //검색 실패시 처음부터 검색
		fseekend(fp);
		last=ftell(fp)/sizeof_bbs;
		rewind(fp);
		for (i=0;i<last;i++) {
			fread(&bbs1,sizeof_bbs,1,fp);
			if (bbs1.num==num) {
				found=Yes;
				NOW_REC=i;
				break;
			}
		}
	}
	fclose(fp);

	_ok:
	g_char=' ';
	jpg_tag=No;
    if (found&&bbs1.del) {
        if (pf.type<18&&bit(bcfg.mode,21)) found=No;
        else if (bbs_man(pf.id)) found=Yes;
        else if (!dd_mode) found=No;
        else if (club_mode&&cpf.type>16) found=Yes;
        else if (!club_mode&&pf.type>15) found=Yes;
        else found=No;
    }
	if (found) {
		re_print=Yes;
		bbs=bbs1;
		del_tab(bbs.title);all_trim(bbs.title);
		del_esc(bbs.key);all_trim(bbs.key);
		del_esc(bbs.id);del_esc(bbs.name);del_space(bbs.id);
		del_space(bbs.filename);del_tab(bbs.filename);del_esc(bbs.filename);bbs.filename[12]=0;
		NOW_NO=bbs.num;
		N_page=1;
		g_char=gd_chr(is_typebbs);	 //추천/조회/다운지정
		jpg_tag=is_jpgfile();
		set_buffnum(buffnum,bbs.num,jpg_tag);	   //buffnum만들기
	}
	return(found);
}



//게시판 수정
//입력 : 수정할 게시물 번호
//출력 : NOW_FILE에 저장한다.

bbs_edit(int num,int mode)
{
	char buff[280];
	int tNOW_NO,tNOW_REC,tNOW_ALLREC,tNOW_TOPREC;
	tNOW_NO=NOW_NO;tNOW_REC=NOW_REC;tNOW_TOPREC=NOW_TOPREC;tNOW_ALLREC=NOW_ALLREC;
	if (guest_mode) {
		view("frm/error/noguest.err");
		return;
	}
	if (list_mode&&num==0) num=bbs.num;
	//임시로 시삽이 올린것이면 수정가능하도록 되어 있음..

    list_mode=No;
	bbs_ltmode=No;		  //lt등으로 검사중 모드
	set_filename(); 	  //현재표시할 화일명
	check_bbs();		  //자료의 갯수를 파악하는 루틴
	if(!bbs_num(num)) {
		mess(" %d번 자료를 찾을수 없습니다.",num);
		return;
	}
	if (check_del()) {	   //삭제가능한 레벨인가?
		sys_tem("bin/ed_bbs %d '%s' '%s' '%d' '%s' '%s'",mode,NOW_FILE,pf.id,NOW_REC,club_go.code,go.code);
	}
	else mess("수정권한이 없습니다.");
	NOW_NO=tNOW_NO;NOW_REC=tNOW_REC;NOW_TOPREC=tNOW_TOPREC;NOW_ALLREC=tNOW_ALLREC;
    bbs_num(num);
	re_print=Yes;
}

bbs_jump(int num)
{
	char buff[280];
	if (list_mode&&num==0) num=bbs.num;
	if(bbs_num(num)&&strlen(bbs.key)>0)
		strcpy(auto_cmd,bbs.key);
}

view_last_num(int mode)
{
	FILE *fp1;
	BBS bbs2;
	int now_rec;
	int now_allrec;
	int found=No;
	//화일이 없으면 새로 만듬
    set_filename();                         //현재표시할 화일명
    read_bcfg();
	if (bbs_ltmode) fp1=fopen(LT_FILE,"r+");
	else			fp1=fopen(NOW_FILE,"r+");
	if( fp1!=NULL ) {
		fseekend(fp1);
		now_allrec=ftell(fp1)/sizeof_bbs;
		while (now_allrec>0) {
			fseek(fp1,(now_allrec-1)*sizeof_bbs,SEEK_SET);
			if (fread(&bbs2,sizeof_bbs,1,fp1)) {
				if (!bbs2.del) {
					bbs=bbs2;
					NOW_REC=now_allrec;
					if (NOW_REC>NOW_ALLREC) {
						check_bbs();
						NOW_REC=now_allrec;
					}
					found=Yes;
					break;
				}
			}
			now_allrec--;
		}
		fclose(fp1);
	}
    if (mode) {
        if (found) {
            bbs_read_num();   //읽음 처리
            add_read_numn(bbs.num);       //읽음 표시 추가
        }
        return;
    }
	if (found) {
		bbs_read_num();   //읽음 처리
        cls();
		bbs_read(0);
		add_read_numn(bbs.num); 	  //읽음 표시 추가
	}
}


bbs_read_num()
{
	int i;
	BBS bbs2;
	FILE *fp1;
	int found;
	int tNOW_NO,tNOW_REC,tNOW_ALLREC,tNOW_TOPREC;
	tNOW_NO=NOW_NO;tNOW_REC=NOW_REC;tNOW_TOPREC=NOW_TOPREC;tNOW_ALLREC=NOW_ALLREC;
	if (is_same(go.code,"rmail")) {  //메일에서는 특별 처리
		mail_set_read();			 //상대방 메일 읽음 처리
	}
	if (is_same(go.code,"cmail")) return;
    if (bit(bcfg.recmode,3)) return;
	//게시판의 읽은횟수 증가  (본인이 아닐때),마지막에 읽은게시물이 아닐때
    if ( (is_same(go.code,"rmail"))||bit(bcfg.recmode,5)||
		 (!is_same(bbs.id,pf.id)&&!is_read_numn(bbs.num))) {
		if ((fp1=fopen(NOW_FILE,"r+"))==NULL) goto _quit;
		c_lock(1,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
		if ((fp1=fopen(NOW_FILE,"r+"))!=NULL) {
			fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
			if (fread(&bbs2,sizeof_bbs,1,fp1)&&bbs2.num==bbs.num) {
				bbs2.read++;
				bbs.read++;
                if (bit(bcfg.recmode,3)) bbs.down++;
				if (is_same(go.code,"rmail")&&is_same(bbs.key,"긴급")) {
					bbs.gd=' ';
					bbs2.gd=' ';
				}
				time(&bbs2.date2);	//읽은 날짜 지정
				time(&bbs.date2);  //읽은 날짜 지정
				fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
				fwrite(&bbs2,sizeof_bbs,1,fp1);
			}
			fclose(fp1);
		}
		c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
	}
    sync();
	_quit:
	NOW_NO=tNOW_NO;NOW_REC=tNOW_REC;NOW_TOPREC=tNOW_TOPREC;NOW_ALLREC=tNOW_ALLREC;
}



//무조건 삭제 지정
bbs_set_del()
{
	int i;
	BBS bbs2;
	FILE *fp1;
	c_lock(1,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
	if ((fp1=fopen(NOW_FILE,"r+"))!=NULL) {
		i=0;
		while (fread(&bbs2,sizeof_bbs,1,fp1)) {
			if (bbs2.num==bbs.num) {
				bbs2.del=1;
				fseek(fp1,i*sizeof_bbs,SEEK_SET);
				if (i<1000000) {
					if (!feof(fp1)) fwrite(&bbs2,sizeof_bbs,1,fp1);
				}
				break;
			}
			i++;
		}
		fclose(fp1);
	}
    sync();
	c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
}


//게시판 답변
bbs_re()
{
	int n,ret,mode;
	char buff[80];
	char buff2[80];
	char buff3[30];
    char buff4[30];
	unsigned int i;
	 re_print=Yes;
	if (!is_char(BBSTYPE,go.type))	return;
	if (!(is_same(mmc[0],"re")||
          is_same(mmc[0],"r")||
          is_same(mmc[0],"ra")||
		  is_same(mmc[0],"rs")||
		  is_same(mmc[0],"rn")||
		  is_same(mmc[0],"rt")||
		  is_same(mmc[0],"at")||
		  is_same(mmc[0],"an")||
		  is_same(mmc[0],"rr")))    return;
	if (is_typepds&&
         (is_same(mmc[0],"ra")||
		  is_same(mmc[0],"rn")||
          is_same(mmc[0],"rt")||
          is_same(mmc[0],"an")||
          is_same(mmc[0],"rr")||
		  is_same(mmc[0],"rr")))    return;
	if (go.type=='k') return;  //비공개 경우
	ret=Yes;
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	if (mmn[1]==0) ret=No;	//선택번호 없을때
	list_mode=No;

	if (bit(limit,1)) {
		view2("frm/_nodeall.log");
		return;
	}

	if (ret==No) {
		printf("\n *****답변적기명령 사용방법****** \n");
		printf("ra [번호]  해당게시물에 추가를 적음 [추가]....\n");
		printf("rn [번호]  해당게시물에 답변을 적음 [답변]....\n");
		printf("rt [번호]  해당게시물에 참고을 적음 [참고]....\n");
		printf("rr [번호]  해당게시물에 잡담을 적음 [잡담]....\n");
		printf("re [번호]  해당게시물 작성자에게 답장메일을 보냄\n");
		printf("rs         해당게시물 담당자에게 메일을 보냄\n");
		printf("an [번호]  해당게시물 답변가능 게시물에 답변적기\n");
		printf("at [번호]  해당게시물 추가(본인또는,담담자) 적기\n");
		pressenter();
		return;
	}

	if (guest_mode&&!is_char("gun",go.type)) {  /* 손님인 경우에는 */
		message("손님은 사용할수 없는 명령입니다.");
		return;
	}

    if (bbs_ltmode) {        //조건지정시
		bbs_ltmode=No;
		set_filename();
		check_bbs();
	}
	if (check_type('w')) return;   //쓰기금지여부
	if (!guest_mode&&wtimes()) return ; 		//적는 횟수 검사

	if (!bbs_num(mmn[1])) {
		message("해당번호를 찾을수 없습니다.");
		return;   //해당번호 찾기
	}
	strcpy(buff3,bbs.id);
	strcpy(buff,bbs.title);
    strcpy(buff2,"");
    if (is_same(mmc[0],"an")||is_same(mmc[0],"at")) { //여유분명령있을때
        if (strlen(mmc[3])>0&&atoi(mmc[3])==0) strcpy(buff2,mmc[3]);
        else if (strlen(mmc[2])>0&&atoi(mmc[2])==0) strcpy(buff2,mmc[2]);
    }

    if (is_same(mmc[0],"an")) {bbs_an(0,buff2);return;}
    else if (is_same(mmc[0],"at")) {bbs_an(1,buff2);return;}
    else if (is_same(mmc[0],"re")) {wmail(2,buff3);return;}
    else if (is_same(mmc[0],"r"))  {wmail(1,buff3);return;}
	else if (is_same(buff,"rs"))       {
		for(i=0;i<10;i++) strcpy(mmc[i],"");
		strcpy(mmc[1],bcfg.man1);  //담당자에게 메일
		if (mmc[1][0]==0) strcpy(mmc[1],bcfg.man2);  //담당자에게 메일
		if (mmc[1][0]==0) strcpy(mmc[1],bcfg.man3);  //담당자에게 메일
		if (mmc[1][0]==0&&!club_mode) strcpy(mmc[1],cfg.sysop);
		if (mmc[1][0]==0&& club_mode) strcpy(mmc[1],ccfg.sysop);
        wmail(1,mmc[1]);
		return;
	}
	sprintf(buff4," (%s)",bbs.id);
	if (is_same(mmc[0],"ra")) strcpy(buff2,"[추가]");
	if (is_same(mmc[0],"rn")) strcpy(buff2,"[답변]");
	if (is_same(mmc[0],"rt")) strcpy(buff2,"[참고]");
	if (is_same(mmc[0],"rr")) strcpy(buff2,"[잡담]");
	n=6;
	mode=0;
	for(i=0;i<strlen(buff);i++) {
		if		(buff[i]=='[') mode=1;
		else if (mode==1&&buff[i]==']') mode=2;
		else if (mode==2) buff2[n++]=buff[i];
	}
	if (mode<2) strcat(buff2,buff);
	else buff2[n]=0;
	buff2[60]=0;
	strcpy(buff,buff2);
	strcat(buff,buff4);
	buff[60]=0;

	strcpy(bbs.title,buff);
	strcpy(bbs.name,pf.name);
	strcpy(bbs.key,"");
    strcpy(bbs.passwd,"");
	bbs.read=0;
	bbs.del =0;
	bbs.ok	=0;
	bbs.down=0;
	strcpy(bbs.id,pf.id);			  //현재의 아이디,이름
	strcpy(bbs.name,bit(bcfg.mode,8)?pf.nick:pf.name);
	time(&bbs.date);
	time(&bbs.date2);
	bbs.gd=' ';
	sprintf(bbs.key,"%d",bbs.num);
	printf("\n    < %s >\n",bbs.title);
	unlink(TMP_FILE);
	ret=bbs_editor(1);
	if (ret) ret=bbs_add(0);  //게시물저장 시작
	if (ret) {
		//화일명입력,전송
		sprintf(buff,"      ** %s님께 답변하신 글이 저장되었습니다. **.",buff3);
		message(buff);
		add_zgo();		//전체자료실 등록
        unlink("tmp/zbtotalb.tmp");
	}
	unlink(TMP_FILE);
	re_print=Yes;
}


//해당 되는 게시판에 글을 적어 추가하기
//num:게시물 번호,mode:0: 답변,1:추가
bbs_an(int mode,char *cmdstr)
{
    char buff[80],buff1[80],c;
	FILE *fp1,*fp2;
	// 답변 가능 조건 확인
	if ( !(go.type=='q'||now_type>15||pf.type>15||bbs_man(pf.id)) ) {
		mess("답변을 적을수 없습니다.");
		return;
	}

    if (strlen(cmdstr)>0) {     //여유분명령 있을때
        sprintf(buff,"frm/bbs/%s.txt",cmdstr);
        if (!is_file(buff)) {
            mess(" [%s] 해당 답변파일이 없습니다.",cmdstr);
            return;
        }
        else {
            bbs2tmp();    //현재BBS의 내용을 TMP로 저장
            file_move(TMP_FILE,TMP_FILE2);
            touch_file(TMP_FILE2);
            printf("\n\n %s(%s) %-40.40s...  [%s]\n",bbs.id,bbs.name,bbs.title,mode==0?"답변":"추가");
            printf("  [%s] 저장\n",cmdstr);
            file_copy(buff,TMP_FILE);
            goto _cont1;
        }

    }
	//내용을 입력받는다. ->TMP_FILE로
	bbs2tmp();	  //현재BBS의 내용을 TMP로 저장
	file_move(TMP_FILE,TMP_FILE2);
	touch_file(TMP_FILE2);
	printf("\n\n %s(%s) %-40.40s...  [%s]\n",bbs.id,bbs.name,bbs.title,mode==0?"답변":"추가");
	if (input_tmp(0)==No) return;

    _cont1:
	if ((fp2=fopen(TMP_FILE,"r+"))==NULL) return;
	if ((fp1=fopen(TMP_FILE2,"a+"))==NULL) fp1=fopen(TMP_FILE2,"w+");
	fprintf(fp1,"\n   ###### [%s%d] ID: %s (%s) 일시 %s ######\n",mode==0?"답변":"추가",calc_ok(3)+1,pf.id,pf.nick,now_str(3));
    while (!feof(fp2)) {
        c=fgetc(fp2);
        if (c==0||(byte)c==255) continue;
        fputc(c,fp1);
    }
	fclose (fp1);
	fclose (fp2);
	if ((fp1=fopen(TMP_FILE1,"w+"))!=NULL) {
		bbs.ok+=1000000;  //응답횟수 추가
		fwrite(&bbs,sizeof_bbs,1,fp1);	//현재변경된 크기 저장
	}
	fclose (fp1);
    sync();
	sys_tem("bin/ed_bbs 999 %s %s %s %d",NOW_FILE,TMP_FILE1,TMP_FILE2,NOW_REC);
    unlink("tmp/zbtotalb.tmp");
	unlink(TMP_FILE);
	unlink(TMP_FILE1);
    unlink(TMP_FILE2);
    sync();
}


//삭제	0:삭제 1:복구 2:삭제시 다운량 삭감 안함 3:삭제자료실로 옮김
bbs_del(int mode)
{
	FILE *fp1;
    char buff[80];
	int num;
	int i;
	int old_level=pf.level;
	int old_down=pf.down;
	char modestr[20];
	char OLDDIR[300];
	time_t t;
	if (guest_mode) return No;
	getcwd(OLDDIR,sizeof(OLDDIR));		//현재의 디렉토리

	if (!is_char(BBSTYPE,go.type)) return No;
	if (mode==1) strcpy(modestr,"복구");
	else		 strcpy(modestr,"삭제");
	if ((mode==2||mode==3)&&!in_file2("system/bbsdel.id",pf.id)&&pf.type<17) mode=0;
	if (pf.type<17&&now_type<17&&bit(bcfg.form,3)) {
		mess(" 삭제할수 없는 게시판입니다.");
		return No;
	}
	re_print=Yes;
	num=mmn[1];
	if (num==0&&list_mode) {
		num=bbs.num;
		mmn[1]=num;
		list_mode=No;
	}
	if (num<1) {
		printf("%s 할 자료 번호 지정  >>",modestr);
		lineinput(buff,30); 	   //명령어 입력
		check_cmd(buff);		   //명령어로 분해
		sprintf(buff,"%s가 취소되었습니다.",modestr);
		if (mmn[1]<1) { message(buff);return No; }
	}

	re_print=Yes;
	for (i=1;i<101;i++) {
		if (mmn[i]>0) {
			if (!bbs_num(mmn[i])&&NOW_REC<NOW_ALLREC){
				printf(" %d 번의 자료가 없습니다.\n",mmn[i]);
			}
			else {
				sprintf(buff,"M:%d S:%dK",mode,bbs.filesize);
				if (is_same(go.code,"cmail")) {      //메일일때 보낸 편지 삭제
					printf("\n\n ## 삭제할메일: %d번 %s\n",bbs.num,bbs.title);
					printf(" ## %s 님에게 보낸 메일을 지우겠습니까? (Y/n) >>",bbs.id);
					if (yesno("",Yes)) {
						sys_tem("bin/delmail '%s' %d",pf.id,bbs.num);
						re_bbs=Yes;
						re_print=Yes;
						continue;
					}
					printf("\n");
				}
				if (check_del()||is_same(go.code,"userroom")) {      //삭제가능
					//게시물 총수
					bbs_set_(1,bbs.num,mode);	  //삭제,복구
					if ((mode==0||mode==3)&&
						!is_same("userroom",go.code)&&
						!is_same("bbs_all",go.code)&&
						!is_same("pds_all",go.code)&&
						!is_char("ozdun",go.type) ) {
						if (!bit(bcfg.mode,1)||(!bit(bcfg.mode,2)&&time_day(bbs.date)<3))
                            bbs_down_up(bbs.filesize,bbs.id,-1);
                        if (is_typepds) set_rec(1,26);  //삭제
                        else            set_rec(1,25);
					}
					if (pf.type>13&&mode==3&&is_typepds) {
                        if (!is_file("system/no_del.dat")) {
							sprintf(buff,"%s/%s",go.dir,bbs.filename);
                            if (is_file(buff)) unlink(buff);
						}
					}
					if (mode==3&&is_same("userroom",go.code)) {
						sprintf(buff,"roombin/%s/%s/%s",c_han(pf2.id),pf2.id,bbs.filename);
						unlink(buff);
					}
					if (mode==3&&is_same("rmail",go.code)) {
						sprintf(buff,"roombin/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
						unlink(buff);
					}
				}
			}
		}
	}
//    re_bbsnum(3);           //BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
    if (is_typebbs) unlink("tmp/zbtotalb.tmp");
    else            unlink("tmp/zbtotalp.tmp");
	return Yes;
}


//우수게시물 변환
bbs_gd(int mode)
{
	FILE *fp;
	char buff[80];
	int num;
	int ret,i,i2,found,count=0;
	BBS bbs2;

	if (is_char(BBSTYPE,go.type)==No) return No;
	ret=No;
	if (bbs_man(pf.id)) ret=Yes; //게시판의 담당자의 경우
	if (pf.type>14) 	ret=Yes; //운영진이상일때
	if (now_type>=14)	ret=Yes; //운영진이상일때
	if (!ret) {message("추천 권한이 없습니다.");return No;}

	num=mmn[1];
	if (num<1) {
		printf("우수 자료로 지정할 자료 번호 입력 >>");
		lineinput(buff,10); 	   //명령어 입력
		check_cmd(buff);		   //명령어로 분해
		if (mmn[0]<1) { message("취소되었습니다.");return No;  }
	}

	for (i=0;i<101;i++) {
		if (bbs_num(mmn[i])) {
			if ((fp=fopen(NOW_FILE,"r+"))==NULL) return No;
			fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
			if (fread(&bbs2,sizeof_bbs,1,fp)) {
				i2=0;
				if		(mode==1) {i2=1;bbs2.gd='#';}
				else if (mode==2) {i2=0;bbs2.gd=' ';}
                else if (mode==3) {i2=0;bbs2.gd='*';}
				else {
					found=No;
					for (i2=0;i2<9;i2++) {
						if (cfg.gdstr[i2]==bbs2.gd) {
							bbs2.gd=cfg.gdstr[i2+1];
							found=Yes;
							break;
						}
					}
					if (!found) bbs2.gd=cfg.gdstr[1];
					if (bbs2.gd==0) bbs2.gd=' ';
				}
				c_lock(1,NOW_FILE);
				fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
				if (!feof(fp )) fwrite(&bbs2,sizeof_bbs,1,fp);
				c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
				count++;
				printf("%7d. %-8s %-40.40s  -> 추천:%d('%c') 지정\n",
				bbs2.num,bbs2.id,bbs2.title,i2,bbs2.gd);
			}
			fclose(fp);
		}
	}
    sync();
	bbs_num(mmn[0]);
	return Yes;
}



//mode1:삭제 2:추천 3:응답
//mode2=0:삭제 1:복구
bbs_set_(int mode1,int num,int mode2)	 //해당 번호 게시물 처리
{
	FILE *fp;
	BBS bbs2;
	int i2,total,found;
	int old_rec=NOW_REC;
	char buff[100];

	c_lock(1,NOW_FILE); 				  //LOCK확인  1:확인 0:종료

	if (!bbs_num(num)) goto _quit1;
	fp=fopen(NOW_FILE,"r+");      //제목화일
	fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
	if (fread(&bbs2,sizeof_bbs,1,fp)&&bbs2.num==num) {
		if (mode1==1) {
            if (mode2==1) bbs2.del=No;
			else		  bbs2.del=Yes;
            time(&bbs2.date2);  //읽은 날짜 지정
		}
		else if (mode1==2) {
			if		(mode2==1)		bbs2.gd='#';
			else if (mode2==2)		bbs2.gd=' ';
			else if (mode2==3)		bbs2.ok+=1000000;  //응답횟수 추가
			else {
				found=No;
				for (i2=0;i2<9;i2++) {
					if (cfg.gdstr[i2]==bbs2.gd) {
						bbs2.gd=cfg.gdstr[i2+1];
						found=Yes;
						break;
					}
				}
				if (!found) bbs2.gd=cfg.gdstr[1];
				if (bbs2.gd==0) bbs2.gd=' ';
			}
		}
		fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
		if (!feof(fp )) fwrite(&bbs2,sizeof_bbs,1,fp);
	}
	fclose(fp);
    sync();

	_quit1:
	c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
	NOW_REC=old_rec;
}



//ls,ll 처리
bbs_list(int n)
{
	list_mode=No;
	re_print=Yes;
	if (n==0) {
		NOW_TOPREC=NOW_ALLREC;
		NOW_NO=NOW_ALLNO;
		NOW_REC=NOW_ALLREC-1;
        NOW_TOPREC=NOW_REC+1;
        NOW_TOPNO =NOW_NO;
		re_bbs=Yes;
	}
	else {
		if (bbs_num(n)) {
//            bbs_back();
//            disp_bbsstat();
//            dev();
			NOW_NO=bbs.num;
            NOW_TOPREC=NOW_REC+1;
			NOW_TOPNO =NOW_NO;
		}
		else bbs_list(0);
	}
}


//현재의 가기코드의 디렉토리에 화일이 있는가?
//있으면 Yes 없으면 No
is_gofile(GO ttgo,char *filename)
{
	char buff[220];
	if(filename[0]==0) return No;
	if (is_same(ttgo.code,"userroom")) {
		sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,filename);
        return (is_file(buff));
	}
	if(ttgo.dir[0]==0) return No;
	sprintf(buff,"%s/%s",ttgo.dir,filename);
	return (access(buff,0)!=-1);
}


//일일 쓰기 제한 확인 문제없으면 No 안되면 Yes
wtimes()
{
	FILE *fp;
	int key=No,ret=No;
	char st[10];
	char st1[10];
	char c,buff[550];
	int i,ii,num,mode;
	del_space(go.wtimes);
	if (guest_mode&&go.wtimes[0]==0||now_type>=14||pf.type>=14||bbs_man(pf.id)) return No;
	ii=0;
	mode=0;
	if		(is_char(go.wtimes,'a')) {mode=1;strcpy(st,"전체");}
	else if (is_char(go.wtimes,'t')) {mode=1;strcpy(st,"전체");}
	else if (is_char(go.wtimes,'m')) {mode=2;strcpy(st,"한달에");}
	else if (is_char(go.wtimes,'w')) {mode=3;strcpy(st,"한주에");}
	else if (is_char(go.wtimes,'d')) {mode=4;strcpy(st,"하루에");}
	bzero(st1,5);
	for (i=0,ii=0;i<5;i++) {c=go.wtimes[i];if (c>='0'&&c<='9') st1[ii++]=c;}
	del_space(st1);del_esc(st1);num=atoi(st1);
	if (mode!=0&&num!=0&&count_bbs(mode,num)) {  //게시물의 수계산
		printf("\n\n*** 이 게시판은 %s %d 번 까지만 적을수 있습니다. ***\n",st,num);
		printf("*** 먼저 이전 게시물을 지우고 등록하십시요. ***\n");
		pressenter();
		return Yes;
	}
	return No;
}


count_bbs(int mode,int n)
{
	FILE *fp;
	int num=0,total,i;
	int nn;
	struct tm *localtime();
	time_t today;
	time_t liday;
	BBS bbs2;
	struct tm *tp1;
	struct tm *tp2;

	time(&today);	//오늘의 날짜 지정
	tp1 = localtime(&today);
	tp1->tm_hour= 23;
	tp1->tm_min= 59;
	tp1->tm_sec= 59;
	today=mktime(tp1);

	liday = 0								 ;	 //삭제대상 날짜
	if (mode==1) liday = 0					 ;	 //삭제대상 날짜
	if (mode==2) liday = today - (30*24*3600);	 //삭제대상 날짜
	if (mode==3) liday = today - ( 7*24*3600);	 //삭제대상 날짜
	if (mode==4) liday = today - ( 1*24*3600);	 //삭제대상 날짜

	fp=fopen(NOW_FILE,"r+");
	if (fp==NULL) return No;
	fseekend(fp);
	total=ftell(fp)/sizeof_bbs;

	for (i=total;i>0&&num<30&&num<=n;i--) {
		fseek(fp,(i-1)*sizeof_bbs,SEEK_SET);
		if (fread(&bbs2,sizeof_bbs,1,fp)<1) break;
		if (bbs2.date<liday) break;
		if (!bbs2.del&&is_same(bbs2.id,pf.id)) num++;
	}
	if (fp!=NULL) fclose(fp);
	if (num<n) return No;
	return Yes;
}


//다음번호의 게시물
go_back_list()
{
	BBS bbs1;
	int i,iold;
	FILE *fp;

	view_pos=0;
	re_print=Yes;
	iold=NOW_REC;

	fp=fopen(NOW_FILE,"r+");      //제목화일
	if (fp==NULL) return Yes;
	for (i=NOW_REC+1;i<NOW_ALLREC;i++) {
		fseek(fp,i*sizeof_bbs,SEEK_SET);
		fread(&bbs1,sizeof_bbs,1,fp);
		if (!bbs1.del) break;
	}
	if (i<NOW_ALLREC) {
		bbs=bbs1;
		NOW_NO=bbs1.num;
		NOW_TOPNO=bbs1.num;
		NOW_REC=i;
		bbs_read_num();
		add_read_numn(bbs.num); 	  //읽음 표시 추가
	}
	if (fp!=NULL) fclose(fp);
	if ((i>=NOW_ALLREC||iold==NOW_REC)) {
		message("[7m << 맨처음입니다 >>[m");
		bbs_list(0);
	}
	return(Yes);
}


//다음번호의 게시물
go_next_list()
{
	BBS bbs1;
	int i,iold;
	FILE *fp;

	view_pos=0;
	re_print=Yes;
	iold=NOW_REC;

	fp=fopen(NOW_FILE,"r+");      //제목화일
	if (fp==NULL) return No;
	for (i=NOW_REC-1;i>=0;i--) {
		fseek(fp,i*sizeof_bbs,SEEK_SET);
		fread(&bbs1,sizeof_bbs,1,fp);
		if (bbs1.del==0) break;
	}
	if (i>=0) {
		bbs=bbs1;
		NOW_NO=bbs1.num;
		NOW_TOPNO=bbs1.num;
		NOW_REC=i;
		bbs_read_num();
		add_read_numn(bbs.num); 	  //읽음 표시 추가
	}
	if ((i<=0||iold==NOW_REC)&&bbs.line<15) message("[7m << 마지막입니다 >>[m");
	if (fp!=NULL) fclose(fp);
	return(Yes);
}


//전체자료실에 저장한다.
//현재 마지막 자료에서 읽어 저장한다.
add_zgo()
{
	char filename[130];
	char buff[130];
	char oldid	[30];
	char oldname[30];
	char tNOW_FILE[100];
	char tNOW_FILED[100];
	BBS oldbbs;
	FILE *fp;
    oldbbs=bbs;
	if (bit(cfg.mode,24)) return;
	if (bit(bcfg.wmode,15)) return;
    if (go.type=='z'||go.type=='b') return;
	if (is_char("ok",go.type)) {
		strcpy(bbs.id  ,"........");
		strcpy(bbs.name,"........");
	}
	if (stristr(NOW_FILE,"/bbs_all.tit")) return;
	strcpy(tNOW_FILE ,NOW_FILE );
	strcpy(tNOW_FILED,NOW_FILED);

	strcpy(NOW_FILE ,"menu/bbs_all.tit");
	strcpy(NOW_FILED,"menu/bbs_all.txt");
	if (is_typepds) {
	strcpy(NOW_FILE ,"menu/pds_all.tit");
	strcpy(NOW_FILED,"menu/pds_all.txt");
	}
	strcpy(bbs.tcode,go.code);
	sprintf(bbs.key,club_go.code);
    bbs.gd=' ';
	bbs_add(1);  //게시물저장 시작 내용 복구함
	strcpy(NOW_FILE ,tNOW_FILE);
	strcpy(NOW_FILED,tNOW_FILED);
    bbs=oldbbs;
}

//게시판 복사 운영자,게시판담당만 가능
//cp hello 1 2 3 4 5....
//mode2= 0:메인<->메인,동호회<->동호회	(cp/mv)
//		 1:메인->동호회 (cpclub/mvclub)  2:동호회->메인 (cpmain/mvmain)
bbs_copy(char *cmd)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char buff[180];
	char buff2[180];
	char NOW_FILE2[180];
	char NOW_FILED2[180];
	char cpstr[80];
	char c;
	int type_mode=0;
	int found=No;
	int ret,i,i2,new_posi;
	GO oldgo,oldclub_go,newgo,ttgo;
	BBS lastbbs;	 //이동대상의 마지막 BBS
	char newgo_dir[150];
	int oldclub_mode;
	int mode,mode2,mode3;
	int bbs_rec;
	int new_bbs_rec;   //새로운 게시판의 레코드번호
	int ii=0,count=0;
	int cont_mode=No;  //연속모드
	int quit_mode=No;
    int no_check_file=No;

	list_mode=No;
	re_print=Yes;
	ret=Yes;
	mode=mode2=0;
	oldgo=go;
	oldclub_go=club_go;
	oldclub_mode=club_mode;
	strcpy(cpstr,"복사");

	check_cmd(cmd);
	printf("\n");
	mode=mode2=mode3=0;
	if (guest_mode||!is_char(BBSTYPE,go.type)) goto fail_bbscopy;
    if (pf.type<18&&go.type=='k') {
		sprintf(buff,"지원되지 않는 메뉴타입 %c 입니다.",go.type);
		goto fail_bbscopy;
	}
	if (pf.type<14&&(is_same(go.code,"rmail")||is_same(go.code,"mbox")||is_same(go.code,"cmail"))) {
		sprintf(buff,"복사,이동명령을 쓸수 없는 게시판");
		goto fail_bbscopy;
	}
    if (pf.type<18&&!bbs_man(pf.id)) {
		sprintf(buff,"복사,이동명령을 쓸수 없는 등급");
		goto fail_bbscopy;
	}
	else if (is_same(mmc[0],"cp"    )) {mode=0;mode2=0;}
	else if (is_same(mmc[0],"cpp"   )) {mode=0;mode2=0;mode3=1;}
	else if (is_same(mmc[0],"cpmain")) {mode=0;mode2=1;}
	else if (is_same(mmc[0],"cppmain")){mode=0;mode2=1;mode3=1;}
	else if (is_same(mmc[0],"cpclub")) {mode=0;mode2=2;} //cpclub pbal1 100
	else if (is_same(mmc[0],"cppclub")){mode=0;mode2=2;mode3=1;} //cpclub pbal1 100
	else if (is_same(mmc[0],"mv"    )) {mode=1;mode2=0;}
	else if (is_same(mmc[0],"mvv"   )) {mode=1;mode2=0;mode3=1;}
	else if (is_same(mmc[0],"mvmain")) {mode=1;mode2=1;}
	else if (is_same(mmc[0],"mvvmain")){mode=1;mode2=1;mode3=1;}
	else if (is_same(mmc[0],"mvclub")) {mode=1;mode2=2;}
	else if (is_same(mmc[0],"mvvclub")){mode=1;mode2=2;mode3=1;}

    if (mmc[1][0]==0) ret=No;
	if (mode2<=1&&atoi(mmc[2])==0) ret=No;
	if (mode2==2&&atoi(mmc[2])!=0) ret=No;
	if (ret==No) {
		view("frm/help/bbscp.hlp");
		return;
	}
	strcpy(cpstr,mode?"이동":"복사");

    if (is_typepds) type_mode=1; //자료실타입
    else            type_mode=0; //게시판타입

	ret=Yes;
	//복사 대상 동호회 자료 검색
	if (mode2==0) { 				 //메인->메인,동호회->동호회
		ret=find_go(mmc[1]);
		if (!ret) {
			sprintf(buff,"[%s] 가기코드가 없습니다.",mmc[1]);
			goto fail_bbscopy;
		}
		set_filename(); 			 //현재표시할 화일명
		strcpy(NOW_FILE2,NOW_FILE);
		strcpy(NOW_FILED2,NOW_FILED);
		newgo=go;
		strcpy(newgo_dir,go.dir);		  //자료실의 경우에만 사용
	}
	else if (mode2==1) {						 //동호회->메인
		set_top();
		set_nowmenu();
		ret=find_go(mmc[1]);
		if (!ret) {
			sprintf(buff,"[%s] 가기코드가 없습니다.",mmc[1]);
			goto fail_bbscopy;
		}
		set_filename(); 						 //현재표시할 화일명
		strcpy(NOW_FILE2,NOW_FILE);
		strcpy(NOW_FILED2,NOW_FILED);
		newgo=go;
		strcpy(newgo_dir,go.dir);		  //자료실의 경우에만 사용
	}
	else if (mode2==2) {			  //메인->동호회, 동호회->타동호회
		club_mode=Yes;
		ret=find_club(mmc[1]);					  //동호회 찾기
		if (!ret) {
			sprintf(buff,"[%s] 동호회가 없습니다.",mmc[1]);
			goto fail_bbscopy;
		}
		ret=find_go(mmc[2]);		   //옮길 가기코드 찾기
		if (!ret) {
			sprintf(buff,"[%s] 가기코드가 없습니다.",mmc[2]);
			goto fail_bbscopy;
		}
		set_filename(); 	  //현재표시할 화일명
		strcpy(NOW_FILE2,NOW_FILE);
		strcpy(NOW_FILED2,NOW_FILED);
		newgo=go;
		strcpy(newgo_dir,go.dir);		  //자료실의 경우에만 사용
	}
	if (mode2==0&&is_same(oldgo.code,newgo.code)) {
		sprintf(buff,"같은 %s 코드 %s 불가능",oldgo.code,cpstr);
		goto fail_bbscopy;
	}
    if (pf.type<18&&!is_char(BBSTYPE,newgo.type)) {
		sprintf(buff,"게시판또는 자료실 타입이어야 합니다.");
		goto fail_bbscopy;
	}

	go=oldgo;
	club_mode=oldclub_mode;
	club_go=oldclub_go;
	set_nowmenu();
	set_filename(); 							//옮기기기 전 화일명

	fp1=fopen(NOW_FILE,"r+");       //제목화일
	if (fp1==NULL) {
		sprintf(buff,"%s 화일이 없습니다.",NOW_FILE);
		goto fail_bbscopy;
	}
	fp2=fopen(NOW_FILED,"r+");      //내용화일
	fp3=fopen(NOW_FILE2,"r+");      //제목화일 (복사될 가기코드)
	if (fp3==NULL) {
		sprintf(buff,"%s될 게시판화일(%s)이 없습니다.(내용없음)",cpstr,NOW_FILE2);
		if (fp1!=NULL) fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
		goto fail_bbscopy;
	}
	fp4=fopen(NOW_FILED2,"r+");      //내용화일

	//이동목적지의 마지막 번호 파악
	fseekend(fp3);
	new_bbs_rec=ftell(fp3)/sizeof_bbs;
	if (new_bbs_rec<1) {
		lastbbs.num=0;
		lastbbs.posi=0;
		lastbbs.size=0;
	}
	else {
		fseek(fp3,(new_bbs_rec-1)*sizeof_bbs,SEEK_SET);  //제일마지막번호로
		fread(&lastbbs,sizeof_bbs,1,fp3);
	}


	for (i2=0;i2<101;i2++) {
		found=Yes;
		if (mmn[i2]>0) {
			//해당되는 번호 찾기
			rewind(fp1);
			found=No;
			bbs_rec=-1;    //찾은 위치
			while (fread(&bbs,sizeof_bbs,1,fp1)>0) {
				bbs_rec++;
				if (bbs.num==mmn[i2]) {  //번호가 같다.
					found=Yes;
					if (bbs.del&&!dd_mode) found=No;  //삭제된 자료일때는 취소함
                    if (bbs.filesize==0&&!bit(cfg.bbsmode,5)&&!bit(bcfg.modelist,19)&&!dd_mode&&is_pds) found=No;
                    if (found&&go.type!='z'&&go.type!='b') {
						printf("%d %s ",bbs.num,bbs.id);
						if (is_typepds) printf("%s ",bbs.filename);
						printf("%s\n",bbs.title);
						if (cont_mode) found=Yes;	//연속모드
						else {
							printf(" 자료를 %s할까요? (실행:Y 취소:N 연속:A 중지:Q) >>",cpstr);
							lineinput(buff,2);check_han(buff);printf("\n");
							if (is_same(buff,"n")) found=No;
							if (is_same(buff,"a")) cont_mode=Yes;
							if (is_same(buff,"q")) quit_mode=Yes;
						}
					}
					break;
				}
			}
			if (quit_mode) break;
			if (bbs.id[0]==0) strcpy(bbs.id,pf.id);
			if (bbs.name[0]==0) strcpy(bbs.name,bit(bcfg.mode,8)?pf.nick:pf.name);

			//자료실의 경우 이동이면 현재자료 삭제,복사면 그대로 둠
			if (found&&is_typepds&&!is_char("z",oldgo.type)) {
				sprintf(buff,"%s/%s",newgo.dir,bbs.filename);
				sprintf(buff2,"%s/%s",oldgo.dir,bbs.filename);
				if (oldgo.type=='c') get_cdrom(buff2,bbs.num);
				printf("\n %s  -> %s %s중 \n",buff2,buff,cpstr);
                if (!is_file(buff2)) found=No;
                else if (found&&is_file(buff)) {
					printf("\n %s 자료실에 %s 화일이 이미 있습니다.\n",newgo.dir,bbs.filename);
					if (cont_mode) found=Yes;
					else if (yesno("그래도 계속 하겠습니까? (Y:덮어쉬움 n:취소) >>",No)==No) found=No;
				}
				if (found) {
					sprintf(buff,"%s/%s",oldgo.dir,bbs.filename);
					if (oldgo.type=='c') get_cdrom(buff,bbs.num);
                    if (is_file(buff)) {
						sprintf(buff2,"cp %s %s/%s -f",buff,newgo.dir,bbs.filename);
						sys_tem(buff2);
						sprintf(buff,"%s/%s",newgo.dir,bbs.filename);
                        if (!is_file(buff)) {
							printf("\n%s 화일 %s 실패\n",buff,cpstr);
							printf("\n%s 디렉토리를 확인하십시요.\n",buff);
							found=No;
						}
						if (found&&mode==1&&oldgo.type!='c') {
							sprintf(buff,"%s/%s",oldgo.dir,bbs.filename);
							unlink(buff);
						}
					}
				}
                if (no_check_file) found=Yes;
                else if (!found&&pf.type<18) {
                    printf("\n ### 파일복사실패 계속(Y) 취소(N) 무시(A)>>");
                    lineinput2(buff,3);check_han();
                    if (is_same(buff,"a")) {no_check_file=Yes;found=Yes;}
                    else if (!is_same(buff,"n")) found=Yes;
                }
                else if (!found) printf("\n **** %s 화일 %s 실패. ****\n",bbs.filename,cpstr);
			}

			if (bbs_rec>=0&&found) {
				count++;
				//원본 bbs 읽기
				c_lock(1,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
				c_lock(1,NOW_FILE2);				   //LOCK확인  1:확인 0:종료
				fseek(fp1,bbs_rec*sizeof_bbs,SEEK_SET);
				fread(&bbs,sizeof_bbs,1,fp1);
				if (mode==1) {	//만일 mode=1이면 삭제를 한다(이동일때)
					bbs.del=Yes;	 //삭제처리
					fseek(fp1,bbs_rec*sizeof_bbs,SEEK_SET);
					if (!feof(fp1)) fwrite(&bbs,sizeof_bbs,1,fp1);
				}

				//fp2 -> fp4 (내용)
                sync();
				fseek(fp2,bbs.posi,SEEK_SET);
				bbs.posi=lastbbs.posi+lastbbs.size; //새로운 게시판위치
				fseek(fp4,bbs.posi,SEEK_SET);
				for (i=0;i<bbs.size;i++) fputc(fgetc(fp2),fp4);

				//fp1 -> fp3 (제목)
				//새로운 게시판의 마지막 번호 증가
				bbs.num=lastbbs.num+1;
				if (bbs.id[0]==0||bbs.name[0]==0) {
					strcpy(bbs.id,pf.id);
					strcpy(bbs.name,pf.name);
				}
				fseek(fp3,new_bbs_rec*sizeof_bbs,SEEK_SET);
				bbs.del=No;
				if (mode3==0) time(&bbs.date);	 //mode3=1 이면 날짜변경안시킴
				if (!feof(fp3)) fwrite(&bbs,sizeof_bbs,1,fp3);
                sync();
				new_bbs_rec++;
				lastbbs=bbs;	//새로운 마지막 게시판
				if (is_char("un",oldgo.type)) {   //업전용 자료실일때 다운/레벨증가
					ttgo=go;
					go=newgo;
                    read_bcfg();
					bbs_down_up(bbs.filesize,bbs.id,1);
                    go=ttgo;
                    read_bcfg();
				}
				c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
				c_lock(0,NOW_FILE2);				   //LOCK확인  1:확인 0:종료
                sync();
			}
		}
	}

	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (fp3!=NULL) fclose(fp3);
	if (fp4!=NULL) fclose(fp4);
    sync();

	go=oldgo;
	club_mode=oldclub_mode;
	club_go=oldclub_go;
	set_nowmenu();
	set_filename(); 								//옮기기기 전 화일명
	re_bbsnum(3);			//BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
	return;

	fail_bbscopy:
	if (strlen(buff)<1) sprintf(buff,"%s 작업이 취소되었습니다.",cpstr);
	mess(buff);
	go=oldgo;
	club_mode=oldclub_mode;
	club_go=oldclub_go;
	set_nowmenu();
	set_filename(); 								//옮기기기 전 화일명
	re_bbsnum(3);			//BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
	return;
}




file_of_num(char *filename,int num)
{
	char buff[200];
	int i;
	if (num>0) {
		for (i=strlen(filename);i>1;i--)
			if (filename[i]=='.') filename[i-1]+=(char)num;
	}
}




//게시판 찾기
//lt 등의 명령으로 찾게 되면 해당되는 게시물을
//임시화일에 제목만 보관한다.
//re_bbs시에 다시 원래대로 돌아 온다.
//w 또는 각종 명령이 수행될때 다시원래의 화일로 돌아온다.
//현재 검색모드일때 bbs_ltmode=Yes;
bbs_search()
{
	int mode,n,i;
	int mode2;
	FILE *fp1,*fp2;
	char buff[100];
	int ok;
	int found_num=0; //검색한 수

    if (bit(bcfg.modelist,20)&&!bbs_man(pf.id)) {
        mess("검색명령이 금지되었습니다.");
        return;
    }
	re_print=Yes;
	list_mode=No;
	if (mmc[1][0]==0&&!is_same(mmc[0],"lg")&&!is_same(mmc[0],"lst")) {
		printf("\n");
		view("frm/help/_search.hlp");
		bbs_ltmode=No;
		re_bbs=Yes;
		set_filename();
		pressenter();
		return;
	}
	if		(is_same(mmc[0],"lt")) mode=1;
	else if (is_same(mmc[0],"li")) mode=2;
    else if (is_same(mmc[0],"ln")) mode=11;
	else if (is_same(mmc[0],"lf")) mode=3;
	else if (is_same(mmc[0],"lk")) mode=4;
	else if (is_same(mmc[0],"le")) mode=5;
	else if (is_same(mmc[0],"lg")) mode=6;
	else if (is_same(mmc[0],"lsc")) mode=7;
	else if (is_same(mmc[0],"lsl")) mode=8;
	else if (is_same(mmc[0],"lu")) mode=9;
	else if (is_same(mmc[0],"lc")) mode=10;
	else return;
    if (go.type=='k'||go.type=='o') {
        if (mode==2||mode==11) return;  //비공개 경우
    }
	if (bbs_ltmode) {	   //lt상태에서 또 부른경우
		bbs_ltmode=No;
		set_filename();
		check_bbs();
	}

	fp1=fopen(NOW_FILE,"r+");
	fp2=fopen(LT_FILE,"w+");
	strcpy(buff,mmc[1]);
	mode2=0;
	if (is_same(mmc[2],"or")||is_same(mmc[2],"+")) mode2=1;
	if (is_same(mmc[2],"and")||is_same(mmc[2],"*")) mode2=2;
	if (mmc[3][0]==0) mode2=0;


	printf(" \n ............. 검색을 시작합니다................\n");
	while (fread(&bbs,sizeof_bbs,1,fp1)>0) {
		ok=No;
		if (mode==1)
			if ( (mode2==0&&stristr(bbs.title,buff))||
				 (mode2==1&&(stristr(bbs.title,buff)||stristr(bbs.title,mmc[3])))||
				 (mode2==2&&(stristr(bbs.title,buff)&&stristr(bbs.title,mmc[3])))) ok=Yes;
		if (mode==2)
			if ( (mode2==0&&stristr(bbs.id,buff))||
				 (mode2==1&&(stristr(bbs.id,buff)||stristr(bbs.id,mmc[3])))||
				 (mode2==2&&(stristr(bbs.id,buff)&&stristr(bbs.id,mmc[3])))) ok=Yes;
		if (mode==3)
			if ( (mode2==0&&stristr(bbs.filename,buff))||
				 (mode2==1&&(stristr(bbs.filename,buff)||stristr(bbs.filename,mmc[3])))||
				 (mode2==2&&(stristr(bbs.filename,buff)&&stristr(bbs.filename,mmc[3])))) ok=Yes;
		if (mode==4)
			if ( (mode2==0&&stristr(bbs.key,buff))||
				 (mode2==1&&(stristr(bbs.key,buff)||stristr(bbs.key,mmc[3])))||
				 (mode2==2&&(stristr(bbs.key,buff)&&stristr(bbs.key,mmc[3])))) ok=Yes;
		if (mode==4&&stristr(bbs.key,buff)) ok=Yes;
		if (mode==5&&bbs.line<=atoi(buff))	ok=Yes;
		if (mode==6&&bbs.gd>' ')            ok=Yes;
		if (mode==7&&bbs_lsc(buff,0)) ok=Yes;
		if (mode==8&&bbs_lsc(buff,1)) ok=Yes;
		if (mode==9&&(stristr(bbs.title,buff)||bbs_lsc(buff,0))) ok=Yes;
		if (mode==10&&is_same(bbs.tcode,buff)) ok=Yes;
        if (mode==11)
            if ( (mode2==0&&stristr(bbs.name,buff))||
                 (mode2==1&&(stristr(bbs.name,buff)||stristr(bbs.name,mmc[3])))||
                 (mode2==2&&(stristr(bbs.name,buff)&&stristr(bbs.name,mmc[3])))) ok=Yes;

		if (ok) {
			if (go.type=='8') sprintf(bbs.tcode,"%s",found_num);
			if (!feof(fp2)) fwrite(&bbs,sizeof_bbs,1,fp2);
			if (found_num++>5000) {
				printf("검색한 자료가 5000 개가 넘어 중단합니다.");
				pressenter();
				break;
			}
		}
	}
    sync();
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	if (found_num<1) {
		message("해당되는 조건의 자료가 없습니다.");
		bbs_ltmode=No;
		set_filename();
		check_bbs();
	}
	else {
		bbs_ltmode=Yes;
		check_bbs();  //자료의 갯수를 파악하는 루틴
		sprintf(buff,"## 총 %d 개의 자료 검색 ##",found_num);
		message(buff);
		list_mode=No;
	}
}


//내용중 문자찾기	mode:0 :대소문자 구별안함
bbs_lsc(char *str,int mode)
{
	int i,i2,i3,n;
	int found=No;
	char buff[100];
	char c;
	FILE *fp;
	int s1,s2;

	fp=fopen(NOW_FILED,"r+");
	if (fp==NULL) return No;

	strcpy(buff,str);
	n=strlen(buff);
	if (mode==0) strlwr(buff);	   //0 이면 소문자로 검색

	i2=0;
	i=0;
	i3=bbs.posi;
	found=No;
	fseek(fp,bbs.posi,SEEK_SET);
	while (i++<bbs.size&&!found) {
		c=fgetc(fp);
		if (mode==0) c=tolower(c);
		if (i2==0&&c!=buff[i2]) i3++;
		else if (c!=buff[i2])  {
			fseek(fp,i3+1,SEEK_SET);	 //다시 검색 계속
			i2=0;
			i3++;
		}
		else if (++i2>=n) found=Yes;
	}
	if (fp!=NULL) fclose(fp);
	return found;
}

//전체 게시판에서 해당 문자열 찾기
//한페이지 다음으로
bbs_next()
{
	FILE *fp;
	int i,back=No,oldrec;
	BBS bbs2;
	char buff[200];
	re_print=Yes;
	NOW_TOPREC=NOW_BOTREC	;  // 현재화면 마지막 번호
	i=NOW_TOPREC;
	if (bbs_ltmode)   fp=fopen(LT_FILE,"r+");
	else			  fp=fopen(NOW_FILE,"r+");
	if (fp!=NULL) {
		while (1) {
			fseek(fp,(i-1)*sizeof_bbs,SEEK_SET);
			if (!fread(&bbs2,sizeof_bbs,1,fp)) {back=Yes;break;}	//자료없다
			if (bbs2.del&&!dd_mode) {
				i++;
				NOW_TOPREC=i;
				back=Yes;
				continue;
			}	 //운영진이 아닐때 삭제된것 안보임
			break;
		}
		fclose(fp);
	}
	if (back) {
		file2buff(buff,"frm/error/bbsend.err",100);
		del_enter(buff);
		message(buff);
	}
}

//한페이지 이전으로
bbs_back()
{
	FILE *fp;
	BBS bbs2;
	char buff[200];
	int i;
	re_print=Yes;

	if (NOW_TOPREC>=NOW_ALLREC) {
		file2buff(buff,"frm/error/bbstop.err",100);
		del_enter(buff);
		message(buff);
		NOW_TOPREC=NOW_ALLREC;
		return;
	}
	i=bcfg.maxline; if (i<1) i=15;
	if (bbs_ltmode)   fp=fopen(LT_FILE,"r+");
	else			  fp=fopen(NOW_FILE,"r+");
	if (fp!=NULL) {
		while (NOW_TOPREC<NOW_ALLREC) {
			NOW_TOPREC++;
			fseek(fp,(NOW_TOPREC-1)*sizeof_bbs,SEEK_SET);
			if (fread(&bbs2,sizeof_bbs,1,fp)<=0) break;
			if (bbs2.del&&!dd_mode) continue ;	  //운영진이 아닐때 삭제된것 안보임
			if (i--<1) break;
		}
		fclose(fp);
	}
}


//화일보기
bbs_arj()
{
	char filename[200];
	char buff[200];
	re_print=Yes;
	if (list_mode) mmn[1]=bbs.num;
	if (mmn[1]<1||!bbs_num(mmn[1])) {
		message("해당되는 번호가 없습니다.");
		return;
	}
	sprintf(filename,"%s/%s",go.dir,bbs.filename);
    if (bbs.filename[0]==0||!is_file(filename)) {
		message("해당되는 자료의 화일이 없습니다.");
		return;
	}
	if (!((stristr(bbs.filename,".rar"))||
		  (stristr(bbs.filename,".r0"))||
		  (stristr(bbs.filename,".arj"))||
		  (stristr(bbs.filename,".a0"))||
		  (stristr(bbs.filename,".tgz"))||
		  (stristr(bbs.filename,".tar"))||
		  (stristr(bbs.filename,".lha"))||
		  (stristr(bbs.filename,".lzh"))||
		  (stristr(bbs.filename,".zip")))) {
			printf("\n 지원하는 압축 포멧은 다음과 같습니다.\n");
			printf(" RAR / ARJ / TGZ / TAR / LHA / ZIP\n");
			pressenter();
			return;
	}
	unlink(TMP_FILE2);
	if		(stristr(bbs.filename,".rar")) {sprintf(buff,"rar l %s > %s",filename,TMP_FILE2);sys_tem(buff);    }
	else if (stristr(bbs.filename,".r0")) {sprintf(buff,"rar l %s > %s",filename,TMP_FILE2);sys_tem(buff);    }
	else if (stristr(bbs.filename,".arj")) {sprintf(buff,"unarj l %s > %s",filename,TMP_FILE2);sys_tem(buff);  }
	else if (stristr(bbs.filename,".a0")) {sprintf(buff,"unarj l %s > %s",filename,TMP_FILE2);sys_tem(buff);  }
	else if (stristr(bbs.filename,".tgz")) {sprintf(buff,"tar tvfz %s > %s",filename,TMP_FILE2);sys_tem(buff);}
	else if (stristr(bbs.filename,".tar")) {sprintf(buff,"tar tvf %s > %s",filename,TMP_FILE2);sys_tem(buff);  }
	else {
		printf("\n\n        압축 화일 보기   %s\n",bbs.filename);
		sprintf(buff,"bin/al %s",filename);sys_tem(buff);
		pressenter();
	}
    if (is_file(TMP_FILE2)) {
		printf("\n\n        압축 화일 보기   %s\n",bbs.filename);
		view_text(TMP_FILE2);
		unlink(TMP_FILE2);
	}
	re_print=Yes;
}


//제목에서 안시를 지운다.
bbs_dansi(int num)
{
	int i;
	BBS bbs2;
	FILE *fp;

	if(bbs_num(num)==No) {
		printf("해당되는 번호의 자료를 찾을수 없습니다.");
		pressenter();
		return;
	}

	if(check_del()) {	  //삭제가능한 레벨인가?
		printf("\n 원래 제목:%s\n",bbs.title);
		del_esc(bbs.title);
		del_esc(bbs.filename);
		del_esc(bbs.id);
		del_esc(bbs.name);
		del_esc(bbs.key);
		printf(" 변경 제목:%s\n",bbs.title);
		if (yesno("변경사항을 저장할까요?",Yes)) {
			c_lock(1,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
			fp=fopen(NOW_FILE,"r+");
			if (fp!=NULL) {
				i=0;
				while (fread(&bbs2,sizeof_bbs,1,fp)) {
					if (bbs2.num==bbs.num) {
						fseek(fp,i*sizeof_bbs,SEEK_SET);
						if (!feof(fp )) fwrite(&bbs,sizeof_bbs,1,fp);
						break;
					}
					i++;
				}
				fclose(fp);
			}
			c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
		}
	}
	re_print=Yes;
}


//전체에서 제목에서 안시를 지운다.
bbs_dansiall()
{
	int i,total;
	BBS bbs2;
	FILE *fp;

	printf ("\n %s 화일의 안시를 전부 제거할까요?",NOW_FILE);
	if (yesno("(제거:Y 취소:N)",No)) {
		if(check_del()) {	  //삭제가능한 레벨인가?
			c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
			fp=fopen(NOW_FILE,"r+");
			if (fp!=NULL) {
				i=0;
				fseekend(fp);
				total=ftell(fp)/sizeof_bbs;
				for (i=0;i<total;i++) {
					fseek(fp,i*sizeof_bbs,SEEK_SET);
					if (fread(&bbs2,sizeof_bbs,1,fp)<1) break;
					del_esc(bbs2.title);
					del_esc(bbs2.filename);
					del_esc(bbs2.id);
					del_esc(bbs2.name);
					del_esc(bbs2.key);
					fseek(fp,i*sizeof_bbs,SEEK_SET);
					if (!feof(fp )) fwrite(&bbs2,sizeof_bbs,1,fp);
					printf(".");
				}
				fclose(fp);
			}
			c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
		}
	}
	re_print=Yes;
}




//자료실에서 화일을 바로 보여준다.
bbs_type()	  //해당번호 내용출력
{
	char filename[200];
	re_print=Yes;
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	if (mmn[1]==0) return;
	if (!bbs_num(mmn[1])) {
		printf("\n ### %d 번 자료는 없습니다. ###\n",mmn[1]);
		pressenter();
		return;
	}
	if (check_bbspasswd()==No) return;
	if (!is_typepds) {
		printf("\n 자료실에서 화일의 내용을 바로 보여주는 명령입니다.\n");
		pressenter();
		return;
	}
	sprintf(filename,"%s/%s",go.dir,bbs.filename);
    if (bbs.filename[0]==0||!is_file(filename)) return;
	if (stristr(filename,".doc")||
		stristr(filename,".txt")||
		stristr(filename,".cap")||
		stristr(filename,".bat")||
		stristr(filename,".scf")) {
		printf("\n %s %s\n",bbs.filename,bbs.title);
		printfL2();
		view_text(filename);
		printf("\n");
		printfL2();
		pressenter();
		return;
	}
	printf("\n 내용을 볼수 없는 화일입니다.\n");
	pressenter();
	return;
}



typedef struct {
	char  id[9];
	time_t date;
	char  mode;
	int   num;
} OKREC;


bbs_ok(int mode)    //ok 처리  0:반대 1:찬성 2:삭제 3:보존 4:기권  6:  7:  8:
{
	FILE *fp,*fp2;
	int key=No;
    int num,i,n,n2,count,i1,i2,i3,mode2;
    char buff[200];
    char buff2[200];
	char filename[300];
	BBS bbs1;
	OKREC okrec;
	re_print=Yes;
	printf("\n");

	if (!is_typebbs&&(mode==2||mode==3||mode==4||mode==12)) {
		message("게시판전용명령");
		return;
	}
	strcpy(buff2,"");
		 if (mode==0) strcpy(buff2,"NO(반대)");
	else if (mode==1) strcpy(buff2,"OK(찬성)");
	else if (mode==2) strcpy(buff2,"DE(삭제)");
	else if (mode==3) strcpy(buff2,"KE(보존)");
	else if (mode==4) strcpy(buff2,"PA(기권)");

	if (bbs_ltmode||guest_mode) {
		message("조건 지정에서는 안됩니다.");
		return;
	}
	if (mmn[0]==0) mmn[0]=mmn[1];	  //그냥 ok만 했을때
	if (list_mode&&mmn[0]==0) mmn[0]=NOW_NO;	 //그냥 ok만 했을때
    mode2=mmn[0];
    if (mode<5&&mmn[0]<=0) {
		printf("\n OK [찬성번호]  NO [반대번호]  DE [삭제번호]  KE [보존번호]  PA [보류번호]\n");
		pressenter();
		return;
	}
	if (mode==11||(mode==12&&is_typebbs)) {
		for (i=0;i<101;i++) {
			if (mmn[i]>0) {
				if (bbs_num(mmn[i])==Yes){	//자료번호 찾기
					//게시물 총수
					c_lock(1,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
					if ((fp=fopen(NOW_FILE,"r+"))!=NULL) {      //제목화일
						fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
							 if (mode==11) bbs.ok=0;
						else if (mode==12) bbs.down=0;
						if (!feof(fp )) fwrite(&bbs,sizeof_bbs,1,fp);
						fclose(fp);
					}
					c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
				}
			}
		}
		return;
	}


	if (mode==6||mode==7) {
		n=now_month();
		if (mode==7) {
			n--;if (n<1) n+=12;
		}
		get_okrec_filename(filename,n);
		i1=i2=i3=0;
        if ((fp2=fopen(TMP_FILE,"w+"))==NULL) return;
        if ((fp=fopen(filename,"r"))==NULL) {fclose(fp2);return;}
        while (fread(&okrec,sizeof(okrec),1,fp)) {
            if (mode2==0||(mode2>0&&mode2==okrec.num)) {
                mode2=mmn[0];
                i1++;
                if (okrec.mode=='1') i2++;
                else                 i3++;
                fprintf(fp2,"%s %6d %-8s %s   OK:%d NO:%d ALL:%d\n",datestr(3,okrec.date),okrec.num,okrec.id,okrec.mode=='1'?"OK":"NO",i2,i3,i1);
            }
        }
        fclose(fp);
        fclose(fp2);
		view_text(TMP_FILE);
		unlink(TMP_FILE);
		return;
	}

	if (mode==8) {
		n=now_month();
		if (yesno("\n   이번달의 찬성/반대 자료를 삭제하겠습니까? (Y/n) >>")) {
			get_okrec_filename(filename,n);
			unlink(filename);
			printf(" 삭제 되었습니다. \n");
		}
		else printf(" 취소 되었습니다. \n");
		pressenter();
		return;
	}


	//지난 화일여부 검사
	n=now_month()-2;
	if (n<1) n+=12;
	get_okrec_filename(filename,n);
    if (is_file(filename)) {
		sprintf(filename,"rm -f %s/bbsdata/okrec/%s.*",club_go.dir,go.code);
		sys_tem(filename);
	}

	get_okrec_filename(filename,now_month());

	if ((fp=fopen(filename,"r+"))!=NULL) {
		while (fread(&okrec,sizeof(okrec),1,fp)) {
			if (is_same(okrec.id,pf.id)) {
				for (i=0;i<101;i++) if (mmn[i]>0&&okrec.num==mmn[i]) mmn[i]=0;
			}
		}
		fclose(fp);
	}

	count=0;
	num=mmn[0];
	fp2=fopen(filename,"r+");
    if (fp2==NULL) {
        sprintf(buff,"%s/bbsdata/okrec/",club_go.dir);check_dir(buff);
        fp2=fopen(filename,"w+");
    }
    if (fp2==NULL) return;
	if (mmn[0]==mmn[1]) mmn[1]=0;

	for (n=0;n<101;n++) {
		if (mmn[n]>0&&bbs_num(mmn[n])){  //자료번호 찾기
			//게시물 총수
			if ((fp=fopen(NOW_FILE,"r+"))==NULL) continue;     //제목화일
			fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
			fread(&bbs1,sizeof_bbs,1,fp);
			bbs1.ok+=1000000;
			okrec.mode='0';
			bbs1.ok+=1000000;
				 if (mode==0) {bbs1.ok+=1000		;okrec.mode='0';}
			else if (mode==1) {bbs1.ok++			;okrec.mode='1';}
			else if (mode==2) {bbs1.down++			;okrec.mode='2';}       //de
			else if (mode==3) {bbs1.down+=1000		;okrec.mode='3';}       //ke
			else if (mode==4) {bbs1.down+=1000000	;okrec.mode='4';}       //pe
			else			  { 					;okrec.mode='9';}
			c_lock(1,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
			fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
			if (!feof(fp )) fwrite(&bbs1,sizeof_bbs,1,fp);
			fclose(fp);
			c_lock(0,NOW_FILE); 				  //LOCK확인  1:확인 0:종료
			sprintf(buff,"%d 번 자료에 [%s] 지정이 되었습니다.",mmn[n],buff2);
			printf("  %s\n",buff);
			strcpy(okrec.id,pf.id);
			okrec.num=mmn[n];
			time(&okrec.date);
            if (fp2!=NULL&&okrec.num>0) {
                fseekend(fp2);
                n2=ftell(fp2)/sizeof(okrec);
                if (n2<0||n2>99999) n2=0;
                fseek(fp2,n2*sizeof(okrec),SEEK_SET);
                fwrite(&okrec,sizeof(okrec),1,fp2);
            }
			count++;
		}
	}
	if (fp2!=NULL) fclose(fp2);
	if (count==0) printf(" ### 이미 OK/NO 응답을 하였습니다. ###");
	pressenter();
}



get_okrec_filename(char *filename,int n)
{
	sprintf(filename,"%s/bbsdata/okrec/%s.%d",club_go.dir,go.code,n);
}





read_bbsgood_color()
{
	FILE *fp;
	int i,i2;
	char buff[300];
	if (club_mode) sprintf(buff,"%s/system/bbsgood.goo",club_go.dir);
    if (!club_mode||!is_file(buff)) sprintf(buff,GOOD_FILE);
	//추천
	if ((fp=fopen(buff,"r+"))!=NULL) {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				fread(&agood[i][i2],sizeof_agood,1,fp);
			}
		}
		fclose(fp);
	}
	else {
		for(i=0;i<4;i++) {
			for(i2=0;i2<10;i2++) {
				agood[i][i2].mark=0;
				agood[i][i2].count=0;
			}
		}
	}
	//색상읽기
	strchng2(buff,".goo",".gol");
	if ((fp=fopen(buff,"r+"))!=NULL) {
		for(i=0;i<10;i++) {
			fread(&acolor[i],sizeof_acolor,1,fp);
		}
		fclose(fp);
	}
	else {
		for(i=0;i<10;i++) {
			acolor[i].mark=0;
			acolor[i].color_char=0;
			acolor[i].color_back=0;
			strcpy(acolor[i].name,"");
		}
	}
	return;
}


check_bbs()  //자료의 갯수를 파악하는 루틴
{

	FILE *fp;
    int ii,n,count=0;
	NOW_REC=0			;	   // 현재자료번호
	NOW_TOPREC=0		;	   // 현재화면 상단번호
	NOW_BOTREC=0		;	   // 현재화면 상단번호
	NOW_ALLREC=0		;	   // 현재총자료번호
	NOW_NO=0			;	   // 현재자료번호
	NOW_ALLNO=0 		;	   // 현재총자료번호
	NOW_TOPNO=0 		;	   // 현재화면상단 번호
	//화일이 없으면 새로 만듬
	if (bbs_ltmode) fp=fopen(LT_FILE,"r+");
	else			fp=fopen(NOW_FILE,"r+");
	if( fp==NULL ) return;
	//게시물 총수
	fseekend(fp);
	n=ftell(fp);
	NOW_ALLREC=n/sizeof_bbs;
	if (NOW_ALLREC>0) {
		ii=NOW_ALLREC;
        while (ii>0) {
			fseek(fp,(ii-1)*sizeof_bbs,SEEK_SET);
            if (count++>5000) goto _break;
			if (!fread(&bbs,sizeof_bbs,1,fp)) break;
				if (bbs.del&&!dd_mode) {ii--;continue;}
                if (bbs.filesize==0&&!bit(cfg.bbsmode,5)&&!bit(bcfg.modelist,19)&&!dd_mode&&is_pds) {ii--;continue;}
				if (bbs.num<0) bbs.num=0;
                _break:
				NOW_TOPREC=NOW_ALLREC;
				NOW_REC=NOW_ALLREC-1;
				NOW_ALLNO=bbs.num;
				NOW_TOPNO=bbs.num;
				NOW_NO=bbs.num;
				break;
		}
	}
	fclose(fp);
}

//게시판 번호로 찾기
view_pfb()
{
	BBS oldbbs;
	if (is_char("ok",go.type)||!is_bbspds) return;
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	oldbbs=bbs;
    if (bbs_num(mmn[1])) view_pf(bbs.id);
	bbs=oldbbs;
}





/*

 [ PUT ]	  기본유틸리티(담당:)					   452/466		 (총405건)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 제  목:dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
 화일명:dddddd		   크기:30615		 UP:06/15  DN:98/09/08 13:33  등록자:xxx
 ───────────────────────────────────────

 제  목:1234567890123456789012345678901234567890
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 제  목:[GREEN] 그린호스트 VER 2.40 			 분류:COMM	   전송:9	   1/ 6
 화일명:grn240.lzh	   크기:380786	 UP:98/08/31 DN:98/09/08   등록자:ct0052
 ───────────────────────────────────────
 (c) GREEN SOFT

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
 제  목:dddddddddddddddddddddddddddddddddddddddd 분류:put	   전송:   3   1/1
 화일명:dddddd		   크기:30615	   UP:98/06/15	DN:98/09/08   등록자:xxxx
 ───────────────────────────────────────
*/

