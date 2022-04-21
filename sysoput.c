
//µ¿È£È¸ »õ·Î ¸¸µé±â
club_make(char *now_dir)
{
	char buff[200];
	char buff2[200];
	int i,n;
	i=0;
	n=0;
	// ±âº» µð·ºÅä¸® ºÎÅÍ ¸¸µé±â
	while (i<=(int)strlen(now_dir)) {
		if (i==(int)strlen(now_dir)||(i>0&&now_dir[i]=='/')) {
			buff2[n]=0;
			if (!is_file(buff2)) {
				sprintf(buff,"\n %s µð·ºÅä¸®¸¦ ¸¸µé±î¿ä? (¸¸µë:Y) >>",buff2);
                if (!yesno2(buff,Yes)) return;
				mkdir3(buff2);
			}
		}
		buff2[n]=now_dir[i];
		n++;
		i++;
	}
    check_dir("%s/system",now_dir);
    check_dir("%s/menu"  ,now_dir);
    check_dir("%s/logo"  ,now_dir);
    check_dir("%s/pds"   ,now_dir);
    check_dir("%s/bbsdata"  ,now_dir);
    check_dir("%s/bbsdata/iprec"  ,now_dir);
    check_dir("%s/bbsdata/menurec",now_dir);
    check_dir("%s/bbsdata/menucfg",now_dir);
    check_dir("%s/user"     ,now_dir);
    check_dir("%s/menustru" ,now_dir);
    check_dir("%s/frm"      ,now_dir);
    sys_tem("touch %s/system/clubcfg.dat",now_dir);
    sys_tem("touch %s/system/clubid.dat",now_dir);
    if (is_file("system/default/clubmenu.dat")) {
        sys_tem("cp -f system/default/clubmenu.dat %s/menustru",now_dir);
    }
    sys_tem("touch %s/menustru/menu.dat",now_dir);
    mess("%s ÀÇ µð·ºÅä¸®°¡ ¸¸µé¾î Á³½À´Ï´Ù.",now_dir);
}


disp_set_stat()        //ÇöÀç »óÅÂ º¸¿©ÁÖ±â
{
	FILE *fp;
	int n,x,i,i2;
	time_t tnow;
	PF pf3;
	struct tm *tp;
    char count_file[100],buff[200];

    for(i=0;i<20;i++) ast[i]=0;
	time(&tnow);   //ÇöÀç½Ã°£
	tp = localtime(&tnow); //12311231
	if ( (tnow-lastnow)>30 ) {	//30ÃÊ °æ°ú ¿©ºÎ
        ast[1]=count_moni();
		stat_pf();
        ast[2]=total;
        ast[3]=0;
		fp=fopen(GUEST_FILE,"r+");
		if (fp!=NULL) {
			fseekend(fp);
            ast[3]=ftell(fp)/sizeof_pf;
			fclose(fp);
		}
		if ( (tnow-lastnow2)>90 ) {  //30ÃÊ °æ°ú ¿©ºÎ
            ast[4]=ast[5]=ast[6]=ast[7]=ast[8]=ast[9]=ast[10]=ast[11]=ast[14]=0;
            if ((fp=fopen(ID_FILE,"r"))!=NULL) {
				while (fread(&pf3,sizeof_pf,1,fp)) {
                    if (pf3.type==0) ast[4]++;
					else {
                        if (pf3.warn%10>0)        ast[5]++;      //°æ°í
                        if (pf3.type>=14)         ast[6]++;
                        if (pf3.type>=18)         ast[14]++;
                        if (bit(pf3.paymode,1))   ast[7]++;        //À¯·á
                        if (bit(pf3.paymode,2))   ast[8]++;        //¿¬Ã¼
                        if (pf3.cash<0)           ast[10]++;      //ÆÄ»êÈ¸¿ø
                        if (pf3.cash>99999999)    ast[11]++;      //Àç¹úÈ¸¿ø
					}
				}
				fclose(fp);
			}
			lastnow2=tnow;
		}
		lastnow=tnow;
	}

	strcpy(ss_1,"");strcpy(ss_2,"");strcpy(ss_3,"");
	strcpy(ss_1,bit(cfg.guestmode,15)?"ÀÚµ¿°¡ÀÔ":"¼öµ¿°¡ÀÔ");
	x=58;n=2;
    xy(x,n++);printf("ÇöÀçÁ¢¼ÓÀÚ:%6d¸í ",ast[1]);
    xy(x,n++);printf("ÃÑÈ¸¿ø¼ö  :%6d¸í ",ast[2]);
    xy(x,n++);printf("°¡ÀÔ´ë±âÀÚ:%6d¸í ",ast[3]);
    xy(x,n++);printf("»èÁ¦È¸¿ø  :%6d¸í ",ast[4]);
    xy(x,n++);printf("°æ°íÈ¸¿ø  :%6d¸í ",ast[5]);
    xy(x,n++);printf("¿î¿µÁø¼ö  :%6d¸í ",ast[6]);
    xy(x,n++);printf("À¯·áÈ¸¿ø  :%6d¸í ",ast[7]);
    xy(x,n++);printf("¿¬Ã¼È¸¿ø  :%6d¸í ",ast[8]);
    xy(x,n++);printf("IP  È¸¿ø  :%6d¸í ",ast[9]);
    xy(x,n++);printf("ÆÄ»êÈ¸¿ø  :%6d¸í ",ast[10]);
    xy(x,n++);printf("Àç¹úÈ¸¿ø  :%6d¸í ",ast[11]);
    xy(x,n++);printf("ÃÑ°Ô½ÃÆÇ¼ö:%6d   ",ast[12]);
    xy(x,n++);printf("ÃÑÀÚ·á½Ç¼ö:%6d   ",ast[13]);
    xy(x,n++);printf("µî±Þ18ÀÌ»ó:%6d¸í ",ast[14]);
	xy(x,n++);printf("½Å±Ô°¡ÀÔ  :%s",ss_1);
	xy(x,n++);printf("D,W Á¦ÇÑ  :%s",ss_2);
	xy(x,n++);printf("ID  ÀÔ·Â  :%s",bit(cfg.guestmode,20)?"ÀüºÎ¼Õ´Ô":"Á¤»óÀÔ·Â");
	xy(x,n++);printf("¼Õ´Ô  ID  :%s",bit(cfg.guestmode,18)?"ÀÏ·Ã¹øÈ£":cfg.guestid);
}

