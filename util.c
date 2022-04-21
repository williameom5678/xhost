////////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
// ³»¿ë : À¯Æ¿¸®Æ¼
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
#include "chat.h"
#include "vote.h"
#include <unistd.h>    //µð·ºÅä¸® È­ÀÏ¸í º¯È¯ÇÔ¼ö¶§¹®¿¡ ÇÊ¿ä
#include <dirent.h>    //µð·ºÅä¸® È­ÀÏ¸í º¯È¯ÇÔ¼ö¶§¹®¿¡ ÇÊ¿ä
#include <fcntl.h>
#include "utilbase.c"  //±âº»°ü·Ã À¯Æ¿
#include "utildate.c"  //³¯Â¥°ü·Ã À¯Æ¿
#include "utilfile.c"  //È­ÀÏ°ü·Ã À¯Æ¿
#include "utilview.c"  //È­ÀÏ°ü·Ã À¯Æ¿
#include "utilupdn.c"  //È­ÀÏ°ü·Ã À¯Æ¿


read_cfg()
{
	FILE *fp;
    int ret=No,i;
	char buff[200];
    if ((fp=fopen("bbscfg.dat","r"))==NULL) fp=fopen(BBS_CFG,"r");
    if (fp!=NULL) {
		if (fread(&cfg,sizeof_cfg,1,fp)) ret=Yes;
		fclose (fp);
	}
	if (file_line(buff,"recdir.txt",1)&&strlen(buff)>3) xstrncpy(REC_DIR,buff,20);
	else strcpy(REC_DIR,"user/rec");

    for (i=0;i<5;i++) {
        if (SERVER_PORT[i]<5000) SERVER_PORT[i]=8000+cfg.hostno;
    }
    strcpy(buff,"/etc/hostno.txt");
    if (!is_file(buff)) strcpy(buff,"./hostno.txt");
    if ((fp=fopen(buff,"r"))!=NULL) {
        i=0;
        while (fgets(buff,20,fp)!=NULL) {
			del_enter(buff);
            if (strlen(buff)>0) {
                i++;
                if (i==1) cfg.hostno=atoi(buff);
                else if (i>1&&i<7) SERVER_PORT[i-2]=atoi(buff);
            }
		}
        fclose(fp);
    }

	if (cfg.count_server<1) cfg.count_server=1; //Á¢¼Ó¼­¹ö¼ö
	cfg.hostno%=20;
	if (cfg.count_server<cfg.hostno) cfg.count_server=cfg.hostno;
    if (!bit(cfg.mprocmode,0)) cfg.count_server=1; //Á¢¼Ó¼­¹ö¼ö
    if (is_file("tag.lo1"))   allmode=set_bit(allmode,10,1);
    if (is_file("tag.lo2"))   allmode=set_bit(allmode,11,1);
    if (is_file("tag.lo3"))   allmode=set_bit(allmode,12,1);
	return ret;
}


read_bcfg()
{
	char bcfgfilename[300];
	char brecfilename[300];
	FILE *fp;
	int ret=No;

	bcfg.bform=bcfg.cmd_x=bcfg.cmd_y=bcfg.dn_limit=bcfg.dn_mult_limit		   =0;
	bcfg.level=bcfg.level2=bcfg.level3=bcfg.li_w							   =0;
	bcfg.rdown=bcfg.rdown2=bcfg.up_limit=bcfg.updown=bcfg.wdown=bcfg.wdown2    =0;
	bcfg.bdntype=bcfg.billtype=bcfg.cashtype=bcfg.dhour1=bcfg.dhour2		   =0;
	bcfg.disp_x=bcfg.disp_y=bcfg.drate=bcfg.dratet=bcfg.dtype=bcfg.fdown	   =0;
	bcfg.fread=bcfg.headnum=bcfg.iptype=bcfg.itype=bcfg.jpg_x=bcfg.jpg_y	   =0;
	bcfg.list_x=bcfg.list_y=bcfg.listline=bcfg.maxline=bcfg.pos_y1=bcfg.pos_y2 =0;
	bcfg.rtype=bcfg.uhour1=bcfg.uhour2=bcfg.urate=bcfg.uratet=bcfg.wartype	   =0;
	bcfg.wtype=bcfg.automode=bcfg.bill1=bcfg.bill2=bcfg.downmode=bcfg.dtime    =0;
	bcfg.form=bcfg.itime=bcfg.jpgmode=bcfg.limitsize=bcfg.mode=bcfg.modelist   =0;
	bcfg.modeok=bcfg.outday=bcfg.price1=bcfg.price2=bcfg.qmode=bcfg.recmode    =0;
	bcfg.rtime=bcfg.runday=bcfg.upcash=bcfg.uplevel=bcfg.wmode=bcfg.wtime	   =0;

	strcpy(bcfg.write_id,"");
	strcpy(bcfg.man1,"");strcpy(bcfg.man2,"");strcpy(bcfg.man3,"");strcpy(bcfg.man4,"");strcpy(bcfg.man5,"");
	strcpy(bcfg.passsys,"");strcpy(bcfg.passin,"");strcpy(bcfg.inbbs,"");strcpy(bcfg.outbbs,"");

	sprintf(bcfgfilename,"%s/menucfg/%s.cfg",club_go.dir,go.code);
	if (!is_file(bcfgfilename)) {
		sprintf(bcfgfilename,"%s/menucfg/def_%s.cfg",club_go.dir,bbs_tail());
		if (!is_file(bcfgfilename)) {
			sprintf(bcfgfilename,"menucfg/def_%s.cfg",bbs_tail());
		}
	}
	ret=No;
	if ((fp=fopen(bcfgfilename,"r+"))!=NULL) {
		fread(&bcfg,sizeof_bcfg,1,fp);
		fclose(fp);
		ret=Yes;
	}
	sprintf(brecfilename,"%s/menurec/%s.bre",club_go.dir,go.code);
	if (!is_file(brecfilename)) {
		sprintf(brecfilename,"%s/menucfg/def_%s.bre",club_go.dir,bbs_tail());
		if (!is_file(brecfilename)) {
			sprintf(brecfilename,"menucfg/def_%s.bre",bbs_tail());
		}
	}
	if ((fp=fopen(brecfilename,"r+"))!=NULL) {
		fread(&brec,sizeof_brec,1,fp);
		fclose(fp);
	}
	return ret;
}

save_bcfg()
{
	FILE *fp;
	char filename[200];
	sprintf(filename,"%s/menucfg/%s.cfg",club_go.dir,go.code);
	if (strlen(go.code)>0&&(fp=fopen(filename,"w+"))!=NULL) {
		fwrite(&bcfg,sizeof_bcfg,1,fp);
		fclose(fp);
	}
}


read_ccfg()
{
    char buff[100];
	FILE *fp;
	//µ¿È£È¸ È¯°æÀÐ±â
	if (!club_mode) return No;
	sprintf(buff,"%s/system/clubcfg.dat",club_go.dir);
	if ((fp=fopen(buff,"r+"))==NULL) return No;
	fread(&ccfg,sizeof_ccfg,1,fp) ;
	fclose(fp);
	return Yes;
}


read_ajob() 	 //Á÷¾÷À» ÀÐ¾î¿È
{
	FILE *fp1;
	int i;
	for (i=1;i<19;i++) strcpy(ajob[i],"");
	strcpy(ajob[0],"........");
	if ((fp1=fopen("system/ajob.dat","r"))!=NULL) {
		for (i=1;i<19;i++)
			fread(&ajob[i],sizeof(ajob[i]),1,fp1);
		fclose (fp1);
	}
}


read_atype()	 //µî±ÞÀÇ ÀÌ¸§À» ÀÐ¾î¿È  ->atype
{
	int i;
	for (i=0;i<20;i++) strcpy(atype[i],"");
	read_atype_(0);
}


read_actype()	  //µ¿È£È¸µî±ÞÀÇ ÀÌ¸§À» ÀÐ¾î¿È	->actype
{
	int i;
	for (i=0;i<20;i++) strcpy(actype[i],"");
	if (bit(ccfg.mode,1)) read_atype_(0);
	else read_atype_(1);
}



read_atype_(int mode)	  //µî±ÞÀÇ ÀÌ¸§À» ÀÐ¾î¿È  ->atype
{
	char type_file[140];
	FILE *fp1;
	int i;
	if (mode==0)		sprintf(type_file,"system/typename.dat");
	else if (club_mode) sprintf(type_file,"%s/system/typename.dat",club_go.dir);
	if ((fp1=fopen(type_file,"r+"))!=NULL) {
		for (i=0;i<20;i++)
			if (mode==0) fread(&atype[i],sizeof(atype[i]),1,fp1);
			else		 fread(&actype[i],sizeof(actype[i]),1,fp1);
		fclose (fp1);
	}
}



// µ¿È£È¸ ÄÚµå Ã£¾Æ¼­ club_go¿¡ ÀúÀå
find_club(char *code)
{
	FILE *fp;
	GO go1,oldgo;
	int i;
	int found=No;
	if (strlen(code)<1) return No;
	oldgo=go;
	club_mode=No;
	strcpy(club_go.code,"main");
	strcpy(club_go.dir,".");
	set_nowmenu();
	if (is_same(code,"main")) return Yes;
	if ((fp=fopen(NOW_MENU,"r+"))!=NULL) {
		while(fread(&go1,sizeof_go,1,fp)) {
			if (is_same(go1.code,code)&&go1.type=='s') {
				found=Yes;
				club_go=go1;
				club_mode=Yes;
				set_nowmenu();
				read_ccfg();
				break;
			}
		}
		fclose(fp);
	}
	go=oldgo;
	return found;
}


// °¡±â ÄÚµå Ã£¾Æ¼­ go¿¡ ÀúÀå
find_go(char *code)
{
	FILE *fp;
	int found=No;
	GO go1,oldgo;
	oldgo=go;
	strcpy(go.code,"");
	go.rec=0;
	go.type='m';
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	list_mode=No;
	bbs_ltmode=No;
	set_nowmenu();
	if (strlen(code)<1) return No;
    if (is_goindex(NOW_MENU,code)) {
        if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
            while(fread(&go1,sizeof_go,1,fp)) {
                if (go1.type>0&&go1.type!='t'&&is_same(go1.code,code)) {
                    found=Yes;
                    go=go1;
                    break;
                }
            }
            fclose(fp);
        }
    }
	if (!found) go=oldgo;
	return found;
}

is_goindex(char *menu,char *gocode)
{
    FILE *fp1,*fp2;
    GO tgo;
    int found=No;
    char menufile[300];
    strcpy(menufile,menu);
    strchng2(menufile,".dat",".inx");
    if ((fp1=fopen(menufile,"r"))==NULL) {
        make_goindex(menu);
        fp1=fopen(menufile,"r");
    }
    if (fp1==NULL) return Yes; //¾øÀ¸¸é ÀÖ´Â°ÍÀ¸·Î °£ÁÖ
    while(fread(&tgo.code,sizeof(tgo.code),1,fp1)) {
        if (is_same(tgo.code,gocode)) {
            found=Yes;
            break;
        }
    }
    fclose(fp1);
    return (found);
}

