////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
//	  �Խ��� ���� ��ƾ
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//�Խ��� ����
//�Է� : ������ �Խù� ��ȣ
//��� : NOW_FILE�� �����Ѵ�.

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
	char tcode[59];
	char ccode[59];
	char id[9];
	char tclub[100];
    BBS bbs0,bbs1,bbs2,bbs3,bbs4;
    int i,ii,i2,found,now_rec,change;
    int count;
    for (ii=1;ii<argc;ii++) {
        strcpy(NOW_FILE,argv[ii]);              //�����ڵ�
        if (!stristr(NOW_FILE,".tit")) {
            printf("Ȯ���ڰ� *.tit �� �����մϴ�.\n");
            continue;
        }
        if ((fp=fopen(NOW_FILE,"r+"))==NULL) {
            printf("%s ������ ã���� �����ϴ�.\n",NOW_FILE);
            continue;
        }
        now_rec=0;
        bbs2.date=0;
        count=0;
        fseek(fp,now_rec*sizeof_bbs,SEEK_SET);

        bbs0.date=str2date2("1999/11/29 00:01:00");
        bbs1.date=str2date2("1999/12/01 11:01:00");
        bbs2.date=str2date2("2000/07/01 09:53:00");
        bbs3.date=str2date2("2000/07/01 09:00:00");
        bbs4.date=str2date2("1999/12/01 04:01:00");
        change=bbs2.date-bbs1.date;
    //  printf("%d %d %d %d\n",bbs1.date,bbs2.date,bbs3.date,change);
    //  bbs1.date=bbs1.date+change;
    //  printf("%s ",datestr(8,bbs1.date));
    //  return;

        found=No;
        count=0;
        while(fread(&bbs,sizeof_bbs,1,fp)) {
            if (bbs.date>bbs4.date) found=Yes;
    //        if (found&&bbs.date>bbs3.date) break;
    //      printf("%d %d %d %d\n",found,bbs.date,bbs0.date,bbs1.date);
            if (!found||bbs.date<bbs0.date||bbs.date>bbs4.date) {
                if (now_rec%100==0)
                printf("%s %d %s -> not_chage\n",NOW_FILE,bbs.num,datestr(8,bbs.date));
                now_rec++;
                continue;  //������ ���
            }
            while(1) {
                fseek(fp,now_rec*sizeof_bbs,SEEK_SET);
                if (!fread(&bbs,sizeof_bbs,1,fp)) break;
            	if (bbs.date<bbs0.date||bbs.date>bbs4.date) {
                    now_rec++;
		    continue;
		}
                printf("%s %d %s -> ",NOW_FILE,bbs.num,datestr(8,bbs.date));
                bbs.date=bbs.date+change;
                printf("%s\n",datestr(8,bbs.date));
                fseek(fp,now_rec*sizeof_bbs,SEEK_SET);
                fwrite(&bbs,sizeof_bbs,1,fp);
                usleep(100000);
                count++;
                now_rec++;
            }
            break;
        }
        fclose(fp);
        printf("               %s ����  %d �� �ڷ� ����\n",NOW_FILE,count);
    }
}