//Á¢¼ÓÀÎ¿ø¼ö °è»ê
count_moni()
{
    int count=0,n;
    FILE *fp;
    char filename[100];
    for (n=0;n<20;n++) {
        sprintf(filename,"user/moni/moni%d.dat",n);
        if ((fp=fopen(filename, "r"))==NULL) continue;
        while (fread(&moni,sizeof_moni,1,fp)) {
            if (moni.mode<1||strlen(moni.id)<3||strlen(moni.tty)<1) continue;
            count++;
        }
        fclose(fp);
    }
	return count;
}


new_cfg()
{
	strcpy(cfg.hostname,"xhost");
	strcpy(cfg.sysop,"sysop");
	cfg.waitkey=5;
	cfg.levelmax=5000;
	cfg.cashmax=900000000;
	cfg.cashmaxday=900000000;
	cfg.downfree=14;		 // ¹«Á¦ÇÑ ´Ù¿î°¡´É Å¸ÀÔ
	cfg.cashfree=16;		 // ¹«Á¦ÇÑ ´Ù¿î°¡´É Å¸ÀÔ
	cfg.billfree=16;		 // ¹«Á¦ÇÑ ´Ù¿î°¡´É Å¸ÀÔ
	cfg.helplevel=100 ; 	 // Àá±ñ µµ¿ò¸»À» º¸¿©ÁÖ´Â ·¹º§ÀÓ
	cfg.helptype =1   ; 	 // Àá±ñ µµ¿ò¸»À» º¸¿©ÁÖ´Â ·¹º§ÀÓ
	cfg.helplog  =10  ; 	 // Àá±ñ µµ¿ò¸»À» º¸¿©ÁÖ´Â ·¹º§ÀÓ
	strcpy(cfg.guestid,"guest");         // ¼Õ´Ô Á¢¼Ó ¾ÆÀÌµð
	cfg.guestlevel=10;		 // ÃÊ±â ·¹º§
	cfg.guesttime=100;		 // ÃÊ±â ½Ã°£ (ºÐ)
	cfg.guestdown=500;		 // ÃÊ±â ´Ù¿î¿ë·®
	cfg.guestcash=100;		 // ÃÊ±â ±Ý¾×
	cfg.chatwarn=100;		 // Ã¼ÆÃ°æ°í½Ã°£ (ºÐ)
	cfg.chattype=16;		 // ºñ°ø°³ ´ëÈ­ ÁøÀÔ µî±Þ
}