make_goindex(char *menu)
{
    FILE *fp1,*fp2;
    char menufile[300];
    GO tgo;
    strcpy(menufile,menu);
    strchng2(menufile,".dat",".inx");
    if ((fp1=fopen(menu,"r"))==NULL) return;
    if ((fp2=fopen(menufile,"w"))==NULL) return;
    while(fread(&tgo,sizeof_go,1,fp1)) {
        if (tgo.type>=' '&&strlen(tgo.code)>0) {
            fwrite(&tgo.code,sizeof(tgo.code),1,fp2);
        }
    }
    fclose(fp1);
    fclose(fp2);
}


//¾ÆÀÌµð¸¦ ÀÐ¾î¼­ È¸¿øÈ­ÀÏ¿¡¼­ Ã£ÀºµÚ pf¿¡ ÀúÀåÇÑ´Ù.
read_pf(char *id)
{
	return read_pf_(0,0,id);
}

read_pf2(char *id)
{
	return read_pf_(2,0,id);
}

read_pf3(char *id)
{
	return read_pf_(0,1,id);
}

read_pf4(char *id)
{
	return read_pf_(2,1,id);
}


read_pf_(int mode,int mode2,char *id)
{
	FILE *fp;
	PF pf3,oldpf;
	int found;
	int i,ii,n;
	char ind_file[100];
	IND ind;
	int id_num=0;
	int last,first,now;
	char tid[30],tid2[30],buff[100];
    SMALLPF smallpf;

	if (mode==2) {
		oldpf=pf;	//ÇöÀçÀÇ PF ¹é¾÷
		strcpy(pf2.id,"guest");
		strcpy(pf2.name,"guest");
		pf2.type=0;
	}

	strcpy(tid,id);strlwr(tid);del_space(tid);del_esc(tid);
	if (strlen(tid)<3) return No;
	if ((fp=fopen("system/guestid.id","r"))!=NULL) {
		while (fgets(buff,15,fp)!=NULL) {
			del_enter(buff);
			if (is_same(buff,tid)) return No;
		}
		fclose(fp);
	}
	if ((fp=fopen(ID_FILE,"r"))!=NULL) {
		fseek(fp,pf.num*sizeof_pf,SEEK_SET);
		if(pf.num>=0&&fread(&pf3,sizeof_pf,1,fp)&&is_same(pf3.id,id)) {
		   found=Yes;
		   fclose(fp);
		   id_num=pf3.num;
		   goto _found;
		}
		fclose(fp);
	}
	sprintf(buff,"id/id_%u.ind",(byte)tolower(tid[0]));
    strcpy(ind_file,buff);
	//¸ÕÀú ÀÎµ¦½º È­ÀÏ¿¡ ÀÖ´ÂÁö °Ë»ö
	if ((fp=fopen(ind_file,"r"))==NULL) goto _cont2;
    if (bit(cfg.mprocmode,2)) {
        while(fread(&ind,sizeof_ind,1,fp)) {
            del_space(ind.id);ind.id[8]=0;ind.cnum[6]=0;
            if (strcasecmp(ind.id,tid)==0) {
                id_num=atoi(ind.cnum);
                break;
            }
        }
    }
    else {
        first=1;
        fseekend(fp);
        last=ftell(fp)/sizeof_ind;
        now=last/2+1;
        found=No;
        while(1) {
            if (now<1) now=1;
            fseek(fp,(now-1)*sizeof_ind,SEEK_SET);
            if (!fread(&ind,sizeof_ind,1,fp)) break;
            del_space(ind.id);ind.id[8]=0;ind.cnum[6]=0;
            del_esc(ind.id);
            n=strcmp(ind.id,tid);
    //        printf("NOW=%d id='%s' tid='%s' cnum=%s\n",now,ind.id,tid,ind.cnum);
            if (n==0) {
                found=Yes;
    //          printf(" 1Â÷Ã£À½:NOW=%d id='%s' tid='%s' cnum=%s\n",now,ind.id,tid,ind.cnum);
                break;
            }
            else if ((last-first<5)||now<0) break;
            else if (n>0) last=now;
            else          first=now;
            now=(last-first)/2+first+1;
        }

        i=now-5;if (i<0) i=0;
        id_num=0;       //´Ù½Ã Àç°Ë»öÇÑ´Ù.   Áßº¹µÈ°ÍÀÌ ÀÖÀ»¼ö ÀÖÀ¸¹Ç·Î
        for (n=i;n<now+15;n++) {
            fseek(fp,n*sizeof_ind,SEEK_SET);
            if (!fread(&ind,sizeof_ind,1,fp)) break;
            del_space(ind.id);ind.id[8]=0;ind.cnum[6]=0;
    //      printf(" n=%d l=%d f=%d NOW=%d id='%s' tid='%s' cnum=%s (%d.ind)\n",n,last,first,now,ind.id,tid,ind.cnum,(byte)tid[0]);;
            if (strcasecmp(ind.id,tid)==0) {
                id_num=atoi(ind.cnum);
                break;
            }
        }
    }
	fclose(fp);

    //½ÇÁ¦ È­ÀÏ¿¡¼­ Ã£±â
	_cont2:
	found=No;
	if (id_num<0) id_num=0;
	if ((fp=fopen(ID_FILE,"r"))==NULL) return No;
	fseek(fp,id_num*sizeof_pf,SEEK_SET);
	if(id_num>=0&&fread(&pf3,sizeof_pf,1,fp)) {
//      printf("Ã£±â:%d %s %s\n",id_num,pf3.id,id);
		if (is_same(pf3.id,id)) {
			found=Yes;
			pf3.num=id_num;
		}
	}
    if (!bit(cfg.pfmode,1)) mode2=0;
    if (!found&&mode2!=1) {    //mode2==1ÀÌ¸é ¼¼ºÎ°Ë»ö¾ÈÇÔ
		id_num=0;
		fseektop(fp);
        while(1) {
            fseek(fp,id_num*sizeof_pf,SEEK_SET);
            if (!fread(&smallpf,sizeof_smallpf,1,fp)) break;
            if (is_same(smallpf.id,id)) {
                fseek(fp,id_num*sizeof_pf,SEEK_SET);
                fread(&pf3,sizeof_pf,1,fp);
				found=Yes;
				pf3.num=id_num;
				break;
			}
			id_num++;
		}
    }
	fclose(fp);
	if (!found) {
		if (mode==2) pf=oldpf;
		return No;
	}

	_found:
	pf=pf3;
    pf.pos1[7]=0;pf.pos2[7]=0;pf.addr[70]=0;pf.add2[70]=0;
	if (pf.num<0) pf.num=0;
	if (strlen(pf.nick)<3) strcpy(pf.nick,pf.id);
    del_esc3(pf.id);del_esc3(pf.name);del_esc3(pf.nick);pf.id_no[14]=0;
    pf.mymode=set_bit(pf.mymode,29,0);
	if (mode==2) {pf2=pf;pf=oldpf;}
	return (found);
}



make_ind(int mode,char *str)
{
	PF pf2;
	FILE *fp1,*fp2;
	char buff[200],buff1[100],buff2[100];
    char ind_file[200],ind_file2[200];
	char big[20],ch;
	IND ind;
	int i,n=0,total=0;
	int afound[270];
    SMALLPF smallpf;

	if (is_same(str,"make_ind_all")) {
        if (mode!=1) printf("\n ## ID REINDEX  (TOTAL:%d)...please wait...\n",count_rec(ID_FILE,sizeof_pf));
		strcpy(ind_file ,"id/id_all.ind");
		if ((fp1=fopen(ID_FILE ,"r"))==NULL) return;
		if ((fp2=fopen(ind_file,"w"))==NULL) return;
		ind.ret='\n';
		n=0;
        while(1) {
            fseek(fp1,n*sizeof_pf,SEEK_SET);
            if (!fread(&smallpf,sizeof_smallpf,1,fp1)) break;
            strlwr(smallpf.id);
            if (strlen(smallpf.id)>=3) {
                  fprintf(fp2,"%s %d\n",smallpf.id,n);
			}
			n++;
			total++;
		}
		fclose(fp1);
		fclose(fp2);
        if (mode!=1) printf("sort file...please wait...\n");
        strcpy(ind_file2,"id/id_out.ind");
        xsort(0,ind_file,ind_file2);

        if ((fp1=fopen(ind_file2,"r"))==NULL) return;
		sprintf(ind_file ,"id/id__.ind");
		if ((fp2=fopen(ind_file,"w"))==NULL) return;
        for (i=0;i<256;i++) afound[i]=0;
		while(fgets(buff,200,fp1)!=NULL) {
			if (strlen(buff)<3) continue;
			bzero(buff1,30);
            sscanf(buff,"%s%s",buff1,buff2);
//            sscanf(buff,"%x %x %x %x %x %x %x %x %s",&buff1[0],&buff1[1],&buff1[2],&buff1[3],&buff1[4],&buff1[5],&buff1[6],&buff1[7],buff2);
			buff1[8]=0;
            strcpy(ind.id,buff1);
            strcpy(ind.cnum,buff2);
			ind.ret='\n';
            del_space(ind.id);strlwr(ind.id);del_space(ind.cnum);
            sprintf(buff,"id/id_%u.ind",(byte)ind.id[0]);
//          printf("*%s*%s*%s*\n",buff,buff1,buff2);
			if (!is_same(buff,ind_file)) {
				fclose(fp2);
				fp2=NULL;
				strcpy(ind_file,buff);
				fp2=fopen(ind_file,"w+");
                if (mode!=1) printf(" file %s SORTING ...please wait...\n",ind_file);
				afound[(byte)ind.id[0]]=Yes;
			}
			if (fp2!=NULL) fwrite(&ind,sizeof_ind,1,fp2);
		}
		fclose(fp1);
		if (fp2!=NULL) fclose(fp2);
        for (i=40;i<=205;i++) {
			if (afound[i]==0) {
                sprintf(ind_file,"id/id_%u.ind",i);
				unlink(ind_file);
			}
		}
		unlink(TMP_FILE);unlink(TMP_FILE2);
		system("chmod 777 id/*.ind");
        if (mode!=1) printf(" %d ID indexing Done!!\n",total);
	}
	else {
		strlwr(str);
		del_esc(str);del_space(str);del_enter(str);
		if (strlen(str)<1) return;
		sprintf(ind_file ,"id/id_%u.ind",(byte)tolower(str[0]));
        if (mode!=1) printf(" ÀÎµ¦½º ÀçÁ¤¸®Áß...(%s)\n",ind_file);
		if ((fp1=fopen(ID_FILE ,"r"))==NULL) return;
		if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
		ind.ret='\n';
		n=0;
        while(1) {
            fseek(fp1,n*sizeof_pf,SEEK_SET);
            if (!fread(&smallpf,sizeof_smallpf,1,fp1)) break;
            strlwr(smallpf.id);
            if (strlen(smallpf.id)>2&&(byte)smallpf.id[0]==(byte)str[0]) {
                smallpf.num=n;
                fprintf(fp2,"%s %d\n",smallpf.id,smallpf.num);
				total++;
			}
			n++;
		}
		fclose(fp1);
		fclose(fp2);
        if (mode!=1) printf(" ¾ÆÀÌµð Á¤·ÄÁß.....(%s)\n",ind_file);
        xsort(0,TMP_FILE,TMP_FILE2);
//		mess("%s %s",TMP_FILE,TMP_FILE2);
		if ((fp1=fopen(TMP_FILE2,"r"))==NULL) return;
		if ((fp2=fopen(ind_file,"w"))==NULL) return;
		while(fgets(buff,200,fp1)!=NULL) {
			if (strlen(buff)<3) continue;
			bzero(buff1,30);
            sscanf(buff,"%s%s",&buff1,buff2);
            if (strlen(buff1)<3) continue;
//            sscanf(buff,"%x %x %x %x %x %x %x %x %s",&buff1[0],&buff1[1],&buff1[2],&buff1[3],&buff1[4],&buff1[5],&buff1[6],&buff1[7],buff2);
			buff1[8]=0;
            strcpy(ind.id,buff1);
            strcpy(ind.cnum,buff2);
			ind.ret='\n';
            del_space(ind.id);strlwr(ind.id);del_space(ind.cnum);
			fwrite(&ind,sizeof_ind,1,fp2);
		}
		fclose(fp1);
		fclose(fp2);
		unlink(TMP_FILE);
		unlink(TMP_FILE2);
        chmod(ind_file,0666);
	}
}



