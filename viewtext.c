////////////////////////////////////////////////////////////////////////
//���� : �輺��  xhost@xhost.co.kr (016-320-7882)
// ���뺸��
///////////////////////////////////////////////////////////////////////////
#include "util.c"

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fp;
    int line,i,n,mode,tline,key;
	char buff[10010];
	char s[10],c;
	char id[100];
	char logo_file[100];
	char text_file[100];
	strcpy(id,argv[1]); 				//���̵�
	strcpy(logo_file,argv[2]);				//�����ڵ�
	strcpy(text_file,argv[3]);				//�����ڵ�
	read_pf(id);
    read_cfg();
    
    findport(tty); //������Ʈ ã��
	set_tmpfile();		 //�ӽ�ȭ�������ϱ�
	if ((fp=fopen(text_file,"r+"))==NULL) return;
	cls();
	view(logo_file);
	xy(1,5);
	tline=0;
	while ((c=getc(fp))!=EOF) if (c=='\n') tline++;

	line=0;
	mode=20;
	re_view:
	rewind(fp);
	key=No;
	while (fgets(buff,10000,fp)!=NULL) {
		line++;
        view3(buff);
		if (line%mode==0) {
			printf(" ####(P%d/%d) ���[ENTER] ����(Q) �ٽ�(R) ����(S) �����(A) >>",line/mode,tline/mode+1);
			lineinput(s,5);check_han(s);
			if (s[0]=='r'||s[0]=='R') { line=0;rewind(fp);}
			if (s[0]=='a'||s[0]=='A') { line=0;rewind(fp);mode=99999;}
			if (is_char("PpQqXx",s[0])) {key=Yes;break;}
			if (s[0]=='S'||s[0]=='s') mode=99999;
			cls();
			view(logo_file);
			xy(1,5);
		}
	}
	fclose(fp);
	if (!key) {
		printf(" #### ������ ���Դϴ�. [ENTER] �ٽ�(R) �����(A) >>");
		lineinput(s,5);check_han(s);
        if (s[0]=='r'||s[0]=='R') { line=0;rewind(fp);goto re_view;}
        if (s[0]=='a'||s[0]=='A') { line=0;rewind(fp);mode=99999; goto re_view;}
	}
}
