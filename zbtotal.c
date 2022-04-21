////////////////////////////////////////////////////////////////////////
//			  XHOST 5.3s: 리눅스용 BBS HOST							  //
////////////////////////////////////////////////////////////////////////
/*
 * 전체 자료실/게시판을 만드는 루틴
 * 데이타에 지정된 게시판화일을 읽어 새로운 화일에 추가한다.
 * 옵션에 따라 7일간의 자료실로 사용이 가능하다.
 * 자료의 다운일때에는 해당 자료실의 권한에 따라 지정되어야 한다.
 */

#include "bbs.h"

char zfile[100][9];
char disp_proc;
char only_mode;
char force_mode;
char lite_mode;
char ztype,zgo_type;

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp1,*fp2;
	char buff[300];
	char buff1[100];
	char buff2[100];
	char tclub[200];
	char zcode[100];
	char gcode[100];
	char ncode[100];
	int mode;
    int i,i1,i2,i3,count,found,rebnum=No;
	GO go2,zgo;

	disp_proc=No;
	only_mode=No;
	force_mode=No;
	lite_mode=No;

	strcpy(tclub,"main");
	for (i=1;i<argc;i++) {
			 if (is_same(argv[i],"-p"))  disp_proc=Yes;
		else if (is_same(argv[i],"-m"))  only_mode=1;        //자료실만
		else if (is_same(argv[i],"-m1")) only_mode=1;        //자료실만
		else if (is_same(argv[i],"-m2")) only_mode=2;        //게시판만
		else if (is_same(argv[i],"-f"))  force_mode=Yes;
		else if (is_same(argv[i],"-l"))  lite_mode=1;
		else if (is_same(argv[i],"-l1")) lite_mode=1;
		else if (is_same(argv[i],"-l2")) lite_mode=2;
		else if (is_same(argv[i],"-l3")) lite_mode=3;
		else if (is_same(argv[i],"-l4")) lite_mode=4;
		else if (is_same(argv[i],"-l5")) lite_mode=5;
		else if (is_same(argv[i],"-l6")) lite_mode=6;
        else if (is_same(argv[i],"rebnum")) rebnum=Yes;
		else if (stristr(argv[i],"/"))   chdir(argv[1]);
		else strcpy(tclub,argv[1]);
	}

	findport(tty); //접속포트 찾기
	read_cfg();
	if (bit(cfg.zmode,1)) disp_proc=Yes;
	if (bit(cfg.zmode,2)) only_mode=1;
	if (bit(cfg.zmode,3)) only_mode=2;
	if (bit(cfg.zmode,4)) force_mode=Yes;
	if (bit(cfg.zmode,11)) lite_mode=1;
	if (bit(cfg.zmode,12)) lite_mode=2;
	if (bit(cfg.zmode,13)) lite_mode=3;
	if (bit(cfg.zmode,14)) lite_mode=4;
	if (bit(cfg.zmode,15)) lite_mode=5;
	if (bit(cfg.zmode,16)) lite_mode=6;
	if (bit(cfg.zmode,17)) lite_mode=7;

	set_tmpfile();		 //임시화일지정하기
	if (!find_club(tclub)) {
		printf("##CLUB NOT FOUND:%s\n",tclub);
		return;
	}
	set_nowmenu();		 //데이타화일지정하기
    if (rebnum) {
        if ((fp2=fopen(NOW_MENU,"r"))==NULL) return;
        while(fread(&go,sizeof_go,1,fp2)) {
            if (go.type<=' '||strlen(go.code)<1) continue;
            if (!is_bbspds) continue;
            sprintf(NOW_FILE ,"%s/menu/%s.tit",club_go.dir,go.code);
            sprintf(NOW_FILED,"%s/menu/%s.txt",club_go.dir,go.code);
            re_bbsnum(3);
        }
        fclose(fp2);
        return;
    }

	strcpy(buff,"tmp/zbtotalr.tmp");
    if(is_file(buff)&&!is_old_file(buff,60*60)) return;
	touch_file(buff);

	for(i=0;i<100;i++) strcpy(zfile[i],"");
	count=0;
	sprintf(buff,"%s/system/zbtotal.dat",club_go.dir);
	if ((fp1=fopen(buff,"r+"))==NULL) goto _quit;  //합치는 화일없다.

	utime(buff,NULL);							 //작업날짜를 오늘날짜로 세팅
	//먼저 전체자료실 부분만 읽어들임
	while (count<100&&fgets(buff,200,fp1)!=NULL) {
		check_cmd(buff);
		strcpy(zcode,mmc[0]);	  //전체 게시판
		strcpy(gcode,mmc[1]);	  //추가할 게시판
		strlwr(zcode);strlwr(gcode);
		del_esc(zcode);del_esc(gcode);
		if (strlen(zcode)<1||strlen(gcode)<1||is_same(zcode,gcode)) continue;
		if (!find_go(zcode)) continue;
		zgo=go;
		zgo_type=is_typebbs;
		ztype=zgo.wtimes[0];
		buff[0]=zgo.wtimes[1];buff[1]=zgo.wtimes[2];buff[2]=0;
			 if (is_same(buff,"l1")) lite_mode=1;
		else if (is_same(buff,"l2")) lite_mode=2;
		else if (is_same(buff,"l3")) lite_mode=3;
		else if (is_same(buff,"l4")) lite_mode=4;
		else if (is_same(buff,"l5")) lite_mode=5;
		else if (is_same(buff,"l6")) lite_mode=6;

		if (only_mode==1&&zgo_type) continue;
		if (only_mode==2&&!zgo_type) continue;

		if (!force_mode) {
			strcpy(buff,"tmp/zbtotalb.tmp");
			if( zgo_type&&is_file(buff)&&!is_old_file(buff,24*60*60)) continue;
			strcpy(buff,"tmp/zbtotalp.tmp");
			if(!zgo_type&&is_file(buff)&&!is_old_file(buff,24*60*60)) continue;
		}
		found=No;
		for (i=0;i<count;i++) {
			if (is_same(zfile[i],zcode)) {found=Yes;break;}
		}
		if (!found) {
			strcpy(zfile[count],zcode);
			count++;
			unlink_file("%s/menu/%s.tit",club_go.dir,zcode);
			unlink_file("%s/menu/%s.txt",club_go.dir,zcode);
            if (zgo_type) touch_file("tmp/zbtotalb.tmp");
            else          touch_file("tmp/zbtotalp.tmp");
		}
		if (disp_proc)
		printf("%s 신규 zcode:%-8s ztype:%1c gcode:%-8s found=%d\n",now_str(10),zcode,ztype,gcode,found);

        if (is_char(gcode,'*')||is_char(gcode,'?')) {
			if ((fp2=fopen(NOW_MENU,"r"))==NULL) break;
			while(fread(&go,sizeof_go,1,fp2)) {
				if (go.type<=' '||strlen(go.code)<1) continue;
				if (!is_bbspds) continue;
				else if (is_char("zbucexd-ok",go.type)) continue;
				else if (is_same(zcode,go.code)) continue;

					 if (zgo_type&&!is_typebbs) continue;
				else if (!zgo_type&&is_typebbs) continue;
				else if (is_same(gcode,"*")) make_add(zcode,go.code,ztype);
				else if (is_char(gcode,'*')) {
                    strcpy(buff1,gcode);
					strcpy(buff2,go.code);
					for (i2=0;i2<(int)strlen(gcode);i2++) {
						if (gcode[i2]=='*') {
							buff1[i2]=0;
							buff2[i2]=0;
							break;
						}
					}
					if (is_same(buff1,buff2)) make_add(zcode,go.code,ztype);
				}
                else if (is_char(gcode,'?')) {
                    strcpy(buff1,gcode);
					strcpy(buff2,go.code);
					for (i2=0;i2<(int)strlen(gcode);i2++) {
                        if (gcode[i2]=='?') {
                            buff1[i2]='?';
                            buff2[i2]='?';
							break;
						}
					}
					if (is_same(buff1,buff2)) make_add(zcode,go.code,ztype);
				}
				else break;
			}
			fclose(fp2);
		}
		else {
			make_add(zcode,gcode,ztype);
		}
	}
	fclose(fp1);
	for (i=0;i<count;i++) {
		zbsort(zfile[i]);
	}
    _quit:
	unlink("tmp/zbtotalr.tmp");
}


