///////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���̹�Ȩ ������� �޴�
///////////////////////////////////////////////////////////////////////////
#include <string.h> /* strrev, strcpy */
#include "bbs.h"
int mode_lo1;
int total;
PF logpf,apf1[132],apf2[132],apf3[132];

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	char id[20];
	char tclub[20];
	int mode;
	if (argc<4) {
		printf("\n����  viewlog <mode> <ID> <club_code>\n");
		printf("mode 1:���ο�  2:����� 4:�������� ����\n");
		return;
	}

	findport(tty); //������Ʈ ã��
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
	read_cfg();
	mode_lo1=No;

	mode=atoi(argv[1]); 				//�޴���ȣ
	if (mode>=997) {
		rank(mode,argv[2],argv[3]);
		return;
	}
	strcpy(id,argv[2]); 				//���̵�
	del_space(id);del_esc(id);
	if (!is_same(id,"root")&&!read_pf(id)) {
		mess("ID %s �˻�����",id);
		return;
	}
	if (is_same(id,"root")) {
		pf.type=19;
	}

	strcpy(tclub,argv[3]);				//���̵�
	if (!find_club(tclub)) {
		mess("%s ��ȣȸ �˻� ����",tclub);
		return;
	}
	if	(mode==1)  view_log1(1);
	else if (mode==2)  view_log2(1);
	else if (mode==3)  view_log2(2);
	else if (mode==5)  view_log3(1);
	else if (mode==6)  view_log4(1);
        else if (mode==62) view_log4(4);
	else if (mode==7)  view_log4(2);
	else if (mode==8)  view_log4(3);
}


//���ӱ�Ϻ��� ���ο�
view_log1(int mode)
{
	FILE *fp,*fp2;
	char buff[500];
	char buff1[500];
	char filename[500];
	int i,n,nn,count;
	int year,month,day,job;
	if (guest_mode||pf.type<1) return;
	year=now_year();
	month=now_month();
	day=0;
	job=0;

	while (1) {
		if (year<=100) year+=1900;
		view("frm/log/mylog.scr");
		lineinput(buff,20);check_han(buff);printf("\n");n=atoi(buff);
		if (is_same(buff,".lo1")) mode_lo1=mode_lo1?No:Yes;
		if (is_same(buff,"h")) {view("frm/log/mylog.hlp");continue;}
		if (is_quit(buff)) break;
		if (n<1||n>9) continue;
		if (n==1) {
			year=now_year();month=now_month();day=0;job=23;
		}
		else if (n==2) {
			year=now_year();month=now_month();day=0;job=0;
		}
		else if (n==3) {
			printf("\n   �˻� �⵵  >>");
			lineinput(buff,10);year=atoi(buff);
			if (is_quit(buff)) break;
			if (strlen(buff)==0) year=now_year();
			printf(" ->  %d ��\n",year);
			printf("\n   �˻� ��    >>");
			lineinput(buff,10);month=atoi(buff);
			if (is_quit(buff)) break;
			if (month<1) month=now_month()%100;
			printf(" ->  %d ��\n",month);
			printf("\n   �˻� �����>>");
			lineinput(buff,10);day=atoi(buff);
			if (is_quit(buff)) break;
			printf(" ->  %d ��\n",day);
			printf("\n   �˻� �۾�  >>");
			lineinput(buff,10);job=atoi(buff);
			if (is_quit(buff)) break;
			printf(" ->  �۾���ȣ:%d\n",job);
		}
		else if (n==4) {
			year=now_year();month=now_month();day=0;job=99;
		}
		sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
		printf("\n   %s��. ��� ��ٷ� �ּ���.. �˻��� �����մϴ�... \n",pf.id);
		printf("     ��� ���� : %s  ũ��:%d byte \n",filename,file_size(filename));
		fflush(stdout);
		if (!is_file(filename)) {mess("%s ��� ȭ���� �����ϴ�.",filename);continue;}
		if ((fp=fopen(filename,"r"))==NULL) break;
		if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
		count=0;
		nn=0;
//	6-02 14:26	77 p5	������	  1 main	 top	  23 ����1 210.99.207.4 STA 0
//	6-02 14:26	77 p1	onex	 12 main	 top	  23 ����2 xhost.co.kr	END 6349

		while (fgets(buff,300,fp)!=NULL) {
			if (strlen(buff)<10||buff[0]==';') continue;
			xstrncpy(buff1,&buff[21],8);del_space(buff1);
			if (!is_same(buff1,pf.id)) continue;
			if (mode_lo1) printf("  BUFF:%s\n",buff);
			nn++;
			if (mode_lo1&&nn%100==99) pressenter();
			if (day>0) {
				xstrncpy(buff1,&buff[3],2);
				if (mode_lo1) printf("  day:%s\n",buff1);
				if (day!=atoi(buff1)) continue;
			}
			if (job>0) {
				xstrncpy(buff1,&buff[51],2);
				i=atoi(buff1);
				if (mode_lo1) printf("  job:%d\n",i);
				if (job==99&&i!=10&&i!=11&&i!=12) continue;
				if (job<99&&i!=job) continue;
			}
			count++;
			if (strlen(buff)>77) {buff[77]=0;strcat(buff,"\n");}
			fprintf(fp2,"%s",buff);    //�ش�Ǵ� ���� ������
		}
		fclose(fp);
		fclose(fp2);
		if (count<1) mess("�ش�Ǵ� ���ӱ���� �����ϴ�.");
		else		 view_text(TMP_FILE);
		unlink(TMP_FILE);
	}
}