#define SORT_MAX1 10000
#define SORT_MAX2 50000
//mode==1 È­¸éÃâ·Â
make_sort(int mode,int imode,char *buffstr)
{
    int i;
    char buff[100];
    int  a_ok[300];
    strcpy(buff,buffstr);
    if (strlen(buff)<1||strcasecmp(buff,"make_ind_all")==0) {
        if (mode!=1) printf("\n ## ID REINDEX  (TOTAL:%d)...please wait...\n",count_rec(ID_FILE,sizeof_pf));
        for(i=40;i<210;i++) a_ok[i]=No;
        for(i=40;i<210;i++) {
            sprintf(buff,"%c%c__",(byte)i,(byte)i);
            strlwr(buff);
            if (!a_ok[(byte)buff[0]]) {
                a_ok[(byte)buff[0]]=Yes;
                a_ok[i]=Yes;
                make_sort(mode,imode,buff);
            }
        }
    }
    else if (imode==1) make_sort1(mode,buffstr);
    else if (imode==2) make_sort2(mode,buffstr);
}

int comp_5(const void *a, const void *b)
{
    IND2 *inda,*indb;
    inda = (IND2 *)a;
    indb = (IND2 *)b;
    return strcmp(inda->id , indb->id);
}


make_sort1(int mode,char *tstr)
{
    char buff[100];
    int i,n,count=0;
	FILE *fp;
	char ind_file[100];
	char ind2_file[100];
    char str[100];
    IND ind;
    IND2 aind[SORT_MAX1];
    SMALLPF smallpf;

    strcpy(str,tstr);
    strlwr(str);
    if (strlen(str)<2) return;
    sprintf(ind_file ,"id/id_%u.ind",(byte)str[0]);
    if (mode!=1) printf(" ÀÎµ¦½º ÀçÁ¤¸®Áß...(%s)  M:%d\n",ind_file,SORT_MAX1);
	if ((fp=fopen(ID_FILE ,"r"))==NULL) return;
    sprintf(buff,"%c%c",(byte)255,(byte)255);
    for (i=0;i<SORT_MAX1;i++) strcpy(aind[i].id,buff);
	n=0;
    count=0;
    while(count<SORT_MAX1){
        fseek(fp,n*sizeof_pf,SEEK_SET);
        if (!fread(&smallpf,sizeof_smallpf,1,fp)) break;
        strlwr(smallpf.id);
        if (strlen(smallpf.id)>=3&&(byte)smallpf.id[0]==(byte)str[0]) {
            sprintf(aind[count].id,smallpf.id);
            aind[count].cnum=n;
            count++;
		}
        n++;
	}
	fclose(fp);
    if (count<1) {unlink(ind_file);return;}
    if (count>(SORT_MAX1-10)) printf("\n ### %s IND ÆÄÀÏ ID %d ÃÊ°ú\n",ind_file,SORT_MAX1);
    qsort(aind,SORT_MAX1,sizeof(aind[0]),comp_5);
	if ((fp=fopen(ind_file,"w"))!=NULL) {
        ind.ret='\n';
        for (i=0;i<count+1;i++) {
			if (strlen(aind[i].id)>2) {
//                printf("%d %s\n",i,aind[i].id);
                strcpy(ind.id,aind[i].id);
                sprintf(ind.cnum,"%u",aind[i].cnum);
                del_space(ind.id);strlwr(ind.id);del_space(ind.cnum);
                fwrite(&ind,sizeof_ind,1,fp);
			}
		}
		fclose(fp);
	}
}



make_sort2(int mode,char *tstr)
{
    char buff[100];
    int i,n,count=0;
	FILE *fp;
	char ind_file[100];
	char ind2_file[100];
    char str[100];
    IND ind;
    IND2 aind[SORT_MAX1];
    SMALLPF smallpf;

    strcpy(str,tstr);
    strlwr(str);
    if (strlen(str)<2) return;
    sprintf(ind_file ,"id/id_%u.ind",(byte)str[0]);
    if (mode!=1) printf(" ÀÎµ¦½º ÀçÁ¤¸®Áß...(%s)  M:%d\n",ind_file,SORT_MAX2);
	if ((fp=fopen(ID_FILE ,"r"))==NULL) return;
    sprintf(buff,"%c%c",(byte)255,(byte)255);
    for (i=0;i<SORT_MAX2;i++) strcpy(aind[i].id,buff);
	n=0;
    count=0;
    while(count<SORT_MAX2) {
        fseek(fp,n*sizeof_pf,SEEK_SET);
        if (!fread(&smallpf,sizeof_smallpf,1,fp)) break;
        strlwr(smallpf.id);
        if (strlen(smallpf.id)>=3&&(byte)smallpf.id[0]==(byte)str[0]) {
            sprintf(aind[count].id,smallpf.id);
            aind[count++].cnum=n;
		}
        n++;
	}
	fclose(fp);
    if (count<1) {unlink(ind_file);return;}
    if (count>(SORT_MAX2-10)) printf("\n ### %s IND ÆÄÀÏ ID %d ÃÊ°ú\n",ind_file,SORT_MAX2);
    qsort(aind,SORT_MAX2,sizeof(aind[0]),comp_5);
	if ((fp=fopen(ind_file,"w"))!=NULL) {
        ind.ret='\n';
        for (i=0;i<count+1;i++) {
			if (strlen(aind[i].id)>2) {
//                printf("%d %s\n",i,aind[i].id);
                strcpy(ind.id,aind[i].id);
                sprintf(ind.cnum,"%u",aind[i].cnum);
                del_space(ind.id);strlwr(ind.id);del_space(ind.cnum);
                fwrite(&ind,sizeof_ind,1,fp);
			}
		}
		fclose(fp);
	}
}


//ÇöÀçÀÇ µ¿È£È¸¿¡ ¾ÆÀÌµð¸¦ Ã£´Â´Ù.	->cpf¿¡ ÀúÀå
read_cpf(char *id)
{
	FILE *fp;
	int found=No;
	char id_file[200];
	CPF cpf2;
	strcpy(cpf.id,"");
	strcpy(cpf.name,"");
	cpf.type=0;
	cpf.warn=0;
	sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
	if ((fp=fopen(id_file,"r"))==NULL) return No;   //¾ÆÀÌµð ¾øÀ½
	while (fread(&cpf2,sizeof_cpf,1,fp))
		if (strcmp(cpf2.id,id)==0) {
			if (cpf2.type>19) cpf2.type=0;
			found=Yes;
			break;
		}
	fclose(fp);
	if (found) cpf=cpf2;
	return found;
}


//cpfÀÇ ³»¿ëÀ» ÀúÀåÇÔ.
save_cpf(char *id)
{
	FILE *fp;
	CPF cpf2;
	int i;
	int found=No;
	char id_file[200];

	sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
	c_lock(1,id_file);
	if ((fp=fopen(id_file,"r+"))==NULL) return No;   //¾ÆÀÌµð ¾øÀ½
	i=0;
	while (fread(&cpf2,sizeof_cpf,1,fp)) {
		if (is_same(cpf2.id,id)) {
			fseek(fp,i*sizeof_cpf,SEEK_SET);
			cpf.num=i+1;
			fwrite(&cpf,sizeof_cpf,1,fp);
			found=Yes;
			break;
		}
		i++;
	}
	fclose(fp);
	c_lock(0,id_file);
	sync();fflush(NULL);
	return found;
}


//pfÀÇ ³»¿ëÀ» ÀúÀåÇÔ.
save_pf(int mode)
{
	FILE *fp;
	PF pf2;
	int i;
	if (bit(allmode,3)) return No;	   //Å×½ºÆ®¿ëÀ¸·Î ÁöÁ¤
	if (pf.type<16&&cfg.levelmax>0&&pf.level>cfg.levelmax) pf.level=cfg.levelmax;

	c_lock(1,ID_FILE);
	if ((fp=fopen(ID_FILE,"r+"))==NULL)  {
		c_lock(0,ID_FILE);
		return No;
	}
	if (pf.num<0) pf.num=0;
	fseek(fp,pf.num*sizeof_pf,SEEK_SET);
	if (!fread(&pf2,sizeof_pf,1,fp)) goto _cont1;
	if (is_same(pf2.id,pf.id)) {
		fseek(fp,pf.num*sizeof_pf,SEEK_SET);
        pf.id_no[14]=0;
//        fwrite(&pf,sizeof_pf,1,fp);
        gwrite(mode,pf,fp);
	}
	else {						 //Æ²¸±°æ¿ì
		_cont1:
		rewind(fp);
		i=0;
		while (1) {
			fseek(fp,i*sizeof_pf,SEEK_SET);
			if (!fread(&pf2,sizeof_pf,1,fp)) break;
			if (is_same(pf2.id,pf.id)) {
				pf.num=i;
                if (pf.type<0||pf.type>19) pf.type=1;
                if (pf.cash<0) pf.cash=0;
                if (pf.level<0) pf.level=0;
				fseek(fp,i*sizeof_pf,SEEK_SET);
//                fwrite(&pf,sizeof_pf,1,fp);
                gwrite(mode,pf,fp);
				break;
			}
			i++;
		}
	}
	fclose(fp);
	c_lock(0,ID_FILE);
	sync();fflush(NULL);
	return Yes;
}


