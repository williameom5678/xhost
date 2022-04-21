////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: 리눅스용 BBS HOST							  //
//			  //			  개발자 : 김성대  xhost@xhost.co.kr (016-320-7882)		  //	  //
//			  이 소스를 변경하여 상용으로 사용하실때에는			  //
//			  개발자의 승인을 받아야 합니다.						  //
////////////////////////////////////////////////////////////////////////
//	  메일관련 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//운영진에게 보내기
do_sysmail(int mode)
{
	int i;
	int ret=No;
	char buff[100];
	if (guest_mode) return;
	strcpy(buff,go.dir);
	del_space(buff);del_esc(buff);buff[8]=0;
	if (strlen(buff)>2&&read_pf4(buff)&&pf2.type>0) {
		printf("\n %s(%s)님 에게 메일을 보내겠습니까? ",pf2.id,pf2.name);
		if (yesno("(Y/n)>>",Yes)) {
			printf("\n ***  [ %s(%s)님 수신메일 ]  ***\n",pf2.id,pf2.name);
			for(i=0;i<10;i++) strcpy(mmc[i],"");
			strcpy(mmc[1],pf2.id);
			wmail(1,pf2.id);
		}
	}
}


wmail(int wmode,char *tidstr)
{
	FILE *fp1,*fp2;
	char abook[1000][10]; //주소록에서 뽑아낸 아이디
	PF apf[50]; 	 //주소록에서의 아이디 저장 (실제 발송가능아이디)
	PF oldpf;
	int i,i2,n,nn=0;
	char ch;
	int mailnum=0;
	int key,ret,found=0;
	char mailid[1000];
	char mailid2[500];
	char tid[50],filename[500],filename2[500];
	char buff[1500],buff1[1500],stemp[9];	   //고유번호
	char file_send=No;
	GO oldgo=go;
	GO oldclub_go=club_go;
	int oldclub_mode=club_mode;
	BBS oldbbs=bbs;

	_last2=6;;;;;;;;;;;;;;;;;;;;;;;;
	strcpy(tid,tidstr);
	re_print=Yes;			   //화면출력
	re_logo=Yes;			   //보조화면출력
	re_bbs=Yes; 			   //게시판로딩
	list_mode=No;
	key=Yes;
	ret=Yes;
	unlink(TMP_FILE2);			//임시화일 삭제
//	  printf("%d %s %s %s",bbs.num,bbs.id,tid,bbs.title);pressenter();
	if (wmode==2&&strlen(tid)>0) {
		if (bit(cfg.mailmode,10)) {wmode=1;goto _cont1;}
		bbs2tmp();	  //현재BBS의 내용을 TMP로 저장
		if ((fp1=fopen(TMP_FILE,"r"))==NULL) goto _cont1;
		if ((fp2=fopen(TMP_FILE2,"w"))==NULL) goto _cont1;
		fprintf(fp2,"> 보낸이:%s\n",bbs.id);
		fprintf(fp2,"> 받는이:%s\n",pf.id);
		fprintf(fp2,"> 날  짜:%s\n",datestr(33,bbs.date));
		fprintf(fp2,"> 제  목:%s\n",bbs.title);
		while(fgets(buff,1000,fp1)) {
			fprintf(fp2,"> %s",buff);
		}
		fprintf(fp2,"\n");
		fclose(fp1);
		fclose(fp2);
	}
	_cont1:
	unlink(TMP_FILE);		   //임시화일 삭제

	if (!is_same(go.code,"wmail")) {
		go_top(0);
		go_code("wmail");
	}
	strcpy(mailid2,"");
	if (pf.type<18&&view("frm/down/noinmail.log")) goto _fail;

	if (wmode==98) {
		form_mail();
		if (!is_file(TMP_FILE)) goto _fail;
	}

	set_stat(9,"");
	if (strlen(tid)<1) strcpy(tid,mmc[1]);
	strcpy(mailid,"");if (strlen(tid)>0) strcpy(mailid,tid);
	if (is_same(mailid,"wmail")||is_same(mailid,"rmail")) strcpy(mailid,mmc[2]);
	if (is_same(mailid,"wmail")||is_same(mailid,"rmail")) strcpy(mailid,"");
	strchng2(mailid,"＠","@");
	strchng2(mailid,"＃","#");
	if (mailid[0]=='#'&&!guest_mode) goto _cont3;
	if (stristr(go.wtimes,"e")) view("frm/mail/wemail.log");
	else						view("frm/mail/wmail.log");

	_err1:
	view("frm/mail/mailid1.log");
	if (strlen(mailid)<3) lineinput(mailid,301);
	else				  printf("%s",mailid);
	printf("\n");
	check_han(mailid);strscheck(mailid);all_trim(mailid);
	if (is_same(mailid,"p")||is_same(mailid,"pp")||
		is_same(mailid,"x")||is_same(mailid,"xx")) goto _fail;
	if (strlen(mailid)<2) goto _fail;
	strchng2(mailid,";"," ");
	strchng2(mailid,","," ");
	sscanf(mailid,"%s",buff);
	xstrncpy(g_id,buff,50);
	if (!stristr(buff,"#")) {
		if (!read_pf4(buff)) {
			view("frm/mail/mailid1.err");
			strcpy(mailid,"");
			goto _err1;
		}
		printf("        %s(%s)\n",pf2.id,pf2.name);
	}

	view("frm/mail/mailid2.log");         //참조
	lineinput(mailid2,490);printf("\n");check_han(buff);strscheck(buff);
	if (is_quit(mailid2)) goto _fail;
	if (strlen(mailid2)<3) strcpy(mailid2,"");

	_cont3:
	for (i=0;i<1000;i++) bzero(abook[i],10);
	mailnum=0;
	if (mailid[0]=='#'&&!guest_mode) {
		strcpy(buff,&mailid[1]);
		n=atoi(buff); if(n<1) n=1;
		check_dir("userdata/%s/%s",c_han(pf.id),pf.id);  //개인주소록화일 오픈
		sprintf(buff,"userdata/%s/%s/book%d.dat",c_han(pf.id),pf.id,n);  //개인주소록화일 오픈
		if ((fp1=fopen(buff,"r"))==NULL) {
			mess("%d 번 주소록이 없습니다. 주소록관리(MAILBOOK)",n);
			goto _fail;
		}
		//주소록에서 같은주소록을 읽어내 amailid에 저장한다.
		while (mailnum<1000&&fgets(buff,300,fp1)) {
			check_cmd(buff);
			for (i=0;i<10;i++) {
				if (strlen(mmc[i])>2&&!is_esc(mmc[i])) {
					xstrncpy(abook[mailnum++],mmc[i],8);
					printf(" %3d. %-8.8s",mailnum,mmc[i]);
					if (mailnum%4==0) printf("\n");
				}
			}
		}
		fclose(fp1);
		printf("\n");
		if (mailnum<1) mess("%d 주소록에 입력된 아이디가 없습니다.",n);
	}
	else {			 //주소록이 아닐때 1명으로 지정한다.
		strcat(mailid," ");
		strcat(mailid,mailid2);
		strchng2(mailid,";"," ");
		strchng2(mailid,","," ");
		strchng2(mailid,"    "," ");
		strchng2(mailid,"   "," ");
		strchng2(mailid,"  "," ");
		strchng2(mailid,"  "," ");
		i2=0;
		check_cmd(mailid);
		for (i=0;i2<10;i2++) {
			if (strlen(mmc[i])>2)
				strcpy(abook[mailnum++],mmc[i]);
		}
	}
	if (mailnum<1) {
		mess("선택된 ID가 없습니다.");
		goto _fail;
	}
	if (pf.type<cfg.mailwrite) {view("frm/mail/notmail.log"); goto _fail;}

	for (n=0;n<mailnum;n++) {
		strcpy(mailid,abook[n]);		//첫번째 메일받을 아이디
		if (strlen(mailid)<3) {strcpy(abook[n],"");continue;}
		if (stristr(mailid,"@")) continue;  //E메일경우 확인안함
		if (strlen(mailid)<3||stristr(mailid,"@")) continue;  //E메일경우 확인안함
		abook[n][8]=0;
		mailid[8]=0;
		nn=0;
		while (nn++<30) {				//만일 30번을 다른아이디로변경했으면 끝
			key=0;
			found=read_pf4(mailid); 	//아이디 찾기
            del_space(pf2.tmail);del_esc3(pf2.tmail);
			if (is_same(pf2.id,mailid)) 			 {	   //아이디를 찾는다
                if      (pf2.tmail[0]==0)           { key= 0;found=Yes;break; }   //정상수신
                else if (is_same(mailid,pf2.tmail)) { key= 0;found=Yes;break; }   //정상수신
                else if (pf2.tmail[0]=='*')         { key=-1;found=Yes;break; }   //수신거부
				else								 { key= 1;found=Yes;	   }   //다른사람
			}
			if (key==0||key==-1||!found) break;
			if (key==1) {
                printf("\n [%s]님의 편지는 [%s]님이 받습니다.\n",mailid,pf2.tmail);
                strcpy(mailid,pf2.tmail);    //다른아이디로 변경됨
				strcpy(abook[n],mailid);		//1명일때 아이디의 내용을 저장한다.
			}
		}
		if (found==No||key==-1||pf2.type<1) {
			if (pf2.type<2)   printf("%s님은 삭제된 ID입니다.\n",mailid);
			else if (key==-1) printf("%s님은 메일 수신을 거부하고 있습니다.\n",mailid);
			else			  printf("%s라는 아이디가 없습니다.\n",mailid);
			//현재의 메일주소록의 위치를 삭제한다.
			strcpy(abook[n],"");
		}
	}

	mailnum=0;
	for (n=0;n<100;n++) {
		found=No;
		for (i=n-1;n>0&&i>=0;i--) {
			if (is_same(abook[n],abook[i])) {
				found=Yes;
				break;
			}
		}
		if (found||strlen(abook[n])<3) {
			for (i=n;i<99;i++) {
				strcpy(abook[i],abook[i+1]);
				strcpy(abook[99],"");
			}
		}
		else mailnum++;
	}

	if (read_pf4(abook[0])) view("userdata/%s/%s/pfinfo.mai",c_han(pf2.id),pf2.id);

	view("frm/mail/write.log");

	n=5;
	strcpy(bbs.title,"");
	strcpy(bbs.key,"");
	strcpy(bbs.filename,"");
	bbs.filesize=0;
	while (--n) {
		view("frm/mail/writet.log");
		dosinput(bbs.title,60);printf("\n");
		all_trim(bbs.title);
		if (bbs.title[0]==0) continue;
		if (is_same(bbs.title,"q")||is_same(bbs.title,"p")||
			is_same(bbs.title,"x")||is_same(bbs.title,"xx")) strcpy(bbs.title,"");
		break;
	}
	if (bbs.title[0]==0) goto _fail;   //취소시
	if (wmode==98) goto _cont4;

	unlink(TMP_FILE);
	if (wmode!=2) unlink(TMP_FILE2);

	_input1:
	strcpy(g_buff4,pf.type>=18?"99.UPS":"");

	view("frm/mail/select.log");
	lineinput(buff,2);check_han(buff);printf("\n");
	if (is_same(buff,"q")||is_same(buff,"n")||is_same(buff,"0")) goto _fail;
	n=atoi(buff);
	if (n==2)  {
		if (wmode==2) view(TMP_FILE2);
		ret=bbs_editor(2);
		if (!is_file(TMP_FILE)) ret=No;
		if (wmode==2) {file_add(TMP_FILE2,TMP_FILE);file_move(TMP_FILE2,TMP_FILE);}
	}
	else if (wmode==98) ;
	else if (n==3||n==4||n==99) {	   //파일전송시
		i=0;
		if (pf.type<18&&n==99) n=3;    //UPSUP;
		if (n==99) i=99;
		else {
			view("frm/mail/selectb.log");
			lineinput(buff,2);check_han(buff);printf("\n");
			if (is_same(buff,"q")||is_same(buff,"n")||is_same(buff,"0")) goto _fail;
			i=atoi(buff);
		}

		if (i==2) strcpy(bbs.key,"vtx");  //vtx
		else if (i==3); 				  //완성형TEXT
		else i=1;						  //바이너리 전송
		if (i==1&&pf.type<cfg.mailbintype) {
			view("frm/mail/notbin.log"); //바이너리 불가능등급
			goto _fail;
		}
			 if (n== 4) ret=up_file( 2,filename,No);	 //kermit
		else if (n==99) ret=up_file(99,filename,No);	 //kermit
		else			ret=up_file( 1,filename,No);	 //zmodem
		if (!ret) goto _fail;

		if (i==1&&is_file(filename)) {	   //파일전송의경우
			file_send=Yes;
			file_copy("frm/mail/filesend.dat",TMP_FILE);        //바이너리파일의 내용
			bbs.filesize=file_size(filename);
			str_filedir(filename,buff1,buff);  //buff를 디렉토리와 파일명으로 분해
			del_space(buff);
			xstrncpy(bbs.filename,buff,12);
		}
		else if ((i==2||i==3)&&is_file(filename)) {  //내용전송의경우
			file_copy(filename,TMP_FILE);
		}
		else ret=No;
	}
	else {		//문서편집
		set_stat(9,"");
		if (wmode==2) {
			file_move(TMP_FILE2,TMP_FILE);
			ret=bbs_editor(11); 	//TMP삭제안함
		}
		else ret=bbs_editor(1);
		if (!ret||!is_file(TMP_FILE)) goto _input1;   //취소시
	}

	_cont4:
	if (!ret||!is_file(TMP_FILE)) goto _fail;	//취소시

	if (wmode==98) strcpy(bbs.key,"카드");
	if (strlen(bbs.key)<1) {
		view("frm/mail/selecty.log");
		lineinput(buff,2);check_han(buff);printf("\n");
		if (is_same(buff,"q")||is_same(buff,"n")||is_same(buff,"0")) goto _fail;
		i=atoi(buff);
			 if (i==2) strcpy(bbs.key,"답요");
		else if (i==3) strcpy(bbs.key,"지연");
		else if (i==4) strcpy(bbs.key,"답지");
		else strcpy(bbs.key,"");
	}

	//발송확인
	printf("\n");
	printf(" 제목 : %s\n",bbs.title);
	printf(" 수신 : %s(%s)",pf2.id,pf2.name);
	if (mailnum>1) printf("포함 총%d명",mailnum);
	printf("\n");
	printf(" 종류 : ");
		 if (is_same(bbs.key,"답요")) printf("답장요망");
	else if (is_same(bbs.key,"지연")) printf("지연편지");
	else if (is_same(bbs.key,"답지")) printf("답장+지연");
	else if (is_same(bbs.key,"vtx"))  printf("VTX편지");
	else if (is_same(bbs.key,"카드")) printf("카드편지");
	else							  printf("일반편지");
		printf("\n");
	if (file_send)
	printf(" 화일명 : %s  크기 : %d 바이트\n",bbs.filename,bbs.filesize);
	view("frm/mail/selecto.log");
	set_stat(9,"");
	lineinput(buff,2);check_han(buff);printf("\n");
	if (is_same(buff,"q")||is_same(buff,"n")||is_same(buff,"0")) goto _fail;
	i=atoi(buff);

	strcpy(bbs.id,pf.id);		//발신인
	strcpy(bbs.name,pf.nick);	//발신자
	strcpy(bbs.tcode,"");       //편지분류
	bbs.gd=' ';                 //화일크기
	time(&bbs.date);			//발신 날짜 저장
	strcpy(bbs.passwd,"");
	bbs.date2=0;				//읽은 날짜 저장 (수신여부)
	bbs.read=0;
	bbs.down=0;
	bbs.del=0;
	bbs.ok=0;

	view("frm/mail/send1.log");
	//메일을 발송한다.
	for (n=0;n<mailnum;n++) {
		//게시물저장 시작
		//아이디를 찾는다. ->pf2에 저장
		if (strlen(abook[n])<3) continue;
		if (!read_pf4(abook[n])) continue;	//없으면 다음 아이디
		if (stristr(abook[n],"@")) {
			wmail_email(abook[n],filename);
			continue;
		}

		//편지수신함에 상대방편지함에 저장
		check_dir("userdata/%s/%s",c_han(pf2.id),pf2.id);  //개인주소록화일 오픈
		sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf2.id),pf2.id);
		sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf2.id),pf2.id);
		touch_file(NOW_FILE);touch_file(NOW_FILED);

		if (file_send&&read_pf4(abook[n])&&strlen(bbs.filename)>0) {
			check_dir("userroom/%s/%s",c_han(pf.id),pf.id);
			check_dir("userroom/%s/%s",c_han(pf2.id),pf2.id);
			//받는사람 파일
			for (i=0;i<200;i++) {
				sprintf(filename2,"userroom/%s/%s/%s",c_han(pf2.id),pf2.id,bbs.filename);
				if (!is_file(filename2)) break;
				incfilename(filename2);
				str_filedir(filename2,buff1,buff);	//buff를 디렉토리와 파일명으로 분해
				del_space(buff);
				printf("\n### %s 님의 디렉토리에 이미 %s파일이 존재합니다.\n",pf2.id,bbs.filename);
				printf("\n### 파일명을 %s -> %s로 변경합니다.",bbs.filename,buff);
				pressenter();
				xstrncpy(bbs.filename,buff,12);
			}
			file_copy(filename,filename2);
			sprintf(buff,"userroom/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
			if (!is_same(pf.id,pf2.id)) {
				unlink(buff);
				sys_tem("ln -sf %s %s",filename2,buff);
			}
			//실제화일 전송 여부 확인
			bbs.filesize=file_size(filename2);
			if (!is_file(filename2)) {
				printf("### 첨부되는 화일(%s)은 전송이 되지 않았습니다.\n",bbs.filename);
				strcpy(bbs.filename,"");
				strcpy(bbs.key,"실패");
			}
		}
		//게시물저장 시작
		set_rec(1,9);
		bbs.date2=0;

		bbs_add(1); 	   //메일을 저장한다. 내용복구
		strcpy(g_id,pf2.id);
		strcpy(g_name,pf2.name);
		strcpy(g_buff1,"");
		if (mailnum>0) sprintf(g_buff1,"<%d통>",mailnum);
		view("frm/mail/send2.log");
		sprintf(buff,"\n ** %s 님으로부터 메일이 도착했습니다.**\n",pf.id);
		if (bit(cfg.mailmode,5)) strcat(buff,"\007");
		to_mess(pf2.id,buff);
		re_answer(pf2.id,pf.id);		//자동 응답메일
	}
	//손님일때는 저장안함
	if (pf.type<1||guest_mode) goto _cont2;
	strcpy(bbs.id,pf2.id);		//발신인
	strcpy(bbs.name,pf2.nick);	//발신자
	//보낸편지 저장
	if (mailnum>1) {
		sprintf(buff,"<%d통>",mailnum);
		strcat(buff,bbs.title);
		xstrncpy(bbs.title,buff,60);
		strcpy(bbs.name,"<다수>");
	}
	check_dir("userdata/%s/%s",c_han(pf.id),pf.id);  //개인주소록화일 오픈
	sprintf(NOW_FILE ,"userdata/%s/%s/send.tit",c_han(pf.id),pf.id);
	sprintf(NOW_FILED,"userdata/%s/%s/send.txt",c_han(pf.id),pf.id);
	touch_file(NOW_FILE);touch_file(NOW_FILED);
	bbs.gd=0;
	bbs.del=0;
	bbs.ok=0;
	bbs.date2=0;
	bbs_add(1);   //메일을 저장한다. 내용 복구

	_cont2:
	unlink(TMP_FILE);		   //임시화일 삭제
	check_cmd("");
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
	view("frm/mail/mailend.log");
	goto _recover;

	_fail:
	unlink(TMP_FILE);		   //임시화일 삭제
	check_cmd("");
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
	view("frm/mail/failmail.log");

	_recover:
	unlink(TMP_FILE2);			//임시화일 삭제
	go=oldgo;
	club_go=oldclub_go;
	club_mode=oldclub_mode;
	bbs=oldbbs;
	re_print=Yes;
}



