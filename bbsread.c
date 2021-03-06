///////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
//	  게시판 관련 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//mode=0:일반 1:새로 8:연속 9:pr 로 보여준다.
//mode=-1:뒤로  1:다음
bbs_read(int mode)	//해당번호 내용출력
{
	FILE *fp1,*fp2;
	char buf[280];
	char buff[280];
	char filename[280];
	char buff1[3502];
	char buff2[3502];
	char ch=0;
	char all_mailmode=No;
    char mailmode=No;
	char view_jpg_ok=No;
	char found=No;
	char old_gotype=go.type;
    int temp_key,mail_mode=No;
	int pagepos[5000];
	int i,ii,disp_x,i2,is_dsp=No;
    int page,pos,count,disp_y;
	int stop_count,last_pos,last_pos2,last_pos3;
	int bbs_mode=is_typebbs;
	BBS bbs2,nowbbs;
    int str1_count=0;
	char TMP_FILE4[280];
	char jpg_file[200];

    _last2=5;;;;;;;;;;;;;;;;;;;;;;;;
	if ((is_same(go.code,"rmail")||
         is_same(go.code,"wmail")||
         is_same(go.code,"mbox")||
		 is_same(go.code,"cmail"))) {
         mailmode=Yes;
		if (bbs.del) {
			list_mode=No;
			re_print=Yes;
			return;
		}
		if (dd_mode) set_ddmode(2); 	 //삭제된것 안보이게
	}
    else {
        sprintf(buff,"%s/logo/%s.nre",club_go.dir,go.code);
        if (is_file(buff)) {view(buff);goto _fail;}
        sprintf(buff,"%s/logo/all.nre",club_go.dir);
        if (is_file(buff)) {view(buff);goto _fail;}
    }
    if (!mailmode&&bit(limit_mode,2)) {
        if (!view("frm/pf/notread.log")) mess("읽기금지");
        return;
    }

	if (go.type=='w'&&bbsreadall&&!bit(bcfg.jpgmode,2)) {        //내용더이상없음
		list_mode=No;
		re_print=Yes;
		return;
	}
    if (bit(bcfg.modelist,8)&&bbs.gd=='*') bbs.gd=' ';
	nowbbs=bbs;
	g_char=gd_chr(bbs_mode);	 //추천/조회/다운지정
	sprintf(filename,"%s/logo/%s.lir",club_go.dir,go.code);
	if (check_bbspasswd()==No) return;
	if (guest_mode&&(is_same(go.code,"rmail")||is_same(go.code,"mbox")||is_same(go.code,"cmail"))) return;

    mail_mode=(is_same(go.code,"rmail")||is_same(go.code,"cmail")||is_same(go.code,"mbox"));

	if (bbs_man(pf.id));
	else if (bbs_mode&&bit(bcfg.mode,29)&&in_file2("system/node_net.txt",tty)) return;
    else if (mail_mode) ;
	else if (is_same(go.code,"notice"));
	else if (check_type('r')) return;   //메뉴 가능 등급확인
	else if (ss_check(7)) return;	//읽기 금지 등급
	else if (bit(bcfg.modelist,5)&&!in_file2(filename,pf.id)) return;

	if (bit(bcfg.automode,1)||
		bit(bcfg.automode,2)||
		bit(bcfg.automode,3)) auto_bbs();	  //자동모드
	re_print=No;

	list_mode=Yes;		//내용출력모드
	if (bcfg.listline<1) bcfg.listline=16;
	//출력시작위치찾기
	disp_x=bcfg.disp_x;
	disp_y=bcfg.disp_y;
	bbs.title[60]=0;del_tab(bbs.title);all_trim(bbs.title);
	del_space(bbs.filename);del_tab(bbs.filename);del_esc(bbs.filename);bbs.filename[12]=0;
	//vtx이지만 데이타가 아님
	if (set_bbs_logo("tor",filename)) view(filename);
	if (bit(bcfg.modelist,0)) return;	//내용안보임
	if (bit(bcfg.mode,24)|| 				//내용->form으로
        go.type=='i'||(mail_mode&&is_same(bbs.key,"카드"))) {            //로고게시판
		view7(NOW_FILED,bbs.posi,bbs.size);
		getchar2();
		if (bbs_mode) {uall.read++;uall.readsize+=bbs.size;}
		return;
	}

    if (go.type=='v'||go.type=='+') {
		cls();
		if (go.type=='v') set_stat2(14,"VTX정보 검색중");
		else			  set_stat2(14,"ANSI정보 검색중");
		if (go.type=='v') printf(ESCVTX);
		view6(NOW_FILED,bbs.posi,bbs.size);
		getchar2();
		if(go.type=='v') printf(VTXOFF);
		if (bbs_mode) {uall.read++;uall.readsize+=bbs.size;}
		re_print=Yes;
		list_mode=No;
		set_stat(1,"");
		return;
	}

    pf.todaymode=set_bit(pf.todaymode,4,1);   //오늘작업
	all_mailmode=No;
	//전체메일일때처리 읽기,다운수처리
	if (is_same(go.code,"rmail")||is_same(go.code,"mbox")) {
		if (is_same(bbs.key,"전체메일")||is_same(bbs.key,"전체카드")) {
			strcpy(filename,"menu/_allmail.tit");
			if ((fp2=fopen(filename,"r"))!=NULL) {
				i=0;
				i2=-1;
				found=No;
				while (fread(&bbs2,sizeof_bbs,1,fp2)) {
					if (!bbs.del&&is_same(bbs2.id,bbs.id)&&bbs2.date==bbs.date) {
						bbs.posi=bbs2.posi;
						bbs.size=bbs2.size;
						bbs.line=bbs2.line;
						strcpy(bbs.filename,bbs2.filename);
						strcpy(bbs.title,bbs2.title);
						if (!is_same(go.code,"cmail")) found=Yes;
						all_mailmode=Yes;
						i2=i;
						break;
					}
					i++;
				}
				fclose(fp2);
				//전체메일읽은수 증가
				if (i2>=0&&found&&(fp2=fopen(filename,"r+"))!=NULL) {
					bbs2.read++;		 //읽기수 증가
					time(&bbs2.date2);
					fseek(fp2,i2*sizeof_bbs,SEEK_SET);
					fwrite(&bbs2,sizeof_bbs,1,fp2);
					fclose(fp2);
				}
			}
		}
	}

	//데이타파일
	strcpy(filename,all_mailmode?"menu/_allmail.txt":NOW_FILED);
	if ((fp1=fopen(filename,"r"))==NULL) goto view_ok2;
    if (view_pos>=bbs.posi+bbs.size-1) view_pos=bbs.posi+bbs.size-1;
    if (view_pos<bbs.posi) view_pos=bbs.posi;


	//현재page 찾기
	N_page=1;
	stop_count=14;		  //화면이 멈추는 줄수
    //현재PAGE수,이전위치계산
	fseek(fp1,bbs.posi,SEEK_SET);
	count=0;
    str1_count=0;

    for(i=bbs.posi;i<view_pos&&!feof(fp1);i++) {
        ch=fgetc(fp1);if(ch=='\n'||str1_count++>(79-disp_x)) {count++;str1_count=0;}
		if (count>=bcfg.listline||!bit(bcfg.mode,3)&&count>=stop_count) {
			count=0;
			N_page++;
		}
	}
    if (mode==-1) {      //이전위치
        count=0;
        for(;view_pos>=bbs.posi&&!feof(fp1);view_pos--) {
            fseek(fp1,view_pos,SEEK_SET);
            ch=fgetc(fp1);if(ch=='\n'||str1_count++>(79-disp_x)) {count++;str1_count=0;}
            if (count>bcfg.listline+1||!bit(bcfg.mode,3)&&count>stop_count+1) {
                view_pos++;
                count=0;
                N_page--;
                break;
            }
        }
    }
    if (mode==1) {      //다음위치
        count=0;
        fseek(fp1,view_pos,SEEK_SET);
        ii=view_pos;
        for(;view_pos<bbs.posi+bbs.size&&!feof(fp1);view_pos++) {
            ch=fgetc(fp1);if(ch=='\n'||str1_count++>(79-disp_x)) {count++;str1_count=0;}
            if (count>bcfg.listline||!bit(bcfg.mode,3)&&count>stop_count) {
                view_pos++;
                count=0;
                N_page++;
                break;
            }
        }
        if (view_pos>=bbs.posi+bbs.size) view_pos=ii;
    }

    if (view_pos>bbs.posi+bbs.size-1) view_pos=bbs.posi+bbs.size-1;
    if (view_pos<bbs.posi) view_pos=bbs.posi;
	if (N_page<1) N_page=1;

	view_jpg_ok=No;
	if (go.type=='w'&&view_num<99&&stristr(bbs.filename,".jpg")) {
		//그림화일명확인
		sprintf(jpg_file,"%s/%s",go.dir,bbs.filename);
		file_of_num(jpg_file,view_num);
		view_jpg_ok=is_file(jpg_file);
	}

	sprintf(buff,"%s/%d.jpg",go.dir,bbs.num);
	if (!view_jpg_ok&&bit(bcfg.jpgmode,1)&&is_file(buff)) {
		go.type='w';
		strcpy(jpg_file,buff);
		view_jpg_ok=Yes;
	}

	//상단화면 출력
	if (bcfg.pos_y1>1) xy(1,bcfg.pos_y1);
	if (set_bbs_logo("ftm",buff)) {
		if (!view_jpg_ok) strchng2(buff,".wtx",".wt2");
		view(buff);
	}

	if (view_jpg_ok) {
		disp_x=bcfg.jpg_x;disp_y=bcfg.jpg_y;
	}
	if (disp_x<1) disp_x=1;
	if (disp_y<1) disp_y=7;
	xy(disp_x,disp_y);
	if (bit(cfg.mode,13)) printf(" ");

	if (fp1==NULL) goto view_ok2;
    fseek(fp1,view_pos,SEEK_SET);
	if (feof(fp1)||bbs.size<1) {
		xy(1,10);
		goto view_ok2;
	}

	count=0;
	page=0;
	stop_count=14;		  //화면이 멈추는 줄수
	temp_key=No;
    str1_count=0;
	ch=0;

    for(ii=view_pos;ii<(bbs.posi+bbs.size);ii++) {
		//다음줄이 나올때까지 반복하여 화면에 출력한다.
		ch=fgetc(fp1);
        if (ch=='') str1_count=0;
		putchar(ch);
		if (feof(fp1)) break;
		if(ch=='\n'||str1_count++>(79-disp_x)) {
            count++;
            disp_y++;
            str1_count=0;
            if (count>=bcfg.listline) break;
            if (!bit(bcfg.mode,3)&&count>stop_count) break;
            if (disp_x>1) xy(disp_x,disp_y);
            if (bit(cfg.mode,13)) printf(" ");
		}
	}

	view_ok2:

/*
    if (bbs.size>1&&!feof(fp1)&&view_pos<(bbs.posi+bbs.size)) {
		view_pos=ftell(fp1);  //볼내용남았을때
	}
*/
    bbsreadall=No;           //내용남음
    if (bbs.size<1||view_pos>=(bbs.posi+bbs.size)) {
		bbsreadall=Yes; 	 //내용다읽음
	}
//    printf("Size:%d pos:%d VIEW=%d READALL=%d",bbs.size,view_pos,view_pos,bbsreadall);pressenter();;;;;;

	if (fp1!=NULL) fclose(fp1);
	if (is_typewtx&&view_num<99&&view_jpg_ok) {
		set_stat2(14,"화상정보 검색중");    //외부
		printf("[s");
		mview(No,jpg_file);
		printf("[u");
		set_stat(1,"");
		if (is_same(bbs.key,"연속")||is_same(bbs.key,"만화")||is_same(bbs.key,"연속만화")) view_num++;
		else view_num=99;	//그이외는 안보여줌
	}
	if (ch!='\n') printf("\n");    //다음줄표시
	if (disp_x>1) printf("[70D");    //다음줄표시
    if (bbs.size<1||disp_y<5) xy(1,10-1);
    if (bcfg.pos_y2>1) xy(1,bcfg.pos_y2-1);
    printf("\n");
	if (!view("%s/logo/%s.bl2",club_go.dir,go.code)) {
		set_bbs_logo("bl2",buff);
		if (!view(buff)) {
			strchng2(buff,"bl2","bl1");
			view(buff);
		}
	}

	if (is_same(bbs.key,"응답")||bit(bcfg.modeok,1)) {
		sprintf(buff,"%s/logo/%s.ok2",club_go.dir,go.code);
		if (!view(buff)) view("frm/bbs/bbsok2.scr");
		i=numinput();
		if (i==1) bbs_ok(1);
		if (i==2) bbs_ok(0);
		if (i==3) bbs_ok(2);
		if (i==4) bbs_ok(3);
		if (i==5) bbs_ok(4);
	}
	if (is_same(bbs.key,"보류")||bit(bcfg.modeok,11)) {
		sprintf(buff,"%s/logo/%s.ok2",club_go.dir,go.code);
		if (!view(buff)) view("frm/bbs/bbsok2.scr");
		i=numinput();
		if (i==1) bbs_ok(2);
		if (i==2) bbs_ok(3);
		if (i==3) bbs_ok(4);
	}
	if (is_same(bbs.key,"답변")&&!is_same(bbs.id,pf.id)&&(bbs_man(bbs.id)||bit(bcfg.modelist,4))) strcpy(auto_cmd,"rn");
	if (is_same(bbs.key,"답장")&&!is_same(bbs.id,pf.id)&&(bbs_man(bbs.id)||bit(bcfg.modelist,4))) strcpy(auto_cmd,"rs");
	if (view_jpg_ok&&bit(bcfg.jpgmode,1)) go.type=old_gotype;
	bbs=nowbbs;
	if (!is_read_numn(bbs.num)) {
		if (is_char("v+w",go.type)) set_rec(1,14);
		else						set_rec(1,7);
	}
    _fail:
    return;
}


