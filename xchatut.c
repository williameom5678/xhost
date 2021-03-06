#include "bbs.h"
#include "chat.h"

int comp_9(const void *a, const void *b)
{
	ROOM *rooma,*roomb;
	rooma = (ROOM *)a;
	roomb = (ROOM *)b;
	return ((rooma->date - roomb->date));
}



set_room_color(char *buffstr) //대화실제목 변경
{
	FILE *fp;
	char buff[300];
	char buff2[100];
	char tid[30];
	int i,n1,n2,found=No;

	strchng(buff,buffstr,","," ");
	for (i=0;i<10;i++) strcpy(mmc[i],"");
	sscanf(buff,"%s%s%s%s",mmc[0],mmc[1],mmc[2],mmc[3]);
	n1=atoi(mmc[1]);
	n2=atoi(mmc[2]);
	for (i=0;i<16;i++) {
		if (is_same(acstr[i],mmc[1])) n1=i+1;
		if (is_same(acstr[i],mmc[2])) n2=i+1;
	}
	xstrncpy(tid,mmc[0],8);
	printf("[%d;1H\n",chaty2);
	for (i=0;i<20;i++) {
        if (is_same(user_room[i].id,tid)) {
            user_room[i].color1=(byte)n1;
            user_room[i].color2=(byte)n2;
            if (user_room[i].color1<1||user_room[i].color1>16) user_room[i].color1=99;
            if (user_room[i].color2<1||user_room[i].color2>16) user_room[i].color2=99;
            get_ansicolor(buff2,user_room[i].color1-1,user_room[i].color2-1);
            printf(" ## %8s님의 색상 글자색:%2d 배경색:%2d  7[s%s  %-8s %2d/%2d ",user_room[i].id,n1,n2,buff2,user_room[i].id,n1,n2);
            printf(" /색상 %s %s/%s",user_room[i].id,
            user_room[i].color1>16?"  ":acstr[user_room[i].color1-1],user_room[i].color2>16?"  ":acstr[user_room[i].color2-1]);
            printf("[u8",user_room[i].id,n1,n2,buff2,user_room[i].id,n1,n2);
			break;
		}
	}
}



chat_convert(char *chatbuff)
{
	strchng2(chatbuff,"![","[");
	strchng2(chatbuff,"!cls_!","[2J[;H");
    strchng2(chatbuff,"!id__!",userid);
    strchng2(chatbuff,"!name!",userna);
    strchng2(chatbuff,"!n_id!",userid);
    strchng2(chatbuff,"!n_na!",userna);
    strchng2(chatbuff,"!n_ni!",userni);
    strchng2(chatbuff,"!별명!",userni);
    strchng2(chatbuff,"!이름!",userna);
    strchng2(chatbuff,"!아디!",userid);
}




roomtype_limit()	 //분류별 제한읽기
{
	FILE *fp;
	CROOM tcroom;
	int found=No;
	if ((fp=fopen("system/chattype.dat","r"))==NULL) return found;
	if (roomtype<1||roomtype>40) return found;
	fseek(fp,roomtype*sizeof_croom,SEEK_SET);
	if (fread(&tcroom,sizeof_croom,1,fp)) {
		if (tcroom.age1>0)	{croom.age1=tcroom.age1;found=Yes;}
		if (tcroom.age2>0)	{croom.age2=tcroom.age2;found=Yes;}
		if (tcroom.itype>0) {croom.itype=tcroom.itype;found=Yes;}
		if (tcroom.sex>0)	{croom.sex=tcroom.sex;found=Yes;}
	}
	fclose(fp);
	return found;
}


nick_name(char *tid,char *nick)
{
	char buff[200];
	printf("[%d;1H\n",chaty2);
	if (strlen(nick)<3) printf(" ## 사용법 /ni 대화명  (대화명은 3~8자입니다.)");
	else {
        room=user_room[mynum-1];
		if (!is_same(room.id,tid)||!is_same(room.hostname,hostname)) {
			check_mynum();
            if (mynum>0) room=user_room[mynum-1];
		}
		if (is_same(room.id,tid)&&is_same(room.hostname,hostname)) {
			sprintf(buff," ## %s님이 %s로 대화명을 바꾸었습니다.",room.nick,nick);
            xstrncpy(user_room[mynum-1].nick,nick,12);
			save_croom();
            save_user_room(mynum);
            chat_line(92,buff);
		}
	}
}