//지정된 아이디번호에 무조건 메일을 보냄
wmail4(char *id, char *filename,char *title,int mode)
{
	FILE *fp1;
	int found;
	char buff[100];
	PF pf2;
	if (guest_mode) return;
	if		(mode==8) strcpy(bbs.key,"전체");
	else if (mode==9) strcpy(bbs.key,"전체메일");
	else			  strcpy(bbs.key,"일반");
	strcpy(bbs.title,title);
	check_dir("userdata/%s/%s",c_han(id),id);
	sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(id),id);
	sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(id),id);
	strcpy(buff,TMP_FILE);
	strcpy(TMP_FILE,filename);
	bbs.gd=0;
	bbs.del=0;
	bbs.ok=0;
	strcpy(bbs.passwd,"");
	bbs.read=0;
	bbs.date2=0;
	if (mode==9) bbs_add(9);  //내용은 추가하지 않는다.
	else		 bbs_add(1);  //임시화일을 삭제하지 않는다.
	strcpy(TMP_FILE,buff);
}



// 메일 읽은 여부 처리
mail_readok()
{
	FILE *fp1,*fp2,*fp3,*fp4;
	int i,found,new_num,new_posi;
	BBS oldbbs;

	_cont:
	view("frm/mail/rmailok.log");
	// 편지(1) 처리(1.삭제 2.저장 3.보관 9.수신거부등록) 번호 >> !중지!
	i=numinput();printf("\n");
	if (i==0) {
		if(bit(cfg.mailmode,7)) i=1;
		else if(bit(cfg.mailmode,8)) i=2;
		else if(bit(cfg.mailmode,9)) i=3;
		else i=3;
	}
	if (i==9) {
		mess("아직 지원되지 않습니다.");
		goto _cont;
	}
	if (i==3) return;
	if (i==1||i==2) {
		if (i==2) {
			bbs2tmp();	  //현재BBS의 내용을 TMP로 저장
			strcpy(go.code,"mbox");
			set_filename();   //현재화일명 설정
			time(&bbs.date2);
			bbs_add(1);   //메일을 저장한다. 내용복구
			strcpy(go.code,"rmail");
			set_filename();
			unlink(TMP_FILE);
		}
		bbs_set_(1,bbs.num,0);		 //삭제

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
				fseek(fp3,(new_num-1)*sizeof_bbs,SEEK_SET);
				fwrite(&bbs,sizeof_bbs,1,fp3);
				new_posi+=bbs.size;
				new_num++;
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
		else {
			unlink(TMP_FILE);
			unlink(TMP_FILE2);
		}
		re_bbs=Yes;
	}
}