//È¸¿øÀÚ·á È®ÀÎÇÏ±â
stat_pf()
{
	FILE *fp;
	if ((fp=fopen(ID_FILE,"r+"))!=NULL) {
		fseekend(fp);
		total=ftell(fp)/sizeof_pf;
		fclose (fp);
	}
}


disp_del_pf()
{
	FILE *fp;
	char buff1[100];
	char buff2[100];

	printf("\n »èÁ¦Ã³¸® È¸¿ø È®ÀÎ\n");
	printfL2();
	fp=fopen(ID_FILE,"r+");
	while (!feof(fp)) {
		if (!fread(&pf,sizeof_pf,1,fp)) break;
		if (pf.type==0||pf.id[0]==0||pf.warn%10==9) {
            printf("%5d.%-8s(%-8s) %2d %5d %2d %s %s\n",pf.num+1,pf.id,pf.name,pf.type,pf.level,pf.warn%10,
			datestr(2,pf.inday),datestr(2,pf.logout));
		}
	}
	printfL2();
	if (fp!=NULL) fclose (fp);
	pressenter();
}


pack_del_pf(int mode)
{
	FILE *fp,*fp2;
	char buff1[100],buff2[100];
	int n,allyes,i;
	time_t tdate;

	allyes=No;
	fp=fopen(ID_FILE,"r+");
	if (mode==1||mode==2) {
		n=0;
		while (1) {
			fseek(fp,n*sizeof_pf,SEEK_SET);
            if (n<0||!fread(&pf,sizeof_pf,1,fp)) break;
			if (pf.type==0&&pf.id[0]!=0) {
                printf("%5d.%-8s(%-8s) %2d %5d »èÁ¦Ã³¸®(Y/n/q/a) >>",pf.num+1,pf.id,pf.name,pf.type,pf.level);
				if (allyes) i=1;
				else		i=yesno("",Yes);
				if (i==-2) {allyes=Yes;i=Yes;}
				if (i==-1) break;
				if (i) {
					fseek(fp,n*sizeof_pf,SEEK_SET);
					strcpy(pf.id,"");strcpy(pf.name,"");
//                    if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
                    if (n<0||!gwrite(205,pf,fp)) break;
					if (mode==2&&pf.id_no[0]!=0) {
						printf(" --> ÁÖ¹Îµî·Ï »ç¿ë±ÝÁö Ã³¸®");
						if ((fp2=fopen("system/bad_idno.txt","a+"))!=NULL) {
							fprintf(fp2,"%s\n",pf.id_no);
							fclose (fp2);
						}
					}
					else printf(" --> »èÁ¦");
				}
				else printf(" --> º¸·ù");
				printf("\n");
			}
			n++;
		}
	}
	else if (mode==3) {
		printf(" ÃÖÁ¾Á¢¼Ó»èÁ¦Ã³¸®±âÁØ³¯Â¥  (¿¹:96/12/31 ->96/12/31ÀÌÀü ÀüºÎ »èÁ¦)>>");
		lineinput(buff2,8);
		tdate=str2date(buff2);
		if (tdate>0) {
			n=0;
			while (1) {
				fseek(fp,n*sizeof_pf,SEEK_SET);
                if (n<0||!fread(&pf,sizeof_pf,1,fp)) break;
				if (pf.logout<tdate&&pf.login<tdate&&pf.inday<tdate&&pf.type>0&&pf.id[0]!=0) {
                    printf("%5d.%-8s(%-8s) %2d %5d ÃÖÁ¾%s »èÁ¦Ã³¸®(Y/n/q/a) >>",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(2,pf.logout));
					if (allyes) i=1;
					else		i=yesno("",Yes);
					if (i==-2) {allyes=Yes;i=Yes;}
					if (i==-1) break;
                    if (i&&n>=0) {
						fseek(fp,n*sizeof_pf,SEEK_SET);
						pf.type=0;
//                        if (!fwrite(&pf,sizeof_pf,1,fp)) break;
                        if (!gwrite(206,pf,fp)) break;
						printf(" ->»èÁ¦");
					}
					else printf(" ->º¸·ù");
					printf("\n");
				}
				n++;
			}
		}
	}
	else if (mode==4) {
		n=0;
        while (1) {
			fseek(fp,n*sizeof_pf,SEEK_SET);
            if (n<0||!fread(&pf,sizeof_pf,1,fp)) break;
			if (pf.warn%10==9&&pf.type>0&&pf.id[0]!=0) {
                printf("%5d.%-8s(%-8s) %2d %5d °æ°í9 ÃÖÁ¾%s »èÁ¦Ã³¸®(Y/n/q/a) >>",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(2,buff2,pf.logout));
				if (allyes) i=1;
				else		i=yesno("",Yes);
				if (i==-2) {allyes=Yes;i=Yes;}
				if (i==-1) break;
                if (i&&n>=0) {
					fseek(fp,n*sizeof_pf,SEEK_SET);
					pf.type=0;
//                    if (!fwrite(&pf,sizeof_pf,1,fp)) break;
                    if (!gwrite(207,pf,fp)) break;
					printf(" ->»èÁ¦");
				}
				else printf(" ->º¸·ù");
				printf("\n");
			}
			n++;
		}
	}
	else if (mode==5) {
		printf("\n¾ÆÁ÷ Áö¿øµÇÁö ¾Ê½À´Ï´Ù.\n");
	}
	if (fp!=NULL) fclose(fp);
	printf("\n .......... ÀÚ·á ÀçÁ¤¸® ¿Ï·á.............\n");
	pressenter();
}


