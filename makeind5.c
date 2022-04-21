#include "bbs.h"

int total;
main(argc, argv)
int argc;
char *argv[];
{
	char buff[100];
	int i;
	findport(tty);
	total=0;

	if (argc<2) {
        printf("make all id file...please wait...\n");
        make_indsort_all();
        printf(" %d ID indexing Done!!\n",total);
	}
	else if (argc==2) {
        make_indsort(0,argv[1]);
	}
	else if (argc==3) {
        make_indsort2(0,argv[1]);
	}
}

make_indsort(int mode,char *str)
{
	char buff[100];
	int total=0,i,n,count;
	PF pf2;
	FILE *fp;
	char ind_file[100];
	char ind2_file[100];
	char big[20];

	strlwr(str);
	del_esc(str);del_space(str);del_enter(str);
    if (strlen(str)<1) return;
	strcpy(big,c_han(str));
    sprintf(ind_file ,"id/id_%s.ind",big);
	if ((fp=fopen(ID_FILE ,"r"))==NULL) return;
    touch_file(ind2_file);  //현재 9900개 보다 많은 IND
    make_indsort2(mode,str);
    return;
}




make_indsort2(int mode,char *str)
{
	PF pf2;
	FILE *fp1,*fp2;
	char buff[100];
	char ind_file[100];
	char big[20];
	IND ind;
	int i,n,count;
	strlwr(str);
	del_esc(str);del_space(str);del_enter(str);
    if (strlen(str)<1) return;
	strcpy(big,c_han(str));
	sprintf(ind_file ,"id/id_%s.ind",big);
	set_tmpfile();
	if ((fp1=fopen(ID_FILE ,"r"))==NULL) return;
	if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
	ind.ret='\n';
	count=0;
	n=0;
	while(fread(&pf2,sizeof_pf,1,fp1)) {
		strlwr(pf2.id);
		del_esc(pf2.id);del_space(pf2.id);del_enter(pf2.id);
		if (strcmp(c_han(pf2.id),big)==0) {
			pf2.num=n;
			if (strlen(pf2.id)>=3) {
                sprintf(ind.id,"%-9.9s",pf2.id);
                sprintf(ind.cnum,"%7d",pf2.num);
                ind.ret='\n';
				fwrite(&ind,sizeof_ind,1,fp2);
				total++;
				count++;
			}
		}
		n++;
	}
	if (fp1!=NULL) fclose(fp1);
	if (fp2!=NULL) fclose(fp2);
	c_lock(1,ind_file);
	if (count>0) {
		sys_tem("sort '%s' > '%s'",TMP_FILE,TMP_FILE2);
        file_copy(TMP_FILE2,ind_file);
		unlink(TMP_FILE2);
	}
	unlink(TMP_FILE);
	c_lock(0,ind_file);
}


make_indsort_all(int mode)
{
	PF pf2;
	FILE *fp1,*fp2;
    char buff[100],buff1[100],buff2[100];
	char ind_file[100];
	char big[20];
	IND ind;
	int i,n,count;

    printf("make all id file...please wait...\n");
    strcpy(ind_file ,"id/id_all.ind");
    if (is_file(ind_file)&&!is_old_file(ind_file,5*60)) return;
    touch_file(ind_file);
	set_tmpfile();
	if ((fp1=fopen(ID_FILE ,"r"))==NULL) return;
	if ((fp2=fopen(TMP_FILE,"w"))==NULL) return;
	ind.ret='\n';
	n=0;
    total=0;
	while(fread(&pf2,sizeof_pf,1,fp1)) {
		strlwr(pf2.id);
		del_esc(pf2.id);del_space(pf2.id);del_enter(pf2.id);
        if (strlen(pf2.id)>=3) {
            fprintf(fp2,"%s %d\n",pf2.id,n);
        }
		n++;
        total++;
	}
    fclose(fp1);
    fclose(fp2);
    printf("sort file...please wait...\n");
    sys_tem("sort '%s' > '%s'",TMP_FILE,TMP_FILE2);

    if ((fp1=fopen(TMP_FILE2,"r"))==NULL) return;
    sprintf(ind_file ,"id/id__.ind");
    if ((fp2=fopen(ind_file,"w"))==NULL) return;
    while(fgets(buff,200,fp1)!=NULL) {
        if (strlen(buff)<3) continue;
        sscanf(buff,"%s%s",buff1,buff2);
        sprintf(ind.id,"%-8.8s",buff1);
        sprintf(ind.cnum,"%-6.6s",buff2);
        ind.ret='\n';
        sprintf(buff,"id/id_%s.ind",c_han(ind.id));
        if (!is_same(buff,ind_file)) {
            fclose(fp2);
            fp2=NULL;
            strcpy(ind_file,buff);
            if ((fp2=fopen(ind_file,"w"))==NULL) return;
            printf("IND file  %s  ...please wait...\n",ind_file);
        }
        if (fp2!=NULL) fwrite(&ind,sizeof_ind,1,fp2);
    }
    fclose(fp1);
    if (fp2!=NULL) fclose(fp2);
    strcpy(ind_file ,"id/id_all.ind");
    unlink(ind_file);unlink(TMP_FILE);unlink(TMP_FILE2);
    printf(" %d ID indexing Done!!\n",total);
}