mail_set_read()
{
	BBS bbs2;
	FILE *fp1;
	int i=0;
	int num=bbs.num;
	char filename[120];


	//상대방 메일에 읽음 표시 지정
	if (bbs.read==0||(word)bbs.date2<1000) {  //처음 읽을때
		time(&bbs.date2);  //읽은 날짜 지정
		if (read_pf4(bbs.id)) { 		//메일수신자 찾기 ->pf2로 저장
			check_dir("userdata/%s/%s",c_han(pf2.id),pf2.id);  //개인주소록화일 오픈
			sprintf(filename,"userdata/%s/%s/send.tit",c_han(pf2.id),pf2.id);
			if ((fp1=fopen(filename,"r+"))==NULL) return;
			i=0;
			while (fread(&bbs2,sizeof_bbs,1,fp1)) {
				if (bbs.date==bbs2.date&&is_same(pf.id,bbs2.id)) {
					fseek(fp1,i*sizeof_bbs,SEEK_SET);
					time(&bbs2.date2);		//읽은 날짜 지정
					fwrite(&bbs2,sizeof_bbs,1,fp1);
					break;
				}
				i++;
			}
			fclose(fp1);
		}
	}
}


// 123.  -> 124.
// 123.4  -> 124.4
// 123.45  -> 124.45
// 123.456	-> 124.456