renum_pf()
{
	FILE *fp;
	char buff2[100];
	int n;
	int old;

	printf("\n .......... ¹øÈ£ ÀçÁ¤¸® Áß ..............\n");
	fp=fopen(ID_FILE,"r+");
	n=0;
	while (1) {
		fseek(fp,n*sizeof_pf,SEEK_SET);
		if (!fread(&pf,sizeof_pf,1,fp)) break;
		old=pf.num;
        pf.num=n;
		if (old!=pf.num) {
			printf("### %-8s(%-8s) ¹øÈ£ %6d ->%6d ·Î º¯°æ \n",pf.id,pf.name,old,pf.num);
			fseek(fp,n*sizeof_pf,SEEK_SET);
//          if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
            if (n<0||!gwrite(208,pf,fp)) break;
		}
		strcpy(buff2,pf.id);del_space(buff2);del_esc(buff2);
		if (!is_same(pf.id,buff2)) {
			printf("### %-8s -> %-8s(%-8s) ·Î º¯°æ  \n",pf.id,buff2,pf.name);
			strcpy(pf.id,buff2);
			fseek(fp,n*sizeof_pf,SEEK_SET);
//            if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
            if (n<0||!gwrite(209,pf,fp)) break;
		}
		strcpy(buff2,pf.name);del_space(buff2);del_esc(buff2);
		if (!is_same(pf.name,buff2)) {
			printf("### %-8s -> %-8s(%-8s) ·Î º¯°æ  \n",pf.name,buff2,pf.name);
			strcpy(pf.name,buff2);
			fseek(fp,n*sizeof_pf,SEEK_SET);
//            if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
            if (n<0||!gwrite(209,pf,fp)) break;
		}
		n++;
	}
	if (fp!=NULL) fclose(fp);
	printf("\n .......... ¹øÈ£ ÀçÁ¤¸® ¿Ï·á.............\n");
	pressenter();
}


