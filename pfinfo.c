////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���� : �û�޴� ���� ��ƾ
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

char idfile[300];
//char atit[10][30];
//char afile[10][300];
char filename5[200];
char atit[][30]={"ȸ������","���Խ�û","���Ժ���","���԰ź�","�������","ȸ��Ż��","ȸ������","ȸ����ü"};
char afile[][300]={ID_FILE,GUEST_FILE,GUESTWAI_FILE,GUESTNOT_FILE,GUESTCAN_FILE,GUESTOUT_FILE,GUESTDEL_FILE,GUESTDEP_FILE};
int nn,top,dsp;
int tot[10];
char nowid[10];
PF jobpf;
int allrec;


main(int argc, char **argv)
{
	char buff[100];
	who=1;
	strcpy(filename5,"");
	findport(tty);
	read_cfg();
	set_tmpfile();
	read_atype();	//����б�
	read_ajob();	//�����б�
	if (argc<2) {
		strcpy(nowid,"root");
		if (!check_pass("zz",8,"PASS >>")) return;
	}
	else		strcpy(nowid,argv[1]);

	if (is_same(nowid,"root")) who=5;
	else {
		if (!read_pf(nowid)) {
			mess("ID %s NOT FOUND",nowid);
			return;
		}
		if (pf.type>=18) who=4;
	}
	if (who==5) read_pf(cfg.sysop);
	jobpf=pf;
	dsp=inc_numfile(0,"system/pfinfod.dat");
	if (argc>2&&is_same(argv[2],"__edit_pf")) {pfinfo2("");return;}
	pfinfo(1);
	save_numfile(dsp,"system/pfinfod.dat");
}

pfinfo(int mode)
{
	FILE *fp1;
	int i,n,i2,linenum;
	char buff[255];
	char datebuff1[55];
	char datebuff2[55];
	char user_idfile[100];
	struct tm *tp;
	time_t today;
	int key,re_calc;

	key=Yes;
	top=0;
	re_calc=Yes;
	strcpy(user_idfile,"");
	while (1) {
		if (mode<1||mode>8) mode=1;
		printf("[H[s[0;37;44m[2J");
		if (re_calc) {
			for (i=0;i<8;i++) tot[i]=count_rec(afile[i],sizeof_pf);
		}
		strcpy(idfile,afile[mode-1]);
		if (strlen(user_idfile)>1) {
			if (!is_file(user_idfile)) {
				mess("%s ���Ͼ���",user_idfile);
				strcpy(user_idfile,"");
			}
			else strcpy(idfile,user_idfile);
		}
		if (!is_file(idfile)) touch_file(idfile);
		print_(2,"#%d. %s ȸ��%d ��û%d ����%d �ź�%d ���%d Ż��%d ����%d  ��Ÿ:%d W:%d",
		mode,atit[mode-1],tot[0],tot[1],tot[2],tot[3],tot[4],tot[5],tot[6],tot[7],who);
		printf("[0;1m%d ��ȣ [33m���̵�  [32m����    [37m",dsp);
			 if (dsp==1) printf("����  [36m���[37m �û�޸� Email  ����ID  ó������\n");
		else if (dsp==2) printf(" ���� �������� �������� ���� D:HH:MM   WR  UP  DN   MB\n");
		else if (dsp==3) printf("[36m���[37m����  CASH  ����  ���  �ٿ�   ����\n");
		else if (dsp==4) printf("���� ������� �ֹε�� �ּ�\n");
		else if (dsp==5) printf("����     �������   �ֹε��      ����\n");
		else if (dsp==6) printf("�ٿ�뷮 �ð��ܷ� ���Ա��� ��ü�׼� ȸ���ܾ�\n");
		else if (dsp==7) printf("�����ڵ� ���Գ�¥ �����̿� �̿��ܾ� ȸ���ܾ�\n");
		else if (dsp==8) printf("���Գ�¥ �ֹε��       ��ȣ     ���� �ּ�\n");
		else if (dsp==9) printf("    ��ȭ��ȣ    �ֹε��       ��ȣ     �ּ�[0;1m\n");
        else if (dsp==10) printf("�ֹε��    �����ȣ  ���ּ�  ��ȭ��ȣ\n");
		else {dsp=1;continue;}
		printfL2();
		if (top<0) top=0;
		if ((fp1=fopen(idfile,"r"))==NULL) {
			mess("%sȭ�� �������",idfile);
			mode++;
			continue;
		}
		linenum=0;
		i=top;
		if (i<=0) i=0;
		fseek(fp1,i*sizeof_pf,SEEK_SET);
		while(linenum<15) {
			if (!fread(&pf,sizeof_pf,1,fp1)) break;
            pf.id_no[14]=0;
			del_clear(pf.id);del_clear(pf.name);
//			  if (strlen(pf.id)<1) continue;
			if (pf.type<0||pf.type>19) pf.type=0;
			sprintf(datebuff1,"[35m%s[37m",datestr(0,pf.logout));
			sprintf(datebuff2,"[36m%s[37m",datestr(0,pf.inday ));
			if		(pf.type<0||pf.type>19) sprintf(buff,"[41mE%5d[37m",pf.num+1);
			else if (strlen(pf.id)<3) sprintf(buff,"[42mE%5d[37m",pf.num+1);
			else if (pf.type>=14) sprintf(buff,"[32mS%5d[37m",pf.num+1);
			else if (pf.type>= 1) sprintf(buff,"%6d",pf.num+1);
			else if (pf.type== 0) sprintf(buff,"[35mD%5d[37m",pf.num+1);
			printf("[0;1m%s [33m%-8s[32m%-8s[37m",buff,pf.id,pf.name);
            if      (dsp== 1) printf("%-8s[36m%2d[37m %-8s %-8s %-8s %-8s\n",pf.nick,pf.type,pf.user1,pf.email,pf.mailid,pf.gofirst);
            else if (dsp== 2) printf("%5d %s %s%5d%8.8s%5d%4d%4d%4dM\n",pf.level%100000,datebuff2,datebuff1,pf.all_log,datestr(26,pf.all_time),pf.countw,pf.countu,pf.countd,(pf.down/(1024*1024))%10000);
            else if (dsp== 3) printf("[36m%2d[37m%5d%7d %6d%6d%6d%5d/%5d\n",pf.type,pf.level%100000,pf.cash,pf.countw,pf.countu,pf.countd,pf.month_log,pf.all_log);
            else if (dsp== 4) printf("[0;1m%s %s %14s %-25.25s\n",pf.sex=='f'?"��":"��",dateyang(3,pf.birthday,pf.moon),pf.id_no,pf.addr);
            else if (dsp== 5) printf("[0;1m%-8s %s %14s %s\n",pf.nick,dateyang(3,pf.birthday,pf.moon),pf.id_no,ajob[pf.job]);
            else if (dsp== 6) printf("[0;1m%s %14s\n",dateyang(3,pf.birthday,pf.moon),pf.id_no);
            else if (dsp== 7) printf("[0;1m%s %14s\n",dateyang(3,pf.birthday,pf.moon),pf.id_no);
            else if (dsp== 8) printf("[0;1m%s %14s %-8s %-8s %-13.13s\n",datebuff2,pf.id_no,who>3&&pf.type<17?pf.passwd:"****",pf.nick,pf.addr);
            else if (dsp== 9) printf("[0;1m%15.15s %14s %-8s %-16.16s\n",pf.tele,pf.id_no,who>3&&pf.type<17?pf.passwd:"****",pf.addr);
            else if (dsp==10) printf("%s %s %s %s\n",pf.id_no,pf.pos1,pf.addr,pf.tele);
			linenum++;
		}
		fclose (fp1);
		printfL2();
        printf("[0;1m ����(E) ��ȣ����(EN,EN2,REN) ����(R) �˻�(S,S1,S2,S3,SS) ��¸��(DM)\n");
        printf("[0;1m ���(L) ��ȣ����(REN) ����(D,DD,PACK) ��ġ(T,B,LAST) ���(LS,LIST)\n");
        printf(" �׸�(. .1~.9) ȸ���̵�(m1~m9) (INDEX) ���(A),(H,DN)\n");
		if (strlen(user_idfile)>1) printf("UF:%s",user_idfile);
		printf(" ����>>");
		lineinput(buff,20);check_han(buff);check_cmd(buff);printf("\n");
		if (mmn[1]>0) nn=mmn[1];
		if		(is_same(mmc[0],"e"))       edit_pf();
		else if (is_same(mmc[0],"en"))      id_num_edit();
		else if (is_same(mmc[0],"en2"))     pfinfo2(mmc[1]);
		else if (is_same(mmc[0],"dn"))      down_id();
		else if (is_same(mmc[0],"er"))      disp_errpf(nn,mmn[2]);
		else if (is_same(mmc[0],"d"))       delete_pf(0);
		else if (is_same(mmc[0],"dd"))      delete_pf(2);
		else if (is_same(mmc[0],"r"))       delete_pf(1);
		else if (is_same(mmc[0],"pack"))    pack_pf();
		else if (is_same(mmc[0],"l"))       print_pf();
		else if (is_same(mmc[0],"ls"))      top=mmn[1]-1;
		else if (is_same(mmc[0],"list"))    list_pf(nn,idfile);
		else if (is_same(mmc[0],"last"))    top=tot[mode-1]-15;
		else if (is_same(mmc[0],"ren"))     renum_pf();
		else if (is_same(mmc[0],"a") )      {sys_tem("bin/pf %s",nowid);re_calc=Yes;}
		else if (is_same(mmc[0],"x"))       break;
		else if (is_same(mmc[0],"p")||is_same(mmc[0],"q")) break;
		else if (is_same(mmc[0],"xx")||is_same(mmc[0],"q")) break;
		else if (is_same(mmc[0],"s"))       ssearch_pf();
        else if (is_same(mmc[0],"s1"))      sys_tem("bin/pf '%s' set_search1",nowid);
        else if (is_same(mmc[0],"s2"))      sys_tem("bin/pf '%s' set_search2",nowid);
        else if (is_same(mmc[0],"s3"))      sys_tem("bin/pf '%s' set_search3",nowid);
        else if (is_same(mmc[0],"ss"))      sys_tem("bin/pf '%s'",nowid);
		else if (is_same(mmc[0],"t"))       top=0;
		else if (is_same(mmc[0],"b"))       backward();
		else if (is_same(mmc[0],"f")||strlen(buff)<1)      forward();
		else if (is_same(mmc[0],"dm"))      dsp++;
		else if (is_same(mmc[0],"dmm"))     dsp--;
		else if (is_same(mmc[0],"."))       mode++;
		else if (is_same(mmc[0],".."))      mode--;
		else if (is_same(mmc[0],".1"))      mode=1;
		else if (is_same(mmc[0],".2"))      mode=2;
		else if (is_same(mmc[0],".3"))      mode=3;
		else if (is_same(mmc[0],".4"))      mode=4;
		else if (is_same(mmc[0],".5"))      mode=5;
		else if (is_same(mmc[0],".6"))      mode=6;
		else if (is_same(mmc[0],".7"))      mode=7;
		else if (is_same(mmc[0],".8"))      mode=8;
		else if (is_same(mmc[0],"m"))       movepf(mode,0);
		else if (is_same(mmc[0],"m1"))      movepf(mode,1);
		else if (is_same(mmc[0],"m2"))      movepf(mode,2);
		else if (is_same(mmc[0],"m3"))      movepf(mode,3);
		else if (is_same(mmc[0],"m4"))      movepf(mode,4);
		else if (is_same(mmc[0],"m5"))      movepf(mode,5);
		else if (is_same(mmc[0],"m6"))      movepf(mode,6);
		else if (is_same(mmc[0],"m7"))      movepf(mode,7);
		else if (is_same(mmc[0],"m8"))      movepf(mode,8);
        else if (is_same(mmc[0],"index")&&strlen(mmc[1])<1)   make_ind(0,"make_ind_all");
        else if (is_same(mmc[0],"index")&&strlen(mmc[1])>0)   make_ind(0,mmc[1]);
		else if (is_same(mmc[0],"uf")) {printf("USER ID FILE>>");get_str(user_idfile,100);}
		else if (atoi(buff)>0)				disp_dispnum(atoi(buff));
	}
}


