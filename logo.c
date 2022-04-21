////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 이 소스를 변경하여 상용으로 사용하실때에는  개발자의 승인을 받아야 합니다.
// 내용 : 각종 화면관련 세팅
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
//로고화일이 없을때의 처리
no_logofile()
{
	FILE *fp;
	char deep,ndeep;
	int i,n,is_bcfg;
	char c;
	GO tgo;
	char buff[90];
	char buff1[90];
	char buff2[90];
	char filename[290];
	BCFG oldbcfg;
	oldbcfg=bcfg;

	if (go.type!='m') return;
	cls();
	if (is_same(go.code,"top")) set_bbs_logo("top",filename);
	else						set_bbs_logo("sub",filename);
	view(filename);
	if (view("%s/logo/%s.mnt",club_go.dir,go.code)) return;
	xy(1,5);
	if ((fp=fopen(NOW_MENU,"r"))==NULL) return;
    fseek(fp,(go.rec+1)*sizeof_go,SEEK_SET);
	fread(&tgo,sizeof_go,1,fp);
	ndeep=tgo.deep;
    fseek(fp,(go.rec+1)*sizeof_go,SEEK_SET);
	while(fread(&tgo,sizeof_go,1,fp)) {
		if (tgo.deep<ndeep) break;
		if (tgo.deep>ndeep) continue;
		if (tgo.code[0]==0) continue;
		if (is_char(tgo.wtimes,'@')) continue;
		if (tgo.type<=' ') continue;
		else {
			sprintf(buff1,"%4s. %s",tgo.num,tgo.name);
			is_bcfg=read_bcfg();
			if (is_bbspds&&is_bcfg&&strlen(bcfg.man1)>0) {
				sprintf(buff," <담당:%s>",bcfg.man1);
				strcat(buff1,buff);
			}
			printf("%-65s",buff1);
				 if (is_bcfg&&is_typebbs&&bit(bcfg.modelist,15)) printf(" %s",b_num(tgo.code,2));
			else if (is_bcfg&&is_typepds&&bit(bcfg.modelist,16)) printf(" %s",b_num(tgo.code,2));
			else if (is_bcfg&&is_typetel&&bit(bcfg.modelist,17)) printf(" %s",tgo.dir);
			else if (is_bcfg&&is_typeexe&&bit(bcfg.modelist,18)) printf(" %s",tgo.dir);
			printf("\n");
		}
	}
	bcfg=oldbcfg;
	printf("\n");
}



set_bbs_logo(char *mstr,char *filename)
{
	char buff[200];
	char tail[100];
	char filename1[300];
	char filename2[300];
	int is_logo=No;
	int bbs_mode=is_typebbs;
	strcpy(filename,"");
	strcpy(filename1,"");strcpy(filename2,"");strcpy(filename1,"");strcpy(filename2,"");
	strcpy(tail,"");
    if (bcfg.bform>0&&is_bbspds) {
        sprintf(filename,"frm/screen/%u.%s",bcfg.bform,mstr);
        is_logo=is_file2(filename);
        if (is_logo) goto cont_1;
    }

    sprintf(filename,"%s/logo/%s.%s",club_go.dir,go.code,mstr);
    local_logo(filename);    //로칼메뉴로고처리하기
    is_logo=is_file2(filename);
	if (!is_logo) {
		strcpy(tail,bbs_tail());
		sprintf(filename1,"%s/frm/screen/%s.%s",club_go.dir,mstr,tail);
		sprintf(filename2,"frm/screen/%s.%s",mstr,tail);
		strcpy(filename,filename1);
        is_logo=is_file2(filename);
		if (!is_logo&&club_mode) {
			strcpy(filename,filename2);
            is_logo=is_file2(filename);
		}
	}
    cont_1:
	if (bit(allmode,10)) {
		printf("1.%s(%d B) \n",filename ,file_size(filename ));
		printf("2.%s(%d B) \n",filename1,file_size(filename1));
		printf("3.%s(%d B) \n",filename2,file_size(filename2));
		printf("결과:%s , 파일:%s(%d B) (%s/%s)\n",is_logo?"찾음":"못찾음",filename,file_size(filename),mstr,tail);
		getchar2();
	}
	return is_logo;
}




view_mbbs(int mode,char *filename,char *buffnum,int npage)
{
	char buff[10000],buff2[10000];
	int len;
	len=file2buff(buff,filename,100000);
	if (mode==1) {
		del_enter(buff);
		sprintf(buff2,"%-79.79s\n",buff);
		view3(buff2);
	}
	else view3(buff);
}


view6(char *filename,int posi,int size)
{
	FILE *fp;
	int i,len=0;
	char ch;
	if ((fp=fopen(filename,"r+"))!=NULL) {
		fseekend(fp);
		len=ftell(fp);
		fseek(fp,posi,SEEK_SET);
		i=0;
		while (posi<len&&i++<size&&posi++<len) putchar(getc(fp));
		fclose(fp);
		return Yes;
	}
	return No;
}



cadd_str(char *buff1,char *buff2,char *form)
{
	char tmp1[300];
	sprintf(tmp1,form,buff2);
	strcat(buff1,tmp1);
}

cadd_num(char *buff1,int num,char *form)
{
	char tmp1[300];
	sprintf(tmp1,form,num);
	strcat(buff1,tmp1);
}


//파일존재여부 검색속도를 빠르게 하기위한 조치
is_file2(char *filename)
{
    int ret,i;
    if (strlen(filename)>50) return (access(filename,0)!=-1);
    //없는 파일인가?
    for (i=0;i<10;i++) {
        if (strcmp(a_is_file[i],filename)==0) return No;
    }
    ret=(access(filename,0)!=-1);
    if (ret==No) {
        for (i=9;i>0;i--) strcpy(a_is_file[i],a_is_file[i-1]);
        strcpy(a_is_file[0],filename);
    }
    return ret;
}

