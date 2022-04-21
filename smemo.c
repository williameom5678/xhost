////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: 리눅스용 BBS HOST							  //
//			  //			  개발자 : 김성대  xhost@xhost.co.kr (016-320-7882)		  //	  //
//			  이 소스를 변경하여 상용으로 사용하실때에는			  //
//			  개발자의 승인을 받아야 합니다.						  //
////////////////////////////////////////////////////////////////////////
//운영진 수신 쪽지가 있는지를 파악한다.
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
    sprintf(TMP_FILE3,"tmp/tmp3%d%s",cfg.hostno,no_spc(tty));   //호스트번호별임시화일

    if (pf.type<1) return No;
	sprintf(filename,"userdata/%s/%s/smemo.dat",c_han(pf.id),pf.id);
	if (!is_file(filename)) return No;
	t=file_time(filename);		 //file의 최종수정시간
	printf("\n");
    printf(" ## %s 발송 쪽지가 있습니다.\n",datestr(39,t));
    printf(" ## 쪽지 읽기(Y) 삭제(D) 취소(P) >>");
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
            for (i=0;i<3;i++) printf("   %s",abuff[i]); //쪽지저장
            for (i=3;i<8;i++) printf("%d. %s",i-2,abuff[i]); //쪽지저장
            sscanf(abuff[1],"%s %s",tid,buff);
            strcpy(buff,"");
            if (stop) {
                printf(" ## 다음(Y) 삭제(D) 전체삭제(DD) 중지(P) 답장(R) 연속(S) >>");
                lineinput(buff,2);check_han(buff);printf("\n");
                if (is_same(buff,"s")) stop=No;
                else if (strlen(tid)>2&&is_same(buff,"r")) write_smemo(tid);
                else if (is_same(buff,"dd")) {
                    fclose(fp1);
                    fclose(fp2);
                    unlink(filename);unlink(TMP_FILE3);
                    printf(" ## 전체가 삭제되었습니다.\n");
                    return Yes;
                }
                else if (is_same(buff,"d")) {
                    printf(" ## 쪽지가 삭제되었습니다.\n");
                    delete_ok=Yes;
                    goto _cont1;
                }
            }
            save_count++;
            for (i=0;i<8;i++) fprintf(fp2,"%s",abuff[i]); //쪽지저장
            if (is_quit(buff)||is_same(buff,"n")) break;
		}
		_cont1:
		count++;
	}
    fclose(fp1);
    fclose(fp2);
    if (!stop) mess("메세지 끝");
    if (save_count<1) unlink(filename);
    else if (delete_ok) file_copy(TMP_FILE3,filename);
    unlink(TMP_FILE3);
	printf("\n");
	return Yes;
}