down_id()
{
	FILE *fp1,*fp2;
	int i,count=0;
	char filename[200];
	strcpy(filename,"tmp/id.tmp");
	if ((fp1=fopen(idfile,"r"))==NULL) return;
	if ((fp2=fopen(filename,"w"))==NULL) return;
	for (i=0;i<100;i++) {
		if (mmn[i]<1) continue;
		fseek(fp1,(mmn[i]-1)*sizeof_pf,SEEK_SET);
		if (!fread(&pf,sizeof_pf,1,fp1)) continue;
		printf("%6d %-8s %-8s  -> id.tmp�� ����\n",pf.num,pf.id,pf.name);
		pf.num=count;
//        fwrite(&pf,sizeof_pf,1,fp2);
        gwrite(101,pf,fp2);
		count++;
	}
	fclose(fp1);
	fclose(fp2);
	if (count<1) {
		mess("���� ID�� �����ϴ�.  DN ID��ȣ....");
	}
	else sz_file(filename);
	unlink(filename);
}

disp_errpf(int nn,int n2)
{
	FILE *fp1;
	if (nn<1) return;
	if (n2<1) n2=144;
	if ((fp1=fopen(idfile,"r"))==NULL) return;
	fseek(fp1,(nn-1)*sizeof_pf+n2,SEEK_SET);
	fread(&pf,sizeof_pf,1,fp1);
	fclose(fp1);
	view_pfw2(pf,who);
	pressenter();
}


