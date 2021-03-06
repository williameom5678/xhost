///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
//	  게시판 관련 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include <unistd.h>    //디렉토리 화일명 변환함수때문에 필요
#include <dirent.h>    //디렉토리 화일명 변환함수때문에 필요

#ifndef CD
// 시디롬 화일 목록
typedef struct {
	int num;				// 일련 번호
	char dir[150];			// 디렉토리
	char filename[50];		// 화일명
	int  filesize;			// 화일의 크기
} CD;
#endif

BBS abbs[102];


//게시판 다운		mode==2:SEE
bbs_down(int mode)
{
	FILE *fp,*fp2;
	char buff[200];
	char buff2[200];
	char newdir[100];
	char filename[200];
	char filename2[200];
	char ch;
	BBS oldbbs;
	int old_level=pf.level;
	int old_down=pf.down;
	int i,n,i2,key,count,ret,tsize;
	int tNOW_NO,tNOW_REC,tNOW_ALLREC,tNOW_TOPREC;
	tNOW_NO=NOW_NO;tNOW_REC=NOW_REC;tNOW_TOPREC=NOW_TOPREC;tNOW_ALLREC=NOW_ALLREC;

	oldbbs=bbs;

    if (bit(bcfg.modelist,21)&&!bbs_man(pf.id)) {
        mess("다운명령이 금지되었습니다.");
        return No;
    }

    re_print=Yes;
    if (bit(limit_mode,3)) {
        if (!view("frm/pf/notdown.log")) mess("다운금지");
        return No;
    }
    _last2=4;;;;;;;;;;;;;;;;;;;;;;;;

	sprintf(buff,"%s/logo/%s.ndn",club_go.dir,go.code);
	if (is_file(buff)) {view(buff);goto _fail;}
    sprintf(buff,"%s/logo/all.ndn",club_go.dir);
	if (is_file(buff)) {view(buff);goto _fail;}

	sprintf(filename,"%s/logo/%s.lir",club_go.dir,go.code);

	if (!is_char("gy",go.type)&&ss_check(3))   goto _fail;   //다운 금지
	if (!is_char("gy",go.type)&&check_warn(3)) goto _fail;  //경고시 다운금지

	if (is_same(go.code,"rmail")||is_same(go.code,"mbox")||is_same(go.code,"cmail")) {
        set_stat(8,bbs.title);     //다운중임
		mail_down();
        set_stat(1,"");     //다운중임
		return Yes;
	}
	else if (bit(bcfg.modelist,5)&&!in_file2(filename,pf.id)) return No; //지정된 ID만 가능한가?
	else if (is_same(go.code,"bbs_all"));
	else if (is_typepds) return pds_down(mode);  //자료실이면 파일다운으로..


	key=No;  //인터넷라인

	view("frm/down/bbsdown.log");

	//화면 출력모드일때 dn만 누름
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	if (mmn[1]==0) {
		_input2:
		view("frm/down/nonumbbs.log");
		lineinput(buff,160);check_han(buff);printf("\n");
		check_cmd(buff);
		if (mmn[1]==0) {
			view("frm/down/nonumbbs.err");
			if (yesno2("",Yes)==No) goto _input2;
			goto _fail;
		}
	}
	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));check_dir(newdir);

	//받을 파일명
	if (bbsdownnum<0) bbsdownnum+=1000;
	sprintf(filename,"%s.%d",pf.id,bbsdownnum%1000);
	sprintf(filename2,"%s/%s",newdir,filename);
	strcpy(g_buff1,filename);
	view("frm/down/filename.log");
	lineinput(buff,12);printf("\n");
	if (strlen(buff)>0) strcpy(filename,buff);
	del_space(filename);del_esc3(filename);
	sprintf(filename2,"%s/%s",newdir,filename);

	count=0;  //다운받을 화일수
	tsize=0;  //다운받는 용량
	count=0;
	if ((fp=fopen(filename2,"w+"))==NULL) goto _fail;
	if ((fp2=fopen(NOW_FILED,"r"))==NULL) {
		touch_file(NOW_FILED);
		if ((fp2=fopen(NOW_FILED,"r"))==NULL) goto _fail;
	}
	for (i=1;i<100;i++) {
		if (mmn[i]==0||!bbs_num(mmn[i])||bbs.del) continue;
		if (mode==2&&!stristr(bbs.filename,".jpg")) continue;
		//암호가 지정되어 있을때
        if (!bit(bcfg.mode,31)&&strlen(bbs.passwd)>0&&!check_pass(bbs.passwd,4,"이 자료는 암호가 지정되어 있습니다.")) continue;
		printf(".");fflush(stdout);
		fprintf(fp,"번  호 : %d\n",bbs.num);
		fprintf(fp,"게시자 : %-8s (%-8s)\n",bbs.id,bbs.name);
		fprintf(fp,"등록일 : %s\n",datestr(8,bbs.date));
		fprintf(fp,"제  목 : %s\n",bbs.title);
		fprintf(fp,"\n");
		fseek(fp2,bbs.posi,SEEK_SET);
		for (n=0;n<bbs.size&&!feof(fp2);n++) {
		   ch=fgetc(fp2);
		   if (ch!='\0') fputc(ch,fp);
		}
		fprintf(fp,"\n");
		bbs_read_num(); 			  //읽은 횟수증가
		add_read_numn(bbs.num); 	  //읽음 표시 추가
		count++;
	}
	printf("\n");
	fclose(fp2);
	fclose(fp);
	if (count<1) {
		printf(" 해당되는 자료가 없습니다\n");
		goto _fail;
	}
	tsize=file_size(filename2);

	//다운불가능 조건
	ret=No;
	if (guest_mode&&bit(cfg.downmode,6)==1)   pf.down=100000000;
		 if (is_char("gy",go.type))           ret=Yes;
	else if (bit(cfg.infomode,3))			  ret=Yes;
	else if (count==0)						  printf("\n### 해당되는 화일이 없습니다.");
	else if (club_mode&&pf.type>14) 		  ret=Yes;	//동호회의 경우 운영진 이상 무시
	else if (bcfg.fread>0&&pf.type>=bcfg.fread) ret=Yes;  //자유다운등급
	else if (bcfg.fread==99)				  ret=Yes;	//자유다운등급
	else if (check_type('i'))                 ret=No; //메뉴 가능 등급확인
	else if (check_type('r'))                 ret=No; //메뉴 가능 등급확인
	else ret=Yes;
	if (!ret) goto _fail;

	set_stat(8,bbs.title);	   //다운중임
	set_rec_(1,8,tsize,filename2);
	sz_file(filename2);
	bbsdownnum++;
	goto _end;

	_fail:
	view("frm/down/downfaib.log");
	bbs=oldbbs;
	ret=No;
	goto _end;

	_end:
	bbs=oldbbs;
	bbs_num(bbs.num);
	NOW_NO=tNOW_NO;NOW_REC=tNOW_REC;NOW_TOPREC=tNOW_TOPREC;NOW_ALLREC=tNOW_ALLREC;
	rm_tmpup();
	return (ret);
}