room_sys()
{
	char buff[300],buff1[300],filename[300];
	int i,n;
	if (!read_croom()) return;
    read_user_room();
	printf("[%d;1H\n",chaty2);
    if (pf.type<17&&!is_same(croom.id,userid)) {
		printf("\n ## 이 명령은 대화실 개설자의 권한입니다.\n");
		return;
	}
	while(1) {
		if (strlen(croom.title)<2) sprintf(croom.title,"%s 대화실",cfg.hostname);
		open_str(croom,buff1,roomnum);
		if (croom.max<1||croom.max>99) croom.max=99;
		printf("[%d;1H\n",chaty2);
		printf("\n << 방장 권한 변경 >> 방번호:%d-%d (방장:%s)\n",croom.roomtype,croom.roomnum,croom.id);
        printf(" 1. 방제목변경 %s\n",croom.title);
        printf(" 2. 인원수변경 %d (현재 %d명 대화중)\n",croom.max,croom.count);
        printf(" 3. 형  식변경 %d (%s)\n",croom.openmode,buff1);
        printf(" 4. 방  장변경 %s\n",croom.id);
        printf(" 5. 환영메세지 %s\n",croom.stitle);
        printf(" 6. 칼 라 변경\n");
        printf(" 7. 특정ID 대화 제한 (제한/금지)\n");
		printf(" 8. HTML 태그사용 (현재:%s)\n",rbit(croom.wmode,1,"사용허가","사용금지"));
		printf("99. 변경완료\n");
		n=numselect();
		printf("[%d;1H\n",chaty2);
		if (n<0) goto _quit;
		else if (n==1) {
			printf("\n 방제목>>");
			lineinput2(buff,50);
			if (strlen(buff)>2) strcpy(croom.title,buff);
		}
		else if (n==2) {
			printf("\n 인원제한 (현재:%d)>>",croom.max);
			lineinput2(buff,2);
			if (atoi(buff)>1) croom.max=atoi(buff);
			if (croom.max<2) croom.max=2;
			if (croom.max>20) croom.max=20;
		}
		else if (n==3) {
			printf(" 대화실 종류를 선택하여주세요...\n");
            printf(" 1.공개대화실  2.비공개\n");
			i=numselect();
			if (i>0&&i<5) croom.openmode=i;
			if (i==2) {
				lineinput3(" 대화실 진입비밀번호:(2자이상)>>",croom.passwd,4);
				if (strlen(croom.passwd)<2) {
					strcpy(croom.passwd,"");
					croom.openmode=1;
				}
			}
		}
		else if (n==4) {
			printf(" 방장권한 위임>>");
			lineinput2(buff,12);
			if (strlen(buff)>2&&in_croom(buff)) {
                xstrncpy(croom.id,room.id,12);
                xstrncpy(croom.name,room.nick,12);
                xstrncpy(croom.hostname,room.hostname,20);
			}
		}
        else if (n==5) {
            printf(" 대화방 환영 메시지를 입력하십시오.\n");
            printf(" 내용>>");
            lineinput2(buff,60);
            if (!is_quit(buff))
                strcpy(croom.stitle,buff);
        }
		else if (n==6) {
			printf(" ## 사용법 아이디 글자색 배경색  (색상은 1 ~ 16)  (예 XHOST 푸 녹  XHOST 2 11)\n");
			printf(" 색상번호: 흑 푸 연 감 주 보 황 하 회 군 녹 청 적 분 노 백\n");
			printf(" 색상변경:>>");
			lineinput2(buff,30);
			if (strlen(buff)>3) set_room_color(buff);
		}
		else if (n==7) {
			exit_chatman();
		}
		else if (n==8) croom.wmode = gtt_bit(croom.wmode,1);
		else if (n==99) {
			_quit:
			save_croom();
            save_user_room(0);
			chat_title(1);
			printf("\n## 변경이 완료되었습니다. ##");
			break;
		}
	}
}



