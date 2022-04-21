////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���ôٿ�
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

	getcwd(olddir,sizeof(olddir));		//������ ���丮

	findport(tty); //������Ʈ ã��
	read_cfg();
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�

    if (argc<4) {
        printf("bbs_cp ID Ÿ�� �����ڵ� ���丮\n");
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
            mess("%s ���丮�� ������ ���丮�Դϴ�.",go.dir);
            return;
    }

    sprintf(sid_file,"system/%s.sid",go.code);
    if (is_file(sid_file)&&pf.type<18&&(pf.type<1||!in_file2(sid_file,pf.id))) {
        mess("%s ���� �������� �����ϴ�. (%s)",pf.id,go.code);
        return;
    }

    view("logo/%s.sd1",go.code);
    while(1) {
        if (!view("logo/%s.sd2",go.code)) {
            printf("\n %s ���丮 ���ϸ��� �����ϼ���\n (H,Q,LS,SID,LOG) ���� >>",go.dir);
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
            printf("### %s Ư�����ڰ� ���ԵǾ� �ֽ��ϴ�.\n",filename);
        }
        else if (!is_file(filename2)) {
            printf("### %s ������ �����ϴ�.\n",filename2);
        }
        else {
            sprintf(buff,"logo/%s.sd7",go.code);
            if (is_file(buff)&&!is_file(buff,filename)) {
                printf("### %s �� �㰡���� ���� �����Դϴ�.\n",filename);
                continue;
            }
            sprintf(buff,"logo/%s.sd8",go.code);
            if (is_file(buff)&&is_file(buff,filename)) {
                printf("### %s �� �㰡���� ���� �����Դϴ�.\n",filename);
                continue;
            }
            sys_tem("ls -l %s",filename2);
            if (!view("logo/%s.sd4",go.code)) {
                printf(" 1.ZIP��������  2.TGZ��������  11~19.����ZIP���� 21~29.����TGZ����  0.��� >>");
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
            getcwd(buff1,sizeof(buff1));      //������ ���丮
            chdir(newdir);
            getcwd(buff2,sizeof(buff2));      //������ ���丮
            if (is_same(buff1,buff2)) {mess("���丮 ���� ����");return;}
            if (n==2) {
                sys_tem("tar cfvz %s.tgz *",filename,filename);
                chdir(olddir);              //���� ���丮
                sprintf(buff,"%s/%s.tgz",newdir,filename);
                sz_file(buff);
                sprintf(buffrec,"%s %-8s %2d %s.tgz\n",now_str(2),pf.id,oldn,filename2);
            }
            else if (n==1){
                sys_tem("zip %s.zip *",filename,filename);
                chdir(olddir);              //���� ���丮
                sprintf(buff,"%s/%s.zip",newdir,filename);
                sz_file(buff);
                sprintf(buffrec,"%s %-8s %2d %s.zip\n",now_str(2),pf.id,oldn,filename2);
            }
            else if (n==3){
                chdir(olddir);              //���� ���丮
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
        mess("%s ����Ÿ���Ͼ���",buff);
        return;
    }
    while (fgets(buff,200,fp)!=NULL) {
        del_space(buff);del_enter(buff);del_esc(buff);
        if (strlen(buff)>0&&is_file(buff))
        sys_tem("cp -f '%s' %s",buff,newdir);
    }
    fclose(fp);
}