//pf2ÀÇ ³»¿ëÀ» ÀúÀåÇÔ.
save_pf2(int mode)
{
	int ret;
	PF oldpf;
	oldpf=pf;
	pf=pf2;
	ret=save_pf(mode);
	pf=oldpf;
	return ret;
}


//return 0: ¾øÀ½  1:¾ÆÀÌµð ÀÖÀ½  2:¸ÞÀÏ¼ö½Å°ÅºÎ  3:¸Þ¼¼Áö¼ö½Å°ÅºÎ
is_moni(char *tid)
{
	FILE *fp;
	int i,n,found;
	char filename[100];
	MONI moni2;
	moni.mode=0;
	strcpy(moni.id,"");
	found=No;
    if (cfg.count_server==1) moni.hostno=cfg.hostno;
    for (n=-1;!found&&n<=cfg.count_server;n++) {
		if (n!=-1&&n==moni.hostno) continue;
		if (n==-1) sprintf(filename,"user/moni/moni%d.dat",moni.hostno);
		else	   sprintf(filename,"user/moni/moni%d.dat",n);
		if ((fp=fopen(filename,"r"))==NULL) continue;
		while (fread(&moni2,sizeof_moni,1,fp)) {
			if (moni2.mode<1||strlen(moni2.id)<3||strlen(moni2.tty)<1) continue;
			if (is_same(moni2.id,tid)) {
				moni=moni2;
				found=moni.mode;
				break;
			}
		}
		fclose(fp);
	}
	return found;
}




int check_pass(char *pass,int num,char *mess)
{
	char buff2[10];
	int i;
	if (strlen(pass)<1) return Yes;
	for (i=1;i<4;i++) {
		printf("\n");
		printf("%s",mess);
		passinput(buff2,num);  //¾ÏÈ£ÀÔ·Â
		printf("\n");
		if (is_same(pass,buff2)) return Yes;
		printf("  ¾ÏÈ£°¡ Æ²¸³´Ï´Ù.\n");
	}
	return No;
}

int check_pass2(int pass,int num,char *mess)
{
	char buff[10];
	sprintf(buff,"%d",pass);
	return (check_pass(buff,num,mess));
}

view_pf (char *tid) {
	viewpf(1,tid,0);
}
view_pfs(char *tid) {
	viewpf(8,tid,0);
}
view_pfw(char *tid,int who) {
	viewpf(9,tid,who);
}
view_pfw2(PF pf2,int who)
{
	PF oldpf;
	oldpf=pf;
	pf=pf2;
	if (who>=5) view("frm/pf/pfs.scr");
	else if (who>=2&&pf.type<18) view("frm/pf/pfs.scr");
	else if (who>=2&&pf.type>=18) printf("## %s ´ÔÀº º¼¼ö¾ø´Â µî±Þ%dÀÔ´Ï´Ù.",pf.id,pf.type);
	else viewpf1();
	pf=oldpf;
}


viewpf(int mode,char *tid,int who)
{
	PF oldpf;
	int i;
	oldpf=pf;
	if (!read_pf4(tid)) goto _fail;
	if (strcmp(tid,pf2.id)!=0) {
		if (check_sameid(tid)) {
			mess("%s ´ÔÀº %s ID·Î º¯°æµÇ¾ú½À´Ï´Ù",tid,g_buff2);
			strcpy(tid,g_buff2);
			if (!read_pf4(tid)) goto _fail;
			goto _cont1;
		}
	}

	_cont1:
	pf=pf2;
	for (i=0;i<23;i++) printf("\n");
		 if (mode==1) viewpf1();
	else if (mode==8&&who<5&&pf.type>=18) viewpf1();
	else if (mode==8) viewpf8();
	else if (mode==9&&who>=4) viewpf8();
	else if (mode==9&&who<=3) viewpf1();
	pf=oldpf;
	return;

	_fail:
	pf=oldpf;
	mess(" %s È¸¿øÀ» Ã£À»¼ö ¾ø½À´Ï´Ù.",tid);return;
	return;

}

check_sameid(char *tid)
{
	FILE *fp;
    int found=No,count;
	char filename[100];
    char buff[200],buff1[200],buff2[100],buff3[100],buff4[100];
	strcpy(g_buff1,"");strcpy(g_buff2,"");
	if (strlen(tid)<1) return No;
	if (tid[0]>='0'&&tid[0]<='F')      strcpy(filename,"system/sameid1.id");
	else if (tid[0]>='F'&&tid[0]<='Z') strcpy(filename,"system/sameid2.id");
	else							   strcpy(filename,"system/sameid3.id");

//  mess("ID:%s  FI:%s SIZE:%d Byte",tid,filename,file_size(filename));
    count=0;
	if ((fp=fopen(filename,"r"))==NULL) return No;
    while (fgets(buff,100,fp)!=NULL) {
        count++;
		del_enter(buff);
//      printf("##%d  '%s'\n",count,buff);
		if (strlen(buff)<8) continue;
//¿ø·¡ID   »õID 	 ±âÁ¸Áßº¹ID
//buff2    buff3
//g_buff1  g_buff2
//7taiji7  kk20153	 7TAIJI7   8438  5627 (¹ÚÇöÁ¤	±èµ¿Çö	)
//6Kies    kk20200	 6KIES	   9777  3026 (ÇÔÇöÁØ	ÇöÀç¿µ	)
        strcpy(buff2,"");strcpy(buff3,"");strcpy(buff4,"");
		sscanf(buff,"%s %s %s",buff2,buff3,buff4);
        if (strlen(buff2)>0&&strcmp(buff2,tid)==0) {
//          printf("## %s LINE:%d\n",filename,count);printf("## %s\n",buff);
//          mess("IDÁßº¹ Tid:'%s' '%s' '%s' '%s'",tid,buff2,buff3,buff4);
			strcpy(g_buff1,buff2);
			strcpy(g_buff2,buff3);
			found=Yes;
			break;
		}
	}
	fclose(fp);
	return found;
}



viewpf1()
{
	int num=1;
	view("frm/pf/pf.scr");
							printf("  (%2d) µî      ±Þ : %2d (%s)\n",num++,pf.type,atype[pf.type]);
//							printf("  (%2d) ·¹      º§ : %d\n",num++,pf.level);
//							printf("  (%2d) »çÀÌ¹ö¸Ó´Ï : %d\n",num++,pf.cash);
							if (is_moni(pf.id))
							printf("  (%2d) Á¢¼Ó  ½ÃÀÛ : %s ºÎÅÍ »ç¿ëÁß\n",num++,datestr(33,pf.login));
							else
							printf("  (%2d) ÃÖ±Ù  ½Ã°£ : %s ¿¡ Á¢¼ÓÁ¾°á\n",num++,datestr(33,pf.logout));
//							printf("  (%2d) ÀÎÅÍ³Ý¸ÞÀÏ : %s\n",num++,pf.email);
    if (bit(pf.myflag, 1))  printf("  (%2d) »ý      ÀÏ : %s    (³ªÀÌ:%d¼¼)\n",num++,datestr(1,pf.birthday),get_age(pf));
	if (bit(pf.myflag, 2))	printf("  (%2d) ¼º      º° : %s\n",num++,is_char("0f",pf.sex)?"¿©":"³²");
	if (bit(pf.myflag,10))	printf("  (%2d) Á÷      ¾÷ : %2d %s\n",num++,pf.job,ajob[pf.job]);
	if (bit(pf.myflag, 3))	printf("  (%2d) ¿ì      Æí : %s\n",num++,pf.pos1);
	if (bit(pf.myflag, 4))	printf("  (%2d) ÁÖ      ¼Ò : %s\n",num++,pf.addr);
	if (bit(pf.myflag,11))	printf("  (%2d) Á÷Àå£¯ÇÐ±³ : %s\n",num++,pf.offi);
	if (bit(pf.myflag,17))	printf("  (%2d) Ãë      ¹Ì : %s\n",num++,pf.user4);
	if (bit(pf.myflag,13))	printf("  (%2d) ÇÏ°í½ÍÀº¸» : %s\n",num++,pf.memo1);
	if (bit(pf.myflag,15))	printf("  (%2d) °ü½É  ºÐ¾ß : %s\n",num++,pf.user2);
	if (bit(pf.myflag,16))	printf("  (%2d) ÄÄÇ»ÅÍ°æ·Â : %s\n",num++,pf.user3);
	view("frm/pf/pf.end");
}


viewpf8()
{
	view("frm/pf/pfs.scr");
	view("frm/pf/pfs.end");
}


view_cpf(char *tid,int type)
{
	CPF oldcpf;
	if (!club_mode) {
		mess("µ¿È£È¸¼­¸¸ °¡´ÉÇÑ ¸í·ÉÀÔ´Ï´Ù.");
		return;
	}
	oldcpf=cpf;
	if (!read_cpf(tid)) {
		mess("%s È¸¿øÀ» µ¿È£È¸ È¸¿ø¸íºÎ ¿¡¼­ Ã£À»¼ö ¾ø½À´Ï´Ù.",tid);
		cpf=oldcpf;
		return;
	}
	if (type>=17) disp_cpf(cpf,2);
	else		  disp_cpf(cpf,1);
	cpf=oldcpf;
}



check_pay(int mode,char *buff,PF pf2)
{
	strcpy(buff,"");
}


//cpfÀÇ ¾ÆÀÌµð¸¦ Ãâ·Â
disp_cpf(CPF cpf,int who)
{
	PF oldpf;
	read_pf4(cpf.id);
	if (pf2.type>=18) {
		strcpy(pf2.passwd,"");
		pf2.type=1;
	}
	if (who>=2) {
		if (!view("%s/frm/pf/cpfs.scr",club_go.dir)) {
			if (!view("frm/pf/cpfs.scr")) goto _cont;
		}
	}
	else {
		_cont:
		if (!view("%s/frm/pf/cpf.scr",club_go.dir)) view("frm/pf/cpf.scr");
	}
}


//¸¶Áö¸· ÀÚ·á¸¦ Ã£¾Æ ÀÚ·áÀÇ ¼ö¸¦ °è»êÇÔ
int  count_pf() {
	return count_rec(ID_FILE,sizeof_pf);
}


//µ¿È£È¸ ÀÚ·áÀÇ ¼ö¸¦ °è»êÇÔ
int  count_cpf(char *idfile) {
	return count_rec(idfile,sizeof_cpf);
}

//µ¿È£È¸ ÀÚ·áÀÇ ¼ö¸¦ °è»êÇÔ
int  count_menu(char *menufile) {
	FILE *fp;
	GO tgo;
	int count=0;
	if ((fp=fopen(menufile,"r"))!=NULL) {
		while(fread(&tgo,sizeof_go,1,fp)) {
			if (tgo.type!=' '&&tgo.type!='t'&&strlen(tgo.code)>0)
				count++;
		}
		fclose(fp);
	}
	return count;
}


