////////////////////////////////////////////////////////////////////////
//°³¹ß : ±è¼º´ë  xhost@xhost.co.kr (016-320-7882)
// ³»¿ë : ¸Þ´º °ü·Ã ·çÆ¾
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

int nn;
int top;
int total;
int last;
int key;
int all_view;

char filename[200];
char now_code[100];
char now_code[100];
char now_dir[100];
char now_id[9];
char NOW_MENU_OLD[250];
char menu_change=No;
int now_deep;
int job;
int count_menu_num;
int now_local;
GO subgo;

//¸Þ´º¸¦ ¼öÁ¤ÇÑ´Ù.
main(int argc, char **argv)
{
	FILE *fp;
	int n,i,i2,y,linecount;
	char buff[255];
	char buffn[50];
	char buff0[50];
	char buff1[50];
	char buff2[50];
	char tid[9];
	char tclub[9];
	int  canforwd=Yes;
	BBS bbs;

	printf("[01;39r");
	read_cfg();
	findport(tty);
	set_tmpfile();
	now_local=0;
	if (argc<3) {
		strcpy(now_id,cfg.sysop);
		strcpy(tclub,"main");
	}
	else {
		strcpy(now_id,argv[1]);
		strcpy(tclub,argv[2]);
		if (argc>3&&is_same(tclub,"main")&&bit(cfg.gomode,11))
			now_local=atoi(argv[3]);
	}
	if (is_same(now_id,"root")) pf.type=19;
	else if (!read_pf(now_id)) {mess("ID:%s not found",now_id);return;}
	if (!find_club(tclub)) {mess("CLUB:%s not found",tclub);return;}
	if (club_mode) now_local=0;
	set_nowmenu();
	check_topmenu();  //¸Þ´º0ÀÌ topÀÌ ¾Æ´Ï¸é topÀ¸·Î ÁöÁ¤
	check_dir("%s/system",club_go.dir);
	check_dir("%s/menustru",club_go.dir);
	check_dir("%s/menucfg",club_go.dir);
	check_dir("%s/menurec",club_go.dir);

	if (now_local>0) {
		sprintf(buff,"menustru/%d",now_local);check_dir(buff);
		sprintf(buff,"menustru/%d/menu.dat",now_local);
	}
	else sprintf(buff,"%s/menustru/menu.dat",club_go.dir);touch_file(buff);
	sprintf(NOW_MENU,"%s/menustru/menu%d.old",club_go.dir,now_day()%10);
	if (!file_copy(buff,NOW_MENU)) return;		 //ÀÓ½ÃÆÄÀÏ·Îº¹»ç

	all_view=No;	 //ÀüÃ¼¸Þ´º º¸±â
	set_postop();
	count_menu_num=scount_menu();

	while (1) {
		if (subgo.deep<0) subgo.deep=0;if (subgo.deep>99) subgo.deep=99;
		if (now_deep<0)   now_deep=0;  if (now_deep>99)   now_deep=99;
		if (last<1) last=1; 		//¸¶Áö¸·¿¡ ÆíÁýÇÑ ¹øÈ£
		if (top<1) top=1;
		if (count_menu_num>1&&top>=count_menu_num) {
			read_gobackwd();
			continue;
		}
		i2=0;
		re_disp:
		cls();
		printf("[H[s[1;37;44m[2J");
		printf("[7m[2K ¸Þ´º:%-8s  ±íÀÌ:%-2d TOP:%d (Ç×¸ñ:%d %dK) LOCAL:%d %s[0;1m\n",
			subgo.code,now_deep,top,count_menu_num,file_size(NOW_MENU)/1024,now_local,all_view?"(ÀüÃ¼º¸±â)":"");
		printf("NO[35m±íÀÌ[32mÅ¸ÀÔ[33m¼±ÅÃ[36m¸Þ´ºÄÚµå[37m¸Þ´ºÁ¦¸ñ                  [35mÁ¦ÇÑ[37mµð·ºÅä¸®\n");
		printfL2();
		linecount=0;
		fp=fopen(NOW_MENU,"r");
		fseek(fp,top*sizeof_go,SEEK_SET);
		n=top;
//		  canforwd=Yes;
		while(!feof(fp)&&linecount<16&&fread(&go,sizeof_go,1,fp)) {
			if (n==0) set_gotop();
			n++;
			if (all_view);				 //ÀüÃ¼¸Þ´ºº¸±â
			else if (go.deep<now_deep) break;
			else if (go.deep>now_deep) continue;
			disp_line(0,n);
			linecount++;
		}
		fclose(fp);
//		  if (linecount<16) canforwd=No;

		xy(1,20);
		printf(" ¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬\n");
		printf("°Ë»ö(S) ¼öÁ¤(E,ED,EE) »èÁ¦(D) ÀÌµ¿(M) Ãë¼Ò(Q) ¹üÀ§(U) µµ¿ò(H) µî·Ï(UP) ´Ù¿î(DN)\n");
		printf("ÀüÃ¼(G) ÀÌÀü(B) ´ÙÀ½(F) »óÀ§(P) Ãß°¡(A) »ðÀÔ(I) ÀúÀå(W) ÀüÃ¼ÀÚ·áÆíÁý(Z) Á¤¸®(R)\n");
		printf("±íÀÌ(DP,D1,D2) ¸Þ´º»ðÀÔ(MI) ¸Þ´ºÃßÃâ(MW) TEXT´Ù¿î(TD) TEXT¾÷(TU) »óÅÂ(?) (PACK)\n");
		if (club_mode) printf("¼±ÅÃ¹øÈ£(SN) µð·ºÅä¸®¸¸µé±â(MK)                                                \n");
		else		   printf("¼±ÅÃ¹øÈ£(SN) µð·ºÅä¸®¸¸µé±â(MK) ·ÎÄ®¸Þ´º(LO)                                   \n");
		printf("¼±ÅÃ >>");
		lineinput(buff,80);check_han(buff);check_cmd(buff);printf("\n");
		if (!is_same(buff,"p")&&is_quit(buff)) strcpy(mmc[0],"q");
		if (is_char("diam",buff[0])&&mmn[1]>0) last=mmn[1];
		nn=mmn[1];
		if (!menu_change&&strlen(mmc[0])>0&&stristr("e ed a d dd up dp mi mw",mmc[0])) menu_change=Yes;
		if (is_same(buff,"0")) nn=0;
		else if (is_same(mmc[1],"0")) nn=0;
		else if (nn==0) {
			if (is_same(mmc[0],"e")) nn=last+1;        //¹øÈ£¸¦ ¼±ÅÃÇÔ
			else					 nn=last;		 //¹øÈ£¸¦ ¼±ÅÃÇÔ
		}
			 if (is_same(mmc[0],"e"))  sedit_menu(1,nn);
		else if (is_same(mmc[0],"ed")) sedit_menu(2,nn);
		else if (is_same(mmc[0],"ee")) sedit_menu(3,nn);
		else if (is_same(mmc[0],"d")) sdel_menu(nn);
		else if (is_same(mmc[0],"i")) sins_menu(nn);
		else if (is_same(mmc[0],"a")) sadd_menu();   //ÇöÀç±íÀÌ¿¡ Ãß°¡
		else if (is_same(mmc[0],"u")) sudel_menu(mmn[1],mmn[2]);
		else if (is_same(mmc[0],"m")) smove_menu(mmn[1],mmn[2]);
		else if (is_same(mmc[0],"s")) search_menu();
		else if (is_same(mmc[0],".def")) edit_def(atoi(mmc[1]));
		else if (is_same(mmc[0],"h")) view_text("frm/help/menu.hlp");
		else if (is_same(mmc[0],"q")) {
			if (!menu_change||yesno("ÀúÀåÀ» Ãë¼ÒÇÏ°í ÀÛ¾÷À» ³¡³¾±î¿ä? (Y/n)",Yes)) break;
		}
		else if (is_same(mmc[0],"xx")||is_same(mmc[0],"w")||is_same(mmc[0],"x"))  {
			check_menu();
			//¸Þ´ºÆÄÀÏ·Î ÀúÀå
			if (now_local>0) {
				sprintf(buff,"menustru/%d",now_local);check_dir(buff);
				sprintf(buff,"menustru/%d/menu.dat",now_local);
			}
			else sprintf(buff,"%s/menustru/menu.dat",club_go.dir);
			copy_file_menu(NOW_MENU,buff);
			file_copy(NOW_MENU,buff);
			strcpy(NOW_MENU,buff);
			make_goindex(NOW_MENU);
			reset_menuind();	 //¸Þ´ºSUB TAG ÀçÁ¤¸®
			make_menu_index();	 //ÀÎµ¦½º »õ·Î Á¤¸®
			break;
		}
		else if (is_same(mmc[0],"t")) set_postop();
		else if (is_same(mmc[0],"p")) read_goprev();
		else if (is_same(mmc[0],"b")) read_gobackwd();
//		  else if (is_same(mmc[0],"f")&&canforwd) read_goforwd();
		else if ((strlen(mmc[0])==0||is_same(mmc[0],"f"))) read_goforwd();
		else if (is_same(mmc[0],"r")) pack_menu();
		else if (is_same(mmc[0],"z")) xedit("%s/system/zbtotal.dat",club_go.dir);   //ÇöÀç±íÀÌ¿¡Ãß°¡
		else if (is_same(mmc[0],"g")) all_view=not(all_view);
		else if (is_same(mmc[0],"dn")) sdn_menu();
		else if (!club_mode&&is_same(mmc[0],"lo")) set_local_number();
		else if (is_same(mmc[0],"up")) sup_menu();
		else if (is_same(mmc[0],"dp")) set_deep(0);
		else if (is_same(mmc[0],"d1")) set_deep(1);
		else if (is_same(mmc[0],"d2")) set_deep(2);
		else if (is_same(mmc[0],"td")) menu_text(1);
		else if (is_same(mmc[0],"tu")) menu_text(2);
		else if (is_same(mmc[0],"sn")) set_selnum(2);
		else if (is_same(mmc[0],"mi")) menu_insert();
		else if (is_same(mmc[0],"mw")) menu_write();
		else if (is_same(mmc[0],"mk")) menu_dir();
		else if (is_same(mmc[0],"pack")) pack_menu();
		else if (is_same(mmc[0],"?")) disp_menustat();
		else if (mmn[0]>0||is_same(mmc[0],"0")) set_gopos(mmn[0]); //ÇØ´ç¹øÈ£Ãâ·Â
		else if (strlen(mmc[0])<1)	  read_goforwd();
		if (is_char("tmedail",mmc[0][0])) count_menu_num=scount_menu();
	}
}


