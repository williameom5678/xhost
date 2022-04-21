////////////////////////////////////////////////////////////////////////
//개발 : 김성대  xhost@xhost.co.kr (016-320-7882)
// 선택다운
////////////////////////////////////////////////////////////////////////
#include "bbs.h"
main(argc, argv)
int argc;
char *argv[];
{
    char buff[280],buff1[280],buff2[280],buff3[280];
	char filename[200];
    char filename1[200];
    char filename2[200];
    char sid_file[200];
	char olddir[280];
    char tid[80];
    char newdir[280];
    char buffrec[280];
    int i,n,oldn;

	getcwd(olddir,sizeof(olddir));		//현재의 디렉토리

	findport(tty); //접속포트 찾기
	read_cfg();
	set_tmpfile();		 //임시화일지정하기

    if (argc<4) {
        printf("bbs_cp ID 타입 가기코드 디렉토리\n");
        return;
    }
    strcpy(pf.id,argv[1]);
    pf.type=atoi(argv[2]);
    strcpy(go.code,argv[3]);
    strcpy(go.dir,argv[4]);

    if (strlen(go.dir)<1||
        stristr(go.dir,"id/")||
        stristr(go.dir,"/etc/")||
        stristr(go.dir,"system/")) {
            mess("%s 디렉토리는 금지된 디렉토리입니다.",go.dir);
            return;
    }

    sprintf(sid_file,"system/%s.sid",go.code);
    if (is_file(sid_file)&&pf.type<18&&(pf.type<1||!in_file2(sid_file,pf.id))) {
        mess("%s 님은 사용권한이 없습니다. (%s)",pf.id,go.code);
        return;
    }

    view("logo/%s.sd1",go.code);
    while(1) {
        if (!view("logo/%s.sd2",go.code)) {
            printf("\n %s 디렉토리 파일명을 선택하세요\n (H,Q,LS,SID,LOG) 선택 >>",go.dir);
        }
        lineinput(filename,20);check_han(filename);printf("\n");
        del_esc3(filename);
        sprintf(filename2,"%s/%s",go.dir,filename);
        if (strlen(filename)<1) return;
        else if (is_quit(filename)) return;
        else if (pf.type>=18&&is_same(filename,"sid")) xedit(sid_file);
        else if (is_same(filename,"h")) view("logo/%s.sd3",go.code);
        else if (is_same(filename,"LOG")) view_text("user/rec/%s.sdn",go.code);
        else if (is_same(filename,"ls")) {
            sys_tem("ls -l %s > %s",go.dir,TMP_FILE);
            view_text(TMP_FILE);
            unlink(TMP_FILE);
        }
        else if (stristr(filename,".tgz")||
                 stristr(filename,"..")||
                 stristr(filename,";")||
                 stristr(filename,">")||
                 stristr(filename,"*")||
                 stristr(filename,"?")||
                 stristr(filename,"<")||
                 stristr(filename,"\\")||
                 is_same(filename,".")||
                 stristr(filename,"/")) {
            printf("### %s 특수문자가 포함되어 있습니다.\n",filename);
        }
        else if (!is_file(filename2)) {
            printf("### %s 파일이 없습니다.\n",filename2);
        }
        else {
            sprintf(buff,"logo/%s.sd7",go.code);
            if (is_file(buff)&&!is_file(buff,filename)) {
                printf("### %s 는 허가되지 않은 파일입니다.\n",filename);
                continue;
            }
            sprintf(buff,"logo/%s.sd8",go.code);
            if (is_file(buff)&&is_file(buff,filename)) {
                printf("### %s 는 허가되지 않은 파일입니다.\n",filename);
                continue;
            }
            sys_tem("ls -l %s",filename2);
            if (!view("logo/%s.sd4",go.code)) {
                printf(" 1.ZIP압축전송  2.TGZ압축전송  11~19.선택ZIP전송 21~29.선택TGZ전송  0.취소 >>");
            }
            lineinput(buff,3);check_han(buff);printf("\n");n=oldn=atoi(buff);
            if (is_same(buff,"0")||is_quit(buff)) continue;
            sprintf(newdir,"tmp/up%d%s",cfg.hostno,no_spc(tty));check_dir(newdir);
                 if (n>=11&&n<=19) {cp_mode(n-10,newdir);n=1;}
            else if (n>=21&&n<=29) {cp_mode(n-20,newdir);n=2;}
            else if (n>=31&&n<=39) {cp_mode(n-30,newdir);n=3;}
            else {
                sys_tem("cp %s %s/ -f",filename2,newdir);
            }
            getcwd(buff1,sizeof(buff1));      //현재의 디렉토리
            chdir(newdir);
            getcwd(buff2,sizeof(buff2));      //현재의 디렉토리
            if (is_same(buff1,buff2)) {mess("디렉토리 변경 실패");return;}
            if (n==2) {
                sys_tem("tar cfvz %s.tgz *",filename,filename);
                chdir(olddir);              //원래 디렉토리
                sprintf(buff,"%s/%s.tgz",newdir,filename);
                sz_file(buff);
                sprintf(buffrec,"%s %-8s %2d %s.tgz\n",now_str(2),pf.id,oldn,filename2);
            }
            else if (n==1){
                sys_tem("zip %s.zip *",filename,filename);
                chdir(olddir);              //원래 디렉토리
                sprintf(buff,"%s/%s.zip",newdir,filename);
                sz_file(buff);
                sprintf(buffrec,"%s %-8s %2d %s.zip\n",now_str(2),pf.id,oldn,filename2);
            }
            else if (n==3){
                chdir(olddir);              //원래 디렉토리
                sprintf(buff,"%s %s/*",cfg.sz,newdir);
                sys_tem(buff);
                sprintf(buffrec,"%s %-8s %2d %s\n",now_str(2),pf.id,oldn,filename2);
            }
            sys_tem("rm -rf %s",newdir);
            sprintf(filename,"user/rec/%s.sdn",go.code);
            put_line(buffrec,filename);
        }
    }
}


cp_mode(int mode,char *newdir)
{
    FILE *fp;
    char buff[200];
    sprintf(buff,"system/se_down%d.dat",mode%10);
    if ((fp=fopen(buff,"r"))==NULL) {
        mess("%s 데이타파일없음",buff);
        return;
    }
    while (fgets(buff,200,fp)!=NULL) {
        del_space(buff);del_enter(buff);del_esc(buff);
        if (strlen(buff)>0&&is_file(buff))
        sys_tem("cp -f '%s' %s",buff,newdir);
    }
    fclose(fp);
}