//파일명중 끝자리를 변경한다.
incfilename2(char *filename,int pos)
{
	int i;
	if (pos<0||pos>=(int)strlen(filename)) return;
	i=(filename[pos]+1);
	if (i>(int)'z'||i<(int)'0') i=(int)'0';
	filename[pos]=(char)(i);
}

incfilename(char *filename)
{
	int len=strlen(filename);
	if (len<1) return;
	if (filename[len-1]=='.') incfilename2(filename,len-2);
	else if (filename[len-2]=='.') incfilename2(filename,len-3);
	else if (filename[len-3]=='.') incfilename2(filename,len-4);
	else if (filename[len-4]=='.') incfilename2(filename,len-5);
	else incfilename2(filename,len-1);
}



//메인 전체 메일 보내기
wmall()
{
	FILE *fp;
	int i,key,key2,n=0,nn,n1,n2;
	char buff[300];
	char buff2[140];
	char id[9];
	char torank1,torank2;
	char tojob1,tojob2;
	int tolevel1,tolevel2,tosex,num,num_all,num_send;
	char tosexstr[10],dir[300];

    if (guest_mode) return;
	if (club_mode) {wmall_club();return;}
	key=Yes;
    if (in_file2("system/wmall.id",pf.id)) ;
    else if (pf.type<18&&pf.type<cfg.wmalltype) {
        mess("전체 메일을 보낼수 없는 등급입니다.");
        go_top(0);
        return;
    }

	torank1=0;torank2=14;tojob1=0;tojob2=19;tolevel1=0;tolevel2=999999;tosex=0;
	num_all=count_pf();

	strcpy(bbs.id,pf.id);
	strcpy(bbs.name,pf.name);
	strcpy(bbs.filename,"");
	strcpy(bbs.key,"전체메일");
	bbs.size=0;bbs.filesize=0;
	bcfg.wmode=0;
	strcpy(go.code,"wmall");strcpy(bbs.title,"");
	unlink(TMP_FILE);
	strcpy(NOW_FILE ,"menu/_allmail.tit");strcpy(NOW_FILED,"menu/_allmail.txt");
	touch_file(NOW_FILE);touch_file(NOW_FILED);
	while (1) {
		if (torank1<1)	torank1=0;if (torank1>19) torank1=19;
		if (torank2<1)	torank2=0;if (torank2>19) torank2=9;
		if (tojob1<1)  tojob1=0;if (tojob1>19)	tojob1=19;
		if (tojob2<1)  tojob2=0;if (tojob2>19)	tojob2=19;
		if (tolevel1<1) tolevel1=0;if (tolevel1>999999) tolevel1=999999;
		if (tolevel2<1) tolevel2=0;if (tolevel2>999999) tolevel2=999999;
		if		(tosex==1) strcpy(tosexstr,"남성회원");
		else if (tosex==2) strcpy(tosexstr,"여성회원");
		else			   strcpy(tosexstr,"구분없음");
		bbs.size=file_size(TMP_FILE);

		cls();
		printf(" *** %s 회원 전체 메일 보내기 (총:%d명)***\n",cfg.hostname,num_all);
		printfL2();
		printf("  1. 회 원 등 급 : %d - %d \n",torank1,torank2);
		printf("  2. 회 원 성 별 : %s      \n",tosexstr);
		printf("  3. 회 원 레 벨 : %d - %d \n",tolevel1,tolevel2);
		printf("  4. 직 업 분 류 : %d - %d \n",tojob1,tojob2);
		printf(" 10. 메 일 제 목 : %s\n",bbs.title);
		printf(" 11. 메 일 내 용 : %d Byte\n",bbs.size);
		if (pf.type>17) {
		printf(" 12. 보 내 는 ID : %s\n",bbs.id);
		printf(" 13. 보내는 이름 : %s\n",bbs.name);
		}
		printf("\n");
		printf(" 99. 위의 조건 회원에게 메일 보내기\n");
		printfL2();
		printf("  선택  종료(Q) >>");
		n=numinput();
		printf("\n");
		if (n<=0) break;
		if (n==1) {
			printf(" 대상회원 등급 하한  >>");torank1=get_num(torank1,2);printf("\n");
			printf(" 대상회원 등급 상한  >>");torank2=get_num(torank2,2);printf("\n");
		}
		if (n==2) {
			printf(" 대상회원 성별  구분없음(0) 남성(1) 여성(2) >>");tosex=get_num(tosex,1);printf("\n");
		}
		if (n==3) {
			printf(" 대상회원 레벨 하한  >>");tolevel1=get_num(tolevel1,6);printf("\n");
			printf(" 대상회원 레벨 상한  >>");tolevel2=get_num(tolevel2,6);printf("\n");
		}
		if (n==4) {
			view("frm/guest/job.scr");
			printf("\n 대상회원 직업번호 하한  >>");tojob1=get_num(tojob1,2);printf("\n");
			printf(" 대상회원 직업번호 상한  >>");tojob2=get_num(tojob2,2);printf("\n");
		}
		if (n==10) {
			view("frm/editor/writetit.wma");
			get_str(bbs.title,60);
		}
		if (n==11) {  //내용
			input_tmp(0);
		}
		if (n==12&&(pf.type>17)) {
			printf(" 메일 발송 ID   >>");
			get_str(bbs.id,8);
		}
		if (n==13&&(pf.type>17)) {
			printf(" 메일 발송 이름 >>");
			get_str(bbs.name,8);
		}
		if (n==99) {
			if (strlen(bbs.title)<1) {mess("발송 메일의 제목을 지정하세요..");continue;}
			if (bbs.size<1) 		 {mess("발송 메일의 내용을 지정하세요..");continue;}
			//대상인원 계산
			num_send=0;
			if ((fp=fopen(ID_FILE,"r"))==NULL) return;
			printf("\n");
			printfL2();
			key=key2=Yes;
			while (fread(&pf2,sizeof_pf,1,fp)) {
				if (pf2.type<torank1||pf2.type>torank2) continue;
				if (pf2.level<tolevel1||pf2.level>tolevel2) continue;
				if (pf2.job<tojob1||pf2.job>tojob2) continue;
				if ((tosex==1&&pf2.sex!='m')||(tosex==2&&pf2.sex!='f')) continue;
				num_send++;
			}
			fclose(fp);
			printf("\n");
			printf(" ### 전체 메일 대상 회원수 %d 명 ###\n",num_send);
			printf(" ### 발송제목 : %s\n",bbs.title);
			if (num_send==0) {mess("해당 조건의 회원이 0 명입니다.");continue;}
			if (!yesno2("     전체 메일을 보내겠습니까?  (Y/n) >>",Yes)) continue;

			num=1;
			key=Yes;
			key2=Yes;
			fp=fopen(ID_FILE,"r");
			time(&bbs.date);  //발신 날짜 저장
			bbs.date2=0;
			strcpy(bbs.id,pf.id);
			strcpy(bbs.name,pf.name);
			bbs.gd=' ';
			bbs.del=No;
			strcpy(bbs.passwd,"");
			strcpy(bbs.key,"전체메일");
			bbs=bbs;
			printf(" ### 전체메일발송시작....... ###\n");
			while (key&&fread(&pf2,sizeof_pf,1,fp)) {
				del_space(pf2.id);del_esc(pf2.id);
				del_esc3(pf2.id);del_esc3(pf2.name);del_esc3(pf2.nick);
				if (strlen(pf2.id)<3) continue;
				if (pf2.type<torank1||pf2.type>torank2) continue;
				if (pf2.job<tojob1||pf2.job>tojob2) continue;
				if (pf2.level<tolevel1||pf2.level>tolevel2) continue;
				if ((tosex==1&&pf2.sex!='m')||(tosex==2&&pf2.sex!='f')) continue;
				nn=0;
				while (nn++<30) {				//만일 30번을 다른아이디로변경했으면 끝
                    if (pf2.tmail[0]<3||is_same(pf2.id,pf2.tmail)) break;
                    if (strlen(pf2.tmail)>2&&!read_pf2(pf2.tmail)) {
						strcpy(pf2.id,"");
						break;
					}
				}
				if (strlen(pf2.id)<1) continue;
                if (strlen(pf2.tmail)>0&&strlen(pf2.tmail)<3) continue;
				wmail4(pf2.id,TMP_FILE,bbs.title,9);
				printf(" %4d. %-8s(%-8s) ",num++,pf2.id,pf2.name);
				if (num%3==0) printf("\n");
				if (key2&&num%30==0) {
					printf(" ### 전체 메일을 계속 보낼까요? 계속(Y/n) 연속(S)>>");
					lineinput(buff,2);check_han(buff);printf("\n");
					if (is_same(buff,"n")) key=No;
					else if (is_same(buff,"s")) key2=No; //연속
				}
			}
			fclose(fp);
			wmail4(pf.id,TMP_FILE,bbs.title,8);
			strcpy(NOW_FILE ,"menu/_allmail.tit");strcpy(NOW_FILED,"menu/_allmail.txt");
			touch_file(NOW_FILE);touch_file(NOW_FILED);
			time(&bbs.date2);
			bbs_add(1);
			printf("\n");
			mess("메일 발송이 모두 끝났습니다.");
			unlink(TMP_FILE);
		}
	}
	unlink(TMP_FILE);
    go_top(0);
}



