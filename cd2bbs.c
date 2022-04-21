#include "bbs.h"

#ifndef CD
// �õ�� ȭ�� ���
typedef struct {
	int num;				// �Ϸ� ��ȣ
    char dir[150];          // ���丮
    char filename[50];      // ȭ�ϸ�
    int  filesize;          // ȭ���� ũ��
} CD;
#endif

CD cd;


main(argc, argv)
int argc;
char *argv[];
{
    char buff[300];
    char buff2[300];
    char dir[300];
    char dirmenu[300];
    char dirfile[300];
    char NOW_FILE2[300];
    char NOW_FILED2[300];
    char OLDDIR[400];
    FILE *fp1,*fp2,*fp3,*fp4,*fp5;
    int key,n2;


    if (argc<6) {
        printf("\n����  cd2bbs <CD ���丮> <�Խ��ǵ��丮> <�Խ���ȭ�ϸ�> <���̵�> <�̸�>\n");
        printf("\n����  cd2bbs /cdrom /xhost/menu hello �û� ���\n");
		getchar();
		return;
	}


    strcpy(dir,argv[1]);              //���丮
    strcpy(dirmenu,argv[2]);          //�Խ��� ���丮
    strcpy(dirfile,argv[3]);          //�Խ��� ȭ�ϸ�
    strcpy(pf.id,argv[4]);            //���̵�
    strcpy(pf.name,argv[5]);          //�̸�
    getcwd(OLDDIR,sizeof(OLDDIR));    //������ ���丮

    sprintf(TMP_FILE  ,"%s/%s/TMP__1",OLDDIR,dirmenu);
    sprintf(NOW_FILE  ,"%s/%s/%s.cd" ,OLDDIR,dirmenu,dirfile);
    sprintf(NOW_FILE2 ,"%s/%s/%s.tit",OLDDIR,dirmenu,dirfile);
    sprintf(NOW_FILED2,"%s/%s/%s.txt",OLDDIR,dirmenu,dirfile);

    printf("\n *****       ��� ��ٷ� �ֽʽÿ�..       *****\n");
    printf(" ***** ���� %s�� �а� �ֽ��ϴ�. *****\n",dir);
    if (!is_file(dir)) return;
    sprintf(buff,"bin/cd2bbs.sh %s %s",dir,TMP_FILE);   //������ ���丮�� ��ü �뷮
    system(buff);

    fp1=fopen(TMP_FILE,"r+");
    if (fp1==NULL) return;
    fp3=fopen(NOW_FILE,"w");
    fp4=fopen(NOW_FILE2,"w");
    fp5=fopen(NOW_FILED2,"w");
    n2=1;
    bbs.size=0;
    bbs.posi=0;
    while (fgets(buff,200,fp1)!=NULL) {
        buff2dir(buff);                  //buff->cd�� ��ȯ
        if (cd.filesize>0&&cd.filename[0]!=0) {
            cd.num=n2++;
            fwrite(&cd,sizeof(cd),1,fp3);   //����
            make_good_filename(bbs.filename,cd.filename);
            sprintf(buff,"%s%s",cd.dir,cd.filename);
            sprintf(bbs.title,"%-59.59s",buff);
            bbs.num=cd.num;
            strcpy(bbs.id,pf.id);
            strcpy(bbs.name,pf.name);
            strcpy(bbs.key,"");
            time(&bbs.date);
            bbs.gd=' ';
            bbs.read=0;
            bbs.filesize=cd.filesize;
            sprintf(buff,"���丮: %s\nȭ �� ��: %s\n",cd.dir,cd.filename);
            bbs.size=(int)strlen(buff);
            fseek(fp4,(bbs.num-1)*sizeof_bbs,SEEK_SET);
            fwrite(&bbs,sizeof_bbs,1,fp4);  //��������
            fseek(fp5,bbs.posi,SEEK_SET);
            fwrite(&buff,bbs.size,1,fp5);   //��������
            bbs.posi+=bbs.size;
            printf("���丮: %s ȭ��: %s\n",cd.dir,cd.filename);
        }
    }
    fclose(fp1);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    unlink(TMP_FILE);
}


//buff���� cd�� ����
buff2dir(char *buff)
{
    char buff2[200];
    char buff3[200];
    int i,i2,i3,key;
    char c;

    key=No; //ȭ�ϸ� ����?
    i2=0;
    i3=0;
    strcpy(buff2,"");   //���丮
    strcpy(buff3,"");
    strcpy(cd.dir,"");
    strcpy(cd.filename,"");
    cd.filesize=-1;

    for (i=0;i<(int)strlen(buff);i++) {
        c=buff[i];
        if (key==No&&(c==' '||c=='\t')) {                  //ȭ��ũ�� ��
            cd.filesize=atoi(buff2);
            strcpy(buff2,"");
            i2=0;
            key=Yes;
        }
        else if (key==No&&c!=' '&&c!='\t') {                  //ȭ��ũ�� ��
            buff2[i2]=c;
            buff2[i2+1]=0;
            i2++;
        }
        else if (key==Yes&&c==' '&&c!='\t') {                 //ȭ�ϸ� ���(��ĭó��)
            continue;
        }
        else if (c=='\n'||c=='\r') {
            strcpy(cd.dir,buff2);
            strcpy(cd.filename,buff3);
            break;
        }
        else if (c=='/') {          //���丮 �����϶�
            strcat(buff2,buff3);            //ȭ�ϸ� �ִ°�
            i2=(int)strlen(buff2);          //���丮�� ����;
            buff2[i2]=c;
            buff2[i2+1]=0;
            strcpy(buff3,"");
            i3=0;
        }
        else {                              //�׿� ȭ�ϸ� ó��
            buff3[i3]=c;
            buff3[i3+1]=0;
            i3++;
        }
    }
}


is_file(char *filename)
{
	return (access(filename,0)!=-1);
}


//buff2�� ȭ�ϸ� -> buff1����
make_good_filename(char *buff1,char *buff2)
{
    int i;
    byte c;
    int key;
    char buff3[200];

    //�켱 ȭ�ϸ��� �ΰ��̻��� . ¥����
    key=Yes;
    for (i=0;i<(int)strlen(buff2);i++) {
        c=buff2[i];
        if (key==Yes&&c=='.') key=No;
        else if (key=='.') c='_';
        buff1[i]=c;
        buff1[i+1]=0;
    }

    strcpy(buff3,buff1);
    //�켱 ȭ�ϸ��� Ư������ ¥����
    key=Yes;
    for (i=0;i<(int)strlen(buff3);i++) {
        c=buff3[i];
        if (c==63||c==47||c==59||c==42) c='_';
        buff1[i]=c;
        buff1[i+1]=0;
    }

    //�켱 ȭ�ϸ��� 13�ں��� ū��?
    buff1[12]=0;
}
