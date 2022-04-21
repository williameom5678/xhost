////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: �������� BBS HOST							  //
//			  //			  ������ : �輺��  xhost@xhost.co.kr (016-320-7882)		  //	  //
//			  �� �ҽ��� �����Ͽ� ������� ����ϽǶ�����			  //
//			  �������� ������ �޾ƾ� �մϴ�.						  //
////////////////////////////////////////////////////////////////////////
//��� ���� ������ �ִ����� �ľ��Ѵ�.
#include "bbs.h"
check_smemo()
{
	FILE *fp1,*fp2;
	char filename[100];
    char buff[200],buff2[200],abuff[10][200],tid[100];
    time_t t;
    int i,count,delete_ok,save_count,stop;
	struct stat stbuf;
    char TMP_FILE3[100];
    sprintf(TMP_FILE3,"tmp/tmp3%d%s",cfg.hostno,no_spc(tty));   //ȣ��Ʈ��ȣ���ӽ�ȭ��

    if (pf.type<1) return No;
	sprintf(filename,"userdata/%s/%s/smemo.dat",c_han(pf.id),pf.id);
	if (!is_file(filename)) return No;
	t=file_time(filename);		 //file�� ���������ð�
	printf("\n");
    printf(" ## %s �߼� ������ �ֽ��ϴ�.\n",datestr(39,t));
    printf(" ## ���� �б�(Y) ����(D) ���(P) >>");
	lineinput(buff,2);check_han(buff);printf("\n");
	if (is_same(buff,"d")) {unlink(filename);return No;}
    if (is_quit(buff)||is_same(buff,"n")) return Yes;
    if ((fp2=fopen(TMP_FILE3,"w+"))==NULL) {mess("%s ERR",TMP_FILE3);return No;}
    if ((fp1=fopen(filename,"r"))==NULL) return No;
	count=0;

    delete_ok=No;
    save_count=0;
    stop=Yes;
    while(fgets(buff2,200,fp1)!=NULL) {
        if (count%8==0) for(i=0;i<8;i++) strcpy(abuff[i],"\n");
        strcpy(abuff[count%8],buff2);
        if (count%8==7) {
            for (i=0;i<3;i++) printf("   %s",abuff[i]); //��������
            for (i=3;i<8;i++) printf("%d. %s",i-2,abuff[i]); //��������
            sscanf(abuff[1],"%s %s",tid,buff);
            strcpy(buff,"");
            if (stop) {
                printf(" ## ����(Y) ����(D) ��ü����(DD) ����(P) ����(R) ����(S) >>");
                lineinput(buff,2);check_han(buff);printf("\n");
                if (is_same(buff,"s")) stop=No;
                else if (strlen(tid)>2&&is_same(buff,"r")) write_smemo(tid);
                else if (is_same(buff,"dd")) {
                    fclose(fp1);
                    fclose(fp2);
                    unlink(filename);unlink(TMP_FILE3);
                    printf(" ## ��ü�� �����Ǿ����ϴ�.\n");
                    return Yes;
                }
                else if (is_same(buff,"d")) {
                    printf(" ## ������ �����Ǿ����ϴ�.\n");
                    delete_ok=Yes;
                    goto _cont1;
                }
            }
            save_count++;
            for (i=0;i<8;i++) fprintf(fp2,"%s",abuff[i]); //��������
            if (is_quit(buff)||is_same(buff,"n")) break;
		}
		_cont1:
		count++;
	}
    fclose(fp1);
    fclose(fp2);
    if (!stop) mess("�޼��� ��");
    if (save_count<1) unlink(filename);
    else if (delete_ok) file_copy(TMP_FILE3,filename);
    unlink(TMP_FILE3);
	printf("\n");
	return Yes;
}