list_pf(int mode,char *idfile)
{
	FILE *fp1,*fp2,*fp3,*fp4;
	char ch;
	char tid[9],filename[100];
	char buff[100],buff1[100],buff2[100],buff3[100],buff4[100];
	int i,found,count,count1,count2,count3,count4,count5;
	count=count1=count2=count3=count4=count5=0;

	if (mode== 1&&!yesno2("���� ID�� ����ұ��?(Y/n)>>",Yes)) return;
	if (mode== 2&&!yesno2("�û� ID�� ����ұ��?(Y/n)>>",Yes)) return;
	if (mode== 3&&!yesno2("���� ID�� ����ұ��?(Y/n)>>",Yes)) return;
	if (mode== 4&&!yesno2("�Ϲ�ȸ�� ID�� ����ұ��?(Y/n)>>",Yes)) return;
	if (mode== 9&&!yesno2("system/sameid1.id����(Y/n)>>",Yes)) return;
    if (mode==81&&!yesno2("�븮���ϼ���ID ����(Y/n)>>",Yes)) return;
	if (mode==89) {list_pf3(idfile);return;}
	if (mode==96&&!yesno2("�ߺ� �ֹε�� ID�� ����ұ��?(Y/n)>>",Yes)) return;
	if (mode==91&&!yesno2("�ߺ� ID�� �����ұ��?(Y/n)>>",Yes)) return;
	if (mode==97&&!yesno2("�ߺ� ID�� ����ұ��?(Y/n)>>",Yes)) return;
	if (mode==98&&!yesno2("��ҹ��� �ߺ� ID�� ����ұ��?(Y/n)>>",Yes)) return;
    if (mode==92) {
        if (yesno2("�ߺ� ID�� ����ұ��?(Y/n)>>",Yes)) {
            list_pf1(mode,idfile);
            return;
        }
    }
	if (mode==93&&!yesno2("�ߺ� ID�� KK20000�������� ����ұ��?(Y/n)>>",Yes)) return;
	if (mode==99) {
		if (yesno2("�̻� ����� 1�� ��ĥ���(Y/n)>>",Yes)) {
			list_pf2(idfile);return;
		}
	}

    if (mode==81) {
        if ((fp1=fopen(idfile,"r+"))==NULL) return;
        count1=0;
        while (1) {
            fseek(fp1,count1*sizeof_pf,SEEK_SET);
            if (!fread(&pf,sizeof_pf,1,fp1)) break;
            xstrncpy(pf.tmail,pf.mailid,8);
            strcpy(pf.mailid,"");
            pf.tmail[8]=0;
            del_space(pf.tmail);
            if (is_esc(pf.tmail)||strlen(pf.tmail)>8) strcpy(pf.tmail,"");
            fseek(fp1,count1*sizeof_pf,SEEK_SET);
            fwrite(&pf,sizeof_pf,1,fp1);
            count1++;
        }
        fclose(fp1);
        mess("�۾���");
        return;
    }


	if (mode==9) {
		strcpy(filename,"system/sameid1.id");
		fp2=fopen(TMP_FILE ,"w");
		fp3=fopen(TMP_FILE1,"w");
		fp4=fopen(TMP_FILE2,"w");
		for (i=0;i<3;i++) {
			if (i==1) strcpy(filename,"system/sameid2.id");
			if (i==2) strcpy(filename,"system/sameid3.id");
			if ((fp1=fopen(filename,"r"))==NULL) continue;
			while (fgets(buff,150,fp1)!=NULL) {
				del_enter(buff);
				sscanf(buff,"%s %s %s",buff2,buff3,buff4);
				if (read_pf(buff3)) {
					count1++;
					if (buff2[0]>='0'&&buff2[0]<='F')      fprintf(fp2,"%s\n",buff);
					else if (buff2[0]>='F'&&buff2[0]<='Z') fprintf(fp3,"%s\n",buff);
					else								   fprintf(fp4,"%s\n",buff);
				}
				else  {count2++;printf(" %s (����ó��)\n",buff);}
			}
			fclose(fp1);
		}
		fclose(fp2);
		fclose(fp3);
		fclose(fp4);
		file_move(TMP_FILE ,"system/sameid1.id");
		file_move(TMP_FILE1,"system/sameid2.id");
		file_move(TMP_FILE2,"system/sameid3.id");

		mess("�۾��Ϸ� �� %d�� ���� (�����ο�:%d��)",count2,count1);
		return;
	}

	if ((fp1=fopen(idfile,"r+"))==NULL) {
		mess("%sȭ�� �������",idfile);
		return;
	}
	fp2=fopen(TMP_FILE,"w+");
	unlink(TMP_FILE2);touch_file(TMP_FILE2);
	fp3=fopen(TMP_FILE2,"r+");
	while (fread(&pf,sizeof_pf,1,fp1)) {
		ch=' ';
			 if (pf.type== 0)			{ch='D';count1++;}
		else if (pf.type<0||pf.type>19) {ch='E';count2++;}
		else if (pf.type>18)			{ch='S';count3++;}
		if (mode==1&&ch!='E') continue;
		if (mode==2&&ch!='S') continue;
		if (mode==3&&ch!='D') continue;
		if (mode==4&&pf.type<14) continue;
		if (mode==91) { 	  //���Ͼ��̵� ����
			del_esc(pf.id);del_space(pf.id);
			if (strlen(pf.id)<3) continue;
			found=No;
			fseektop(fp3);
            printf("%d / %d  %-8s ó����\r",pf.num+1,tot[0],pf.id);
			while(fread(&pf2,sizeof_pf,1,fp3)) {
				if (is_same(pf.id,pf2.id)) {
					found=Yes;
					break;
				}
			}
			if (found) {
				count4++;
				count5++;
                fprintf(fp2,"   %5d %-8s %-8s %3d %8d %s\n",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(0,pf.inday));
                fprintf(fp2,"-> %5d %-8s %-8s %3d %8d %s\n",pf2.num+1,pf2.id,pf2.name,pf2.type,pf2.level,datestr(0,pf2.inday));
				if (pf.num>0) {
					strcpy(pf.id,"");strcpy(pf.name,"");
					strcpy(pf.nick,"");strcpy(pf.id_no,"");
					pf.type=0;
					fseek(fp1,pf.num*sizeof_pf,SEEK_SET);
//                    fwrite(&pf,sizeof_pf,1,fp1);
                    gwrite(102,pf,fp1);
				}
				continue;		 //�̹� �˻��� �� ���
			}
			else {
				fseekend(fp3);
				i=ftell(fp3)/sizeof_pf;
				fseek(fp3,i*sizeof_pf,SEEK_SET);
//                fwrite(&pf,sizeof_pf,1,fp3);
                gwrite(103,pf,fp3);
			}
			continue;
		}
		else if (mode==96) {	   //�˻��Ұ��
			del_esc(pf.id_no);del_space(pf.id_no);
			if (strlen(pf.id_no)<5) continue;
			del_esc(pf.id);del_space(pf.id);
			if (strlen(pf.id)<3) continue;
			found=No;
            printf("%d / %d  %-8s ó����\r",pf.num+1,tot[0],pf.id);
			fseektop(fp3);
			while(fread(&pf2,sizeof_pf,1,fp3)) {
				if (is_same(pf.id_no,pf2.id_no)) {
					found=Yes;
					break;
				}
			}
			if (found) {
				count4++;
				count5++;
                fprintf(fp2,"   %5d %-8s %-8s %3d %8d %s\n",pf.num+1,pf.id,pf.name,pf.type,pf.level,pf.id_no,datestr(0,pf.inday));
                fprintf(fp2,"-> %5d %-8s %-8s %3d %8d %s\n",pf2.num+1,pf2.id,pf2.name,pf2.type,pf2.level,pf2.id_no,datestr(0,pf2.inday));
				continue;		 //�̹� �˻��� �� ���
			}
			else {
				fseekend(fp3);
				i=ftell(fp3)/sizeof_pf;
				fseek(fp3,i*sizeof_pf,SEEK_SET);
//                fwrite(&pf,sizeof_pf,1,fp3);
                gwrite(104,pf,fp3);
			}
			continue;
		}
		else if (mode==97||mode==98||mode==92||mode==93) {		 //�˻��Ұ��
			del_esc(pf.id);del_space(pf.id);
			if (strlen(pf.id)<3) continue;
			found=No;
			fseektop(fp3);
            printf("%d / %d  %-8s ó����\r",pf.num+1,tot[0],pf.id);
			while(fread(&pf2,sizeof_pf,1,fp3)) {
				if (mode==97&&is_same(pf.id,pf2.id)) {
					found=Yes;
					break;
				}
				else if (mode==98&&is_same(pf.id,pf2.id)&&strcmp(pf.id,pf2.id)!=0) {
					found=Yes;
					break;
				}
				else if ((mode==92||mode==93)&&is_same(pf.id,pf2.id)&&strcmp(pf.id,pf2.id)!=0) {
					found=Yes;
					break;
				}
			}
			if (found) {
				count4++;
				count5++;
				if (mode==98) {
                    fprintf(fp2,"   %5d %-8s %-8s %3d %8d %s\n",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(0,pf.inday));
                    fprintf(fp2,"-> %5d %-8s %-8s %3d %8d %s\n",pf2.num+1,pf2.id,pf2.name,pf2.type,pf2.level,datestr(0,pf2.inday));
				}
				else if (mode==92) {
                    fprintf(fp2,"%-8s %-8s   %5d %5d (%-8s %-8s)  \n",pf.id,pf2.id,pf.num+1,pf2.num+1,pf.name,pf2.name);
				}
				else if (mode==93) {
                    fprintf(fp2,"%-8s kk%05d   %-8s %5d %5d (%-8s %-8s)  \n",pf.id,20000+count5,pf2.id,pf.num+1,pf2.num+1,pf.name,pf2.name);
				}
				continue;		 //�̹� �˻��� �� ���
			}
			else {
				fseekend(fp3);
				i=ftell(fp3)/sizeof_pf;
				fseek(fp3,i*sizeof_pf,SEEK_SET);
//                fwrite(&pf,sizeof_pf,1,fp3);
                gwrite(105,pf,fp3);
			}
			continue;
		}
		else {
			count4++;
            fprintf(fp2,"%c %5d %-8s %-8s %3d %8d %s\n",ch,pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(0,pf.inday));
		}
	}
	if (mode==91||mode==92||mode==93) ;
	else fprintf(fp2,"��:%d  ����:%d  ����:%d  �û�:%d\n",count4,count1,count2,count3);

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	view_text(TMP_FILE);
	unlink(TMP_FILE);
	unlink(TMP_FILE2);
}