//회원 수신 쪽지를 쓴다.
write_smemo(char *id)
{
    FILE *fp,*fp2;
	char filename[100];
    char buff[200],buff2[200],abuff[10][200];
    int i,num,n,nn;
	if (strlen(id)>2) strcpy(buff,id);
	else {
        printf("\n ## 쪽지를 보낼 ID >>");
		lineinput(buff,8);check_han(buff);printf("\n");
	}
	if (is_quit(buff)) return;
	if (!read_pf4(buff)) {mess("%s ID가 없습니다",buff);return;}
    if (strlen(pf2.tmail)>0) {
        if (pf2.tmail[0]=='*') {
			mess("%s님은 메일거부상태입니다.",pf2.id);
			return;
		}
	}
    check_dir("userdata/%s/%s",c_han(pf2.id),pf2.id);
	sprintf(filename,"userdata/%s/%s/smemo.dat",c_han(pf2.id),pf2.id);
    printf("\n ## %s(%s) 님에게 쪽지를 적습니다.\n",pf2.id,pf2.name);
    printf(" ## 쪽지는 5줄까지 입력이 가능합니다.\n");
    for(i=0;i<10;i++) strcpy(abuff[i],"\n");
    sprintf(abuff[0],"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    sprintf(abuff[1],"%s (%s)님의 쪽지입니다. 보낸시간:%s\n",pf.id,pf.nick,now_str(33));
    sprintf(abuff[2],"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    for(i=0;i<3;i++) printf("   %s",abuff[i]);
    strcpy(buff,"");
    for(i=0;i<5;i++) {
        printf("%d. ",i+1);
        lineinput(buff,76);printf("\n");
        if (is_same(buff,".")||is_quit(buff)) break;
        sprintf(abuff[3+i],"%s\n",buff);
    }
    if (is_quit(buff)) {mess("쪽지가 취소되었습니다");return;}
    if (!yesno2("\n ## 쪽지를 발송할까요? (Y/n)>>",Yes)) return;

    if ((fp=fopen(filename,"a+"))==NULL) {
        if ((fp=fopen(filename,"w+"))==NULL) return;
    }
    for (i=0;i<8;i++) {
        del_enter(abuff[i]);
        fprintf(fp,"%s\n",abuff[i]); //쪽지저장
    }
    fclose(fp);
    sprintf(buff,"\n ## %s(%s)님의 쪽지도착(쪽지읽기:RMEMO)\n",pf.id,pf.name);
	to_mess(pf2.id,buff);
    printf("== %s(%)님에게 쪽지가 발송되었습니다. ==\n",pf2.id,pf2.name);
}
//동호회 수신 쪽지를 쓴다.
write_cmemo()
{
    FILE *fp,*fp2;
	char filename[100];
    char buff[200],buff2[200],abuff[10][200],id_file[200];
    CPF cpf2;
    int i,num,n,nn,count,type1,type2;
    type1=0;type2=19;
    if (!club_mode) {mess("동호회전용명령");return;}
    while (1) {
        printf("\n %s 동호회 동보 쪽지 발송\n",club_go.code);
        printf("  1. 발송 등급 시작 : %d \n",type1);
        printf("  2. 발송 등급 끝   : %d \n",type2);
        printf(" 99. 쪽지 발송 시작\n");
        n=numselect();
        if (n<1) return;
        if (n==1) {printf(" 시작등급 >>");type1=get_num(type1,2);}
        if (n==2) {printf(" 끝  등급 >>");type2=get_num(type2,2);}
        if (n==99) break;
	}

    printf("\n ## 동호회 회원에게 쪽지를 적습니다.\n");
    printf(" ## 쪽지는 5줄까지 입력이 가능합니다.\n");
    for(i=0;i<10;i++) strcpy(abuff[i],"\n");
    sprintf(abuff[0],"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    sprintf(abuff[1],"%s (%s)님의 쪽지입니다. 보낸시간:%s\n",pf.id,pf.nick,now_str(33));
    sprintf(abuff[2],"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    for(i=0;i<3;i++) printf("   %s",abuff[i]);
    strcpy(buff,"");
    for(i=0;i<5;i++) {
        printf("%d. ",i+1);
        lineinput(buff,76);printf("\n");
        if (is_same(buff,".")||is_quit(buff)) break;
        sprintf(abuff[3+i],"%s\n",buff);
    }
    if (is_quit(buff)) {mess("쪽지가 취소되었습니다");return;}
    printf("\n");
    for(i=0;i<3;i++) printf("   %s",abuff[i]);
    for(i=3;i<8;i++) printf("%d. %s",i-2,abuff[i]);
    if (!yesno2(" ## 쪽지를 발송할까요? (Y/n)>>",Yes)) return;
    for (i=0;i<8;i++) del_enter(abuff[i]);

    sprintf(id_file,"%s/system/clubid.dat",club_go.dir);
    if ((fp=fopen(id_file,"r"))==NULL) {mess("동호회 회원파일없음");return;}
    count=0;
    while (fread(&cpf2,sizeof_cpf,1,fp)>0) {
        if (strlen(cpf.id)<3||cpf2.type<type1||cpf2.type>type2) continue;
        if (!read_pf4(cpf.id)) continue;
        if (pf2.tmail[0]=='*') continue;
        sprintf(filename,"userdata/%s/%s/smemo.dat",c_han(pf2.id),pf2.id);
        if ((fp=fopen(filename,"a+"))==NULL) {
            if ((fp=fopen(filename,"w+"))==NULL) continue;
        }
        for (i=0;i<8;i++) fprintf(fp,"%s\n",abuff[i]); //쪽지저장
        fclose(fp);
        sprintf(buff,"\n ## %s 님의 쪽지도착(쪽지읽기:RMEMO)\n",pf.id);
        to_mess(pf2.id,buff);
        printf(" %6d.  %-8s %-8s 쪽지 발송\n",count+1,pf2.id,pf2.name);
        count++;
	}
}