renum_proccode()
{
	FILE *fp;
	char buff2[100];
	int n,n2;
	int old;

	printf("\n");
	printf(" ÀÌ ±â´ÉÀº È¸¿øÀÇ ¹ß¼Û/Ã³¸®¿ë ³»ºÎ ½Äº°ÄÚµå¸¦ ÀçÁ¤¸® ÇÏ´Â ±â´ÉÀÔ´Ï´Ù.\n");
	printf(" ±âÁ¸ÀÇ Á¤º¸¸¦ ¹«½ÃÇÏ°í »õ·Î ÁöÁ¤µÇ¹Ç·Î ÁÖÀÇÇÏ¿©¾ß ÇÕ´Ï´Ù.\n");
	printf(" »õ·Î ÀçÁ¤¸®¸¦ ÇÏ½Ã°Ú½À´Ï±î? (y/N) >>");
	if (yesno("",No)==No) return;
	printf("\n ½Äº°¹øÈ£ ºÎ¿©½Ã ½ÃÀÛ¹øÈ£¸¦ ÁöÁ¤ÇÏ¼¼¿ä. (1~100000)>>");
	n2=numinput();
	if (n2<0) n2=1;
	if ((fp=fopen(ID_FILE,"r+"))!=NULL) {
		n=0;
		while (1) {
			fseek(fp,n*sizeof_pf,SEEK_SET);
			if (!fread(&pf,sizeof_pf,1,fp)) break;
			pf.idnum=n2;
			fseek(fp,n*sizeof_pf,SEEK_SET);
//            if (n<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
            if (n<0||!gwrite(210,pf,fp)) break;
			n++;
			n2++;
		}
		fclose(fp);
	}
	printf("\n .......... ½Äº°¹øÈ£ ÀçÁ¤¸® ¿Ï·á.............\n");
	pressenter();
}




add_new_pf()
{
	char buff[100];
	char buff1[100];
	char buff2[100];
	char buff3[100];
	char buff4[100];
	char buff5[100];
	char buff6[100];
	int n,i;
	FILE *fp;
	while(1) {
		printf("\n");
		printfL2();
		printf("       ½Å±Ô È¸¿ø ¼öµ¿À¸·Î ¸¸µé±â    (Enter:Á¾°á)\n");
		printfL2();
        printf("¸¸µé È¸¿ø ¾ÆÀÌµð¸¦   ÀÔ·ÂÇÏ¼¼¿ä     >>");
        lineinput(buff1,8);printf("\n");
        if (strlen(buff1)<2||is_quit(buff1)) break;
		if (read_pf2(buff1)) {
			printf(" ÇØ´ç È¸¿øÀÇ ¾ÆÀÌµð°¡ ÀÌ¹Ì Á¸ÀçÇÕ´Ï´Ù.  %s(%s)\n",pf2.id,pf2.name);
			continue;
		}
		printf("¸¸µé È¸¿ø ¼º¸íÀ»  ÀÔ·ÂÇÏ¼¼¿ä  (8ÀÚ) >>");lineinput(buff2,8);printf("\n");
        if (is_quit(buff2)) break;
		if (buff2[0]==0) strcpy(buff2,buff1);
		printf("¸¸µé È¸¿ø µî±ÞÀ»  ÀÔ·ÂÇÏ¼¼¿ä  (0-17)>>");lineinput(buff3,2);printf("\n");
        if (is_quit(buff3)) break;
		if (buff3[0]==0) strcpy(buff3,"1");
		printf("¸¸µé È¸¿ø ·¹º§À»  ÀÔ·ÂÇÏ¼¼¿ä(0-5000)>>");lineinput(buff4,6);printf("\n");
        if (is_quit(buff4)) break;
		if (buff4[0]==0) strcpy(buff4,"100");
		printf("¸¸µé È¸¿ø ´Ù¿î¿ë·®À» ÀÔ·ÂÇÏ¼¼¿ä (MB)>>");lineinput(buff5,6);printf("\n");
        if (is_quit(buff5)) break;
		if (buff5[0]==0) strcpy(buff5,"5");
		printf("¸¸µé È¸¿ø CASH¸¦  ÀÔ·ÂÇÏ¼¼¿ä(³É)    >>");lineinput(buff6,9);printf("\n");
        if (is_quit(buff6)) break;
		if (buff6[0]==0) strcpy(buff6,"5000");
		strcpy(pf2.pos1 ,"");
		strcpy(pf2.addr ,"");
		strcpy(pf2.tele ,"");
		strcpy(pf2.offi ,"");
		strcpy(pf2.gofirst,"");
		strcpy(pf2.id_no,"");
		pf2.birthday=0;
		strcpy(pf2.mailid,"");
		strcpy(pf2.memo1,"");
		strcpy(pf2.memo2,"");
        strcpy(pf2.user1,"");
        strcpy(pf2.user2,"");
        strcpy(pf2.user3,"");
        strcpy(pf2.user4,"");
		strcpy(pf2.email,"");
		pf2.num=0;
		pf2.myflag=0xffffffff;
		pf2.sex='m';
		pf2.job= 0;
		time(&pf2.inday);
		time(&pf2.login);
		time(&pf2.logout);
		pf2.countw=0;
		pf2.all_time=0;
		pf2.all_log=0;
		pf2.month_time=0;
		pf2.month_log=0;
		pf2.level=10;
		strcpy(pf2.id,buff1);
		del_space(pf2.id);del_esc(pf2.id);strlwr(pf2.id);strcpy(pf2.name,buff2);
		strcpy(pf2.nick,buff2);
		pf2.type=atoi(buff3);
		pf2.level=atoi(buff4);
		pf2.down=atoi(buff5)*1000000;
		pf2.cash=atoi(buff6)*1000000;
		strcpy(pf2.passwd,pf2.id);
        if (pf2.type>17||pf2.type<1) pf2.type=1;
        view_pfw2(pf2,who);
		xy(1,20);
		if (yesno(" ½ÅÀÔÈ¸¿ø °¡ÀÔÃ³¸®¸¦ ÇÒ±î¿ä? (Y/n)",Yes)) {
			fp=fopen(ID_FILE,"r+");
			if(fp!=NULL) {
				fseekend(fp);
				n=ftell(fp)/sizeof_pf; //ÇöÀç ·¹ÄÚµå¼ö
                if (n>=0) {
                    pf2.num=n;               //¸¶Áö¸·¹øÈ£¿¡ Ãß°¡ÇÔ
                    fseek(fp,n*sizeof_pf,SEEK_SET);
//                    fwrite(&pf2,sizeof_pf,1,fp);
                    gwrite(211,pf2,fp);
                }
				if (fp!=NULL) fclose(fp);
				sprintf(buff,"%s(%s) È¸¿øÀÌ ÀúÀåµÇ¾ú½À´Ï´Ù.",pf2.id,pf2.name);
				mess(buff);
			}
		}
	}
    if (yesno("ÀÎµ¦½º ÀçÁ¤¸®¸¦ ÇÒ±î¿ä? (Y/n)>>",Yes)) make_ind(0,"make_ind_all");
}