//�ߺ�ID�˻�
list_pf1(int mode,char *idfile)
{
    FILE *fp1,*fp2;
    int ii;
    if ((fp1=fopen(idfile,"r"))==NULL) return;
	fp2=fopen(TMP_FILE,"w+");
    ii=0;
    while (1) {
        fseek(fp1,ii*sizeof_pf,SEEK_SET);
        if (!fread(&pf,sizeof_pf,1,fp1)) break;
        printf("%d / %d  %-8s ó����\r",pf.num+1,tot[0],pf.id);
        while(strlen(pf.id)>0&&fread(&pf2,sizeof_pf,1,fp1)) {
            if (strcmp(pf.id,pf2.id)==0) {
                fprintf(fp2,"�ߺ� %-8s %-8s   %5d %5d (%-8s %-8s)  \n",pf.id,pf2.id,pf.num+1,pf2.num+1,pf.name,pf2.name);
                break;
            }
        }
        ii++;
	}
	fclose(fp1);
    fclose(fp2);
	view_text(TMP_FILE);
}


list_pf2(char *idfile)
{
	FILE *fp1,*fp2;
	int count1,count2,count3,count4;
	count1=count2=count3=count4=0;
	if ((fp1=fopen(idfile,"r+"))==NULL) {
		mess("%sȭ�� �������",idfile);
		return;
	}
	count1=0;
	while (1) {
		fseek(fp1,count1*sizeof_pf,SEEK_SET);
		if (!fread(&pf,sizeof_pf,1,fp1)) break;
		if (pf.type<0||pf.type>19) {
            printf("%5d %-8s %-8s %4d -> 1  %8d %s\n",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(0,pf.inday));
			pf.type=1;

			fseek(fp1,count1*sizeof_pf,SEEK_SET);
//          fwrite(&pf,sizeof_pf,1,fp1);
            gwrite(106,pf,fp1);
		}
		count1++;
	}
	fclose(fp1);
	mess("End of job");
}


list_pf3(char *idfile)
{
	FILE *fp1,*fp2;
	char buff[200],buff1[200],oldid[100],newid[100];
	char samefile[100];
	int count1,count2,count3,count4;
	count1=count2=count3=count4=0;
	strcpy(samefile,"system/sameid9.id");
	if (!is_file(samefile)) {
		mess("%s ������ �����ϴ�.",samefile);
		return;
	}
	if ((fp1=fopen(idfile,"r+"))==NULL) {
		mess("%sȭ�� �������",idfile);
		return;
	}
	count1=0;
	while (1) {
		fseek(fp1,count1*sizeof_pf,SEEK_SET);
		if (!fread(&pf,sizeof_pf,1,fp1)) break;
        printf("%d / %d  %-8s ó����\r",pf.num+1,tot[0],pf.id);
		fp2=fopen(samefile,"r");
		while (fgets(buff,150,fp2)!=NULL) {
			del_enter(buff);strcpy(oldid,"");strcpy(newid,"");
			sscanf(buff,"%s %s %s",oldid,newid,buff1);
			if (strcmp(pf.id,oldid)==0&&strlen(newid)>2) {
                printf("%5d %-8s %-8s %4d %8d %s -> %s\n",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(0,pf.inday),newid);
				strcpy(pf.nick,pf.id);
				strcpy(pf.id,newid);
				strcpy(pf.email,newid);
				fseek(fp1,count1*sizeof_pf,SEEK_SET);
//                fwrite(&pf,sizeof_pf,1,fp1);
                gwrite(107,pf,fp1);
				break;
			}
		}
		fclose(fp2);
		count1++;
	}
	fclose(fp1);
	mess("End of job");
}


movepf(int mode,int moveto)
{
	int i,n,n2;
	FILE *fp1,*fp2;
	if (mode==0||moveto==0||mmn[1]<1) {
		printf(" �����׸�(.) ȸ��(.1)����(.2)����(.3)�ź�(.4)���(.5)Ż��(.6)����(.7)��ü(.8)\n");
		printf("����: m1 ��ȣ : �ش��ȣ �ڷ� ȸ�� �̵�\n");
		printf("        m2 ��ȣ : �ش��ȣ �ڷ� ���� �̵�\n");
		printf("        m3 ��ȣ : �ش��ȣ �ڷ� ���� �̵�\n");
		printf("        m4 ��ȣ : �ش��ȣ �ڷ� �ź� �̵�\n");
		printf("        m5 ��ȣ : �ش��ȣ �ڷ� ��� �̵�\n");
		printf("        m6 ��ȣ : �ش��ȣ �ڷ� Ż�� �̵�\n");
		printf("        m7 ��ȣ : �ش��ȣ �ڷ� ���� �̵�\n");
		printf("        m8 ��ȣ : �ش��ȣ �ڷ� ��ü �̵�\n");
		printf(" ��ȣ�� 1,2,3 �Ǵ� 1-99 ������ ���� ���� ����\n");
		pressenter();
		return;
	}
	if (mode==moveto) {
		mess("���� ���� %s �׸��� ȸ���Դϴ�.\n",atit[mode-1]);
		return;
	}
	if ((fp1=fopen(afile[mode-1],"r+"))==NULL) return;
	if ((fp2=fopen(afile[moveto-1],"r+"))==NULL) fp2=fopen(afile[moveto-1],"w+");
	for (i=1;i<100;i++) {
		if (mmn[i]<1) continue;
		fseek(fp1,(mmn[i]-1)*sizeof_pf,SEEK_SET);
		if (!fread(&pf,sizeof_pf,1,fp1)) continue;
		if (strlen(pf.id)<1) continue;
		n=count_frec(fp2,sizeof_pf);
		pf2=pf;
		pf2.num=n;
		if (n<0) n=0;
		fseek(fp2,n*sizeof_pf,SEEK_SET);
//        if (fwrite(&pf2,sizeof_pf,1,fp2)) {
        if (gwrite(108,pf2,fp2)) {
			fseek(fp1,(mmn[i]-1)*sizeof_pf,SEEK_SET);
			strcpy(pf.id,"");strcpy(pf.name,"");strcpy(pf.id_no,"");
//            fwrite(&pf,sizeof_pf,1,fp1);
            gwrite(110,pf,fp1);
			printf(" %5d. %-8s(%-8s) %s -> %s �̵�\n",mmn[i],pf2.id,pf2.name,atit[mode-1],atit[moveto-1]);
		}
	}
	fclose(fp1);
	fclose(fp2);
}


