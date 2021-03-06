////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
//가기 코드 처리
////////////////////////////////////////////////////////////////////////

#include "bbs.h"

//다음메뉴로 이동
go_next(int mode)
{
    return go_back_next(1);
}
go_back(int mode)
{
    return go_back_next(2);
}


//다음메뉴로 이동
go_back_next(int direct)
{
	FILE *fp;
	int n,i;
	char deep;
	int found;
	GO oldgo,tgo,tgo2;
	int ndeep;
	found=0;

	re_print=Yes;
    view_pos=0;
	if (list_mode) {
		if (direct==1) go_next_list();
		else		   go_back_list();
		return Yes;
	}

    set_nowmenu();
    if ((fp=fopen(NOW_MENU,"r"))==NULL) return No;
	oldgo=go;
	//NEXT
	if (direct==1) {
		ndeep=go.deep;
        fseek(fp,go.rec*sizeof_go,SEEK_SET);
		found=No;
		while(fread(&tgo,sizeof_go,1,fp)) {
            if (tgo.deep<ndeep) break;
            else if (tgo.deep>ndeep||strlen(tgo.code)<1||tgo.type<=' ') ;
			else if (!is_same(oldgo.code,tgo.code)) {  //같을때
				go=tgo;
				found=Yes;
				break;
			}
		}
        //검색실패일때 상위에서 처음 메뉴를 검색
		if (!found) {
            n=oldgo.rec;
            while(1) {
                fseek(fp,n*sizeof_go,SEEK_SET);
                if (!fread(&tgo,sizeof_go,1,fp)) break;
				if (tgo.deep<ndeep) break;
                else if (is_char(tgo.wtimes,'@'));
                else if (tgo.deep>ndeep||tgo.code[0]==0||tgo.type<=' ') ;
				else {
					go=tgo;
					found=Yes;
				}
                n--;
			}
		}
	}
	else {		  //BACK
		ndeep=go.deep;
		n=go.rec-1;
		if (n<0) n=0;
		found=No;
		while(1) {
            fseek(fp,n*sizeof_go,SEEK_SET);
			if (!fread(&tgo,sizeof_go,1,fp)) break;
			n--;
            if (tgo.deep<ndeep) break;
            else if (is_char(tgo.wtimes,'@')) continue;
            else if (tgo.deep>ndeep||tgo.code[0]==0||tgo.type<=' ') continue;
			else if (!is_same(oldgo.code,tgo.code)) {  //같을때
				go=tgo;
				found=Yes;
				break;
			}
		}
		//검색실패일때
		if (!found) {
			n=go.rec+1;
            fseek(fp,n*sizeof_go,SEEK_SET);
			tgo2=go;
			while(fread(&tgo,sizeof_go,1,fp)) {
				n++;
				if (tgo.deep<ndeep) break;
                else if (tgo.deep>ndeep||tgo.code[0]==0||tgo.type<=' ');
				else {
					tgo2=tgo;
					found=Yes;
				}
			}
			if (found) {
				go=tgo2;
				found=Yes;
			}
		}
	}
    fclose(fp);

    read_bcfg();
    if (found&&go.type=='s') {
        club_go=go;
		club_mode=Yes;
		re_club=Yes;
	}
    else if (!found||check_type('j')||check_type('i')) {
        go=oldgo;
        found=No;
        read_bcfg();
    }
    if (is_typepds) {
        del_space(go.dir);
        if (strlen(go.dir)<1) {
            if (club_mode) sprintf(go.dir,"%s/pds/%s",club_go.dir,go.code);
            else           sprintf(go.dir,"pds/%s",go.code);
            check_dir(go.dir);
        }
    }
    if (found) up_level_cash();
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	return (found);
}


prev()
{
    go_prev();
    while (is_char("rejnp",go.type)) go_prev();
}


//p처리
go_prev()
{
	//번호보기 모드일때
    if (list_mode) {
        list_mode=No;
        re_print=Yes;
        return;
    }
	p_command=Yes;
	bbs_ltmode=No;
    if (club_mode&&go.rec==0) go_prev1();  //동호회의 상위메뉴에서 p를 사용함
    else                      go_prev2();     //일반상태에서 P 사용
	return;
}



//동호회에서 빠져나옴
go_prev1()
{
	FILE *fp;
    int i,n,found=No;
    GO tgo,tgo2,oldclub_go;
    oldclub_go=club_go;
    go_top(0);  //메인메뉴의 최상단메뉴에서 시작
    tgo2=go;
    if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
        i=oldclub_go.rec;
        while(i>=0) {
            fseek(fp,i*sizeof_go,SEEK_SET);
            if (!fread(&tgo,sizeof_go,1,fp)) break;
            if (tgo.deep<oldclub_go.deep&&tgo.type=='m') {
                tgo2=tgo;
                found=Yes;
                break;
            }
            i--;
        }
        fclose(fp);
    }
    if (found) go=tgo2;
    else go_top(1);
    read_bcfg();
    return;
}


