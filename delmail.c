////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���� : ���� ���� ���� ����
// delmail <���̵�> <�����ҹ�ȣ>
////////////////////////////////////////////////////////////////////////		who=0;
#include "bbs.h"

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	int i,n=0;
	int found;
	int id_num1;
	int id_num2;
	int b_num;
	int read_ok=No;
	int old_posi,new_posi;
	char buff[140];
	char filename[140];
	FILE *fp1,*fp2,*fp3,*fp4;
	BBS bbs2,bbs3;
	findport(tty); //������Ʈ ã��
    read_cfg();
    set_tmpfile();
    if (is_same(argv[1],"_all_maildel")) {
        all_maildel(atoi(argv[2]),atoi(argv[3]));
        return;
    }
    if (!read_pf(argv[1])) {
        mess("%s ID �˻�����",argv[1]);
        return;        //���̵� �˻� ����
    }
    b_num=atoi(argv[2]);

    sprintf(NOW_FILE ,"userdata/%s/%s/send.tit",c_han(pf.id),pf.id);
    sprintf(NOW_FILED,"userdata/%s/%s/send.txt",c_han(pf.id),pf.id);

	if (!is_file(NOW_FILE)) return;
    if (!bbs_num(b_num)) {
        mess("���� %s�� �ڷ� �˻�����",argv[2]);
        return;       //�ش� ��ȣ ã�Ƽ� BBS�� ����
    }
    if (bbs.del) {
        mess("���� %d���� �̹� ������ �ڷ��Դϴ�.",bbs.num);
        return;       //�ش� ��ȣ ã�Ƽ� BBS�� ����
    }
	bbs3=bbs;

    printf("\n");
    printf("��ȣ:%4d  ����:%-8s �߽�:%-8s  ����:%s\n",bbs3.num,bbs3.id,datestr(2,bbs3.date),bbs3.title);
    if (bbs3.date2>1) {
        printf("��ȣ:%4d.���� �̹� %s �� %s ���� ���� �����Դϴ�. (���� �Ұ�) \n",bbs3.num,datestr(2,bbs3.date2),bbs3.id);
        if (!yesno2(" ### ���� ������ �״�� �ΰ� ������ ������ �����ұ��? (Y/n) >>",Yes)) {
            mess("������ ��� �Ǿ����ϴ�.");
			return;
		}
		read_ok=Yes;
	}

    fp1=fopen(NOW_FILE ,"r+");              //�ڽ��� ���� ������
    fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
    if (fread(&bbs,sizeof_bbs,1,fp1)&&bbs3.num==bbs.num) {
        bbs.del=Yes;
        sprintf(buff,"[������������]%s",bbs.title);
        sprintf(bbs.title,"%-60.60s",buff);
        fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
        fwrite(&bbs,sizeof_bbs,1,fp1);
        fclose(fp1);
    }
    sprintf(filename,"roombin/%s/%s/%s",c_han(pf.id),pf.id,bbs3.filename);
	unlink(filename);
	if (read_ok) return;				  //�̹� ����
	if (!read_pf2(bbs3.id)) return; 	   //���� ���̵� �˻� ����
    sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf2.id),pf2.id);
    sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf2.id),pf2.id);

	found=No;
    if ((fp1=fopen(NOW_FILE ,"r+"))==NULL) return;
    NOW_REC=0;
	while (fread(&bbs2,sizeof_bbs,1,fp1)) {
//		  printf("%s %d %d %d",bbs2.id,bbs2.num,bbs2.date,bbs2.date2);getchar();
		if (is_same(bbs2.id,pf.id)&&bbs2.date==bbs3.date) {
			found=Yes;
			break;
		}
        NOW_REC++;
	}
    fclose(fp1);

	if (!found) {
        mess(" ������ ���Ͽ��� %d.�� ������ ã���� �����ϴ�.",bbs3.num,bbs3.id);
		return;
	}
    else if (bbs2.read>0) {
        mess("%d.���� �̹� %s �� %s ���� ���� �����Դϴ�.(���� �Ұ�)",bbs3.num,datestr(2,bbs3.date2),bbs3.id);
		return;
	}
    else if (bbs2.del==Yes) {
        mess("%d.���� �̹� %s ���� ������ �����Դϴ�. (���� �Ұ�)",bbs3.num,bbs3.id);
		return;
	}
    else if (found) {
        if ((fp1=fopen(NOW_FILE ,"r+"))==NULL) return;
        fseek(fp1,NOW_REC*sizeof_bbs,SEEK_SET);
        if (!feof(fp1)) {
            bbs2.del=Yes;
            strcpy(bbs2.id,"");
            strcpy(bbs2.name,"");
            strcpy(bbs2.title,"");
            bbs2.size=0;
            fwrite(&bbs2,sizeof_bbs,1,fp1);
        }
        fclose(fp1);
        mess("%s�Կ��� ���� %d.�� ������ �����Ǿ����ϴ�.",bbs3.id,bbs3.num);
    }
}



