////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: 리눅스용 BBS HOST							  //
// 손님가입메뉴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

reset_pf() {
	//가입신청신상의 초기화
	strcpy(pf.id   ,"");
	strcpy(pf.name ,"");
	strcpy(pf.nick ,"");
	strcpy(pf.passwd,"");
	strcpy(pf.pos1 ,"");
	strcpy(pf.pos2 ,"");
	strcpy(pf.addr ,"");
	strcpy(pf.tele ,"");
	strcpy(pf.offi ,"");
	strcpy(pf.gofirst,"");
	strcpy(pf.id_no,"");
	strcpy(pf.mailid,"");
	strcpy(pf.memo1,"");
    strcpy(pf.user1,"");
    strcpy(pf.user2,"");
    strcpy(pf.user3,"");
    strcpy(pf.user4,"");
	strcpy(pf.email,"");
	pf.num=0;
	pf.itype=0;
	pf.myflag=0;
	pf.sex='m';
	pf.job= 0;
	time(&pf.inday);
	time(&pf.login);
	time(&pf.logout);
	pf.birthday=0;
	pf.countw=0;
	pf.countd=0;
	pf.countu=0;
	pf.level=cfg.guestlevel;
	pf.type =cfg.guesttype ;
	pf.cash=cfg.guestcash;
	pf.down=cfg.guestdown*1000;
	pf.all_log=0;			//총접속횟수
	pf.month_log=0; 		//이번달총접속횟수
	pf.all_time=0;			//총이용시간기록  (초)
	pf.month_time=0;		//이번달총이용시간(초)
	pf.left_time=cfg.guesttime;
}



check_sameid2(char *id)
{
	if (read_pf2(id)) {
		strcpy(g_buff1,pf2.id);
		strcpy(g_buff2,pf2.name);
		strcpy(g_buff3,datestr(34,pf2.inday));
		return Yes;
	}
	return No;
}

//현재의신청내역의 정당함을 확인
check_waitid(char *id)
{
	FILE *fp;
	PF pf2;
	if ((fp=fopen(GUEST_FILE,"r"))!=NULL) {
		while (fread(&pf2,sizeof_pf,1,fp)) {
			if (is_same(pf2.id,id)) {
				strcpy(g_buff1,pf2.id);
				strcpy(g_buff2,pf2.name);
				strcpy(g_buff3,datestr(34,pf2.login));
				fclose(fp);
				return Yes;
			}
		}
		fclose(fp);
	}
	return No;
}


check_badid(char *id)
{
	//사용이 거부된 아이디인가?
	FILE *fp;
	PF pf2;
	if ((fp=fopen(GUESTNOT_FILE,"r"))!=NULL) {
		while (fread(&pf2,sizeof_pf,1,fp)) {
			if (is_same(pf2.id,id)) {
				fclose(fp);
				return Yes;
			}
		}
		fclose(fp);
	}
	return No;
}


check_badname(char *id)
{
	if (is_same(cfg.guestid,id)||is_same(cfg.hostname,id)) return Yes;
	if (is_same(id,"root")||stristr(id,"guest")||stristr(id,"손님")) return Yes;
	if (in_file("frm/guest/notgid.txt",id)) return Yes;
	return No;
}


check_badchar(char *buff)
{
	int i=0;
	unsigned char c,ch;
	while ((c=buff[i++])&&i<(int)strlen(buff)) {
			 if (c>='0'&&c<='9') ;
		else if (c=='&'||c=='<'||c=='>'||c=='?'||c==';'||c=='\\') return Yes;
		else if (is_char("+=<>{}[]$&@!^?/.,;*|^",c)) return Yes;
		else if (c==34||c==39||c==96)				 return Yes;
        else if (bit(cfg.guestmode,10)&&!((c>='0'&&c<='9')||(c>='a'&&c<='z')||(c>='A'&&c<='Z'))) return Yes;
		else if (bit(cfg.guestmode,11)&&c>=164) return Yes;
		else if (bit(cfg.guestmode,12)&&c>=202) return Yes;
		else if (i==0&&c=='-') return Yes;
		else if (c>='a'&&c<='z') ;
		else if (c>='A'&&c<='Z') ;                           //
		else if (c>='('&&c<=')') ;                           //
		else if (c=='_'||c=='-') ;
		else if (c<48) return Yes;
	}
	return No;
}