set_local_number()
{
	FILE *fp,*fp2;
	int num,i,n,n2,nn,count;
	char buff[100];
	n=n2=0;
	while (1) {
		cls();
		print_(0,"·ÎÄ®¸Þ´ºÁöÁ¤");
		if (n2<1) sprintf(buff,"menustru/menu.dat");
		else	  sprintf(buff,"menustru/%d/menu.dat",n2);
		printf("   1. ·ÎÄ®¸Þ´º -> ÇöÀç¸Þ´º ºÒ·¯¿À±â (%3d :%10u Byte :%4d )\n",now_local,file_size(NOW_MENU),file_size(NOW_MENU)/sizeof_go);
		printf("   2. ÇöÀç¸Þ´º -> ·ÎÄ®¸Þ´º ÀúÀåÇÏ±â (%3d :%10u Byte :%4d )\n",n2,file_size(buff),file_size(buff)/sizeof_go);
		printf("   3. ·ÎÄ®¸Þ´º -> ÇöÀç¸Þ´º Ãß°¡ÇÏ±â               \n");
		printf("   4. ·ÎÄ® ¸Þ´º ¼³Á¤\n");
		num=numselect();printf("\n");
		if (num<= 0) break;
		if (num== 1) {
			if (menu_change&&yesno("º¯°æ³»¿ëÀ» Ãë¼ÒÇÒ±î¿ä?(y/N)>>",No)) continue;
			printf(" ºÒ·¯¿Ã ·ÎÄ®¸Þ´º¹øÈ£?>>");now_local=get_num(now_local,3);printf("\n");
			if (now_local<1) sprintf(buff,"menustru/menu.dat");
			else			 {
				sprintf(buff,"menustru/%d/",now_local);check_dir(buff);
				sprintf(buff,"menustru/%d/menu.dat",now_local);touch_file(buff);
			}
			touch_file(buff);
			file_copy(buff,NOW_MENU);
			now_deep=0;
			top=0;
			set_postop();
			count_menu_num=scount_menu();
		}
		else if (num== 2) {
			printf(" ÀúÀåÇÒ ·ÎÄ®¸Þ´º¹øÈ£?>>");n2=get_num(n2,3);printf("\n");
			if (n2<1) sprintf(buff,"menustru/menu.dat");
			else	  {
				sprintf(buff,"menustru/%d/",n2);check_dir(buff);
				sprintf(buff,"menustru/%d/menu.dat",n2);
			}
			touch_file(buff);
			file_copy(NOW_MENU,buff);
		}
		else if (num== 3) {
			//ÇöÀç ¹øÈ£ ´ÙÀ½ÆÄÀÏ ÀÓ½Ã ÀúÀå
			printf(" Ãß°¡ÇÒ ·ÎÄ®¸Þ´º¹øÈ£?>>");n2=get_num(n2,3);printf("\n");
			if (n2<1) sprintf(buff,"menustru/menu.dat");
			else	  sprintf(buff,"menustru/%d/menu.dat",n2);
			nn=count_rec(NOW_MENU,sizeof_go);
			printf(" »ðÀÔÇÒ ¸Þ´º À§Ä¡´Â?>>");nn=get_num(nn,4);printf("\n");
			fp=fopen(NOW_MENU,"r+");
			//ÇöÀç ¹øÈ£ ´ÙÀ½ÆÄÀÏ ÀÓ½Ã ÀúÀå
			fp2=fopen(TMP_FILE2,"w");
			if (fp2==NULL) {fclose(fp);return;}
			fseek(fp,(nn+1)*sizeof_go,SEEK_SET);
			while(fread(&go,sizeof_go,1,fp))
				fwrite(&go,sizeof_go,1,fp2);
			fclose(fp2);

			fp2=fopen(buff,"r");
			if (fp2==NULL) {fclose(fp);continue;}
			fseek(fp,(nn+1)*sizeof_go,SEEK_SET);
			count=0;
			while(fread(&go,sizeof_go,1,fp2)) {
				if (strlen(go.code)>0&&go.type>=' '&&go.deep>=0) {
					del_esc3(go.dir);del_space(go.code);
					printf(" %5d  %-8s  %s\n",count+1,go.code,go.name);
					fwrite(&go,sizeof_go,1,fp);
					count++;
				}
			}
			fclose(fp2);
			fp2=fopen(TMP_FILE2,"r");
			if (fp2==NULL) {fclose(fp);return;}
			while(fread(&go,sizeof_go,1,fp2)) {
				fwrite(&go,sizeof_go,1,fp);
			}
			fclose(fp2);
			fclose(fp);
			unlink(TMP_FILE2);
			printf(" ÃÑ %d °³ÀÇ ¸Þ´º°¡ »ðÀÔµÇ¾ú½À´Ï´Ù. \n",count);
			pressenter();
		}
		else if (num==4) sys_tem("bin/pf '%s' set_localmenu",now_id);
	}
}

// È¸¿øÀÎµ¦½º Á¤º¸ÀÌ´Ù.
// 0	1	  2    3		4
//¹øÈ£ Áö¿ª1 Áö¿ª2 ³ªÀÌ½ÃÀÛ  ³ªÀÌÁ¾°á
typedef struct {
	char addr1[21];
	char addr2[21];
	int  age1;
	int  age2;
	int  num;
} LOCAL;


reset_pf_local()
{
	FILE *fp;
	LOCAL alocal[100];
	int i,n,nn,i2,n2,rec;
	char buff[300],buff1[300];
	char mms[10][300];

	for (i=0;i<100;i++) {
		strcpy(alocal[i].addr1,"");strcpy(alocal[i].addr2,"");
		alocal[i].age1=alocal[i].age2=alocal[i].num=0;
	}
	if ((fp=fopen("system/local.txt","r"))==NULL) return;
	nn=0;
	while (nn<100&&fgets(buff1,200,fp)!=NULL) {
		if (strlen(buff1)<10||buff1[0]=='#'||buff1[1]==';') continue;
		for (i=0;i<6;i++) bzero(mms[i],200);
		sscanf(buff1,"%s%s%s%s%s%s",mms[0],mms[1],mms[2],mms[3],mms[4],mms[5]);
		if (strlen(mms[1])<=1&&strlen(mms[2])<=1&&atoi(mms[3])<1&&atoi(mms[4])<1) continue;
		if (strlen(mms[1])>1) sprintf(alocal[nn].addr1,"**%-15.15s",mms[1]);
		if (strlen(mms[2])>1) sprintf(alocal[nn].addr2,"%-20.20s",mms[2]);
		del_space(alocal[nn].addr1);del_space(alocal[nn].addr2);
		alocal[nn].age1=atoi(mms[3]);alocal[nn].age2=atoi(mms[4]);
		alocal[nn].num=atoi(mms[0]);
		nn++;
	}
	fclose(fp);
	if (nn<=0) {
		mess("system/local.txt ¿¡ ·ÎÄ®ºÐ·ùÁ¤º¸°¡ ¾ø½À´Ï´Ù.");
		return;
	}

	if ((fp=fopen(ID_FILE,"r+"))==NULL) return;
	rec=0;
	while((fread(&pf,sizeof_pf,1,fp))) {
		if (pf.type<1||strlen(pf.id)<3) goto _cont3;
		sprintf(buff1,"**%s",pf.addr);del_space(buff1);
		i2=get_age(pf);
		for (i=0;i<nn;i++) {
			if (strlen(alocal[i].addr1)>2&&!stristr(buff1,alocal[i].addr1)) continue;
			if (strlen(alocal[i].addr2)>0&&!stristr(pf.addr,alocal[i].addr2)) continue;
			if (alocal[i].age1>0&&i2<alocal[i].age1) continue;
			if (alocal[i].age2>0&&i2>alocal[i].age2) continue;
			pf.localmenu=alocal[i].num;
			goto _cont2;
		}
		pf.localmenu=0;
		_cont2:
		fseek(fp,pf.num*sizeof_pf,SEEK_SET);
//		  fwrite(&pf,sizeof_pf,1,fp);
		gwrite(251,pf,fp);
		_cont3:
		rec++;
		printf("%d %-8s %-20.20s %s -> %d\n",rec,pf.id,pf.addr,datestr(0,pf.birthday),pf.localmenu);
	}
	fclose(fp);
}


set_deep(int mode)
{
	FILE *fp,*fp2;
	int i;
	if (mmn[1]<1) {
		printf("  ±íÀÌ ÁöÁ¤ »ç¿ë¹ý DP  [±íÀÌ]  [¼±ÅÃ¹øÈ£....]\n");
		printf("  ±íÀÌ Áõ°¡        D+  [¼±ÅÃ¹øÈ£....]\n");
		printf("  ±íÀÌ °¨¼Ò        D-  [¼±ÅÃ¹øÈ£....]\n");
		printf("  ¿¹ DP 2 1-30,33,32\n");
		pressenter();
		return;
	}
	fp=fopen(NOW_MENU,"r+");
	if (mode==0) {
		for (i=2;i<100;i++) {
			if (mmn[i]>0) {
				fseek(fp,mmn[i]*sizeof_go,SEEK_SET);
				if (fread(&go,sizeof_go,1,fp)) {
					go.deep=mmn[1];
					fseek(fp,mmn[i]*sizeof_go,SEEK_SET);
					fwrite(&go,sizeof_go,1,fp);
				}
			}
		}
	}
	else if (mode==1||mode==2) {
		for (i=2;i<100;i++) {
			if (mmn[i]>0) {
				fseek(fp,mmn[i]*sizeof_go,SEEK_SET);
				if (fread(&go,sizeof_go,1,fp)) {
					if (mode==1) go.deep++;
					else if (mode==2&&go.deep>0) go.deep--;
					fseek(fp,mmn[i]*sizeof_go,SEEK_SET);
					fwrite(&go,sizeof_go,1,fp);
				}
			}
		}
	}
	fclose(fp);
}