//자료실다운
//multi_down 이저정 되면 멀티다운 가능
pds_down(int mode)
{
	FILE *fp;
	char buff[200];
	char buff2[200];
	char TNOW_FILE[300];
	char TNOW_FILED[300];
	char newdir[100];
	char adnfile[102][151];
	char filename[200];
	char newfile[200];
	BBS oldbbs,bbs2;
	GO	oldgo,oldgo2,old_club;
	int old_clubmode,count;
	int old_level=pf.level;
	int old_down=pf.down;
	int i,n,i2,key,ret,tsize;
	int from_userroom=No;	//사용자영역일때
	int to_userroom=No;   //사용자영역으로 전송
	int mult_down=No;	//다중다운일때
	int rz_mode=0,fail=No;
	time_t old_t,now_t;
	int tNOW_NO,tNOW_REC,tNOW_ALLREC,tNOW_TOPREC;
	tNOW_NO=NOW_NO;tNOW_REC=NOW_REC;tNOW_TOPREC=NOW_TOPREC;tNOW_ALLREC=NOW_ALLREC;

    if (bit(limit_mode,3)) {
        if (!view("frm/pf/notdown.log")) mess("다운금지");
        return No;
    }
	oldgo=go;
	oldbbs=bbs;
	strcpy(TNOW_FILE ,NOW_FILE);
	strcpy(TNOW_FILED,NOW_FILED);

	if (!is_char("gy",go.type)&&ss_check(3))   goto _fail;   //다운 금지
	if (!is_char("gy",go.type)&&check_warn(3)) goto _fail;  //경고시 다운금지

    if (pf.type<18&&!club_mode&&view("frm/down/nodnmain.log")) goto _fail;
    if (pf.type<18&& club_mode&&view("frm/down/nodnclub.log")) goto _fail;

	ret=No;
	re_print=Yes;
	key=No; 													 //인터넷라인

	sprintf(buff,"%s/logo/%s.ndn",club_go.dir,go.code);
	if (is_file(buff)) {view(buff);goto _fail;}
    sprintf(buff,"%s/logo/all.ndn",club_go.dir);
	if (is_file(buff)) {view(buff);goto _fail;}

	view("%s/logo/%s.dow",club_go.dir,go.code);
	if (!view("%s/frm/down/down.log",club_go.dir)) view("frm/down/down.log");

	//사용자영역일때
	from_userroom=No;	 //사용자영역으로 전송
	if (is_same(go.code,"userroom")) from_userroom=Yes;
	//화면 출력모드일때 dn만 누름
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;

	if (mmn[1]==0) {
		_input2:
		view("frm/down/nonum.log");
		lineinput(buff,160);
		check_cmd(buff);
		if (mmn[1]==0) {
			view("frm/down/nonum.err");
			if (yesno("",Yes)==No) goto _input2;
			goto _fail;
		}
	}

	count=0;  //다운받을 화일수
	tsize=0;  //다운받는 용량
    to_userroom=No;   //사용자영역으로 전송
    mult_down=Yes;    //다중다운일때


	del_space(go.dir);if (strlen(go.dir)==0) strcpy(go.dir,".");
	del_space(club_go.dir);if (strlen(club_go.dir)==0) strcpy(club_go.dir,".");

	view("frm/down/title.log");
	for (i=0;i<100;i++) {
		abbs[i].num=abbs[i].filesize=-1;
		strcpy(abbs[i].filename,"");
		strcpy(adnfile[i],"");
	}
	count=0;
	for (i=0;i<100;i++) {
		if (mmn[i]==0||!bbs_num(mmn[i])||bbs.del) continue;
		if (mode==2&&!stristr(bbs.filename,".jpg")) continue;
		//암호가 지정되어 있을때
        if (!bit(bcfg.mode,31)&&strlen(bbs.passwd)>0&&
            !check_pass(bbs.passwd,4,"이 자료는 암호가 지정되어 있습니다.")) continue;
		strcpy(filename,"");
             if (is_same(go.code,"userroom")) sprintf(filename,"userroom/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
        else if (is_same(go.code,"dncart")) make_code_filename(filename,bbs.key,bbs.tcode,bbs.filename);
        else if (go.type=='z') make_code_filename(filename,bbs.key,bbs.tcode,bbs.filename);
		else if (go.type=='c') get_cdrom(filename,bbs.num);
		else sprintf(filename,"%s/%s",go.dir,bbs.filename);
		del_space(filename);del_esc3(filename);

		if (strlen(filename)<1) 	 printf("      %d.번 ->빈 파일\n",mmn[i]);
		else if (!is_file(filename)) printf("      %d.번 ->없는 파일(%s)\n",mmn[i],filename);
		else {
			abbs[count]=bbs;
			strcpy(adnfile[count],filename);
			sprintf(g_buff1,"%3d",count+1);
			sprintf(g_buff2,adnfile[count]);
			if (mode!=2)
			view("frm/down/downlist.log");
			tsize+=bbs.filesize;
			uall.down++;
			uall.downsize+=bbs.filesize;
			count++;
			if (pf.type<18&&(cfg.dn_mult_limit>0||bcfg.dn_mult_limit>0)) {
				if ( (cfg.dn_mult_limit>0&&count>cfg.dn_mult_limit)||
					 (bcfg.dn_mult_limit>0&&count>bcfg.dn_mult_limit) ) {
					view("frm/down/dn_mult.log");
					break;
				}
			}
		}
	}

	if (count<1) goto _fail;
	if (count>1) {
		sprintf(g_buff1,"%3d",count);
		sprintf(g_buff2,"%10d",tsize);
		if (mode!=2)
		view("frm/down/downsum.log");
	}
    if (read_pf4(pf.id)) pf.down=pf2.down;

	//다운불가능 조건
	if (guest_mode)   pf.down=100000000;
	ret=No;
		 if (is_char("gy",go.type))           ret=Yes;
	else if (bit(cfg.infomode,3))			  ret=Yes;
	else if (guest_mode&&!bit(cfg.downmode,6)) printf("\n### 손님은 다운받을수 없습니다.");
	else if (count==0)						  printf("\n### 해당되는 화일이 없습니다.");
	else if (tsize==0)						  printf("\n### 화일의 크기가 0입니다.");
	else if (club_mode&&pf.type>14) 		  ret=Yes;	//동호회의 경우 운영진 이상 무시
	else if (from_userroom) 				  ret=Yes;	//사용자영역에서는 무조건 ok
	else if (!club_mode&&pf.type>=15&&go.type=='u')  ret=Yes;  //무조건 다운
	else if (club_mode&&cpf.type>=15&&go.type=='u')  ret=Yes;  //무조건 다운
	else if (go.type=='u')                    printf("등록전용자료실에선 다운이 되지 않습니다.");  //자유다운모드에서는 무조건 다운
	else if (bcfg.fdown>0&&pf.type>=bcfg.fdown) ret=Yes;  //자유다운등급
	else if (bcfg.fdown==99)				  ret=Yes;	//자유다운등급
	else if (check_type('i'))                 ret=No; //메뉴 가능 등급확인
	else if (check_type('d'))                 ret=No; //메뉴 가능 등급확인
	else if (pf.type<cfg.downfree&&(pf.down<=0||pf.down<tsize)) printf("\n### %s님의 다운가능한 용량( %d KB) 을 초과합니다.",pf.id,pf.down/1000);
	else ret=Yes;

	if (!ret) goto _fail;

	if (c_proc) {
		printf("ID:%s %s T:%d  G:%s GT:%c\n",pf.id,pf.name,pf.type,go.code,go.type);
		printf("가능:%dB 필요:%dB freetype:%d Bfdown:%dn\n",pf.down,tsize,cfg.downfree,bcfg.fdown);
		pressenter();
	}

	//멀티다운가능 여부?
	//회원구분별 다운/업이 지정되어 있다.
	mult_down=Yes;	  //멀티다운가능하다.
	to_userroom=No;
		 if (count<2) ;
	else if (!club_mode&&bcfg.fdown>0&&pf.type>=bcfg.fdown) ;
	else if (pf.type>=14||now_type>=14) ;
	else if (cfg.downmult);
	else if (from_userroom);
	else if (go.type=='y');          //자유다운모드에서는 무조건 다운
	else if (go.type=='g');          //자유다운모드에서는 무조건 다운
	else if (pf.down>100000000);	 //100메가 이상일때 무조건 다운
	else mult_down=No;				 //멀티다운불가능
	if (guest_mode&&from_userroom) goto _fail;
	if (guest_mode&&to_userroom)   to_userroom=No;

	if (!ret) goto _fail;

	if (pf.type<18&&(cfg.dn_limit||bcfg.dn_limit)) {
		key=No;
		if (cfg.dn_limit>0) {
			i=set_tag_down(4);	//현재 전체업/다운수
            if (i>=cfg.dn_limit) {a_num[8]=i;a_num[9]=cfg.dn_limit;key=Yes;}
		}
		if (!key&&bcfg.dn_limit>0) {
			i=set_tag_down(6);	//현재 전체업/다운수
            if (i>=bcfg.dn_limit) {a_num[8]=i;a_num[9]=bcfg.dn_limit;key=Yes;}
		}
		if (key) {
			view("frm/editor/dn_limit.log");
			goto _fail;
		}
	}

	//자료실 프로토콜 선택
	i2=1;
	if (mode!=2) {
        strcpy(g_buff4,pf.type==19?"9.DNS  ":"");
		if (from_userroom) view("frm/down/selectu.log");
        else if (is_same(go.code,"dncart")) view("frm/down/selectc.log");
        else view("frm/down/selectp.log");
        lineinput(buff,4);check_han(buff);printf("\n");
		i2=atoi(buff);
	}
	if (i2==0&&strlen(buff)==1) goto _fail;
	if (guest_mode&&i2==3) i2=1;
    if (is_same(go.code,"dncart")&&i2==4) i2=1;
	if (from_userroom&&i2==3) i2=1;

	set_stat(8,bbs.title);	   //다운중임
	if (mode!=2)
	view("frm/down/wait.log");

	if (!bit(cfg.downmode,0)) {  //다운전 감소
		//다운수지정을 함
		set_bbs_down(count);	  //다운수지정
		bbs_down_down(tsize);
	}
	pf.todaymode=set_bit(pf.todaymode,5,1);   //오늘작업

	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	sys_tem("rm -rf %s",newdir);
	check_dir(newdir);
	if (i2==3) {	//사용자 영역에 링크로 복사
		//사용자 영역 방을 만듬
		sprintf(newdir,"userroom/%s/%s",c_han(pf.id),pf.id);
		check_dir(newdir);	   //방이 없으면 새로 만듬
		for (i=0;i<100;i++) {
			if (strlen(adnfile[i])>0&&is_file(adnfile[i])) {
				sprintf(newfile,"%s/%s",newdir,abbs[i].filename);
				sys_tem("ln -sf %s %s",adnfile[i],newfile);
				bbs=abbs[i];
                strcpy(bbs.passwd,"");
				bbs.del=No;
				bbs_add_userroom();  //게시판 추가(사용자영역에..)
			}
		}
		if (mode!=2)
		view("frm/down/downoku.log");
		goto _done;
	}
    else if (i2==4) {    //DNCART
		for (i=0;i<100;i++) {
            bbs=abbs[i];
            bbs2tmp();    //현재BBS의 내용을 TMP로 저장
            dncart();
		}
        view("frm/down/downca.log");
		goto _done;
	}
	else if (i2==9&&pf.type==19) {	  //지정디렉토리에 복사
		_cont4:
		view("frm/down/dns.log");
		dosinput(DEF_DNS,50);check_han(DEF_DNS);printf("\n");
		if (strlen(DEF_DNS)<2) goto _fail;
		if (!is_dir(DEF_DNS)||in_file2("system/cannot.dir",DEF_DNS)) {
			printf(" 디렉토리를 찾을수 없습니다.\n");
			goto _cont4;
		}
		for (i=0;i<100;i++) {
			if (strlen(adnfile[i])>0&&is_file(adnfile[i])) {
				sys_tem("cp -f %s %s",adnfile[i],DEF_DNS);
			}
		}
		view("frm/down/downok.log");
		goto _done;
	}
	else {		//일반다운일때
		count=0;
		for (i=0;i<100;i++) {
			if (strlen(adnfile[i])<1) continue;
			if (bit(cfg.downmode,15)) {
				n=sys_tem("ln -sf %s %s",adnfile[i],newdir);
				if (n!=0) {
					printf(" 링크 실패 %s %s\n",adnfile[i],newdir);
					goto _fail_ln;
				}
			}
			else {
				if (bit(cfg.downmode,21)&&check_free()<50) {view("frm/system/nospace.log");goto _fail;}
				_fail_ln:
                if (strlen(adnfile[i])>0&&strlen(newdir)>0)
                    sys_tem("cp -f %s %s",adnfile[i],newdir);
			}
			printf(".");fflush(stdout);
			count++;
		}
		sprintf(buff,"총%d건",count);
		set_rec_(1,8,tsize,buff);
		if (mode!=2)
		printf("[80D[2K");
		sprintf(buff,"tmp/up%d%s/*",cfg.hostno,no_spc(tty));
		if (i2==2) {
			view("frm/down/kermit.log");
			n=kermitsz(buff);
		}
		else {
			if (mode!=2)
			view("frm/down/zmodem.log");
			n=sz(buff);
		}
		//다운취소시
		if (n!=0&&bit(cfg.downmode,16)==0) {
			view("frm/down/down.err");
			goto _fail;
		}
		if (mode!=2)
		view("frm/down/downok.log");
		goto _done;
	}

	_fail:
	view("frm/down/downfaip.log");
	bbs=oldbbs;
	ret=No;
	goto _end;

	_done:
    if (bit(cfg.downmode,0)||is_same(go.code,"dncart")) {  //다운후 감소
		//다운수지정을 함
		set_bbs_down(count);	  //다운수지정
        if (!is_same(go.code,"dncart"))
            bbs_down_down(tsize);
	}
	ret=Yes;
	if (bit(cfg.downmode,12)==0) {
		if (view("frm/down/downstop.log")) {
			while (1) {
				printf("7");
				lineinput9(buff,20,6);
				if (stristr(buff,"^B0")||stristr(buff,"rz")||stristr(buff,"*B0")) {printf("7");continue;}
				break;
			}
			printf("\n");
		}
	}
	goto _end;

	_end:


	strcpy(NOW_FILE ,TNOW_FILE);
	strcpy(NOW_FILED,TNOW_FILED);
    if (is_same(go.code,"dncart")) {
        count=0;
        if ((fp=fopen(NOW_FILE,"r"))!=NULL) {
            while (fread(&bbs,sizeof_bbs,1,fp)) {
                if (bbs.down==0) {
                    count++;
                    break;
                }
            }
            fclose(fp);
            if (count==0) {
                unlink(NOW_FILE);
                unlink(NOW_FILED);
            }
        }
    }
	bbs=oldbbs;
	go=oldgo;
	bbs_num(bbs.num);
	NOW_NO=tNOW_NO;NOW_REC=tNOW_REC;NOW_TOPREC=tNOW_TOPREC;NOW_ALLREC=tNOW_ALLREC;
	set_stat(1,"");     //다운중 해제
	rm_tmpup();
	return (ret);
}


//현재 게시판의 내용을 현재 사용자 영역에 복사한다.
bbs_add_userroom()
{
	BBS oldbbs;
	FILE *fp1;
	FILE *fp2;
	int num;
	char c;
	int i;
	char oldNOW_FILE[100];
	char oldNOW_FILED[100];

	oldbbs=bbs;
	//현재의 bbs의 내용을 임시화일로 저장한다.
	strcpy(oldNOW_FILE,NOW_FILE);
	strcpy(oldNOW_FILED,NOW_FILED);

	unlink(TMP_FILE);
	//내용을 임시화일로 저장
	fp1=fopen(NOW_FILED,"r+");
	fp2=fopen(TMP_FILE,"w");
	fseek(fp1,bbs.posi,SEEK_SET);
	for (i=0;i<bbs.size;i++) {
		c=fgetc(fp1);
		fputc(c,fp2);
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);

    if (!read_pf4(pf.id)) return;

	//저장된 임시화일과 bbs를 사용자영역에 저장한다.
	sprintf(NOW_FILE ,"userdata/%s/%s/room.tit",c_han(pf2.id),pf2.id);
	sprintf(NOW_FILED,"userdata/%s/%s/room.txt",c_han(pf2.id),pf2.id);
	time(&bbs.date);  //날짜를 오늘날짜로 세팅
	bbs_add(1);
	strcpy(NOW_FILE ,oldNOW_FILE);
	strcpy(NOW_FILED,oldNOW_FILED);
	unlink(TMP_FILE);
}



set_bbs_down(int count)
{
	FILE *fp;
	GO	oldgo,oldgo2,old_club;
	int i,i2,n,key;
	BBS bbs2,oldbbs;

	char filename[200];
	int arec[110];

	for (i=0;i<100;i++) arec[i]=-1;

	oldgo=go;
	oldbbs=bbs;
	if (count<1) return;

	//먼저 해당번호 REC 찾는다.
	if (!is_typepds) return;
	if ((fp=fopen(NOW_FILE,"r"))==NULL) return;
	for (i=0;i<count;i++) {
		rewind(fp);
		i2=0;
		while (fread(&bbs,sizeof_bbs,1,fp)) {
			if (bbs.num==abbs[i].num) {
				arec[i]=i2;
				break;
			}
			i2++;
		}
	}
	fclose(fp);

	//찾은REC에 추가함
	c_lock(1,NOW_FILE);
	if ((fp=fopen(NOW_FILE,"r+"))==NULL) return;
	for (i=0;i<count;i++) {
		if (arec[i]<0) continue;
		fseek(fp,arec[i]*sizeof_bbs,SEEK_SET);
		if (fread(&bbs,sizeof_bbs,1,fp)) {
			bbs.down++; 		   //다운수증가
			time(&bbs.date2);	   //마지막날자수
			fseek(fp,arec[i]*sizeof_bbs,SEEK_SET);
			del_esc3(bbs.filename);del_esc3(bbs.id);del_esc3(bbs.name);
			if (!feof(fp )) fwrite(&bbs,sizeof_bbs,1,fp);
		}
	}
	fclose(fp);
	c_lock(0,NOW_FILE);


	//전체자료실에서 다운했을때 원래에 다운수 추가
	if (go.type=='z') { //전체자료실일때
		for (i=0;i<count;i++) {
			sprintf(filename,"%s/menu/%s.tit",club_go.dir,abbs[i].tcode);
			if ((fp=fopen(filename,"r+"))==NULL) continue;
			i2=0;
			while (fread(&bbs,sizeof_bbs,1,fp)) {
				if (is_same(bbs.title,abbs[i].title)&&
					is_same(bbs.filename,abbs[i].filename)&&
					bbs.date==abbs[i].date) {
					bbs.down++;
					time(&bbs.date2);
					fseek(fp,i2*sizeof_bbs,SEEK_SET);
					fwrite(&bbs,sizeof_bbs,1,fp);
					break;
				}
				i2++;
			}
			fclose(fp);
		}
	}
	bbs=oldbbs;
	go=oldgo;
}



//시디롬 자료실일때 *.cd에서 화일명을 읽어옴
get_cdrom(char *filename,int num)
{
	CD cd;
	FILE *fp;
	strcpy(filename,"");
	sprintf(filename,"%s/menu/%s.cd" ,club_go.dir,go.code);
	fp=fopen(filename,"r+");
	if (fp==NULL) return;
	while (fread(&cd,sizeof(cd),1,fp)) {   //읽기
		if (cd.num==num) {
			sprintf(filename,"%s%s",cd.dir,cd.filename);
			break;
		}
	}
	fclose(fp);
}


cpf_down_down(int w,int d)	  //해당동호회 다운수 증가
{
	FILE *fp;
	int i;
	char id_file[300];
	CPF cpf2;

	if (!club_mode) return;
	sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
	if ((fp=fopen(id_file,"r+"))!=NULL) {
		i=-1;
		while (fread(&cpf2,sizeof_cpf,1,fp)&&i++)
			if (is_same(cpf2.id,cpf.id)) {
				fseek(fp,i*sizeof_cpf,SEEK_SET);
				if (is_char(BBBTYPE)) cpf2.countw+=w;
				else				  cpf2.countu+=w;
				cpf2.countd+=d;
				fread(&cpf2,sizeof_cpf,1,fp);
				break;
			}
		fclose(fp);
	}
}



//게시물의 내용을 다운받는다.  0:읽기 1:다운
bbs_pr(int mode)
{
	char ch;
	char OLDDIR[500];
	char buff[200];
	char buff2[200];
	char buff3[50];
	char filename[200];
	char filename2[100];
	BBS oldbbs;
	BBS bbs2;
	BBS abbs[102];

	int tNOW_NO=NOW_NO;
	int tNOW_REC=NOW_REC;
	int tNOW_ALLREC=NOW_ALLREC;
	int i,i2,key,count,ret,tsize_bbs;
	int count_file,tsize_file;
	FILE *fp;
	FILE *fp1;
	FILE *fp2;
	struct tm *tp;
	time_t today;
	int mail_mode=No;
	int bbs_down=Yes;
	int file_down=No;

    oldbbs=bbs;
	getcwd(OLDDIR,sizeof(OLDDIR));		//현재의 디렉토리
	re_print=Yes;

    //사용자영역일때
	//화면 출력모드일때 dn만 누름
	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	if (pf.type<18&&bit(bcfg.form,1)) {mess("PR/DN금지 영역입니다.");return No;}
	if (go.type=='k') {message("비공개대화방입니다.");return No;}
    if (bit(limit_mode,3)) {
        if (!view("frm/pf/notdown.log")) mess("PR금지");
        return No;
    }
	if (mmn[1]==0) {
		printf("\n 게시판 화일 다운 방법 pr [번호]\n");
		printf("   예) pr 1,2,3 1-5   (연속으로 100개까지 다운이 가능합니다.)\n");
		printf("   예) pr 10 11 12 13-15 (10,11,12,13,14,15번 다운)\n");
		pressenter();
		return No;
	}
	if (mode==0) {
		printf("\n PRINTER/CAPTURE(갈무리)를 준비하시고  [ENTER] 를 누르십시오.\n");
        lineinput(buff,10);
        if (is_quit(buff)) return No;
	}

	if (is_same(go.code,"rmail")||is_same(go.code,"mbox")||is_same(go.code,"cmail")) mail_mode=Yes;
	printf("\n");
	count=0;
	count_file=0;
	tsize_file=0;
	tsize_bbs =0;
	for (i=1;i<101;i++) {
        if (mmn[i]<=0||!bbs_num(mmn[i])||bbs.del) continue;
        key=Yes;
        if (pf.type<18&&!bit(bcfg.mode,31)&&strlen(bbs.passwd)>0)    //암호가 지정되어 있을때
            key=check_pass(bbs.passwd,4,"이 자료는 암호가 지정되어 있습니다.");
        if (!key) continue;
        abbs[count]=bbs;
        tsize_bbs+=bbs.size;
        strcpy(abbs[count].filename,"");
        abbs[count].filesize=0;
        count++;
        if (mode==1&&mail_mode) {
            sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
            if (is_file(buff)) {
                i2=file_size(buff);
                if (i2>0) {
                    count_file++;
                    tsize_file+=i2;
                    strcpy(abbs[count].filename,bbs.filename);
                    abbs[count].filesize=i2;
                }
            }
        }
	}

	printfL2();
	sprintf(buff,"tmp/up%d%s",cfg.hostno,no_spc(tty));check_dir(buff);
	strcpy(buff3,"");
	for (i=0;i<count;i++) {
		if (abbs[i].filename[i]!=0) strcpy(buff2,byte2str(abbs[i].filesize));
		else						strcpy(buff2,byte2str(abbs[i].size));
        if (mode==1) {
            printf("%3d 등록:%-8s %-34.34s %-12.12s%s\n"
            ,abbs[i].num,abbs[i].id,abbs[i].title,abbs[i].filename,buff2);
        }
		if (abbs[i].filename[0]!=0) strcpy(buff3,abbs[i].filename);
	}

    sprintf(filename2,"%s.cap",now_str(11));
    sprintf(filename,"tmp/up%d%s",cfg.hostno,no_spc(tty));check_dir(filename);
    sprintf(filename,"tmp/up%d%s/%s",cfg.hostno,no_spc(tty),filename2);
    if (mode==1) {
        printf("\n");
        printf(" 다운로드하여 자신의 PC에 저장될 화일명을 입력하십시오.(예: i1.txt)\n");
        printf(" Enter키를 누르면 \"날짜시간.CAP\" 화일명이 자동으로 지정됩니다.\n");
        printf(" (Q:취소) >>");
        lineinput(buff,13);del_space(buff);del_esc(buff);
        if (is_same(buff,"q")) goto not_pr;
        if (buff[0]==0) printf(filename2);
        printf("\n");
        if (buff[0]!=0&&good_filename(buff)) strcpy(filename2,buff);

        sprintf(filename,"tmp/up%d%s/%s",cfg.hostno,no_spc(tty),filename2);
        printf(" ### 총 %3d개 게시물   총화일크기 %5d KB  화일명:%s\n",count,tsize_bbs/1000,filename2);
        if (count_file>0&&tsize_file>0&&mode==1&&mail_mode)
        printf(" ### 총 %3d개 화  일   총화일크기 %5d KB  화일명:%s\n",count_file,tsize_file/1000,buff3);

        ret=No;
        if (!is_char("gy",go.type)&&guest_mode) printf("손님은 사용할수 없습니다.");
        else if (count==0)                      printf("해당되는 화일이 없습니다.");
        else if (bbs_man(pf.id))                ret=Yes;
        else if (check_type('r'))               ret=No;
        else if (count_file>0&&check_type('d')) ret=No;
        else ret=Yes;

        if (ret==No) goto not_pr;

        bbs_down=Yes;
        file_down=No;
        if (count_file>0&&tsize_file>0&&mail_mode&&mode==1) {
            printf("\n 1.메일+첨부화일다운 2.메일 다운 3.화일만 다운  0.취소 >>");
            lineinput(buff,1);printf("\n");
            if      (buff[0]==0||buff[0]=='1')  {bbs_down=Yes;file_down=Yes;}
            else if (buff[0]=='2')              {bbs_down=Yes;file_down=No; }
            else if (buff[0]=='3')              {bbs_down=No ;file_down=Yes;}
            else goto not_pr;
        }
    }

    //게시판 갈무리
	if (bbs_down) {
		fp1=fopen(filename,"w+");
		if (fp1==NULL) {
			printf("%s 화일 OPEN 실패",filename);
			goto not_pr;
		}
		fp2=fopen(NOW_FILED,"r+");
		if (fp2==NULL) {
			printf("%s 화일 OPEN 실패",NOW_FILED);
			goto not_pr;
		}
		for (i=0;i<count;i++) {
			tp=localtime(&abbs[i].date);
			fprintf(fp1,"#%-7d",abbs[i].num);
			fprintf(fp1,"%-8s(%-8s)\n",abbs[i].id,abbs[i].name);
			fprintf(fp1,"%-60.60s %02d/%02d %02d:%02d%6d\n",
			abbs[i].title,tp->tm_mon+1,tp->tm_mday,tp->tm_mday,
			tp->tm_hour,tp->tm_min,abbs[i].line);
			fseek(fp2,abbs[i].posi,SEEK_SET);
			for(i2=0;i2<abbs[i].size;i2++) {
				ch = fgetc(fp2);
				fputc(ch,fp1);
			}
		}
		if (fp1!=NULL) fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
	}

	//읽은횟수 증가
	fp=fopen(NOW_FILE,"r+");
	for (i=0;i<count;i++) {
		if (bbs_num(abbs[i].num)) {
			if (fread(&bbs2,sizeof_bbs,1,fp)&&bbs2.num==abbs[i].num) {
				time(&bbs2.date2);
				bbs2.read++;
				if (mode!=0&&abbs[i].filename[0]!=0&&file_down&&is_typepds) bbs2.down++;
				fseek(fp,NOW_REC*sizeof_bbs,SEEK_SET);
				del_esc3(bbs2.filename);del_esc3(bbs2.id);del_esc3(bbs2.name);
				if (NOW_REC<1000000) {
					if (!feof(fp )) fwrite(&bbs2,sizeof_bbs,1,fp);
				}
				bbs=bbs2;
			}
		}
	}
	if (fp!=NULL) fclose(fp);

	if (mode==0) {
		view_text(filename);
	}
	else {	   //게시판 다운일때
		ret=Yes;
		set_stat(8,bbs2.title); 	//다운중임
		sprintf(buff,"PR %s",bbs2.title);
		set_rec_(1,8,0,buff);
		if (file_down&&mail_mode&&mode==1) {
			for (i=0;i<count;i++) {  //바이너리 전송
				if (abbs[i].filename[0]!=0) {
					sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,abbs[i].filename);
					if (is_file(buff)) sys_tem("ln -sf %s tmp/up%d%s",buff,cfg.hostno,no_spc(tty));
				}
			}
		}
		printf("\n ## zmodem 화일전송을 시작합니다. ## \n");
		sys_tem("%s tmp/up%d%s/*",cfg.sz,cfg.hostno,no_spc(tty));
	}

	set_stat(1,"");     //다운중 해제
	printf("\n");
	bbs=oldbbs;
	NOW_NO=tNOW_NO; 			//원래의 번호 복구
	NOW_REC=tNOW_REC;
	NOW_ALLREC=tNOW_ALLREC;
    if (mode==1) {
        printf("\n ## 게시판 다운이 끝났습니다. [Enter]\n");getchar();
    }
	return Yes;

	not_pr:
		bbs=oldbbs;
		NOW_NO=tNOW_NO; 			//원래의 번호 복구
		NOW_REC=tNOW_REC;
		NOW_ALLREC=tNOW_ALLREC;
		sprintf(buff,"rm tmp/up%d%s -rf",cfg.hostno,no_spc(tty));sys_tem(buff);
		printf("\n ## 게시판 다운이 취소되었습니다. [Enter]\n");getchar();
		return No;
}