exit_chatman()
{
	FILE *fp;
	int mode,n,i,ii,count;
	char modestr[30],buff[300],buff2[300],tid[30],filename[300];
	printf("[%d;1H\n",chaty2);
    printf("  1. 대화실 진입 가능 등급 (%d)\n",croom.itype);
    printf("  2. 대화실 진입 가능 나이 (%d-%d)\n",croom.age1,croom.age2);
    printf("  3. 대화실 진입 가능 성별 1:남 2:여 (%d)\n",croom.sex);
	printf("  선택 >>");
	lineinput2(buff,20);printf("\n");n=atoi(buff);
	printf("[%d;1H\n",chaty2);
    if (n==1) {
		//진입등급
		lineinput3(" 대화실     진입등급: (0-19      0:무시) >>",buff,2);
		croom.itype=atoi(buff);
	}
    else if (n==2) {
		lineinput3(" 대화실     나이 (시작,끝 )   (예:10 30) >>",buff,10);
		check_cmd(buff);
		strcpy(croom.passwd,"");
		if (mmn[0]>0&&mmn[1]>0) {
			croom.age1=(byte)mmn[0];
			croom.age2=(byte)mmn[1];
		}
	}
    else if (n==3) {
		lineinput3(" 대화실     진입성별: (1:남 2:여 0:무시) >>",buff,1);
		croom.sex=atoi(buff);
	}
}


invite_man(char *tid,char *buffstr)
{
    char messbuff[300],buff[100],toid[9];
	int n,key;
	printf("[%d;1H\n",chaty2);
	strcpy(toid,tid);
	if (strlen(toid)<3) {  //아이디 지정안됨
		if (sel_user("frm/say/invite.mes",0))
			strcpy(toid,moni.id);
	}
	key=No;
	n=is_moni(toid);
	if (strlen(toid)<3) printf(" ## 사용법 : /in 이용자ID   (도움말: /h)");
	else if (n==0)		printf(" ## [%s] 님은 현재 접속중이 아닙니다.",toid);
	else if (n==4||(n==5&&!bit(cfg.messmode,4))||
			 n==6||n==8||n==14||
			(n==3&&bit(cfg.messmode,5))||
			(n==4&&bit(cfg.messmode,6))||
			(n==15&&bit(cfg.messmode,6))||
			(n==16&&bit(cfg.messmode,1))||
			 n==19) printf(" ## [%s] 님은 초대가능상태가 아닙니다.",toid);
	else key=Yes;
	if (key==No) return;
    strcpy(messbuff,"");
	strcpy(buff,"");
    if (strlen(croom.passwd)>0) sprintf(buff,"(비밀번호:%s)",croom.passwd);

         if (strlen(croom.club)>0) sprintf(messbuff,"\n ## %s님을 %s동호회에서 [%s]님이 초대합니다. (%s cchat)%s\n",toid,club_go.name,pf.id,club_go.code,buff);
    else if (n==4&&roomtype==1) sprintf(messbuff," ## %s(%s) => [7m대화초대: go chat %d번방%s[m",pf.id,pf.name,roomnum,buff);
    else if (n==4&&roomtype!=1) sprintf(messbuff," ## %s(%s) => [7m대화초대: %d - %d번방%s[m",pf.id,pf.name,roomtype,roomnum,buff);
    else                        sprintf(messbuff,"[25;15H ## %s님을 %d번 대화실에서 [%s]님이 초대합니다.%s[m",toid,roomnum,pf.id,buff);

    if (to_moni2(moni,0,messbuff)) printf(" ## [%s]님에게 초대메세지를 보냈습니다.\n",toid);
    else                           printf(" ## 초대에 실패하였습니다.\n");
}


show_view_user(int mode,char *str)
{
	printf("[%d;1H\n",chaty2);
	view_user(mode,0,str);	 //접속자보기
}

view_chat_pf(int mode,char *tid)
{
	char buff[200];
	char tbuff[200];
	printf("[%d;1H\n",chaty2);
	if (mode==1)	view_pf(tid);
	else if (mode==2)  view_pfs(tid);
	printf("[%d;%dr[%d;1H\n[%d;1H",chaty1,chaty2,chaty2,chaty2);
}