set_selnum()
{
	FILE *fp,*fp2;
	int i,num;
	if (mmn[1]<1) {
		printf("  ¼±ÅÃ¹øÈ£ ÀÚµ¿ Á¤·Ä  SN [¼±ÅÃ¹øÈ£....]\n");
		printf("  ¿¹ SN 2 1-30,33,32\n");
		pressenter();
		return;
	}
	printf("\n  %d.¸Þ´ººÎÅÍ ÁöÁ¤ÇÒ Ç×¸ñ ¼±ÅÃ ½ÃÀÛ ¹øÈ£¸¦ ÁöÁ¤ÇÏ¼¼¿ä..>>",mmn[1]);
	num=numinput();
	printf("\n");
	if(num<1||num>9999) return;
	fp=fopen(NOW_MENU,"r+");
	for (i=1;i<100;i++) {
		if (mmn[i]>0) {
			fseek(fp,mmn[i]*sizeof_go,SEEK_SET);
			if (fread(&go,sizeof_go,1,fp)) {
				sprintf(go.num,"%d",num++);
				fseek(fp,mmn[i]*sizeof_go,SEEK_SET);
				fwrite(&go,sizeof_go,1,fp);
			}
		}
	}
	fclose(fp);
}


menu_insert()
{
	FILE *fp,*fp2;
	int nn,ret,count=0,key;
	char filename[300];
	if (mmn[1]<1) {
		printf("\n  ¸Þ´º »ðÀÔ ¹æ¹ý MI »ðÀÔÀ§Ä¡\n");
		pressenter();
		return;
	}
	nn=mmn[1];
	printf("\n %d ¹ø¿¡ »ðÀÔÇÒ ¸Þ´ºÆÄÀÏÀ» Àü¼ÛÇØÁÖ¼¼¿ä.. \n",nn);
	ret=up_file(0,filename,No);   //È­ÀÏÀ» Àü¼Û¹Þ¾Æ filename¿¡ ÀúÀåÇÑ´Ù.
	if (!ret||!is_file(filename)) return;

	//ÇöÀç ¹øÈ£ ´ÙÀ½ÆÄÀÏ ÀÓ½Ã ÀúÀå
	fp=fopen(NOW_MENU,"r");
	fp2=fopen(TMP_FILE2,"w");
	if (fp2==NULL) {fclose(fp);return;}
	fseek(fp,(nn+1)*sizeof_go,SEEK_SET);
	while(fread(&go,sizeof_go,1,fp)) {
		fwrite(&go,sizeof_go,1,fp2);
	}
	fclose(fp);
	fclose(fp2);

	fp=fopen(NOW_MENU,"r+");
	fp2=fopen(filename,"r");
	if (fp2==NULL) {fclose(fp);return;}
	fseek(fp,(nn+1)*sizeof_go,SEEK_SET);
	while(fread(&go,sizeof_go,1,fp2)) {
		if (strlen(go.code)>0&&go.type>=' '&&go.deep>=0&&go.deep<=9&&!is_same(go.code,"top")) {
			del_esc3(go.dir);del_space(go.code);
			printf(" %5d  %-8s  %s\n",count+1,go.code,go.name);
			if (pf.type<18) {
				key=Yes;
				if (is_char("sde",go.type)) key=No;
				if (stristr(go.dir,"system")) key=No;
				if (stristr(go.dir,"menu")) key=No;
				if (stristr(go.dir,"root")) key=No;
				if (stristr(go.dir,"/etc")) key=No;
				if (stristr(go.dir,"id")) key=No;
				if (stristr(go.dir,"..")) key=No;
				if (!key) {
					mess("½Ã»ð¸¸ »ç¿ë°¡´ÉÇÑ ¸Þ´ºÅ¸ÀÔ(sde) ¶Ç´Â ºÒ°¡´É µð·ºÅä¸® ±¸Á¶");
					continue;
				}
			}
			fwrite(&go,sizeof_go,1,fp);
			count++;
		}
	}
	fclose(fp2);
	fp2=fopen(TMP_FILE2,"r");
	if (fp2==NULL) {fclose(fp);return;}
	while(fread(&go,sizeof_go,1,fp2)) {
		fwrite(&go,sizeof_go,1,fp);
	}
	fclose(fp2);
	fclose(fp);
	unlink(TMP_FILE2);
	printf(" ÃÑ %d °³ÀÇ ¸Þ´º°¡ »ðÀÔµÇ¾ú½À´Ï´Ù. \n",count);
	pressenter();
}


//¸Þ´º ÀúÀå
menu_write()
{
	FILE *fp,*fp2;
	char filename[300];
	int i,count=0;
	if (pf.type<18) {
		printf("\n ½Ã»ð¸¸ °¡´ÉÇÑ ±â´ÉÀÔ´Ï´Ù.\n");
		pressenter();
		return;
	}
	if (mmn[1]<1) {
		printf("\n  ¸Þ´º ÃßÃâ ¹æ¹ý MW ÃßÃâ½ÃÀÛ-ÃßÃâ³¡\n");
		pressenter();
		return;
	}
	fp=fopen(NOW_MENU,"r");
	fp2=fopen(TMP_FILE2,"w");
	if (fp2==NULL) {fclose(fp);return;}
	if (mmn[3]>0) {
		for (i=1;i<100;i++) {
			if (mmn[i]>0) {
				fseek(fp,mmn[i]*sizeof_go,SEEK_SET);
				if (fread(&go,sizeof_go,1,fp)) {
					fwrite(&go,sizeof_go,1,fp2);
					count++;
					printf(" %5d  %-8s  %s\n",mmn[i],go.code,go.name);
				}
			}
		}
	}
	else {
		for (i=mmn[1];i>0&&i<=mmn[2];i++) {
			fseek(fp,i*sizeof_go,SEEK_SET);
			if (!fread(&go,sizeof_go,1,fp)) break;
			fwrite(&go,sizeof_go,1,fp2);
			count++;
			printf(" %5d  %-8s  %s\n",i,go.code,go.name);
		}
	}
	fclose(fp);
	fclose(fp2);
	if (count>0&&file_size(TMP_FILE2)>10) {
		sprintf(filename,"tmp/menu%s.dat",now_str(18));
		file_move(TMP_FILE2,filename);
		printf(" ÃÑ %d °³ÀÇ ¸Þ´º°¡ ÃßÃâµÇ¾ú½À´Ï´Ù. \n",count);
		printf(" ÆÄÀÏ¸í:%s  ÆÄÀÏÅ©±â:%d \n",filename,file_size(filename));
		sz_file(filename);
		unlink(filename);
	}
}


disp_line(int mode,int n)
{
	char buffn[10];
		 if (go.type=='m')  sprintf(buffn,"[%c]",go.type);
	else if (go.type=='e')  sprintf(buffn,"{%c}",go.type);
	else if (go.type=='j')  sprintf(buffn,"{%c}",go.type);
	else if (go.type=='s')  sprintf(buffn,"<%c>",go.type);
	else if (go.type=='r')  sprintf(buffn,"_%c_",go.type);
	else					sprintf(buffn," %c ",go.type);
	printf("%4d[35m%2d[32m%3s[33m%4s",n-1,go.deep,buffn,go.num);
	printf(" [36m%-8s[37m%-26.26s[35m%-4s[37m%-27.27s\n",go.code,go.name,go.wtimes,go.dir);
}


sadd_menu()
{
	FILE *fp;
	int n,n2;
	clear_menu();
	fp=fopen(NOW_MENU,"r");
	fseek(fp,top*sizeof_go,SEEK_SET);
	n=top;
	n2=n;
	while(fread(&go,sizeof_go,1,fp)) {
		n++;
		if (all_view);				 //ÀüÃ¼¸Þ´ºº¸±â
		else if (go.deep<now_deep) break;
		else if (go.deep>now_deep) continue;
		n2=n;
	}
	fclose(fp);

	//Ãß°¡ÇÒ ¸Þ´º¹øÈ£¸¦ Ã£´Â´Ù.
	read_go(n2);
	read_bcfg();
	sins_menu(n2);	 //»ðÀÔ
	sedit_menu(1,n2);
	read_go(n2);
	read_bcfg();
	if (go.type=='m') {
		top=n2;
		subgo=go;
		now_deep=find_deep(n2+1);
	}
}


search_menu()
{
	FILE *fp;
	int i;
	char buff[100];
	printf("\n °Ë»öÇÏ·Á´Â ¸Þ´ºÀÌ¸§ ¶Ç´Â ÄÚµå? >>");
	lineinput(buff,20);check_han(buff);check_cmd(buff);printf("\n");
	if (strlen(buff)<1) return;
	clear_menu();
	fp=fopen(NOW_MENU,"r");
	i=0;
	while(fread(&go,sizeof_go,1,fp)) {
		if (stristr(go.code,buff)||stristr(go.name,buff)) {
			printf("%5d %d %4s %c %-8s %-30.30s %-23.23s\n",
			i,go.deep,go.num,go.type,go.code,go.name,go.dir);
			if (yesno2("(Y/n)>>",Yes)) {
				set_gopos(i);
				last=top;
				break;
			}
		}
		i++;
	}
	fclose(fp);
}


int nnum=0;

sedit_menu(int mode,int nn)
{
	int n;
	char buff[140];
	char buff1[140];
	char buff2[140];
	GO oldgo;

	if (mode==2||mode==3) {
		sedit_menu2(mode,nn);
		return;
	}
	read_go(nn);
	read_bcfg();
	oldgo=go;

	while (1) {
		key=0;
		if (nnum<0) nnum=0;
		if (nnum>0) {nn=nnum;nnum=0;read_go(nn);read_bcfg();}
		sdisp_menu(nn);
		xy(1,22);
		printf(" º¹±Í(P) ÀúÀå(W) ´ÙÀ½(N) ÀÌÀü(A) Ãë¼Ò(Q) ÆíÁý(Enter) ¼±ÅÃ>>");
		lineinput(buff,9);check_han(buff);printf("\n");n=atoi(buff);
		if (is_same(buff,"w")||is_quit(buff)) break;
		if		(strlen(buff)<1) key=ed_menu2(0);
		else if (n>=1&&n<=20)	 key=ed_menu2(n);
		else if (is_same(buff,"p")) key=2;
		else if (is_same(buff,"q")) key=3;
		else if (is_same(buff,"w")) key=4;
		else if (is_same(buff,"a")) key=5;
		else if (is_same(buff,"n")) key=6;
		else if (atoi(buff)>100)	{nnum=atoi(buff);key=1;}
		if (key!=3) {
			check_dir("%s/menucfg",club_go.dir);
			save_go(nn);
			save_bcfg();
		}
		if		(key==1) {nn=nnum;nnum=0;read_go(nn);read_bcfg();}
		else if (key==2) {break;	}
		else if (key==3) {go=oldgo;break;}
		else if (key==4) {break;	}
		else if (key==5) {nnum=nn-1;}
		else if (key==6) {nnum=nn+1;}
	}
	last=nn;
	return;
}