//°è½ÃÆÇ ÀÚ·á¼ö¸¦ return ÇØÁØ´Ù.
char *b_disp(char *goname,int mode)
{
	static char buff[90];
	GO oldgo;
	GO newgo;
	oldgo=go;
	find_go(goname);
	newgo=go;
	go=oldgo;
	if		(mode==0)  strcpy(buff,"");
	else if (mode==1)  strcpy(buff,newgo.num);
	else if (mode==2)  strcpy(buff,newgo.name);
	else if (mode==3)  strcpy(buff,newgo.dir);
	else strcpy(buff,"");
	return (buff);
}
//ÀÔ·ÂÇÑ ³»¿ëÀ» mmc(¹®ÀÚ)¿Í mmn(¹øÈ£) À¸·Î ºÐÇØÇÑ´Ù
//±¸ºÐÀÚ´Â - , ÀÌ´Ù

//6:µî·Ï 8:´Ù¿î 18:ÀÛ¾÷
set_statdown(int mode)		 //¸Þ¼¼Áö ¼ö½Å¹æÁö
{
	FILE *fp;
	int i=0;
	MONI moni;
	char buff[200];
	char filename[100],filename2[100];
	sprintf(filename,"user/moni/moni%d.dat",cfg.hostno);
	if ((fp=fopen(filename,"r+"))==NULL) return;
	while (fread(&moni,sizeof_moni,1,fp)) {
		if(moni.mode>0&&
			is_same(moni.tty,tty)&&
			moni.mode!=mode) {
			OLD_MONI_MODE=moni.mode;  //º¹±¸¸¦ À§ÇØ..
			moni.mode=mode;
			fseek(fp,i*sizeof_moni,SEEK_SET);
			mymoni_rec=i;
			fwrite(&moni,sizeof_moni,1,fp);
			break;
		}
		i++;
	}
	fclose(fp);
	sync();fflush(NULL);
	//´Ù¿î,¾÷½Ã ½Ã°£È®ÀÎ±ÝÁöÇØÁ¦
	if (mode==6||mode==8) allmode=set_bit(allmode2,2,1);
}


set_nowmenu()		//ÀÓ½ÃÈ­ÀÏÁöÁ¤ÇÏ±â
{
	if (club_mode) sprintf(NOW_MENU ,"%s/%s",club_go.dir,MENU_FILE);
	else if (pf.type>0&&pf.localmenu>0&&bit(cfg.gomode,11)) {
		sprintf(NOW_MENU,"menustru/%d/menu.dat",pf.localmenu);
		if (!is_file(NOW_MENU)) sprintf(NOW_MENU,"menustru/menu.dat");
	}
	else strcpy(NOW_MENU,MENU_FILE);
}


set_tmpfile()		//ÀÓ½ÃÈ­ÀÏÁöÁ¤ÇÏ±â
{
	sprintf(TMP_FILE ,"tmp/tmp0%d%s",cfg.hostno,no_spc(tty));   //È£½ºÆ®¹øÈ£º°ÀÓ½ÃÈ­ÀÏ
	sprintf(TMP_FILE1,"tmp/tmp1%d%s",cfg.hostno,no_spc(tty));   //È£½ºÆ®¹øÈ£º°ÀÓ½ÃÈ­ÀÏ
	sprintf(TMP_FILE2,"tmp/tmp2%d%s",cfg.hostno,no_spc(tty));   //È£½ºÆ®¹øÈ£º°ÀÓ½ÃÈ­ÀÏ
	sprintf(LT_FILE  ,"tmp/tmpt%d%s",cfg.hostno,no_spc(tty)); // ÀÓ½ÃÈ­ÀÏ
}
set_bcfgfile()
{
	sprintf(BCFG_FILE,"%s/bbsdata/%s/bcfg",club_go.dir,go.code);
}

clear_menu()
{
	go.deep=9;
	go.type=' ';
	bzero(go.num,5);
	bzero(go.code,9);
	bzero(go.name,31);
	bzero(go.wtimes,4);
	bzero(go.dir,61);
}

char *byte2str(int size)
{
	static char buff[20];
	if		(size<1000) sprintf(buff,"%dB",size);
	else if (size>=1000&&size<1024) sprintf(buff,"%.1fK",(float)size/1000);
	else if (size>=1024&&size<10000) sprintf(buff,"%.1fK",(float)size/1024);
	else if (size>=10024&&size<1000000) sprintf(buff,"%dK",size/1024);
	else if (size>=1000000&&size<10000000) sprintf(buff,"%.1fM",(float)size/1000000);
	else if (size>=10000000) sprintf(buff,"%dM",size/1000000);
	return (buff);
}


xedit(const char *fmt,...)
{
	char buff[300];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	xedit_(buff);
	return Yes;
}


xedit2(const char *fmt,...)
{
	char buff[300];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buff,fmt, ap);
	va_end(ap);
	xedit_(buff);
	if(file_size(buff)<5) unlink(buff);
	return Yes;
}


xedit_(char *buff)
{
	int i;
	del_esc3(buff);del_clear(buff);
	if (strlen(buff)<1) {printf("\n ### editor1 ÆÄÀÏÁöÁ¤¾ÈµÊ\n");return;}
	xy(1,30);
	for (i=30;i--;) printf("\n");
	sys_tem("bin/editor1 -w '%s'",buff);
}


get_age(PF pf3)
{
	char buff[30];
    int n,n2,n3;
    if (pf3.type<1) return 0;
	strcpy(buff,pf3.id_no);buff[2]=0;
	n2=now_year()%100;
	if (n2<80) n2+=100;
	n=atoi(buff);
	if (n<=0) return 0;
    n3=n2-n;
    if (n3<0) n3=0;
    if (n3>99) n3=99;
    return n3;
}


check_han(char *buff)
{
	int i,n,found=No;
	char buff1[200];
	char buff2[4];
	if (strlen(buff)<2||strlen(buff)>3) return No;
	if (strlen(buff)==2&&!ishangul(buff[0])) return No;
	if (strlen(buff)==3&&buff[0]!='/') return No;
	if (strlen(buff)==3&&buff[0]=='/'&&!ishangul(buff[0])) return No;
	if (strlen(buff)==3&&buff[0]=='/') strcpy(buff2,&buff[1]);
	else strcpy(buff2,buff);

	strcpy(buff1,"¤±¤Ð¤º¤·¤§¤©¤¾¤Ç¤Á¤Ã¤¿¤Ó¤Ñ¤Ì¤À¤Ä¤²¤¡¤¤¤µ¤Å¤½¤¸¤¼¤Ë¤»");
	for (i=0;i<26;i++) {
		if (buff1[i*2+0]==buff2[0]&&buff1[i*2+1]==buff2[1]) {
			buff2[0]='a'+i;
			buff2[1]=0;
			found=Yes;
			break;
		}
	}
	if (found) {
		if (strlen(buff)==3&&buff[0]=='/') sprintf(buff,"/%s",buff2);
		else							   strcpy(buff,buff2);
	}
	return found;
}


bbs_size(char *filename)
{
	return (file_size(filename)/sizeof_bbs);
}


e_mess(char *filename,int key)
{
	char buff[300];
	if (is_char(filename,'/')) logo(filename,key);
	else {
		sprintf(buff,"frm/%s.mes",filename);
		logo(buff,key);
	}
}


bbs_man(char *id)
{
	if (pf.type>=17) return Yes;
	if (club_mode&&cpf.type>=17) return Yes;
	if (is_same(go.code,"rmail")||is_same(go.code,"cmail")||
		is_same(go.code,"userroom")||is_same(go.code,"mbox")) return Yes;
	if (is_same(bcfg.man1,id)||
		is_same(bcfg.man2,id)||
		is_same(bcfg.man3,id)||
		is_same(bcfg.man4,id)||
		is_same(bcfg.man5,id) ) return Yes;
	return No;
}


str_typenum(char *filename,char *str)
{
	FILE *fp;
	int n,i,ret,found;
	char buff1[300],buff2[300];
	char mms[30][180];

	if (str[0]==0||(fp=fopen(filename,"r+"))==NULL) return 0;
	found=No;
	ret=0;
	while (!found&&fgets(buff1,200,fp)!=NULL) {
		for (i=0;i<30;i++) strcpy(mms[i],"");
		sscanf(buff1,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		mms[0],mms[1],mms[2],mms[3],mms[4],mms[5],mms[6],mms[7],mms[8],mms[9],
		mms[10],mms[11],mms[12],mms[13],mms[14],mms[15],mms[16],mms[17],mms[18],mms[19],
		mms[20],mms[21],mms[22],mms[23],mms[24],mms[25],mms[26],mms[27],mms[28],mms[29]);
		for (i=1;!found&&i<30;i++) {
			if (is_same(mms[i],str)) {
				found=Yes;
				break;
				ret=atoi(mms[0]);
			}
		}
	}
	fclose(fp);
	return ret;
}

check_who()
{
	int who=0;		  //1:µ¿È£È¸ ¿î¿µÁø 2:µ¿È£È¸ ½Ã»ð  3:root±ÇÇÑ
	if (pf.type==16&&cpf.type<17)	 cpf.type=17;
	if (pf.type==17&&cpf.type<18)	 cpf.type=18;
	if (pf.type>=18&&cpf.type<19)	 cpf.type=19;
	if (pf.type==16)				  who=1;
	if (pf.type==17)				  who=2;
	if (pf.type==18)				  who=3;
	if (pf.type==19)				  who=4;
	if (club_mode) {
		if (is_same(ccfg.sysop,pf.id))	 cpf.type=19;
		if (cpf.type>=17&&who<1)		 who=1;
		if (cpf.type==18&&who<2)		 who=2;
		if (cpf.type==19&&who<2)		 who=2;
	}
		 if (is_same(bcfg.man1,pf.id)&&who<1) who=1;
	else if (is_same(bcfg.man2,pf.id)&&who<1) who=1;
	else if (is_same(bcfg.man3,pf.id)&&who<1) who=1;
	else if (is_same(bcfg.man4,pf.id)&&who<1) who=1;
	else if (is_same(bcfg.man5,pf.id)&&who<1) who=1;
	return who;
}




int ishan(unsigned char *str)
{
	int i;
	for(i=0;i<(int)strlen(str);i+=2) {
		if(!is_hangul(str+i)) return 0;
	}
	return 1;
}

int is_hangul(unsigned char *str)
{
	/* ¼ø¼öÇÑ ÇÑ±ÛÀÎÁö °Ë»ç */
	if(str[0]>=0xb0 && str[0]<=0xc8 && str[1]>=0xa1 && str[1]<=0xfe) return 1;
	return 0;
}


//¾ÏÈ£¸ðÀ½
spass(int mode,char buff)
{
	char filename[200];
	strcpy(filename,"system/spass.dat");
}