//ȸ�� ���� ������ ����.
write_smemo(char *id)
{
    FILE *fp,*fp2;
	char filename[100];
    char buff[200],buff2[200],abuff[10][200];
    int i,num,n,nn;
	if (strlen(id)>2) strcpy(buff,id);
	else {
        printf("\n ## ������ ���� ID >>");
		lineinput(buff,8);check_han(buff);printf("\n");
	}
	if (is_quit(buff)) return;
	if (!read_pf4(buff)) {mess("%s ID�� �����ϴ�",buff);return;}
    if (strlen(pf2.tmail)>0) {
        if (pf2.tmail[0]=='*') {
			mess("%s���� ���ϰźλ����Դϴ�.",pf2.id);
			return;
		}
	}
    check_dir("userdata/%s/%s",c_han(pf2.id),pf2.id);
	sprintf(filename,"userdata/%s/%s/smemo.dat",c_han(pf2.id),pf2.id);
    printf("\n ## %s(%s) �Կ��� ������ �����ϴ�.\n",pf2.id,pf2.name);
    printf(" ## ������ 5�ٱ��� �Է��� �����մϴ�.\n");
    for(i=0;i<10;i++) strcpy(abuff[i],"\n");
    sprintf(abuff[0],"��������������������������������������������������������������\n");
    sprintf(abuff[1],"%s (%s)���� �����Դϴ�. �����ð�:%s\n",pf.id,pf.nick,now_str(33));
    sprintf(abuff[2],"��������������������������������������������������������������\n");
    for(i=0;i<3;i++) printf("   %s",abuff[i]);
    strcpy(buff,"");
    for(i=0;i<5;i++) {
        printf("%d. ",i+1);
        lineinput(buff,76);printf("\n");
        if (is_same(buff,".")||is_quit(buff)) break;
        sprintf(abuff[3+i],"%s\n",buff);
    }
    if (is_quit(buff)) {mess("������ ��ҵǾ����ϴ�");return;}
    if (!yesno2("\n ## ������ �߼��ұ��? (Y/n)>>",Yes)) return;

    if ((fp=fopen(filename,"a+"))==NULL) {
        if ((fp=fopen(filename,"w+"))==NULL) return;
    }
    for (i=0;i<8;i++) {
        del_enter(abuff[i]);
        fprintf(fp,"%s\n",abuff[i]); //��������
    }
    fclose(fp);
    sprintf(buff,"\n ## %s(%s)���� ��������(�����б�:RMEMO)\n",pf.id,pf.name);
	to_mess(pf2.id,buff);
    printf("== %s(%)�Կ��� ������ �߼۵Ǿ����ϴ�. ==\n",pf2.id,pf2.name);
}
//��ȣȸ ���� ������ ����.
write_cmemo()
{
    FILE *fp,*fp2;
	char filename[100];
    char buff[200],buff2[200],abuff[10][200],id_file[200];
    CPF cpf2;
    int i,num,n,nn,count,type1,type2;
    type1=0;type2=19;
    if (!club_mode) {mess("��ȣȸ������");return;}
    while (1) {
        printf("\n %s ��ȣȸ ���� ���� �߼�\n",club_go.code);
        printf("  1. �߼� ��� ���� : %d \n",type1);
        printf("  2. �߼� ��� ��   : %d \n",type2);
        printf(" 99. ���� �߼� ����\n");
        n=numselect();
        if (n<1) return;
        if (n==1) {printf(" ���۵�� >>");type1=get_num(type1,2);}
        if (n==2) {printf(" ��  ��� >>");type2=get_num(type2,2);}
        if (n==99) break;
	}

    printf("\n ## ��ȣȸ ȸ������ ������ �����ϴ�.\n");
    printf(" ## ������ 5�ٱ��� �Է��� �����մϴ�.\n");
    for(i=0;i<10;i++) strcpy(abuff[i],"\n");
    sprintf(abuff[0],"��������������������������������������������������������������\n");
    sprintf(abuff[1],"%s (%s)���� �����Դϴ�. �����ð�:%s\n",pf.id,pf.nick,now_str(33));
    sprintf(abuff[2],"��������������������������������������������������������������\n");
    for(i=0;i<3;i++) printf("   %s",abuff[i]);
    strcpy(buff,"");
    for(i=0;i<5;i++) {
        printf("%d. ",i+1);
        lineinput(buff,76);printf("\n");
        if (is_same(buff,".")||is_quit(buff)) break;
        sprintf(abuff[3+i],"%s\n",buff);
    }
    if (is_quit(buff)) {mess("������ ��ҵǾ����ϴ�");return;}
    printf("\n");
    for(i=0;i<3;i++) printf("   %s",abuff[i]);
    for(i=3;i<8;i++) printf("%d. %s",i-2,abuff[i]);
    if (!yesno2(" ## ������ �߼��ұ��? (Y/n)>>",Yes)) return;
    for (i=0;i<8;i++) del_enter(abuff[i]);

    sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
    if ((fp=fopen(id_file,"r"))==NULL) {mess("��ȣȸ ȸ�����Ͼ���");return;}
    count=0;
    while (fread(&cpf2,sizeof_cpf,1,fp)>0) {
        if (strlen(cpf.id)<3||cpf2.type<type1||cpf2.type>type2) continue;
        if (!read_pf4(cpf.id)) continue;
        if (pf2.tmail[0]=='*') continue;
        sprintf(filename,"userdata/%s/%s/smemo.dat",c_han(pf2.id),pf2.id);
        if ((fp=fopen(filename,"a+"))==NULL) {
            if ((fp=fopen(filename,"w+"))==NULL) continue;
        }
        for (i=0;i<8;i++) fprintf(fp,"%s\n",abuff[i]); //��������
        fclose(fp);
        sprintf(buff,"\n ## %s ���� ��������(�����б�:RMEMO)\n",pf.id);
        to_mess(pf2.id,buff);
        printf(" %6d.  %-8s %-8s ���� �߼�\n",count+1,pf2.id,pf2.name);
        count++;
	}
}
