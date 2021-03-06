////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
//	  게시판 관련 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//게시판 제목을 화면에 표시한다.
run_bbsscreen()
{
	FILE *fp,*fp1,*fp2;
	int ii,i,n,j,i2,i3,n2,n3,linenum;
    char buff[255],buff1[200],buffkey[50],buff2[50];
    char char3,c;
    char buffsize[30],buffdate[30],buffdate2[30],bufftitle[70],abuff[6][90];
    char top_file[200],frm_file[200],inv1[150],inv2[150];
    char is_bbs_top=No,is_bbs_frm=No,is_bbsvtx,is_bbsjpg,is_bbsansi;
	char ch[2];
	char bbs_mode=Yes;	 //게시판
	char bbs_ad=No;
    BBS  t_bbs;
    int re_logo_head,re_numok=No,mail_mode=No,count=0;
    int found;
	int maxline;
	view_pos=0;
	bbsreadall=Yes; 	 //볼내용없다.
    _last2=2;;;;;;;;;;;;;;;;;;;;;;;;

    sync();
	for (i=0;i<50;i++) list_anum[i]=0;	 //화면출력 번호

	//해당화일정보읽기
	bbs_mode=is_typebbs;
	if ((is_same(go.code,"rmail")||
		 is_same(go.code,"mbox")||
		 is_same(go.code,"cmail"))) {
        mail_mode=Yes;
		if (dd_mode) set_ddmode(2); 	 //삭제된것 안보이게
		if (guest_mode) {
			view("frm/error/noguest.err");
			go_prev();
			return;
		}
	}
    if (mail_mode&&pf.type<18&&view("frm/down/noinmail.log")) {
        go_prev();
        return;
    }
	g_char=' ';
	if (check_type('i')) return;       //메뉴 가능 등급확인
	if (no_exec_port(go.code)) {
		go_top(0);
		return; //사용가능 메뉴인가?
	}
	list_mode=No;
	bbs_ad=(bit(bcfg.mode,16)||bit(bcfg.mode,17));
	is_bbsvtx =bit(bcfg.mode,1);
	is_bbsansi=bit(bcfg.mode,2);
	is_bbsjpg =bit(bcfg.mode,3);

	if (dd_mode) {
		if (pf.type<18&&bit(bcfg.mode,21)) set_ddmode(2);
		else if (bbs_man(pf.id)) ;
		else if (club_mode&&cpf.type>16) ;
		else if (!club_mode&&pf.type>15) ;
		else set_ddmode(2);
	}

    _cont0:
	if (re_bbs) {
		check_bbsrec(0);					  //게시판 기록 관련
		set_filename(); 						//현재표시할 화일명
		if (bit(bcfg.automode,15)) auto_bbs2(); //자동불러오기 모드
		read_bcfg();
        if (!mail_mode) re_bbsnum(1);           //BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
		bbs_ltmode=No;			//lt등으로 검사중 모드
		if (go.type=='d') {     //자동검색디렉토리 구조일때
			sprintf(buff,"bin/dir2bbs %s %s/menu/%s 0",go.dir,club_go.dir,go.code);
			sys_tem(buff);
		}
		check_bbs();		  //자료의 갯수를 파악하는 루틴
		re_bbs=No;
        if (!mail_mode) {
            if (is_char("z",go.type)) {
                     if (bit(cfg.zmode,21)) check_zbtotal(1*60*60,1);
                else if (bit(cfg.zmode,22)) check_zbtotal(6*60*60,1);
            }
            if (is_char("b",go.type)) {
                     if (bit(cfg.zmode,21)) check_zbtotal(1*60*60,2);
                else if (bit(cfg.zmode,22)) check_zbtotal(6*60*60,2);
            }
        }
	}

	if (bbs_ltmode)   fp1=fopen(LT_FILE,"r+");
	else			  fp1=fopen(NOW_FILE,"r+");
	maxline=bcfg.maxline; if (maxline<1) maxline=15;
    is_bbs_top=set_bbs_logo("top",top_file);
    is_bbs_frm=set_bbs_logo("frm",frm_file);
	re_logo_head=Yes;
	if (is_bbsvtx) printf(VTXOFF);
	//상단자료를 빈것을 파악한다.
	if (NOW_TOPREC<0) NOW_TOPREC=0;

	ii=NOW_TOPREC;
	NOW_TOPNO=0;
	linenum=0;
    t_bbs=bbs;
    t_bbs.num=0;
	if (fp1!=NULL) {
    	sync();fflush(NULL);
        while (count++<10000) {
			if (bit(bcfg.modelist,7)) break;	 //목록 안보임
			if (ii<1||linenum>=maxline) break;
			fseek(fp1,(ii-1)*sizeof_bbs,SEEK_SET);
			if (feof(fp1)||!fread(&bbs,sizeof_bbs,1,fp1)) break;
            if (!re_numok&&stristr(go.code,"mail")) {
                if (t_bbs.num>0&&bbs.num>0&&bbs.num<t_bbs.num) {
                    fclose(fp1);
                    sys_tem("bin/bsysop '_renum_bbs' '%s' '%s'",NOW_FILE,NOW_FILED);
                    re_numok=Yes;
                    re_bbs=Yes;
                    goto _cont0;
                }
                t_bbs=bbs;
            }
			if (bbs.del&&!dd_mode) {ii-- ;continue;}	//운영진이 아닐때 삭제된것 안보임
            if (bit(bcfg.modelist,22)&&bbs.gd<=' ') {ii-- ;continue;}    //추천된것만 보임
			if (bbs.filesize==0&&!bit(cfg.bbsmode,5)&&!bit(bcfg.modelist,19)&&!dd_mode&&is_pds) {ii-- ;continue;}	 //운영진이 아닐때 삭제된것 안보임
			if (bbs.num>0&&NOW_TOPNO==0) NOW_TOPNO=bbs.num;
            if (!bbs.del&&bcfg.outday>0&&bbs.date>0&&time_day(bbs.date)>bcfg.outday) {
                check_outday();
                re_bbs=Yes;
                goto _cont0;
            }
			ii--;
			linenum++;
        }
		if (NOW_TOPNO==0) {
			NOW_TOPREC=0;	//제일끝에서부터 시작
			bbs_back();
			if (NOW_TOPREC>0) {
				fseek(fp1,(NOW_TOPREC-1)*sizeof_bbs,SEEK_SET);
				if (fread(&bbs,sizeof_bbs,1,fp1)) NOW_TOPNO=bbs.num;
			}
		}
	}

	ii=NOW_TOPREC;
	linenum=0;
    if (!mail_mode&&brec.allnum==0&&NOW_TOPNO>1) re_bbsnum(3);
	//BREC 0:읽기 1:자동  2:오늘  3:전체갱신  8:1개추가 9:1개삭제
    	sync();fflush(NULL);
	while (1) {
		//게시판의 상단 화면 출력
		if (re_logo_head) {
			strcpy(g_buff1,"");
			if (bbs_ltmode) 	   strcat(g_buff1,"[검색중]");
			if (go.type=='q')      strcat(g_buff1,"답변(AN)");
			if (is_char(go.wtimes,'#')) strcat(g_buff1,"<BDN>");
			cls();
			if (is_bbs_top) view(top_file);
			re_logo_head=No;
			if (bcfg.list_y==0&&bcfg.list_x==0) {bcfg.list_y=6;bcfg.list_x=1;}
            printf("[u");
			xy(1,bcfg.list_y);
			if (bcfg.list_x>1) printf("[%dC",bcfg.list_x-1);
			linenum=0;
		}
		if (bit(bcfg.modelist,7)) break;	 //목록 안보임
		if (ii<1||linenum>=maxline) break;
		if (go.type=='6'&&linenum>5) break;
		if (fp1==NULL) break;
		fseek(fp1,(ii-1)*sizeof_bbs,SEEK_SET);
		if (feof(fp1)||!fread(&bbs,sizeof_bbs,1,fp1)) break;
		//삭제되었으며,삭제된것은 보이지 않고,삭제가능레벨이 아닐때
		if (bbs.del&&!dd_mode) {ii-- ;continue;}	//운영진이 아닐때 삭제된것 안보임
        if (bit(bcfg.modelist,22)&&bbs.gd<=' ') {ii-- ;continue;}    //추천된것만 보임
		if (bbs.filesize==0&&!bit(cfg.bbsmode,5)&&!bit(bcfg.modelist,19)&&!dd_mode&&is_pds) {ii-- ;continue;}	 //운영진이 아닐때 삭제된것 안보임
		NOW_TOPNO=bbs.num;
		bbs.title[60]=0;all_trim(bbs.title);
		del_esc(bbs.key);all_trim(bbs.key);
		del_space(bbs.id);
		bbs.title[60]=0;del_tab(bbs.title);all_trim(bbs.title);
		del_space(bbs.filename);del_tab(bbs.filename);del_esc(bbs.filename);bbs.filename[12]=0;

		jpg_tag=is_jpgfile();
        if (bit(bcfg.modelist,8)&&bbs.gd=='*') bbs.gd=' ';
		set_buffnum(buffnum,bbs.num,jpg_tag);	   //buffnum만들기
			 if (bit(bcfg.modelist,11)) sprintf(buffsize,"%dB",bbs.filesize);
		else if (bit(bcfg.modelist,12)) sprintf(buffsize,"%dK",bbs.filesize/1000);
		else if (bit(bcfg.modelist,13)) sprintf(buffsize,"%dM",bbs.filesize/1000000);
		else if (bit(bcfg.modelist,14)) sprintf(buffsize,"");
		else							strcpy(buffsize,byte2str(bbs.filesize));
		strcpy(buffdate ,datestr(17,bbs.date ));
		strcpy(buffdate2,datestr(17,bbs.date2));
		char3=gd_chr(bbs_mode); 	//추천/조회/다운지정
		strcpy(inv1,"");strcpy(inv2,"");
		if (is_same(go.code,"notice")&&bbs.gd=='*') char3=' ';

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
		strcpy(buffkey,bbs.key);

		if (linenum>=0&&linenum<50) list_anum[linenum]=bbs.num;   //화면출력 번호
		//리스트모양보이기..
		if (bcfg.list_x>1) printf("[%dC",bcfg.list_x-1);
		if (is_bbs_frm) view(frm_file);
		else if (is_same(go.code,"cmail")) { //보낸 메일의 경우
            strcpy(buff2,"*안읽음");
			if (bbs.date2>0) sprintf(buff2,"[%s]",datestr(17,bbs.date2));
			printf("%s%-8.8s%c%-4.4s %s %-7s%3d %s%-41.41s%s\n",
                    buffnum,bbs.id,char3,bbs.key,buffdate,buff2,bbs.line/16+1,inv1,bbs.title,inv2);
		}
        else if (mail_mode) {     //메일의 경우
			if (bbs.read<=0) buffnum[0]='>';
			del_esc(bbs.id);
			strcpy(buffdate,datestr(0,bbs.date));
			if (bbs.filesize==0||strlen(bbs.filename)<1) {
				strcpy(buffsize,byte2str(bbs.size));
			}
			if (bbs.filesize>0&&strlen(bbs.filename)>0) strcpy(buffkey,"BIN ");
			else strcpy(buffkey,"TXT ");
			if (bcfg.list_x>1) printf("[%dC",bcfg.list_x-1);
			printf("%s%-8s %-8s %s %4.4s%5s %-34.34s\n",
					buffnum,bbs.id,bbs.name,buffdate,buffkey,buffsize,bbs.title);
		}
		else if (is_same(go.code,"userroom")) {     //사용자영역
			printf("%s%s %-12s%5s %s%-40.40s%s\n",
			buffnum,buffdate,bbs.filename,buffsize,inv1,bbs.title,inv2);
		}
		else if (is_char(go.wtimes,'/')) {
			printf("%s%-8s%4d %s%-60.60s%s\n",
			buffnum,bbs.id,bbs.line,inv1,bbs.title,inv2);
		}
		else if (is_char(go.wtimes,'+')) {
			printf("%s%s%-72.72s%s\n",buffnum,inv1,bbs.title,inv2);
		}
		else if (is_char(go.wtimes,'=')) {
			printf("%s%s%-58.58s %s%5d%s\n",
            buffnum,inv1,bbs.title,datestr(0,bbs.date),is_typepds?bbs.down:bbs.read,inv2);
		}
		else if (is_char(go.wtimes,'*')) {
			printf("%s%s%-60.60s %s%5d%s\n",
            buffnum,inv1,bbs.title,datestr(5,bbs.date),is_typepds?bbs.down:bbs.read,inv2);
		}
		else if (is_char(go.wtimes,'-')) {
			printf("%s%s%-60.60s %-8s%s\n",
			buffnum,inv1,bbs.title,bbs.key,inv2);
		}
        else if (is_same(go.code,"bbs_all")) {
            strupr(bbs.tcode);
/*
  번호 ID       등록일      동호회 분류 PG 제  목
  1590 MSWIAGB  12-31 12:31 main   UTIL 12 [유틸] NETINFO 3.30 CR
                                      123456789012345678901234567890123456789012
*/
//          printf("%s%-8s %s %-7.7s%-5.5s%2d %-36.36s\n",
            printf("%s%-8s %s %-8.8s %-8.8s %-33.33s\n",
            buffnum,bbs.id,datestr(3,bbs.date),bbs.key,bbs.tcode,bbs.title);
		}
        else if (is_same(go.code,"pds_all")) {
/*
  번호 ID       파일명        크기 동호회 분류   제  목
  1591 MSWIAGB  boremove.zip  156K        UTIL  [유틸] 백오리스트 방어프로그램
  1590 MSWIAGB  net330cr.zip    8K        UTIL  [유틸] NETINFO 3.30 CR
                                                123456789012345678901234567890123456789012
  1590 MSWIAGB  net330cr.zip    8K        UTIL  [유틸] NETINFO 3.30 CR
*/
            strupr(bbs.tcode);
//          printf("%s%-8s %-12s %6s %-7.7s%-5.5s %-30.30s\n",
            printf("%s%-8s %-12s %6s %-8.8s %-8.8s %-25 25s\n",
            buffnum,bbs.id,bbs.filename,buffsize,bbs.key,bbs.tcode,bbs.title);
		}
        else if (go.type=='b') {
            strupr(bbs.tcode);
/*
  번호 ID       등록일     조회 분류   제  목
  1590 MSWIAGB  12-31 12:31   5 UTIL  [유틸] NETINFO 3.30 CR
                                      123456789012345678901234567890123456789012
*/
            printf("%s%-8s %s%4d %-5.5s %s%-40.40s%s\n",
            buffnum,bbs.id,datestr(3,bbs.date),bbs.read,bbs.tcode,inv1,bbs.title,inv2);
		}
        else if (go.type=='z') {
/*
  번호 ID       파일명        크기 전송 분류   제  목
  1591 MSWIAGB  boremove.zip  156K    5 UTIL  [유틸] 백오리스트 방어프로그램
  1590 MSWIAGB  net330cr.zip    8K    4 UTIL  [유틸] NETINFO 3.30 CR
                                              123456789012345678901234567890123456789012
  1590 MSWIAGB  net330cr.zip    8K    4 UTIL  [유틸] NETINFO 3.30 CR
*/
            strupr(bbs.tcode);
            printf("%s%-8s %-12s %6s%5d %-5.5s %s%-32.32s%s\n",
            buffnum,bbs.id,bbs.filename,buffsize,bbs.down,bbs.tcode,inv1,bbs.title,inv2);
		}
		else if (is_typesecu||(is_typecons&&!is_same(bcfg.man1,pf.id))) {	  //비공개 경우
            printf("%s%s%5d%3d %s%-58.58s%s\n",buffnum,buffdate,bbs.read,bbs.line/16+1,inv1,bbs.title,inv2);
		}
		else if (is_typecons&&is_same(bcfg.man1,pf.id)) {	  //비공개담당일경우
			printf("%s(%-8s %-8s)%c%s%6d%4d %s%-36.36s%s\n",
            buffnum,bbs.name,bbs.id,char3,buffdate,bbs.read,bbs.line/16+1,inv1,bbs.title,inv2);
		}
		else if (is_typeans) {	   //답변 게시판의 경우
			printf("%s%-8s%c%s%5d%3d%3d %s%-46.46s%s\n",
            buffnum,bbs.id,char3,buffdate,bbs.read,calc_ok(3),bbs.line/16+1,inv1,bbs.title,inv2);
		}
		else if (is_typewtx) {
			printf("%s%-8s%c%s%5d%6s %s%-46.46s%s\n",
			buffnum,bbs.id,char3,buffdate,bbs.read,buffsize,inv1,bbs.title,inv2);
		}
		else if (is_typevtx||is_typeansi) { 	//VTX게시판의 경우
			strcpy(buffsize,byte2str(bbs.size));
			printf("%s%-8s%c%s%5d%5s %s%-41.41s%s\n",
			buffnum,bbs.id,char3,buffdate,bbs.read,buffsize,inv1,bbs.title,inv2);
		}
		else if (is_char("agnil-8",go.type)) {
			if (!club_mode)
			printf("%s%-8s %-8s%c%s%6d%4d %s%-38.38s%s\n",
            buffnum,bbs.name,bbs.id,char3,buffdate,bbs.read,bbs.line/16+1,inv1,bbs.title,inv2);
			else
			printf("%s%-8s %-8s%c%s%5d%3d %s%-40.40s%s\n",
            buffnum,bbs.name,bbs.id,char3,buffdate,bbs.read,bbs.line/16+1,inv1,bbs.title,inv2);
		}
		else if (is_char("dc",go.type)) {
			if (bbs.filename[0]==0) {ii--;continue;};
			printf("%s %-12s %5s %s%-50.50s%s\n",
					buffnum,bbs.filename,buffsize,inv1,bbs.title,inv2);
		}
        else if (is_same(go.code,"dncart")) {
            char3=' ';
            if (bbs.down>0) char3='D';
            printf("%c%3d %-8s %-12s%6s %-38.38s\n",
                    char3,bbs.num,bbs.tcode,bbs.filename,buffsize,bbs.title);
		}
		else if (is_typepds) {
			strcpy(buff1,bbs.filename);
			if (bit(cfg.downmode,11)) {
				sprintf(buff,"%s/%s",go.dir,bbs.filename);
                if (is_same(bbs.filename,"화일없음")||!is_file2(buff)) {
					strcpy(buff1,"");
				}
			}
//	5644 petabyte 815etc.zip	6.2M	 3 한글815 특별판 기능개선판(영문윈)
//123456712345678 123456789012123456123456 12345678901234567890123456789012345678
			printf("%s%-8s%c%-12s%6s%6d %s%-38.38s%s\n",
					buffnum,bbs.id,char3,buff1,buffsize,bbs.down,inv1,bbs.title,inv2);
		}
		ii--;
		linenum++;
		NOW_BOTREC=ii;
		if (ii<0) break;
	}
	if (fp1!=NULL) fclose(fp1);
    	sync();fflush(NULL);
    set_bbs_logo("bl1",buff);
    view(buff);
}