lottery(int mode,int mode2,char *chatbuff)
{
    char lottfile[300],recfile[300],logofile[20][100];
    char buff[300],buff1[300],buff2[300];
    int i,i2,i3,n,n2,found;
    static char *abig[10]= { "０","１","２","３","４","５","６","７","８","９"};

    if (is_same(go.code,"chat")) printf("[%d;1H\n",chaty2);
    sprintf(buff,"xgame/lottery");check_dir(buff);
    sprintf(buff,"xgame/lottery/%d",mode2);check_dir(buff);
    sprintf(lottfile,"xgame/lottery/%d/lottery.txt",mode2);
    sprintf(recfile ,"xgame/lottery/%d/lottery.rec",mode2);
    for (i=0;i<20;i++)
        sprintf(logofile[i],"xgame/lottery/%d/%02d.log",mode2,i);

    if (mode==1) {
        while(1) {
            if (is_same(go.code,"chat")) printf("[%d;1H\n",chaty2);
            printf("\n");
            printf("  1. 정답파일  %s  %5d Byte\n",lottfile,file_size(lottfile));
            printf("  2. 당첨파일  %s  %5d Byte\n",recfile ,file_size(recfile ));
            for (i=0;i<=14;i++)
            printf(" %2d. 화면파일  %s  %5d Byte\n",i+10,logofile[i],file_size(logofile[i]));
            n=numselect();
                 if (n< 0) break;
            else if (n==1) xedit2(lottfile);
            else if (n==2) xedit2(recfile);
            else if (n>=10&&n<=24) xedit2(logofile[n-10]);
        }
    }
    else {
        strcpy(chatbuff,"");
        if (view(logofile[8])) return;     //사용중지 안내 화면
        found=No;
        for (i=0;i<5;i++) {
            file_line(buff,lottfile,i+1);
            if (strlen(buff)<3) continue;
            check_cmd(buff);
            for (i2=0;i2<5;i2++) {
                a_num[11+i*5+i2]=mmn[i2];
                if (mmn[i2]!=0) found=Yes;
            }
        }
        if (!found) {printf("\n문제가 없습니다.\n");return;}

        _cont1:
        for (i=1;i<6;i++) {
            a_num[i]=rand()%10;
            strcpy(a_str[i],abig[a_num[i]]);
        }
        if (a_num[1]==0||a_num[2]==0||a_num[3]==0||a_num[4]==0) goto _cont1;
        n=a_num[1]*1000+a_num[2]*100+a_num[3]*10+a_num[4];
        if (n<=0) goto _cont1;

        for (i=0;i<7;i++) {
            view(logofile[i]);          //1단계씩 보여주는 화면
        }

        found=a_num[1];
        for (i=2;i<5;i++) if (found!=a_num[i]) found=-1;

        if (found!=-1) {        //전부 동일함
            sprintf(buff,"%s %-8s %2d %4d번 %d등\n",
            now_str(3),pf.id,pf.type,n,i+1);
            put_line(buff,recfile);
            for (i3=0;i3<25;i3++) printf("\n## 특별상 ## \n");
            if (is_same(go.code,"chat"))
                sprintf(chatbuff," ### %s님 특별상 당첨 (번호:%04d)###",pf.id,n);
            return;
        }

        for (i=0;i<5;i++) {
            for (i2=0;i2<5;i2++) {
                if (a_num[11+i*5+i2]==n) {
                    for (i3=0;i3<25;i3++) printf("\n## %d등 ## \n",i+1);
                    view(logofile[10+i]);       //당첨되었을때 보여주는 화면
                    sprintf(buff,"%s %-8s %2d %4d번 %d등\n",
                    now_str(3),pf.id,pf.type,n,i+1);
                    put_line(buff,recfile);
                    if (is_same(go.code,"chat"))
                        sprintf(chatbuff," ### %s님 %d등 당첨 (번호:%04d)###",pf.id,i+1,n);
                    return;
                }
            }
        }
        view(logofile[9]);
        if (is_same(go.code,"chat")) {
            sprintf(chatbuff,"%s 님의 복권 %04d 은 꽝입니다",pf.id,n);
        }
    }
}