//�ش� �Ǵ� ��ȣ�� ã�� bbs�� ����
int bbs_num(int num)
{
	BBS bbs1;
	int i,n,found=No;
	FILE *fp;
	int last,first,now;

	fp=fopen(NOW_FILE,"r+");
	if (fp==NULL) return No;

	//���� �ε��� �˻�
	first=1;
	fseekend(fp);
	last=ftell(fp)/sizeof_bbs;
	now=last/2+1;
	while(1) {
		fseek(fp,(now-1)*sizeof_bbs,SEEK_SET);
		if (fread(&bbs1,sizeof_bbs,1,fp)) {
			if (bbs1.num==num) {
				found=Yes;
				NOW_REC=now-1;
				break;
			}
			else if ((last-first<5)||now<0) break;
			else if (bbs1.num>num) last=now;
			else				   first=now;
			now=(last-first)/2+first+1;
		}
		else break;
	}
	if (!found) {
		i=now-15;if (i<0) i=0;
		for (n=i;n<now+30;n++) {
			fseek(fp,n*sizeof_bbs,SEEK_SET);
			if (fread(&bbs1,sizeof_bbs,1,fp)) {
				if (bbs1.num==num) {
					NOW_REC=n;
					found=Yes;
					break;
				}
			}
		}
	}


	if (!found) {	  //�˻� ���н� ó������ �˻�
		fseekend(fp);
		last=ftell(fp)/sizeof_bbs;
		rewind(fp);
		for (i=0;i<last;i++) {
			fread(&bbs1,sizeof_bbs,1,fp);
			if (bbs1.num==num) {
				found=Yes;
				NOW_REC=i;
				break;
			}
		}
	}

    if (fp!=NULL) fclose(fp);

	if (found) {
		del_esc(bbs1.id);
		del_esc(bbs1.name);
		bbs=bbs1;
		NOW_NO=bbs1.num;
	}
	return(found);
}


all_maildel(int day1,int day2)
{
	FILE *fp;
    if ((fp=fopen(ID_FILE,"r+"))==NULL) return;
    while (fread(&pf,sizeof_pf,1,fp)) {
        if (pf.type<1||pf.type>17||strlen(pf.id)<3) continue;
        maildel(1,day1);  //��������
        maildel(2,day1);  //��������
        maildel(3,day2);  //����������
    }
    fclose(fp);
}


maildel(int mode,int day)
{
    FILE *fp1,*fp2,*fp3,*fp4;
    int found=No,now_pos,i,n;
    int now;

    if (day<1) return;
    if (mode==1) {
        sprintf(NOW_FILE ,"userdata/%s/%s/letter.tit",c_han(pf.id),pf.id);
        sprintf(NOW_FILED,"userdata/%s/%s/letter.txt",c_han(pf.id),pf.id);
    }
    else if (mode==2) {
        sprintf(NOW_FILE ,"userdata/%s/%s/send.tit",c_han(pf.id),pf.id);
        sprintf(NOW_FILED,"userdata/%s/%s/send.txt",c_han(pf.id),pf.id);
    }
    else if (mode==3) {
        sprintf(NOW_FILE ,"userdata/%s/%s/save.tit",c_han(pf.id),pf.id);
        sprintf(NOW_FILED,"userdata/%s/%s/save.txt",c_han(pf.id),pf.id);
    }

    found=No;
    if ((fp1=fopen(NOW_FILE ,"r"))==NULL) return;
    if ((fp2=fopen(NOW_FILED,"r"))==NULL) return;
    if ((fp3=fopen(TMP_FILE1,"w"))==NULL) return;
    if ((fp4=fopen(TMP_FILE2,"w"))==NULL) return;

    now_pos=0;
    while (fread(&bbs,sizeof_bbs,1,fp1)) {
    	now=how_old(bbs.date);
        if (now>0&&now>day*24*60*60) {
            printf("���� %d. %s %s %s\n",bbs.num,bbs.id,datestr(0,bbs.date),bbs.title);
            found=Yes;
            if (strlen(bbs.filename)>0) {
                unlink_file("userroom/%s/%s/%s",c_han(pf.id),pf.id,bbs.filename);
            }
            continue;
        }
    	fseek(fp2,bbs.posi,SEEK_SET);
    	fseek(fp4,now_pos,SEEK_SET);
        for (i=0;i<bbs.size;i++)
            fputc(fgetc(fp2),fp4);
        bbs.posi=now_pos;
        fwrite(&bbs,sizeof_bbs,1,fp3);
        now_pos=bbs.posi+bbs.size;
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    if (!found) {
        unlink(TMP_FILE1);
        unlink(TMP_FILE2);
    }
    else {
        file_move(TMP_FILE1,NOW_FILE );
        file_move(TMP_FILE2,NOW_FILED);
    }
}