sedit_menu2(int mode,int nn)
{
	int n;
	char buff[140];
	char buff1[140];
	char buff2[140];
	GO oldgo;

	while(1) {
		read_go(nn);
		read_bcfg();
		oldgo=go;
		key=0;
		if (nnum<0) nnum=0;
		if (nnum>0) {nn=nnum;nnum=0;read_go(nn);read_bcfg();}
		sdisp_menu(nn);
		if (mode==2) key=ed_menu2(0);
		else		 key=ed_menu2(-1);
		if (key!=3) {
			check_dir("%s/menucfg",club_go.dir);
			save_go(nn);
			save_bcfg();
		}
		if		(key==1) {nn=nnum;nnum=0;read_go(nn);read_bcfg();continue;}
		else if (key==3) go=oldgo;
		else if (key==5) {nnum=nn-1;continue;}
		else if (key==6) {nnum=nn+1;continue;}
		break;
	}
	last=nn;
	return;
}

ed_menu2(int n)
{
	int key=0;
	char buff[200];
	GO	oldgo;
	oldgo=go;
	if (n==0) {
		if ((key=ed_menu2(1))>0) return key;
		if ((key=ed_menu2(2))>0) return key;
		if ((key=ed_menu2(3))>0) return key;
		if ((key=ed_menu2(4))>0) return key;
		if ((key=ed_menu2(5))>0) return key;
		if ((key=ed_menu2(6))>0) return key;
		if ((key=ed_menu2(7))>0) return key;
		if ((key=ed_menu2(8))>0) return key;
		key=4;
	}
	else if (n==-1) {
		if ((key=ed_menu2(1))>0) return key;
		if ((key=ed_menu2(2))>0) return key;
		if ((key=ed_menu2(3))>0) return key;
		if ((key=ed_menu2(4))>0) return key;
		if ((key=ed_menu2(5))>0) return key;
		if ((key=ed_menu2(6))>0) return key;
		if ((key=ed_menu2(7))>0) return key;
		if ((key=ed_menu2(8))>0) return key;
		if ((key=ed_menu2(9))>0) return key;
		if ((key=ed_menu2(10))>0) return key;
		if ((key=ed_menu2(11))>0) return key;
		if ((key=ed_menu2(12))>0) return key;
		if ((key=ed_menu2(13))>0) return key;
		if ((key=ed_menu2(14))>0) return key;
		if ((key=ed_menu2(15))>0) return key;
		if ((key=ed_menu2(16))>0) return key;
		if ((key=ed_menu2(17))>0) return key;
		if ((key=ed_menu2(18))>0) return key;
		key=4;
	}
	else if (n==1) {
		xy(15, 3);
		lineinput(buff,4);check_han(buff);
			 if (is_same(buff,"p")) key=2;
		else if (is_same(buff,"q")) key=3;
		else if (is_same(buff,"w")) key=4;
		else if (is_same(buff,"a")) key=5;
		else if (is_same(buff,"n")) key=6;
		else if (atoi(buff)>0)		{nnum=atoi(buff);key=1;}
	}
	else if (n==2) {
		xy(15, 4);
		lineinput(buff,2);check_han(buff);
			 if (is_same(buff,"p")) key=2;
		else if (is_same(buff,"q")) key=3;
		else if (is_same(buff,"w")) key=4;
		else if (is_same(buff,"a")) key=5;
		else if (is_same(buff,"n")) key=6;
		else if (is_same(buff,"0")||atoi(buff)>0) go.deep=atoi(buff);
	}
	else if (n==3) {
		xy(15, 5);
		lineinput(buff,3);check_han(buff);strlwr(buff);
		if (buff[0]!=0) {
			if		(pf.type<18&&is_char("ecdx",buff[0])) ;        //½Ã»ð¿Ü c,d,x ÀÚ·á½Ç±ÝÁö
			else if (pf.type<16&&is_char("s",buff[0])) ;
			else go.type=buff[0];
		}
		//µð·ºÅä¸®¸¦ ÀÓÀÇ·Î ¹Ù²Ù´Â°ÍÀ» ¹æÁöÇÑ´Ù.
		if (pf.type<18&&is_char("nr",oldgo.type)&&oldgo.type!=go.type) strcpy(go.dir,"");
	}
	else if (n==4) {
		strcpy(buff,go.code);
		xy(15, 6);dosinput(buff, 8);check_han(buff);strlwr(buff);del_esc3(buff);
			 if (is_same(buff,"p")) key=2;
		else if (is_same(buff,"q")) key=3;
		else if (is_same(buff,"w")) key=4;
		else if (is_same(buff,"a")) key=5;
		else if (is_same(buff,"n")) key=6;
		else if (is_same(buff," ")) strcpy(go.code,"");
		else if (strlen(buff)>0) strcpy(go.code,buff);
	}
	else if (n==5) {
		strcpy(buff,go.name);
		xy(15, 7);dosinput(buff,40);check_han(buff);
		all_trim(buff);
			 if (is_same(buff,"p")) key=2;
		else if (is_same(buff,"q")) key=3;
		else if (is_same(buff,"w")) key=4;
		else if (is_same(buff,"a")) key=5;
		else if (is_same(buff,"n")) key=6;
		else if (is_same(buff," ")) strcpy(go.name,"");
		else if (strlen(buff)>0) strcpy(go.name,buff);
	}
	else if (n==6) {
		xy(15, 8);
		lineinput(buff, 4);
		if (buff[0]!=0) strcpy(go.num,buff);
		del_space(go.num );strlwr(go.num );
	}
	else if (n==7) {
		if (go.type=='z'||go.type=='b') {
			xy(20, 9);printf("1:¿À´Ã 2:3ÀÏ°£ 3:7ÀÏ°£ 4:30ÀÏ°£ 5:»èÁ¦ 6:»èÁ¦+7ÀÏ");
			xy(20,10);printf("7:ÃßÃµ 8:ÃßÃµ+7ÀÏ°£ 9:%s",cfg.sysop);
		}
		xy(15, 9);lineinput(buff,4);check_han(buff);strlwr(buff);
		if (is_same(buff," ")) strcpy(go.wtimes,"");
		else if (is_same(buff,"q")) key=3;
		else if (strlen(buff)>0) strcpy(go.wtimes,buff);
	}
	else if (n==8) {
		if (pf.type>=17||go.type=='n'||go.type=='r') {
			//µð·ºÅä¸® Áú¹®
			xy(5,10);
			if (go.type=='r'&&is_same(go.code,"sysmail")) printf("¸ÞÀÏ¼ö½ÅID  : ");
			else if (is_char("ej",go.type))               printf("ÇÁ·Î±×·¥°æ·Î: ");
			else if (go.type=='n')                        printf("ÅÚ³ÝÁÖ¼ÒÀÔ·Â: ");
			else										  printf("µð·ºÅä¸®: ");
			strcpy(buff,go.dir);
			dosinput(buff,60);check_han(buff);strlwr(buff);
			all_trim(buff);
			//¹®ÀÚ¿­¿¡ È­ÀÏ¼ÓÀÇ ³»¿ëÀÌ Æ÷ÇÔµÇ¸é Yes;
			if (pf.type<19&&((stristr(buff,"bin/")||stristr(buff,"system/")||
				stristr(buff,";")||stristr(buff,"&")||
				stristr(buff,"*")||stristr(buff,"?")||
				stristr(buff,"cfg/")||stristr(buff,"sys_back")||
				stristr(buff,"/etc/")||stristr(buff,"/system/")))) strcpy(go.dir,"");
			else if (in_file3("system/notdir.txt",buff)) strcpy(go.dir,"");
			else if (is_same(buff,"p")) key=2;
			else if (is_same(buff,"q")) key=3;
			else if (is_same(buff,"w")) key=4;
			else if (is_same(buff,"a")) key=5;
			else if (is_same(buff,"n")) key=6;
			else strcpy(go.dir,buff);
		}
		if (is_typepds){
			del_space(go.dir);
			if (strlen(go.dir)<1) {
				if (club_mode) sprintf(go.dir,"%s/pds/%s",club_go.dir,go.code);
				else sprintf(go.dir,"pds/%s",go.code);
				strchng2(go.dir,"//","/");strchng2(go.dir,"..",".");
				del_space(go.dir);
			}
		}
	}
	else if (n==11) {
		xy(15,13);
		lineinput(buff,30);check_han(buff);strlwr(buff);
			 if (is_same(buff,"p")) key=2;
		else if (is_same(buff,"q")) key=3;
		else if (is_same(buff,"w")) key=4;
		else if (is_same(buff,"a")) key=5;
		else if (is_same(buff,"n")) key=6;
		else if (is_same(buff," ")) strcpy(bcfg.key,"");
		else if (strlen(buff)>0) strcpy(bcfg.key,buff);
	}
	else if (n==12) {
		xy(15,14);get_str(bcfg.man1,8);
		xy(25,14);get_str(bcfg.man2,8);
		xy(35,14);get_str(bcfg.man3,8);
		xy(45,14);get_str(bcfg.man4,8);
		xy(55,14);get_str(bcfg.man5,8);
	}
	else if (n==13) {
		xy(25,15);bcfg.itype =get_num(bcfg.itype ,2);
		xy(40,15);bcfg.rtype =get_num(bcfg.rtype ,2);
		xy(55,15);bcfg.dtype =get_num(bcfg.dtype ,2);
		xy(70,15);bcfg.wtype =get_num(bcfg.wtype ,2);
	}
	else if (n==14) {
		xy(25,16);bcfg.fdown =get_num(bcfg.fdown ,2);
		xy(40,16);bcfg.fread =get_num(bcfg.fread ,2);
	}
	else if (n==15) {
		xy(25,17);bcfg.wdown =get_num(bcfg.wdown ,2);
		xy(40,17);bcfg.wdown2=get_num(bcfg.wdown2,2);
		xy(55,17);bcfg.rdown =get_num(bcfg.rdown ,2);
		xy(70,17);bcfg.rdown2=get_num(bcfg.rdown2,2);
	}
	else if (n==16) {
		xy(25,18);bcfg.level =get_num(bcfg.level ,3);
		xy(40,18);bcfg.level2=get_num(bcfg.level2,3);
		xy(55,18);bcfg.level3=get_num(bcfg.level3,3);
	}
	else if (n==17&&pf.type>17) {
		xy(25,19);bcfg.uplevel=get_num(bcfg.uplevel ,3);
		xy(40,19);bcfg.upcash =get_num(bcfg.upcash	,3);
		xy(55,19);bcfg.updown =get_num(bcfg.updown	,3);
	}
	else if (n==18) {
		xy(25,20);bcfg.li_w=get_num(bcfg.li_w,3);
	}
	return key;
}



