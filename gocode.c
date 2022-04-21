////////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
//°¡±â ÄÚµå Ã³¸®
////////////////////////////////////////////////////////////////////////

#include "bbs.h"

//´ÙÀ½¸Þ´º·Î ÀÌµ¿
go_next(int mode)
{
    return go_back_next(1);
}
go_back(int mode)
{
    return go_back_next(2);
}


//´ÙÀ½¸Þ´º·Î ÀÌµ¿
go_back_next(int direct)
{
	FILE *fp;
	int n,i;
	char deep;
	int found;
	GO oldgo,tgo,tgo2;
	int ndeep;
	found=0;

	re_print=Yes;
    view_pos=0;
	if (list_mode) {
		if (direct==1) go_next_list();
		else		   go_back_list();
		return Yes;
	}

    set_nowmenu();
    if ((fp=fopen(NOW_MENU,"r"))==NULL) return No;
	oldgo=go;
	//NEXT
	if (direct==1) {
		ndeep=go.deep;
        fseek(fp,go.rec*sizeof_go,SEEK_SET);
		found=No;
		while(fread(&tgo,sizeof_go,1,fp)) {
            if (tgo.deep<ndeep) break;
            else if (tgo.deep>ndeep||strlen(tgo.code)<1||tgo.type<=' ') ;
			else if (!is_same(oldgo.code,tgo.code)) {  //°°À»¶§
				go=tgo;
				found=Yes;
				break;
			}
		}
        //°Ë»ö½ÇÆÐÀÏ¶§ »óÀ§¿¡¼­ Ã³À½ ¸Þ´º¸¦ °Ë»ö
		if (!found) {
            n=oldgo.rec;
            while(1) {
                fseek(fp,n*sizeof_go,SEEK_SET);
                if (!fread(&tgo,sizeof_go,1,fp)) break;
				if (tgo.deep<ndeep) break;
                else if (is_char(tgo.wtimes,'@'));
                else if (tgo.deep>ndeep||tgo.code[0]==0||tgo.type<=' ') ;
				else {
					go=tgo;
					found=Yes;
				}
                n--;
			}
		}
	}
	else {		  //BACK
		ndeep=go.deep;
		n=go.rec-1;
		if (n<0) n=0;
		found=No;
		while(1) {
            fseek(fp,n*sizeof_go,SEEK_SET);
			if (!fread(&tgo,sizeof_go,1,fp)) break;
			n--;
            if (tgo.deep<ndeep) break;
            else if (is_char(tgo.wtimes,'@')) continue;
            else if (tgo.deep>ndeep||tgo.code[0]==0||tgo.type<=' ') continue;
			else if (!is_same(oldgo.code,tgo.code)) {  //°°À»¶§
				go=tgo;
				found=Yes;
				break;
			}
		}
		//°Ë»ö½ÇÆÐÀÏ¶§
		if (!found) {
			n=go.rec+1;
            fseek(fp,n*sizeof_go,SEEK_SET);
			tgo2=go;
			while(fread(&tgo,sizeof_go,1,fp)) {
				n++;
				if (tgo.deep<ndeep) break;
                else if (tgo.deep>ndeep||tgo.code[0]==0||tgo.type<=' ');
				else {
					tgo2=tgo;
					found=Yes;
				}
			}
			if (found) {
				go=tgo2;
				found=Yes;
			}
		}
	}
    fclose(fp);

    read_bcfg();
    if (found&&go.type=='s') {
        club_go=go;
		club_mode=Yes;
		re_club=Yes;
	}
    else if (!found||check_type('j')||check_type('i')) {
        go=oldgo;
        found=No;
        read_bcfg();
    }
    if (is_typepds) {
        del_space(go.dir);
        if (strlen(go.dir)<1) {
            if (club_mode) sprintf(go.dir,"%s/pds/%s",club_go.dir,go.code);
            else           sprintf(go.dir,"pds/%s",go.code);
            check_dir(go.dir);
        }
    }
    if (found) up_level_cash();
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	return (found);
}


prev()
{
    go_prev();
    while (is_char("rejnp",go.type)) go_prev();
}


