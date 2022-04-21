#include "chat.h"
#define MAX_V 50000

view_(int mode,char *filename,char *sbuff)
{
	char buff[280],ch,ch2,str[280],buff1[280];
	FILE *fp1,*fp2;
	int jpg_tag;
	int ret=No;
	int n=0,i,i1,i2,i3,i4,i5,len;
	int num=0;
	int bmode=1;
	int iii=0;
	int ntype;
	int who=0;
	int today=now_time_num();
	char nomacro=No;
	char ss[MAX_V+15];		  //πﬁ¥¬πˆ∆€
	PF pf3;
	CPF cpf3;

	bzero(ss,100);
	if (cpf.type>=17||pf.type==16) who=1;
	if (pf.type==17||is_same(ccfg.sysop,pf.id)||cpf.type>=18) who=2;
	if (pf.type==18) who=3;
	if (pf.type==19) who=4;
	len=MAX_V;
    strchng2(filename,"..",".");
	if (mode==1) {
		if((fp1=fopen(filename,"r"))==NULL) goto _quit;
		fseekend(fp1);
		len=ftell(fp1);
		rewind(fp1);
		for (i=0;i<MAX_V&&i<len;i++) ss[i]=fgetc(fp1);
		len=i;
		for (n=i;n<i+10;n++) ss[n]=0;
		fclose(fp1);
	}
	else if (mode==3) {
		for (i=0;i<MAX_V;i++) if ((ss[i]=sbuff[i])==0) break;
		len=i;
		for (n=i;n<i+10;n++) ss[n]=0;
	}

	ntype=pf.type;
	if (club_mode) ntype=now_type;
	if (ntype<0||ntype>19) ntype=0;

	ret=Yes;
	iii=0;
	while(iii<MAX_V&&iii<len) {
		ch = ss[iii++];
		if (ch=='\0') break;
		if (ch!='!'||nomacro) putchar(ch);
		else if (ch=='!'&&iii>len-5) putchar(ch);
		else {
			str[0]=ch;
			str[1]=ss[iii++];
			if(iii>=len) goto _quit;
				 if(str[1]=='\n') { printf("!\n");continue;}
			else if(str[1]=='[')  { printf("[");continue;}
			else if(str[1]=='!')  { printf("!!");continue;}
            else if(is_char("@#$%_^&|",str[1])) {
				num=0;
                bzero(buff,30);bzero(buff1,30);
				while((ch = ss[iii++]) != EOF) {
					if(iii>=len) goto _quit;
					if(ch<=' ') {
						buff[num]=0;
							 if (str[1]=='@') strcpy(buff1,b_num(buff,bmode));
						else if (str[1]=='&') strcpy(buff1,b_num(buff,7));
                        else if (str[1]=='_') strcpy(buff1,b_num(buff,9));
						else if (str[1]=='|') strcpy(buff1,b_num(buff,11));
						else if (str[1]=='#') strcpy(buff1,b_disp(buff,1));
						else if (str[1]=='$') strcpy(buff1,b_disp(buff,2));
						else if (str[1]=='^') strcpy(buff1,b_disp(buff,3));
						printf(buff1);
						if(ch=='\n'||ch=='\r') putchar(ch);
						break;
					} else buff[num++]=ch;
				}
				continue;
			}
			str[2]=ss[iii++];
			str[3]=ss[iii++];
			str[4]=ss[iii++];
			str[5]=ss[iii++];
			str[6]=0;
			if(iii>=len) goto _quit;
			if (str[1]=='\n'||str[2]=='\n'||str[3]=='\n'||str[4]=='\n'||str[5]!='!') {
				printf("!");
				iii-=5;if (iii<0) iii=1;
				continue;
			}
			fflush(stdout);
				 if (strcmp(str,"!∞Ëº”!")==0) {nostop=Yes;}
			else if (strcmp(str,"!≈ı∏Ì!")==0) {nomacro=Yes;}
			else if (strcmp(str,"!»≠∏È!")==0) {cls();fflush(stdout);}
			else if (strcmp(str,"!¡ŸπŸ!")==0) printf("\n");
			else if (strcmp(str,"!∏ÿ√„!")==0) {
				if (!nostop) {lineinput(buff,20);if (is_quit(buff)) break;}
			}
			else if (strcmp(str,"!»Æ¿Œ!")==0) {
				if (!nostop) {printf("[Enter] ≈∞∏¶ ¥©∏£Ω Ω√ø‰.");lineinput(buff,20);if (is_quit(buff)) break;}
			}
			else if (strcmp(str,"!ø£≈Õ!")==0) {if (!nostop) getchar3();  }
			else if (strcmp(str,"!¡ˆø¨!")==0) {fflush(stdout);str[0]=ss[iii++];str[1]=0;n=atoi(str);if (pf.type<17) ssleep(n);}
			else if (strcmp(str,"!¿·±Ò!")==0) {fflush(stdout);str[0]=ss[iii++];str[1]=0;n=atoi(str);if (pf.type<17) msleep(n);}
			else if (strcmp(str,"!¿⁄µø!")==0) {str[0]=ss[iii++];str[1]=0;g_yes=atoi(str);}
			else if (strcmp(str,"!πˆ¡Ø!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(VER1);
				else if (ch=='1') printf(VER2);
				else if (ch=='2') printf(MAKENUM);
				else if (ch=='3') printf(MAKEDATE);
			}
			else if (strcmp(str,"!¡ﬂ¡ˆ!")==0) break;
			else if (strcmp(str,"!¡æ∑·!")==0) break;
			else if (strcmp(str,"!≥°__!")==0) break;
			else if (strcmp(str,"!∆˜∆Æ!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(tty);
				else if (ch=='1') printf(&tty[3]);
				else if (ch=='2') printf(hostname);
				else if (ch=='3') {file_line(buff,"system/hostaddr.txt",1);printf(buff);}
				else iii--;
			}
			else if (strcmp(str,"!±§ø™!")==0) {
				ch=tolower(ss[iii++]);
					 if (ch=='0') printf(g_id);
				else if (ch=='1') printf(g_buff1);
				else if (ch=='2') printf(g_buff2);
				else if (ch=='3') printf(g_buff3);
				else if (ch=='4') printf(g_buff4);
				else if (ch=='5') printf("%d",a_num[1]);
				else if (ch=='6') printf("%d",a_num[2]);
				else if (ch=='7') printf("%d",a_num[3]);
				else if (ch=='8') printf(g_id);
				else if (ch=='9') printf(g_name);
				else if (ch=='a') printf("%d",a_num[4]);
				else if (ch=='b') printf("%d",a_num[5]);
				else if (ch=='c') printf("%d",a_num[6]);
				else if (ch=='d') printf("%d",a_num[7]);
				else if (ch=='e') printf("%d",a_num[8]);
				else if (ch=='f') printf("%d",a_num[9]);
				else if (ch=='g') printf("%d",a_num[10]);
				else if (ch=='h') printf("%d",a_num[11]);
				else if (ch=='i') printf("%d",a_num[12]);
				else if (ch=='j') printf("%d",a_num[13]);
				else if (ch=='k') printf("%d",a_num[14]);
				else if (ch=='l') printf("%d",a_num[15]);
				else iii--;
			}
			else if (strcmp(str,"!ø¢Ω∫!")==0) {
				view("logo/_xlogo%c.log",ss[iii++]);
			}
			else if (strcmp(str,"!±§πÆ!")==0) {
				str[0]=ss[iii++];n=atoi(str);
					 if (n==1) printf(g_buff1);
				else if (n==2) printf(g_buff2);
				else if (n==3) printf(g_buff3);
				else if (n==4) printf(g_buff4);
				else if (n==5) printf(g_buff5);
				else if (n==6) printf(g_buff6);
				else if (n==7) printf(g_buff7);
				else if (n==8) printf(g_buff8);
				else if (n==9) printf(g_buff9);
				else if (n==0) printf(g_buff10);
			}
			else if (strcmp(str,"!±§ºˆ!")==0) {
				ch=tolower(ss[iii++]);
					 if (ch=='1') printf("%d",a_num[1]);
				else if (ch=='2') printf("%d",a_num[2]);
				else if (ch=='3') printf("%d",a_num[3]);
				else if (ch=='4') printf("%d",a_num[4]);
				else if (ch=='5') printf("%d",a_num[5]);
				else if (ch=='6') printf("%d",a_num[6]);
				else if (ch=='7') printf("%d",a_num[7]);
				else if (ch=='8') printf("%d",a_num[8]);
				else if (ch=='9') printf("%d",a_num[9]);
				else if (ch=='0') printf("%d",a_num[10]);
				else if (ch=='a') printf("%d",a_num[11]);
				else if (ch=='b') printf("%d",a_num[12]);
				else if (ch=='c') printf("%d",a_num[13]);
				else if (ch=='d') printf("%d",a_num[14]);
				else if (ch=='e') printf("%d",a_num[15]);
			}
			else if (strcmp(str,"!πÆ¿⁄!")==0) {
				str[0]=ss[iii++];str[1]=ss[iii++];n=atoi(str);
                if (n>=0&&n<100) printf("%s",a_str[n]);
			}
			else if (strcmp(str,"!º˝¿⁄!")==0) {
				str[0]=ss[iii++];str[1]=ss[iii++];n=atoi(str);
                if (n>=0&&n<=100) printf("%d",a_num[n]);
			}
			else if (strcmp(str,"!º˝03!")==0) {
				str[0]=ss[iii++];str[1]=ss[iii++];n=atoi(str);
                if (n>=0&&n<=100) printf("%03d",a_num[n]);
			}
            else if (strcmp(str,"!º˝_3!")==0) {
				str[0]=ss[iii++];str[1]=ss[iii++];n=atoi(str);
                if (n>=0&&n<=100) printf("%3d",a_num[n]);
			}
            else if (strcmp(str,"!º˝_5!")==0) {
				str[0]=ss[iii++];str[1]=ss[iii++];n=atoi(str);
                if (n>=0&&n<=100) printf("%5d",a_num[n]);
			}
            else if (is_same(str,"!º˝¡ˆ!")) {
                bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];n=atoi(str);
                bzero(str,20);for (i=0;i<10&&ss[iii]>' ';i++) str[i]=ss[iii++];
                i=atoi(str);
                a_num[n]=i;
			}
			else if (strcmp(str,"!¥Î»≠!")==0) {
				ch=ss[iii++];
					 if (ch=='1') printf(croom.title);
				else if (ch=='2') printf(croom.id);
				else if (ch=='3') printf(croom.name);
				else if (ch=='4') printf("%d",croom.max);
				else if (ch=='5') printf("%d",croom.count);
				else if (ch=='6') printf("%d",croom.openmode);
				else if (ch=='7') {open_str(croom,buff1,roomnum);printf(buff1);}
				else if (ch=='9') printf("%d",croom.itype);
				else if (ch=='0') printf("%s",strlen(croom.passwd)?"¿÷¿Ω":"æ¯¿Ω");
				else if (ch=='a') printf("%d",roomtype);
				else if (ch=='b') printf("%d",roomnum);
				else if (ch=='c') printf(croom.club);
                else if (ch=='x') { //«ˆ¿Á¥Î»≠πÊ ªÛ≈¬ ->a_num ¿˙¿Â
                    FILE *fp;
                    char buff[100];
                    int i,i2,count;
                    CROOM croom;
                    ROOM room;
                    for (i=0;i<100;i++) a_num[i]=0;
                    for (i2=0;i2<300;i2++) {
                        sprintf(buff,"user/data/croom.%d",i2);
                        if ((fp=fopen(buff,"r"))==NULL) continue;
                        if (i2==0) {     //¥Î±‚Ω«¿œ∂ß ø¨º”¿∏∑Œ ¥ı«‘
                            while(fread(&room,sizeof_room,1,fp)) {
                                if (strlen(room.id)>0&&strlen(room.tty)>0) {
                                    a_num[91]++;  //√—¿Œø¯ºˆ
                                    a_num[92]++;  //√—¥Î±‚¿⁄
                                }
                            }
                        }
                        else {
                            if (fread(&croom,sizeof_croom,1,fp)) {
                                if (croom.count>0&&croom.roomtype>0&&croom.roomtype<=40) {
                                    a_num[91]+=croom.count;  //√—¿Œø¯ºˆ
                                    a_num[93]++;  //√—∞≥º≥ºˆ
                                    a_num[croom.roomtype]++;               //πÊ∫–∑˘ ∞≥º≥ºˆ
                                    a_num[40+croom.roomtype]+=croom.count;  //πÊ∫–∑˘¥Á ¿Œø¯
                                }
                            }
                        }
                        fclose(fp);
                    }
                }
				else iii--;
			}
			else if (strcmp(str,"!∞°¿‘!")==0) printf(cfg.guestid);
			else if (strcmp(str,"!æ∆µ!")==0||strcmp(str,"!¿Ã∏ß!")==0||strcmp(str,"!∫∞∏Ì!")==0) {
				ch=ss[iii++];
					 if (strcmp(str,"!æ∆µ!")==0) strcpy(buff,pf.id);
				else if (strcmp(str,"!∫∞∏Ì!")==0) strcpy(buff,pf.nick);
				else							  strcpy(buff,pf.name);
					 if (ch=='0') printf(buff);
				else if (ch=='1') printf("%s"  ,upr(buff));
				else if (ch=='2') printf("%s"  ,lwr(buff));
				else if (ch=='3') printf("%-8s",upr(buff));
				else if (ch=='4') printf("%-8s",lwr(buff));
				else if (ch=='5') printf("%8s" ,buff);
				else if (ch=='6') printf("%-8s" ,buff);
				else {iii--;printf(buff);}
			}
			else if (strcmp(str,"!∏ﬁ¿œ!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(pf.mailid);
				else if (ch=='1') printf("%-8.8s",pf.mailid);
				else if (ch=='2') printf("%s",pf.email);
				else if (ch=='3') printf("%-8.8s",pf.email);
				else if (ch=='4') {file_line(buff,"system/hostaddr.txt",1);printf("%s@%s",pf.email,buff);}
				else {iii--;printf(pf.mailid);}
			}
			else if (strcmp(str,"!æœ»£!")==0) {
				ch=ss[iii++];
					 if (ch=='0'&&bit(allmode,7)) printf(pf.passwd);
				else if (ch=='1'&&bit(allmode,7)) printf("%-8.8s",pf.passwd);
				else if (ch=='9'                ) printf("%-8.8s",pf.passwd);
				else if (bit(allmode,7)) {iii--;printf(pf.passwd);}
			}
			else if (strcmp(str,"!¡÷º“!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(pf.addr);
				else if (ch=='1') printf(pf.pos1);
				else if (ch=='2') printf(pf.add2);
				else if (ch=='3') printf(pf.pos2);
				else if (ch=='4') printf(pf.offi);
				else if (ch=='8') printf("%-20.20s",pf.addr);
				else if (ch=='9') printf("%-10.10s",pf.addr);
				else {iii--;printf(pf.addr);}
			}
			else if (strcmp(str,"!¿¸»≠!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(pf.tele);
				else if (ch=='1') printf(pf.bibi);
				else if (ch=='2') printf(pf.hand);
				else if (ch=='3') printf(pf.otel);
				else {iii--;printf(pf.tele);}
			}
			else if (strcmp(str,"!≥™¿Ã!")==0) printf("%d",get_age(pf));
			else if (strcmp(str,"!ª˝¿œ!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(datestr(0,pf.birthday));
				else if (ch=='1') printf(datestr(37,pf.birthday));
				else if (ch=='2') printf(dateyang(11,pf.birthday,pf.moon));
				else if (ch=='3') printf(yangstr(3,pf.moon));
				else if (ch=='4') printf(datestr(0,pf.wbirthday));
				else if (ch=='5') printf(datestr(37,pf.wbirthday));
				else if (ch=='6') printf(dateyang(11,pf.wbirthday,pf.wmoon));
				else if (ch=='7') printf(yangstr(3,pf.wmoon));
				else if (ch=='8') printf(datestr(0,pf.marryday));
				else if (ch=='9') printf(datestr(37,pf.marryday));
				else if (ch=='_') {
                    bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];
                    printf("%s",datestr(atoi(str),pf.birthday));
				}
				else {iii--;printf(datestr(0,pf.birthday));}
			}
			else if (strcmp(str,"!Ω≈ªÛ!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(is_char("0f",pf.sex)?"ø©":"≥≤");
				else if (ch=='1') printf(pf.id_no);
				else if (ch=='2') printf("%d",pf.job);
				else if (ch=='3') printf(ajob[pf.job]);
				else if (ch=='4') printf(pf.memo1);
				else if (ch=='6') printf("%d",pf.localmenu);
			}
			else if (strcmp(str,"!º“∞≥!")==0) {
				ch=ss[iii++];
					 if (ch=='1') strcpy(buff,pf.memo1);
				else if (ch=='4') strcpy(buff,pf.user1);
				else if (ch=='5') strcpy(buff,pf.user2);
				else if (ch=='6') strcpy(buff,pf.user3);
				else if (ch=='7') strcpy(buff,pf.user4);
				ch=ss[iii++];
					 if (ch=='0') printf("%s",buff);
				else if (ch=='1') printf("%-10.10s",buff);
				else if (ch=='2') printf("%-20.20s",buff);
				else if (ch=='3') printf("%-30.30s",buff);
				else if (ch=='4') printf("%-40.40s",buff);
				else if (ch=='5') printf("%10.10s",buff);
				else if (ch=='6') printf("%20.20s",buff);
				else if (ch=='7') printf("%30.30s",buff);
				else if (ch=='8') printf("%40.40s",buff);
			}
			else if (strcmp(str,"!»∏π¯!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d",pf.idnum);
				else if (ch=='1') printf("%-7d",pf.idnum);
				else if (ch=='2') printf("%d",pf.num+1);
				else if (ch=='3') printf("%-7d",pf.num+1);
				else {iii--;printf("%d",pf.idnum);}
			}
			else if (strcmp(str,"!¡¢º”!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(datestr( 2,pf.inday));
				else if (ch=='1') printf(datestr(19,pf.inday));
				else if (ch=='2') printf(datestr(38,pf.inday));
				else if (ch=='3') printf(datestr( 2,pf.login));
				else if (ch=='4') printf(datestr(19,pf.login));
				else if (ch=='5') printf(datestr(38,pf.login));
				else if (ch=='6') printf(datestr( 2,pf.logout));
				else if (ch=='7') printf(datestr(19,pf.logout));
				else if (ch=='8') printf(datestr(38,pf.logout));
				else if (ch=='_') {
                    bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];
                    printf("%s",datestr(atoi(str),pf.login));
				}
				else iii--;
			}
			else if (strcmp(str,"!¿ÃøÎ!")==0) {
				ch=ss[iii++];
				if (START_TIME==0) time(&START_TIME);
				if (CHECK_TIME==0) time(&CHECK_TIME);
					 if (ch=='0') printf(datestr( 3,START_TIME));
				else if (ch=='1') printf(datestr(20,how_old(START_TIME)));
				else if (ch=='2') printf(datestr(19,CHECK_TIME));
				else iii--;
			}
			else if (strcmp(str,"!µÓ±ﬁ!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d",pf.type);
				else if (ch=='1') printf("%2d",pf.type);
				else if (ch=='2') printf("%s",atype[pf.type]);
				else if (ch=='3') printf("%-8.8s",atype[pf.type]);
				else if (ch=='8') printf("%d",pf.type+1);
				else if (ch=='9') printf("%s",pf.type<18?atype[pf.type+1]:"");
				else {iii--;printf("%d",pf.type);}
			}
			else if (strcmp(str,"!∑π∫ß!")==0) {
				n=next_level(pf.type);
				if (n<0) n=0;
				ch=ss[iii++];
					 if (ch=='0') printf("%d",pf.level);
				else if (ch=='1') printf("%5d",pf.level);
				else if (ch=='2') printf("%8d",pf.level);
				else if (ch=='3') printf("%-8d",pf.level);
				else if (ch=='4') printf("%d",n);
				else if (ch=='5') printf("%8d",n);
				else if (ch=='6') printf("%-8d",n);
				else if (ch=='7') printf("%d",pf.level-n);
				else if (ch=='8') printf("%6d/%-6d",pf.level,pf.level-n);
				else if (ch=='9') printf("%d",UP_LEVEL);
				else {iii--;printf("%d",pf.level);}
			}
			else if (strcmp(str,"!ªÛ≈¬!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%s",disp_istat(0,pf.itype));
				else if (ch=='1') printf("%s",disp_istat(1,pf.istat));
				else {iii--;printf("%d",pf.itype);}
			}
			else if (strcmp(str,"!ƒ≥Ω√!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d",pf.cash);
				else if (ch=='1') printf("%8d",pf.cash);
				else if (ch=='2') printf("%d",pf.cash/10000);
				else if (ch=='3') printf("%8d",pf.cash/10000);
				else if (ch=='9') printf("%8d",UP_CASH);
			}
			else if (strcmp(str,"!∞≥¿Œ!")==0) {
				str[0]=ss[iii++];str[1]=ss[iii++];n=atoi(str);
					 if (n==10) printf("%d",pf.down);
				else if (n==11) printf("%d",pf.down/1024);
				else if (n==12) printf("%6d",pf.down/1024);
				else if (n==13) printf("%s",byte2str(pf.down));
				else if (n==14) printf("%6s",byte2str(pf.down));
				else if (n==15) printf("%-6s",byte2str(pf.down));
				else if (n==21) printf("%d",pf.countw);
				else if (n==22) printf("%d",pf.countu);
				else if (n==23) printf("%d",pf.countd);
				else if (n==24) printf("%d",pf.today/1000000);
				else if (n==25) printf("%d",(pf.today%1000)/1000);
				else if (n==26) printf("%d",pf.today%1000000);
				else if (n==41) printf("%d",pf.all_log);
				else if (n==42) printf("%6d",pf.all_log);
				else if (n==43) printf("%d",pf.month_log);
				else if (n==44) printf("%6d",pf.month_log);
				else if (n==51) printf("%d",pf.all_time);
				else if (n==52) printf("%6d",pf.all_time/60);
				else if (n==53) printf("%7s",datestr(22,pf.all_time));
				else if (n==54) printf("%d",pf.month_time);
				else if (n==55) printf("%6d",pf.month_time/60);
				else if (n==56) printf("%7s",datestr(22,pf.month_time));
				else if (n==61) printf("%d",pf.warn);
				else if (n==62) printf("%3d",pf.warn);
				else if (n==65) printf("%d",pf.good);
				else if (n==66) printf("%6d",pf.good);
				else if (n==67) printf("%s",bit(pf.mymode,9)?"∞≈∫Œ":"«„∞°");
				else if (n==91) printf("%3d",pf.offmin);
				else if (n==98) printf(bitstr(pf.todaymode));
				else if (n==99) printf(bitstr(pf.mymode));
			}
			else if (strcmp(str,"!√º∑¬!")==0||
					 strcmp(str,"!¡ˆ∑¬!")==0||
					 strcmp(str,"!∏≈∑¬!")==0||
					 strcmp(str,"!πÊæÓ!")==0) {
				ch=ss[iii++];
					 if (strcmp(str,"!√º∑¬!")==0) n=pf.p_ph;
				else if (strcmp(str,"!¡ˆ∑¬!")==0) n=pf.p_it;
				else if (strcmp(str,"!∏≈∑¬!")==0) n=pf.p_at;
				else							  n=pf.p_pr;
					 if (ch=='0') printf(bitstr(pf.p_mo));
				else if (ch=='1') printf("%d",n/100000);
				else if (ch=='2') printf("%d",n%100000);
				else if (ch=='3') printf("%5d",n/100000);
				else if (ch=='4') printf("%5d",n%100000);
				else if (ch=='5') printf("%d/%d",n%100000,n/100000);
				else if (ch=='6') printf("%5d/%5d",n%100000,n/100000);
			}
			else if (strcmp(str,"!»£Ω∫!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(cfg.hostname);
				else if (ch=='1') printf("%s"  ,upr(cfg.hostname));
				else if (ch=='2') printf("%s"  ,lwr(cfg.hostname));
				else if (ch=='3') printf("%-8s",upr(cfg.hostname));
				else if (ch=='4') printf("%-8s",lwr(cfg.hostname));
				else if (ch=='5') printf(club_mode?club_go.code:cfg.hostname);
				else if (ch=='6') printf("%s"  ,upr(club_mode?club_go.code:cfg.hostname));
				else if (ch=='7') printf("%s"  ,lwr(club_mode?club_go.code:cfg.hostname));
				else if (ch=='8') printf("%-8s",upr(club_mode?club_go.code:cfg.hostname));
				else if (ch=='9') printf("%-8s",lwr(club_mode?club_go.code:cfg.hostname));
				else if (ch=='a') printf("%d",cfg.hostno);
				else if (ch=='b') printf("%3d",cfg.hostno);
				else {iii--;printf(cfg.hostname);}
			}
			else if (strcmp(str,"!µ∑∫!")==0) {
				ch=ss[iii++];
				getcwd(buff,sizeof(buff));		//«ˆ¿Á¿« µ∑∫≈‰∏Æ
					 if (ch=='0') printf(buff);
				else if (ch=='1') printf(club_mode?club_go.dir:buff);
				else if (ch=='2') printf(club_go.dir);
				else if (ch=='3') printf(go.dir);
				else {iii--;printf(buff);}
			}
			else if (strcmp(str,"!∞°±‚!")==0) {
				ch=ss[iii++];
				if		(ch=='0') printf(go.code);
				else if (ch=='1') printf("%-8s",go.code);
				else if (ch=='2') printf("%s",upr(go.code));
				else if (ch=='3') printf("%s",lwr(go.code));
				else if (ch=='4') printf("%-8s",upr(go.code));
				else if (ch=='5') printf("%-8s",lwr(go.code));
				else if (ch=='6') printf("%d",pf.localmenu);
				else if (ch=='9') printf("%d",go.rec);
				else {iii--;printf(go.code);}
			}
			else if (strcmp(str,"!∞°¿Ã!")==0) {
				ch=ss[iii++];
				if		(ch=='0') printf(go.name);
				else if (ch=='1') printf("%-10.10s",go.name);
				else if (ch=='2') printf("%-20.20s",go.name);
				else if (ch=='3') printf("%-30.30s",go.name);
				else if (ch=='4') printf("%-40.40s",go.name);
				else {iii--;printf(go.name);}
			}
			else if (strcmp(str,"!µø»£!")==0) {
				ch=ss[iii++];
				if		(ch=='0') printf(club_go.code);
				else if (ch=='1') printf("%-8s",club_go.code);
				else if (ch=='2') printf("%s",upr(club_go.code));
				else if (ch=='3') printf("%s",lwr(club_go.code));
				else if (ch=='4') printf("%-8s",upr(club_go.code));
				else if (ch=='5') printf("%-8s",lwr(club_go.code));
				else if (ch=='9') printf("%d",club_go.rec);
				else {iii--;printf(club_go.code);}
			}
			else if (strcmp(str,"!µø¿Ã!")==0) {
				ch=ss[iii++];
				if		(ch=='0') printf(club_go.name);
				else if (ch=='1') printf("%-10.10s",club_go.name);
				else if (ch=='2') printf("%-20.20s",club_go.name);
				else if (ch=='3') printf("%-30.30s",club_go.name);
				else {iii--;printf(club_go.name);}
			}
			else if (strcmp(str,"!¥„¥Á!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf(bcfg.man1);
				else if (ch=='1') printf(bcfg.man2);
				else if (ch=='2') printf(bcfg.man3);
				else if (ch=='3') printf(bcfg.man4);
				else if (ch=='4') printf(bcfg.man5);
				else if (ch=='9') {if (strlen(bcfg.man1)>0) printf("¥„¥Á:%s",bcfg.man1);}
				else {iii--;printf(bcfg.man1);}
			}
			else if (strcmp(str,"!µÓ∑œ!")==0||strcmp(str,"!µÓæ∆!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%s"   ,bbs.id  );
				else if (ch=='1') printf("%-8s" ,bbs.id  );
				else if (ch=='2') printf("%s",upr(bbs.id));
				else if (ch=='3') printf("%-8s",upr(bbs.id));
				else if (ch=='4') printf("%s",lwr(bbs.id));
				else if (ch=='5') printf("%-8s",lwr(bbs.id));
				else if (ch=='8') printf("%-8s",pf.type<18?"........":bbs.id);
				else if (ch=='9') printf("%-8s",go.type=='k'&&!is_same(bcfg.man1,pf.id)?"........":bbs.id  );
				else {iii--;printf("%s",bbs.id);}
			}
			else if (strcmp(str,"!µÓ¿Ã!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%s"  ,bbs.name);
				else if (ch=='1') printf("%-8s",bbs.name);
				else if (ch=='2') printf("%-8s",bbs.name);
				else if (ch=='8') printf("%-8s",pf.type<18?"........":bbs.name);
				else if (ch=='9') printf("%-8s",go.type=='k'&&!is_same(bcfg.man1,pf.id)?"........":bbs.name);
				else {iii--;printf("%s",bbs.name);}
			}
			else if (strcmp(str,"!√—π¯!")==0) {
				ch=ss[iii++];
					 if (ch=='1') printf("%d",NOW_ALLNO);
				else if (ch=='2') printf("%d",NOW_TOPNO);
				else if (ch=='3') printf("%d",NOW_NO);
				else if (ch=='4') printf("%d",brec.allnum);
				else if (ch=='5') printf("%d",brec.newnum);
				else if (ch=='9') {
					if (list_mode) sprintf(buff,"%d/%d (√— %d∞«)",bbs.num,NOW_ALLNO,brec.allnum);
					else		   sprintf(buff,"%d/%d (√— %d∞«)",NOW_TOPNO,NOW_ALLNO,brec.allnum);
					printf("%25s",buff);
				}
			}
			else if (strcmp(str,"!√—∏ﬁ!")==0) {
				str[0]=ss[iii++];str[1]=0;n=atoi(str);
				if (n==9) {
					printf("%d",count_rec(ID_FILE,sizeof_pf));
				}
				else if (n==5) printf("%d",count_rec(GUEST_FILE,sizeof_pf));
				else {
					i1=i2=i3=i4=i5=0;
					if (n<5) {
						if ((fp2=fopen(ID_FILE,"r"))==NULL) continue;
						while(fread(&pf3,sizeof_pf,1,fp2)) {
							if (pf3.type>0) i1++;
							if (time_num(pf3.inday)==today) i2++;
							if (time_num(pf3.login)==today) i3++;
						}
						fclose(fp2);
					}
						 if (n<=1) printf("%d",i1);
					else if (n==2) printf("%d",i2);
					else if (n==3) printf("%d",i3);
					else if (n==4) printf("%d",i4);
				}
			}
			else if (strcmp(str,"!¡˝∞Ë!")==0) {
				ch=ss[iii++];
					 if (ch=='1') calc_stotal(1);
				else if (ch=='2') calc_stotal(2);
				else if (ch=='3') calc_stotal(3);
				else if (ch=='4') calc_stotal(4);
				else if (ch=='5') calc_stotal(5);
				else			  calc_stotal(0);
			}
			else if (strcmp(str,"!√—ø¯!")==0) {
				str[0]=ss[iii++];str[1]=ss[iii++];str[2]=0;n=atoi(str);
				printf("%d",check_id_count(n,today));
			}
			//¡¢≈ÎYYMMDDHH
			else if (strcmp(str,"!¡¢≈Î!")==0) {
				int yy,mm,dd,hh;
				bzero(str,10);str[0]=ss[iii++];str[1]=ss[iii++];yy=atoi(str);
				bzero(str,10);str[0]=ss[iii++];str[1]=ss[iii++];mm=atoi(str);
				bzero(str,10);str[0]=ss[iii++];str[1]=ss[iii++];dd=atoi(str);
				bzero(str,10);str[0]=ss[iii++];str[1]=ss[iii++];hh=atoi(str);
				printf("%d",calc_logcount(yy,mm,dd,hh));
			}
            else if (strcmp(str,"!√÷≥Ø!")==0) {
                int i0,i1,i2,i3;
                time_t t;
                struct tm *tp;
                time(&t);
                bzero(str,10);str[0]=ss[iii++];i0=atoi(str);
                bzero(str,10);str[0]=ss[iii++];str[1]=ss[iii++];i1=atoi(str);
                bzero(str,10);str[0]=ss[iii++];str[1]=ss[iii++];i2=atoi(str);
                     if (i0<=1) t-=i1*60*60;
                else if (i0==2) t-=i1*24*60*60;
                else {
                    if (time_num(t)%100<15) t-=i1*30*24*60*60;
                    else t-=i1*31*24*60*60;
                }
                printf("%s",datestr(i2,t));
            }
			else if (strcmp(str,"!√—µø!")==0) {
				bzero(str,20);
				str[0]=ss[iii++];str[1]=0;n=atoi(str);
				i1=i2=i3=i4=i5=0;
				sprintf(buff,"%s/system/clubid.dat",club_go.dir);
				if ((fp2=fopen(buff,"r"))==NULL) continue;
				while(fread(&cpf3,sizeof_cpf,1,fp2)) {
					if (cpf3.type>0) i1++;
					if (time_num(cpf3.inday)==today) i2++;
					if (time_num(cpf3.login)==today) i3++;
					if (cpf3.type<1) i5++;
				}
				fclose(fp2);
					 if (n<=1) printf("%d",i1);
				else if (n==2) printf("%d",i2);
				else if (n==3) printf("%d",i3);
				else if (n==4) printf("%d",i4);
				else if (n==5) printf("%d",i5);
			}
			else if (strcmp(str,"!π¯»£!")==0) {
				jpg_tag=is_jpgfile();
				set_buffnum(buff1,bbs.num,jpg_tag); 	 //buffnum∏∏µÈ±‚
				ch=ss[iii++];
					 if (ch=='0') printf("%s" ,buff1);
				else if (ch=='1') {del_space(buff1);printf("%4.4s",buff1);}
				else if (ch=='2') {del_space(buff1);printf("%6s",buff1);  }
				else if (ch=='3') {del_space(buff1);printf("%-6s",buff1); }
				else if (ch=='4') printf("%d" ,bbs.num);
				else if (ch=='5') printf("%3d",bbs.num);
				else if (ch=='6') printf("%4d",bbs.num);
				else if (ch=='7') printf("%5d",bbs.num);
				else if (ch=='8') printf("%6d",bbs.num);
				else if (ch=='9') printf("%7d",bbs.num);
				else {iii--;printf("%s",buff1);}
			}
			else if (strcmp(str,"!µÓ≥Ø!")==0||strcmp(str,"!¡∂≥Ø!")==0) {
				time_t tdate;
				tdate=strcmp(str,"!µÓ≥Ø!")==0?bbs.date:bbs.date2;
				ch=ss[iii++];
					 if (ch=='0') printf("%s",datestr( 0,tdate));
				else if (ch=='1') printf("%s",datestr( 1,tdate));
				else if (ch=='2') printf("%s",datestr( 2,tdate));
				else if (ch=='3') printf("%s",datestr( 3,tdate));
				else if (ch=='4') printf("%s",datestr( 4,tdate));
				else if (ch=='5') printf("%s",datestr( 5,tdate));
				else if (ch=='6') printf("%s",datestr( 6,tdate));
				else if (ch=='7') printf("%s",datestr(17,tdate));
				else if (ch=='8') printf("%s",datestr(24,tdate));
				else if (ch=='9') printf("%s",datestr(25,tdate));
				else if (ch=='_') {
                    bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];
                    printf("%s",datestr(atoi(str),tdate));
				}
				else {iii--;printf("%s",datestr(8,tdate));}
			}
			else if (strcmp(str,"!∏ﬁ¿œ!")==0) {
				if (bbs.filesize>0&&strlen(bbs.filename)>0) strcpy(buff,"BIN ");
				else strcpy(buff,"TXT ");
				ch=ss[iii++];
					 if (ch=='0') printf("%-4.4s",buff);
				else if (ch=='1') printf("%-4.4s",bbs.key);
				else if (ch=='2') printf("%-8.8s",bbs.key);
			}
			else if (strcmp(str,"!¡¶∏Ò!")==0) {
				ch=ss[iii++];
				if (bbs.del&&dd_mode&&bit(allmode2,1)) printf("[7mD[1;0m");
					 if (ch=='0') printf("%s",bbs.title);
				else if (ch=='1') printf("%60.60s",bbs.title);
				else if (ch=='2') printf("%-60.60s",bbs.title);
				else if (ch=='3') printf("%-55.55s",bbs.title);
				else if (ch=='4') printf("%-50.50s",bbs.title);
				else if (ch=='5') printf("%-42.42s",bbs.title);
				else if (ch=='6') printf("%-40.40s",bbs.title);
				else if (ch=='7') printf("%-35.35s",bbs.title);
				else if (ch=='8') printf("%-30.30s",bbs.title);
				else if (ch=='9') {
					bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];n=atoi(str);
					sprintf(buff,"%%-%d.%ds",n,n);
					printf(buff,bbs.title);
				}
				else {iii--;printf("%s",bbs.title);}
			}
			else if (strcmp(str,"!¡∂»∏!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d",bbs.read);
				else if (ch=='1') printf("%3d",bbs.read);
				else if (ch=='2') printf("%4d",bbs.read);
				else if (ch=='3') printf("%5d",bbs.read);
				else {iii--;printf("%d",bbs.read);}
			}
			else if (strcmp(str,"!¥ŸøÓ!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d",bbs.down);
				else if (ch=='1') printf("%3d",bbs.down);
				else if (ch=='2') printf("%4d",bbs.down);
				else if (ch=='3') printf("%5d",bbs.down);
				else if (ch=='9') printf("%-6d",bbs.down);
				else if (ch=='f') printf("%d",check_free());
				else {iii--;printf("%d",bbs.down);}
			}
			else if (strcmp(str,"!º≥¡§!")==0) {
				ch=ss[iii++];
				if (ch=='1') {
					sprintf(buff,"%s/logo/%s.ndn",club_go.dir,go.code);
					if (is_file(buff)) printf("¥ŸøÓ±›¡ˆ");
				}
			}
			else if (strcmp(str,"!∂Û¿Œ!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d",bbs.line);
				else if (ch=='1') printf("%2d",bbs.line);
				else if (ch=='2') printf("%4d",bbs.line);
				else if (ch=='3') printf("%d",bbs.line/16+1);
				else if (ch=='4') printf("%3d",bbs.line/16+1);
				else if (ch=='5') printf("%d",N_page);
				else if (ch=='6') printf("%3d",N_page);
				else if (ch=='7') printf("%d/%d",N_page,bbs.line/16+1);
				else if (ch=='8') printf("%3d/%-3d",N_page,bbs.line/16+1);
				else if (ch=='9') {
					sprintf(buff,"%d/%d",N_page,bbs.line/16+1);
					printf("%-16s",buff);
				}
				else {iii--;printf("%d",bbs.line);}
			}
			else if (strcmp(str,"!±Ê¿Ã!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d",bbs.size);
				else if (ch=='1') printf("%2d",bbs.size);
				else if (ch=='2') printf("%4d",bbs.size);
				else if (ch=='3') printf("%6d",bbs.size);
				else if (ch=='4') printf("%s",byte2str(bbs.size));
				else if (ch=='5') printf("%5s",byte2str(bbs.size));
				else if (ch=='6') printf("%6s",byte2str(bbs.size));
				else {iii--;printf("%d",bbs.size);}
			}
			else if (strcmp(str,"!»≠¿œ!")==0||strcmp(str,"!∆ƒ¿œ!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%s",bbs.filename);
				else if (ch=='1') printf("%-12.12s",bbs.filename);
				else if (ch=='2') printf("%12.12s",bbs.filename);
				else if (ch=='3') printf("%-12.12s",bbs.filename);
				else if (ch=='4') printf("%12.12s",bbs.filename);
				else if (ch=='5') printf("%-12.12s",bbs.filename);
				else if (ch=='6') printf("%12.12s",bbs.filename);
				else {iii--;printf("%s",bbs.filename);}
			}
			else if (strcmp(str,"!≈©±‚!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%s",byte2str(bbs.filesize));
				else if (ch=='1') printf("%5s",byte2str(bbs.filesize));
				else if (ch=='2') printf("%6s",byte2str(bbs.filesize));
				else if (ch=='3') printf("%d",bbs.filesize);
				else if (ch=='4') printf("%6d",bbs.filesize);
				else if (ch=='5') printf("%8d",bbs.filesize);
				else if (ch=='6') printf("%d",bbs.filesize/1024);
				else if (ch=='7') printf("%6d",bbs.filesize/1024);
				else if (ch=='8') printf("%-8d",bbs.filesize);
				else if (ch=='9') printf("%4.4s",byte2str(bbs.filesize));
				else {iii--;printf("%d",bbs.filesize);}
			}
			else if (strcmp(str,"!∫–∑˘!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%s",bbs.key);
				else if (ch=='1') printf("%-8s",bbs.key);
				else if (ch=='2') printf("%-4.4s",bbs.key);
				else if (ch=='3') printf("%8s",bbs.key);
				else if (ch=='4') printf("%4.4s",bbs.key);
				else if (ch=='5') printf("%2.2s",bbs.key);
				else if (ch=='6') printf("%s",bbs.tcode);
				else if (ch=='7') printf("%-4.4s",bbs.tcode);
				else if (ch=='8') printf("%-8s",bbs.tcode);
				else if (ch=='9') {
					if (strlen(bbs.key)>0) printf("∫–∑˘:%-8.8s",bbs.key);
					else				   printf("%13s","");
				}
				else {iii--;printf("%s",bbs.key);}
			}
			else if (strcmp(str,"!±◊∏≤!")==0) {jpg_tag=is_jpgfile();printf(jpg_tag?"¢¿":"  ");}
			else if (strcmp(str,"!√ﬂ√µ!")==0) {
				char buff2[280],buff3[280],buff4[280];
				strcpy(buff3,"");strcpy(buff4,"");
				if (bbs.gd!=' ') strcpy(buff2,gd_str(bbs.gd));      //√ﬂ√µ∫∞ªˆªÛ
				else			 strcpy(buff2,gd_str(g_char));		//√ﬂ√µ∫∞ªˆªÛ
				if (strlen(buff2)>0) {
					sprintf(buff3,"[s%s",buff2);
					strcpy(buff4,"[u");
				}
				ch=ss[iii++];
					 if (ch=='0') printf("%s",buff3);
				else if (ch=='1') printf("%s",buff4);
				else if (ch=='2') printf("%c",g_char);
				else if (ch=='3') printf("%s",gd_name(g_char));
				else if (ch=='4') printf("%-8s",gd_name(g_char));
				else if (ch=='5') printf("%-4.4s",gd_name(g_char));
				else if (ch=='6') printf("%s",buff2);
				else {iii--;printf("%c",g_char);}
			}
			else if (strcmp(str,"!¬˘º∫!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d" ,calc_ok(1));
				else if (ch=='1') printf("%d" ,calc_ok(2));
				else if (ch=='2') printf("%d" ,calc_ok(3));
				else if (ch=='3') printf("%3d",calc_ok(1));
				else if (ch=='4') printf("%3d",calc_ok(2));
				else if (ch=='5') printf("%3d",calc_ok(3));
				else if (ch=='6') {sprintf(buff,"¬˘º∫:%d ",calc_ok(1));printf(calc_ok(1)?buff:"");}
				else if (ch=='7') {sprintf(buff,"π›¥Î:%d ",calc_ok(2));printf(calc_ok(2)?buff:"");}
				else if (ch=='8') {sprintf(buff,"¿¿¥‰:%d ",calc_ok(3));printf(calc_ok(3)?buff:"");}
				else if (ch=='9') sprintf(buff,"%3d:%-3d",calc_ok(1),calc_ok(2));
				else iii--;
			}
			else if (strcmp(str,"!ªË¡¶!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%s" ,bbs.del?"[ªË¡¶]":"");
				else if (ch=='1') printf("%s" ,bbs.del?"[ªË¡¶]":"      ");
				else if (ch=='2') printf("%s" ,bbs.del?"D":"");
				else if (ch=='3') printf("%s" ,bbs.del?"D":" ");
				else if (ch=='4') printf("%s" ,bbs.del?"Del":"");
				else if (ch=='5') printf("%s" ,bbs.del?"Del":"   ");
				else iii--;
			}
			else if (strcmp(str,"!ªÛ«∞!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d" ,bcfg.uplevel);
				else if (ch=='1') printf("%d" ,bcfg.updown );
				else if (ch=='2') printf("%d" ,bcfg.upcash );
				else iii--;
			}
			else if (strcmp(str,"!∫∏¡∏!")==0) {
				ch=ss[iii++];
					 if (ch=='0') printf("%d" ,calc_ok(11));
				else if (ch=='1') printf("%d" ,calc_ok(12));
				else if (ch=='2') printf("%d" ,calc_ok(13));
				else if (ch=='3') printf("%3d",calc_ok(11));
				else if (ch=='4') printf("%3d",calc_ok(12));
				else if (ch=='5') printf("%3d",calc_ok(13));
				else if (ch=='6') {sprintf(buff,"ªË¡¶:%d ",calc_ok(11));printf(calc_ok(11)?buff:"");}
				else if (ch=='7') {sprintf(buff,"∫∏¡∏:%d ",calc_ok(12));printf(calc_ok(12)?buff:"");}
				else if (ch=='8') {sprintf(buff,"±‚±«:%d ",calc_ok(13));printf(calc_ok(13)?buff:"");}
				else if (ch=='9') sprintf(buff,"%3d:%-3d",calc_ok(11),calc_ok(12));
				else iii--;
			}
			else if (strcmp(str,"!∏Ì∑…!")==0) {
				bzero(str,10);
				for (i=0;i<8;i++) {
					if (ss[iii]<=' ') break;
					str[i]=ss[iii++];
				}
				del_space(str);del_esc3(str);
				sprintf(buff,"bin/%s.lsh",str);
				if(is_file(buff)) {
					sys_tem("%s '%s' '%s' '%s' %d '%s'",
						buff,pf.id,pf.name,dateyang(11,pf.birthday,pf.moon),pf.type,go.code);
				}
				else printf(buff);
				fflush(stdout);
			}
            else if (strcmp(str,"!√ﬂ∞‘!")==0) {
				bzero(str,10);
				for (i=0;i<8;i++) {
                    if (ss[iii]<=' ') break;
					str[i]=ss[iii++];
				}
				del_space(str);del_esc3(str);
//////////////////////////////////// ∏ﬁ¿Œ¿œ∂ß∏∏ ƒƒ∆ƒ¿œ
//                view_bbs1(str);
			}
			else if (strcmp(str,"!∞≠¡¶!")==0) {
				bzero(str,50);
				for (i=0;i<50;i++) {if (ss[iii]<' ') break;str[i]=ss[iii++];}
				strcpy(auto_cmd,str);
			}
			else if (strcmp(str,"!ø‹∫Œ!")==0) {
				char buff2[280],buff3[280],buff4[280];
				fflush(stdout);
				ch=ss[iii++];
				strcpy(buff,"");
				if (ch=='<') {
					bzero(buff,300);
					for (i=0;i<300&&ss[iii]>=' ';i++) {
						if (ss[iii]=='>'||ss[iii]=='\n') {iii++;break;}
						buff[i]=ss[iii++];
					}
				}
				if (strlen(buff)>0) {
					buff2buff1(buff,buff3,buff4);
					//¿Ã ∆ƒ¿œø° ∆˜«‘µ» ≥ªøÎ∏∏ µø¿€¿Ã ∞°¥…«œ¥Ÿ.
					if (in_file2("system/vexec.cmd",buff3)) {
						system(buff);
					}
					fflush(stdout);
				}
			}
			else if (strcmp(str,"!∑Œ∞Ì!")==0) {  //text »≠∏È ∫∏ø©¡‹
				bzero(str,40);
				for (i=0;i<40;i++) {if (ss[iii]<=' ') break;str[i]=ss[iii++];}
				sprintf(buff,"%s/logo/%s",club_go.dir,str);
				mview(Yes,str);
			}
			else if (strcmp(str,"!±§∞‘!")==0) {  //√÷±Ÿ¿⁄∑·4∞≥∏¶ ∫∏ø©¡‹
				bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];n=atoi(str);
				bzero(str,40);
				for (i=0;i<8;i++) {if (ss[iii]<=' ') break;str[i]=ss[iii++];}
				if (strlen(str)>0) make_bbs_g_buff(1,str,n,0,0);
			}
			else if (strcmp(str,"!±§≥ª!")==0) {  //√÷±Ÿ¿⁄∑·≥ªøÎ¿ª ∫∏ø©¡‹
				bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];str[2]=ss[iii++];n=atoi(str);
				bzero(str,40);
				for (i=0;i<8;i++) {if (ss[iii]<=' ') break;str[i]=ss[iii++];}
				if (strlen(str)>0) make_bbs_g_buff(2,str,n,0,0);
			}
			else if (strcmp(str,"!±§¿ß!")==0) {  //√÷±Ÿ¿⁄∑·≥ªøÎ¿ª ∫∏ø©¡‹
				int x,y,line,nn;
				bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];x=atoi(str);
				bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];y=atoi(str);
				bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];line=atoi(str);
				bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];nn=atoi(str);
				bzero(str,40);
				for (i=0;i<8;i++) {if (ss[iii]<=' ') break;str[i]=ss[iii++];}
				if (strlen(str)>0) make_bbs2_g_buff(str,nn,x,y,line);
			}
			else if (strcmp(str,"!¥∫∆ƒ!")==0) {  //text »≠∏È ∫∏ø©¡‹
				bzero(str,40);
				for (i=0;i<8;i++) {if (ss[iii]<=' ') break;str[i]=ss[iii++];}
				if (strlen(str)>0) strcpy(news_code,str);
			}
			else if (strcmp(str,"!∆ƒ∂Û!")==0) {
				for (i=0;i<2;i++) str[i]=ss[iii++];str[2]=0;i=atoi(str);
				file_line(buff,TMP_FILE2,i);
				printf(buff);
			}
			else if (strcmp(str,"!¥∫Ω∫!")==0||strcmp(str,"!≥∂µ∂!")==0) {
				bzero(str,20);
				str[0]=ss[iii++];str[1]=0;i1=atoi(str);
				str[0]=ss[iii++];str[1]=0;i2=atoi(str);
				str[0]=ss[iii++];str[1]=ss[iii++];str[2]=0;i3=atoi(str);
				str[0]=ss[iii++];str[1]=ss[iii++];str[2]=0;i4=atoi(str);
				str[0]=ss[iii++];str[1]=0;i5=atoi(str);
				view_news1(i1,i2,i3,i4,i5);
				//viewnews ¥∫Ω∫ ∏µÂ ≥Ø¬•(µŒ¿⁄) ¡Ÿºˆ(µŒ¿⁄) ¡¶∏Ò
			}
			else if (strcmp(str,"!¥∫∏!")==0) {
				bzero(str,20);
				str[0]=ss[iii++];str[1]=0;i1=atoi(str);
				str[0]=ss[iii++];str[1]=0;
				set_newsmode(i1,str);
			}
			else if (strcmp(str,"!∏æÁ!")==0) {   //»≠∏È«•Ω√πÊπ˝ºˆ¡§
				bzero(str,20);
				str[0]=ss[iii++];str[1]=ss[iii++];str[2]=0;
				bmode=atoi(str);
			}
			else if (strcmp(str,"!º≠πˆ!")==0) {
				ch=ss[iii++];
				strcpy(buff,"");
				if (ch=='0') sprintf(buff,"%s.%u",cfg.sip,cfg.nip[cfg.hostno%20]);
				if (ch=='1') sprintf(buff,"%s.%u",cfg.sip,cfg.nip[moni.hostno%20]);
				printf("%-15s",buff);
				//126.123.123.123
			}
			else if (strcmp(str,"!º≠π¯!")==0) {
				bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];str[2]=0;n=atoi(str);
				sprintf(buff,"%s.%u",cfg.sip,cfg.nip[n%20]);
				printf("%-15s",buff);
				//126.123.123.123
			}
			else if (strcmp(str,"!±‚≈∏!")==0) {   //»≠∏È«•Ω√πÊπ˝ºˆ¡§
				ch=ss[iii++];
					 if (ch=='0') printf("%s",autoup?"[¿⁄µøµÓ∑œªÛ≈¬]":"");
				else iii--;
			}
			else if (strcmp(str,"!≈ı«•!")==0) {
				bzero(str,20);
				str[0]=ss[iii++];str[1]=ss[iii++];
				i=atoi(str);
				if (i>0) printf("%3d",vote.avote[i-1]);        //Ω√∫∏±‚
				else	 printf("%3d",vote.ans);
			}
			else if (strcmp(str,"!Ω√∞£!")==0) {
				bzero(str,20);str[0]=ss[iii++];str[1]=ss[iii++];str[2]=0;n=atoi(str);
				printf(now_str(n));
			}
			else if (strcmp(str,"!±Ó¡ˆ!")==0) {
				bzero(str,20);
				for(i=0;i<14;i++) str[i]=ss[iii++];
				calc_date(str);
				//g_buff8,g_buff9,a_num[30]~a_num[33]
			}
			else if (strcmp(str,"!∏¥œ!")==0) {
				ch=tolower(ss[iii++]);
				if (ch=='0') {
					if (bit(moni.mode2,1)==0) printf("%-8.8s",moni.name);
					else					  printf("[35;1m%-8.8s[0;1m",moni.name);
				}
				else if (ch=='1') printf("%-6.6s",moni.tty);
				else if (ch=='2') {del_esc(moni.hostname);del_space(moni.hostname);printf("%-20.20s",moni.hostname);}
				else if (ch=='3') printf("%-8.8s",moni.gocode);
				else if (ch=='4') printf("%-20.20s",moni.goname);
				else if (ch=='5') printf("%-8.8s",moni.club);
				else if (ch=='6') printf("%-2d",moni.hostno);
				else if (ch=='7') printf("%s",moni.msg?"#":"|");
				else if (ch=='8') printf("%s",moni.msg?"∞≈∫Œ":"«„øÎ");
				else if (ch=='9') printf("%s",datestr(4,moni.login));
				else if (ch=='a') printf("%2d",moni.type);
				else if (ch=='b') printf("%2d",moni.mode);
				else if (ch=='c') printf("%2d",moni.stat);
				else if (ch=='d') printf("%-70.70s",moni.what);
				else if (ch=='e') printf("%-30.30s",moni.what);
				else if (ch=='f') printf("%-30.30s",moni.what);
				else if (ch=='g') printf("%s",datestr(4,moni.logout));
				else if (ch=='h') printf("%d",moni.msg);
				else if (ch=='j') printf("%6d",moni.pfnum);
				else if (ch=='k') printf("%s",datestr(43,how_old(moni.login)));
				else if (ch=='l') {
					strcpy(buff,no_spc(moni.tty));
					strchng2(buff,"tty","");
					strchng2(buff,"dev","");
					strchng2(buff,"pts","t");
					printf("%-3.3s",buff);
				}
				else if (ch=='m') {
					if (moni.mode==99) printf("¿·ºˆ");
					else if (moni.msg) printf("∞≈∫Œ");
					else if (bit(moni.mode2,3)) printf("∞‘¿”");
					else printf(awork[moni.mode>29?29:moni.mode]);
				}
				else if (ch=='n') {sprintf(buff,"%3d",how_old(moni.login)/60);printf("%3.3s",buff);}
				else if (ch=='o') printf("%-8s",moni.nick);
				else if (ch=='p') printf("%-8s",bit(cfg.usermode,7)?moni.nick:moni.name);
				else if (ch=='x') allmode2=set_bit(allmode2,1,1);
				else {
					iii--;
					if (is_same(tty,moni.tty)&&moni.hostno==cfg.hostno)
						printf("[7m%-8.8s[0;1m",moni.id);
					else
						printf("%-8.8s",moni.id);
				}
			}
			else if (strcmp(str,"!µøΩ≈!")==0) {
				bzero(str,20);
				str[0]=ss[iii++];str[1]=ss[iii++];str[2]=0;n=atoi(str);
					 if (n== 0) printf("%05d",cpf.num);
				else if (n== 1) printf("%-12s",cpf.id);
				else if (n== 2) printf("%-14s",cpf.name);
				else if (n== 3) printf("%2d",cpf.type);
				else if (n== 4) printf("%s",actype[cpf.type]);
				else if (n== 5) printf(who>=1?datestr(0,pf2.birthday):"********");
				else if (n== 6) printf("%c",pf2.sex);
				else if (n== 7) printf(who>2?pf2.id_no:"********");
				else if (n== 8) printf("%s",who>=1?pf2.pos1:"****");
				else if (n== 9) printf("%s",who>=1?pf2.addr:"****");
				else if (n==10) printf("%s",who>=1?pf2.tele:"****");
				else if (n==11) printf("%s",who>=1?pf2.offi:"****");
				else if (n==20) printf("%s",datestr(2,cpf.inday ));
				else if (n==21) printf("%s",datestr(2,cpf.login ));
				else if (n==22) printf("%s",datestr(2,cpf.logout));
				else if (n==23) printf("%-6d",cpf.all_log);
				else if (n==24) printf("%-6d",cpf.countw);
				else if (n==25) printf("%-6d",cpf.countu);
				else if (n==26) printf("%-6d",cpf.countd);
				else if (n==27) printf(datestr(3,cpf.all_time));
				else if (n==31) printf(!bit(ccfg.memo_disp,1)?ccfg.note[0]:"");
				else if (n==32) printf(!bit(ccfg.memo_disp,2)?ccfg.note[1]:"");
				else if (n==33) printf(!bit(ccfg.memo_disp,3)?ccfg.note[2]:"");
				else if (n==34) printf(!bit(ccfg.memo_disp,4)?ccfg.note[3]:"");
				else if (n==35) printf(!bit(ccfg.memo_disp,5)?ccfg.note[4]:"");
				else if (n==41) printf(!bit(ccfg.memo_disp,1)?cpf.memo1:"");
				else if (n==42) printf(!bit(ccfg.memo_disp,2)?cpf.memo2:"");
				else if (n==43) printf(!bit(ccfg.memo_disp,3)?cpf.memo3:"");
				else if (n==44) printf(!bit(ccfg.memo_disp,4)?cpf.memo4:"");
				else if (n==45) printf(!bit(ccfg.memo_disp,5)?cpf.memo5:"");
			}
			else printf(str);
		}
	}
	_quit:
	fflush(NULL);
	return (ret);
}



