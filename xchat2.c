///////////////////////////////////////////////////////////////////////////
//∞≥πﬂ : ±Ëº∫¥Î  xhost@xhost.co.kr (016-320-7882)
///////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include "chat.h"
#include <dirent.h>    //µ∑∫≈‰∏Æ »≠¿œ∏Ì ∫Ø»Ø«‘ºˆ∂ßπÆø° « ø‰

chat_line(int mode,char *chatbuff1)
{
	int i,found=No;
	char buff[300],buff1[300],buff2[30],name[30];
	char chatbuff[500];
	if (roomtype<1) roomtype=1;
	if (roomnum <1) roomnum =1;
	strcpy(chatbuff,chatbuff1);
	strcpy(name,"");
	check_mynum();		 //≥™¿« ¥Î»≠πÊπ¯»£ √£±‚
	if (mynum>0) strcpy(name,croom.room[mynum-1].nick);
	if (mode>=93) {
		add_chat_line(mynum,mode,name,chatbuff);
		return;
	}
	else if (mode>=90) {
		add_chat_line(0,mode,name,chatbuff);
		return;
	}
	if (croom.count<2) check_newboss();

	if (mode==0&&mynum>0&&croom.room[mynum-1].mode==1) {
		read_croom();
		check_mynum();
		del_croom(roomnum);
		save_croom();
		return;
	}

	check_cmd(chatbuff);
	if (is_same(mmc[0],"/n")||is_same(mmc[0],"/ni")||is_same(mmc[0],"/nn")||is_same(mmc[0],"/al")) {
		if (strlen(mmc[1])>1&&mynum>0) {
			read_croom();
			check_mynum();
			mmc[1][12]=0;
			//πÊ¿Â¿œ∂ß ∫∞∫¥∫Ø∞Ê
			for (i=0;i<20;i++) {
				if (is_same(croom.id,croom.room[i].id)&&is_same(croom.hostname,croom.room[i].hostname)) {
					strcpy(croom.name,mmc[1]);
					break;
				}
			}
			sprintf(chatbuff,"<u> %s </u>¥‘¿Ã \"<u>%s</u>\"∑Œ ¥Î»≠∏Ì¿ª ∫Ø∞Ê«ﬂΩ¿¥œ¥Ÿ.",croom.room[mynum-1].nick,mmc[1]);
			strcpy(croom.room[mynum-1].nick,mmc[1]);
			save_croom();
			chat_line(92,buff);
			return;
		}
	}
	else if (strlen(tosay)>0&&!stristr(tosay,"¿¸√º∏ª")) {
		char buff[100];
		strcpy(buff,"");
		found=No;
		for (i=0;i<20;i++) {
			if ((strlen(croom.room[i].id)>1)&&
				is_same(croom.room[i].nick,tosay)) {
				strcpy(buff,croom.room[i].nick);
				found=Yes;
				add_chat_line(i+1,94,croom.room[mynum-1].nick,chatbuff);
			}
		}
		if (!found) strcat(chatbuff,"[¿¸º€Ω«∆–]");
		add_chat_line(mynum,96,buff,chatbuff);
	}
	else if (strlen(tosay2)>0) {	 //¥Î»≠πÊø‹ »∏ø¯ø°∞‘ ¿¸º€
		to_say3(2,tosay2,chatbuff);
	}
	else if (croom.count>0) {
		add_chat_line(0,mode,name,chatbuff);
	}
}

add_chat_line(int mynum,int mode,char *name,char *chatbuff1)
{
	FILE *fp;
	char buff[2500],buff1[200],buff2[200],chat_file[300],facebuff[100];
	char chatbuff[1500],chatbuff2[1500];
	int  color;
	int i,ii;
	int facenum=0,fontface=0,fontnum=0;
	strcpy(chatbuff,chatbuff1);
	del_enter(chatbuff);
	if ((mynum>0&&mynum<30)||mode==93||mode>=97) {
		if (!is_same(croom.room[mynum-1].tty,"www")) {
			if (strlen(croom.room[mynum-1].id)>1&&croom.room[mynum-1].mode!=1) {
				del_tag(chatbuff);
				chat_say_tty(mode,
							 croom.room[mynum-1].hostno,
							 croom.room[mynum-1].tty,
							 name,
							 chatbuff);
			}
			return;
		}
		sprintf(chat_file,"user/chat/wchat.%d.%d",roomnum,mynum);
	}
	else  sprintf(chat_file,"user/chat/wchat.%d",roomnum);
	if ((fp=fopen(chat_file,"a+"))==NULL) {
		fp=fopen(chat_file,"w+");
	}
	if (fp==NULL) {mess("%s FILE ERR",chat_file);return;}
	color=1;

	strcpy(chatbuff2,chatbuff);
	strcpy(facebuff,"");
	strchng2(chatbuff2,"\"","\\\"");
	strchng2(chatbuff2,"\'","\\\'");
//	  fprintf(fp,"%2d %2d %2d %d %d %-25.25s %s\n",
//	  mode,facenum,color,fontnum%10,fontface%10,name,facebuff,chatbuff2);
	fprintf(fp,"%2d %2d %2d %d %d %-25.25s %s%s\n",
	mode,facenum,color,fontnum%10,fontface%10,name,facebuff,chatbuff);
	fclose(fp);
	chmod(chat_file,0666);
	del_tag(chatbuff);
	for (i=0;i<20;i++) {
		if (mynum>0&&i!=mynum-1) continue;
		if (strlen(croom.room[i].id)>1&&croom.room[i].mode!=1) {
			chat_say_tty(mode,
				croom.room[i].hostno,
				croom.room[i].tty,
				name,
				chatbuff);
		}
	}
}


//7  8∫π±∏
chat_say_tty(int mode,int hostno,char *dtty,char *nick,char *mess_str)
{
	char out[2000],outstr[2000],buff1[10],buff2[10],buffi[50];
	if (strlen(dtty)<1||is_same(dtty,"www")) return;
	if (cfg.count_server<=1) hostno=cfg.hostno;
	if (mode==90||mode==95) return;
	else if (mode==94||mode==96) sprintf(outstr," [7m%12.12s ¢–[m %s",nick,mess_str);
	else if (mode>90) sprintf(outstr,center_line(mess_str));
	else if (is_same(dtty,tty))
		sprintf(outstr," [7m%12.12s ¢∫[m %s",nick,mess_str);
	else
		sprintf(outstr," [m%12.12s ¢∫ %s",nick,mess_str);

	sprintf(out,"[s[%d;1H\n%s[u",chaty2,outstr);
	to_tty2(hostno,dtty,out);
}