//방저장
save_croom()
{
	FILE *fp;
	if (roomnum<1||roomnum>99999) return;
    if (croom.count<1) {del_room_file(roomnum);return;}
    sprintf(ROOM_FILE,"user/data/croom.%d",roomnum);
	strchng2(croom.title,"\\\'","\'");strchng2(croom.title,"\\\"","\"");
	strchng2(croom.title,"\'","\\\'");strchng2(croom.title,"\"","\\\"");
	strchng2(croom.stitle,"\\\'","\'");strchng2(croom.stitle,"\\\"","\"");
	strchng2(croom.stitle,"\'","\\\'");strchng2(croom.stitle,"\"","\\\"");
    cut_badhan(croom.id,12);cut_badhan(croom.name,12);cut_badhan(croom.boss2,12);
    cut_badhan(croom.title,150);cut_badhan(croom.stitle,60);cut_badhan(croom.ctitle,20);
	if ((fp=fopen(ROOM_FILE,"w+"))!=NULL) {
		if (croom.roomnum<1) croom.roomnum=1;
		if (croom.roomtype<1||croom.roomtype>90) croom.roomtype=1;
		fwrite(&croom,sizeof_croom,1,fp);
		fclose(fp);
		chmod(ROOM_FILE,0666);
	}
	fflush(NULL);
}



count_croom()
{
	int i,t;
	croom.count=0;	// 대화방의 현재 인원수
	for (i=0;i<CMAX;i++) {	  // user_room[i].ctime : 마지막 입력시간
		t=how_old(user_room[i].ctime);
        if (strlen(user_room[i].id)>0&&t>30*60) {
			bzero(&user_room[i],sizeof_room);
		}
		else if (strlen(user_room[i].id)>1&&strlen(user_room[i].tty)>2) {
			croom.count++;
		}
	}
	return croom.count;
}


make_quit_croom(int mode)
{
	char buff[300];
	int i;
	ROOM myroom;

    if (roomnum==0) {
		FILE *fp;
		char tROOM_FILE[100];
		ROOM troom;
        sprintf(tROOM_FILE,"user/data/croom.%d",roomnum);
        if ((fp=fopen(tROOM_FILE,"r+"))==NULL) return;
		i=0;
		while(1) {
			fseek(fp,i*sizeof_room,SEEK_SET);
			if (!fread(&troom,sizeof_room,1,fp)) break;
            if (
               (troom.hostno==cfg.hostno&&
			   is_same(troom.tty,tty)&&
               is_same(troom.hostname,hostname) ) ||
               (is_same(pf.id,troom.id)&&is_same(pf.name,troom.name))
               ) {
				strcpy(troom.id  ,"");
				strcpy(troom.name,"");
				strcpy(troom.tty ,"");
				troom.hostno=0;
				fseek(fp,i*sizeof_room,SEEK_SET);
				fwrite(&troom,sizeof_room,1,fp);
			}
			i++;
		}
		fclose(fp);
        return;
    }
	check_mynum();		 // 나의 대화방번호 찾기
	myroom=room;
    if (roomnum<1) return;
	//sprintf(gbuff4,"mynum: %d roomnum:%d",mynum,roomnum);
	strcpy(buff,"");
	del_user_room();
	count_croom();
	if (croom.count<1) {
        del_room_file(roomnum);
		bzero(&croom,sizeof_croom);
		return;
	}
	save_croom();
	save_user_room(mynum);
	check_newboss();
    if (mode==9) return;
    unlink_file("user/chat/wchat.%d.%d",roomnum,mynum);
    if (strlen(myroom.nick)>0) {
        if (mode==2) sprintf(buff,"<u> %s </u>님이 입력시간 시간초과로 종료되었습니다.",myroom.nick);
        else if (mode==3) sprintf(buff,"<u> %s </u>님이 강제 퇴장되었습니다.",myroom.nick);
        else if (strlen(myroom.nick)>0) sprintf(buff,"<u> %s </u>님이 퇴장하였습니다.",myroom.nick);
        add_chat_line(0,92,userni,buff);
    }
    fflush(NULL);
    add_chat_line(0,90,userni,"<script>update(0);</script>");
}