//pÃ³¸®
go_prev()
{
	//¹øÈ£º¸±â ¸ðµåÀÏ¶§
    if (list_mode) {
        list_mode=No;
        re_print=Yes;
        return;
    }
	p_command=Yes;
	bbs_ltmode=No;
    if (club_mode&&go.rec==0) go_prev1();  //µ¿È£È¸ÀÇ »óÀ§¸Þ´º¿¡¼­ p¸¦ »ç¿ëÇÔ
    else                      go_prev2();     //ÀÏ¹Ý»óÅÂ¿¡¼­ P »ç¿ë
	return;
}



//µ¿È£È¸¿¡¼­ ºüÁ®³ª¿È
go_prev1()
{
	FILE *fp;
    int i,n,found=No;
    GO tgo,tgo2,oldclub_go;
    oldclub_go=club_go;
    go_top(0);  //¸ÞÀÎ¸Þ´ºÀÇ ÃÖ»ó´Ü¸Þ´º¿¡¼­ ½ÃÀÛ
    tgo2=go;
    if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
        i=oldclub_go.rec;
        while(i>=0) {
            fseek(fp,i*sizeof_go,SEEK_SET);
            if (!fread(&tgo,sizeof_go,1,fp)) break;
            if (tgo.deep<oldclub_go.deep&&tgo.type=='m') {
                tgo2=tgo;
                found=Yes;
                break;
            }
            i--;
        }
        fclose(fp);
    }
    if (found) go=tgo2;
    else go_top(1);
    read_bcfg();
    return;
}


//pÃ³¸®
go_prev2()
{
	FILE *fp;
    int i,n,found=No;
	GO tgo,tgo2;

    set_nowmenu();
	go_prev2_:
	if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
		i=go.rec-1;
		go_top(1);
		tgo2=go;
		while(i>=0) {
            fseek(fp,i*sizeof_go,SEEK_SET);
			if (!fread(&tgo,sizeof_go,1,fp)) break;
            else if (tgo.deep<go.deep&&tgo.type=='m') {
                found=Yes;
				tgo2=tgo;
				break;
			}
			i--;
		}
		fclose(fp);
	}
    if (found) go=tgo2;
    else go_top(1);

	if (go.type=='r'||go.type=='t') goto go_prev2_;
	read_bcfg();
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	list_mode=No;
	return;
}