check_badidno(char *idno)
{
	if (in_file("frm/guest/notgidno.txt",idno)) return Yes;
	return No;
}



check_waitidno(char *idno)
{
	FILE *fp;
	PF pf2;
	if ((fp=fopen(GUEST_FILE,"r"))!=NULL) {
		while (fread(&pf2,sizeof_pf,1,fp)) {
			pf2.id_no[6]='-';
			if (strlen(pf2.id)>2&&is_same(pf2.id_no,idno)) {
				sprintf(g_buff2,"%s",pf2.id);
				sprintf(g_buff3,"%s",pf2.name);
				sprintf(g_buff4,"%-10.10s",pf2.addr);
				fclose(fp);
				return Yes;
			}
		}
		fclose(fp);
	}
	return No;
}

check_sameidno(char *idno)
{
	FILE *fp;
	PF pf2;
	if ((fp=fopen(ID_FILE,"r"))!=NULL) {
		while (fread(&pf2,sizeof_pf,1,fp)) {
			if (strlen(pf2.id)<3||is_esc(pf2.id)) continue;
			pf2.id_no[6]='-';
			if (is_same(pf2.id_no,idno)) {
				sprintf(g_buff2,"%s",pf2.id);
				sprintf(g_buff3,"%s",pf2.name);
				sprintf(g_buff4,"%-8.8s",pf2.addr);
				fclose(fp);
				return Yes;
			}
		}
		fclose(fp);
	}
	return No;
}



is_good_idno(char *idstr)
{
	int i,j,k,a[20];
	char buff[300];

	if (strlen(idstr)<8) return No;
	strcpy(buff,idstr);buff[2]=0;
	i=atoi(buff)+1900;
	strcpy(buff,idstr);
	if (buff[6]=='-') {
		buff[6] = '0';
		for(i = 0; i < 14; i++) a[i] = buff[i] - 48;
		j = a[0]*2+a[1]*3+a[2]*4+a[3]*5+a[4]*6+a[5]*7+a[7]*8+a[8]*9+a[9]*2+a[10]*3+a[11]*4+a[12]*5;
		j = j % 11;
		k = 11 - j;
		if(k > 9) k = k % 10;
		j = buff[13] - 48;
		if(j != k) return No;
	}
	else {
		for(i = 0; i < 13; i++) a[i] = buff[i] - 48;
		j = a[0]*2+a[1]*3+a[2]*4+a[3]*5+a[4]*6+a[5]*7+a[6]*8+a[7]*9+a[8]*2+a[9]*3+a[10]*4+a[11]*5;
		j = j % 11;
		k = 11 - j;
		if(k > 9) k = k % 10;
		j = buff[12] - 48;
		if(j != k) return No;
	}
	return Yes;
}


check_badbibi(char *str)
{
	if (strlen(str)<6) return Yes;
	if (str[0]!='0') return Yes;
	if (!is_char(str,'-')) return Yes;
	return No;
}
check_badhand(char *str)
{
	if (strlen(str)<6) return Yes;
	if (str[0]!='0') return Yes;
	if (!is_char(str,'-')) return Yes;
	return No;
}