//현재 게시판의 내용을 전송한다.
bdn()
{
	char buff[300];
	char bdn_file[300];
	sprintf(bdn_file,"%s/logo/%s.bdn",club_go.code,go.code);
	if (ss_check(3))  return;  //다운 금지
	if (pf.type>=18);
	else if (bit(bcfg.downmode,20));
	else if (is_char(go.wtimes,'#'));
	else if (in_file2("system/bdn_id.dat",pf.id));
	else if (in_file2(bdn_file,pf.id));
	else {
		mess(" BDN 기능을 사용할수 없습니다.");
		return;
	}
	sys_tem("bin/bdn '%s' '%s' '%s'",go.code,club_go.code,pf.id);
	re_print=Yes;
}


//다운용량증가 시킴  업/적기 시  mode 1:증가 -1:감소
bbs_down_up(int tsize,char *tid,int mode)
{
	int i,olddown;
	PF pf3,oldpf;
	char buff1[30],buff2[30],buff3[30];

	oldpf=pf;
	if (guest_mode) 		 return;
    if (!read_pf4(tid))      return;
	if (pf2.type<1) 		 return;
	if (bcfg.fdown>0&&pf2.type>=bcfg.fdown) return;   //자유다운가능자일때
	olddown=pf2.down;
	if (club_mode&&cfg.clublevel!=0&&(byte)bcfg.level>(byte)cfg.clublevel)
		bcfg.level=(byte)cfg.clublevel;
	if (mode==-1)  {	 //감쇄일때 (자료 삭제시)
        if ((word)bcfg.level_size!=0) pf2.level -=(word)(tsize/bcfg.level_size);
		else if (is_typepds&&(word)cfg.level_size>0) pf2.level -=(word)(tsize/cfg.level_size);
		else						 pf2.level -=bcfg.level;
		if (is_same(pf2.id,pf.id)) {
			UP_LEVEL-=bcfg.level;
		}
		pf2.down  -=(tsize*cfg.drate)/10;	//가산다운용량 조정

	}
	else {
		if ((word)bcfg.level_size>0) pf2.level +=(word)(tsize/bcfg.level_size);
		else if (is_typepds&&(word)cfg.level_size>0) pf2.level +=(word)(tsize/cfg.level_size);
		else						 pf2.level +=bcfg.level;
		if (cfg.urate >0) tsize=(tsize*cfg.urate )/10;	  //등록시 비율
		if (bcfg.uratet>0&&in_hour(bcfg.uhour1,bcfg.uhour2)) tsize=(tsize*bcfg.uratet)/10;	//심야등록시 비율
		else if (bcfg.urate>0) tsize=(tsize*bcfg.urate)/10; 								//일반등록시 비율
		pf2.down  +=tsize;
		if (is_same(pf2.id,pf.id)) UP_LEVEL+=bcfg.level;
	}
	if (pf2.down>500000000) pf2.down=500000000;
	if (pf2.down<0		  ) pf2.down=0;
	if (pf2.level<0 	  ) pf2.level=0;
	if (is_typepds) 		pf2.countu++;
	else					pf2.countw++;
    if (pf2.type>0) save_pf2(0);
	if (is_bbspds&&is_same(pf.id,pf2.id)) {
		pf.down=pf2.down;
		pf.level=pf2.level;
//		pf.countu=pf2.countu;
//		pf.countw=pf2.countw;
		if (bit(cfg.downmode,14)) {
			if (oldpf.down !=pf.down  ) printf(" *** ID:%s 총크기:%dK 다운량변경 %dK -> %dK \n",pf.id,tsize/1000,oldpf.down/1000,pf.down/1000);
			if (oldpf.level!=pf.level ) printf(" *** ID:%s 레벨변경 %d -> %d \n",pf.id,oldpf.level,pf.level);
		}
	}
}