pack_pf()
{
	int i,n,n2,num,del;
	FILE *fp1,*fp2;
	char buff[200];
	char big[200];
	char aid[3001][9];
	printf("\n#######################################################\n");
	if (yesno("���� ȸ���� ���0�� ȸ���� ���� ��������ó�� �ұ��? (Y/n)",Yes)==No) return;
	printf("\n#######################################################\n");
	if ((fp1=fopen(idfile,"r+"))==NULL) return;
	if ((fp2=fopen(TMP_FILE1,"w+"))==NULL) return;
	if (!file_copy(idfile,TMP_FILE2)) {
		mess("���� ID ������� %s",TMP_FILE2);
	}
	n=0;
	del=0;
	while(fread(&pf,sizeof_pf,1,fp1)) {
		if (strlen(pf.id)<3||pf.type<1) {
			printf(" %5d. %-8s(%-8s) ����ó��\n",pf.num+1,pf.id,pf.name);
			if (del<3000) strcpy(aid[del++],pf.id);
		}
		else {
			pf.num=n++;
//            if (!fwrite(&pf,sizeof_pf,1,fp2)) {
              if (!gwrite(110,pf,fp2)) {
				mess("%s �������� ����",TMP_FILE1);
				fclose(fp1);
				fclose(fp2);
				unlink(TMP_FILE1);
				unlink(TMP_FILE2);
				return;
			}
		}
	}
	fclose(fp1);
	fclose(fp2);
	printf(".... ȸ���ڷ� ����................\n");
	if (del==0) {
		mess("������ �ڷᰡ �����ϴ�.");
		unlink(TMP_FILE1);
		unlink(TMP_FILE2);
		return;
	}
	if (!file_move(TMP_FILE1,idfile)) {
		mess("���� ���Ž���->�������Ϸ� ������");
		file_move(TMP_FILE2,idfile);
	}
	unlink(TMP_FILE1);
	unlink(TMP_FILE2);
	printf(".... ����� ���� ����.............\n");
	for (i=0;i<del;i++) {
		del_space(aid[i]);del_esc(aid[i]);
		if (strlen(aid[i])>0) {
			strcpy(big,c_han(aid[i]));
			sys_tem("rm -rf userdata/%s/%s",big,aid[i]);
			sys_tem("rm -rf userroom/%s/%s",big,aid[i]);
		}
	}
	printf("\n .......... �ڷ� PACK �Ϸ�.............\n");
	if (yesno("ȸ�� �ε����� ������ �ұ��? (Y/n)>>",Yes)) {
        make_ind(0,"make_ind_all");
	}
	printf("\n");
}


backward()
{
	FILE *fp;
	int i,last,linenum;
	if ((fp=fopen(idfile,"r"))==NULL) return;
	last=count_frec(fp,sizeof_pf);
	i=top;
	linenum=15;
	while(i>0&&linenum>0) {
		if (i>last) i=last;
		i--;
		if (i>=0) {
			fseek(fp,i*sizeof_pf,SEEK_SET);
			if (!fread(&pf,sizeof_pf,1,fp)) break;
			if (strlen(pf.id)<1) continue;
			linenum--;
		}
	}
	fclose (fp);
	top=i;
}


forward()
{
	FILE *fp;
	int i,linenum;
	if ((fp=fopen(idfile,"r"))==NULL) return;
	i=top;
	linenum=0;
	while(linenum<15) {
		if (!fread(&pf,sizeof_pf,1,fp)) break;
		if (strlen(pf.id)<1) continue;
		linenum++;
		i++;
	}
	fclose (fp);
	top=i;
}


disp_dispnum(int n)
{
	FILE *fp;
	int i,i2;
	int found=No;
	fp=fopen(idfile,"r");
	fseek(fp,(n-1)*sizeof_pf,SEEK_SET);
	if (n>0&&!feof(fp)&&fread(&pf2,sizeof_pf,1,fp)) found=Yes;
	fclose (fp);
	if (found) {
		view_pfw2(pf2,who);
		pressenter();
	}
}


edit_pf()
{
	FILE *fp;
	PF pf2;
	if (nn<1) return;
	if ((fp=fopen(idfile,"r"))==NULL) return;
	fseek(fp,(nn-1)*sizeof_pf,SEEK_SET);
	if (!fread(&pf2,sizeof_pf,1,fp)) {
		mess("%d ���� �ش�Ǵ� ȸ���� �����ϴ�.",nn);
	}
	else if (strlen(pf2.id)<3) {
		printf(" ## %d. %s�� �������� ID�� �ƴմϴ�.\n",nn,pf2.id);
		printf(" ## EN ��ȣ ����� ����ϼ���..");
		pressenter();
	}
	else {
		fclose(fp);
		if (who==5) sys_tem("bin/ed_pf root '%s' -9",pf2.id);
		else		sys_tem("bin/ed_pf '%s' '%s'",nowid,pf2.id);
		return;
	}
	fclose(fp);
}


//����
// mode==2 ��ĭ���� ����
delete_pf(int mode)
{
	FILE *fp;
	int i,old;
	fp=fopen(idfile,"r+");
	for (i=1;i<100;i++) {
		if (mmn[i]<1) continue;
		fseek(fp,(mmn[i]-1)*sizeof_pf,SEEK_SET);
		if (!fread(&pf,sizeof_pf,1,fp)) continue;
		if (who<4&&pf.type>=18) printf("*** 18�̻��� ����忡�� ����\n");
		else {
			old=pf.type;
			if (mode==1&&pf.type!=0) pf.type=old;
			else if (mode==2) {
				strcpy(pf.id_no,"");
				pf.type=0;
			}
			else pf.type=mode;
			if (old!=pf.type||mode==2) {
				if (mmn[i]>0) {
					fseek(fp,(mmn[i]-1)*sizeof_pf,SEEK_SET);
					pf.num=(mmn[i]-1);
//                    fwrite(&pf,sizeof_pf,1,fp);
                    gwrite(111,pf,fp);
					printf("*** %-8s(%-8s) ���  %2d ->% 2d ����\n",pf.id,pf.name,old,pf.type);
				}
			}
		}
	}
	fclose (fp);
}