//���ӱ�Ϻ��� �����
view_log2(int mode)
{
	FILE *fp,*fp2;
	char buff[500],buff1[500],buff2[500],tstr[100];
	char filename[500];
	char a_nid[500][10],a_ntty[500][10],a_nhost[500][30];
	char a_ncode[500][10],a_nclub[500][10];
	char st[30][100];
	int c_nid,c_ntty,c_nhost,c_ncode,c_nclub,re_aload=Yes;
	int i,n,n1,n2,nn,count,pr1;
	float pr2, pr3;
	int it1,it2,it3;
	int year,month,day1,day2,job1,job2,hour1,hour2,type1,type2,hostno1,hostno2;
	char tid[10],tcode[10],tclub[10],thost[30],ttty[30];
	char tfile[30],tid2[10],tip[30];
	PF oldpf;
	oldpf=pf;
	pr1=10;
	pr2=0.0;
	it1=it2=it3=0;
	c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;

	for (i=0;i<100;i++) {
		strcpy(a_nid[i],"");strcpy(a_ntty[i],"");strcpy(a_nhost[i],"");
		strcpy(a_ncode[i],"");strcpy(a_nclub[i],"");
	}
	if (pf.type<15) return;
	year=now_year();
	month=now_month();
	day1=day2=job1=job2=hour1=hour2=type1=type2=hostno1=hostno2=0;
	strcpy(tid,"");strcpy(tcode,"");strcpy(tclub,"");strcpy(thost,"");
	strcpy(ttty,"");strcpy(tstr,"");
	strcpy(tfile,"");strcpy(tid2,"");
	strcpy(tip,"");strcpy(tid2,"");
	strcpy(pf.user1,"");strcpy(pf.user2,"");strcpy(pf.user3,"");
	while (1) {
		if (re_aload) {
			c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;
			re_aload=No;
			for (i=0;i<500;i++) {
				strcpy(a_nid[i],"");strcpy(a_ntty[i],"");strcpy(a_nhost[i],"");
				strcpy(a_ncode[i],"");strcpy(a_nclub[i],"");
			}
			for (n=0;n<5;n++) {
					 if (n==0) sprintf(buff,"%s/f_nid.dat",REC_DIR);
				else if (n==1) sprintf(buff,"%s/f_ntty.dat",REC_DIR);
				else if (n==2) sprintf(buff,"%s/f_nhost.dat",REC_DIR);
				else if (n==3) sprintf(buff,"%s/f_ncode.dat",REC_DIR);
				else if (n==4) sprintf(buff,"%s/f_nclub.dat",REC_DIR);

				i=0;
				if ((fp=fopen(buff,"r"))!=NULL) {
					while (i<500&&fgets(buff,30,fp)) {
						del_enter(buff);del_space(buff);
							 if (n==0) strcpy(a_nid[i],buff);
						else if (n==1) strcpy(a_ntty[i],buff);
						else if (n==2) strcpy(a_nhost[i],buff);
						else if (n==3) strcpy(a_ncode[i],buff);
						else if (n==4) strcpy(a_nclub[i],buff);
						i++;
					}
					fclose(fp);
				}
					 if (n==0) c_nid=i;
				else if (n==1) c_ntty=i;
				else if (n==2) c_nhost=i;
				else if (n==3) c_ncode=i;
				else if (n==4) c_nclub=i;
			}
		}
		del_space(thost);del_space(ttty);del_space(tclub);del_space(tcode);
		del_space(tfile);del_space(tid);del_space(tid2);del_space(tstr);
		if (year<=100) year+=1900;
		cls();
		print_(1,"��ü ���� ��� Ȯ��");
		printf("  1. �ý� �۹�ȣ   : %d - %d\n",hostno1,hostno2);
        printf("  2. �˻� �����   : %d �� %d ��  %d �� - %d ��   %d �� - %d ��\n",year%100,month,day1,day2,hour1,hour2);
		printf("  3. �˻�   ID     : %s\n",tid);
		printf("  4. ���   ID     : %s\n",tid2);
		printf("  5. �˻�   ���   : %d - %d\n",type1,type2);
		printf("  6. ���� ȣ��Ʈ   : %s\n",thost);
		printf("  7. ����   tty    : %s\n",ttty);
		printf("  8. �˻� ��ȣȸ   : %s\n",tclub);
		printf("  9. �˻� �����ڵ� : %s\n",tcode);
		printf(" 10. �˻� �۾���ȣ : %d - %d\n",job1,job2);
		printf(" 11. �˻� ���ڿ�   : %s\n",tstr);
		printf(" 12. �˻� ���ϸ�   : %s\n",tfile);
		printf(" 13. ���� �ݾ����� : %d��/��  %5.1f��/KB\n",pr1,pr2);
		printf(" 14. ���� HOST IP  : %s\n",tip);
		printf(" 15. �˻�����ȯ����������\n");
		printf(" 91. ��ü ��� �˻�      \n");
		printf(" 92. �ڷ�Ǵٿ�˻�      \n");
		printf(" 96. ���� �˻�  (�Ϲ���) \n");
		printf(" 97. ���� �˻�  (������) \n");
		printf(" 98. ���� �˻�  (������) \n");
		printf(" 99. ���� �˻�  (�����׸�)\n");
		printf(" ���� >>");

		lineinput(buff,20);check_han(buff);printf("\n");n=atoi(buff);
		if (is_same(buff,".lo1")) mode_lo1=mode_lo1?No:Yes;
		if (is_same(buff,"h")) {view("frm/log/syslog.hlp");continue;}
		if (is_quit(buff)) break;
		if (n== 1) {printf(" ȣ��Ʈ��ȣ���� : ");hostno1=get_num(hostno1,3);printf("\n");
					printf(" ȣ��Ʈ��ȣ��   : ");hostno2=get_num(hostno2,3);printf("\n");}
		if (n== 2) {printf(" �����   YY : ");year =get_num(year ,4);printf("\n");
					printf(" �����   MM : ");month=get_num(month,2);printf("\n");
					printf(" ��  ���� DD : ");day1 =get_num(day1 ,2);printf("\n");
					printf(" �ϳ�     DD : ");day2 =get_num(day2 ,2);printf("\n");
					printf(" �ð����� HH : ");hour1=get_num(hour1,2);printf("\n");
					printf(" �ð���   HH : ");hour2=get_num(hour2,2);printf("\n");}
		if (n== 3) {printf(" �˻�ID      : ");get_str(tid,8);}
		if (n== 4) {printf(" ���ID      : ");get_str(tid2 , 8);}
		if (n== 5) {printf(" ��޽���    : ");type1=get_num(type1,2);printf("\n");
					printf(" ��޳�      : ");type2=get_num(type2,2);printf("\n");}
		if (n== 6) {printf(" ȣ��Ʈ��    : ");get_str(thost,20);}
		if (n== 7) {printf(" ����TTY     : ");get_str(ttty,8);}
		if (n== 8) {printf(" ���絿ȣȸ  : ");get_str(tclub,8);}
		if (n== 9) {printf(" ���簡���ڵ�: ");get_str(tcode,8);}
		if (n==10) {printf(" �۾����۹�ȣ: ");job1 =get_num(job1 ,2);printf("\n");
					printf(" �۾���  ��ȣ: ");job2 =get_num(job2 ,2);printf("\n");}
		if (n==11) {printf(" ���Թ��ڿ�  : ");get_str(tstr ,20);}
		if (n==12) {printf(" �˻����ϸ�  : ");get_str(tfile,15);}
		if (n==13) {printf(" �д���ݱݾ�: ");pr1=get_num(pr1, 4);printf("\n");
					printf(" KB����ݱݾ�(XX.X): ");pr2=get_float(pr2,7);printf("\n");}
		if (n==14) {printf(" ����ȣ��ƮIP: ");get_str(tip ,25);}
		if (n==15) {
			printf(" 1. f_nid.dat       \n");
			printf(" 2. f_ntty.dat      \n");
			printf(" 3. f_nhost.dat     \n");
			printf(" 4. f_ncode.dat     \n");
			printf(" 5. f_nclub.dat     \n");
			i=numinput();
			if (i==1) xedit2("%s/f_nid.dat",REC_DIR);
			if (i==2) xedit2("%s/f_ntty.dat",REC_DIR);
			if (i==3) xedit2("%s/f_nhost.dat",REC_DIR);
			if (i==4) xedit2("%s/f_ncode.dat",REC_DIR);
			if (i==5) xedit2("%s/f_nclub.dat",REC_DIR);
		}
		if (n==91) {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			if (mode==2) strchng2(filename,"/rec_","/bbs_");
			if (!is_file(filename)) {mess("%s ��� ȭ���� �����ϴ�.",filename);continue;}
			view_text(filename);
		}
		else if (n==92) {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			printf("   [%s] �˻��� �����մϴ�... \n",filename);
			if (!is_file(filename)) {mess("%s ��� ȭ���� �����ϴ�.",filename);continue;}
			if ((fp=fopen(filename,"r"))==NULL) break;
			if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
			count=it1=it2=it3=0;
			nn=0;
			fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			fprintf(fp2,"�� ���� �ð�  ##�� ��ȣȸ �������ڵ妭 ����� ��  �� �� ��  ��ũ�� K�� �ݾ� ��\n");    //�ش�Ǵ� ���� ������
			fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			while (fgets(buff,200,fp)!=NULL) {
				if (strlen(buff)<40||buff[0]==';') continue;
				nn++;
				if (mode_lo1&&nn%100==99) pressenter();
				if (mode_lo1) printf("  BUFF:%s\n",buff);
				for (i=0;i<30;i++) strcpy(st[i],"");
				sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
				strcpy(buff2,&buff[91]);
				sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
/*
1	   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 ��ȣ1 kslab						   STA 0 1
05-26 00:40  77 p1	 �û�	  19 main	  f1		8 �ٿ�1 kslab						   P �û�	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 �û�	  19 main	  f1		6 ���1 kslab						   P �û�	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 ����1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 ����2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 ����1 kslab		 STA 0
*/
				if (!is_same(st[10],"�ٿ�1")||!is_same(st[21],"P")) continue;
				if (strlen(tstr)>0) {
					if (mode_lo1) printf("  tstr:%s\n",buff);
					if (!stristr(buff,tstr)) continue;
				}
				if (strlen(tid)>0) {
					if (mode_lo1) printf("  tid:%s\n",st[5]);
					if (!is_same(tid,st[5])) continue;
				}
				if (day1>0) {
					xstrncpy(buff1,&buff[3],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  tday:%d\n",i);
					if (i<day1||i>day2) continue;
				}
				if (hour1!=0||hour2!=0) {
					xstrncpy(buff1,&buff[6],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  hour:%d\n",i);
					if (i<hour1||i>hour2) continue;
				}
				if (type1!=0||type2!=0) {
					i=atoi(st[6]);
					if (mode_lo1) printf("  type:%d\n",i);
					if (i<type1||i>type2) continue;
				}
				if (strlen(thost)>0) {
					if (mode_lo1) printf("  host:%s\n",st[11]);
					if (!stristr(st[11],thost)) continue;
				}
				if (hostno1!=0||hostno2!=0) {
					i=atoi(st[3]);
					if (mode_lo1) printf("  hostno:%d\n",i);
					if (i<hostno1||i>hostno2) continue;
				}
				if (strlen(ttty)>0) {
					if (mode_lo1) printf("  tty:%s\n",st[4]);
					if (!is_same(st[4],ttty)) continue;
				}
				if (strlen(tclub)>0) {
					if (mode_lo1) printf("  club:%s\n",st[7]);
					if (!is_same(st[7],tclub)) continue;
				}
				if (strlen(tcode)>0) {
					if (mode_lo1) printf("  code:%s\n",st[8]);
					if (!is_same(st[8],tcode)) continue;
				}
				if (job1!=0||job2!=0) {
					i=atoi(st[9]);
					if (mode_lo1) printf("  job:%d\n",i);
					if (i<job1||i>job2) continue;
				}
				if (strlen(tip)>0) {
					if (mode_lo1) printf("  tip:%s\n",st[11]);
					if (!stristr(st[11],tip)) continue;
				}
/*
P �û�	  autoexec.bat 280 ddd
P �û�	  bomul136.zip 374677
*/
				if (strlen(tfile)>0) {			 //���ϸ�˻�
					i=atoi(st[24]);
					if (mode_lo1) printf("  file:%s %d %s\n",st[23],i,buff2);
					if (!stristr(st[23],tfile)) continue;
				}
				if (strlen(tid2)>0) {  //ID�˻�
					if (mode_lo1) printf("  id:%s\n",st[2]);
					if (!is_same(st[2],tid2)) continue;
				}
				count++;
				n1=n2=0;

/*
1	   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 ��ȣ1 kslab						   STA 0 1
05-26 00:40  77 p1	 �û�	  19 main	  f1		8 �ٿ�1 kslab						   P �û�	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 �û�	  19 main	  f1		6 ���1 kslab						   P �û�	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 ����1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 ����2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 ����1 kslab		 STA 0
*/


				n1=atoi(st[24])/1024;
				it1+=n1;
				it3+=(n1*pr2)/10;
				sprintf(buff,"��%s %s%3s��%-8s��%-8s��%-8s��%-12.12s��%6d��%6.1f��",
							st[1],st[2],st[3],
							st[7],st[8],st[5],st[23],
							n1,(n1*pr2)/10);
				del_enter(buff);
				fprintf(fp2,"%s\n",buff);    //�ش�Ǵ� ���� ������
			}
			fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			fprintf(fp2,"��%4d��  ��%dKb  ����:%5.1f��/K  �Ѿ�:%s��\n",
						count,it1,pr2,n2c(it3));	//�ش�Ǵ� ���� ������
			fclose(fp);
			fclose(fp2);
			if (count<1) mess("�ش�Ǵ� ���ӱ���� �����ϴ�.");
			else		 view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
		else if (n>95) {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			printf("\n   ... ��� ��ٷ� �ּ���.. \n");
			printf("   [%s] �˻��� �����մϴ�... \n",filename);
			if (!is_file(filename)) {mess("%s ��� ȭ���� �����ϴ�.",filename);continue;}
			if ((fp=fopen(filename,"r"))==NULL) break;
			if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
			count=it1=it2=it3=0;
			nn=0;
			if (n==97) {
			fprintf(fp2,"������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			fprintf(fp2,"������    �ð��������ڵ妭 ����� �� �۾� �����ݾצ�\n");    //�ش�Ǵ� ���� ������
			fprintf(fp2,"������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			}
			if (n==99) { //05-26 00:31 12 123
			fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			fprintf(fp2,"������   �ð� ## tty�������ڵ妭 ����� �� �۾� �� ��� �����ݦ���ݦ� �ݾ� ��\n");    //�ش�Ǵ� ���� ������
			fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			}
			while (fgets(buff,200,fp)!=NULL) {
				if (strlen(buff)<40||buff[0]==';') continue;
				nn++;
				if (mode_lo1&&nn%100==99) pressenter();
				if (mode_lo1) printf("  BUFF:%s\n",buff);
				for (i=0;i<30;i++) strcpy(st[i],"");
				sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
				strcpy(buff2,&buff[91]);
				sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
/*
1	   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 ��ȣ1 kslab						   STA 0 1
05-26 00:40  77 p1	 �û�	  19 main	  f1		8 �ٿ�1 kslab						   P �û�	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 �û�	  19 main	  f1		6 ���1 kslab						   P �û�	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 ����1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 ����2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 ����1 kslab		 STA 0
*/
				if (strlen(tstr)>0) {
					if (mode_lo1) printf("  tstr:%s\n",buff);
					if (!stristr(buff,tstr)) continue;
				}
				if (strlen(tid)>0) {
					if (mode_lo1) printf("  tid:%s\n",st[5]);
					if (!is_same(tid,st[5])) continue;
				}
				if (day1>0) {
					xstrncpy(buff1,&buff[3],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  tday:%d\n",i);
					if (i<day1||i>day2) continue;
				}
				if (hour1!=0||hour2!=0) {
					xstrncpy(buff1,&buff[6],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  hour:%d\n",i);
					if (i<hour1||i>hour2) continue;
				}
				if (type1!=0||type2!=0) {
					i=atoi(st[6]);
					if (mode_lo1) printf("  type:%d\n",i);
					if (i<type1||i>type2) continue;
				}
				if (strlen(thost)>0) {
					if (mode_lo1) printf("  host:%s\n",st[11]);
					if (!stristr(st[11],thost)) continue;
				}
				if (hostno1!=0||hostno2!=0) {
					i=atoi(st[3]);
					if (mode_lo1) printf("  hostno:%d\n",i);
					if (i<hostno1||i>hostno2) continue;
				}
				if (strlen(ttty)>0) {
					if (mode_lo1) printf("  tty:%s\n",st[4]);
					if (!is_same(st[4],ttty)) continue;
				}
				if (strlen(tclub)>0) {
					if (mode_lo1) printf("  club:%s\n",st[7]);
					if (!is_same(st[7],tclub)) continue;
				}
				if (strlen(tcode)>0) {
					if (mode_lo1) printf("  code:%s\n",st[8]);
					if (!is_same(st[8],tcode)) continue;
				}
				if (job1!=0||job2!=0) {
					i=atoi(st[9]);
					if (mode_lo1) printf("  job:%d\n",i);
					if (i<job1||i>job2) continue;
				}
				if (strlen(tip)>0) {
					if (mode_lo1) printf("  tip:%s\n",st[11]);
					if (!stristr(st[11],tip)) continue;
				}
/*
P �û�	  autoexec.bat 280 ddd
P �û�	  bomul136.zip 374677
*/
				if (is_same(st[21],"P")&&strlen(tfile)>0) {           //���ϸ�˻�
					i=atoi(st[24]);
					if (mode_lo1) printf("  file:%s %d %s\n",st[23],i,buff2);
					if (!stristr(st[23],tfile)) continue;
				}
				if ((is_same(st[21],"B")||is_same(st[21],"P"))&&strlen(tid2)>0) {  //ID�˻�
					if (mode_lo1) printf("  id:%s\n",st[2]);
					if (!is_same(st[2],tid2)) continue;
				}
				count++;
				n1=n2=0;
				if (is_same(st[12],"STA")||is_same(st[12],"END")) {
					n1=atoi(st[13]);it1+=n1;
					n2=(n1+30)/60;it2+=n2;
					it3+=(n2*pr1);
				}
				if (n==97) sprintf(buff,"��%s  %s��%-8s��%-8s��%-6s��%7d ��",
							st[1],st[2],st[8],st[5],st[10],n2*pr1);
				if (n==98) sprintf(buff,"%s %s%3s %3s %-8s %-8s %-6s %6d %4d %4d %6d",
							st[1],st[2],st[3],st[4],st[8],st[5],st[10],n1,n2,pr1,n2*pr1);
				if (n==99) sprintf(buff,"��%s %s%3s %3s��%-8s��%-8s��%-6s��%6d��%4d��%4d��%6d��",
							st[1],st[2],st[3],st[4],st[8],st[5],st[10],n1,n2,pr1,n2*pr1);
				del_enter(buff);
				fprintf(fp2,"%s\n",buff);    //�ش�Ǵ� ���� ������
			}
			if (n==97) fprintf(fp2,"������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			if (n==99) fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			fprintf(fp2,"��%4d��  ���:%d��  ����:%d��  �Ѿ�:%s��\n",
						count,it1,it2,n2c(it3));	//�ش�Ǵ� ���� ������
			fclose(fp);
			fclose(fp2);
			if (count<1) mess("�ش�Ǵ� ���ӱ���� �����ϴ�.");
			else		 view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
	}
	pf=oldpf;
}



//���ӱ�Ϻ��� �����
view_log3(int mode)
{
	FILE *fp,*fp2;
	char buff[500],buff1[500],buff2[500],tstr[100];
	char filename[500];
	char a_nid[500][10],a_ntty[500][10],a_nhost[500][30];
	char a_ncode[500][10],a_nclub[500][10];
	char st[30][100];
	int c_nid,c_ntty,c_nhost,c_ncode,c_nclub,re_aload=Yes;
	int i,n,n1,n2,nn,count=0,pr1;
	int it1,it2,it3;
	int year,month,day1,day2,job1,job2,hour1,hour2,type1,type2,hostno1,hostno2;
	char tid[10],tcode[10],tclub[10],thost[30],ttty[30];
	char tfile[30],tid2[10],tip[30];
	PF oldpf;
	oldpf=pf;
	pr1=10;
	it1=it2=it3=0;
	c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;

	for (i=0;i<100;i++) {
		strcpy(a_nid[i],"");strcpy(a_ntty[i],"");strcpy(a_nhost[i],"");
		strcpy(a_ncode[i],"");strcpy(a_nclub[i],"");
	}
	if (pf.type<15) return;
	year=now_year();
	month=now_month();
	day1=day2=job1=job2=hour1=hour2=type1=type2=hostno1=hostno2=0;
	strcpy(tid,"");strcpy(tcode,"");strcpy(tclub,"");strcpy(thost,"");
	strcpy(ttty,"");strcpy(tstr,"");
	strcpy(tfile,"");strcpy(tid2,"");
	strcpy(tip,"");strcpy(tid2,"");
	strcpy(pf.user1,"");strcpy(pf.user2,"");strcpy(pf.user3,"");
	while (1) {
		del_space(thost);del_space(ttty);del_space(tclub);del_space(tcode);
		del_space(tfile);del_space(tid);del_space(tid2);del_space(tstr);
		if (year<=100) year+=1900;
		cls();
		print_(1,"����۾�");
		printf("  1. HOSTNO                 : %d - %d\n",hostno1,hostno2);
        printf("  2. ����/�� (YYMM)         : %02d/%02d\n",year%100,month);
		printf("  3. ������-������          : %d - %d\n",day1,day2);
		printf("  4. �˻����               : %d - %d\n",type1,type2);
		printf("  5. �˻���ȣȸ             : %s\n",tclub);
		printf("  6. �˻������ڵ�           : %s\n",tcode);
		printf("  7. �˻� ID                : %s\n",tid);
		printf("  8. ��� ID                : %s\n",tid2);
		printf("  9. ���Թ��ڿ�             : %s\n",tstr);
		printf(" 10. HOST IP                : %s\n",tip );
		printf(" 91. ��ü   ��� �ð� ��� (����-����)  \n");
		printf(" 92. HOSTNO ��� �ð� ���              \n");
		printf(" 93. �Ϻ�   ��� �ð� ���              \n");
		printf(" 94. ��ȣȸ ��� �ð� ���              \n");
		printf(" 95. �Խ��� ��� �ð� ���              \n");
		printf(" 96. ���ID ��ȸ/�ٿ� ���              \n");
		n=numselect();
		if (n<= 0) break;
		if (n== 1) {printf(" ȣ��Ʈ��ȣ���� : ");hostno1=get_num(hostno1,3);printf("\n");
					printf(" ȣ��Ʈ��ȣ��   : ");hostno2=get_num(hostno2,3);printf("\n");}
		if (n== 2) {printf(" �����   YY : ");year =get_num(year ,4);
					printf(" �����   MM : ");month=get_num(month,2);}
		if (n== 3) {printf(" ������   DD : ");day1 =get_num(day1 ,2);printf("\n");
					printf(" ������   DD : ");day2 =get_num(day2 ,2);printf("\n");}
		if (n== 4) {printf(" ��޽���    : ");type1=get_num(type1,2);printf("\n");
					printf(" ��޳�      : ");type2=get_num(type2,2);printf("\n");}
		if (n== 5) {printf(" �˻���ȣȸ  : ");get_str(tclub,8);}
		if (n== 6) {printf(" �˻������ڵ�: ");get_str(tcode,8);}
		if (n== 7) {printf(" �˻� ID     : ");get_str(tid,8);}
		if (n== 8) {printf(" ��� ID      : ");get_str(tid2 , 8);}
		if (n== 9) {printf(" ���Թ��ڿ�  : ");get_str(tstr ,20);}
		if (n==10) {printf(" ����ȣ��ƮIP: ");get_str(tip ,25);}

		if (n>90) {
			sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
			printf("\n   ... ��� ��ٷ� �ּ���.. \n");
			printf("   [%s] �˻��� �����մϴ�... \n",filename);
			if (!is_file(filename)) {mess("%s ��� ȭ���� �����ϴ�.",filename);continue;}
			if ((fp=fopen(filename,"r"))==NULL) break;
			if ((fp2=fopen(TMP_FILE,"w"))==NULL) break;
			nn=0;
			while (fgets(buff,200,fp)!=NULL) {
				if (strlen(buff)<40||buff[0]==';') continue;
				nn++;
				for (i=0;i<30;i++) strcpy(st[i],"");
				sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
				strcpy(buff2,&buff[91]);
				sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
/*
1     2	     3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 ��ȣ1 kslab						   STA 0 1
05-26 00:40  77 p1	 �û�	  19 main	  f1		8 �ٿ�1 kslab						   P �û�	 autoexec.bat 280 ddd
05-26 00:40  77 p1	 �û�	  19 main	  f1		6 ���1 kslab						   P �û�	 bomul136.zip 374677
06-16 01:45  77 p0	 xxxx	   4 main	  top	   23 ����1 .							   STA 0
06-15 23:56  77 p0	 xxxx	   4 main	  top	   23 ����2 kslab		 END 140
06-15 23:54  77 p0	 xxxx	   4 main	  top	   23 ����1 kslab		 STA 0
*/
				if (strlen(tstr)>0) {
					if (mode_lo1) printf("  tstr:%s\n",buff);
					if (!stristr(buff,tstr)) continue;
				}
				if (strlen(tid)>0) {
					if (mode_lo1) printf("  tid:%s\n",st[5]);
					if (!is_same(tid,st[5])) continue;
				}
				if (day1>0) {
					xstrncpy(buff1,&buff[3],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  tday:%d\n",i);
					if (i<day1||i>day2) continue;
				}
				if (hour1!=0||hour2!=0) {
					xstrncpy(buff1,&buff[6],2);
					i=atoi(buff1);
					if (mode_lo1) printf("  hour:%d\n",i);
					if (i<hour1||i>hour2) continue;
				}
				if (type1!=0||type2!=0) {
					i=atoi(st[6]);
					if (mode_lo1) printf("  type:%d\n",i);
					if (i<type1||i>type2) continue;
				}
				if (strlen(thost)>0) {
					if (mode_lo1) printf("  host:%s\n",st[11]);
					if (!stristr(st[11],thost)) continue;
				}
				if (hostno1!=0||hostno2!=0) {
					i=atoi(st[3]);
					if (mode_lo1) printf("  hostno:%d\n",i);
					if (i<hostno1||i>hostno2) continue;
				}
				if (strlen(ttty)>0) {
					if (mode_lo1) printf("  tty:%s\n",st[4]);
					if (!is_same(st[4],ttty)) continue;
				}
				if (strlen(tclub)>0) {
					if (mode_lo1) printf("  club:%s\n",st[7]);
					if (!is_same(st[7],tclub)) continue;
				}
				if (strlen(tcode)>0) {
					if (mode_lo1) printf("  code:%s\n",st[8]);
					if (!is_same(st[8],tcode)) continue;
				}
				if (job1!=0||job2!=0) {
					i=atoi(st[9]);
					if (mode_lo1) printf("  job:%d\n",i);
					if (i<job1||i>job2) continue;
				}
				if (strlen(tip)>0) {
					if (mode_lo1) printf("  tip:%s\n",st[11]);
					if (!stristr(st[11],tip)) continue;
				}
/*
P �û�	  autoexec.bat 280 ddd
P �û�	  bomul136.zip 374677
*/
				if (is_same(st[21],"P")&&strlen(tfile)>0) {           //���ϸ�˻�
					i=atoi(st[24]);
					if (mode_lo1) printf("  file:%s %d %s\n",st[23],i,buff2);
					if (!stristr(st[23],tfile)) continue;
				}
				if ((is_same(st[21],"B")||is_same(st[21],"P"))&&strlen(tid2)>0) {  //ID�˻�
					if (mode_lo1) printf("  id:%s\n",st[2]);
					if (!is_same(st[2],tid2)) continue;
				}
				count++;
				n1=n2=0;
				if (is_same(st[12],"STA")||is_same(st[12],"END")) {
					n1=atoi(st[13]);it1+=n1;
					n2=(n1+30)/60;it2+=n2;
					it3+=(n2*pr1);
				}
				if (n==97) sprintf(buff,"��%s  %s��%-8s��%-8s��%-6s��%7d ��",
							st[1],st[2],st[8],st[5],st[10],n2*pr1);
				if (n==98) sprintf(buff,"%s %s%3s %3s %-8s %-8s %-6s %6d %4d %4d %6d",
							st[1],st[2],st[3],st[4],st[8],st[5],st[10],n1,n2,pr1,n2*pr1);
				if (n==99) sprintf(buff,"��%s %s%3s %3s��%-8s��%-8s��%-6s��%6d��%4d��%4d��%6d��",
							st[1],st[2],st[3],st[4],st[8],st[5],st[10],n1,n2,pr1,n2*pr1);
				del_enter(buff);
				fprintf(fp2,"%s\n",buff);    //�ش�Ǵ� ���� ������
			}
			if (n==97) fprintf(fp2,"������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			if (n==99) fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
			fprintf(fp2,"��%4d��  ���:%d��  ����:%d��  �Ѿ�:%d��\n",
						count,it1,it2,it3);    //�ش�Ǵ� ���� ������
			fclose(fp);
			fclose(fp2);
			if (count<1) mess("�ش�Ǵ� ���ӱ���� �����ϴ�.");
			else		 view_text(TMP_FILE);
			unlink(TMP_FILE);
		}
	}
	pf=oldpf;
}


//mode==1:���� 2:�ٿ�
view_log4(int mode)
{
	FILE *fp,*fp2;
	char buff[500],buff1[500],buff2[500],tstr[100];
	char filename[500];
	char a_nid[500][10],a_ntty[500][10],a_nhost[500][30];
	char a_ncode[500][10],a_nclub[500][10];
	char st[30][100];
	int c_nid,c_ntty,c_nhost,c_ncode,c_nclub,re_aload=Yes;
	int i,n,n1,n2,count;
	float pr1,pr2,pr3;
	int it1,it2,it3;
	int year,month,day1,day2,job1,job2,hour2,type1,type2,hostno1,hostno2;
	char tid[10],tcode[10],tclub[10],thost[30],ttty[30];
	char tfile[30],tid2[10],tip[30];
	PF oldpf;
	oldpf=pf;
	it1=it2=it3=0;
	c_nid=c_ntty=c_nhost=c_ncode=c_nclub=0;

	for (i=0;i<100;i++) {
		strcpy(a_nid[i],"");strcpy(a_ntty[i],"");strcpy(a_nhost[i],"");
		strcpy(a_ncode[i],"");strcpy(a_nclub[i],"");
	}
	if (pf.type<15) return;
	year=now_year();
	month=now_month();
	day1=day2=0;
	sprintf(buff,"%02d%02d",year%100,month);
	printf(" ���Ȯ�γ�� (YYMM)>>");
	dosinput(buff,4);printf("\n");
	i=atoi(buff);year=i/100;month=i%100;
	printf(" ���Ȯ�ν����� (1~31)>>");
	day1=numinput();printf("\n");if (day1<1||day1>31) day1=1;
	printf(" ���Ȯ��  ���� (1~31)>>");
	day2=numinput();printf("\n");if (day2<1||day2>31) day2=31;
	pr1=9.0;    //���� ȣ��Ʈ ��ȣ 30������ ���ӽ�
	pr3=15.0;    //���� ȣ��Ʈ ��ȣ 50, 51,52 ������ ���ӽ� ����
	pr2=2.0;
	if (mode==1||mode==3||mode==4) {
		printf(" 014xy ���� �д� �ݾ� (%5.1f) >>",pr1);pr1=get_float(pr1,6);printf("\n");
	}
	if (mode==1||mode==3||mode==4) {
		printf(" �Ϲ���ȭ �д� �ݾ� (%5.1f)>>",pr3);pr3=get_float(pr3,6);printf("\n");
	}
	else if (mode==2){
		printf(" KB��   �ݾ� (%5.1f)>>",pr2);pr2=get_float(pr2,6);printf("\n");
	}

	sprintf(filename,"%s/rec_%02d%02d.txt",REC_DIR,year%100,month);
	printf("     ��� ���� : %s  ũ��:%d byte \n",filename,file_size(filename));
	if (!is_file(filename)) {mess("%s ��� ȭ���� �����ϴ�.",filename);return;}
	if ((fp=fopen(filename,"r"))==NULL) return;
	if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
	count=it1=it2=it3=0;
	if (mode==1) {
		fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
		fprintf(fp2,"�� ���� �ð�  ##�� ����� ��TTY ��         ȣ��Ʈ         �� �ð� ��  �ݾ�  ��\n");    //�ش�Ǵ� ���� ������
		fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
	}
	else if (mode==2) {
		fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
		fprintf(fp2,"�� ���� �ð�  ##�� ��ȣȸ �������ڵ妭 ����� ��  �� �� ��  ��ũ�� K�� �ݾ� ��\n");    //�ش�Ǵ� ���� ������
		fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
	}
	else if (mode==3) {
		fprintf(fp2,"��������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
		fprintf(fp2,"�� ���� �ð�  ##�� ����� ��TTY �� ��ȣȸ �� �ð� ��  �ݾ�  ��\n");    //�ش�Ǵ� ���� ������
		fprintf(fp2,"��������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
	}
        else if (mode==4) {
        fprintf(fp2,"");    //�ش�Ǵ� ���� ������
		fprintf(fp2," \n");    //�ش�Ǵ� ���� ������
	}
	while (fgets(buff,200,fp)!=NULL) {
		if (strlen(buff)<40||buff[0]==';') continue;
		for (i=0;i<30;i++) strcpy(st[i],"");
		sscanf(buff,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8],st[9],st[10],st[11],st[12],st[13],st[14]);
		strcpy(buff2,&buff[91]);
		sscanf(buff2,"%s%s%s%s%s",st[21],st[22],st[23],st[24],st[25]);
		if (strlen(st[1])!=5||strlen(st[2])!=5||strlen(st[5])<3) continue;
/*
1   2	 3	4	   5	   6  7 	  8 	   9  10	11							   12
																						   21 22	 23 		  24  25
05-26 00:31  77 p1	 xxxx	   9 xhost	  top		2 ��ȣ1 kslab						   STA 0 1
05-26 00:40  77 p1	 �û�	  19 main	  f1		8 �ٿ�1 kslab						   P �û�	 autoexec.bat 280 ddd
01-04 20:03   1 /5	 dunk1125  2 main	  hostbbs  23 ����2 203.232.36.225				   END 109
01-04 20:06   1 /4	 ���Ա��� 10 main	  my79	   23 ����2 203.234.157.76				   END 664
*/
		xstrncpy(buff1,&buff[3],2);
		i=atoi(buff1);
		if (i<day1||i>day2) continue;
		xstrncpy(buff1,&buff[0],2);
		i=atoi(buff1);
		if (i!=month) continue;
		if (mode==1) {
			if (!is_same(st[10],"����2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			sprintf(buff,"��%5.5s %5.5s%3.3s��%-8.8s��%-4.8s��%-24.24s��%6d��%8.1f��",
						st[1],st[2],st[3],st[5],st[4],st[11],
						n1,(n1/60)*pr1);
		}
		else if (mode==2) {
			if (!is_same(st[10],"�ٿ�1")||!is_same(st[21],"P")) continue;
			n1=atoi(st[24])/1024;
			if (n1<0||n1>500*1024*1024) continue;
			it3+=(n1*pr2)/10;
			count++;
			it1+=n1/1024;
			sprintf(buff,"��%5.5s %5.5s%3.3s��%-8.8s��%-8.8s��%-8.8s��%-12.12s��%6d��%6.1f��",
						st[1],st[2],st[3],
						st[7],st[8],st[5],st[23],
						n1,(n1*pr2)/10);
		}
		else if (mode==3) {
			if (!is_same(st[10],"��ȣ2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			sprintf(buff,"��%5.5s %5.5s%3.3s��%-8.8s��%-4.4s��%-8.8s��%6d��%8.1f��",
						st[1],st[2],st[3],
						st[5],st[4],st[7],n1,(n1/60)*pr1);
		}
		else if ((mode==4)&&is_same(st[11],"168.126.145.30")) {
			if (!is_same(st[10],"����2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			st[1][2]=';';
			st[2][2]=';';
			sprintf(buff,"%-5.5s;%-5.5s;%-4.4s;%-8.8s;%-15.15s;%-6d;%-8.1f ",
						st[1],st[2],st[3],
						st[5],st[11],n1,(n1/60)*pr1);
		}
		else if ((mode==4)&&(is_same(st[11],"168.126.145.50")||is_same(st[11],"168.126.145.51")||is_same(st[11],"168.126.145.52")) ) {
			if (!is_same(st[10],"����2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			st[1][2]=';';
			st[2][2]=';';
			sprintf(buff,"%-5.5s;%-5.5s;%-4.4s;%-8.8s;%-15.15s;%-6d;%-8.1f ",
						st[1],st[2],st[3],
						st[5],st[11],n1,(n1/60)*pr3);
		}
		else if ((mode==4)&&(!is_same(st[11],"168.126.145.50"))&&(!is_same(st[11],"168.126.145.51"))&&
			(!is_same(st[11],"168.126.145.52"))&&(!is_same(st[11],"168.126.145.30")) ) {
			if (!is_same(st[10],"����2")) continue;
			n1=(word)atoi(st[22]);
			if (n1<0||n1>5*60*60) continue;
			it3+=(n1/60)*pr1;
			count++;
			it1+=n1/60;
			st[1][2]=';';
			st[2][2]=';';
			sprintf(buff,"%-5.5s;%-5.5s;%-4.4s;%-8.8s;%-15.15s;%-6d;%-8.1f ",
						st[1],st[2],st[3],
						st[5],st[11],n1,0.0);
		}
		
		
		del_enter(buff);
		fprintf(fp2,"%s\n",buff);    //�ش�Ǵ� ���� ������
	}
	if (mode==1) {
		fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
		fprintf(fp2,"��%4d��  �� %d�ð� %d��   ����:%5.1f��/��  �Ѿ�:%s��\n",
					count,it1/60,it1%60,pr2,n2c(it3));	  //�ش�Ǵ� ���� ������
	}
	else if (mode==2) {
		fprintf(fp2,"������������������������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
		fprintf(fp2,"��%4d��  �� %d MB  ����:%5.1f��/K  �Ѿ�:%s��\n",
					count,it1/1024,pr2,n2c(it3));	 //�ش�Ǵ� ���� ������
	}
	else if (mode==3) {
		fprintf(fp2,"��������������������������������������������������������������\n");    //�ش�Ǵ� ���� ������
		fprintf(fp2,"��%4d��  �� %d MB  ����:%5.1f��/K  �Ѿ�:%s��\n",
					count,it1/1024,pr2,n2c(it3));	 //�ش�Ǵ� ���� ������
	}
	else if (mode==4) {
		fprintf(fp2,"");    //�ش�Ǵ� ���� ������
        fprintf(fp2,"\n");  //�ش�Ǵ� ���� ������
	}
	fclose(fp);
	fclose(fp2);
	if (count<1) mess("�ش�Ǵ� ���ӱ���� �����ϴ�.");
	else		 view_text(TMP_FILE);
	unlink(TMP_FILE);
	pf=oldpf;
}


// viewlog.c���� ȣ��Ǵ� �κ�
//rank mode1 mode2 mode3
//mode1:1 rank	2:����
//mode2:1 �Ϲ�	2:���
//mode3:1 �ݿ�	2:��ü
rank(int mode,char *cmd1,char *cmd2)
{
	char buff[512];
	int i,n,n1,n2;
	int mode2=0;


	findport(tty); //������Ʈ ã��
	read_cfg();
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
	set_nowmenu();		 //����Ÿȭ�������ϱ�
	read_pf(cmd1);
	logpf=pf;
	mode2=0;
	if (is_same(cmd2,"rk1")) mode2=1;
	if (is_same(cmd2,"rk2")) mode2=2;
	if (is_same(cmd2,"rk3")) mode2=3;
	if (is_same(cmd2,"rk4")) mode2=4;
	if (mode2>0) {
		n=mode2;
		goto _direct;
	}

	if (mode==997) {
		while (1) {
			view("frm/rank/rank.sel");
			lineinput(buff,3);check_han(buff);
			if (is_same(buff,"q")||is_same(buff,"p")||
				is_same(buff,"qq")||is_same(buff,"pp")||
				is_same(buff,"x")||is_same(buff,"xx")) return;
			n=atoi(buff);
			clear_apf();
			if (logpf.type<14&&(n==3||n==4)) n=1;
				 if (n==1) rescan5(TMP_FILE2,1,1);		//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
			else if (n==2) rescan5(TMP_FILE2,1,2);		//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
			cls();
			view("frm/rank/rank%d.scr",n);
			view_text_line(TMP_FILE2,15);
			if (mode2>0) break;
		}
	}
	if (mode==998) {
		while (1) {
			strcpy(g_buff1,"");
			strcpy(g_buff2,"");
			if (logpf.type>=14) {
				strcpy(g_buff1,"3. ��� �ݿ� ����");
				strcpy(g_buff2,"4. ��� ��ü ����");
			}
			view("frm/rank/rank.sel");
			lineinput(buff,3);check_han(buff);
			if (is_same(buff,"q")||is_same(buff,"p")||
				is_same(buff,"qq")||is_same(buff,"pp")||
				is_same(buff,"x")||is_same(buff,"xx")) return;
			n=atoi(buff);
			_direct:
			clear_apf();
			if (logpf.type<14&&(n==3||n==4)) n=1;
				 if (n==1) rescan1(TMP_FILE2,1,1);		//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
			else if (n==2) rescan1(TMP_FILE2,1,2);		//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
			else if (n==3) rescan1(TMP_FILE2,2,1);		//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
			else if (n==4) rescan1(TMP_FILE2,2,2);		//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
			else if (n==5) rescan3(TMP_FILE2,1,1);		//�Ϲ��� ���� ����
			else if (n==6) rescan3(TMP_FILE2,2,1);		//��ü	 ���� ����
			else if (n==7) rescan4(TMP_FILE2,1,1);		//�Ϲ��� ���� ����
			else if (n==8) rescan4(TMP_FILE2,2,1);		//��ü	 ���� ����
			cls();
			view("frm/rank/rank%d.scr",n);
			view_text_line(TMP_FILE2,15);
			if (mode2>0) break;
		}
	}
	else if (mode==999) {
		clear_apf();
		rescan2(TMP_FILE2);
		cls();
		view("frm/rank/ranklog.scr");
		view_text_line(TMP_FILE2,15);
	}
	unlink(TMP_FILE2);
}



//������ �ڷḦ ã�� �ڷ��� ���� �����
//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
rescan1(char *filename,int mode1,int mode2) {
	FILE *fp;
	int  i,n1,n2,n3;

	printf("\n ��� ��ٷ� �ֽʽÿ�.. ���� �ڷ����� ���Դϴ�.\n");

	fp = fopen(ID_FILE, "r");
	total=0;
	rewind(fp);
	while(fread(&pf,sizeof_pf,1,fp)) {		   //���̵� ����
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		if (mode1==1&&pf.type>=14) continue;
		total++;
		for (i=120;i>=0;i--) {
			if (pf.level<apf1[i].level) break;
			apf1[i+1]=apf1[i];
			apf1[i]=pf;
		}
		if (mode2==1) {
			for (i=120;i>=0;i--) {
				if (pf.month_log==0||pf.month_log<apf2[i].month_log) break;
				apf2[i+1]=apf2[i];
				apf2[i]=pf;
			}
			for (i=120;i>=0;i--) {
				if (pf.month_time==0||pf.month_time<apf3[i].month_time) break;
				apf3[i+1]=apf3[i];
				apf3[i]=pf;
			}
		}
		else {
			for (i=120;i>=0;i--) {
				if (pf.all_log==0||pf.all_log<apf2[i].all_log) break;
				apf2[i+1]=apf2[i];
				apf2[i]=pf;
			}
			for (i=120;i>=0;i--) {
				if (pf.all_time==0||pf.all_time<apf3[i].all_time) break;
				apf3[i+1]=apf3[i];
				apf3[i]=pf;
			}
		}
	}

	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	fprintf(fp,"[2;4H��ȸ��:%5d��",total);
	for (i=0;i<120;i++) {
		if (mode2==1) {
			n2=apf2[i].month_log;
			n3=apf3[i].month_time;
		}
		else {
			n2=apf2[i].all_log;
			n3=apf3[i].all_time;
		}
		fprintf(fp,"[%02d;5H%3d.  %-8s %6d   | %3d. %-8s %6d   | %3d. %-8s%4d:%02d\n",
		(i%15)+6,
		i+1,apf1[i].id, apf1[i].level,
		i+1,apf2[i].id, n2,
		i+1,apf3[i].id, n3/3600,(n3%3600)/60);
	}
	fprintf(fp,"[21;01H");
	fclose(fp);
}


rescan2(char *filename) {
	FILE *fp;
	int  i;
	char str[100];

	printf("\n\n\n *** ��� ��ٷ� �ּ��� ���� �ڷ� �������Դϴ�. *** \n\n\n");
	fp = fopen(ID_FILE, "r");
	total=0;				  //ȸ�� �� �� ���
	while(fread(&pf,sizeof_pf,1,fp)) {
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		total++;
		for (i=120;i>=0;i--) {
			if (pf.login>pf.logout) pf.logout=pf.login;
			if (pf.logout<apf1[i].logout) break;
			apf1[i+1]=apf1[i];
			apf1[i]=pf;
		}
	}
	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	for (i=0;i<120;i++) {
		if (strlen(apf1[i].id)<3) continue;
		fprintf(fp,"[%02d;5H%3d.  %-8s(%-8s) %2d    ",
				(i%15)+6,i+1,apf1[i].id, apf1[i].name,apf1[i].type);
		fprintf(fp,"%s �� ",datestr(2,apf1[i].login));
		fprintf(fp,"%s ",datestr(3,apf1[i].logout));
		fprintf(fp,"%4d /%4d\n",apf1[i].month_log,apf1[i].all_log);

	}
	fprintf(fp,"[21;01H");
	fclose(fp);

}

//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
rescan3(char *filename,int mode1,int mode2) {
	FILE *fp;
	int  i,n1,n2,n3;

	printf("\n ��� ��ٷ� �ֽʽÿ�.. ���� �ڷ����� ���Դϴ�.\n");

	fp = fopen(ID_FILE, "r");
	total=0;
	rewind(fp);
	while(fread(&pf,sizeof_pf,1,fp)) {		   //���̵� ����
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		if (mode1==1&&pf.type>=14) continue;
		total++;
		for (i=120;i>=0;i--) {
			if (pf.countw==0||pf.countw<apf1[i].countw) break;
			apf1[i+1]=apf1[i];
			apf1[i]=pf;
		}
		for (i=120;i>=0;i--) {
			if (pf.countu==0||pf.countu<apf2[i].countu) break;
			apf2[i+1]=apf2[i];
			apf2[i]=pf;
		}
		for (i=120;i>=0;i--) {
			if (pf.countd==0||pf.countd<apf3[i].countd) break;
			apf3[i+1]=apf3[i];
			apf3[i]=pf;
		}
	}

	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	fprintf(fp,"[2;4H��ȸ��:%5d��",total);
	for (i=0;i<120;i++) {
		fprintf(fp,"[%02d;5H%3d.  %-8s %6d   | %3d. %-8s %6d   | %3d. %-8s %6d\n",
		(i%15)+6,
		i+1,apf1[i].id, apf1[i].countw,
		i+1,apf2[i].id, apf2[i].countu,
		i+1,apf3[i].id, apf3[i].countd);
	}
	fprintf(fp,"[21;01H");
	fclose(fp);
}


//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
rescan4(char *filename,int mode1,int mode2) {
	FILE *fp;
	int  i,n1,n2,n3;

	printf("\n ��� ��ٷ� �ֽʽÿ�.. ���� �ڷ����� ���Դϴ�.\n");

	fp = fopen(ID_FILE, "r");
	total=0;
	rewind(fp);
	while(fread(&pf,sizeof_pf,1,fp)) {		   //���̵� ����
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		if (mode1==1&&pf.type>=14) continue;
		total++;
		for (i=120;i>=0;i--) {
			if (pf.warn==0||pf.warn<apf1[i].warn) break;
			apf1[i+1]=apf1[i];
			apf1[i]=pf;
		}
		for (i=120;i>=0;i--) {
			if (pf.point==0||pf.point<apf2[i].point) break;
			apf2[i+1]=apf2[i];
			apf2[i]=pf;
		}
		for (i=120;i>=0;i--) {
			if (pf.good==0||pf.good<apf3[i].good) break;
			apf3[i+1]=apf3[i];
			apf3[i]=pf;
		}
	}

	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	fprintf(fp,"[2;4H��ȸ��:%5d��",total);
	for (i=0;i<120;i++) {
		fprintf(fp,"[%02d;5H%3d.  %-8s %6d   | %3d. %-8s %6d   | %3d. %-8s %6d\n",
		(i%15)+6,
		i+1,apf1[i].id, apf1[i].warn,
		i+1,apf2[i].id, apf2[i].point,
		i+1,apf3[i].id, apf3[i].good);
	}
	fprintf(fp,"[21;01H");
	fclose(fp);
}

//������ �ڷḦ ã�� �ڷ��� ���� �����
//mode1:1:�Ϲ� 2:���   mode2:1:���� 2:��ü
rescan5(char *filename,int mode1,int mode2) {
	FILE *fp;
	int  i,n1,n2,n3;

	printf("\n ��� ��ٷ� �ֽʽÿ�.. ���� �ڷ����� ���Դϴ�.\n");

	fp = fopen(ID_FILE, "r");
	total=0;
	rewind(fp);
	while(fread(&pf,sizeof_pf,1,fp)) {		   //���̵� ����
		if (pf.type<1||strlen(pf.id)<3||pf.level<=0) continue;
		if (mode1==1&&pf.type>=14) continue;
		total++;
		if (mode2==1) {
			for (i=120;i>=0;i--) {
				if (pf.month_log==0||pf.month_log<apf2[i].month_log) break;
				apf2[i+1]=apf2[i];
				apf2[i]=pf;
			}
			for (i=120;i>=0;i--) {
				if (pf.month_time==0||pf.month_time<apf3[i].month_time) break;
				apf3[i+1]=apf3[i];
				apf3[i]=pf;
			}
		}
		else {
			for (i=120;i>=0;i--) {
				if (pf.all_log==0||pf.all_log<apf2[i].all_log) break;
				apf2[i+1]=apf2[i];
				apf2[i]=pf;
			}
			for (i=120;i>=0;i--) {
				if (pf.all_time==0||pf.all_time<apf3[i].all_time) break;
				apf3[i+1]=apf3[i];
				apf3[i]=pf;
			}
		}
	}

	fclose(fp);

	if ((fp=fopen(filename, "w"))==NULL) return;
	fprintf(fp,"[2;4H��ȸ��:%5d��",total);
	for (i=0;i<120;i++) {
		if (mode2==1) {
			n2=apf2[i].month_log;
			n3=apf3[i].month_time;
		}
		else {
			n2=apf2[i].all_log;
			n3=apf3[i].all_time;
		}
		fprintf(fp,"[%02d;5H    %3d. %-8s %6d        |       %3d. %-8s%4d:%02d\n",
		(i%15)+6,
		i+1,apf2[i].id, n2,
		i+1,apf3[i].id, n3/3600,(n3%3600)/60);
	}
	fprintf(fp,"[21;01H");
	fclose(fp);
}




clear_apf()
{
	int i;
	PF apf;
	strcpy(apf.id,"");
	apf.level=0;
	apf.month_log=0;
	apf.all_log=0;
	apf.month_time=0;
	apf.all_time=0;
	apf.type=0;
	apf.login=0;
	apf.logout=0;
	apf.countu=0;
	apf.countd=0;
	apf.countw=0;
	apf.good=0;
	apf.warn=0;
	apf.point=0;
	for (i=0;i<122;i++) {
		apf1[i]=apf;
		apf2[i]=apf;
		apf3[i]=apf;
	}
}