//»≠∏Èø° ªË¡¶µ» ∞ËΩ√π∞ ∫∏¿Ã¥¬∞°?
set_newsmode(int mode,char *str)
{
	char buff1[100];
		 if (mode==0) strcpy(buff1,"TIME LOGO");
	else if (mode==1) strcpy(buff1,"TIME NEWS");
	else if (mode==2) strcpy(buff1,"TIME HELP");
	else if (mode==3) strcpy(buff1,"TIME HELP");
	else			  strcpy(buff1,"TIME");
		 if (is_same(str,"1")) newsmode[mode]=0;
	else if (is_same(str,"0")) newsmode[mode]=1;
	else if (strlen(str)==0) printf("\n ### %s %s ###\n",buff1,newsmode[mode]?"Yes":"No");
}




check_id_count(int n,int today)
{
	FILE *fp;
	int i,anum[30];
	char filename[100];
	PF pf3;
	strcpy(filename,"tmp/idcount.num");
	for (i=0;i<30;i++) anum[i]=0;
	if (!is_old_file(filename,30*60)) {
		if ((fp=fopen(filename,"r"))==NULL) goto cont_5;
		for (i=0;i<30;i++) fread(&anum[i],sizeof(anum[i]),1,fp);
		fclose(fp);
	}
	else {
		cont_5:
		if ((fp=fopen(ID_FILE,"r"))==NULL) return 0;
		while(fread(&pf3,sizeof_pf,1,fp)) {
			if (strlen(pf3.id)>2) anum[0]++;
			if (pf3.type > 0) anum[1]++;
			if (pf3.type>=10&&pf3.type<14) anum[2]++;
			if (pf3.type>=14&&pf3.type<18) anum[3]++;
			if (pf3.type>=18) anum[4]++;
			if (pf3.type== 0&&strlen(pf3.id)>0) anum[5]++;
			if (time_num(pf3.inday)==today) anum[6]++;
			if (time_num(pf3.login)==today) anum[7]++;
			if (time_num(pf3.inday)/100==today/100) anum[8]++;
			if (time_num(pf3.login)/100==today/100) anum[9]++;

			if (pf3.warn%10>0)		  anum[10]++;	  //∞Ê∞Ì
			if (bit(pf3.paymode,1))   anum[11]++;	  //¿Ø∑·
			if (bit(pf3.paymode,2))   anum[12]++;	  //ø¨√º
			if (pf3.cash<=0)		  anum[13]++;	  //∆ƒªÍ»∏ø¯
			if (pf3.cash>99999999)	  anum[14]++;	  //¿Áπ˙»∏ø¯
			if (pf3.type<18) {
				anum[15]+=pf3.countw;
				anum[16]+=pf3.countu;
				anum[17]+=pf3.countd;
				anum[18]+=pf3.all_log;
				anum[19]+=pf3.month_log;
				anum[20]+=pf3.all_time/60;
				anum[21]+=pf3.month_time/60;
			}
			if (pf3.type > 0&&pf3.type<14) anum[25]++;
			if (pf3.type>=14			 ) anum[26]++;
		}
		fclose(fp);
		anum[29]=count_rec(GUEST_FILE,sizeof_pf);
		if ((fp=fopen(filename,"w+"))!=NULL) {
			for (i=0;i<30;i++) fwrite(&anum[i],sizeof(anum[i]),1,fp);
			fclose(fp);
		}
	}
	return (anum[n]);
}


