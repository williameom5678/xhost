////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 내용 : 시삽메뉴 관련 루틴
////////////////////////////////////////////////////////////////////////
#include "bbs.h"

char idfile[300];
//char atit[10][30];
//char afile[10][300];
char atit[][30]={"회원정보","가입신청","가입보류","가입거부","가입취소","회원탈퇴","회원삭제","회원연체"};
char afile[][300]={ID_FILE,GUEST_FILE,GUESTWAI_FILE,GUESTNOT_FILE,GUESTCAN_FILE,GUESTOUT_FILE,GUESTDEL_FILE,GUESTDEP_FILE};
int nn,top,dsp;
int tot[10];
int allrec;
char nowid[10];
struct termio term_old;

main(int argc, char **argv)
{
    FILE *fp;
    int i,i2,n,k;
    char buff[200];
    int  arec[100];
    int sysmode=No;
    PF  apf[100];
    PF oldpf;
    findport(tty);
    read_cfg();
    set_tmpfile();
    read_ajob();      //직업을 읽어옴
    strcpy(nowid,argv[1]);
    if (is_same(nowid,"root")) sysmode=Yes;
	ioctl(0, TCGETA, &term_old);
	rawmode();	// 넌 캐노니칼 모드
    if (argc>1) strcpy(buff,argv[2]);
    else {
        printf("\n 편집할 ID, 성명또는 주민등록번호일부분을 지정하세요 >>");
        keyinput2(buff,8);
    }
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
    //총수 파악
    fseek(fp,1,SEEK_END);
    allrec=ftell(fp)/sizeof_pf;
    fclose(fp);
    if (n<1) {
        mess("%s 자료를 찾을수 없습니다.",buff);
        goto all_quit;
    }
    if (n>1) {
        printf("\n        < 다음의 회원중에서 선택하세요 > \n");
        for (i=0;i<n;i++) {
            printf(" %3d. %-8s (%-8s) %14s %-30.30s\n",i+1,apf[i].id,apf[i].name,apf[i].id_no,apf[i].addr);
            if (i%20==19) getchar3();
        }
        printf(" 선택 >>");
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
    //시삽은 수정불가능하게
    if (sysmode||pf.type<18) {
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
    printf(" 이대로 저장할까요?  저장(Y) 취소(q) 재입력(r) >>");
    keyinput2(buff,10);check_han(buff);printf("\n");
    if (is_same(buff,"q")) goto all_quit;
    if (is_same(buff,"r")) goto re_start;
    if (pf.num>=0&&(fp=fopen(ID_FILE,"r+"))!=NULL) {
        fseek(fp,pf.num*sizeof_pf,SEEK_SET);
//      fwrite(&pf,sizeof_pf,1,fp);
        gwrite(187,pf,fp);
        fclose(fp);
    }
    goto all_quit;

save_no:
    cls();
    pf=oldpf;
    disp_pf9();
    printf(" 원래대로 복구되었습니다. [Enter]");getchar3();
    goto all_quit;

all_quit:
	ioctl(0, TCSETA, &term_old);
    printf("\n");
    return;
}

disp_pf9()
{
    cls();
    printf("[0;1m << %s(%s)님의 신상 변경 회원번호 : %d / %d >>\n",pf.id,pf.name,pf.num+1,allrec);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf(" 1. ID      : %-8s\n",pf.id);
    printf(" 2. 성명    : %-8s\n",pf.name);
    printf(" 3. 암호    : %-8s\n",pf.type>=18?starlen(pf.passwd):pf.passwd);
    printf(" 4. 등급    : %d\n",pf.type);
    printf(" 5. 레벨    : %d\n",pf.level);
    printf(" 6. 다운용량: %d\n",pf.down);
    printf(" 7. 사이버M : %d\n",pf.cash);
    printf("11. 주소    : %s\n",pf.addr);
    printf("12. 전화    : %s\n",pf.tele);
    printf("13. 삐삐    : %s\n",pf.bibi);
    printf("14. 핸드폰  : %s\n",pf.hand);
    printf("15. 주민등록: %s\n",pf.id_no);
    printf("                \n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    xy(46,3);printf("가입일시 : %s\n",datestr(2,pf.inday));
    xy(46,4);printf("최종접속 : %s\n",datestr(2,pf.logout));
    xy(46,5);printf("사용시간 : %s\n",datestr(21,pf.all_time));
    xy(46,6);printf("총접속수 : %d\n",pf.all_log);
    xy(46,7);printf("월사용   : %s\n",datestr(21,pf.month_time));
    xy(46,8);printf("월접속수 : %d\n",pf.month_log);
    xy(1,18);
}

rawmode()
{
	struct termio tdes;
	ioctl(0,TCGETA,&tdes);	 //현재 터미날 모드를 읽어온다
	tdes.c_iflag = 0;
	tdes.c_iflag |= ICRNL ;
	tdes.c_iflag |= IXON ;
	tdes.c_oflag = 0;
	tdes.c_oflag |= OPOST ;
	tdes.c_oflag |= ONLCR ;
	tdes.c_cflag |= CRTSCTS ;
	tdes.c_cflag |= HUPCL ;
	tdes.c_lflag = 0;
	tdes.c_lflag |= IEXTEN;
	tdes.c_cc[VMIN]=1;
	tdes.c_cc[VTIME]=0;
	ioctl(0,TCSETA,&tdes);	 //현재 터미날 모드를 저장한다.
}


keyinput2(char *buff,int len)
{
    lineinput(buff,len);
    return 0;
}
