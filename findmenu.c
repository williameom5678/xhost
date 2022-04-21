////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 내용 : 메뉴 관련 루틴  findmenu
////////////////////////////////////////////////////////////////////////		who=0;
#include "bbs.h"

char out_file[200];
char filename[200];
char buffgo[50];
char abuff[3001][101];
//메뉴를 수정한다.
main(argc, argv)
int argc;
char *argv[];
{
    int mode;
	char buff[255];

	read_cfg();
	findport(tty);
	set_tmpfile();
	strcpy(buffgo,"");
	mode=atoi(argv[1]);
    if (mode==2) {find_club("main");mode=0;}
    else         find_club(argv[2]);
	set_nowmenu();		 //데이타화일지정하기
    strcpy(out_file,argv[3]);
	unlink(out_file);

    if (mode==0) {
		view("frm/go/gosel.log");
		lineinput(buff,20);
		if (strlen(buff)<1) return;
        if (is_same(buff,"h")) {
            view("frm/go/gohelp.log");
        }
        else if (is_same(buff,"0")) {
            find_menu(1);
        }
        else if (is_same(buff,"1")) {
            if (!view("%s/frm/go/goindex.log",club_go.dir))
                 view("frm/go/goindex.log");
        }
        else {
            find_menu(3);        //검색
        }
	}
    else if (mode==3) find_menu(3);        //검색
    else if (mode==4) find_menu2(1);        //검색
    else if (mode==5) find_menu2(2);        //검색
}


find_menu(int mode)
{
	FILE *fp,*fp2;
    int n,i,found,count,top;
	char buff[255];

    sprintf(filename,"%s/menustru/menu.dat",club_go.dir);
	if ((fp=fopen(filename,"r"))==NULL) {
		mess("%s 파일이 없습니다.",filename);
		return;
	}

	if (mode==3) {
		gofind();
        goto _quit;
	}
	count=0;
	for (i=0;i<3000;i++) strcpy(abuff[i],"");
	if (mode==1||mode==2) {
		while(count<3000&&fread(&go,sizeof_go,1,fp)) {
            if (is_char(go.wtimes,'@')) continue;
            if (is_bbspds||is_char("msejnxdr",go.type)) {
				if (mode==1||stristr(go.code,buffgo)) {
					sprintf(buff,"%-8s %s",go.code,go.name);
                    //중복여부 검사
					found=No;
					for (i=0;i<count;i++) {
						if (is_same(abuff[i],buff)) {found=Yes;break;}
					}
					if (!found) {
						strncpy(abuff[count],buff,80);
						count++;
					}
				}
			}

		}
	}
	fclose(fp);
	if (count==0) {
		mess("해당되는 가기코드가 없습니다.");
		return;
	}

	top=0;
	found=No;
	while (1) {
		if (top>=count) top=count-1;
        if (count<20||top<0) top=0;
		cls();
		if (mode==1) strcpy(g_buff1,"전체");
		else		 strcpy(g_buff1,buffgo);
		sprintf(g_buff2,"(%d/%d)",(top/20)+1,(count/20)+1);
        view("frm/go/gosel.lst");
		printf(C_SAVE);
		xy(1,3);
		for (i=top;i<top+20;i++) {
			printf("%3d. %s\n",i+1,abuff[i]);
		}
		printf(C_UNDO);
		lineinput(buff,60);check_han(buff);printf("\n");
		n=atoi(buff);
		if (is_same(buff,"q")||is_same(buff,"p")) break;
		else if (is_same(buff,"z")) continue;
		else if (is_same(buff,"b")) {top-=20;continue;}
		else if (is_same(buff,"f")) ;
		else if (n>0&&n<=count) {
			found=Yes;
			check_cmd(abuff[n-1]);
            if (is_same(mmc[0],"main")) strcpy(mmc[0],"");
			strcpy(auto_cmd,mmc[0]);
			break;
		}
		else if (strlen(buff)==0) ;
		else if (strlen(buff)<2) {
			found=No;
			break;
		}
		else {
			found=Yes;
			strcpy(auto_cmd,buff);
			break;
		}
		top+=20;
	}
    if (!found) {
        unlink(out_file);
        return;
    }

	_quit:
    unlink(out_file);
	if (strlen(auto_cmd)>0) {
		if ((fp2=fopen(out_file,"w+"))==NULL) return;
		fprintf(fp2,"%s",auto_cmd);
		fclose(fp2);
	}
}


find_menu2(int mode)
{
	FILE *fp,*fp2;
    int n,i,found,count,top;
    char buff[255],filename[255];

    sprintf(filename,"%s/menustru/menu.dat",club_go.dir);
	if ((fp=fopen(filename,"r"))==NULL) {
		mess("%s 파일이 없습니다.",filename);
		return;
	}
    if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
    while(fread(&go,sizeof_go,1,fp)) {
        if (is_char(go.wtimes,'@')) continue;
        if (mode==1&&!is_typepds) continue;
        if (mode==2&&!is_char("nre",go.type)) continue;
        fprintf(fp2,"%d %c %-8s %-20.20s %s\n",go.deep,go.type,go.code,go.name,go.dir);
    }
	fclose(fp);
    fclose(fp2);
    view_text(TMP_FILE);
}