//zcode : 전체자료실  codeg:추가할 자료실
make_add(char *zcode,char *gcode,int ztype)
{
	FILE *fp1;
	FILE *fp2;
	FILE *fp3;
	FILE *fp4;
	int i,num,oldsize,newnum,newposi,count,count2,old_num;
	char buff[100];
	char dir[200];
	char tclub[100];
	char filename1[200];
	char filename2[200];
	char filename3[200];
	char filename4[200];
	time_t date1,date2,date3,date7,date30,date100,today;
	GO	 new_go;

	time(&today);
	date1=today-1*24*60*60;
	date2=today-2*24*60*60;
	date3=today-3*24*60*60;
	date7=today-7*24*60*60;
	date30=today-30*24*60*60;
	date100=today-100*24*60*60;

	if (stristr(gcode,"bbs_all")) return;
	if (stristr(gcode,"pbs_all")) return;
	if (is_same("allmail",gcode)) return;
	if (is_same("notice",gcode))  return;
    if (is_same(zcode,gcode))  return;
	new_go=go;
	sprintf(filename1,"%s/menu/%s.tit",club_go.dir,gcode);
	sprintf(filename2,"%s/menu/%s.txt",club_go.dir,gcode);
	sprintf(filename3,"%s/menu/%s.tit",club_go.dir,zcode);
	sprintf(filename4,"%s/menu/%s.txt",club_go.dir,zcode);
//	  printf("%s\n",filename1);printf("%s\n",filename2);printf("%s\n",filename3);
//	  printf("%s\n",filename4);pressenter();
	if (is_same(filename1,filename3)) return;

	fp1=fopen(filename1,"r"); if (fp1==NULL) return;
	fp2=fopen(filename2,"r"); if (fp2==NULL) return;
	fp3=fopen(filename3,"r+"); if (fp3==NULL) fp3=fopen(filename3,"w");
	fp4=fopen(filename4,"r+"); if (fp4==NULL) fp4=fopen(filename4,"w");
	if (fp3==NULL) return;
	if (fp4==NULL) return;
	//저장할 자료의 마지막 위치
	newposi=0;
	newnum =0;
	fseekend(fp3);
	num=ftell(fp3)/sizeof_bbs;
	if (num>0) {
		fseek(fp3,(num-1)*sizeof_bbs,SEEK_SET);
		if (!fread(&bbs,sizeof_bbs,1,fp3)) goto _quit1;
		newnum =bbs.num;
		newposi=bbs.posi+bbs.size;
	}
	old_num=num;
	fseek(fp3,num*sizeof_bbs,SEEK_SET);

	//자료추가 시작
	count=0;
	count2=0;
	while (fread(&bbs,sizeof_bbs,1,fp1)) {
		count2++;
			 if (ztype=='1'&&bbs.date>=date1);
		else if (ztype=='2'&&bbs.date>=date3);
		else if (ztype=='3'&&bbs.date>=date7);
		else if (ztype=='4'&&bbs.date>=date30);
        else if (ztype=='5'&&bbs.del)   bbs.del=No;
        else if (ztype=='6'&&bbs.del&&bbs.date>=date7) bbs.del=No;
        else if (ztype=='7'&&bbs.gd>' ')   bbs.gd=' ';
        else if (ztype=='8'&&bbs.gd>' '&&bbs.date>=date7) bbs.gd=' ';
		else if (ztype=='9'&&is_same(bbs.id,cfg.sysop));
		else if (ztype=='0'||ztype<' ');
		else continue;

        if (bbs.del) continue;

		if (lite_mode==1&&bbs.date<date100&&bbs.size>1024*3) bbs.size=1024*3;
		else if (lite_mode==2&&bbs.date<date100&&bbs.size>1024) bbs.size=1024;
		else if (lite_mode==3&&bbs.date<date100) bbs.size=0;
		else if (lite_mode==4&&bbs.size>1024*3) bbs.size=1024*3;
		else if (lite_mode==5&&bbs.size>1024) bbs.size=1024;
        else if (lite_mode==6) bbs.size=0;

		//내용 저장    2->4
		if (fp2==NULL) bbs.posi=bbs.size=0;
		else {
            if (bbs.posi<0) bbs.posi=0;
            if (newposi<0) newposi=0;
			if (bbs.size>1024*20) bbs.size=1024*20;
			fseek(fp2,bbs.posi,SEEK_SET);
			fseek(fp4,newposi,SEEK_SET);
			for (i=0;i<bbs.size;i++) fputc(fgetc(fp2),fp4);
		}
		//제목 저장
		newnum++;
		count++;
		bbs.num=newnum;
		bbs.posi=newposi;
		strcpy(bbs.tcode,gcode);
		fwrite(&bbs,sizeof_bbs,1,fp3);
		newposi+=bbs.size;
		num++;
	}
	if (count>0&&disp_proc) {
		printf("%s 추가 gcode:%-8s %6d /%6d ->%7d %7uK\n",
		now_str(10),gcode,count,count2,num,newposi/1024);
	}

	_quit1:
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
}