id_num_edit()
{
	int n,n2;
	char buff[200];
	FILE *fp;

	fp=fopen(ID_FILE,"r+");
	fseekend(fp);
	n2=ftell(fp)/sizeof_pf; //ÇöÀç ·¹ÄÚµå¼ö
	fclose(fp);
	printf("\n ¼öÁ¤ÇÒ È¸¿ø¹øÈ£¸¦ ÀÔ·ÂÇÏ¼¼¿ä. (1-%d) >>",n2);
	lineinput(buff,8);printf("\n");
	n=atoi(buff);
	if (n<1||n>n2) return;
    n--;
    if (n<0) n=0;
	fp=fopen(ID_FILE,"r+");
	fseek(fp,n*sizeof_pf,SEEK_SET);
	fread(&pf2,sizeof_pf,1,fp);
	printf(" È¸¿ø¹øÈ£:%5d  ID:%-8s  ¼º¸í:%-8s  º°¸í:%-8s  µî±Þ:%2d  ·¹º§:%5d\n"
    ,pf2.num+1,pf2.id,pf2.name,pf2.nick,pf2.type,pf2.level);
	printf("# È¸¿ø º¯°æ ID  :(%8s) >>",pf2.id);
	get_str(pf2.id,8);printf("  ->%s\n",pf2.id);
	printf("# È¸¿ø º¯°æ ¼º¸í:(%8s) >>",pf2.name);
	get_str(pf2.name,8);printf("  ->%s\n",pf2.name);
	fseek(fp,n*sizeof_pf,SEEK_SET);
    pf2.num=n;
//    fwrite(&pf2,sizeof_pf,1,fp);
    gwrite(212,pf2,fp);
    fclose(fp);
    make_ind(0,"make_ind_all");
}


check_pf(int mode,int who)
{
	int i=0;
	FILE *fp;
	char buff[100];
	int found=No;
	PF pf;

	printfL2();
	fp=fopen(ID_FILE,"r+");
	while (fp!=NULL&&fread(&pf,sizeof_pf,1,fp)>0) {
		if ( (mode==1&&pf.type==0)||(mode==2&&pf.type>=16)) {
			printf(" %-8s(%-8s)%2d ",pf.id,pf.name,pf.type);
			if (i++%3==2) printf("\n");
			found=Yes;
		}
	}
	fclose(fp);
	if (!found) i=0;
	printf("\n  ÃÑ %d ¸íÀÌ °Ë»öµÇ¾ú½À´Ï´Ù.\n",i);
	getchar();
}