#include <utmp.h>
#define STRUCT_UTMP struct utmp
char *view_network(char *tty)
{
	register STRUCT_UTMP utmp_entry;
	static char hostbuff[20];
	FILE *fp;
	STRUCT_UTMP utmp;
	if (bit(allmode2,10)&&strlen(hostname)>0) {
		strcpy(hostbuff,hostname);
		return (hostbuff);
	}
	strcpy(hostbuff,tty);
//	  printf("Àá½ÃÅ×½ºÆ®Áß.. Àá½ÃÈÄ rc·Î ÀçÁ¢¼ÓÇÏ¼¼¿ä.\n");printf(cfg.utmpfile);pressenter();
	if (strlen(cfg.utmpfile)<3||(fp=fopen(cfg.utmpfile,"r"))==NULL) return (hostbuff);
	while (fread (&utmp,1,sizeof(utmp),fp)) {  //ÀÐ¾î³¿
		if (utmp.ut_name[0]&&!strncmp(tty, utmp.ut_line, sizeof (utmp.ut_line))) {
			xstrncpy(hostbuff,utmp.ut_host,20);
			break;
		}
	}
	fclose(fp);
	del_space(hostbuff);
	del_esc(hostbuff);
	return (hostbuff);
}



str_filedir(char *str,char *dir,char *filename)
{
	char buff1[300],buff2[300];
	int i;
	strcpy(dir,"");
	strcpy(filename,"");
	strcpy(buff1,str);
	for (i=(int)strlen(buff1)-2;i>=0;i--) {
		if (buff1[i]=='/') {
			strcpy(buff2,&buff1[i+1]);	   //ÆÄÀÏ¸í
			buff1[i+1]=0;
			break;
		}
	}
	if (i<0) {
		strcpy(dir,"./");
		strcpy(filename,buff1);
	}
	else {
		buff2[12]=0;
		strcpy(dir,buff1);
		strcpy(filename,buff2);
	}
}


//µð·ºÅä¸®¿Í È­ÀÏ¸íÀ» ÀÔ·Â¹Þ¾Æ ºÐÇØÇÑ´Ù.
get_filename(char *dir, char *filename,char *def_ups)
{
	int n,i;
	char buff[380];
	char buff2[380];
	strcpy(dir,"");
	strcpy(filename,"");
	if (strlen(def_ups)>0) printf("%s/",def_ups);
	lineinput(buff,120);check_han(buff);printf("\n");
	if (strlen(buff)<1) return No;
	else if (is_same(buff,"q")||is_same(buff,"p")) return No;
	else if (is_same(buff,"x")||is_same(buff,".")) return No;
	if (buff[0]!='/'&&strlen(def_ups)>0) {
		sprintf(buff2,"%s/%s",def_ups,buff);
		strcpy(buff,buff2);
	}
	str_filedir(buff,dir,filename);
	return Yes;
}

//¸Þ´º¸¦ ÃÊ±â ¸Þ´ºÄÚµå·Î ¼±ÅÃÇÔ
set_top()
{
	strcpy(go.code,"top");
	strcpy(go.name,"ÃÊ±â¸Þ´º");
	go.type='m';
	club_mode=No;
	club_go.rec=0;
	strcpy(club_go.dir,".");
	strcpy(club_go.code,"main");
}


char *bbs_tail()
{
	static char tail[10];
	int mode=0;
	strcpy(tail,"");
		 if (is_typebbs) mode=1;
	else if (is_typepds) mode=2;
	//È­¸é Ç¥½Ã ½ÃÀÛ
	if		(is_same(go.code,"top"))          strcpy(tail,"top");
	else if (is_same(go.code,"rmail"))        strcpy(tail,"rma");
	else if (is_same(go.code,"mbox"))         strcpy(tail,"rmb");
	else if (is_same(go.code,"cmail"))        strcpy(tail,"wma");
	else if (is_same(go.code,"wmail"))        strcpy(tail,"wma");
	else if (is_same(go.code,"wmall"))        strcpy(tail,"wma");
	else if (is_same(go.code,"userroom"))     strcpy(tail,"use");
	else if (is_char(go.wtimes,'*'))          strcpy(tail,"nos");
	else if (is_char(go.wtimes,'-'))          strcpy(tail,"noo");
	else if (is_same(go.code,"notice"))       strcpy(tail,"not");
	else if (is_same(go.code,"bbs_all"))      strcpy(tail,"zab");
	else if (is_same(go.code,"pds_all"))      strcpy(tail,"zap");
    else if (is_same(go.code,"dncart"))       strcpy(tail,"dnc");
	else if (go.type=='m')                    strcpy(tail,"sub");
	else if (go.type=='g')                    strcpy(tail,"gub");
	else if (go.type=='y')                    strcpy(tail,"gup");
	else if (go.type=='d')                    strcpy(tail,"dir");
	else if (go.type=='w')                    strcpy(tail,"wtx");
	else if (mode==1&&is_typevtx)			  strcpy(tail,"vtx");
	else if (go.type=='q')                    strcpy(tail,"que");
	else if (is_typesecu)					  strcpy(tail,"sec");
	else if (is_typecons&&!is_same(bcfg.man1,pf.id))  strcpy(tail,"sek");
	else if (is_typecons)					  strcpy(tail,"sem");
	else if (go.type=='b')                    strcpy(tail,"zbb");
	else if (go.type=='z')                    strcpy(tail,"zbp");
	else if (mode==1)						  strcpy(tail,"bbs");
	else if (mode==2)						  strcpy(tail,"pds");
	else									  strcpy(tail,"def");
	return (tail);
}



//°Ô½ÃÆÇ ÀÚ·á¼ö¸¦ return ÇØÁØ´Ù.
char *b_num(char *gocode,int mode)
{
	FILE *fp;
	static char buff[50];
	int now_num,n1,n2,n3;
	time_t t;
	BREC brec;
	char brecname[300];

	brec.allnum=0;
	brec.newnum=0;
	sprintf(brecname,"%s/menurec/%s.bre",club_go.dir,gocode);
	if ((fp=fopen(brecname,"r+"))!=NULL) {
		fread(&brec,sizeof_bcfg,1,fp);
		fclose(fp);
		if (brec.newnum>0&&is_old_day(brec.numdate)>0) {		//°Ô½ÃÆÇÁ¤¸®ÃÖÁ¾³¯Â¥
			re_bbsnum(3);								  //¿À´ÃÀÚ·á»õ·Î°»½Å
		}

	}
	if (brec.allnum<0||brec.allnum>9999999) brec.allnum=0;
	if (brec.newnum<0||brec.newnum>9999999) brec.newnum=0;
	if		(mode== 0) sprintf(buff,"%2d/%3d",brec.newnum,brec.allnum);
	else if (mode== 1) sprintf(buff,"%2d/%4d",brec.newnum,brec.allnum);
	else if (mode== 2) sprintf(buff,"%3d/%5d",brec.newnum,brec.allnum);
	else if (mode== 3) sprintf(buff,"%2d",brec.newnum);
	else if (mode== 4) sprintf(buff,"%3d",brec.newnum);
	else if (mode== 5) sprintf(buff,"%3d",brec.allnum);
	else if (mode== 6) sprintf(buff,"%4d",brec.allnum);
	else if (mode== 7) sprintf(buff,"%5d",brec.allnum);
	else if (mode== 8) sprintf(buff,"%6d",brec.allnum);
	else if (mode== 9) sprintf(buff,"%d" ,brec.newnum);
	else if (mode==10) sprintf(buff,"%d" ,brec.allnum);
	else if (mode==11) sprintf(buff,"(%c%5d)",brec.newnum>0?'*':' ',brec.allnum);
	else			   sprintf(buff,"");
	return (buff);
}






//mode	0:±×³ÉbrecÀÐ±â		1:ÀÚµ¿°»½Å 2:¿À´ÃÀÚ·á¸¸ »õ·Î°»½Å  3:ÀüÃ¼ÀÚ·á»õ·Î °»½Å
//mode	8:ÀüÃ¼ÀÚ·á1°³Ãß°¡	9:ÀüÃ¼ÀÚ·á1°³»èÁ¦
re_bbsnum(int mode)
{
	FILE *fp;
	char brecfilename[380];
	time_t tdate,t;
	struct tm *tp;
	int i,ii,n,rec,oldmode,count=0;
	BBS bbs2;
	BREC oldbrec;
	sprintf(brecfilename,"%s/menurec/%s.bre",club_go.dir,go.code);
	if ((fp=fopen(brecfilename,"r"))==NULL) {
		brec.newnum=0;
		brec.allnum=0;			  //ÀüÃ¼°Ô½Ã¹°¼ö
		brec.numdate=0; 	   //°Ô½ÃÆÇÁ¤¸®ÃÖÁ¾³¯Â¥
		mode=3;
	}
	else {
		fread(&brec,sizeof_brec,1,fp);
		fclose(fp);
	}
	oldbrec=brec;
	if (mode==0) return;
	oldmode=mode;
	if (oldmode==8||oldmode==9) mode=1;    //1ÀÚ·á Ãß°¡,»èÁ¦ÀÏ¶§
	else if (mode==1)		 mode=2;		//°£´ÜÁ¤¸®
	if (is_old_day(brec.numdate)!=0) mode=3;		//ÀüÃ¼Á¤¸®
	if ((fp=fopen(NOW_FILE,"r"))==NULL) {unlink(brecfilename);return;}
	rec=count_frec(fp,sizeof_bbs);			   //Á¦ÀÏ ¸¶Áö¸· ÀÚ·á¸¦ Ã£±â
	ii=rec;
	count=0;
	if (mode==2) {
		brec.newnum=0;
		while (count++<500000&&ii>0) {
			fseek(fp,(ii-1)*sizeof_bbs,SEEK_SET);
			if (!fread(&bbs2,sizeof_bbs,1,fp)) break;
			if (is_old_day(bbs2.date)) break;  //³¯Â¥°¡ ´Ù¸£¸é
			if (!bbs2.del) brec.newnum++;
			ii--;
		}
		if (oldmode==8) brec.allnum++;
		if (oldmode==9) brec.allnum--;
	}
	else if (mode==3) {
		brec.allnum=0;
		brec.newnum=0;
		time(&brec.numdate);
		fseektop(fp);
		while (count++<500000&&fread(&bbs2,sizeof_bbs,1,fp)) {
			if (!bbs2.del) {
				brec.allnum++;
				if (!is_old_day(bbs2.date)) brec.newnum++;
			}
		}
	}
	fclose(fp);

	_cont:
	check_push_oldbrec();  //Áö³­´ÞÀÚ·áÀÏ¶§ Áö³­´ÞÀÚ·á¸¦ ÀÌÀü´Þ·Î ³Ñ±ä´Ù.
	if ((fp=fopen(brecfilename,"w+"))!=NULL) {
		fwrite(&brec,sizeof_brec,1,fp);
		fclose(fp);
	}
	sync();fflush(NULL);
}