//p처리
go_prev2()
{
	FILE *fp;
    int i,n,found=No;
	GO tgo,tgo2;

    set_nowmenu();
	go_prev2_:
	if ((fp=fopen(NOW_MENU,"r"))!=NULL) {
		i=go.rec-1;
		go_top(1);
		tgo2=go;
		while(i>=0) {
            fseek(fp,i*sizeof_go,SEEK_SET);
			if (!fread(&tgo,sizeof_go,1,fp)) break;
            else if (tgo.deep<go.deep&&tgo.type=='m') {
                found=Yes;
				tgo2=tgo;
				break;
			}
			i--;
		}
		fclose(fp);
	}
    if (found) go=tgo2;
    else go_top(1);

	if (go.type=='r'||go.type=='t') goto go_prev2_;
	read_bcfg();
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	list_mode=No;
	return;
}



//메뉴를 가기코드로 선택함
go_code(char *buff)
{
	FILE *fp;
	int n,n1,n2,ns,deep,i,i2;
	int found=No;
    char buffgo[80];
	char buff1[80];
	GO find_arr_tgo[100];
    GO tgo,oldgo,oldclub_go;
    int oldclub_mode;
    int main_find=No;

	oldgo=go;
    oldclub_go=club_go;
    oldclub_mode=club_mode;

	strcpy(buffgo,buff);
    if (go.type!='t'&&is_same(go.code,buffgo)) goto go_code_ok;
    if (is_same(buffgo,"t")||is_same(buffgo,"top")||is_same(buffgo,"main")) {
        go_top(0);
        goto go_code_ok;
    }
	go_top(1);
	if (club_mode) {
        //동호회메인메뉴에서 동일한 메뉴를 먼저 찾는다.
        if (is_goindex(NOW_MENU,buffgo)&&(fp=fopen(NOW_MENU,"r"))!=NULL) {
			while(fread(&tgo,sizeof_go,1,fp)) {
				if (is_same(tgo.code,buffgo)&&tgo.type!='t'&&tgo.type!=' ') {
					go_top(0);
					go=tgo;
					fclose(fp);
					goto go_code_ok;
				}
			}
			fclose(fp);
		}
	}
    //메인메뉴에서 찾기
    go_top(0);
    found=No;
    if (is_goindex(NOW_MENU,buffgo)) {
        if ((fp=fopen(NOW_MENU,"r"))==NULL) return No;
        while(fread(&tgo,sizeof_go,1,fp)) {
            //동일한 메뉴를 먼저 찾는다.
            if (is_same(tgo.code,buffgo)&&tgo.type!='t'&&tgo.type!=' ') {
                go=tgo;
                fclose(fp);
                if (oldclub_mode) main_find=Yes;
                found=Yes;
                goto go_code_ok;
            }
        }
        fclose(fp);
    }

    //TOP 에서 유사한 메뉴를  찾는다.
    for (i=0;i<50;i++) strcpy(find_arr_tgo[i].code,"");
    n1=0;
    if ((fp=fopen(NOW_MENU,"r"))==NULL) goto _fail;
    while(strlen(buffgo)>1&&n1<100&&fread(&tgo,sizeof_go,1,fp)) {
        if (tgo.type!='t'&&tgo.type>=' '&&strlen(tgo.code)>2) {      //포함여부?
            if (stristr(tgo.code,buffgo)||
                (!bit(cfg.gomode,2)&&strlen(tgo.name)>0&&stristr(tgo.name,buffgo))) {
                found=No;
                for (i=0;!found&&i<50;i++) {
                    if (is_same(find_arr_tgo[i].code,tgo.code)) {
                        found=Yes;
                        break;
                    }
                }
                if (!found) find_arr_tgo[n1++]=tgo;
            }
        }
        if (n1>0) main_find=Yes;
    }
    fclose(fp);

    if (n1==0) goto _fail;
    if (n1==1) {      //자료가 1개밖에 없을때
        go_top(0);
        go=find_arr_tgo[0];
        found=Yes;
        goto go_code_ok;
    }

	//유사자료를 찾았을때 선택하기
	if (n1>1) {  //n1:찾은숫자 최고100개까지..
		ns=0;
		while (1) {
			clrscr();
			printf("                        ━━━━━[[7m 가기코드 [m]━━━━━\n");
			printfL2();
			i=0;
            for (n=ns;n<n1&&i<20&&n<50;n++,i++) {
				if (find_arr_tgo[n].code[0]!=0) {
					printf("  %2d. %-9s %-22.22s",n+1,find_arr_tgo[n].code,find_arr_tgo[n].name);
					if (n%2==1) printf("\n");
				}
			}
			xy(0,18);
			printfL2();
			printf("번호를 선택하세요 (0:취소 번호:선택) >>");
			lineinput(buff1,4); 	   //명령어 입력
			check_cmd(buff1);	   //명령어로 분해
			n2=mmn[0];
			if (n2==0||**mmc=='p') break;
			if (n2>0&&n2<=n1) {
				go_top(0);
				go=find_arr_tgo[n2-1];
				goto go_code_ok;
			}
			if (ns>n1) ns=0;
			ns+=20;
		}
	}

	//최종적으로 못찾았을때 원래 자료를 보여줌
    _fail:
    go=oldgo;
    club_mode=oldclub_mode;
    club_go=oldclub_go;
    set_nowmenu();
	re_print=Yes;
	list_mode=No;
    re_logo=No;
    re_club=No;
	return No;

	go_code_ok:
    club_mode=oldclub_mode;
    club_go=oldclub_go;
    if (main_find) {  //동호회검색실패->메뉴에서 찾음
        club_mode=No;
        tgo=go;
        go_top(0);
        go=tgo;
		re_club=Yes;
    }
	read_bcfg();
    all_trim(go.dir);
	if (go.type=='s') {    //선택한 메뉴가 동호회의 경우
        club_mode=No;strcpy(club_go.code,"main");
        check_another_rec();        //동호회위치변경시 기록
        club_go=go;
		club_mode=Yes;
		re_club=Yes;
	}
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	list_mode=No;
    if (is_typepds) {
        del_space(go.dir);
        if (strlen(go.dir)<1) {
            if (club_mode) sprintf(go.dir,"%s/pds/%s",club_go.dir,go.code);
            else           sprintf(go.dir,"pds/%s",go.code);
            check_dir(go.dir);
        }
    }
    return (Yes);
}