sdisp_menu(int nn)
{
	int n1,n2,n3,n4;
	cls();
	printf(" SUB:%s  ÇöÀç±íÀÌ:%d    (Ç×¸ñ:%d)\n",subgo.code,subgo.deep,count_menu_num);
	printfL2();
	printf("  1.Ç×¸ñ¹øÈ£: %d\n",nn);
	printf("  2.¸Þ´º±íÀÌ: %d\n",go.deep);
	printf("  3.¸Þ´ºÅ¸ÀÔ: %c\n",go.type);
	printf("  4.°¡±âÄÚµå: %s\n",go.code);
	printf("  5.¸Þ´ºÁ¦¸ñ: %s\n",go.name);
	printf("  6.¼±ÅÃ¹øÈ£: %s\n",go.num );
	printf("  7.¾²±âÁ¦ÇÑ: %s\n",go.wtimes);
	printf("  8.µð·ºÅä¸®: %s\n",go.dir );
	printf("\n");
	printf("\n");
	printf(" 11.Å° ¿ö µå: %s  \n",bcfg.key);
	printf(" 12.´ã´çÁöÁ¤: %-10s%-10s%-10s%-10s%-10s\n",
			bcfg.man1,bcfg.man2,bcfg.man3,bcfg.man4,bcfg.man5);
	printf(" 13.µî±ÞÁ¦ÇÑ:  ÁøÀÔµî±Þ:%3d   ÀÐ±âµî±Þ:%3d   ´Ù¿îµî±Þ:%3d   ¾²±âµî±Þ:%3d\n",
			bcfg.itype,bcfg.rtype,bcfg.dtype,bcfg.wtype);
	printf(" 14.ÀÚÀ¯µî±Þ:  ÀÚÀ¯´Ù¿î:%3d   ÀÚÀ¯ÀÐ±â:%3d \n",
			bcfg.fdown,bcfg.fread);
	printf(" 15.º¸»ó¼³Á¤:  ¾÷º¸»ó  :%3d   ¾ß°£º¸»ó:%3d   ´Ù¿î°¨¼Ò:%3d   ¾ß°£°¨¼Ò:%2d\n",
			bcfg.wdown,bcfg.wdown2,bcfg.rdown,bcfg.rdown2);
	printf(" 16.·¹º§º¸»ó:  ¾²±â·¹º§:%3d   ÀÐ±â·¹º§:%3d   ´Ù¿î·¹º§:%3d\n",
			bcfg.level,bcfg.level2,bcfg.level3);
	printf(" 17.»óÇ°º¸»ó:  ·¹º§»óÇ°:%3d   Ä³½Ã»óÇ°:%3d   ´Ù¿î»óÇ°:%3d Mbyte\n",
			bcfg.uplevel,bcfg.upcash,bcfg.updown);
	printf(" 18.ÁøÀÔÁ¦ÇÑ:  ÀÏÀÏ¾²±â:%3d\n",bcfg.li_w);
}



sins_menu(int nn)	//»ðÀÔ
{
	FILE *fp;
	int i,n,nn2;
	char buff[20];
	GO go2;
	if (nn<1) {
		printf("\n ¸Þ´º »ðÀÔ »ç¿ë¹æ¹ý:  i [»ðÀÔÇÒ ¸Þ´º ´ÙÀ½ ¹øÈ£]\n");
		pressenter();
		return No;
	}
	fp=fopen(NOW_MENU,"r+");if (fp==NULL) return No;
	n=count_frec(fp,sizeof_go);   //ÃÑ¼ö
	if (nn>n) nn=n;
	for (i=n;i>nn;i--) {
		fseek(fp,(i-1)*sizeof_go,SEEK_SET);
		fread(&go,sizeof_go,1,fp);
		fseek(fp,i*sizeof_go,SEEK_SET);
		fwrite(&go,sizeof_go,1,fp);
	}
	fclose(fp);
	return Yes;
}


sdel_menu(int nn)	//»èÁ¦
{
	FILE *fp;
	int i,n;
	if (mmn[1]<1) mess("¸Þ´º »èÁ¦ »ç¿ë¹æ¹ý:  d [»èÁ¦ÇÒ ¸Þ´º ¹øÈ£]");
	else {
		if ((fp=fopen(NOW_MENU,"r+"))==NULL) return No;
		n=count_frec(fp,sizeof_go);   //ÃÑ¼ö
		if (n<1) {fclose(fp);return No;}
		go.deep=0;
		strcpy(go.code,"");
		for (i=1;i<101;i++) {
			if (mmn[i]>0&&mmn[i]<=n) {
				fseek(fp,mmn[i]*sizeof_go,SEEK_SET);
				fwrite(&go,sizeof_go,1,fp);
			}
		}
		fclose(fp);
		pack_menu();
	}
	return Yes;
}


pack_menu()
{
	FILE *fp1,*fp2;
	int change=No;
	int rec=0;
	GO tgo;
	fp1=fopen(NOW_MENU,"r");if (fp1==NULL) return No;
	fp2=fopen(TMP_FILE,"w");if (fp2==NULL) return No;
	while (fread(&tgo,sizeof_go,1,fp1)) {
		if (strlen(tgo.code)<1||tgo.type<' ') change=Yes;
		else {
			tgo.rec=rec++;
			fwrite(&tgo,sizeof_go,1,fp2);
		}
	}
	fclose(fp1);
	fclose(fp2);
	if (change) file_move(TMP_FILE,NOW_MENU);
	else unlink(TMP_FILE);
	return Yes;
}


sudel_menu(int num1,int num2)	//»èÁ¦
{
	FILE *fp;
	int i,n;
	char buff[200];
	if (num1<1||num2<1) {
		printf("\n ¹üÀ§ÁöÁ¤»èÁ¦ u 11 40  : 11ºÎÅÍ 40±îÁö ¸Þ´º»èÁ¦\n");
		pressenter();
		return No;
	}
	fp=fopen(NOW_MENU,"r+");if (fp==NULL) return No;
	n=count_frec(fp,sizeof_go);   //ÃÑ¼ö
	if (n<1) {fclose(fp);return No;}
	if (num1<0) num1=0;if (num1>n) num1=n;
	if (num2<0) num2=0;if (num2>n) num2=n;
	sprintf(buff,"%d - %d ÇØ´çµÇ´Â ¹üÀ§¸¦ »èÁ¦ÇÒ±î¿ä?",num1,num2);
	if (yesno(buff,No)) {
		go.deep=0;strcpy(go.code,"");
		for (i=num1;i<=num2&&i<=n;i++) {
			fseek(fp,i*sizeof_go,SEEK_SET);
			fwrite(&go,sizeof_go,1,fp);
		}
		printf("%d - %d ÇØ´çµÇ´Â ¹üÀ§°¡ »èÁ¦µÇ¾ú½À´Ï´Ù\n",num1,num2);
		pressenter();
	}
	fclose(fp);
	pack_menu();
	return Yes;
}


smove_menu(int n1,int n2)	//ÀÌµ¿
{
	FILE *fp;
	int i,n,nn2;
	char buff[20];
	GO go2;
	if (n1==n2||n1<1||n2<0) {
		printf("\n ¸Þ´º ÀÌµ¿ »ç¿ë¹æ¹ý:  m [ÀÌµ¿ÇÒ ¸Þ´º ¹øÈ£] [ÀÌµ¿ÇÒ À§Ä¡]\n");
		pressenter();
		return No;
	}

	fp=fopen(NOW_MENU,"r+");if (fp==NULL) return No;
	n=count_frec(fp,sizeof_go);   //ÃÑ¼ö
	if (n1>n) n1=n;
	if (n2>n) n2=n;
	if (n<1||n1==n2) {fclose(fp);return No;}

	if (n1<n2) {		//n1->n2
		fseek(fp,n1*sizeof_go,SEEK_SET);
		fread(&go2,sizeof_go,1,fp);
		for (i=n1;i<n2;i++) {
			fseek(fp,(i+1)*sizeof_go,SEEK_SET);
			fread(&go,sizeof_go,1,fp);
			fseek(fp,i*sizeof_go,SEEK_SET);
			fwrite(&go,sizeof_go,1,fp);
		}
		fseek(fp,n2*sizeof_go,SEEK_SET);
		fwrite(&go2,sizeof_go,1,fp);
	}
	else {		  //n1->n2
		fseek(fp,n1*sizeof_go,SEEK_SET);
		fread(&go2,sizeof_go,1,fp);
		for (i=n1;i>n2;i--) {
			fseek(fp,(i-1)*sizeof_go,SEEK_SET);
			fread(&go,sizeof_go,1,fp);
			fseek(fp,i*sizeof_go,SEEK_SET);
			fwrite(&go,sizeof_go,1,fp);
		}
		fseek(fp,(n2+1)*sizeof_go,SEEK_SET);
		fwrite(&go2,sizeof_go,1,fp);
	}
	fclose(fp);
	return Yes;
}