//게시물 읽을시 특정 번호에 해당 되는 TEXT 읽어오기
auto_bbs()
{
	char filename[300];
	char buffkey[30];
	int oldview_pos=view_pos;
	if (strlen(go.dir)<1) return;
	strcpy(buffkey,bbs.key);
	del_space(buffkey);
	if (bit(bcfg.automode,6)) sprintf(filename,"%s%s.txt",go.dir,buffkey);
	else					  sprintf(filename,"%s%d.txt",go.dir,bbs.num);
	del_space(filename);del_esc(filename);
	if (!is_file(filename)) return;
	if (bit(bcfg.automode,5)&&bbs.date==file_time(filename)&&bbs.size==file_size(filename)) return;    //file의 최종수정시간 같을때
	//그 이외의 경우 파일에서 읽어서 내용으로 만듬
	sys_tem("bin/ed_bbs 1 '%s' '%s' '%d' '%s' '%s' '%s' %d",NOW_FILE,pf.id,NOW_REC,club_go.code,go.code,filename,bcfg.automode);
	bbs_num(bbs.num);
	view_pos=oldview_pos;
}


//게시물 진입시 특정 게시판 내용 불러오기
auto_bbs2()
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char filename[300];
	char TNOW_FILE[300];
	char TNOW_FILED[300];
	char buffkey[30];
	BBS bbs2,bbs3;
	char key_mode;
	int  i,n,new_pos;
	int oldview_pos=view_pos;
	del_space(bcfg.inbbs);del_esc3(bcfg.inbbs);
	if (list_mode||!bit(bcfg.automode,15)||strlen(bcfg.inbbs)<1||view_pos>0) return;
	if (is_same(go.code,bcfg.inbbs)) return;
	sprintf(TNOW_FILE,"%s/menu/%s.tit",club_go.dir,bcfg.inbbs);
	sprintf(TNOW_FILED,"%s/menu/%s.txt",club_go.dir,bcfg.inbbs);
	if (!is_file(TNOW_FILE)||!is_file(TNOW_FILED)) return;
	//file의 최종수정시간 같을때
	if (!bit(bcfg.automode,16)&&file_time(NOW_FILED)<file_time(TNOW_FILED)) return;
	fp1=fopen(TNOW_FILE,"r");
	fp2=fopen(TNOW_FILED,"r");
	if (fp1==NULL||fp2==NULL) {mess("%s 파일 열기실패",NOW_FILE);return;}
	c_lock(1,NOW_FILE);
	fp3=fopen(NOW_FILE,"w");
	fp4=fopen(NOW_FILED,"w");
	if (fp3==NULL||fp4==NULL) {mess("%s 파일 열기실패",TNOW_FILE);return;}
	key_mode=bit(bcfg.automode,17);
	all_trim(bcfg.inbbskey);
	if (strlen(bcfg.inbbskey)<1) key_mode=9;  //전부해당
	new_pos=0;
	printf(" ## 잠시만 기다려 주세요.. 자료갱신중입니다....\n");
	fflush(stdout);
	while (fread(&bbs,sizeof_bbs,1,fp1)) {
		if (bbs.del) continue;
        else if (key_mode==0&&!stristr(bbs.title,bcfg.inbbskey)) continue;
		else if (key_mode==1&&!is_same(bbs.key,bcfg.inbbskey)) continue;
		fseek(fp2,bbs.posi,SEEK_SET);
		fseek(fp4,new_pos,SEEK_SET);
		i=0;
		while (!feof(fp2)&&i<bbs.size) {
			fputc(fgetc(fp2),fp4);			   //내용에 저장
			i++;
		}
		bbs.size=i;
		bbs.posi=new_pos;
		new_pos+=bbs.size;
		if (!fwrite(&bbs,sizeof_bbs,1,fp3)) break;
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	c_lock(0,NOW_FILE);
	touch_file(NOW_FILED);
	touch_file(TNOW_FILED);
	view_pos=oldview_pos;
}


view7(char *filename,int posi,int size)
{
	FILE *fp;
    char buff[100000];
    int i=0;
    bzero(buff,100000);
    if ((fp=fopen(filename,"r"))==NULL) return No;
    fseek(fp,posi,SEEK_SET);
    while (i<100000&&i<size&&!feof(fp)) {
        buff[i++]=getc(fp);
    }
    fclose(fp);
    view3(buff);
    return Yes;
}