//�˻�
ssearch_pf()
{
	FILE *fp;
	char buff[140],buff2[80];
	int mode;
	int key,ret;

	printf("1.��ü 2.ID 3.���� 4.�ֹε�� 6.��ȭ��ȣ 7.�ּ� 8.ȸ������ 0.��� >>");
	lineinput(buff,5);
	printf("\n");
	mode=atoi(buff);
	if (mode==0) return;
	printf("�˻��� ���ڸ� �Է��ϼ���. >>");
	lineinput(buff,20);
	printf("\n");
	if (buff[0]==0) return;

	fp=fopen(idfile,"r+");
	key=Yes;
	ret=No;
	while (key==Yes&&fread(&pf,sizeof_pf,1,fp)) {
		ret=No;
		switch (mode) {
		case (1) :
				   ret=(stristr(pf.id  ,buff)	||
						stristr(pf.name,buff)	||
						stristr(pf.id_no,buff)	||
						stristr(pf.tele,buff)	||
						stristr(pf.offi,buff)	||
						stristr(pf.memo1,buff)	 ||
                        stristr(pf.user1,buff)   ||
						stristr(pf.addr,buff) );
				   break;
		case (2) : ret=stristr(pf.id  ,buff)	  ;break;
		case (3) : ret=stristr(pf.name,buff)	  ;break;
		case (4) : ret=stristr(pf.id_no,buff)	  ;break;
		case (6) : ret=stristr(pf.tele,buff)	  ;break;
		case (7) : ret=stristr(pf.addr,buff)	  ;break;
		case (8) : if (pf.type==atoi(buff)) ret=Yes  ;break;
		default  : break;
		}
		if (ret) {
			view_pfw2(pf,who);
			if (yesno("\n�� ȸ���� �½��ϱ�? (Y:�ش��ڷ�� �̵� N:��Ӱ˻�)>>",Yes)) {
				nn=pf.num;
				ret=Yes;
				key=No;
			}
		}
	}
	if (fp!=NULL) fclose (fp);
	if (ret==Yes) top=pf.num;
}


//�˻� -> top �� ����
ssearch_pf2(char *buff)
{
	FILE *fp;
	PF pf3;
	fp=fopen(idfile,"r+");
	fseek(fp,top*sizeof_pf,SEEK_SET);
	while (fread(&pf3,sizeof_pf,1,fp)) {
		if (stristr(pf3.id	,buff)	 ||
			stristr(pf3.name,buff)	 ||
			stristr(pf3.id_no,buff)  ||
			stristr(pf3.tele,buff)	 ||
			stristr(pf3.offi,buff)	 ||
			stristr(pf3.memo1,buff)   ||
			stristr(pf3.user1,buff)   ||
			stristr(pf3.user2,buff)   ||
			stristr(pf3.user3,buff)   ||
			stristr(pf3.user4,buff)   ||
			stristr(pf3.addr,buff) ) {
				top=pf3.num;
				view_pfw2(pf3,who);
				pressenter();
				break;
			}
	}
	if (fp!=NULL) fclose (fp);
}


//���
print_pf()
{
	FILE *fp,*fp2;
	char buff[140],buff1[80],buff2[80];
	int mode;
	int key,ret;

	printf("\n  ��¾�� : 1.80�÷�  2.136�÷� 0.��� >>");
	mode=numinput();
	printf("\n");
	if (mode==0) return;
	fp=fopen(idfile,"r+");
	fp2=fopen(TMP_FILE,"w");
	key=Yes;
	ret=No;
	while (fread(&pf,sizeof_pf,1,fp)) {
		if (pf.id[0]!=0) {

			if (mode==1) {
				fprintf(fp2,"%4d %-8s %-8s %-8s%5d %2d% ",
				pf.num+1,pf.id,pf.name,pf.nick,pf.level,pf.type);
				fprintf(fp2,"%s %s %4d\n",
				datestr(0,pf.inday),datestr(0,pf.logout),pf.all_log);
			}
			else if(mode==2) {
				fprintf(fp2,"%4d %-8s %-8s %-8s%5d %2d ",
				pf.num+1,pf.id,pf.name,pf.nick,pf.level,pf.type);
				fprintf(fp2,"%s %s %4d",
				datestr(0,pf.inday),datestr(0,pf.logout),pf.all_log);
				fprintf(fp2," %14.14s %-50.50s\n",
				pf.id_no,pf.addr);
			}
		}
	}
	if (fp!=NULL) fclose (fp);
	if (fp2!=NULL) fclose (fp2);
	view_text(TMP_FILE);
}




disp_del_pf()
{
	FILE *fp;
	char buff1[100];
	char buff2[100];

	printf("\n ����ó�� ȸ�� Ȯ��\n");
	printfL2();
	fp=fopen(idfile,"r+");
	while (!feof(fp)) {
		if (!fread(&pf,sizeof_pf,1,fp)) break;
		if (pf.type==0||pf.id[0]==0||pf.warn%10==9) {
			printf("%5d.%-8s(%-8s) %2d %5d %2d %s %s\n",pf.num+1,pf.id,pf.name,pf.type,pf.level,pf.warn%10,
			datestr(0,pf.inday),datestr(0,pf.logout));
		}
	}
	printfL2();
	if (fp!=NULL) fclose (fp);
	pressenter();
}


new_month()
{
	FILE *fp;
	PF pf2;
	int i;
	fp=fopen(idfile,"r+");
	i=0;
	printf("\n ���ο� ���� ȸ�� �ڷḦ �����մϴ�.\n");
	printf("\n �����Ͻø� ȸ���� �����ӽð�/Ƚ���� ���� �ʱ�ȭ �˴ϴ�.\n");
	if (yesno(" �� ���� �۾��� �����ұ��? <Y/n>",Yes)) {
		printf("\n\n ��� ��ٷ��ּ���... ���� ���� �������Դϴ�.\n\n");
		while (1) {
			fseek(fp,i*sizeof_pf,SEEK_SET);
			if (fread(&pf2,sizeof_pf,1,fp)<=0) break;
			pf2.month_log=0;
			pf2.all_time=0; 				//���̿�ð���� (��)
			if (i>=0) {
				pf2.num=i;
				fseek(fp,i*sizeof_pf,SEEK_SET);
//                fwrite(&pf2,sizeof_pf,1,fp);
                gwrite(112,pf2,fp);
			}
			i++;
		}
		if (fp!=NULL) fclose (fp);
	}
}