zbsort(char *zcode)
{
	FILE *fp1;
	FILE *fp2;
	FILE *fp3;
	FILE *fp4;
	FILE *fp5;
	int i,num,new_posi,new_num,old_num;
	int n,n1,n2;
	char buff[200];
	char buff2[200];
	char dir[200];
	char tclub[100];
	char gocode1[100];
	char gocode2[100];
	char filename1[200];
	char filename2[200];
	char filename3[200];
	char filename4[200];
	char SORT_FILE[200];

	sprintf(filename1,"%s/menu/%s.tit",club_go.dir,zcode);
	sprintf(filename2,"%s/menu/%s.txt",club_go.dir,zcode);
	sprintf(filename3,TMP_FILE);
	sprintf(filename4,TMP_FILE2);
	sprintf(SORT_FILE,"tmp/tmp3%d%s",cfg.hostno,no_spc(tty));   //호스트번호별임시화일

	if (strlen(zcode)<1) return;
	if ((fp1=fopen(filename1,"r+"))==NULL) return;
	if ((fp5=fopen(TMP_FILE,"w"))==NULL) return;
	num=0;
	while (fread(&bbs,sizeof_bbs,1,fp1)) {
		fprintf(fp5,"%12u %d\n",bbs.date,++num);
	}
	fclose(fp1);
	fclose(fp5);

	if (disp_proc) {
		printf("%s 정렬 zcode:%-8s %8d ",now_str(10),zcode,num);fflush(NULL);
	}

	unlink(SORT_FILE);
    xsort(0,TMP_FILE,SORT_FILE);
	if (!is_file(SORT_FILE)) return;

	fp1=fopen(filename1,"r"); //제목
	fp2=fopen(filename2,"r"); //내용
    fp3=fopen(filename3,"w+"); //새제목
    fp4=fopen(filename4,"w+"); //새내용
	fp5=fopen(SORT_FILE,"r");
	new_posi=0; //새로운 위치
	new_num=1;	//새로운 번호

	while (1) {
		//정렬된 순서읽기  fp5->
		n=fscanf(fp5,"%12s%d",&buff,&n2);
		if (n<1) break;
		//제목읽기 fp1->fp3
		fseek(fp1,(n2-1)*sizeof_bbs,SEEK_SET);
		if (fread(&bbs,sizeof_bbs,1,fp1)<1) continue;
		//내용읽음 -> 새내용  fp2->fp4
		fseek(fp2,bbs.posi,SEEK_SET);
		fseek(fp4,new_posi,SEEK_SET);
		for (i=0;i<bbs.size;i++)
			fputc(fgetc(fp2),fp4);			   //내용에 저장
		bbs.posi=new_posi;
		bbs.num=new_num;
		//새제목 저장 ->fp3
		fwrite(&bbs,sizeof_bbs,1,fp3);
		//새로운 위치지정
		new_posi+=bbs.size;
		new_num++;
	}

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	file_move(filename3,filename1);
	file_move(filename4,filename2);
	unlink(SORT_FILE);
	if (disp_proc)
		printf("-> %s%8dK종료\n",now_str(13),new_posi/1024);
}


//추천관련정보
typedef struct {
	int date;
	int rec;
} DATA;

int comp3(DATA *data1,DATA *data2) {
  return (data1->date-data2->date);
}