//0:보통  1:없음  2:재입력
check_selectaddr(char *addr,char *post)
{
	FILE *fp;
	char buff[205];
	char buffaddr[100];
	char buff1[205];
	char buff2[205];
	char buff3[205];
	char abuff[200][101];
	int  i,n,top,linenum;
	strcpy(post,"");
/*
   02 100-011 서울시 중구 충무로1가
   02 100-012 서울시 중구 충무로2가
   02 100-013 서울시 중구 충무로3가
   02 100-014 서울시 중구 충무로4가
*/
	if ((fp=fopen("frm/guest/_post.txt","r"))==NULL) {
		mess("frm/guest/_post.txt 파일이 없습니다.");
		strcpy(addr,"");
		strcpy(post,"");
		return No;
	}
	strcpy(buffaddr,addr);
	del_space(buffaddr);
	i=0;
	while (i<200) {
		if (fgets(buff,200,fp)==NULL) break;
		strcpy(buff1,buff);del_space(buff1);
		if (strlen(buff1)<3||!stristr(buff1,buffaddr)) continue;
		all_trim(buff);
		xstrncpy(abuff[i++],buff,100);
	}
	fclose(fp);
	if (i==0) return 1; 	   //찾는자료 없음
	if (i==1) { 			   //1개 찾았음
		if (strlen(abuff[0])<10) return 1;
		post_str(abuff[0],buff1,buff2,buff3);		//문자열을 전화,우편번호,주소로
		strcpy(addr,buff3);
		strcpy(post,buff2);
		all_trim(addr);all_trim(post);
		return No;
	}
	top=0;
	while(1) {
		cls();
		view("frm/guest/selecta1.scr");
		linenum=0;
		if (top<0||top>199) top=0;
		i=top;
		while(linenum<15&&i<200) {
			if (strlen(abuff[i])<3) break;
			post_str(abuff[i],buff1,buff2,buff3);		//문자열을 전화,우편번호,주소로
			printf("%4d. (%-7.7s) %s\n",i+1,buff2,buff3); //  1. (701-300) 대구시 동구 신서동
			i++;
			linenum++;
		}
		view("frm/guest/selecta2.scr");
		lineinput(buff,20);check_han(buff);check_cmd(buff);printf("\n");
		n=atoi(buff);
		if		(is_same(mmc[0],"b")||is_same(mmc[0],"a")) top-=15;
		else if (is_same(mmc[0],"p")) return 2;  //재입력
		else if (is_same(mmc[0],"x")) return Yes;
		else if (is_same(mmc[0],"q")) return Yes;
		else if (is_same(mmc[0],"m")) return Yes;
		else if (n>0&&n<201) {
			if (strlen(abuff[n-1])<3) continue;
			post_str(abuff[n-1],buff1,buff2,buff3); 	  //문자열을 전화,우편번호,주소로
			if (strlen(buff3)<3) return 2;
			strcpy(addr,buff3);
			strcpy(post,buff2);
            all_trim(addr);all_trim(post);
			return No;		  //찾는자료
		}
		else top+=15;
	}
	return Yes;
}


//02 100-014 서울시 중구 충무로4가
//			   문자열	  전화		우편	   주소
post_str(char *buff0,char *buff1,char *buff2,char *buff3)		//문자열을 전화,우편번호,주소로
{
	char buff[300];
	int i,i1,i2,i3,mode;
	char c,ch;
	strcpy(buff,buff0);
	strcpy(buff1,"");strcpy(buff2,"");strcpy(buff3,"");
	i1=i2=i3=0;
	all_trim(buff);
	strchng2(buff,"   "," ");
	strchng2(buff,"  "," ");
	strchng2(buff,"  "," ");
	strchng2(buff,"  "," ");
	mode=0;
	for (i=0;i<(int)strlen(buff);i++) {
		c=buff[i];
		if (c==' ') mode++;
		else if (mode==0) buff1[i1++]=c;   //전화
		else if (mode==1) buff2[i2++]=c;   //우편
		else			  buff3[i3++]=c;   //주소
		buff1[i1]=0;
		buff2[i2]=0;
		buff3[i3]=0;
	}
	all_trim(buff1);
	all_trim(buff2);
	all_trim(buff3);
}


check_easypass(char *buff)
{
	int n,i,j;
	if (stristr(buff,pf.id)||stristr(buff,pf.name)) return Yes;
	if (stristr(pf.id,buff)||stristr(pf.name,buff)) return Yes;
	n=0;
	for (i=0;i<8;i++) {
		for (j=i+1;j<8;j++) {
			if (buff[i]==buff[j]) n++;
		}
	}
	if (n>3) return Yes;
	return No;
}