check_push_oldbrec()	//Áö³­´ÞÀÚ·áÀÏ¶§ Áö³­´ÞÀÚ·á¸¦ ÀÌÀü´Þ·Î ³Ñ±ä´Ù.
{
	if (yearmonth(brec.numdate2)==now_yearmonth()) return;
	brec.numdate4 =brec.numdate4 ;
	brec.time4	  =brec.time3	 ;		//ÀüÃ¼ »ç¿ë½Ã°£(ÃÊ)
	brec.timebill4=brec.timebill3;		//½Ã°£º° ¿ä±Ý´©Àû
	brec.size4	  =brec.size3	 ;		//ÀüÃ¼ »ç¿ëSIZE(KB)
	brec.sizebill4=brec.sizebill3;		//SIZEº° ¿ä±Ý´©Àû
	brec.numdate3 =brec.numdate2 ;
	brec.time3	  =brec.time2	 ;		//ÀüÃ¼ »ç¿ë½Ã°£(ÃÊ)
	brec.timebill3=brec.timebill2;		//½Ã°£º° ¿ä±Ý´©Àû
	brec.size3	  =brec.size2	 ;		//ÀüÃ¼ »ç¿ëSIZE(KB)
	brec.sizebill3=brec.sizebill2;		//SIZEº° ¿ä±Ý´©Àû
	time(&brec.numdate2);
	brec.time2	  =0;
	brec.timebill2=0;
	brec.size2	  =0;
	brec.sizebill2=0;
}


bbs2tmp()	 //ÇöÀçBBSÀÇ ³»¿ëÀ» TMP·Î ÀúÀå
{
	FILE *fp1,*fp2;
	int i;
	char c;
	unlink(TMP_FILE);
	if ((fp1=fopen(NOW_FILED,"r+"))==NULL) return;
	if ((fp2=fopen(TMP_FILE ,"w+"))==NULL) return;
	fseek(fp1,bbs.posi,SEEK_SET);
	for(i=0;i<bbs.size&&!feof(fp1);i++) {
		c=fgetc(fp1);
		if (c==0||(byte)c==255) break;
		fputc(c,fp2);
	}
	fclose(fp1);
	fclose(fp2);
}


get_ansicolor(char *buff, int n1,int n2)
{
	sprintf(buff,"[=%dF[=%dG",n1,n2);
}




char *get_gool(int mark)
{
	static char buff[20];
	int i;
	strcpy(buff,"");
	if (mark<=' ') return (buff);
	for (i=0;i<10;i++) {
		if ((char)acolor[i].mark==(char)mark) {
			if (acolor[i].color_char==0&&acolor[i].color_back==0) return(buff);
			get_ansicolor(buff,acolor[i].color_char,acolor[i].color_back);
//			printf("%c %s %d %d",mark,buff,acolor[i].color_char,acolor[i].color_back);
//			pressenter();
			return (buff);
		}
	}
	return (buff);
}



char *gd_str(int mark)
{
	static char buff[20];
	strcpy(buff,"");
	if (mark<=' ') return (buff);
	if (bit(cfg.bbsmode,1)) return(buff);	 //µ¿ÀÛÁßÁö
	strcpy(buff,get_gool(mark));
	return (buff);
}


int exec_pid;
void sig_exec_alarm(int sig)
{
	kill(exec_pid,SIGKILL);
}

save_rec(int mode,int job,char *buff)
{
	char filename[300];
	char buff2[300];
	char stty[10];
	char shostname[40];
	if (bit(allmode,29)) return;
	if (bit(allmode,30)) return;
    if (strlen(buff)<3) return;
	strcpy(shostname,hostname);shostname[30]=0;del_space(shostname);
	if (strlen(shostname)<1) strcpy(shostname,".");
	strcpy(stty,&tty[3]);strcat(stty,"    ");stty[4]=0;
	if (job<0||job>24) job=24;
	if (strlen(go.code)<1) strcpy(go.code,".");
	if (strlen(club_go.code)<1) strcpy(club_go.code,".");
	if (strlen(pf.id)<1) strcpy(pf.id,".");
	sprintf(filename,"%s/rec_%s.txt",REC_DIR,now_str(12));
	sprintf(buff2,"%s %3d %s %-8s %2d %-8s %-8s %2d %-4s%d %-30s %s\n",
	now_str(3),cfg.hostno,stty,pf.id,pf.type,club_go.code,go.code,job,awork[job>24?24:job],mode,shostname,buff);
	del_esc(buff2);
    if (!bit(cfg.mrecmode,4)) put_line(buff2,filename);
}


is_oldtty(char *dtty,int sec)
{
	char ttyfile[100];
	struct stat stbuf;
	struct utimebuf timestamp,*times=&timestamp;
    if (is_same(dtty,"www")) return No;
	sprintf(ttyfile,"/dev/%s",dtty);
    if (strlen(dtty)==0||stat(ttyfile,&stbuf)==-1) return Yes;
	times->actime =stbuf.st_atime;
	times->modtime=stbuf.st_mtime;
	if (how_old(stbuf.st_mtime)>sec) return Yes;
	utime(ttyfile,times);
	return No;
}


calc_logcount(int year,int month,int day,int hour)
{
    FILE *fp;
    time_t t;
    struct tm *tp;
    int i,n=0;
    char filename[100];
    char buff[100];
    time(&t);
    if (strlen(subkey4)>0&&is_char(subkey4,'-')) {
        if (strlen(subkey4)==7) {
            strcpy(buff,subkey4);
            strcat(buff,"-10");
            t=str2date(buff);
            tp = localtime(&t);
            t=str2date(subkey4);
            year=tp->tm_year%100;
            month=tp->tm_mon+1;
        }
        else if (strlen(subkey4)==10) {
            t=str2date(subkey4);
            tp = localtime(&t);
            year=tp->tm_year%100;
            month=tp->tm_mon+1;
            day=tp->tm_mday;
        }
    }
    if (hour >=30)   {      //ÃÖ±Ù?½Ã°£ Åë°è
        if (day<=1) {
            t-=(hour-30)*60*60;
            tp = localtime(&t);
            year=tp->tm_year%100;
            month=tp->tm_mon+1;
            day=tp->tm_mday;
            hour=tp->tm_hour;
        }
        else if (day==2) {    //ÃÖ±Ù?ÀÏ Åë°è
            t-=(hour-30)*24*60*60;
            tp = localtime(&t);
            year=tp->tm_year%100;
            month=tp->tm_mon+1;
            day=tp->tm_mday;
            hour=-1;
        }
        else if (day==3) {    //ÃÖ±Ù?¿ù Åë°è
            month=now_month()-(hour-30);
            year=now_year()%100;
            while (month<1) {month+=12;year--;}
            if (year<0) year+=100;
            day=0;
            hour=-1;
        }
    }
    if (year <  0)   year=now_year();
    if (month==-2)   month=now_month();
    if (day  ==-2)   day=now_day();
    if (hour ==-2)   hour=now_hour();
    if (month<  1)   {month=0;day=0;hour=-1;}
    if (day  <  1)   {day=0;hour=-1;}
    if (hour <  0)   hour=-1;

    n=0;
    sprintf(filename,"user/rec/count%02d.dat",year%100);
    if ((fp=fopen(filename,"r"))!=NULL) {
        i=(month-1)*31*25+day*25+hour+1+1;
        if (month==0||i<0) i=0;
        fseek(fp,i*sizeof(n),SEEK_SET);
        fread(&n,sizeof(n),1,fp);
//        printf("%s %d %d %d %d ->%u",filename,month,day,hour,i,n);pressenter();
        fclose(fp);
    }
//    printf("%s %d",filename,file_size(filename));
    if (n<0) n=0;
    return n;
}


check_cmd(char *buffstr)
{
	int i,i1,i2,i3;
	int n1,n2;
	int first,lastno;
	int mmt[301];
	char c;
	char buff[220];
	char buff2[220];
	char mms[70][200];

	//ÃÖ±âÈ­
	for (i=0;i<100;i++) mmn[i]=0;
	for (i=0;i<10;i++) strcpy(mmc[i],"");
	for (i=0;i<301;i++) mmt[i]=0;

	if (buffstr[0]==0||strlen(buffstr)>170) return;
	strcpy(buff,buffstr);
	i=i1=i2=i3=n1=n2=0;
	first=0;

	//¸í·É->mms·Î ºÐÇØ
	for (i=0;i<30;i++) bzero(mms[i],200);
	i1=i2=0;
	for (i=0;i<(int)strlen(buffstr)&&i1<30;i++) {
		c=buffstr[i];
		if (c==0||c=='\n'||c=='\r') break;
		else if (c==' '||c==',') {i1++;i2=0;}
		else mms[i1][i2++]=c;
	}

	//mms¸¦ ¼ýÀÚ·Î ºÐÇØ
	first=0;
	for (i=0;i<30&&first<300;i++) {
		strcpy(buff,mms[i]);
		del_space(buff);
		if (buff[0]==0) continue;
		if(is_char(buff,'-')) {        // - Æ÷ÇÔ¿©ºÎ    -10 10-20  20-
			if (strlen(buff)<2);	   // -ÇÑÀÚ¸é ¹«½Ã
			else if (buff[0]=='-') {     //  -??
				n2=atoi(buff+1);
				for (i2=1;i2<=n2&&first<300;i2++) mmt[first++]=i2;
			}
			else if (buff[strlen(buff)-1]=='-') {     //  ??-
				strchng(buff2,buff,"-","");
				n1=atoi(buff2);if(n1<1) n1=1;
				while (n1>=0&&first<300) mmt[first++]=n1++;
			}
			else {								 //??-??
				n1=atoi(buff);
				n2=0;
				i2=0; while (buff[i2]) if (buff[i2++]=='-') n2=atoi(buff+i2);
				if (n1<1) n1=1;
				if (n2<1) n2=1;
				if (n1<=n2) for (i2=n1;i2<=n2&&first<300;i2++) mmt[first++]=i2;
				else		for (i2=n2;i2<=n1&&first<300;i2++) mmt[first++]=i2;
			}
		}
		else mmt[first++]=atoi(buff);
	}


	if (is_bbspds) {
		for (i=1;i<15;i++) {
			if (is_same(mms[i],"con")) {
				for (i2=0;i2<50;i2++) {
					mmt[i2+1]=list_anum[i2];
					sprintf(mms[i+i2],"%d",mmt[i2+1]);
				}
				break;
			}
			else if (is_same(mms[i],"rcon")) {
				for (i2=0;i2<50;i2++) {
					mmt[i2+1]=list_anum[49-i2];
					sprintf(mms[i+i2],"%d",mmt[i2+1]);
				}
				break;
			}
		}
	}
	//0 ¶Ç´Â °°Àº¼ýÀÚ°¡ ³ª¿Â°ÍÀº ¹«½ÃÇÑ´Ù.
	//mmt -> mmnÀ¸·Î
	for (i=0;i<101;i++) mmn[i]=0;
	mmn[0]=mmt[0];	   //Ã¹¹øÈ£´Â ¹«Á¶°Ç
	first=1;
	for (i=1;i<300&&first<101;i++) {
		if (mmt[i]>0) {
			for (i2=0;i2<101;i2++) if (mmt[i]==mmn[i2]) break;
			if (i2>=101&&mmt[i]>=0) mmn[first++]=mmt[i];
		}
	}
	//°°Àº ¹®ÀÚ°¡ ³ª¿Â°ÍÀº ¹«½ÃÇÑ´Ù.
	//mms -> mmc·Î
	first=0;
	for (i=0;i<30&&first<100;i++) {
		mms[i][20]=0;
		if (mms[i][0]!=0) {
			for (i2=0;i2<10;i2++) if (is_same(mmc[i2],mms[i])) break;
			if (i2>=10) strcpy(mmc[first++],mms[i]);
		}
	}
	strlwr(mmc[0]);    //Ã¹¹øÂ° ¸í·ÉÀº ¼Ò¹®ÀÚ·Î
}