is_gocode(char *tcode)
{
	GO oldgo;
	int ret;
	oldgo=go;
    ret=find_go(tcode);
	go=oldgo;
	return ret;
}


//메뉴를 초기 메뉴코드로 선택함
go_top(int mode)
{
	strcpy(go.code,"top");
    strcpy(go.name,"메인메뉴");
    if (club_mode) strcpy(go.name,club_go.name);
    newsmode[0]=newsmode[1]=newsmode[2]=newsmode[3]=0;
	go.rec=0;
	go.type='m';
	re_print=Yes;
	re_logo=Yes;
	re_bbs=Yes;
	list_mode=No;
	bbs_ltmode=No;
	if (mode==0) {
		now_type=1;
		club_mode=No;
		club_go.rec=0;
		strcpy(club_go.dir,".");
		strcpy(club_go.code,"main");
        strcpy(club_go.name,"메인메뉴");
	}
    set_nowmenu();
    read_bcfg();
}


gg_code(int mode)
{
	char buff[10];
	int i;
	GG gg[10];
	FILE *fp;

	for (i=0;i<10;i++) {
		strcpy(gg[i].code,"");
		strcpy(gg[i].name,"");
	}

	fp=fopen("system/gg.dat","r+");
	if (fp!=NULL) {
		for (i=0;i<10;i++) fread(&gg[i],sizeof_gg,1,fp) ;
		fclose(fp);
	}

	re_print=Yes;
	if (mode>10) return;
	if (mode==0) {
		printf("\n");
		printfL2();
		printf("     %s 추천 동호회/게시판\n",cfg.hostname);
		printfL2();
		for (i=0;i<10;i++)
			printf("       [  G%-2d ] %-20s ( %-8s )\n",i+1,gg[i].name,gg[i].code);
		printfL2();
		printf("     선택  취소(Q)  >>");
		lineinput(buff,4);
		mode=atoi(buff);
		if (mode==0) {
			buff[0]=' ';
			mode=atoi(buff);
		}
		if (mode<1||mode>10) return;
	}
	if (gg[mode-1].code[0]==0) return;
	go_top(0);
	go_code(gg[mode-1].code);
	check_smenu("");
}


mm_code(int mode)
{
	int i;
    char mm[31][250];
	char filename[250];
    char mtitle[200];
	FILE *fp;

    for (i=0;i<30;i++) strcpy(mm[i],"");

    sprintf(filename,"%s/logo/%s.dim",club_go.dir,go.code);
    if (bbs_man(pf.id)&&mode==0) xedit(filename);
    if ((fp=fopen(filename,"r"))!=NULL) {
        for(i=0;i<30;i++) {
            fgets(mm[i],200,fp);del_enter(mm[i]);
        }
		fclose(fp);
        if (mode>0&&mode<30&&mm[mode][0]!=0) strcpy(auto_cmd,mm[mode]);
	}
}


up_level_cash()
{
    if (bcfg.idlevel==0&&bcfg.idcash==0) return;
    if (!read_pf4(pf.id)) return;
    pf2.level+=bcfg.idlevel;
    pf2.cash +=bcfg.idcash;
    save_pf2(pf2.id);
}