//¸Þ´º¸¦ °¡±âÄÚµå·Î ¼±ÅÃÇÔ
go_code(char *buff)
{
	FILE *fp;
	int n,n1,n2,ns,deep,i,i2;
	int found=No;
    char buffgo[80];
	char buff1[80];
	GO find_arr_tgo[100];
    GO tgo,oldgo,oldclub_go;
    int oldclub_mode;
    int main_find=No;

	oldgo=go;
    oldclub_go=club_go;
    oldclub_mode=club_mode;

	strcpy(buffgo,buff);
    if (go.type!='t'&&is_same(go.code,buffgo)) goto go_code_ok;
    if (is_same(buffgo,"t")||is_same(buffgo,"top")||is_same(buffgo,"main")) {
        go_top(0);
        goto go_code_ok;
    }
	go_top(1);
	if (club_mode) {
        //µ¿È£È¸¸ÞÀÎ¸Þ´º¿¡¼­ µ¿ÀÏÇÑ ¸Þ´º¸¦ ¸ÕÀú Ã£´Â´Ù.
        if (is_goindex(NOW_MENU,buffgo)&&(fp=fopen(NOW_MENU,"r"))!=NULL) {
			while(fread(&tgo,sizeof_go,1,fp)) {
				if (is_same(tgo.code,buffgo)&&tgo.type!='t'&&tgo.type!=' ') {
					go_top(0);
					go=tgo;
					fclose(fp);
					goto go_code_ok;
				}
			}
			fclose(fp);
		}
	}
    //¸ÞÀÎ¸Þ´º¿¡¼­ Ã£±â
    go_top(0);
    found=No;
    if (is_goindex(NOW_MENU,buffgo)) {
        if ((fp=fopen(NOW_MENU,"r"))==NULL) return No;
        while(fread(&tgo,sizeof_go,1,fp)) {
            //µ¿ÀÏÇÑ ¸Þ´º¸¦ ¸ÕÀú Ã£´Â´Ù.
            if (is_same(tgo.code,buffgo)&&tgo.type!='t'&&tgo.type!=' ') {
                go=tgo;
                fclose(fp);
                if (oldclub_mode) main_find=Yes;
                found=Yes;
                goto go_code_ok;
            }
        }
        fclose(fp);
    }

    //TOP ¿¡¼­ À¯»çÇÑ ¸Þ´º¸¦  Ã£´Â´Ù.
    for (i=0;i<50;i++) strcpy(find_arr_tgo[i].code,"");
    n1=0;
    if ((fp=fopen(NOW_MENU,"r"))==NULL) goto _fail;
    while(strlen(buffgo)>1&&n1<100&&fread(&tgo,sizeof_go,1,fp)) {
        if (tgo.type!='t'&&tgo.type>=' '&&strlen(tgo.code)>2) {      //Æ÷ÇÔ¿©ºÎ?
            if (stristr(tgo.code,buffgo)||
                (!bit(cfg.gomode,2)&&strlen(tgo.name)>0&&stristr(tgo.name,buffgo))) {
                found=No;
                for (i=0;!found&&i<50;i++) {
                    if (is_same(find_arr_tgo[i].code,tgo.code)) {
                        found=Yes;
                        break;
                    }
                }
                if (!found) find_arr_tgo[n1++]=tgo;
            }
        }
        if (n1>0) main_find=Yes;
    }
    fclose(fp);

    if (n1==0) goto _fail;
    if (n1==1) {      //ÀÚ·á°¡ 1°³¹Û¿¡ ¾øÀ»¶§
        go_top(0);
        go=find_arr_tgo[0];
        found=Yes;
        goto go_code_ok;
    }

	//À¯»çÀÚ·á¸¦ Ã£¾ÒÀ»¶§ ¼±ÅÃÇÏ±â
	if (n1>1) {  //n1:Ã£Àº¼ýÀÚ ÃÖ°í100°³±îÁö..
		ns=0;
		while (1) {
			clrscr();
			printf("                        ¦¬¦¬¦¬¦¬¦¬[[7m °¡±âÄÚµå [m]¦¬¦¬¦¬¦¬¦¬\n");
			printfL2();
			i=0;
            for (n=ns;n<n1&&i<20&&n<50;n++,i++) {
				if (find_arr_tgo[n].code[0]!=0) {
					printf("  %2d. %-9s %-22.22s",n+1,find_arr_tgo[n].code,find_arr_tgo[n].name);
					if (n%2==1) printf("\n");
				}
			}
			xy(0,18);
			printfL2();
			printf("¹øÈ£¸¦ ¼±ÅÃÇÏ¼¼¿ä (0:Ãë¼Ò ¹øÈ£:¼±ÅÃ) >>");
			lineinput(buff1,4); 	   //¸í·É¾î ÀÔ·Â
			check_cmd(buff1);	   //¸í·É¾î·Î ºÐÇØ
			n2=mmn[0];
			if (n2==0||**mmc=='p') break;
			if (n2>0&&n2<=n1) {
				go_top(0);
				go=find_arr_tgo[n2-1];
				goto go_code_ok;
			}
			if (ns>n1) ns=0;
			ns+=20;
		}
	}

	//ÃÖÁ¾ÀûÀ¸·Î ¸øÃ£¾ÒÀ»¶§ ¿ø·¡ ÀÚ·á¸¦ º¸¿©ÁÜ
    _fail:
    go=oldgo;
    club_mode=oldclub_mode;
    club_go=oldclub_go;
    set_nowmenu();
	re_print=Yes;
	list_mode=No;
    re_logo=No;
    re_club=No;
	return No;

	go_code_ok:
    club_mode=oldclub_mode;
    club_go=oldclub_go;
    if (main_find) {  //µ¿È£È¸°Ë»ö½ÇÆÐ->¸Þ´º¿¡¼­ Ã£À½
        club_mode=No;
        tgo=go;
        go_top(0);
        go=tgo;
		re_club=Yes;
    }
	read_bcfg();
    all_trim(go.dir);
	if (go.type=='s') {    //¼±ÅÃÇÑ ¸Þ´º°¡ µ¿È£È¸ÀÇ °æ¿ì
        club_mode=No;strcpy(club_go.code,"main");
        check_another_rec();        //µ¿È£È¸À§Ä¡º¯°æ½Ã ±â·Ï
        club_go=go;
		club_mode=Yes;
		re_club=Yes;
	}
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	list_mode=No;
    if (is_typepds) {
        del_space(go.dir);
        if (strlen(go.dir)<1) {
            if (club_mode) sprintf(go.dir,"%s/pds/%s",club_go.dir,go.code);
            else           sprintf(go.dir,"pds/%s",go.code);
            check_dir(go.dir);
        }
    }
    return (Yes);
}