view_text(const char *fmt,...)
{
	char buff[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	view_text_(20,buff);
}

view_text5(const char *fmt,...)
{
	char buff[500];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	view_text_(-20,buff);
}

view_text_line(char *filename,int line) {view_text_(line,filename);}

view_text_(int mode,char *filename)
{
	char c, s[10];
	int line=0;
	int tline=0;
	int ii=0,key,len,linemode=No;
	FILE *fp;

	tline=count_line(filename);
	len=file_size(filename);
	if (mode<0) {linemode=Yes;mode*=-1;}  //√‚∑¬Ω√ ¡Ÿπ¯»£µµ √‚∑¬

	re_view:
	line=0;
	if ((fp=fopen(filename,"r"))==NULL) return No;
	key=No;
	ii=0;
	if (linemode) printf("%5d ",line+1);
	while (ii++<len) {
		c=getc(fp);
		putchar(c);
		if (mode>0&&c=='\n') {
			line ++;
			if (line % mode ==0) {
				s[0]=0;
				printf(" ####(P%d/%d) ∞Ëº”[ENTER] ¡ﬂ¡ˆ(Q) ¥ŸΩ√(R) ø¨º”(S) ¥ŸøÓ(D) ¿Á√‚∑¬(A) >>",line/mode,tline/mode+1);
				lineinput(s,5);check_han(s);printf("[80D[2K");
				if (s[0]=='r'||s[0]=='R') { ii=0;line=0;rewind(fp);}                if (s[0]=='a'||s[0]=='A') { ii=0;line=0;rewind(fp);mode=99999;}
				if (is_char("PpQqXx",s[0])) {key=Yes;break;}
				if (s[0]=='S'||s[0]=='s') mode=99999;
				if ((s[0]=='D'||s[0]=='d')) {
					set_statdown(8);	   //∏ﬁºº¡ˆ ºˆΩ≈πÊ¡ˆ
					sz_file(filename);
					set_statdown(OLD_MONI_MODE);
				}
			}
			if (linemode) printf("%5d ",line+1);
		}
	}
	if (!key) {
		printf("[80D[2K #### ≥ªøÎ¿« ≥°¿‘¥œ¥Ÿ. [ENTER] ¥ŸΩ√(R) ¿Á√‚∑¬(A) >>");
		lineinput(s,5);check_han(s);
		if (s[0]=='r'||s[0]=='R') { line=0;rewind(fp);goto re_view;}
		if (s[0]=='a'||s[0]=='A') { line=0;rewind(fp);mode=99999; goto re_view;}
	}
	fclose(fp);
	return Yes;
}


view_text1(char *filename)
{
	char c;
	FILE *fp;
	if ((fp=fopen(filename,"r"))==NULL) return No;
	while (!feof(fp)&&(c=fgetc(fp))!=0&&c!=-1) putchar(c);
	fclose(fp);
	return Yes;
}

view_text2(char *filename)
{
	char buff[300];
	FILE *fp;
	int i=0;
	if ((fp=fopen(filename,"r"))==NULL) return;
	while (fgets(buff,200,fp)!=NULL) {
		printf(buff);
		if (i++%20==19) getchar();
	}
	fclose(fp);
	if (i++%20!=0) getchar();
}


view_textxy(char *filename,int x,int y)
{
	char buff[300];
	FILE *fp;

	if ((fp=fopen(filename,"r"))==NULL) return;

	while (fgets(buff,200,fp)!=NULL)
		if (buff[0]!=0) {
			xy(x,y++);
			printf("%s",buff);    //«ÿ¥Áµ«¥¬ ¡Ÿ¿ª ∫∏ø©¡‹
		}
	fclose(fp);
}



view3(char *buff)
{
	return view_(3,"",buff);
}


open_str(CROOM croom,char *buff,int roomnum)
{
		 if (strlen(croom.passwd)>0) strcpy(buff,"∫Ò∞¯∞≥");
	else if (croom.openmode==3	  ) strcpy(buff,"¡¶  «—");
	else if (croom.openmode==4	  ) strcpy(buff,"∆Ø  ºˆ");
	else if (croom.openmode==5	  ) strcpy(buff,"øÓøµ¡¯");
	else if (strlen(croom.club)>1 ) strcpy(buff,"µø»£»∏");
    else strcpy(buff,"∞¯  ∞≥");
}



make_bbs_g_buff(int mode,char *code,int nn,int x,int y)
{
	FILE *fp,*fp2;
	BBS bbs2;
	int n,i,i2,i3,count;
	char ch,ch2;
	char filename[300];
	char filename2[300];
	char buff[300];
	strcpy(g_buff1,"");strcpy(g_buff2,"");strcpy(g_buff3,"");strcpy(g_buff4,"");strcpy(g_buff5,"");
	strcpy(g_buff6,"");strcpy(g_buff7,"");strcpy(g_buff8,"");strcpy(g_buff9,"");strcpy(g_buff10,"");
	sprintf(filename,"%s/menu/%s.tit",club_go.dir,code);
	sprintf(filename2,"%s/menu/%s.txt",club_go.dir,code);
	if ((fp=fopen(filename,"r"))==NULL) return;
	if ((fp2=fopen(filename2,"r"))==NULL) return;
	n=count_frec(fp,sizeof_bbs);
	count=0;
//		  printf("%s %d %s",filename,n,bbs2.title);pressenter();
	for (i=n;i>0;i--) {
		fseek(fp,(i-1)*sizeof_bbs,SEEK_SET);
		if (!fread(&bbs2,sizeof_bbs,1,fp)) break;
		if (bbs2.del||bbs2.gd<=' ') continue;
//		  printf("%s %d %s",filename,i,bbs2.title);pressenter();
		count++;
		if (count>5) break;
		if (mode==1) {
			if (nn>0&&nn<60) bbs2.title[nn]=0;
				 if (count==1) sprintf(g_buff1,"%-60.60s",bbs2.title);
			else if (count==2) sprintf(g_buff2,"%-60.60s",bbs2.title);
			else if (count==3) sprintf(g_buff3,"%-60.60s",bbs2.title);
			else if (count==4) sprintf(g_buff4,"%-60.60s",bbs2.title);
			else if (count==5) sprintf(g_buff5,"%-60.60s",bbs2.title);
				 if (count==1) sprintf(g_buff6,"%-8.8s",bbs2.id);
			else if (count==2) sprintf(g_buff7,"%-8.8s",bbs2.id);
			else if (count==3) sprintf(g_buff8,"%-8.8s",bbs2.id);
			else if (count==4) sprintf(g_buff9,"%-8.8s",bbs2.id);
			else if (count==5) sprintf(g_buff10,"%-8.8s",bbs2.id);
		}
		else if (mode==2) {
			if (nn>0&&bbs2.size>nn) bbs2.size=nn;
			if (bbs2.size>290) bbs2.size=290;
			bzero(buff,300);
			strcpy(buff,"[s");
			ch=ch2=i3=0;
			fseek(fp2,bbs2.posi,SEEK_SET);
			for (i2=0;i2<bbs2.size;i2++) {
				ch=fgetc(fp2);
				if (ch2=='!'&&ch=='[') buff[3+i3-1]='';
				buff[3+i3++]=ch;
				ch2=ch;
			}
			strcat(buff,"[u");
				 if (count==1) strcpy(g_buff1,buff);
			else if (count==2) strcpy(g_buff2,buff);
			else if (count==3) strcpy(g_buff3,buff);
			else if (count==4) strcpy(g_buff4,buff);
			else if (count==5) strcpy(g_buff5,buff);
				 if (count==1) strcpy(g_buff6 ,datestr(17,bbs2.date));
			else if (count==2) strcpy(g_buff7 ,datestr(17,bbs2.date));
			else if (count==3) strcpy(g_buff8 ,datestr(17,bbs2.date));
			else if (count==4) strcpy(g_buff9 ,datestr(17,bbs2.date));
			else if (count==5) strcpy(g_buff10,datestr(17,bbs2.date));
		}
	}
	fclose(fp);
	fclose(fp2);
}


make_bbs2_g_buff(char *code,int nn,int x,int y,int line)
{
	FILE *fp,*fp2;
	BBS bbs2;
	int n,i,i2,i3,count;
	char ch,ch2;
	char filename[300];
	char filename2[300];
	char buff[300];
	strcpy(g_buff1,"");strcpy(g_buff2,"");strcpy(g_buff3,"");strcpy(g_buff4,"");strcpy(g_buff5,"");
	strcpy(g_buff6,"");strcpy(g_buff7,"");strcpy(g_buff8,"");strcpy(g_buff9,"");strcpy(g_buff10,"");
	sprintf(filename,"%s/menu/%s.tit",club_go.dir,code);
	sprintf(filename2,"%s/menu/%s.txt",club_go.dir,code);
	if ((fp=fopen(filename,"r"))==NULL) return;
	if ((fp2=fopen(filename2,"r"))==NULL) return;
	n=count_frec(fp,sizeof_bbs);
	count=0;
	for (i=n;i>0;i--) {
		fseek(fp,(i-1)*sizeof_bbs,SEEK_SET);
		if (!fread(&bbs2,sizeof_bbs,1,fp)) break;
		if (bbs2.del||bbs2.gd<=' ') continue;
		count++;
		if (count<nn) continue;
		bzero(buff,300);
		ch=ch2=i3=0;
		fseek(fp2,bbs2.posi,SEEK_SET);
		xy(x,y+i3);
		for (i2=0;i2<bbs2.size&&i3<nn;i2++) {
			ch=fgetc(fp2);
			if (ch=='\n') {i3++;xy(x,y+i3);}
			else printf("%c",ch);
		}
		strcpy(g_buff1,bbs.title);
		strcpy(g_buff2,bbs.id);
		strcpy(g_buff3,bbs.name);
		strcpy(g_buff6 ,datestr(1,bbs2.date));
		strcpy(g_buff7 ,datestr(2,bbs2.date));
		strcpy(g_buff8 ,datestr(3,bbs2.date));
		strcpy(g_buff9 ,datestr(4,bbs2.date));
		strcpy(g_buff10,datestr(17,bbs2.date));
		break;
	}
	fclose(fp);
	fclose(fp2);
}




next_level(int type)  //»∏ø¯¿« µÓ±ﬁ,¿œ¿œªÁøÎ∞°¥…Ω√∞£¿ª »Æ¿Œ«—¥Ÿ.
{
	FILE *fp;
	int i,n,ret=0;
	if (type<13&&(fp=fopen("system/pftype.dat","r"))!=NULL) {
		for (i=0;i<type+1;i++) {
			fread(&n,sizeof(n),1,fp);	   //º’¥‘,øÓøµ¡¯¿∫ ¿˚øÎæ»µ 
			if (n>0) ret=n;
		}
		fclose(fp);
	}
	if (ret<=0) ret=0;
	return ret;
}


char *gd_name(int mark)
{
	int i2;
	static char buff[30];
	strcpy(buff,"");
	for (i2=0;i2<10;i2++) {
		if ((int)acolor[i2].mark==(int)mark) {
			strcpy(buff,acolor[i2].name);
			break;
		}
	}
	return (buff);
}


is_jpgfile()
{
	char buff[300];
	if (stristr(bbs.filename,".jpg")) {
		if (is_typepds) sprintf(buff,"%s/%s",go.dir,bbs.filename);
		else			sprintf(buff,"%s/bbsdata/bbs_jpg/%s/%s",club_go.dir,go.code,bbs.filename);
		if (is_file(buff)) return Yes;
	}
	return No;
}




char *disp_istat(int mode,int num)
{
	static char out[40];
	char buff[200];
	strcpy(out,"");
	file_line(buff,"system/idstat.dat",num+mode*100);
	buff[30]=0;
	strcpy(out,buff);
	return (out);
}


/* ∑Œ∞Ì»≠¿œ √‚∑¬ */
//»≠¿œ¿÷¿∏∏È 1 æ¯¿∏∏È 0 return
view2(char *filename)
{
	int ret=No;
	ret=view(filename);
	if (ret) pressenter();
	return ret;
}


/* ∏ﬁ¥∫¿« ∑Œ∞Ì»≠¿œ √‚∑¬ */
//»≠¿œ¿÷¿∏∏È 1 æ¯¿∏∏È 0 return
view_logo(char *go_code)
{
	char filename[80];
	int ret=No;
	sprintf(filename,"%s/logo/%s.mnu",club_go.dir,go_code);
	ret=view(filename);
	return ret;
}


//»≠¿œ¿÷¿∏∏È 1 æ¯¿∏∏È 0 return
logo(char *filename,int stop)
{
	int ret=No;
	char buff[80];
	strcpy(buff,filename);
	if (empty(buff)) sprintf(buff,"logo/%s.mnu",go.code);
	ret=view(buff);
	if(ret&&stop&&!nostop) getchar3();
	return (ret);
}


set_buffnum(char *buffnum,int n,int jpg_tag)
{
	bzero(buffnum,10);
	sprintf(buffnum,"%6d ",n%1000000);
	if (bbs.gd<32) bbs.gd=' ';
	if		(bbs.del)		 buffnum[0]='D';       //ªË¡¶¿⁄∑·
	else if (bbs.gd>32) 	 buffnum[0]=bbs.gd;    //√ﬂ√µ¿⁄∑·
	else if (strlen(bbs.passwd)>0&&!bit(bcfg.mode,31)) buffnum[0]='?';       //æœ»£¿÷¿Ω
	if (jpg_tag&&buffnum[0]==' ') {
			 if (is_typewtx&&bit(cfg.jpgmode,1)) ;
		else if (is_typebbs&&bit(cfg.jpgmode,2)) ;
		else if (is_typepds&&bit(cfg.jpgmode,3)) ;
		else if (cfg.jpgchar[0]<=' ');
		else {
			buffnum[0]=cfg.jpgchar[0];	//¢¿
			if (cfg.jpgchar[1]>' ') buffnum[1]=cfg.jpgchar[1];       //¢¿
		}
	}
}

//mode=1 : OK  2:NO  3:¿¿¥‰
int calc_ok(int mode)
{
		 if (mode==1)  return bbs.ok%1000;
	else if (mode==2)  return (bbs.ok/1000)%1000;
	else if (mode==3)  return (bbs.ok/1000000)%100;
	else if (mode==11) return bbs.down%1000;
	else if (mode==12) return (bbs.down/1000)%1000;
	else if (mode==13) return bbs.down/1000000;
	return 0;
}


view_news1(int poem,int mode,int ndate,int line,int mode2)
{
	FILE *fp,*fp2;
	BBS bbs2;
	char title[100][71];
	char filename1[100];
	char filename2[100];
	char buff2[30];
	int n,i,n2,i2,i3;
	time_t t;

	strcpy(buff2,poem?"poem":"news");
	sprintf(filename1,"menu/%s_%d.tit",buff2,mode);
	sprintf(filename2,"menu/%s_%d.txt",buff2,mode);
	if (!is_file(filename1)) return;

	if ((fp=fopen(filename1,"r"))==NULL) return;
	fseekend(fp);
	n=ftell(fp)/sizeof_bbs;
	if (n<1) {
		fclose(fp);
		return;
	}
	if (ndate<=0) { 										 //≥Ø¬• π´Ω√
		i3=0;
		while (i3++<30) {									 //30π›∫π
			if (n<100) n2=rand() % n;						 //¥∫Ω∫ π¯»£ ¿”¿« º±≈√
			else	   n2=n-(rand()%100);
			if (n2<0||n2>=n) continue;
			fseek(fp,n2*sizeof_bbs,SEEK_SET);
			if (!fread(&bbs2,sizeof_bbs,1,fp)||bbs2.del) continue;
			all_trim(bbs2.title);
			if (line==0) printf("%s",bbs2.title);            //¡¶∏Ò√‚∑¬
			else		 view_news2(mode2,bbs2.posi,bbs2.size,line,filename2,bbs2.title);	 //≥ªøÎ√‚∑¬
			break;
		}
	}
	else {
		//Ω√¿€ ≥Ø¬• √£±‚ -> n2
		n2=n;
		fseek(fp,(n2-1)*sizeof_bbs,SEEK_SET);
		time(&t);
		while (n2-->=0) {
			fseek(fp,(n2-1)*sizeof_bbs,SEEK_SET);
			if (!fread(&bbs2,sizeof_bbs,1,fp)) break;
			if ((t-bbs2.date)>ndate*24*60*60) break;
		}
		if (n2>=0) {
			i3=0;
			while (i3++<30) {									 //30π›∫π
				if (n2<0) n2=0;
				if (n<=n2) n2=n-1;
				i2=n-rand()%(n-n2);
				if (i2<0||i2>n) i2=n;
				fseek(fp,(i2-1)*sizeof_bbs,SEEK_SET);
				if (!fread(&bbs2,sizeof_bbs,1,fp)) continue;
				if (bbs2.del) continue;
				if (line==0) printf("%s",bbs2.title);            //¡¶∏Ò√‚∑¬
				else		 view_news2(mode2,bbs2.posi,bbs2.size,line,filename2,bbs2.title);	 //≥ªøÎ√‚∑¬
				break;
			}
		}
	}
	fclose(fp);
}


view_news2(int mode2,int posi,int size,int line,char *filename2,char *title)
{
	FILE *fp1;
	FILE *fp2;
	int count,i;
	char bufftit[300];
	char TMP_FILE5[200];
	char c;

	sprintf(TMP_FILE5,"tmp/tmp5%d%s",cfg.hostno,no_spc(tty)); //»£Ω∫∆Æπ¯»£∫∞¿”Ω√»≠¿œ
	fp1=fopen(TMP_FILE5,"w+");
	fp2=fopen(filename2,"r+");
	if (fp1==NULL||fp2==NULL) return;
	strcpy(bufftit,title);
	if (mode2==2) sprintf(bufftit,"°º %s °Ω",title);
	if (mode2==3) sprintf(bufftit,"### %s ###",title);
	if (mode2!=5) {
		if (mode2==4) fprintf(fp1,"%s\n",bufftit);
		else		  fprintf(fp1,"%s\n",center_line(bufftit));
	}
	fseek(fp2,posi,SEEK_SET);
	count=0;
	for (i=0;i<size;i++) {
		if((c = fgetc(fp2)) == '\n') count++;
		if(count>line) break;
		fputc(c,fp1);
	}
	fclose(fp1);
	fclose(fp2);
	view(TMP_FILE5);
	unlink(TMP_FILE5);
}


//!±Ó¡ˆ!20000101000000	   -> g_buff9ø° ¿˙¿Â«‘
calc_date(char *datestr)
{
	int i,n,n1,n2,n3,n4,n5;
	char buff[100];
	char buff2[100];
	time_t t,t2;

	strcpy(g_buff8,"");
	strcpy(g_buff9,"");
	for (i=30;i<40;i++) a_num[i]=0;
	time(&t);
	t2=str2date4(datestr);
	n=t2-t;
	if (now_year()<2000) {
		n1=n/(24*60*60);
		n2=(n/(60*60))%24;
		n3=(n/60)%60;
		n4=n%60;
		a_num[30]=n1;
		a_num[31]=n2;
		a_num[32]=n3;
		a_num[33]=n4;
		a_num[34]=n/(60*60);  //√—Ω√∞£
		a_num[35]=n/60; 	  //√—∫–
		if (n1>0) {sprintf(buff,"%d ¿œ",n1);  strcat(g_buff9,buff);}
		if (n2>0) {sprintf(buff," %d Ω√∞£",n2);strcat(g_buff9,buff);}
		if (n3>0) {sprintf(buff," %d ∫–",n3);  strcat(g_buff9,buff);}
		if (n4>0) {sprintf(buff," %d √ ",n4);  strcat(g_buff9,buff);}
		sprintf(g_buff8,"%d ¿œ %d Ω√∞£ %d ∫– %d √ ",n1,n2,n3,n4);
	}
}