//다운용량감소
bbs_down_down(int tsize)
{
	int fdown=No,olddown;
	PF oldpf;
	char buff1[30],buff2[30],buff3[30];
	oldpf=pf;
	if (guest_mode) 			return;
    if (!read_pf4(pf.id))       return;             //해당회원이 없을때
	if (!is_typepds) return;
	if (bbs_man(pf.id)) 		return;
	if (stristr(go.code,"mail")||stristr(go.code,"mbox")) return;

	olddown=pf2.down;
	if (is_char("yg",go.type)||pf2.type>=cfg.downfree)   fdown=Yes;
	if (cfg.drate >0) tsize=(tsize*cfg.drate )/10;	  //다운시 비율
	if (bcfg.dratet>0&&in_hour(bcfg.dhour1,bcfg.dhour2)) tsize=(tsize*bcfg.dratet)/10;	//심야다운시 비율
	else if (bcfg.drate>0) tsize=(tsize*bcfg.drate)/10;    //다운시 비율			  //일반다운시 비율
	if (pf2.type>0&&!fdown)  pf2.down  -=tsize ; //가산다운용량 조정
	if (pf2.down<0) 		 pf2.down=0;
	pf2.countd++;
    if (pf2.type>0) save_pf2(0);
	if (is_bbspds&&is_same(pf.id,pf2.id)) {
		pf.down=pf2.down;
//		pf.level=pf2.level;
//		pf.countu=pf2.countu;
//		pf.countw=pf2.countw;
		if (bit(cfg.downmode,14)) {
			if (oldpf.down !=pf.down  ) printf(" *** ID:%s 총크기:%dK 다운량변경 %dK -> %dK \n",pf.id,tsize/1000,oldpf.down/1000,pf.down/1000);
			if (oldpf.level!=pf.level ) printf(" *** ID:%s 레벨변경 %d -> %d \n",pf.id,oldpf.level,pf.level);
		}
	}
	cpf_down_down(0,1);   //해당동호회 다운수 증가
}