check_menu()
{
	FILE *fp;
	char buff[120];
	char filename[120];
	int i,n;
	char allyes=No;
	char allquit=No;
	key=No;

	if ((fp=fopen(NOW_MENU,"r+"))==NULL) return;
	n=-1;
	while(1) {
		n++;
		fseek(fp,n*sizeof_go,SEEK_SET);
		if (!fread(&go,sizeof_go,1,fp)) break;
		del_space(go.code);
		go.rec=n;
		if (is_same(go.code,"rmail"))  go.type='l';
		if (is_same(go.code,"cmail"))  go.type='l';
		if (is_same(go.code,"mbox"))   go.type='l';
		if (is_same(go.code,"wmail"))  go.type='r';
		if (is_same(go.code,"cguest")) go.type='r';
		if (is_same(go.code,"guest"))  go.type='r';
		if (is_same(go.code,"chat"))   go.type='r';
		if (is_same(go.code,"cchat"))  go.type='r';
		if (is_same(go.code,"sysmail")) go.type='r';
		if (is_same(go.code,"notice")) go.type='l';
		if (go.deep>9||go.deep<0) go.deep=9;
		strscheck(go.code); 		 //Æ¯¼öÇÑ¹®ÀÚ¸¦ ¹«½ÃÇÑ´Ù.
		fseek(fp,n*sizeof_go,SEEK_SET);
		all_trim(go.name);
		all_trim(go.dir);
		if (is_typepds) {
			del_space(go.dir);
			if (strlen(go.dir)<1||is_same(go.dir,"./")) {
				if (club_mode) sprintf(go.dir,"%s/pds/%s",club_go.dir,go.code);
				else		   sprintf(go.dir,"pds/%s",go.code);
				check_dir(go.dir);
			}
		}
		fwrite(&go,sizeof_go,1,fp);
	}
	fclose(fp);
}


copy_file_menu(char *file1,char *file2)
{
	FILE *fp1,*fp2;
	int i,n;
	if ((fp1=fopen(file1,"r"))==NULL) return;
	if ((fp2=fopen(file2,"w"))==NULL) return;
	n=0;
	while(fread(&go,sizeof_go,1,fp1)) {
		del_space(go.code);
		if (strlen(go.code)<1||go.type<' ') continue;
		go.rec=++n;
		fwrite(&go,sizeof_go,1,fp2);
	}
	fclose(fp1);
	fclose(fp2);
}


reset_menuind() 	//¸Þ´ºSUB TAG ÀçÁ¤¸®
{
	FILE *fp,*fp2;
	char filename[120],buff[100];
	int n,deep;
	GO tgo;

	if (now_local>0) {
		sprintf(buff,"menustru/%d",now_local);check_dir(buff);
		sys_tem("rm -f menustru/%d/*.ind",now_local);
	}
	else {
		sys_tem("rm -f %s/menustru/%d/*.ind",club_go.dir);
	}
	if ((fp=fopen(NOW_MENU,"r"))==NULL) return;
	deep=0;
	n=-1;
	while(1) {
		n++;
		fseek(fp,n*sizeof_go,SEEK_SET);
		if (!fread(&tgo,sizeof_go,1,fp)) break;
		if (tgo.type=='m'&&strlen(tgo.code)>0) {
			if (now_local>0) sprintf(filename,"menustru/%d/%s.ind",now_local,tgo.code);
			else sprintf(filename,"%s/menustru/%s.ind",club_go.dir,tgo.code);
			if (!fread(&tgo,sizeof_go,1,fp)) continue;
			if ((fp2=fopen(filename,"w"))!=NULL) {
				deep=tgo.deep;
				while(fread(&tgo,sizeof_go,1,fp)&&tgo.deep>=deep) {
					if (deep==tgo.deep&&tgo.type>' '&&strlen(tgo.code)>0)
						fwrite(&tgo,sizeof_go,1,fp2);
				}
				fclose(fp2);
			}
		}
	}
}



read_go(int n)
{
	FILE *fp;
	int ret=No;
	clear_menu();
	if (n==0) {
		set_gotop();
		top=0;
		ret=Yes;
	}
	else {
		if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
			fseek(fp,n*sizeof_go,SEEK_SET);
			if (fread(&go,sizeof_go,1,fp)) {
				top=n;
				ret=Yes;
			}
			fclose(fp);
		}
	}
	return ret;
}


save_go(int n)
{
	FILE *fp;
	if (n<0||n>20000) return;
	if ((fp=fopen(NOW_MENU,"r+"))!=NULL) {
		if (n==0) set_gotop();
		fseek(fp,n*sizeof_go,SEEK_SET);
		go.rec=n;
		fwrite(&go,sizeof_go,1,fp);
		fclose(fp);
	}
}


read_goprev()
{
	FILE *fp;
	int n;
	GO go2;
	clear_menu();
	if (top==0||subgo.deep<1||all_view) top=0;
	else {
		n=top;
		if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
			while(1) {
				n--;
				fseek(fp,n*sizeof_go,SEEK_SET);
				if (!fread(&go2,sizeof_go,1,fp)) {top=0;break;}
				if (go2.deep<subgo.deep&&go2.type=='m'&&!is_same(go2.code,subgo.code)) {
					go=go2;
					subgo=go;
					n++;
					now_deep=find_deep(n);
					top=n;
					break;
				}
			}
			fclose(fp);
		}
	}
	if (top==0) read_gotop();
}


read_gotop()
{
	top=0;
	read_go(0);
	subgo=go;
	now_deep=find_deep(1);
}


read_goforwd()
{
	FILE *fp;
	int n,linecount,found,old_top;
	clear_menu();
	fp=fopen(NOW_MENU,"r");
	fseek(fp,top*sizeof_go,SEEK_SET);
	linecount=0;
	n=old_top=top;
	found=No;
	while(linecount<15&&fread(&go,sizeof_go,1,fp)) {
		n++;
			 if (all_view); 			  //ÀüÃ¼¸Þ´ºº¸±â
		else if (go.deep<now_deep) break;
		else if (go.deep>now_deep) continue;
		linecount++;
		if (!all_view&&strlen(go.code)>1&&go.deep==now_deep) top=n;
		found=Yes;
	}
	if (found&&linecount<15) top=old_top;
	fseek(fp,top*sizeof_go,SEEK_SET);
	if (fread(&go,sizeof_go,1,fp)&&go.type=='m') top++;
	fclose(fp);
	if (!found) read_gobackwd();
}


read_gobackwd()
{
	FILE *fp;
	int n,linecount,old_top;
	fp=fopen(NOW_MENU,"r");
	if (fp==NULL) return;
	fseek(fp,top*sizeof_go,SEEK_SET);
	linecount=14;
	old_top=top;
	n=top-1;
	while(!feof(fp)&&linecount>0) {
		fseek(fp,n*sizeof_go,SEEK_SET);
		if (feof(fp)||!fread(&go,sizeof_go,1,fp)) break;
		n--;
			 if (all_view); 			  //ÀüÃ¼¸Þ´ºº¸±â
		else if (go.deep<now_deep) break;
		else if (go.deep>now_deep) continue;
		top=n;
		linecount--;
	}
	if (linecount<0) top=old_top;
	fseek(fp,top*sizeof_go,SEEK_SET);
	if (fread(&go,sizeof_go,1,fp)&&go.type=='m') top++;
	fclose(fp);
}


//go¿¡ topÁ¤º¸¸¦ ÀúÀåÇÑ´Ù.
set_gotop()
{
	go.deep=0;
	go.rec=0;
	go.type='m';
	now_deep=0;
	strcpy(go.code,"top");
	strcpy(go.name,"¸ÞÀÎ¸Þ´º");
	strcpy(go.num,"");
	strcpy(go.dir,"");
}


set_gopos(int mn)
{
	strcpy(subgo.code,"");
	if (read_go(mn)) {
		read_bcfg();
		sdisp_menu(mn); 	//È­¸éÃâ·Â
		if (go.type=='m') {
			subgo=go;
			mn++;
			now_deep=find_deep(mn);
		}
		last=mn;
		top=mn;
		pressenter();
	}
	else {
		mess("%d ¿¡ ÇØ´çµÇ´Â ¸Þ´º ¹øÈ£°¡ ¾ø½À´Ï´Ù.",mn);
	}
}


set_postop()
{
	read_go(0);
	read_bcfg();
	subgo=go;
	now_deep=find_deep(1);
	last=1;
	top=1;
}


scount_menu()
{
	FILE *fp;
	int n=0;
	if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
		n=count_frec(fp,sizeof_go);   //ÃÑ¼ö
		fclose(fp);
	}
	return n;
}


find_deep(int n)
{
	FILE *fp;
	int ret=0;
	GO go2;
	if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
		fseek(fp,n*sizeof_go,SEEK_SET);
		if (fread(&go2,sizeof_go,1,fp)) ret=go2.deep;
		fclose(fp);
	}
	return ret;
}



sdn_menu()
{
	char buff[300];
	if (yesno("\n ÇöÀçÀÇ ¸Þ´º¸¦ MENU.ZIPÈ­ÀÏ·Î ´Ù¿î¹ÞÀ¸½Ã°Ú½À´Ï±î? (y/N)>>",No)) {
		printf("\n menu.zip È­ÀÏÀÇ ´Ù¿îÀ» ½ÃÀÛÇÕ´Ï´Ù.\n");
		sprintf(buff,"%s/menustru",club_go.dir);
		xsz_zipfile(buff,"menu.dat");
		printf("\n menu.zip È­ÀÏÀÇ ´Ù¿îÀÌ ³¡³µ½À´Ï´Ù.\n");
	}
}


sup_menu()
{
	char buff[300];
	char filename[300];
	if (!yesno("\n ### ÇöÀçÀÇ ¸Þ´º¸¦ Àü¼ÛÇÏ¿© µ¤¾î¾¯¿ì°Ú½À´Ï±î? (y/N)>>",No)) goto fail_upload;
	if (pf.type<17&&cpf.type<17) goto fail_upload;
	printf(" ### menu.zip ¶Ç´Â menu.dat È­ÀÏ¸íÀ¸·Î º¸³»ÁÖ½Ê½Ã¿ä. \n");
	if (!rz_file(filename)||!is_file(filename)) goto fail_upload;  //È­ÀÏÀü¼Û¹Þ±â
	if (stristr(filename,".zip")) {
		sys_tem("unzip -j-n-qq -dtmp/up%d%s '%s' menu.dat",cfg.hostno,no_spc(tty),filename);
		sprintf(filename,"tmp/up%d%s/menu.dat",cfg.hostno,no_spc(tty));
		if (!is_file(filename)||file_size(filename)<1) goto fail_upload;
	}
	file_copy(filename,NOW_MENU);
	printf("\n ### menu.dat È­ÀÏÀÇ µî·ÏÀ» ¸¶ÃÆ½À´Ï´Ù.\n");
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
	set_postop();
	return Yes;

	fail_upload:
	printf("\n ### menu.dat È­ÀÏÀÇ µî·ÏÀÌ Ãë¼ÒµÇ¾ú½À´Ï´Ù.\n");
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
	return No;
}