//가기코드
typedef struct {
    char club[9] ;
	char code[9];			//가기코드
	char name[41];			//메뉴명
}INDEXGO;
#define sizeof_indexgo           (sizeof(INDEXGO))

//가기코드
typedef struct {
    char club[9];
    char dir[61];           //가기코드
}ACLUB;

gofind()
{
    char buff[100];
    char buff1[100];
    char buff2[100];
    strcpy(buff1,"");
    strcpy(buff2,"");
	view("frm/go/gofind.scr");
    xy(19,15);dosinput(buff1,20);
    if (is_same(buff1,"q")||is_same(buff1,"p")) return;
    while(1) {
        cls();
        view("frm/go/gofind.scr");
        xy(19,15);printf(buff1);
        xy(21,20);
        lineinput(buff,20);check_han(buff);printf("\n");
        if (is_same(buff,"q")||is_same(buff,"p")) return;
        if (is_same(buff,"0")) {
            xy(19,15);
            dosinput(buff1,20);
            if (is_same(buff1,"q")||is_same(buff1,"p")) return;
        }
        else if (is_same(buff,"9")) {   //검색시작
            if (strlen(buff1)<1) {
                xy(1,24);
                printf("검색내용을 입력하세요! ");pressenter();
                continue;
            }
            view_ind(buff1,buff2);
            break;
        }
    }
}


view_ind(char *buff1,char *buff2)
{
    FILE *fp;
    char menu_ind[300];
    char buff[300];
    int i,n,count,found,top;
    INDEXGO indexgo;
    strcpy(auto_cmd,"");
    strcpy(menu_ind,"menustru/menuind.tmp");
    if ((fp=fopen(menu_ind,"r"))==NULL) {
        make_manu_index();
        if ((fp=fopen(menu_ind,"r"))==NULL) return;
    }
    count=0;
    for (i=0;i<3000;i++) strcpy(abuff[i],"");
    del_space(buff1);
    while(count<3000&&fread(&indexgo,sizeof_indexgo,1,fp)) {
        strcpy(buff,indexgo.name);del_space(buff);
        if (stristr(indexgo.code,buff1)||stristr(buff,buff1)) {
            sprintf(buff,"%-8s %-8s %s",indexgo.club,indexgo.code,indexgo.name);
            //중복여부 검사
            found=No;
            for (i=0;i<count;i++) {
                if (is_same(abuff[i],buff)) {found=Yes;break;}
            }
            if (!found) {
                strncpy(abuff[count],buff,80);
                count++;
            }
        }
    }
    if (count==0) {
        mess("해당되는 가기코드가 없습니다.");
        return;
    }

    top=0;
    found=No;
    while (1) {
        if (top>=count) top=count-1;
        if (count<20||top<0) top=0;
        cls();
        strcpy(g_buff1,buff1);
        sprintf(g_buff2,"(%d/%d)",(top/20)+1,(count/20)+1);
        view("frm/go/gofind.lst");
        printf(C_SAVE);
        xy(1,3);
        for (i=top;i<top+20;i++) {
            printf("%3d. %s\n",i+1,abuff[i]);
        }
        printf(C_UNDO);
        lineinput(buff,60);check_han(buff);printf("\n");
        n=atoi(buff);
        if (is_same(buff,"q")||is_same(buff,"p")) break;
        else if (is_same(buff,"z")) continue;
        else if (is_same(buff,"b")) {top-=20;continue;}
        else if (is_same(buff,"f")) ;
        else if (n>0&&n<=count) {
            found=Yes;
            check_cmd(abuff[n-1]);
            if (is_same(mmc[0],"main")) strcpy(mmc[0],"");
            sprintf(auto_cmd,"%s %s",mmc[0],mmc[1]);
            break;
        }
        else if (strlen(buff)==0) ;
        else if (strlen(buff)<2) {
            found=No;
            break;
        }
        else {
            found=Yes;
            strcpy(auto_cmd,buff);
            break;
        }
        top+=20;
    }
    fclose(fp);
}


make_manu_index()
{
    FILE *fp,*fp2;
    char filename[300];
    char menu_ind[300];
    ACLUB aclub[501];
    INDEXGO indexgo;
    int i,n,count,found,club_count;
    //메인 읽기
    strcpy(menu_ind,"menustru/menuind.tmp");
    if ((fp2=fopen(menu_ind,"w"))==NULL) return;

    count=0;
    club_count=0;
    sprintf(filename,"%s/menustru/menu.dat",club_go.dir);
    if ((fp=fopen(filename,"r"))==NULL) return;
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
                    strcpy(aclub[club_count++].club,go.code);
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
}