sys_back()
{
	struct tm *tp;
	time_t today;
	int now_day;
	char filename1[100];
	char filename2[100];
	char buff[100];
	struct stat stbuf;
	time(&today);	//¿À´ÃÀÇ ³¯Â¥ ÁöÁ¤
	tp = localtime(&today);
	now_day=tp->tm_mday;

    sprintf(filename1,"sys_back/id%d.tgz",now_day%10);
    sprintf(filename2,"sys_back/cfg%d.tgz",now_day%10);
	//¸¶Áö¸·À¸·Î  µ¥ÀÌÅ¸¸¦ ¼öÁ¤ÇÑ½Ã°£
	if (!is_file(filename1)||is_old_file(filename1,24*60*60)) {
		job_backup();
	}
}




job_backup()
{
	char filename1[300];
	char filename2[300];
	char filename3[300];
	char buff[300];
    check_dir("sys_back");
    sprintf(filename1,"sys_back/id%d.tgz",now_day());
    sprintf(filename2,"sys_back/cfg%d.tgz",now_day());
	printf("\n\n");
	printf(" ID  -> %s ¿¡ ¹é¾÷ÇÕ´Ï´Ù.\n",filename1);
    printf(" CFG -> %s ¿¡ ¹é¾÷ÇÕ´Ï´Ù.\n",filename2);
    sprintf(buff,"tar cfvz %s id/id.dat & > /dev/NULL",filename1);
    system(buff);
    sprintf(buff,"tar cfvz %s system/bbscfg.dat system/syscmd.dat & > /dev/NULL",filename3);
    system(buff);
}





//ÀüÈ¸¿ø ¼¼ÆÃ
all_set_level()
{
	int i,n,n2,nn;
	char buff[200];
	char buff1[200];
	char buff2[200];
	FILE *fp;
	while (1) {
		cls();
		printf(" ###  ÀüÃ¼ È¸¿ø ¼¼ÆÃ\n");
        printf("  1.  ÀüÃ¼ È¸¿ø ÀüÃ¼ »ç¿ë½Ã°£ 0        ¼¼ÆÃ         \n" );
        printf("  2.  ÀüÃ¼ È¸¿ø ±Ý¿ù »ç¿ë½Ã°£ 0        ¼¼ÆÃ         \n" );
		printf("  3.  ÀüÃ¼ È¸¿ø ÀüÃ¼ Á¢¼ÓÈ½¼ö 0        ¼¼ÆÃ         \n" );
        printf("  4.  ÀüÃ¼ È¸¿ø ±Ý¿ù Á¢¼ÓÈ½¼ö 0        ¼¼ÆÃ         \n" );
        printf("  5.  ÀüÃ¼ È¸¿ø ´Ù¿î·® Á¶Á¤                         \n" );
		printfL2();
		n=numselect();
        if (n<=0)  break;
        else if (n==1||n==2||n==3||n==4)  {
            printf("\n Áö±ÝºÎÅÍ ÀüÃ¼È¸¿ø¼³Á¤À» ÃÊ±âÈ­ÇÕ´Ï´Ù.\n");
            if (n==1) printf("ÀüÃ¼»ç¿ë½Ã°£");
            if (n==2) printf("±Ý¿ù»ç¿ë½Ã°£");
            if (n==3) printf("ÀüÃ¼Á¢¼ÓÈ½¼ö");
            if (n==4) printf("±Ý¿ùÁ¢¼ÓÈ½¼ö");
            if (!yesno(" ÃÊ±âÈ­ ¼¼ÆÃÀ» ÇÒ±î¿ä?(y/N)>>",No)) continue;
			fp=fopen(ID_FILE,"r+");
			i=0;
			printf("\n");
			while (1) {
				if (!fread(&pf,sizeof_pf,1,fp)) break;
                printf("%-8s (%-8s)  ",pf.id,pf.name);
                if (n==1) {
                    printf("%8d -> 0\n",pf.all_time/60);
                    pf.all_time=0;
                }
                else if (n==2) {
                    printf("%8d -> 0\n",pf.month_time/60);
                    pf.month_time=0;
                }
                else if (n==3) {
                    printf("%8d -> 0\n",pf.all_log);
                    pf.all_log=0;
                }
                else if (n==4) {
                    printf("%8d -> 0\n",pf.month_log);
                    pf.month_log=0;
                }
                fseek(fp,i*sizeof_pf,SEEK_SET);
//                if (i<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
                if (i<0||!gwrite(212,pf,fp)) break;
				i++;
			}
			if (fp!=NULL) fclose (fp);
			pressenter();
        }
        if (n==5)  {
			xy(1,20);
			printf("ÀüÃ¼È¸¿ø Áõ°¡/°¨¼Ò ÇÒ ´Ù¿î·®(KB) >>");
			lineinput(buff,8);check_han(buff);printf("\n");
			n2=atoi(buff);
			if (n2==0) continue;
			xy(1,21);
			printf("    ÀüÃ¼È¸¿øÀÇ ´Ù¿î·®À» %d KB¸¦ Áõ°¡/°¨¼ÒÇÒ±î¿ä? (Y/n)>>",n2);
			if (yesno("",Yes)==No) continue;
			fp=fopen(ID_FILE,"r+");
			i=0;
			printf("\n");
			while (1) {
				fseek(fp,i*sizeof_pf,SEEK_SET);
                if (i<0||!fread(&pf,sizeof_pf,1,fp)) break;
				printf("%-8s (%-8s)  %8d ->",pf.id,pf.name,pf.down/1000);
				pf.down+=n2*1000;
				if (pf.down<0) pf.down=0;
				printf("%8d KB\n",pf.down/1000);
				fseek(fp,i*sizeof_pf,SEEK_SET);
//                if (i<0||!fwrite(&pf,sizeof_pf,1,fp)) break;
                if (i<0||!gwrite(212,pf,fp)) break;
				i++;
			}
			if (fp!=NULL) fclose (fp);
			pressenter();
		}
	}
}