view_str_file(char *filename,char *buffstr) //¹®ÀÚ¿­Æ÷ÇÔµÈ ÀÚ·áº¸±â
{
	FILE *fp;
	int count,i;
	char buff[300],buff1[300];
	if (strlen(filename)<1||strlen(buffstr)<1) return No;
	if ((fp=fopen(filename,"r"))==NULL) {
		mess("%s ¿­±â½ÇÆÐ",filename);
		return No;
	}
	count=0;
	while (fgets(buff,200,fp)!=NULL) {
		if (stristr(buff,buffstr)) {
			printf(buff);
			count++;
			if (count%20==19) {
				if (!yesno(" ### Ãâ·Â °è¼Ó (Y/n)>>",Yes)) {fclose(fp);return Yes;}
				printf("[30D[2K");
			}
		}
	}
	fclose(fp);
	if (count<1) {
		printf("\n ### ÇØ´ç ÀÚ·á°¡ ¾ø½À´Ï´Ù.");
		pressenter();
		return No;
	}
	printf("\n ### ÀÚ·áÀÇ ³¡ÀÔ´Ï´Ù.");
	pressenter();
	return Yes;
}


clear_pf()
{
	pf.istat=0;
	pf.itype=0;
	pf.loginmode=0;
	pf.offmin=0;
	pf.warn=0;
	pf.moon=0;
	pf.sex=0;
	pf.type=0;
	pf.wmoon=0;
	pf.all_log=0;
	pf.all_time=0;
	pf.cash=0;
	pf.cashmode=0;
	pf.down=0;
	pf.good=0;
	pf.countd=0;
	pf.countu=0;
	pf.countw=0;
	pf.idnum=0;
	pf.left_time=0;
	pf.level=0;
	pf.month_log=0;
	pf.month_time=0;
	pf.myflag=0;
	pf.mymode=0;
	pf.num=0;
	pf.paymode=0;
	pf.today=0;
	pf.birthday=0;
	pf.inday=0;
	pf.login=0;
	pf.logout=0;
	pf.job=0;
	pf.left_time=0;
	pf.p_ph=pf.p_it=pf.p_at=pf.p_pr=pf.p_mo=0;
	pf.marryday=0;
	strcpy(pf.add2	   ,"");
	strcpy(pf.addr	   ,"");
	strcpy(pf.bibi	   ,"");
	strcpy(pf.email    ,"");
	strcpy(pf.gofirst  ,"");
	strcpy(pf.hand	   ,"");
	strcpy(pf.id	   ,"");
	strcpy(pf.id_no    ,"");
	strcpy(pf.mailid   ,"");
	strcpy(pf.memo1    ,"");
	strcpy(pf.name	   ,"");
	strcpy(pf.nick	   ,"");
	strcpy(pf.offi	   ,"");
	strcpy(pf.otel	   ,"");
	strcpy(pf.passwd   ,"");
	strcpy(pf.pos1	   ,"");
	strcpy(pf.pos2	   ,"");
	strcpy(pf.tele	   ,"");
	strcpy(pf.user1    ,"");
	strcpy(pf.user2    ,"");
	strcpy(pf.user3    ,"");
	strcpy(pf.user4    ,"");
	strcpy(pf.wifeid   ,"");
}


clear_bbs()
{
	bbs.del=0;
	bbs.read=0;
	bbs.down=0;
	bbs.gd=' ';
	bbs.line=0;
	strcpy(pf.id,"");
	strcpy(pf.name,"");
	strcpy(bbs.title,"");
}


calc_stotal(int mode)
{
	FILE *fp;
	int i,n,found;
	MONI moni2;
	char filename[100],buff[200],buff1[200],buff2[200];
	char ahoststr[100][30];
	for (i=0;i<60;i++) a_num[i]=0;
    for (i=0;i<60 ;i++) strcpy(ahoststr[i],"");

	if ((fp=fopen("system/iplist.txt","r"))!=NULL) {
		i=0;
		while (i<40&&fgets(buff2,200,fp)!=NULL) {
			del_enter(buff2);
			del_space(buff2);
			strcpy(ahoststr[i],buff2);
			i++;
		}
		fclose(fp);
	}

	for (n=0;n<20;n++) {
		sprintf(filename,"user/moni/moni%d.dat",n);
		if ((fp=fopen(filename, "r"))==NULL) continue;
		while (fread(&moni2,sizeof_moni,1,fp)) {
			if (moni2.mode<1||strlen(moni2.id)<3||strlen(moni2.tty)<1) continue;
			a_num[40]++;
            for (i=0;i<20;i++) {
				if (cfg.nip[i]==moni2.hostno) {
					a_num[i]++;
					break;
				}
			}
				 if (moni2.type>=18) a_num[44]++;
			else if (moni2.type>=14) a_num[43]++;
			else if (moni2.type>=1 ) a_num[42]++;
			else if (moni2.type==0 ) a_num[41]++;

			found=No;
            for (i=0;i<20;i++) {
				if (is_same(ahoststr[i],moni2.hostname)) {
					a_num[51+i/5]++;
					found=Yes;
					break;
				}
			}
			if (!found) a_num[50]++;
		}
		fclose(fp);
	}
}



//ÆÄÀÏ·Î ÇÏ´Â c_lock
c_lock_file(int mode,char *filename)
{
	char buff[300];
	char lock_filename[300];
	int i,n;
	strcpy(buff,filename);del_esc3(buff);del_space(buff);
	strchng2(buff,"/","");strchng2(buff,".","");
	if (strlen(buff)<1) return Yes;
	sprintf(lock_filename,"user/lock/%s",buff);
	if (mode==0) goto make_chlock3;
	if (mode==1&&!is_file(lock_filename)) goto make_chlock1;
	else if (mode==1||mode==2) {
		if (is_old_file(lock_filename,300)) goto make_chlock2;
		n=40;if (mode==2) n=1000;
        for (i=0;i<50;i++) {
			if (!is_file(lock_filename))  goto make_chlock1;
			if (i==0&&mode==2) {
				printf(" Áö±Ý µ¥ÀÌÅ¸ °»½Å ÁßÀÔ´Ï´Ù. Àá½Ã¸¸ ±â´Ù·Á ÁÖ¼¼¿ä..\n");
				if (pf.type>=18) printf("LOCK:%s\n",filename);
			}
            msleep(1);
		}
		goto make_chlock2;
	}
	return Yes;
	make_chlock1:
	create_file(lock_filename);
	sync();fflush(NULL);
	return Yes;
	make_chlock2:
	unlink(lock_filename);
	create_file(lock_filename);
	sync();fflush(NULL);
	return No;
	make_chlock3:
	unlink(lock_filename);
	sync();fflush(NULL);
	return Yes;
}


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUF_LEN 128
#define LOCK_PORT 4000

int c_lock(int mode,char *filename) {
    int i;

	if (bit(cfg.mprocmode,1)) return 0; //LOCK»ç¿ë¾ÈÇÔ
    if (strlen(cfg.lock_ip_addr)<=1||!bit(cfg.mprocmode,3))
        return c_lock_file(mode,filename); //ÆÄÀÏLOCK

    for(i=0; i < 50; i++) {
        if(c_lock_exec(mode,filename)) break;
//		  printf("±â´Ù¸² %d\n",i);
        msleep(1);
	}
    return No;
}

//xlockd µ¥¸ó
int c_lock_exec(int mode, char *filename)
{
		int i,s,n,len_in,len_out,count;
		struct sockaddr_in server_addr;
		char haddr[20];
		char IP_ADDRESS[20];
		char buf[BUF_LEN+1], result[BUF_LEN+1];

	strcpy(haddr,cfg.lock_ip_addr);
	if((s=socket(PF_INET,SOCK_STREAM,0)) < 0) {
//		printf("can't create socket\n");
		return -1;
	}
	bzero((char *)&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(haddr);
	server_addr.sin_port=htons(LOCK_PORT);

	if(connect(s,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        cfg.mprocmode=set_bit(cfg.mprocmode,3,0);
//		printf("can't connect\n");
		return -1;
	}

    if(mode == 0) sprintf(buf,"after,%s",filename);
    else          sprintf(buf,"before,%s",filename);
    if(write(s,buf,strlen(buf)) < 0) {
//		printf("write error\n");
        return -1;
	}
    //LOCKÇ®¶§ ¹Ù·Î ³¡³¿
    if (mode==0) {
        close(s);
        return 1;
    }
//	printf("W '%s'\n",buf);
    //LOCKÀÏ¶§ ¼º°ø¿©ºÎ °Ë»ç
    n = 0;
    count=0;
    while(count++<30) {
        if((n=read(s,result,sizeof(result))) < 0) {
//          printf("read error\n");
            return -1;
        }
        if(n > 0) break;
        msleep(1);
        printf("wait...\n",count);
    }
    result[n] = '\0';
//  printf("R '%s'\n",result);
    if(strcmp(result,"true")==0) {
        close(s);
        return 1;       //¼º°ø.. (¾²±â°¡´É)
    }
	close(s);
    return 0;       //´ë±âÇÏ¶ó..
}


//file1->file2 ¼ÒÆÃ
xsort(int mode,char *file1,char *file2)
{
    char buff[500];
    if (strlen(file1)<1||strlen(file2)<1||is_same(file1,file2)) return;
    unlink(file2);
    if (mode==0) sprintf(buff,"sort '%s' -o '%s'",file1,file2);
    else         sprintf(buff,"sort -r '%s' -o '%s'",file1,file2);
    system(buff);
}


gwrite(int mode,PF pf,FILE *fp)
{
    pf.marryday=0;
    strcpy(pf.sp_1,"");
    sprintf(pf.sp_1,"%d '%s' %d '%s' ",mode,_last1,_last2,go.code);
    if (pf.type<0||pf.type>19) {
        time(&pf.marryday);
        sprintf(pf.sp_1,"%3d '%s' %d '%s' ",mode,_last1,_last2,go.code);
        put_line(pf.sp_1,"user/rec/err.rec");
    }
    return fwrite(&pf,sizeof_pf,1,fp);
}

int cut_badhan(unsigned char *str, int len)
{   
    register int i;
	str[len] = '\0';
    for(i=0; i < len || i < (int)strlen(str); i++) {
		if(str[i] > 127) {
			if(str[i+1] > 127) 
				i++;
			else {
				str[i] = '\0';
				return 0;
			}   
		}   
	}   

	return 1;
}   