//동호회 전체 메일 보내기
wmall_club()
{
	FILE *fp;
	int i,key,key2,n=0,nn,n1,n2;
	char buff[300];
	char buff2[140];
	char id[9];
	char torank1,torank2;
	char tojob1,tojob2;
	int tolevel1,tolevel2,tosex,num,num_all,num_send;
	char tosexstr[10],dir[300],id_file[300];
	CPF cpf2;

	key=Yes;
	if (pf.type<17&&cpf.type<ccfg.wmalltype) key=No;
	if (!key&&in_file2("system/wmall.id",pf.id)) key=Yes;
	if (!key) {mess("전체 메일을 보낼수 없는 동호회 등급입니다.");return;}

	torank1=0;torank2=14;tojob1=0;tojob2=19;tolevel1=0;tolevel2=999999;tosex=0;
	sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
	num_all=count_cpf(id_file);

	strcpy(bbs.id,pf.id);
	strcpy(bbs.name,pf.name);
	strcpy(bbs.filename,"");
	strcpy(bbs.key,"전체메일");
	bbs.size=0;bbs.filesize=0;
	bcfg.wmode=0;
	strcpy(go.code,"wmall");strcpy(bbs.title,"");
	unlink(TMP_FILE);
	strcpy(NOW_FILE ,"menu/_allmail.tit");strcpy(NOW_FILED,"menu/_allmail.txt");
	touch_file(NOW_FILE);touch_file(NOW_FILED);
	while (1) {
		if (torank1<1)	torank1=0;if (torank1>19) torank1=19;
		if (torank2<1)	torank2=0;if (torank2>19) torank2=9;
		if (tojob1<1)  tojob1=0;if (tojob1>19)	tojob1=19;
		if (tojob2<1)  tojob2=0;if (tojob2>19)	tojob2=19;
		if (tolevel1<1) tolevel1=0;if (tolevel1>999999) tolevel1=999999;
		if (tolevel2<1) tolevel2=0;if (tolevel2>999999) tolevel2=999999;
		if		(tosex==1) strcpy(tosexstr,"남성회원");
		else if (tosex==2) strcpy(tosexstr,"여성회원");
		else			   strcpy(tosexstr,"구분없음");
		bbs.size=file_size(TMP_FILE);

		cls();
		printf(" *** 동호회 회원 전체 메일 보내기 (총:%d명)***\n",num_all);
		printfL2();
		printf("  1. 회 원 등 급 : %d - %d \n",torank1,torank2);
		printf("  2. 회 원 성 별 : %s      \n",tosexstr);
		printf("  3. 회 원 레 벨 : %d - %d \n",tolevel1,tolevel2);
		printf("  4. 직 업 분 류 : %d - %d \n",tojob1,tojob2);
		printf(" 10. 메 일 제 목 : %s\n",bbs.title);
		printf(" 11. 메 일 내 용 : %d Byte\n",bbs.size);
		if (pf.type>17) {
		printf(" 12. 보 내 는 ID : %s\n",bbs.id);
		printf(" 13. 보내는 이름 : %s\n",bbs.name);
		}
		printf("\n");
		printf(" 99. 위의 조건 회원에게 메일 보내기\n");
		printfL2();
		printf("  선택  종료(Q) >>");
		n=numinput();
		printf("\n");
		if (n<=0) break;
		if (n==1) {
			printf(" 대상회원 등급 하한  >>");torank1=get_num(torank1,2);printf("\n");
			printf(" 대상회원 등급 상한  >>");torank2=get_num(torank2,2);printf("\n");
		}
		if (n==2) {
			printf(" 대상회원 성별  구분없음(0) 남성(1) 여성(2) >>");tosex=get_num(tosex,1);printf("\n");
		}
		if (n==3) {
			printf(" 대상회원 레벨 하한  >>");tolevel1=get_num(tolevel1,6);printf("\n");
			printf(" 대상회원 레벨 상한  >>");tolevel2=get_num(tolevel2,6);printf("\n");
		}
		if (n==4) {
			view("frm/guest/job.scr");
			printf("\n 대상회원 직업번호 하한  >>");tojob1=get_num(tojob1,2);printf("\n");
			printf(" 대상회원 직업번호 상한  >>");tojob2=get_num(tojob2,2);printf("\n");
		}
		if (n==10) {
			view("frm/editor/writetit.wma");
			get_str(bbs.title,60);
		}
		if (n==11) {  //내용
			input_tmp(0);
		}
		if (n==12&&(pf.type>17)) {
			printf(" 메일 발송 ID   >>");
			get_str(bbs.id,8);
		}
		if (n==13&&(pf.type>17)) {
			printf(" 메일 발송 이름 >>");
			get_str(bbs.name,8);
		}
		if (n==99) {
			if (strlen(bbs.title)<1) {mess("발송 메일의 제목을 지정하세요..");continue;}
			if (bbs.size<1) 		 {mess("발송 메일의 내용을 지정하세요..");continue;}
			//대상인원 계산
			num_send=0;
			if ((fp=fopen(id_file,"r"))==NULL) return;
			printf("\n");
			printfL2();
			key=key2=Yes;
			while (fread(&cpf2,sizeof_cpf,1,fp)) {
				if (cpf2.type<torank1||cpf2.type>torank2) continue;
				if (!read_pf4(cpf2.id)) continue;
				if (pf2.level<tolevel1||pf2.level>tolevel2) continue;
				if (pf2.job<tojob1||pf2.job>tojob2) continue;
				if ((tosex==1&&pf2.sex!='m')||(tosex==2&&pf2.sex!='f')) continue;
				num_send++;
			}
			printf("\n");
			printf(" ### 전체 메일 대상 회원수 %d 명 ###\n",num_send);
			printf(" ### 발송제목 : %s\n",bbs.title);
			if (num_send==0) {mess("해당 조건의 회원이 0 명입니다.");continue;}
			if (!yesno2("     전체 메일을 보내겠습니까?  (Y/n) >>",Yes)) continue;

			num=1;
			key=Yes;
			key2=Yes;
			fp=fopen(id_file,"r");
			time(&bbs.date);  //발신 날짜 저장
			bbs.date2=0;
			strcpy(bbs.id,pf.id);
			strcpy(bbs.name,pf.name);
			bbs.gd=' ';
			bbs.del=No;
			strcpy(bbs.passwd,"");
			strcpy(bbs.key,"전체메일");
			bbs=bbs;
			printf(" ### 전체메일발송시작....... ###\n");
			while (key&&fread(&cpf2,sizeof_cpf,1,fp)) {
				if (cpf2.type<torank1||cpf2.type>torank2) continue;
				if (!read_pf2(cpf2.id)) continue;
				del_esc3(pf2.id);del_esc3(pf2.name);del_esc3(pf2.nick);
				if (strlen(pf2.id)<3) continue;
				if (pf2.level<tolevel1||pf2.level>tolevel2) continue;
				if (pf2.job<tojob1||pf2.job>tojob2) continue;
				if ((tosex==1&&pf2.sex!='m')||(tosex==2&&pf2.sex!='f')) continue;
                if (strlen(pf2.tmail)>0&&strlen(pf2.tmail)<3) continue;
				nn=0;
				while (nn++<30) {				//만일 30번을 다른아이디로변경했으면 끝
                    if (pf2.tmail[0]<3||is_same(pf2.id,pf2.tmail)) break;
                    if (strlen(pf2.tmail)>2&&!read_pf2(pf2.tmail)) {
						strcpy(pf2.id,"");
						break;
					}
				}
				if (strlen(pf2.id)<1) continue;
                if (strlen(pf2.tmail)>0&&strlen(pf2.tmail)<3) continue;
				wmail4(pf2.id,TMP_FILE,bbs.title,9);
				printf(" %4d. %-8s(%-8s) ",num++,pf2.id,pf2.name);
				if (num%3==0) printf("\n");
				if (key2&&num%30==0) {
					printf(" ### 동호회회원에게 계속 보낼까요? 계속(Y/n) 연속(S)>>");
					lineinput(buff,2);check_han(buff);printf("\n");
					if (is_same(buff,"n")) key=No;
					else if (is_same(buff,"s")) key2=No; //연속
				}
			}
			fclose(fp);
			wmail4(pf.id,TMP_FILE,bbs.title,8);
			strcpy(NOW_FILE ,"menu/_allmail.tit");strcpy(NOW_FILED,"menu/_allmail.txt");
			touch_file(NOW_FILE);touch_file(NOW_FILED);
			time(&bbs.date2);
			bbs_add(1);
			printf("\n");
			mess("메일 발송이 모두 끝났습니다.");
			unlink(TMP_FILE);
		}
	}
	unlink(TMP_FILE);
}