//nowdir ¹æ¿¡¼­ filename À» zip À¸·Î ÇØ¼­ Àü¼Û
xsz_zipfile(char *nowdir,char *filename)
{
	char buff[400];
	char dir[400];
	char file[400];
	char file2[400];
	char olddir[480];
	getcwd(olddir,sizeof(olddir));		//ÇöÀçÀÇ µð·ºÅä¸®
	strcpy(file,filename);
	pure_name(2,file);	 //È­ÀÏ¸í¿¡¼­ È®ÀåÀÚ Á¦¿Ü »Ì¾Æ³¿
	sprintf(file2,"%s/tmp/up%d%s/%s.zip",olddir,cfg.hostno,no_spc(tty),file);
	sprintf(buff,"%s/tmp/up%d%s",olddir,cfg.hostno,no_spc(tty));check_dir(buff);
	if (strlen(nowdir)>0) chdir(nowdir);
	sys_tem("zip '%s' '%s'",file2,filename);
	chdir(olddir);
	if (!is_file(file2)) mess("%s.zip È­ÀÏÀÌ ¾ø½À´Ï´Ù.",file);
	else sz_file(file2);
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
}


disp_menustat()
{
	FILE *fp;
	GO go;
	int i,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11;
	n1=n2=n3=n4=n5=n6=n7=n8=n9=n10=n11=0;
	if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
		while(fread(&go,sizeof_go,1,fp)) {
			n1++;
			if (go.type<=' '||strlen(go.code)<1) n3++;
			else {
				n2++;
				if (go.type=='m') n4++;
				else if (go.type=='s') n5++;
				else if (is_typebbs) n6++;
				else if (is_typepds) n7++;
				else if (go.type=='n') n9++;
				else if (go.type=='t') n10++;
				else if (is_typeexe) n8++;
			}
		}
		fclose(fp);
	}
	printf("\n\n  ========= ¸Þ´º »óÅÂ =========\n");
	printf("ÀÛ¾÷ÀÚ ID   : %s   (µî±Þ:%d)\n",pf.id,pf.type);
	printf("µ¿È£È¸      : %s\n",club_go.code);
	printf("¸Þ´ºÆÄÀÏ    : %s\n",NOW_MENU);
	printf("ÆÄÀÏÅ©±â    : %d byte\n",file_size(NOW_MENU));
	printf("ÃÑÇ×¸ñ¼ö    : %d \n",n1);
	printf("½ÇÁ¦Ç×¸ñ¼ö  : %d \n",n2);
	printf("ºóÇ×¸ñ¼ö    : %d \n",n3);
	printf("¼­ºê¸Þ´º(M) : %d \n",n4);
	printf("µ¿È£È¸¼ö(S) : %d \n",n5);
	printf("°Ô½ÃÆÇ  (L) : %d \n",n6);
	printf("ÀÚ·á½Ç  (F) : %d \n",n7);
	printf("¿ÜºÎ¸í·É(E) : %d \n",n8);
	printf("ÅÚ³Ý¸í·É(N) : %d \n",n9);
	printf("¹Ù·Î°¡±â(T) : %d \n",n10);
	pressenter();
}

menu_dir()
{
	FILE *fp;
	int i;
	int all=No,total,count;
	char buff[300];
	total=count=0;
	fp=fopen(NOW_MENU,"r");
	while (fread(&go,sizeof_go,1,fp)) {
		del_space(go.dir);del_esc3(go.dir);
		if (is_bbspds&&strlen(go.dir)>0) {
			printf("N:%3d CODE:%-8s DIR:%-50.50s",go.rec,go.code,go.dir);
			total++;
			if (is_dir(go.dir)) printf("\n");
			else {
				if (!all) {
					printf("\n  µð·ºÅä¸®¸¦ ¸¸µé±î¿ä?(Y/n/a/q)>>");
					lineinput(buff,3);check_han(buff);
					if (is_same(buff,"a")) all=Yes;
					if (is_same(buff,"q")) break;
					if (is_same(buff,"n")) printf("\n");
				}
				if (all||!is_same(buff,"n")) {
					check_dir(go.dir);
					if (!is_dir(go.dir)) printf(" ->¸¸µé±â ½ÇÆÐ\n");
					else				 {
						printf(" ->¸¸µë\n");
						count++;
					}
				}
			}
		}
	}
	fclose(fp);
	mess("%d/%d °³ÀÇ µð·ºÅä¸®°¡ »õ·Î ¸¸µé¾îÁ³½À´Ï´Ù.",count,total);
}

//°¡±âÄÚµå
typedef struct {
	char club[9] ;
	char code[9];			//°¡±âÄÚµå
	char name[41];			//¸Þ´º¸í
}INDEXGO;
#define sizeof_indexgo			 (sizeof(INDEXGO))

//°¡±âÄÚµå
typedef struct {
	char type ;
	char code[9];			//°¡±âÄÚµå
	char name[41];			//¸Þ´º¸í
}SGO;
#define sizeof_sgo	(sizeof(SGO))

//°¡±âÄÚµå
typedef struct {
	char club[9];
	char dir[61];			//°¡±âÄÚµå
}ACLUB;


int comp_s(SGO *sgo1,SGO *sgo2) {
	return strcmp(sgo1->code,sgo2->code);
}

make_menu_index()
{
	FILE *fp,*fp2;
	char filename[300];
	char menu_ind[300];
	SGO  asgo[3000],sgo;
	ACLUB aclub[501];
	INDEXGO indexgo;
	int i,ii,n,count,found,club_count;
	//¸ÞÀÎ ÀÐ±â
	if (now_local>0) sprintf(menu_ind,"menustru/%d/menuind.tmp",now_local);
	else			 sprintf(menu_ind,"menustru/menuind.tmp");
	if ((fp2=fopen(menu_ind,"w"))==NULL) return;
	count=0;
	club_count=0;
	if (now_local>0) sprintf(filename,"menustru/%d/menu.dat",now_local);
	else			 sprintf(filename,"menustru/menu.dat");
	if ((fp=fopen(filename,"r"))==NULL) return;
	ii=0;
	while(fread(&go,sizeof_go,1,fp)) {
		if (strlen(go.code)<1||go.type<=' ') continue;
		if (is_char(go.wtimes,'@')) continue;
		if (is_bbspds||is_char("msejnxdr",go.type)) {
			strcpy(indexgo.club,"main");
			strcpy(indexgo.code,go.code);
			strcpy(indexgo.name,go.name);
			count++;
			fwrite(&indexgo,sizeof_indexgo,1,fp2);
			if (go.type=='s'&&club_count<500&&strlen(go.dir)>1) {
				found=No;
				for (i=0;i<club_count;i++) {
					if (is_same(aclub[i].dir,go.dir)) {found=Yes;break;}
				}
				if (!found) {
					strncpy(aclub[club_count].dir,go.dir,40);
					strcpy(aclub[club_count].club,go.code);
					club_count++;
				}
			}
		}
	}
	fclose(fp);


	for (i=0;i<club_count;i++) {
		sprintf(filename,"%s/menustru/menu.dat",aclub[i].dir);
		del_space(filename);del_esc(filename);
		if ((fp=fopen(filename,"r"))==NULL) continue;
		while(fread(&go,sizeof_go,1,fp)) {
			if (strlen(go.code)<1||go.type<=' ') continue;
			if (is_bbspds||is_char("ejnxdr",go.type)) {
				strcpy(indexgo.club,aclub[i].club);
				strcpy(indexgo.code,go.code);
				strcpy(indexgo.name,go.name);
				fwrite(&indexgo,sizeof_indexgo,1,fp2);
			}
		}
		fclose(fp);
	}
	fclose(fp2);

	sprintf(filename,"%s/menustru/menu.dat",club_go.dir);
	if ((fp=fopen(filename,"r"))==NULL) return;
	for (i=0;i<3000;i++) strcpy(asgo[i].code,"");
	ii=0;
	while(fread(&go,sizeof_go,1,fp)) {
		if (strlen(go.code)<1||go.type<=' ') continue;
		if (ii<3000) {
			asgo[ii].type=go.type;
			strcpy(asgo[ii].code,go.code);
			strcpy(asgo[ii].name,go.name);
			del_space(asgo[ii].code);
			ii++;
		}
	}
	fclose(fp);
	qsort(asgo,3000,sizeof_sgo,comp_s);
	strchng2(filename,".dat",".sgo");
	if ((fp=fopen(filename,"w"))!=NULL) {
		for (i=0;i<3000;i++) {
			sgo=asgo[i];
			if (strlen(sgo.code)>0) {
				fwrite(&sgo,sizeof_sgo,1,fp);
			}
		}
		fclose(fp);
	}
	chmod(filename,0666);

}


//¸Þ´º¸¦ TEXT·Î º¯È¯
menu_text(int mode)
{
	if (mode==1) menu_text1();
	if (mode==2) menu_text2();
}


//¸Þ´º¸¦ TEXT·Î º¯È¯
menu_text1()
{
	FILE *fp,*fp2;
	int nn,ret,count=0;
	char filename[300];
	char newdir[300];
	char buff[300];
	char buff1[300];
	char buff2[300];
	//ÇöÀç ¹øÈ£ ´ÙÀ½ÆÄÀÏ ÀÓ½Ã ÀúÀå
	fp=fopen(NOW_MENU,"r");
	fp2=fopen(TMP_FILE2,"w");
	if (fp2==NULL) {fclose(fp);return;}
	count=0;
	while(fread(&go,sizeof_go,1,fp)) {
		del_space(go.code);all_trim(go.name);
		if (go.type<' '||strlen(go.code)<1) continue;
		count++;
		sprintf(buff,"%04d%2d %-8s %c %-4s %-4s %-40s %s",
			count,go.deep,go.code,go.type,go.num,go.wtimes,go.name,go.dir);
		all_trim(buff);
		printf("%-78.78s\r\n",buff);
		fprintf(fp2,"%s\r\n",buff);
	}
	fclose(fp);
	fclose(fp2);
	sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));
	check_dir(newdir);
	sprintf(buff2,"%s/menu%s.txt",newdir,now_str(18));
	unlink(buff2);
	file_move(TMP_FILE2,buff2);
	if (is_file(buff2)) {
		printf(" ÃÑ %d ¸Þ´º  ÆÄÀÏ¸í:menu%s.txt (%d Byte)\n",count,now_str(18),file_size(buff2));
		sz_file(buff2);
	}
	unlink(TMP_FILE2);
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
}