/*
 선택 화일 이름
	☞test.zip
 파일 전송 프로토콜을 선택하십시오.
 1.[7mZmodem[m  2.Kermit	0.취소
 번호선택(ENTER:Zmodem) >> !중지!

 파일 전송 프로그램을 실행시켜 주십시오 !!
잠시 기다리십시오.
*/
mail_down()
{
	BBS abbs[101],oldbbs=bbs;
	int i,i2,n,n2,count=0,tsize;
	int count_file,tsize_bbs,tsize_file;
	char buff[300];
	char buff2[300];
	char newdir[300];

	if (guest_mode) goto _fail;
	count_file=tsize_bbs=tsize_file=0;
	view("frm/down/title.log");
	for (i=0;i<100;i++) {
		abbs[i].num=abbs[i].filesize=-1;
		strcpy(abbs[i].filename,"");
	}

	if (list_mode&&mmn[1]==0) mmn[1]=bbs.num;
	for (i=1;i<101;i++) {
		if (mmn[i]==0||!bbs_num(mmn[i])||bbs.del) continue;
		//암호가 지정되어 있을때
        if (!bit(bcfg.mode,31)&&strlen(bbs.passwd)>0&&
            !check_pass(bbs.passwd,4,"이 자료는 암호가 지정되어 있습니다.")) continue;
		abbs[count++]=bbs;
	}
	count=0;
	tsize=0;
	for (i=0;i<100;i++) {
		if (abbs[i].num>=0) {
			sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,abbs[i].filename);
			if (strlen(abbs[i].filename)>0&&is_file(buff)) {
				printf("    ☞%-13s %8d 바이트 (파일)\n",abbs[i].filename,abbs[i].filesize);
				count++;
				tsize+=abbs[i].filesize;
			}
			else {
				sprintf(abbs[i].filename,"%s.%d",pf.id,i+1);
				abbs[i].filesize=-1;
				printf("    ☞%-13s %8d 바이트 (메일)\n",abbs[i].filename,abbs[i].size);
				count++;
				tsize+=abbs[i].size;
			}
		}
	}
	if (count<1) goto _fail;
	if (count>1) printf("    총 %d 개 화일  %10d 바이트\n",count,tsize);

	//프로토콜 선택
	view("frm/down/select.log");
	lineinput(buff,2);check_han(buff);printf("\n");
	i2=atoi(buff);
	if (i2==0&&strlen(buff)==1) goto _fail;

	view("frm/down/wait.log");
	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	check_dir(newdir);
	for (i=0;i<100;i++) {
		if (strlen(abbs[i].filename)>0&&abbs[i].num>=0) {
			sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,abbs[i].filename);
            if (is_file(buff)) {
				sys_tem("cp -f '%s' %s/",buff,newdir);
				printf(".");fflush(stdout);
			}
			else if (bbs_num(abbs[i].num)) {
				bbs2tmp(); //현재BBS의 내용을 TMP로 저장
				sprintf(buff,"%s/%s",newdir,abbs[i].filename);
				file_move(TMP_FILE,buff);
				if (is_file(buff)) {
					printf(".");fflush(stdout);
				}
			}
		}
	}
	sprintf(buff,"tmp/up%d%s/*",cfg.hostno,no_spc(tty));
	if (i2==2) {
		view("frm/down/kermit.log");
		n=kermitsz(buff);
	}
	else {
		view("frm/down/zmodem.log");
		n=sz(buff);
	}
	sys_tem("rm -rf %s",newdir);
	view("frm/down/downok.log");
	return Yes;

	_fail:
	view("frm/down/downfail.log");
	return No;

}