form_mail()
{
	FILE *fp1,*fp2;
	char buff[780],buff1[780],buff2[780];
	char filename[200];
	int count,i,n,total,found;
	for (i=0;i<20;i++) strcpy(a_str[i],"");
	while (1) {
		_cont1:
		unlink(TMP_FILE);
		strcpy(filename,"");
		view("frm/fmail/fmail.scr");     //메일화면 출력
		lineinput(buff,30);check_cmd(buff);printf("\n");
		if (is_quit(buff)) break;
		if (strlen(buff)<1) continue;
		sprintf(filename,"frm/fmail/%s.txt",buff);
		if (pf.type>17&&is_same(mmc[0],"ed")&&strlen(mmc[1])>0) {
			if (is_same(mmc[1],"fmail.scr")) sprintf(filename,"frm/fmail/%s",mmc[1]);
			else sprintf(filename,"frm/fmail/%s.txt",mmc[1]);
			xedit2(filename);
			printf("\n");
			system("ls -l frm/fmail/");
			pressenter();
			continue;
		}
		else if (is_same(buff,"e")) {
			get_vmail_str();
			continue;
		}
		if (!view(filename)) continue;
		view(filename);printf(VTXOFF);
		get_vmail_str();
		_cont2:
		if((fp1=fopen(filename,"r"))==NULL) return;
		if((fp2=fopen(TMP_FILE,"w"))==NULL) return;
		while (fgets(buff,300,fp1)) {
			del_enter(buff);
			for (i=1;i<15;i++) {
				sprintf(buff1,"!문자!%02d",i);
				strchng2(buff,buff1,a_str[i]);
			}
			fprintf(fp2,"%s\n",buff);
		}
		fclose(fp1);
		fclose(fp2);
		view(TMP_FILE);printf(VTXOFF);
		printf("\n ##  이 내용으로 발송할까요? 발송(Y) 재선택(N) 취소(Q) 내용수정(E) >>");
		lineinput2(buff,3);check_han(buff);printf("\n");
		if (is_same(buff,"e")) {get_vmail_str();goto _cont2;}
		if (is_same(buff,"n")) continue;
		if (is_quit(buff)) {unlink(TMP_FILE);return;}
		if (is_same(buff,"y")) return;
		goto _cont2;
	}
	unlink(TMP_FILE); //취소시..
}