//해당방 인원 조정,방장 조정
check_newboss()
{
	char buff[100];
	ROOM aroom[21];
	int i,ii;

	if (croom.count==0) return;
	if (strlen(croom.id)>0) {
		for (i=0;i<20;i++) {
            if (strlen(user_room[i].id)>2&&
                strlen(user_room[i].tty)>2&&
                is_same(user_room[i].id,croom.id)&&
                is_same(user_room[i].nick,croom.name)&&
                is_same(user_room[i].hostname,croom.hostname)) return;
		}
	}
	ii=0;
	for (i=0;i<20;i++) {
        strcpy(aroom[i].id,"");strcpy(aroom[i].name,"");strcpy(aroom[i].nick,"");strcpy(aroom[i].hostname,"");
	}
	for (i=0;i<20;i++) {
        if (strlen(user_room[i].id)>2&&strlen(user_room[i].tty)>2) {
            if (strlen(user_room[i].id)>0) aroom[ii++]=user_room[i];
		}
	}
	count_croom();
	qsort(aroom, 20, sizeof(aroom[0]),comp_9);
	if (croom.count>0) {
		for (i=0;i<20;i++) {
			if (aroom[i].date>0&&strlen(aroom[i].id)>0) {
				strcpy(croom.id,aroom[i].id);
				strcpy(croom.name,aroom[i].nick);
				strcpy(croom.hostname,aroom[i].hostname);
				save_croom();
				sprintf(buff,"## <u> %s </u>님이 방장이 되었습니다.##",croom.name);
				chat_line(92,buff);
				break;
			}
		}
	}
}


del_tag(char *outstr)
{
    int pos1,pos2,i,ii,qflag;
    char buff1[1000],buff2[1000],buff3[1000],stop,found=No;
    char out[5000];
    strcpy(out,outstr);
	for (ii=0;ii<5;ii++) {
		if (is_char(out,'<')&&is_char(out,'>')) {
			pos1=pos2=0;
			//<3333>1234
			//0123456
            qflag=No;
            stop='\0';
			for(i=0;i<(int)strlen(out);i++) {
                if (!qflag&&out[i]=='\"') {qflag=Yes;stop='\"';}
                else if (!qflag&&out[i]=='\'') {qflag=Yes;stop='\'';}
                else if (qflag&&out[i]==stop) qflag=No;

                     if (!qflag&&pos2==0&&out[i]=='<') pos1=i+1;
                else if (!qflag&&pos1!=0&&out[i]=='>') {pos2=i+1;found=Yes;break;}
			}
            if (pos1>0&&pos2>0&&(pos2-pos1)>1) {
				strcpy(buff1,out);buff1[pos1-1]=0;
				xstrncpy(buff2,&out[pos2]);
				sprintf(out,"%s%s",buff1,buff2);
			}
		}
	}
    if (found) {
        strcpy(outstr,out);
    }
}



disp_tty(int mode)
{
	char buff[500],buff2[100];
	int i;
    printf("[%d;1H\n",chaty2);
	if (mode==2) {
		for (i=0;i<5;i++) {
            printf("PORT:%d ",SERVER_PORT[i]);      //파일편집
		}
        printf("\n");      //파일편집
		for (i=0;i<=cfg.count_server;i++) {
            if (cfg.nip[i]>0) {
                printf("서버IP %d = %s.%d \n",i,cfg.sip,cfg.nip[i]);
			}
		}
		return;
	}
    printf("방현황: 번호 %d-%d: 제목:%s\n  방장:%s %d/%d명 M:%d ID:%s(%s/%s) H:%s\n",
	roomtype,roomnum,croom.title,croom.name,croom.count,croom.max,mynum,userid,userna,userni,hostname);
    for (i=0;i<CMAX;i++) {
        if (strlen(user_room[i].id)>0) {
            printf("%s %d: ID:%-12s(%12s/%12s) TY:%-8s HNO:%d\n",
            (i==mynum-1)?"->":"  ",
			i+1,
            user_room[i].id,
            user_room[i].nick,
            user_room[i].name,
            user_room[i].tty,
            user_room[i].hostno);
            strcpy(buff2,datestr(13,user_room[i].ctime));
            printf("-------------IP:%s IN:%s LA:%d초 %s\n",
            user_room[i].hostname,
            datestr(13,user_room[i].date),
            how_old(user_room[i].ctime),
			buff2);
		}
	}
}