pack_del_pf(int mode)
{
	FILE *fp,*fp2;
	char buff1[100],buff2[100];
	int n,allyes,i;
	time_t tdate;

	allyes=No;
	fp=fopen(idfile,"r+");
	if (mode==1||mode==2) {
		n=0;
		while (1) {
			fseek(fp,n*sizeof_pf,SEEK_SET);
			if (!fread(&pf,sizeof_pf,1,fp)) break;
			if (pf.type==0&&pf.id[0]!=0) {
				printf("%5d.%-8s(%-8s) %2d %5d ����ó��(Y/n/q/a) >>",pf.num+1,pf.id,pf.name,pf.type,pf.level);
				if (allyes) i=1;
				else		i=yesno("",Yes);
				if (i==-2) {allyes=Yes;i=Yes;}
				if (i==-1) break;
				if (i&&n>=0) {
					fseek(fp,n*sizeof_pf,SEEK_SET);
					strcpy(pf.id,"");strcpy(pf.name,"");
					if (mode==2&&pf.id_no[0]!=0) {
						printf(" --> �ֹε�� ������ ó��");
						if ((fp2=fopen("system/bad_idno.txt","a+"))!=NULL) {
							fprintf(fp2,"%s\n",pf.id_no);
							fclose (fp2);
						}
					}
					else printf(" --> ����");
					pf.num=n;
					strcpy(pf.id_no,"");
					fseek(fp,n*sizeof_pf,SEEK_SET);
//                    if (!fwrite(&pf,sizeof_pf,1,fp)) break;
                    if (!gwrite(113,pf,fp)) break;
				}
				else printf(" --> ����");
				printf("\n");
			}
			n++;
		}
	}
	else if (mode==3) {
		printf(" �������ӻ���ó�����س�¥  (��:96/12/31 ->96/12/31���� ���� ����)>>");
		lineinput(buff2,8);
		tdate=str2date(buff2);
		if (tdate>0) {
			n=0;
			while (1) {
				fseek(fp,n*sizeof_pf,SEEK_SET);
				if (n<0||!fread(&pf,sizeof_pf,1,fp)) break;
				if (pf.logout<tdate&&pf.login<tdate&&pf.inday<tdate&&pf.type>0&&pf.id[0]!=0) {
					printf("%5d.%-8s(%-8s) %2d %5d ����%s ����ó��(Y/n/q/a) >>",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(2,pf.logout));
					if (allyes) i=1;
					else		i=yesno("",Yes);
					if (i==-2) {allyes=Yes;i=Yes;}
					if (i==-1) break;
					if (i&&n>=0) {
						fseek(fp,n*sizeof_pf,SEEK_SET);
						pf.type=0;
						pf.num=n;
						fseek(fp,n*sizeof_pf,SEEK_SET);
//                        if (!fwrite(&pf,sizeof_pf,1,fp)) break;
                        if (!gwrite(114,pf,fp)) break;
						printf(" ->����");
					}
					else printf(" ->����");
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
				printf("%5d.%-8s(%-8s) %2d %5d ���9 ����%s ����ó��(Y/n/q/a) >>",pf.num+1,pf.id,pf.name,pf.type,pf.level,datestr(2,pf.logout));
				if (allyes) i=1;
				else		i=yesno("",Yes);
				if (i==-2) {allyes=Yes;i=Yes;}
				if (i==-1) break;
				if (i&&n>=0) {
					pf.type=0;
					pf.num=n;
					fseek(fp,n*sizeof_pf,SEEK_SET);
//                    if (!fwrite(&pf,sizeof_pf,1,fp)) break;
                    if (!gwrite(115,pf,fp)) break;
					printf(" ->����");
				}
				else printf(" ->����");
				printf("\n");
			}
			n++;
		}
	}
	else if (mode==5) {
		printf("\n���� �������� �ʽ��ϴ�.\n");
	}
	if (fp!=NULL) fclose(fp);
	printf("\n .......... �ڷ� ������ �Ϸ�.............\n");
	if (yesno("ȸ�� �ε����� ������ �ұ��? (Y/n)>>",Yes)) {
        make_ind(0,"make_ind_all");
	}
	printf("\n");
}


renum_pf()
{
	FILE *fp;
	int n,num;
	if ((fp=fopen(idfile,"r+"))==NULL) return;
	printf("\n .......... ��ȣ ������ �� ..............\n");
	n=0;
	while (1) {
		fseek(fp,n*sizeof_pf,SEEK_SET);
		if (n<0||!fread(&pf,sizeof_pf,1,fp)) break;
		pf.num=n;
		fseek(fp,n*sizeof_pf,SEEK_SET);
//        fwrite(&pf,sizeof_pf,1,fp);
        gwrite(116,pf,fp);
		n++;
	}
	fclose(fp);
	printf("\n ......�� %d ��.... ��ȣ ������ �Ϸ�.............\n",n);
	if (yesno("ȸ�� �ε����� ������ �ұ��? (Y/n)>>",Yes)) {
		printf("\n");
        make_ind(0,"make_ind_all");
	}
	printf("\n");
}


renum_proccode()
{
	FILE *fp;
	char buff2[100];
	int n,n2;
	int old;

	printf("\n");
	printf(" �� ����� ȸ���� �߼�/ó���� ���� �ĺ��ڵ带 ������ �ϴ� ����Դϴ�.\n");
	printf(" ������ ������ �����ϰ� ���� �����ǹǷ� �����Ͽ��� �մϴ�.\n");
	printf(" ���� �������� �Ͻðڽ��ϱ�? (y/N) >>");
	if (yesno("",No)==No) return;
	printf("\n �ĺ���ȣ �ο��� ���۹�ȣ�� �����ϼ���. (1~100000)>>");
	n2=numinput();
	if (n2<0) n2=1;
	if ((fp=fopen(idfile,"r+"))!=NULL) {
		n=0;
		while (1) {
			fseek(fp,n*sizeof_pf,SEEK_SET);
			if (n<0||!fread(&pf,sizeof_pf,1,fp)) break;
			if (pf.idnum!=n2) {
				pf.idnum=n2;
				pf.num=n;
				fseek(fp,n*sizeof_pf,SEEK_SET);
//                if (!fwrite(&pf,sizeof_pf,1,fp)) break;
                if (!gwrite(117,pf,fp)) break;
			}
			n++;
			n2++;
		}
		fclose(fp);
	}
	printf("\n .......... �ĺ���ȣ ������ �Ϸ�.............\n");
	pressenter();
}



id_num_edit()
{
	int n,n2;
	char buff[200];
	FILE *fp;

	if (mmn[1]<1) {
		printf("\n ������ ȸ����ȣ�� �Է��ϼ��� >>");
		lineinput(buff,8);printf("\n");
		n=atoi(buff);
	}
	else n=mmn[1];
	if (n<1) return;
	fp=fopen(idfile,"r+");
	fseek(fp,(n-1)*sizeof_pf,SEEK_SET);
	if (fread(&pf2,sizeof_pf,1,fp)) {
		printf(" ȸ����ȣ:%5d  ID:%-8s ����:%-8s ����:%-8s ���:%2d ����:%5d\n"
		,pf2.num+1,pf2.id,pf2.name,pf2.nick,pf2.type,pf2.level);
		printf("# ȸ�� ���� ID  :(%8s) >>",pf2.id);
		get_str(pf2.id,8);printf("  ->%s\n",pf2.id);
		printf("# ȸ�� ���� ����:(%8s) >>",pf2.name);
		get_str(pf2.name,8);printf("  ->%s\n",pf2.name);
		printf("# ȸ�� ���� ���:(%8s) >>",pf2.passwd);

		get_str(pf2.passwd,8);printf("  ->%s\n",pf2.passwd);

		if (who>=4) {
			printf("# ȸ�� ���� ���:(%d) >>",pf2.type);
			pf2.type=get_num(pf2.type,2);printf("  ->%d\n",pf2.type);
		}
		printf("# ȸ�� ���� ����:(%d) >>",pf2.level);
		pf2.level=get_num(pf2.level,7);printf("  ->%d\n",pf2.level);
		printf("# ȸ�� ���� �ֹε��:(%s) >>",pf2.id_no);
		get_str(pf2.id_no,14);printf("  ->%s\n",pf2.id_no);
		fseek(fp,(n-1)*sizeof_pf,SEEK_SET);
		pf2.num=n-1;
//        fwrite(&pf2,sizeof_pf,1,fp);
        gwrite(118,pf2,fp);
	}
	fclose(fp);
    make_ind(0,pf2.id);
}


check_pf(int mode,int who)
{
	int i=0;
	FILE *fp;
	char buff[100];
	int found=No;
	PF pf;

	printfL2();
	fp=fopen(idfile,"r+");
	while (fp!=NULL&&fread(&pf,sizeof_pf,1,fp)>0) {
		if ( (mode==1&&pf.type==0)||(mode==2&&pf.type>=16)) {
			printf(" %-8s(%-8s)%2d ",pf.id,pf.name,pf.type);
			if (i++%3==2) printf("\n");
			found=Yes;
		}
	}
	fclose(fp);
	if (!found) i=0;
	printf("\n  �� %d ���� �˻��Ǿ����ϴ�.\n",i);
	getchar();
}


pfinfo2(char *tid)
{
	FILE *fp;
	int i,i2,n,k;
	char buff[200];
	int  arec[100];
	PF	apf[100];
	PF oldpf;
	if (strlen(tid)>2&&read_pf(tid)) {oldpf=pf;goto re_start;}

	printf("\n ������ ID, ����Ǵ� �ֹε�Ϲ�ȣ�Ϻκ��� �����ϼ��� >>");
	lineinput2(buff,8);
	if (strlen(buff)<2) goto all_quit;
	if ((fp=fopen(ID_FILE,"r"))==NULL) goto all_quit;
	n=i=0;
	while(n<100&&fread(&pf,sizeof_pf,1,fp)) {
		if (stristr(pf.id,buff)||stristr(pf.name,buff)||
			stristr(pf.id_no,buff)||stristr(pf.offi,buff)||
			stristr(pf.tele,buff)||stristr(pf.addr,buff)) {
			pf.num=i;
			apf[n++]=pf;
		}
		i++;
	}
	//�Ѽ� �ľ�
	fseek(fp,1,SEEK_END);
	allrec=ftell(fp)/sizeof_pf;
	fclose(fp);
	if (n<1) {
		mess("%s �ڷḦ ã���� �����ϴ�.",buff);
		goto all_quit;
	}
	if (n>1) {
		printf("\n        < ������ ȸ���߿��� �����ϼ��� > \n");
		for (i=0;i<n;i++) {
			printf(" %3d. %-8s (%-8s) %14s %-30.30s\n",i+1,apf[i].id,apf[i].name,apf[i].id_no,apf[i].addr);
			if (i%20==19) getchar3();
		}
		printf(" ���� >>");
		lineinput(buff,9);
		i=atoi(buff);
		if (i<1||i>n) goto all_quit;
		apf[0]=apf[i-1];
	}
	pf=apf[0];
	oldpf=pf;

re_start:
	pf=oldpf;
	cls();
	disp_pf9();
	strcpy(buff,pf.id);
	xy(15,3);k=keyinput2(buff, 8);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
	if (strlen(buff)>0) strcpy(pf.id,buff);
	strcpy(buff,pf.name);
	xy(15,4);k=keyinput2(buff, 8);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
	if (strlen(buff)>0) strcpy(pf.name,buff);
	//�û��� �����Ұ����ϰ�
	if (who>4||pf.type<18) {
		strcpy(buff,pf.passwd);
		xy(15,5);k=keyinput2(buff, 8);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
		if (strlen(buff)>0) strcpy(pf.passwd,buff);
		sprintf(buff,"%d",pf.type);
		xy(15,6);k=keyinput2(buff, 3);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
		if (strlen(buff)>0) pf.type=atoi(buff);
		sprintf(buff,"%d",pf.level);
		xy(15,7);k=keyinput2(buff, 8);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
		if (strlen(buff)>0) pf.level=atoi(buff);
	}
	disp_pf9();
	sprintf(buff,"%d",pf.down);
	xy(15,8);k=keyinput2(buff,12);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
	if (strlen(buff)>0) pf.down=atoi(buff);
	sprintf(buff,"%d",pf.cash);
	xy(15,9);k=keyinput2(buff,12);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
	if (strlen(buff)>0) pf.cash=atoi(buff);
	sprintf(buff,"%s",pf.addr);
	xy(15,13);k=keyinput2(buff,70);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
	if (strlen(buff)>0) strcpy(pf.addr,buff);
	sprintf(buff,"%s",pf.tele);
	xy(15,14);k=keyinput2(buff,20);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
	if (strlen(buff)>0) strcpy(pf.tele,buff);
	sprintf(buff,"%s",pf.bibi);
	xy(15,15);k=keyinput2(buff,14);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
	if (strlen(buff)>0) strcpy(pf.bibi,buff);
	sprintf(buff,"%s",pf.hand);
	xy(15,16);k=keyinput2(buff,14);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
	if (strlen(buff)>0) strcpy(pf.hand,buff);
	sprintf(buff,"%s",pf.id_no);
	xy(15,17);k=keyinput2(buff,14);if (is_same(buff,"w")) goto save_ok;if (is_same(buff,"q")||k==3) goto save_no;if (is_same(buff,"r")) goto re_start;
	if (strlen(buff)>0) strcpy(pf.id_no,buff);

save_ok:
	cls();
	disp_pf9();
	printf(" �̴�� �����ұ��?  ����(Y) ���(q) ���Է�(r) >>");
	keyinput2(buff,10);check_han(buff);printf("\n");
	if (is_same(buff,"q")) goto all_quit;
	if (is_same(buff,"r")) goto re_start;
	if (pf.num>=0&&(fp=fopen(ID_FILE,"r+"))!=NULL) {
		fseek(fp,pf.num*sizeof_pf,SEEK_SET);
//        fwrite(&pf,sizeof_pf,1,fp);
        gwrite(119,pf,fp);
		fclose(fp);
	}
	goto all_quit;

save_no:
	cls();
	pf=oldpf;
	disp_pf9();
	printf(" ������� �����Ǿ����ϴ�. [Enter]");getchar3();
	goto all_quit;

all_quit:
	printf("\n");
	return;
}

disp_pf9()
{
	cls();
	printf("[0;1m << %s(%s)���� �Ż� ���� ȸ����ȣ : %d / %d >>\n",pf.id,pf.name,pf.num+1,allrec);
	printf("��������������������������������������������������������������������\n");
	printf(" 1. ID      : %-8s\n",pf.id);
	printf(" 2. ����    : %-8s\n",pf.name);
	printf(" 3. ��ȣ    : %-8s\n",pf.type>=18?starlen(pf.passwd):pf.passwd);
	printf(" 4. ���    : %d\n",pf.type);
	printf(" 5. ����    : %d\n",pf.level);
	printf(" 6. �ٿ�뷮: %d\n",pf.down);
	printf(" 7. ���̹�M : %d\n",pf.cash);
	printf("11. �ּ�    : %s\n",pf.addr);
	printf("12. ��ȭ    : %s\n",pf.tele);
	printf("13. �߻�    : %s\n",pf.bibi);
	printf("14. �ڵ���  : %s\n",pf.hand);
	printf("15. �ֹε��: %s\n",pf.id_no);
	printf("                \n");
	printf("��������������������������������������������������������������������\n");
	xy(46,3);printf("�����Ͻ� : %s\n",datestr(2,pf.inday));
	xy(46,4);printf("�������� : %s\n",datestr(2,pf.logout));
	xy(46,5);printf("���ð� : %s\n",datestr(21,pf.all_time));
	xy(46,6);printf("�����Ӽ� : %d\n",pf.all_log);
	xy(46,7);printf("�����   : %s\n",datestr(21,pf.month_time));
	xy(46,8);printf("�����Ӽ� : %d\n",pf.month_log);
	xy(1,18);
}

keyinput2(char *buff,int len)
{
	lineinput(buff,len);
	return 0;
}