check_outday()
{
    FILE *fp;
    int i;
    if (bcfg.outday>0) return;
    if ((fp=fopen(NOW_FILE,"r+"))==NULL) return;;  //제목화일
    i=0;
    printf("\n ### 잠시만 기다려 주십시요 자료를 정리하고 있습니다.. ###\n");
    while (1) {
        if(!fread(&bbs,sizeof_bbs,1,fp)) break;
        if (bbs.date>0&&time_day(bbs.date)<=bcfg.outday) break;
        if (!bbs.del&&bbs.date>0) {
            bbs.del=Yes;
            fseek(fp,i*sizeof_bbs,SEEK_SET);
            fwrite(&bbs,sizeof_bbs,1,fp);
            fseek(fp,(i+1)*sizeof_bbs,SEEK_SET);
		}
        i++;
	}
    fclose(fp);
    sync();
    printf("\n ### 정리가 끝났습니다.###\n");
}

bbs_vv()	//내용중 가기
{
    FILE *fp;
	char buff1[305];
	char buff2[305];
	char ch;
	int i,i2,n,n2,pos,mode;
	int found=No;

	bzero(buff1,300);
	bzero(buff2,300);
    fp=fopen(NOW_FILED,"r+");  //내용이 없을때
    if (fp==NULL) return;
    fseek(fp,bbs.posi,SEEK_SET);
	i2=0;mode=0;
	for(pos = 0; bbs.size>0&&pos<bbs.size&&pos<100000&&i2<200; pos++) {
        ch=fgetc(fp);
		if (ch=='<'&&mode==0)      mode=1;
		else if (ch=='<'&&mode==1) mode=2;
		else if (ch=='>'&&mode==2) mode=3;
		else if (ch=='>'&&mode==3) {mode=4;break;}
		else if (mode==2) buff2[i2++]=ch;
		else {mode=0;i2=0;}
		if (mode==3&&!stristr(buff2,"(v)")) {mode=0;i2=0;}
	}
    fclose(fp);
	if (mode<4||i2<1) return;	   //실패일때
	buff2[i2]=0;
	if (buff2[0]==0||!stristr(buff2,"(v)")) return;
	for (i=0;i<300;i++) {
		if (buff2[i+0]=='('&&buff2[i+1]=='v'&&buff2[i+2]==')') {
			strcpy(buff1,&buff2[i+3]);
			strchng(buff2,buff1,">>","");
			del_esc(buff2);del_enter(buff2);
			strcpy(auto_cmd,buff2);
			found=Yes;
			break;
		}
	}
	if (!found) message("바로가기 정보가 없습니다.");
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


/*
//게시판 복사 운영자,게시판담당만 가능
//cp hello 1 2 3 4 5....
//mode2= 0:메인<->메인,동호회<->동호회	(cp/mv)
//		 1:메인->동호회 (cpclub/mvclub)  2:동호회->메인 (cpmain/mvmain)
nnnnn()
{
    FILE *fp1,*fp2,*fp3,*fp4,*fp5;
	char buff[180];
    char ch,filename[200],filename1[200],filename2[200],filename3[200];
    int size1,size2,i,n,n2,count=0,found;
    time_t date1,date2;
    BBS lastbbs,bbs2;

                   //YYYYMMDDHHMMSS
    date1=str2date4("20000105010000");
    unlink(TMP_FILE1);unlink(TMP_FILE2);
    touch_file(TMP_FILE1);touch_file(TMP_FILE2);
    fp1=fopen(NOW_FILE,"r");       //제목화일
    fp3=fopen(TMP_FILE1,"w+");     //제목
    fp4=fopen(TMP_FILE2,"w+");     //내용
    lastbbs.posi=lastbbs.size=0;
    if (club_mode) {
        sprintf(filename1,"/share/old/forum/%s/menu/%s.txt",club_go.code,go.code);
        sprintf(filename2,"forum/%s/menu/%s.txt",club_go.code,go.code);
        sprintf(filename3,"/share/old/forum/%s/menu/%s.tit",club_go.code,go.code);
    }
    else {
        sprintf(filename1,"/share/old/menu/%s.txt",go.code);
        sprintf(filename2,"menu/%s.txt",go.code);
        sprintf(filename3,"/share/old/menu/%s.tit",go.code);
    }
    if (!is_file(filename1)) {
        mess("%s 파일없음",filename1);
        strcpy(filename1,filename2);
    }
    printf(" F1:%s  SIZE:%d\n",filename1,file_size(filename1));
    printf(" F2:%s  SIZE:%d\n",filename2,file_size(filename2));
    if (!yesno("계속 할까요?(Y/n)>>",Yes)) return;
    size1=file_size(filename1);
    size2=file_size(filename2);
    while (fread(&bbs,sizeof_bbs,1,fp1)) {
        bbs2=bbs;
        strcpy(filename,filename2);
        found=No;
        if (size1>0&&bbs.date<date1) {
            strcpy(filename,filename1);
            if ((fp5=fopen(filename3,"r"))!=NULL) {     //내용
                while (fread(&bbs2,sizeof_bbs,1,fp5)) {
                    if(bbs2.date==bbs.date&&bbs2.line==bbs.line&&
                       is_same(bbs2.id,bbs.id)&&
                       is_same(bbs2.name,bbs.name)) {
                        found=Yes;
                        break;
                    }
                }
            }
            fclose(fp5);
        }
        fp2=fopen(filename,"r");      //제목화일
        if (found) {
            bbs.posi=bbs2.posi;
            bbs.size=bbs2.size;
        }
        fseek(fp2,bbs.posi,SEEK_SET);
        fseekend(fp4);
        bbs.posi=ftell(fp4)-1;
        if (bbs.posi<1) bbs.posi=0;
        fseek(fp4,bbs.posi,SEEK_SET);
        n2=0;
        for (i=0;!feof(fp2)&&i<bbs.size;i++) {
            ch=fgetc(fp2);
            fputc(ch,fp4);
            n2++;
//          printf("%c",ch);
        }
        bbs.size=n2;
        fclose(fp2);
        fwrite(&bbs,sizeof_bbs,1,fp3);
//      printf("\n N=%d ID:%s POS=%d SIZE=%d %s \n",bbs.num,bbs.id,bbs.posi,bbs.size,bbs.title);
//      if (count++%100==0) pressenter();
        lastbbs=bbs;
	}
    fclose(fp1);
    fclose(fp3);
    fclose(fp4);
    file_move(TMP_FILE1,NOW_FILE );
    file_move(TMP_FILE2,NOW_FILED);
}
*/





view_bbs1(char *tcode)
{
    int ii,last,old_club_mode=club_mode;
    char OLD_NOW_FILE[200],OLD_NOW_FILED[200];
    GO old_go=go,old_club_go=club_go;
    strcpy(OLD_NOW_FILE,NOW_FILE);
    strcpy(OLD_NOW_FILED,NOW_FILED);
    sprintf(NOW_FILE ,"menu/%s.tit",tcode);
    sprintf(NOW_FILED,"menu/%s.txt",tcode);
    if (is_file(NOW_FILE)) {
        last=count_rec(NOW_FILE,sizeof_bbs);
        club_mode=No;
        strcpy(go.code,tcode);
        strcpy(club_go.code,"main");
        strcpy(club_go.dir,"./");
        go.type='l';
        for (ii=0;ii<100;ii++) {
            if (bbs_num(rand()%last+1)&&!bbs.del) {
                bbs_read(0);                   //해당번호 내용출력
                break;
            }
        }
    }
    club_mode=old_club_mode;
    club_go=old_club_go;
    go=old_go;
    strcpy(NOW_FILE,OLD_NOW_FILE);
    strcpy(NOW_FILED,OLD_NOW_FILED);
}