//¸Þ´º¸¦ TEXT·Î º¯È¯
menu_text2()
{
	FILE *fp,*fp2;
	int nn,n,ret,count=0,key;
	char filename[300];
	char newdir[300];
	char buff[300];
	char buff1[300];
	char buff2[300];

	printf("\n µî·ÏÇÒ TEXT ¸Þ´ºÆÄÀÏÀ» Àü¼ÛÇØÁÖ¼¼¿ä.. (ÆÄÀÏ¸í MENU????.TXT) \n");
	ret=up_file(0,filename,No);   //È­ÀÏÀ» Àü¼Û¹Þ¾Æ filename¿¡ ÀúÀåÇÑ´Ù.
	if (!ret||!is_file(filename)) {mess("¸Þ´º ÆÄÀÏ µî·Ï½ÇÆÐ");return;}
	if (!stristr(filename,"menu")||!stristr(filename,".txt")) {mess("¸Þ´º ÆÄÀÏ¸íÀº menu????.txt Çü½ÄÀÌ¾î¾ß ÇÕ´Ï´Ù.");return;}


	if ((fp=fopen(TMP_FILE2,"w"))==NULL) {mess("%s ÀúÀå½ÇÆÐ",NOW_MENU);return;}
	if ((fp2=fopen(filename,"r"))==NULL) {mess("%s ÀÐ±â½ÇÆÐ",filename);return;}

//1234 12345678 1 1234 1234 1234567890123456789012345678901234567890 1234567890
//234 1 12345678 1 1234 1234 1234567890123456789012345678901234567890 1234567890
	count=0;
	while (fgets(buff1,200,fp2)!=NULL) {
		del_enter(buff1);
		printf("%s",buff1);
		if (strlen(buff1)<20) continue;
		sprintf(buff2,"%-200.200s",buff1);  //°ø¹é´õÇÏ±â
		if (buff2[6]!=' '||buff2[15]!=' '||buff2[17]!=' '||buff2[22]!=' '||buff2[27]!=' ')
			{printf("-> °ø¹é ¿À·ù(Ä­¾î±ß³²)\n");continue;}
		xstrncpy(buff,&buff2[4],2);del_esc3(buff);del_space(buff);go.deep=atoi(buff);
		xstrncpy(buff,&buff2[7],8);del_esc3(buff);del_space(buff);strcpy(go.code,buff);
		go.type=buff2[16];
		xstrncpy(buff,&buff2[18],4);del_esc3(buff);del_space(buff);strcpy(go.num,buff);
		xstrncpy(buff,&buff2[23],4);del_esc3(buff);del_space(buff);strcpy(go.wtimes,buff);
		xstrncpy(buff,&buff2[28],40);del_esc3(buff);all_trim(buff);strcpy(go.name,buff);
		xstrncpy(buff,&buff2[69],60);del_esc3(buff);all_trim(buff);strcpy(go.dir,buff);
		if (go.type<' ') {printf("-> Å¸ÀÔ'%c' ¿À·ù\n",go.type);continue;}
		if (strlen(go.code)<1) {printf("-> °¡±âÄÚµå¿À·ù\n");continue;}
		if (go.type<' ') {printf("-> Å¸ÀÔ'%c' ¿À·ù\n",go.type);continue;}
		if (count==0&&!is_same(go.code,"top")) {mess("-> Ã¹ ¸Þ´º´Â TOP ÀÌ¾î¾ß ÇÕ´Ï´Ù.");break;}
		if (pf.type<18) {
			key=Yes;
			if (is_char("sde",go.type)) key=No;
			if (stristr(go.dir,"/system")) key=No;
			if (stristr(go.dir,"/menu")) key=No;
			if (stristr(go.dir,"/user")) key=No;
			if (stristr(go.dir,"/bin")) key=No;
			if (stristr(go.dir,"/etc")) key=No;
			if (stristr(go.dir,"/id")) key=No;
			if (stristr(go.dir,"..")) key=No;
			if (!key) {
				mess("½Ã»ð¸¸ »ç¿ë°¡´ÉÇÑ ¸Þ´ºÅ¸ÀÔ(sde) ¶Ç´Â ºÒ°¡´É µð·ºÅä¸® ±¸Á¶");
				count=0;
				break;
			}
		}
		go.rec=count;
		count++;
		fwrite(&go,sizeof_go,1,fp);
		printf("\n");
	}
	fclose(fp);
	fclose(fp2);
	n=file_size(TMP_FILE2);
	printf(" ÃÑ %d °³ÀÇ ¸Þ´º°¡ Ãß°¡µÇ¾ú½À´Ï´Ù.  ÆÄÀÏÅ©±â:%d  ÀÚ·á¼ö:%d\n",count,n,n/sizeof_go);
	if (n>0&&count>0&&yesno2("»õ·Î µî·ÏÇÑ TEXT¸Þ´º·Î ÇöÀç ¸Þ´º¸¦ º¯°æÇÒ±î¿ä? (Y/n)>>",Yes)) {
		file_move(TMP_FILE2,NOW_MENU);
		mess("º¯°æÀÌ ¿Ï·áµÇ¾ú½À´Ï´Ù.");
	}
	else {
		mess("º¯°æÀÌ Ãë¼ÒµÇ¾ú½À´Ï´Ù.");
	}
	unlink(TMP_FILE2);
	sys_tem("rm -rf tmp/up%d%s",cfg.hostno,no_spc(tty));
}


check_topmenu()  //¸Þ´º0ÀÌ topÀÌ ¾Æ´Ï¸é topÀ¸·Î ÁöÁ¤
{
	FILE *fp;
	fp=fopen(NOW_MENU,"r+");
	if (fp==NULL) fp=fopen(NOW_MENU,"w+");
	if (fp==NULL) return;
	fseek(fp,0*sizeof_go,SEEK_SET);
	if (!fread(&go,sizeof_go,1,fp)||!is_same(go.code,"top")||
		go.type!='m'||go.deep!=0) {
		set_gotop();
		fseek(fp,0*sizeof_go,SEEK_SET);
		fwrite(&go,sizeof_go,1,fp);
	}
	fclose(fp);
}

edit_def(int mode)
{
	FILE *fp,*fp2;
	char buff[1500],now_menu[1500],filename[1500];
	int i,ii,count;
	GO old_club_go;
	old_club_go=club_go;

	if (mode==1) {
		del_space(club_go.dir);del_space(club_go.code);del_space(go.dir);
		sprintf(filename,"%s.sh",club_go.code);
        printf(" µ¿È£È¸:%s ÆÄÀÏ¸í:%s\n",club_go.code,filename);
		sprintf(now_menu,"%s/menustru/menu.dat",club_go.dir);
		fp=fopen(now_menu,"r");
        if (fp==NULL) {
            printf(" %s ¾øÀ½\n",now_menu);
            return;
        }
		fp2=fopen(filename,"w+");
		count=0;
		while (fread(&go,sizeof_go,1,fp)) {
			del_space(go.code);del_space(go.dir);
			if (strlen(go.code)>0&&is_typepds) {
				count++;
				fprintf(fp2,"mkdir forum/%s/pds/%s -p\n",club_go.code,go.code);
				fprintf(fp2,"mv %s/* forum/%s/pds/%s/\n",go.dir,club_go.code,go.code);
                printf("±â·Ï -> mv %s/* forum/%s/pds/%s/\n",go.dir,club_go.code,go.code);
			}
		}
		fclose(fp);
		if (count==0) unlink(filename);
		else sys_tem("chmod 777 %s",filename);
		fclose(fp2);
        printf("ÃÑ %d ÁÙÀ» %s ÆÄÀÏ¿¡ ±â·ÏÇÏ¿´½À´Ï´Ù.\n",count,filename);
        pressenter();
	}
	else if (mode==2) {
		sprintf(now_menu,"%s/menustru/menu.dat",club_go.dir);
        sys_tem("cp -f %s forum/%s.menu.dat",now_menu,club_go.code);
        printf("%s  %s º¯È¯Áß\n",club_go.code,now_menu);
		fp=fopen(now_menu,"r+");
		if (fp==NULL) return;
		ii=0;
		while (1) {
			fseek(fp,ii*sizeof_go,SEEK_SET);
			if (!fread(&go,sizeof_go,1,fp)) break;
			del_space(go.code);del_space(go.dir);
			if (strlen(go.code)>0&&is_typepds) {
				fseek(fp,ii*sizeof_go,SEEK_SET);
				printf("%d  %s %s ->",ii,go.code,go.dir);
				sprintf(go.dir,"forum/%s/pds/%s",club_go.code,go.code);
				printf("%s\n",go.dir);
				fseek(fp,ii*sizeof_go,SEEK_SET);
				fwrite(&go,sizeof_go,1,fp);
			}
            ii++;
		}
		fclose(fp);
		pressenter();
	}
	else if (mode==11) {
		sprintf(filename,"menustru/menu.dat");
		fp=fopen(filename,"r");
		if (fp==NULL) return;
		while (fread(&go,sizeof_go,1,fp)) {
			del_space(go.code);del_space(go.dir);
			if (strlen(go.code)>0&&go.type=='s') {
				club_go=go;
				edit_def(1);
			}
		}
		fclose(fp);
		pressenter();
	}
	else if (mode==12) {
		sprintf(filename,"menustru/menu.dat");
		fp=fopen(filename,"r");
		if (fp==NULL) return;
		while (fread(&go,sizeof_go,1,fp)) {
			del_space(go.code);del_space(go.dir);
			if (strlen(go.code)>0&&go.type=='s') {
				club_go=go;
				edit_def(2);
			}
		}
		fclose(fp);
	}
	else if (mode==13) {
		sprintf(filename,"menustru/menu.dat");
		fp=fopen(filename,"r+");
		if (fp==NULL) return;
		ii=0;
		while (1) {
			fseek(fp,ii*sizeof_go,SEEK_SET);
			if (!fread(&go,sizeof_go,1,fp)) break;
			del_space(go.code);del_space(go.dir);
			if (strlen(go.code)>0&&go.type=='s') {
				printf("%d  %s %s ->",ii,go.code,go.dir);
				fseek(fp,ii*sizeof_go,SEEK_SET);
				sprintf(go.dir,"forum/%s",go.code);
				fseek(fp,ii*sizeof_go,SEEK_SET);
				printf("%s\n",go.dir);
				fwrite(&go,sizeof_go,1,fp);
			}
			ii++;
		}
		fclose(fp);
		pressenter();
	}
	club_go=old_club_go;
}
