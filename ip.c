////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// IP���� ���
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

//�����ڵ�,�ڷ��ȣ�� ���Ͽ� ������ return Yes
is_read_numn(int n)
{
	int i;
	char buff[100];
	sprintf(buff,"%s%s%5d",go.code,club_go.code,n);
	//�̹� ���� �ڷ������� Ȯ���Ѵ�.
	for (i=0;i<50;i++) {
		if(strcmp(aread_nums[i],buff)==0) return Yes;
	}
	return No;
}

//�߰�
add_read_numn(int n)
{
	int i;
	char buff[200];
	sprintf(buff,"%s%s%5d",go.code,club_go.code,n);
	//�̹� ���� �ڷ������� Ȯ���Ѵ�.
	for (i=0;i<50;i++) {
		if(strcmp(aread_nums[i],buff)==0) return;
	}
	//���� ó���� �Ѵ�.
	for (i=0;i<49;i++) strcpy(aread_nums[i],aread_nums[i+1]);
	strcpy(aread_nums[49],buff);
}

check_another_rec()        //��ȣȸ��ġ����� ���
{
    if (!is_same(last_club,club_go.code)) {
        if (!club_mode) set_rec3(2,2,last_club,last_go.code);    //��ȣȸ���ӳ�
        strcpy(last_club,club_go.code);
        last_go=go;
    }
}


//mode=1:����  2:��
set_rec(int mode,int job){set_rec_(mode,job,0,"");}

//������ GO�ڵ�� ����
set_rec3(int mode,int job,char *lastclub,char *lastgo)
{
    char oldclub[10],oldgo[10];
    strcpy(oldclub,club_go.code);
    strcpy(oldgo,go.code);
    strncpy(club_go.code,lastclub,8);
    strncpy(go.code,lastgo,8);
    set_rec_(mode,job,0,"");
    strcpy(club_go.code,oldclub);
    strcpy(go.code,oldgo);
}

//mode=1:����  2:��
set_rec_(int mode,int job,int size,char *title)
{
    char filename[200];
    char buff[500],buff2[500];

    //�ش�ȣ��Ʈ,ID�� ��Ͼ���
    if (bit(allmode,9)||bit(allmode,8)) return;
	strcpy(buff,"");strcpy(buff2,"");

	//stotal ���
		 if (pf.type==19&&bit(cfg.mrecmode,1)) return;
	else if (pf.type>=16&&bit(cfg.mrecmode,2)) return;
	else if (pf.type==0 &&bit(cfg.mrecmode,3)) return;

         if (job==23&&mode==1) sprintf(buff,"STA 0");
    else if (job==23&&mode==2) sprintf(buff,"END %d",how_old(START_TIME));
    else if (job== 2&&mode==1) sprintf(buff,"STA 0 %d" ,cpf.type);
    else if (job== 2&&mode==2) sprintf(buff,"END %d %d",how_old(CLUB_START_TIME),cpf.type);
    else if (job== 3&&mode==1) sprintf(buff,"STA 0");
    else if (job== 3&&mode==2) sprintf(buff,"END %d",how_old(CHECK_TIME));
    else if (job== 5) sprintf(buff,"B %-8s %s"        ,bbs.id,bbs.title);
    else if (job== 6) sprintf(buff,"P %-8s %s %8d %s" ,bbs.id,bbs.filename,bbs.filesize,bbs.title);
    else if (job== 7) sprintf(buff,"B %-8s %s"        ,bbs.id,bbs.title);
    else if (job== 8) sprintf(buff,"P %-8s %s %8d %s" ,bbs.id,bbs.filename,bbs.filesize,bbs.title);
    else if (job== 9) sprintf(buff,"B %-8s %s"        ,bbs.id,bbs.title);
    else if (job==10) sprintf(buff,"B %-8s %s"        ,bbs.id,bbs.title);
    else if (job==11) sprintf(buff,"B %-8s %s"        ,bbs.id,bbs.title);
    else if (job==12) sprintf(buff,"B %-8s %s"        ,bbs.id,bbs.title);
    else if (job==14) sprintf(buff,"P %-8s %-12.12s %8d %s" ,bbs.id,bbs.filename,bbs.filesize,bbs.title);
    else if (job==25) sprintf(buff,"B %-8s %s"        ,bbs.id,bbs.title); //����
    else if (job==26) sprintf(buff,"P %-8s %-12.12s %8d %s" ,bbs.id,bbs.filename,bbs.filesize,bbs.title);
    del_enter(buff);del_esc(buff);
    buff[71]=0;
    save_rec(mode,job,buff);
}


//�Խ��Ǳ�ϰ���
check_bbsrec(int mode)
{
	char filename[200],tailname[20];
	char buff[500],buff2[500],nowbuff[20];
	char oldtype;
	int jobmode;

	//�ش�ȣ��Ʈ,ID�� ��Ͼ���
	if (bit(allmode,18)) return;
	if (bit(allmode,9)||bit(allmode,8)) return;

	if (mode==0) {	   //���� �ٸ��� ����+���α��
		if (!is_same(bbsrec_club,club_go.code)||!is_same(bbsrec_code,go.code)) {
			check_bbsrec(2);
		}
	}
	else if (mode==1) { 	 //������ ����
		if (bbsrec_time>0&&strlen(bbsrec_code)>0) {
			oldtype=go.type;
			go.type=bbsrec_gotype;
			if ( (is_bbspds &&!bit(cfg.mrecmode,6))||
				 (is_typeexe&&!bit(cfg.mrecmode,8)) ) {
				sprintf(buff2,"END %d",how_old(bbsrec_time));
				save_rec(1,bbsrec_jobmode,buff2);
			}
			go.type=oldtype;
			bbsrec_time=0;
			bbsrec_jobmode=0;
			bbsrec_gotype=' ';
			strcpy(bbsrec_code,"");
			strcpy(bbsrec_club,"");
		}
	}

	else if (mode==2) { 	//������ ���� + ���� ���
		jobmode=is_bbspds?21:20;
		check_bbsrec(1);
		if ( (is_bbspds||is_typeexe)&&
			!bit(bcfg.recmode,2)&&
			!is_same(go.code,"notice")&&
			!stristr(go.code,"mail")&&
			!is_char("ok",go.type)) {
			if (!is_same(bbsrec_code,go.code)&&
				!is_same(bbsrec_club,club_go.code)) {
				time(&bbsrec_time);
				strcpy(bbsrec_code,go.code);
				strcpy(bbsrec_club,club_go.code);
				bbsrec_gotype=go.type;
				bbsrec_jobmode=jobmode;
				if ( (is_bbspds &&!bit(cfg.mrecmode,5))||
					 (is_typeexe&&!bit(cfg.mrecmode,7)) ) {
					save_rec(1,bbsrec_jobmode,"STA 0");
				}
			}
		}
		else {		//�Խ����� �ƴҶ�
			bbsrec_time=0;
			bbsrec_jobmode=0;
			bbsrec_gotype=' ';
			strcpy(bbsrec_code,"");
			strcpy(bbsrec_club,"");
		}
	}
}