#include <dirent.h>    //디렉토리 화일명 변환함수때문에 필요
//mode==3이면 같은ID또는 같은 tty면 삭제한다.
quit_all_room(int mode)
{
	int i,n,found=No,old_roomnum=roomnum;
    char buff[500];
	DIR *dp;
	struct dirent *dfp;
    CROOM oldcroom;
    oldcroom=croom;
	//기존방 삭제
	if (strlen(tty)<1||strlen(hostname)<1) return;

	//디렉토리에서 croom.*을 읽어낸다.
    dp= opendir("user/data/");
    while((dfp=readdir(dp))!= NULL) {
		if(stristr(dfp->d_name,"croom.")) {
			strcpy(buff,&dfp->d_name[6]);
            roomnum=atoi(buff);
            if (roomnum<1) {
                make_quit_croom(0);
                continue;
            }
            if (!read_croom()) continue;
            read_user_room();
            if (croom.count<1) {unlink(ROOM_FILE);continue;}
            found=No;
            for (i=0;i<CMAX;i++) {
                if (strlen(user_room[i].id)<2) continue;
                if (mode<3&&is_same(user_room[i].id,pf.id)) found=Yes;
                if (mode==3&&pf.type>0&&is_same(user_room[i].id,pf.id)) found=Yes;
                if (mode==3&&
                    user_room[i].hostno==cfg.hostno&&
                    is_same(user_room[i].hostname,hostname)&&
                    is_same(user_room[i].tty,tty)) found=Yes;
                if (found) {
                   strcpy(user_room[i].id,"");
                   strcpy(user_room[i].name,"");
                   strcpy(user_room[i].tty ,"");
                   user_room[i].hostno=0;
                }
            }
            if (found) {
                save_croom();
                save_user_room(0);
            }
		}
	}
	closedir(dp);

	roomnum=old_roomnum;
    croom=oldcroom;
}

del_room_file(int roomnum)
{
	char buff[500];
	if (roomnum<1) return;
    unlink_file("user/data/croom.%d",roomnum);
    unlink_file("user/data/cuser.%d",roomnum);
    unlink_file("user/chat/wchat.%d",roomnum);
    sprintf(buff,"rm -f user/chat/wchat.%d.*",roomnum);
	system(buff);
}

//검사없을때 mynum을 비교후 결정
check_mynum()		//나의 대화방에서 나의 정보가 저장된 위치 찾기
{
	int i,count,old_mynum=mynum;
	count=0;
	mynum=0;
	for (i=0;i<CMAX;i++) {
		if (strlen(user_room[i].id)>1&&
			is_same(user_room[i].id,userid)&&
			is_same(user_room[i].nick,userni)&&
			is_same(user_room[i].hostname,hostname)&&
            !is_same(user_room[i].tty,"www")&&
			is_same(user_room[i].name,userna)&&
			is_same(user_room[i].nick,userni)) {
			room=user_room[i];
			mynum=i+1;
			break;
		}
	}
	return mynum;
}


//검사없을때 mynum을 비교후 결정
check_mynum2()		 //나의 대화방에서 나의 정보가 저장된 위치 찾기
{
	int i,count,old_mynum=mynum;
	ROOM oldroom;
	count=0;
	mynum=0;
	for (i=0;i<CMAX;i++) {
		if (strlen(user_room[i].id)>1&&
			is_same(user_room[i].id,userid)&&
			is_same(user_room[i].hostname,hostname)&&
            !is_same(user_room[i].tty,"www")) {
			room=user_room[i];
			mynum=i+1;
			count++;
		}
	}
	if (count>1) { //같은것 두개이상일때
		count=0;
		for (i=0;i<CMAX;i++) {
			if (strlen(user_room[i].id)>1&&
				is_same(user_room[i].id,userid)&&
				is_same(user_room[i].nick,userni)&&
				is_same(user_room[i].hostname,hostname)&&
                !is_same(user_room[i].tty,"www")&&
				is_same(user_room[i].name,userna)) {
				room=user_room[i];
				mynum=i+1;
				count++;
			}
		}
	}
	if (count>1) { //같은것 두개이상일때
		count=0;
		for (i=0;i<CMAX;i++) {
			if (strlen(user_room[i].id)>1&&
				is_same(user_room[i].id,userid)&&
				is_same(user_room[i].nick,userni)&&
				is_same(user_room[i].hostname,hostname)&&
                !is_same(user_room[i].tty,"www")&&
				is_same(user_room[i].name,userna)&&
				is_same(user_room[i].nick,userni)) {
				room=user_room[i];
				mynum=i+1;
				count++;
				break;
			}
		}
	}

	if (mynum==0&&old_mynum>0) {
		if (strlen(userid)>0&&
			is_same(user_room[old_mynum-1].id,userid)&&
			is_same(user_room[old_mynum-1].hostname,hostname)) mynum=old_mynum;
	}

	return mynum;
}