get_vmail_str()
{
	int i,i2;
	char buff[100];
	i=1;
	printf(" 본문을 작성하십시요.\n");
	printf(" 줄당 한글16자로 14줄까지,끝내기:첫칸에서 . 입력\n");
	while(i<15) {
		printf("%3d:",i);
		lineinput(buff,34);printf("\n");
		if (is_same(buff,".")||is_quit(buff)) {
			for (i2=i;i2<20;i2++) strcpy(a_str[i2],"");
			break;
		}
		strcpy(a_str[i++],buff);
	}
}

			//수신		 발신
re_answer(char *id1,char *id2)		  //자동 응답메일
{
	FILE *fp;
	char filename[100],title[100];
	char buff[300],buff1[300],buff2[300],buff3[300],buff4[300];
	sprintf(filename,"system/manswer.id");
	if ((fp=fopen(filename,"r"))==NULL) return;
	while (fgets(buff,390,fp)!=NULL) {
		sscanf(buff,"%s%s",buff1,buff2);
		if (is_same(buff1,id1)) {
			sprintf(title,"[답장]%s",bbs.title);
			title[60]=0;
			sprintf(buff3,"frm/letter/%s.txt",buff2);
			strchng2(buff3,"../","./");
			strchng2(buff3,"//","/");
			wmail4(id2,buff3,title,1);
			break;
		}
	}
	fclose(fp);
}