set_tag_down(int mode)
{
	FILE *fp;
    int i,n,count=0;
    char filename[100];

    for (n=0;n<20;n++) {
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename, "r"))==NULL) continue;
        while (fread(&moni,sizeof_moni,1,fp)) {
            if (moni.mode<1||strlen(moni.id)<3||strlen(moni.tty)<1) continue;
            if(moni.mode<1||strlen(moni.tty)<1||strlen(moni.id)<1) continue;
                 if (mode==3&&moni.mode==6) count++;   //업수
            else if (mode==4&&moni.mode==8) count++;   //다운수
            else if (mode==5&&moni.mode==6&&is_same(moni.club,club_go.code)&&is_same(moni.gocode,go.code)) count++;   //업수
            else if (mode==6&&moni.mode==8&&is_same(moni.club,club_go.code)&&is_same(moni.gocode,go.code)) count++;   //다운수
            else break;
        }
        fclose(fp);
    }
	return count;
}


//예비다운로드
dncart()
{
    char filename1[300];
    char filename2[300];
    char old_NOW_FILE[300],old_NOW_FILED[300];
    if (strlen(bbs.filename)<1) return;
    strcpy(old_NOW_FILE ,NOW_FILE );
    strcpy(old_NOW_FILED,NOW_FILED);
    sprintf(filename1,"tmp/tmpa%d%s",cfg.hostno,no_spc(tty));   //호스트번호별임시화일
    sprintf(filename2,"tmp/tmpb%d%s",cfg.hostno,no_spc(tty));   //호스트번호별임시화일
    strcpy(NOW_FILE ,filename1);
    strcpy(NOW_FILED,filename2);
    strcpy(bbs.tcode,go.code);
	sprintf(bbs.key,club_go.code);
    bbs.date2=0;
    bbs.down=0;
    bbs_add(1);
    strcpy(NOW_FILE ,old_NOW_FILE );
    strcpy(NOW_FILED,old_NOW_FILED);
}