is_gocode(char *tcode)
{
	GO oldgo;
	int ret;
	oldgo=go;
    ret=find_go(tcode);
	go=oldgo;
	return ret;
}


//¸Þ´º¸¦ ÃÊ±â ¸Þ´ºÄÚµå·Î ¼±ÅÃÇÔ
go_top(int mode)
{
	strcpy(go.code,"top");
    strcpy(go.name,"¸ÞÀÎ¸Þ´º");
    if (club_mode) strcpy(go.name,club_go.name);
    newsmode[0]=newsmode[1]=newsmode[2]=newsmode[3]=0;
	go.rec=0;
	go.type='m';
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	list_mode=No;
	bbs_ltmode=No;
	if (mode==0) {
		now_type=1;
		club_mode=No;
		club_go.rec=0;
		strcpy(club_go.dir,".");
		strcpy(club_go.code,"main");
        strcpy(club_go.name,"¸ÞÀÎ¸Þ´º");
	}
    set_nowmenu();
    read_bcfg();
}


gg_code(int mode)
{
	char buff[10];
	int i;
	GG gg[10];
	FILE *fp;

	for (i=0;i<10;i++) {
		strcpy(gg[i].code,"");
		strcpy(gg[i].name,"");
	}

	fp=fopen("system/gg.dat","r+");
	if (fp!=NULL) {
		for (i=0;i<10;i++) fread(&gg[i],sizeof_gg,1,fp) ;
		fclose(fp);
	}

	re_print=Yes;
	if (mode>10) return;
	if (mode==0) {
		printf("\n");
		printfL2();
		printf("     %s ÃßÃµ µ¿È£È¸/°Ô½ÃÆÇ\n",cfg.hostname);
		printfL2();
		for (i=0;i<10;i++)
			printf("       [  G%-2d ] %-20s ( %-8s )\n",i+1,gg[i].name,gg[i].code);
		printfL2();
		printf("     ¼±ÅÃ  Ãë¼Ò(Q)  >>");
		lineinput(buff,4);
		mode=atoi(buff);
		if (mode==0) {
			buff[0]=' ';
			mode=atoi(buff);
		}
		if (mode<1||mode>10) return;
	}
	if (gg[mode-1].code[0]==0) return;
	go_top(0);
	go_code(gg[mode-1].code);
	check_smenu("");
}


mm_code(int mode)
{
	int i;
    char mm[31][250];
	char filename[250];
    char mtitle[200];
	FILE *fp;

    for (i=0;i<30;i++) strcpy(mm[i],"");

    sprintf(filename,"%s/logo/%s.dim",club_go.dir,go.code);
    if (bbs_man(pf.id)&&mode==0) xedit(filename);
    if ((fp=fopen(filename,"r"))!=NULL) {
        for(i=0;i<30;i++) {
            fgets(mm[i],200,fp);del_enter(mm[i]);
        }
		fclose(fp);
        if (mode>0&&mode<30&&mm[mode][0]!=0) strcpy(auto_cmd,mm[mode]);
	}
}


up_level_cash()
{
    if (bcfg.idlevel==0&&bcfg.idcash==0) return;
    if (!read_pf4(pf.id)) return;
    pf2.level+=bcfg.idlevel;
    pf2.cash +=bcfg.idcash;
    save_pf2(pf2.id);
}