//ÀüÈ¸¿ø ¼¼ÆÃ
recover()
{
	int i,n,n2,nn,count;
	FILE *fp;
	printf("\n    »èÁ¦µÈ ÀüÃ¼È¸¿øÀ» º¹±¸ Ã³¸® ÇÒ±î¿ä? (Y/n)>>");
	if (yesno("",No)) {
		c_lock(1,ID_FILE);
		fp=fopen(ID_FILE,"r+");
		i=0;
		count=0;
		printf("\n");
		while (1) {
			fseek(fp,i*sizeof_pf,SEEK_SET);
            if (i<0||!fread(&pf,sizeof_pf,1,fp)) break;
			if (pf.type==0&&strlen(pf.id)>0) {
				printf("%-8s (%-8s)  º¹±¸Ã³¸®\n",pf.id,pf.name);
				fseek(fp,i*sizeof_pf,SEEK_SET);
				pf.type=1;
                pf.num=i;
				fwrite(&pf,sizeof_pf,1,fp);
                gwrite(212,pf,fp);
				count++;
			}
			i++;
		}
        fclose (fp);
		c_lock(0,ID_FILE);
		printf(" ÃÑ %d¸íÀÇ º¹±¸ ÀÛ¾÷ÀÌ ³¡³µ½À´Ï´Ù. \n",count);
		pressenter();
	}
}


save_cfg()
{
	FILE *fp;
	del_clear(cfg.guestid);del_clear(cfg.sysop);
    if (strlen(cfg.jpgsz)==0) strcpy(cfg.jpgsz,"7\n8[5;5H");
    if (strlen(cfg.sz)==0)    strcpy(cfg.sz,"sz");
    if (strlen(cfg.rz)==0)    strcpy(cfg.rz,"rz");
	if (strlen(cfg.guestid)<3) strcpy(cfg.guestid,"guest");
	if (strlen(cfg.sysop  )<3) strcpy(cfg.sysop  ,"guest");
    if (is_file("bbscfg.dat")) fp=fopen("bbscfg.dat","w+");
    else fp=fopen(BBS_CFG,"w+");
    if (fp!=NULL) {
        fwrite(&cfg,sizeof_cfg,1,fp);
		fclose (fp);
		touch_file(BBS_CFG);
	}
}