make_code_filename(char *filename,char *club,char *code,char *bbs_filename)
{
	FILE *fp;
	int found=No;
    char menufile[300];
    GO go,club_go;

    strcpy(filename,"");//없음
    if (strlen(bbs_filename)<1) return No;
    strcpy(club_go.dir,"./");
    if (!is_same(club,"main")) {//동호회가아닐경우
        found=No;
        fp=fopen(MENU_FILE,"r");
        while(fread(&go,sizeof_go,1,fp)) {
            if (go.type=='s'&&is_same(go.code,club)) {
                found=Yes;
                club_go=go;
                break;
            }
        }
        fclose(fp);
        if (!found) return No;
    }
    found=No;
    sprintf(menufile,"%s/%s",club_go.dir,MENU_FILE);
    if ((fp=fopen(menufile,"r"))==NULL) return No;
    while(fread(&go,sizeof_go,1,fp)) {
        if (is_char(PDSTYPE,go.type)&&is_same(go.code,code)) {
            found=Yes;
            break;
        }
    }
    fclose(fp);
    if (!found) return No;
    if (strlen(go.dir)<1) sprintf(go.dir,"%s/pds/%s",club_go.dir,go.code);
    sprintf(filename,"%s/%s",go.dir,bbs_filename);
    return Yes;
}
